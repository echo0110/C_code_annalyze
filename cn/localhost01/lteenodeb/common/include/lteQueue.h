/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteQueue.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:45:38 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteQueue.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:45:38  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.7  2009/03/02 09:10:54  gur19140
 * Changes reqarding Zero copy,pdcp discard and execution legs.
 *
 * Revision 1.6  2009/01/13 06:23:33  gur11912
 * Modified LTE_LIST_NODE to LTE_QUEUE_NODE
 *
 * Revision 1.5  2009/01/12 18:02:26  gur20294
 * added new APIs
 *
 * Revision 1.4  2008/09/09 06:47:46  ssinghal
 * Added comments and removed tabs from the file
 *
 * Revision 1.3  2008/09/08 06:06:01  ssinghal
 * extra _ removed from _LTE_QUEUE_
 *
 * Revision 1.2  2008/08/25 10:04:35  ssinghal
 * Template applied and minor changes done
 *
 *
 *
 ****************************************************************************/


#ifndef _LTE_QUEUE_
#define _LTE_QUEUE_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include    "ylib.h"
#include    "lteTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* This is needed to create a user defined queue. An object of this needs to 
   be passed to the intialization function of the queue. */
typedef YLIST LTE_QUEUE;

/* This is the anchor of the entry to be made in the queue. This object 
   is part of every node that is created to be inserted into a queue. */
typedef YLNODE LTE_QUEUE_NODE;


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 *  function(): Description.
 *      This API initialises the queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    queueInit(LTE_QUEUE *pList);

/****************************************************************************
 *  function(): Description.
 *      This API initialises the queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    delQueueNode(LTE_QUEUE *pList,LTE_QUEUE_NODE *pNode);


/****************************************************************************
 *  function(): Description.
 *      This API counts the element in the queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
UInt32  queueCount(const LTE_QUEUE *pList);


/****************************************************************************
 *  function(): Description.
 *      This API returns the element at the top of the queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_QUEUE_NODE  *getQueueNode(LTE_QUEUE *pList); /* gcc 5.0 */


/****************************************************************************
 *  function(): Description.
 *      This API pushes one element in the queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    enQueue(LTE_QUEUE *pList, LTE_QUEUE_NODE *pNode);


/****************************************************************************
 *  function(): Description.
 *      This API removes the element from the queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_QUEUE_NODE  *deQueue(LTE_QUEUE *pList);
/****************************************************************************
 *  function(): Description.
 *      This API return the last element of the queue.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_QUEUE_NODE *getLastQueueNode(const LTE_QUEUE *pList);
/****************************************************************************
 *  function(): Description.
 *      This API returns prev element node of given node.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_QUEUE_NODE  *getPrevQueueNode(LTE_QUEUE_NODE *pNode);
/****************************************************************************
 *  function(): Description.
 *      This API retunrs next element node of given node.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
LTE_QUEUE_NODE  *getNextQueueNode(LTE_QUEUE *pList, LTE_QUEUE_NODE *pNode);

/****************************************************************************
 * Function Name  : queueInsertBefore
 * Inputs         : pList - Pointer to the LTE_QUEUE object. pNext Node and pNode
 * Outputs        : None.
 * Returns        : The front popped element.
 * Variables      : None.
 * Description    : This function add next node element for given node in queue.
 *****************************************************************************/
void queueInsertBefore(YLIST *pList, YLNODE *pNext, YLNODE *pNode);

/****************************************************************************
 * Function Name  : queueInsertAfter 
 * Inputs         : pList - Pointer to the LTE_QUEUE object. pNext Node and pNode
 * Outputs        : None.
 * Returns        : The front popped element.
 * Variables      : None.
 * Description    : This function add next node element for given node in queue.
 *****************************************************************************/
void queueInsertAfter(YLIST *pList, YLNODE *pNext, YLNODE *pNode);

/****************************************************************************
 * Function Name  : getFirstQueueNode 
 * Inputs         : pList - Pointer to the LTE_LIST object.
 * Outputs        : None.
 * Returns        : The first node in the list.
 * Variables      : None.
 * Description    : Returns the first node in the list. Note that the 
 *                  node is not removed from the list.
 ****************************************************************************/
LTE_QUEUE_NODE *getFirstQueueNode(const LTE_QUEUE *pList);

#endif
