/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLQosScheduler.c
 *
 ******************************************************************************
 *
 *  File Description : This file includes the functionality of distributing the
 Downlink RBs in round robin fashion.The number of RBs 
 distributed to particular UE will be depending upon the 
 configurale parameters 
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacDLSpsScheduler.h" 
#include "lteMacDLQosScheduler.h"
#include "lteMacCellConfMgr.h"
#include "lteMacDLQosTokenAlgo.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
/* + SPS_TDD_Changes*/
#include "lteMacTddMgr.h"
/* - SPS_TDD_Changes*/
#endif
#include "lteMacDLHarqMgr.h"
#include "lteMacDLResourceInfoMgr.h"
#include "ltePerfStats.h"
#include "lteMacTypes.h"
/******************************************************************************
  Private Definitions
 *****************************************************************************/
#define NUM_OF_TA_SCHEDULED_PER_TTI 5

/*4x4 DL MIMO CHG END*/
TopSchedulableUEs topSchedulableUEs_g[MAX_NUM_SCELL +1];

/*  EICIC + */
TopSchedulableUEs topSchedulableVictimUEs_g[MAX_NUM_SCELL +1];
/* EICIC -*/

/* ICIC changes starts */
BufferPool dlBufferPool_g[MAX_NUM_CELL] ;
static UInt16 unusedDLStrategyBufferRB_g[MAX_NUM_CELL];
/* ICIC changes ends */

/*QOS_RB_RESTICTION_CHANGE*/
/* To be discussed : instaed of UInt32, it should be UInt8 as max RBs would be 100 only*/
STATIC UInt32 maxRestrictedRbDL_g[MAX_NUM_CELL];
/*QOS_RB_RESTICTION_CHANGE*/



static DLScheduleFlag dlQosStrategyScheduleSM_g[DL_QOS_MAX_STATE][DL_QOS_MAX_EVENT] =
{
	/* DL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_DRX_ON  */
		DL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_DRX_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_DRX_ON_MEAS_GAP_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON,
	},
	/* DL_QOS_DRX_ON_MEAS_GAP_ON */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_MEAS_GAP_ON,
		/* DL_QOS_EVENT_DRX_ON */
		DL_QOS_DRX_ON_MEAS_GAP_ON,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_DRX_ON,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_DRX_ON_MEAS_GAP_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_DRX_ON_MEAS_GAP_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON,
	},
	/* DL_QOS_DRX_ON_SYNC_LOSS_ON */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_DRX_ON */
		DL_QOS_DRX_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_DRX_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_DRX_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_DRX_ON_SYNC_LOSS_ON,
	},
	/* DL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_DRX_ON */
		DL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_MEAS_GAP_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON,
	},
	/* DL_QOS_DRX_ON */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_DRX_ON */
		DL_QOS_DRX_ON,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_DRX_ON,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_DRX_ON_MEAS_GAP_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_DRX_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_DRX_ON_SYNC_LOSS_ON,
	},
	/* DL_QOS_MEAS_GAP_ON */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_MEAS_GAP_ON,
		/* DL_QOS_EVENT_DRX_ON */
		DL_QOS_DRX_ON_MEAS_GAP_ON,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_MEAS_GAP_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_MEAS_GAP_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON,
	},
	/* DL_QOS_SYNC_LOSS_ON */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_DRX_ON */
		DL_QOS_DRX_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_SYNC_LOSS_ON,
	},
	/* DL_QOS_NO_TRANSMISSION */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_DRX_ON */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_NEW_TRANSMISSION,
	},
	/* DL_QOS_NO_NEW_TRANSMISSION */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_NEW_TRANSMISSION, 
		/* DL_QOS_EVENT_DRX_ON */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_NEW_TRANSMISSION,
	},
	/* DL_QOS_ALREADY_TRANSMITED */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_DRX_ON */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_NEW_TRANSMISSION,
	},
	/* DL_QOS_NEW_TRANSMISSION */
	{
		/* DL_QOS_EVENT_DRX_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_DRX_ON */
		DL_QOS_DRX_ON,
		/* DL_QOS_EVENT_MEAS_GAP_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_MEAS_GAP_ON */
		DL_QOS_MEAS_GAP_ON,
		/* DL_QOS_EVENT_SYNC_LOSS_OFF */
		DL_QOS_NEW_TRANSMISSION,
		/* DL_QOS_EVENT_SYNC_LOSS_ON */
		DL_QOS_SYNC_LOSS_ON,
	}
};


LTE_TREE* dlPriorityLcTree_gp[MAX_NUM_CELL][MAX_VALID_UE_TYPE]; /* EICIC +-*/

/*Queue of type LTE_CIRCQUEUE which contains indexes of UEs having pending TA 
  Request*/

TA_Q_TYPE* pdcchPdschFailureTAQueue_gp[MAX_NUM_CELL];
/* + SPR 22243 Fix*/
/*Queue of type LTE_CIRCQUEUE which contains indexes of UEs having pending DRX 
  Request for which PDCCH allocation fails */
DRX_Q_TYPE*  pdcchPdschFailureDRXQueue_gp[MAX_NUM_CELL];
/* - SPR 22243 Fix*/

/* Queue of type LTE_CIRCQUEUE* which contains the node for MAC 
 * Scell Activation CE */
extern DL_CA_MAC_CE_QUEUE_TYPE* dlCAMacCEQueue_gp[]; 
extern DL_CA_MAC_CE_QUEUE_TYPE* pendingFailureDlCAMacCEQueue_gp[MAX_NUM_CELL];

/* This Queue holds the node for each retransmissions failure */
extern LTE_SQUEUE *dlRetransmissionFailureQueue_gp[MAX_NUM_CELL];

/* This Queue holds the node for each retransmissions */
extern LTE_SQUEUE *dlRetransmissionQueue_gp[MAX_NUM_CELL];

/* This is the global structure for the maintain strategy scheduling details 
 * and to process at the end  
 */
DLStrategyScheduleInfo dlStrategyScheduleInfo_g[STRATEGY_MAX_CONTAINER_INDEX][MAX_NUM_CELL];

/*Queue of type LTE_SQUEUE which contains Triggers of UEs trigger to enter/to 
  remove from / into the Eligible List and Priority LC TREE */
/* CirQ changes start*/
extern DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_TYPE *dlUEZeroNonZeroQueueLoadTriggerQueue_gp[MAX_NUM_CELL];
/* CirQ changes end*/

/*Queue of type LTE_CIRCQUEUE which contains Triggers of UEs trigger to enter/to 
  remove Eligible LC's from / into the Priority LC TREE */
extern DL_DRX_ON_OFF_TRIGGER_TYPE* dlDRXOnOffTriggerQueue_gp[MAX_NUM_CELL];

extern DL_MEAS_GAP_ON_OFF_TRIGGER_TYPE* dlMeasGapOnOffTriggerQueue_gp[MAX_NUM_CELL];

/*Queue of type LTE_SQUEUE which contains Triggers of UEs trigger to enter/to 
  remove Eligible LC's from / into the Priority LC TREE */

/*Queue of type LTE_SQUEUE which contains Triggers of UEs trigger to enter/to 
  remove Eligible LC's from / into the Priority LC TREE */

/* Indicates the minimum GBR Bytes to be scheduled per TTI */
/* EICIC +*/
/* SPR 21149 Start */
UInt32 dlAggregateGbrPerTTI_g[MAX_NUM_SCELL + MAX_NUM_PCELL][MAX_VALID_UE_TYPE] = {{0}}; 
/* SPR 21149 End */
extern UInt32 totalDlAggregateGbrPerTTI_g[MAX_VALID_UE_TYPE];
/* EICIC -*/
/* Explicit DL DCI Schedule queue */
extern EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE* explicitDLPucchTpcScheduleQueue_gp[MAX_NUM_CELL];

/* Queue of LTE_CIRCQUEUE type is used to schedule DCI Format 1x/2x for those
 *  * UEs whose scheduling is failed in first attempt
 *   */
extern EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE* explicitDLPucchTpcScheduleFailureQueue_gp[MAX_NUM_CELL];

/* SPR 3679 QOS Changes Start */
extern UInt32 totalRbMIMO_g[MAX_NUM_CELL];
/* SPR 3679 QOS Changes End */
/*spr 13395 fix start*/
extern LTE_SEM cellSyncSem_g;
/*spr 13395 fix end*/
#ifdef SSI_DEBUG_STATS
/* CA Stats Changes Start */
extern void updateDlQosPerTTIStats(InternalCellIndex internalCellIndex);
/* CA Stats Changes End */
#endif
/*+ Dyn UEs To Be SchPerTTI +*/
/* + PERF_CA GBL */
extern UInt16 totalActiveUEsDL_g[MAX_NUM_CELL] ;
/* - PERF_CA GBL */
/*- Dyn UEs To Be SchPerTTI -*/

STATIC UInt8  numDlUEsRemainingToSchd_g[MAX_NUM_CELL];
STATIC SInt32 numRBsAvailableForScheduling_g[MAX_NUM_CELL][MAX_USER_LOCATION];

#ifdef DL_UL_SPLIT
#ifdef CIRC_QUEUE_IMPL
extern SCELL_SCHEDULING_CIRC_Q dlSecondayCellScheulingQueue_g[MAX_NUM_CELL];
#else
extern LTE_SQUEUE dlSecondayCellScheulingQueue_g[MAX_NUM_CELL];
#endif
#else
extern LTE_SQUEUE dlSecondayCellScheulingQueue_g[MAX_NUM_CELL];
#endif

/******************************************************************************
  Private Types
 *****************************************************************************/

#define DL_QOS_UPDATE_SCHEDULE_FLAG(flag_p, Event) \
	*flag_p = dlQosStrategyScheduleSM_g[*flag_p][Event];

#define IS_DL_QOS_ALREADY_TRANSMITED_ON_SCELL(ueDLContext_p) {\
	( ( !IS_CA_ENABLED()) || \
	  ( (IS_CA_ENABLED()) && \
	    /* + SPR 12229 */\
        /*SPR 21948 Changes Start */\
        /* Changes under spr 21762 have been reverted */\
	    (!isUeEligibleForCaScheduling(ueDLContext_p)) )|| \
        /*SPR 21948 Changes End */\
	  /* - SPR 12229 */\
	  (DL_QOS_ALREADY_TRANSMITED > \
	   ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag)) ? \
	LTE_TRUE : LTE_FALSE; \
}


/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void dlSpsUpdateStrategyNodes( InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount);
/* SPR 15909 fix end */

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/


/*****************************************************************************
 * Functions
 *****************************************************************************/
/* Cyclomatic Complexity changes - starts here */
static  void dlQosUpdateHarqContext( DLStrategyTxNode *dlStrategyTxNode_p,
		DLUEContext  *ueDLContext_p,       
		DLHARQProcess *harqProcess_p,
		InternalCellIndex internalCellIndex);

STATIC  MacRetType dlQosValidatePriorityTreeNode(DLUEContext  *ueDLContext_p,
		DLQosStrategyLCNode * dlQosStrategyLCNode_p,
        /* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		InternalCellIndex internalCellIndex,
		UInt8 gbrSchedulingFlag
		);
static  SInt8 dlQosDetermineTbSizeForNewTx(DLUEContext * ueDLContext_p,
		DLHARQProcess * harqProcess_p,
		SInt32 numAvailableRBs,
		UInt32 schedulableBytes,
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
		UInt8 subFrameNum,
#endif
        /* +- SPR 17777 */
		InternalCellIndex internalCellIndex);

#ifdef PERF_STATS
/* + SPR 11214 Fix */
static  void updatePerformanceStats(DLUEContext * ueDLContext_p, InternalCellIndex internalCellIndex);
/* - SPR 11214 Fix */
#endif
/*ca chngs */
STATIC  InternalCellIndex getCellIndexOfPriorityLcNode( DLQosStrategyLCNode *dlQosStrategyLCNode_p);
/*ca chngs */
/* Cyclomatic Complexity changes - ends here */

STATIC  void dlQosInitStrategyLocalParams( 
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
		UInt8  *gbrSchedulingFlag_p,
		UInt8  *numDlUEsToBeScheduled_p,
		InternalCellIndex internalCellIndex);

STATIC  UInt8 dlQosGetLoadDiffBetweenCells(
		DLUEContext* ueDLContext_p); 

STATIC  UInt8 dlQosGetMcsDiffBetweenCells(
		DLUEContext* ueDLContext_p); 

 void dlQosAddCellInPreferredCellList( 
		InternalCellIndex pCellIndex,
		ServingCellIndex servingCellIndex);

STATIC  void dlQosGetLoadWisePreferredCellList(
		DLUEContext* ueDLContext_p); 

STATIC  void dlQosGetMcsWisePreferredCellList(
		DLUEContext* ueDLContext_p); 

STATIC  UInt8 dlQosPreparePreferredCellsList( 
		DLUEContext* ueDLContext_p,
		InternalCellIndex priorityLcNodeCellIndex);

STATIC  MacRetType dlQosGetPreferedCellListForScheduling( 
		UInt16 ueIndex,
		UInt8 isMacCEScheduled,
                                UInt8 *preferredListIndex_p,
                                /* SPR 16116 fix start */
                                UInt32 totLCWeightWithLP);
                                /* SPR 16116 fix end */

STATIC  void dlQosUpdateGbrSchdlAndReStartTraversing( 
		DLQosStrategyLCNode** dlQosStrategyLCNode_p,
		UInt8 *gbrSchedulingFlag_p,
		UInt8 *reStartTraversing_p,
		InternalCellIndex internalCellIndex,
		UserType	userType);

STATIC  DLStrategyTxNode* dlQoSPrepareUEStrategyNode( 
		DLUEContext *ueDLContext_p,
        /* +- SPR 17777 */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		ServingCellIndex servingCellIndex,
		DLQosStrategyLCNode *dlQosStrategyLCNode_p,
		dlQosTraversalType traversalType);

/* + SPR 13605 Fix */
STATIC  void dlQoSApplyPreferenceFactor( 
		DLUEContext *ueDLContext_p,
		DLQosStrategyLCNode *dlQosStrategyLCNode_p,
		UInt8 preferredCellListIndex,
        UInt32 tempDlAggregateGbrPerTTI[MAX_NUM_CELL][MAX_VALID_UE_TYPE]);

STATIC  void dlQoSScheduleLcOnPreferredCells( 
		DLUEContext *ueDLContext_p,
		DLQosStrategyLCNode *dlQosStrategyLCNode_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		UInt8  preferredCellListIndex,
		UInt8  *gbrSchedulingFlag_p,
		UInt8  *reStartTraversing_p,
        UInt32 tempDlAggregateGbrPerTTI[MAX_NUM_CELL][MAX_VALID_UE_TYPE]);
/* - SPR 13605 Fix */

STATIC  UInt8 isRbAvailableOnPreferredCells(
		InternalCellIndex internalCellIndex,
		UInt8 preferredCellListIndex);

STATIC  UInt8 isTotalRbMIMOAvailableOnPreferredCells(
		InternalCellIndex internalCellIndex,
		UInt8 preferredCellListIndex);

STATIC  void dlQosUpdateDlBufferPoolAndResetTotalRbMIMO(
		InternalCellIndex internalCellIndex);

STATIC  void dlQosReTraversePriorityLcListForAllCells( 
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		InternalCellIndex internalCellIndex);

UInt8 dlQosGetMcsInServingCell( 
		DLUEContext *ueDLContext_p,
		ServingCellIndex servingCellIndex);

STATIC  void dlQosPostStrategyExecution(
		InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount);
        /* SPR 15909 fix end */

 DLQosStrategyLCNode* dlQosGetPriorityLcTreeNode( 
		InternalCellIndex internalCellIndex,
		UInt8 reStartTraversing);

DLQosStrategyLCNode* dlQosGetPriorityLcTreeVictimNode( 
		InternalCellIndex internalCellIndex,
		UInt8 reStartTraversing);

DLQosStrategyLCNode* dlQosGetFirstNode(
		DLQosStrategyLCNode *dlQosStrategyLCNodeForCellIndex_p[],
		InternalCellIndex internalCellIndex,
		UInt8 *workingTreeIndex);

DLQosStrategyLCNode* dlQosGetFirstVictimNode(
		DLQosStrategyLCNode *dlQosStrategyLCNodeForCellIndex_p[],
		InternalCellIndex internalCellIndex,
		UInt8 *workingTreeIndex);

DLQosStrategyLCNode* dlQosGetNextNode(
		DLQosStrategyLCNode *dlQosStrategyLCNodeForCellIndex_p[],
		UInt8 *workingTreeIndex);

STATIC  InternalCellIndex getCellIdWithHighestMcs(
		DLUEContext* ueDLContext_p); 

STATIC  MacRetType isMacCEAlreadyScheduledForUE( 
		InternalCellIndex internalCellIndex,
		UInt16 ueIndex,
		UInt8 listIndex); 

STATIC  void dlQosUpdatePreferredCellList( 
		UInt16 ueIndex, 
		UInt8 listIndex,
                                InternalCellIndex internalCellIndex,
                                /* SPR 16116 fix start */
                                UInt32 totLCWeightWithLP);
                                /* SPR 16116 fix end */

STATIC  void removeCellFromPreferredCellList( 
		InternalCellIndex internalCellIndex,
		UInt8 listIndex, 
		ServingCellIndex servingCellIndex);

STATIC  void dlQosStrategyProcessDlCAMacCEQueueofAllCells( 
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		DL_CA_MAC_CE_QUEUE_TYPE** tempDlCAMacCEQueue_p,
		UInt8   *numDlUEsToBeScheduled_p,
        /* +- SPR 17777 */
		FlagForPendingScellCE flagForPendingScellCE);
        /* +- SPR 17777 */

STATIC  void dlQosStrategyProcessDlCAMacCEQueue( 
		UInt32 dlCAMacCEQCount,
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		DL_CA_MAC_CE_QUEUE_TYPE* tempDlCAMacCEQueue_p,
		UInt8   *numDlUEsToBeScheduled_p,
		FlagForPendingScellCE flagForPendingScellCE,
		InternalCellIndex internalCellIndex);

STATIC  void dlQosResetPreferredCellList(
		InternalCellIndex internalCellIndex);

STATIC  void dlQosStrategyProcessDLReTxFailQueues( 
		InternalCellIndex internalCellIndex,
		UInt8   *numDlUEsToBeScheduled_p,
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
		/* SPR 15909 fix start */
        /* +- SPR 17777 */
        tickType_t currentGlobalTTITickCount
#ifdef FDD_CONFIG
        /* SPR 15909 fix end */
		,UInt32 dlDelay
#endif
        );
/* +- SPR 17777 */

STATIC  void dlQosStrategyProcessDLReTxQueues( 
		InternalCellIndex internalCellIndex,
		UInt8   *numDlUEsToBeScheduled_p,
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
		/* SPR 15909 fix start */
        /* +- SPR 17777 */
        tickType_t currentGlobalTTITickCount
#ifdef FDD_CONFIG
        /* SPR 15909 fix end */
		,UInt32 dlDelay
#endif
        );
/* +- SPR 17777 */

STATIC  void dlQosInitStrategyParams( 
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
		UInt8  *numDlUEsToBeScheduled_p,
		InternalCellIndex internalCellIndex);

STATIC  void dlQosPreStrategyExecution( 
		InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
		/* SPR 15909 fix end */
		/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
		UInt32 dlDelay, 
#endif
		/* - SPS_TDD_Changes */
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
		UInt8   *numDlUEsToBeScheduled_p);

STATIC  void dlQosProcessAllTriggerQueues( 
		InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount
		/* SPR 15909 fix end */
		/* + SPS_TDD_CHANGES */
#ifdef FDD_CONFIG
		,UInt32 dlDelay
#endif
		/* - SPS_TDD_CHANGES */
		);

STATIC  void dlQosHandleTASchedulingFailureCase( 
		InternalCellIndex internalCellIndex,
		FlagForPendingTADRX flagForTADRXPendingFresh,
		TimeAdvanceQueueNode* taNode_p
		/*SPR 9664 Fix Start*/
		,DLUEContext *ueDLContext_p
		/*SPR 9664 Fix End*/
		);

STATIC  void dlQosHandleDRXSchedulingFailureCase( 
		InternalCellIndex internalCellIndex,
		DRXQueueNode* drxNode_p);

STATIC  void dlQosHandleExplicitDciSchedulingFailureCase( 
		InternalCellIndex internalCellIndex,
		ExplicitDLDCIQueueNode* node_p);

STATIC  void dlQosHandleSCellCESchedulingFailureCase( 
		InternalCellIndex internalCellIndex,
		FlagForPendingScellCE flagForPendingScellCE,
		DLCAMacCEQueueNode* node_p);

 void putEntryInSCellSchedulingQueue( 
		DLUEContext* ueDLContext_p, 
		DLHARQProcess* harqProcess_p,
		ServingCellIndex servingCellIndex);

 UInt8 dlQosGetPCellMcs(  DLUEContext* ueDLContext_p);

 UInt8 dlQosGetSCellMcs(  DLUEContext* ueDLContext_p,
		InternalCellIndex sCellIndex);
/* Cyclomatic_complexity_changes_start */
STATIC void getMsgTypeTaDrxFlagNonSpatial(UInt8              scheduledLcCount,
		MsgType            isTaDrxScheduled,
		DLStrategyTxNode   *dlStrategyTxNode_p,
        /* +- SPR 17777 */
		DLHARQProcess      *harqProcess_p
#ifdef SSI_DEBUG_STATS
        ,InternalCellIndex internalCellIndex
#endif
        );
        /* +- SPR 17777 */
STATIC void getMsgTypeTaDrxFlagSpatial(UInt8              scheduledLcCount,
		MsgType            isTaDrxScheduled,
		DLStrategyTxNode   *dlStrategyTxNode_p,
        /* +- SPR 17777 */
		DLHARQProcess      *harqProcess_p);
        /* +- SPR 17777 */
STATIC void  processPendingTAQueue( TimeAdvanceQueueNode *taNode_p, 
		FlagForPendingTADRX flagForTADRXPendingFresh,
		InternalCellIndex internalCellIndex, DLUEContext *ueDLContext_p);

/* Cyclomatic_complexity_changes_end */


/* EICIC +*/
extern UInt8 	absPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
extern UInt8 	usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
extern  UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL];
extern UInt8  	isEicicEnabled_g[MAX_NUM_CELL];
extern UInt8 		redistributeRb_g ;
extern UInt8 		ueTypeToScheduleDL_g[MAX_NUM_CELL] ;
extern UInt32 		phyDelay_g ;
extern UInt8 absPatternIndex_g[MAX_NUM_CELL];
void eicicUpdateSfAndSfn(UInt32 *sysFrameNum,UInt32 *subFrameNum); 

/* EICIC -*/
/* + SPR 22352 fix */
STATIC  void checkMsgTypeTaDrxScellActDeact(DLUEContext *ueDLContext_p,
        MsgType currentMsgType, MsgType *updatedMsgType,InternalCellIndex servingCellIndex);
STATIC  void updateRbAndTBSizeInDlStrategyNode(MsgType msgType,DLStrategyTxNode *dlStrategyTxNode_p,
        SInt32  *numberOfAvailableRBs_p,UInt8 userLocationType,UInt16 ueIndex);
STATIC  void setBitmaskForScellActDeact(UInt16 ueIndex,DLStrategyTxNode* dlStrategyTxNode_p);
/* - SPR 22352 fix */

/* Complexity starts*/
/*****************************************************************************
 * Function Name  : eicicFindNumOfBitsinABSPattern 
 * Inputs         : internalCellIndex
 * Returns        : No of bits in ABS pattern
 * Description    : This function finds no of bits in ABS pattern
 *****************************************************************************/
UInt8 eicicFindNumOfBitsinABSPattern(InternalCellIndex internalCellIndex)
{
	UInt8 numBitsAbsPattern = 0;

#ifdef FDD_CONFIG
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(internalCellIndex)
	numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
	if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
			[internalCellIndex]->cellParams_p->subFrameAssign)
	{
		numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
	}
	else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
			[internalCellIndex]->cellParams_p->subFrameAssign)
	{
		numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
	}
	else
	{
		numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
	}
#endif

	return numBitsAbsPattern;
}

/*****************************************************************************
 * Function Name  : UpdateUEtypeToSchedule 
 * Inputs         : internalCellIndex-Cell Index at Mac
 *                  absSubFrameNum
 *                  numDlUEsToBeScheduled -Num ues to be schd in DL
 * Returns        : None
 * Description    : Updates the UEType 
 *****************************************************************************/
void UpdateUEtypeToSchedule(InternalCellIndex internalCellIndex,UInt8 absSubFrameNum,UInt8* numDlUEsToBeScheduled)
{
	if (((LTE_TRUE == absPattern_g[internalCellIndex][absPatternIndex_g[internalCellIndex]][absSubFrameNum] )&& (isEicicEnabled_g[internalCellIndex]))  /* Work as aggressor*/
#ifdef LTE_EMBMS_SUPPORTED
        || (isEmbmsValidSf_g[internalCellIndex])
#endif
    )
	{
		ueTypeToScheduleDL_g[internalCellIndex] =  INVALID_UE_TYPE; 
		numDlUEsToBeScheduled[internalCellIndex] = ZERO;
	}
	else if ((LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) && (isEicicEnabled_g[internalCellIndex])) /* Work as victim*/
	{
		ueTypeToScheduleDL_g[internalCellIndex] = VICTIM_UE;
	}
	else
	{
		ueTypeToScheduleDL_g[internalCellIndex] = NON_VICTIM_UE; /* By default if No ABS is configured then schedule NON victim UEs */
	}
}
/*****************************************************************************
 * Function Name  : eicicUpdateUEtypeToSchedule 
 * Inputs         : internalCellIndex-Cell Index at Mac
 *                  absSubFrameNum
 *                  numDlUEsToBeScheduled -Num ues to be schd in DL
 * Returns        : None
 * Description    : Updates the UEType 
 *****************************************************************************/
void eicicUpdateUEtypeToSchedule(InternalCellIndex internalCellIndex,UInt8 absSubFrameNum,UInt8* numDlUEsToBeScheduled)
{
	UInt8 isCaEnable = IS_CA_ENABLED();
	InternalCellIndex cellIndex         = 0;
	if(!isCaEnable) 
		/** SPR 13599 Changes End **/
	{
		UpdateUEtypeToSchedule(internalCellIndex,absSubFrameNum,numDlUEsToBeScheduled);

	}
	else
	{
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
		{
			if (CELL_PHY_RUNNING == cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->cellState)
			{	
				UpdateUEtypeToSchedule(cellIndex,absSubFrameNum,numDlUEsToBeScheduled);

			}
		}

	}
}
/*Complexity End*/
/*****************************************************************************
 * Function Name  : dlQosCalculateMinMaxTBSize
 * Inputs         : schedulableBytes - schedulable bytes,
 *                  subFrameNum - Subframe number,
 *                  numAvailableRBs - Number of available RB's,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : dlStrategyTxNode_p Pointer to DLStrategyTxNode in whihc 
 *                  needs to update the TB size
 * Returns        : deltaRB
 * Description    : This function will calculate the minumum and maximum TB sixe 
 *****************************************************************************/

 SInt8 dlQosCalculateMinMaxTBSize(
		DLStrategyTxNode   *dlStrategyTxNode_p,
		UInt32              schedulableBytes,
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
		UInt8               subFrameNum,
#endif
        /* +- SPR 17777 */
		SInt16              numAvailableRBs,
		InternalCellIndex    internalCellIndex)
{
	DLHARQProcess *harqProcess_p = PNULL;
	UInt8 requiredRB = 0;
	SInt8 deltaRB = 0;
	SInt8 tempRB = 0;
	UInt16 maxTBSize = 0;
	/* SPR 10143 fix start */
    /* +- SPR 17777 */
#ifdef TDD_CONFIG
	dlQosTraversalType traversalType = NEW_TRAVERSAL;
#endif
    /* +- SPR 17777 */
	/* SPR 10143 fix end */

	harqProcess_p = dlStrategyTxNode_p->harqProcess_p;

	/* Increment the Max TB Size and  Strategy LC node 
	 * with the schedulable bytes for the LC
	 */

	dlStrategyTxNode_p->maxTBSize += schedulableBytes;

	/* SPR 4543 changes start */
	DLUEContextInfo *ueDLContextInfo_p = PNULL;
	DLUEContext *ueDLContext_p = PNULL;
	ueDLContextInfo_p   = &dlUECtxInfoArr_g[dlStrategyTxNode_p->ueIndex];
	ueDLContext_p       = ueDLContextInfo_p->dlUEContext_p;

	if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
	{
		maxTBSize = ueDLContext_p->maxTBSize;
	}
	else
	{
		maxTBSize = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->maxTBSize;
	}
	/* SPR 4543 changes end */
	/*4x4 DL MIMO CHG START*/
	if(((NET_UE_QL(ueDLContext_p)) < dlStrategyTxNode_p->maxTBSize) && harqProcess_p->numOfLayer >= RANK_INDICATOR_3)
	{
		harqProcess_p->numOfLayer = RANK_INDICATOR_2;
	}
	/*4x4 DL MIMO CHG END*/
	/* get the RB.s required for the Max Data Scheduled */
	requiredRB =
		dlQosGetResBlksForScheduledBytes(
				/* SPR 4543 changes start */
				maxTBSize, 
				/* SPR 4543 changes end */
				dlStrategyTxNode_p->maxTBSize, 
				harqProcess_p,
                /* +- SPR 17777 */
				/*4x4 DL MIMO CHG START*/
				ueDLContext_p->ueCategory,
				/*4x4 DL MIMO CHG END*/
				/* SPR 10143 fix start */
                /* +- SPR 17777 */
#ifdef TDD_CONFIG
				subFrameNum,
				traversalType,
#endif
                /* +- SPR 17777 */
				/* SPR 10143 fix end */
				internalCellIndex);

	deltaRB = requiredRB - harqProcess_p->assignedResourcesNB;

	/* If scheduling for 100 RB.s is not crossed */
	if( numAvailableRBs > unusedDLStrategyBufferRB_g[internalCellIndex])
	{
		tempRB = numAvailableRBs - 
			unusedDLStrategyBufferRB_g[internalCellIndex];

		if(tempRB >= deltaRB)
		{
			dlStrategyTxNode_p->minTBSize += schedulableBytes;

			/* update the RB.s required for the Data scheduled 
			 */
			harqProcess_p->assignedResourcesNB = requiredRB;
		}
		else
		{
			/* + CQI_5.0 */
			if(TRUE == harqProcess_p->isTBOneValid)
			{

				dlStrategyTxNode_p->minTBSize += 
					determineTBSize( 
							harqProcess_p->dlHARQTBOneInfo.mcsIndex,
							tempRB,TB_MAPPED_TO_ONE_LAYER);
			}
			/* 
			   else if( TRUE == harqProcess_p->isTBTwoValid)
			   {
			   dlStrategyTxNode_p->minTBSize += 
			   determineTBSize( 
			   harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
			   tempRB);
			   }*/
			/* - CQI_5.0 */
			/* update the RB.s required for the Data scheduled
			 */
			harqProcess_p->assignedResourcesNB += tempRB;

			/*    ICIC changes start  */
			if(deltaRB - tempRB > unusedDLStrategyBufferRB_g[internalCellIndex])
			{
				unusedDLStrategyBufferRB_g[internalCellIndex] = 0;
				deltaRB = numAvailableRBs;
			}
			else
			{
				unusedDLStrategyBufferRB_g[internalCellIndex] -= (deltaRB - tempRB);
			}

			/*    ICIC changes end   */

		}
	}
	/*QOS ICIC fix :-31st Aug 2012 As in case of buffer Over Allocation, we do not need to store assigned RBs in harq process*/
	else
	{
		unusedDLStrategyBufferRB_g[internalCellIndex] = 0;
		return numAvailableRBs;
	}
	return deltaRB;
}


/*QOS_RB_RESTICTION_CHANGE*/

/*****************************************************************************
 * Function Name  : initDlQosRbRestriction
 * Inputs         : internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialise the max restriction RB's in Dl 
 *****************************************************************************/
/*   ICIC changes start */
 void initDlQosRbRestriction(InternalCellIndex internalCellIndex)
{
	InitParams *initParams_p = cellSpecificParams_g.\
				   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;

    /* SPR 22275 Fix + */
    UInt8 tempMaxUEsToBeScheduledDL =  MIN_BETWEEN_TWO(totalActiveUEsDL_g[internalCellIndex],(initParams_p->dlSchdConfig.maxUEsToBeScheduledDL));
    tempMaxUEsToBeScheduledDL = ((tempMaxUEsToBeScheduledDL >0)?tempMaxUEsToBeScheduledDL:1) ;

	maxRestrictedRbDL_g[internalCellIndex] = 
		(((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB * (initParams_p->dlRBRestrictionFactor))
         /tempMaxUEsToBeScheduledDL);
    /* SPR 22275 Fix - */
	if ( maxRestrictedRbDL_g[internalCellIndex] > (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB )
	{
		maxRestrictedRbDL_g[internalCellIndex] = (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB;
		lteWarning("Check the RB_RESTIRCTION_FACTOR[%u] or\
				NumSchUEPerTTI[%u]",initParams_p->dlRBRestrictionFactor,
				initParams_p->dlSchdConfig.maxUEsToBeScheduledDL);
	}
}
/* ICIC changes ends */

/*QOS_RB_RESTICTION_CHANGE*/


/*****************************************************************************
 * Function Name  : dlQosProcessUeForRBRestriction
 * Inputs         : traversalType - NEW TRAVERSAL/RETRAVERSAL
 * Outputs        : harqProcess_p - Pointer to DLHARQProcess
 *                  ueDLContext_p - Pointer to DLUEContext,
 *                  internalCellIndex - cell index used at MAC.
 * Returns        : None
 * Description    : This functions set the restricted TbSize for a particluar
 *                  UE.
 *****************************************************************************/
 void dlQosProcessUeForRBRestriction(
		dlQosTraversalType traversalType,
		DLHARQProcess      *harqProcess_p,
		DLUEContext        *ueDLContext_p,
		InternalCellIndex internalCellIndex)
{
	UInt32 tbSize = 0;
	UInt16 *maxTBSize_p       = PNULL;
	UInt32 *maxRestrictedTB_p = PNULL;

	if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
	{
		maxTBSize_p = &(ueDLContext_p->maxTBSize);
		maxRestrictedTB_p = &(ueDLContext_p->maxRestrictedTB);
	}
	else
	{
		maxTBSize_p = 
			&(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->maxTBSize);
		maxRestrictedTB_p = 
			&(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->maxRestrictedTB);
	}


	/* + CQI_5.0 */
	if(TRUE == harqProcess_p->isTBOneValid)
	{
		/*4X4 DL MIMO CHG START*/
		/* SPR 11355 Fix Start */
		if(((ueDLContext_p->ueCategory == UE_CAT_5) || (ueDLContext_p->ueCategory == UE_CAT_8))
				&& harqProcess_p->numOfLayer >= RANK_INDICATOR_3)
			/* SPR 11355 Fix End */
		{
			*maxRestrictedTB_p =
				determineTBSize( harqProcess_p->dlHARQTBOneInfo.mcsIndex,
						maxRestrictedRbDL_g[internalCellIndex],TB_MAPPED_TO_TWO_LAYER);
		}/*4X4 DL MIMO CHG END*/
		else
		{
			*maxRestrictedTB_p =
				determineTBSize( harqProcess_p->dlHARQTBOneInfo.mcsIndex,
						maxRestrictedRbDL_g[internalCellIndex],TB_MAPPED_TO_ONE_LAYER); 

		}    

	}

	/* else if(TRUE == harqProcess_p->isTBTwoValid)
	   {

	   ueDLContext_p->maxRestrictedTB =
	   determineTBSize( harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
	   maxRestrictedRbDL_g);
	   }*/
	/* - CQI_5.0 */

	if ( harqProcess_p->isTBOneValid == TRUE &&
			harqProcess_p->isTBTwoValid == TRUE )
	{
		tbSize = ueDLContext_p->maxMIMOTBSize;
		/* SPR# 3214 Changes Starts*/
		*maxRestrictedTB_p = (2*(*maxRestrictedTB_p));
		/* SPR# 3214 Changes Ends*/
	}
	else
	{
		tbSize = ueDLContext_p->maxSISOTBSize;
	}

	if ( NEW_TRAVERSAL == traversalType )
	{
		if ( tbSize < *maxRestrictedTB_p )
		{
			*maxTBSize_p = tbSize;
		}
		else
		{
			*maxTBSize_p = *maxRestrictedTB_p;
		}
	}
	else if (RETRAVERSAL == traversalType )
	{
		*maxTBSize_p = tbSize;
	}

}

#ifdef FDD_CONFIG
#endif
/* SPR 3608 changes start */
/*****************************************************************************
 * Function Name  : dlQosProcessPriorityQueue
 * Inputs         : numAvailableRBs_p - Number of available RB's,
 *                  numDlUEsToBeScheduled_p - maximum DL UE's can be scheduled,
 *                  dlPriorityQueueCount - DL Priority Queue Count,
 *                  currentGlobalTTITickCount - Current Global TTI Tick,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to prosess PSR for a UE. 
 *****************************************************************************/
void dlQosProcessPriorityQueue(SInt32 *numAvailableRBs_p,
		UInt8 *numDlUEsToBeScheduled_p,
		UInt32 dlPriorityQueueCount,
		/* + TM7_8 Changes Start */
        /* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		InternalCellIndex internalCellIndex)
/* - TM7_8 Changes End */
{
	UInt16 ueIndex = 0;
	UInt32 avgMcs = 0;
	UInt32 dataSize = 0;
	UInt32 maxDLRBWithPriority = 0;
	void   *dlEligibleLcNodeVoid_p = NULL;
	DLQosStrategyLCNode *dlEligibleLcNode_p = NULL;
	DLUEContextInfo * ueDLContextInfo_p = PNULL;
	DLUEContext * ueDLContext_p = PNULL;
	DLPriorityQueueNode *dlPriorityQNode_p = PNULL;
	DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
	DLStrategyTxNode * dlStrategyTxNode_p = PNULL;
	/* SPR 5313 Fix start */
	UInt8 userLocationType = CC_USER;
	while((dlPriorityQueueCount) &&
			(*numDlUEsToBeScheduled_p > 0) &&
			/* SPR 5313 Fix start */
			(0 < numAvailableRBs_p[CC_CE_REGION]))
		/* SPR 5313 Fix end */
	{
		/* SPR 9654 changes start*/
		popNodeCircQ(dlPriorityQueue_gp[internalCellIndex],
				(void **)&dlPriorityQNode_p);
		if(dlPriorityQNode_p)
		{
			/* SPR 9654 changes end*/
			dataSize = 0;
			ueIndex = dlPriorityQNode_p->ueIndex;
			ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
			if(!(ueDLContextInfo_p->pendingDeleteFlag))
			{
				/* Get UE Context */
				ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
				if(PNULL == ueDLContext_p)
				{
					LOG_MAC_MSG(MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID,LOGFATAL,MAC_DL_Strategy,
							currentGlobalTTITickCount,
							__LINE__,ueIndex,
							DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
							"Ue Index is null and a node is present in Tree");
					freeMemPool(dlPriorityQNode_p);
					dlPriorityQNode_p = PNULL;
					dlPriorityQueueCount--;
					continue;
				}
				/* EICIC +*/
				/* Pushing UE back in trigger queue.*/
				/*Complexity Start*/
				if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType)) || ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{              
					if (pushNodeCircQ(dlPriorityQueue_gp[internalCellIndex],
            /* SPR 23483 Changes Start */
								dlPriorityQNode_p) == CIRC_FAILURE)
            /* SPR 23483 Changes End */
					{
						freeMemPool(dlPriorityQNode_p);
						dlPriorityQNode_p = PNULL;
					}
					dlPriorityQueueCount--;
					continue;
				}
				/* EICIC - */

				/*Complexity End*/


				/* SPR 5313 Fix start */
				userLocationType = ueDLContext_p->userLocationType;
				/* \code
				 * if schType == SPS_SCHEDULING, then do not perform priority 
				 * scheduling for this UE.
				 * */
				if(( SPS_SCHEDULING == ueDLContext_p->schType ) || 
						(( CE_USER == userLocationType ) && ( !maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] )))
					/* SPR 5313 Fix end */
				{
					/* SPR 6897 changes start */
					/* SPR 6897 changes correction start */
					if (pushNodeCircQ(dlPriorityQueue_gp[internalCellIndex],
            /* SPR 23483 Changes Start */
								dlPriorityQNode_p) == CIRC_FAILURE)
            /* SPR 23483 Changes End */
						/* SPR 6897 changes correction end */
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
				/*Queue Load is non Zero*/
				if (DL_QOS_NEW_TRANSMISSION ==  ueDLContext_p->scheduleFlag)
				{
					if(PNULL == ueDLContext_p->dlStrategyTxNode_p)
					{   

						/*SPR 5619 START*/
						dlStrategyTxNode_p = 
							/*QOS_CYCLIC_COMPLEXITY_CHG*/ 
							dlQosStrategyUpdateNewTxUEList(ueDLContext_p, INVALID_TA_DRX_MSG, NEW_TRAVERSAL,
									/* - TM7_8 Changes Start */
									currentGlobalTTITickCount,
									internalCellIndex);
						/* - TM7_8 Changes End */
						/*QOS_CYCLIC_COMPLEXITY_CHG*/
						/*SPR 5619 END*/
						if(PNULL != dlStrategyTxNode_p)
						{
							ueDLContext_p->dlStrategyTxNode_p = dlStrategyTxNode_p;

							(*numDlUEsToBeScheduled_p)--;
							/* SPR 5313 Fix start */
							/* Update Cell Edge Ue */
							if ( CE_USER == userLocationType )
							{
								--maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] ;
							}   
							/* SPR 5313 Fix end */

							ueDLContext_p->scheduleFlag = DL_QOS_ALREADY_TRANSMITED;

							LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCHEDULE_FLAG(ueIndex,
									ueDLContext_p->scheduleFlag);

						}
						else
						{
							/* If there are no FREE HARQ Process you cannot schedule
							 * get the next node and continue 
							 */
							/* SPR 6897 changes start */
							if (pushNodeCircQ(dlPriorityQueue_gp[internalCellIndex],
            /* SPR 23483 Changes Start */
										dlPriorityQNode_p) == CIRC_FAILURE)
            /* SPR 23483 Changes End */
							{
								lteWarning("Error in Pushing in Pending"
										" dlPriorityQueue in funtion %s\n",
										__func__);
								freeMemPool(dlPriorityQNode_p);
								/* Coverity_fix 30670 Start */
								dlPriorityQNode_p = NULL;
								/* Coverity_fix 30670 End */
								/*SPR_13439_Changes Start*/
								ueDLContext_p->isPresentInPriorityQueue = FALSE;
								/*SPR_13439_Changes End*/
							}
							dlPriorityQueueCount--;
							continue;
						}
					}
					else
					{
						dlStrategyTxNode_p = ueDLContext_p->dlStrategyTxNode_p;

					}

					dlEligibleLcNodeVoid_p = (void*) getFirstListNode(
							&ueDLContext_p->dlEligibleLcList);
					while(PNULL != dlEligibleLcNodeVoid_p)
					{
						DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
								dlEligibleLcNodeVoid_p, dlEligibleLcNode_p);

						dlQosStrategyLCNode_p = (DLQosStrategyLCNode *)dlEligibleLcNode_p;
      		   dlQosStrategyLCNode_p->schedulableBytes = dlEligibleLcNode_p->lcInfo_p->queueLoad;
						dlQosStrategyLCNode_p->schedulableBytes += MAC_SUBHEADER_LENGTH_THREE_BYTE;
						dataSize += dlQosStrategyLCNode_p->schedulableBytes;
#ifdef MAC_CA_WORKAROUND
if(ueDLContext_p->scheduledLcCount == 0)
#endif
{
						ueDLContext_p->scheduledLcCount++;
				                //fprintf(stderr,"Error scheduledLcCount more than 1 %s %d at %llu\n",__func__,__LINE__,getCurrentTick());

						LOG_MAC_MSG(MAC_INSERT_SCH_LIST_NODE,LOGBRIEF,
								MAC_MUX,currentGlobalTTITickCount, __LINE__,
								(UInt32)ueDLContext_p->ueIndex, dlQosStrategyLCNode_p->lcId,
								ueDLContext_p->scheduledLcCount,
								DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

						/* Insert the Lc Node in the scheduled LC list */
						listInsertNode(&ueDLContext_p->dlScheduledLcList,
								&dlQosStrategyLCNode_p->scheduledLcAnchor);
}
						dlEligibleLcNodeVoid_p = (void*)
							getNextListNode(
									&(dlEligibleLcNode_p->eligibleLcAnchor));

					};
					if (dataSize > ueDLContext_p->maxTBSize)
					{
						dataSize = ueDLContext_p->maxTBSize;
#ifdef PERF_STATS
						if ( ueDLContext_p->dlStrategyTxNode_p->harqProcess_p->isTBOneValid == TRUE &&
								ueDLContext_p->dlStrategyTxNode_p->harqProcess_p->isTBTwoValid == TRUE )
						{
							if (ueDLContext_p->maxMIMOTBSize < ueDLContext_p->maxRestrictedTB)
							{
								/*  number of ms where respective UE is limited in the
								 *       DL direction by its UE capability */
								gMacUePerfStats_p[ueIndex].lteUeMacSchedDLPerfStats.\
									totalTimeRestrictUeInDlbyUeCap++;
							}
						}
						else 
						{
							if((TRUE == ueDLContext_p->dlStrategyTxNode_p->harqProcess_p->isTBOneValid) &&
									(ueDLContext_p->maxSISOTBSize < ueDLContext_p->maxRestrictedTB))
							{
								/*  number of ms where respective UE is limited in the
								 *       DL direction by its UE capability */
								gMacUePerfStats_p[ueIndex].lteUeMacSchedDLPerfStats.\
									totalTimeRestrictUeInDlbyUeCap++;
							}
						}
#endif
					}
					avgMcs = ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
					/* SPR 5999 changes start */
					maxDLRBWithPriority = DETERMINE_RBs_DL(avgMcs, dataSize,internalCellIndex);
					/* SPR 5999 changes end */
					/* SPR 5313 Fix start */
					numAvailableRBs_p[userLocationType] -= maxDLRBWithPriority;
					RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(maxDLRBWithPriority, 
							numAvailableRBs_p); 
					/* SPR 5313 Fix end */
					dlStrategyTxNode_p->maxTBSize = dataSize;
					dlStrategyTxNode_p->minTBSize = dataSize;
					/*SPR 5388 Fix start*/
					dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB = maxDLRBWithPriority;
					/*SPR 5388 Fix end*/


				}
				else
				{
					LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"[DL-Strategy][%s] NET_UE_QL is Non Zero for ue ID: %d"
							,__func__, ueDLContext_p->ueIndex);
					/* SPR 6897 changes start */
					/* SPR 6897 changes correction start */
					if (pushNodeCircQ(dlPriorityQueue_gp[internalCellIndex],
            /* SPR 23483 Changes Start */
								dlPriorityQNode_p) == CIRC_FAILURE)
            /* SPR 23483 Changes End */
						/* SPR 6897 changes correction end */
						/* SPR 6897 changes end */
					{
						lteWarning("Error in Pushing in Pending"
								" dlPriorityQueue in funtion %s\n",
								__func__);
						freeMemPool(dlPriorityQNode_p);
						/* Coverity_fix 30670 Start */
						dlPriorityQNode_p = NULL;
						/* Coverity_fix 30670 End */
						/*SPR_13439_Changes Start*/
						ueDLContext_p->isPresentInPriorityQueue = FALSE;
						/*SPR_13439_Changes End*/
					}
					dlPriorityQueueCount--;
					continue;
				}
				/* Also free the memory of Priority Nodes*/
				freeMemPool((void *) dlPriorityQNode_p);
				dlPriorityQNode_p = PNULL;
				dlPriorityQueueCount--;
				ueDLContext_p->isPresentInPriorityQueue = FALSE;
			}
			else
			{
				/* Also free the memory of Priority Nodes*/
				freeMemPool((void *) dlPriorityQNode_p);
				dlPriorityQNode_p = PNULL;
				dlPriorityQueueCount--;
			}
		}
		/* SPR#13151 fix start */
		else
		{
			dlPriorityQueueCount--;
			continue;
		}
		/* SPR#13151 fix end */
	}

	LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,"Exit:[DL-Strategy][%s]"
			,__func__);
}
/* SPR 3608 changes end */

/********************************************************************************
 * Function Name  : dlQosStrategyMakeScheduleUeQueue
 * Inputs         : numberOfAvailableRBs_p - Pointer to the number of 
 *                                           Available RBs,
 *                  currentGlobalTTITickCount - Current Global TTI Tick,
 *                  numDlUEsToBeScheduled - Number of DL UE's to be scheduled,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This functions forms the scheduled UE queue and provide to 
 *                  PDCCH. It schedules the UE for 100 + extra buffer configured
 *                  number of RB's. It also restricts the maximum number of UE 
 *                  to be scheduled per tick by cofigured value.
 *                  with decreasng priority and process them. An UE Transmission 
 *                  node will be formed for each scheduled UE when the first LC 
 *                  node of the UE is processed. And it is maintained in an 
 *                  global Array. 
 *                  It fetches the nodes from the prioritised LC Tree and 
 *                  with decreasng priority and process them. An UE Transmission.
 ********************************************************************************/
/* ICIC changes start */
MacRetType dlQosStrategyMakeScheduleUeQueue(SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
        /* SPR 15909 fix start */
		tickType_t  currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		UInt8*  numDlUEsToBeScheduled,
/*+ SPR 22819*/
/*- SPR 22819*/
		InternalCellIndex internalCellIndex)
/* ICIC changes end */
{
	/*
	   1.  Get teh total schedulable RB's 
	   2.  Traverse through the prioritized LC list and allocate RB.s 
	   for each LC upto 150 RB (+ )
	   3.  for each LC node, if((scheduled RB.s <=  maxSchedulableRB) &&
	   lcInfo_p->availToken))
	   a.  if the LC node ==  first LC node of UE to be scheduled
	   and Number of schedulable UE's is Non-Zero
	   i.  And insert UE Node for each scheduled first LC node of UE, in
	   schedule UE list by calling
	   dlQosStrategyUpdateNewTxUEList(eDlTxMsgType)
	   ii. check for hte strategy node if NULL continue with next node
	   b.  Else
	   i.  Get  the strategy node
	   c.  Check for the head of line packet expiry, if not expired
	   i.  Set the Packet drop flag
	   ii. Continue;
	   d.  Else if (Scheduled TB Size of UE  < Max TB Size of UE )
	   i.  if (DL_QOS_RLC_DATA_PDU == lcInfo_p->rlcAmStats)||
	   (DL_QOS_RLC_AM_MODE != lcInfo_p->rlcMode)
	   1.  Obtain the schedulable bytes equal to minimum of 
	   { available tokens , queueLoad } of the LC
	   ii. Else (if (DL_QOS_RLC_DATA_PDU == lcInfo_p->rlcAmStats)
	   1.  Obtain the schedulable bytes equal to 
	   Control Qload of the LC
	   iii.    else
	   1.  Obtain the schedulable bytes equal to First 
	   Re Tx PDU Qload of the LC
	   iv. schedulable bytes  +=  (MAC_SUBHEADER_SIZE + rlcFixedHeaderSize)
	   v.  If((Scheduled TB Size of UE +  schedulable bytes)  >  
	   Max TB Size of UE)
	   1. schedulable bytes  +=  
	   Scheduled TB Size of UE - Max TB Size of UE 
	   2. if(scheduled bytes < 
	   (MAC_SUBHEADER_SIZE + rlcFixedHeaderSize))
	   a. scheduled bytes = 0;
	   3. if(scheduled bytes)
	   a. If scheduling for 100 RB.s is not crossed
	   1.  Increment the Min TB Size with the 
	   schedulable bytes for the LC
	   b. Increment the Max TB Size with the 
	   schedulable bytes for the LC
	   c. get the RB.s required for the LC scheduled and 
	   increment the scheduled RB by it, 
	   dlQosGetResBlksForScheduledBytes(mcsIndex , minTBSize, 
	   harqProcess_p )
	   d. update dlQosStrategyLCNode_p->schedulableBytes
	   e. Append the LC node into the schedlue LC list
	   4.  If scheduled RB.s <=  *numberOfAvailableRBs_p
	   a.  Re-Traverse the Priority LC list  for scheduling remaining RB.s, 
	   dlQosReTraversePriorityLcList(ScheduledRBs,
	   Number of schedulable UE's
	   subframe)
	   5.  Traverse through the UE list and update the required strategy parameters
	   dlQosUpdateStrategyNodes

	 */
	UInt8   reStartTraversing       = TRUE;
	UInt8   gbrSchedulingFlag       = TRUE; 
	UInt8   preferredCellListIndex  = MAX_NUM_CELL;
	UInt16  ueIndex                 = 0;

	MacRetType            retVal                 = MAC_SUCCESS;
	DLUEContext           *ueDLContext_p         = PNULL;
	DLUEContextInfo       *ueDLContextInfo_p     = PNULL;
	DLQosStrategyLCNode   *dlQosStrategyLCNode_p = PNULL;
	DLLogicalChannelInfo  *lcInfo_p              = PNULL;

	InternalCellIndex pCellIndex = INVALID_CELL_INDEX;
	InternalCellIndex cellIndex = 0;


	/* + SPR 13605 Fix */
    UInt32 tempDlAggregateGbrPerTTI[MAX_NUM_CELL][MAX_VALID_UE_TYPE] = {{0,0},{0,0}};

	/** SPR 13599 Changes Start **/
	UInt8 isCaEnabled = macParams_g.isCASupported;
	/** SPR 13599 Changes End **/
	/* EICIC Optimization +- */
       if (isEicicEnabled_g[internalCellIndex])
       {
	/* EICIC DEBUG*/
	UInt32 sysFrameNum 			= 0;
	UInt32 subFrameNum 			= 0;  

	getSFAndSFN(&sysFrameNum, &subFrameNum,internalCellIndex);

	/*Complexity Start*/
	eicicUpdateSfAndSfn(&sysFrameNum, &subFrameNum);
	/*Complexity End*/

	if(isCaEnabled)
	{
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
		{
			tempDlAggregateGbrPerTTI[cellIndex][VICTIM_UE] = dlAggregateGbrPerTTI_g[cellIndex][VICTIM_UE];
			tempDlAggregateGbrPerTTI[cellIndex][NON_VICTIM_UE] = dlAggregateGbrPerTTI_g[cellIndex][NON_VICTIM_UE];
		}
	}
	else
	{
		if ( VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex])
		{
				tempDlAggregateGbrPerTTI[cellIndex][VICTIM_UE] = dlAggregateGbrPerTTI_g[cellIndex][VICTIM_UE];
			}
		else if ( NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex])
		{
			tempDlAggregateGbrPerTTI[cellIndex][NON_VICTIM_UE] = dlAggregateGbrPerTTI_g[cellIndex][NON_VICTIM_UE];
		}

	}
       }
	else
	{
	  	if(isCaEnabled)
		{
			for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
			{
				tempDlAggregateGbrPerTTI[cellIndex][NON_VICTIM_UE] = dlAggregateGbrPerTTI_g[cellIndex][NON_VICTIM_UE];
			}
		}
		else
		{ 
			tempDlAggregateGbrPerTTI[cellIndex][VICTIM_UE] = dlAggregateGbrPerTTI_g[cellIndex][VICTIM_UE];
		        tempDlAggregateGbrPerTTI[cellIndex][NON_VICTIM_UE] = dlAggregateGbrPerTTI_g[cellIndex][NON_VICTIM_UE];

	}
	}
	/* EICIC Optimization +- */

	/* - SPR 13605 Fix */
	/** SPR 13599 Changes End **/
#ifdef SSI_DEBUG_STATS
	/* CA Stats Changes Start */
	updateDlQosPerTTIStats(internalCellIndex);
	/* CA Stats Changes End */
#endif

	/* Initialize local parameters used for Scheduling */
	dlQosInitStrategyLocalParams( numberOfAvailableRBs, 
			&gbrSchedulingFlag,
			numDlUEsToBeScheduled,
			internalCellIndex);

	/** SPR 13599 Changes Start **/
	/*Complexity Start*/

	dlQosStrategyLCNode_p = eicicUpdatePreferredCellList(
			isCaEnabled,
			internalCellIndex,
			reStartTraversing
			);
	/*Complexity End*/
	/* EICIC -*/
	reStartTraversing = FALSE;

	while(NULL != dlQosStrategyLCNode_p)
	{
		if(isCaEnabled)
		{
			/* Get preference Cell List for the UE */
			retVal = dlQosGetPreferedCellListForScheduling( 
					dlQosStrategyLCNode_p->ueIndex,
					FALSE /*isMacCEScheduled*/,
                                                &preferredCellListIndex,
                                                /* SPR 16116 fix start */
                                                dlQosStrategyLCNode_p->totLCWeightWithLP); 
                                                /* SPR 16116 fix end */
			if(MAC_FAILURE == retVal)
			{
				/* EICIC +*/    
				if ( VICTIM_UE != ueTypeToScheduleDL_g[internalCellIndex])
				{	    
					/* Get the next node from the TREE traversing IN-ORDER */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( 
							internalCellIndex, 
							reStartTraversing); 

					/* update gbrSchedulingFlag  & reStartTraversing if required */ 
					dlQosUpdateGbrSchdlAndReStartTraversing( 
							&dlQosStrategyLCNode_p,
							&gbrSchedulingFlag,
							&reStartTraversing,
							internalCellIndex, NON_VICTIM_UE);
				}
				else
				{
					/* Get the next node from the TREE traversing IN-ORDER */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode( 
							internalCellIndex, 
							reStartTraversing); 

					/* update gbrSchedulingFlag  & reStartTraversing if required */ 
					dlQosUpdateGbrSchdlAndReStartTraversing( 
							&dlQosStrategyLCNode_p,
							&gbrSchedulingFlag,
							&reStartTraversing,
							internalCellIndex,VICTIM_UE);
				}    
				/* EICIC -*/
				continue;
			}
		}

		ueIndex  = dlQosStrategyLCNode_p->ueIndex;
		ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
		if(!isCaEnabled)
		{
			if(ueDLContextInfo_p->pendingDeleteFlag)
			{
				if ( VICTIM_UE != ueTypeToScheduleDL_g[internalCellIndex])
				{

					/* Get the next node from the TREE traversing IN-ORDER */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( 
							internalCellIndex, 
							reStartTraversing); 
					/* EICIC +*/    
					/* update gbrSchedulingFlag  & reStartTraversing if required */ 
					dlQosUpdateGbrSchdlAndReStartTraversing( 
							&dlQosStrategyLCNode_p,
							&gbrSchedulingFlag,
							&reStartTraversing,
							internalCellIndex,NON_VICTIM_UE);

				}
				else 
				{
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode( 
							internalCellIndex, 
							reStartTraversing); 

					/* update gbrSchedulingFlag  & reStartTraversing if required */ 
					dlQosUpdateGbrSchdlAndReStartTraversing( 
							&dlQosStrategyLCNode_p,
							&gbrSchedulingFlag,
							&reStartTraversing,
							internalCellIndex,VICTIM_UE);		
				}
				continue;
			}
		}

		/** SPR 13599 Changes End **/
		/* Get UE Context */
		ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
		pCellIndex = ueDLContext_p->internalCellIndex;

            if((isRbAvailableOnPreferredCells( pCellIndex, preferredCellListIndex)) ||
                    (isTotalRbMIMOAvailableOnPreferredCells( pCellIndex, preferredCellListIndex)))
		{
			lcInfo_p = dlQosStrategyLCNode_p->lcInfo_p; 

#ifdef SSI_DEBUG_STATS
			gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat
				[lcInfo_p->logicalChannelId].ctrlFlag =  
				lcInfo_p->dlLcQosInfo.eRlcAmStatus;
#endif

			/* Check all validation before processing dlQosStrategyLCNode_p */
			retVal = dlQosValidatePriorityTreeNode( ueDLContext_p,
					dlQosStrategyLCNode_p,
					currentGlobalTTITickCount,
					internalCellIndex, 
					gbrSchedulingFlag);
			if(MAC_FAILURE == retVal)
			{
				/* EICIC +*/    
				if ( VICTIM_UE != ueTypeToScheduleDL_g[internalCellIndex])
				{	    
					/* Get the next node from the TREE traversing IN-ORDER */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( 
							internalCellIndex, 
							reStartTraversing); 

					/* update gbrSchedulingFlag  & reStartTraversing if required */ 
					dlQosUpdateGbrSchdlAndReStartTraversing( 
							&dlQosStrategyLCNode_p,
							&gbrSchedulingFlag,
							&reStartTraversing,
							internalCellIndex,NON_VICTIM_UE);
				}
				else
				{
					/* Get the next node from the TREE traversing IN-ORDER */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode( 
							internalCellIndex, 
							reStartTraversing); 

					/* update gbrSchedulingFlag  & reStartTraversing if required */ 
					dlQosUpdateGbrSchdlAndReStartTraversing( 
							&dlQosStrategyLCNode_p,
							&gbrSchedulingFlag,
							&reStartTraversing,
							internalCellIndex, VICTIM_UE);
				}    
				/* EICIC -*/
				continue;
			}

			LOG_MAC_MSG(MAC_SCHD_LC_ON_PREF_CELL_5,LOGDEBUG,MAC_DL_Strategy,
					getCurrentTick(),
					ueIndex,
					dlQosStrategyLCNode_p->lcId,
					dlQosStrategyLCNode_p->totLCWeight,
					lcInfo_p->dlLcQosInfo.availToken,
					lcInfo_p->queueLoad,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME, "");
			/* If availToken is zero , UE can't be scheduled in any serving cell. */
			if ( lcInfo_p->dlLcQosInfo.availToken <= 0 ) 
			{

				LOG_UT(MAC_LC_NODE_NOT_PROCESSED_IN_DL_LOG_ID,
						LOGDEBUG, MAC_DL_Strategy,
						currentGlobalTTITickCount,
						ueIndex,
						dlQosStrategyLCNode_p->lcId,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME, "No Tokens Available for the LC");


				/* EICIC +*/    
				if ( VICTIM_UE != ueTypeToScheduleDL_g[internalCellIndex])
				{	    
					/* Get the next node from the TREE traversing IN-ORDER */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( 
							internalCellIndex, 
							reStartTraversing); 

					/* update gbrSchedulingFlag  & reStartTraversing if required */ 
					dlQosUpdateGbrSchdlAndReStartTraversing( 
							&dlQosStrategyLCNode_p,
							&gbrSchedulingFlag,
							&reStartTraversing,
							internalCellIndex, NON_VICTIM_UE);
				}
				else
				{
					/* Get the next node from the TREE traversing IN-ORDER */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode( 
							internalCellIndex, 
							reStartTraversing); 

					/* update gbrSchedulingFlag  & reStartTraversing if required */ 
					dlQosUpdateGbrSchdlAndReStartTraversing( 
							&dlQosStrategyLCNode_p,
							&gbrSchedulingFlag,
							&reStartTraversing,
							internalCellIndex, VICTIM_UE);
				}    
				/* EICIC -*/
                /*SPR 21452 code removed*/
				continue;
			}

			/* SPR 12372 Changes End */
			if((TRUE == gbrSchedulingFlag) &&
					(dlQosStrategyLCNode_p->schedulableBytes))
			{
				ltePanic("Scheduled LC Not Freed\n");
			}

			/* Check whether need to apply any preference to Cells or not */
			/* + SPR 13605 Fix */
			/** SPR 13599 Changes Start **/
			if( isCaEnabled && dlQoSPreferredCellList_g[pCellIndex].preferredCellList\
					[preferredCellListIndex].isPrefRequired)
				/** SPR 13599 Changes End **/
            {
                    dlQoSApplyPreferenceFactor( ueDLContext_p, 
                            dlQosStrategyLCNode_p,
                            preferredCellListIndex,
                            tempDlAggregateGbrPerTTI);
                /*SPR 21948 Changes End */
            }

			/* - SPR 13605 Fix */

			/* Schedule LC on the Cells as per PreferredCellList of the UE */
			dlQoSScheduleLcOnPreferredCells(
					ueDLContext_p, 
					dlQosStrategyLCNode_p,
					currentGlobalTTITickCount,
					preferredCellListIndex,
					&gbrSchedulingFlag,
					&reStartTraversing,
					tempDlAggregateGbrPerTTI);
		}
        /*+- SPR 20895 */
        else
        {
            break;
        }
        /*+- SPR 20895 */

		/* EICIC +*/    
		if ( VICTIM_UE != ueTypeToScheduleDL_g[internalCellIndex])
		{	    
			/* Get the next node from the TREE traversing IN-ORDER */
			dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( 
					internalCellIndex, 
					reStartTraversing); 

			/* update gbrSchedulingFlag  & reStartTraversing if required */ 
			dlQosUpdateGbrSchdlAndReStartTraversing( 
					&dlQosStrategyLCNode_p,
					&gbrSchedulingFlag,
					&reStartTraversing,
					internalCellIndex, NON_VICTIM_UE);
		}
		else
		{
			/* Get the next node from the TREE traversing IN-ORDER */
			dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode( 
					internalCellIndex, 
					reStartTraversing); 

			/* update gbrSchedulingFlag  & reStartTraversing if required */ 
			dlQosUpdateGbrSchdlAndReStartTraversing( 
					&dlQosStrategyLCNode_p,
					&gbrSchedulingFlag,
					&reStartTraversing,
					internalCellIndex,VICTIM_UE);
		}    
		/* EICIC -*/

	}/* End of while */

	if (isCaEnabled)
	{
		/* Retraverse Priority LC Tree if required */
		dlQosReTraversePriorityLcListForAllCells( currentGlobalTTITickCount,
				internalCellIndex);
	}
	else
	{
		if (INVALID_UE_TYPE != ueTypeToScheduleDL_g[internalCellIndex])
		{
			/* Retraverse Priority LC Tree if required */
			dlQosReTraversePriorityLcListForAllCells( currentGlobalTTITickCount,
					internalCellIndex);
		}
	}

	dlQosUpdateDlBufferPoolAndResetTotalRbMIMO(internalCellIndex);

	return MAC_SUCCESS;
}/* End of the function dlQosStrategyMakeScheduleUeQueue */


/*Complexity Start*/
/*EICIC +*/
/*****************************************************************************
 * Function Name  : eicicUpdatePreferredCellList
 * Input          : isCaEnabled 
 *                  internalCellIndex
 *                  reStartTraversing
 * Output         : Pointer to DL Startegy Node
 * Description    : Updates Preferred Cell list for EICIC
 *****************************************************************************/

DLQosStrategyLCNode* eicicUpdatePreferredCellList(
		UInt8 isCaEnabled,
		InternalCellIndex internalCellIndex,
		UInt8   reStartTraversing
		)
{
    DLQosStrategyLCNode   *dlQosStrategyLCNode_p = PNULL;
    InternalCellIndex cellIndex = 0;
    UInt8 isflag_Set = FALSE;		
    UInt8   listIndex               = 0; 
    /* +- SPR 18268 */
    UInt16   ueCount                 = 0; 
    /* +- SPR 18268 */
    UInt16  ueIndex                 = 0;


    /* SPR 16116 fix start */
    /* Get first LC node from the Priority LC Tree */   
    /* internalCellIndex would be used only in non-CA case */
    dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( internalCellIndex, 
            reStartTraversing);

    /* Removed code: After setting reStartTraversing as FALSE, 
     * tree traversal is returning NULL value resulting problem in UE attach .*/
    if((isCaEnabled) && (NULL != dlQosStrategyLCNode_p))
    {
        /* SPR 16116 fix end */
        /* Update Preferred cell list of All UEs considered for  
         * MAC CE scheduling in either preferredCellList */
        for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++ )
        {
            ueCount = dlQoSPreferredCellList_g[cellIndex].preferredCellListCount;
            for(listIndex = 0; listIndex < ueCount; listIndex++)
            {
                if(dlQoSPreferredCellList_g[cellIndex].preferredCellList[listIndex].count > 1)
                {
                    ueIndex = dlQoSPreferredCellList_g[cellIndex].preferredCellList[listIndex].ueIndex;

                    dlQosUpdatePreferredCellList( ueIndex, 
                            listIndex, 
                            cellIndex,
                            /* SPR 16116 fix start */
                            dlQosStrategyLCNode_p->totLCWeightWithLP);
                    /* SPR 16116 fix end */
                }
            }
        }
    }

	if (isEicicEnabled_g[internalCellIndex])
    {
        if(isCaEnabled)
        {
            /* Update Preferred cell list of All UEs considered for  
             * MAC CE scheduling in either preferredCellList */
            for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++ )
            {
                if (VICTIM_UE == ueTypeToScheduleDL_g[cellIndex] )
                {
                    isflag_Set = TRUE;
                    break;
                }
                else
                {
                    isflag_Set  = FALSE;
                }
            }

            if (!isflag_Set)
            {
                for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++ )
                {
                    if (NON_VICTIM_UE == ueTypeToScheduleDL_g[cellIndex] )
                    {
                        isflag_Set = TRUE;
                        break;
                    }
                    else
                    {
                        isflag_Set  = FALSE;
                    }
                }

            }

        }

        if (isflag_Set)
        {
            if ( NON_VICTIM_UE == ueTypeToScheduleDL_g[cellIndex])
            {
                /* Get first LC node from the Priority LC Tree */   
                /* internalCellIndex would be used only in non-CA case */

                dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( cellIndex, 
                        reStartTraversing);
            }
            else if (VICTIM_UE == ueTypeToScheduleDL_g[cellIndex])
            {
                /* Get first LC node from the Priority LC Tree */   
                /* internalCellIndex would be used only in non-CA case */

                dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode(cellIndex, 
                        reStartTraversing);
            }
        }
        else
        {
            if ( NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex])
            {
                /* Get first LC node from the Priority LC Tree */   
                /* internalCellIndex would be used only in non-CA case */

                dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( internalCellIndex, 
                        reStartTraversing);
            }
            else if (VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex])
            {
                /* Get first LC node from the Priority LC Tree */   
                /* internalCellIndex would be used only in non-CA case */
                dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode(internalCellIndex, 
                        reStartTraversing);
            }
        }
    }
	else
	{
		/* Get first LC node from the Priority LC Tree */   
		/* internalCellIndex would be used only in non-CA case */

		dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( internalCellIndex, 
				reStartTraversing);
	}

    return dlQosStrategyLCNode_p;
}
/*****************************************************************************
 * Function Name  : dlQosGetTransmissionMode
 * Inputs         : dlStrategyTxNode_p - The UE Tx Node to be updated,
 *                  ueDLContext_p - Pointer to UE Context,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  currentGlobalTTITickCount -Current Global TTI Tick,
 *                  servingCellIndex - cell index used at MAC.
 * Outputs        : dlStrategyTxNode_p - The UE Tx Node to be updated
 *                  harqProcess_p - Pointer to DLHARQProcess,
 * Returns        : NONE
 * Description    : This fucntion will decide the Transmission mode for the 
 *                  current scheduling i.e Spatial or Non-Spatial.
 *                  It will decide based on the RI received whetehre TWO TB 
 *                  need to be transmitted or only one.
 *                  It will also decide the primary and secondary DCI for 
 *                  based on the transmission mode And HARQ will be updated
 *                  for the same.
 *****************************************************************************/

 void  dlQosGetTransmissionMode (DLStrategyTxNode * dlStrategyTxNode_p,
		DLUEContext      * ueDLContext_p,
		DLHARQProcess    * harqProcess_p,
		/* + TM7_8 Changes Start */
        /* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		/* - TM7_8 Changes End */
		ServingCellIndex servingCellIndex)
{
	/*
	   1. Check for the Transmission mode
	   2. Update the reported RI
	   3. Validate the MCS for the threshold value
	   4. Update the Number fo TB.s to be transmitted 
	   5. Select DCI format and update the New Tx node    
	   6. Update the HARQ for MCS  and  transmission Indicator
	 */

	/* + CL-MIMO LJA*/
	UInt8  tempTransmissionMode = 0;
	/* - CL-MIMO LJA*/
	/* + CQI_5.0 */
	TempStrategyInputInfoForOneTB tempStrategyInputInfoForOneTB = {0};
	UInt8 tb1only_sendFlag = 0;
	UInt8 mcsIndexCW1 = 0;
	UInt8 mcsIndexCW2 = 0;
	UInt8 mcsThreshold = 0;
	/*4x4 DL MIMO CHG START*/
	UInt8 mcsThresholdLayer3 = 0;
	UInt8 mcsThresholdLayer4 = 0;
	/*4x4 DL MIMO CHG END*/
	/* - CQI_5.0 */

	DLMIMOInfo* dlMIMOInfo_p     = PNULL;
	DLCQIInfo*  dlCQIInfo_p      = PNULL;
	DCIFormat   tm6DCIFormat     = MAX_NUM_DCI_FORMAT;
	UInt8       userLocationType = CC_USER;
	UInt16*     maxTBSize_P      = PNULL;

	if(IS_PCELL_OF_UE(ueDLContext_p, servingCellIndex))
	{
		dlMIMOInfo_p     = &(ueDLContext_p->dlMIMOInfo);
		dlCQIInfo_p      = &(ueDLContext_p->dlCQIInfo);
		tm6DCIFormat     = ueDLContext_p->tm6DCIFormat;
		userLocationType = ueDLContext_p->userLocationType;
		maxTBSize_P      = &(ueDLContext_p->maxTBSize);
	}
	else
	{
		dlMIMOInfo_p     = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo);
		dlCQIInfo_p      = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
		tm6DCIFormat     = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->Scelltm6DCIFormat;
		userLocationType = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
		maxTBSize_P      = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->maxTBSize);
	}
	tempTransmissionMode = dlMIMOInfo_p->transmissionMode;

	/* Check for the Transmission mode */
	/*Two TBs can be sent if TX_MODE_3 or TX_MODE_4*/
	if ((TX_MODE_3 == tempTransmissionMode)||
			(TX_MODE_4 == tempTransmissionMode)||
			/* + TM6_5.2 */ 
			(TX_MODE_6 == tempTransmissionMode)|| 
			/* - TM6_5.2 */ 
			/* + TM7_8 Changes Start */
			(((TX_MODE_7 == tempTransmissionMode) ||
			  (TX_MODE_8 == tempTransmissionMode)) &&
			 (1 < cellSpecificParams_g.cellConfigAndInitParams_p[servingCellIndex]\
			  ->cellParams_p->numOfTxAnteenas)))
		/* - TM7_8 Changes End */
	{
		/* SPR 4725 Start */
		if ( ueDLContext_p->srbQloadAvailbl == TRUE)
		{
			/*Setting only TBOne as valid and other INVALID*/
			harqProcess_p->isTBOneValid = TRUE;
			harqProcess_p->isTBTwoValid = FALSE;

			harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;
			harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;
			/*DCI Format Calculation*/

			dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_1A ;

			dlStrategyTxNode_p->secondaryDCIFormat=DCI_FORMAT_1A;

			harqProcess_p->dlHARQTBOneInfo.mcsIndex  =
				dlCQIInfo_p->mcsIndexCodeWordOne;

			*maxTBSize_P = ueDLContext_p->maxSISOTBSize;
			dlStrategyTxNode_p->msgType = NON_SPATIAL_MUL;
		}
		else
		{
			/* SPR 4725 End */
			/* + TM6_5.2 */
			if(TX_MODE_6 == tempTransmissionMode)
			{
				tb1only_sendFlag = 1;
				POPULATE_INFO_FOR_ONE_TB(tempTransmissionMode,
						1,
						tempStrategyInputInfoForOneTB,
						/* + TM7_8 Changes Start */
						BF_ALGO_NOT_AVAILABLE,servingCellIndex);
				/* - TM7_8 Changes End */
				tempStrategyInputInfoForOneTB.primaryDCIFormat = tm6DCIFormat;
				tempStrategyInputInfoForOneTB.secondaryDCIFormat = DCI_FORMAT_1A;
			}
			else
			{
				/* - TM6_5.2 */
				/* + CQI_5.0 */
				mcsIndexCW1 = dlCQIInfo_p->mcsIndexCodeWordOne;
				mcsIndexCW2 = dlCQIInfo_p->mcsIndexCodeWordTwo;
				/* SPR 7981 changes start */
				mcsThreshold = cellSpecificParams_g.cellConfigAndInitParams_p\
					       [servingCellIndex]->initParams_p->dlSchdConfig.mcsThreshold[userLocationType];
				/* SPR 7981 changes end */

				/* SPR 7188 Changes starts */
				if(( (RANK_INDICATOR_1 == dlMIMOInfo_p->dlRi)
							|| ( (mcsIndexCW1 <=  mcsThreshold) || (mcsIndexCW2 <= mcsThreshold))
							/* SPR 7188 Changes ends */
				   )||
						/* + TM7_8 Changes Start */
						((TX_MODE_8 == tempTransmissionMode) && 
						 (BF_ALGO_NOT_AVAILABLE ==
						  (macReconfigSchedulerParamsDL_gp[servingCellIndex] + schParamsIndexDL_g[servingCellIndex])->bfVectorPresentFlag))) 
					/* - TM7_8 Changes End */
				{
					tb1only_sendFlag = 1;
					POPULATE_INFO_FOR_ONE_TB(tempTransmissionMode,
							1,
							tempStrategyInputInfoForOneTB,
							/* + TM7_8 Changes Start */
							ueDLContext_p->beamFormingAlgoToBeUsed,servingCellIndex);
					/* - TM7_8 Changes End */
					/* + SPR_6854 */
					/* SPR 7981 changes start */
					if((mcsIndexCW2 <= mcsThreshold) &&
							(mcsIndexCW1 > mcsThreshold))
					{
						dlCQIInfo_p->cqiCorrectionFactorForCW1 =
							dlCQIInfo_p->cqiCorrectionFactorForCW0;
					}
					/* SPR 7981 changes end */
					/* - SPR_6854 */
				}
				/* + TM7_8 Changes End */ 
				if((6 == cellSpecificParams_g.cellConfigAndInitParams_p[servingCellIndex]\
							->cellParams_p->dlAvailableRBs) &&        
                        /* SPR 21631 Start */
                    (CHECK_SF_MAPPED_ON_BCH_PSS_SSS(currentGlobalTTITickCount,
										tempTransmissionMode)))
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
				mcsThresholdLayer3 = getMCSFromCQI(cellSpecificParams_g.cellConfigAndInitParams_p[servingCellIndex]\
                ->initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer3[userLocationType]
                 );
				mcsThresholdLayer4 = getMCSFromCQI(cellSpecificParams_g.cellConfigAndInitParams_p[servingCellIndex]\
                ->initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer4[userLocationType]
                 );

				if ( ( (RANK_INDICATOR_4 == dlMIMOInfo_p->dlRi) ||
							(RANK_INDICATOR_3 == dlMIMOInfo_p->dlRi)) &&
						(tb1only_sendFlag != 1) )
				{
					if ((dlCQIInfo_p->mcsIndexCodeWordOne > mcsThresholdLayer4) &&
							(dlCQIInfo_p->mcsIndexCodeWordTwo > mcsThresholdLayer4))
					{
						harqProcess_p->numOfLayer = dlMIMOInfo_p->dlRi;
					}
					else if(dlCQIInfo_p->mcsIndexCodeWordTwo > mcsThresholdLayer3)
					{
						harqProcess_p->numOfLayer = RANK_INDICATOR_3;
					}
					else
					{
						harqProcess_p->numOfLayer = RANK_INDICATOR_2;
					}
				}
				/*4X4 DL MIMO CHG END*/
				/* + TM6_5.2 */
			} 
			/* - TM6_5.2 */
			if (tb1only_sendFlag)
			{
				harqProcess_p->isTBOneValid = 
					tempStrategyInputInfoForOneTB.isTBOneValid;
				harqProcess_p->isTBTwoValid = 
					tempStrategyInputInfoForOneTB.isTBTwoValid;

				harqProcess_p->dlHARQTBOneInfo.txIndicator = 
					tempStrategyInputInfoForOneTB.txIndicatorForTB1;
				harqProcess_p->dlHARQTBTwoInfo.txIndicator = 
					tempStrategyInputInfoForOneTB.txIndicatorForTB2;

				/*DCI Format Calculation*/
				dlStrategyTxNode_p->primaryDCIFormat = 
					tempStrategyInputInfoForOneTB.primaryDCIFormat;
				dlStrategyTxNode_p->secondaryDCIFormat = 
					tempStrategyInputInfoForOneTB.secondaryDCIFormat;

				harqProcess_p->dlHARQTBOneInfo.mcsIndex  =  
					dlCQIInfo_p->mcsIndexCodeWordOne;

				/* else
				   {
				   harqProcess_p->dlHARQTBTwoInfo.mcsIndex  =  
				   dlCQIInfo_p->mcsIndexCodeWordTwo;
				   }*/   

				*maxTBSize_P = ueDLContext_p->maxSISOTBSize;

			}
			else
			{
				harqProcess_p->isTBOneValid = TRUE;
				harqProcess_p->isTBTwoValid = TRUE;

				harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;
				harqProcess_p->dlHARQTBTwoInfo.txIndicator = NEW_TX;
				/*DCI Format Calculation*/
				dciFormatSelector (ueDLContext_p,dlStrategyTxNode_p,
						/* + TM7_8 Changes Start */
						HARQ_FREE_TX_3_4_8,
						/* - TM7_8 Changes End */
						servingCellIndex);

				harqProcess_p->dlHARQTBOneInfo.mcsIndex  =  
					dlCQIInfo_p->mcsIndexCodeWordOne;
				harqProcess_p->dlHARQTBTwoInfo.mcsIndex  =  
					dlCQIInfo_p->mcsIndexCodeWordTwo;

				*maxTBSize_P = ueDLContext_p->maxMIMOTBSize;
			}

			dlStrategyTxNode_p->msgType = SPATIAL_MUL;
			/* - CQI_5.0 */
			/* SPR 4725 Start */
		}
		/* SPR 4725 End */
	}
	else
	{
		/*Setting only TBOne as valid and other INVALID*/
		harqProcess_p->isTBOneValid = TRUE;
		harqProcess_p->isTBTwoValid = FALSE;

		harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;
		harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;
		/* SPR 14495 fix start */
		/*DCI Format Calculation*/
		if (ueDLContext_p->srbQloadAvailbl == TRUE)
		{
			dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_1A ;
			dlStrategyTxNode_p->secondaryDCIFormat=DCI_FORMAT_1A;
		}
		else
		{
			dciFormatSelector (ueDLContext_p,dlStrategyTxNode_p,
					HARQ_FREE_TX_OTHER_THAN_3_4,
					servingCellIndex);
		}
		/* SPR 14495 fix end */

		harqProcess_p->dlHARQTBOneInfo.mcsIndex  =  
			dlCQIInfo_p->mcsIndexCodeWordOne;

		*maxTBSize_P = ueDLContext_p->maxSISOTBSize;

		dlStrategyTxNode_p->msgType = NON_SPATIAL_MUL;
	}

	/* SPR 4725 Start */
	ueDLContext_p->srbQloadAvailbl = FALSE;
	/* SPR 4725 End */
}


/*****************************************************************************
 * Function Name  : dlQosGetResBlksForScheduledBytes
 * Inputs         : maxTBSize - MAX TB Size,
 *                  scheduledDataSize - Scheduled Data Size,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  subFrameNum - Subframe number,
 *                  ueCategory - UE Category,
 *                  traversalType - NEW TRAVERSAL/RETRAVERSAL,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function determines the RB's required for the minTB.
 *****************************************************************************/
 SInt16 dlQosGetResBlksForScheduledBytes(
		/* SPR 4543 changes start */
		UInt16 maxTBSize, 
		/* SPR 4543 changes end */
		UInt32 scheduledDataSize, 
		DLHARQProcess  *harqProcess_p,
        /* +- SPR 17777 */
		/*4x4 DL MIMO CHG START*/
		UInt8 ueCategory,
		/*4x4 DL MIMO CHG END*/
		/* SPR 10143 fix start */
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
		UInt8 subFrameNum,
		dlQosTraversalType traversalType,
#endif
        /* +- SPR 17777 */
		/* SPR 10143 fix end */
		InternalCellIndex internalCellIndex
		)
{

	UInt8 mcsIndex = 0;
	UInt8 numScheduledRB = 0;
	UInt8 numScheduledRBFromArr1 = 0;
	UInt8 numScheduledRBFromArr2 = 0;
	UInt32 scheduledDataSizeFromArr1= 0;
	UInt32 scheduledDataSizeFromArr2 = 0;
#ifdef TDD_CONFIG
	UInt8 isSpecialSubFrame = checkForSpecialSubframe(subFrameNum, internalCellIndex);
	harqProcess_p->subFrameType = DL;
#endif
	/* SPR 4543 changes start */
	UInt32 tbSizeCW1 = 0;
	UInt32 tbSizeCW2 = 0;
	/* SPR 4543 changes end */

	if((TRUE == harqProcess_p->isTBOneValid ) && 
			(TRUE == harqProcess_p->isTBTwoValid))
	{   /*4x4 DL MIMO CHG START*/
		/* SPR 11355 Fix Start */
		if(harqProcess_p->numOfLayer >= RANK_INDICATOR_3 && 
				((ueCategory == UE_CAT_5) || (ueCategory == UE_CAT_8)))
			/* SPR 11355 Fix End */
		{
			mcsIndex = ( harqProcess_p->dlHARQTBOneInfo.mcsIndex <    
					harqProcess_p->dlHARQTBTwoInfo.mcsIndex) ?
				harqProcess_p->dlHARQTBOneInfo.mcsIndex:
				harqProcess_p->dlHARQTBTwoInfo.mcsIndex;

			if (harqProcess_p->numOfLayer == RANK_INDICATOR_3)
			{
				scheduledDataSizeFromArr1 = (scheduledDataSize / RANK_INDICATOR_3);
				numScheduledRBFromArr1 = DETERMINE_RBs_DL(mcsIndex, scheduledDataSizeFromArr1, internalCellIndex);
				scheduledDataSizeFromArr2 = ((scheduledDataSize << 1)/ RANK_INDICATOR_3);
				numScheduledRBFromArr2 = DETERMINE_RBs_ARRL2_DL(mcsIndex, scheduledDataSizeFromArr2,internalCellIndex);
				numScheduledRB = (numScheduledRBFromArr1 > numScheduledRBFromArr2) ?
					numScheduledRBFromArr1:
					numScheduledRBFromArr2;
			}
			else
			{    
				scheduledDataSize = scheduledDataSize >> 1;
				numScheduledRB = DETERMINE_RBs_ARRL2_DL(mcsIndex, scheduledDataSize,internalCellIndex);
			}  
		} 
		else
		{    
			scheduledDataSize = scheduledDataSize >> 1;
			mcsIndex = ( harqProcess_p->dlHARQTBOneInfo.mcsIndex <    
					harqProcess_p->dlHARQTBTwoInfo.mcsIndex) ?
				harqProcess_p->dlHARQTBOneInfo.mcsIndex:
				harqProcess_p->dlHARQTBTwoInfo.mcsIndex;
			/* SPR 5999 changes start */
			numScheduledRB = DETERMINE_RBs_DL(mcsIndex, scheduledDataSize, internalCellIndex);
			/* SPR 5999 changes end */
		} 
	}
	else
	{
		/* + CQI_5.0 */
		mcsIndex = harqProcess_p->dlHARQTBOneInfo.mcsIndex;
		/* SPR 5999 changes start */
		numScheduledRB = DETERMINE_RBs_DL(mcsIndex, scheduledDataSize, internalCellIndex);
		/* SPR 5999 changes end */

		/* else if( TRUE == harqProcess_p->isTBTwoValid)
		   {
		   mcsIndex = harqProcess_p->dlHARQTBTwoInfo.mcsIndex; 
		   }*/
		/* - CQI_5.0 */  
	}    
	/* SPR 11355 Fix Start */
	if(((ueCategory == UE_CAT_5) || (ueCategory == UE_CAT_8))
			&& harqProcess_p->numOfLayer >= RANK_INDICATOR_3)
		/* SPR 11355 Fix End */
	{
		/* Coverity_73339 Fix Start */
		if(0 != numScheduledRB)
		{
			if(harqProcess_p->numOfLayer == RANK_INDICATOR_3)
			{
				tbSizeCW1 = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
						numScheduledRB,TB_MAPPED_TO_ONE_LAYER);
			}
			else
			{
				tbSizeCW1 = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
						numScheduledRB,TB_MAPPED_TO_TWO_LAYER);
			}
		}
		/* Coverity_73339 Fix End */

		/* SPR 6136 start */
		/* tbSizeCW2 = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
		   numScheduledRB);*/
		/* SPR 6136 end */
		if((TRUE == harqProcess_p->isTBOneValid ) &&
				(TRUE == harqProcess_p->isTBTwoValid))
		{
			/* SPR 6136 start */

			/* Coverity_73339 Fix Start */
			if(0 != numScheduledRB)
			{
			tbSizeCW2 = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
					numScheduledRB,TB_MAPPED_TO_TWO_LAYER);
			} 
			/* Coverity_73339 Fix End */

			/* SPR 6136 end */
			while( (numScheduledRB > 1) &&
					((tbSizeCW1 + tbSizeCW2) > maxTBSize) )
			{
				numScheduledRB--;
				if(harqProcess_p->numOfLayer == RANK_INDICATOR_3)
				{
					tbSizeCW1 = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
							numScheduledRB,TB_MAPPED_TO_ONE_LAYER);
				}
				else
				{
					tbSizeCW1 = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
							numScheduledRB,TB_MAPPED_TO_TWO_LAYER);
				}
				tbSizeCW2 = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
						numScheduledRB,TB_MAPPED_TO_TWO_LAYER);
			}
		}
	}/*4x4 DL MIMO CHG END*/
	else
	{
		/* SPR 4543 changes start */
		/*4x4 DL MIMO CHG START*/

		/* Coverity_73339 Fix Start */
		if(0 != numScheduledRB)
		{
		tbSizeCW1 = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
				numScheduledRB,TB_MAPPED_TO_ONE_LAYER);
		} 
		/* Coverity_73339 Fix End */

		/*4x4 DL MIMO CHG END*/ 
		/* SPR 6136 start */
		/* tbSizeCW2 = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
		   numScheduledRB);*/
		/* SPR 6136 end */
		if((TRUE == harqProcess_p->isTBOneValid ) && 
				(TRUE == harqProcess_p->isTBTwoValid))
		{
			/* SPR 6136 start */

			/* Coverity_73339 Fix Start */
			if(0 != numScheduledRB)
			{
			tbSizeCW2 = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
					numScheduledRB,TB_MAPPED_TO_ONE_LAYER);
			} 
			/* Coverity_73339 Fix End */

			/* SPR 6136 end */
			while( (numScheduledRB > 1) && 
					((tbSizeCW1 + tbSizeCW2) > maxTBSize) )
			{
				numScheduledRB--;
				/*4x4 DL MIMO CHG START*/ 
				tbSizeCW1 = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
						numScheduledRB,TB_MAPPED_TO_ONE_LAYER);
				tbSizeCW2 = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
						numScheduledRB,TB_MAPPED_TO_ONE_LAYER);
				/*4x4 DL MIMO CHG END*/
			}
		}
	}
	/* SPR 4543 changes end */
#ifdef TDD_CONFIG
	/* SPR 9123 fix start */
	if(isSpecialSubFrame)
	{
		harqProcess_p->subFrameType = SP;
        	/* SPR 22169 fix start */
		/* SPR 10144 fix removed */

	        /* Logic to calculate IBS index from MCS Index is removed. 
        	 * since the calculation for IBS index is already done in function
	         * determineRBs()
        	 */
		numScheduledRB = DETERMINE_RBs_DL(mcsIndex, scheduledDataSize, internalCellIndex);
	        /* SPR 22169 fix end */
		numScheduledRB = (numScheduledRB*4)/3 +1 ;
		/* SPR 10143 fix start */
		if ((DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.\
					cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
					dlSchdConfig.dlSchedulerStrategy) &&\
				(numScheduledRB > maxRestrictedRbDL_g[internalCellIndex]) &&
				(NEW_TRAVERSAL == traversalType))
		{
			numScheduledRB = maxRestrictedRbDL_g[internalCellIndex];
		}
		/* SPR 10143 fix end */
		else if (numScheduledRB >= cellSpecificParams_g.\
				cellConfigAndInitParams_p[internalCellIndex]\
				->cellParams_p->dlAvailableRBs)
		{
			numScheduledRB = cellSpecificParams_g.\
					 cellConfigAndInitParams_p[internalCellIndex]\
					 ->cellParams_p->dlAvailableRBs;
		}

		/* Cyclomatic_complexity_changes_start */
		checkSplSfAndCalcTbsizeSiso(isSpecialSubFrame,
				harqProcess_p->dlHARQTBOneInfo.mcsIndex,
				numScheduledRB,
				&tbSizeCW1);
		/* SPR 6136 end */
		if((TRUE == harqProcess_p->isTBOneValid ) &&
				(TRUE == harqProcess_p->isTBTwoValid))
		{

			checkSplSfAndCalcTbsizeSiso(isSpecialSubFrame,
					harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
					numScheduledRB,
					&tbSizeCW2);

			/* SPR 6136 end */
			while( (numScheduledRB > 1) &&
					((tbSizeCW1 + tbSizeCW2) > maxTBSize) )
			{
				numScheduledRB--;
				checkSplSfAndCalcTbsizeSiso(isSpecialSubFrame,
						harqProcess_p->dlHARQTBOneInfo.mcsIndex,
						numScheduledRB,
						&tbSizeCW1);


				checkSplSfAndCalcTbsizeSiso(isSpecialSubFrame,
						harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
						numScheduledRB,
						&tbSizeCW2);
				/*tbSizeCW1 = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
				 *                   numScheduledRB);
				 * tbSizeCW2 = determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
				 *                                          numScheduledRB);*/
			}
		}
		else if ( harqProcess_p->isTBOneValid )
		{
			while( (numScheduledRB > 1) &&
					((tbSizeCW1 ) > maxTBSize) )
			{
				numScheduledRB--;
				checkSplSfAndCalcTbsizeSiso(isSpecialSubFrame,
						harqProcess_p->dlHARQTBOneInfo.mcsIndex,
						numScheduledRB,
						&tbSizeCW1);
				/* Cyclomatic_complexity_changes_end */

			}
			numScheduledRB++;
			if (numScheduledRB > cellSpecificParams_g.\
					cellConfigAndInitParams_p[internalCellIndex]\
					->cellParams_p->dlAvailableRBs)
			{
				numScheduledRB = cellSpecificParams_g.\
						 cellConfigAndInitParams_p[internalCellIndex]\
						 ->cellParams_p->dlAvailableRBs;
			}

		}
	}
	/* SPR 9123 fix end */
#endif   
	return numScheduledRB;
}

/*****************************************************************************
 * Function Name  : dlSpsUpdateStrategyNodes
 * Inputs         : internalCellIndex - cell index used at MAC,
 *                  currentGlobalTTITickCount -Current Global TTI Tick.
 * Outputs        : None
 * Returns        : None
 * Description    : This function Puts entry to PDCCH.
 *****************************************************************************/
 void dlSpsUpdateStrategyNodes( InternalCellIndex internalCellIndex, 
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount)
/* SPR 15909 fix end */
{
	UInt8 countDLUENewTx = 0;
	DLStrategyTxNode *dlStrategyTxNode_p = PNULL;
	DLUeScheduleInfo *dlUeScheduleInfo_p = PNULL;
	DLUEContext   *ueDLContext_p = PNULL;
    /*SPR 20899 Fix Start */
	UInt8 strategyContainerIndex = MODULO_TWO(getCurrentTick()); 
    /*SPR 20899 Fix Stop */

	while(countDLUENewTx < dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount)
	{
		/*putEntry of DLStrategyTXNode in DL Strategy Schedule Info */

		dlUeScheduleInfo_p = &(dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].dlUeScheduleInfo
				[countDLUENewTx]);
		ueDLContext_p = dlUeScheduleInfo_p->ueDLContext_p;
		if(PNULL != ueDLContext_p )
		{
			if ( SPS_SCHEDULING == ueDLContext_p->schType )
			{
				dlStrategyTxNode_p = dlUeScheduleInfo_p->dLStrategyTxNode_p;

				if( PNULL == dlStrategyTxNode_p )
				{
/* SPR 20636 Changes Start */
					LOG_MAC_MSG(MAC_SCHEDULED_UE_HAS_NULL_POINTER, LOGFATAL, 
							MAC_DL_Strategy, currentGlobalTTITickCount, 0, 
							(ADDR)dlStrategyTxNode_p, 0, __LINE__,0, 0, 0, __func__, "");
/* SPR 20636 Changes End */
				}
				else
				{
					/*SPR 6690 fix Start */
					dlStrategyTxNode_p->resourceAllocInput.requiredRB = dlUeScheduleInfo_p->harqProcess_p->assignedResourcesNB;
					/*SPR 6690 fix End */
					/*putEntry of DLStrategyTXNode in PDCCH Queue */
					if(!( putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex)))
					{
						LOG_MAC_MSG(MAC_PUSH_IN_QUEUE_FAILURE_IN_DL_LOG_ID, LOGFATAL, 
								MAC_DL_Strategy,
								currentGlobalTTITickCount,
								dlStrategyTxNode_p->ueIndex,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
								"DL Strategy Tx Queue");
						freeDLHarqProcess(ueDLContext_p,dlUeScheduleInfo_p->harqProcess_p,
								internalCellIndex);
						ueDLContext_p->dlStrategyTxNode_p = PNULL;

					}
				}    
			}
		}
		else
		{
/* SPR 20636 Changes Start */
			LOG_MAC_MSG(MAC_SCHEDULED_UE_HAS_NULL_POINTER, LOGFATAL, 
					MAC_DL_Strategy, currentGlobalTTITickCount, (ADDR)ueDLContext_p, 
					0, 0, __LINE__,0, 0, 0, __func__, "");
/* SPR 20636 Changes End */
		}
		countDLUENewTx++;
	}
}

/*****************************************************************************
 * Function Name  : dlQosUpdateStrategyNodes
 * Inputs         : internalCellIndex - cell index used at MAC,
 *                  currentGlobalTTITickCount -Current Global TTI Tick.
 * Outputs        : None
 * Returns        : None
 * Description    : This fucntion updates the msg type of the transmisson node
 *                  Updated teh Total number of RB's scheduled in transmission 
 *                  node.
 *****************************************************************************/
void dlQosUpdateStrategyNodes( InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount)
/* SPR 15909 fix end */
{
	/* 
	 *  1.  Traverse through the Scheduled UE nodes
	 *  2.  update the message type
	 *  3.  update the harq for the scheduled RB.s 
	 *  4.  validate for minimum TB size
	 */

	SInt8 countDLUENewTx = 0;
	DLHARQProcess *harqProcess_p = PNULL;
	DLUEContext   *ueDLContext_p = PNULL;
	DLStrategyTxNode *dlStrategyTxNode_p = PNULL;
	DLUeScheduleInfo *dlUeScheduleInfo_p = PNULL;
	DLStrategyTxNode **tempDlStrategyTxNode_p = PNULL;
	DLScheduleFlag* scheduleFlag_p = PNULL;
	/*SPR 20899 Fix Start */
	UInt8 strategyContainerIndex = MODULO_TWO(getCurrentTick()); 
    /*SPR 20899 Fix Stop */

	while(countDLUENewTx < dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount)
	{
		/*putEntry of DLStrategyTXNode in DL Strategy Schedule Info */

		dlUeScheduleInfo_p = &(dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].dlUeScheduleInfo
				[countDLUENewTx]);

		ueDLContext_p = dlUeScheduleInfo_p->ueDLContext_p;

		dlStrategyTxNode_p = dlUeScheduleInfo_p->dLStrategyTxNode_p;

		harqProcess_p = dlUeScheduleInfo_p->harqProcess_p;

		if((PNULL == ueDLContext_p) || 
				(PNULL == dlStrategyTxNode_p) ||
				(PNULL == harqProcess_p))
		{
			LOG_MAC_MSG(MAC_SCHEDULED_UE_HAS_NULL_POINTER, LOGFATAL, MAC_DL_Strategy,
					currentGlobalTTITickCount,
					(ADDR)ueDLContext_p, (ADDR)dlStrategyTxNode_p, 
					(ADDR)harqProcess_p, __LINE__,
					DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
					DEFAULT_FLOAT_VALUE, FUNCTION_NAME, 
					"");
			countDLUENewTx++;
			continue;
		}

        LOG_MAC_MSG(MAC_DL_STRATEGY_UE_ID, LOGBRIEF, MAC_DL_Strategy,
                currentGlobalTTITickCount,
                dlStrategyTxNode_p->ueIndex, dlStrategyTxNode_p->harqProcessId, 
                dlStrategyTxNode_p->transmissionType, dlStrategyTxNode_p->minTBSize,
                dlStrategyTxNode_p->maxTBSize,
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME, "");

		if( SPS_SCHEDULING == ueDLContext_p->schType )
		{
			countDLUENewTx++;
			continue;
		}

		if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
		{
			tempDlStrategyTxNode_p = &(ueDLContext_p->dlStrategyTxNode_p);
			scheduleFlag_p = &(ueDLContext_p->scheduleFlag);
		}
		else
		{
			tempDlStrategyTxNode_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);
			scheduleFlag_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag);
		}
		/*SPR 3679 QOS Changes Start*/
		/* SPR 4186 Changes Start */
		if (NEW_TX == dlStrategyTxNode_p->transmissionType)
			/* SPR 4186 Changes End */
		{
			/* Cyclomatic Complexity changes - starts here */
			dlQosUpdateHarqContext( dlStrategyTxNode_p, 
					ueDLContext_p, 
					harqProcess_p,
					internalCellIndex );
			/* Cyclomatic Complexity changes - ends here */
			/* Cyclomatic_complexity_changes_start */
			if(NON_SPATIAL_MUL == dlStrategyTxNode_p->msgType)
			{
				getMsgTypeTaDrxFlagNonSpatial(ueDLContext_p->scheduledLcCount,
						dlUeScheduleInfo_p->isTaDrxScheduled,
						dlStrategyTxNode_p,
                        /* +- SPR 17777 */
						harqProcess_p
#ifdef SSI_DEBUG_STATS
                        ,internalCellIndex
#endif
                        );
                        /* +- SPR 17777 */
			}
			else
			{
				getMsgTypeTaDrxFlagSpatial(ueDLContext_p->scheduledLcCount,
						dlUeScheduleInfo_p->isTaDrxScheduled,
						dlStrategyTxNode_p,
                        /* +- SPR 17777 */
						harqProcess_p);
                        /* +- SPR 17777 */

			}
			/* Cyclomatic_complexity_changes_end */
			if(!IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex)) 
			{
				/** SPR 12457 Fix : Start**/
				if(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus == NO_TB )
					/** SPR 12457 Fix : End **/
				{
					putEntryInSCellSchedulingQueue(
							ueDLContext_p,
							dlStrategyTxNode_p->harqProcess_p,
							internalCellIndex);
				}
			}
			if ( dlStrategyTxNode_p->maxTBSize )
			{
				/* SPR# 5189 Changes Starts */
				if((dlStrategyTxNode_p->transmissionType == RE_TX) && 
						(dlStrategyTxNode_p->msgType == SPATIAL_MUL))
				{
					/* + coverity 54141 */
					if (!(putEntryInDLSchedulerReTxQueue(dlStrategyTxNode_p,internalCellIndex)))
					{
						LOG_MAC_MSG(MAC_PUSH_IN_QUEUE_FAILURE_IN_DL_LOG_ID, LOGFATAL, 
								MAC_DL_Strategy,
								currentGlobalTTITickCount,
								dlStrategyTxNode_p->ueIndex,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
								"DL Strategy Re-Tx Queue");

						freeMemPool(dlStrategyTxNode_p);
						dlStrategyTxNode_p = PNULL;
						*tempDlStrategyTxNode_p = PNULL;
					}
					/* - coverity 54141 */
				}
				/* SPR# 5189 Changes Ends */

				else if (!(putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex)))
				{
					LOG_MAC_MSG(MAC_PUSH_IN_QUEUE_FAILURE_IN_DL_LOG_ID, LOGFATAL, 
							MAC_DL_Strategy,
							currentGlobalTTITickCount,
							dlStrategyTxNode_p->ueIndex,
							DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
							"DL Strategy Tx Queue");
					/*SPR 7925 Changes Start*/
					freeDLHarqProcess(ueDLContext_p,harqProcess_p,
							internalCellIndex);
					/* SPR 21036 start */
                                        /* freeMemPool(dlStrategyTxNode_p); */
                                        /* SPR 21036 end   */
					
					*tempDlStrategyTxNode_p = PNULL;
					*scheduleFlag_p = DL_QOS_NEW_TRANSMISSION;
				}
			}
			else
			{
				if((dlStrategyTxNode_p->transmissionType == RE_TX) && 
						(dlStrategyTxNode_p->msgType == SPATIAL_MUL))
				{
					/* - coverity 55030 */
					if (!(putEntryInDLSchedulerReTxQueue(dlStrategyTxNode_p, internalCellIndex)))
					{
						LOG_MAC_MSG(MAC_PUSH_IN_QUEUE_FAILURE_IN_DL_LOG_ID, LOGFATAL, 
								MAC_DL_Strategy,
								currentGlobalTTITickCount,
								dlStrategyTxNode_p->ueIndex,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
								"DL Strategy Re-Tx Queue");

						freeMemPool(dlStrategyTxNode_p);
						dlStrategyTxNode_p = PNULL;
						*tempDlStrategyTxNode_p = PNULL;
					}
					/* - coverity 55030 */
				}
				else
				{
					freeDLHarqProcess(ueDLContext_p,harqProcess_p,internalCellIndex);
					freeMemPool(dlStrategyTxNode_p);
					dlStrategyTxNode_p = PNULL;
					*tempDlStrategyTxNode_p = PNULL;
					*scheduleFlag_p = DL_QOS_NEW_TRANSMISSION;
				}
				/*SPR 7925 Changes End*/
			}

			/* +COVERITY 29855 */
#ifdef SSI_DEBUG_STATS
			if (dlStrategyTxNode_p)
			{  
				/* CA Stats Changes Start */
				LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED
					(ueDLContext_p, harqProcess_p, dlStrategyTxNode_p, internalCellIndex);
				/* CA Stats Changes End */
			}
			/* -COVERITY 29855 */
#endif
		}
		/*SPR 3679 QOS Changes End*/
		countDLUENewTx++;
	}
}

/*****************************************************************************
 * Function Name  : dlQosProcessSchedulerStrategy
 * Inputs         : currentGlobalTTITickCount - current Global Tick for which 
 *                                              processing need to be done,
 *                  dlDelay - Number of ticks DL is getting processed in 
 *                            Advance,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This processess the DL scheduling strategy for QOS based 
 *                  scheduling.
 *                  Strategy is to First process the triggers fron RLC and 
 *                  updates the LC strategy nodes in the Priority LC tree
 *                  Then process the DRX, measurement gap and Sync Loss 
 *                  Triggers.
 *                  Schedule the retransmissions, Process the TA queue and 
 *                  schedule them on priority Last but not least process the 
 *                  Priority LC traa and schedule the UE's.
 *****************************************************************************/
/* SPR 15909 fix start */
UInt32 dlQosProcessSchedulerStrategy( tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		UInt32 dlDelay,
		InternalCellIndex internalCellIndex)
{
    /*SPR 21948 Changes Start */
    /* Changes under spr 21762 have been reverted */
    /*SPR 21948 Changes End */
	UInt8  numDlUEsToBeScheduled[MAX_NUM_CELL] = {0};
    /*+- SPR 20897  */
    /** SPR 21714 changes start **/
    UInt8 isNewTraverse = LTE_FALSE;
    /** SPR 21714 changes ends **/
    /*+- SPR 20897 */
	
	SInt32 numberOfAvailableRBs[MAX_NUM_CELL][MAX_USER_LOCATION] = {{0},{0}};

	/* SPR 13078 Changes Start */
	UInt8  numTAToSchdule[MAX_NUM_CELL] = {NUM_OF_TA_SCHEDULED_PER_TTI, NUM_OF_TA_SCHEDULED_PER_TTI};
	/* SPR 13078 Changes End */
	UInt32 totalNumberOfAvailableRBs    = 0; 
	UInt32 totalRbMIMO                  = 0;
	InternalCellIndex cellIndex         = 0;
	/** SPR 13599 Changes Start **/
	UInt8 isCaEnable = IS_CA_ENABLED();
	/** SPR 13599 Changes End **/

	/* EICIC +*/
	UInt32 sysFrameNum 			= 0;
	UInt32 subFrameNum 			= 0;
	UInt8  absSubFrameNum 		= 0;
	UInt8  strategyScanCount		= 0;
	InternalCellIndex cellIndexTemp = 0;
	UInt8 numBitsAbsPattern = 0;
	UInt32 totalAvailableRbLeft = 0;
	/*EICIC -*/

#ifdef SSI_DEBUG_STATS
	UInt16 writeTTIIndex = 0;
	/* CA Stats Changes Start */
	writeTTIIndex = gMacStats.pStats->writeTTIIndex[internalCellIndex];
	gMacStats.pStats->dlStrategyTTIStats[internalCellIndex][\
		writeTTIIndex].scheduledReTXUE = 0;
	gMacStats.pStats->dlStrategyTTIStats[internalCellIndex][\
		writeTTIIndex].scheduledTAUE = 0;
	gMacStats.pStats->dlStrategyTTIStats[internalCellIndex][\
		writeTTIIndex].scheduledTxUE = 0;
	gMacStats.pStats->dlStrategyTTIStats[internalCellIndex][\
		writeTTIIndex].scheduledUETotal = 0;
	gMacStats.pStats->dlStrategyLcTTIStats[internalCellIndex][\
		writeTTIIndex].numScheduledLC = 0;
	/* CA Stats Changes End */
#endif



	currentGlobalTTITickCount = currentGlobalTTITickCount + dlDelay;

	/************ Update All Cells Info if CA Enabled ************/
	/** SPR 13599 Changes Start**/
	if(isCaEnable)
		/** SPR 13599 Changes End **/
	{
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
		{
			if (CELL_PHY_RUNNING == cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->cellState)
			{
				dlQosInitStrategyParams( numberOfAvailableRBs,
						numDlUEsToBeScheduled,
						cellIndex);
			}
		}
	}
	else
	{
		/************ Update Current Cell Info only ************/
		dlQosInitStrategyParams( numberOfAvailableRBs,
				numDlUEsToBeScheduled,
				internalCellIndex);
	}
	/* EICIC +*/
	/* EICIC Optimization +- */
    if (isEicicEnabled_g[internalCellIndex]) 
	{     
		getSFAndSFN(&sysFrameNum, &subFrameNum,internalCellIndex);
		/*Calculate the SFN and SF by considering the delay to PHY*/
#ifdef TDD_CONFIG
		subFrameNum = subFrameNum + 1;
		phyDelay_g =  1; 
#else
		subFrameNum = subFrameNum + dlDelay;
		phyDelay_g = dlDelay;
#endif 

		if(subFrameNum >= MAX_SUB_FRAME)
		{
			subFrameNum -= MAX_SUB_FRAME;
			sysFrameNum++;
			if (sysFrameNum == MAX_SFN_VALUE)
			{
				sysFrameNum = 0;
			}
		}
		/*Complexity Start*/
		numBitsAbsPattern=eicicFindNumOfBitsinABSPattern(internalCellIndex);
		/*Complexity End*/

		absSubFrameNum = ((((sysFrameNum )*MAX_SUB_FRAME) +(subFrameNum)) %numBitsAbsPattern) ;

	/*Complexity Start*/
	eicicUpdateUEtypeToSchedule(internalCellIndex,absSubFrameNum,numDlUEsToBeScheduled);
	/*Complexity End*/
	}
    else
    {
                ueTypeToScheduleDL_g[internalCellIndex] = NON_VICTIM_UE;
    }     
	/* EICIC Optimization +- */

	/* EICIC -*/
	do /* EICIC */
	{
#ifdef OVERSHOOT_CORRECTION
		modifiedNoOfUEsAfterOverLoadDL( &numDlUEsToBeScheduled[internalCellIndex],
				internalCellIndex);

#endif

		/* CA Stats Changes Start */
		LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_ABSOLUTE_TTI(currentGlobalTTITickCount, 
				internalCellIndex);
		/* CA Stats Changes End */

		/** SPR 13599 Changes Start **/
		if(isCaEnable) 
			/** SPR 13599 Changes End **/
		{
			/* To be discussed: Currently below func is called for all cells since
			 * no Pre startegy EL is there to be called. 
			 * This would be modify later */
			for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
			{
				if (CELL_PHY_RUNNING == cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->cellState)
				{
					dlQosPreStrategyExecution( cellIndex,
							currentGlobalTTITickCount,
							/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
							dlDelay, 
#endif
							/* - SPS_TDD_Changes */
							numberOfAvailableRBs,
							numDlUEsToBeScheduled);
				}
			}
		}
		else
		{
			dlQosPreStrategyExecution( internalCellIndex,
					currentGlobalTTITickCount,
					/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
					dlDelay, 
#endif
					/* - SPS_TDD_Changes */
					numberOfAvailableRBs,
					numDlUEsToBeScheduled);
		}

		dlQosStrategyProcessDLReTxFailQueues( 
				internalCellIndex,
				numDlUEsToBeScheduled,
				numberOfAvailableRBs,
                /* +- SPR 17777 */
				currentGlobalTTITickCount
#ifdef FDD_CONFIG
				,dlDelay
#endif
                );

		dlQosStrategyProcessDLReTxQueues( 
				internalCellIndex,
				numDlUEsToBeScheduled,
				numberOfAvailableRBs,
                /* +- SPR 17777 */
				currentGlobalTTITickCount
#ifdef FDD_CONFIG
                ,dlDelay
#endif
                );
        /* +- SPR 17777 */

		/* CAUTION: Currently the below sequence is followed for 
		 * scheduling Control elements: 
		 *  - All TA  queues
		 *  - SCELL CE queues
		 *  - DRX queues
		 *  - Explicit Tpc queues
		 *
		 *  On changing the sequence, need to take care while 
		 *  scheduling SCELL CE 
		 */


		/*
		 *   Process the pending TA queue and 
		 *   add the node in the strategy UE transmission list
		 */
        /* +- SPR 20897  */
        if(isCaEnable)
        {
			for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
            {
/*SPR 22919 Start*/
                if(numDlUEsToBeScheduled[cellIndex] > 0 || totalRbMIMO_g[cellIndex])
/*SPR 22919 End*/
                {
		    /** SPR 21714 changes start **/
                    isNewTraverse = LTE_TRUE;
		    /** SPR 21714 changes ends **/
                    break;
                }
            }
        }
        else
        {
	    /** SPR 21714 changes start **/
/*SPR 22919 Start*/
            if(numDlUEsToBeScheduled[internalCellIndex] || totalRbMIMO_g[internalCellIndex]) 
                    isNewTraverse = LTE_TRUE;
/*SPR 22919 End*/
	    /** SPR 21714 changes start **/
        }
	/** SPR 21714 changes start **/ 
	/** To handle the case of Single TB NACK and there is a possibility of new data transmission on other TB. **/
/*SPR 22919 +-*/
	/** SPR 21714 changes ends **/

        if(isNewTraverse)
        {
            /*+- SPR 20897 */
		dlQosStrategyProcessTAQueueOfAllCells( 
				numberOfAvailableRBs,
				currentGlobalTTITickCount, 
				pdcchPdschFailureTAQueue_gp,
				numTAToSchdule, 
				numDlUEsToBeScheduled, 
				PENDING_TA_FLAG,
				internalCellIndex);
		/*
		 *   Process the pending TA queue and 
		 *   add the node in the strategy UE transmission list
		 */
		dlQosStrategyProcessTAQueueOfAllCells( 
				numberOfAvailableRBs,
				currentGlobalTTITickCount, 
				pendingTAQueue_gp,
				numTAToSchdule,
				numDlUEsToBeScheduled, 
				PENDING_TA_FLAG,
				internalCellIndex);
		/*
		 *   Process the TA queue and 
		 *   add the node in the strategy UE transmission list
		 */
		dlQosStrategyProcessTAQueueOfAllCells( 
				numberOfAvailableRBs,
				currentGlobalTTITickCount,
				dlTimeAdvanceQueue_gp, 
				numTAToSchdule, 
				numDlUEsToBeScheduled, 
				NEW_TA_FLAG,
				internalCellIndex);

		/** SPR 13599 Changes Start**/
		if(isCaEnable)
			/** SPR 13599 Changes End **/
		{
			/* Process pending CA MAC CE queue*/ 
			dlQosStrategyProcessDlCAMacCEQueueofAllCells(
					numberOfAvailableRBs,
					currentGlobalTTITickCount,
					pendingFailureDlCAMacCEQueue_gp,
					numDlUEsToBeScheduled,
                    /* +- SPR 17777 */
					PENDING_SCELL_CE_FLAG);
                    /* +- SPR 17777 */

			/* Process CA MAC CE queue*/ 
			dlQosStrategyProcessDlCAMacCEQueueofAllCells(
					numberOfAvailableRBs,
					currentGlobalTTITickCount,
					dlCAMacCEQueue_gp,
					numDlUEsToBeScheduled,
                    /* +- SPR 17777 */
					NEW_SCELL_CE_FLAG);
                    /* +- SPR 17777 */
		}
	        /* + SPR 22243 Fix*/
        	/* Process the PDCCH failure DRX queue */
		dlQosStrategyProcessDRXQueueOfAllCells( 
				numberOfAvailableRBs,
				currentGlobalTTITickCount,
				pdcchPdschFailureDRXQueue_gp, 
				numDlUEsToBeScheduled,
				internalCellIndex);
	        /* - SPR 22243 Fix*/

		/* Process the pending DRX queue */
		dlQosStrategyProcessDRXQueueOfAllCells( 
				numberOfAvailableRBs,
				currentGlobalTTITickCount,
				pendingDRXQueue_gp, 
				numDlUEsToBeScheduled,
				internalCellIndex);

		/* Process the DRX queue */
		dlQosStrategyProcessDRXQueueOfAllCells( 
				numberOfAvailableRBs,
				currentGlobalTTITickCount,
				dlDRXQueue_gp, 
				numDlUEsToBeScheduled,
				internalCellIndex);

		/* Process Explicit DLDCI Queues only if the powerControlEnable is 
		 * true for any of the HARQ or CQI*/
		dlQosStrategyProcessExplicitDCIDLQueueOfAllCells( 
				numberOfAvailableRBs,
				currentGlobalTTITickCount,
				explicitDLPucchTpcScheduleQueue_gp, 
				numDlUEsToBeScheduled,
				internalCellIndex);

		/* Get number of RBs & TotalRBMIMO available in (All) Serving Cell(s) */
		/** SPR 13599 Changes Start **/
		if(!isCaEnable)
			/** SPR 13599 Changes End **/
		{
			totalNumberOfAvailableRBs = numberOfAvailableRBs[internalCellIndex][CC_CE_REGION];
			totalRbMIMO = totalRbMIMO_g[internalCellIndex];
		}
		else
		{
			for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
			{
				if (CELL_PHY_RUNNING == cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->cellState)
				{
					totalNumberOfAvailableRBs += numberOfAvailableRBs[cellIndex][CC_CE_REGION];
					totalRbMIMO += totalRbMIMO_g[cellIndex];
				}
			}
		}

		/*
		 *   Process the priority lc list from the TREE and schedule the UE
		 */
		if((0 < totalNumberOfAvailableRBs) || (0 < totalRbMIMO))
		{
			dlQosStrategyMakeScheduleUeQueue( numberOfAvailableRBs,  
					currentGlobalTTITickCount,  
					numDlUEsToBeScheduled, 
                    /*+ SPR 22819*/
                    /*- SPR 22819*/
					internalCellIndex);
		}
		/* To be discussed: Currently below func is called for all cells since
		 * no Post startegy EL is there to be called. 
		 * This would be modify later */
            /*+- SPR 20897  */
        } /*end of isNewTraverse*/
        /*+- SPR 20897 */
		if(isCaEnable)
		{
			for(cellIndexTemp = 0; cellIndexTemp < cellSpecificParams_g.numOfCells; cellIndexTemp++)
			{
				if (CELL_PHY_RUNNING == cellSpecificParams_g.cellConfigAndInitParams_p[cellIndexTemp]->cellState)
				{
					dlQosResetPreferredCellList(cellIndexTemp);
				}
			}
		}
		else
		{
			dlQosResetPreferredCellList(internalCellIndex);

		}



		strategyScanCount = 0;

		if(!isCaEnable) 
		{
			numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] = numRBsAvailableForScheduling_g[internalCellIndex][CC_CE_REGION] - cellSpecificParams_g.\
										cellConfigAndInitParams_p[internalCellIndex]->\
										initParams_p->dlSchdConfig.strategyBufferRB;
		}
		else
		{
			for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
			{

				numberOfAvailableRBs[cellIndex][CC_CE_REGION] = numRBsAvailableForScheduling_g[cellIndex][CC_CE_REGION] - cellSpecificParams_g.\
										cellConfigAndInitParams_p[cellIndex]->\
										initParams_p->dlSchdConfig.strategyBufferRB;
				totalAvailableRbLeft += numberOfAvailableRBs[cellIndex][CC_CE_REGION] ;

			}
		}
        /* EICIC Optimization +- */
        if (isEicicEnabled_g[internalCellIndex]) 
		{
		if(!isCaEnable) 
		{
			if (( numberOfAvailableRBs[internalCellIndex][CC_CE_REGION]) && (numDlUEsToBeScheduled[internalCellIndex]) && (VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex])
					&& (redistributeRb_g) )
			{
				if (!strategyScanCount)
				{
					strategyScanCount = 1;
					ueTypeToScheduleDL_g[internalCellIndex] = NON_VICTIM_UE;

				}
			}
		}
		else
		{
			for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
			{

				if (( totalAvailableRbLeft) && (numDlUEsToBeScheduled[cellIndex]) && (VICTIM_UE == ueTypeToScheduleDL_g[cellIndex])
						&& (redistributeRb_g) )
				{
					if (!strategyScanCount)
					{
						strategyScanCount = 1;
						break;

					}
				}

			}
			if (strategyScanCount)
			{
				for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
				{
					ueTypeToScheduleDL_g[cellIndex] =NON_VICTIM_UE;
				}
			}

		}
              }
              /* EICIC Optimization +- */
		/* EICIC -*/

	}while (strategyScanCount);

	dlQosPostStrategyExecution( internalCellIndex,
			currentGlobalTTITickCount);
	return MAC_SUCCESS;
}

/************************************************************************************************
 * Function Name  : dlQosProcessZeroNonZeroTriggerQueue
 * Inputs         : ZeroNonZeroTriggerQueueCount - Total Count of the 
 *                                                 HARQZeroNonZeroTriggerQueue or 
 *                                                 dlUEZeroNonZeroQueueLoadTriggerQueue,
 *                  internalCellIndex - cell index used at MAC,
 *                  currentGlobalTTITickCount -Current Global TTI Tick.
 * Outputs        : updates the Non Zero Queue Load List
 * Returns        : None
 * Description    : It the Process the HARQZeroNonZeroQueueLoadTrigger Queue and 
 Update the NonZeroQueueLoadAndHarq List,after adding and 
 deleting UEs as per the request in the Triggers.
 ***********************************************************************************************/
void dlQosProcessZeroNonZeroTriggerQueue(
		SInt16     zeroNonZeroTriggerQueueCount,
		/* CirQ changes start */
        /* +- SPR 17777 */
		InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount)
/* SPR 15909 fix end */
/* CirQ changes end */
{
	ZeroNonZeroTriggerNode * node_p        = PNULL;
	DLUEContext* ueDLContext_p             = PNULL;  
	UInt32 ueIndex;
	DLUEContextInfo * ueDLContextInfo_p    = PNULL;


	/*Processed for the number of  Queue Count*/
	while(zeroNonZeroTriggerQueueCount)
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
				ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

				if ((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))
				{              
					if(!ENQUEUE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(ZeroNonZeroTriggerNode, 
								&(node_p->ZeroNonZeroTriggerNodeAnchor), internalCellIndex))
					{
						FREE_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p);
					}
					zeroNonZeroTriggerQueueCount--;   
					continue;
				}

				else if ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType))
				{           
					if(!ENQUEUE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(ZeroNonZeroTriggerNode, 
								&(node_p->ZeroNonZeroTriggerNodeAnchor), internalCellIndex))
					{
						FREE_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p);
					}
					zeroNonZeroTriggerQueueCount--;   
					continue;
				}
				/* EICIC - */


				dlQosProcessUeForPrioritisedLcList( ueDLContext_p, 
						node_p,
						internalCellIndex,
						currentGlobalTTITickCount);
			}
			FREE_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p);
		}
		else
		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,LOGDEBUG,
					MAC_DL_Strategy,
					currentGlobalTTITickCount,
					zeroNonZeroTriggerQueueCount,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");
			break;
		}
		/*Free the memory for this node_p.*/
		zeroNonZeroTriggerQueueCount--;   
	}
}


/*****************************************************************************
 * Function Name  : dlQosProcessUeForPrioritisedLcList
 * Inputs         : ueDLContext_p - pointer to DLUEContext,
 *                  node_p - Pointer to ZeroNonZeroTriggerNode,
 *                  internalCellIndex - cell index used at MAC,
 *                  currentGlobalTTITickCount -Current Global TTI Tick.
 * Outputs        : updates the Non Zero Queue Load List
 * Returns        : None
 * Description    : It the Process the Add/Delete Trigger of the UE
 *                  Add/Delete the strategy LC nodes into/from the Priority Lc 
 *                  Tree as per the request in the Triggers.
 *****************************************************************************/
void dlQosProcessUeForPrioritisedLcList(
		DLUEContext *ueDLContext_p,
		ZeroNonZeroTriggerNode * node_p
		,InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount)
/* SPR 15909 fix end */
{
	UInt8 lcCount = 0;
	/* SPR# 3169 Changes Starts*/
	UInt8 lcId = 0;
	/* SPR# 3169 Changes Ends*/
	DLLogicalChannelInfo *lcInfo_p = PNULL;
    /* SPR 16916 FIX START */
    /* +- */
    /* SPR 16916 FIX END */

	/*
	   1.  For each LC present in the eligible LC List in the UE trigger node
	   2.  Check the Trigger
	   a.  If trigger for Add
	   i.  Add the node in the Eligible List
	   ii. Calculate the weight of the queueLoad and total weight of the 
	   LC node dlQosCalcLCWeight
	   iii.Call dlQosPutEntryInPrioritisedLcList() to add node in LC list
	   iv.     set the Flag for NON_ZERO_QLOAD_IN_ACTIVE_LIST
	   b.  Else if trigger for Delete
	   i.  delete the node from the Eligible LC list
	   ii. Call dlQosPutEntryInPrioritisedLcList() to delete the node 
	   from the LC list
	   iii. set the Flag for ZERO_QLOAD_NOT_IN_ACTIVE_LIST
	 */

	while(lcCount <  node_p->triggerLcCount)
	{
		/* SPR# 3169 Changes Starts*/
		lcId = ueDLContext_p->logicalChannel[node_p->qosLcTrigger[lcCount].lcId].logicalChannelId;
		if(lcId != INVALID_LCID ) 
		{
			/* SPR# 3169 Changes Ends*/

			lcInfo_p = &ueDLContext_p->logicalChannel[node_p->qosLcTrigger[lcCount].lcId];
			/*Trigger Node have flag to add the UE in Non Zero List*/
			if (ADD_TRIGGER == node_p->qosLcTrigger[lcCount].addDeleteTrigger )
			{

                /* SPR 16916 FIX START */
                /* +- */
                /* SPR 16916 FIX END */

				/* Add the node in the Eligible List */
				if(lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->isPresentInEligibleList == FALSE)
				{
					listInsertNode(&(ueDLContext_p->dlEligibleLcList),
							&(lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->eligibleLcAnchor));

					lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->isPresentInEligibleList = TRUE;
				}

				/* To be discussed: 
				 * Verifying whether the LC Node is already present in 
				 * Priority Tree ,if Already present, No Need to add LC in Tree 
				 * if (lcInfo_p->isLcNodeInTree)
				 */

				/* Check for the UE State whether it is in DRX-ON, MEAS_GAP-ON or 
				 * PDCCH_ORDER-ON. If yes then , do not add into the tree 
				 */
				if( (DL_QOS_NO_TRANSMISSION < ueDLContext_p->scheduleFlag) ||
                        /*SPR 21948 Changes Start */
                        /* Changes under spr 21762 have been reverted */
						( (isUeEligibleForCaScheduling(ueDLContext_p)) &&
                        /*SPR 21948 Changes End */
						  (DL_QOS_NO_TRANSMISSION < ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag)))
				{
					/* SPR# 10042 fix start */
					if (ueDLContext_p->dlMIMOInfo.dlRi == 1)
					{
                        /* SPR 18122 Changes Start*/
						ueDLContext_p->mcsPriority =
                            dlQosGetMcsPriority((DIVIDE_BY_TWO(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne)));
                        /* SPR 18122 Changes End*/
					}
					else
					{
                        /* SPR 18122 Changes Start*/
						ueDLContext_p->mcsPriority =
                            dlQosGetMcsPriority(((DIVIDE_BY_TWO(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne+ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo))));
                        /* SPR 18122 Changes End*/
					}

					/* SPR# 10042 fix end */
					if(lcInfo_p->dlLcQosInfo.channelType != SRB)
					{
						/* Calculate Token Priority */
						lcInfo_p->dlLcQosInfo.tokenPriority =
							dlQosGetTknPriority(
									lcInfo_p->dlLcQosInfo.scheduledToken[lcInfo_p->dlLcQosInfo.curTknWindowInd],
									lcInfo_p->dlLcQosInfo.lcTknTable,
									lcInfo_p->dlLcQosInfo.totSchdTknInWindow);
					}
					dlQosCalcLCWeightAndInsert(lcInfo_p,ueDLContext_p->mcsPriority,
							ZERO_DRX_DELAY,internalCellIndex);
#ifdef UTFWK_SIMULATION
					gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueDLContext_p->ueIndex].qoSLCPerTTIStat[lcInfo_p->logicalChannelId].STX[((GET_CURRENT_TICK_FOR_CELL(internalCellIndex))%MAX_UTFWK_TICK)].LCWeight = lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->totLCWeight;
#endif
					/* Distribute the AMBR tokens among the Non-GBR LC's */
					dlQosUpdateNonGbrLCTknValue(ueDLContext_p);                
					/* + SPR 11933 Changes */
					/* Distribute total available tokens among both serving cells
					 * only if UE's Scell is in Active state */
                    if( (IS_CA_ENABLED()) &&
                            /*SPR 21948 Changes Start */
                            /* Changes under spr 21762 have been reverted */
                            (isUeEligibleForCaScheduling(ueDLContext_p)))
                        /*SPR 21948 Changes End */
                    {
						lteMacDLQoSDistributeTokenAmongCells(ueDLContext_p);
					}
					/* - SPR 11933 Changes */
				}
				else
				{
					LOG_UT(MAC_LC_NODE_ALREADY_DELETED_FROM_TREE, LOGDEBUG,
							MAC_DL_Strategy,
							currentGlobalTTITickCount,
							ueDLContext_p->ueIndex,
							DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE,
							DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,
							FUNCTION_NAME,"");
				}

				LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ZERO_NONZERO_COUNT(ueDLContext_p->ueIndex);
				LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ELIGIBLE_LC_COUNT(ueDLContext_p->ueIndex);
			}
			/*Trigger Node have flag is to delete the UE from Non Zero List*/
			else
			{

                /* SPR 16916 FIX START */
                /* code removed */
                /* Now, when it has been taken care at encoder, that when current qload 
                 * becomes zero, we set the flag zeroQloadFlag for the corresponding qci
                 * and harqprocess.Later, when Harq Ack/Nack is received for that packet,
                 * the activeUECount will be decreased, as per spec TS 36.314, section 4.1.3.1
                 */
                /* SPR 16916 FIX END */

				/* SPR# 3169 Changes Starts */
				if(lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->isPresentInEligibleList)
				{
					/* SPR# 3169 Changes Ends */
					/* Delete the node from the Eligible List */
					listDeleteNode(&(ueDLContext_p->dlEligibleLcList),
							&(lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->eligibleLcAnchor));

					lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->isPresentInEligibleList = FALSE;

					/* To be discussed:
					 * Verifying whether the UE is present in the Non Zero List,
					 * if UE is not Present in the Non Zero List, then it cannot
					 * be deleted from the non Zero List
					 * if (lcInfo_p->isLcNodeInTree)
					 */

					/* Check for the UE State whether it is in DRX-ON, MEAS_GAP-ON or 
					 * PDCCH_ORDER-ON. If yes then , do not add into the tree 
					 */
                    if( (DL_QOS_NO_TRANSMISSION < ueDLContext_p->scheduleFlag)||
                            /*SPR 21948 Changes Start */
                            /* Changes under spr 21762 have been reverted */
                            ( (isUeEligibleForCaScheduling(ueDLContext_p)) &&
                              /*SPR 21948 Changes End */
							  (DL_QOS_NO_TRANSMISSION < ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag)))
					{ 

						/* Trigger may be due to zero token so only
						 * non-zero queueLoad check is not valid for 
						 * qos trigger
						 */
						dlQosDeleteEntryFromPrioritisedLcTree(
								lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p, 
								internalCellIndex,
								currentGlobalTTITickCount);
					}
					LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_NONZERO_ZERO_COUNT(ueDLContext_p->ueIndex);
					LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ELIGIBLE_LC_COUNT(ueDLContext_p->ueIndex);
				}
			}
		}
		/* SPR# 3799 Changes Starts */
		lcCount++;
		/* SPR# 3799 Changes Ends */
	}
	/*+ Dyn UEs To Be SchPerTTI +*/
	if(listCount(&(ueDLContext_p->dlEligibleLcList)) > 0)
	{
		if (!ueDLContext_p->isUEConsideredForTotalActiveUeDL)
		{
			totalActiveUEsDL_g[internalCellIndex]++;
			ueDLContext_p->isUEConsideredForTotalActiveUeDL = TRUE;
		}
	}
	else
	{
		if(ueDLContext_p->isUEConsideredForTotalActiveUeDL)
		{
			totalActiveUEsDL_g[internalCellIndex]--;
			ueDLContext_p->isUEConsideredForTotalActiveUeDL = FALSE;
		}
	}
	/*+ Dyn UEs To Be SchPerTTI +*/
}

/*********************************************************************************
 * Function Name  : dlQosStrategyProcessTAQueueForNonCAUes
 * Inputs         : timeAdvanceQueueCount - This is Count of the total nodes
 *                                          present in the dlTimeAdvanceQueue_g
 *                                          or pendingTAQueue,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  tempTimeAdvanceQueue_p - Pointer to Time Advance Queue
 *                                           Either New or Pending TA Queues,
 *                  numTAToSchedule_p - Number of TA to Schedule,
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled,
 *                  flagForTADRXPendingFresh - flag for Pending TA DRX,
 *                  internalCellIndex - cell index used at MAC,
 * Outputs        : It will allocate RBs to PendingTA/FreshTAQueue Nodes &
 place the DL Strategy O/P either in the PDCCHNewTXQueue_g
 or PDCHHReTXQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the TA Queues Nodes
 fresh and Pending both. Allocate RBs to the UEs with non
 Zero Queue Load otherwise add then in scheduler Array.
 *********************************************************************************/
/*SPR 22919 Start*/
 void dlQosStrategyProcessTAQueueForNonCAUes( UInt32 timeAdvanceQueueCount, 
/*SPR 22919 End*/
		SInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		TA_Q_TYPE * tempTimeAdvanceQueue_p,
		UInt8   *numTAToSchedule_p,
		UInt8   *numDlUEsToBeScheduled_p,
		/*SPR 6333 Fix Start*/
		FlagForPendingTADRX flagForTADRXPendingFresh,
		InternalCellIndex internalCellIndex)
/*SPR 6333 Fix End*/
{
	SInt8 numDlUEsToBeScheduled = 0;
	SInt8 numDlUEsStarted = 0;
	UInt16 ueIndex = 0; 
	TimeAdvanceQueueNode *taNode_p      = PNULL;
	DLUEContext *ueDLContext_p          = PNULL;
	DLUEContextInfo * ueDLContextInfo_p = PNULL;
	DLStrategyTxNode * dlStrategyTxNode_p = PNULL;
	/* ICIC Change start */
	UInt8 userLocationType = CC_USER;
	/* ICIC change End */
	LP_TimeAdvanceQueueNode tempTaNode_p  = PNULL; 

	/* + SPR 22243 Fix*/
    	MsgType msgType = INVALID_MSG;
    	/* - SPR 22243 Fix*/
	/* get total number of TA that can be scheduled */
	numDlUEsToBeScheduled = (*numTAToSchedule_p < *numDlUEsToBeScheduled_p) ?
		*numTAToSchedule_p : *numDlUEsToBeScheduled_p;

	numDlUEsToBeScheduled = ((SInt16)timeAdvanceQueueCount < numDlUEsToBeScheduled) ?
		(SInt16)timeAdvanceQueueCount : numDlUEsToBeScheduled;

	numDlUEsStarted = numDlUEsToBeScheduled;

	/* ICIC Changes starts    */ 
	while ((numDlUEsToBeScheduled)&&(numberOfAvailableRBs_p[CC_CE_REGION] &&
				(timeAdvanceQueueCount)))
		/* ICIC Changes Ends    */
	{
		/* +COVERITY 26439 */
		if (MAC_SUCCESS != DEQUEUE_TA_Q(tempTimeAdvanceQueue_p,TimeAdvanceQueueNode,
					(void **)&(taNode_p)))
		{
			taNode_p = PNULL;
		}
		/* -COVERITY 26439 */

		if (taNode_p)
		{
			ueIndex = taNode_p->ueIdx;    
			ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
			if (!(ueDLContextInfo_p->pendingDeleteFlag))
			{
				/* Check for pending TA Ack/Nack removed along with SPR_1346 */
				ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
				/* EICIC +*/
				/*Complexity Start*/
				if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))||
						((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{              
					if(!ENQUEUE_TA_Q(tempTimeAdvanceQueue_p,TimeAdvanceQueueNode,&(taNode_p->timeAdvanceQueueNodeAnchor)))
					{
						tempTaNode_p = taNode_p;
						FREE_MEM_NODE_TA_Q(tempTaNode_p);
					}
					timeAdvanceQueueCount--;
					continue;
				}

				/* EICIC -*/
				/*Complexity End*/	
				/*Checking if Already being considered for RB Distribution*/
				if (DL_QOS_ALREADY_TRANSMITED < ueDLContext_p->scheduleFlag) 
				{

					/* ICIC changes start */
					userLocationType = ueDLContext_p->userLocationType;
					if ((!numberOfAvailableRBs_p[userLocationType])
							|| ((!maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] && CE_USER == userLocationType)))

					{
						LOG_MAC_MSG(MAC_RB_NOT_AVAILABLE_FOR_RETX,LOGFATAL,MAC_DL_Strategy,
								currentGlobalTTITickCount,
								ueIndex,DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "");
						/*SPR 6333 Fix Start*/
						/* Cyclomatic_complexity_changes_start */
						processPendingTAQueue( taNode_p, flagForTADRXPendingFresh,
								internalCellIndex, ueDLContext_p);
						/* Cyclomatic_complexity_changes_end */


						/*SPR 6333 Fix End*/
						timeAdvanceQueueCount--;
						continue;
					}

					/* ICIC Change End */

					/* + SPR 9653 Changes */ 
					/* Processing of UE when Queue Load is Zero but 
					   nack Queue Load iS non Zero*/
		                        /* + SPR 22352 fix */
                     
                   			 /* As the processing of TA queue is done first, so we need to check if
			                  * DRX or Scell Act/Deact or both are also to be scheduled in this tti, 
                      			  * then change the msgType accordingly. As the scheduleFlag in Ue context 
			                  * is set as DL_QOS_ALREADY_TRANSMITED below.So, in this same tick, 
                      			  * DRX node  or scell node will not be processed and will be freed in 
                    			  * while processing of respective queue*/

			                checkMsgTypeTaDrxScellActDeact(ueDLContext_p,TA_MSG,&msgType,
									ueDLContext_p->internalCellIndex);
					
					dlStrategyTxNode_p = 
						/*QOS_CYCLIC_COMPLEXITY_CHG*/
						dlQosStrategyUpdateNewTxUEList(ueDLContext_p,msgType,NEW_TRAVERSAL,
								/* + TM7_8 Changes Start */
								currentGlobalTTITickCount,
								internalCellIndex);
                     			/* - SPR 22352 fix */
					/* - TM7_8 Changes End */
					/*QOS_CYCLIC_COMPLEXITY_CHG*/
					/* - SPR 9653 Changes */ 
					/* ueDLContext_p->isAlreadyConsideredFlag = dlReferenceFlag;*/
					if(PNULL != dlStrategyTxNode_p)
					{
						ueDLContext_p->dlStrategyTxNode_p = dlStrategyTxNode_p;
						ueDLContext_p->scheduleFlag = DL_QOS_ALREADY_TRANSMITED;
						/* + SPR 22352 fix */ 
						updateRbAndTBSizeInDlStrategyNode(msgType,dlStrategyTxNode_p,
								numberOfAvailableRBs_p,userLocationType,ueIndex);
						/* - SPR 22352 fix */ 
					}
					else
					{
						/* If there are no FREE HARQ Process to schedule UE
						 * get the next node and continue 
						 */
						LOG_MAC_MSG(MAC_ALL_HARQ_ARE_BUSY,LOGFATAL,MAC_DL_Strategy,
								currentGlobalTTITickCount,
								ueIndex,DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "No FREE HARQ Available");

						/* SPR 5817 Start */
						/*SPR 6333 Fix Start*/
						/* Cyclomatic_complexity_changes_start */
						processPendingTAQueue( taNode_p, flagForTADRXPendingFresh,
								internalCellIndex, ueDLContext_p);	
						/* Cyclomatic_complexity_changes_end */    
						/*SPR 6333 Fix Start*/
						/* SPR 5817 End */

						timeAdvanceQueueCount--;
						continue;
					}


				}/*end of already considered flag check, The UE which are 
				   already considered will be deleted automatically */
				else if(DL_QOS_ALREADY_TRANSMITED != ueDLContext_p->scheduleFlag)
				{
					/*SPR 6333 Fix Start*/
					/* Cyclomatic_complexity_changes_start */
					processPendingTAQueue( taNode_p, flagForTADRXPendingFresh,
							internalCellIndex, ueDLContext_p);
					/*SPR 6333 Fix Start*/

					/* Cyclomatic_complexity_changes_end */
					timeAdvanceQueueCount--;
					continue;
				}
				else /* scheduleFlag == DL_QOS_ALREADY_TRANSMITED */
				{
					/*! \code
					 * If this UE is for SPS in this tick and 
					 *   spsOpp - (headPktSize + Header size ) >= LENGTH_OF_TA
					 *      - Then send TA for this UE. 
					 * Else skip this UE for next tick.
					 * \endcode
					 * */
					if( SPS_SCHEDULING == ueDLContext_p->schType )
					{
						{
							/*SPR 6333 Fix Start*/
							/* Cyclomatic_complexity_changes_start */
							processPendingTAQueue( taNode_p, flagForTADRXPendingFresh,
									internalCellIndex, ueDLContext_p);	
							/* Cyclomatic_complexity_changes_end */
							/*SPR 6333 Fix Start*/
							timeAdvanceQueueCount--;
							continue;
						}                
					}
					LOG_UT(MAC_TA_NODE_ALREADY_PROCESSED_FOR_UE, LOGDEBUG,
							MAC_DL_Strategy,
							currentGlobalTTITickCount,
							ueDLContext_p->ueIndex,
							taNode_p->timeAdvanceValue,
							DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE,
							DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,
							FUNCTION_NAME,
							"UE is already considered,Updating TA");
				}
			}
			else
			{
				LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,LOGDEBUG,
						MAC_DL_Strategy,
						currentGlobalTTITickCount,
						ueIndex,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME,"");

				tempTaNode_p = taNode_p;
				/* TA_FIX */
				FREE_MEM_NODE_TA_Q(tempTaNode_p);
				timeAdvanceQueueCount--;
				continue; 
				/* TA_FIX */
			}
			/* Also free the memory of TA Nodes*/

			/* SPR 6118 Changes Start */
			ueDLContext_p->timingAdvanceValue = taNode_p->timeAdvanceValue;

			/* Refer Spec 36.213 Section 4.2.3 for the calculations 
			 * NTA,new = NTA,old + (TA - 31) * 16 */
			ueDLContext_p->timingAdvanceAbsValue = 
				ueDLContext_p->timingAdvanceAbsValue + ((taNode_p->
							timeAdvanceValue - MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31) 
						<< 4);                    
			LOG_MAC_MSG(TIMING_ADVANCE_VALUE,LOGINFO,MAC_TA,
					currentGlobalTTITickCount,ueDLContext_p->ueIndex, 
					ueDLContext_p->timingAdvanceAbsValue, 
					taNode_p->timeAdvanceValue,
					DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
					__func__,"");
			/* SPR 6118 Changes End */

			tempTaNode_p = taNode_p;
			FREE_MEM_NODE_TA_Q(tempTaNode_p);
			timeAdvanceQueueCount--;
			numDlUEsToBeScheduled--;
			/* ICIC Change Start */
			/* Update Cell Edge Ue */
			if ( CE_USER == userLocationType )
			{
				--maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] ;
			}   
			/* ICIC Change End */
		}
		else
		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,LOGDEBUG,
					MAC_DL_Strategy,
					currentGlobalTTITickCount,
					timeAdvanceQueueCount,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");

			break;
		}

	};

	/* get number of UEs scheduled */
	numDlUEsStarted -= numDlUEsToBeScheduled;

	/* Update the scheduled UE count */
	*numTAToSchedule_p  -= numDlUEsStarted;
	*numDlUEsToBeScheduled_p -= numDlUEsStarted;
}

/*****************************************************************************
 * Function Name  : dlQosStrategyProcessDRXTrigQueue
 * Inputs         : maxDrxTriggerCount - Max DRX Trigger Count,
 *                  internalCellIndex - cell index used at MAC,
 *                  currentGlobalTTITickCount -Current Global TTI Tick.
 * Outputs        : None
 * Returns        : None
 * Description    : This function pops the DRX on/off trigger queue
 *                  If the trigger is for the DRX_ON, then the 
 *                  eligible LC nodes are removed from the Priority LC tree
 *                  And Updates the Schedule flag as NO_TRANSMISSIOn for the UE
 *                  else the priorities of the LC nodes are recalculated with 
 *                  the DRX on duraton delay and inserted in to the tree with
 *                  new priority/weightage And Updates the Schedule flag as 
 *                  DL_QOS_NEW_TRANSMISSION for the UE DRX.
 *****************************************************************************/
void dlQosStrategyProcessDRXTrigQueue( SInt16 maxDrxTriggerCount, 
		InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount)
/* SPR 15909 fix end */
{
	UInt16                  ueIndex                 = 0; 
	DLUEContext             *ueDLContext_p          = PNULL;
	DLUEContextInfo         *ueDLContextInfo_p      = PNULL;
	void                    *dlEligibleLcNodeVoid_p = NULL;
	DLQosStrategyLCNode     *dlEligibleLcNode_p     = NULL;
	DRXOnOffTriggerNode     *drxTrigNode_p          = NULL;
	DLScheduleFlag          prevScheduleFlag;
	/*QOS-DRX related changes*/
	/*coverity-530 CID-32313*/
	dlQosStrategyEvent      event = DL_QOS_EVENT_DRX_OFF;
	DLScheduleFlag prevScellScheduleFlag = DL_QOS_NEW_TRANSMISSION;
	/*coverity-530 CID-32313*/
	/*QOS-DRX related changes*/
    /* SPR 16050 fix start */
#ifdef LOG_PRINT_ENABLED
    DRXContext                  *drxContext_p = PNULL;
#endif
    /* SPR 16050 fix end */

	/*
	   1.  Pop the node from drxSMTriggerQueue_g  queue
	   2.  if the Trigger is DRX_OFF
	   a.  For each eligible LC node of  the UE
	   i.  Calculate the LC node PDB priority with effective delay as 
	   mentioned below 
	   DrxOnDurationDelay = next short/long DRX cycle duration
	   PPDB = dlQosGetPdbProirity(lcNode_p, DrxOnDurationDelay)
	   ii. Call dlQosCalcLCWeightAndInsert ()
	   3.  if the Trigger is DRX_ON
	   a.  remove the LC nodes from the prioritized LC list
	 */



	while (maxDrxTriggerCount)
	{
		DEQUEUE_DL_DRX_ON_OFF_TRIGGER_Q(DRXOnOffTriggerNode,(void **)&(drxTrigNode_p),internalCellIndex);

		if (drxTrigNode_p)
		{
			ueIndex = drxTrigNode_p->ueIndex;    
			ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
			if (!(ueDLContextInfo_p->pendingDeleteFlag))
			{
				ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

				prevScheduleFlag = ueDLContext_p->scheduleFlag;
                /* SPR 16050 fix start */
#ifdef LOG_PRINT_ENABLED
                drxContext_p = &ueDLContext_p->drxContext;
#endif
                /* SPR 16050 fix end */

				/* EICIC +*/
				/* Pushing UE back in trigger queue.*/
				/*Complexity Start*/      
				if(((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))||((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{              
					if(!ENQUEUE_DL_DRX_ON_OFF_TRIGGER_Q(DRXOnOffTriggerNode, 
								&(drxTrigNode_p->anchorNode), internalCellIndex))
					{
						FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxTrigNode_p);
					}  

					maxDrxTriggerCount--;
					continue;
				}
				/*Complexity End*/
				/* EICIC - */


                /*SPR 21948 Changes Start */
                /* Changes under spr 21762 have been reverted */
                if(isUeEligibleForCaScheduling(ueDLContext_p))
                    /*SPR 21948 Changes End */
				{
					prevScellScheduleFlag = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag;
				}

				/*START : 5352*/
				LOG_MAC_MSG(MAC_DL_DRX_TRIGGER_INFO,LOGDEBUG,
						MAC_DL_Strategy, currentGlobalTTITickCount,
						ueDLContext_p->ueIndex, drxTrigNode_p->onOffTrigger,
						DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
						__func__,"DRX_TRIGGER_INFO_RECIEVED_INFO");
				/*END   : 5352*/

				/*QOS-DRX related changes*/
				if (DRX_ON_TO_DRX_OFF == drxTrigNode_p->onOffTrigger)
				{
					event = DL_QOS_EVENT_DRX_OFF;
				}
				/*SPR_3244_Fix */
				else if (DRX_OFF_TO_DRX_ON == drxTrigNode_p->onOffTrigger)
					/* SPR_3244_Fix */
				{
					event = DL_QOS_EVENT_DRX_ON;
				}
				else
				{
					FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxTrigNode_p);
					maxDrxTriggerCount--;
					continue;
				}


				DL_QOS_UPDATE_SCHEDULE_FLAG(&(ueDLContext_p->scheduleFlag),
						event);

                /*SPR 21948 Changes Start */
                /* Changes under spr 21762 have been reverted */
                if(isUeEligibleForCaScheduling(ueDLContext_p))
                    /*SPR 21948 Changes End */
				{
					DL_QOS_UPDATE_SCHEDULE_FLAG(&(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag),
							event);
				}

				LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCHEDULE_FLAG
					(ueIndex, ueDLContext_p->scheduleFlag);

				/*Checking if the trigger is for DRX-ON/DRX-OFF */
				if (DRX_ON_TO_DRX_OFF == drxTrigNode_p->onOffTrigger)
					/*QOS-DRX related changes*/
				{
                         /* SPR 16050 fix start */
                         if (UL_SYNC_LOSS_ON == ueDLContext_p->
                                    ulSyncLossOrHOInfoInDL.ulSyncLossState)
                         {
                             if ((TRUE == ueDLContext_p->ulSyncLossOrHOInfoInDL.
                                         pendingPDCCHOrder))
                             {
                                 putEntryInPdcchOrderQ(
                                         ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleIdx, \
                                         ueDLContext_p->ueIndex,
                                         getCurrentTick()+
                                         PHY_DL_DELAY,internalCellIndex);
                                 ueDLContext_p->ulSyncLossOrHOInfoInDL.
                                     pendingPDCCHOrder = FALSE;
                                 LOG_MAC_MSG(MAC_UL_SYNC_LOSS_DRX_MEAS, LOGDEBUG,
                                         MAC_DL_Strategy, getCurrentTick(),
                                         ueDLContext_p->ueIndex, drxTrigNode_p->onOffTrigger,
                                         UL_SYNC_LOSS_ON, DEFAULT_INT_VALUE,
                                         FALSE,
                                         DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                         __func__,"SYNC_LOSS_ON_DRX_OFF_PDCCH_TRIG");  
                             }
                             LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                     LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                     ueDLContext_p->ueIndex,
                                     ueDLContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                     CHECK_UE_DRX_STATE(drxContext_p),
                                     DEFAULT_INT_VALUE,
                                     LINE_NUMBER,
                                     DEFAULT_FLOAT_VALUE, 
                                     DEFAULT_FLOAT_VALUE,
                                     FUNCTION_NAME,"UL_DRX_MEAS_STATE");

                             FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxTrigNode_p);
                             maxDrxTriggerCount--;
                             continue;
                         }
                        /* + E_CID_5.3 */
			            if(TRUE == ueDLContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder)
                        {
                            putEntryInPdcchOrderQ(
                                    ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleIdx, \
                                    ueDLContext_p->ueIndex, getCurrentTick()+
                                    PHY_DL_DELAY,internalCellIndex);
                            ueDLContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder = FALSE;
                        } 
                        /* - E_CID_5.3 */
                    /* SPR 16050 fix end */

                        if ( (DL_QOS_NEW_TRANSMISSION == ueDLContext_p->scheduleFlag)||
                                /*SPR 21948 Changes Start */
                                /* Changes under spr 21762 have been reverted */
                                ( (isUeEligibleForCaScheduling(ueDLContext_p)) &&
                                  /*SPR 21948 Changes End */
							  (DL_QOS_NEW_TRANSMISSION == ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag)))
					{
						/* If DRX-OFF : reomove the nodes from the tree and 
						 * calculate their PDB priority with the on-duration 
						 * delay and calculate the LC weight & insert the node 
						 * back into the tree
						 */

						dlEligibleLcNodeVoid_p = (void*) getFirstListNode(
								&ueDLContext_p->dlEligibleLcList);
						while(PNULL != dlEligibleLcNodeVoid_p)
						{
							DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
									dlEligibleLcNodeVoid_p, dlEligibleLcNode_p);

							LOG_UT(
									MAC_LC_PRIORITY_BEFORE_ONDURATON_DELAY_CONSIDERED_IN_DL_LOG_ID,
									LOGDEBUG, MAC_DL_Strategy,
									currentGlobalTTITickCount,
									dlEligibleLcNode_p->totLCWeight, ueIndex, 
									dlEligibleLcNode_p->lcInfo_p->logicalChannelId,
									drxTrigNode_p->onOffTrigger, DEFAULT_INT_VALUE ,
									DEFAULT_FLOAT_VALUE,
									DEFAULT_FLOAT_VALUE,
									FUNCTION_NAME,"");

							/* Delete th strategy Node From the tree */
                            if( (DL_QOS_NEW_TRANSMISSION == prevScheduleFlag) ||
                                    /*SPR 21948 Changes Start */
                                    /* Changes under spr 21762 have been reverted */
                                    ( (isUeEligibleForCaScheduling(ueDLContext_p)) &&
                                      /*SPR 21948 Changes End */
                                      (DL_QOS_NEW_TRANSMISSION == prevScellScheduleFlag)))
							{
								dlQosDeleteEntryFromPrioritisedLcTree
									( dlEligibleLcNode_p, 
									  internalCellIndex,
									  currentGlobalTTITickCount);
							}
							/* Calculate the LC node PDB priority 
							 * with effective delay 
							 * The effective delay will be passed into the 
							 * dlQosCalcLCWeightAndInsert and PDB will be calclated 
							 * inside the function with effective delay
							 */
							/* Calculate the new weight and Insert the node */
							dlQosCalcLCWeightAndInsert(dlEligibleLcNode_p->lcInfo_p,
									ueDLContext_p->mcsPriority,
									drxTrigNode_p->nextOnDurationTTI,internalCellIndex);

							LOG_UT(
									MAC_LC_PRIORITY_AFTER_ONDURATON_DELAY_CONSIDERED,
									LOGDEBUG, MAC_DL_Strategy,
									currentGlobalTTITickCount,
									dlEligibleLcNode_p->totLCWeight, ueIndex,
									dlEligibleLcNode_p->lcInfo_p->logicalChannelId,
									drxTrigNode_p->onOffTrigger,DEFAULT_INT_VALUE,
									DEFAULT_FLOAT_VALUE,
									DEFAULT_FLOAT_VALUE,
									FUNCTION_NAME,"");

							dlEligibleLcNodeVoid_p = (void*) 
								getNextListNode(
										&(dlEligibleLcNode_p->eligibleLcAnchor));

						};
					}
					LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_OFF_DL_COUNT(ueIndex);
				}/*end of if DRX_OFF check */
				/*QOS-DRX related changes*/
				else if(DRX_OFF_TO_DRX_ON == drxTrigNode_p->onOffTrigger)
					/*QOS-DRX related changes*/
				{
                    if((DL_QOS_DRX_ON == ueDLContext_p->scheduleFlag) &&
                            ( (DL_QOS_NEW_TRANSMISSION == prevScheduleFlag) ||
                              /*SPR 21948 Changes Start */
                              /* Changes under spr 21762 have been reverted */
                              ( (isUeEligibleForCaScheduling(ueDLContext_p)) &&
                                /*SPR 21948 Changes End */
							    (DL_QOS_NEW_TRANSMISSION == prevScellScheduleFlag))))
					{
						/* If DRX-ON : reomove the nodes from the tree */ 
						dlEligibleLcNodeVoid_p = (void*)getFirstListNode(
								&ueDLContext_p->dlEligibleLcList);
						while(PNULL != dlEligibleLcNodeVoid_p)
						{
							DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
									dlEligibleLcNodeVoid_p, dlEligibleLcNode_p);

							/* Delete th strategy Node From the tree */
							dlQosDeleteEntryFromPrioritisedLcTree
								( dlEligibleLcNode_p, 
								  internalCellIndex, 
								  currentGlobalTTITickCount);

							dlEligibleLcNodeVoid_p = 
								(void*) getNextListNode(
										&(dlEligibleLcNode_p->eligibleLcAnchor));
						};
					}
					LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_ON_DL_COUNT(ueIndex);
				}
			}
			else
			{
				LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,LOGDEBUG,
						MAC_DL_Strategy,
						currentGlobalTTITickCount,
						ueIndex,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME,"");
			}
			/* Also free the memory of DRX Trigger Nodes*/

			FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxTrigNode_p);
			maxDrxTriggerCount--;
		}
		else
		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,LOGDEBUG,
					MAC_DL_Strategy,
					currentGlobalTTITickCount,
					maxDrxTriggerCount,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");
			break;
		}
	};

}/* End of function dlQosStrategyProcessDRXTrigQueue */

/********************************************************************************
 * Function Name  : dlQosStrategyProcessMeasGapTrigQueue
 * Inputs         : measGapTriggerCount - Measurement gap trigger count
 *                  internalCellIndex - cell index used at MAC,
 *                  currentGlobalTTITickCount -Current Global TTI Tick.
 * Outputs        : None
 * Returns        : None
 * Description    : This function pops the Measurement gap trigger queue
 *                  If the trigger is for the Measurement gap ON, then the 
 *                  eligible LC nodes are removed from the Priority LC tree
 *                  And Updates the Schedule flag as NO_TRANSMISSIOn for the UE.
 ********************************************************************************/
void dlQosStrategyProcessMeasGapTrigQueue( SInt16 measGapTriggerCount, 
		InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount)
/* SPR 15909 fix end */
{
	UInt16                  ueIndex                 = 0; 
	DLUEContext             *ueDLContext_p          = PNULL;
	DLUEContextInfo         *ueDLContextInfo_p      = PNULL;
	void                    *dlEligibleLcNodeVoid_p = NULL;
	DLQosStrategyLCNode     *dlEligibleLcNode_p     = NULL;
	/* SPR 3185 : measGapTrigNode_p declared as MeasGapOnOffTriggerNode*  instead of MeasGapTriggerQueueNode*/
	MeasGapOnOffTriggerNode *measGapTrigNode_p     = NULL;
	DLScheduleFlag          prevScheduleFlag;
	/* SPR_3244_Fix*/
	/*coverity-530 CID-32314*/
	dlQosStrategyEvent event = DL_QOS_EVENT_DRX_OFF;
	/*coverity-530 CID-32314*/
	/* SPR_3244_Fix*/
	DLScheduleFlag prevScellScheduleFlag = DL_QOS_NEW_TRANSMISSION;
    /* SPR 16050 fix start */
#ifdef LOG_PRINT_ENABLED
    MeasGapContextDL          *measGapContext_p = PNULL;
#endif
    /* SPR 16050 fix end */

	/*
	   1.  Pop the node from drxSMTriggerQueue_g  queue
	   2.  if the Trigger is MEAS_GAP_OFF
	   a.  For each eligible LC node of  the UE
	   i.  Remove the strategy  LC Node From the tree, 
	   dlQosDeleteEntryFromPrioritisedLcTree(dlQosStrategyLCNode_p)
	   ii. Calculate the LC node weight, 
	   call dlQosCalcLCWeightAndInsert( dlLogicalChannelInfo_p, 
	   mcsPriority, drxOnDurationDelay)
	   3.  if the Trigger is MEAS_GAP_ON
	   a.  Remove the LC nodes from the prioritized LC list, 
	   dlQosDeleteEntryFromPrioritisedLcTree(dlQosStrategyLCNode_p)

	 */

	while (measGapTriggerCount)
	{
		/* SPR 3185 : Typecasted from (MeasGapTriggerQueueNode*) to (MeasGapOnOffTriggerNode*) */
		DEQUEUE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode, (void **)&measGapTrigNode_p, internalCellIndex);

		if (measGapTrigNode_p)
		{
			ueIndex = measGapTrigNode_p->ueIndex;    
			ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
			if (!(ueDLContextInfo_p->pendingDeleteFlag))
			{
				/* Check for pending TA Ack/Nack removed along with SPR_1346 */
				ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
				prevScheduleFlag = ueDLContext_p->scheduleFlag;
                /* SPR 16050 fix start */
#ifdef LOG_PRINT_ENABLED
                measGapContext_p = &ueDLContext_p->measGapContext;
#endif
                /* SPR 16050 fix end */

				/* EICIC +*/
				/* Pushing UE back in trigger queue.*/
				/*Complexity Start*/

				if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))|| ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{              
					if(!ENQUEUE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
								&(measGapTrigNode_p->anchorNode), internalCellIndex))
					{
						FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapTrigNode_p);
					}
					measGapTriggerCount--;
					continue;
				}
				/*Complexity End*/
				/* EICIC - */

                /*SPR 21948 Changes Start */
                /* Changes under spr 21762 have been reverted */
                if(isUeEligibleForCaScheduling(ueDLContext_p))
                    /*SPR 21948 Changes End */
				{
					prevScellScheduleFlag = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag;
				}

				/* SPR_3244_Fix*/
				if (MEASUREMENT_PERIOD_OFF_DL == measGapTrigNode_p->onOffTrigger)
				{
					event = DL_QOS_EVENT_MEAS_GAP_OFF;
				}
				else if (MEASUREMENT_PERIOD_ON_DL == measGapTrigNode_p->onOffTrigger)
				{
					event = DL_QOS_EVENT_MEAS_GAP_ON;
					/* Review comment fix start GG10 */
					LOG_MAC_MSG(MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_DL_LOG_ID 
							,LOGDEBUG, MAC_DL_Strategy, 
							currentGlobalTTITickCount, ueDLContext_p->ueIndex, 
							measGapTrigNode_p->onOffTrigger,
							LINE_NUMBER, DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
							DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"MEASUREMENTGAPON");
					/* Review comment fix end GG10 */
				}
				else
				{
					FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapTrigNode_p);
					measGapTriggerCount--;
					continue;
				}
				DL_QOS_UPDATE_SCHEDULE_FLAG(&(ueDLContext_p->scheduleFlag),
						event);
				/* SPR_3244_Fix*/

                /*SPR 21948 Changes Start */
                /* Changes under spr 21762 have been reverted */
                if(isUeEligibleForCaScheduling(ueDLContext_p))
                    /*SPR 21948 Changes End */
				{
					DL_QOS_UPDATE_SCHEDULE_FLAG(&(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag),
							event);
				}

				LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCHEDULE_FLAG
					(ueIndex, ueDLContext_p->scheduleFlag);

				/*Checking if Already being considered for RB Distribution*/
				/* SPR_3244_Fix*/
				if (MEASUREMENT_PERIOD_OFF_DL == measGapTrigNode_p->onOffTrigger) 
					/* SPR_3244_Fix*/
				{
                         /* SPR 16050 fix start */
                         if (UL_SYNC_LOSS_ON == ueDLContext_p->
                                    ulSyncLossOrHOInfoInDL.ulSyncLossState)
                         {
                             if ((TRUE == ueDLContext_p->ulSyncLossOrHOInfoInDL.
                                         pendingPDCCHOrder))
                             {
                                 putEntryInPdcchOrderQ(ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleIdx, \
                                         ueDLContext_p->ueIndex,
                                         getCurrentTick()+
                                         PHY_DL_DELAY,internalCellIndex);
                                 ueDLContext_p->ulSyncLossOrHOInfoInDL.
                                     pendingPDCCHOrder = FALSE;
                                 LOG_MAC_MSG(MAC_UL_SYNC_LOSS_DRX_MEAS, LOGDEBUG,
                                         MAC_DL_Strategy, getCurrentTick(),
                                         ueDLContext_p->ueIndex,measGapTrigNode_p->onOffTrigger,
                                         UL_SYNC_LOSS_ON, DEFAULT_INT_VALUE,
                                         FALSE,
                                         DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                                         __func__,"SYNCLOSS_ON_MEAS_OFF_PDCCH_TRIG");  
                             }
                             LOG_MAC_MSG(MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE,
                                     LOGDEBUG, MAC_DL_Strategy, getCurrentTick(),
                                     ueDLContext_p->ueIndex,
                                     ueDLContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,  
                                     DEFAULT_INT_VALUE,
                                     CHECK_UE_MEAS_GAP_PERIOD(measGapContext_p),
                                     LINE_NUMBER,
                                     DEFAULT_FLOAT_VALUE, 
                                     DEFAULT_FLOAT_VALUE,
                                     FUNCTION_NAME,"UL_DRX_MEAS_STATE");

                             FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapTrigNode_p);
                             measGapTriggerCount--;
                             continue;

                         }
                        /* + E_CID_5.3 */
                        if(TRUE == ueDLContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder)
                        {
                            putEntryInPdcchOrderQ(
                                    ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleIdx, \
                                    ueDLContext_p->ueIndex, getCurrentTick()+
                                    PHY_DL_DELAY,internalCellIndex);
                            ueDLContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder = FALSE;
                        }
                        /* - E_CID_5.3 */
                    /* SPR 16050 fix end */

					if ( (DL_QOS_NEW_TRANSMISSION == ueDLContext_p->scheduleFlag)||
                            /*SPR 21948 Changes Start */
                            /* Changes under spr 21762 have been reverted */
                            ( (isUeEligibleForCaScheduling(ueDLContext_p)) &&
                              /*SPR 21948 Changes End */
							  (DL_QOS_NEW_TRANSMISSION == ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag)))
					{
						dlEligibleLcNodeVoid_p = (void*) getFirstListNode(
								&ueDLContext_p->dlEligibleLcList);
						while(PNULL != dlEligibleLcNodeVoid_p)
						{
							DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
									dlEligibleLcNodeVoid_p, dlEligibleLcNode_p);
							/* Calculate the new weight and Insert the node 
							 * In the Prioritised LC Tree
							 * No effective Delay, for PDB priority 
							 */
							dlQosCalcLCWeightAndInsert(dlEligibleLcNode_p->lcInfo_p,
									ueDLContext_p->mcsPriority,
									ZERO_DRX_DELAY,internalCellIndex);

							dlEligibleLcNodeVoid_p = 
								(void*) getNextListNode(&(dlEligibleLcNode_p->eligibleLcAnchor));
						};
					}
					LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_OFF_DL_COUNT(ueIndex); 
				}/*end of if DRX_OFF check */
				/* SPR_3244_Fix*/
				else if(MEASUREMENT_PERIOD_ON_DL == measGapTrigNode_p->onOffTrigger)
					/* SPR_3244_Fix*/
				{
					if((DL_QOS_MEAS_GAP_ON == ueDLContext_p->scheduleFlag) &&
							( (DL_QOS_NEW_TRANSMISSION == prevScheduleFlag) ||
                              /*SPR 21948 Changes Start */
                              /* Changes under spr 21762 have been reverted */
                              ( (isUeEligibleForCaScheduling(ueDLContext_p)) &&
                                /*SPR 21948 Changes End */
							    (DL_QOS_NEW_TRANSMISSION == prevScellScheduleFlag))))
					{
						dlEligibleLcNodeVoid_p = (void*) getFirstListNode(
								&ueDLContext_p->dlEligibleLcList);

						while(PNULL != dlEligibleLcNodeVoid_p)
						{
							DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
									dlEligibleLcNodeVoid_p, dlEligibleLcNode_p);

							/* Delete th strategy Node From the tree */
							dlQosDeleteEntryFromPrioritisedLcTree
								( dlEligibleLcNode_p, 
								  internalCellIndex,
								  currentGlobalTTITickCount);

							dlEligibleLcNodeVoid_p = 
								(void*) getNextListNode(
										&(dlEligibleLcNode_p->eligibleLcAnchor));
						};
					}
					LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_ON_DL_COUNT(ueIndex);
				}
			}
			else
			{
				LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,LOGDEBUG,
						MAC_DL_Strategy,
						currentGlobalTTITickCount,
						ueIndex,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME,"");
			}
			/* Also free the memory of TA Nodes*/

			FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapTrigNode_p);
			measGapTriggerCount--;
		}
		else
		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,LOGDEBUG,
					MAC_DL_Strategy,
					currentGlobalTTITickCount,
					measGapTriggerCount,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");
			break;
		}
	};

}/* End of function dlQosStrategyProcessMeasGapTrigQueue */

/*****************************************************************************
 * Function Name  : dlQosStrategyProcessPdcchOrderQueue
 * Inputs         : pdcchOrderCount - PDCCH Order count,
 *                  dlDelay - DL Delay,
 *                  internalCellIndex - cell index used at MAC,
 *                  currentGlobalTTITickCount -Current Global TTI Tick.
 * Outputs        : None
 * Returns        : None
 * Description    : This function pops the UL Sync Loss trigger queue
 *                  If the trigger is for the SYNC_LOSS_ON, then the 
 *                  eligible LC nodes are removed from the Priority LC tree
 *                  And Updates the Schedule flag as NO_TRANSMISSIOn for the UE
 *                  Else the Nodes in the eligible list will be Added into the 
 *                  TREE after calculating thr priority.
 *****************************************************************************/
void dlQosStrategyProcessPdcchOrderQueue( SInt16 pdcchOrderCount,
		/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
		UInt32 dlDelay, 
#endif
		/* - SPS_TDD_Changes */
		InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount)
/* SPR 15909 fix end */
{
	UInt16                  ueIndex                 = 0; 
	DLUEContext             *ueDLContext_p          = PNULL;
	DLUEContextInfo         *ueDLContextInfo_p      = PNULL;
	void                    *dlEligibleLcNodeVoid_p = NULL;
	DLQosStrategyLCNode     *dlEligibleLcNode_p     = NULL;
	ULSyncLossQueueNode     *ulSyncLossNode_p     = NULL;
	DLScheduleFlag          prevScheduleFlag;
	dlQosStrategyEvent event;
	LP_SpsDlInfo            spsDlInfo_p           = PNULL;
	DLScheduleFlag prevScellScheduleFlag = DL_QOS_NEW_TRANSMISSION;
	/* +- SPR 17777 */

	/*

	   1.  Pop the node from drxSMTriggerQueue_g  queue
	   2.  if the Trigger is SYNC_LOSS_OFF
	   a.  For each eligible LC node of  the UE
	   i.  Remove the strategy  LC Node From the tree,
	   dlQosDeleteEntryFromPrioritisedLcTree(dlQosStrategyLCNode_p)
	   ii. Calculate the LC node weight, 
	   call dlQosCalcLCWeightAndInsert( dlLogicalChannelInfo_p, 
	   mcsPriority, drxOnDurationDelay)
	   3.  if the Trigger is SYNC_LOSS_ON
	   a.   Remove the LC nodes from the prioritized LC list,
	   dlQosDeleteEntryFromPrioritisedLcTree(dlQosStrategyLCNode_p)
	   */

	pdcchOrderCount = COUNT_UL_SYNC_LOSS_TRIGGERQ_DL_Q(internalCellIndex);

	while (pdcchOrderCount)
	{
		DEQUEUE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ULSyncLossQueueNode, (void **)&ulSyncLossNode_p,internalCellIndex);

		if (ulSyncLossNode_p)
		{
			ueIndex = ulSyncLossNode_p->ueIndex;    
			ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
			if (!(ueDLContextInfo_p->pendingDeleteFlag))
			{
				/* Check for pending TA Ack/Nack removed along with SPR_1346 */
				ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
				prevScheduleFlag = ueDLContext_p->scheduleFlag;

				/* EICIC +*/
				/* Pushing UE back in trigger queue.*/        
				/*Complexity Start */
				if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))||((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{              
					if(!ENQUEUE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ULSyncLossQueueNode,
								&(ulSyncLossNode_p->anchorNode), internalCellIndex))
					{
						FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ulSyncLossNode_p);
					}

					pdcchOrderCount--;   
					continue;
				}
				/*Complexity End*/
				/* EICIC - */

                /*SPR 21948 Changes Start */
                /* Changes under spr 21762 have been reverted */
                if(isUeEligibleForCaScheduling(ueDLContext_p))
                    /*SPR 21948 Changes End */
				{
					prevScellScheduleFlag = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag;
				}
				if(UL_SYNC_LOSS_OFF == ulSyncLossNode_p->onOffTrigger)
				{
					event = DL_QOS_EVENT_SYNC_LOSS_OFF;
				}
				else if(UL_SYNC_LOSS_ON == ulSyncLossNode_p->onOffTrigger)
				{
					event = DL_QOS_EVENT_SYNC_LOSS_ON;
				}
				else
				{
					FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ulSyncLossNode_p);
					pdcchOrderCount--;
					continue;
				}
				DL_QOS_UPDATE_SCHEDULE_FLAG(&(ueDLContext_p->scheduleFlag),event);

                /*SPR 21948 Changes Start */
                /* Changes under spr 21762 have been reverted */
                if(isUeEligibleForCaScheduling(ueDLContext_p))
                    /*SPR 21948 Changes End */
				{
					DL_QOS_UPDATE_SCHEDULE_FLAG(&(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag),
							event);
				}

				LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCHEDULE_FLAG
					(ueIndex, ueDLContext_p->scheduleFlag);

				if (UL_SYNC_LOSS_OFF == ulSyncLossNode_p->onOffTrigger)
				{
					/* SPR 4751 changes */
					ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleIdx = INVALID_PREAMBLE_IDX;
					ueDLContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState = UL_SYNC_LOSS_OFF;
					/* SPR 4751 changes */
					spsDlInfo_p = &ueDLContext_p->spsDlInfo;
					/* Call SPS state machine to change the state for SPS 
					 * strategy */
					if(spsDlInfo_p->spsState != INVALID_STATE)
					{
						/* +- SPR 17777 */
						dlStateMachine[spsDlInfo_p->spsState][UE_SYNC_LOSS_OFF](
								ueIndex, spsDlInfo_p->spsState,
								UE_SYNC_LOSS_OFF, ueDLContext_p, PNULL, 
								/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
								dlDelay ,
#else
								DL_ADVANCE_TICK,
#endif                            
								/* - SPS_TDD_Changes */
								internalCellIndex );
						LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
								getCurrentTick(), __LINE__, ueIndex,spsDlInfo_p->spsState, UE_SYNC_LOSS_OFF,
								spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
					}

					if ( (DL_QOS_NEW_TRANSMISSION == ueDLContext_p->scheduleFlag)||
                            /*SPR 21948 Changes Start */
                            /* Changes under spr 21762 have been reverted */
                            ( (isUeEligibleForCaScheduling(ueDLContext_p)) &&
                              /*SPR 21948 Changes End */
							  (DL_QOS_NEW_TRANSMISSION == ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag)))
					{
						/*  For each eligible LC of  the UE
						 *  remove the LC nodes from the prioritized LC list
						 */                 

						dlEligibleLcNodeVoid_p = (void*) getFirstListNode(
								&ueDLContext_p->dlEligibleLcList);

						while(PNULL != dlEligibleLcNodeVoid_p)
						{
							DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
									dlEligibleLcNodeVoid_p, dlEligibleLcNode_p);

							/* Calculate the new weight and Insert the node 
							 * In the Prioritised LC Tree
							 * No effective Delay, for PDB priority 
							 */
							dlQosCalcLCWeightAndInsert(dlEligibleLcNode_p->lcInfo_p,
									ueDLContext_p->mcsPriority,
									ZERO_DRX_DELAY,internalCellIndex);

							/* Get Next LC Node */
							dlEligibleLcNodeVoid_p = 
								(void*) getNextListNode(
										&(dlEligibleLcNode_p->eligibleLcAnchor));

						};
					}
					LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_OFF_DL_COUNT(ueIndex);
				}/*end of if SYNC_LOSS_OFF check */
				else if(UL_SYNC_LOSS_ON == ulSyncLossNode_p->onOffTrigger)
				{
					if((DL_QOS_SYNC_LOSS_ON == ueDLContext_p->scheduleFlag) &&
							( (DL_QOS_NEW_TRANSMISSION == prevScheduleFlag) ||
                              /*SPR 21948 Changes Start */
                              /* Changes under spr 21762 have been reverted */
                              ( (isUeEligibleForCaScheduling(ueDLContext_p)) &&
                                /*SPR 21948 Changes End */
							    (DL_QOS_NEW_TRANSMISSION == prevScellScheduleFlag))))
					{
						dlEligibleLcNodeVoid_p = (void*) getFirstListNode(
								&ueDLContext_p->dlEligibleLcList);

						while(PNULL != dlEligibleLcNodeVoid_p)
						{
							DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
									dlEligibleLcNodeVoid_p, dlEligibleLcNode_p);

							/* Delete th strategy Node From the tree */
							dlQosDeleteEntryFromPrioritisedLcTree
								( dlEligibleLcNode_p, 
								  internalCellIndex,
								  currentGlobalTTITickCount);

							/* Get Next LC Node */
							dlEligibleLcNodeVoid_p = 
								(void*) getNextListNode(
										&(dlEligibleLcNode_p->eligibleLcAnchor));
						};
					}
					LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_ON_DL_COUNT(ueIndex);
				}/*end of else SYNC_LOSS_OFF check */
			}
			else
			{
				LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,LOGDEBUG,
						MAC_DL_Strategy,
						currentGlobalTTITickCount,
						ueIndex,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME,"");
			}
			/* Also free the memory of TA Nodes*/

			FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(ulSyncLossNode_p);
			pdcchOrderCount--;
		}
		else
		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,LOGDEBUG,
					MAC_DL_Strategy,
					currentGlobalTTITickCount,
					pdcchOrderCount,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");
			break;
		}
	};

}/* End of function dlQosStrategyProcessPdcchOrderQueue */
/*****************************************************************************
 * Function Name  : dlQosStrategyUpdateNewTxUEList
 * Inputs         : ueDLContext_p - pointer to DLUEContext,
 *                  eDlTxMsgType,
 *                  traversalType - NEW TRAVERSAl/RETRAVERSAL,
 *                  currentGlobalTTITickCount -Global TTI Tick,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : dlStrategyTxNode_p - pointer of the strategy node created for
 *                  the DL transmission of UE 
 * Description    : Updates the Strategy LC node in the Tree.
 *****************************************************************************/
/*QOS_CYCLIC_COMPLEXITY_CHG*/
DLStrategyTxNode* dlQosStrategyUpdateNewTxUEList(DLUEContext *ueDLContext_p,
		MsgType eDlTxMsgType,
		dlQosTraversalType traversalType,
		/* + TM7_8 Changes Start */
        /* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		/* - TM7_8 Changes End */
		ServingCellIndex servingCellIndex)
/*QOS_CYCLIC_COMPLEXITY_CHG*/
{
	DLStrategyTxNode *dlStrategyTxNode_p = PNULL;
	DLHARQProcess *harqProcess_p = PNULL;
	ResourceAllocatorInput *tempResourceAllocatorInput_p = PNULL;
	DLUeScheduleInfo * dlUeScheduleInfo_p = PNULL;

	UInt8         isSubbandPMIAvailable = FALSE;
	UInt8         aperiodicMode12Rcvd   = FALSE;
	DLHARQEntity* dlHarqContext_p       = PNULL;
	DLCQIInfo*    dlCQIInfo_p           = PNULL; 

	/*SPR 20899 Fix Start */
	UInt8 strategyContainerIndex = MODULO_TWO(getCurrentTick()); 
    /*SPR 20899 Fix Stop */

	if(IS_PCELL_OF_UE(ueDLContext_p, servingCellIndex))
	{
		dlHarqContext_p       = ueDLContext_p->dlHarqContext_p;
		isSubbandPMIAvailable = ueDLContext_p->isSubbandPMIAvailable;
		aperiodicMode12Rcvd   = ueDLContext_p->aperiodicMode12Rcvd;
		dlCQIInfo_p           = &(ueDLContext_p->dlCQIInfo);
	}
	else
	{
		dlHarqContext_p       = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p;
		isSubbandPMIAvailable = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->isSubbandPMIAvailable;
		aperiodicMode12Rcvd   = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->aperiodicMode12Rcvd;
		dlCQIInfo_p           = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
	}


	/* Get the Free HARQ process ID update in Strategy TX Node */
	if (PNULL == (harqProcess_p = checkForFreeHarqProcess
				(ueDLContext_p, dlHarqContext_p, 
				 DRA_SCHEDULING, servingCellIndex)))
	{
		LOG_MAC_MSG(MAC_ALL_HARQ_ARE_BUSY,LOGFATAL,
				MAC_DL_Strategy,
				currentGlobalTTITickCount,
				ueDLContext_p->ueIndex,
				servingCellIndex,
				ueDLContext_p->internalCellIndex,
				dlHarqContext_p->numHARQProcess,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME,"");
		return PNULL;
	}


	GET_MEM_FROM_POOL(DLStrategyTxNode,dlStrategyTxNode_p,
			sizeof(DLStrategyTxNode),PNULL);

	/* + Coverity 24525 */
	if( PNULL == dlStrategyTxNode_p )
	{
		return PNULL;
	}
	/* - Coverity 24525 */

	/*Updating the parameters & Fields of DL Strategy Tx node*/
	dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;

	dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId ;
	dlStrategyTxNode_p->harqProcess_p = harqProcess_p;
	harqProcess_p->dlHARQTBOneInfo.tbSize    =  0;
	harqProcess_p->dlHARQTBTwoInfo.tbSize    =  0;

	dlStrategyTxNode_p->transmissionType = NEW_TX;

	dlStrategyTxNode_p->maxTBSize = 0;
	dlStrategyTxNode_p->minTBSize = 0;
	/*initialising the resource allocator input parameters*/

	tempResourceAllocatorInput_p = 
		&(dlStrategyTxNode_p->resourceAllocInput); 

	tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;        
	/* + CQI_5.0 */
	tempResourceAllocatorInput_p->subBandPMIInfo_p = PNULL;
	tempResourceAllocatorInput_p->subbandPMI = FALSE;
	tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;        
	/* - CQI_5.0 */

	/* + CQI_5.0 */
	if (isSubbandPMIAvailable)
	{
		tempResourceAllocatorInput_p->subbandPMI = TRUE;
		tempResourceAllocatorInput_p->subBandPMIInfo_p =
			&dlCQIInfo_p->subBandReportInfo_p->cqiInfo.cqiInfoSpatialMul.pmiInfo;
	}
	if(TRUE == aperiodicMode12Rcvd) 
	{
		tempResourceAllocatorInput_p->aperiodicMode12 = TRUE;
		tempResourceAllocatorInput_p->subBandReportInfo_p =
			dlCQIInfo_p->subBandReportInfo_p;
	}    
	/* - CQI_5.0 */

	/* + CQI_4.1 */
	if (dlCQIInfo_p->isSubBandReportAvailable)
	{
		tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;
		tempResourceAllocatorInput_p->subBandReportInfo_p =
			dlCQIInfo_p->subBandReportInfo_p;
	}
	/* - CQI_4.1 */

	tempResourceAllocatorInput_p->strictAllocSubBand = FALSE;

	dlStrategyTxNode_p->isCQIRequestedULGrantMsg = FALSE;

	dlStrategyTxNode_p->sumLCPriority = FALSE;

	dlStrategyTxNode_p->strictAllocatedFlag = FALSE;

	dlStrategyTxNode_p->weightedQueueLoad = FALSE;

	dlStrategyTxNode_p->numPushedInAdvanceQueue = ZERO;

	dlQosGetTransmissionMode(dlStrategyTxNode_p, 
			ueDLContext_p, 
			harqProcess_p,
			/* + TM7_8 Changes Start */
			currentGlobalTTITickCount,
			/* - TM7_8 Changes End */
			servingCellIndex);

	/*putEntry of DLStrategyTXNode in DL Strategy Schedule Info */

	dlUeScheduleInfo_p = &(dlStrategyScheduleInfo_g[strategyContainerIndex][servingCellIndex].dlUeScheduleInfo
			[dlStrategyScheduleInfo_g[strategyContainerIndex][servingCellIndex].schdeuledUeCount]);

	dlUeScheduleInfo_p->ueDLContext_p = ueDLContext_p;

	dlUeScheduleInfo_p->dLStrategyTxNode_p = dlStrategyTxNode_p;

	dlUeScheduleInfo_p->harqProcess_p = harqProcess_p;

	dlUeScheduleInfo_p->isTaDrxScheduled = eDlTxMsgType;

	/* Increament the scheduled UE count */
	dlStrategyScheduleInfo_g[strategyContainerIndex][servingCellIndex].schdeuledUeCount++;
        /*fprintf(stderr,"Info at %s %d %llu : strategyContainerIndex %d servingCellIndex %d .schdeuledUeCount %d \n",__func__,__LINE__,getCurrentTick(),strategyContainerIndex,servingCellIndex,dlStrategyScheduleInfo_g[strategyContainerIndex][servingCellIndex].schdeuledUeCount);*/

	/*QOS_CYCLIC_COMPLEXITY_CHG*/
	dlQosProcessUeForRBRestriction(traversalType,
			dlStrategyTxNode_p->harqProcess_p,
			ueDLContext_p,
			servingCellIndex);
	/*QOS_CYCLIC_COMPLEXITY_CHG*/
	return dlStrategyTxNode_p;
}/* End of Function dlQosStrategyUpdateNewTxUEList */

/*****************************************************************************
 * Function Name  : dlQosPutEntryInPrioritisedLcTree
 * Inputs         : dlQosStrategyLCNode_p - pointer to LC strategy Node to be
 *                                          inserted into tree ,
 *                  internalCellIndex - cell index used at MAC,
 *                  currentGlobalTTITickCount -Current Global TTI Tick.
 * Outputs        : None
 * Returns        : None
 * Description    : Inserts the Strategy LC node in the Tree.
 *****************************************************************************/
void dlQosPutEntryInPrioritisedLcTree( DLQosStrategyLCNode *dlQosStrategyLCNode_p,
		InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount)
/* SPR 15909 fix end */
{
	if ( FALSE == dlQosStrategyLCNode_p->lcInfo_p->isLCNodePresent )
	{
		/* EICIC +*/
		DLUEContext *dlUEContext_p = PNULL;
		DLUEContextInfo *dlUEContextInfo_p = PNULL;
		/* EICIC -*/
		LOG_MAC_MSG(MAC_QOS_INSERT_NODE_PRIORITY_LC_TREE,LOGBRIEF,MAC_DL_Strategy,
				currentGlobalTTITickCount,
				dlQosStrategyLCNode_p->ueIndex, dlQosStrategyLCNode_p->lcId,
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
				"");
		/* EICIC +*/
		dlUEContextInfo_p = &dlUECtxInfoArr_g[dlQosStrategyLCNode_p->ueIndex];
		dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;    


		dlQosInsertLcNode(dlPriorityLcTree_gp[internalCellIndex][dlUEContext_p->userType] , &dlQosStrategyLCNode_p->lcTreeAnchor);
		/* EICIC -*/

		LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_TREE_COUNT(dlQosStrategyLCNode_p->ueIndex);

		LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOTAL_LC_WIEGHT
			(dlQosStrategyLCNode_p->ueIndex, dlQosStrategyLCNode_p->lcId,
			 dlQosStrategyLCNode_p->totLCWeight);
		dlQosStrategyLCNode_p->lcInfo_p->isLCNodePresent = TRUE;
	}
}

/*****************************************************************************
 * Function Name  : dlQosDeleteEntryFromPrioritisedLcTree
 * Inputs         : dlQosStrategyLCNode_p - pointer to LC strategy Node to be
 *                                          deleted from tree,
 *                  internalCellIndex - cell index used at MAC,
 *                  currentGlobalTTITickCount -Current Global TTI Tick.
 * Outputs        : None
 * Returns        : None
 * Description    : Deletes the Strategy LC node form the Tree.
 *****************************************************************************/
void dlQosDeleteEntryFromPrioritisedLcTree(
		DLQosStrategyLCNode *dlQosStrategyLCNode_p,
		InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount)
/* SPR 15909 fix end */
{
	if ( dlQosStrategyLCNode_p->lcInfo_p->isLCNodePresent == TRUE)
	{
		/* EICIC +*/
		DLUEContext *dlUEContext_p = PNULL;
		DLUEContextInfo *dlUEContextInfo_p = PNULL;
		/* EICIC -*/

		LOG_MAC_MSG(MAC_QOS_DELETE_NODE_PRIORITY_LC_TREE,LOGBRIEF,MAC_DL_Strategy,
				currentGlobalTTITickCount,
				dlQosStrategyLCNode_p->ueIndex, dlQosStrategyLCNode_p->lcId,
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
				"");
		/* EICIC +*/
		dlUEContextInfo_p = &dlUECtxInfoArr_g[dlQosStrategyLCNode_p->ueIndex];
		dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;      


		dlQosDeleteLcNode(dlPriorityLcTree_gp[internalCellIndex][dlUEContext_p->userType] , &dlQosStrategyLCNode_p->lcTreeAnchor);
		/* EICIC -*/

		LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_TREE_COUNT(dlQosStrategyLCNode_p->ueIndex);
		dlQosStrategyLCNode_p->lcInfo_p->isLCNodePresent = FALSE;
	}
}

/*******************************************************************************
 * Function Name  : dlQosReTraversePriorityLcList
 * Inputs         : numberOfAvailableRBs_p - Number of RBs available,
 *                  numDlUEsToBeScheduled - Number of UE's to be scheduled,
 *                  subFrameNum - Subframe for which scheduling is done,
 *                  currentGlobalTTITickCount - Global TTI Tick,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function traverses the Strategy LC node form the Tree.
 *******************************************************************************/
/* +- SPR 17777 */
void dlQosReTraversePriorityLcList(
#ifdef TDD_CONFIG
        UInt8 subFrameNum,
#endif
        /* +- SPR 17777 */
		tickType_t currentGlobalTTITickCount,
		InternalCellIndex internalCellIndex
        /* SPR 22539 Fix + */
        ,UInt8 allowAllocBitRate 
        /* SPR 22539 Fix - */
        )
{

	/*
	   1.  Get teh total schedulable RB's 
	   2.  Traverse through the prioritized LC list and allocate RB.s for 
	   each LC upto 150 RB (+ )
	   3.  for each LC node, if((scheduled RB.s <=  maxSchedulableRB) && 
	   lcInfo_p->availToken))
	   a.  if the LC node ==  first LC node of UE to be scheduled
	   and Number of schedulable UE's is Non-Zero
	   i.  And insert UE Node for each scheduled first LC node of UE, in 
	   schedule UE list by calling 
	   dlQosStrategyUpdateNewTxUEList(eDlTxMsgType)
	   ii. check for hte strategy node if NULL continue with next node
	   b.  Else
	   i.  Get  the strategy node
	   ii. if(lc not scheduled )
	   1. Append the LC node to the tail od LC list

	   c.  if ( Scheduled TB Size of UE  < Max TB Size of UE)
	   i.   Obtain the schedulable bytes equal to 
	   maximum of {queueLoad , previously Scheduled Bytes} of the LC
	   ii.  schedulable bytes += (MAC_SUBHEADER_SIZE + rlcFixedHeaderSize)
	   iii. If((Scheduled TB Size of UE +  schedulable bytes)  >
	   Max TB Size of UE)
	   1. schedulable bytes  +=
	   Scheduled TB Size of UE - Max TB Size of UE
	   2. if(scheduled bytes < 
	   (MAC_SUBHEADER_SIZE + rlcFixedHeaderSize))
	   a. scheduled bytes = 0;
	   3. reScheduledBytes = schedulableBytes -
	   dlQosStrategyLCNode_p->schedulableBytes;
	   4. if(reScheduled bytes)
	   a. If scheduling for 100 RB.s is not crossed
	   1.  Increment the Min TB Size with the
	   reSchedulable bytes for the LC
	   b. Increment the Max TB Size with the
	   reSchedulable bytes for the LC
	   c. get the RB.s required for the LC scheduled and
	   increment the scheduled RB by it,
	   dlQosGetResBlksForScheduledBytes(mcsIndex , minTBSize,
	   harqProcess_p )
	   d. update dlQosStrategyLCNode_p->schedulableBytes
	   e. Append the LC node into the schedlue LC list
	 */

	UInt32 schedulableBytes = 0;
	UInt32 reScheduledBytes = 0;
	UInt8 requiredRB = 0;
	SInt8 deltaRB = 0;
	UInt16 ueIndex = 0;
	SInt8 tempRB = 0;
	/* SPR 10143 fix start */
	dlQosTraversalType traversalType = RETRAVERSAL;
	/* SPR 10143 fix end */

	DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;

	DLUEContextInfo * ueDLContextInfo_p = PNULL;
	DLUEContext * ueDLContext_p = PNULL;
	DLLogicalChannelInfo *lcInfo_p = PNULL;
	DLStrategyTxNode * dlStrategyTxNode_p = PNULL;
	DLHARQProcess *harqProcess_p = PNULL;
	UInt8 userLocationType = CC_USER ;
	MacRetType retVal = MAC_SUCCESS;
    /* SPR 22539 Fix + */
	UInt16* totalSchBytes_p                 = PNULL;
	SInt32* servingCellAvailToken_p         = PNULL;
	UInt16* servingCellSchedulableBytes_p   = PNULL;
	UInt8 transmissionMode                  = 0;
	UInt32  netQueueLoad                    = 0;
	UInt32  netAvlblTkns                    = 0;
    /* SPR 22539 Fix - */

	UInt16 maxTBSize = 0;
	DLScheduleFlag* scheduleFlag_p = PNULL;
	UInt8 preferredCellListIndex   = MAX_NUM_CELL;
	UInt8 listIndex                = 0;
	/** SPR 13599 Changes Start **/
	UInt8 numOfPreferredCells      = MAX_NUM_PCELL;
	/** SPR 13599 Changes End **/
	ServingCellIndex servCellIndex = INVALID_CELL_INDEX;
	InternalCellIndex pCellIndex   = INVALID_CELL_INDEX;
	ServingCellIndex servingCellIndex = INVALID_CELL_INDEX;
	DLCQIInfo*  dlCQIInfo_p           = PNULL; 

	/** SPR 13599 Changes Start **/
	UInt8 isCaEnabled = IS_CA_ENABLED();
	/** SPR 13599 Changes End **/

	/* EICIC SPR 15107 Fix Start */
	/*Complexity Start*/
	dlQosStrategyLCNode_p = eicicUpdate_ueTypeToScheduleDL_g(
			isCaEnabled,
			internalCellIndex);
	/*Complexity End*/
	/* EICIC SPR 15107 Fix End */
	/*EICIC -*/
	while(NULL != dlQosStrategyLCNode_p)
	{
		/** SPR 13599 Changes Start **/
		if(isCaEnabled)
			/** SPR 13599 Changes End **/
		{
			/* Get preference Cell List for the UE */
			retVal = dlQosGetPreferedCellListForScheduling(
					dlQosStrategyLCNode_p->ueIndex,
					FALSE /*isMacCEScheduled*/,
                                                &preferredCellListIndex,
                                                /* SPR 16116 fix start */
                                                dlQosStrategyLCNode_p->totLCWeightWithLP);
                                                /* SPR 16116 fix end */
			if(MAC_FAILURE == retVal)
			{
				if ( VICTIM_UE != ueTypeToScheduleDL_g[internalCellIndex])
				{
					/* Get the next node from the TREE traversing IN-ORDER */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode(
							internalCellIndex,
							FALSE /*reStartTraversing*/);
				}
				else
				{
					/* Get the next node from the TREE traversing IN-ORDER */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode(
							internalCellIndex,
							FALSE /*reStartTraversing*/);

				}
				continue;
			}
		}
		ueIndex = dlQosStrategyLCNode_p->ueIndex;
		/* Get UE Context */
		ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        /* SPR 21482 Fix Start */
        if(ueDLContextInfo_p->pendingDeleteFlag)
        {
            if ( VICTIM_UE != ueTypeToScheduleDL_g[internalCellIndex])
            {
                /* Move  to next node  */
                dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode(
                        internalCellIndex,
                        FALSE /*reStartTraversing*/);

            }
            else
            {
                /* Move  to next node  */
                dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode(
                        internalCellIndex,
                        FALSE /*reStartTraversing*/);

            }
            continue;

        }
        /* SPR 21482 fix end */
		ueDLContext_p     = ueDLContextInfo_p->dlUEContext_p;

		pCellIndex = ueDLContext_p->internalCellIndex;
		servingCellIndex = pCellIndex ;

		if(isRbAvailableOnPreferredCells( pCellIndex ,
					preferredCellListIndex))
		{
			retVal = dlQosValidatePriorityTreeNode( ueDLContext_p, 
					dlQosStrategyLCNode_p, 
					currentGlobalTTITickCount,
					internalCellIndex, 
					FALSE /*gbrScheduling*/);
			if (MAC_FAILURE == retVal)
			{
				if ( VICTIM_UE != ueTypeToScheduleDL_g[internalCellIndex])
				{
					/* Move to next node */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode(
							internalCellIndex,
							FALSE /*reStartTraversing*/);

				}
				else
				{
					/* Move to next node */
					dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode(
							internalCellIndex,
							FALSE /*reStartTraversing*/);

				}
				continue;
			}

			/** SPR 13599 Changes Start **/
			if(isCaEnabled)
			{
				numOfPreferredCells = dlQoSPreferredCellList_g[pCellIndex].preferredCellList\
						      [preferredCellListIndex].count;
			}
			for(listIndex = 0; listIndex < numOfPreferredCells; listIndex++)
			{
				schedulableBytes = 0;
				deltaRB = 0;
				tempRB = 0;
				reScheduledBytes = 0;
				requiredRB = 0;
                /* SPR 22539 Fix + */
                totalSchBytes_p = &(dlQosStrategyLCNode_p->schedulableBytes);
                /* SPR 22539 Fix - */

				if(isCaEnabled)
				{
					servingCellIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellList\
							   [preferredCellListIndex].preferredCellListInfo[listIndex].cellIndex;
				}
				/** SPR 13599 Changes End **/
				dlStrategyTxNode_p = dlQoSPrepareUEStrategyNode( ueDLContext_p,
                        /* +- SPR 17777 */
						currentGlobalTTITickCount,
						servingCellIndex,
						dlQosStrategyLCNode_p,
						traversalType);
				if(PNULL == dlStrategyTxNode_p)
				{
					continue;
				}

				if(pCellIndex == servingCellIndex)
				{
					maxTBSize        = ueDLContext_p->maxTBSize;
					userLocationType = ueDLContext_p->userLocationType;
					scheduleFlag_p   = &(ueDLContext_p->scheduleFlag);
					dlCQIInfo_p      = &(ueDLContext_p->dlCQIInfo);
				}
				else
				{
					servCellIndex = getServeCellIndexFromInternalCellId(
							ueDLContext_p, 
							servingCellIndex);

					maxTBSize        = ueDLContext_p->dlUeScellContext_p[servCellIndex]->maxTBSize;
					userLocationType = ueDLContext_p->dlUeScellContext_p[servCellIndex]->userLocationType;
					scheduleFlag_p   = &(ueDLContext_p->dlUeScellContext_p[servCellIndex]->scheduleFlag);
					dlCQIInfo_p      = &(ueDLContext_p->dlUeScellContext_p[servCellIndex]->ScelldlCQIInfo);
				}

				if (RE_TX == dlStrategyTxNode_p->transmissionType && scheduleFlag_p != PNULL)
				{
					*scheduleFlag_p = DL_QOS_NO_NEW_TRANSMISSION;

					LOG_UT(MAC_NUM_UE_PER_TICK_REACHED_IN_DL_LOG_ID,LOGDEBUG,MAC_DL_Strategy,
							currentGlobalTTITickCount,
							ueIndex,DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,FUNCTION_NAME, 
							"No UE will be scheduled in Re-Tx");

					continue;
				}
				lcInfo_p = dlQosStrategyLCNode_p->lcInfo_p;

				/* Check the TB is not more than the Maximum supported 
				 * as per UE category
				 */
				if(dlStrategyTxNode_p->maxTBSize < maxTBSize)
				{
                    /* SPR 22539 Fix + */
                    if (ENABLE_ALLOW_ALLOC_BITRATE_TOKEN_BASED == allowAllocBitRate)
                    {
                        if(pCellIndex == servingCellIndex)
                        {
                            servingCellAvailToken_p = &(dlQosStrategyLCNode_p->\
                                    schedulableTokensPcell);
                            servingCellSchedulableBytes_p = &(dlQosStrategyLCNode_p->\
                                    schedulableBytesPcell);

                            userLocationType = ueDLContext_p->userLocationType;
                            transmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;
                        }
                        else
                        {
                            servingCellAvailToken_p = &(dlQosStrategyLCNode_p->\
                                    dLUESCellStartegyNode_p[START_SCELL_INDEX]->schedulableTokensScell);
                            servingCellSchedulableBytes_p = &(dlQosStrategyLCNode_p->\
                                    dLUESCellStartegyNode_p[START_SCELL_INDEX]->schedulableBytesScell);

                            userLocationType = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
                            transmissionMode = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.transmissionMode;
                        }
                        if((RLC_AM_MODE != lcInfo_p->rlcMode) || (!lcInfo_p->dlLcQosInfo.retxnPduSize))
                        {
                            if ( RLC_WAIT_FOR_STATUS_PDU != lcInfo_p->dlLcQosInfo.eRlcAmStatus )
                            {
                                if ( lcInfo_p->queueLoad  > *totalSchBytes_p)
                                {
                                    netQueueLoad = lcInfo_p->queueLoad - *totalSchBytes_p;
                                }
                                else
                                {
                                    //fprintf(stderr,"totalSchBytes_p %d queueLoad %d\n",*totalSchBytes_p,lcInfo_p->queueLoad);
                                    netQueueLoad = 0;
                                }

                                if ( *servingCellAvailToken_p > *servingCellSchedulableBytes_p) 
                                {
                                    netAvlblTkns = *(servingCellAvailToken_p) - 
                                        *servingCellSchedulableBytes_p;
                                }
                                else
                                {
                                    //fprintf(stderr,"*servingCellAvailToken_p %d *servingCellSchedulableBytes_p %d\n",*servingCellAvailToken_p,*servingCellSchedulableBytes_p);
                                    netAvlblTkns = 0;
                                }
                                schedulableBytes = MIN_BETWEEN_TWO(netAvlblTkns, netQueueLoad); 
                            }
                            if ( !*totalSchBytes_p )
                            {
                                schedulableBytes += lcInfo_p->dlLcQosInfo.ctrlQload;
                            }
                        }
                        else
                        {
                            /* Obtain the schedulable bytes equal to 
                             * First Re Tx PDU Qload of the LC
                             */
                            if ( !*totalSchBytes_p )
                            {
                                schedulableBytes = lcInfo_p->dlLcQosInfo.retxnPduSize + 
                                    lcInfo_p->dlLcQosInfo.ctrlQload;
                            }
                        }
                        if (schedulableBytes && (!*servingCellSchedulableBytes_p))
                        {
                            /* Add MAC Subheader and RLC fixed header size */
                            schedulableBytes += MAC_SUBHEADER_LENGTH_THREE_BYTE;
                            if( (transmissionMode == TX_MODE_3) ||
                                    (transmissionMode == TX_MODE_4) ||
                                    (transmissionMode == TX_MODE_8))
                            {
                                schedulableBytes += MAC_SUBHEADER_LENGTH_THREE_BYTE;
                            }
                            schedulableBytes += lcInfo_p->dlLcQosInfo.rlcFixedHeaderSize;
                        }
                        reScheduledBytes = schedulableBytes;
                        //fprintf(stderr,"UE %d LC %d reScheduledBytes %d netAvlblTkns %d netQueueLoad %d TTI %llu \n",ueDLContext_p->ueIndex,lcInfo_p->logicalChannelId,reScheduledBytes,netAvlblTkns,netQueueLoad,getCurrentTick());

                    }
                    else
                    {
					/* Obtain the schedulable bytes equal to maximum of 
					 * {queueLoad , previously Scheduled Bytes} of the LC
					 */
					schedulableBytes = lcInfo_p->queueLoad + 
						lcInfo_p->dlLcQosInfo.ctrlQload +
						lcInfo_p->dlLcQosInfo.retxnPduSize;

					/* Add MAC Subheader and RLC fixed header size */
					schedulableBytes += MAC_SUBHEADER_LENGTH_THREE_BYTE;
					schedulableBytes += 2;
					reScheduledBytes = schedulableBytes -
						dlQosStrategyLCNode_p->schedulableBytes;
                    }
                    /* SPR 22539 Fix - */

					if((dlStrategyTxNode_p->maxTBSize + reScheduledBytes) >
							maxTBSize)
					{
						reScheduledBytes = maxTBSize -
							dlStrategyTxNode_p->maxTBSize;
#ifdef PERF_STATS
						/* + SPR 11214 Fix */                            
						updatePerformanceStats( ueDLContext_p, servingCellIndex);
						/* - SPR 11214 Fix */
#endif
						if (!dlQosStrategyLCNode_p->schedulableBytes)
						{
							if(((RLC_TM_MODE != lcInfo_p->rlcMode)&& (reScheduledBytes <=
											(UInt32)(MAC_SUBHEADER_LENGTH_THREE_BYTE +
											lcInfo_p->dlLcQosInfo.rlcFixedHeaderSize))) ||
									((RLC_TM_MODE == lcInfo_p->rlcMode)&&
									 (MAC_SUBHEADER_LENGTH_THREE_BYTE >= reScheduledBytes)))
							{
								reScheduledBytes = 0;
							}
						}
					}

					if(0 < reScheduledBytes)
					{
						/* Increment the Max TB Size and  Strategy LC node 
						 * with the schedulable bytes for the LC
						 */
						harqProcess_p = dlStrategyTxNode_p->harqProcess_p;
						if(((harqProcess_p->dlHARQTBOneInfo.txIndicator == NEW_TX) && 
									(harqProcess_p->dlHARQTBTwoInfo.txIndicator == RE_TX)) ||
								((harqProcess_p->dlHARQTBOneInfo.txIndicator == RE_TX) &&
								 (harqProcess_p->dlHARQTBTwoInfo.txIndicator == NEW_TX)))

						{
							if (NEW_TX ==  harqProcess_p->dlHARQTBOneInfo.txIndicator && dlCQIInfo_p != PNULL)
							{
								deltaRB = 
									DETERMINE_RBs_DL(
											dlCQIInfo_p->mcsIndexCodeWordOne, 
											reScheduledBytes,servingCellIndex); 
							}
							else if (NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator && dlCQIInfo_p != PNULL)
							{
								deltaRB = 
									DETERMINE_RBs_DL(
											dlCQIInfo_p->mcsIndexCodeWordTwo, 
											reScheduledBytes,servingCellIndex);
							}
							if ((dlStrategyTxNode_p->totalRBMIMO  + deltaRB ) >= 
									harqProcess_p->assignedResourcesNB)
							{
								totalRbMIMO_g[servingCellIndex] -= 
									(harqProcess_p->assignedResourcesNB - dlStrategyTxNode_p->totalRBMIMO);                                            
								dlStrategyTxNode_p->totalRBMIMO = 
									harqProcess_p->assignedResourcesNB;
							}
							else
							{
								totalRbMIMO_g[servingCellIndex] -= deltaRB;
								dlStrategyTxNode_p->totalRBMIMO += deltaRB;
							}
						}
						else
						{
							dlStrategyTxNode_p->maxTBSize += reScheduledBytes;

							requiredRB =
								dlQosGetResBlksForScheduledBytes(
										maxTBSize, 
										dlStrategyTxNode_p->maxTBSize, 
										harqProcess_p,
                                        /* +- SPR 17777 */
										/*4x4 DL MIMO CHG START*/
										ueDLContext_p->ueCategory,
										/*4x4 DL MIMO CHG END*/
										/* SPR 10143 fix start */
                                        /* +- SPR 17777 */
#ifdef TDD_CONFIG
										subFrameNum,
										traversalType,
#endif
                                        /* +- SPR 17777 */
										/* SPR 10143 fix end */
										servingCellIndex
										);

							deltaRB = requiredRB - 
								dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB;

							/* If scheduling for 100 RB.s is not crossed */
							if( numRBsAvailableForScheduling_g[servingCellIndex][userLocationType] > 
									unusedDLStrategyBufferRB_g[servingCellIndex] )
							{
								tempRB = numRBsAvailableForScheduling_g[servingCellIndex][userLocationType] - 
									unusedDLStrategyBufferRB_g[servingCellIndex];
								if(tempRB >= deltaRB)
								{
									dlStrategyTxNode_p->minTBSize += reScheduledBytes;
									/* update the RB.s required for the Data scheduled */
									harqProcess_p->assignedResourcesNB = requiredRB;
								}
								else
								{
									/* + CQI_5.0 */
									dlStrategyTxNode_p->minTBSize += 
										determineTBSize( 
												harqProcess_p->dlHARQTBOneInfo.mcsIndex,
												tempRB,
												/*4X4 DL MIMO CHG START*/
												TB_MAPPED_TO_ONE_LAYER);
									/*4X4 DL MIMO CHG END*/   
									/* update the RB.s required for the Data scheduled
									 */
									harqProcess_p->assignedResourcesNB += tempRB;

									if(deltaRB - tempRB > unusedDLStrategyBufferRB_g[servingCellIndex])
									{
										unusedDLStrategyBufferRB_g[servingCellIndex] = 0;
										deltaRB = numRBsAvailableForScheduling_g[servingCellIndex][userLocationType];
									}
									else
									{
										unusedDLStrategyBufferRB_g[servingCellIndex] -= (deltaRB - tempRB);
									}
								}
							}
							else
							{
								deltaRB = numRBsAvailableForScheduling_g[servingCellIndex][userLocationType];
								unusedDLStrategyBufferRB_g[servingCellIndex] = 0; 
							}
						}

						LOG_UT(MAC_RB_TEST_BEFORE_ALLOCATE_RB_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
								currentGlobalTTITickCount,
								numRBsAvailableForScheduling_g[servingCellIndex][CC_CE_REGION],
								dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB,
								userLocationType,
								ueIndex,
								requiredRB,
								DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "RB info \
								for Qos Schduling");
						numRBsAvailableForScheduling_g[servingCellIndex][userLocationType] -= deltaRB;


						RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(deltaRB,numRBsAvailableForScheduling_g[servingCellIndex]);

						LOG_UT(MAC_RB_INFO_RETRAVERSAL_AFTER_ALLOCATE_RB_IN_DL,LOGDEBUG,
								MAC_DL_Strategy,
								currentGlobalTTITickCount,
								numRBsAvailableForScheduling_g[servingCellIndex][CC_CE_REGION],
								numRBsAvailableForScheduling_g[servingCellIndex][CE_REGION],
								userLocationType,
								deltaRB,
								dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated,
								DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "RB info\
								for Qos Schduling");

						/* Insert the Lc Node in the scheduled LC list only if 
						 * LC is considered for scheduling first time on any cell */
						if( (!dlQosStrategyLCNode_p->schedulableBytes) && 
								(GET_CURRENT_TICK_FOR_CELL(internalCellIndex) != lcInfo_p->lastScheduledTick))
						{
							/* No need to update schedulableBytes here as it will be
							 * taken care at MUX */

							lcInfo_p->lastScheduledTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
#ifdef MAC_CA_WORKAROUND
if(ueDLContext_p->scheduledLcCount == 0)
#endif
{
							 ueDLContext_p->scheduledLcCount++;
				                //fprintf(stderr,"Error scheduledLcCount %d %s %d at %llu\n",ueDLContext_p->scheduledLcCount,__func__,__LINE__,getCurrentTick());

							LOG_MAC_MSG(MAC_INSERT_SCH_LIST_NODE,LOGBRIEF,
									MAC_MUX,currentGlobalTTITickCount, __LINE__,
									(UInt32)ueDLContext_p->ueIndex, dlQosStrategyLCNode_p->lcId,
									ueDLContext_p->scheduledLcCount,
									DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
									DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

							listInsertNode(&ueDLContext_p->dlScheduledLcList, 
									&dlQosStrategyLCNode_p->scheduledLcAnchor);
}

							/* CA Stats Changes Start */
							LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC
								(ueIndex,(GET_CURRENT_TICK_FOR_CELL(internalCellIndex) + 1), 
								 dlQosStrategyLCNode_p,lcInfo_p, (&lcInfo_p->dlLcQosInfo), 
								 servingCellIndex);
							/* CA Stats Changes End */
						}
						else
						{
							/* No need to update schedulableBytes here as it will be
							 * taken care at MUX */

							LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC_SCHEDULED_BYTES(
									ueDLContext_p->ueIndex,
									dlQosStrategyLCNode_p->lcId,
									dlQosStrategyLCNode_p->schedulableBytes, 
									servingCellIndex);
							/* CA Stats Changes End */
						}
                        /* SPR 22539 Fix + */
                        if (ENABLE_ALLOW_ALLOC_BITRATE_TOKEN_BASED == allowAllocBitRate)
                        {
                            *servingCellSchedulableBytes_p += reScheduledBytes;
                            *totalSchBytes_p += reScheduledBytes;
                        }
                        /* SPR 22539 Fix - */

						/* CA Stats Changes Start */
						LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC_RESCHEDULED_BYTES(
								ueDLContext_p->ueIndex,
								dlQosStrategyLCNode_p->lcId,
								reScheduledBytes, servingCellIndex);
						/* CA Stats Changes End */
					}
				}
			}
		}

		/* EICIC +*/
		if ( VICTIM_UE != ueTypeToScheduleDL_g[internalCellIndex])
		{
			/* Get the next node from the TREE traversing IN-ORDER */
			dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode(
					internalCellIndex,
					FALSE /*reStartTraversing*/);
		}
		else
		{
			/* Get the next node from the TREE traversing IN-ORDER */
			dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode(
					internalCellIndex,
					FALSE /*reStartTraversing*/);

		}
		/* EICIC-*/

	}/* End of while */
}/* End of the function dlQosReTraversePriorityLcList */

/*Complexity Start*/
DLQosStrategyLCNode*  eicicUpdate_ueTypeToScheduleDL_g(
		UInt8 isCaEnabled,
		InternalCellIndex internalCellIndex)
{
	DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
	/* EICIC Optimization +- */
	if (isEicicEnabled_g[internalCellIndex])
	{

	UInt8 isflag_Set = FALSE;
	UInt8 cellIndex = 0;

	if(isCaEnabled)
	{
		/* Update Preferred cell list of All UEs considered for  
		 * MAC CE scheduling in either preferredCellList */
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++ )
		{
			if (VICTIM_UE == ueTypeToScheduleDL_g[cellIndex] )
			{
				isflag_Set = TRUE;
				break;
			}
			else
			{
				isflag_Set  = FALSE;
			}
		}

		if (!isflag_Set)
		{
			for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++ )
			{
				if (NON_VICTIM_UE == ueTypeToScheduleDL_g[cellIndex] )
				{
					isflag_Set = TRUE;
					break;
				}
				else
				{
					isflag_Set  = FALSE;
				}
			}

		}

	}
	if (isflag_Set)
	{
		if ( NON_VICTIM_UE == ueTypeToScheduleDL_g[cellIndex])
		{
			/* Get first LC node from the Priority LC Tree */   
			/* internalCellIndex would be used only in non-CA case */

			dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( cellIndex, 
					TRUE);
		}
		else if (VICTIM_UE == ueTypeToScheduleDL_g[cellIndex])
		{
			/* Get first LC node from the Priority LC Tree */   
			/* internalCellIndex would be used only in non-CA case */

			dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode(cellIndex, 
					TRUE);

		}

	}
	else
	{
		if ( NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex])
		{
			/* Get first LC node from the Priority LC Tree */   
			/* internalCellIndex would be used only in non-CA case */

			dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( internalCellIndex, 
					TRUE);
		}
		else if (VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex])
		{
			/* Get first LC node from the Priority LC Tree */   
			/* internalCellIndex would be used only in non-CA case */
			dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode(internalCellIndex, 
					TRUE);

		}

	}
	}
	else
	{
		/* Get first LC node from the Priority LC Tree */   
		/* internalCellIndex would be used only in non-CA case */
		dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode(internalCellIndex, 
				TRUE);
	}
	/* EICIC Optimization +- */
	return dlQosStrategyLCNode_p;
}
/*Complexity End*/


/**********************************************************************************
 * Function Name  : dlQosStrategyProcessExplicitDCIDLQueueForNonCAUes 
 * Inputs         : explicitDlDciQCount - This is Count of the total nodes
 *                                        present in the
 *                                        explicitDLPucchTpcScheduleQueue_g,
 *                  numberOfAvailableRBs_p - pointer to current available RB's,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  tempExplicitDLDCIScheduleQueue_p - Pointer to explicit
 *                                                     DL_PUCCH_TPC_SCHEDULE_TYPE,
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : It will allocate RBs to PendingTA/FreshTAQueue Nodes &
 *                  place the DL Strategy O/P either in the PDCCHNewTXQueue_g
 *                  or PDCHHReTXQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the TA Queues Nodes
 *                  fresh and Pending both . Allocate RBs to the UEs with non
 *                  Zero Queue Load otherwise add then in scheduler Array.
 *******************************************************************************/
/*SPR 22919 Start*/
 void dlQosStrategyProcessExplicitDCIDLQueueForNonCAUes( UInt32 explicitDlDciQCount,
/*SPR 22919 End*/
		SInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE * tempExplicitDLDCIScheduleQueue_p,
		UInt8   *numDlUEsToBeScheduled_p,
		InternalCellIndex internalCellIndex)
{
	UInt8 numDlUEsToBeScheduled = 0;
	SInt8 numDlUEsStarted = 0;
	UInt16 ueIndex = 0; 
	ExplicitDLDCIQueueNode *node_p      = PNULL;
	DLUEContext *ueDLContext_p          = PNULL;
	DLUEContextInfo * ueDLContextInfo_p = PNULL;
	DLStrategyTxNode * dlStrategyTxNode_p = PNULL;
	/* ICIC Change Start */
	UInt8 userLocationType = CC_USER;

	/* get total number of UEs that can be scheduled */
	numDlUEsToBeScheduled = *numDlUEsToBeScheduled_p; 

	numDlUEsToBeScheduled = (explicitDlDciQCount < numDlUEsToBeScheduled) ?
		explicitDlDciQCount : numDlUEsToBeScheduled;

	numDlUEsStarted = numDlUEsToBeScheduled;

	/* ICIC Changes Starts    */
	while ((numDlUEsToBeScheduled)&&(numberOfAvailableRBs_p[CC_CE_REGION] &&
				(explicitDlDciQCount)))
		/* ICIC Changes Ends    */
	{
		/* +COVERITY 29845 */
		if (MAC_FAILURE == DEQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(
					tempExplicitDLDCIScheduleQueue_p,ExplicitDLDCIQueueNode, (void **)&node_p))
		{
			node_p = PNULL;
		}
		/* -COVERITY 29845 */

		if (node_p)
		{
			ueIndex = node_p->ueIndex;    
			ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
			if (!(ueDLContextInfo_p->pendingDeleteFlag))
			{
				ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
				/* ICIC Change Start */
				userLocationType = ueDLContext_p->userLocationType;
				/* ICIC Change End */
				/*EICIC +*/                
				/*Complexity Start*/ 
				if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))||((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{              
					if(!ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(tempExplicitDLDCIScheduleQueue_p,
								ExplicitDLDCIQueueNode,&(node_p->nodeAnchor)))
					{
						FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
					}
					explicitDlDciQCount--;
					continue;

				}
				/*Complexity End*/
				/* EICIC -*/
				/* \code
				 * if schType == SPS_SCHEDULING, then do not perform
				 * scheduling for this UE.
				 * */
				if( SPS_SCHEDULING == ueDLContext_p->schType )
				{
					if(!(ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],
									ExplicitDLDCIQueueNode, (void *) node_p)))
					{
						FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
					}
					explicitDlDciQCount--;
					continue;
				}

				/*Checking if Already being considered for RB Distribution*/
				if (DL_QOS_ALREADY_TRANSMITED < ueDLContext_p->scheduleFlag) 
				{
					/* Processing of UE when Queue Load is Zero but 
					   nack Queue Load iS non Zero*/
					/* ICIC Change Start */
					/* Check if no cell edge ue available at that time and in new traversal there is a
					 * cell edge ue and number of required available rb not available */

					if ((!numberOfAvailableRBs_p[userLocationType])  || 
							((!maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]) && CE_USER == userLocationType))
					{
						if(!(ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],
										ExplicitDLDCIQueueNode, (void *) node_p)))
						{
							lteWarning("Error in Pushing in tempExplicitDLDCIScheduleQueue"
									"  funtion %s\n",
									__func__);
							FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);

						}
						explicitDlDciQCount--;
						continue;                    

					}    
					/* ICIC Change End */

					/* + SPR 9653 Changes */ 
					dlStrategyTxNode_p = 
						dlQosStrategyUpdateNewTxUEList(ueDLContext_p,TPC_MSG, NEW_TRAVERSAL,
								/* + TM7_8 Changes Start */
								currentGlobalTTITickCount,
								internalCellIndex);
					/* - TM7_8 Changes End */
					/* - SPR 9653 Changes */ 

					if(PNULL != dlStrategyTxNode_p)
					{
						ueDLContext_p->dlStrategyTxNode_p = dlStrategyTxNode_p;
						dlStrategyTxNode_p->minTBSize =  LENGTH_OF_TPC;
						dlStrategyTxNode_p->maxTBSize =  LENGTH_OF_TPC;

						ueDLContext_p->scheduleFlag = DL_QOS_ALREADY_TRANSMITED;
						/* ICIC changes starts */                        
						/* According to user location , subtract from those available RB */
						numberOfAvailableRBs_p[userLocationType] -= MIN_RB_FOR_TPC_ONLY;

						/* then update RBs for both cell centered and cell edge */
						RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(MIN_RB_FOR_TPC_ONLY,numberOfAvailableRBs_p);
						/* ICIC changes ends */                        

						dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB = 
							MIN_RB_FOR_TPC_ONLY;

						LOG_MAC_MSG(DL_EXPLICIT_Q_PROCESS_ID, LOGDEBUG, MAC_UL_PC,
								0,
								ueIndex,
								explicitDlDciQCount,
								MIN_RB_FOR_TPC_ONLY,
								dlStrategyTxNode_p->harqProcessId,
								DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "DL_EXPLICIT_QOS_Q_PROCESS");


					}
					else
					{

						/* If there are no FREE HARQ Process to schedule UE
						 * get the next node and continue 
						 */
						LOG_MAC_MSG(MAC_ALL_HARQ_ARE_BUSY,LOGFATAL,MAC_DL_Strategy,
								currentGlobalTTITickCount,
								ueIndex,DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "No FREE HARQ Available");

						FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
						explicitDlDciQCount--;
						continue;
					}


				}/*end of already considered flag check, The UE which are 
				   already considered will be deleted automatically */
				else 
				{
					FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
					explicitDlDciQCount--;
					continue;
				}
			}
			else
			{
				LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,LOGDEBUG,
						MAC_DL_Strategy,
						currentGlobalTTITickCount,
						ueIndex,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME,"");


				FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
				explicitDlDciQCount--;
				continue; 
			}
			FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
			explicitDlDciQCount--;
			numDlUEsToBeScheduled--;

			/* ICIC Change Start */
			/* Update Cell Edge Ue */
			if ( CE_USER == userLocationType )
			{
				--maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex];
			}

			/* ICIC Change End */
		}
		else
		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,LOGDEBUG,
					MAC_DL_Strategy,
					currentGlobalTTITickCount,
					explicitDlDciQCount,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");

			break;
		}

	}

	/* get number of UEs scheduled */
	numDlUEsStarted -= numDlUEsToBeScheduled;

	/* Update the scheduled UE count */
	*numDlUEsToBeScheduled_p -= numDlUEsStarted;

}
/* SPR# 5189 Changes Starts  */
/*****************************************************************************
 * Function Name  : handleMIMORetxForOneTBInQos 
 * Inputs         : ueDLContext_p - DL UE Context,
 * Outputs          harqProcess_p - Pointer to HarqProcess,
 *                  dlStrategyTxNode_p - Pointer to DLStrategyTxNode 
 * Returns        : None
 * Description    : This function will check whether the scheduledLcCount is 0 
 *                  or not. If it is 0, then only TB for which Re-tx is
 *                  scheduled will be transmitted.
 *****************************************************************************/
void handleMIMORetxForOneTBInQos(DLUEContext * ueDLContext_p,
		DLHARQProcess *harqProcess_p,
		DLStrategyTxNode * dlStrategyTxNode_p)
{
	if(!ueDLContext_p->scheduledLcCount)
	{
		if((NEW_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator )&&
				(RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator))
		{
			/* SPR 6136 start */
			harqProcess_p->isTBOneValid = FALSE;
			harqProcess_p->dlHARQTBOneInfo.txIndicator = INVALID_TX;
			dlStrategyTxNode_p->transmissionType = RE_TX;
			/* SPR 6136 end */
		}
		else if((NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator )&&
				(RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator))
		{
			/* SPR 6136 start */
			harqProcess_p->isTBTwoValid = FALSE;
			harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;
			dlStrategyTxNode_p->transmissionType = RE_TX;
			/* SPR 6136 end */
		}
	} 
	else
	{
		if((RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator)||
				(RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator))
		{
			dlStrategyTxNode_p->transmissionType = RE_TX;
		}
	}    
}    
/* SPR# 5189 Changes Ends  */
/*START : DRX_MAC_CE*/
/***********************************************************************************
 * Function Name  : dlQosStrategyProcessDRXQueueForNonCAUes 
 * Inputs         : drxQueueCount - This is Count of the total nodes
 *                                  present in the dlDRXQueue_g or pendingDRXQueue,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  tempDRXQueue_p - Pointer to DRX_Q_TYPE,
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : It will allocate RBs to PendingDRX/FreshDRXQueue Nodes &
 *                  place the DL Strategy O/P either in the PDCCHNewTXQueue_g
 *                  or PDCHHReTXQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the DRX Queues Nodes
 *                  fresh and Pending both . Allocate RBs to the UEs with non
 *                  Zero Queue Load otherwise add then in scheduler Array.
 *****************************************************************************/
/*SPR 22919 Start*/
 void dlQosStrategyProcessDRXQueueForNonCAUes( UInt32 drxQueueCount,
/*SPR 22919 End*/
		SInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		DRX_Q_TYPE * tempDRXQueue_p,
		UInt8   *numDlUEsToBeScheduled_p,
		InternalCellIndex internalCellIndex)
{
	UInt8 numDlUEsToBeScheduled           = 0;
	SInt8 numDlUEsStarted                 = 0;
	UInt16 ueIndex                        = 0; 
	DRXQueueNode *drxNode_p               = PNULL;
	DLUEContext *ueDLContext_p            = PNULL;
	/*Drx Cmd Qos Fix Start*/
	ULUEContext *ueULContext_p            = PNULL;
	ULUEContextInfo * ueULContextInfo_p   = PNULL;
	/*Drx Cmd Qos Fix End*/
	DLUEContextInfo * ueDLContextInfo_p   = PNULL;
	DLStrategyTxNode * dlStrategyTxNode_p = PNULL;
	/* ICIC Change start */
	UInt8 userLocationType                = CC_USER;
	/* ICIC change End */

	numDlUEsToBeScheduled         = *numDlUEsToBeScheduled_p;

	numDlUEsToBeScheduled = (drxQueueCount < numDlUEsToBeScheduled) ?
		drxQueueCount : numDlUEsToBeScheduled;

	numDlUEsStarted = numDlUEsToBeScheduled;


	/* ICIC Changes starts    */ 
	while ((numDlUEsToBeScheduled)&&(numberOfAvailableRBs_p[CC_CE_REGION]) &&
			(drxQueueCount))
		/* ICIC Changes Ends      */
	{
		/*START : DRX_CMD_DL_UL_SPLIT_FIX*/
		DEQUEUE_DRX_Q(tempDRXQueue_p,DRXQueueNode,(void **)&(drxNode_p));
		/*END   : DRX_CMD_DL_UL_SPLIT_FIX*/

		if (drxNode_p)
		{
			ueIndex = drxNode_p->ueIdx;    
			ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
			/*Drx Cmd Qos Fix Start*/
			ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
			/*Drx Cmd Qos Fix End*/
			if (!(ueDLContextInfo_p->pendingDeleteFlag))
			{
				ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
				/* EICIC +*/             
				/*Complexity Start*/             
				if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType)) ||((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{   	            
					if(!ENQUEUE_DRX_Q(tempDRXQueue_p,DRXQueueNode, (void *) drxNode_p))
					{
						lteWarning("Error in Pushing in Pending"
								" DRXQueue in funtion %s\n",
								__func__);
						FREE_MEM_NODE_DRX_Q(drxNode_p);
					}
					drxQueueCount--;
					continue;
				}
				/*Complexity End*/
				/* EICIC -*/
				/*Drx Cmd Qos Fix Start*/
				ueULContext_p = ueULContextInfo_p->ulUEContext_p;
				/*Drx Cmd Qos Fix End*/
				if ((DL_QOS_ALREADY_TRANSMITED < ueDLContext_p->scheduleFlag)&& 
						/*Drx Cmd Qos Fix Start*/
						(ueDLContext_p->ueQueueLoad == 0)&&
						(ueULContext_p->bsrNetTotal == 0))
					/*Drx Cmd Qos Fix End*/
				{

					/* ICIC changes start */
					userLocationType = ueDLContext_p->userLocationType;
					if ((!numberOfAvailableRBs_p[userLocationType])
							|| ((!maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] && CE_USER == userLocationType)))
					{
						LOG_MAC_MSG(MAC_RB_NOT_AVAILABLE_FOR_RETX,LOGFATAL,MAC_DL_Strategy,
								currentGlobalTTITickCount,
								ueIndex,DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "");

						if(!ENQUEUE_DRX_Q(pendingDRXQueue_gp[internalCellIndex],DRXQueueNode, (void *) drxNode_p))
						{
							lteWarning("Error in Pushing in Pending"
									" DRXQueue in funtion %s\n",
									__func__);
							FREE_MEM_NODE_DRX_Q(drxNode_p);
						}
						drxQueueCount--;
						continue;
					}
					/* ICIC Change End */

					/* + SPR 9653 Changes */ 
					dlStrategyTxNode_p = 
						/*QOS_CYCLIC_COMPLEXITY_CHG*/
						dlQosStrategyUpdateNewTxUEList(ueDLContext_p,DRX_MSG,NEW_TRAVERSAL,
								/* + TM7_8 Changes Start */
								currentGlobalTTITickCount,
								internalCellIndex);
					/* - TM7_8 Changes End */
					/* - SPR 9653 Changes */ 
					/*QOS_CYCLIC_COMPLEXITY_CHG*/
					/* ueDLContext_p->isAlreadyConsideredFlag = dlReferenceFlag;*/
					if(PNULL != dlStrategyTxNode_p)
					{
						ueDLContext_p->dlStrategyTxNode_p = dlStrategyTxNode_p;
						/* + SPR 22352 fix */ 
						ueDLContext_p->scheduleFlag = DL_QOS_NO_NEW_TRANSMISSION;

						updateRbAndTBSizeInDlStrategyNode(DRX_MSG,dlStrategyTxNode_p,
								numberOfAvailableRBs_p,userLocationType,ueIndex);
						/* - SPR 22352 fix */ 

					}
					else
					{
						/* If there are no FREE HARQ Process to schedule UE
						 * get the next node and continue 
						 */
						LOG_MAC_MSG(MAC_ALL_HARQ_ARE_BUSY,LOGFATAL,MAC_DL_Strategy,
								currentGlobalTTITickCount,
								ueIndex,DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "No FREE HARQ Available");
						/* SPR_9737 Changes Start */
						if(!ENQUEUE_DRX_Q(pendingDRXQueue_gp[internalCellIndex],DRXQueueNode, (void *) drxNode_p))
						{
							lteWarning("Error in Pushing in Pending"
									" DRXQueue in funtion %s\n",
									__func__);
							FREE_MEM_NODE_DRX_Q(drxNode_p);
						}
						drxQueueCount--;
						/* SPR_9737 Changes End */
						continue;
					}


				}/*end of already considered flag check, The UE which are 
				   already considered will be deleted automatically */
				else 
				{
					/*Drx Cmd Qos Fix Start*/
					FREE_MEM_NODE_DRX_Q(drxNode_p);
					/*Drx Cmd Qos Fix End*/
					drxQueueCount--;
					continue;
				}
			}
			else
			{
				LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,LOGDEBUG,
						MAC_DL_Strategy,
						currentGlobalTTITickCount,
						ueIndex,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME,"");
				freeMemPool((void *)drxNode_p);
				drxNode_p = PNULL;
				drxQueueCount--;
				continue; 
			}
			freeMemPool((void *)drxNode_p);
			drxNode_p = PNULL;
			drxQueueCount--;
			numDlUEsToBeScheduled--;
			/* ICIC Change Start */
			/* Update Cell Edge Ue */
			if ( CE_USER == userLocationType )
			{
				--maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] ;
			}   
			/* ICIC Change End */
		}
		else
		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,LOGDEBUG,
					MAC_DL_Strategy,
					currentGlobalTTITickCount,
					drxQueueCount,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");

			break;
		}

	};

	/* get number of UEs scheduled */
	numDlUEsStarted -= numDlUEsToBeScheduled;

	/* Update the scheduled UE count */
	*numDlUEsToBeScheduled_p -= numDlUEsStarted;

}
/*END   : DRX_MAC_CE*/

/* Cyclomatic Complexity changes - starts here */

/*****************************************************************************
 * Function Name  : dlQosUpdateHarqContext
 * Inputs         : ueDLContext_p - DL UE Context,
 * Outputs        : dlStrategyTxNode_p - Pointer to DLStrategyTxNode,
 *                  harqProcess_p - Pointer to HarqProcess
 *                  internalCellIndex - cell index used at MAC.
 * Returns        : None
 * Description    : This function will update the DL Harq context information
 *****************************************************************************/
static  void dlQosUpdateHarqContext( DLStrategyTxNode * dlStrategyTxNode_p,
		DLUEContext * ueDLContext_p,       
		DLHARQProcess * harqProcess_p,
		InternalCellIndex internalCellIndex)

{
	ResourceAllocatorInput *tempResourceAllocatorInput_p = PNULL;
	tempResourceAllocatorInput_p = &dlStrategyTxNode_p->resourceAllocInput;

	tempResourceAllocatorInput_p->requiredRB = 
		harqProcess_p->assignedResourcesNB;

	/* SPR 4704 Start */
	UInt32 tbSize = 0;
	UInt8 transmissionMode = INVALID_TX_MODE; 
	UInt16* maxTBSize_p = PNULL;

	if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
	{
		transmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;
		maxTBSize_p = &(ueDLContext_p->maxTBSize);
	}
	else
	{
		transmissionMode = 
			ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.transmissionMode;
		maxTBSize_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->maxTBSize);
	}
	/** SPR 11158 Changes Start **/
	if (( dlStrategyTxNode_p->msgType == SPATIAL_MUL ) &&
			/* + TM6_5.2 */
			((TX_MODE_6 != transmissionMode) && (TX_MODE_7 != transmissionMode)))
		/** SPR 11158 Changes End **/
		/* - TM6_5.2 */
	{
		tbSize = determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
				harqProcess_p->assignedResourcesNB,
				/*4X4 DL MIMO CHG START*/
				TB_MAPPED_TO_ONE_LAYER
				);/*4X4 DL MIMO CHG END*/
		/* SPR 6136 start */
		if (RE_TX == harqProcess_p->dlHARQTBOneInfo.txIndicator || RE_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
		{
			// do nothing
		}    
		/* SPR 11184 Changes Start */
		/*klockwork warning fix*/
		else if (( tbSize > ( ueDLContext_p->ueQueueLoad+((MAC_MIN_SDU_SIZE*(ueDLContext_p->scheduledLcCount)))))&&
				(transmissionMode < MAX_TRANSMISSION_MODES) && (macParams_g.preferredPrimaryDCIFormatForSingleTB[transmissionMode] == DCI_FORMAT_1A))
			/* SPR 11184 Changes End */
			/* SPR 6136 end */   
		{
			dlStrategyTxNode_p->msgType = NON_SPATIAL_MUL;

			harqProcess_p->isTBOneValid = TRUE;
			harqProcess_p->isTBTwoValid = FALSE;

			harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;
			harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;

			/*DCI Format Calculation*/
			dlStrategyTxNode_p->primaryDCIFormat=DCI_FORMAT_1A;
			dlStrategyTxNode_p->secondaryDCIFormat=DCI_FORMAT_1A;

			*maxTBSize_p = ueDLContext_p->maxSISOTBSize;
		}
		/* SPR# 5189 Changes Starts */
		handleMIMORetxForOneTBInQos(ueDLContext_p, harqProcess_p, dlStrategyTxNode_p);
		/* SPR# 5189 Changes Ends */
	}
	/* SPR 4704 End */

	/* + CQI_5.0 */
	if ( TRUE == harqProcess_p->isTBOneValid && 
			TRUE == harqProcess_p->isTBTwoValid)
	{
		/*4X4 DL MIMO CHG START*/ 
		/* SPR 11355 Fix Start */
		if(((ueDLContext_p->ueCategory == UE_CAT_5 )|| (ueDLContext_p->ueCategory == UE_CAT_8) )
				&& harqProcess_p->numOfLayer >= 3)
			/* SPR 11355 Fix End */
		{
			if(harqProcess_p->numOfLayer == 4)
			{
				harqProcess_p->dlHARQTBOneInfo.tbSize    =
					determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
							harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_TWO_LAYER);
			}
			else
			{
				harqProcess_p->dlHARQTBOneInfo.tbSize    =
					determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
							harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_ONE_LAYER); 
			} 
			harqProcess_p->dlHARQTBTwoInfo.tbSize    =
				determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
						harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_TWO_LAYER);
		}
		else
		{    
			harqProcess_p->dlHARQTBOneInfo.tbSize    =  
				determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
						harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_ONE_LAYER);

			harqProcess_p->dlHARQTBTwoInfo.tbSize    =  
				determineTBSize(harqProcess_p->dlHARQTBTwoInfo.mcsIndex,
						harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_ONE_LAYER);
		}
	}/*4X4 DL MIMO CHG END*/    
	else if (TRUE == harqProcess_p->isTBOneValid)
	{  /*4X4 DL MIMO CHG START*/  
		harqProcess_p->dlHARQTBOneInfo.tbSize    =  
			determineTBSize(harqProcess_p->dlHARQTBOneInfo.mcsIndex,
					harqProcess_p->assignedResourcesNB,TB_MAPPED_TO_ONE_LAYER);
		/*4X4 DL MIMO CHG END*/     
	}   
	/* - CQI_5.0 */

}
/*****************************************************************************
 * Function Name  : dlQosValidatePriorityTreeNode
 * Inputs         : ueDLContext_p - DL UE Context,
 *                  dlQosStrategyLCNode_p - Pointer to DLQosStrategyLCNode,
 *                  currentGlobalTTITickCount - Global TTI Tick
 *                  internalCellIndex - cell index used at MAC,
 *                  gbrSchedulingFlag - GBR Scheduling status Flag.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will validate the strategy node
 *****************************************************************************/
STATIC  MacRetType dlQosValidatePriorityTreeNode(DLUEContext * ueDLContext_p,
		DLQosStrategyLCNode * dlQosStrategyLCNode_p ,
        /* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		InternalCellIndex internalCellIndex,
		UInt8 gbrSchedulingFlag)
{
	DLLogicalChannelInfo *lcInfo_p = PNULL; 

	if(PNULL == ueDLContext_p)
	{
		/*Coverity 530 CID 42174 */
		LOG_MAC_MSG(MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID,LOGFATAL,MAC_DL_Strategy,
				currentGlobalTTITickCount,
				__LINE__,dlQosStrategyLCNode_p->ueIndex,
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
				"Ue Index is null and a node is present in Tree");

		/*Coverity 530 CID 42174 */
		return MAC_FAILURE ;

	}
	else	
	{
		/* SPR 4751 changes */
		if(UL_SYNC_LOSS_ON == checkUlSyncLossForUE(ueDLContext_p,
					currentGlobalTTITickCount,ueDLContext_p->internalCellIndex))
		{
			return  MAC_FAILURE;

		}
		/* SPR 4751 changes */
	}
	/* SPR 4946 Start */
	/* SPR 7469 changes start */

	lcInfo_p = dlQosStrategyLCNode_p->lcInfo_p;

	if ( ( (DL_QOS_ALREADY_TRANSMITED > ueDLContext_p->scheduleFlag) &&
				/* + SPR 12229 */\
				(IS_DL_QOS_ALREADY_TRANSMITED_ON_SCELL(ueDLContext_p)) 
	     )|| 
			/* - SPR 12229 */\
			( ( RLC_WAIT_FOR_STATUS_PDU == lcInfo_p->dlLcQosInfo.eRlcAmStatus) &&
			  ( !lcInfo_p->dlLcQosInfo.retxnPduSize ) && 
			  ( !lcInfo_p->dlLcQosInfo.ctrlQload ) 
			)
	   )
	{
		/* SPR 7469 changes end */
		/* SPR 4946 End */        
		/* If there are no tokens for the LC 
		 * get the next node and continue 
		 */
		LOG_UT(MAC_LC_NODE_NOT_PROCESSED_IN_DL_LOG_ID,LOGDEBUG,MAC_DL_Strategy,
				currentGlobalTTITickCount,
				ueDLContext_p->ueIndex,dlQosStrategyLCNode_p->lcId,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME, " scheduleFlag is greater than DL_QOS_ALREADY_TRANSMITED");
		return  MAC_FAILURE;

	}

	if( (TRUE == gbrSchedulingFlag ) &&
			(NON_GBR == lcInfo_p->dlLcQosInfo.channelType))
	{
		return  MAC_FAILURE;
	}

	/* SPS_CHG */
	/*! \fn MacRetType  dlQosValidatePriorityTreeNode(DLUEContext * ueDLContext_p,
	 *                               DLQosStrategyLCNode * dlQosStrategyLCNode_p, 
     *              tickType_t currentGlobalTTITickCount)
	 *  \brief Function Modification
	 * */

	if(IS_PCELL_OF_UE(ueDLContext_p,internalCellIndex))
	{
		if( ( SPS_ACTIVATED == ueDLContext_p->spsDlInfo.spsState ) &&
				(FALSE == lcInfo_p->isSpsLc ))
		{
			/*! \code
			 * Handling of strictSpsResourceUsageForSpsLc for Qos 
			 * If (UE State == SPS_ACTIVATED) and 
			 *    (Current TTI correspond to SPS Occasion)
			 * \endcode
			 */
			UInt8 dlSpsInterval = 0;
			LP_SpsCommIntervalProfile commSpsIntervalProfile_p =PNULL;
			dlSpsInterval = ueDLContext_p->spsDlInfo.spsDlSetupInfo_p->\
					semiPersistentSchedIntervalDL;
			commSpsIntervalProfile_p = 
				&cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
				initParams_p->spsCommIntervalProfile[dlSpsInterval];

			if(commSpsIntervalProfile_p->strictSpsResourceUsageForSpsLc)
			{
				/*! \code
				 * If strictSpsResourceUsageForSpsLc is TRUE, 
				 *      Do not multiplex SPS LC with any other LC
				 * Else, do the normal processing
				 * \endcode
				 */
				if( SPS_SCHEDULING == ueDLContext_p->schType )
				{
					return MAC_FAILURE;
				}
			}
		}
		else if( SPS_SCHEDULING == ueDLContext_p->schType ) 
		{
			return MAC_FAILURE;
		}
	}
	/* SPS_CHG */


	return MAC_SUCCESS;

}
/*****************************************************************************
 * Function Name  : dlQosDetermineTbSizeForNewTx
 * Inputs         : ueDLContext_p - DL UE Context,
 *                  harqProcess_p - Pointer to HarqProcess,
 *                  numAvailableRBs - Number of available RB's,
 *                  schedulableBytes,
 *                  subFrameNum - Subframe number,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : deltaRB
 * Description    : This function will determine the TB sixe for New TX 
 *****************************************************************************/
static  SInt8 dlQosDetermineTbSizeForNewTx(DLUEContext *ueDLContext_p,
		DLHARQProcess *harqProcess_p,
		SInt32 numAvailableRBs,
		UInt32 schedulableBytes,
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
		UInt8 subFrameNum,
#endif
        /* +- SPR 17777 */
		InternalCellIndex internalCellIndex)
{
	SInt8              deltaRB            = 0;
	DLStrategyTxNode*  dlStrategyTxNode_p = PNULL;
	DLCQIInfo*         dlCQIInfo_p        = PNULL;

	if(IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
	{
		dlStrategyTxNode_p  = ueDLContext_p->dlStrategyTxNode_p;
		dlCQIInfo_p         = &(ueDLContext_p->dlCQIInfo);
	}
	else
	{
		dlStrategyTxNode_p  = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p;
		dlCQIInfo_p         = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
	}


	/* SPR 6136 start */
	if( (SPATIAL_MUL == dlStrategyTxNode_p->msgType) &&
			(((harqProcess_p->dlHARQTBOneInfo.txIndicator == NEW_TX) &&
			  (harqProcess_p->dlHARQTBTwoInfo.txIndicator == RE_TX)) ||
			 ((harqProcess_p->dlHARQTBOneInfo.txIndicator == RE_TX) &&
			  (harqProcess_p->dlHARQTBTwoInfo.txIndicator == NEW_TX))))
	{
		if (NEW_TX ==  harqProcess_p->dlHARQTBOneInfo.txIndicator)
		{
			deltaRB = 
				DETERMINE_RBs_DL(
						dlCQIInfo_p->mcsIndexCodeWordOne, 
						schedulableBytes,
						internalCellIndex);
		}
		else if (NEW_TX ==  harqProcess_p->dlHARQTBTwoInfo.txIndicator)
		{
			deltaRB = 
				DETERMINE_RBs_DL(
						dlCQIInfo_p->mcsIndexCodeWordTwo, 
						schedulableBytes,
						internalCellIndex);
		}
		dlStrategyTxNode_p->totalRBMIMO += deltaRB;
		if (dlStrategyTxNode_p->totalRBMIMO >= harqProcess_p->assignedResourcesNB)
		{	
			dlStrategyTxNode_p->totalRBMIMO = harqProcess_p->assignedResourcesNB;
		}
		totalRbMIMO_g[internalCellIndex] -= deltaRB;
	}
	else
	{
		deltaRB = dlQosCalculateMinMaxTBSize(
				dlStrategyTxNode_p,
				schedulableBytes,
                /* +- SPR 17777 */
#ifdef TDD_CONFIG
				subFrameNum,
#endif
                /* +- SPR 17777 */
				numAvailableRBs,
				internalCellIndex );
	}
	/* SPR 6136 end */
	return deltaRB;
}

#ifdef PERF_STATS
/*****************************************************************************
 * Function Name  : updatePerformanceStats
 * Inputs         : ueDLContext_p - DL UE Context,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function will update the performance statistics 
 *****************************************************************************/
/* + SPR 11214 Fix */
static  void updatePerformanceStats(DLUEContext * ueDLContext_p, InternalCellIndex servingCellIndex)
{
	UInt16 ueIndex = ueDLContext_p->ueIndex;
	UInt8 isTBOneValid     = 0;
	UInt8 isTBTwoValid     = 0;
	UInt32 maxRestrictedTB = 0;

	if(IS_PCELL_OF_UE(ueDLContext_p, servingCellIndex))    
	{
		isTBOneValid = ueDLContext_p->dlStrategyTxNode_p->harqProcess_p->isTBOneValid;
		isTBTwoValid = ueDLContext_p->dlStrategyTxNode_p->harqProcess_p->isTBTwoValid;
		maxRestrictedTB = ueDLContext_p->maxRestrictedTB;
	}
	else
	{
		isTBOneValid = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p->harqProcess_p->isTBOneValid;
		isTBTwoValid = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p->harqProcess_p->isTBTwoValid;
		maxRestrictedTB = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->maxRestrictedTB;
	}

	if ( isTBOneValid == TRUE && isTBTwoValid == TRUE )
	{
		if (ueDLContext_p->maxMIMOTBSize < maxRestrictedTB)
		{
			/*  number of ms where respective UE is limited in the
			 *       DL direction by its UE capability */
			gMacUePerfStats_p[ueIndex].lteUeMacSchedDLPerfStats.\
				totalTimeRestrictUeInDlbyUeCap++;
		}
	}
	else 
	{
		if((TRUE == isTBOneValid) && (ueDLContext_p->maxSISOTBSize < maxRestrictedTB))
		{
			/*  number of ms where respective UE is limited in the
			 *       DL direction by its UE capability */
			gMacUePerfStats_p[ueIndex].lteUeMacSchedDLPerfStats.\
				totalTimeRestrictUeInDlbyUeCap++;
		}
	}

}
/* - SPR 11214 Fix */
#endif

/* Cyclomatic Complexity changes - ends here */
/*CA chngs start */
/*****************************************************************************
 * Function Name  : updateTopSchedulableUEInfoList
 * Inputs         : internalCellIndex - InternalCellIndex
 * Outputs        : topSchedulableUE_g list for each cell 
 * Returns        : None
 * Description    : This function will traverse the lc node tree 
 *                  and manintain a topSchedulableUE_g list for each cell.
 *****************************************************************************/
void updateTopSchedulableUEInfoList(InternalCellIndex internalCellIndex)
{
	DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
	UInt8 cellIdx = 0;
	UInt8 reStartTraversing = TRUE;
	UInt8 localArr[MAX_NUM_UE] = {0};
	DLUEContextInfo *dlUEContextInfo_p = PNULL;
	DLUEContext *ueDLContext_p = PNULL;
	UInt16 maxUEsToBeScheduledDL = 0;
    /* SPR 16116 fix start */
    UInt32 numUEProcessed = 0;
    UInt32 tsIndex = 0;
    UInt32 isTopSchedulableUEListFull = FALSE;
    /* SPR 16116 fix end */



	/*traversing all lcTreeNodes for each cell*/
	dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode(internalCellIndex,reStartTraversing);
	if(dlQosStrategyLCNode_p == NULL)
	{
		return;
	}
	/*Code Review Comments Changes Start*/
	for(cellIdx = 0; cellIdx <= MAX_NUM_SCELL; cellIdx++)
	{
		topSchedulableUEs_g[cellIdx].arrCount = 0;
	}
	/*Code Review Comments Changes End*/
	cellIdx = getCellIndexOfPriorityLcNode(dlQosStrategyLCNode_p);
	/*klockwork warning fix*/
	if(cellIdx >= MAX_NUM_CELL)
	{
		return;
	}
	maxUEsToBeScheduledDL = cellSpecificParams_g.cellConfigAndInitParams_p\
				[cellIdx]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL; 

	reStartTraversing = FALSE;
	/*Cov_fix_28April_62168_start*/

    /* SPR 16116 fix start */
    while ((cellIdx <= MAX_NUM_SCELL) && (numUEProcessed < MAX_TOP_SCHEDULABLE_UE_LIST_SIZE ))
    {
        /* Check if all the topSchedulableUEs_g[] have sufficient number of entries.
           If yes, then break   */
        for(tsIndex = 0; tsIndex <= MAX_NUM_SCELL; tsIndex++)
        {
            if(topSchedulableUEs_g[tsIndex].arrCount >= maxUEsToBeScheduledDL)
            {
                isTopSchedulableUEListFull = TRUE;
            }
            else
	{
                isTopSchedulableUEListFull = FALSE;
                break;
            }
        }
        if(isTopSchedulableUEListFull)
	{
            break;
        }
        /* SPR 16116 fix end */

		/*Cov_fix_28April_62168_stop*/    
		if (localArr[dlQosStrategyLCNode_p->ueIndex] != 1)
		{    
			dlUEContextInfo_p = &dlUECtxInfoArr_g[dlQosStrategyLCNode_p->ueIndex];
			if(dlUEContextInfo_p == PNULL || dlUEContextInfo_p->pendingDeleteFlag)
			{
                /* SPR 16116 fix start */
                break;
                /* SPR 16116 fix end */
			}
			ueDLContext_p = dlUEContextInfo_p->dlUEContext_p;
            /* SPR 16116 fix start */
            /* Only consider those UEs that are not in DRX/MeasGap/Syncloss states*/
            if(ueDLContext_p->scheduleFlag >= DL_QOS_NO_NEW_TRANSMISSION)
            {
                UInt32 arrCount = topSchedulableUEs_g[cellIdx].arrCount;

                topSchedulableUEs_g[cellIdx].topSchedulableUEInfo[arrCount].ueIndex = dlQosStrategyLCNode_p->ueIndex;
                topSchedulableUEs_g[cellIdx].topSchedulableUEInfo[arrCount].totLCWeight = dlQosStrategyLCNode_p->totLCWeight;  
			topSchedulableUEs_g[cellIdx].arrCount++;

			localArr[dlQosStrategyLCNode_p->ueIndex] = 1;
                numUEProcessed++;
            }
            /* SPR 16116 fix end */

		}
		dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode(internalCellIndex,reStartTraversing);  
		if(dlQosStrategyLCNode_p == NULL)
		{
            /* SPR 16116 fix start */
            break;
            /* SPR 16116 fix end */
		}
		cellIdx = getCellIndexOfPriorityLcNode(dlQosStrategyLCNode_p);
		/*spr 12560 fix start*/
		if(cellIdx > MAX_NUM_SCELL)
		{
            /* SPR 16116 fix start */
            break;
            /* SPR 16116 fix end */
		}
		/*spr 12560 fix end*/
		maxUEsToBeScheduledDL = cellSpecificParams_g.cellConfigAndInitParams_p\
					[cellIdx]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL; 
	}
    /* SPR 16116 fix start */
    for(tsIndex = 0; tsIndex <= MAX_NUM_SCELL; tsIndex++)
    {
        maxUEsToBeScheduledDL = cellSpecificParams_g.cellConfigAndInitParams_p[tsIndex]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL;
        if(topSchedulableUEs_g[tsIndex].arrCount >= maxUEsToBeScheduledDL)
        {
            topSchedulableUEs_g[tsIndex].arrCount = maxUEsToBeScheduledDL;
        }
    }
    /* SPR 16116 fix end */

	/* EICIC + */
	/* +++++++++++++++++++++++++++++++++ */
	/* Updating top list for victim UEs as well */

	reStartTraversing = TRUE;
	/*Complexity Start*/
	updateTopSchedulableVictimUEInfoList(internalCellIndex,reStartTraversing,localArr);
}

/*****************************************************************************
 * Function Name  : checkTopSchedulableUEInfoList 
 * Inputs         : cellId : InternalCellIndex
 *                  ueId   : ue index 
 *                  totLCWeightWithLP : total LC weight with lowest MCS priority
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will check the top schedulable list and 
 *                  returns the succcess if list has empty index or has entry 
 *                  for the ue index passed. 
 *****************************************************************************/
 MacRetType checkTopSchedulableUEInfoList( InternalCellIndex cellId,
		UInt32 ueId,
                                                 /* SPR 16116 fix start */
                                                 UInt32 totLCWeightWithLP)
                                                 /* SPR 16116 fix end */
{
	UInt16 maxUEsToBeScheduledDL = 0;
	DLUEContext *ueDLContext_p          = PNULL;
	DLUEContextInfo * ueDLContextInfo_p = PNULL;

	/* EICIC +*/
	ueDLContextInfo_p = &dlUECtxInfoArr_g[ueId];
	ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;        
	/* EICIC -*/

	if ( VICTIM_UE != ueDLContext_p->userType)
	{
		maxUEsToBeScheduledDL = cellSpecificParams_g.cellConfigAndInitParams_p\
					[cellId]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL; 
		/*klockworks warning fix*/ 
		/*Coverity_Warning_Fix_Start*/
		if(maxUEsToBeScheduledDL <= MAX_DL_UE_SCHEDULED) 
		{
			if((topSchedulableUEs_g[cellId].arrCount  < (maxUEsToBeScheduledDL * 2)) || \
                    (topSchedulableUEs_g[cellId].topSchedulableUEInfo[(maxUEsToBeScheduledDL * 2) - 1].totLCWeight < totLCWeightWithLP))
			{
				return MAC_SUCCESS; 
			}
            else if(topSchedulableUEs_g[cellId].topSchedulableUEInfo[(maxUEsToBeScheduledDL * 2) - 1].totLCWeight == totLCWeightWithLP)
			{
				if(topSchedulableUEs_g[cellId].topSchedulableUEInfo[(maxUEsToBeScheduledDL * 2) - 1].ueIndex == ueId)
				{
					return MAC_SUCCESS;
				}
				else
				{
					return MAC_FAILURE;
				}
			}
			else
			{
				return MAC_FAILURE;
			}
		}
		else
		{
			return MAC_FAILURE;
		}
	}
	else
	{
		maxUEsToBeScheduledDL = cellSpecificParams_g.cellConfigAndInitParams_p\
					[cellId]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL; 

		if( (topSchedulableVictimUEs_g[cellId].arrCount  < (maxUEsToBeScheduledDL * 2)) || \
                (topSchedulableVictimUEs_g[cellId].topSchedulableUEInfo[(maxUEsToBeScheduledDL * 2) - 1].totLCWeight < totLCWeightWithLP))
		{
			return MAC_SUCCESS; 
		}
        else if(topSchedulableVictimUEs_g[cellId].topSchedulableUEInfo[(maxUEsToBeScheduledDL * 2) - 1].totLCWeight == totLCWeightWithLP)
		{
			if(topSchedulableVictimUEs_g[cellId].topSchedulableUEInfo[(maxUEsToBeScheduledDL * 2) - 1].ueIndex == ueId)
			{
				return MAC_SUCCESS;
			}
			else
			{
				return MAC_FAILURE;
			}
		}
		else
		{
			return MAC_FAILURE;
		}
	}
	/*Coverity_Warning_Fix_End*/    
}
/*CA chngs end */

/********************************************************************************
 * Function Name  : dlQosInitStrategyLocalParams 
 * Inputs         : numberOfAvailableRBs_p 
 *                : gbrSchedulingFlag_p
 *                : numDlUEsToBeScheduled_p 
 *                : internalCellIndex 
 * Outputs        : 
 * Returns        : None
 * Description    : This func will initialize the variables local to 
 *                  strategy required before scheduling UEs in any Serving Cell.
 ********************************************************************************/
 void dlQosInitStrategyLocalParams( SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
		UInt8  *gbrSchedulingFlag_p,
		UInt8  *numDlUEsToBeScheduled_p,
		InternalCellIndex internalCellIndex)
{
	InternalCellIndex  cellIndex                 = 0;
	UInt8              strategyBufferRB          = 0;

	if(!IS_CA_ENABLED())
	{
		strategyBufferRB = cellSpecificParams_g.\
				   cellConfigAndInitParams_p[internalCellIndex]->\
				   initParams_p->dlSchdConfig.strategyBufferRB;

		numRBsAvailableForScheduling_g[internalCellIndex][CC_CE_REGION] = 
			numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] + strategyBufferRB;
		numRBsAvailableForScheduling_g[internalCellIndex][CE_REGION] = 
			numberOfAvailableRBs[internalCellIndex][CE_REGION] + strategyBufferRB;

		unusedDLStrategyBufferRB_g[internalCellIndex] = strategyBufferRB;
		/* EICIC +*/
		if (VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex])
		{
			/* This is done since initial when only SRB channels are added aggGBR is 0 */
			if ( !dlAggregateGbrPerTTI_g[internalCellIndex][VICTIM_UE] )
			{
				*gbrSchedulingFlag_p = FALSE;
			} 
		}
		else
		{
			/* This is done since initial when only SRB channels are added aggGBR is 0 */
			if ( !dlAggregateGbrPerTTI_g[internalCellIndex][NON_VICTIM_UE] )
			{
				*gbrSchedulingFlag_p = FALSE;
			} 
		}
		/* EICIC -*/
		numDlUEsRemainingToSchd_g[internalCellIndex] = 
			numDlUEsToBeScheduled_p[internalCellIndex];
	}
	else
	{
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
		{
			if (CELL_PHY_RUNNING == cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->cellState)
			{
				strategyBufferRB = cellSpecificParams_g.\
						   cellConfigAndInitParams_p[cellIndex]->\
						   initParams_p->dlSchdConfig.strategyBufferRB;

				numRBsAvailableForScheduling_g[cellIndex][CC_CE_REGION] = 
					numberOfAvailableRBs[cellIndex][CC_CE_REGION] + strategyBufferRB;
				numRBsAvailableForScheduling_g[cellIndex][CE_REGION] = 
					numberOfAvailableRBs[cellIndex][CE_REGION] + strategyBufferRB;

				unusedDLStrategyBufferRB_g[cellIndex] = strategyBufferRB;

				numDlUEsRemainingToSchd_g[cellIndex] = 
					numDlUEsToBeScheduled_p[cellIndex];
			}
		}
		/* EICIC +*/
		if  (VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex])
		{
			if ( !totalDlAggregateGbrPerTTI_g[VICTIM_UE] )
			{
				*gbrSchedulingFlag_p = FALSE;
			}
		}
		else
		{
			if (!totalDlAggregateGbrPerTTI_g[NON_VICTIM_UE] )
			{
				*gbrSchedulingFlag_p = FALSE;
			}
		}

		/* EICIC -*/
	}
}

/*****************************************************************************
 * Function Name  : getCellIndexOfPriorityLcNode 
 * Inputs         : dlQosStrategyLCNode_p  
 * Outputs        : 
 * Returns        : priorityLcNodeCellIndex 
 * Description    : This func will return serving cell index of the node 
 *                  which is responsible for Priority LC node insertion 
 *                  in that cell at higher priority.
 *****************************************************************************/
 InternalCellIndex getCellIndexOfPriorityLcNode( DLQosStrategyLCNode *dlQosStrategyLCNode_p)
{
	UInt8              sCellIdx                 = START_SCELL_INDEX;
	DLUEContextInfo*   ueDLContextInfo_p        = PNULL;
	DLUEContext*       ueDLContext_p            = PNULL;
	InternalCellIndex  priorityLcNodeCellIndex  = INVALID_CELL_INDEX;

	ueDLContextInfo_p   = &dlUECtxInfoArr_g[dlQosStrategyLCNode_p->ueIndex];
	if(!(ueDLContextInfo_p->pendingDeleteFlag))
	{
		ueDLContext_p       = ueDLContextInfo_p->dlUEContext_p;
		/* Compare totLCWeight with 
		 * lcInfo_p->dlLcQosInfo->qosPriority of each cell */
		if(dlQosStrategyLCNode_p->totLCWeight ==
				dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.qosPriority[PCELL_SERV_CELL_IX])
		{
			/* Considered qosPriority[PCELL_SERV_CELL_IX] as PCell QosPriority */
			priorityLcNodeCellIndex = ueDLContext_p->internalCellIndex;
		}
		else
		{
			/* otherwise, Check all Scells */
			for(sCellIdx = START_SCELL_INDEX; sCellIdx <= ueDLContext_p->scellCount; sCellIdx++)
			{
				if(dlQosStrategyLCNode_p->totLCWeight == 
						dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.qosPriority[sCellIdx])
				{
					priorityLcNodeCellIndex = 
						ueDLContext_p->dlUeScellContext_p[sCellIdx]->internalCellIndex;
					break;
				}
			}
		}
	}
	/* return internalCellIndex of the matching cell */
	return priorityLcNodeCellIndex;

}

/*****************************************************************************
 * Function Name  : dlQosGetLoadDiffBetweenCells 
 * Inputs         : ueDLContext_p 
 * Outputs        : 
 * Returns        : absolute load difference b/w two cells 
 * Description    : Currently this function will return load difference b/w 
 *                  two serving Cells only.
 *****************************************************************************/
 UInt8 dlQosGetLoadDiffBetweenCells(DLUEContext* ueDLContext_p)
{
	UInt8               currentLoadCellOne  = 0;
	UInt8               currentLoadCellTwo  = 0;
	InternalCellIndex   pCellIndex          = 0;  
	InternalCellIndex   sCellIndex          = 0; 

	pCellIndex    = ueDLContext_p->internalCellIndex;
	sCellIndex    = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex;

	currentLoadCellOne = GET_CA_DLPRB_USAGE(pCellIndex);
	currentLoadCellTwo = GET_CA_DLPRB_USAGE(sCellIndex);

	return abs_wrapper(currentLoadCellOne - currentLoadCellTwo);
}

/*****************************************************************************
 * Function Name  : dlQosGetMcsDiffBetweenCells 
 * Inputs         : ueDLContext_p 
 * Outputs        : 
 * Returns        : absolute MCS difference b/w two cells 
 * Description    : Currently this function will return MCS difference b/w
 *                  two serving Cells only.
 *****************************************************************************/
 UInt8 dlQosGetMcsDiffBetweenCells(DLUEContext* ueDLContext_p)
{
	UInt8               cellOneMcs     = 0;    
	UInt8               cellTwoMcs     = 0;

	/* Currently Minimum(Mcs_CW1 , Mcs_CW2) is considered for comparison */
	cellOneMcs = dlQosGetPCellMcs( ueDLContext_p);

	cellTwoMcs = dlQosGetSCellMcs( 
			ueDLContext_p, 
			ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex);

	return abs_wrapper(cellOneMcs - cellTwoMcs);
}

/*****************************************************************************
 * Function Name  : dlQosAddCellInPreferredCellList 
 * Inputs         : pCellIndex 
 *                  internalCellIndex
 * Outputs        : 
 * Returns        : None 
 * Description    : This func will be used to insert cellIndex in
 *                  preferredCellList.
 *****************************************************************************/
 void dlQosAddCellInPreferredCellList( InternalCellIndex pCellIndex, 
		ServingCellIndex servingCellIndex)
{
    /* +- SPR 18268 */
	UInt16 currentUeCount  = dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount;
    /* +- SPR 18268 */

	UInt8 listIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellList[currentUeCount].count;

	dlQoSPreferredCellList_g[pCellIndex].preferredCellList[currentUeCount].\
		preferredCellListInfo[listIndex].cellIndex = servingCellIndex;

	dlQoSPreferredCellList_g[pCellIndex].preferredCellList[currentUeCount].count++;
}

/*****************************************************************************
 * Function Name  : dlQosGetLoadWisePreferredCellList 
 * Inputs         : ueDLContext_p 
 * Outputs        : 
 * Returns        : None 
 * Description    : This func will create preferredCellList based on current
 *                  load of the cells.
 *****************************************************************************/
 void dlQosGetLoadWisePreferredCellList(DLUEContext* ueDLContext_p )
{
	UInt8               currentLoadPcell  = 0;
	UInt8               currentLoadScell  = 0;
	InternalCellIndex   pCellIndex        = 0;  
	InternalCellIndex   sCellIndex        = 0; 

	pCellIndex    = ueDLContext_p->internalCellIndex;
	sCellIndex    = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex;

	/* Currently no need to check for multiple cells*/
	currentLoadPcell = GET_CA_DLPRB_USAGE(pCellIndex);
	currentLoadScell = GET_CA_DLPRB_USAGE(sCellIndex);

	if(currentLoadPcell <= currentLoadScell)
	{
		dlQosAddCellInPreferredCellList(pCellIndex, pCellIndex);
		dlQosAddCellInPreferredCellList(pCellIndex, sCellIndex);
	}
	else
	{
		dlQosAddCellInPreferredCellList(pCellIndex, sCellIndex);
		dlQosAddCellInPreferredCellList(pCellIndex, pCellIndex);
	}
	LOG_MAC_MSG(MAC_LOAD_WISE_PREF_CELL_LIST,LOGDEBUG,MAC_DL_Strategy,
			getCurrentTick(),
			ueDLContext_p->ueIndex,
			pCellIndex,
			currentLoadPcell,
			currentLoadScell,
			macReconfigSchedulerParamsDL_gp[pCellIndex][schParamsIndexDL_g[pCellIndex]].caParams.loadDisparityThreshold,
			DEFAULT_FLOAT_VALUE,
			DEFAULT_FLOAT_VALUE,
			FUNCTION_NAME, "");
}

/*****************************************************************************
 * Function Name  : dlQosGetMcsWisePreferredCellList 
 * Inputs         : ueDLContext_p 
 * Outputs        : 
 * Returns        : None 
 * Description    : This func will create preferredCellList based on MCS
 *                  priority for a UE on all serving cells.
 *****************************************************************************/
 void dlQosGetMcsWisePreferredCellList(DLUEContext* ueDLContext_p)
{
	InternalCellIndex cellIdWithHighestMcs = INVALID_CELL_INDEX;

	InternalCellIndex pCellIndex  = ueDLContext_p->internalCellIndex; 
	InternalCellIndex sCellIndex  = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex;

	cellIdWithHighestMcs = getCellIdWithHighestMcs(ueDLContext_p);

	dlQosAddCellInPreferredCellList(pCellIndex, cellIdWithHighestMcs);

	/* Currently no need to check for multiple cells*/
	if(cellIdWithHighestMcs == pCellIndex)    
	{
		dlQosAddCellInPreferredCellList(pCellIndex, sCellIndex);
	}
	else
	{
		dlQosAddCellInPreferredCellList(pCellIndex, pCellIndex);
	}
	LOG_MAC_MSG(MAC_MCS_WISE_PREF_CELL_LIST,LOGDEBUG,MAC_DL_Strategy,
			getCurrentTick(),
			ueDLContext_p->ueIndex,
			pCellIndex,
			cellIdWithHighestMcs,
			macReconfigSchedulerParamsDL_gp[pCellIndex][schParamsIndexDL_g[pCellIndex]].caParams.mcsDiffThreshold,
			0,
			DEFAULT_FLOAT_VALUE,
			DEFAULT_FLOAT_VALUE,
			FUNCTION_NAME, "");
}

/*****************************************************************************
 * Function Name  : dlQosPreparePreferredCellsList 
 * Inputs         : ueDLContext_p 
 *                : priorityLcNodeCellIndex     
 * Outputs        : 
 * Returns        : None 
 * Description    : This func will create preferredCellList based on either 
 *                  current load/MCS and determine is any preference is 
 *                  required among cells or not
 *****************************************************************************/
 UInt8 dlQosPreparePreferredCellsList( DLUEContext* ueDLContext_p,  
		InternalCellIndex priorityLcNodeCellIndex)
{
	UInt8               isPrefRequired  = TRUE;
	UInt8               listIndex       = 0;
	InternalCellIndex   pCellIndex      = 0;  
	InternalCellIndex   sCellIndex      = 0; 

	pCellIndex    = ueDLContext_p->internalCellIndex;
	sCellIndex    = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex;

	/* + SPR 11718 Changes */
	MacReconfigSchedulerParamsDL *schedulerParamsDL_p =
		&(macReconfigSchedulerParamsDL_gp[pCellIndex][schParamsIndexDL_g[pCellIndex]]);
	/* - SPR 11718 Changes */

	UInt8 loadDispThreshold = schedulerParamsDL_p->caParams.loadDisparityThreshold;
	UInt8 mcsDiffThreshold  = schedulerParamsDL_p->caParams.mcsDiffThreshold;

	if( loadDispThreshold <= dlQosGetLoadDiffBetweenCells(ueDLContext_p))
	{
		dlQosGetLoadWisePreferredCellList(ueDLContext_p);
	}
	else if (mcsDiffThreshold <= dlQosGetMcsDiffBetweenCells(ueDLContext_p)) 
	{
		dlQosGetMcsWisePreferredCellList(ueDLContext_p);
	}
	else
	{
		if(pCellIndex == priorityLcNodeCellIndex)
		{
			dlQosAddCellInPreferredCellList(pCellIndex, pCellIndex);
			dlQosAddCellInPreferredCellList(pCellIndex, sCellIndex);
		}
		else
		{
			dlQosAddCellInPreferredCellList(pCellIndex, sCellIndex);
			dlQosAddCellInPreferredCellList(pCellIndex, pCellIndex);
		}
		isPrefRequired = FALSE;
		LOG_MAC_MSG(MAC_PREF_CELL_LIST,LOGDEBUG,MAC_DL_Strategy,
				getCurrentTick(),
				ueDLContext_p->ueIndex,
				pCellIndex,
				priorityLcNodeCellIndex,
				0,
				0,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME, "");
	}

	listIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount;

	dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].isPrefRequired = isPrefRequired;

	dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].ueIndex = ueDLContext_p->ueIndex;

	dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount++;

	return listIndex;
}

/*****************************************************************************
 * Function Name  : dlQosGetMcsInServingCell
 * Inputs         : ueDLContext_p 
 *                : servingCellIndex 
 * Outputs        : 
 * Returns        : mcsIndex
 * Description    : This func will provide the mcsindex of UE in 
 *                  servingCellIndex
 *****************************************************************************/
UInt8 dlQosGetMcsInServingCell( DLUEContext* ueDLContext_p,
		ServingCellIndex servingCellIndex)
{
	UInt8 mcsInCellIndex = 0;

	if(IS_PCELL_OF_UE(ueDLContext_p, servingCellIndex))    
	{
		mcsInCellIndex = dlQosGetPCellMcs(ueDLContext_p);
	}
	else
	{
		mcsInCellIndex = dlQosGetSCellMcs(ueDLContext_p, servingCellIndex);
	}
	return mcsInCellIndex;
}
/*SPR 21948 Changes Start */
/* Changes under spr 21762 have been reverted */
/*SPR 21948 Changes End */
/*****************************************************************************
 * Function Name  : isUeEligibleForCaScheduling 
 * Inputs         : ueDLContext_p
 * Outputs        : 
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This function checks whether for scheduling any Scell of UE 
 *                  is in Active state or not.
 *****************************************************************************/
 MacRetType isUeEligibleForCaScheduling(DLUEContext* ueDLContext_p)
{
	UInt8 cellIdx = START_SCELL_INDEX;
	UInt8 scellArrIndex = INVALID_8_VAL;
	for(cellIdx = PCELL_SERV_CELL_IX; cellIdx <= ueDLContext_p->scellCount; cellIdx++)
	{
		scellArrIndex = getServeCellIndexFromInternalCellId(
				ueDLContext_p, 
				cellIdx);

		if( (INVALID_8_VAL != scellArrIndex) && 
				(IS_SCELL_ACTIVE(ueDLContext_p->ueIndex, scellArrIndex))) 
		{
			return MAC_SUCCESS;
		}
	}
	return MAC_FAILURE;
}

/*****************************************************************************
 * Function Name  : dlQosGetPreferedCellListForScheduling 
 * Inputs         : 
 *                : ueIndex 
 *                : isMacCEScheduled
 * Outputs        : preferredListIndex_p -listIndex of the preferredCellList_g
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This func will create Preferred Cell List for CA & non-CA 
 *                  UEs based on current load & MCS difference b/w Cells.
 *****************************************************************************/
 MacRetType dlQosGetPreferedCellListForScheduling( UInt16 ueIndex,
		UInt8 isMacCEScheduled,
                                                         UInt8 *preferredListIndex_p,
                                                         /* SPR 16116 fix start */
                                                         UInt32 totLCWeightWithLP)
                                                         /* SPR 16116 fix end */
{
	UInt8              listIndex                  = 0;
	UInt8              isUEPresentInTopSchList    = 0;
	UInt8              preferredListCount         = 0;
	InternalCellIndex  cellIdWithHighestMcs       = INVALID_CELL_INDEX;
	InternalCellIndex  pCellIndex                 = INVALID_CELL_INDEX;
	DLUEContextInfo*   ueDLContextInfo_p          = PNULL;
	DLUEContext* ueDLContext_p                    = PNULL; 

	ueDLContextInfo_p   = &dlUECtxInfoArr_g[ueIndex];
	/** SPR 13599 Changes Start **/
	UInt8 isCaEnable = IS_CA_ENABLED();
	if(ueDLContextInfo_p->pendingDeleteFlag)
	{
		return MAC_FAILURE;
	}

	ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

	pCellIndex = ueDLContext_p->internalCellIndex;
	preferredListCount = dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount;

	for(listIndex = 0; listIndex < preferredListCount; listIndex++)
	{
		if(ueIndex == dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].ueIndex)
		{
			/* List is already prepared for this UE */
			*preferredListIndex_p = listIndex;
			return MAC_SUCCESS;
		}
	}

    if( (isCaEnable) &&
            /*SPR 21948 Changes Start */
            /* Changes under spr 21762 have been reverted */
            (isUeEligibleForCaScheduling(ueDLContext_p)))
        /*SPR 21948 Changes End */
        /** SPR 13599 Changes End **/
	{
		/* get cellIndex due to which node is present in priority LC tree */
		cellIdWithHighestMcs = getCellIdWithHighestMcs(ueDLContext_p);

		if (isMacCEScheduled) 
		{
			/* Store index on which UE's PreferredCell List will be created */
			listIndex = dlQosPreparePreferredCellsList( 
					ueDLContext_p, 
					cellIdWithHighestMcs);
		}
		else /*if (!isMacCEScheduled)*/
		{
            /* SPR 16116 fix start */

			/*check UE can be schduled on other cells based on  their topSchdList_g */
			/*To be discussed ---: checkTopSchedulableUE doesn't use ueIndex.... */
			isUEPresentInTopSchList = checkTopSchedulableUEInfoList( 
					!(cellIdWithHighestMcs), 
					ueIndex, 
                                              totLCWeightWithLP);
            /* SPR 16116 fix end */
			if (!isUEPresentInTopSchList) 
			{
				/* Store index on which UE's PreferredCell List will be created */
				listIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount;
				/* Add priorityLcNode CellIndex in prefered cell list */
				dlQosAddCellInPreferredCellList(pCellIndex, cellIdWithHighestMcs);
				dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].isPrefRequired = FALSE;
				dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].ueIndex = ueIndex;
				dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount++;
			}
			else
			{
				/* Store index on which UE's PreferredCell List will be created */
				listIndex = dlQosPreparePreferredCellsList( ueDLContext_p,
						cellIdWithHighestMcs);
			}
		}
	}
	else
	{
		/* Store index on which UE's PreferredCell List will be created */
		listIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount;
		/* Add pCellIndex in prefered cell list */
		dlQosAddCellInPreferredCellList(pCellIndex, pCellIndex);
		dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].isPrefRequired = FALSE;
		dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].ueIndex = ueIndex;
		dlQoSPreferredCellList_g[pCellIndex].preferredCellListCount++;
	}
	*preferredListIndex_p = listIndex;
	return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : dlQosGetPriorityLcTreeNode 
 * Inputs         : reStartTraversing - Flag having reStartTraversing status, 
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : 
 * Returns        : dLQosStrategyLCNode_p
 * Description    : 
 *****************************************************************************/
 DLQosStrategyLCNode* dlQosGetPriorityLcTreeNode( InternalCellIndex internalCellIndex,
		UInt8 reStartTraversing)
{
	/* add multiTree traversal changes */
	static UInt8   workingTreeIndex       = INVALID_8_VAL;
	static DLQosStrategyLCNode *dlQosStrategyLCNodeForCellIndex_p[MAX_NUM_CELL] = {PNULL};
	/** SPR 13599 Changes Start **/
	UInt8 isCaEnable = IS_CA_ENABLED();
	/** SPR 13599 Changes End **/
	DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
	/* if reStartTraversing is True then get the node from strating of the Priority tree */
	if (reStartTraversing == TRUE )
	{
		workingTreeIndex = INVALID_8_VAL;
		/* Get the first node from priority tree */
		dlQosStrategyLCNode_p = dlQosGetFirstNode(dlQosStrategyLCNodeForCellIndex_p,internalCellIndex ,&workingTreeIndex);
	}
	/* if reStartTraversing is False */
	else
	{
		/* CA enable case */
		/** SPR 13599 Changes Start **/
		if(isCaEnable)
			/** SPR 13599 Changes End **/
		{
			dlQosStrategyLCNode_p = dlQosGetNextNode(dlQosStrategyLCNodeForCellIndex_p,&workingTreeIndex);
		}
		else
		{
			if(PNULL != dlQosStrategyLCNodeForCellIndex_p[internalCellIndex])
			{
				dlQosStrategyLCNodeForCellIndex_p[internalCellIndex] = (DLQosStrategyLCNode *)dlQosGetNextLcNode(
						&(dlQosStrategyLCNodeForCellIndex_p[internalCellIndex])->lcTreeAnchor);
				dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[internalCellIndex];
			}
		}
	}
	return dlQosStrategyLCNode_p;
}
/* EICIC +*/
/*****************************************************************************
 * Function Name  : dlQosGetPriorityLcTreeVictimNode 
 * Inputs         : reStartTraversing - Flag having reStartTraversing status, 
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : 
 * Returns        : dLQosStrategyLCNode_p
 * Description    : 
 *****************************************************************************/
DLQosStrategyLCNode* dlQosGetPriorityLcTreeVictimNode( InternalCellIndex internalCellIndex,
		UInt8 reStartTraversing)
{
	/* add multiTree traversal changes */
	static UInt8   workingTreeIndex       = INVALID_8_VAL;
	static DLQosStrategyLCNode *dlQosStrategyLCNodeForCellIndex_p[MAX_NUM_CELL] = {PNULL};
	DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
	/* if reStartTraversing is True then get the node from strating of the Priority tree */
	if (reStartTraversing == TRUE )
	{
		workingTreeIndex = INVALID_8_VAL;
		/* Get the first node from priority tree */
		dlQosStrategyLCNode_p = dlQosGetFirstVictimNode(dlQosStrategyLCNodeForCellIndex_p,internalCellIndex ,&workingTreeIndex);

	}
	/* if reStartTraversing is False */
	else
	{
		/* CA enable case */
		if(IS_CA_ENABLED())
		{
			dlQosStrategyLCNode_p = dlQosGetNextNode(dlQosStrategyLCNodeForCellIndex_p,&workingTreeIndex);
		}
		else
		{
			if(PNULL != dlQosStrategyLCNodeForCellIndex_p[internalCellIndex])
			{
				dlQosStrategyLCNodeForCellIndex_p[internalCellIndex] = (DLQosStrategyLCNode *)dlQosGetNextLcNode(
						&(dlQosStrategyLCNodeForCellIndex_p[internalCellIndex])->lcTreeAnchor);
				dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[internalCellIndex];
			}
		}
	}
	return dlQosStrategyLCNode_p;
}
/* EICIC -*/
/*****************************************************************************
 * Function Name  : dlQosUpdateGbrSchdlAndReStartTraversing 
 * Inputs         : dlQosStrategyLCNode_p,
 *                  gbrSchedulingFlag_p,
 *                  reStartTraversing_p,
 *                : internalCellIndex
 * Outputs        : gbrSchedulingFlag_p
 *                : reStartTraversing_p
 *                : dlQosStrategyLCNode_p
 * Returns        : None 
 * Description    : This func will update whether gbrScheduling is required 
 *                  or not and whether need to traverse all PriorityLCTree
 *                  from the root or from the previous point.
 *****************************************************************************/
 void dlQosUpdateGbrSchdlAndReStartTraversing( DLQosStrategyLCNode** dlQosStrategyLCNode_p,
		UInt8 *gbrSchedulingFlag_p,
		UInt8 *reStartTraversing_p,
		InternalCellIndex internalCellIndex,
		UserType userType)
{
	if((*dlQosStrategyLCNode_p == PNULL) && (*gbrSchedulingFlag_p == TRUE))
	{                    
		*gbrSchedulingFlag_p = FALSE;
		*reStartTraversing_p = TRUE;
		/* Now traverse from starting of Priority LC tree */
		if (VICTIM_UE != userType)
		{
			*dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeNode( 
					internalCellIndex,
					*reStartTraversing_p);
		}
		else if (VICTIM_UE == userType)
		{

			*dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode( 
					internalCellIndex,
					*reStartTraversing_p);

		}
		*reStartTraversing_p = FALSE;
	}
	else if((*gbrSchedulingFlag_p == FALSE) && (*reStartTraversing_p == TRUE))
	{
		*reStartTraversing_p = FALSE;
	}
}

/*****************************************************************************
 * Function Name  : dlQoSPrepareUEStrategyNode 
 * Inputs         : ueDLContext_p
 *                : preferredCellListIndex
 *                : currentGlobalTTITickCount
 *                : servingCellIndex
 *                : dlQosStrategyLCNode_p
 * Outputs        : 
 * Returns        : dlStrategyTxNode_p
 * Description    : This func prepares UE's StrategyTxNode w.r.t provided
 *                  serving cell index.
 *****************************************************************************/
 DLStrategyTxNode* dlQoSPrepareUEStrategyNode( DLUEContext *ueDLContext_p,
        /* +- SPR 17777 */
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		ServingCellIndex servingCellIndex,
		DLQosStrategyLCNode *dlQosStrategyLCNode_p,
		dlQosTraversalType traversalType)
{
	UInt8              userLocationType                = CC_USER;
	DLStrategyTxNode*  dlStrategyTxNode_p              = PNULL;
	DLStrategyTxNode** preferredCellDlStrategyTxNode_p = PNULL;
	DLScheduleFlag*    scheduleFlag_p                  = PNULL;
	UInt16             servingCellAvailToken           = 0;

	if(IS_PCELL_OF_UE(ueDLContext_p, servingCellIndex))    
	{
		preferredCellDlStrategyTxNode_p = &(ueDLContext_p->dlStrategyTxNode_p);
		userLocationType = ueDLContext_p->userLocationType;
		scheduleFlag_p = &(ueDLContext_p->scheduleFlag);
		servingCellAvailToken = dlQosStrategyLCNode_p->\
					schedulableTokensPcell;
	}
	else
	{
		preferredCellDlStrategyTxNode_p = 
			&(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);

		userLocationType = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType; 
		scheduleFlag_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag);
		servingCellAvailToken = dlQosStrategyLCNode_p->\
					dLUESCellStartegyNode_p[START_SCELL_INDEX]->schedulableTokensScell;
	}
	if(*scheduleFlag_p == DL_QOS_ALREADY_TRANSMITED)
        {
           return PNULL; //Abhishek
        }

	if(PNULL == *preferredCellDlStrategyTxNode_p)
	{
		if (numDlUEsRemainingToSchd_g[servingCellIndex])
		{
			if ((!numRBsAvailableForScheduling_g[servingCellIndex][userLocationType]) ||
					( (CE_USER == userLocationType) &&
					  ( (numRBsAvailableForScheduling_g[servingCellIndex][CE_REGION] <= 
					     (unusedDLStrategyBufferRB_g[servingCellIndex]) )||
					    (!maxCellEdgeUEsToBeScheduledDL_g[servingCellIndex]))) ||
					/* + SPR 11656 Changes */
					((NEW_TRAVERSAL == traversalType) && (servingCellAvailToken <= 0)) )
				/* - SPR 11656 Changes */
			{
				LOG_UT(MAC_LC_NODE_NOT_PROCESSED_IN_DL,LOGDEBUG,MAC_DL_Strategy,
						currentGlobalTTITickCount,
						numRBsAvailableForScheduling_g[servingCellIndex][userLocationType],
						maxCellEdgeUEsToBeScheduledDL_g[servingCellIndex],
						userLocationType,
						ueDLContext_p->ueIndex,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME, "RB is not available for the UE or No more Cell\
						edge ue Availbale to Schdule");
				return PNULL;
			}

			dlStrategyTxNode_p = dlQosStrategyUpdateNewTxUEList(
					ueDLContext_p, 
					INVALID_TA_DRX_MSG,
					traversalType,
					currentGlobalTTITickCount,
					servingCellIndex);

			if(PNULL != dlStrategyTxNode_p)
			{
				*preferredCellDlStrategyTxNode_p = dlStrategyTxNode_p;

				numDlUEsRemainingToSchd_g[servingCellIndex]--;
				/* Update Cell Edge Ue */
				if ( CE_USER == userLocationType )
				{    
					--maxCellEdgeUEsToBeScheduledDL_g[servingCellIndex];
				}

				*scheduleFlag_p = DL_QOS_ALREADY_TRANSMITED;

				LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCHEDULE_FLAG(
						ueDLContext_p->ueIndex,
						*scheduleFlag_p);

				LOG_UT(MAC_DL_STRATEGY_TX_NODE_CREATED,LOGDEBUG,MAC_DL_Strategy,
						currentGlobalTTITickCount,
						ueDLContext_p->ueIndex,
						DEFAULT_INT_VALUE,
						(UInt32)dlStrategyTxNode_p,
						*scheduleFlag_p,
						numDlUEsRemainingToSchd_g[servingCellIndex],
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME, "");

			}
			else
			{
				/* If there are no FREE HARQ Process you cannot schedule
				 * get the next node and continue 
				 */
				LOG_MAC_MSG(MAC_ALL_HARQ_ARE_BUSY,LOGFATAL,MAC_DL_Strategy,
						currentGlobalTTITickCount,
						ueDLContext_p->ueIndex,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME, "No FREE HARQ Available");
				return PNULL;
			}
		}
		else
		{
			/* If the number of scheduled for the tick is reached MAX
			 * Then get the next node and continue 
			 */
			LOG_UT(MAC_NUM_UE_PER_TICK_REACHED_IN_DL_LOG_ID,
					LOGDEBUG,MAC_DL_Strategy,
					currentGlobalTTITickCount,
					ueDLContext_p->ueIndex,
					DEFAULT_INT_VALUE, 
					DEFAULT_INT_VALUE, 
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE, 
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME, 
					"Number of schedulable UE reached MAX");
			return PNULL;
		}
	}
	else
	{
		dlQosProcessUeForRBRestriction( traversalType,
				(*preferredCellDlStrategyTxNode_p)->harqProcess_p,
				ueDLContext_p,
				servingCellIndex);
	}
	return *preferredCellDlStrategyTxNode_p; 
}

/*****************************************************************************
 * Function Name  : dlQoSApplyPreferenceFactor 
 * Inputs         : dlQosStrategyLCNode_p
 *                : preferredCellListIndex
 * Outputs        : 
 * Returns        : None 
 * Description    : This func applies preference factor, if required, to the 
 *                  cells to update schedulable tokens divided as per 
 *                  bandwidthDist given from RRM.
 *****************************************************************************/
/* + SPR 13605 Fix */
 void dlQoSApplyPreferenceFactor( DLUEContext* ueDLContext_p, 
		DLQosStrategyLCNode *dlQosStrategyLCNode_p,
		UInt8 preferredCellListIndex,
        UInt32 tempDlAggregateGbrPerTTI[MAX_NUM_CELL][MAX_VALID_UE_TYPE])
    /* - SPR 13605 Fix */
{
	UInt8  prefCellMcsValue                   = 0;
	UInt8  prbUsageWaterMark                  = 0;
	UInt16 maxBorrowableTokens                = 0;
	UInt16 numOfTokensNeedToBorrow            = 0;
	UInt16 numOfTokensBorrowed                = 0;
	/* + SPR 11656 Changes */
	SInt32 prefCellSchedulableTokens          = 0;
	SInt32 othreCellSchedulableTokens         = 0;
	/* - SPR 11656 Changes */
	UInt32 totalSchedulableBytes              = 0;
	UInt32 maxBytesAvailableAsPerRestrictedRb = 0;
	UInt32 maxBytesAvailableAsPerCellLoad     = 0;
	UInt32 availToken                         = 0;
	UInt32 queueLoad   = dlQosStrategyLCNode_p->lcInfo_p->queueLoad;
	DLUESCellStartegyNode*  dLUESCellStartegyNode_p = PNULL;
	InternalCellIndex       preferredCellIndex      = INVALID_CELL_INDEX; 
	InternalCellIndex       pCellIndex              = ueDLContext_p->internalCellIndex; 
	DLStrategyTxNode*       dlStrategyTxNode_p      = PNULL;

	if(dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.availToken > 0)
        {
            availToken  = dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.availToken;
        }
	/* + SPR 11718 Changes */
	MacReconfigSchedulerParamsDL *schedulerParamsDL_p =
		&(macReconfigSchedulerParamsDL_gp[pCellIndex]\
				[schParamsIndexDL_g[pCellIndex]]);
	/* - SPR 11718 Changes */

	prbUsageWaterMark      = schedulerParamsDL_p->caParams.prbUsageWaterMark;                                
	totalSchedulableBytes  = MIN_BETWEEN_TWO(availToken , queueLoad );

	if(totalSchedulableBytes)
	{
		preferredCellIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellList\
				     [preferredCellListIndex].preferredCellListInfo[0].cellIndex;
		/*coverity id 63740*/
		dLUESCellStartegyNode_p = dlQosStrategyLCNode_p->dLUESCellStartegyNode_p[START_SCELL_INDEX];
		if(!dLUESCellStartegyNode_p)
		{
			return ;
		}
		/*coverity id 63740*/
		if(pCellIndex == preferredCellIndex)
		{
			prefCellSchedulableTokens = dlQosStrategyLCNode_p->schedulableTokensPcell;
			prefCellMcsValue = dlQosGetPCellMcs(ueDLContext_p);
			/* + SPR 11656 Changes */
			othreCellSchedulableTokens = dLUESCellStartegyNode_p->schedulableTokensScell;
			/* - SPR 11656 Changes */
			/* + SPR 13605 Fix */
			dlStrategyTxNode_p = ueDLContext_p->dlStrategyTxNode_p;
			/* - SPR 13605 Fix */
		}
		else
		{
			prefCellSchedulableTokens = dLUESCellStartegyNode_p->schedulableTokensScell;
			prefCellMcsValue = dlQosGetSCellMcs( ueDLContext_p, preferredCellIndex);
			/* + SPR 11656 Changes */
			othreCellSchedulableTokens = dlQosStrategyLCNode_p->schedulableTokensPcell;
			/* - SPR 11656 Changes */
			/* + SPR 13605 Fix */
			dlStrategyTxNode_p = (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);
			/* - SPR 13605 Fix */
		}

		/* + SPR 13605 Fix */
		if( (dlStrategyTxNode_p == PNULL) && 
				(numDlUEsRemainingToSchd_g[preferredCellIndex] == 0) )
		{
			return ;
		}           
		/* - SPR 13605 Fix */
		/* + SPR 11656 Changes */
		if(othreCellSchedulableTokens > 0)
		{
			/* - SPR 11656 Changes */
			maxBytesAvailableAsPerRestrictedRb = determineTBSize(
					prefCellMcsValue, 
					maxRestrictedRbDL_g[preferredCellIndex],
					TB_MAPPED_TO_ONE_LAYER);

			if(prefCellSchedulableTokens < (SInt64)totalSchedulableBytes)
			{
				if(prefCellSchedulableTokens < (SInt64)maxBytesAvailableAsPerRestrictedRb)
				{
					maxBytesAvailableAsPerCellLoad = determineTBSize(
							prefCellMcsValue,
							(prbUsageWaterMark - 
							 GET_CA_DLPRB_USAGE(preferredCellIndex)),
							TB_MAPPED_TO_ONE_LAYER);

					maxBorrowableTokens = MIN_BETWEEN_TWO(
							maxBytesAvailableAsPerRestrictedRb,
							maxBytesAvailableAsPerCellLoad);

					numOfTokensNeedToBorrow = 
						(totalSchedulableBytes - prefCellSchedulableTokens);
					/* + SPR 11656 Changes */
					numOfTokensBorrowed = MIN_BETWEEN_THREE(
							maxBorrowableTokens , 
							numOfTokensNeedToBorrow,
							othreCellSchedulableTokens); 
					/* - SPR 11656 Changes */
					if(pCellIndex == preferredCellIndex)
					{
						dLUESCellStartegyNode_p->schedulableTokensScell  -= numOfTokensBorrowed;
						dlQosStrategyLCNode_p->schedulableTokensPcell    += numOfTokensBorrowed;
					}
					else
					{
						dLUESCellStartegyNode_p->schedulableTokensScell  += numOfTokensBorrowed;
						dlQosStrategyLCNode_p->schedulableTokensPcell    -= numOfTokensBorrowed;
					}

					/* + SPR 13605 Fix */
					/* EICIC +*/
					if ( NON_VICTIM_UE == ueTypeToScheduleDL_g[ueDLContext_p->internalCellIndex])
					{
						if(QOS_GBR == dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.channelType)
						{
							tempDlAggregateGbrPerTTI[preferredCellIndex][NON_VICTIM_UE] += numOfTokensBorrowed;
                            if(tempDlAggregateGbrPerTTI[!preferredCellIndex][NON_VICTIM_UE] < numOfTokensBorrowed)
                            {
                                tempDlAggregateGbrPerTTI[!preferredCellIndex][NON_VICTIM_UE] = 0;
                            }
                            else
                            {
							tempDlAggregateGbrPerTTI[!preferredCellIndex][NON_VICTIM_UE] -= numOfTokensBorrowed;
						}
					}
                    }
					if ( VICTIM_UE == ueTypeToScheduleDL_g[ueDLContext_p->internalCellIndex])
					{
						if(QOS_GBR == dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.channelType)
						{
							tempDlAggregateGbrPerTTI[preferredCellIndex][VICTIM_UE] += numOfTokensBorrowed;
                            if(tempDlAggregateGbrPerTTI[!preferredCellIndex][VICTIM_UE] < numOfTokensBorrowed)
                            {
                                tempDlAggregateGbrPerTTI[!preferredCellIndex][VICTIM_UE] = 0;
                            }
                            else
                            {
							tempDlAggregateGbrPerTTI[!preferredCellIndex][VICTIM_UE] -= numOfTokensBorrowed;
						}
					}
                    }
					/* EICIC -*/
					/* - SPR 13605 Fix */

					LOG_MAC_MSG(MAC_APPLY_PREF_FACTOR_1,LOGDEBUG,MAC_DL_Strategy,
							getCurrentTick(),
							ueDLContext_p->ueIndex,
							dlQosStrategyLCNode_p->lcId,
							preferredCellIndex,
							dlQosStrategyLCNode_p->schedulableTokensPcell,
							dLUESCellStartegyNode_p->schedulableTokensScell,
							DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,
							FUNCTION_NAME, "");

					LOG_MAC_MSG(MAC_APPLY_PREF_FACTOR_2,LOGDEBUG,MAC_DL_Strategy,
							getCurrentTick(),
							ueDLContext_p->ueIndex,
							numOfTokensBorrowed,
							maxBorrowableTokens,
							numOfTokensNeedToBorrow,
							othreCellSchedulableTokens,
							DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,
							FUNCTION_NAME, "");
				}
				else
				{
					/* restrictedDLRBs limit reached ;
					 * can't borrow tokens to schedule LC */ 
					LOG_MAC_MSG(MAC_APPLY_PREF_FACTOR_3,LOGDEBUG,MAC_DL_Strategy,
							getCurrentTick(),
							ueDLContext_p->ueIndex,
							dlQosStrategyLCNode_p->lcId,
							preferredCellIndex,
							prefCellSchedulableTokens,
							maxBytesAvailableAsPerRestrictedRb,
							DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,
							FUNCTION_NAME, "restrictedDLRBs limit reached");
				}
			}
			else
			{
				/* No Need to borrow tokens */
				LOG_MAC_MSG(MAC_APPLY_PREF_FACTOR_4,LOGDEBUG,MAC_DL_Strategy,
						getCurrentTick(),
						ueDLContext_p->ueIndex,
						dlQosStrategyLCNode_p->lcId,
						preferredCellIndex,
						prefCellSchedulableTokens,
						totalSchedulableBytes,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME, "No Need to borrow tokens");
			}
			/* + SPR 11656 Changes */
		}
		/* - SPR 11656 Changes */
	}
}

/*****************************************************************************
 * Function Name  : dlQoSScheduleLcOnPreferredCells 
 * Inputs         : dlQosStrategyLCNode_p
 *                : currentGlobalTTITickCount
 *                : preferredCellListIndex
 *                : gbrSchedulingFlag_p
 *                : reStartTraversing_p 
 * Outputs        : 
 * Returns        : None 
 * Description    : This func will process StrategyLCNode of the UE in order of 
 *                  Serving Cells given as per preferredCellList of the UE.
 *****************************************************************************/
/* + SPR 13605 Fix */
 void dlQoSScheduleLcOnPreferredCells( DLUEContext* ueDLContext_p,
		DLQosStrategyLCNode *dlQosStrategyLCNode_p,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		UInt8  preferredCellListIndex,
		UInt8  *gbrSchedulingFlag_p,
		UInt8  *reStartTraversing_p,
        UInt32 tempDlAggregateGbrPerTTI[MAX_NUM_CELL][MAX_VALID_UE_TYPE])
    /* - SPR 13605 Fix */
{
/* SPR 23393 Changes Start */
    /* Code Removed */
/* SPR 23393 Changes Start */
	UInt8   userLocationType        = CC_USER;
	UInt8   transmissionMode        = 0;
	UInt8   listIndex               = 0; 
	SInt8   deltaRB                 = 0;

	/* SPR 12206 Start */
	UInt32  schedulableBytes        = 0;
	/* SPR 12206 End */

	UInt16  maxTBSize               = 0;
	UInt32  netQueueLoad            = 0;
	UInt32  netAvlblTkns            = 0;

	UInt16* totalSchBytes_p                 = PNULL;
	SInt32* servingCellAvailToken_p         = PNULL;
	UInt16* servingCellSchedulableBytes_p   = PNULL;

	DLHARQEntity*           dlHarqContext_p       = PNULL;
	DLHARQProcess*          harqProcess_p         = PNULL;
	DLStrategyTxNode*       dlStrategyTxNode_p    = PNULL;
	DLLogicalChannelInfo*   lcInfo_p              = PNULL;

	static UInt16 scheduledGbrBytes[MAX_NUM_CELL] = {0};

	InternalCellIndex pCellIndex = ueDLContext_p->internalCellIndex;
	/* + SPR 13605 Fix */
	ServingCellIndex servingCellIndex   = pCellIndex;

	/** SPR 13599 Changes Start **/
	UInt8 numOfPreferredCells    = MAX_NUM_PCELL;
	UInt8 isCaEnabled            = macParams_g.isCASupported;

	if(isCaEnabled)
	{
		numOfPreferredCells    = dlQoSPreferredCellList_g[pCellIndex].preferredCellList\
					 [preferredCellListIndex].count;
	}
	/* - SPR 13605 Fix */
	lcInfo_p = dlQosStrategyLCNode_p->lcInfo_p;

	for(listIndex = 0; listIndex < numOfPreferredCells; listIndex++)
	{
		totalSchBytes_p = &(dlQosStrategyLCNode_p->schedulableBytes);
		if(lcInfo_p->queueLoad <= *totalSchBytes_p)
		{
			/* LC has been scheduled on any serving cell & now 
			 * no data available to schedule further */
			break;
		}
		if(isCaEnabled)
		{
			servingCellIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellList\
					   [preferredCellListIndex].preferredCellListInfo[listIndex].cellIndex;
		}
		/** SPR 13599 Changes End **/

		/* EICIC +*/
		/* Complexity Start*/
		if(isCaEnabled)
		{
			if ((INVALID_UE_TYPE == ueTypeToScheduleDL_g[servingCellIndex])||((VICTIM_UE == ueTypeToScheduleDL_g[servingCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))||((NON_VICTIM_UE == ueTypeToScheduleDL_g[servingCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))     
			{
				continue;
			}
			/* Complexity End*/
		}
		/*  EICIC -*/
		dlStrategyTxNode_p = dlQoSPrepareUEStrategyNode( ueDLContext_p,
                /* +- SPR 17777 */
				currentGlobalTTITickCount,
				servingCellIndex,
				dlQosStrategyLCNode_p,
				NEW_TRAVERSAL);

		if( PNULL == dlStrategyTxNode_p )
		{
			continue;
		}

		if(pCellIndex == servingCellIndex)
		{
			maxTBSize = ueDLContext_p->maxTBSize;
		}
		else
		{
			maxTBSize = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->maxTBSize;
		}

		LOG_MAC_MSG(MAC_SCHD_LC_ON_PREF_CELL_1,LOGDEBUG,MAC_DL_Strategy,
				getCurrentTick(),
				ueDLContext_p->ueIndex,
				dlStrategyTxNode_p->maxTBSize,
				maxTBSize,
				servingCellIndex,
				*totalSchBytes_p,
				DEFAULT_FLOAT_VALUE,
				DEFAULT_FLOAT_VALUE,
				FUNCTION_NAME, "");
		if(dlStrategyTxNode_p->maxTBSize < maxTBSize)
		{  
			if(pCellIndex == servingCellIndex)
			{
				servingCellAvailToken_p = &(dlQosStrategyLCNode_p->\
						schedulableTokensPcell);
				servingCellSchedulableBytes_p = &(dlQosStrategyLCNode_p->\
						schedulableBytesPcell);

				userLocationType = ueDLContext_p->userLocationType;
				dlHarqContext_p  = ueDLContext_p->dlHarqContext_p;
				transmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;
			}
			else
			{
				servingCellAvailToken_p = &(dlQosStrategyLCNode_p->\
						dLUESCellStartegyNode_p[START_SCELL_INDEX]->schedulableTokensScell);
				servingCellSchedulableBytes_p = &(dlQosStrategyLCNode_p->\
						dLUESCellStartegyNode_p[START_SCELL_INDEX]->schedulableBytesScell);

				userLocationType = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
				dlHarqContext_p  = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p;
				transmissionMode = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.transmissionMode;
			}

			if((RLC_AM_MODE != lcInfo_p->rlcMode) || (!lcInfo_p->dlLcQosInfo.retxnPduSize))
			{
				if ( RLC_WAIT_FOR_STATUS_PDU != lcInfo_p->dlLcQosInfo.eRlcAmStatus )
				{
					if ( lcInfo_p->queueLoad  > *totalSchBytes_p)
					{
						netQueueLoad = lcInfo_p->queueLoad - *totalSchBytes_p;
					}
					else
					{
						netQueueLoad = 0;
					}

					if ( *servingCellAvailToken_p > *servingCellSchedulableBytes_p) 
					{
						netAvlblTkns = *(servingCellAvailToken_p) - 
							*servingCellSchedulableBytes_p;
					}
					else
					{
						netAvlblTkns = 0;
					}
					schedulableBytes = MIN_BETWEEN_TWO(netAvlblTkns, netQueueLoad); 
				}
				if ( !*totalSchBytes_p )
				{
					schedulableBytes += lcInfo_p->dlLcQosInfo.ctrlQload;
				}
			}
			else
			{
				/* Obtain the schedulable bytes equal to 
				 * First Re Tx PDU Qload of the LC
				 */
				if ( !*totalSchBytes_p )
				{
					schedulableBytes = lcInfo_p->dlLcQosInfo.retxnPduSize + 
						lcInfo_p->dlLcQosInfo.ctrlQload;
				}
			}
			if (schedulableBytes && (!*servingCellSchedulableBytes_p))
			{
				/* Add MAC Subheader and RLC fixed header size */
				schedulableBytes += MAC_SUBHEADER_LENGTH_THREE_BYTE;
				if( (transmissionMode == TX_MODE_3) ||
						(transmissionMode == TX_MODE_4) ||
						(transmissionMode == TX_MODE_8))
				{
					schedulableBytes += MAC_SUBHEADER_LENGTH_THREE_BYTE;
				}
				schedulableBytes += lcInfo_p->dlLcQosInfo.rlcFixedHeaderSize;
			}

			if((dlStrategyTxNode_p->maxTBSize + schedulableBytes) > maxTBSize)
			{
				schedulableBytes = maxTBSize - dlStrategyTxNode_p->maxTBSize;
#ifdef PERF_STATS
				/* + SPR 11214 Fix */
				updatePerformanceStats( ueDLContext_p, servingCellIndex);
				/* - SPR 11214 Fix */

#endif
				if(((RLC_TM_MODE != lcInfo_p->rlcMode)&& 
							(schedulableBytes <= (UInt32)MAC_SUBHEADER_LENGTH_THREE_BYTE + 
							 lcInfo_p->dlLcQosInfo.rlcFixedHeaderSize)) || 
						((RLC_TM_MODE == lcInfo_p->rlcMode)&&
						 (MAC_SUBHEADER_LENGTH_THREE_BYTE >= schedulableBytes)))
				{
					schedulableBytes = 0;
				}
			}

			harqProcess_p = &dlHarqContext_p->harqProcess[
				dlStrategyTxNode_p->harqProcessId];

			/* + SPR 13605 Fix */
			if(TRUE == *gbrSchedulingFlag_p )
			{
				/* EICIC + */
				if ( VICTIM_UE == ueTypeToScheduleDL_g[ueDLContext_p->internalCellIndex])
				{

                    if((scheduledGbrBytes[servingCellIndex] + schedulableBytes) < 
							tempDlAggregateGbrPerTTI[servingCellIndex][VICTIM_UE]) 
					{
						scheduledGbrBytes[servingCellIndex] += schedulableBytes;
					}
					else                     
					{
						schedulableBytes = tempDlAggregateGbrPerTTI[servingCellIndex][VICTIM_UE] - 
							scheduledGbrBytes[servingCellIndex];
						scheduledGbrBytes[servingCellIndex] += schedulableBytes;
						/** SPR 13599 Changes Start **/
						if(isCaEnabled)
						{
							/* False gbrScheduling only if dlAggregateGbrPerTTI of other 
							 * serving cell is also reached */                                                
							if(tempDlAggregateGbrPerTTI[!servingCellIndex][VICTIM_UE] <= 
									scheduledGbrBytes[!servingCellIndex])
							{
								*gbrSchedulingFlag_p = FALSE;
								*reStartTraversing_p = TRUE;

							}
						}
						else
						{
							*gbrSchedulingFlag_p = FALSE;
							*reStartTraversing_p = TRUE;
						}
						/** SPR 13599 Changes End **/
					}
					LOG_MAC_MSG(MAC_SCHD_LC_ON_PREF_CELL_4,LOGDEBUG,MAC_DL_Strategy,
							getCurrentTick(),
							ueDLContext_p->ueIndex,
							tempDlAggregateGbrPerTTI[servingCellIndex][VICTIM_UE],
							tempDlAggregateGbrPerTTI[!servingCellIndex][VICTIM_UE],
							*gbrSchedulingFlag_p,
							*reStartTraversing_p,
							DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,
							FUNCTION_NAME, "");
				}
				if ( NON_VICTIM_UE == ueTypeToScheduleDL_g[ueDLContext_p->internalCellIndex])
				{

                    if((scheduledGbrBytes[servingCellIndex] + schedulableBytes) < 
							tempDlAggregateGbrPerTTI[servingCellIndex][NON_VICTIM_UE]) 
					{
						scheduledGbrBytes[servingCellIndex] += schedulableBytes;
					}
					else                     
					{
						schedulableBytes = tempDlAggregateGbrPerTTI[servingCellIndex][NON_VICTIM_UE] - 
							scheduledGbrBytes[servingCellIndex];
						scheduledGbrBytes[servingCellIndex] += schedulableBytes;
						/** SPR 13599 Changes Start **/
						if(isCaEnabled)
						{
							/* False gbrScheduling only if dlAggregateGbrPerTTI of other 
							 * serving cell is also reached */                                                
							if(tempDlAggregateGbrPerTTI[!servingCellIndex][NON_VICTIM_UE] <= 
									scheduledGbrBytes[!servingCellIndex])
							{
								*gbrSchedulingFlag_p = FALSE;
								*reStartTraversing_p = TRUE;

							}
						}
						else
						{
							*gbrSchedulingFlag_p = FALSE;
							*reStartTraversing_p = TRUE;
						}
						/** SPR 13599 Changes End **/
					}

					LOG_MAC_MSG(MAC_SCHD_LC_ON_PREF_CELL_4,LOGDEBUG,MAC_DL_Strategy,
							getCurrentTick(),
							ueDLContext_p->ueIndex,
							tempDlAggregateGbrPerTTI[servingCellIndex][NON_VICTIM_UE],
							tempDlAggregateGbrPerTTI[!servingCellIndex][NON_VICTIM_UE],
							*gbrSchedulingFlag_p,
							*reStartTraversing_p,
							DEFAULT_FLOAT_VALUE,
							DEFAULT_FLOAT_VALUE,
							FUNCTION_NAME, "");
				}
				/* EICIC -*/
				LOG_MAC_MSG(MAC_SCHD_LC_ON_PREF_CELL_3,LOGDEBUG,MAC_DL_Strategy,
						getCurrentTick(),
						ueDLContext_p->ueIndex,
						dlQosStrategyLCNode_p->lcId,
						schedulableBytes,
						scheduledGbrBytes[servingCellIndex],
						scheduledGbrBytes[!servingCellIndex],
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME, "");

			}
			/* - SPR 13605 Fix */
			if(FALSE == *gbrSchedulingFlag_p)
			{
				scheduledGbrBytes[servingCellIndex] = 0;
				scheduledGbrBytes[!servingCellIndex] = 0;
			}
			    if(!schedulableBytes)
			    {
				    schedulableBytes =  MAC_SUBHEADER_LENGTH_THREE_BYTE;
			    }
			/* Insert Node in dlScheduledLcList only for One serving cell */
			if(!(*totalSchBytes_p))
			{
#ifdef MAC_CA_WORKAROUND
            if(ueDLContext_p->scheduledLcCount == 0)
#endif
            {
				ueDLContext_p->scheduledLcCount++;
				                //fprintf(stderr,"Error scheduledLcCount more than 1 %s %d at %llu\n",__func__,__LINE__,getCurrentTick());
				LOG_MAC_MSG(MAC_INSERT_SCH_LIST_NODE,
						LOGBRIEF, MAC_MUX,
						currentGlobalTTITickCount,
						__LINE__,
						(UInt32)ueDLContext_p->ueIndex, 
						dlQosStrategyLCNode_p->lcId,
						ueDLContext_p->scheduledLcCount,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME,"");

				/* Insert the Lc Node in the scheduled LC list */
				listInsertNode(&ueDLContext_p->dlScheduledLcList, 
						&dlQosStrategyLCNode_p->scheduledLcAnchor);

				LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC
					(ueDLContext_p->ueIndex, currentGlobalTTITickCount, dlQosStrategyLCNode_p,
					 lcInfo_p, (&lcInfo_p->dlLcQosInfo), servingCellIndex); 
}

			}
			*servingCellSchedulableBytes_p += schedulableBytes;
			*totalSchBytes_p += schedulableBytes;
/* SPR 23393 Changes Start */
    /* +- SPR 17777 */
#if   TDD_CONFIG
	UInt8   subFrameNum             = currentGlobalTTITickCount % MAX_SUB_FRAME;
#endif
    /* +- SPR 17777 */
/* SPR 23393 Changes End */

			deltaRB = dlQosDetermineTbSizeForNewTx(
					ueDLContext_p,
					harqProcess_p,
					numRBsAvailableForScheduling_g[servingCellIndex][userLocationType],
					*servingCellSchedulableBytes_p,
                    /* +- SPR 17777 */
#ifdef TDD_CONFIG
					subFrameNum,
#endif
                    /* +- SPR 17777 */
					servingCellIndex);

			/* decrement the scheduled RB from available RB's */
			numRBsAvailableForScheduling_g[servingCellIndex][userLocationType] -= deltaRB;
			LOG_MAC_MSG(MAC_SCHD_LC_ON_PREF_CELL_2,LOGDEBUG,MAC_DL_Strategy,
					getCurrentTick(),
					ueDLContext_p->ueIndex,
					*servingCellAvailToken_p,
					*servingCellSchedulableBytes_p,
					schedulableBytes,
					deltaRB,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME, "");
			/* then update RBs for both cell centered and cell edge */
			RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(deltaRB,
					numRBsAvailableForScheduling_g[servingCellIndex]) ;

			LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC_SCHEDULED_BYTES(
					ueDLContext_p->ueIndex,
					dlQosStrategyLCNode_p->lcId,
					*totalSchBytes_p, 
					servingCellIndex);
		}
	}/* End of for loop */
}

/*****************************************************************************
 * Function Name  : isRbAvailableOnPreferredCells 
 * Inputs         : preferredCellListIndex,pCellIndex 
 * Outputs        : 
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This func will check for Positive number of available
 *                  RBs in Serving Cells given as per preferredCellList.
 *****************************************************************************/
 UInt8 isRbAvailableOnPreferredCells( InternalCellIndex pCellIndex,
		UInt8 preferredCellListIndex)
{
	if(IS_CA_ENABLED())
	{
		UInt8 listIndex = 0;
		UInt8 preferredCellListCount = 
			dlQoSPreferredCellList_g[pCellIndex].preferredCellList[preferredCellListIndex].count;

		for(listIndex = 0;listIndex < preferredCellListCount; listIndex++)
		{
                /* SPR 16734 fix start */
            ServingCellIndex servingCellIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellList[preferredCellListIndex].\
                                                preferredCellListInfo[listIndex].cellIndex; 
			/*+- SPR 20895 */
            if( numRBsAvailableForScheduling_g[servingCellIndex][CC_CE_REGION] > 2)
		    /*+- SPR 20895 */
                /* SPR 16734 fix end */
			{
				return MAC_SUCCESS;
			}
		}
	}
	else
	{
	    /*+- SPR 20895 */
		if(numRBsAvailableForScheduling_g[pCellIndex][CC_CE_REGION] > 0 )
		/*+- SPR 20895 */
		{
			return MAC_SUCCESS;
		}
	}
	return MAC_FAILURE;
}

/*****************************************************************************
 * Function Name  : isTotalRbMIMOAvailableOnPreferredCells 
 * Inputs         : preferredCellListIndex,pCellIndex
 * Outputs        : 
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This func will check for Positive number of available
 *                  MIMO Rbs in Serving Cells given as per preferredCellList.
 *****************************************************************************/
 UInt8 isTotalRbMIMOAvailableOnPreferredCells( InternalCellIndex pCellIndex,
		UInt8 preferredCellListIndex)
{
	if(IS_CA_ENABLED())
	{
		UInt8 listIndex = 0;
		UInt8 preferredCellListCount = 
			dlQoSPreferredCellList_g[pCellIndex].preferredCellList[preferredCellListIndex].count;
		ServingCellIndex servingCellIndex = INVALID_CELL_INDEX;

		for(listIndex = 0; listIndex < preferredCellListCount; listIndex++)
		{
			servingCellIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellList\
					   [preferredCellListIndex].preferredCellListInfo[listIndex].cellIndex;

			if( totalRbMIMO_g[servingCellIndex])
			{
				return MAC_SUCCESS;
			}
		}
	}
	else
	{
		if(totalRbMIMO_g[pCellIndex])
		{
			return MAC_SUCCESS;
		}
	}
	return MAC_FAILURE;
}

/********************************************************************************
 * Function Name  : dlQosReTraversePriorityLcListForAllCells 
 * Inputs         : currentGlobalTTITickCount
 *                : internalCellIndex
 * Outputs        : 
 * Returns        : None 
 * Description    : This func will retraverse the priorityLcTree, if required,
 *                  in all or particular cell based on whether CA is enabled or not.
 **********************************************************************************/
 void dlQosReTraversePriorityLcListForAllCells( 
        tickType_t currentGlobalTTITickCount,
		InternalCellIndex internalCellIndex)
{
#ifdef TDD_CONFIG
/* SPR 23393 Changes Start */
	UInt8 subFrameNum = currentGlobalTTITickCount % MAX_SUB_FRAME;
/* SPR 23393 Changes End */
#endif

    InternalCellIndex cellIndex = 0;
    UInt8 isReTraverseRequired = 0;

	for( ; cellIndex < cellSpecificParams_g.numOfCells ; cellIndex++ )
    {

        if( (0 < numRBsAvailableForScheduling_g[cellIndex][CC_CE_REGION]) && 
                (cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->
			 initParams_p->enableAllocAfterAllowedBitrate))
	{
            isReTraverseRequired = TRUE ;
            break;
        }
    }

	if( isReTraverseRequired)
	{
		dlQosReTraversePriorityLcList(
#ifdef TDD_CONFIG
                subFrameNum,
#endif
				currentGlobalTTITickCount,
				internalCellIndex,
                /* SPR 22539 Fix + */
                cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->
                initParams_p->enableAllocAfterAllowedBitrate
                /* SPR 22539 Fix - */
                );
	}
}

/*****************************************************************************
 * Function Name  : dlQosUpdateDlBufferPoolAndResetTotalRbMIMO 
 * Inputs         : internalCellIndex
 * Outputs        : 
 * Returns        : None 
 * Description    : This func will update Buffer RBs in all/particular Cell 
 *                  for each user loaction and reset totalRBMIMO.
 *****************************************************************************/
 void dlQosUpdateDlBufferPoolAndResetTotalRbMIMO(InternalCellIndex internalCellIndex)
{
	ServingCellIndex servingCellIndex = 0;

	if(!IS_CA_ENABLED())
	{
		dlBufferPool_g[internalCellIndex].totalRBs = 
			( numRBsAvailableForScheduling_g[internalCellIndex][CC_CE_REGION]- 
			  unusedDLStrategyBufferRB_g[internalCellIndex]);

		dlBufferPool_g[internalCellIndex].cellEdgeRBs = 
			( numRBsAvailableForScheduling_g[internalCellIndex][CE_REGION]-
			  unusedDLStrategyBufferRB_g[internalCellIndex]);

		totalRbMIMO_g[internalCellIndex] = 0;                                                    
	}
	else
	{
		for(servingCellIndex = 0; servingCellIndex < cellSpecificParams_g.numOfCells ; servingCellIndex++)
		{
			dlBufferPool_g[servingCellIndex].totalRBs = 
				( numRBsAvailableForScheduling_g[servingCellIndex][CC_CE_REGION]- 
				  unusedDLStrategyBufferRB_g[servingCellIndex]);

			dlBufferPool_g[servingCellIndex].cellEdgeRBs = 
				( numRBsAvailableForScheduling_g[servingCellIndex][CE_REGION]-
				  unusedDLStrategyBufferRB_g[servingCellIndex]);

			totalRbMIMO_g[servingCellIndex] = 0;                                                    
		}
	}
}

/*****************************************************************************
 * Function Name  : dlQosPostStrategyExecution 
 * Inputs         : internalCellIndex
 *                  currentGlobalTTITickCount
 * Outputs        : 
 * Returns        : None 
 * Description    : This func will update Strategy Nodes in all/particular
 *                  serving cells based on whether CA is enabled or not.
 *                  Also, it will reset Preferred cell list.
 *****************************************************************************/
 void dlQosPostStrategyExecution( InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount)
/* SPR 15909 fix end */
{
	InternalCellIndex cellIndex = 0;
	/* To be discussed: Currently below func is called for all cells since
	 * no Post startegy EL is there to be called. 
	 * This would be modify later */
	if(IS_CA_ENABLED())
	{
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
		{
			if (CELL_PHY_RUNNING == cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->cellState)
			{
				dlSpsUpdateStrategyNodes(cellIndex, currentGlobalTTITickCount);
				dlQosUpdateStrategyNodes(cellIndex, currentGlobalTTITickCount);
				dlQosResetPreferredCellList(cellIndex);
			}
		}
	}
	else
	{
		dlSpsUpdateStrategyNodes(internalCellIndex, currentGlobalTTITickCount);
		dlQosUpdateStrategyNodes(internalCellIndex, currentGlobalTTITickCount);
		/* + SPR 13605 Fix */
		/* No need to reset PreferredCellList as creation is removed for non-CA */
		/* dlQosResetPreferredCellList(internalCellIndex);*/
		/* - SPR 13605 Fix */

	}
}
/*****************************************************************************
 * Function Name  : getCellIdWithHighestMcs 
 * Inputs         : ueDLContext_p
 * Outputs        : 
 * Returns        : None 
 * Description    : This func will determine the serving cell index with 
 *                  highest MCS value for the UE.
 *****************************************************************************/
 InternalCellIndex getCellIdWithHighestMcs(DLUEContext* ueDLContext_p)
{
	InternalCellIndex highestMcsCellIndex = ueDLContext_p->internalCellIndex;
	UInt8 highestMcs = dlQosGetPCellMcs(ueDLContext_p);
	UInt8 sCellMcs = 0;
	InternalCellIndex scellIndex = START_SCELL_INDEX;

	for (scellIndex = START_SCELL_INDEX ; scellIndex <= ueDLContext_p->scellCount; scellIndex++)
	{
		sCellMcs = dlQosGetSCellMcs( ueDLContext_p, 
				ueDLContext_p->dlUeScellContext_p[scellIndex]->internalCellIndex);
		if(highestMcs < sCellMcs)
		{
			highestMcs = sCellMcs;
			highestMcsCellIndex = scellIndex;
		}
	}
	return highestMcsCellIndex;
}
/*********************************************************************************
 * Function Name  : dlQosStrategyProcessTAQueueOfAllCells
 * Inputs         : 
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  tempTimeAdvanceQueue_p - Pointer to Time Advance Queue
 *                                           Either New or Pending TA Queues,
 *                  numTAToSchedule_p - Number of TA to Schedule,
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled,
 *                  flagForTADRXPendingFresh - flag for Pending TA DRX,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : It will allocate RBs to PendingTA/FreshTAQueue Nodes &
 place the DL Strategy O/P either in the PDCCHNewTXQueue_g
 or PDCHHReTXQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the TA Queues Nodes
 fresh and Pending both. Allocate RBs to the UEs with non
 Zero Queue Load otherwise add then in scheduler Array.
 *********************************************************************************/
/*SPR 22919 Start*/
 void dlQosStrategyProcessTAQueueOfAllCells( 
/*SPR 22919 End*/
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		TA_Q_TYPE** tempTimeAdvanceQueue_p,
		UInt8   *numTAToSchedule_p,
		UInt8   *numDlUEsToBeScheduled_p,
		FlagForPendingTADRX flagForTADRXPendingFresh,
		InternalCellIndex internalCellIndex)
{
	UInt16 queueCount = 0;
	InternalCellIndex cellIndex = 0;

	if(!IS_CA_ENABLED())
	{
		queueCount = COUNT_TA_Q(tempTimeAdvanceQueue_p[internalCellIndex]);

		/** SPR 13599 Changes Start **/
		if( ( queueCount) &&
				( numDlUEsToBeScheduled_p[internalCellIndex]) &&
				( numberOfAvailableRBs[internalCellIndex][CC_CE_REGION]))
			/** SPR 13599 Changes End **/
		{
			dlQosStrategyProcessTAQueueForNonCAUes( queueCount,
					numberOfAvailableRBs[internalCellIndex],
					currentGlobalTTITickCount, 
					tempTimeAdvanceQueue_p[internalCellIndex],
					&numTAToSchedule_p[internalCellIndex], 
					&numDlUEsToBeScheduled_p[internalCellIndex], 
					flagForTADRXPendingFresh,
					internalCellIndex);
		}
	}
	else
	{
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
		{
			queueCount = COUNT_TA_Q(tempTimeAdvanceQueue_p[cellIndex]);
			if(queueCount)
			{
				dlQosStrategyProcessTAQueueForCAUes( queueCount,
						numberOfAvailableRBs,
						currentGlobalTTITickCount, 
						tempTimeAdvanceQueue_p[cellIndex],
						numTAToSchedule_p, 
						numDlUEsToBeScheduled_p, 
						flagForTADRXPendingFresh,
						cellIndex);
			}
		}
	}
}

/*********************************************************************************
 * Function Name  : dlQosStrategyProcessTAQueueForCAUes
 * Inputs         : timeAdvanceQueueCount - This is Count of the total nodes
 *                                          present in the dlTimeAdvanceQueue_g
 *                                          or pendingTAQueue,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  tempTimeAdvanceQueue_p - Pointer to Time Advance Queue
 *                                           Either New or Pending TA Queues,
 *                  numTAToSchedule_p - Number of TA to Schedule,
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled,
 *                  flagForTADRXPendingFresh - flag for Pending TA DRX,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : It will allocate RBs to PendingTA/FreshTAQueue Nodes &
 place the DL Strategy O/P either in the PDCCHNewTXQueue_g
 or PDCHHReTXQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the TA Queues Nodes
 fresh and Pending both. Allocate RBs to the UEs with non
 Zero Queue Load otherwise add then in scheduler Array.
 *********************************************************************************/
/*SPR 22919 Start*/
 void dlQosStrategyProcessTAQueueForCAUes( UInt32 timeAdvanceQueueCount, 
/*SPR 22919 End*/
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		TA_Q_TYPE* tempTimeAdvanceQueue_p,
		UInt8   *numTAToSchedule_p,
		UInt8   *numDlUEsToBeScheduled_p,
		FlagForPendingTADRX flagForTADRXPendingFresh,
		InternalCellIndex internalCellIndex)
{
	UInt8  userLocationType                      = CC_USER;
	UInt8  loopCount                             = 0; 
	UInt8  loopIndex                             = 0; 
	UInt8  listIndex                             = 0;
	UInt16 ueIndex                               = 0; 
	SInt8  totalNumDlUEsToBeScheduled            = 0;

	TimeAdvanceQueueNode*    taNode_p                  = PNULL;
	DLUEContext*             ueDLContext_p             = PNULL;
	DLUEContextInfo*         ueDLContextInfo_p         = PNULL;
	DLStrategyTxNode*        dlStrategyTxNode_p        = PNULL;
	DLStrategyTxNode*        tempDlStrategyTxNode_p    = PNULL;
	LP_TimeAdvanceQueueNode  tempTaNode_p              = PNULL; 
	ServingCellIndex         servingCellIndex          = INVALID_CELL_INDEX; 
	InternalCellIndex        pCellIndex                = INVALID_CELL_INDEX; 
	MacRetType               retVal                    = MAC_FAILURE;
	DLScheduleFlag*          scheduleFlag_p            = PNULL;

	SInt32 totalNumOfAvailableRBs[MAX_USER_LOCATION]  = {0};
	/* + SPR 22243 Fix*/
    	MsgType msgType = INVALID_MSG;
    	/* - SPR 22243 Fix*/

	/* get total number of TA that can be scheduled on both cells */
	totalNumDlUEsToBeScheduled = MIN_BETWEEN_THREE(
			( numTAToSchedule_p[internalCellIndex] +
			  numTAToSchedule_p[!(internalCellIndex)]),
			( numDlUEsToBeScheduled_p[internalCellIndex] +
			  numDlUEsToBeScheduled_p[!(internalCellIndex)]),
			( timeAdvanceQueueCount));

	/* get total number of RBs available for CC & CE users in both cells */
	totalNumOfAvailableRBs[CC_CE_REGION] = 
		numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] +
		numberOfAvailableRBs[!(internalCellIndex)][CC_CE_REGION];

	totalNumOfAvailableRBs[CE_REGION] = 
		numberOfAvailableRBs[internalCellIndex][CE_REGION] +
		numberOfAvailableRBs[!(internalCellIndex)][CE_REGION];

	while (( totalNumDlUEsToBeScheduled) && 
			( totalNumOfAvailableRBs[CC_CE_REGION]) && (timeAdvanceQueueCount)) 
	{
		if (MAC_SUCCESS != DEQUEUE_TA_Q( tempTimeAdvanceQueue_p,
					TimeAdvanceQueueNode,
					(void **)&(taNode_p)))
		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,
					LOGDEBUG, MAC_DL_Strategy, 
					currentGlobalTTITickCount, timeAdvanceQueueCount,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");

			break;
		}

		ueIndex = taNode_p->ueIdx;    
		ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

		if (ueDLContextInfo_p->pendingDeleteFlag)
		{
			LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,
					LOGDEBUG, MAC_DL_Strategy,
					currentGlobalTTITickCount, ueIndex,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");

			tempTaNode_p = taNode_p;
			FREE_MEM_NODE_TA_Q(tempTaNode_p);
			timeAdvanceQueueCount--;
			continue; 
		}

		ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

        if( (DL_QOS_ALREADY_TRANSMITED > ueDLContext_p->scheduleFlag) &&
                /*SPR 21948 Changes Start */
                /* Changes under spr 21762 have been reverted */
                ( ( (isUeEligibleForCaScheduling(ueDLContext_p)) && 
                    (DL_QOS_ALREADY_TRANSMITED > ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag)) ||
                  ( !(isUeEligibleForCaScheduling(ueDLContext_p)))))
            /*SPR 21948 Changes End */
		{
			dlQosHandleTASchedulingFailureCase( internalCellIndex,
					flagForTADRXPendingFresh,
					taNode_p
					/*SPR 9664 Fix Start*/
					,ueDLContext_p
					/*SPR 9664 Fix End*/
					);
			timeAdvanceQueueCount--;
			continue;
		}
        else if( ( (DL_QOS_ALREADY_TRANSMITED == ueDLContext_p->scheduleFlag ) ||
                    /*SPR 21948 Changes Start */
                    /* Changes under spr 21762 have been reverted */
                    ( (isUeEligibleForCaScheduling(ueDLContext_p)) &&
                      /*SPR 21948 Changes End */
					  (DL_QOS_ALREADY_TRANSMITED == ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag))) &&
				(DRA_SCHEDULING == ueDLContext_p->schType))  
		{
			ueDLContext_p->timingAdvanceValue = 
				taNode_p->timeAdvanceValue;

			/* Refer Spec 36.213 Section 4.2.3 for the calculations 
			 * NTA,new = NTA,old + (TA - 31) * 16 */
			ueDLContext_p->timingAdvanceAbsValue = 
				(ueDLContext_p->timingAdvanceAbsValue) + 
				((taNode_p->timeAdvanceValue - 
				  MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31)<< 4);                    

			LOG_MAC_MSG(TIMING_ADVANCE_VALUE,LOGINFO,MAC_TA,
					currentGlobalTTITickCount,ueDLContext_p->ueIndex, 
					ueDLContext_p->timingAdvanceAbsValue, 
					taNode_p->timeAdvanceValue,
					DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
					__func__,"");

			tempTaNode_p = taNode_p;
			FREE_MEM_NODE_TA_Q(tempTaNode_p);
			timeAdvanceQueueCount--;
			continue;
		}
		else 
		{
			retVal = dlQosGetPreferedCellListForScheduling(
					ueIndex, 
					TRUE /*isMacCEScheduled*/,
                                                &listIndex,
                                                /* SPR 16116 fix start */
                                                0);
                                                /* SPR 16116 fix end */

			if(MAC_SUCCESS == retVal)
			{
				pCellIndex = ueDLContext_p->internalCellIndex;
				loopCount = dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].count;

				for(loopIndex = 0; loopIndex < loopCount; loopIndex++)
				{
					servingCellIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].\
							   preferredCellListInfo[loopIndex].cellIndex;

					/* EICIC +*/
					/* Complexity Start */
					if (((VICTIM_UE == ueTypeToScheduleDL_g[servingCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType)) || ((NON_VICTIM_UE == ueTypeToScheduleDL_g[servingCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
					{              
						if(!ENQUEUE_TA_Q(tempTimeAdvanceQueue_p,TimeAdvanceQueueNode,&(taNode_p->timeAdvanceQueueNodeAnchor)))
						{
							tempTaNode_p = taNode_p;
							FREE_MEM_NODE_TA_Q(tempTaNode_p);
						}
						timeAdvanceQueueCount--;
						continue;
					}

					/* EICIC -*/
					/* Complexity End */

					if(pCellIndex == servingCellIndex)
					{
						if(SPS_SCHEDULING == ueDLContext_p->schType)
						{
							if(loopIndex == (loopCount -1))
							{
								dlQosHandleTASchedulingFailureCase( internalCellIndex,
										flagForTADRXPendingFresh,
										taNode_p
										/*SPR 9664 Fix Start*/
										,ueDLContext_p
										/*SPR 9664 Fix End*/
										);
								timeAdvanceQueueCount--;
							}
							continue;
						}
						userLocationType = ueDLContext_p->userLocationType;
						tempDlStrategyTxNode_p = (ueDLContext_p->dlStrategyTxNode_p);
						scheduleFlag_p = &(ueDLContext_p->scheduleFlag);
					}
					else
					{
						userLocationType = 
							ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
						tempDlStrategyTxNode_p = 
							(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);
						scheduleFlag_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag);
					}
					if(DL_QOS_ALREADY_TRANSMITED > *scheduleFlag_p)
					{
						if(loopIndex == (loopCount -1))
						{
							dlQosHandleTASchedulingFailureCase( internalCellIndex,
									flagForTADRXPendingFresh,
									taNode_p
									/*SPR 9664 Fix Start*/
									,ueDLContext_p
									/*SPR 9664 Fix End*/
									);
							timeAdvanceQueueCount--;
						}
						continue;
					}

					if ( (!numberOfAvailableRBs[servingCellIndex][userLocationType]) ||
							(!numDlUEsToBeScheduled_p[servingCellIndex]) ||
							( (!maxCellEdgeUEsToBeScheduledDL_g[servingCellIndex]) && 
							  (CE_USER == userLocationType)))  
					{
						if(loopIndex == (loopCount-1))
						{
							LOG_MAC_MSG(MAC_RB_NOT_AVAILABLE_FOR_RETX,
									LOGFATAL, MAC_DL_Strategy,
									currentGlobalTTITickCount, ueIndex,
									DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
									DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
									DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
									FUNCTION_NAME, "");

							dlQosHandleTASchedulingFailureCase( internalCellIndex,
									flagForTADRXPendingFresh,
									taNode_p
									/*SPR 9664 Fix Start*/
									,ueDLContext_p
									/*SPR 9664 Fix End*/
									);
							timeAdvanceQueueCount--;
						}
						continue;
					}

					/* if UE is getting scheduled first time */
					if(PNULL == tempDlStrategyTxNode_p)
					{
						/* + SPR 22352 fix */

						/* As the processing of TA queue is done first, so we need to check if
						 * DRX or SCELL Act/Deact CE is also to be scheduled in this tti, then change 
						 * the msgType accordingly and as the scheduleFlag in Ue context is set as 
						 * DL_QOS_ALREADY_TRANSMITED below. So, in this same tick, DRX node  or scell node
						 * will not be processed and will be freed */

						checkMsgTypeTaDrxScellActDeact(ueDLContext_p,TA_MSG,&msgType,servingCellIndex);

						dlStrategyTxNode_p = 
							dlQosStrategyUpdateNewTxUEList( ueDLContext_p,
									msgType,
									NEW_TRAVERSAL,
									currentGlobalTTITickCount,
									servingCellIndex);
						/* - SPR 22352 fix */

						if(pCellIndex == servingCellIndex)
						{
							ueDLContext_p->dlStrategyTxNode_p = dlStrategyTxNode_p;
						}
						else
						{
							ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p = dlStrategyTxNode_p;
						}

					}
					else
					{
						dlStrategyTxNode_p = tempDlStrategyTxNode_p;
					}

					if(PNULL == dlStrategyTxNode_p)
					{
						if(loopIndex == (loopCount-1))
						{
							/* If there are no FREE HARQ Process to schedule UE
							 * get the next node and continue 
							 */
							LOG_MAC_MSG(MAC_ALL_HARQ_ARE_BUSY,
									LOGFATAL, MAC_DL_Strategy,
									currentGlobalTTITickCount, ueIndex,
									DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
									DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
									DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
									FUNCTION_NAME, "No FREE HARQ Available");

							dlQosHandleTASchedulingFailureCase( internalCellIndex,
									flagForTADRXPendingFresh,
									taNode_p
									/*SPR 9664 Fix Start*/
									,ueDLContext_p
									/*SPR 9664 Fix End*/
									);
							timeAdvanceQueueCount--;
						}
						/* Continue to next serving cell */
						continue;
					}
					else    
					{

						*scheduleFlag_p = DL_QOS_ALREADY_TRANSMITED;
						/* + SPR 22352 fix */
						updateRbAndTBSizeInDlStrategyNode(msgType,dlStrategyTxNode_p,
								numberOfAvailableRBs[servingCellIndex],userLocationType,ueIndex);

						if(TA_DRX_SCELL_ACT_DEACT_MSG == msgType)
						{
							totalNumOfAvailableRBs[userLocationType] -= MIN_RB_FOR_TA_DRX_SCELL_ACT_DEACT_ONLY;
						}
						else if (TA_MSG == msgType)
						{
							totalNumOfAvailableRBs[userLocationType] -= MIN_RB_FOR_TA_DRX_ONLY;
						}
						else /* TA_DRX_MSG or TA_SCELL_ACT_DEACT_MSG  */
						{
							totalNumOfAvailableRBs[userLocationType] -= MIN_RB_FOR_BOTH_TA_DRX_ONLY;
						}
						/* set TA bit in the bitmap of UE context, this bitmap will be reset 
						 * at encoder after successful encoding */
						ueDLContext_p->TaDrxScellBitmap |= TA_BIT;
						/* - SPR 22352 fix */

						/* debug Stats */
						LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STAS_SCHEDULE_TA_UE(servingCellIndex);

						ueDLContext_p->timingAdvanceValue = taNode_p->timeAdvanceValue;

						/* Refer Spec 36.213 Section 4.2.3 for the calculations 
						 * NTA,new = NTA,old + (TA - 31) * 16 */
						ueDLContext_p->timingAdvanceAbsValue = 
							ueDLContext_p->timingAdvanceAbsValue + ((taNode_p->
										timeAdvanceValue - MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31) 
									<< 4);                    
						LOG_MAC_MSG(TIMING_ADVANCE_VALUE,LOGINFO,MAC_TA,
								currentGlobalTTITickCount,ueDLContext_p->ueIndex, 
								ueDLContext_p->timingAdvanceAbsValue, 
								taNode_p->timeAdvanceValue,
								DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
								__func__,"");

						numTAToSchedule_p[servingCellIndex]--;
						numDlUEsToBeScheduled_p[servingCellIndex]--;
						totalNumDlUEsToBeScheduled--;
						/* Update Cell Edge Ue */
						if ( CE_USER == userLocationType )
						{
							--maxCellEdgeUEsToBeScheduledDL_g[servingCellIndex] ;
						}   

						tempTaNode_p = taNode_p;
						FREE_MEM_NODE_TA_Q(tempTaNode_p);
						timeAdvanceQueueCount--;

						/* No need to go for next serving cell*/
						break;
					}
				}
			}
			else
			{
				/* Currently the failure condition pending deletion is 
				 * already taken care */
			}
		}/*end of already considered flag check, The UE which are 
		   already considered will be deleted automatically */
	};
}
/***********************************************************************************
 * Function Name  : dlQosStrategyProcessDRXQueueOfAllCells 
 * Inputs         : 
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  tempDRXQueue_p - Pointer to DRX_Q_TYPE,
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : It will allocate RBs to PendingDRX/FreshDRXQueue Nodes &
 *                  place the DL Strategy O/P either in the PDCCHNewTXQueue_g
 *                  or PDCHHReTXQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the DRX Queues Nodes
 *                  fresh and Pending both . Allocate RBs to the UEs with non
 *                  Zero Queue Load otherwise add then in scheduler Array.
 *****************************************************************************/
/*SPR 22919 Start*/
 void dlQosStrategyProcessDRXQueueOfAllCells( SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
/*SPR 22919 End*/
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		DRX_Q_TYPE** tempDRXQueue_p,
		UInt8   *numDlUEsToBeScheduled_p,
		InternalCellIndex internalCellIndex)
{
	UInt16              queueCount  = 0;
	InternalCellIndex   cellIndex   = 0;

	if(!IS_CA_ENABLED())
	{
		queueCount = COUNT_DRX_Q(tempDRXQueue_p[internalCellIndex]);

		/** SPR 13599 Changes Start **/ 
		if( ( queueCount) &&
				( numDlUEsToBeScheduled_p[internalCellIndex]) &&
				( numberOfAvailableRBs[internalCellIndex][CC_CE_REGION]))
			/** SPR 13599 Changes End **/ 
		{
			dlQosStrategyProcessDRXQueueForNonCAUes( queueCount,
					numberOfAvailableRBs[internalCellIndex],
					currentGlobalTTITickCount, 
					tempDRXQueue_p[internalCellIndex], 
					&numDlUEsToBeScheduled_p[internalCellIndex], 
					internalCellIndex);
		}
	}
	else
	{
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
		{
			queueCount = COUNT_DRX_Q(tempDRXQueue_p[cellIndex]);
			if(queueCount)
            {
                {
                    dlQosStrategyProcessDRXQueueForCAUes( queueCount,
                            numberOfAvailableRBs,
                            currentGlobalTTITickCount, 
                            tempDRXQueue_p[cellIndex], 
                            numDlUEsToBeScheduled_p, 
                            internalCellIndex);
                }
            }
		}
	}
}

/***********************************************************************************
 * Function Name  : dlQosStrategyProcessDRXQueueForCAUes 
 * Inputs         : drxQueueCount - This is Count of the total nodes
 *                                  present in the dlDRXQueue_g or pendingDRXQueue,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  tempDRXQueue_p - Pointer to DRX_Q_TYPE,
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : It will allocate RBs to PendingDRX/FreshDRXQueue Nodes &
 *                  place the DL Strategy O/P either in the PDCCHNewTXQueue_g
 *                  or PDCHHReTXQueue_g
 * Returns        : None
 * Description    : Its main function will be to process the DRX Queues Nodes
 *                  fresh and Pending both . Allocate RBs to the UEs with non
 *                  Zero Queue Load otherwise add then in scheduler Array.
 *****************************************************************************/
/*SPR 22919 Start*/
 void dlQosStrategyProcessDRXQueueForCAUes( UInt32 drxQueueCount,
/*SPR 22919 End*/
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		DRX_Q_TYPE * tempDRXQueue_p,
		UInt8   *numDlUEsToBeScheduled_p,
		InternalCellIndex internalCellIndex)
{
	UInt8   userLocationType                      = CC_USER;
	UInt8   loopCount                             = 0;
	UInt8   loopIndex                             = 0;
	UInt8   listIndex                             = 0;
	UInt16  ueIndex                               = 0; 
	SInt8   totalNumDlUEsToBeScheduled            = 0;

	DRXQueueNode*      drxNode_p               = PNULL;
	DLUEContext*       ueDLContext_p           = PNULL;
	ULUEContext*       ueULContext_p           = PNULL;
	ULUEContextInfo*   ueULContextInfo_p       = PNULL;
	DLUEContextInfo*   ueDLContextInfo_p       = PNULL;
	DLStrategyTxNode*  dlStrategyTxNode_p      = PNULL;
	DLStrategyTxNode*  tempDlStrategyTxNode_p  = PNULL;
	ServingCellIndex   servingCellIndex        = INVALID_CELL_INDEX; 
	InternalCellIndex  pCellIndex              = INVALID_CELL_INDEX; 
	MacRetType         retVal                  = MAC_FAILURE;
	DLScheduleFlag*    scheduleFlag_p          = PNULL;

	SInt32  totalNumOfAvailableRBs[MAX_USER_LOCATION] = {0};

	totalNumDlUEsToBeScheduled = MIN_BETWEEN_TWO(
			drxQueueCount,
			( numDlUEsToBeScheduled_p[internalCellIndex] + 
			  numDlUEsToBeScheduled_p[!(internalCellIndex)]));

	totalNumOfAvailableRBs[CC_CE_REGION] = 
		numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] +
		numberOfAvailableRBs[!(internalCellIndex)][CC_CE_REGION];

	totalNumOfAvailableRBs[CE_REGION] = 
		numberOfAvailableRBs[internalCellIndex][CE_REGION] +
		numberOfAvailableRBs[!(internalCellIndex)][CE_REGION];

	while ( (totalNumDlUEsToBeScheduled) &&
			(totalNumOfAvailableRBs[CC_CE_REGION]) &&
			(drxQueueCount))
	{
		if(MAC_SUCCESS != DEQUEUE_DRX_Q( tempDRXQueue_p,
					DRXQueueNode,
					(void **)&(drxNode_p)))
		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,
					LOGDEBUG, MAC_DL_Strategy,
					currentGlobalTTITickCount, drxQueueCount,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");

			break;
		}
		ueIndex = drxNode_p->ueIdx;    
		ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
		/*Drx Cmd Qos Fix Start*/
		ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
		/*Drx Cmd Qos Fix End*/
		if (ueDLContextInfo_p->pendingDeleteFlag)
		{
			LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,
					LOGDEBUG, MAC_DL_Strategy,
					currentGlobalTTITickCount, ueIndex,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");

			FREE_MEM_NODE_DRX_Q(drxNode_p);
			drxQueueCount--;
			continue; 
		}

		ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
		ueULContext_p = ueULContextInfo_p->ulUEContext_p;

		/* No need to send DRX for UEs having +ve Qload/BSR or already 
		 * considered for scheduling */
		if( (DL_QOS_ALREADY_TRANSMITED >= ueDLContext_p->scheduleFlag) ||
                /*SPR 21948 Changes Start */
                /* Changes under spr 21762 have been reverted */
                ( (isUeEligibleForCaScheduling(ueDLContext_p)) && 
                  /*SPR 21948 Changes End */
				  (DL_QOS_ALREADY_TRANSMITED >= ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag)) || 
				(0 != ueDLContext_p->ueQueueLoad) ||
				(0 != ueULContext_p->bsrNetTotal))
		{
			FREE_MEM_NODE_DRX_Q(drxNode_p);
			drxQueueCount--;
			continue;
		}

		retVal = dlQosGetPreferedCellListForScheduling(
				ueIndex, 
				TRUE /*isMacCEScheduled*/,
                                            &listIndex,
                                            /* SPR 16116 fix start */
                                            0);
                                            /* SPR 16116 fix end */

		if(MAC_SUCCESS == retVal)
		{            
			pCellIndex = ueDLContext_p->internalCellIndex;
			loopCount = dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].count;

			for(loopIndex = 0; loopIndex < loopCount; loopIndex++)
			{
				servingCellIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].\
						   preferredCellListInfo[loopIndex].cellIndex;
				/* EICIC +*/             
				/*Complexity Start*/
				if (((VICTIM_UE == ueTypeToScheduleDL_g[servingCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))||((NON_VICTIM_UE == ueTypeToScheduleDL_g[servingCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{   	            
					if(!ENQUEUE_DRX_Q(tempDRXQueue_p,DRXQueueNode, (void *) drxNode_p))
					{
						lteWarning("Error in Pushing in Pending"
								" DRXQueue in funtion %s\n",
								__func__);
						FREE_MEM_NODE_DRX_Q(drxNode_p);
					}
					drxQueueCount--;
					continue;
				}
				/*Complexity End*/
				/* EICIC -*/

				if(pCellIndex == servingCellIndex)
				{
					userLocationType = ueDLContext_p->userLocationType;
					tempDlStrategyTxNode_p = (ueDLContext_p->dlStrategyTxNode_p);
					scheduleFlag_p = &(ueDLContext_p->scheduleFlag);
				}
				else
				{
					userLocationType = 
						ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
					tempDlStrategyTxNode_p = 
						(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);
					scheduleFlag_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag);
				}

				if ( (!numberOfAvailableRBs[servingCellIndex][userLocationType]) ||
						(!numDlUEsToBeScheduled_p[servingCellIndex]) ||
						( (!maxCellEdgeUEsToBeScheduledDL_g[servingCellIndex]) && 
						  (CE_USER == userLocationType)))
				{
					if(loopIndex == (loopCount -1) )
					{
						LOG_MAC_MSG(MAC_RB_NOT_AVAILABLE_FOR_RETX,
								LOGFATAL, MAC_DL_Strategy,
								currentGlobalTTITickCount, ueIndex,
								DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "");

						dlQosHandleDRXSchedulingFailureCase( internalCellIndex,
								drxNode_p);
						drxQueueCount--;
					}
					continue;
				}

				if(PNULL == tempDlStrategyTxNode_p)
				{
					dlStrategyTxNode_p = 
						dlQosStrategyUpdateNewTxUEList( ueDLContext_p,
								DRX_MSG,
								NEW_TRAVERSAL,
								currentGlobalTTITickCount,
								servingCellIndex);
					if(pCellIndex == servingCellIndex)
					{
						ueDLContext_p->dlStrategyTxNode_p = dlStrategyTxNode_p;
					}
					else
					{
						ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p = dlStrategyTxNode_p;
					}
				}
				else
				{
					dlStrategyTxNode_p = tempDlStrategyTxNode_p;
				}
				if(PNULL == dlStrategyTxNode_p)
				{
					/* If there are no FREE HARQ Process to schedule UE
					 * get the next node and continue 
					 */

					if(loopIndex == (loopCount -1) )
					{
						LOG_MAC_MSG(MAC_ALL_HARQ_ARE_BUSY,
								LOGFATAL, MAC_DL_Strategy,
								currentGlobalTTITickCount, ueIndex,
								DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "No FREE HARQ Available");

						dlQosHandleDRXSchedulingFailureCase( internalCellIndex,
								drxNode_p);
						drxQueueCount--;
					}
					continue;
				}


				*scheduleFlag_p = DL_QOS_NO_NEW_TRANSMISSION;
		                /* + SPR 22352 fix */
                		updateRbAndTBSizeInDlStrategyNode(DRX_MSG,dlStrategyTxNode_p,
					numberOfAvailableRBs[servingCellIndex],userLocationType,ueIndex);
		                /* - SPR 22352 fix */
				totalNumOfAvailableRBs[userLocationType] -= 
					MIN_RB_FOR_TA_DRX_ONLY;

				numDlUEsToBeScheduled_p[servingCellIndex]--;
				totalNumDlUEsToBeScheduled--;
				/* Update Cell Edge Ue */
				if ( CE_USER == userLocationType )
				{
					--maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] ;
				}   
				FREE_MEM_NODE_DRX_Q(drxNode_p);            
				drxQueueCount--;

				/* No need to go for next serving cell*/
				break;
			}
		}
		else
		{
			/* Currently the failure condition pending deletion is 
			 * already taken care */
		}
	};
}

/**********************************************************************************
 * Function Name  : dlQosStrategyProcessExplicitDCIDLQueueOfAllCells 
 * Inputs         : 
 *                  numberOfAvailableRBs_p - pointer to current available RB's,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  tempExplicitDLDCIScheduleQueue_p - Pointer to explicit
 *                                                     DL_PUCCH_TPC_SCHEDULE_TYPE,
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : 
 * Returns        : None
 * Description    : 
 *******************************************************************************/
/*SPR 22919 Start*/
 void dlQosStrategyProcessExplicitDCIDLQueueOfAllCells( 
/*SPR 22919 End*/
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE** tempExplicitDLDCIScheduleQueue_p,
		UInt8   *numDlUEsToBeScheduled_p,
		InternalCellIndex internalCellIndex)
{
	UInt16              explicitDlDciQCount  = 0;
	InternalCellIndex   cellIndex            = 0;

	if(!IS_CA_ENABLED())
	{
		if ( (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
					cellParams_p->powerControlEnableInfo.cqiSinrClpcPucchEnable == TRUE) ||
				(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
				 cellParams_p->powerControlEnableInfo.harqBlerClpcPucchEnable == TRUE))
		{
			explicitDlDciQCount = COUNT_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(
					tempExplicitDLDCIScheduleQueue_p[internalCellIndex]);

			/** SPR 13599 Changes Start **/
			if( ( explicitDlDciQCount) &&
					( numDlUEsToBeScheduled_p[internalCellIndex]) &&
					( numberOfAvailableRBs[internalCellIndex][CC_CE_REGION]))
				/** SPR 13599 Changes End **/
			{
				dlQosStrategyProcessExplicitDCIDLQueueForNonCAUes( 
						explicitDlDciQCount,
						numberOfAvailableRBs[internalCellIndex],
						currentGlobalTTITickCount,
						tempExplicitDLDCIScheduleQueue_p[internalCellIndex], 
						&numDlUEsToBeScheduled_p[internalCellIndex],
						internalCellIndex);
			}
		}
	}
	else
	{
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
		{
			if ( (cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->\
						cellParams_p->powerControlEnableInfo.cqiSinrClpcPucchEnable == TRUE) ||
					(cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->\
					 cellParams_p->powerControlEnableInfo.harqBlerClpcPucchEnable == TRUE))
			{
				explicitDlDciQCount = COUNT_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(
						tempExplicitDLDCIScheduleQueue_p[cellIndex]);

				if(explicitDlDciQCount)
				{
					dlQosStrategyProcessExplicitDCIDLQueueForCAUes( 
							explicitDlDciQCount,
							numberOfAvailableRBs,
							currentGlobalTTITickCount,
							tempExplicitDLDCIScheduleQueue_p[cellIndex], 
							numDlUEsToBeScheduled_p,
							cellIndex);
				}
			}
		}
	}
}

/**********************************************************************************
 * Function Name  : dlQosStrategyProcessExplicitDCIDLQueueForCAUes 
 * Inputs         : explicitDlDciQCount - This is Count of the total nodes
 *                                        present in the
 *                                        explicitDLPucchTpcScheduleQueue_g,
 *                  numberOfAvailableRBs_p - pointer to current available RB's,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  tempExplicitDLDCIScheduleQueue_p - Pointer to explicit
 *                                                     DL_PUCCH_TPC_SCHEDULE_TYPE,
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : 
 * Returns        : None
 * Description    : 
 *******************************************************************************/
/*SPR 22919 Start*/
 void dlQosStrategyProcessExplicitDCIDLQueueForCAUes( 
/*SPR 22919 End*/
		UInt32 explicitDlDciQCount,
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE * tempExplicitDLDCIScheduleQueue_p,
		UInt8   *numDlUEsToBeScheduled_p,
		InternalCellIndex internalCellIndex)
{
	UInt8   userLocationType                      = CC_USER;
	UInt8   loopCount                             = 0;
	UInt8   loopIndex                             = 0;
	UInt8   listIndex                             = 0;
	UInt16  ueIndex                               = 0; 
	SInt8   totalNumDlUEsToBeScheduled            = 0;

	ExplicitDLDCIQueueNode* node_p                  = PNULL;
	DLUEContext*            ueDLContext_p           = PNULL;
	DLUEContextInfo*        ueDLContextInfo_p       = PNULL;
	DLStrategyTxNode*       dlStrategyTxNode_p      = PNULL;
	DLStrategyTxNode*       tempDlStrategyTxNode_p  = PNULL;
	ServingCellIndex        servingCellIndex        = INVALID_CELL_INDEX; 
	InternalCellIndex       pCellIndex              = INVALID_CELL_INDEX; 
	MacRetType              retVal                  = MAC_FAILURE;
	DLScheduleFlag*         scheduleFlag_p          = PNULL;

	SInt32  totalNumOfAvailableRBs[MAX_USER_LOCATION] = {0};

	/* get total number of UEs that can be scheduled */
	totalNumDlUEsToBeScheduled = MIN_BETWEEN_TWO(
			explicitDlDciQCount,
			( numDlUEsToBeScheduled_p[internalCellIndex] + 
			  numDlUEsToBeScheduled_p[!(internalCellIndex)]));

	totalNumOfAvailableRBs[CC_CE_REGION] = 
		numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] +
		numberOfAvailableRBs[!(internalCellIndex)][CC_CE_REGION];

	totalNumOfAvailableRBs[CE_REGION] = 
		numberOfAvailableRBs[internalCellIndex][CE_REGION] +
		numberOfAvailableRBs[!(internalCellIndex)][CE_REGION];

	while ( (totalNumDlUEsToBeScheduled) &&
			(totalNumOfAvailableRBs[CC_CE_REGION])&& (explicitDlDciQCount))
	{
		if (MAC_FAILURE == DEQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(
					tempExplicitDLDCIScheduleQueue_p,
					ExplicitDLDCIQueueNode, (void **)&node_p))
		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,LOGDEBUG,
					MAC_DL_Strategy,
					currentGlobalTTITickCount,
					explicitDlDciQCount,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");

			break;
		}

		ueIndex = node_p->ueIndex;    
		ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
		if (ueDLContextInfo_p->pendingDeleteFlag)
		{
			LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,LOGDEBUG,
					MAC_DL_Strategy,
					currentGlobalTTITickCount,
					ueIndex,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");


			FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
			explicitDlDciQCount--;
			continue; 
		}
		ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

		/*Checking if Already being considered for RB Distribution*/
		/*SPR 21948 Changes Start */
         /* Changes under spr 21762 have been reverted */
        if ( (DL_QOS_ALREADY_TRANSMITED >= ueDLContext_p->scheduleFlag) ||
				( (isUeEligibleForCaScheduling(ueDLContext_p)) && 
				  (DL_QOS_ALREADY_TRANSMITED >= ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag)))
          /*SPR 21948 Changes End */
		{
			FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
			explicitDlDciQCount--;
			continue;
		}

		retVal = dlQosGetPreferedCellListForScheduling(
				ueIndex, 
				TRUE /*isMacCEScheduled*/,
                                            &listIndex,
                                            /* SPR 16116 fix start */
                                            0);
                                            /* SPR 16116 fix end */

		if (MAC_SUCCESS == retVal)
		{
			pCellIndex = ueDLContext_p->internalCellIndex;
			loopCount = dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].count;

			for(loopIndex = 0; loopIndex < loopCount; loopIndex++)
			{
				servingCellIndex = dlQoSPreferredCellList_g[pCellIndex].preferredCellList[listIndex].\
						   preferredCellListInfo[loopIndex].cellIndex;
				/*EICIC +*/                
				/*Complexity Start*/
				if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))||((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{              
					if(!ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(tempExplicitDLDCIScheduleQueue_p,
								ExplicitDLDCIQueueNode,&(node_p->nodeAnchor)))
					{
						FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
					}
					explicitDlDciQCount--;
					continue;

				}
				/*Complexity End*/
				/* EICIC -*/
				if(pCellIndex == servingCellIndex)
				{
					/* \code
					 * if schType == SPS_SCHEDULING, then do not perform
					 * scheduling for this UE on this serving cell.
					 * */
					if( SPS_SCHEDULING == ueDLContext_p->schType )
					{
						if(loopIndex == (loopCount -1))
						{
							dlQosHandleExplicitDciSchedulingFailureCase( internalCellIndex,
									node_p);
							explicitDlDciQCount--;
						}
						continue;
					}
					userLocationType = ueDLContext_p->userLocationType;
					tempDlStrategyTxNode_p = (ueDLContext_p->dlStrategyTxNode_p);
					scheduleFlag_p = &(ueDLContext_p->scheduleFlag);
				}
				else
				{
					userLocationType = 
						ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->userLocationType;
					tempDlStrategyTxNode_p = 
						(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p);
					scheduleFlag_p = &(ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag);
				}

				/* Check if no cell edge ue available at that time and in new traversal there 
				 * is a cell edge ue and number of required available rb not available */
				if ( (!numberOfAvailableRBs[servingCellIndex][userLocationType])  || 
						(!numDlUEsToBeScheduled_p[servingCellIndex]) ||
						( (!maxCellEdgeUEsToBeScheduledDL_g[servingCellIndex]) && 
						  (CE_USER == userLocationType)))
				{
					if(loopIndex == (loopCount -1))
					{
						dlQosHandleExplicitDciSchedulingFailureCase( internalCellIndex,
								node_p);
						explicitDlDciQCount--;
					}
					continue;                    
				}    

				if(PNULL == tempDlStrategyTxNode_p)
				{
					dlStrategyTxNode_p =
						dlQosStrategyUpdateNewTxUEList( ueDLContext_p,
								TPC_MSG, 
								NEW_TRAVERSAL,
								currentGlobalTTITickCount,
								servingCellIndex);
					if(pCellIndex == servingCellIndex)
					{
						ueDLContext_p->dlStrategyTxNode_p = dlStrategyTxNode_p;
					}
					else
					{
						ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p = dlStrategyTxNode_p;
					}
				}
				else
				{
					dlStrategyTxNode_p = tempDlStrategyTxNode_p;
				}

				if(PNULL == dlStrategyTxNode_p)
				{
					/* If there are no FREE HARQ Process to schedule UE
					 * get the next node and continue 
					 */
					if(loopIndex == (loopCount -1))
					{
						LOG_MAC_MSG(MAC_ALL_HARQ_ARE_BUSY,LOGFATAL,MAC_DL_Strategy,
								currentGlobalTTITickCount,
								ueIndex,DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_INT_VALUE,
								DEFAULT_FLOAT_VALUE,
								DEFAULT_FLOAT_VALUE,
								FUNCTION_NAME, "No FREE HARQ Available");

						FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
						explicitDlDciQCount--;
					}
					continue;
				}

				dlStrategyTxNode_p->minTBSize =  LENGTH_OF_TPC;
				dlStrategyTxNode_p->maxTBSize =  LENGTH_OF_TPC;

				*scheduleFlag_p = DL_QOS_ALREADY_TRANSMITED;
				/* ICIC changes starts */                        
				/* According to user location , subtract from those available RB */
				numberOfAvailableRBs[servingCellIndex][userLocationType] -= 
					MIN_RB_FOR_TPC_ONLY;

				totalNumOfAvailableRBs[userLocationType] -= 
					MIN_RB_FOR_TPC_ONLY;

				/* then update RBs for both cell centered and cell edge */
				RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(
						MIN_RB_FOR_TPC_ONLY,
						numberOfAvailableRBs[servingCellIndex]);
				/* ICIC changes ends */                        

				dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB = 
					MIN_RB_FOR_TPC_ONLY;

				LOG_MAC_MSG(DL_EXPLICIT_Q_PROCESS_ID, LOGDEBUG, MAC_UL_PC,
						currentGlobalTTITickCount,
						ueIndex,
						explicitDlDciQCount,
						MIN_RB_FOR_TPC_ONLY,
						dlStrategyTxNode_p->harqProcessId,
						DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME, "DL_EXPLICIT_QOS_Q_PROCESS");


				numDlUEsToBeScheduled_p[servingCellIndex]--;
				totalNumDlUEsToBeScheduled--;

				/* Update Cell Edge Ue */
				if ( CE_USER == userLocationType )
				{
					--maxCellEdgeUEsToBeScheduledDL_g[servingCellIndex];
				}

				FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
				explicitDlDciQCount--;

				/* No need to go for next serving cell */
				break;
			}
		}
		else
		{
			/* Currently the failure condition pending deletion is 
			 * already taken care */
		}
	}
}

/*****************************************************************************
 * Function Name  : isMacCEAlreadyScheduledForUE 
 * Inputs         : ueIndex      
 *                : listIndex,internalCellIndex
 * Outputs        : 
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This func will check whether UE already considered for
 *                  MAC CE scheduling or not
 *****************************************************************************/
 MacRetType isMacCEAlreadyScheduledForUE( InternalCellIndex internalCellIndex,
		UInt16 ueIndex,
		UInt8 listIndex) 
{
	UInt8 idx               = 0;
	/*SPR 21769 Changes Start*/
	UInt16 ueCount           = 0; 
	/*SPR 21769 Changes End*/
	UInt8 schdeuledUeCount  = 0; 
	UInt8 cellCount         = dlQoSPreferredCellList_g[internalCellIndex].preferredCellList[listIndex].count;

	ServingCellIndex    servingCellIndex    = INVALID_CELL_INDEX;
	DLUeScheduleInfo*   dlUeScheduleInfo_p  = PNULL;

    /*SPR 20899 Fix Start */
	UInt8 strategyContainerIndex = MODULO_TWO(getCurrentTick()); 
    /*SPR 20899 Fix Stop */

	for(idx = 0; idx < cellCount ; idx++)
	{
		servingCellIndex = 
			dlQoSPreferredCellList_g[internalCellIndex].preferredCellList[listIndex].\
			preferredCellListInfo[idx].cellIndex;
		schdeuledUeCount = 
			dlStrategyScheduleInfo_g[strategyContainerIndex][servingCellIndex].schdeuledUeCount;

		for(ueCount = 0; ueCount < schdeuledUeCount ; ueCount++)
		{
			dlUeScheduleInfo_p = 
				&(dlStrategyScheduleInfo_g[strategyContainerIndex][servingCellIndex].dlUeScheduleInfo[ueCount]);

			if( (dlUeScheduleInfo_p->ueDLContext_p->ueIndex == ueIndex ) &&
					( (dlUeScheduleInfo_p->isTaDrxScheduled == TA_MSG) ||
					  (dlUeScheduleInfo_p->isTaDrxScheduled == DRX_MSG) ||
					  (dlUeScheduleInfo_p->isTaDrxScheduled == SCELL_ACT_DEACT_MSG)))
			{
				return MAC_SUCCESS; 
			}
		}
	}
	return MAC_FAILURE;
}

/*****************************************************************************
 * Function Name  : dlQosUpdatePreferredCellList 
 * Inputs         : ueIndex 
 *                : listIndex,internalCellIndex
 * Outputs        : 
 * Returns        : None
 * Description    : This func with check whether UE is present in other 
 *                  serving cells apart from one used to scheduled MAC CE.
 *****************************************************************************/
 void dlQosUpdatePreferredCellList( UInt16 ueIndex, 
		UInt8 listIndex,
                                          InternalCellIndex internalCellIndex,
                                          /* SPR 16116 fix start */
                                          UInt32 totLCWeightWithLP) 
                                          /* SPR 16116 fix end */
{
	UInt8 loopIdx                   = 0;
	UInt8 transmissionMode          = 0;
	UInt8 isUEPresentInTopSchList   = FALSE;

	DLUEContextInfo*  ueDLContextInfo_p  = PNULL;
	DLUEContext*      ueDLContext_p      = PNULL;
	MacRetType        macCeRetVal        = MAC_FAILURE;

	ServingCellIndex  servingCellIndex       = INVALID_CELL_INDEX;
	InternalCellIndex cellIdWithHighestMcs   = INVALID_CELL_INDEX;
	DLScheduleFlag    scheduleFlag           = DL_QOS_NEW_TRANSMISSION;

	DLStrategyTxNode* tempDlStrategyTxNode_p = PNULL;

	ueDLContextInfo_p   = &dlUECtxInfoArr_g[ueIndex];
	ueDLContext_p       = ueDLContextInfo_p->dlUEContext_p;

	cellIdWithHighestMcs = getCellIdWithHighestMcs(ueDLContext_p);
	/*coverity id 63741*/
	for(loopIdx = 0; 
			loopIdx < dlQoSPreferredCellList_g[internalCellIndex].preferredCellList[listIndex].count ; 
	   )
	{
		servingCellIndex = 
			dlQoSPreferredCellList_g[internalCellIndex].preferredCellList[listIndex].\
			preferredCellListInfo[loopIdx].cellIndex;

		if(IS_CA_ENABLED())
		{
			/* EICIC +*/
			/*Complexity Start*/
			if (((VICTIM_UE == ueTypeToScheduleDL_g[servingCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType)) || ((NON_VICTIM_UE == ueTypeToScheduleDL_g[servingCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
			{              
				continue;
			}
		}
		/*Complexity End*/
		/* EICIC -*/


		if(IS_PCELL_OF_UE(ueDLContext_p, servingCellIndex))
		{
			scheduleFlag = ueDLContext_p->scheduleFlag;
			transmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;
			tempDlStrategyTxNode_p = ueDLContext_p->dlStrategyTxNode_p;
		}
		else
		{
			scheduleFlag = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scheduleFlag;
			transmissionMode = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo.transmissionMode;
			tempDlStrategyTxNode_p = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p;
		}

		if(DL_QOS_NO_NEW_TRANSMISSION >= scheduleFlag)
		{
			removeCellFromPreferredCellList(
					internalCellIndex, 
					listIndex, 
					servingCellIndex);
			/*coverity id 63741*/
			continue;
		}
		else
		{
			macCeRetVal = isMacCEAlreadyScheduledForUE( 
					internalCellIndex, 
					ueIndex,
					listIndex); 

			if( (scheduleFlag >= DL_QOS_ALREADY_TRANSMITED) &&
					(!macCeRetVal) &&
					(servingCellIndex != cellIdWithHighestMcs) &&
					(PNULL != tempDlStrategyTxNode_p) &&
					( ( ( (TX_MODE_3 == transmissionMode) ||
					      (TX_MODE_4 == transmissionMode) ||
					      (TX_MODE_8 == transmissionMode)) && 
					    (!((RE_TX == tempDlStrategyTxNode_p->harqProcess_p->dlHARQTBOneInfo.txIndicator) && 
					       (RE_TX == tempDlStrategyTxNode_p->harqProcess_p->dlHARQTBTwoInfo.txIndicator)))) ||
					  ( (!((TX_MODE_3 == transmissionMode) || 
					       (TX_MODE_4 == transmissionMode) ||
					       (TX_MODE_8 == transmissionMode))) && 
					    (RE_TX != tempDlStrategyTxNode_p->harqProcess_p->dlHARQTBOneInfo.txIndicator))))
			{
                /* SPR 16116 fix start */

				isUEPresentInTopSchList = checkTopSchedulableUEInfoList( 
						servingCellIndex, 
						ueIndex, 
                                                totLCWeightWithLP);
                /* SPR 16116 fix end */
				if(!isUEPresentInTopSchList)
				{
					removeCellFromPreferredCellList(
							internalCellIndex, 
							listIndex, 
							servingCellIndex);
					continue;
				}
			}
		}
		/*coverity id 63741*/
		loopIdx++;
	}
}

/*****************************************************************************
 * Function Name  : removeCellFromPreferredCellList 
 * Inputs         : internalCellIndex,listIndex, 
 *                : servingCellIndex
 * Outputs        : 
 * Returns        : None 
 * Description    : This func will remove servingCellIndex from the 
 *                  preferredCellList of the UE.
 *****************************************************************************/
 void removeCellFromPreferredCellList( InternalCellIndex internalCellIndex, 
		UInt8 listIndex, 
		ServingCellIndex  servingCellIndex)
{
	UInt8 cellCount     = dlQoSPreferredCellList_g[internalCellIndex].preferredCellList[listIndex].count;
	UInt8 loopIdx       = 0;
	UInt8 innerLoopIdx  = 0;

	InternalCellIndex cellId = INVALID_CELL_INDEX;

	for(loopIdx = 0; loopIdx < cellCount ; loopIdx++)
	{
		cellId = dlQoSPreferredCellList_g[internalCellIndex].preferredCellList[listIndex].\
			 preferredCellListInfo[loopIdx].cellIndex;
		if(cellId == servingCellIndex)
		{
			for(innerLoopIdx = loopIdx; innerLoopIdx < cellCount - 1; innerLoopIdx++)
			{
				dlQoSPreferredCellList_g[internalCellIndex].preferredCellList[listIndex].\
					preferredCellListInfo[innerLoopIdx].cellIndex = 
					dlQoSPreferredCellList_g[internalCellIndex].preferredCellList[listIndex].\
					preferredCellListInfo[innerLoopIdx + 1].cellIndex;

			}
			dlQoSPreferredCellList_g[internalCellIndex].preferredCellList[listIndex].count--;
			break;
		}
	}
}


/**********************************************************************************
 * Function Name  : dlQosStrategyProcessDlCAMacCEQueueofAllCells
 * Inputs         : tempDlCAMacCEQueue_p 
 *                  numberOfAvailableRBs_p - pointer to current available RB's,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled
 *                  internalCellIndex
 *                  flagForPendingScellCE
 * Outputs        : 
 * Returns        : None
 * Description    : 
 *******************************************************************************/
 void dlQosStrategyProcessDlCAMacCEQueueofAllCells( 
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		DL_CA_MAC_CE_QUEUE_TYPE** tempDlCAMacCEQueue_p,
		UInt8   *numDlUEsToBeScheduled_p,
        /* +- SPR 17777 */
		FlagForPendingScellCE flagForPendingScellCE) 
        /* +- SPR 17777 */
{
	UInt16              qCount      = 0;
	InternalCellIndex   cellIndex   = 0;    

	for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
	{
		if(PNULL != tempDlCAMacCEQueue_p[cellIndex])
		{
			qCount = COUNT_DL_CA_MAC_CE_Q(tempDlCAMacCEQueue_p[cellIndex]);

			if(qCount)
			{
				dlQosStrategyProcessDlCAMacCEQueue( 
						qCount,
						numberOfAvailableRBs,
						currentGlobalTTITickCount,
						tempDlCAMacCEQueue_p[cellIndex], 
						numDlUEsToBeScheduled_p,
						flagForPendingScellCE,
						cellIndex);
			}
		}
	}
}


/**********************************************************************************
 * Function Name  : dlQosStrategyProcessDlCAMacCEQueue 
 * Inputs         : dlCAMacCEQCount - This is Count of the total nodes
 *                                    present in the dlCAMacCEQueue_gp
 *                  numberOfAvailableRBs_p - pointer to current available RB's,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  tempDlCAMacCEQueue_p
 *                  numDlUEsToBeScheduled_p - No. of DL UE's to be scheduled
 *                  internalCellIndex
 *                  flagForPendingScellCE
 * Outputs        : 
 * Returns        : None
 * Description    : 
 *******************************************************************************/
 void dlQosStrategyProcessDlCAMacCEQueue( 
		UInt32 dlCAMacCEQCount,
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
		/* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
		DL_CA_MAC_CE_QUEUE_TYPE* tempDlCAMacCEQueue_p,
		UInt8   *numDlUEsToBeScheduled_p,
		FlagForPendingScellCE flagForPendingScellCE,
		InternalCellIndex internalCellIndex)
{
	UInt8   userLocationType            = CC_USER;
	UInt8   listIndex                   = 0;
	UInt16  ueIndex                     = 0; 
	SInt8   totalNumDlUEsToBeScheduled  = 0;

	DLCAMacCEQueueNode*     node_p                  = PNULL;
	DLUEContext*            ueDLContext_p           = PNULL;
	DLUEContextInfo*        ueDLContextInfo_p       = PNULL;
	DLStrategyTxNode*       dlStrategyTxNode_p      = PNULL;
	DLStrategyTxNode*       tempDlStrategyTxNode_p  = PNULL;
	InternalCellIndex       servingCellIndex        = INVALID_CELL_INDEX; 
	InternalCellIndex       pCellIndex              = INVALID_CELL_INDEX; 
	MacRetType              retVal                  = MAC_FAILURE;
	DLScheduleFlag          scellScheduleFlag       = DL_QOS_NEW_TRANSMISSION;
	DLScheduleFlag*         scheduleFlag_p          = PNULL;

	SInt32  totalNumOfAvailableRBs[MAX_USER_LOCATION] = {0};

	/* get total number of UEs that can be scheduled */
	totalNumDlUEsToBeScheduled = MIN_BETWEEN_TWO(
			dlCAMacCEQCount,
			( numDlUEsToBeScheduled_p[internalCellIndex] + 
			  numDlUEsToBeScheduled_p[!(internalCellIndex)]));

	totalNumOfAvailableRBs[CC_CE_REGION] = 
		numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] +
		numberOfAvailableRBs[!(internalCellIndex)][CC_CE_REGION];

	totalNumOfAvailableRBs[CE_REGION] = 
		numberOfAvailableRBs[internalCellIndex][CE_REGION] +
		numberOfAvailableRBs[!(internalCellIndex)][CE_REGION];
 	/* + SPR 22352 fix */
    	MsgType msgType = INVALID_MSG;
    	/* - SPR 22352 fix */

	while ( (totalNumDlUEsToBeScheduled && dlCAMacCEQCount) &&
			(totalNumOfAvailableRBs[CC_CE_REGION]))
	{
		if (MAC_FAILURE == DEQUEUE_DL_CA_MAC_CE_Q(
					tempDlCAMacCEQueue_p,
					(void **)&node_p))

		{
			LOG_UT(MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,
					LOGDEBUG, MAC_DL_Strategy,
					currentGlobalTTITickCount, dlCAMacCEQCount,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");

			break;
		}

		ueIndex = node_p->ueId;   
		ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
		if (ueDLContextInfo_p->pendingDeleteFlag)
		{
			LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,
					LOGDEBUG, MAC_DL_Strategy,
					currentGlobalTTITickCount, ueIndex,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");


			FREE_MEM_NODE_DL_CA_MAC_CE_Q(node_p);
			dlCAMacCEQCount--;
			continue; 
		}
		ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
		/*spr 12510 fix start*/
		if (ueDLContext_p->dlUeScellContext_p[node_p->scellId] == PNULL)
		{
			LOG_UT(MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,
					LOGDEBUG, MAC_DL_Strategy,
					currentGlobalTTITickCount, ueIndex,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");


			FREE_MEM_NODE_DL_CA_MAC_CE_Q(node_p);
			dlCAMacCEQCount--;
			continue; 
		}
		/*spr 12510 fix end*/

		if( ( (DL_CA_SCELL_CONFIGURED !=
						ueDLContext_p->dlUeScellContext_p[node_p->scellId]->scellState) &&
					(DL_CA_EVENT_MAC_SCELL_ACT_CE_TRIGGER == node_p->dlCAQosEvent)) ||
				( (DL_CA_SCELL_ACTIVE != 
				   ueDLContext_p->dlUeScellContext_p[node_p->scellId]->scellState) &&
				  (DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER == node_p->dlCAQosEvent)))
		{           
			dlCAScellStateManagerRegenerateMacCEEvent(internalCellIndex,node_p);
			FREE_MEM_NODE_DL_CA_MAC_CE_Q(node_p);
			dlCAMacCEQCount--;
			continue;
		}
		scellScheduleFlag = ueDLContext_p->dlUeScellContext_p[node_p->scellId]->scheduleFlag;
	        /* + SPR 22352 fix */
        	/* Check if TA is also being scheduled in this tick then,Scell Act/Deact MAC CE
       		 * handling is already done while processing TA Queue, so need to free the DLCA 
         	 * MAC CE Node from queue.
         	 * */
        	if((DL_QOS_ALREADY_TRANSMITED == ueDLContext_p->scheduleFlag) && 
                     (ueDLContext_p->TaDrxScellBitmap & TA_BIT))
		{
			FREE_MEM_NODE_DL_CA_MAC_CE_Q(node_p);
			dlCAMacCEQCount--;
			continue;
		}
	        /* - SPR 22352 fix */

		/*Checking if Already being considered for RB Distribution*/
		if( (DL_QOS_ALREADY_TRANSMITED > ueDLContext_p->scheduleFlag) && 
				( ( (IS_SCELL_ACTIVE(ueDLContext_p->ueIndex,node_p->scellId )) && 
				    (DL_QOS_ALREADY_TRANSMITED > scellScheduleFlag)) ||
				  ( !(IS_SCELL_ACTIVE(ueDLContext_p->ueIndex,node_p->scellId)))))
		{
			dlQosHandleSCellCESchedulingFailureCase( internalCellIndex,
					flagForPendingScellCE,
					node_p);
			dlCAMacCEQCount--;
			continue;
		}
		/* SPR 22493 fix start */
		/* if already transmitted, then  push this node in failure queue */
		else if((DL_QOS_ALREADY_TRANSMITED == ueDLContext_p->scheduleFlag) ||
			( (IS_SCELL_ACTIVE(ueDLContext_p->ueIndex,node_p->scellId )) && 
			(DL_QOS_ALREADY_TRANSMITED == scellScheduleFlag)))
		{
			dlQosHandleSCellCESchedulingFailureCase( internalCellIndex,
					flagForPendingScellCE,
					node_p);
			dlCAMacCEQCount--;
			continue;
		}
		/* SPR 22493 fix end */

		else
		{
			retVal = dlQosGetPreferedCellListForScheduling(
					ueIndex, 
					TRUE /*isMacCEScheduled*/,
                                                &listIndex,
                                                /* SPR 16116 fix start */
                                                0);
                                                /* SPR 16116 fix end */

			if(MAC_SUCCESS == retVal)
			{
				pCellIndex = ueDLContext_p->internalCellIndex;

				/* + Coverity 69700 */
				/* SPR 15140 Start */
				servingCellIndex = pCellIndex;
				/* SPR 15140 Start */
				/* EICIC +*/

				/* Pushing UE back in  queue.*/
				/*Complexity Start*/
				if (((VICTIM_UE == ueTypeToScheduleDL_g[servingCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType)) || 
					((NON_VICTIM_UE == ueTypeToScheduleDL_g[servingCellIndex]) && (VICTIM_UE == ueDLContext_p->userType)))
				{              
					/* + SPR 22352 fix */
					SET_SCELL_ACT_DEACT_BIT(node_p->ueId,node_p->scellId,node_p->dlCAQosEvent);
					/* - SPR 22352 fix */
					if (MAC_FAILURE == ENQUEUE_DL_CA_MAC_CE_Q(
								tempDlCAMacCEQueue_p,
								(void *)node_p))
					{
						FREE_MEM_NODE_DL_CA_MAC_CE_Q(node_p);
					}
					dlCAMacCEQCount--;
					continue;
				}

				/* EICIC - */
				/*Complexity End*/

				if(pCellIndex == servingCellIndex)
				{
					if( SPS_SCHEDULING == ueDLContext_p->schType )
					{
						dlQosHandleSCellCESchedulingFailureCase( internalCellIndex,
								flagForPendingScellCE,
								node_p);
						dlCAMacCEQCount--;
						continue;
					}
					userLocationType = ueDLContext_p->userLocationType;
					tempDlStrategyTxNode_p = (ueDLContext_p->dlStrategyTxNode_p);
					scheduleFlag_p = &(ueDLContext_p->scheduleFlag);
				}
					/*Coverity 108783 Fix */

				if(DL_QOS_ALREADY_TRANSMITED > *scheduleFlag_p)
				{
					dlQosHandleSCellCESchedulingFailureCase( internalCellIndex,
							flagForPendingScellCE,
							node_p);
					dlCAMacCEQCount--;
					continue;
				}
				/* Check if no cell edge ue available at that time and in new traversal there 
				 * is a cell edge ue and number of required available rb not available */
				if ( (!numberOfAvailableRBs[servingCellIndex][userLocationType])  ||
						(!numDlUEsToBeScheduled_p[servingCellIndex]) ||
						( (!maxCellEdgeUEsToBeScheduledDL_g[servingCellIndex]) && 
						  (CE_USER == userLocationType)))
				{
					dlQosHandleSCellCESchedulingFailureCase( internalCellIndex,
							flagForPendingScellCE,
							node_p);
					dlCAMacCEQCount--;
					continue;                    
				}    
				if(PNULL == tempDlStrategyTxNode_p)
				{
					/* + SPR 22352 fix */
					/* check if in the same TTI, DRX command also needs to be scheduled,
					 * in that case, set the msgType as DRX_SCELL_ACT_DEACT_MSG */

					checkMsgTypeTaDrxScellActDeact(ueDLContext_p,SCELL_ACT_DEACT_MSG,&msgType,servingCellIndex);

					dlStrategyTxNode_p = 
						dlQosStrategyUpdateNewTxUEList( ueDLContext_p,
								msgType,
								NEW_TRAVERSAL,
								currentGlobalTTITickCount,
								servingCellIndex);
					/* - SPR 22352 fix */

					if(pCellIndex == servingCellIndex)
					{
						ueDLContext_p->dlStrategyTxNode_p = dlStrategyTxNode_p;
					}
					else
					{
						ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->dlStrategyTxNode_p = dlStrategyTxNode_p;
					}
				}
				else
				{
					dlStrategyTxNode_p = tempDlStrategyTxNode_p;
				}

				if(PNULL == dlStrategyTxNode_p)
				{
					/* If there are no FREE HARQ Process to schedule UE
					 * get the next node and continue 
					 */
					LOG_MAC_MSG(MAC_ALL_HARQ_ARE_BUSY,
							LOGFATAL, MAC_DL_Strategy,
							currentGlobalTTITickCount, ueIndex,
							DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
							DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
							DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
							FUNCTION_NAME, "No FREE HARQ Available");

					dlQosHandleSCellCESchedulingFailureCase( internalCellIndex,
							flagForPendingScellCE,
							node_p);
					dlCAMacCEQCount--;
					continue;
				}

				/*  Spec 36.321 - a90
				 *  Figure 6.1.3.8-1: Activation/Deactivation MAC control element  
				 *
				 * |--|--|--|--|--|--|--|--|
				 * |C7|C6|C5|C4|C3|C2|C1|R |
				 */
				if(DL_CA_EVENT_MAC_SCELL_ACT_CE_TRIGGER == node_p->dlCAQosEvent)
				{
					/* SET_BYTE_FIELD(data_var, value, position, numbits */
					SET_BYTE_FIELD( dlStrategyTxNode_p->harqProcess_p->scellActBitmask, 
							1, 
							node_p->scellId, 
							1);
				}
				else if(DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER == node_p->dlCAQosEvent)
				{
					SET_BYTE_FIELD( dlStrategyTxNode_p->harqProcess_p->scellDeactBitmask, 
							1, 
							node_p->scellId, 
							1);
				}

		                /* + SPR 22352 fix */
				*scheduleFlag_p = DL_QOS_ALREADY_TRANSMITED;

                		updateRbAndTBSizeInDlStrategyNode(msgType,dlStrategyTxNode_p,numberOfAvailableRBs[servingCellIndex],userLocationType,ueIndex);

		                if(DRX_SCELL_ACT_DEACT_MSG == msgType)
				{
					totalNumOfAvailableRBs[userLocationType] -= 
						MIN_RB_FOR_BOTH_TA_DRX_ONLY; /* 2 RB*/
				}
		                else /* SCELL_ACT_DEACT_MSG*/
				{
					totalNumOfAvailableRBs[userLocationType] -= 
						MIN_RB_FOR_SCELL_ACT_DEACT_ONLY;
				}
		                /* - SPR 22352 fix */

				numDlUEsToBeScheduled_p[servingCellIndex]--;
				totalNumDlUEsToBeScheduled--;

				LOG_MAC_MSG(MAC_SCHD_SCELL_ACT_DEACT_CE,LOGWARNING,MAC_DL_Strategy,
						getCurrentTick(),
						ueDLContext_p->ueIndex,
						pCellIndex,
						dlStrategyTxNode_p->harqProcess_p->harqProcessId,
						dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB,
						node_p->dlCAQosEvent,
						DEFAULT_FLOAT_VALUE,
						DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME, "");

				/* Update Cell Edge Ue */
				if ( CE_USER == userLocationType )
				{
					--maxCellEdgeUEsToBeScheduledDL_g[servingCellIndex];
				}
				FREE_MEM_NODE_DL_CA_MAC_CE_Q(node_p);
				dlCAMacCEQCount--;

				/* No need to go for next serving cell */
				break;
				/* - Coverity 69700 */
			}
			else
			{
				/* Currently the failure condition pending deletion is 
				 * already taken care */
			}
		}
	}
}

/*****************************************************************************
 * Function Name  : putEntryInPendingFailureDlCAMacCEQueue 
 * Inputs         : ueIndex
 *                  dlHarqProcess_p
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
void putEntryInPendingFailureDlCAMacCEQueue( UInt16 ueIndex,
		DLHARQProcess* dlHarqProcess_p,
		InternalCellIndex internalCellIndex)
{
	DLCAMacCEQueueNode* node_p       = PNULL;
	InternalCellIndex   scellIndex   = INVALID_CELL_INDEX;
	DLCAQosEvent        dlCAQosEvent = INVALID_DL_CA_EVENT_MAC_SCELL_CE;

	GET_MEM_NODE_DL_CA_MAC_CE_Q(node_p, DLCAMacCEQueueNode);

	if (PNULL != node_p)
	{
		if(dlHarqProcess_p->scellActBitmask)
		{
			/* Need to update in case there are more than 1 scell */
			scellIndex = dlHarqProcess_p->scellActBitmask >> 1;
			dlCAQosEvent = DL_CA_EVENT_MAC_SCELL_ACT_CE_TRIGGER;
		}
		else
		{
			scellIndex = dlHarqProcess_p->scellDeactBitmask >> 1;
			dlCAQosEvent = DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER;
		}

		node_p->ueId = ueIndex;
		node_p->scellId = scellIndex;
		node_p->dlCAQosEvent = dlCAQosEvent;

		/* + SPR 22352 fix */
		SET_SCELL_ACT_DEACT_BIT(node_p->ueId,node_p->scellId,node_p->dlCAQosEvent);
		/* - SPR 22352 fix */
		if (MAC_FAILURE == ENQUEUE_DL_CA_MAC_CE_Q(
					pendingFailureDlCAMacCEQueue_gp[internalCellIndex],
					(void *)node_p))
		{
			dlCAScellStateManagerRegenerateMacCEEvent(internalCellIndex,node_p);
			FREE_MEM_NODE_DL_CA_MAC_CE_Q(node_p);
		}
	}
	else
	{
		/* Not able to allocate Memory */
	}
}

/*****************************************************************************
 * Function Name  : dlQosResetPreferredCellList
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void dlQosResetPreferredCellList(InternalCellIndex internalCellIndex)
{
	UInt8  listIndex = 0;
    /* +- SPR 18268 */
	UInt16  ueCount   = 0; 
    /* +- SPR 18268 */
	ServingCellIndex servingCellIndex = 0;

	if(!IS_CA_ENABLED())
	{
		ueCount = dlQoSPreferredCellList_g[internalCellIndex].preferredCellListCount;
		for(listIndex = 0; listIndex < ueCount; listIndex++)
		{
			dlQoSPreferredCellList_g[internalCellIndex].preferredCellList[listIndex].count = 0;
		}
		dlQoSPreferredCellList_g[internalCellIndex].preferredCellListCount = 0;
	}
	else
	{
		for(servingCellIndex = 0; 
				servingCellIndex < cellSpecificParams_g.numOfCells ; 
				servingCellIndex++)
		{
			ueCount = dlQoSPreferredCellList_g[servingCellIndex].preferredCellListCount;
			for (listIndex = 0; listIndex < ueCount; listIndex++)
			{
				dlQoSPreferredCellList_g[servingCellIndex].preferredCellList[listIndex].count = 0;
			}
			dlQoSPreferredCellList_g[servingCellIndex].preferredCellListCount = 0;
		}
	}
}

/*****************************************************************************
 * Function Name  : getServeCellIndexFromInternalCellId 
 * Inputs         : ueDLContext_p
 *                  cellIndex
 * Outputs        : None
 * Returns        : 
 * Description    : 
 *****************************************************************************/
UInt8 getServeCellIndexFromInternalCellId( DLUEContext* ueDLContext_p, 
		InternalCellIndex cellIndex)
{
	UInt8 loopIndex = START_SCELL_INDEX;
	for(loopIndex = START_SCELL_INDEX; loopIndex <=  ueDLContext_p->scellCount; loopIndex++)
	{
		if(ueDLContext_p->dlUeScellContext_p[loopIndex])
		{
			if(cellIndex == ueDLContext_p->dlUeScellContext_p[loopIndex]->internalCellIndex)
			{
				return loopIndex;
			}
		}
	}
	return INVALID_8_VAL;
}

/*****************************************************************************
 * Function Name  : getInternalCellIdFromServeCellId 
 * Inputs         : ueDLContext_p
 *                  servCellIndex
 * Outputs        : None
 * Returns        : internalCellIndex
 * Description    : 
 *****************************************************************************/
 InternalCellIndex getInternalCellIdFromServeCellId( DLUEContext* ueDLContext_p, 
		InternalCellIndex servCellIndex)
{
	/* + SPR_14522 */
	if (ueDLContext_p->dlUeScellContext_p[servCellIndex])
	{    
		return (ueDLContext_p->dlUeScellContext_p[servCellIndex]->internalCellIndex);
	}

	return INVALID_8_VAL;
	/* - SPR_14522 */
}

/*****************************************************************************
 * Function Name  : dlQosHandleTASchedulingFailureCase 
 * Inputs         : internalCellIndex
 *                  flagForTADRXPendingFresh
 *                  taNode_p
 *                  ueDLContext_p
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void dlQosHandleTASchedulingFailureCase( InternalCellIndex internalCellIndex,
		FlagForPendingTADRX flagForTADRXPendingFresh,
		TimeAdvanceQueueNode* taNode_p
		/*SPR 9664 Fix Start*/
		,DLUEContext *ueDLContext_p
		/*SPR 9664 Fix End*/
		)
{
	if(flagForTADRXPendingFresh == PENDING_TA_FLAG)
	{
		FREE_MEM_NODE_TA_Q(taNode_p);
		/*SPR 9664 Fix Start*/
		/*if TA MAC CE could not be sent, Set TA average value to default value
		  and clear all accumulated history */
		ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
		/*SPR 9664 Fix End*/
	}
	else
	{
		if(!ENQUEUE_TA_Q( pendingTAQueue_gp[internalCellIndex],
					TimeAdvanceQueueNode,
					&(taNode_p->timeAdvanceQueueNodeAnchor)))
		{
			lteWarning("Error in Pushing in Pending"
					" TAQueue in funtion %s\n",
					__func__);
			FREE_MEM_NODE_TA_Q(taNode_p);
			/*SPR 9664 Fix Start*/
			/*if TA MAC CE could not be sent, Set TA average value to default value
			  and clear all accumulated history */
			ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
			/*SPR 9664 Fix End*/
		}
	}
}

/*****************************************************************************
 * Function Name  : dlQosHandleDRXSchedulingFailureCase 
 * Inputs         : internalCellIndex
 *                  drxNode_p
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void dlQosHandleDRXSchedulingFailureCase ( InternalCellIndex internalCellIndex,
		DRXQueueNode* drxNode_p)
{
	if(!ENQUEUE_DRX_Q( pendingDRXQueue_gp[internalCellIndex],
				DRXQueueNode, (void *) drxNode_p))
	{
		lteWarning("Error in Pushing in Pending"
				" DRXQueue in funtion %s\n",
				__func__);
		FREE_MEM_NODE_DRX_Q(drxNode_p);
	}
}

/*****************************************************************************
 * Function Name  : dlQosHandleExplicitDciSchedulingFailureCase 
 * Inputs         : internalCellIndex
 *                  node_p
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void dlQosHandleExplicitDciSchedulingFailureCase( InternalCellIndex internalCellIndex,
		ExplicitDLDCIQueueNode* node_p)
{
	if(!(ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(
					explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex],
					ExplicitDLDCIQueueNode, (void *) node_p)))
	{
		FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p);
	}
}


/*****************************************************************************
 * Function Name  : dlQosHandleSCellCESchedulingFailureCase 
 * Inputs         : internalCellIndex
 *                  flagForPendingScellCE
 *                  node_p
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void dlQosHandleSCellCESchedulingFailureCase( InternalCellIndex internalCellIndex,
		FlagForPendingScellCE flagForPendingScellCE,
		DLCAMacCEQueueNode* node_p)
{
	if(PENDING_SCELL_CE_FLAG == flagForPendingScellCE)
	{
		dlCAScellStateManagerRegenerateMacCEEvent(internalCellIndex,node_p);
		FREE_MEM_NODE_DL_CA_MAC_CE_Q(node_p);
	}
	else
	{
		/* + SPR 22352 fix */
		SET_SCELL_ACT_DEACT_BIT(node_p->ueId,node_p->scellId,node_p->dlCAQosEvent);
		/* - SPR 22352 fix */
		if (MAC_FAILURE == ENQUEUE_DL_CA_MAC_CE_Q(
					pendingFailureDlCAMacCEQueue_gp[internalCellIndex],
					(void *)node_p))
		{
			FREE_MEM_NODE_DL_CA_MAC_CE_Q(node_p);
		}
	}
}

/*****************************************************************************
 * Function Name  : dlQosInitStrategyParams 
 * Inputs         : numberOfAvailableRBs
 *                  numDlUEsToBeScheduled_p
 *                  internalCellIndex
 *                  dlDelay
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void dlQosInitStrategyParams( SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
		UInt8  *numDlUEsToBeScheduled_p,
		InternalCellIndex internalCellIndex)
{
	numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] = 
		( dlRAInfo_gp[internalCellIndex] + 
		  dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB;

	numberOfAvailableRBs[internalCellIndex][CE_REGION] = 
		( dlRAInfo_gp[internalCellIndex] + 
		  dlRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB;

	/* Decrement number of available RBs if reserved by SPS */
	numberOfAvailableRBs[internalCellIndex][CC_CE_REGION] -= 
		numDlSpsRbsInTti_g[internalCellIndex];

	numDlUEsToBeScheduled_p[internalCellIndex] = cellSpecificParams_g.\
						     cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
						     dlSchdConfig.maxUEsToBeScheduledDL;

	maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] = cellSpecificParams_g.\
							     cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
							     dlSchdConfig.maxCellEdgeUEsToBeScheduledDL;


}
/*****************************************************************************
 * Function Name  : dlQosPreStrategyExecution 
 * Inputs         : internalCellIndex
 *                  currentGlobalTTITickCount
 *                  dlDelay,
 *                  numberOfAvailableRBs,
 *                  numDlUEsToBeScheduled_p
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void dlQosPreStrategyExecution( InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount,
		/* SPR 15909 fix end */
		/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
		UInt32 dlDelay, 
#endif
		/* - SPS_TDD_Changes */
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
		UInt8   *numDlUEsToBeScheduled_p)
{
	SInt16 dlPriorityQueueCount = 0;

	/* Update the tokens */
    /* +- SPR 17777 */
	dlQosProcessTokenUpdate(internalCellIndex);
    /* +- SPR 17777 */

	dlQosProcessAllTriggerQueues( internalCellIndex,
			currentGlobalTTITickCount
			/* + SPS_TDD_CHANGES */
#ifdef FDD_CONFIG
			,dlDelay
#endif
			/* - SPS_TDD_CHANGES */
			);
	/*
	 *   Process the PSR UE List for each UE, 
	 */



	dlPriorityQueueCount = circQueueCount(dlPriorityQueue_gp[internalCellIndex]);

	/** SPR 13599 Changes Start **/ 
	if( (dlPriorityQueueCount) &&
			(numDlUEsToBeScheduled_p[internalCellIndex]) &&
			(numberOfAvailableRBs[internalCellIndex][CC_CE_REGION]) )
		/** SPR 13599 Changes End **/ 
	{
		dlQosProcessPriorityQueue( numberOfAvailableRBs[internalCellIndex],
				&numDlUEsToBeScheduled_p[internalCellIndex],
				dlPriorityQueueCount,
				currentGlobalTTITickCount,
				internalCellIndex);
	}
}

/*****************************************************************************
 * Function Name  : dlQosProcessAllTriggerQueues 
 * Inputs         : internalCellIndex
 *                  currentGlobalTTITickCount
 *                  dlDelay
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void dlQosProcessAllTriggerQueues( InternalCellIndex internalCellIndex,
		/* SPR 15909 fix start */
		tickType_t currentGlobalTTITickCount
		/* SPR 15909 fix end */
		/* + SPS_TDD_CHANGES */
#ifdef FDD_CONFIG
		,UInt32 dlDelay
#endif
		/* - SPS_TDD_CHANGES */
		) 
{
	SInt16 zeroNonZeroTriggerQueueCount = 0;
	SInt16 maxDrxTriggerCount           = 0;
	SInt16 measGapTriggerCount          = 0;
	SInt16 pdcchOrderCount              = 0;
	/* + SPR 11101 Changes */
	UInt16 dlCAEventQueueCount = 0; 
	/* - SPR 11101 Changes */
#ifdef FDD_CONFIG
#endif

	/*
	 * Process the RLC Zero to Non-Zero Queue
	 * The LC's with non-Zero queue load will be added into 
	 * Eligible list of the UE.
	 * It also add's LC into the priority LC tree if the UE is not in 
	 * Measurement GAP ON state, DRX ON state and UL Sync Loss on state
	 * 
	 */
	zeroNonZeroTriggerQueueCount = 
		COUNT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(internalCellIndex);

	if (zeroNonZeroTriggerQueueCount)
	{
		dlQosProcessZeroNonZeroTriggerQueue( 
				zeroNonZeroTriggerQueueCount,
                /* +- SPR 17777 */
				internalCellIndex,
				currentGlobalTTITickCount);
	}

	/*
	 *   Process the UL SYNC LOSS Trigger Queue. 
	 *   For each UE, Updated the lc's LC Strategy Node in the priority tree
	 *   based on the Trigger received
	 *   If the Trigger is for UL SYNC Loss OFF, then add all Eligible LC 
	 *   nodes into the LC priority tree, else delete all Eligible LC nodes  
	 *   from the Tree
	 */
	pdcchOrderCount = COUNT_UL_SYNC_LOSS_TRIGGERQ_DL_Q(internalCellIndex);

	if(pdcchOrderCount)
	{
		dlQosStrategyProcessPdcchOrderQueue( pdcchOrderCount, 
				/* + SPS_TDD_CHANGES */
#ifdef FDD_CONFIG
				dlDelay ,
#endif
				/* - SPS_TDD_CHANGES */
				internalCellIndex,
				currentGlobalTTITickCount);
	}

	/*
	 *   Process the DRX trigger queue. 
	 *   For each UE, Updated the lc's LC Strategy Node in the priority tree
	 *   based on the Trigger received
	 *   If the Trigger is for DRX OFF, then add all Eligible LC nodes into the
	 *   LC priority tree, else delete all Eligible LC nodes from the 
	 *   Tree
	 */
	maxDrxTriggerCount = COUNT_DL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex);

	if(maxDrxTriggerCount)
	{
		dlQosStrategyProcessDRXTrigQueue( maxDrxTriggerCount, 
				internalCellIndex,
				currentGlobalTTITickCount);
	}

	/*
	 *   Process the Measurement Gap trigger Queue. 
	 *   For each UE, Updated the lc's LC Strategy Node in the priority tree
	 *   based on the Trigger received
	 *   If the Trigger is for Measurement Gap OFF, then add all Eligible LC 
	 *   nodes into the LC priority tree, else delete all Eligible LC nodes  
	 *   from the Tree
	 */
	measGapTriggerCount = COUNT_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(internalCellIndex);

	if(measGapTriggerCount)
	{
		dlQosStrategyProcessMeasGapTrigQueue( measGapTriggerCount,
				internalCellIndex,
				currentGlobalTTITickCount);
	}
	/* + SPR 11101 Changes */
	/*processing scell state transition event Q*/
	/** SPR 13599 Changes Start **/ 
	if(IS_CA_ENABLED())
	{
		dlCAEventQueueCount = COUNT_DL_CA_EVENT_Q(dlCAEventQueue_gp[internalCellIndex]);
		if (dlCAEventQueueCount)
		{
			LTE_MAC_UT_LOG(LOG_INFO,DL_STRATEGY,
					"[DL-Strategy][%s]: Processing CAEventQueue"
					"CAEventQueue Count:%d\n",__func__,
					dlCAEventQueueCount );
			processdlCAEventQueue(internalCellIndex,dlCAEventQueueCount);
		}
	}
	/** SPR 13599 Changes End **/ 
	/* + SPR 11101 Changes */
    /* SPR 22275 Fix + */
    initDlQosRbRestriction(internalCellIndex);
    /* SPR 22275 Fix - */

}

/*****************************************************************************
 * Function Name  : dlQosStrategyProcessDLReTxFailQueues 
 * Inputs         : internalCellIndex
 *                  currentGlobalTTITickCount
 *                  numDlUEsToBeScheduled_p
 *                  numberOfAvailableRBs_p
 *                  dlDelay
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void dlQosStrategyProcessDLReTxFailQueues( InternalCellIndex internalCellIndex,
		UInt8   *numDlUEsToBeScheduled_p,
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
		/* SPR 15909 fix start */
        /* +- SPR 17777 */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
		,UInt32 dlDelay
#endif
        )
/* +- SPR 17777 */
{
	SInt8 dlRetxFailQueueCount  = 0;
	InternalCellIndex cellIndex = 0;
	/*
	 *   Process the retransmission UE List and for each UE, 
	 *   remove the the lc.s from the priority tree.
	 */
	if(!IS_CA_ENABLED())
	{
		dlRetxFailQueueCount = 
			sQueueCount( dlRetransmissionFailureQueue_gp[internalCellIndex]);

		/** SPR 13599 Changes Start **/ 
		if (dlRetxFailQueueCount && numDlUEsToBeScheduled_p[internalCellIndex])
			/** SPR 13599 Changes End **/ 
		{
			LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_FAILURE_QUEUE
				(dlRetxFailQueueCount, internalCellIndex);/* COVERITY 19394 */

			processDLRetransmissionFailureQueue(
					(UInt32 *)numberOfAvailableRBs[internalCellIndex],
					currentGlobalTTITickCount, 
#ifdef FDD_CONFIG        
					dlDelay,
#endif
					(UInt8 *)(&numDlUEsToBeScheduled_p[internalCellIndex]),
					internalCellIndex);
		}
	}
	else    
	{
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++ )
		{
			if ( (numDlUEsToBeScheduled_p[cellIndex]) &&
					(CELL_PHY_RUNNING == cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->cellState))
			{
				dlRetxFailQueueCount = 
					sQueueCount( dlRetransmissionFailureQueue_gp[cellIndex]);

				if (dlRetxFailQueueCount)
				{
					LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_FAILURE_QUEUE
						(dlRetxFailQueueCount, cellIndex);/* COVERITY 19394 */

					processDLRetransmissionFailureQueue(
							(UInt32 *)numberOfAvailableRBs[cellIndex],
							currentGlobalTTITickCount, 
#ifdef FDD_CONFIG        
							dlDelay,
#endif
							(UInt8 *)(&numDlUEsToBeScheduled_p[cellIndex]),
							cellIndex);
				}
			}
		}
	}
}
/*****************************************************************************
 * Function Name  : dlQosStrategyProcessDLReTxQueues 
 * Inputs         : internalCellIndex
 *                  currentGlobalTTITickCount
 *                  numDlUEsToBeScheduled_p
 *                  numberOfAvailableRBs_p
 *                  dlDelay
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void dlQosStrategyProcessDLReTxQueues( InternalCellIndex internalCellIndex,
		UInt8   *numDlUEsToBeScheduled_p,
		SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION],
		/* SPR 15909 fix start */
        /* +- SPR 17777 */
        tickType_t currentGlobalTTITickCount
#ifdef FDD_CONFIG
        ,UInt32 dlDelay
#endif
        )
/* +- SPR 17777 */
{
	SInt8 dlRetxQueueCount      = 0;
	InternalCellIndex cellIndex = 0;
	/*
	 *   Process the retransmission UE List and for each UE, 
	 *   remove the the lc.s from the priority tree.
	 */
	if (DL_QOS_BASED_SCHEDULING != (cellSpecificParams_g.cellConfigAndInitParams_p[\
				internalCellIndex]->initParams_p->\
				dlSchdConfig.dlSchedulerStrategy))  
	{
		dlRetxQueueCount =  sQueueCount(dlRetransmissionQueue_gp[internalCellIndex]);
		/** SPR 13599 Changes Start **/ 
		if ( dlRetxQueueCount && numDlUEsToBeScheduled_p[internalCellIndex])
			/** SPR 13599 Changes End **/ 
		{
			LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_QUEUE(dlRetxQueueCount, internalCellIndex);

			processDLRetransmissionQueue(
					(UInt32 *)numberOfAvailableRBs[internalCellIndex],
					currentGlobalTTITickCount, 
#ifdef FDD_CONFIG        
					dlDelay,
#endif
					&numDlUEsToBeScheduled_p[internalCellIndex],
					internalCellIndex);
		}
	}
	else
	{
		if(!IS_CA_ENABLED())
		{
			dlRetxQueueCount =  sQueueCount(dlRetransmissionQueue_gp[internalCellIndex]);

			if (dlRetxQueueCount)
			{
				LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_QUEUE(dlRetxQueueCount, internalCellIndex);

				processDLRetransmissionQueue(
						numberOfAvailableRBs,
						currentGlobalTTITickCount, 
#ifdef FDD_CONFIG        
						dlDelay,
#endif
						numDlUEsToBeScheduled_p,
						internalCellIndex);
			}
		}
		else
		{
			for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++ )
			{
				if ( (numDlUEsToBeScheduled_p[cellIndex]) &&
						(CELL_PHY_RUNNING == cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->cellState))
				{
					dlRetxQueueCount =  sQueueCount(dlRetransmissionQueue_gp[cellIndex]);

					if (dlRetxQueueCount)
					{
						LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_QUEUE(dlRetxQueueCount, cellIndex);

						processDLRetransmissionQueue(
								numberOfAvailableRBs,
								currentGlobalTTITickCount, 
#ifdef FDD_CONFIG        
								dlDelay,
#endif
								numDlUEsToBeScheduled_p,
								cellIndex);
					}
				}
			}
		}
	}
}
/*****************************************************************************
 *  *  * Function Name  : dlQosGetFirstNode
 *  *  * Inputs         : dlQosStrategyLCNodeForCellIndex_p
 *  *  *                : internalCellIndex
 *  *                     *workingTreeIndex
 *  *  * Outputs        :
 *  *  * Returns        : None
 *  *  * Description    : This func will update Strategy Nodes in all/particular
 *  *  *                  serving cells based on whether CA is enabled or not.
 *  *  *****************************************************************************/

DLQosStrategyLCNode* dlQosGetFirstNode(DLQosStrategyLCNode *dlQosStrategyLCNodeForCellIndex_p[],
		InternalCellIndex internalCellIndex,
		UInt8 *workingTreeIndex)
{
	/* add multiTree traversal changes */
	DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
	/* if reStartTraversing is True then get the node from strating of the Priority tree */
	*workingTreeIndex = INVALID_8_VAL;
	/* Get the node from priority tree */
	dlQosStrategyLCNodeForCellIndex_p[internalCellIndex] = (DLQosStrategyLCNode *)dlQosGetFirstLcNode
		(dlPriorityLcTree_gp[internalCellIndex][NON_VICTIM_UE]);
	if(dlQosStrategyLCNodeForCellIndex_p[internalCellIndex] != NULL)
	{
		/* CA Enable case */
		if(IS_CA_ENABLED())
		{
			/* Get the node from the  priority tree */
			dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)] = (DLQosStrategyLCNode *)dlQosGetFirstLcNode
				(dlPriorityLcTree_gp[!(internalCellIndex)][NON_VICTIM_UE]);
			if(dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)] != NULL)
			{
				/* If LC wieght of one tree is greater than the node from other tree */
				if(dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)]->totLCWeight >
						dlQosStrategyLCNodeForCellIndex_p[internalCellIndex]->totLCWeight)
				{
					/*get the node from tree*/
					dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)];
					*workingTreeIndex = !(internalCellIndex) ;
				}
				else
				{
					/*get the node from tree */
					dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[internalCellIndex];
					*workingTreeIndex = (internalCellIndex);
				}
			}
			else
			{
				/*get the node from tree when other tree is empty*/
				dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[internalCellIndex];
				*workingTreeIndex = (internalCellIndex);
			}
		}
		/* CA disable case*/
		else
		{
			/*get the node from tree */
			dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[internalCellIndex];
		}
	}
	else
	{
		if(IS_CA_ENABLED())
		{
			/*get the node from priority tree*/
			dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)] = (DLQosStrategyLCNode *)dlQosGetFirstLcNode
				(dlPriorityLcTree_gp[!(internalCellIndex)][NON_VICTIM_UE]);
			if(dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)] != NULL)
			{
				dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)];
				*workingTreeIndex = !(internalCellIndex);
			}
			else
			{
				*workingTreeIndex = INVALID_8_VAL;
			}
		}
	}
	return dlQosStrategyLCNode_p;
}

/* EICIC +*/
/*****************************************************************************
 *  *  * Function Name  : dlQosGetFirstVictimNode
 *  *  * Inputs         : dlQosStrategyLCNodeForCellIndex_p
 *  *  *                : internalCellIndex
 *  *                     *workingTreeIndex
 *  *  * Outputs        :
 *  *  * Returns        : None
 *  *  * Description    : This func will update Strategy Nodes in all/particular
 *  *  *                  serving cells based on whether CA is enabled or not.
 *  *  *****************************************************************************/

DLQosStrategyLCNode* dlQosGetFirstVictimNode(DLQosStrategyLCNode *dlQosStrategyLCNodeForCellIndex_p[],
		InternalCellIndex internalCellIndex,
		UInt8 *workingTreeIndex)
{
	/* add multiTree traversal changes */
	DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
	/* if reStartTraversing is True then get the node from strating of the Priority tree */
	*workingTreeIndex = INVALID_8_VAL;
	/* Get the node from priority tree */
	dlQosStrategyLCNodeForCellIndex_p[internalCellIndex] = (DLQosStrategyLCNode *)dlQosGetFirstLcNode
		(dlPriorityLcTree_gp[internalCellIndex][VICTIM_UE]);

	if(dlQosStrategyLCNodeForCellIndex_p[internalCellIndex] != NULL)
	{
		/* CA Enable case */
		if(IS_CA_ENABLED())
		{
			/* Get the node from the  priority tree */
			dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)] = (DLQosStrategyLCNode *)dlQosGetFirstLcNode
				(dlPriorityLcTree_gp[!(internalCellIndex)][VICTIM_UE]);
			if(dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)] != NULL)
			{
				/* If LC wieght of one tree is greater than the node from other tree */
				if(dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)]->totLCWeight >
						dlQosStrategyLCNodeForCellIndex_p[internalCellIndex]->totLCWeight)
				{
					/*get the node from tree*/
					dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)];
					*workingTreeIndex = !(internalCellIndex) ;
				}
				else
				{
					/*get the node from tree */
					dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[internalCellIndex];
					*workingTreeIndex = (internalCellIndex);
				}
			}
			else
			{
				/*get the node from tree when other tree is empty*/
				dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[internalCellIndex];
				*workingTreeIndex = (internalCellIndex);
			}
		}
		/* CA disable case*/
		else
		{
			/*get the node from tree */
			dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[internalCellIndex];
		}
	}
	else
	{
		if(IS_CA_ENABLED())
		{
			/*get the node from priority tree*/

			dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)] = (DLQosStrategyLCNode *)dlQosGetFirstLcNode
				(dlPriorityLcTree_gp[!(internalCellIndex)][VICTIM_UE]);
			if(dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)] != NULL)
			{
				dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[!(internalCellIndex)];
				*workingTreeIndex = !(internalCellIndex);
			}
			else
			{
				*workingTreeIndex = INVALID_8_VAL;
			}
		}
	}
	return dlQosStrategyLCNode_p;
}
/* EICIC -*/


/*****************************************************************************
 *  *  *  * Function Name  : dlQosGetNextNode
 *  *  *  * Inputs         : dlQosStrategyLCNodeForCellIndex_p
 *  *  *  *                : *workingTreeIndex
 *  *  *  * Outputs        :
 *  *  *  * Returns        : None
 *  *  *  * Description    : This func will update Strategy Nodes in all/particular
 *  *  *  *                  serving cells based on whether CA is enabled or not.
 *  *  *  *****************************************************************************/
DLQosStrategyLCNode* dlQosGetNextNode(DLQosStrategyLCNode *dlQosStrategyLCNodeForCellIndex_p[],
		UInt8 *workingTreeIndex)
{
	DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
	if(*workingTreeIndex < MAX_NUM_CELL)
	{
		/*If both tree have empty node then workingTreeIndex to be Invalid*/
		if ((PNULL == dlQosStrategyLCNodeForCellIndex_p[*workingTreeIndex]) && (PNULL == dlQosStrategyLCNodeForCellIndex_p[!(*workingTreeIndex)]))
		{
			*workingTreeIndex = INVALID_8_VAL;
			return PNULL;
		}
		/*  pick the node from priority tree */
		dlQosStrategyLCNodeForCellIndex_p[*workingTreeIndex] = (DLQosStrategyLCNode *)dlQosGetNextLcNode(
				&(dlQosStrategyLCNodeForCellIndex_p[*workingTreeIndex])->lcTreeAnchor);
		/*if node present in both tree*/
		if((PNULL != dlQosStrategyLCNodeForCellIndex_p[*workingTreeIndex]) && (PNULL != dlQosStrategyLCNodeForCellIndex_p[!(*workingTreeIndex)]))
		{
			/*compare the priority node w.r.to LcWeight*/
			if( dlQosStrategyLCNodeForCellIndex_p[*workingTreeIndex]->totLCWeight > dlQosStrategyLCNodeForCellIndex_p[!(*workingTreeIndex)]->totLCWeight)
			{
				dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[*workingTreeIndex];
				/*working tree index remains same */
			}
			else
			{
				dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[!(*workingTreeIndex)];
				/*working tree index changes*/
				*workingTreeIndex = !(*workingTreeIndex);
			}
		}
		/*if one tree have empty node and other have*/
		else if ((PNULL != dlQosStrategyLCNodeForCellIndex_p[*workingTreeIndex]) && (PNULL == dlQosStrategyLCNodeForCellIndex_p[!(*workingTreeIndex)]))
		{
			dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[*workingTreeIndex];
			/*working tree index remains same */
		}
		/*if one tree have empty node and other have*/
		else if ((PNULL == dlQosStrategyLCNodeForCellIndex_p[*workingTreeIndex]) && (PNULL != dlQosStrategyLCNodeForCellIndex_p[!(*workingTreeIndex)]))
		{
			dlQosStrategyLCNode_p = dlQosStrategyLCNodeForCellIndex_p[!(*workingTreeIndex)];
			/*working tree index changes*/
			*workingTreeIndex = !(*workingTreeIndex);
		}
		/* if both tree have empty node*/
		else if ((PNULL == dlQosStrategyLCNodeForCellIndex_p[*workingTreeIndex]) && (PNULL == dlQosStrategyLCNodeForCellIndex_p[!(*workingTreeIndex)]))
		{
			*workingTreeIndex = INVALID_8_VAL;
		}
	}
	return dlQosStrategyLCNode_p;
}

/*****************************************************************************
 * Function Name  : putEntryInSCellSchedulingQueue 
 * Inputs         : ueDLContext_p
 *                  harqProcess_p
 *                  servingCellIndex
 * Outputs        : 
 * Returns        : 
 * Description    : 
 *****************************************************************************/
void putEntryInSCellSchedulingQueue( DLUEContext* ueDLContext_p, 
		DLHARQProcess* harqProcess_p,
		ServingCellIndex servingCellIndex) 
{
	SCellSchedulingQueueNode *sCellScheduledNode_p = PNULL;
	UInt8 serveCellArrIndex = INVALID_8_VAL; 

	serveCellArrIndex = getServeCellIndexFromInternalCellId(ueDLContext_p, servingCellIndex);

	/*klockwork warning fix*/
	if(serveCellArrIndex <= MAX_NUM_SCELL)
	{
		if( (TRUE == harqProcess_p->isTBOneValid) && 
				(TRUE == harqProcess_p->isTBTwoValid))
		{
			/** SPR 12457 Fix : Start**/
			ueDLContext_p->dlUeScellContext_p[serveCellArrIndex]->pucchReqInfo.schedulingStatus = TWO_TB ;
		}
		else
		{
			ueDLContext_p->dlUeScellContext_p[serveCellArrIndex]->pucchReqInfo.schedulingStatus = SINGLE_TB ;
			/** SPR 12457 Fix : End **/
		}

		GET_MEM_FROM_POOL(SCellSchedulingQueueNode, sCellScheduledNode_p,
				sizeof(SCellSchedulingQueueNode), PNULL);

		if(PNULL != sCellScheduledNode_p)
		{
			sCellScheduledNode_p->ueIndex = ueDLContext_p->ueIndex; 

			sCellScheduledNode_p->serveCellIndex = serveCellArrIndex; 

			if(!ENQUEUE_SCELL_SCHEDULING_NODE( sCellScheduledNode_p, 
						ueDLContext_p->internalCellIndex))
			{
				FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
			}
		}
	}
}

/*****************************************************************************
 * Function Name  : dlQosGetPCellMcs 
 * Inputs         : ueDLContext_p 
 * Outputs        : 
 * Returns        : mcsIndex
 * Description    : This func will provide the mcsindex of UE in 
 *                  Primary cell
 *****************************************************************************/
UInt8 dlQosGetPCellMcs( DLUEContext* ueDLContext_p)
{
	UInt8 mcsInCellIndex = 0;
	UInt8 transmissionMode = 0;

	transmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;

	if( (TX_MODE_3 == transmissionMode) ||
			(TX_MODE_4 == transmissionMode) ||
			(TX_MODE_8 == transmissionMode))
	{
		mcsInCellIndex = MIN_BETWEEN_TWO(
				(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne),
				(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo));
	}
	else
	{
		mcsInCellIndex = ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
	}
	return mcsInCellIndex;
}

/*****************************************************************************
 * Function Name  : dlQosGetSCellMcs 
 * Inputs         : ueDLContext_p 
 *                : servingCellIndex 
 * Outputs        : 
 * Returns        : mcsIndex
 * Description    : This func will provide the mcsindex of UE in 
 *                  secondary Cell
 *****************************************************************************/
UInt8 dlQosGetSCellMcs( DLUEContext* ueDLContext_p,
		InternalCellIndex sCellIndex)
{
	UInt8 mcsInCellIndex = 0;
	UInt8 transmissionMode = 0;
	ServingCellIndex servCellIndex = INVALID_CELL_INDEX;

	servCellIndex = getServeCellIndexFromInternalCellId( 
			ueDLContext_p,
			sCellIndex);
	/*klockwork warning fix*/
	if(servCellIndex <= MAX_NUM_SCELL)
	{
		transmissionMode = 
			ueDLContext_p->dlUeScellContext_p[servCellIndex]->ScelldlMIMOInfo.transmissionMode;

		if( (TX_MODE_3 == transmissionMode) ||
				(TX_MODE_4 == transmissionMode) ||
				(TX_MODE_8 == transmissionMode))
		{
			mcsInCellIndex = MIN_BETWEEN_TWO(
					(ueDLContext_p->dlUeScellContext_p[servCellIndex]->\
					 ScelldlCQIInfo.mcsIndexCodeWordOne),
					(ueDLContext_p->dlUeScellContext_p[servCellIndex]->\
					 ScelldlCQIInfo.mcsIndexCodeWordTwo));
		}
		else
		{
			mcsInCellIndex = 
				ueDLContext_p->dlUeScellContext_p[servCellIndex]->ScelldlCQIInfo.mcsIndexCodeWordOne;
		}
	}
	return mcsInCellIndex;
}
/* Cyclomatic Complexity changes - ends here */

/* Cyclomatic_complexity_changes_start */
/*****************************************************************************
 * Function Name  : getMsgTypeTaDrxFlagNonSpatial
 * Inputs         : scheduledLcCount - number of LC.s scheduled in current tick
 *                  isTaDrxScheduled - whether TA is scheduled or Not
 *                  dlStrategyTxNode_p, 
 *                  harqProcess_p context,
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function will populate msgType and taDrxFlag .
 ******************************************************************************/
STATIC void getMsgTypeTaDrxFlagNonSpatial(UInt8 scheduledLcCount,
		MsgType            isTaDrxScheduled,
		DLStrategyTxNode   *dlStrategyTxNode_p,
        /* +- SPR 17777 */
		DLHARQProcess      *harqProcess_p
#ifdef SSI_DEBUG_STATS
        ,InternalCellIndex internalCellIndex
#endif
        )
        /* +- SPR 17777 */
{
	if(scheduledLcCount)
	{
		/* update the message type */
		switch(isTaDrxScheduled)
		{
			case TA_MSG:
				{
					dlStrategyTxNode_p->msgType =
						TA_WITH_DATA_NON_SPATIAL_MUL;
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						TA_MSG;
					/* +- SPR 22352 fix */

					/* CA Stats Changes Start */
					LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_TA_WITH_DATA(
							internalCellIndex);
					/* CA Stats Changes End */

					break;
				}
			case DRX_MSG:
				dlStrategyTxNode_p->msgType =
					DRX_WITH_DATA_NON_SPATIAL_MUL ;
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					DRX_MSG;
				break;
			case TA_DRX_MSG:
				dlStrategyTxNode_p->msgType =
					TA_DRX_WITH_DATA_NON_SPATIAL_MUL;
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					TA_DRX_MSG;
				break;
				/* CLPC_CHANGE */
			case TPC_MSG:
				dlStrategyTxNode_p->msgType =
					TPC_WITH_DATA;
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					INVALID_TA_DRX_MSG;
				break;
			case SCELL_ACT_DEACT_MSG:
				{
					dlStrategyTxNode_p->msgType =
						SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL;
					harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
						SCELL_ACT_DEACT_MSG;

					break;
				}
				/* + SPR 22352 fix */
			case TA_SCELL_ACT_DEACT_MSG:
				{
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						TA_MSG;
					if( (harqProcess_p->scellActBitmask) ||
							(harqProcess_p->scellDeactBitmask))
					{
						dlStrategyTxNode_p->msgType =
							TA_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL;
						harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
							SCELL_ACT_DEACT_MSG;
					}
					break;
				}
			case DRX_SCELL_ACT_DEACT_MSG:
				{
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						DRX_MSG;
					if( (harqProcess_p->scellActBitmask) ||
							(harqProcess_p->scellDeactBitmask))
					{
						dlStrategyTxNode_p->msgType =
							DRX_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL;
						harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
							SCELL_ACT_DEACT_MSG;
					}
					break;
				}
			case TA_DRX_SCELL_ACT_DEACT_MSG:
				{
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						TA_DRX_MSG;
					if( (harqProcess_p->scellActBitmask) ||
							(harqProcess_p->scellDeactBitmask))
					{
						dlStrategyTxNode_p->msgType =
							TA_DRX_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL;
						harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
							SCELL_ACT_DEACT_MSG;
					}
					break;
				}

				/* - SPR 22352 fix */

			default:
				dlStrategyTxNode_p->msgType =
					NON_SPATIAL_MUL ;
				break;
		}
	}
	else
	{
		/* update the message type */
		switch(isTaDrxScheduled)
		{
			case TA_MSG:
				{
					dlStrategyTxNode_p->msgType =
						TA_NON_SPATIAL_MUL;
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						TA_MSG;

					/* +- SPR 22352 fix */

					break;
				}
			case DRX_MSG:
				dlStrategyTxNode_p->msgType =
					DRX_NON_SPATIAL_MUL ;
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					DRX_MSG;
				break;
			case TA_DRX_MSG:
				dlStrategyTxNode_p->msgType =
					TA_DRX_NON_SPATIAL_MUL;
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					TA_DRX_MSG;
				break;
				/* CLPC_CHANGE */
			case TPC_MSG:
				dlStrategyTxNode_p->msgType =
					TPC_WITHOUT_DATA;
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					INVALID_TA_DRX_MSG;
				break;
			case SCELL_ACT_DEACT_MSG:
				{
					dlStrategyTxNode_p->msgType =
						SCELL_ACT_DEACT_NON_SPATIAL_MUL;
					harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
						SCELL_ACT_DEACT_MSG;
					break;
				}
				/* + SPR 22352 fix */
			case TA_SCELL_ACT_DEACT_MSG:
				{
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						TA_MSG;
					if( (harqProcess_p->scellActBitmask) ||
							(harqProcess_p->scellDeactBitmask))
					{
						dlStrategyTxNode_p->msgType =
							TA_SCELL_ACT_DEACT_NON_SPATIAL_MUL;
						harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
							SCELL_ACT_DEACT_MSG;
					}
					break;
				}
			case DRX_SCELL_ACT_DEACT_MSG:
				{
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						DRX_MSG;
					if( (harqProcess_p->scellActBitmask) ||
							(harqProcess_p->scellDeactBitmask))
					{
						dlStrategyTxNode_p->msgType =
							DRX_SCELL_ACT_DEACT_NON_SPATIAL_MUL;
						harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
							SCELL_ACT_DEACT_MSG;
					}
					break;
				}
			case TA_DRX_SCELL_ACT_DEACT_MSG:
				{
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						TA_DRX_MSG;
					if( (harqProcess_p->scellActBitmask) ||
							(harqProcess_p->scellDeactBitmask))
					{
						dlStrategyTxNode_p->msgType =
							TA_DRX_SCELL_ACT_DEACT_NON_SPATIAL_MUL;
						harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
							SCELL_ACT_DEACT_MSG;
					}
					break;
				}
				/* - SPR 22352 fix */
			default:
				dlStrategyTxNode_p->msgType =
					NON_SPATIAL_MUL ;
				break;
		}
	}

	return;
}

/*****************************************************************************
 * Function Name  : getMsgTypeTaDrxFlagSpatial
 * Inputs         : scheduledLcCount - number of LC.s scheduled in current tick
 *                  isTaDrxScheduled - whether TA is scheduled or Not
 *                  dlStrategyTxNode_p
 *                  harqProcess_p context
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function will populate msgType and taDrxFlag .
 ******************************************************************************/
STATIC void getMsgTypeTaDrxFlagSpatial(UInt8 scheduledLcCount,
		MsgType            isTaDrxScheduled,
		DLStrategyTxNode   *dlStrategyTxNode_p,
        /* +- SPR 17777 */
		DLHARQProcess      *harqProcess_p)
        /* +- SPR 17777 */
{
	if(scheduledLcCount)
	{
		/* update the message type */
		switch(isTaDrxScheduled)
		{
			case TA_MSG:
				{
					dlStrategyTxNode_p->msgType =
						TA_WITH_DATA_SPATIAL_MUL;
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						TA_MSG;
					/* +- SPR 22352 fix */

					break;
				}
			case DRX_MSG:
				dlStrategyTxNode_p->msgType =
					DRX_WITH_DATA_SPATIAL_MUL ;
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					DRX_MSG;
				break;
			case TA_DRX_MSG:
				dlStrategyTxNode_p->msgType =
					TA_DRX_WITH_DATA_SPATIAL_MUL;
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					TA_DRX_MSG;
				break;
				/* CLPC_CHANGE */
			case TPC_MSG:
				dlStrategyTxNode_p->msgType =
					TPC_WITH_DATA;
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					INVALID_TA_DRX_MSG;
				break;
			case SCELL_ACT_DEACT_MSG:
				{
					dlStrategyTxNode_p->msgType =
						SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL;
					harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
						SCELL_ACT_DEACT_MSG;
					break;
				}
				/* + SPR 22352 fix */
			case TA_SCELL_ACT_DEACT_MSG:
				{
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						TA_MSG;
					if( (harqProcess_p->scellActBitmask) ||
							(harqProcess_p->scellDeactBitmask))
					{
						dlStrategyTxNode_p->msgType =
							TA_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL;
						harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
							SCELL_ACT_DEACT_MSG;
					}
					break;
				}
			case DRX_SCELL_ACT_DEACT_MSG:
				{
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						DRX_MSG;
					if( (harqProcess_p->scellActBitmask) ||
							(harqProcess_p->scellDeactBitmask))
					{
						dlStrategyTxNode_p->msgType =
							DRX_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL;
						harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
							SCELL_ACT_DEACT_MSG;
					}
					break;
				}
			case TA_DRX_SCELL_ACT_DEACT_MSG:
				{
					harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
						TA_DRX_MSG;
					if( (harqProcess_p->scellActBitmask) ||
							(harqProcess_p->scellDeactBitmask))
					{
						dlStrategyTxNode_p->msgType =
							TA_DRX_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL;
						harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
							SCELL_ACT_DEACT_MSG;
					}
					break;
				}
				/* - SPR 22352 fix */

			default:
				dlStrategyTxNode_p->msgType = SPATIAL_MUL ;
				break;
		}
	}
	else
	{
		/* update the message type */
		switch(isTaDrxScheduled)
		{
            /* SES-515 fix +- */
			case TA_MSG:
			{
				/* + SPR 22352 fix */ 
				dlStrategyTxNode_p->msgType =TA_SPATIAL_MUL;
				/* - SPR 22352 fix */

				/* - CQI_5.0 */
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					TA_MSG;
				/*SPR 16610 fix start*/
				/* +- SPR 22352 fix */
				break;
			}
			case DRX_MSG:
			dlStrategyTxNode_p->msgType =
				DRX_SPATIAL_MUL ;
			harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
				DRX_MSG;
			break;
			case TA_DRX_MSG:
			dlStrategyTxNode_p->msgType =
				TA_DRX_SPATIAL_MUL;
			harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
				TA_DRX_MSG;
			break;
			/* CLPC_CHANGE */
			case TPC_MSG:
			dlStrategyTxNode_p->msgType =
				TPC_WITHOUT_DATA;
			harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
				INVALID_TA_DRX_MSG;
			break;
			case SCELL_ACT_DEACT_MSG:
			{
				/* + SPR 12118 Changes */
				dlStrategyTxNode_p->msgType =
					SCELL_ACT_DEACT_SPATIAL_MUL; 
				harqProcess_p->isTBTwoValid = FALSE; 
				/* - SPR 12118 Changes */
				harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
					SCELL_ACT_DEACT_MSG;
				break;
			}
			/* + SPR 22352 fix */
			case TA_SCELL_ACT_DEACT_MSG:
			{
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					TA_MSG;
				if( (harqProcess_p->scellActBitmask) ||
						(harqProcess_p->scellDeactBitmask))
				{
					dlStrategyTxNode_p->msgType =
						TA_SCELL_ACT_DEACT_SPATIAL_MUL;
					harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
						SCELL_ACT_DEACT_MSG;
				}

				break;
			}
			case DRX_SCELL_ACT_DEACT_MSG:
			{
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					DRX_MSG;
				if( (harqProcess_p->scellActBitmask) ||
						(harqProcess_p->scellDeactBitmask))
				{
					dlStrategyTxNode_p->msgType =
						DRX_SCELL_ACT_DEACT_SPATIAL_MUL;
					harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
						SCELL_ACT_DEACT_MSG;
				}
				break;
			}
			case TA_DRX_SCELL_ACT_DEACT_MSG:
			{
				harqProcess_p->dlHARQTBOneInfo.taDrxFlag =
					TA_DRX_MSG;
				if( (harqProcess_p->scellActBitmask) ||
						(harqProcess_p->scellDeactBitmask))
				{
					dlStrategyTxNode_p->msgType =
						TA_DRX_SCELL_ACT_DEACT_SPATIAL_MUL;
					harqProcess_p->dlHARQTBOneInfo.macScellCEFlag =
						SCELL_ACT_DEACT_MSG;
				}
				break;
			}
			/* - SPR 22352 fix */
			default:
			dlStrategyTxNode_p->msgType = SPATIAL_MUL ;
			break;
		}
        /* SES-515 fix + */
        /* + SPR 22352 fix */ 
        /* In this case when MAC CE needs to be scheduled without data, only one TB 
         * should be sent. So, marking second TB as invalid. 
         * */
        if(NEW_TX == harqProcess_p->dlHARQTBTwoInfo.txIndicator)
        {
            harqProcess_p->isTBTwoValid = FALSE;
            harqProcess_p->dlHARQTBTwoInfo.tbSize = 0;
        }
        /* - SPR 22352 fix */ 
        /* SES-515 fix - */
	}

	return;
}
/*****************************************************************************
 * Function Name  : processPendingTAQueue
 * Inputs         : taNode_p - TimeAdvanceQueueNode,
 *                  flagForTADRXPendingFresh - FlagForPendingTADRX,
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the pending TAQueue

 * *****************************************************************************/

STATIC void  processPendingTAQueue( TimeAdvanceQueueNode *taNode_p, FlagForPendingTADRX flagForTADRXPendingFresh,
		InternalCellIndex internalCellIndex, DLUEContext *ueDLContext_p)
{
	LP_TimeAdvanceQueueNode tempTaNode_p  = PNULL;
	if(flagForTADRXPendingFresh == PENDING_TA_FLAG)
	{
		FREE_MEM_NODE_TA_Q(taNode_p);
		/*SPR 9664 Fix Start*/
		/*if TA MAC CE could not be sent, Set TA average value to default value
		  and clear all accumulated history */
		ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
		/*SPR 9664 Fix End*/
	}
	else
	{
		UInt32 retVal = 0;
		tempTaNode_p = taNode_p;
		retVal = ENQUEUE_TA_Q(pendingTAQueue_gp[internalCellIndex],TimeAdvanceQueueNode, tempTaNode_p);
		if(!retVal)
		{
			lteWarning("Error in Pushing in Pending"
					" TAQueue in funtion %s\n",
					__func__);
			FREE_MEM_NODE_TA_Q(taNode_p);
			/*SPR 9664 Fix Start*/
			/*if TA MAC CE could not be sent, Set TA average value to default value
			  and clear all accumulated history */
			ueDLContext_p->lastCalculatedTAAverage = MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;
			/*SPR 9664 Fix End*/
		}
	}
}
/* Cyclomatic_complexity_changes_end */
/*Complexity Start*/
void updateTopSchedulableVictimUEInfoList(InternalCellIndex internalCellIndex,UInt8 reStartTraversing,UInt8* localArr)
{	
	DLQosStrategyLCNode *dlQosStrategyLCNode_p;
	UInt8 cellIdx = 0;
	UInt16 maxUEsToBeScheduledDL = 0;
	DLUEContextInfo *dlUEContextInfo_p = PNULL;
	dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode(internalCellIndex,reStartTraversing);
	if(dlQosStrategyLCNode_p == NULL)
	{
		return;
	}
	/*Code Review Comments Changes Start*/
	for(cellIdx = 0; cellIdx <= MAX_NUM_SCELL; cellIdx++)
	{
		topSchedulableVictimUEs_g[cellIdx].arrCount = 0;
	}
	/*Code Review Comments Changes End*/
	cellIdx = getCellIndexOfPriorityLcNode(dlQosStrategyLCNode_p);
	if(cellIdx == INVALID_CELL_INDEX)
	{
		return;
	}
	maxUEsToBeScheduledDL = cellSpecificParams_g.cellConfigAndInitParams_p\
				[cellIdx]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL; 

	reStartTraversing = FALSE;
	/*Cov_fix_28April_62168_start*/
	while (cellIdx <= MAX_NUM_SCELL && topSchedulableVictimUEs_g[cellIdx].arrCount < (maxUEsToBeScheduledDL * 2))
	{
		/*Cov_fix_28April_62168_stop*/    
		if (localArr[dlQosStrategyLCNode_p->ueIndex] != 1)
		{    
			dlUEContextInfo_p = &dlUECtxInfoArr_g[dlQosStrategyLCNode_p->ueIndex];
			if(dlUEContextInfo_p == PNULL || dlUEContextInfo_p->pendingDeleteFlag)
			{
				return;
			}
			topSchedulableVictimUEs_g[cellIdx].topSchedulableUEInfo[topSchedulableVictimUEs_g[cellIdx].arrCount].ueIndex = \
																       dlQosStrategyLCNode_p->ueIndex;
                                                                       /* SPR 16116 fix start */
			topSchedulableVictimUEs_g[cellIdx].topSchedulableUEInfo[topSchedulableVictimUEs_g[cellIdx].arrCount].totLCWeight = \
																	   dlQosStrategyLCNode_p->totLCWeightWithLP;
                                                                       /* SPR 16116 fix end */
			topSchedulableVictimUEs_g[cellIdx].arrCount++;
			localArr[dlQosStrategyLCNode_p->ueIndex] = 1;
		}
		dlQosStrategyLCNode_p = dlQosGetPriorityLcTreeVictimNode(internalCellIndex,reStartTraversing);  
		if(dlQosStrategyLCNode_p == NULL)
		{
			return;
		}
		cellIdx = getCellIndexOfPriorityLcNode(dlQosStrategyLCNode_p);
		/*spr 12560 fix start*/
		if(cellIdx > MAX_NUM_SCELL)
		{
			return;
		}
		/*spr 12560 fix end*/
		maxUEsToBeScheduledDL = cellSpecificParams_g.cellConfigAndInitParams_p\
					[cellIdx]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL; 
	}
}
/*Complexity Ends*/
/* + SPR 22352 fix */

/*****************************************************************************
 * Function Name  : checkMsgTypeTaDrxScellActDeact
 * Inputs         : ueDLContext_p
 *                  currentMsgType
 *                  updatedMsgType
 *                  servingCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates the msgType while processing TA 
 * queue and scell MAC CE queues based on some conditions, if more than one
 * MAC CE needs to be scheduled together.
 * *****************************************************************************/

STATIC  void checkMsgTypeTaDrxScellActDeact(DLUEContext *ueDLContext_p,
        MsgType currentMsgType, MsgType *updatedMsgType,InternalCellIndex servingCellIndex)
{
    InternalCellIndex scellId = 0;
    *updatedMsgType = currentMsgType;
    switch(currentMsgType)
    {
        case TA_MSG:
        {
            if((ueDLContext_p->TaDrxScellBitmap & SCELL_ACT_BIT) ||
               (ueDLContext_p->TaDrxScellBitmap & SCELL_DEACT_BIT) )
            {
                /* check if MAC CE for SCELL Act/Deact can be scheduled in this tick or not*/
                /* The activation/deactivation commands should be sent on Pcell only */

                scellId = ueDLContext_p->TaDrxScellBitmap >> 4;
                if(((ueDLContext_p->dlUeScellContext_p[scellId]->scellState !=DL_CA_SCELL_CONFIGURED) && 
                            (ueDLContext_p->TaDrxScellBitmap & SCELL_ACT_BIT))||
                     ((DL_CA_SCELL_ACTIVE != ueDLContext_p->dlUeScellContext_p[scellId]->scellState) && 
                            (ueDLContext_p->TaDrxScellBitmap & SCELL_DEACT_BIT))
                        || (ueDLContext_p->internalCellIndex != servingCellIndex))
                {
                    /* SCELL MAC CE will not be scheduled with TA in this tick, it will be processed 
                     * during processing of dlCA MAC CE queue.*/
                    *updatedMsgType = TA_MSG;
                }
                else
                {

                    *updatedMsgType = TA_SCELL_ACT_DEACT_MSG;
                }
                /* check if DRX MAC CE also needs to be scheduled along with TA and SCELL MAC CE */
                if (((ueDLContext_p->drxFlag) && (!(ueDLContext_p->pendingDRXAckNack))) &&
                        ((ueDLContext_p->ueQueueLoad == 0)&& 
                         (ulUECtxInfoArr_g[ueDLContext_p->ueIndex].ulUEContext_p->bsrNetTotal == 0)))
                {
                    if(*updatedMsgType == TA_SCELL_ACT_DEACT_MSG)
                    {
                        *updatedMsgType = TA_DRX_SCELL_ACT_DEACT_MSG;
                    }
                    else
                    {
                        *updatedMsgType = TA_DRX_MSG;
                    }
                }
            }
             /* check if DRX MAC CE also needs to be scheduled along with TA  */
            else if (((ueDLContext_p->drxFlag) && (!(ueDLContext_p->pendingDRXAckNack))) &&
                    ((ueDLContext_p->ueQueueLoad == 0)&& 
                     (ulUECtxInfoArr_g[ueDLContext_p->ueIndex].ulUEContext_p->bsrNetTotal == 0)))
            {
                *updatedMsgType = TA_DRX_MSG;
            }
            else
            {
                *updatedMsgType = TA_MSG;
            }
        }
        break;
        case SCELL_ACT_DEACT_MSG:
        {
             /* check if DRX MAC CE also needs to be scheduled along with SCELL MAC CE  */
            if (((ueDLContext_p->drxFlag) && (!(ueDLContext_p->pendingDRXAckNack))) &&
                    ((ueDLContext_p->ueQueueLoad == 0)&& 
                     ((ulUECtxInfoArr_g[ueDLContext_p->ueIndex].ulUEContext_p->bsrNetTotal == 0))))
            {
                *updatedMsgType = DRX_SCELL_ACT_DEACT_MSG;
            }
            else
            {
                *updatedMsgType = SCELL_ACT_DEACT_MSG;
            }
        }
        break;
        default:
        break;

    }
}

/*****************************************************************************
 * Function Name  : updateRbAndTBSizeInDlStrategyNode
 * Inputs         : msgType
 *                  dlStrategyTxNode_p-strtaegy node to be updated
 *                  numberOfAvailableRBs_p- number of availableRB
 *                  userLocationType
 *                  ueIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates the max and min TBSize and RBs required
 *                  based on msgType.
 * *****************************************************************************/

STATIC  void updateRbAndTBSizeInDlStrategyNode(MsgType msgType,DLStrategyTxNode *dlStrategyTxNode_p,SInt32  *numberOfAvailableRBs_p,UInt8 userLocationType,UInt16 ueIndex)
{
    switch(msgType)
    {
        case TA_MSG:
        {
            dlStrategyTxNode_p->minTBSize =  LENGTH_OF_TA;
            dlStrategyTxNode_p->maxTBSize =  LENGTH_OF_TA;

            /* According to user location , subtract from those available RB */
            numberOfAvailableRBs_p[userLocationType] -= MIN_RB_FOR_TA_DRX_ONLY;
            RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(MIN_RB_FOR_TA_DRX_ONLY,numberOfAvailableRBs_p);
            dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB = 
                MIN_RB_FOR_TA_DRX_ONLY;
        }
        break;
        case DRX_MSG:
        {
            dlStrategyTxNode_p->minTBSize =  LENGTH_OF_DRX;
            dlStrategyTxNode_p->maxTBSize =  LENGTH_OF_DRX;
            /* ICIC Changes Starts */
            /* According to user location , subtract from those available RB */
            numberOfAvailableRBs_p[userLocationType] -= MIN_RB_FOR_TA_DRX_ONLY;
            RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(MIN_RB_FOR_TA_DRX_ONLY,numberOfAvailableRBs_p);
            /* ICIC Changes End */ 
            dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB = 
                MIN_RB_FOR_TA_DRX_ONLY;
        }
        break;
        case SCELL_ACT_DEACT_MSG:
        {
            dlStrategyTxNode_p->minTBSize +=  LENGTH_OF_SCELL_ACT_DEACT_CE;
            dlStrategyTxNode_p->maxTBSize +=  LENGTH_OF_SCELL_ACT_DEACT_CE;
            numberOfAvailableRBs_p[userLocationType] -= MIN_RB_FOR_SCELL_ACT_DEACT_ONLY;

            /* then update RBs for both cell centered and cell edge */
            RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(
                    MIN_RB_FOR_SCELL_ACT_DEACT_ONLY,
                    numberOfAvailableRBs_p);

            /* Add min Rbs as another MAC CE might be scheduled for UE */
            dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB += 
                MIN_RB_FOR_SCELL_ACT_DEACT_ONLY;

        }
        break;
        case TA_DRX_MSG:
        {
            dlStrategyTxNode_p->minTBSize =  LENGTH_OF_TA_DRX;/* 3 bytes */
            dlStrategyTxNode_p->maxTBSize =  LENGTH_OF_TA_DRX;
            numberOfAvailableRBs_p[userLocationType] -= MIN_RB_FOR_BOTH_TA_DRX_ONLY; 
            RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(MIN_RB_FOR_BOTH_TA_DRX_ONLY,numberOfAvailableRBs_p);
            dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB = 
                MIN_RB_FOR_BOTH_TA_DRX_ONLY;
        }
        break;
        case TA_SCELL_ACT_DEACT_MSG:
        {
            dlStrategyTxNode_p->minTBSize =  LENGTH_OF_TA_SCELL_ACT_DEACT_CE;/* 4 bytes */
            dlStrategyTxNode_p->maxTBSize =  LENGTH_OF_TA_SCELL_ACT_DEACT_CE;
            numberOfAvailableRBs_p[userLocationType] -= MIN_RB_FOR_BOTH_TA_DRX_ONLY; 
            RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(MIN_RB_FOR_BOTH_TA_DRX_ONLY,numberOfAvailableRBs_p);
            dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB = 
                MIN_RB_FOR_BOTH_TA_DRX_ONLY;
            /* set bitmask for act/deact in Harq Process */
            setBitmaskForScellActDeact(ueIndex,dlStrategyTxNode_p);

        }
        break;
        case DRX_SCELL_ACT_DEACT_MSG:
        {
            dlStrategyTxNode_p->minTBSize =  LENGTH_OF_TA_DRX;/* 3 bytes */
            dlStrategyTxNode_p->maxTBSize =  LENGTH_OF_TA_DRX;
            numberOfAvailableRBs_p[userLocationType] -= MIN_RB_FOR_BOTH_TA_DRX_ONLY; 
            RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(MIN_RB_FOR_BOTH_TA_DRX_ONLY,numberOfAvailableRBs_p);
            dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB = 
                MIN_RB_FOR_BOTH_TA_DRX_ONLY;
            /* set bitmask for act/deact in Harq Process */
            setBitmaskForScellActDeact(ueIndex,dlStrategyTxNode_p);

        }
        break;
        case TA_DRX_SCELL_ACT_DEACT_MSG:
        {
            dlStrategyTxNode_p->minTBSize =  LENGTH_OF_TA_DRX_SCELL_ACT_DEACT_CE;/* 5 bytes */
            dlStrategyTxNode_p->maxTBSize =  LENGTH_OF_TA_DRX_SCELL_ACT_DEACT_CE;
            numberOfAvailableRBs_p[userLocationType] -= MIN_RB_FOR_TA_DRX_SCELL_ACT_DEACT_ONLY; 
            RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(MIN_RB_FOR_TA_DRX_SCELL_ACT_DEACT_ONLY,numberOfAvailableRBs_p);
            dlStrategyTxNode_p->harqProcess_p->assignedResourcesNB = 
                MIN_RB_FOR_TA_DRX_SCELL_ACT_DEACT_ONLY;
            /* set bitmask for act/deact in Harq Process */
            setBitmaskForScellActDeact(ueIndex,dlStrategyTxNode_p);
        }
        break;

    default:
    break;
    }
}

/*****************************************************************************
 * Function Name  : setBitmaskForScellActDeact
 * Inputs         : ueIndex
 *                  dlStrategyTxNode_p-strtaegy node to be updated
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the bitmask for scell activation or deactivation
 *                  in the Harq context of strtaegy node.
 * *****************************************************************************/

STATIC  void setBitmaskForScellActDeact(UInt16 ueIndex,DLStrategyTxNode* dlStrategyTxNode_p)
{
    DLUEContext*            ueDLContext_p           = PNULL;
    DLUEContextInfo*        ueDLContextInfo_p       = PNULL;
    InternalCellIndex scellId = 0;

    ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

    if(ueDLContext_p)
    {
        /* In this bitmap, the first 4 bit from MSB represents teh scellID*/
        scellId = ueDLContext_p->TaDrxScellBitmap >> 4;
        /*  Spec 36.321 - a90
         *  Figure 6.1.3.8-1: Activation/Deactivation MAC control element  
         *
         * |--|--|--|--|--|--|--|--|
         * |C7|C6|C5|C4|C3|C2|C1|R |
         */

        if(ueDLContext_p->TaDrxScellBitmap & SCELL_ACT_BIT)
        {
            /* SET_BYTE_FIELD(data_var, value, position, numbits */
            SET_BYTE_FIELD( dlStrategyTxNode_p->harqProcess_p->scellActBitmask, 
                    1,scellId, 1);
        }
        else if(ueDLContext_p->TaDrxScellBitmap & SCELL_DEACT_BIT)
        {
            SET_BYTE_FIELD(dlStrategyTxNode_p->harqProcess_p->scellDeactBitmask, 
                    1,scellId, 1);
        }
    }
}

/* - SPR 22352 fix */
/******************************** End of File ********************************/
