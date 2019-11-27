/***************************************************************************
*
* ARICENT -
*
* Copyright (C) 2014 Aricent Inc . All Rights Reserved.
*
****************************************************************************
*
* \$Id: ylib-list.c
*
****************************************************************************
*
* File Description: This file contains APIs for operating on List Data
*                   structure of Ylib.
*
****************************************************************************
* Revision Details
* ----------------------
*
***************************************************************************/

#include <assert.h>
#include "ylib.h"
#include "lteLog.h"

#define	QV_NIL	((YLNODE *)0)
#define NUM_SEM 1


/** Doubly-Linked Lists *****************************************************/
void ylInit(YLIST *pList)
{
	pList->count = 0;
	pList->node.next = QV_NIL;
	pList->node.previous = QV_NIL;
    /*SPR_7217_changes_start*/
    //semInit(&(pList->sem_lock), NUM_SEM);
    /*SPR_7217_changes_end*/
}

void ylInit_qmsg(YLIST_QMSG *pList)
{
	pList->count = 0;
	pList->node.next = QV_NIL;
	pList->node.previous = QV_NIL;
}

void ylPushTail(YLIST *pList, YLNODE *pNode)
{
	if (pList->node.next == QV_NIL) {
		/** This is the first element of the list **/
		pList->node.next = pNode;
		pList->node.previous = pNode;
		pNode->next = QV_NIL;
		pNode->previous = QV_NIL;
		pList->count = 1;
	} else {
	if (pNode == pList->node.next)
	{
	    ltePanic("Same node inserted twice\n");
	}
		pNode->next = QV_NIL;
		pNode->previous = pList->node.previous;
		pList->node.previous->next = pNode;
		pList->node.previous = pNode;
		pList->count++;
	}
}

/** Add to the head of the list **/
void ylPushHead(YLIST *pList, YLNODE *pNode)
{
	if (pList->node.next == QV_NIL) {
		/** This is the first element of the list **/
		pList->node.next = pNode;
		pList->node.previous = pNode;
		pNode->next = QV_NIL;
		pNode->previous = QV_NIL;
		pList->count = 1;
	} else {
		pNode->previous = QV_NIL;
		pNode->next = pList->node.next;
		pList->node.next->previous = pNode;
		pList->node.next = pNode;
		pList->count++;
	}
}

unsigned int ylCount(const YLIST *pList)
{
	return(pList->count);
}

/* SPR 5599 changes start (ZIP ID 128826) */
void ylDelete(YLIST *pList, YLNODE *pNode)
/* SPR 5599 changes end (ZIP ID 128826) */
{

	assert(pList->count);

	if (pNode->next == QV_NIL) {
		/** Last in List **/
		pList->node.previous = pNode->previous;
	} else {
		pNode->next->previous = pNode->previous;
	}

	if (pNode->previous == QV_NIL) {
		/** First in List **/
		pList->node.next = pNode->next;
	} else {
		pNode->previous->next = pNode->next;
	}

	pList->count--;
}

/*********** ZBC OPTIMIZATION start*****************/
/* SPR 5599 changes start (ZIP ID 128826) */
void ylDelete_first(YLIST *pList, YLNODE *pNode)
/* SPR 5599 changes end (ZIP ID 128826) */
{

	pNode->next->previous = pNode->previous;
	pList->node.next = pNode->next;
	pList->count--;
}

/* SPR 5599 changes start (ZIP ID 128826) */
void ylDelete_last(YLIST *pList, YLNODE *pNode)
/* SPR 5599 changes end (ZIP ID 128826) */
{

	assert(pList->count);

	pList->node.previous = pNode->previous;

	if (pNode->previous == QV_NIL) {
		/** First in List **/
		pList->node.next = pNode->next;
	} else {
		pNode->previous->next = pNode->next;
	}

	pList->count--;
}

/* SPR 5599 changes start (ZIP ID 128826) */
void ylDelete_single(YLIST *pList, YLNODE *pNode)
/* SPR 5599 changes end (ZIP ID 128826) */
{

	assert(pList->count);

	pList->node.previous = pNode->previous;
	pList->node.next = pNode->next;
	pList->count--;
}
/*********** ZBC OPTIMIZATION end*******************/

/* SPR 5599 changes start (ZIP ID 128826) */
YLNODE *ylFirst(const YLIST *pList)
/* SPR 5599 changes end (ZIP ID 128826) */
{
	return(pList->node.next);
}

/* SPR 5599 changes start (ZIP ID 128826) */
YLNODE *ylLast(const YLIST *pList)
/* SPR 5599 changes end (ZIP ID 128826) */
{
	return(pList->node.previous);
}


void ylInsertAfter(YLIST *pList, YLNODE *pPrev, YLNODE *pNode)
{
	if (pPrev == QV_NIL) {
		ylPushHead(pList, pNode);
		return;
	} else if ((pList->count == 0) || (pPrev->next == 0)) {
		ylPushTail(pList, pNode);
		return;
	}

	pNode->next = pPrev->next;
	pNode->previous = pPrev;
	pPrev->next->previous = pNode;
	pPrev->next = pNode;

	pList->count++;
}

void ylInsertBefore(YLIST *pList, YLNODE *pNext, YLNODE *pNode)
{
	if (pNext == QV_NIL) {
		ylPushTail(pList, pNode);
		return;
	} else if ((pList->count == 0) || (pNext->previous == 0)) {
		ylPushHead(pList, pNode);
		return;
	}

	pNode->previous = pNext->previous;
	pNode->next = pNext;
	pNext->previous->next = pNode;
	pNext->previous = pNode;

	pList->count++;
}


/* SPR 5599 changes start (ZIP ID 128826) */
YLNODE *ylNext(const YLNODE *pNode)
/* SPR 5599 changes end (ZIP ID 128826) */
{
	return(pNode->next);
}


YLNODE *ylPrev(const YLNODE *pNode)
{
	return(pNode->previous);
}

/* SPR 5599 changes start (ZIP ID 128826) */
YLNODE *ylPopHead(YLIST *pList)
/* SPR 5599 changes end (ZIP ID 128826) */
{
	YLNODE	*N;

	N = pList->node.next;
	if (N) {
		ylDelete(pList, N);
	}
	return(N);
}

/* SPR 5599 changes start (ZIP ID 128826) */
YLNODE *ylPopTail(YLIST *pList)
/* SPR 5599 changes end (ZIP ID 128826) */
{
	YLNODE	*N;

	N = pList->node.previous;
	if (N) {
		ylDelete(pList, N);
	}
	return(N);
}

YLNODE *ylStep(YLNODE *pNode, int nStep)
{
	int	i;
	YLNODE	*p = pNode;

	if (nStep > 0) {
		for (i = 0; i < nStep; i++) {
			if (p->next) {
				p = p->next;
			} else {
				return QV_NIL;
			}
		}
	} else if (nStep < 0) {
		for (i = 0; i < (-nStep); i++) {
			if (p->previous) {
				p = p->previous;
			} else {
				return QV_NIL;
			}
		}
	}

	return (p);
}

YLNODE *ylNth(const YLIST *pList, unsigned int nodenum)
{
	YLNODE	*p;

	p = ylFirst(pList);

	/** Assuming that numbering starts from 0 **/
	return(ylStep(p, nodenum));
}

int	ylIndex(const YLIST *pList, YLNODE *pNode)
{
	YLNODE	*p;
	int	i = 0;

	p = ylFirst(pList);
	for ( ; ; i++, p = p->next) {
		if (p == pNode) {
			return i;
		}

		if (p == QV_NIL) {
			return (-1);
		}
	}
}

void	ylConcat(YLIST *pDstList, YLIST *pAddList)
{
	YLNODE	*last = ylLast(pDstList);
	YLNODE	*first = ylFirst(pAddList);

	if (!last) {
		pDstList->node.next = first;
	} else {
		last->next = first;
	}

	if (!first) {
		/* Nothing */
	} else {
		first->previous = last;
	}

	pDstList->node.previous = pAddList->node.previous;
	pDstList->count += pAddList->count;
}

void ylWalk(const YLIST *pList,  void (*walker)(YLNODE *, void *), void *z)
{
	YLNODE	*p;

	for (p = ylFirst(pList); p; p = ylNext(p)) {
		walker(p, z);
	}
}

void	ylInsert(YLIST *L, YLNODE *N, const void *(*keyof)(const YLNODE *), int (*compare)(const void *, const void *))
{
	YLNODE	*p = L->node.next;

	while (p && (compare( keyof(N), keyof(p)) < 0) ) {
		p = p->next;
	}

	if (p) {
		ylInsertBefore(L, p, N);
	} else {
		ylPushTail(L, N);
	}

}

YLNODE	*ylFind(const YLIST *pList, const void *pValue, const void *(*keyof)(const YLNODE *),
		int (*compare)(const void *, const void *))
{
	YLNODE	*p;

	for (p = pList->node.next; p ; p = p->next) {
		if (!compare( pValue, keyof(p)) ) {
			return p;
		}
	}
	return 0;
}

void	ylSort(YLIST *L, const void *(*keyof)(const YLNODE *), int (*compare)(const void *, const void *))
{
	YLNODE	*p,
		*q,
		*e,
		*tail,
		*head = L->node.next;
	int	insize = 1,
		nmerges,
		psize,
		qsize,
		i;

	if (!head) {
		return;
	}

	for ( ; ; ) {
		p = head;
		head = NULL;
		tail = NULL;
		nmerges = 0;

		while (p) {
			nmerges++;
			q = p;
			psize = 0;
			for (i = 0; i < insize; i++) {
				psize++;
				q = q->next;
				if (!q) {
					break;
				}
			}

			qsize = insize;

			while ((psize > 0) || ( (qsize > 0) && q)) {
				if (psize == 0) {
					e = q; q = q->next; qsize--;
				} else if (qsize == 0 || !q) {
					e = p; p = p->next; psize--;
				} else if (compare(keyof(p), keyof(q)) <= 0) {
					e = p; p = p->next; psize--;
				} else {
					e = q; q = q->next; qsize--;
				}

				if (tail) {
					tail->next = e;
				} else {
					head = e;
				}
				e->previous = tail;
				tail = e;
			}

			p = q;
		}
	/*++klockwork warning */
	if (tail)
	/*--klockwork warning */
	{
		    tail->next = NULL;
	}

		if (nmerges <= 1)   {
			L->node.next = head;
			L->node.previous = tail;
			return;
		}

		insize *= 2;
	}
}
