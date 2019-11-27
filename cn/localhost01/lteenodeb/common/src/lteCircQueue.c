/***************************************************************************
 * *
 * *  ARICENT -
 * *
 * *  Copyright (c) Aricent.
 * *
 * ****************************************************************************
 * *
 * *  $Id: lteCircQueue.c,v 1.1.1.1 2011/11/29
 * *
 * ****************************************************************************
 * *
 * *  File Description : This file contains bounded buffered Queue
 * *                     implementation.
 * *
 * ****************************************************************************
 *
 ****************************************************************************/

/****************************************************************************

 * * Standard Library Includes
 * ****************************************************************************/
#include "lteCircQueue.h"
#include "lteMisc.h"
#include "lteLog.h"
/****************************************************************************
 * * Project Includes
 * ****************************************************************************/

/****************************************************************************
Private Definitions
****************************************************************************/
#ifdef MAC_AUT_TEST

#define AUT_TEST_VAL 10
UInt8 enqueue_check_g;

#endif
/****************************************************************************
 * Private Types
 * ****************************************************************************/

/****************************************************************************
 * * Private Function Prototypes
 * ****************************************************************************/

/*****************************************************************************
 * * Private Constants
 * ****************************************************************************/

/****************************************************************************
 * * Exported Variables
 * ****************************************************************************/

/****************************************************************************
 * * Private Variables (Must be declared static)
 * ****************************************************************************/

/****************************************************************************
* Function Name  : circQueueInit
* Inputs         : list : Pointer to the Queue to be initialized
*                : maxNodes : max no of nodes in queue
* Outputs        : Queue Initialization
* Returns        : CIRC_SUCCESS opr CIRC_FAILURE
* Variables      :
* Description    : This function performs the initialization of the Circular Queue
		   whose pointer is passed as an arguement to it.
****************************************************************************/
#if ((defined CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
UInt32 _circQInit(LTE_CIRCQUEUE *list, UInt32 maxNodes, UInt8 *func, UInt32 *line)
#else
UInt32 circQInit(LTE_CIRCQUEUE *list, UInt32 maxNodes)
#endif
{
	UInt32 i = 2;
	SInt32 idx = 0;

	list->max = 0;
	list->count = 0;
	while (i < maxNodes)
		i = i * 2;

	list->max = i - 1;

	/* 128 UE Memory optimization Start */
	createMemPool((sizeof(LTE_CNODE) * (list->max + 1)), 1);
	/* 128 UE Memory optimization End */
#if ((defined CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
	list->ring = _getMemFromPoolDebug(sizeof(LTE_CNODE) * (list->max + 1), PNULL, func, line);
#else
	list->ring = getMemFromPool(sizeof(LTE_CNODE) * (list->max + 1), PNULL);
#endif
	/* Coverity TI Fix start */
	if (list->ring == PNULL) {
		/* SPR 23483 Changes Start */
		return CIRC_FAILURE;
		/* SPR 23483 Changes End */
	}
	/* Coverity TI Fix end */
	for (idx = 0; idx <= list->max; idx++) {
		list->ring[idx].valid = CIRC_INVALID;
		list->ring[idx].data = PNULL;
	}

	list->head = 0;
	list->tail = 0;
	list->count = 0;
	/* SPR 23483 Changes Start */
	return CIRC_SUCCESS;
	/* SPR 23483 Changes End */
}

/* 128 UE Memory optimization Start */
/****************************************************************************
* Function Name  : circQInitAfterCellSetup
* Inputs         : list : Pointer to the Queue to be initialized
*                : maxNodes : max no of nodes in queue
* Outputs        : Queue Initialization
* Returns        : CIRC_SUCCESS opr CIRC_FAILURE
* Variables      :
* Description    : This function performs the initialization of the Circular Queue
		   whose pointer is passed as an arguement to it.
****************************************************************************/
UInt32 circQInitAfterCellSetup(LTE_CIRCQUEUE *list, UInt32 maxNodes)
{
    UInt32 i = 2;
    SInt32 idx = 0;

    list->max = 0;
    list->count = 0;
    while (i < maxNodes)
		i = i * 2;

    list->max = i - 1;
    list->ring = getMemFromPool(sizeof(LTE_CNODE) * (list->max + 1), PNULL);
    /* Coverity TI Fix start */
    if (list->ring == PNULL) {
		/* SPR 23483 Changes Start */
		return CIRC_FAILURE;
		/* SPR 23483 Changes End */
    }
    /* Coverity TI Fix end */
    for (idx = 0; idx <= list->max; idx++) {
		list->ring[idx].valid = CIRC_INVALID;
		list->ring[idx].data = PNULL;
    }
    list->head = 0;
    list->tail = 0;
    list->count = 0;
    /* SPR 23483 Changes Start */
    return CIRC_SUCCESS;
    /* SPR 23483 Changes End */
}
/* 128 UE Memory optimization End */

/* API Introduced for EMBMS */
/****************************************************************************
* Function Name  : circQReInit
* Inputs         : list : Pointer to the Queue to be initialized
*                : maxNodes : max no of nodes in queue
* Outputs        : Queue Re-Initialization
* Returns        : SUCCESS opr FAILURE
* Variables      :
* Description    : This function performs the Re-initialization of the Circular Queue
		   whose pointer is passed as an arguement to it.
****************************************************************************/
UInt32 circQReInit(LTE_CIRCQUEUE *list, UInt32 maxNodes)
{
    UInt32 i = 2;
    SInt32 idx = 0;

    list->max = 0;
    list->count = 0;
    while (i < maxNodes)
		i = i*2;

    list->max = i - 1;
    if (list->ring == PNULL) {
		/* SPR 23483 Changes Start */
		return CIRC_FAILURE;
		/* SPR 23483 Changes End */
    }

    for (idx = 0; idx <= list->max; idx++) {
		list->ring[idx].valid = CIRC_INVALID;
		list->ring[idx].data = PNULL;
    }

    list->head = 0;
    list->tail = 0;
    list->count = 0;
    /* SPR 23483 Changes Start */
    return CIRC_SUCCESS;
    /* SPR 23483 Changes End */
}

/****************************************************************************
* Function Name  : pushNodeCircQ
* Inputs         : list - Pointer to the Queue
*                : *data - data(pointer to the structure) to be pushed in queue
* Outputs        :
* Returns        : CIRC_SUCCESS and CIRC_FAILURE
* Variables      :
* Description    : This function pushes 1 node to the Queue
****************************************************************************/
/*
 * push
 * Assumption :
 *     list-> max will always be 2^n - 1. i.e. maximum nodes in the list can
 *     be 2^n. This is because, by doing this increment of tail and head
 *     with wrap around becomes atomic by atomic inrementing and anding
 *     operations.
 *
 *     tail point to next free node.
 *
 * steps :
 *     1. fetch tail in temp variable (so that another push operation does
 *        not impact the next check.
 *     2. if (head == tail) => list is full, hence return failure.
 *     3. mark node as pending addition (valid flag). Now, another addition
 *        will assume that this node will be added, while another deletion
 *        will assume that the node is yet not added.
 *     4. if marking fails, try to mark next node by continuing.
 *     5. if marking successful, we can safely use this slot.
 *     6. save data to this slot.
 *     7. move tail ahead.
 *     8. mark this slot as valid.
 *
 */
UInt32 pushNodeCircQ(LTE_CIRCQUEUE *list, void *data)
{
	UInt32 prev;
	UInt32 temp;

#ifdef MAC_AUT_TEST
	if (enqueue_check_g == AUT_TEST_VAL)
		return CIRC_FAILURE;
#endif

	prev = list->tail;
	do {
		prev = prev & list->max;
		if (list->count == (list->max + 1)) {
			lteWarning("push failed head = %d, tail = %d\n", list->head, list->tail);
			/* SPR 23483 Changes Start */
			return CIRC_FAILURE;
			/* SPR 23483 Changes End */
		}
		if (!BOOL_COMPARE_AND_SWAP(&list->ring[prev].valid, CIRC_INVALID, PENDING)) {
			FETCH_AND_ADD(&prev, 1);

			continue;
		} else {
			break;
		}
	} while (1);

	list->ring[prev].data = data;
	if (!BOOL_COMPARE_AND_SWAP(&list->ring[prev].valid, PENDING, CIRC_VALID))
		ltePanic("......How did this happen?\n");

	/* Rel 5.3: Coverity 54148 Fix Start */
	FETCH_AND_ADD(&list->tail, 1);
	/* Rel 5.3: Coverity 54148 Fix End */
	/* - coverity 32890 */
	do {
		temp = list->tail;
	} while (!BOOL_COMPARE_AND_SWAP(&list->tail, temp, temp & list->max));
	/* + coverity 32889 */
	/* Rel 5.3: Coverity 54147 Fix Start */
	FETCH_AND_ADD(&list->count, 1);
	/* Rel 5.3: Coverity 54147 Fix End */
	/* - coverity 32889 */

	/* SPR 23483 Changes Start */
	return CIRC_SUCCESS;
	/* SPR 23483 Changes End */
}

/****************************************************************************
* Function Name  : popNodeCircQ
* Inputs         : queue_p - Pointer to the Queue
* Outputs        : Deletes a node from the head of the Queue
* Returns        : void **outData - pointer to data popped from Queue
* Variables      :
* Description    : This function deletes 1 node from the Queue and returns it
		   to the calling function.
****************************************************************************/

/*
 * pop
 *
 * steps:
 *     1. save head to local variable.
 *     2. if head + 1 == tail, then return fail, as no nodes in the list.
 *     3. move head ahead, to po the node (if head has not changed yet).
 *     4. If head has changed retry.
 *     5. If the popped node is invalid (i.e. it has already been deleted),
 *        then throw away the node and retry to pop another one.
 *     6. Else, return the data from popped node.
 *     7. Inrement head with wrap around.
 *
 */
 UInt32 popNodeCircQ(LTE_CIRCQUEUE *list, void **outData)
{
	UInt32 prev;
	UInt32 next;

	prev = list->head & list->max;
	next = (prev + 1) & list->max;

	do {
		if (list->count == 0) {
			lteWarning("pop failed head = %d, tail = %d\n", list->head, list->tail);
			/* SPR 23483 Changes Start */
			return CIRC_FAILURE; //no node in ring
			/* SPR 23483 Changes End */
		}
		if (list->ring[prev].valid == PENDING) {
			// cannot pop, a push is ongoing
			// This may result in one thread pushing a node, but not
			// able to pop it as another thread is also pushing a node
			// Cannot continue, as it will create a spin lock
			lteWarning("ongoing push, cannot pop head = %d, tail = %d\n", list->head, list->tail);
			/* +SPR 15234 */
			/* SPR 23483 Changes Start */
			return CIRC_PARTIAL_FAILURE;
			/* SPR 23483 Changes End */
			/* -SPR 15234 */
		}
		if (!BOOL_COMPARE_AND_SWAP(&list->head, prev, next)) {
			prev = (list->head) & list->max;
			next = (prev + 1) & list->max;
			continue;
		} else if (list->ring[prev].valid == CIRC_INVALID) {
			//throw away invalid (already deleted) node
			// call back free : need to free the memory here (or while deleteing
			lteWarning("fail: freeing memory\n");
			prev = list->head & list->max;
			next = (prev + 1 )& list->max;
			continue;
		} else {
			break;
		}
	} while (1);

	*outData = list->ring[prev].data;
	/* + SPR#7373 */
	FETCH_AND_SUB(&list->count, 1);
	/* - SPR#7373 */

	if (!BOOL_COMPARE_AND_SWAP(&list->ring[prev].valid, CIRC_VALID, CIRC_INVALID)) {
		/*commented code deleted*/
		*outData = PNULL;
		/* SPR 23483 Changes Start */
		return CIRC_FAILURE;
		/* SPR 23483 Changes End */
		/** SPR 3578 Changes End **/
	}
	/* SPR 23483 Changes Start */
	return CIRC_SUCCESS;
	/* SPR 23483 Changes End */
}

UInt32 getNodeCircQ(LTE_CIRCQUEUE *list, void **outData)
{
	UInt32 prev;

	prev = list->head & list->max;
	do {
		if (list->count == 0) {
		    lteWarning("pop failed head = %d, tail = %d\n", list->head, list->tail);
		    /* SPR 23483 Changes Start */
			return CIRC_FAILURE; //no node in ring
		    /* SPR 23483 Changes End */
		}
		if (list->ring[prev].valid == PENDING) {
		    // cannot pop, a push is ongoing
		    // This may result in one thread pushing a node, but not
		    // able to pop it as another thread is also pushing a node
		    // Cannot continue, as it will create a spin lock
		    lteWarning("ongoing push, cannot pop head = %d, tail = %d\n", list->head, list->tail);
		    /* SPR 23483 Changes Start */
			return CIRC_FAILURE;
		    /* SPR 23483 Changes End */
		} else if (list->ring[prev].valid == CIRC_INVALID) {
		    //throw away invalid (already deleted) node
		    // call back free : need to free the memory here (or while deleteing
		    lteWarning("fail: freeing memory\n");
		     /* SPR 23483 Changes Start */
			return CIRC_FAILURE;
		     /* SPR 23483 Changes End */
		} else {
			break;
		}
	} while (0);

	*outData = list->ring[prev].data;
	/* SPR 23483 Changes Start */
	return CIRC_SUCCESS;
	/* SPR 23483 Changes End */
}

UInt32 getNextNodeCircQ(LTE_CIRCQUEUE *list, void **outData, UInt32 *PrevIndx)
{
	UInt32 prev = *PrevIndx;
	UInt32 next;

	next = (prev) & list->max;
	do {
		//if (next == list->tail)
		if (list->count == 0) {
		    lteWarning("pop failed head = %d, tail = %d\n", list->head, list->tail);
			/* SPR 23483 Changes Start */
			return CIRC_FAILURE; //no node in ring
			/* SPR 23483 Changes End */
		}
		if (list->ring[next].valid == PENDING) {
		    // cannot pop, a push is ongoing
		    // This may result in one thread pushing a node, but not
		    // able to pop it as another thread is also pushing a node
		    // Cannot continue, as it will create a spin lock
		    lteWarning("ongoing push, cannot pop head = %d, tail = %d\n", list->head, list->tail);
			/* SPR 23483 Changes Start */
			return CIRC_FAILURE;
			/* SPR 23483 Changes End */
		} else if (list->ring[next].valid == CIRC_INVALID) {
		    //throw away invalid (already deleted) node
		    // call back free : need to free the memory here (or while deleteing
		     lteWarning("fail: freeing memory\n");
		    // free(list->ring[prev].data);
		    //prev = list->head & list->max;
		    //prev = (prev + 1)&list->max;
		    //continue;
			/* SPR 23483 Changes Start */
			return CIRC_FAILURE;
			/* SPR 23483 Changes End */
		} else {
			break;
		}
	} while (0);

	*outData = list->ring[next].data;
	*PrevIndx = next;

	/* SPR 23483 Changes Start */
	return CIRC_SUCCESS;
	/* SPR 23483 Changes End */
}

/****************************************************************************
* Function Name  : circQueueCount
* Inputs         : queue_p - Pointer to the Queue
* Outputs        : None
* Returns        : UINT64 count - Number of nodes present in the Queue
* Variables      :
* Description    : This function counts the number of nodes present in Queue
		   whose pointer is passed as an arguement.
****************************************************************************/
UInt32 circQueueCount(LTE_CIRCQUEUE *list)
{
	if (list != PNULL)
		return(list->count);
	else
		return 0;
}

/****************************************************************************
* Function Name  : circQueueDelNode
* Inputs         : list : Pointer to the Queue to be initialized
*                : index: index of the buffer location to be deleted
* Outputs        : Queue Initialization
* Returns        : CIRC_SUCCESS opr CIRC_FAILURE
* Variables      :
* Description    : This function deletes the buffer from the Circular Queue
		   whose index is passed .
****************************************************************************/
UInt32 delNodeCircQ(LTE_CIRCQUEUE *list, UInt32 index)
{
	if (!list)
		return 0;

	/* SPR 3406 changes start */
	if (BOOL_COMPARE_AND_SWAP(&list->ring[index].valid, CIRC_VALID, CIRC_INVALID)) {
		FETCH_AND_SUB(&list->count, 1);
		/* SPR 23483 Changes Start */
		return CIRC_SUCCESS;
		/* SPR 23483 Changes End */
	} else {
		/* SPR 23483 Changes Start */
		return CIRC_FAILURE;
		/* SPR 23483 Changes End */
	}
	/* SPR 3406 changes end */

}
/****************************************************************************
* Function Name  : qLinearSearch
* Inputs         : list : Pointer to the Queue to be initialized
*                : compare: pointer to the function which searches and compares.
*                : arg : the data to be searched
* Outputs        : None
* Returns        : Index where the seached element is prsent
* Variables      :
* Description    : This function searches the Circular Queue
		   until the  arg is found .
****************************************************************************/
SInt32 qLinearSearch(LTE_CIRCQUEUE *list, UInt32 (*compare)(void *, void *), void *arg)
{
	/** SPR 3558 Changes Start **/
	SInt32 index = ((list->head) & list->max);

	while (1) {
		if (list->ring[index].valid == CIRC_VALID) {
			/* SPR 23483 Changes Start */
			if (CIRC_SUCCESS == (*compare)(arg, list->ring[index].data)) {
				/* SPR 23483 Changes End */
				return index;
			}
		}
		index = ((index + 1) & list->max);
		if (index == ((list->tail + 1) & list->max))
			break;
	}
	/** SPR 3558 Changes End **/
	return (-1);
}


/****************************************************************************
* Function Name  : circQDeInit
* Inputs         : list : Pointer to the Queue to be initialized
* Outputs        : Queue DeInitialization
* Returns        : CIRC_SUCCESS opr CIRC_FAILURE
* Variables      :
* Description    : This function performs the de-initialization of the Circular Queue
		   whose pointer is passed as an arguement to it.
****************************************************************************/
UInt32 circQDeInit(LTE_CIRCQUEUE *list)
{
	if (!list->ring)
	/* SPR 23483 Changes Start */
		return CIRC_SUCCESS;

	/* SPR 23483 Changes End */
	freeMemPool(list->ring);
	list->ring = PNULL;
	/* SPR 23483 Changes Start */
	return CIRC_SUCCESS;
	/* SPR 23483 Changes End */
}

