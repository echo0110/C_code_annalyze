/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteQueue.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:45:39 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Wrapper implementation of queue functionality.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteQueue.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:45:39  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.7  2009/03/02 09:10:54  gur19140
 * Changes reqarding Zero copy,pdcp discard and execution legs.
 *
 * Revision 1.6  2009/01/13 06:19:22  gur20294
 * added new apis
 *
 * Revision 1.5  2009/01/12 17:08:44  gur20294
 * added some method
 *
 * Revision 1.4  2008/09/18 05:25:08  gur19836
 * Review comments incorporated
 *
 * Revision 1.3  2008/09/09 11:11:53  ssinghal
 * comments added to functions
 *
 * Revision 1.2  2008/08/25 10:05:03  ssinghal
 * Template applied and minor changes done
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include    "lteQueue.h"


/****************************************************************************
  Private Definitions
 ****************************************************************************/
/*SPR_7217_changes_start*/
#define NUM_SEM_QUEUE 1
/*SPR_7217_changes_end*/


/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Function Name  : queueInit 
 * Inputs         : pList - Pointer to the LTE_QUEUE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function initialises the queue.
 ****************************************************************************/
void    queueInit(LTE_QUEUE *pList)
{
    if (pList != PNULL)
    {
        ylInit(pList);
        /*SPR_7217_changes_start*/
        semInit(&(pList->sem_lock), NUM_SEM_QUEUE);
        /*SPR_7217_changes_end*/
    }
    return;
}    


/****************************************************************************
 * Function Name  : queueCount 
 * Inputs         : pList - Pointer to the LTE_QUEUE object.
 * Outputs        : None.
 * Returns        : Number of elements in  the queue.
 * Variables      : None.
 * Description    : This function returns the number of elements in queue.
 ****************************************************************************/
UInt32  queueCount(const LTE_QUEUE *pList)
{
    if (pList != PNULL)
    {
        return ylCount(pList);
    }
    return 0;
}


/****************************************************************************
 * Function Name  : getQueueNode 
 * Inputs         : pList - Pointer to the LTE_QUEUE object.
 * Outputs        : None.
 * Returns        : The front element in the queue.
 * Variables      : None.
 * Description    : This function returns the front element in the queue.
 *                  Note that the element is not deleted from the queue.
 ****************************************************************************/
LTE_QUEUE_NODE  *getQueueNode(LTE_QUEUE *pList) /* gcc 5.0 */
{
    LTE_QUEUE_NODE *node_p;
    if (pList != PNULL)
    {
        semWait(&(pList->sem_lock));
        node_p = ylFirst(pList);
        semPost(&(pList->sem_lock));
        return node_p;
    }
    return PNULL;
}


/****************************************************************************
 * Function Name  : enQueue 
 * Inputs         : pList - Pointer to the LTE_QUEUE object.
 *                  pNode - Pointer to the LTE_QUEUE_NODE object.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function inserts an element in the queue.
 ****************************************************************************/
void    enQueue(LTE_QUEUE *pList, LTE_QUEUE_NODE *pNode)
{
    if ((pList != PNULL) && (pNode != PNULL))
    {
        semWait(&(pList->sem_lock));
        ylPushTail(pList, pNode);
        semPost(&(pList->sem_lock));
    }
    return;
}    


/****************************************************************************
 * Function Name  : deQueue 
 * Inputs         : pList - Pointer to the LTE_QUEUE object.
 * Outputs        : None.
 * Returns        : The front popped element.
 * Variables      : None.
 * Description    : This function popps the front element from the queue.
 ****************************************************************************/
LTE_QUEUE_NODE  *deQueue(LTE_QUEUE *pList)
{
    LTE_QUEUE_NODE *node_p = PNULL;
    if (pList != PNULL)
    {
        semWait(&(pList->sem_lock));
        node_p = ylPopHead(pList);
        semPost(&(pList->sem_lock));
        return(node_p);
    }
    return PNULL;

}
/****************************************************************************
 * Function Name  : queueInsertBefore 
 * Inputs         : pList - Pointer to the LTE_QUEUE object. pNext Node and pNode
 * Outputs        : None.
 * Returns        : The front popped element.
 * Variables      : None.
 * Description    : This function add next node element for given node in queue.
 ****************************************************************************/
void queueInsertBefore(YLIST *pList, YLNODE *pNext, YLNODE *pNode)
{
    if( pList != PNULL ) {
        ylInsertBefore(pList, pNext, pNode);
    } 
}


/****************************************************************************
 * Function Name  : queueInsertAfter 
 * Inputs         : pList - Pointer to the LTE_QUEUE object. pPrev Node and pNode
 * Outputs        : None.
 * Returns        : The front popped element.
 * Variables      : None.
 * Description    : This function add next node element for given node in queue.
 ****************************************************************************/
void queueInsertAfter(YLIST *pList, YLNODE *pPrev, YLNODE *pNode)
{
    if( pList != PNULL ) {
        ylInsertAfter(pList, pPrev, pNode);
    } 
}

/****************************************************************************
 * Function Name  : delQueueNode 
 * Inputs         : pList - Pointer to the LTE_QUEUE object.
 * Outputs        : None.
 * Returns        : The front popped element.
 * Variables      : None.
 * Description    : This function delete node element from the queue.
 ****************************************************************************/
void delQueueNode(LTE_QUEUE *pList,LTE_QUEUE_NODE *pNode)
{
    if (pList != PNULL && PNULL != pNode)
    {
     /* SPR 4010 Fix Start */
        semWait(&(pList->sem_lock));
        ylDelete( pList,pNode);
        semPost(&(pList->sem_lock));
     /* SPR 4010 Fix End */
    }
}

/****************************************************************************
 * Function Name  : getNextQueueNode 
 * Inputs         : pNode - Pointer to the LTE_QUEUE_NODE object.
 * Outputs        : None.
 * Returns        : The Next node of given node.
 * Variables      : None.
 * Description    : This function return the next node of given node.
 ****************************************************************************/
LTE_QUEUE_NODE  *getNextQueueNode(LTE_QUEUE *pList, LTE_QUEUE_NODE *pNode)
{
    LTE_QUEUE_NODE *node_p = PNULL;
    if (pNode != PNULL && pList != PNULL)
    {
        semWait(&(pList->sem_lock));
        node_p = ylNext(pNode);
        semPost(&(pList->sem_lock));
        return node_p;
    }
    return PNULL;
}
/****************************************************************************
 * Function Name  : getPrevQueueNode 
 * Inputs         : pNode - Pointer to the LTE_QUEUE_NODE object.
 * Outputs        : None.
 * Returns        : The Previous node of given node.
 * Variables      : None.
 * Description    : This function return the previous node of given node.
 ****************************************************************************/
LTE_QUEUE_NODE  *getPrevQueueNode(LTE_QUEUE_NODE *pNode)
{
    if (pNode != PNULL)
    {
        return ylPrev(pNode);
    }
    return PNULL;
}
/****************************************************************************
 * Function Name  : getFirstQueueNode 
 * Inputs         : pList - Pointer to the LTE_LIST object.
 * Outputs        : None.
 * Returns        : The first node in the list.
 * Variables      : None.
 * Description    : Returns the first node in the list. Note that the 
 *                  node is not removed from the list.
 ****************************************************************************/
LTE_QUEUE_NODE *getFirstQueueNode(const LTE_QUEUE *pList)
{
    if (pList != PNULL)
    {
        return ylFirst(pList);
    }
    return PNULL;
}
/****************************************************************************
 * Function Name  : getLastQueueNode 
 * Inputs         : pList - Pointer to the LTE_LIST object.
 * Outputs        : None.
 * Returns        : The last node in the list.
 * Variables      : None.
 * Description    : Returns the last node in the list. Note that the 
 *                  node is not removed from the list.
 ****************************************************************************/
LTE_QUEUE_NODE *getLastQueueNode(const LTE_QUEUE *pList)
{
    if (pList != PNULL)
    {
        return ylLast(pList);
    }
    return PNULL;
}

