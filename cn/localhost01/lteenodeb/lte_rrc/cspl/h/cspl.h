/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Interface Definition                    **/
/**                                                                        **/
/****************************************************************************/

#ifndef	__HSS_CSPL_H__
#define	__HSS_CSPL_H__

#include	<ylib.h>
#include	<stdarg.h>

/* Defining Version of CSPL Release */
#define qvRelVer	htonl( (1<<24) | (5<<16) | (0<<8) )
#define MAX(A,B)	((A) > (B) ? (A) : (B))

#define 	MAX_POOLS		64
/* SPR 5045*/
#define 	MAX_VAL_U32		4294967295U

/** Types *******************************************************************/

typedef unsigned int    U32;
typedef unsigned long int       Ulong32;
typedef unsigned short  U16;
typedef unsigned char   U8;
typedef signed int      S32;
typedef signed short    S16;
typedef signed char         S8;
/* SPR 20430 Changes Start*/
#if defined(__x86_64__)
typedef unsigned long   U32Long; 
typedef signed long     S32Long;

typedef	unsigned int	U32;
typedef	unsigned short	U16;
typedef	unsigned char	U8;
typedef	signed int      S32;
typedef	signed short	S16;
typedef	signed char     S8;
#else
typedef unsigned long   U32Long;  /* For compatibility after 64 bit compilation changes */

typedef signed long     S32Long;
#endif
/* SPR 20430 Changes End*/


/* SPR 20636 Changes Start */
#ifdef __x86_64__
#define U32_64_Bit U32Long
#else
#define U32_64_Bit U32
#endif
/* SPR 20636 Changes Start */


typedef	struct	{
	unsigned long	s;
	unsigned long	us;
}	QTIME;

typedef	struct	{
	unsigned char	*base;
	unsigned int	size;
}	QSEGMENT;

typedef	void		*QMODULE;
typedef	void		*QTIMER;
typedef	void		*QMPOOL;
typedef	void		*QLOCK;
typedef	void		*QDRIVER;
typedef void		**QVMPOOLSET;

typedef	struct	{
	unsigned long	ntimers;
	unsigned long	nmodules;
	unsigned long	npctls;
}	QTUNING;

typedef struct {
	unsigned int	bufsize;
	unsigned int	nbufs;
}	QPOOLTUNING;
	
typedef struct {
	unsigned int	opt;
	unsigned long	npools;
	QPOOLTUNING	qpt[MAX_POOLS];
}	QPOOLSETTUNING;	


typedef	struct	{
	void	*(*malloc)(unsigned int);
	void	(*free)(void *);
	void	*(*newkey)(void);
	void	*(*getkey)(void *);
	void	(*setkey)(void *, void *);
	void	*(*newlock)(void);
	void	(*destroylock)(void *);
	void	(*lock)(void *);
	void	(*unlock)(void *);
	int	(*vprintf)(const char *, va_list);
	int	(*iprintf)(const char *, va_list);
	void	(*abort)(void);
}	QSYSOP;

struct QSHELL;
typedef	struct	QSHELL QSHELL;
struct	QSHELL {
	void	(*timedwait)( void *, const QTIME * );
	void	(*send)( void *, const void *, QMODULE, QMODULE, void *, unsigned int );
	void	*(*receive)( void *, QMODULE *, QMODULE *, signed char * );
	void	*(*open)(void *);
	void	(*close)(void *);
	void	*(*hash)(void *, void *);
};

struct QWAIT;
typedef	struct	QWAIT QWAIT;
struct	QWAIT {
	void	*(*newchannel)(void);
	void	(*sleep)(void *, const QTIME *);
	void	(*wakeup)(void *);
	void	(*walltime)( QTIME * );
};

typedef struct	QMANIFEST {
	const char	*name;
	unsigned long
		service;

	struct	{
		void	*(*early)(void *);
		void	*(*late)(void *); 
	} init;

	struct {
		S32	(*message)(void *, void *); 
		void	(*timer)(QTIMER, void *, void *); 
	} handler;

	struct	{
		void	*(*pack)( QMODULE, void *, U32 * );
		void	*(*unpack)( QMODULE, void * );
		void	*(*alloc)( QMODULE, U32 );
	} method;

	const unsigned long *destinations;
} QMANIFEST;


typedef struct	QADAPTER {
	void	*(*init)(QMODULE, void *);
	void	*(*pack)( QMODULE, void *, U32 * );
	void	*(*unpack)( QMODULE, void * );
	void	*(*alloc)( QMODULE, U32 );
	void	*(*in)(void *, QMODULE *, QMODULE *); 
	void	*(*out)(void *, QMODULE *, QMODULE *);

	const unsigned long *destinations;
} QADAPTER;

#define		LOGGER_MODULE_ID	999
#define		LOGGER_API_SIZE		6

/** Library Initialization **************************************************/

void	qvInit( const QTUNING *t, const QSYSOP *op );
void	qvInitEx( const QTUNING *t, const QSYSOP *op, int *cErr);
QDRIVER	qvNewDriver( void );
QDRIVER	qvNewDriverEx(int *cErr);
QDRIVER	qvDriverStart(QDRIVER driver, void (*wakeupfn)(void *), void *arg);
#define	qvDriver(w,a)	(qvDriverStart(qvNewDriver(),(w),(a)))
#define	qvSimpleInit(op)	(qvInit(0,(op)), qvDriver(0,0))
void qvPoolStats(void);


/** Memory Pool Management **************************************************/
QMPOOL	qvPoolCreate( unsigned int size, unsigned int nbuf );
QMPOOL	qvPoolCreateEx( unsigned int size, unsigned int nbuf, int *cErr);
void	qvPoolDelete( QMPOOL Q );
int	qvPoolExtend( QMPOOL Q, unsigned int nbuf );
int	qvPoolExtendEx( QMPOOL Q, unsigned int nbuf, int *cErr);
unsigned int	
	qvPoolSize( QMPOOL Q, unsigned int *p_alloc );
void	*qvPoolAlloc(QMPOOL pool);
void	*qvPoolAllocEx(QMPOOL pool, int *cErr);
void	*qvPoolAllocSpecific( QMPOOL Q, unsigned int indexparam, unsigned short sequence );
void	*qvPoolAllocSpecificEx( QMPOOL Q, unsigned int indexparam, unsigned short sequence, int *cErr);
void	qvPoolFree(void *buffer);
unsigned short	
	qvPoolSequence( void *buffer );
unsigned int
	qvPoolIndex( QMPOOL Q, void *buffer);
unsigned int
	qvPoolIndexEx( QMPOOL Q, void *buffer, int *cErr);
void	*qvPoolLocate( QMPOOL Q, unsigned int indexparam);
void	*qvPoolLocateEx( QMPOOL Q, unsigned int indexparam, int *cErr);





/* Task common memory pools *************************************************/

/** Constants **/
#define	__BUFMAGIC	0xACE12E92
#define	__POOLMAGIC	0xCA98362D

typedef void        *QMPOOLSET;

typedef struct {
    unsigned int    bufsize;
    unsigned int    nbufs;
    unsigned int    nalloc;
}   QPOOLTUNINGINFO;

typedef struct {
    unsigned long   npools;
    QPOOLTUNINGINFO qpt[MAX_POOLS];
}   QPOOLSETTUNINGINFO;


typedef	struct	qmpool {
	unsigned long	magic;
	unsigned int	size, nbuf, alloc;
	YLIST		chunks;
	YLIST		list;

	QLOCK		lock;
#ifdef MEM_DEBUG
        unsigned long   ref_ctr;
#endif
} qmpool;

typedef	struct	qmbuf {
	union {
		YLNODE	__header__;
		qmpool	*pool;
	} u;

	unsigned short	incarnation;
	unsigned char	allocated;
#ifdef MEM_DEBUG
        unsigned long   ref_id;
#endif
} qmbuf;

typedef	struct	qmchunk {
	YLNODE	__header__;
	unsigned int	nbuf;
} qmchunk;

typedef struct qmpoolsetContext {
	unsigned int	magic;
	unsigned int	opt;
        unsigned int    npools;
        QMPOOL          alloc[MAX_POOLS];
        QLOCK           lock ;

}qmpoolset ;


/* task common memory pools structures */
extern unsigned int qvGetQMessageSize(void);
extern void    qvMsgPoolDelete( QMPOOL Q );
extern unsigned int qvMsgPoolBufferSize(QMPOOL Q);
extern void __qvMsgPoolDecrease(QMPOOL Q, unsigned int nbufs);


QMPOOLSET qvMsgPoolSetCreate(QPOOLSETTUNING *t, int *cErr);
void    *qvMsgPoolSetAlloc(QMPOOLSET *rpd, unsigned int size,unsigned int *psize, int *cErr);

#define qvPoolSetFree(buffer)      (qvPoolFree(buffer))
#define qvMsgPoolSetFree(buffer)   (qvMsgFree(buffer))

unsigned int qvMsgPoolSetExtend(QMPOOLSET *rpd, QPOOLSETTUNING *t, int *cErr);
unsigned int    qvMsgPoolSetDelete( QMPOOLSET *rpd, int *cErr);

QMPOOLSET qvPoolSetCreate(QPOOLSETTUNING *t, int *cErr);
void    *qvPoolSetAlloc(QMPOOLSET rpd, unsigned int size,unsigned int *psize, int *cErr);
unsigned int qvPoolSetExtend(QMPOOLSET rpd, QPOOLSETTUNING *t, int *cErr);
unsigned int    qvPoolSetDelete( QMPOOLSET rpd, int *cErr);
unsigned int qvMsgPoolSetSize(QMPOOLSET *rpd, QPOOLSETTUNINGINFO *t, int *cErr);
unsigned int qvPoolSetSize(QMPOOLSET rpd, QPOOLSETTUNINGINFO *t, int *cErr);




/** Malloc Emulation ********************************************************/
void	*qvAlloc(unsigned int size, unsigned int *psize);
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
void	*qvAllocEX(unsigned int size, unsigned int *psize, int *cErr, unsigned char *func, unsigned int line);
void displayMemPoolStatsCspl(void);
void qvPrintMemPoolAllocBuffers();
#else
void	*qvAllocEx(unsigned int size, unsigned int *psize, int *cErr);
#endif
/*SPR_18125_END*/
void	qvFree( void *p );
void	qvFreeEx( void *p,int *cErr );

/** Access to System Memory Allocation **************************************/
void	*qvSysMalloc(unsigned int size);
void	qvSysFree( void *p );

/** Message Buffer Management ***********************************************/
QMPOOL	qvMsgPoolCreate( unsigned int attrsize, unsigned int nbuf );
QMPOOL	qvMsgPoolCreateEx( unsigned int attrsize, unsigned int nbuf, int *cErr);
unsigned int
	qvMsgPoolSize( QMPOOL Q, unsigned int *p_alloc );
int	qvMsgPoolExtend( QMPOOL Q, unsigned int nbuf );
int	qvMsgPoolExtendEx( QMPOOL Q, unsigned int nbuf, int *cErr);
QMPOOL	qvMsgPoolOf(const void *buffer);
void	*qvMsgAlloc(QMPOOL pool, unsigned int headroom, 
		unsigned int tailroom, unsigned int attrsize);
void	*qvMsgAllocEx(QMPOOL pool, unsigned int headroom, 
		unsigned int tailroom, unsigned int attrsize, int *cErr);
#define	qvMsgAllocFromPool(pool)		qvMsgAlloc((pool),0,0,0)
#define	qvMsgAllocFromPoolEx(pool, cErr)	qvMsgAllocEx((pool),0,0,0, cErr)
#define	qvMsgAllocBySize(size)			qvMsgAlloc(0,0,0,(size))
#define	qvMsgAllocBySizeEx(size, cErr)		qvMsgAllocEx(0,0,0,(size), cErr)
void	qvMsgFree(void *buffer);
void	qvMsgFreeEx(void *buffer,int *cErr);
unsigned int	
	qvMsgSize(const void *buffer, U32 *attrsize);
unsigned char	
	*qvMsgReserve(void *buffer, int where, unsigned int count);
unsigned char	
	*qvMsgReserveEx(void *buffer, int where, unsigned int count, int *cErr);
unsigned char	
	*qvMsgInsert(void *buffer, int where, unsigned char *data, unsigned int count);
unsigned char	
	*qvMsgInsertEx(void *buffer, int where, unsigned char *data, unsigned int count, int *cErr);
unsigned char	
	*qvMsgInsertExternal(void *buffer, int where, unsigned char *data, 
		unsigned int count, 
		void (*freefn)(void *, void *), void *freearg);
unsigned char	
	*qvMsgInsertExternalEx(void *buffer, int where, unsigned char *data, 
		unsigned int count, 
		void (*freefn)(void *, void *), void *freearg, int *cErr);
int	qvMsgRemove(void *buffer, int where, unsigned int count);
int	qvMsgRemoveEx(void *buffer, int where, unsigned int count, int *cErr);
int	qvMsgExtract(const void *buffer, int where, unsigned char *data, unsigned int count);
int	qvMsgExtractEx(const void *buffer, int where, unsigned char *data, unsigned int count, int *cErr);
void	*qvMsgClone(const void *buffer);
void	*qvMsgCloneEx(const void *buffer, int *cErr);
void	*qvMsgCopy(void *tobuffer, const void *frombuffer, unsigned int attrsize);
void	*qvMsgCopyEx(void *tobuffer, const void *frombuffer, unsigned int attrsize, int *cErr);
void	*qvMsgSplit(void *buffer, unsigned int offset, void *tobuffer);
void	*qvMsgSplitEx(void *buffer, unsigned int offset, void *tobuffer, int *cErr);
void	*qvMsgJoin(void *buffer, void *append);
void	*qvMsgJoinEx(void *buffer, void *append, int *cErr);
void	*qvMsgPack(void *buffer, unsigned int headroom, unsigned int tailroom);
void	*qvMsgPackEx(void *buffer, unsigned int headroom, unsigned int tailroom, int *cErr);
unsigned int
	qvMsgSegCount( const void *buffer );
void	*qvMsgSegNext( const void *buffer, void *last, QSEGMENT *segment );

/* Pool Specific functions */
int	qvCsplPoolsExtend( QTUNING *t, int *cErr);
void 	qvPoolSort( QMPOOLSET Q, int n );

/** Timer Management ********************************************************/
const QTIME	
	*qvNow(QTIME *T);
unsigned int	qvTimerGetRepeatFlag(QTIMER T);
int	qvTimeDiff(const QTIME *A, const QTIME *B, QTIME *result);
QTIMER	qvTimerStart(const QTIME *duration, void *data, int repeat);
QTIMER	qvTimerStartEx(const QTIME *duration, void *data, int repeat, int *cErr);
void	qvTimerStop(QTIMER T, QTIME *ptime );
void	qvTimerStopEx(QTIMER T, QTIME *ptime, int *cErr );
int	qvModuleTimerStop(QTIMER T, QTIME *ptime, int *cErr );
int	qvTimerRunning(QTIMER T);
void	*qvTimerData(QTIMER T);
int qvTimerRemaining(QTIMER T,QTIME *rem);
QTIMER	qvModuleTimerStart(QMODULE module,const QTIME *duration, 
					void *data, int repeat, int *cErr);

QTIMER	qvAbsoluteTimerStart(QMODULE module,const QTIME *expiry, void *data, int repeat, int *cErr);
#define qvAbsoluteTimerStop(T,ptime,pErr)		(qvModuleTimerStop((T),(ptime),(pErr)))

/** Scheduling **************************************************************/
int	qvPending(QDRIVER driver);
QMODULE	qvModuleAttach( 
		QDRIVER	driver,
		const char *name,
		void *(*init)(void *), 
		S32 (*msgfn)(void *, void *), 
		void (*tmrfn)(QTIMER, void *, void *), 
		void *initarg,
		unsigned long service,
		int timer_tuning,
		int *cErr);
QMODULE	qvModuleCreate( 
		const char *name,
		void *(*init)(void *), 
		S32 (*msgfn)(void *, void *), 
		void (*tmrfn)(QTIMER, void *, void *), 
		void *initarg,
		unsigned long service);
void	*qvModuleContext( QMODULE module );
QMODULE	qvSelf( void );
QDRIVER	qvThisDriver( void );
void	*qvThisContext( void );
void	*qvThisMessage( void );
QTIMER	qvThisTimer( void );
#define	qvName()	(qvGetServiceName(qvSelf()))
void	qvSetMethods( QMODULE module, const void *m );
const void	
	*qvGetMethods( QMODULE module );

const QTIME	*qvSchedule( const QTIME * );
void	qvWakeup(QDRIVER driver);

/** Messaging ***************************************************************/
QMODULE qvGetService( unsigned long name );
QMODULE qvGetServiceEx( unsigned long name, int *cErr);
unsigned long
	qvGetServiceName( QMODULE service );
void	qvSetFilter( QMODULE module, 
		void *(*in)(void *, QMODULE *, QMODULE *), 
		void *(*out)(void *, QMODULE *, QMODULE *) );
QMODULE	qvSetDispatch( unsigned long name, 
		int (*func)(void *, void *, QMODULE, QMODULE), void *data );
QMODULE	qvSetDispatchEx( unsigned long name, 
		int (*func)(void *, void *, QMODULE, QMODULE), void *data, int *cErr);
void	qvSetImmediate( unsigned long name, int* cErr);
void	qvDeliver(QMODULE from, QMODULE to, int priority, void *message );
void	qvDeliverEx(QMODULE from, QMODULE to, int priority, void *message, int *cErr);
void	qvSend(QMODULE to, int priority, void *message);
void	qvSendEx(QMODULE to, int priority, void *message, int *cErr);
QMODULE	qvFrom( const void *message );
QMODULE	qvTo( const void *message );
int	qvPriority( const void *message );

/** Locks *******************************************************************/
QLOCK	qvNewLock( void );
void	qvLock( QLOCK lock );
void	qvUnlock( QLOCK lock );
void	qvDestroyLock( QLOCK lock );

/** Tracing *****************************************************************/
int	qvPrintf( const char *format, ...);
void	qvDump( const unsigned char *buf, unsigned int n, unsigned long style );
int	qvLog( int level, const char *format, ...);
void	qvLogDump( int level, const unsigned char *buf, unsigned int n, unsigned long style );
void	qvLogLevel( unsigned char level );
/* GDB Fix - add U8 qvGetLogLevel( void ) to fix unresolved in L3 module */
unsigned char qvGetLogLevel( void );

#ifdef MODULE_LOGGING
void qvSetModuleLogLevel(QMODULE m,unsigned char level);
unsigned char qvGetModuleLogLevel(QMODULE m);
#endif
void	qvLogTime( const QTIME *T );

int	qvPrintfVa( const char *format, va_list A);
int	qvLogVa( int level, const char *format, va_list A);

enum {
	QVLOG_NONE = 0,
	QVLOG_CRITICAL = 1,
	QVLOG_MAJOR = 2,
	QVLOG_MINOR = 3,
	QVLOG_INFO = 4,
	QVLOG_DETAIL = 5,
	QVLOG_DUMP = 6,
	QVLOG_LIBRARY = 255
};

/** Error Handling **********************************************************/
void	qvPanic( const char *format, ... );
void	qvWarning( const char *format, ... );
void	qvAbort(void);
#define qvAssert(d)      (void)( (d) || (__qvAssert__(__FILE__,__LINE__,#d),0) )
#define __qvAssert__(a,b,c)      qvPanic("[%s:%u] Assertion Failed <%s>", a, b, c )

/** Manifest ****************************************************************/
/* Values of timer_tuning param for module-timer pool.  */
#define QV_TPOOL_EXT	0
#define QV_TPOOL_NONE	-1

QMODULE	qvRegister( QDRIVER driver, const char *name, 
		unsigned int service, const QMANIFEST *methods, void *data);
QMODULE	qvRegisterEx( QDRIVER driver, const char *name, 
		unsigned int service, const QMANIFEST *methods, void *data,
		int timer_tuning, int *cErr);
void	*qvMsgAllocFromModule( QMODULE module, unsigned int size );
void	qvIncoming( QMODULE from, QMODULE to, signed int priority, void *message );
QMODULE	qvSetOutgoing( unsigned long service, 
		int (*outgoing)(void *, void *, QMODULE, QMODULE, unsigned int), void *calldata );
void	qvAdapt( QMODULE module, const QADAPTER *adapter, void *data );
void	*qvAdaptData( QMODULE module );

/** Skeleton Encapsulating Shell ********************************************/
QMODULE	qvSetDestination( unsigned long service, const QSHELL *, void *listener, void *data );


/******************Commented as multiply defined functions SPR No: 2550**********************/
/*void	qvSetImmediate( unsigned long name, int* cErr );*/
void	qvRun( const QSHELL *, void *listener, const QWAIT *W, int policy );
void	qvInputLoop( const QSHELL *, void *listener );
void	qvDriverLoop( QDRIVER driver, const QWAIT *W );
#define	qvOpen(s,d) (((QSHELL *)s)->open((d)))
#define	qvClose(s,d) (((QSHELL *)s)->close((d)))

/** Library Level Debugging *************************************************/
unsigned int 	__qvPoolStatus(QMPOOL pool);
unsigned int	__qvHoldStatus(QDRIVER driver);
unsigned int	__qvQueueStatus(QDRIVER driver);
void	__qvLibraryStatus(void);

/******************Commented as multiply defined functions SPR No: 2550**********************/
/*unsigned int    qvGetQMessageSize(void);
void            qvMsgPoolDelete( QMPOOL Q );
unsigned int    qvMsgPoolBufferSize(QMPOOL Q);
void            __qvMsgPoolDecrease(QMPOOL Q, unsigned int nbufs);*/


/** Error Codes and some description ********************************************/
/* Each API functions, Initializes 'cErr' to QVERROR_NONE.
	So, Internally called functions need not reset this again.
	This is done to avoid unnecesary checks in the system. */
enum {
	QVERROR_NONE = 0,
	QVERROR_GENERIC,
	QVERROR_INVALID_SRV,
	QVERROR_TIMER_NOT_OWNER,
	QVERROR_TIMER_NOT_RUNNING,
	QVERROR_SYS_ALLOC_FAIL,
	QVERROR_MEM_POOL_EXHAUSTED,
	QVERROR_TIMER_ALLOC_FAIL,
	QVERROR_MODULE_ALLOC_FAIL,
	QVERROR_PCTL_ALLOC_FAIL,
	QVERROR_MEM_ALLOC_ILLEGAL_SIZE,
	QVERROR_MEM_ILLEGAL_LOCATION,
	QVERROR_MEM_ILLEGAL_INDEX,
	QVERROR_MEM_UNALLOCATED_BUFFER,
	QVERROR_MEM_ALLOCATED_BUFFER,
	QVERROR_MEM_ILLEGAL_BUFFER,
	QVERROR_MEM_ILLEGAL_POOL,
	QVERROR_INVALID_ARGUMENT
};
/*spr 22474 start*/
int getMemPoolStatsInShm( unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf );
int getMsgPoolStatsInShm( unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf );
int getQMsgPoolStatsInShm( unsigned int *allocBuf, unsigned int *peakAllocBuf );
int getQpctlPoolStatsInShm( unsigned int *allocBuf, unsigned int *peakAllocBuf );
int getMsgPoolStats( unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf );
int getQMsgPoolStats( unsigned int *allocBuf, unsigned int *peakAllocBuf );
int getQpctlPoolStats( unsigned int *allocBuf, unsigned int *peakAllocBuf );
unsigned int getQMessageSize(void);
unsigned int getQpctlSize(void);
/*spr 22474 end*/
#ifdef CSPL_LEAK_DEBUG
void qvPrintMemAllocBuf(void);
void qvPrintMemCurrentBt(void);
#endif

#endif
