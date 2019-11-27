/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Utility Library Interface Definition                                   **/
/**                                                                        **/
/****************************************************************************/

#ifndef __CommonStackPortingLayerUtilityLibraryHss__
#define __CommonStackPortingLayerUtilityLibraryHss__

#include <os.h>
#include "lteSync.h"

#define T_INORDER	0
#define T_PREORDER	1
#define T_POSTORDER	2

#define YOFFSETOF(s, f)			((unsigned long)(&(((s *)0)->f)))
#define __YMEMBEROF(s, f, p)	((s *)((unsigned char*)(p) - (unsigned char*)&((s *)0)->f))
#define YMEMBEROF(s, f, p)		((p) ? __YMEMBEROF(s, f, p) : ((s *)0))
/* Rel 5.3: Coverity 24417 Fix Start */
#define YMEMBEROFWO(s, f, p)	__YMEMBEROF(s, f, p)
/* Rel 5.3: Coverity 24417 Fix End */
#define MEMBEROF(s, f, p)		YMEMBEROF(s, f, p)
#define YALIGN(p, n)			((((p) + ((n) - 1))/(n))*(n))

/* uion of all types of node */
typedef union __ynode__		YNODE;
typedef struct __ylnode__	YLNODE;
typedef struct __yhnode__	YHNODE;
typedef	struct __ypnode__	YPNODE;
typedef struct __ytnode__	YTNODE;
typedef struct __ysnode__	YSNODE;

struct __ylnode__ {
	YLNODE *next;
	YLNODE *previous;
};

struct __ytnode__ {
	YTNODE *child[2];
	YTNODE *parent;
	unsigned char red;
};

struct __ysnode__ {
	struct {
		YSNODE *next;
		YSNODE *previous;
	} v, h;
};

struct __yhnode__ {
	YHNODE *link;
};

struct __ypnode__ {
	YPNODE *link;
};

union __ynode__ {
	YLNODE	l;
	YTNODE	t;
	YHNODE	h;
	YSNODE	s;
};

typedef struct{
	YLNODE node;
	unsigned int count;
    LTE_SEM sem_lock;
} YLIST;

typedef struct{
	YLNODE node;
	unsigned int count;
} YLIST_QMSG;

typedef struct  {
	YTNODE root;
	int	(*compare)(const void *, const void *);
	const void *(*keyof)(const YTNODE *);

	unsigned int count;
} YTREE;

typedef struct {
	unsigned long (*hash)(const void *);
	int	(*compare)(const void *, const void *);
	const void *(*keyof)(const YHNODE *);
	YHNODE **table;

	unsigned int nbuckets;
	unsigned int count;
} YHASH;

typedef struct {
	YPNODE	*link;
	unsigned int count;
} YPOOL;

typedef struct {
	YSNODE	*head;
	YSNODE	*tail;

	unsigned int count;
	int	(*priority)(const YSNODE *);
} YSQUEUE;

/** Red-black Trees *********************************************************/
void ytInit(YTREE *, int (*)(const void *, const void *), const void *(*)(const YTNODE *));

unsigned int ytCount(const YTREE *);
void ytWalk(const YTREE *, void (*)(YTNODE *, void *), void *);
void ytWalkOrder(const YTREE *, void (*)(YTNODE *, void *), void *, int);
YTNODE *ytFirst(const YTREE *, int);
YTNODE *ytLast(const YTREE *, int);
YTNODE *ytNext(const YTNODE *, int);
YTNODE *ytPrev(const YTNODE *, int);
/*QOS_CHG*/
void ytInsertWithDuplicate(YTREE *, YTNODE *);
/*QOS_CHG*/

void ytInsert(YTREE *, YTNODE *);
void ytDelete(YTREE *, YTNODE *);

YTNODE *ytFind(const YTREE *, const void *);
#define	ytEmpty(T)		((T)->count == 0)

unsigned int __ytHeight(const YTREE *);
int	__ytAudit(YTREE *);
/** Time               ******************************************************/

typedef struct {
	unsigned long   sec;
	unsigned short  msec;
} YTIME_T;

YTIME_T ytGetSystemTime(void);
signed long ytTimeDiff(const YTIME_T *ptm_one, const YTIME_T *ptm_two);
YTIME_T ytTimeAdd(const YTIME_T *ptm, signed long msec);

/** Doubly Linked List ******************************************************/
void ylInit(YLIST *pList);
void ylInit_qmsg(YLIST_QMSG *pList);

unsigned int ylCount(const YLIST *pList);
/* SPR 5599 changes start (ZIP ID 128826) */
YLNODE *ylFirst(const YLIST *pList);
YLNODE *ylLast(const YLIST *pList);
YLNODE *ylNext(const YLNODE *pNode);
/* SPR 5599 changes end (ZIP ID 128826) */
YLNODE *ylPrev(const YLNODE *pNode);

#define	ylEmpty(L)		((L)->count == 0)
void ylInsertAfter(YLIST *pList, YLNODE *pPrev, YLNODE *pNode);
void ylInsertBefore(YLIST *pList, YLNODE *pNext, YLNODE *pNode);
/* SPR 5599 changes start (ZIP ID 128826) */
void ylDelete(YLIST *pList, YLNODE *pNode);
void ylDelete_first(YLIST *, YLNODE *);
void ylDelete_last(YLIST *, YLNODE *);
void ylDelete_single(YLIST *, YLNODE *);
/* SPR 5599 changes end (ZIP ID 128826) */

void ylPushHead(YLIST *pList, YLNODE *pNode);
void ylPushTail(YLIST *pList, YLNODE *pNode);
/* SPR 5599 changes start (ZIP ID 128826) */
YLNODE *ylPopHead(YLIST *pList);
YLNODE *ylPopTail(YLIST *pList);
/* SPR 5599 changes end (ZIP ID 128826) */

void ylInsert(YLIST *, YLNODE *, const void *(*)(const YLNODE *), int (*)(const void *, const void *));
void ylWalk(const YLIST *, void (*)(YLNODE *, void *), void *);
YLNODE *ylFind(const YLIST *, const void *, const void *(*)(const YLNODE *), int (*)(const void *, const void *));
void ylSort(YLIST *, const void *(*)(const YLNODE *), int (*)(const void *, const void *));

void ylConcat(YLIST *pDstList, YLIST *pAddList);
void ylExtract(YLIST *pSrcList, YLNODE *pStartNode, YLNODE *pEndNode, YLIST *pDstList);

YLNODE *ylNth(const YLIST *pList, unsigned int nodenum);
YLNODE *ylStep(YLNODE *pNode, int nStep);
int	ylIndex(const YLIST *pList, YLNODE *pNode);

/** Hash Tables *************************************************************/
void yhInit(YHASH *, unsigned int, unsigned long (*)(const void *),
			int (*)(const void *, const void *), const void *(*)(const YHNODE *), YHNODE **);
void yhInsert(YHASH *, YHNODE *);
void yhDelete(YHASH *, YHNODE *);
unsigned int yhCount(const YHASH *);
#define yhEmpty(L)	((L)->count == 0)
YHNODE *yhFind(const YHASH *, const void *);
YHNODE *yhFirst(const YHASH *H);
YHNODE *yhNext(const YHASH *H, const YHNODE *N);

/** Pools (Free Lists) ******************************************************/
void ypInit(YPOOL *);
unsigned int ypCount(const YPOOL *);
#define	ypEmpty(L)	((L)->count == 0)
YPNODE *ypGet(YPOOL *);
YPNODE *ypGetAtomic(YPOOL *F);
void ypPut(YPOOL *, YPNODE *);
void ypPutAtomic(YPOOL *F, YPNODE *N);

/** Priority Queues *********************************************************/
void ysInit(YSQUEUE *, int (*)(const YSNODE *));
unsigned int ysCount(const YSQUEUE *);
#define	ysEmpty(L)		((L)->count == 0)
YSNODE *ysPopHead(YSQUEUE *);
void ysPushTail(YSQUEUE *, YSNODE *);

#endif
