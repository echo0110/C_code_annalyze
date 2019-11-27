/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Implementation                          **/
/**                                                                        **/
/****************************************************************************/
#include <lteMisc.h>

#include "msgpool.h"
#include "atomic-ops.h"

#ifdef CSPL_LEAK_DEBUG
#include <execinfo.h>
#endif
/* SPR 19288 change start */
/* SPR 21001 Changes end */
/* SPR 19288 change end */

/** Constants ***************************************************************/
#define	__POOLMAGIC	0xCA98362D

#define	MINBUF		8
#define	NVARPOOL	27

enum {
	QSRV_UNKNOWN = 0,
	QSRV_MODULE = 1,
	QSRV_FUNCTION = 2
};

/** Data Types (Implementation Private) *************************************/
/*********** ZBC OPTIMIZATION start*****************/
typedef	struct qmessage {
	unsigned int size;
	YLIST_QMSG	buffers;
#ifdef ZBC_STRUCT_ALIGN_64BYTE
	int padding[10];
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
	char *func;
	int  line;
#endif
} qmessage;

/** refcount of qpseg has been changed from unsigned short int(size = 2) to
 * unsigned int(size = 4) so that it can be used in atomic built-ins.
 * If atomic built-ins exist for 16bit variables also, refcount can be
 * converted back into unsigned short, padding2[2] can be removed and 16-bit
 * atomic built-ins can be used for refcount.
 **/
typedef	struct qpseg {
	/** SPR 13295 Changes Start **/
	unsigned int refcount;
	unsigned short int size;
	unsigned char padding2[2];
	/** SPR 13295 Changes End **/
	unsigned char *data;
	void (*free)(void *, void *);
	void *freearg;
#ifdef ZBC_STRUCT_ALIGN_64BYTE
	int padding[10];
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
	char *func;
	int  line;
#endif
} qpseg;
/*********** ZBC OPTIMIZATION End*****************/

typedef	struct	qpctl {
	YLNODE	__header__;

	qpseg *segment;
	unsigned char *start;
	unsigned char *end;
#ifdef ZBC_STRUCT_ALIGN_64BYTE
	int padding[9];
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
	char *func;
	int  line;
#endif
} qpctl;

typedef struct qpool {
	unsigned long   magic;
	unsigned int    size, nbuf, alloc, peakVal;
	YLIST chunks;
	YPOOL list;

	QLOCK lock;
} qpool;

typedef struct qbuf {
	union {
		YPNODE  __header__;
		qpool   *pool;
	} u;

	unsigned char   allocated;
#ifdef CSPL_LEAK_DEBUG_LITE
	char *func;
	int  line;
#endif
#ifdef MEM_FENCING
	void *protectaddr;
#endif
#ifdef CSPL_LEAK_DEBUG
	int nptrs;
	/* Max. No. of frames in Backtrace */
#define MAX_FRAMES_IN_BT 10
	void *buffer[MAX_FRAMES_IN_BT];
#endif
} qbuf;

typedef struct qchunk {
	YLNODE  __header__;
	unsigned int nbuf;
} qchunk;

typedef struct qvcontext {
    unsigned char loglevel;
    QTUNING tuning;
    QSYSOP  op;
    QLOCK   lock;
    void    *key;
    qpool   *alloc[NVARMSGPOOL];
/*********** ZBC OPTIMIZATION start*****************/
    qpool   *modulepool, *pctlpool, *qmsgpool;
/*********** ZBC OPTIMIZATION End*****************/
} qcontext;

typedef struct qvars {
	YLNODE  __header__;
	QMODULE self;
	QTIME   lastexpiry;
	QTIME   now;
	QTIME   nextexpiry;

	YLIST   timertable[8][256]; /*2 Dimensional Array of Timer Lists*/

	YSQUEUE queue;
	YSQUEUE hold;
	YLIST   modules;
	YLIST   uninitialized;

	void    (*wakeup)(void *);
	void    *argwakeup;

	struct  {
		QTIMER  timer;
		void    *message;
	} current;

	QLOCK   lock;
	QLOCK   sync;

	QLOCK   lock_mtimer;    /* Lock for handling inter-thread Module Timers. */
} qvars;


/** Global Data *************************************************************/
static qcontext *qd;
static const QTUNING __default = {
    0,
    0,
    LTE_MSG_POOL_QPCTL_MAX_BUF,
/*********** ZBC OPTIMIZATION start*****************/
    LTE_MSG_POOL_QMSG_MAX_BUF
/*********** ZBC OPTIMIZATION end*******************/
};

#define QV_LOCK()   qvMsgPoolLock(qvc->lock)
#define QV_UNLOCK() qvMsgPoolUnlock(qvc->lock)


/** Forward Declarations ****************************************************/
static qpool *__qvPoolCreate(unsigned int size, unsigned int nbuf);
static int __qvPoolExtend(qpool *pool, unsigned int nbuf);
static unsigned int __qvPoolSize(qpool *pool, unsigned int *p_alloc);
#ifdef CSPL_LEAK_DEBUG_LITE
static void	*__qvPoolAlloc(qpool *pool, char *func, int line);
static void	__qvPoolFree(void *buffer, char *func, int line);
#else
static void	*__qvPoolAlloc(qpool *pool);
static void	__qvPoolFree(void *buffer);
#endif

/** Default System Operations ***********************************************/
/* +- SPR 17777 */
static void *s_malloc(unsigned int size) {(void)size; return 0; }
static void s_free(void *m) { (void)m; }
static void *s_newlock(void) { return 0; }
static void s_lock(void *lock) { (void)lock; }
static void s_unlock(void *lock) { (void)lock; }
static void s_destroylock(void *lock) { (void)lock; }
static int s_vprintf(const char *format, va_list A) { (void)format; (void)A; return 0; }
static int s_iprintf(const char *format, va_list A) { (void)format; (void)A; return 0; }
static void s_abort(void) { *((unsigned char *)0); }
/** Locks *******************************************************************/

#ifdef CSPL_LEAK_DEBUG
void qvPrintBufferBt(qbuf * q);
void qvPrintPoolAllocBuf(qpool   *pool);
#endif

/*****************************************************************************
 * FUNCTION: qvMsgPoolNewLock
 *
 * DESCRIPTION:
 *          This function is called for getting a new lock,
 *          which internally calls the 'newlock' function
 *          defined in QSYSOP structure for getting the new lock.
 *
 * RETURNS:
 *          Lock
 *
 ******************************************************************************/
QLOCK qvMsgPoolNewLock(void)
{
	return (QLOCK)qd->op.newlock();
}
/*****************************************************************************
 * FUNCTION: qvMsgPoolLock
 *
 * DESCRIPTION:
 *          This function is called for activating
 *          the lock. 'lock' function defined in QSYSOP structure
 *          is called in turn.
 *
 * ARGS:
 *          Lock id returned from qvMsgPoolNewLock
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvMsgPoolLock(QLOCK lock)
{
	qd->op.lock((void *)lock);
}
/*****************************************************************************
 * FUNCTION: qvMsgPoolUnlock
 *
 * DESCRIPTION:
 *          This function is called for deactivating
 *          the lock. 'unlock' function defined in QSYSOP structure
 *          is called in turn.
 *
 * ARGS:
 *          Lock id returned from qvMsgPoolNewLock
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvMsgPoolUnlock(QLOCK lock)
{
	qd->op.unlock((void *)lock);
}

/*****************************************************************************
 * FUNCTION: qvMsgPoolDestroyLock
 *
 * DESCRIPTION:
 *          This function is called for destroying (free)
 *          the lock. 'destroylock' function defined in QSYSOP
 *          structure is called in turn.
 * ARGS:
 *          Lock id returned from qvMsgPoolNewLock
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvMsgPoolDestroyLock(QLOCK lock)
{
	qd->op.destroylock((void *)lock);
}


/** Library Initialization **************************************************/

/*****************************************************************************
* FUNCTION: qvMsgPoolInit
*
* DESCRIPTION:
*          Performs the initilization of the library and threads.
*          It registers with the OS services, tunes the library and
*          allocates memory for various pools.
*
*
* ARGS:
*          op          Pointer to QSYSOP structure specifying OS services
*                      to be accessed.
*
* RETURNS:
*
******************************************************************************/

void qvMsgPoolInit(const QSYSOP *op)
{
	int cErr = QVERROR_NONE;

	qvMsgPoolInitEx(op, &cErr);
	if (cErr == QVERROR_SYS_ALLOC_FAIL)
		qvPrintf("Unable to allocate memory for Context");

	return;
}


void qvMsgPoolInitEx(const QSYSOP *op, int *cErr)
{
	unsigned int    i;
	unsigned int    size = MINBUF;

	/* If required, Reset the Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	/* +COVERITY 10230 */
	qd = op->malloc ? (qcontext *)op->malloc(sizeof(qcontext)) :
						(qcontext *)s_malloc(sizeof(qcontext));
	/* -COVERITY 10230 */
	if (!qd) {
		if (cErr)
		    *cErr = QVERROR_SYS_ALLOC_FAIL;

		return;
	}

	qd->op.malloc = op->malloc ? op->malloc : s_malloc;
	qd->op.free = op->free ? op->free : s_free;
	qd->op.newlock = op->newlock ? op->newlock : s_newlock;
	qd->op.destroylock = op->destroylock ? op->destroylock : s_destroylock;
	qd->op.lock = op->lock ? op->lock : s_lock;
	qd->op.unlock = op->unlock ? op->unlock : s_unlock;
	qd->op.vprintf = op->vprintf ? op->vprintf : s_vprintf;
	qd->op.iprintf = op->iprintf ? op->iprintf : s_iprintf;
	qd->op.abort = op->abort ? op->abort : s_abort;

	for (i = 0; i < NVARMSGPOOL; i++, size *= 2) {
		qd->alloc[i] = __qvPoolCreate(size, 0);
		if (!qd->alloc[i]) {
			if (cErr)
				*cErr = QVERROR_SYS_ALLOC_FAIL;
			return;
		}
	}
	qd->tuning = __default;

	qd->pctlpool = __qvPoolCreate(sizeof(qpctl), qd->tuning.npctls);
	if (!qd->pctlpool) {
		if (cErr)
		    *cErr = QVERROR_SYS_ALLOC_FAIL;
		return;
	}

	/*********** ZBC OPTIMIZATION start*****************/
	qd->qmsgpool = __qvPoolCreate(sizeof(qmessage), qd->tuning.nqmsg);
	if (!qd->qmsgpool) {
		printf("qmsg pool alloc failure\n");
		if (cErr)
		    *cErr = QVERROR_SYS_ALLOC_FAIL;
		return;
	}
	/*********** ZBC OPTIMIZATION end*******************/

	qd->loglevel = 255;
	qd->lock = qvMsgPoolNewLock();
}

/** Memory Pool Management **************************************************/

#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgPoolAllocEx(unsigned int size, unsigned int *psize, int *cErr, char *func, int line)
#else
void *qvMsgPoolAllocEx(unsigned int size, unsigned int *psize, int *cErr)
#endif
{
	unsigned int i, n;

	/* If required, Reset the Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	for (i = 0, n = MINBUF; i < NVARMSGPOOL; i++, n *= 2) {
		if (size <= n) {
			qpool *q = (qpool *)qd->alloc[i];
#ifdef CSPL_LEAK_DEBUG_LITE
			void *p = __qvPoolAlloc(q, func, line);
#else
			void *p = __qvPoolAlloc(q);
#endif

#ifdef UE_SIM_TESTING
			if (!p) {
				unsigned int nbuf = __qvPoolSize(q, 0);

				__qvPoolExtend(q, nbuf?nbuf:1);
				p = __qvPoolAlloc(q);
			}

			/* Coverity 10725 */
			if (!p) {
				if (cErr)
					*cErr = QVERROR_SYS_ALLOC_FAIL;
			}



	/* Coverity 10725 */
#else
			if (!p)
				continue;
#endif
			if (p && psize)
				*psize = q->size;

			return p;
		}
	}

	/*
	* No pool available for the required bufsize!
	*/
	if (cErr)
		*cErr = QVERROR_MEM_ALLOC_ILLEGAL_SIZE;

	return 0;
}


/*****************************************************************************
 * FUNCTION: __qvPoolCreate
 *
 * DESCRIPTION:
 *			This function creates a context pool of specified size of buffer
 *			and the number of buffers in the pool.
 *			Used to create default pools at the time of Init, and pools
 *			for modules, timers and pctl pools.
 *
 * ARGS:
 *			size				size of each buffer in the pool
 *			nbuf				number of buffers in the pool
 *
 * RETURNS:
 *			Pointer to the allocated pool
 *
 ******************************************************************************/
static qpool *__qvPoolCreate(unsigned int size, unsigned int nbuf)
{
	qpool *pool;

	/** Allocate the memory **/
	pool = (qpool *)qd->op.malloc(YALIGN(sizeof(qpool), 8));
	if (!pool)
		return 0;

	ypInit(&(pool->list));
	ylInit(&(pool->chunks));
	pool->size = size;
	pool->nbuf = 0;
	pool->alloc = 0;
	pool->peakVal = 0;
	pool->magic = __POOLMAGIC;
	pool->lock = qvMsgPoolNewLock();
	/*coverity-530 CID-36231*/
	if (pool->lock == PNULL) {
		qd->op.free((void *)pool);
		return 0;
	}

	/*coverity-530 CID-36231*/
	/* SPR 11109 16-JUN-2014 fix start */
#ifndef MEM_LEAK_DEBUG
	if (nbuf) {
		if (!__qvPoolExtend(pool, nbuf)) {
			qd->op.free((void *)pool);
			return 0;
		}
	}
#endif
	/* SPR 11109 16-JUN-2014 fix end */

	return pool;
}
/*****************************************************************************
 * FUNCTION: __qvPoolExtend
 *
 * DESCRIPTION:
 *			This function extends an existing pool by 'nbuf' number
 *			of buffers.
 * ARGS:
 *			Q					Pointer to Pool.
 *			nbuf				number of buffers in the pool
 *
 * RETURNS:
 *			1 if successful
 *			0 if unsuccessful
 *
 ******************************************************************************/
#ifdef MEM_FENCING
#include <malloc.h>
#include <sys/mman.h>
#define PAGESIZE 4096
#define POOL_SIZE_TO_FENCE 2048
#define MAX_NUM_FENCE_BUF  20000

static unsigned int getChunkSize(unsigned int realsize, unsigned int nbuf)
{
	return ((((realsize / PAGESIZE) + 2) * PAGESIZE * nbuf) + PAGESIZE);
}
#endif

static int __qvPoolExtend(qpool *pool, unsigned int nbuf)
{
	qchunk *chunk;
	unsigned int size = pool->size;
	unsigned int realsize, i;
	unsigned char *p;

#ifdef MEM_FENCING
	unsigned int chunkSize = 0;
	unsigned int bufsize = 0;
	unsigned int returnExtendedMem = 0;
	void **retPtr_pp = 0;
#endif

#ifdef MEM_DEBUG
	if (nbuf > 0)
		nbuf = 1;
#endif
	/** Compute the memory required **/
	realsize = YALIGN(sizeof(qbuf), 8) + YALIGN(size, 8);

	/** Allocate the memory **/
	/*Memory is allocated/extended in chunks of nbuf buffers.
	 * This list of chunks and actual buffers is maintained in the pool.*/
#ifdef MEM_FENCING
	if (pool->size == POOL_SIZE_TO_FENCE) {
		chunkSize = getChunkSize(realsize, nbuf);
		p = (unsigned char *)memalign(PAGESIZE, chunkSize);
		if (!p)
		    perror("Failed to allocate memory");

		chunk = (qchunk *)(void *)p;
		chunk->nbuf = nbuf;
		qvMsgPoolLock(pool->lock);
		ylPushTail(&(pool->chunks), (YLNODE *)chunk);
		bufsize = ((realsize / PAGESIZE) + 2) * PAGESIZE;
		for (i = 0, p += PAGESIZE; i < nbuf; i++, p += bufsize) {
#ifdef UNDERRUN_PROTECT
			qbuf *q = (qbuf *)(void *)p;
#else
			qbuf *q = (qbuf *)(((char *)p) + (((realsize / PAGESIZE) + 1)*PAGESIZE) - realsize);
#endif
		    q->allocated = 0;
		    q->protectaddr = p;
			/* SPR 21829 Start */
#ifdef LOCKLESS_IMPL
		    ypPutAtomic(&(pool->list), (YPNODE *)q);
#else
		    ypPut(&(pool->list), (YPNODE *)q);
#endif
	/* SPR 21829 End */
			if (-1 == mprotect(p, bufsize, PROT_READ)) {
				perror("mprotect failed\n");
				abort_wrapper();
			}
		}

		pool->nbuf += nbuf;

		qvMsgPoolUnlock(pool->lock);
	} else {
		p = (unsigned char *)qd->op.malloc(YALIGN(sizeof(qchunk), 8) + (nbuf * realsize));
		if (!p)
			return 0;	/** Failed **/

		chunk = (qchunk *)(void *)p;
		chunk->nbuf = nbuf;

		qvLock(pool->lock);

		ylPushTail(&(pool->chunks), (YLNODE *)chunk);
		for (i = 0, p += YALIGN(sizeof(qchunk), 8); i < nbuf; i++, p += realsize) {
			qbuf *q = (qbuf *)(void *)p;

			q->allocated = 0;
			/* SPR 21829 Start */
#ifdef LOCKLESS_IMPL
			ypPutAtomic(&(pool->list), (YPNODE *)q);
#else
			ypPut(&(pool->list), (YPNODE *)q);
#endif
		/* SPR 21829 End */
		}

		pool->nbuf += nbuf;
		/*code to return immediately extended memory*/
		if (returnExtendedMem) {
		   void	*p;
		/* SPR 21829 Start */
#ifdef LOCKLESS_IMPL
			if ((p = ypGetAtomic(&pool->list)) != 0)
			    atomicIncrement(&pool->alloc);
#else
			if ((p = ypGet(&pool->list)) != 0)
			    pool->alloc++;
#endif
			/* SPR 21829 End */
			if (p) {
				unsigned char *c;
				qbuf *q = (qbuf *)p;

				q->allocated = 1;
				q->u.pool = pool;
				c = (unsigned char *)q;
				c += YALIGN(sizeof(qbuf), 8);

				*retPtr_pp = (void *)c;
			} else {
				*retPtr_pp = 0;
			}
		}
		/*end*/
		qvUnlock(pool->lock);
	}
#else
	p = (unsigned char *)qd->op.malloc(YALIGN(sizeof(qchunk), 8) + (nbuf * realsize));
	if (!p)
		return 0;	/** Failed **/

	chunk = (qchunk *)(void *)p;
	chunk->nbuf = nbuf;

	qvMsgPoolLock(pool->lock);

	ylPushTail(&(pool->chunks), (YLNODE *)chunk);
	for (i = 0, p += YALIGN(sizeof(qchunk), 8); i < nbuf; i++, p += realsize) {
		qbuf *q = (qbuf *)(void *)p;

		q->allocated = 0;
#ifdef LOCKLESS_IMPL
		ypPutAtomic(&(pool->list), (YPNODE *)q);
#else
		ypPut(&(pool->list), (YPNODE *)q);
#endif
	}
	pool->nbuf += nbuf;

	qvMsgPoolUnlock(pool->lock);
#endif
	return 1;
}
/*****************************************************************************
 * FUNCTION: __qvPoolSize
 *
 * DESCRIPTION:
 *			This function provides the size of pool in terms of number
 *			of buffers in the pool. If the second argument is
 *			not null, it is set to the number of buffers already
 *			allocated in the pool.
 * ARGS:
 *			Q					Pointer to Pool.
 *			p_alloc				Out param, returns number of buffers
 *								already allocated in the pool.
 *
 * RETURNS:
 *			Size of the pool
 ******************************************************************************/
static unsigned int	__qvPoolSize(qpool *pool, unsigned int *p_alloc)
{
	unsigned int nbuf, nalloc;

	qvMsgPoolLock(pool->lock);
	nbuf = pool->nbuf;
	nalloc = pool->alloc;
	qvMsgPoolUnlock(pool->lock);

	if (p_alloc)
		*p_alloc = nalloc;

	return nbuf;
}
/*****************************************************************************
 * FUNCTION: __qvPoolAlloc
 *
 * DESCRIPTION:
 *			This function allocates a buffer from the pool.
 * ARGS:
 *			pool					Pointer to Pool.
 *
 * RETURNS:
 *			Pointer to the buffer allocated
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
static void	*__qvPoolAlloc(qpool *pool, char *func, int line)
#else
static void	*__qvPoolAlloc(qpool *pool)
#endif
{
	void *p;
#ifdef MEM_DEBUG
	void *k;
#endif

#ifdef MEM_LEAK_DEBUG
	p = malloc_wrapper(pool->size + YALIGN(sizeof(qbuf), 8));
	pool->alloc++;
#else
#ifdef LOCKLESS_IMPL
	if ((p = ypGetAtomic(&pool->list)) != 0)
		atomicIncrement(&pool->alloc);
#else /* LOCKLESS_IMPL */
	qvMsgPoolLock(pool->lock);
	if ((p = ypGet(&pool->list)) != 0)
		pool->alloc++;

	qvMsgPoolUnlock(pool->lock);
#endif /* LOCKLESS_IMPL */
#endif

	if (p) {
		unsigned char *c;
		qbuf *q = (qbuf *)p;
#ifdef CSPL_LEAK_DEBUG_LITE
		if (q->allocated == 1)
			fprintf(stderr, "########## Double Allocation detected %p %s %d %s %d ##########\n", q, q->func, q->line, func, line);
		q->func = func;
		q->line = line;
#endif

#ifdef MEM_FENCING
		if (pool->size == POOL_SIZE_TO_FENCE) {
			/*Fix Jira-374 start*/
			int protSize = ((pool->size / PAGESIZE) + 1) * PAGESIZE;

			/*Fix Jira-374 end*/
			if (-1 == mprotect(q->protectaddr, protSize, PROT_READ|PROT_WRITE)) {
				printf("protSze:%d q->protectaddr:%x errno:%d\n", protSize, q->protectaddr, errno);
				perror("unprotect failed\n");
				abort_wrapper();
			}
		}
#endif
		q->allocated = 1;
		q->u.pool = pool;
#ifdef CSPL_LEAK_DEBUG
		q->nptrs = backtrace(q->buffer, MAX_FRAMES_IN_BT);
#endif
		c = (unsigned char *)q;
		c += YALIGN(sizeof(qbuf), 8);

		if (pool->alloc > pool->peakVal)
		pool->peakVal = pool->alloc;
		return (void *)c;
	} else {
#ifdef MEM_DEBUG
		if (!__qvPoolExtend(pool, 1))
			return 0;

#ifdef CSPL_LEAK_DEBUG_LITE
		k = __qvPoolAlloc(pool, func, line);
#else
		k = __qvPoolAlloc(pool);
#endif
		return k;
#else
		return 0;
#endif
	}
}
/*****************************************************************************
 * FUNCTION: __qvPoolFree
 *
 * DESCRIPTION:
 *			This function frees the buffer, which was previously
 *			allocated from a pool. The buffer is returned back to
 *			the list of free buffers of pool to which it belonged.
 * ARGS:
 *			buffer			Pointer to the buffer to be freed.
 *
 * RETURNS:
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
static void	__qvPoolFree(void *buffer, char *func, int line)
#else
static void	__qvPoolFree(void *buffer)
#endif
{
	unsigned char *c = (unsigned char *)buffer;
	qbuf *q;
	qpool *pool;

	c -= YALIGN(sizeof(qbuf), 8);
	q = (qbuf *)(void *)c;
	pool = q->u.pool;
	/* Safe check is introduced to avoid double free */
	if (q->allocated == 0) {
#ifdef CSPL_LEAK_DEBUG_LITE
		fprintf(stderr, "########## Double Free detected %p %s %d ##########\n", q, func, line);
#endif
#ifdef CSPL_LEAK_DEBUG
		fprintf(stderr, "########## Double Free detected ##########\n");
		fprintf(stderr, "First Free done by :-\n");
		/* print backtrace for double free */
		qvPrintBufferBt(q);
		fprintf(stderr, "Second Free done by :-\n");
		/* print current backtrace*/
		qvPrintCurrentBt();
#endif
		return;
	}

	q->allocated = 0;
#ifdef CSPL_LEAK_DEBUG_LITE
	q->func = func;
	q->line = line;
#endif
#ifdef CSPL_LEAK_DEBUG
	q->nptrs = backtrace(q->buffer, MAX_FRAMES_IN_BT);
#endif

#ifdef MEM_LEAK_DEBUG
	free(q);
		pool->alloc--;
#else

#ifdef LOCKLESS_IMPL
	ypPutAtomic(&(pool->list), (YPNODE *)q);
	atomicDecrement(&pool->alloc);

#else /* LOCKLESS_IMPL */
	qvMsgPoolLock(pool->lock);
#ifdef MEM_DEBUG
	c -= YALIGN(sizeof(qchunk), 8);
	ylDelete(&(pool->chunks), (YLNODE *)c);
	qd->op.free(c);
#else
	ypPut(&(pool->list), (YPNODE *)q);
#endif
#ifdef MEM_FENCING
	if (pool->size == POOL_SIZE_TO_FENCE) {
		int protSize = ((q->u.pool->size / PAGESIZE) + 1)*PAGESIZE;

		if (-1 == mprotect(q->protectaddr, protSize, PROT_READ)) {
			perror("protect failed\n");
			abort_wrapper();
			}
	}
#endif
	pool->alloc--;
#ifdef MEM_DEBUG
	pool->nbuf--;
#endif
	qvMsgPoolUnlock(pool->lock);
#endif /* LOCKLESS_IMPL */
#endif
}

/*********** ZBC OPTIMIZATION start*****************/
/*****************************************************************************
 * FUNCTION: NEWQMSG
 *
 * DESCRIPTION:  Allocate a zero cpy buffer from qmsg pool.
 *				If short of memory , extend the pool size
 *
 * ARGS:
 *				None
 * RETURNS:		Pointer to the buffer allocated from pool.
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
static qmessage *NEWQMSG(char *func, int line)
#else
static qmessage *NEWQMSG(void)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	void *p = __qvPoolAlloc(qd->qmsgpool, func, line);
#else
	void *p = __qvPoolAlloc(qd->qmsgpool);
#endif

	if (!p && !qd->tuning.nqmsg) {
		qpool *q = (qpool *)qd->qmsgpool;
		unsigned int n = __qvPoolSize(q, 0);

		__qvPoolExtend(q, n ? n : 1);
#ifdef CSPL_LEAK_DEBUG_LITE
		p = __qvPoolAlloc(qd->qmsgpool, func, line);
#else
		p = __qvPoolAlloc(qd->qmsgpool);
#endif
	}
#ifdef CSPL_LEAK_DEBUG_LITE
	((qmessage *)p)->func = func;
	((qmessage *)p)->line = line;
#endif

	return (qmessage *)p;
}
/*********** ZBC OPTIMIZATION end*******************/

/*****************************************************************************
 * FUNCTION: NEWPCTL
 *
 * DESCRIPTION:  Allocate a zero cpy buffer from pctl pool.
 *				If short of memory , extend the pool size
 *
 * ARGS:
 *				None
 * RETURNS:		Pointer to the buffer allocated from pool.
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
static qpctl *NEWPCTL(char *func, int line)
#else
static qpctl *NEWPCTL(void)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	void *p = __qvPoolAlloc(qd->pctlpool, func, line);
#else
	void *p = __qvPoolAlloc(qd->pctlpool);
#endif

	if (!p && !qd->tuning.npctls) {
		qpool *q = (qpool *)qd->pctlpool;
		unsigned int n = __qvPoolSize(q, 0);

		__qvPoolExtend(q, n ? n : 1);
#ifdef CSPL_LEAK_DEBUG_LITE
		p = __qvPoolAlloc(qd->pctlpool, func, line);
#else
		p = __qvPoolAlloc(qd->pctlpool);
#endif
	}
#ifdef CSPL_LEAK_DEBUG_LITE
	((qpctl *)p)->func = func;
	((qpctl *)p)->line = line;
#endif

	return (qpctl *)p;
}


/*****************************************************************************
 * FUNCTION: qvCleanupMsgPool
 *
 * DESCRIPTION:
 *           This function frees all the memory allocated for context
 *           message pools.
 *
 * ARGS:
 *
 * RETURNS:
 *           None
 *
 *****************************************************************************/
void qvCleanupMsgPool(void)
{
	unsigned int i, n;
	YLNODE *ylNode_p = NULL;

	if (qd) {
		for (i = 0, n = MINBUF; i < NVARPOOL; i++, n *= 2) {
			qpool *q = (qpool *)qd->alloc[i];

			if (q) {
				/* Freeing the chunks of memory allocated during
				poolCreate or poolExtend */
				while ((ylNode_p = ylPopHead(&(q->chunks)))) {
					qd->op.free((void *)(ylNode_p));
					ylNode_p = NULL;
				}

				qvMsgPoolDestroyLock(q->lock);
				qd->op.free((void *)(q));
				qd->alloc[i] = NULL;
			}
		}
		/* Free the memory allocated for qd context*/
		qvMsgPoolDestroyLock(qd->lock);
		qd->op.free((void *)(qd));
		qd = NULL;
	}
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
 *			This function frees the segment  when the reference count
 *			reaches zero.
 * ARGS:
 *			segment			Pointer to the segment
 *
 * RETURNS:
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
static void __qvMsgFreeSeg(qpseg *segment, char *func, int line)
#else
static void __qvMsgFreeSeg(qpseg *segment)
#endif
{
	/** SPR 13295 Changes Start **/
	atomicDecrement(&segment->refcount);
	/** SPR 13295 Changes End **/
	if (!segment->refcount) {
		if (segment->free)
			segment->free(segment->data, segment->freearg);
#ifdef CSPL_LEAK_DEBUG_LITE
		__qvPoolFree(segment, func, line);
#else
		__qvPoolFree(segment);
#endif
	}
}
/*****************************************************************************
 * FUNCTION: __qvMsgSplitCtl
 *
 * DESCRIPTION:
 *			Split c present in q, at the offset and return pointer
 to the first splitted block. c now points to second
 split block
 * ARGS:
 *			q		Pointer to the message
 *			c		Pointer to the qpctl buffer to be split
 *			offset	Position at which c is to be split
 *
 * RETURNS:
 *			Pointer to the first splitted block
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
#define __qvMsgSplitCtl(q, c, offset, cErr) ___qvMsgSplitCtl(q, c, offset, cErr, func, line)
static qpctl *___qvMsgSplitCtl(qmessage *q, qpctl *c, int offset, int *cErr, char *func, int line)
#else
static qpctl *__qvMsgSplitCtl(qmessage *q, qpctl *c, int offset, int *cErr)
#endif
{
	qpctl *c_prev;

	qvAssert(offset);
	qvAssert(offset < (c->end - c->start));

	/*c_prev shall point to the first splitted block*/
#ifdef CSPL_LEAK_DEBUG_LITE
	c_prev = NEWPCTL(func, line);
#else
	c_prev = NEWPCTL();
#endif
	if (!c_prev) {
		if (cErr)
			*cErr = QVERROR_PCTL_ALLOC_FAIL;
		return 0;
	}
	/*Make c_prev point to the same segment*/
	c_prev->segment = c->segment;

	/*increment  number of pctls refering to this segment*/

	/** SPR 13295 Changes Start **/
	atomicIncrement(&c->segment->refcount);
	/** SPR 13295 Changes End **/

	c_prev->start = c->start;

	/*End of the first splitted block*/
	c_prev->end = c->start + offset;

	/*Start of the second splitted block*/
	c->start = c_prev->end;

	/*Insert c_prev before c in the ylist of q*/
	ylInsertBefore((YLIST *)&(q->buffers), (YLNODE *)c, (YLNODE *)c_prev);
	return c_prev;
}

/*****************************************************************************
 * FUNCTION: __qvMsgLocate
 *
 * DESCRIPTION:
 *			This function is called to get the offset of 'location' present in
 *			the message pointed by q
 * ARGS:
 *			q			Pointer to the message
 *			location	Nth location whose offset is to be determined
 *			poffset		OutParam, returns the offset of location
 *
 * RETURNS:
 *			Pointer of the node which has the 'location'
 *
 ******************************************************************************/
static qpctl *__qvMsgLocate(const qmessage *q, unsigned int location,
							unsigned int *poffset, int *cErr)
{
	unsigned int here = 0;
	qpctl *c;

	if (location < q->size) {
		c = (qpctl *)ylFirst((const YLIST *)&(q->buffers));
		while (c && (location >= (here + (c->end - c->start)))) {
			here += (c->end - c->start);
			c = (qpctl *)ylNext((YLNODE *)c);
		}
		*poffset = location - here;
		return c;
	} else {
		if (cErr)
			*cErr = QVERROR_MEM_ILLEGAL_LOCATION;
		return 0;
	}
}
/*****************************************************************************
 * FUNCTION: __qvMsgDetachSeg
 *
 * DESCRIPTION:
 *			This function removes the zero cpy buffer from q and adjusts the
 *			size of q appropriately
 * ARGS:
 *			q			Pointer to the message
 *			c			Pointer to the qpctl buffer to be deleted
 *
 * RETURNS:
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
static void	__qvMsgDetachSeg(qmessage *q, qpctl *c, char *func, int line)
#else
static void __qvMsgDetachSeg(qmessage *q, qpctl *c)
#endif
{
	ylDelete((YLIST *)&(q->buffers), (YLNODE *)c);
	q->size -= (c->end - c->start);
#ifdef CSPL_LEAK_DEBUG_LITE
	__qvMsgFreeSeg(c->segment, func, line);
	__qvPoolFree(c, func, line);
#else
	__qvMsgFreeSeg(c->segment);
	__qvPoolFree(c);
#endif
}
/*****************************************************************************
 * FUNCTION: __qvMsgAddNewSeg
 *
 * DESCRIPTION:
 *			This function adds the new allocated segment to the message.
 * ARGS:
 *			q			Pointer to the message
 *			before		Node in segments' list before which new seg
 *						is to be added
 *			size		Size of data in new segment
 *			headroom	Size of memory to be allocated before the payload
 *			tailroom	Size of memory to be allocated after the payload
 *
 *
 * RETURNS:
 *			Pointer to the qpctl buffer added in q
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
#define __qvMsgAddNewSeg(q, before, size, headroom, tailroom, cErr) \
      ___qvMsgAddNewSeg(q, before, size, headroom, tailroom, cErr, func, line)
static qpctl *___qvMsgAddNewSeg(qmessage *q,
								qpctl *before, unsigned int size,
								unsigned int headroom, unsigned int tailroom,
								int *cErr, char *func, int line)
#else
static qpctl *__qvMsgAddNewSeg(qmessage *q,
								qpctl *before, unsigned int size,
								unsigned int headroom, unsigned int tailroom,
								int *cErr)
#endif
{
	unsigned int realsize, n = YALIGN(sizeof(qpseg), 8) + headroom + tailroom + size;
	qpctl	*c;
	qpseg	*s;
	unsigned char *p;

#ifdef CSPL_LEAK_DEBUG_LITE
	c = NEWPCTL(func, line);
#else
	c = NEWPCTL();
#endif
	if (!c) {
		if (cErr)
			*cErr = QVERROR_PCTL_ALLOC_FAIL;
		return 0;
	}

#ifdef CSPL_LEAK_DEBUG_LITE
	p = (unsigned char *)qvMsgPoolAllocEx(n, &realsize, cErr, func, line);
#else
	p = (unsigned char *)qvMsgPoolAllocEx(n, &realsize, cErr);
#endif
	if (!p) {						/* Error code set inside */
#ifdef CSPL_LEAK_DEBUG_LITE
		__qvPoolFree(c, func, line);
#else
		__qvPoolFree(c);
#endif
		return 0;
	}

	s = (qpseg *)(void *)p;

	s->data = p + YALIGN(sizeof(qpseg), 8);/*Point to start of data*/
	s->size = realsize - (s->data - p);/*total size minus size of qpseg hdr*/
#ifdef CSPL_LEAK_DEBUG_LITE
	s->func = func;
	s->line = line;
#endif

	c->segment = s;
	c->start = s->data + headroom;
	c->end = c->start + size;
	s->refcount = 1;

	s->free = 0; s->freearg = 0;

	if (!before)
		ylPushTail((YLIST *)&(q->buffers), (YLNODE *)c);
	else
		ylInsertBefore((YLIST *)&(q->buffers), (YLNODE *)before, (YLNODE *)c);

	q->size += size;
	return c;
}
/*****************************************************************************
 * FUNCTION: __qvMsgAddNewSegExternal
 *
 * DESCRIPTION:
 *			This function is used for inserting external data
 *			at a specified location in a message
 * ARGS:
 *			q			Pointer to the message
 *			before		Node in segments' list before which new seg
 *						is to be added
 *			size		Size of data in ext buffer
 *			extbuf		Pointer to the external buffer to be added as segment
 *			free		Pointer to the function used to free the memory
 *						block
 *			freearg		Pointer to the argument to be passed to free function
 *
 * RETURNS:
 *			Pointer to the qpctl buffer added in q
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
#define __qvMsgAddNewSegExternal(q, before, size, extbuf, freefn, freearg, cErr) \
	___qvMsgAddNewSegExternal(q, before, size, extbuf, freefn, freearg, cErr, func, line)

static qpctl *___qvMsgAddNewSegExternal(qmessage *q,
		qpctl *before, unsigned int size, unsigned char *extbuf,
		void (*freefn)(void *, void *), void *freearg,
		int *cErr, char *func, int line)
#else
static qpctl *__qvMsgAddNewSegExternal(qmessage *q,
		qpctl *before, unsigned int size, unsigned char *extbuf,
		void (*freefn)(void *, void *), void *freearg,
		int *cErr)
#endif
{
	qpctl	*c;
	qpseg	*s;

#ifdef CSPL_LEAK_DEBUG_LITE
	c = NEWPCTL(func, line);
#else
	c = NEWPCTL();
#endif
	if (!c) {
		if (cErr)
		    *cErr = QVERROR_PCTL_ALLOC_FAIL;
		return 0;
	}

#ifdef CSPL_LEAK_DEBUG_LITE
	s = (qpseg *)qvMsgPoolAllocEx(sizeof(qpseg), 0, cErr, func, line);
#else
	s = (qpseg *)qvMsgPoolAllocEx(sizeof(qpseg), 0, cErr);
#endif
	if (!s) {
#ifdef CSPL_LEAK_DEBUG_LITE
		__qvPoolFree(c, func, line);
#else
		__qvPoolFree(c);
#endif
		return 0;
	}

	s->data = extbuf;
	s->size = size;
#ifdef CSPL_LEAK_DEBUG_LITE
	s->func = func;
	s->line = line;
#endif

	c->segment = s;
	c->start = s->data;
	c->end = c->start + size;
	s->refcount = 1;

	s->free = freefn; s->freearg = freearg;

	if (!before)
		ylPushTail((YLIST *)&(q->buffers), (YLNODE *)c);
	else
		ylInsertBefore((YLIST *)&(q->buffers), (YLNODE *)before, (YLNODE *)c);

	q->size += size;
	return c;
}
/*****************************************************************************
 * FUNCTION: qvMsgPoolCreate
 *
 * DESCRIPTION:
 *			This function creates a message pool.
 * ARGS:
 *			size				size of each buffer in the pool
 *			nbuf				number of buffers in the pool
 *
 * RETURNS:
 *			Pointer to the allocated pool
 *
 ******************************************************************************/
QMPOOL qvMsgPoolCreate(unsigned int size, unsigned int nbuf)
{
	return qvMsgPoolCreateEx(size, nbuf, 0);
}
QMPOOL qvMsgPoolCreateEx(unsigned int size, unsigned int nbuf, int *cErr)
{
	QMPOOL	qm_pool;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	size += YALIGN(sizeof(qmessage), 8);
	qm_pool = (QMPOOL)__qvPoolCreate(size, nbuf);
	if (!qm_pool) {
		if (cErr)
			*cErr = QVERROR_SYS_ALLOC_FAIL;
	}

	return qm_pool;
}
/*****************************************************************************
 * FUNCTION: qvMsgPoolSize
 *
 * DESCRIPTION:
 *			This function provides the size of pool in terms of number
 *			of buffers in the pool. If the second argument is
 *			not null, it is set to the number of buffers already
 *			allocated in the pool.
 * ARGS:
 *			Q					Pointer to Pool.
 *			p_alloc				Out param, returns number of buffers
 *								already allocated in the pool.
 *
 * RETURNS:
 *			Size of the pool
 *
 ******************************************************************************/
unsigned int qvMsgPoolSize(QMPOOL Q, unsigned int *p_alloc)
{
	return __qvPoolSize((qpool *)Q, p_alloc);
}
/*****************************************************************************
 * FUNCTION: qvMsgPoolExtend
 *
 * DESCRIPTION:
 *			This function extends an existing pool by 'nbuf' number
 *			of buffers.
 * ARGS:
 *			Q					Pointer to Pool.
 *			nbuf				number of buffers in the pool
 *
 * RETURNS:
 *			1 if successful
 *			0 if unsuccessful
 *
 ******************************************************************************/
int	qvMsgPoolExtend(QMPOOL Q, unsigned int nbuf)
{
	return qvMsgPoolExtendEx(Q, nbuf, 0);
}
int	qvMsgPoolExtendEx(QMPOOL Q, unsigned int nbuf, int *cErr)
{
    int	p;
    /* If required, Reset the CSPL Error Code. */
    if (cErr)
		*cErr = QVERROR_NONE;

    p = __qvPoolExtend((qpool *)Q, nbuf);
    if (!p) {
		if (cErr)
		    *cErr = QVERROR_SYS_ALLOC_FAIL;
    }

    return p;
}

/*****************************************************************************
 * FUNCTION: qvMsgAlloc
 *
 * DESCRIPTION:
 *			This function allocates a buffer for a message from the pool.
 * ARGS:
 *			pool					Pointer to Pool.
 *
 * RETURNS:
 *			Pointer to the buffer allocated
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgAlloc(QMPOOL pool, unsigned int headroom, unsigned int tailroom, unsigned int attrsize, char *func, int line)
#else
void *qvMsgAlloc(QMPOOL pool, unsigned int headroom, unsigned int tailroom, unsigned int attrsize)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgAllocEx(pool, headroom, tailroom, attrsize, 0, func, line);
#else
	return qvMsgAllocEx(pool, headroom, tailroom, attrsize, 0);
#endif
}
/*********** ZBC OPTIMIZATION start*****************/
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgAllocEx(QMPOOL pool, unsigned int headroom, unsigned int tailroom, unsigned int attrsize, int *cErr, char *func, int line)
#else
void *qvMsgAllocEx(QMPOOL pool, unsigned int headroom, unsigned int tailroom, unsigned int attrsize, int *cErr)
#endif
{
	qmessage *q;
	qpctl	*c;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	if (pool) {
		if (attrsize) {
			attrsize += YALIGN(sizeof(qmessage), 8);
			if (((qpool *)pool)->size < attrsize) {
				if (cErr)
				*cErr = QVERROR_MEM_ALLOC_ILLEGAL_SIZE;
				return 0;
			}
		}
#ifdef CSPL_LEAK_DEBUG_LITE
		q = (qmessage *)__qvPoolAlloc((qpool *)pool, func, line);
#else
		q = (qmessage *)__qvPoolAlloc((qpool *)pool);
#endif
		if (!q) {
			if (cErr)
				*cErr = QVERROR_MEM_POOL_EXHAUSTED;
			return 0;
		}
#ifdef CSPL_LEAK_DEBUG_LITE
		q->func = func;
		q->line = line;
#endif
	} else {
		if (attrsize) {
			attrsize += YALIGN(sizeof(qmessage), 8);
#ifdef CSPL_LEAK_DEBUG_LITE
			q = (qmessage *)qvMsgPoolAllocEx(attrsize, 0, cErr, func, line); /* Error code set inside */
#else
			q = (qmessage *)qvMsgPoolAllocEx(attrsize, 0, cErr); /* Error code set inside */
#endif
			if (!q)
				return 0;

#ifdef CSPL_LEAK_DEBUG_LITE
			q->func = func;
			q->line = line;
#endif
		} else {
#ifdef CSPL_LEAK_DEBUG_LITE
			q = NEWQMSG(func, line);
#else
			q = NEWQMSG();
#endif
			if (!q)
				return 0;

		}
	}

	ylInit_qmsg(&(q->buffers));
	q->size = 0;

	if (headroom || tailroom) {
		c = __qvMsgAddNewSeg(q, 0, 0, headroom, tailroom, cErr);
		if (!c) {
#ifdef CSPL_LEAK_DEBUG_LITE
			__qvPoolFree(q, func, line);
#else
			__qvPoolFree(q);
#endif
			return 0;
		}
	}

	return (void *)((unsigned char *)q + YALIGN(sizeof(qmessage), 8));
}


#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgAllocAndReserve(unsigned int headroom, unsigned int tailroom, unsigned int attrsize, char *func, int line)
#else
void *qvMsgAllocAndReserve(unsigned int headroom, unsigned int tailroom, unsigned int attrsize)
#endif
{
	qmessage *q;
	qpctl *c;

	if (attrsize) {
#ifdef CSPL_LEAK_DEBUG_LITE
		q = NEWQMSG(func, line);
#else
		q = NEWQMSG();
#endif
		if (!q)
			return 0;

		ylInit_qmsg(&(q->buffers));
		q->size = 0;
		if (headroom || tailroom || attrsize) {
			c = __qvMsgAddNewSeg(q, 0, attrsize, headroom, tailroom, 0);
			if (!c) {
#ifdef CSPL_LEAK_DEBUG_LITE
			__qvPoolFree(q, func, line);
#else
			__qvPoolFree(q);
#endif
			return 0;
			}
		}
	} else {
#ifdef CSPL_LEAK_DEBUG_LITE
		q = NEWQMSG(func, line);
#else
		q = NEWQMSG();
#endif
		if (!q)
			return 0;
	}

	return	(void *)((unsigned char *)q + YALIGN(sizeof(qmessage), 8));
}

/*****************************************************************************
 * FUNCTION: qvMsgFree
 *
 * DESCRIPTION:
 *			This function requests de-allocation of memory pointed
 *			by the buffer argument. This function is used in conjunction
 *			with qvMsgAlloc.
 * ARGS:
 *			buffer			Pointer to the buffer to be freed.
 *
 * RETURNS:
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
void qvMsgFree(void *buffer, char *func, int line)
#else
void qvMsgFree(void *buffer)
#endif
{
	qmessage *q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));
	qpctl *c, *c_del;


	/*Free all the zero cpy buffers associated with q*/
	c = (qpctl *)ylFirst((const YLIST *)&(q->buffers));
	while(q->buffers.count > 1) {
		c_del = c;
		c = (qpctl *)ylNext((YLNODE *)c);

		ylDelete_first((YLIST *)&(q->buffers), (YLNODE *)c_del);
#ifdef CSPL_LEAK_DEBUG_LITE
		__qvMsgFreeSeg(c_del->segment, func, line);
		__qvPoolFree(c_del, func, line);
#else
		__qvMsgFreeSeg(c_del->segment);
		__qvPoolFree(c_del);
#endif
	}

	/* only a single buffer left */
	if (q->buffers.count == 1) {
		ylDelete_single((YLIST *)&(q->buffers), (YLNODE *)c);
#ifdef CSPL_LEAK_DEBUG_LITE
		__qvMsgFreeSeg(c->segment, func, line);
		__qvPoolFree(c, func, line);
#else
		__qvMsgFreeSeg(c->segment);
		__qvPoolFree(c);
#endif

	}

	/*Free q, and thus its non-zero cpy buffer*/
#ifdef CSPL_LEAK_DEBUG_LITE
	__qvPoolFree(q, func, line);
#else
	__qvPoolFree(q);
#endif
}
/*********** ZBC OPTIMIZATION end*******************/

/*****************************************************************************
 * FUNCTION: qvMsgSize
 *
 * DESCRIPTION:
 *			This function returns the size of buffer. 'attrsize'
 *			will be set to the size of memory that was actually allocated at
 *			the time of qvMsgAlloc call. While the function will return
 *			the size of payload attached to the message.
 * ARGS:
 *			buffer			Pointer to the buffer whose size is to be
 *							retrieved
 *			attrsize		OutParam, returns the size of
 *
 * RETURNS:
 *			Size of the buffer data present in the message
 *
 ******************************************************************************/
unsigned int qvMsgSize(const void *buffer, unsigned int *attrsize)
{
	const qmessage *q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));

	if (attrsize) {
		qbuf *m;

		m = (qbuf *)(void *)((const unsigned char *)q - YALIGN(sizeof(qbuf), 8));
		/*Non-zero cpy buffer, allocated from pool of specific size*/
		*attrsize = m->u.pool->size - YALIGN(sizeof(qmessage), 8);
	}
	return q->size;/*Size of zero cpy buffers*/
}
/*****************************************************************************
 * FUNCTION: qvMsgReserve
 *
 * DESCRIPTION:
 *			This function can be used for reserving a specified
 *			area of memory in a message buffer.
 * ARGS:
 *			buffer		Pointer to the message
 *			where		Nth byte where data is to be reserved
 *			count		Size of buffer to be reserved
 * RETURNS:
 *			Pointer to the start of reserved area
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *_qvMsgReserve(void *buffer, int where, unsigned int count, char *func, int line)
#else
unsigned char *qvMsgReserve(void *buffer, int where, unsigned int count)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgReserveEx(buffer, where, count, 0, func, line);
#else
	return qvMsgReserveEx(buffer, where, count, 0);
#endif
}
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *qvMsgReserveEx(void *buffer, int where, unsigned int count, int *cErr, char *func, int line)
#else
unsigned char *qvMsgReserveEx(void *buffer, int where, unsigned int count, int *cErr)
#endif
{
	qmessage *q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));
	qpctl *c, *c_prev;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	if ((unsigned int)where == q->size)
		where = -1;

	/*Reserve at the start*/
	if (where == 0) {
		c = (qpctl *)ylFirst((const YLIST *)&(q->buffers));
		if (!c) {
			c = __qvMsgAddNewSeg(q, 0, count, 0, 0, cErr);
			if (!c)
				return 0;

			return c->start;
		} else if (ONLY(c) && (HEADROOM(c) >= count)) {
			/*Accomodate headroom to reserve space*/
			c->start -= count;
			q->size += count;
			return c->start;
		} else {
			/*allocate and place memory block before c*/
			c = __qvMsgAddNewSeg(q, c, count, 0, 0, cErr);
			if (!c)
				return 0;
			return c->start;
		}
	} else if (where == -1) {
		/*Reserve at the end*/
		c = (qpctl *)ylLast((const YLIST *)&(q->buffers));
		if (!c) {
			c = __qvMsgAddNewSeg(q, 0, count, 0, 0, cErr);
			if (!c)
				return 0;

			return c->start;
		} else if (ONLY(c) && (TAILROOM(c) >= count)) {
			unsigned char	*r = c->end;

			c->end += count;
			q->size += count;
			return r;
		} else {
			c = __qvMsgAddNewSeg(q, 0, count, 0, 0, cErr);
			if (!c)
				return 0;

			return c->start;
		}
	} else {
		unsigned int offset;

		c = __qvMsgLocate(q, where, &offset, cErr);
		if (!c)
			return 0;

		c_prev = (qpctl *)ylPrev((YLNODE *)c);

		if (offset) {
			if (!__qvMsgSplitCtl(q, c, offset, cErr))
				return 0;

			c = __qvMsgAddNewSeg(q, c, count, 0, 0, cErr);
			if (!c)
				return 0;

			return c->start;
		} else if (ONLY(c) && (HEADROOM(c) >= count)) {
			c->start -= count;
			q->size += count;
			return c->start;
		} else if (c_prev && ONLY(c_prev) && (TAILROOM(c_prev) >= count)) {
			unsigned char	*r = c_prev->end;

			c_prev->end += count;
			q->size += count;
			return r;
		} else {
			c = __qvMsgAddNewSeg(q, c, count, 0, 0, cErr);
			if (!c)
				return 0;

			return c->start;
		}
	}
}
/*****************************************************************************
 * FUNCTION: qvMsgInsert
 *
 * DESCRIPTION:
 *			This function is used for inserting data
 *			at a specified location in a message
 * ARGS:
 *			buffer		Pointer to the message
 *			where		Nth byte where data is to be inserted
 *			data		Pointer to the external buffer to be added as segment
 *			count		Size of data in ext buffer
 * RETURNS:
 *			Pointer to data which was inserted.
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *_qvMsgInsert(void *buffer, int where, unsigned char *data, unsigned int count, char *func, int line)
#else
unsigned char *qvMsgInsert(void *buffer, int where, unsigned char *data, unsigned int count)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgInsertEx(buffer, where, data, count, 0, func, line);
#else
	return qvMsgInsertEx(buffer, where, data, count, 0);
#endif
}
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *qvMsgInsertEx(void *buffer, int where, unsigned char *data,
						unsigned int count, int *cErr, char *func, int line)
#else
unsigned char *qvMsgInsertEx(void *buffer, int where, unsigned char *data,
						unsigned int count, int *cErr)
#endif
{
	unsigned char *P;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

#ifdef CSPL_LEAK_DEBUG_LITE
	P = qvMsgReserveEx(buffer, where, count, cErr, func, line);
#else
	P = qvMsgReserveEx(buffer, where, count, cErr);
#endif
	if (P) {
		memCpy(P, data, count);
		return P;
	} else {
		return 0;
	}
}
/*****************************************************************************
 * FUNCTION: qvMsgInsertExternal
 *
 * DESCRIPTION:
 *			This function is used for inserting external data
 *			at a specified location in a message
 * ARGS:
 *			buffer		Pointer to the message
 *			where		Nth byte where data is to be inserted
 *			data		Pointer to the external buffer to be added as segment
 *			count		Size of data in ext buffer
 *			free		Pointer to the function used to free the memory
 *						block
 *			freearg		Pointer to the argument to be passed to free function
 *
 * RETURNS:
 *			Pointer to data which was inserted.
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *qvMsgInsertExternal(void *buffer, int where, unsigned char *data,
					unsigned int count,
					void (*freefn)(void *, void *), void *freearg, char *func, int line)
#else
unsigned char *qvMsgInsertExternal(void *buffer, int where, unsigned char *data,
					unsigned int count,
					void (*freefn)(void *, void *), void *freearg)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgInsertExternalEx(buffer, where, data, count, freefn, freearg, 0, func, line);
#else
	return qvMsgInsertExternalEx(buffer, where, data, count, freefn, freearg, 0);
#endif
}
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *qvMsgInsertExternalEx(void *buffer, int where, unsigned char *data,
				unsigned int count,
				void (*freefn)(void *, void *), void *freearg, int *cErr, char *func, int line)
#else
unsigned char *qvMsgInsertExternalEx(void *buffer, int where, unsigned char *data,
				unsigned int count,
				void (*freefn)(void *, void *), void *freearg, int *cErr)
#endif
{
	qmessage *q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));
	qpctl *c;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	if (where && ((unsigned int)where == q->size))
		where = -1;

	if (where == 0) {
		/*Insert data at start*/
		c = (qpctl *)ylFirst((const YLIST *)&(q->buffers));
		if (!c) {
			c = __qvMsgAddNewSegExternal(q, 0, count, data, freefn, freearg, cErr);
			if (!c)
				return 0;
		} else {
			c = __qvMsgAddNewSegExternal(q, c, count, data, freefn, freearg, cErr);
			if (!c)
				return 0;
		}
	} else if (where == -1) {/*Insert data at the end*/
		c = __qvMsgAddNewSegExternal(q, 0, count, data, freefn, freearg, cErr);
		if (!c)
			return 0;
	} else {
		unsigned int offset;

		c = __qvMsgLocate(q, where, &offset, cErr);/*Find offset of 'where'*/
		if (!c) {
			/*c is the node which has the 'where'*/
			return 0;
		}

		if (offset) {
			/*Split c present in q, at the offset and return pointer
			* to the first splitted block. c now points to second
			* split block*/
			if (!__qvMsgSplitCtl(q, c, offset, cErr))
				return 0;
			/*Add data before c, which is the second split block*/
			c = __qvMsgAddNewSegExternal(q, c, count, data, freefn, freearg, cErr);
			if (!c)
				return 0;
		} else {
			/*add data before c as where matches start of some segment in the q*/
			c = __qvMsgAddNewSegExternal(q, c, count, data, freefn, freearg, cErr);
			if (!c)
				return 0;
		}
	}

	return data;
}
/*****************************************************************************
 * FUNCTION: qvMsgRemove
 *
 * DESCRIPTION:
 *			This function can be used for removing part of data at
 *			a specified location in a message.
 * ARGS:
 *			buffer			Pointer to the message
 *			where			Nth byte from where data is to be removed
 *			count			Size of data to be removed.
 *
 * RETURNS:
 *			Number of bytes removed.
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
int	_qvMsgRemove(void *buffer, int where, unsigned int count, char *func, int line)
#else
int	qvMsgRemove(void *buffer, int where, unsigned int count)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgRemoveEx(buffer, where, count, 0, func, line);
#else
	return qvMsgRemoveEx(buffer, where, count, 0);
#endif
}
#ifdef CSPL_LEAK_DEBUG_LITE
int	qvMsgRemoveEx(void *buffer, int where, unsigned int count, int *cErr, char *func, int line)
#else
int	qvMsgRemoveEx(void *buffer, int where, unsigned int count, int *cErr)
#endif
{
	qmessage *q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));
	qpctl *c;
	unsigned int offset, remaining;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	if (where == (-1)) {
		/*Remove bytes from the end*/
		where = q->size - count;
	}

	/* If count is more than the data available to be removed beyond 'where',
	 * set it the maximum possible value */
	if (count > (q->size - where))
		count = q->size - where;

	c = __qvMsgLocate(q, where, &offset, cErr);
	if (!c)
		return 0;

	if (offset) {
		if (!__qvMsgSplitCtl(q, c, offset, cErr))
			return 0;
	}

	remaining = count;
	while (c && remaining) {
		/*Size of data other than head and tail room*/
		unsigned int n = SIZE(c);

		/*Bytes within one pctl, just adjust pointers and size.*/
		if (n > remaining) {
			c->start += remaining;
			q->size -= remaining;
			return count;
		} else {
			/*Bytes to be removed span more than one pctl*/
			qpctl *c_old = c;

			c = (qpctl *)ylNext((YLNODE *)c);
#ifdef CSPL_LEAK_DEBUG_LITE
			__qvMsgDetachSeg(q, c_old, func, line);/*Remove the pctl*/
#else
			__qvMsgDetachSeg(q, c_old);/*Remove the pctl*/
#endif
			remaining -= n;
			if (!remaining)
				return count;
		}
	}
	return 1;
}

/* ECN changes start*/
/*****************************************************************************
 * FUNCTION: qvMsgLocate
 *
 * DESCRIPTION:
 *			This function is called to get the Pointer of the 'location' in
 *			present in the message pointed by q( It is assumend that the
 *			 data indicated byu the location shall be in same node)
 * ARGS:
 *			q			Pointer to the message
 *			location	Nth location whose offset is to be determined
 *			pLocation	OutParam, returns the offset of location
 *
 * RETURNS:
 *			Pointer of the 'location' in the node
 *
 ******************************************************************************/
int qvMsgLocate(void *buffer, int where, unsigned char **pLocation)
{
	const qmessage *q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));
	int cErr = QVERROR_NONE;
	unsigned int offset;
	qpctl *c;

	c = __qvMsgLocate(q, where, &offset, &cErr);
	if (!c)
		return 0;

	/* Assiging pointer of the location */
	*pLocation = c->start + offset;
	return 1;
}
/* ECN chnages  end*/

/*****************************************************************************
 * FUNCTION: qvMsgExtract
 *
 * DESCRIPTION:
 *			This function can be used for extracting a part of data
 *			at a specified location in a message.
 *
 * ARGS:
 *			buffer			Pointer to the message
 *			where			Nth byte from where data is to be extracted
 *			data			Pointer of location where extracted data
 *							is copied
 *			count			Size of data to be extracted.
 *
 * RETURNS:
 *			Number of bytes extracted.
 *
 ******************************************************************************/
/* SPR 19288 change start */
/* SPR 19288 change end */
int	qvMsgExtract(const void *buffer, int where, unsigned char *data, unsigned int count)
{
	return qvMsgExtractEx(buffer, where, data, count, 0);
}

int	qvMsgExtractEx(const void *buffer, int where, unsigned char *data,
					unsigned int count, int *cErr)
{
	const qmessage *q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));
	qpctl *c;
	unsigned int offset, remaining;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	if (where == (-1))
		where = q->size - count;

	if (count > (q->size - where))
		count = q->size - where;

	c = __qvMsgLocate(q, where, &offset, cErr);
	if (!c)
		return 0;

	remaining = count;

	while (c && remaining) {
		if ((SIZE(c) - offset) < remaining) {
			memCpy(data + count - remaining, c->start + offset, SIZE(c)-offset);
			remaining -= SIZE(c) - offset;
			c = (qpctl *)ylNext((YLNODE *)c);
			offset = 0;
		} else {
			memCpy(data + count - remaining, c->start + offset, remaining);
			return count;
		}
	}

	return count - remaining;
}
/*****************************************************************************
 * FUNCTION: qvMsgPoolOf
 *
 * DESCRIPTION:
 *			This function returns the pool to which the buffer belongs.
 * ARGS:
 *			buffer			Pointer to the buffer
 *
 * RETURNS:
 *			Pointer to the pool to which the buffer belongs.
 *
 ******************************************************************************/
QMPOOL qvMsgPoolOf(const void *buffer)
{
	const qbuf *m;

	if (buffer) {
		m = (const qbuf *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage), 8)
								- YALIGN(sizeof(qbuf), 8));
		return (QMPOOL)m->u.pool;
	} else {
		return 0;
	}
}
#ifdef CSPL_LEAK_DEBUG_LITE
void *_qvMsgClone(const void *buffer, char *func, int line)
#else
void *qvMsgClone(const void *buffer)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgCloneEx(buffer, 0, func, line);
#else
	return qvMsgCloneEx(buffer, 0);
#endif
}
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgCloneEx(const void *buffer, int *cErr, char *func, int line)
#else
void *qvMsgCloneEx(const void *buffer, int *cErr)
#endif
{
	unsigned int attrsize;
	void *newbuf;
	qmessage *nq;
	const qmessage *q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));
	qbuf *m = (qbuf *)(void *)((const unsigned char *)q - YALIGN(sizeof(qbuf), 8));
	qpctl *p;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	attrsize = m->u.pool->size - YALIGN(sizeof(qmessage), 8);
#ifdef CSPL_LEAK_DEBUG_LITE
	newbuf = qvMsgAllocEx(m->u.pool, 0, 0, 0, cErr, func, line);
#else
	newbuf = qvMsgAllocEx(m->u.pool, 0, 0, 0, cErr);
#endif
	if (!newbuf) {
		if (cErr)
			*cErr = QVERROR_NONE;
#ifdef CSPL_LEAK_DEBUG_LITE
		newbuf = qvMsgAllocEx(0, 0, 0, attrsize, cErr, func, line);
#else
		newbuf = qvMsgAllocEx(0, 0, 0, attrsize, cErr);
#endif
	}

	if (!newbuf)
		return 0;

	nq = (qmessage *)(void *)((unsigned char *)newbuf - YALIGN(sizeof(qmessage), 8)),
	nq->size = q->size;
#ifdef CSPL_LEAK_DEBUG_LITE
	nq->func = func;
	nq->line = line;
#endif

	ylInit_qmsg(&(nq->buffers));
	for (p = (qpctl *)ylFirst((const YLIST *)&(q->buffers)); p; p = (qpctl *)ylNext((YLNODE *)p)) {
#ifdef CSPL_LEAK_DEBUG_LITE
		qpctl *c = NEWPCTL(func, line);
#else
		qpctl *c = NEWPCTL();
#endif
		if (!c) {
#ifdef CSPL_LEAK_DEBUG_LITE
			qvMsgFree(newbuf, func, line);
#else
			qvMsgFree(newbuf);
#endif
			if (cErr)
				*cErr = QVERROR_PCTL_ALLOC_FAIL;
			return 0;
		}
		c->segment = p->segment;
		c->start = p->start;
		c->end = p->end;
		/** SPR 13295 Changes Start **/
		atomicIncrement(&c->segment->refcount);
		/** SPR 13295 Changes End **/
		ylPushTail((YLIST *)&(nq->buffers), (YLNODE *)c);
	}
	return	newbuf;
}
/*****************************************************************************
 * FUNCTION: qvMsgCopy
 *
 * DESCRIPTION:
 *			This function is used for creating a copy of message.
 *			Contents of 'frombuffer' are copied to contents of 'tobuffer'.
 *			Both 'non-zero copy' and 'zero copy' buffers of the message
 *			are copied. The zero copy buffers, if any, in the 'tobuffer'
 *			are freed prior to the copy operation.
 * ARGS:
 *			tobuffer		pointer to destination buffer for copy operation
 *			frombuffer		pointer to source buffer for copy operation
 *			attrsize		size of 'non-zero copy buffer'
 *
 * RETURNS:
 *			Pointer to the 'tobuffer'
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
void *_qvMsgCopy(void *tobuffer, const void *frombuffer, unsigned int attrsize, char *func, int line)
#else
void *qvMsgCopy(void *tobuffer, const void *frombuffer, unsigned int attrsize)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgCopyEx(tobuffer, frombuffer, attrsize, 0, func, line);
#else
	return qvMsgCopyEx(tobuffer, frombuffer, attrsize, 0);
#endif
}
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgCopyEx(void *tobuffer, const void *frombuffer, unsigned int attrsize, int *cErr, char *func, int line)
#else
void *qvMsgCopyEx(void *tobuffer, const void *frombuffer, unsigned int attrsize, int *cErr)
#endif
{
	qmessage *qto = (qmessage *)(void *)((unsigned char *)tobuffer - YALIGN(sizeof(qmessage), 8));
	const qmessage *qfrom = (const qmessage *)(void *)((const unsigned char *)frombuffer - YALIGN(sizeof(qmessage), 8));
	qpctl *p;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	if (ylCount((const YLIST *)&(qto->buffers)) != 0) {
		while ((p = (qpctl *)ylPopHead((YLIST *)&(qto->buffers))) != 0) {
#ifdef CSPL_LEAK_DEBUG_LITE
			__qvMsgFreeSeg(p->segment, func, line);
			__qvPoolFree(p, func, line);
#else
			__qvMsgFreeSeg(p->segment);
			__qvPoolFree(p);
#endif
		}
	} else {
		qvAssert(qto->size == 0);
	}

	qto->size = qfrom->size;
	if (attrsize) {
		/* Check sizes */
		qbuf *m;

		m = (qbuf *)(void *)((const unsigned char *)qto - YALIGN(sizeof(qbuf), 8));
		/*Non-zero cpy buffer, allocated from pool of specific size*/
		if (attrsize > (m->u.pool->size - YALIGN(sizeof(qmessage), 8))) {
			if (cErr)
				*cErr = QVERROR_MEM_ILLEGAL_LOCATION;
			return 0;
		}
		/* Now, its safe to copy on tobuffer. */
		memCpy(tobuffer, frombuffer, attrsize);
	}

	for (p = (qpctl *)ylFirst((const YLIST *)&(qfrom->buffers)); p; p = (qpctl *)ylNext((YLNODE *)p)) {
#ifdef CSPL_LEAK_DEBUG_LITE
		qpctl *c = NEWPCTL(func, line);
#else
		qpctl *c = NEWPCTL();
#endif
		if (!c) {
			while ((p = (qpctl *)ylPopHead((YLIST *)&(qto->buffers))) != 0) {
#ifdef CSPL_LEAK_DEBUG_LITE
			__qvMsgFreeSeg(p->segment, func, line);
			__qvPoolFree(p, func, line);
#else
			__qvMsgFreeSeg(p->segment);
			__qvPoolFree(p);
#endif
			}
		if (cErr)
			*cErr = QVERROR_PCTL_ALLOC_FAIL;
		qto->size = 0;
		return 0;
		}

		c->segment = p->segment;
		c->start = p->start;
		c->end = p->end;
		/** SPR 13295 Changes Start **/
		atomicIncrement(&c->segment->refcount);
		/** SPR 13295 Changes End **/
		ylPushTail((YLIST *)&(qto->buffers), (YLNODE *)c);
	}

	return tobuffer;
}
/*****************************************************************************
 * FUNCTION: qvMsgSplit
 *
 * DESCRIPTION:
 *			This function can be used for splitting 'frombuffer'
 *			message at 'offset' position.
 *			The buffers after the split position ('offset') are moved
 *			to 'tobuffer'. The zero copy buffers, if any, in the
 *			'tobuffer' are freed prior to the moving split buffers.
 * ARGS:
 *			frombuffer		Pointer to message whose buffers are to be spilt
 *			offset			Location at which buffers are to be split
 *			tobuffer		Pointer to destination buffer where buffers
 *							after the split point are moved
 *
 * RETURNS:
 *			Pointer to the tobuffer
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
void *_qvMsgSplit(void *frombuffer, unsigned int offset, void *tobuffer, char *func, int line)
#else
void *qvMsgSplit(void *frombuffer, unsigned int offset, void *tobuffer)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgSplitEx(frombuffer, offset, tobuffer, 0, func, line);
#else
	return qvMsgSplitEx(frombuffer, offset, tobuffer, 0);
#endif
}
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgSplitEx(void *frombuffer, unsigned int offset, void *tobuffer, int *cErr, char *func, int line)
#else
void *qvMsgSplitEx(void *frombuffer, unsigned int offset, void *tobuffer, int *cErr)
#endif
{
	qmessage *qto = (qmessage *)(void *)((unsigned char *)tobuffer - YALIGN(sizeof(qmessage), 8)),
			*qfrom = (qmessage *)(void *)((unsigned char *)frombuffer - YALIGN(sizeof(qmessage), 8));
	qpctl *c, *c_next, *p;
	unsigned int seg_off;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	/*Find seg_off of 'offset'*/
	c = __qvMsgLocate(qfrom, offset, &seg_off, cErr);
	/*c is node which has the offset*/
	if (!c)
		return 0;

	if (seg_off) {
		/* Split c present in qfrom, at the seg_off and return pointer
		* to the first splitted block. c now points to second
		* split block*/
		if (!__qvMsgSplitCtl(qfrom, c, seg_off, cErr))
			return 0;
	}
	/*Free segments present in ylist of qto*/
	if (ylCount((const YLIST *)&(qto->buffers)) != 0) {
		while ((p = (qpctl *)ylPopHead((YLIST *)&(qto->buffers))) != 0) {
#ifdef CSPL_LEAK_DEBUG_LITE
			__qvMsgFreeSeg(p->segment, func, line);
			__qvPoolFree(p, func, line);
#else
			__qvMsgFreeSeg(p->segment);
			__qvPoolFree(p);
#endif
		}
	} else {
		qvAssert(qto->size == 0);
	}

	/*Move all segments present in 'qfrom' from c and after c, to 'qto'*/
	while(c) {
		c_next = (qpctl *)ylNext((YLNODE *)c);

		ylDelete((YLIST *) &(qfrom->buffers), (YLNODE *)c);
		ylPushTail((YLIST *)&(qto->buffers), (YLNODE *)c);
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
 *			This function is used for joining list of 'zero copy buffers'
 *			in 'append' message to the list of 'zero copy buffers' in
 *			'buffer' message.
 *			'Append' remains unaltered
 * ARGS:
 *			buffer			Pointer to message in which the list of buffers
 *							shall be appended
 *			append			Pointer to message whose list of buffers are to
 *							be appended in 'buffer'
 *
 * RETURNS:
 *			Pointer to the 'buffer'
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
void *_qvMsgJoin(void *buffer, void *append, char *func, int line)
#else
void *qvMsgJoin(void *buffer, void *append)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgJoinEx(buffer, append, 0, func, line);
#else
	return qvMsgJoinEx(buffer, append, 0);
#endif
}
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgJoinEx(void *buffer, void *append, int *cErr, char *func, int line)
#else
void *qvMsgJoinEx(void *buffer, void *append, int *cErr)
#endif
{
	qpctl *p;
	unsigned int nbuf = 0, count;
	qmessage *qto = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage), 8)),
			*qfrom = (qmessage *)(void *)((unsigned char *)append - YALIGN(sizeof(qmessage), 8));

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	for (p = (qpctl *)ylFirst((const YLIST *)&(qfrom->buffers)); p; p = (qpctl *)ylNext((YLNODE *)p)) {
#ifdef CSPL_LEAK_DEBUG_LITE
		qpctl *c = NEWPCTL(func, line);
#else
		qpctl *c = NEWPCTL();
#endif
		if (!c) {
			for (count = nbuf; count; count--) {
				p = (qpctl *)ylPopTail((YLIST *)&(qto->buffers));
				/* +COVERITY 16260 */
				if (p) {
#ifdef CSPL_LEAK_DEBUG_LITE
					__qvMsgFreeSeg(p->segment, func, line);
					__qvPoolFree(p, func, line);
#else
					__qvMsgFreeSeg(p->segment);
					__qvPoolFree(p);
#endif
				}
				/* +COVERITY 16260 */
			}
			if (cErr)
				*cErr = QVERROR_PCTL_ALLOC_FAIL;
			return 0;
		}
		nbuf++;

		c->segment = p->segment;
		c->start = p->start;
		c->end = p->end;
		/** SPR 13295 Changes Start **/
		atomicIncrement(&c->segment->refcount);
		/** SPR 13295 Changes End **/
		ylPushTail((YLIST *)&(qto->buffers), (YLNODE *)c);
	}

	qto->size += qfrom->size;
	return	buffer;
}
/*****************************************************************************
 * FUNCTION: qvMsgPack
 *
 * DESCRIPTION:
 *			This function can be used for packing all the payload
 *			buffers attached to a message into a single payload buffer.
 * ARGS:
 *			buffer			Pointer to the buffer
 *			headroom		Size of empty space at the start of the
 *							payload buffer (optional)
 *			tailroom		Size of empty space at the end of the
 *							payload buffer (optional)
 *
 * RETURNS:
 *			Pointer to message with packed payload buffer
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
void *_qvMsgPack(void *buffer, unsigned int headroom, unsigned int tailroom, char *func, int line)
#else
void *qvMsgPack(void *buffer, unsigned int headroom, unsigned int tailroom)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgPackEx(buffer, headroom, tailroom, 0, func, line);
#else
	return qvMsgPackEx(buffer, headroom, tailroom, 0);
#endif
}
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvMsgPackEx(void *buffer, unsigned int headroom, unsigned int tailroom, int *cErr, char *func, int line)
#else
void *qvMsgPackEx(void *buffer, unsigned int headroom, unsigned int tailroom, int *cErr)
#endif
{
    qmessage *q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));
    unsigned int realsize, n = YALIGN(sizeof(qpseg), 8) + headroom + tailroom + q->size;
#ifdef CSPL_LEAK_DEBUG_LITE
    qpctl *c = NEWPCTL(func, line),
#else
    qpctl *c = NEWPCTL(),
#endif
	    *d;
    unsigned char *p;
    qpseg *s;
    /* If required, Reset the CSPL Error Code. */
    if (cErr)
		*cErr = QVERROR_NONE;

    if (!c) {
		if (cErr)
		    *cErr = QVERROR_PCTL_ALLOC_FAIL;
		return 0;
    }

#ifdef CSPL_LEAK_DEBUG_LITE
    p = (unsigned char *)qvMsgPoolAllocEx(n, &realsize, cErr, func, line);
#else
    p = (unsigned char *)qvMsgPoolAllocEx(n, &realsize, cErr);
#endif
    if (!p) {
#ifdef CSPL_LEAK_DEBUG_LITE
		__qvPoolFree(c, func, line);
#else
		__qvPoolFree(c);
#endif
		return 0;
    }

    s = (qpseg *)(void *)p;

    s->data = p  + YALIGN(sizeof(qpseg), 8);
    s->size = realsize - (s->data - p);

    c->segment = s;
    c->start = s->data + headroom;
    c->end = c->start + q->size;
    s->refcount = 1;

    s->free = 0; s->freearg = 0;

    /*Extract q->size bytes from buffer into c->start*/
    /* coverity 24357 28June2014 */
    if (0 == (qvMsgExtractEx(buffer, 0, c->start, q->size, cErr))) {
#ifdef CSPL_LEAK_DEBUG_LITE
		__qvPoolFree(c, func, line);
#else
		__qvPoolFree(c);
#endif
		return 0;
    }
    /* coverity 24357 28June2014 */

    /*Free the segregated segments present in the buffer*/
    while ((d = (qpctl *)ylPopHead((YLIST *)&(q->buffers))) != 0) {
#ifdef CSPL_LEAK_DEBUG_LITE
		__qvMsgFreeSeg(d->segment, func, line);
		__qvPoolFree(d, func, line);
#else
		__qvMsgFreeSeg(d->segment);
		__qvPoolFree(d);
#endif
    }
    /*Add the new packed node to the message*/
    ylPushTail((YLIST *)&(q->buffers), (YLNODE *)c);
    return buffer;
}
/*****************************************************************************
 * FUNCTION: qvMsgSegCount
 *
 * DESCRIPTION:
 *			This function returns the number of payload buffers
 *			attached to a message.
 * ARGS:
 *			buffer			Pointer to the message
 *
 * RETURNS:
 *			Number of payload buffers attached to the message
 *
 ******************************************************************************/
unsigned int qvMsgSegCount(const void *buffer)
{
	const qmessage *q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));
	return ylCount((const YLIST *)&(q->buffers));
}
/*****************************************************************************
 * FUNCTION: qvMsgSegNext
 *
 * DESCRIPTION:
 *			This function can be used for fetching a payload buffer
 *			attached to a message.
 * ARGS:
 *			buffer			Pointer to the message
 *			last			Pointer to control structure for payload
 *							buffer accessed last time
 *			segment			OutParam, returns the data and size of the payload
 *
 * RETURNS:
 *			Pointer to control structure for payload buffer
 *			whose data is copied in 'segment'.
 *			The same can be passed as 'last' in next call of this
 *			function for fetching the next payload buffer.
 *
 ******************************************************************************/
void *qvMsgSegNext(const void *buffer, void *last, QSEGMENT *segment)
{
	qpctl *p;

	/*Fetch the payload next to the 'last' which was fetched previously*/
	if (last) {
		p = (qpctl *)ylNext((YLNODE *)last);
	} else {
		/*Fetch the first node in the message*/
		const qmessage *q = (const qmessage *)(void *)((const unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));

		p = (qpctl *)ylFirst((const YLIST *)&(q->buffers));
	}

	if (p && segment) {
		segment->base = p->start;
		segment->size = SIZE(p);
	}

	return (void *)p;
}

/*****************************************************************************
 * FUNCTION: qvMsgPoolStats
 *
 * DESCRIPTION:
 *           This function displays message pool stats on screen.
 *
 * ARGS:
 *
 * RETURNS:
 *           None
 *
 *****************************************************************************/
void qvMsgPoolStats(void)
{
	unsigned int i = 0, n = 0, allocBuf = 0, freeBuf = 0, nbuf = 0;

	if (qd) {
		for (i = 0, n = MINBUF; i < NVARMSGPOOL; i++, n *= 2) {
			allocBuf = freeBuf = nbuf = 0;
			qpool *q = (qpool *)qd->alloc[i];

			if (q) {
				nbuf = __qvPoolSize(q, &allocBuf);
				freeBuf = nbuf - allocBuf;
			}
			fprintf(stderr, "Pool of %d\t:\tnBuf=%d, allocated Buf=%d, free Buf=%d\n",
					n, nbuf, allocBuf, freeBuf);
		}
	}
}

/*****************************************************************************
 * FUNCTION: qvResetMsgPoolPeakVal
 *
 * DESCRIPTION:
 *           This function resets message pool Peak value.
 *
 * ARGS:
 *
 * RETURNS:
 *           None
 *
 *****************************************************************************/
void qvResetMsgPoolPeakVal(void)
{
	unsigned int i = 0;

	if (qd)
	{
		for (i = 0 ; i < NVARMSGPOOL; i++)
		{
			qpool *q = (qpool *)qd->alloc[i];

			if (q)
			{
				q->peakVal = 0;
			}
		}
	}
}

/*spr 22474 start*/
/*CSPL_STATS flag removed*/
/*****************************************************************************
 * FUNCTION: getMsgPoolStatsInShm
 *
 * DESCRIPTION:
 *           This function returns message pool stats
 *
 * ARGS: poolIndex  ; pool id for which the function returns stats
 *       allocBuf  ; pointer to store no of allocated buffers in the pool
 *
 * RETURNS:nbuf ; no of buffers in the pool
 *
 *
 *****************************************************************************/
int getMsgPoolStatsInShm(unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf)
{
	unsigned int nbuf = 0;

	if (qd) {
		qpool *q = (qpool *)qd->alloc[poolIndex];

		if (q) {
			nbuf = __qvPoolSize(q, allocBuf);
			*peakAllocBuf = q->peakVal;
		}
	}
	return(nbuf);
}


/* SPR 10724 */
/*****************************************************************************
 * FUNCTION: getQMsgPoolStatsInShm
 * DESCRIPTION: This function returns Q message pool stats
 * ARGS: allocBuf  ; pointer to store no of allocated buffers in the pool
 * RETURNS:nbuf ; no of buffers in the pool
 *****************************************************************************/
int getQMsgPoolStatsInShm(unsigned int *allocBuf, unsigned int *peakAllocBuf)
{
	unsigned int nbuf = 0;

	if (qd) {
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
int getQpctlPoolStatsInShm(unsigned int *allocBuf, unsigned int *peakAllocBuf)
{
	unsigned int nbuf = 0;

	if (qd) {
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
unsigned int getQMessageSize(void)
{
	return sizeof(qmessage);
}

/* SPR 10724 */
/*****************************************************************************
 * FUNCTION: getQpctlSize
 * DESCRIPTION: This function returns QPCTL pool size
 * ARGS: None
 * RETURNS:size of QPCTL
 *****************************************************************************/
 unsigned int getQpctlSize(void)
{
	return sizeof(qpctl);
}
/*spr 22474 end*/


/*****************************************************************************
 * FUNCTION: qvMsgJoinAndFree
 *
 * DESCRIPTION:
 *			This function is used for joining list of 'zero copy buffers'
 *			in 'append' message to the list of 'zero copy buffers' in
 *			'buffer' message.
 *			'Append' remains unaltered
 * ARGS:
 *			buffer			Pointer to message in which the list of buffers
 *							shall be appended
 *			append			Pointer to message whose list of buffers are to
 *							be appended in 'buffer'
 *
 * RETURNS:
 *			Pointer to the 'buffer'
 *
 ******************************************************************************/
void *qvMsgJoinAndFree(void *buffer, void *append)
{
	return qvMsgJoinAndFreeEx(buffer, append, 0);
}
void *qvMsgJoinAndFreeEx(void *buffer, void *append, int *cErr)
{
	qmessage *qto = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage), 8)),
		*qfrom = (qmessage *)(void *)((unsigned char *)append - YALIGN(sizeof(qmessage), 8));

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	ylConcat((YLIST *)&(qto->buffers), (YLIST *)&(qfrom->buffers));

	qto->size += qfrom->size;

	/*Free q, and thus its non-zero cpy buffer*/
#ifdef CSPL_LEAK_DEBUG_LITE
	__qvPoolFree(qfrom, __func__, __LINE__);
#else
	__qvPoolFree(qfrom);
#endif
	return buffer;
}

/*****************************************************************************
 * FUNCTION: qvMsgPoolForSize
 *
 * DESCRIPTION:
 *			This function returns the pointer to msg pool for buffers of length
 *			'size'
 * ARGS:
 *			size			Size of buffers in the pool
 *
 * RETURNS:
 *			Pointer to the 'pool'
 *
 ******************************************************************************/
QMPOOL qvMsgPoolForSize(unsigned int size)
{
	unsigned int i;
	unsigned int n;
	qpool *q = NULL;

	for (i = 0, n = MINBUF; i < NVARPOOL; i++, n *= 2) {
		if (size <= n) {
			q = (qpool *)qd->alloc[i];
			break;
		}
	}
	return (QMPOOL)q;
}

/*****************************************************************************
 * FUNCTION: qvMsgPoolGetUsagePercentage
 *
 * DESCRIPTION:
 *          This function returns the pointer to msg pool for buffers of length
 *          'size'
 * ARGS:
 *          size            Size of buffers in the pool
 *
 * RETURNS:
 *          Pointer to the 'pool'
 *
 ******************************************************************************/
unsigned int qvMsgPoolGetUsagePercentage(void)
{
	unsigned int i;
	unsigned int totalBuf = 0;
	unsigned int allocatedBuf = 0;
	unsigned int usagePercentage = 0;
	/*SPR 5394 Fix*/
	/* SPR 11109 fix start */
	unsigned int pctlUsagePercentage = 0;

	if (((qpool *)qd->pctlpool)->nbuf)
		pctlUsagePercentage = (((qpool *)qd->pctlpool)->alloc * 100)/((qpool *)qd->pctlpool)->nbuf;

	/* SPR 11109 fix end */
	for (i = 8 ; i < NVARPOOL; i++) {
		allocatedBuf += ((qpool *)qd->alloc[i])->alloc;
		totalBuf += ((qpool *)qd->alloc[i])->nbuf;
	}
	/* SPR 11109 fix start */
	if (totalBuf)
		usagePercentage = (allocatedBuf * 100)/totalBuf;
	/* SPR 11109 fix end */
	if (usagePercentage > pctlUsagePercentage)
		return usagePercentage;
	else
		return pctlUsagePercentage;
}

#ifdef CSPL_LEAK_DEBUG_LITE
void qvPrintLeakedMsgBuf(void)
{
	qpool *pool = qd->qmsgpool;
	qchunk *chunk;
	qmessage *message_p;
	qpctl *pctl;
	unsigned int realsize, i, count;
	unsigned char *p;

	realsize = YALIGN(sizeof(qbuf), 8) + YALIGN(pool->size, 8);
	chunk = (qchunk *)ylFirst(&pool->chunks);

	fprintf(stderr, "Waiting for PDCP thread to clean the data!!!!\n");
	sleep(10);
	fprintf(stderr, "MSG POOL LEAK SUMMARY:\n");

	p = (unsigned char *) chunk;
	for (i = 0, p += YALIGN(sizeof(qchunk), 8); i < chunk->nbuf; i++, p += realsize) {
		message_p = (qmessage *)(p + YALIGN(sizeof(qbuf), 8));

		if (message_p->func) {
			fprintf(stderr, "\tQMSG: func[%s] line[%u]\n", message_p->func, message_p->line);
			count = message_p->buffers.count;
			while (count) {
				pctl = (qpctl *)listPopNode(message_p->buffers.node);
				if (pctl) {
					fprintf(stderr, "\t\tQPCTL: func[%s] line[%u]\n", pctl->func, pctl->line);
					if (pctl->segment && pctl->segment->refcount) {
						fprintf(stderr, "\t\tQSEG: func[%s] line[%d] refCount[%d] size[%d]\n",
						pctl->segment->func, pctl->segment->line, pctl->segment->refcount, pctl->segment->size);
					}
				}
				count--;
			}
		}
	}
}
#endif
#ifdef CSPL_LEAK_DEBUG
/*****************************************************************************
 * FUNCTION: qvPrintPoolAllocBuf
 *
 * DESCRIPTION: This function prints the backtrace of the buffers which are not
 *              freed or doublefreed
 *
 * ARGS: *q            pointer to the buffer
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvPrintBufferBt(qbuf *q)
{
	char **strings;
	int j;

	/* Get symbol names - necessary to compile with rdynamic
	* to get meaningful names */
	strings = backtrace_symbols(q->buffer, q->nptrs);

	if (strings == NULL)
		perror("backtrace_symbols");

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
void qvPrintPoolAllocBuf(qpool   *pool)
{
	qchunk *chunk;
	unsigned int realsize, i;
	unsigned char *p;

	realsize = YALIGN(sizeof(qbuf), 8) + YALIGN(pool->size, 8);
	chunk = (qchunk *)ylFirst(&pool->chunks);
	while (chunk) {
		p = (unsigned char *) chunk;
		for (i = 0, p += YALIGN(sizeof(qchunk), 8); i < chunk->nbuf; i++, p += realsize) {
			qbuf *q = (qbuf *)(void *)p;

			/* print the backtrace for the allocation */
			if (q->allocated != 0)
				qvPrintBufferBt(q);
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
void qvPrintAllocBuf(void)
{
    int i;

	for (i = 0; i < NVARMSGPOOL; i++) {
		fprintf(stderr, "##### Pool[%d] Size [%d] Total Buffer [%d] Allocated Buffer[%d] #####\n",
		i, qd->alloc[i]->size, qd->alloc[i]->nbuf, qd->alloc[i]->alloc);
		qvPrintPoolAllocBuf(qd->alloc[i]);
	}

	fprintf(stderr, "#### Allocated pctlpool ####\n");
	qvPrintPoolAllocBuf(qd->pctlpool);
	fprintf(stderr, "#### Allocated qmsgpool ####\n");
	qvPrintPoolAllocBuf(qd->qmsgpool);
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
void qvPrintCurrentBt(void)
{
	int j, nptrs;
	void *buffer[MAX_FRAMES_IN_BT];
	char **strings;

	nptrs = backtrace(buffer, MAX_FRAMES_IN_BT);
	/* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
	 * would produce similar output to the following: */
	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL)
		perror("backtrace_symbols");

	for (j = 0; j < nptrs; j++)
		fprintf(stderr, "%s\n", strings[j]);

	free(strings);
}
#endif
/*****************************************************************************
 * FUNCTION: qvMsgReserveHeadTailRoom
 *
 * DESCRIPTION:
 *          This function can be used for reserving a specified
 *          area of memory in a message buffer when memory is already allocated
 *          in headroom or tailroom.
 * ARGS:
 *          buffer      Pointer to the message
 *          where       Nth byte where data is to be reserved
 *          count       Size of buffer to be reserved
 * RETURNS:
 *          Pointer to the start of reserved area
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *qvMsgReserveHeadTailRoom(void *buffer, int where, unsigned int count, char *func, int line)
#else
unsigned char *qvMsgReserveHeadTailRoom(void *buffer, int where, unsigned int count)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return qvMsgReserveHeadTailRoomEx(buffer, where, count, 0, func, line);
#else
	return qvMsgReserveHeadTailRoomEx(buffer, where, count, 0);
#endif
}

#ifdef CSPL_LEAK_DEBUG_LITE
unsigned char *qvMsgReserveHeadTailRoomEx(void *buffer, int where, unsigned int count, int *cErr, char *func, int line)
#else
unsigned char *qvMsgReserveHeadTailRoomEx(void *buffer, int where, unsigned int count, int *cErr)
#endif
{
	qmessage *q = (qmessage *)(void *)((unsigned char *)buffer - YALIGN(sizeof(qmessage), 8));
	qpctl *c, *c_prev;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	if (where && ((unsigned int)where == q->size))
		where = -1;

	/*Reserve at the start*/
	if (where == 0) {
		c = (qpctl *)ylFirst((const YLIST *)&(q->buffers));

		if (!c) {
			c = __qvMsgAddNewSeg(q, 0, count, 0, 0, cErr);
			if (!c)
				return 0;

			return c->start;
		} else if ((HEADROOM(c) >= count)) {
			/*Accomodate headroom to reserve space*/
			c->start -= count;
			q->size += count;
			return c->start;
		} else {
			/*allocate and place memory block before c*/
			c = __qvMsgAddNewSeg(q, c, count, 0, 0, cErr);
			if (!c)
				return 0;
			return c->start;
		}
	} else if (where == -1) {
		/*Reserve at the end*/
		c = (qpctl *)ylLast((const YLIST *)&(q->buffers));
		if (!c) {
			c = __qvMsgAddNewSeg(q, 0, count, 0, 0, cErr);
			if (!c)
				return 0;

			return c->start;
		} else if ((TAILROOM(c) >= count)) {
			unsigned char  *r = c->end;

			c->end += count;
			q->size += count;
			return r;
		} else {
			c = __qvMsgAddNewSeg(q, 0, count, 0, 0, cErr);
			if (!c)
				return 0;

			return c->start;
		}
	} else {
		unsigned int    offset;

		c = __qvMsgLocate(q, where, &offset, cErr);
		if (!c)
			return 0;

		c_prev = (qpctl *)ylPrev((YLNODE *)c);

		if (offset) {
			if (!__qvMsgSplitCtl(q, c, offset, cErr))
				return 0;

			c = __qvMsgAddNewSeg(q, c, count, 0, 0, cErr);
			if (!c)
				return 0;

			return c->start;
		} else if (ONLY(c) && (HEADROOM(c) >= count)) {
			c->start -= count;
			q->size += count;
			return c->start;
		} else if (c_prev && ONLY(c_prev) && (TAILROOM(c_prev) >= count)) {
			unsigned char *r = c_prev->end;

			c_prev->end += count;
			q->size += count;
			return r;
		} else {
			c = __qvMsgAddNewSeg(q, c, count, 0, 0, cErr);
			if (!c)
				return 0;
			return c->start;
		}
	}
}
/* + SPR 18103 */

/* - SPR 18103 */
