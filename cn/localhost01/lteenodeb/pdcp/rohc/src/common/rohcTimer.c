/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohcTimer.c,v 1.1.1.1.16.1 2010/11/03 06:49:09 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the implementation of ROHC Timers 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohcTimer.c,v $
 * Revision 1.1.1.1.16.1  2010/11/03 06:49:09  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.6.36.2  2010/09/10 10:38:54  gur22059
 * SPR 656: Updated for Memory Leak.
 *
 * Revision 1.6  2009/05/28 13:49:13  gur19140
 * *** empty log message ***
 *
 * Revision 1.5  2009/05/27 07:15:32  gur19836
 * File Headers and Function Headers updated
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteFramework.h"
#include "rohcTimer.h"
#include "rohc_ex.h"
#include "rohc_trace_mcr.h"
#include "alarm.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/
typedef struct rohcTimerReqT {
    LTE_LIST_NODE   lNode;
    /* SPR 15909 fix start */
    tickType_t      expiredTime;
    /* SPR 15909 fix end */
    LTE_LIST*       list_p;
    void*           node;
} rohcTimerReq;

typedef struct RohcTimerQT {
    LTE_LIST *pQueue ;
}RohcTimerQ;

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* + SPR 8907 fix */
static void rohcProcessTimerList(RohcTimerQ* rohcQueue_p, rohc_U8bit_t timerType);
/* - SPR 8907 fix */
static void expireRohcTimer(LTE_LIST_NODE* node_p);
static RohcTimerQ* getListTimer(UInt16 timer);
static void queueDeInitRohcTimerQ(RohcTimerQ* pL);
static rohcTimerReq* deQueueRohcTimerQ(RohcTimerQ* pL);

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
static RohcTimerQ   lRohcIRTimer;
static RohcTimerQ   lRohcFOTimer;

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
* Function Name  : queueInitRohcTimerQ
* Inputs         : pL - a pointer to RohcTimerQ
* Outputs        :
* Returns        : void
* Variables      :
* Description    : initializes ROHC timer queue .
****************************************************************************/
void queueInitRohcTimerQ(RohcTimerQ *pL)
{
    if((pL->pQueue = (LTE_LIST*)getMemFromPool(sizeof(LTE_LIST) ,PNULL))== PNULL){
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("Memory Allocation Failure");
    }
    listInit(pL->pQueue);
}

/****************************************************************************
* Function Name  : queueDeInitRohcTimerQ
* Inputs         : pL - a pointer to RohcTimerQ
* Outputs        :
* Returns        : void
* Variables      :
* Description    : deinitializes ROHC timer queue .
****************************************************************************/
static void queueDeInitRohcTimerQ(RohcTimerQ* pL)
{
    rohcTimerReq* pTimerP = PNULL ;
    while((pTimerP = deQueueRohcTimerQ(pL)) != PNULL ){
        freeMemPool(pTimerP);
        pTimerP = PNULL;
    }
    freeMemPool(pL->pQueue);
    pL->pQueue = PNULL;
}

/****************************************************************************
* Function Name  : deQueueRohcTimerQ
* Inputs         : pL - a pointer to RohcTimerQ
* Outputs        :
* Returns        : a valid pointer to RohcTimer or PNULL for empty queue
* Variables      :
* Description    : pops first element from queue @pL .
****************************************************************************/
static rohcTimerReq*  deQueueRohcTimerQ(RohcTimerQ* pL)
{
    UInt32 numTimerNodes = 0;
    LTE_LIST_NODE *pN = PNULL ;
    rohcTimerReq *timerReq = PNULL;
    
    numTimerNodes = listCount(pL->pQueue);
    if(numTimerNodes > 0)
    {
        pN = getListNode(pL->pQueue,0) ;
        listDeleteNode(pL->pQueue,pN);
    }

    timerReq = (rohcTimerReq*)pN ;
    return timerReq;
}

/****************************************************************************
 * Function Name  : initRohcTimer  
 * Inputs         : RohcTimerCallback_func func
 * Outputs        : None.
 * Returns        : None.
* Description    : Inits all timer queues and callback
  ****************************************************************************/
/* + SPR 17439 */
void initRohcTimer(void)
/* - SPR 17439 */    
{
    queueInitRohcTimerQ(&lRohcIRTimer);
    queueInitRohcTimerQ(&lRohcFOTimer);
}

/****************************************************************************
 * Function Name  : cleanRohcTimer  
 * Inputs         : None
 * Outputs        : None.
 * Returns        : None.
 * Description    : Cleans all timer queues
  ****************************************************************************/
void cleanRohcTimer(void)
{
    queueDeInitRohcTimerQ(&lRohcIRTimer);
    queueDeInitRohcTimerQ(&lRohcFOTimer);
}

/****************************************************************************
 * Function Name  : startRohcTimer  
 * Inputs         : UInt16 timeout - timeout for message
 *                  timerType - ROHC timer type
 *                  void* node_p - pointer to node
 * Outputs        : LTE_LIST_NODE*
 * Returns        : LTE_LIST_NODE*
 * Description    : Push message to timer queue
  ****************************************************************************/
LTE_LIST_NODE* startRohcTimer(UInt16 timeout, UInt16 timerType, void* node_p)
{
    rohcTimerReq *timeReq = PNULL;
    LTE_LIST* p_list = PNULL;
    RohcTimerQ*  rohcQueue_p = PNULL;

    /*Choose list for pushing*/
    rohcQueue_p = getListTimer(timerType);
    /*SPR 3444 Changes Start */
    if ( PNULL == rohcQueue_p )
    {
        ROHC_LOG(LOG_WARNING, "Wrong timer type. timertype[%u]", timerType);
        return PNULL;
    }
    /*SPR 3444 Changes end */
    
    p_list = rohcQueue_p->pQueue;
    /*SPR 3444 Changes Start*/
    if ( PNULL == p_list )
    {
        ROHC_LOG(LOG_WARNING, "Wrong timer type. timertype[%u]", timerType);
        return PNULL;
    }
    /* SPR 3444 changes end  */

    /*Allocate timeout structure*/
    timeReq = (rohcTimerReq*)getMemFromPool( sizeof(rohcTimerReq), PNULL );
    if ( PNULL == timeReq )
    {
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("MemAlloc fails for TIMER_DATA_REQ\n");
        return PNULL;
    }
    ROHC_LOG(LOG_INFO,"Rohc Timer Started Successfully"); 
    /*Fill timeout structure*/ 
    timeReq->expiredTime = pdcpCurrentTime_g + timeout;
    ROHC_LOG(LOG_INFO,"Expired Tick is %u and Current Tick is %u",
           timeReq->expiredTime,pdcpCurrentTime_g); 
    timeReq->node = node_p;
    timeReq->list_p = p_list;
    listPushNode(p_list, &(timeReq->lNode));
    return &(timeReq->lNode);
}

/****************************************************************************
 * Function Name  : stopRohcTimer  
 * Inputs         : timerType
 *                  void* node_p - pointer to node
 * Outputs        : void
 * Returns        : void
 * Description    : Pops node from timer queue
  ****************************************************************************/
void stopRohcTimer(UInt16 timerType, void* node_p)
{
    LTE_LIST* p_list = PNULL;
    RohcTimerQ*  rohcQueue_p = PNULL;
    UInt32       numTimerNodes = 0;

    /*Choose list for pushing*/
    rohcQueue_p = getListTimer(timerType);
     /* SPR 3444 changes start  */
    if ( PNULL == rohcQueue_p)
    {
         ROHC_LOG(LOG_WARNING, "Wrong timer type. timertype[%u]", timerType);
         return;
    }
     /* SPR 3444 changes end  */

    p_list = rohcQueue_p->pQueue;
    /* SPR 3444 changes start  */
    if ( PNULL == p_list )
    {
         ROHC_LOG(LOG_WARNING, "Wrong timer type. timertype[%u]", timerType);
         return;
    }
    /* SPR 3444 changes end  */

    /*Free timeout structure*/
    /** Fix Start for SPR#2189, 2190, 2193 */
    numTimerNodes = listCount(p_list);
    if(numTimerNodes > 0)
    {
        listDeleteNode(p_list,node_p);
    }
    /** Fix End for SPR#2189, 2190, 2193 */
    
    /*
    ** START_SPR_656_FIX : Though the node was deleted but memory pool was
    ** not freed, which results in memory leak.
    */
    freeMemPool(node_p);
    /* END_SPR_656_FIX */
    
}

/****************************************************************************
 * Function Name  : rohcProcessRohcTimers  
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Processing queues with timers
  ****************************************************************************/
void rohcProcessTimers(void)
{
    /* + SPR 8907 fix */
    rohcProcessTimerList(&lRohcIRTimer, ROHC_IR_RET_TIMER);
    rohcProcessTimerList(&lRohcFOTimer, ROHC_FO_RET_TIMER);
    /* - SPR 8907 fix */
}

/****************************************************************************
 * Function Name  : removeRohcTimer  
 * Inputs         : LTE_LIST_NODE* node_p - node to remove
 * Outputs        : None
 * Returns        : None
 * Description    : Extern interface, removing rohc timer
  ****************************************************************************/
void removeRohcTimer(LTE_LIST_NODE* node_p)
{
    /* Klocwork Fix: removed Null check for node_p */
    freeMemPool( node_p );
}

/****************************************************************************
 * Function Name  : rohcProcessTimerList  
 * Inputs         : RohcTimerQ* rohcQueue_p - pointer to queue for processing
 * Outputs        : None
 * Returns        : None
 * Description    : Processing list with timers
  ****************************************************************************/
/* + SPR 8907 fix */
static void rohcProcessTimerList(RohcTimerQ* rohcQueue_p, rohc_U8bit_t timerType)
/* - SPR 8907 fix */
{
    LTE_LIST_NODE *lp_node= PNULL;
    rohcTimerReq  *timeReq = PNULL; 
    UInt32 numTimerNodes = 0;
    if(PNULL == rohcQueue_p) {
        ROHC_LOG(LOG_WARNING,"rohcQueue_p is NULL");
        return;
    }
    LTE_LIST * lp_list = rohcQueue_p->pQueue; 
    if(PNULL == lp_list) {
        ROHC_LOG(LOG_WARNING,"lp_list is NULL");
        return;
    }
    lp_node = getFirstListNode(lp_list);
    timeReq = (rohcTimerReq *)lp_node;
    while( (PNULL!=lp_node) && pdcpCurrentTime_g >= timeReq->expiredTime)
    {
        ROHC_LOG(LOG_INFO,"Rohc Timer Expired");
        ROHC_LOG(LOG_INFO,"Expire Tick is %u and Current Tick is %u",
                ((rohcTimerReq*)lp_node)->expiredTime,pdcpCurrentTime_g); 
        numTimerNodes = listCount(lp_list);

        /* + SPR 8907 fix */
        /* when timer expire set the node to NULL in CSC context */
        if (timerType == ROHC_IR_RET_TIMER)
        {
           ((rohc_csc_common_t *)(((rohc_timer_data_t *)(timeReq->node))->p_csc_ptr))->ir_ret_timer.tid = PNULL;
        }
        else if (timerType == ROHC_FO_RET_TIMER)
        {
           ((rohc_csc_common_t *)(((rohc_timer_data_t *)(timeReq->node))->p_csc_ptr))->fo_ret_timer.tid = PNULL;
        }
        /* - SPR 8907 fix */

        if(numTimerNodes > 0)
        {
            listDeleteNode(lp_list, lp_node);
        }
        expireRohcTimer(lp_node);
        lp_node = getFirstListNode(lp_list);
        timeReq = (rohcTimerReq *)lp_node;
    }
}
/****************************************************************************
 * Function Name  : expireRohcTimer  
 * Inputs         : LTE_LIST_NODE* node_p - node to remove
 * Outputs        : None
 * Returns        : None
 * Description    : Removing rohc timer and dependent node
  ****************************************************************************/
static void expireRohcTimer(LTE_LIST_NODE* node_p)
{
    if(PNULL==node_p)
    {
        ROHC_LOG(LOG_WARNING,"node_p is NULL");
        return;
    }
    rohc_timer_handler (((rohcTimerReq *)node_p)->node); 
    ((rohcTimerReq*)node_p)->node = ROHC_NULL;
    removeRohcTimer( node_p );
}

/****************************************************************************
 * Function Name  : getListTimer 
 * Inputs         : UInt16 timeType - timeout value
 * Outputs        : None
 * Returns        : RohcTimerQ - pointer to rohc timer queue
 * Description    : returns pointer to rohc timer list
 *                  
  ****************************************************************************/
static RohcTimerQ* getListTimer(UInt16 timerType)
{
    switch(timerType)
    {
        case ROHC_IR_RET_TIMER:    return &lRohcIRTimer;
        case ROHC_FO_RET_TIMER:    return &lRohcFOTimer;
        default:    return PNULL;
    }    
}


