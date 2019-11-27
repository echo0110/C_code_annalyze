/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include <assert.h>
#include "ylib.h"

#define	QV_NIL	((YLNODE *)0)


/** Doubly-Linked Lists *****************************************************/
void ylInit(YLIST *pList)
{
	pList->count = 0;
	pList->node.next = QV_NIL;
	pList->node.previous = QV_NIL;
}

void ylPushTail(YLIST *pList, YLNODE *pNode)
{
	if( pList->node.next == QV_NIL ) {
		/** This is the first element of the list **/
		pList->node.next = pNode;
		pList->node.previous = pNode;
		pNode->next = QV_NIL;
		pNode->previous = QV_NIL;
		pList->count = 1;
	} else {
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
	if( pList->node.next == QV_NIL ) {
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

void ylDelete(YLIST *pList, YLNODE *pNode)
{
	assert( pList->count );

	if(pNode->next == QV_NIL) {
		/** Last in List **/
		pList->node.previous = pNode->previous;
	} else {
		pNode->next->previous = pNode->previous;
	}

	if(pNode->previous == QV_NIL) {
		/** First in List **/
		pList->node.next = pNode->next;
	} else {
		pNode->previous->next = pNode->next;
	}

	pList->count--;
}

YLNODE *ylFirst(const YLIST *pList)
{
	return(pList->node.next);
}

YLNODE *ylLast(const YLIST *pList)
{
	return(pList->node.previous);
}


void ylInsertAfter(YLIST *pList, YLNODE *pPrev, YLNODE *pNode)
{
	if( pPrev == QV_NIL ) {
		ylPushHead( pList, pNode);
		return;
	} else if( (pList->count == 0) || (pPrev->next == 0) ) {
		ylPushTail( pList, pNode);
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
	if( pNext == QV_NIL ) {
		ylPushTail( pList, pNode);
		return;
	} else if( (pList->count == 0) || (pNext->previous == 0) ) {
		ylPushHead( pList, pNode);
		return;
	} 

	pNode->previous = pNext->previous;
	pNode->next = pNext;
	pNext->previous->next = pNode;
	pNext->previous = pNode;

	pList->count++;
}


YLNODE *ylNext(const YLNODE *pNode)
{
	return(pNode->next);
}


YLNODE *ylPrev(const YLNODE *pNode)
{
	return(pNode->previous);
}

YLNODE *ylPopHead(YLIST *pList)
{
	YLNODE	*N;

	N = pList->node.next;
	if (N) {
		ylDelete( pList, N );
	}
	return(N);
}

YLNODE *ylPopTail(YLIST *pList)
{
	YLNODE	*N;

	N = pList->node.previous;
	if (N) {
		ylDelete( pList, N );
	}
	return(N);
}

YLNODE *ylStep(YLNODE *pNode, int nStep)
{
	int	i;
	YLNODE	*p = pNode;

	if (nStep > 0) {
		for( i=0; i<nStep; i++ ) {
			if (p->next) {
				p = p->next;
			} else {
				return QV_NIL;
			}
		}
	} else if (nStep < 0) {
		for( i=0; i<(-nStep); i++ ) {
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

	p = ylFirst( pList );

	/** Assuming that numbering starts from 0 **/
	return( ylStep(p, nodenum) );
}

int	ylIndex(const YLIST *pList, YLNODE *pNode)
{
	YLNODE	*p;
	int	i = 0;

	p = ylFirst( pList );
	for( ; ; i++, p = p->next  ) {
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
	YLNODE	*last = ylLast( pDstList );
	YLNODE	*first = ylFirst( pAddList );

	if( !last ) {
		pDstList->node.next = first;
	} else {
		last->next = first;
	}

	if( !first ) {
		/* Nothing */
	} else {
		first->previous = last;
	}

	pDstList->node.previous = pAddList->node.previous;
	pDstList->count += pAddList->count;
}

void ylWalk(const YLIST *pList,  void (*walker)(YLNODE *, void *), void *z )
{
	YLNODE	*p;

	for( p = ylFirst(pList); p; p = ylNext(p) ) {
		walker( p, z );
	}
}

void	ylInsert(YLIST *L, YLNODE *N, const void *(*keyof)(const YLNODE *), int (*compare)(const void *, const void *) )
{
	YLNODE	*p = L->node.next;

	while( p && (compare( keyof(N), keyof(p) ) < 0) ) {
		p = p->next;
	}

	if( p ) {
		ylInsertBefore( L, p, N );
	} else {
		ylPushTail( L, N );
	}
	
}

YLNODE	*ylFind(const YLIST *pList, const void *pValue, const void *(*keyof)(const YLNODE *), 
		int (*compare)(const void *, const void *) )
{
	YLNODE	*p;

	for( p = pList->node.next; p ; p = p->next ) {
		if( !compare( pValue, keyof(p) ) ) {
			return p;
		}
	}
	return 0;
}

void	ylSort(YLIST *L, const void *(*keyof)(const YLNODE *), int (*compare)(const void *, const void *) )
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

	if( !head ) {
		return;
	}

	for( ; ; ) {
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

			while ( (psize > 0) || ( (qsize > 0) && q)) {
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
		/* Start SPR 4990 Changes. */ 
		if(tail)
		{
			tail->next = NULL;
		}
		/* End SPR 4990 Changes. */   

		if (nmerges <= 1)   { 
			L->node.next = head;
			L->node.previous = tail;
			return;
		}

		insize *= 2;
	}
}



/** Doubly Linked List, Alternate Implementation ****************************/
void	ycInit(YLIST *pList)
{
	pList->node.next = pList->node.previous = ycEnd(pList);
}

int	ycEmpty( const YLIST *pList )
{
	return	(pList->node.next == ycEnd(pList)) ;
}

void	ycInsertAfter(YLNODE *pPrev, YLNODE *pNode)
{
	pNode->previous = pPrev;
	pNode->next = pPrev->next;
	pNode->next->previous = pNode;
	pNode->previous->next = pNode;
}

void	ycInsertBefore(YLNODE *pNext, YLNODE *pNode)
{
	pNode->next = pNext;
	pNode->previous = pNext->previous;
	pNode->next->previous = pNode;
	pNode->previous->next = pNode;
}

YLNODE	*ycDelete( YLNODE *pNode)
{
	if( pNode->next != pNode ) {
		pNode->previous->next = pNode->next;
		pNode->next->previous = pNode->previous;
		return pNode;
	} else {
		return 0;
	}
}

void	ycConcat(YLIST *pDstList, YLIST *pAddList)
{
	pDstList->node.previous->next = pAddList->node.next;
	pAddList->node.next->previous = pDstList->node.previous;

	pDstList->node.previous = pAddList->node.previous;
	pAddList->node.previous->next = ycEnd( pDstList );

	pAddList->node.previous = pAddList->node.next = 0;
}

void	ycExtract(YLIST *pSrcList, YLNODE *pStartNode, YLNODE *pEndNode, YLIST *pDstList)
{
	pStartNode->previous->next = pEndNode->next;
	pEndNode->next->previous = pStartNode->previous;
    pSrcList=pSrcList;   
	pDstList->node.next = pStartNode;
	pDstList->node.previous = pEndNode;
	pStartNode->previous = pEndNode->next = ycEnd(pDstList);
}

YLNODE	*ycFind(const YLIST *pList, const void *pValue, const void *(*keyof)(const YLNODE *), 
		int (*compare)(const void *, const void *) )
{
	YLNODE	*p; 
	const YLNODE	
		*z = &(pList->node);

	for( p = pList->node.next; p != z; p = p->next ) {
		if( !compare( pValue, keyof(p) ) ) {
			return p;
		}
	}
	return 0;
}

void	ycSort(YLIST *L, const void *(*keyof)(const YLNODE *), int (*compare)(const void *, const void *) )
{
	YLNODE	*p, 
		*q, 
		*e, 
		*z = &(L->node),
		*tail, 
		*head = L->node.next;
	int	insize = 1, 
		nmerges, 
		psize, 
		qsize, 
		i;

	if( head == z ) {
		return;
	}

	for( ; ; ) {
		p = head;
		head = z;
		tail = z;
		nmerges = 0;  

		while (p != z) {
			nmerges++;  
			q = p;
			psize = 0;
			for (i = 0; i < insize; i++) {
				psize++;
                                if(q) {
				    q = q->next;
                                }
				if (q == z) {
					break;
				}
			}

			qsize = insize;

			while ( (psize > 0) || ( (qsize > 0) && q)) {
				if (psize == 0) {
					e = q; q = q->next; qsize--;
				} else if (qsize == 0 || !q) {
					e = p; 
					if(p) {
						p = p->next; 
					}
					psize--;
				} else if (compare(keyof(p), keyof(q)) <= 0) {
					e = p; 
					if(p) {
						p = p->next; 
					}
					psize--;
				} else {
					e = q; q = q->next; qsize--;
				}

				if (tail != z) {
                                        if(tail) {
					    tail->next = e;
                                        }
				} else {
					head = e;
				}
				if (e) {
				    e->previous = tail;
				}
				tail = e;
			}

			p = q;
		}
		tail->next = NULL;

		if (nmerges <= 1)   { 
			L->node.next = head;
			L->node.previous = tail;
			return;
		}

		insize *= 2;
	}
}

