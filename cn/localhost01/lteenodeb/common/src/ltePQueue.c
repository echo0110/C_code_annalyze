/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: ltePQueue.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Wrapper implementation of priority queue. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePQueue.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
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
#include    "ltePQueue.h"


/****************************************************************************
  Private Definitions
 ****************************************************************************/



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
 * Function Name  : pqInit 
 * Inputs         : pQueue - pointer to the LTE_PQUEUE structure.
 *                  priority - callback function for deciding the priority.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Initialises the priority queue.
 ****************************************************************************/
void pqInit(LTE_PQUEUE *pQueue, SInt32 (*priority)(const LTE_PQUEUE_NODE *))
{
    if (pQueue != PNULL)
    {
        ysInit(pQueue, priority);
    }
    return;
}


/****************************************************************************
 * Function Name  : pqCount 
 * Inputs         : pQueue - pointer to the LTE_PQUEUE structure.
 * Outputs        : None.
 * Returns        : Number of nodes in the prority queue.
 * Variables      : None.
 * Description    : This function returns the number of nodes in the 
 *                  priority queue.
 ****************************************************************************/
UInt32 pqCount(const LTE_PQUEUE *pQueue)
{
    if (pQueue != PNULL)
    {
        return ysCount(pQueue);
    }
    return 0;
}


/****************************************************************************
 * Function Name  : pqPushTail 
 * Inputs         : pQueue - pointer to the LTE_PQUEUE structure.
 *                  pNode - pointer to the LTE_PQUEUE_NODE structure.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function pushes an element in the priority queue.
 ****************************************************************************/
void pqPushTail(LTE_PQUEUE *pQueue, LTE_PQUEUE_NODE *pNode)
{
    if ((pQueue != PNULL) && (pNode != PNULL))
    {
        ysPushTail(pQueue, pNode);
    }
    return;
}


/****************************************************************************
 * Function Name  : pqPopHead 
 * Inputs         : pQueue - pointer to the LTE_PQUEUE structure.
 * Outputs        : None.
 * Returns        : The node that is popped.
 * Variables      : None.
 * Description    : This function popps an element from the head of the 
 *                  priority queue.
 ****************************************************************************/
LTE_PQUEUE_NODE  *pqPopHead(LTE_PQUEUE *pQueue)
{
    if (pQueue != PNULL)
    {
        return ysPopHead(pQueue);
    }
    return PNULL;
}
