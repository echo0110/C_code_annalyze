/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include <assert.h>
#include "ylib.h"

#define	QV_NIL	((YLNODE *)0)

/** Priority Queues (Non-heap!) *********************************************/
void	ysInit(YSQUEUE *Q, int (*priority)(const YSNODE *) )
{
	Q->count = 0;
	Q->head = Q->tail = 0;
	Q->priority = priority;
}

unsigned int	ysCount(const YSQUEUE *Q)
{
	return Q->count;
}

YSNODE	*ysPopHead( YSQUEUE *Q )
{
	YSNODE	*N = Q->head;
	if( N ) {
		if( N->h.next == 0 ) {
			Q->head = N->v.next;

			if(N->v.next == 0) {
				Q->tail = 0;
			} else {
				N->v.next->v.previous = N->v.previous;
			}
		} else {
			Q->head = N->h.next;
			N->h.next->v.previous = 0;
			N->h.next->v.next = N->v.next;

			if( N->h.previous == N->h.next ) {
				N->h.next->h.previous = 0;
			} else {
				N->h.next->h.previous = N->h.previous;
			}

			if(N->v.next == 0) {
				Q->tail = N->h.next;
			} else {
				N->v.next->v.previous = N->h.next;
			}
		}

		Q->count--;
		return N;
	} else {
		return 0;
	}
}

void	ysPushTail( YSQUEUE *Q, YSNODE *N )
{
	if( Q->head == 0 ) {
		Q->head = Q->tail = N;
		N->v.previous = N->h.previous = N->v.next = N->h.next = 0;

		Q->count++;
		return;
	} else {
		YSNODE	*scan = Q->head;
		int	priority = Q->priority(N);

		while( scan ) {
			int	thisprio = Q->priority(scan);	

			if( priority == thisprio ) {
				N->h.next = 0;
				if( scan->h.previous ) {
					N->h.previous = scan->h.previous;
					scan->h.previous->h.next = N;
				} else {
					scan->h.next = N;
					N->h.previous = scan;
				}
				scan->h.previous = N;
	
				Q->count++;
				return;
			} else if (priority < thisprio) {
				N->h.previous = N->h.next = 0;
				if( scan->v.previous == 0 ) {
					Q->head = N; 
					N->v.previous = 0;
				} else {
					scan->v.previous->v.next = N; 
					N->v.previous = scan->v.previous;
				}
				N->v.next = scan; scan->v.previous = N;
	
				Q->count++;
				return;
			} else {
				scan = scan->v.next;
			}
		}

		assert( Q->tail );

		N->h.previous = N->h.next = 0;
		Q->tail->v.next = N;
		N->v.previous = Q->tail;
		N->v.next = 0;
		Q->tail = N;

		Q->count++;
		return;
	}
}

