/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLSchedulerStrategy.c,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This File is used to apply the DL Scheduling strategy 
                       to all the UEs which needs to be schedule depending upon 
                       the DL RBs requests. the Request can be for sending TA ,
                       DRX, Retransmission or for the fresh scheduling.  
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacDLSchedulerStrategy.c,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.50  2009/12/02 07:16:49  gur18550
 * Optimisation related changes
 *
 * Revision 1.49  2009/12/02 06:36:27  gur18550
 * Bug Solved for tempNewCalculatedRBs
 *
 * Revision 1.48  2009/11/15 12:57:14  gur11912
 * LTE_MAC_UT_LOG(LOG_WARNING) used instead of lteWarning
 *
 * Revision 1.47  2009/11/10 12:45:13  gur11912
 * modified for warning UE not present in non zero lists
 *
 * Revision 1.46  2009/10/06 06:11:29  gur11912
 * optimization changes
 *
 * Revision 1.45  2009/10/05 08:04:37  gur11912
 * optimization changes
 *
 * Revision 1.44  2009/08/04 15:25:12  gur20052
 * Added more LOGS related to subnband info
 *
 * Revision 1.43  2009/08/01 09:43:49  gur20052
 * UT Defects are Fixed
 *
 * Revision 1.42  2009/08/01 09:42:18  gur20052
 * Added the implementation of TA DRX,now cheking pendingTA/DRX Ack
 *
 * Revision 1.41  2009/07/31 06:32:13  gur15697
 * cleanup Bugfix
 *
 * Revision 1.40  2009/07/30 18:10:14  gur20052
 * UT Defects are fixed related to isAlreadyConsideredFlag
 *
 * Revision 1.39  2009/07/30 14:44:41  gur20052
 * UT Defects are fixed
 *
 * Revision 1.38  2009/07/30 12:28:55  gur20052
 * UT Defects are fixed
 *
 * Revision 1.37  2009/07/29 16:25:30  gur20052
 * UT Defect is fixed
 *
 * Revision 1.36  2009/07/27 06:09:22  gur20052
 * UT Defects are fixed
 *
 * Revision 1.35  2009/07/22 14:18:50  gur20052
 * UT Defect are fixed
 *
 * Revision 1.34  2009/07/20 17:45:04  gur20052
 * UT Defects are Fixed
 *
 * Revision 1.33  2009/07/16 17:36:14  gur20052
 * Added code for Setting up of aperiodicCQIRequested flag in DL UE Context and UT Bug Fix
 *
 * Revision 1.32  2009/07/15 05:59:30  gur20052
 * UT Defects fixed
 *
 * Revision 1.31  2009/07/13 14:06:30  gur20052
 * UT Bugs Fixed
 *
 * Revision 1.29  2009/07/10 13:50:38  gur20052
 * MACModuleLogDetail_g is replaced with the Module Specific STRINGS
 *
 * Revision 1.28  2009/07/07 13:28:42  gur20052
 * Updated for UT Defects
 *
 * Revision 1.27  2009/07/02 10:57:36  gur20052
 * Enhancement Comments are incorporated
 *
 * Revision 1.26  2009/06/29 15:09:13  gur20052
 * UT Defects are removed
 *
 * Revision 1.25  2009/06/25 06:45:05  gur20052
 * UT Defects are resolved
 *
 * Revision 1.24  2009/06/24 08:10:45  gur20052
 * Reveiw Comments are incorporated
 *
 * Revision 1.23  2009/06/23 16:55:42  gur20052
 * Review comments are incorporated
 *
 * Revision 1.21  2009/06/18 06:44:20  gur20052
 * UT Defects are removed
 *
 * Revision 1.20  2009/06/11 14:52:56  gur20052
 * Removed the Linking Errors and Resource Allocator Node
 *
 * Revision 1.18  2009/06/10 14:26:07  gur20052
 * Removed the Linking errors and updated for createMemPool changes
 *
 * Revision 1.17  2009/06/08 08:21:19  gur20052
 * Updated for the Review comments incorporations
 *
 * Revision 1.16  2009/06/05 16:56:28  gur20052
 * Incorporated the reveiw comments
 *
 * Revision 1.15  2009/06/05 16:20:23  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.14  2009/06/05 06:33:48  gur20052
 * Incorporated the reveiw comments
 *
 * Revision 1.13  2009/06/04 07:39:50  gur20052
 * Review Comments are incorporated
 *
 * Revision 1.12  2009/06/02 07:19:08  gur20052
 * Updated during comments incorporation and dispostion
 *
 * Revision 1.11  2009/05/28 11:33:01  gur20052
 * The changes for the UE contexts are done
 *
 * Revision 1.10  2009/05/26 10:16:04  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.9  2009/05/25 16:20:01  gur20052
 * Updated the version to support the UL Fair and Round Robin Scheduler
 *
 * Revision 1.8  2009/05/21 17:22:46  gur20052
 * Updated the Basic Framework of UL Strategy
 *
 * Revision 1.7  2009/05/21 06:24:55  gur20052
 * Added code for round robin Scheduler
 *
 * Revision 1.6  2009/05/20 13:02:47  gur20052
 *  Added The new Enums for message Type
 *
 * Revision 1.5  2009/05/20 08:01:45  gur20052
 * incorporated the review comments
 *
 * Revision 1.4  2009/05/19 14:29:31  gur20052
 * Updated with DL Fair Scheduler module specific changes
 *
 * Revision 1.3  2009/05/11 06:18:32  gur20052
 * Removed compilation errors
 *
 * Revision 1.2  2009/04/30 16:27:35  gur20052
 * Added initial version of DL Round Robin Scheduler
 *
 * Revision 1.1  2009/04/30 14:07:40  gur20052
 * Added initial version of  DL Scheduler Strategy
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteTypes.h"
#include "lteMacCellConfMgr.h"
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacDLScellStateManager.h"
#include "lteMacDLFairScheduler.h"
#include "lteMacUtilityAndSharedStructs.h"
#include "lteMacDLQosTokenAlgo.h"
#include "lteCommonStatsManager.h"
#include "lteMacDRXMgr.h" /* DRX_CHG */
#include "lteMacMeasGapHandler.h" /* MEAS_GAP_CHG */
#include "lteCommonStatsManager.h"
#include "lteMacDLResourceInfoMgr.h"
#include "lteMacDLSpsScheduler.h"
#include "lteMacDLHarqMgr.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
/* Start 128 UE: Memory Changes */
#include "lteMacTddMgr.h"
/* End 128 UE: Memory Changes */
#endif
#include "ltePerfStats.h"
#include "lteMacDLQosScheduler.h"
/* SPR 23402 Changes Start */
/* MOCN CHANGES START */
#include "lteMacOAMInterface.h"
/* MOCN CHANGES END */
/* SPR 23402 Changes End */


/******************************************************************************
 Private Definitions
 *****************************************************************************/
#define MAX_SPS_CIRC_Q 7

/******************************************************************************
 Private Types
 *****************************************************************************/

 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/
/* RETX_CHG */


MacRetType assignRBsForReTx(DLUEContext *dlUEContext_p, DLHARQProcess
        *dlHarqProcess_p, UInt32 *numberOfAvailableRBs_p, 
        UInt8 userLocationType, UInt8 schUsed,InternalCellIndex internalCellIndex);

/* + Rel_6_0_reTx_Changes */
/* SPR 21922 start end */
MacRetType lteMacStrategyAssignRBsForReTxFail( 
                                DLUEContext *dlUEContext_p,
                                DLHARQProcess *dlHarqProcess_p,
                                DLStrategyTxNode *dlStrategyTxNode_p,
                                InternalCellIndex internalCellIndex);

MacRetType lteMacStrategyDLNackTwoTBWithNonZeroQueueLoadForReTxFail( 
                                DLUEContext* ueDLContext_p,
                                DLHARQProcess *harqProcess_p,
                                TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo,
                                DLStrategyTxNode *dlStrategyTxNode_p,
                                InternalCellIndex internalCellIndex);

MacRetType lteMacStrategyDLNackTwoTBWithZeroQueueLoadForReTxFail( 
                                DLUEContext* ueDLContext_p,
                                DLHARQProcess *harqProcess_p,
                                TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo,
                                DLStrategyTxNode *dlStrategyTxNode_p,
                                InternalCellIndex internalCellIndex);

MacRetType lteMacStrategyDLNackForOneTBOnlyForReTxFail( 
                                DLUEContext* ueDLContext_p,
                                DLHARQProcess *harqProcess_p,
                                DLStrategyTxNode *dlStrategyTxNode_p,
                                InternalCellIndex internalCellIndex);

/* - Rel_6_0_reTx_Changes */

 /* SPR 2446 Changes End */
/*SPR 2446 Fix Begins*/
void processDLNackForTwoTBs( UInt32 *availableRB_p, 
        DLUEContext* ueDLContext_p, DLHARQProcess *harqProcess_p,
        TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo, UInt8 userLocationType);
void processDLNackForOneTBOnly( UInt32 *availableRB_p, 
        DLUEContext* ueDLContext_p, DLHARQProcess *harqProcess_p, 
        UInt8 userLocationType, UInt8 schUsed,
        InternalCellIndex internalCellIndex);
/* SPR 2446 Fix Ends*/
/* RETX_CHG */
void processDRXOnOffTriggerQueueDL(UInt32 drxQueueCount, /* DRX_CHG */
                                    InternalCellIndex internalCellIndex);
/* MEAS_GAP_CHG */
void processMeasGapOnOffTriggerQueueDL(UInt32 measGapQueueCount,
        /* +- SPR 17777 */
                                        InternalCellIndex internalCellIndex);
/* SPR 5599 changes start (ZIP ID 129193, 129105) */            
UInt8 putEntryInExplicitDLDCIScheduleQueue(UInt16 ueIdx, InternalCellIndex internalCellIndex);
STATIC void initDLQueueNodesAndLists (UInt8 numOfCells); 
STATIC void initializeDlSPSQueues (UInt8 cellCount); 
STATIC void initDlCAQueue( UInt8 cellCount ); 
extern void freeDLHarqProcess(DLUEContext*  ueContext_p,
/* SPR 5599 changes end (ZIP ID 129193, 129105) */            
                              DLHARQProcess* dlHarqProcess_p,
                              InternalCellIndex internalCellIndex);
void processSyncLossOnOffTriggerQueueDL(UInt32 ulSyncLossOnOffQueueCount,
        /* +- SPR 17777 */
                                        UInt32 dlDelay,
                                        InternalCellIndex internalCellIndex);
/* SPR 3679 Changes Start */
MacRetType processDLNackTwoTBWithNonZeroQueueLoad(
        /* +- SPR 17777 */
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo,
        /* SPR 5313 Fix start */
        UInt8 resourceRegionAllocated,
        /* SPR 5313 Fix end */
        InternalCellIndex internalCellIndex);

MacRetType processDLNackTwoTBWithZeroQueueLoad(
        /* +- SPR 17777 */
        UInt32 *availableRB_p, 
        DLUEContext* ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo,
        /* SPR 5313 Fix start */
        UInt8 resourceRegionAllocated,
        /*SPS_TDD_Changes*/
        UInt8 schType,
        /*SPS_TDD_Changes*/
        InternalCellIndex internalCellIndex);
        /* SPR 5313 Fix end */
/* SPR 3679 Changes End */
/* SRP 3608 changes start */
void putEntryInDLPriorityQueue( UInt32 ueIndex,
                                InternalCellIndex internalCellIndex);
/* SPR 3608 changes end */


/* Initialisation function for DL PFS Scheduler*/
extern void dlPfsInitMacSchedulerData(UInt8 cellCount);
/* SPR 11403 fix start */
MacRetType validateTxModeAndDciFormat(TransmissonMode transMode,DCIFormat dciFormat);
/* SPR 11403 fix end */

/* Cyclomatic_complexity_changes_start */
void updateNumAvailableRBsAndMaxDlUeSchedule(UInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
        UInt32 *totalNumberOfAvailableRBs_p,
        UInt8 maxUEsToBeScheduledDL[],
        UInt16 *totalMaxUEsToBeScheduledDL_p,
        UInt32 *tempNumOfAvailableRBs_p,
        UInt8 *tempMaxUEsToBeScheduledDL_p,
        InternalCellIndex internalCellIndex);
/* Cyclomatic_complexity_changes_start */
void putEntryInSCellSchedulingQueue( DLUEContext* ueDLContext_p, 
                                     DLHARQProcess* harqProcess_p,
                                     ServingCellIndex servingCellIndex);
/*CYCLOMATIC_CHANGES_6.1_START*/
STATIC  void determineDCIFormatForTxOtherThanThreeFour(DLStrategyTxNode *dlStrategyTxNode_p,
                                          UInt32 tempTransmissionMode);
STATIC  void determinePrimaryDCIFormatForTxThreeEight(DLStrategyTxNode *dlStrategyTxNode_p,
                                                            UInt32 tempTransmissionMode);
/*CYCLOMATIC_CHANGES_6.1_END*/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern UInt8  explicitTpcTriggerEnabled_g[MAX_NUM_CELL];

/* SPR 3679 QOS Changes Start */
/* Start of change for VARIBLE Declaration */
extern UInt32 totalRbMIMO_g[MAX_NUM_CELL];
/* End of change for VARIBLE Declaration */
extern DLStrategyScheduleInfo dlStrategyScheduleInfo_g[STRATEGY_MAX_CONTAINER_INDEX][MAX_NUM_CELL];
/* SPR 3679 QOS Changes End */


/*Queue of type LTE_CIRCQUEUE which contains indexes of UEs having TA Requests*/
extern TA_Q_TYPE* dlTimeAdvanceQueue_gp[MAX_NUM_CELL];

/*Queue of type LTE_CIRCQUEUE which contains indexes of UEs having DRX Requests*/
extern DRX_Q_TYPE dlDRXQueue_g;
/* Queue of LTE_CIRCQUEUE type is used to schedule DCI Format 1x/2x for those 
 * UEs whose scheduling is failed in first attempt
 */
EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE* explicitDLPucchTpcScheduleFailureQueue_gp[MAX_NUM_CELL] = {PNULL};
/* CLPC CHANGES */
/* Queue of LTE_CIRCQUEUE type is used to schedule DCI Format 1x/2x for those 
 * UEs whose TPC need to be triggered and sending of DCI Format 3A has failed
 */
EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE* explicitDLPucchTpcScheduleQueue_gp[MAX_NUM_CELL] = {PNULL};
/*Queue of type LTE_CIRCQUEUE which contains indexes of UEs having pending DRX 
  Request*/
DRX_Q_TYPE* pendingDRXQueue_gp[MAX_NUM_CELL] = {PNULL};

/*Queue of type LTE_CIRCQUEUE which contains indexes of UEs having pending TA 
  Request*/
TA_Q_TYPE* pendingTAQueue_gp[MAX_NUM_CELL] = {PNULL};

/*Qeeue of type LTE_SQUEUE which contains Triggers of UEs trigger to enter/to 
  remove from / into the NonZeroQueueLoadAndHarqList */
/* CirQ changes start */
DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_TYPE *dlUEZeroNonZeroQueueLoadTriggerQueue_gp[MAX_NUM_CELL];
/* CirQ changes end */

/* ICIC Change Start */
LTE_SQUEUE *advanceAllocInfoQueue_gp[MAX_NUM_CELL];
/* ICIC Change end */

/* A list of UEs which have non zero queueLoad or UEs whose queueLoad is zero 
   but their harq processes are active*/
LTE_LIST  *dlUENonZeroQueueLoadAndHarqList_gp[MAX_NUM_CELL];

/*Qeeue of type LTE_CIRCQUEUE which contains Triggers of UEs trigger to enter/to
 * remove from / into the NonZeroQueueLoadAndHarqList */
DL_DRX_ON_OFF_TRIGGER_TYPE* dlDRXOnOffTriggerQueue_gp[MAX_NUM_CELL];
DL_MEAS_GAP_ON_OFF_TRIGGER_TYPE* dlMeasGapOnOffTriggerQueue_gp[MAX_NUM_CELL] = {PNULL};


/* SRP 3608 changes start */
/* SPR 6897 changes start */
LTE_CIRCQUEUE *dlPriorityQueue_gp[MAX_NUM_CELL];
/* SPR 6897 changes end */
/* SPR 3608 changes end */
/*Used for checking whether the UE is considered by Scheduler strategy already 
  or not.If UE's rbsReservedForUE is equal to dlReferenceFlag, that means the UE 
  is already considered else yet to considered*/
/* SPR 15909 fix start */
tickType_t dlReferenceFlag_g[MAX_NUM_CELL] = {0};
/* SPR 15909 fix end */
STATIC UInt8 alreadyScheduledUEs[MAX_NUM_CELL] = {0};

#ifdef OVERSHOOT_CORRECTION
/*Define as enum variable with different enums for different levels like OVERLOAD_LEVEL_0*/
/** CA-TDD Changes Start **/
extern OverLoadLevel macOverLoadLevel_g[MAX_NUM_CELL];
/** CA-TDD Changes Stop **/
/* Temporarily Declaration */
#endif 


/* ICIC changes start */
UInt32 maxCellEdgeUEsToBeScheduledDL_g[MAX_NUM_CELL];
/* MIB nRB FIX start */
extern UInt8   mibOverlapRBs_g[MAX_NUM_CELL][MAX_USER_LOCATION];
/* MIB nRB FIX end */
/* ICIC changes end */

/*+ Dyn UEs To Be SchPerTTI +*/
/* + PERF_CA GBL */
UInt16 totalActiveUEsDL_g[MAX_NUM_CELL] = {0};
/* - PERF_CA GBL */
/*- Dyn UEs To Be SchPerTTI -*/
extern DLResourceAllocationInfo *dlRAInfo_gp[MAX_NUM_CELL];
extern UInt8 dlRAInfoIndex_g[MAX_NUM_CELL];
#define SPS_QUEUES_COUNT 7

DlQoSPreferredCellList dlQoSPreferredCellList_g[MAX_NUM_CELL];

extern  InternalCellIndex getInternalCellIdFromServeCellId( 
                                        DLUEContext* ueDLContext_p, 
                                        InternalCellIndex servCellIndex);

extern  void dlQosAddCellInPreferredCellList( 
                                        InternalCellIndex pCellIndex,
                                        ServingCellIndex servingCellIndex);

STATIC  void dlQosPreparePreferredCellListForReTxFail( 
                                        DLUEContext* dlUEContext_p, 
                                        InternalCellIndex cellIdTobeAdded);

STATIC  void dlQosPreparePreferredCellListForReTx( 
                                        DLUEContext* dlUEContext_p, 
                                        DLRetxQueueNode* dlReTxNode_p);

STATIC  MacRetType isNodeValidForRetransmissionOnCell(
                                        UInt16 ueIndex,
                                        UInt32 *numberOfAvailableRBs_p,
                                        UInt8 *deleteNode,
                                        UInt8 *resourceRegionAllocated_p,
                                        /*SPR 15909 fix start*/
                                        tickType_t globalTick
                                        /*SPR 15909 fix end*/
                                        #ifdef FDD_CONFIG
                                        ,UInt32 dlDelay
                                        #endif
                                        ,InternalCellIndex internalCellIndex,
                                        DLHARQProcess *dlHarqProcess_p,
                                        UInt8 maxUEsToBeScheduledDL);

STATIC  void  addCellInPrefListForReTx( 
                                        DLHARQEntity* dlHarqEntity_p,
                                        UInt8 harqProcessId,
                                        InternalCellIndex pCellIndex, 
                                        InternalCellIndex cellIdTobeAdded);

#ifdef FDD_CONFIG
extern void putEntryInSCellSchedulingQueue( 
                                DLUEContext* ueDLContext_p, 
                                DLHARQProcess* harqProcess_p,
                                ServingCellIndex servingCellIndex);
#endif                    
/* SPR 15446 Fix Start */
UInt32 allocateRbBasedOnQload(
        DLUEContext* ueDLContext_p,
        UInt32 availableRB,
        UInt8 numTB);
UInt16 dlSchedulableUECount_g = 0;  
/* SPR 15446 Fix End */
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
extern mocnOperatorParams mocnOperatorParams_g;
/* MOCN Changes End */
/* SPR 23402 Changes End */

/* EICIC +*/
extern UInt8 	absPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
extern UInt8 	usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
extern UInt8  	isEicicEnabled_g[MAX_NUM_CELL];
extern UInt8 	redistributeRb_g ;
UInt8 		ueTypeToScheduleDL_g[MAX_NUM_CELL] = {NON_VICTIM_UE};
extern UInt32 		phyDelay_g ;
extern UInt8 absPatternIndex_g[MAX_NUM_CELL];
extern  UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL];
/* EICIC -*/



/*Complexity Start*/
/*****************************************************************************
 * Function Name  : eicicUpdateSfAndSfn 
 * Inputs         : sysFrameNum - pointer to System Frame number
 *                  subFrameNum-  Pointer to Subframe number
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates System Frame Number and Subframe
 *                  Number.
 ******************************************************************************/
void eicicUpdateSfAndSfn(UInt32 *sysFrameNum,UInt32 *subFrameNum)
{
    if(*subFrameNum >= MAX_SUB_FRAME)
    {
        *subFrameNum -= MAX_SUB_FRAME;
        (*sysFrameNum)++;
        if (*sysFrameNum == MAX_SFN_VALUE)
        {
            *sysFrameNum = 0;
        }
    }
}

/*Complexity End*/



/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : processSyncLossOnOffTriggerQueueDL 
 * Inputs         : ulSyncLossQueueCount - Nodes to be processed from the queue
 *                  currentGlobalTTITickCount - current global tick
 *                  dlDelay - delay in dl path to phy,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Sync Loss ON/ OFF trigger queue
 *                  and based on the trigger it adds or deletes the UE from the 
 *                  active UE list.  
 *****************************************************************************/
void processSyncLossOnOffTriggerQueueDL(UInt32 ulSyncLossQueueCount, 
        /* +- SPR 17777 */
                                        UInt32 dlDelay,
                                        InternalCellIndex internalCellIndex)
{
    UInt16 queueCount = ulSyncLossQueueCount;
    UInt16 ueIndex = 0;
    UlSyncLossOnOff ulSyncLossOnOffTrigger = UL_SYNC_LOSS_INVALID;
    ULSyncLossQueueNode *ulSyncLossQueueNode_p = PNULL;
    DLUEContext *dlUEContext_p  = PNULL;  
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    LTE_LIST_NODE *activeUENode_p = PNULL;
    LTE_LIST_NODE *activeUENodeTempDeleted_p = PNULL;
#ifdef LOG_PRINT_ENABLED    
    MeasGapContextDL *measGapContext_p = PNULL;
    DRXContext *drxContext_p = PNULL;
#endif    

    /* + SPS_TDD_Changes */
    LP_SpsDlInfo spsDlInfo_p = PNULL;
    /* - SPS_TDD_Changes */
    /* +- SPR 17777 */
#ifdef LOG_PRINT_ENABLED    
    UInt32 state = 0;
#endif
    /* +- SPR 17777 */

    while (queueCount)
    {
        DEQUEUE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ULSyncLossQueueNode,(void **)&ulSyncLossQueueNode_p, internalCellIndex);
        if (ulSyncLossQueueNode_p)
        {
            ueIndex = ulSyncLossQueueNode_p->ueIndex;
            dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

            /*Checking the UE Context & Checking for Pending delete Flag 
              of UE*/
            if (!(dlUEContextInfo_p->pendingDeleteFlag))
            {
                dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;


		    /* EICIC +*/
		    /* Pushing UE back in trigger queue.*/
/*Complexity Start*/
		    dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

		    if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                    (NON_VICTIM_UE == dlUEContext_p->userType))||
                    ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                    (VICTIM_UE == dlUEContext_p->userType)))
		    {              
			    if(!ENQUEUE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ULSyncLossQueueNode,
						    &(ulSyncLossQueueNode_p->anchorNode), internalCellIndex))
			    {
				    FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ulSyncLossQueueNode_p);
			    }

			    queueCount--;   
			    continue;
		    }
/*Complexity End*/
		    /* EICIC - */
                activeUENode_p =
                    dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p;
                activeUENodeTempDeleted_p =
                    dlUEContext_p->activeUENodeTempDeleted_p;
                ulSyncLossOnOffTrigger =
                    ulSyncLossQueueNode_p->onOffTrigger;
#ifdef LOG_PRINT_ENABLED    
                measGapContext_p = &dlUEContext_p->measGapContext;
                drxContext_p = &dlUEContext_p->drxContext;
#endif
                switch(ulSyncLossOnOffTrigger)
                {
                    case UL_SYNC_LOSS_ON:
                    if (activeUENode_p)
                    {
                        LOG_MAC_MSG(MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_DL_LOG_ID,
                                LOGDEBUG,
                                MAC_DL_Strategy, 
                                getCurrentTick(),
                                dlUEContext_p->ueIndex, 
                                ulSyncLossOnOffTrigger,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE, 
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"ULSYNCLOSSON");

                        /*Store the node pointer in DLUEContext to add it
                         * later back in the active UE list*/
                        dlUEContext_p->activeUENodeTempDeleted_p =
                            dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p;

                        /* The Node in the dlUENonZeroQueueLoadAndHarqList
                         * LIST will be deleted */
                        deleteEntryFromUENonZeroQueueLoadAndHarqListTemporarily(
                                dlUEContext_p,
                                internalCellIndex);

                        dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p
                            = PNULL;
                    }
                    else
                    {
                        LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                dlUEContext_p->ueIndex,
                                dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                CHECK_UE_DRX_STATE(drxContext_p),
                                CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                LINE_NUMBER,
                                DEFAULT_FLOAT_VALUE, 
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"UL_DRX_MEAS_STATE");
                    }
                    break;

                    case UL_SYNC_LOSS_OFF:
                    {
                        /* + SPS_TDD_Changes */
                        spsDlInfo_p = &dlUEContext_p->spsDlInfo;
                        /* Call SPS state machine to change the state for SPS
                         * strategy */
                        if(spsDlInfo_p->spsState != INVALID_STATE)
                        {
#ifdef LOG_PRINT_ENABLED    
                            state = spsDlInfo_p->spsState;
#endif
                            dlStateMachine[spsDlInfo_p->spsState]
                                [UE_SYNC_LOSS_OFF](
                                        dlUEContext_p->ueIndex, spsDlInfo_p->spsState,
                                        UE_SYNC_LOSS_OFF, dlUEContext_p, PNULL, 
                                        /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                                        dlDelay
#else
                                        DL_ADVANCE_TICK
#endif
                                        /* - SPS_TDD_Changes */
                                        ,internalCellIndex);
                            LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                                 getCurrentTick(), __LINE__, dlUEContext_p->ueIndex, state, UE_SYNC_LOSS_OFF,
                                 spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                        }
                        /* - SPS_TDD_Changes */
                        dlUEContext_p->ulSyncLossOrHOInfoInDL.preambleIdx = INVALID_PREAMBLE_IDX;
                        dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState = UL_SYNC_LOSS_OFF;
                        if(activeUENodeTempDeleted_p)
                        {
                            if (0 == dlUEContext_p->ueQueueLoad)
                            {
                                /* SPR 12284 - Removed line */

                                /* SPR_8524_FIX_START */
                                /*deleteEntryFromUENonZeroQueueLoadAndHarqList(
                                  ueIndex, dlUEContext_p);*/
                                /* SPR_8524_FIX_END */
                                /* SPR 12284 changes start */
                                if( PNULL != dlUEContext_p->
                                        activeUENodeTempDeleted_p )
                                {
                                    freeMemPool( (void *)dlUEContext_p->
                                            activeUENodeTempDeleted_p);
                                    /* SPR 12284 changes end */
                                    dlUEContext_p->activeUENodeTempDeleted_p = PNULL;
                                }

                                FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ulSyncLossQueueNode_p);
                                queueCount--;
                                continue;
                            }

                            LOG_MAC_MSG(MAC_UE_ADDED_IN_ACTIVE_LIST_UL_SYNC_LOSS,
                                    LOGWARNING,
                                    MAC_DL_Strategy, 
                                    getCurrentTick(),
                                    dlUEContext_p->ueIndex, 
                                    ulSyncLossOnOffTrigger,
                                    LINE_NUMBER,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"ULSYNCLOSSOFF");

                            /* We need to insert node in the Active UE List*/
                            listInsertNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex], 
                                    activeUENodeTempDeleted_p);

                            dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p =
                                dlUEContext_p->activeUENodeTempDeleted_p;
                            dlUEContext_p->activeUENodeTempDeleted_p = PNULL;
                        }
                        else
                        {
                            LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                    LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex,
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                    CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    LINE_NUMBER,
                                    dlDelay, 
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"UL_DRX_MEAS_STATE");

                        }

                        if (((PNULL == dlUEContext_p->
                                        dlUENonZeroQueueLoadAndHarqList_p)
                                    && dlUEContext_p->ueQueueLoad != 0))
                        {
                            putEntryInUENonZeroQueueLoadAndHarqList(
                                    ueIndex, dlUEContext_p, internalCellIndex);
                        }

                        break;
                    }
                    default:
                    /* Log error */
                    break;
                }
            }
            FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ulSyncLossQueueNode_p);
        }
        queueCount--;
    }/*End of while loop*/

    return;
}
                        
/*****************************************************************************
 * Function Name  : processMeasGapOnOffTriggerQueueDL 
 * Inputs         : measGapQueueCount - measurement gap queue count to be processed
 *                  currentGlobalTTITickCount -current global tick,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Meas Gap ON/ OFF trigger queue 
 *                  and based on the trigger it adds or deletes the UE from the 
 *                  active UE list  
 *****************************************************************************/
void processMeasGapOnOffTriggerQueueDL(UInt32 measGapQueueCount,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    UInt16                      queueCount = measGapQueueCount;
    UInt16                      ueIndex = 0;
    MeasGapTrigger              measGapOnOffTrigger = MEASUREMENT_PERIOD_INVALID;
    MeasGapOnOffTriggerNode     *measGapOnOffTriggerNode_p = PNULL;
    DLUEContext                 *dlUEContext_p  = PNULL;  
    DLUEContextInfo             *dlUEContextInfo_p = PNULL;
    LTE_LIST_NODE               *activeUENode_p = PNULL;
    LTE_LIST_NODE               *activeUENodeTempDeleted_p = PNULL;
    DRXContext                  *drxContext_p = PNULL;
/*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED    
    MeasGapContextDL *measGapContext_p = PNULL;
#endif    
/*533-GCC-warning-fix*/
    InitParams *initParams_p = cellSpecificParams_g.\
                               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;

    while (queueCount--)
    {
        DEQUEUE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode, (void *)&measGapOnOffTriggerNode_p, internalCellIndex);

        if (measGapOnOffTriggerNode_p)
        {
            ueIndex = measGapOnOffTriggerNode_p->ueIndex;
            dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            
            /*Checking the UE Context & Checking for Pending 
              delete Flag of UE*/
            if (!(dlUEContextInfo_p->pendingDeleteFlag))
            {
                dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
		    /* EICIC +*/
/*Complexity Start*/
		    /* Pushing UE back in trigger queue.*/

		    if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                   (NON_VICTIM_UE == dlUEContext_p->userType))||
                   ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                    (VICTIM_UE == dlUEContext_p->userType)))
		    {              
			    if(!ENQUEUE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
						    &(measGapOnOffTriggerNode_p->anchorNode), internalCellIndex))
			    {
				    FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
			    }
			    continue;
		    }
/*Complexity End*/
		    /* EICIC - */
                drxContext_p = &dlUEContext_p->drxContext;
                activeUENode_p = dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p;
                activeUENodeTempDeleted_p =
                    dlUEContext_p->activeUENodeTempDeleted_p;
                measGapOnOffTrigger = measGapOnOffTriggerNode_p->onOffTrigger;
/*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED    
                measGapContext_p = &dlUEContext_p->measGapContext;
#endif                
/*533-GCC-warning-fix*/
                switch(measGapOnOffTrigger)
                {
                    case MEASUREMENT_PERIOD_ON_DL:
                    
                        /* This code is only for testing purpose. To be removed
                         * after testing */
                        /* HO_TEST_CODE */
                        if (UL_SYNC_LOSS_ON == dlUEContext_p->
                                ulSyncLossOrHOInfoInDL.ulSyncLossState)  
                        {
                            if ( PNULL == activeUENodeTempDeleted_p)
                            {
                                lteWarning("Invalid Scenario. UL Synch Loss "
                                        "is On. And UE is still in Active "
                                        "UE list\n");
                            }
                            LOG_MAC_MSG(MAC_UL_SYNC_LOSS_DRX_MEAS, LOGDEBUG,
                                    MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex, measGapOnOffTrigger,
                                    UL_SYNC_LOSS_ON, DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                    __func__,"UL_SYNC_LOSS_ON_MEAS_GAP_ON_DL");
                            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
                            continue;
                        }
                        /* HO_TEST_CODE */
                        
                        /* The Node in the dlUENonZeroQueueLoadAndHarqList LIST 
                           will be deleted */
                        if (activeUENode_p)
                        {   
                            /* SPR 2377 changes start */
                            LOG_MAC_MSG(MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_DL_LOG_ID 
                                    ,LOGDEBUG, MAC_DL_Strategy, 
                                    getCurrentTick(), dlUEContext_p->ueIndex, 
                                    measGapOnOffTrigger,
                                    LINE_NUMBER, DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"MEASUREMENTGAPON");
                             
                            /* SPR 2377 changes end */
                            
                            dlUEContext_p->activeUENodeTempDeleted_p = 
                              dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p;
                            
                            deleteEntryFromUENonZeroQueueLoadAndHarqListTemporarily(
                                    dlUEContext_p,
                                    internalCellIndex);
                            
                            dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p = 
                                PNULL; 
                        }
                        else
                        {
                            LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                    LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex,
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                    CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    LINE_NUMBER,
                                    DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"UL_DRX_MEAS_STATE");
                        }
                        
                        break;

                    case MEASUREMENT_PERIOD_OFF_DL:

                        if (DRX_STATE_ON == CHECK_UE_DRX_STATE(drxContext_p)) 
                        {
                            /* Dont do anything and proceed towards next node 
                             */
                            LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                    LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex,
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                    CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    LINE_NUMBER,
                                    DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"UL_DRX_MEAS_STATE");
                           
                            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
                            continue;
                        }

                        if (UL_SYNC_LOSS_ON == dlUEContext_p->
                                    ulSyncLossOrHOInfoInDL.ulSyncLossState)
                        {
                            if ((TRUE == dlUEContext_p->ulSyncLossOrHOInfoInDL.
                                        pendingPDCCHOrder))
                            {
                                putEntryInPdcchOrderQ(dlUEContext_p->ulSyncLossOrHOInfoInDL.preambleIdx, \
                                        dlUEContext_p->ueIndex, getCurrentTick()+ PHY_DL_DELAY,
                                        internalCellIndex);
                                dlUEContext_p->ulSyncLossOrHOInfoInDL.
                                    pendingPDCCHOrder = FALSE;
                                LOG_MAC_MSG(MAC_UL_SYNC_LOSS_DRX_MEAS, LOGDEBUG,
                                        MAC_DL_Strategy, getCurrentTick(),
                                        dlUEContext_p->ueIndex, measGapOnOffTrigger,
                                        UL_SYNC_LOSS_ON, DEFAULT_INT_VALUE,
                                        FALSE,
                                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                        __func__,"SYNCLOSS_ON_MEAS_OFF_PDCCH_TRIG");  
                            }
                            LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                    LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex,
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                    CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    LINE_NUMBER,
                                    DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"UL_DRX_MEAS_STATE");

                            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
                            continue;
                               
                        }
                        /* + E_CID_5.3 */
                        if(TRUE == dlUEContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder)
                        {
                            putEntryInPdcchOrderQ(
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.preambleIdx, \
                                    dlUEContext_p->ueIndex, getCurrentTick()+ PHY_DL_DELAY,
                                    internalCellIndex);
                            dlUEContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder = FALSE;
                        }
                        /* - E_CID_5.3 */

                        if (activeUENodeTempDeleted_p)
                        {
                            if (0 == dlUEContext_p->ueQueueLoad)
                            {
                                freeMemPool((void *)dlUEContext_p->
                                        activeUENodeTempDeleted_p);

                                dlUEContext_p->activeUENodeTempDeleted_p = PNULL;

                                FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
                                continue;
                            }

                            /* SPR 2377 changes start */
                            LOG_MAC_MSG(MAC_UE_ADDED_IN_ACTIVE_LIST_IN_DL_LOG_ID, 
                                    LOGDEBUG, MAC_DL_Strategy, 
                                    getCurrentTick(), dlUEContext_p->ueIndex, 
                                    measGapOnOffTrigger,
                                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE, __func__,"");
                          
                            /* SPR 2377 changes end */
                            
                            if(DL_FAIR_SCHEDULING == 
                                    initParams_p->dlSchdConfig.dlSchedulerStrategy)
                            {
                                listInsertNodeAtHead(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex],
                                                     activeUENodeTempDeleted_p);
                            }
                            else
                            {
                                listInsertNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex], 
                                    activeUENodeTempDeleted_p);
                            }

                            dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p = 
                                dlUEContext_p->activeUENodeTempDeleted_p;
                            dlUEContext_p->activeUENodeTempDeleted_p = PNULL;
                        }
                        else
                        {
                            LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                    LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex,
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                    CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    LINE_NUMBER,
                                    DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"UL_DRX_MEAS_STATE");
                        }
                        if (((PNULL == dlUEContext_p->
                                        dlUENonZeroQueueLoadAndHarqList_p) &&
                                    0 != dlUEContext_p->ueQueueLoad)) 
                        {
                            putEntryInUENonZeroQueueLoadAndHarqList(
                                    ueIndex, dlUEContext_p, internalCellIndex);
                        }

                       break;
                       
                    default:
                       /* Log error */
                       break;
                }
            }
            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
            /*SPR 4017 PH2 End*/
        }
    }

    return;
}

/* DRX_CHG */
/*****************************************************************************
 * Function Name  : processDRXOnOffTriggerQueueDL 
 * Inputs         : drxQueueCount - the count of DRX to be processed,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the DRX ON/ OFF trigger queue and
 *                  based on the trigger it adds or deletes the UE from the 
 *                  active UE list  
 *****************************************************************************/
void processDRXOnOffTriggerQueueDL(UInt32 drxQueueCount,InternalCellIndex internalCellIndex)
{
    UInt16                  queueCount = drxQueueCount;
    UInt16                  ueIndex = 0;
    DRXOnOffTrigger         drxOnOffTrigger = DRX_TRIGGER_INVALID;
    DRXOnOffTriggerNode     *drxOnOffTriggerNode_p = PNULL;
    DLUEContext             *dlUEContext_p  = PNULL;  
    DLUEContextInfo         *dlUEContextInfo_p = PNULL;
    LTE_LIST_NODE           *activeUENode_p = PNULL;
    LTE_LIST_NODE           *activeUENodeTempDeleted_p = PNULL;
    MeasGapContextDL          *measGapContext_p = PNULL;
/*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED    
    DRXContext                  *drxContext_p = PNULL;
#endif    
/*533-GCC-warning-fix*/
    InitParams *initParams_p = cellSpecificParams_g.\
                               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    while (queueCount--)
    {
        DEQUEUE_DL_DRX_ON_OFF_TRIGGER_Q(DRXOnOffTriggerNode, (void *)&drxOnOffTriggerNode_p,internalCellIndex);
        if (drxOnOffTriggerNode_p)
        {
            ueIndex = drxOnOffTriggerNode_p->ueIndex;
            dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            
            /*Checking the UE Context & Checking for Pending 
              delete Flag of UE*/
            if (!(dlUEContextInfo_p->pendingDeleteFlag))
            {
                dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
		    /* EICIC +*/
		    /* Pushing UE back in trigger queue.*/
/*Complexity Start*/
		    if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                    (NON_VICTIM_UE == dlUEContext_p->userType))||
                    ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                   (VICTIM_UE == dlUEContext_p->userType)))
		    {              
			    if(!ENQUEUE_DL_DRX_ON_OFF_TRIGGER_Q(DRXOnOffTriggerNode, 
						    &(drxOnOffTriggerNode_p->anchorNode), internalCellIndex))
			    {
				    FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
			    } 

			    
			    continue;
		    }
/*Complexity End*/
		    /* EICIC - */
                measGapContext_p = &dlUEContext_p->measGapContext;
                activeUENode_p = 
                    dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p;
                activeUENodeTempDeleted_p =
                    dlUEContext_p->activeUENodeTempDeleted_p;
                drxOnOffTrigger = drxOnOffTriggerNode_p->onOffTrigger;
/*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED    
                drxContext_p = &dlUEContext_p->drxContext;
#endif    
/*533-GCC-warning-fix*/
                /*START : 5352*/
                LOG_MAC_MSG(MAC_DL_DRX_TRIGGER_INFO,LOGDEBUG,
                    MAC_DL_Strategy, getCurrentTick(),
                    dlUEContext_p->ueIndex, drxOnOffTrigger,
                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__,"");  
                /*END   : 5352*/
                switch(drxOnOffTrigger)
                {
                    case DRX_OFF_TO_DRX_ON:

                        /* This code is only for testing purpose. To be removed
                         * after testing */
                        /* HO_TEST_CODE */
                        if (UL_SYNC_LOSS_ON == dlUEContext_p->
                                ulSyncLossOrHOInfoInDL.ulSyncLossState) 
                        {
                            if (PNULL == activeUENodeTempDeleted_p)
                            {
                                lteWarning("Invalid Scenario. UL Synch Loss "
                                        "is On. And UE is still in Active "
                                        "UE list\n");
                            }
                            LOG_MAC_MSG(MAC_UL_SYNC_LOSS_DRX_MEAS, LOGDEBUG,
                                    MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex, drxOnOffTrigger,
                                    UL_SYNC_LOSS_ON, DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                    __func__,"UL_SYNC_LOSS_ON_DRX_ON");  
                            FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
                            continue;
                        }
                        /* HO_TEST_CODE */

                        if (activeUENode_p)
                        {   
                            /* SPR 2377 changes start */
                            LOG_MAC_MSG(MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_DL_LOG_ID 
                                    ,LOGDEBUG, MAC_DL_Strategy, 
                                    getCurrentTick(), dlUEContext_p->ueIndex, 
                                    drxOnOffTrigger,
                                    LINE_NUMBER, DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"DRXON");
                           
                            /* SPR 2377 changes end */
                            
                            /* The Node in the dlUENonZeroQueueLoadAndHarqList 
                               LIST will be deleted */
                            dlUEContext_p->activeUENodeTempDeleted_p = 
                              dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p;
                            
                            deleteEntryFromUENonZeroQueueLoadAndHarqListTemporarily(
                                    dlUEContext_p,internalCellIndex);
                            
                            dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p = 
                                PNULL; 
                        }
                        else
                        {
                            LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                    LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex,
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                    CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    LINE_NUMBER,
                                    DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"UL_DRX_MEAS_STATE");
                            
                        }

                        break;

                    case DRX_ON_TO_DRX_OFF:

                        if (TRUE == CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p))
                        {
                            LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                    LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex,
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                    CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    LINE_NUMBER,
                                    DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"UL_DRX_MEAS_STATE");
                           
                            FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
                            continue;
                        }
                       
                        if (UL_SYNC_LOSS_ON == dlUEContext_p->
                                    ulSyncLossOrHOInfoInDL.ulSyncLossState)
                        {
                            if ((TRUE == dlUEContext_p->ulSyncLossOrHOInfoInDL.
                                        pendingPDCCHOrder))
                            {
                                putEntryInPdcchOrderQ(
                                        dlUEContext_p->ulSyncLossOrHOInfoInDL.preambleIdx, \
                                        dlUEContext_p->ueIndex, getCurrentTick()+ PHY_DL_DELAY,
                                        internalCellIndex);
                                dlUEContext_p->ulSyncLossOrHOInfoInDL.
                                    pendingPDCCHOrder = FALSE;
                                LOG_MAC_MSG(MAC_UL_SYNC_LOSS_DRX_MEAS, LOGDEBUG,
                                        MAC_DL_Strategy, getCurrentTick(),
                                        dlUEContext_p->ueIndex, drxOnOffTrigger,
                                        UL_SYNC_LOSS_ON, DEFAULT_INT_VALUE,
                                        FALSE,
                                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                        __func__,"SYNC_LOSS_ON_DRX_OFF_PDCCH_TRIG");  
                            }
                            LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                    LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex,
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                    CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    LINE_NUMBER,
                                    DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"UL_DRX_MEAS_STATE");
                           
                            FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
                            continue;
                        }
                        /* + E_CID_5.3 */
			            if(TRUE == dlUEContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder)
                        {
                            putEntryInPdcchOrderQ(
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.preambleIdx, \
                                    dlUEContext_p->ueIndex, getCurrentTick()+ PHY_DL_DELAY,
                                    internalCellIndex);
                            dlUEContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder = FALSE;
                        } 
                        /* - E_CID_5.3 */
                        if (dlUEContext_p->activeUENodeTempDeleted_p)
                        {
                            if (0 == dlUEContext_p->ueQueueLoad)
                            {
                                /* SPR 2548 Changes Start */
                                freeMemPool((void *)dlUEContext_p->
                                        activeUENodeTempDeleted_p);

                                dlUEContext_p->activeUENodeTempDeleted_p = PNULL;
                                /* SPR 2548 Changes Start */

                                FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
                                continue;
                            }
                            /* SPR 2377 changes start */
                            LOG_MAC_MSG(MAC_UE_ADDED_IN_ACTIVE_LIST_IN_DL_LOG_ID, 
                                    LOGDEBUG, MAC_DL_Strategy, 
                                    getCurrentTick(), dlUEContext_p->ueIndex, 
                                    drxOnOffTrigger,
                                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE, __func__,"");
                            
                            /* SPR 2377 changes end */
                                                        
                            /* We need to insert the node at the top of the
                             * list. */
                            dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p = 
                                dlUEContext_p->activeUENodeTempDeleted_p;

                            if (DL_ROUND_ROBIN_SCHEDULING == 
                                    initParams_p->dlSchdConfig.dlSchedulerStrategy)
                            {
                                listInsertNodeBefore
                                    (dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex],
                                        dlNonZeroListStartRoundRobin_p[internalCellIndex], 
                                        dlUEContext_p->
                                        activeUENodeTempDeleted_p);

                                dlNonZeroListStartRoundRobin_p[internalCellIndex] = 
                                    dlUEContext_p->
                                    dlUENonZeroQueueLoadAndHarqList_p;
                            }
                            else if (DL_FAIR_SCHEDULING == 
                                    initParams_p->dlSchdConfig.dlSchedulerStrategy)
                            {
                                listInsertNodeAtHead(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex],
                                    dlUEContext_p->activeUENodeTempDeleted_p);
                            }
                            else
                            {
                                listInsertNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex], 
                                    dlUEContext_p->activeUENodeTempDeleted_p);
                            }
                            
                            dlUEContext_p->activeUENodeTempDeleted_p = PNULL;
                        }
                        else
                        {
                            LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                    LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                    dlUEContext_p->ueIndex,
                                    dlUEContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                    CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    LINE_NUMBER,
                                    DEFAULT_FLOAT_VALUE, 
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"UL_DRX_MEAS_STATE");
                           
                        }
                        if (((PNULL == dlUEContext_p->
                                        dlUENonZeroQueueLoadAndHarqList_p) &&
                                    0 != dlUEContext_p->ueQueueLoad)) 
                        {
                            putEntryInUENonZeroQueueLoadAndHarqList(
                                    ueIndex, dlUEContext_p,
                                    internalCellIndex);
                        }
                        
                        break;

                    case DRX_OFF_TO_DRX_OFF:
                        /* Dont do anything */
                        break;

                    default:
                        /* Log error */
                        break;
                }
            }
        
            FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
        }
    }

    return;
}
/* DRX_CHG */

/*****************************************************************************
 * Function Name  : processDLSchedulerStrategy
 * Inputs         : currentGlobalTTITickCount : current Global Tick
 *                  dlDelay  - The amount of delay expected in the DownLink path
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : DLStrategyTxNode is inserted into the PDCCH New and ReTx 
 *                  Queues Which  will be used by the PDCCH  for CCE distribution.
 *                  scheduler strategy populated the output
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Scheduler Strategy will allocate the RBs to TA transmission, 
 *                  DRX transmission and to UE depending upon the number of 
 *                  sub-frames in advance for which strategy is run to schedule
 *                  allocations and depending upon the available queue load
 *                  of the UEs in the downlink. 
 *                  The strategy uses either the proportional fair or round 
 *                  robin algorithm to distribute the RBs.
 *****************************************************************************/
UInt32 processDLSchedulerStrategy (
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 dlDelay,
        InternalCellIndex internalCellIndex)
{
    /* ICIC changes start */
    UInt32 numberOfAvailableRBs[MAX_USER_LOCATION] = {0,0};
    /* ICIC changes end */

    UInt32 dlTimeAdvanceQueueCount = 0;
    UInt32 dlDRXQueueCount         = 0;
    UInt32 pendingTAQueueCount     = 0;
    UInt32 pendingDRXQueueCount    = 0;
    /* SRP 3608 changes start */
    UInt32 dlPriorityQueueCount    = 0;
    /* SPR 3608 changes end */
    /* + SPR 17439 */
    DLSchedulerDataElement dlScheduleData;
    /* - SPR 17439 */
    UInt32 UEZeroNonZeroTriggerQueueCount   = 0;
    UInt32 ulSyncLossOnOffQueueCount = 0;
    UInt32 drxOnOffQueueCount = 0; /* DRX_CHG */
    UInt32 explicitDLDCIQueueCount = 0; /* CLPC CHANGE*/
    UInt32 failExplicitDLDCIQueueCount = 0; /* CLPC CHANGE*/
    UInt32 measGapOnOffQueueCount = 0; /* MEAS_GAP_CHG */ 
    alreadyScheduledUEs[internalCellIndex] = 0;
    /* RETX_CHG */
    UInt32 dlRetxQueueCount = 0;
    UInt32 dlRetxFailQueueCount = 0;
    /* ICIC Change Start */
    DLSchedulerStrategyConfig* dlSchdConfig_p;
    dlSchdConfig_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
                       initParams_p->dlSchdConfig);
    UInt8 maxUEsToBeScheduledDL     = dlSchdConfig_p->maxUEsToBeScheduledDL;
    UInt32 numOfUsersToSchedule = 0;
    maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] = dlSchdConfig_p->maxCellEdgeUEsToBeScheduledDL;
    UInt32 advanceAllocQueueCount = 0;
    DLResourceAllocationInfo *tempdlRAInfo_p;
    /* ICIC Change End */
    /* RETX_CHG */
#ifdef OVERSHOOT_CORRECTION
    modifiedNoOfUEsAfterOverLoadDL(&maxUEsToBeScheduledDL, internalCellIndex);
#endif
    /* SPR_1346_START */
    /*RETX_CHG*/
    UInt16  numTAUEScheduled = 0; /* This parameter is kept to take into account 
                                     that TA UEs(TA+DATA) dont exceed max dl sch 
                                     UEs */
    /* SPR_1346_END */
  /*EICIC +*/
    UInt32 sysFrameNum 			= 0;
    UInt32 subFrameNum 			= 0;
    UInt8  absSubFrameNum 		= 0;
    UInt8  strategyScanCount	= 0;
    UInt32 	numBitsAbsPattern = 0;
    memSet(&dlScheduleData,0,sizeof(DLSchedulerDataElement));
   getSFAndSFN(&sysFrameNum, &subFrameNum,internalCellIndex);

    /*Calculate the SFN and SF by considering the delay to PHY*/
#ifdef FDD_CONFIG
/*Calculate the SFN and SF by considering the delay to PHY*/
subFrameNum = subFrameNum + dlDelay;
phyDelay_g =  dlDelay; 
#endif
#ifdef TDD_CONFIG
subFrameNum = subFrameNum + 1 ;
phyDelay_g =  1;
#endif
 /*Complexity Start*/  
eicicUpdateSfAndSfn(&sysFrameNum, &subFrameNum);
numBitsAbsPattern=eicicFindNumOfBitsinABSPattern(internalCellIndex);
/*Complexity End*/
     absSubFrameNum = ((((sysFrameNum )*MAX_SUB_FRAME) +(subFrameNum)) %numBitsAbsPattern) ;

 if ((LTE_TRUE == absPattern_g[internalCellIndex][absPatternIndex_g[internalCellIndex]][absSubFrameNum]) && 
         (isEicicEnabled_g[internalCellIndex])) /* Work as aggressor*/
  {
      ueTypeToScheduleDL_g[internalCellIndex] =  INVALID_UE_TYPE; 
     maxUEsToBeScheduledDL = ZERO;

  }
 else  if ((LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) && 
             (isEicicEnabled_g[internalCellIndex])) /* Work as victim*/
  {
      ueTypeToScheduleDL_g[internalCellIndex] = VICTIM_UE;

  }
  else
  {
    	ueTypeToScheduleDL_g[internalCellIndex] = NON_VICTIM_UE; /* By default if No ABS is configured then schedule NON victim UEs */
  }

/* EICIC -*/



    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Entry:[DL-Strategy][%s]"
            " currentGlobalTTITickCount %d delayToPHYDLScheduler %d"
            " Reference Flag Value is =%d",__func__,currentGlobalTTITickCount,
            dlDelay, dlReferenceFlag_g[internalCellIndex] );

    /* ICIC changes start */
    tempdlRAInfo_p = (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex]);
    numberOfAvailableRBs[CC_CE_REGION] = MAX_ADVANCE_DL_SUBFRAMES_ALLOC * 
        tempdlRAInfo_p->totalAllowedRB;
    numberOfAvailableRBs[CE_REGION]    = MAX_ADVANCE_DL_SUBFRAMES_ALLOC *
        tempdlRAInfo_p->totalCellEdgeRB;
    /* ICIC changes end */

    /* MIB nRB FIX start */
    /* RR Scheduler Fix */
#ifdef FDD_CONFIG
    if ( (0 == ((currentGlobalTTITickCount + PHY_DL_DELAY) % 10)) ) 
    {
        /* ICIC changes start */
        numberOfAvailableRBs[CC_CE_REGION] -= mibOverlapRBs_g[internalCellIndex][CC_CE_REGION];
        numberOfAvailableRBs[CE_REGION] -= mibOverlapRBs_g[internalCellIndex][CE_REGION];
        /* ICIC changes end */
    }
#endif
    /* RR Scheduler Fix */
    /* MIB nRB FIX end */

do /* EICIC */
{
    /* RETX_CHG */
    /* +- SPR 17777 */
    
    dlRetxFailQueueCount = sQueueCount(dlRetransmissionFailureQueue_gp[internalCellIndex]);
    if (dlRetxFailQueueCount)
    {
        /* CA Stats Changes Start */
        /* Debug Stats */
        LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_FAILURE_QUEUE
            (dlRetxFailQueueCount, internalCellIndex); /* COVERITY 19395 */
        /* CA Stats Changes End */
        /*SPR 2446 Fix Begins*/
        /* ICIC changes start */
        processDLRetransmissionFailureQueue(numberOfAvailableRBs,
                currentGlobalTTITickCount,
#ifdef FDD_CONFIG
                dlDelay, 
#endif
                &maxUEsToBeScheduledDL,
                internalCellIndex);
        /* ICIC changes end */
        /*SPR 2446 Fix Ends*/
    }

    /* SRP 3608 changes start */
    /*Processing the PSR Priority Queue*/
    /* SPR 6897 changes start */
    dlPriorityQueueCount = circQueueCount(dlPriorityQueue_gp[internalCellIndex]);
    /* SPR 6897 changes end */
    if (dlPriorityQueueCount)
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "[DL-Strategy][%s]: Processing PSR Priority Queue."
                "Priority Queue Count:%d\n",__func__,
                dlPriorityQueueCount );
        processPriorityQueue(dlPriorityQueueCount,
                numberOfAvailableRBs,currentGlobalTTITickCount,
                /* +- SPR 17777 */
                dlPriorityQueue_gp[internalCellIndex], &maxUEsToBeScheduledDL,
                internalCellIndex);
    }
    /* SPR 3608 changes end */
    
    dlRetxQueueCount =  sQueueCount(dlRetransmissionQueue_gp[internalCellIndex]);  
    if (dlRetxQueueCount)
    {
        /* CA Stats Changes Start */
        /* debug Stats */
        LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_QUEUE(dlRetxQueueCount, 
                internalCellIndex);
        /* CA Stats Changes End */

        /*SPR 2446 Fix Begins*/
        /* ICIC changes start */
        processDLRetransmissionQueue(numberOfAvailableRBs,
                currentGlobalTTITickCount,
#ifdef FDD_CONFIG
                dlDelay, 
#endif
                &maxUEsToBeScheduledDL,
        /* ICIC changes end */
        /*SPR 2446 Fix Ends*/
        internalCellIndex);
    }
    /* RETX_CHG */

    /*LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
      "[DL-Strategy][%s]DL Strategy: Total No.Of RBs to be Distribute = %d\
      for ADVANCE SUBFRAMES = %d \n Reference Flag Value is =%d\n",__func__,
      numberOfAvailableRBs,
      MAX_ADVANCE_DL_SUBFRAMES_ALLOC,dlReferenceFlag_g[internalCellIndex] );*/

    /*Process the UEZeroNonZeroQueueLoadTrigger Queue and update the 
      dlUENonZeroQueueLoadAndHarqList,after adding and deleting UEs according to
      Trigger received in dlUEZeroNonZeroQueueLoadTriggerQueue */

    /*SPR 7470 Fix start */
    /** CA-TDD Changes Start **/
    static UInt32 processZeroNonZeroQueueCnt[MAX_NUM_CELL] = {0};
    /* SPR_1293_FIX_START */
    /* RETX_CHG */
    if((processZeroNonZeroQueueCnt[internalCellIndex] == 0))
    /** CA-TDD Changes End **/
    {
        /* SPR_1293_FIX_START */
        /* RETX_CHG */
        UEZeroNonZeroTriggerQueueCount = 
            COUNT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(internalCellIndex);
        /* CirQ changes end */
        if (UEZeroNonZeroTriggerQueueCount)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                    "[DL-Strategy][%s]: Processing UEZeroNonZeroTriggerQueue.\
                    UEZeroNonZeroTriggerQueueCount:%d\n",
                    __func__,UEZeroNonZeroTriggerQueueCount);

            /* +- SPR 17777 */
            processZeroNonZeroTriggerQueue(UEZeroNonZeroTriggerQueueCount,
                    internalCellIndex);
            /* +- SPR 17777 */
        }

        UInt32 tempTotalActiveUE = listCount( 
                dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex] );

        if(tempTotalActiveUE)
        {
            if(1 < dlSchdConfig_p->maxUEsToBeScheduledDL)
            {
                /** CA-TDD Changes Start **/
                processZeroNonZeroQueueCnt[internalCellIndex] = 
                /** CA-TDD Changes End **/
                    ((tempTotalActiveUE -1) / 
                     dlSchdConfig_p->maxUEsToBeScheduledDL);
            }
            else
            {
                /** CA-TDD Changes Start **/
                processZeroNonZeroQueueCnt[internalCellIndex] = (tempTotalActiveUE -1);
                /** CA-TDD Changes End **/
            }
        }
    }
    else
    {
        /** CA-TDD Changes Start **/
        processZeroNonZeroQueueCnt[internalCellIndex]--;
        /** CA-TDD Changes End **/
    }
    /*SPR 7470 Fix end */

    /* DRX_CHG */
    drxOnOffQueueCount = COUNT_DL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex);
    if (drxOnOffQueueCount)
    {
        processDRXOnOffTriggerQueueDL(drxOnOffQueueCount,internalCellIndex);
    }
    /* DRX_CHG */

    /* MEAS_GAP_CHG */
    measGapOnOffQueueCount = COUNT_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(internalCellIndex);
    if (measGapOnOffQueueCount)
    {
        processMeasGapOnOffTriggerQueueDL(measGapOnOffQueueCount,
                /* +- SPR 17777 */
                                          internalCellIndex);
    }
    /* MEAS_GAP_CHG */

    ulSyncLossOnOffQueueCount = COUNT_UL_SYNC_LOSS_TRIGGERQ_DL_Q(internalCellIndex);
    if (ulSyncLossOnOffQueueCount)
    {
        processSyncLossOnOffTriggerQueueDL(ulSyncLossOnOffQueueCount,
                /* +- SPR 17777 */
                                    dlDelay,
                                    internalCellIndex);
    }
    /* ICIC changes start */
    /* Adjusting max number of cell edge users to schedule  as per Minimum number of RB
     * available */
    numOfUsersToSchedule = 
        numberOfAvailableRBs[CE_REGION]/MIN_RB_REQUIRED_FOR_SCHEDULING;

    if(maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] > numOfUsersToSchedule)
    {
        maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] = numOfUsersToSchedule;
    }

    /* Adjusting max number of cell center users to schedule  as per Minimum number of RB
     * available */
    numOfUsersToSchedule = 
        numberOfAvailableRBs[CC_CE_REGION]/MIN_RB_REQUIRED_FOR_SCHEDULING;

    if( maxUEsToBeScheduledDL > numOfUsersToSchedule)
    {
        maxUEsToBeScheduledDL = numOfUsersToSchedule;
    }
    /* dlAdvanceQueue Start (new function )*/
    if (numberOfAvailableRBs[CC_CE_REGION]) 
    {
        advanceAllocQueueCount  = sQueueCount(advanceAllocInfoQueue_gp[internalCellIndex]);
        if ( advanceAllocQueueCount )  
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                    "[DL-Strategy][%s]: Processing DL Advance Queue."
                    " DL Advance QueueCount:%d\n",
                    __func__,advanceAllocQueueCount);

            processDLAdvanceQueue(numberOfAvailableRBs,
                    &dlScheduleData,
                    currentGlobalTTITickCount,
#ifdef FDD_CONFIG
                    dlDelay, 
#endif
                    &maxUEsToBeScheduledDL,
                    internalCellIndex);
        }
    }              
    /* ICIC Changes end */

    /*Processing the Pending TA Request Queue*/
    /* ICIC changes start */
    if (numberOfAvailableRBs[CC_CE_REGION])
    /* ICIC changes end */
    {
        pendingTAQueueCount = COUNT_TA_Q(pendingTAQueue_gp[internalCellIndex]);
        if (pendingTAQueueCount)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                    "[DL-Strategy][%s]: Processing Pending TA Queue."
                    " pending TA QueueCount:%d\n",
                    __func__,pendingTAQueueCount);

            processTAQueue(pendingTAQueueCount, 
                    /* ICIC changes start */
                    numberOfAvailableRBs,
                    /* ICIC changes end */
                    currentGlobalTTITickCount, &dlScheduleData,
                    (pendingTAQueue_gp[internalCellIndex]),PENDING_TA_FLAG, &numTAUEScheduled,
                    &maxUEsToBeScheduledDL
                    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                    ,dlDelay
#endif
                    , internalCellIndex
                    /*HD FDD Changes End*/
                    ); 
            /* SPR_1346 */ /*RETX_CHG*/
        }

        /*Processing the Pending DRX Request Queue*/
        /* ICIC changes start */
        if (numberOfAvailableRBs[CC_CE_REGION])
            /* ICIC changes end */
        {
            pendingDRXQueueCount = COUNT_DRX_Q( (pendingDRXQueue_gp[internalCellIndex]));
            if (pendingDRXQueueCount)
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                        "[DL-Strategy][%s]: Processing Pending DRX Queue.\
                        pending DRX QueueCount:%d\n",
                        __func__,pendingDRXQueueCount);

                processDRXQueue(pendingDRXQueueCount, 
                        /* ICIC changes start */
                        numberOfAvailableRBs,
                        /* ICIC changes end */
                        currentGlobalTTITickCount, &dlScheduleData,
                        (pendingDRXQueue_gp[internalCellIndex]),PENDING_DRX_FLAG,
                        &maxUEsToBeScheduledDL,
                        internalCellIndex);
            }

            /*Processing the TA Request Queue*/
            /* ICIC changes start */
            if (numberOfAvailableRBs[CC_CE_REGION])
                /* ICIC changes end */
            {
                dlTimeAdvanceQueueCount = COUNT_TA_Q(dlTimeAdvanceQueue_gp[internalCellIndex]);
                if (dlTimeAdvanceQueueCount)
                {
                    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                            "[DL-Strategy][%s]: Processing TA Queue."
                            "TA Queue Count:%d\n",__func__,
                            dlTimeAdvanceQueueCount );
                    processTAQueue(dlTimeAdvanceQueueCount, 
                            /* ICIC changes start */
                            numberOfAvailableRBs,
                            /* ICIC changes end */
                            currentGlobalTTITickCount, 
                            &dlScheduleData, dlTimeAdvanceQueue_gp[internalCellIndex], 
                            NEW_TA_FLAG, &numTAUEScheduled
                            ,&maxUEsToBeScheduledDL
                            /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                            ,dlDelay
#endif
                            , internalCellIndex
                            /*HD FDD Changes End*/
                            ); /* SPR_1346 */
                    /*RETX_CHG*/
                }
                /* ICIC changes start */
                if (numberOfAvailableRBs[CC_CE_REGION])
                    /* ICIC changes end */
                {
                    /* CLPC CHANGES */
                    /* PUSCH Power control is enabled so process the explicit DLDCI 
                       Failure queue first*/
                    failExplicitDLDCIQueueCount = COUNT_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q
                                    (explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex]);    
                    if (failExplicitDLDCIQueueCount)
                    {
                        processExplicitDLDCIScheduleQueue(failExplicitDLDCIQueueCount,
                                /* ICIC changes start */
                                numberOfAvailableRBs,
                                /* ICIC changes end */
                                currentGlobalTTITickCount,
                                &dlScheduleData,
                                explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],
                                &numTAUEScheduled,
                                &maxUEsToBeScheduledDL
                                /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                                ,dlDelay
#endif
                                /*HD FDD Changes End*/
                                ,internalCellIndex
                              ) ;
                    }

                    /* ICIC changes start */
                    if (numberOfAvailableRBs[CC_CE_REGION])
                        /* ICIC changes end */
                    {
                        /* PUSCH Power control is enabled so process the explicit DLDCI queue*/
                        explicitDLDCIQueueCount = COUNT_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(
                                    explicitDLPucchTpcScheduleQueue_gp[internalCellIndex]);
                        if (explicitDLDCIQueueCount)
                        {
                            LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                                    "[DL-Strategy][%s]: Processing explicit DL DCI Queue\
                                    DLDCI SCHEDULE Queue Count:%d\n",
                                    __func__, explicitDLDCIQueueCount );
                            processExplicitDLDCIScheduleQueue(explicitDLDCIQueueCount, 
                                    /* ICIC changes start */
                                    numberOfAvailableRBs,
                                    /* ICIC changes end */
                                    currentGlobalTTITickCount, 
                                    &dlScheduleData, 
                                    explicitDLPucchTpcScheduleQueue_gp[internalCellIndex],
                                    &numTAUEScheduled,
                                    &maxUEsToBeScheduledDL
                                    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                                    ,dlDelay
#endif
                                    /*HD FDD Changes End*/
                                    ,internalCellIndex
                                    ) ;
                        }

                        /*Processing the DRX Request Queue*/
                        /* ICIC changes start */
                        if (numberOfAvailableRBs[CC_CE_REGION])
                            /* ICIC changes end */
                        {
                            dlDRXQueueCount = COUNT_DRX_Q(dlDRXQueue_gp[internalCellIndex]);
                            if (dlDRXQueueCount)
                            {
                                LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                                        "[DL-Strategy][%s]: Processing DRX Queue\
                                        DRX Queue Count:%d\n",
                                        __func__,dlDRXQueueCount );

                                processDRXQueue(dlDRXQueueCount,
                                        /* ICIC changes start */
                                        numberOfAvailableRBs,
                                        /* ICIC changes end */
                                        currentGlobalTTITickCount, &dlScheduleData,
                                        dlDRXQueue_gp[internalCellIndex], NEW_DRX_FLAG, 
                                        &maxUEsToBeScheduledDL,
                                        internalCellIndex);
                            }

                            /* Processing of the UEs present in Non Zero Queue Load &
                               distributing the RBs among UEs as per the Scheduler 
                               Algorithm used i.e.Fair or Round Robin Scheduling 
                               Algorithm */
                            LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                                    "[DL-Strategy][%s]Number"
                                    " Of Available RBs %d before calling"
                                    " scheduleUEsByDLStrategy ", __func__, 
                                    numberOfAvailableRBs );
                            /* ICIC changes start */
                            if (numberOfAvailableRBs[CC_CE_REGION])
                                /* ICIC changes end */    
                            {
                                scheduleUEsByDLStrategy 
                                (
                                 /* ICIC changes start */
                                 numberOfAvailableRBs,
                                 /* ICIC changes end */
                                 &dlScheduleData, currentGlobalTTITickCount, maxUEsToBeScheduledDL,
                                 /* RETX_CHG */
                                internalCellIndex);
                            }                
                            else
                            {
                                LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                                        "Entry:[DL-Strategy][%s]Since Number"
                                        " Of Available RBs are Total %d and Cell Edge %d, Hence Exiting\n", __func__,
                                        numberOfAvailableRBs[CC_CE_REGION], numberOfAvailableRBs[CE_REGION]
                                        );
                            }
                        } // process dlDRXQueue_g end
                    } // process explicitDLPucchTpcScheduleQueue_g end
                } // process explicitDLPucchTpcScheduleFailureQueue_g end
            }
        }
    } // process PendingTAQueue
		/* EICIC +*/
		/* Assigning available RBs to non-Victim UEs if this is the ABS subframe */

strategyScanCount = 0;

	
		if (( numberOfAvailableRBs[CC_CE_REGION]) && (maxUEsToBeScheduledDL) && (VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex])
			&& (redistributeRb_g))
		{
			if (!strategyScanCount)
			{
				strategyScanCount = 1;
				ueTypeToScheduleDL_g[internalCellIndex] = NON_VICTIM_UE;
			}
		}
		/* EICIC -*/


}while (strategyScanCount); /* EICIC change*/
        /*SPR_15900_fix_start*/
#ifdef FDD_CONFIG
        if ( (0 == ((currentGlobalTTITickCount + PHY_DL_DELAY) % 10)) )
        {
            /* ICIC changes start */
            dlBufferPool_g[internalCellIndex].totalRBs    = (numberOfAvailableRBs[CC_CE_REGION] + mibOverlapRBs_g[internalCellIndex][CC_CE_REGION]);
            dlBufferPool_g[internalCellIndex].cellEdgeRBs = (numberOfAvailableRBs[CE_REGION] + mibOverlapRBs_g[internalCellIndex][CE_REGION]);
        }
        else
        {
            dlBufferPool_g[internalCellIndex].totalRBs    = numberOfAvailableRBs[CC_CE_REGION];
            dlBufferPool_g[internalCellIndex].cellEdgeRBs = numberOfAvailableRBs[CE_REGION];
        }
#endif
       /*SPR_15900_fix_end*/
    /* ICIC changes end */

    /*LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
      "[DL-Strategy][%s]: DL Strategy populated for %d"
      "Subframes out of MAX SUBFRAME:%d\n",__func__,numOfSubframe, 
      MAX_ADVANCE_DL_SUBFRAMES_ALLOC );*/
    /* dlReferenceFlag_g[internalCellIndex] will be used as reference Flag,which identifies the UE 
       which have got the RBs in the same subframe and preventing the 
       repetition of distribition of RBs to the same UE.
       The Flag is using a MODULO by 2 to change it every time strategy runs*/

    dlReferenceFlag_g[internalCellIndex] = currentGlobalTTITickCount;

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Exit:[DL-Strategy][%s]",
            __func__);

    return MAC_SUCCESS;
}


/* CA_phase2_harq_code Start */
/*****************************************************************************
 * Function Name  :    macPushHarqNackInfoToRetxQueue
 * Inputs         : ueContext_p- A pointer to DLUEContextueIndex
 *                  - The harq process id whose NACK has been
 *                               received.
 *                  nackFlag_u8 - Contains Nack enabled Flags  for each cell,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        :
 * Returns        : MAC_SUCCESS/MAC_FAILURE.
 * Description    : When NACK is recieved for UE in any of cell 
 *                   calls this function to put retransmission info into global
 *                  retransmission Queue.
 *                  Returns Failure in case of pushing node into Q
 *                  fails/ memory is not allocated
 *****************************************************************************/
MacRetType macPushHarqNackInfoToRetxQueue( DLUEContext *ueContext_p,
                             DLRetxQueueNode retxNodeFilled ,
                             UInt8 *nackFlag_p,
                             InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG                            
                             ,UInt8 currentSubframe
#endif                              
                             )
{
    UInt8   ueCellTravesId  = 0;
    UInt8   arrIndex = 0,retxArrIndex = 0;
    UInt8   harqProcessId = 0;

    UInt32 drxRetransmissionTimer = 0;

    DLRetxQueueNode *retxNode_p = PNULL;
    DLHARQProcess *dlHarqProcess_p = PNULL;

    GET_MEM_FROM_POOL (DLRetxQueueNode,
            retxNode_p,sizeof (DLRetxQueueNode), PNULL);

    if (PNULL == retxNode_p)
    {
        return MAC_FAILURE;
    }

    retxNode_p->harqProcessId = INVALID_HARQ_ID;
    retxNode_p->ueIndex = retxNodeFilled.ueIndex;
    retxNode_p->sCellCount = 0;
    retxNode_p->isNodeInvalid = retxNodeFilled.isNodeInvalid;
    /* + SPR 11772 Changes */
    retxNode_p->schUsed = 0;
    /* - SPR 11772 Changes */

    /*
     * In below for loop MAX_NUM_CELL is used 
     * instead of scellCount is one sec cell may have NACK Info
     * other may Ack Info. 
     * In case of Ack below nackFlag[0] will be False
     * then loop will not processed
     */

    for(ueCellTravesId = 0;ueCellTravesId < MAX_NUM_CELL ; ueCellTravesId++)
    {
        if(nackFlag_p[ueCellTravesId] == TRUE)
        { 
            if(!ueCellTravesId)
            {
                retxNode_p->harqProcessId = retxNodeFilled.harqProcessId;
                harqProcessId = retxNodeFilled.harqProcessId;
                retxNode_p->schUsed = retxNodeFilled.schUsed;

                dlHarqProcess_p = 
                    &(ueContext_p->dlHarqContext_p->harqProcess[harqProcessId]);

            }
            else
            {
                arrIndex  = ueCellTravesId - 1;
                retxArrIndex = retxNode_p->sCellCount; 

                retxNode_p->sCellRetxInfo[retxArrIndex].servCellIndex = 
                    retxNodeFilled.sCellRetxInfo[arrIndex].servCellIndex ;

                retxNode_p->sCellRetxInfo[retxArrIndex].schUsed = 
                    retxNodeFilled.sCellRetxInfo[arrIndex].schUsed ;

                retxNode_p->sCellRetxInfo[retxArrIndex].harqProcessId = 
                    retxNodeFilled.sCellRetxInfo[arrIndex].harqProcessId;

                harqProcessId = 
                    retxNodeFilled.sCellRetxInfo[arrIndex].harqProcessId;


                dlHarqProcess_p = 
                    &(ueContext_p->dlUeScellContext_p[ueCellTravesId]->
                            ScelldlHarqContext_p->harqProcess[harqProcessId]);


                retxNode_p->sCellCount++;

            }

            if(SETUP == ueContext_p->drxConfigType)
            {    
#ifdef TDD_CONFIG
                drxRetransmissionTimer = tddConvertTimerVal
                    (ueContext_p->drxContext.drxConfig.drxRetransmissionTimer, 
                     dlHarqProcess_p->dlharqRttExpiryTick, internalCellIndex);

                dlHarqProcess_p->drxRetxExpiryTTI = drxRetransmissionTimer;
#else
                drxRetransmissionTimer = 
                    ueContext_p->drxContext.drxConfig.drxRetransmissionTimer;
                dlHarqProcess_p->drxRetxExpiryTTI = drxRetransmissionTimer + 
                    (dlHarqProcess_p->dlharqRttExpiryTick);
#endif
            }
        }
    }

    {
        if(pushNode(dlRetransmissionQueue_gp[internalCellIndex],
                    &(retxNode_p->anchorNode)))
        {
            freeMemPool(retxNode_p);
            return MAC_FAILURE;
        }

    }    /* SPR 21875 Fix + */
    /* SPR 21875 Fix - */
#ifdef TDD_CONFIG
    if (ueContext_p->tempRetxNode.isNodeInvalid == FALSE)   
    {     
        ueContext_p->retxQueueNode_p[currentSubframe] = retxNode_p;
    }
#endif     
    if (dlHarqProcess_p)
    {
        LOG_MAC_MSG (DRX_UE_NACK_RECIEVED_LOG_ID, LOGINFO, MAC_DL_HARQ,
                getCurrentTick (),
                retxNode_p->ueIndex, retxNode_p->harqProcessId,
                dlHarqProcess_p->drxRetxExpiryTTI, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME, "SUCCESS");
    }
    else
    {
        LOG_MAC_MSG (DRX_UE_NACK_RECIEVED_LOG_ID, LOGINFO, MAC_DL_HARQ,
                getCurrentTick (),
                retxNode_p->ueIndex, retxNode_p->harqProcessId,
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME, "FAILURE");
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : putEntryInDLRetransmissionQueue
 * Inputs         : ueContext_p- A pointer to DLUEContextueIndex
 *                  hProcessId - The harq process id whose NACK has been
 *                               received.
 *                  harqRTTExpiryTTI - Expiry TTI of HARQ RTT timer
 *                  schUsed - Scheduler used,
 *                  ueInternalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE.
 * Description    : When handling NACK or timer expiry ,timer thread
 *                   calls this
 *                  function to put the harq process id in the global
 *                  retransmission Queue.
 *****************************************************************************/
           
/* SPR 5599 changes start (ZIP ID 129193) */            
MacRetType putEntryInDLRetransmissionQueue(
/* SPR 5599 changes end (ZIP ID 129193) */            
        DLUEContext*  ueContext_p, UInt8 harqProcessId,
                /*SPR 15909 fix start*/
                tickType_t harqRTTExpiryTTI, UInt8 schUsed,
                /*SPR 15909 fix end*/
                InternalCellIndex ueInternalCellIndex)
{
    UInt32 drxRetransmissionTimer = 0;
    DLRetxQueueNode *retxNode_p = PNULL;
    DLHARQProcess *dlHarqProcess_p = PNULL;
    DRXContext *drxContext_p = PNULL;

    GET_MEM_FROM_POOL (DLRetxQueueNode, retxNode_p,
            sizeof (DLRetxQueueNode), PNULL);

    /* + coverity 24539 */
    if (PNULL == retxNode_p)
    {
        return MAC_FAILURE;
    }
    /* - coverity 24539 */

    drxContext_p = &(ueContext_p->drxContext);
    retxNode_p->harqProcessId = harqProcessId;
    retxNode_p->ueIndex = ueContext_p->ueIndex;
    retxNode_p->isNodeInvalid = FALSE;
    /* + CA_TDD_HARQ_CHANGES */
    retxNode_p->schUsed = schUsed;
    retxNode_p->sCellCount = 0;
    /* + CA_TDD_HARQ_CHANGES */

    if(IS_PCELL_OF_UE(ueContext_p, ueInternalCellIndex))
    {
        dlHarqProcess_p =
            &(ueContext_p->dlHarqContext_p->harqProcess[harqProcessId]);
    }
    else
    {
        retxNode_p->sCellRetxInfo[0].servCellIndex = 
                 getServeCellIndexFromInternalCellId(ueContext_p,ueInternalCellIndex);
        retxNode_p->sCellRetxInfo[0].schUsed = schUsed ;
        retxNode_p->sCellRetxInfo[0].harqProcessId = harqProcessId;
        retxNode_p->sCellCount++;

        dlHarqProcess_p =
            &(ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p->harqProcess[harqProcessId]);
    }

    if(SETUP == ueContext_p->drxConfigType) 
    {
#ifdef TDD_CONFIG
        drxRetransmissionTimer = tddConvertTimerVal
            (drxContext_p->drxConfig.drxRetransmissionTimer, harqRTTExpiryTTI, ueInternalCellIndex);
        dlHarqProcess_p->drxRetxExpiryTTI = drxRetransmissionTimer;
#else
        drxRetransmissionTimer = (drxContext_p)->drxConfig.drxRetransmissionTimer;
        dlHarqProcess_p->drxRetxExpiryTTI = harqRTTExpiryTTI + 
                                    drxRetransmissionTimer;
#endif
    }

    /* + coverity 32505 */
    if (pushNode (dlRetransmissionQueue_gp[ueContext_p->internalCellIndex],
                   &(retxNode_p->anchorNode)))
    {
        freeMemPool(retxNode_p);
        retxNode_p = PNULL;
    }
    /*cov warning 24539 fix*/
    if(PNULL!= retxNode_p)
    {    
        LOG_MAC_MSG (DRX_UE_NACK_RECIEVED_LOG_ID, LOGINFO, MAC_DL_HARQ,
                getCurrentTick (),
                retxNode_p->ueIndex, retxNode_p->harqProcessId,
                dlHarqProcess_p->drxRetxExpiryTTI, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME, "SUCCESS");
    }
    return MAC_SUCCESS;
}

/* cyclomatic changes*/

/*****************************************************************************
 * Function Name  : validateCcCeRBForRetx
 * Inputs         : dlHarqProcess_p - pointer to the Dl harq process context
 *                  numberOfAvailableRBs_p - pointer to number of available RB's
 *                  userLocationType - CC or CE user type
 *                  globalTick - current global tick,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : resourceRegionAllocated_p - pointer to update hte resource
 *                  region allocated
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function check if the UE retransmission is possible
 *                  in current tick.
 *****************************************************************************/
static  MacRetType validateCcCeRBForRetx
(
   DLHARQProcess *dlHarqProcess_p,
   UInt32 *numberOfAvailableRBs_p,
   UInt8  userLocationType,
   UInt8  *resourceRegionAllocated_p,
   /*SPR 15909 fix start*/
   tickType_t globalTick,
    /*SPR 15909 fix end*/
   InternalCellIndex internalCellIndex
)
{
    /* SPR 15544 fix start */
    *resourceRegionAllocated_p = userLocationType;
    /* SPR 15544 fix end */

    if (dlHarqProcess_p->assignedResourcesNB > numberOfAvailableRBs_p[userLocationType])
    {
       if (userLocationType == CE_USER)\
       {
           if (dlHarqProcess_p->assignedResourcesNB > (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB)
           {
               if(dlHarqProcess_p->assignedResourcesNB <= numberOfAvailableRBs_p[CC_CE_REGION])
               {
                   LOG_UT(MAC_DL_CC_RESOURCE_ALLOCATED_TO_CE_UE,LOGDEBUG,MAC_DL_Strategy,
                           getCurrentTick(),
                           DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                           DEFAULT_INT_VALUE,LINE_NUMBER,
                           DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                           FUNCTION_NAME,"");
                   /* schedule from CC_CE region */
                   *resourceRegionAllocated_p = CC_CE_REGION;
               }
               else
               {
                   return MAC_FAILURE;
               }
           }
           else
           {
               if(((globalTick - dlHarqProcess_p->dlharqRttExpiryTick) > DL_RETX_TTI_DIFF_THRESHOLD) && 
                       (dlHarqProcess_p->assignedResourcesNB <= numberOfAvailableRBs_p[CC_CE_REGION])) 
               {
                   LOG_UT(MAC_DL_CC_RESOURCE_ALLOCATED_TO_CE_UE,LOGDEBUG,MAC_DL_Strategy,
                           getCurrentTick(),
                           DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                           DEFAULT_INT_VALUE,LINE_NUMBER,
                           DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                           FUNCTION_NAME,"");
                   *resourceRegionAllocated_p = CC_CE_REGION;
               }
               else
               {
                    return MAC_FAILURE;
               }
           }
       }
       else
       {
            return MAC_FAILURE;
       }
   }
   return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : isReTxFailNodeValidForRetransmission
 * Inputs         : dlReTxFailNode_p - pointer to Re TX faliure queue node
 *                  numberOfAvailableRBs_p - pointer to number of avaliable RB's
 *                  deleteNode - Pointer to delete node,
 *                  resourceRegionAllocated_p - Pointer to resouce region allocated
 *                  globalTick - current global tick
 *                  dlDelay - delay in DL path to phy
 *                  internalCellIndex - cell index used at MAC.
 * Outouts        : deleteNode - flag to delete node or not
 * Description    : This function check if the UE retransmission is possible
 *                  in current tick.
 *****************************************************************************/
static  MacRetType isReTxFailNodeValidForRetransmission(
     DLRetxFailureQueueNode *dlReTxFailNode_p,
     UInt32                 *numberOfAvailableRBs_p,
     UInt8                  *deleteNode,
     UInt8                  *resourceRegionAllocated_p,
     /*SPR 15909 fix start*/
     tickType_t                 globalTick
     /*SPR 15909 fix end*/
     #ifdef FDD_CONFIG
     ,UInt32                 dlDelay
     #endif
     ,InternalCellIndex      internalCellIndex
)
{

    UInt16 ueIndex = 0;
    DLUEContext      *dlUEContext_p = PNULL;
    DLUEContextInfo  *dlUEContextInfo_p = PNULL;
    MeasGapContextDL *measgapContext_p = PNULL;
    DLHARQProcess    *dlHarqProcess_p = PNULL;
    LP_SpsDlInfo     spsDlInfo_p       = PNULL;
    UInt8            userLocationType = CC_USER;
#ifdef TDD_CONFIG
    UInt8 currentSubframeType = 0;
    /* CA Changes Start */
    currentSubframeType = getSubframeType((globalTick % 10),internalCellIndex);
    /* CA Changes End */
#endif

    DLScheduleFlag scheduleFlag = DL_QOS_NEW_TRANSMISSION; 
#ifdef LOG_PRINT_ENABLED
    UInt32 state = 0;
#endif
    ueIndex = dlReTxFailNode_p->ueIndex;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    InitParams *initParams_p = cellSpecificParams_g.\
                               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;


     /* + SPR_6061_6062 */
     if(dlReTxFailNode_p->isNodeInvalid) 
     {
     /* - SPR_6061_6062 */
        /* Node is invalid hence needs to be deleted */
        *deleteNode = TRUE;
        return MAC_FAILURE;
     }
     if(dlUEContextInfo_p->pendingDeleteFlag)
     {
        *deleteNode = TRUE;
        return MAC_FAILURE;
     }
    dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

    if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
    {
        userLocationType = dlUEContext_p->userLocationType;
        scheduleFlag = dlUEContext_p->scheduleFlag;
        dlHarqProcess_p = &(dlUEContext_p->dlHarqContext_p->
		    harqProcess[(dlReTxFailNode_p->dlStrategyTxNode_p)->
		    harqProcessId]);
    }
    else
    {
        userLocationType = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
        scheduleFlag = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag;
        dlHarqProcess_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p->
                harqProcess[(dlReTxFailNode_p->dlStrategyTxNode_p)->
                harqProcessId]);
    }


#ifdef HD_FDD_CONFIG
     /* If HD FDD UE is in Retx Fail Q,then check Schedule map whether 
      * it can be scheduled in currenttick or not*/
     if ((TRUE == dlUEContext_p->isHDFddFlag)
         && (!(CAN_SCHEDULE_DL_FOR_HD_UE(globalTick, ueIndex, hdUeContextInfo_g.hdfddUeSchedMap))))
     {
          UPDATE_UE_DENIED_DL_RESOURCE(ueIndex);
          return MAC_FAILURE;
     }
#endif
     if ( HARQ_PROCESS_RESET == dlHarqProcess_p->harqStatus 
         	    || dlHarqProcess_p->dciFormat >= MAX_NUM_DCI_FORMAT )
     {
          /* + SPR_6061_6062 */
        freeDLHarqProcess(dlUEContext_p, dlHarqProcess_p,internalCellIndex);
          /* - SPR_6061_6062 */
          *deleteNode = TRUE;
          return MAC_FAILURE;
     }
     if( SPS_SCHEDULING == dlUEContext_p->schType )
     {
            /* SPR 19704 + */
            /* If SPS Retx harq collides with new sps transmission 
               Then discard the previouse retransmission 
               Discarding will be done in next TTI */
            if( dlHarqProcess_p->harqProcessId == dlUEContext_p->spsDlInfo.hpId )
            {
                dlReTxFailNode_p->isNodeInvalid = TRUE;
            }
            /* SPR 19704 - */
          return MAC_FAILURE;
     }
     spsDlInfo_p = &dlUEContext_p->spsDlInfo;
     if( (SPS_SETUP == spsDlInfo_p->requestType) && 
         (dlHarqProcess_p->harqProcessId < 
                    spsDlInfo_p->spsDlSetupInfo_p->numberOfConfSpsProcesses))
     {

        if(UL_SYNC_LOSS_ON == checkUlSyncLossForUE(dlUEContext_p,globalTick,internalCellIndex))
          {
              /* free the HARQ process for SPS NEW_TX */
            freeDLHarqProcessTBSpecific( dlUEContext_p, dlHarqProcess_p, TB_ONE, internalCellIndex);
#ifdef LOG_PRINT_ENABLED
              state = spsDlInfo_p->spsState;
#endif
              /* Change the SPS state */
              dlStateMachine[spsDlInfo_p->spsState][UE_SYNC_LOSS_ON](
                            dlUEContext_p->ueIndex, spsDlInfo_p->spsState, 
                            UE_SYNC_LOSS_ON, dlUEContext_p, 
                            spsDlInfo_p->sdlOccNode_p, 
                            /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                            dlDelay,
#else
                            DL_ADVANCE_TICK,
#endif
                            /* - SPS_TDD_Changes */
                            internalCellIndex );

              LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, dlUEContext_p->ueIndex, state, UE_SYNC_LOSS_ON,
                     spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");

              *deleteNode = TRUE;
              return MAC_FAILURE;
          }
     }
    if ((DL_QOS_BASED_SCHEDULING != (initParams_p->dlSchdConfig.dlSchedulerStrategy) 
                && dlUEContext_p->isAlreadyConsideredFlag == dlReferenceFlag_g[internalCellIndex] )   
            || ( DL_QOS_BASED_SCHEDULING == (initParams_p->dlSchdConfig.dlSchedulerStrategy)
                && DL_QOS_NO_NEW_TRANSMISSION >= scheduleFlag ))
     {
              return MAC_FAILURE;
     }
     if ((( SETUP == dlUEContext_p->drxConfigType) && 
		  (globalTick > dlHarqProcess_p->drxRetxExpiryTTI)) ||
         /*  ( globalTick > dlHarqRTTExpiryTTI ) ||*/
         /* ICIC changes start */
         (dlHarqProcess_p->assignedResourcesNB > 
             (dlRAInfo_gp[internalCellIndex]+ dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB)  
         /* ICIC changes end */
		)
     {
        freeDLHarqProcess(dlUEContext_p, dlHarqProcess_p, internalCellIndex);
           *deleteNode = TRUE;
           return MAC_FAILURE;
     }
     measgapContext_p = &(dlUEContext_p->measGapContext);
     if (CHECK_UE_MEAS_GAP_PERIOD(measgapContext_p))
     {
           return MAC_FAILURE;
     }
    if((CE_USER == userLocationType ) && (!maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]))
     {
           return MAC_FAILURE;
     }
     #ifdef TDD_CONFIG
     if (dlHarqProcess_p->subFrameType != currentSubframeType)
     {
           return MAC_FAILURE;
     }
     #endif
     return (validateCcCeRBForRetx(dlHarqProcess_p,
                                   numberOfAvailableRBs_p,
                                   userLocationType,
                                   resourceRegionAllocated_p,
                globalTick,
                internalCellIndex));
}
/*****************************************************************************
 * Function Name  : checkAndPopReTxFailNode
 * Inputs         : dlReTxFailNode_p - pointer to Re TX faliure queue node,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will pop the node if the passes node is
 *                  first node in the queue. else, it will mark this node as 
 *                  invalid.
 *****************************************************************************/
static  MacRetType checkAndPopReTxFailNode(
        DLRetxFailureQueueNode *dlReTxFailNode_p,
        /* SPR 10573 changes start */
        UInt8 deleteFlag,
        /* SPR 10573 changes end */
        InternalCellIndex internalCellIndex )
{
    DLRetxFailureQueueNode *dlReTxFailNodeTmp_p = NULL;
    DLHARQProcess   *dlHarqProcess_p = PNULL;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext     *dlUEContext_p = PNULL;
    
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlReTxFailNode_p->ueIndex];
    dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

    dlReTxFailNodeTmp_p = (DLRetxFailureQueueNode *)getFirstNode(
            dlRetransmissionFailureQueue_gp[internalCellIndex]);
    if(dlReTxFailNodeTmp_p != PNULL)
    {
        if(dlReTxFailNodeTmp_p == dlReTxFailNode_p)
        {
            /* SPR_3713_FIX_START */
            if (dlUEContextInfo_p->pendingDeleteFlag && PNULL != dlUEContext_p)
            {
                /* + SPR 11469 Changes */
                dlHarqProcess_p = dlReTxFailNode_p->dlStrategyTxNode_p->harqProcess_p;
                /* - SPR 11469 Changes */
                freeDLHarqProcess(dlUEContext_p, dlHarqProcess_p, internalCellIndex);
            }
            /* SPR_3713_FIX_END */
            /* Remove First node in Queue */
            dlReTxFailNode_p = (DLRetxFailureQueueNode *) popNode
                (dlRetransmissionFailureQueue_gp[internalCellIndex]);
            /* + Coverity_36188 */
            if (dlReTxFailNode_p)
            {
                /* SPR 10573 changes start */
                if (deleteFlag)
                {
                    if (PNULL != dlUEContext_p)
                    {
                        dlHarqProcess_p = dlReTxFailNode_p->
                            dlStrategyTxNode_p->harqProcess_p;
                        if (dlHarqProcess_p)
                        {
                            freeDLHarqProcess(dlUEContext_p, dlHarqProcess_p, 
                                    internalCellIndex );
                        }
                    }
                    freeMemPool(dlReTxFailNode_p->dlStrategyTxNode_p);
                    dlReTxFailNode_p->dlStrategyTxNode_p = PNULL;
                }
                /* SPR 10573 changes end */

                dlReTxFailNodeTmp_p = dlReTxFailNode_p;
                freeMemPool (dlReTxFailNodeTmp_p);
                /* + SPR 5584 (COVERITY CID 25531) FIX */
                dlReTxFailNodeTmp_p = PNULL;
                dlReTxFailNode_p = PNULL;
                /* - SPR 5584 (COVERITY CID 25531) FIX */
            }
            /* - Coverity_36188 */
        }
        else
        {
            dlReTxFailNode_p->isNodeInvalid = TRUE;
        }
    }
    else
    {
        /* +COVERITY 10149 */
        return MAC_FAILURE;
        /* -COVERITY 10149 */
    }
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : checkAndPopReTxNode
 * Inputs         : dlReTxNode_p - pointer to Re TX  queue node,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will pop the node if the passes node is
 *                  first node in the queue. else, it will mark this node as 
 *                  invalid.
 *****************************************************************************/
static  MacRetType checkAndPopReTxNode(DLRetxQueueNode *dlReTxNode_p,InternalCellIndex internalCellIndex)
{
    DLRetxQueueNode *dlReTxNodeTmp_p = NULL;
    DLHARQProcess   *dlHarqProcess_p = PNULL;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext     *dlUEContext_p = PNULL;
    /* SPR 11912 Changes Start */
    if (!dlReTxNode_p)
    {
        return MAC_FAILURE;
    }
    /* SPR 11912 Changes End */
    dlUEContextInfo_p = &dlUECtxInfoArr_g[dlReTxNode_p->ueIndex];
    dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
    /*+ SPR_22837 fix*/
    /*+ SPR_22887 fix*/
    dlReTxNodeTmp_p = (DLRetxQueueNode *)getFirstNode(dlRetransmissionQueue_gp[internalCellIndex]);
    /*- SPR_22887 fix*/
    /*- SPR_22837 fix*/
    if(dlReTxNodeTmp_p != PNULL)
    {
        if(dlReTxNodeTmp_p == dlReTxNode_p)
        {
            /* SPR_3713_FIX_START */
            if (dlUEContextInfo_p->pendingDeleteFlag && PNULL != dlUEContext_p)
            {
                /* + SPR 11469 Changes */
                if(INVALID_HARQ_ID != dlReTxNode_p->harqProcessId)
                {
                    dlHarqProcess_p = &(dlUEContext_p->dlHarqContext_p->
                            harqProcess[(dlReTxNode_p->harqProcessId)]);
                    freeDLHarqProcess(dlUEContext_p, dlHarqProcess_p,internalCellIndex);
                }
                /* + SPR_14522 */
                if((dlReTxNode_p->sCellCount == 1) &&
                        (dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]))
                /* - SPR_14522 */
                {
                    dlHarqProcess_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
                            ScelldlHarqContext_p->harqProcess[(dlReTxNode_p->sCellRetxInfo[0].harqProcessId)]);
                    /* + SPR 14341 Changes */
                    freeDLHarqProcess(dlUEContext_p, dlHarqProcess_p, dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex);
                    /* - SPR 14341 Changes */
                }
                /* - SPR 11469 Changes */
            }
            /* SPR_3713_FIX_END */
            /* Remove First node in Queue */
    /*+ SPR_22837 fix*/
    /*+ SPR_22887 fix*/
            dlReTxNode_p = (DLRetxQueueNode *) popNode
                (dlRetransmissionQueue_gp[internalCellIndex]);
    /*- SPR_22887 fix*/
    /*- SPR_22837 fix*/
            /* + Coverity_36189 */
            if (dlReTxNode_p) 
			{ 
				dlReTxNodeTmp_p = dlReTxNode_p;
				freeMemPool (dlReTxNodeTmp_p);
				/* + SPR 5584 (COVERITY CID 25531) FIX */
				dlReTxNodeTmp_p = NULL;
				dlReTxNode_p = NULL;
				/* - SPR 5584 (COVERITY CID 25531) FIX */
			}
            /* - Coverity_36189 */
        }
        else
        {
            dlReTxNode_p->isNodeInvalid = TRUE;
        }
    }
    else
    {
        /* +COVERITY 10149 */
        return MAC_FAILURE;
        /* -COVERITY 10149 */
    }
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : processDLRetransmissionFailureQueue
 * Inputs         : globalTick : current Global Tick
 *                  maxUEsToBeScheduledDL_p - maximum number of UEs that can be
 *                                              scheduled in a single tick,
 *                  dlDelay - delay in DL path to phy
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : numberOfAvailableRBs_p - No of RBs allcated are decremented
 *                                           and returned 
 *                  maxUEsToBeScheduledDL_p - After a UE is scheduled its count
 *                  is decremented and is returned
 * Returns        : Void
 * Description    : This function processes the global Retransmission queue, and
 *                  schedules the UE first whose data is queued for
 *                  retransmissios
 *****************************************************************************/
/*SPR 2446 Fix Begins*/
        /*SPR 15909 fix start*/
void processDLRetransmissionFailureQueue( 
        UInt32* numberOfAvailableRBs_p,tickType_t  globalTick,
        /*SPR 15909 fix end*/
#ifdef FDD_CONFIG        
        UInt32 dlDelay,
#endif
        UInt8 *maxUEsToBeScheduledDL_p,
        InternalCellIndex internalCellIndex)
/*SPR 2446 Fix Ends*/
{
    UInt16 ueIndex = 0;
    DLRetxFailureQueueNode *dlReTxFailNode_p = PNULL;
    DLRetxFailureQueueNode *dlReTxFailNodeTmp_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLHARQProcess *dlHarqProcess_p = PNULL;
    UInt8  userLocationType = CC_USER;
    UInt32 dlRetxFailQueueCount = sQueueCount(dlRetransmissionFailureQueue_gp[internalCellIndex]);
    UInt8 resourceRegionAllocated = CC_CE_REGION;
    MacRetType ret_val = MAC_FAILURE;
    UInt8 deleteNode = FALSE;
    UInt8 retxFlag = FALSE;
    UInt8 harqProcessId = INVALID_HARQ_ID;
    DLStrategyTxNode **dlStrategyTxNode_p = PNULL;
    /*SPR 21290 Start*/
    TransmissonMode     transmissionMode  = INVALID_TX_MODE;
    /*SPR 21290 End */

#ifdef FDD_CONFIG
    globalTick += dlDelay;
#endif
    UInt32 count = dlRetxFailQueueCount;
#ifdef SSI_DEBUG_STATS
    DLScheduleFlag scheduleFlag = DL_QOS_NEW_TRANSMISSION; 
#endif

    while( count-- )
    {
        dlReTxFailNode_p = 
            (DLRetxFailureQueueNode *) popNode(dlRetransmissionFailureQueue_gp[internalCellIndex]);
        if( PNULL != dlReTxFailNode_p )
        {
            if (dlReTxFailNode_p->isNodeInvalid)
            {
                freeMemPool (dlReTxFailNode_p); 
                dlReTxFailNode_p = PNULL;
            }
            else
            {
                /*cov_warning 56014 fix start*/
                if(pushNode (dlRetransmissionFailureQueue_gp[internalCellIndex], &(dlReTxFailNode_p->anchorNode))) 
                {
                    freeMemPool(dlReTxFailNode_p);
                    dlReTxFailNode_p = PNULL; 
                }
                /*cov_warning 56014 fix end*/
            } 
        }
    }

    dlReTxFailNode_p = (DLRetxFailureQueueNode *) getFirstNode
        (dlRetransmissionFailureQueue_gp[internalCellIndex]);

    /*SPR 3679 QOS Changes Start*/
    while (dlReTxFailNode_p && numberOfAvailableRBs_p[CC_CE_REGION] && 
            *maxUEsToBeScheduledDL_p && dlRetxFailQueueCount)
        /*SPR 3679 QOS Changes End*/
    {
        ueIndex = dlReTxFailNode_p->ueIndex;
        dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

        dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

        retxFlag = isReTxFailNodeValidForRetransmission( dlReTxFailNode_p,
                numberOfAvailableRBs_p, &deleteNode,
                &resourceRegionAllocated, globalTick
#ifdef FDD_CONFIG
                ,dlDelay
#endif
                ,internalCellIndex
                );

        dlReTxFailNodeTmp_p = (DLRetxFailureQueueNode *) getNextNode
            ((const LTE_SNODE *) (dlReTxFailNode_p));

        if(retxFlag)
        {
            /*  Lets check the UE type ( Victim / Non Victim ) required to be scheduled  
               1. Victim UE to be scheduled and the UE popped is non - victim 
               2. Non victim UE to be scheduled and the UE popped is Victim 
               3. Victim UE to be scheduled and the UE popped is victim 
               4. Non victim UE to be scheduled and the UE popped is non-Victim */
            dlUEContext_p = dlUEContextInfo_p->dlUEContext_p; 
            
            if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == dlUEContext_p->userType)) ||
                    ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == dlUEContext_p->userType)))
            {   
                dlReTxFailNode_p = dlReTxFailNodeTmp_p;
                deleteNode = FALSE;
                dlRetxFailQueueCount--;
                continue;
            }

            harqProcessId = (dlReTxFailNode_p->dlStrategyTxNode_p)->harqProcessId;
            if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
            {
                userLocationType = dlUEContext_p->userLocationType;
#ifdef SSI_DEBUG_STATS
                scheduleFlag = dlUEContext_p->scheduleFlag;
#endif
                dlStrategyTxNode_p = &(dlUEContext_p->dlStrategyTxNode_p);

                dlHarqProcess_p = &(dlUEContext_p->dlHarqContext_p->
                        harqProcess[harqProcessId]);
                transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
            }
            else
            {
                /* + SPR_14522 */
                if (dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX])
                {    
                    userLocationType = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
#ifdef SSI_DEBUG_STATS
                    scheduleFlag = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag;
#endif
                    dlStrategyTxNode_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);

                    dlHarqProcess_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->\
                            ScelldlHarqContext_p->harqProcess[harqProcessId]);
                    transmissionMode =
                        dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.transmissionMode;
                }
                else
                {
                    /*SPR 21290 Start*/
                    if(MAC_SUCCESS == checkAndPopReTxFailNode(dlReTxFailNode_p, FALSE, internalCellIndex))
                    {
                    /*SPR 21290 End*/
                        dlReTxFailNode_p = dlReTxFailNodeTmp_p;
                        dlRetxFailQueueCount--;
                        deleteNode = FALSE;
                        continue;
                    /*SPR 21290 Start*/
                    }
                    /*SPR 21290 End*/
                }    
                /* - SPR_14522 */
            }
            /* SPR 11403 fix start */
            if(MAC_SUCCESS != validateTxModeAndDciFormat(transmissionMode,
                        dlHarqProcess_p->dciFormat))
            {
                /*SPR 21290 Start*/
                if(MAC_SUCCESS == checkAndPopReTxFailNode(dlReTxFailNode_p, FALSE, internalCellIndex))
                {
                /*SPR 21290 End*/
                dlReTxFailNode_p = dlReTxFailNodeTmp_p;
                dlRetxFailQueueCount--;
                deleteNode = FALSE;
                continue;
                /*SPR 21290 Start*/
                }
                /*SPR 21290 End*/

            }
            /* SPR 11403 fix end */

            dlReTxFailNode_p->dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated = 
                resourceRegionAllocated;
            /* + Rel_6_0_reTx_Changes */

            if (MAC_SUCCESS == lteMacStrategyAssignRBsForReTxFail (
                        dlUEContext_p, 
                        dlHarqProcess_p,
                        dlReTxFailNode_p->dlStrategyTxNode_p,
                        internalCellIndex)) 
            {
                /* Use already allocated Rbs from harq context 
                 * to reduce available RBs*/
                numberOfAvailableRBs_p[resourceRegionAllocated] -= 
                    (dlHarqProcess_p)->assignedResourcesNB;
                /* Update num of available RBs if allocated from CE region */                    
                if (resourceRegionAllocated == CE_REGION)
                {
                    numberOfAvailableRBs_p[CC_CE_REGION] -= 
                        (dlHarqProcess_p)->assignedResourcesNB;
                }
                if (numberOfAvailableRBs_p[CC_CE_REGION] <  
                        numberOfAvailableRBs_p[CE_REGION])
                {
                    numberOfAvailableRBs_p[CE_REGION] = 
                        numberOfAvailableRBs_p[CC_CE_REGION];
                }

                dlUEContext_p->isAlreadyConsideredFlag = dlReferenceFlag_g[internalCellIndex];

                /* No Need to set scheduleFlag here. 
                 * Already set through lteMacStrategyAssignRBsForReTxFail(). 
                 * In case, If one TB is scheduled as NewTX, scheduleFlag would be DL_QOS_ALREADY_TRANSMITED */

                (*maxUEsToBeScheduledDL_p)--;
                /* Save startegy node created earlier in Ue Context */
                *dlStrategyTxNode_p = dlReTxFailNode_p->dlStrategyTxNode_p;
                /* Update Cell Edge Ue */
                if ( CE_USER == userLocationType )
                {
                    maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] --;
                }

#ifdef SSI_DEBUG_STATS
                LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCHEDULE_FLAG(
                        ueIndex,
                        scheduleFlag);
#endif

                /* debug Stats */
                LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED(
                        dlUEContext_p, 
                        dlHarqProcess_p, 
                        (*dlStrategyTxNode_p),
                        internalCellIndex);
                /** SPR 13599 Changes Start **/
                if(IS_CA_ENABLED())
                {
                    dlQosPreparePreferredCellListForReTxFail(
                            dlUEContext_p,
                            internalCellIndex);
                }
                /** SPR 13599 Changes End **/
                if (DL_QOS_BASED_SCHEDULING != (cellSpecificParams_g.cellConfigAndInitParams_p[\
                            dlUEContext_p->internalCellIndex]->initParams_p->\
                            dlSchdConfig.dlSchedulerStrategy))  
                {
                    alreadyScheduledUEs[internalCellIndex]++;
                }
                dlRetxFailQueueCount--;

                if(!IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
                {
                    /* + SPR 11712 Changes */
                    /** SPR 12457 Fix : Start**/
                    if(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus == NO_TB )
                        /** SPR 12457 Fix : End **/
                        /* - SPR 11712 Changes */
                    {
                        putEntryInSCellSchedulingQueue(
                                dlUEContext_p,
                                dlHarqProcess_p,
                                internalCellIndex);
                    }
                }

                /* Now This node needs to be deleted */
                /* SPR 10573 changes start */
                ret_val = checkAndPopReTxFailNode(dlReTxFailNode_p, FALSE, 
                        internalCellIndex);
                /* SPR 10573 changes end */
                if(MAC_FAILURE == ret_val)
                {
                    break;
                }
            }
        }
        else
        {
            if(deleteNode)
            {
                /* This node should be removed from the queue */
                /* SPR 10573 changes start */
                ret_val = checkAndPopReTxFailNode(dlReTxFailNode_p, 
                        deleteNode, internalCellIndex);
                /* SPR 10573 changes end */
                if(MAC_FAILURE == ret_val)
                {
                    break;
                }
            }
        }
        dlReTxFailNode_p = dlReTxFailNodeTmp_p;
        dlRetxFailQueueCount--;
        deleteNode = FALSE;
    }
}

/*****************************************************************************
 * Function Name  : isNodeValidForRetransmission
 * Inputs         : dlReTxFailNode_p - pointer to Re TX faliure queue node
 *                  globalTick - current global tick
 * Outouts        : deleteNode - flag to delete node or not
 * Description    : This function check if the UE retransmission is possible
 *                  in current tick.
 *****************************************************************************/
static  MacRetType isNodeValidForRetransmission(
     DLRetxQueueNode        *dlReTxNode_p,
     /* +- SPR 17777 */
     /* +- SPR 17777 */
     UInt8                  *deleteNode
#ifdef HD_FDD_CONFIG
     ,tickType_t                 globalTick
#endif
     /* +- SPR 17777 */
     /* +- SPR 17777 */
     )
{

    UInt16 ueIndex = 0;
    DLUEContext      *dlUEContext_p = PNULL;
    DLUEContextInfo  *dlUEContextInfo_p = PNULL;
    MeasGapContextDL *measgapContext_p = PNULL;

    ueIndex = dlReTxNode_p->ueIndex;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

     if(dlReTxNode_p->isNodeInvalid) 
     {
        /* Node is invalid hence needs to be deleted */
        *deleteNode = TRUE;
        return MAC_FAILURE;
     }
     if(dlUEContextInfo_p->pendingDeleteFlag)
     {
        *deleteNode = TRUE;
        return MAC_FAILURE;
     }
     /* SPR 8284 Fix Start */
     dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

     /* SPR 8284 Fix End */
#ifdef HD_FDD_CONFIG
     /* If HD FDD UE is in Retx Q,then check Schedule map whether 
      * it can be scheduled in currenttick or not*/
     if ((TRUE == dlUEContext_p->isHDFddFlag)
         && (!(CAN_SCHEDULE_DL_FOR_HD_UE(globalTick, ueIndex, hdUeContextInfo_g.hdfddUeSchedMap))))
     {
          UPDATE_UE_DENIED_DL_RESOURCE(ueIndex);
          return MAC_FAILURE;
     }
#endif

     measgapContext_p = &(dlUEContext_p->measGapContext);

     if (CHECK_UE_MEAS_GAP_PERIOD(measgapContext_p))
     {
           return MAC_FAILURE;
     }
     if ( (DL_QOS_BASED_SCHEDULING != 
                (cellSpecificParams_g.cellConfigAndInitParams_p[dlUEContext_p->internalCellIndex]->
                      initParams_p->dlSchdConfig.dlSchedulerStrategy)) && 
           (dlUEContext_p->isAlreadyConsideredFlag == dlReferenceFlag_g[dlUEContext_p->internalCellIndex]) )
     {
           return MAC_FAILURE;
     }
     return MAC_SUCCESS;
}
/* cyclomatic changes */
/*****************************************************************************
 * Function Name  : processDLRetransmissionQueue
 * Inputs         : globalTick - current Global Tick,
 *                  dlDelay - delay in DL path to phy,
 *                  maxUEsToBeScheduledDL_p - maximum number of UEs that can be
 *                                              scheduled in a single tick,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : numberOfAvailableRBs_p - No of RBs allcated are decremented
 *                                           and returned 
 *                  maxUEsToBeScheduledDL_p - After a UE is scheduled its count
 *                  is decremented and is returned
 * Returns        : None
 * Description    : This function processes the global Retransmission queue, and
 *                  schedules the UE first whose data is queued for
 *                  retransmissios
 *****************************************************************************/
                    /*SPR 15909 fix start*/
void processDLRetransmissionQueue( 
        void *numOfAvailableRBs_p,tickType_t  globalTick,
                    /*SPR 15909 fix start*/
#ifdef FDD_CONFIG
        UInt32 dlDelay,
#endif
        void *maxUEsToBeScheduledDL_p,

    InternalCellIndex internalCellIndex)
{
    UInt16 ueIndex                      = 0;
    DLRetxQueueNode *dlReTxNode_p       = PNULL;
    DLRetxQueueNode *dlReTxNodeTmp_p    = PNULL;
    DLUEContext *dlUEContext_p          = PNULL;
    DLUEContextInfo *dlUEContextInfo_p  = PNULL;
    DLHARQProcess *dlHarqProcess_p      = PNULL;
    UInt8 userLocationType              = CC_USER;
    UInt32 dlRetxQueueCount             = sQueueCount(dlRetransmissionQueue_gp[internalCellIndex]);
    UInt8 resourceRegionAllocated       = CC_CE_REGION;
    UInt32 tempRBCount = 0;
    UInt8 retxFlag = FALSE;
    UInt8 deleteNode[MAX_NUM_CELL] = {0};;
    MacRetType ret_val = MAC_FAILURE;
    
#ifdef FDD_CONFIG
    globalTick += dlDelay;
#endif
    UInt32 count = dlRetxQueueCount;
#ifdef SSI_DEBUG_STATS
    DLScheduleFlag scheduleFlag = DL_QOS_NEW_TRANSMISSION;
#endif
    UInt32 totalNumberOfAvailableRBs    = 0; 
    UInt16 totalMaxUEsToBeScheduledDL   = 0; 
    UInt8  retxLoopCount      = 0; 
    UInt8  scellLoopCount     = 0; 
    UInt8  loopIndex          = 0;     
    UInt8  scellHarqProcessId = 0;
    UInt32 usedRBs            = 0;
    InternalCellIndex cellIndex     = INVALID_CELL_INDEX;
    InternalCellIndex servCellIndex = INVALID_CELL_INDEX;
    UInt32 numberOfAvailableRBs[MAX_NUM_CELL][MAX_USER_LOCATION];
    UInt32 *tempNumOfAvailableRBs_p = PNULL;
    UInt8 *tempMaxUEsToBeScheduledDL_p = PNULL;
    /* + coverity 83632 */
    UInt8 maxUEsToBeScheduledDL[MAX_NUM_CELL] = {0};
    /* - coverity 83632 */
    UInt8 rowIndex = 0;
    UInt8 colIndex = 0;
    /* + SPR 11772 Changes */
    UInt8 schUsed = 0;
    /* - SPR 11772 Changes */

    /* + SPR 14341 Changes */
    ReTransmissionType reTransmissionType = INVALID_RETX_TYPE;
    ReTransmissionState reTransmissionState = INVALID_RETX_STATE;
    /* - SPR 14341 Changes */

    deleteInvalidNodeFromDLRetransmissionQueue(count, internalCellIndex);

    dlReTxNode_p = (DLRetxQueueNode *)getFirstNode(dlRetransmissionQueue_gp[internalCellIndex]);

    tempNumOfAvailableRBs_p = (UInt32 *)numOfAvailableRBs_p;
    tempMaxUEsToBeScheduledDL_p = (UInt8 *) maxUEsToBeScheduledDL_p;
    /*SPR 12015 Fix Start */
    updateNumAvailableRBsAndMaxDlUeSchedule(numberOfAvailableRBs,
            &totalNumberOfAvailableRBs,
            maxUEsToBeScheduledDL,
            &totalMaxUEsToBeScheduledDL,
            tempNumOfAvailableRBs_p,
            tempMaxUEsToBeScheduledDL_p,
            internalCellIndex);

    while (dlReTxNode_p && totalNumberOfAvailableRBs && 
            totalMaxUEsToBeScheduledDL && dlRetxQueueCount)
    {
        /* SPR 17240 Fix Start */
        dlRetxQueueCount--;
        /* SPR 17240 Fix End */
        deleteNode[0] = FALSE;
        deleteNode[1] = FALSE;

        /* + SPR 14341 Changes */
        reTransmissionType = INVALID_RETX_TYPE;
        reTransmissionState = INVALID_RETX_STATE;
        /* - SPR 14341 Changes */

        dlReTxNodeTmp_p = (DLRetxQueueNode *) getNextNode
            ((const LTE_SNODE *) (dlReTxNode_p));

        /* common validation for all cells */
        retxFlag = isNodeValidForRetransmission( 
                dlReTxNode_p, 
                /* +- SPR 17777 */
                &deleteNode[0]
#ifdef HD_FDD_CONFIG
                ,globalTick
#endif
                /* +- SPR 17777 */
                );

        if(MAC_FAILURE == retxFlag)
        {
            if(deleteNode[0])
            {
                /* This node should be removed from the queue */ 
                ret_val = checkAndPopReTxNode(dlReTxNode_p,internalCellIndex);
                if(MAC_FAILURE == ret_val)
                {
                    break;
                }
            }
            dlReTxNode_p = dlReTxNodeTmp_p;
            /* SPR 17240 Fix Start */
            /* dlRetxQueueCount--; */
            /* SPR 17240 Fix End */
            continue;
        }

        ueIndex = dlReTxNode_p->ueIndex;
        dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

        dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
        /* EICIC +*/
        /*  Lets check the UE type ( Victim / Non Victim ) required to be scheduled  

            1. Victim UE to be scheduled and the UE popped is non - victim 
            2. Non victim UE to be scheduled and the UE popped is Victim 
            3. Victim UE to be scheduled and the UE popped is victim 
            4. Non victim UE to be scheduled and the UE popped is non-Victim 
            */

        dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
        /*Complexity Start*/
        if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == dlUEContext_p->userType)) ||
                ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == dlUEContext_p->userType)))
        {   
            dlReTxNode_p = dlReTxNodeTmp_p;
            /* SPR 17240 Fix Start */
            /* dlRetxQueueCount--; */
            /* SPR 17240 Fix End */
            continue;
        }

        /*Complexity End*/

        /* EICIC - */
        retxLoopCount = (INVALID_HARQ_ID != dlReTxNode_p->harqProcessId ) ? 
            (dlReTxNode_p->sCellCount + 1 /* Pcell */): 
            (dlReTxNode_p->sCellCount);

        /* + SPR 14341 Changes */
        if(INVALID_HARQ_ID != dlReTxNode_p->harqProcessId)
        {
            reTransmissionType = (dlReTxNode_p->sCellCount)? 
                RETX_REQUIRED_ON_BOTH_CELLS: 
                RETX_REQUIRED_ON_PCELL_ONLY;
        }
        else if (dlReTxNode_p->sCellCount)
        {
            reTransmissionType = RETX_REQUIRED_ON_SCELL_ONLY;
        }
        /* - SPR 14341 Changes */

        for (loopIndex = 0, scellLoopCount = 0;loopIndex < retxLoopCount; loopIndex++)
        {
            if( (INVALID_HARQ_ID != dlReTxNode_p->harqProcessId) && 
                    (0 == loopIndex))
            {
                cellIndex = dlUEContext_p->internalCellIndex;
                dlHarqProcess_p = &(dlUEContext_p->dlHarqContext_p->
                        harqProcess[dlReTxNode_p->harqProcessId]);
                /* + SPR 11772 Changes */
                schUsed = dlReTxNode_p->schUsed;
                /* - SPR 11772 Changes */
            }
            else
            {
                servCellIndex = dlReTxNode_p->sCellRetxInfo[scellLoopCount].servCellIndex;
                cellIndex = getInternalCellIdFromServeCellId ( dlUEContext_p,servCellIndex);
                /* + SPR_14522 */
                if (INVALID_8_VAL == cellIndex)
                {
                    /* + SPR 14583 Changes */
                    if(reTransmissionType == RETX_REQUIRED_ON_SCELL_ONLY)
                    {
                        reTransmissionState = RETX_ALLOC_SUCC_ON_SCELL;
                    }
                    else if(reTransmissionType == RETX_REQUIRED_ON_BOTH_CELLS)
                    {
                        if(reTransmissionState == RETX_ALLOC_SUCC_ON_PCELL)
                        {
                            reTransmissionState = RETX_ALLOC_SUCC_ON_BOTH_CELLS;
                        }
                        else
                        {
                            reTransmissionState = RETX_ALLOC_SUCC_ON_SCELL;
                        }
                    }
                    /* - SPR 14583 Changes */
                    break;
                }    
                /* - SPR_14522 */
                scellHarqProcessId = dlReTxNode_p->sCellRetxInfo[scellLoopCount].harqProcessId;
                dlHarqProcess_p = &(dlUEContext_p->dlUeScellContext_p[servCellIndex]->\
                        ScelldlHarqContext_p->harqProcess[scellHarqProcessId]);
                /* + SPR 11772 Changes */
                schUsed = dlReTxNode_p->sCellRetxInfo[scellLoopCount].schUsed;
                /* - SPR 11772 Changes */
                scellLoopCount++;
            }
            retxFlag = isNodeValidForRetransmissionOnCell( 
                    ueIndex,
                    numberOfAvailableRBs[cellIndex], 
                    &deleteNode[loopIndex],
                    &resourceRegionAllocated,
                    globalTick
#ifdef FDD_CONFIG
                    , dlDelay
#endif
                    ,cellIndex,
                    dlHarqProcess_p,
                    maxUEsToBeScheduledDL[cellIndex]);

            if(!retxFlag)
            {
                /* SPR 19704 + */
                /* If SPS Retx harq collides with new sps transmission 
                   Then discard the previouse retransmission 
                   Discarding will be done in next TTI */
                if ((dlUEContext_p->schType == SPS_SCHEDULING) &&
                    (dlHarqProcess_p->harqProcessId == dlUEContext_p->spsDlInfo.hpId ))
                {
                    dlReTxNode_p->isNodeInvalid = TRUE;
                }
                /* SPR 19704 - */
                /* SPR 11912 Changes Start */
                if (deleteNode[loopIndex])
                {
                    /* + SPR 14341 Changes */
                    if(reTransmissionType == RETX_REQUIRED_ON_PCELL_ONLY)
                    {
                        reTransmissionState = RETX_ALLOC_SUCC_ON_PCELL;
                    }
                    else if(reTransmissionType == RETX_REQUIRED_ON_SCELL_ONLY)
                    {
                        reTransmissionState = RETX_ALLOC_SUCC_ON_SCELL;
                    }
                    /* - SPR 14341 Changes */
                    freeDLHarqProcess(dlUEContext_p, dlHarqProcess_p, cellIndex);
                }
                /* SPR 11912 Changes End */
                if(deleteNode[loopIndex] && deleteNode[retxLoopCount - loopIndex - 1])
                {
                    /* + SPR 14341 Changes */
                    if(reTransmissionType == RETX_REQUIRED_ON_BOTH_CELLS)
                    {
                        reTransmissionState = RETX_ALLOC_SUCC_ON_BOTH_CELLS;
                    }
                    /* - SPR 14341 Changes */
                }
                continue;
            }
            if(IS_PCELL_OF_UE(dlUEContext_p,cellIndex))
            {
                userLocationType = dlUEContext_p->userLocationType;
#ifdef SSI_DEBUG_STATS
                scheduleFlag = dlUEContext_p->scheduleFlag;
#endif
            }
            else
            {
                userLocationType = dlUEContext_p->dlUeScellContext_p[servCellIndex]->userLocationType;
#ifdef SSI_DEBUG_STATS
                scheduleFlag = dlUEContext_p->dlUeScellContext_p[servCellIndex]->scheduleFlag;
                /*transmissionMode =
                  dlUEContext_p->dlUeScellContext_p[servCellIndex]->ScelldlMIMOInfo.transmissionMode;*/
#endif
            }

            tempRBCount = numberOfAvailableRBs[cellIndex][resourceRegionAllocated];

            /* + SPR 11772 Changes */
            if (MAC_SUCCESS == assignRBsForReTx (dlUEContext_p, dlHarqProcess_p, 
                        &numberOfAvailableRBs[cellIndex][resourceRegionAllocated], 
                        resourceRegionAllocated, schUsed,cellIndex ))
                /* - SPR 11772 Changes */
            {
                usedRBs = (tempRBCount - 
                        numberOfAvailableRBs[cellIndex][resourceRegionAllocated]);
                if (resourceRegionAllocated == CE_REGION)
                {
                    numberOfAvailableRBs[cellIndex][CC_CE_REGION] -= usedRBs;
                }
                else
                {
                    if ((numberOfAvailableRBs[cellIndex][CC_CE_REGION] < 
                                numberOfAvailableRBs[cellIndex][CE_REGION]))
                    {
                        numberOfAvailableRBs[cellIndex][CE_REGION] =
                            numberOfAvailableRBs[cellIndex][CC_CE_REGION];
                    }
                }

                totalNumberOfAvailableRBs -= usedRBs;
                dlUEContext_p->isAlreadyConsideredFlag = dlReferenceFlag_g[internalCellIndex];
#ifdef SSI_DEBUG_STATS
                LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCHEDULE_FLAG
                    (ueIndex, scheduleFlag);
#endif

                maxUEsToBeScheduledDL[cellIndex]--;
                totalMaxUEsToBeScheduledDL--;
                /* Update Cell Edge Ue */
                if ( CE_USER == userLocationType )
                {
                    maxCellEdgeUEsToBeScheduledDL_g[cellIndex]--;
                }
                /** SPR 13599 Changes Start **/
                if(IS_CA_ENABLED())
                {
                    dlQosPreparePreferredCellListForReTx(
                            dlUEContext_p,
                            dlReTxNode_p);
                }
                /** SPR 13599 Changes End **/
                /* used only in RR */
                if (DL_QOS_BASED_SCHEDULING != (cellSpecificParams_g.cellConfigAndInitParams_p[\
                            dlUEContext_p->internalCellIndex]->initParams_p->\
                            dlSchdConfig.dlSchedulerStrategy))  
                {
                    alreadyScheduledUEs[cellIndex]++;
                }
                if(!IS_PCELL_OF_UE(dlUEContext_p, cellIndex))
                {
                    /* + SPR 11712 Changes */
                    /** SPR 12457 Fix : Start**/
                    if(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus == NO_TB )
                        /** SPR 12457 Fix : End **/
                        /* - SPR 11712 Changes */
                    {
                        putEntryInSCellSchedulingQueue(
                                dlUEContext_p,
                                dlHarqProcess_p,
                                cellIndex);
                    }
                }
                /* + SPR 14341 Changes */
                SET_RETX_STATE_ON_RB_ALLOC_SUCCESS(reTransmissionType, reTransmissionState);
                /* - SPR 14341 Changes */

                LOG_MAC_MSG(MAC_ALLOCATED_RBS_FOR_RETX,LOGDEBUG,MAC_DL_Strategy,
                        getCurrentTick(),
                        dlUEContext_p->ueIndex,
                        dlUEContext_p->internalCellIndex,
                        cellIndex,
                        usedRBs,
                        dlHarqProcess_p->harqProcessId,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME, "");
            }
            else
            {
                /* + SPR 14341 Changes */
                SET_RETX_STATE_ON_RB_ALLOC_FAIL(reTransmissionType, reTransmissionState);
                /* - SPR 14341 Changes */
                LOG_MAC_MSG(MAC_RB_ALLOCATION_FAIL_FOR_RETX,LOGWARNING,MAC_DL_Strategy,
                        getCurrentTick(),
                        dlUEContext_p->ueIndex,
                        dlUEContext_p->internalCellIndex,
                        cellIndex,
                        dlHarqProcess_p->harqProcessId,
                        retxLoopCount,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME, "");
                /* Break from For loop */
                break;
            }
        }
        /* + SPR 14341 Changes */
        if( (reTransmissionState == RETX_ALLOC_SUCC_ON_PCELL && reTransmissionType == RETX_REQUIRED_ON_PCELL_ONLY) ||
                (reTransmissionState == RETX_ALLOC_SUCC_ON_SCELL && reTransmissionType == RETX_REQUIRED_ON_SCELL_ONLY) ||
                (reTransmissionState == RETX_ALLOC_SUCC_ON_BOTH_CELLS && reTransmissionType == RETX_REQUIRED_ON_BOTH_CELLS))
        {
            /* Now This node needs to be deleted */
            ret_val = checkAndPopReTxNode(dlReTxNode_p,internalCellIndex);
            if(MAC_FAILURE == ret_val)
            {
                break;
            } 
        }
        else if( (reTransmissionState == RETX_ALLOC_SUCC_ON_PCELL && reTransmissionType == RETX_REQUIRED_ON_BOTH_CELLS))
        {
            /* Mark harq process invalid for pcell only */
            dlReTxNode_p->harqProcessId = INVALID_HARQ_ID;
        }
        else if( (reTransmissionState == RETX_ALLOC_SUCC_ON_SCELL && reTransmissionType == RETX_REQUIRED_ON_BOTH_CELLS))
        {
            /* Mark harq process invalid for scell only */
            /* + SPR 14583 Changes */
            dlReTxNode_p->sCellCount = 0;
            /* - SPR 14583 Changes */
            dlReTxNode_p->sCellRetxInfo[0].harqProcessId = INVALID_HARQ_ID;
        }
        else if( ( (reTransmissionState == RETX_ALLOC_FAIL_ON_PCELL) && 
                    ((reTransmissionType == RETX_REQUIRED_ON_BOTH_CELLS) || 
                     (reTransmissionType == RETX_REQUIRED_ON_PCELL_ONLY))) ||
                (reTransmissionState == RETX_ALLOC_FAIL_ON_SCELL && reTransmissionType == RETX_REQUIRED_ON_SCELL_ONLY) ||
                (reTransmissionType != INVALID_RETX_TYPE && reTransmissionState == INVALID_RETX_STATE))
        {
            /* Do nothing. Node will be processed in next TTI */
        }
        /* - SPR 14341 Changes */
        /* Move to next node */
        dlReTxNode_p = dlReTxNodeTmp_p;
        /* SPR 17240 Fix Start */
        /* dlRetxQueueCount--; */
        /* SPR 17240 Fix End */
    }
    if(!IS_CA_ENABLED())
    {
        tempNumOfAvailableRBs_p[CC_CE_REGION] = 
                numberOfAvailableRBs[internalCellIndex][CC_CE_REGION];
        tempNumOfAvailableRBs_p[CE_REGION] = 
                numberOfAvailableRBs[internalCellIndex][CE_REGION];
        *tempMaxUEsToBeScheduledDL_p = maxUEsToBeScheduledDL[internalCellIndex];
    }
    else
    {
	    for(rowIndex = 0; rowIndex < MAX_NUM_CELL; rowIndex++)
	    {
           for(colIndex= 0; colIndex < MAX_USER_LOCATION; colIndex++)
	       {
		       tempNumOfAvailableRBs_p[(2*rowIndex)+ colIndex] = numberOfAvailableRBs[rowIndex][colIndex];
	       }
	    }
        tempMaxUEsToBeScheduledDL_p[internalCellIndex] = maxUEsToBeScheduledDL[internalCellIndex];
        tempMaxUEsToBeScheduledDL_p[!(internalCellIndex)] = maxUEsToBeScheduledDL[!(internalCellIndex)];
    }

    return;
}
/*****************************************************************************
 * Function Name  : areTokensAvailableForEligbleLC
 * Inputs         : dlUEContext_p -  pointer to DL UE context,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : If tokens available then return TRUE otherwise 
 *                  returned FALSE
 * Returns        : MAC_SUCCESS/ MAC_FAILURE
 * Description    : This function is used to determine whether tokens are 
 *                  available or not.
 *****************************************************************************/
UInt8 areTokensAvailableForEligbleLC(DLUEContext * ueDLContext_p,
                                     InternalCellIndex internalCellIndex)
{

    UInt8 retVal = 0;
    void   *dlEligibleLcNodeVoid_p = NULL;
    DLQosStrategyLCNode *dlEligibleLcNode_p = NULL;
    DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;

    if ( DL_QOS_BASED_SCHEDULING != cellSpecificParams_g.cellConfigAndInitParams_p
                    [internalCellIndex]->initParams_p->dlSchdConfig.dlSchedulerStrategy)
    {
        return 1;
    }

    dlEligibleLcNodeVoid_p = (void*) getFirstListNode(
            &ueDLContext_p->dlEligibleLcList);
    while(PNULL != dlEligibleLcNodeVoid_p)
    {
        DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                dlEligibleLcNodeVoid_p, dlEligibleLcNode_p);

        dlQosStrategyLCNode_p = (DLQosStrategyLCNode *)dlEligibleLcNode_p;
        if(dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.availToken)
        {
            retVal = 1;
            break;
        }

        /* Insert the Lc Node in the scheduled LC list */
        dlEligibleLcNodeVoid_p = (void*)
            getNextListNode(
                    &(dlEligibleLcNode_p->eligibleLcAnchor));

    };
    return retVal;

}

/*SPR 3679 QOS Changes End*/
/*****************************************************************************
 * Function Name  : assignRBsForReTx
 * Inputs         : dlUEContext_p -  pointer to DL UE context 
 *                  dlHarqProcess_p - pointer to the harq process
 *                  numberOfAvailableRBs_p - number of RB available,
 *                  resourceRegionAllocated - Used to determine region allocated
 *                  schUsed - Scheduler used
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : numberOfAvailableRBs_p - RBs allocated shall be decremented 
 *                  and returned as output
 * Returns        : MAC_SUCCESS/ MAC_FAILURE
 * Description    : This function based on the transmission mode, sets the DL
 *                  Harq TB info for RE_TX and calls the appropriate function to
 *                  push a node in the DL Scheduler Re-TX queue
 *****************************************************************************/
MacRetType assignRBsForReTx(DLUEContext *dlUEContext_p, DLHARQProcess
        *dlHarqProcess_p, UInt32 *numberOfAvailableRBs_p, 
        UInt8 resourceRegionAllocated, UInt8 schUsed,
        InternalCellIndex internalCellIndex) 
/* SPR 2446 Changes */
{
    /*SPR 8187 Fix Start*/
    MacRetType retVal = MAC_SUCCESS;
    /*SPR 8187 Fix End*/
    /*coverity id 62171*/
    TransmissonMode transMode = TX_MODE_ATTACH_NOT_COMPLETE; 
    /* SPR 3679 Changes Start */
    UInt32 tempTBSizeRet = 0; 
    UInt8  tb1OnlyFlag = FALSE;
    UInt8  userLocationType = CC_USER;
    DLScheduleFlag* scheduleFlag_p = PNULL;
    DCIFormat tm6DCIFormat = INVALID_DCI_FORMAT;
    DLCQIInfo* dlCQIInfo_p = PNULL;
    UInt8 dlRi = 0;;

    if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
    {
        userLocationType = dlUEContext_p->userLocationType;
        scheduleFlag_p = &(dlUEContext_p->scheduleFlag);
        transMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
        dlCQIInfo_p = &(dlUEContext_p->dlCQIInfo);
        tm6DCIFormat = dlUEContext_p->tm6DCIFormat;
        dlRi = dlUEContext_p->dlMIMOInfo.dlRi;
    }
    else
    {
        userLocationType = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
        scheduleFlag_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag);
        transMode = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.transmissionMode;
        dlCQIInfo_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
        tm6DCIFormat = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->Scelltm6DCIFormat;
        dlRi = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.dlRi;
    }
    /* SPR 3679 Changes End */
    if (TX_MODE_3 == transMode || TX_MODE_4 == transMode ||
          /* + TM7_8_Changes Start */
          ((TX_MODE_7 == transMode || TX_MODE_8 == transMode) &&
          (1 < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas)))
          /* - TM7_8_Changes Start */
    {
        /* SPR 3679 Changes Start */
     	/* SPR 7981 changes start */
     	/* SPR 10110 changes start */
        if (
            (RANK_INDICATOR_1 == dlRi) ||
     	/* SPR 10110 changes end */
            (dlCQIInfo_p->mcsIndexCodeWordOne <=
             cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->
             dlSchdConfig.mcsThreshold[userLocationType]) ||
            (dlCQIInfo_p->mcsIndexCodeWordTwo <= 
             cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->
             dlSchdConfig.mcsThreshold[userLocationType]) ||
	/* SPR 7981 changes end */
            /* SPR 4493 START */
            (DCI_FORMAT_1A == dlHarqProcess_p->dciFormat))
        /* SPR 4493 END */
        {
            tb1OnlyFlag = TRUE;
        }

        if (tb1OnlyFlag)
        {
            if ((dlHarqProcess_p->isTBOneValid) &&
                    (dlHarqProcess_p->isTBTwoValid))
            {
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;
                dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator = RE_TX;

                /*SPR 8187 Fix Start*/
                retVal = processDLNackTwoTBWithZeroQueueLoad
                    (
                     /* +- SPR 17777 */
                     numberOfAvailableRBs_p,dlUEContext_p,
                     dlHarqProcess_p,
                     TB_ONE_RETX_TB_TWO_RETX,
                     /* SPR 5313 Fix start */
                     resourceRegionAllocated,
                     /* SPR 5313 Fix end */
                     /*SPS_TDD_Chnages*/
                     DRA_SCHEDULING,
                     /*SPS_TDD_Chnages*/
                /*SPR 8187 Fix End*/
                     internalCellIndex);
            }
            else if (dlHarqProcess_p->isTBOneValid)
            {
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;

                /*SPR 8187 Fix Start*/
                retVal = processDLNackTwoTBWithZeroQueueLoad
                    (
                     /* +- SPR 17777 */
                     numberOfAvailableRBs_p,dlUEContext_p,
                     dlHarqProcess_p,
                     TB_ONE_RETX_TB_TWO_INVALID,
                     /* SPR 5313 Fix start */
                     resourceRegionAllocated,
                     /* SPR 5313 Fix end */
                     /*SPS_TDD_Chnages*/
                     schUsed,
                     /*SPS_TDD_Chnages*/
                /*SPR 8187 Fix End */
                     internalCellIndex);
            }
            else
            {
                dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator = RE_TX;

                /*SPR 8187 Fix Start */
                retVal = processDLNackTwoTBWithZeroQueueLoad
                    (
                     /* +- SPR 17777 */
                     numberOfAvailableRBs_p,dlUEContext_p,
                     dlHarqProcess_p,
                     TB_ONE_INVALID_TB_TWO_RETX,
                     /* SPR 5313 Fix start */
                     resourceRegionAllocated,
                     /* SPR 5313 Fix end */
                     /*SPS_TDD_Chnages*/
                     DRA_SCHEDULING,
                     /*SPS_TDD_Chnages*/
                /*SPR 8187 Fix End */
                     internalCellIndex);
            }
        }
        else
        {
            /* SPR 3679 Changes End */
            if (dlHarqProcess_p->isTBOneValid && !dlHarqProcess_p->isTBTwoValid)
            {
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;

                /* SPR 3679 Changes Start */
                /*4X4 DL MIMO CHG START*/
                tempTBSizeRet = determineTBSize(
                        dlCQIInfo_p->mcsIndexCodeWordTwo,
                        (dlHarqProcess_p)->assignedResourcesNB,
                        TB_MAPPED_TO_ONE_LAYER);
                /*4X4 DL MIMO CHG END*/
                if ((NET_UE_QL(dlUEContext_p) > ZERO) &&
                        (tempTBSizeRet > 
                         HARQ_TB_MINIMUM_SIZE_IN_BYTES))
                        /* SPR# 5189 Changes Starts */
                       /* && areTokensAvailableForEligbleLC(dlUEContext_p))*/
                        /* SPR# 5189 Changes Ends */
                {
                    dlHarqProcess_p->isTBTwoValid = TRUE ;
                    dlHarqProcess_p->dlHARQTBOneInfo.txIndicator
                        = RE_TX;
                    dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator
                        = NEW_TX;

                    /*SPR 8187 Fix Start */
                    retVal = processDLNackTwoTBWithNonZeroQueueLoad
                        (
                         /* +- SPR 17777 */
                         numberOfAvailableRBs_p,
                         dlUEContext_p,
                         dlHarqProcess_p,
                         TB_ONE_RETX_TB_TWO_NEW,
                         /* SPR 5313 Fix start */
                         resourceRegionAllocated,
                         /* SPR 5313 Fix end */
                    /*SPR 8187 Fix End */
                         internalCellIndex);
                }
                else
                {
                    dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;

                    *scheduleFlag_p = DL_QOS_NO_NEW_TRANSMISSION;

                    /*SPR 8187 Fix Start*/
                    retVal = processDLNackTwoTBWithZeroQueueLoad
                        (
                         /* +- SPR 17777 */
                         numberOfAvailableRBs_p,
                         dlUEContext_p,
                         dlHarqProcess_p,
                         TB_ONE_RETX_TB_TWO_INVALID,
                         /* SPR 5313 Fix start */
                         resourceRegionAllocated,
                         /* SPR 5313 Fix end */
                         /*SPS_TDD_Changes*/
                         DRA_SCHEDULING,
                         /*SPS_TDD_Changes*/
                    /*SPR 8187 Fix End*/
                         internalCellIndex);
                }
                /* SPR 3679 Changes End */
            }
            else if (!dlHarqProcess_p->isTBOneValid &&
                    dlHarqProcess_p->isTBTwoValid)
            {
                dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator = RE_TX;
                /* SPR 3679 Changes Start */
                /*4X4 DL MIMO CHG START*/
                tempTBSizeRet = determineTBSize(
                        dlCQIInfo_p->mcsIndexCodeWordOne,
                        dlHarqProcess_p->assignedResourcesNB,TB_MAPPED_TO_ONE_LAYER);
                /*4X4 DL MIMO CHG END*/
                if ((NET_UE_QL(dlUEContext_p) > ZERO)&&
                        (tempTBSizeRet > HARQ_TB_MINIMUM_SIZE_IN_BYTES))
                {
                    dlHarqProcess_p->isTBOneValid = TRUE;
                    dlHarqProcess_p->dlHARQTBOneInfo.txIndicator
                        = NEW_TX;
                    dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator
                        = RE_TX;

                    /*SPR 8187 Fix Start*/
                    retVal = processDLNackTwoTBWithNonZeroQueueLoad
                        (
                         /* +- SPR 17777 */
                         numberOfAvailableRBs_p,
                         dlUEContext_p,
                         dlHarqProcess_p,
                         TB_ONE_NEW_TB_TWO_RETX,
                         /* SPR 5313 Fix start */
                         resourceRegionAllocated,
                         /* SPR 5313 Fix end */
                    /*SPR 8187 Fix End */
                         internalCellIndex);
                }
                else
                {
                    dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator = RE_TX;

                    *scheduleFlag_p = DL_QOS_NO_NEW_TRANSMISSION;

                    /*SPR 8187 Fix Start*/
                    retVal = processDLNackTwoTBWithZeroQueueLoad
                        (
                         /* +- SPR 17777 */
                         numberOfAvailableRBs_p,
                         dlUEContext_p,
                         dlHarqProcess_p,
                         TB_ONE_INVALID_TB_TWO_RETX,
                         /* SPR 5313 Fix start */
                         resourceRegionAllocated,
                         /* SPR 5313 Fix end */
                         /*SPS_TDD_Changes*/
                         DRA_SCHEDULING,
                         /*SPS_TDD_Changes*/
                    /*SPR 8187 Fix End */
                         internalCellIndex);
                }
                /* SPR 3679 Changes End */
            }
            else
            {
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;
                dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator = RE_TX;

                /* SPR 3679 Changes Start */
                /*SPR 8187 Fix Start*/
                retVal = processDLNackTwoTBWithZeroQueueLoad
                    (
                     /* +- SPR 17777 */
                     numberOfAvailableRBs_p,
                     dlUEContext_p,
                     dlHarqProcess_p,
                     TB_ONE_RETX_TB_TWO_RETX,
                     /* SPR 5313 Fix start */
                     resourceRegionAllocated,
                     /*SPS_TDD_Changes*/
                     DRA_SCHEDULING,
                     /*SPS_TDD_Changes*/
                     /* SPR 5313 Fix end */
                     internalCellIndex);
                /* SPR 3679 Changes End */
                /*SPR 8187 Fix End */
            }
            /* SPR 3679 Changes Start */
            /* SPR 3679 Changes End */
        }
        LOG_MAC_MSG(MAC_RBS_ASSIGNED_FOR_RETX_1,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                dlUEContext_p->ueIndex,
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator,
                dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator,
                dlUEContext_p->scheduleFlag,
                dlHarqProcess_p->harqProcessId,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME, "");
    }
    /* + TM6_5.2 */
    else if (TX_MODE_6 == transMode)
    {
        dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;
        if((DCI_FORMAT_1A == dlHarqProcess_p->dciFormat) 
            && (DCI_FORMAT_1B == tm6DCIFormat))
        {
           dlHarqProcess_p->dciFormat = DCI_FORMAT_1B;
        }
        /*SPR 8187 Fix Start*/
        retVal = processDLNackTwoTBWithZeroQueueLoad
            (
             /* +- SPR 17777 */
             numberOfAvailableRBs_p,dlUEContext_p,
             dlHarqProcess_p,
             TB_ONE_RETX_TB_TWO_INVALID,
             /* SPR 5313 Fix start */
             resourceRegionAllocated,
            /* SPR 5313 Fix end */
             /*SPS_TDD_Changes*/
             DRA_SCHEDULING,
             /*SPS_TDD_Changes*/
        /*SPR 8187 Fix End */
             internalCellIndex);
        LOG_MAC_MSG(MAC_RBS_ASSIGNED_FOR_RETX_2,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                dlUEContext_p->ueIndex,
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator,
                dlUEContext_p->scheduleFlag,
                dlHarqProcess_p->harqProcessId,
                tm6DCIFormat,                
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME, "");
    }
    /* - TM6_5.2 */
    else
    {
        dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;

        /* SPR 2446 Fix Begins*/
        LTE_MAC_INCREMENT_SPS_HARQ_STATS( dlUEContext_p->ueIndex, 
                dlUEContext_p, getCurrentTick(), schUsed, 
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator);

        processDLNackForOneTBOnly(numberOfAvailableRBs_p, dlUEContext_p,
                dlHarqProcess_p, resourceRegionAllocated, schUsed,
                internalCellIndex);
        /* SPR 2446 Fix Ends*/        
        LOG_MAC_MSG(MAC_RBS_ASSIGNED_FOR_RETX_3,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                dlUEContext_p->ueIndex,
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator,
                dlUEContext_p->scheduleFlag,
                dlHarqProcess_p->harqProcessId,
                __LINE__,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME, "");

    }

    /*SPR 8187 Fix Start*/
    return retVal;
    /*SPR 8187 Fix End*/
}

/* SPR 3679 Changes Start */


/*****************************************************************************
 * Function Name  : processDLNackTwoTBWithNonZeroQueueLoad
 * Inputs         : allocatedRBFromRoundRobin- The Number of RBs Allocated 
 *                     Orginally
 *                  availableRB_p -Pointer to the number of Avavailable RBs
 *                  ueDLContext_p - Pointer to UE Context
 *                  harqProcess_p - Pointer to UEs DL HARQ Context
 *                  tBOneTBTwoReTxNewInfo- information about the type of 
 *                     transmission of two TBs i.e. either New Transmission or Re-
 *                     transmissions
 *                  resourceRegionAllocated - CCor CE region allocated,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : Process for UE's DL Nack for Two TBs with Non Zero Queue Load
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : Its main function is for processing the UE for 
 *                  Re-Transmission for Two TBs when the UE  is having Non Zero 
 *                  Queue Load.One TB is used for the retransmission and other TB
 *                  is used for new transmission. The output is prepared  and 
 *                  sent to PDCCH Module
 * *****************************************************************************/
MacRetType processDLNackTwoTBWithNonZeroQueueLoad(
        /* +- SPR 17777 */
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo,
        /* SPR 5313 Fix start */
        UInt8 resourceRegionAllocated,
        /* SPR 5313 Fix end */
        InternalCellIndex internalCellIndex)
{
    ResourceAllocatorInput * tempResourceAllocatorInput_p = PNULL;
    DLStrategyTxNode * dlStrategyTxNode_p=PNULL;

        /*SPR 20899 Fix Start*/
    UInt8 strategyContainerIndex = MODULO_TWO(getCurrentTick()); 
        /*SPR 20899 Fix Stop*/

    /* SPR 3679 QOS Changes Start */
    DLUeScheduleInfo * dlUeScheduleInfo_p = PNULL;
    /* SPR 3679 QOS Changes End */
    /*4X4 DL MIMO CHG START*/
    UInt32 mcsThresholdLayer3 = 0;
    UInt32 mcsThresholdLayer4 = 0;
    UInt8  newTXNumOfLayer    = 0;
    /*4X4 DL MIMO CHG END*/
    UInt8  userLocationType = CC_USER;
    UInt8 dlRi = 0;
    DLScheduleFlag* scheduleFlag_p = PNULL;
    DLCQIInfo* dlCQIInfo_p = PNULL;
    DLStrategyTxNode** ueDlStrategyTxNode_p =PNULL;

    if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
    {
        userLocationType = ueDLContext_p->userLocationType;
        dlRi = ueDLContext_p->dlMIMOInfo.dlRi;
        scheduleFlag_p = &(ueDLContext_p->scheduleFlag);
        dlCQIInfo_p = &(ueDLContext_p->dlCQIInfo);
        ueDlStrategyTxNode_p = &(ueDLContext_p->dlStrategyTxNode_p);
    }
    else
    {
        userLocationType = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
        dlRi = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.dlRi;
        scheduleFlag_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag);
        dlCQIInfo_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
        ueDlStrategyTxNode_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);
    }
    /*Preparing the DL Strategy Output Node*/
    GET_MEM_FROM_POOL(DLStrategyTxNode,dlStrategyTxNode_p,
            sizeof(DLStrategyTxNode),PNULL);

    /* + coverity 24534 */
    if (PNULL == dlStrategyTxNode_p)
    {
        freeDLHarqProcess (ueDLContext_p, harqProcess_p, internalCellIndex);
        return MAC_FAILURE;
    }
    /* - coverity 24534 */

    tempResourceAllocatorInput_p = &(dlStrategyTxNode_p->resourceAllocInput);
    tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
    /* SPR 12239 Fix start */
    tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;
    /* SPR 12239 Fix end */
    tempResourceAllocatorInput_p->strictAllocSubBand  = FALSE;
    dlStrategyTxNode_p->isCQIRequestedULGrantMsg        = FALSE;

    /* Updating the dlStrategyTxNode's Fields*/
    dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;
    dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId;

    if (!(dlCQIInfo_p->aperiodicCQIRequested)&&
         ( (harqProcess_p->dlHARQTBOneInfo.current_TX_NB >= MAX_RETX_COUNT_FOR_STRICT_SUBBAND) || 
           (harqProcess_p->dlHARQTBTwoInfo.current_TX_NB >=  MAX_RETX_COUNT_FOR_STRICT_SUBBAND)) &&
         (dlCQIInfo_p->cqiMode != INVALID_APERIODIC_CQI_MODE) )
    {
        dlStrategyTxNode_p->isCQIRequestedULGrantMsg = TRUE;
        dlCQIInfo_p->aperiodicCQIRequested = TRUE;
    }

    switch (tBOneTBTwoReTxNewInfo){
        /*When TB One Retransmission to be done, TB Two can be used for the 
         *           New Transmission */
        case TB_ONE_RETX_TB_TWO_NEW:
            {

                harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize =
                    harqProcess_p->dlHARQTBOneInfo.tbSize;
                  
                /*4x4 DL MIMO CHG START*/

                /* SPR 11355 Fix Start */
                if(((ueDLContext_p->ueCategory == UE_CAT_5) || (ueDLContext_p->ueCategory == UE_CAT_8)))
                /* SPR 11355 Fix End */
                { 
                    mcsThresholdLayer3 = getMCSFromCQI( 
                            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer3[userLocationType]
                            );
                    mcsThresholdLayer4 = getMCSFromCQI(
                            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer4[userLocationType]
                           );
                 
                    if((dlCQIInfo_p->mcsIndexCodeWordTwo > mcsThresholdLayer4) && 
                        (RANK_INDICATOR_4 == dlRi))                        
                    {
                        newTXNumOfLayer = RANK_INDICATOR_4;
                    }
                    else if((dlCQIInfo_p->mcsIndexCodeWordTwo > mcsThresholdLayer3) &&
                           (RANK_INDICATOR_3 == dlRi))
                    {
                        newTXNumOfLayer = RANK_INDICATOR_3;
                    }
                    else
                    {
                        newTXNumOfLayer = RANK_INDICATOR_2;
                    }
                    
                    if( newTXNumOfLayer >=RANK_INDICATOR_3 && 
                        harqProcess_p->numOfLayer == RANK_INDICATOR_2)
                    {
                        harqProcess_p->dlHARQTBTwoInfo.tbSize = 
                                    determineTBSize(
                                            dlCQIInfo_p->mcsIndexCodeWordTwo,
                                            harqProcess_p->assignedResourcesNB,
                                            TB_MAPPED_TO_TWO_LAYER);
                        harqProcess_p->numOfLayer++;
                    }
                    else if( newTXNumOfLayer >=RANK_INDICATOR_3 && 
                             harqProcess_p->numOfLayer == RANK_INDICATOR_3)
                    {
                        harqProcess_p->dlHARQTBTwoInfo.tbSize = 
                                    determineTBSize(
                                            dlCQIInfo_p->mcsIndexCodeWordTwo,
                                            harqProcess_p->assignedResourcesNB,
                                            TB_MAPPED_TO_TWO_LAYER);
                    }
                    else if( harqProcess_p->numOfLayer == RANK_INDICATOR_4 && 
                             newTXNumOfLayer < RANK_INDICATOR_3)
                    {
                        harqProcess_p->isTBTwoValid = FALSE;
                    }
                    else if( harqProcess_p->numOfLayer == RANK_INDICATOR_4 && 
                             newTXNumOfLayer >= RANK_INDICATOR_3)
                    {
                        harqProcess_p->dlHARQTBTwoInfo.tbSize = 
                                    determineTBSize(
                                          dlCQIInfo_p->mcsIndexCodeWordTwo,
                                          harqProcess_p->assignedResourcesNB,
                                          TB_MAPPED_TO_TWO_LAYER);
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBTwoInfo.tbSize = 
                                    determineTBSize(
                                         dlCQIInfo_p->mcsIndexCodeWordTwo,
                                         harqProcess_p->assignedResourcesNB,
                                         TB_MAPPED_TO_ONE_LAYER);     
                        harqProcess_p->numOfLayer = RANK_INDICATOR_2;   
                    }
                }   /*4x4 DL MIMO CHG END*/ 
                else
                {
                    harqProcess_p->dlHARQTBTwoInfo.tbSize = 
                                    determineTBSize(
                                         dlCQIInfo_p->mcsIndexCodeWordTwo,
                                         harqProcess_p->assignedResourcesNB,
                                         TB_MAPPED_TO_ONE_LAYER);
                }                
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = 
                    dlCQIInfo_p->mcsIndexCodeWordTwo;                    
                /* SPR 3679 QOS Changes Start */
                totalRbMIMO_g[internalCellIndex] += harqProcess_p->assignedResourcesNB;
                if(harqProcess_p->isTBTwoValid)
                {
                   *scheduleFlag_p = DL_QOS_ALREADY_TRANSMITED;
                }
                /* SPR 3679 QOS Changes End */
            }
            break;
            /*When TB Two Retransmission to be done, TB One can be used 
             *               for the New Transmission */
        case TB_ONE_NEW_TB_TWO_RETX:
            {

                harqProcess_p->dlHARQTBTwoInfo.retransmissionTBSize =
                    harqProcess_p->dlHARQTBTwoInfo.tbSize;
                /*4x4 DL MIMO CHG START*/
                /* SPR 11355 Fix Start */
                if(((ueDLContext_p->ueCategory == UE_CAT_5) || (ueDLContext_p->ueCategory == UE_CAT_8)))
                /* SPR 11355 Fix End */
                {
                    mcsThresholdLayer3 = getMCSFromCQI(
                            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer3[userLocationType]
                            );

                    mcsThresholdLayer4 = getMCSFromCQI(
                            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer4[userLocationType]
                            );


                    if((dlCQIInfo_p->mcsIndexCodeWordOne > mcsThresholdLayer4) &&
                        (RANK_INDICATOR_4 == dlRi))
                    {
                        newTXNumOfLayer = RANK_INDICATOR_4;
                    }
                    else if((dlCQIInfo_p->mcsIndexCodeWordOne > mcsThresholdLayer3) &&
                           (RANK_INDICATOR_3 == dlRi))
                    {
                        newTXNumOfLayer = RANK_INDICATOR_3;
                    }
                    else
                    {
                        newTXNumOfLayer = RANK_INDICATOR_2;
                    }

                    if( newTXNumOfLayer == RANK_INDICATOR_4 && 
                        harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                    {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = 
                                    determineTBSize(
                                          dlCQIInfo_p->mcsIndexCodeWordOne,
                                          harqProcess_p->assignedResourcesNB,
                                          TB_MAPPED_TO_TWO_LAYER);
                        harqProcess_p->numOfLayer = newTXNumOfLayer;
                    }
                    else if( newTXNumOfLayer == RANK_INDICATOR_3 && 
                             harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                    {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = 
                                    determineTBSize(
                                          dlCQIInfo_p->mcsIndexCodeWordOne,
                                          harqProcess_p->assignedResourcesNB,
                                          TB_MAPPED_TO_ONE_LAYER);
                        harqProcess_p->numOfLayer = newTXNumOfLayer;
                    }
                    else if( newTXNumOfLayer == RANK_INDICATOR_2 && 
                             harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                    {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = 
                                    determineTBSize(
                                         dlCQIInfo_p->mcsIndexCodeWordOne,
                                         harqProcess_p->assignedResourcesNB,
                                         TB_MAPPED_TO_ONE_LAYER);
                        harqProcess_p->numOfLayer = RANK_INDICATOR_3;
                    }
                    else if( newTXNumOfLayer == RANK_INDICATOR_4 && 
                             harqProcess_p->numOfLayer == RANK_INDICATOR_3)
                    {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = 
                                    determineTBSize(
                                         dlCQIInfo_p->mcsIndexCodeWordOne,
                                         harqProcess_p->assignedResourcesNB,
                                         TB_MAPPED_TO_TWO_LAYER);
                        harqProcess_p->numOfLayer = RANK_INDICATOR_4;
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = 
                                    determineTBSize(
                                         dlCQIInfo_p->mcsIndexCodeWordOne,
                                         harqProcess_p->assignedResourcesNB,
                                         TB_MAPPED_TO_ONE_LAYER);
                    }
                }/*4x4 DL MIMO CHG END*/
                else
                {
                    harqProcess_p->dlHARQTBOneInfo.tbSize = 
                                    determineTBSize(
                                         dlCQIInfo_p->mcsIndexCodeWordOne,
                                         harqProcess_p->assignedResourcesNB,
                                         TB_MAPPED_TO_ONE_LAYER);
                    harqProcess_p->numOfLayer = RANK_INDICATOR_2;
                }
             
                       
                harqProcess_p->dlHARQTBOneInfo.mcsIndex = 
                    dlCQIInfo_p->mcsIndexCodeWordOne;
                /* SPR 3679 QOS Changes Start */
                totalRbMIMO_g[internalCellIndex] += harqProcess_p->assignedResourcesNB;
                *scheduleFlag_p = DL_QOS_ALREADY_TRANSMITED;
                /* SPR 3679 QOS Changes End */
            }
            break;
        default:
            {
                LTE_MAC_UT_LOG(LOG_WARNING,DL_RR_STRATEGY,
                        "[DL-Strategy][%s]TBOneTBTwoReTxNewInfo"
                        " choice is INVALID", __func__);
            }
    }

    /*DCI Format Calculation*/
    dciFormatSelector (ueDLContext_p,
            dlStrategyTxNode_p,
            HARQ_NOT_FREE_TX_3_4_ONE_NACK_NON_ZERO_QUEUE_LOAD,internalCellIndex);

    /*Here The required RBs will be taken from the previous 
     *       assigned RBs can be fetched from Harq context */
    tempResourceAllocatorInput_p->requiredRB = 
        harqProcess_p->assignedResourcesNB ;
    /* Setting the transmission type as Adaptive ReTx :NEW_VERSION*/
    dlStrategyTxNode_p->transmissionType  = RE_TX;
    dlStrategyTxNode_p->sumLCPriority       = FALSE;
    dlStrategyTxNode_p->weightedQueueLoad = FALSE;
    dlStrategyTxNode_p->msgType = SPATIAL_MUL ;

    /* SPR 3679 QOS Changes Start */  

    /* Update the minTBSize and maxTBSize for QOS Scheduling */
    dlStrategyTxNode_p->minTBSize = 
        harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize +
        harqProcess_p->dlHARQTBTwoInfo.retransmissionTBSize;

    dlStrategyTxNode_p->maxTBSize = dlStrategyTxNode_p->minTBSize;

    /* SPR 7433 Changes Starts*/
    /* SPR 5313 Fix start */
    dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated = 
        resourceRegionAllocated;
    /* SPR 5313 Fix end */
    /* SPR 7433 Changes Ends */

    /*putEntry of DLStrategyTXNode in DL Strategy Schedule Info */
    /* SPR# 5189 Changes Starts  */
    /* SPR# 5189 Changes Ends  */

//SPR 8538 FIX
    dlStrategyTxNode_p->harqProcess_p = harqProcess_p; //v_g
    if ( DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p
                    [internalCellIndex]->initParams_p->dlSchdConfig.dlSchedulerStrategy)
    {
        dlUeScheduleInfo_p = &(dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].dlUeScheduleInfo
                [dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount]);

        dlUeScheduleInfo_p->ueDLContext_p = ueDLContext_p;
        /* SPR# 5189 Changes Starts  */
        dlStrategyTxNode_p->transmissionType  = NEW_TX;
        /* SPR# 5189 Changes Ends  */

        /* SPR# 5189_5320 Changes Starts  */
        dlStrategyTxNode_p->harqProcess_p = harqProcess_p;
        /* SPR# 5189_5320 Changes Ends  */
        dlUeScheduleInfo_p->dLStrategyTxNode_p = dlStrategyTxNode_p;
        dlUeScheduleInfo_p->harqProcess_p = harqProcess_p;
        dlUeScheduleInfo_p->isTaDrxScheduled = INVALID_TA_DRX_MSG;

        /* Increament the scheduled UE count */
        dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount++;

        *ueDlStrategyTxNode_p = dlStrategyTxNode_p;
        /* SPR# 5189 Changes Starts */
        /*Updating the remaining RB available*/
        *availableRB_p -=  tempResourceAllocatorInput_p->requiredRB;
        return MAC_SUCCESS;
        /* SPR# 5189 Changes Ends */

    }
    /* SPR 3679 QOS Changes End */ 
    /*putEntry of DLStrategyTXNode in PDCCH Queue */

    /* + coverity 32499 */
    if (MAC_FAILURE == putEntryInDLSchedulerReTxQueue(dlStrategyTxNode_p, internalCellIndex))
    {
        freeDLHarqProcess (ueDLContext_p, harqProcess_p, internalCellIndex);
        freeMemPool(dlStrategyTxNode_p);
        return MAC_FAILURE;
    }
    else
    {
        /*Updating the remaining RB available*/
        *availableRB_p -=  tempResourceAllocatorInput_p->requiredRB;
        return MAC_SUCCESS;
    }
    /* - coverity 32499 */
}

/*****************************************************************************
 * * Function Name  : processDLNackTwoTBWithZeroQueueLoad 
 * * Inputs         : allocatedRBFromRoundRobin- The Number of RBs Allocated 
 *                        Orginally 
 *                    availableRB_p -Pointer to the number of Avavailable RBs
 *                    ueDLContext_p - Pointer to UE Context
 *                    harqProcess_p - Pointer to UEs HARQ Context
 *                    tBOneTBTwoReTxNewInfo- information about the type of
 *                       transmission of two TBs i.e. either New Transmission or 
 *                       Re- transmissions        
 *                    resourceRegionAllocated - CCor CE region allocated
 *                    internalCellIndex - cell index used at MAC
 * * Outputs        : Process for UE's DL Nack for Two TBs with Zero Queue Load 
 * * Returns        : MAC_SUCCESS/MAC_FAILURE
 * * Description    : Its main function is for processing the UE for 
 *                    Re-Transmission for Two TBs when the UE  is having Zero 
 *                    Queue Load.One TB is used for the Retransmision and other
 *                    TB is made INVALID.The output is prepared  and sent to 
 *                    PDCCH Module
 *****************************************************************************/
MacRetType processDLNackTwoTBWithZeroQueueLoad(
        /* +- SPR 17777 */
        UInt32 *availableRB_p, 
        DLUEContext* ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo,
        /* SPR 5313 Fix start */
        UInt8 resourceRegionAllocated,
		/* + SPS_TDD_Changes*/
		UInt8 schUsed,
		/* - SPS_TDD_Changes*/
        /* SPR 5313 Fix end */
        InternalCellIndex internalCellIndex)
{

    ResourceAllocatorInput *tempResourceAllocatorInput_p = PNULL;
    DLStrategyTxNode * dlStrategyTxNode_p=PNULL;
    DLCQIInfo* dlCQIInfo_p = PNULL;
    /* SPR 18769 Fix Start */    
    DLStrategyTxNode** ueDlStrategyTxNode_p =PNULL;
    /* SPR 18769 Fix End */    

    /*Preparing the DL Strategy Output Node*/
    GET_MEM_FROM_POOL(DLStrategyTxNode,dlStrategyTxNode_p,
            sizeof(DLStrategyTxNode),PNULL);

    /* + coverity 24535 */
    if (PNULL == dlStrategyTxNode_p)
    {
        freeDLHarqProcess (ueDLContext_p, harqProcess_p, internalCellIndex);
        return MAC_FAILURE;
    }
    /* - coverity 24535 */

    tempResourceAllocatorInput_p = &(dlStrategyTxNode_p->resourceAllocInput);
    tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
    /* SPR 12239 Fix Start */
    tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;
    /* SPR 12239 Fix End */
    tempResourceAllocatorInput_p->strictAllocSubBand = FALSE;
    dlStrategyTxNode_p->isCQIRequestedULGrantMsg = FALSE;

    /* Updating the dlStrategyTxNode's Fields*/
    dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;
    dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId;

    if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
    {
        ueDLContext_p->scheduleFlag = DL_QOS_NO_NEW_TRANSMISSION;
        dlCQIInfo_p = &(ueDLContext_p->dlCQIInfo);
    /* SPR 18769 Fix Start */    
        ueDlStrategyTxNode_p = &(ueDLContext_p->dlStrategyTxNode_p);
    /* SPR 18769 Fix End */    
    }
    else
    {
        ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag = DL_QOS_NO_NEW_TRANSMISSION;
        dlCQIInfo_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
    /* SPR 18769 Fix Start */    
        ueDlStrategyTxNode_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);
    /* SPR 18769 Fix End */    
    }

    if (!(dlCQIInfo_p->aperiodicCQIRequested)&&
            ((harqProcess_p->dlHARQTBOneInfo.current_TX_NB >=
              MAX_RETX_COUNT_FOR_STRICT_SUBBAND) || 
             (harqProcess_p->dlHARQTBTwoInfo.current_TX_NB >=
              MAX_RETX_COUNT_FOR_STRICT_SUBBAND))&&
            (dlCQIInfo_p->cqiMode !=
             INVALID_APERIODIC_CQI_MODE) )
    {
        dlStrategyTxNode_p->isCQIRequestedULGrantMsg = TRUE;
        dlCQIInfo_p->aperiodicCQIRequested = TRUE;
    }

    if ( TB_ONE_RETX_TB_TWO_RETX == tBOneTBTwoReTxNewInfo ) 
    {
        /*DCI Format Calculation*/
        dciFormatSelector (ueDLContext_p,
                dlStrategyTxNode_p,
                HARQ_NOT_FREE_TX_3_4_TWO_NACK,
                internalCellIndex);

        tempResourceAllocatorInput_p->requiredRB = 
            (harqProcess_p)->assignedResourcesNB ;
        /* Setting the transmission type as Adaptive ReTx :NEW_VERSION*/
        dlStrategyTxNode_p->transmissionType = RE_TX;

        harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize =
            harqProcess_p->dlHARQTBOneInfo.tbSize;
        harqProcess_p->dlHARQTBTwoInfo.retransmissionTBSize =
            harqProcess_p->dlHARQTBTwoInfo.tbSize;
    }
    else 
    {
        switch (tBOneTBTwoReTxNewInfo){
            case TB_ONE_RETX_TB_TWO_INVALID:
                {
                    harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize =
                        harqProcess_p->dlHARQTBOneInfo.tbSize;

                }
                break;
            case TB_ONE_INVALID_TB_TWO_RETX:
                {
                    harqProcess_p->dlHARQTBTwoInfo.retransmissionTBSize =
                        harqProcess_p->dlHARQTBTwoInfo.tbSize;

                }
                break;
            default:
                {
                    freeMemPool(dlStrategyTxNode_p);

                    return MAC_FAILURE;
                }
        }

        /* SPR 4493 START */
        /*DCI Format Calculation*/
        /*dciFormatSelector (ueDLContext_p,
          dlStrategyTxNode_p,
          HARQ_NOT_FREE_TX_3_4_ONE_NACK_SECOND_INVALID_ZERO_QL);*/

        dlStrategyTxNode_p->primaryDCIFormat = harqProcess_p->dciFormat;
        /* SPR 6136 start */
        dlStrategyTxNode_p->secondaryDCIFormat = MAX_NUM_DCI_FORMAT;
        /* SPR 6136 end */
        /* SPR 4493 END */
        tempResourceAllocatorInput_p->requiredRB = 
            (harqProcess_p)->assignedResourcesNB ;
        /* Setting the transmission type as Adaptive ReTx :NEW_VERSION*/
        dlStrategyTxNode_p->transmissionType = RE_TX;
    }

    dlStrategyTxNode_p->sumLCPriority = FALSE;
    dlStrategyTxNode_p->weightedQueueLoad = FALSE;
    /*SPS_TDD_Changes*/
    if(SPS_SCHEDULING !=  schUsed)
    {
        dlStrategyTxNode_p->msgType = SPATIAL_MUL;
    }
    else
    {
        dlStrategyTxNode_p->msgType = SPS_RETX;
    }
    /*SPS_TDD_Changes*/

    /* SPR 6136 start */
    dlStrategyTxNode_p->harqProcess_p = harqProcess_p;
    /* SPR 6136 end */

    /* SPR 5313 Fix start */
    dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated = 
        resourceRegionAllocated;
    /* SPR 5313 Fix end */

    /* SPR 18769 Fix Start */
    *ueDlStrategyTxNode_p = dlStrategyTxNode_p;
    /* SPR 18769 Fix End */
    /*SPR 21948 Changes End */

    /*putEntry of DLStrategyTXNode in PDCCH Queue */
    /* + coverity 32500 */
    if (MAC_FAILURE == putEntryInDLSchedulerReTxQueue(dlStrategyTxNode_p,internalCellIndex))
    {
        freeDLHarqProcess (ueDLContext_p, harqProcess_p, internalCellIndex);
        freeMemPool(dlStrategyTxNode_p); 
        return MAC_FAILURE;
    }
    else
    {
        *availableRB_p -= tempResourceAllocatorInput_p->requiredRB;
        return MAC_SUCCESS;
    }
    /* - coverity 32500 */

}
/* SPR 3679 Changes End */
/*****************************************************************************
 * Function Name  : processDLNackForOneTBOnly 
 * Inputs         : ueDLContext_p - pointer to DL UE context
 *                  harqProcess_p - pointer to Harq process whose transmission
 *                                  needs to be done
 *                  resourceRegionAllocated - Used to determine region allocated
 *                  schUsed - Scheduler used
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : availableRB_p - Leftover RBs
 * Returns        : None 
 * Description    : This function processes the UE for Re-Transmission for
 *                  single TB. The output is prepared  and sent to PDCCH Module 
 *****************************************************************************/
/*SPR 2446 Fix Begins*/
/* + SPS_CHG */
void processDLNackForOneTBOnly( UInt32 *availableRB_p, 
        DLUEContext* ueDLContext_p, DLHARQProcess *harqProcess_p, 
        UInt8 resourceRegionAllocated, UInt8 schUsed,
        InternalCellIndex internalCellIndex)
/* - SPS_CHG */
/*SPR 2446 Fix Ends*/
{
    ResourceAllocatorInput *tempResourceAllocatorInput_p = PNULL;
    harqProcess_p->isTBOneValid = TRUE;
    harqProcess_p->isTBTwoValid = FALSE;
    DLStrategyTxNode *dlStrategyTxNode_p = PNULL;

    /*Preparing the DL Strategy Output Node */
    GET_MEM_FROM_POOL (DLStrategyTxNode, dlStrategyTxNode_p,
            sizeof (DLStrategyTxNode), PNULL);

    /* + coverity 24533 */
    if (PNULL == dlStrategyTxNode_p)
    {
        freeDLHarqProcess (ueDLContext_p, harqProcess_p, internalCellIndex);
        return;
    }
    /* - coverity 24533 */

    tempResourceAllocatorInput_p = &(dlStrategyTxNode_p->resourceAllocInput);
    tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
    /* SPR 12239 Fix start */
    tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;
    /* SPR 12239 Fix end */
    tempResourceAllocatorInput_p->strictAllocSubBand = FALSE;
    dlStrategyTxNode_p->isCQIRequestedULGrantMsg = FALSE;
    dlStrategyTxNode_p->strictAllocatedFlag = FALSE;                
//SPR 8538 FIX 
    dlStrategyTxNode_p->harqProcess_p = harqProcess_p;                 

    /* Updating the dlStrategyTxNode's Fields */
    dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;
    dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId;
    /* SPS_CHG */
    if(SPS_SCHEDULING !=  schUsed)
    {
        dlStrategyTxNode_p->msgType = NON_SPATIAL_MUL;
    }
    else
    {
        dlStrategyTxNode_p->msgType = SPS_RETX;
    }
    /* SPS_CHG */
    dlStrategyTxNode_p->secondaryDCIFormat = MAX_NUM_DCI_FORMAT;
    dlStrategyTxNode_p->primaryDCIFormat = harqProcess_p->dciFormat;

    if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
    {
        ueDLContext_p->scheduleFlag = DL_QOS_NO_NEW_TRANSMISSION;
    }
    else
    {
        ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag = DL_QOS_NO_NEW_TRANSMISSION;
    }

    /*Here The required RBs will be taken from the previous 
      assigned RBs can be fetched from Harq context */
    tempResourceAllocatorInput_p->requiredRB =
        (harqProcess_p)->assignedResourcesNB;

    harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize =
        harqProcess_p->dlHARQTBOneInfo.tbSize;

    /* Setting the transmission type as Adaptive ReTx :NEW_VERSION */
    dlStrategyTxNode_p->transmissionType = RE_TX;

    dlStrategyTxNode_p->sumLCPriority = FALSE;
    dlStrategyTxNode_p->weightedQueueLoad = FALSE;
    
/*Qos changes*/  
    /* Update the minTBSize and maxTBSize for QOS Scheduling */
    dlStrategyTxNode_p->minTBSize = 
        harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize;

    dlStrategyTxNode_p->maxTBSize = dlStrategyTxNode_p->minTBSize;

/*Qos changes*/  

    LOG_MAC_MSG (DRX_RETX_UE_SCHEDULED_LOG_ID, LOGINFO, MAC_DL_Strategy,
            getCurrentTick (),
            dlStrategyTxNode_p->ueIndex, dlStrategyTxNode_p->harqProcessId,
            dlStrategyTxNode_p->transmissionType, DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
            __func__, " ");
    /* ICIC changes start */
    /*+COVERITY 5.3.0 - 54177*/
    dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated = (ResourceRegion)resourceRegionAllocated;
    /*-COVERITY 5.3.0 - 54177*/
    /* ICIC changes end */

    /*putEntry of DLStrategyTXNode in PDCCH Queue */
    if (!(putEntryInDLSchedulerReTxQueue(dlStrategyTxNode_p,internalCellIndex)))
    {
        /* + Coverity 32498 */
        LTE_MAC_UT_LOG(LOG_WARNING,DL_STRATEGY,
                "[DL-Strategy][%s]dlStrategyTxNode_p Entry is failed\n",
                __func__);
        /*SPR 17049 Fix Start*/
        freeDLHarqProcess (ueDLContext_p, harqProcess_p, internalCellIndex);
        /*SPR 17049 Fix End*/
        freeMemPool(dlStrategyTxNode_p);
        /* - Coverity 32498 */
    }
    else
    {
        /* CA Stats Changes Start */
        /*Qos changes*/
        /* debug Stats */
        LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED
        (ueDLContext_p, harqProcess_p, dlStrategyTxNode_p, internalCellIndex);
/*Qos changes*/
        /* CA Stats Changes End */
        /*SPR 17049 Fix Start*/
        /*Updating the remaining RB available */
        *availableRB_p -= tempResourceAllocatorInput_p->requiredRB;
        /*SPR 17049 Fix End*/

    }
    
    /*SPR 17049 Fix Start*/
    /*Code Deleted*/
    /*SPR 17049 Fix End*/
    
}
/* RETX_CHG */

/*****************************************************************************
 * Function Name : scheduleUEsByDLStrategy
 * Inputs        : numberOfAvailableRBs_p :pointer to current available RBs.
                    dlScheduleData_p : Pointer to DL Scheduler Data Element
                    currentGlobalTTITickCount:Current Global Tick
                    maxUEsToBeScheduledDL: Max UE that can be scheduled in DL
 *                  internalCellIndex - cell index used at MAC
 * Outputs       : It process the UE present in the Non Zero List by following 
                   the configured algorithm
 * Returns       : MAC_SUCCESS
 *                 scheduler strategy populated the output
 * Description   : Its main function is to distributes RBs among the UEs 
                   present in the UENonZeroQueueLoadAndHArqList after applying
                   the configured scheduling algorithm either Proportional Fair
                   or Round Robin algorithm .
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129193) */            
UInt32 scheduleUEsByDLStrategy(
/* SPR 5599 changes end (ZIP ID 129193) */            
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement *dlScheduleData_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt8  maxUEsToBeScheduledDL, /* RETX_CHG */
        InternalCellIndex internalCellIndex)
{

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Entry:[DL-Strategy][%s]Number"
            " Of Available RBs %d", __func__, *numberOfAvailableRBs_p );

    /*When the Algorithm to be followed for RB distribution is DL_FAIR_SCHEDULING*/
    if  (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
            initParams_p->dlSchdConfig.dlSchedulerStrategy == DL_FAIR_SCHEDULING)
    {
        dlProportionalFairScheduler(
            dlReferenceFlag_g[internalCellIndex],
            numberOfAvailableRBs_p, dlScheduleData_p,
            currentGlobalTTITickCount, 
            /* SPR 17777 */
            maxUEsToBeScheduledDL, /* RETX_CHG */
            internalCellIndex);
        
    }
    else /* ROUND_ROBIN_SCHEDULING */
    {
        dlRoundRobinScheduler(
            dlReferenceFlag_g[internalCellIndex],
            numberOfAvailableRBs_p, dlScheduleData_p,
            /* +- SPR 17777 */
            currentGlobalTTITickCount,
            maxUEsToBeScheduledDL, /* RETX_CHG */
            internalCellIndex);

    }

    /*LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Exit:[DL-Strategy][%s]",
      __func__);*/
    return MAC_SUCCESS;
}
/* ICIC change start */
/*****************************************************************************
 * Function Name  : processDLAdvanceQueue
 * Inputs         : numberOfAvailableRBs_p    : Pointer to current available RBs.
 *                  globalTick - Current Global Tick
 *                  dlScheduleData_p          : Pointer to DL Scheduler Data Element,
 *                  dlDelay - Number of ticks DL is getting processed in Advance,
 *                  maxUEsToBeScheduledDL_p - max UE's can scheduled in DL
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : Its main function will be to process the DLAdvance Queues Nodes
 *                  Allocate RBs to the UEs otherwise add then in scheduler Array.
 *****************************************************************************/
void processDLAdvanceQueue( 
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement * dlScheduleData_p,
        /*SPR 15909 fix start*/
        tickType_t  globalTick,
        /*SPR 15909 fix end*/
#ifdef FDD_CONFIG
        UInt32 dlDelay,
#endif
        UInt8 *maxUEsToBeScheduledDL_p,
        InternalCellIndex internalCellIndex)

{
    UInt8 firstAdvanceNode = TRUE;
/* #ifdef TDD_CONFIG
    UInt8 currentSubframeType = 0;
#endif TDD warning Fix */
    UInt16 ueIndex                      = 0;
    DLAdvanceQueueNode *dlAdvNode_p   = PNULL;
    DLStrategyTxNode *dlStrategyTxNode_p = PNULL;
    DLAdvanceQueueNode *dlAdvNodeTmp_p    = PNULL;
    DLUEContext *dlUEContext_p          = PNULL;
    DLUEContextInfo *dlUEContextInfo_p  = PNULL;
    DLHARQProcess *dlHarqProcess_p      = PNULL;
    MeasGapContextDL *measgapContext_p  = PNULL;
    UInt8 maxUEsToBeScheduledDL         = 0;
    UInt8 userLocationType              = CC_USER;
    DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p =
        dlScheduleData_p->dlSchedulerDataNodeArr;
    UInt32 rbsAllocated                 = 0;
    DRXContext           *drxContext_p  = PNULL;

    maxUEsToBeScheduledDL = *maxUEsToBeScheduledDL_p;
/* SPR 10801 [BUG 739] FIX START */
#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe = 0;
    UInt8 subFrame = globalTick % MAX_SUBFRAME;
    isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
#endif
/* SPR 10801 [BUG 739] FIX END */

#ifdef FDD_CONFIG
    globalTick += dlDelay;
#endif
/* #ifdef TDD_CONFIG
    currentSubframeType = getSubframeType((globalTick % 10));
#endif TDD warning Fix */
    dlAdvNode_p = (DLAdvanceQueueNode *)getFirstNode(advanceAllocInfoQueue_gp[internalCellIndex]);

    while (dlAdvNode_p && numberOfAvailableRBs_p[CC_CE_REGION] && 
            maxUEsToBeScheduledDL)
    {
        if (dlAdvNode_p->isNodeInvalid)
        {
            if(firstAdvanceNode)
            {
                dlAdvNode_p = (DLAdvanceQueueNode *) popNode (advanceAllocInfoQueue_gp[internalCellIndex]);
                /* +COVERITY 11433 */
                if (NULL == dlAdvNode_p)
                {
                    break;
                }
                /* -COVERITY 11433 */

                dlAdvNodeTmp_p = dlAdvNode_p;
                dlAdvNode_p = (DLAdvanceQueueNode *)
                    getNextNode ((const LTE_SNODE *) (dlAdvNodeTmp_p));

                freeMemPool (dlAdvNodeTmp_p);
                continue;
            }
            else
            {
                dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
                    ((const LTE_SNODE *) (dlAdvNode_p));
                continue;
            }
        } 
        dlStrategyTxNode_p = dlAdvNode_p->dlStrategyTxNode;
        ueIndex = dlStrategyTxNode_p->ueIndex;
        dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        rbsAllocated = 0;

        dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
        /* + SPR#6952 */
        if(PNULL != dlUEContext_p)
        {
            dlHarqProcess_p = &(dlUEContext_p->dlHarqContext_p->
                      harqProcess[dlStrategyTxNode_p->harqProcessId]);
        }
        /* + SPR#6952 */

        if (!dlUEContextInfo_p->pendingDeleteFlag)
	{
            /* + Coverity_31440 */   
	    if (PNULL != dlUEContext_p)
	    {
            /* - Coverity_31440 */   
			/* EICIC +*/
/*Complexity Start*/
			if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == dlUEContext_p->userType)) 
                    || ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == dlUEContext_p->userType)))
			{              
				firstAdvanceNode = FALSE;
				dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode((const LTE_SNODE *) (dlAdvNode_p));  
				continue;

			}

/*Complexity End*/
			/* EICIC -*/
		/* + SPR_6061_6062 */
		userLocationType = dlUEContext_p->userLocationType;\
	        /* - SPR_6061_6062 */
	        measgapContext_p = &(dlUEContext_p->measGapContext);
		drxContext_p     = &(dlUEContext_p->drxContext);
		/* \code
		 * if DRX State is ON or Meas Gap is ON and 
		 * schType == SPS_SCHEDULING, then do not consider
		 * this UE for scheduling.
		 * */

		/* + SPR_6061_6062 */

		/*  Assuming only NEW tx will come in this queue */
                 /* Cyclomatic_complexity_changes_start */
                if(validateUeForScheduling(drxContext_p, dlUEContext_p, measgapContext_p))
                 /* Cyclomatic_complexity_changes_end */
		{
		    firstAdvanceNode = FALSE;
		    dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
			((const LTE_SNODE *) (dlAdvNode_p));

		    continue;

		}

		/* - SPR_6061_6062 */

		if (dlUEContext_p->isAlreadyConsideredFlag == dlReferenceFlag_g[internalCellIndex] )
		{
		    /* Do not consider the UE for scheduling */
		    firstAdvanceNode = FALSE;
		    dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
			((const LTE_SNODE *) (dlAdvNode_p));
		    continue;
		}
		/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
		if((TRUE == dlUEContext_p->isHDFddFlag) && 
			(!(CAN_SCHEDULE_DL_FOR_HD_UE(globalTick, ueIndex, hdUeContextInfo_g.hdfddUeSchedMap))))
		{
		    /* + SPR_6061_6062 */
		    firstAdvanceNode = FALSE;
		    dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
			((const LTE_SNODE *) (dlAdvNode_p));
		    /* - SPR_6061_6062 */
		    UPDATE_UE_DENIED_DL_RESOURCE(ueIndex);
		    continue;
		}
#endif
		/*HD FDD Changes End*/

		if (( CE_USER == userLocationType ) 
			&& (( maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] == 0 )
			    || !numberOfAvailableRBs_p[CE_REGION]))
		{

		    /* + SPR_6061_6062 */
		    /* Do not consider the UE for scheduling */
		    firstAdvanceNode = FALSE;
		    dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
			((const LTE_SNODE *) (dlAdvNode_p));
		    continue;
		    /* - SPR_6061_6062 */
		}

/* Spr 23393 Changes Start */
#ifdef TDD_CONFIG
        if((DL == dlStrategyTxNode_p->harqProcess_p->subFrameType && isSpecialSubframe) || 
			(SP == dlStrategyTxNode_p->harqProcess_p->subFrameType && !isSpecialSubframe))
		{
		    firstAdvanceNode = FALSE;
		    dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
			((const LTE_SNODE *) (dlAdvNode_p));
		    continue;
		}
#endif
/* Spr 23393 Changes End */
		if (dlStrategyTxNode_p->resourceAllocInput.requiredRB <= 
			numberOfAvailableRBs_p[userLocationType])
		{
            /* Spr 23393 Changes Start */
           /*  code removed */ 
            /* Spr 23393 Changes End */
		    rbsAllocated = dlStrategyTxNode_p->resourceAllocInput.requiredRB;
		}
		else
		{
		    if ((tempDLSchedulerDataNodeArr_p[ueIndex].msgType == TA_DRX_MSG) &&
			    (numberOfAvailableRBs_p[userLocationType] >= MIN_RB_FOR_BOTH_TA_DRX_ONLY)) 

		    {
			rbsAllocated = MIN_RB_FOR_BOTH_TA_DRX_ONLY;
		    }
		    else if (((tempDLSchedulerDataNodeArr_p[ueIndex].msgType == TA_MSG) ||
				(tempDLSchedulerDataNodeArr_p[ueIndex].msgType == DRX_MSG))&&
			    (numberOfAvailableRBs_p[userLocationType] >= MIN_RB_FOR_TA_DRX_ONLY))
		    {
			rbsAllocated = MIN_RB_FOR_TA_DRX_ONLY;
		    }
		    else
		    {

			/* + SPR_6061_6062 */
			firstAdvanceNode = FALSE;
			/* - SPR_6061_6062 */
			dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
			    ((const LTE_SNODE *) (dlAdvNode_p));
			continue;
		    }

		}
		numberOfAvailableRBs_p[userLocationType] -= rbsAllocated;

		/* + SPR_6061_6062 */
		dlStrategyTxNode_p->resourceAllocInput.requiredRB = rbsAllocated;
		/* - SPR_6061_6062 */
		if ( userLocationType == CE_USER)
		{
		    maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;                   
		    numberOfAvailableRBs_p[CC_CE_REGION] -=
			dlStrategyTxNode_p->resourceAllocInput.requiredRB;
		}
		else if (numberOfAvailableRBs_p[CC_CE_REGION] <  numberOfAvailableRBs_p[CE_REGION])
		{
		    numberOfAvailableRBs_p[CE_REGION] = numberOfAvailableRBs_p[CC_CE_REGION];
		}
		dlUEContext_p->isAlreadyConsideredFlag = dlReferenceFlag_g[internalCellIndex];
		maxUEsToBeScheduledDL--;

		/* ICIC changes start */
		/* Update Cell Edge UE global count*/
		alreadyScheduledUEs[internalCellIndex]++;

		/* ICIC changes end */

		if (!(putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex)))
		{
		    freeDLHarqProcess (dlUEContext_p, dlHarqProcess_p, internalCellIndex);
		    /*SPR 21036 Fix Start*/
		    /*freeMemPool(dlStrategyTxNode_p);*/
		    /*SPR 21036 Fix End*/
		    dlUEContext_p->dlStrategyTxNode_p = PNULL;
		}


		if (firstAdvanceNode)
		{
		    dlAdvNode_p = (DLAdvanceQueueNode *) popNode
			(advanceAllocInfoQueue_gp[internalCellIndex]);
		    /* +COVERITY 11433 */
		    if (NULL == dlAdvNode_p)
		    {
			break;
		    }
		    /* -COVERITY 11433 */

		    dlAdvNodeTmp_p = dlAdvNode_p;

		    dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
			((const LTE_SNODE *) (dlAdvNodeTmp_p));

		    freeMemPool (dlAdvNodeTmp_p);

		}
		else
		{
		    dlAdvNode_p->isNodeInvalid = TRUE;
		    dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
			((const LTE_SNODE *) (dlAdvNode_p));
		}

            /* + Coverity_31440 */   
	    }
           /* - Coverity_31440 */   

	}

        else
        {
            if (firstAdvanceNode)
            {
                dlAdvNode_p = (DLAdvanceQueueNode *) popNode
                    (advanceAllocInfoQueue_gp[internalCellIndex]);
                /* +COVERITY 11433 */
                if (NULL == dlAdvNode_p)
                {
                    break;
                }
                /* -COVERITY 11433 */

                dlAdvNodeTmp_p = dlAdvNode_p;

                dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
                    ((const LTE_SNODE *) (dlAdvNodeTmp_p));
		
	        /* + SPR_6061_6062 */
		if (dlUEContext_p != PNULL)
		{
	            freeDLHarqProcess (dlUEContext_p, dlHarqProcess_p, internalCellIndex);
		}

	        /* - SPR_6061_6062 */
                freeMemPool(dlStrategyTxNode_p);

                freeMemPool (dlAdvNodeTmp_p);
            }
            else
            {
                dlAdvNode_p->isNodeInvalid = TRUE;

	        /* + SPR_6061_6062 */
		if (dlUEContext_p != PNULL)
		{
                    freeDLHarqProcess (dlUEContext_p, dlHarqProcess_p, internalCellIndex);
		}
	        /* - SPR_6061_6062 */
                freeMemPool(dlStrategyTxNode_p);

                dlAdvNode_p = (DLAdvanceQueueNode *) getNextNode
                    ((const LTE_SNODE *) (dlAdvNode_p));

            }

        }

    }

    *maxUEsToBeScheduledDL_p = maxUEsToBeScheduledDL;
    return;
}
/*****************************************************************************
 * Function Name  : processTAQueue
 * Inputs         : timeAdvanceQueueCount - This is Count of the total nodes 
 *                     present in the dlTimeAdvanceQueue_g or pendingTAQueue
 *                  numberOfAvailableRBs_p:pointer to current available RBs.
 *                  currentGlobalTTITickCount - Current Global Tick
 *                  dlScheduleData_p - Pointer to DL Scheduler Data Element
 *                  tempTimeAdvanceQueue_p - Pointer to Time Advance Queue 
 *                     Either New or Pending TA Queues.
 *                  flagForTADRXPendingFresh-This Flag helps in identifying 
 *                     whether the Queue is Pending or Fresh.
 *                  numTAUEScheduled_p - Pointer to number of TA UE to be schedule.
 *                  maxUEsToBeScheduledDL_p - max UE's can scheduled in DL
 *                  dlDelay - Number of ticks DL is getting processed in Advance
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : It will allocate RBs to PendingTA/FreshTAQueue Nodes &
 *                  place the DL Strategy O/P either in the PDCCHNewTXQueue_g
 *                  or PDCHHReTXQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the TA Queues Nodes
 *                  fresh and Pending both . Allocate RBs to the UEs with non 
 *                  Zero Queue Load otherwise add then in scheduler Array. 
 *****************************************************************************/
void processTAQueue(
        UInt32 timeAdvanceQueueCount, 
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        DLSchedulerDataElement * dlScheduleData_p,
        TA_Q_TYPE * tempTimeAdvanceQueue_p,
        FlagForPendingTADRX flagForTADRXPendingFresh,
        UInt16 *numTAUEScheduled_p,
        UInt8 *maxUEsToBeScheduledDL_p
        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
        ,UInt8 dlDelay
#endif
        /*HD FDD Changes End*/
        , InternalCellIndex internalCellIndex
        ) /* SPR_1346 */ /*RETX_CHG*/ 
{
    DLUEContext *ueDLContext_p          = PNULL;
    DLHARQEntity *ueharqDLContext_p     = PNULL ;
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    DLHARQProcess *harqProcess_p = PNULL;          
    UInt32 ueIndex; 
    TimeAdvanceQueueNode *taNode_p      = PNULL;
    DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p =
        dlScheduleData_p->dlSchedulerDataNodeArr;
    UInt32 totalUE = dlScheduleData_p->dataNodeArrTotalUE;
    /* SPR_1346_START */
    UInt8 maxUEsToBeScheduledDL        = *maxUEsToBeScheduledDL_p;
    /* SPR_1346_END */

    DRXContext          *drxContext_p = PNULL; /* DRX_CHG */
    MeasGapContextDL      *measGapContext_p = PNULL; /* MEAS_GAP_CHG */    
    /* ICIC changes start */
    UInt32 tempRBCount                  = 0;
    UInt8 userLocationType              = CC_USER;
    MacRetType retTypeTADRXProcess      = MAC_SUCCESS;
    /* ICIC changes end */
    /*Soft Buffer Changes Start*/
    UInt32 minTBSizeToSendTADRX = 0;
    /*Soft Buffer Changes End*/
       /* EICIC+ */
    	UInt8	isTraverseList = LTE_FALSE;    
    TimeAdvanceQueueNode     *taNodeScan_p = PNULL;
	/* EICIC -*/ 
#ifdef TDD_CONFIG
    UInt8 subFrame = currentGlobalTTITickCount % MAX_SUBFRAME;
#endif
    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Entry:[DL-Strategy][%s]:"
            "Available RBs:Total %d, Cell Edge %d,TA Queue Count:%d",__func__,
            numberOfAvailableRBs_p[CC_CE_REGION],numberOfAvailableRBs_p[CE_REGION],
            timeAdvanceQueueCount
            );


    while ((timeAdvanceQueueCount)&&(numberOfAvailableRBs_p[CC_CE_REGION])
            && (maxUEsToBeScheduledDL))
    /* ICIC changes end */
    {
        DEQUEUE_TA_Q(tempTimeAdvanceQueue_p,TimeAdvanceQueueNode, (void **)&taNode_p);
        if (taNode_p)
        {
            ueIndex = taNode_p->ueIdx;    
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            /*SPR 15208 Fix*/
            /*SPR 20213 Fix + */
            if ((PNULL == ueDLContextInfo_p) ||  (ueDLContextInfo_p->dlUEContext_p == PNULL))
	    {
                FREE_MEM_NODE_TA_Q(taNode_p);
                continue ;
	    }
	    if (!(ueDLContextInfo_p->pendingDeleteFlag) && 
			    (PNULL != ueDLContextInfo_p->dlUEContext_p))
	    {
                ueDLContextInfo_p->dlUEContext_p->TA_ScheduleFlag = LTE_FALSE;
	    }
            /*SPR 20213 Fix - */
            /*SPR 15208 Fix*/
            if ((!(ueDLContextInfo_p->pendingDeleteFlag)) &&
                    (UL_SYNC_LOSS_OFF ==
                     checkUlSyncLossForUE(ueDLContextInfo_p->dlUEContext_p, 
                        currentGlobalTTITickCount, internalCellIndex)))
            {
                /* Check for pending TA Ack/Nack removed along with SPR_1346 */
                ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
				/* EICIC +*/
/*Complexity Start*/
				if( ((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))
					||((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{              
					if(!ENQUEUE_TA_Q(tempTimeAdvanceQueue_p,TimeAdvanceQueueNode,&(taNode_p->timeAdvanceQueueNodeAnchor)))
					{
						FREE_MEM_NODE_TA_Q(taNode_p);
					}
					isTraverseList = LTE_TRUE;
					timeAdvanceQueueCount--;
					continue;
				}
/*Complexity End*/
				/* EICIC -*/
                    userLocationType = ueDLContext_p->userLocationType;
                    /* Checking if Already being considered for RB Distribution*/
                    if (ueDLContext_p->isAlreadyConsideredFlag != 
                            dlReferenceFlag_g[internalCellIndex] )
                    {
                        /* DRX_CHG and MEAS_GAP_CHG */
                        drxContext_p = &ueDLContext_p->drxContext;
                        measGapContext_p = &ueDLContext_p->measGapContext;
                        
                        if ( (DRX_STATE_ON == CHECK_UE_DRX_STATE(drxContext_p)) ||
                             (TRUE == CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p))
                             /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                             /* If HD FDD UE is in TA advance Q,then check Schedule map whether
                              * it can be scheduled in currenttick or not*/
                             ||
                            ((TRUE == ueDLContext_p->isHDFddFlag) &&
                             (!(CAN_SCHEDULE_DL_FOR_HD_UE(currentGlobalTTITickCount+dlDelay, ueIndex, hdUeContextInfo_g.hdfddUeSchedMap))))
#endif
                             /*HD FDD Changes End*/
                            )
                        {
                            /* SPR 2377 changes start */
                            LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_DL_LOG_ID, LOGDEBUG, 
                                    MAC_DL_Strategy, currentGlobalTTITickCount, 
                                    ueIndex, CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                    __func__,"");

                            /* SPR 2377 changes end */
                            /*SPR 6333 Fix Start*/
                            if(flagForTADRXPendingFresh == PENDING_TA_FLAG)
			                {
                                FREE_MEM_NODE_TA_Q(taNode_p);
                                /*SPR 7982 Fix Start*/
                                ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                /*SPR 7982 Fix End*/
			                }
			                else
 			                {        
                                UInt32 retVal = 0;
                                retVal = ENQUEUE_TA_Q(pendingTAQueue_gp[internalCellIndex],TimeAdvanceQueueNode,&(taNode_p->timeAdvanceQueueNodeAnchor));
                                if (!retVal)
                                {
                                    lteWarning("Error in Pushing in Pending"
                                            " TAQueue in funtion %s\n",
                                            __func__);
                                    FREE_MEM_NODE_TA_Q(taNode_p);
                                    /*SPR 7982 Fix Start*/
                                    ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                    /*SPR 7982 Fix End*/
                                }
                                /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                                if(TRUE == ueDLContext_p->isHDFddFlag)
                                {
                                    /* Increment the statistics counter UE denied DL resources due to
                                     * UL clash in the same TTI of HD FDD UE*/
                                    UPDATE_UE_DENIED_DL_RESOURCE(ueIndex);
                                }
#endif
                                /*HD FDD Changes End*/
                            }
                            timeAdvanceQueueCount--;
                            continue;
                            /*SPR 6333 Fix End*/
                        }
                        /* DRX_CHG and MEAS_GAP_CHG */

                        /*! \code
                         * If this UE is for SPS in this tick and
                         *   spsOpp - (headPktSize + Header size ) >= 
                         *   LENGTH_OF_TA 
                         *   - Then send TA for this UE.
                         * Else skip this UE for next tick.
                         * \endcode
                         * */
                        if( SPS_SCHEDULING == ueDLContext_p->schType )
                        {
                            /*SPR 6333 Fix Start*/
                            if(flagForTADRXPendingFresh == PENDING_TA_FLAG)
                            {
                                FREE_MEM_NODE_TA_Q(taNode_p);
                            }
                            else
                            {
                                if( !ENQUEUE_TA_Q(pendingTAQueue_gp[internalCellIndex], TimeAdvanceQueueNode,
                                         &(taNode_p->timeAdvanceQueueNodeAnchor)))
                                {
                                    FREE_MEM_NODE_TA_Q(taNode_p);
                                }
                            }
                            timeAdvanceQueueCount--;
                            continue;
                            /*SPR 6333 Fix End*/
                        }

                        ueharqDLContext_p = ueDLContext_p->dlHarqContext_p;           

                        if (ueDLContext_p->isAlreadyConsideredFlag != dlReferenceFlag_g[internalCellIndex] )
                        {

                            /* ICIC changes start */
                            if ((numberOfAvailableRBs_p[userLocationType] <= ZERO)
                                || ((!maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]) && CE_USER == userLocationType))
                            {
                                LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_DL_LOG_ID, LOGDEBUG, 
                                        MAC_DL_Strategy, currentGlobalTTITickCount, 
                                        ueIndex, CHECK_UE_DRX_STATE(drxContext_p),
                                        CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                        __func__,"");

                                /* SPR 2377 changes end */
                                /*SPR 6333 Fix Start*/
								if(flagForTADRXPendingFresh == PENDING_TA_FLAG)
								{
                                    FREE_MEM_NODE_TA_Q(taNode_p);
                                    /*SPR 7982 Fix Start*/
                                    ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                    /*SPR 7982 Fix End*/
								}
								else
								{   
                                    UInt32 retVal = 0;                                    
                                    retVal = ENQUEUE_TA_Q( pendingTAQueue_gp[internalCellIndex],
                                                 TimeAdvanceQueueNode,&(taNode_p->timeAdvanceQueueNodeAnchor));
                                    if(!retVal)
                                    {
                                        lteWarning("Error in Pushing in Pending"
                                                " TAQueue in funtion %s\n",
                                                __func__);
                                        FREE_MEM_NODE_TA_Q(taNode_p);
                                            /*SPR 7982 Fix Start*/
                                            ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                            /*SPR 7982 Fix End*/
                                    }
								}
                                /*SPR 6333 Fix End*/
                                timeAdvanceQueueCount--;
                                continue;
                            }
                            /* ICIC changes end */
                        /*Checking if UE Queue Load is Zero*/
                            /*Soft Buffer Changes Start*/
                        if ( (NET_UE_QL(ueDLContext_p) > ZERO) &&
                                ueDLContext_p->permissibleTBSize >= HARQ_TB_MINIMUM_SIZE_IN_BYTES)
                            /*Soft Buffer Changes End*/
                        {
                            /*Finding the free Harq process for New Transmission*/
                            if ( PNULL == 
                                    (harqProcess_p = checkForFreeHarqProcess
                                     ( ueDLContext_p,
                                       ueDLContext_p->dlHarqContext_p,
                                       DRA_SCHEDULING,
				       internalCellIndex)))
                            {
                                LOG_MAC_MSG (DL_TA_SCHEDULED_FAILURE_LOG_ID, LOGWARNING,
                                MAC_DL_Strategy, getCurrentTick (), ueDLContext_p->ueIndex,
                                0,
                                0,
                                0,
                                0,
                                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");

                                LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                                        "[DL-Strategy][%s]No free Harq process "
                                        "Found for UE ID: %d\n",
                                        __func__,ueDLContext_p->ueIndex);
                                LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                                        "[DL-Strategy][%s]:Pending TA in UE Context,"
                                        "Pushing TA Request Node in Pending TA Queue"
                                        ,__func__);
                                /*SPR 6333 Fix Start*/
								if(flagForTADRXPendingFresh == PENDING_TA_FLAG)
								{
                                    FREE_MEM_NODE_TA_Q(taNode_p);
                                    /*SPR 7982 Fix Start*/
                                    ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                    /*SPR 7982 Fix End*/
								}
                                else
                                {        
                                    UInt32 retVal = 0;
                                    retVal = ENQUEUE_TA_Q( pendingTAQueue_gp[internalCellIndex],TimeAdvanceQueueNode,
                                            &(taNode_p->timeAdvanceQueueNodeAnchor));
                                    if(!retVal)
                                    {
                                        lteWarning("Error in Pushing in Pending"
                                                " TAQueue in funtion %s\n",
                                                __func__);
                                        FREE_MEM_NODE_TA_Q(taNode_p);
                                        /*SPR 7982 Fix Start*/
                                        ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                        /*SPR 7982 Fix End*/
                                    }

                                }
                                timeAdvanceQueueCount--;
                                continue;
                                /*SPR 6333 Fix End*/
                            }

                            LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                                    "[DL-Strategy][%s]:TA Request of UE ID"
                                    " %d is received Since Queue Load is Non"
                                    "-Zero,Updating TA Request in"
                                    " DLSchedulerDataNodeArr_p UE Will be "
                                    "scheduled during Main Scheduling Algorithm"
                                    ,__func__, ueDLContext_p->ueIndex);

                            tempDLSchedulerDataNodeArr_p[totalUE].harqProcessId = 
                                harqProcess_p->harqProcessId;
                            /*process TA when UE has Non Zero QueueLoad
                              Preparing the scheduler data*/
                            tempDLSchedulerDataNodeArr_p[totalUE].ueDLContext_p=
                                ueDLContext_p;
                            /* + SPR 22243 Fix*/
                            /* DRX should be scheduled only if qload and bsr is 0 */ 
                            if (((ueDLContext_p->drxFlag) 
                                &&(!(ueDLContext_p->pendingDRXAckNack))) &&
                                  ((ueDLContext_p->ueQueueLoad == 0)&& 
                                   (ulUECtxInfoArr_g[ueIndex].ulUEContext_p->bsrNetTotal == 0)))
                            {
                                /* - SPR 22243 Fix*/
                                tempDLSchedulerDataNodeArr_p[totalUE].msgType
                                    = TA_DRX_MSG;
                            }
                            else
                            {
                                tempDLSchedulerDataNodeArr_p[totalUE].msgType
                                    = TA_MSG;
                            }
                            /* + COVERITY 5.0 */
                            /* + SPR 6970 Start */
                            LOG_MAC_MSG (DL_TA_QUEUE_SCHEDULED_LOG_ID, LOGDEBUG,
                                    MAC_DL_Strategy, getCurrentTick (), ueIndex,
                                    harqProcess_p->harqProcessId,
                                    harqProcess_p->assignedResourcesNB,
                                    ueDLContext_p->ueQueueLoad,
                                    tempDLSchedulerDataNodeArr_p[totalUE].msgType,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
                            /* - SPR 6970 Start */

                            /* - COVERITY 5.0 */

                            totalUE++;
                            /* TA_FIX */
                            ueDLContext_p->isAlreadyConsideredFlag 
                                    = dlReferenceFlag_g[internalCellIndex];
                            /* TA_FIX */
                            /* SPR_1346_START */
                            (*numTAUEScheduled_p)++;
                            alreadyScheduledUEs[internalCellIndex]++;
                            maxUEsToBeScheduledDL--;
                                /* SPR_1346_END */
                            if (userLocationType == CE_USER)
                            {
                                maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;
                            }
                        }
                        else
                        {
                            /*Soft Buffer Changes Start*/
                            if ((ueDLContext_p->drxFlag)
                                    &&(!(ueDLContext_p->pendingDRXAckNack)))
                            {
                                /*4X4 DL MIMO CHG START*/
                                minTBSizeToSendTADRX = determineTBSize(
                                        ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                        MIN_RB_FOR_BOTH_TA_DRX_ONLY,TB_MAPPED_TO_ONE_LAYER);
                            }
                            else
                            {
                                minTBSizeToSendTADRX = determineTBSize(
                                        ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                        MIN_RB_FOR_TA_DRX_ONLY,TB_MAPPED_TO_ONE_LAYER);
                            }/*4X4 DL MIMO CHG END*/
                            /* RETX_CHG */

                            /* ICIC changes start */
                            if ((!numberOfAvailableRBs_p[ueDLContext_p->userLocationType])|| (ueDLContext_p->permissibleTBSize < minTBSizeToSendTADRX))
                            /*Soft Buffer Changes End*/
                            {
                                /*SPR 6333 Fix Start*/
								if(flagForTADRXPendingFresh == PENDING_TA_FLAG)
								{
                                    FREE_MEM_NODE_TA_Q(taNode_p);
                                    /*SPR 7982 Fix Start*/
                                    ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                    /*SPR 7982 Fix End*/
								}
								else
                                {        
                                    UInt32 retVal = 0;
                                    retVal = ENQUEUE_TA_Q(pendingTAQueue_gp[internalCellIndex],
                                            TimeAdvanceQueueNode,&(taNode_p->timeAdvanceQueueNodeAnchor));
                                    if (!retVal)
                                    {
                                        lteWarning("Error in Pushing in Pending"
                                                " TAQueue in funtion %s\n",
                                                __func__);
                                        FREE_MEM_NODE_TA_Q(taNode_p);
                                        /*SPR 7982 Fix Start*/
                                        ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                        /*SPR 7982 Fix End*/
                                    }
								}
                                /*SPR 6333 Fix End*/
                                timeAdvanceQueueCount--;
                                continue;
                            }
                            tempRBCount = numberOfAvailableRBs_p[ueDLContext_p->userLocationType];                                
                            /* ICIC changes end */
                            /* Finding Free HarQ process to process TA*/
                            harqProcess_p = checkForFreeHarqProcess( 
                                ueDLContext_p, ueharqDLContext_p,
                                DRA_SCHEDULING, internalCellIndex);

                            /*If Free HarQ Process Found*/
                            if (harqProcess_p)
                            {
                                LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                                        "[DL-Strategy][%s]:Found Harq Process:%d"
                                        " Free , Queue Load is Zero",__func__,
                                        harqProcess_p->harqProcessId);

                                retTypeTADRXProcess = processTADRXNonSPALFreeHarq( ueDLContext_p,
                                                        harqProcess_p,
                                                        /* ICIC changes start */
                                                        &numberOfAvailableRBs_p[ueDLContext_p->userLocationType],
                                                        /* ICIC changes end */
                                                        flagForTADRXPendingFresh
#ifdef TDD_CONFIG
                                                        , subFrame
#endif
                                                        ,internalCellIndex
                                                        );
                                if (!retTypeTADRXProcess)
                                {
                                    /*SPR 6333 Fix Start*/
									if(flagForTADRXPendingFresh == PENDING_TA_FLAG)
									{
                                        FREE_MEM_NODE_TA_Q(taNode_p);
                                        /*SPR 7982 Fix Start*/
                                        ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                        /*SPR 7982 Fix End*/
									}
									else
									{        
										UInt32 retVal = 0;
                                        retVal = ENQUEUE_TA_Q( pendingTAQueue_gp[internalCellIndex],TimeAdvanceQueueNode,
                                            &(taNode_p->timeAdvanceQueueNodeAnchor));
										if (!retVal)
										{
											lteWarning("Error in Pushing in Pending"
													" TAQueue in funtion %s\n",
													__func__);
                                            FREE_MEM_NODE_TA_Q(taNode_p);
                                            /*SPR 7982 Fix Start*/
                                            ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                            /*SPR 7982 Fix End*/
										}
									}
                                    /*SPR 6333 Fix End*/
                                    timeAdvanceQueueCount--;
                                    continue;
                                    
                                }
                                /* ICIC changes start */
                                if (ueDLContext_p->userLocationType == CE_USER)
                                {
                                    /* UE is a cell edge UE, update the CC_CE_REGION RB count  */
                                    numberOfAvailableRBs_p[CC_CE_REGION] -= 
                                        (tempRBCount - numberOfAvailableRBs_p[ueDLContext_p->userLocationType]);
                                    maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;
                                }
                                else
                               {
                                    if ((numberOfAvailableRBs_p[CC_CE_REGION] < 
                                            numberOfAvailableRBs_p[CE_REGION]))
                                    {
                                        numberOfAvailableRBs_p[CE_REGION] =
                                            numberOfAvailableRBs_p[CC_CE_REGION];
                                    }
                                }
                                /* ICIC changes end */

                                /* Updating the reference flag in UE Context */
                                /* SPR_1293_FIX_START */
                                (*numTAUEScheduled_p)++;
                                alreadyScheduledUEs[internalCellIndex]++;
                                /* SPR_1293_FIX_END */
                                maxUEsToBeScheduledDL--;
                                ueDLContext_p->isAlreadyConsideredFlag 
                                    = dlReferenceFlag_g[internalCellIndex];
                            }
                            else
                                /*If Free Harq Process in not Found */
                            {
 
                                /*Unable to process TA hence Pushing
                                 * into pendingTAQueue */
                                LTE_MAC_UT_LOG(LOG_DETAIL,
                                        DL_STRATEGY,
                                        "[DL-Strategy][%s]Unable\
                                        to process TA,Pushing into Pending\
                                        TAQueue:Queue count:%d \n",__func__,
                                        timeAdvanceQueueCount);
                                /*SPR 6333 Fix Start*/
								if(flagForTADRXPendingFresh == PENDING_TA_FLAG)
								{
                                    FREE_MEM_NODE_TA_Q(taNode_p);
                                    /*SPR 7982 Fix Start*/
                                    ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                    /*SPR 7982 Fix End*/
								}
								else
								{        
									UInt32 retVal = 0;
                                    retVal = ENQUEUE_TA_Q(pendingTAQueue_gp[internalCellIndex],TimeAdvanceQueueNode,
                                            &(taNode_p->timeAdvanceQueueNodeAnchor));
									if(!retVal)
                                    {
										lteWarning("Error in Pushing in Pending"
												" TAQueue in funtion %s\n",
												__func__);
                                        FREE_MEM_NODE_TA_Q(taNode_p);
                                        /*SPR 7982 Fix Start*/
                                        ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                                        /*SPR 7982 Fix End*/
                                    }
                                }    
                                /*SPR 6333 Fix End*/
                                timeAdvanceQueueCount--;
                                continue;
                            }

                        }
                    }
                }/*end of already considered flag check, The UE which are 
                   already considered will be deleted automatically */
                else
                {
                    /* SPR 16814 fix start */
                    if(flagForTADRXPendingFresh == PENDING_TA_FLAG)
                    {
                        FREE_MEM_NODE_TA_Q(taNode_p);
                        ueDLContext_p->lastCalculatedTAAverage =MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                    }
                    else
                    {        
                        UInt32 retVal = 0;
                        retVal = ENQUEUE_TA_Q(pendingTAQueue_gp[internalCellIndex],TimeAdvanceQueueNode,
                                &(taNode_p->timeAdvanceQueueNodeAnchor));
                        if (!retVal)
                        {
                            lteWarning("Error in Pushing in Pending"
                                    " TAQueue in funtion %s\n",
                                    __func__);
                            FREE_MEM_NODE_TA_Q(taNode_p);
                            ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
                        }
                    }    
                    timeAdvanceQueueCount--;
                    continue;
                    /* SPR 16814 fix end */
                    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                            "[DL-Strategy][%s]:UE is already considered,"
                            " hence NOT considering UE,Updating TA %d in"
                            " UE's Context",
                            __func__,taNode_p->timeAdvanceValue);
                }
                
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
                        "UE Context is NULL or Pending to delele\n");
                LOG_MAC_MSG(MAC_QUEUE_NODE_NOT_PROCESSED,
                        LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                        ueDLContextInfo_p->pendingDeleteFlag,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE, 
                        DEFAULT_FLOAT_VALUE,
                        __func__,"TA Node not processed");
                /* TA_FIX */
                FREE_MEM_NODE_TA_Q(taNode_p);
                timeAdvanceQueueCount--;
                continue; 
                /* TA_FIX */
            }
            /* Also free the memory of TA Nodes*/

            /* TA_CHG */
            /* SPR 2628 Changes Start */
            /* Compute tavalue here and Update UE Context */
            
            ueDLContext_p->timingAdvanceValue = taNode_p->timeAdvanceValue;

            /* Refer Spec 36.213 Section 4.2.3 for the calculations 
             * NTA,new = NTA,old + (TA - 31) * 16 */
            ueDLContext_p->timingAdvanceAbsValue = 
                ueDLContext_p->timingAdvanceAbsValue + ((taNode_p->
                    timeAdvanceValue - MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31) 
                        << 4);
                    /*NEW_LOG_REQ*/
                  LOG_MAC_MSG(TIMING_ADVANCE_VALUE,LOGINFO,MAC_TA,
                         getCurrentTick(),ueDLContext_p->ueIndex, 
                         ueDLContext_p->timingAdvanceAbsValue,
                         taNode_p->timeAdvanceValue,
                         DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                         DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                         __func__,"");
            /* SPR 2628 Changes End */
            /* TA_CHG */

            FREE_MEM_NODE_TA_Q(taNode_p);
            timeAdvanceQueueCount--;
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_WARNING, DL_STRATEGY, "[DL-Strategy][%s] TA Queue returned NULL,"
                    " Queue count %d\n",
                    __func__, timeAdvanceQueueCount);
            break;
        }
    }
     /* EICIC +*/
    /* Check if count is still left , then pop nodes and push them back to maintiain same UE priority for next cycle */
     
    if ((timeAdvanceQueueCount) && (INVALID_UE_TYPE != ueTypeToScheduleDL_g[internalCellIndex]) && (isEicicEnabled_g[internalCellIndex]) && (isTraverseList))
    {
	    while(timeAdvanceQueueCount--) 
	    {
	        DEQUEUE_TA_Q(tempTimeAdvanceQueue_p,TimeAdvanceQueueNode, (void **)&taNodeScan_p);

		        if( PNULL != taNodeScan_p )
		        {
				                if(!ENQUEUE_TA_Q(pendingTAQueue_gp[internalCellIndex],TimeAdvanceQueueNode,&(taNodeScan_p->timeAdvanceQueueNodeAnchor)))
						        {
						            FREE_MEM_NODE_TA_Q(taNodeScan_p);
						        }
		          } 
		 }
    }

    /* EICIC -*/   
    dlScheduleData_p->dataNodeArrTotalUE = (UInt16)totalUE ;
    dlScheduleData_p->countTADRX =  (UInt16)totalUE;

    *maxUEsToBeScheduledDL_p = maxUEsToBeScheduledDL;

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Exit:[DL-Strategy][%s]"
            ,__func__);
}


/*****************************************************************************
 * Function Name  : processDRXQueue
 * Inputs         : dRXQueueCount - This is Count of the total node 
 *                     present in the DRX/Pending DRX Queue
 *                  numberOfAvailableRBs_p - pointer to current available RBs.
 *                  currentGlobalTTITickCount:Current Global Tick
 *                  dlScheduleData_p - Pointer to DL Scheduler Data Element
 *                  tempDRXQueue_p - Pointer to the DRX Queues either New or 
 *                     Pending DRXs
 *                  flagForTADRXPendingFresh - This Flag helps in identifying 
 *                     whether the Queue is Pending or Fresh.
 *                  maxUEsToBeScheduledDL_p - max UE's can scheduled in DL
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : It will allocate RBs to Pending DRX/Fresh DRX Queue Nodes &
 *                  place the DL Strategy O/P either in the PDCCHNewTXQueue_g
 *                  or PDCHHReTXQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the DRX Queues Nodes
 *                  fresh and Pending both  & Allocate RBs to the UEs. 
 *****************************************************************************/
void processDRXQueue(
        UInt32 dRXQueueCount, 
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        DLSchedulerDataElement * dlScheduleData_p,
        DRX_Q_TYPE  * tempDRXQueue_p,
        FlagForPendingTADRX flagForTADRXPendingFresh,
        /* ICIC changes start */
        UInt8 *maxUEsToBeScheduledDL_p,
        /* ICIC changes end */
        InternalCellIndex internalCellIndex)
{
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    ULUEContextInfo * ueULContextInfo_p = PNULL;
    DRXQueueNode * drxNode_p            = PNULL; 
    DLUEContext *ueDLContext_p          = PNULL;
    ULUEContext *ueULContext_p          = PNULL;
    DLHARQEntity *ueharqDLContext_p     = PNULL;
    DLHARQProcess *harqProcess_p        = PNULL;
    UInt32 ueIndex                      = 0; 
    UInt32 totalUE = dlScheduleData_p->dataNodeArrTotalUE;
#ifdef TDD_CONFIG
    UInt8 subFrame = currentGlobalTTITickCount % MAX_SUBFRAME;
#endif

    DRXContext          *drxContext_p = PNULL; /* DRX_CHG */
    MeasGapContextDL      *measGapContext_p = PNULL; /* MEAS_GAP_CHG */

    /* ICIC changes start */
    UInt32 tempRBCount                  = 0;
    UInt8 userLocationType = CC_USER;
    MacRetType retTypeTADRXProcess      = MAC_SUCCESS;
    /* ICIC changes end */
    /* EICIC +*/
    DRXQueueNode * drxNodeScan_p            =  PNULL;
    UInt8	isTraverseList = LTE_FALSE;
    /* EICIC -*/

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Entry:[DL-Strategy][%s]"
            "Available RBs:Total %d Cell Edge %d,DRX Queue Count:%d",__func__,
            numberOfAvailableRBs_p[CC_CE_REGION],numberOfAvailableRBs_p[CE_REGION],dRXQueueCount
            );

    while ((dRXQueueCount)&&(numberOfAvailableRBs_p[CC_CE_REGION])
            && *maxUEsToBeScheduledDL_p)
    {

        DEQUEUE_DRX_Q(tempDRXQueue_p, DRXQueueNode,(void **)&drxNode_p);

        if (drxNode_p)
        {
            ueIndex = drxNode_p->ueIdx;    
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            /* Cyclomatic_complexity_changes_start */ 
            if (validateUeStateForDrxQueueProcessing(ueDLContextInfo_p,currentGlobalTTITickCount,internalCellIndex))
            /* Cyclomatic_complexity_changes_end */
            {
                ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
                ueULContext_p = ueULContextInfo_p->ulUEContext_p;

	    /* EICIC +*/
/* Complexity Start */
		    if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType)) 
                   || ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
		    {   	            
			    if(!ENQUEUE_DRX_Q(tempDRXQueue_p, DRXQueueNode,&(drxNode_p->dRXQueueNodeAnchor)))
			    {
				    freeMemPool((void *)drxNode_p);
				    /* EICIC_Coverity CID 26441 Start */
				    drxNode_p = PNULL;
				    /* EICIC_Coverity CID 26441 End */
			    }
			    isTraverseList = LTE_TRUE;
			    dRXQueueCount--;
			    continue;
		    }
/* Complexity End */
		    /* EICIC -*/

                userLocationType = ueDLContext_p->userLocationType;
                /*Checking whether any Pending DRX present in UE Context*/
                if (!(ueDLContext_p->pendingDRXAckNack))
                {
                    /*Checking if Already being considered for RB 
                      Distribution*/
                    if ((ueDLContext_p)->isAlreadyConsideredFlag != 
                            dlReferenceFlag_g[internalCellIndex] )
                    {
                        ueharqDLContext_p = (ueDLContext_p)->dlHarqContext_p;           

                        /* DRX_CHG and MEAS_GAP_CHG */
                        drxContext_p = &ueDLContext_p->drxContext;
                        measGapContext_p = &ueDLContext_p->measGapContext;

                        if (DRX_STATE_ON == CHECK_UE_DRX_STATE(drxContext_p) || 
                                TRUE == CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p))
                        {
                            /* SPR 2377 changes start */
                            LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_DL_LOG_ID, LOGDEBUG, 
                                    MAC_DL_Strategy, currentGlobalTTITickCount, 
                                    ueIndex, CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                    __func__,"");

                            /* SPR 2377 changes end */
//#ifdef DL_UL_SPLIT
                            UInt32 retVal = 0;
                            retVal = ENQUEUE_DRX_Q( pendingDRXQueue_gp[internalCellIndex],
                                    DRXQueueNode,&(drxNode_p->dRXQueueNodeAnchor));
                            if (!retVal)
//#else
                             //   if (pushNode(&pendingDRXQueue,&(drxNode_p->dRXQueueNodeAnchor)))
//#endif
                                {
                                    lteWarning("Error in Pushing in Pending"
                                            " DRX Queue in funtion %s\n",
                                            __func__);
                                    FREE_MEM_NODE_DRX_Q(drxNode_p);
                                    drxNode_p = PNULL; /* COVERITY 26441 */
                                }
                            dRXQueueCount--;
                            continue;
                        }
                        /* DRX_CHG and MEAS_GAP_CHG */
                        if (ueDLContext_p->isAlreadyConsideredFlag != dlReferenceFlag_g[internalCellIndex] )
                        {

                            /* ICIC changes start */
                            if (((CE_USER == userLocationType) &&
                                        ((numberOfAvailableRBs_p[CE_REGION] <= ZERO)
                                         || ((!maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]))))
                                    /** HD FDD Changes **/
#ifdef HD_FDD_CONFIG
                                    || 
                                    ((TRUE == ueDLContext_p->isHDFddFlag) &&
                                     (!(CAN_SCHEDULE_DL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY, \
                                                                  ueIndex, hdUeContextInfo_g.hdfddUeSchedMap))))
#endif
                                    /** HD FDD Changes **/
                               )
                            {
                                LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_DL_LOG_ID, LOGDEBUG, 
                                        MAC_DL_Strategy, currentGlobalTTITickCount, 
                                        ueIndex, CHECK_UE_DRX_STATE(drxContext_p),
                                        CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                        __func__,"");

                                UInt32 retVal = 0;
                                retVal = ENQUEUE_DRX_Q( pendingDRXQueue_gp[internalCellIndex],
                                        DRXQueueNode,&(drxNode_p->dRXQueueNodeAnchor));
                                if (!retVal)
                                    {
                                        lteWarning("Error in Pushing in Pending"
                                                " DRX Queue in funtion %s\n",
                                                __func__);
                                        FREE_MEM_NODE_DRX_Q(drxNode_p);
                                        //drxNode_p = PNULL; /* COVERITY 26441 */
                                    }
                                dRXQueueCount--;
                                /* HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                                if (ueDLContext_p->isHDFddFlag == TRUE)
                                {
                                    /* Increment the statistics counter UE denied DL resources due to
                                     * UL clash in the same TTI of HD FDD UE*/
                                    UPDATE_UE_DENIED_DL_RESOURCE(ueIndex);
                                }
#endif
                                continue;
                            }
                        /*Queue Load is non Zero*/
                        //if ((NET_UE_QL(ueDLContext_p)) > ZERO)
                        if ((ueDLContext_p->ueQueueLoad) > ZERO ||
                                ueULContext_p->bsrNetTotal > ZERO)
                            /*process DRX when UE has Non Zero QueueLoad*/
                        {
/*START_DRX_OPT*/
                            freeMemPool(drxNode_p);
                            drxNode_p = PNULL; /* COVERITY 26441 */
                            dRXQueueCount--;
                            continue;
/*END_DRX_OPT*/
                        }
                        else
                        {
                            /* ICIC change start */
                            if (!numberOfAvailableRBs_p[ueDLContext_p->userLocationType])
                            {
                                /*Klocwork Fix*/
                                if(!(ENQUEUE_DRX_Q( pendingDRXQueue_gp[internalCellIndex],
                                                DRXQueueNode, (void *) drxNode_p)))
                                /*Klocwork Fix*/
                                {
                                    lteWarning("Error in Pushing in Pending"
                                            " DRX Queue in funtion %s\n",
                                            __func__);
                                    FREE_MEM_NODE_DRX_Q(drxNode_p);
                                    drxNode_p = PNULL; /* COVERITY 26441 */
                                }

                                dRXQueueCount--;
                                continue;
                            }
                            /* ICIC change end */
                            /* Finding Free HarQ process to process TA*/
                            harqProcess_p = checkForFreeHarqProcess( 
                                ueDLContext_p, ueharqDLContext_p,
                                DRA_SCHEDULING, internalCellIndex);

                            /*If Free HarQ Process Found*/
                            if (harqProcess_p)
                            {
                                LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                                        "[DL-Strategy][%s]:Found Harq Process:%d"
                                        " Free.Queue Load is Zero",__func__,
                                        harqProcess_p->harqProcessId);

                                tempRBCount = numberOfAvailableRBs_p[ueDLContext_p->userLocationType];                                

                                retTypeTADRXProcess = processTADRXNonSPALFreeHarq( ueDLContext_p,
                                                        harqProcess_p,
                                                        /* ICIC changes start */
                                                        &numberOfAvailableRBs_p[ueDLContext_p->userLocationType],
                                                        /* ICIC changes end */
                                                        flagForTADRXPendingFresh
#ifdef TDD_CONFIG
                                                        , subFrame
#endif
                                                        ,internalCellIndex
                                                        );

                                if (!retTypeTADRXProcess)
                                {
                                    /*Klocwork Fix*/
                                    if(!( ENQUEUE_DRX_Q( pendingDRXQueue_gp[internalCellIndex],
                                            DRXQueueNode, (void *) drxNode_p)))
                                    /*Klocwork Fix*/
                                    {
                                        lteWarning("Error in Pushing in Pending"
                                                " DRX Queue in funtion %s\n",
                                                __func__);
                                        FREE_MEM_NODE_DRX_Q(drxNode_p);
                                    }

                                    dRXQueueCount--;
                                    continue;
                                    
                                }
                                /* ICIC changes start */
                                if ((ueDLContext_p->userLocationType == CC_USER) &&
                                    (numberOfAvailableRBs_p[CC_CE_REGION] < 
                                     numberOfAvailableRBs_p[CE_REGION]))
                                {
                                    numberOfAvailableRBs_p[CE_REGION] = 
                                        numberOfAvailableRBs_p[CC_CE_REGION];
                                }
                                else
                                {
                                    maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;
                                    numberOfAvailableRBs_p[CC_CE_REGION] -= 
                                        (tempRBCount - numberOfAvailableRBs_p[ueDLContext_p->userLocationType]);
                                }

                                (*maxUEsToBeScheduledDL_p)--;
                                /* ICIC changes end */

                                /* Updating the reference flag in UE Context */
                                alreadyScheduledUEs[internalCellIndex]++;
                                ueDLContext_p->isAlreadyConsideredFlag 
                                    = dlReferenceFlag_g[internalCellIndex];
                            }
                            else
                                /*If Free Harq Process in not Found */
                            {
                                /*Unable to process DRX hence Pushing
                                 * into pending DRX Queue */
                                LTE_MAC_UT_LOG(LOG_DETAIL,
                                        DL_STRATEGY,
                                        "[DL-Strategy][%s]Unable to process DRX,"
                                        "Pushing into Pending DRXQueue:Queue"
                                        " count:%d \n",__func__,dRXQueueCount);

                                /*coverity id 26398*/
                                if(!ENQUEUE_DRX_Q( pendingDRXQueue_gp[internalCellIndex],
                                        DRXQueueNode , &(drxNode_p->dRXQueueNodeAnchor)))
                                {
                                    FREE_MEM_NODE_DRX_Q(drxNode_p);
                                }
                                dRXQueueCount--;
                                continue;
                            }
                        }
                        
                        /* + COVERITY 5.0 */
                        /* - COVERITY 5.0 */

                       }
                    }/*end of already considered flag check, The UE which are 
                       already considered will be deleted automatically */
                    else
                    {
                        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                                "[DL-Strategy][%s]:UE ID: %d is already "
                                "considered, hence NOT \
                                considering UE AlreadyConsideredFlag:%d"
                                " curr Global Tick %d"
                                ,__func__,ueDLContext_p->ueIndex,
                                ueDLContext_p->isAlreadyConsideredFlag,
                                currentGlobalTTITickCount );
                    }
                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                            "[DL-Strategy][%s]:Pending DRX Already in"
                            " UE Context,hence Pushing \
                            DRX Request Node in Pending DRX Queue",__func__);
                    UInt32 retVal = 0;
                    retVal = ENQUEUE_DRX_Q( pendingDRXQueue_gp[internalCellIndex],
                            DRXQueueNode ,&(drxNode_p->dRXQueueNodeAnchor));
                    if (!retVal)
                    {
                        lteWarning("Error in Pushing in Pending"
                                " DRX Queue in funtion %s\n",
                                __func__);
                        FREE_MEM_NODE_DRX_Q(drxNode_p);
                        drxNode_p = PNULL; /* COVERITY 26441 */
                    }

                    dRXQueueCount--;
                    continue;
                }
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
                        "UE Context is NULL or Pending to delele\n");
                LOG_MAC_MSG(MAC_QUEUE_NODE_NOT_PROCESSED,
                        LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                        ueDLContextInfo_p->pendingDeleteFlag,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE, 
                        DEFAULT_FLOAT_VALUE,
                        __func__,"DRX Node not processed");

            }
            /* Also free the memory of TA Nodes*/
            freeMemPool((void *)drxNode_p);
            drxNode_p = PNULL;
            dRXQueueCount--;
        }
        else
        {
            lteWarning("[DL-Strategy][%s] DRX Queue returned NULL:"
                    "Queue count=%d\n",
                    __func__, dRXQueueCount);
            break;
        }
    }

     /* EICIC +*/
    /* Check if count is still left , then pop nodes and push them back to maintiain same UE priority for next cycle */
     
    if ((dRXQueueCount) && (INVALID_UE_TYPE != ueTypeToScheduleDL_g[internalCellIndex]) && (isEicicEnabled_g[internalCellIndex]) && (isTraverseList))
    {
	    while(dRXQueueCount--) 
	    {
	          DEQUEUE_DRX_Q(tempDRXQueue_p, DRXQueueNode,(void **)&drxNodeScan_p);

		        if( PNULL != drxNodeScan_p )
		        {
				        if(!ENQUEUE_DRX_Q(tempDRXQueue_p, DRXQueueNode,&(drxNodeScan_p->dRXQueueNodeAnchor)))
					{
						  freeMemPool((void *)drxNodeScan_p);
						  /* EICIC_Coverity CID 69409 Start */
						  drxNodeScan_p = PNULL;
						  /* EICIC_Coverity CID 69409 End */
					}
		        } 
		 }
     }

    /* EICIC -*/


    dlScheduleData_p->dataNodeArrTotalUE = (UInt16) totalUE ;
    dlScheduleData_p->countTADRX = (UInt16)totalUE;

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Exit:[DL-Strategy][%s]"
            ,__func__);
}

/* + CQI_4.1 */
/* - CQI_4.1 */

/*****************************************************************************
 * Function Name  : processDLNackRoundRobin
 * Inputs         : ueDLContext_p - Pointer to UE Context who's nack is handled
 *                  harqProcess_p - Pointer to the UE's HARQ context 
 *                  dlStrategyTxNode_p - Pointer to the DL Strategy TX Node 
 *                  tBOneTBTwoReTxNewInfo - Contains information regarding TB one
 *                     and TB two i.e. which TB to be retransmit and which 
 *                     TB to be transmit first time
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : It will fill the parameters in rbAllocForNAck variable.
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This will determine the number of RBs required for a 
 *                  retransmission for NACK (whether it is for spatial 
 *                  multiplexing or non-spatial multiplexing)
 *****************************************************************************/
MacRetType processDLNackRoundRobin(
        DLUEContext * ueDLContext_p,
        DLHARQProcess *harqProcess_p, 
        DLStrategyTxNode *dlStrategyTxNode_p,
        TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo)
{   

    ResourceAllocatorInput * tempResourceAllocatorInput_p   = PNULL;
    tempResourceAllocatorInput_p = &(dlStrategyTxNode_p->resourceAllocInput) ;
    dlStrategyTxNode_p->isCQIRequestedULGrantMsg            = FALSE;
    tempResourceAllocatorInput_p->strictAllocSubBand        = FALSE;
    tempResourceAllocatorInput_p->subBandReportInfo_p       = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Entry:[DL-Strategy][%s]\n",
            __func__);

    switch (tBOneTBTwoReTxNewInfo){

        case TB_ONE_NEW_TB_TWO_RETX :
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
                        "[DL-Strategy][%s][TB_ONE:NEW TX],[TB_TWO:RETX]\n",__func__);
                tempResourceAllocatorInput_p->requiredRB = 
                    (harqProcess_p)->assignedResourcesNB ;
                /* Setting the transmission type as Adaptive ReTx :NEW_VERSION*/
                dlStrategyTxNode_p->transmissionType = RE_TX;
                harqProcess_p->dlHARQTBTwoInfo.retransmissionTBSize  =  
                    harqProcess_p->dlHARQTBTwoInfo.tbSize;

                harqProcess_p->dlHARQTBOneInfo.tbSize =
                    harqProcess_p->dlHARQTBTwoInfo.tbSize;
                harqProcess_p->dlHARQTBOneInfo.mcsIndex  =  
                    ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
            }
            break;
        case TB_ONE_RETX_TB_TWO_NEW:
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
                        "[DL-Strategy][%s][TB_ONE:RETX],[TB_TWO:NEW]\n",__func__);

                tempResourceAllocatorInput_p->requiredRB = 
                    (harqProcess_p)->assignedResourcesNB ;
                /* Setting the transmission type as Adaptive ReTx :NEW_VERSION*/
                dlStrategyTxNode_p->transmissionType = RE_TX;

                harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize  =  
                    harqProcess_p->dlHARQTBOneInfo.tbSize;

                harqProcess_p->dlHARQTBTwoInfo.tbSize = 
                    harqProcess_p->dlHARQTBOneInfo.tbSize;
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex  =  
                    ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
            }
            break;

        case TB_ONE_RETX_TB_TWO_RETX:
            {
                lteWarning("[DL-Strategy][%s]TBOneTBTwoReTxNewInfo choice is "
                        "TB_ONE_RETX_TB_TWO_RETX which is a INVALID choice", __func__);
                return MAC_FAILURE;
            }
            break;
        default: 
            {
                lteWarning("[DL-Strategy][%s]"
                        "TBOneTBTwoReTxNewInfo choice is INVALID",
                        __func__);
                return MAC_FAILURE;
            }
            break;
    }

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Exit:[DL-Strategy][%s]",__func__);

    return MAC_SUCCESS ;
}

/*****************************************************************************
 * Function Name : processTADRXNonSPALFreeHarq
 * Inputs        : ueDLContext_p - UE Context of the UE 
 *                 harqProcess_p - pointer to the UE's Harq Context
 *                 numberOfAvailableRBs_p - pointer to current available RBs.
 *                 flagForTADRXPendingFresh - This Flag helps in identifying
 *                 whether the Queue is Pending or Fresh.
 *                 internalCellIndex - cell index used at MAC
 * Outputs       : It will populate the Queues used by PDCCH with the DL
 *                 Strategy output Nodes
 * Returns       : MAC_SUCCESS or MAC_FAILURE
 * Description   : Its main function will be to process the TA/DRX Queues Nodes
 *                 when the UE has Zero Queue Load, Free Harq is available.
 *****************************************************************************/
#ifdef FDD_CONFIG
MacRetType  processTADRXNonSPALFreeHarq(
        DLUEContext* ueDLContext_p,
        DLHARQProcess *harqProcess_p, 
        UInt32 * numberOfAvailableRBs_p,
        FlagForPendingTADRX flagForTADRXPendingFresh,
        InternalCellIndex internalCellIndex)
#elif TDD_CONFIG
MacRetType  processTADRXNonSPALFreeHarq(
        DLUEContext* ueDLContext_p,
        DLHARQProcess *harqProcess_p, 
        UInt32 * numberOfAvailableRBs_p,
        FlagForPendingTADRX flagForTADRXPendingFresh,
        UInt8 subFrame,
        /* CA Changes Start */
        InternalCellIndex internalCellIndex
        /* CA Changes End */
        )
#endif
{
    DLStrategyTxNode *dlStrategyTxNode_p = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Entry:[DL-Strategy][%s]",
            __func__);
    GET_MEM_FROM_POOL(DLStrategyTxNode,dlStrategyTxNode_p,sizeof(DLStrategyTxNode),PNULL);

    /* + coverity 24537 */
    if (PNULL == dlStrategyTxNode_p)
    {
        freeDLHarqProcess (ueDLContext_p, harqProcess_p, internalCellIndex);
        return MAC_FAILURE;
    }
    /* - coverity 24537 */
    /*initialising the resource allocator input parameters*/
    ResourceAllocatorInput* tempResourceAllocatorInput_p = 
        &(dlStrategyTxNode_p->resourceAllocInput) ;                
    tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;        
    tempResourceAllocatorInput_p->strictAllocSubBand = FALSE;

    dlStrategyTxNode_p->isCQIRequestedULGrantMsg = FALSE;

    /*Updating the parameters & Fields of DL Strategy Tx node*/
    dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;
    dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId ;
    //SPR 8538 FIX
    dlStrategyTxNode_p->harqProcess_p = harqProcess_p; 

    if ((TX_MODE_3 == (ueDLContext_p->dlMIMOInfo).transmissionMode) ||
            (TX_MODE_4 == (ueDLContext_p->dlMIMOInfo).transmissionMode))
    {
        dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_1A;
        dlStrategyTxNode_p->secondaryDCIFormat = DCI_FORMAT_1A; 
    }
    /* SPR 15560 Fix Start*/
    /* + TM7_8 Changes Start */
    else if(((TX_MODE_7 == (ueDLContext_p->dlMIMOInfo).transmissionMode) ||
                (TX_MODE_8 == (ueDLContext_p->dlMIMOInfo).transmissionMode)) &&
            /* SPR 15560 Fix End*/
            (BF_ALGO_NOT_AVAILABLE !=
                (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->bfVectorPresentFlag) &&
            (1 < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas))
    {
        if(TX_MODE_7 == (ueDLContext_p->dlMIMOInfo).transmissionMode)
        {
            dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_1;

        }
        else if(TX_MODE_8 == (ueDLContext_p->dlMIMOInfo).transmissionMode)
        {
            dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_2B;
        }
        /* SPR 15560 Fix Start*/
        /** Dead code removed **/
        /* SPR 15560 Fix End*/
        dlStrategyTxNode_p->secondaryDCIFormat = DCI_FORMAT_1A; 
    }
    /* - TM7_8 Changes End */
    else
    {
        dciFormatSelector ( ueDLContext_p, dlStrategyTxNode_p, 
                HARQ_FREE_TX_OTHER_THAN_3_4, internalCellIndex);
    }

    switch (flagForTADRXPendingFresh)
    {
        case PENDING_TA_FLAG:
        case NEW_TA_FLAG:
        if ((ueDLContext_p->drxFlag) 
                &&(!(ueDLContext_p->pendingDRXAckNack)))
        {
            dlStrategyTxNode_p->msgType = TA_DRX_NON_SPATIAL_MUL;
            harqProcess_p->dlHARQTBOneInfo.taDrxFlag = TA_DRX_MSG;

#ifdef FDD_CONFIG							
            tempResourceAllocatorInput_p->requiredRB = 
                MIN_RB_FOR_BOTH_TA_DRX_ONLY;
#elif TDD_CONFIG
            tempResourceAllocatorInput_p->requiredRB = 
                MIN_RB_FOR_BOTH_TA_DRX_ONLY_TDD;
#endif
        }
        else
        {
            dlStrategyTxNode_p->msgType = TA_NON_SPATIAL_MUL;
            harqProcess_p->dlHARQTBOneInfo.taDrxFlag = TA_MSG;
            tempResourceAllocatorInput_p->requiredRB = 
                MIN_RB_FOR_TA_DRX_ONLY ;
        }
        break;
        case PENDING_DRX_FLAG:
        case NEW_DRX_FLAG:
        /* +- SPR 22243 Fix*/
        if (ueDLContext_p->timingAdvanceValue!=INVALID_TA_VALUE)
        {
            dlStrategyTxNode_p->msgType = TA_DRX_NON_SPATIAL_MUL;
            harqProcess_p->dlHARQTBOneInfo.taDrxFlag = TA_DRX_MSG;

#ifdef FDD_CONFIG							
            tempResourceAllocatorInput_p->requiredRB = 
                MIN_RB_FOR_BOTH_TA_DRX_ONLY;
#elif TDD_CONFIG
            tempResourceAllocatorInput_p->requiredRB = 
                MIN_RB_FOR_BOTH_TA_DRX_ONLY_TDD;
#endif	
        }
        else
        {   
            dlStrategyTxNode_p->msgType = DRX_NON_SPATIAL_MUL;
            harqProcess_p->dlHARQTBOneInfo.taDrxFlag = DRX_MSG;
            tempResourceAllocatorInput_p->requiredRB = 
                MIN_RB_FOR_TA_DRX_ONLY ;
        }
        break;
        default:
        {
            lteWarning("[DL-Strategy][%s]"
                    "flagForTADRXPendingFresh is Wrong",
                    __func__);
        }
        break ;                                                                              
    }

    /*Since Free Harq is picked up,Only One TB is used to send TA/DRX*/
    harqProcess_p->isTBOneValid = TRUE;
    harqProcess_p->isTBTwoValid = FALSE;
    harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;

    /* To Send TA/DRX, One RB is allocated,update in resource allocator 
     * input and update the remaining RBs*/

    /* ICIC changes start */
    if (*numberOfAvailableRBs_p < tempResourceAllocatorInput_p->requiredRB)
    {
        freeMemPool(dlStrategyTxNode_p);
        /* SPR 21035 start */
        freeDLHarqProcess (ueDLContext_p, harqProcess_p, internalCellIndex); 
        /* SPR 21035 end   */ 
        return MAC_FAILURE;
    }

    *numberOfAvailableRBs_p -= tempResourceAllocatorInput_p->requiredRB;

    dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated = ueDLContext_p->userLocationType;
    /* ICIC changes end */

    /*Finding whether Subband Report is available */
    if ((ueDLContext_p->dlCQIInfo.isSubBandReportAvailable)&&
            (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
             initParams_p->dlSchdConfig.dlSchedulerStrategy == DL_FAIR_SCHEDULING))
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
                "[DL-Strategy][%s]INSIDE SubBand Report Available\n"
                ,__func__);
        /* Updating the subband Pointer in resource allocator input if the
         * subband report is available within UE context */
        tempResourceAllocatorInput_p->subBandReportInfo_p = ueDLContext_p 
            ->dlCQIInfo.subBandReportInfo_p;
    }

    harqProcess_p->assignedResourcesNB = 
        (tempResourceAllocatorInput_p->requiredRB);

    harqProcess_p->dlHARQTBOneInfo.mcsIndex  =  
        ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;

#ifdef FDD_CONFIG
    /*4X4 DL MIMO CHG START*/
    harqProcess_p->dlHARQTBOneInfo.tbSize  =  determineTBSize(
            harqProcess_p->dlHARQTBOneInfo.mcsIndex, 
            tempResourceAllocatorInput_p->requiredRB,TB_MAPPED_TO_ONE_LAYER);
    /*4X4 DL MIMO CHG END*/
#elif TDD_CONFIG
    if (checkForSpecialSubframe(subFrame, internalCellIndex))
    {/*4X4 DL MIMO CHG START*/
        harqProcess_p->dlHARQTBOneInfo.tbSize  =  determineTBSize(
                harqProcess_p->dlHARQTBOneInfo.mcsIndex, 
                SP_SUBFRAME_PRB(tempResourceAllocatorInput_p->requiredRB) ,TB_MAPPED_TO_ONE_LAYER);
    }/*4X4 DL MIMO CHG END*/
    else
    {
        harqProcess_p->dlHARQTBOneInfo.tbSize  =  determineTBSize(
                harqProcess_p->dlHARQTBOneInfo.mcsIndex, 
                tempResourceAllocatorInput_p->requiredRB,TB_MAPPED_TO_ONE_LAYER);
    }
#endif

    dlStrategyTxNode_p->transmissionType = NEW_TX;
        /* SPR 16054 Fix Start */
        dlStrategyTxNode_p->minTBSize = harqProcess_p->dlHARQTBOneInfo.tbSize;
        dlStrategyTxNode_p->maxTBSize = harqProcess_p->dlHARQTBOneInfo.tbSize;
        /* SPR 16054 Fix End */

    dlStrategyTxNode_p->sumLCPriority = FALSE;
    dlStrategyTxNode_p->weightedQueueLoad = FALSE;
    /* + SPR 22243 Fix*/
    dlStrategyTxNode_p->allocPushCount = 0;  
    /* - SPR 22243 Fix*/
    /*putEntry of DLStrategyTXNode in PDCCH Queue */
    if (!(putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex)))
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DL_STRATEGY,
                "[DL-Strategy][%s]dlStrategyTxNode_p Entry is failed\n",
                __func__);
        /* SPR 21036 start */ 
        /* freeMemPool(dlStrategyTxNode_p);*/
        /* SPR 21036 end   */ 
        /* SPR 21035 start */
        freeDLHarqProcess (ueDLContext_p, harqProcess_p, internalCellIndex);
        /* SPR 21035 end   */
        /* + coverity 32503 */
        return MAC_FAILURE;
        /* - coverity 32503 */
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Exit:[DL-Strategy][%s]",
            __func__);
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name : calculateMinMaxTBForRRMimo 
 * Inputs        : netUeQ1 - net Queue load of UE 
 *                 ueDLContext_p - pointer to the DL UE context 
 *                 tempTBSizeRet - Tmeprory TB size used 
 * Outputs       : minTBS - pointer to store minimum TB size 
 *                 maxTBS - pointer to store maximum TB size 
 * Returns       : None 
 * Description   : This function will calculate the minimum and maximum TB size
 *                 for RR schedular MIMO case
 *****************************************************************************/
void calculateMinMaxTBForRRMimo(
                     SInt32 netUeQ1,
                     DLUEContext* ueDLContext_p,
                     UInt32 tempTBSizeRet,
                     UInt32 *minTBS,
                     UInt32 *maxTBS)
{
    /* SPR 15446 Fix Start */
    UInt32 maxSISOTBSize = 0;
    if (ueDLContext_p->ueCategory == 2)
    {
        /*SPR 20899 Fix Start*/
        maxSISOTBSize = DIVIDE_BY_TWO(ueDLContext_p->maxTBSize);
        /*SPR 20899 Fix Stop*/
    }
    else
    {
        maxSISOTBSize = ueDLContext_p->maxSISOTBSize;
    }
    /* SPR 15446 Fix End */
    /* QOS_PDSCH */
    if ( netUeQ1 > (SInt64)tempTBSizeRet)	
    {
        /* SPR 14807 fix start */
        if ( netUeQ1 > (SInt64)maxSISOTBSize )
        {
            if ( tempTBSizeRet > maxSISOTBSize )
            {
                *minTBS = maxSISOTBSize ;
                *maxTBS = maxSISOTBSize;
            }
            else
            {
                *minTBS = tempTBSizeRet;
                *maxTBS = maxSISOTBSize;
                /* SPR 14807 fix end */
            }
        }
        else
        {

            *minTBS = tempTBSizeRet;
            *maxTBS = netUeQ1;
        }
    }
    else
    {
        /* SPR 14807 fix start */
        if ( netUeQ1 > (SInt64)maxSISOTBSize )
        {
            *minTBS = maxSISOTBSize;
            *maxTBS = maxSISOTBSize;
            /* SPR 14807 fix end */
    /* SPR 15446 Fix End */
        }
        else
        {
            *minTBS = netUeQ1;
            *maxTBS = netUeQ1;
        }
    }
    /* QOS_PDSCH */
}

/*****************************************************************************
 * Function Name : calculateMinMaxTBForRR 
 * Inputs        : netUeQ1 - net Queue load of UE 
 *                 ueDLContext_p - pointer to the DL UE context 
 *                 tempTBSizeRet - Tmeprory TB size used 
 * Outputs       : minTBS - pointer to store minimum TB size 
 *                 maxTBS - pointer to store maximum TB size 
 * Returns       : None 
 * Description   : This function will calculate the minimum and maximum TB size
 *                 for RR schedular 
 *****************************************************************************/
void calculateMinMaxTBForRR(
                     SInt32 netUeQ1,
                     DLUEContext* ueDLContext_p,
                     UInt32 tempTBSizeRet,
                     UInt32 **minTBS,
                     UInt32 **maxTBS)
{
    /* QOS_PDSCH */
    if ( netUeQ1 > (SInt64)tempTBSizeRet)
    {
        if ( netUeQ1 > ueDLContext_p->maxTBSize )
        {
            if ( tempTBSizeRet > ueDLContext_p->maxTBSize )
            {
                **minTBS = ueDLContext_p->maxTBSize ;
                **maxTBS = ueDLContext_p->maxTBSize;
#ifdef PERF_STATS
                /*  number of ms where respective UE is limited in the
                 *  DL direction by its UE capability */
                gMacUePerfStats_p[ueDLContext_p->ueIndex].lteUeMacSchedDLPerfStats.\
                    totalTimeRestrictUeInDlbyUeCap++;
#endif
            }
            else
            {
                **minTBS = tempTBSizeRet;
                **maxTBS = ueDLContext_p->maxTBSize;
            }
        }
        else
        {
            
            **minTBS = tempTBSizeRet;
            **maxTBS = netUeQ1;
        }
    }
    else
    {
        if ( netUeQ1 > ueDLContext_p->maxTBSize )
	{
        **minTBS = ueDLContext_p->maxTBSize;
        **maxTBS = ueDLContext_p->maxTBSize;
#ifdef PERF_STATS
        /*  number of ms where respective UE is limited in the
         *   DL direction by its UE capability */
        gMacUePerfStats_p[ueDLContext_p->ueIndex].lteUeMacSchedDLPerfStats.\
            totalTimeRestrictUeInDlbyUeCap++;
#endif
    }
        else
        {  
            **minTBS = netUeQ1;
            **maxTBS = netUeQ1;
        }
    }
    /* QOS_PDSCH */
}

/*****************************************************************************
 * Function Name  : validateRBForOneTB
 * Inputs         : ueDLContext_p - Pointer to UE Context
 *                  allocatedRB -The Number of RBs Allocated Orginally
 *                  numOfNonZeroLCs - Number of Non Zero Queue Load 
 *                    logical Channel
 *                  tbSizeRet - It is pointer to the TB Size
 *                  availableRB_p -Pointer to the number of Avavailable RBs
 *                  msgType - message type to check whether TA/DRX/both
 *                  minTBS - pointer contains minimum TB size 
 *                  maxTBS - pointer contains maximum TB size 
 *                  subFrame -  Sub Frame Number
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : validated the UE queue Load and assign the RB which can 
                    be used by UE
 * Returns        : allocatedRB : These are the actual RB Allocated after 
                    the validation
 * Description    : This functions checks whether a UE really requires the RBs
 *                  allocated to and if less number of RBs are required ,then
 *                  only that amount of RBs are allocated.
 *****************************************************************************/
UInt32 validateRBForOneTB (
        DLUEContext* ueDLContext_p,
        UInt32 allocatedRB,
        UInt32 numOfNonZeroLCs,
        UInt32* tbSizeRet,
        UInt32 availableRB,
        MsgType msgType,
        /* +- SPR 17777 */
        /* QOS_PDSCH */
        UInt32 *minTBS,
        UInt32 *maxTBS
        /* QOS_PDSCH */
#ifdef TDD_CONFIG
        , UInt8 subFrame
        /* +- SPR 17777 */
        ,InternalCellIndex internalCellIndex
#endif
        /* +- SPR 17777 */
        )
{
    UInt32 dataSize         = 0;
    UInt32 previousDataSize = 0;
    SInt32 netUeQl          = 0;
    UInt32 tempTBSizeRet    = *tbSizeRet;
	
#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe = 0;
#endif

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "[DL-Strategy][%s(allocatedRB %u)]Entry\n",__func__,allocatedRB);
    /*Soft Buffer Changes Start*/
    if (ueDLContext_p->permissibleTBSize < HARQ_TB_MINIMUM_SIZE_IN_BYTES)
    {
        return ZERO;
    }
    /*Soft Buffer Changes End*/

    /*If allcatedRB's are more the maximum RBs for tht UE then set the
     * allocatedRB's to maximum RBs*/
    if (allocatedRB > ueDLContext_p->maxNbOfRBs)
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "[DL-Strategy][%s]allocated RBs( %d ) > MAX No.of RBs(%d ) "
                "for UE ID:%d,Hence making allocatedRB = %d ",
                __func__,allocatedRB,ueDLContext_p->maxNbOfRBs, 
                ueDLContext_p->ueIndex,ueDLContext_p->maxNbOfRBs);
        allocatedRB = ueDLContext_p->maxNbOfRBs;

    }
    if (allocatedRB > availableRB)
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "[DL-Strategy][%s]allocated RBs( %d ) > Available RBs Total (%d ), Cell Edge (%d )"
                "to UE ID:%d,Hence making allocatedRB = %d ",
                __func__,allocatedRB,
                availableRB, availableRB,
                ueDLContext_p->ueIndex,availableRB
                );
        allocatedRB = availableRB;
    }

    /*determine TB size from the MCS Index and from the allocated RBs.  
      If alllocated bits is less than minimum TB Size,Then 
      Increasae the allocated RBs to cross the minimum TB Size limitation */

#ifdef FDD_CONFIG
/* + CQI_5.0 */
    tempTBSizeRet = determineTBSize(
                          ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                         allocatedRB,TB_MAPPED_TO_ONE_LAYER);
/*    
    else if(TB_TWO == tbValue)
    {
        tempTBSizeRet = determineTBSize(
                          ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo,
                         allocatedRB );
    }    
*/    
#elif TDD_CONFIG
/* SPR 609 changes start */
    isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
    /* Cyclomatic_complexity_changes_start */
    checkSplSfAndCalcTbsizeSiso(isSpecialSubframe, 
                                ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne, 
                                allocatedRB, 
                                &tempTBSizeRet);
    /* Cyclomatic_complexity_changes_end */
/*    
    else if(TB_TWO == tbValue)
    {

         checkSplSfAndCalcTbsizeSiso(isSpecialSubframe, 
                                                 ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo, 
                                                 allocatedRB, 
                                                 tempTBSizeRet);
    }     
*/        
/* + CQI_5.0 */
/* SPR 609 changes end */
#endif

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "[DL-Strategy][%s]TB Size UE ID:%d is %d, mcsIndex %d,"
            "allocated RBs =%d ",__func__,
            ueDLContext_p->ueIndex,tempTBSizeRet,
            ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,allocatedRB);

    if (tempTBSizeRet < HARQ_TB_MINIMUM_SIZE_IN_BYTES)
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "[DL-Strategy][%s]TB Size is Less Than MINIMUM TB SIZE\n",
                __func__);
        
        while ( (tempTBSizeRet  < HARQ_TB_MINIMUM_SIZE_IN_BYTES) &&
                (allocatedRB < ueDLContext_p->maxNbOfRBs)&&
                (allocatedRB < availableRB)
                )
        {
            allocatedRB++;

#ifdef FDD_CONFIG
            /*4X4 DL MIMO CHG START*/
            tempTBSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                            allocatedRB,TB_MAPPED_TO_ONE_LAYER);
           /*4X4 DL MIMO CHG END*/
#elif TDD_CONFIG            
            /* Cyclomatic_complexity_changes_start */
            checkSplSfAndCalcTbsizeSiso(isSpecialSubframe, ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                                          allocatedRB, &tempTBSizeRet);
            /* Cyclomatic_complexity_changes_end */
#endif			
        }
        
        /*If after increasing the RBs to cross the minimum TB Size limitation,
          the allocatedRB are less than available then return with 0 RBs*/
        if (tempTBSizeRet < HARQ_TB_MINIMUM_SIZE_IN_BYTES)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                    "[DL-Strategy][%s]TB Size after increasing also Less"
                    " Than MINIMUM TB SIZE"
                    " Hence Returning Zero\n",__func__);
            return ZERO;
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                    "[DL-Strategy][%s]After increasing meets the MINIMUM"
                    " TB Size Allocated RBs: %d \n",__func__,allocatedRB);

            netUeQl = NET_UE_QL(ueDLContext_p);

/* SPR #3081 Changes Starts */
            netUeQl+= HARQ_TB_MINIMUM_SIZE_IN_BYTES; 
/* SPR #3081 Changes Ends */
            calculateMinMaxTBForRR(
                              netUeQl,
                              ueDLContext_p,
                              tempTBSizeRet,
                              &minTBS,
                              &maxTBS);
            /*Soft Buffer Changes Start*/
            if (ueDLContext_p->permissibleTBSize < tempTBSizeRet)
            {
                return ZERO;
            }
            /*Soft Buffer Changes End*/

            *tbSizeRet = tempTBSizeRet;

            /* SPR 15446 Fix Start */
            if (dlSchedulableUECount_g == 1)
            {
                allocatedRB = allocateRbBasedOnQload(ueDLContext_p,availableRB,1);
            }
            /* SPR 15446 Fix End */
            return allocatedRB;
        }
    }
    else
    {
        /*Calculating the NET Queue load of the UE, and if that become zero or
          less than return with 0 RBs */
        netUeQl = NET_UE_QL(ueDLContext_p);

/* SPR #3081 Changes Starts */
            netUeQl+= HARQ_TB_MINIMUM_SIZE_IN_BYTES; 
/* SPR #3081 Changes Ends */
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"[DL-Strategy][%s]"
                "NET_UE_QL of UE ID %d is %d",
                __func__,ueDLContext_p->ueIndex,netUeQl);

        UInt32 tempLength =0;

        /* Cyclomatic_complexity_changes_start */
        tempLength = getTempLength(msgType);
        /* Cyclomatic_complexity_changes_end */

        /*Now the Validation to match the UE Queue Load is done, by decreasing 
          the RBs allocated against the Queue Load */
        do {
            /*Assuming each logical channel will be assingned one RB */
#ifdef FDD_CONFIG
            /*4X4 DL MIMO CHG START*/ 
            tempTBSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                            allocatedRB,TB_MAPPED_TO_ONE_LAYER);
            /*4X4 DL MIMO CHG END*/ 
            /* SPR 14163 Changes Starts  */
            /*Soft Buffer Changes Start*/
            if (tempTBSizeRet > ueDLContext_p->permissibleTBSize)
            {
                allocatedRB--;
                continue;
            }
            /*Soft Buffer Changes End*/ 
            /* SPR 14163 Changes Ends  */

#elif TDD_CONFIG
            /* Cyclomatic_complexity_changes_start */ 
            checkSplSfAndCalcTbsizeSiso(isSpecialSubframe, 
                                                    ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                                    allocatedRB, 
                                                    &tempTBSizeRet);
            /* Cyclomatic_complexity_changes_end */    
#endif				

            dataSize = (tempTBSizeRet - 
                    (MAC_SDU_SUBHEADER_SIZE_IN_BYTES) * (numOfNonZeroLCs)
                    - tempLength);

            if ((netUeQl > (SInt64)dataSize)||
                    (tempTBSizeRet < HARQ_TB_MINIMUM_SIZE_IN_BYTES))
            {
                if (previousDataSize)
                {
                    allocatedRB++;
#ifdef FDD_CONFIG
                    /*4X4 DL MIMO CHG START*/
                    tempTBSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                            allocatedRB,TB_MAPPED_TO_ONE_LAYER);
                    /*4X4 DL MIMO CHG END*/ 
#elif TDD_CONFIG
                    /* Cyclomatic_complexity_changes_start */
                    checkSplSfAndCalcTbsizeSiso(isSpecialSubframe,
                                                            ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                                            allocatedRB,
                                                             &tempTBSizeRet);
                    /* Cyclomatic_complexity_changes_end */
#endif						

                }

                calculateMinMaxTBForRR(
                              netUeQl,
                              ueDLContext_p,
                              tempTBSizeRet,
                              &minTBS,
                              &maxTBS);

                *tbSizeRet  = tempTBSizeRet;

                /* SPR 15446 Fix Start */
                if (dlSchedulableUECount_g == 1) 
                {
                    allocatedRB = allocateRbBasedOnQload(ueDLContext_p,availableRB,1);
                }
                /* SPR 15446 Fix End */
                return allocatedRB;
            }
            else if (netUeQl == (SInt64)dataSize)
            {
                *tbSizeRet = tempTBSizeRet;

                calculateMinMaxTBForRR(
                              netUeQl,
                              ueDLContext_p,
                              tempTBSizeRet,
                              &minTBS,
                              &maxTBS);

                /* SPR 15446 Fix Start */
                if (dlSchedulableUECount_g == 1)
                {
                    allocatedRB = allocateRbBasedOnQload(ueDLContext_p,availableRB,1);
                }
                /* SPR 15446 Fix End */
                return allocatedRB;
            }

            previousDataSize = dataSize;
            allocatedRB--;
        } while (allocatedRB > 1 );
    }
    /*It could well be the case when queueload is less than data size of one RB
      in tht case the above for loop decrements the allocatedRB to zero, 
      it should be  atleast 1 to clear that very less data */
    if (!allocatedRB)
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "[DL-Strategy][%s]Very less Qload ( %u )Still 1 RB is Allocated\n",
                __func__,netUeQl);
        allocatedRB = ONE;
    }

    LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
      "[DL-Strategy][%s]allocatedRB at exit %u\n",__func__
      ,allocatedRB);
/* SPR 2930 CHANGES START */
    /* SPR 21122 fix start */
    /* code removed */
    /* if availableRB = 1 then allocatedRB becomes two which is greater than
     * availableRB */ 
    /* SPR 21122 fix end */
/* SPR 2930 CHANGES END*/

/* SPR 1599 changes start */
#ifdef FDD_CONFIG
     /*4X4 DL MIMO CHG START*/
    tempTBSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                            allocatedRB,TB_MAPPED_TO_ONE_LAYER);
    /*4X4 DL MIMO CHG END*/
    /*Soft Buffer Changes Start*/
    if (tempTBSizeRet > ueDLContext_p->permissibleTBSize)
    {
        return ZERO;
    }
#elif TDD_CONFIG
      /* Cyclomatic_complexity_changes_start */
      checkSplSfAndCalcTbsizeSiso(isSpecialSubframe,
                                  ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                  allocatedRB,
                                  &tempTBSizeRet);
      /* Cyclomatic_complexity_changes_end */
#endif						
   *tbSizeRet = tempTBSizeRet;

    calculateMinMaxTBForRR(
                    netUeQl,
                    ueDLContext_p,
                    (*tbSizeRet),
                    &minTBS,
                    &maxTBS);


    /* SPR 15446 Fix Start */
    if (dlSchedulableUECount_g == 1)
    {
        allocatedRB = allocateRbBasedOnQload(ueDLContext_p,availableRB,1);
    }
    /* SPR 15446 Fix End */
    return allocatedRB;
}

/*4x4 DL MIMO CHG START*/

/*****************************************************************************
 * Function Name  : determineTBSizeForRank3And4
 * Inputs         : ueDLContext_p - Pointer to UE Context 
                    allocatedRB -The Number of RBs Allocated Orginally    
                    numOfNonZeroLCs - Number of Non Zero Queue Load 
                    logical Channel
                    tempTBOneSizeRet - It is the pointer to TB Size of TB One
                    tempTBTwoSizeRet - It is the pointer to TB Size of TB Two
                    tempLength - Length of a particular MsgType. 
                    harqProcess_p - It is ptr to dl Harq process info  
 *                  subFrame -  Sub Frame Number
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : validated the UE queue Load against the Two TBs and assign
                    the RB which can be used by UE
 * Returns        : allocatedRB : These are the actual RB can be allocated after 
                    the validation
 * Description    : This functions checks whether a UE of rank 3 or 4,can be 
 *                  allocated the increased TB Size for codewords wrt Qload
 *                  and totalDataSize of tb1 and tb2.
 *****************************************************************************/
UInt32 determineTBSizeForRank3And4(DLUEContext* ueDLContext_p,
                                   UInt32 allocatedRB,
                                   UInt32 numOfNonZeroLCs,
                                   UInt32* tempTBOneSizeRet_p,
                                   UInt32* tempTBTwoSizeRet_p,
                                   /* +- SPR 17777 */
#ifdef TDD_CONFIG
                                   UInt8 subFrame,
                                   InternalCellIndex internalCellIndex, 
#endif
                                   UInt32 tempLength,
                                   DLHARQProcess *harqProcess_p)
                                   /* +- SPR 17777 */
{
    UInt32 tempTBOneSize = 0;
    UInt32 tempTBTwoSize = 0;
    UInt32 dataSize = 0;
    UInt32 previousDataSize = 0;
#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe = 0;
#endif	
    SInt32 netUeQl = NET_UE_QL(ueDLContext_p);
#ifdef FDD_CONFIG   
    /*old version of determineTBSize*/
    tempTBOneSize  = determineTBSize(ueDLContext_p->dlCQIInfo.
                                     mcsIndexCodeWordOne, allocatedRB,TB_MAPPED_TO_ONE_LAYER);
    /*new version of determineTBSize*/
    tempTBTwoSize  = determineTBSize(ueDLContext_p->dlCQIInfo.
                                     mcsIndexCodeWordTwo, allocatedRB,TB_MAPPED_TO_TWO_LAYER);
#elif TDD_CONFIG
    /* SPR 609 changes start */
    isSpecialSubframe = checkForSpecialSubframe(subFrame,internalCellIndex);
    /* Cyclomatic_complexity_changes_start */
    checkSplSfAndCalcTbsizeMimo(isSpecialSubframe, ueDLContext_p, allocatedRB,
                                            &tempTBOneSize, &tempTBTwoSize,RANK_INDICATOR_3);
    /* Cyclomatic_complexity_changes_end */
    /* SPR 609 changes end */
#endif		
    dataSize = ((tempTBOneSize + tempTBTwoSize) - 
               ((MAC_SDU_SUBHEADER_SIZE_IN_BYTES) * (numOfNonZeroLCs))
                - tempLength);
    if ((netUeQl > (SInt64)dataSize) && (harqProcess_p->numOfLayer > RANK_INDICATOR_3)) 
    {    
        do
        {
#ifdef FDD_CONFIG   
            /*old version of determineTBSize*/
            tempTBOneSize  = determineTBSize(ueDLContext_p->dlCQIInfo.
                                          mcsIndexCodeWordOne, allocatedRB,TB_MAPPED_TO_TWO_LAYER);
            /*new version of determineTBSize*/
            tempTBTwoSize  = determineTBSize(ueDLContext_p->dlCQIInfo.
                                           mcsIndexCodeWordTwo, allocatedRB,TB_MAPPED_TO_TWO_LAYER);
#elif TDD_CONFIG
            /* SPR 609 changes start */
            isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
            /* Cyclomatic_complexity_changes_start */
            checkSplSfAndCalcTbsizeMimo(isSpecialSubframe, ueDLContext_p, allocatedRB,
                                                      &tempTBOneSize, &tempTBTwoSize,RANK_INDICATOR_4);
            /* Cyclomatic_complexity_changes_end */
            /* SPR 609 changes end */
#endif

            dataSize = ((tempTBOneSize + tempTBTwoSize) -
                   ((MAC_SDU_SUBHEADER_SIZE_IN_BYTES) * (numOfNonZeroLCs))
                    - tempLength);
            if (netUeQl > (SInt64)dataSize)
            {
                if (previousDataSize)
                {
                    allocatedRB++;
                    #ifdef FDD_CONFIG                       
                    tempTBOneSize  = determineTBSize(ueDLContext_p->dlCQIInfo.
                                        mcsIndexCodeWordOne, allocatedRB,TB_MAPPED_TO_TWO_LAYER);
                    tempTBTwoSize = determineTBSize(ueDLContext_p->dlCQIInfo.
                                        mcsIndexCodeWordTwo, allocatedRB,TB_MAPPED_TO_TWO_LAYER);
#elif TDD_CONFIG
                    /* SPR 609 changes start */
                    isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
                    /* Cyclomatic_complexity_changes_start */
                    checkSplSfAndCalcTbsizeMimo(isSpecialSubframe, ueDLContext_p, allocatedRB,
                                                             &tempTBOneSize, &tempTBTwoSize,RANK_INDICATOR_4);
                    /* Cyclomatic_complexity_changes_end */
                    /* SPR 609 changes end */
#endif
                }
                *tempTBOneSizeRet_p = tempTBOneSize;
                *tempTBTwoSizeRet_p = tempTBTwoSize;
                return allocatedRB;
            }
            else if (netUeQl == (SInt64)dataSize)
            {
                *tempTBOneSizeRet_p = tempTBOneSize;
                *tempTBTwoSizeRet_p = tempTBTwoSize;

                return allocatedRB;
            }
            previousDataSize = dataSize;
            allocatedRB--;
        } while (allocatedRB > 1);
    }       
    else if (netUeQl == (SInt64)dataSize)
    {
        *tempTBOneSizeRet_p = tempTBOneSize;
        *tempTBTwoSizeRet_p = tempTBTwoSize;

        return allocatedRB;
    }    
    else
    {
       if(harqProcess_p->numOfLayer == RANK_INDICATOR_4)
       {    
           harqProcess_p->numOfLayer = RANK_INDICATOR_3;
       }    
       do 
       {
#ifdef FDD_CONFIG   
            /*old version of determineTBSize*/
    	    tempTBOneSize  = determineTBSize(ueDLContext_p->dlCQIInfo.
                                         mcsIndexCodeWordOne, allocatedRB,TB_MAPPED_TO_ONE_LAYER);
    	    /*new version of determineTBSize*/
            tempTBTwoSize  = determineTBSize(ueDLContext_p->dlCQIInfo.
                                         mcsIndexCodeWordTwo, allocatedRB,TB_MAPPED_TO_TWO_LAYER);
#elif TDD_CONFIG
            /* SPR 609 changes start */
            isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
            /* Cyclomatic_complexity_changes_start */
            checkSplSfAndCalcTbsizeMimo(isSpecialSubframe, ueDLContext_p, allocatedRB,
                                                    &tempTBOneSize, &tempTBTwoSize,RANK_INDICATOR_3);
            /* Cyclomatic_complexity_changes_end */
            /* SPR 609 changes end */
#endif		
            dataSize = ((tempTBOneSize + tempTBTwoSize) - 
                    ((MAC_SDU_SUBHEADER_SIZE_IN_BYTES) * (numOfNonZeroLCs))
                    - tempLength);
            if (netUeQl > (SInt64)dataSize)
            {
                if (previousDataSize)
                {
                    allocatedRB++;
#ifdef FDD_CONFIG                       
                    tempTBOneSize  = determineTBSize(ueDLContext_p->dlCQIInfo.
	                                             mcsIndexCodeWordOne, allocatedRB,TB_MAPPED_TO_ONE_LAYER);
                    tempTBTwoSize = determineTBSize(ueDLContext_p->dlCQIInfo.
	                                            mcsIndexCodeWordTwo, allocatedRB,TB_MAPPED_TO_TWO_LAYER);
#elif TDD_CONFIG
                    /* SPR 609 changes start */
                    isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
                    /* Cyclomatic_complexity_changes_start */
                    checkSplSfAndCalcTbsizeMimo(isSpecialSubframe, ueDLContext_p, allocatedRB,
                                                                &tempTBOneSize, &tempTBTwoSize,RANK_INDICATOR_3);
                    /* Cyclomatic_complexity_changes_end */
                    /* SPR 609 changes end */
#endif					
                }
                *tempTBOneSizeRet_p = tempTBOneSize;
                *tempTBTwoSizeRet_p = tempTBTwoSize;
                return allocatedRB;
            }
            else if (netUeQl == (SInt64)dataSize)
            {
                *tempTBOneSizeRet_p = tempTBOneSize;
                *tempTBTwoSizeRet_p = tempTBTwoSize;
                return allocatedRB;
            }
            previousDataSize = dataSize;
            allocatedRB--;
        }while (allocatedRB > 1);
    }
    return allocatedRB;
}
/*4x4 DL MIMO CHG END*/    
/*****************************************************************************
 * Function Name  : validateRBForTwoTB
 * Inputs         : ueDLContext_p - Pointer to UE Context 
 *                  allocatedRB -The Number of RBs Allocated Orginally    
 *                  numOfNonZeroLCs - Number of Non Zero Queue Load 
 *                  logical Channel
 *                  tbOneSizeRet- It is the pointer to TB Size of TB One
 *                  tbTwoSizeRet- It is the pointer to TB Size of TB Two
 *                  availableRB_p -Pointer to the number of Avavailable RBs
 *                  msgType - message type to check whether TA/DRX/both 
 *                  minTBS - pointer contains minimum TB size 
 *                  maxTBS - pointer contains maximum TB size 
                    harqProcess_p - It is ptr to dl Harq process info  
 *                  subFrame -  Sub Frame Number
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : validated the UE queue Load against the Two TBs and assign
 *                  the RB which can be used by UE
 * Returns        : allocatedRB : These are the actual RB Allocated after 
                    the validation
 * Description    : This functions checks whether a UE really requires the RBs
 *                  allocated to and if less number of RBs are required ,then
 *                  only that amount of RBs are allocated.
 *****************************************************************************/
UInt32 validateRBForTwoTB (
        DLUEContext* ueDLContext_p,
        UInt32 allocatedRB,
        UInt32 numOfNonZeroLCs,
        UInt32* tbOneSizeRet,
        UInt32* tbTwoSizeRet,
        UInt32 availableRB,
        MsgType msgType,
        UInt32 *minTBS,
        UInt32 *maxTBS,
#ifdef TDD_CONFIG
        UInt8 subFrame,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex,
#endif
        /*4x4 DL MIMO CHG START*/
        DLHARQProcess *harqProcess_p
        /*4x4 DL MIMO CHG END*/
        /* +- SPR 17777 */
      )
/* + coverity 55310 */
{
    UInt32 dataSize = 0;
    UInt32 previousDataSize = 0;
    UInt32 tempLength = 0;
    SInt32 netUeQl          = 0;
    UInt32 tempTBOneSizeRet = *tbOneSizeRet;
    UInt32 tempTBTwoSizeRet = *tbTwoSizeRet;
    UInt32 minTBS1 = 0;
    UInt32 maxTBS1 = 0;
    UInt32 minTBS2 = 0;
    UInt32 maxTBS2 = 0;

#ifdef TDD_CONFIG
    UInt8 isSpecialSubframe = 0;
#endif	

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "[DL-Strategy][%s(allocatedRB %u)]Entry\n",__func__,allocatedRB);
    /*Soft Buffer Changes Start*/
    if (ueDLContext_p->permissibleTBSize < HARQ_TB_MINIMUM_SIZE_IN_BYTES)
    {
        return ZERO;
    }
    /*Soft Buffer Changes End*/

    /*If allcatedRB's are more the maximum RBs for tht UE then set the
     * allocatedRB's to maximum RBs'*/
    if (allocatedRB > ueDLContext_p->maxNbOfRBs)
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "[DL-Strategy][%s]allocated RBs( %d ) > MAX No.of RBs(%d ) "
                "for UE ID:%d,Hence making allocatedRB = %d ",
                __func__,allocatedRB,ueDLContext_p->maxNbOfRBs, 
                ueDLContext_p->ueIndex,ueDLContext_p->maxNbOfRBs);
        allocatedRB = ueDLContext_p->maxNbOfRBs;
    }

    /* ICIC changes start */
    if (allocatedRB > availableRB)
        /* ICIC changes end */
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "[DL-Strategy][%s]allocated RBs( %d ) > Available RBs Total (%d ), Cell Edge (%d )"
                "to UE ID:%d,Hence making allocatedRB = %d ",
                __func__,allocatedRB,
                availableRB, availableRB, 
                ueDLContext_p->ueIndex,availableRB
                );
        /* ICIC changes start */
        allocatedRB = availableRB;
        /* ICIC changes end */
    }

    /*determine TB size from the MCS Index and from the allocated RBs.  
      If alllocated bits is less than minimum TB Size,Then 
      Increasae the allocated RBs to cross the minimum TB Size limitation */

#ifdef FDD_CONFIG     
    /*4X4 DL MIMO CHG START*/  
    tempTBOneSizeRet  = determineTBSize(ueDLContext_p->dlCQIInfo.
                                        mcsIndexCodeWordOne, allocatedRB, TB_MAPPED_TO_ONE_LAYER);
    tempTBTwoSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                                        mcsIndexCodeWordTwo, allocatedRB, TB_MAPPED_TO_ONE_LAYER);
    /*4X4 DL MIMO CHG END*/
#elif TDD_CONFIG
    /* SPR 609 changes start */
    isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
    /* Cyclomatic_complexity_changes_start */
    checkSplSfAndCalcTbsizeMimo(isSpecialSubframe, ueDLContext_p, allocatedRB, 
                                            &tempTBOneSizeRet, &tempTBTwoSizeRet,RANK_INDICATOR_2); 
    /* Cyclomatic_complexity_changes_end */

#endif

    netUeQl = NET_UE_QL(ueDLContext_p);

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "[DL-Strategy][%s]UE ID :%d's TB One Size :%d , TB Two Size %d"
            ",Net Queue Load  %d , HARQ_TB_MINIMUM_SIZE :%d",
            __func__,ueDLContext_p->ueIndex,
            tempTBOneSizeRet, tempTBTwoSizeRet, netUeQl,
            HARQ_TB_MINIMUM_SIZE_IN_BYTES );


    /*When TBOne is minimum TB Size then TBOne size needs to
      increase to meet the HARQ MINIMUM Size */
    if (tempTBOneSizeRet <= tempTBTwoSizeRet )
    {
        if ( tempTBOneSizeRet < HARQ_TB_MINIMUM_SIZE_IN_BYTES )
        {
            while ( (tempTBOneSizeRet < HARQ_TB_MINIMUM_SIZE_IN_BYTES) &&
                    (allocatedRB < ueDLContext_p->maxNbOfRBs)&&
                    /* ICIC changes start */
                    (allocatedRB < availableRB)
                    /* ICIC changes end */
                  )
            {
                allocatedRB++;
#ifdef FDD_CONFIG
                /*4X4 DL MIMO CHG START*/
                tempTBOneSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                                       mcsIndexCodeWordOne, allocatedRB ,TB_MAPPED_TO_ONE_LAYER);
                /*4X4 DL MIMO CHG END*/
#elif TDD_CONFIG
                /* Cyclomatic_complexity_changes_start */ 
                checkSplSfAndCalcTbsizeSiso(isSpecialSubframe,
                        ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                        allocatedRB,
                        &tempTBOneSizeRet);
                /* Cyclomatic_complexity_changes_end */
#endif					
            }
            /*If after increasing the RBs to cross the minimum TB Size 
              limitation,the allocatedRB are less than available then 
              return with 0 RBs*/
            if (tempTBOneSizeRet < HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                LTE_MAC_UT_LOG(LOG_WARNING,DL_STRATEGY,
                        "[DL-Strategy][%s]TB Size after increasing also Less "
                        "Than MINIMUM TB SIZE"
                        " Hence Returning Zero\n",__func__);
                return ZERO;
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                        "[DL-Strategy][%s]After increasing meets the"
                        " MINIMUM TB Size Allocated RBs: %d \n",
                        __func__,allocatedRB);
                /*Soft Buffer Changes Start*/
                if (ueDLContext_p->permissibleTBSize < tempTBOneSizeRet)
                {
                    return ZERO;
                }
                /*Soft Buffer Changes End*/

#ifdef FDD_CONFIG
                /*4X4 DL MIMO CHG START*/
                tempTBTwoSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo,
                       
                                            allocatedRB ,TB_MAPPED_TO_ONE_LAYER);
                /*4X4 DL MIMO CHG END*/
#elif TDD_CONFIG
                /* Cyclomatic_complexity_changes_start */
                checkSplSfAndCalcTbsizeSiso(isSpecialSubframe, 
                        ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                        allocatedRB,
                        &tempTBTwoSizeRet);
                /* Cyclomatic_complexity_changes_end */
#endif					
                *tbOneSizeRet  = tempTBOneSizeRet;
                calculateMinMaxTBForRRMimo(
                        netUeQl,
                        ueDLContext_p,
                        tempTBOneSizeRet,
                        &minTBS1,
                        &maxTBS1);

                /* SPR 14807 fix start */
                /* Subtract netUeQl with minimum size of TB1 as it is already
                 * considered */
                netUeQl -= minTBS1; 
                /* SPR 14807 fix end */
                /*Soft Buffer Changes Start*/
                if (((SInt64)tempTBOneSizeRet >= 
                            (netUeQl + HARQ_TB_MINIMUM_SIZE_IN_BYTES-1)) ||
                        (tempTBTwoSizeRet > (ueDLContext_p->permissibleTBSize - tempTBOneSizeRet)))
                    /*Soft Buffer Changes End*/
                {
                    *tbTwoSizeRet=0;

                    minTBS2 = 0;
                    maxTBS2 = 0;
                }
                else
                {
                    *tbTwoSizeRet = tempTBTwoSizeRet ;

                    calculateMinMaxTBForRRMimo(
                            netUeQl,
                            ueDLContext_p,
                            tempTBTwoSizeRet,
                            &minTBS2,
                            &maxTBS2);

                }

                *minTBS = minTBS1 + minTBS1;
                *maxTBS = maxTBS1 + maxTBS2;

                /* SPR 15446 Fix Start */
                if (dlSchedulableUECount_g == 1)
                {
                    allocatedRB = allocateRbBasedOnQload(ueDLContext_p,availableRB,2);
                }
                /* SPR 15446 Fix End */
                return allocatedRB;
            }                
        }
    }
    /*When TBTwo is minimum TB Size then TBTwo size needs to
      increase to meet the HARQ MINIMUM Size */
    else
    {
        if ( tempTBTwoSizeRet < HARQ_TB_MINIMUM_SIZE_IN_BYTES )
        {
            while ( (tempTBTwoSizeRet < HARQ_TB_MINIMUM_SIZE_IN_BYTES) &&
                    (allocatedRB <= ueDLContext_p->maxNbOfRBs)&&
                    /* ICIC changes start */
                    (allocatedRB < availableRB)
                    /* ICIC changes end */
                  )
            {
                allocatedRB++;

#ifdef FDD_CONFIG
                /*4X4 DL MIMO CHG START*/  
                tempTBTwoSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                        
            	                            mcsIndexCodeWordTwo, allocatedRB ,TB_MAPPED_TO_ONE_LAYER);
               /*4X4 DL MIMO CHG END*/
#elif TDD_CONFIG
                /* Cyclomatic_complexity_changes_start */
                checkSplSfAndCalcTbsizeSiso(isSpecialSubframe,
                                            ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                            allocatedRB,
                                            &tempTBTwoSizeRet);
                /* Cyclomatic_complexity_changes_end */
#endif						
            }
            /*If after increasing the RBs to cross the minimum TB Size
              limitation,the allocatedRB are less than available then
              return with 0 RBs*/

            if (tempTBTwoSizeRet < HARQ_TB_MINIMUM_SIZE_IN_BYTES)
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                        "[DL-Strategy][%s]TB Size after increasing also Less"
                        " Than MINIMUM TB SIZE Hence Returning Zero\n",
                        __func__);
                return ZERO;
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                        "[DL-Strategy][%s]After increasing meets the"
                        " MINIMUM TB Size Allocated RBs: %d \n",
                        __func__,allocatedRB);

#ifdef FDD_CONFIG
                /*4X4 DL MIMO CHG START*/  
                tempTBOneSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                                       mcsIndexCodeWordOne, allocatedRB ,TB_MAPPED_TO_ONE_LAYER);
                /*4X4 DL MIMO CHG END*/
                /*Soft Buffer Changes Start*/
                if (ueDLContext_p->permissibleTBSize < tempTBOneSizeRet)
                {
                    return ZERO;
                }
                /*Soft Buffer Changes End*/

#elif TDD_CONFIG
                /* Cyclomatic_complexity_changes_start */
                checkSplSfAndCalcTbsizeSiso(isSpecialSubframe, 
                                            ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne, 
                                            allocatedRB,
                                            &tempTBOneSizeRet);
                /* Cyclomatic_complexity_changes_end */
#endif					

                *tbOneSizeRet  = tempTBOneSizeRet;

                calculateMinMaxTBForRRMimo(
                        netUeQl,
                        ueDLContext_p,
                        tempTBOneSizeRet,
                        &minTBS1,
                        &maxTBS1);
                /* SPR 14807 fix start */
                /* Subtract netUeQl with minimum size of TB1 as it is already
                 * considered */
                netUeQl -= minTBS1; 
                /* SPR 14807 fix end */
                /*Soft Buffer Changes Start*/
                if (((SInt64)tempTBOneSizeRet >= 
                            (netUeQl + HARQ_TB_MINIMUM_SIZE_IN_BYTES-1)) ||
                        (tempTBTwoSizeRet > (ueDLContext_p->permissibleTBSize - tempTBOneSizeRet)))
                    /*Soft Buffer Changes End*/
                {
                    *tbTwoSizeRet=0;
                    minTBS2 = 0;
                    maxTBS2 = 0;
                }
                else
                {
                    *tbTwoSizeRet = tempTBTwoSizeRet ;
                    calculateMinMaxTBForRRMimo(
                            netUeQl,
                            ueDLContext_p,
                            tempTBTwoSizeRet,
                            &minTBS2,
                            &maxTBS2);


                }

                *minTBS = minTBS2 + minTBS1;
                *maxTBS = maxTBS1 + maxTBS2;

                /* SPR 15446 Fix Start */
                if (dlSchedulableUECount_g == 1)
                {
                    allocatedRB = allocateRbBasedOnQload(ueDLContext_p,availableRB,2);
                }
                /* SPR 15446 Fix End */
                return allocatedRB;
            }
        }
    }
    /*Calculating the NET Queue load of the UE, and if that become zero or
      less than return with 0 RBs */
    dataSize = ((tempTBOneSizeRet + tempTBTwoSizeRet) - 
            ((MAC_SDU_SUBHEADER_SIZE_IN_BYTES) * (numOfNonZeroLCs))
            - tempLength);

    /* Cyclomatic_complexity_changes_start */
    tempLength = getTempLength(msgType);
    /* Cyclomatic_complexity_changes_end */
    /*Now the Validation to match the UE Queue Load is done, by decreasing 
      the RBs allocated against the Queue Load */
    do 
    {
#ifdef FDD_CONFIG      
        tempTBOneSizeRet  = determineTBSize(ueDLContext_p->dlCQIInfo.
                mcsIndexCodeWordOne, allocatedRB, TB_MAPPED_TO_ONE_LAYER);
        tempTBTwoSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                mcsIndexCodeWordTwo, allocatedRB ,TB_MAPPED_TO_ONE_LAYER );
#elif TDD_CONFIG
        /* SPR 609 changes start */
        isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
         /* Cyclomatic_complexity_changes_start */
        checkSplSfAndCalcTbsizeMimo(isSpecialSubframe, ueDLContext_p, allocatedRB,
                                                &tempTBOneSizeRet, &tempTBTwoSizeRet,RANK_INDICATOR_2);
          /* Cyclomatic_complexity_changes_end */
        /* SPR 609 changes end */
#endif		

        dataSize = ((tempTBOneSizeRet + tempTBTwoSizeRet) - 
                ((MAC_SDU_SUBHEADER_SIZE_IN_BYTES) * (numOfNonZeroLCs))
                - tempLength);
        /* SPR 14163 Changes Starts  */
        /*Soft Buffer Changes Start*/
        if (((tempTBOneSizeRet + tempTBTwoSizeRet) > (ueDLContext_p->maxTBSize))||\
                ueDLContext_p->permissibleTBSize < tempTBOneSizeRet)
        {
        /*Soft Buffer Changes End*/
            allocatedRB--;
            continue;
        }
        /* SPR 14163 Changes Ends  */
        if ((netUeQl > (SInt64)dataSize) || 
                ((tempTBOneSizeRet > tempTBTwoSizeRet ? tempTBTwoSizeRet 
                  : tempTBOneSizeRet) 
                 < HARQ_TB_MINIMUM_SIZE_IN_BYTES ))
        {
            if (previousDataSize)
            {
                allocatedRB++;

#ifdef FDD_CONFIG      
                /*4X4 DL MIMO CHG START*/
                tempTBOneSizeRet  = determineTBSize(ueDLContext_p->dlCQIInfo.
                        mcsIndexCodeWordOne, allocatedRB , TB_MAPPED_TO_ONE_LAYER);
                tempTBTwoSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                        mcsIndexCodeWordTwo, allocatedRB , TB_MAPPED_TO_ONE_LAYER);
#elif TDD_CONFIG
                /* SPR 609 changes start */
                isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
                 /* Cyclomatic_complexity_changes_start */
                checkSplSfAndCalcTbsizeMimo(isSpecialSubframe, ueDLContext_p, allocatedRB,
                                                        &tempTBOneSizeRet, &tempTBTwoSizeRet,RANK_INDICATOR_2);
                 /* Cyclomatic_complexity_changes_end */
                /* SPR 9031 Fix Start */
                if ( TRUE == isSpecialSubframe )
                {
                    /* In case of Special Subframe, for optimum allocation of required RB's
                     * increasing RB by 1 till it does not affect the TBSIZE
                     * e.g (FLOOR(0.75 * 4(RB)) = 3, FLOOR(0.75 * 5(RB)) = 3 and thus calculation of TBSize is same
                     */
                    /* + SPR 17439 */
                    if ( ( availableRB > allocatedRB ) && (netUeQl > ((SInt64)((tempTBOneSizeRet + tempTBTwoSizeRet) -
                        ((2*MAC_SDU_SUBHEADER_SIZE_IN_BYTES) * (numOfNonZeroLCs))
                        - tempLength))) )
                    /* - SPR 17439 */    
                    {
                        allocatedRB++;
                        /* Cyclomatic_complexity_changes_start */
                        checkSplSfAndCalcTbsizeMimo(isSpecialSubframe, ueDLContext_p, allocatedRB,
                                        &tempTBOneSizeRet, &tempTBTwoSizeRet, RANK_INDICATOR_2);
                                        
                        /* Cyclomatic_complexity_changes_end */
                    }
                }
                /* SPR 9031 Fix End */

                /* SPR 609 changes end */
#endif					
            }
            /*4x4 MIMO CHG START*/
            else if (harqProcess_p->numOfLayer == RANK_INDICATOR_3 || harqProcess_p->numOfLayer == RANK_INDICATOR_4)
            {
               /*Allocate the increased TB size for rank 3 and 4 codewords for 4x4 dl mimo*/     
               allocatedRB = determineTBSizeForRank3And4(ueDLContext_p,
                                                         allocatedRB,
                                                         numOfNonZeroLCs,
                                                         &tempTBOneSizeRet,
                                                         &tempTBTwoSizeRet,
                                                         /* +- SPR 17777 */
#ifdef TDD_CONFIG                                           
                                                         subFrame,
                                                         internalCellIndex,
#endif                  
                                                         tempLength,
                                                         harqProcess_p
                                                         ); 
                                                         /* +- SPR 17777 */
                
                
            }    
            /*4x4 MIMO CHG END*/

            /* SPR 14163 Changes Starts  */
            /* SPR 4543 changes start */
            while ( (tempTBOneSizeRet + tempTBTwoSizeRet) > ueDLContext_p->maxTBSize )
            {
                allocatedRB--;
#ifdef FDD_CONFIG

              /* Coverity_73216 Fix Start */  
                if(0 != allocatedRB) 
                {
                    tempTBOneSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                        mcsIndexCodeWordOne, allocatedRB, TB_MAPPED_TO_ONE_LAYER);
                }
              /* Coverity_73216 Fix End */

#elif TDD_CONFIG
                /* TDD_Warning_Fix */
                checkSplSfAndCalcTbsizeSiso(isSpecialSubframe, 
                                            ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne, 
                                            allocatedRB,&tempTBOneSizeRet);
                /* TDD_Warning_Fix */
#endif                                                                   

#ifdef FDD_CONFIG

              /* Coverity_73216 Fix Start */  
                if(0 != allocatedRB) 
                {
            tempTBTwoSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo,
                    allocatedRB, TB_MAPPED_TO_ONE_LAYER);
                }
              /* Coverity_73216 Fix End */

#elif TDD_CONFIG
            /* TDD_Warning_Fix */
            checkSplSfAndCalcTbsizeSiso(isSpecialSubframe, 
                                        ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo,
                                        allocatedRB,&tempTBTwoSizeRet);
            /* TDD_Warning_Fix */
#endif                                                                   
            }
            /* SPR 4543 changes end */
            /* SPR 14163 Changes Ends  */

            *tbOneSizeRet  = tempTBOneSizeRet;

            calculateMinMaxTBForRRMimo(
                    netUeQl,
                    ueDLContext_p,
                    tempTBOneSizeRet,
                    &minTBS1,
                    &maxTBS1);

            /* SPR 14807 fix start */
            /* Subtract netUeQl with minimum size of TB1 as it is already
             * considered */
            netUeQl -= minTBS1; 
            /* SPR 14807 fix end */
            /* SPR 14163 Changes Starts  */
            /*Soft Buffer Changes Start*/
            if (((SInt64)tempTBOneSizeRet >= 
                    (netUeQl + HARQ_TB_MINIMUM_SIZE_IN_BYTES-1)) ||
                    (tempTBTwoSizeRet>(ueDLContext_p->permissibleTBSize-tempTBOneSizeRet)))
                /*Soft Buffer Changes End*/
            {
                *tbTwoSizeRet=0;
                 minTBS2 = 0;
                 maxTBS2 = 0;
            }
            else
            {
                *tbTwoSizeRet = tempTBTwoSizeRet ;
                /* SPR 14163 Changes Ends  */

            calculateMinMaxTBForRRMimo(
                    netUeQl,
                    ueDLContext_p,
                    tempTBTwoSizeRet,
                    &minTBS2,
                    &maxTBS2);

            }

            *minTBS = minTBS2 + minTBS1;
            *maxTBS = maxTBS1 + maxTBS2;

            /* SPR 14163 Changes Starts  */
            /* SPR 4543 changes start */
            if(ueDLContext_p->maxTBSize < *maxTBS)
            {
                *maxTBS = ueDLContext_p->maxTBSize;
            }
            /* SPR 4543 changes end */
            /* SPR 14163 Changes Ends  */
#ifdef PERF_STATS
            /*  number of ms where respective UE is limited in the
             *  DL direction by its UE capability */
            gMacUePerfStats_p[ueDLContext_p->ueIndex].lteUeMacSchedDLPerfStats.\
                totalTimeRestrictUeInDlbyUeCap++;
#endif

            /* SPR 15446 Fix Start */
            if (dlSchedulableUECount_g == 1)
            {
                allocatedRB = allocateRbBasedOnQload(ueDLContext_p,availableRB,2);
            }
            /* SPR 15446 Fix End */
            return allocatedRB;
        }
        else if (netUeQl == (SInt64)dataSize)
        {
            *tbOneSizeRet  = tempTBOneSizeRet;

            calculateMinMaxTBForRRMimo(
                    netUeQl,
                    ueDLContext_p,
                    tempTBOneSizeRet,
                    &minTBS1,
                    &maxTBS1);
            /* SPR 14807 fix start */
            /* Subtract netUeQl with minimum size of TB1 as it is already
             * considered */
            netUeQl -= minTBS1; 
            /* SPR 14807 fix end */

            /* SPR 14163 Changes Starts  */
            /*Soft Buffer Changes Start*/
            if (((SInt64)tempTBOneSizeRet >= 
                    (netUeQl + HARQ_TB_MINIMUM_SIZE_IN_BYTES-1)) ||
                    (tempTBTwoSizeRet>(ueDLContext_p->permissibleTBSize-tempTBOneSizeRet)))
                /*Soft Buffer Changes End*/
            {
                *tbTwoSizeRet=0;
                minTBS2 = 0;
                maxTBS2 = 0;
            }
            else
            {
                *tbTwoSizeRet = tempTBTwoSizeRet ;
            /* SPR 14163 Changes Ends  */

            calculateMinMaxTBForRRMimo(
                    netUeQl,
                    ueDLContext_p,
                    tempTBTwoSizeRet,
                    &minTBS2,
                    &maxTBS2);


            }

            *minTBS = minTBS2 + minTBS1;
            *maxTBS = maxTBS1 + maxTBS2;

            /* SPR 15446 Fix Start */
            if (dlSchedulableUECount_g == 1)
            {
                allocatedRB = allocateRbBasedOnQload(ueDLContext_p,availableRB,2);
            }
            /* SPR 15446 Fix End */
            return allocatedRB;
        }

        previousDataSize = dataSize;
        allocatedRB--;
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "[DL-Strategy][%s]#### UE ID :%d's TB One Size :%d , TB Two Size %d"
                ",Net Queue Load  %d , HARQ_TB_MINIMUM_SIZE :%d",
                __func__,ueDLContext_p->ueIndex,
                tempTBOneSizeRet, tempTBTwoSizeRet, netUeQl,
                HARQ_TB_MINIMUM_SIZE_IN_BYTES );
    } while (allocatedRB > 1);

    /*It could well be the case when queueload is less than data size of one RB
      in tht case the above for loop decrements the allocatedRB to zero, it 
      should be atleast 1 to clear that very less data. */
    if (!allocatedRB)
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "[DL-Strategy][%s]Very less Qload ( %u ) Still 1 RB is Allocated\n",
                __func__ ,netUeQl);
        allocatedRB = ONE;
    }

    LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
            "[DL-Strategy][%s]allocatedRB at exit %u\n",__func__,allocatedRB);

    /* SPR 1683 changes start */
#ifdef FDD_CONFIG      
                /*4X4 DL MIMO CHG START*/
    *tbOneSizeRet  = determineTBSize(ueDLContext_p->dlCQIInfo.
            mcsIndexCodeWordOne, allocatedRB , TB_MAPPED_TO_ONE_LAYER );
    *tbTwoSizeRet  = determineTBSize(ueDLContext_p->dlCQIInfo.
            mcsIndexCodeWordTwo, allocatedRB , TB_MAPPED_TO_ONE_LAYER );
    /*Soft Buffer Changes Start*/
    if (ueDLContext_p->permissibleTBSize < *tbOneSizeRet )
    {
        return ZERO;
    }
    /*Soft Buffer Changes End*/

#elif TDD_CONFIG
    /* SPR 609 changes start */
    isSpecialSubframe = checkForSpecialSubframe(subFrame, internalCellIndex);
    if (isSpecialSubframe)
        /* SPR 609 changes end */
    {
        *tbOneSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                mcsIndexCodeWordOne,
                SP_SUBFRAME_PRB(allocatedRB), TB_MAPPED_TO_ONE_LAYER); 

        *tbTwoSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                mcsIndexCodeWordTwo,
                SP_SUBFRAME_PRB(allocatedRB), TB_MAPPED_TO_ONE_LAYER);
    }
    else
    {
        *tbOneSizeRet  = determineTBSize(ueDLContext_p->dlCQIInfo.
                mcsIndexCodeWordOne, allocatedRB , TB_MAPPED_TO_ONE_LAYER);
        *tbTwoSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                mcsIndexCodeWordTwo, allocatedRB , TB_MAPPED_TO_ONE_LAYER);
    }
#endif					

    calculateMinMaxTBForRRMimo(
            netUeQl,
            ueDLContext_p,
            (*tbOneSizeRet),
            &minTBS1,
            &maxTBS1);
    /* SPR 14807 fix start */
    /* Subtract netUeQl with minimum size of TB1 as it is already
     * considered */
    netUeQl -= minTBS1; 
    /* SPR 14807 fix end */
    /*Soft Buffer Changes Start*/	
    if (((SInt64)*tbOneSizeRet >= 
                (netUeQl + HARQ_TB_MINIMUM_SIZE_IN_BYTES-1)) ||
            ((*tbTwoSizeRet)>(ueDLContext_p->permissibleTBSize-(*tbOneSizeRet))))
        /*Soft Buffer Changes End*/
    {
        *tbTwoSizeRet=0;
        minTBS2 = 0;
        maxTBS2 = 0;
    }
    else
    {
        calculateMinMaxTBForRRMimo(
                netUeQl,
                ueDLContext_p,
                (*tbTwoSizeRet),
                &minTBS2,
                &maxTBS2);
    }
    /* SPR 1683 changes end */

    *minTBS = minTBS2 + minTBS1;
    *maxTBS = maxTBS1 + maxTBS2;
 /* SPR 15446 Fix Start */
  if (dlSchedulableUECount_g == 1)
  {
      allocatedRB = allocateRbBasedOnQload(ueDLContext_p,availableRB,2);
  }
   /* SPR 15446 Fix End */
    return allocatedRB;
}
/* - coverity 55310 */

/*****************************************************************************
 * Function Name  : getTempTransmissionMode 
 * Inputs         : ueDLContext_p 
 *                  servingCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : tempTransmissionMode
 * Description    : Its main function is to return temp transmission mode for
 *                  the Downlink transmission.
 *****************************************************************************/
UInt32 getTempTransmissionMode( DLUEContext* ueDLContext_p, 
        ServingCellIndex servingCellIndex )
{
    UInt32 tempTransmissionMode = 0;
    if(IS_PCELL_OF_UE(ueDLContext_p, servingCellIndex))
    {
        tempTransmissionMode = (ueDLContext_p->dlMIMOInfo).transmissionMode;
    }
    else
    {
        tempTransmissionMode =
            (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
             ScelldlMIMOInfo).transmissionMode;
    }

    return tempTransmissionMode;
}

/*****************************************************************************
 * Function Name  : dciFormatSelector 
 * Inputs         : ueDLContext_p - UE who's DCI format is to be determined.
 *                  dlStrategyNewTxNode_p - This is the pointer of the 
 *                     structure of the node which need to be inserted into the 
 *                     PDCCH NewTXQueue_g or PDCCH ReTXQueue_g
 *                  txModeTBAndHarqStatus - On the basis of the combination of 
 *                     the Transmission mode,Harq Status and TB, DCI Format 
 *                     primary and secondary is being selected.
 *                  servingCellIndex - cell index used at MAC.
 * Outputs        : updates the primary and seconadary DCI Formats in the 
 *                  dlStrategyNewTxNode_p
 * Returns        : None
 * Description    : Its main function will be to select the Primary and 
                    secondary DCI format suitable for the Downlink transmission 
 *****************************************************************************/
void dciFormatSelector (
        DLUEContext* ueDLContext_p ,
        DLStrategyTxNode *dlStrategyTxNode_p,
        TxModeTBAndHarqStatus txModeTBAndHarqStatus,
        ServingCellIndex servingCellIndex)
{
    UInt32 tempTransmissionMode =
        getTempTransmissionMode( ueDLContext_p, servingCellIndex);

    switch ( txModeTBAndHarqStatus ) 
    {
        /*When UEs Harq Free and Transmssion mode Mode 3*/
        case HARQ_FREE_TX_3:
        {
            dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_2A ;
            dlStrategyTxNode_p->secondaryDCIFormat=DCI_FORMAT_1A;
            break;
        }
        /*When UEs Harq Free and Transmssion mode Mode 4*/
        case HARQ_FREE_TX_4:
        {
            dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_2 ;
            dlStrategyTxNode_p->secondaryDCIFormat=DCI_FORMAT_1A;
            break;
        }
        case HARQ_FREE_TX_GENERAL_NO_NACK_ZERO_QUEUE_LOAD:
        case HARQ_FREE_TX_3_4 :
        case HARQ_FREE_TX_3_4_8 :
        case HARQ_NOT_FREE_TX_3_4_ONE_NACK :
        case HARQ_NOT_FREE_TX_3_4_ONE_NACK_NON_ZERO_QUEUE_LOAD:
        case HARQ_NOT_FREE_TX_3_4_ONE_NACK_ZERO_QUEUE_LOAD:
        {
            if (tempTransmissionMode  == TX_MODE_3)
            {
                dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_2A ;
            }
            else if (tempTransmissionMode == TX_MODE_4 )           
            {
                dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_2 ;
            }
            else if (tempTransmissionMode == TX_MODE_8)
            {
                if((BF_ALGO_NOT_AVAILABLE == ueDLContext_p->beamFormingAlgoToBeUsed)&&
                        (BF_ALGO_NOT_AVAILABLE == 
                         (macReconfigSchedulerParamsDL_gp[servingCellIndex] + 
                          schParamsIndexDL_g[servingCellIndex])->bfVectorPresentFlag))
                {
                    dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_1A ;
                }
                else
                {
                    dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_2B ;
                }
            }
            /* SPR 6136 Fix Start */
            if (HARQ_NOT_FREE_TX_3_4_ONE_NACK_NON_ZERO_QUEUE_LOAD == txModeTBAndHarqStatus ||
                    HARQ_NOT_FREE_TX_3_4_ONE_NACK_ZERO_QUEUE_LOAD == txModeTBAndHarqStatus)
            {
                dlStrategyTxNode_p->secondaryDCIFormat= MAX_NUM_DCI_FORMAT;
            }
            else
            {
                /* SPR 6136 Fix End */
                dlStrategyTxNode_p->secondaryDCIFormat=DCI_FORMAT_1A;
            }
            break;
        }
        case HARQ_FREE_TX_OTHER_THAN_3_4:
        case HARQ_NOT_FREE_TX_OTHER_THAN_3_4:
        {
            /*CYCLOMATIC_CHANGES_6.1_START*/ 
            determineDCIFormatForTxOtherThanThreeFour(dlStrategyTxNode_p,
                                                      tempTransmissionMode);
            /*CYCLOMATIC_CHANGES_6.1_END*/
            break;
        }
        case HARQ_NOT_FREE_TX_3_4_TWO_NACK:
        case HARQ_NOT_FREE_TX_3_4_ONE_NACK_SECOND_INVALID_ZERO_QL :
        case HARQ_NOT_FREE_TX_3_4_ONE_NACK_SECOND_TA_DRX :
        {
            /*CYCLOMATIC_CHANGES_6.1_START*/
            determinePrimaryDCIFormatForTxThreeEight(dlStrategyTxNode_p,
                                                     tempTransmissionMode);
            /*CYCLOMATIC_CHANGES_6.1_END*/
            break;
        }
        default:
        lteWarning("[DL-Strategy][%s]DCI format request choice is INVALID",
                __func__);
        break;
    }
}

/*****************************************************************************
 * Function Name  : processZeroNonZeroTriggerQueue
 * Inputs         : ZeroNonZeroTriggerQueueCount - Total Count of the 
 *                     HARQZeroNonZeroTriggerQueue or 
 *                     dlUEZeroNonZeroQueueLoadTriggerQueue
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : updates the Non Zero Queue Load List
 * Returns        : None
 * Description    : It the Process the HARQZeroNonZeroQueueLoadTrigger Queue and 
 *                  Update the NonZeroQueueLoadAndHarq List,after adding and 
 *                  deleting UEs as per the request in the Triggers
 *****************************************************************************/
void processZeroNonZeroTriggerQueue(
        UInt32 ZeroNonZeroTriggerQueueCount,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    ZeroNonZeroTriggerNode * node_p        = PNULL;
    DLUEContext* ueDLContext_p             = PNULL;  
    UInt32 ueIndex;
    DLUEContextInfo * ueDLContextInfo_p    = PNULL;
    DRXContext      *drxContext_p = PNULL; /* DRX_CHG */  
    MeasGapContextDL  *measGapContext_p = PNULL; /* MEAS_GAP_CHG */

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Entry:[DL-Strategy][%s]",
            __func__);

    /*Processed for the number of  Queue Count*/
    do 
    {
        /* CirQ changes start */
	DEQUEUE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(ZeroNonZeroTriggerNode,(void **)&node_p,internalCellIndex); 
	/* CirQ changes end */

        /*Checking the Node for NULL*/
        if (node_p)
        {
            ueIndex = node_p->ueIdx;
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            /*Checking the UE Context & Checking for Pending delete Flag of UE*/
            if (!(ueDLContextInfo_p->pendingDeleteFlag))
            {
                ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

			/* EICIC +*/
			/* Pushing UE back in trigger queue.*/
			if ((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))
			{              
				if(!ENQUEUE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(ZeroNonZeroTriggerNode, 
							&(node_p->ZeroNonZeroTriggerNodeAnchor), internalCellIndex))
				{
					FREE_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p);
				}
			
				ZeroNonZeroTriggerQueueCount--;   
				continue;
			}

			else if ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType))
			{           
				if(!ENQUEUE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(ZeroNonZeroTriggerNode, 
							&(node_p->ZeroNonZeroTriggerNodeAnchor), internalCellIndex))
				{
					FREE_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p);
				}
			
				ZeroNonZeroTriggerQueueCount--;   
				continue;
			}

			/* EICIC - */


                /*+ Dyn UEs To Be SchPerTTI +*/
                if (ADD_TRIGGER == node_p->addDeleteTrigger)
                {
                    if (!ueDLContext_p->isUEConsideredForTotalActiveUeDL)
                    {
						/* + PERF_CA GBL */
                        totalActiveUEsDL_g[internalCellIndex]++;
                        ueDLContext_p->isUEConsideredForTotalActiveUeDL = TRUE;
                    }
                }
                else
                {
                    if(ueDLContext_p->isUEConsideredForTotalActiveUeDL)
                    {
                        totalActiveUEsDL_g[internalCellIndex]--;
						/* - PERF_CA GBL */
                        ueDLContext_p->isUEConsideredForTotalActiveUeDL = FALSE;
                    }
                }
                /*- Dyn UEs To Be SchPerTTI -*/
                /* DRX_CHG and MEAS_GAP_CHG */
                drxContext_p = &(ueDLContext_p->drxContext);
                measGapContext_p = &(ueDLContext_p->measGapContext);
                
                if ((DRX_STATE_ON != CHECK_UE_DRX_STATE(drxContext_p)) && 
                    (TRUE != CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p)) &&
                     (UL_SYNC_LOSS_OFF 
                         == ueDLContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState))
                {
                    /* DRX_CHG and MEAS_GAP_CHG */
                /*Trigger Node have flag to add the UE in Non Zero List*/
                if (ADD_TRIGGER == node_p->addDeleteTrigger )
                {
                    /* Verifying whether the UE is already present in Non Zero
                       List,if Already present, No Need to add UE in List*/
                    /* SPR 15652 Fix Start */
                    if (PNULL == ueDLContext_p->activeUENodeTempDeleted_p)
                    {
                    if (ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p)
                    {
                        LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
                                "[DL-Strategy][%s]UE is Already in "
                                "NonZeroQueueLoadAndHarqList",
                                __func__);
                    }
                    else
                    {
                        putEntryInUENonZeroQueueLoadAndHarqList(
                                ueIndex,ueDLContext_p,internalCellIndex);

                    }
                    }
                    /* SPR 15652 Fix End */
                }
                /*Trigger Node have flag is to delete the UE from Non Zero List*/
                else
                {
                    /*Verifying whether the UE is present in the Non Zero List,
                      if UE is not Present in the Non Zero List, then it cannot
                      be deleted from the non Zero List*/
                    if (ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p)
                    { 
                            if (ueDLContext_p->ueQueueLoad)
                            {
                                LTE_MAC_UT_LOG(LOG_DETAIL,
                                        DL_STRATEGY,
                                        "[DL-Strategy][%s]UE's Queue "
                                        "Load is [ %d ] Still NON ZERO Hence"
                                        " UE Cannot be deleted from"
                                        " NonZeroQueueLoadAndHarqList"
                                                " Non Zero List"
                                        ,__func__,
                                        ueDLContext_p->ueQueueLoad
                                        );
                                    }
                                    else
                                    {
                                        /* +- SPR 17777 */
                                        deleteEntryFromUENonZeroQueueLoadAndHarqList 
                                            (ueDLContext_p, internalCellIndex);
                                        /* +- SPR 17777 */
                        }                        
                    }
                    else
                    {
                        LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
                                "[DL-Strategy][%s]UE is NOT Present in "
                                "NonZeroQueueLoadAndHarqList, Hence Cannot"
                                "delete the Entry from Non Zero List",__func__);
                    }
                }
                }/* DRX_CHG and MEAS_GAP_CHG */
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
                        "[DL-Strategy][%s]UE Context is NULL or Pending to "
                        "delele\n",__func__);
            }
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
                    "[DL-Strategy][%s]dlHARQZeroNonZeroQueueLoadTriggerQueue"
                    "returned NULL\n",
                    __func__); 
            break;
        }
        /*Free the memory for this node_p.*/
        FREE_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p);
        ZeroNonZeroTriggerQueueCount--;   
    }while (ZeroNonZeroTriggerQueueCount);

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Exit:[DL-Strategy][%s]",
            __func__);
}

/*****************************************************************************
 * Function Name  : putEntryInUENonZeroQueueLoadAndHarqList
 * Inputs         : ueIndex - Index of UE which is to be added in the 
 *                     dlUENonZeroQueueLoadAndHarqList
 *                  ueDLContext_p - Pointer to the UE Context of the UE which 
 *                     is to be added in the dlUENonZeroQueueLoadAndHarqList
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : inserts the ueIndex in Non Zero List 
 *                     dlUENonZeroQueueLoadAndHarqList
 * Returns        : None
 * Description    : It inserts the UEs provided as argument into 
                    dlUENonZeroQueueLoadAndHarqList
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129193) */            
void putEntryInUENonZeroQueueLoadAndHarqList(
/* SPR 5599 changes end (ZIP ID 129193) */            
        UInt32 ueIndex,
        DLUEContext* ueDLContext_p
        ,InternalCellIndex internalCellIndex)
{
    /* SPR 16916 FIX START */
    /* +- */
    /* SPR 16916 FIX END */

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Entry:[DL-Strategy][%s]"
            ,__func__);
    NonZeroQueueLoadAndHarqNode * node_p = PNULL;

    /* SPR 2541 Changes Start 
     * This scenario will be taken care of during 'OFF' trigger 
     * handling of MeasGap/DRX and we don't need to add an entry
     * in UENonZeroQueueLoadAndHarqList right now. We can safely return 
     * from here. (ueDLContext_p->activeUENodeTempDeleted_p!=NULL)
     * implies that the node was deleted due to either MeasGap/DRX 'ON' trigger
     * and will be checked again during 'OFF' trigger processing*/
    if (ueDLContext_p->activeUENodeTempDeleted_p)
    {
        return;
    }
    /* SPR 2541 Changes End */
    GET_MEM_FROM_POOL(NonZeroQueueLoadAndHarqNode,node_p,
            sizeof(NonZeroQueueLoadAndHarqNode ),PNULL);

    /* + coverity 24541 */
    if (PNULL != node_p)
    {
        node_p->ueIndex = (UInt16)ueIndex;
        /*Adding the UE in the dlUENonZeroQueueLoadAndHarqList */
        listInsertNode((dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]),
                &(node_p->nodeAnchor));
        /*Storing the Pointer of the node in the 
          dlUENonZeroQueueLoadAndHarqList into the DL UE Context */
        ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p = 
            &(node_p->nodeAnchor);

        /*Setting the UE Priority Factor to Zero if its not Zero */
        if (ueDLContext_p->dlSchedulerAlgoInfo.uePriorityFactor)
        {
            ueDLContext_p->dlSchedulerAlgoInfo.uePriorityFactor = FALSE;
        }        
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "Exit[DL-Strategy][%s]Entry added for ueIndex %d in"
                " UE NON ZERO LIST\n"
                ,__func__,ueIndex);

        /* SPR 16916 FIX START */
        /* +- */
        /* SPR 16916 FIX END */

    }
    /* - coverity 24541 */
}



/*****************************************************************************
 * Function Name  : deleteEntryFromUENonZeroQueueLoadAndHarqList
 * Inputs         : ueIndex - Index of UE which is to be deleted from the 
 *                     dlUENonZeroQueueLoadAndHarqList
 *                  ueDLContext_p - Pointer to the UE Context of the UE which 
 *                     is to be deleted from the dlUENonZeroQueueLoadAndHarqList
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : Delete the UE from the Non Zero List
 * Description    : It deletes the UEs provided as argument from 
 *                  dlUENonZeroQueueLoadAndHarqList.It is also taken care if 
 *                  the Scheduling algorithm is Round Robin and the node to 
 *                  be deleted is the node for the pointer to round robin, 
 *                  then this node is also updated to the next node in the 
 *                  Non Zero List.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129193) */           
/* Review comment fix start MAC_RESET_8 */
void deleteEntryFromUENonZeroQueueLoadAndHarqList(
/* Review comment fix end MAC_RESET_8 */
/* SPR 5599 changes end (ZIP ID 129193) */            
        /* +- SPR 17777 */
        DLUEContext* ueDLContext_p
        ,InternalCellIndex internalCellIndex)
{
    LTE_LIST_NODE * tempNode_p = 
        ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p;
    LTE_LIST_NODE * tempNextNode_p = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "Entry:[DL-Strategy][%s]\n",__func__);

    /*if the Scheduling algorithm is Round Robin and the node to be deleted
      is the node for the pointer to round robin scheduler, then this node 
      is updated to the next node present in the Non Zero List.*/
    if ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
                initParams_p->dlSchdConfig.dlSchedulerStrategy == DL_ROUND_ROBIN_SCHEDULING)&&
            (tempNode_p ==  dlNonZeroListStartRoundRobin_p[internalCellIndex]))
    {    
        tempNextNode_p = getNextListNode(tempNode_p );
        if (!tempNextNode_p)
        {
            if (1 == listCount(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]))
            {
                dlNonZeroListStartRoundRobin_p[internalCellIndex] = PNULL;
            }
            else
            {         
                dlNonZeroListStartRoundRobin_p[internalCellIndex] = 
                    getListNode((dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]),0);
            }
        }
        else
        {
            dlNonZeroListStartRoundRobin_p[internalCellIndex] =  tempNextNode_p;
        }
    }
    /* 5858 changes start */
    if (listCount(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]))
    {
        /*The Node in the LIST will be delelted */
        listDeleteNode( (dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]), tempNode_p);
    }
    else
    {
        dlNonZeroListStartRoundRobin_p[internalCellIndex] = PNULL;
        /* SPR 9286 changes start */ 
        ueDLContext_p->activeUENodeTempDeleted_p = PNULL; 
        /* SPR 9286 changes end */ 
    }
    /* 5858 changes end */

    /*Free the Memory of the Node*/
    /* 5858 changes start */
    if (PNULL != tempNode_p)
    {
        freeMemPool(tempNode_p);
    }
    /* 5858 changes end */
    ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p = PNULL;
    /* SPR_8524_FIX_START */
    /* SPR 16916 FIX START */
    /* code removed */
    /* SPR 16916 FIX END */

    /* SPR_8524_FIX_END */

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "[DL-Strategy][%s]UE's entry is Deleted for ueIndex %d from UE NON"
            " ZERO LIST\n",__func__,ueDLContext_p->ueIndex);
    
    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "Exit[DL-Strategy][%s]\n", __func__);
}

/*****************************************************************************
 * Function Name  : deleteEntryFromUENonZeroQueueLoadAndHarqListTemporarily 
 * Inputs         : ueDLContext_p - Pointer to the UE Context of the UE which 
 *                     is to be deleted from the dlUENonZeroQueueLoadAndHarqList,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None 
 * Description    : It deletes the UEs itemporarily from 
 *                  dlUENonZeroQueueLoadAndHarqList.It is also taken care if 
 *                  the Scheduling algorithm is Round Robin and the node to 
 *                  be deleted is the node for the pointer to round robin, 
 *                  then this node is also updated to the next node in the 
 *                  Non Zero List.
 *                  Reasons (DRX/ Meas Gap/ ULSynch loss)
 *****************************************************************************/
void deleteEntryFromUENonZeroQueueLoadAndHarqListTemporarily(
        DLUEContext* ueDLContext_p,InternalCellIndex internalCellIndex)
{
    LTE_LIST_NODE * tempNode_p = 
        ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p;
    LTE_LIST_NODE * tempNextNode_p = PNULL;

    /*if the Scheduling algorithm is Round Robin and the node to be deleted
      is the node for the pointer to round robin scheduler, then this node 
      is updated to the next node present in the Non Zero List.*/
    if ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->
                dlSchdConfig.dlSchedulerStrategy == DL_ROUND_ROBIN_SCHEDULING)&&
            (tempNode_p ==  dlNonZeroListStartRoundRobin_p[internalCellIndex]))
    {    
        tempNextNode_p = getNextListNode(tempNode_p );
        if (!tempNextNode_p)
        {
            if (1 == listCount(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]))
            {
                dlNonZeroListStartRoundRobin_p[internalCellIndex] = PNULL;
            }
            else
            {         
                dlNonZeroListStartRoundRobin_p[internalCellIndex] = 
                    getListNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex],0);
            }
        }
        else
        {
            dlNonZeroListStartRoundRobin_p[internalCellIndex] =  tempNextNode_p;
        }
    }
    
    /* 5858 changes start */
    if (listCount(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]))
    {
        /*The Node in the LIST will be delelted */
        listDeleteNode( dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex], tempNode_p);
    }
    else
    {
        dlNonZeroListStartRoundRobin_p[internalCellIndex] = PNULL;
    }
/* SPR_5948_CHG_START */
/* SPR_5948_CHG_END */
   return;
}

/****************************************************************************
 * Function Name  : putEntryInZeroNonZeroQueueLoadTriggerQueue
 * Inputs         : ueContext_p - Pointer to the UE Context of the UE,
 *                  addDeleteTrigger- DELETE_TRIGGER/ADD_TRIGGER,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will creates the ZeroNonZeroQueueLoad Trigger queue
                    node and push node to ZeroNonZeroQueueLoadTriggerQueue
 ****************************************************************************/
void putEntryInZeroNonZeroQueueLoadTriggerQueue(
        DLUEContext * ueContext_p, 
        AddDeleteTrigger addDeleteTrigger,
        InternalCellIndex internalCellIndex)
{
    ZeroNonZeroTriggerNode *node_p = PNULL;
    
    /* CirQ changes start */
    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "Entry:[DL-Strategy][%s]Queue Count is %d \n",__func__,
            COUNT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q());
    /* CirQ changes end */

    GET_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(
                            node_p,ZeroNonZeroTriggerNode);

    /* + coverity 24542 */
    if (PNULL != node_p)
    {
        node_p->ueIdx  = ueContext_p->ueIndex;
        node_p->addDeleteTrigger = addDeleteTrigger;

        /* CirQ changes start */
        /*coverity id 25575*/
        if(!ENQUEUE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(ZeroNonZeroTriggerNode, 
					     &(node_p->ZeroNonZeroTriggerNodeAnchor),
                         internalCellIndex))
        {
            FREE_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p);
        }
        /* CirQ changes end */
        /* CirQ changes start */
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "Exit[DL-Strategy][%s]Queue Count is %d \n", __func__,
                COUNT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q());
        /* CirQ changes end */
    }
    /* - coverity 24542 */
}

/****************************************************************************
 * Function Name  : dlQosPutEntryInZeroNonZeroQueueLoadTriggerQueue
 * Inputs         : ueContext_p - Pointer to the UE Context of the UE,
 *                  addDelTrigLcNode_p- DELETE_TRIGGER/ADD_TRIGGER,
 *                  lcTrigCount - LC trig count, 
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will creates the ZeroNonZeroQueueLoad Trigger queue
                    node and push node to ZeroNonZeroQueueLoadTriggerQueue
 ****************************************************************************/
void dlQosPutEntryInZeroNonZeroQueueLoadTriggerQueue(DLUEContext * ueContext_p, 
        AddDelTrigLCNode *addDelTrigLcNode_p, UInt8 lcTrigCount
        ,InternalCellIndex internalCellIndex)
{
    ZeroNonZeroTriggerNode *node_p = PNULL;

    /* CirQ changes start */
    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "Entry:[DL-Strategy][%s]Queue Count is %d \n",__func__,
            COUNT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q());
    /* CirQ changes end */

    GET_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(
            node_p,ZeroNonZeroTriggerNode);

    /* + coverity 24531 */
    if (PNULL != node_p)
    {
        node_p->ueIdx  = ueContext_p->ueIndex;
        /* coverity-530 CID-32322*/ 
        /* In case of Qos this value is not read, 
         * rather the values inside the qosLcTrigger array are read
         */
        node_p->addDeleteTrigger = ADD_TRIGGER; 
        /* coverity-530 CID-32322*/ 
        node_p->triggerLcCount = lcTrigCount;


        while(lcTrigCount--)
        {
            node_p->qosLcTrigger[lcTrigCount] = addDelTrigLcNode_p[lcTrigCount];
        }

        /* CirQ changes start */
        /* + coverity 32497 */
        if (!ENQUEUE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(ZeroNonZeroTriggerNode,
                    &(node_p->ZeroNonZeroTriggerNodeAnchor),
                         internalCellIndex))
        {
            FREE_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p);
        }
        /* - coverity 32497 */

        /* CirQ changes end */

        /* CirQ changes start */
        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "Exit[DL-Strategy][%s]Queue Count is %d \n", __func__,
                COUNT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q());
        /* CirQ changes end */
    }
    /* - coverity 24531 */

}
/*****************************************************************************
 * Function Name: calculateNumLCWithNonZeroQueueLoad
 * Inputs       : ueDLContext_p - Pointer to UE Context
 * Outputs      : gives number of LC With Non Zero Queue Load
 * Returns      : numLCWithNonZeroQueueLoad- number of LC With Non Zero Queue
 *                Load
 * Description  : Its main function is to calculate the number of LC with Non 
 *                Zero Queue Load
 *****************************************************************************/

UInt32 calculateNumLCWithNonZeroQueueLoad (
             DLUEContext* ueDLContext_p )
{
    UInt32 numLCWithNonZeroQueueLoad=0;
    UInt32 j = MAX_NUMBER_OF_LOGICAL_CHANNEL;
    /*For all the Logical channels present*/
    while (j--)
    {
        /*For All the Valid Channels ID and For all the Logical channles with 
          queue Load, Count of Non Zero LC will be incremented*/
        if (ueDLContext_p->logicalChannel[j].queueLoad)
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,DL_STRATEGY,
                    "[scheduler]UE ID %u  lcpr %d * ql %u  \n",
                    ueDLContext_p->ueIndex,
                    ueDLContext_p->logicalChannel[j].logicalChannelPriority,
                    ueDLContext_p->logicalChannel[j].queueLoad
                    );
            numLCWithNonZeroQueueLoad++;   
        }
    }
    return numLCWithNonZeroQueueLoad;
}

/*****************************************************************************
 * Function Name  : initDLSchedulerStrategy
 * Inputs         : numOfCells - Number of available cell
 * Outputs        : initialize all the Queue and List Used by the DL Strategy
 * Returns        : None
 * Description    : This function initialize All queues and lists to be used 
 *                  by DL Scheduler Strategy.
 *****************************************************************************/
void initDLSchedulerStrategy ( UInt8 numOfCells )
{
    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "Entry[DL-Strategy][%s]\n", __func__);

    /* MEAS_GAP_CHG */
    CONSTRUCT_AND_INIT_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(numOfCells);
    /* MEAS_GAP_CHG */


    /* Creating Missing DLRetxQueueNode mempool */
    /* Assuming 10% of MAX_HARQ_RETX at any time. */
#ifdef TDD_CONFIG
    createMemPool(sizeof(DLRetxQueueNode),((MAX_DL_UE_SCHEDULED *MAX_HARQ_RETRANSMISSION_COUNT * DL_HARQ_RTT_TIMER_13_PLUS_4)/MAC_LTE_MEMORY_HARQ_RTX_FACT));
#else
    createMemPool(sizeof(DLRetxQueueNode),(numOfCells * (MAX_DL_UE_SCHEDULED *MAX_HARQ_RETRANSMISSION_COUNT * 17)/MAC_LTE_MEMORY_HARQ_RTX_FACT));
#endif
    createMemPool(sizeof(PFSGlobals),numOfCells);
    createMemPool(sizeof(DRX_Q_TYPE),numOfCells);
    createMemPool(sizeof(TA_Q_TYPE),numOfCells);
    createMemPool(sizeof(DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_TYPE),numOfCells);
    createMemPool(sizeof(LTE_LIST),numOfCells); /* dlUENonZeroQueueLoadAndHarqList_gp */
    createMemPool(sizeof(LTE_SQUEUE),numOfCells); /* dlCellEdgeUserPriorityQueue_gp */
    createMemPool(sizeof(DL_DRX_ON_OFF_TRIGGER_TYPE),numOfCells);
    createMemPool(sizeof(MACPriorityLcTree),numOfCells*MAX_VALID_UE_TYPE);
    createMemPool(sizeof(EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE),numOfCells); /*explicitDLPucchTpcScheduleQueue_gp */
    createMemPool(sizeof(EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE),numOfCells);/* explicitDLPucchTpcScheduleFailureQueue_gp */
    createMemPool(sizeof(LTE_CIRCQUEUE),numOfCells);
    createMemPool(sizeof(LTE_CIRCQUEUE),MAX_SPS_CIRC_Q * numOfCells);
    /* End 128 UE: Memory Changes */
    /* Cyclomatic Complexity changes start */
    initDLQueueNodesAndLists(numOfCells);
    /* Cyclomatic Complexity changes end */
    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "Exit[DL-Strategy][%s]\n", __func__);
}

/*****************************************************************************
 * Function Name  : initDLQueueNodesAndLists
 * Inputs         : numOfCells - number of available cell.
 * Outputs        : initialize all the Queue and List Used by the DL Strategy
 * Returns        : None
 * Description    : This function initialize All queues and lists to be used 
 *                  by DL Scheduler Strategy.
 *****************************************************************************/
STATIC void initDLQueueNodesAndLists ( UInt8 numOfCells )
{
    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "Entry[DL-Strategy][%s]\n", __func__);

    UInt8 cellCount = 0; 
    InitParams *initParams_p = PNULL;
    UInt8 ueTypeLoop = 0; /* EICIC +-*/
    for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {
        initParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[cellCount]->initParams_p;

        /* Start 128 UE: Memory Changes */
        /* 
         ** 128 UE Memory Changes: Replaced Macros which define the maximum UE or LC
         ** with their corresponding global variables.
         */
        /* Creating the mempool for Dl Qos Strategy Node */


        createMemPool(sizeof(DLQosStrategyLCNode),(initParams_p-> macMaxNumOfUeSupported * macParams_g.macMaxNumOfLcSupported));

        /* Creating the mempool for Dl Qos token update UE Node */
        createMemPool(sizeof(TknUpdateUENode),(initParams_p-> macMaxNumOfUeSupported));

        /*
         ** Moved memory allocation for TokenPeriodicityLisElem and TokenPeriodicityMap
         ** structures to initTokenPeriodicityArray because the number of blocks to be created
         ** depend on Token periodicity which comes in cell configuration parameters.
         */

        /* Creating the mempool for Zero Non  Zero Trigger  Node */
        createMemPool(sizeof(ZeroNonZeroTriggerNode),(initParams_p-> macMaxNumOfUeSupported));

        /* Creating the mempool for NonZeroQueueLoadAndHarq List Node */
        createMemPool(sizeof(NonZeroQueueLoadAndHarqNode),(initParams_p-> macMaxNumOfUeSupported));

        /* Creating the mempool for CEUserPriority Queue Node */
        createMemPool(sizeof(CEUserPriorityQueueNode),(initParams_p-> macMaxNumOfUeSupported));

        /* Initializing Pending DRX Queue */
        GET_MEM_FROM_POOL(DRX_Q_TYPE, pendingDRXQueue_gp[cellCount], 
                sizeof(DRX_Q_TYPE), PNULL);
        INIT_DRX_Q((pendingDRXQueue_gp[cellCount]));

        /* Initializing Pending TA Queue */
        GET_MEM_FROM_POOL(TA_Q_TYPE, pendingTAQueue_gp[cellCount], 
                sizeof(TA_Q_TYPE), PNULL);
        INIT_TA_Q(pendingTAQueue_gp[cellCount]);

        /* Initializing HARQ Zero <-> NonZero Trigger Queue*/

        /* Initializing  UE Zero <-> NonZero Trigger Queue */
        GET_MEM_FROM_POOL(DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_TYPE,
                dlUEZeroNonZeroQueueLoadTriggerQueue_gp[cellCount],
                sizeof(DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_TYPE),PNULL); 
        INIT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(cellCount,
                initParams_p-> macMaxNumOfUeSupported,
                macParams_g.macMaxNumOfLcSupported);

        /* Initializing Non Zero Queue Load and Harq List */
        GET_MEM_FROM_POOL(LTE_LIST, \
                dlUENonZeroQueueLoadAndHarqList_gp[cellCount], \
                sizeof(LTE_LIST), PNULL);
        listInit(dlUENonZeroQueueLoadAndHarqList_gp[cellCount]);

        /* Initializing pending Cell edge UE's List */
        GET_MEM_FROM_POOL(LTE_SQUEUE, \
                dlCellEdgeUserPriorityQueue_gp[cellCount], \
                sizeof(LTE_SQUEUE), PNULL);
        sQueueInit(dlCellEdgeUserPriorityQueue_gp[cellCount]);

        GET_MEM_FROM_POOL(DL_DRX_ON_OFF_TRIGGER_TYPE, dlDRXOnOffTriggerQueue_gp[cellCount], \
                sizeof(DL_DRX_ON_OFF_TRIGGER_TYPE), PNULL); 
        INIT_DL_DRX_ON_OFF_TRIGGER_Q(cellCount);

        /* EICIC +*/
        for (ueTypeLoop = 0; ueTypeLoop < MAX_VALID_UE_TYPE ; ueTypeLoop++)
        {
        GET_MEM_FROM_POOL(MACPriorityLcTree, dlPriorityLcTree_gp[cellCount][ueTypeLoop], \
                sizeof(MACPriorityLcTree), PNULL);
        dlQosInitLcTree(dlPriorityLcTree_gp[cellCount][ueTypeLoop],&dlQosCompareLcNode, &dlQosKeyOfLcNode);
        }
	/* EICIC -*/
        GET_MEM_FROM_POOL(EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE, explicitDLPucchTpcScheduleQueue_gp[cellCount], \
                sizeof(EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE), PNULL);
        INIT_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleQueue_gp[cellCount]);

        GET_MEM_FROM_POOL(EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE, explicitDLPucchTpcScheduleFailureQueue_gp[cellCount], \
                sizeof(EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE), PNULL);
        INIT_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleFailureQueue_gp[cellCount]);

        /* SRP 3608 changes start */
        /* SPR 6897 changes start */
        GET_MEM_FROM_POOL(LTE_CIRCQUEUE, dlPriorityQueue_gp[cellCount], \
                sizeof(LTE_CIRCQUEUE), PNULL);
        circQInit(dlPriorityQueue_gp[cellCount], MAX_NODES);
        /* SPR 6897 changes end */
        /* SPR 3608 changes end */

        /* Initialize CA queues */
        initDlCAQueue( cellCount );

        if(DL_FAIR_SCHEDULING == (cellSpecificParams_g.cellConfigAndInitParams_p[cellCount]->\
                    initParams_p->dlSchdConfig.dlSchedulerStrategy))
        {
            GET_MEM_FROM_POOL(PFSGlobals,pfsGlobals_gp[cellCount], \
                    sizeof(PFSGlobals), PNULL);
            INIT_PFS_GLOBALS(pfsGlobals_gp[cellCount]);
            dlPfsInitMacSchedulerData(cellCount);
        }
        initDlCAScellStateEventArr();
        
        /* Initialize SPS queues */
        initializeDlSPSQueues(cellCount);
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "Exit[DL-Strategy][%s]\n", __func__);
}

/*****************************************************************************
 * Function Name  : initDlCAQueue
 * Inputs         : cellCount -  number of available cell.
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialize all the CA queues if CA is 
 *                  enabled.
 *****************************************************************************/
void initDlCAQueue( UInt8 cellCount )
{
    if(IS_CA_ENABLED())
    {    
        /*spr 11102 fix start*/ 
        GET_MEM_NODE_DL_CA_EVENT_Q( dlCAEventQueue_gp[cellCount], 
                DL_CA_EVENT_QUEUE_TYPE );
        INIT_DL_CA_EVENT_Q(dlCAEventQueue_gp[cellCount]);

        GET_MEM_NODE_DL_CA_MAC_CE_Q( dlCAMacCEQueue_gp[cellCount],
                DL_CA_MAC_CE_QUEUE_TYPE );
        INIT_DL_CA_MAC_CE_Q(dlCAMacCEQueue_gp[cellCount]);

        GET_MEM_NODE_DL_CA_MAC_CE_Q( pendingDlCAMacCEQueue_gp[cellCount],
                DL_CA_MAC_CE_QUEUE_TYPE );
        INIT_DL_CA_MAC_CE_Q(pendingDlCAMacCEQueue_gp[cellCount]);

        GET_MEM_NODE_DL_CA_MAC_CE_Q( 
                pendingFailureDlCAMacCEQueue_gp[cellCount],
                DL_CA_MAC_CE_QUEUE_TYPE );
        INIT_DL_CA_MAC_CE_Q(pendingFailureDlCAMacCEQueue_gp[cellCount]);

        GET_MEM_NODE_FREE_SCELL_HARQ_Q( 
                dlCAMacFreeScellHarqBufferQueue_gp[cellCount], 
                DL_CA_MAC_FREE_SCELL_HARQ_QUEUE_TYPE );
        INIT_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp[cellCount]);
        /*spr 11102 fix end*/ 
    }
}

/*****************************************************************************
 * Function Name  : initializeDlSPSQueues
 * Inputs         : cellCount -  number of available cell.
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialize all the SPS Queues used by the SPS
 *                  for DL Scheduler Strategy.
 *****************************************************************************/
STATIC void initializeDlSPSQueues( UInt8 cellCount )
{
    /*! \code
     * SPS Queues initialized which are used by SPS strategy: (dlSpsStatusQ_g,
     * dlSpsActQ_g, dlSpsDeactQ_g, dlSpsReactQ_g, dlSpsUeExclude_g,
     * dlSpsResRsvQ_g, dlSpsResRelQ_g)
     * \endcode
     * */
    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, dlSpsStatusQ_gp[cellCount], \
            sizeof(LTE_CIRCQUEUE), PNULL);
    QINIT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[cellCount]);

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, dlSpsActQ_gp[cellCount], \
            sizeof(LTE_CIRCQUEUE), PNULL);
    QINIT_MAC_DL_SPS_Q( dlSpsActQ_gp[cellCount] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, dlSpsDeactQ_gp[cellCount], \
            sizeof(LTE_CIRCQUEUE), PNULL);
    QINIT_MAC_DL_SPS_Q( dlSpsDeactQ_gp[cellCount] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, dlSpsReactQ_gp[cellCount], \
            sizeof(LTE_CIRCQUEUE), PNULL);
    QINIT_MAC_DL_SPS_Q( dlSpsReactQ_gp[cellCount] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, dlSpsUeExclude_gp[cellCount], \
            sizeof(LTE_CIRCQUEUE), PNULL);
    QINIT_MAC_DL_SPS_Q( dlSpsUeExclude_gp[cellCount] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, dlSpsResRsvQ_gp[cellCount], \
            sizeof(LTE_CIRCQUEUE), PNULL);
    QINIT_MAC_DL_SPS_Q( dlSpsResRsvQ_gp[cellCount] );

    GET_MEM_FROM_POOL(LTE_CIRCQUEUE, dlSpsResRelQ_gp[cellCount], \
            sizeof(LTE_CIRCQUEUE), PNULL);
    QINIT_MAC_DL_SPS_Q( dlSpsResRelQ_gp[cellCount] );
}

/****************************************************************************
* Function Name  : deInitDLStrategyQueues
* Inputs         : internalCellIndex - cell index used at MAC
* Outputs        : None
* Returns        : None
* Variables      :
* Description    : This API deInit the Circular queues created at Mac Init
****************************************************************************/
/* SPR 8702 changes start */
void deInitDLSchedulerStrategyQueues(InternalCellIndex internalCellIndex)
{
    /*+COVERITY 5.3.0 - 54659*/
    QDEINIT_NODE_DRX_Q((pendingDRXQueue_gp[internalCellIndex]));
    QDEINIT_NODE_TA_Q(pendingTAQueue_gp[internalCellIndex]);
    QDEINIT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(dlUEZeroNonZeroQueueLoadTriggerQueue_gp[internalCellIndex]);
    QDEINIT_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex]); 
    QDEINIT_MAC_DL_SPS_Q(dlSpsDeactQ_gp[internalCellIndex]); 
    QDEINIT_MAC_DL_SPS_Q(dlSpsActQ_gp[internalCellIndex]); 
    QDEINIT_MAC_DL_SPS_Q(dlSpsReactQ_gp[internalCellIndex]); 
    QDEINIT_MAC_DL_SPS_Q(dlSpsUeExclude_gp[internalCellIndex]); 
    QDEINIT_MAC_DL_SPS_Q(dlSpsResRsvQ_gp[internalCellIndex]); 
    QDEINIT_MAC_DL_SPS_Q(dlSpsResRelQ_gp[internalCellIndex]); 
    /*ca chngs start*/
    QDEINIT_DL_CA_MAC_CE_Q(internalCellIndex);
    QDEINIT_DL_CA_EVENT_Q(internalCellIndex);
    QDEINIT_DL_CA_PEND_MAC_CE_Q(internalCellIndex);
    /*phase-3 change*/
    QDEINIT_FREE_SCELL_HARQ_Q(internalCellIndex);
    /*spr 11102 fix */
    QDEINIT_DL_CA_PEND_FAIL_MAC_CE_Q(internalCellIndex);
    /*ca chngs end*/
    /*-COVERITY 5.3.0 - 54659*/

    if (pendingDRXQueue_gp[internalCellIndex])
    {
        freeMemPool(pendingDRXQueue_gp[internalCellIndex]);
        pendingDRXQueue_gp[internalCellIndex] = PNULL;
    }

    if (pendingTAQueue_gp[internalCellIndex])
    {
        freeMemPool(pendingTAQueue_gp[internalCellIndex]);
        pendingTAQueue_gp[internalCellIndex] = PNULL;
    }

    if (dlUEZeroNonZeroQueueLoadTriggerQueue_gp[internalCellIndex])
    {
        freeMemPool(dlUEZeroNonZeroQueueLoadTriggerQueue_gp[internalCellIndex]);
        dlUEZeroNonZeroQueueLoadTriggerQueue_gp[internalCellIndex] = PNULL;
    }

    if (dlSpsStatusQ_gp[internalCellIndex])
    {
        freeMemPool(dlSpsStatusQ_gp[internalCellIndex]);
        dlSpsStatusQ_gp[internalCellIndex] = PNULL;
    }

    if (dlSpsDeactQ_gp[internalCellIndex])
    {
        freeMemPool(dlSpsDeactQ_gp[internalCellIndex]);
        dlSpsDeactQ_gp[internalCellIndex] = PNULL;
    }

    if (dlSpsActQ_gp[internalCellIndex])
    {
        freeMemPool(dlSpsActQ_gp[internalCellIndex]);
        dlSpsActQ_gp[internalCellIndex] = PNULL;
    }

    if (dlSpsReactQ_gp[internalCellIndex])
    {
        freeMemPool(dlSpsReactQ_gp[internalCellIndex]);
        dlSpsReactQ_gp[internalCellIndex] = PNULL;
    }

    if (dlSpsUeExclude_gp[internalCellIndex])
    {
        freeMemPool(dlSpsUeExclude_gp[internalCellIndex]);
        dlSpsUeExclude_gp[internalCellIndex] = PNULL;
    }

    if (dlSpsResRsvQ_gp[internalCellIndex])
    {
        freeMemPool(dlSpsResRsvQ_gp[internalCellIndex]);
        dlSpsResRsvQ_gp[internalCellIndex] = PNULL;
    }

    if (dlSpsResRelQ_gp[internalCellIndex])
    {
        freeMemPool(dlSpsResRelQ_gp[internalCellIndex]);
        dlSpsResRelQ_gp[internalCellIndex] = PNULL;
    }

    if (dlCAMacCEQueue_gp[internalCellIndex])
    {
        freeMemPool(dlCAMacCEQueue_gp[internalCellIndex]);
        dlCAMacCEQueue_gp[internalCellIndex] = PNULL;
    }

    if (dlCAEventQueue_gp[internalCellIndex])
    {
        freeMemPool(dlCAEventQueue_gp[internalCellIndex]);
        dlCAEventQueue_gp[internalCellIndex] = PNULL;
    }

    if (pendingDlCAMacCEQueue_gp[internalCellIndex])
    {
        freeMemPool(pendingDlCAMacCEQueue_gp[internalCellIndex]);
        pendingDlCAMacCEQueue_gp[internalCellIndex] = PNULL;
    }
    /*spr 11102 fix*/
    if (pendingFailureDlCAMacCEQueue_gp[internalCellIndex])
    {
        freeMemPool(pendingFailureDlCAMacCEQueue_gp[internalCellIndex]);
        pendingFailureDlCAMacCEQueue_gp[internalCellIndex] = PNULL;
    }     
}
/* SPR 8702 changes end */

/****************************************************************************
* Function Name  : cleanupDLSchedulerStrategy
* Inputs         : internalCellIndex - cell index used at MAC.
* Outputs        : None
* Returns        : None
* Description    : This API cleanup the cleanup DLSchedulerStrategy
****************************************************************************/
void cleanupDLSchedulerStrategy(InternalCellIndex internalCellIndex)
{
    void * dlSpsNode_p = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
            "Entry[DL-Strategy][%s]\n", __func__);

    /* Cyclomatic_complexity_changes_start */    
    cleanupDlCAEventQueue(internalCellIndex);

    cleanupDlCAMacCEQueue(internalCellIndex);

    cleanupPendingDlCAMacCEQueue(internalCellIndex);

    cleanupPendingFailureDlCAMacCEQueue(internalCellIndex);

    cleanupDlCAMacFreeScellHarqBufferQueue(internalCellIndex);

    cleanupMeasGapOnOffTriggerQueue(internalCellIndex);

    cleanupPendingDRXQueue(internalCellIndex);

    cleanupPendingTAQueue(internalCellIndex);

    cleanupDlUeZeroNonZeroQueueLoadTriggrQueue(internalCellIndex);

    cleanupDlUENonZeroQueueLoadAndHarqList(internalCellIndex);

    /* SPR 4834 Fix Start */
    cleanupDlCellEdgeUserPriorityQueue(internalCellIndex);

    cleanupDRXOnOffTriggerQueue(internalCellIndex);

    cleanupAdvanceAllocInfoQueue(internalCellIndex);
    /* SPR 4834 Fix End */
    /* Cyclomatic_complexity_changes_end */

    dlNonZeroListStartRoundRobin_p[internalCellIndex] = PNULL;
    /* Cyclomatic_complexity_changes_start */
    CleanupSPSStrategyQueues(internalCellIndex, dlSpsNode_p);
   /* Cyclomatic_complexity_changes_end */
}

/*****************************************************************************
 * Function Name : processDummyDlSchedulerStrategy
 * Inputs        : currentGlobalTTITickCount : current Global Tick
 *                   dlDelay  - The amount of delay expected in the DownLink path
 *                  internalCellIndex - cell index used at MAC.
 * Outputs       : DLStrategyTxNode is inserted into the PDCCH New and ReTx
 *                  Queues Which  will be used by the PDCCH  for CCE distribution
 * Outputs       : None
 * Returns       : MAC_SUCCESS
 * Description   : This funtion is called when DL startegy is not to be done
 *                 during the special subframe for special subframe
 *                 configurations 0,1,5,6
 *****************************************************************************/
                                    /* SPR 15909 fix start */
UInt32 processDummyDlSchedulerStrategy ( tickType_t currentGlobalTTITickCount,
                                    /* SPR 15909 fix end */
                          UInt32 dlDelay, InternalCellIndex internalCellIndex)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(currentGlobalTTITickCount)
  LTE_GCC_UNUSED_PARAM(dlDelay)
  LTE_GCC_UNUSED_PARAM(internalCellIndex)
    /* +- SPR 17777 */

/* SPR 609 changes start */
    return MAC_SUCCESS;
/* SPR 609 changes end */    
}

/*****************************************************************************
 * Function Name  : putEntryInExplicitDLDCIScheduleQueue
 * Inputs         : node_p : Pointer to the node,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None 
 * Returns        : TRUE/FALSE
 * Description    : Used to put entry into explicitDLPucchTpcScheduleQueue_g
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129193) */            
UInt8 putEntryInExplicitDLDCIScheduleQueue(UInt16 ueIdx, InternalCellIndex internalCellIndex)
/* SPR 5599 changes end (ZIP ID 129193) */            
{
    ExplicitDLDCIQueueNode * dlDciNode_p = PNULL;
    if (explicitTpcTriggerEnabled_g[internalCellIndex] == TRUE)
    {
        GET_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(dlDciNode_p,ExplicitDLDCIQueueNode);

        /* + coverity 24540 */
        if (PNULL == dlDciNode_p)
        {
            return 1;
        }
        /* - coverity 24540 */

        dlDciNode_p->ueIndex = ueIdx; 
        UInt32 retVal = 0;
        retVal = ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleQueue_gp[internalCellIndex],
        ExplicitDLDCIQueueNode,&(dlDciNode_p->nodeAnchor));

        /* + coverity 32506 */
            /* SPR 23483 Changes Start */
        if (CIRC_FAILURE == retVal)
            /* SPR 23483 Changes End */
        {
            FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(dlDciNode_p);
            return 1;
        }
        /* - coverity 32506 */

        return (!retVal);
    }
    else
    {
        return 1;
    }
}

/*****************************************************************************
 * Function Name  : processExplicitDLDCIScheduleQueue
 * Inputs         : explicitDLDCIScheduleQueueCount - This is Count of the 
 *                     total nodes present in the explicitDLDCIScheduleQueue_p 
 *                  numberOfAvailableRBs_p - pointer to current available RBs.
 *                  currentGlobalTTITickCount - Current Global Tick
 *                  dlScheduleData_p - Pointer to DL Scheduler Data Element
 *                  explicitDLDCIScheduleQueue_p - Pointer to DL DCI Queue
 *                  numUEScheduled_p - Number of UE's to be scheduled
 *                  maxUEsToBeScheduledDL_p - max UE's can be scheduled in DL
 *                  dlDelay - Number of ticks DL is getting processed in Advance
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : It will allocate RBs to explicit DL DCI  Nodes &
 *                  place the DL Strategy O/P either in the 
 *                  explicitDLPucchTpcScheduleQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the iexplicit DL DCI
 *                  Queues Nodes. Allocate RBs to the UEs with non
 *                  Zero Queue Load otherwise add them in scheduler Array.
 *****************************************************************************/
void processExplicitDLDCIScheduleQueue (
        UInt32 explicitDLDCIScheduleQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        DLSchedulerDataElement * dlScheduleData_p,
        EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE * explicitDLDCIScheduleQueue_p,
        UInt16 * numUEScheduled_p,
        UInt8 * maxUEsToBeScheduledDL_p
        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
        ,UInt8 dlDelay
#endif
        /*HD FDD Changes End*/
        , InternalCellIndex internalCellIndex
        ) 
{
    DLUEContext *ueDLContext_p          = PNULL;
    /* DLHARQEntity *ueharqDLContext_p     = PNULL ; TDD Warning Fix */
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    DLHARQProcess *harqProcess_p = PNULL;
    UInt32 ueIndex;
    ExplicitDLDCIQueueNode *node_p      = PNULL;
    DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p =
        dlScheduleData_p->dlSchedulerDataNodeArr;
    /* ICIC Change start */
    UInt8 userLocationType = CC_USER ;
    /* ICIC Change End */

    /* EICIC +*/
     ExplicitDLDCIQueueNode *nodeScan_p      = PNULL;
     UInt8	isTraverseList = LTE_FALSE;
      /* EICIC -*/

    UInt32 totalUE = dlScheduleData_p->dataNodeArrTotalUE;
    UInt8 maxUEsToBeScheduledDL        = *maxUEsToBeScheduledDL_p;
    DRXContext          *drxContext_p = PNULL;
    MeasGapContextDL      *measGapContext_p = PNULL;
    DLStrategyTxNode *dlStrategyTxNode_p = PNULL;


#ifdef TDD_CONFIG
    UInt8 subFrame = currentGlobalTTITickCount % MAX_SUBFRAME;
#endif

    /* ICIC changes start */
    while ((explicitDLDCIScheduleQueueCount)&&(numberOfAvailableRBs_p[CC_CE_REGION])
            && maxUEsToBeScheduledDL)
        /* ICIC changes end */
    {

        DEQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLDCIScheduleQueue_p,
                ExplicitDLDCIQueueNode,(void ** )&node_p);
        if (node_p)
        {
            /* A valid node */
            ueIndex = node_p->ueIndex;
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            if (!ueDLContextInfo_p->pendingDeleteFlag)
            {
                ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
		    /*EICIC +*/
/*Complexity Start*/
		    if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))
			||((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
		    {              
			    if(!ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLDCIScheduleQueue_p,
						    ExplicitDLDCIQueueNode,&(node_p->nodeAnchor)))
			    {
				    FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
			    }
			    isTraverseList = LTE_TRUE;
			    explicitDLDCIScheduleQueueCount--;
			    continue;
		    }
/*Complexity End*/
		    /* EICIC -*/
                /* ICIC Change start */
                userLocationType = ueDLContext_p->userLocationType ;
                /*Checking if Already being considered for RB Distribution*/
                if (ueDLContext_p->isAlreadyConsideredFlag != dlReferenceFlag_g[internalCellIndex])
                {
                    drxContext_p = &ueDLContext_p->drxContext;
                    measGapContext_p = &ueDLContext_p->measGapContext;
                    if ((DRX_STATE_ON == CHECK_UE_DRX_STATE(drxContext_p) ||
                                TRUE == CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p)) 
                            /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                            /* If HD FDD UE is in DL DCI Q,then check Schedule map whether
                             * it can be scheduled in currenttick or not*/
                            ||
                            ((TRUE == ueDLContext_p->isHDFddFlag) &&
                             CAN_SCHEDULE_DL_FOR_HD_UE(currentGlobalTTITickCount+dlDelay, \
                                 ueIndex, hdUeContextInfo_g.hdfddUeSchedMap))
#endif
                            /*HD FDD Changes End */                     
                       )                      
                    {
                        // Push UE into explicitDLPucchTpcScheduleFailureQueue_g
                        // if not scheduled
                        UInt32 retVal = 0;
                        retVal = ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],
                                ExplicitDLDCIQueueNode,&(node_p->nodeAnchor));
                        if(!retVal)
                        {
                            LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_DL_LOG_ID, LOGDEBUG,
                                    MAC_DL_Strategy, currentGlobalTTITickCount,
                                    ueIndex, CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                    __func__,"PUSH_NODE_IN_EXPLICIT_DL_DCI_FAIL_Q");

                            FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
                        }

                        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                        if(TRUE == ueDLContext_p->isHDFddFlag)
                        {
                            /* Increment the statistics counter UE denied DL resources due to
                             * UL clash in the same TTI of HD FDD UE*/
                            UPDATE_UE_DENIED_DL_RESOURCE(ueIndex);
                        }
#endif
                        /*HD FDD Changes End */                     
                        explicitDLDCIScheduleQueueCount--;
                        continue;
                    }
                    /* \code
                     * if schType == SPS_SCHEDULING, then do not schedule
                     * this UE.
                     * */
                    if( SPS_SCHEDULING == ueDLContext_p->schType )
                    {
                        UInt32 retVal = 0;
                        retVal = ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],
                                ExplicitDLDCIQueueNode,&(node_p->nodeAnchor));
                        if(!retVal)
                        {
                            LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_DL_LOG_ID, LOGDEBUG,
                                    MAC_DL_Strategy, currentGlobalTTITickCount,
                                    ueIndex, CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                    __func__,"PUSH_NODE_IN_EXPLICIT_DL_DCI_FAIL_Q");

                            FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
                        }
                        explicitDLDCIScheduleQueueCount--;
                        continue;
                    }
                    /* ICIC Change Start */
                    if ((CE_USER == userLocationType) && ((!maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex])
                                ||  (!numberOfAvailableRBs_p[CE_REGION])))

                    {
                        UInt32 retVal = 0;
                        retVal = ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],
                                ExplicitDLDCIQueueNode,&(node_p->nodeAnchor));
                        if(!retVal)
                        {
                            LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_DL_LOG_ID, LOGDEBUG,
                                    MAC_DL_Strategy, currentGlobalTTITickCount,
                                    ueIndex, CHECK_UE_DRX_STATE(drxContext_p),
                                    CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                    __func__,"PUSH_NODE_IN_EXPLICIT_DL_DCI_FAIL_Q");

                            FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
                        }

                        explicitDLDCIScheduleQueueCount--;
                        continue;
                    }

                    /* ICIC Change End */

                    /*  ueharqDLContext_p = ueDLContext_p->dlHarqContext_p; 
                        TDD  warning Fix */
                    /* Check if queueload is more than 0 */
                    /*Soft Buffer Changes Start*/
                    if (( NET_UE_QL(ueDLContext_p) > ZERO)&&
                            (ueDLContext_p->permissibleTBSize >= HARQ_TB_MINIMUM_SIZE_IN_BYTES))
                        /*Soft Buffer Changes End*/
                    {

                        if (PNULL == (harqProcess_p = checkForFreeHarqProcess(
                                        ueDLContext_p,
                                        ueDLContext_p->dlHarqContext_p, 
                                        DRA_SCHEDULING, internalCellIndex)))
                        {
                            FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
                            explicitDLDCIScheduleQueueCount--;
                            continue;
                        }

                        tempDLSchedulerDataNodeArr_p[totalUE].harqProcessId =
                            harqProcess_p->harqProcessId;
                        tempDLSchedulerDataNodeArr_p[totalUE].ueDLContext_p=
                            ueDLContext_p;

                        tempDLSchedulerDataNodeArr_p[totalUE].msgType = TPC_WITH_DATA;
                        totalUE++;
                        ueDLContext_p->isAlreadyConsideredFlag = 
                            dlReferenceFlag_g[internalCellIndex];
                        (*numUEScheduled_p)++;
                        alreadyScheduledUEs[internalCellIndex]++; 
                        maxUEsToBeScheduledDL--;
                        if(userLocationType == CE_USER)
                        {
                            maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;
                        }
                        /* + COVERITY 5.0 */
                        LOG_MAC_MSG (DL_EXPLICIT_DCI_SCHEDULED_LOG_ID, LOGDEBUG,
                                MAC_DL_Strategy, getCurrentTick (), ueIndex,
                                harqProcess_p->harqProcessId,
                                harqProcess_p->assignedResourcesNB,
                                ueDLContext_p->ueQueueLoad,
                                /* SPR 14845 FIX START */
                                tempDLSchedulerDataNodeArr_p[totalUE -1].msgType, 
                                /* SPR 14845 FIX END */
                                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
                        /* - COVERITY 5.0 */

                    }
                    else
                    {
                        /*Soft Buffer Changes Start*/
                        /*4X4 DL MIMO CHG START*/
                        UInt32 minTBSizeToSendTPC = determineTBSize(
                                ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                MIN_RB_FOR_TPC_ONLY,TB_MAPPED_TO_ONE_LAYER);
                        /*4X4 DL MIMO CHG END*/
                        if(ueDLContext_p->permissibleTBSize < minTBSizeToSendTPC)
                        {
                            // Push UE into explicitDLPucchTpcScheduleFailureQueue_g
                            // if not scheduled
                            if(!ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],
                                        ExplicitDLDCIQueueNode,&(node_p->nodeAnchor)))
                            {
                                LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_DL_LOG_ID, LOGDEBUG,
                                        MAC_DL_Strategy, currentGlobalTTITickCount,
                                        ueIndex, CHECK_UE_DRX_STATE(drxContext_p),
                                        CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                        __func__,"PUSH_NODE_IN_EXPLICIT_DL_DCI_FAIL_Q");
                                FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
                            }
                            explicitDLDCIScheduleQueueCount--;
                            continue;
                        }
                        /*Soft Buffer Changes End*/
                        /* Finding Free HarQ process to process TA*/
                        if (PNULL == (harqProcess_p = checkForFreeHarqProcess(
                                        ueDLContext_p, ueDLContext_p->dlHarqContext_p,
                                        DRA_SCHEDULING, internalCellIndex ) ))
                        {
                            FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
                            explicitDLDCIScheduleQueueCount--;
                            continue;
                        }
                        else
                        {
                            /*Preparing the DL Strategy Output Node */
                            GET_MEM_FROM_POOL (DLStrategyTxNode, dlStrategyTxNode_p,
                                    sizeof (DLStrategyTxNode), PNULL);
                            /* + coverity 24536 */
                            if (PNULL != dlStrategyTxNode_p)
                            {
                                ResourceAllocatorInput* tempResourceAllocatorInput_p =
                                    &(dlStrategyTxNode_p->resourceAllocInput) ;
                                tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
                                tempResourceAllocatorInput_p->strictAllocSubBand = FALSE;

                                /*If Free HarQ Process Found*/

                                if ((TX_MODE_3 == (ueDLContext_p->dlMIMOInfo).transmissionMode) ||
                                        (TX_MODE_4 == (ueDLContext_p->dlMIMOInfo).transmissionMode))
                                {
                                    dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_1A;
                                    dlStrategyTxNode_p->secondaryDCIFormat = DCI_FORMAT_1A;
                                }
                                else
                                {
                                    dciFormatSelector ( ueDLContext_p, dlStrategyTxNode_p,
                                            HARQ_FREE_TX_OTHER_THAN_3_4,internalCellIndex);
                                }

                                dlStrategyTxNode_p->msgType = TPC_WITHOUT_DATA;
                                dlStrategyTxNode_p->ueIndex = ueIndex;
                                dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId;
                                /*+ SPR 22400 Changes */
                                dlStrategyTxNode_p->harqProcess_p = harqProcess_p;
                                /*- SPR 22400 Changes */
                                tempResourceAllocatorInput_p->requiredRB = MIN_RB_FOR_TPC_ONLY;
                                harqProcess_p->isTBOneValid = TRUE;
                                harqProcess_p->isTBTwoValid = FALSE;
                                harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;
                                /* ICIC changes start */
                                numberOfAvailableRBs_p[userLocationType] -=
                                    tempResourceAllocatorInput_p->requiredRB;
                                if (userLocationType == CC_USER)
                                {
                                    if (numberOfAvailableRBs_p[CC_CE_REGION] < 
                                            numberOfAvailableRBs_p[CE_REGION])
                                    { 
                                        numberOfAvailableRBs_p[CE_REGION] =
                                            numberOfAvailableRBs_p[CC_CE_REGION]; 
                                    }
                                }
                                else
                                {
                                    numberOfAvailableRBs_p[CC_CE_REGION] -=
                                        tempResourceAllocatorInput_p->requiredRB;
                                }
                                dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated = 
                                    userLocationType;
                                /* ICIC changes end */
                                harqProcess_p->assignedResourcesNB =
                                    (tempResourceAllocatorInput_p->requiredRB);

                                harqProcess_p->dlHARQTBOneInfo.mcsIndex  =
                                    ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;

#ifdef FDD_CONFIG
                            /*4X4 DL MIMO CHG START*/
                            harqProcess_p->dlHARQTBOneInfo.tbSize  =  determineTBSize(
                                    harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                    tempResourceAllocatorInput_p->requiredRB,TB_MAPPED_TO_ONE_LAYER);
#elif TDD_CONFIG
                            if (checkForSpecialSubframe(subFrame, internalCellIndex))
                            {
                                harqProcess_p->dlHARQTBOneInfo.tbSize  =  determineTBSize(
                                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                        SP_SUBFRAME_PRB(tempResourceAllocatorInput_p->requiredRB),
                                        /*4X4 DL MIMO CHG START*/
                                        TB_MAPPED_TO_ONE_LAYER
                                        /*4X4 DL MIMO CHG END*/);
                            }
                            else
                            {
                                harqProcess_p->dlHARQTBOneInfo.tbSize  =  determineTBSize(
                                        harqProcess_p->dlHARQTBOneInfo.mcsIndex,
                                        tempResourceAllocatorInput_p->requiredRB,
                                        /*4X4 DL MIMO CHG START*/
                                        TB_MAPPED_TO_ONE_LAYER
                                        /*4X4 DL MIMO CHG END*/
                                        );

                                }
#endif  
                                dlStrategyTxNode_p->transmissionType = NEW_TX;

                                dlStrategyTxNode_p->sumLCPriority = FALSE;
                                dlStrategyTxNode_p->weightedQueueLoad = FALSE;

                                if (!putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex))
                                {
                                    lteWarning("putEntryInDLSchedulerNewTxQueue failed\n");
                                    /* SPR 21036 start */  
                                    /* freeMemPool(dlStrategyTxNode_p); */
                                    /* SPR 21036 end   */ 
                                    /* SPR 21035 start */
                                    freeDLHarqProcess (ueDLContext_p, harqProcess_p, internalCellIndex);
                                    /* SPR 21035 end   */ 
                                }
                                else
                                {
                                    /* Update the isAlreadyConsideredFlag */
                                    ueDLContext_p->isAlreadyConsideredFlag = dlReferenceFlag_g[internalCellIndex];

                                    (*numUEScheduled_p)++;
                                    alreadyScheduledUEs[internalCellIndex]++;
                                    maxUEsToBeScheduledDL--;
                                    if (userLocationType == CE_USER)
                                    {
                                        maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] --;
                                    }
                                    /* + COVERITY 5.0 */
                                    LOG_MAC_MSG (DL_EXPLICIT_DCI_SCHEDULED_LOG_ID, LOGDEBUG,
                                            MAC_DL_Strategy, getCurrentTick (), ueIndex,
                                            harqProcess_p->harqProcessId,
                                            harqProcess_p->assignedResourcesNB,
                                            ueDLContext_p->ueQueueLoad,
                                            /* SPR 14845 FIX START */
                                            dlStrategyTxNode_p->msgType,
                                            /* SPR 14845 FIX END */
                                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
                                    /* - COVERITY 5.0 */
                                }
                            }
                            else
                            {
                                // Push UE into explicitDLPucchTpcScheduleFailureQueue_g
                                // if not scheduled
                                if(!ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],
                                            ExplicitDLDCIQueueNode,&(node_p->nodeAnchor)))
                                {
                                    LOG_MAC_MSG(MAC_UE_NOT_SCHEDULED_IN_DL_LOG_ID, LOGDEBUG,
                                            MAC_DL_Strategy, currentGlobalTTITickCount,
                                            ueIndex, CHECK_UE_DRX_STATE(drxContext_p),
                                            CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                            __func__,"PUSH_NODE_IN_EXPLICIT_DL_DCI_FAIL_Q");
                                    FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
                                }
                                explicitDLDCIScheduleQueueCount--;
                                continue;
                            }
                            /* - coverity 24536 */
                        }
                    }   
                }
                else
                {
                    /* UE Already considered */

                    FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
                }
            }
            else
            {
                FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
                explicitDLDCIScheduleQueueCount--;
                continue;

            }
        }
        else
        {
            /* Invalid Node */
            LTE_MAC_UT_LOG(LOG_WARNING, DL_STRATEGY, "[DL-Strategy][%s] Explicit DL DCI Queue returned NULL,"
                    " Queue count %d\n",
                    __func__, explicitDLDCIScheduleQueueCount);
            break;
        }
        if (node_p)
        {
            FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
        }

        explicitDLDCIScheduleQueueCount--;
    } // end of while loop 

    /* EICIC +*/
    /* Check if count is still left , then pop nodes and push them back to maintiain same UE priority for next cycle */
     
    if ((explicitDLDCIScheduleQueueCount) && (INVALID_UE_TYPE != ueTypeToScheduleDL_g[internalCellIndex]) && (isEicicEnabled_g[internalCellIndex]) && (isTraverseList))
    {
	    while(explicitDLDCIScheduleQueueCount--) 
	    {
	        DEQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLDCIScheduleQueue_p,
	                ExplicitDLDCIQueueNode,(void ** )&nodeScan_p);

		        if( PNULL != nodeScan_p )
		        {
				                    if(!ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLDCIScheduleQueue_p,
	                      			ExplicitDLDCIQueueNode,&(nodeScan_p->nodeAnchor)))
						        {
						            FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(nodeScan_p);
						        }
		          } 
		   }
      }

    /* EICIC -*/


    *maxUEsToBeScheduledDL_p = maxUEsToBeScheduledDL;
    dlScheduleData_p->dataNodeArrTotalUE = (UInt16)totalUE ;
    dlScheduleData_p->countTADRX =  (UInt16)totalUE;
} // end of  processExplicitDLDCIScheduleQueue()

#ifdef OVERSHOOT_CORRECTION
/**********************************************************************************
 * Function Name  : modifiedNoOfUEsAfterOverLoadDL 
 * Inputs         : modifiedUEs_p - pointer to Modified UE's
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : updates the modified UE's modifiedUEs_p 
 * Returns        : None
 * Description    : This function will modify the number of UE's in a tick in 
 *                  case of OVERSHOOT based on the overload level
 *********************************************************************************/
void modifiedNoOfUEsAfterOverLoadDL(UInt8  *modifiedUEs_p,
        InternalCellIndex internalCellIndex)
{

    DLSchedulerStrategyConfig* dlSchdConfig_p;
    dlSchdConfig_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
            initParams_p->dlSchdConfig);

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
            *modifiedUEs_p = DIVIDE_BY_FOUR(dlSchdConfig_p->maxUEsToBeScheduledDL*3);
            /* SPR 18122 Changes End*/
              LOG_MAC_MSG(MAC_DL_MODIFY_UE_ID,\
                      LOGWARNING,MAC_DL_Strategy,\
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
            *modifiedUEs_p = DIVIDE_BY_TWO(dlSchdConfig_p->maxUEsToBeScheduledDL);
            /* SPR 18122 Changes End*/
              LOG_MAC_MSG(MAC_DL_MODIFY_UE_ID,\
                      LOGWARNING,MAC_DL_Strategy,\
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
            *modifiedUEs_p = DIVIDE_BY_FOUR(dlSchdConfig_p->maxUEsToBeScheduledDL);
            /* SPR 18122 Changes End*/
              LOG_MAC_MSG(MAC_DL_MODIFY_UE_ID,\
                      LOGWARNING,MAC_DL_Strategy,\
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
  dlUeSchdIndxInCurrTick_g =  *modifiedUEs_p;
#endif
  getIndexForReqElProcessingCycles();
  /*- Dyn UEs To Be SchPerTTI -*/
}
    /* +- SPR 17777 */

#endif 

/*Cyclomatic Complexity changes - starts here */
/*****************************************************************************
 * Function Name  : processNonZeroQLoadInPriorityQueue 
 * Inputs         : ueDLContext_p - Pointer to the UE Context of the UE which 
 *                     is to be processdin nonzero priority queuw
 *                  currentGlobalTTITickCount - Current Global Tick
 *                  numberOfAvailableRBs_p - pointer to current available RBs.
 *                  maxUEsToBeScheduledDL_p - max UE's can schiduled in DL
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None 
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : Its main function is for processing the UE for 
 *                  for Two TBs when the UE  is having Non Zero 
 *                  Queue Load. The output is prepared  and sent to PDCCH Module
 * *****************************************************************************/
static  MacRetType processNonZeroQLoadInPriorityQueue(
    DLUEContext *ueDLContext_p,
    /* SPR 15909 fix start */
    tickType_t currentGlobalTTITickCount,
    /* SPR 15909 fix end */
    UInt32 * numberOfAvailableRBs_p,
    UInt8 *maxUEsToBeScheduledDL_p ,
    InternalCellIndex internalCellIndex)
{
    DLHARQProcess *harqProcess_p = PNULL;
    UInt8 mcsIndexCW1 = 0;
    UInt8 mcsIndexCW2 = 0;
    UInt8 mcsThreshold = 0;
    UInt32 tb1only_sendFlag           = 0;
    DLStrategyTxNode *dlStrategyTxNode_p = PNULL;
    TempStrategyInputInfoForOneTB tempStrategyInputInfoForOneTB = {0};
    UInt32 usedRB = 0;
    UInt32 tbSize = 0;
    UInt32 tbOneSize = 0;
    UInt32 tbTwoSize = 0;
    UInt32 minTBS = 0;
    UInt32 maxTBS = 0;
    UInt32 avgMcs = 0;
    UInt32 dataSize = 0;
    UInt32 maxDLRBWithPriority = 0;
    /*4x4 DL MIMO CHG START*/
    UInt8 mcsThresholdLayer3 = 0;
    UInt8 mcsThresholdLayer4 = 0;
    /*4x4 DL MIMO CHG END*/
    DLSchedulerStrategyConfig* dlSchdConfig_p;
    dlSchdConfig_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
            initParams_p->dlSchdConfig);

    if ( PNULL == (harqProcess_p = checkForFreeHarqProcess
                ( ueDLContext_p,
                  ueDLContext_p->dlHarqContext_p,
                  DRA_SCHEDULING,
		  internalCellIndex)))
    {
        LOG_MAC_MSG (DL_PRIORITY_SCHEDULED_FAILURE_LOG_ID, LOGWARNING,
                MAC_DL_Strategy, getCurrentTick (), ueDLContext_p->ueIndex,
                0,
                0,
                0,
                0,
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");

        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                "[DL-Strategy][%s]No free Harq process "
                "Found for UE ID: %d\n",
                __func__,ueDLContext_p->ueIndex);

        return MAC_FAILURE;
    }
    else
    {
        UInt32 tempTransmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;

        UInt32 numLCWithNonZeroQueueLoad =
            calculateNumLCWithNonZeroQueueLoad (ueDLContext_p );
        if ((TX_MODE_3 == tempTransmissionMode)||
                (TX_MODE_4 == tempTransmissionMode)||
                /* + TM7_8 Changes Start */
                (((TX_MODE_7 == tempTransmissionMode) ||
                  (TX_MODE_8 == tempTransmissionMode)) &&
                  (1 < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas)))
                /* - TM7_8 Changes End */
        {
            mcsIndexCW1 = ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
            mcsIndexCW2 = ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
            /* SPR 7981 changes start */
            mcsThreshold =
                dlSchdConfig_p->mcsThreshold[ueDLContext_p->userLocationType];
            if( ((RANK_INDICATOR_1 == ueDLContext_p->dlMIMOInfo.dlRi) ||
                        (mcsIndexCW1 <=  mcsThreshold)|| 
                        (mcsIndexCW2 <= mcsThreshold)) ||
                    /* SPR 7981 changes end */
                    /* + TM7_8 Changes Start */
                    ((TX_MODE_8 == tempTransmissionMode) && 
                     (BF_ALGO_NOT_AVAILABLE == 
                      (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->bfVectorPresentFlag)))
                /* - TM7_8 Changes End */

            {
                tb1only_sendFlag = 1;
                POPULATE_INFO_FOR_ONE_TB(tempTransmissionMode,
                        1,
                        tempStrategyInputInfoForOneTB,
                        /* + TM7_8 Changes Start */
                        ueDLContext_p->beamFormingAlgoToBeUsed,
                        internalCellIndex);
                /* - TM7_8 Changes End */
                /* SPR 7981 changes start */
                if((mcsIndexCW2 <= mcsThreshold) &&
                        (mcsIndexCW1 > mcsThreshold))
                {
                    ueDLContext_p->dlCQIInfo.cqiCorrectionFactorForCW1 =
                        ueDLContext_p->dlCQIInfo.cqiCorrectionFactorForCW0;
                }
                /* SPR 7981 changes end */

            }
            /* + TM7_8 Changes Start */ 
            /* SPR 21631 Start */
            if((6 == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]
                        ->cellParams_p->dlAvailableRBs) &&        
                    (CHECK_SF_MAPPED_ON_BCH_PSS_SSS((currentGlobalTTITickCount+
                                                     PHY_DL_DELAY),tempTransmissionMode)))
                /* SPR 21631 End */
            {
                tb1only_sendFlag = 1;
                tempStrategyInputInfoForOneTB.isTBOneValid = TRUE;
                tempStrategyInputInfoForOneTB.isTBTwoValid = FALSE;
                tempStrategyInputInfoForOneTB.txIndicatorForTB1 = NEW_TX;
                tempStrategyInputInfoForOneTB.txIndicatorForTB2 = INVALID_TX;
                tempStrategyInputInfoForOneTB.primaryDCIFormat = DCI_FORMAT_1A;
                tempStrategyInputInfoForOneTB.secondaryDCIFormat = INVALID_DCI_FORMAT;
            }
            /* - TM7_8 Changes End */ 

            /*4X4 DL MIMO CHG START*/
            mcsThresholdLayer3 = 
                getMCSFromCQI(dlSchdConfig_p->cqiThresholdCellCenterLayer3[ueDLContext_p->userLocationType]
                            );

            mcsThresholdLayer4 = 
                getMCSFromCQI(dlSchdConfig_p->cqiThresholdCellCenterLayer4[ueDLContext_p->userLocationType]
                            );

            if ((RANK_INDICATOR_4 == ueDLContext_p->dlMIMOInfo.dlRi\
                        || RANK_INDICATOR_3 == ueDLContext_p->dlMIMOInfo.dlRi) &&\
                             tb1only_sendFlag != 1)
            {
                if ((ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne > mcsThresholdLayer4) &&
                    (ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo > mcsThresholdLayer4))
                {
                    harqProcess_p->numOfLayer = ueDLContext_p->dlMIMOInfo.dlRi;
                }
                else if(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo > mcsThresholdLayer3)
                {
                    harqProcess_p->numOfLayer = RANK_INDICATOR_3;
                }
                else
                {
                    harqProcess_p->numOfLayer = RANK_INDICATOR_2;
                }
            }
            /*4X4 DL MIMO CHG END*/
        }
        /* + TM6_5.2 */
        else if(TX_MODE_6 == tempTransmissionMode)
        {
            tb1only_sendFlag = 1;
            POPULATE_INFO_FOR_ONE_TB(tempTransmissionMode,
                    1,
                    tempStrategyInputInfoForOneTB,
                    /* + TM7_8 Changes Start */
                    BF_ALGO_NOT_AVAILABLE,
                    /* - TM7_8 Changes End */
                    internalCellIndex);

            tempStrategyInputInfoForOneTB.primaryDCIFormat = ueDLContext_p->tm6DCIFormat;
            tempStrategyInputInfoForOneTB.secondaryDCIFormat = DCI_FORMAT_1A;

        }
        /* - TM6_5.2 */
        else
        {
            tb1only_sendFlag = 2;
        }

        ResourceAllocatorInput * tempResourceAllocatorInput_p = PNULL;
        dlStrategyTxNode_p=PNULL;
#ifdef TDD_CONFIG
        UInt8 subFrame = currentGlobalTTITickCount % MAX_SUBFRAME;
#endif
        dataSize = NET_UE_QL(ueDLContext_p);
        avgMcs = ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
        /* SPR 5999 changes start */
        maxDLRBWithPriority = DETERMINE_RBs_DL(avgMcs, dataSize, internalCellIndex);
        /* SPR 5999 changes end */
        /*Preparing the DL Strategy Output Node*/
        dlStrategyTxNode_p =(DLStrategyTxNode*)getMemFromPool
            (sizeof(DLStrategyTxNode),PNULL);

        /* + CQI_5.0 */
        if (dlStrategyTxNode_p)
        {
            tempResourceAllocatorInput_p = &(dlStrategyTxNode_p->resourceAllocInput);
            tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
            tempResourceAllocatorInput_p->strictAllocSubBand  = FALSE;
            dlStrategyTxNode_p->isCQIRequestedULGrantMsg       = FALSE;
            tempResourceAllocatorInput_p->subBandPMIInfo_p = PNULL;
            tempResourceAllocatorInput_p->subbandPMI = FALSE;
            tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;

            /*SPR 5313 Fix start */
            tempResourceAllocatorInput_p->resourceRegionAllocated  =
                ueDLContext_p->userLocationType;
            /*SPR 5313 Fix end */

            /* SPR 3430 */
            dlStrategyTxNode_p->allocPushCount = 0;
            /* SPR 3430 */
            dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;
            dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId ;
            dlStrategyTxNode_p->harqProcess_p = harqProcess_p; 

            dlStrategyTxNode_p->sumLCPriority     = FALSE;
            dlStrategyTxNode_p->weightedQueueLoad = FALSE;

            if(tb1only_sendFlag == 2)
            {
                ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;
                usedRB = validateRBForOneTB(ueDLContext_p,
                        maxDLRBWithPriority,
                        numLCWithNonZeroQueueLoad,
                        &tbSize,
                        numberOfAvailableRBs_p[ueDLContext_p->userLocationType],
                        /* + CQI_5.0*/
                        /* +- SPR 17777 */
                        NON_SPATIAL_MUL,&minTBS,&maxTBS
                        /* - CQI_5.0*/
#ifdef TDD_CONFIG
                        , subFrame
                        ,internalCellIndex
#endif
                        /* +- SPR 17777 */
                        );

                if (usedRB)
                {
                    /*Setting only TBOne as valid and other INVALID*/
                    harqProcess_p->isTBOneValid = TRUE;
                    harqProcess_p->isTBTwoValid = FALSE;
                    dlStrategyTxNode_p->transmissionType = NEW_TX;
                    dlStrategyTxNode_p->minTBSize = minTBS;
                    dlStrategyTxNode_p->maxTBSize = maxTBS;
                    harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;
                    harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;
                    if (ueDLContext_p->dlCQIInfo.isSubBandReportAvailable)
                    {
                        LTE_MAC_UT_LOG(LOG_DETAIL,DL_RR_STRATEGY,
                                "[DL-Strategy][%s]INSIDE SubBand Report Available\n"
                                ,__func__);
                        tempResourceAllocatorInput_p->subBandReportInfo_p =
                            ueDLContext_p->dlCQIInfo.subBandReportInfo_p;
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBOneInfo.mcsIndex  =
                            ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
                        harqProcess_p->dlHARQTBOneInfo.tbSize  =  tbSize;
                        harqProcess_p->dlHARQTBTwoInfo.tbSize    =  0;

                    }
                    /*DCI Format Calculation*/
                    dciFormatSelector (ueDLContext_p,dlStrategyTxNode_p,
                            HARQ_FREE_TX_OTHER_THAN_3_4,internalCellIndex);

                    dlStrategyTxNode_p->msgType = NON_SPATIAL_MUL;
                }
                /* + SPR 13318 Changes */
                else
                {
                    freeMemPool(dlStrategyTxNode_p);
                    dlStrategyTxNode_p = PNULL;	
                    freeDLHarqProcess (ueDLContext_p, harqProcess_p,internalCellIndex);
                    return MAC_FAILURE;
                }
                /* - SPR 13318 Changes */
            }
            else if(tb1only_sendFlag == 1)
            {
                ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;
                usedRB = validateRBForOneTB(ueDLContext_p,
                        maxDLRBWithPriority,
                        numLCWithNonZeroQueueLoad,
                        &tbSize,
                        numberOfAvailableRBs_p[ueDLContext_p->userLocationType],
                        /* + CQI_5.0*/
                        /* +- SPR 17777 */
                        SPATIAL_MUL,&minTBS,&maxTBS
                        /* - CQI_5.0*/
#ifdef TDD_CONFIG
                        , subFrame
                        ,internalCellIndex
#endif
                        /* +- SPR 17777 */
                        );

                if (usedRB)
                {

                    /*Setting only TBOne as valid and other INVALID*/
                    harqProcess_p->isTBOneValid = 
                        tempStrategyInputInfoForOneTB.isTBOneValid;
                    harqProcess_p->isTBTwoValid = 
                        tempStrategyInputInfoForOneTB.isTBTwoValid;
                    dlStrategyTxNode_p->transmissionType = NEW_TX;
                    dlStrategyTxNode_p->minTBSize = minTBS;
                    dlStrategyTxNode_p->maxTBSize = maxTBS;
                    harqProcess_p->dlHARQTBOneInfo.txIndicator = 
                        tempStrategyInputInfoForOneTB.txIndicatorForTB1;
                    harqProcess_p->dlHARQTBTwoInfo.txIndicator = 
                        tempStrategyInputInfoForOneTB.txIndicatorForTB2;

                    if (ueDLContext_p->isSubbandPMIAvailable)
                    {
                        tempResourceAllocatorInput_p->subbandPMI = TRUE;
                        tempResourceAllocatorInput_p->subBandPMIInfo_p =
                            &ueDLContext_p->dlCQIInfo.subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo;
                    }
                    if(TRUE == ueDLContext_p->aperiodicMode12Rcvd)
                    {
                        tempResourceAllocatorInput_p->aperiodicMode12 = TRUE;
                        tempResourceAllocatorInput_p->subBandReportInfo_p =
                            ueDLContext_p->dlCQIInfo.subBandReportInfo_p;
                    }

                    if (ueDLContext_p->dlCQIInfo.isSubBandReportAvailable)
                    {
                        LTE_MAC_UT_LOG(LOG_DETAIL,DL_RR_STRATEGY,
                                "[DL-Strategy][%s]INSIDE SubBand Report Available\n"
                                ,__func__);

                        tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;
                        tempResourceAllocatorInput_p->subBandReportInfo_p =
                            ueDLContext_p->dlCQIInfo.subBandReportInfo_p;
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBOneInfo.mcsIndex  =
                            ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
                        harqProcess_p->dlHARQTBOneInfo.tbSize  =  tbSize;
                        harqProcess_p->dlHARQTBTwoInfo.tbSize    =  0;

                    }
                    /*DCI Format Calculation*/

                    dlStrategyTxNode_p->primaryDCIFormat =
                        tempStrategyInputInfoForOneTB.primaryDCIFormat;
                    dlStrategyTxNode_p->secondaryDCIFormat =
                        tempStrategyInputInfoForOneTB.secondaryDCIFormat;
                    dlStrategyTxNode_p->msgType = SPATIAL_MUL;
                } 
                /* + SPR 13318 Changes */
                else
                {
                    freeMemPool(dlStrategyTxNode_p);
                    dlStrategyTxNode_p = PNULL;	
                    freeDLHarqProcess (ueDLContext_p, harqProcess_p,internalCellIndex);
                    return MAC_FAILURE;
                }
                /* - SPR 13318 Changes */
            }    
            else 
            {
                /* - CL-MIMO LJA*/
                ueDLContext_p->maxTBSize = ueDLContext_p->maxMIMOTBSize;
                /*4X4 DL MIMO CHG START*/

                if (

              /* Coverity_73310 Fix Start */  
                    (0 != maxDLRBWithPriority) && 
              /* Coverity_73310 Fix End */

                    ((SInt64)(tbOneSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                                maxDLRBWithPriority,TB_MAPPED_TO_ONE_LAYER)) < NET_UE_QL(ueDLContext_p))
                   )

                {/*4X4 DL MIMO CHG END*/
                    /*Validating the allocated RBS against the Queue Load considering TWO
                     * TBs*/
                    usedRB = validateRBForTwoTB(ueDLContext_p, maxDLRBWithPriority,
                            numLCWithNonZeroQueueLoad,&tbOneSize,&tbTwoSize,
                            numberOfAvailableRBs_p[ueDLContext_p->userLocationType], 
                            SPATIAL_MUL,&minTBS,&maxTBS
#ifdef TDD_CONFIG
                            , subFrame
                            ,internalCellIndex
                            /* +- SPR 17777 */
#endif
                            /*4X4 DL MIMO CHG START*/
                            ,harqProcess_p 
                            /*4X4 DL MIMO CHG END*/
                            );
                }
                else
                {
                    tbTwoSize = 0;
                    usedRB = validateRBForOneTB(ueDLContext_p, maxDLRBWithPriority,
                            numLCWithNonZeroQueueLoad, &tbOneSize,
                            /* + CQI_5.0*/
                            /* +- SPR 17777 */
                            numberOfAvailableRBs_p[ueDLContext_p->userLocationType], SPATIAL_MUL,&minTBS,&maxTBS
                            /* - CQI_5.0*/
#ifdef TDD_CONFIG
                            , subFrame
                            ,internalCellIndex
#endif
                            /* +- SPR 17777 */
                            );
                }

                if (usedRB)
                {
                    LTE_MAC_UT_LOG(LOG_DETAIL,DL_RR_STRATEGY,
                            "[DL-Strategy][%s][TB_ONE:NEW DATA],[TB_TWO:NEW DATA]\n"
                            "validateRBForOneTB returned %d RBs for UEID %d\n",__func__,
                            usedRB,ueDLContext_p->ueIndex);

                    /*Preparing the DL Strategy Output Node*/
                    /* Setting  both TBOne and TBTwo as valid */
                    harqProcess_p->isTBOneValid = TRUE;
                    harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;

                    if (tbTwoSize)
                    {
                        harqProcess_p->isTBTwoValid = TRUE;
                        harqProcess_p->dlHARQTBTwoInfo.txIndicator = NEW_TX;
                        /*DCI Format Calculation*/
                        dciFormatSelector (ueDLContext_p,dlStrategyTxNode_p,
                                                   /* + TM7_8 Changes Start */
                                                    HARQ_FREE_TX_3_4_8,
                                                   /* - TM7_8 Changes End */
                                                   internalCellIndex);

                    }
                    else
                    {
                        harqProcess_p->isTBTwoValid = FALSE;
                        harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;
                        /* + TM7_8 Changes Start */
                        if(TX_MODE_8 == tempTransmissionMode)
			{
				dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_2B;
				dlStrategyTxNode_p->secondaryDCIFormat = DCI_FORMAT_1A;
			}
                        else
			{
			/* - TM7_8 Changes End */
				dlStrategyTxNode_p->secondaryDCIFormat = 
					macParams_g.preferredSecondaryDCIFormatForSingleTB[tempTransmissionMode];
				dlStrategyTxNode_p->primaryDCIFormat = 
					macParams_g.preferredPrimaryDCIFormatForSingleTB[tempTransmissionMode];
                /* CA Changes end */
			}
                    }
                    dlStrategyTxNode_p->minTBSize = minTBS;
                    dlStrategyTxNode_p->maxTBSize = maxTBS;

                    dlStrategyTxNode_p->msgType = SPATIAL_MUL;
                    /*+ SPR 22400 Changes */
                    dlStrategyTxNode_p->transmissionType = NEW_TX;
                    /*- SPR 22400 Changes */


                    if (ueDLContext_p->isSubbandPMIAvailable)
                    {
                        tempResourceAllocatorInput_p->subbandPMI = TRUE;
                        tempResourceAllocatorInput_p->subBandPMIInfo_p =
                            &ueDLContext_p->dlCQIInfo.subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo;
                    }
                    if(TRUE == ueDLContext_p->aperiodicMode12Rcvd)
                    {
                        tempResourceAllocatorInput_p->aperiodicMode12 = TRUE;
                        tempResourceAllocatorInput_p->subBandReportInfo_p =
                            ueDLContext_p->dlCQIInfo.subBandReportInfo_p;


                    }
                    if (ueDLContext_p->dlCQIInfo.isSubBandReportAvailable)
                    {
                        LTE_MAC_UT_LOG(LOG_DETAIL,DL_RR_STRATEGY,
                                "[DL-Strategy][%s]INSIDE SubBand Report Available\n"
                                ,__func__);
                        tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;
                        tempResourceAllocatorInput_p->subBandReportInfo_p =
                            ueDLContext_p->dlCQIInfo.subBandReportInfo_p;
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBOneInfo.mcsIndex  =
                            ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
                        harqProcess_p->dlHARQTBTwoInfo.mcsIndex  =
                            ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
                        harqProcess_p->dlHARQTBOneInfo.tbSize  =  tbOneSize;
                        harqProcess_p->dlHARQTBTwoInfo.tbSize  =  tbTwoSize;
                    }

                }
                /* + SPR 13318 Changes */
                else
                {
                    freeMemPool(dlStrategyTxNode_p);
                    freeDLHarqProcess (ueDLContext_p, harqProcess_p,internalCellIndex);
                    return MAC_FAILURE;
                }
                /* - SPR 13318 Changes */
            }
            /* - CQI_5.0 */
            /*Preparing the scheduler data*/
            tempResourceAllocatorInput_p->requiredRB = usedRB;
            harqProcess_p->assignedResourcesNB       = usedRB;
            /*Updating the remaining RB available*/
            numberOfAvailableRBs_p[ueDLContext_p->userLocationType]  -= usedRB;
            (*maxUEsToBeScheduledDL_p)--;

            /* SPR Fix 5147 start*/
            /* ICIC changes start */
            /* SPR 5147 Fix Start */
            if (ueDLContext_p->userLocationType == CE_USER)
            {
                /* UE is a cell edge UE, update the CC_CE_REGION RB count  */
                numberOfAvailableRBs_p[CC_CE_REGION] -= usedRB;
                maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;
            }
            else
            {
                if ((numberOfAvailableRBs_p[CC_CE_REGION] < 
                            numberOfAvailableRBs_p[CE_REGION]))
                {
                    numberOfAvailableRBs_p[CE_REGION] =
                        numberOfAvailableRBs_p[CC_CE_REGION];
                }
            }
            /* SPR 5147 Fix End*/
            /* ICIC changes end */

            /* + COVERITY 5.0 */
            LOG_MAC_MSG (DL_PRIORITY_QUEUE_SCHEDULED_LOG_ID, LOGDEBUG,
                    MAC_DL_Strategy, getCurrentTick (), ueDLContext_p->ueIndex,
                    harqProcess_p->harqProcessId,
                    harqProcess_p->assignedResourcesNB,
                    ueDLContext_p->ueQueueLoad,
                    dlStrategyTxNode_p->msgType,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
            /* - COVERITY 5.0 */

            /* SPR #3098 Changes Starts*/
            /*putEntry of DLStrategyTXNode in PDCCH Queue */
            if (!(putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex)))
            {
                /* + SPR 13318 Changes */
                /* dlStrategyTxNode_p is getting freed in putEntryInDLSchedulerNewTxQueue()*/
                /* freeMemPool(dlStrategyTxNode_p); */
                /* - SPR 13318 Changes */
                harqProcess_p->harqProcessId = INVALID_HARQ_ID;
                harqProcess_p->harqStatus    = HARQ_PROCESS_FREE;
            }
        }
        else
        {
            ltePanic("\ndlStrategyTxNode_p getMemFromPool failed");
        }
    }
    return MAC_SUCCESS;

}
/*Cyclomatic Complexity changes - ends here */

/* SRP 3608 changes start */
/*****************************************************************************
 * Function Name  : processPriorityQueue
 * Inputs         : priorityQueueCount - This is Count of the
 *                  total nodes present in the dlPriorityQueue
 *                  numberOfAvailableRBs_p-pointer to current available RBs.
 *                  currentGlobalTTITickCount:Current Global Tick
 *                  dlPriorityQueue_p - Pointer to DL Priority Queue
 *                  maxUEsToBeScheduledDL_p - max UE's can schduled in DL
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : It will allocate RBs to PSR priority Queue  Nodes & 
 *                  place the DL Strategy O/P either in the PDCCHNewTXQueue_g
 *                  or PDCHHReTXQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the Priority PSR
                    Queues Nodes. Allocate RBs to the UEs with non
                    Zero Queue Load otherwise add them in scheduler Array.
 *****************************************************************************/
void processPriorityQueue(
        UInt32 dlPriorityQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        /* SPR 6897 changes start */
        LTE_CIRCQUEUE * dlPriorityQueue_p,
        /* SPR 6897 changes end */
        UInt8 *maxUEsToBeScheduledDL_p,
        InternalCellIndex internalCellIndex)
{

    DLUEContext *ueDLContext_p          = PNULL;
    /* DLHARQEntity *ueharqDLContext_p     = PNULL ; TDD Warning Fix */
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    UInt32 ueIndex = 0;
    DLPriorityQueueNode *dlPriorityQNode_p = PNULL;
    DRXContext          *drxContext_p = PNULL;
    MeasGapContextDL      *measGapContext_p = PNULL;
    MacRetType retType = MAC_SUCCESS;
    /* ICIC Changes start */
    UInt8 userLocationType = CC_USER;
    /* ICIC Changes end */

   /* EICIC +*/
	UInt8	isTraverseList = LTE_FALSE;

    /* EICIC -*/

    while ((dlPriorityQueueCount)&&( numberOfAvailableRBs_p[CC_CE_REGION])&& *maxUEsToBeScheduledDL_p)
    {
        /* SPR 6897 changes start */
        popNodeCircQ( dlPriorityQueue_p, (void **)&dlPriorityQNode_p );
        /* SPR 6897 changes end */

        if (dlPriorityQNode_p)
        {
            ueIndex = dlPriorityQNode_p ->ueIndex;
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            if (!(ueDLContextInfo_p->pendingDeleteFlag))
            {
                ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
		    /* EICIC +
		       Lets check the UE type ( Victim / Non Victim ) required to be scheduled  */
/* Complexity Start */
		    if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                  (NON_VICTIM_UE == ueDLContext_p->userType))||
                   ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                   (VICTIM_UE == ueDLContext_p->userType)))
		    {   
			    if (pushNodeCircQ(dlPriorityQueue_p,
            /* SPR 23483 Changes Start */
						    dlPriorityQNode_p) == CIRC_FAILURE)
            /* SPR 23483 Changes End */
			    {
				    lteWarning("Error in Pushing in Pending"
						    " dlPriorityQueue in funtion %s\n",
						    __func__);
				    freeMemPool((void *) dlPriorityQNode_p);
				    dlPriorityQNode_p = PNULL;
			    } 

			    isTraverseList = LTE_TRUE;
			    dlPriorityQueueCount--;  
			    continue;		             
		    }

/* Complexity End */
		    /* EICIC - */  


                userLocationType = ueDLContext_p->userLocationType;
                /* ICIC change end */
                /*SPR 12339 changes start */
                /*SPR_13439_Changes Start*/
                /*ueDLContext_p->isPresentInPriorityQueue = FALSE;*/
                /*SPR_13439_Changes End*/
                /*SPR 12339 changes end */
                /*Checking if Already being considered for RB Distribution*/
                if (ueDLContext_p->isAlreadyConsideredFlag != 
                        dlReferenceFlag_g[internalCellIndex] )
                {
                    /* DRX_CHG and MEAS_GAP_CHG */
                    drxContext_p = &ueDLContext_p->drxContext;
                    measGapContext_p = &ueDLContext_p->measGapContext;

                    if (DRX_STATE_ON == CHECK_UE_DRX_STATE(drxContext_p) ||
                            TRUE == CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p) ||
                            (UL_SYNC_LOSS_ON ==
                             checkUlSyncLossForUE(ueDLContextInfo_p->dlUEContext_p, 
                                    currentGlobalTTITickCount,internalCellIndex))||
                            (( CE_USER == userLocationType ) && ( !maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex])) 
                            /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                            ||
                            ((TRUE == ueDLContext_p->isHDFddFlag &&
                            (!(CAN_SCHEDULE_DL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY, \
                            ueIndex, hdUeContextInfo_g.hdfddUeSchedMap)))))
#endif
                            /*HD FDD Changes End */
                            )
                    {
                        /* SPR 6897 changes start */
                        if (pushNodeCircQ(dlPriorityQueue_p,
            /* SPR 23483 Changes Start */
                                    dlPriorityQNode_p) == CIRC_FAILURE)
            /* SPR 23483 Changes End */
                        /* SPR 6897 changes end */
                        {
                            lteWarning("Error in Pushing in Pending"
                                    " dlPriorityQueue in funtion %s\n",
                                    __func__);
                            freeMemPool(dlPriorityQNode_p);
                            /* + Coverity_31464 */
                             dlPriorityQNode_p = PNULL;
                            /* - Coverity_31464 */
                            /*SPR_13439_Changes Start*/
                            ueDLContext_p->isPresentInPriorityQueue = FALSE;
                            /*SPR_13439_Changes End*/

                        }
                        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                        if (TRUE == ueDLContext_p->isHDFddFlag)
                        {
                            UPDATE_UE_DENIED_DL_RESOURCE(ueIndex);
                        }
#endif
                        /*HD FDD Changes End */

                        dlPriorityQueueCount--;
                        continue;
                    }
                    /* DRX_CHG and MEAS_GAP_CHG */
                    /* \code
                     * if schType == SPS_SCHEDULING, then do not perform
                     * priority scheduling for this UE.
                     * */
                    if( SPS_SCHEDULING == ueDLContext_p->schType )
                    {
                        /* SPR 6897 changes start */
                        if (pushNodeCircQ(dlPriorityQueue_p,
            /* SPR 23483 Changes Start */
                                    dlPriorityQNode_p) == CIRC_FAILURE)
            /* SPR 23483 Changes End */
                        /* SPR 6897 changes end */
                        {
                            freeMemPool(dlPriorityQNode_p);
                            dlPriorityQNode_p = PNULL;
                            /*SPR_13439_Changes Start*/
                            ueDLContext_p->isPresentInPriorityQueue = FALSE;
                            /*SPR_13439_Changes End*/
                        }
                        dlPriorityQueueCount--;
                        continue;
                    }

                    /*ueharqDLContext_p = ueDLContext_p->dlHarqContext_p;         
                      TDD warning Fix */

                    /*Queue Load is non Zero*/
                    if ((NET_UE_QL(ueDLContext_p)) > ZERO)
                    {
                        /*Cyclomatic Complexity changes - starts here */
                        retType = processNonZeroQLoadInPriorityQueue(ueDLContext_p,
                                                           currentGlobalTTITickCount,
                                                           numberOfAvailableRBs_p,
                                                           maxUEsToBeScheduledDL_p,
                                                           internalCellIndex);
                        if(MAC_FAILURE == retType)
                        {
                            /* SPR 6897 changes start */
                            if (pushNodeCircQ(dlPriorityQueue_p,
            /* SPR 23483 Changes Start */
                                        dlPriorityQNode_p) == CIRC_FAILURE)
            /* SPR 23483 Changes End */
                                /* SPR 6897 changes end */
                            {
                                lteWarning("Error in Pushing in Pending"
                                        " dlPriorityQueue in funtion %s\n",
                                        __func__);
                                freeMemPool((void *) dlPriorityQNode_p);
                                dlPriorityQNode_p = PNULL;
                                /*SPR_13439_Changes Start*/
                                ueDLContext_p->isPresentInPriorityQueue = FALSE;
                                /*SPR_13439_Changes End*/

                            }
                            dlPriorityQueueCount--;
                            continue;

                        }
                        /*Cyclomatic Complexity changes - ends here */

                    }
                    else
                    {
                        LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"[DL-Strategy][%s] NET_UE_QL is Non Zero for ue ID: %d"
                                ,__func__, ueDLContext_p->ueIndex);
                    }
                }/*end of already considered flag check, The UE which are
                   already considered will be deleted automatically */
                else
                {
                    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
                            "[DL-Strategy][%s]:UE ID: %d is already "
                            "considered, hence NOT \
                            considering UE AlreadyConsideredFlag:%d"
                            " curr Global Tick %d"
                            ,__func__,ueDLContext_p->ueIndex,
                            ueDLContext_p->isAlreadyConsideredFlag,
                            currentGlobalTTITickCount );
                }
                /* Also free the memory of Priority Nodes*/
                freeMemPool((void *) dlPriorityQNode_p);
                dlPriorityQNode_p = PNULL;
                dlPriorityQueueCount--;
                /* E_CID_FIX */
                ueDLContext_p->isPresentInPriorityQueue = FALSE;
                /* E_CID_FIX */
            }
            else
            {
                /* Also free the memory of Priority Nodes*/
                freeMemPool((void *) dlPriorityQNode_p);
                dlPriorityQNode_p = PNULL;
                dlPriorityQueueCount--;

            }
        }
    } 

   /* EICIC +*/
   /* Check if count is still left , then pop nodes and push them back to maintiain same UE priority for next cycle */

   /* Complexity Start */
   eicicPopDLPriorityQNodeScan(
	dlPriorityQueueCount,
	internalCellIndex,
	isTraverseList,
	dlPriorityQueue_p);
    /* Complexity End */
    /* EICIC -*/

    LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Exit:[DL-Strategy][%s]"
            ,__func__);

}

/* Complexity Start*/
/*****************************************************************************
 * Function Name  : eicicPopDLPriorityQNodeScan 
 * Inputs         : dlPriorityQueueCount - Queue Count
 *                  internalCellIndex- Cell Index at Mac
                    isTraverseList
                    dlPriorityQueue_p- Pointer to Queue
 * Outputs        : None
 * Returns        : None
 * Description    : This function Pops node from DL Priority Queue 
 *                 
 ******************************************************************************/
void eicicPopDLPriorityQNodeScan(
	UInt32 dlPriorityQueueCount,
	InternalCellIndex internalCellIndex,
	UInt8 isTraverseList,
	LTE_CIRCQUEUE *dlPriorityQueue_p)
{
DLPriorityQueueNode *dlPriorityQNodeScan_p = PNULL;
if ((dlPriorityQueueCount) && (INVALID_UE_TYPE != ueTypeToScheduleDL_g[internalCellIndex]) && (isEicicEnabled_g[internalCellIndex]) && (isTraverseList))
    {

	    while(dlPriorityQueueCount--) 
	    {
	        popNodeCircQ( dlPriorityQueue_p, (void **)&dlPriorityQNodeScan_p );
	        
	        if( PNULL != dlPriorityQNodeScan_p )
	        {
	            		 if (pushNodeCircQ(dlPriorityQueue_p,
            /* SPR 23483 Changes Start */
	                                        dlPriorityQNodeScan_p) == CIRC_FAILURE)
            /* SPR 23483 Changes End */
	         	 	{
	                    lteWarning("Error in Pushing in Pending"
	                      " dlPriorityQueue in funtion %s\n",
	                          __func__);
	                     freeMemPool((void *) dlPriorityQNodeScan_p);
	                      dlPriorityQNodeScan_p = PNULL;
	         		}
	            } 
	        }
    }

}
/* Complexity End*/


/*****************************************************************************
 * Function Name  : putEntryInDLPriorityQueue
 * Inputs         : ueIndex- A pointer to DLUEContextueIndex,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None.
 * Description    : This function create an entry for ueIndex and add it into 
 *                 DLpriority queue
 *****************************************************************************/
void putEntryInDLPriorityQueue( UInt32 ueIndex,
                                InternalCellIndex internalCellIndex)
{
    DLPriorityQueueNode *dlPriorityQNode_p = PNULL;
    GET_MEM_FROM_POOL(DLPriorityQueueNode, dlPriorityQNode_p,
            sizeof(DLPriorityQueueNode),PNULL);

    /* + coverity 24538 */
    if (PNULL != dlPriorityQNode_p)
    {
        dlPriorityQNode_p->ueIndex = ueIndex;
        /* SPR 6897 changes start */
        /** SPR 14884 changes start **/
        if(!(pushNodeCircQ(dlPriorityQueue_gp[internalCellIndex], dlPriorityQNode_p)))
        /** SPR 14884 changes End **/
        {
           freeMemPool(dlPriorityQNode_p);
        /** SPR 14884 changes start **/
           dlPriorityQNode_p = PNULL;
        /** SPR 14884 changes End **/
}   
        /* SPR 6897 changes end */
    }
    /* - coverity 24538 */
}
/* SPR 3608 changes end */

//=========================================================================
/* + Rel_6_0_reTx_Changes */
//=========================================================================

/*****************************************************************************
 * Function Name  : lteMacStrategyAssignRBsForReTxFail
 * Inputs         : dlUEContext_p -  pointer to DL UE context,
 *                  dlHarqProcess_p - pointer to the harq process,
 *                  dlStrategyTxNode_p -  Pointer to DL Strategy Node.
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : 
 * Returns        : MAC_SUCCESS/ MAC_FAILURE
 * Description    : This function based on the transmission mode, sets the DL
 *                  Harq TB info for RE_TX and calls the appropriate function to
 *                  push a node in the DL Scheduler Re-TX queue
 *****************************************************************************/
MacRetType lteMacStrategyAssignRBsForReTxFail( 
                                 DLUEContext *dlUEContext_p, 
                                 DLHARQProcess *dlHarqProcess_p, 
                                 DLStrategyTxNode *dlStrategyTxNode_p,
                                 InternalCellIndex internalCellIndex) 
{
    MacRetType retVal = MAC_SUCCESS;
    TransmissonMode transMode = INVALID_TX_MODE; 

#ifdef DL_UL_SPLIT
    ULUEContext *ulUEContext_p = PNULL; 
#else
    UInt8  dlRi = 0;
#endif
    UInt32 tempTBSizeRet = 0; 
    UInt8  tb1OnlyFlag = FALSE;
    UInt8  userLocationType = CC_USER;
    DLScheduleFlag* scheduleFlag_p = PNULL;
    DLCQIInfo* dlCQIInfo_p = PNULL;
    DCIFormat tm6DCIFormat;

    if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
    {
        userLocationType = dlUEContext_p->userLocationType;
        scheduleFlag_p = &(dlUEContext_p->scheduleFlag);
        transMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
        dlCQIInfo_p = &(dlUEContext_p->dlCQIInfo);
        tm6DCIFormat = dlUEContext_p->tm6DCIFormat;
#ifndef DL_UL_SPLIT
        dlRi = dlUEContext_p->dlMIMOInfo.dlRi;
#endif
    }
    else
    {
        userLocationType = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
        scheduleFlag_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag);
        transMode = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.transmissionMode;
        dlCQIInfo_p = &(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
        tm6DCIFormat = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->Scelltm6DCIFormat;
#ifndef DL_UL_SPLIT
        dlRi = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.dlRi;
#endif
    }

    *scheduleFlag_p =  DL_QOS_NO_NEW_TRANSMISSION;

    if ( TX_MODE_3 == transMode || TX_MODE_4 == transMode ||
         ( (TX_MODE_7 == transMode || TX_MODE_8 == transMode) &&
           (1 < cellSpecificParams_g.cellConfigAndInitParams_p[\
                internalCellIndex]->cellParams_p->numOfTxAnteenas)))
    {
#ifdef DL_UL_SPLIT
        ulUEContext_p = ulUECtxInfoArr_g[dlUEContext_p->ueIndex].ulUEContext_p;

        if( (RANK_INDICATOR_1 == ulUEContext_p->pucchConfigInfo.ueReportedRI)|| 
#else
        if( (RANK_INDICATOR_1 == dlRi) ||
#endif
            (dlCQIInfo_p->mcsIndexCodeWordOne <=
               cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->
               dlSchdConfig.mcsThreshold[userLocationType]) ||
            (dlCQIInfo_p->mcsIndexCodeWordTwo <=
               cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->
               dlSchdConfig.mcsThreshold[userLocationType]) ||
            (DCI_FORMAT_1A == dlHarqProcess_p->dciFormat))
        {
            /* Set tb1OnlyFlag to True. Only reTx can be schduled. 
             * No New_Tx would be there even Ue have sufficient 
             * qload and TB available for New_Tx */
            tb1OnlyFlag = TRUE;
        }

        if (tb1OnlyFlag)
        {

            if ((dlHarqProcess_p->isTBOneValid) &&
                    (dlHarqProcess_p->isTBTwoValid))
            {
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;
                dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator = RE_TX;

                retVal = lteMacStrategyDLNackTwoTBWithZeroQueueLoadForReTxFail(
                                     dlUEContext_p,
                                     dlHarqProcess_p,
                                     TB_ONE_RETX_TB_TWO_RETX,
                                     dlStrategyTxNode_p,
                                     internalCellIndex);
            }
            else if (dlHarqProcess_p->isTBOneValid)
            {
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;

                retVal = lteMacStrategyDLNackTwoTBWithZeroQueueLoadForReTxFail(
                                     dlUEContext_p,
                                     dlHarqProcess_p,
                                     TB_ONE_RETX_TB_TWO_INVALID,
                                     dlStrategyTxNode_p,
                                     internalCellIndex);
            }
            else
            {
                dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator = RE_TX;

                retVal = lteMacStrategyDLNackTwoTBWithZeroQueueLoadForReTxFail(
                                     dlUEContext_p,
                                     dlHarqProcess_p,
                                     TB_ONE_INVALID_TB_TWO_RETX,
                                     dlStrategyTxNode_p,
                                     internalCellIndex);
            }
        }
        else
        {
            if (dlHarqProcess_p->isTBOneValid && !dlHarqProcess_p->isTBTwoValid)
            {
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;

                tempTBSizeRet = determineTBSize(
                                    dlCQIInfo_p->mcsIndexCodeWordTwo,
                                    (dlHarqProcess_p)->assignedResourcesNB,
                                    TB_MAPPED_TO_ONE_LAYER);

                if ((NET_UE_QL(dlUEContext_p) > ZERO) &&
                        (tempTBSizeRet > HARQ_TB_MINIMUM_SIZE_IN_BYTES))
                {
                    /* New_Tx can be schduled for another TB */
                    dlHarqProcess_p->isTBTwoValid = TRUE ;
                    dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator = NEW_TX;
                    *scheduleFlag_p = DL_QOS_ALREADY_TRANSMITED;

                    retVal = lteMacStrategyDLNackTwoTBWithNonZeroQueueLoadForReTxFail(
                                     dlUEContext_p,
                                     dlHarqProcess_p,
                                     TB_ONE_RETX_TB_TWO_NEW,
                                     dlStrategyTxNode_p,
                                     internalCellIndex);
                }
                else
                {
                    retVal = lteMacStrategyDLNackTwoTBWithZeroQueueLoadForReTxFail(
                                     dlUEContext_p,
                                     dlHarqProcess_p,
                                     TB_ONE_RETX_TB_TWO_INVALID,
                                     dlStrategyTxNode_p,
                                     internalCellIndex);
                }
            }
            else if (! (dlHarqProcess_p->isTBOneValid) &&
                       (dlHarqProcess_p->isTBTwoValid))
            {
                dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator = RE_TX;

                tempTBSizeRet = determineTBSize(
                                    dlCQIInfo_p->mcsIndexCodeWordOne,
                                    dlHarqProcess_p->assignedResourcesNB,
                                    TB_MAPPED_TO_ONE_LAYER);

                if ((NET_UE_QL(dlUEContext_p) > ZERO)&&
                        (tempTBSizeRet > HARQ_TB_MINIMUM_SIZE_IN_BYTES))
                {
                    /* New_Tx can be schduled for another TB */
                    dlHarqProcess_p->isTBOneValid = TRUE;
                    dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;
                    *scheduleFlag_p = DL_QOS_ALREADY_TRANSMITED;

                    retVal = lteMacStrategyDLNackTwoTBWithNonZeroQueueLoadForReTxFail(
                                     dlUEContext_p,
                                     dlHarqProcess_p,
                                     TB_ONE_NEW_TB_TWO_RETX,
                                     dlStrategyTxNode_p,
                                     internalCellIndex);
                }
                else
                {
                    retVal = lteMacStrategyDLNackTwoTBWithZeroQueueLoadForReTxFail(
                                     dlUEContext_p,
                                     dlHarqProcess_p,
                                     TB_ONE_INVALID_TB_TWO_RETX,
                                     dlStrategyTxNode_p,
                                     internalCellIndex);
                }
            }
            else
            {
                dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;
                dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator = RE_TX;

                retVal = lteMacStrategyDLNackTwoTBWithZeroQueueLoadForReTxFail(
                                     dlUEContext_p,
                                     dlHarqProcess_p,
                                     TB_ONE_RETX_TB_TWO_RETX,
                                     dlStrategyTxNode_p, 
                                     internalCellIndex);
            }
        }
    }
    else if (TX_MODE_6 == transMode)
    {
        /* Only one TB can be schduled. Second TB would be invalid */
        dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;

        if( (DCI_FORMAT_1A == dlHarqProcess_p->dciFormat) && 
            (DCI_FORMAT_1B == tm6DCIFormat))
        {
           dlHarqProcess_p->dciFormat = DCI_FORMAT_1B;
        }

        retVal = lteMacStrategyDLNackTwoTBWithZeroQueueLoadForReTxFail(
                                     dlUEContext_p,
                                     dlHarqProcess_p,
                                     TB_ONE_RETX_TB_TWO_INVALID,
                                     dlStrategyTxNode_p,
                                     internalCellIndex);
    }
    else
    {
        dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;

        retVal = lteMacStrategyDLNackForOneTBOnlyForReTxFail(
                                     dlUEContext_p,
                                     dlHarqProcess_p, 
                                     dlStrategyTxNode_p,
                                     internalCellIndex);

    }

    return retVal;
}


/*****************************************************************************
 * Function Name  : lteMacStrategyDLNackTwoTBWithNonZeroQueueLoadForReTxFail
 * Inputs         : ueDLContext_p - Pointer to UE Context
 *                  harqProcess_p - Pointer to UEs DL HARQ Context
 *                  tBOneTBTwoReTxNewInfo- information about the type of 
 *                     transmission of two TBs i.e. either New Transmission or Re-
 *                     transmissions
 *                  dlStrategyTxNode_p - Pointer to DL Strategy TX Node
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : Process for UE's DL Nack for Two TBs with Non Zero Queue Load
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : Its main function is for processing the UE for 
 *                  Re-Transmission for Two TBs when the UE  is having Non Zero 
 *                  Queue Load.One TB is used for the retransmission and other TB
 *                  is used for new transmission. The output is prepared  and 
 *                  sent to PDCCH Module
 * *****************************************************************************/
MacRetType lteMacStrategyDLNackTwoTBWithNonZeroQueueLoadForReTxFail( 
                                    DLUEContext* ueDLContext_p,
                                    DLHARQProcess *harqProcess_p,
                                    TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo,
                                    DLStrategyTxNode *dlStrategyTxNode_p,
                                    InternalCellIndex internalCellIndex)
{
    DLUeScheduleInfo * dlUeScheduleInfo_p = PNULL;
    UInt32 mcsThresholdLayer3 = 0;
    UInt32 mcsThresholdLayer4 = 0;
    UInt8  newTXNumOfLayer    = 0;
    UInt8  dlRi               = 0;
    UInt8  userLocationType = CC_USER;
    DLScheduleFlag*     scheduleFlag_p       = PNULL;
    DLCQIInfo*          dlCQIInfo_p          = PNULL;
    DLStrategyTxNode**   ueDlStrategyTxNode_p = PNULL;

        /*SPR 20899 Fix Start*/
    UInt8 strategyContainerIndex = MODULO_TWO(getCurrentTick()); 
        /*SPR 20899 Fix Stop*/

    if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
    {
        userLocationType = ueDLContext_p->userLocationType;
        scheduleFlag_p = &(ueDLContext_p->scheduleFlag);
        dlCQIInfo_p = &(ueDLContext_p->dlCQIInfo);
        ueDlStrategyTxNode_p = &(ueDLContext_p->dlStrategyTxNode_p);
        dlRi = ueDLContext_p->dlMIMOInfo.dlRi;
    }
    else
    {
        userLocationType = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
        scheduleFlag_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag);
        dlCQIInfo_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
        ueDlStrategyTxNode_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);
        dlRi = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.dlRi;
    }

    if (!(dlCQIInfo_p->aperiodicCQIRequested) &&
            ( (harqProcess_p->dlHARQTBOneInfo.current_TX_NB >=
               MAX_RETX_COUNT_FOR_STRICT_SUBBAND) || 
              (harqProcess_p->dlHARQTBTwoInfo.current_TX_NB >=
               MAX_RETX_COUNT_FOR_STRICT_SUBBAND)) &&
            (dlCQIInfo_p->cqiMode !=
             INVALID_APERIODIC_CQI_MODE) )
    {
        dlStrategyTxNode_p->isCQIRequestedULGrantMsg = TRUE;
        dlCQIInfo_p->aperiodicCQIRequested = TRUE;
    }

    switch (tBOneTBTwoReTxNewInfo){
        /*When TB One Retransmission to be done, TB Two can be used for the 
         *           New Transmission */
        case TB_ONE_RETX_TB_TWO_NEW:
            {

                harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize =
                    harqProcess_p->dlHARQTBOneInfo.tbSize;

                /* SPR 11355 Fix Start */
                if(((ueDLContext_p->ueCategory == UE_CAT_5) || (ueDLContext_p->ueCategory == UE_CAT_8)))
                    /* SPR 11355 Fix End */
                { 
                    mcsThresholdLayer3 = getMCSFromCQI( 
                            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer3[userLocationType]
                            );

                    mcsThresholdLayer4 = getMCSFromCQI(
                            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer4[userLocationType]
                            );


                    if((dlCQIInfo_p->mcsIndexCodeWordTwo > mcsThresholdLayer4) && 
                            (RANK_INDICATOR_4 == dlRi)) 
                    {
                        newTXNumOfLayer = RANK_INDICATOR_4;
                    }
                    else if((dlCQIInfo_p->mcsIndexCodeWordTwo > mcsThresholdLayer3) &&
                            (RANK_INDICATOR_3 == dlRi))
                    {
                        newTXNumOfLayer = RANK_INDICATOR_3;
                    }
                    else
                    {
                        newTXNumOfLayer = RANK_INDICATOR_2;
                    }

                    if( (newTXNumOfLayer >=RANK_INDICATOR_3) && 
                            (harqProcess_p->numOfLayer == RANK_INDICATOR_2) )
                    {
                        harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(
                                dlCQIInfo_p->mcsIndexCodeWordTwo,
                                harqProcess_p->assignedResourcesNB,
                                TB_MAPPED_TO_TWO_LAYER);
                        harqProcess_p->numOfLayer++;
                    }
                    else if(newTXNumOfLayer >=RANK_INDICATOR_3 && 
                            harqProcess_p->numOfLayer == RANK_INDICATOR_3)
                    {
                        harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(
                                dlCQIInfo_p->mcsIndexCodeWordTwo,
                                harqProcess_p->assignedResourcesNB,
                                TB_MAPPED_TO_TWO_LAYER);
                    }
                    else if( (harqProcess_p->numOfLayer == RANK_INDICATOR_4) && 
                            (newTXNumOfLayer < RANK_INDICATOR_3))
                    {
                        harqProcess_p->isTBTwoValid = FALSE;
                        /* reset schedule Flag as only retx can be done */
                        *scheduleFlag_p = DL_QOS_NO_NEW_TRANSMISSION;
                    }  
                    else if(harqProcess_p->numOfLayer == RANK_INDICATOR_4 && 
                            newTXNumOfLayer >= RANK_INDICATOR_3)
                    {
                        harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(
                                dlCQIInfo_p->mcsIndexCodeWordTwo,
                                harqProcess_p->assignedResourcesNB,
                                TB_MAPPED_TO_TWO_LAYER);
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(
                                dlCQIInfo_p->mcsIndexCodeWordTwo,
                                harqProcess_p->assignedResourcesNB,
                                TB_MAPPED_TO_ONE_LAYER);     
                        harqProcess_p->numOfLayer = RANK_INDICATOR_2;   
                    }
                }   
                else
                {
                    harqProcess_p->dlHARQTBTwoInfo.tbSize = determineTBSize(
                            dlCQIInfo_p->mcsIndexCodeWordTwo,
                            harqProcess_p->assignedResourcesNB,
                            TB_MAPPED_TO_ONE_LAYER);
                }                
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex = 
                    dlCQIInfo_p->mcsIndexCodeWordTwo; 
            }
            break;
            /*When TB Two Retransmission to be done, TB One can be used 
             *               for the New Transmission */
        case TB_ONE_NEW_TB_TWO_RETX:
            {

                harqProcess_p->dlHARQTBTwoInfo.retransmissionTBSize =
                    harqProcess_p->dlHARQTBTwoInfo.tbSize;
                /* SPR 11355 Fix Start */
                if(((ueDLContext_p->ueCategory == UE_CAT_5) || (ueDLContext_p->ueCategory == UE_CAT_8)))
                    /* SPR 11355 Fix End */
                {
                    mcsThresholdLayer3 = getMCSFromCQI( 
                            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer3[userLocationType]
                            );

                    mcsThresholdLayer4 = getMCSFromCQI(
                            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer4[userLocationType]
                            );


                    if((dlCQIInfo_p->mcsIndexCodeWordOne > mcsThresholdLayer4) &&
                            (RANK_INDICATOR_4 == dlRi))
                    {
                        newTXNumOfLayer = RANK_INDICATOR_4;
                    }
                    else if((dlCQIInfo_p->mcsIndexCodeWordOne > mcsThresholdLayer3) &&
                            (RANK_INDICATOR_3 == dlRi))
                    {
                        newTXNumOfLayer = RANK_INDICATOR_3;
                    }
                    else
                    {
                        newTXNumOfLayer = RANK_INDICATOR_2;
                    }

                    if(newTXNumOfLayer == RANK_INDICATOR_4 && 
                            harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                    {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(
                                dlCQIInfo_p->mcsIndexCodeWordOne,
                                harqProcess_p->assignedResourcesNB,
                                TB_MAPPED_TO_TWO_LAYER);
                        harqProcess_p->numOfLayer = newTXNumOfLayer;
                    }
                    else if(newTXNumOfLayer == RANK_INDICATOR_3 && 
                            harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                    {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(
                                dlCQIInfo_p->mcsIndexCodeWordOne,
                                harqProcess_p->assignedResourcesNB,
                                TB_MAPPED_TO_ONE_LAYER);
                        harqProcess_p->numOfLayer = newTXNumOfLayer;
                    }
                    else if(newTXNumOfLayer == RANK_INDICATOR_2 && 
                            harqProcess_p->numOfLayer == RANK_INDICATOR_4)
                    {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(
                                dlCQIInfo_p->mcsIndexCodeWordOne,
                                harqProcess_p->assignedResourcesNB,
                                TB_MAPPED_TO_ONE_LAYER);
                        harqProcess_p->numOfLayer = RANK_INDICATOR_3;
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(
                                dlCQIInfo_p->mcsIndexCodeWordOne,
                                harqProcess_p->assignedResourcesNB,
                                TB_MAPPED_TO_ONE_LAYER);
                    }
                }
                else
                {
                    harqProcess_p->dlHARQTBOneInfo.tbSize = determineTBSize(
                            dlCQIInfo_p->mcsIndexCodeWordOne,
                            harqProcess_p->assignedResourcesNB,
                            TB_MAPPED_TO_ONE_LAYER);
                    harqProcess_p->numOfLayer = RANK_INDICATOR_2;
                }


                harqProcess_p->dlHARQTBOneInfo.mcsIndex = 
                    dlCQIInfo_p->mcsIndexCodeWordOne;
                totalRbMIMO_g[internalCellIndex] += harqProcess_p->assignedResourcesNB;
            }
            break;
        default:
            {
                /* reset schedule Flag as only retx can be done */
                *scheduleFlag_p = DL_QOS_NO_NEW_TRANSMISSION;

                LTE_MAC_UT_LOG(LOG_WARNING,DL_RR_STRATEGY,
                        "[DL-Strategy][%s]TBOneTBTwoReTxNewInfo"
                        " choice is INVALID", __func__);
                return MAC_FAILURE;
            }
    }

    /* Update the minTBSize and maxTBSize for QOS Scheduling */
    dlStrategyTxNode_p->minTBSize = 
        harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize +
        harqProcess_p->dlHARQTBTwoInfo.retransmissionTBSize;

    dlStrategyTxNode_p->maxTBSize = dlStrategyTxNode_p->minTBSize;

    dlStrategyTxNode_p->harqProcess_p = harqProcess_p; 
    if ( (DL_QOS_BASED_SCHEDULING == 
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->dlSchdConfig.dlSchedulerStrategy) &&
            (*scheduleFlag_p == DL_QOS_ALREADY_TRANSMITED))
    {
        /* Update dlStrategyScheduleInfo_g as UE is considered for 
         * New_Tx on either TB. 
         * Here Strategy transmissionType is set as NEW_TX 
         * which will be updated once UE gets schduled through QoS strategy.
         * Same strategy node would be pushed in SchedulerReTxQueue */
        dlUeScheduleInfo_p = &(dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].dlUeScheduleInfo
                [dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount]);
        dlUeScheduleInfo_p->ueDLContext_p = ueDLContext_p;
        dlStrategyTxNode_p->transmissionType  = NEW_TX;

        dlUeScheduleInfo_p->dLStrategyTxNode_p = dlStrategyTxNode_p;
        dlUeScheduleInfo_p->harqProcess_p = harqProcess_p;
        dlUeScheduleInfo_p->isTaDrxScheduled = INVALID_TA_DRX_MSG;

        /* Increament the scheduled UE count */
        dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount++; 
        *ueDlStrategyTxNode_p = dlStrategyTxNode_p;
        return MAC_SUCCESS;

    }

    if (MAC_FAILURE == putEntryInDLSchedulerReTxQueue(dlStrategyTxNode_p,internalCellIndex))
    {
        freeDLHarqProcess (ueDLContext_p, harqProcess_p,internalCellIndex);
        freeMemPool(dlStrategyTxNode_p);
        dlStrategyTxNode_p = PNULL;	
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : lteMacStrategyDLNackForOneTBOnlyForReTxFail 
 * Inputs         : ueDLContext_p - pointer to DL UE context
 *                  harqProcess_p - pointer to Harq process whose transmission
 *                                  needs to be done
 *                  dlStrategyTxNode_p - Pointer to DL TX Strategy Node
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : availableRB_p - Leftover RBs
 * Returns        : None 
 * Description    : This function processes the UE for Re-Transmission for
 *                  single TB. The output is prepared  and sent to PDCCH Module 
 *****************************************************************************/
 MacRetType lteMacStrategyDLNackForOneTBOnlyForReTxFail(
                            DLUEContext* ueDLContext_p, 
                            DLHARQProcess *harqProcess_p, 
                            DLStrategyTxNode *dlStrategyTxNode_p,
                            InternalCellIndex internalCellIndex) 
{
    harqProcess_p->isTBOneValid = TRUE;
    harqProcess_p->isTBTwoValid = FALSE;

    dlStrategyTxNode_p->harqProcess_p = harqProcess_p;                 

    if( (IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex)) && 
        (SPS_SCHEDULING == ueDLContext_p->schType) )
    {
        dlStrategyTxNode_p->msgType = SPS_RETX;
    }
    else
    {
        dlStrategyTxNode_p->msgType = NON_SPATIAL_MUL;
    }

    if (!(putEntryInDLSchedulerReTxQueue(dlStrategyTxNode_p ,internalCellIndex)))
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DL_STRATEGY,
                "[DL-Strategy][%s]dlStrategyTxNode_p Entry is failed\n",
                __func__);
        freeDLHarqProcess (ueDLContext_p, harqProcess_p,internalCellIndex);
        freeMemPool(dlStrategyTxNode_p);
        dlStrategyTxNode_p = PNULL;	
        return MAC_FAILURE;
    }
    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED(
            ueDLContext_p, harqProcess_p, dlStrategyTxNode_p,
            internalCellIndex);
/* CA Stats Changes End */

    return MAC_SUCCESS;
}


/*****************************************************************************
 * * Function Name  : lteMacStrategyDLNackTwoTBWithZeroQueueLoadForReTxFail 
 * * Inputs         : ueDLContext_p - Pointer to UE Context
 *                    harqProcess_p - Pointer to UEs HARQ Context
 *                    tBOneTBTwoReTxNewInfo- information about the type of
 *                       transmission of two TBs i.e. either New Transmission or 
 *                       Re- transmissions        
 *                    dlStrategyTxNode_p - Pointer to DL TX Strategy Node
 *                    internalCellIndex - cell index used at MAC
 * * Outputs        : Process for UE's DL Nack for Two TBs with Zero Queue Load 
 * * Returns        : MAC_SUCCESS/MAC_FAILURE
 * * Description    : Its main function is for processing the UE for 
 *                    Re-Transmission for Two TBs when the UE  is having Zero 
 *                    Queue Load.One TB is used for the Retransmision and other
 *                    TB is made INVALID.The output is prepared  and sent to 
 *                    PDCCH Module
 *****************************************************************************/
MacRetType lteMacStrategyDLNackTwoTBWithZeroQueueLoadForReTxFail( 
                                DLUEContext* ueDLContext_p,
                                DLHARQProcess *harqProcess_p,
                                TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo,
                                DLStrategyTxNode *dlStrategyTxNode_p,
                                InternalCellIndex internalCellIndex)
{
    DLCQIInfo* dlCQIInfo_p = PNULL;
    if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
    {
        dlCQIInfo_p = &(ueDLContext_p->dlCQIInfo);
    }
    else
    {
        dlCQIInfo_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
    }


    if (!(dlCQIInfo_p->aperiodicCQIRequested)&&
         ( (harqProcess_p->dlHARQTBOneInfo.current_TX_NB >= 
                        MAX_RETX_COUNT_FOR_STRICT_SUBBAND) || 
           (harqProcess_p->dlHARQTBTwoInfo.current_TX_NB >= 
                        MAX_RETX_COUNT_FOR_STRICT_SUBBAND)) &&
         (dlCQIInfo_p->cqiMode != INVALID_APERIODIC_CQI_MODE))
    {
        dlStrategyTxNode_p->isCQIRequestedULGrantMsg = TRUE;
        dlCQIInfo_p->aperiodicCQIRequested = TRUE;
    }

    if ( TB_ONE_RETX_TB_TWO_RETX == tBOneTBTwoReTxNewInfo ) 
    {
        harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize =
                    harqProcess_p->dlHARQTBOneInfo.tbSize;
        harqProcess_p->dlHARQTBTwoInfo.retransmissionTBSize =
                    harqProcess_p->dlHARQTBTwoInfo.tbSize;
    }
    else 
    {
        switch (tBOneTBTwoReTxNewInfo){
            case TB_ONE_RETX_TB_TWO_INVALID:
                {
                    harqProcess_p->dlHARQTBOneInfo.retransmissionTBSize =
                                harqProcess_p->dlHARQTBOneInfo.tbSize;
                }
                break;
            case TB_ONE_INVALID_TB_TWO_RETX:
                {
                    harqProcess_p->dlHARQTBTwoInfo.retransmissionTBSize =
                                harqProcess_p->dlHARQTBTwoInfo.tbSize;
                }
                break;
            default:
                {
                    freeMemPool(dlStrategyTxNode_p);
                    dlStrategyTxNode_p = PNULL;	
                    return MAC_FAILURE;
                }
        }
    }
    dlStrategyTxNode_p->harqProcess_p = harqProcess_p;

    if (MAC_FAILURE == putEntryInDLSchedulerReTxQueue(dlStrategyTxNode_p, internalCellIndex))
    {
        freeDLHarqProcess (ueDLContext_p, harqProcess_p,internalCellIndex);
        freeMemPool(dlStrategyTxNode_p); 
        dlStrategyTxNode_p = PNULL;	
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;

}
/* - Rel_6_0_reTx_Changes */

/*****************************************************************************
 * Function Name  : dlQosPreparePreferredCellListForReTx 
 * Inputs         : dlUEContext_p 
 *                  dlReTxNode_p
 * Outputs        : 
 * Returns        : 
 * Description    : 
 *****************************************************************************/
 void dlQosPreparePreferredCellListForReTx( DLUEContext* dlUEContext_p, 
                                                  DLRetxQueueNode* dlReTxNode_p)
{
    DLHARQEntity      *dlHarqEntity_p  = PNULL;
    InternalCellIndex pCellIndex       = dlUEContext_p->internalCellIndex;
    InternalCellIndex sCellIndex       = 0; 

    UInt8 listIndex = 0;
    UInt8 scellReTxCount = 0;
    UInt8 sCellHarqProcessId = INVALID_HARQ_ID;
    UInt8 preferredListCount = dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount;

    for(listIndex = 0; listIndex < preferredListCount; listIndex++)
    {
        if(dlUEContext_p->ueIndex == 
                dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].ueIndex)
        {
            /* Preferred cell list already exist for this UE */
            return ;
        }
    }

    listIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount;   

    /* First check retx for Pcell of UE*/
    if(INVALID_HARQ_ID != dlReTxNode_p->harqProcessId)
    {
        dlHarqEntity_p = dlUEContext_p->dlHarqContext_p;
        addCellInPrefListForReTx( dlHarqEntity_p, 
                                  dlReTxNode_p->harqProcessId, 
                                  pCellIndex, 
                                  pCellIndex);
    }
    /* Check retx for Scell of UE*/
    for (scellReTxCount = 0; scellReTxCount < dlReTxNode_p->sCellCount; scellReTxCount++)
    {
        sCellIndex = getInternalCellIdFromServeCellId( 
                                dlUEContext_p, 
                                dlReTxNode_p->sCellRetxInfo[scellReTxCount].servCellIndex);

        /* + SPR_14522 */
        if (INVALID_8_VAL == sCellIndex)
        {
            continue;
        }    
        /* - SPR_14522 */
        dlHarqEntity_p = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p;
        sCellHarqProcessId = dlReTxNode_p->sCellRetxInfo[scellReTxCount].harqProcessId;

        addCellInPrefListForReTx( dlHarqEntity_p, 
                                  sCellHarqProcessId,
                                  pCellIndex, 
                                  sCellIndex);
    }
    /* Both TB NEW_TX case on Pcell of UE*/
    if(INVALID_HARQ_ID == dlReTxNode_p->harqProcessId)
    {
        dlQosAddCellInPreferredCellList(pCellIndex, pCellIndex);
    }
    /* Both TB NEW_TX case on Scell of UE*/
    /* Currently add only one scell. Need to modify later for multi cells. */
    if( (IS_CA_ENABLED()) &&
        /*SPR 21948 Changes Start */
         /* Changes under spr 21762 have been reverted */
          /*SPR 21948 Changes End */
        (isUeEligibleForCaScheduling(dlUEContext_p)) &&
        (0 == dlReTxNode_p->sCellCount))
    {
        sCellIndex = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex; 
        dlQosAddCellInPreferredCellList(pCellIndex, sCellIndex);
    }

    /* Add UE in Preferred cell list only if having any schedulable cell */
    if(dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].count)
    {
        /* Preference need to take care during opt */
        dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].isPrefRequired = FALSE;
        dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].ueIndex = dlUEContext_p->ueIndex;
        dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount++;
    }
    LOG_MAC_MSG(MAC_PREF_CELL_LIST_FOR_RETX,LOGDEBUG,MAC_DL_Strategy,
            getCurrentTick(),
            dlUEContext_p->ueIndex,
            pCellIndex,
            dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].count,
            dlReTxNode_p->harqProcessId,
            dlReTxNode_p->sCellCount,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME, "");
}

/*****************************************************************************
 * Function Name  : addCellInPrefListForReTx 
 * Inputs         : dlHarqEntity_p
 *                  harqProcessId
 *                  pCellIndex
 *                  cellIdTobeAdded
 *                  preferredCellListIndex
 * Outputs        : 
 * Returns        : 
 * Description    : 
 *****************************************************************************/
 void  addCellInPrefListForReTx( DLHARQEntity* dlHarqEntity_p,
                                       UInt8 harqProcessId,
                                       InternalCellIndex pCellIndex, 
                                       InternalCellIndex cellIdTobeAdded)
{
    DLHARQProcess* harqProcess_p = &(dlHarqEntity_p->harqProcess[harqProcessId]);

    if( ( (harqProcess_p->isTBOneValid) &&
          (NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator))||
        ( (harqProcess_p->isTBTwoValid) &&
          (NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)))  
    {
        dlQosAddCellInPreferredCellList(pCellIndex, cellIdTobeAdded);
    }
}
/*****************************************************************************
 * Function Name  : isNodeValidForRetransmissionOnCell
 * Inputs         : ueIndex - Index of UE.
 *                  numberOfAvailableRBs_p - pointer to number of avaliable RB's
 *                  deleteNode - flag to delete node or not 
 *                  resourceRegionAllocated_p - Pointer to resouce region allocated
 *                  globalTick - current global tick
 *                  dlDelay - delay in DL path to phy
 *                  internalCellIndex - cell index used at MAC
 *                  dlHarqProcess_p - pointer to the harq process 
 *                  maxUEsToBeScheduledDL - 
 * Outouts        : deleteNode - flag to delete node or not
 * Description    : This function check if the UE retransmission is possible
 *                  in current tick.
 *****************************************************************************/
 MacRetType isNodeValidForRetransmissionOnCell(
                                        UInt16 ueIndex,
                                        UInt32*numberOfAvailableRBs_p,
                                        UInt8 *deleteNode,
                                        UInt8 *resourceRegionAllocated_p,
                                        /*SPR 15909 fix start*/
                                        tickType_t globalTick
                                        /*SPR 15909 fix end*/
                                        #ifdef FDD_CONFIG
                                        ,UInt32 dlDelay
                                        #endif
                                        ,InternalCellIndex internalCellIndex,
                                        DLHARQProcess *dlHarqProcess_p,
                                        UInt8 maxUEsToBeScheduledDL)
{

    DLUEContext      *dlUEContext_p = PNULL;
    DLUEContextInfo  *dlUEContextInfo_p = PNULL;
    LP_SpsDlInfo     spsDlInfo_p       = PNULL;
    UInt8            userLocationType = CC_USER;
#ifdef TDD_CONFIG
    UInt8 currentSubframeType = 0;
    /* CA Changes Start */
    currentSubframeType = getSubframeType((globalTick % 10), internalCellIndex);
    /* CA Changes End */ 
#endif
    DLScheduleFlag scheduleFlag = DL_QOS_NEW_TRANSMISSION;
    DLStrategyTxNode *dlStrategyTxNode_p = PNULL;
    TransmissonMode     transmissionMode;
#ifdef LOG_PRINT_ENABLED
    UInt32 state = 0;
#endif

    dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

    if(0 == maxUEsToBeScheduledDL)
    {
        /* SPR 11912 Changes Start */
        /* + SPR 14341 Changes */
        /*  *deleteNode = TRUE; */
        /* - SPR 14341 Changes */
        /* SPR 11912 Changes End */
        return MAC_FAILURE;
    }
    if(IS_PCELL_OF_UE(dlUEContext_p, internalCellIndex))
    {
        /*! \code
         * If Retx for a SPS UE overlaps with SPS Occasion 
         *   Do not schedule that UE for Retx.
         * \endcode   
         * */
        if ( SPS_SCHEDULING == dlUEContext_p->schType )
        {
            return MAC_FAILURE;
        }
        userLocationType = dlUEContext_p->userLocationType;
        scheduleFlag = dlUEContext_p->scheduleFlag;
        dlStrategyTxNode_p = dlUEContext_p->dlStrategyTxNode_p;
        transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode; 

    }
    else
    {
        userLocationType = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
        scheduleFlag = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag;
        dlStrategyTxNode_p = dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p;
        transmissionMode =
            dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.transmissionMode;
    }

    if ((dlHarqProcess_p->dciFormat >= MAX_NUM_DCI_FORMAT) ||
            (HARQ_PROCESS_RESET == dlHarqProcess_p->harqStatus))
    {
        freeDLHarqProcess(dlUEContext_p, dlHarqProcess_p,internalCellIndex);
        *deleteNode = TRUE;
        return MAC_FAILURE;
    }
    /*! \code
     * Check if sync loss is detected for this UE
     *   Free the HARQ process
     *   Send trigger to SPS scheduler to change state to UE_NOT_READY.
     * \endcode   
     * */
    spsDlInfo_p = &dlUEContext_p->spsDlInfo;
    if ( (SPS_SETUP == spsDlInfo_p->requestType && 
/*SPR 20008 Fix Start*/
	(INVALID_STATE != spsDlInfo_p->spsState)) && 
/*SPR 20008 Fix End*/
            (dlHarqProcess_p->harqProcessId < 
             spsDlInfo_p->spsDlSetupInfo_p->numberOfConfSpsProcesses) )
    {
        if( UL_SYNC_LOSS_ON == checkUlSyncLossForUE( dlUEContext_p,
                    globalTick ,internalCellIndex) )
        {
            /* free the HARQ process for SPS NEW_TX */
            freeDLHarqProcessTBSpecific( dlUEContext_p, dlHarqProcess_p,
                    TB_ONE, internalCellIndex);

#ifdef LOG_PRINT_ENABLED
            state = spsDlInfo_p->spsState;
#endif
            /* Change the SPS state */
            dlStateMachine[spsDlInfo_p->spsState][UE_SYNC_LOSS_ON](
                    dlUEContext_p->ueIndex, spsDlInfo_p->spsState, 
                    UE_SYNC_LOSS_ON, dlUEContext_p, 
                    spsDlInfo_p->sdlOccNode_p,
                    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                    dlDelay
#else
                    DL_ADVANCE_TICK
#endif
                    /* - SPS_TDD_Changes */
                    ,internalCellIndex);

            LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, dlUEContext_p->ueIndex, state, UE_SYNC_LOSS_ON,
                     spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");

            *deleteNode = TRUE;
            return MAC_FAILURE;

        }
    }
    if((DL_QOS_BASED_SCHEDULING == 
                (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
                 initParams_p->dlSchdConfig.dlSchedulerStrategy)) && 
            ((DL_QOS_NO_NEW_TRANSMISSION >= scheduleFlag)|| (DL_QOS_ALREADY_TRANSMITED == scheduleFlag))) //CASA_CA_Changes
    {
        return MAC_FAILURE;
    }
    /* If Harq TTI is not expired, Retransmission cant be done 
       so go to the next node */
    if (globalTick < dlHarqProcess_p->dlharqRttExpiryTick
            || dlStrategyTxNode_p)
    {
        return MAC_FAILURE;
    }

    /* If the drxRetransmission timer is expired or
     * assignedResourcesNB are not enough:
     */
    {
        if ( ( (SETUP == dlUEContext_p->drxConfigType) && 
                    (globalTick > dlHarqProcess_p->drxRetxExpiryTTI)) ||
                (dlHarqProcess_p->assignedResourcesNB >
                 (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB))
        {
            freeDLHarqProcess(dlUEContext_p, dlHarqProcess_p,internalCellIndex);
            *deleteNode = TRUE;
            return MAC_FAILURE;
        }
    }
#ifdef TDD_CONFIG
    if(dlHarqProcess_p->subFrameType != currentSubframeType)
    {
        LOG_MAC_MSG (DRX_RETX_UE_NOT_SCHEDULED_LOG_ID, LOGDEBUG,
                MAC_DL_Strategy, getCurrentTick (), ueIndex,
                dlHarqProcess_p->harqProcessId,
                dlUEContext_p->isAlreadyConsideredFlag,
                dlUEContextInfo_p->pendingDeleteFlag,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE, FUNCTION_NAME, "");
        LOG_MAC_MSG(MAC_RETX_UE_SF_MISMATCH,
                LOGDEBUG, MAC_DL_Strategy,getCurrentTick(),
                currentSubframeType,
                dlHarqProcess_p->subFrameType,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"SUBFRAME TYPE MISMATCH");
        return MAC_FAILURE;
    }
#endif 
    if (( CE_USER == userLocationType ) && 
            ( maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] <= 0 ))
    {
        return MAC_FAILURE;
    }

    /* SPR 11403 fix start */
    if(MAC_SUCCESS != validateTxModeAndDciFormat(transmissionMode,
                dlHarqProcess_p->dciFormat))
    {
        *deleteNode = TRUE;
        return MAC_FAILURE;

    }
    /* SPR 11403 fix end */

    return (validateCcCeRBForRetx(dlHarqProcess_p,
                numberOfAvailableRBs_p,
                userLocationType,
                resourceRegionAllocated_p,
                globalTick,
                internalCellIndex));
}

/*****************************************************************************
 * Function Name  : dlQosPreparePreferredCellListForReTxFail 
 * Inputs         : ueDLContext_p 
 *                  cellIdTobeAdded
 * Outputs        : 
 * Returns        : 
 * Description    : 
 *****************************************************************************/
 void dlQosPreparePreferredCellListForReTxFail( DLUEContext* ueDLContext_p, 
                                                      InternalCellIndex cellIdTobeAdded)
{
    UInt8 listIndex = 0;
    UInt16 ueIndex = ueDLContext_p->ueIndex;
    InternalCellIndex pCellIndex = ueDLContext_p->internalCellIndex;
    
    UInt8 preferredListCount = dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount;
    for(listIndex = 0; listIndex < preferredListCount; listIndex++)
    {
        if(ueIndex == dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].ueIndex)
        {
            /* Preferred cell list already exist for this UE */
            return ;
        }
    }
    listIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount;   
    dlQosAddCellInPreferredCellList(pCellIndex, cellIdTobeAdded);
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */
    if( IS_CA_ENABLED() && isUeEligibleForCaScheduling(ueDLContext_p))
    {
        /* Add remaining cells in list */
        dlQosAddCellInPreferredCellList(pCellIndex, (!cellIdTobeAdded));
    }
    /* Preference need to take care during opt */
    dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].isPrefRequired = FALSE;
    dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].ueIndex = ueIndex;
    dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount++;

    LOG_MAC_MSG(MAC_PREF_CELL_LIST_FOR_RETX_FAIL,LOGDEBUG,MAC_DL_Strategy,
            getCurrentTick(),
            ueDLContext_p->ueIndex,
            pCellIndex,
            cellIdTobeAdded,
            dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].count,
            dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME, "");
}
/* Cyclomatic_complexity_changes_start */
/*****************************************************************************
 * Function Name  : validateUeForScheduling
 * Inputs         : drxContext_p
                    dlUEContext_p
                    measgapContext_p
 * Outputs        : tempLenth
 * Returns        : None.
 * Description    : This function validates ue for scheuling
 ******************************************************************************/
UInt32 validateUeForScheduling(DRXContext *drxContext_p, DLUEContext *dlUEContext_p, MeasGapContextDL *measgapContext_p)
{
	if ( ((SETUP == dlUEContext_p->drxConfigType) &&
				(DRX_STATE_ON == CHECK_UE_DRX_STATE(drxContext_p)))
			|| (TRUE == CHECK_UE_MEAS_GAP_PERIOD(measgapContext_p)
				|| (SPS_SCHEDULING == dlUEContext_p->schType) ))
		return MAC_SUCCESS;
	else
		return MAC_FAILURE;
}
/*****************************************************************************
 * Function Name  : getTempLength
 * Inputs         : msgType
 * Outputs        : tempLenth
 * Returns        : None
 * Description    : This function calculate length of particular msgType
 ******************************************************************************/
UInt32 getTempLength(MsgType msgType)
{
        UInt32 tempLength = 0;
        switch (msgType){
            case TA_MSG:
                {
                    tempLength = LENGTH_OF_TA;
                    break;
                }
            case DRX_MSG:
                {
                    tempLength = LENGTH_OF_DRX;
                    break;
                }
            case TA_DRX_MSG:
                {
                    tempLength = LENGTH_OF_TA_DRX;
                    break;
                }
            default:
                {
                    break;
                }
        }
        return tempLength;
}
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : checkSplSfAndCalcTbsizeSiso
 * Inputs         : isSpecialSubframe - If the Subframe is SPL subframe
 *                  mcsIndex
 *                  allocatedRB
 * Outputs        : tbSize
 * Returns        : None.
 * Description    : This function calculate mcsIndex and then TB size
 ****************************************************************************/
void checkSplSfAndCalcTbsizeSiso(   UInt8  isSpecialSubframe,
                        	    UInt8  mcsIndex,
                                    UInt8  allocatedRB,
                                    UInt32 *tbSize
                                )
{
    if ( isSpecialSubframe )
    {
        /*4X4 DL MIMO CHG START*/
        /* SPR 22169 fix start */
        /* Logic to calculate IBS index from MCS Index is removed. 
         * since the calculation for IBS index is already done in function
         * determineRBs()
         */
        *tbSize = determineTBSize(mcsIndex , SP_SUBFRAME_PRB(allocatedRB),TB_MAPPED_TO_ONE_LAYER );
        /* SPR 22169 fix end */
    }
    else
    {
        *tbSize = determineTBSize(mcsIndex , allocatedRB,TB_MAPPED_TO_ONE_LAYER );
    }
    /*4X4 DL MIMO CHG END*/
}
/*****************************************************************************
 * Function Name  : checkSplSfAndCalcTbsizeMimo
 * Inputs         : isSpecialSubframe - If the Subframe is SPL subframe
 *                  mcsIndex
 *                  allocatedRB
 *                  numOfLayer
 * Outputs        : tbOneSize,tbTwoSize
 * Returns        : None.
 * Description    : This function calculate mcsIndex and then TB size
 ******************************************************************************/
void checkSplSfAndCalcTbsizeMimo(   UInt8        isSpecialSubframe,
		DLUEContext* ueDLContext_p,
		UInt8        allocatedRB,
		UInt32       *tbOneSize,
		UInt32       *tbTwoSize,
		UInt8        numOfLayer)
{
	if ( isSpecialSubframe )
	{
		/* SPR 22169 fix start */
		/* Logic to calculate IBS index from MCS Index is removed. 
		 * since the calculation for IBS index is already done in function
		 * determineRBs()
		 */
		/*4X4 DL MIMO CHG START */
		if(numOfLayer == RANK_INDICATOR_4)
		{
			*tbOneSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
					SP_SUBFRAME_PRB(allocatedRB) ,TB_MAPPED_TO_TWO_LAYER);
			*tbTwoSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo,
					SP_SUBFRAME_PRB(allocatedRB),TB_MAPPED_TO_TWO_LAYER);
		}
		else if (numOfLayer == RANK_INDICATOR_3)
		{
			*tbOneSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
					SP_SUBFRAME_PRB(allocatedRB) ,TB_MAPPED_TO_ONE_LAYER);
			*tbTwoSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo,
					SP_SUBFRAME_PRB(allocatedRB),TB_MAPPED_TO_TWO_LAYER);
		}
		else
		{
			*tbOneSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
					SP_SUBFRAME_PRB(allocatedRB) ,TB_MAPPED_TO_ONE_LAYER);
			*tbTwoSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo,
					SP_SUBFRAME_PRB(allocatedRB),TB_MAPPED_TO_ONE_LAYER);
		}
		/* SPR 22169 fix end */
		/*4X4 DL MIMO CHG END */
	}
	/* SPR 10144 fix end*/
	else
	{
		/*4X4 DL MIMO CHG START */
		if(numOfLayer == RANK_INDICATOR_4)
		{
			*tbOneSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
					allocatedRB ,TB_MAPPED_TO_TWO_LAYER);
			*tbTwoSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo,
					allocatedRB,TB_MAPPED_TO_TWO_LAYER);
		}
		else if (numOfLayer == RANK_INDICATOR_3)
		{
			*tbOneSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
					allocatedRB ,TB_MAPPED_TO_ONE_LAYER);
			*tbTwoSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo,
					allocatedRB,TB_MAPPED_TO_TWO_LAYER);
		}
		else
		{
			*tbOneSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
					allocatedRB ,TB_MAPPED_TO_ONE_LAYER);
			*tbTwoSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo,
					allocatedRB,TB_MAPPED_TO_ONE_LAYER);
		}
		/*4X4 DL MIMO CHG END */
	}
}
#endif
/*****************************************************************************
 * Function Name  : pushInPendingQueueAsPerMsgType
 * Inputs         : msgType
 *                  ueDLContext
 *                  internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function the msg in pnding queue as pr it's type.
 *******************************************************************************/

void  pushInPendingQueueAsPerMsgType(MsgType msgType,
                                     /* +- SPR 17777 */
                                     DLUEContext * ueDLContext,
                                     InternalCellIndex internalCellIndex)
{
	if(TA_MSG == msgType || TA_DRX_MSG == msgType)
	{
		TimeAdvanceQueueNode *taNode_p = PNULL;
		LOG_UT(MAC_RR_RB_NOT_AVAILABLE_PUSHING_IN_TA_Q,LOGDEBUG,MAC_DL_Strategy,
				getCurrentTick(),
                /* +- SPR 17777 */
				0,ueDLContext->ueIndex,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,LINE_NUMBER,
				DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,"");
		GET_MEM_NODE_TA_Q(taNode_p,TimeAdvanceQueueNode);
		if(PNULL != taNode_p)
		{
			taNode_p->ueIdx = ueDLContext->ueIndex;
			taNode_p->timeAdvanceValue = ueDLContext->timingAdvanceValue;
			if(!(ENQUEUE_TA_Q(pendingTAQueue_gp[internalCellIndex],TimeAdvanceQueueNode,&(taNode_p->timeAdvanceQueueNodeAnchor))))
			{
				FREE_MEM_NODE_TA_Q(taNode_p);
			}
		}
	}
	else if(TPC_WITH_DATA == msgType)
	{
		ExplicitDLDCIQueueNode * dlDciNode_p = PNULL;
		GET_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(
				dlDciNode_p,ExplicitDLDCIQueueNode);
		if(PNULL != dlDciNode_p)
		{
			dlDciNode_p->ueIndex = ueDLContext->ueIndex;
            if(!(ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],
                            ExplicitDLDCIQueueNode, &(dlDciNode_p->nodeAnchor))))
            {
                FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(dlDciNode_p);
            }
		}
	}

    return;
}
/*****************************************************************************
 * Function Name  : CleanupSPSStrategyQueues
 * Inputs         : internalCellIndex
 *                  dlSpsNode_p               
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up SPS strategy queues.
 ********************************************************************************/
void CleanupSPSStrategyQueues( InternalCellIndex internalCellIndex, void *dlSpsNode_p)
{
	/*! * \code
	 ** Cleans up SPS strategy queues in following order:
	 ** 1. 1. Staus queue, dlSpsStatusQ_g
	 ** 2. Deactivation queue, dlSpsDeactQ_g
	 ** 3. Activation queue, dlSpsActQ_g
	 ** 4. Reeactivation queue, dlSpsReactQ_g
	 ** 5. UE exclude list, dlSpsUeExclude_g
	 ** 6. Resource reserve request queue, dlSpsResRsvQ_g
	 ** 7. Resource reserve release queue, dlSpsResRelQ_g
	 ** \endcode
	 ** */
	/* Cleans DL Status queue */
	do
	{
		dlSpsNode_p = PNULL;
		DEQUEUE_MAC_DL_SPS_Q ( dlSpsStatusQ_gp[internalCellIndex], dlSpsNode_p );
		if ( PNULL != dlSpsNode_p )
		{
			freeMemPool( dlSpsNode_p);
		}
	}
	while ( PNULL != dlSpsNode_p);
	/* Cleans DL Deactivation queue */
	do
	{
		dlSpsNode_p = PNULL;
		DEQUEUE_MAC_DL_SPS_Q ( dlSpsDeactQ_gp[internalCellIndex], dlSpsNode_p );
		if ( PNULL != dlSpsNode_p )
		{
			freeMemPool( dlSpsNode_p);
		}
	}
	while ( PNULL != dlSpsNode_p);

	/* Cleans DL Activation queue */
	do
	{
		dlSpsNode_p = PNULL;
		DEQUEUE_MAC_DL_SPS_Q ( dlSpsActQ_gp[internalCellIndex], dlSpsNode_p );
		if ( PNULL != dlSpsNode_p )
		{
			freeMemPool( dlSpsNode_p);
		}
	}
	while ( PNULL != dlSpsNode_p);

	/* Cleans DL Reactivation queue */
	do
	{
		dlSpsNode_p = PNULL;
		DEQUEUE_MAC_DL_SPS_Q ( dlSpsReactQ_gp[internalCellIndex], dlSpsNode_p );
		if ( PNULL != dlSpsNode_p )
		{
			freeMemPool( dlSpsNode_p);
		}
	}
	while ( PNULL != dlSpsNode_p);

	/* Cleans DL UE exclude list */
	do
	{
		dlSpsNode_p = PNULL;
		DEQUEUE_MAC_DL_SPS_Q ( dlSpsUeExclude_gp[internalCellIndex], dlSpsNode_p );
		if ( PNULL != dlSpsNode_p )
		{
			freeMemPool( dlSpsNode_p);
		}
	}
	while ( PNULL != dlSpsNode_p);

	/* Cleans DL Resource reserve-release request queue */
	do
	{
		dlSpsNode_p = PNULL;
		DEQUEUE_MAC_DL_SPS_Q ( dlSpsResRsvQ_gp[internalCellIndex], dlSpsNode_p );
		if ( PNULL != dlSpsNode_p )
		{
			freeMemPool( dlSpsNode_p);
		}
	}
	while ( PNULL != dlSpsNode_p);

	do
	{
		dlSpsNode_p = PNULL;
		DEQUEUE_MAC_DL_SPS_Q ( dlSpsResRelQ_gp[internalCellIndex], dlSpsNode_p );
		if ( PNULL != dlSpsNode_p )
		{
			freeMemPool( dlSpsNode_p);
		}
	}
	while ( PNULL != dlSpsNode_p);

	LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
			"Exit[DL-Strategy][%s]\n", __func__);
}
/*****************************************************************************
 * Function Name  : validateUeStateForDrxQueueProcessing
 * Inputs         : ueDLContextInfo_p- Pointer to DLUEContextInfo
 *                  internalCellIndex
 *                  currentGlobalTTITickCount
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE
 * Description    : This function cleans up SPS strategy queues.
 *********************************************************************************/

UInt32 validateUeStateForDrxQueueProcessing( DLUEContextInfo * ueDLContextInfo_p,
                   /* SPR 15909 fix start */
                   tickType_t currentGlobalTTITickCount,
                   /* SPR 15909 fix end */
                   InternalCellIndex internalCellIndex)
{
	if ((!ueDLContextInfo_p->pendingDeleteFlag) &&
			( (UL_SYNC_LOSS_OFF ==
			   checkUlSyncLossForUE(ueDLContextInfo_p->dlUEContext_p,
				   currentGlobalTTITickCount, internalCellIndex))
			  && (DRA_SCHEDULING == ueDLContextInfo_p->dlUEContext_p->schType ) ) )
	{
		return MAC_SUCCESS;
	}
	else
		return MAC_FAILURE;
}

/*****************************************************************************
 * Function Name  : deleteInvalidNodeFromDLRetransmissionQueue
 * Inputs         : count - Number of nodes in queue.
 *                  internalCellIndex
 * Outputs        : None.
 * Returns        : None
 * Description    : This function deletes the invalid node from DL Retransmission
 *                  queue.
 *********************************************************************************/
void deleteInvalidNodeFromDLRetransmissionQueue(UInt32 count,
        InternalCellIndex internalCellIndex)
{
    DLRetxQueueNode *dlReTxNode_p       = PNULL;

    while(count--)
    {
        dlReTxNode_p = (DLRetxQueueNode *) popNode(dlRetransmissionQueue_gp[internalCellIndex]);
        if (PNULL != dlReTxNode_p)
        {
            if (dlReTxNode_p->isNodeInvalid)
            {
                freeMemPool (dlReTxNode_p);
                dlReTxNode_p = PNULL;
            }
            else
            {
                /*cov_warning 56015 fix start*/
                if(pushNode (dlRetransmissionQueue_gp[internalCellIndex], &(dlReTxNode_p->anchorNode)))
                {
                   freeMemPool(dlReTxNode_p);
                   dlReTxNode_p = PNULL;
                }
                /*cov_warning 50615 fix end */
            } 
        }
    }

    return;
}

/*****************************************************************************
 * Function Name  : cleanupDlCAEventQueue
 * Inputs         : internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up dlCAEventQueue.
 **********************************************************************************/
void cleanupDlCAEventQueue(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;

    DLCAEventQueueNode *eventQNode_p = PNULL;
    count = COUNT_DL_CA_EVENT_Q(dlCAEventQueue_gp[internalCellIndex]);
    for(ind = 0; ind < count; ind++)
    {
        DEQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[internalCellIndex],(void **)&eventQNode_p);
        if(eventQNode_p)
        {
            FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
            eventQNode_p = PNULL;
        }
    }
    return;
}

/*****************************************************************************
 * Function Name  : cleanupDlCAMacCEQueue
 * Inputs         : internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up dlCAMacCEQueue.
 ***********************************************************************************/
void cleanupDlCAMacCEQueue(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;
     
    DLCAMacCEQueueNode *dlCAMacCEQueueNode_p = PNULL;
    count = COUNT_DL_CA_MAC_CE_Q(dlCAMacCEQueue_gp[internalCellIndex]);
    for(ind = 0; ind < count; ind++)
    {
        DEQUEUE_DL_CA_MAC_CE_Q(dlCAMacCEQueue_gp[internalCellIndex],(void **)&dlCAMacCEQueueNode_p);
        if(dlCAMacCEQueueNode_p)
        {
            FREE_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueueNode_p);
            dlCAMacCEQueueNode_p = PNULL;
        }
    }
    return;
}

/*****************************************************************************
 * Function Name  : cleanupPendingDlCAMacCEQueue
 * Inputs         : internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up pendingdlCAMacCEQueue.
 ***********************************************************************************/
void cleanupPendingDlCAMacCEQueue(InternalCellIndex internalCellIndex)
{
    UInt32 index = 0;
    UInt32 count = 0;
    DLCAMacCEQueueNode *dlCAMacCEQueuePendingNode_p         = PNULL;

    /* SPR 12916 Changes Start */
    count = COUNT_DL_CA_MAC_CE_Q(pendingDlCAMacCEQueue_gp[internalCellIndex]);
    for(index = 0; index < count; index++)
    {
        DEQUEUE_DL_CA_MAC_CE_Q(pendingDlCAMacCEQueue_gp[internalCellIndex],(void **)&dlCAMacCEQueuePendingNode_p);
        /* SPR 12916 Changes End */
        if(dlCAMacCEQueuePendingNode_p)
        {
            FREE_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueuePendingNode_p);
            dlCAMacCEQueuePendingNode_p = PNULL;
        }
    }

    return;
}

/*****************************************************************************
 * Function Name  : cleanupPendingFailureDlCAMacCEQueue
 * Inputs         : internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up pendingdlCAMacCEQueue.
 ***********************************************************************************/
void cleanupPendingFailureDlCAMacCEQueue(InternalCellIndex internalCellIndex)
{
    UInt32 index = 0;
    UInt32 count = 0;
    DLCAMacCEQueueNode *dlCAMacCEQueuePendingFailureNode_p  = PNULL;

    /* SPR 12916 Changes Start */
    count = COUNT_DL_CA_MAC_CE_Q(pendingFailureDlCAMacCEQueue_gp[internalCellIndex]);
    for(index = 0; index < count; index++)
    {
        DEQUEUE_DL_CA_MAC_CE_Q(pendingFailureDlCAMacCEQueue_gp[internalCellIndex],(void **)&dlCAMacCEQueuePendingFailureNode_p);
        /* SPR 12916 Changes End */
        if(dlCAMacCEQueuePendingFailureNode_p)
        {
            FREE_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueuePendingFailureNode_p);
            dlCAMacCEQueuePendingFailureNode_p = PNULL;
        }
    }

    return;
}
/*****************************************************************************
 * Function Name  : cleanupDlCAMacFreeScellHarqBufferQueue
 * Inputs         : internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up cleanupDlCAMacFreeScellHarqBufferQueue.
 ************************************************************************************/

void cleanupDlCAMacFreeScellHarqBufferQueue(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;

    DLCAMacFreeScellHarqBufferQueueNode *scellFreeHarqQNode_p = PNULL;

    count = COUNT_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp[internalCellIndex]);
    for(ind = 0; ind < count; ind++)
    {
        DEQUEUE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp[internalCellIndex],(void **)&scellFreeHarqQNode_p);
        if(scellFreeHarqQNode_p)
        {
            FREE_MEM_NODE_FREE_SCELL_HARQ_Q(scellFreeHarqQNode_p);
            scellFreeHarqQNode_p = PNULL;
        }
    }
    return;
}
/*****************************************************************************
 * Function Name  : cleanupMeasGapOnOffTriggerQueue
 * Inputs         : internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up cleanupMeasGapOnOffTriggerQueue.
 *************************************************************************************/

void cleanupMeasGapOnOffTriggerQueue(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;

    MeasGapOnOffTriggerNode *measGapOnOffTriggerNode_p = PNULL;

    count = COUNT_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(internalCellIndex);
    for (ind = 0; ind < count; ind++)
    {
        DEQUEUE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,\
                    (void *)&measGapOnOffTriggerNode_p, internalCellIndex);
        if (measGapOnOffTriggerNode_p)
        {
            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapOnOffTriggerNode_p);
            measGapOnOffTriggerNode_p = PNULL;
        }
    }
    return;
}
/*****************************************************************************
 * Function Name  : cleanupPendingDRXQueue
 * Inputs         : internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up cleanupPendingDRXQueue.
 **************************************************************************************/

void cleanupPendingDRXQueue(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;

    DRXQueueNode * drxNode_p = PNULL;

    /* Clean up for Pending DRX Queue */
    count = COUNT_DRX_Q(pendingDRXQueue_gp[internalCellIndex]);
    for (ind = 0; ind < count; ind++)
    {
        DEQUEUE_DRX_Q((pendingDRXQueue_gp[internalCellIndex]),DRXQueueNode ,(void **)&drxNode_p);

        if (drxNode_p)
        {
            FREE_MEM_NODE_DRX_Q(drxNode_p);
            drxNode_p = PNULL;
        }
    }
    return;
}
/*****************************************************************************
 * Function Name  : cleanupPendingTAQueue
 * Inputs         : internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up cleanupPendingDRXQueue.
 ***************************************************************************************/
void cleanupPendingTAQueue(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;

    TimeAdvanceQueueNode *taNode_p = PNULL;

    /* Clean up for Pending TA Queue */
    count = COUNT_TA_Q(pendingTAQueue_gp[internalCellIndex]);
    for (ind = 0; ind < count; ind++)
    {
        DEQUEUE_TA_Q(pendingTAQueue_gp[internalCellIndex],TimeAdvanceQueueNode ,(void **) &taNode_p);

        if (taNode_p)
        {
            FREE_MEM_NODE_TA_Q(taNode_p);
            taNode_p = PNULL;
        }
    }
    return;
}
/*****************************************************************************
 * Function Name  : cleanupDlUeZeroNonZeroQueueLoadTriggrQueue
 * Inputs         : internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up cleanupDlUeZeroNonZeroQueueLoadTriggrQueue.
 ****************************************************************************************/

void cleanupDlUeZeroNonZeroQueueLoadTriggrQueue(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;

    ZeroNonZeroTriggerNode * zeroNonZeroNode_p = PNULL;

    /* Clean up for  UE Zero <-> NonZero Trigger Queue */
    /* CirQ changes start */
    count = COUNT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(internalCellIndex);
    /* CirQ changes end */
    for (ind = 0; ind < count; ind++)
    {
    /* CirQ changes start */
        DEQUEUE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(ZeroNonZeroTriggerNode,
                (void **)&zeroNonZeroNode_p, internalCellIndex);
    /* CirQ changes end */

        if (zeroNonZeroNode_p)
        {
            FREE_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(
                                                zeroNonZeroNode_p);
        }
    }
    return;
}
/*****************************************************************************
 * Function Name  : cleanupDlUENonZeroQueueLoadAndHarqList
 * Inputs         : internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function cleans up cleanupDlUENonZeroQueueLoadAndHarqList.
 *****************************************************************************************/

void cleanupDlUENonZeroQueueLoadAndHarqList(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;

    NonZeroQueueLoadAndHarqNode * node_p = PNULL;

    /* Clean up for  Non Zero Queue Load and Harq List */
    count = listCount(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]);
    for (ind = 0; ind < count; ind++)
    {
        node_p = 
            (NonZeroQueueLoadAndHarqNode *)getFirstListNode
            (dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]);

        if (node_p)
        {
            /* Rel 5.3.1: Memory related changes start*/
            listDeleteNode( dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex],(LTE_LIST_NODE*)node_p);
            /* Rel 5.3.1: Memory related changes End*/
            freeMemPool(node_p);
            node_p = PNULL;
        }
    }
    return;
}
/*****************************************************************************
 *Function Name  : cleanupDlCellEdgeUserPriorityQueue
 *Inputs         : internalCellIndex
 *Outputs        : None.
 *Returns        : None.
 *Description    : This function cleans up cleanupDlCellEdgeUserPriorityQueue.
 ******************************************************************************************/

void cleanupDlCellEdgeUserPriorityQueue(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;

    CEUserPriorityQueueNode *ceNode_p = PNULL;

    count = sQueueCount(dlCellEdgeUserPriorityQueue_gp[internalCellIndex]);
    for (ind = 0; ind < count; ind++)
    {
        ceNode_p = 
            (CEUserPriorityQueueNode *)popNode(dlCellEdgeUserPriorityQueue_gp[internalCellIndex]);

        if (ceNode_p)
        {
            freeMemPool(ceNode_p);
            ceNode_p = PNULL;
        }
    }
    return;
}
/*****************************************************************************
 *Function Name  : cleanupDRXOnOffTriggerQueue
 *Inputs         : internalCellIndex
 *Outputs        : None.
 *Returns        : None.
 *Description    : This function cleans up cleanupDRXOnOffTriggerQueue.
 *******************************************************************************************/

void cleanupDRXOnOffTriggerQueue(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;

    DRXOnOffTriggerNode *drxOnOffTriggerNode_p = PNULL;
    count = COUNT_DL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex);
     for (ind = 0; ind < count; ind++)
     {
         DEQUEUE_DL_DRX_ON_OFF_TRIGGER_Q(DRXOnOffTriggerNode, (void *)&drxOnOffTriggerNode_p,internalCellIndex);
         if (drxOnOffTriggerNode_p)
         {
             FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxOnOffTriggerNode_p);
             drxOnOffTriggerNode_p = PNULL;
         }
     }
     return;
}
/*****************************************************************************
 *Function Name  : cleanupAdvanceAllocInfoQueue
 *Inputs         : internalCellIndex
 *Outputs        : None.
 *Returns        : None.
 *Description    : This function cleans up cleanupAdvanceAllocInfoQueue.
 ********************************************************************************************/

void cleanupAdvanceAllocInfoQueue(InternalCellIndex internalCellIndex)
{
    UInt32 ind = 0;
    UInt32 count = 0;
    /* SPR 4834 Fix Start */
    UInt16 ueIndex = 0;
    /* SPR 4834 Fix End */    

    /* SPR 4834 Fix Start */
    DLUEContextInfo *dlUEContextInfo_p   = PNULL;
    DLUEContext *ueDLContext_p           = PNULL;
    DLAdvanceQueueNode *dlAdvNode_p = PNULL;
    /* SPR 4834 Fix End */    

    count = sQueueCount(advanceAllocInfoQueue_gp[internalCellIndex]);
    for (ind = 0; ind < count; ind++)
    {
        dlAdvNode_p = 
            (DLAdvanceQueueNode *)popNode(advanceAllocInfoQueue_gp[internalCellIndex]);

        if (dlAdvNode_p)
        {
            /* Find the dlContext of the UE */
            ueIndex = dlAdvNode_p->dlStrategyTxNode->ueIndex;
            dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            ueDLContext_p = dlUEContextInfo_p->dlUEContext_p;
            /* FDD_Warning_Fix */
            freeDLHarqProcess(ueDLContext_p, (dlAdvNode_p->dlStrategyTxNode->harqProcess_p), internalCellIndex);
            /* FDD_Warning_Fix */
            freeMemPool(dlAdvNode_p->dlStrategyTxNode);
            freeMemPool(dlAdvNode_p);
            dlAdvNode_p = PNULL;
        }
    }
    return;
}

/*****************************************************************************
 *Function Name  : updateNumAvailableRBsAndMaxDlUeSchedule
 *Inputs         : numberOfAvailableRBs
 *                 totalNumberOfAvailableRBs_p
 *                 maxUEsToBeScheduledDL
 *                 totalMaxUEsToBeScheduledDL_p
 *                 tempNumOfAvailableRBs_p
 *                 tempMaxUEsToBeScheduledDL_p
 *                 internalCellIndex
 *Outputs        : None.
 *Returns        : None.
 *Description    : This function updates num of available rb and max dl ue schedule.
 ********************************************************************************************/
void updateNumAvailableRBsAndMaxDlUeSchedule(UInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
        UInt32 *totalNumberOfAvailableRBs_p,
        UInt8 maxUEsToBeScheduledDL[],
        UInt16 *totalMaxUEsToBeScheduledDL_p,
        UInt32 *tempNumOfAvailableRBs_p,
        UInt8 *tempMaxUEsToBeScheduledDL_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 rowIndex = 0;
    UInt8 colIndex = 0;

    if ((DL_QOS_BASED_SCHEDULING != cellSpecificParams_g.\
                cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                dlSchdConfig.dlSchedulerStrategy))
    {
        numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] =
            tempNumOfAvailableRBs_p[CC_CE_REGION];
        numberOfAvailableRBs[internalCellIndex][CE_REGION] =
            tempNumOfAvailableRBs_p[CE_REGION];
        *totalNumberOfAvailableRBs_p =
            numberOfAvailableRBs[internalCellIndex][CC_CE_REGION];
        maxUEsToBeScheduledDL[internalCellIndex] = *tempMaxUEsToBeScheduledDL_p;
        *totalMaxUEsToBeScheduledDL_p = maxUEsToBeScheduledDL[internalCellIndex];

    }
    else if(!IS_CA_ENABLED())        
        /*SPR 12015 Fix End */
    {
        /* SPR 11410 FIX START */
        numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] = 
            tempNumOfAvailableRBs_p[CC_CE_REGION + 2*internalCellIndex];
        numberOfAvailableRBs[internalCellIndex][CE_REGION] =
            tempNumOfAvailableRBs_p[internalCellIndex*2 + CE_REGION];
        *totalNumberOfAvailableRBs_p = 
            numberOfAvailableRBs[internalCellIndex][CC_CE_REGION]; 

        maxUEsToBeScheduledDL[internalCellIndex] = tempMaxUEsToBeScheduledDL_p[internalCellIndex];
        *totalMaxUEsToBeScheduledDL_p = maxUEsToBeScheduledDL[internalCellIndex];
        /* SPR 11410 FIX End */
    }
    else
    {
        /* UT FIX */
        for(rowIndex = 0; rowIndex < MAX_NUM_CELL; rowIndex++)
        {
            for(colIndex= 0; colIndex < MAX_USER_LOCATION; colIndex++)
            {
                numberOfAvailableRBs[rowIndex][colIndex] = tempNumOfAvailableRBs_p[(2*rowIndex)+ colIndex]; 
            } 
        }
        *totalNumberOfAvailableRBs_p = 
            numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] + 
            numberOfAvailableRBs[!(internalCellIndex)][CC_CE_REGION];

        maxUEsToBeScheduledDL[internalCellIndex] = tempMaxUEsToBeScheduledDL_p[internalCellIndex];
        maxUEsToBeScheduledDL[!(internalCellIndex)] = tempMaxUEsToBeScheduledDL_p[!(internalCellIndex)];

        *totalMaxUEsToBeScheduledDL_p = maxUEsToBeScheduledDL[internalCellIndex] +
            maxUEsToBeScheduledDL[!(internalCellIndex)];
    }

    return;
}
/* Cyclomatic_complexity_changes_end */
/* SPR 11403 fix start */
/*****************************************************************************
 * Function Name  : validateTxModeAndDciFormat
 * Inputs         : transMode- current transmission mode of UE
 * 		  : dciFormat - dciFormat present in HarqProcess
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks if the dciFormat in harqProcess is compatible 
 * 		    with current transmissionMode, if Not, it returns failure.
 *****************************************************************************/
MacRetType validateTxModeAndDciFormat(TransmissonMode transMode,DCIFormat dciFormat)
{
	if(DCI_FORMAT_1A == dciFormat)
	{
		return MAC_SUCCESS;
	}
	else if((DCI_FORMAT_1 == dciFormat)&& ((TX_MODE_1 == transMode)||
		(TX_MODE_2 == transMode) || (TX_MODE_7 == transMode)))
	{
		return MAC_SUCCESS;
	}
	else if((DCI_FORMAT_1B == dciFormat)&& (TX_MODE_6 == transMode))
	{
		return MAC_SUCCESS;
	}
	else if((DCI_FORMAT_2A == dciFormat)&& (TX_MODE_3 == transMode))
	{
		return MAC_SUCCESS;
	}
	else if((DCI_FORMAT_2 == dciFormat)&& (TX_MODE_4 == transMode))
	{
		return MAC_SUCCESS;
	}
	else if((DCI_FORMAT_2B == dciFormat)&& (TX_MODE_8 == transMode))
	{
		return MAC_SUCCESS;
	}
	else
	{
		return  MAC_FAILURE;
	}
}
/* SPR 11403 fix end */
/* SPR 15446 Fix Start */
/*****************************************************************************
 * Function Name  :allocateRbBasedOnQload 
 * Inputs         : DLUEContext
 *                  availableRB
 * Outputs        : None
 * Returns        : allocatedRB
 * Description    : THis function adjust RB based on Queload. 
 * 		    
 *****************************************************************************/
UInt32 allocateRbBasedOnQload(
        DLUEContext* ueDLContext_p,
        UInt32 availableRB,
        UInt8 numTB)
{
    UInt32 allocatedRB      = availableRB;
    UInt32 tempTBOneSizeRet = 0;
    UInt32 tempTBTwoSizeRet = 0;
    SInt32 netUeQl          = 0;
    netUeQl = NET_UE_QL(ueDLContext_p);

    /* Decrement the RB till Queload is satisfiled */
    do
    {
        tempTBOneSizeRet  = determineTBSize(ueDLContext_p->dlCQIInfo.
                mcsIndexCodeWordOne, allocatedRB, TB_MAPPED_TO_ONE_LAYER);
        if (numTB == 2)
        {
        tempTBTwoSizeRet = determineTBSize(ueDLContext_p->dlCQIInfo.
                mcsIndexCodeWordTwo, allocatedRB ,TB_MAPPED_TO_ONE_LAYER );
        }
        /* + SPR 17439 */
        if (((SInt32)(tempTBOneSizeRet + tempTBTwoSizeRet)) > (netUeQl))
        /* - SPR 17439 */     
        {
            allocatedRB--;
            continue;
        }
        break;
    }while(allocatedRB > 1);

    return allocatedRB;
}
/* SPR 15446 Fix End */

/*CYCLOMATIC_CHANGES_6.1_START*/
/*****************************************************************************
 * Function Name  : determineDCIFormatForTxOtherThanThreeFour
 * Inputs         : dlStrategyTxNode_p
 *                  tempTransmissionMode
 * Outputs        : None
 * Returns        : This will update DCI formats for transmission modes other
 *                  than 3 & 4 in dlStrategyNode
 * Description    : This function will determine the Primary and Secondary DCI 
                    formats for Transmission modes other than 3 and 4
 *****************************************************************************/
STATIC  void determineDCIFormatForTxOtherThanThreeFour(DLStrategyTxNode *dlStrategyTxNode_p,
                                                             UInt32 tempTransmissionMode)
{
    switch (tempTransmissionMode){
        case TX_MODE_5:
        dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_1D;
        dlStrategyTxNode_p->secondaryDCIFormat = DCI_FORMAT_1A;
        break;
        case TX_MODE_6:
        dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_1B;
        dlStrategyTxNode_p->secondaryDCIFormat = DCI_FORMAT_1A;
        break;
        case TX_MODE_7:
        dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_1A;
        dlStrategyTxNode_p->secondaryDCIFormat = INVALID_DCI_FORMAT;
        break;
        case TX_MODE_8:
        dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_1A;
        dlStrategyTxNode_p->secondaryDCIFormat = INVALID_DCI_FORMAT;
        break;
        /*SPR 545 Fix start*/
        case TX_MODE_ATTACH_NOT_COMPLETE:
        dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_1A;
        dlStrategyTxNode_p->secondaryDCIFormat = DCI_FORMAT_1A;
        break;
        /*SPR 545 Fix starts*/
        default:
        dlStrategyTxNode_p->primaryDCIFormat =
              macParams_g.preferredPrimaryDCIFormatForSingleTB[tempTransmissionMode];
        dlStrategyTxNode_p->secondaryDCIFormat =
               macParams_g.preferredSecondaryDCIFormatForSingleTB[tempTransmissionMode];
        }
}

/*****************************************************************************
 * Function Name  : determinePrimaryDCIFormatForTxThreeEight
 * Inputs         : dlStrategyTxNode_p
 *                  tempTransmissionMode
 * Outputs        : None
 * Returns        : This will update DCI formats for transmission modes 
 *                  3 & 8 in dlStrategyNode
 * Description    : This function will determine the Primary and Secondary DCI 
                    formats for Transmission modes 3 and 8
 *****************************************************************************/
STATIC  void determinePrimaryDCIFormatForTxThreeEight(DLStrategyTxNode *dlStrategyTxNode_p,
                                                            UInt32 tempTransmissionMode)
{
    if (tempTransmissionMode == TX_MODE_3)
    {
        dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_2A ;
    }
    else if(tempTransmissionMode == TX_MODE_8)
    {
        dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_2B ;
    }
    else
    {
        dlStrategyTxNode_p->primaryDCIFormat= DCI_FORMAT_2 ;
    }

    dlStrategyTxNode_p->secondaryDCIFormat = INVALID_DCI_FORMAT ;
}
/*CYCLOMATIC_CHANGES_6.1_END*/
/* SPR 16916 FIX START */
#ifdef KPI_STATS
/****************************************************************************
 * Function Name  : macUpdateActiveUePerQciInfo 
 * Inputs         : lcTrigIndexForKpi : Number of LC for KPI to be considered
 *                  DLUEContext : DL UE Context.
 *                  addTrigLCNodeForKpi: LC's for consideration
 * Outputs        : None
 * Returns        : void
 * Description    : This function updates the the KPI for Active UE's
 *                  based on queload in each qci.
 ****************************************************************************/
/* SPR 16916 FIX START */
void macUpdateActiveUePerQciInfo(
        UInt8 lcTrigIndexForKpi,
        DLUEContext* ueContext_p,
        UInt8 *addTrigLCNodeForKpi,
        InternalCellIndex internalCellIndex)
/* SPR 16916 FIX END */
{
    UInt8 qci          = 0;
    UInt8 lcId         = 0;
    UInt8 lcIndex      = 0;
    for ( lcIndex = 0; lcIndex < lcTrigIndexForKpi ; lcIndex++)
    {
        lcId = addTrigLCNodeForKpi[lcIndex];
        if(lcId != INVALID_LCID) 
        {
            qci = ueContext_p->logicalChannel[lcId].dlLcQosInfo.QCI;
            if((qci) && (qci <= MAX_QCI))
            {
                /* Reset the Harq zeroQloadFlag of the QCI */
                resetZeroQloadFlag(ueContext_p->ueIndex,qci);

                /* If qciInfoFlag is not set update the KPI */
                if (!ueContext_p->qciInfoFlag[--qci])
                {
                    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci] += 1; 
                    ueContext_p->qciInfoFlag[qci] = 1;
                    /* SPR 23402 Changes Start */
                    /* MOCN Changes Start */
                    /*MOCN SPR21942 Fix Start*/
                    if(mocnOperatorParams_g.isMocnEnabled && ueContext_p->operatorId < mocnOperatorParams_g.numOfOperator ) 
                        /*MOCN SPR21942 Fix End*/
                    {
                        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciDL[ueContext_p->operatorId][qci] += 1; 
                    }
                    /* MOCN Changes End */
                    /* SPR 23402 Changes End */
                    /*Coverity 94821 Fix Start*/
                    LOG_MAC_MSG (MAC_DL_ACTIVE_UE_QCI_LOG_ID, LOGDEBUG,
                            MAC_DL_Strategy, getCurrentTick (), 
                            ueContext_p->ueIndex,lcId,(qci + 1 ),
                            DEFAULT_INT_VALUE,
                            gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci],
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "SET_DL_UE");
                    /*Coverity 94821 Fix End*/
                }
            }
        }
    }
}
/****************************************************************************
 * Function Name  : macUpdateActiveUlUePerQciInfo 
 * Inputs         : lcTrigIndexForKpi : Number of LCG for KPI to be considered
 *                  ULUEContext : UL UE Context.
 *                  addTrigLCGNodeForKpi: LCG's for consideration
 * Outputs        : None
 * Returns        : void
 * Description    : This function updates the the KPI for Active UE's in Uplink
 *                  based on queload in each qci.
 ****************************************************************************/
void macUpdateActiveUlUePerQciInfo(
        UInt8 lcgTrigIndexForKpi,
        ULUEContext* ueULContext_p,
        UInt8 *addTrigLCGNodeForKpi,
        InternalCellIndex internalCellIndex)
{
    UInt8 lcgId     = 0;
    UInt8 lcgIndex  = 0;
    UInt8 numLC     = 0;
    UInt8 qci       = 0;
    UInt8 lcId      = 0;
    UInt8 lcIndex   = 0;

    for(lcgIndex = 0; lcgIndex < lcgTrigIndexForKpi ; lcgIndex++)
    {
        lcgId = addTrigLCGNodeForKpi[lcgIndex];
        numLC = ueULContext_p->ulLCGInfo[lcgId].numLC;
        if(numLC && ueULContext_p->ulLCGInfo[lcgId].isDrbPresent)
        {
            for ( lcIndex = MAC_LCID_3; lcIndex < MAX_NUMBER_OF_LOGICAL_CHANNEL ; lcIndex++)
            {
                lcId = ueULContext_p->ulLCGInfo[lcgId].lcId[lcIndex];
                if(lcId != INVALID_LCID )
                {
                    qci = ueULContext_p->lcInfo[lcId].ulQosInfo.QCI;
                    if((qci) && (qci <= MAX_QCI))
                    {
                        if (!ueULContext_p->qciInfoFlag[--qci])
                        {
                            gMacStats.pStats-> lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci]++;
                            ueULContext_p->qciInfoFlag[qci] = 1;
                            /* SPR 23402 Changes Start */
                            /* MOCN SPR21942 changes Start */
                            if(mocnOperatorParams_g.isMocnEnabled && (ueULContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                            {
                                gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciUL[ueULContext_p->operatorId][qci]++; 
                            }
                            /* MOCN SPR21942 changes End */
                            /* SPR 23402 Changes End */
                            LOG_MAC_MSG (MAC_UL_ACTIVE_UE_QCI_LOG_ID, LOGERROR,
                                    MAC_UL_Strategy, getCurrentTick (), 
                                    ueULContext_p->ueIndex,lcId,qci,
                                    lcgId,
                                    gMacStats.pStats-> lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci],
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "SET_UL_UE");
                        }                                                
                    }
                }
            }
        }
    } 
}

/*****************************************************************************
 * Function Name  : resetZeroQloadFlag
 * Inputs         : ueIndex,
 *                  qci
 * Outputs        : None
 * Returns        : None
 * Description    : This function will reset the flag resetZeroQloadFlag in 
 *                  UEContext for all harq processes for the corresponding qci.
 *****************************************************************************/

void resetZeroQloadFlag(UInt16 ueIndex, UInt8 qci)
{
    UInt8 count = 0;
    DLUEContext *ueDLContext_p          = PNULL;
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
    for(count = 0; count < MAX_HARQ_PROCESS_NB ; count++)
    {
        ueDLContext_p->zeroQloadFlag[qci-1][count] = 0;
    }
}
/*****************************************************************************
 * Function Name  : updateNumOfActiveUePerQciUl 
 * Inputs         : ueIndex - ueIndex 
 * Outputs        : None
 * Returns        : None 
 * Description    : Updates the KPI stat for numberOfActiveUE in UL.
 *****************************************************************************/
void  updateNumOfActiveUePerQciUl(UInt16 ueIndex,
        InternalCellIndex internalCellIndex)
{
    UInt8 qci      = 0;
    UInt8 lcId     = 0;
    UInt8 numLC    = 0;
    UInt8 lcIndex  = 0;
    UInt8 lcCount  = 0;
    UInt8 lcgIdVal = 0;
    ULUEContext*  ueULContext_p = PNULL;      

    if(MAX_UE_SUPPORTED <= ueIndex)
    {
        /* No need to process as UE is not valid */
        /* UE does not exist */
        LOG_MAC_MSG( MAC_ULUECONTEXT_NOT_FOUND_ID, LOGERROR, MAC_RRC_INF,
                getCurrentTick(), ueIndex, __LINE__, 0,0,0, 0,0,
                __func__, "KPI" );
        return;
    }
    ueULContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    if( PNULL == ueULContext_p )
    {
        /* UE does not exist */
        LOG_MAC_MSG( MAC_ULUECONTEXT_NOT_FOUND_ID, LOGERROR, MAC_RRC_INF,
                getCurrentTick(), ueIndex, __LINE__, 0,0,0, 0,0,
                __func__, "KPI" );
        return;
    }

    for(lcgIdVal = 0; lcgIdVal < NUM_OF_LCG; lcgIdVal++)
    {
        lcCount = 0;
        numLC = ueULContext_p->ulLCGInfo[lcgIdVal].numLC;
        if(numLC)
        {
            for ( lcIndex = MAC_LCID_3; lcIndex < MAX_NUMBER_OF_LOGICAL_CHANNEL; 
                    lcIndex++ )
            {
                lcId = ueULContext_p->ulLCGInfo[lcgIdVal].lcId[lcIndex];
                if(lcId != INVALID_LCID )
                {
                    /* In case the last packet was Acked and there
                     * is no buffer available for UL grant */
                    if(0 == ueULContext_p->bsrNet[lcgIdVal])
                    {
                        qci = ueULContext_p->lcInfo[lcId].ulQosInfo.QCI;
                        if ((qci) && (qci <= MAX_QCI))
                        {
                            if ((ueULContext_p->qciInfoFlag[--qci] ) &&
                                (gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci]))
                            {
                                gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci]--;
                                /* SPR 23402 Changes Start */
                                /* MOCN Changes Start */
                                /*MOCN SPR21942 Fix Start*/
                                if(mocnOperatorParams_g.isMocnEnabled && ueULContext_p->operatorId < mocnOperatorParams_g.numOfOperator)
                                {
                                    if(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciUL[ueULContext_p->operatorId][qci])
                                    {
                                        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciUL[ueULContext_p->operatorId][qci]--; 
                                    }
                                }
                                /*MOCN SPR21942 Fix End*/
                                /* MOCN Changes End */
                                /* SPR 23402 Changes End */
                                ueULContext_p->qciInfoFlag[qci] = 0;
                                LOG_MAC_MSG (MAC_UL_ACTIVE_UE_QCI_LOG_ID, LOGERROR,
                                        MAC_UL_Strategy, getCurrentTick (), 
                                        ueULContext_p->ueIndex,lcId,qci,
                                        lcgIdVal,
                                        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci],
                                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "RESET_UL_UE");
                            }
                            lcCount++;
                        }
                    }
                    if(lcCount == numLC)
                    {
                        break;
                    }
                }
            }
        }
    }
}

/*****************************************************************************
 * Function Name  :  updateKpiStatsForNumOfActiveUeDlPerQci
 * Inputs         :  ueindex,
 *                   TBInfo,
 *                   TBFlag,
 *                   HarqProcessId
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function decrements KPI counters for the active 
 *                   number of UEs in DL, if Harq Ack has been received 
 *                   for the entire buffered data or max retransmission
 *                   count is reached.
 *****************************************************************************/

 void updateKpiStatsForNumOfActiveUeDlPerQci(UInt16 ueIndex,
        DLHARQTBInfo *TBInfo,TbValue TBFlag,UInt8 harqProcessId,
        InternalCellIndex internalCellIndex)
{
    UInt8 lc      = 0;
    UInt8 qci     = 0; 
    UInt8 lcCount = 0;
    RLCDataBuffer* rlcDataBufferTB = PNULL;
    rlcDataBufferTB = &(TBInfo->transmissionAndRLCData_p->rlcDataBufferArr[0]);

    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p = PNULL;
    dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;
    if((rlcDataBufferTB != NULL) && (dlUeContext_p != PNULL))
    {
        if(TBFlag == TB_ONE)
            lcCount = TBInfo->transmissionAndRLCData_p->lcCountTBOne; 
        else
            lcCount = TBInfo->transmissionAndRLCData_p->lcCountTBTwo; 

        for(lc = 0; lc < lcCount; lc++)
        {
            qci = rlcDataBufferTB[lc].qci;
            if ((qci) && (qci <= MAX_QCI))
            {
                /*Coverity 94821 Fix Start*/
                /* + Coverity 94739 */
                qci--;
                LOG_MAC_MSG (MAC_DL_ACTIVE_UE_QCI_LOG_ID, LOGDEBUG,
                        MAC_DL_Strategy, getCurrentTick (), 
                        ueIndex,rlcDataBufferTB[lc].lcID,(qci + 1),
                        DEFAULT_INT_VALUE,
                        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci],/* coverity out of bound fix */
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "B4_RESET_DL_UE");
                /*Coverity 94821 Fix End*/
                if ((rlcDataBufferTB[lc].lcID >= MAC_LCID_3) && 
                        (rlcDataBufferTB[lc].lcID < MAX_NUMBER_OF_LOGICAL_CHANNEL))
                {
                    if((TRUE == dlUeContext_p->zeroQloadFlag[qci][harqProcessId]) &&
                            (dlUeContext_p->qciInfoFlag[qci]))
                    {
                        if ( gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci])
                        {
                            gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci]--;
                            /* SPR 23402 Changes Start */
                            /* MOCN Changes Start */
                            /*MOCN SPR21942 Fix Start*/
                            if(mocnOperatorParams_g.isMocnEnabled && dlUeContext_p->operatorId < mocnOperatorParams_g.numOfOperator)
                            {
                                if(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciDL[dlUeContext_p->operatorId][qci]) 
                                {
                                    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciDL[dlUeContext_p->operatorId][qci]--; 
                                }
                            }
                            /*MOCN SPR21942 Fix End*/
                            /* MOCN Changes End */
                            /* SPR 23402 Changes End */
                            LOG_MAC_MSG (MAC_DL_ACTIVE_UE_QCI_LOG_ID, LOGDEBUG,
                                    MAC_DL_Strategy, getCurrentTick (), 
                                    dlUeContext_p->ueIndex,rlcDataBufferTB[lc].lcID,
                                    (qci + 1),
                                    DEFAULT_INT_VALUE,
                                    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci],
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "RESET_DL_UE");
                            /* - Coverity 94739 */
                        }
                        dlUeContext_p->zeroQloadFlag[qci][harqProcessId] = FALSE;
                        dlUeContext_p->qciInfoFlag[qci] = 0;
                    }
                }
            }
        }
    }
}
/*****************************************************************************
 * Function Name  : updateNumOfActiveUeDlOnUeDelete
 * Inputs         : ueIndex - ueIndex of dlUEContext to be deleted
 * Outputs        : None
 * Returns        : None 
 * Description    : Updates the KPI stat for numberOfActiveUE in DL when UE
 *                : is being deleted.
 *****************************************************************************/
void updateNumOfActiveUeDlOnUeDelete(UInt16 ueIndex,
        InternalCellIndex internalCellIndex)
{
    UInt8 qci                           = 0;
    UInt8 lcId                          = INVALID_LCID;
    UInt8 lcIndex                       = 0;
    DLUEContext *dlUEContext_p          = PNULL;
    DLUEContextInfo *dlUEContextInfo_p  = PNULL;
    dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

    if(PNULL != dlUEContext_p)
    {
        for ( lcIndex = MAC_LCID_3; lcIndex < MAX_NUMBER_OF_LOGICAL_CHANNEL ; lcIndex++)
        {
            lcId = dlUEContext_p->logicalChannel[lcIndex].logicalChannelId;
            if(lcId != INVALID_LCID)            
            {
                qci = dlUEContext_p->logicalChannel[lcIndex].dlLcQosInfo.QCI;
                if((qci) && (qci <= MAX_QCI))
                {
                    if ((dlUEContext_p->qciInfoFlag[--qci] )&& 
                        (gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci]))
                    {
                        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci]--; 
                        resetZeroQloadFlag(ueIndex,(qci+1));
                        /* SPR 23402 Changes Start */
                        /* MOCN Changes Start */
                        /*MOCN SPR21942 Fix Start*/
                        if(mocnOperatorParams_g.isMocnEnabled && dlUEContext_p->operatorId < mocnOperatorParams_g.numOfOperator )
                        {
                            if(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciDL[dlUEContext_p->operatorId][qci]) 
                            {
                                gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciDL[dlUEContext_p->operatorId][qci]--; 
                            }
                        }
                        /*MOCN SPR21942 Fix End*/
                        /* MOCN Changes End */
                        /* SPR 23402 Changes End */
                        dlUEContext_p->qciInfoFlag[qci] = 0;
                        /*Coverity 94821 Fix Start*/
                        LOG_MAC_MSG (MAC_DL_ACTIVE_UE_QCI_LOG_ID, LOGDEBUG,
                                MAC_DL_Strategy, getCurrentTick (), 
                                dlUEContext_p->ueIndex,lcId,(qci + 1),
                                DEFAULT_INT_VALUE,
                                gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci],
                                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "RESET_DL_ON_DEL_UE");
                        /*Coverity 94821 Fix End*/
                    }
                }
            }
        }
    }
}
/*****************************************************************************
 * Function Name  : updateNumOfActiveUeUlOnUeDelete
 * Inputs         : ueIndex - ueIndex of ulUEContext to be deleted
 * Outputs        : None
 * Returns        : None 
 * Description    : Updates the KPI stat for numberOfActiveUE in UL when UE
 *                : is being deleted.
 *****************************************************************************/
void updateNumOfActiveUeUlOnUeDelete(UInt16 ueIndex,
        InternalCellIndex internalCellIndex)
{
    UInt8 qci                   = 0;
    UInt8 lcId                  = 0;
    UInt8 lcIndex               = 0;
    ULUEContext*  ueULContext_p = PNULL;      

    if(MAX_UE_SUPPORTED <= ueIndex)
    {
        /* No need to process as UE is not valid */
        return;
    }
    ueULContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    if( PNULL == ueULContext_p )
    {
        /* UE does not exist */
        LOG_MAC_MSG( MAC_ULUECONTEXT_NOT_FOUND_ID, LOGERROR, MAC_RRC_INF,
                getCurrentTick(), ueIndex, __LINE__, 0,0,0, 0,0,
                __func__, "KPI" );
        return;
    }
    for ( lcIndex = MAC_LCID_3; lcIndex < MAX_NUMBER_OF_LOGICAL_CHANNEL; 
            lcIndex++ )
    {
        lcId = ueULContext_p->lcInfo[lcIndex].lchId;
        if(lcId != INVALID_LCID )  
        {
            qci = ueULContext_p->lcInfo[lcId].ulQosInfo.QCI;
            if ((qci) && (qci <= MAX_QCI))
            {
                if ((ueULContext_p->qciInfoFlag[--qci] ) &&
                    (gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci]) )
                {
                    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci]--;
                    ueULContext_p->qciInfoFlag[qci] = 0;
                    /* SPR 23402 Changes Start */
                    /* MOCN Changes Start */
                    /*MOCN SPR21942 Fix Start*/
                    if(mocnOperatorParams_g.isMocnEnabled && ueULContext_p->operatorId < mocnOperatorParams_g.numOfOperator)
                    {
                        if(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciUL[ueULContext_p->operatorId][qci])
                        {
                            gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerOperatorPerQciUL[ueULContext_p->operatorId][qci]--; 

                        }
                    }
                    /*MOCN SPR21942 Fix End*/
                    /* MOCN Changes End */
                    /* SPR 23402 Changes End */
                    LOG_MAC_MSG (MAC_UL_ACTIVE_UE_QCI_LOG_ID, LOGERROR,
                            MAC_UL_Strategy, getCurrentTick (), 
                            ueULContext_p->ueIndex,lcId,qci,
                            DEFAULT_INT_VALUE,
                            gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci],
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "RESET_UL_ON_DEL_UE");
                }
            }

        }
    }
}
/*****************************************************************************
 * Function Name  : macIsQciUniqueForTheDlLc
 * Inputs         : lcId - lcIndex of the UE to be deleted
 *                : qci - QCI to be checked
 *                : ueIndex - ueIndex of the DL UE.
 * Outputs        : None
 * Returns        : True : if the LC has only the QCI
 *                  False : If other LC have the same QCI.
 * Description    : This function checks if the QCI is unique for the LC 
 *                  In downlink.
 *****************************************************************************/
UInt8 macIsQciUniqueForDlLc(
        UInt8 lcId,
        UInt8 qci,
        UInt16 ueIndex)
{
    UInt8 lcIndex                                   = 0;
    DLUEContext              *dlUeContext_p         = PNULL;
    DLLogicalChannelInfo     *dlLogicalChInfo_p     = PNULL;
    dlUeContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    if (dlUeContext_p)
    {   
        for (lcIndex = MAC_LCID_3; lcIndex < MAX_NUMBER_OF_LOGICAL_CHANNEL; lcIndex++)
        {
            dlLogicalChInfo_p = &dlUeContext_p->logicalChannel[lcIndex];
            if ((INVALID_LCID != dlLogicalChInfo_p->logicalChannelId)
                    && (lcIndex != lcId))
            {
                if (qci == dlLogicalChInfo_p->dlLcQosInfo.QCI)
                {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}
/*****************************************************************************
 * Function Name  : macIsQciUniqueForUlLc
 * Inputs         : lcId - lcIndex of the UE to be deleted
 *                : qci - QCI to be checked
 *                : ueIndex - ueIndex of the Ul UE.
 * Outputs        : None
 * Returns        : True : if the LC has only the QCI
 *                  False : If other LC have the same QCI.
 * Description    : This function checks if the QCI is unique for the LC 
 *                  In uplink.
 *****************************************************************************/
UInt8 macIsQciUniqueForUlLc(
        UInt8 lcId,
        UInt8 qci,
        UInt16 ueIndex)
{
    UInt8 lcIndex                                   = 0;
    ULUEContext              *ulUeContext_p         = PNULL;
    ULLogicalChannelInfo     *ulLogicalChInfo_p     = PNULL;
    ulUeContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    if (ulUeContext_p)
    {   
        for (lcIndex = MAC_LCID_3; lcIndex < MAX_NUMBER_OF_LOGICAL_CHANNEL; lcIndex++)
        {
            ulLogicalChInfo_p = &ulUeContext_p->lcInfo[lcIndex];
            if ((INVALID_LCID != ulLogicalChInfo_p->lchId)
                    && (lcIndex != lcId))
            {
                if (qci == ulLogicalChInfo_p->ulQosInfo.QCI)
                {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}
#endif
/* SPR 21660 changes end_vishal */
/* SPR 16916 FIX END */
