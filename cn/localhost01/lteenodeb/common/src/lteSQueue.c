/***************************************************************************
*
*  ARICENT -
*
*  Copyright (c) Aricent.
*
****************************************************************************
*
*  $Id: lteSQueue.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
*
****************************************************************************
*
*  File Description : This file contains non bounded buffered Queue
*                     implementation. 
*
****************************************************************************
*
* Revision Details
* ----------------
* $Log: lteSQueue.c,v $
* Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
* eNB framework for intel
*
* Revision 1.14.2.1  2009/06/19 13:41:16  gur19140
* *** empty log message ***
*
* Revision 1.14  2009/04/10 17:22:38  gur18569
* merged with optmization changes
*
* Revision 1.13  2009/04/10 11:51:37  gur19836
* 100ue fixes
*
* Revision 1.12  2009/03/25 06:33:47  gur20439
* LTE_LOG macro called modified as per the new macro.
*
* Revision 1.11  2009/03/02 09:10:54  gur19140
* Changes reqarding Zero copy,pdcp discard and execution legs.
*
* Revision 1.10  2009/01/20 16:29:12  gur11912
* Updated for Porting
*
* Revision 1.9  2009/01/12 09:44:16  gur19836
* Log removed in getFirstNode API
*
* Revision 1.8  2008/12/29 10:08:13  gur19836
* Changed algo in sQueueCount for optimization
*
* Revision 1.7  2008/12/24 05:18:21  gur19836
* Removed compilation warning in sQueueCount
*
* Revision 1.6  2008/12/11 09:57:52  gur12905
* Unnecessary Logs Commented
*
* Revision 1.5  2008/10/14 05:30:06  gur11912
* Fixed for Race condition
*
* Revision 1.4  2008/09/18 05:25:08  gur19836
* Review comments incorporated
*
* Revision 1.3  2008/09/09 08:34:23  gur19836
* File Description Added
*
* Revision 1.2  2008/09/08 10:33:43  gur19836
* API provided to access first node in Queue
*
* Revision 1.1  2008/09/04 08:34:56  gur19836
* lteSQueue.c Added
*
*
*
****************************************************************************/


/****************************************************************************
* Standard Library Includes
****************************************************************************/
#include "lteSQueue.h"

/****************************************************************************
* Project Includes
****************************************************************************/

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
#ifdef MAC_AUT_TEST
extern UInt8 macAutFlag;
#endif

/****************************************************************************
* Private Variables (Must be declared static)
****************************************************************************/

#ifdef MAC_AUT_TEST
UInt8 pushNodeFailure_g = 0;
#define CHECK_AUT_CONFIG_FOR_PUSH_NODE()\
    if (macAutFlag)\
    {\
        if (pushNodeFailure_g == 1)\
        {\
            return 1;\
        }\
    }
#endif

/****************************************************************************
* Function Name  : sQueueInit
* Inputs         : queue_p : Pointer to the Queue to be initialized
* Outputs        : Queue Initialization
* Returns        : None
* Variables      : 
* Description    : This function performs the initialization of the Queue
                   whose pointer is passed as an arguement to it.
****************************************************************************/

void sQueueInit(LTE_SQUEUE *queue_p)
{
    if(queue_p != PNULL) {
        queue_p->head = queue_p->tail = PNULL;
        queue_p->headcount = queue_p->tailcount = 0;
        semInit(&(queue_p->sem_lock), NUM_SEM);
        /*SPR_7217_changes_start*/
        // semInit(&(queue_p->sem_popLock), NUM_SEM);
        /*SPR_7217_changes_end*/
        LTE_LOG(LOG_INFO,PNULL,"In Fn %s, LTE_SQUEUE Initialized\n", __func__);
    }
    else {
        LTE_LOG(LOG_MAJOR,PNULL,"In Fn %s, LTE_SQUEUE* passed is NULL\n", __func__);
    }        
}

/****************************************************************************
* Function Name  : sQueueCount
* Inputs         : queue_p - Pointer to the Queue
* Outputs        : None
* Returns        : UINT64 count - Number of nodes present in the Queue
* Variables      : 
* Description    : This function counts the number of nodes present in Queue 
                   whose pointer is passed as an arguement.
****************************************************************************/

UInt64 sQueueCount(const LTE_SQUEUE *queue_p)
{
    UInt64 count = 0;

    if(queue_p != PNULL) {
        /* Case when wrap around happen at tail and it wraps to 0 and start
           again but head is still not reached max value*/
        if(queue_p->headcount > queue_p->tailcount)
        {
            count = ((MAX_SIZE - queue_p->headcount) + queue_p->tailcount + 1);
        }
        else
        {
            count = (queue_p->tailcount - queue_p->headcount);
        }
        /* This is the case when it is called from within pushNode or popNode */
      count -= (count & 0x1);
      return (count>>1);
      }
    else {
        //LTE_LOG(LOG_MAJOR,PNULL,"In Fn %s, LTE_SQUEUE* passed is NULL\n", __func__);
    }        
    return count;
}    


/****************************************************************************
* Function Name  : printSQueue
* Inputs         : queue_p - Pointer to the Queue
* Outputs        : Prints the details of all the nodes present in the Queue
* Returns        : None
* Variables      : 
* Description    : This function prints all the nodes present in the Queue
                   sequentially. 
****************************************************************************/

void printSQueue(LTE_SQUEUE *queue_p)
{
    UInt32 x = 1;
    LTE_SNODE *node_p = PNULL;

    if (queue_p != PNULL) {
         
        LTE_LOG(LOG_INFO,PNULL,"No of nodes in Queue = %d\n", sQueueCount(queue_p)); 
        LTE_LOG(LOG_INFO,PNULL,"head = %p, tail = %p\n",queue_p->head,
                                                  queue_p->tail);
        node_p = (LTE_SNODE *)queue_p->head; /*Icc-Warn*/
        while(node_p)
        {
            LTE_LOG(LOG_INFO,PNULL,"Node No = %d\t", x); 
            LTE_LOG(LOG_INFO,PNULL,"Address of Node = %p\n", node_p); 
            node_p = node_p->next;
            x++;
        }
    }
    else {
        LTE_LOG(LOG_MAJOR,PNULL,"In Fn %s, LTE_SQUEUE* passed is NULL\n",__func__);
    }        
}


/****************************************************************************
* Function Name  : pushNode
* Inputs         : queue_p - Pointer to the Queue where node is to be inserted
                   node_p  - Pointer to the node to be inserted
* Outputs        : Node inserted at the end of the Queue
* Returns        : 1 on failure, 0 on success
* Variables      :
* Description    : This function adds the node in the Queue whose pointer 
                   is passed as an arguement
****************************************************************************/
    
UInt8 pushNode(LTE_SQUEUE *queue_p, LTE_SNODE *node_p)
{
#ifdef MAC_AUT_TEST
    if (macAutFlag)
    {
    CHECK_AUT_CONFIG_FOR_PUSH_NODE();
    }
#endif
    SInt32 value=0;

    if((queue_p == PNULL) || (node_p == PNULL))
    {
        LTE_LOG(LOG_MAJOR,PNULL,"In Fn %s, LTE_SQUEUE* passed is NULL or ",__func__);
        LTE_LOG(LOG_MAJOR,PNULL,"LTE_SNODE* passed is NULL\n");
        LTE_LOG(LOG_MAJOR,PNULL,"queue_p = %p, node_p = %p\n", queue_p, node_p);
        /*+++530-coverity-55019*/
        /*code removed*/
        /*+++530-coverity-55019*/
        return 1;
    }
   
    /* Coverity CID 54673 start */
    node_p->next = NULL;
    queue_p->tailcount++;
    /* Coverity CID 54673 end */
    /* tailcount will be incremented twice during one pushNode operation to
      minimize the use of semaphores. Let us suppose there were multiple
      nodes in the queue & one more node is getting added.But while pushing, 
      pop thread got scheduled and start popping the nodes such that only 
      1 node left at which pushNode is adding a node. Now in normal case as
      pushNode has not taken sem (as head!=tail at that time), so popNode
      will take sem and will delete the last node thus causing system to crash.
      But here, in popNode to avoid this error, tailcount will be checked to
      see if pushNode is in progress when there is only 1 node. If so, 
      popNode will return NULL to calling function and can pop the node only 
      after pushNode completes its operation */

    /* Case : Queue is empty or contains only 1 node */
        if ((queue_p->head == queue_p->tail) || (queue_p->tail == PNULL) || (sQueueCount(queue_p)<=2))
        {
            semWait(&(queue_p->sem_lock));
            /* Case : When the Queue is empty initially */
            if (queue_p->head == PNULL)
            {
                if (queue_p->tail != PNULL)
                {
                    printSQueue(queue_p);
                    ltePanic("In pushNode : queue_p->head == NULL but queue_p->tail != NULL\n");
                }

                queue_p->tail = node_p;
                queue_p->head = queue_p->tail;
            }
            /* Case : When the Queue contains only 1 node */
            else
            {
                if (queue_p->tail == PNULL)
                {
                    if (queue_p->head != PNULL)
                    {
                        printSQueue(queue_p);
                        ltePanic("In pushNode : queue_p->tail == NULL but queue_p->head != NULL\n");
                    }
                }
                if (queue_p->tail == NULL)
                {
                    printSQueue(queue_p);
                    ltePanic("Tail is NULL in PushNode 1\n");
                }
                else
                {
                    ((LTE_SNODE *)(queue_p->tail))->next = node_p;
                    queue_p->tail = node_p;
                }
            }

            queue_p->tailcount++;
            semPost(&(queue_p->sem_lock));
        }
        /* Case : More than 1 node are present in the Queue */
        else
        {
            if (queue_p->tail == NULL)
            {
                printSQueue(queue_p);
                ltePanic("Tail is NULL in PushNode 2 value %d\n", value);
            }
            ((LTE_SNODE *)(queue_p->tail))->next = node_p;
            queue_p->tail = node_p;
           if ( queue_p->head == PNULL &&  queue_p->tail != PNULL)
            {
                printSQueue(queue_p);
                ltePanic ("Some Problem here\n");
            }
            queue_p->tailcount++;
        }
    
//    LTE_LOG(LOG_INFO,PNULL,"Node added at the bottom of the queue\n");
    return 0;
}


/****************************************************************************
* Function Name  : popNode
* Inputs         : queue_p - Pointer to the Queue 
* Outputs        : Deletes a node from the head of the Queue
* Returns        : LTE_SNODE * - Pointer to the node popped from Queue
* Variables      :
* Description    : This function deletes 1 node from the Queue and returns it
                   to the calling function.
****************************************************************************/

LTE_SNODE *popNode(LTE_SQUEUE *queue_p)
{
    LTE_SNODE *node_p = PNULL;
    //SInt32 value = 0;
    
    if((queue_p != PNULL) && (queue_p->head != PNULL))
    {
        /* Case : Comes here if Queue contains atleast 1 node */

        /* Case : Only 1 node present in the queue */
        if ((queue_p->head == queue_p->tail) || (sQueueCount(queue_p) <= 1))
        {
          queue_p->headcount++;
          /* Case : Check if operation is going on by push mode on this node */
          /* SPR 18122 Changes Start*/
            if(MODULO(queue_p->tailcount,2) != 0)
          /* SPR 18122 Changes End*/
            {
                queue_p->headcount--;
                LTE_LOG(LOG_MAJOR,PNULL,
                 "In Fn %s, only 1 node in Queue and push op is in process\n",
                 __func__);
                return PNULL;
            }
            semWait(&(queue_p->sem_lock));
            /* Case : When pop in progress and push request comes and executes
                      before pop going further */
            if(queue_p->head == queue_p->tail)
            {
                node_p = (LTE_SNODE *)queue_p->head;/*Icc-Warn*/
                queue_p->tail = PNULL;
                queue_p->head = queue_p->tail;
            }
            else
            {
                node_p = (LTE_SNODE *)queue_p->head;/*Icc-Warn*/
                queue_p->head = node_p->next;
                if (queue_p->head == PNULL)
                {
                    printSQueue(queue_p);
                    ltePanic("Head got NULL 2 %p %p %p\n",node_p,node_p->next,queue_p->tail);
                }
            }
            if ( (queue_p->head == PNULL) &&  (queue_p->tail != PNULL) )
            {
                printSQueue(queue_p);
                lteWarning ("Problem starts here\n");
            }
            semPost(&(queue_p->sem_lock));
            queue_p->headcount++;
        }
        /* Case : Queue contains more than 1 node */
        else
        {
           queue_p->headcount++;
            if (queue_p->head == PNULL)
            {
                printSQueue(queue_p);
                ltePanic("Head got NULL before %p %p %p\n",node_p,node_p->next,queue_p->tail);
            }
            node_p = (LTE_SNODE *)queue_p->head;/*Icc-Warn*/
            queue_p->head = node_p->next;
            queue_p->headcount++;
            if (queue_p->head == PNULL)
            {
                printSQueue(queue_p);
                ltePanic("Head got NULL after %p %p %p\n",node_p,node_p->next,queue_p->tail);
            }
        }
//        LTE_LOG(LOG_INFO,PNULL,"Node popped from queue, node_p = %p\n", node_p);
    }
    else {
        //LTE_LOG(LOG_MAJOR,PNULL,"In Fn %s, LTE_SQUEUE* passed is NULL or ", __func__);
        //LTE_LOG(LOG_MAJOR,PNULL,"Queue is empty if head is NULL\n");
        //LTE_LOG(LOG_MAJOR,PNULL,"queue_p = %p, head = %p\n", queue_p, queue_p->head);
    }
    return node_p;
}


/****************************************************************************
* Function Name  : getFirstNode
* Inputs         : queue_p - Pointer to the Queue 
* Outputs        : None
* Returns        : LTE_SNODE * - Pointer to the first node in Queue
* Variables      :
* Description    : This function returns the pointer to first node in the
                   queue to the calling function.
****************************************************************************/
LTE_SNODE *getFirstNode(LTE_SQUEUE *queue_p)
{
    if(queue_p != PNULL) {
        return ((LTE_SNODE*)queue_p->head);/*Icc-Warn*/
    }
    else {
        LTE_LOG(LOG_MAJOR,PNULL,"In %s fn, LTE_SQUEUE* passed is NULL\n", __func__);
        return PNULL;
    }        
}


/****************************************************************************
* Function Name  : getNextNode
* Inputs         : node_p - Pointer to the LTE_SNODE 
* Outputs        : None
* Returns        : LTE_SNODE * - Pointer to the next node in Queue
* Variables      :
* Description    : This function returns the pointer to next node in the
                   queue to the calling function. 
****************************************************************************/
LTE_SNODE *getNextNode(const LTE_SNODE *node_p)
{
    if(node_p != PNULL) {
        LTE_LOG(LOG_INFO,PNULL,"Address of returned next node=%p\n",(node_p->next));
        return (node_p->next);
    }        
    else {
        LTE_LOG(LOG_MAJOR,PNULL,"In %s fn, LTE_SNODE* passed is NULL\n", __func__);
        return PNULL;
    }        
}
