/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacTimingAdvanceMgr.c,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacTimingAdvanceMgr.c,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.6  2009/06/24 13:58:11  gur19413
 * node structure changed.
 *
 * Revision 1.5  2009/06/10 12:41:16  gur19413
 * comments incorportated
 *
 * Revision 1.4  2009/06/02 13:10:03  gur18569
 * removed compilation error
 *
 * Revision 1.3  2009/05/28 08:46:21  gur19413
 * Initial Version
 *
 * Revision 1.2  2009/05/23 12:40:09  gur18550
 * lteMacTransmitter.c
 *
 * Revision 1.1  2009/05/10 09:07:40  gur19413
 * Initial Version
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include "lteLayer2CommanTypes.h"
#include "lteMacTimingAdvanceMgr.h"
/*TA Avging */
#include "lteMacDLUEContext.h"
/*TA Avging */
#ifdef DL_UL_SPLIT
#include "lteMacDLSchedulerStrategy.h"
#endif

/******************************************************************************
 * Project Includes
 *****************************************************************************/

/******************************************************************************
 Private Definitions
 *****************************************************************************/



/******************************************************************************
 Private Types
 *****************************************************************************/

/*****************************************************************************
  * Private Function Prototypes
 ****************************************************************************/
  
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/*Queue for sending TA to DL*/
/* CA Changes start  */
TA_Q_TYPE* dlTimeAdvanceQueue_gp[MAX_NUM_CELL] = {PNULL};
/* CA Changes end  */

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : initTimingAdvanceMgr 
 * Inputs         : Number of cells configured
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialize the dlTimeAdvanceQueue_g Queues for all cells.
 *                  this is called at MAC_INIT_LAYER_REQ
 *****************************************************************************/
/* CA Changes start  */
void initTimingAdvanceMgr(UInt8 numCellsConfigured)
{
    UInt8 cellCount = 0;
    /* Initialising Timing Advance Queue.*/

    createMemPool(sizeof(TA_Q_TYPE),numCellsConfigured);
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        GET_MEM_FROM_POOL(TA_Q_TYPE,dlTimeAdvanceQueue_gp[cellCount],
                sizeof(TA_Q_TYPE),PNULL);
        INIT_TA_Q(dlTimeAdvanceQueue_gp[cellCount]);
    }        
    return;
}

/****************************************************************************
* Function Name  : cleanupTimingAdvanceMgr
* Inputs         : internalCellIndex- specific cellIndex for which cleanup is called.
* Outputs        : None
* Returns        : None
* Description    : This API cleanup the CCCH Manager. (called at cell stop)
****************************************************************************/
void cleanupTimingAdvanceMgr(InternalCellIndex internalCellIndex)
{
    TimeAdvanceQueueNode *taNode_p      = PNULL;

    DEQUEUE_TA_Q(dlTimeAdvanceQueue_gp[internalCellIndex], TimeAdvanceQueueNode, (void **)&(taNode_p));

    while(taNode_p)
    {
        FREE_MEM_NODE_TA_Q(taNode_p);

        DEQUEUE_TA_Q(dlTimeAdvanceQueue_gp[internalCellIndex], TimeAdvanceQueueNode, (void **)&(taNode_p));
    }

    return;
}

/*****************************************************************************
 * Function Name  : deInitTimingAdvanceMgr
 * Inputs         : internalCellIndex- specific cellIndex for which cleanup is called.
 * Outputs        : None
 * Returns        : None
 * Description    : This fuction will release the memory for global queues and 
 *                  variables required by the TA manager. This is called at the time 
 *                  at MAC_CLEANUP_LAYER_REQ
 *****************************************************************************/
void deInitTimingAdvanceMgr(InternalCellIndex internalCellIndex)
{
    QDEINIT_NODE_TA_Q(dlTimeAdvanceQueue_gp[internalCellIndex]);

    if(dlTimeAdvanceQueue_gp[internalCellIndex])
    {
        freeMemPool(dlTimeAdvanceQueue_gp[internalCellIndex]);
        dlTimeAdvanceQueue_gp[internalCellIndex] = PNULL;
    }

    return;
}
/* CA Changes end  */

/*TA Avging */
/*****************************************************************************
 * Function Name  : calculateTimingAdvanceTimeAvg 
 * Inputs         : newUEcontext_p - pointer to Downlink UE Context 
 *                  timingAdvance - Value of timing advance
 * Outputs        : Puts the TimingAdvanceNode in dlTimingAdvanceQueue.
 * Returns        : timeAveragedTAValue/timingAdvance
 * Description    : This function calculates the time Averaged value of TA.
 *****************************************************************************/
/* +- SPR 17777 */
UInt32 calculateTimingAdvanceTimeAvg(
#ifdef TA_AVG
        DLUEContext *newUEcontext_p,
#endif
        UInt16 timingAdvance)
/* +- SPR 17777 */
{
#ifdef TA_AVG
    UInt32 timeAveragedTAValue=0;
    
    timeAveragedTAValue = (MAC_ALPHA_COEFFICIENT_IIR_TA*newUEcontext_p->lastCalculatedTAAverage + \
            (1024 - MAC_ALPHA_COEFFICIENT_IIR_TA)*timingAdvance*100)/1024;
    
    newUEcontext_p->lastCalculatedTAAverage = timeAveragedTAValue;
    
    if (timeAveragedTAValue >= TA_BETA_FACTOR) 
    {
        timeAveragedTAValue = (timeAveragedTAValue / 100) + 1;
    }
    else if(timeAveragedTAValue <= TA_GAMMA_FACTOR)
    {
        timeAveragedTAValue = (timeAveragedTAValue / 100);
    }
    else
    {
        timeAveragedTAValue = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31;
    }
    return timeAveragedTAValue ; 
#else
    return timingAdvance;
#endif

}

/*TA Avging */
/*****************************************************************************
 * Function Name  : putEntryInTimingAdvanceQueue
 * Inputs         : ueIndex - Index of UE 
 *                  timeAdvanceValue
 *                  internalCellIndex - cell id  
 * Outputs        : Puts the TimingAdvanceNode in dlTimingAdvanceQueue.
 * Returns        : None
 * Description    : This function creates a new timingAdvanceNode and inserts
 *                  this node in the dlTimeAdvanceQueue to be processed by 
 *                  Strategy
 *****************************************************************************/
/*CA Changes start  */
void putEntryInTimingAdvanceQueue( UInt32 ueIndex, UInt16 timeAdvanceValue, InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    TimeAdvanceQueueNode *timeVal_p = PNULL;

    GET_MEM_NODE_TA_Q(timeVal_p,TimeAdvanceQueueNode);

    /* Rel 5.3: Coverity Fix Start */
    if (PNULL != timeVal_p)
    {
        timeVal_p->ueIdx = ueIndex;
        timeVal_p->timeAdvanceValue = timeAdvanceValue;


        /*CA Changes start  */
        /* Coverity 26389 Fix Start */
        if(!ENQUEUE_TA_Q( dlTimeAdvanceQueue_gp[internalCellIndex],
                TimeAdvanceQueueNode,&(timeVal_p->timeAdvanceQueueNodeAnchor)))
        {
            lteWarning("Error in Pushing in Pending"
                     " TAQueue in funtion %s\n",
                     __func__);
            FREE_MEM_NODE_TA_Q(timeVal_p);
        }

        /* Coverity 26389 Fix End */
        /*CA Changes end  */
    }
    else
    {
        /* Not able to allocate Memory */
    }
    /* Rel 5.3: Coverity Fix End */
}

/*SPR 15208 Fix*/ 
/*****************************************************************************
 * Function Name  : checkTAEntryCriteria
 * Inputs         : newUEcontext_p - pointer to Downlink UE Context,
                  :  delay - PHY_DL DELAY
 * Outputs        : True/False
 * Returns        : True/False
 * Description    : This function checks if TA command to be sent or not
 *****************************************************************************/
 UInt8 checkTAEntryCriteria(DLUEContext *newUEcontext_p, UInt8 delay)
{
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */

    if((TIME_ALIGNMENT_TIMER_INFINITY != newUEcontext_p->timeAlignmentTimer) &&
       (((UInt32)UL_SYNC_TIME_TO_PROCESS_TA+delay) < (currentTTI - newUEcontext_p->lastTATriggeredTick)) &&
       ((currentTTI - newUEcontext_p->lastTASentTick) > newUEcontext_p->syncTimerDelta) &&
       ((currentTTI - newUEcontext_p->lastTASentTick) < ((UInt32)newUEcontext_p->timeAlignmentTimer - (UInt32)UL_SYNC_TIME_TO_PROCESS_TA)))
    {
        return LTE_TRUE;
    }
    return LTE_FALSE;
}
/*SPR 15208 Fix*/ 

/*****************************************************************************
 * Function Name  : handleTimingAdvance 
 * Inputs         : newUEcontext_p - pointer to Downlink UE Context,
 *                  ueIndex - Index of UE and
 *                  timingAdvance - Value of timing advance
 *                  internalCellIndex - cell id  
 * Outputs        : Puts the TimingAdvanceNode in dlTimingAdvanceQueue.
 * Returns        : None
 * Description    : This function creates a new timingAdvanceNode and inserts
 *                  this node in the dlTimeAdvanceQueue to be processed by 
 *                  Strategy.
 *****************************************************************************/
/*CA Changes start  */
void handleTimingAdvance(DLUEContext *newUEcontext_p,UInt32 ueIndex,UInt16 timingAdvance,
                         InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    /* SPR 6333 TI_TA_FIX_START */
    UInt32  prevTAAvgValue = newUEcontext_p->lastCalculatedTAAverage;
    UInt8 taRangeExceed = LTE_FALSE;
    UInt8 taTatTrigger = LTE_FALSE;
    UInt32 currentTAAvgValue = 0;
    /* SPR 6333 TI_TA_FIX_END */
    /* + SPR_3939 */ 
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    /* - SPR_3939 */
   /* SPR 10187 Changes */ 


    if((prevTAAvgValue < TA_BETA_FACTOR) && (prevTAAvgValue > TA_GAMMA_FACTOR))
    /* SPR 10187 Changes */  
    {
        /*TA Avging */
        /* +- SPR 17777 */
        timingAdvance = calculateTimingAdvanceTimeAvg(
#ifdef TA_AVG
        newUEcontext_p,
#endif
        timingAdvance);
        /* +- SPR 17777 */
        /*TA Avging */
        currentTAAvgValue = timingAdvance*100;
    }
    else
    {
        timingAdvance = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31;
        currentTAAvgValue = timingAdvance*100;
    }
    /* SPR 6333 TI_TA_FIX_START */
    /*TA Range Check:
     * The below check ensures that no new TA is sent, till TA HARQ is freed at eNB
     */
    if((MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31 != timingAdvance) &&
        ((currentTAAvgValue >= TA_BETA_FACTOR) || (currentTAAvgValue <= TA_GAMMA_FACTOR))
	/* SPR 13402 fix start */
	&& (UL_SYNC_TIME_TO_PROCESS_TA < (currentTTI - newUEcontext_p->lastTATriggeredTick)))
	/* SPR 13402 fix end */

    {
        taRangeExceed = LTE_TRUE;
    }
    /*SPR 15208 Fix*/ 
    if(LTE_TRUE == checkTAEntryCriteria(newUEcontext_p,0))
    /*SPR 15208 Fix*/ 
    {
        taTatTrigger = LTE_TRUE;
    }
     
    /* + SPR_3939 */ 
     /* If TA value is valid and   
     *  either TA != 31 OR Timing alignment Timer is going  to expire 
     *  then trigger TA. 
     */
    /*SPR 15208 Fix*/ 
    if((timingAdvance <= MAC_TIMING_ADVANCE_VAL_MAX) && 
       ((taRangeExceed == LTE_TRUE) || (taTatTrigger == LTE_TRUE))&&
       (LTE_FALSE == newUEcontext_p->TA_ScheduleFlag))
        /*SPR 15208 Fix*/ 
     {
         /* The below check ensures that no new TA is sent, till 
            TA HARQ is freed at eNB */
            /*CA Changes start  */
            putEntryInTimingAdvanceQueue(ueIndex, timingAdvance, internalCellIndex);
            /*CA Changes end  */
            /*SPR 15208 Fix*/ 
            newUEcontext_p->TA_ScheduleFlag = LTE_TRUE;
            /*SPR 15208 Fix*/ 
     }
     /* SPR 6333 TI_TA_FIX_END */
    /* - SPR_3939 */ 

}

/*****************************************************************************
 * Function Name  : putEntryInPdcchPdschFailureTAQueue
 * Inputs         : ueIndex - Index of UE and
 *                  timeAdvanceValue - Value of timing advance
 *                  internalCellIndex - cell id  
 * Outputs        : Puts the TimingAdvanceNode in dlTimingAdvanceQueue.
 * Returns        : None
 * Description    : This function creates a new timingAdvanceNode and inserts 
 *                  this node in the dlPendingTimeAdvanceQueue to be processed
 *                  by Strategy.
 *****************************************************************************/
void putEntryInPdcchPdschFailureTAQueue( UInt32 ueIndex, 
                                         UInt16 timeAdvanceValue,
                                         InternalCellIndex internalCellIndex)
{
    TimeAdvanceQueueNode *timeVal_p = PNULL;
    GET_MEM_NODE_TA_Q(timeVal_p , TimeAdvanceQueueNode);

    /* Rel 5.3: Coverity Fix Start */
    if (PNULL != timeVal_p)
    {
        timeVal_p->ueIdx = ueIndex;
        timeVal_p->timeAdvanceValue = timeAdvanceValue;
        /* Coverity 26389 Fix Start */
        if(!ENQUEUE_TA_Q( (pdcchPdschFailureTAQueue_gp[internalCellIndex]),
                TimeAdvanceQueueNode,&(timeVal_p->timeAdvanceQueueNodeAnchor) ))
        {
            lteWarning("Error in Pushing in Pending"
                     " TAQueue in funtion %s\n",
                     __func__);
            FREE_MEM_NODE_TA_Q(timeVal_p);
        }
        /* Coverity 26389 Fix End */
    }
    else
    {
        /* Not able to allocate Memory */
    }
    /* Rel 5.3: Coverity Fix End */
}

/*SPR 15208 Fix*/ 
/*****************************************************************************
 * Function Name  : handleDLTimingAdvance 
 * Inputs         : newUEcontext_p - pointer to Downlink UE Context,
 * Outputs        : Puts the TimingAdvanceNode in dlTimingAdvanceQueue.
 * Returns        : None
 * Description    : This function creates a new timingAdvanceNode and inserts
 *                  this node in the dlTimeAdvanceQueue to be processed by 
 *                  Strategy.
 *****************************************************************************/
void handleDLTimingAdvance(DLUEContext *newUEcontext_p,InternalCellIndex internalCellIndex)
{
    UInt32 timingAdvance = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31;

    /* Here PHY_DL_DELAY is passed because in case the TA command is triggerred because of DL,
     * then same tick TA will be scheduled. In that case the TA will be re triggerred before
     * HARQ received, lead to sending of double TA command */
    if((LTE_TRUE == checkTAEntryCriteria(newUEcontext_p,PHY_DL_DELAY)) &&
            (LTE_FALSE == newUEcontext_p->TA_ScheduleFlag))
    {
        /* The below check ensures that no new TA is sent, till 
           TA HARQ is freed at eNB */
        putEntryInTimingAdvanceQueue(newUEcontext_p->ueIndex, timingAdvance,internalCellIndex);
        newUEcontext_p->TA_ScheduleFlag = LTE_TRUE;
    }

}
/*SPR 15208 Fix*/ 
