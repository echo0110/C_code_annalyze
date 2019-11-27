/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLSchedulerStrategy.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacDLSchedulerStrategy.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.23  2009/11/10 12:45:39  gur11912
 * modified for warning UE not present in non zero lists
 *
 * Revision 1.22  2009/07/13 11:05:05  gur20052
 * Removed code in #if 0
 *
 * Revision 1.21  2009/07/02 08:40:39  gur20052
 * Updated the TxModeTBAndHarqStatus , added one new enum HARQ_NOT_FREE_TX_3_4_ONE_NACK_SECOND_TA_DRX
 *
 * Revision 1.20  2009/06/29 15:09:34  gur20052
 * UT Defects are removed
 *
 * Revision 1.19  2009/06/24 14:18:50  gur20052
 * Included lteMacUEConfMgr.h
 *
 * Revision 1.18  2009/06/24 14:13:10  gur20052
 *  Removed the TA Node and DRX Node and moved to respective header files
 *
 * Revision 1.17  2009/06/23 16:55:58  gur20052
 * Review comments are incorporated
 *
 * Revision 1.15  2009/06/17 12:22:04  gur20052
 * extern LTE_SQUEUE dlHARQZeroNonZeroQueueLoadTriggerQueue;
 *
 * Revision 1.14  2009/06/08 09:58:43  gur20052
 * Removed the linking errors
 *
 * Revision 1.13  2009/06/08 08:22:20  gur20052
 *  review comments incorporated
 *
 * Revision 1.12  2009/06/05 16:20:48  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.11  2009/06/05 06:33:06  gur20052
 * incorporated the Review comments
 *
 * Revision 1.10  2009/06/04 07:42:10  gur20052
 * Review comments are incorporated
 *
 * Revision 1.9  2009/06/02 07:19:29  gur20052
 * Updated during comments incorporation and dispostion
 *
 * Revision 1.7  2009/05/28 11:33:52  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.6  2009/05/26 10:16:52  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.5  2009/05/25 16:20:36  gur20052
 * Updated the version to support the UL Fair and Round Robin Scheduler
 *
 * Revision 1.4  2009/05/21 06:25:59  gur20052
 * Added code for Round Robin Scheduler
 *
 * Revision 1.3  2009/05/19 14:29:57  gur20052
 * Updated with DL Fair Scheduler module specific changes
 *
 * Revision 1.2  2009/05/11 06:20:24  gur20052
 * Updated the function prototype
 *
 * Revision 1.1  2009/04/30 16:25:10  gur20052
 * Added initial version of  DL Scheduler Strategy Header File
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_DL_SCHEDULER_STRATEGY_H
#define LTE_MAC_DL_SCHEDULER_STRATEGY_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacDLUEContext.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacDLQueueLoadMgr.h"
#include "lteMacTBSize.h"
#include "lteMacTimingAdvanceMgr.h"
#include "lteMacUEConfMgr.h"
#include "lteCircQueue.h"
#include "lteMacDLSpsScheduler.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* RETX_CHG */
extern LTE_SQUEUE *dlRetransmissionQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE *dlRetransmissionFailureQueue_gp[MAX_NUM_CELL];;
/* RETX_CHG */

/* extern DL_MEAS_GAP_ON_OFF_TRIGGER_TYPE* dlMeasGapOnOffTriggerQueue_gp[MAX_NUM_CELL]; */ /* MEAS_GAP_CHG */

extern LTE_TREE* dlPriorityLcTree_gp[MAX_NUM_CELL][MAX_VALID_UE_TYPE];/*EICIC +-*/
/* ICIC Change Start */
extern UInt32 maxCellEdgeUEsToBeScheduledDL_g[MAX_NUM_CELL];
extern LTE_SQUEUE* dlCellEdgeUserPriorityQueue_gp[MAX_NUM_CELL];
extern EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE* explicitDLPucchTpcScheduleFailureQueue_gp[MAX_NUM_CELL];
/* ICIC Change End */
extern TA_Q_TYPE* pendingTAQueue_gp[MAX_NUM_CELL];
extern DRX_Q_TYPE *dlDRXQueue_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE *dlPriorityQueue_gp[MAX_NUM_CELL];
extern DRX_Q_TYPE* pendingDRXQueue_gp[MAX_NUM_CELL];
/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* ICIC changes start */
#define MIN_RB_REQUIRED_FOR_SCHEDULING 1
/* ICIC changes  end*/
#define MAX_BETWEEN_TWO(a, b) ((a) < (b) ? (b) : (a))
#define MIN_BETWEEN_TWO(a, b) ((a) > (b) ? (b) : (a))
#define MIN_BETWEEN_THREE(a, b, c) \
            (((a) < (b) && (a) < (c)) ? (a): (((b) < (c)) ? (b) : (c)))
/*SPR 1273 START*/
#define NET_UE_QL(uC_p)\
                     (SInt32)(uC_p->ueQueueLoad-(uC_p->assignedRBQueueLoad[0]))
/*SPR 1273 END*/
                                                 
/*                                         uC_p->assignedRBQueueLoad[1]+\
                                         uC_p->assignedRBQueueLoad[2]+\
                                         uC_p->assignedRBQueueLoad[3]))
*/
#ifdef TDD_CONFIG
#define SP_FRAME_FACTOR 3


#define SP_SUBFRAME_PRB(prb)\
	MAX_BETWEEN_TWO((DIVIDE_BY_FOUR(prb * SP_FRAME_FACTOR)), 1)

#define FOUR_BY_THREE(prb)\
    ((prb<<2))/SP_FRAME_FACTOR

#endif

typedef enum ReTransmissionTypeT
{
    RETX_REQUIRED_ON_PCELL_ONLY,
    RETX_REQUIRED_ON_SCELL_ONLY,
    RETX_REQUIRED_ON_BOTH_CELLS,
    INVALID_RETX_TYPE
}ReTransmissionType;

typedef enum ReTransmissionStateT
{
    RETX_ALLOC_SUCC_ON_PCELL,
    RETX_ALLOC_SUCC_ON_SCELL,
    RETX_ALLOC_SUCC_ON_BOTH_CELLS,
    RETX_ALLOC_FAIL_ON_PCELL,
    RETX_ALLOC_FAIL_ON_SCELL,
    RETX_ALLOC_FAIL_ON_BOTH_CELLS,
    INVALID_RETX_STATE
}ReTransmissionState;

#define SET_RETX_STATE_ON_RB_ALLOC_FAIL(reTransmissionType, reTransmissionState)\
{\
    if( reTransmissionType == RETX_REQUIRED_ON_PCELL_ONLY )\
    {\
        reTransmissionState = RETX_ALLOC_FAIL_ON_PCELL;\
    }\
    else if(reTransmissionType == RETX_REQUIRED_ON_SCELL_ONLY)\
    {\
        reTransmissionState = RETX_ALLOC_FAIL_ON_SCELL;\
    }\
    else if(reTransmissionType == RETX_REQUIRED_ON_BOTH_CELLS )\
    {\
        reTransmissionState = (!IS_PCELL_OF_UE(dlUEContext_p,cellIndex))? \
                                RETX_ALLOC_FAIL_ON_SCELL:\
                                RETX_ALLOC_FAIL_ON_PCELL;\
    }\
}                

#define SET_RETX_STATE_ON_RB_ALLOC_SUCCESS(reTransmissionType, reTransmissionState)\
{\
    if(IS_PCELL_OF_UE(dlUEContext_p,cellIndex))\
    {\
        reTransmissionState = RETX_ALLOC_SUCC_ON_PCELL;\
    }\
    else\
    {\
        reTransmissionState = (reTransmissionType == RETX_REQUIRED_ON_BOTH_CELLS && reTransmissionState == RETX_ALLOC_SUCC_ON_PCELL)? \
                                RETX_ALLOC_SUCC_ON_BOTH_CELLS:\
                                RETX_ALLOC_SUCC_ON_SCELL;\
    }\
}                
/* - SPR 14341 Changes */
/* Cyclomatic_complexity_changes_start */
UInt32 getTempLength(MsgType msgType);
void CleanupSPSStrategyQueues( InternalCellIndex internalCellIndex, void *dlSpsNode_p);
/* SPR 17777 */
void  pushInPendingQueueAsPerMsgType(MsgType msgType,DLUEContext * ueDLContext, InternalCellIndex internalCellIndex);
/* SPR 17777 */
UInt32 validateUeStateForDrxQueueProcessing( DLUEContextInfo * ueDLContextInfo_p,
                    /* SPR 15909 fix start */
                    tickType_t currentGlobalTTITickCount,
                    /* SPR 15909 fix end */
                    InternalCellIndex internalCellIndex);
UInt32 validateUeForScheduling(DRXContext *drxContext_p, DLUEContext *dlUEContext_p, MeasGapContextDL *measgapContext_p);

#ifdef TDD_CONFIG
void checkSplSfAndCalcTbsizeSiso(   UInt8  isSpecialSubframe,
                                    UInt8  mcsIndex,
                                    UInt8  allocatedRB,
                                    UInt32 *tbSize
                                 );
void checkSplSfAndCalcTbsizeMimo(   UInt8        isSpecialSubframe,
                                    DLUEContext* ueDLContext_p,
                                    UInt8        allocatedRB,
                                    UInt32       *tbOneSize,
                                    UInt32       *tbTwoSize,
                                    UInt8        numOfLayer);
#endif
/* Cyclomatic_complexity_changes_end */


/* ICIC changes start */
/* + Coverity 24528 , 24529, 24521, 24520 Fix */
/* - Coverity 24528 , 24529, 24521, 24520 Fix */

#ifdef MAC_AUT_TEST
#define PUSH_IN_PENDING_QUEUE_AS_PER_MSG_TYPE(msgType,listIndex,ueDLContext,internalCellIndex)\
if(TA_MSG == msgType || TA_DRX_MSG == msgType)\
 {\
     TimeAdvanceQueueNode *taNode_p = PNULL;\
     LOG_UT(MAC_RR_RB_NOT_AVAILABLE_PUSHING_IN_TA_Q,LOGDEBUG,MAC_DL_Strategy,\
            getCurrentTick(),\
            listIndex,ueDLContext->ueIndex,\
            DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,LINE_NUMBER,\
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
            FUNCTION_NAME,"");\
     GET_MEM_FROM_POOL(TimeAdvanceQueueNode, taNode_p,\
             sizeof(TimeAdvanceQueueNode), PNULL);\
     taNode_p->ueIdx = ueDLContext->ueIndex;\
     taNode_p->timeAdvanceValue = ueDLContext->timingAdvanceValue;\
     ENQUEUE_TA_Q(pendingTAQueue_gp[internalCellIndex], TimeAdvanceQueueNode, taNode_p );\
 }\
else if(TPC_WITH_DATA == msgType)\
{\
	ExplicitDLDCIQueueNode * dlDciNode_p = PNULL;\
	GET_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(\
                    dlDciNode_p,ExplicitDLDCIQueueNode);\
	dlDciNode_p->ueIndex = ueDLContext->ueIndex; \
	if(ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(&explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],\
                    ExplicitDLDCIQueueNode, &(dlDciNode_p->nodeAnchor)))\
	{\
		FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(dlDciNode_p);\
	}\
}
#endif
extern BufferPool dlBufferPool_g[MAX_NUM_CELL] ;
/* ICIC changes end */


/* + CQI_5.0 */
/** klockwork Changes Start **/
#define POPULATE_INFO_FOR_ONE_TB(transmissionMode,TBValid,tempStrategyInfo,\
                                 /* + TM7_8 Changes Start */ \
                                 beamFormingAlgoToBeUsed,internalCellIndex)\
                                 /* - TM7_8 Changes End */ \
{\
    \
       if(1 == TBValid)\
        {\
                   tempStrategyInfo.isTBOneValid = TRUE;\
                   tempStrategyInfo.isTBTwoValid = FALSE;\
                   tempStrategyInfo.txIndicatorForTB1 = NEW_TX;\
                   tempStrategyInfo.txIndicatorForTB2 = INVALID_TX;\
        }\
        /* + TM7_8 Changes Start */ \
        if(TX_MODE_7 == transmissionMode) \
        {\
           if((BF_ALGO_NOT_AVAILABLE == beamFormingAlgoToBeUsed)&& \
              (BF_ALGO_NOT_AVAILABLE == \
               (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->bfVectorPresentFlag))\
           {\
             tempStrategyInfo.primaryDCIFormat = DCI_FORMAT_1A;\
             tempStrategyInfo.secondaryDCIFormat = INVALID_DCI_FORMAT;\
           }\
           else\
           {\
             tempStrategyInfo.primaryDCIFormat = DCI_FORMAT_1;\
             tempStrategyInfo.secondaryDCIFormat = DCI_FORMAT_1A;\
           }\
        }\
        else if(TX_MODE_8 == transmissionMode) \
        {\
           if((BF_ALGO_NOT_AVAILABLE == beamFormingAlgoToBeUsed) &&\
              (BF_ALGO_NOT_AVAILABLE == \
               (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->bfVectorPresentFlag))\
           {\
             tempStrategyInfo.primaryDCIFormat = DCI_FORMAT_1A;\
             tempStrategyInfo.secondaryDCIFormat = INVALID_DCI_FORMAT;\
           }\
           else\
           {\
             tempStrategyInfo.primaryDCIFormat = DCI_FORMAT_2B;\
             tempStrategyInfo.secondaryDCIFormat = DCI_FORMAT_1A;\
           }\
        }\
        /* - TM7_8 Changes End */ \
        else \
        { \
        /* - TM6_5.2 */ \
        tempStrategyInfo.primaryDCIFormat = \
        macParams_g.preferredPrimaryDCIFormatForSingleTB[transmissionMode];\
     \
        tempStrategyInfo.secondaryDCIFormat = \
        macParams_g.preferredSecondaryDCIFormatForSingleTB[transmissionMode];\
        /* + TM6_5.2 */ \
        }\
        /* - TM6_5.2 */ \
    \
}
/** klockwork Changes End **/

/* + TM7_8 Changes Start */
#ifdef TDD_CONFIG 

#define CHECK_SF_MAPPED_ON_BCH_PSS_SSS(scheduledTick,transmissionMode)\
        /*check cellConfig_g specialSubfarme, check if it is same*/ \
        ((((0 == (scheduledTick % MAX_NUM_OF_SUBFRAME)) && (transmissionMode == TX_MODE_7))||\
                ((transmissionMode == TX_MODE_8) && \
                 ((0 == (scheduledTick % MAX_NUM_OF_SUBFRAME)) || \
                  (1 == (scheduledTick % MAX_NUM_OF_SUBFRAME)) || \
                  (5 == (scheduledTick % MAX_NUM_OF_SUBFRAME))|| \
                  (6 == (scheduledTick % MAX_NUM_OF_SUBFRAME))/*subframe 0,1,5,6 with pss and sss*/)))? TRUE:FALSE) 
#endif

#ifdef FDD_CONFIG

#define CHECK_SF_MAPPED_ON_BCH_PSS_SSS(scheduledTick,transmissionMode)\
            ((((0 == (scheduledTick % MAX_NUM_OF_SUBFRAME)) ||\
               (5 == (scheduledTick % MAX_NUM_OF_SUBFRAME))) && \
                    ((transmissionMode == TX_MODE_7) || \
                     (transmissionMode == TX_MODE_8))) ? TRUE:FALSE) 
#endif 


/* - TM7_8 Changes End */



typedef struct TempStrategyInputInfoForOneTBT
{
  UInt8 isTBOneValid;
  UInt8 isTBTwoValid;
  UInt8 txIndicatorForTB1;
  UInt8 txIndicatorForTB2;
  DCIFormat primaryDCIFormat;
  DCIFormat secondaryDCIFormat;
}TempStrategyInputInfoForOneTB;

/* - CQI_5.0 */


/*This enum is used to identify the Type of scheduling Strategy to be used*/
 typedef enum DLSchedulerStrategyTypeT
{
    /*When the Fair scheduling stratgy used*/
    DL_FAIR_SCHEDULING=0,
    /*When the Round Robin scheduling stratgy used*/
    DL_ROUND_ROBIN_SCHEDULING,
    /*When the Round Robin scheduling stratgy used*/
    DL_QOS_BASED_SCHEDULING
}DLSchedulerStrategyType;

/* Structure containing all the configuration parameters needed by Downlink MAC
scheduler as configured by the configMAC.txt file */
typedef struct DLSchedulerStrategyConfigT
{
    /*Indicates the type of Downlink scheduler Strategy to run i.e. 
      Fair Scehduler Strategy or Round Robin*/
    DLSchedulerStrategyType dlSchedulerStrategy;
    /*indicates the maximum number of UEs to be  scehduled in a TTI in downlink*/
    UInt8 maxUEsToBeScheduledDL;
    /* ICIC Change Start */
    /*indicates the maximum number of Cell Edge UEs to be  scehduled in a TTI in downlink*/
    UInt8 maxCellEdgeUEsToBeScheduledDL;
    /* ICIC Change End */
    /* Downlink UE Queue Load Weight*/
    UInt8   dwlkUEQLoadWt;
    /* Downlink Logical Channel Queue Load Weight*/
    UInt8   dwlkLCQLoadWt;
    /* Downlink UE Priority Weight*/
    UInt8   dwlkUEPriorityWt;
    /* Downlink Logical Channel Priority Weight*/
    UInt8   dwlkLCPriorityWt;
    /* Downlink UE Delay Weight*/
    UInt8   dwlkUEDelayWt;
    /* Downlink Logical Channel Delay Weight*/
    UInt8   dwlkLCDelayWt;
    /* Downlink CQI Weight*/
    UInt8   dwlkcqiWt;
    /* Downlink Modulation Scheme Weight*/
    UInt8   dwlkMSWt;
    /* Downlink Modulation Scheme Weight for 64QAM*/
    UInt8   dlMs64QAMFactorWt;
    /* Downlink Modulation Scheme Weight for 16QAM*/
    UInt8   dlMs16QAMFactorWt;
    /* Downlink Modulation Scheme Weight for QPSK*/
    UInt8   dlMsQPSKFactorWt;
    /* + CL-MIMO LJA*/
    /*Used to choose between spatial multiplexing and 
     *transmit diversity for Transmission mode 4
     *Range is 0..15*/
    UInt8   cqiThreshold[MAX_USER_LOCATION];
    /*4x4 DL MIMO CHG START*/
    UInt8   cqiThresholdCellCenterLayer3[MAX_USER_LOCATION];
    UInt8   cqiThresholdCellCenterLayer4[MAX_USER_LOCATION];
    /*4x4 DL MIMO CHG START*/
    /* SPR 7981 changes start */
    UInt8   mcsThreshold[MAX_USER_LOCATION];
    /* SPR 7981 changes end */
    /* - CL-MIMO LJA*/

    UInt8 strategyBufferRB;
    /* Qos Related Params */
    /* Indicates the factor for calculating the bucket depth for GBR LC's */
    UInt8 gbrBktFactor;
    /* Indicates the factor for calculating the bucket depth for AMBR */
    UInt8 ambrBktFactor;
    /* Indicates the EPC packet delay to be considered */
    UInt8 epcPacketDelay;
    /* Indicates Token update periodicity */
    UInt16 dlTokenPeriodicity;
    /* Indicates Bit Map for strict scheduling, 
     * each bit(0-8) mapping each QCI(1-9)
     */
    UInt16 stictScheduleQciMap;

    /* Stores the weights for different DL strategy foctors  */
    QosStrategyWeights dlQosStrategyWeights;
    /* DESIGN REVIEW CHANGES INCORPORATION */
    /* Indicates the factor for limiting the token down fall */
    /* SPR 11115 Fix Start */
    SInt8 ambrTokenLimitFactor;
    SInt8 gbrTokenLimitFactor;
    /* SPR 11115 Fix End */
    /* qos changes stop */
    /* temp_logs */
    UInt8 muxLogs;
    UInt8 muxThroughput;
    /* SPS DL profile per Interval */
    SpsDLIntervalProfile spsDlIntervalProfile[SPS_INTERVAL_LIST];
}DLSchedulerStrategyConfig;

    
                                                  
/*This Enum is used to identify the whther Harq is free or not,Transmission 
  mode,Zero or non Zero Queue Load .Based on the different combination of these,
  This enum will be used for selecting the DCI Format*/
typedef enum TxModeTBAndHarqStatusT
{
    /*when Harq Free,No Nack Pending,and Zero Queue Load*/
    HARQ_FREE_TX_GENERAL_NO_NACK_ZERO_QUEUE_LOAD = 0,
    /*when Harq Free,TX Mode 3*/
    HARQ_FREE_TX_3,
    /*when Harq Free,TX Mode 4*/
    HARQ_FREE_TX_4,
    /*when Harq Free,TX Mode either 3 or 4*/
    HARQ_FREE_TX_3_4,
    /*when Harq Free,TX Modes other than 3 or 4*/
    HARQ_FREE_TX_OTHER_THAN_3_4,
    /*when Harq is not Free,TX Modes either 3 or 4,Only One TBs Nack is pending*/
    HARQ_NOT_FREE_TX_3_4_ONE_NACK,
    /*when Harq is not Free,TX Modes either 3 or 4,Only One TBs Nack is pending,
      Second used for TA DRX*/
    HARQ_NOT_FREE_TX_3_4_ONE_NACK_SECOND_TA_DRX,
    /*when Harq is not Free,TX Modes either 3 or 4,Only One TBs Nack is pending,
      Non-Zero Queue Load*/
    HARQ_NOT_FREE_TX_3_4_ONE_NACK_NON_ZERO_QUEUE_LOAD,
    /*when Harq is not Free,TX Modes either 3 or 4,Only One TBs Nack is pending,
    Second Invalid Zero Queue Load*/
    HARQ_NOT_FREE_TX_3_4_ONE_NACK_SECOND_INVALID_ZERO_QL,
    /*when Harq is not Free,TX Modes either 3 or 4,Only One TBs Nack is pending,
      Zero Queue Load*/
    HARQ_NOT_FREE_TX_3_4_ONE_NACK_ZERO_QUEUE_LOAD,
    /*when Harq is not Free,TX Modes either 3 or 4, Two TBs Nack are pending*/
    HARQ_NOT_FREE_TX_3_4_TWO_NACK,
    /*when Harq is not Free,TX Modes other than 3 or 4*/
    HARQ_NOT_FREE_TX_OTHER_THAN_3_4,
    /* + TM7_8 Changes Start */
    HARQ_FREE_TX_3_4_8,
    /* - TM7_8 Changes End */
    INVALID_STATUS = 255
} TxModeTBAndHarqStatus;



/*This Enum is used to identify the whether TBs are of New Transmission or 
  Retransmission type for both the TB one and TB two*/
typedef enum TBOneTBTwoReTxNewInfoT
{
    /*When TB one and TB Two are of New Transmission Type*/
    TB_ONE_NEW_TB_TWO_NEW = 0,
    /*When TB one is of New Transmission;TB two is of Retransmission type*/
    TB_ONE_NEW_TB_TWO_RETX,
    /*When TB one is INVALID and TB two is of Retransmission type*/
    TB_ONE_INVALID_TB_TWO_RETX,
    /*When TB one is of Retransmission ;TB two is of New Transmission*/
    TB_ONE_RETX_TB_TWO_NEW,
    /*When both TB one and TB Two are of Retransmission Type*/
    TB_ONE_RETX_TB_TWO_RETX,
     /*When TB one is of Retransmission type and TB two is of INVALID*/
    TB_ONE_RETX_TB_TWO_INVALID
}TBOneTBTwoReTxNewInfo;


/*This enum is used to identify the Node in between the Pending and Fresh of TA
  and DRX Types FlagForTADRXPendingFresh  */
typedef enum FlagForPendingTADRXT 
{
    /* Represents the Pending TA Node*/
    PENDING_TA_FLAG=0,
    /* Represents the Pending DRX Node*/
    PENDING_DRX_FLAG,
    /* Represents the Fresh/New TA Node*/
    NEW_TA_FLAG,
    /* Represents the Fresh/New DRX Node*/
    NEW_DRX_FLAG
}FlagForPendingTADRX;

typedef enum FlagForPendingScellCET
{
    /* Represents the Pending Scell MAC CE Node */
    PENDING_SCELL_CE_FLAG = 0,
    /* Represents the New Scell MAC CE Node */
    NEW_SCELL_CE_FLAG
}FlagForPendingScellCE;

/*This is node that are present in the scheduler info Array,containing the 
  UE to be scheduled along with some other parameters */
typedef struct DLSchedulerDataNodeInfoT
{
    /*DL UE Context*/
    DLUEContext* ueDLContext_p;
    /*Weighted Priority of the UE based on scheduling algorithm*/
    UInt32     weightedPriority;
    /*Weighted Queue Load of the UE*/
    UInt32     weightedQueueLoad;         
    /*Sum of Logical Channel priority of UE*/
    UInt32     sumLCPriority;         
    /*This Will contain the different type of MsgType*/
    MsgType    msgType;
    /*Number of RBs which this UE got after the scheduling Algorithm*/
    UInt8     numOfRb;
    /*Harq Process ID of the UE*/
    UInt8     harqProcessId;
    /*number of logical channel ID with the non Zero Queue Load of the UE*/
    UInt8      numLCWithNonZeroQueueLoad;     
}DLSchedulerDataNodeInfo;

/*This is the DL scheduler output which is populated during the algorithm,
  containing the array of UE which needs to be scheduled along with the RBs
  Allocated */
typedef struct DLSchedulerDataElementT
{   
    /*This will maintain the sum of the Weighted Priority for CC and CE user*/
    UInt32 sumWeightedPriority[MAX_USER_LOCATION];
    /*total number of entries in schedulerInfoArr[] with either TA or DRX or both*/
    UInt16 countTADRX;
    /* SRP 3608 changes start */
    /*total number of entries in schedulerInfoArr[] with High Priority Data*/
    UInt16 countHighPriorityData;
    /* SRP 3608 changes end */
    /*total number of entries in schedulerInfoArr[]*/
    UInt16 dataNodeArrTotalUE;
    /*array will store the calculated weightedPriority and UeId of each UE*/
    DLSchedulerDataNodeInfo dlSchedulerDataNodeArr[
       MAX_DL_SCH_ARR * MAX_ADVANCE_DL_SUBFRAMES_ALLOC]; 
}DLSchedulerDataElement;


/*Its Node structure of the  ZeroNonZeroTriggerQueue*/
typedef struct ZeroNonZeroTriggerNodeT
{
    /*CAUTION: Required circular queue enqueue changes if Anchor node moved down in the structure */
    LTE_SNODE        ZeroNonZeroTriggerNodeAnchor;
    UInt16           ueIdx;
    /*Its is used to differentiate between the Adding or deleting UE*/
    AddDeleteTrigger addDeleteTrigger;
    /* DL qos strategy start */
    /* Stores the number of LC triggers */
    UInt8 triggerLcCount;
    /* stores the trigger for LC.s of the UE along with trigger for Add/Delete */
    AddDelTrigLCNode qosLcTrigger[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    /* DL qos strategy stop */
}ZeroNonZeroTriggerNode;

/*This is Node data type of the NonZeroQueueLoadAndHarqList */
typedef struct NonZeroQueueLoadAndHarqNodeT
{
    LTE_LIST_NODE nodeAnchor;
    UInt16        ueIndex;
}NonZeroQueueLoadAndHarqNode;
/* Queue node for Explicit DL DCI queue */
typedef struct ExplicitDLDCIQueueNodeT
{
    /*CAUTION: Required circular queue enqueue changes if Anchor node moved down in the structure */
    LTE_SNODE nodeAnchor;
    UInt16 ueIndex;
}ExplicitDLDCIQueueNode;
/* ICIC Change Start */
/* Queue node for DL ADVANCE queue */
typedef struct DLAdvanceQueueNodeT
{
    LTE_SNODE dlAdvanceAnchorNode;
    UInt8 isNodeInvalid;

    /* DL strategy node */
DLStrategyTxNode *dlStrategyTxNode;
}DLAdvanceQueueNode;


/* ICIC Change End */

/* SRP 3608 changes start */
/*Node structure of the Priority Queue */
typedef struct DLPriorityQueueNodeT
{
    /*UE ID of the UE*/
    UInt16      ueIndex;
}DLPriorityQueueNode;
/* SPR 3608 changes end */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern LTE_LIST  ueNonZeroQueueLoadAndHarqList; 

/* A list of UEs which have non zero queueLoad or UEs whose queueLoad is zero
      but their harq processes are active*/
extern  LTE_LIST  *dlUENonZeroQueueLoadAndHarqList_gp[MAX_NUM_CELL];

/*A pointer to nodeAnchor of the UE in ueNonZeroQueueLoadList_g*/
extern LTE_LIST_NODE * dlNonZeroListStartRoundRobin_p[MAX_NUM_CELL];

/*Queue of type LTE_SQUEUE which contains HARQ Triggers of UEs trigger to
    enter/to remove from / into the NonZeroQueueLoadAndHarqList */
extern LTE_SQUEUE dlHARQZeroNonZeroQueueLoadTriggerQueue;

/* ICIC changes start */
extern LTE_LIST  dlCellEdgeUserPriorityList_g;

#define DL_RETX_TTI_DIFF_THRESHOLD 0
/*  ICIC changes end */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

UInt32 processDLSchedulerStrategy 
        (
         /* SPR 15909 fix start */
         tickType_t currentGlobalTTITickCount,
         /* SPR 15909 fix end */
         UInt32 delayToPHYDLScheduler,
         InternalCellIndex internalCellIndex
         );
#ifdef TDD_CONFIG
UInt32 processDummyDlSchedulerStrategy (
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 dlDelay
        ,InternalCellIndex internalCellIndex
        );
#endif

/* ICIC Change Start */

void processDLAdvanceQueue(
        UInt32 * numberOfAvailableRBs_p,
        DLSchedulerDataElement * dlScheduleData_p,
       /* SPR 15909 fix start */
        tickType_t globalTick,
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
        UInt32 dlDelay,
#endif
        UInt8 *maxUEsToBeScheduledDL_p,
        InternalCellIndex internalCellIndex);
/* ICIC Change end */


void
processTAQueue (UInt32 timeAdvanceQueueCount,
		UInt32 * numberOfAvailableRBs_p,
	        /* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
        	/* SPR 15909 fix end */
		DLSchedulerDataElement * dlScheduleData_p,
		TA_Q_TYPE * tempTimeAdvanceQueue,
		FlagForPendingTADRX flagForTADRXPendingFresh,
		UInt16 * numTAUEScheduled_p, UInt8 * maxUEsToBeScheduledDL_p
		/*HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
		,UInt8 dlDelay
#endif
        , InternalCellIndex internalCellIndex
		/*HD FDD Changes End */
  );				/* SPR_1346 *//*RETX_CHG */


void processDRXQueue
        (
        UInt32 dRXQueueCount,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount_g,
        /* SPR 15909 fix end */
        DLSchedulerDataElement * dlScheduleData_p,
        DRX_Q_TYPE * dRXQueue,
        FlagForPendingTADRX flagForTADRXPendingFresh,
        /* ICIC changes start */
        UInt8 *maxUEsToBeScheduledDL_p,
        /* ICIC changes end */ 
        InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129059) */
UInt32 scheduleUEsByDLStrategy
/* SPR 5599 changes end (ZIP ID 129059) */
        (
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement *dlScheduleData_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt8  maxUEsToBeScheduledDL, /* DRX_CHG */
        InternalCellIndex internalCellIndex);

void dciFormatSelector 
        (
        DLUEContext * ueDLContext_p ,
        DLStrategyTxNode * dlStrategyTxNode_p,
        TxModeTBAndHarqStatus txModeTBAndHarqStatus,
        ServingCellIndex servingCellIndex 
        );

MacRetType processDLNack
        (
        DLUEContext * ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode *dlStrategyTxNode_p,
        TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo,
        UInt32 * numberOfAvailableRBs_p
        );
        
MacRetType processDLNackRoundRobin
        (
        DLUEContext * ueDLContext_p,
        DLHARQProcess *harqProcess_p, 
        DLStrategyTxNode *dlStrategyTxNode_p,
        TBOneTBTwoReTxNewInfo tBOneTBTwoReTxNewInfo
        );

DLHARQProcess *checkForFreeHarqProcess (
        DLUEContext * dlUeCtx_p,
        DLHARQEntity * harqContext_p, 
        SchType schType ,
	    ServingCellIndex servingCellIndex);


MacRetType  processTADRXNonSPALFreeHarq
        (
        DLUEContext * ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        UInt32 * numberOfAvailableRBs_p,
        FlagForPendingTADRX flagForTADRXPendingFresh
#ifdef TDD_CONFIG
        ,UInt8 subframe
#endif
        ,InternalCellIndex internalCellIndex);

extern MacRetType putEntryInDLShedulerNewTxQueue
        (
        DLStrategyTxNode * 
        );

extern MacRetType putEntryInDLShedulerReTxQueue
        (
        DLStrategyTxNode * 
        );

void processZeroNonZeroTriggerQueue (UInt32 ZeroNonZeroTriggerQueueCount,
        /*  +- SPR 17777 */
        InternalCellIndex internalCellIndex
        );

/* SPR 5599 changes start (ZIP ID 129059) */
void putEntryInUENonZeroQueueLoadAndHarqList
/* SPR 5599 changes end (ZIP ID 129059) */
        (
        UInt32 ueIndex,
        DLUEContext* ueDLContext_p,
        InternalCellIndex internalCellIndex
        );

/* SPR 5599 changes start (ZIP ID 129059) */
/* Review comment fix start MAC_RESET_8 */        
void deleteEntryFromUENonZeroQueueLoadAndHarqList
/* Review comment fix end MAC_RESET_8 */        
/* SPR 5599 changes end (ZIP ID 129059) */
        (
        /* SPR 17777 */
        DLUEContext* ueDLContext_p,
        InternalCellIndex internalCellIndex
        );

extern UInt32 dlProportionalFairScheduler
        (
        /* SPR 21068 start */ 
        tickType_t referenceFlag,
        /* SPR 21068 end  */
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement *dlScheduleData_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /*SPR 15909 fix end */
        /* SPR 17777 */
        UInt8  maxUEsToBeScheduledDL, /* RETX_CHG */
        InternalCellIndex internalCellIndex
        );

extern UInt32 dlRoundRobinScheduler(
        /* SPR 21068 start */
        tickType_t referenceFlag,
        /* SPR 21068 end  */
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement *dlScheduleData_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        UInt8  maxUEsToBeScheduledDL, /* RETX_CHG */
        InternalCellIndex internalCellIndex
        );

void initDLSchedulerStrategy (
        UInt8 numOfCells 
        );
/* SPR 8702 changes start */
void deInitDLSchedulerStrategyQueues(InternalCellIndex internalCellIndex);
/* SPR 8702 changes end */
UInt32 validateRBForOneTB
        (
         DLUEContext* ueDLContext_p,
         UInt32 allocatedRB,
         UInt32 numOfNonZeroLCs,
         UInt32* tbSizeRet,
         UInt32 availableRB,
         MsgType msgType,
         /* +- SPR 17777 */
         UInt32 *minTBS,
         UInt32 *maxTBS
         /* +- SPR 17777 */
#ifdef TDD_CONFIG
         ,UInt8 subFrame
         ,InternalCellIndex internalCellIndex
#endif         
         /* +- SPR 17777 */
        );

UInt32 validateRBForTwoTB
        (
         DLUEContext* ueDLContext_p,
         UInt32 allocatedRB,
         UInt32 numOfNonZeroLCs,
         UInt32* tbOneSizeRet,
         UInt32* tbTwoSizeRet,
         UInt32  availableRB_p,
         MsgType msgType,
         UInt32 *minTBS,
         UInt32 *maxTBS,
         /* +- SPR 17777 */
#ifdef TDD_CONFIG
         UInt8 subFrame,
         InternalCellIndex internalCellIndex,
#endif         
         /*4x4 DL MIMO CHG START*/         
         DLHARQProcess *harqProcess_p         
         /*4x4 DL MIMO CHG END*/
          /* +- SPR 17777 */
        );
/*4x4 DL MIMO CHG START*/
UInt32 determineTBSizeForRank3And4
         (
          DLUEContext* ueDLContext_p,
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
          /*4x4 DL MIMO CHG START*/
          DLHARQProcess *harqProcess_p          
          /*4x4 DL MIMO CHG END*/
          /* +- SPR 17777 */
         );
/*4x4 DL MIMO CHG END*/
void putEntryInZeroNonZeroQueueLoadTriggerQueue
        (
        DLUEContext * ueContext_p,
        AddDeleteTrigger addDeleteTrigger,
        InternalCellIndex internalCellIndex
        );
        
UInt32 getDLModulationScheme 
        (
         DLUEContext * ueDLContext_p
         );
         
extern UInt32 calculateNumLCWithNonZeroQueueLoad
        (
        DLUEContext* ueDLContext_p
        );                 

extern void initDLSchedulerStrategy( UInt8 numOfCells );

extern void cleanupDLSchedulerStrategy(InternalCellIndex internalCellIndex);

/* Cyclomatic_complexity_changes_start */
void deleteInvalidNodeFromDLRetransmissionQueue(UInt32 count,
        InternalCellIndex internalCellIndex);
void cleanupDlCAEventQueue(InternalCellIndex internalCellIndex);
void cleanupDlCAMacCEQueue(InternalCellIndex internalCellIndex);
void cleanupPendingDlCAMacCEQueue(InternalCellIndex internalCellIndex);
void cleanupPendingFailureDlCAMacCEQueue(InternalCellIndex internalCellIndex);
void cleanupDlCAMacFreeScellHarqBufferQueue(InternalCellIndex internalCellIndex);
void cleanupMeasGapOnOffTriggerQueue(InternalCellIndex internalCellIndex);
void cleanupPendingDRXQueue(InternalCellIndex internalCellIndex);
void cleanupPendingTAQueue(InternalCellIndex internalCellIndex);
void cleanupDlUeZeroNonZeroQueueLoadTriggrQueue(InternalCellIndex internalCellIndex);
void cleanupDlUENonZeroQueueLoadAndHarqList(InternalCellIndex internalCellIndex);
void cleanupDlCellEdgeUserPriorityQueue(InternalCellIndex internalCellIndex);
void cleanupDRXOnOffTriggerQueue(InternalCellIndex internalCellIndex);
void cleanupAdvanceAllocInfoQueue(InternalCellIndex internalCellIndex);
/* Cyclomatic_complexity_changes_end */

/* UL_SYNC_LOSS_CHG, DRX_CHG and MEAS_GAP_CHG */
void deleteEntryFromUENonZeroQueueLoadAndHarqListTemporarily(
        DLUEContext* ueDLContext_p,
        InternalCellIndex internalCellIndex);

/*QOS_CHG*/

/*SPR 2446 Fix Begins*/
void processDLRetransmissionQueue(
        /* SPR 15909 fix start */
        void *numberOfAvailableRBs_p,tickType_t  globalTick,
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
        UInt32 dlDelay,
#endif
        void* maxUEsToBeScheduledDL_p,
        InternalCellIndex internalCellIndex);
/*SPR 2446 Fix Ends*/

void processDLRetransmissionFailureQueue(
        /* SPR 15909 fix start */
         UInt32 *numberOfAvailableRBs_p,tickType_t  globalTick,
         /* SPR 15909 fix end */
#ifdef FDD_CONFIG
         UInt32 dlDelay,
#endif
         UInt8 *maxUEsToBeScheduledDL_p,
         InternalCellIndex internalCellIndex);
/*QOS_CHG*/

#ifdef OVERSHOOT_CORRECTION
void modifiedNoOfUEsAfterOverLoadDL(UInt8  *modifiedUEs_p,
                                    InternalCellIndex internalCellIndex);

/* +- SPR 17777 */
/*+ Dyn UEs To Be SchPerTTI +*/
/* + SPR 17439 */
void getIndexForReqElProcessingCycles(void);
/* - SPR 17439 */
/*+ Dyn UEs To Be SchPerTTI +*/
#endif
/* HD FDD Changes Start*/
void processExplicitDLDCIScheduleQueue (UInt32 explicitDLDCIScheduleQueueCount,
                                        UInt32 * numberOfAvailableRBs_p,
                                        /* SPR 15909 fix start */
                                        tickType_t currentGlobalTTITickCount,
                                       /* SPR 15909 fix end */
                                        DLSchedulerDataElement * dlScheduleData_p,
                                        EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE * explicitDLDCIScheduleQueue_p,
                                        UInt16 * numUEScheduled_p,
                                        UInt8 * maxUEsToBeScheduledDL_p
                                        /*HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
                                        ,UInt8 dlDelay
#endif
                                        /*HD FDD Changes End */
                                        ,InternalCellIndex internalCellIndex
                                       );

/* HD FDD Changes End*/
/* SRP 3608 changes start */
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
                UInt8 *numUEScheduled_p,
                InternalCellIndex internalCellIndex);
/*Complexity Start*/
void eicicPopDLPriorityQNodeScan(
	UInt32 dlPriorityQueueCount,
	InternalCellIndex internalCellIndex,
	UInt8 isTraverseList,
	LTE_CIRCQUEUE *dlPriorityQueue_p
	);
/*Complexity End*/
/* SPR 3608 changes end */

/* SPR 16916 FIX START */
#ifdef KPI_STATS
void resetZeroQloadFlag(UInt16 ueIndex, UInt8 qci);
void macUpdateActiveUePerQciInfo(
        UInt8 lcTrigIndexForKpi,
        DLUEContext* ueContext_p,
        UInt8 *addTrigLCNodeForKpi,
        InternalCellIndex internalCellIndex);
void macUpdateActiveUlUePerQciInfo(
        UInt8 lcgTrigIndexForKpi,
        ULUEContext* ueULContext_p,
        UInt8 *addTrigLCGNodeForKpi,
        InternalCellIndex internalCellIndex);
 void updateKpiStatsForNumOfActiveUeDlPerQci(UInt16 ueIndex,
        DLHARQTBInfo *TBInfo,TbValue TBFlag,UInt8 harqProcessId,
        InternalCellIndex internalCellIndex);
void  updateNumOfActiveUePerQciUl(UInt16 ueIndex,
                InternalCellIndex internalCellIndex);
void updateNumOfActiveUeDlOnUeDelete(UInt16 ueIndex,
                InternalCellIndex internalCellIndex);
UInt8 macIsQciUniqueForDlLc(
        UInt8 lcId,
        UInt8 qci,
        UInt16 ueIndex);
UInt8 macIsQciUniqueForUlLc(
        UInt8 lcId,
        UInt8 qci,
        UInt16 ueIndex);
void updateNumOfActiveUeUlOnUeDelete(UInt16 ueIndex,
                InternalCellIndex internalCellIndex);
#endif
/* SPR 16916 FIX END */

/* SPR 21660 changes start_vishal */
/* SPR 21660 changes end_vishal */

#endif  /* LTE_MAC_DL_SCHEDULER_STRATEGY_H */

