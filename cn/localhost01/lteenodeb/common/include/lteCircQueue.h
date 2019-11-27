
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteCircQueue.h,v 1.1.1.1 2011/11/29
 *
 ****************************************************************************
 *
 *  File Description : Header file for Circular Queue
 *
 ****************************************************************************
 ****************************************************************************/
#ifndef _LTE_CQUEUE_
#define _LTE_CQUEUE_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include "lteTypes.h"
#include "lteSync.h"
#include "lteLog.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/


/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/** SPR 14758 Changes Start **/
typedef struct lteCnode_t {
#define CIRC_VALID 0xBADFACE
#define CIRC_INVALID 0xFEEDBAC
#define PENDING 0x0DDF00D
	int valid; /* validity =CIRC_VALID,CIRC_INVALID, PENDING */
	void *data;
} LTE_CNODE;
/** SPR 14758 Changes End **/

typedef struct lteCircQueue_t {
	int head;
	int tail;
	int count;
	int max; /* should always be 2^n - 1 */
	LTE_CNODE *ring;
} LTE_CIRCQUEUE;

/* SPR 23483 Changes Start */
#define SUCCESS 1
#define FAILURE 0
#define CIRC_SUCCESS 1
#define CIRC_FAILURE 0
#define CIRC_PARTIAL_FAILURE 2
#define FILE_OPEN_ERROR 0
/* SPR 23483 Changes End */

#ifdef UE_SIM_TESTING
#define MAX_NODES 1024
#else
#define MAX_NODES 512
#define MAX_NODES_SEG_PDU_IND 10
#define MAX_NODES_TO_DISCARD 4096
#endif

/****************************************************************************/
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#if ((defined CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
#define circQInit(list, maxNodes) _circQInit(list, maxNodes, __func__, __LINE__)
UInt32 _circQInit(LTE_CIRCQUEUE * list, UInt32 maxNodes, UInt8 * func, UInt32 * line);
#else
UInt32 circQInit(LTE_CIRCQUEUE * list, UInt32 maxNodes);
#endif

UInt32 circQInitAfterCellSetup(LTE_CIRCQUEUE * list, UInt32 maxNodes);

UInt32 pushNodeCircQ(LTE_CIRCQUEUE *queue_p, void *data);
UInt32 popNodeCircQ(LTE_CIRCQUEUE *queuue_p, void **outData);
UInt32 circQueueCount(LTE_CIRCQUEUE *list);
UInt32 delNodeCircQ(LTE_CIRCQUEUE *list, UInt32 index);
UInt32 circQDeInit(LTE_CIRCQUEUE *list);
SInt32 qLinearSearch(LTE_CIRCQUEUE *list, UInt32 (*compare)(void *, void*), void *arg);
/* API Indroduced due to EMBMS*/
UInt32 circQReInit(LTE_CIRCQUEUE *list, UInt32 maxNodes);
UInt32 getNodeCircQ(LTE_CIRCQUEUE *list, void **outData);
UInt32 getNextNodeCircQ(LTE_CIRCQUEUE *list, void **outData, UInt32 *index);
/****************************************************************************/

#endif
