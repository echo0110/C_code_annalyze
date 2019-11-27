/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULSchedulerStrategy.c,v 1.2.6.1.6.1 2010/09/29 14:48:05 gur10694 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file has description of the UL Scheduler Strategy.
 Its main function is to allocate the RBs to UE for one 
 or more than one sub-frame. This will be decided from 
 depending upon the number of sub-frames in advance for
 which strategy is run to schedule allocations 
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacULSchedulerStrategy.c,v $
 * Revision 1.2.6.1.6.1  2010/09/29 14:48:05  gur10694
 * Purify UL crash Fix
 *
 * Revision 1.2.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.31  2009/11/10 13:02:53  gur11912
 * modified for warning UE not present in non zero lists
 *
 * Revision 1.30  2009/10/14 08:17:46  gur11912
 * optimization changes
 *
 * Revision 1.29  2009/08/27 17:43:32  gur20052
 * SIT BUG has been fixed
 *
 * Revision 1.28  2009/07/24 12:39:34  gur20052
 * Splint Warnings are removed
 *
 * Revision 1.27  2009/07/20 17:45:04  gur20052
 * UT Defects are Fixed
 *
 * Revision 1.26  2009/07/15 05:59:30  gur20052
 * UT Defects fixed
 *
 * Revision 1.25  2009/07/13 14:06:30  gur20052
 * UT Bugs Fixed
 *
 * Revision 1.23  2009/07/10 13:50:38  gur20052
 * MACModuleLogDetail_g is replaced with the Module Specific STRINGS
 *
 * Revision 1.22  2009/07/07 05:32:13  gur20052
 * UT Defects are fixed
 *
 * Revision 1.21  2009/06/24 08:35:14  gur20052
 * changed the MAX_NUM_DEMUX_THREADS to NUM_OF_DEMUX_THREADS
 *
 * Revision 1.20  2009/06/24 08:10:45  gur20052
 * Reveiw Comments are incorporated
 *
 * Revision 1.19  2009/06/23 16:55:42  gur20052
 * Review comments are incorporated
 *
 * Revision 1.17  2009/06/11 14:52:56  gur20052
 * Removed the Linking Errors and Resource Allocator Node
 *
 * Revision 1.16  2009/06/11 06:14:54  gur12140
* Removed CreateMemPool as already defined in PDCCH
*
* Revision 1.15  2009/06/10 14:27:01  gur20052
* Removed the Linking errors and updated for createMemPool changes
*
* Revision 1.14  2009/06/08 08:21:19  gur20052
* Updated for the Review comments incorporations
*
* Revision 1.13  2009/06/05 16:56:28  gur20052
* Incorporated the reveiw comments
*
* Revision 1.12  2009/06/05 16:20:23  gur20052
* Done the Changes for the Enhancements
*
* Revision 1.11  2009/06/05 06:33:48  gur20052
* Incorporated the reveiw comments
*
* Revision 1.10  2009/06/04 07:39:50  gur20052
* Review Comments are incorporated
*
* Revision 1.9  2009/06/02 07:19:08  gur20052
* Updated during comments incorporation and dispostion
*
* Revision 1.8  2009/05/28 11:33:01  gur20052
* The changes for the UE contexts are done
*
* Revision 1.7  2009/05/26 10:16:04  gur20052
* Updated to incorporated UE Context Changes
*
* Revision 1.6  2009/05/25 16:20:01  gur20052
* Updated the version to support the UL Fair and Round Robin Scheduler
*
* Revision 1.5  2009/05/23 13:17:58  gur20052
* Updated for NON Zero Triggers
*
* Revision 1.4  2009/05/21 17:22:46  gur20052
* Updated the Basic Framework of UL Strategy
*
* Revision 1.3  2009/05/19 14:29:31  gur20052
* Updated with DL Fair Scheduler module specific changes
*
* Revision 1.2  2009/05/11 06:18:32  gur20052
* Removed compilation errors
*
* Revision 1.1  2009/04/30 15:37:10  gur20052
* Initial Version of Uplink Scheduler Strategy added
*
*
*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include "lteMacCellConfMgr.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacExecutionLegs.h"
#include "lteMacULUEContext.h"
#include "lteMacDLHarqMgr.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#include "lteMacTddMgr.h"
#endif
#include "lteMacPHICH.h"
#include "lteMacULQosScheduler.h"
#include "lteMacStatsManager.h"
#include "lteMacDRXMgr.h" 
#include "lteMacMeasGapHandler.h" 
#include "lteMacPUSCH.h"
#include "lteMacDLQosTokenAlgo.h"
#include "lteMacULResourceInfoMgr.h"
#include "lteMacULFairScheduler.h"
#include "lteMacPRACH.h"
#include "lteMacPuschPowerControl.h"
#include "lteCommonStatsManager.h"
#include "lteMacULSpsScheduler.h"
#include "ltePerfStats.h"
/******************************************************************************
  Private Definitions
 *****************************************************************************/



/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/*CA Changes start  */
/* +- SPR 17777 */
void processSyncLossOnOffTriggerQueueUL(InternalCellIndex internalCellIndex);
void processDRXOnOffTriggerQueueUL(InternalCellIndex internalCellIndex); 
/* DRX_CHG */
void processMeasGapOnOffTriggerQueueUL(InternalCellIndex internalCellIndex); 
/* +- SPR 17777 */
/* MEAS_GAP_CHG */
/* Initialisation function for UL PFS Scheduler */
extern void ulPfsInitMacSchedulerData(InternalCellIndex internalCellIndex);
/* + E_CID_5.3 */
void processDummyDCI0List (LTE_LIST *dummyDCI0ListRequiredList_g,
        UInt32 *numberOfAvailableRBs_p,UInt8 *maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
        ,UInt32 ulDelay
#endif 
        ,InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
);

UInt8 eicicFindNumOfBitsinABSPattern(InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* - E_CID_5.3 */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*CA Changes start  */
UL_GRANT_PUSCH_QUEUE_TYPE* ulGrantPUSCHFailureQueue_gp[MAX_NUM_CELL] = {PNULL};
UL_GRANT_PUSCH_QUEUE_TYPE* ulGrantPendingQueue_gp[MAX_NUM_CELL] = {PNULL}; 

LTE_SQUEUE* ulGrantContentionResQueue_gp[MAX_NUM_CELL] = {PNULL};
LTE_SQUEUE* ulGrantGlobalSrQueue_gp[MAX_NUM_CELL] = {PNULL};

UL_GRANT_APERIODIC_CQI_TYPE* ulGrantAperiodicCqiQueue_gp[MAX_NUM_CELL] = {PNULL};
EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE* explicitULPuschTpcScheduleQueue_gp[MAX_NUM_CELL] = {PNULL};
EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE* explicitULPuschTpcScheduleFailureQueue_gp[MAX_NUM_CELL] = {PNULL};
#ifdef DL_UL_SPLIT
extern MacRetType putEntryInULSchedulerNewTxQueueUsingICC(ULStrategyTxNode *ulStrategyTxNode_p, InternalCellIndex internalCellIndex);
#endif

extern RACHGlobals *rachGlobals_gp[MAX_NUM_CELL];
extern ULPfsGlobals *ulPfsGlobals_gp[MAX_NUM_CELL];

/* SPR 5798 Start */
#ifdef FDD_CONFIG
extern UInt8 msg3ScheduledCount_g[MAX_SUB_FRAME];
#elif TDD_CONFIG
extern UInt8 msg3ScheduledCount_g[MAX_NUMBER_OF_CONTAINER];
#endif
/* SPR 5798 End */

/* SRP 3608 changes start */
LTE_SQUEUE* ulHighPriorityGrantQueue_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */
/* SPR 3608 changes end */


/* ICIC Change Start */
/*CA Changes start  */
extern LTE_SQUEUE* ulCellEdgeUserPriorityQueue_gp[MAX_NUM_CELL];
UInt32 maxCellEdgeUEsToBeScheduledUL_g[MAX_NUM_CELL] = {0};
/*CA Changes end  */
/* ICIC Change End */
/* SPR 15909 fix start */
tickType_t ulReferenceFlag_g[MAX_NUM_CELL] = {0};
/* SPR 15909 fix end */

/* SPR 19600 start end */

/*CA Changes start  */
LTE_SQUEUE* ZeroNonZeroBSRTriggerQueue_gp[MAX_NUM_CELL] = {PNULL};
LTE_LIST* nonZeroBsrList_gp[MAX_NUM_CELL] = {PNULL};

/* TX_RETX */
/* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
UInt16 totalRetxRb_g[MAX_NUM_CELL][MAX_USER_LOCATION][NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME];
/*CA Changes end  */
#elif TDD_CONFIG
/* For TDD config 0 ,S subframe ulStrategyTdd0SsfTxList_g will be used to sotre 
 * the StrategyTx node of ulSubframe +k and ulStrategyTdd0SsfTxListUSfOffset7_g 
 * will be used for ulSubframe +7 (where +k and +7 are the offset from the dl SF
 * from where grant is being given for two UL SF) */
/* CA TDD CHANGES START */
extern LTE_LIST ulStrategyTdd0SsfTxList_g[MAX_NUM_CELL];
extern LTE_LIST ulStrategyTdd0SsfTxListUSfOffset7_g[MAX_NUM_CELL];
/* CA TDD CHANGES END */
extern LTE_SQUEUE* ulStrategyTxQueue_gp[MAX_NUM_CELL];
extern RRMMACCellGlobal *rrmMacCellGlobal_gp[MAX_NUM_CELL];
#endif
/* TDD Config 0 Changes Start */

AperiodicModeMinRBInfo* aperiodicModesRBInfo_gp[MAX_NUM_CELL] = {PNULL};

#ifdef OVERSHOOT_CORRECTION
/** CA-TDD Changes Start **/
extern OverLoadLevel macOverLoadLevel_g[MAX_NUM_CELL];
/** CA-TDD Changes Stop **/
#endif

/* EICIC +*/
extern UInt8 absPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
extern UInt8 usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
UInt8 redistributeRb_g = 	LTE_TRUE;
extern UInt8 isEicicEnabled_g[MAX_NUM_CELL];
UInt8 ueTypeToScheduleUL_g[MAX_NUM_CELL] = {NON_VICTIM_UE};
extern UInt8 absPatternIndex_g[MAX_NUM_CELL];
extern  UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL];
/* EICIC -*/

/*** SPR 2934 **/
extern UInt8 memoryOverflow_g;
extern UInt8 msgOverflow_g;
/*** SPR 2934 **/
/*CA Changes start  */
ULUESchedulerInfo* ulScheduleInfo_gp[MAX_NUM_CELL] = {PNULL};
#ifdef FDD_CONFIG
UInt32 availableRBForTtiBundlingPerTick_g[MAX_NUM_CELL];
#endif
extern LTE_LIST* dummyDCI0SendList_gp[MAX_NUM_CELL];
/*CA Changes end  */

/*Cyclomatic Complexity changes - starts here */
STATIC  MacRetType processSRInULPriorityQueue(
        ULUEContext * tempUplinkContext_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        ULGrantRequestQueueNode *srNode_p,
        UInt32 ulDelay,
        /*SPR 22946 Fix Start*/
        /*code removed*/
        /*SPR 22946 Fix End*/
        UInt32 * numberOfAvailableRBs_p,
        UInt8 *tempMaxUEsToBeScheduledUL
#ifdef TDD_CONFIG
        ,UInt8 ulHarqProcessId
        /* TDD Config 0 Changes Start */
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        ,InternalCellIndex internalCellIndex
        );
STATIC  void putEntryInBSRListForULPriorityQueue(
        ULUEContext * tempUplinkContext_p,
        /*SPR 22946  FIX+- code removed*/
        UInt8 *tempMaxUEsToBeScheduledUL,
        ULGrantRequestQueueNode *srNode_p,
        InternalCellIndex internalCellIndex
        ,ExecutionLegFunctionArg* execArgs
        );

void initSPSInULScheduler(UInt8 cellCount);
STATIC void initExplicitUlPuschTpc( UInt8 cellCount );
STATIC void initZeroNonZeroBsr( UInt8 cellCount );

/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
STATIC  void processUlSfOffset7ListNodes(
        void *ulStrategyTxNodeOffset7_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
        );
STATIC  void processUlSfOffsetKListNodes(
        void *ulStrategyNodeVoid_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
        );
STATIC  void processBothUlSfListNodes(
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
        );
STATIC  MacRetType  processUlSfOffsetKSchCountTwoNodes(
        ULStrategyTxNode *ulStrategyTxNode_p,
        ULUEContext     *ulUEContext_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
STATIC  MacRetType isUeEligibleForSingleDCI0(
        ULStrategyTxNode *ulStrategyTxNode_p,
        ULUEContext     *ulUEContext_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt8 *eligibleForSingleDCI0_p,
        InternalCellIndex internalCellIndex
        );
STATIC void deleteUlSfOffset7ListNode(UInt16 ueIndex, 
        /* CA TDD CHANGES START*/
        InternalCellIndex internalCellIndex
        /* CA TDD CHANGES END*/
        );
STATIC  MacRetType updateSchCountInUlStrategyNewTxQueue(
    UInt16 ueIndex,
    UInt8 scheduledCount,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex
    /*CA Changes end  */ 
    );
#endif
/* TDD Config 0 Changes End */
/*Cyclomatic Complexity changes - ends here */


#define  FETCH_NEW_NODE_FROM_DUMMY_DCI0_LIST(node_p,nextNode_p,list_p,dummyDCI0Entry_p)\
{\
    nextNode_p = (DummyDCI0Node *) getNextListNode(&(node_p->nodeAnchor));\
    listDeleteNode((list_p),&(node_p->nodeAnchor));\
    freeMemPool(node_p);\
    node_p = nextNode_p;\
    dummyDCI0Entry_p = PNULL;\
}

UInt16 totalActiveUEsUL_g[MAX_NUM_CELL] = {0};
/* logging changes - 13Jan*/
/* EICIC +*/
UInt32 		phyDelay_g = 0;
/* EICIC -*/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
STATIC MacRetType processSrULGrantReqMsg( UInt32* ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p, 
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay, ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p, 
        ULStrategyTxNode **ulStrategyTxNode_pp,
        /* +- SPR 17777 */
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        );
        /* TDD Config 0 Changes End */
STATIC MacRetType processCrULGrantReqMsg( UInt32* ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p, 
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay, ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p, 
        ULStrategyTxNode **ulStrategyTxNode_pp,
        /* +- SPR 17777 */
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        );
        /* TDD Config 0 Changes End */

STATIC MacRetType processCqiWithAndWithoutDataULGrantReqMsg( 
        UInt32* ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p, 
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay, ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p, 
        ULStrategyTxNode **ulStrategyTxNode_pp,
        /* +- SPR 17777 */
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );
/* SPR 17211 Fix Start */
STATIC MacRetType processNewULGrantReqMsg( UInt32 *ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        tickType_t currentGlobalTTITickCount,
        UInt32 ulDelay,
        ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p,
        ULStrategyTxNode **ulStrategyTxNode_pp,
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        );
/* SPR 17211 Fix End */
STATIC MacRetType processUlTpcWithAndWithoutData( 
        UInt32* ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p, 
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay, ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p, 
        ULStrategyTxNode **ulStrategyTxNode_pp,
        /* +- SPR 17777 */
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        InternalCellIndex internalCellIndex 
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        );
        /* TDD Config 0 Changes End */

STATIC MacRetType processDefaultReqMsg( UInt32* ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p, 
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay, ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p, 
        ULStrategyTxNode **ulStrategyTxNode_pp,
        /* +- SPR 17777 */
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/* State machine to handle UL Grant Types actions for various Grants */
macUlStrategyGrantHandlerFuncT ulGrantStateMachine [INVALID_GRANT] = 
{
    processSrULGrantReqMsg /* SR_ULGRANT_REQ_MSG */,
    processCrULGrantReqMsg /* CR_ULGRANT_REQ_MSG */,
    /* CQI_WITH_DATA_ULGRANT_REQ_MSG */
    processCqiWithAndWithoutDataULGrantReqMsg, 
    /* CQI_WITHOUT_DATA_ULGRANT_REQ_MSG */
    processCqiWithAndWithoutDataULGrantReqMsg,
    /* SPR 17211 Fix Start */
    processNewULGrantReqMsg /* NEW_ULGRANT_REQ_MSG */,
    /* SPR 17211 Fix End */
    processUlTpcWithAndWithoutData /* UL_TPC_WITH_DATA */,
    processUlTpcWithAndWithoutData /* UL_TPC_WITHOUT_DATA */,
    processDefaultReqMsg /* UL_SPS_ACTIVATION */,
    processDefaultReqMsg /* UL_SPS_DEACTIVATION */,
    processDefaultReqMsg /* UL_SPS_OCCASION */,
    processUlTpcWithAndWithoutData /* UL_EXPLICIT_PHR_WITHOUT_DATA */,
#ifdef FDD_CONFIG
    processDefaultReqMsg /* NEW_TTIB_ULGRANT_REQ_MSG */,
    processDefaultReqMsg /* TTIB_UL_SPS_OCCASION */
#endif
};

/*****************************************************************************
 * Function Name  : processMeasGapOnOffTriggerQueueUL 
 * Inputs         :
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Meas Gap ON/OFF trigger queue 
 *                  and based on the trigger it adds or deletes the UE from the 
 *                  nonZero BSR list.
 *****************************************************************************/
/* +- SPR 17777 */
void processMeasGapOnOffTriggerQueueUL(InternalCellIndex internalCellIndex)
    /* +- SPR 17777 */
{
    UInt16                      queueCount = 0;
    UInt16                      ueIndex = 0;
    MeasGapTrigger              measGapOnOffTrigger = MEASUREMENT_PERIOD_INVALID;
    MeasGapOnOffTriggerNode     *measGapOnOffTriggerNode_p = PNULL;
    ULUEContext                 *ulUEContext_p  = PNULL;  
    ULUEContextInfo             *ulUEContextInfo_p = PNULL;
    LTE_LIST_NODE               *nonZeroBsrList_p = PNULL;
    LTE_LIST_NODE               *nonZeroBsrListTempDeleted_p = PNULL;

    /* CA Changes start */
    UInt8 ulSchedulerStrategy = cellSpecificParams_g.
        cellConfigAndInitParams_p[internalCellIndex]->initParams_p->
        ulSchdConfig.ulSchedulerStrategy;

    queueCount = COUNT_UL_MEAS_GAPON_OFF_TRIGGER_Q(internalCellIndex);

    while (queueCount--)
    {
        DEQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q( MeasGapOnOffTriggerNode,
                (void *)&measGapOnOffTriggerNode_p, internalCellIndex);
        /*CA Changes end  */

        if (measGapOnOffTriggerNode_p)
        {
            ueIndex = measGapOnOffTriggerNode_p->ueIndex;
            ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            /*Checking the UE Context & Checking for Pending 
              delete Flag of UE*/
            if (!(ulUEContextInfo_p->pendingDeleteFlag))
            {
                /* SPR 18958 Fix Start */
                /*  EICIC + */
		/* EICIC Optimization +- */
		if(isEicicEnabled_g[internalCellIndex])
		{
                /* Pushing UE back in trigger queue.*/
                if (((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && 
                            (NON_VICTIM_UE == ulUEContext_p->userType))||
                        ((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && 
                         (VICTIM_UE == ulUEContext_p->userType)))
                {              
                    if(!ENQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
                                &(measGapOnOffTriggerNode_p->anchorNode), internalCellIndex))
                    {
                        FREE_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
                    }
                    continue;
                } 
                /* SPR 18958 Fix End */
		}
		/* EICIC Optimization +- */
                /* EICIC - */
                nonZeroBsrList_p = ulUEContext_p->nonZeroBsrList_p;
                nonZeroBsrListTempDeleted_p = ulUEContext_p->
                    nonZeroBsrListTempDeleted_p;

                measGapOnOffTrigger = measGapOnOffTriggerNode_p->onOffTrigger;
                switch(measGapOnOffTrigger)
                {
                    case MEASUREMENT_PERIOD_ON_UL:

                        /* This code is only for testing purpose. To be removed
                         * after testing */
                        if (UL_SYNC_LOSS_ON == ulUECtxInfoArr_g[ueIndex].
                                ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState)
                        {
                            if (PNULL == nonZeroBsrListTempDeleted_p)
                            {
                                lteWarning("Invalid Scenario. UL Synch Loss is "
                                        "On. And UE is still in Active UE BSR "
                                        "list\n");
                            }
                            FREE_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
                            continue;
                        }

                        if (nonZeroBsrList_p)
                        {
                            /*The Node in the nonZeroBsrList LIST will be 
                              deleted */

                            /* SPR 2377 changes start */
                            LOG_MAC_MSG(MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_UL_LOG_ID
                                    ,LOGBRIEF, MAC_UL_Strategy, 
                                    getCurrentTick(), ulUEContext_p->ueIndex, 
                                    measGapOnOffTrigger,
                                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE, __func__,"");

                            /* SPR 2377 changes end */

                            ulUEContext_p->nonZeroBsrListTempDeleted_p = 
                                ulUEContext_p->nonZeroBsrList_p;

                            deleteEntryFromNonZeroBSRListTemporarily(
                                    ulUEContext_p,
                                    internalCellIndex);  

                            ulUEContext_p->nonZeroBsrList_p = PNULL;
                        }

                        break;

                    case MEASUREMENT_PERIOD_OFF_UL:

                        if ( ( DRX_STATE_ON == ulUEContext_p->drxCurrentState ) || 
                                ( UL_SYNC_LOSS_ON == 
                                  ulUEContext_p->ulSyncLossOrHOInfoInUL.
                                  ulSyncLossState ) )
                        {
                            FREE_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q( 
                                    measGapOnOffTriggerNode_p );
                            continue;
                        }

                        if (nonZeroBsrListTempDeleted_p)
                        {
                            if (0 == ulUEContext_p->bsrNetTotal)
                            {
                                freeMemPool((void *)ulUEContext_p->
                                        nonZeroBsrListTempDeleted_p);
                                ulUEContext_p->nonZeroBsrListTempDeleted_p = 
                                    PNULL;
                                FREE_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
                                continue;
                            }

                            /* SPR 2377 changes start */
                            LOG_MAC_MSG(MAC_UE_ADDED_IN_ACTIVE_LIST_IN_UL_LOG_ID, 
                                    LOGBRIEF, MAC_UL_Strategy, 
                                    getCurrentTick(), ulUEContext_p->ueIndex, 
                                    measGapOnOffTrigger,
                                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE, __func__,"");

                            /* SPR 2377 changes end */

                            /* CA Changes start */
                            if(UL_FAIR_SCHEDULING == ulSchedulerStrategy)
                            {       
                                listInsertNodeAtHead(nonZeroBsrList_gp[internalCellIndex],
                                        nonZeroBsrListTempDeleted_p);
                            }
                            else
                            {
                                listInsertNode(nonZeroBsrList_gp[internalCellIndex],
                                        /*CA Changes end  */
                                        nonZeroBsrListTempDeleted_p);
                            }

                            ulUEContext_p->nonZeroBsrList_p = 
                                ulUEContext_p->nonZeroBsrListTempDeleted_p;
                            ulUEContext_p->nonZeroBsrListTempDeleted_p = PNULL;
                        }
                        else
                        {
                            if ((PNULL == ulUEContext_p->nonZeroBsrList_p) &&
                                    0 != ulUEContext_p->bsrNetTotal) 
                            {
                                putEntryInNonZeroBSRList(ueIndex, 
                                        ulUEContext_p,
                                        internalCellIndex);
                            }
                        }

                        break;

                    default:
                        /* Log error */
                        break;
                }
            }
            FREE_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
        }
    }
}

/*****************************************************************************
 * Function Name  : processDRXOnOffTriggerQueueUL 
 * Inputs         : currentGlobalTTITickCount - current Global Tick for which 
                        processing need to be done,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the DRX ON/OFF trigger queue and
 *                  based on the trigger it adds or deletes the UE from the 
 *                  nonZero BSR list.
 *****************************************************************************/
/*CA Changes start  */
/* +- SPR 17777 */
void processDRXOnOffTriggerQueueUL(InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
/*CA Changes end  */
{
    UInt16                  queueCount = 0;
    UInt16                  ueIndex = 0;
    DRXOnOffTrigger         drxOnOffTrigger = DRX_TRIGGER_INVALID;
    DRXOnOffTriggerNode     *drxOnOffTriggerNode_p = PNULL;
    ULUEContext             *ulUEContext_p  = PNULL;  
    MeasGapContextUL        *ulMeasGapContext_p  = PNULL;  
    ULUEContextInfo         *ulUEContextInfo_p = PNULL;
    LTE_LIST_NODE           *nonZeroBsrList_p = PNULL;
    LTE_LIST_NODE           *nonZeroBsrListTempDeleted_p = PNULL;

    /* CA Changes start */
    UInt8 ulSchedulerStrategy = cellSpecificParams_g.
        cellConfigAndInitParams_p[internalCellIndex]->initParams_p->
        ulSchdConfig.ulSchedulerStrategy;

    queueCount =  COUNT_UL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex);

    while (queueCount--)
    {
        DEQUEUE_UL_DRX_ON_OFF_TRIGGER_Q( DRXOnOffTriggerNode,
                (void *)&(drxOnOffTriggerNode_p), internalCellIndex);
        /*CA Changes end  */
        if (drxOnOffTriggerNode_p)
        {
            ueIndex = drxOnOffTriggerNode_p->ueIndex;
            ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

            /*Checking the UE Context & Checking for Pending 
              delete Flag of UE*/
            if (!(ulUEContextInfo_p->pendingDeleteFlag))
            {
                ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

				/* EICIC + */	
			/*Complexity Start*/	
		        if (((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && 
                      (NON_VICTIM_UE == ulUEContext_p->userType)) ||
                      ((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && 
                      (VICTIM_UE == ulUEContext_p->userType)))
		        {              
			        if(!ENQUEUE_UL_DRX_ON_OFF_TRIGGER_Q(DRXOnOffTriggerNode, 
						    &(drxOnOffTriggerNode_p->anchorNode), internalCellIndex))
			        {
				        FREE_MEM_NODE_UL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
			        }
                    continue;
		        }
/*Complexity End*/
		/*  EICIC - */

                ulMeasGapContext_p = &ulUEContext_p->measGapContext;
                nonZeroBsrList_p = ulUEContext_p->nonZeroBsrList_p;
                nonZeroBsrListTempDeleted_p = ulUEContext_p->
                    nonZeroBsrListTempDeleted_p;
                drxOnOffTrigger = drxOnOffTriggerNode_p->onOffTrigger;

                /*START : 5352*/
                LOG_MAC_MSG(MAC_UL_DRX_TRIGGER_INFO,LOGDEBUG,
                        MAC_UL_Strategy, getCurrentTick(),
                        ulUEContext_p->ueIndex, drxOnOffTrigger,
                        DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        __func__,"DRX_TRIGGER_INFO_RECIEVED_INFO");
                /*END   : 5352*/

                switch(drxOnOffTrigger)
                {
                    case DRX_OFF_TO_DRX_ON:

                    /* This code is only for testing purpose. To be removed
                     * after testing */
                    if (UL_SYNC_LOSS_ON == ulUEContext_p->
                            ulSyncLossOrHOInfoInUL.ulSyncLossState)
                    {
                        if (PNULL == nonZeroBsrListTempDeleted_p)
                        {
                            lteWarning("Invalid Scenario. UL Synch Loss is "
                                    "On. And UE is still in Active UE BSR "
                                    "list\n");
                        }
                        FREE_MEM_NODE_UL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
                        continue;
                    }

                    if (nonZeroBsrList_p)
                    {
                        /*The Node in the nonZeroBsrList LIST will be 
                          deleted */
                        /* SPR 2377 changes start */
                        LOG_MAC_MSG(MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_UL_LOG_ID
                                ,LOGBRIEF, MAC_UL_Strategy, 
                                getCurrentTick(), ulUEContext_p->ueIndex, 
                                drxOnOffTrigger,
                                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                                DEFAULT_FLOAT_VALUE, __func__,"");

                        /* SPR 2377 changes end */

                        ulUEContext_p->nonZeroBsrListTempDeleted_p = 
                            ulUEContext_p->nonZeroBsrList_p;

                        deleteEntryFromNonZeroBSRListTemporarily(
                                ulUEContext_p,
                                internalCellIndex);  

                        ulUEContext_p->nonZeroBsrList_p = PNULL;
                    }

                    break;

                    case DRX_ON_TO_DRX_OFF:

                    if ( ( TRUE == ulMeasGapContext_p->isMeasGapPeriodOn ) ||
                            ( UL_SYNC_LOSS_ON == 
                              ulUEContext_p->ulSyncLossOrHOInfoInUL.
                              ulSyncLossState ) )
                    {
                        FREE_MEM_NODE_UL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
                        continue;
                    }

                    if (nonZeroBsrListTempDeleted_p)
                    {
                        if (0 == ulUEContext_p->bsrNetTotal)
                        {
                            /* SPR 2548 Changes Start */
                            freeMemPool((void *)ulUEContext_p->
                                    nonZeroBsrListTempDeleted_p);
                            ulUEContext_p->nonZeroBsrListTempDeleted_p = 
                                PNULL;
                            /* SPR 2548 Changes End */

                            FREE_MEM_NODE_UL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
                            continue;
                        }

                        /* SPR 2377 changes start */
                        LOG_MAC_MSG(MAC_UE_ADDED_IN_ACTIVE_LIST_IN_UL_LOG_ID, 
                                LOGBRIEF, MAC_UL_Strategy, 
                                getCurrentTick(), ulUEContext_p->ueIndex, 
                                drxOnOffTrigger,
                                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                                DEFAULT_FLOAT_VALUE, __func__,"");

                        /* SPR 2377 changes end */

                        /* We need to insert the node at the top of the
                         * list. */

                        ulUEContext_p->nonZeroBsrList_p = 
                            ulUEContext_p->nonZeroBsrListTempDeleted_p;

                        if (UL_ROUND_ROBIN_SCHEDULING == ulSchedulerStrategy)
                        {
                            listInsertNodeBefore(nonZeroBsrList_gp[internalCellIndex],
                                    nonZeroBsrListStartRoundRobin_p[internalCellIndex], 
                                    ulUEContext_p->
                                    nonZeroBsrListTempDeleted_p);

                            nonZeroBsrListStartRoundRobin_p[internalCellIndex] = 
                                ulUEContext_p->nonZeroBsrList_p;
                        }
                        else if(UL_FAIR_SCHEDULING == ulSchedulerStrategy)
                        {
                            listInsertNodeAtHead(nonZeroBsrList_gp[internalCellIndex],
                                    ulUEContext_p->nonZeroBsrListTempDeleted_p);
                        }
                        else
                        {
                            listInsertNode(nonZeroBsrList_gp[internalCellIndex], 
                                    ulUEContext_p->
                                    nonZeroBsrListTempDeleted_p);
                        }

                        ulUEContext_p->nonZeroBsrListTempDeleted_p = PNULL;
                    }
                    else
                    {
                        if ((PNULL == ulUEContext_p->nonZeroBsrList_p) &&
                                0 != ulUEContext_p->bsrNetTotal)
                        {
                            putEntryInNonZeroBSRList(ueIndex, 
                                    ulUEContext_p,
                                    internalCellIndex);
                        }
                    }
                    break;
                    case DRX_OFF_TO_DRX_OFF:
                    /* Dont do anything */
                    break;

                    default:
                    break;
                }
            }

            FREE_MEM_NODE_UL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
        }
    }
}

/*****************************************************************************
 * Function Name  : processSyncLossOnOffTriggerQueueUL 
 * Inputs         : currentGlobalTTITickCount - current Global Tick for which 
                        processing need to be done,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the UL Sync Loss ON/OFF trigger
 *                  queue and based on the trigger it adds or deletes the
 *                  UE from the nonZero BSR list.
 *****************************************************************************/
/* SPR 15909 fix start */
void processSyncLossOnOffTriggerQueueUL(/* +- SPR 17777 */
/* SPR 15909 fix end */        
                                        InternalCellIndex internalCellIndex)
{
    UInt16 queueCount = 0;
    UInt16 ueIndex = 0;
    UlSyncLossOnOff ulSyncLossOnOffTrigger = UL_SYNC_LOSS_INVALID;
    ULSyncLossQueueNode *ulSyncLossQueueNode_p = PNULL;
    ULUEContext *ulUEContext_p  = PNULL;  
    ULUEContextInfo *ulUEContextInfo_p = PNULL;
    LTE_LIST_NODE *nonZeroBsrList_p = PNULL;
    LTE_LIST_NODE *nonZeroBsrListTempDeleted_p = PNULL;

    queueCount = COUNT_UL_SYNC_LOSS_TRIGGERQ_UL_Q(internalCellIndex);
    while (queueCount)
    {
        DEQUEUE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ULSyncLossQueueNode, (void *)&ulSyncLossQueueNode_p, internalCellIndex);
        if(ulSyncLossQueueNode_p)
        {
            ueIndex = ulSyncLossQueueNode_p->ueIndex;
            ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

            /*Checking the UE Context & Checking for Pending delete Flag 
              of UE*/ 
            if(!(ulUEContextInfo_p->pendingDeleteFlag))
            {
                ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

              /* EICIC +*/
		      /* Pushing UE back in trigger queue.*/
		/* EICIC Optimization +- */
		if(isEicicEnabled_g[internalCellIndex])
		{     
		if ((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == ulUEContext_p->userType))
		{              
			if(!ENQUEUE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ULSyncLossQueueNode,
						&(ulSyncLossQueueNode_p->anchorNode), internalCellIndex))
			{
				FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossQueueNode_p);
			}

			queueCount--;   
			continue;
		}

		else if ((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == ulUEContext_p->userType))
		{           
			if(!ENQUEUE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ULSyncLossQueueNode,
						&(ulSyncLossQueueNode_p->anchorNode), internalCellIndex))
			{
				FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossQueueNode_p);
			}

			queueCount--;   
			continue;
		}
		}
		/* EICIC Optimization +- */
		/* EICIC - */

                nonZeroBsrList_p = ulUEContext_p->nonZeroBsrList_p;
                nonZeroBsrListTempDeleted_p = ulUEContext_p->
                    nonZeroBsrListTempDeleted_p;
                ulSyncLossOnOffTrigger = ulSyncLossQueueNode_p->onOffTrigger;

                switch(ulSyncLossOnOffTrigger)
                {
                    case UL_SYNC_LOSS_ON:

                    ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState = UL_SYNC_LOSS_ON;
                    if (nonZeroBsrList_p)
                    {
                        /*The Node in the nonZeroBsrList LIST will be
                         * deleted and stored in UL UE Context*/
                        LOG_MAC_MSG(MAC_UE_DELETED_FROM_ACTIVE_LIST_UL_SYNC_LOSS, 
                                LOGWARNING,
                                MAC_UL_Strategy, 
                                getCurrentTick(),
                                ulUEContext_p->ueIndex, 
                                ulSyncLossOnOffTrigger,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE, 
                                DEFAULT_FLOAT_VALUE,
                                __func__,"");

                        ulUEContext_p->nonZeroBsrListTempDeleted_p =
                            ulUEContext_p->nonZeroBsrList_p;
                        deleteEntryFromNonZeroBSRListTemporarily(
                                ulUEContext_p,
                                internalCellIndex);
                        ulUEContext_p->nonZeroBsrList_p = PNULL;
                    }
                    break;

                    case UL_SYNC_LOSS_OFF:

                    /* SPR  3444 changes start */

                    if (nonZeroBsrListTempDeleted_p)
                    {
                        if (0 == ulUEContext_p->bsrNetTotal)
                        {
                            freeMemPool((void *)ulUEContext_p->
                                    nonZeroBsrListTempDeleted_p);
                            ulUEContext_p->nonZeroBsrListTempDeleted_p = 
                                PNULL;

                            FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossQueueNode_p);
						queueCount--;  /* EICIC Changes , count needs to be 
							decremented */
                            continue;
                        }

                        ulUEContext_p->nonZeroBsrList_p = 
                            ulUEContext_p->nonZeroBsrListTempDeleted_p;

                        /*CA Changes start  */
                        listInsertNode(nonZeroBsrList_gp[internalCellIndex], 
                        /*CA Changes end  */
                                nonZeroBsrListTempDeleted_p);

                        ulUEContext_p->nonZeroBsrListTempDeleted_p = PNULL;
                    }
                    else
                    {
                        if ((PNULL == ulUEContext_p->nonZeroBsrList_p) &&
                                0 != ulUEContext_p->bsrNetTotal)
                        {
                            putEntryInNonZeroBSRList(ueIndex, 
                                    ulUEContext_p,
                                    /*CA Changes start  */
                                    internalCellIndex);
                                    /*CA Changes end  */
                        }
                    }
                    /* SPR  3444 changes end */

                    break;

                    default:

                    break;
                }
            }
            FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossQueueNode_p);
            
        }
        queueCount--;
    }/* End of while loop */
    return;
}

/* TTIB Code Optimization Start */
/**********************************************************************************
 * Function Name  : ulStrategyQueueProcessfn_p
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This is function pointer which calls the appropriate function. 
 **********************************************************************************/
void (*ulStrategyQueueProcessfn_p[MAX_UL_STRATEGY_Q])(UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        )=
        /* TDD Config 0 Changes End */
{
    processUlGrantPUSCHFailureQueueFn,
    processUlGrantPendingQueueFn,
    processUlGrantContentionResQueue_gFn,
    processUlHighPriorityGrantQueue_gFn,
    processUlGrantGlobalSrQueue_gFn,
    processExplicitULPuschTpcScheduleFailureQueue_gFn,
    processExplicitULPuschTpcScheduleQueue_gFn,
    /* + E_CID_5.3 */
    processDummyDCI0ListFailureQueue_gFn,
    processDummyDCI0ListQueue_gFn,
    /* - E_CID_5.3 */
    processUlGrantAperiodicCqiQueue_gFn,
    processScheduleUEsByULStrategyFN
};
/*Processing of the Pending UL Grant Queue*/
/**********************************************************************************
 * Function Name  : processUlGrantPUSCHFailureQueueFn
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the UL Grant PUSCH Failure Queue.
 **********************************************************************************/
/*SPR 22919 Start*/
 void processUlGrantPUSCHFailureQueueFn(UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(execArgs)
    /* +- SPR 17777 */
    UInt32 qCount = 0;
    qCount = COUNT_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPUSCHFailureQueue_gp[internalCellIndex]);
    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_UL_GRANTS(qCount, 
            internalCellIndex);
    /* CA Stats Changes End */
    if (qCount)
    {
        processPendingULGrantQueue(ulGrantPUSCHFailureQueue_gp[internalCellIndex],
                qCount,
                /* ICIC changes start */
                numberOfAvailableRBs_p,
                /* ICIC changes end */
                maxUEsToBeScheduledUL_p,
                currentGlobalTTITickCount,
                ulDelay,
                /*CA Changes start  */
                internalCellIndex
                /*CA Changes end  */
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );
    }
}
/*Processing of the Pending UL Grant Queue*/
/**********************************************************************************
 * Function Name  : processUlGrantPendingQueueFn
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Pending UL Grant Queue.
 **********************************************************************************/
/*SPR 22919 Start*/
 void processUlGrantPendingQueueFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(execArgs)
    /* +- SPR 17777 */
    UInt32 qCount = 0;
    /*CA Changes start  */
    qCount = COUNT_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PENDING_UL_GRANTS(qCount, 
            internalCellIndex);
    /* CA Stats Changes End */
    if (qCount)
    {
        /*CA Changes start  */
        processPendingULGrantQueue(ulGrantPendingQueue_gp[internalCellIndex],
                qCount,
                /* ICIC changes start */
                numberOfAvailableRBs_p,
                /* ICIC changes end */
                maxUEsToBeScheduledUL_p,
                currentGlobalTTITickCount,
                ulDelay,
                internalCellIndex
                /*CA Changes end  */
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );
    }
}

/*Processing of the Contention Resolution Queue*/
/************************************************************************************
 * Function Name  : processUlGrantContentionResQueue_gFn
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the UL Grant Contention Resolution Queue.
 ************************************************************************************/
/*SPR 22919 Start*/
 void processUlGrantContentionResQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(execArgs)
    /* +- SPR 17777 */
    UInt32 qCount = 0;
    /*CA Changes start  */
    qCount = sQueueCount(ulGrantContentionResQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    if (( qCount))
    {
        processContentionResULGrantQueue(qCount,
                /* ICIC changes start */
                numberOfAvailableRBs_p,
                /* ICIC changes end */
                maxUEsToBeScheduledUL_p,
                currentGlobalTTITickCount,
                ulDelay, internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );
    }
}

/*Processing of the Priority Scheduling Request Queue */
/*****************************************************************************************
 * Function Name  : processUlHighPriorityGrantQueue_gFn
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the UL High Priority Scheduling Request Queue.
 *****************************************************************************************/
/*SPR 22919 Start*/
 void processUlHighPriorityGrantQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    UInt32 qCount = 0;
    /*CA Changes start  */
    qCount = sQueueCount(ulHighPriorityGrantQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    if (qCount)
    {
        processULPriorityQueue(qCount,
                numberOfAvailableRBs_p,
                maxUEsToBeScheduledUL_p,
                currentGlobalTTITickCount,
                ulDelay,
                /*CA Changes start  */
                internalCellIndex,
                /*CA Changes end  */
                execArgs
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );
    }
}

/*Processing of the Scheduling Request Queue */
/**********************************************************************************
 * Function Name  : processUlGrantGlobalSrQueue_gFn
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the UL Global Scheduling Request Queue.
 **********************************************************************************/
/*SPR 22919 Start*/
 void processUlGrantGlobalSrQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(execArgs)
    /* +- SPR 17777 */
    UInt32 qCount = 0;
    /*CA Changes start  */
    qCount = sQueueCount(ulGrantGlobalSrQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    if (qCount)
    {
        processSRULGrantQueue(qCount,
                /* ICIC changes start */
                numberOfAvailableRBs_p,
                /* ICIC changes end */
                maxUEsToBeScheduledUL_p,
                currentGlobalTTITickCount,
                ulDelay,
                /*CA Changes start  */
                internalCellIndex
                /*CA Changes end  */
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );
    }
}

/* Process the explicit UL UCI queue */
/**********************************************************************************
 * Function Name  : processExplicitULPuschTpcScheduleFailureQueue_gFn
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Explicit UL PUSCH TPC
 *                  Schedule Failure Queue.
 **********************************************************************************/
/*SPR 22919 Start*/
 void processExplicitULPuschTpcScheduleFailureQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(execArgs)
    /* +- SPR 17777 */
    /* +- SPR 17777 */
#ifdef TDD_CONFIG
  LTE_GCC_UNUSED_PARAM(ulDelay)
#endif
    /* +- SPR 17777 */
    UInt32 qCount = 0;

    /*CA Changes start  */
    qCount = COUNT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
            explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    if (qCount)
    {
        processExplicitULDCIAndPHRScheduleQueue(
                qCount,
                numberOfAvailableRBs_p,
                maxUEsToBeScheduledUL_p,
                currentGlobalTTITickCount,
                /*CA Changes start  */
                explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex]
                /*CA Changes end  */
#ifdef FDD_CONFIG
                , ulDelay
#endif
                , internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );
    }
}

/* Process the explicit UL UCI queue */
/************************************************************************************
 * Function Name  : processExplicitULPuschTpcScheduleQueue_gFn
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Explicit UL PUSCH TPC Schedule Queue.
 ************************************************************************************/
/*SPR 22919 Start*/
 void processExplicitULPuschTpcScheduleQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
        /* +- SPR 17777 */
      LTE_GCC_UNUSED_PARAM(execArgs)
        /* +- SPR 17777 */
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
  LTE_GCC_UNUSED_PARAM(ulDelay)
#endif
    /* +- SPR 17777 */
        UInt32 qCount = 0;

        /*CA Changes start  */
        qCount = COUNT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
                 explicitULPuschTpcScheduleQueue_gp[internalCellIndex]);
        /*CA Changes end  */
        if (qCount)
        {
            processExplicitULDCIAndPHRScheduleQueue(
                    qCount,
                    numberOfAvailableRBs_p,
                    maxUEsToBeScheduledUL_p,
                    currentGlobalTTITickCount,
                    /*CA Changes start  */
                    explicitULPuschTpcScheduleQueue_gp[internalCellIndex]
                    /*CA Changes end  */
#ifdef FDD_CONFIG
                    , ulDelay
#endif
                    , internalCellIndex
                    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                    ,ulSubFrameNum
#endif
                    /* TDD Config 0 Changes End */
                    );
        }
    }

/* + E_CID_5.3 */
/* Processing E_CID dummy DCI0 faiure list */
/**********************************************************************************
 * Function Name  : processDummyDCI0ListFailureQueue_gFn
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the E_CID dummy DCI0 faiure Queue.
 **********************************************************************************/
/*SPR 22919 Start*/
 void processDummyDCI0ListFailureQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(execArgs)
#ifdef TDD_CONFIG
  LTE_GCC_UNUSED_PARAM(ulDelay)
#endif
    /* +- SPR 17777 */

    /*CA Changes start  */
	processDummyDCI0List(dummyDCI0SendFailureList_gp[internalCellIndex],
			numberOfAvailableRBs_p,
			maxUEsToBeScheduledUL_p,
			currentGlobalTTITickCount
#ifdef FDD_CONFIG
			,ulDelay 
#endif
            ,internalCellIndex
            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
            ,ulSubFrameNum
#endif
            /* TDD Config 0 Changes End */
			);
    /*CA Changes end  */


}

/* Process E_CID dummy DCI0  list */
/**********************************************************************************
 * Function Name  : processDummyDCI0ListQueue_gFn
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes E_CID dummy DCI0 Queue.
 **********************************************************************************/
/*SPR 22919 Start*/
 void processDummyDCI0ListQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(execArgs)
#ifdef TDD_CONFIG
  LTE_GCC_UNUSED_PARAM(ulDelay)
#endif
    /* +- SPR 17777 */
    /*CA Changes start  */
	processDummyDCI0List(dummyDCI0SendList_gp[internalCellIndex],
			numberOfAvailableRBs_p,
			maxUEsToBeScheduledUL_p,
			currentGlobalTTITickCount
#ifdef FDD_CONFIG
			,ulDelay 
#endif
            , internalCellIndex
            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
            ,ulSubFrameNum
#endif
            /* TDD Config 0 Changes End */
			);
    /*CA Changes end  */
}
/* - E_CID_5.3 */

/*Process of the aperiodic CQI Rquest Queue */
/************************************************************************************
 * Function Name  : processUlGrantAperiodicCqiQueue_gFn
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the UL Grant aperiodic CQI Rquest Queue.
 ************************************************************************************/
/*SPR 22919 Start*/
 void processUlGrantAperiodicCqiQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(execArgs)
    /* +- SPR 17777 */
    UInt32 qCount = 0;

    qCount = COUNT_UL_GRANT_APERIODIC_CQI_Q(internalCellIndex);
    if (qCount)
    {
        /*SPR 2446 Fix Begins*/
        processCQIULGrantQueue(qCount,
                /* ICIC changes start */
                numberOfAvailableRBs_p,
                /* ICIC changes end */
                maxUEsToBeScheduledUL_p,
                currentGlobalTTITickCount
                ,ulDelay
                ,internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );

        /*SPR 2446 Fix Ends*/
    }
}

/**********************************************************************************
 * Function Name  : processScheduleUEsByULStrategyFN
 * Inputs         : numberOfAvailableRBs_p - Number of availabel RB's,
 *                  maxUEsToBeScheduledUL_p- Maxumum number of UE can be scheduled in UL,
 *                  currentGlobalTTITickCount - current TTI,
 *                  ulDelay - Dleay to phy,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Scehduled UE's by UL Startegy.
 **********************************************************************************/
/*SPR 22919 Start*/
 void processScheduleUEsByULStrategyFN (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(execArgs)
#ifdef TDD_CONFIG
  LTE_GCC_UNUSED_PARAM(ulDelay)
#endif
    /* +- SPR 17777 */
    scheduleUEsByULStrategy (
    /* ICIC changes start */
    numberOfAvailableRBs_p,
    (*maxUEsToBeScheduledUL_p),
    currentGlobalTTITickCount,
    /* +- SPR 17777 */
#ifdef FDD_CONFIG
    ulDelay,
#endif
    /* CA Changes start */
    internalCellIndex
    /* +- SPR 17777 */
    /* CA Changes end */
    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    ,ulSubFrameNum
#endif
    /* TDD Config 0 Changes End */
    );
}
#ifdef FDD_CONFIG
/*******************************************************************************
 *  Function Name  : processMeasGapValidation
 *  Inputs         : currentGlobalTTITickCount : current Global Tick
 *                   ulDelay - The amount of delay expected in the uplink path.
 *                   internalCellIndex - cell index used at MAC.
 *  Outputs        : Validate the TTI Bundled UE for measGap
 *  Returns        : MAC_SUCCESS/MAC_FAILURE
 *  Description    : This function will validate in nth tick whether the UE can 
 *                   lie under measGap in n+1 tick and (n+5, n+6 and n+7)th tick.
 ********************************************************************************/
MacRetType processMeasGapValidation(ULUEContext *ulUEContext_p, 
                                    /* SPR 15909 fix start */
                                   tickType_t currentGlobalTTITickCount, 
                                    /* SPR 15909 fix end */
                                   UInt32 ulDelay)
{

    UInt32 newBundleIndex = 0;
    MacRetType retval = MAC_FAILURE;

    /* TTI_BUNDLING: If DCI0 tick is in measurement gap then don't
     * schedule bundled UE */

    if (FALSE == isTTIInMeasGapPeriod(ulUEContext_p,( currentGlobalTTITickCount + ulDelay )))
    {
        
        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy][%s] [%d] TTI in Meas Gap Period.\n",
                __func__,( currentGlobalTTITickCount + ulDelay));
        
        for(; newBundleIndex < (TTI_BUNDLE_SIZE-1) ; newBundleIndex++ )
        {
            /* TTI_BUNDLING: If three consecutive bundled tick is in measurement gap
             * then don't schedule bundled UE*/

            if (FALSE == isTTIInMeasGapPeriod(ulUEContext_p,( currentGlobalTTITickCount
                            + ulDelay + newBundleIndex + FDD_HARQ_OFFSET)))
            {
                retval=MAC_SUCCESS;

                LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy][%s] [%d] TTI in Meas Gap Period.\n",
                        __func__,( currentGlobalTTITickCount + ulDelay + newBundleIndex + FDD_HARQ_OFFSET));
                break;
            }
        }
    }
    return retval;
}


/*****************************************************************************
 *  Function Name  : processTtiBHarqCheck
 *  Inputs         : currentGlobalTTITickCount : current Global Tick,
 *                   ulDelay - The amount of delay expected in the uplink path,
 *                   ulUEContext_p - Pointer to UL UE Context.
 *  Outputs        : HARQ Process Free of UE
 *  Returns        : MAC_SUCCESS/MAC_FAILURE
 *  Description    : This function will check whether HARQ process is free for
 *                   bundled transmission of UE or not.
 * ******************************************************************************/
/* SPR 15909 fix start */
MacRetType processTtiBHarqCheck(tickType_t currentGlobalTTITickCount,UInt32 ulDelay,ULUEContext *ulUEContext_p)
    /* SPR 15909 fix end */
{

    UInt32 bundleTick = 0;
    UInt32 bundleIndex = 0;
    UInt8 spsTick      = 0;

    /* SPR 19031 fix start */
    if(currentGlobalTTITickCount <= (ulUEContext_p->aperiodicCQIWithoutDataTTI +
                                        TTIB_FIRST_TO_LAST_TICK_OFFSET))
    {
        /* If grant for Aperiodic CQI without data is processed within previous 4 ticks,
         * the next bundled packet should be scheduled after 4 ticks to avoid same 
         * Harq Id allocation for AP CQI grant and the next bundled Data */
        return   MAC_FAILURE;
    }
    /* SPR 19031 fix end */

    for (bundleTick = 0 ; bundleTick < TTI_BUNDLE_SIZE; bundleTick++)
    {
        bundleIndex = (currentGlobalTTITickCount + ulDelay + bundleTick + FDD_DATA_OFFSET) %
            TTI_BUNDLING_RTT ;
        if( TTI_MAP_FREE != ulUEContext_p->ttiHarqMap[bundleIndex] )
        {
            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy][%s] Harq Process is not free for bundled UE in next 3 ticks [%d].\n",
                    __func__, bundleIndex);
            return   MAC_FAILURE;
        }
        /* checking if current tick is SPS occasion and for TTIB UE ,
         * one nedd to check for 4 ticks as bundle can't overlap*/
        spsTick = (currentGlobalTTITickCount + bundleTick + ulDelay ) % MAX_SPS_INTERVAL;
        if(( SPS_ACTIVATED == ulUEContext_p->spsUlInfo.spsState)
            || ( SPS_ACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState)
            || ( SPS_DEACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState)
          || ( SPS_DEACTIVATED == ulUEContext_p->spsUlInfo.spsState))
        {
            /*++ SPR 19271 */
            if( (spsTick ==( ulUEContext_p->spsUlInfo.actTtiIndex)) ||
                ( (SPS_INTERVAL_20 == spsIntervalMap_g[ulUEContext_p->spsUlInfo.spsUlSetupInfo_p->semiPersistentSchedIntervalUL] ) &&
                  ( spsTick == ((ulUEContext_p->spsUlInfo.actTtiIndex + SPS_INTERVAL_20 ) % MAX_SPS_INTERVAL) ) ) )
            /*-- SPR 19271 */
            {
                return   MAC_FAILURE;
            }
            if( (SPS_DEACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState ) &&
                    (spsTick == (ulUEContext_p->spsUlInfo.ttiIndex)  % MAX_SPS_INTERVAL))
            {
                return MAC_FAILURE ;
            } 
        }

    }
    return MAC_SUCCESS;
}
/*****************************************************************************
 *  Function Name  : processTtiBundleUes
 *  Inputs         : currentGlobalTTITickCount : current Global Tick,
 *                   ulDelay - The amount of delay expected in the uplink path,
 *                   numberOfAvailableRBs_p:  pointer to current available RBs,
 *                   maxUEsToBeScheduledUL_p:  pointer to the max UE to be,
 *                   internalCellIndex - cell index used at MAC.
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This function will deduct no of RBs and UEs as per the nodes present in
 *                   ttibScheduleQueue_g in current tick.
 ******************************************************************************/
                        /* SPR 15909 fix start */
void processTtiBundleUes(tickType_t currentGlobalTTITickCount,
                        /* SPR 15909 fix end */
                         UInt32 ulDelay,    
                         UInt32 * numberOfAvailableRBs_p,
                         UInt8 * maxUEsToBeScheduledUL_p,
                         /*CA Changes start  */
                         InternalCellIndex internalCellIndex)
                         /*CA Changes end  */
{
    UInt8                   bundleQueueIndex    = 0;
    UInt8                   numOfUePerTti       = 0;
    TtiBScheduleNode*       ttibNode_p          =PNULL;
    UInt16                  ueIndex             = 0;
    UInt8                   harqId              = 0;
    ULUEContext*            ulUEContext_p       = PNULL;
    UInt8                   numOfUsedRb         = 0;
    UInt8                   numOfUsedUe         = 0;

    bundleQueueIndex = ( currentGlobalTTITickCount + ulDelay ) % MAX_TTIB_SCHEDULE_QUEUE;
    /*CA Changes start  */
    numOfUePerTti = COUNT_TTIB_SCHEDULE_Q((ttibScheduleQueue_gp[internalCellIndex] + bundleQueueIndex));
    /*CA Changes end  */
    while (numOfUePerTti-- )
    {
        /*CA Changes start  */
        GET_NODE_TTIB_SCHEDULE_Q((ttibScheduleQueue_gp[internalCellIndex] + bundleQueueIndex),TtiBScheduleNode,(void **)&ttibNode_p);
        /*CA Changes end  */
        ueIndex = ttibNode_p->ueIndex;
/* TTIBug1_Fix1 Start */
        if(ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
        {
           continue;   
        }
/* TTIBug1_Fix1 End*/
        harqId = ttibNode_p->harqProcessId;
        ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
             
        if ( ttibNode_p->isSpsEnable )
        {
            continue;    
        }
        if( TRUE == ulUEContext_p->measGapContext.isMeasGapPeriodOn )
        {    
            numOfUsedUe++;
            continue;
        }
        numOfUsedRb = numOfUsedRb + ulUEContext_p->ulresInfo[harqId].riLength;
        numOfUsedUe++;
    }

    if ( *maxUEsToBeScheduledUL_p > numOfUsedUe)
    {
        *maxUEsToBeScheduledUL_p -= numOfUsedUe;
    }
    else
    {
        *maxUEsToBeScheduledUL_p = 0;
    }

    if ( numberOfAvailableRBs_p[CC_CE_REGION] > numOfUsedRb )
    {
        numberOfAvailableRBs_p[CC_CE_REGION] -= numOfUsedRb;

        if((ulUEContext_p != PNULL) &&(CE_USER == ulUEContext_p->userLocationType))
        {
            if(numberOfAvailableRBs_p[CE_REGION] > numOfUsedRb)
            {
                numberOfAvailableRBs_p[CE_USER] -=numOfUsedRb;
            }
            else
            {
                numberOfAvailableRBs_p[CE_USER] =0;
            }
        }
    }
    else 
    {
        numberOfAvailableRBs_p[CC_CE_REGION] = 0;
        numberOfAvailableRBs_p[CE_USER] =0;
    }
}
/*****************************************************************************
 *  Function Name  : getRequiredRBForTtiBundling
 *  Inputs         : currentGlobalTTITickCount : current Global Tick,
 *                   ulDelay - The amount of delay expected in the uplink path,
 *                   internalCellIndex - cell index used at MAC.
 *  Outputs        : Calculate maximum number of RB available for TTI BUndling 
 *  Returns        : Max Num RB For TTI BUNDLING
 *  Description    : Calculate the Maxumum number RBs available for TTI
 *                   Bundling after deducting RBs for RACH and SPS.
 ******************************************************************************/
/* SPR 15909 fix start */
UInt32 getRequiredRBForTtiBundling(tickType_t currentGlobalTTITickCount,
/* SPR 15909 fix end */        
                                  UInt32 ulDelay, InternalCellIndex internalCellIndex)
{
    UInt32                           bundleTick                              = 0;
    UInt32                           bundleIndex                             = 0;
    UInt32                           rachRB                                  = 0;
    UInt32                           numberOfAvailableRBs[MAX_USER_LOCATION] = {0,0};
    UInt32                           SF                                      = 0;
    UInt32                           SFN                                     = 0;
    UInt32                           prachFrequencyOffset                    = 0;
    UInt32                           totalRB                                 = 0;
    UInt32                           maxAvailRB                              = 0;
    LP_SpsULStrIntervalTTIContext    spsUlStrIntervalTTI_p                   = PNULL; 

    for ( bundleTick = 1 ; bundleTick < TTI_BUNDLE_SIZE;bundleTick++ )
    {
        totalRB = 0;
        numberOfAvailableRBs[CC_CE_REGION] = MAX_ADVANCE_UL_SUBFRAMES_ALLOC *
            /*CA Changes start  */
            (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->totalAvailableRB;
        /*CA Changes end  */

        SF = (currentGlobalTTITickCount + ulDelay + bundleTick + FDD_DATA_OFFSET ) % MAX_SUB_FRAME ;

        SFN = MODULO_ONEZEROTWOFOUR(
                (currentGlobalTTITickCount + ulDelay + bundleTick + FDD_DATA_OFFSET )/MAX_SUB_FRAME); 

        bundleIndex = (currentGlobalTTITickCount + ulDelay + bundleTick) % MAX_SPS_INTERVAL;

        UInt32 rbMapNum = MODULO_SIXTEEN(
                currentGlobalTTITickCount + ulDelay + bundleTick + FDD_HARQ_OFFSET);

        if(TRUE == cellSpecificParams_g.\
                cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps)
        {
            spsUlStrIntervalTTI_p = spsULStrTTIMap_gp[internalCellIndex] + bundleIndex;
            totalRB = spsUlStrIntervalTTI_p->numULSpsRbsPerTti;
        }

        /* Check for first SF only for RACH */
        if ( isValidFrameForPRACH( SF, SFN, &prachFrequencyOffset, 
                    &rachRB, internalCellIndex))
        {
            totalRB = totalRB + rachRB;
        }
        /* Check for other SFs in case of Preamble format other that 0 */
        else if (RET_TRUE == isPRACHResourcesAllocated( rbMapNum,
                    internalCellIndex ) )
        {
            totalRB += NUM_PRACH_PHYSICAL_RESOURCES;
        }

        maxAvailRB = ( maxAvailRB > totalRB) ? maxAvailRB : totalRB;
    }
    return (numberOfAvailableRBs[CC_CE_REGION] - maxAvailRB);
}
#endif
/*****************************************************************************
 * Function Name    : processULSchedulerStrategy 
 * Inputs           : currentGlobalTTITickCount : current Global Tick,
 *                    ulDelay - The amount of delay expected in 
 *                              the UpLink path,
 *                    internalCellIndex - cell index used at MAC.
 *                    execArgs - Execution Legs Arguments
 *                    ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs          : UL Strategy Output Nodes are passed to PDCCH Module
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : Uplink Scheduler Strategy will allocate the Uplink RBs to 
 *                    UE for one or more than one sub-frame. This will be decided
 *                    depending upon the number of sub-frames in advance for 
 *                    which strategy is run to schedule allocations and depending
 *                    upon available BSR load in uplink.The UL strategy will also
 *                    consider the other UL Grant Request i.e. SR Request, 
 *                    Contention Resolution and for Apeiodic CQI reports.
 *                    The strategy uses either the proportional fair or round
 *                    robin algorithm to distribute the RBs.
 *****************************************************************************/
UInt32 processULSchedulerStrategy(
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        InternalCellIndex  internalCellIndex,
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    UInt32 numUEsInNackQueue = 0;
    UInt32 numberOfAvailableRBs[MAX_USER_LOCATION] = {0,0};
    /* SPR 5798 Start */
    UInt32 prachFrequencyOffset = 0;
    UInt32 numPhysicalResources = 0;
    UInt32 ulStrategyFnIndex = 0;

   /*EICIC +*/
    UInt8  absSubFrameNum = 0;
    UInt8  strategyScanCount  = 0;
    UInt32 ulSubFrameNumTemp = 0;
    UInt32 ulSysFrameNumTemp = 0;
    UInt8 numBitsAbsPattern = 0;
    /* EICIC -*/

#ifdef FDD_CONFIG
    UInt32 ulSubFrameNum = 
        ( currentGlobalTTITickCount + ulDelay + FDD_DATA_OFFSET ) % 
        MAX_SUB_FRAME;
    UInt32 ulSysFrameNum = MODULO_ONEZEROTWOFOUR(
            ( currentGlobalTTITickCount + ulDelay + FDD_DATA_OFFSET ) / 
            MAX_SUB_FRAME);
    UInt32 rbMapNum = MODULO_SIXTEEN(
            currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
    /* TDD Config 0 Changes Start */
    UInt8  subFrame                 = 0;
    UInt8  nackQContainerTick       = 0;
    UplinkContainer* ulContainerQueue_p    = PNULL;
    /* TDD Config 0 Changes End */
#elif TDD_CONFIG
    /* TDD Config 0 Changes Start */
    /* SPR 23393 Changes Start */
    UInt32 subFrameNum       = 0;
    /* SPR 23393 Changes End */
    UInt32 sysFrameNum      = 0;
    UInt32 ulSysFrameNum = 0;
    UInt32 ulSubFrameOffSet = 0;
    /* SPR 23393 Changes Start */
    GET_SFN_SF_FROM_TTI((currentGlobalTTITickCount + ulDelay ),sysFrameNum,subFrameNum);
    /* SPR 23393 Changes End */
    if( ulSubFrameNum < subFrameNum )
    {
        ulSysFrameNum = (sysFrameNum + 1 ) % (MAX_SYS_FRAME_NUM + 1);
    }
    else
    {
        ulSysFrameNum = sysFrameNum;
    }

    /* Get the ulSubFrameOffSet from current subframe num */
    GET_UL_SUBFRAME_OFFSET(ulSubFrameOffSet,subFrameNum,ulSubFrameNum)

    UInt32 rbMapNum = ( currentGlobalTTITickCount +
            ulSubFrameOffSet ) %  getMaxULRBMaps(internalCellIndex);
    /* TDD Config 0 Changes End */
#endif
    UInt8 msg3ScheduledCount = 0;
    
    ulScheduleInfo_gp[internalCellIndex]->numOfUe = 0;
    ULSchedulerStrategyConfig *ulSchdConfig_p = & cellSpecificParams_g.
        cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig;
    UInt8 maxUEsToBeScheduledUL = ulSchdConfig_p->maxUEsToBeScheduledUL;

    maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] = ulSchdConfig_p->maxCellEdgeUEsToBeScheduledUL;
#ifdef OVERSHOOT_CORRECTION
    modifiedNoOfUEsAfterOverLoadUL(&maxUEsToBeScheduledUL,internalCellIndex);
#endif

    /* SPR 5798 Start */
#ifdef FDD_CONFIG
    msg3ScheduledCount = rachGlobals_gp[internalCellIndex]->
        msg3ScheduledCount[ulSubFrameNum];
#endif
    /* SPR 5798 End */

    numberOfAvailableRBs[CC_CE_REGION] = MAX_ADVANCE_UL_SUBFRAMES_ALLOC *
        (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->totalAvailableRB;
    numberOfAvailableRBs[CE_REGION]    = MAX_ADVANCE_UL_SUBFRAMES_ALLOC *
        (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB;

    /* Decrement number of available RBs if reserved by SPS */
    numberOfAvailableRBs[CC_CE_REGION] -= numUlSpsRbsInTti_g[internalCellIndex];

    /* Decrement number of available RBs if reserved by PRACH */
    /* Check if this SF frame is valid for PRACH resource allocation.
     * In case of Preamble format other than 0, if its resources are not
     * reserved in advance as the current tick is not valid frame for PRACH */
    if ( isValidFrameForPRACH( ulSubFrameNum, ulSysFrameNum , 
                &prachFrequencyOffset, &numPhysicalResources, 
                internalCellIndex ) || 
            ( RET_TRUE == isPRACHResourcesAllocated( rbMapNum, 
                                                     internalCellIndex ) ) )
    {
#ifdef TDD_CONFIG
        if( 0 == numPhysicalResources )
        {
            RbMapNode *rbMapNode_p = 
                rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
            numPhysicalResources = rbMapNode_p->allocatedPrachNumRbs; 
        }
#elif FDD_CONFIG
        numPhysicalResources = NUM_PRACH_PHYSICAL_RESOURCES;
#endif

        if(numPhysicalResources > numberOfAvailableRBs[CC_CE_REGION])
        {
            numberOfAvailableRBs[CC_CE_REGION] = 0;
        }
        else
        {
            numberOfAvailableRBs[CC_CE_REGION] -= numPhysicalResources;
        }
    }

#ifdef FDD_CONFIG
    processTtiBundleUes(currentGlobalTTITickCount,
            ulDelay,
            numberOfAvailableRBs,
            &maxUEsToBeScheduledUL,
            internalCellIndex);
    availableRBForTtiBundlingPerTick_g[internalCellIndex] = getRequiredRBForTtiBundling(
            currentGlobalTTITickCount,
            ulDelay ,
            internalCellIndex);
#endif
/* +- SPR 17777 */
#ifdef FDD_CONFIG
    subFrame           = (currentGlobalTTITickCount + ulDelay)%  MAX_SUB_FRAME;
#endif

    /* SPR 5798 Start */
#ifdef TDD_CONFIG
    /* TDD Config 0 Changes Start */
    UInt32 msg3CountContainer = (currentGlobalTTITickCount + ulSubFrameNum) % MAX_NUMBER_OF_CONTAINER;
    /* TDD Config 0 Changes End */
    msg3ScheduledCount = rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer];
#endif
    /* SPR 5798 End */

    /* EICIC +*/
    /* EICIC Optimization +- */
    if(isEicicEnabled_g[internalCellIndex])
    {

    getSFAndSFN(&ulSysFrameNumTemp, &ulSubFrameNumTemp,internalCellIndex);

#ifdef TDD_CONFIG
    ulSysFrameNumTemp  = sysFrameNum;
	ulSubFrameNumTemp = subFrameNum;
#else
    ulSubFrameNumTemp = ulSubFrameNumTemp + ulDelay;

    if(ulSubFrameNumTemp >= MAX_SUB_FRAME)
    {
        ulSubFrameNumTemp -= MAX_SUB_FRAME;
        ulSysFrameNumTemp++;
        if (ulSysFrameNumTemp == MAX_SFN_VALUE)
        {
            ulSysFrameNumTemp = 0;
        }
    }
#endif
   /*Complexity Start*/ 
   numBitsAbsPattern=eicicFindNumOfBitsinABSPattern(internalCellIndex);
	    /*Complexity End*/
   absSubFrameNum = ((((ulSysFrameNumTemp )*MAX_SUB_FRAME) +(ulSubFrameNumTemp )) %numBitsAbsPattern) ;




  if ((LTE_TRUE == absPattern_g[internalCellIndex][absPatternIndex_g[internalCellIndex]][absSubFrameNum]) && 
            (isEicicEnabled_g[internalCellIndex])) /* Work as aggressor*/
  {
      ueTypeToScheduleUL_g[internalCellIndex] =  INVALID_UE_TYPE; 
      maxUEsToBeScheduledUL = ZERO;
  }
 else if ((LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) && 
              (isEicicEnabled_g[internalCellIndex])) /* Work as victim*/
  {
      ueTypeToScheduleUL_g[internalCellIndex] = VICTIM_UE;
  }
  else
  {
      ueTypeToScheduleUL_g[internalCellIndex] = NON_VICTIM_UE; /* By default if No ABS is configured then schedule NON victim UEs */
  }
    }
    else
    {
	    ueTypeToScheduleUL_g[internalCellIndex] = NON_VICTIM_UE;
    }
    /* EICIC Optimization +- */
/* EICIC -*/


    /* subtract the Retransmitted bytes from available RBs */
    /* ICIC changes start */
    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    if ( numberOfAvailableRBs[CC_CE_REGION] >
        (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->totalRetxRb[CC_CE_REGION]
        )
    {    
        numberOfAvailableRBs[CC_CE_REGION] -=
        (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->totalRetxRb[CC_CE_REGION];
        numberOfAvailableRBs[CE_REGION]    -= 
        (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->totalRetxRb[CE_REGION];
    }
#else
    if ( numberOfAvailableRBs[CC_CE_REGION] >
        totalRetxRb_g[internalCellIndex][CC_CE_REGION][subFrame]
        )
    {    
        numberOfAvailableRBs[CC_CE_REGION] -=
        totalRetxRb_g[internalCellIndex][CC_CE_REGION][subFrame]; /* TX_RETX */
        numberOfAvailableRBs[CE_REGION]    -= 
        totalRetxRb_g[internalCellIndex][CE_REGION][subFrame];    /* TX_RETX */
    }
#endif
    /* TDD Config 0 Changes End */
    else 
    {
        numberOfAvailableRBs[CC_CE_REGION] = 0;
    }    
    if (numberOfAvailableRBs[CC_CE_REGION] < numberOfAvailableRBs[CE_REGION])
    {
        /*SPR_8630_CHANGES_START*/
        LOG_UT (MAC_UL_CE_RESOURCE_GT_CC_CE_RESOURCE, LOGWARNING,
                MAC_UL_Strategy, getCurrentTick (),
                numberOfAvailableRBs[CC_CE_REGION],numberOfAvailableRBs[CE_REGION],
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
                "");
        /*SPR_8630_CHANGES_END*/
        numberOfAvailableRBs[CE_REGION] = numberOfAvailableRBs[CC_CE_REGION];
    }
    /* ICIC changes end */
    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
    /*CA Changes start  */
    totalRetxRb_g[internalCellIndex][CC_CE_REGION][subFrame] = 0;
    totalRetxRb_g[internalCellIndex][CE_REGION][subFrame]    = 0;
    /*CA Changes end  */
#endif
    /* TDD Config 0 Changes End */

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
            "[%s]: Total '%d' No.Of RBs, Cell Edge RB's '%d' to be Distribute by UL Strategy in \
            MAX ADVANCE SUBFRAME %d\n",__func__,numberOfAvailableRBs[CC_CE_REGION],
            numberOfAvailableRBs[CE_REGION], MAX_ADVANCE_UL_SUBFRAMES_ALLOC );

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
            "[%s]: Reference Flag Value is: %d\n",__func__,ulReferenceFlag_g[internalCellIndex]);

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy][%s]"
            ": Processing ZeroNonZeroBSRTriggerQueue\n", __func__);

    /* CA Stats Changes Start */
    /*debug stats*/
    LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_ABSOLUTE_TTI(currentGlobalTTITickCount, 
            internalCellIndex);
    /* Getting Container Tick*/
    /* CA Stats Changes End */
#ifdef FDD_CONFIG
    nackQContainerTick = (currentGlobalTTITickCount + ulDelay - FDD_HARQ_OFFSET) & (MAX_CONTAINER_NUM - 1);
    /*CA Changes start  */
    ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;
    /*CA Changes end  */
    /* SPR 5798 Fix Start */
    numUEsInNackQueue  = ulContainerQueue_p->nackQueueCount;
    /* SPR 5798 Fix End */
    /* TDD Config 0 Changes Start */
#elif TDD_CONFIG
    numUEsInNackQueue =  (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->nackQueueCount; 
    /* TDD Config 0 Changes End */
#endif

        /* SPR 5798 Start */
    if(maxUEsToBeScheduledUL > (msg3ScheduledCount + numUEsInNackQueue))
    {
        maxUEsToBeScheduledUL -= (msg3ScheduledCount + numUEsInNackQueue);
    }
    else
    {
        maxUEsToBeScheduledUL =0;
    }
    /* SPR 5798 End */

    do
    {
        /* To Process the ZeroNonZeroBSRTriggerQueue_g Queue and Update the
           NonZeroBsr List,After adding and deleting UEs according to Trigger*/

        if(ulSchdConfig_p->ulSchedulerStrategy == UL_QOS_SCHEDULING)
        {
            /* SPR 10824 Changes Start */
            /*
             ** When UL Strategy is executed for first ulsubframe
             ** in special subframe for TDD Configuration 0, ulQosProcessTokenUpdate()
             ** is invoked twice to update the token for both ulsubframe and
             ** ulQosProcessTokenUpdate() is not invoked whne UL strategy is
             ** executed for second ulsbuframe.
             **
             ** Otherwise, ulQosProcessTokenUpdate() is invoked only once.
             */
#ifdef TDD_CONFIG
/*SPR 22919 Start*/
            /*CA Changes start  */
            /* + SPS_TDD_CHANGES */
            if ((LTE_TRUE != strategyScanCount) && (TRUE != cellSpecificParams_g.\
                        cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps))
                /* - SPS_TDD_CHANGES */
            {
                ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount = 0;
            }
            /*CA Changes end  */
            /* Update the UL tokens for the UE's */
            if (LTE_FALSE == ((LTE_TRUE == isTddConfig0SsubFrame(subFrameNum,internalCellIndex)) &&
                        ((ulSubFrameNum == SUB_FRAME_8) || (ulSubFrameNum == SUB_FRAME_3))))
            {
                /* +- SPR 17777 */
                ulQosProcessTokenUpdate(internalCellIndex,execArgs);
                /* +- SPR 17777 */
            }
            if ((LTE_TRUE == isTddConfig0SsubFrame(subFrameNum,internalCellIndex)) &&
                    ((ulSubFrameNum == SUB_FRAME_7) || (ulSubFrameNum == SUB_FRAME_2)))
            {
                /* +- SPR 17777 */
                ulQosProcessTokenUpdate(internalCellIndex,execArgs);
                /* +- SPR 17777 */
            }
/*SPR 22919 End*/
#elif FDD_CONFIG
            /* Update the UL tokens for the UE's */
            /* +- SPR 17777 */
            ulQosProcessTokenUpdate(internalCellIndex,execArgs);
            /* +- SPR 17777 */
#endif

            /*
             *   Process the DXR trigger queue. 
             *   For each UE, remove the lc.s from the priority tree
             */
            /*CA Changes start  */
            ulQosStrategyProcessDRXTrigQueue(internalCellIndex, execArgs);
            /*CA Changes start  */

            /*
             *   Process the Measurement Gap trigger Queue. 
             *   For each UE, remove the lc.s from the priority tree
             */
            /*CA Changes start  */
            ulQosStrategyProcessMeasGapTrigQueue(internalCellIndex, execArgs);
            /*CA Changes start  */
            /*
             *   Process the PDCCH Order Queue. 
             *   For each UE, remove the lc.s from the priority tree
             */
            /*CA Changes start  */
            ulQosStrategyProcessPdcchOrderQueue(internalCellIndex, execArgs);
            /*CA Changes start  */
            /* SPR 22228 fix start */
            /* redundant code removed */
            /* this functionality is already taken care in function ElProcessUlSpsStrategy */
            /* SPR 22228 fix end */
            /* SPR 22275 Fix + */
            initUlQosRbRestriction(internalCellIndex);
            /* SPR 22275 Fix - */
        }
        else
        {
            /* SPR 22228 fix start */
            /* redundant code removed */
            /* this functionality is already taken care in function ElProcessUlSpsStrategy */
            /* SPR 22228 fix end */
            /* DRX_CHG */
            /*CA Changes start  */
            /* +- SPR 17777 */
            processDRXOnOffTriggerQueueUL(internalCellIndex);
            /* DRX_CHG */
            /* MEAS_GAP_CHG */
            processMeasGapOnOffTriggerQueueUL(internalCellIndex);
            /* MEAS_GAP_CHG */
            processSyncLossOnOffTriggerQueueUL(internalCellIndex);
            /* +- SPR 17777 */
            /*CA Changes end  */
        }


        /* TTI Bundling Changes Function Pointer Start */
        for(ulStrategyFnIndex=0; (ulStrategyFnIndex < MAX_UL_STRATEGY_Q) &&
                (!(memoryOverflow_g || msgOverflow_g) &&

                 /* CA Phase2 CSI changes*/ /* maxUEsToBeScheduledUL*/ 
                 /* Note- As part of CA Phase2 CSI changes, this check of
                  * maxUEsToBeScheduledUL is removed because if multiple entries of
                  * same ue are present in aperiodic cqi queue, one for Pcell and other for Scell, then
                  * we have to club the two nodes into one strategy node for CA QOS
                  * scheduling*/
                 numberOfAvailableRBs[CC_CE_REGION]); ulStrategyFnIndex++)
        {
            ulStrategyQueueProcessfn_p[ulStrategyFnIndex](numberOfAvailableRBs,
                    &maxUEsToBeScheduledUL,
                    currentGlobalTTITickCount,
                    ulDelay,              
                    /*CA Changes start  */
                    internalCellIndex
                    /*CA Changes end  */
                    ,execArgs
                    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                    ,ulSubFrameNum
#endif
                    /* TDD Config 0 Changes End */
                    );
        }
        /* TTI Bundling Changes Function Pointer End */

        /* EICIC +*/
        strategyScanCount = 0;
	/* EICIC Optimization +- */
	if(isEicicEnabled_g[internalCellIndex])
	{
        if (( numberOfAvailableRBs[CC_CE_REGION]) && (maxUEsToBeScheduledUL) && (VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex])
                && (redistributeRb_g))
        {
            strategyScanCount = 1;
            ueTypeToScheduleUL_g[internalCellIndex] = NON_VICTIM_UE;

        }
	}
	/* EICIC Optimization +- */
        /* EICIC -*/

    }while((strategyScanCount)); /* EICIC change*/


    /* SPR 4645 fix start */
    if(ulSchdConfig_p->ulSchedulerStrategy == UL_QOS_SCHEDULING)
        /* CA Changes end */
    {

        /*CA Changes start  */
        ulQosUpdateStrategyNodes(internalCellIndex
                /*CA Changes end  */
#ifdef TDD_CONFIG
                /* +- SPR 17777 */
                ,ulSubFrameNum,
                /* +- SPR 17777 */
                currentGlobalTTITickCount,
                execArgs
#endif
                );
        /* TDD Config 0 Changes End */
    }
    /* SPR 4645 fix end */

    /* ICIC changes start */
    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->ulAvailRB.totalRBs = 
        numberOfAvailableRBs[CC_CE_REGION];
    (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->ulAvailRB.cellEdgeRBs = 
        numberOfAvailableRBs[CE_REGION];
#else
    /*CA Changes start  */
    ulBufferPool_g[internalCellIndex].totalRBs    = numberOfAvailableRBs[CC_CE_REGION];
    ulBufferPool_g[internalCellIndex].cellEdgeRBs = numberOfAvailableRBs[CE_REGION];
    /*CA Changes end  */
#endif
    /* TDD Config 0 Changes End */
    /* ICIC changes end */

    /* ulReferenceFlag will be used as reference Flag,which identifies the UE
       which have got the RBs in the same subframe and preventing the
       repetition of distribition of RBs to the same UE.
       The Flag is using a MODULO by 2 to change it every time strategy runs*/
    ulReferenceFlag_g[internalCellIndex] = currentGlobalTTITickCount; 

#ifdef DL_UL_SPLIT
    ULStrategyTxNode *ulStrategyTxNode_p = PNULL;

    /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
    GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
    GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
            sizeof(ULStrategyTxNode),PNULL);
#endif
    /*cov_warning 26414 fix start*/
    if(PNULL == ulStrategyTxNode_p)
    {
        ltePanic("Memory Allocation failed in %s\n",__func__);
        return MAC_FAILURE;
    }
    /*cov warning 26414 fix end */
    ulStrategyTxNode_p->ueIndex     = INVALID_UE_INDEX;
    ulStrategyTxNode_p->requiredRB  = 0;
    ulStrategyTxNode_p->strictAllocRB = FALSE;
    ulStrategyTxNode_p->ulGrantRequestType = NEW_ULGRANT_REQ_MSG;
    ulStrategyTxNode_p->ttiCounter = 0;
#ifdef TDD_CONFIG
    ulStrategyTxNode_p->ulHarqProcessId = INVALID_HARQ_ID;
#endif
    /* ULA_CHG */
    ulStrategyTxNode_p->minDataSize = 0;
    ulStrategyTxNode_p->maxDataSize = 0;
    ulStrategyTxNode_p->avgMcs = INVALID_MCS_INDEX;
    /* ULA_CHG */
    // SPLIT GC +  : ICC (Inter core communication ) Sending to DL Core
    /*CA Changes start  */
    putEntryInULSchedulerNewTxQueueUsingICC(ulStrategyTxNode_p, internalCellIndex);
    /*CA Changes end  */
#endif
#ifdef FDD_CONFIG
    ulContainerQueue_p->nackQueueCount=0;
#endif

    return MAC_SUCCESS;
}


/***************************************************************************************
 * Function Name  : processSrULGrantReqMsg
 * Inputs         : ulGrantPendingQueueCount,
 *                  numberOfAvailableRBs_p,
 *                  currentGlobalTTITickCount,
 *                  ulDelay - UL Delay,
 *                  tempUplinkContext_p - Pointer to ULUEContext,
 *                  pendNode_p - Pointer to ULGrantRequestQueueNode,
 *                  ulStrategyTxNode_pp - Pointer to ULStrategyTxNode,
 *                  tempMaxUEsToBeScheduledUL_p - Maximum No. of UE's to be scheduled,
 *                  ulHarqProcessId - Uplink HARQ Process ID,
 *                  internalCellIndex - cell index used at MAC.
 *                  ulSubFrameNum - Uplink subframe number 
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function processes the Sr UL Grant request message.
 ***************************************************************************************/
MacRetType processSrULGrantReqMsg( UInt32* ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p,
        ULStrategyTxNode **ulStrategyTxNode_pp,
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex 
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* SPR 15909 fix start */
    tickType_t ttiTick                      = 0;
    /* SPR 15909 fix end */
    MeasGapContextUL * measGapContext_p     = PNULL;
    UInt8 avgMcs                            = 0;
    UInt32 tempUeId                         = 0;
    UInt8 minRb                             = 0;
    UInt8 userLocationType = CC_USER ;
    /* Coverity 30671 */
    ULGrantRequestQueueNode *pendingNode_p  = *pendNode_p;
    /* Coverity 30671 */
    ttiTick =  pendingNode_p->ttiCounter;
    measGapContext_p = &tempUplinkContext_p->measGapContext;
    avgMcs = tempUplinkContext_p->mcsIndex;
    tempUeId = pendingNode_p->ueIndex;
    userLocationType = tempUplinkContext_p->userLocationType ;
    UInt8 retValue = 0;
    ULStrategyTxNode *ulStrategyTxNode_p = *ulStrategyTxNode_pp;
    /* Phase2_CSI_Strategy_Changes start */
    if(0 == *tempMaxUEsToBeScheduledUL_p)
    {
            /* COVERITY CID 63531 FIX START */
            if ( !ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                    &(pendingNode_p->
                        ulGrantRequestNodeAnchor)) )
            {
                FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);
            }
            /* COVERITY CID 63531 FIX END */
            (*ulGrantPendingQueueCount)--;
            *ulStrategyTxNode_pp = ulStrategyTxNode_p;
            return MAC_FAILURE;

    }
    /* Phase2_CSI_Strategy_Changes end */

    LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
            "[UL-Strategy][%s]SR Request Received: \
            SR Queue Cnt:%d Glbl TTI:%d,PhyTTi:%d \
            SR Periodicity:%d \n",__func__ ,
            ulGrantPendingQueueCount,
            currentGlobalTTITickCount,ttiTick ,
            tempUplinkContext_p->pucchConfigInfo.
            srPeriodicity);

    /*If the SR Periodicty Timer is expired,
      delete that SR Request*/
    if (((currentGlobalTTITickCount - ttiTick + 
                    ulDelay  >=
                    (tempUplinkContext_p->
                     pucchConfigInfo.srPeriodicity))))
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,
                UL_STRATEGY,
                "[UL-Strategy][%s]SR_Periodicty expired\
                deleting SR\n" ,__func__);
        /*Freeing the SR Node*/
        freeMemPool((void *)pendingNode_p);
        pendingNode_p = PNULL; 
    }
    /*If SR Periodicity timer is not Expired
      Then Send SR Response*/
    else
    {
        /* MEAS_GAP_CHG */
        if ((TRUE == measGapContext_p->
                    isMeasGapPeriodOn) ||
                (UL_SYNC_LOSS_ON ==
                 tempUplinkContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState))
        {

            /* SPR 2377 changes start */
            LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_UL_LOG_ID 
                    ,LOGDEBUG, MAC_UL_Strategy, 
                    currentGlobalTTITickCount, 
                    tempUeId, 
                    tempUplinkContext_p->drxCurrentState,
                    measGapContext_p->isMeasGapPeriodOn,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE, 
                    DEFAULT_FLOAT_VALUE,
                    __func__,"");

            /* SPR 2377 changes end */

	    if (tempUplinkContext_p->bsrNetTotal)
	    {
		    FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);
	    }
	    else
	    {
		    if(!ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
					    ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor)))
		    {
			    FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);
		    }
	    }




            /*Cov_fix_6May_63531_Stop*/
            (*ulGrantPendingQueueCount)--;
            *ulStrategyTxNode_pp = ulStrategyTxNode_p;
            return MAC_FAILURE;
        }
        /* MEAS_GAP_CHG */

        /* ULA_CHG */
        /* SPR 5713 changes start */
        /* optimization Start */
        UInt8 minUlGrant = 0;
        if (avgMcs >=  MIN_GRANT_MCS_INDEX )
        {
            minUlGrant = MAC_MIN_UL_GRANT_FOR_SR;
        }
        else
        {
            CALCULATE_MAC_MIN_UL_GRANT_RBs(avgMcs,minRb,tempUplinkContext_p->ueComplianceRelease);
            minUlGrant = minRb;
        }
        if(numberOfAvailableRBs_p[userLocationType] < minUlGrant)
            /* ICIC changes end */
        {
            /*Available no. of Rbs is less so unable to 
              process SR Node hence Pushing into 
              ulGrantPendingQueue_gp[internalCellIndex] */
            /*CA Changes start  */
            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                    /*CA Changes end  */
                    ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
            if (!retValue)
            {
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGWARNING,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        /* SPR_2446 start */
                        pendingNode_p->ueIndex,
                        /* SPR_2446 end */
                        0,0,0,0,0.0,0.0,__func__,
                        "Error in Pushing SR in\
                        ulGrantPendingQueue");
                FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);
            }
            (*ulGrantPendingQueueCount)--;
            *ulStrategyTxNode_pp = ulStrategyTxNode_p;
            return MAC_FAILURE;
        }
        else
        {
            minRb = minUlGrant;
        }
        /* optimization End*/
        if(minRb) 
        {
#ifdef FDD_CONFIG 

            if( tempUplinkContext_p->ttiBundlingEnabled )
            {
                if ( avgMcs > MAX_TTI_BUNDLE_MCS )
                {
                    avgMcs = MAX_TTI_BUNDLE_MCS;
                }
                /*CA Changes start  */
                if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < minRb ) 
                        || (MAC_FAILURE == processMeasGapValidation(
                                tempUplinkContext_p,currentGlobalTTITickCount,ulDelay )))
                {
                    /*coverity id 63531*/
                    if(!ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                                ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor)))
                    {
                        FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);
                    }
                    (*ulGrantPendingQueueCount)--;
                    *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                    return MAC_FAILURE;
                }
                availableRBForTtiBundlingPerTick_g[internalCellIndex] -= minRb;
                LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                        "[%s][%d]:TTIB UE ID %d scheduled RB:%d MCS:%d \n",
                        getCurrentTick(),__func__, ueULContext_p->ueIndex,ulStrategyTxNode_p->requiredRB);
                /*CA Changes end  */
            }
#endif
            /* ULA_CHG */
            /*Preparing the UL Strategy outPut Node*/
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
            GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else            
            GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                    sizeof(ULStrategyTxNode),PNULL);
#endif            
            /*+COVERITY 5.2.2 - 32648*/
            if(PNULL == ulStrategyTxNode_p)
            {
                ltePanic("Memory Allocation failed in %s\n",__func__);
                /* Klockwork warning Changes Start */
                return MAC_FAILURE;
                /* Klockwork warning Changes End */
            }
            ulStrategyTxNode_p->requiredRB = 0;
            ulStrategyTxNode_p->maxDataSize = 0;
            ulStrategyTxNode_p->minDataSize = 0;

            /*-COVERITY 5.2.2 - 32648*/
            (*tempMaxUEsToBeScheduledUL_p)--;
            /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
            tempUplinkContext_p->isAlreadyConsideredFlag
            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
            tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
            = ulReferenceFlag_g[internalCellIndex];
#endif
            /* TDD Config 0 Changes End */

            /* optimization Start */
#ifdef TDD_CONFIG
            PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, (UInt16)tempUeId, minRb, TRUE, SR_ULGRANT_REQ_MSG, 
                    ttiTick, ulHarqProcessId, tempUplinkContext_p->ulGrantMinDataSize, 
                    determineUlTBSize(avgMcs,minRb), avgMcs)
#else
                PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, (UInt16)tempUeId, minRb, TRUE, SR_ULGRANT_REQ_MSG, 
                        ttiTick, tempUplinkContext_p->ulGrantMinDataSize, 
                        determineUlTBSize(avgMcs,minRb), avgMcs)
#endif
                /* optimization End */
                /* ICIC changes start */
                numberOfAvailableRBs_p[userLocationType] -= minRb;

            if (userLocationType == CE_USER)
            {
                /*CA Changes start  */
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                /*CA Changes end  */
                numberOfAvailableRBs_p[CC_CE_REGION] -= minRb;
            }else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                    numberOfAvailableRBs_p[CE_REGION])
            {
                numberOfAvailableRBs_p[CE_REGION] = 
                    numberOfAvailableRBs_p[CC_CE_REGION];
            }
            /* ICIC changes end */
            /* ULA_CHG */
            /*putEntry of ULStrategyTXNode 
              in PDCCH Queue */
            if (!(putEntryInULSchedulerNewTxQueue(
                            /*CA Changes start  */
                            ulStrategyTxNode_p, internalCellIndex
                /*CA Changes end  */
                            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                            ,currentGlobalTTITickCount
                            ,ulSubFrameNum
                            ,tempUplinkContext_p
#endif
                            /* TDD Config 0 Changes End */
                            )))
            {
                /* ULA_CHG */
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGERROR,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        0,0,0,0,0.0,0.0,__func__,
                        "ulStrategyTxNode_p entry failed");
                /* ULA_CHG */
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
                FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else                
                freeMemPool(ulStrategyTxNode_p);
#endif
                /*coverity_530_55390*/
                ulStrategyTxNode_p = PNULL;
                /*coverity_530_55390*/
            }
            /* ULA_CHG */
            else
            {
                LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                        LOGDEBUG,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        ulStrategyTxNode_p->requiredRB,\
                        ulStrategyTxNode_p->strictAllocRB,\
                        ulStrategyTxNode_p->minDataSize,\
                        ulStrategyTxNode_p->maxDataSize,\
                        ulStrategyTxNode_p->avgMcs,\
                        ulStrategyTxNode_p->ttiCounter,\
                        __func__,"ulStrategyTxNode_p parameters");
            }
            /* ULA_CHG */
            freeMemPool((void *)pendingNode_p);
            pendingNode_p = PNULL;
        }
    }
    *ulStrategyTxNode_pp = ulStrategyTxNode_p;
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name  : processCrULGrantReqMsg
 * Inputs         : ulGrantPendingQueueCount,
 *                  numberOfAvailableRBs_p,
 *                  currentGlobalTTITickCount,
 *                  ulDelay - UL Delay,
 *                  tempUplinkContext_p - Pointer to ULUEContext,
 *                  pendNode_p - Pointer to ULGrantRequestQueueNode,
 *                  ulStrategyTxNode_pp - Pointer to ULStrategyTxNode,
 *                  tempMaxUEsToBeScheduledUL_p - Maximum No. of UE's to be scheduled,
 *                  ulHarqProcessId - Uplink HARQ Process ID,
 *                  internalCellIndex - cell index used at MAC.
 *                  ulSubFrameNum - upplink subframe number
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Cr UL Grant request message.
 *****************************************************************************/
MacRetType processCrULGrantReqMsg(UInt32 *ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p,
        ULStrategyTxNode **ulStrategyTxNode_pp,
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        /* +- SPR 17777 */

        InternalCellIndex internalCellIndex
        /*CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* SPR 15909 fix end */
    tickType_t ttiTick                          = 0;
    /* SPR 15909 fix end */
    UInt8 avgMcs                            = 0;
    UInt32 tempUeId                         = 0;
    UInt8 minRb                             = 0;
    /* Coverity 30671 */
    ULGrantRequestQueueNode *pendingNode_p  = *pendNode_p;
    /* Coverity 30671 */
    UInt8 userLocationType = CC_USER ;
    ttiTick =  pendingNode_p->ttiCounter;
    avgMcs = tempUplinkContext_p->mcsIndex;
    tempUeId = pendingNode_p->ueIndex;
    userLocationType = tempUplinkContext_p->userLocationType ;
    UInt8 retValue = 0;
    ULStrategyTxNode *ulStrategyTxNode_p = *ulStrategyTxNode_pp;
    
    if(0 == *tempMaxUEsToBeScheduledUL_p)
    {
        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                ulGrantPendingQueue_gp[internalCellIndex],
                ULGrantRequestQueueNode,
                &(pendingNode_p->ulGrantRequestNodeAnchor));

        if (!retValue)
        {
            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,
                    LOGWARNING,MAC_UL_Strategy,
                    currentGlobalTTITickCount,
                    /* SPR_2446 start */
                    pendingNode_p->ueIndex,
                    /* SPR_2446 end */
                    0,0,0,0,0.0,0.0,__func__,
                    "Error in Pushing CR in\
                    ulGrantPendingQueue");
            FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);
        }
        if(*ulGrantPendingQueueCount)
        {
            (*ulGrantPendingQueueCount)--;
        }
        return MAC_FAILURE;
    }
    LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
            "[UL-Strategy][%s]Contention Resoultion\
            Request Received:Contention Res Queue Cnt:%d \
            Glbl TTI:%d,CR Expiry Tick %d \n",__func__ ,
            /* + SPR 8550 Fix */
            *ulGrantPendingQueueCount,
            /* - SPR 8550 Fix */
            currentGlobalTTITickCount,ttiTick);

    /*If the Contention Resoluion Request is expired,
      delete that CR Request*/
    if (((currentGlobalTTITickCount + ulDelay 
                    >= (ttiTick))))
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                "[UL-Strategy][%s]Contention Resolution \
                Timer is expired... deleting Contention \
                Resolution \n",__func__);
        /*Freeing the CR Node*/
        freeMemPool((void *)pendingNode_p);
        pendingNode_p = PNULL; 
    }
    /*If CR timer is not Expired Then Send 
      Allocation for CR*/
    else
    {
        /* ULA_CHG */
        /* SPR 5713 changes start */
        if (avgMcs >=  MIN_GRANT_MCS_INDEX )
        {
            /* ICIC changes start */
            if(numberOfAvailableRBs_p[userLocationType] <
                    MAC_UL_GRANT_FOR_CONTENTION_RES)
                /* ICIC changes end */
            {
                /*Available no. of Rbs is less so unable to
                  process CR Node hence Pushing into
                  ulGrantPendingQueue */

                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                        /*CA Changes start  */
                        ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                        /*CA Changes end  */
                        &(pendingNode_p->ulGrantRequestNodeAnchor));

                if (!retValue)
                {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            /* SPR_2446 start */
                            pendingNode_p->ueIndex,
                            /* SPR_2446 end */
                            0,0,0,0,0.0,0.0,__func__,
                            "Error in Pushing CR in\
                            ulGrantPendingQueue");
                    FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);
                }
                (*ulGrantPendingQueueCount)--;
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;
            }
            else
            {
                minRb = MAC_UL_GRANT_FOR_CONTENTION_RES;
            }
        }
        else
        {
            CALCULATE_MAC_MIN_UL_GRANT_RBs(avgMcs,minRb,tempUplinkContext_p->ueComplianceRelease);
            /* ICIC changes start */
            if(numberOfAvailableRBs_p[userLocationType] < minRb)
                /* ICIC changes end */
            {
                /*Available no. of Rbs is less so unable to
                  process CR Node hence Pushing into
                  ulGrantPendingQueue */

                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                        ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                        &(pendingNode_p->ulGrantRequestNodeAnchor));

                if (!retValue)
                {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            /* SPR_2446 start */
                            pendingNode_p->ueIndex,
                            /* SPR_2446 end */
                            0,0,0,0,0.0,0.0,__func__,
                            "Error in Pushing CR in\
                            ulGrantPendingQueue");
                    FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);
                }
                (*ulGrantPendingQueueCount)--;
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;
            }
        }
        /* SPR 5713 changes end */

        if(minRb)
        {
            /* ULA_CHG */
            /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
            GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
            GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                    sizeof(ULStrategyTxNode) , PNULL);
#endif
            /*+COVERITY 5.2.2 - 32647*/
            if(PNULL == ulStrategyTxNode_p)
            {
                ltePanic("Memory Allocation failed in %s\n",__func__);
                return MAC_FAILURE ;
            }
            ulStrategyTxNode_p->requiredRB = 0;
            ulStrategyTxNode_p->maxDataSize = 0;
            ulStrategyTxNode_p->minDataSize = 0;

            /*-COVERITY 5.2.2 - 32647*/
            (*tempMaxUEsToBeScheduledUL_p)--;
            /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
            tempUplinkContext_p->isAlreadyConsideredFlag
                = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
            tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                = ulReferenceFlag_g[internalCellIndex];
#endif
            /* TDD Config 0 Changes End */
            ulStrategyTxNode_p->ueIndex =
                (UInt16)tempUeId;
            /* ULA_CHG */
            ulStrategyTxNode_p->requiredRB = minRb;
            /* ULA_CHG */
            ulStrategyTxNode_p->strictAllocRB = TRUE;
            ulStrategyTxNode_p->ulGrantRequestType=
                CR_ULGRANT_REQ_MSG;
            ulStrategyTxNode_p->ttiCounter = ttiTick;
#ifdef TDD_CONFIG
            ulStrategyTxNode_p->ulHarqProcessId =
                ulHarqProcessId;
            /*TDD Config 0 Changes Start*/
            ulStrategyTxNode_p->scheduledCount = 0;
            /*TDD Config 0 Changes End*/
#endif
            /* ULA_CHG */
            ulStrategyTxNode_p->minDataSize =
                /* SPR 5713 changes start */
                tempUplinkContext_p->ulGrantMinDataSize;
            /* SPR 5713 changes end */
            ulStrategyTxNode_p->maxDataSize =
                determineUlTBSize(avgMcs,minRb);
            ulStrategyTxNode_p->avgMcs = avgMcs;
            /* ICIC changes start */
            numberOfAvailableRBs_p[userLocationType] -= minRb;

            if (userLocationType == CE_USER)
            {
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                numberOfAvailableRBs_p[CC_CE_REGION] -= minRb;
            }

            else if (numberOfAvailableRBs_p[CC_CE_REGION] <
                    numberOfAvailableRBs_p[CE_REGION])
            {
                numberOfAvailableRBs_p[CE_REGION] =
                    numberOfAvailableRBs_p[CC_CE_REGION];
            }
            /* ICIC changes end */
            /*putEntry of ULStrategyTXNode in
              PDCCH Queue */
            if (!(putEntryInULSchedulerNewTxQueue(
                            /*CA Changes start  */
                            ulStrategyTxNode_p, internalCellIndex
                            /*CA Changes end  */
                            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                            ,currentGlobalTTITickCount
                            ,ulSubFrameNum
                            ,tempUplinkContext_p
#endif
                            /* TDD Config 0 Changes End */
                            )))
            {
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,
                        LOGERROR,MAC_UL_Strategy,
                        currentGlobalTTITickCount,
                        ulStrategyTxNode_p->ueIndex,
                        0,0,0,0,0.0,0.0,__func__,
                        "ulStrategyTxNode_p entry failed");
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
                freeMemPool(ulStrategyTxNode_p);
#endif
                /*coverity_530_55389*/
                ulStrategyTxNode_p = PNULL;
                /*coverity_530_55389*/
            }
            else
            {
                LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,
                        LOGDEBUG,MAC_UL_Strategy,
                        currentGlobalTTITickCount,
                        ulStrategyTxNode_p->ueIndex,
                        ulStrategyTxNode_p->requiredRB,
                        ulStrategyTxNode_p->strictAllocRB,
                        ulStrategyTxNode_p->minDataSize,
                        ulStrategyTxNode_p->maxDataSize,
                        ulStrategyTxNode_p->avgMcs,
                        ulStrategyTxNode_p->ttiCounter,
                        __func__,"ulStrategyTxNode_p parameters");
            }
            freeMemPool((void *)pendingNode_p);
            pendingNode_p = PNULL;
        }
    }
    *ulStrategyTxNode_pp = ulStrategyTxNode_p;

    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : processCqiWithAndWithoutDataULGrantReqMsg
 * Inputs         : ulGrantPendingQueueCount,
 *                  numberOfAvailableRBs_p,
 *                  currentGlobalTTITickCount,
 *                  ulDelay - UL Delay,
 *                  tempUplinkContext_p - Pointer to ULUEContext,
 *                  pendNode_p - Pointer to ULGrantRequestQueueNode,
 *                  ulStrategyTxNode_pp - Pointer to ULStrategyTxNode,
 *                  tempMaxUEsToBeScheduledUL_p - Maximum No. of UE's to be scheduled,
 *                  ulHarqProcessId - Uplink HARQ Process ID,
 *                  internalCellIndex - cell index used at MAC.
 *                  ulSubFrameNum - uplink subframe number
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function processes the CQI With and Without Data UL Grant 
 *                  request message.
 *****************************************************************************/
MacRetType processCqiWithAndWithoutDataULGrantReqMsg( UInt32 *ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start*/
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p,
        ULStrategyTxNode **ulStrategyTxNode_pp,
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        /* +- SPR 17777 */

        InternalCellIndex internalCellIndex
        /*CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* SPR 15909 fix start */
    tickType_t ttiTick                          = 0;
    /* SPR 15909 fix end */
    MeasGapContextUL * measGapContext_p     = PNULL;
    UInt8 avgMcs                            = 0;
    UInt32 tempUeId                         = 0;
    UInt8 minRb                             = 0;
    UInt8 userLocationType = CC_USER ;
    UInt32 minDataSize                      = 0;
    UInt32 maxDataSize                      = 0;
    UInt8 mcsVal                            = 0;
    UInt8 minRBsforAperiodicCQIReports = 0;
    UInt32 tempMinDataSize = 0;
    UInt32 scellTempMinDataSize = 0; 
    UInt8 scellMinRBsforAperiodicCQIReports = 0; 
    UInt32 usedRB = 0;
    UInt8 isCQIRequestedForPcell = 0; 
    UInt8 isCQIRequestedForScell = 0; 
    /* Coverity 30671 */
    ULGrantRequestQueueNode *pendingNode_p = *pendNode_p; 
    /* Coverity 30671 */
    ULUeScheduleInfo *ulUeScheduleInfo_p = PNULL;
    ttiTick =  pendingNode_p->ttiCounter;
    measGapContext_p = &tempUplinkContext_p->measGapContext;
    avgMcs = tempUplinkContext_p->mcsIndex;
    tempUeId = pendingNode_p->ueIndex;
    userLocationType = tempUplinkContext_p->userLocationType ;
    UInt8 retValue = 0;
    ULStrategyTxNode *ulStrategyTxNode_p = *ulStrategyTxNode_pp;
    /* CA Changes start */
    ULSchedulerStrategyType ulSchedulerStrategy = cellSpecificParams_g.\
                                cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                                ulSchdConfig.ulSchedulerStrategy;
    /* CA Changes end */
    LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
            "[UL-Strategy][%s]Aperiodic CQI Request\
            Received:CQI Queue Cnt:%d Glbl TTI:%d\n",
            __func__ ,*ulGrantPendingQueueCount,
            currentGlobalTTITickCount);

    /* DRX_CHG */
    /* Phase2_CSI_Strategy_Changes start */
    if(*tempMaxUEsToBeScheduledUL_p )
    {
        if ((DRX_STATE_ON == tempUplinkContext_p->
                    drxCurrentState) || (TRUE == 
                        measGapContext_p->isMeasGapPeriodOn) ||
                (UL_SYNC_LOSS_ON == ulUECtxInfoArr_g[(UInt16)tempUeId].
                 ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState))
        {
            /* SPR 2377 changes start */
            LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_UL_LOG_ID, 
                    LOGDEBUG, MAC_UL_Strategy, 
                    currentGlobalTTITickCount, 
                    tempUeId, 
                    tempUplinkContext_p->drxCurrentState,
                    measGapContext_p->isMeasGapPeriodOn,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE, 
                    DEFAULT_FLOAT_VALUE,
                    __func__,"");

            /* SPR 2377 changes end */
            /*CA Changes start  */
            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                    /*CA Changes end  */
                    ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
            if (!retValue)
            {
                lteWarning("Error in Pushing CQI in"
                        "ulGrantPendingQueue in"
                        " funtion %s\n",__func__);
                freeMemPool(pendingNode_p);
            }
            /* + SPR 8550 Fix */
            (*ulGrantPendingQueueCount)--;
            /* - SPR 8550 Fix */
            *ulStrategyTxNode_pp = ulStrategyTxNode_p;
            return MAC_FAILURE;
        }
        /* DRX_CHG */
    }
    /* Phase2_CSI_Strategy_Changes end */

    UInt32 tempNetBsrLoad = tempUplinkContext_p->bsrNetTotal;
    /*CA Qos UL Strategy Chngs start*/
    /* + CQI_4.1 */
    /*minRBsforAperiodicCQIReports = 
      determineRBsForAperiodicCQIReports(tempUplinkContext_p,
      &tempMinDataSize, internalCellIndex);*/
    /* - CQI_4.1 */

    /*CA Qos Ul Strategy changes start*/
    /*CID 109438 Fix start*/
    isCQIRequestedForPcell = (pendingNode_p->aperiodicCQIToBeRequested & servingCellBitMask_g[PCELL_SERV_CELL_IX]);
    isCQIRequestedForScell = (pendingNode_p->aperiodicCQIToBeRequested & servingCellBitMask_g[START_SCELL_INDEX]);
    /*CID 109438 Fix start*/
    /*CID 109609 Fix start*/
    /*CID 109609 Fix end*/
    if (isCQIRequestedForScell && 
            (tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX] == NULL || 
             !IS_SCELL_ACTIVE(tempUeId,START_SCELL_INDEX)))
    {
        if(isCQIRequestedForPcell == 0)
        {
            FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(pendingNode_p);
            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
                    "[%s]:UE does not have scell context initialized or scell is not active, hence NOT \
                    considering UE %d ",
                    __func__,tempUeId);
            (*ulGrantPendingQueueCount)--;
            return MAC_FAILURE;
        }
        else
        {
            isCQIRequestedForScell = 0;
        }
    } 
    /*If the UE has zero BSR available*/
    /* QOS_CHG */
    if((!tempNetBsrLoad)||((tempNetBsrLoad &&
                    ( 
                     /* ICIC changes start */
                     numberOfAvailableRBs_p[userLocationType] 
                     /* ICIC changes end */
                     <
                     MAC_UL_GRANT_FOR_AP_CQI_WITH_BSR_LOAD )) || 
                (ulSchedulerStrategy  == UL_QOS_SCHEDULING)))        /* QOS_CHG */
    {
        /* ULA_CHG */
        // SPR 3486  - START
        /*CA Changes start  */
        /* Phase2_CSI_Strategy_Changes start */
        if((
        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            != ulReferenceFlag_g[internalCellIndex]
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
            != ulReferenceFlag_g[internalCellIndex]
#endif
        /* TDD Config 0 Changes End */
            ) && (tempUplinkContext_p->ulStrategyTxNode_p == PNULL))
        {
            if (isCQIRequestedForPcell && isCQIRequestedForScell)
            {
                minRBsforAperiodicCQIReports = 
                    determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                            &tempMinDataSize, internalCellIndex);
                scellMinRBsforAperiodicCQIReports = 
                    determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                            &scellTempMinDataSize, tempUplinkContext_p->
                            ulUeScellContext_p[START_SCELL_INDEX]->internalCellIndex);
                tempMinDataSize += scellTempMinDataSize;
                minRBsforAperiodicCQIReports += scellMinRBsforAperiodicCQIReports;
                tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs =
                    scellMinRBsforAperiodicCQIReports;
                tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minDataSize = 
                    scellTempMinDataSize;  
            }
            else if(isCQIRequestedForScell)
            {
                minRBsforAperiodicCQIReports = 
                    determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                            &tempMinDataSize, tempUplinkContext_p->
                            ulUeScellContext_p[START_SCELL_INDEX]->internalCellIndex);
                tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs = 
                    minRBsforAperiodicCQIReports;
                tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minDataSize = 
                    tempMinDataSize;  
            }
            else /*scenario for isCQIRequestedForPcell only*/
            {
                minRBsforAperiodicCQIReports = 
                    determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                            &tempMinDataSize, internalCellIndex);
            } 
            minRb = (MAC_MIN_UL_GRANT > MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex)) ? \
                    MAC_MIN_UL_GRANT : MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex);
            /*CA Changes end  */
            /* + CQI_4.1 */
            if(minRb > minRBsforAperiodicCQIReports)
            {
                minRBsforAperiodicCQIReports = minRb;
            }   
            /* ICIC changes start */
            if(numberOfAvailableRBs_p[userLocationType] < 
                    minRBsforAperiodicCQIReports)
                /* ICIC changes end */
            {
                /*Available no. of resources is less 
                  so unable to process CQI Node hence 
                  Pushing into ulGrantPendingQueue */
                /*CA Changes start  */
                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                        /*CA Changes end  */
                        ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
                if (!retValue)
                {
                    /* +- SPR 17777 */
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            /* SPR_2446 start */
                            pendingNode_p->ueIndex,\
                            /* SPR_2446 end */
                            ulDelay,0,0,0,0.0,0.0,__func__,
                            "Error in Pushing CQI node in\
                            ulGrantPendingQueue");
                    freeMemPool(pendingNode_p);
                    /* Coverity Fix 30671 Start */
                    pendingNode_p = NULL;
                    /* Coverity Fix 30671 End*/
                }
                /* Coverity Fix 30671 Start */
                /* + SPR 8550 Fix */
                (*ulGrantPendingQueueCount)--;
                /* - SPR 8550 Fix */
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;
                /* Coverity Fix 30671 End*/
            }
            /*Preparing the UL Strategy outPut Node*/
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
            GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else        
            GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p,
                    sizeof(ULStrategyTxNode),PNULL);
#endif        

            //Coverity_36246 fixed

            if(PNULL == ulStrategyTxNode_p)
            {
                ltePanic("getMemFromPool fails %s", __func__);
                /* Klockwork warning Changes Start */
                return MAC_FAILURE;
                /* Klockwork warning Changes End */
            }
            ulStrategyTxNode_p->requiredRB = 0;
            ulStrategyTxNode_p->maxDataSize = 0;
            ulStrategyTxNode_p->minDataSize = 0;
            if(*tempMaxUEsToBeScheduledUL_p == 0)
            {
                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                        /*CA Changes end  */
                        ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
                if (!retValue)
                {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            /* SPR_2446 start */
                            pendingNode_p->ueIndex,\
                            /* SPR_2446 end */
                            0,0,0,0,0.0,0.0,__func__,
                            "Error in Pushing CQI node in\
                            ulGrantPendingQueue");
                    freeMemPool(pendingNode_p);
                    /* Coverity Fix 30671 Start */
                    pendingNode_p = NULL;
                    /* Coverity Fix 30671 End*/
                }
                (*ulGrantPendingQueueCount)--;
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;
            }
            (*tempMaxUEsToBeScheduledUL_p)--;
            if (userLocationType == CE_USER)
            {   
                /*CA Changes start  */
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
            }
            /*coverity id 61342*/
            ulStrategyTxNode_p->requiredRB = 0;
            ulStrategyTxNode_p->maxDataSize = 0;
            ulStrategyTxNode_p->minDataSize = 0;

        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
            = ulReferenceFlag_g[internalCellIndex];
#endif
        /* TDD Config 0 Changes End */
            ulStrategyTxNode_p->ueIndex = (UInt16)tempUeId;
            /* + CQI_4.1 */
            ulStrategyTxNode_p->requiredRB += minRBsforAperiodicCQIReports;
            /* ULA_CHG */
            /* ICIC changes start */
            numberOfAvailableRBs_p[userLocationType] -= minRBsforAperiodicCQIReports;
            if (userLocationType == CE_USER)
            {   
                numberOfAvailableRBs_p[CC_CE_REGION] -= 
                    minRBsforAperiodicCQIReports;
            }

            else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                    numberOfAvailableRBs_p[CE_REGION])
            {
                numberOfAvailableRBs_p[CE_REGION] = 
                    numberOfAvailableRBs_p[CC_CE_REGION];
            }
            /* ICIC changes end */
            /* ULA_CHG */
            ulStrategyTxNode_p->maxDataSize += tempMinDataSize;
            ulStrategyTxNode_p->minDataSize += tempMinDataSize;
            /* - CQI_4.1 */
            ulStrategyTxNode_p->strictAllocRB = TRUE;
            ulStrategyTxNode_p->ulGrantRequestType = CQI_WITHOUT_DATA_ULGRANT_REQ_MSG;
            ulStrategyTxNode_p->ttiCounter = ttiTick;
#ifdef TDD_CONFIG                                                
            ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessId;
        /*TDD Config 0 Changes Start*/
        ulStrategyTxNode_p->scheduledCount = 0;
        /*TDD Config 0 Changes End*/
#endif                                 
            /* + CQI_5.0 */
            ulStrategyTxNode_p->avgMcs = MCS_INDEX_CQI_WITHOUT_DATA;
            /* - CQI_5.0 */
            if (ulSchedulerStrategy == UL_QOS_SCHEDULING)
            {
                /*putEntry of ULStrategyTXNode in UL Strategy Schedule Info */

                /*CA Changes start  */
                ulUeScheduleInfo_p = &(ulStrategyScheduleInfo_gp[internalCellIndex]->ulUeScheduleInfo
                        [ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount]);
                /*CA Changes end  */

                ulUeScheduleInfo_p->ulStrategyTxNode_p = ulStrategyTxNode_p;


                ulUeScheduleInfo_p->ueULContext_p = tempUplinkContext_p;

                tempUplinkContext_p->ulStrategyTxNode_p = ulStrategyTxNode_p;
                /*spr 11709 fix start*/
                ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount++;
                /*spr 11709 fix end*/

                if (isCQIRequestedForScell)
                {
                    tempUplinkContext_p->aperiodicCQIToBeRequested =
                        tempUplinkContext_p->aperiodicCQIToBeRequested |
                        servingCellBitMask_g[START_SCELL_INDEX];
                }
                if (isCQIRequestedForPcell) 
                { 
                    tempUplinkContext_p->aperiodicCQIToBeRequested = 
                        tempUplinkContext_p->aperiodicCQIToBeRequested | 
                        servingCellBitMask_g[PCELL_SERV_CELL_IX];
                }

                freeMemPool((void *)pendingNode_p);
                pendingNode_p = PNULL;

                /* CA Stats Changes Start */
                /*debug stats*/
                LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_AND_PENDING_AP_CQI(
                        internalCellIndex);
                LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_CQI(
                        internalCellIndex);
                /* CA Stats Changes End */

            } /*CA Qos Ul Strategy changes start*/
            else
            {
                /*putEntry of ULStrategyTXNode in 
                  PDCCH Queue */
                /*CA Changes start  */
                if (!(putEntryInULSchedulerNewTxQueue(ulStrategyTxNode_p, internalCellIndex
                    /*CA Changes end  */
                            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                            ,currentGlobalTTITickCount
                            ,ulSubFrameNum
                            ,tempUplinkContext_p
#endif
                            /* TDD Config 0 Changes End */
                            )))
                {
                    /* ULA_CHG */
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGERROR,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            ulStrategyTxNode_p->ueIndex,\
                            0,0,0,0,0.0,0.0,__func__,
                            "ulStrategyTxNode_p entry failed");
                    /* ULA_CHG */
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
                    FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else                
                    freeMemPool(ulStrategyTxNode_p);
#endif                
                    /*coverity_530_55388-II*/
                    ulStrategyTxNode_p = PNULL;
                    /*coverity_530_55388-II*/
                }
                /* ULA_CHG */
                else
                {
                    tempUplinkContext_p->aperiodicCQIToBeRequested = TRUE;
                    LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                            LOGDEBUG,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            ulStrategyTxNode_p->ueIndex,\
                            ulStrategyTxNode_p->requiredRB,\
                            ulStrategyTxNode_p->strictAllocRB,\
                            ulStrategyTxNode_p->minDataSize,\
                            ulStrategyTxNode_p->maxDataSize,\
                            ulStrategyTxNode_p->avgMcs,\
                            ulStrategyTxNode_p->ttiCounter,\
                            __func__,"ulStrategyTxNode_p parameters");
                }
                /* ULA_CHG */
                freeMemPool((void *)pendingNode_p);
                pendingNode_p = PNULL;
            }
        }
        else
        {
            if (ulSchedulerStrategy != UL_QOS_SCHEDULING)
            {
                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                        /*CA Changes end  */
                        ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
                if (!retValue)
                {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            /* SPR_2446 start */
                            pendingNode_p->ueIndex,\
                            /* SPR_2446 end */
                            0,0,0,0,0.0,0.0,__func__,
                            "Error in Pushing CQI node in\
                            ulGrantPendingQueue");
                    freeMemPool(pendingNode_p);
                    /* Coverity Fix 30671 Start */
                    pendingNode_p = NULL;
                    /* Coverity Fix 30671 End*/
                }
                (*ulGrantPendingQueueCount)--;
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;

            }
            ulStrategyTxNode_p = tempUplinkContext_p->ulStrategyTxNode_p;
            if(((PNULL == ulStrategyTxNode_p)) ||
                    ((ulStrategyTxNode_p->ulGrantRequestType != CQI_WITH_DATA_ULGRANT_REQ_MSG) &&
                     (ulStrategyTxNode_p->ulGrantRequestType != CQI_WITHOUT_DATA_ULGRANT_REQ_MSG)))
            {
                /* ue is already considered for some other grant type so
                 * * will not process its aperiodic cqi queue */
                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                        /*CA Changes end  */
                        ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
                if (!retValue)
                {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            /* SPR_2446 start */
                            pendingNode_p->ueIndex,\
                            /* SPR_2446 end */
                            0,0,0,0,0.0,0.0,__func__,
                            "Error in Pushing CQI node in\
                            ulGrantPendingQueue");
                    freeMemPool(pendingNode_p);
                    /* Coverity Fix 30671 Start */
                    pendingNode_p = NULL;
                    /* Coverity Fix 30671 End*/
                }
                (*ulGrantPendingQueueCount)--;
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;
            }
            /* update the existing strategy node in case of Qos */
            if(isCQIRequestedForPcell && (tempUplinkContext_p->aperiodicCQIToBeRequested
                        & servingCellBitMask_g[PCELL_SERV_CELL_IX]))
            {
                isCQIRequestedForPcell = 0;
            }
            if(isCQIRequestedForScell && (tempUplinkContext_p->aperiodicCQIToBeRequested
                        & servingCellBitMask_g[START_SCELL_INDEX]))
            {
                isCQIRequestedForScell = 0;
            }
            if((isCQIRequestedForPcell == 0) &&
                    (isCQIRequestedForScell == 0))
            {
                /* node is already considered so free it from queue */
                FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(pendingNode_p);
                (*ulGrantPendingQueueCount)--;
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;
            }
            if(isCQIRequestedForScell)
            {
                minRBsforAperiodicCQIReports = 
                    determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                            &tempMinDataSize, tempUplinkContext_p->
                            ulUeScellContext_p[START_SCELL_INDEX]->internalCellIndex);
                tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs = 
                    minRBsforAperiodicCQIReports;
                tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minDataSize =
                    tempMinDataSize;  
            }
            if(isCQIRequestedForPcell)
            {
                minRBsforAperiodicCQIReports = 
                    determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                            &tempMinDataSize, internalCellIndex);
            }
            ulStrategyTxNode_p = tempUplinkContext_p->ulStrategyTxNode_p;
            minRb = (MAC_MIN_UL_GRANT > MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex)) ? \
                    MAC_MIN_UL_GRANT : MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex);
            /*CA Changes end  */
            /* + CQI_4.1 */
            if(minRb > minRBsforAperiodicCQIReports)
            {
                minRBsforAperiodicCQIReports = minRb;
            }   
            /* ICIC changes start */
            if(numberOfAvailableRBs_p[userLocationType] < 
                    minRBsforAperiodicCQIReports)
                /* ICIC changes end */
            {
                /*Available no. of resources is less 
                  so unable to process CQI Node hence 
                  Pushing into ulGrantPendingQueue */
                /*CA Changes start  */
                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                        /*CA Changes end  */
                        ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
                if (!retValue)
                {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            /* SPR_2446 start */
                            pendingNode_p->ueIndex,\
                            /* SPR_2446 end */
                            0,0,0,0,0.0,0.0,__func__,
                            "Error in Pushing CQI node in\
                            ulGrantPendingQueue");
                    freeMemPool(pendingNode_p);
                    /* Coverity Fix 30671 Start */
                    pendingNode_p = NULL;
                    /* Coverity Fix 30671 End*/
                }
                /* Coverity Fix 30671 Start */
                /* + SPR 8550 Fix */
                (*ulGrantPendingQueueCount)--;
                /* - SPR 8550 Fix */
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;
                /* Coverity Fix 30671 End*/
            }
            /* + CQI_4.1 */
            ulStrategyTxNode_p->requiredRB += minRBsforAperiodicCQIReports;
            /* ULA_CHG */
            /* ICIC changes start */
            numberOfAvailableRBs_p[userLocationType] -= minRBsforAperiodicCQIReports;
            if (userLocationType == CE_USER)
            {   
                numberOfAvailableRBs_p[CC_CE_REGION] -= 
                    minRBsforAperiodicCQIReports;
            }

            else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                    numberOfAvailableRBs_p[CE_REGION])
            {
                numberOfAvailableRBs_p[CE_REGION] = 
                    numberOfAvailableRBs_p[CC_CE_REGION];
            }
            /* ICIC changes end */
            /* ULA_CHG */
            ulStrategyTxNode_p->maxDataSize += tempMinDataSize;
            ulStrategyTxNode_p->minDataSize += tempMinDataSize;
            /* - CQI_4.1 */
            if (isCQIRequestedForScell)
            {
                tempUplinkContext_p->aperiodicCQIToBeRequested = 
                    tempUplinkContext_p->aperiodicCQIToBeRequested | 
                    servingCellBitMask_g[START_SCELL_INDEX];
            }
            if (isCQIRequestedForPcell) 
            { 
                tempUplinkContext_p->aperiodicCQIToBeRequested = 
                    tempUplinkContext_p->aperiodicCQIToBeRequested | 
                    servingCellBitMask_g[PCELL_SERV_CELL_IX];
            }

            freeMemPool((void *)pendingNode_p);
            pendingNode_p = PNULL;

            /* CA Stats Changes Start */
            /*debug stats*/
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_AND_PENDING_AP_CQI(
                    internalCellIndex);
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_CQI(
                    internalCellIndex);
            /* CA Stats Changes End */
        }
    }
    /*else if the UE has non zero BSR*/
    else
    {
        /* Phase2_CSI_Strategy_Changes start */
        if((
        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            != ulReferenceFlag_g[internalCellIndex]
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
            != ulReferenceFlag_g[internalCellIndex]
#endif
        /* TDD Config 0 Changes End */
            )
                && (*tempMaxUEsToBeScheduledUL_p !=0))
        {
            /* Phase2_CSI_Strategy_Changes end */
            // SPR 3486  - START
            /*CA Changes start  */
            minRb = (MAC_UL_GRANT_FOR_AP_CQI_WITH_BSR_LOAD > MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex)) ?\
                    MAC_UL_GRANT_FOR_AP_CQI_WITH_BSR_LOAD:MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex);
            /*CA Changes end  */
            // SPR 3486  - END
            /* ULA_CHG */
            UInt32 strictAllocRBFlag = FALSE;
            /* ULA_CHG */
            (*tempMaxUEsToBeScheduledUL_p)--;
        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
            = ulReferenceFlag_g[internalCellIndex];
#endif
        /* TDD Config 0 Changes End */
            usedRB = validateUplinkRB 
                ( tempUplinkContext_p, 
                  // SPR 3486  - START
                  minRb,
                  // SPR 3486  - END
                  numberOfAvailableRBs_p[userLocationType],
                  &strictAllocRBFlag
                  /* ULA_CHG */
                  ,&minDataSize,
                  &maxDataSize,
                  /* ULA_CHG */
                  /* ATB Changes Start */
                  &mcsVal,
                  /*SPR 16417 Changes Start*/
#ifdef FDD_CONFIG
                  /* SPR 13532 Fix Start */
                  (currentGlobalTTITickCount + ulDelay),
                  /* SPR 13532 Fix End */
#elif TDD_CONFIG
                  currentGlobalTTITickCount,
                  ulSubFrameNum,
#endif
                  /*SPR 16417 Changes End*/
                  /* ATB Changes End */
                  internalCellIndex
                );
            if (usedRB)
            {
#ifdef FDD_CONFIG
                /* TTIB_Code Start */
                if( tempUplinkContext_p->ttiBundlingEnabled )
                {
                    if ( usedRB > MAX_TTI_BUNDLE_RB )
                    {
                        usedRB = MAX_TTI_BUNDLE_RB;
                    }
                    if ( mcsVal > MAX_TTI_BUNDLE_MCS )
                    {
                        mcsVal = MAX_TTI_BUNDLE_MCS;
                    }
                    maxDataSize = determineUlTBSize(mcsVal,usedRB);
                    minDataSize = determineUlTBSize(mcsVal,MIN_TTI_BUNDLE_RB);
                    /*CA Changes start  */
                    if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < usedRB ) 
                            || (MAC_FAILURE == processMeasGapValidation(
                                    tempUplinkContext_p,currentGlobalTTITickCount,ulDelay )))
                    {
                        /*coverity id 63530*/
                        if(!ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex], 
                                    ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor)))
                        {
                            FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);
                        }
                        /* + SPR 8550 Fix */
                        (*ulGrantPendingQueueCount)--;
                        /* - SPR 8550 Fix */
                        *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                        return MAC_FAILURE;
                    }
                    availableRBForTtiBundlingPerTick_g[internalCellIndex] -= usedRB;
                    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                            "[%s][%d]:TTIB UE ID %d scheduled RB:%d MCS:%d \n",
                            __func__,getCurrentTick(),tempUeId,usedRB,mcsVal);
                    /*CA Changes end  */
                }
                /* TTIB_Code End */
#endif
                /*Preparing the UL Strategy outPut Node*/
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
                GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
                GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p,
                        sizeof(ULStrategyTxNode),PNULL);
#endif            
                /*cov_warning 32646 fix start*/
                if(PNULL == ulStrategyTxNode_p)
                {
                    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                            ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
                    if (!retValue)
                    {
                        freeMemPool(pendingNode_p);
                    }
                    /* + SPR 8550 Fix */
                    (*ulGrantPendingQueueCount)--;
                    /* - SPR 8550 Fix */
                    ltePanic("Memory Allocation failed in %s\n",__func__);
                    return MAC_FAILURE;
                }
                ulStrategyTxNode_p->requiredRB = 0;
                ulStrategyTxNode_p->maxDataSize = 0;
                ulStrategyTxNode_p->minDataSize = 0;

                /*cov warning 32646 fix end */

                LTE_MAC_UT_LOG(LOG_DETAIL,
                        UL_STRATEGY,"[UL-Strategy][%s]"
                        "validateUplinkRB returned"
                        "= %d RBs ,Available RBs =%d, Cell Edge RBs =%d"
                        "for UEID %d",__func__,
                        usedRB,numberOfAvailableRBs_p[CC_CE_REGION],
                        numberOfAvailableRBs_p[CE_REGION],tempUeId);
                /* ICIC changes start */
                /* ULA_CHG */

                /* ATB Changes Start */
                avgMcs = mcsVal;
                /* ATB Changes End */

                numberOfAvailableRBs_p[userLocationType] -= usedRB;

                if (userLocationType == CE_USER)
                {
                    /*CA Changes start  */
                    maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                    /*CA Changes end  */
                    numberOfAvailableRBs_p[CC_CE_REGION] -= usedRB;
                }

                else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                        numberOfAvailableRBs_p[CE_REGION])
                {
                    numberOfAvailableRBs_p[CE_REGION] = 
                        numberOfAvailableRBs_p[CC_CE_REGION];
                }
                /* ICIC changes end */
                /* + CQI_5.0 */
                tempUplinkContext_p->aperiodicCQIToBeRequested = 
                    pendingNode_p->aperiodicCQIToBeRequested;
                /* - CQI_5.0 */
                /*Optimization Start */
#ifdef TDD_CONFIG
                PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, (UInt16)tempUeId, usedRB, strictAllocRBFlag, 
                        CQI_WITH_DATA_ULGRANT_REQ_MSG, ttiTick, ulHarqProcessId, 
                        minDataSize, maxDataSize, avgMcs)
#else
                    PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, (UInt16)tempUeId, usedRB, strictAllocRBFlag, 
                            CQI_WITH_DATA_ULGRANT_REQ_MSG, ttiTick, minDataSize, maxDataSize, avgMcs)
#endif
                    /*Optimization End */

                    /*putEntry of ULStrategyTXNode in 
                      PDCCH Queue */
                    /*CA Changes start  */
                    if (!(putEntryInULSchedulerNewTxQueue(ulStrategyTxNode_p, internalCellIndex
                        /*CA Changes end  */
                            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                            ,currentGlobalTTITickCount
                            ,ulSubFrameNum
                            ,tempUplinkContext_p
#endif
                            /* TDD Config 0 Changes End */
                            )))
                    {
                        /* ULA_CHG */
                        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                LOGERROR,MAC_UL_Strategy,\
                                currentGlobalTTITickCount,\
                                ulStrategyTxNode_p->ueIndex,\
                                0,0,0,0,0.0,0.0,__func__,
                                "ulStrategyTxNode_p entry failed");
                        /* ULA_CHG */
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
                        FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else                
                        freeMemPool(ulStrategyTxNode_p);
#endif               
                        /*coverity_530_55388*/
                        ulStrategyTxNode_p = PNULL;
                        /*coverity_530_55388*/

                    }
                /* ULA_CHG */
                    else
                    {
                        LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                                LOGDEBUG,MAC_UL_Strategy,\
                                currentGlobalTTITickCount,\
                                ulStrategyTxNode_p->ueIndex,\
                                ulStrategyTxNode_p->requiredRB,\
                                ulStrategyTxNode_p->strictAllocRB,\
                                ulStrategyTxNode_p->minDataSize,\
                                ulStrategyTxNode_p->maxDataSize,\
                                ulStrategyTxNode_p->avgMcs,\
                                ulStrategyTxNode_p->ttiCounter,\
                                __func__,"ulStrategyTxNode_p parameters");
                    }
                /* ULA_CHG */
            }
            else
            {
                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                        /*CA Changes start  */
                        ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                        &(pendingNode_p->ulGrantRequestNodeAnchor));
                /*CA Changes end  */
                if (!retValue)
                {
                    lteWarning("Error in Pushing CQI in"
                            "ulGrantPendingQueue in"
                            " funtion %s\n",__func__);
                    freeMemPool(pendingNode_p);
                }

                /* + SPR 8550 Fix */
                (*ulGrantPendingQueueCount)--;
                /* - SPR 8550 Fix */
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;
            }
            freeMemPool((void *)pendingNode_p);
            pendingNode_p = PNULL;
        }
        /* Phase2_CSI_Strategy_Changes start */
        else
        {
            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                    /*CA Changes start  */
                    ulGrantPendingQueue_gp[internalCellIndex],
                    ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
            /*CA Changes end  */
            if (!retValue)
            {
                lteWarning("Error in Pushing CQI in"
                        "ulGrantPendingQueue in"
                        " funtion %s\n",__func__);
                freeMemPool(pendingNode_p);
            }

            /* + SPR 8550 Fix */
            (*ulGrantPendingQueueCount)--;
            /* - SPR 8550 Fix */
            *ulStrategyTxNode_pp = ulStrategyTxNode_p;
            return MAC_FAILURE;

        }
        /* Phase2_CSI_Strategy_Changes end */
    }
    *ulStrategyTxNode_pp = ulStrategyTxNode_p;
    return MAC_SUCCESS;
}
/* SPR 16840 fix start */
/* SPR 17211 Fix Start */
/***************************************************************************************
 * Function Name  : processNewULGrantReqMsg
 * Inputs         : ulGrantPendingQueueCount,
 *                  numberOfAvailableRBs_p,
 *                  currentGlobalTTITickCount,
 *                  ulDelay - UL Delay,
 *                  tempUplinkContext_p - Pointer to ULUEContext,
 *                  pendNode_p - Pointer to ULGrantRequestQueueNode,
 *                  ulStrategyTxNode_pp - Pointer to ULStrategyTxNode,
 *                  tempMaxUEsToBeScheduledUL_p - Maximum No. of UE's to be scheduled,
 *                  ulHarqProcessId - Uplink HARQ Process ID
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function processes the CQI With and Without Data UL Grant 
 *                  request message.
 ***************************************************************************************/
MacRetType processNewULGrantReqMsg( UInt32 *ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        tickType_t currentGlobalTTITickCount,
        UInt32 ulDelay,
        ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p,
        ULStrategyTxNode **ulStrategyTxNode_pp,
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        )
{
    tickType_t ttiTick                          = 0;
    MeasGapContextUL * measGapContext_p     = PNULL;
    UInt8 avgMcs                            = 0;
    UInt32 tempUeId                         = 0;
    UInt8 minRb                             = 0;
    UInt8 userLocationType = CC_USER ;
    UInt32 minDataSize                      = 0;
    UInt32 maxDataSize                      = 0;
    UInt8 mcsVal                            = 0;
    UInt32 usedRB = 0;
	UInt32 strictAllocRBFlag = FALSE;
    /* Coverity 30671 */
    ULGrantRequestQueueNode *pendingNode_p = *pendNode_p; 
    /* Coverity 30671 */
    ULUeScheduleInfo *ulUeScheduleInfo_p = PNULL;
    ttiTick =  pendingNode_p->ttiCounter;
    measGapContext_p = &tempUplinkContext_p->measGapContext;
    /* coverity fix 100453 : Removed Assignment of avgMcs */
    tempUeId = pendingNode_p->ueIndex;
    userLocationType = tempUplinkContext_p->userLocationType ;
    UInt8 retValue = 0;
    ULStrategyTxNode *ulStrategyTxNode_p = *ulStrategyTxNode_pp;
    ULSchedulerStrategyType ulSchedulerStrategy = cellSpecificParams_g.\
                                cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                                ulSchdConfig.ulSchedulerStrategy;

    LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
            "[UL-Strategy][%s]Aperiodic CQI Request\
            Received:CQI Queue Cnt:%d Glbl TTI:%d\n",
            __func__ ,*ulGrantPendingQueueCount,
            currentGlobalTTITickCount);

    /* DRX_CHG */
     if(*tempMaxUEsToBeScheduledUL_p )
     {
    if ((DRX_STATE_ON == tempUplinkContext_p->
                drxCurrentState) || (TRUE == 
                    measGapContext_p->isMeasGapPeriodOn) ||
            (UL_SYNC_LOSS_ON == ulUECtxInfoArr_g[(UInt16)tempUeId].
             ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState))
    {
        /* SPR 2377 changes start */
        LOG_MSG(MAC_UE_NOT_SCHEDULED_IN_UL_LOG_ID, 
                LOGDEBUG, MAC_UL_Strategy, 
                currentGlobalTTITickCount, 
                tempUeId, 
                tempUplinkContext_p->drxCurrentState,
                measGapContext_p->isMeasGapPeriodOn,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE, 
                DEFAULT_FLOAT_VALUE,
                __func__,"");

        /* SPR 2377 changes end */
            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
        if (!retValue)
        {
            lteWarning("Error in Pushing CQI in"
                    "ulGrantPendingQueue in"
                    " funtion %s\n",__func__);
            freeMemPool(pendingNode_p);
        }
        /* + SPR 8550 Fix */
        (*ulGrantPendingQueueCount)--;
        /* - SPR 8550 Fix */
        *ulStrategyTxNode_pp = ulStrategyTxNode_p;
        return MAC_FAILURE;
    }
    /* DRX_CHG */
     }

    UInt32 tempNetBsrLoad = tempUplinkContext_p->bsrNetTotal;

    /*If the UE has zero BSR available*/
    /* QOS_CHG */
    if((!tempNetBsrLoad)||((tempNetBsrLoad &&
                    ( 
                     /* ICIC changes start */
                     numberOfAvailableRBs_p[userLocationType] 
                     /* ICIC changes end */
                     <
                     MAC_MIN_UL_GRANT )) || 
                (ulSchedulerStrategy 
                 == UL_QOS_SCHEDULING)))
        /* QOS_CHG */
    {
	    minRb = MAC_SCH_MIN_UL_GRANT; 
	    if(numberOfAvailableRBs_p[userLocationType] < minRb)
	    {
		    /*Available no. of resources is less 
		      so unable to process CQI Node hence 
		      Pushing into ulGrantPendingQueue */
		    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
				    ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
		    if (!retValue)
		    {
			    LOG_MSG(MAC_UL_STRATEGY_ERROR_ID,\
					    LOGWARNING,MAC_UL_Strategy,\
					    currentGlobalTTITickCount,\
					    /* SPR_2446 start */
					    pendingNode_p->ueIndex,\
					    /* SPR_2446 end */
					    ulDelay,0,0,0,0.0,0.0,__func__,
					    "Error in Pushing CQI node in\
					    ulGrantPendingQueue");
			    freeMemPool(pendingNode_p);
			    /* Coverity Fix 30671 Start */
			    pendingNode_p = NULL;
			    /* Coverity Fix 30671 End*/
		    }
		    /* coverity fix : removed the assignment as minRb was never used within the scope 
             *                of this function.*/
		    /* Coverity Fix 30671 Start */
		    /* + SPR 8550 Fix */
		    (*ulGrantPendingQueueCount)--;
		    /* - SPR 8550 Fix */
		    *ulStrategyTxNode_pp = ulStrategyTxNode_p;
		    return MAC_FAILURE;
		    /* Coverity Fix 30671 End*/
	    }
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
            = ulReferenceFlag_g[internalCellIndex];
#endif

	    usedRB = validateUplinkRB 
		    ( tempUplinkContext_p, 
		      minRb,
		      numberOfAvailableRBs_p[userLocationType],
		      &strictAllocRBFlag
		      ,&minDataSize,
		      &maxDataSize,
		      &mcsVal,
#ifdef FDD_CONFIG
              (currentGlobalTTITickCount + ulDelay),
#elif TDD_CONFIG
                  currentGlobalTTITickCount,
                  ulSubFrameNum,
#endif
              internalCellIndex
		    );
	    if(usedRB)
	    {
		    /*Preparing the UL Strategy outPut Node*/
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
		    GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else        
		    GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p,
				    sizeof(ULStrategyTxNode),PNULL);
#endif        


		    if(PNULL == ulStrategyTxNode_p)
		    {
			    ltePanic("getMemFromPool fails %s", __func__);
                return MAC_FAILURE; /*CoverityId 101189; SPR:19339*/
		    }
            /*SPR 19852 fix start*/
            if(0 == *tempMaxUEsToBeScheduledUL_p)
            {
                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                        ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
                if (!retValue)
                {
                    LOG_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            pendingNode_p->ueIndex,\
                            ulDelay,0,0,0,0.0,0.0,__func__,
                            "tempMaxUEsToBeScheduledUL_p is NULL");
                    freeMemPool(pendingNode_p);
                    pendingNode_p = PNULL;
                }
                (*ulGrantPendingQueueCount)--;
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;

            }
            /*SPR 19852 fix start*/
		    (*tempMaxUEsToBeScheduledUL_p)--;
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
            = ulReferenceFlag_g[internalCellIndex];
		    ulStrategyTxNode_p->scheduledCount = 0;/*SPR 19078 - CoverityId 107819*/
#endif
		    ulStrategyTxNode_p->ueIndex = (UInt16)tempUeId;
		    ulStrategyTxNode_p->requiredRB = minRb;
		    numberOfAvailableRBs_p[userLocationType] -= minRb;
		    if (userLocationType == CE_USER)
		    {   
			    maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
			    numberOfAvailableRBs_p[CC_CE_REGION] -= 
				    minRb;
		    }

		    else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
				    numberOfAvailableRBs_p[CE_REGION])
		    {
			    numberOfAvailableRBs_p[CE_REGION] = 
				    numberOfAvailableRBs_p[CC_CE_REGION];
		    }
		    ulStrategyTxNode_p->maxDataSize = maxDataSize;
		    ulStrategyTxNode_p->minDataSize = minDataSize;
		    ulStrategyTxNode_p->strictAllocRB = FALSE;
		    ulStrategyTxNode_p->ulGrantRequestType = NEW_ULGRANT_REQ_MSG;
		    ulStrategyTxNode_p->ttiCounter = ttiTick;
#ifdef TDD_CONFIG
            ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessId;
#endif 
		    ulStrategyTxNode_p->avgMcs = mcsVal;

		    if (ulSchedulerStrategy == UL_QOS_SCHEDULING)
		    {
			    /*putEntry of ULStrategyTXNode in UL Strategy Schedule Info */

			    ulUeScheduleInfo_p = &(ulStrategyScheduleInfo_gp[internalCellIndex]->ulUeScheduleInfo
					    [ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount]);

			    ulUeScheduleInfo_p->ulStrategyTxNode_p = ulStrategyTxNode_p;


			    ulUeScheduleInfo_p->ueULContext_p = tempUplinkContext_p;

			    tempUplinkContext_p->ulStrategyTxNode_p = ulStrategyTxNode_p;

			    /* Increament the scheduled UE count */
			    ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount++;

			    freeMemPool((void *)pendingNode_p);
			    pendingNode_p = PNULL;

		    }
		    else
		    {

			    /*putEntry of ULStrategyTXNode in 
			      PDCCH Queue */
			    if (!(putEntryInULSchedulerNewTxQueue(ulStrategyTxNode_p,internalCellIndex
#ifdef TDD_CONFIG
                            ,currentGlobalTTITickCount
                            ,ulSubFrameNum
                            ,tempUplinkContext_p
#endif
                    )))
			    {
				    /* ULA_CHG */
				    LOG_MSG(MAC_UL_STRATEGY_ERROR_ID,\
						    LOGERROR,MAC_UL_Strategy,\
						    currentGlobalTTITickCount,\
						    ulStrategyTxNode_p->ueIndex,\
						    0,0,0,0,0.0,0.0,__func__,
						    "ulStrategyTxNode_p entry failed");
				    /* ULA_CHG */
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
				    FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else                
				    freeMemPool(ulStrategyTxNode_p);
#endif                
				    /*coverity_530_55388-II*/
				    ulStrategyTxNode_p = PNULL;
				    /*coverity_530_55388-II*/
			    }
			    /* ULA_CHG */
			    else
			    {
				    LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
						    LOGDEBUG,MAC_UL_Strategy,\
						    currentGlobalTTITickCount,\
						    ulStrategyTxNode_p->ueIndex,\
						    ulStrategyTxNode_p->requiredRB,\
						    ulStrategyTxNode_p->strictAllocRB,\
						    ulStrategyTxNode_p->minDataSize,\
						    ulStrategyTxNode_p->maxDataSize,\
						    ulStrategyTxNode_p->avgMcs,\
						    ulStrategyTxNode_p->ttiCounter,\
						    __func__,"ulStrategyTxNode_p parameters");
			    }
			    /* ULA_CHG */
			    freeMemPool((void *)pendingNode_p);
			    pendingNode_p = PNULL;
		    }
	    }
	    else
	    {
		    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
				    ulGrantPendingQueue_gp[internalCellIndex],
                    ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
		    if (!retValue)
		    {
			    lteWarning("Error in Pushing NEW UL GRANT in"
					    "ulGrantPendingQueue in"
					    " funtion %s\n",__func__);
			    freeMemPool(pendingNode_p);
		    }

		    /* + SPR 8550 Fix */
		    (*ulGrantPendingQueueCount)--;
		    /* - SPR 8550 Fix */
		    *ulStrategyTxNode_pp = ulStrategyTxNode_p;
		    return MAC_FAILURE;
	    }
    }
    /*else if the UE has non zero BSR*/
    else
    {
        minRb = MAC_MIN_UL_GRANT; 
        /*SPR 19852 fix start*/
        if(0 == *tempMaxUEsToBeScheduledUL_p)
        {
            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                    ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
            if (!retValue)
            {
                LOG_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGWARNING,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        pendingNode_p->ueIndex,\
                        ulDelay,0,0,0,0.0,0.0,__func__,
                        "tempMaxUEsToBeScheduledUL_p is NULL");
                freeMemPool(pendingNode_p);
                pendingNode_p = PNULL;
            }
            (*ulGrantPendingQueueCount)--;
            *ulStrategyTxNode_pp = ulStrategyTxNode_p;
            return MAC_FAILURE;

        }
        /*SPR 19852 fix start*/

        (*tempMaxUEsToBeScheduledUL_p)--;
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
            = ulReferenceFlag_g[internalCellIndex];
#endif

        usedRB = validateUplinkRB 
            ( tempUplinkContext_p, 
              minRb,
              numberOfAvailableRBs_p[userLocationType],
              &strictAllocRBFlag
              ,&minDataSize,
              &maxDataSize,
              &mcsVal,
#ifdef FDD_CONFIG
              (currentGlobalTTITickCount + ulDelay),
#elif TDD_CONFIG
                  currentGlobalTTITickCount,
                  ulSubFrameNum,
#endif
              internalCellIndex
            );
        if (usedRB)
        {
            /*Preparing the UL Strategy outPut Node*/
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
            GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
            GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p,
                    sizeof(ULStrategyTxNode),PNULL);
#endif            
            if(PNULL == ulStrategyTxNode_p)
            {
                ltePanic("Memory Allocation failed in %s\n",__func__);
                return MAC_FAILURE; /*CoverityId 101189; SPR:19339*/
            }
#ifdef TDD_CONFIG
            ulStrategyTxNode_p->scheduledCount = 0;/*SPR 19078 - CoverityId 107819*/
#endif
            LTE_MAC_UT_LOG(LOG_DETAIL,
                    UL_STRATEGY,"[UL-Strategy][%s]"
                    "validateUplinkRB returned"
                    "= %d RBs ,Available RBs =%d, Cell Edge RBs =%d"
                    "for UEID %d",__func__,
                    usedRB,numberOfAvailableRBs_p[CC_CE_REGION],
                    numberOfAvailableRBs_p[CE_REGION],tempUeId);
            avgMcs = mcsVal;
            numberOfAvailableRBs_p[userLocationType] -= usedRB;

            if (userLocationType == CE_USER)
            {
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                numberOfAvailableRBs_p[CC_CE_REGION] -= usedRB;
            }

            else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                    numberOfAvailableRBs_p[CE_REGION])
            {
                numberOfAvailableRBs_p[CE_REGION] = 
                    numberOfAvailableRBs_p[CC_CE_REGION];
            }
            PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, (UInt16)tempUeId, usedRB, strictAllocRBFlag, 
                                        NEW_ULGRANT_REQ_MSG, ttiTick, 
#ifdef TDD_CONFIG
                                        ulHarqProcessId, 
#endif
                                        minDataSize, maxDataSize, avgMcs)

            /*putEntry of ULStrategyTXNode in 
              PDCCH Queue */
            if (!(putEntryInULSchedulerNewTxQueue(ulStrategyTxNode_p, internalCellIndex
#ifdef TDD_CONFIG
                            ,currentGlobalTTITickCount
                            ,ulSubFrameNum
                            ,tempUplinkContext_p
#endif
                )))
            {
                /* ULA_CHG */
                LOG_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGERROR,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        0,0,0,0,0.0,0.0,__func__,
                        "ulStrategyTxNode_p entry failed");
                /* ULA_CHG */
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
                FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else                
                freeMemPool(ulStrategyTxNode_p);
#endif               
                /*coverity_530_55388*/
                ulStrategyTxNode_p = PNULL;
                /*coverity_530_55388*/

            }
            /* ULA_CHG */
            else
            {
                LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                        LOGDEBUG,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        ulStrategyTxNode_p->requiredRB,\
                        ulStrategyTxNode_p->strictAllocRB,\
                        ulStrategyTxNode_p->minDataSize,\
                        ulStrategyTxNode_p->maxDataSize,\
                        ulStrategyTxNode_p->avgMcs,\
                        ulStrategyTxNode_p->ttiCounter,\
                        __func__,"ulStrategyTxNode_p parameters");
            }
            /* ULA_CHG */
        }
        else
        {
            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                    ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
            if (!retValue)
            {
                lteWarning("Error in Pushing CQI in"
                        "ulGrantPendingQueue in"
                        " funtion %s\n",__func__);
                freeMemPool(pendingNode_p);
            }

            /* + SPR 8550 Fix */
            (*ulGrantPendingQueueCount)--;
            /* - SPR 8550 Fix */
            *ulStrategyTxNode_pp = ulStrategyTxNode_p;
            return MAC_FAILURE;
        }
        freeMemPool((void *)pendingNode_p);
        pendingNode_p = PNULL;
    }
    *ulStrategyTxNode_pp = ulStrategyTxNode_p;
    return MAC_SUCCESS;
}
/* SPR 17211 Fix End */
 /* SPR 16840 fix end */
/***************************************************************************************
 * Function Name  : processUlTpcWithAndWithoutData
 * Inputs         : ulGrantPendingQueueCount,
 *                  numberOfAvailableRBs_p,
 *                  currentGlobalTTITickCount,
 *                  ulDelay - UL Delay,
 *                  tempUplinkContext_p - Pointer to ULUEContext,
 *                  pendNode_p - Pointer to ULGrantRequestQueueNode,
 *                  ulStrategyTxNode_pp - Pointer to ULStrategyTxNode,
 *                  tempMaxUEsToBeScheduledUL_p - Maximum No. of UE's to be scheduled,
 *                  ulHarqProcessId - Uplink HARQ Process ID,
 *                  internalCellIndex - cell index used at MAC.
 *                  ulSubFrameNum - uplink subframe number
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function processes the UL TPC With or Without Data.
 ***************************************************************************************/
MacRetType processUlTpcWithAndWithoutData(UInt32 *ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p,
        ULStrategyTxNode **ulStrategyTxNode_pp,
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* SPR 15909 fix start */
    tickType_t ttiTick                          = 0;
    /* SPR 15909 fix end */
    MeasGapContextUL * measGapContext_p     = PNULL;
    UInt8 avgMcs                            = 0;
    UInt32 tempUeId                         = 0;
    UInt8 userLocationType = CC_USER ;
    UInt32 minDataSize                      = 0;
    UInt32 maxDataSize                      = 0;
    UInt8 mcsVal                            = 0;
    /* Coverity 30671 */
    ULGrantRequestQueueNode *pendingNode_p  = *pendNode_p;
    /* Coverity 30671 */
    ULGrantRequestType tempULGrantRequestType;
    UInt32 usedRB = 0;
    ULUeScheduleInfo *ulUeScheduleInfo_p = PNULL;
    ttiTick =  pendingNode_p->ttiCounter;
    measGapContext_p = &tempUplinkContext_p->measGapContext;
    avgMcs = tempUplinkContext_p->mcsIndex;
    tempUeId = pendingNode_p->ueIndex;
    userLocationType = tempUplinkContext_p->userLocationType ;
    tempULGrantRequestType = pendingNode_p->ulGrantRequestType;
    UInt8 retValue = 0;
    ULStrategyTxNode *ulStrategyTxNode_p = *ulStrategyTxNode_pp;
    /* CA Changes start */
    UInt8 ulSchedulerStrategy = cellSpecificParams_g.\
                                cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                                ulSchdConfig.ulSchedulerStrategy;
    /* CA Changes end */
    /* Phase2_CSI_Strategy_Changes start */
    if(0 == *tempMaxUEsToBeScheduledUL_p)
    {
        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                /*CA Changes end  */
                ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
        if (!retValue)
        {
            freeMemPool(pendingNode_p);
        }
        /* + SPR 8550 Fix */
        (*ulGrantPendingQueueCount)--;
        /* - SPR 8550 Fix */
        *ulStrategyTxNode_pp = ulStrategyTxNode_p;
        return MAC_FAILURE;
    }
    if ((DRX_STATE_ON == tempUplinkContext_p->
                drxCurrentState) || (TRUE == 
                    measGapContext_p->isMeasGapPeriodOn) ||
            (UL_SYNC_LOSS_ON == ulUECtxInfoArr_g[(UInt16)tempUeId].
             ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState))
    {
        LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_UL_LOG_ID, 
                LOGDEBUG, MAC_UL_Strategy,
                currentGlobalTTITickCount, 
                tempUeId, 
                tempUplinkContext_p->drxCurrentState,
                measGapContext_p->isMeasGapPeriodOn,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE, 
                DEFAULT_FLOAT_VALUE,
                __func__,"");
        /*CA Changes start  */
        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
        /*CA Changes end  */
                ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
        if (!retValue)
        {
            freeMemPool(pendingNode_p);
        }
        /* + SPR 8550 Fix */
        (*ulGrantPendingQueueCount)--;
        /* - SPR 8550 Fix */
        *ulStrategyTxNode_pp = ulStrategyTxNode_p;
        return MAC_FAILURE;
    }

    UInt32 tempNetBsrLoad = tempUplinkContext_p->bsrNetTotal;

    /*If the UE has zero BSR available*/
    if((!tempNetBsrLoad)||((tempNetBsrLoad &&
                    ( 
                     /* ICIC changes start */
                     numberOfAvailableRBs_p[userLocationType] 
                     /* ICIC changes end */
                     <
                     MAC_UL_GRANT_FOR_TPC_WITH_BSR_LOAD)) || 
                (ulSchedulerStrategy == UL_QOS_SCHEDULING)))
    {
#ifdef FDD_CONFIG 
        /* TTIB_Code Start */
        if ( tempUplinkContext_p->ttiBundlingEnabled )
        {
            if ( avgMcs > MAX_TTI_BUNDLE_MCS )
            {
                avgMcs = MAX_TTI_BUNDLE_MCS;
            }
            /*CA Changes start  */
            if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD ) 
                   || (MAC_FAILURE == processMeasGapValidation(
                       tempUplinkContext_p,currentGlobalTTITickCount,ulDelay )))
            {
                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                        ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
                if (!retValue)
                {
                    freeMemPool(pendingNode_p);
                }
                /* + SPR 8550 Fix */
                (*ulGrantPendingQueueCount)--;
                /* - SPR 8550 Fix */
                *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                return MAC_FAILURE;
            }
            availableRBForTtiBundlingPerTick_g[internalCellIndex] -= MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD;
            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                    "[%s][%d]:TTIB UE ID %d scheduled RB:1 MCS:%d \n",
                    __func__,getCurrentTick(),tempUeId,avgMcs);
            /*CA Changes end  */
        }
        /* TTIB_Code End */
#endif                                 
        /*Preparing the UL Strategy outPut Node*/
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL)) 
        GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else        
        GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p,
                sizeof(ULStrategyTxNode),PNULL);
#endif        

//Coverity_36249 fixed
           
	    if(PNULL == ulStrategyTxNode_p)
        {
            ltePanic("getMemFromPool fails %s", __func__);
		    /* Klockwork warning Changes Start */
		    return MAC_FAILURE;
		    /* Klockwork warning Changes End */
        }
        ulStrategyTxNode_p->requiredRB = 0;
        ulStrategyTxNode_p->maxDataSize = 0;
        ulStrategyTxNode_p->minDataSize = 0;

        (*tempMaxUEsToBeScheduledUL_p)--;

        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
            = ulReferenceFlag_g[internalCellIndex];
#endif
        /* TDD Config 0 Changes End */
            /*Optimization Start */
#ifdef TDD_CONFIG
            PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, (UInt16)tempUeId, 
                                        MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD, 
                                        TRUE, tempULGrantRequestType, ttiTick, ulHarqProcessId, 
                                        MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD, 
                                        determineUlTBSize(avgMcs, MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD), 
                                        avgMcs)
#else
            PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, (UInt16)tempUeId, 
                                        MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD, 
                                        TRUE, tempULGrantRequestType, ttiTick, MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD, 
                                        determineUlTBSize(avgMcs, MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD), 
                                        avgMcs)
#endif
            /*Optimization End */
        numberOfAvailableRBs_p[userLocationType] -= ulStrategyTxNode_p->requiredRB;

        if (userLocationType == CE_USER)
        {
            /*CA Changes start  */
            maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
            /*CA Changes end  */
            numberOfAvailableRBs_p[CC_CE_REGION] -= 
                ulStrategyTxNode_p->requiredRB;
        }

        else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                numberOfAvailableRBs_p[CE_REGION])
        {
            numberOfAvailableRBs_p[CE_REGION] = 
                numberOfAvailableRBs_p[CC_CE_REGION];
        }
        /* ICIC changes end */

        if (ulSchedulerStrategy  == UL_QOS_SCHEDULING)
        {
            /*putEntry of ULStrategyTXNode in UL Strategy Schedule Info */
            /*CA Changes start  */
            ulUeScheduleInfo_p = &(ulStrategyScheduleInfo_gp[internalCellIndex]->ulUeScheduleInfo
                    [ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount]);
            /*CA Changes end  */

            ulUeScheduleInfo_p->ulStrategyTxNode_p = ulStrategyTxNode_p;

            ulUeScheduleInfo_p->ueULContext_p = tempUplinkContext_p;

            tempUplinkContext_p->ulStrategyTxNode_p = ulStrategyTxNode_p;

            /* Increament the scheduled UE count */
            /*CA Changes start  */
            ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount++;
            /*CA Changes end  */

            freeMemPool((void *)pendingNode_p);
            pendingNode_p = PNULL;

            /* CA Stats Changes Start */
            /*debug stats*/
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_AND_PENDING_AP_CQI(
                    internalCellIndex);
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_CQI(
                    internalCellIndex);
            /* CA Stats Changes End */

        }
        else
        {
            /*putEntry of ULStrategyTXNode in PDCCH Queue */
            /*CA Changes start  */
            if (!(putEntryInULSchedulerNewTxQueue(ulStrategyTxNode_p, internalCellIndex
            /*CA Changes end  */
                            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                            ,currentGlobalTTITickCount
                            ,ulSubFrameNum
                            ,tempUplinkContext_p
#endif
                            /* TDD Config 0 Changes End */
                            )))
            {
                /* +- SPR 17777 */
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGERROR,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        ulDelay,0,0,0,0.0,0.0,__func__,
                        "ulStrategyTxNode_p entry failed");
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL)) 
                FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else                
                freeMemPool(ulStrategyTxNode_p);
#endif                
                /*coverity_530_55391-II*/
                ulStrategyTxNode_p = PNULL;
                /*coverity_530_55391-II*/
            }
            else
            {
                LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                        LOGDEBUG,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        ulStrategyTxNode_p->requiredRB,\
                        ulStrategyTxNode_p->strictAllocRB,\
                        ulStrategyTxNode_p->minDataSize,\
                        ulStrategyTxNode_p->maxDataSize,\
                        ulStrategyTxNode_p->avgMcs,\
                        ulStrategyTxNode_p->ttiCounter,\
                        __func__,"ulStrategyTxNode_p parameters");
            }
            freeMemPool((void *)pendingNode_p);
            pendingNode_p = PNULL;
        }
    }
    /*else if the UE has non zero BSR*/
    else
    {
        UInt32 strictAllocRBFlag = FALSE;
        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
            = ulReferenceFlag_g[internalCellIndex];
#endif
        /* TDD Config 0 Changes End */
        usedRB = validateUplinkRB ( 
                tempUplinkContext_p, 
                MAC_UL_GRANT_FOR_TPC_WITH_BSR_LOAD,
                numberOfAvailableRBs_p[userLocationType],
                &strictAllocRBFlag
                ,&minDataSize,
                &maxDataSize,
                /* ATB Changes Start */
                &mcsVal,
                /*SPR 16417 Changes Start*/
#ifdef FDD_CONFIG
                /* SPR 13532 Fix Start */
                (currentGlobalTTITickCount + ulDelay),
                /* SPR 13532 Fix End */
#elif TDD_CONFIG
                currentGlobalTTITickCount,
                ulSubFrameNum,
#endif
                /*SPR 16417 Changes End*/
                internalCellIndex);
        /* ATB Changes End */

        if (usedRB)
        {
#ifdef FDD_CONFIG
            /* TTIB_Code Start */
            if ( tempUplinkContext_p->ttiBundlingEnabled )
            {
                if ( mcsVal > MAX_TTI_BUNDLE_MCS )
                {
                    mcsVal = MAX_TTI_BUNDLE_MCS;    
                }
                if ( usedRB > MAX_TTI_BUNDLE_RB )
                {
                    usedRB = MAX_TTI_BUNDLE_RB;
                }
                maxDataSize = determineUlTBSize(mcsVal,usedRB);
                minDataSize = determineUlTBSize(mcsVal,MIN_TTI_BUNDLE_RB);
                /*CA Changes start  */
                if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < usedRB ) 
                      ||(MAC_FAILURE == processMeasGapValidation(
                       tempUplinkContext_p,currentGlobalTTITickCount,ulDelay )))
                {
                    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                            ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
                    if (!retValue)
                    {
                        freeMemPool(pendingNode_p);
                    }
                    /* + SPR 8550 Fix */
                    (*ulGrantPendingQueueCount)--;
                    /* - SPR 8550 Fix */
                    *ulStrategyTxNode_pp = ulStrategyTxNode_p;
                    return MAC_FAILURE;
                }
                availableRBForTtiBundlingPerTick_g[internalCellIndex] -= usedRB; 
                LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                        "[%s][%d]:TTIB UE ID %d scheduled RB:%d MCS:%d \n",
                        __func__,getCurrentTick(),tempUeId,usedRB,mcsVal);
                /*CA Changes end  */
            }
            /* TTIB_Code End */
#endif                                 
            /*Preparing the UL Strategy outPut Node*/
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))            
            GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else            
            GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p,
                    sizeof(ULStrategyTxNode),PNULL);
#endif            
            /*cov_warning 32649 fix start*/
            if(PNULL == ulStrategyTxNode_p)
            {
                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                        ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
                if (!retValue)
                {
                    freeMemPool(pendingNode_p);
                }
                /* + SPR 8550 Fix */
                (*ulGrantPendingQueueCount)--;
                /* - SPR 8550 Fix */
                ltePanic("Memory Allocation failed in %s\n",__func__);
                return MAC_FAILURE;
            }
            ulStrategyTxNode_p->requiredRB = 0;
            ulStrategyTxNode_p->maxDataSize = 0;
            ulStrategyTxNode_p->minDataSize = 0;

            /*cov warning 32649 fix end */

            /* ICIC changes start */
            numberOfAvailableRBs_p[userLocationType] -= usedRB;

            /* ATB Changes Start */
            avgMcs = mcsVal;
            /* ATB Changes End */

            if (userLocationType == CE_USER)
            {
                /*CA Changes start  */
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                /*CA Changes end  */
                numberOfAvailableRBs_p[CC_CE_REGION] -= usedRB;
            }

            else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                    numberOfAvailableRBs_p[CE_REGION])
            {
                numberOfAvailableRBs_p[CE_REGION] = 
                    numberOfAvailableRBs_p[CC_CE_REGION];
            }
            /* ICIC changes end */

            /*Optimization Start */
#ifdef TDD_CONFIG
            PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, (UInt16)tempUeId, usedRB, strictAllocRBFlag, 
                                        UL_TPC_WITH_DATA, ttiTick, ulHarqProcessId, minDataSize, 
                                        maxDataSize, avgMcs)
#else
            PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, (UInt16)tempUeId, usedRB, strictAllocRBFlag, 
                                        UL_TPC_WITH_DATA, ttiTick, minDataSize, maxDataSize, avgMcs)
#endif
            /*Optimization End */
            /*putEntry of ULStrategyTXNode in 
              PDCCH Queue */
            /*CA Changes start  */
            if (!(putEntryInULSchedulerNewTxQueue(ulStrategyTxNode_p, internalCellIndex
            /*CA Changes end  */
                            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                            ,currentGlobalTTITickCount
                            ,ulSubFrameNum
                            ,tempUplinkContext_p
#endif
                            /* TDD Config 0 Changes End */
                            )))
            {
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGERROR,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        0,0,0,0,0.0,0.0,__func__,
                        "ulStrategyTxNode_p entry failed");
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL)) 
                FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else                
                freeMemPool(ulStrategyTxNode_p);
#endif                
                /*coverity_530_55391*/
                ulStrategyTxNode_p = PNULL;
                /*coverity_530_55391*/
            }
            else
            {
                LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                        LOGDEBUG,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        ulStrategyTxNode_p->requiredRB,\
                        ulStrategyTxNode_p->strictAllocRB,\
                        ulStrategyTxNode_p->minDataSize,\
                        ulStrategyTxNode_p->maxDataSize,\
                        ulStrategyTxNode_p->avgMcs,\
                        ulStrategyTxNode_p->ttiCounter,\
                        __func__,"ulStrategyTxNode_p parameters");
            }
        }
        else
        {
            /*CA Changes start  */
            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
            /*CA Changes end  */
                    ULGrantRequestQueueNode,&(pendingNode_p->ulGrantRequestNodeAnchor));
            if (!retValue)
            {
                freeMemPool(pendingNode_p);
                pendingNode_p = PNULL;
            }

            /* + SPR 8550 Fix */
            (*ulGrantPendingQueueCount)--;
            /* - SPR 8550 Fix */
            *ulStrategyTxNode_pp = ulStrategyTxNode_p;
            return MAC_FAILURE;
        }
        (*tempMaxUEsToBeScheduledUL_p)--;
        freeMemPool((void *)pendingNode_p);
        pendingNode_p = PNULL;
    }
    *ulStrategyTxNode_pp = ulStrategyTxNode_p;
    return MAC_SUCCESS;
}

    /* Coverity 30671 */
/***************************************************************************************
 * Function Name  : processDefaultReqMsg
 * Inputs         : pendingNode_p - Pointer to ULGrantRequestQueueNode,
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Default Request message.
 ***************************************************************************************/
MacRetType processDefaultReqMsg( UInt32 *ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        ULUEContext * tempUplinkContext_p,
        ULGrantRequestQueueNode **pendNode_p,
        ULStrategyTxNode **ulStrategyTxNode_pp,
        UInt8 * tempMaxUEsToBeScheduledUL_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    freeMemPool((void *)*pendNode_p);
    *pendNode_p = PNULL;
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(ulGrantPendingQueueCount)
  LTE_GCC_UNUSED_PARAM(numberOfAvailableRBs_p)
  LTE_GCC_UNUSED_PARAM(currentGlobalTTITickCount)
  LTE_GCC_UNUSED_PARAM(ulDelay)
  LTE_GCC_UNUSED_PARAM(tempUplinkContext_p)
  LTE_GCC_UNUSED_PARAM(ulStrategyTxNode_pp)
  LTE_GCC_UNUSED_PARAM(tempMaxUEsToBeScheduledUL_p)
  LTE_GCC_UNUSED_PARAM(internalCellIndex)
#ifdef TDD_CONFIG
  LTE_GCC_UNUSED_PARAM(ulHarqProcessId)
  LTE_GCC_UNUSED_PARAM(ulSubFrameNum)
#endif

    lteWarning("[UL-Strategy][%s]Request Received is\
            of New Transmission Type, Ignoring it");

    return MAC_SUCCESS;
}
/* TTIB Code Optimization Changes End */

/*****************************************************************************
 * Function Name    : processPendingULGrantQueue
 * Inputs           : ulPendingQueue - Pointer to UL_GRANT_PUSCH_QUEUE_TYPE,
 *                    ulGrantPendingQueueCount - Its the total Nodes present in
 *                                               the ulGrantPendingQueue which
 *                                               needs to be processed,
 *                    numberOfAvailableRBs_p - pointer to current available RBs,
 *                    maxUEsToBeScheduledUL_p - pointer to the max UE to be
 *                                              scheduled in UL,
 *                    currentGlobalTTITickCount - Current Global Tick,
 *                    ulDelay- The amount of delay expected in the UpLink path,
 *                    internalCellIndex - cell index used at MAC.
 *                    ulSubFrameNum - ulSubFrameNumber for which grant is 
 *                                    being given
 * Outputs        :   Allocates Uplink RBs - It will allocate RBs to Queue Nodes 
 *                                           & pass the UL Strategy output to the
 *                                           PDCCH module
 * Returns        :   None
 * Description    :   All the UEs, which were not taken care in the last 
 *                    execution of UL Strategy, were enqueued in 
 *                    ulGrantPendingQueue Queue. Its function is to process 
 *                    the UEs present in this Queue.
 *****************************************************************************/
/*SPR 22919 Start*/
 void processPendingULGrantQueue(
/*SPR 22919 End*/
        UL_GRANT_PUSCH_QUEUE_TYPE * ulPendingQueue,
        UInt32 ulGrantPendingQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        InternalCellIndex  internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        )
        /* TDD Config 0 Changes End */
{
    /* + Coverity 54523 */
    ULGrantRequestQueueNode *pendingNode_p  = PNULL;
    UInt32 tempUeId                         = 0;
    ULStrategyTxNode *ulStrategyTxNode_p    = PNULL;
    ULUEContext * tempUplinkContext_p       = PNULL;
    ULUEContextInfo * tempUplinkContextInfo_p = PNULL;
    ULGrantRequestType tempULGrantRequestType;
    UInt8 tempMaxUEsToBeScheduledUL        = *maxUEsToBeScheduledUL_p;
    UInt8 userLocationType = CC_USER ;
    UInt32 checkPendingULGrantQueue = 0;
    UInt8  isEntryInULGrantPuschQueue = 0;
#ifdef FDD_CONFIG 
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2 = TTI_MAP_FREE;
#endif    
    UInt8 ret = MAC_SUCCESS;
    UInt8 retValue = 0;


    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Entry:[UL-Strategy][%s]",
            __func__);

        checkPendingULGrantQueue = (ulGrantPendingQueueCount)&&(numberOfAvailableRBs_p[CC_CE_REGION]);
    /*Processing for Queue Count and till the Available RBs*/
    while(checkPendingULGrantQueue) 
            /*Phase2_CSI_Strategy_Changes( tempMaxUEsToBeScheduledUL)*/
        /* Note- As part of CA Phase2 CSI changes, this check of
         * tempMaxUEsToBeScheduledUL is removed because if multiple entries of
         * same ue are present in queue, one for Pcell and other for Scell, then
         * we have to club the two nodes into one strategy node for CA QOS
         * scheduling*/
    {
        ulStrategyTxNode_p    = PNULL;



            checkPendingULGrantQueue = (ulGrantPendingQueueCount)&&(numberOfAvailableRBs_p[CC_CE_REGION]);

        if(!checkPendingULGrantQueue )
        {
            break;
        }
        /* COVERITY CID 30671 FIX START */
        pendingNode_p = PNULL;
        /* COVERITY CID 30671 FIX END */
        DEQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                ulPendingQueue,ULGrantRequestQueueNode,(void**)&pendingNode_p);
        if (pendingNode_p)
        {
            tempULGrantRequestType = pendingNode_p->ulGrantRequestType;
            tempUeId = pendingNode_p->ueIndex;
            tempUplinkContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
            tempUplinkContext_p = tempUplinkContextInfo_p->ulUEContext_p;
            /*SPR 3146 start */
            /*Checking for UE context */
            if (!(tempUplinkContextInfo_p->pendingDeleteFlag))
            {
                /*SPR 3146 end */
                /* ICIC Change start */
                userLocationType = tempUplinkContext_p->userLocationType ;
		/* EICIC Optimization +- */
                    isEntryInULGrantPuschQueue = (isEicicEnabled_g[internalCellIndex]);
                if(isEntryInULGrantPuschQueue)
		{

			  /* EICIC +*/
		          /* Pushing UE back in queue.*/
		      /*Complexity Start*/      
			  if (((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == tempUplinkContext_p->userType))
			  	||((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == tempUplinkContext_p->userType)))
			  {              
				  retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(                            
						  ulPendingQueue,ULGrantRequestQueueNode,                            
						  &(pendingNode_p->ulGrantRequestNodeAnchor)); 
				  if (!retValue)
				  {
					  lteWarning("Error in Pushing "
							  "ulPendingQueue in"
							  " funtion %s\n",__func__);

					  freeMemPool((void *)pendingNode_p);
					  pendingNode_p = PNULL;       
				  }
				  ulGrantPendingQueueCount--;
				  continue;
			  }
		}
		/* EICIC Optimization +- */


		/*Complexity End*/
           	/* EICIC -*/

                /* If user is cell ege then we are checking if 
                   maxCellEdgeUEsToBeScheduledUL_g is reached
                   or cell edge region RB is not available */
                if(tempMaxUEsToBeScheduledUL)
                {
                    if ((CE_USER == userLocationType && (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]
                                    || !numberOfAvailableRBs_p[userLocationType])) 
#ifdef HD_FDD_CONFIG
                            ||
                            ((TRUE == tempUplinkContext_p->isHDFddFlag)&&
                             (!CAN_SCHEDULE_UL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY, \
                                                         tempUeId , hdUeContextInfo_g.hdfddUeSchedMap )))
#endif
                       )
                    {

                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(pendingNode_p->ulGrantRequestNodeAnchor)); 
                        if (!retValue)
                        {
                            lteWarning("Error in Pushing "
                                    "ulGrantPendingQueue in"
                                    " funtion %s\n",__func__);
                            FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);        
                        }
                        ulGrantPendingQueueCount--;
#ifdef HD_FDD_CONFIG
                        if(TRUE == tempUplinkContext_p->isHDFddFlag)
                        {
                            /* Increment the statistics counter UE denied UL resources due to
                             * DL clash in the same TTI of HD FDD UE*/
                            UPDATE_UE_DENIED_UL_RESOURCE(tempUeId);
                        }
#endif
                        continue;
                    }
                }

                /* \code
                 * if schType == SPS_SCHEDULING, then do not schedule this UE.
                 * */
                if(tempMaxUEsToBeScheduledUL)
                {
                    if( SPS_SCHEDULING == tempUplinkContext_p->schType )
                    {
                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(pendingNode_p->ulGrantRequestNodeAnchor)); 
                        if (!retValue)
                        {
                            FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);        
                        }
                        ulGrantPendingQueueCount--;
                        continue;
                    }
                }
#ifdef TDD_CONFIG
                /* TDD Config 0 and 6 Changes Start */
    /* SPR 23393 Changes Start */
                UInt32 subframe     = 0;
                UInt32 sysFrameNum = 0;
                GET_SFN_SF_FROM_TTI((currentGlobalTTITickCount + ulDelay), sysFrameNum, subframe)
                    /* SPR 23393 Changes End */
                    /* +- SPR 17777 */
              LTE_GCC_UNUSED_PARAM(sysFrameNum)
                UInt8 ulHarqProcessId = BUSY_ALL_HARQ;
                /* Get the free ID in ulHarqProcessId  */
    		    /* SPR 11257 Changes Start*/
                ulHarqProcessId = getFreeULHarqId(tempUplinkContext_p,subframe,internalCellIndex,
				            ulSubFrameNum,currentGlobalTTITickCount);
                /* +- SPR 17777 */
                /* SPR 11257 Changes End*/
                /* TDD Config 0 and 6 Changes End*/
#elif FDD_CONFIG
                UInt8 ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + 
                        ulDelay + FDD_HARQ_OFFSET);
                if ( tempUplinkContext_p->ttiBundlingEnabled )
                {
                    ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
                }
                ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + 
                        ulDelay + FDD_HARQ_OFFSET);
                ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + 
                        ulDelay + FDD_HARQ_OFFSET + MAX_HARQ_PROCESS_NB );
#endif                
                /* COVERITY 69296 START */
#ifdef TDD_CONFIG             
                /*SPR 19852 fix start*/
                if(BUSY_ALL_HARQ == ulHarqProcessId)
                /*SPR 19852 fix end*/
#elif FDD_CONFIG
                    if(tempMaxUEsToBeScheduledUL)
                    {
                /* COVERITY 69296 END */
                        /* TTIB_Code Start */
                        if((HARQ_PROCESS_FREE != tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                                ||(tempUplinkContext_p->ttiBundlingEnabled &&
                                    (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,tempUplinkContext_p)))
                                ||(!(tempUplinkContext_p->ttiBundlingEnabled)&&
                                    ((TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId1]) ||
                                     ( TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId2])))
                                /*SPR 7944 START*/
                                /* + SPS_TDD_Changes */
                                /*
                                   ||checkSpsOccasionTti(tempUplinkContext_p,ulHarqProcessId) 
                                   ||checkSpsActTti(tempUplinkContext_p,ulHarqProcessId)
                                   */
                                /* - SPS_TDD_Changes */
                                /*SPR 7944 END*/     
                          )  
#endif                 
                        {
                            /*lteWarning("\n[%s]ALL Harq Process Id is busy\n",
                              __func__);*/
#ifdef FDD_CONFIG                        
                            if(HARQ_PROCESS_RESET == tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                            {
                                freeMemPool((void *)pendingNode_p);
                                pendingNode_p = PNULL;
                            }
                            else 
#endif                    
                                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                        /*CA Changes start  */
                                        ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                        /*CA Changes end  */
                                        &(pendingNode_p->ulGrantRequestNodeAnchor));

                            if (!retValue)
                            {
                                lteWarning("Error in Pushing "
                                        "ulGrantPendingQueue in"
                                        " funtion %s\n",__func__);
                                if(pendingNode_p)
                                { 
                                    FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);        
                                }
                            }
                            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                                    "[%s]:UE ID %d is not schedule due to busy HARQ"\
                                    " process Global Tick:%d\n",
                                    __func__,tempUeId,currentGlobalTTITickCount);
                            ulGrantPendingQueueCount--;
                            continue;
                        }
/* COVERITY 69296 START */
#ifdef FDD_CONFIG
                    }
#endif
/* COVERITY 69296 END */
                /*Checking if Already being considered for RB Distribution*/
                /* check if maxuetobeschedule ==0  
                 * then no processing is to be done */
        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                if ((tempUplinkContext_p->isAlreadyConsideredFlag != 
						ulReferenceFlag_g[internalCellIndex] )
						&&
                       (tempULGrantRequestType != CQI_WITH_DATA_ULGRANT_REQ_MSG)&&
                       (tempULGrantRequestType != CQI_WITHOUT_DATA_ULGRANT_REQ_MSG))
#elif TDD_CONFIG
				if ((tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                    != ulReferenceFlag_g[internalCellIndex] )
						&&
                        (tempULGrantRequestType != CQI_WITH_DATA_ULGRANT_REQ_MSG)&&
                        (tempULGrantRequestType != CQI_WITHOUT_DATA_ULGRANT_REQ_MSG))
#endif
        /* TDD Config 0 Changes End */
                {
                    ret = ulGrantStateMachine[tempULGrantRequestType]( 
                            &ulGrantPendingQueueCount, numberOfAvailableRBs_p,
                            currentGlobalTTITickCount, ulDelay,
                            tempUplinkContext_p, &pendingNode_p,
                            &ulStrategyTxNode_p, &tempMaxUEsToBeScheduledUL,
                            /* +- SPR 17777 */
                            internalCellIndex 
                            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                            /* +- SPR 17777 */
                            ,ulHarqProcessId
                            ,ulSubFrameNum
#endif
                            /* TDD Config 0 Changes End */
                            );
                    if( MAC_FAILURE == ret )
                    {
                        continue;
                    }
                }
                else
                {
                    if((tempULGrantRequestType != CQI_WITH_DATA_ULGRANT_REQ_MSG)
                            && (tempULGrantRequestType != CQI_WITHOUT_DATA_ULGRANT_REQ_MSG))
                    {
                        /*UE is already considered so unable to process the pending
                          UL Grant Node hence Pushing into ulGrantPendingQueue */

                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                /*CA Changes start  */
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                /*CA Changes end  */
                                &(pendingNode_p->ulGrantRequestNodeAnchor));

                        if (!retValue)
                        {
                            lteWarning("Error in Pushing UL Grant Node in"
                                    "ulGrantPendingQueue in"
                                    " funtion %s\n",__func__);
                            FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(pendingNode_p);        
                        }
                    }
                    else
                    {
                        ret = ulGrantStateMachine[tempULGrantRequestType]( 
                                &ulGrantPendingQueueCount, 
                                numberOfAvailableRBs_p,
                                currentGlobalTTITickCount, ulDelay,
                                tempUplinkContext_p, &pendingNode_p,
                                &ulStrategyTxNode_p, 
                                /* +- SPR 17777 */
                                &tempMaxUEsToBeScheduledUL,
                                internalCellIndex
                                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                                /* +- SPR 17777 */
                                ,ulHarqProcessId
                                ,ulSubFrameNum
#endif
                                /* TDD Config 0 Changes End */
                                );
                        if(ret == MAC_FAILURE)
                            continue;
                    }

                    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
                            "[%s]:UE ID %d is already considered, hence NOT considering\
                            UE,Adding UE in UL Grant Pending Queue",
                            __func__,tempUeId);
                }
                if (NULL != ulStrategyTxNode_p)
                {
                    LOG_MAC_MSG (UL_PEND_GRANT_QUEUE_SCHEDULED_LOG_ID, LOGDEBUG,
                            MAC_UL_Strategy, getCurrentTick (), tempUplinkContext_p->ueIndex,
                            ulHarqProcessId,
                            ulStrategyTxNode_p->requiredRB,
                            tempUplinkContext_p->bsrNetTotal,
                            ulStrategyTxNode_p->ulGrantRequestType,
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
                }
            }        
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,"[UL-Strategy]"
                        "[%s]UE ID %d Context either deleted or is to be deleted.\n",
                        __func__,tempUeId);                
                /*Freeing the  Node*/
                freeMemPool((void *)pendingNode_p);
                pendingNode_p = PNULL;
            }
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,"[UL-Strategy]"
                    "[%s]Pending UL GRANT Node is not valid...\n",__func__);
            break;
        }
        ulGrantPendingQueueCount--;
    }
    *maxUEsToBeScheduledUL_p = tempMaxUEsToBeScheduledUL ;
    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]",
            __func__);
    /* - Coverity 54523 */
}

/******************************************************************************
 * Function Name  : processContentionResULGrantQueue
 * Inputs         : contentionResolutionQueueCount - Its the total Nodes present 
 *                                                  in contentionResolutionQueue,
 *                                                  which needs to be processed,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  maxUEsToBeScheduledUL_p - pointer to the max UE to be
 *                                            scheduled in UL,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  ulDelay - The total amount of delay 
 *                            expected in the UpLink path,
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : Allocate RBs to Contention Resolution - It will allocate RBs
 *                  to CR Queue Nodes & pass the UL Strategy output to the PDCCH
 *                  module.
 * Returns        : None
 * Description    : This will be allocating RBs to the UE whose contention 
 *                  resolution was pending and who already had CRNTI. 
 *                  So Allocating RBs to those UEs to complete the contention 
 *                  resolution.
 *********************************************************************************/
/*SPR 22919 Start*/
 void processContentionResULGrantQueue(
/*SPR 22919 End*/
        UInt32 contentionResolutionQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
    /* +- SPR 18490 */

    ULGrantRequestQueueNode *crNode_p   = PNULL;
    /* SPR 15909 fix start */
    tickType_t expiryTick                   = 0;
    /* SPR 15909 fix end */
    UInt32 tempUeId                     = 0;
    ULStrategyTxNode *ulStrategyTxNode_p= PNULL;
    ULUEContext * tempUplinkContext_p   = PNULL;
    ULUEContextInfo * tempUplinkContextInfo_p = PNULL;
    UInt32 tempMaxUEsToBeScheduledUL    = *maxUEsToBeScheduledUL_p;
    /* ULA_CHG */
    UInt8 avgMcs			            = 0;
    UInt8 minRb                         = 0;
    /* ULA_CHG */

    /* ICIC Change start */
    UInt8 userLocationType = CC_USER ;
    /* ICIC Change End */
#ifdef FDD_CONFIG 
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2 = TTI_MAP_FREE;
#endif
    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,
            "Entry:[UL-Strategy][%s]",__func__);
    UInt8 retValue = 0;

    /* EICIC +*/
        ULGrantRequestQueueNode *crNodeScan_p   = PNULL;
	UInt8	isTraverseList = LTE_FALSE;
    /* EICIC -*/

    /*Processing for Queue Count and till the Available RBs*/
    /* ULA_CHG */
    /* ICIC changes start */
    while ((contentionResolutionQueueCount)&&(numberOfAvailableRBs_p[CC_CE_REGION] )&&
            (tempMaxUEsToBeScheduledUL))
        /* ICIC changes end */
        /* ULA_CHG */
    {
        crNode_p =  (ULGrantRequestQueueNode *)popNode 
            /*CA Changes start  */
            (ulGrantContentionResQueue_gp[internalCellIndex]);
            /*CA Changes end  */
        if (crNode_p)
        {
            tempUeId = crNode_p->ueIndex;
            expiryTick =  crNode_p->ttiCounter;
            tempUplinkContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
            tempUplinkContext_p = tempUplinkContextInfo_p->ulUEContext_p;
            /*SPR 3146 start */
            /*Checking for UE context */
            if (!(tempUplinkContextInfo_p->pendingDeleteFlag))
            {
		    /* EICIC +*/
		    /* EICIC Optimization +- */
		    if(isEicicEnabled_g[internalCellIndex])
		    {
		    /* Pushing UE back in trigger queue.*/
			    /*Complexity Start*/
		    if (((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == tempUplinkContext_p->userType))
				||((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == tempUplinkContext_p->userType)))
		    {              
			    retValue = pushNode(ulGrantContentionResQueue_gp[internalCellIndex], &crNode_p->ulGrantRequestNodeAnchor);

			    if (retValue)
			    {
				    lteWarning("Error in Pushing "
						    "ulGrantContentionResQueue_gp in"
						    " funtion %s\n",__func__);
				    freeMemPool(crNode_p);									        			
				    crNode_p = PNULL;

			    }
			    isTraverseList = LTE_TRUE;
			    contentionResolutionQueueCount--;
			    continue;
		    }
		    }
		    /* EICIC Optimization +- */
		    /*Complexity End*/
		    /* EICIC - */
                /* ICIC Change start */
                userLocationType = tempUplinkContext_p->userLocationType ;

                /* If user is cell edge then we are checking if
                   maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] is reached
                   or cell edge region RB is not available */

                /* \code
                 * if schType == SPS_SCHEDULING, then do not schedule this UE.
                 * */
                /*CA Changes start  */
                if (CE_USER == userLocationType && (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]
                /*CA Changes end  */
                            || !numberOfAvailableRBs_p[userLocationType]))

                {
                    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                            /*CA Changes start  */
                            ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                            /*CA Changes end  */
                            &(crNode_p->ulGrantRequestNodeAnchor));

                    if (!retValue)
                    {
                        lteWarning("Error in Pushing "
                                "ulGrantPendingQueue in"
                                " funtion %s\n",__func__);
                        FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(crNode_p);        
                    }
                    contentionResolutionQueueCount--;
                    continue;

                }
                /*SPR 3146 end */

                /* \code
                 * if schType == SPS_SCHEDULING, then do not schedule this UE.
                 * */
                if( SPS_SCHEDULING == tempUplinkContext_p->schType )
                {
                    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                            /*CA Changes start  */
                            ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                            /*CA Changes end  */
                            &(crNode_p->ulGrantRequestNodeAnchor));

                    if (!retValue)
                    {
                        FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(crNode_p);        
                    }
                    contentionResolutionQueueCount--;
                    continue;
                }
                /* +- SPR 18490 */
#ifdef TDD_CONFIG
    /* SPR 23393 Changes Start */
                UInt32 sysFrameNum = 0;
                UInt32 subframe =     0;
                GET_SFN_SF_FROM_TTI((currentGlobalTTITickCount + ulDelay), sysFrameNum, subframe)
    /* SPR 23393 Changes End */
                LTE_GCC_UNUSED_PARAM(sysFrameNum)
                /* +- SPR 18490 */
                UInt8 ulHarqProcessId = BUSY_ALL_HARQ;
                /* TDD Config 0 and 6 Changes Start */
                /* +- SPR 17777 */
                /* Get the free ID in ulHarqProcessId  */
                /* SPR 11257 Changes Start*/
                ulHarqProcessId = getFreeULHarqId(tempUplinkContext_p,subframe,internalCellIndex,
                        ulSubFrameNum,currentGlobalTTITickCount);
                /* +- SPR 17777 */
                /* SPR 11257 Changes End*/
                /* TDD Config 0 and 6 Changes End*/
#elif FDD_CONFIG
                    /* TTIB_Code Start */
                UInt8 ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
                if ( tempUplinkContext_p->ttiBundlingEnabled )
                {
                    ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
                }
                ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
                ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET + 
                MAX_HARQ_PROCESS_NB);
                    /* TTIB_Code End */
#endif            
#ifdef TDD_CONFIG            
                if ( BUSY_ALL_HARQ == ulHarqProcessId)
#elif FDD_CONFIG
                    /* TTIB_Code Start */
                    if((HARQ_PROCESS_FREE != tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                                ||(tempUplinkContext_p->ttiBundlingEnabled &&
                                    (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,tempUplinkContext_p)))
                                ||(!(tempUplinkContext_p->ttiBundlingEnabled)&&
                                    ((TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId1]) ||
                                     ( TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId2])))
                                /*SPR 7944 START*/
                                /* + SPS_TDD_Changes*/
                                /*
                                   || checkSpsOccasionTti(tempUplinkContext_p,ulHarqProcessId)
                                   || checkSpsActTti(tempUplinkContext_p,ulHarqProcessId)
                                   */
                                /* - SPS_TDD_Changes */
                                /*SPR 7944 END*/
                      )
                    /* TTIB_Code End */
#endif                
                    {                
                        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY, "failure scenario in ul strategy\n");
#ifdef FDD_CONFIG                
                        if(HARQ_PROCESS_RESET == tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                        {
                            freeMemPool((void *)crNode_p);
                            crNode_p = PNULL;                    
                        }
                        else 
#endif
                        /*CA Changes start  */
                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(crNode_p->ulGrantRequestNodeAnchor));
                        /*CA Changes end  */

                        if (!retValue)
                        {
                            lteWarning("Error in Pushing "
                                    "ulGrantPendingQueue in"
                                    " funtion %s\n",__func__);
                            if(crNode_p)
                            {   
                                FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(crNode_p);        
                            }
                        }
                        contentionResolutionQueueCount--;
                        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                            "[%s]:UE ID %d is not schedule due to busy HARQ"\
                            " process Global Tick:%d\n",
                            __func__,tempUeId,currentGlobalTTITickCount);
                        continue;
                    } 
                /*Checking if Already being considered for RB Distribution*/
        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
				if (tempUplinkContext_p->isAlreadyConsideredFlag != 
						ulReferenceFlag_g[internalCellIndex] )
#elif TDD_CONFIG
				if (tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                    != ulReferenceFlag_g[internalCellIndex] )
#endif
        /* TDD Config 0 Changes End */
                {
                    LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                            "[UL-Strategy][%s]Contention Resoultion Request\
                            Received:Contention Res Queue Cnt:%d \
                            Glbl TTI:%d,CR Expiry Tick %d \n",__func__ ,
                            contentionResolutionQueueCount,
                            currentGlobalTTITickCount,expiryTick);

                    /*If the Contention Resoluion Request is expired,
                      delete that CR Request*/
                    if (((currentGlobalTTITickCount + ulDelay 
                                    >= (expiryTick))))
                    {
                        LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                                "[UL-Strategy][%s]Contention Resolution \
                                Timer is expired... deleting Contention \
                                Resolution \n",__func__);
                        /*Freeing the CR Node*/
                        freeMemPool((void *)crNode_p);
                        crNode_p = PNULL;                    
                    }
                    /*If CR timer is not Expired Then Send Allocation for CR*/
                    else
                    {
                        /* ULA_CHG */
                        avgMcs = tempUplinkContext_p->mcsIndex;

                        /* SPR 5713 changes start */
                        if (avgMcs >=  MIN_GRANT_MCS_INDEX )
                        {
                            /* ICIC changes start */
                            if (numberOfAvailableRBs_p[userLocationType] < 
                                    MAC_UL_GRANT_FOR_CONTENTION_RES)
                            /* ICIC changes end */
                            {
                                /*Available number of RBs is less, unable to 
                                  process CR Node hence Pushing into 
                                  ulGrantPendingQueue */

                                /*CA Changes start  */
                                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                        ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                        &(crNode_p->ulGrantRequestNodeAnchor));
                                /*CA Changes end  */

                                if (!retValue)
                                {
                                    /* +- SPR 18490 */
                                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                            LOGWARNING,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            /* SPR_2446 start */
                                            crNode_p->ueIndex,\
                                            /* SPR_2446 end */
                                            0,0,0,0,0.0,0.0,__func__,\
                                            "Error in Pushing CR node in\
                                            ulGrantPendingQueue");
                                    /* +- SPR 18490 */
                                    FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(crNode_p);        
                                }
                                minRb = 0;
                            }
                            else
                            {
                                minRb = MAC_UL_GRANT_FOR_CONTENTION_RES;
                            }
                        } 
                        else
                        {
                            CALCULATE_MAC_MIN_UL_GRANT_RBs(avgMcs,minRb,tempUplinkContext_p->ueComplianceRelease);
                            /* ICIC changes start */
                            if(numberOfAvailableRBs_p[userLocationType] < minRb)
                            /* ICIC changes end */
                            {
                                /*Available number of RBs is less, unable to 
                                  process CR Node hence Pushing into 
                                  ulGrantPendingQueue */

                                /*CA Changes start  */
                                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                        ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                        &(crNode_p->ulGrantRequestNodeAnchor));
                                /*CA Changes end  */

                                if (!retValue)
                                {
                                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                            LOGWARNING,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            /* SPR_2446 start */
                                            crNode_p->ueIndex,\
                                            /* SPR_2446 end */
                                            0,0,0,0,0.0,0.0,__func__,
                                            "Error in Pushing CR node in\
                                            ulGrantPendingQueue");
                                    FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(crNode_p);        
                                }
                                minRb = 0;
                            } /* available PRbs < min UL grant*/
                        }
                        /* SPR 5713 changes end */

                        if(minRb) 
                        {
                            /* ULA_CHG */
                            /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                            GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
                            GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                                    sizeof(ULStrategyTxNode),PNULL);
#endif
                              
                            /*coverity-5.3.0 CID-10754*/
                            if(PNULL == ulStrategyTxNode_p)
                            {
                                /*CA Changes start  */
                                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                        ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                        &(crNode_p->ulGrantRequestNodeAnchor));
                                /*CA Changes end  */

                                if (!retValue)
                                {
                                    lteWarning("Error in Pushing "
                                            "ulGrantPendingQueue in"
                                            " funtion %s\n",__func__);
                                    if(crNode_p)
                                    {   
                                        FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(crNode_p);        
                                    }
                                }
                                contentionResolutionQueueCount--;
                                LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                                        "[%s]:UE ID %d is not schedule due to busy HARQ"\
                                        " process Global Tick:%d\n",
                                        __func__,tempUeId,currentGlobalTTITickCount);
                                continue;
                            }
                            ulStrategyTxNode_p->requiredRB = 0;
                            ulStrategyTxNode_p->maxDataSize = 0;
                            ulStrategyTxNode_p->minDataSize = 0;
                            /*coverity-5.3.0 CID-10754*/
                            tempMaxUEsToBeScheduledUL--;
                            /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                            tempUplinkContext_p->isAlreadyConsideredFlag 
                                = ulReferenceFlag_g[internalCellIndex] ;
#elif TDD_CONFIG
                            tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum] 
                                = ulReferenceFlag_g[internalCellIndex] ;
#endif
                            /* TDD Config 0 Changes End */
                            ulStrategyTxNode_p->ueIndex = (UInt16)tempUeId;
                            /* ULA_CHG */
                            ulStrategyTxNode_p->requiredRB = minRb; 
                            /* ICIC changes start */
                            numberOfAvailableRBs_p[userLocationType] -= minRb;

                            if (userLocationType == CE_USER)
                            {
                                /*CA Changes start  */
                                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                                /*CA Changes end  */
                                numberOfAvailableRBs_p[CC_CE_REGION] -= minRb;
                            }

                            else if(numberOfAvailableRBs_p[CC_CE_REGION] < 
                                    numberOfAvailableRBs_p[CE_REGION])
                            {
                                numberOfAvailableRBs_p[CE_REGION] = 
                                    numberOfAvailableRBs_p[CC_CE_REGION];
                            }
                            /* ICIC changes end */

                            /* ULA_CHG */
                            ulStrategyTxNode_p->strictAllocRB = TRUE;
                            ulStrategyTxNode_p->ulGrantRequestType =
                                CR_ULGRANT_REQ_MSG;
                            ulStrategyTxNode_p->ttiCounter = expiryTick;
#ifdef TDD_CONFIG                                                
                            ulStrategyTxNode_p->ulHarqProcessId =
                                ulHarqProcessId;
                            /*TDD Config 0 Changes Start*/
                            ulStrategyTxNode_p->scheduledCount = 0;
                            /*TDD Config 0 Changes End*/
#endif                                                              
                            /* ULA_CHG */
                            ulStrategyTxNode_p->minDataSize = 
                                /* SPR 5713 changes start */ 
                                tempUplinkContext_p->ulGrantMinDataSize;
                                /* SPR 5713 changes end */ 
                            ulStrategyTxNode_p->maxDataSize = 
                                determineUlTBSize(avgMcs,minRb) ;
                            ulStrategyTxNode_p->avgMcs = avgMcs;
                            /* ULA_CHG */

                            /*putEntry of ULStrategyTXNode in PDCCH Queue */
                            if (!(putEntryInULSchedulerNewTxQueue
                                        /*CA Changes start  */
                                        (ulStrategyTxNode_p, internalCellIndex
                                        /*CA Changes end  */
                                         /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                                         ,currentGlobalTTITickCount
                                         ,ulSubFrameNum
                                         ,tempUplinkContext_p
#endif
                                         /* TDD Config 0 Changes End */
                                        )))
                            {
                                /* ULA_CHG */
                                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                        LOGERROR,MAC_UL_Strategy,\
                                        currentGlobalTTITickCount,\
                                        ulStrategyTxNode_p->ueIndex,\
                                        0,0,0,0,0.0,0.0,__func__,
                                        "ulStrategyTxNode_p entry failed");
                                /* ULA_CHG */
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                                        FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
                                        freeMemPool(ulStrategyTxNode_p);
#endif
                            }
                            /* ULA_CHG */
                            else
                            {
                                LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                                        LOGDEBUG,MAC_UL_Strategy,\
                                        currentGlobalTTITickCount,\
                                        ulStrategyTxNode_p->ueIndex,\
                                        ulStrategyTxNode_p->requiredRB,\
                                        ulStrategyTxNode_p->strictAllocRB,\
                                        ulStrategyTxNode_p->minDataSize,\
                                        ulStrategyTxNode_p->maxDataSize,\
                                        ulStrategyTxNode_p->avgMcs,\
                                        ulStrategyTxNode_p->ttiCounter,\
                                        __func__,"ulStrategyTxNode_p parameters");

                                /* +COVERITY 5.0 */
                                LOG_MAC_MSG (UL_CONTENTION_GRANT_QUEUE_SCHEDULED_LOG_ID, LOGDEBUG,
                                        MAC_UL_Strategy, getCurrentTick (), tempUplinkContext_p->ueIndex,
                                        ulHarqProcessId,
                                        ulStrategyTxNode_p->requiredRB,
                                        tempUplinkContext_p->bsrNetTotal,
                                        ulStrategyTxNode_p->ulGrantRequestType,
                                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
                                /* -COVERITY 5.0 */
                            }
                            /* ULA_CHG */
                            freeMemPool((void *)crNode_p);
                            crNode_p = PNULL;
                        } /* minRb is assigned */
                    }/*timer is not expired*/
                }/*Not Already considered, created a new UL TX node*/
                else
                {
                    /*UE is already considered so unable to process CR Node 
                      hence Pushing into ulGrantPendingQueue */

                    /*CA Changes start  */
                    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                            ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                            &(crNode_p->ulGrantRequestNodeAnchor));
                    /*CA Changes end  */

                    if (!retValue)
                    {
                        lteWarning("Error in Pushing CR in"
                                "ulGrantPendingQueue in"                  
                                " function %s\n",__func__);
                        FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(crNode_p);        
                    }
                    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
                            "[%s]:UE ID %d is already considered, hence NOT \
                            considering UE,Adding UE in UL Grant Pending Queue",
                            __func__,tempUeId);
                }
            } /* checking for ue context */        
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,"[UL-Strategy]"
                        "[%s]UE Context either deleted or is to be deleted.\n");                
                /*Freeing the CR Node*/
                freeMemPool((void *)crNode_p);
                crNode_p = PNULL;
            }
        } /* cr node */
        else
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,"[UL-Strategy][%s]"
                    "Contention Resolution Node not valid...\n",__func__);
            break;
        } /* cr node*/
        contentionResolutionQueueCount--;
    }//end While loop

    /* EICIC +*/
    /* Check if count is still left , then pop nodes and push them back to maintiain same UE priority for next cycle - For victim UEs*/
    /* EICIC Optimization +- */
    if (isEicicEnabled_g[internalCellIndex])
    {
	    if ((contentionResolutionQueueCount) && (INVALID_UE_TYPE != ueTypeToScheduleUL_g[internalCellIndex])  && (isTraverseList))
    {
	    while(contentionResolutionQueueCount--) 
	    {
	    
	          crNodeScan_p =  (ULGrantRequestQueueNode *)popNode 
	            (ulGrantContentionResQueue_gp[internalCellIndex]);
		        
		        if( PNULL != crNodeScan_p )
		        {
		                    retValue = pushNode(ulGrantContentionResQueue_gp[internalCellIndex], &crNodeScan_p->ulGrantRequestNodeAnchor);

		                    if (retValue)
		                    {
		                        lteWarning("Error in Pushing "
		                                "ulGrantContentionResQueue_gp in"
		                                " funtion %s\n",__func__);
		        			   freeMemPool(crNodeScan_p);
							   crNodeScan_p = PNULL;
		                    }
	            } 
	        }
    }
    }
    /* EICIC Optimization +- */
    /* EICIC -*/  


    *maxUEsToBeScheduledUL_p = tempMaxUEsToBeScheduledUL ;

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]",
            __func__);
}
/*Cyclomatic Complexity changes - starts here */
/***************************************************************************************
 * Function Name  : putEntryInULGrantPendingQueue
 * Inputs         : srNode_p - Pointer to ULGrantRequestQueueNode,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function puts entry in UL Grant pending Queue.
 ***************************************************************************************/
static  void putEntryInULGrantPendingQueue(ULGrantRequestQueueNode **srNode_p, InternalCellIndex internalCellIndex)
{
    UInt32 retValue = 0;
    ULGrantRequestQueueNode *tempSrNode_p = *srNode_p;

    /*CA Changes start  */
    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
            ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
            &(tempSrNode_p->ulGrantRequestNodeAnchor));
    /*CA Changes end  */
    if (!retValue)
    { 
        lteWarning("Error in Pushing SR in"
                "ulGrantPendingQueue in"
                " funtion %s\n",__func__);
        FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(tempSrNode_p);        
    }
    *srNode_p = PNULL;
}
/*Cyclomatic Complexity changes - ends here */

/**************************************************************************************
 * Function Name  : processSRULGrantQueue
 * Inputs         : globalSrQueueCount - Its the total Nodes present in globalSrQueue
 *                                       which needs to be processed,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  maxUEsToBeScheduledUL_p - pointer to the max UE to be
 *                                            scheduled in UL,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  ulDelay - The total amount of delay 
 *                            expected in the UpLink path,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Allocate RBs to Contention Resolution - It will allocate RBs
 *                  to CR Queue Nodes & pass the UL Strategy output to the PDCCH
 *                  module.
 * Returns        : None
 * Description    : This function will process on the Scheduling requests in
 *                  the global SR Queue. Prepare the response and allocation 
 *                  of UL resources for long BSR and send to ulStrategyTXQueue_g. 
 *************************************************************************************/
void processSRULGrantQueue(
        UInt32 globalSrQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
    /* +- SPR 18490 */
    ULGrantRequestQueueNode *srNode_p   = PNULL;
    /* SPR 15909 fix start */
    tickType_t tempPhyTti                   = 0;
    /* SPR 15909 fix end */
    UInt32 tempUeId                     = 0;
    ULStrategyTxNode *ulStrategyTxNode_p= PNULL;
    ULUEContext * tempUplinkContext_p   = PNULL;
    MeasGapContextUL * measGapContext_p   = PNULL;
    ULUEContextInfo * tempUplinkContextInfo_p = PNULL;
    UInt8 tempMaxUEsToBeScheduledUL    = *maxUEsToBeScheduledUL_p;
    /* ULA_CHG */
    UInt8 avgMcs			            = 0;
    UInt8 minRb				            = 0;
    /* ULA_CHG */
    /* Rel9_upgrade_CR410 */
    UInt16      srPeriodicity = 0;
    /* Rel9_upgrade_CR410 */

    ULUEContext *ulUEContext_p  = PNULL;  
    ULUEContextInfo *ulUEContextInfo_p = PNULL;
#ifdef FDD_CONFIG 
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2 = TTI_MAP_FREE;
#endif           
    /* ICIC Change start */
    UInt8 userLocationType = CC_USER ;
    /* ICIC Change End */

   /* EICIC +*/
    ULGrantRequestQueueNode 	*srNodeScan_p   = PNULL;
    UInt8	isTraverseList = LTE_FALSE;
    /* EICIC -*/

    UInt8 retValue = 0;
    UInt32 checkSrQueueCountAndAvailableResources = 0;


    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Entry:[UL-Strategy][%s]",
            __func__);

    /*Processing for Queue Count and till the Available RBs*/
    /* ULA_CHG */
    /* ICIC changes start */
        checkSrQueueCountAndAvailableResources = ((globalSrQueueCount)&&(numberOfAvailableRBs_p[CC_CE_REGION]) 
                &&(tempMaxUEsToBeScheduledUL));
    while(checkSrQueueCountAndAvailableResources)
    {
        {
            checkSrQueueCountAndAvailableResources = ((globalSrQueueCount)&&(numberOfAvailableRBs_p[CC_CE_REGION]) 
                    &&(tempMaxUEsToBeScheduledUL));
        }
        if( !checkSrQueueCountAndAvailableResources )
        {
            break;
        }
        /*CA Changes start  */
        srNode_p =  (ULGrantRequestQueueNode *)popNode (ulGrantGlobalSrQueue_gp[internalCellIndex]);
        /*CA Changes end  */
        if (srNode_p)
        {
            tempUeId = srNode_p->ueIndex;
            tempPhyTti =  srNode_p->ttiCounter;
            tempUplinkContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
            ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

            /*Checking for UE context */
            if ( !(tempUplinkContextInfo_p->pendingDeleteFlag )
                    && ( PNULL != ulUEContext_p ) )
            {
                tempUplinkContext_p = tempUplinkContextInfo_p->ulUEContext_p;

		    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

		    /* EICIC +*/
		    /* EICIC Optimization +- */
		    if(isEicicEnabled_g[internalCellIndex])
		    {
		    /* Pushing UE back in trigger queue.*/   
			    /*Complexity Start*/ 	    
		    if (((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == ulUEContext_p->userType))
				||((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == ulUEContext_p->userType)))
		    {              
			    retValue = pushNode(ulGrantGlobalSrQueue_gp[internalCellIndex],&(srNode_p->ulGrantRequestNodeAnchor));

			    if (retValue)
			    {
				    lteWarning("Error in Pushing SR in"
						    "processSRULGrantQueue in"
						    " funtion %s\n",__func__);
				    freeMemPool((void *)srNode_p);
				    srNode_p = PNULL;      
			    }
			    isTraverseList = LTE_TRUE;
			    globalSrQueueCount--;
			    continue;   
		    }
		    }


/*Complexity End*/
		    /* EICIC - */

                measGapContext_p = &tempUplinkContext_p->measGapContext;
                if(PNULL !=  tempUplinkContext_p)
                {
                    /* + SPR_17858_465_CHANGES */
                    measGapContext_p = &tempUplinkContext_p->measGapContext;
                    /* - SPR_17858_465_CHANGES */
                    /*SR starvation fix start */
                    tempUplinkContext_p->srNode_p = PNULL;
                    /*SR starvation fix end */
                    /* ICIC Change start */
                    userLocationType = tempUplinkContext_p->userLocationType ;

                    /* If user is cell edge then we are checking if
                       maxCellEdgeUEsToBeScheduledUL_g is reached
                       or cell edge region RB is not available */

                    /*CA Changes start  */
                    if ((CE_USER == userLocationType && (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]
                    /*CA Changes end  */
                                    || !numberOfAvailableRBs_p[userLocationType]))
                            /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                            ||
                            ((TRUE == tempUplinkContext_p->isHDFddFlag) && 
                             (!CAN_SCHEDULE_UL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY, \
                                                         tempUeId , hdUeContextInfo_g.hdfddUeSchedMap)))
#endif
                            /*HD FDD Changes End*/
                       )
                    {
                        //  Pushing into ulGrantPendingQueue 
                        /*CA Changes start  */
                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(srNode_p->ulGrantRequestNodeAnchor));
                        /*CA Changes end  */
  
                        if (!retValue)
                        {
                            lteWarning("Error in Pushing SR in"
                                    "ulGrantPendingQueue in"
                                    " funtion %s\n",__func__);
                            FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(srNode_p);        
                        }
                        globalSrQueueCount--;
                        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                        if(TRUE == tempUplinkContext_p->isHDFddFlag)
                        {
                            /* Increment the statistics counter UE denied UL resources due to
                             * DL clash in the same TTI of HD FDD UE*/
                            UPDATE_UE_DENIED_UL_RESOURCE(tempUeId);
                        }
#endif
                        /* SR Drop Log Start */
                        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                LOGWARNING,MAC_UL_Strategy,\
                                currentGlobalTTITickCount,\
                                tempUeId,\
                                __LINE__,0,0,0,0.0,0.0,__func__,
                                "SrDrop_maxCellEdge");
                        /* SR Drop Log End */
                        /*HD FDD Changes End*/
                        continue;   
                    }
                    /* ICIC Change End */
                    /* \code
                     * if schType == SPS_SCHEDULING, then do not schedule this UE.
                     * */
                    if( SPS_SCHEDULING == tempUplinkContext_p->schType )
                    {
                        /*CA Changes start  */
                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(srNode_p->ulGrantRequestNodeAnchor));
                        /*CA Changes end  */
  
                        if (!retValue)
                        {
                            FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(srNode_p);        
                        }
                        globalSrQueueCount--;
                        /* SR Drop Log Start */
                        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                LOGWARNING,MAC_UL_Strategy,\
                                currentGlobalTTITickCount,\
                                tempUeId,\
                                __LINE__,0,0,0,0.0,0.0,__func__,
                                "SrDrop_spsScheduling");
                        /* SR Drop Log End */
                        continue;   
                    }
                    /* +- SPR 18490 */
#ifdef TDD_CONFIG                
    /* SPR 23393 Changes Start */
                    UInt32 sysFrameNum = 0;
                    UInt32 subframe     = 0;
                    GET_SFN_SF_FROM_TTI((currentGlobalTTITickCount + ulDelay), sysFrameNum, subframe)
    /* SPR 23393 Changes End */
                    LTE_GCC_UNUSED_PARAM(sysFrameNum)
                    /* +- SPR 18490 */
                    /* TDD Config 0 and 6 Changes Start */
                    UInt8 ulHarqProcessId = BUSY_ALL_HARQ;
                    /* +- SPR 17777 */
                        /* Get the free ID in ulHarqProcessId  */
                        /* SPR 11257 Changes Start*/
                        ulHarqProcessId = getFreeULHarqId(tempUplinkContext_p,subframe,internalCellIndex,
                                ulSubFrameNum,currentGlobalTTITickCount);
                    /* +- SPR 17777 */
                    /* SPR 11257 Changes End*/
                   /* TDD Config 0 and 6 Changes End*/
#elif FDD_CONFIG
                    /* TTIB_Code Start */
                    UInt8 ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + 
                            ulDelay + FDD_HARQ_OFFSET);
                    if ( tempUplinkContext_p->ttiBundlingEnabled )
                    {
                        ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
                    }
                    ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount +  ulDelay + FDD_HARQ_OFFSET);
                    ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount +  ulDelay + FDD_HARQ_OFFSET
                    + MAX_HARQ_PROCESS_NB);
                    /* TTIB_Code End */
#endif           
#ifdef TDD_CONFIG                     
                    if ( BUSY_ALL_HARQ == ulHarqProcessId)
#elif FDD_CONFIG
                    /* TTIB_Code Start */
                        if((HARQ_PROCESS_FREE != tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                                ||(tempUplinkContext_p->ttiBundlingEnabled &&
                                    (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,tempUplinkContext_p)))
                                ||(!(tempUplinkContext_p->ttiBundlingEnabled)&&
                                    ((TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId1]) ||
                                     ( TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId2])))
                                /*SPR 7944 START*/
                                /* + SPS_TDD_Changes*/
                                /*
                                   || checkSpsOccasionTti(tempUplinkContext_p,ulHarqProcessId)
                                   ||checkSpsActTti(tempUplinkContext_p,ulHarqProcessId)
                                 */
                                /* - SPS_TDD_Changes*/
                                /*SPR 7944 END*/
                          )
                    /* TTIB_Code End */
#endif                         
                        {
#ifdef FDD_CONFIG                        
                            if(HARQ_PROCESS_RESET == tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                            {
                                freeMemPool((void *)srNode_p);
                                srNode_p = PNULL;                             
                            }
                            else
#endif                            
                            {
                                /*CA Changes start Cov_fix_6May_63529_Start  */
                                if(!ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex],
                                    ULGrantRequestQueueNode,&(srNode_p->ulGrantRequestNodeAnchor)))
                                /*CA Changes end  */
                                {
                                    FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(srNode_p);
                                }
                                /*Cov_fix_6May_63529_Stop */
                            }
                            /* SR Drop Log Start */
                            /* +- SPR 18490 */
                            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                    LOGWARNING,MAC_UL_Strategy,\
                                    currentGlobalTTITickCount,\
                                    tempUeId,\
                                    __LINE__,0,0,0,0.0,0.0,__func__,
                                    "SrDrop_HarqBusy");
                            /* +- SPR 18490 */
                            /* SR Drop Log End */
                            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                                "[%s]:UE ID %d is not schedule due to busy HARQ"\
                                " process Global Tick:%d\n",
                                __func__,tempUeId,currentGlobalTTITickCount);
                         /* SPR_10907_fix Start */
                            globalSrQueueCount--;
                         /* SPR_10907_fix End */
                            continue;   
                        }

                    /*Checking if Already being considered for RB Distribution*/
                    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                    if (tempUplinkContext_p->isAlreadyConsideredFlag 
                        != ulReferenceFlag_g[internalCellIndex] )
#elif TDD_CONFIG
                    if (tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum] 
                        != ulReferenceFlag_g[internalCellIndex] )
#endif
                    /* TDD Config 0 Changes End */
                    {
                        LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                                "[UL-Strategy][%s]SR Request Received:SR Queue Cnt:%d \
                                Glbl TTI:%d,PhyTTi:%d,SR Periodicity:%d ,ulDelay %d \n"
                                ,__func__ ,globalSrQueueCount,
                                currentGlobalTTITickCount,tempPhyTti,
                                tempUplinkContext_p->pucchConfigInfo.srPeriodicity, 
                                ulDelay);

                        /*If the SR Periodicty Timer is expired,delete that SR Request*/
                        /* Rel9_upgrade_CR410 */
                        if (tempUplinkContext_p->pucchConfigInfo.srProhibitTimerValv920)
                        {
                            srPeriodicity = tempUplinkContext_p->
                                pucchConfigInfo.srProhibitTimerValv920;
                        }
                        else 
                        {
                            srPeriodicity = tempUplinkContext_p->
                                pucchConfigInfo.srPeriodicity;
                        }
                        /*Multi UE - Last UEs starvation fix Start */
/*Cyclomatic Complexity changes - starts here */
/* TDD SB_CQI */
                        /* 
                         * Do not schedule an SR when scheduling is being done through bsr 
                         * There can be a case in multi UE scenario: 
                         * for a particular UE, sr nodes will be accumulated at receiver, until
                         * scheduler gives it a grant for the first time. Until then its SRs will
                         * still be serviced when there is no need. This will cause starvation of the
                         * last UEs
                         */

#ifdef TDD_CONFIG
/* SPR_10089_FIX_START */
                        if (((tempUplinkContext_p->pucchConfigInfo.srPeriodicity
                                    > SHORT_SR_PERIODICITY) && 
                                ((currentGlobalTTITickCount - tempPhyTti)
									 >= srPeriodicity))
                               || (tempUplinkContext_p->bsrNetTotal))
#else
                        if (((tempUplinkContext_p->pucchConfigInfo.srPeriodicity
                                    > SHORT_SR_PERIODICITY) && 
                                ((currentGlobalTTITickCount - tempPhyTti 
                                  + ulDelay) >= srPeriodicity))
                               || (tempUplinkContext_p->bsrNetTotal))
/* TDD SB_CQI */
#endif
/* SPR_10089_FIX_END */
                        {
                            /* SR Drop Log Start */
                            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                    LOGWARNING,MAC_UL_Strategy,\
                                    currentGlobalTTITickCount,\
                                    tempUeId,\
                                    __LINE__,0,0,0,0.0,0.0,__func__,
                                    "SrDrop_srPeriod");
                            /* SR Drop Log End */
                            LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                                    "[UL-Strategy][%s]SR_Periodicty expired deleting SR\n"
                                    ,__func__);
                            /*Freeing the SR Node*/
                            freeMemPool((void *)srNode_p);
                            srNode_p = PNULL;                    
                        }
/*Cyclomatic Complexity changes - ends here */

                        /*Multi UE - Last UEs starvation fix End*/

                        /*If SR Periodicity timer is not Expired Then Send SR Response*/
                        else
                        {
                            /* UL_SYNC_LOSS_ON CHG */
                            /* MEAS_GAP_CHG */
                            if ((TRUE == measGapContext_p->isMeasGapPeriodOn) ||
                                    (UL_SYNC_LOSS_ON ==
                                     ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState))
                            {
                                /* SR Drop Log Start */
                                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                        LOGWARNING,MAC_UL_Strategy,\
                                        currentGlobalTTITickCount,\
                                        tempUeId,\
                                        __LINE__,measGapContext_p->isMeasGapPeriodOn,\
                                        ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState,0,0.0,0.0,__func__,
                                        "SrDrop_MeasGapOnSyncLossOn");
                                /* SR Drop Log End */

                                /* SPR 2377 changes start */
                                LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_UL_LOG_ID, 
                                        LOGDEBUG, MAC_UL_Strategy, 
                                        currentGlobalTTITickCount, 
                                        tempUeId, 
                                        tempUplinkContext_p->drxCurrentState,
                                        measGapContext_p->isMeasGapPeriodOn,
                                        ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_FLOAT_VALUE, 
                                        DEFAULT_FLOAT_VALUE,
                                        __func__,"");

                                /* SPR 2377 changes start */
                                /*CA Changes start  */
                                /*coverity id 63529*/
                                if(!ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                        ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                        &(srNode_p->ulGrantRequestNodeAnchor)))
                                {
                                    FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(srNode_p);
                                }
                                /*CA Changes end  */
                                globalSrQueueCount--;
                                continue;   
                            }
                            /* MEAS_GAP_CHG */
                            /* UL_SYNC_LOSS_ON CHG */

                            /* ULA_CHG */
                            avgMcs = tempUplinkContext_p->mcsIndex;

                            /* SPR 5713 changes start */ 
                            if (avgMcs >=  MIN_GRANT_MCS_INDEX )
                            {
                                /* ICIC changes start */
                                if (numberOfAvailableRBs_p[userLocationType] < 
                                       MAC_MIN_UL_GRANT_FOR_SR )
                                /* ICIC changes end */
                                {
                                    /*Available number of RBs is less, unable to
                                      process SR Node hence Pushing into 
                                      ulGrantPendingQueue */
                                    /*CA Changes start  */
                                    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                               ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                               &(srNode_p->ulGrantRequestNodeAnchor));
                                    /*CA Changes end  */
                                    if (!retValue)
                                    {
                                        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                                LOGWARNING,MAC_UL_Strategy,\
                                                currentGlobalTTITickCount,\
                                                /* SPR_2446 start */
                                                srNode_p->ueIndex,\
                                                /* SPR_2446 end */
                                                0,0,0,0,0.0,0.0,__func__,
                                                "Error in Pushing SR node in\
                                                ulGrantPendingQueue");
                                        FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(srNode_p);        
                                    }
                                    minRb = 0;
                                }
                                else
                                {
                                    minRb = MAC_MIN_UL_GRANT_FOR_SR;
                                }
                            }
                            else
                            {
                                CALCULATE_MAC_MIN_UL_GRANT_RBs(avgMcs,minRb,tempUplinkContext_p->ueComplianceRelease);
                                /* ICIC changes start */
                                if(numberOfAvailableRBs_p[userLocationType] < minRb)
                                /* ICIC changes end */
                                {
                                    /*Available number of RBs is less, unable to 
                                      process SR Node hence Pushing into 
                                      ulGrantPendingQueue */
                                    /*CA Changes start  */
                                    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                               ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                               &(srNode_p->ulGrantRequestNodeAnchor));
                                    /*CA Changes end  */
                                    if (!retValue)
                                    {
                                        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                                LOGWARNING,MAC_UL_Strategy,\
                                                currentGlobalTTITickCount,\
                                                /* SPR_2446 start */
                                                srNode_p->ueIndex,\
                                                /* SPR_2446 end */
                                                0,0,0,0,0.0,0.0,__func__,
                                                "Error in Pushing SR node in\
                                                ulGrantPendingQueue");
                                        FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(srNode_p);        
                                    }
                                    minRb = 0;
                                }
                            }
                            /* SPR 5713 changes end */ 

                            if(minRb) 
                            {
#ifdef FDD_CONFIG
                                if( tempUplinkContext_p->ttiBundlingEnabled )
                                {
                                    if ( avgMcs > MAX_TTI_BUNDLE_MCS )
                                    {
                                        avgMcs = MAX_TTI_BUNDLE_MCS;
                                    }
                                    /*CA Changes start  */
                                    if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < minRb ) 
                                          ||(MAC_FAILURE == processMeasGapValidation(
                                             tempUplinkContext_p,currentGlobalTTITickCount,ulDelay )))
                                    {
                                        /* SR Drop Log Start */
                                        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                                LOGWARNING,MAC_UL_Strategy,\
                                                currentGlobalTTITickCount,\
                                                tempUeId,\
                                                __LINE__,minRb,0,0,0.0,0.0,__func__,
                                                "SrDrop_measGap");
                                        /* SR Drop Log End */
                                        if (!(ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q( ulGrantPendingQueue_gp[internalCellIndex], 
                                                    ULGrantRequestQueueNode,&(srNode_p->ulGrantRequestNodeAnchor))))
                                        {
                                            freeMemPool(srNode_p);
                                            srNode_p = PNULL;
                                        }
                                        globalSrQueueCount--;
                                        continue;   

                                    }
                                    availableRBForTtiBundlingPerTick_g[internalCellIndex] -= minRb;
                                    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                                            "[%s][%d]:TTIB UE ID %d scheduled RB:%d MCS:%d \n",
                                            __func__,getCurrentTick(),tempUeId,minRb,avgMcs);
                                    /*CA Changes end  */
                                }
#endif
                                /* ULA_CHG */
                                LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                                        "[UL-Strategy][%s]SR_Periodicty is NOT Expired,hence"
                                        "Processing SR , CurGlbl Tick %d,\n" ,__func__,
                                        currentGlobalTTITickCount);


                                /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                                GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
                                GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                                        sizeof(ULStrategyTxNode),PNULL);
#endif
                                /*coverity-530 CID-10757*/
                                if(PNULL == ulStrategyTxNode_p)
                                {
                                    /* SR Drop Log Start */
                                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                            LOGWARNING,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            tempUeId,\
                                            __LINE__,0,0,0,0.0,0.0,__func__,
                                            "SrDrop_ulStrNode_null");
                                    /* SR Drop Log End */
                                    freeMemPool((void *)srNode_p);
                                    srNode_p = PNULL;
                                    globalSrQueueCount--;
                                    continue;    
                                }
                                ulStrategyTxNode_p->requiredRB = 0;
                                ulStrategyTxNode_p->maxDataSize = 0;
                                ulStrategyTxNode_p->minDataSize = 0;

                                /*coverity-530 CID-10757*/
                                tempMaxUEsToBeScheduledUL--;
                                /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                                tempUplinkContext_p->isAlreadyConsideredFlag 
                                    = ulReferenceFlag_g[internalCellIndex] ;
#elif TDD_CONFIG
                                tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum] 
                                    = ulReferenceFlag_g[internalCellIndex] ;
#endif
                                /* TDD Config 0 Changes End */
                                ulStrategyTxNode_p->ueIndex = (UInt16)tempUeId;
                                /* ULA_CHG */
                                ulStrategyTxNode_p->requiredRB = minRb;

                                /* ICIC changes start */
                                numberOfAvailableRBs_p[userLocationType] -= minRb;

                                if (userLocationType == CE_USER)
                                {
                                    /*CA Changes start  */
                                    maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                                    /*CA Changes end  */
                                    numberOfAvailableRBs_p[CC_CE_REGION] -= minRb;
                                }
                                else
                                {
                                    if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                                            numberOfAvailableRBs_p[CE_REGION])
                                    {
                                        numberOfAvailableRBs_p[CE_REGION] = 
                                            numberOfAvailableRBs_p[CC_CE_REGION];
                                    }
                                }
                                /* ICIC changes end */

                                /* ULA_CHG */
                                ulStrategyTxNode_p->strictAllocRB = TRUE;
                                ulStrategyTxNode_p->ulGrantRequestType =
                                    SR_ULGRANT_REQ_MSG;
                                ulStrategyTxNode_p->ttiCounter = tempPhyTti;
/* logging changes - 13Jan*/
#ifdef TDD_CONFIG                            
                                ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessId;
                                /*TDD Config 0 Changes Start*/
                                ulStrategyTxNode_p->scheduledCount = 0;
                                /*TDD Config 0 Changes End*/
#endif                            
                                /* ULA_CHG */
                                ulStrategyTxNode_p->minDataSize = 
                                    /* SPR 5713 changes start */ 
                                    tempUplinkContext_p->ulGrantMinDataSize;
                                    /* SPR 5713 changes end */ 
                                ulStrategyTxNode_p->maxDataSize = 
                                    determineUlTBSize(avgMcs,minRb);
                                ulStrategyTxNode_p->avgMcs = avgMcs;
                                /* ULA_CHG */

                                /*putEntry of ULStrategyTXNode in PDCCH Queue */
                                if (!(putEntryInULSchedulerNewTxQueue
                                            /*CA Changes start  */
                                            (ulStrategyTxNode_p, internalCellIndex
                                            /*CA Changes end  */
                                             /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                                             ,currentGlobalTTITickCount
                                             ,ulSubFrameNum
                                             ,tempUplinkContext_p
#endif
                                             /* TDD Config 0 Changes End */
                                            )))
                                {
                                    /* ULA_CHG */
                                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                            LOGERROR,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            ulStrategyTxNode_p->ueIndex,\
                                            0,0,0,0,0.0,0.0,__func__,
                                            "ulStrategyTxNode_p entry failed");
                                    /* ULA_CHG */
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                                        FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
                                        freeMemPool(ulStrategyTxNode_p);
#endif

                                }
                                /* ULA_CHG */
                                else
                                {
                                    LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                                            LOGDEBUG,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            ulStrategyTxNode_p->ueIndex,\
                                            ulStrategyTxNode_p->requiredRB,\
                                            ulStrategyTxNode_p->strictAllocRB,\
                                            ulStrategyTxNode_p->minDataSize,\
                                            ulStrategyTxNode_p->maxDataSize,\
                                            ulStrategyTxNode_p->avgMcs,\
                                            ulStrategyTxNode_p->ttiCounter,\
                                            __func__,"ulStrategyTxNode_p parameters");

                                    /* +COVERITY 5.0 */
                                    LOG_MAC_MSG (UL_SR_GRANT_QUEUE_SCHEDULED_LOG_ID, LOGDEBUG,
                                            MAC_UL_Strategy, currentGlobalTTITickCount, tempUplinkContext_p->ueIndex,
                                            ulHarqProcessId,
                                            ulStrategyTxNode_p->requiredRB,
                                            tempUplinkContext_p->bsrNetTotal,
                                            ulStrategyTxNode_p->ulGrantRequestType,
                                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
                                    /* -COVERITY 5.0 */
                                }
                                /* ULA_CHG */
                                freeMemPool((void *)srNode_p);
                                srNode_p = PNULL;
                            }
                        }
                    }
                    else
                    {
                        /* SR Drop Log Start */
                        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                LOGWARNING,MAC_UL_Strategy,\
                                currentGlobalTTITickCount,\
                                tempUeId,\
                                __LINE__,0,0,0,0.0,0.0,__func__,
                                "SrDrop_alReadyConsidered");
                        /* SR Drop Log End */
                        /*UE is already considered so unable to process SR Node 
                          hence Pushing into ulGrantPendingQueue */

                        /*Cyclomatic Complexity changes - starts here */
                        putEntryInULGrantPendingQueue(&srNode_p,internalCellIndex);
                        /*Cyclomatic Complexity changes - ends here */

                        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
                                "[%s]:UE is already considered, hence NOT \
                                considering UE,Adding UE %d in UL Grant Pending Queue",
                                __func__,tempUeId);

                    }
                }
                else
                {
                    /* SR Drop Log Start */
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            tempUeId,\
                            __LINE__,0,0,0,0.0,0.0,__func__,
                            "SrDrop_ueCtxt_null");
                    /* SR Drop Log End */
                    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[%s] UEID %d is NOT CREATED\n",__func__,tempUeId);                
                    LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,"[UL-Strategy][%s]"
                            "UE ID %d's Context either deleted or is to be deleted.\n",
                            __func__,tempUeId);                
                    /*Freeing the SR Node*/
                    freeMemPool((void *)srNode_p);
                    srNode_p = PNULL;
                }
            }        
            else
            {
            /* SR Drop Log Start */
            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                    LOGWARNING,MAC_UL_Strategy,\
                    currentGlobalTTITickCount,\
                    tempUeId,\
                    __LINE__,0,0,0,0.0,0.0,__func__,
                    "SrDrop_UeDeleted");
            /* SR Drop Log End */
                LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,"[UL-Strategy][%s]"
                        "UE ID %d's Context either deleted or is to be deleted.\n",
                        __func__,tempUeId);                
                /*Freeing the SR Node*/
                freeMemPool((void *)srNode_p);
                srNode_p = PNULL;
            }
        }
        else
        {
            /* SR Drop Log Start */
            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                    LOGWARNING,MAC_UL_Strategy,\
                    currentGlobalTTITickCount,\
                    0,\
                    __LINE__,0,0,0,0.0,0.0,__func__,
                    "SrDrop_srNode_null");
            /* SR Drop Log End */
            LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                    "[UL-Strategy][%s] srNode not valid...\n",__func__);
            break;
        }
        globalSrQueueCount--;
    }//end While loop

     /* EICIC +*/
    /* Check if count is still left , then pop nodes and push them back to maintiain same UE priority for next cycle */
    /* EICIC Optimization +- */
    if(isEicicEnabled_g[internalCellIndex])
    {
	    if ((globalSrQueueCount) && (INVALID_UE_TYPE != ueTypeToScheduleUL_g[internalCellIndex]) && (isTraverseList))
    {
	    while(globalSrQueueCount--) 
	    {    
     		    srNodeScan_p =  (ULGrantRequestQueueNode *)popNode(ulGrantGlobalSrQueue_gp[internalCellIndex]);
		        
		        if( PNULL != srNodeScan_p )
		        {
	                        if(pushNode(ulGrantGlobalSrQueue_gp[internalCellIndex],&(srNodeScan_p->ulGrantRequestNodeAnchor)))
	                        {
	                            lteWarning("Error in Pushing SR in"
	                                    "processSRULGrantQueue in"
	                                    " funtion %s\n",__func__);
	                            freeMemPool((void *)srNodeScan_p);
                				srNodeScan_p = PNULL;   
	                        }
	            } 
	        }
    }
    }

    /* EICIC -*/
    /* EICIC Optimization +- */

    *maxUEsToBeScheduledUL_p = tempMaxUEsToBeScheduledUL ;
    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]",
            __func__);
}

/*Cyclomatic Complexity changes - starts here */
/**********************************************************************************
 * Function Name  : processUeBSRInCQIULGrantQueue
 * Inputs         : tempUplinkContext_p - Pointer to ULUEContext, 
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  minRBsforAperiodicCQIReports - pointer to the min RBs for
 *                                                 aperiodic CQI reports,
 *                  cqiNode_p - pointer to ULGrantRequestQueueNode,
 *                  tempMinDataSize - Minimum data size,
 *                  ulHarqProcessId - UL HARQ Process ID,
 *                  currentGlobalTTITickCount - Current Global Tick Count,
 *                  ulDelay - The total amount of delay 
 *                            expected in the UpLink path,
 *                  ulSubFrameNum - uplink subframe number
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will be allocating RBs to the UE whose contention 
 *                  resolution was pending and who already had CRNTI. 
 *                  So Allocating RBs to those UEs to complete the contention 
 *                  resolution.
 *********************************************************************************/
STATIC  MacRetType processUeBSRInCQIULGrantQueue(
        ULUEContext * tempUplinkContext_p,
        UInt32 *numberOfAvailableRBs_p,
        UInt8 minRBsforAperiodicCQIReports,
        ULGrantRequestQueueNode *cqiNode_p,
        UInt32 tempMinDataSize
#ifdef TDD_CONFIG
        ,UInt8 ulHarqProcessId
#endif
        /* SPR 15909 fix start */
        ,tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
        ,UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    UInt32 usedRB                       = 0;
    UInt32 minDataSize			        = 0;
    UInt32 maxDataSize			        = 0;
    UInt32 retValue = 0;
    UInt8 mcsVal = 0;
    UInt8 avgMcs			            = 0;
    UInt8 minRb = 0;
    ULStrategyTxNode *ulStrategyTxNode_p= PNULL;
    /*If the UE has zero BSR available*/
    if ((!tempUplinkContext_p->bsrNetTotal)||(tempUplinkContext_p->bsrNetTotal &&
                (( 
                  /* ICIC changes start */
                  numberOfAvailableRBs_p[tempUplinkContext_p->userLocationType] 
                  /* ICIC changes end */
                  <
                  MAC_UL_GRANT_FOR_AP_CQI_WITH_BSR_LOAD ))))
    {
        /* ULA_CHG */
        // SPR 3486  - START
        /*CA Changes start  */
        minRb = (MAC_MIN_UL_GRANT > MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex)) ? \
                MAC_MIN_UL_GRANT:MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex);
        /*CA Changes end  */
        /* + CQI_4.1 */
        if(minRb > minRBsforAperiodicCQIReports)
        {
            minRBsforAperiodicCQIReports = minRb;
        }   
        // SPR 3486  - END
        if(numberOfAvailableRBs_p[tempUplinkContext_p->userLocationType] < minRBsforAperiodicCQIReports)
            /* - CQI_4.1 */
        {
            /*UE is already considered so unable to 
              process CQI Node hence Pushing into 
              ulGrantPendingQueue */
            /*CA Changes start  */
            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                    ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                    &(cqiNode_p->ulGrantRequestNodeAnchor));
            /*CA Changes end  */
            if (!retValue)
            {
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGWARNING,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        /* SPR_2446 start */
                        cqiNode_p->ueIndex,\
                        /* SPR_2446 end */
                        0,0,0,0,0.0,0.0,__func__,
                        "Error in Pushing CQI node in\
                        ulGrantPendingQueue");
                FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
            }
            /*Coverity 73498 Changes Start*/
            /*minRb = 0;*/
            /*Coverity 73498 Changes End*/
            return MAC_FAILURE;
        }
        /* ULA_CHG */
        /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
        GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
        GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                sizeof(ULStrategyTxNode),PNULL);
#endif
        /*coverity 530 CID 32738 */
        if(PNULL == ulStrategyTxNode_p)
        {
            return MAC_FAILURE;   
        }
        ulStrategyTxNode_p->requiredRB = 0;
        ulStrategyTxNode_p->maxDataSize = 0;
        ulStrategyTxNode_p->minDataSize = 0;
        /*coverity 530 CID 32738 */
        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum] 
            = ulReferenceFlag_g[internalCellIndex] ;
#endif
        /* TDD Config 0 Changes End */
        ulStrategyTxNode_p->ueIndex = (UInt16)cqiNode_p->ueIndex;

        /* + CQI_4.1 */
        /* ULA_CHG */
        ulStrategyTxNode_p->requiredRB = 
            minRBsforAperiodicCQIReports;
        /* ICIC changes start */
        numberOfAvailableRBs_p[tempUplinkContext_p->userLocationType] -= 
            minRBsforAperiodicCQIReports;
        ulStrategyTxNode_p->maxDataSize =
            ulStrategyTxNode_p->minDataSize =
            tempMinDataSize;
        /* - CQI_4.1 */

        if (tempUplinkContext_p->userLocationType == CE_USER)
        {
            /*CA Changes start  */
            maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
            /*CA Changes end  */
            numberOfAvailableRBs_p[CC_CE_REGION] -= 
                minRBsforAperiodicCQIReports;
        }

        else if(numberOfAvailableRBs_p[CC_CE_REGION] < 
                numberOfAvailableRBs_p[CE_REGION])
        {
            numberOfAvailableRBs_p[CE_REGION] = 
                numberOfAvailableRBs_p[CC_CE_REGION];
        }
        /* ICIC changes end */

        /* ULA_CHG */
        ulStrategyTxNode_p->strictAllocRB = TRUE;
        ulStrategyTxNode_p->ulGrantRequestType =
            CQI_WITHOUT_DATA_ULGRANT_REQ_MSG;
        ulStrategyTxNode_p->ttiCounter = 
            cqiNode_p->ttiCounter;
        /* ULA_CHG */
        /* Check why UL HARQ is missing (for TDD) */
        /* + CQI_5.0 */
        ulStrategyTxNode_p->avgMcs = MCS_INDEX_CQI_WITHOUT_DATA;
        /* - CQI_5.0 */
        /* ULA_CHG */
        /*SPR 8007 Fix Start*/
#ifdef TDD_CONFIG
        ulStrategyTxNode_p->ulHarqProcessId =
            ulHarqProcessId;
        /*TDD Config 0 Changes Start*/
        ulStrategyTxNode_p->scheduledCount = 0;
        /*TDD Config 0 Changes End*/
#endif
        /*SPR 8007 Fix End*/
    }
    /*else if the UE has non zero BSR*/
    else
    {
        // SPR 3486  - START
        /*CA Changes start  */
        minRb = (MAC_UL_GRANT_FOR_AP_CQI_WITH_BSR_LOAD > MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex))?\
                MAC_UL_GRANT_FOR_AP_CQI_WITH_BSR_LOAD:MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex);
        /*CA Changes end  */
        // SPR 3486  - END
        UInt32 strictAllocRBFlag = FALSE;
        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag
            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum] 
            = ulReferenceFlag_g[internalCellIndex] ;
#endif
        /* TDD Config 0 Changes End */
        usedRB = validateUplinkRB
            ( tempUplinkContext_p,
              // SPR 3486  - START
              minRb,
              // SPR 3486  - END
              numberOfAvailableRBs_p[tempUplinkContext_p->userLocationType],
              &strictAllocRBFlag
              /* ULA_CHG */
              ,&minDataSize,&maxDataSize,
              /* ULA_CHG */
              /* ATB Changes Start */
              /* SPR 13532 Fix Start */
              &mcsVal,
              /*SPR 16417 Changes Start*/
#ifdef FDD_CONFIG
              (currentGlobalTTITickCount + ulDelay),
              /* SPR 13532 Fix End */
#elif TDD_CONFIG
              currentGlobalTTITickCount,
              ulSubFrameNum,
#endif
              /*SPR 16417 Changes End*/
              /* ATB Changes End */
              internalCellIndex
            );
        if (usedRB )
        {
#ifdef FDD_CONFIG
            if( tempUplinkContext_p->ttiBundlingEnabled )
            {
                if ( usedRB > MAX_TTI_BUNDLE_RB )
                {
                    usedRB = MAX_TTI_BUNDLE_RB;
                }
                if ( mcsVal > MAX_TTI_BUNDLE_MCS )
                {
                    mcsVal = MAX_TTI_BUNDLE_MCS;
                }
                maxDataSize = determineUlTBSize(mcsVal,usedRB);
                minDataSize = determineUlTBSize(mcsVal,MIN_TTI_BUNDLE_RB);
                /*CA Changes start  */
                if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < minRb ) 
                        ||(MAC_FAILURE == processMeasGapValidation(
                                tempUplinkContext_p,currentGlobalTTITickCount,ulDelay )))
                {
                    retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q
                        (ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                         &(cqiNode_p->ulGrantRequestNodeAnchor));
                    if (!retValue)
                    {
                        FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                    }
                    return MAC_FAILURE;
                }
                availableRBForTtiBundlingPerTick_g[internalCellIndex] -= usedRB;
                LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                        "[%s][%d]:TTIB UE ID %d scheduled RB:%d MCS:%d \n",
                        __func__,getCurrentTick(),tempUeId,usedRB,mcsVal);
                /*CA Changes end  */
            }
#endif
            /* ULA_CHG */
            /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
            GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
            GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                    sizeof(ULStrategyTxNode),PNULL);
#endif
            /*coverity 530 CID 32738 */
            if(PNULL == ulStrategyTxNode_p)
            {
                return MAC_FAILURE;   
            }
            /*coverity 530 CID 32738 */
            ulStrategyTxNode_p->requiredRB = 0;
            ulStrategyTxNode_p->maxDataSize = 0;
            ulStrategyTxNode_p->minDataSize = 0;

            ulStrategyTxNode_p->ueIndex = (UInt16)cqiNode_p->ueIndex;
            /* ULA_CHG */
            ulStrategyTxNode_p->requiredRB = usedRB;
            /* ULA_CHG */
            ulStrategyTxNode_p->strictAllocRB = 
                strictAllocRBFlag;
            /* ICIC changes start */

            /* ATB Changes Start */
            avgMcs = mcsVal;
            /* ATB Changes End */

            numberOfAvailableRBs_p[tempUplinkContext_p->userLocationType] -= usedRB;

            if (tempUplinkContext_p->userLocationType == CE_USER)
            {
                /*CA Changes start  */
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                /*CA Changes end  */
                numberOfAvailableRBs_p[CC_CE_REGION] -= usedRB;
            }

            else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                    numberOfAvailableRBs_p[CE_REGION])
            {
                numberOfAvailableRBs_p[CE_REGION] = 
                    numberOfAvailableRBs_p[CC_CE_REGION];
            }
            /* ICIC changes end */

            /* ULA_CHG */
            ulStrategyTxNode_p->ulGrantRequestType =
                CQI_WITH_DATA_ULGRANT_REQ_MSG;
            ulStrategyTxNode_p->ttiCounter = 
                currentGlobalTTITickCount + ulDelay; //TDD SB_CQI// 
#ifdef TDD_CONFIG
            ulStrategyTxNode_p->ulHarqProcessId =
                ulHarqProcessId;
            /*TDD Config 0 Changes Start*/
            ulStrategyTxNode_p->scheduledCount = 0;
            /*TDD Config 0 Changes End*/
#endif
            /* ULA_CHG */
            ulStrategyTxNode_p->minDataSize = 
                minDataSize;
            ulStrategyTxNode_p->maxDataSize = 
                maxDataSize;
            /* + CQI_5.0 */
            tempUplinkContext_p->aperiodicCQIToBeRequested
                = TRUE;
            /* - CQI_5.0 */
            ulStrategyTxNode_p->avgMcs = avgMcs;
            /* ULA_CHG */
        }
        else
        {
            /*CA Changes start  */
            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                    ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                    &(cqiNode_p->ulGrantRequestNodeAnchor));
            /*CA Changes end  */
            if (!retValue)
            {
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGERROR,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        /* SPR_2446 start */
                        cqiNode_p->ueIndex,\
                        /* SPR_2446 end */
                        0,0,0,0,0.0,0.0,__func__,
                        "Error in Pushing CQI node in\
                        ulGrantPendingQueue");
                FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
            }

            return MAC_FAILURE;
        }
    }
    /*putEntry of ULStrategyTXNode in PDCCH Queue */
    /*CA Changes start  */
    if (!(putEntryInULSchedulerNewTxQueue(ulStrategyTxNode_p, internalCellIndex
    /*CA Changes end  */
                    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                    ,currentGlobalTTITickCount
                    ,ulSubFrameNum
                    ,tempUplinkContext_p
#endif
                    /* TDD Config 0 Changes End */
                    )))
    {
        /* ULA_CHG */
        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                LOGERROR,MAC_UL_Strategy,\
                currentGlobalTTITickCount,\
                ulStrategyTxNode_p->ueIndex,0,0,0,0,0.0,0.0,
                __func__,"ulStrategyTxNode_p entry failed");
        /* ULA_CHG */
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
        FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
        freeMemPool(ulStrategyTxNode_p);
#endif
    }
    /* ULA_CHG */
    else
    {
        LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                LOGDEBUG,MAC_UL_Strategy,\
                currentGlobalTTITickCount,\
                ulStrategyTxNode_p->ueIndex,\
                ulStrategyTxNode_p->requiredRB,\
                ulStrategyTxNode_p->strictAllocRB,\
                ulStrategyTxNode_p->minDataSize,\
                ulStrategyTxNode_p->maxDataSize,\
                ulStrategyTxNode_p->avgMcs,\
                ulStrategyTxNode_p->ttiCounter,\
                __func__,"ulStrategyTxNode_p parameters");
    }
    /* ULA_CHG */
    FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
    return MAC_SUCCESS;

}


/********************************************************************************
 * Function Name  : processCQIULGrantQueue
 * Inputs         : cqiQueueCount -  Its the total Nodes present in cqiQueue,
 *                                   which needs to be processed,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  maxUEsToBeScheduledUL_p - pointer to the max UE to be
 *                                            scheduled in UL,
 *                  currentGlobalTTITickCount - Current Global Tick Count,
 *                  ulSubFrameNum - ulSubFrameNumber for which grant is being
 *                  given
 * Outputs        : Process UE and allocate RBs for CQI Reports
 * Returns        : None 
 * Description    : This function will process the global CQI queue and 
 *                  allocating RBs to UEs for Aperiodic CQI Reports.  
 ********************************************************************************/
/*SPR 2446 Fix Begins*/
void processCQIULGrantQueue(
        UInt32 cqiQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
        ,UInt32 ulDelay
        ,InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
/*SPR 2446 Fix Ends*/

{
    /* +- SPR 17777 */
    /*coverity 96992 +-*/
    ULGrantRequestQueueNode *cqiNode_p  = PNULL;
    UInt32 tempUeId                     = 0;
    ULStrategyTxNode *ulStrategyTxNode_p= PNULL;
    ULUEContext * tempUplinkContext_p   = PNULL;
    ULUEContextInfo * tempUplinkContextInfo_p = PNULL;
    MeasGapContextUL * measGapContext_p   = PNULL;
    UInt8 tempMaxUEsToBeScheduledUL    = *maxUEsToBeScheduledUL_p;
    ULUeScheduleInfo *ulUeScheduleInfo_p = PNULL;
    /* + CQI_4.1 */
    UInt8 minRBsforAperiodicCQIReports = 0;
    UInt32 tempMinDataSize = 0;
    UInt32 scellTempMinDataSize = 0; 
    UInt8 scellMinRBsforAperiodicCQIReports = 0; 
    /* - CQI_4.1 */
    /* ICIC Change start */
    UInt8 userLocationType = CC_USER ;
    /* ICIC Change End */

    /* EICIC +*/
    ULGrantRequestQueueNode *cqiNodeScan_p  = PNULL;
    UInt8	isTraverseList = LTE_FALSE;
    /* EICIC -*/

    /* ULA_CHG */
    ULUEContext *ulUEContext_p  = PNULL;  
    ULUEContextInfo *ulUEContextInfo_p = PNULL;

#ifdef FDD_CONFIG /* SPR# 9916 fix */
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2 = TTI_MAP_FREE;
#endif
    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Entry:[UL-Strategy][%s]",
            __func__);
    UInt8 retValue = MAC_SUCCESS;
    UInt8 isCQIRequestedForPcell = 0; 
    UInt8 isCQIRequestedForScell = 0; 

    /*Processing for Queue Count and till the Available RBs*/
    /* ULA_CHG */
    /* ICIC changes start */
    while ((cqiQueueCount)&&(numberOfAvailableRBs_p[CC_CE_REGION]) 
            /*&& (tempMaxUEsToBeScheduledUL)*/)
        /* Note- As part of CA Phase2 CSI changes, this check of
         * tempMaxUEsToBeScheduledUL is removed because if multiple entries of
         * same ue are present in queue, one for Pcell and other for Scell, then
         * we have to club the two nodes into one strategy node for CA QOS
         * scheduling*/
        /* ICIC changes end */
        /* ULA_CHG */
    {
        DEQUEUE_UL_GRANT_APERIODIC_CQI_Q(ULGrantRequestQueueNode,(void *)&cqiNode_p,
                internalCellIndex);
        if (cqiNode_p)
        {
            tempUeId = cqiNode_p->ueIndex;                
            tempUplinkContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
            /*DRX_CHG*/
            tempUplinkContext_p = (tempUplinkContextInfo_p->ulUEContext_p);
            measGapContext_p = &tempUplinkContext_p->measGapContext;
            ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            /*SPR 3146 start */
            /*Checking for UE context */
            if (!(tempUplinkContextInfo_p->pendingDeleteFlag))
            {

		    /* EICIC +*/
		    /* EICIC Optimization +- */
		    if(isEicicEnabled_g[internalCellIndex])
		    {
		    /* Pushing UE back in trigger queue.*/
			    /*Complexity Start*/
		    if (((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == ulUEContext_p->userType))
				||((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == ulUEContext_p->userType)))
		    {              
			    retValue =  ENQUEUE_UL_GRANT_APERIODIC_CQI_Q(ULGrantRequestQueueNode,(void *)&cqiNode_p->ulGrantRequestNodeAnchor, internalCellIndex);
			    if (!retValue)
			    {
				    lteWarning("Error in Pushing "
						    "ULGrantRequestQueueNode in"
						    " funtion %s\n",__func__);
				    FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);        
			    }
			    isTraverseList = LTE_TRUE;
			    cqiQueueCount--;
			    continue;
		    }
			    /*Complexity End*/
		    }
		    /* EICIC Optimization +- */
		    /* EICIC - */  

                /* ICIC Change start */
                userLocationType = tempUplinkContext_p->userLocationType ;
                /*CA Changes start  */
                /* Phase2_CSI_Strategy_Changes start */
                if(tempMaxUEsToBeScheduledUL)
                {
                    /* Phase2_CSI_Strategy_Changes end */
                    if ((CE_USER == userLocationType && (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]
                                    /*CA Changes end  */
                                    || !numberOfAvailableRBs_p[userLocationType]))
                            /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                            /* If HD FDD UE is in  CQI UL grant  Q,then check Schedule map whether
                             * it can be scheduled in currenttick or not*/
                            ||
                            ((TRUE == tempUplinkContext_p->isHDFddFlag) && 
                             (!CAN_SCHEDULE_UL_FOR_HD_UE (currentGlobalTTITickCount + PHY_DL_DELAY, \
                                                          tempUeId, hdUeContextInfo_g.hdfddUeSchedMap)))
#endif
                            /*HD FDD Changes End*/
                       )
                    {
                        /*push the node in pnding queue*/
                        /*CA Changes start  */
                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(cqiNode_p->ulGrantRequestNodeAnchor));
                        /*CA Changes end  */
                        if (!retValue)
                        {
                            lteWarning("Error in Pushing "
                                    "ulGrantPendingQueue in"
                                    " funtion %s\n",__func__);
                            FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                        }
                        cqiQueueCount--;
                        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                        if(TRUE == tempUplinkContext_p->isHDFddFlag)
                        {
                            /* Increment the statistics counter UE denied UL resources due to
                             * DL clash in the same TTI of HD FDD UE*/
                            UPDATE_UE_DENIED_UL_RESOURCE(tempUeId);
                        }
#endif
                        /*HD FDD Changes End*/
                        continue;
                    }
                }
                /* ICIC Change End */
                /* \code
                 * if schType == SPS_SCHEDULING, then do not schedule this UE.
                 * */
                /* Phase2_CSI_Strategy_Changes start */
                if(tempMaxUEsToBeScheduledUL)
                {
                    /* Phase2_CSI_Strategy_Changes end */
                    if( SPS_SCHEDULING == tempUplinkContext_p->schType )
                    {
                        /*CA Changes start  */
                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(cqiNode_p->ulGrantRequestNodeAnchor));
                        /*CA Changes end  */
                        if (!retValue)
                        {
                            FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                        }
                        cqiQueueCount--;
                        continue;   
                    }

                    if((DRX_STATE_ON == tempUplinkContext_p->drxCurrentState)||
                            (TRUE == measGapContext_p->isMeasGapPeriodOn) ||
                            (UL_SYNC_LOSS_ON ==
                             ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState))

                    {
                        /* SPR 2377 changes start */
                        LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_UL_LOG_ID, 
                                LOGDEBUG, MAC_UL_Strategy, 
                                currentGlobalTTITickCount, 
                                tempUeId, 
                                tempUplinkContext_p->drxCurrentState,
                                measGapContext_p->isMeasGapPeriodOn,
                                ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE, 
                                DEFAULT_FLOAT_VALUE,
                                __func__,"");

                        /* SPR 2377 changes start */

                        /*push the node in pnding queue*/
                        /*CA Changes start  */
                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(cqiNode_p->ulGrantRequestNodeAnchor));
                        /*CA Changes end  */
                        if (!retValue)
                        {
                            lteWarning("Error in Pushing "
                                    "ulGrantPendingQueue in"
                                    " funtion %s\n",__func__);
                            FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                        }
                        cqiQueueCount--;
                        continue;
                    }
                }
#ifdef TDD_CONFIG
                UInt8 ulHarqProcessId = BUSY_ALL_HARQ ;
                /* TDD SB_CQI*/
                /* Get the free ID in ulHarqProcessId
                   if all the harq id is busy the skip this node and goto the next node in
                   the list and start this procedure again.
                   */
                /* TDD Config o and 6 Changes Start */
                /*coverity 96992 +*/
                /* SPR 23393 Changes Start */
                UInt8 subframe     = (currentGlobalTTITickCount + ulDelay) % MAX_SUB_FRAME;
                /* SPR 23393 Changes End */
                /*coverity 96992 -*/
                /* +- SPR 17777 */
                /* Get the free ID in ulHarqProcessId
                   if all the harq id is busy then skip this node and goto the next node in
                   the list and start this procedure again.
                   */
                    /* SPR 11257 Changes Start*/
    	        ulHarqProcessId = getFreeULHarqId(tempUplinkContextInfo_p->ulUEContext_p,
				            subframe,internalCellIndex,ulSubFrameNum,currentGlobalTTITickCount);
                /* +- SPR 17777 */
                /* SPR 11257 Changes End*/
                /* TDD Config 0 and 6 Changes End*/
#elif FDD_CONFIG
                UInt8 ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
                if ( tempUplinkContext_p->ttiBundlingEnabled )
                {
                    ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
                }
                ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
                ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET +
                        MAX_HARQ_PROCESS_NB);
#endif
                /* Phase2_CSI_Strategy_Changes start */
                if(tempMaxUEsToBeScheduledUL)
                {
                    /* Phase2_CSI_Strategy_Changes end */
#ifdef TDD_CONFIG            
                    if ( BUSY_ALL_HARQ == ulHarqProcessId)
#elif FDD_CONFIG
                        if((HARQ_PROCESS_FREE != tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                                ||(tempUplinkContext_p->ttiBundlingEnabled &&
                                    (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,tempUplinkContext_p)))
                                ||(!(tempUplinkContext_p->ttiBundlingEnabled)&&
                                    ((TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId1]) ||
                                     ( TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId2])))
                                /*SPR 7944 START*/
                                /* + SPS_TDD_Changes */
                                /*
                                   ||checkSpsOccasionTti(tempUplinkContext_p,ulHarqProcessId)
                                   ||checkSpsActTti(tempUplinkContext_p,ulHarqProcessId)
                                 */
                                /* - SPS_TDD_Changes */
                                /*SPR 7944 END*/

                          )
#endif                
                        {
#ifdef FDD_CONFIG                
                            if(HARQ_PROCESS_RESET == tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                            {
                                FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                            }
                            else 
#endif
                                /* SPR 8228 changes start */
                            {
                                /*CA Changes start  */
                                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                        ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                        &(cqiNode_p->ulGrantRequestNodeAnchor));
                                /*CA Changes end  */
                                if (!retValue)
                                {
                                    lteWarning("Error in Pushing "
                                            "ulGrantPendingQueue in"
                                            " function %s\n",__func__);
                                    FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                                }
                            }
                            cqiQueueCount--;
                            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                                    "[%s]:UE ID %d is not schedule due to busy HARQ"\
                                    " process Global Tick:%d\n",
                                    __func__,tempUeId,currentGlobalTTITickCount);
                            continue;
                            /* SPR 8228 changes end */
                        }
                }
                tempUplinkContext_p = tempUplinkContextInfo_p->ulUEContext_p;
                isCQIRequestedForPcell = (cqiNode_p->aperiodicCQIToBeRequested & 
                        servingCellBitMask_g[PCELL_SERV_CELL_IX]);
                isCQIRequestedForScell = (cqiNode_p->aperiodicCQIToBeRequested & 
                        servingCellBitMask_g[START_SCELL_INDEX]);
                if (isCQIRequestedForScell && 
                        (tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX] == NULL || 
                         !IS_SCELL_ACTIVE(tempUeId,START_SCELL_INDEX)))
                {
                    if(isCQIRequestedForPcell == 0)
                    {
                        FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
                                "[%s]:UE does not have scell context initialized or scell is not active, hence NOT \
                                considering UE %d ",
                                __func__,tempUeId);
                        cqiQueueCount--;
                        continue;
                    }
                    else
                    {
                        isCQIRequestedForScell = 0;
                    }
                } 

                /*Checking if Already being considered for RB Distribution*/
                /*CA Qos Ul Strategy changes start*/
                /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                if ( (tempUplinkContext_p->isAlreadyConsideredFlag !=
                            ulReferenceFlag_g[internalCellIndex])
                    &&
                        (tempUplinkContext_p->ulStrategyTxNode_p == PNULL))
#elif TDD_CONFIG
                if ( (tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum] != 
                        ulReferenceFlag_g[internalCellIndex])
                    &&
                        (tempUplinkContext_p->ulStrategyTxNode_p == PNULL))
#endif
                {
                    /*If the UE has zero BSR available*/
                    if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->ulSchdConfig.ulSchedulerStrategy == UL_QOS_SCHEDULING)
                    {
                        /*check minRb's Required for cell's configured */
                        if (isCQIRequestedForPcell && isCQIRequestedForScell)
                        {
                            minRBsforAperiodicCQIReports = 
                                determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                                        &tempMinDataSize, internalCellIndex);
                            scellMinRBsforAperiodicCQIReports = 
                                determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                                        &scellTempMinDataSize, tempUplinkContext_p->
                                        ulUeScellContext_p[START_SCELL_INDEX]->internalCellIndex);
                            tempMinDataSize += scellTempMinDataSize;
                            minRBsforAperiodicCQIReports += scellMinRBsforAperiodicCQIReports;
                            tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs = 
                                scellMinRBsforAperiodicCQIReports;
                            tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minDataSize = 
                                scellTempMinDataSize;  
                        }
                        else if(isCQIRequestedForScell)
                        {
                            minRBsforAperiodicCQIReports = 
                                determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                                        &tempMinDataSize, tempUplinkContext_p->
                                        ulUeScellContext_p[START_SCELL_INDEX]->internalCellIndex);
                            tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs = 
                                minRBsforAperiodicCQIReports;
                            tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minDataSize =
                                tempMinDataSize;  
                        }
                        else /*scenario for isCQIRequestedForPcell only*/
                        {
                            minRBsforAperiodicCQIReports = 
                                determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                                        &tempMinDataSize, internalCellIndex);
                        } 
                        /* ICIC changes start */
                        if ( numberOfAvailableRBs_p[userLocationType] >
                                minRBsforAperiodicCQIReports )
                            /* ICIC changes end */
                        {
                            /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                            GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
                            GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                                    sizeof(ULStrategyTxNode),PNULL);
#endif
                            /* Rel 5.3: Coverity 10503 Fix Start */
                            if (PNULL == ulStrategyTxNode_p)
                            {
                                FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                                cqiQueueCount--;
                                continue;
                            }
                            /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                            tempUplinkContext_p->isAlreadyConsideredFlag
                                = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
                            tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                                = ulReferenceFlag_g[internalCellIndex];
#endif
                            if(tempMaxUEsToBeScheduledUL != 0)
                            {
                                tempMaxUEsToBeScheduledUL --;
                            }
                            else
                            {
                                cqiQueueCount--;
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                                FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
                                freeMemPool(ulStrategyTxNode_p);
                                ulStrategyTxNode_p = PNULL;
#endif
                                retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                        ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                        &(cqiNode_p->ulGrantRequestNodeAnchor));
                                /*CA Changes end  */
                                if (!retValue)
                                {
                                    lteWarning("Error in Pushing CQI in"
                                            "ulGrantPendingQueue in"
                                            " funtion %s\n",__func__);
                                    FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                                }

                                continue;
                            }
                            if (userLocationType == CE_USER)
                            {
                                /*CA Changes start  */
                                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                                /*CA Changes end  */
                            }
                            /*coverity id 63554*/
                            ulStrategyTxNode_p->requiredRB = 0;
                            ulStrategyTxNode_p->maxDataSize = 0;
                            ulStrategyTxNode_p->minDataSize = 0;
                            /* Increament the scheduled UE count */
                        }
                        else
                        {
                            /*RBs are not Sufficient so unable to
                              process CQI Node hence Pushing CQI Node into
                              ulGrantPendingQueue */
                            /*CA Changes start  */
                            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                    ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                    &(cqiNode_p->ulGrantRequestNodeAnchor));
                            /*CA Changes end  */
                            if (!retValue)
                            {
                                lteWarning("Error in Pushing CQI in"
                                        "ulGrantPendingQueue in"
                                        " funtion %s\n",__func__);
                                FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                            }

                            cqiQueueCount--;
                            continue;
                        }
                    }
                    else
                    {
                        /* handling of  strategy other than QOS */
                        if(tempMaxUEsToBeScheduledUL == 0)
                        {
                            retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                    ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                    &(cqiNode_p->ulGrantRequestNodeAnchor));
                            /*CA Changes end  */
                            if (!retValue)
                            {
                                lteWarning("Error in Pushing CQI in"
                                        "ulGrantPendingQueue in"
                                        " funtion %s\n",__func__);
                                FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                            }
                            cqiQueueCount--;
                            continue;
                        }
                        /* + CQI_4.1 */
                        minRBsforAperiodicCQIReports = 
                            determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                                    &tempMinDataSize, internalCellIndex);
                        /* - CQI_4.1 */
                        /*Cyclomatic Complexity changes - starts here */
                        retValue = processUeBSRInCQIULGrantQueue(tempUplinkContext_p,
                                numberOfAvailableRBs_p,
                                minRBsforAperiodicCQIReports,
                                cqiNode_p,
                                tempMinDataSize
#ifdef TDD_CONFIG
                                ,ulHarqProcessId
#endif
                                ,currentGlobalTTITickCount
                                ,ulDelay
                                /*CA Changes start  */
                                ,internalCellIndex
                                /*CA Changes end  */
                                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                                ,ulSubFrameNum
#endif
                                /* TDD Config 0 Changes End */
                                );
                        /*Coverity_31972 Fix Start*/                              
                        cqiNode_p = PNULL;
                        /*Coverity_31972 Fix end*/                              
                        if(MAC_FAILURE == retValue)
                        {
                            cqiQueueCount--;
                            continue;
                        }
                        tempMaxUEsToBeScheduledUL--;
                        tempUplinkContext_p->aperiodicCQIToBeRequested = TRUE;
                        /*Cyclomatic Complexity changes - ends here */
                    }
                }
                else
                {
                    if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->ulSchdConfig.ulSchedulerStrategy != UL_QOS_SCHEDULING)
                    {
                        /*UE is already considered so unable to process CQI Node 
                          hence Pushing into ulGrantPendingQueue */
                        /*CA Changes start  */
                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(cqiNode_p->ulGrantRequestNodeAnchor));
                        /*CA Changes end  */
                        if (!retValue)
                        {
                            lteWarning("Error in Pushing in"      
                                    "ulGrantPendingQueue in"
                                    " funtion %s\n",__func__);
                            FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                        }
                        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
                                "[%s]:UE is already considered, hence NOT \
                                considering UE,Adding UE %d in UL Grant Pending Queue",
                                __func__,tempUeId);
                        cqiQueueCount--;
                        continue;
                    }
                    ulStrategyTxNode_p = tempUplinkContext_p->ulStrategyTxNode_p; 
                    if((PNULL == ulStrategyTxNode_p) ||
                            ((ulStrategyTxNode_p->ulGrantRequestType != CQI_WITH_DATA_ULGRANT_REQ_MSG)
                             &&(ulStrategyTxNode_p->ulGrantRequestType != CQI_WITHOUT_DATA_ULGRANT_REQ_MSG)))
                    {
                        /* ue is already considered for some other grant type so
                         * will not process its aperiodic cqi queue */
                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(cqiNode_p->ulGrantRequestNodeAnchor));
                        /*CA Changes end  */
                        if (!retValue)
                        {
                            lteWarning("Error in Pushing in"      
                                    "ulGrantPendingQueue in"
                                    " funtion %s\n",__func__);
                            FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                        }
                        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
                                "[%s]:UE is already considered, hence NOT \
                                considering UE,Adding UE %d in UL Grant Pending Queue",
                                __func__,tempUeId);
                        cqiQueueCount--;
                        continue;
                    }
                    /* update the existing strategy node */
                    if(isCQIRequestedForPcell && (tempUplinkContext_p->aperiodicCQIToBeRequested
                                & servingCellBitMask_g[PCELL_SERV_CELL_IX]))
                    {
                        isCQIRequestedForPcell = 0;
                    }
                    if(isCQIRequestedForScell && (tempUplinkContext_p->aperiodicCQIToBeRequested
                                & servingCellBitMask_g[START_SCELL_INDEX]))
                    {
                        isCQIRequestedForScell = 0;
                    }
                    if((isCQIRequestedForPcell == 0) &&
                            (isCQIRequestedForScell == 0))
                    {
                        /* node is already considered so free it from queue */
                        FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                        cqiQueueCount--;
                        continue;
                    }
                    if(isCQIRequestedForScell)
                    {
                        minRBsforAperiodicCQIReports = 
                            determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                                    &tempMinDataSize, tempUplinkContext_p->
                                    ulUeScellContext_p[START_SCELL_INDEX]->internalCellIndex);
                        tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minRBs = 
                            minRBsforAperiodicCQIReports;
                        tempUplinkContext_p->ulUeScellContext_p[START_SCELL_INDEX]->minDataSize =
                            tempMinDataSize;  
                    }
                    if(isCQIRequestedForPcell)
                    {
                        minRBsforAperiodicCQIReports = 
                            determineRBsForAperiodicCQIReports(tempUplinkContext_p,
                                    &tempMinDataSize, internalCellIndex);
                    }
                    if ( numberOfAvailableRBs_p[userLocationType] <=
                            minRBsforAperiodicCQIReports )
                    {
                        /*RBs are not Sufficient so unable to
                          process CQI Node hence Pushing CQI Node into
                          ulGrantPendingQueue */
                        /*CA Changes start  */
                        retValue = ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                                ulGrantPendingQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                                &(cqiNode_p->ulGrantRequestNodeAnchor));
                        /*CA Changes end  */
                        if (!retValue)
                        {
                            lteWarning("Error in Pushing CQI in"
                                    "ulGrantPendingQueue in"
                                    " funtion %s\n",__func__);
                            FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
                        }

                        cqiQueueCount--;
                        continue;

                    }
                }
                if(ulStrategyTxNode_p != PNULL &&
                        (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                         initParams_p->ulSchdConfig.ulSchedulerStrategy == UL_QOS_SCHEDULING))
                {
                    /* Rel 5.3: Coverity 10503 Fix Start */
#ifdef FDD_CONFIG
                    tempUplinkContext_p->isAlreadyConsideredFlag
#else
                    tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
#endif
                        = ulReferenceFlag_g[internalCellIndex];
                    ulStrategyTxNode_p->ueIndex = (UInt16)tempUeId;

                    /* + CQI_4.1 */
                    ulStrategyTxNode_p->requiredRB +=
                        minRBsforAperiodicCQIReports;
                    /* ICIC changes start */
                    numberOfAvailableRBs_p[userLocationType] -= 
                        minRBsforAperiodicCQIReports;
                    ulStrategyTxNode_p->maxDataSize += tempMinDataSize; 
                    ulStrategyTxNode_p->minDataSize += tempMinDataSize;

                    /* - CQI_4.1 */

                    if (userLocationType == CE_USER)
                    {
                        numberOfAvailableRBs_p[CC_CE_REGION] -= 
                            minRBsforAperiodicCQIReports;
                    }
                    else if(numberOfAvailableRBs_p[CC_CE_REGION] < 
                            numberOfAvailableRBs_p[CE_REGION])
                    {
                        numberOfAvailableRBs_p[CE_REGION] = 
                            numberOfAvailableRBs_p[CC_CE_REGION];
                    }
                    /* ICIC changes end */

                    ulStrategyTxNode_p->strictAllocRB = TRUE;
                    ulStrategyTxNode_p->ulGrantRequestType =
                        CQI_WITHOUT_DATA_ULGRANT_REQ_MSG;
                    ulStrategyTxNode_p->ttiCounter = currentGlobalTTITickCount + ulDelay; //TDD SB_CQI//

                    /* SPR 18918_18643_19032 Start */
                    tempUplinkContext_p->aperiodicCQIToBeRequested = TRUE;
                    /* SPR 18918_18643_19032 End */
                    /* ULA_CHG */
                    /* Check why UL HARQ is missing (for TDD) */ 
                    /* + CQI_5.0 */
                    ulStrategyTxNode_p->avgMcs = MCS_INDEX_CQI_WITHOUT_DATA; 
                    /* - CQI_5.0 */
                    /* ULA_CHG */
#ifdef TDD_CONFIG
                    ulStrategyTxNode_p->ulHarqProcessId =
                        ulHarqProcessId;
                    /*TDD Config 0 Changes Start*/
                    ulStrategyTxNode_p->scheduledCount = 0;
                    /*TDD Config 0 Changes End*/
#endif

                    /*putEntry of ULStrategyTXNode in UL Strategy Schedule Info */
                    /*CA Changes start  */
                    ulUeScheduleInfo_p = &(ulStrategyScheduleInfo_gp[internalCellIndex]->ulUeScheduleInfo
                            [ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount]);
                    /*CA Changes end  */

                    ulUeScheduleInfo_p->ulStrategyTxNode_p = ulStrategyTxNode_p;


                    ulUeScheduleInfo_p->ueULContext_p = 
                        tempUplinkContext_p;
                /*spr 11709 fix start*/
                    ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount++;
                /*spr 11709 fix end*/

                    tempUplinkContext_p->ulStrategyTxNode_p = ulStrategyTxNode_p;
                    if (isCQIRequestedForScell)
                    {
                        tempUplinkContext_p->aperiodicCQIToBeRequested = 
                            tempUplinkContext_p->aperiodicCQIToBeRequested | 
                            servingCellBitMask_g[START_SCELL_INDEX];
                    }
                    if (isCQIRequestedForPcell) 
                    {
                        tempUplinkContext_p->aperiodicCQIToBeRequested = 
                            tempUplinkContext_p->aperiodicCQIToBeRequested | 
                            servingCellBitMask_g[PCELL_SERV_CELL_IX];
                    }
                    FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);

                    /* CA Stats Changes Start */
                    /*debug stats*/
                    LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_CQI(
                            internalCellIndex);
                    /* CA Stats Changes End */
                }

                /* Rel 5.3: Coverity 10503 Fix Start */
                if (PNULL != ulStrategyTxNode_p)
                {
                    LOG_MAC_MSG (UL_CQI_GRANT_QUEUE_SCHEDULED_LOG_ID, LOGDEBUG,
                            MAC_UL_Strategy, getCurrentTick (), tempUplinkContext_p->ueIndex,
                            ulHarqProcessId,
                            ulStrategyTxNode_p->requiredRB,
                            tempUplinkContext_p->bsrNetTotal,
                            ulStrategyTxNode_p->ulGrantRequestType,
                            /*coverity 96992 +*/
                            DEFAULT_FLOAT_VALUE,
                            /*coverity 96992 -*/
                            DEFAULT_FLOAT_VALUE, __func__, " ");
                }
                /* Rel 5.3: Coverity 10503 Fix End */
            }        
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,"[UL-Strategy][%s]"
                        "UE Context either deleted or is to be deleted.\n",__func__);                
                /*Freeing the SR Node*/
                FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNode_p);
            }
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,"[UL-Strategy][%s]"
                    "CQI Node not valid...\n",__func__);
            break;
        }
        cqiQueueCount--;
    }//end While loop

    /* EICIC +*/
    /* EICIC Optimization +- */

    /* Check if count is still left , then pop nodes and push them back to maintiain same UE priority for next cycle */
    if(isEicicEnabled_g[internalCellIndex])
    {
	    if ((cqiQueueCount) && (INVALID_UE_TYPE != ueTypeToScheduleUL_g[internalCellIndex]) && (isTraverseList))
    {
	    while(cqiQueueCount--) 
	    {
      		DEQUEUE_UL_GRANT_APERIODIC_CQI_Q(ULGrantRequestQueueNode,(void *)&cqiNodeScan_p,internalCellIndex);
	        
	        if( PNULL != cqiNodeScan_p )
	        {
		                 retValue =  ENQUEUE_UL_GRANT_APERIODIC_CQI_Q(ULGrantRequestQueueNode,(void *)&cqiNodeScan_p->ulGrantRequestNodeAnchor, internalCellIndex);
	                         if (!retValue)
	                        {
	                            lteWarning("Error in Pushing "
	                                    "processCQIULGrantQueue in"
	                                    " funtion %s\n",__func__);
	                            FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(cqiNodeScan_p);        
	                        }
	                        
	        } 
	    }
    }
    }
    /* EICIC Optimization +- */

    /* EICIC -*/


    *maxUEsToBeScheduledUL_p = tempMaxUEsToBeScheduledUL ;
    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]",
            __func__);
}

/*****************************************************************************
 * Function Name  : scheduleUEsByULStrategy
 * Inputs         : numberOfAvailableRBs_p - pointer to current available RBs,
 *                  maxUEsToBeScheduledUL  - max UE to be scheduled in UL,
 *                  currentGlobalTTITickCount - Current Global Tick Count,
 *                  ulDelay - The total amount of delay 
 *                            expected in the UpLink path,
 *                  internalCellIndex - cell index used at MAC.
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : Schedule UEs according to Algorithm and update the Non Zero 
                    BSR List
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Its main function is to distributes RBs among the UEs 
 *                  present in the UENonZeroQueueLoadAndHArqList after applying
 *                  scheduling algorithm either Proportional Fair or Round Robin
 *                  algorithm depending upon the dlSchedulerStrategyType_g.It
 *                  Also Update the Non Zero Bsr List Accroding to the triggers 
 *                  received for adding or deleting.
 *****************************************************************************/
 UInt32 scheduleUEsByULStrategy(
        UInt32 *numberOfAvailableRBs_p,
        UInt8  maxUEsToBeScheduledUL,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
#ifdef FDD_CONFIG
        UInt8 ulDelay,
#endif
        /* CA Changes start */
        InternalCellIndex internalCellIndex
        /* +- SPR 17777 */
        /* CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]",
            __func__);
     /* CA Changes start */
     UInt8 ulSchedulerStrategy = cellSpecificParams_g.\
           cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
           ulSchdConfig.ulSchedulerStrategy;
      /* CA Changes end */
    /*Process the ZeroNonZeroBSRTriggerQueue_g trigger Queue and Update the 
      Non Zero BSR List,after adding or deleting UEs according to
      Trigger  received in ZeroNonZeroBSRTriggerQueue_g */

    /* LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy][%s]"
       ": Processing ZeroNonZeroBSRTriggerQueues:%d\n", __func__);*/

    /* To Process the ZeroNonZeroBSRTriggerQueue_g Queue and Update the
       NonZeroBsr List,After adding and deleting UEs according to Trigger*/

    if (ulSchedulerStrategy == UL_FAIR_SCHEDULING)
    {
        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[%s]:"
                "UL_FAIR_SCHEDULING ul Scheduler Strategy is Called \n"
                "Number of Total Avalibale RBs %d, Cell Edge Available RBs %s, Number of UE to be schedule:%d",
                __func__,numberOfAvailableRBs_p[CC_CE_REGION],numberOfAvailableRBs_p[CE_REGION],
                maxUEsToBeScheduledUL );
        /* SPR 2446 Fix Begins*/ 
        ulProportionalFairScheduler(
                ulReferenceFlag_g[internalCellIndex],
                numberOfAvailableRBs_p,
                maxUEsToBeScheduledUL,
                currentGlobalTTITickCount
#ifdef FDD_CONFIG
                ,ulDelay
#endif
                ,internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */

                );
        /* SPR 2446 Fix Ends*/
    }
    /* ROUND_ROBIN_SCHEDULING */
    else if (ulSchedulerStrategy == UL_ROUND_ROBIN_SCHEDULING)
    {
        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[%s]:"
                "ROUND_ROBIN_SCHEDULING ul Scheduler Strategy is Called\n"
                "Number of Total Avalibale RBs %d, Cell Edge Available RBs %s, Number of UE to be schedule:%d",
                __func__,numberOfAvailableRBs_p[CC_CE_REGION],numberOfAvailableRBs_p[CE_REGION],
                maxUEsToBeScheduledUL );

        /* SPR 2446 Fix begins*/ 
        ulRoundRobinScheduler(
                ulReferenceFlag_g[internalCellIndex],
                numberOfAvailableRBs_p,
                maxUEsToBeScheduledUL, 
                currentGlobalTTITickCount
#ifdef FDD_CONFIG
                ,ulDelay
#endif
                ,internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );

        /* SPR 2446 Fix Ends*/
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[%s]:"
                "QOS_SCHEDULING ul Scheduler Strategy is Called\n"
                "Number of Total Avalibale RBs %d, Cell Edge Available RBs %s, Number of UE to be schedule:%d",
                __func__,numberOfAvailableRBs_p[CC_CE_REGION],numberOfAvailableRBs_p[CE_REGION],
                maxUEsToBeScheduledUL );

        ulQosScheduler(
                numberOfAvailableRBs_p,
                currentGlobalTTITickCount,
                maxUEsToBeScheduledUL
#ifdef FDD_CONFIG
                ,ulDelay
#endif
                 /* CA Changes start */
                 , internalCellIndex
                /* CA Changes end */
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );
    }

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]"
            ,__func__);
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : processBSRZeroNonZeroTriggerQueue
 * Inputs         : currentGlobalTTITickCount,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : updates the Non Zero BSR List
 * Returns        : None
 * Description    : Its main function will be to Process the 
 *                  BSRZeroNonZeroTriggerQueue Queue and Update the 
 *                  NonZeroQueueLoadAndHarq List,after adding and deleting UEs 
 *                  according to Trigger.
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
void processBSRZeroNonZeroTriggerQueue(tickType_t currentGlobalTTITickCount,
/* SPR 15909 fix end */
                                      InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    bsrQueueNode * node_p       = PNULL;
    ULUEContext* ueULContext_p  = PNULL;  
    MeasGapContextUL* measGapContext_p  = PNULL;  
    UInt32 ueIndex;
    ULUEContextInfo * ueULContextInfo_p = PNULL;
    UInt32 zeroNonZeroBSRTriggerQueueCount = 0;
    UInt32 j                    = 0;

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Entry:[UL-Strategy][%s]",
            __func__);

    for ( j = NUM_OF_DEMUX_THREADS; j > 0; j--  )
    {
        /*CA Changes start  */
        zeroNonZeroBSRTriggerQueueCount =  sQueueCount
            (ZeroNonZeroBSRTriggerQueue_gp[internalCellIndex] + (j -1));
        /*CA Changes end  */

        /*Processed for the number of  Queue Count*/
        while (zeroNonZeroBSRTriggerQueueCount)
        {
            /*CA Changes start  */
            node_p = ( bsrQueueNode * )popNode(ZeroNonZeroBSRTriggerQueue_gp[internalCellIndex] + (j - 1));
            /*CA Changes end  */

            /*Checking the Node for NULL*/
            if (node_p)
            {
                ueIndex = node_p->ueIdx;
                ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
                /*Checking the UE Context & Checking for Pending 
                  delete Flag of UE*/
                if (!(ueULContextInfo_p->pendingDeleteFlag))
                {
                    ueULContext_p = ueULContextInfo_p->ulUEContext_p;
                    /*+ Dyn UEs To Be SchPerTTI +*/
                    if (ADD_TRIGGER == node_p->addDeleteTrigger)
                    {
                        if (!ueULContext_p->isUEConsideredForTotalActiveUeUL)
                        {
                            totalActiveUEsUL_g[internalCellIndex]++;
                            ueULContext_p->isUEConsideredForTotalActiveUeUL = TRUE;
                        }
                    }
                    /*- Dyn UEs To Be SchPerTTI -*/
                    measGapContext_p = &ueULContext_p->measGapContext;
                    /* DRX_CHG */
                    if ((DRX_STATE_ON != ueULContext_p->drxCurrentState) && 
                            (TRUE != measGapContext_p->isMeasGapPeriodOn) &&
                            (UL_SYNC_LOSS_OFF == 
                             ulUECtxInfoArr_g[ueIndex].ulUEContext_p->
                             ulSyncLossOrHOInfoInUL.ulSyncLossState))
                    {
                        /*Trigger Node have flag to add the UE in Non Zero List*/
                        if (ADD_TRIGGER == node_p->addDeleteTrigger )
                        {
                            /* Verifying whether the UE is already present 
                               in Non Zero List,if Already present, 
                               No Need to add UE in List*/
                            /* SPR 15652 Fix Start */
                         if (PNULL == ueULContext_p->nonZeroBsrListTempDeleted_p)
                          {
                            if (ueULContext_p->nonZeroBsrList_p)
                            {
                                LTE_MAC_UT_LOG(LOG_DETAIL,
                                        UL_STRATEGY,
                                        "[UL-Strategy]"
                                        "[%s]UE ID %d is Already in NonZeroBSRList",
                                        __func__,ueIndex);
                                    /* SR Drop Log Start */
                                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                            LOGWARNING,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            ueIndex,\
                                            node_p->addDeleteTrigger,__LINE__,\
                                            ueULContext_p->bsrNetTotal,ulUECtxInfoArr_g[ueIndex].ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState,\
                                            0.0,0.0,__func__,
                                            "AddDelTriggerF_nonZeroBsrList_p");
                                    /* SR Drop Log End */
                            }
                            else
                            {
                                /*CA Changes start  */
                                putEntryInNonZeroBSRList(ueIndex,ueULContext_p, internalCellIndex);
                                /*CA Changes end  */
                            }
                         }
                          /* SPR 15652 Fix End */
                            /* SR Drop Log Start */
                            else
                            {
                                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                        LOGWARNING,MAC_UL_Strategy,\
                                        currentGlobalTTITickCount,\
                                        ueIndex,\
                                        node_p->addDeleteTrigger,__LINE__,\
                                        ueULContext_p->bsrNetTotal,ulUECtxInfoArr_g[ueIndex].ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState,\
                                        0.0,0.0,__func__,
                                        "AddDelTriggeer_fail_nonZeroBsrListTempDeleted_p");
                            }
                            /* SR Drop Log End */

                        }
                        /*Trigger Node have flag is to delete the UE from
                          Non Zero List*/
                        else
                        {
                            /*Verifying whether the UE is present in
                              the Non Zero List,if UE is not Present in the Non
                              Zero List, then it cannot be deleted from the Non
                              Zero List*/

                            if (ueULContext_p->nonZeroBsrList_p)
                            { 
                                /*check whether the UE Queue Load is also Zero,
                                  if Not then UE cannot be deleted from the 
                                  non Zero List*/

                                if (ueULContext_p->bsrNetTotal)
                                {
                                    /* SR Drop Log Start */
                                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                            LOGWARNING,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            ueIndex,\
                                            node_p->addDeleteTrigger,__LINE__,\
                                            ueULContext_p->bsrNetTotal,ulUECtxInfoArr_g[ueIndex].ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState,\
                                            0.0,0.0,__func__,
                                            "AddDelTriggerF_bsrNetTotal");
                                    /* SR Drop Log End */
                                    LTE_MAC_UT_LOG(LOG_DETAIL,
                                            UL_STRATEGY,
                                            "[UL-Strategy]"
                                            "[%s]UE ID %d 's BSR net Load is"
                                            " still NON ZERO and num of active"
                                            " logical channels are non Zero" 
                                            "Hence UE Cannot be deleted "
                                            "from NonZeroBSRList"
                                            ,__func__,ueIndex);
                                }
                                else
                                {
                                    /*CA Changes start  */
                                    deleteEntryFromNonZeroBSRList(ueULContext_p,ueULContext_p->nonZeroBsrList_p, internalCellIndex);
                                    /*CA Changes end  */
                                }
                            }
                            else
                            {
                                /* SR Drop Log Start */
                                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                        LOGWARNING,MAC_UL_Strategy,\
                                        currentGlobalTTITickCount,\
                                        ueIndex,\
                                        node_p->addDeleteTrigger,__LINE__,\
                                        ueULContext_p->bsrNetTotal,ulUECtxInfoArr_g[ueIndex].ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState,\
                                        0.0,0.0,__func__,
                                        "AddDelTriggerF_nonZeroBsrList_p");
                                /* SR Drop Log End */
                                LTE_MAC_UT_LOG(LOG_DETAIL,
                                        UL_STRATEGY,
                                        "[UL-Strategy]"
                                        "[%s]UE ID %d is NOT Present in"
                                        " NonZeroBSRList Hence"
                                        "Cannot delete the Entry"
                                        " from Non Zero BSR List"
                                        ,__func__,ueIndex);
                            }
                        }
                    }/* DRX_CHG*/
                    /* SR Drop Log Start */
                    else
                    {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            ueIndex,\
                            node_p->addDeleteTrigger,ueULContext_p->drxCurrentState,\
                            measGapContext_p->isMeasGapPeriodOn,ulUECtxInfoArr_g[ueIndex].ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState,\
                            0.0,0.0,__func__,
                            "AddDelTriggeer_fail_state");
                    }
                    /* SR Drop Log End */
                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                            "[UL-Strategy][%s]UE ID %d Context is NULL"
                            " or Pending to delele\n",__func__,ueIndex);
                }
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                        "[UL-Strategy]"
                        "[%s]ZeroNonZeroBSRQueue returned NULL\n",
                        __func__); 
                break;
            }
            /*Free the memory for this node_p.*/
            freeMemPool((void *)node_p);
            node_p = PNULL;
            zeroNonZeroBSRTriggerQueueCount--;   
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]",
            __func__);
}



/*****************************************************************************
 * Function Name  : putEntryInNonZeroBSRList 
 * Inputs         : ueIndex - Index of UE whose is non zero,
 *                  ueULContext_p - Pointer to the UL UE Context,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Add UE in Non Zero List and update the pointer in UE Context
 * Returns        : None
 * Description    : It inserts the ueIndex provided as argument into 
 *                  nonZeroBsrList.
 *****************************************************************************/
 void putEntryInNonZeroBSRList(
        UInt32 ueIndex,
        ULUEContext* ueULContext_p,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex)
        /*CA Changes end  */
{
    nonZeroBsrListNode * node_p = PNULL;
    /* SPR 16916 FIX START */
    /* +- */
    /* SPR 16916 FIX END */

    GET_MEM_FROM_POOL(nonZeroBsrListNode,node_p,sizeof(nonZeroBsrListNode),PNULL);

    /* + coverity 24551 */
    if (PNULL != node_p)
    {
        node_p->ueIndex = (UInt16)ueIndex;
        /*Adding the UE in the ueNonZeroQueueLoadAndHarqList */
        /*CA Changes start  */
            /*Adding the UE in the dlUENonZeroQueueLoadAndHarqList */
        listInsertNode(nonZeroBsrList_gp[internalCellIndex],&(node_p->nodeAnchor));
        /*CA Changes end  */
        /*Storing the Pointer of the node in the ueNonZeroQueueLoadAndHarqList 
         into the DL UE Context */
        ueULContext_p->nonZeroBsrList_p = &(node_p->nodeAnchor);

        /*Setting the UE Priority Factor to Zero if its not Zero */
        if (ueULContext_p->ulSchedulerAlgoInfo.uePriorityFactor)
        {
            ueULContext_p->ulSchedulerAlgoInfo.uePriorityFactor = FALSE;
        }        
        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
                "[%s]Entry added for ueIndex %d in UE NON BSR ZERO LIST\n",
                __func__,ueIndex);
        /* SPR 16916 FIX START */
        /* +- */
        /* SPR 16916 FIX END */
    }
    /* - coverity 24551 */
}



/*****************************************************************************
 * Function Name  : deleteEntryFromNonZeroBSRList
 * Inputs         : tempNode_p - Pointer to LIST Node,
 *                  ueULContext_p - pointer to the UL UE context,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Delete the UE from the Non Zero BSR List
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : It deletes the ueIndex provided as argument from 
 *                  nonZeroBsrList.
 *****************************************************************************/
 MacRetType  deleteEntryFromNonZeroBSRList(        
        ULUEContext* ueULContext_p,
        LTE_LIST_NODE * tempNode_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
{
    ULSchedulerStrategyType ulSchedulerStrategy =
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
        initParams_p->ulSchdConfig.ulSchedulerStrategy ;
    MacRetType retType = MAC_FAILURE;
    LTE_LIST_NODE * tempNextNode_p = PNULL;
    /* SPR_8524_FIX_START */
    /* SPR 16916 FIX START */
    /* code removed */
    /* SPR 16916 FIX END */
    /* SPR_8524_FIX_END */

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
            "[%s]Entry \n", __func__);
    {
        if ( ( ulSchedulerStrategy == UL_ROUND_ROBIN_SCHEDULING)&&
                (tempNode_p == nonZeroBsrListStartRoundRobin_p[internalCellIndex]))
        {
            tempNextNode_p = getNextListNode(tempNode_p );
            if (!tempNextNode_p)
            {
                /*CA Changes start  */
                if (1 ==listCount(nonZeroBsrList_gp[internalCellIndex]))
                    /*CA Changes end  */
                {
                    nonZeroBsrListStartRoundRobin_p[internalCellIndex] = PNULL;
                }
                else
                {
                    nonZeroBsrListStartRoundRobin_p[internalCellIndex] = 
                        /*CA Changes start  */
                        getListNode(nonZeroBsrList_gp[internalCellIndex],0);
                    /*CA Changes end  */
                }
            }
            else
            {
                nonZeroBsrListStartRoundRobin_p[internalCellIndex] = tempNextNode_p ;
            }
            retType = MAC_SUCCESS;
            /* SPR 22275 Fix + */
            /*The Node in the LIST will be deleted */
            /*CA Changes start  */
            listDeleteNode(nonZeroBsrList_gp[internalCellIndex], tempNode_p);
            /*CA Changes end  */
            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
                    "[%s]UE's entry is Deleted for ueIndex %d from UE NON ZERO LIST\n",
                    __func__,ueULContext_p->ueIndex);

            /*Making the pointer of the Non Zero BSR list Node*/
            freeMemPool(tempNode_p);
            /* SPR 22275 Fix - */
        }
        /* SPR 12809 fix start */
        else if( UL_QOS_SCHEDULING == ulSchedulerStrategy )
        {
            UInt8 lcgId = 0;
            for(;lcgId < NUM_OF_LCG;lcgId++)
            {
                ulQosDeleteEntryFromNonZeroBSRList(ueULContext_p, lcgId, internalCellIndex);
            }
        }
    }
    /* SPR 12809 fix end */
    /* SPR 22275 Fix + */
    /* Code Removed */
    /* SPR 22275 Fix - */
    /*
    ** In case, if the UE is deleted at LP thread, Its necessary to
    ** check whether the ueContext is already NULL
    */
    if ( PNULL != ueULContext_p)
    {
        ueULContext_p->nonZeroBsrList_p = PNULL;
        /*+ Dyn UEs To Be SchPerTTI +*/
        if(ueULContext_p->isUEConsideredForTotalActiveUeUL)
        {
            totalActiveUEsUL_g[internalCellIndex]--;
            ueULContext_p->isUEConsideredForTotalActiveUeUL = FALSE;
        }
        /*- Dyn UEs To Be SchPerTTI -*/
        /* SPR 22275 Fix + */
        retType = MAC_SUCCESS;
    }
    /* ++coverity 56624*/
    else
    {
        retType = MAC_FAILURE;
        /* SPR 22275 Fix - */
    }
    /* --coverity 56624*/
/* SPR_8524_FIX_START */
    /* SPR 16916 FIX START */
    /* code removed */
    /* Now, when the bsrNet becomes zero for the lcg, the activeUECount in 
     * UL will be decreased for the qci while freeing the Harq,to take the
     * buffered data into consideration for which Harq transmission is ongoing
     * as per spec TS 36.314, section 4.1.3.2.
     */
    /* SPR 16916 FIX END */
 /* SPR_8524_FIX_END */

    return retType;

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
            "[%s]Exit \n", __func__);
}

/* ULA_CHG */
/*****************************************************************************
 * Function Name  : deleteEntryFromNonZeroBSRListTemporarily 
 * Inputs         : ueULContext_p - pointer to the UL UE context,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Delete the UE from the Non Zero BSR List
 * Returns        : None
 * Description    : It deletes the ueIndex provided as argument from 
 *                  nonZeroBsrList temporarily i.e does not free that memory
 *                  Reasons (DRX/ Meas Gap/ ULSynch loss).
 *****************************************************************************/
 /* CA changes start */
void deleteEntryFromNonZeroBSRListTemporarily(        
        ULUEContext* ueULContext_p,
        InternalCellIndex internalCellIndex)
{
    LTE_LIST_NODE * tempNode_p = ueULContext_p->nonZeroBsrList_p;
    LTE_LIST_NODE * tempNextNode_p = PNULL;

    if ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
        initParams_p->ulSchdConfig.ulSchedulerStrategy == UL_ROUND_ROBIN_SCHEDULING)&&
            (tempNode_p == nonZeroBsrListStartRoundRobin_p[internalCellIndex]))
    {
 /* CA changes end */
        tempNextNode_p = getNextListNode(tempNode_p );
        if (!tempNextNode_p)
        {
            /*CA Changes start  */
            if (1 ==listCount(nonZeroBsrList_gp[internalCellIndex]))
            /*CA Changes end  */
            {
                nonZeroBsrListStartRoundRobin_p[internalCellIndex] = PNULL;
            }
            else
            {
                nonZeroBsrListStartRoundRobin_p[internalCellIndex] = 
                    /*CA Changes start  */
                    getListNode(nonZeroBsrList_gp[internalCellIndex],0);
                    /*CA Changes end  */
            }
        }
        else
        {
            nonZeroBsrListStartRoundRobin_p[internalCellIndex] = tempNextNode_p ;
        }
    }

    /*The Node in the LIST will be deleted */
    /*CA Changes start  */
    listDeleteNode(nonZeroBsrList_gp[internalCellIndex], tempNode_p);
    /*CA Changes end  */

    return;
}

/****************************************************************************
 * Function Name  : validateUplinkRB
 * Inputs         : ueContext_p - UE who's RBs are to be determined,
 *                  allocatedRB - Number of RBs allocated initially,
 *                  numberOfAvailableRBs_p - Total RBs to be distributed,
 *                  strictAllocRBFlag -Flag for indicating Strict RB Allocation,
 *                  minDataSize,
 *                  maxDataSize,
 *                  mcsVal,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : validated the allocated RB against the UE's BSR Load and
 *                  assign the validated RB to UE
 * Returns        : allocatedRB - actual RBs allocated.
 * Description    : This functions checks whether a UE really requires the RBs
 *                  allocated to and if less number of RBs are required ,then
 *                  only that amount of RBs required are allocated.
 ****************************************************************************/
UInt32 validateUplinkRB(
        ULUEContext* ueULContext_p,
        UInt32 allocatedRB,
        UInt32  numberOfAvailableRBs,
        UInt32* strictAllocRBFlag,
        UInt32* minDataSize,
        UInt32* maxDataSize,
        /* ATB Changes Start */
        /* SPR 13532 Fix Start */
        UInt8 * mcsVal,
        /*SPR 16417 Changes Start*/
#ifdef FDD_CONFIG
        /* SPR 15909 fix start */
        tickType_t currentGlobalTickWithULDelay,
#elif TDD_CONFIG
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt8 ulSubFrameNum,
#endif
        /* SPR 13532 Fix End */
        /*SPR 16417 Changes End*/
/* ATB Changes End */
        InternalCellIndex internalCellIndex)

{
    /* SPR 19600 Changes start */
    UInt32 dataSize = 0;
    UInt8 mcsIndex = 0;
    UInt32 maxData = 0;
    UInt32 maxAllocatedRBsByATB = 0;
    UInt32 macHeaderInBytes = MAC_HEADER_SIZE_IN_BYTES(ZERO,ONE);
    UInt32 ueMaxPowerFlag = 0;
    UInt32 ueMinPowerFlag = 0;

    /* SPR 13532 Fix Start */
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    /* SPR 13532 Fix End */

    UInt32 totalQLoad = 0;
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
    SInt32 regionPowerMask = ulPowerMaskInfo_p->powerMask[ueULContext_p->userLocationType];
    UInt32 minRBAsPerUeRelease = 0;

    /* SPR 23259 Fix + */
    maxData = ulUeCategoryInfo_g[(ueULContext_p->ueCategory) - 1].maxBytesULSCHTb;
    /* SPR 23259 Fix - */
    *mcsVal = 0;
    /* SPR 19600 Changes end */
    mcsIndex = ueULContext_p->mcsIndex;

    /* If allocated Rbs is greater than availble , 
       then allocated Rbs should be equal to available Rbs */
    /* ICIC changes start */
    if(allocatedRB > numberOfAvailableRBs)
    {
        allocatedRB = numberOfAvailableRBs;
    }
    /* ICIC changes end */

    /* +DYNAMIC_ICIC */    
    CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ueULContext_p,regionPowerMask); 

    /* + SPR_17858_510_CHANGES */
    allocatedRB = getUplinkMaxRBsInPowerOfTwoThreeFive(allocatedRB);
    /* - SPR_17858_510_CHANGES */

    /* validate allocated Rb from ATB */
    /* Time Averaging changes start */
    /* ATB Changes Start */
    /*SPR 16417 Changes Start*/
#ifdef FDD_CONFIG
    /* SPR 13532 Fix Start */
    powerCorrectionAndTpc = getTpcForPusch(ueULContext_p,currentGlobalTickWithULDelay,internalCellIndex);
    /* SPR 13532 Fix End */
#elif TDD_CONFIG
    powerCorrectionAndTpc = getTpcForPusch(ueULContext_p,
                                    currentGlobalTick,
                                    internalCellIndex,
                                    ulSubFrameNum);
#endif
    /*SPR 16417 Changes End*/
    maxAllocatedRBsByATB = validateATB(allocatedRB,&mcsIndex,ueULContext_p,powerCorrectionAndTpc.powerCorrectionVal,
                                       &ueMaxPowerFlag,&ueMinPowerFlag,regionPowerMask,
                                       NEW_TX, internalCellIndex);
    allocatedRB = maxAllocatedRBsByATB;

    *mcsVal = mcsIndex;

    dataSize = determineUlTBSize(mcsIndex, allocatedRB);

    totalQLoad = ueULContext_p->bsrNetTotal + ueULContext_p->phrGrant ;
    /* validate dataSize with the maxData limit set by ueCategory*/
    if (dataSize >= maxData)
    {
        if(totalQLoad < maxData)
        {
            dataSize = ueULContext_p->bsrNetTotal + macHeaderInBytes;
            allocatedRB = DETERMINE_RBs_UL(mcsIndex,dataSize, internalCellIndex);
        }
        else
        {
            dataSize = maxData;
            allocatedRB = DETERMINE_RBs_UL(mcsIndex,maxData, internalCellIndex);
#ifdef PERF_STATS
            /*  number of ms where respective UE is limited in the
             *  UL direction by its UE capability */
            gMacUePerfStats_p[ueULContext_p->ueIndex].lteUeMacSchedULPerfStats.\
                totalTimeRestrictUeInUlbyUeCap++;
#endif
        }
        *minDataSize = dataSize;
        *maxDataSize = dataSize;
    }
    else
    {
        UInt32 tempDataSize = 0;
        SInt32 temp_BsrLoad = 0;
        /*This will store the net BSR now after subtracting the already allocated RBs
          from the net BSR Load*/
        temp_BsrLoad = totalQLoad;
        tempDataSize = dataSize - macHeaderInBytes;

        if(dataSize <= UL_TB_MINIMUM_SIZE_IN_BYTES)
        {
            *strictAllocRBFlag = TRUE;
            do
            {
                allocatedRB++;
                dataSize = determineUlTBSize(mcsIndex,allocatedRB);
            }while ((dataSize < UL_TB_MINIMUM_SIZE_IN_BYTES )&&
                    /* ICIC changes start */
                    (allocatedRB < numberOfAvailableRBs )&&
                    /* ICIC changes end */
                    (allocatedRB <= maxAllocatedRBsByATB)); 

            if (dataSize < UL_TB_MINIMUM_SIZE_IN_BYTES)
            {
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGERROR,MAC_UL_Strategy,\
                        getCurrentTick(),\
                        ueULContext_p->ueIndex,0,0,0,0,0.0,0.0,
                        __func__,"Either maxNbOfRBs reached or \
                        AvailableRB exhausted or Allocated RBs are still LESS \
                        than MINIMUM TB Size so NOT Allocating RBS");
                /* Coverity 72765 Fix Start*/
                allocatedRB =1;
                /* Coverity 72765 Fix End*/
                return allocatedRB;
            }
        }
        else
        {
            /* Now the Validation to match the UE BSR Load is done, by decreasing 
               the RBs allocated against the BSR Load */
            if ((SInt64)tempDataSize > temp_BsrLoad)
            {
                do {
                    // SPR 3486  - START
                    /*CA Changes start  */
                    if(allocatedRB <= MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex))
                    /*CA Changes end  */
                    {
                        break;
                    }
                    // SPR 3486  - END
                    /** SPR 2811 Changes Start **/

                    /** SPR 2811 Changes End **/
    				/* Coverity 73551 */
					if (allocatedRB > 0)
					{
                    	dataSize = determineUlTBSize(mcsIndex,allocatedRB);
					}
    				/* Coverity 73551 */
                    tempDataSize  = dataSize - macHeaderInBytes;
                    /*Coverity Fix 73551*/
                    allocatedRB--;

                } while ((dataSize > UL_TB_MINIMUM_SIZE_IN_BYTES) &&
                        ((SInt64)tempDataSize > temp_BsrLoad) &&
                        (allocatedRB));

                if (((SInt64)tempDataSize < temp_BsrLoad)||(!allocatedRB))
                {
                    allocatedRB++;
                    dataSize = determineUlTBSize(mcsIndex,allocatedRB);
                }
            }
        }

        temp_BsrLoad  = temp_BsrLoad + UL_TB_MINIMUM_SIZE_IN_BYTES -1;
        /* When BSR is greater that the dataSize allocated */
        if(temp_BsrLoad > (SInt64)dataSize)
        {
            /* validate temp_BsrLoad with "maxData"- limit set by ueCategory*/
            if(temp_BsrLoad > (SInt64)maxData)
            {
                *minDataSize = dataSize;
                /* naxDataSize can not be more than the limit set by ue category*/
                *maxDataSize = maxData;
#ifdef PERF_STATS
                /*  number of ms where respective UE is limited in the
                 *  UL direction by its UE capability */
                gMacUePerfStats_p[ueULContext_p->ueIndex].lteUeMacSchedULPerfStats.\
                    totalTimeRestrictUeInUlbyUeCap++;
#endif
            }
            else
            {
                *minDataSize = dataSize;
                /* maxDataSize equals temp_BsrLoad if Bsr<maxData set by uecategory*/
                *maxDataSize = temp_BsrLoad;
            }
        }
        else  
        {
            /* when Bsr us less than allocated dataSize,
               set min and max data size as temp_BsrLoad*/
            *minDataSize = temp_BsrLoad;
            *maxDataSize = temp_BsrLoad;
        }
    }
    /* SPR 5713 changes start */
    VALIDATE_UPLINK_RB(mcsIndex,minRBAsPerUeRelease,ueULContext_p->ulGrantMinDataSize,
                       ueULContext_p->ueComplianceRelease);
    if(minRBAsPerUeRelease > allocatedRB)
    {
       allocatedRB = minRBAsPerUeRelease;
    }
    /* SPR 5713 changes end */
    return allocatedRB;
}
/* ULA_CHG */

/*****************************************************************************
 * Function Name  : putEntryInULAperiodicCQIQueue
 * Inputs         : ueIndex - Index of UE whose is non zero,
 *                  bitMaskForAperiodicReport - Bitmask to check Aperiodic Report
 *                      ON/OFF,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function creates a new srNode and inserts
 this node in the globalSrQueue.*/
/*****************************************************************************/
/*CA Changes start  */
 MacRetType putEntryInULAperiodicCQIQueue(UInt32 ueIndex, InternalCellIndex internalCellIndex
        /* CA_phase2_csi_code Start */
        ,UInt8 bitMaskForAperiodicReport
        /* CA_phase2_csi_code End */
        )
/*CA Changes end  */
{
    ULGrantRequestQueueNode *ulGrantRequestQueueNode_p = PNULL;
    /*Prepare UL grant for Contention Resolution and push to queue*/
    GET_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(ulGrantRequestQueueNode_p, ULGrantRequestQueueNode);


    /* + coverity 24552 */
    if (PNULL == ulGrantRequestQueueNode_p)
    {
        return MAC_FAILURE;
    }
    /* - coverity 24552 */

    ulGrantRequestQueueNode_p->ueIndex = (UInt16)ueIndex;
    if ( ((ULUEContext *)(ulUECtxInfoArr_g[ueIndex].ulUEContext_p))->bsrNetTotal)
    {
        ulGrantRequestQueueNode_p->ulGrantRequestType = 
            CQI_WITH_DATA_ULGRANT_REQ_MSG;
    }
    else
    {
        ulGrantRequestQueueNode_p->ulGrantRequestType = 
            CQI_WITHOUT_DATA_ULGRANT_REQ_MSG;
    }
    /* + SPR_14408 */
    /* CA_phase2_csi_code Start */
    ulGrantRequestQueueNode_p->aperiodicCQIToBeRequested = bitMaskForAperiodicReport;
    /* CA_phase2_csi_code End */
    /* - SPR_14408 */

    if (!(ENQUEUE_UL_GRANT_APERIODIC_CQI_Q(ULGrantRequestQueueNode, 
                    (void *)&ulGrantRequestQueueNode_p->ulGrantRequestNodeAnchor,
                    internalCellIndex)))
        {
            lteWarning("[%s]: ERROR: pushNode() Failed\n", __func__);
            FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(ulGrantRequestQueueNode_p);
            return MAC_FAILURE;
        }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : initULSchedulerStrategy
 * Inputs         : numCellsConfigured - Number of configured cell.
 * Outputs        : initialize all the Queue and List Used by the UL Strategy
 * Returns        : None
 * Description    : This function initialize All queues and lists to be used 
 *                  by DL Scheduler Strategy.
 *****************************************************************************/
/*CA Changes start  */
void initULSchedulerStrategy (UInt8 numCellsConfigured)
/*CA Changes end  */
{
    UInt8  cellCount = 0;
    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,
            "Entry[UL-Strategy][%s]\n", __func__);
    /* CA Changes start */
    InitParams *initParams_p = PNULL;
    /* Start 128 UE: Memory Changes */
    /* 
    ** 128 UE Memory Changes: Replaced Macros which define the maximum UE or LC
    ** with their corresponding global variables.
    */
    /*Begin Qos Mem pool creations */
    /* Creating the mempool for Dl Qos Strategy Node */
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {    
       initParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[cellCount]\
                    ->initParams_p;
        createMemPool(sizeof(ULQosStrategyLCNode),
          (initParams_p->macMaxNumOfUeSupported * macParams_g.macMaxNumOfLcSupported));

    /* Creating the mempool for Dl Qos token update UE Node */
        createMemPool(sizeof(TknUpdateUENode),
                          (initParams_p->macMaxNumOfUeSupported));

    /*
    ** Moved memory allocation for TokenPeriodicityLisElem and TokenPeriodicityMap
    ** structures to initTokenPeriodicityArray because the number of blocks to be created
    ** depend on Token periodicity which comes in cell configuration parameters.
    */

    /* Creating the mempool for UL Grant Request  Node */
        createMemPool(sizeof(ULGrantRequestQueueNode),
                           (initParams_p->macMaxNumOfUeSupported));
    /* End 128 UE: Memory Changes */
    }
    /* CA Changes end */
    /* Initializing Pending UL Grant Queue */
    /*CA Changes start  */
    CONSTRUCT_AND_INIT_UL_GRANT_PUSCH_PENDING_QUEUE_Q(numCellsConfigured);

    /* Initializing Pending PUSH Failure UL Grant Queue */
    CONSTRUCT_AND_INIT_UL_GRANT_PUSCH_FAILURE_QUEUE_Q(numCellsConfigured);

    /* Cyclomatic_complexity_changes_start */

    /* Initializing Aperiodic UL Grant Queue */
    CONSTRUCT_AND_INIT_UL_GRANT_APERIODIC_CQI_Q(numCellsConfigured);

    /* Initializing Contention Resolution  UL Grant Queue */
    createMemPool(sizeof(LTE_SQUEUE), numCellsConfigured);

    /* ICIC changes start */
    /* Initializing Cell Edge Priority Queue for UL */
    createMemPool(sizeof(LTE_SQUEUE), numCellsConfigured);
    /* ICIC changes end */

    /* zero and non zero BSR trigger queue */
    createMemPool(sizeof(LTE_SQUEUE) * NUM_OF_DEMUX_THREADS, numCellsConfigured);

    /* Initializing Non Zero BSR List */
    createMemPool(sizeof(LTE_LIST), numCellsConfigured);

    /* ul qos strategy lc tree */
    createMemPool(sizeof(LTE_TREE), numCellsConfigured*MAX_VALID_UE_TYPE);

    /* SPR 3608 changes start */
    createMemPool(sizeof(LTE_SQUEUE), numCellsConfigured);

    createMemPool(sizeof(ULStrategyScheduleInfo), numCellsConfigured);
    createMemPool(sizeof(AperiodicModeMinRBInfo), numCellsConfigured);
    /* SPR 3608 changes end */

    createMemPool(sizeof(EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE), numCellsConfigured);
    createMemPool(sizeof(EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE), numCellsConfigured);
    
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    /* Initializing ulStrategyTdd0SsfTxList_g and 
     * ulStrategyTdd0SsfTxListUSfOffset7_g List */
    /* CA TDD CHANGES START */
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        listInit(&ulStrategyTdd0SsfTxList_g[cellCount]);
        listInit(&ulStrategyTdd0SsfTxListUSfOffset7_g[cellCount]);
    }
    /* CA TDD CHANGES END */

#endif
    /*TDD Config 0 Changes Start*/
    createMemPool(sizeof(ULPfsGlobals), numCellsConfigured);
    createMemPool(sizeof(ULUESchedulerInfo), numCellsConfigured);

    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        /* Initializing Contention Resolution  UL Grant Queue */
        GET_MEM_FROM_POOL(LTE_SQUEUE, ulGrantContentionResQueue_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        sQueueInit(ulGrantContentionResQueue_gp[cellCount]);

        /* Initializing Cell Edge Priority Queue for UL */
        GET_MEM_FROM_POOL(LTE_SQUEUE, ulCellEdgeUserPriorityQueue_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        sQueueInit(ulCellEdgeUserPriorityQueue_gp[cellCount]);
        
        /* zero and non zero BSR trigger queue and list and UL QOS Tree */
        initZeroNonZeroBsr( cellCount );
        
        /* SPR 3608 changes start */
        GET_MEM_FROM_POOL(LTE_SQUEUE, ulHighPriorityGrantQueue_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        sQueueInit(ulHighPriorityGrantQueue_gp[cellCount]);
        
        GET_MEM_FROM_POOL(ULStrategyScheduleInfo, ulStrategyScheduleInfo_gp[cellCount],sizeof(ULStrategyScheduleInfo), PNULL);
        GET_MEM_FROM_POOL(AperiodicModeMinRBInfo, aperiodicModesRBInfo_gp[cellCount],sizeof(AperiodicModeMinRBInfo), PNULL);
        /* SPR 3608 changes end */
       
        /* Init TPC queues */
        initExplicitUlPuschTpc( cellCount );
        
        /* ul PFS */
        GET_MEM_FROM_POOL(ULPfsGlobals, ulPfsGlobals_gp[cellCount],sizeof(ULPfsGlobals), PNULL);
        ulPfsGlobals_gp[cellCount]->calcBsrloadMeanItr = 0;
        ulPfsGlobals_gp[cellCount]->bsrLoadComputed = FALSE;

        GET_MEM_FROM_POOL(ULUESchedulerInfo, ulScheduleInfo_gp[cellCount],sizeof(ULUESchedulerInfo), PNULL);

        ulPfsGlobals_gp[cellCount]->calcBsrloadMeanItr = 0;

        if(UL_FAIR_SCHEDULING == cellSpecificParams_g.\
                cellConfigAndInitParams_p[cellCount]->initParams_p->ulSchdConfig.\
                ulSchedulerStrategy)
        {
            ulPfsInitMacSchedulerData(cellCount);
        }
        /* Initialize SPS Queues*/
        initSPSInULScheduler(cellCount);
    }

    CONSTRUCT_AND_INIT_UL_DRX_ON_OFF_TRIGGER_Q(numCellsConfigured);
    CONSTRUCT_AND_INIT_UL_MEAS_GAPON_OFF_TRIGGER_Q(numCellsConfigured);

    /* init UL UE states array */
    initUlQosStrategyScheduleSM(numCellsConfigured);

#ifdef FDD_CONFIG
    CONSTRUCT_AND_INIT_TTIB_SCHEDULE_Q(numCellsConfigured);
#endif

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,
            "Exit[UL-Strategy][%s]\n", __func__);
}


/* Cyclomatic_complexity_changes_start */
/****************************************************************************
 * Function Name  : cleanupULSchedulerStrategy
 * Inputs         : internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This API cleanup the cleanup ULSchedulerStrategy.
 ****************************************************************************/
/*CA Changes start  */
void cleanupULSchedulerStrategy(InternalCellIndex internalCellIndex)
/*CA Changes end  */
{

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,
            "Entry[UL-Strategy][%s]\n", __func__);

    /* Clean up for  Pending UL Grant Queue */
    cleanUpUlGrantQ(ulGrantPendingQueue_gp[internalCellIndex]);
    /* Clean up for  Pending PUSH Failure UL Grant Queue */
    cleanUpUlGrantQ(ulGrantPUSCHFailureQueue_gp[internalCellIndex]);
    /*Clean up for Contention Resolution  UL Grant Queue */
    cleanUpUlGrantConResQ(internalCellIndex);

    /* Clean up for Aperiodic UL Grant Queue */
    cleanUpAperiodicUlCqiQ(internalCellIndex);
    /*Clean up for  Zero NonZero BSR Trigger Queues */
    cleanUpBsrTriggerQ(internalCellIndex);


    /* Clean up for  Non Zero Queue Load and Harq List */
    cleanUpNonZeroBsrList(internalCellIndex);
    /* SPR 6941 FIX Start */
    /* Clean up for  Non Zero BSR List */
    /* SPR 6941 End Start */

    /* SPR 4834 Fix Start */
    cleanUpCEUserPriorityQ( internalCellIndex);
    /* SPR 4834 Fix End */

    /*Coverity-5.3.0 CID-54632*/
    cleanUpExpilicitPuschTpcScheduleQ(explicitULPuschTpcScheduleQueue_gp[internalCellIndex]);
    /*Coverity-5.3.0 CID-54632*/

    /*Coverity-5.3.0 CID-54633*/
    cleanUpExpilicitPuschTpcScheduleQ(explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex]);
    /*Coverity-5.3.0 CID-54633*/

    /*Coverity-5.3.0 CID-54634*/
    cleanUpDrxOnOffTriggerQ( internalCellIndex);
    /*Coverity-5.3.0 CID-54634*/

    /*Coverity-5.3.0 CID-54638*/
    cleanUpMeasGapOnOffTriggerQ( internalCellIndex);
    /*Coverity-5.3.0 CID-54638*/

    /*Coverity-5.3.0 CID-54639*/
    cleanUpUlSpsQueues( internalCellIndex);
    /*Coverity-5.3.0 CID-54644*/
    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,
            "Exit[UL-Strategy][%s]\n", __func__);

    return;
}

/* Cyclomatic_complexity_changes_end */
/*********************************************************************************************
 * Function Name  : processExplicitULDCIAndPHRScheduleQueue
 * Inputs         : explicitDCIULAndPHRQueueCount - Its the total Nodes present in 
 *                                                  explicitULDCIQueue which needs
 *                                                  to be processed,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  maxUEsToBeScheduledUL_p - pointer to the max UE to be
 *                                            scheduled in UL,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  explicitULDCIAndPHRScheduleQueue_p - Pointer to
 *                                                       EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE,
 *                  ulDelay - UE UL Delay,
 *                  internalCellIndex - cell index used at MAC.
 *                  ulSubFrameNum - ulSubFrameNumber for which grant is being given
 * Outputs        : Process UE and allocate RBs for TPC
 * Returns        : None
 * Description    : This function will process the explicit DLDCI queue and
 *                  allocating RBs to UEs for sending TPC.
 **********************************************************************************************/
void processExplicitULDCIAndPHRScheduleQueue (
        UInt32 explicitDCIULAndPHRQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE * explicitULDCIAndPHRScheduleQueue_p
#ifdef FDD_CONFIG
        ,UInt32 ulDelay
#endif
        ,InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */

    /* +- SPR 18490 */

    ExplicitULDCIAndPHRNode *node_p  = PNULL;
    UInt32 tempUeId                     = 0;
    ULStrategyTxNode *ulStrategyTxNode_p= PNULL;
    ULUEContext * tempUplinkContext_p   = PNULL;
    ULUEContextInfo * tempUplinkContextInfo_p = PNULL;
    MeasGapContextUL * measGapContext_p   = PNULL;
    UInt8 tempMaxUEsToBeScheduledUL    = *maxUEsToBeScheduledUL_p;
    ULUeScheduleInfo *ulUeScheduleInfo_p = PNULL;
    UInt8 avgMcs                                    = 0;
    /* SPR 15909 fix start */
    tickType_t ttiTick = currentGlobalTTITickCount;
    /* SPR 15909 fix end */
    /* ICIC Change start */
    UInt8 userLocationType = CC_USER ;
    /* ICIC Change End */
    /* +DYNAMIC_ICIC */
    UInt16 numOfUe = 0;
    /* -DYNAMIC_ICIC */
    /* +- SPR 18490 */
#ifdef TDD_CONFIG
    /* SPR 23393 Changes Start */
    UInt32 sysFrameNum = 0;
    UInt32 subframe     = 0;
    GET_SFN_SF_FROM_TTI(ttiTick, sysFrameNum, subframe)
    /* SPR 23393 Changes End */
    LTE_GCC_UNUSED_PARAM(sysFrameNum)
#endif
    /* +- SPR 18490 */
#ifdef FDD_CONFIG 
        UInt8 ttiMapId1 = TTI_MAP_FREE;
        UInt8 ttiMapId2 = TTI_MAP_FREE;
#endif
    /* EICIC +*/
    ExplicitULDCIAndPHRNode *nodeScan_p  = PNULL;
    UInt8	isTraverseList = LTE_FALSE;
    /* EICIC -*/
    /*Processing for Queue Count and till the Available RBs*/
    /* ICIC changes start */
    /* +DYNAMIC_ICIC */
    while ((explicitDCIULAndPHRQueueCount)&&(numberOfAvailableRBs_p[CC_CE_REGION]) && 
            (tempMaxUEsToBeScheduledUL))
        /* -DYNAMIC_ICIC */
        /* ICIC changes end */
    {
        DEQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
            explicitULDCIAndPHRScheduleQueue_p,ExplicitULDCIAndPHRNode,(void *)&node_p);

        if (node_p)
        {
            tempUeId = node_p->ueIndex;
            tempUplinkContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
            if (! tempUplinkContextInfo_p->pendingDeleteFlag)
            {
                tempUplinkContext_p = (tempUplinkContextInfo_p->ulUEContext_p);

		    /* EICIC +*/
		    /* Pushing UE back in trigger queue.*/    

		    if(PNULL == tempUplinkContext_p )
		    {
			    FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
			    explicitDCIULAndPHRQueueCount--;
			    continue;
		    }
/*Complexity Start*/
		    /* EICIC Optimization +- */
		    if(isEicicEnabled_g[internalCellIndex])
		    {

		    if( ((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == tempUplinkContext_p->userType))
				||((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == tempUplinkContext_p->userType)))
		    {           
			    if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
							    explicitULDCIAndPHRScheduleQueue_p,
							    ExplicitULDCIAndPHRNode,(void *)&(node_p->nodeAnchor))))
			    {

				    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
						    LOGERROR,MAC_UL_Strategy,\
						    currentGlobalTTITickCount,\
						    node_p->ueIndex,
#ifdef TDD_CONFIG
						    sysFrameNum,
#else
                            0,
#endif
                            0,0,0,0.0,0.0,__func__,
						    "Error in Pushing node in\
						    explicitULDCIAndPHRScheduleQueue_p");

				    FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
			    }
			    isTraverseList = LTE_TRUE;
			    explicitDCIULAndPHRQueueCount--;
			    continue; 
		    }
			    /*Complexity End*/
		    /* EICIC - */
		    }
		    /* EICIC Optimization +- */

                /*SPR 4038 Start*/
                if(PNULL == tempUplinkContext_p )
                {
                    FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                    explicitDCIULAndPHRQueueCount--;
                    continue;
                }
                /*SPR 4038 End*/
                measGapContext_p = &tempUplinkContext_p->measGapContext;

                if ( (UL_TPC_WITHOUT_DATA == node_p->ulGrantRequestType) && 
                      (tempUplinkContext_p->tpcInfoForPusch.tpcTriggeredValue == TPC_NO_CHANGE) )
                {
                    FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                    /* +DYNAMIC_ICIC */
                    explicitDCIULAndPHRQueueCount--;
                    /* -DYNAMIC_ICIC */
                    continue;
                }
                else if ( (UL_EXPLICIT_PHR_WITHOUT_DATA == node_p->ulGrantRequestType) &&
                          (getCurrentTick() < tempUplinkContext_p->phrProhibitTimerExpiryTick) )
                {                            /*push the node in pending queue*/
                    if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
                                /*CA Changes start  */
                                explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex],
                                /*CA Changes end  */
                                ExplicitULDCIAndPHRNode,(void *)&(node_p->nodeAnchor))))
                    {
                        FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                    }
                    /* +DYNAMIC_ICIC */
                    explicitDCIULAndPHRQueueCount--;
                    /* -DYNAMIC_ICIC */
                    continue;
                }
                    
             /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                if (tempUplinkContext_p->isAlreadyConsideredFlag !=
                            ulReferenceFlag_g[internalCellIndex])
#elif TDD_CONFIG
                if (tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                    !=  ulReferenceFlag_g[internalCellIndex])
#endif
             /* TDD Config 0 Changes End */
                {
                    /* \code
                     * if schType == SPS_SCHEDULING, DRX is ON and Meas 
                     *  gap period is ON
                     *     then do not schedule this UE.
                     * */
                    if((DRX_STATE_ON == tempUplinkContext_p->drxCurrentState)||
                            (TRUE == measGapContext_p->isMeasGapPeriodOn) || 
                            ( SPS_SCHEDULING == tempUplinkContext_p->schType )
                            /*HD FDD Chages Start*/
#ifdef HD_FDD_CONFIG
                            ||
                            /* If HD FDD UE is in  Explicit CQI UL grant  Q,then check Schedule map whether
                             * it can be scheduled in currenttick or not*/
                            ((TRUE == tempUplinkContext_p->isHDFddFlag) && 
                             (!CAN_SCHEDULE_UL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY, \
                                                         tempUeId , hdUeContextInfo_g.hdfddUeSchedMap)))
#endif
                            /*HD FDD Chages End*/
                      )
                    {
                        LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_UL_LOG_ID,
                                LOGDEBUG, MAC_UL_Strategy,
                                currentGlobalTTITickCount,
                                tempUeId,
                                tempUplinkContext_p->drxCurrentState,
                                measGapContext_p->isMeasGapPeriodOn,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                __func__,"");
                        /*push the node in pending queue*/
                        if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
                                   /*CA Changes start  */
                                   explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex],
                                   /*CA Changes end  */
                                    ExplicitULDCIAndPHRNode,(void *)&(node_p->nodeAnchor))))
                        {
                                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                        LOGERROR,MAC_UL_Strategy,\
                                        currentGlobalTTITickCount,\
                                        node_p->ueIndex,\
                                        0,0,0,0,0.0,0.0,__func__,
                                        "Error in Pushing node in\
                                        explicitULPuschTpcScheduleFailureQueue_g");

                                FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                        }
                        /* +DYNAMIC_ICIC */
                        explicitDCIULAndPHRQueueCount--;
                        /* -DYNAMIC_ICIC */
                        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                        if(TRUE == tempUplinkContext_p->isHDFddFlag)
                        {
                            /* Increment the statistics counter UE denied UL resources due to
                             * DL clash in the same TTI of HD FDD UE*/
                            UPDATE_UE_DENIED_UL_RESOURCE(tempUeId);

                        }
#endif
                        /*HD FDD Changes End*/
                        continue;
                    }
#ifdef TDD_CONFIG
                    UInt8 ulHarqProcessId = BUSY_ALL_HARQ ;
                    /* Get the free ID in ulHarqProcessId
                       if all the harq id is busy the skip this node and goto the next node in
                       the list and start this procedure again.
                       */
                    /* TDD Config 0 and 6 Changes Start */
                    /* +- SPR 17777 */
                    /* Get the free ID in ulHarqProcessId
                       if all the harq id is busy then skip this node and goto the next node in
                       the list and start this procedure again.
                       */
                    /* SPR 11257 Changes Start*/
    	            ulHarqProcessId = getFreeULHarqId(tempUplinkContextInfo_p->ulUEContext_p,
				            subframe,internalCellIndex,ulSubFrameNum,currentGlobalTTITickCount);
                    /* +- SPR 17777 */
                    /* SPR 11257 Changes End*/
                    /* TDD Config 0 and 6 Changes End*/
#elif FDD_CONFIG
                    /* TTIB_Code Start */
                    UInt8 ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount +
                            ulDelay + FDD_HARQ_OFFSET);
                    if ( tempUplinkContext_p->ttiBundlingEnabled )
                    {
                        ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
                    }
                    ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
                    ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET +
                            MAX_HARQ_PROCESS_NB);
                    /* TTIB_Code End */
#endif
#ifdef TDD_CONFIG
                    if ( BUSY_ALL_HARQ == ulHarqProcessId)
#elif FDD_CONFIG
                    /* TTIB_Code Start */
                    if((HARQ_PROCESS_FREE != tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                                ||(tempUplinkContext_p->ttiBundlingEnabled &&
                                    (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,tempUplinkContext_p)))
                                ||(!(tempUplinkContext_p->ttiBundlingEnabled)&&
                                    ((TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId1]) ||
                                     ( TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId2])))
                                /*SPR 7944 START*/
                                /* + SPS_TDD_Changes */
                                /*
                                   ||checkSpsOccasionTti(tempUplinkContext_p,ulHarqProcessId)
                                   ||checkSpsActTti(tempUplinkContext_p,ulHarqProcessId)
                                   */
                                /* - SPS_TDD_Changes */
                                /*SPR 7944 END*/
                      )
                    /* TTIB_Code End */
#endif
                    {
                        FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                        /* +DYNAMIC_ICIC */
                        explicitDCIULAndPHRQueueCount--;
                        /* -DYNAMIC_ICIC */
                        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                            "[%s]:UE ID %d is not schedule due to busy HARQ"\
                            " process Global Tick:%d\n",
                            __func__,tempUeId,currentGlobalTTITickCount);
                        continue;
                    }
                    avgMcs = tempUplinkContext_p->mcsIndex;
#ifdef FDD_CONFIG
                    if ( tempUplinkContext_p->ttiBundlingEnabled && (avgMcs > MAX_TTI_BUNDLE_MCS) )
                    {
                        avgMcs = MAX_TTI_BUNDLE_MCS;    
                    }
#endif             

                    userLocationType = tempUplinkContext_p->userLocationType ;
                    /* If user is cell edge then we are checking if
                       maxCellEdgeUEsToBeScheduledUL_g is reached
                       or cell edge region RB is not available */
                    /*CA Changes start  */
                    if (CE_USER == userLocationType && (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]
                    /*CA Changes end  */
                                || !numberOfAvailableRBs_p[userLocationType]))
                    {
                        // push node in the queue
                        if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
                                    /*CA Changes start  */
                                    explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex],
                                    /*CA Changes end  */
                                    ExplicitULDCIAndPHRNode,(void *)&(node_p->nodeAnchor))))
                        {
                            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                    LOGERROR,MAC_UL_Strategy,\
                                    currentGlobalTTITickCount,\
                                    node_p->ueIndex,\
                                    0,0,0,0,0.0,0.0,__func__,
                                    "Error in Pushing node in\
                                    explicitULPuschTpcScheduleFailureQueue_g");
                            FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                        }

                        /* +DYNAMIC_ICIC */
                        explicitDCIULAndPHRQueueCount--;
                        /* -DYNAMIC_ICIC */
                        continue;
                    }                    
                    /* ICIC Change End */
                    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                    if (tempUplinkContext_p->isAlreadyConsideredFlag !=
                        ulReferenceFlag_g[internalCellIndex])
#elif TDD_CONFIG
                    if (tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                        !=  ulReferenceFlag_g[internalCellIndex])
#endif
                    /* TDD Config 0 Changes End */
                    {
                        /*Checking if Already being considered for RB Distribution*/
                        UInt32 tempNetBsrLoad = tempUplinkContext_p->bsrNetTotal;

                        /*If the UE has zero BSR available*/
                        if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->ulSchdConfig.ulSchedulerStrategy == UL_QOS_SCHEDULING)
                        {
                            /* ICIC changes start */
                            if ( numberOfAvailableRBs_p[userLocationType] >=
                                    /* +DYNAMIC_ICIC */
                                    MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD )
                                /* -DYNAMIC_ICIC */
                                /* ICIC changes end */
                            {
#ifdef FDD_CONFIG
                                /* TTIB_Code Start */
                                if ( tempUplinkContext_p->ttiBundlingEnabled )
                                {
                                    /*CA Changes start  */
                                    if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD ) 
                                          ||(MAC_FAILURE == processMeasGapValidation(
                                             tempUplinkContext_p,currentGlobalTTITickCount,ulDelay )))
                                    {
                                        if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
                                                        explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex],
                                                        ExplicitULDCIAndPHRNode,(void *)&(node_p->nodeAnchor))))
                                        {
                                            FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                                        }
                                        explicitDCIULAndPHRQueueCount--;
                                        continue;
                                    }
                                    availableRBForTtiBundlingPerTick_g[internalCellIndex] -= MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD;
                                    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                                            "[%s][%d]:TTIB UE ID %d scheduled RB:1 MCS:%d \n",
                                            __func__,getCurrentTick(),tempUeId,avgMcs);
                                    /*CA Changes end  */
                                }
                                /* TTIB_Code End */
#endif                                 
                                /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                                GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
                                GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                                sizeof(ULStrategyTxNode),PNULL);
#endif
                                /*+COVERITY 5.3.0 - 26279*/
                                if(PNULL == ulStrategyTxNode_p)
                                {
                                    if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
                                                    /*CA Changes start  */
                                                    explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex],
                                                    /*CA Changes end  */
                                                    ExplicitULDCIAndPHRNode,(void *)&(node_p->nodeAnchor))))
                                    {
                                        FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                                    }
                                    explicitDCIULAndPHRQueueCount--;
                                    continue;
                                }
                                ulStrategyTxNode_p->requiredRB = 0;
                                ulStrategyTxNode_p->maxDataSize = 0;
                                ulStrategyTxNode_p->minDataSize = 0;

                                /*-COVERITY 5.3.0 - 26279*/
                                /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                                tempUplinkContext_p->isAlreadyConsideredFlag
                                    = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
                                tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                                    = ulReferenceFlag_g[internalCellIndex];
#endif
                                /* TDD Config 0 Changes End */
                                tempMaxUEsToBeScheduledUL--;
                                ulStrategyTxNode_p->ueIndex = (UInt16)tempUeId;

                                ulStrategyTxNode_p->requiredRB =
                                    /* +DYNAMIC_ICIC */
                                    MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD;
                                /* -DYNAMIC_ICIC */
                                numberOfAvailableRBs_p[userLocationType] -= 
                                    /* +DYNAMIC_ICIC */
                                    MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD;
                                /* -DYNAMIC_ICIC */

                                if (userLocationType == CE_USER)
                                {
                                    /*CA Changes start  */
                                    maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                                    /*CA Changes end  */
                                    numberOfAvailableRBs_p[CC_CE_REGION] -= 
                                        /* +DYNAMIC_ICIC */
                                        MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD;
                                    /* -DYNAMIC_ICIC */
                                }

                                else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                                        numberOfAvailableRBs_p[CE_REGION])
                                {
                                    numberOfAvailableRBs_p[CE_REGION] = 
                                        numberOfAvailableRBs_p[CC_CE_REGION];
                                }
                                /* ICIC changes end */
                                ulStrategyTxNode_p->strictAllocRB = TRUE;
                                if (UL_EXPLICIT_PHR_WITHOUT_DATA == node_p->ulGrantRequestType)
                                {
                                    ulStrategyTxNode_p->ulGrantRequestType = UL_EXPLICIT_PHR_WITHOUT_DATA;
                                }
                                else
                                {
                                    ulStrategyTxNode_p->ulGrantRequestType = UL_TPC_WITHOUT_DATA;
                                }
                                ulStrategyTxNode_p->ttiCounter =  ttiTick;

                                ulStrategyTxNode_p->minDataSize =
                                    /* +DYNAMIC_ICIC */
                                    TPC_PHR_GRANT_DATA_SIZE;
                                /* -DYNAMIC_ICIC */

                                ulStrategyTxNode_p->maxDataSize =
                                    determineUlTBSize(
                                            avgMcs,
                                            /* +DYNAMIC_ICIC */
                                            MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD);
                                /* -DYNAMIC_ICIC */

                                ulStrategyTxNode_p->avgMcs = avgMcs;
#ifdef TDD_CONFIG
                                /*TDD Config 0 Changes Start*/
                                ulStrategyTxNode_p->scheduledCount = 0;
                                /*TDD Config 0 Changes End*/
#endif

                                /*putEntry of ULStrategyTXNode in UL Strategy Schedule Info */
                                /*CA Changes start  */
                                ulUeScheduleInfo_p = &(ulStrategyScheduleInfo_gp[internalCellIndex]->ulUeScheduleInfo
                                        [ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount]);
                                /*CA Changes end  */

                                ulUeScheduleInfo_p->ulStrategyTxNode_p = ulStrategyTxNode_p;


                                ulUeScheduleInfo_p->ueULContext_p =
                                    tempUplinkContext_p;

                                tempUplinkContext_p->ulStrategyTxNode_p = ulStrategyTxNode_p;

                                /* Increament the scheduled UE count */
                                /*CA Changes start  */
                                ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount++;
                                /*CA Changes end  */
                            }
                        }
                        else
                        {
                            /* Case when either RR or PF scheduler strategy */

                            /*If the UE has zero BSR available*/
                            if (!tempNetBsrLoad)
                            {
#ifdef FDD_CONFIG
                                /* TTIB_Code Start */
                                if ( tempUplinkContext_p->ttiBundlingEnabled )
                                {
                                    /*CA Changes start  */
                                    if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD ) 
                                          ||(MAC_FAILURE == processMeasGapValidation(
                                             tempUplinkContext_p,currentGlobalTTITickCount,ulDelay )))
                                    {
                                        if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
                                                        explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex],
                                                        ExplicitULDCIAndPHRNode,(void *)&(node_p->nodeAnchor))))
                                        {
                                            FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                                        }
                                        explicitDCIULAndPHRQueueCount--;
                                        continue;
                                    }
                                    availableRBForTtiBundlingPerTick_g[internalCellIndex] -= MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD;
                                    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                                            "[%s][%d]:TTIB UE ID %d scheduled RB:1 MCS:%d \n",
                                            __func__,getCurrentTick(),tempUeId,avgMcs);
                                    /*CA Changes end  */
                                }
                                /* TTIB_Code End */
#endif                                 
                                /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                                GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
                                GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                                sizeof(ULStrategyTxNode),PNULL);
#endif
                                /*+COVERITY 5.3.0 - 26279*/
                                if(PNULL == ulStrategyTxNode_p)
                                {
                                    if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
                                                    /*CA Changes start  */
                                                    explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex],
                                                    /*CA Changes end  */
                                                    ExplicitULDCIAndPHRNode,(void *)&(node_p->nodeAnchor))))
                                    {
                                        FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                                    }
                                    explicitDCIULAndPHRQueueCount--;
                                    continue;
                                }
                                ulStrategyTxNode_p->requiredRB = 0;
                                ulStrategyTxNode_p->maxDataSize = 0;
                                ulStrategyTxNode_p->minDataSize = 0;

                                /*-COVERITY 5.3.0 - 26279*/
                                /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                                tempUplinkContext_p->isAlreadyConsideredFlag
                                    = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
                                tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                                    = ulReferenceFlag_g[internalCellIndex];
                                /*TDD Config 0 Changes Start*/
                                ulStrategyTxNode_p->scheduledCount = 0;
                                /*TDD Config 0 Changes End*/
#endif
                                /* TDD Config 0 Changes End */
                                tempMaxUEsToBeScheduledUL--;
                                ulStrategyTxNode_p->ueIndex = (UInt16)tempUeId;

                                /* +DYNAMIC_ICIC */
                                ulStrategyTxNode_p->requiredRB =
                                    MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD;
                                /* ICIC changes start */
                                numberOfAvailableRBs_p[userLocationType] -= 
                                    MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD;

                                if (userLocationType == CE_USER)
                                {
                                    /*CA Changes start  */
                                    maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                                    /*CA Changes end  */
                                    numberOfAvailableRBs_p[CC_CE_REGION] -= 
                                        MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD;
                                    /* -DYNAMIC_ICIC */
                                }

                                else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                                        numberOfAvailableRBs_p[CE_REGION])
                                {
                                    numberOfAvailableRBs_p[CE_REGION] = 
                                        numberOfAvailableRBs_p[CC_CE_REGION];
                                }
                                /* ICIC changes end */

                                ulStrategyTxNode_p->strictAllocRB = TRUE;
                                if (UL_EXPLICIT_PHR_WITHOUT_DATA == node_p->ulGrantRequestType)
                                {
                                    ulStrategyTxNode_p->ulGrantRequestType = UL_EXPLICIT_PHR_WITHOUT_DATA;
                                }
                                else
                                {
                                    ulStrategyTxNode_p->ulGrantRequestType = UL_TPC_WITHOUT_DATA;
                                }

                                 /*SPR_11774 Fix Start*/
#ifdef TDD_CONFIG
                                 ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessId;
#endif
                                 /*SPR_11774 Fix End*/

                                ulStrategyTxNode_p->ttiCounter =  ttiTick;

                                ulStrategyTxNode_p->minDataSize =
                                    /* +DYNAMIC_ICIC */
                                    TPC_PHR_GRANT_DATA_SIZE;
                                /* -DYNAMIC_ICIC */

                                ulStrategyTxNode_p->maxDataSize =
                                    determineUlTBSize(
                                            avgMcs,
                                            /* +DYNAMIC_ICIC */
                                            MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD);
                                /* -DYNAMIC_ICIC */
                                ulStrategyTxNode_p->avgMcs = avgMcs;

                                /*putEntry of ULStrategyTXNode in PDCCH Queue */
                                /*CA Changes start  */
                                if (!(putEntryInULSchedulerNewTxQueue(ulStrategyTxNode_p, internalCellIndex
                                /*CA Changes end  */
                                                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                                                ,currentGlobalTTITickCount
                                                ,ulSubFrameNum
                                                ,tempUplinkContext_p
#endif
                                                /* TDD Config 0 Changes End */
                                                )))
                                {
                                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                            LOGERROR,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            ulStrategyTxNode_p->ueIndex,0,0,0,0,0.0,0.0,
                                            __func__,"ulStrategyTxNode_p entry failed");
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                                        FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
                                        freeMemPool(ulStrategyTxNode_p);
#endif
                                }
                                else
                                {
                                    LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                                            LOGDEBUG,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            ulStrategyTxNode_p->ueIndex,\
                                            ulStrategyTxNode_p->requiredRB,\
                                            ulStrategyTxNode_p->strictAllocRB,\
                                            ulStrategyTxNode_p->minDataSize,\
                                            ulStrategyTxNode_p->maxDataSize,\
                                            ulStrategyTxNode_p->avgMcs,\
                                            ulStrategyTxNode_p->ttiCounter,\
                                            __func__,"ulStrategyTxNode_p parameters");

                                    /* +COVERITY 5.0 */
                                    LOG_MAC_MSG (UL_EXPLICIT_DCI_QUEUE_SCHEDULED_LOG_ID, LOGDEBUG,
                                            MAC_UL_Strategy, getCurrentTick (), tempUplinkContext_p->ueIndex,
                                            ulHarqProcessId,
                                            ulStrategyTxNode_p->requiredRB,
                                            tempUplinkContext_p->bsrNetTotal,
                                            ulStrategyTxNode_p->ulGrantRequestType,
                                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
                                    /* -COVERITY 5.0 */
                                }
                            }
                            /*else if the UE has non zero BSR*/
                            else
                            {
                                /* this pointer may be null if
                                 * explicitULPuschTpcScheduleFailureQueue_g
                                 * is being processed */
                                /* +DYNAMIC_ICIC */
                                numOfUe = ulScheduleInfo_gp[internalCellIndex]->numOfUe;
                                ulScheduleInfo_gp[internalCellIndex]->ulUeSchedulerInfoArr[numOfUe].ueIndex =
                                    (UInt16)tempUeId;
                                ulScheduleInfo_gp[internalCellIndex]->numOfUe++;
                                /*
                                numOfUe = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                          initParams_p->ulSchdConfig.numOfUe;
                                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                                ulSchdConfig.ulUeSchedulerInfoArr[numOfUe].ueIndex = (UInt16)tempUeId;
                                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                initParams_p->ulSchdConfig.numOfUe++;*/
                                /* -DYNAMIC_ICIC */
                            }
                        }
                    }
                    else
                    {
                        /* UE is already considered so unable to process Node
                           hence freeing it since its already considered
                           So free the node */
                        FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                        /* +DYNAMIC_ICIC */
                        explicitDCIULAndPHRQueueCount--;
                        /* -DYNAMIC_ICIC */
                        continue;
                    }
                }
                FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p); 
                /* +DYNAMIC_ICIC */
                explicitDCIULAndPHRQueueCount--;
                /* -DYNAMIC_ICIC */
                continue;

            }
            else
            {
                FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                /* +DYNAMIC_ICIC */
                explicitDCIULAndPHRQueueCount--;
                /* -DYNAMIC_ICIC */
                continue;
            }
        }
        else
        {
            break;
        }
    }//end While loop

    /* EICIC +*/
    /* EICIC Optimization +- */

    /* Check if count is still left , then pop nodes and push them back to maintiain same UE priority for next cycle */
    if((isEicicEnabled_g[internalCellIndex]) )
    {
	    if ((explicitDCIULAndPHRQueueCount) && (INVALID_UE_TYPE != ueTypeToScheduleUL_g[internalCellIndex]) &&  (isTraverseList))
    {
	    while(explicitDCIULAndPHRQueueCount--) 
	    {
	    
        		DEQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
            		explicitULDCIAndPHRScheduleQueue_p,ExplicitULDCIAndPHRNode,(void *)&nodeScan_p);
		        
		        if( PNULL != nodeScan_p )
		        {
	                        if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
	                                   explicitULDCIAndPHRScheduleQueue_p,
	                                    ExplicitULDCIAndPHRNode,(void *)&(nodeScan_p->nodeAnchor))))
	                        {
	                                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
	                                        LOGERROR,MAC_UL_Strategy,\
	                                        currentGlobalTTITickCount,\
	                                        nodeScan_p->ueIndex,\
	                                        0,0,0,0,0.0,0.0,__func__,
	                                        "Error in Pushing node in\
	                                        explicitULDCIAndPHRScheduleQueue_p");

	                                FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(nodeScan_p);
	                        }
	            } 
	        }
    }
    }
	/*EICIC -*/
    /* EICIC Optimization +- */

    *maxUEsToBeScheduledUL_p = tempMaxUEsToBeScheduledUL ;
}

#ifdef OVERSHOOT_CORRECTION
/**********************************************************************************
 * Function Name  : modifiedNoOfUEsAfterOverLoadUL
 * Inputs         : modifiedUEs_p - Modified UE's,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function will modify the number of UE's in a tick in 
 *                  case of OVERSHOOT based on the overload level
 *********************************************************************************/
void modifiedNoOfUEsAfterOverLoadUL(UInt8  *modifiedUEs_p,
                                     InternalCellIndex  internalCellIndex)
{

    ULSchedulerStrategyConfig *ulSchdConfig_p = & cellSpecificParams_g.\
                 cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig;
    /** CA-TDD Changes Start **/             
    switch(macOverLoadLevel_g[internalCellIndex])
    /** CA-TDD Changes Stop **/
    {
        case OVERLOAD_LEVEL_NORMAL:
        {
            break;
        }
        case OVERLOAD_LEVEL_MEDIUM:
        {
            /* SPR 18122 Changes Start*/
            *modifiedUEs_p = DIVIDE_BY_FOUR(ulSchdConfig_p->maxUEsToBeScheduledUL*3);
            /* SPR 18122 Changes End*/
            LOG_MAC_MSG(MAC_UL_MODIFY_UE_ID,\
                    LOGWARNING,MAC_UL_Strategy,\
                    getCurrentTick(),\
                    *modifiedUEs_p,\
                    0,0,0,0,0.0,0.0,__func__,
                    "MEDIUM_OVERLOAD_LEVEL\
                    set");
            break;
        }
        case OVERLOAD_LEVEL_HIGH:
        {
            /* SPR 18122 Changes Start*/
            *modifiedUEs_p = DIVIDE_BY_TWO(ulSchdConfig_p->maxUEsToBeScheduledUL);
            /* SPR 18122 Changes End*/
            LOG_MAC_MSG(MAC_UL_MODIFY_UE_ID,\
                    LOGWARNING,MAC_UL_Strategy,\
                    getCurrentTick(),\
                    *modifiedUEs_p,\
                    0,0,0,0,0.0,0.0,__func__,
                    "HIGH_OVERLOAD_LEVEL\
                    set");
            break;
        }
        case OVERLOAD_LEVEL_CRITICAL:
        {
            /* SPR 18122 Changes Start*/
            *modifiedUEs_p = DIVIDE_BY_FOUR(ulSchdConfig_p->maxUEsToBeScheduledUL);
            /* SPR 18122 Changes End*/
            LOG_MAC_MSG(MAC_UL_MODIFY_UE_ID,\
                    LOGWARNING,MAC_UL_Strategy,\
                    getCurrentTick(),\
                    *modifiedUEs_p,\
                    0,0,0,0,0.0,0.0,__func__,
                    "CRITICAL_OVERLOAD_LEVEL\
                    set");
            break;
        }
        default:
        break;
    }
    /*+ Dyn UEs To Be SchPerTTI +*/
#ifndef DL_UL_SPLIT
    ulUeSchdIndxInCurrTick_g = *modifiedUEs_p; 
#endif
    getIndexForReqElProcessingCycles();
    /*- Dyn UEs To Be SchPerTTI -*/
}
/* CA Changes end */
/* +- SPR 17777 */

#endif

/* + CQI_4.1 */
/****************************************************************************
 * Function Name  : determineRBsForAperiodicCQIReports
 * Inputs         : ulUEContextInfo_p - Pointer to ULUEContext,
 *                  minDataSize_p - Minimum Data Size, 
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : minRequiredRBs
 * Description    : This API fills the Aperiodic Mode minimum size in bytes
 *                  and min RBs required for corresponding Aperiodic Mode. 
 ****************************************************************************/
UInt8 determineRBsForAperiodicCQIReports(ULUEContext *ulUEContext_p,
                                         UInt32 *minDataSize_p,
                                         /*CA Changes start */
                                         InternalCellIndex internalCellIndex)
                                         /*CA Changes end */

{
    /*  CSI_P2_PUSCH_CHANGES start */
 /* Coverity 32355 Fix Start */
    CQIMode cqiMode = CQI_MODE_INVALID;
    UInt8 ri = 0;
    if(ulUEContext_p->internalCellIndex == internalCellIndex)
    {
        /* Pcell */
        cqiMode = ulUEContext_p->pucchConfigInfo.cqiPmiRiInfoAperiodic.cqiMode;
        /* + CQI_5.0 */
        ri = ulUEContext_p->pucchConfigInfo.ueReportedRI;
        /* - CQI_5.0 */
    }
    else
    {
        cqiMode = ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->scellpucchConfigInfo.
            cqiPmiRiInfoAperiodic.cqiMode;
        /* + CQI_5.0 */
        ri = ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->scellpucchConfigInfo.ueReportedRI;
        /* - CQI_5.0 */
    }
    /*  CSI_P2_PUSCH_CHANGES end */

    AperiodicModeMinRBInfo *tempAperiodicModesRBInfo_p = aperiodicModesRBInfo_gp[internalCellIndex];
    UInt8 minRequiredRBs = 0;

    /*CA Changes start */
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
       cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*CA Changes end */

    switch(cqiMode)
    {
        case CQI_MODE_1_2:
        /* 4x2 MIMO S */
        if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
        {
            if (1 == ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_12_RI_1_FOR_4_ANT_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1;
            }
            else if (1 < ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_12_RI_GT_1_FOR_4_ANT_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2;
            }
        }
        else
        {     
            if (1 == ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_12_RI_1_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1;
            }
            else if (2 == ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_12_RI_2_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2;
            }
        }
        /* 4x2 MIMO E */
        break;

        case CQI_MODE_2_0:
        *minDataSize_p = CEIL_DIV(APERIODIC_MODE_20_BITS(cellParams_p),8);
        minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode20;
        break;

        case CQI_MODE_2_2:
        /* 4x2 MIMO S */
        if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
        {
            if (1 == ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_22_RI_1_FOR_4_ANT_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1;
            }
            else if (1 < ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_22_RI_GT_1_FOR_4_ANT_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2;
            }
        }
        else 
        {     
            if (1 == ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_22_RI_1_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1;
            }
            else if (2 == ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_22_RI_2_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2;
            }
        }
        /* 4x2 MIMO E */
        break;

        case CQI_MODE_3_0:
        *minDataSize_p = CEIL_DIV(APERIODIC_MODE_30_BITS(cellParams_p),8);
        minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode30;
        break;

        case CQI_MODE_3_1:
        /* 4x2 MIMO S */
        if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
        {
            if (1 == ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_31_RI_1_FOR_4_ANT_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1;
            }
            else if (1 < ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_31_RI_GT_1_FOR_4_ANT_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2;
            }    
        }
        else
        {     
            if (1 == ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_31_RI_1_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1;
            }
            else if (2 == ri)
            {
                *minDataSize_p = CEIL_DIV(APERIODIC_MODE_31_RI_2_BITS(cellParams_p),8);
                minRequiredRBs = tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2;
            }
        }
        /* 4x2 MIMO E */
        break;

        default:
        {
            lteWarning("INVALID_APERIODIC_CQI_MODE\n");
        }
        break;
    }

    return minRequiredRBs; 
}

/****************************************************************************
 * Function Name  : fillMinRBsForAperiodicModes
 * Inputs         : dlAvailableRBs - Total available RB for DL,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This API fills the min RBs for configured Aperiodic Mode.
 ****************************************************************************/
void fillMinRBsForAperiodicModes(UInt8 dlAvailableRBs,
                       InternalCellIndex internalCellIndex)
{
    AperiodicModeMinRBInfo *tempAperiodicModesRBInfo_p = aperiodicModesRBInfo_gp[internalCellIndex];
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    switch(dlAvailableRBs)
    {
        case 100:
        {   
            /* 4x2 MIMO S */
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1 = MIN_RB_APERIODIC_MODE_12_RI_1_20_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2 = MIN_RB_APERIODIC_MODE_12_RI_GT_1_20_MH_4_ANT;
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1 = MIN_RB_APERIODIC_MODE_22_RI_1_20_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2 = MIN_RB_APERIODIC_MODE_22_RI_GT_1_20_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1 = MIN_RB_APERIODIC_MODE_31_RI_1_20_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2 = MIN_RB_APERIODIC_MODE_31_RI_GT_1_20_MH_4_ANT; 
            }
            else
            {
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1 = MIN_RB_APERIODIC_MODE_12_RI_1_20_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2 = MIN_RB_APERIODIC_MODE_12_RI_2_20_MHz;
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1 = MIN_RB_APERIODIC_MODE_22_RI_1_20_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2 = MIN_RB_APERIODIC_MODE_22_RI_2_20_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1 = MIN_RB_APERIODIC_MODE_31_RI_1_20_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2 = MIN_RB_APERIODIC_MODE_31_RI_2_20_MHz; 
            }
            /* 4x2 MIMO E */
            tempAperiodicModesRBInfo_p->minRBsForAperiodicMode20 = MIN_RB_APERIODIC_MODE_20_20_MHz; 
            tempAperiodicModesRBInfo_p->minRBsForAperiodicMode30 = MIN_RB_APERIODIC_MODE_30_20_MHz;  
        }
        break;
        case 75:
        {
            /* 4x2 MIMO S */
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1 = MIN_RB_APERIODIC_MODE_12_RI_1_15_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2 = MIN_RB_APERIODIC_MODE_12_RI_GT_1_15_MH_4_ANT;
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1 = MIN_RB_APERIODIC_MODE_22_RI_1_15_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2 = MIN_RB_APERIODIC_MODE_22_RI_GT_1_15_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1 = MIN_RB_APERIODIC_MODE_31_RI_1_15_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2 = MIN_RB_APERIODIC_MODE_31_RI_GT_1_15_MH_4_ANT; 
            }
            else
            {
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1 = MIN_RB_APERIODIC_MODE_12_RI_1_15_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2 = MIN_RB_APERIODIC_MODE_12_RI_2_15_MHz;
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1 = MIN_RB_APERIODIC_MODE_22_RI_1_15_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2 = MIN_RB_APERIODIC_MODE_22_RI_2_15_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1 = MIN_RB_APERIODIC_MODE_31_RI_1_15_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2 = MIN_RB_APERIODIC_MODE_31_RI_2_15_MHz; 
            }
            /* 4x2 MIMO E */
            tempAperiodicModesRBInfo_p->minRBsForAperiodicMode20 = MIN_RB_APERIODIC_MODE_20_15_MHz; 
            tempAperiodicModesRBInfo_p->minRBsForAperiodicMode30 = MIN_RB_APERIODIC_MODE_30_15_MHz;  
        }
        break;
        case 50:
        {
            /* 4x2 MIMO S */
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1 = MIN_RB_APERIODIC_MODE_12_RI_1_10_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2 = MIN_RB_APERIODIC_MODE_12_RI_GT_1_10_MH_4_ANT;
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1 = MIN_RB_APERIODIC_MODE_22_RI_1_10_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2 = MIN_RB_APERIODIC_MODE_22_RI_GT_1_10_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1 = MIN_RB_APERIODIC_MODE_31_RI_1_10_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2 = MIN_RB_APERIODIC_MODE_31_RI_GT_1_10_MH_4_ANT; 
            }
            else
            {
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1 = MIN_RB_APERIODIC_MODE_12_RI_1_10_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2 = MIN_RB_APERIODIC_MODE_12_RI_2_10_MHz;
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1 = MIN_RB_APERIODIC_MODE_22_RI_1_10_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2 = MIN_RB_APERIODIC_MODE_22_RI_2_10_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1 = MIN_RB_APERIODIC_MODE_31_RI_1_10_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2 = MIN_RB_APERIODIC_MODE_31_RI_2_10_MHz; 
            }
            /* 4x2 MIMO E */
            tempAperiodicModesRBInfo_p->minRBsForAperiodicMode20 = MIN_RB_APERIODIC_MODE_20_10_MHz; 
            tempAperiodicModesRBInfo_p->minRBsForAperiodicMode30 = MIN_RB_APERIODIC_MODE_30_10_MHz;  
        }
        break;
        case 25:
        {
            /* 4x2 MIMO S */
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1 = MIN_RB_APERIODIC_MODE_12_RI_1_5_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2 = MIN_RB_APERIODIC_MODE_12_RI_GT_1_5_MH_4_ANT;
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1 = MIN_RB_APERIODIC_MODE_22_RI_1_5_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2 = MIN_RB_APERIODIC_MODE_22_RI_GT_1_5_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1 = MIN_RB_APERIODIC_MODE_31_RI_1_5_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2 = MIN_RB_APERIODIC_MODE_31_RI_GT_1_5_MH_4_ANT; 
            }
            else
            {
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1 = MIN_RB_APERIODIC_MODE_12_RI_1_5_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2 = MIN_RB_APERIODIC_MODE_12_RI_2_5_MHz;
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1 = MIN_RB_APERIODIC_MODE_22_RI_1_5_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2 = MIN_RB_APERIODIC_MODE_22_RI_2_5_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1 = MIN_RB_APERIODIC_MODE_31_RI_1_5_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2 = MIN_RB_APERIODIC_MODE_31_RI_2_5_MHz; 
            }
            /* 4x2 MIMO E */
            tempAperiodicModesRBInfo_p->minRBsForAperiodicMode20 = MIN_RB_APERIODIC_MODE_20_5_MHz; 
            tempAperiodicModesRBInfo_p->minRBsForAperiodicMode30 = MIN_RB_APERIODIC_MODE_30_5_MHz;  
        }
        break;
        case 15:
        {
            /* 4x2 MIMO S */
            if (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas)
            {
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1 = MIN_RB_APERIODIC_MODE_12_RI_1_3_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2 = MIN_RB_APERIODIC_MODE_12_RI_GT_1_3_MH_4_ANT;
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1 = MIN_RB_APERIODIC_MODE_22_RI_1_3_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2 = MIN_RB_APERIODIC_MODE_22_RI_GT_1_3_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1 = MIN_RB_APERIODIC_MODE_31_RI_1_3_MH_4_ANT; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2 = MIN_RB_APERIODIC_MODE_31_RI_GT_1_3_MH_4_ANT; 
            }
            else
            {
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI1 = MIN_RB_APERIODIC_MODE_12_RI_1_3_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode12RI2 = MIN_RB_APERIODIC_MODE_12_RI_2_3_MHz;
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI1 = MIN_RB_APERIODIC_MODE_22_RI_1_3_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode22RI2 = MIN_RB_APERIODIC_MODE_22_RI_2_3_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI1 = MIN_RB_APERIODIC_MODE_31_RI_1_3_MHz; 
                tempAperiodicModesRBInfo_p->minRBsForAperiodicMode31RI2 = MIN_RB_APERIODIC_MODE_31_RI_2_3_MHz; 
            }
            /* 4x2 MIMO E */
            tempAperiodicModesRBInfo_p->minRBsForAperiodicMode20 = MIN_RB_APERIODIC_MODE_20_3_MHz; 
            tempAperiodicModesRBInfo_p->minRBsForAperiodicMode30 = MIN_RB_APERIODIC_MODE_30_3_MHz;  
        }
        break;
        default:
        {
            lteWarning("DL Bandwidth less than 3 MHz\n");
        }
        break;
    }
}
/* - CQI_4.1 */
/* SRP 3608 changes start */
/*******************************************************************************************
 * Function Name  : processULPriorityQueue
 * Inputs         : priorityGrantQueueCount - Its the total Nodes present in 
 *                                            globalSrQueue, which needs to
 *                                            be processed,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  maxUEsToBeScheduledUL_p - pointer to the max UE to be
 *                                            scheduled in UL,
 *                  currentGlobalTTITickCount - Current Global Tick
 *                  ulDelay - The amount of delay expected in 
 *                            the UpLink path,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : Allocates RBs to SR, It will allocate RBs to SR Queue Nodes & 
 *                  pass the UL Strategy  in the PDCCH module
 * Returns        : None
 * Description    : This function will process on the Scheduling requests in 
 *                  the global SR Queue. Prepare the response and allocation 
 *                  of UL resources for long BSR and send to ulStrategyTXQueue_g.
 *********************************************************************************************/
/*SPR 22919 Start*/
 void processULPriorityQueue(
/*SPR 22919 End*/
        UInt32 priorityGrantQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex
        /*CA Changes end  */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
{
    /* +- SPR 17777 */
    /* +- SPR 18490 */
    ULGrantRequestQueueNode *srNode_p   = PNULL;
    UInt32 tempUeId                     = 0;
    UInt32 dataSize                     = 0;
    ULStrategyTxNode *ulStrategyTxNode_p= PNULL;
    ULUEContext * tempUplinkContext_p   = PNULL;
    MeasGapContextUL * measGapContext_p   = PNULL;
    ULUEContextInfo * tempUplinkContextInfo_p = PNULL;
    UInt8 tempMaxUEsToBeScheduledUL    = *maxUEsToBeScheduledUL_p;
    UInt8 avgMcs                        = 0;
    DLUEContext *dlUEContext_p  = PNULL;  
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    UInt32 usedRB                       = 0;
    UInt32 minDataSize                  = 0;
    UInt32 maxDataSize                  = 0;
    UInt32 maxULRBWithPriority = 0;
    UInt32 maxAvailableRBs     = 0;
    UInt8 userLocationType     = CC_USER;


    /*Cyclomatic Complexity changes - starts here */
    MacRetType retType = MAC_SUCCESS;
    /*Cyclomatic Complexity changes - ends here */
    /* ATB Changes Start */
    UInt8 mcsVal = 0;
    /* ATB Changes End   */
    
#ifdef FDD_CONFIG
    UInt8 ttiMapId1= TTI_MAP_FREE;
    UInt8 ttiMapId2= TTI_MAP_FREE;
#endif




    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Entry:[UL-Strategy][%s]",
            __func__);

    /*Processing for Queue Count and till the Available RBs*/
    /* ULA_CHG */
    /*SPR 22946 Fix Start*/
    while ((priorityGrantQueueCount--)&&(numberOfAvailableRBs_p[CC_CE_REGION] != 0) 
    /*SPR 22946 Fix End*/
            &&(tempMaxUEsToBeScheduledUL))
        /* ULA_CHG */
    {
        /*CA Changes start  */
        srNode_p =  (ULGrantRequestQueueNode *)popNode (ulHighPriorityGrantQueue_gp[internalCellIndex]);
        /*CA Changes end  */
        if (srNode_p)
        {
            tempUeId = srNode_p->ueIndex;
            tempUplinkContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
            dlUEContextInfo_p = &dlUECtxInfoArr_g[tempUeId];
            dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

            /*Checking for UE context */
            if (!(tempUplinkContextInfo_p->pendingDeleteFlag))
            {
                tempUplinkContext_p = tempUplinkContextInfo_p->ulUEContext_p;
                if(PNULL !=  tempUplinkContext_p)
                {


			    /* EICIC +*/
			if(isEicicEnabled_g[internalCellIndex])
			{
			    /* Pushing UE back in trigger queue.*/    	    
				/*Complexity Start*/
			    if (((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == tempUplinkContext_p->userType))
					||((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == tempUplinkContext_p->userType)))
			    {              
				    if(pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],
							    &(srNode_p->ulGrantRequestNodeAnchor)))
				    {
					    freeMemPool((void *)srNode_p);
					    srNode_p = PNULL;
				    }
                        /*SPR 22946 Fix Start*/
                         /*code removed*/
                        /*SPR 22946 Fix End*/
				    continue;
			    }

			}
/*Complexity End*/
			    /* EICIC - */
                /* EICIC Optimization +- */
                    /* Coverity CID 54672 Start */ 
                userLocationType    = tempUplinkContext_p->userLocationType;
                    /* Coverity CID 54672 End */ 
                    /* \code
                     * if schType == SPS_SCHEDULING, then do not schedule 
                     * this UE.
                     * */
                    if( SPS_SCHEDULING == tempUplinkContext_p->schType )
                    {
                        /*CA Changes start  */
                        if( pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],
                        /*CA Changes end  */
                                    &(srNode_p->ulGrantRequestNodeAnchor)) )
                        {
                            freeMemPool((void *)srNode_p);
                            srNode_p = PNULL;
                        }
                        /*SPR 22946 Fix Start*/
                         /*code removed*/
                        /*SPR 22946 Fix End*/
                        continue;
                    }
                    /*SR starvation fix start */
                    tempUplinkContext_p->srNode_p = PNULL;
                    /*SR starvation fix end */
                    measGapContext_p = &tempUplinkContext_p->measGapContext;
#ifdef TDD_CONFIG                
                    UInt8 ulHarqProcessId = BUSY_ALL_HARQ;
                    /* TDD Config 0 and 6 Changes Start */
                    /* SPR 23393 Changes Start */
                    UInt32 sysFrameNum = 0;
                    UInt32 subframe     = 0;
                    /* +- SPR 17777 */
                    GET_SFN_SF_FROM_TTI((currentGlobalTTITickCount + ulDelay), sysFrameNum, subframe)
            /* SPR 23393 Changes End */
                        LTE_GCC_UNUSED_PARAM(sysFrameNum)
                    /* SPR 11257 Changes Start*/
    		    ulHarqProcessId = getFreeULHarqId(tempUplinkContext_p,subframe,internalCellIndex,
				            ulSubFrameNum,currentGlobalTTITickCount);
                    /* +- SPR 17777 */
                    /* SPR 11257 Changes End*/
                    /* TDD Config 0 and 6 Changes End*/
#elif FDD_CONFIG
                    UInt8 ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + 
                            ulDelay + FDD_HARQ_OFFSET);
                if ( tempUplinkContext_p->ttiBundlingEnabled )
                {
                    ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
                }
                ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
                ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET + 
                MAX_HARQ_PROCESS_NB);
#endif           
#ifdef TDD_CONFIG                     
                    if ( BUSY_ALL_HARQ == ulHarqProcessId)
#elif FDD_CONFIG
                        if((HARQ_PROCESS_FREE != tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                                ||(tempUplinkContext_p->ttiBundlingEnabled &&
                                    (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,tempUplinkContext_p)))
                                ||(!(tempUplinkContext_p->ttiBundlingEnabled)&&
                                    ((TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId1]) ||
                                     ( TTI_MAP_FREE != tempUplinkContext_p->ttiHarqMap[ttiMapId2])))
                                /*SPR 7944 START*/
                                /* + SPS_TDD_Changes */
                                /*
                                   ||checkSpsOccasionTti(tempUplinkContext_p,ulHarqProcessId)
                                   ||checkSpsActTti(tempUplinkContext_p,ulHarqProcessId)
                                 */
                                /* - SPS_TDD_Changes */
                                /*SPR 7944 END*/
                          )
#endif                         
                        {
#ifdef FDD_CONFIG                        
                            if(HARQ_PROCESS_RESET == tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                            {
                                freeMemPool((void *)srNode_p);
                                srNode_p = PNULL;                             
                            }
                            else
#endif                            
                            {
                                /*CA Changes start  */
                                if(pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],
                                         &(srNode_p->ulGrantRequestNodeAnchor)))
                                {
                                   freeMemPool(srNode_p);
                                   srNode_p = PNULL;
                                }   
                                /*CA Changes end  */
                            }
			    /* SPR 13196 fix start */ 
                            /*SPR 22946 Fix Start*/
                            /*code removed*/
                            /*SPR 22946 Fix End*/
			    /* SPR 13196 fix end */ 
                            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                                "[%s]:UE ID %d is not schedule due to busy HARQ"\
                                " process Global Tick:%d\n",
                                __func__,tempUeId,currentGlobalTTITickCount);
                            continue;   
                        }

                    /* HD FDD  Changes Start */
#ifdef HD_FDD_CONFIG

                    /** check if this UE is HD FDD UE **/
                    /* If HD FDD UE is in   UL priority  Q,then check Schedule map whether
                     * it can be scheduled in currenttick or not*/
                    if(tempUplinkContext_p->isHDFddFlag)
                    {
                        if (!CAN_SCHEDULE_UL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY, \
                                    tempUeId , hdUeContextInfo_g.hdfddUeSchedMap ))
                        {
                           /*CA Changes start  */
                           if(pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],
                           /*CA Changes end  */
                                  &(srNode_p->ulGrantRequestNodeAnchor)))
                           {
                              freeMemPool(srNode_p);
                              srNode_p = PNULL;
                           }

                            /*SPR 22946 Fix Start*/
                            /*code removed*/
                            /*SPR 22946 Fix End*/
                            /* Increment the statistics counter UE denied UL resources due to
                             * DL clash in the same TTI of HD FDD UE*/
                            UPDATE_UE_DENIED_UL_RESOURCE(tempUeId);
                            continue;
                        }
                    }
#endif                            
                    /* HD FDD  Changes End */

                    /*Checking if Already being considered for RB Distribution*/
                    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                    if (tempUplinkContext_p->isAlreadyConsideredFlag !=  ulReferenceFlag_g[internalCellIndex]  )
#elif TDD_CONFIG
                    if (tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum] != 
                         ulReferenceFlag_g[internalCellIndex]  )
#endif
                    /* TDD Config 0 Changes End */
                    {
                        if(srNode_p->ulGrantRequestType == SR_ULGRANT_REQ_MSG)
                        {
                            LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                                    "[UL-Strategy][%s]SR Request Received:SR Queue Cnt:%d \
                                    Glbl TTI:%d,PhyTTi:%d,SR Periodicity:%d ,ulDelay %d \n"
                                    ,__func__ ,priorityGrantQueueCount,
                                    currentGlobalTTITickCount, srNode_p->ttiCounter,
                                    tempUplinkContext_p->pucchConfigInfo.srPeriodicity, 
                                    ulDelay);

                            /*Cyclomatic Complexity changes - starts here */
                            retType = processSRInULPriorityQueue(tempUplinkContext_p,
                                    currentGlobalTTITickCount,
                                    srNode_p,
                                    ulDelay,
                                    /*SPR 22946 Fix Start*/
                                    /*code removed*/
                                    /*SPR 22946 Fix End*/
                                    numberOfAvailableRBs_p,
                                    &tempMaxUEsToBeScheduledUL
#ifdef TDD_CONFIG
                                    ,ulHarqProcessId
                                    /* TDD Config 0 Changes Start */
                                    ,ulSubFrameNum
                                    /* TDD Config 0 Changes End */
#endif
                                    ,internalCellIndex
                                    );
                            
                            if(MAC_FAILURE == retType)
                            {
                                continue;
                            }

                            /*Cyclomatic Complexity changes - ends here */

                        }
                        /* Process Non Zero BSR Request for SRB Data */
                        else if ((DRX_STATE_ON != tempUplinkContext_p->drxCurrentState) && 
                                (TRUE != measGapContext_p->isMeasGapPeriodOn) && 
                                (UL_SYNC_LOSS_OFF ==
                                 dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState) 
                                /*SPR 5060 Start */
                                /*&& (tempUplinkContext_p->bsrNetTotal)*/ )
                            /*SPR 5060 Start */
                        {

                            /*CA Changes start  */
                            if ((CE_USER == userLocationType) && (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]))
                            {
                                if(pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],&(srNode_p
                                            ->ulGrantRequestNodeAnchor)))
                                {
                                   freeMemPool(srNode_p);
                                   srNode_p = PNULL;
                                }   
                                tempMaxUEsToBeScheduledUL--;
                                    /*SPR 22946 Fix Start*/
                                    /*code removed*/
                                    /*SPR 22946 Fix End*/
                                continue;
                            }
                            /*CA Changes end  */

                            avgMcs = tempUplinkContext_p->mcsIndex;
                            dataSize = tempUplinkContext_p->bsrNetTotal + MAC_HEADER_SIZE_IN_BYTES(ZERO,ONE);
                            /* SPR 5999 changes start */
                            /*CA Changes start  */
                            maxULRBWithPriority = DETERMINE_RBs_UL(avgMcs, dataSize, internalCellIndex);
                            /*CA Changes end  */
                            /* SPR 5999 changes end */

                            LTE_MAC_UT_LOG(LOG_DETAIL,DL_RR_STRATEGY,
                                    "[DL-Strategy][%s]Total RBs to be distribute per UE :%d "
                                    "in DL Priority Queue Processing",__func__,maxULRBWithPriority);

                            /* Process Non Zero BSR Request */
                            UInt32 strictAllocRBFlag = FALSE;
                            usedRB = validateUplinkRB(tempUplinkContext_p ,
                                    maxULRBWithPriority,
                                    numberOfAvailableRBs_p[userLocationType],
                                    &strictAllocRBFlag,&minDataSize,&maxDataSize,
                                    /* ATB Changes Start */
                                    &mcsVal,
                                    /*SPR 16417 Changes Start*/
#ifdef FDD_CONFIG
                                    /* SPR 13532 Fix Start */
                                    (currentGlobalTTITickCount + ulDelay),
                                    /* SPR 13532 Fix End */
#elif TDD_CONFIG
                                    currentGlobalTTITickCount,
                                    ulSubFrameNum,
#endif
                                    /*SPR 16417 Changes End*/
                                    internalCellIndex);
                            /* ATB Changes End */
                            if (usedRB)
                            {
                                ulStrategyTxNode_p = PNULL;
                                /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                                GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
                                GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                                sizeof(ULStrategyTxNode),PNULL);
#endif

                                /*coverity-530 CID-10758*/
                                if(PNULL == ulStrategyTxNode_p)
                                {
                                    /*CA Changes start  */
                                    if (pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],
                                    /*CA Changes end  */
                                                &(srNode_p->ulGrantRequestNodeAnchor)))
                                    { 
                                        lteWarning("Error in Pushing SR in"
                                                "ulHighPriorityQueue in"
                                                " funtion %s\n",__func__);
                                        freeMemPool((void *)srNode_p);
                                        srNode_p = PNULL;
                                    }
                                    tempMaxUEsToBeScheduledUL--;
                                        /*SPR 22946 Fix Start*/
                                        /*code removed*/
                                        /*SPR 22946 Fix End*/
                                    /*++530-coverity-10758*/
                                    continue;
                                    /*++530-coverity-10758*/
                                }
                                ulStrategyTxNode_p->requiredRB = 0;
                                ulStrategyTxNode_p->maxDataSize = 0;
                                ulStrategyTxNode_p->minDataSize = 0;

                                /*coverity-530 CID-10758*/
                                ulStrategyTxNode_p->ueIndex = (UInt16)tempUeId;
                                ulStrategyTxNode_p->requiredRB = usedRB;
                                /* ICIC changes start */
                                maxAvailableRBs = numberOfAvailableRBs_p[userLocationType];
                                numberOfAvailableRBs_p[userLocationType] -= usedRB;

                                /* ATB Changes Start */
                                avgMcs = mcsVal;
                                /* ATB Changes End */

                                if (userLocationType == CE_USER)
                                {
                                    /*CA Changes start  */
                                    maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]--;
                                    /*CA Changes end  */
                                    numberOfAvailableRBs_p[CC_CE_REGION] -= usedRB;
                                }
                                else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                                        numberOfAvailableRBs_p[CE_REGION])
                                {
                                    numberOfAvailableRBs_p[CE_REGION] = 
                                        numberOfAvailableRBs_p[CC_CE_REGION];
                                }
                                /* ICIC changes end */
                                ulStrategyTxNode_p->strictAllocRB = strictAllocRBFlag;
                                ulStrategyTxNode_p->ulGrantRequestType =
                                    NEW_ULGRANT_REQ_MSG;
                                ulStrategyTxNode_p->ttiCounter = currentGlobalTTITickCount;
#ifdef TDD_CONFIG                            
                                ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessId;
                                /*TDD Config 0 Changes Start*/
                                ulStrategyTxNode_p->scheduledCount = 0;
                                /*TDD Config 0 Changes End*/
#endif                            
                                ulStrategyTxNode_p->minDataSize = minDataSize;
                                ulStrategyTxNode_p->maxDataSize = maxDataSize; 
                                ulStrategyTxNode_p->avgMcs = avgMcs;

                                /*putEntry of ULStrategyTXNode in PDCCH Queue */
                                if (!(putEntryInULSchedulerNewTxQueue
                                            /*CA Changes start  */
                                            (ulStrategyTxNode_p, internalCellIndex
                                            /*CA Changes end  */
                                             /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                                             ,currentGlobalTTITickCount
                                             ,ulSubFrameNum
                                             ,tempUplinkContext_p
#endif
                                             /* TDD Config 0 Changes End */
                                            )))
                                {
                                    /* ULA_CHG */

                                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                            LOGERROR,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            ulStrategyTxNode_p->ueIndex,
#ifdef TDD_CONFIG 
                                            sysFrameNum,
#else
                                            0,
#endif
                                            0,0,0,0.0,0.0,__func__,
                                            /* +- SPR 17777 */
                                            "ulStrategyTxNode_p entry failed");
                                    /* ULA_CHG */
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                                        FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
                                        freeMemPool(ulStrategyTxNode_p);
#endif
                                        /*coverity_530_55392*/
                                        ulStrategyTxNode_p = PNULL;
                                        /*coverity_530_55392*/
                                }
                                else
                                {
                                    LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                                            LOGDEBUG,MAC_UL_Strategy,\
                                            currentGlobalTTITickCount,\
                                            ulStrategyTxNode_p->ueIndex,\
                                            ulStrategyTxNode_p->requiredRB,\
                                            ulStrategyTxNode_p->strictAllocRB,\
                                            ulStrategyTxNode_p->minDataSize,\
                                            ulStrategyTxNode_p->maxDataSize,\
                                            ulStrategyTxNode_p->avgMcs,\
                                            ulStrategyTxNode_p->ttiCounter,\
                                            __func__,"ulStrategyTxNode_p parameters");

                                    /* +COVERITY 5.0 */
                                    LOG_MAC_MSG (UL_PRIORITY_QUEUE_SCHEDULED_LOG_ID, LOGDEBUG,
                                            MAC_UL_Strategy, getCurrentTick (), tempUplinkContext_p->ueIndex,
                                            ulHarqProcessId,
                                            ulStrategyTxNode_p->requiredRB,
                                            tempUplinkContext_p->bsrNetTotal,
                                            ulStrategyTxNode_p->ulGrantRequestType,
                                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
                                    /* -COVERITY 5.0 */
                                }
                                tempUplinkContext_p->scheduleFlag = UL_QOS_ALREADY_TRANSMITED;
                            }
                            /* In case required RB's are not assigned or MAX_availableRB's are assigned 
                             *  * to the UE then it is assumed that it
                             *   * has some pending BSR so, putting it back into the queue */
                            if((maxULRBWithPriority < usedRB) || 
                                    (maxULRBWithPriority == maxAvailableRBs))
                            {
                                if(TRUE == tempUplinkContext_p->prioritySchedulingReqUL)
                                {
                                    /*CA Changes start  */
                                    if (pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],
                                                &(srNode_p->ulGrantRequestNodeAnchor)))
                                    /*CA Changes end  */ 
                                    { 
                                        lteWarning("Error in Pushing SR in"
                                                "ulHighPriorityQueue in"
                                                " funtion %s\n",__func__);
                                        /* +- SPR 21131 */
                                        /* ULA_CHG */
                                        freeMemPool((void *)srNode_p);
                                        srNode_p = PNULL;
                                    }
                                    tempMaxUEsToBeScheduledUL--;
                                        /*SPR 22946 Fix Start*/
                                        /*code removed*/
                                        /*SPR 22946 Fix End*/

                                }
                                else if (tempUplinkContext_p->nonZeroBsrListTempDeleted_p)
                                {
                                    listInsertNode((nonZeroBsrList_gp[internalCellIndex]), 
                                            tempUplinkContext_p->nonZeroBsrListTempDeleted_p);

                                    tempUplinkContext_p->nonZeroBsrList_p = 
                                        tempUplinkContext_p->nonZeroBsrListTempDeleted_p;
                                    tempUplinkContext_p->nonZeroBsrListTempDeleted_p = PNULL;
                                    tempMaxUEsToBeScheduledUL--;
                                        /*SPR 22946 Fix Start*/
                                        /*code removed*/
                                        /*SPR 22946 Fix End*/
                                    /* ULA_CHG */
                                    freeMemPool((void *)srNode_p); 
                                    srNode_p = PNULL;
                                }
                                else
                                {
                                    /*Cyclomatic Complexity changes - starts here */
                                    putEntryInBSRListForULPriorityQueue(tempUplinkContext_p,
                                            &tempMaxUEsToBeScheduledUL,
                                        /*SPR 22946 Fix Start*/
                                        /*code removed*/
                                        /*SPR 22946 Fix End*/
                                            srNode_p,
                                            internalCellIndex,
                                            execArgs
                                            ); 
                                    /*Cyclomatic Complexity changes - starts here */
                                    freeMemPool((void *)srNode_p); 
                                    srNode_p = PNULL;
                                }
                            }
                            else
                            {
                                /* UE Scheduled Successfully */
                                        /*SPR 22946 Fix Start*/
                                        /*code removed*/
                                        /*SPR 22946 Fix End*/
                                tempMaxUEsToBeScheduledUL--;
                                /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                                tempUplinkContext_p->isAlreadyConsideredFlag 
                                    =  ulReferenceFlag_g[internalCellIndex]  ;
#elif TDD_CONFIG
                                tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum] 
                                    =  ulReferenceFlag_g[internalCellIndex]  ;
#endif
                                /* TDD Config 0 Changes End */
                                /* ULA_CHG */
                                freeMemPool((void *)srNode_p); 
                                srNode_p = PNULL;
                            }
                        }
                        else
                        {
                            /*CA Changes start  */
                            /*cov warning 24377 fix start*/
                            if(pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],&(srNode_p
                                        ->ulGrantRequestNodeAnchor)))
                            {
                               freeMemPool(srNode_p);
                               srNode_p = PNULL;
                            }   
                            /*cov warning 24377 fix end*/
                            /*CA Changes end  */
                            tempMaxUEsToBeScheduledUL--;
                                /*SPR 22946 Fix Start*/
                                /*code removed*/
                                /*SPR 22946 Fix End*/
                            continue;   
                        }
                    }
                    else
                    {
                        /*UE is already considered so unable to process SR Node 
                         *                           hence Pushing into ulGrantPendingQueue */
                        tempMaxUEsToBeScheduledUL--;
                            /*SPR 22946 Fix Start*/
                            /*code removed*/
                            /*SPR 22946 Fix End*/
                        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"
                                "[%s]:UE is already considered, hence NOT \
                                considering UE",
                                __func__,tempUeId);
                        /*Freeing the SR Node*/
                        freeMemPool((void *)srNode_p);
                        srNode_p = PNULL;
                    }
                    if(ulStrategyTxNode_p)
                    {
                        LOG_MAC_MSG (UL_PRIORITY_QUEUE_SCHEDULED_LOG_ID, LOGDEBUG,
                                MAC_UL_Strategy, getCurrentTick (), tempUplinkContext_p->ueIndex,
                                ulHarqProcessId,
                                ulStrategyTxNode_p->requiredRB,
                                tempUplinkContext_p->bsrNetTotal,
                                ulStrategyTxNode_p->ulGrantRequestType,
                                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
                    }

                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[%s] UEID %d is NOT CREATED\n",__func__,tempUeId);                
                    LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,"[UL-Strategy][%s]"
                            "UE ID %d's Context either deleted or is to be deleted.\n",
                            __func__,tempUeId);                
                    /*Freeing the SR Node*/
                    freeMemPool((void *)srNode_p);
                    srNode_p = PNULL;
		    /* SPR 13196 fix start */ 
                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
		    /* SPR 13196 fix end */ 
                }
            }        
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,"[UL-Strategy][%s]"
                        "UE ID %d's Context either deleted or is to be deleted.\n",
                        __func__,tempUeId);                
                /*Freeing the SR Node*/
                freeMemPool((void *)srNode_p);
                srNode_p = PNULL;
		/* SPR 13196 fix start */ 
                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
		/* SPR 13196 fix end */ 
            }
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                    "[UL-Strategy][%s] srNode not valid...\n",__func__);
	    /* SPR 13196 fix start */ 
                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
	    /* SPR 13196 fix end */ 
            break;
        }
    }//end While loop

    *maxUEsToBeScheduledUL_p = tempMaxUEsToBeScheduledUL ;
    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]",
            __func__);
}
/* SRP 3608 changes end */



/*****************************************************************************
 * Function Name  : processDummyDCI0List
 * Inputs         : dummyDCI0ListRequiredList_g - List to be processed,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  maxUEsToBeScheduledUL_p - pointer to the max UE to be 
 *                                            scheduled in UL,
 *                  currentGlobalTTITickCount-Current Global Tick,
 *                  ulDelay - UL delay ,
 *                  internalCellIndex - cell index used at MAC.
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : Process UE and make strategy node
 * Returns        : None
 * Description    : This function will process the E_CID dummy DCI0 list.
 *****************************************************************************/
void processDummyDCI0List (
	LTE_LIST *dummyDCI0ListRequiredList_p,
	UInt32 *numberOfAvailableRBs_p,
	UInt8 *maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
	tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
	,UInt32 ulDelay
#endif
    ,InternalCellIndex internalCellIndex
    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    ,UInt8 ulSubFrameNum
#endif
    /* TDD Config 0 Changes End */
	)
{
    /* +- SPR 17777 */


    UInt32 tempUeId                     = 0;
    ULStrategyTxNode *ulStrategyTxNode_p= PNULL;
    ULUEContext * tempUplinkContext_p   = PNULL;
    ULUEContextInfo * tempUplinkContextInfo_p = PNULL;
    MeasGapContextUL * measGapContext_p   = PNULL;
    UInt8 tempMaxUEsToBeScheduledUL    = *maxUEsToBeScheduledUL_p;
    ULUeScheduleInfo *ulUeScheduleInfo_p = PNULL;
    UInt8 avgMcs                                    = 0;
    /* SPR 15909 fix start */
    tickType_t globalTick = currentGlobalTTITickCount;
    /* SPR 15909 fix end */
    /* ICIC Change start */
    UInt8 userLocationType = CC_USER ;
    /* ICIC Change End */
    DummyDCI0Node *node_p = PNULL;
    DummyDCI0Node *nextNode_p = PNULL;

    node_p =(DummyDCI0Node *) getFirstListNode(dummyDCI0ListRequiredList_p);
    /*Processing for Queue Count and till the Available RBs*/
    /* ICIC changes start */
    /* +DYNAMIC_ICIC */
    while ((numberOfAvailableRBs_p[CC_CE_REGION]) && (tempMaxUEsToBeScheduledUL))
	/* -DYNAMIC_ICIC */
	/* ICIC changes end */
    {
        if (node_p)
        {
            /* Current tick is less than the dummyDCI0ProcessTTI so no need to process the List*/
            if (node_p->dummyDCI0ProcessTTI > currentGlobalTTITickCount)
            {
                return;
            }
            tempUeId = node_p->ueIndex;
            tempUplinkContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
            if (! tempUplinkContextInfo_p->pendingDeleteFlag)
            {
                tempUplinkContext_p = (tempUplinkContextInfo_p->ulUEContext_p);
                /*SPR 4038 Start*/
                if(PNULL == tempUplinkContext_p)
                {
                    nextNode_p = (DummyDCI0Node *) getNextListNode(&(node_p->nodeAnchor));
                    listDeleteNode(dummyDCI0ListRequiredList_p,&(node_p->nodeAnchor));
                    freeMemPool(node_p);
                    node_p = nextNode_p;
                    continue;
                }

		    /* EICIC +*/
		/* EICIC Optimization +- */
		if(isEicicEnabled_g[internalCellIndex])
		{
			/*Complexity Start*/
		    if (((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == tempUplinkContext_p->userType))||
				((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == tempUplinkContext_p->userType)))
		    {      
			    node_p = (DummyDCI0Node *) getNextListNode(&(node_p->nodeAnchor));
			    continue;
		    }
			/* EICIC Optimization +- */
		}

                /*SPR 4038 End*/
                measGapContext_p = &tempUplinkContext_p->measGapContext;

                /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                if (tempUplinkContext_p->isAlreadyConsideredFlag !=
                         ulReferenceFlag_g[internalCellIndex] )
#elif TDD_CONFIG
                if (tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum] 
                    !=  ulReferenceFlag_g[internalCellIndex] )
#endif
                /* TDD Config 0 Changes End */
                {
                    /* \code
                     * if schType == SPS_SCHEDULING, DRX is ON and Meas 
                     *  gap period is ON
                     *     then do not schedule this UE.
                     * */
                    if((DRX_STATE_ON == tempUplinkContext_p->drxCurrentState)||
                            (TRUE == measGapContext_p->isMeasGapPeriodOn) || 
                            ( SPS_SCHEDULING == tempUplinkContext_p->schType )
                            /*HD FDD Chages Start*/
#ifdef HD_FDD_CONFIG
                            ||
                            /* If HD FDD UE is in  Explicit CQI UL grant  Q,then check Schedule map whether
                             * it can be scheduled in currenttick or not*/
                            ((TRUE == tempUplinkContext_p->isHDFddFlag) && 
                             (!CAN_SCHEDULE_UL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY, \
                                                         tempUeId , hdUeContextInfo_g.hdfddUeSchedMap)))
#endif
                            /*HD FDD Chages End*/
                      )
                    {
                        LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_UL_LOG_ID,
                                LOGDEBUG, MAC_UL_Strategy,
                                currentGlobalTTITickCount,
                                tempUeId,
                                tempUplinkContext_p->drxCurrentState,
                                measGapContext_p->isMeasGapPeriodOn,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                __func__,"");
                        /*push the node in pending queue*/

                        if (MAX_RETRY_FOR_LIST_INSERT > node_p->retryCounter)
                        {

                            if (!node_p->retryCounter)
                            {
                                listPopNode(dummyDCI0SendList_gp[internalCellIndex]);
                                listInsertNode(dummyDCI0SendFailureList_gp[internalCellIndex],&(node_p->nodeAnchor));
                                /* SPR_8672_FIX */
                                tempUplinkContext_p->eCidReportParams.dummyDCI0PresentFlag = PRESENT_IN_FAILURE_LIST;
                                /* SPR_8672_FIX */
                            }

                            node_p->retryCounter++;
                            node_p = (DummyDCI0Node *) getNextListNode(&(node_p->nodeAnchor));

                        }
                        else 
                        {
                            /* Max retries are done delete the node and send failure to RRM */
                            FETCH_NEW_NODE_FROM_DUMMY_DCI0_LIST(node_p,nextNode_p,
                                    dummyDCI0ListRequiredList_p,
                                    tempUplinkContext_p->eCidReportParams.dummyDCI0Entry_p);

                            putEntryInEcidMeasReportQueue(tempUeId,\
                                    NO_REPORT_TYPE_REQUIRED, \
                                    MAC_FAILURE, \
                                    /*CA Changes start  */
                                    internalCellIndex);
                            /*CA Changes end  */

                        }
                        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                        if(TRUE == tempUplinkContext_p->isHDFddFlag)
                        {
                            /* Increment the statistics counter UE denied UL resources due to
                             * DL clash in the same TTI of HD FDD UE*/
                            UPDATE_UE_DENIED_UL_RESOURCE(tempUeId);

                        }
#endif
                        /*HD FDD Changes End*/
                        continue;
                    }
#ifdef TDD_CONFIG
                    UInt8 ulHarqProcessId = BUSY_ALL_HARQ ;
                    /* Get the free ID in ulHarqProcessId
                       if all the harq id is busy the skip this node and goto the next node in
                       the list and start this procedure again.
                     */
                    /* TDD Config 0 and 6 Changes Start */
                    /* SPR 23393 Changes Start */
                    UInt32 sysFrameNum = 0;
                    UInt32 subframe     = 0;
                    /* +- SPR 17777 */
                    GET_SFN_SF_FROM_TTI(globalTick, sysFrameNum, subframe)
            /* SPR 23393 Changes End */
                    LTE_GCC_UNUSED_PARAM(sysFrameNum)
                        /* Get the free ID in ulHarqProcessId
                           if all the harq id is busy then skip this node and goto the next node in
                           the list and start this procedure again.
                         */
                        /* SPR 11257 Changes Start*/
                        ulHarqProcessId = getFreeULHarqId(tempUplinkContextInfo_p->ulUEContext_p,
				            subframe,internalCellIndex,ulSubFrameNum,currentGlobalTTITickCount);
                    /* +- SPR 17777 */
                    /* SPR 11257 Changes End*/
                    /* TDD Config 0 and 6 Changes End */        
#elif FDD_CONFIG
                    UInt8 ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount +
                            ulDelay + FDD_HARQ_OFFSET);
#endif
#ifdef TDD_CONFIG
                    if ( BUSY_ALL_HARQ == ulHarqProcessId)
#elif FDD_CONFIG
                        if(HARQ_PROCESS_FREE !=
                                tempUplinkContext_p->ulresInfo[ulHarqProcessId].harqStatus
                                /*SPR 7944 START*/
                                /* + SPS_TDD_Changes */
                                /*
                                   || checkSpsOccasionTti(tempUplinkContext_p,ulHarqProcessId)
                                   || checkSpsActTti(tempUplinkContext_p,ulHarqProcessId))
                                   */
                            /* - SPS_TDD_Changes */
                            )
                            /*SPR 7944 END*/
#endif
                    {
                        /* SPR_8672_FIX */
                        if (MAX_RETRY_FOR_LIST_INSERT > node_p->retryCounter)
                        {
                            if (!node_p->retryCounter)
                            {
                                    listPopNode(dummyDCI0SendList_gp[internalCellIndex]);
                                    listInsertNode(dummyDCI0SendFailureList_gp[internalCellIndex],&(node_p->nodeAnchor));
                                tempUplinkContext_p->eCidReportParams.dummyDCI0PresentFlag = PRESENT_IN_FAILURE_LIST;
                            }
                            node_p->retryCounter++;
                            node_p = (DummyDCI0Node *) getNextListNode(&(node_p->nodeAnchor));
                        }

                        else
                        {

                            FETCH_NEW_NODE_FROM_DUMMY_DCI0_LIST(node_p,nextNode_p,
                                    dummyDCI0ListRequiredList_p,
                                    tempUplinkContext_p->eCidReportParams.dummyDCI0Entry_p);

                            tempUplinkContext_p->eCidReportParams.typeofReportsRequired = 
                                NO_REPORT_TYPE_REQUIRED;
                        }
                        /* SPR_8672_FIX */
                        continue; 
                    }
                    avgMcs = tempUplinkContext_p->mcsIndex;
                    userLocationType = tempUplinkContext_p->userLocationType ;
                    /* If user is cell edge then we are checking if
                       maxCellEdgeUEsToBeScheduledUL_g is reached
                       or cell edge region RB is not available */
                        /*CA Changes start  */
                       if (CE_USER == userLocationType && (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]
                                /*CA Changes end  */
                                || !numberOfAvailableRBs_p[userLocationType]))
                    {
                        if (MAX_RETRY_FOR_LIST_INSERT > node_p->retryCounter)
                        {
                            if (!node_p->retryCounter)
                            {
                                   listPopNode(dummyDCI0SendList_gp[internalCellIndex]);
                                   listInsertNode(dummyDCI0SendFailureList_gp[internalCellIndex],&(node_p->nodeAnchor));
                                /* SPR_8672_FIX */
                                tempUplinkContext_p->eCidReportParams.dummyDCI0PresentFlag = PRESENT_IN_FAILURE_LIST;
                                /* SPR_8672_FIX */
                            }
                            node_p->retryCounter++;
                            node_p = (DummyDCI0Node *) getNextListNode(&(node_p->nodeAnchor));

                        }
                        else 
                        {
                            /* Max retries are done delete the node send failure to RRM */
                            FETCH_NEW_NODE_FROM_DUMMY_DCI0_LIST(node_p,nextNode_p,
                                    dummyDCI0ListRequiredList_p,
                                    tempUplinkContext_p->eCidReportParams.dummyDCI0Entry_p);
                            putEntryInEcidMeasReportQueue(tempUeId,\
                                       NO_REPORT_TYPE_REQUIRED, \
                                       MAC_FAILURE, \
                                       /*CA Changes start  */
                                       internalCellIndex);
                               /*CA Changes end  */
                           }
                           continue;
                       }                    
                       /* ICIC Change End */
                       /*If the UE has zero BSR available*/
                       if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->ulSchdConfig.ulSchedulerStrategy == UL_QOS_SCHEDULING)
                    {
                        /* ICIC changes start */
                        if ( numberOfAvailableRBs_p[userLocationType] >=
                                MAC_UL_GRANT_E_CID_DUMMY_DCI0_LOAD )
                            /* ICIC changes end */
                        {
                            /*Preparing the UL Strategy outPut Node*/
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
                            GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
                            GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                                    sizeof(ULStrategyTxNode),PNULL);
#endif
                            /* + Coverity 40850*/
                            if( PNULL == ulStrategyTxNode_p )
                            {
                                   if (MAX_RETRY_FOR_LIST_INSERT > node_p->retryCounter)
                                   {
                                       if (!node_p->retryCounter)
                                       {
                                           listPopNode(dummyDCI0SendList_gp[internalCellIndex]);
                                           listInsertNode(dummyDCI0SendFailureList_gp[internalCellIndex],&(node_p->nodeAnchor));
                                           /* SPR_8672_FIX */
                                           tempUplinkContext_p->eCidReportParams.dummyDCI0PresentFlag = PRESENT_IN_FAILURE_LIST;
                                           /* SPR_8672_FIX */
                                       }
                                       node_p->retryCounter++;
                                   }
                                   else
                                   {
                                       /* Max retries are done delete the node send failure to RRM */
                                       /*cov warning 61256*/
                                       listDeleteNode((dummyDCI0ListRequiredList_p),&(node_p->nodeAnchor));
                                       freeMemPool(node_p);
                                       node_p = PNULL;
                                       tempUplinkContext_p->eCidReportParams.dummyDCI0Entry_p = PNULL;
                                       putEntryInEcidMeasReportQueue(tempUeId,\
                                               NO_REPORT_TYPE_REQUIRED, \
                                               MAC_FAILURE, \
                                               /*CA Changes start  */
                                               internalCellIndex);
                                       /*CA Changes end  */
                                   }

                                   ltePanic("Cannot allocate memory for UL Startegy node, %s",
                                           __func__ );
                                   /* Klockwork warning Changes Start */
                                   return ;
                                   /* Klockwork warning Changes End */
                               }
                               ulStrategyTxNode_p->requiredRB = 0;
                               ulStrategyTxNode_p->maxDataSize = 0;
                               ulStrategyTxNode_p->minDataSize = 0;

                               /* - Coverity 40850*/
                               /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                               tempUplinkContext_p->isAlreadyConsideredFlag
                                   = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
                               tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                                   = ulReferenceFlag_g[internalCellIndex];
#endif
                               /* TDD Config 0 Changes End */
                               tempMaxUEsToBeScheduledUL--;
                            ulStrategyTxNode_p->ueIndex = (UInt16)tempUeId;

                            ulStrategyTxNode_p->requiredRB = MAC_UL_GRANT_E_CID_DUMMY_DCI0_LOAD;
                            numberOfAvailableRBs_p[userLocationType] -= 
                                MAC_UL_GRANT_E_CID_DUMMY_DCI0_LOAD;

                            if (userLocationType == CE_USER)
                            {
                                   maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                                numberOfAvailableRBs_p[CC_CE_REGION] -= MAC_UL_GRANT_E_CID_DUMMY_DCI0_LOAD; 
                            }

                            else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                                    numberOfAvailableRBs_p[CE_REGION])
                            {
                                numberOfAvailableRBs_p[CE_REGION] = 
                                    numberOfAvailableRBs_p[CC_CE_REGION];
                            }
                            /* ICIC changes end */
                            ulStrategyTxNode_p->strictAllocRB = TRUE;
                            ulStrategyTxNode_p->ulGrantRequestType = NEW_ULGRANT_REQ_MSG;
                            ulStrategyTxNode_p->ttiCounter =  globalTick;

                            ulStrategyTxNode_p->minDataSize = 
                                ulStrategyTxNode_p->maxDataSize =
                                determineUlTBSize(avgMcs,
                                        MAC_UL_GRANT_E_CID_DUMMY_DCI0_LOAD);

                            ulStrategyTxNode_p->avgMcs = avgMcs;
                            /* TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                            ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessId;
                            ulStrategyTxNode_p->scheduledCount = 0;
#endif
                            /* TDD Config 0 Changes End */

                            /*putEntry of ULStrategyTXNode in UL Strategy Schedule Info */

                               ulUeScheduleInfo_p = &(ulStrategyScheduleInfo_gp[internalCellIndex]->ulUeScheduleInfo
                                       [ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount]);

                            ulUeScheduleInfo_p->ulStrategyTxNode_p = ulStrategyTxNode_p;


                            ulUeScheduleInfo_p->ueULContext_p =
                                tempUplinkContext_p;

                            tempUplinkContext_p->ulStrategyTxNode_p = ulStrategyTxNode_p;

                            /* Increament the scheduled UE count */
                               /*CA Changes start  */
                               ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount++;
                               /*CA Changes end  */
                           }
                           else
                           {
                               if (MAX_RETRY_FOR_LIST_INSERT > node_p->retryCounter)
                               {
                                   if (!node_p->retryCounter)
                                   {
                                       listPopNode(dummyDCI0SendList_gp[internalCellIndex]);
                                       listInsertNode(dummyDCI0SendFailureList_gp[internalCellIndex],&(node_p->nodeAnchor));
                                    /* SPR_8672_FIX */
                                    tempUplinkContext_p->eCidReportParams.dummyDCI0PresentFlag = PRESENT_IN_FAILURE_LIST;
                                    /* SPR_8672_FIX */
                                }

                                node_p->retryCounter++;
                                node_p = (DummyDCI0Node *) getNextListNode(&(node_p->nodeAnchor));
                            }
                            else 
                            {
                                /* Max retries are done delete the node send failure to RRM */
                                FETCH_NEW_NODE_FROM_DUMMY_DCI0_LIST(node_p,nextNode_p,
                                        dummyDCI0ListRequiredList_p,
                                        tempUplinkContext_p->eCidReportParams.dummyDCI0Entry_p);

                                putEntryInEcidMeasReportQueue(tempUeId,\
                                           NO_REPORT_TYPE_REQUIRED,MAC_FAILURE,
                                           internalCellIndex);
                            }
                            continue;
                        }
                    }
                    else
                    {
                        /* Case when either RR or PF scheduler strategy */
                        /*Preparing the UL Strategy outPut Node*/
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
                        GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
                        GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                                sizeof(ULStrategyTxNode),PNULL);
#endif
                        /* + Coverity 40850*/
                        if( PNULL == ulStrategyTxNode_p )
                        {
                            ltePanic("Cannot allocate memory for UL Startegy node, %s",
                                    __func__ );
                               /*cov warning 40850 start*/
                               return ;
                               /*cov warning 40850 end */

                        }
                        ulStrategyTxNode_p->requiredRB = 0;
                        ulStrategyTxNode_p->maxDataSize = 0;
                        ulStrategyTxNode_p->minDataSize = 0;
                        /* - Coverity 40850*/
                        /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                        tempUplinkContext_p->isAlreadyConsideredFlag
                            = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
                        tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                            = ulReferenceFlag_g[internalCellIndex];
#endif
                        /* TDD Config 0 Changes End */
                        tempMaxUEsToBeScheduledUL--;
                        ulStrategyTxNode_p->ueIndex = (UInt16)tempUeId;

                        /* +DYNAMIC_ICIC */
                        ulStrategyTxNode_p->requiredRB =
                            MAC_UL_GRANT_E_CID_DUMMY_DCI0_LOAD;
                        /* ICIC changes start */
                        numberOfAvailableRBs_p[userLocationType] -= 
                            MAC_UL_GRANT_E_CID_DUMMY_DCI0_LOAD;

                        if (userLocationType == CE_USER)
                        {
                               /*CA Changes start  */
                               maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                               /*CA Changes end  */
                            numberOfAvailableRBs_p[CC_CE_REGION] -= 
                                MAC_UL_GRANT_E_CID_DUMMY_DCI0_LOAD;
                            /* -DYNAMIC_ICIC */
                        }

                        else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                                numberOfAvailableRBs_p[CE_REGION])
                        {
                            numberOfAvailableRBs_p[CE_REGION] = 
                                numberOfAvailableRBs_p[CC_CE_REGION];
                        }
                        /* ICIC changes end */

                        ulStrategyTxNode_p->strictAllocRB = TRUE;
                        ulStrategyTxNode_p->ulGrantRequestType = NEW_ULGRANT_REQ_MSG;

                        ulStrategyTxNode_p->ttiCounter =  globalTick;

                        ulStrategyTxNode_p->minDataSize = 
                            ulStrategyTxNode_p->maxDataSize =
                            determineUlTBSize(avgMcs,
                                    MAC_UL_GRANT_E_CID_DUMMY_DCI0_LOAD);
                        ulStrategyTxNode_p->avgMcs = avgMcs;
                        /* TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                        ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessId;
                        ulStrategyTxNode_p->scheduledCount = 0;
#endif
                        /* TDD Config 0 Changes End */

                        /*putEntry of ULStrategyTXNode in PDCCH Queue */
                           /*CA Changes start  */
                           if (!(putEntryInULSchedulerNewTxQueue(ulStrategyTxNode_p, internalCellIndex
                                           /*CA Changes end  */
                                           /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                                           ,currentGlobalTTITickCount
                                           ,ulSubFrameNum
                                           ,tempUplinkContext_p
#endif
                                           /* TDD Config 0 Changes End */
                                           )))
                           {
                            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                    LOGERROR,MAC_UL_Strategy,\
                                    currentGlobalTTITickCount,\
                                    ulStrategyTxNode_p->ueIndex,
#ifdef TDD_CONFIG
                                    sysFrameNum,
#else
                                    0,
#endif
                                    0,0,0,0.0,0.0,
                                    __func__,"ulStrategyTxNode_p entry failed");
#if ((DL_UL_SPLIT) && (CIRC_QUEUE_IMPL))
                            FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
                            freeMemPool(ulStrategyTxNode_p);
#endif

                            tempUplinkContext_p->eCidReportParams.typeofReportsRequired = 
                                NO_REPORT_TYPE_REQUIRED;
                        }
                        else
                        {
                            LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                                    LOGDEBUG,MAC_UL_Strategy,\
                                    currentGlobalTTITickCount,\
                                    ulStrategyTxNode_p->ueIndex,\
                                    ulStrategyTxNode_p->requiredRB,\
                                    ulStrategyTxNode_p->strictAllocRB,\
                                    ulStrategyTxNode_p->minDataSize,\
                                    ulStrategyTxNode_p->maxDataSize,\
                                    ulStrategyTxNode_p->avgMcs,\
                                    ulStrategyTxNode_p->ttiCounter,\
                                    __func__,"ulStrategyTxNode_p parameters");
                            //Log
                        }
                    }
                }
                else
                {
                    FETCH_NEW_NODE_FROM_DUMMY_DCI0_LIST(node_p,nextNode_p,dummyDCI0ListRequiredList_p,
                            tempUplinkContext_p->eCidReportParams.dummyDCI0Entry_p);
                    continue;

                }

            }
            else
            {
                tempUplinkContext_p = (tempUplinkContextInfo_p->ulUEContext_p);
                FETCH_NEW_NODE_FROM_DUMMY_DCI0_LIST(node_p,nextNode_p,dummyDCI0ListRequiredList_p,
                        tempUplinkContext_p->eCidReportParams.dummyDCI0Entry_p);

                tempUplinkContext_p->eCidReportParams.typeofReportsRequired = NO_REPORT_TYPE_REQUIRED;
                continue;
            }
            /* SPR_8672_FIX */
            FETCH_NEW_NODE_FROM_DUMMY_DCI0_LIST(node_p,nextNode_p,dummyDCI0ListRequiredList_p,
                    tempUplinkContext_p->eCidReportParams.dummyDCI0Entry_p);
            continue;
            /* SPR_8672_FIX */
        }
        else
        {
            break;
        }
    }//end While loop

    *maxUEsToBeScheduledUL_p = tempMaxUEsToBeScheduledUL ;
}

/*Cyclomatic Complexity changes - starts here */
/*****************************************************************************
 * Function Name  : processSRInULPriorityQueue
 * Inputs         : tempUplinkContext_p - Pointer to ULUEContext,
 *                  currentGlobalTTITickCount-Current Global Tick,
 *                  srNode_p - Pointer to ULGrantRequestQueueNode,
 *                  ulDelay - UL delay,
 *                  priorityGrantQueueCount - Count of Priority Grant Queue,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  tempMaxUEsToBeScheduledUL - Max UL UE's to be Scheduled,
 *                  ulHarqProcessId - UL HARQ Process ID,
 *                  internalCellIndex - cell index used at MAC.
 *                  ulSubFrameNum uplink subfraem number
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will process the SR resuqsts in UL Priority
 *                  Queue.
 *****************************************************************************/
STATIC  MacRetType processSRInULPriorityQueue(
        ULUEContext * tempUplinkContext_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        ULGrantRequestQueueNode *srNode_p,
        UInt32 ulDelay,
                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
        UInt32 * numberOfAvailableRBs_p,
                
        UInt8 *tempMaxUEsToBeScheduledUL
#ifdef TDD_CONFIG
        ,UInt8 ulHarqProcessId
        /* TDD Config 0 Changes Start */
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        /*CA Changes start */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end */
        )
{

    UInt16 srPeriodicity                 = 0;
    /* SPR 15909 fix start */
    tickType_t tempPhyTti                =  srNode_p->ttiCounter;
    /* SPR 15909 fix end */
    MeasGapContextUL * measGapContext_p  = PNULL;
    UInt8 avgMcs                         = 0;
    UInt8 minRb                          = 0;
    ULStrategyTxNode *ulStrategyTxNode_p = PNULL;
    DLUEContext *dlUEContext_p           = PNULL;
    DLUEContextInfo *dlUEContextInfo_p   = PNULL;

    dlUEContextInfo_p = &dlUECtxInfoArr_g[srNode_p->ueIndex];
    dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
    measGapContext_p = &tempUplinkContext_p->measGapContext;
    /*If the SR Periodicty Timer is expired,delete that SR Request*/
    /* Rel9_upgrade_CR410 */
    if (tempUplinkContext_p->pucchConfigInfo.srProhibitTimerValv920)
    {
        srPeriodicity = tempUplinkContext_p->
            pucchConfigInfo.srProhibitTimerValv920;
    }
    else 
    {
        srPeriodicity = tempUplinkContext_p->
            pucchConfigInfo.srPeriodicity;
    }
    if ((tempUplinkContext_p->pucchConfigInfo.srPeriodicity
                > SHORT_SR_PERIODICITY) && 
            ((currentGlobalTTITickCount - tempPhyTti 
              + ulDelay) >= srPeriodicity))
        /* Rel9_upgrade_CR410 */
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                "[UL-Strategy][%s]SR_Periodicty expired deleting SR\n"
                ,__func__);
        /*Freeing the SR Node*/
        freeMemPool((void *)srNode_p);
        srNode_p = PNULL;
    }
    /*If SR Periodicity timer is not Expired Then Send SR Response*/
    else
    {
        /* UL_SYNC_LOSS_ON CHG */
        /* MEAS_GAP_CHG */
        if ((TRUE == measGapContext_p->isMeasGapPeriodOn) ||
                (UL_SYNC_LOSS_ON ==
                 dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState) ||
                ((CE_USER == tempUplinkContext_p->userLocationType) && 
                /*CA Changes start  */
                 (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex])))
                /*CA Changes end  */
        {

            /* SPR 2377 changes start */
            LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_UL_LOG_ID, 
                    LOGDEBUG, MAC_UL_Strategy, 
                    currentGlobalTTITickCount, 
                    srNode_p->ueIndex, 
                    tempUplinkContext_p->drxCurrentState,
                    measGapContext_p->isMeasGapPeriodOn,
                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE, 
                    DEFAULT_FLOAT_VALUE,
                    __func__,"");

            /* SPR 2377 changes start */
            if(pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],&(srNode_p
                        ->ulGrantRequestNodeAnchor)))
            {
               freeMemPool(srNode_p);
               srNode_p = PNULL;
            }

                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
            return MAC_FAILURE;   
        }
        /* MEAS_GAP_CHG */
        /* UL_SYNC_LOSS_ON CHG */

        /* ULA_CHG */
        avgMcs = tempUplinkContext_p->mcsIndex;

        /* SPR 5713 changes start */ 
        if (avgMcs >=  MIN_GRANT_MCS_INDEX )
        {
            if (numberOfAvailableRBs_p[tempUplinkContext_p->userLocationType] < 
                    MAC_MIN_UL_GRANT_FOR_SR )
            {
                /* Available number of RBs is less, unable to
                 * process SR Node hence Pushing back into 
                 * ulHighPriorityGrantQueue_g */
                if (pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],
                            &(srNode_p->
                                ulGrantRequestNodeAnchor)))
                {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            /* SPR_2446 start */
                            srNode_p->ueIndex,\
                            /* SPR_2446 end */
                            0,0,0,0,0.0,0.0,__func__,
                            "Error in Pushing SR node in\
                            ulHighPriorityGrantQueue_g");
                    freeMemPool(srNode_p);
                }
                minRb = 0;
     	    /* SPR 10295 Fix Start */
                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
	    /* SPR 10295 Fix End */ 
            }
            else
            {
                minRb = MAC_MIN_UL_GRANT_FOR_SR;
            }
        }
        else
        {
            CALCULATE_MAC_MIN_UL_GRANT_RBs(avgMcs,minRb,tempUplinkContext_p->ueComplianceRelease);
            if(numberOfAvailableRBs_p[tempUplinkContext_p->userLocationType] < minRb)
            {
                /* Available number of RBs is less, unable to 
                 * process SR Node hence Pushing back into 
                 * ulHighPriorityGrantQueue_g */
                if (pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],
                            &(srNode_p->
                                ulGrantRequestNodeAnchor)))
                {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGWARNING,MAC_UL_Strategy,\
                            currentGlobalTTITickCount,\
                            /* SPR_2446 start */
                            srNode_p->ueIndex,\
                            /* SPR_2446 end */
                            0,0,0,0,0.0,0.0,__func__,
                            "Error in Pushing SR node in\
                            ulHighPriorityGrantQueue_g");
                    freeMemPool(srNode_p);
                }
                minRb = 0;
     	        /* SPR 10295 Fix Start */
                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
	        /* SPR 10295 Fix End */ 
            }
        }
        /* SPR 5713 changes end */

        if(minRb) 
        {
#ifdef FDD_CONFIG
            /** TTIB_Code Start */
            if( tempUplinkContext_p->ttiBundlingEnabled )
            {
                if ( avgMcs > MAX_TTI_BUNDLE_MCS )
                {
                    avgMcs = MAX_TTI_BUNDLE_MCS;
                }
                /*CA Changes start  */
                if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < minRb ) 
                        ||(MAC_FAILURE == processMeasGapValidation(
                                tempUplinkContext_p,currentGlobalTTITickCount,ulDelay )))
                {
                    /*cov_warning 56012 fix start*/
                    if(pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex],&(srNode_p
                                ->ulGrantRequestNodeAnchor)))
                    {    
                       freeMemPool(srNode_p);
                       srNode_p = PNULL;
                    }  
                    /*cov_warning 56012 fix end */
                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
                    return MAC_FAILURE;   
                }
                availableRBForTtiBundlingPerTick_g[internalCellIndex] -= minRb;
                LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy]"\
                        "[%s][%d]:TTIB UE ID %d scheduled RB:%d MCS:%d \n",
                        __func__,getCurrentTick(),srNode_p->ueIndex,minRb,avgMcs);
                /*CA Changes end  */
            }
            /** TTIB_Code End */
#endif

            /* ULA_CHG */
            LTE_MAC_UT_LOG(LOG_DETAIL,UL_STRATEGY,
                    "[UL-Strategy][%s]SR_Periodicty is NOT Expired,hence"
                    "Processing SR , CurGlbl Tick %d,\n" ,__func__,
                    currentGlobalTTITickCount);

            ulStrategyTxNode_p = PNULL;
            /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
            GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
            GET_MEM_FROM_POOL(ULStrategyTxNode , ulStrategyTxNode_p ,
                    sizeof(ULStrategyTxNode),PNULL);
#endif
            /*coverity 530 CID 32737 */
            if(PNULL == ulStrategyTxNode_p)
            {
                /*+COVERITY 5.3.0 - 52759*/
                freeMemPool((void *)srNode_p);
                srNode_p = PNULL;
                /*-COVERITY 5.3.0 - 52759*/

                /* SPR 10295 Fix Start review comment */
                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
                /* SPR 10295 Fix End review comment */

                return MAC_FAILURE;
            }
            ulStrategyTxNode_p->requiredRB = 0;
            ulStrategyTxNode_p->maxDataSize = 0;
            ulStrategyTxNode_p->minDataSize = 0;
            /*coverity 530 CID 32737 */
            (*tempMaxUEsToBeScheduledUL)--;
            /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
            tempUplinkContext_p->isAlreadyConsideredFlag 
                = ulReferenceFlag_g[internalCellIndex];
#elif TDD_CONFIG
            tempUplinkContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                = ulReferenceFlag_g[internalCellIndex];
#endif
            /* TDD Config 0 Changes End */
            ulStrategyTxNode_p->ueIndex = (UInt16)srNode_p->ueIndex;
            /* ULA_CHG */
            ulStrategyTxNode_p->requiredRB = minRb;
            /*  ICIC changes start */
            numberOfAvailableRBs_p[tempUplinkContext_p->userLocationType] -= minRb;
            if (tempUplinkContext_p->userLocationType == CE_USER)
            {
                /*CA Changes start  */
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]--;
                /*CA Changes end  */
                numberOfAvailableRBs_p[CC_CE_REGION] -= minRb;
            }
            else if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                    numberOfAvailableRBs_p[CE_REGION])
            {
                numberOfAvailableRBs_p[CE_REGION] = 
                    numberOfAvailableRBs_p[CC_CE_REGION];
            }
            /*  ICIC changes end */

            /* ULA_CHG */
            ulStrategyTxNode_p->strictAllocRB = TRUE;
            ulStrategyTxNode_p->ulGrantRequestType =
                SR_ULGRANT_REQ_MSG;
            ulStrategyTxNode_p->ttiCounter = srNode_p->ttiCounter;
#ifdef TDD_CONFIG                            
            ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessId;
            /*TDD Config 0 Changes Start*/
            ulStrategyTxNode_p->scheduledCount = 0;
            /*TDD Config 0 Changes End*/
#endif                            
            /* ULA_CHG */
            ulStrategyTxNode_p->minDataSize = 
                /* SPR 5713 changes start */ 
                tempUplinkContext_p->ulGrantMinDataSize;
            /* SPR 5713 changes end */ 
            ulStrategyTxNode_p->maxDataSize = 
                determineUlTBSize(avgMcs,minRb);
            ulStrategyTxNode_p->avgMcs = avgMcs;
            /* ULA_CHG */

            /*putEntry of ULStrategyTXNode in PDCCH Queue */
            /*CA Changes start  */
            if (!(putEntryInULSchedulerNewTxQueue
                        (ulStrategyTxNode_p, internalCellIndex
                         /*CA Changes end  */
                         /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                         ,currentGlobalTTITickCount
                         ,ulSubFrameNum
                         ,tempUplinkContext_p
#endif
                         /* TDD Config 0 Changes End */
                        )))
            {
                /* ULA_CHG */
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGERROR,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        0,0,0,0,0.0,0.0,__func__,
                        "ulStrategyTxNode_p entry failed");
                /* ULA_CHG */
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
                freeMemPool(ulStrategyTxNode_p);
#endif
            }
            /* ULA_CHG */
            else
            {
                LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                        LOGDEBUG,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        ulStrategyTxNode_p->requiredRB,\
                        ulStrategyTxNode_p->strictAllocRB,\
                        ulStrategyTxNode_p->minDataSize,\
                        ulStrategyTxNode_p->maxDataSize,\
                        ulStrategyTxNode_p->avgMcs,\
                        ulStrategyTxNode_p->ttiCounter,\
                        __func__,"ulStrategyTxNode_p parameters");
            }
            tempUplinkContext_p->scheduleFlag = UL_QOS_ALREADY_TRANSMITED;
                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
            /* ULA_CHG */
            freeMemPool((void *)srNode_p);
            srNode_p = PNULL;
        }
    }
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : putEntryInBSRListForULPriorityQueue
 * Inputs         : tempUplinkContext_p - Pointer to ULUEContext,
 *                  priorityGrantQueueCount - Count of Priority Grant Queue,
 *                  tempMaxUEsToBeScheduledUL - Max UL UE's to be Scheduled,
 *                  srNode_p - Pointer to ULGrantRequestQueueNode,
 *                  internalCellIndex - cell index used at MAC.
 *                  execArgs - Execution Legs Arguments
 * Outputs        : None
 * Returns        : None
 * Description    : This function put the entry in BSR List for UL Priority
 *                  Queue.
 *****************************************************************************/
STATIC  void putEntryInBSRListForULPriorityQueue(
        ULUEContext * tempUplinkContext_p,
                    /*SPR 22946 Fix Start*/
                    /*code removed*/
                    /*SPR 22946 Fix End*/
        UInt8 *tempMaxUEsToBeScheduledUL,
        ULGrantRequestQueueNode *srNode_p,
        /*CA Changes start */
        InternalCellIndex internalCellIndex 
        ,ExecutionLegFunctionArg* execArgs)
{
    UInt8 lcgId = 0;
    /*UL Force Grant Changes Start */
    UInt8 forceGrantFlag = LTE_FALSE;
    if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
       initParams_p->ulSchdConfig.ulSchedulerStrategy == UL_QOS_SCHEDULING)
    {
        for(;lcgId < NUM_OF_LCG; lcgId++)
        {
            if (tempUplinkContext_p->ulLCGInfo[lcgId].qLoad)
            {
                ulQosPutEntryInNonZeroBSRList(tempUplinkContext_p, lcgId, 
                        internalCellIndex, execArgs, &forceGrantFlag );
                /*UL Force Grant Changes End */
            }
        }
    }
    else if ((PNULL == tempUplinkContext_p->nonZeroBsrList_p))
    {
        putEntryInNonZeroBSRList(srNode_p->ueIndex,
                tempUplinkContext_p,
                /*CA Changes start  */
                internalCellIndex);
                /*CA Changes end  */
        (*tempMaxUEsToBeScheduledUL)--;
        /*SPR 22946 Fix Start*/
        /*code removed*/
        /*SPR 22946 Fix End*/
        /* SPR 12307 - Removed Lines */
    }

}

/****************************************************************************
 * Function Name  : cleanUpUlGrantPendingQ
 * Inputs         : ulGrantQueue_p,
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API clean up UL pending grant queue
 *****************************************************************************/
void cleanUpUlGrantQ(UL_GRANT_PUSCH_QUEUE_TYPE * ulGrantQueue_p)
{
    UInt32 i = 0;
    UInt32 count = 0;
    ULGrantRequestQueueNode * node_p  = PNULL;

     /*CA Changes start  */
    count = COUNT_UL_GRANT_PUSCH_QUEUE_Q(ulGrantQueue_p);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        /*CA Changes start  */
        DEQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
              ulGrantQueue_p,ULGrantRequestQueueNode,(void**)&node_p);
        /*CA Changes end  */

        if (node_p)
        {
            freeMemPool(node_p);
            node_p = PNULL;
        }
    }
 
}

/****************************************************************************
 * Function Name  : cleanUpUlGrantConResQ
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API clean up UL grant contention resolution Queue
 *****************************************************************************/
void cleanUpUlGrantConResQ(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    ULGrantRequestQueueNode * node_p  = PNULL;

      /*CA Changes start  */
    count = sQueueCount(ulGrantContentionResQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        /*CA Changes start  */
        node_p = (ULGrantRequestQueueNode *)popNode(ulGrantContentionResQueue_gp[internalCellIndex]);
        /*CA Changes end  */

        if (node_p)
        {
            freeMemPool(node_p);
            node_p = PNULL;
        }
    }
  
    return;
}

/****************************************************************************
 * Function Name  : cleanUpAperiodicUlCqiQ
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API clean up Ul grant Request queue
 *****************************************************************************/
void cleanUpAperiodicUlCqiQ(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    ULGrantRequestQueueNode * node_p  = PNULL;

      count = COUNT_UL_GRANT_APERIODIC_CQI_Q(internalCellIndex);
    for (i = 0; i < count; i++)
    {
        DEQUEUE_UL_GRANT_APERIODIC_CQI_Q(ULGrantRequestQueueNode,(void *)&node_p,
                                         internalCellIndex);

        if (node_p)
        {
            FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(node_p);
        }
    }

    return;
}
/****************************************************************************
 * Function Name  : cleanUpBsrTriggerQ
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API clean up BSR trigger queue
 *****************************************************************************/
void cleanUpBsrTriggerQ(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    /* SPR 23728 FIX START */
    UInt32 j = 0;
    /* SPR 23728 FIX START */
    UInt32 count = 0;
    bsrQueueNode * bsrQueueNode_p     = PNULL;
    
     for ( i = 0 ; i < NUM_OF_DEMUX_THREADS;  i++)
    {
        /*Clean up for  Zero NonZero BSR Trigger Queues */
        /*CA Changes start  */
        count = sQueueCount(ZeroNonZeroBSRTriggerQueue_gp[internalCellIndex] + i);
        /*CA Changes end  */
    /* SPR 23728 FIX START */
        for (j = 0; j < count; j++)
    /* SPR 23728 FIX START */
        {
            /*CA Changes start  */
            bsrQueueNode_p = (bsrQueueNode *)popNode(ZeroNonZeroBSRTriggerQueue_gp[internalCellIndex] + i);
            /*CA Changes end  */

            if (bsrQueueNode_p)
            {
                freeMemPool(bsrQueueNode_p);
                bsrQueueNode_p = PNULL;
            }
        }
    }

    /*CA Changes end  */

    return;
}
/****************************************************************************
 * Function Name  : cleanUpNonZeroBsrList
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API clean up Non BSR list
 *****************************************************************************/
void cleanUpNonZeroBsrList(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    nonZeroBsrListNode * nonZeroBsrListNode_p       = PNULL;
    
    /* Clean up for  Non Zero Queue Load and Harq List */
    /*CA Changes start  */
    count = listCount(nonZeroBsrList_gp[internalCellIndex]);
    for (i = 0; i < count; i++)
    {
        nonZeroBsrListNode_p = (nonZeroBsrListNode *)listPopNode(nonZeroBsrList_gp[internalCellIndex]);

        if (nonZeroBsrListNode_p)
        {
            freeMemPool(nonZeroBsrListNode_p);
            nonZeroBsrListNode_p = PNULL;
        }
    }

    listInit(nonZeroBsrList_gp[internalCellIndex]);

    return;
   /*CA Changes end  */
}

/****************************************************************************
 * Function Name  : cleanUpCEUserPriorityQ
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API clean up CE User priority queue
 *****************************************************************************/
void cleanUpCEUserPriorityQ(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    CEUserPriorityQueueNode *ceNode_p = PNULL;

     /*CA Changes start  */
    count = sQueueCount(ulCellEdgeUserPriorityQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        ceNode_p =
            /*CA Changes start  */
            (CEUserPriorityQueueNode *)popNode(ulCellEdgeUserPriorityQueue_gp[internalCellIndex]);
            /*CA Changes end  */

        if (ceNode_p)
        {
            freeMemPool(ceNode_p);
            ceNode_p = PNULL;
        }
    }

    return;
}
/****************************************************************************
 * Function Name  : cleanUpexpilicitPuschTpcScheduleQ
 * Inputs         : explicitULPuschTpcScheduleQueue_p,
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API clean up TPC schedule failure queue
 *****************************************************************************/
void cleanUpExpilicitPuschTpcScheduleQ(
            EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE * explicitULPuschTpcScheduleQueue_p)
{
    UInt32 i = 0;
    UInt32 count = 0;
    ExplicitULDCIAndPHRNode *explicitNode_p   = PNULL;
 
    /*Coverity-5.3.0 CID-54633*/
    /*CA Changes start  */
    count =COUNT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(explicitULPuschTpcScheduleQueue_p);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
       DEQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(
              /*CA Changes start  */
              explicitULPuschTpcScheduleQueue_p, ExplicitULDCIAndPHRNode,(void**)&explicitNode_p);
              /*CA Changes end  */
        if (explicitNode_p)
        {
            FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(explicitNode_p);
        }
    }

    /*Coverity-5.3.0 CID-54633*/
    return;
}

/****************************************************************************
 * Function Name  : cleanUpDrxOnOffTriggerQ
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API cleanup DRX on-off trigger queue
 *****************************************************************************/
void cleanUpDrxOnOffTriggerQ(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    DRXOnOffTriggerNode     *drxTrigNode_p             = PNULL;
    
     /*Coverity-5.3.0 CID-54634*/
    /*CA Changes start  */
    count =COUNT_UL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        /*CA Changes start  */
        DEQUEUE_UL_DRX_ON_OFF_TRIGGER_Q(DRXOnOffTriggerNode,(void *)&(drxTrigNode_p), internalCellIndex);
        /*CA Changes end  */
        if (drxTrigNode_p)
        {
           FREE_MEM_NODE_UL_DRX_ON_OFF_TRIGGER_Q(drxTrigNode_p);
        }
    }
    /*Coverity-5.3.0 CID-54634*/
    return;
}
/****************************************************************************
 * Function Name  : cleanUpMeasGapOnOffTriggerQ
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API cleanup Meas Gap On/OFF trigger queue
 *****************************************************************************/
void cleanUpMeasGapOnOffTriggerQ(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    MeasGapOnOffTriggerNode *measGapOnOffTriggerNode_p = PNULL;
    
     /*Coverity-5.3.0 CID-54638*/
    /*CA Changes start  */
    count = COUNT_UL_MEAS_GAPON_OFF_TRIGGER_Q(internalCellIndex);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        /*CA Changes start  */
        DEQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode, (void *)&measGapOnOffTriggerNode_p, internalCellIndex);
        /*CA Changes end  */
        if (measGapOnOffTriggerNode_p)
        {
            FREE_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
        }
    }
    /*Coverity-5.3.0 CID-54638*/

    return;
}
/****************************************************************************
 * Function Name  :cleanUpUlSpsQueues
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API will cleanup UL SPS related queues
 *****************************************************************************/
void cleanUpUlSpsQueues(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    LP_MacULSpsActReq        macUlSpsActReq_p          = PNULL;
    LP_MacULSpsDeactReq      macUlSpsDectReq_p         = PNULL;
    LP_MacULSpsReactReq      macUlSpsReactReq_p        = PNULL;
    LP_MacULSpsResReq        spsUlResRelReq_p          = PNULL;
    LP_MacULSpsResReq        spsUlResReq_p             = PNULL;
    LP_MacULSpsStatus        macUlSpsStatus_p          = PNULL;

    /*Coverity-5.3.0 CID-54639*/
    count =QCOUNT_MAC_UL_SPS_Q(ulSpsActQ_gp[internalCellIndex]);
    for (i = 0; i < count; i++)
    {
        DEQUEUE_MAC_UL_SPS_Q(ulSpsActQ_gp[internalCellIndex], macUlSpsActReq_p );
        if (macUlSpsActReq_p)
        {
            freeMemPool(macUlSpsActReq_p);
            macUlSpsActReq_p = PNULL;
        }
    }
    /*Coverity-5.3.0 CID-54639*/

    /*Coverity-5.3.0 CID-54640*/
    count =QCOUNT_MAC_UL_SPS_Q(ulSpsDeactQ_gp[internalCellIndex]);
    for (i = 0; i < count; i++)
    {
        DEQUEUE_MAC_UL_SPS_Q(ulSpsDeactQ_gp[internalCellIndex], macUlSpsDectReq_p);
        if (macUlSpsDectReq_p)
        {
            freeMemPool(macUlSpsDectReq_p);
            macUlSpsDectReq_p = PNULL;
        }
    }
    /*Coverity-5.3.0 CID-54640*/

    /*Coverity-5.3.0 CID-54641*/
    count =QCOUNT_MAC_UL_SPS_Q(ulSpsReactQ_gp[internalCellIndex]);
    for (i = 0; i < count; i++)
    {
        DEQUEUE_MAC_UL_SPS_Q(ulSpsReactQ_gp[internalCellIndex], macUlSpsReactReq_p);
        if (macUlSpsReactReq_p)
        {
            freeMemPool(macUlSpsReactReq_p);
            macUlSpsReactReq_p = PNULL;
        }
    }
    /*Coverity-5.3.0 CID-54641*/

    /*Coverity-5.3.0 CID-54642*/
    count =QCOUNT_MAC_UL_SPS_Q(ulSpsResRelQ_gp[internalCellIndex]);
    for (i = 0; i < count; i++)
    {
        DEQUEUE_MAC_UL_SPS_Q(ulSpsResRelQ_gp[internalCellIndex],spsUlResRelReq_p);
        if (spsUlResRelReq_p)
        {
            freeMemPool(spsUlResRelReq_p);
            spsUlResRelReq_p = PNULL;
        }
    }
    /*Coverity-5.3.0 CID-54642*/

    /*Coverity-5.3.0 CID-54643*/
    count =QCOUNT_MAC_UL_SPS_Q(ulSpsResRsvQ_gp[internalCellIndex]);
    for (i = 0; i < count; i++)
    {
        DEQUEUE_MAC_UL_SPS_Q(ulSpsResRsvQ_gp[internalCellIndex],spsUlResReq_p);
        if (spsUlResReq_p)
        {
            freeMemPool(spsUlResReq_p);
            spsUlResReq_p = PNULL;
        }
    }
    /*Coverity-5.3.0 CID-54643*/

    /*Coverity-5.3.0 CID-54644*/
    count =QCOUNT_MAC_UL_SPS_Q(ulSpsStatusQ_gp[internalCellIndex]);
    for (i = 0; i < count; i++)
    {
        DEQUEUE_MAC_UL_SPS_Q(ulSpsStatusQ_gp[internalCellIndex],macUlSpsStatus_p);
        if (macUlSpsStatus_p)
        {
            freeMemPool(macUlSpsStatus_p);
            macUlSpsStatus_p = PNULL;
        }
    }
}

/****************************************************************************
 * Function Name  : initZeroNonZeroBsr 
 * Inputs         : cellCount
 * Outputs        : None
 * Returns        : None
 * Description    : This function inits zero and non zero BSR trigger queues 
 *                  and list and QOS strategy tree.
 *****************************************************************************/
void initZeroNonZeroBsr( UInt8 cellCount )
{
    UInt32 i = 0;
    /* EICIC +*/
   UInt8  uetype = 0;
    /* EICIC -*/

    GET_MEM_FROM_POOL(LTE_SQUEUE, ZeroNonZeroBSRTriggerQueue_gp[cellCount],
            sizeof(LTE_SQUEUE) * NUM_OF_DEMUX_THREADS, PNULL);
    for( i = 0; i < NUM_OF_DEMUX_THREADS; i++)
    {
        /* Initializing Zero NonZero BSR Trigger Queues */
        sQueueInit(ZeroNonZeroBSRTriggerQueue_gp[cellCount] + i);
    }

    /* Initializing Non Zero BSR List */
    GET_MEM_FROM_POOL( LTE_LIST, nonZeroBsrList_gp[cellCount], 
            sizeof(LTE_LIST), PNULL);
    listInit(nonZeroBsrList_gp[cellCount]);

    for (uetype = 0;  uetype <MAX_VALID_UE_TYPE; uetype++)
    {
        /* ul qos strategy lc tree */
        GET_MEM_FROM_POOL( LTE_TREE, ulQosStrategyLCTree_gp[cellCount][uetype],
            sizeof(LTE_TREE), PNULL);
        ulQosInitLcTree(ulQosStrategyLCTree_gp[cellCount][uetype],
            &ulQosCompareLcNode,
            &ulQosKeyOfLcNode);

    }
}

/****************************************************************************
 * Function Name  : initExplicitUlPuschTpc 
 * Inputs         : cellCount
 * Outputs        : None
 * Returns        : None
 * Description    : This function inits TPC queues.
 *****************************************************************************/
void initExplicitUlPuschTpc( UInt8 cellCount )
{
    GET_MEM_FROM_POOL( EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE, 
            explicitULPuschTpcScheduleQueue_gp[cellCount], 
            sizeof(EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE), PNULL);
    INIT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q( 
            explicitULPuschTpcScheduleQueue_gp[cellCount] );

    GET_MEM_FROM_POOL( EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE, 
            explicitULPuschTpcScheduleFailureQueue_gp[cellCount],
            sizeof(EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE), PNULL );
    INIT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q( 
            explicitULPuschTpcScheduleFailureQueue_gp[cellCount] );
}

/****************************************************************************
 * Function Name  : initSPSInULScheduler 
 * Inputs         : cellCount
 * Outputs        : None
 * Returns        : None
 * Description    : This function init SPS in ul scheduler initialization.
 *****************************************************************************/
void initSPSInULScheduler(UInt8 cellCount)
{
    /*! \code
     * SPS Queues initialized which are used by SPS strategy: (ulSpsStatusQ_g,
     * ulSpsActQ_g, ulSpsDeactQ_g, ulSpsReactQ_g, ulSpsUeExclude_g,
     * ulSpsResRsvQ_g, ulSpsResRelQ_g)
     * \endcode
     * */
    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, ulSpsStatusQ_gp[cellCount],
            sizeof(LTE_CIRCQUEUE),PNULL  );
    QINIT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[cellCount] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, ulSpsActQ_gp[cellCount],
            sizeof(LTE_CIRCQUEUE),PNULL  );
    QINIT_MAC_UL_SPS_Q( ulSpsActQ_gp[cellCount] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, ulSpsDeactQ_gp[cellCount],
            sizeof(LTE_CIRCQUEUE),PNULL  );
    QINIT_MAC_UL_SPS_Q( ulSpsDeactQ_gp[cellCount] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, ulSpsReactQ_gp[cellCount],
            sizeof(LTE_CIRCQUEUE),PNULL  );
    QINIT_MAC_UL_SPS_Q( ulSpsReactQ_gp[cellCount] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, ulSpsResRsvQ_gp[cellCount],
            sizeof(LTE_CIRCQUEUE),PNULL  );
    QINIT_MAC_UL_SPS_Q( ulSpsResRsvQ_gp[cellCount] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, ulSpsResRelQ_gp[cellCount],
            sizeof(LTE_CIRCQUEUE),PNULL  );
    QINIT_MAC_UL_SPS_Q( ulSpsResRelQ_gp[cellCount]);
}
/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name    : updateSchCountInUlStrategyNewTxQueue 
 * Inputs           : ueIndex : UeIndex of the node to be modified
 * Outputs          : scheduledCount : scheduled count 
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function updates the scheduledCount of 
 *                    ulStrategyTxQueue_g already pushed.
 *****************************************************************************/
STATIC  MacRetType updateSchCountInUlStrategyNewTxQueue(
    UInt16 ueIndex,
    UInt8 scheduledCount,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex
    /*CA Changes end  */ 
    )
{
    ULStrategyTxNode *ulStrategyTxNode_p = PNULL;
    ulStrategyTxNode_p = (ULStrategyTxNode*)
        getFirstNode( ulStrategyTxQueue_gp[internalCellIndex]);
    while (PNULL != ulStrategyTxNode_p)
    {
        /*Check the ueIndex */
        if (ueIndex == ulStrategyTxNode_p->ueIndex)
        {
            ulStrategyTxNode_p->scheduledCount = scheduledCount;
            return MAC_SUCCESS;
        }
        /* Get the next Node */
        ulStrategyTxNode_p = (ULStrategyTxNode *)
                getNextNode((const LTE_SNODE *)ulStrategyTxNode_p);
    }
    return MAC_FAILURE;
}
/*****************************************************************************
 * Function Name    : processBothUlSfListNodes 
 * Inputs           : ULStrategyTxNode of LIST ulStrategyTdd0SsfTxList_g
 * Outputs          : UL Strategy Output Nodes are passed to PDCCH Module
 * Returns          : void
 * Description      : This function traverse LIST Q1 and LIST Q2 alternately
 *                    and pushed the node in ulStrategyTxQueue_g Q whose 
 *                    scheduled count is <= 1.
 *                    - Travrse alternately LIST Q1 and LIST Q2
 *                    - If scheduled count is <= 1
 *                      1. Set UE's scheduledCount to 0
 *                      2. Setstrategy node's schCount to 1
 *                      3. Delete Node from the List.
 *****************************************************************************/
STATIC  void processBothUlSfListNodes(
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
)
{
    UInt16 ueIndex    = 0;
    MacRetType retVal = MAC_SUCCESS;
    ULUEContext      *ulUEContext_p                    = PNULL;
    ULUEContextInfo  *uplinkContextInfo_p              = PNULL;
    void             *ulStrategyNodeVoid_p             = PNULL;
    void             *ulStrategyOffset7NodeVoid_p      = PNULL;
    ULStrategyTxNode *ulStrategyTxNodeOffsetK_p        = PNULL;
    ULStrategyTxNode *ulStrategyTxNodeOffset7_p        = PNULL;

    /* CA TDD CHANGES START */
    ulStrategyNodeVoid_p =(void *) getFirstListNode(
            &ulStrategyTdd0SsfTxList_g[internalCellIndex]);
    ulStrategyOffset7NodeVoid_p =(void *) getFirstListNode(
            &ulStrategyTdd0SsfTxListUSfOffset7_g[internalCellIndex]);
    /* CA TDD CHANGES END */

    /* Traverse both the Lists ulStrategyTdd0SsfTxList_g and 
     * ulStrategyTdd0SsfTxListUSfOffset7_g Alternatively  */
    do
    {
        if (PNULL != ulStrategyNodeVoid_p)
        {
            /* Reseting the retVal with MAC_SUCCESS */
            retVal = MAC_SUCCESS;
            /* Get the base address */
            UL_GET_BASE_ADDRESS_FROM_STR_TDD0_S_SF_TX_NODE_ANCHOR(
                    ulStrategyNodeVoid_p, ulStrategyTxNodeOffsetK_p) 

            /* Getting the ueContext */
            ueIndex = ulStrategyTxNodeOffsetK_p->ueIndex;
            uplinkContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

            /* Check if pendingDeleteFlag is true or ulUEContext_p is NULL*/
            if ((uplinkContextInfo_p->pendingDeleteFlag) || 
                    (PNULL == ulUEContext_p))
            {
                retVal = MAC_FAILURE;
            }
            else if (ulUEContext_p->scheduledCount < NUM_TWO)
            {

                /* Initialize strategy node's scheduled count
                 * and push into PDCCH output Q.*/
                ulStrategyTxNodeOffsetK_p->scheduledCount = NUM_ONE;

                /* Reset the scheduledCount to 0 in UL UE Context */
                ulUEContext_p->scheduledCount = 0;

                /* Push the node in PDCCH output Queue */
                retVal = pushListNodeInULSchedulerNewTxQueue(
                        ulStrategyTxNodeOffsetK_p,
                        internalCellIndex);
            }


            /* Deleting the node from ulStrategyTdd0SsfTxList_g List */
            /* CA TDD CHANGES START*/
            listDeleteNode(&ulStrategyTdd0SsfTxList_g[internalCellIndex],
            /* CA TDD CHANGES END*/
                    &ulStrategyTxNodeOffsetK_p->ulStrategyTdd0SsfnTxNodeAnchor);

            /* Get the next node */
            ulStrategyNodeVoid_p =(void *) getNextListNode(
                    &ulStrategyTxNodeOffsetK_p->ulStrategyTdd0SsfnTxNodeAnchor);

            if (MAC_FAILURE == retVal)
            {
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGERROR,MAC_UL_Strategy,\
                        getCurrentTick(),\
                        ulStrategyTxNodeOffsetK_p->ueIndex,\
                        0,0,0,0,0.0,0.0,__func__,
                        "ulStrategyTxNodeOffsetK_p entry failed");
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNodeOffsetK_p);
#else
                freeMemPool(ulStrategyTxNodeOffsetK_p);
#endif
            }

        }
        while (PNULL != ulStrategyOffset7NodeVoid_p)
        {
            /* Reseting the retVal with MAC_SUCCESS */
            retVal = MAC_SUCCESS;
            /* Get the base address */
            UL_GET_BASE_ADDRESS_FROM_STR_TDD0_S_SF_TX_NODE_ANCHOR(
                    ulStrategyOffset7NodeVoid_p, ulStrategyTxNodeOffset7_p) 

                /* Getting the ueContext */
                ueIndex = ulStrategyTxNodeOffset7_p->ueIndex;
            uplinkContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

            /* Check if pendingDeleteFlag is true or ulUEContext_p is NULL*/
            if ((uplinkContextInfo_p->pendingDeleteFlag) || 
                    (PNULL == ulUEContext_p))
            {
                retVal = MAC_FAILURE;

                /* Deleting the node from ulStrategyTdd0SsfTxListUSfOffset7_g List */
                /* CA TDD CHANGES START */
                listDeleteNode(&ulStrategyTdd0SsfTxListUSfOffset7_g[internalCellIndex],
                        &ulStrategyTxNodeOffset7_p->ulStrategyTdd0SsfnTxNodeAnchor);
                /* CA TDD CHANGES END */
                /* Get the next node */
                ulStrategyOffset7NodeVoid_p =(void *) getNextListNode(
                        &ulStrategyTxNodeOffset7_p->ulStrategyTdd0SsfnTxNodeAnchor);

                if (MAC_FAILURE == retVal)
                {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGERROR,MAC_UL_Strategy,\
                            getCurrentTick(),\
                            ulStrategyTxNodeOffset7_p->ueIndex,\
                            0,0,0,0,0.0,0.0,__func__,
                            "ulStrategyTxNodeOffset7_p entry failed");
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                    FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNodeOffset7_p);
#else
                    freeMemPool(ulStrategyTxNodeOffset7_p);
#endif
                }
            }
            else if (ulUEContext_p->scheduledCount < NUM_TWO)
            {

                /* Initialize strategy node's scheduled count
                 * and push into PDCCH output Q.*/
                ulStrategyTxNodeOffset7_p->scheduledCount = NUM_ONE;

                /* Reset the scheduledCount to 0 in UL UE Context */
                ulUEContext_p->scheduledCount = 0;

                /* Push the node in PDCCH output Queue */
                retVal = pushListNodeInULSchedulerNewTxQueue(
                        ulStrategyTxNodeOffset7_p,
                        internalCellIndex);


                /* Deleting the node from ulStrategyTdd0SsfTxListUSfOffset7_g List */
                /* CA TDD CHANGES START */
                listDeleteNode(&ulStrategyTdd0SsfTxListUSfOffset7_g[internalCellIndex],
                        &ulStrategyTxNodeOffset7_p->ulStrategyTdd0SsfnTxNodeAnchor);
                /* CA TDD CHANGES END */

                /* Get the next node */
                ulStrategyOffset7NodeVoid_p =(void *) getNextListNode(
                        &ulStrategyTxNodeOffset7_p->ulStrategyTdd0SsfnTxNodeAnchor);

                if (MAC_FAILURE == retVal)
                {
                    LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                            LOGERROR,MAC_UL_Strategy,\
                            getCurrentTick(),\
                            ulStrategyTxNodeOffset7_p->ueIndex,\
                            0,0,0,0,0.0,0.0,__func__,
                            "ulStrategyTxNodeOffset7_p entry failed");
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                    FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNodeOffset7_p);
#else
                    freeMemPool(ulStrategyTxNodeOffset7_p);
#endif
                }
                /* Break from the loop */
                break;
            }
            else
            {
                /* Get the next node */
                ulStrategyOffset7NodeVoid_p =(void *) getNextListNode(
                        &ulStrategyTxNodeOffset7_p->ulStrategyTdd0SsfnTxNodeAnchor);
                continue;
            }
        }
    } while ((PNULL != ulStrategyNodeVoid_p) ||
            (PNULL != ulStrategyOffset7NodeVoid_p));
}
/*****************************************************************************
 * Function Name    : processUlSfOffset7ListNodes 
 * Inputs           : void * node of LIST ulStrategyTdd0SsfTxList_g
 *                    used for processing of nodes 
 * Outputs          : UL Strategy Output Nodes are passed to PDCCH Module
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This Function process the List 
 *                    ulStrategyTdd0SsfTxListUSfOffset7_g. Scheduled count 2
 *                    value nodes will be processed.
 *
 *****************************************************************************/
STATIC  void processUlSfOffset7ListNodes(
         void *ulStrategyNodeVoid_p,
         /*CA Changes start  */
         InternalCellIndex internalCellIndex
         /*CA Changes end  */ 
        )
{
    UInt16     ueIndex = 0;
    MacRetType retVal  = MAC_SUCCESS;
    ULUEContext      *ulUEContext_p        = PNULL;
    ULUEContextInfo  *uplinkContextInfo_p  = PNULL;
    ULStrategyTxNode *ulStrategyTxNode_p   = PNULL;

    /* Traverse the ulStrategyTdd0SsfTxListUSfOffset7_g List */
    while (PNULL != ulStrategyNodeVoid_p)
    {
        /* Reseting the retVal with MAC_SUCCESS */
        retVal = MAC_SUCCESS;
        /* Get the base address */
        UL_GET_BASE_ADDRESS_FROM_STR_TDD0_S_SF_TX_NODE_ANCHOR(
                ulStrategyNodeVoid_p, ulStrategyTxNode_p) 

            /* Getting the ueContext */
            ueIndex = ulStrategyTxNode_p->ueIndex;
        uplinkContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

        /* Check if pendingDeleteFlag is true or ulUEContext_p is NULL*/
        if ((uplinkContextInfo_p->pendingDeleteFlag) || 
                (PNULL == ulUEContext_p))
        {
            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                    LOGERROR,MAC_UL_Strategy,\
                    getCurrentTick(),\
                    ulStrategyTxNode_p->ueIndex,\
                    0,0,0,0,0.0,0.0,__func__,
                    "pendingDeleteFlag is TRUE or Ue Context is NULL");

            /* Deleting the node from ulStrategyTdd0SsfTxListUSfOffset7_g List */
            /* CA TDD CHANGES START */
            listDeleteNode(&ulStrategyTdd0SsfTxListUSfOffset7_g[internalCellIndex],
                    &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);
            /* CA TDD CHANGES END */

            /* Get the next node */
            ulStrategyNodeVoid_p =(void *) getNextListNode(
                    &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);

#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
            FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
            freeMemPool(ulStrategyTxNode_p);
#endif
            continue;
        }

        /* Initialize strategy node's scheduled count
         * and push into PDCCH output Q.*/
        ulStrategyTxNode_p->scheduledCount = NUM_TWO;

        /* Reset the scheduledCount to 0 in UL UE Context */
        ulUEContext_p->scheduledCount = 0;

        /* Push the node in PDCCH output Queue */
        retVal = pushListNodeInULSchedulerNewTxQueue(
                ulStrategyTxNode_p,
                internalCellIndex);


        /* Deleting the node from ulStrategyTdd0SsfTxListUSfOffset7_g List */
        /* CA TDD CHANGES START */
        listDeleteNode(&ulStrategyTdd0SsfTxListUSfOffset7_g[internalCellIndex],
                &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);
        /* CA TDD CHANGES END */

        /* Get the next node */
        ulStrategyNodeVoid_p =(void *) getNextListNode(
                &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);

        if (MAC_FAILURE == retVal)
        {
            /* Reset the scheduledCount as 1 for the kth node
               Already pushed in PDCCH queue */
            if (MAC_FAILURE == 
                    updateSchCountInUlStrategyNewTxQueue(ulStrategyTxNode_p->ueIndex,
                        NUM_ONE,
                        internalCellIndex
                        ))
            {
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGERROR,MAC_UL_Strategy,\
                        getCurrentTick(),\
                        ulStrategyTxNode_p->ueIndex,\
                        0,0,0,0,0.0,0.0,__func__,
                        "updateSchCountInUlStrategyNewTxQueue failed");
            }
            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                    LOGERROR,MAC_UL_Strategy,\
                    getCurrentTick(),\
                    ulStrategyTxNode_p->ueIndex,\
                    0,0,0,0,0.0,0.0,__func__,
                    "ulStrategyTxNode_p entry failed");
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
            FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
            freeMemPool(ulStrategyTxNode_p);
#endif
        }
    }
}
/*****************************************************************************
 * Function Name    : isUeEligibleForSingleDCI0 
 * Inputs           : ULStrategyTxNode - Pointer to ULStrategyTxNode
 *                    ulUEContext_p - UL UE Context.
 *                    subFrameNum - DL Subframe Number
 *                    ulSubFrameNum - UL Subframe Number
 *                    sysFrameNum - System Frame Number
 * Outputs          : eligibleForSingleDCI0_p - If single DCI0 can be sent
 *                    then this value is TRUE else FALSE.
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function checks the UE if single DCI0 can be sent 
 *                    for both the UL subframe of the UE.
 *                    - If UE is UL MIMO Candidate
 *                      OR
 *                    - If numRB and NDI are not same for both schedule 
 *                      U Subframe instances
 *                      - UE Is not eligible for Single DCI0.
 *                      - Else Eligble for Singl DCI0
 *****************************************************************************/
STATIC  MacRetType isUeEligibleForSingleDCI0(
        ULStrategyTxNode *ulStrategyTxNode_p,
        ULUEContext     *ulUEContext_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt8 *eligibleForSingleDCI0_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
        )
{
    UInt8 retVal = LTE_FALSE;
    UInt8 eligibleForSingleDCI0 = LTE_TRUE;
    UInt8 ulHarqProcessId1stSF = BUSY_ALL_HARQ;
    UInt8 ulHarqProcessId2ndSF = BUSY_ALL_HARQ;
    UInt8 avgMCS = ulStrategyTxNode_p->avgMcs;
    UInt8 ulSubFrameNum = ulStrategyTxNode_p->ulSubFrameNum;
    UInt8 ulSubFrameNumOffset7 = ulSubFrameNum + 1;
    /* SPR 23393 Changes Start */
    UInt32 subFrameNum = MAX_SUB_FRAME;
    /* SPR 23393 Changes End */
    UInt32 sysFrameNum = 0;
    ULGrantRequestType ulGrantRequestType = ulStrategyTxNode_p->ulGrantRequestType;
    ULHarqInfo  *ulHarqInfo1stSF_p    = PNULL;
    ULHarqInfo  *ulHarqInfo2ndSF_p    = PNULL;
    ULUEContextInfo *ulUEContextInfo_p = 
        &ulUECtxInfoArr_g[ulStrategyTxNode_p->ueIndex];

    /* Check if UE is Eligible for UL MU MIMO */
    UL_MU_MIMO_CANDIDATE_SELECTION(
            ulUEContextInfo_p,ulUEContext_p,avgMCS,ulGrantRequestType,ulSubFrameNum,retVal,internalCellIndex);
    /* No Need to check UL MU MIMO for other UL SF as the value are 
     * same(avgMCS and ulGrantRequestType) */
     
    /* If UE is Not UL MIMO Candidate then check numRB and NDI for both UL SF */
    /* Check if numRB and NDI for both UL SF scheduled instancesi are same or not .*/
    if (LTE_FALSE == retVal) 
    {
        /* Get the Harq Process ID of both UL SF */
        ulHarqProcessId1stSF = ulStrategyTxNode_p->ulHarqProcessId;

        /* Getting the SFN and SF */
    /* SPR 23393 Changes Start */
        GET_SFN_SF_FROM_TTI(currentGlobalTTITickCount,sysFrameNum,subFrameNum);
    /* SPR 23393 Changes End */
        LTE_GCC_UNUSED_PARAM(sysFrameNum)
        /* Get the free ID in ulHarqProcessId */
        /* SPR 11257 Changes Start*/
    	ulHarqProcessId2ndSF = getFreeULHarqId(ulUEContext_p,subFrameNum,internalCellIndex,
				            ulSubFrameNumOffset7,currentGlobalTTITickCount);
        /* +- SPR 17777 */
        /* SPR 11257 Changes End*/
        if ( BUSY_ALL_HARQ == ulHarqProcessId2ndSF )
        {
            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                    LOGERROR,MAC_UL_Strategy,\
                    currentGlobalTTITickCount,\
                    ulStrategyTxNode_p->ueIndex,
#ifdef TDD_CONFIG
                    sysFrameNum,
#else
                    0,
#endif
                    0,0,0,0.0,0.0,__func__,
                    "ul Harq is Invalid NA ");
            deleteUlSfOffset7ListNode(ulStrategyTxNode_p->ueIndex, internalCellIndex);
            return MAC_FAILURE;
        }

        /* Get the UL Harq process of both the UL SF */
        ulHarqInfo1stSF_p = 
            &ulUEContext_p->ulresInfo[ulHarqProcessId1stSF];
        ulHarqInfo2ndSF_p = 
            &ulUEContext_p->ulresInfo[ulHarqProcessId2ndSF];

        /*  Check if numRB and NDI for both UL SF scheduled instances
         *  are same or not */
        if ((ulHarqInfo1stSF_p->newDataIndicator != 
                    ulHarqInfo2ndSF_p->newDataIndicator) ||
                (ulHarqInfo1stSF_p->requiredRB != 
                 ulHarqInfo2ndSF_p->requiredRB))
        {
            /* As numRB and NDI for both UL SF scheduled instances
             * are not same so not eligible for single DCI0 */
            eligibleForSingleDCI0 = LTE_FALSE;
        }
    }
    else
    {
        /* As UE Is Eligible for MU UE so not eligible for single DCI0 */
        eligibleForSingleDCI0 = LTE_FALSE;
    }
    /* Assigning the value to eligibleForSingleDCI0_p */
    *eligibleForSingleDCI0_p = eligibleForSingleDCI0;

    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name    :  processUlSfOffsetKSchCountTwoNodes
 * Inputs           :  ulStrategyTxNode_p - startegy node  
 *                     ulUEContext_p - UL UE context
 * Outputs          : UL Strategy Output Nodes are passed to PDCCH Module
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function processes the ulStrategyTdd0SsfTxList_g LIST
 *                    Nodes whose ulGrantRequestType is Not NEW_ULGRANT_REQ and
 *                    Scheduled count is 2 in UL UEContext.
 *                    - Check If UE is eligible For Single DCI0
 *                    - If UE is Eligible for Single DCI0
 *                      1. set strategy node's schCount to 2
 *                      2. Push the node in ulStrategyTxQueue_g Queue
 *                      3. Delete the node from ulStrategyTdd0SsfTxList_g List
 *                    - Else IF UE is not eligible For Single DCI0 
 *                      1.Change the UE's scheduledCount to 1
 *                      2.Do not Delete the node from ulStrategyTdd0SsfTxList_g
 *                        List
 *****************************************************************************/
STATIC  MacRetType  processUlSfOffsetKSchCountTwoNodes(
        ULStrategyTxNode *ulStrategyTxNode_p,
        ULUEContext     *ulUEContext_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
        )
{
    UInt8 eligibleForSingleDCI0 = LTE_FALSE;
    MacRetType retVal           = MAC_SUCCESS;

    /* Check if UE is Eligible for Single DCI0 for both
     * scheduled UL Subframes */
    if (MAC_FAILURE == isUeEligibleForSingleDCI0(
                ulStrategyTxNode_p,
                ulUEContext_p,
                currentGlobalTTITickCount,
                &eligibleForSingleDCI0,
                internalCellIndex
                ))
    {
        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\

                LOGERROR,MAC_UL_Strategy,\
                currentGlobalTTITickCount,\
                ulStrategyTxNode_p->ueIndex,\
                0,0,0,0,0.0,0.0,__func__,
                "Failure from isUeEligibleForSingleDCI0");


        /*Change the UE's scheduledCount to 1 and do not delete the node
         * from the LIST*/
        ulUEContext_p->scheduledCount = NUM_ONE;
        return MAC_FAILURE;
    }

    /* If eligibleForSingleDCI0 is True
     * - then UE is Eligible for single DCI0 for both 
     *   UL Subframe
     * Else if  retVal is False 
     * - Then separate DCI0 will be sent for both UL SF */
    if (eligibleForSingleDCI0)
    {
        /* initialize strategy node's schCount as 2,
         * delete it from the List ulStrategyTdd0SsfTxList_g and
         * push into  PDCCH output Q */ 
        ulStrategyTxNode_p->scheduledCount = NUM_TWO;

        /* Push the node in PDCCH output Queue */
        retVal = pushListNodeInULSchedulerNewTxQueue(
                ulStrategyTxNode_p,
                internalCellIndex);

        /* Deleting the node from ulStrategyTdd0SsfTxList_g List */
        /* CA TDD CHANGES START*/
        listDeleteNode(&ulStrategyTdd0SsfTxList_g[internalCellIndex],
        /* CA TDD CHANGES END*/
            &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);

        if (MAC_FAILURE == retVal)
        {
            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                    LOGERROR,MAC_UL_Strategy,\
                    currentGlobalTTITickCount,\
                    ulStrategyTxNode_p->ueIndex,\
                    0,0,0,0,0.0,0.0,__func__,
                    "ulStrategyTxNode_p entry failed");
            /*Change the UE's scheduledCount to 1 and do not delete the node
             * from the LIST*/
            ulUEContext_p->scheduledCount = NUM_ONE;
            return MAC_FAILURE;
        }
    }
    else
    {
        /*Change the UE's scheduledCount to 1 and do not delete the node
         * from the LIST*/
        ulUEContext_p->scheduledCount = NUM_ONE;
    }
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name    : processUlSfOffsetKListNodes 
 * Inputs           : ULStrategyTxNode void* of LIST ulStrategyTdd0SsfTxList_g
 *                    currentGlobalTTITickCount - Current tick
 * Outputs          : UL Strategy Output Nodes are passed to PDCCH Module
 * Returns          : void
 * Description      : This function processs the UL Offset K List nodes
 *                    with ulGrantRequestType is Not  NEW_ULGRANT_REQ and 
 *                    Scheduled count2 nodes
 *                    - If node's ulGrantRequestType is Not NEW_ULGRANT_REQ
 *                        1. initialize strategy node's schCount as 1
 *                        2. delete it from List ulStrategyTdd0SsfTxList_g
 *                        3. Pushes the Node into ulStrategyTxQueue_g which 
 *                           is used by PDCCH.
 *                    - For Nodes which have Scheduled count 2 in UlUEContext
 *                        1. If UE is UL MIMO Candidate. 
 *                           OR 
 *                           numRB and NDI are not same for both schedule 
 *                           UL SF instances
 *                             - Change the UE's scheduledCount to 1 
 *                             - Do not delete the node from the LIST.
 *                        2. Else
 *                          - initialize strategy node's schCount as 2
 *                          - Delete it from List 
 *                          - Pushed the Node into ulStrategyTxQueue_g Queue
 *
 *****************************************************************************/
STATIC  void processUlSfOffsetKListNodes(
        void *ulStrategyNodeVoid_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
        )
{
    UInt16     ueIndex  = 0;
    MacRetType retVal   = MAC_SUCCESS;
    ULStrategyTxNode *ulStrategyTxNode_p    = PNULL;
    ULUEContext      *ulUEContext_p         = PNULL;
    ULUEContextInfo  *uplinkContextInfo_p   = PNULL;

    /* Traverse the List ulStrategyTdd0SsfTxList_g */
    while (PNULL != ulStrategyNodeVoid_p)
    {
        /* Reseting the retVal with MAC_SUCCESS */
        retVal = MAC_SUCCESS;
        /* Get the base address */
        UL_GET_BASE_ADDRESS_FROM_STR_TDD0_S_SF_TX_NODE_ANCHOR(
                ulStrategyNodeVoid_p, ulStrategyTxNode_p) 

        /* Getting the ueContext */
        ueIndex = ulStrategyTxNode_p->ueIndex;
        uplinkContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

        /* Check if pendingDeleteFlag is true or ulUEContext_p is NULL*/
        if ((uplinkContextInfo_p->pendingDeleteFlag) || 
                (PNULL == ulUEContext_p))
        {
            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                    LOGERROR,MAC_UL_Strategy,\
                    currentGlobalTTITickCount,\
                    ulStrategyTxNode_p->ueIndex,\
                    0,0,0,0,0.0,0.0,__func__,
                    "pendingDeleteFlag True ");

            /* Deleting the node from ulStrategyTdd0SsfTxList_g List */
            /* CA TDD CHANGES START*/
            listDeleteNode(&ulStrategyTdd0SsfTxList_g[internalCellIndex],
            /* CA TDD CHANGES END */
                    &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);

            /* Get the next node */
            ulStrategyNodeVoid_p =(void *) getNextListNode(
                    &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);

#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
            FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
            freeMemPool(ulStrategyTxNode_p);
#endif
            continue;
        }

        /* If node is Not NEW_ULGRANT, initialize strategy node's 
         * schCount as 1, delete it from List ulStrategyTdd0SsfTxList_g  
         * and push into ulStrategyTxQueue_g queue.*/
        if (NEW_ULGRANT_REQ_MSG != ulStrategyTxNode_p->ulGrantRequestType)
        {
            ulStrategyTxNode_p->scheduledCount = NUM_ONE;

            /* Push the node in PDCCH output Queue */
            retVal = pushListNodeInULSchedulerNewTxQueue(
                    ulStrategyTxNode_p,
                    internalCellIndex);

            /* Deleting the node from ulStrategyTdd0SsfTxList_g List */
            /* CA TDD CHANGES START */
            listDeleteNode(&ulStrategyTdd0SsfTxList_g[internalCellIndex],
            /* CA TDD CHANGES END */
                    &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);

            if (MAC_FAILURE == retVal)
            {
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGERROR,MAC_UL_Strategy,\
                        currentGlobalTTITickCount,\
                        ulStrategyTxNode_p->ueIndex,\
                        0,0,0,0,0.0,0.0,__func__,
                        "ulStrategyTxNode_p entry failed");

                /* Get the next node */
                ulStrategyNodeVoid_p =(void *) getNextListNode(
                        &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);

#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
                freeMemPool(ulStrategyTxNode_p);
#endif
                continue;
            }
        }
        else
        {
            /* If ulGrantRequestType is NEW_ULGRANT_REQ 
             * - check if ulUEContext_p->scheduledCount is 2 
             * - If true then call processUlSfOffsetKSchCountTwoNodes */
            if (NUM_TWO == ulUEContext_p->scheduledCount)
            {
                retVal = processUlSfOffsetKSchCountTwoNodes(
                        ulStrategyTxNode_p,
                        ulUEContext_p,
                        currentGlobalTTITickCount,
                        /*CA Changes start  */
                        internalCellIndex
                        /*CA Changes end  */ 
                        );
                if (MAC_FAILURE == retVal)
                {
                    /* Get the next node */
                    ulStrategyNodeVoid_p =(void *) getNextListNode(
                            &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);

#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
                    FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
                    freeMemPool(ulStrategyTxNode_p);
#endif
                    continue;
                }

            }
        }

        /* Get the next node */
        ulStrategyNodeVoid_p =(void *) getNextListNode(
                &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);
    }
}
/*****************************************************************************
 * Function Name    : deleteUlSfOffset7ListNode
 * Inputs           : ueIndex, UE Index,internalCellIndex
 * Outputs          : 
 * Returns          : void 
 * Description      : This function will delete one sreategy node from 
 *                    ulStrategyTdd0SsfTxListUSfOffset7_g list for the UEIndex
 *                    passed to this function.
 *****************************************************************************/
STATIC void deleteUlSfOffset7ListNode(UInt16 ueIndex,
                            /* CA TDD CHANGES START */
                            InternalCellIndex internalCellIndex
                            /* CA TDD CHANGES END */
                            )
{
    ULStrategyTxNode *ulStrategyTxNode_p    = PNULL;
    void *ulStrategyNodeVoid_p              = PNULL;

    ulStrategyNodeVoid_p = (void *) getFirstListNode(
    /* Traverse the ulStrategyTdd0SsfTxListUSfOffset7_g List */
            /* CA TDD CHANGES START */
                &ulStrategyTdd0SsfTxListUSfOffset7_g[internalCellIndex]);
    /* CA TDD CHANGES END */
    while (PNULL != ulStrategyNodeVoid_p)
    {
        /* Get the base address */
        UL_GET_BASE_ADDRESS_FROM_STR_TDD0_S_SF_TX_NODE_ANCHOR(
                ulStrategyNodeVoid_p, ulStrategyTxNode_p)
        if (ueIndex == ulStrategyTxNode_p->ueIndex)
        {
            /* Deleting the node from ulStrategyTdd0SsfTxListUSfOffset7_g List */
            listDeleteNode(&ulStrategyTdd0SsfTxListUSfOffset7_g[internalCellIndex],
                    &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
            FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
            freeMemPool(ulStrategyTxNode_p);
#endif
            return;
        }
        /* Get the next node */
        ulStrategyNodeVoid_p =(void *) getNextListNode(
                &ulStrategyTxNode_p->ulStrategyTdd0SsfnTxNodeAnchor);
    }
}
/*****************************************************************************
 * Function Name    : mergeULStrategyNodes 
 * Inputs           : currentGlobalTTITickCount : current Global Tick,
 *                    ulDelay - The amount of delay expected in 
 *                              the UpLink path
 * Outputs          : UL Strategy Output Nodes are passed to PDCCH Module
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description    : This execution leg invoke mergeULStrategyNode function 
 *                  to merge the two UL Strtegy out put node Lists.
 *                  The Lists are populated by ULStartegy called for ulSubframe
 *                  +k and +7 respectively.
 *****************************************************************************/
UInt32 mergeULStrategyNodes(
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 

        )
{
    void *ulStrategyNodeVoid_p                         = PNULL;
    void *ulStrategyNodeOffset7Void_p                  = PNULL;

    /* UL starategy inserts strategy nodes in List ulStrategyTdd0SsfTxList_g
     * If U subframe is n+k and inserts in List ulStrategyTdd0SsfTxListUSfOffset7_g
     * If U subframe is n+7 */

    ulStrategyNodeVoid_p =(void *) getFirstListNode(
            /* CA TDD CHANGES START */
            &ulStrategyTdd0SsfTxList_g[internalCellIndex]
            /* CA TDD CHANGES END */
            );

    /* Traverse the List ulStrategyTdd0SsfTxList_g */
    if (PNULL != ulStrategyNodeVoid_p)
    {
        /* Process the NON_NEW_GRANT and Scheduled count 2 Nodes 
         * of ulStrategyTdd0SsfTxList_g LIST */
        processUlSfOffsetKListNodes(
                ulStrategyNodeVoid_p,
                currentGlobalTTITickCount,
                internalCellIndex);
    }

    /* Traverse the List ulStrategyTdd0SsfTxList_g and ulStrategyTdd0SsfTxListUSfOffset7_g
     * And process the Nodes with scheduled count 1 */
    processBothUlSfListNodes(internalCellIndex);

    /* Traverse the List ulStrategyTdd0SsfTxListUSfOffset7_g 
    In ulStrategyTdd0SsfTxListUSfOffset7_g only sch count 2 elemnets are left */
    ulStrategyNodeOffset7Void_p =(void *) getFirstListNode(
            &ulStrategyTdd0SsfTxListUSfOffset7_g[internalCellIndex]);
    if (PNULL != ulStrategyNodeOffset7Void_p)
    {
        processUlSfOffset7ListNodes(ulStrategyNodeOffset7Void_p,internalCellIndex);
    }
    return MAC_SUCCESS;
}
#endif

