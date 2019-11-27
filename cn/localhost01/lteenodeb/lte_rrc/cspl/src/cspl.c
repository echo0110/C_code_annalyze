/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Implementation                          **/
/**                                                                        **/
/****************************************************************************/

#include	<cspl.h>

/** Constants ***************************************************************/
#define	__BUFMAGIC	0xACE12E92
#define	__POOLMAGIC	0xCA98362D

#ifdef CSPL_LEAK_DEBUG
#include <execinfo.h>
#endif

/* SPR 21496 Fix Start */
#define	MINBUF		8
/* SPR 21496 Fix End */
#define	NVARPOOL	27

/* Code corres. to SPR No: 2667 */
#ifdef MEM_OVERWRITE
unsigned char __MBUFPADMAGIC[]={0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x3a}; 
#endif

/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
#define qvAllocEx(size, psize, cErr) qvAllocEX(size, psize, cErr,__func__,__LINE__)
#endif
/*SPR_18125_END*/

enum {
	QSRV_UNKNOWN = 0,
	QSRV_MODULE = 1,
	QSRV_FUNCTION = 2
};

/** Data Types (Implementation Private) *************************************/
typedef	struct	qtimerindex {
	unsigned int	major, minor;
} qtimerindex;

typedef	struct	qpool {
	unsigned long	magic;
	unsigned int	size, nbuf, alloc, peakVal;
	YLIST		chunks;
	YPOOL		list;

	QLOCK		lock;
} qpool;

typedef	struct	qbuf {
	union {
		YPNODE	__header__;
		qpool	*pool;
	} u;

	unsigned char	allocated;
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
    unsigned char *func;
    unsigned int   line;
#endif
/*SPR_18125_END*/
/* SPR 21496 Fix Start */
#ifdef CSPL_LEAK_DEBUG
/* SPR 21496 Fix End */
    int nptrs;
/* Max. No. of frames in Backtrace */
#define MAX_FRAMES_IN_BT 10
    void *buffer[MAX_FRAMES_IN_BT];
#endif
} qbuf;
#ifdef CSPL_LEAK_DEBUG
void qvPrintMemBufferBt(qbuf *q);
void qvPrintMemPoolAllocBuf (qpool   *pool);
#endif

#ifdef LINUX_PC_TEST
U32 qvGetModuleId(void);
extern void pc_test_forward_timer_msg(U32 moduldId,const void* pdata,U8 timeType);
#endif
typedef	struct	qchunk {
	YLNODE	__header__;
	unsigned int	nbuf;
} qchunk;

typedef	struct	qtimer {
	union	{
		YLNODE	table;
		YPNODE	freelist;
	} __header__;

	QMODULE	owner;
	QTIME	expiry;
	QTIME	duration;
	YLIST	*list; /* Timer Table List in the qvars structure*/
	unsigned int
		major, minor, repeat;
	void	*data;
} qtimer;

typedef struct qvcontext {
	YTREE	services;
	unsigned char	loglevel;
	QSYSOP	op;
	QTUNING	tuning;
	void	*key;
	QLOCK	lock;

	QTIME	logtime;

	YLIST	driverloops;

	qpool	*timerpool, *modulepool, *pctlpool;
	qpool	*alloc[NVARPOOL];

	int	(*dispatch)(void *, void *, QMODULE, QMODULE);
	void	*dispatchdata;
/*spr 22474 start*/
    qpool   *qmsgpool;
/*spr 22474 end*/
} qcontext;

typedef	struct	qvars {
	YLNODE	__header__;

	QMODULE	self;

	QTIME	lastexpiry;
	QTIME	now;
	QTIME	nextexpiry;

	unsigned long
		timerbitmap[YMAPSIZE(256*8)];
	YLIST	timertable[8][256]; /*2 Dimensional Array of Timer Lists*/

	YSQUEUE	queue;
	YSQUEUE	hold;
	YLIST	modules;
	YLIST	uninitialized;

	void	(*wakeup)(void *);
	void	*argwakeup;

	struct	{
		QTIMER	timer;
		void	*message;
	} current;

	QLOCK	lock;
	QLOCK	sync;

	QLOCK	lock_mtimer;	/* Lock for handling inter-thread Module Timers.	*/

} qvars;

typedef	struct	qmodule {
	struct {
		YLNODE	init;
		YTNODE	service;
	} anchor;

	unsigned long name;
	qvars	*scheduler;

	const char	
		*identifier;
	void	*(*init)(void *pdata);
	void	(*timerservice)(QTIMER id, void *tdata, void *pdata);
	S32	(*messageservice)(void *mdata, void *pdata);
	void	*data;

	int	(*dispatch)(void *, void *, QMODULE, QMODULE);

	unsigned int
		immediate;

	struct	{
		void	*(*in)(void *, QMODULE *, QMODULE *);
		void	*(*out)(void *, QMODULE *, QMODULE *);
	} filter;

	const void *methods;

	unsigned int	timer_tuning;
	qpool	*timerpool;

	#ifdef MODULE_LOGGING
	unsigned char level; /* CSR-1-3200662 */
    #endif

} qmodule;

typedef	struct	qmessage {
	YSNODE	__header__;

	QMODULE	owner, sender, receiver;
	int	priority;
	unsigned int size;
	YLIST	buffers;
} qmessage;

typedef	struct	qpseg {
	unsigned int	refcount;
	unsigned char	*data;
	unsigned int	size;
	void		(*free)(void *, void *);
	void		*freearg;
} qpseg;

typedef	struct	qpctl {
	YLNODE	__header__;

	qpseg		*segment;
	unsigned char	*start;
	unsigned char	*end;
} qpctl;

/** Global Data *************************************************************/
 static	qcontext	*qd;

static	const QTUNING	__default = {
	0,
	0,
	0
};


#define	QV_GETKEY()	(qvars *)(qd->op.getkey(qd->key))

#define QV_LOCK()   qvLock(qvc->lock)
#define QV_UNLOCK() qvUnlock(qvc->lock)



/** Forward Declarations ****************************************************/
static	void	__qvPendingInits(qvars *);
static	int	__qvGetPriority( const YSNODE *N );
static qmodule	*__qvGetService( unsigned long name, int *cErr);
static	void	__qvTimerStart( qvars *qvc, qtimer *T, const QTIME *duration );
static	QTIME	*__qvNextTime(qvars *qvc, QTIME *T, const QTIME *from, qtimerindex *X);
static qpool	*__qvPoolCreate( unsigned int size, unsigned int nbuf );
static int	__qvPoolExtend( qpool *pool, unsigned int nbuf );
static unsigned int	
		__qvPoolSize(qpool *pool, unsigned int *p_alloc);
static void	*__qvPoolAlloc(qpool *pool);
static void	__qvPoolFree(void *buffer);
static void	__qvWakeup(qvars *driver);

/** Default System Operations ***********************************************/

static	void	*s_malloc( unsigned int size  ){ 
		size=size;
		return 0; }
static	void	s_free( void *m ) {
		m=m;
		 }
static	void	*s_newkey( void ) { return qd->op.malloc(sizeof(void *)); }
static	void	*s_getkey( void *key ) { return *((void **)key); }
static	void	s_setkey( void *key, void *value ) { *((void **)key) = value; }
static	void	*s_newlock( void ) { return 0; }
static	void	s_lock( void *lock ) {lock=lock; }
static	void	s_unlock( void *lock ) {lock=lock; }
static	void	s_destroylock( void *lock ) {lock=lock; }
static	int	s_vprintf( const char *format, va_list A ) { 
		format=format;
		A=A;
		return 0; }
static	int	s_iprintf( const char *format, va_list A ) { 
		format=format;
		A=A;
		return 0; }
static	void	s_abort(void) { *((unsigned char *)0) = 0; } 

#define PUBLICMODULE(m)	((QMODULE)(m))
#define LOCALMODULE(m)	((qmodule *)(m))

static	const void	*__keyof(const YTNODE *N)
{
	return & (__YMEMBEROF( qmodule, anchor.service, N )->name);
}

static	int	__compare(const void *a, const void *b)
{
	if( *((const unsigned long *)a) < *((const unsigned long *)b) ) {
		return -1;
	} else if( *((const unsigned long *)a) > *((const unsigned long *)b) ) {
		return 1;
	} else {
		return 0;
	}

}

/** Library Initialization **************************************************/
/*****************************************************************************
* FUNCTION: qvNewDriver
*
* DESCRIPTION:  
* 			The function allocates an identifier for the driver and 
* 			initializes its data structures. Modules may be attached to this
* 			driver, while the thread itself may be started later.
*
* ARGS:	
* 			None
*
* RETURNS:		
* 			Identifier for the driver.
*
******************************************************************************/
QDRIVER	qvNewDriver( void )
{
	int	cErr = QVERROR_NONE;
	QDRIVER	q;

	q = qvNewDriverEx(&cErr);
	if( cErr == QVERROR_SYS_ALLOC_FAIL )
	{
		qvPanic("Cannot Initialize Driver");
		/* Code for Null check is added to remove errors appeared with klocwork SPR No: 2639 */
		/*return 0;*/
	}

	return (QDRIVER)q;
}
QDRIVER	qvNewDriverEx(int *cErr)
{
	unsigned int	i, j;
	qvars	*q = (qvars *)qd->op.malloc( sizeof(qvars) );

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	if( !q ) {
		if(cErr)
			*cErr = QVERROR_SYS_ALLOC_FAIL;
		return 0;
	}

	ysInit( &(q->queue), __qvGetPriority );
	ysInit( &(q->hold), __qvGetPriority );
	ylInit( &(q->modules) );
	ylInit( &(q->uninitialized) );
	ymInit( q->timerbitmap, 256*8, 1 );

	for( i=0; i<8; i++ ) {
		for( j=0; j<256; j++ ) {
			ylInit( &(q->timertable[i][j]) );
		}
	}

	q->lock = qvNewLock();
	q->lock_mtimer = qvNewLock();
	q->sync = qvNewLock();
	q->self = 0;
	q->current.timer = 0;
	q->current.message = 0;

	q->now.s = q->now.us = 0;
	q->lastexpiry.s = q->lastexpiry.us = 0;
	q->nextexpiry.s = q->nextexpiry.us = 0;

	q->wakeup = 0;
	q->argwakeup = 0;

	qvLock( qd->lock );
	ylPushTail( &(qd->driverloops), (YLNODE *)(q) );
	qvUnlock( qd->lock );

	return (QDRIVER)q;
}
/*****************************************************************************
* FUNCTION: qvDriverStart
*
* DESCRIPTION:  
*			This function attaches 'driver'(qvars) to a thread context. 
*			Function also registers the wakeup function for multithreaded
*			applications.
*			For the single threaded applications set wakeup and arg parameters
*			to 0.
*
* ARGS:	
* 			driver			Driver to be attached to a thread
* 			wakeup			Pointer to wakeup function
* 			arg				Pointer to any data that is needed by the wakeup 
* 							function
*
* RETURNS:		
* 			Identifier for the driver registered with the thread
*
******************************************************************************/
QDRIVER	qvDriverStart(QDRIVER driver, void (*wakeupfn)(void *), void *arg)
{
	/* SPR 20007 : CID 83291 Start*/
	if(!driver)
	{
		return 0;
	}
	/*SPR 20007 : CID 83291 End*/
	qvars	*qvc = (qvars *)driver;
	
	qvLock( qvc->sync );
	qvc->wakeup = wakeupfn;
	qvc->argwakeup = arg;
	/*Attaches the qvars thread structure with the key*/
	qd->op.setkey( qd->key, (void *)qvc );
	qvUnlock( qvc->sync );

	return driver;
}
/*****************************************************************************
* FUNCTION: qvInit
*
* DESCRIPTION:  
* 			Performs the initilization of the CSPL library and threads.
* 			It registers with the OS services, tunes the library and 
* 			allocates memory for various pools.
* 
*
* ARGS:	
* 			t			Specifies the number of buffers to be present in the 
* 						pools to be created for each of timer, modules and
* 						pctl(zero buffer copy mgmt).
* 			op			Pointer to QSYSOP structure specifying OS services 
* 						to be accessed.
*
* RETURNS:		
*
******************************************************************************/
void	qvInit( const QTUNING *t, const QSYSOP *op )
{
	int	cErr = QVERROR_NONE;
	qvInitEx(t, op, &cErr);
	if(cErr == QVERROR_SYS_ALLOC_FAIL)
	{
		qvPanic("Unable to allocate memory for CSPL Context");
		return;
	}
}
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
void displayMemPoolStatsCspl(void) 
{
    int i = 0;
    fprintf (stderr,"Memory Pool Stats\n\n\tSize\tnBuf\tBusy\tPeak\n");
	for( i=0; i<NVARPOOL; i++) {
        if(qd->alloc[i]->nbuf){
        fprintf (stderr,"\t%4u\t%4u\t%4u\t%4u\n",
	        qd->alloc[i]->size,
	        qd->alloc[i]->nbuf,
	        qd->alloc[i]->alloc,
	        qd->alloc[i]->peakVal);
        }
    }
    fprintf (stderr,"\t%4u\t%4u\t%4u\t%4u\n",
	        qd->pctlpool->size,
	        qd->pctlpool->nbuf,
	        qd->pctlpool->alloc,
	        qd->pctlpool->peakVal);

    fprintf (stderr,"\t%4u\t%4u\t%4u\t%4u\n",
	        qd->modulepool->size,
	        qd->modulepool->nbuf,
	        qd->modulepool->alloc,
	        qd->modulepool->peakVal);

    fprintf (stderr,"\t%4u\t%4u\t%4u\t%4u\n",
	        qd->timerpool->size,
	        qd->timerpool->nbuf,
	        qd->timerpool->alloc,
	        qd->timerpool->peakVal);
}
#endif
/*SPR_18125_END*/
void	qvInitEx( const QTUNING *t, const QSYSOP *op, int *cErr)
{
	unsigned int	i;
	unsigned int	size = MINBUF;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	/* Code for Null check is added to remove errors appeared with klocwork SPR No: 2639 */
	if(!op || !(op->malloc))
	{
		qvPanic(" No application is there to initialize!!!");
		return;
	}


	qd = (qcontext *)op->malloc( sizeof(qcontext) );
	if ( !qd ) {
		if(cErr)
			*cErr = QVERROR_SYS_ALLOC_FAIL;
		return;
	}

	if( !t ) {
		t = &__default;
	}

	qd->op.malloc = op->malloc?op->malloc:s_malloc;
	qd->op.free = op->free?op->free:s_free;
	qd->op.newkey = op->newkey?op->newkey:s_newkey;
	qd->op.getkey = op->getkey?op->getkey:s_getkey;
	qd->op.setkey = op->setkey?op->setkey:s_setkey;
	qd->op.newlock = op->newlock?op->newlock:s_newlock;
	qd->op.destroylock = op->destroylock?op->destroylock:s_destroylock;
	qd->op.lock = op->lock?op->lock:s_lock;
	qd->op.unlock = op->unlock?op->unlock:s_unlock;
	qd->op.vprintf = op->vprintf?op->vprintf:s_vprintf;
	qd->op.iprintf = op->iprintf?op->iprintf:s_iprintf;
	qd->op.abort = op->abort?op->abort:s_abort;

	qd->tuning = *t;
	qd->key = qd->op.newkey();

	for( i=0; i<NVARPOOL; i++, size *= 2 ) {
		qd->alloc[i] = __qvPoolCreate( size, 0 );
		if ( !qd->alloc[i] ) {
			if(cErr)
				*cErr = QVERROR_SYS_ALLOC_FAIL;
			return;
		}
	}	

	qd->timerpool = __qvPoolCreate( sizeof(qtimer), t->ntimers );
	if ( !qd->timerpool ) {
		if(cErr)
			*cErr = QVERROR_SYS_ALLOC_FAIL;
		return;
	}
	qd->modulepool = __qvPoolCreate( sizeof(qmodule), t->nmodules );
	if ( !qd->modulepool ) {
		if(cErr)
			*cErr = QVERROR_SYS_ALLOC_FAIL;
		return;
	}
	qd->pctlpool = __qvPoolCreate( sizeof(qpctl), t->npctls );
	if ( !qd->pctlpool ) {
		if(cErr)
			*cErr = QVERROR_SYS_ALLOC_FAIL;
		return;
	}

	qd->loglevel = 255;
	qd->lock = qvNewLock();
	qd->op.setkey( qd->key, 0 );

	ytInit( &(qd->services), __compare, __keyof );
	ylInit( &(qd->driverloops) );

	qd->logtime.s = qd->logtime.us = 0;
	qd->dispatch = 0;
}


/** Access to System Memory Allocation **************************************/
/*****************************************************************************
* FUNCTION: qvSysMalloc
*
* DESCRIPTION:  
* 			Calls Directly the OS specific memory allocation 
* 			routines is for memory allocation.
*
* ARGS:	
* 			size			size of buffer to be allocated
*
* RETURNS:		
* 			Pointer to the memory allocated
*
******************************************************************************/
void	*qvSysMalloc(unsigned int size)
{
	return qd->op.malloc( size );
}
/*****************************************************************************
* FUNCTION: qvSysFree
*
* DESCRIPTION:  
* 			The function calls OS memory free routine directly
* 			for freeing the memory allocated by qvSysMalloc.	
*
* ARGS:	
* 			pointer to the memory to be freed.
*
* RETURNS:		
*
******************************************************************************/
void	qvSysFree( void *p )
{
	qd->op.free( p );
}

/** Memory Pool Management **************************************************/
/*****************************************************************************
* FUNCTION: __qvPoolCreate
*
* DESCRIPTION:  
* 			This function creates a context pool of specified size of buffer
* 			and the number of buffers in the pool.
* 			Used to create default pools at the time of Init, and pools
* 			for modules, timers and pctl pools.
*
* ARGS:	
* 			size				size of each buffer in the pool
* 			nbuf				number of buffers in the pool
*
* RETURNS:		
* 			Pointer to the allocated pool
*
******************************************************************************/
static qpool	*__qvPoolCreate( unsigned int size, unsigned int nbuf )
{
	qpool		*pool;

	/** Allocate the memory **/
	pool = (qpool *)qd->op.malloc( YALIGN(sizeof(qpool),8) );
	if( !pool ) {
		return 0;
	}

	ypInit( &(pool->list) );
	ylInit( &(pool->chunks) );
/*valgrind_fix start */
	pool->peakVal = 0;
/*valgrind_fix start */
	pool->size = size;
	pool->nbuf = 0;
	pool->alloc = 0;
	pool->magic = __POOLMAGIC;
	pool->lock = qvNewLock();


	if( nbuf ) {
		if( !__qvPoolExtend(pool, nbuf) ) {
			qd->op.free( (void *)pool );
			return 0;
		}
	}

	return pool;
}
/*****************************************************************************
* FUNCTION: __qvPoolExtend
*
* DESCRIPTION:  
*			This function extends an existing pool by 'nbuf' number 
*			of buffers. 
* ARGS:	
* 			Q					Pointer to Pool.
* 			nbuf				number of buffers in the pool
*
* RETURNS:			
* 			1 if successful
* 			0 if unsuccessful
*
******************************************************************************/
static int	__qvPoolExtend( qpool *pool, unsigned int nbuf )
{
	qchunk		*chunk;
	unsigned int	size = pool->size;
	unsigned int	realsize, i;
	unsigned char	*p;
	/* Start SPR 5045 Fix */
	double total_size = 0,size_alloc = 0;
	/* End SPR 5045 Fix*/

/* Code corres. to SPR No: 2667*/
#ifdef MEM_OVERWRITE
	unsigned char *r=NULL;
	int actsize=0,j=0,temp=0;;
#endif

#ifdef MEM_DEBUG
	if ( nbuf > 0)
		nbuf = 1;
#endif
	/** Compute the memory required **/
	realsize = YALIGN(sizeof(qbuf),8) + YALIGN(size,8);
	
	/* Start SPR 5045 Fix */
	size_alloc= nbuf * (double)realsize;
	total_size= YALIGN(sizeof(qchunk),8) + size_alloc ;
	if (total_size > MAX_VAL_U32)
	{
	  return 0;
	}
	/* End SPR 5045 Fix*/

	/** Allocate the memory **/
	/* Memory is allocated/extended in chunks of nbuf buffers.
	 * This list of chunks and actual buffers is maintained in 
	 * the pool.
	 */
	p = (unsigned char *)qd->op.malloc
			( YALIGN(sizeof(qchunk),8) + (nbuf * realsize) );
	
	if( !p ) 
	{
		return 0;	/** Failed **/
	}

	chunk = (qchunk *)(void *)p;
	chunk->nbuf = nbuf;

	qvLock( pool->lock );

	ylPushTail( &(pool->chunks), (YLNODE *)chunk );
	for( i=0, p += YALIGN(sizeof(qchunk),8); i<nbuf; i++, p += realsize ) {
		qbuf	*q = (qbuf *)(void *)p;

		q->allocated = 0;

/* Code corres. to SPR No: 2667 */
#ifdef MEM_OVERWRITE
		r = (unsigned char *)((unsigned char *)q)+(YALIGN(sizeof(qbuf),8)+(YALIGN(sizeof(qmessage),8)));
		    actsize=size-YALIGN(sizeof(qmessage),8);
			temp=(YALIGN((actsize),8)-actsize);
			for( j=0; j<(YALIGN((actsize),8)-actsize); j++) 
			{	  
		  	
				r[actsize+j]= __MBUFPADMAGIC[j];	
						
			}
#endif	
		ypPut( &(pool->list), (YPNODE *)q );
	}
	pool->nbuf += nbuf;

	qvUnlock( pool->lock );
	return 1;
}
/*****************************************************************************
* FUNCTION: __qvPoolSize
*
* DESCRIPTION:  
* 			This function provides the size of pool in terms of number 
* 			of buffers in the pool. If the second argument is
* 			not null, it is set to the number of buffers already 
* 			allocated in the pool.
* ARGS:	
* 			Q					Pointer to Pool.
* 			p_alloc				Out param, returns number of buffers
* 								already allocated in the pool.
*
* RETURNS:			
*			Size of the pool
******************************************************************************/
static unsigned int	__qvPoolSize(qpool *pool, unsigned int *p_alloc)
{
	unsigned int	nbuf, nalloc;

	qvLock( pool->lock );
	nbuf = pool->nbuf;
	nalloc = pool->alloc;
	qvUnlock( pool->lock );

	if( p_alloc ) {
		*p_alloc = nalloc;
	}
	return nbuf;
}
/*****************************************************************************
* FUNCTION: __qvPoolAlloc
*
* DESCRIPTION:  
*			This function allocates a buffer from the pool.
* ARGS:	
* 			pool					Pointer to Pool.
*
* RETURNS:			
*			Pointer to the buffer allocated
******************************************************************************/
static void	*__qvPoolAlloc(qpool *pool)
{
	void	*p;
#ifdef MEM_DEBUG
	void	*k;
#endif

	qvLock( pool->lock );
	if( (p = ypGet(&pool->list)) != 0 ) {
		pool->alloc++;
	}
	qvUnlock( pool->lock );

	if( p ) {
		unsigned char	*c;
		qbuf	*q = (qbuf *)p;

		q->allocated = 1;
		q->u.pool = pool;
#ifdef CSPL_LEAK_DEBUG
                q->nptrs = backtrace(q->buffer, MAX_FRAMES_IN_BT);
#endif
		c = (unsigned char *)q;
		c += YALIGN(sizeof(qbuf),8);

        if(pool->alloc > pool->peakVal )
        pool->peakVal = pool->alloc;
        
		return (void *)c;
	} else {
#ifdef MEM_DEBUG
		if( !__qvPoolExtend(pool, 1) ) {
			return 0;
		}
		k = __qvPoolAlloc(pool);
		return k;
	}
#else	
		return 0; 
	}
#endif
}
/*****************************************************************************
* FUNCTION: __qvPoolFree
*
* DESCRIPTION:  
* 			This function frees the buffer, which was previously 
* 			allocated from a pool. The buffer is returned back to 
* 			the list of free buffers of pool to which it belonged.
* ARGS:	
* 			buffer			Pointer to the buffer to be freed.
*
* RETURNS:			
*
******************************************************************************/
static void	__qvPoolFree(void *buffer)
{
	unsigned char	*c = (unsigned char *)buffer;
	qbuf	*q;
	qpool	*pool;

	c -= YALIGN(sizeof(qbuf),8);
	q = (qbuf *)(void *)c;
	pool = q->u.pool;

#ifdef CSPL_LEAK_DEBUG
    if(0 == q->allocated)
	{
        fprintf(stderr,"########## Double Free detected ##########\n");
        fprintf(stderr,"First Free done by :- \n");
        /* print backtrace for double free */
        qvPrintMemBufferBt(q);
        fprintf(stderr,"Second Free done by :- \n");
      /* print current backtrace*/
        qvPrintMemCurrentBt();
        return;
	}
#else
	qvAssert( q->allocated );
#endif

	q->allocated = 0;
#ifdef CSPL_LEAK_DEBUG
    q->nptrs = backtrace(q->buffer, MAX_FRAMES_IN_BT);
#endif

	qvLock( pool->lock );
#ifdef MEM_DEBUG
	c -= YALIGN(sizeof(qchunk),8);
	ylDelete( &(pool->chunks), (YLNODE *)c);
	qd->op.free(c);
#else
	ypPut( &(pool->list), (YPNODE *)q );
#endif
	pool->alloc--;
#ifdef MEM_DEBUG
	pool->nbuf--;
#endif
	qvUnlock( pool->lock );
}

/*****************************************************************************
* FUNCTION: __qvPoolFreeEx
*
* DESCRIPTION:  
* 			This function frees the buffer, which was previously 
* 			allocated from a pool. The buffer is returned back to 
* 			the list of free buffers of pool to which it belonged.
* ARGS:	
* 			buffer			Pointer to the buffer to be freed.
* 			cErr            Error value to be returned if any
*
* RETURNS:			
*
******************************************************************************/
static void	__qvPoolFreeEx(void *buffer,int *Err)
{
	unsigned char	*c = (unsigned char *)buffer;
	qbuf	*q;
	qpool	*pool;

	c -= YALIGN(sizeof(qbuf),8);
	q = (qbuf *)(void *)c;
	pool = q->u.pool;

	qvAssert( q->allocated );

    if(0 == q->allocated)
    {
        *Err = QVERROR_MEM_UNALLOCATED_BUFFER; 
        return;
    }

	q->allocated = 0;

	qvLock( pool->lock );
#ifdef MEM_DEBUG
	c -= YALIGN(sizeof(qchunk),8);
	ylDelete( &(pool->chunks), (YLNODE *)c);
	qd->op.free(c);
#else
	ypPut( &(pool->list), (YPNODE *)q );
#endif
	pool->alloc--;
#ifdef MEM_DEBUG
	pool->nbuf--;
#endif
	qvUnlock( pool->lock );
}
/** Allocation from system pools ********************************************/
/*****************************************************************************
* FUNCTION: NEWTIMER
*
* DESCRIPTION:  Allocate a timer from timerpool. 
* 				If short of memory , extend the pool size
*
* ARGS:		module		module identifier.
*				
* RETURNS:		Pointer to the timer buffer allocated from timer pool.
*
******************************************************************************/
static	qtimer *NEWTIMER(QMODULE module)
{
	qmodule	*qm = (qmodule*) module;

	if( qm && qm->timerpool )
	{
		void	*p = __qvPoolAlloc( qm->timerpool );

		if( !p && !qm->timer_tuning ) {
			qpool	*q = (qpool *)qm->timerpool; 
			unsigned int n = __qvPoolSize( q, 0 );

			__qvPoolExtend( q, n?n:1 );
			p = __qvPoolAlloc( qm->timerpool );
		}

		return (qtimer *)p;
	}
	else
	{
		void	*p = __qvPoolAlloc( qd->timerpool );

		if( !p && !qd->tuning.ntimers ) {
			qpool	*q = (qpool *)qd->timerpool; 
			unsigned int n = __qvPoolSize( q, 0 );

			__qvPoolExtend( q, n?n:1 );
			p = __qvPoolAlloc( qd->timerpool );
		}

		return (qtimer *)p;
	}
}
/*****************************************************************************
* FUNCTION: NEWPCTL
*
* DESCRIPTION:  Allocate a zero cpy buffer from pctl pool. 
* 				If short of memory , extend the pool size
*
* ARGS:	
*				None
* RETURNS:		Pointer to the buffer allocated from pool.
*
******************************************************************************/
static	qpctl *NEWPCTL(void)
{
	void	*p = __qvPoolAlloc( qd->pctlpool );

	if( !p && !qd->tuning.npctls ) {
		qpool	*q = (qpool *)qd->pctlpool;
		unsigned int n = __qvPoolSize( q, 0 );

		__qvPoolExtend( q, n?n:1 );
		p = __qvPoolAlloc( qd->pctlpool );
	}

	return (qpctl *)p;
}
/*****************************************************************************
* FUNCTION: NEWMODULE
*
* DESCRIPTION:  Allocate a module buffer from module pool. 
* 				If short of memory , extend the pool size
*
* ARGS:	
*				None
* RETURNS:		Pointer to the buffer allocated from pool.
*
******************************************************************************/
static	qmodule	*NEWMODULE(void)
{
	void	*p = __qvPoolAlloc( qd->modulepool );

	if( !p && !qd->tuning.nmodules ) {
		qpool	*q = (qpool *)qd->modulepool;
		unsigned int n = __qvPoolSize( q, 0 );

		__qvPoolExtend( q, n?n:1 );
		p = __qvPoolAlloc( qd->modulepool );
	}

	if( p ) {
		((qmodule *)p)->identifier = 0;
		((qmodule *)p)->scheduler = 0;
		((qmodule *)p)->init = 0;
		((qmodule *)p)->timerservice = 0;
		((qmodule *)p)->messageservice = 0;
		((qmodule *)p)->data = 0;
		((qmodule *)p)->dispatch = 0;
		((qmodule *)p)->immediate = 0;
		((qmodule *)p)->filter.in = 0;
		((qmodule *)p)->filter.out = 0;
		((qmodule *)p)->methods = 0;
		((qmodule *)p)->timer_tuning = 0;
		((qmodule *)p)->timerpool = 0;
	}

	return (qmodule *)p;
}

/** Malloc Emulation ********************************************************/
/*****************************************************************************
* FUNCTION: qvAlloc
*
* DESCRIPTION:  
* 			This function allocates memory of size specified by the 
* 			first parameter to the function. The second parameter is
* 			optional, if passed it returns the actual amount of memory 
* 			allocated. The memory allocation is done from the pools that
* 			CSPL maintains. These pools are of block sizes from 32 bytes
* 			to 2Gb. Memory allocation request is serviced from the pool 
* 			of smallest block size that will fit the requested size.
*
* ARGS:	
* 			size			size of the buffer needed
* 			psize			actual size of buffer returned
* RETURNS:		
* 			Pointer to the buffer allocated from any of the various pools.
*
******************************************************************************/
void	*qvAlloc(unsigned int size, unsigned int *psize)
{
	return qvAllocEx(size, psize, 0);
}
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
void	*qvAllocEX(unsigned int size, unsigned int *psize, int *cErr, unsigned char *func, unsigned int line)
#else
void	*qvAllocEx(unsigned int size, unsigned int *psize, int *cErr)
#endif
{
	unsigned int	i, n;
#ifdef CSPL_LEAK_DEBUG_LITE
    qbuf *_buf = NULL;
#endif
/*SPR_18125_END*/

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	for( i=0, n=MINBUF; i<NVARPOOL; i++, n *= 2 ) {
		if ( size <= n ) {
			qpool	*q = (qpool *)qd->alloc[i];
		    void	*p = __qvPoolAlloc( q );
			if( p == 0 ) {
				unsigned int nbuf = __qvPoolSize(q, 0);
				__qvPoolExtend( q, nbuf?nbuf:1 );
				p = __qvPoolAlloc( q );
			}

			if ( !p )
			{
				if(cErr)
					*cErr = QVERROR_SYS_ALLOC_FAIL;
			}
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
            if(p)
            {
                _buf = p - YALIGN(sizeof(qbuf),8); 
                _buf->func = func;
                _buf->line = line;
            }
#endif
/*SPR_18125_END*/
			if( p && psize ) {
				*psize = q->size;
			}
			return p;
		}
	}
	/* 
	 * No pool available for the required bufsize! 
	 */
	if(cErr)
		*cErr = QVERROR_MEM_ALLOC_ILLEGAL_SIZE;

	return 0;  
}
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
void qvPrintMemPoolAllocBuffers ()
{
    qpool               *pool;
    qchunk                             *chunk;
    unsigned int   realsize, i,j;
    unsigned char                *p;
    unsigned long long  leakMemory = 0;
    unsigned int        bufferLeaked = 0;

    for( j=0; j< NVARPOOL; j++ ) 
    {
        pool = qd->alloc[j];
        fprintf(stderr, "MEMORY POOL SIZE [%u] BUFFERS LEAK SUMMARY:\n", pool->size);
        realsize = YALIGN(sizeof(qbuf),8) + YALIGN(pool->size,8);
        chunk = (qchunk *)ylFirst(&pool->chunks);
        while(chunk)
        {
            p =(unsigned char*) chunk;
            for( i=0, p += YALIGN(sizeof(qchunk),8); i<chunk->nbuf; i++, p += realsize ) {
                qbuf      *q = (qbuf *)(void *)p;

                if ((q->allocated != 0) && (q->func)){
                    fprintf(stderr, "\tbuffer addr = %p  FUNC [%s] LINE [%u]\n",q, q->func, q->line);
                    bufferLeaked++;
                    leakMemory += pool->size;
                }
            }
            chunk = (qchunk *)ylNext((const YLNODE *)chunk);
        }
    }

    fprintf(stderr, "\n\nSUMMARY:\n\n\tTOTAL MEMORY LEAKED [%lld] in BUFFERS [%u]\n",
            leakMemory, bufferLeaked);
}
#endif
/*SPR_18125_END*/

/*****************************************************************************
* FUNCTION: qvFree
*
* DESCRIPTION:  
* 			This function frees the buffer, which was previously 
* 			allocated from CSPL pools. 
* 			The buffer is returned back to 
* 			the list of free buffers of pool to which it belonged.
* ARGS:	
* 			buffer			Pointer to the buffer to be freed.
*
* RETURNS:			
*
******************************************************************************/
void	qvFree( void *p )
{
	__qvPoolFree( p );
}

/*****************************************************************************
* FUNCTION: qvFreeEx
*
* DESCRIPTION:  
* 			This function frees the buffer, which was previously 
* 			allocated from CSPL pools. 
* 			The buffer is returned back to 
* 			the list of free buffers of pool to which it belonged.
* ARGS:	
* 			buffer			Pointer to the buffer to be freed.
* 			cErr            Error value to be returned if any
*
* RETURNS:			
*
******************************************************************************/
void	qvFreeEx( void *p,int *cErr )
{
	__qvPoolFreeEx( p,cErr );
}

/** Packet Buffer Management ************************************************/
#define	HEADROOM(c)	((unsigned int)((c)->start - (c)->segment->data))
#define	TAILROOM(c)	((unsigned int)(((c)->segment->data + (c)->segment->size) - (c)->end))
#define	ONLY(c)		(((c)->segment->refcount == 1) && !(c)->segment->free)
#define	SIZE(c)		((unsigned int)((c)->end - (c)->start))
#define	EMPTY(c)	(SIZE((c)) == 0)
/*****************************************************************************
* FUNCTION: __qvMsgFreeSeg
*
* DESCRIPTION:  
* 			This function frees the segment  when the reference count 
* 			reaches zero. 
* ARGS:	
* 			segment			Pointer to the segment
* 			
* RETURNS:			
*
******************************************************************************/
static	void	__qvMsgFreeSeg( qpseg *segment )
{
	segment->refcount--;
	if( !segment->refcount ) {
		if( !segment->free ) {
			qvFree( segment );
		} else {
			segment->free( segment->data, segment->freearg );
			qvFree( segment );
		}
	}
}
/*****************************************************************************
* FUNCTION: __qvMsgSplitCtl
*
* DESCRIPTION:  
* 			Split c present in q, at the offset and return pointer
			to the first splitted block. c now points to second 
			split block
* ARGS:	
* 			q 		Pointer to the message
* 			c		Pointer to the qpctl buffer to be split
* 			offset	Position at which c is to be split
* 			
* RETURNS:			
*			Pointer to the first splitted block			
*
******************************************************************************/
static	qpctl	*__qvMsgSplitCtl( qmessage *q, qpctl *c, int offset, int *cErr)
{
	qpctl *c_prev;

	qvAssert( offset );
	qvAssert( offset < (c->end - c->start) );

	/*c_prev shall point to the first splitted block*/
	c_prev = NEWPCTL();
	if( !c_prev ) {
		if(cErr)
		{
			*cErr = QVERROR_PCTL_ALLOC_FAIL;
		}
		return 0; 
	}
	/*Make c_prev point to the same segment*/
	c_prev->segment = c->segment;

	/*increment  number of pctls refering to this segment*/
	c->segment->refcount++;
	
	c_prev->start = c->start;
	
	/*End of the first splitted block*/
	c_prev->end = c->start + offset;

	/*Start of the second splitted block*/
	c->start = c_prev->end;

	/*Insert c_prev before c in the ylist of q*/
	ylInsertBefore( &(q->buffers), (YLNODE *)c, (YLNODE *)c_prev );
	return c_prev;
}

/*****************************************************************************
* FUNCTION: __qvMsgLocate
*
* DESCRIPTION:  
* 			This function is called to get the offset of 'location' present in 
* 			the message pointed by q
* ARGS:		
* 			q			Pointer to the message
* 			location	Nth location whose offset is to be determined
* 			poffset		OutParam, returns the offset of location
*
* RETURNS:			
* 			Pointer of the node which has the 'location'
*
******************************************************************************/
static	qpctl	*__qvMsgLocate( const qmessage *q, unsigned int location,
				unsigned int *poffset, int *cErr)
{
	unsigned int	here = 0;
	qpctl	*c;

	if( location < q->size ) {
		c = (qpctl *)ylFirst( &(q->buffers) );
		while( c && (location >= (here + (c->end - c->start))) ) {
			here += (c->end - c->start);
			c = (qpctl *)ylNext( (YLNODE *)c );
		}
		*poffset = location - here;
		return c;
	} else {
		if(cErr)
		{
			*cErr = QVERROR_MEM_ILLEGAL_LOCATION;
		}
		return 0;
	}
}
/*****************************************************************************
* FUNCTION: __qvMsgDetachSeg
*
* DESCRIPTION:  
* 			This function removes the zero cpy buffer from q and adjusts the
* 			size of q appropriately
* ARGS:		
* 			q			Pointer to the message
* 			c			Pointer to the qpctl buffer to be deleted
*
* RETURNS:			
*
******************************************************************************/
static	void	__qvMsgDetachSeg( qmessage *q, qpctl *c )
{
	ylDelete( &(q->buffers), (YLNODE *)c );
	q->size -= (c->end - c->start);
	__qvMsgFreeSeg(c->segment);
	__qvPoolFree( c );
}
/*****************************************************************************
* FUNCTION: __qvMsgAddNewSeg
*
* DESCRIPTION:  
*			This function adds the new allocated segment to the message.
* ARGS:	
* 			q			Pointer to the message
* 			before 		Node in segments' list before which new seg 
* 						is to be added
* 			size		Size of data in new segment
* 			headroom	Size of memory to be allocated before the payload
* 			tailroom	Size of memory to be allocated after the payload
* 			
*
* RETURNS:			
* 			Pointer to the qpctl buffer added in q
*
******************************************************************************/
static	qpctl	*__qvMsgAddNewSeg( qmessage *q, 
		qpctl *before, unsigned int size, 
		unsigned int headroom, unsigned int tailroom,
		int *cErr )
{
	unsigned int	
		realsize, 
		n = YALIGN(sizeof(qpseg),8) + headroom + tailroom + size;
	qpctl	*c;
	qpseg	*s;
	unsigned char	*p;

	c = NEWPCTL();
	if ( !c ) {
		if(cErr)
		{
			*cErr = QVERROR_PCTL_ALLOC_FAIL;
		}
		return 0;
	}

	p = (unsigned char *)qvAllocEx( n, &realsize, cErr); 
	if ( !p ) {						/* Error code set inside */
		__qvPoolFree ( c );
		return 0;
	}

	s = (qpseg *)(void *)p;
	
	s->data = p  + YALIGN(sizeof(qpseg),8);/*Point to start of data*/
	s->size = realsize - (s->data - p);/*total size minus size of qpseg hdr*/

	c->segment = s;
	c->start = s->data + headroom;
	c->end = c->start + size;
	s->refcount = 1;

	s->free = 0; s->freearg = 0;

	if( !before ) {
		ylPushTail( &(q->buffers), (YLNODE *)c );
	} else {
		ylInsertBefore( &(q->buffers), (YLNODE *)before, (YLNODE *)c );
	}
	q->size += size;
	return c;
}
/*****************************************************************************
* FUNCTION: __qvMsgAddNewSegExternal
*
* DESCRIPTION:  
* 			This function is used for inserting external data
* 			at a specified location in a message
* ARGS:	
* 			q			Pointer to the message
* 			before 		Node in segments' list before which new seg 
* 						is to be added
* 			size		Size of data in ext buffer
* 			extbuf		Pointer to the external buffer to be added as segment
*			free		Pointer to the function used to free the memory
*						block
*			freearg		Pointer to the argument to be passed to free function			
*
* RETURNS:			
* 			Pointer to the qpctl buffer added in q
*
******************************************************************************/
static	qpctl	*__qvMsgAddNewSegExternal( qmessage *q, 
		qpctl *before, unsigned int size, unsigned char *extbuf,
		void (*freefn)(void *, void *), void *freearg,
		int *cErr )
{
	qpctl	*c;
	qpseg	*s;

	c = NEWPCTL(); 
	if ( !c ) {
		if(cErr)
		{
			*cErr = QVERROR_PCTL_ALLOC_FAIL;
		}
		return 0;
	}

	s = (qpseg *)qvAllocEx( sizeof(qpseg), 0, cErr);
	if ( !s )
	{
		__qvPoolFree ( c );
		return 0;
	}
	
	s->data = extbuf;
	s->size = size;

	c->segment = s;
	c->start = s->data;
	c->end = c->start + size;
	s->refcount = 1;

	s->free = freefn; s->freearg = freearg;

	if( !before ) {
		ylPushTail( &(q->buffers), (YLNODE *)c );
	} else {
		ylInsertBefore( &(q->buffers), (YLNODE *)before, (YLNODE *)c );
	}
	q->size += size;
	return c;
}
/*****************************************************************************
* FUNCTION: qvMsgPoolCreate
*
* DESCRIPTION:  
*			This function creates a message pool. 
* ARGS:	
* 			size				size of each buffer in the pool
* 			nbuf				number of buffers in the pool
*
* RETURNS:		
* 			Pointer to the allocated pool
*
******************************************************************************/
QMPOOL	qvMsgPoolCreate( unsigned int size, unsigned int nbuf )
{
	return qvMsgPoolCreateEx(size, nbuf, 0);
}
QMPOOL	qvMsgPoolCreateEx( unsigned int size, unsigned int nbuf, int *cErr)
{
	QMPOOL	qm_pool;
	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	size += YALIGN(sizeof(qmessage),8);
	qm_pool = (QMPOOL)__qvPoolCreate( size, nbuf );
	if (!qm_pool)
	{
		if(cErr)
			*cErr = QVERROR_SYS_ALLOC_FAIL;
	}

	return qm_pool;
}
/*****************************************************************************
* FUNCTION: qvMsgPoolSize
*
* DESCRIPTION:  
* 			This function provides the size of pool in terms of number 
* 			of buffers in the pool. If the second argument is
* 			not null, it is set to the number of buffers already 
* 			allocated in the pool.
* ARGS:	
* 			Q					Pointer to Pool.
* 			p_alloc				Out param, returns number of buffers
* 								already allocated in the pool.
*
* RETURNS:			
* 			Size of the pool
*
******************************************************************************/
unsigned int	qvMsgPoolSize( QMPOOL Q, unsigned int *p_alloc )
{
	return __qvPoolSize( (qpool *)Q, p_alloc );
}
/*****************************************************************************
* FUNCTION: qvMsgPoolExtend
*
* DESCRIPTION:  
*			This function extends an existing pool by 'nbuf' number 
*			of buffers. 
* ARGS:	
* 			Q					Pointer to Pool.
* 			nbuf				number of buffers in the pool
*
* RETURNS:			
* 			1 if successful
* 			0 if unsuccessful
*
******************************************************************************/
int	qvMsgPoolExtend( QMPOOL Q, unsigned int nbuf )
{
	return qvMsgPoolExtendEx(Q, nbuf, 0);
}
int	qvMsgPoolExtendEx( QMPOOL Q, unsigned int nbuf, int *cErr)
{
	int	p;
	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	p = __qvPoolExtend( (qpool *)Q, nbuf );
	if ( !p ) {
		if(cErr)
			*cErr = QVERROR_SYS_ALLOC_FAIL;
	}

	return p;
}

/*****************************************************************************
* FUNCTION: qvCsplPoolsExtend
*
* DESCRIPTION: 	
*			Extension function for each of the definite pool sizes created  
*			at the time of initialisation in rvInit.
*
* ARGS:		
*			RTUNING *t		Tuning parameter to provide values by which current
*							existing pools need to be extended
*			int *cErr		Pointer to errorcode
*
* RETURNS:		0 on Failure
*			1 on Success
*
******************************************************************************/
int	qvCsplPoolsExtend( QTUNING *t, int *cErr)
{
	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	
	if( !t ) {
		if(cErr)
			*cErr = QVERROR_INVALID_ARGUMENT;
		return 0;       
	}
	
	if( t->ntimers > 0 ) {
		if(!__qvPoolExtend( qd->timerpool, t->ntimers ) ) {
			if(cErr)
				*cErr = QVERROR_SYS_ALLOC_FAIL;
                        return 0;
		}
	}
	if( t->nmodules > 0 ) {
		if(!__qvPoolExtend( qd->modulepool, t->nmodules ) ) {
			if(cErr)
				*cErr = QVERROR_SYS_ALLOC_FAIL;
                        return 0;
		}
	}
	if( t->npctls > 0 ) {
		if(!__qvPoolExtend( qd->pctlpool, t->npctls ) ) {
			if(cErr)
				*cErr = QVERROR_SYS_ALLOC_FAIL;
                        return 0;
		}
	}

	return 1;
}


/*****************************************************************************
* FUNCTION: qvMsgAlloc
*
* DESCRIPTION:  
*			This function allocates a buffer for a message from the pool.
* ARGS:	
* 			pool					Pointer to Pool.
*
* RETURNS:			
* 			Pointer to the buffer allocated
*
******************************************************************************/
void	*qvMsgAlloc(QMPOOL pool, unsigned int headroom, unsigned int tailroom, unsigned int attrsize)
{
	return qvMsgAllocEx(pool, headroom, tailroom, attrsize, 0);
}
void	*qvMsgAllocEx(QMPOOL pool, unsigned int headroom, unsigned int tailroom, unsigned int attrsize, int *cErr)
{
	qmessage *q;
	qvars	*qvc = QV_GETKEY();
	qpctl	*c;
	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	if( pool ) {
		if( attrsize ) {
			attrsize += YALIGN(sizeof(qmessage),8);
			if( ((qpool *)pool)->size < attrsize ) {
				if(cErr)
					*cErr = QVERROR_MEM_ALLOC_ILLEGAL_SIZE;
				return 0;
			}
		}
		q = (qmessage *)__qvPoolAlloc( (qpool *)pool );
		if ( !q ) {
			if(cErr)
				*cErr = QVERROR_MEM_POOL_EXHAUSTED;
			return 0;
		}
	} else {
		attrsize += YALIGN(sizeof(qmessage),8);
		q = (qmessage *)qvAllocEx(attrsize, 0, cErr); /* Error code set inside */
		if( !q ) {
			return 0;
		}
	}

	ylInit( &(q->buffers) );
	q->size = 0;
	q->owner = qvc?qvc->self:0;
	q->sender = q->receiver = 0;

	if( headroom || tailroom ) {
		c = __qvMsgAddNewSeg( q, 0, 0, headroom, tailroom, cErr);
		if ( !c )
		{
			__qvPoolFree( q );
			return 0;
		}
	}
	return	(void *)((unsigned char *)q + YALIGN(sizeof(qmessage),8));
}
/*****************************************************************************
* FUNCTION: qvMsgFree
*
* DESCRIPTION:  
* 			This function requests de-allocation of memory pointed
* 			by the buffer argument. This function is used in conjunction
* 			with qvMsgAlloc.
* ARGS:	
* 			buffer			Pointer to the buffer to be freed.
*
* RETURNS:			
*
******************************************************************************/
void	qvMsgFree(void *buffer)
{
	qmessage	*q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage),8));
	qpctl	*c, *c_del;

	/*Free all the zero cpy buffers associated with q*/
	for( c = (qpctl *)ylFirst( &(q->buffers) ); c ; ) {
		c_del = c;
		c = (qpctl *)ylNext( (YLNODE *)c );

		ylDelete( &(q->buffers), (YLNODE *)c_del );
		__qvMsgFreeSeg( c_del->segment );
		__qvPoolFree( c_del );
	}

	q->owner = q->sender = q->receiver = 0;
	/*Free q, and thus its non-zero cpy buffer*/
	__qvPoolFree( q );
}

/*****************************************************************************
* FUNCTION: qvMsgFreeEx
*
* DESCRIPTION:  
* 			This function requests de-allocation of memory pointed
* 			by the buffer argument. This function is used in conjunction
* 			with qvMsgAlloc.
* ARGS:	
* 			buffer			Pointer to the buffer to be freed.
* 			cErr            Error value to be returned if any
*
* RETURNS:			
*
******************************************************************************/
void	qvMsgFreeEx(void *buffer,int *cErr)
{
	qmessage	*q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage),8));
	qpctl	*c, *c_del;

	/*Free all the zero cpy buffers associated with q*/
	for( c = (qpctl *)ylFirst( &(q->buffers) ); c ; ) {
		c_del = c;
		c = (qpctl *)ylNext( (YLNODE *)c );

		ylDelete( &(q->buffers), (YLNODE *)c_del );
		__qvMsgFreeSeg( c_del->segment );
	    __qvPoolFreeEx( c_del,cErr );
	}

	q->owner = q->sender = q->receiver = 0;
	/*Free q, and thus its non-zero cpy buffer*/
    __qvPoolFreeEx( q,cErr );
}
/*****************************************************************************
* FUNCTION: qvMsgSize
*
* DESCRIPTION:  
* 			This function returns the size of buffer. 'attrsize'
* 			will be set to the size of memory that was actually allocated at
* 			the time of qvMsgAlloc call. While the function will return 
* 			the size of payload attached to the message.
* ARGS:	
* 			buffer			Pointer to the buffer whose size is to be
* 							retrieved
* 			attrsize		OutParam, returns the size of 				
*
* RETURNS:			
* 			Size of the buffer data present in the message
*
******************************************************************************/
unsigned int	qvMsgSize(const void *buffer, U32 *attrsize)
{
	const qmessage	*q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage),8));

	if( attrsize ) {
		qbuf	*m;
		m = (qbuf *)(void *)((const unsigned char *)q - YALIGN(sizeof(qbuf),8));
		/*Non-zero cpy buffer, allocated from pool of specific size*/
		*attrsize = m->u.pool->size - YALIGN(sizeof(qmessage),8);
	}

	return q->size;/*Size of zero cpy buffers*/
}
/*****************************************************************************
* FUNCTION: qvMsgReserve
*
* DESCRIPTION:  
* 			This function can be used for reserving a specified 
* 			area of memory in a message buffer. 
* ARGS:	
* 			buffer		Pointer to the message
* 			where		Nth byte where data is to be reserved
* 			count		Size of buffer to be reserved
* RETURNS:			
* 			Pointer to the start of reserved area
*
******************************************************************************/
unsigned char	*qvMsgReserve(void *buffer, int where, unsigned int count)
{
	return qvMsgReserveEx(buffer, where, count, 0);
}
unsigned char	*qvMsgReserveEx(void *buffer, int where, unsigned int count, int *cErr)
{
	qmessage	*q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage),8));
	qpctl	*c, *c_prev;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	if( where && ((unsigned int)where == q->size) ) {
		where = -1;
	}

	if( where == 0 ) { /*Reserve at the start*/
		c = (qpctl *)ylFirst( &(q->buffers) );
		if( !c ) {
			c = __qvMsgAddNewSeg( q, 0, count, 0, 0, cErr);
			if ( !c ) {
				return 0;
			}
			return c->start;
		} else if (ONLY(c) && (HEADROOM(c) >= count)) {
			c->start -= count;/*Accomodate headroom to reserve space*/
			q->size += count;
			return c->start;
		} else {/*allocate and place memory block before c*/
			c = __qvMsgAddNewSeg( q, c, count, 0, 0, cErr );
			if ( !c ) {
				return 0;
			}
			return c->start;
		}
	} else if ( where == -1 ) {/*Reserve at the end*/
		c = (qpctl *)ylLast( &(q->buffers) );
		if( !c ) {
			c = __qvMsgAddNewSeg( q, 0, count, 0, 0, cErr );
			if ( !c ) {
				return 0;
			}
			return c->start;
		} else if(ONLY(c) && (TAILROOM(c) >= count)) {
			unsigned char	*r = c->end;

			c->end += count;
			q->size += count;
			return r;
		} else {
			c = __qvMsgAddNewSeg( q, 0, count, 0, 0, cErr );
			if ( !c ) {
				return 0;
			}
			return c->start;
		}
	} else {
		unsigned int	offset;

		c = __qvMsgLocate( q, where, &offset, cErr);
		if( !c ) {
			return 0;
		}

		c_prev = (qpctl *)ylPrev((YLNODE *)c);

		if( offset ) {
			if( !__qvMsgSplitCtl( q, c, offset, cErr) ) {
				return 0;
			}

			c = __qvMsgAddNewSeg( q, c, count, 0, 0, cErr );
			if ( !c ) {
				return 0;
			}
			return c->start;
			
		} else if(ONLY(c) && (HEADROOM(c) >= count)) {
			c->start -= count;
			q->size += count;
			return c->start;
		} else if( c_prev && ONLY(c_prev) && (TAILROOM(c_prev) >= count) ) {
			unsigned char	*r = c_prev->end;

			c_prev->end += count;
			q->size += count;
			return r;
		} else {
			c = __qvMsgAddNewSeg( q, c, count, 0, 0, cErr );
			if ( !c ) {
				return 0;
			}
			return c->start;
		}
	}
}
/*****************************************************************************
* FUNCTION: qvMsgInsert
*
* DESCRIPTION:  
* 			This function is used for inserting data
* 			at a specified location in a message
* ARGS:	
* 			buffer		Pointer to the message
* 			where		Nth byte where data is to be inserted
* 			data		Pointer to the external buffer to be added as segment
* 			count		Size of data in ext buffer
* RETURNS:			
* 			Pointer to data which was inserted.
*
******************************************************************************/
unsigned char	*qvMsgInsert(void *buffer, int where, unsigned char *data, unsigned int count)
{
	return qvMsgInsertEx(buffer, where, data, count, 0);
}
unsigned char	*qvMsgInsertEx(void *buffer, int where, unsigned char *data,
				unsigned int count, int *cErr)
{
	unsigned char	*P;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	P = qvMsgReserveEx( buffer, where, count, cErr);
	if( P ) {
		memcpy(P, data, count );
		return P;
	} else {
		return 0;
	}
}
/*****************************************************************************
* FUNCTION: qvMsgInsertExternal
*
* DESCRIPTION:  
* 			This function is used for inserting external data
* 			at a specified location in a message
* ARGS:	
* 			buffer		Pointer to the message
* 			where		Nth byte where data is to be inserted
* 			data		Pointer to the external buffer to be added as segment
* 			count		Size of data in ext buffer
*			free		Pointer to the function used to free the memory
*						block
*			freearg		Pointer to the argument to be passed to free function			
*
* RETURNS:			
* 			Pointer to data which was inserted.
*
******************************************************************************/
unsigned char	*qvMsgInsertExternal(
		void *buffer, int where, 
		unsigned char *data, unsigned int count, 
		void (*freefn)(void *, void *), void *freearg)
{
	return qvMsgInsertExternalEx(buffer, where, data, count, freefn, freearg, 0);
}
unsigned char	*qvMsgInsertExternalEx(
		void *buffer, int where, 
		unsigned char *data, unsigned int count, 
		void (*freefn)(void *, void *), void *freearg,
		int *cErr)
{
	qmessage	*q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage),8));
	qpctl	*c;
	
	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	if( where && ((unsigned int)where == q->size) ) {
		where = -1;
	}

	if( where == 0 ) {/*Insert data at start*/
		c = (qpctl *)ylFirst( &(q->buffers) );
		if( !c ) {
			c = __qvMsgAddNewSegExternal( q, 0, count, data, freefn, freearg, cErr);
			if ( !c ) {
				return 0;
			}
		} else {
			c = __qvMsgAddNewSegExternal( q, c, count, data, freefn, freearg, cErr);
			if ( !c ) {
				return 0;
			}
		}
	} else if ( where == -1 ) {/*Insert data at the end*/
		c = __qvMsgAddNewSegExternal( q, 0, count, data, freefn, freearg, cErr);
			if ( !c ) {
				return 0;
			}
	} else {
		unsigned int	offset;

		c = __qvMsgLocate( q, where, &offset, cErr);/*Find offset of 'where'*/
		if( !c ) {/*c is the node which has the 'where'*/
			return 0;
		}

		if( offset ) {
			/*Split c present in q, at the offset and return pointer
			 * to the first splitted block. c now points to second 
			 * split block*/
			if( !__qvMsgSplitCtl( q, c, offset, cErr) ) {
				return 0;
			}
			/*Add data before c, which is the second split block*/
			c = __qvMsgAddNewSegExternal( q, c, count, data, freefn, freearg, cErr);
			if ( !c ) {
				return 0;
			}
		} else {/*add data before c as where matches start of some 
				 segment in the q*/
			c = __qvMsgAddNewSegExternal( q, c, count, data, freefn, freearg, cErr);
			if ( !c ) {
				return 0;
			}
		}
	}
	return data;
}
/*****************************************************************************
* FUNCTION: qvMsgRemove
*
* DESCRIPTION:  
* 			This function can be used for removing part of data at
* 			a specified location in a message.
* ARGS:		
* 			buffer			Pointer to the message
* 			where			Nth byte from where data is to be removed
* 			count			Size of data to be removed.
*
* RETURNS:		
* 			Number of bytes removed.
*
******************************************************************************/
int	qvMsgRemove(void *buffer, int where, unsigned int count)
{
	return qvMsgRemoveEx(buffer, where, count, 0);
}
int	qvMsgRemoveEx(void *buffer, int where, unsigned int count, int *cErr)
{
	qmessage	*q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage),8));
	qpctl	*c;
	unsigned int	offset, remaining;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	if( where == (-1) ) { /*Remove bytes from the end*/
		where = q->size - count;
	}

	if( count > (q->size - where) ) {/*If count is more than the data
									  available to be removed beyond 'where',
									  set it the maximum possible value*/
		count = q->size - where;
	}

	c = __qvMsgLocate( q, where, &offset, cErr);
	if( !c ) {
		return 0;
	}

	if( offset ) {
		if( !__qvMsgSplitCtl( q, c, offset, cErr) ) {
				return 0;
		}
	}

	remaining = count;
	while( c && remaining ) {
		unsigned int	n = SIZE(c);/*Size of data other than head and tail
									 room*/

		if( n > remaining ) {/*Bytes within one pctl, just adjust pointers
							  and size.*/
			c->start += remaining;
			q->size -= remaining;
			return count;
		} else {/*Bytes to be removed span more than one pctl*/
			qpctl	*c_old = c;
			c = (qpctl *)ylNext( (YLNODE *)c );
			__qvMsgDetachSeg( q, c_old );/*Remove the pctl*/
			remaining -= n;
			if( !remaining )
				return count;
		}
	}
	return 1;
}
/*****************************************************************************
* FUNCTION: qvMsgExtract
*
* DESCRIPTION:  
*			This function can be used for extracting a part of data 
*			at a specified location in a message. 
* 
* ARGS:		
* 			buffer			Pointer to the message
* 			where			Nth byte from where data is to be extracted
* 			data			Pointer of location where extracted data
* 							is copied
* 			count			Size of data to be extracted.
*
* RETURNS:		
* 			Number of bytes extracted.
*
******************************************************************************/
int	qvMsgExtract(const void *buffer, int where, unsigned char *data, unsigned int count)
{
	return qvMsgExtractEx(buffer, where, data, count, 0);
}
int	qvMsgExtractEx(const void *buffer, int where, unsigned char *data,
			unsigned int count, int *cErr)
{
	const qmessage	*q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage),8));
	qpctl	*c;
	unsigned int	offset, remaining;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	if( where == (-1) ) {
		where = q->size - count;
	}

	if( count > (q->size - where) ) {
		count = q->size - where;
	}

	c = __qvMsgLocate( q, where, &offset, cErr);
	if( !c ) {
		return 0;
	}

	remaining = count; 

	while( c && remaining ) {
		if( (SIZE(c) - offset) < remaining ) {
			memcpy( data + count - remaining, c->start + offset, SIZE(c)-offset );
			remaining -= SIZE(c) - offset;
			c = (qpctl *)ylNext( (YLNODE *)c );
			offset = 0;
		} else {
			memcpy( data + count - remaining, c->start + offset, remaining );
			return count;
		}
	}
	return count - remaining;
}
/*****************************************************************************
* FUNCTION: qvMsgPoolOf
*
* DESCRIPTION:  
* 			This function returns the pool to which the buffer belongs.
* ARGS:	
* 			buffer			Pointer to the buffer
*
* RETURNS:			
* 			Pointer to the pool to which the buffer belongs.
*
******************************************************************************/
QMPOOL	qvMsgPoolOf(const void *buffer)
{
	const qbuf	*m;

	if( buffer ) {
		m = (const qbuf *)(void *)((const unsigned char *)buffer 
			- YALIGN(sizeof(qmessage),8) 
			- YALIGN(sizeof(qbuf),8));
		return (QMPOOL)m->u.pool;
	} else {
		return 0;
	}
}

void	*qvMsgClone(const void *buffer)
{
	return qvMsgCloneEx(buffer, 0);
}
void	*qvMsgCloneEx(const void *buffer, int *cErr)
{
	unsigned int	attrsize;
	void	*newbuf;
	const qmessage
		*q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage),8));
	qmessage
		*nq;
	qbuf	*m = (qbuf *)(void *)((const unsigned char *)q - YALIGN(sizeof(qbuf),8));
	qpctl	*p;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	attrsize = m->u.pool->size - YALIGN(sizeof(qmessage),8);
	newbuf = qvMsgAllocEx( m->u.pool, 0, 0, 0, cErr);
	if( !newbuf ) {
		if(cErr)
		{
			*cErr = QVERROR_NONE;
		}
		newbuf = qvMsgAllocEx( 0, 0, 0, attrsize, cErr);
	}

	if( !newbuf ) {
		return 0;
	}

	memcpy( newbuf, buffer, attrsize );
	nq = (qmessage *)(void *)((unsigned char *)newbuf - YALIGN(sizeof(qmessage),8)),
	nq->owner = q->owner;
	nq->sender = q->sender;
	nq->receiver = q->receiver;
	nq->priority = q->priority;
	nq->size = q->size;

	ylInit( &(nq->buffers) );
	for( p = (qpctl *)ylFirst(&(q->buffers)); p; p = (qpctl *)ylNext((YLNODE *)p) ) {
		qpctl	*c = NEWPCTL();
		if ( !c ) {
			qvMsgFree( newbuf );
			if(cErr)
			{
				*cErr = QVERROR_PCTL_ALLOC_FAIL;
			}
			return 0;
		}
		c->segment = p->segment;
		c->start = p->start;
		c->end = p->end;
		c->segment->refcount++;
		ylPushTail( &(nq->buffers), (YLNODE *)c );
	}
	return	newbuf;
}
/*****************************************************************************
* FUNCTION: qvMsgCopy
*
* DESCRIPTION:  
* 			This function is used for creating a copy of message. 
* 			Contents of 'frombuffer' are copied to contents of 'tobuffer'.
* 			Both 'non-zero copy' and 'zero copy' buffers of the message 
* 			are copied. The zero copy buffers, if any, in the 'tobuffer'
* 			are freed prior to the copy operation.
* ARGS:	
* 			tobuffer		pointer to destination buffer for copy operation
* 			frombuffer		pointer to source buffer for copy operation
* 			attrsize		size of 'non-zero copy buffer'
*
* RETURNS:			
* 			Pointer to the 'tobuffer'
*
******************************************************************************/
void	*qvMsgCopy(void *tobuffer, const void *frombuffer, unsigned int attrsize)
{
	return qvMsgCopyEx(tobuffer, frombuffer, attrsize, 0);
}
void	*qvMsgCopyEx(void *tobuffer, const void *frombuffer, unsigned int attrsize, int *cErr)
{
	qmessage
		*qto = (qmessage *)(void *)((unsigned char *)tobuffer - YALIGN(sizeof(qmessage),8));
	const qmessage
		*qfrom = (const qmessage *)(void *)((const unsigned char *)frombuffer - YALIGN(sizeof(qmessage),8));
	qpctl	*p;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	if( ylCount(&(qto->buffers)) != 0 ) {
		while( (p = (qpctl *)ylPopHead(&(qto->buffers))) != 0 ) {
			__qvMsgFreeSeg(p->segment);
			__qvPoolFree( p );
		}
	} else {
		qvAssert( qto->size == 0 );
	}

	qto->owner = qfrom->owner;
	qto->sender = qfrom->sender;
	qto->receiver = qfrom->receiver;
	qto->priority = qfrom->priority;
	qto->size = qfrom->size;
	if( attrsize ) {
		/* Check sizes */
		qbuf	*m;
		m = (qbuf *)(void *)((const unsigned char *)qto - YALIGN(sizeof(qbuf),8));
		/*Non-zero cpy buffer, allocated from pool of specific size*/
		if( attrsize > (m->u.pool->size - YALIGN(sizeof(qmessage),8))  )
		{
			if(cErr)
			{
				*cErr = QVERROR_MEM_ILLEGAL_LOCATION;
			}
			return 0;
		}
		/* Now, its safe to copy on tobuffer. */
		memcpy( tobuffer, frombuffer, attrsize );
	}

	for( p = (qpctl *)ylFirst(&(qfrom->buffers)); p; p = (qpctl *)ylNext((YLNODE *)p) ) {
		qpctl	*c = NEWPCTL();
		if ( !c ) {
			while( (p = (qpctl *)ylPopHead(&(qto->buffers))) != 0 ) {
				__qvMsgFreeSeg(p->segment);
				__qvPoolFree( p );
			}
			if(cErr)
			{
				*cErr = QVERROR_PCTL_ALLOC_FAIL;
			}
			qto->size = 0;
			return 0;
		}
		
		c->segment = p->segment;
		c->start = p->start;
		c->end = p->end;
		c->segment->refcount++;
		ylPushTail( &(qto->buffers), (YLNODE *)c );
	}

	return tobuffer;
}
/*****************************************************************************
* FUNCTION: qvMsgSplit
*
* DESCRIPTION:  
* 			This function can be used for splitting 'frombuffer'
* 			message at 'offset' position.
* 			The buffers after the split position ('offset') are moved 
* 			to 'tobuffer'. The zero copy buffers, if any, in the 
* 			'tobuffer' are freed prior to the moving split buffers.
* ARGS:	
* 			frombuffer		Pointer to message whose buffers are to be spilt
* 			offset			Location at which buffers are to be split
* 			tobuffer		Pointer to destination buffer where buffers 
* 							after the split point are moved
*
* RETURNS:			
* 			Pointer to the tobuffer
*
******************************************************************************/
void	*qvMsgSplit(void *frombuffer, unsigned int offset, void *tobuffer)
{
	return qvMsgSplitEx(frombuffer, offset, tobuffer, 0);
}
void	*qvMsgSplitEx(void *frombuffer, unsigned int offset, void *tobuffer, int *cErr)
{
	qmessage
		*qto = (qmessage *)(void *)((unsigned char *)tobuffer - YALIGN(sizeof(qmessage),8)),
		*qfrom = (qmessage *)(void *)((unsigned char *)frombuffer - YALIGN(sizeof(qmessage),8));
	qpctl	*c, *c_next, *p;
	unsigned int	seg_off;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	c = __qvMsgLocate( qfrom, offset, &seg_off, cErr);/*Find seg_off of 'offset'*/
	if( !c ) {/*c is node which has the offset*/
		return 0;
	}

	if( seg_off ) {
		/* Split c present in qfrom, at the seg_off and return pointer
		 * to the first splitted block. c now points to second 
		 * split block*/
		if( !__qvMsgSplitCtl( qfrom, c, seg_off, cErr) ) {
			return 0;
		}
	}
	/*Free segments present in ylist of qto*/
	if( ylCount(&(qto->buffers)) != 0 ) {
		while( (p = (qpctl *)ylPopHead(&(qto->buffers))) != 0 ) {
			__qvMsgFreeSeg(p->segment);
			__qvPoolFree( p );
		}
	} else {
		qvAssert( qto->size == 0 );
	}

	for( ; c; ) {/*Move all segments present in 'qfrom' from c 
				   and after c, to 'qto'*/
		c_next = (qpctl *)ylNext((YLNODE *)c);

		ylDelete( &(qfrom->buffers), (YLNODE *)c );
		ylPushTail( &(qto->buffers), (YLNODE *)c );
		c = c_next;
	}
	qto->size = (qfrom->size - offset);
	qfrom->size = offset;
	return tobuffer;
}
/*****************************************************************************
* FUNCTION: qvMsgJoin
*
* DESCRIPTION:  
* 			This function is used for joining list of 'zero copy buffers'
* 			in 'append' message to the list of 'zero copy buffers' in 
* 			'buffer' message. 
* 			'Append' remains unaltered
* ARGS:	
* 			buffer			Pointer to message in which the list of buffers
* 							shall be appended
* 			append			Pointer to message whose list of buffers are to
* 							be appended in 'buffer'
*
* RETURNS:			
* 			Pointer to the 'buffer'
*
******************************************************************************/
void	*qvMsgJoin(void *buffer, void *append)
{
	return qvMsgJoinEx(buffer, append, 0);
}
void	*qvMsgJoinEx(void *buffer, void *append, int *cErr)
{
	qpctl	*p;
	unsigned int nbuf=0, count;
	qmessage
		*qto = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage),8)),
		*qfrom = (qmessage *)(void *)((unsigned char *)append - YALIGN(sizeof(qmessage),8));

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	for( p = (qpctl *)ylFirst(&(qfrom->buffers)); p; p = (qpctl *)ylNext((YLNODE *)p) ) {
		qpctl	*c = NEWPCTL();
		if (  !c ) {
			for ( count = nbuf; count; count-- ) {
				p = (qpctl *)ylPopTail( &(qto->buffers));
				__qvMsgFreeSeg( p->segment );
				__qvPoolFree( p );
			}
			if(cErr)
			{
				*cErr = QVERROR_PCTL_ALLOC_FAIL;
			}
			return 0;
		}
		nbuf++;

		c->segment = p->segment;
		c->start = p->start;
		c->end = p->end;
		c->segment->refcount++;
		ylPushTail( &(qto->buffers), (YLNODE *)c );
	}
	qto->size += qfrom->size;
	return	buffer;
}
/*****************************************************************************
* FUNCTION: qvMsgPack
*
* DESCRIPTION:
* 			This function can be used for packing all the payload
* 			buffers attached to a message into a single payload buffer.
* ARGS:	
* 			buffer			Pointer to the buffer
* 			headroom		Size of empty space at the start of the
* 							payload buffer (optional)
* 			tailroom		Size of empty space at the end of the 
* 							payload buffer (optional)		
*	
* RETURNS:			
* 			Pointer to message with packed payload buffer
*
******************************************************************************/
void	*qvMsgPack(void *buffer, unsigned int headroom, unsigned int tailroom)
{
	return qvMsgPackEx(buffer, headroom, tailroom, 0);
}
void	*qvMsgPackEx(void *buffer, unsigned int headroom, unsigned int tailroom, int *cErr)
{
	qmessage	*q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage),8));
	unsigned int	
		realsize, 
		n = YALIGN(sizeof(qpseg),8) + headroom + tailroom + q->size;
	qpctl	*c = NEWPCTL(),
		*d;
	unsigned char	*p;
	qpseg	*s;
	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	if ( !c ) {
		if(cErr)
		{
			*cErr = QVERROR_PCTL_ALLOC_FAIL;
		}
		return 0;
	}

	p = (unsigned char *)qvAllocEx( n, &realsize, cErr);
	if ( !p )
	{
		__qvPoolFree ( c );
		return 0;
	}

	s = (qpseg *)(void*)p;

	s->data = p  + YALIGN(sizeof(qpseg),8);
	s->size = realsize - (s->data - p);

	c->segment = s;
	c->start = s->data + headroom;
	c->end = c->start + q->size;
	s->refcount = 1;

	s->free = 0; s->freearg = 0;

	/*Extract q->size bytes from buffer into c->start*/
	qvMsgExtractEx( buffer, 0, c->start, q->size, cErr);

	/*Free the segregated segments present in the buffer*/
	while( (d = (qpctl *)ylPopHead(&(q->buffers))) != 0 ) {
		__qvMsgFreeSeg(d->segment);
		__qvPoolFree( d );
	}
	/*Add the new packed node to the message*/
	ylPushTail( &(q->buffers), (YLNODE *)c );
	return buffer;
}
/*****************************************************************************
* FUNCTION: qvMsgSegCount
*
* DESCRIPTION:  
* 			This function returns the number of payload buffers 
* 			attached to a message.
* ARGS:	
* 			buffer			Pointer to the message
*
* RETURNS:			
* 			Number of payload buffers attached to the message
*
******************************************************************************/
unsigned int qvMsgSegCount( const void *buffer )
{
	const qmessage	
		*q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage),8));
	return ylCount(&(q->buffers));
}
/*****************************************************************************
* FUNCTION: qvMsgSegNext
*
* DESCRIPTION:  
* 			This function can be used for fetching a payload buffer
* 			attached to a message. 
* ARGS:	
* 			buffer			Pointer to the message
* 			last			Pointer to control structure for payload 
* 							buffer accessed last time
* 			segment			OutParam, returns the data and size of the payload 
*
* RETURNS:			
* 			Pointer to control structure for payload buffer
* 			whose data is copied in 'segment'.
* 			The same can be passed as 'last' in next call of this
* 			function for fetching the next payload buffer.
*
******************************************************************************/
void	*qvMsgSegNext( const void *buffer, void *last, QSEGMENT *segment )
{
	qpctl	*p;

	if( last ) {/*Fetch the payload next to the 'last' which was
				 fetched previously*/
		p = (qpctl *)ylNext((YLNODE *)last);
	} else {/*Fetch the first node in the message*/
		const qmessage *q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage),8));

		p = (qpctl *)ylFirst(&(q->buffers));
	}

	if( p && segment ) {
		segment->base = p->start;
		segment->size = SIZE(p);
	}
	return (void *)p;
}

/** Timer Management ********************************************************/
/*****************************************************************************
* FUNCTION:  qvNow
*
* DESCRIPTION: 
*			This function is used to fetch current value of wall time. 	
* RETURNS:
* 			Pointer to QTIME object whose value is set to walltime.
*
******************************************************************************/
const QTIME	
	*qvNow(QTIME *T)
{
	qvars	*qvc = QV_GETKEY();

	if( !T ) {
		return	&(qvc->now);
	} else {
		*T = qvc->now;
		return T;
	}
}
/*****************************************************************************
* FUNCTION:  __qvTimerBitmapSet
*
* DESCRIPTION: 
* 			This function sets the appropriate bit in the bitmap
*
* RETURNS:
*
******************************************************************************/
static	void	__qvTimerBitmapSet( qvars *qvc, unsigned int major, unsigned int minor )
{
	ymSet( qvc->timerbitmap, 256*major + minor + 1 );
}
/*****************************************************************************
* FUNCTION:  __qvTimerBitmapClear
*
* DESCRIPTION: 
* 			This function clears the appropriate bit in the bitmap
*
* RETURNS:
*
******************************************************************************/
static	void	__qvTimerBitmapClear( qvars *qvc, unsigned int major, unsigned int minor )
{
	ymClear( qvc->timerbitmap, 256*major + minor + 1 );
}
/*****************************************************************************
* FUNCTION:  __qvTimerIndex
*
* DESCRIPTION: This function calculate the row and column number 
* 			of the timerbitmap/timer table to indicate the
* 			location to be set for this timer.
*
* ARGS:	
* 				expiry
* 				now
* 				result			Out Param. Returns the major and minor
* 								value of the array.
*
* RETURNS:
*
******************************************************************************/
static	void	__qvTimerIndex( QTIME *expiry, QTIME *now, qtimerindex *result )
{
	unsigned int	k, n, o;
	QTIME		D;

	/* expiry timer must be greater than current time */
	
	if( qvTimeDiff( expiry, now, 0 ) > 0 ) {
	/*
	 * use XOR operation to find the greatest bit on which 
	 * the two times differ
	 */
		D.s = expiry->s ^ now->s;
		D.us = expiry->us ^ now->us;

		k = yzFfs( D.s );

		if( k ) {
			o = k-1;/* o = MSByte at which the now and expiry differ*/
			o /= 8;
			n = (expiry->s >> (o*8)) & 0x000000FF;/* Value of 
													 the byte which differs*/
			o += 4;  
		} else {
			k = yzFfs( D.us );

			o = k-1;
			o /= 8;
			n = (expiry->us >> (o*8)) & 0x000000FF;
		}
	} else {
		n = (now->us >> 0) & 0x000000FF;
		o = 0;
	}

	result->major = o;
	result->minor = n;
}
/*****************************************************************************
* FUNCTION: __qvTimerStart
*
* DESCRIPTION: Starts a timer. Called from qvTimerStart or 
* 				__qvProcessTimerTable
*
* ARGS:	
* 				qvc				Driver specific data structure
* 				T				Pointer to the timer buffer 
* 								allocated from timer pool
* 				duration		Value for which timer is started
*
* RETURNS:
*
******************************************************************************/
static	void	__qvTimerStart( qvars *qvc, qtimer *T, const QTIME *duration )
{
	QTIME		expiry = *duration;
	qtimerindex	X;

	if( !expiry.s && !expiry.us) {
		expiry.us = 1;
	}

	expiry.s += qvc->now.s;
	expiry.us += qvc->now.us;
	expiry.s += expiry.us/1000000UL;
	expiry.us %= 1000000UL;

	T->expiry = expiry;

	__qvTimerIndex( &expiry, &(qvc->now), &X );

	T->major = X.major; /* MSByte at which the now and expiry differ*/
	T->minor = X.minor; /* Value of the byte which differs*/

	QV_LOCK();
	/* T->List is Timer Table List in the qvars structure(per Thread)*/
	T->list = &(qvc->timertable[X.major][X.minor]);
    
	/*Push the timer in the list in the timer table array*/
	ylPushHead( T->list, (YLNODE *)T );

	/*Set the bit for this timer in the BitMap*/
	__qvTimerBitmapSet( qvc, X.major, X.minor );
	QV_UNLOCK();
}

/*****************************************************************************
* FUNCTION: qvTimerStart
*
* DESCRIPTION: Starts a timer of given duration. 
*
* ARGS:	
* 				duration		Value for which timer is started
* 				data 			If present, it shall be made available
* 								to the timer Handler at the timer's expiry.
*		 		repeat 			If present, timer is cyclic.
*
* RETURNS:		Timer Id
*
******************************************************************************/
QTIMER	qvTimerStart(const QTIME *duration, void *data, int repeat)
{
	return qvTimerStartEx(duration, data, repeat, 0);
}
QTIMER	qvTimerStartEx(const QTIME *duration, void *data, int repeat, int *cErr)
{
	qvars	*qvc = QV_GETKEY();
	qtimer	*T; 

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	/* Allocate timer from timer pool */
	T = NEWTIMER(qvc->self); 
	if (!T) {
		if(cErr)
			*cErr = QVERROR_TIMER_ALLOC_FAIL;
		return (QTIMER)0;
	}

	T->owner = qvc->self; /*Owner of this timer*/
	T->data = data;
	T->duration = *duration;
	T->repeat = repeat;

	__qvTimerStart( qvc, T, duration );

#ifdef LINUX_PC_TEST
	pc_test_forward_timer_msg(qvGetModuleId(),data,1);
#endif
	return (QTIMER)T; /* Return pointer to timer typecaseted as void* */
}
/*****************************************************************************
* FUNCTION: qvTimerStop
*
* DESCRIPTION: 
* 			Stops a timer. 
*
* ARGS:	
* 				T				Timer Id specifying the timer to be stopped
* 				ptime			Returns the value remaining before its expiry.
*
* RETURNS:	
*
******************************************************************************/
void	qvTimerStop(QTIMER T, QTIME *ptime )
{
	qvars	*qvc = QV_GETKEY();
	qtimer	*timer = (qtimer *)T;

	/* 
	 * Shall Assert timer is already stopped as owner of stopped or 
	 * expired timers is zero. Also, only the owner of the timer is 
	 * allowed to stop the timer in this function
	 * */
	qvAssert( timer->owner == qvc->self );

	if( !timer->list ) {
		/** This is an expired timer -- stop it from repeating if it was planning to **/
		timer->repeat = 0;

		if( ptime ) {
			ptime->s = ptime->us = 0;
		}
		return;
	}

	QV_LOCK();
	ylDelete( timer->list, (YLNODE *)timer );
	if( ylCount(timer->list) == 0 ) {
		__qvTimerBitmapClear( qvc, timer->major, timer->minor );
	}
	QV_UNLOCK();
#ifdef LINUX_PC_TEST
	pc_test_forward_timer_msg(qvGetModuleId(),timer->data,0);
#endif
	timer->list = 0;
	timer->owner = 0;
	if( ptime ) {
		if( timer->expiry.us < qvc->now.us ) {
			timer->expiry.us += 1000000UL;
			timer->expiry.s--;
		}
		if( timer->expiry.s < qvc->now.s ) {
			ptime->s = ptime->us = 0;
		} else {
			ptime->s = timer->expiry.s - qvc->now.s;
			ptime->us = timer->expiry.us - qvc->now.us;
		}
	}
	__qvPoolFree( timer );
}

/*****************************************************************************
 * FUNCTION: qvModuleTimerStop
 *
 * DESCRIPTION: To stop timer , which is started by any thread/task.
 *
 * ARGS:
 *              QTIMER           Timer Id(as returned in timer start fn).
 *				QTIME			 Returns back remaining time in time expiry.
 *
 * RETURNS:
 *			0 		in case of invalid timer.
 *		    1		If timer is stopped successfully.
 *
 ******************************************************************************/
int qvModuleTimerStop(QTIMER T, QTIME *ptime, int *cErr)
{
	qtimer	*timer = (qtimer *)T;
	qmodule *module = (qmodule*)timer->owner;
	qvars	*qvc = (qvars*) module->scheduler;
	QTIME	now = qvc->now;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	QV_LOCK();
	if( !timer->list ) 
	{
		/** This is an expired timer **/ 
		if( ptime ) 
		{
			ptime->s = ptime->us = 0;
		}
		
		/** If this timer was a cyclic timer, return successfully **/
        if(timer->repeat)
          {
              timer->repeat = 0;
              QV_UNLOCK();
              return 1;
          }
        else
        {
            if(cErr)
			    *cErr = QVERROR_TIMER_NOT_RUNNING;

		    QV_UNLOCK();
		    return 0;
        }
	}

	ylDelete( timer->list, (YLNODE *)timer );
	if( ylCount(timer->list) == 0 ) 
	{
		__qvTimerBitmapClear( qvc, timer->major, timer->minor );
	}
	QV_UNLOCK();


	timer->list = 0;
	timer->owner = 0;
	if( ptime ) 
	{
		if( timer->expiry.us < now.us ) 
		{
			timer->expiry.us += 1000000UL;
			timer->expiry.s--;
		}
		if( timer->expiry.s < now.s ) 
		{
			ptime->s = ptime->us = 0;
		} 
		else
		{
			ptime->s = timer->expiry.s - now.s;
			ptime->us = timer->expiry.us - now.us;
		}
	}

	__qvPoolFree( timer );
	return 1;
}

/*****************************************************************************
* FUNCTION: qvTimerRunning
*
* DESCRIPTION: 
* 		This function is called to check whether the timer is running 
* 		or it has already expired. User is recommended to use this 
* 		function before stopping the timer by call qvTimerStop 
* 		as an attempt to stop an already stopped/expired timer 
* 		may lead to memory corruption and segmentation fault. 
*
* ARGS:	
* 				T				Timer Id specifying the timer to be checked
*
* RETURNS:	
*
******************************************************************************/
int	qvTimerRunning(QTIMER T)
{
	return ((qtimer *)T)->list ? 1 : 0;
}

/*****************************************************************************
 * FUNCTION: qvModuleTimerStart
 *
 * DESCRIPTION: To start timer from any task of any module
 *
 * ARGS:
 *				QMODULE			 Source Module,whose timer callback function 
 * 								 will be called in case of timer expiry
 *				duration		 Timer duration
 *				data			 Timer data , the user want to associate with
 *								 this timer.
 *				repeat			 value = 1 means timer will run in repeated 
 *								 mode, i.e. timer will started again in case
 *								 of expiry.
 *				wait			 pointer to QWAIT which contains function 
 *								 pointer of walltime function.
 *
 * RETURNS:
 *			QTIMER 		timer id	
 *
 ******************************************************************************/
QTIMER	qvModuleTimerStart(QMODULE module,const QTIME *duration, 
								void *data, int repeat, int *cErr)
{
	qmodule *mod = (qmodule*)module;
	qvars	*qvc_dest = (qvars*) mod->scheduler;
	qvars	*qvc_self = QV_GETKEY();
	qtimer	*T = NEWTIMER(module);
	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

    if (!T) {
		if(cErr)
		{
			*cErr = QVERROR_TIMER_ALLOC_FAIL;
		}
        return (QTIMER)0;
    }

	T->owner = module;
	T->data = data;
	T->duration = *duration;
	T->repeat = repeat;
	
	if( qvc_self == qvc_dest )
	{
		__qvTimerStart( qvc_self, T, duration );
	}
	else
	{
		QTIME	expiry, delta;
		int		ret_val;

		qvLock( qvc_dest->lock_mtimer );
		
		expiry.s = qvc_self->now.s;
		expiry.us = qvc_self->now.us;

		expiry.s += duration->s;
		expiry.us += duration->us;
		expiry.s += expiry.us/1000000UL;
		expiry.us %= 1000000UL;

		ret_val = qvTimeDiff(&expiry, &qvc_dest->now, &delta);
		if (ret_val != 1) {
			delta.s = 0;
			delta.us = 0;
		}
			
		__qvTimerStart( qvc_dest, T, &delta );
		__qvNextTime( qvc_dest, &(qvc_dest->nextexpiry), &(qvc_dest->now), 0 );

		qvUnlock( qvc_dest->lock_mtimer );
		__qvWakeup(qvc_dest);
	}

	return (QTIMER)T;
}

/*****************************************************************************
 * FUNCTION: qvTimerRemaining
 *
 * DESCRIPTION: To get back remaining time for any running timer.
 *
 * ARGS:
 *              QTIMER           Timer Id(as returned in timer start fn).
 *				QTIME			 Returns back remaining time in time expiry.
 *
 * RETURNS:
 *			1 		If there is some time left in expiry.
 *
 ******************************************************************************/
int qvTimerRemaining(QTIMER T,QTIME *rem)
{
	qtimer *t = (qtimer *) T;
	qmodule *module = (qmodule*)t->owner;
	qvars	*qvc = (qvars*) module->scheduler;
	QTIME now = qvc->now;

	return qvTimeDiff(&t->expiry,&now,rem); /** Ideally the function should 
												not return -1 or 0 value */
}

/*****************************************************************************
* FUNCTION: qvTimerData
*
* DESCRIPTION: 
*			This function is called for fetching data set for the
*			timer at the time of starting the timer.
* ARGS:	
* 				T			Timer Id specifying the timer whose data is to 
* 							fetched
*
* RETURNS:	The timer data.
*
******************************************************************************/
void	*qvTimerData(QTIMER T)
{
	return ((qtimer *)T)->data;
}


static	const unsigned long	mask[] = {
	0xFFFFFF00UL,
	0xFFFF0000UL,
	0xFF000000UL,
	0x00000000UL
};
//add by wood for fix bug at 8019.5.20 begin
/*****************************************************************************
* FUNCTION: qvTimerGetRepeatFlag
*
* DESCRIPTION: 
*			This function is called for fetching repeat flag for the
*			timer .
* ARGS:	
* 				T			Timer Id specifying the timer whose repeat flag is to 
* 							fetched
*
* RETURNS:	The timer repeat flag.
*
******************************************************************************/
unsigned int	qvTimerGetRepeatFlag(QTIMER T)
{
	return ((qtimer *)T)->repeat;
}
//add by wood for fix bug at 8019.5.20 begin end
/*****************************************************************************
* FUNCTION: qvTimerDiff
*
* DESCRIPTION: 
*			This function is called for calculating the difference of two
*			times.
* ARGS:	
* 				A, B		Timer Structues specifying the times to be compared
* 				result		Out Param, specifying the difference
*
* RETURNS:	
* 				0			The two times are same
* 				1			A is greater than B
* 				-1			B is greater than A
*
******************************************************************************/
int	qvTimeDiff(const QTIME *A, const QTIME *B, QTIME *result)
{
	if( (A->s == B->s) && (A->us == B->us) ) {
		if (result) {
			result->s = result->us = 0;
		}
		return 0;
	} else if (A->s == B->s) {
		if ( A->us > B->us ) {
			if(result) {
				result->s = 0;
				result->us = A->us - B->us;
			}
			return 1;
		} else {
			if(result) {
				result->s = 0;
				result->us = B->us - A->us;
			}
			return -1;
		}
	} else if (A->s > B->s) {
		if(result) {
			if( B->us > A->us) {
				result->s = A->s - 1 - B->s;
				result->us = A->us + 1000000UL - B->us;
			} else {
				result->s = A->s - B->s;
				result->us = A->us - B->us;
			}
		}
		return 1;
	} else {
		if(result) {
			if( A->us > B->us) {
				result->s = B->s - 1 - A->s;
				result->us = B->us + 1000000UL - A->us;
			} else {
				result->s = B->s - A->s;
				result->us = B->us - A->us;
			}
		}
		return -1;
	}
}
/*****************************************************************************
* FUNCTION: __qvNextTime
*
* DESCRIPTION: 
* 			Calculates the next timer expiry.
*
* ARGS:	
* 				qvc				Driver specific data structure
* 				now				present time.
*
* RETURNS:		
* 				Next Expiry
*
******************************************************************************/
static	QTIME	*__qvNextTime(qvars *qvc, QTIME *T, const QTIME *from, qtimerindex *X)
{
	unsigned int	n,
		major, 
		minor;
	
	QV_LOCK();
	n = ymFirstSet( qvc->timerbitmap );
	QV_UNLOCK();
	if( !n ) {
		return 0;
	}

	*T = *from;
	n--;

	major = n/256;
	minor = n%256;

	if( X ) {
		X->major = major;
		X->minor = minor;
	}

	if (major < 4) {/* First 4 bytes are for micro seconds, 
					   thus it differed in microseconds */
		T->us &= mask[major];/* zero out starting from the 
								byte whose value will be filled in 
								next statement*/
		T->us |= minor << (major*8); /*fill in the value on 
									   zeroed byte */
		return T; /*Next Expiry*/
	}

	major -= 4;
	T->s &= mask[major];
	T->s |= minor << (major*8);
	T->us = 0;
	return T;
}
/*****************************************************************************
* FUNCTION: __qvProcessTimerTable
*
* DESCRIPTION: 
* 			Checks for the next timer expiry. If timer expiry 
* 			taken place, calls the timer handler, else rearranges 
* 			the bitmap tbl.
* 			Called from qvSchedule. 
*
* ARGS:	
* 				qvc				Driver specific data structure
* 				now				present time.
*
* RETURNS:	
*
******************************************************************************/
static	QTIME	*__qvProcessTimerTable(qvars *qvc, const QTIME *now)
{
	qtimerindex	X;
	qtimer		*p;
	YLIST		*L;
	QTIME		*T = &(qvc->nextexpiry);

	for( ; ; ) {
		if( __qvNextTime(qvc, T, &(qvc->lastexpiry), &X) == 0 ) {
			/** No timers **/
			qvc->now = *now;
			__qvPendingInits(qvc); /* clear the pending list 
									  for modules waiting to be 
									  initialised */
			return 0;
		}

		if( qvTimeDiff( now, T, 0 ) < 0 ) { /* T(next expiry) > now */
			/** Too Early **/
			qvc->now = *now;
			__qvPendingInits(qvc);
			return T;		/* return time for next expiry */
		}
		/*Expiry happened*/
		qvc->now = qvc->nextexpiry;
		QV_LOCK();
		__qvTimerBitmapClear( qvc, X.major, X.minor );
		L = &(qvc->timertable[X.major][X.minor]); 
		QV_UNLOCK();

		if( X.major == 0 ) { /*Least byte of microsec*/
			do {/*Get all timers*/
				qmodule		*module;

				QV_LOCK();
				p = (qtimer *)ylPopHead(L);
				if( p == 0 )
				{
					QV_UNLOCK();
					break;
				}

				__qvPendingInits(qvc);

				p->list = 0;
				QV_UNLOCK();

				qvc->self = p->owner;
				qvc->current.timer = (QTIMER)p;
				module = LOCALMODULE( qvc->self );
				if( module->timerservice ) {/*Init by qvModuleAttach*/
					qvUnlock( qvc->lock_mtimer );
#ifdef LINUX_PC_TEST
					pc_test_forward_timer_msg(qvGetModuleId(),p->data,2);
#endif							
					(module->timerservice)((QTIMER)p, p->data, module->data);		
					qvLock( qvc->lock_mtimer );
				}
				qvc->current.timer = 0;
				qvc->self = 0;

				if( p->repeat ) {
					__qvTimerStart( qvc, p, &(p->duration) );
				} else {
					p->owner = 0;
					__qvPoolFree( p );
				}
			}while( 1 );
		} else { /*Get timers, recalculate position in timertbl 
				   and put them back in the list*/
			do {
				QV_LOCK();
				p = (qtimer *)ylPopHead(L);
				if( p == 0 )
				{
					QV_UNLOCK();
					break;
				}

				__qvTimerIndex( &(p->expiry), T, &X );

				p->major = X.major;
				p->minor = X.minor;
				p->list = &(qvc->timertable[X.major][X.minor]);
				ylPushHead( p->list, (YLNODE *)p );
				__qvTimerBitmapSet( qvc, X.major, X.minor );
				QV_UNLOCK();
			} while( 1 );
		}

		qvc->lastexpiry = qvc->now;
		qvc->self = 0;
	}
}

/** Scheduling **************************************************************/
/*****************************************************************************
* FUNCTION: __qvPendingInits
*
* DESCRIPTION: 
*			This function is used to initialize all the pending modules.
* ARGS:	
* 				qvc				Driver specific data structure
*
* RETURNS:	
*
******************************************************************************/
static	void	__qvPendingInits(qvars *qvc)
{
	qmodule	*m;

	while( (m = (qmodule *)ylPopHead(&(qvc->uninitialized))) != 0 ) {
		qvc->self = PUBLICMODULE(m);
		if( m->init ) {
			qvUnlock( qvc->lock_mtimer );
			m->data = (m->init)(m->data);/*Call the static init fn, if present*/
			qvLock( qvc->lock_mtimer );
		} else {
			/** Nothing - and the `data' field stays as it is **/
		}
		ylPushTail(&(qvc->modules), (YLNODE *)m);
		qvc->self = 0;
	}
}
/*****************************************************************************
* FUNCTION: __qvPending
*
* DESCRIPTION: 
*			This function is used to check if any of the input or the 
*			hold queue is empty or not.
*
* ARGS:	
* 				qvc				Driver specific data structure
*
* RETURNS:	
*
******************************************************************************/
static	int	__qvPending(qvars *qvc)
{
	int	n;

	qvLock( qvc->lock );
	n = ysEmpty(&(qvc->queue)) && ysEmpty(&(qvc->hold));
	qvUnlock( qvc->lock );
	return	!n;
}
/*****************************************************************************
* FUNCTION: qvPending
*
* DESCRIPTION: 
*			This function is used to check if any of the input or the 
*			hold queue or the uninitialized queue is empty or not.
*
* ARGS:	
*			driver			Pointer to the driver
* RETURNS:	
*
******************************************************************************/
int	qvPending(QDRIVER driver)
{
	qvars	*qvc = (qvars *)driver;
	int	n;

	qvLock( qvc->lock );
	n = ysEmpty(&(qvc->queue)) && ysEmpty(&(qvc->hold)) && ylEmpty(&(qvc->uninitialized));
	qvUnlock( qvc->lock );
	return	!n;
}
/*****************************************************************************
* FUNCTION: qvModuleAttach
*
* DESCRIPTION: 
* 			This function registers modules with a thread.
* ARGS:	
* 			driver			Id of the thread with which to attach the module
* 			name			Name of the module as present in QMANIFEST str
* 			init			Pointer to the late init function of QMANIFEST 
* 							str. Executed when Module is initialised. As of
* 							now the module is put in the uninitialised list.
* 			msgfn			Pointer to the message handler for this module
* 			tmrfn			Pointer to the timer handler for this module
* 			initarg			Passed as argument to the late init fn. Eventually
* 							becomes the pointer to the context data of the
* 							stack entity.
* 			service			Module Id as present in the QMANIFEST Str
*			timer_tuning		Tuning for the timer pool for this module.
*
* RETURNS:	
*
******************************************************************************/
QMODULE	qvModuleAttach( 
		QDRIVER	driver,
		const char *name,
		void *(*init)(void *), 
		S32 (*msgfn)(void *, void *), 
		void (*tmrfn)(QTIMER, void *, void *), 
		void *initarg,
		unsigned long service,
		int timer_tuning,
		int *cErr)
{
	qvars	*qvc = (qvars *)driver;
	qmodule	*m;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	if( service ) {
		m = __qvGetService( service, cErr);
	} else {
		m = NEWMODULE();
	}

	if( !m ) {
		if(cErr)
		{
			*cErr = QVERROR_MODULE_ALLOC_FAIL;
		}
		return 0;
	}

	qvLock( qvc->lock );
	qvLock( qd->lock );

	m->identifier = name;
	m->init = init;
	m->timerservice = tmrfn;
	m->messageservice = msgfn;
	m->data = initarg;	/** This may be changed by the initializer function **/
	m->dispatch = 0;
	m->immediate = 0;
	m->methods = 0;
    
	#ifdef MODULE_LOGGING
	m->level = QVLOG_NONE  ;   /* CSR-1-3200662 */
	#endif
	
	if(timer_tuning > QV_TPOOL_NONE)
	{
		m->timer_tuning = (unsigned int) timer_tuning;
		m->timerpool = __qvPoolCreate( sizeof(qtimer), timer_tuning );
		if ( !m->timerpool ) {
			if(cErr)
			{
				*cErr = QVERROR_SYS_ALLOC_FAIL;
			}
			qvFree(m);

			qvUnlock( qd->lock );
			qvUnlock( qvc->lock );

			return 0;
		}
	}

	m->scheduler = qvc;
	ylPushTail(&(qvc->uninitialized), (YLNODE *)m);

	qvUnlock( qd->lock );
	qvUnlock( qvc->lock );

	return PUBLICMODULE( m );
}

QMODULE	qvModuleCreate( 
		const char *name,
		void *(*init)(void *), 
		S32 (*msgfn)(void *, void *), 
		void (*tmrfn)(QTIMER, void *, void *), 
		void *initarg,
		unsigned long service)
{
	qvars	*qvc = QV_GETKEY();

	return qvModuleAttach( (QDRIVER)qvc, name, init, msgfn, tmrfn, initarg, service, QV_TPOOL_NONE, 0);
}

void	*qvModuleContext( QMODULE module )
{
	qmodule	*m = (qmodule *)module;

	return m?m->data:0;
}
/*****************************************************************************
* FUNCTION: qvSelf
*
* DESCRIPTION: 
* 			This function returns the identifier for current module.
*
* ARGS:	
* 			None
*
* RETURNS:	Module Id
*
******************************************************************************/
QMODULE	qvSelf( void )
{
	qvars	*qvc = QV_GETKEY();
	return	qvc?qvc->self:0;
}

/*****************************************************************************
* FUNCTION: qvThisDriver
*
* DESCRIPTION: 
*			This function is used for getting identifier for the 
*			current driver (thread).
* ARGS:	
* 			None
*
* RETURNS:	Driver Id
*
******************************************************************************/
QDRIVER	qvThisDriver( void )
{
	return (QDRIVER)QV_GETKEY();
}
/*****************************************************************************
* FUNCTION: qvThisContext
*
* DESCRIPTION: 
* 			This function can be used for getting a pointer to current context.
* ARGS:	
* 			None
*
* RETURNS:	Pointer to the current context
*
******************************************************************************/
void	*qvThisContext( void )
{
	qvars	*qvc = QV_GETKEY();
	qmodule	*m;

	if( !qvc || !(m = LOCALMODULE(qvc->self)) ) {
		return 0;
	}
	return m->data;
}
/*****************************************************************************
* FUNCTION: qvThisMessage
*
* DESCRIPTION: 
*			This function can be used for getting pointer to the 
*			current message from message handler context
* 
* ARGS:	
* 			None
*
* RETURNS:	Pointer to the current message
*
******************************************************************************/
void	*qvThisMessage( void )
{
	qvars	*qvc = QV_GETKEY();
	return	qvc?qvc->current.message:0;
}
/*****************************************************************************
* FUNCTION: qvThisTimer
*
* DESCRIPTION: 
* 			This function can be used for getting identifier for 
* 			current timer from timer handler context. 
* 
* ARGS:	
* 			None
*
* RETURNS:	Pointer to the current timer
*
******************************************************************************/
QTIMER	qvThisTimer( void )
{
	qvars	*qvc = QV_GETKEY();
	return	qvc?qvc->current.timer:0;
}
/*****************************************************************************
* FUNCTION: qvSetMethods
*
* DESCRIPTION: 
* 			Sets the Pointer to the QMANIFEST structure in the qmodule.
* 			The methods specified in the QMANIFEST structure are pack, 
* 			unpack, alloc.
* ARGS:	
*
* RETURNS:	
*
******************************************************************************/
void	qvSetMethods( QMODULE module, const void *m )
{
	qmodule	*q = (qmodule *)module;

	qvAssert(q);
	q->methods = m;
}
/*****************************************************************************
* FUNCTION: qvGetMethods
*
* DESCRIPTION: 
* 			Gets the Pointer to the QMANIFEST structure in the qmodule.
* 			The methods specified in the QMANIFEST structure are pack, 
* 			unpack, alloc.
* ARGS:	
*
* RETURNS:	
*
******************************************************************************/
const void	*qvGetMethods( QMODULE module )
{
	qmodule	*q = (qmodule *)module;

	return q?q->methods:0;
}
/*****************************************************************************
* FUNCTION: qvSchedule
*
* DESCRIPTION: 
* 			It processes all pending initializations, messages and 
* 			timers till there are none left -  then return to the 
* 			caller the next desired wakeup (wall-)time. The function 
* 			qvSchedule is called per thread, and one of the first actions
* 			taken is to retrieve the thread specific data. Then, the 
* 			pending timers are processed with a call to qvProcessTimerTable. 
* 			Then, until the input queue (meant for messages between modules
* 			of a CSPL based entity) and the hold queue (meant for messages
* 			for external entities) are both empty, this function performs 
* 			the following sequence of actions - until input queue is empty, 
* 			dequeue and process message from input queue, then until the 
* 			hold queue is empty, dequeue and process messages from the 
* 			hold queue. After processing each message from the input queue,
* 			any pending initializations are checked for and executed.
*
* ARGS:	
*
* RETURNS:	
*
******************************************************************************/
const QTIME	*qvSchedule( const QTIME *now )
{
	qvars	
		*qvc = QV_GETKEY();
	const	QTIME	
		*N;
	qmessage	
		*p;
	qmodule		
		*module;
	unsigned char	
		*c;
	int	ret_val;

	qvLock(qvc->lock_mtimer);
	
	__qvProcessTimerTable( qvc, now );

	/* While queues are non-empty*/
	while( __qvPending(qvc) ) {
		for( ; ; ) { /*Process all nodes in the input queue*/
			qvLock( qvc->lock );
			p = (qmessage *)ysPopHead(&(qvc->queue));
			qvUnlock( qvc->lock );

			if( !p ) {
				break;
			}

			qvc->self = p->owner;/*Assume the role of message's owner*/

			c = (unsigned char *)p;
			c += YALIGN(sizeof(qmessage),8);

			module = LOCALMODULE(qvc->self);
			if( module->messageservice ) { /*If msg handler initialized, call 
											it else free the message*/
				qvc->current.message = (void *)c;
				qvUnlock(qvc->lock_mtimer);
				ret_val = (module->messageservice)((void *)c, module->data);
				qvLock(qvc->lock_mtimer);
				if( ret_val ) {
					qvMsgFree( (void *)c );
				}
				qvc->current.message = 0;
			} else {
				qvMsgFree( (void *)c );
			}
			/*Initialize the pending modules*/
			__qvPendingInits(qvc);
		}
		qvc->self = 0;

		for( ; ; ) {/*Process all the nodes in the hold queue, meant
					 for processing messages for ext entities*/
			qmodule	*s;

			qvLock( qvc->lock );
			p = (qmessage *)ysPopHead(&(qvc->hold));
			qvUnlock( qvc->lock );

			if( !p ) {
				break;
			}

			c = (unsigned char *)p;
			c += YALIGN(sizeof(qmessage),8);

			if( !p->receiver ) {/*No Receiver specified for this message*/
				if( qd->dispatch ) {/*If Global Dispatch present*/
					if(qd->dispatch((void *)c, qd->dispatchdata, p->sender, p->receiver)) {
						qvMsgFree( (void *)c );
					}
				} else {
					qvPanic("Received message for nothing" );
					qvMsgFree( (void *)c );
				}
			} else {
				s = LOCALMODULE(p->receiver);

				if( !s->dispatch ) {/*If no Dispatch function for this receiver*/
					if( qd->dispatch ) {/*If Global Dispatch present*/
						if(qd->dispatch((void *)c, qd->dispatchdata, p->sender, p->receiver)) {
							qvMsgFree( (void *)c );
						}
					} else {
						qvPanic("Received message for unknown service %lu", s->name );
						qvMsgFree( (void *)c );
					}
				} else if( s->dispatch( (void *)c, s->data, p->sender, p->receiver ) ) {
					qvMsgFree( (void *)c );
				}
			}
		}
	}

	qvAssert( ysEmpty( &(qvc->hold) ) );
	N = __qvNextTime( qvc, &(qvc->nextexpiry), &(qvc->now), 0 );
	qvc->lastexpiry = qvc->now;

	qvUnlock( qvc->lock_mtimer );
	
	return N;/*Return next desired wakeup time*/
}

static void	__qvWakeup(qvars *driver)
{
	qvLock( driver->sync );
	if( driver->wakeup ) {
		driver->wakeup(driver->argwakeup);
	}
	qvUnlock( driver->sync );
}

void	qvWakeup(QDRIVER driver)
{
	qvars	*qvc = QV_GETKEY();

	if( qvc != (qvars *)driver ) {
		__qvWakeup( (qvars *)driver );
	}
}

/** Messaging ***************************************************************/
/*****************************************************************************
* FUNCTION: __qvGetPriority
*
* DESCRIPTION: 
* 			This function returns the priority assigned to the message
* ARGS:	
*
* RETURNS:	
*
******************************************************************************/
static	int __qvGetPriority( const YSNODE *N )
{
	const unsigned char *c = (const unsigned char *)N;
	const qmessage *q;

	q = (const qmessage *)(void *)c;
	return -q->priority;
}
/*****************************************************************************
* FUNCTION: __qvGetService
*
* DESCRIPTION: 
* 			This function returns the pointer to the module structure
* 			identifying the module whose module id is given by name.
* ARGS:	
* 			name			Module id
*
* RETURNS:	
*			Pointer to the module structure
******************************************************************************/
static qmodule *__qvGetService( unsigned long name, int *cErr)
{
	qmodule		*q;
	YTNODE		*N;

	if( !name ) {
		if(cErr)
		{
			*cErr = QVERROR_INVALID_ARGUMENT;
		}
		return 0;
	}

	qvLock( qd->lock );

	N = ytFind( &(qd->services), &name );

	if( N == 0 ) {
		/** Service does not exist yet. So create it! **/
		q = NEWMODULE();
		if ( !q ) {
			if(cErr)
			{
				*cErr = QVERROR_MODULE_ALLOC_FAIL;
			}
/* SPR 11412 Starts */
			qvUnlock( qd->lock );
/* SPR 11412 Ends */
			return 0;
		}

		q->name = name;
		q->scheduler = 0;
		q->identifier = 0;
		q->messageservice = 0;
		q->timerservice = 0;
		q->init = 0;
		q->data = 0;
		q->dispatch = 0;
		q->immediate = 0;
		q->timer_tuning = 0;
		q->timerpool = 0;

		N = &(q->anchor.service);
		ytInsert( &(qd->services), N );
	} else {
		q = __YMEMBEROF( qmodule, anchor.service, N );
	}

	qvUnlock( qd->lock );
	return q;
}
/*****************************************************************************
* FUNCTION: qvGetService
*
* DESCRIPTION: 
* 			This function returns the pointer to the module structure
* 			identifying the module whose module id is given by name.
* ARGS:	
* 			name			Module id
*
* RETURNS:	
*			Pointer to the module structure
******************************************************************************/
QMODULE qvGetService( unsigned long name )
{
	return qvGetServiceEx(name, 0);
}
QMODULE qvGetServiceEx( unsigned long name, int *cErr)
{
	qmodule	*m;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	m = __qvGetService(name, cErr);

	return PUBLICMODULE(m);
}
/*****************************************************************************
* FUNCTION: qvGetServiceName
*
* DESCRIPTION: 
* 			This function returns the module id of the module specified
* 			by the pointer to the qmodule structure
* ARGS:	
* 			service			Pointer to the module
*
* RETURNS:	
* 			Module id
******************************************************************************/
unsigned long qvGetServiceName( QMODULE service )
{
	qmodule	*q = (qmodule *)service;
	unsigned long	name = 0UL;

	qvLock( qd->lock );
	if( q ) {
		name = q->name;
	}
	qvUnlock( qd->lock );

	return name;
}
/*****************************************************************************
* FUNCTION: qvSetFilter
*
* DESCRIPTION: 
* 			This function can be used for defining filter functions for
* 			a module. The 'in' function ensures that all the incoming 
* 			messages meet the requirements of filter. Similarly 'out' 
* 			function ensures that all the outgoing messages meet the 
* 			requirements of filter. 
* ARGS:	
*			module 		Pointer to the module for which filters 
*						are to be set.
*			in			Function for filtering out the incoming messages
*			out			Function for filtering out the outgoing messages
*
* RETURNS:	
******************************************************************************/
void qvSetFilter( 
		QMODULE module, 
		void *(*in)(void *, QMODULE *, QMODULE *), 
		void *(*out)(void *, QMODULE *, QMODULE *) )
{
	qmodule	*q = (qmodule *)module;

	q->filter.in = in;
	q->filter.out = out;
}
/*****************************************************************************
* FUNCTION: qvSetDispatch
*
* DESCRIPTION: 
* 			Registers Dispatch function for each destination
* ARGS:		
* 			name			service number for named destination
* 			dispatch		Function pointer of Dispatch function for each 
* 							destination
* 			data			Pointer to QOUTGOING structure				
*
* RETURNS:	
*
******************************************************************************/
QMODULE qvSetDispatch( unsigned long name, int (*dispatch)(void *, void *, QMODULE, QMODULE), void *data )
{
	return qvSetDispatchEx(name, dispatch, data, 0);
}
QMODULE qvSetDispatchEx( unsigned long name, int (*dispatch)(void *, void *, QMODULE, QMODULE), void *data, int *cErr)
{
	qmodule	*q;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	qvAssert(dispatch);
	if( !name ) {
		qvLock( qd->lock );
		qd->dispatch = dispatch;
		qd->dispatchdata = data;
		qvUnlock( qd->lock );
		return 0;
	}

	q = __qvGetService( name, cErr);
	if ( !q ) {
		return 0;
	}

	qvLock( qd->lock );

	q->scheduler = 0;
	q->identifier = 0;
	q->messageservice = 0;
	q->timerservice = 0;
	q->init = 0;

	q->methods = 0;

	q->data = data;
	q->dispatch = dispatch;
	q->immediate = 0;

	qvUnlock( qd->lock );
	return PUBLICMODULE(q);
}

/*****************************************************************************
* FUNCTION: qvSetImmediate
*
* DESCRIPTION: 
* 			This function call is used to make all 'qvDeliver' calls 
*			to this destination 'immediate', i.e. the message does 
*			not go into the hold queue, and the dispatch function 
*			is called immediately.
*			Please note that the 'destination' cannot be '0' (meaning 
*			'all destinations')-- you must specify each valid 'immediate' 
*			destination.
* ARGS:	
*			name			destination's service name
* 			cErr			To return result/error code of fn.
*
* RETURNS:	
*
******************************************************************************/
void	qvSetImmediate( unsigned long name, int* cErr )
{
	qmodule	*q;
	if(cErr) { *cErr = QVERROR_NONE; }

	if( !(q = __qvGetService( name, cErr ) )  )
		return;

	qvLock( qd->lock );
	q->immediate = 1;
	qvUnlock( qd->lock );
}

/*****************************************************************************
* FUNCTION: qvDeliver
*
* DESCRIPTION: 
* 			This function call is used to deliver the external messages
* 			received to the appropriate module within CSPL context(called 
* 			from qvIncoming, which is called in qvRun after receiving a 
* 			message). 
* 			It can also be called from qvSend to send messages outside(messages
* 			are put in the hold queue which shall be processed in qvSchedule).
* ARGS:	
*			from				sender
* 			to					receiver
* 			priority			priority associated with the message
* 			message				pointer to the message to be delivered
*
* RETURNS:	
*
******************************************************************************/
void	qvDeliver(QMODULE from, QMODULE to, int priority, void *message )
{
	int	cErr = QVERROR_NONE;

	qvDeliverEx(from, to, priority, message, &cErr);
	if( cErr == QVERROR_INVALID_SRV )
	{
		qvPanic("Trying to send message to invalid service");
	}

	return;
}
void	qvDeliverEx(QMODULE from, QMODULE to, int priority, void *message, int *cErr)
{
	qvars		*qvc = QV_GETKEY();	
	qmessage	*qm;
	qmodule		*q;
	unsigned char	*c; 
	qvars	*dest_sched;

	/* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

	/*If "from" is NULL, its the current module. qvDeliver is called from 
	 * qvSend */
	if( !from && qvc ) {
		from = qvc->self;
	}

	if( from && ((qmodule *)from)->filter.out ) {
		message = ((qmodule *)from)->filter.out( message, &from, &to );
		if( !message ) {
			return;
		}
	}

	if( to && ((qmodule *)to)->filter.in ) {
		message = ((qmodule *)to)->filter.in( message, &from, &to );
		if( !message ) {
			return;
		}
	}

	/*Reformatting the message to be put in the Q*/
	c = (unsigned char *)message;
	c -= YALIGN(sizeof(qmessage),8);
	qm = (qmessage *)(void *)c;
	qm->sender = from;
	qm->receiver = to;
	qm->priority = priority;
	qm->owner = to;

	if( !to ) {/*No Destination module given*/
		if( !qvc ) { /*No thread for sender module*/
			if( qd->dispatch ) {
				if(qd->dispatch(message, qd->dispatchdata, from, to)) {
					qvMsgFree( message );
				}
			} else {
				if(cErr)
				{
					*cErr = QVERROR_INVALID_SRV;
				}
				qvMsgFree( message );
				return;
			}
		} else { /*Put the message on the hold queue*/
			qvLock( qvc->lock );
			ysPushTail( &(qvc->hold), (YSNODE *)qm );
			qvUnlock( qvc->lock );
		}
		return;
	}

	q = LOCALMODULE(to);
	dest_sched = q->scheduler;/*Get thread of the destination module*/
	if( q->scheduler == 0 ) {/* no thread defined for receiving module 
								(external module). Called from qvSend*/
		if( !qvc || q->immediate ) {/*if thread is not defined for sending module*/
			if( !q->dispatch ) { /*Same as in qvSchedule*/
				if( qd->dispatch ) {
					if(qd->dispatch(message, qd->dispatchdata, from, to)) {
						qvMsgFree( message );
					}
				} else {
					if(cErr)
					{
						*cErr = QVERROR_INVALID_SRV;
					}
					qvMsgFree( message );
					return;
				}
			} else if(q->dispatch(message, q->data, from, to)) {
				qvMsgFree( message );
			}
		} else {/*if thread is defined for sending module
				 Put the msg is Hold queue to be sent out later*/
			qvLock( qvc->lock );
			ysPushTail( &(qvc->hold), (YSNODE *)qm );
			qvUnlock( qvc->lock );
		}
	} else {
		/* destination thread information exists */
		/* push the msg to be delivered in input queue for
		 * the destination module. Called from qvIncoming*/
		qvLock( dest_sched->lock );
		ysPushTail( &(dest_sched->queue), (YSNODE *)qm );
		qvUnlock( dest_sched->lock );
	
		if( dest_sched != qvc ) {
			__qvWakeup( dest_sched );
		}
	}
}
/*****************************************************************************
* FUNCTION: qvSend
*
* DESCRIPTION: 
* 			This function is used for sending 'message' to 'to' module 
* 			from the current module.
* ARGS:		
*			to			destination module
*			priority	priority assigned to the message
*			message		pointer to the message to be sent.
* RETURNS:	
*
******************************************************************************/
#ifdef LINUX_PC_TEST
//add by wood begin
extern void SEND_TO_TEST(U32 ulMsgId,char *buf,int len);
extern U8 pctest_check_destid_beset(U32 destid);
U32 qvGetModuleId(void)
{
	qmodule *q = (qmodule *)qvSelf();
	return q?q->name:0;
}
//add by wood end
#endif
void	qvSend(QMODULE to, int priority, void *message )
{
#ifdef LINUX_PC_TEST
	qmodule *q = to;
	SEND_TO_TEST(test_get_api_id((U8 *)message),
		(char *)message,test_get_api_buf_size((U8 *)message)); //add by wood
	if (pctest_check_destid_beset(q->name) == 0)
		//no need put into queue,because we don't test other module
		return;
#endif
	qvDeliver( 0, to, priority, message );
	
}
void	qvSendEx(QMODULE to, int priority, void *message, int *cErr)
{
	qvDeliverEx( 0, to, priority, message, cErr);
}

/*****************************************************************************
* FUNCTION: qvFrom
*
* DESCRIPTION: 
* 			This function returns the identifier of the module, which 
* 			sent the 'message'.
* ARGS:		
*			message		pointer to the message to be sent.
* RETURNS:	
* 			Source Module
*
******************************************************************************/
QMODULE	qvFrom( const void *message )
{
	const unsigned char	*c = (const unsigned char *)message;
	const qmessage	*q;

	c -= YALIGN(sizeof(qmessage),8);
	q = (const qmessage *)(void *)c;
	return q->sender;
}
/*****************************************************************************
* FUNCTION: qvTo
*
* DESCRIPTION: 
* 			This function returns the identifier of the module, to 
* 			which the 'message' is sent.
* ARGS:		
*			message		pointer to the message to be sent.
* RETURNS:	
* 			Destination Module
*
******************************************************************************/
QMODULE	qvTo( const void *message )
{
	const unsigned char	*c = (const unsigned char *)message;
	const qmessage	*q;

	c -= YALIGN(sizeof(qmessage),8);
	q = (const qmessage *)(void *)c;
	return q->receiver;
}
/*****************************************************************************
* FUNCTION: qvPriority
*
* DESCRIPTION: 
* 			This function returns the priority of the 'message'.
* ARGS:		
* 			message			message whose priority is to be determined
* RETURNS:	
* 			Priority
*
******************************************************************************/
int	qvPriority( const void *message )
{
	const unsigned char	*c = (const unsigned char *)message;
	const qmessage	*q;

	c -= YALIGN(sizeof(qmessage),8);
	q = (const qmessage *)(void *)c;
	return q->priority;
}

/** Locks *******************************************************************/
/*****************************************************************************
* FUNCTION: qvNewLock
*
* DESCRIPTION: 
*			This function is called for getting a new lock,
*			which internally calls the 'newlock' function
*			defined in QSYSOP structure for getting the new lock.
*
* RETURNS:
* 			Lock
*
******************************************************************************/
QLOCK	qvNewLock( void )
{
	return (QLOCK)qd->op.newlock();
}
/*****************************************************************************
* FUNCTION: qvLock
*
* DESCRIPTION: 
* 			This function is called for activating 
* 			the lock. 'lock' function defined in QSYSOP structure 
* 			is called in turn.
*
* ARGS:	
* 			Lock id returned from qvNewLock
*
* RETURNS:
*
******************************************************************************/
void	qvLock( QLOCK lock )
{
    /* SES-488 Coverity 110286 */
    if(NULL != lock)
    {
        qd->op.lock((void *)lock);
    }
    /* SES-488 Coverity 110286 */
}
/*****************************************************************************
* FUNCTION: qvUnlock
*
* DESCRIPTION: 
* 			This function is called for deactivating 
* 			the lock. 'unlock' function defined in QSYSOP structure 
* 			is called in turn.
*
* ARGS:	
* 			Lock id returned from qvNewLock
*
* RETURNS:
*
******************************************************************************/
void	qvUnlock( QLOCK lock )
{
    /* SES-488 Coverity 110286 */
    if(NULL != lock)
    {
        qd->op.unlock((void *)lock);
    }
    /* SES-488 Coverity 110286 */
}
/*****************************************************************************
* FUNCTION: qvDestroyLock
*
* DESCRIPTION: 
*			This function is called for destroying (free)
*			the lock. 'destroylock' function defined in QSYSOP 
*			structure is called in turn.
* ARGS:		
* 			Lock id returned from qvNewLock
*
* RETURNS:
*
******************************************************************************/
void	qvDestroyLock( QLOCK lock )
{
    /* SES-488 Coverity 110286 */
    if(NULL != lock)
    {
        qd->op.destroylock((void *)lock);
    }
    /* SES-488 Coverity 110286 */
}

/** Tracing *****************************************************************/
static void	__dump( const unsigned char *D, unsigned int count, 
			unsigned long style, int (*printffn)(const char *, ...) )
{
	unsigned long	i,
			base = 0;
			style=style;

	(*printffn)( "-------- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- ----------------\n" );
	for( ; base+16 < count; base += 16 ) {
		(*printffn)( "%08lX ", base );

		for( i=0; i<16; i++ ) {
			(*printffn)( "%02X ", D[base+i] );
		}

		for( i=0; i<16; i++ ) {
			(*printffn)( "%c", isprint(D[base+i])?D[base+i]:'.' );
		}
		(*printffn)( "\n" );
	}

	(*printffn)( "%08lX ", base );
	for( i=base; i<count; i++ ) {
		(*printffn)( "%02X ", D[i] );
	}
	for( ; i<base+16; i++ ) {
		(*printffn)( "   " );
	}
	for( i=base; i<count; i++ ) {
		(*printffn)( "%c", isprint(D[i])?D[i]:'.' );
	}
	for( ; i<base+16; i++ ) {
		(*printffn)( " ");
	}
	(*printffn)( "\n" );
}

static int	__lprint( const char *format, ...)
{
	va_list A;
	int i;

	va_start( A, format );
	i = qd->op.vprintf(format, A );
	va_end( A );
	return i;
}

static	void	__lstamp( void )
{
	qvars	*qvc = QV_GETKEY();
	const char	*name = "None";

	if( qvc ) {
		if ( qvc->self ) {
			name = LOCALMODULE(qvc->self)->identifier;
			if(!name) {
				name = "Unknown";
			}
		}
	} 
	__lprint( "+ %08lX:%08lX [%s] ", qd->logtime.s, qd->logtime.us, name );
}

static	int	__lscan( const char *format )
{
	int	count = 0, length = 0;
	char	c; 
	const char 
		*s = format;

	while( (c = *s++) != 0 ) {
		length++;
		if( c == '\n' ) {
			count++;
		}
	}

	if( !count ) {
		return 0;
	} else if ( length && (format[length-1] == '\n') ) {
		return -count;
	} else {
		return count;
	}
}
/*****************************************************************************
* FUNCTION: qvPrintfVa
*
* DESCRIPTION: 
* 			This function prints the argument list 'A', as per the
* 			format parameter passed to it
* ARGS:	
* 			format			String describing the format to use to 
* 							write the remaining operands
* 			A				argument list to be printed as per 'format'
* RETURNS:
*			Return value of iprintf function
******************************************************************************/
int	qvPrintfVa( const char *format, va_list A)
{
	return qd->op.iprintf(format, A );
}
/*****************************************************************************
* FUNCTION: qvPrintf
*
* DESCRIPTION: 
* 			This function prints the variable number of arguments as per
* 			the format parameter passed to it.
* ARGS:	
* 			format			String describing the format to use to 
* 							write the remaining operands
* RETURNS:
*			Return value of iprintf function
******************************************************************************/
int	qvPrintf( const char *format, ...)
{
	va_list A;
	int i;

	va_start( A, format );
	i = qvPrintfVa( format, A );
	va_end( A );
	return i;
}
/*****************************************************************************
* FUNCTION: qvDump
*
* DESCRIPTION: 
* 			This function is used for logging dump of message
* ARGS:	
* 			D			Pointer to message buffer to be dumped
* 			count 		Length of buffer to be printed
* 			style		style in which string is to be printed
* RETURNS:
*
******************************************************************************/
void	qvDump( const unsigned char *D, unsigned int count, unsigned long style )
{
	style=style;
	__dump( D, count, style, qvPrintf );
}

static	void	__qvLog( const char *label, const char *format, va_list A )
{
	int	n = __lscan(format);

	qvLock( qd->lock );

	__lstamp();
	__lprint(" ** %s ** ", label );

	if( n && (n != -1) ) {
		__lprint("\n");
	}

	qd->op.vprintf(format, A );

	if( n >= 0 ) {
		__lprint("\n");
	}

	qvUnlock( qd->lock );
}
/*****************************************************************************
* FUNCTION: qvLogVa
*
* DESCRIPTION: 
* 			This function is used for logging traces for defined levels
* 			as per the format parameter passed to it. The argument list
* 			to be printed is specified in variable 'A'.
* ARGS:	
* 			level			Trace Level
* 			format			String describing the format to use to 
* 							write the remaining operands
* 			A				argument list to be printed as per 'format'
* RETURNS:
*
******************************************************************************/
int	qvLogVa( int level, const char *format, va_list A)
{
	int i, n;
	
	#ifdef MODULE_LOGGING /* CSR-1-3200662 */
	qmodule *m;
	unsigned char loglevel=qd->loglevel;
	
	if(qvSelf())
    {
      m=LOCALMODULE(qvSelf());
	  loglevel=MAX(qd->loglevel,m->level);
	} 
		 if(loglevel!=QVLOG_LIBRARY && level >= loglevel) {
		return 0;
	}

    #else
	
	if( qd->loglevel < level ) {
		return 0;
	}
	#endif
	
	n = __lscan( format );

	qvLock( qd->lock );

	__lstamp();
	if( n && (n != -1) ) {
		__lprint("\n");
	}

	i = qd->op.vprintf(format, A );

	if( n >= 0 ) {
		__lprint("\n");
	}

	qvUnlock( qd->lock );

	return i;
}

/*****************************************************************************
* FUNCTION: qvLog
*
* DESCRIPTION: 
* 			This function is used for logging traces for defined 
* 			levels as per the format parameter passed to it. 
* ARGS:	
* 			level			Trace Level
* 			format			String describing the format to use to 
* 							write the remaining operands
* RETURNS:
*
******************************************************************************/
int	qvLog( int level, const char *format, ...)
{
	va_list A;
	int i;

	va_start( A, format );
	i = qvLogVa( level, format, A );
	va_end( A );

	return i;
}
/*****************************************************************************
* FUNCTION: qvLogDump
*
* DESCRIPTION: 
* 			This function is used for logging dump of message for 
* 			defined levels. The time of logging and name of logger 
* 			module is also printed for each dump. 
* ARGS:	
* 			level			Trace Level
* 			D				Pointer to message buffer to be dumped
* 			count			Length of buffer to be printed
* 			style			style in which string is to be printed
* RETURNS:
*
******************************************************************************/
void	qvLogDump( int level, const unsigned char *D, unsigned int count, unsigned long style )
{
	#ifdef MODULE_LOGGING /* CSR-1-3200662 */
	qmodule *m;
	unsigned char loglevel=qd->loglevel;
	
	if(qvSelf())
    {
      m=LOCALMODULE(qvSelf());
	  loglevel=MAX(qd->loglevel,m->level);
	} 
		 if(loglevel!=QVLOG_LIBRARY && level >= loglevel) {
		return ;
	}

    #else
	
	if( qd->loglevel < level ) {
		return ;
	}
	#endif

	
	
	qvLock( qd->lock );
	__lstamp();
	__lprint("\n");
	__dump( D, count, style, __lprint );
	qvUnlock( qd->lock );
}
/*****************************************************************************
* FUNCTION: qvLogLevel
*
* DESCRIPTION: 
* 			This function is used for setting the log level as required.
* 			Trace statements with level greater than the set log level
* 			will not be printed.
* ARGS:	
* 			level			Trace Level to be set
* RETURNS:
*
******************************************************************************/
void	qvLogLevel( unsigned char level )
{
	qd->loglevel = level;
}


/* GDB Fix - add Unsigned char qvGetLogLevel( void ) to fix unresolved in L3 module */

/***************************************************************************************************
*
* Function      : unsigned char qvGetLogLevel( void )
*
* Description   : Best guess at what qvGetLogLevel() should do based on qvLogLevel()
*
* Notes         : None
*
***************************************************************************************************/
unsigned char qvGetLogLevel( void )
{
    unsigned char theLogLevel = 0;
    
    if( qd )
    {
        theLogLevel = qd->loglevel;
    }

    return theLogLevel;
}

/*****************************************************************************
* FUNCTION: qvLogTime
*
* DESCRIPTION: 
* 			This function is used for setting the log time. 
* 			This log time will be printed by the relevant log functions. 
* ARGS:	
* 			T			Log time value to be set
* RETURNS:
*
******************************************************************************/
void	qvLogTime( const QTIME *T )
{
	qvLock( qd->lock );
	qd->logtime = *T;
	qvUnlock( qd->lock );
}

/** Error Handling **********************************************************/
/*****************************************************************************
* FUNCTION: qvPanic
*
* DESCRIPTION: 
* 			This function is called when some error condition
* 			occurs and the program must be aborted due to it. 
* 			The function logs the error condition by printing all 
* 			the parameters passed to it (in the printf format). 
* ARGS:	
*			format		String describing the format to use to
*						write the remaining operands
* RETURNS:
*
******************************************************************************/
void	qvPanic( const char *format, ... )
{
	va_list A;

	va_start( A, format );
	qvPrintf( "** PANIC - " );
	qd->op.iprintf( format, A );
	qvPrintf( "\n" );
	va_end( A );

	va_start( A, format );
	__qvLog( "PANIC", format, A );
	va_end( A );

	qvAbort();
}
/*****************************************************************************
* FUNCTION: qvWarning
*
* DESCRIPTION: 
* 			This function is called when some warning condition 
* 			occurs and the user needs to be informed about it. The 
* 			function logs the warning condition by printing all the
* 			parameters passed to it (in the printf format).
* ARGS:	
*			format		String describing the format to use to
*						write the remaining operands
* RETURNS:
*
******************************************************************************/
void	qvWarning( const char *format, ... )
{
	va_list A;

	va_start( A, format );
	qvPrintf( "** WARNING - " );
	qd->op.iprintf( format, A );
	qvPrintf( "\n" );
	va_end( A );

	va_start( A, format );
	__qvLog( "WARNING", format, A );
	va_end( A );
}
/*****************************************************************************
* FUNCTION: qvAbort
*
* DESCRIPTION: 
*			This function is called when the program needs to be aborted
*			due to an erroneous condition
* ARGS:	
*
* RETURNS:
*
******************************************************************************/
void	qvAbort( void )
{
	qd->op.abort();
}


/** Library Debugging *******************************************************/


unsigned int qvGetQMessageSize(void)
{
        return (YALIGN(sizeof(qmessage),8));
}

void    qvMsgPoolDelete( QMPOOL Q )
{
    qpool      *pool = (qpool *)Q;
    qchunk     *chunk;
    QLOCK       lock = pool->lock;

    qvLock( lock );
    while( (chunk=(qchunk *)ylPopHead(&(pool->chunks))) != 0 ) {
        qvSysFree( (void *)chunk );
    }

    qvSysFree( (void *)pool );
    qvUnlock( lock );
}

unsigned int qvMsgPoolBufferSize(QMPOOL Q)
{
        return (((qpool *)Q)->size);
}

void __qvMsgPoolDecrease(QMPOOL Q, unsigned int nbufs)
{
	qpool *q=(qpool *)Q;
	unsigned int i ;
        YLNODE	      *chunk;
        QLOCK       lock = q->lock;

        qvLock( lock );
	for (i=0;i<nbufs;i++) {
              ypGet(&(q->list));
	}
        chunk = ylPopTail(&(q->chunks));
        if (!chunk)
              qvSysFree(chunk);

        qvUnlock( lock );

	return ;

}

/*****************************************************************************
 * FUNCTION: qvAbsoluteTimerStart
 *
 * DESCRIPTION: Starts a timer for a module 
 *
 * ARGS:
 *				module          Identifies the module,for which the timer is started.
 *				                The module for which the timer is started can be attached 
 *				                to any driver(thread)
 *				                
 *				expiry		    The absolute time at which the timer is to expire.
 *				                The absolute expiry time is the sum of the current 
 *				                wall time and the desired duration.
 *				
 *				data			This data is made available to the timer handler of 
 *				                the module after the timer expires
 *				                 
 *				repeat		    If set to 1, the timer started is cyclic 
 *
 * NOTE:
 *          1) This function can be called both inside and outside the stack context.
 *          2) The wall time function used to compute the absolute expiry time should be
 *             the same as the walltime function provided in the QWAIT structure. 
 * 
 * RETURNS:
 *			On success,a identifier to timer.
 *			On Error,0
 *
 * ERROR CODES:
 *               QVERROR_NONE - No Error.Timer for the module successfully started 
 *
 *               QVERROR_TIMER_ALLOC_FAIL
 *                           -  Error. Unable to create a timer.
 ******************************************************************************/
QTIMER	qvAbsoluteTimerStart(QMODULE module,const QTIME *expiry, void *data, int repeat, int *cErr)
{
    QTIME duration;
    int		ret_val;
	
    qmodule *mod = (qmodule*)module;
	qvars	*qvc_dest = (qvars*) mod->scheduler;
	qvars	*qvc_self = QV_GETKEY();
	qtimer	*T = NEWTIMER(module);

    /* If required, Reset the CSPL Error Code. */
	if(cErr)
	{
		*cErr = QVERROR_NONE;
	}

    if (!T) {
		if(cErr)
		{
			*cErr = QVERROR_TIMER_ALLOC_FAIL;
		}
        return (QTIMER)0;
    }

	T->owner = module;
	T->data = data;
	T->repeat = repeat;
	
	if( qvc_self == qvc_dest )
	{
		ret_val = qvTimeDiff(expiry, &qvc_self->now, &duration);
		if (ret_val != 1)
        {
			duration.s = 0;
			duration.us = 0;
		}
			
	    T->duration = duration; 
		__qvTimerStart( qvc_self, T, &duration);
	}
	else
	{

        qvLock( qvc_dest->lock_mtimer );
		
		ret_val = qvTimeDiff(expiry, &qvc_dest->now, &duration);
		if (ret_val != 1) {
			duration.s = 0;
			duration.us = 0;
		}
			
        T->duration=duration;
		__qvTimerStart( qvc_dest, T, &duration);
		__qvNextTime( qvc_dest, &(qvc_dest->nextexpiry), &(qvc_dest->now), 0 );

		qvUnlock( qvc_dest->lock_mtimer );
		__qvWakeup(qvc_dest);
	}

	return (QTIMER)T;
}
#ifdef MODULE_LOGGING

/**********************************************************************
*
*FUNCTION:qvSetModuleLogLevel
*
*DESCRIPTION:This function is used to set loglevel for a particular 
*           module 
*
*ARGS:
*     moudule:module whose loglevel has to be changed
*     level:LogLevel for the module
*
*OUT:
*      void
**********************************************************************/

void qvSetModuleLogLevel(QMODULE m,unsigned char level)
{
		
(LOCALMODULE(m))->level=level;

}
/**********************************************************************
*
*FUNCTION:qvGetModuleLogLevel
*
*DESCRIPTION:This function is used to get loglevel for a particular 
*           module 
*
*ARGS:
*     moudule:module whose loglevel is needed
*
*OUT:
*      loglevel for the module specified 

**********************************************************************/

unsigned char qvGetModuleLogLevel(QMODULE m)
{
		
return((LOCALMODULE(m))->level);

}

#endif

/*****************************************************************************
 * FUNCTION: qvPoolStats
 *
 * DESCRIPTION:
 *           This function displays pool stats on screen.
 *
 * ARGS:
 *
 * RETURNS:
 *           None
 *
 *****************************************************************************/
#define CSPL_LOG_LEVEL_INFO       3   
#define CSPL_LOG_LEVEL_BRIEF      4   
#define CSPL_LOG_LEVEL_DETAILED   5

void qvPoolStats(void)
{
    unsigned int i=0, n=0, allocBuf=0, freeBuf=0, nbuf =0;
    unsigned long memSize = 0;
    unsigned long allocMemSize = 0;
    unsigned long peakMemSize = 0;
    qpool   *q = ((void *)0);
    if(qd) 
    {
        for(i=0, n=MINBUF; i<NVARPOOL; i++, n *= 2 ) 
        {
            allocBuf = freeBuf = nbuf = 0;
            q = ((void *)0);
            q = (qpool*)qd->alloc[i];
            if(q) 
            {
                nbuf = __qvPoolSize(q,&allocBuf);
                freeBuf = nbuf - allocBuf;
            }
            if((nbuf !=0) && (q))
            {
                qvLog(CSPL_LOG_LEVEL_INFO,"\nPool of %d:nBuf=%d,Alloc Buf=%d,Free Buf=%d,Peak Alloc Buf=%d\n",
                        n,
                        nbuf,
                        allocBuf,
                        freeBuf,
                        q->peakVal);
            }
            memSize += (n * nbuf);
            allocMemSize += (n * allocBuf);
            if(q)
            {
                peakMemSize += (n * q->peakVal);
            }
        }
        qvLog(CSPL_LOG_LEVEL_INFO, "Total Mem = %lu Alloc Mem = %lu Peak Alloc Mem =%lu\n",
                 memSize, allocMemSize, peakMemSize);
    }
}
#ifdef CSPL_LEAK_DEBUG
/*****************************************************************************
 * FUNCTION: qvPrintMemPoolAllocBuf
 *
 * DESCRIPTION: This function prints the backtrace of the buffers which are not 
 *              freed or doublefreed 
 *       
 * ARGS: *q            pointer to the buffer
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvPrintMemBufferBt(qbuf *q)
{
    char **strings;
    int j;

    /* Get symbol names - necessary to compile with rdynamic
     * to get meaningful names */
    strings = backtrace_symbols(q->buffer, q->nptrs);

    if (strings == NULL) {
        perror("backtrace_symbols");
    }
    fprintf(stderr, "----- 0x%x -----\n", (unsigned int)q);
    for (j = 0; j < q->nptrs; j++)
        fprintf(stderr, "%s\n", strings[j]);

    free(strings);
}

/*****************************************************************************
 * FUNCTION: qvPrintPoolAllocBuf
 *
 * DESCRIPTION: This function checks whether buffers of the pool are free or not 
 *              and prints the buffer which are not free.
 *          
 * ARGS: *pool        pointer to the pool
 *
 * RETURNS:
 *
 ******************************************************************************/

void qvPrintMemPoolAllocBuf (qpool   *pool)
{
    qchunk		*chunk;
    unsigned int	realsize, i;
    unsigned char	*p;

    realsize = YALIGN(sizeof(qbuf),8) + YALIGN(pool->size,8);
    chunk = (qchunk *)ylFirst(&pool->chunks);
    while(chunk)
    {
        p =(unsigned char*) chunk;
        for( i=0, p += YALIGN(sizeof(qchunk),8); i<chunk->nbuf; i++, p += realsize ) {
            qbuf	*q = (qbuf *)(void *)p;

            if (q->allocated != 0){
                /* print the backtrace for the allocation */
                qvPrintMemBufferBt(q);
            }
        }
        chunk = (qchunk *)ylNext((const YLNODE *)chunk);
    }
}


/*****************************************************************************
 * FUNCTION: qvPrintAllocBuf
 *
 * DESCRIPTION: This function prints buffer info of various msg pools 
 *         
 * ARGS:
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvPrintMemAllocBuf(void)
{
    int i;
    for( i=0; i< NVARPOOL; i++ ) {
	    if (0 != qd->alloc[i]->alloc) {
            fprintf (stderr, "##### Pool[%d] Size [%d] Total Buffer [%d] "
                    "Allocated Buffer[%d] #####\n",
                    i, qd->alloc[i]->size, qd->alloc[i]->nbuf, qd->alloc[i]->alloc);
            qvPrintMemPoolAllocBuf(qd->alloc[i]);
        }
    }

}

/*****************************************************************************
 * FUNCTION: qvPrintCurrentBt
 *
 * DESCRIPTION: This function prints the current backtrace*
 * ARGS:
 *
 * RETURNS:
 *
 ******************************************************************************/

void qvPrintMemCurrentBt(void)
{
    int j, nptrs;
    void *buffer[MAX_FRAMES_IN_BT];
    char **strings;

    nptrs = backtrace(buffer, MAX_FRAMES_IN_BT);
   // printf("backtrace() returned %d addresses\n", nptrs);
    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
    * would produce similar output to the following: */
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
    }
    for (j = 0; j < nptrs; j++)
        fprintf(stderr, "%s\n", strings[j]);

    free(strings);
}

#endif

/*spr 22474 start*/
/*****************************************************************************
 * FUNCTION: getMemPoolStatsInShm
 *
 * DESCRIPTION:
 *           This function returns pool stats
 *
 * ARGS: poolIndex  ; for which the function returns stats
 *       allocBuf ;  pointer to store no of allocated buffers in the pool
 * RETURNS:
 *           no of buffers in the pool
 *
 *****************************************************************************/
int getMemPoolStatsInShm( unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf )
{
	unsigned int nbuf =0;
	if(qd)
	{
		qpool *q =(qpool *)qd->alloc[poolIndex];
		if(q)
		{
			nbuf = __qvPoolSize(q, allocBuf);
			*peakAllocBuf = q->peakVal;

		}
	}
	return(nbuf);
}

/*****************************************************************************
 * FUNCTION: getMsgPoolStatsInShm
 * DESCRIPTION: This function returns message pool stats
 * ARGS: poolIndex; pool id for which the function returns stats
 *           allocBuf  ; pointer to store no of allocated buffers in the pool
 * RETURNS:nbuf ; no of buffers in the pool
 *****************************************************************************/
int getMsgPoolStatsInShm( unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf )
{
	unsigned int nbuf =0;
	if(qd) {
		qpool   *q = (qpool *)qd->alloc[poolIndex];
		if(q) {
			nbuf = __qvPoolSize(q,allocBuf);
			*peakAllocBuf = q->peakVal;
		}
	}
	return(nbuf);
}

/*****************************************************************************
 * FUNCTION: getQMsgPoolStatsInShm
 * DESCRIPTION: This function returns Q message pool stats
 * ARGS: allocBuf  ; pointer to store no of allocated buffers in the pool
 * RETURNS:nbuf ; no of buffers in the pool
 *****************************************************************************/
int getQMsgPoolStatsInShm( unsigned int *allocBuf, unsigned int *peakAllocBuf )
{       
    unsigned int nbuf =0;
	if(qd) {
        *allocBuf = ((qpool *)qd->qmsgpool)->alloc;
        *peakAllocBuf  = ((qpool *)qd->qmsgpool)->peakVal; 
        nbuf = ((qpool *)qd->qmsgpool)->nbuf;
    }
    return(nbuf);
}   

/*****************************************************************************
 * FUNCTION: getQpctlPoolStatsInShm
 * DESCRIPTION: This function returns QPCTL pool stats
 * ARGS: allocBuf  ; pointer to store no of allocated buffers in the pool
 * RETURNS:nbuf ; no of buffers in the pool
 *****************************************************************************/
int getQpctlPoolStatsInShm( unsigned int *allocBuf, unsigned int *peakAllocBuf )
{
    unsigned int nbuf =0;
    if(qd) {
        *allocBuf = ((qpool *)qd->pctlpool)->alloc;
        *peakAllocBuf  = ((qpool *)qd->pctlpool)->peakVal;
        nbuf = ((qpool *)qd->pctlpool)->nbuf;
    }
    return(nbuf);
}


/*****************************************************************************
 * FUNCTION: getMsgPoolStats
 * DESCRIPTION: This function returns message pool stats
 * ARGS: poolIndex; pool id for which the function returns stats
 *           allocBuf  ; pointer to store no of allocated buffers in the pool
 * RETURNS:nbuf ; no of buffers in the pool
 *****************************************************************************/
int getMsgPoolStats( unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf )
{
    unsigned int nbuf =0;
    if(qd) {
        qpool   *q = (qpool *)qd->alloc[poolIndex];
        if(q) {
            nbuf = __qvPoolSize(q,allocBuf);
            *peakAllocBuf = q->peakVal;
        }
    }
    return(nbuf);
}

/*****************************************************************************
 * FUNCTION: getQMsgPoolStats
 * DESCRIPTION: This function returns Q message pool stats
 * ARGS: allocBuf  ; pointer to store no of allocated buffers in the pool
 * RETURNS:nbuf ; no of buffers in the pool
 *****************************************************************************/
int getQMsgPoolStats( unsigned int *allocBuf, unsigned int *peakAllocBuf )
{       
    unsigned int nbuf =0;
    if(qd) {
        *allocBuf = ((qpool *)qd->qmsgpool)->alloc;
        *peakAllocBuf  = ((qpool *)qd->qmsgpool)->peakVal; 
        nbuf = ((qpool *)qd->qmsgpool)->nbuf;
    }
    return(nbuf);
}   

/*****************************************************************************
 * FUNCTION: getQpctlPoolStats
 * DESCRIPTION: This function returns QPCTL pool stats
 * ARGS: allocBuf  ; pointer to store no of allocated buffers in the pool
 * RETURNS:nbuf ; no of buffers in the pool
 *****************************************************************************/
int getQpctlPoolStats( unsigned int *allocBuf, unsigned int *peakAllocBuf )
{
    unsigned int nbuf =0;
    if(qd) {
        *allocBuf = ((qpool *)qd->pctlpool)->alloc;
        *peakAllocBuf  = ((qpool *)qd->pctlpool)->peakVal;
        nbuf = ((qpool *)qd->pctlpool)->nbuf;
    }
    return(nbuf);
}

/*****************************************************************************
 * FUNCTION: getQMessageSize
 * DESCRIPTION: This function returns Q message pool size
 * ARGS: None
 * RETURNS:size of Qmessage
 *****************************************************************************/
unsigned int getQMessageSize()
{
  return sizeof(qmessage);
} 

/*****************************************************************************
 * FUNCTION: getQpctlSize
 * DESCRIPTION: This function returns QPCTL pool size
 * ARGS: None
 * RETURNS:size of QPCTL
 *****************************************************************************/
 unsigned int getQpctlSize()
{
  return sizeof(qpctl);
}
/*spr 22474 end*/

#ifdef LINUX_PC_TEST
U8 qvDeliverForL3Test(QMODULE from, QMODULE to, void *message)  //modify from qvDeliverEx
{
    qmessage	*qm;
    qmodule		*q;
    unsigned char	*c; 
    qvars	*dest_sched;

    /*Reformatting the message to be put in the Q*/
    c = (unsigned char *)message;
    c -= YALIGN(sizeof(qmessage),8);
    qm = (qmessage *)(void *)c;
    qm->sender = from;
    qm->receiver = to;
    qm->priority = 0;
    qm->owner = to;

    if( !to || !from)
        return 1;

    q = LOCALMODULE(to);
    dest_sched = q->scheduler;
    if (q->scheduler == 0)
        return 2; 

    /* destination thread information exists */
    /* push the msg to be delivered in input queue for
     * the destination module. Called from qvIncoming*/
    qvLock( dest_sched->lock );
    ysPushTail( &(dest_sched->queue), (YSNODE *)qm );
    qvUnlock( dest_sched->lock );

    //if( dest_sched != qvc ) 
    {   //if target module is not myself
        __qvWakeup( dest_sched );
    }
    
    return 0;
}
#endif