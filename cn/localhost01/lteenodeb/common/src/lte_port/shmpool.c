
/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: shmpool.c
 *
 *************************************************************************
 *
 * File Description : This file contains the functions and structure
 *                    that are used for shared message pool
 * Comments: never found any user for this shmpool
 *
 *************************************************************************/
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "ylib.h"
#include "atomic-ops.h"
#include "lteMisc.h"

/******************************************************************/

typedef pthread_mutex_t QLOCK;

/******************************************************************/
/* SPR 4971 Fix start */
#undef LOCKLESS_IMPL
/* SPR 4971 Fix End */

#define QV_NIL  ((YLNODE *)0)
#define __POOLMAGIC 0xCA98362D

typedef struct qpool {
	unsigned long   magic;
	unsigned int    size, nbuf, alloc, peakVal;
	YPOOL       list;
	QLOCK       lock;
} qpool;

typedef struct  qbuf {
	union {
		YPNODE  __header__;
		qpool   *pool;
	} u;

	unsigned char   allocated;
#ifdef CSPL_LEAK_DEBUG
	int nptrs;
	/* Max. No. of frames in Backtrace */
#define MAX_FRAMES_IN_BT 10
	void *buffer[MAX_FRAMES_IN_BT];
#endif

} qbuf;

static void qvShmMsgPoolNewLock(QLOCK *lock)//name changed
{
    pthread_mutexattr_t attr;

	if (pthread_mutexattr_init(&attr) != 0) {
		fprintf(stderr, "mutex attr init failed\n");
		abort();
	}
	if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0) {
		fprintf(stderr, "Unable to share mutex across process\n");
		abort();
	}
	if (pthread_mutex_init((pthread_mutex_t *)lock, &attr) != 0) {
		fprintf(stderr, "Unable to initialize mutex\n");
		abort();
	}
}

static void qvShmMsgPoolLock(QLOCK *lock)//name changed
{
	/* Coverity 110290 Fix Start */
	SInt32 n;

	if ((n = pthread_mutex_lock((pthread_mutex_t *)lock)) != 0)
		errno = n;
	/* Coverity 110290 Fix End */
}


static void qvShmMsgPoolUnlock(QLOCK *lock)//name changed
{
	/* Coverity 110290 Fix Start */
	SInt32 n;

	if ((n = pthread_mutex_unlock((pthread_mutex_t *)lock)) != 0)
		errno = n;

	/* Coverity 110290 Fix End */
}

intptr_t qvGetRelativeAddr(void *shmAddr, void *buffer)
{
	intptr_t offset = (intptr_t) buffer - (intptr_t)shmAddr;

	return offset;
}

void *qvGetPointer(void *shmAddr, intptr_t offset)
{
	void *ptr = (void *)((char *)shmAddr + offset);

	return ptr;
}

static void shmYpInit(YPOOL *F)
{
	F->count = 0;
	F->link = 0;
}

static YPNODE *shmYpGet(void *shmAddr, YPOOL *F)
{
	YPNODE *p = qvGetPointer(shmAddr, (intptr_t)F->link);

	if (!F->link)
		return 0;

	F->link = p->link;
	F->count--;

	return p;
}

static void shmYpPut(void *shmAddr, YPOOL *F, YPNODE *N)
{
	N->link = F->link;
	F->link = (void *)qvGetRelativeAddr(shmAddr, N);
	F->count++;
}

/*
 * calculate the size requirement for a qvshmpool, whose buffer number is nbuf,
 * each buffer size is bufsize
 */
int qvShmPoolGetSize(unsigned int bufSize, unsigned int nbuf)
{
	int totalSize = 0;
	int realBufSize = 0;

	/* Add size of aligned pool structure */
	totalSize += YALIGN(sizeof(qpool), 8);

	/* Calculate size of aligned buffer structure */
	realBufSize = bufSize + YALIGN(sizeof(qbuf), 8);

	/* Add size of nbuf buffer structures*/
	totalSize += realBufSize * nbuf;

	/* Return size */
	return totalSize;
}

int qvShmCreatePool(void *shmAddr, unsigned int shmSize, unsigned int poolOffset, unsigned int size, unsigned int nbuf)
{
	unsigned int poolSize = 0;
	qpool *pool;
	unsigned char *p;
	unsigned int realsize, i;

	/* Get size required for creating pool */
	poolSize = qvShmPoolGetSize(size, nbuf);

	/* If size is not available, then fail*/
	if (poolSize > (shmSize - poolOffset))
		return 0;

	/* create pool */
	pool = (qpool *)((char *)shmAddr + poolOffset);
	shmYpInit(&(pool->list));
	pool->size = size;
	pool->nbuf = nbuf;
	pool->alloc = 0;
	pool->magic = __POOLMAGIC;
	qvShmMsgPoolNewLock(&pool->lock);

	/* Create chunk */
	p = ((unsigned char *)pool + YALIGN(sizeof(qpool), 8));

	/** Compute the memory required **/
	realsize = YALIGN(sizeof(qbuf), 8) + YALIGN(size, 8);
	qvShmMsgPoolLock(&pool->lock);

	for (i = 0; i < nbuf; i++, p += realsize) {
		/* Create buffers */
		qbuf *q = (qbuf *)(void *)p;

		q->allocated = 0;
		shmYpPut(shmAddr, &(pool->list), (YPNODE *)q);
	}

	qvShmMsgPoolUnlock(&pool->lock);

	/* Covert the pointers to relative address */

	/* enqueue the buffers in pool's free list */
	return 1;
}

void *qvShmPoolAlloc(void *shmAddr, void *allocPool)
{
	void *p;
	qpool *pool = (qpool *)allocPool;

	qvShmMsgPoolLock(&pool->lock);
	if ((p = shmYpGet(shmAddr, &pool->list)) != 0)
		pool->alloc++;

	qvShmMsgPoolUnlock(&pool->lock);
	if (p) {
		unsigned char *c;
		qbuf *q = (qbuf *)p;

		q->allocated = 1;
		/*
		 * just using q->u.pool = pool in this function and using pool = q->u.pool in
		 * poolfree function is fine, but I did not verify this.
		 */
		q->u.pool = (qpool *)((intptr_t)pool - (intptr_t)shmAddr); /* gcc 5.0 */
		c = (unsigned char *)q;
		c += YALIGN(sizeof(qbuf), 8);

		if (pool->alloc > pool->peakVal)
		pool->peakVal = pool->alloc;
		return (void *)c;
	} else {
		return 0;
	}
}
void qvShmPoolFree(void *shmAddr, void *buffer)
{
	unsigned char *c = (unsigned char *)buffer;
	qbuf *q;
	qpool *pool;

	c -= YALIGN(sizeof(qbuf), 8);
	q = (qbuf *)(void *)c;
	pool = (qpool *)((intptr_t)shmAddr + (intptr_t)q->u.pool);
	/* Safe check is introduced to avoid double free */
	if (q->allocated == 0)
		return;

	q->allocated = 0;

	qvShmMsgPoolLock(&pool->lock);
	shmYpPut(shmAddr, &(pool->list), (YPNODE *)q);
	pool->alloc--;
	qvShmMsgPoolUnlock(&pool->lock);
}

/**** Test function ****/

/**** Test suite ****/
