/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Implementation                          **/
/**                                                                        **/
/****************************************************************************/

#include "mempool.h"
#include "atomic-ops.h"
#ifdef CSPL_LEAK_DEBUG
#include <execinfo.h>
#endif

/** Constants ***************************************************************/
#define	__POOLMAGIC	0xCA98362D

#define	MINBUF		8
#define	NVARPOOL	27


/** Data Types (Implementation Private) *************************************/
typedef	struct qpool {
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
	unsigned short  incarnation;
	unsigned char	allocated;
#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
	unsigned char *func;
	unsigned int   line;
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

typedef	struct qchunk {
    YLNODE	__header__;
    unsigned int	nbuf;
} qchunk;

typedef struct qvcontext {
    unsigned char loglevel;
    QSYSOP	op;
    QLOCK	lock;
    qpool	*alloc[NVARMEMPOOL];
} qcontext;

/** Global Data *************************************************************/
static	qcontext	*qd;
/** Forward Declarations ****************************************************/
static qpool *__qvPoolCreate(unsigned int size, unsigned int nbuf);
static qbuf *__qvPoolLocate(qpool *, unsigned int);
static int __qvPoolExtend(qpool *pool, unsigned int nbuf,
					unsigned int returnExtendedMem, void **retPtr_pp);
static unsigned int __qvPoolSize(qpool *pool, unsigned int *p_alloc);
#ifdef CSPL_LEAK_DEBUG_LITE
void *qvPoolAlloc(void *pool, char *func, int line);
static void	*__qvPoolAlloc(qpool *pool, char *func, int line);
#else
void *qvPoolAlloc(void *pool);
static void	*__qvPoolAlloc(qpool *pool);
#endif
#ifdef CSPL_LEAK_DEBUG_LITE
static void	__qvPoolFree(void *buffer, char *func, int line);
#else
static void	__qvPoolFree(void *buffer);
#endif

/** Default System Operations ***********************************************/

/* +- SPR 17777 */
static void *s_malloc(unsigned int size) {(void)size; return 0; }
/* +- SPR 17777 */
/*--coverity--530-54312*/
static void s_free(void *m)
{
    if (m)
		free(m);
}

/*--coverity--530-54312*/
/* +- SPR 17777 */
static void *s_newlock(void) { return 0; }
static void s_lock(void *lock) { (void)lock; }
static void s_unlock(void *lock) { (void)lock; }
static void s_destroylock(void *lock) { (void)lock; }
static int s_vprintf(const char *format, va_list A) { (void)format; (void)A; return 0; }
static	int	s_iprintf(const char *format, va_list A) { (void)format; (void)A; return 0; }
static	void	s_abort(void) { *((unsigned char *)0); }
/* +- SPR 17777 */

#ifdef CSPL_LEAK_DEBUG
void qvPrintMemBufferBt(qbuf *q);
void qvPrintMemPoolAllocBuf(qpool *pool);
#endif
/*****************************************************************************
 * FUNCTION: qvInit
 *
 * DESCRIPTION:
 *			Performs the initilization of the library and threads.
 *			It registers with the OS services, tunes the library and
 *			allocates memory for various pools.
 *
 *
 * ARGS:
 *			op			Pointer to QSYSOP structure specifying OS services
 *						to be accessed.
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvInit(const QSYSOP *op)
{
    int	cErr = QVERROR_NONE;

    qvInitEx(op, &cErr);
    if (cErr == QVERROR_SYS_ALLOC_FAIL)
		qvPrintf("Unable to allocate memory for Context");
}

void qvInitEx(const QSYSOP *op, int *cErr)
{
    unsigned int i;
    unsigned int size = MINBUF;

    /* If required, Reset the Error Code. */
    if (cErr)
		*cErr = QVERROR_NONE;

    /* +COVERITY 10229 */
    qd = op->malloc ? (qcontext *)op->malloc(sizeof(qcontext)) :
		(qcontext *)s_malloc(sizeof(qcontext));

    /* -COVERITY 10229 */
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

    for (i = 0; i < NVARMEMPOOL; i++, size *= 2) {
		qd->alloc[i] = __qvPoolCreate(size, 0);
		if (!qd->alloc[i]) {
			if (cErr)
				*cErr = QVERROR_SYS_ALLOC_FAIL;
			return;
		}
    }

    qd->loglevel = 255;
    qd->lock = qvNewLock();
}


/** Access to System Memory Allocation **************************************/
/*****************************************************************************
 * FUNCTION: qvSysMalloc
 *
 * DESCRIPTION:
 *			Calls Directly the OS specific memory allocation
 *			routines is for memory allocation.
 *
 * ARGS:
 *			size			size of buffer to be allocated
 *
 * RETURNS:
 *			Pointer to the memory allocated
 *
 ******************************************************************************/
void *qvSysMalloc(unsigned int size)
{
    return qd->op.malloc(size);
}
/*****************************************************************************
 * FUNCTION: qvSysFree
 *
 * DESCRIPTION:
 *			The function calls OS memory free routine directly
 *			for freeing the memory allocated by qvSysMalloc.
 *
 * ARGS:
 *			pointer to the memory to be freed.
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvSysFree(void *p)
{
    qd->op.free(p);
}

/** Memory Pool Management **************************************************/
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
void *qvPoolCreateAndReturn(unsigned int size, unsigned int nbuf)
{
    return __qvPoolCreate(size, nbuf);
}

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
    pool->lock = qvNewLock();

    /*coverity-530 CID-36231*/
    if (pool->lock == PNULL) {
		qd->op.free((void *)pool);
		return 0;
	}
    /*coverity-530 CID-36231*/

	/* SPR 11109 16-JUN-2014 Fix start */
#ifndef MEM_LEAK_DEBUG
    if (nbuf) {
		if (!__qvPoolExtend(pool, nbuf, 0, 0)) {
				qd->op.free((void *)pool);
				return 0;
		}
    }
#endif
	/* SPR 11109 16-JUN-2014 Fix end */

    return pool;
}

/*****************************************************************************
 * FUNCTION: qvPoolCreate
 *
 * DESCRIPTION:
 *           This function creates a context pool of specified size of buffer
 *           and the number of buffers in the pool.
 *
 * ARGS:
 *           size                size of each buffer in the pool
 *           nbuf                number of buffers in the pool
 *
 * RETURNS:
 *           None
 *
 ******************************************************************************/
void qvPoolCreate(unsigned int size, unsigned int nbuf)
{
    unsigned int i, n;

    for (i = 0, n = MINBUF; i < NVARMEMPOOL; i++, n *= 2) {
		if (size <= n) {
			qpool *q = (qpool *)qd->alloc[i];
			__qvPoolExtend(q, nbuf, 0, 0);
			break;
		}
    }
}

/*****************************************************************************
 * FUNCTION: qvCleanupMemPool
 *
 * DESCRIPTION:
 *           This function frees all the memory allocated for context pools.
 *
 * ARGS:
 *
 * RETURNS:
 *           None
 *
 *****************************************************************************/
void qvCleanupMemPool(void)
{
    unsigned int i, n;
    YLNODE *ylNode_p = NULL;

	if (!qd)
		return;

    for (i = 0, n = MINBUF; i < NVARMEMPOOL; i++, n *= 2) {
		qpool *q = (qpool *)qd->alloc[i];

		if (q) {
		    /* Freeing the chunks of memory allocated during
		       poolCreate or poolExtend */
				while ((ylNode_p = ylPopHead(&(q->chunks)))) {
					qd->op.free((void *)(ylNode_p));
					ylNode_p = NULL;
				}

				qvDestroyLock(q->lock);
				qd->op.free((void *)(q));
				qd->alloc[i] = NULL;
		}
    }
    /* Free the memory allocated for qd context*/
    qvDestroyLock(qd->lock);
    qd->op.free((void *)(qd));
    qd = NULL;
}
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
void qvPoolStats(void)
{
	unsigned int i = 0, n = 0, allocBuf = 0, freeBuf = 0, nbuf = 0;

	if (!qd)
		return;

	for (i = 0, n = MINBUF; i < NVARMEMPOOL; i++, n *= 2) {
		qpool *q = (qpool *)qd->alloc[i];

		if (q) {
			nbuf = __qvPoolSize(q, &allocBuf);
			allocBuf = q->alloc;
			freeBuf = nbuf - allocBuf;
		}
		printf("Pool of %d\t:\tnBuf=%d, allocated Buf=%d, free Buf=%d\n",
				n, nbuf, allocBuf, freeBuf);
	}
}


/*****************************************************************************
 * FUNCTION: qvResetMemPoolPeakVal
 *
 * DESCRIPTION:
 *           This function resets pool peak value.
 *
 * ARGS:
 *
 * RETURNS:
 *           None
 *
 *****************************************************************************/
void qvResetMemPoolPeakVal(void)
{
	unsigned int i = 0;

	if (!qd)
		return;

	for (i = 0; i < NVARMEMPOOL; i++)
	{
		qpool *q = (qpool *)qd->alloc[i];

		if (q)
			q->peakVal = 0;
	}
}

/*spr 22474 start*/
/*CSPL_STATS flag removed*/
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
int getMemPoolStatsInShm(unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf)
{
	unsigned int nbuf = 0;
	qpool *q;

	if (!qd)
		return 0;

	q = (qpool *)qd->alloc[poolIndex];
	if (q) {
		nbuf = __qvPoolSize(q, allocBuf);
		*peakAllocBuf = q->peakVal;
	}

	return nbuf;
}

/*spr 22474 end*/


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

int qvPoolExtend(void *qpool, unsigned int nbuf)
{
    return (__qvPoolExtend(qpool, nbuf, 0, 0));
}

static int __qvPoolExtend(qpool *pool, unsigned int nbuf, unsigned int returnExtendedMem, void **retPtr_pp)
{
    qchunk *chunk;
    unsigned int size = pool->size;
    unsigned int realsize, i;
    unsigned char *p;

#ifdef MEM_FENCING
    unsigned int chunkSize = 0;
    unsigned int bufsize = 0;
#endif

#ifdef MEM_DEBUG
    if (nbuf)
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
			if (p == NULL) {
			perror("Failed to allocate memory");
			return 0;
		}

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
#ifndef LOCKLESS_IMPL
	    ypPut(&(pool->list), (YPNODE *)q);
#else
	    ypPutAtomic(&(pool->list), (YPNODE *)q);
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
			q->incarnation = 0;
			/* SPR 21829 Start */
#ifndef LOCKLESS_IMPL
			ypPut(&(pool->list), (YPNODE *)q);
#else
			ypPutAtomic(&(pool->list), (YPNODE *)q);
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

	qvLock(pool->lock);

	ylPushTail(&(pool->chunks), (YLNODE *)chunk);
	for (i = 0, p += YALIGN(sizeof(qchunk), 8); i < nbuf; i++, p += realsize) {
		qbuf *q = (qbuf *)(void *)p;

		q->allocated = 0;
		q->incarnation = 0;
		/* SPR 21829 Start */
#ifdef LOCKLESS_IMPL
		ypPutAtomic(&(pool->list), (YPNODE *)q);
#else
		ypPut(&(pool->list), (YPNODE *)q);
#endif
	}

	pool->nbuf += nbuf;
	/*code to return immediately extended memory*/
	if (returnExtendedMem) {
		void *p;
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

	qvLock(pool->lock);
	nbuf = pool->nbuf;
	nalloc = pool->alloc;
	qvUnlock(pool->lock);

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
void *qvPoolAlloc(void *pool, char *func, int line)
#else
void *qvPoolAlloc(void *pool)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	return (__qvPoolAlloc(pool, func, line));
#else
	return (__qvPoolAlloc(pool));
#endif
}


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
	p = malloc(pool->size + YALIGN(sizeof(qbuf), 8));
	pool->alloc++;
#else
#ifdef LOCKLESS_IMPL
	if ((p = ypGetAtomic(&pool->list)) != 0)
		atomicIncrement(&pool->alloc);

#else /* LOCKLESS_IMPL */
	qvLock(pool->lock);
	if ((p = ypGet(&pool->list)) != 0)
		pool->alloc++;

	qvUnlock(pool->lock);
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
			int protSize = ((pool->size / PAGESIZE) + 1)*PAGESIZE;

			/*Fix Jira-374 end*/
			if (-1 == mprotect(q->protectaddr, protSize, PROT_READ|PROT_WRITE)) {
				printf("protSze:%d q->protectaddr:%x errno:%d\n", protSize, q->protectaddr, errno);
				perror("unprotect failed\n");
				abort_wrapper();
			}
		}
#endif
		q->allocated = 1;
		q->incarnation++;
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
		if (!__qvPoolExtend(pool, 1, 0, 0))
			return 0;
#ifdef CSPL_LEAK_DEBUG
		k = __qvPoolAlloc(pool, __func__, __LINE__);
#else
		k = __qvPoolAlloc(pool);
#endif
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

#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
	q->func = func;
	q->line = line;
#endif

	/* Safe check is introduced to avoid double free */
	//qvAssert( q->allocated );
	if (q->allocated == 0) {
#ifdef CSPL_LEAK_DEBUG_LITE
		fprintf(stderr, "########## Double Free detected [%p] %s, %d ##########\n", q, func, line);
#endif
#ifdef CSPL_LEAK_DEBUG
		fprintf(stderr, "########## Double Free detected ##########\n");
		fprintf(stderr, "First Free done by :-\n");
		/* print backtrace for double free */
		qvPrintMemBufferBt(q);
		fprintf(stderr, "Second Free done by :-\n");
		/* print current backtrace*/
		qvPrintMemCurrentBt();
#endif
		return;
	}

	q->allocated = 0;

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
	qvLock(pool->lock);
#ifdef MEM_DEBUG
	c -= YALIGN(sizeof(qchunk), 8);
	ylDelete(&(pool->chunks), (YLNODE *)c);
	qd->op.free(c);
#else
	ypPut(&(pool->list), (YPNODE *)q);
#endif
#ifdef MEM_FENCING
	if (pool->size == POOL_SIZE_TO_FENCE) {
		/*L2_CRASH_FIX*/
		int protSize = ((pool->size / PAGESIZE) + 1)*PAGESIZE;

		/*L2_CRASH_FIX*/
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
	qvUnlock(pool->lock);
#endif /* LOCKLESS_IMPL */
#endif
}

void qvAllFreeFromMemPool(void *qv)
{
	YLNODE *ylNode_p = NULL;
	qpool *q = qv;

	while ((ylNode_p = ylPopHead(&(q->chunks)))) {
		qd->op.free((void *)(ylNode_p));
		ylNode_p = NULL;
	}

	qvDestroyLock(q->lock);
	qd->op.free((void *)(q));
}
/** Malloc Emulation ********************************************************/
/*****************************************************************************
 * FUNCTION: qvAlloc
 *
 * DESCRIPTION:
 *			This function allocates memory of size specified by the
 *			first parameter to the function. The second parameter is
 *			optional, if passed it returns the actual amount of memory
 *			allocated.
 *			Memory allocation request is serviced from the pool
 *			of smallest block size that will fit the requested size.
 *
 * ARGS:
 *			size			size of the buffer needed
 *			psize			actual size of buffer returned
 * RETURNS:
 *			Pointer to the buffer allocated from any of the various pools.
 *
 ******************************************************************************/
void *qvAlloc(unsigned int size, unsigned int *psize)
{
#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
    return qvAllocEx(size, psize, 0, __func__, __LINE__);
#else
    return qvAllocEx(size, psize, 0);
#endif
}
#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
void *qvAllocEx(unsigned int size, unsigned int *psize, int *cErr, unsigned char *func, unsigned int line)
#else
void *qvAllocEx(unsigned int size, unsigned int *psize, int *cErr)
#endif
{
	unsigned int i, n;
#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
	qbuf *_buf = NULL;
#endif

	/* If required, Reset the Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	for (i = 0, n = MINBUF; i < NVARPOOL; i++, n *= 2) {
		if (size <= n) {
			qpool *q = (qpool *)qd->alloc[i];
#ifdef CSPL_LEAK_DEBUG_LITE
			void *p = __qvPoolAlloc(q, __func__, __LINE__);
#else
			void *p = __qvPoolAlloc(q);
#endif

#ifdef UE_SIM_TESTING
			if (p == 0) {
				unsigned int nbuf = __qvPoolSize(q, 0);

				__qvPoolExtend(q, nbuf ? nbuf : 1, 1, &p);
				//p = __qvPoolAlloc( q );
			}
			/* Coverity 10724 */
			if (!p) {
				if (cErr)
					*cErr = QVERROR_SYS_ALLOC_FAIL;
			}
			/* Coverity 10724 */

#else
			if (p == 0)
				continue;
#endif

#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
			_buf = p - YALIGN(sizeof(qbuf), 8);
			_buf->func = func;
			_buf->line = line;
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
 * FUNCTION: qvFree
 *
 * DESCRIPTION:
 *			This function frees the buffer, which was previously
 *			allocated from pools.
 *			The buffer is returned back to
 *			the list of free buffers of pool to which it belonged.
 * ARGS:
 *			buffer			Pointer to the buffer to be freed.
 *
 * RETURNS:
 *
 ******************************************************************************/
#ifdef CSPL_LEAK_DEBUG_LITE
void qvFree(void *p, char *func, int line)
#else
void qvFree(void *p)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
    __qvPoolFree(p, func, line);
#else
    __qvPoolFree(p);
#endif
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
 *			Lock
 *
 ******************************************************************************/
QLOCK qvNewLock(void)
{
    return (QLOCK)qd->op.newlock();
}
/*****************************************************************************
 * FUNCTION: qvLock
 *
 * DESCRIPTION:
 *			This function is called for activating
 *			the lock. 'lock' function defined in QSYSOP structure
 *			is called in turn.
 *
 * ARGS:
 *			Lock id returned from qvNewLock
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvLock(QLOCK lock)
{
    /* SES-488 Coverity 110286 */
    if (lock != NULL)
		qd->op.lock((void *)lock);
    /* SES-488 Coverity 110286 */
}
/*****************************************************************************
 * FUNCTION: qvUnlock
 *
 * DESCRIPTION:
 *			This function is called for deactivating
 *			the lock. 'unlock' function defined in QSYSOP structure
 *			is called in turn.
 *
 * ARGS:
 *			Lock id returned from qvNewLock
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvUnlock(QLOCK lock)
{
    /* SES-488 Coverity 110286 */
    if (lock != NULL)
		qd->op.unlock((void *)lock);
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
 *			Lock id returned from qvNewLock
 *
 * RETURNS:
 *
 ******************************************************************************/
void qvDestroyLock(QLOCK lock)
{
    /* SES-488 Coverity 110286 */
    if (lock != NULL)
		qd->op.destroylock((void *)lock);
    /* SES-488 Coverity 110286 */
}


static int __lprint(const char *format, ...)
{
    /* SPR 20430 Changes Start */
#ifdef __x86_64__
	/* Coverity 54677 */
	va_list A = {{0} };
	/* Coverity 54677 */
#else
    /* SPR 20430 Changes End */
    /* Coverity 54677 */
    va_list A = {0};
    /* Coverity 54677 */
    /* SPR 20430 Changes Start */
#endif
    /* SPR 20430 Changes End */
    int i;

    va_start(A, format);
    i = qd->op.vprintf(format, A);
    va_end(A);
    return i;
}


static int	__lscan(const char *format)
{
    int	count = 0, length = 0;
    char c;
    const char *s = format;

    while ((c = *s++) != 0) {
		length++;
		if (c == '\n')
		    count++;
    }

    if (!count) {
		return 0;
    } else if (length && (format[length-1] == '\n')) {
		return -count;
    } else {
		return count;
    }
}
/*****************************************************************************
 * FUNCTION: qvPrintfVa
 *
 * DESCRIPTION:
 *			This function prints the argument list 'A', as per the
 *			format parameter passed to it
 * ARGS:
 *			format			String describing the format to use to
 *							write the remaining operands
 *			A				argument list to be printed as per 'format'
 * RETURNS:
 *			Return value of iprintf function
 ******************************************************************************/
int	qvPrintfVa(const char *format, va_list A)
{
    if (qd)
		return qd->op.iprintf(format, A);

    return 0;
}
/*****************************************************************************
 * FUNCTION: qvPrintf
 *
 * DESCRIPTION:
 *			This function prints the variable number of arguments as per
 *			the format parameter passed to it.
 * ARGS:
 *			format			String describing the format to use to
 *							write the remaining operands
 * RETURNS:
 *			Return value of iprintf function
 ******************************************************************************/
int	qvPrintf(const char *format, ...)
{
    /* SPR 20430 Changes Start */
#ifdef __x86_64__
	/* Coverity 54678 */
	va_list A = {{0} };
	/* Coverity 54678 */
#else
    /* SPR 20430 Changes End */
    /* Coverity 54678 */
    va_list A = {0};
    /* Coverity 54678 */
    /* SPR 20430 Changes Start */
#endif
    /* SPR 20430 Changes End */
    int i;

    va_start(A, format);
    i = qvPrintfVa(format, A);
    va_end(A);
    return i;
}

static void __qvLog(const char *label, const char *format, va_list A)
{
    int	n = __lscan(format);

    if (qd) {
		qvLock(qd->lock);

		__lprint(" ** %s ** ", label);

		if (n && (n != -1))
		    __lprint("\n");

		qd->op.vprintf(format, A);

		if (n >= 0)
		    __lprint("\n");

		qvUnlock(qd->lock);
    }
}
/*****************************************************************************
 * FUNCTION: qvLogVa
 *
 * DESCRIPTION:
 *			This function is used for logging traces for defined levels
 *			as per the format parameter passed to it. The argument list
 *			to be printed is specified in variable 'A'.
 * ARGS:
 *			level			Trace Level
 *			format			String describing the format to use to
 *							write the remaining operands
 *			A				argument list to be printed as per 'format'
 * RETURNS:
 *
 ******************************************************************************/
int	qvLogVa(int level, const char *format, va_list A)
{
    int i = 0, n;

    if (qd) {
		if (qd->loglevel < level)
		return 0;

		n = __lscan(format);

		qvLock(qd->lock);

		if (n && (n != -1))
		    __lprint("\n");

		i = qd->op.vprintf(format, A);

		if (n >= 0)
		    __lprint("\n");

		qvUnlock(qd->lock);
    }

    return i;
}

/*****************************************************************************
 * FUNCTION: qvLog
 *
 * DESCRIPTION:
 *			This function is used for logging traces for defined
 *			levels as per the format parameter passed to it.
 * ARGS:
 *			level			Trace Level
 *			format			String describing the format to use to
 *							write the remaining operands
 * RETURNS:
 *
 ******************************************************************************/
int	qvLog(int level, const char *format,  va_list A)
{
    int i;

    i = qvLogVa(level, format, A);

    return i;
}

/*****************************************************************************
 * FUNCTION: qvLogLevel
 *
 * DESCRIPTION:
 *			This function is used for setting the log level as required.
 *			Trace statements with level greater than the set log level
 *			will not be printed.
 * ARGS:
 *			level			Trace Level to be set
 * RETURNS:
 *
 ******************************************************************************/
void qvLogLevel(unsigned char level)
{
    if (qd)
		qd->loglevel = level;
}

/** Error Handling **********************************************************/
/*****************************************************************************
 * FUNCTION: qvPanic
 *
 * DESCRIPTION:
 *			This function is called when some error condition
 *			occurs and the program must be aborted due to it.
 *			The function logs the error condition by printing all
 *			the parameters passed to it (in the printf format).
 * ARGS:
 *			format		String describing the format to use to
 *						write the remaining operands
 * RETURNS:
 *
 ******************************************************************************/
void qvPanic(const char *format,  va_list A)
{
    if (qd) {
		qvPrintf("** PANIC - ");
		qd->op.iprintf(format, A);
		qvPrintf("\n");

		__qvLog("PANIC", format, A);
    }
}
/*****************************************************************************
 * FUNCTION: qvWarning
 *
 * DESCRIPTION:
 *			This function is called when some warning condition
 *			occurs and the user needs to be informed about it. The
 *			function logs the warning condition by printing all the
 *			parameters passed to it (in the printf format).
 * ARGS:
 *			format		String describing the format to use to
 *						write the remaining operands
 * RETURNS:
 *
 ******************************************************************************/
void qvWarning(const char *format, va_list A)
{
    if (qd) {
		if (qd->loglevel < QVLOG_WARNING)
		return;

		qvPrintf("\n");
		__qvLog("WARNING", format, A);
		qvPrintf("\n");
    }
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
void qvAbort(void)
{
    if (qd)
		qd->op.abort();
}

/*****************************************************************************
 * * FUNCTION: qvPoolSequence
 * *
 * * DESCRIPTION:
 * *                 This function returns the unique number depending upon
 * *                 the number of times 'buffer' has been allocated.
 * *
 * * ARGS:
 * *                 buffer, Pointer to the buffer.
 * *
 * * RETURNS:
 * *
 * ******************************************************************************/
unsigned short qvPoolSequence(void *buffer)
{
	unsigned char *c = (unsigned char *)buffer;
	qbuf  *q = (qbuf *)(void *)(c - YALIGN(sizeof(qbuf), 8));

	return q->incarnation;
}

/*****************************************************************************
* * FUNCTION: qvPoolIndex
* *
* * DESCRIPTION:
* *              This function returns the index of 'buffer' in the pool 'Q'
* *              only if the buffer is allocated.
* *              Range of index is from 1 to nbuf where nbuf is the number
* *              of buffers in the pool.
* *
* * ARGS:
* *           Q               Pointer to the pool
* *           buffer          Pointer to the buffer.
* *
* * RETURNS:
* *                       index of buffer if successful, else 0
* *
*******************************************************************************/

unsigned int qvPoolIndex(void *Q, void *buffer)
{
	return qvPoolIndexEx(Q, buffer, 0);
}

unsigned int qvPoolIndexEx(void *Q, void *buffer, int *cErr)
{
	qpool *pool = (qpool *)Q;
	unsigned int var_index = 0;
	unsigned char *c = ((unsigned char *)buffer - YALIGN(sizeof(qbuf), 8));
#ifndef MEM_DEBUG
	unsigned int realsize = YALIGN(sizeof(qbuf), 8) + YALIGN(pool->size, 8);
	qchunk *q;
#endif

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	qvLock(pool->lock);
#ifndef MEM_DEBUG
	/*For loop to find out in which chunk the buffer lies.*/
	for (q = (qchunk *)ylFirst(&(pool->chunks)); q; q = (qchunk *)ylNext((YLNODE *)q)) {
		unsigned char *base = ((unsigned char *)q +  YALIGN(sizeof(qchunk), 8));

		if ((c >= base) && (c < (base + (realsize * q->nbuf)))) {
			/** Within range, is it aligned? **/
			if (((c - base) % realsize) != 0) {
				q = 0;
				if (cErr)
					*cErr = QVERROR_MEM_ILLEGAL_BUFFER;

				break;
			} else {
				if (!((qbuf *)(void *)c)->allocated) {
					q = 0;
					if (cErr)
						*cErr = QVERROR_MEM_UNALLOCATED_BUFFER;

					break;
				} else {
					/*Index is combination of position of buffer
					* * within various lists of chunks and its
					* * position within the chunk it exists in*/
					var_index += (c - base)/realsize;
					break;
				}
			}
		}
		var_index += q->nbuf;
	}
	var_index = q?(var_index + 1):0;
#else
	//var_index = ((qmbuf *)c)->ref_id;
#endif
	qvUnlock(pool->lock);
	return  var_index;
}

/*****************************************************************************
* * FUNCTION: __qvPoolLocate
* *
* * DESCRIPTION:
* *              Used to retrieve the pointer to a buffer in a pool based on the
* *              index of this buffer in that pool.
* *
* * ARGS:
* *           pool            Pointer to the pool
* *           index           Index of buffer in a pool.
* *
* * RETURNS:
* *                   Pointer to the buffer.
* *
*******************************************************************************/

static qbuf *__qvPoolLocate(qpool *pool, unsigned int indexparam)
{
#ifndef MEM_DEBUG
	unsigned int realsize = YALIGN(sizeof(qbuf), 8) + YALIGN(pool->size, 8);
#endif
	qchunk *q;
	unsigned char *c;

	if (!indexparam)
		return 0;
#ifndef MEM_DEBUG
	indexparam--;
#endif
	c = 0;

	for (q = (qchunk *)ylFirst(&(pool->chunks)); q; q = (qchunk *)ylNext((YLNODE *)q)) {
#ifndef MEM_DEBUG
		if (indexparam < q->nbuf) {
			c = (unsigned char *)q;
			c += YALIGN(sizeof(qchunk), 8);
			c += (realsize * indexparam);
			break;
		}
		indexparam -= q->nbuf;
#else
	c = (unsigned char *)q + YALIGN(sizeof(qchunk), 8);
		if (indexparam == ((qbuf *)c)->ref_id)
			return (qbuf *)c;
#endif
	}

#ifndef MEM_DEBUG
	return  (qbuf *)(void *)c;
#else
	return 0;
#endif
}

/*****************************************************************************
* * FUNCTION: qvPoolLocate
* *
* * DESCRIPTION:
* *             Used to retrieve the pointer to a buffer in a pool based on the
* *             index of this buffer in that pool. Buffer returned only
* *             if it is allocated.
* *
* * ARGS:
* *           pool            Pointer to the pool
* *           index           Index of buffer in a pool.
* *
* * RETURNS:
* *                   Pointer to the buffer.
* *
*******************************************************************************/

void *qvPoolLocate(void *Q, unsigned int indexparam)
{
	return qvPoolLocateEx(Q, indexparam, 0);
}

void *qvPoolLocateEx(void *Q, unsigned int indexparam, int *cErr)
{
	qbuf *m;
	unsigned char *c;

	/* If required, Reset the CSPL Error Code. */
	if (cErr)
		*cErr = QVERROR_NONE;

	qvLock(((qpool *)Q)->lock);
	m = __qvPoolLocate((qpool *)Q, indexparam);

	if (m && m->allocated) {
		c = (unsigned char *)m;
		c += YALIGN(sizeof(qbuf), 8);
	} else {
		c = 0;
		if (!m) {
			if (cErr)
				*cErr = QVERROR_MEM_ILLEGAL_INDEX;
		} else if (!m->allocated) {
			if (cErr)
				*cErr = QVERROR_MEM_UNALLOCATED_BUFFER;
		}
	}
	qvUnlock(((qpool *)Q)->lock);

	return (void *)c;
}

/*****************************************************************************
* FUNCTION: qvPoolDelete
* *
* * DESCRIPTION:
* *           This function deletes the memory allocated for the pool.
* * ARGS:
* *           Q     Pointer to Pool.
* *
* * RETURNS:
* *
* ******************************************************************************/
void qvPoolDelete(void    *Q)
{
    qpool      *pool = (qpool *)Q;
    qchunk     *chunk;
    QLOCK       lock = pool->lock;

    qvLock(lock);
    while ((chunk = (qchunk *)ylPopHead(&(pool->chunks))) != 0)
		qvSysFree((void *)chunk);

    qvSysFree((void *)pool);
    qvUnlock(lock);

    /* Changes corres to SPR No: 2339 */
    qvDestroyLock(lock);

}

/*****************************************************************************
* * FUNCTION: cvContextPoolSetup
* *
* * DESCRIPTION:
* *           The function creates a context pool.
* *
* * ARGS:
* *           cpool           Pointer to the allocated pool
* *           bufsize         Size of buffers in pool
* *           min             Minimun buffers in the pool
* *           max             Maximum buffers in the pool
* *           step            Number of buffers by which the pool is extended
* *                           when the pool is exhausted
* * RETURNS:  1 if successful, else 0
* *
* ******************************************************************************/

int cvContextPoolSetup(
	STACKCPOOL * cpool, unsigned int bufsize, unsigned int min,
	unsigned int max, unsigned int step)
{
    cpool->bufsize = bufsize;
    cpool->min = min;
    cpool->max = max;
    cpool->step = step;

    //if( (cpool->pool = qvPoolCreate( bufsize, min )) == 0 ) {
    if ((cpool->pool = qvPoolCreateAndReturn(bufsize, min)) == 0) {
		return 0;
    } else {
		return 1;
    }
}

/*****************************************************************************
 * * FUNCTION: cvContextAlloc
 * *
 * * DESCRIPTION:
 * *           This function allocates a buffer from the 'cpool'
 * * ARGS:
 * *           cpool           Pointer to the pool from which buffer is to
 * *                           be allocated
 * * RETURNS:
 * *           Pointer to the allocated buffer
 * *
 * ******************************************************************************/
void  *cvContextAlloc(STACKCPOOL *cpool)
{
#ifdef CSPL_LEAK_DEBUG_LITE
    void *m = qvPoolAlloc(cpool->pool, __func__, __LINE__);
#else
    void *m = qvPoolAlloc(cpool->pool);
#endif

    if (m) {
		return m;
    } else {/*Pool exhausted*/
		unsigned int n = qvPoolSize(cpool->pool, 0), step;

		if ((n < cpool->max) || !cpool->max) {/*If max not specified,
							pool extension is limitless*/
		    step = cpool->step;
			if ((cpool->max - n) < step)
				step = cpool->max - n;/*Pool can be extended only till its max limit*/
			if (!step) {/*If not step specified, extended by 1 only*/
				step = 1;
			}
			if (!qvPoolExtend(cpool->pool, step)) {
				return 0;
			}
		}
#ifdef CSPL_LEAK_DEBUG_LITE
		m = qvPoolAlloc(cpool->pool, __func__, __LINE__);
#else
		m = qvPoolAlloc(cpool->pool);
#endif
		return m;
    }
}

/*****************************************************************************
* * FUNCTION: qvPoolSize
* *
* * DESCRIPTION:
* *           This function provides the size of pool in terms of number
* *           of buffers in the pool. If the second argument is
* *           not null, it is set to the number of buffers already
* *           allocated in the pool.
* * ARGS:
* *           Q               Pointer to Pool.
* *           p_alloc         Out param, returns number of buffers
* *                           already allocated in the pool.
* *
* * RETURNS:
* *           Size of the pool
* *
* ******************************************************************************/
unsigned int qvPoolSize(void *Q, unsigned int *p_alloc)
{
    qpool *pool = (qpool *)Q;
    unsigned int nbuf, nalloc;

    qvLock(pool->lock);
    nbuf = pool->nbuf;
    nalloc = pool->alloc;
    qvUnlock(pool->lock);

    if (p_alloc)
		*p_alloc = nalloc;

    return nbuf;
}

#ifdef CSPL_LEAK_DEBUG_LITE
void qvPoolFree(void *buf, char *func, int line)
#else
void qvPoolFree(void *buf)
#endif
{
#ifdef CSPL_LEAK_DEBUG_LITE
	__qvPoolFree(buf, func, line);
#else
	__qvPoolFree(buf);
#endif
}

/*****************************************************************************
 * FUNCTION: qvMemPoolGetUsagePercentage
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
unsigned int qvMemPoolGetUsagePercentage(void)
{
    /* Start 128 UE: Memory Changes */
    unsigned int i = 8;
    /* End 128 UE: Memory Changes */
    unsigned int totalBuf = 0;
    unsigned int allocatedBuf = 0;
    unsigned int usagePercentage = 0;

    /* Start 128 UE: Memory Changes */
    /*
    ** Memory Usage will be calculated for only 2k Memory pool.
    */
    allocatedBuf = ((qpool *)qd->alloc[i])->alloc;
    totalBuf = ((qpool *)qd->alloc[i])->nbuf;
    /* End 128 UE: Memory Changes */
    /* SPR 11109 16-JUN-2014 Fix start */
    if (totalBuf)
		usagePercentage = (allocatedBuf * 100)/totalBuf;

    /* SPR 11109 16-JUN-2014 Fix end */
    return usagePercentage;
}

#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
void qvPrintMemPoolAllocBuffers(void)
{
	qpool *pool;
	qchunk *chunk;
	unsigned int realsize, i, j;
	unsigned char *p;
	unsigned long long leakMemory = 0;
	unsigned int bufferLeaked = 0;

	for (j = 0; j < NVARPOOL; j++) {
		pool = qd->alloc[j];
		fprintf(stderr, "MEMORY POOL SIZE [%u] BUFFERS LEAK SUMMARY:\n", pool->size);
		realsize = YALIGN(sizeof(qbuf), 8) + YALIGN(pool->size, 8);
		chunk = (qchunk *)ylFirst(&pool->chunks);

		while (chunk) {
			p = (unsigned char *) chunk;

			for (i = 0, p += YALIGN(sizeof(qchunk), 8); i < chunk->nbuf; i++, p += realsize) {
				qbuf *q = (qbuf *)(void *)p;

				if (q->allocated != 0) {
					fprintf(stderr, "\tFUNC [%s] LINE [%u]\n", q->func, q->line);
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

void qvPrintMemPoolAllocBuf(qpool   *pool)
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

			if (q->allocated != 0) {
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

    for (i = 0; i < NVARPOOL; i++) {
		fprintf(stderr, "##### Pool[%d] Size [%d] Total Buffer [%d] Allocated Buffer[%d] #####\n",
				i, qd->alloc[i]->size, qd->alloc[i]->nbuf, qd->alloc[i]->alloc);
		qvPrintMemPoolAllocBuf(qd->alloc[i]);
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
    if (strings == NULL)
		perror("backtrace_symbols");

    for (j = 0; j < nptrs; j++)
		fprintf(stderr, "%s\n", strings[j]);

    free(strings);
}

#endif
