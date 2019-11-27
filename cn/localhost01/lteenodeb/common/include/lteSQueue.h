/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteSQueue.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Header file for non buffered Queue 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteSQueue.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.10  2009/04/10 17:22:38  gur18569
 * merged with optmization changes
 *
 * Revision 1.9  2009/03/02 09:10:54  gur19140
 * Changes reqarding Zero copy,pdcp discard and execution legs.
 *
 * Revision 1.8  2008/12/29 10:07:01  gur19836
 * Changed algo in sQueueCount for optimization
 *
 * Revision 1.7  2008/09/18 05:25:46  gur19836
 * Review comments incorporated
 *
 * Revision 1.6  2008/09/09 08:34:23  gur19836
 * File Description Added
 *
 * Revision 1.5  2008/09/09 07:27:13  gur19836
 * Added Comments
 *
 * Revision 1.4  2008/09/09 06:47:46  ssinghal
 * Added comments and removed tabs from the file
 *
 * Revision 1.3  2008/09/08 10:33:43  gur19836
 * API provided to access first node in Queue
 *
 * Revision 1.2  2008/09/05 08:55:42  gur19836
 * Semaphore typedef added
 *
 * Revision 1.1  2008/09/04 08:33:03  gur19836
 * lteSQueue.h Added
 * 
 *
 *
 ****************************************************************************/


#ifndef _LTE_SQUEUE_
#define _LTE_SQUEUE_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include    <stdio.h>
#include    "lteTypes.h"
#include    "lteSync.h"
#include    "lteLog.h"

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
#define MAX_SIZE 0xFFFFFFFF

#define NUM_SEM 1

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
typedef struct lteSNode_t {
    struct lteSNode_t *next;
} LTE_SNODE;

typedef struct lteSQueue_t {
/** SPR 14789 Changes Start **/
#ifdef UE_SIM_TESTING
    void  *padding1;
    void  *padding2;
#endif
/** SPR 14789 Changes End **/
    volatile void *head;
    volatile void *tail;
    volatile UInt64 headcount;
    volatile UInt64 tailcount;
    LTE_SEM sem_lock;
    /*SPR_7217_changes_start*/
    //LTE_SEM sem_popLock;
    /*SPR_7217_changes_end*/
} LTE_SQUEUE;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 *  function(): Description.
 *      This function performs the initialization of the Queue whose pointer
 *      is passed as an arguement to it.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    sQueueInit(LTE_SQUEUE *queue_p);
void    sQueueDeInit(LTE_SQUEUE *queue_p);


/****************************************************************************
 *  function(): Description.
 *      This function counts the number of nodes present in Queue whose
 *      pointer is passed as an arguement.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
UInt64  sQueueCount(const LTE_SQUEUE *queue_p);


/****************************************************************************
 *  function(): Description.
 *      This function prints all the nodes present in the Queue sequentially.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void printSQueue(LTE_SQUEUE *queue_p);


/****************************************************************************
 *  function(): Description.
 *      This function deletes 1 node from the Queue and returns it to the
 *      calling function.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_SNODE *popNode(LTE_SQUEUE *queue_p);


/****************************************************************************
 *  function(): Description.
 *      This function adds the node in the Queue whose pointer is passed as
 *      an arguement
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
UInt8 pushNode(LTE_SQUEUE *queue_p, LTE_SNODE *node_p);


/****************************************************************************
 *  function(): Description.
 *      This function returns the pointer to first node in the queue to
 *      the calling function.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_SNODE *getFirstNode(LTE_SQUEUE *queue_p);


/****************************************************************************
 *  function(): Description.
 *      This function returns the pointer to next node in the queue to
 *      the calling function.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_SNODE *getNextNode(const LTE_SNODE *node_p);
#endif
