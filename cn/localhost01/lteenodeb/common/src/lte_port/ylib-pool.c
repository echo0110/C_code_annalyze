/***************************************************************************
*
* ARICENT -
*
* Copyright (C) 2014 Aricent Inc . All Rights Reserved.
*
****************************************************************************
*
* \$Id: ylib-pool.c
*
****************************************************************************
*
* File Description: This file contains APIs for operations on Ylib's
*                   Memory Pools.
*
****************************************************************************
* Revision Details
* ----------------------
*
***************************************************************************/

#include <assert.h>
#include "ylib.h"
#include "atomic-ops.h"
#include "lteMisc.h"

#define	QV_NIL	((YLNODE *)0)

/** Pools (Free-Lists) ******************************************************/
void ypInit(YPOOL *F)
{
	F->count = 0;
	F->link = 0;
}

unsigned int ypCount(const YPOOL *F)
{
	return F->count;
}

YPNODE *ypGet(YPOOL *F)
{
	YPNODE	*p = F->link;

	if (!p)
		return 0;

	F->link = p->link;
	F->count--;
	return p;
}

#ifdef __PPC__
YPNODE *ypGetAtomic(YPOOL *F)
{
    YPNODE *p;

    __asm__ volatile ("asmypgat:");
    lwarx(p, F->link);

    if (!p)
		return 0;

    stwcx(&F->link, p->link);
    __asm__ volatile("bne- asmypgat");
    atomicDecrement(&F->count);

    return p;
}
/* SPR 4283 Fix Start */
#elif __arm__
YPNODE *ypGetAtomic(YPOOL *F)
{
    YPNODE *p;
    size_t error;
asmypgat:

    //__asm__ volatile ("asmypgat:");
    ldrex(p, &F->link);

    if (!p)
		return 0;

    strex(&F->link, p->link, error);
    //__asm__ volatile("bne asmypgat");
    if (error) {
		fprintf(stderr, "strex failed in get atomic\n");
		goto asmypgat;
	}
	atomicDecrement(&F->count);
	return p;
}
/* SPR 4283 Fix End */
#else
YPNODE *ypGetAtomic(YPOOL *F)
{
    YPNODE *p = NULL;

    do {
		p = F->link;

		if (!p)
			return 0;
/*SPR 20636 Changes Start*/
    } while (!BOOL_COMPARE_AND_SWAP_ADDR(&F->link, p, p->link));
/*SPR 20636 Changes End*/
    SUB_AND_FETCH(&F->count, 1);
    return p;
}
#endif

void ypPut(YPOOL *F, YPNODE *N)
{
	N->link = F->link;
	F->link = N;
	F->count++;
}

#ifdef __PPC__
void ypPutAtomic(YPOOL *F, YPNODE *N)
{
    __asm__ volatile ("asmyppat:");
    lwarx(N->link, F->link);
    stwcx(&F->link, N);
    __asm__ volatile("bne- asmyppat");
	atomicIncrement(&F->count);
}
/* SPR 4283 Fix Start */
#elif __arm__
void ypPutAtomic(YPOOL *F, YPNODE *N)
{
    size_t error;
asmyppat:
    //__asm__ volatile ("asmyppat:");
    ldrex(N->link, &F->link);
    strex(&F->link, N, error);
    if (error) {
		fprintf(stderr, "strex failed in ypput atomic\n");
		goto asmyppat;
    }
    //__asm__ volatile("bne asmyppat");
    atomicIncrement(&F->count);
}
/* SPR 4283 Fix End */
#else
void ypPutAtomic(YPOOL *F, YPNODE *N)
{
    do {
		N->link = F->link;
/*SPR 20636 Changes Start*/
    } while (!BOOL_COMPARE_AND_SWAP_ADDR(&F->link, N->link, N));
/*SPR 20636 Changes End*/
    ADD_AND_FETCH(&F->count, 1);
}
#endif

