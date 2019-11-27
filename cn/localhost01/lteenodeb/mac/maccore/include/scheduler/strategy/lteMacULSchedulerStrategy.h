/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULSchedulerStrategy.h,v 1.1.1.1.6.1.6.1 2010/09/29 14:48:29 gur10694 Exp $
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
 * $Log: lteMacULSchedulerStrategy.h,v $
 * Revision 1.1.1.1.6.1.6.1  2010/09/29 14:48:29  gur10694
 * Purify UL crash Fix
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.17  2009/11/10 13:03:16  gur11912
 * modified for warning UE not present in non zero lists
 *
 * Revision 1.16  2009/07/24 12:39:09  gur20052
 * Splint Warnings are removed
 *
 * Revision 1.15  2009/07/13 11:05:05  gur20052
 * Removed code in #if 0
 *
 * Revision 1.14  2009/06/24 14:11:17  gur20052
 *  included lteMacExecutionLegs.h
 *
 * Revision 1.13  2009/06/23 16:55:58  gur20052
 * Review comments are incorporated
 *
 * Revision 1.10  2009/06/08 08:22:20  gur20052
 *  review comments incorporated
 *
 * Revision 1.9  2009/06/05 16:20:48  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.8  2009/06/05 06:33:06  gur20052
 * incorporated the Review comments
 *
 * Revision 1.7  2009/06/04 07:42:10  gur20052
 * Review comments are incorporated
 *
 * Revision 1.6  2009/06/02 07:19:29  gur20052
 * Updated during comments incorporation and dispostion
 *
 * Revision 1.5  2009/05/28 11:33:52  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.4  2009/05/25 16:20:36  gur20052
 * Updated the version to support the UL Fair and Round Robin Scheduler
 *
 * Revision 1.3  2009/05/21 17:22:24  gur20052
 * Updated the Basic Framework of UL Strategy
 *
 * Revision 1.2  2009/05/11 06:20:24  gur20052
 * Updated the function prototype
 *
 * Revision 1.1  2009/04/30 15:38:16  gur20052
 * Initial Version of Uplink Scheduler Strategy added
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_UL_SCHEDULER_STRATEGY_H
#define LTE_MAC_UL_SCHEDULER_STRATEGY_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacULUEContext.h"
#include "lteMacULRoundRobinScheduler.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacTBSize.h"
#include "lteMacULQueueLoad.h"
#include "lteMacExecutionLegs.h"
#ifdef DL_UL_SPLIT
#include "lteCircQueue.h"
#endif
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAC_UL_GRANT_FOR_AP_CQI_WITH_BSR_LOAD       5
#define MAC_UL_GRANT_FOR_AP_CQI_WITHOUT_BSR_LOAD    1
/* ULA_CHG */
#define MAC_UL_GRANT_FOR_CONTENTION_RES             1
/* Minimum grant for SR if avgMcs > 2 */
#define MAC_MIN_UL_GRANT_FOR_SR                     1
/* Minimum grant given by RR and PF scheduler */
#define MAC_SCH_MIN_UL_GRANT                        1
/* ULA_CHG */
#define MAC_MIN_UL_GRANT                            2
/* CLPC CHANGES */
#define MAC_UL_GRANT_FOR_TPC_WITH_BSR_LOAD       3

/* ULA_CHG */
/* Mimimum data size values for SR,CR and CQI(without BSR)*/
/* + SPR 6194 Fix */
#define CQI_ULGRANT_MIN_DATA_SIZE		            8
/* - SPR 6194 Fix */
#define MIN_BETWEEN_TWO(a, b) ((a) > (b) ? (b) : (a))
/* ULA_CHG */

/* CLPC CHANGES */
/* MACRO for UL grant for TPC without BSR */
#define MAC_UL_GRANT_FOR_TPC_WITHOUT_BSR_LOAD       1
/* +DYNAMIC_ICIC */
#define PHR_GRANT_DATA_SIZE                         2
#define TPC_PHR_GRANT_DATA_SIZE                     2
#define INVALID_PHR_GRANT                           0
#define MAC_UL_GRANT_TPC_WITHOUT_BSR_OR_EXPLICIT_PHR_LOAD   1
/* -DYNAMIC_ICIC */
/* + E_CID_5.3 */
#define E_CID_DUMMY_DCI0_GRANT_DATA_SIZE            2
/* - E_CID_5.3 */
/*To Calculate the Net BSR Load,which is cal;culated from the bsrNetTotal 
subtracted with the Allocations being made*/
#define NETBSRLOAD(UEContext_p)\
  ( UEContext_p->bsrNetTotal - ( \
      UEContext_p->ulresInfo[0].deltabsrLoad +\
      UEContext_p->ulresInfo[1].deltabsrLoad +\
      UEContext_p->ulresInfo[2].deltabsrLoad +\
      UEContext_p->ulresInfo[3].deltabsrLoad +\
      UEContext_p->ulresInfo[4].deltabsrLoad +\
      UEContext_p->ulresInfo[5].deltabsrLoad +\
      UEContext_p->ulresInfo[6].deltabsrLoad +\
      UEContext_p->ulresInfo[7].deltabsrLoad \
      ) )

/* ICIC changes start */
#define MAX_RETX_RB 2
/* ICIC changes end */
/* + CQI_5.0 */
#define MCS_INDEX_CQI_WITHOUT_DATA   29
#define CEIL_DIV(x,y)  (((x)-1) / y) + 1
/* - CQI_5.0 */
/*TTIB Code Optimization Start*/
 /* + E_CID_5.3 */
#define MAX_UL_STRATEGY_Q 11
 /* - E_CID_5.3 */
#ifdef TDD_CONFIG
#define PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, ueId, minRB, strictAlloc, ulGrantReqType, ttiCount, ulHarqProcessIndex, minDataSz, maxDataSz, mcs)\
            ulStrategyTxNode_p->ueIndex = ueId;\
            ulStrategyTxNode_p->requiredRB = minRB;\
            ulStrategyTxNode_p->strictAllocRB = strictAlloc;\
            ulStrategyTxNode_p->ulGrantRequestType = ulGrantReqType;\
            ulStrategyTxNode_p->ttiCounter = ttiCount;\
            ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessIndex;\
            ulStrategyTxNode_p->minDataSize = minDataSz;\
            ulStrategyTxNode_p->maxDataSize = maxDataSz;\
            ulStrategyTxNode_p->avgMcs = mcs;\
            ulStrategyTxNode_p->scheduledCount= 0;
#else
#define PREPARE_UL_STRATEGY_TXNODE(ulStrategyTxNode_p, ueId, minRB, strictAlloc, ulGrantReqType, ttiCount, minDataSz, maxDataSz, mcs)\
            ulStrategyTxNode_p->ueIndex = ueId;\
            ulStrategyTxNode_p->requiredRB = minRB;\
            ulStrategyTxNode_p->strictAllocRB = strictAlloc;\
            ulStrategyTxNode_p->ulGrantRequestType = ulGrantReqType;\
            ulStrategyTxNode_p->ttiCounter = ttiCount;\
            ulStrategyTxNode_p->minDataSize = minDataSz;\
            ulStrategyTxNode_p->maxDataSize = maxDataSz;\
            ulStrategyTxNode_p->avgMcs = mcs;
#endif
/*TTIB Code Optimization End*/

/* SPR 5713 changes start */
#define CALCULATE_MAC_MIN_UL_GRANT_RBs(avgMcs,minRB,ueComplianceRelease) \
{\
  if(RELEASE_10 == ueComplianceRelease) \
  {\
    if(avgMcs >= 1)\
    {\
      minRB = 2;\
    }\
    else\
    {\
      minRB = 3;\
    }\
  }\
  else\
  {\
    if(avgMcs >= 2)\
    {\
      minRB = 1;\
    }\
    else\
    {\
      minRB = 2;\
    }\
  }\
}

#define VALIDATE_UPLINK_RB(mcsIndex,minRBAsPerUeRelease,ulGrantMinDataSize,\
                           ueComplianceRelease)\
{\
    if(mcsIndex >=4)\
    {\
        minRBAsPerUeRelease = 1;\
    }\
    else\
    {\
        CALCULATE_MAC_MIN_UL_GRANT_RBs(mcsIndex,minRBAsPerUeRelease,\
                                       ueComplianceRelease); \
    }\
}
/* SPR 5713 changes end */

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*CA Changes start  */
extern UL_MEAS_GAPON_OFF_TRIGGER_TYPE* ulMeasGapOnOffTriggerQueue_gp[MAX_NUM_CELL]; /* MEAS_GAP_CHG */
extern LTE_LIST* nonZeroBsrList_gp[MAX_NUM_CELL];
extern UInt16 totalRetxRb_g[MAX_NUM_CELL][MAX_USER_LOCATION][NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME];
extern LTE_TREE*  ulQosStrategyLCTree_gp[MAX_NUM_CELL][MAX_VALID_UE_TYPE]; /* EICIC +-*/
extern ULStrategyScheduleInfo* ulStrategyScheduleInfo_gp[MAX_NUM_CELL];
/* ICIC changes start */
extern UInt32 maxCellEdgeUEsToBeScheduledUL_g[MAX_NUM_CELL];
/* ICIC changes end */
/*CA Changes end  */
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/*This enum is used to identify the Type of scheduling Strategy to be used*/
 typedef enum ULSchedulerStrategyTypeT
{
        /*When the Fair scheduling stratgy used*/
        UL_FAIR_SCHEDULING=0,
       /*When the Round Robin scheduling stratgy used*/
        UL_ROUND_ROBIN_SCHEDULING,
        /*When QoS scheduling is used*/
        UL_QOS_SCHEDULING,
}ULSchedulerStrategyType;

/* Structure containing all the configuration parameters needed by Uplink MAC
   scheduler as configured by the configMAC.txt file */
typedef struct ULSchedulerStrategyConfigT
{
    /*Indicates the type of Uplink scheduler Strategy to run i.e.
      Fair Scehduler Strategy or Round Robin*/
    ULSchedulerStrategyType ulSchedulerStrategy;
    /*indicates the maximum number of UEs to be  scehduled in a TTI in uplink*/
    UInt8 maxUEsToBeScheduledUL;
    /* ICIC Change Start */
    /*indicates the maximum number of Cell Edge UEs to be  scehduled in a TTI in uplink*/
    UInt8 maxCellEdgeUEsToBeScheduledUL;
    /* ICIC Change End */
    /*UPlink BSR Weight */
    UInt8   uplkUEBSRLoadWt;
    /*uplink retransmission type*/
    UInt8   uplkRetransType;
    /* Uplink UE Priority Weight*/
    UInt8   uplkUEPriorityWt;
    /* Uplink UE Delay Weight*/
    UInt8   uplkUEDelayWt;
    /* Uplink Logical Channel Group Priority Weight*/
    UInt8   uplkLCGPriorityWt;
    /* Uplink Modulation Scheme Weight*/
    UInt8   uplkMSWt;
     /* Uplink Modulation Scheme Weight for 64QAM*/
    UInt8 ulMs64QAMFactorWt;
     /* Uplink Modulation Scheme Weight for 16QAM*/
    UInt8 ulMs16QAMFactorWt;
    UInt16 ulTokenPeriodicity;
    
    /* Stores the weights for different DL strategy foctors  */
    QosStrategyWeights ulQosStrategyWeights;
    /* Uplink Modulation Scheme Weight for QPSK*/
    UInt8 ulMsQPSKFactorWt;
 
    
    UInt16 strategyBufferRB;

    UInt16 ambrBktFactor;
    
    UInt16 gbrBktFactor;
    
    /* DESIGN REVIEW CHANGES INCORPORATION */
    /* Indicates the factor for limiting the token down fall */
    /* SPR 12975 Changes Start */
    SInt8 ambrTokenLimitFactor;
    SInt8 gbrTokenLimitFactor;
    /* SPR 12975 Changes End */
    /* SPS UL profile per Interval */
    SpsULIntervalProfile spsUlIntervalProfile[SPS_INTERVAL_LIST];
                
}ULSchedulerStrategyConfig;


/* ICIC changes start */
/* SPR 4834 Fix Start */
/* SPR 4834 Fix End */
/*CA Changes start  */
extern BufferPool ulBufferPool_g[MAX_NUM_CELL];
/*CA Changes end  */
/* ICIC changes end */

/*This is node that are present in the scheduler info Array,containing the
  UE to be scheduled along with some other parameters */
typedef struct ULSchedulerDataNodeInfoT 
{
    /*UL UE Context*/
    ULUEContext*    ueULContext_p;
    /*Number of RBs which this UE got after the scheduling Algorithm*/
    UInt8           numOfRBs;
    /*Weighted Priority of the UE based on scheduling algorithm*/
    UInt32          weightedPriority;
    /*Weighted BSR Load of the UE*/
    UInt32          weightedBsrLoad;
} ULSchedulerDataNodeInfo;


/*This is the UL scheduler output in which is populated during the Fair 
  Scheduling algorithm,containing the array of UE which needs to be 
  scheduled along with the RBs  Allocated */
typedef struct ULSchedulerDataElementT 
{
    /*total number of entries in schedulerInfoArr[]*/
    UInt16  totalUE;
    /*This will maintain the sum of the Weighted Priority*/
    UInt32 sumWeightedPriority;
    /*array will store the calculated weightedPriority and UeId of each UE*/
    ULSchedulerDataNodeInfo  ulSchedulerDataNodeArr[
        MAX_UL_SCH_ARR * MAX_ADVANCE_UL_SUBFRAMES_ALLOC];
} ULSchedulerDataElement;

/*This is node of nonZeroBsrList_g */
typedef struct  nonZeroBsrListNodeT
{
    /*Node Anchor of the Node*/ 
    LTE_LIST_NODE   nodeAnchor;
    /*UE Identification in MAC*/
    UInt16          ueIndex;
}nonZeroBsrListNode;

/* Node for Explicit UL UIC Queue */
/* +DYNAMIC_ICIC */
typedef struct ExplicitULDCIAndPHRNodeT
{
    /* Node Anchor of the node */
    LTE_SNODE nodeAnchor;
    /* UE Identification index in MAC */
    UInt16      ueIndex;
    /* SPR 15909 fix start */
    tickType_t      ttiCounter;
    /* SPR 15909 fix end */
    ULGrantRequestType ulGrantRequestType;
}ExplicitULDCIAndPHRNode;

typedef struct ULUESchedulerInfoNodeT
{
    UInt16 ueIndex;
    UInt8 harqProcessId;
}ULUESchedulerInfoNode;
typedef struct ULUESchedulerInfoT
{
    /* +- SPR 18268 */
    UInt16 numOfUe;
    /* +- SPR 18268 */
    ULUESchedulerInfoNode ulUeSchedulerInfoArr[
        MAX_UL_SCH_ARR * MAX_ADVANCE_UL_SUBFRAMES_ALLOC];
}ULUESchedulerInfo;
/* -DYNAMIC_ICIC */
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* TDD Config 0 Changes Start */
void ulQosUpdateStrategyNodes(
        InternalCellIndex cellIndex
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulSubFrameNum,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        ExecutionLegFunctionArg* execArgs
#endif
        );
/* TDD Config 0 Changes End */
UInt32 processULSchedulerStrategy
        (
         /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex  cellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 changes start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 changes end */
        );
/* TDD Config 0 changes start */
#ifdef TDD_CONFIG
UInt32 mergeULStrategyNodes(
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */ 
        );
#endif
/* TDD Config 0 changes end */

/* SPR 5599 changes start (ZIP ID 129059) */
UInt32 scheduleUEsByULStrategy
/* SPR 5599 changes end (ZIP ID 129059) */
        (
        UInt32 *numberOfAvailableRBs_p,
        UInt8 maxUEsToBeScheduledUL,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
#ifdef FDD_CONFIG
        UInt8 ulDelay,
#endif
 /* +- SPR 17777 */
        /* CA Changes start */
        InternalCellIndex internalCellIndex
        /* CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

void processPendingULGrantQueue
        (
        UL_GRANT_PUSCH_QUEUE_TYPE * ulPendingQueue,
        UInt32 pendingULStrategyQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /* CA Changes start */
        InternalCellIndex  internalCellIndex
        /* CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

void processContentionResULGrantQueue
        (
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
        );

/* SRP 3608 changes start */
void processULPriorityQueue(
        UInt32 globalSrQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex,
        /*CA Changes end */
        ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );
/* SPR 3608 changes end */
void processSRULGrantQueue
(
        UInt32 globalSrQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex
        /*CA Changes end  */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
 );
/* SPR 2446 Fix Begins*/
void processCQIULGrantQueue
        (
         UInt32 cqiQueueCount,
         UInt32 * numberOfAvailableRBs_p,
         UInt8 * maxUEsToBeScheduledUL_p,
         /* SPR 15909 fix start */
         tickType_t currentGlobalTTITickCount,
         /* SPR 15909 fix end */
         UInt32 ulDelay,
         /*CA Changes start  */
         InternalCellIndex  internalCellIndex
         /*CA Changes end  */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
         );
/* SPR 2446 Fix Ends*/
UInt32 processBSRQueue
        (
        UInt32 * numberOfAvailableRBs_p ,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
        );
        
extern MacRetType putEntryInULShedulerNewTxQueue
        (
         ULStrategyTxNode *
         );
 

void processBSRZeroNonZeroTriggerQueue
        (
         /* SPR 15909 fix start */
         tickType_t currentGlobalTTITickCount,
         /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex
        /*CA Changes end  */
        );

/* SPR 5599 changes start (ZIP ID 129059) */
void putEntryInNonZeroBSRList
/* SPR 5599 changes end (ZIP ID 129059) */
        (
        UInt32 ueIndex,
        ULUEContext* ueULContext_p,
        /*CA Changes start  */
        InternalCellIndex  cellIndex
        /*CA Changes end  */
        );


/* SPR 7435 Changes Starts */
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType deleteEntryFromNonZeroBSRList
/* SPR 5599 changes end (ZIP ID 129059) */
/* SPR 7435 Changes Ends */
        (
        ULUEContext * ueULContext_p,
        LTE_LIST_NODE * tempNode_p,
        /*CA Changes start  */
        InternalCellIndex  cellIndex
        /*CA Changes end  */
        );

/* SPR 2446 Fix Begins*/
extern UInt32 ulProportionalFairScheduler
        (
         /*SPR 21068 start */
         tickType_t ulReferenceFlag,
         /*SPR 21068 end   */
         UInt32 * numberOfAvailableRBs_p,
         UInt8 maxUEsToBeScheduledUL,
         /* SPR 15909 fix start */
         tickType_t currentGlobalTTITickCount
         /* SPR 15909 fix end */
#ifdef FDD_CONFIG
         ,UInt8 ulDelay
#endif

         /*CA Changes start */
         ,InternalCellIndex internalCellIndex
         /*CA Changes end */
         /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
         ,UInt8 ulSubFrameNum
#endif
         /* TDD Config 0 Changes End */
         );
/* SPR 2446 Fix Ends*/
/* SPR 2446 Fix Begins*/ 
extern UInt32 ulRoundRobinScheduler 
        (
         /*SPR 21068 start */
         tickType_t ulReferenceFlag,
         /*SPR 21068 end   */
         UInt32 * numberOfAvailableRBs_p,
         UInt8 maxUEsToBeScheduledUL,
         /* SPR 15909 fix start */
         tickType_t currentGlobalTTITickCount
         /* SPR 15909 fix end */
#ifdef FDD_CONFIG
        ,UInt8 ulDelay
#endif
         /*CA Changes start */
         ,InternalCellIndex internalCellIndex
         /*CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );
/* SPR 2446 Fix Ends*/
        
extern UInt32 validateUplinkRB
        (
         ULUEContext* ueULContext_p,
         UInt32 allocatedRB,
         UInt32 numberOfAvailableRBs_p ,
         UInt32* strictAllocRBFlag,
         UInt32* minDataSize,
         UInt32* maxDataSize,
         /* ATB changes Start */
         UInt8 *mcsVal,
         /*SPR 16417 Changes Start*/
#ifdef FDD_CONFIG
         /* SPR 13532 Fix Start */
         /* SPR 15909 fix start */
         tickType_t currentGlobalTickWithULDelay,
         /* SPR 13532 Fix End */
#elif TDD_CONFIG
         tickType_t currentGlobalTTITickCount,
         /* SPR 15909 fix end */
         UInt8 ulSubFrameNum,
#endif
         /*SPR 16417 Changes End*/
         /* ATB changes End */
         /* CA changes start */
         InternalCellIndex internalCellIndex
         /* CA changes end */
        );

/*CA Changes start  */
/* + CQI_5.0 */        
extern UInt8 determineRBsForAperiodicCQIReports(ULUEContext* ulUEContext_p,
                                                UInt32 *minDataSize_p,
                                                InternalCellIndex internalCellIndex);
/* - CQI_5.0 */        

void initULSchedulerStrategy(UInt8 numCellsConfigured);

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType putEntryInULAperiodicCQIQueue
/* SPR 5599 changes end (ZIP ID 129059) */
         (
         UInt32 ueIndex,
         InternalCellIndex cellIndex
         /* CA_phase2_csi_code Start */
         ,UInt8 bitMaskForAperiodicReport
         /* CA_phase2_csi_code End */
         );

extern void initULSchedulerStrategy(UInt8 numCellsConfigured);
extern void cleanupULSchedulerStrategy(InternalCellIndex cellIndex);
/* CA Changes end  */

/* UL_SYNC_LOSS_CHG, DRX_CHG and MEAS_GAP_CHG */
/* CA changes start */
void  deleteEntryFromNonZeroBSRListTemporarily(ULUEContext* ueULContext_p,
        InternalCellIndex internalCellIndex);

#ifdef OVERSHOOT_CORRECTION
void modifiedNoOfUEsAfterOverLoadUL(UInt8  *modifiedUEs_p,
                        InternalCellIndex  internalCellIndex);
/* CA changes end */

/* +- SPR 17777 */
#endif

/* CLPC CHANGES */
    /* +DYNAMIC_ICIC */
void processExplicitULDCIAndPHRScheduleQueue (
    /* -DYNAMIC_ICIC */
        UInt32 explicitDCIULQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE * explicitULDCIAndPHRScheduleQueue_p
#ifdef FDD_CONFIG
        ,UInt32 ulDelay
#endif
        /*CA Changes start */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );
/* TTIB_Code Start */
#ifdef FDD_CONFIG 
MacRetType processMeasGapValidation(ULUEContext *ulUEContext_p, 
                                    /* SPR 15909 fix start */
                                    tickType_t currentGlobalTTITickCount, 
                                    /* SPR 15909 fix end */
                                    UInt32 ulDelay);
/* SPR 15909 fix start */
void processTtiBundleUes(tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
                         UInt32 ulDelay,
                         UInt32 * numberOfAvailableRBs_p,
                         UInt8 * maxUEsToBeScheduledUL_p,
                         /*CA Changes start  */
                         InternalCellIndex cellIndex
                         /*CA Changes end  */
                         );
/* SPR 15909 fix start */
MacRetType processTtiBHarqCheck(tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
                                  UInt32 ulDelay,ULUEContext *ulUEContext_p);
/* SPR 15909 fix start */
UInt32 getRequiredRBForTtiBundling(tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
                                  UInt32 ulDelay,
                                  /* CA Changes start */
                                  InternalCellIndex internalCellIndex);
                                  /* CA Changes end */
#endif
/* TTIB_Code End */

/* + E_CID_5.3 */
/*SPR 22919 Start*/
 void processDummyDCI0ListFailureQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/*SPR 22919 Start*/
 void processDummyDCI0ListQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/* - E_CID_5.3 */
/* Cyclomatic Complexity changes - starts here*/
/*SPR 22919 Start*/
 void processUlGrantPUSCHFailureQueueFn(UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/*SPR 22919 Start*/
 void processUlGrantPendingQueueFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/*SPR 22919 Start*/
 void processUlGrantContentionResQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/*SPR 22919 Start*/
 void processUlHighPriorityGrantQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 *  maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/*SPR 22919 Start*/
 void processUlGrantGlobalSrQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/*SPR 22919 Start*/
 void processExplicitULPuschTpcScheduleFailureQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/*SPR 22919 Start*/
 void processExplicitULPuschTpcScheduleQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/*SPR 22919 Start*/
 void processUlGrantAperiodicCqiQueue_gFn (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/*SPR 22919 Start*/
 void processScheduleUEsByULStrategyFN (UInt32 * numberOfAvailableRBs_p,
/*SPR 22919 End*/
        UInt8 * maxUEsToBeScheduledUL_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 ulDelay,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        ,ExecutionLegFunctionArg* execArgs
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

void cleanUpUlGrantQ(UL_GRANT_PUSCH_QUEUE_TYPE * ulGrantQueue_p);
void cleanUpUlGrantConResQ(InternalCellIndex internalCellIndex);
void cleanUpAperiodicUlCqiQ(InternalCellIndex internalCellIndex);
void cleanUpBsrTriggerQ(InternalCellIndex internalCellIndex);
void cleanUpNonZeroBsrList(InternalCellIndex internalCellIndex);
void cleanUpCEUserPriorityQ(InternalCellIndex internalCellIndex);
void cleanUpExpilicitPuschTpcScheduleQ(
            EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE *explicitULPuschTpcScheduleQueue_p);
void cleanUpDrxOnOffTriggerQ(InternalCellIndex internalCellIndex);
void cleanUpMeasGapOnOffTriggerQ(InternalCellIndex internalCellIndex);
void cleanUpUlSpsQueues(InternalCellIndex internalCellIndex);

typedef MacRetType (*macUlStrategyGrantHandlerFuncT) ( 
        UInt32 *ulGrantPendingQueueCount,
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
        ,UInt8 ulHarqProcessId
        ,UInt8 ulSubFrameNum
        /* +- SPR 17777 */
#endif
        /* TDD Config 0 Changes End */
        );

/* State machine to handle UL Grant Types actions for various Grants */
extern macUlStrategyGrantHandlerFuncT ulGrantStateMachine [INVALID_GRANT];

#endif  /*LTE_MAC_UL_SCHEDULER_STRATEGY_H */
