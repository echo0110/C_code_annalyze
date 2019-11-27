/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLQosStrategy.h ,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 *
 ****************************************************************************/

#ifndef LTE_MAC_DL_QOS_STRATEGY_H
#define LTE_MAC_DL_QOS_STRATEGY_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacTypes.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/* Indicates the minimum GBR Bytes to be scheduled per TTI */
extern UInt32 dlAggregateGbrPerTTI_g[MAX_NUM_SCELL +1][MAX_VALID_UE_TYPE];/* EICIC 
+-*/
/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* ICIC changes starts */
#define RB_INFO_UPDATE_AFTER_RB_ALLOC_TO_USER(requiredRB,numberOfAvailableRBs)\
{\
	if (userLocationType == CC_USER )\
	{\
		dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated = CC_CE_REGION;\
        /* numberOfAvailableRBs of CE_REGION should not be greater than numberOfAvailableRBs CC_CE_REGION*/\
		if (numberOfAvailableRBs[CC_CE_REGION] <  numberOfAvailableRBs[CE_REGION])\
		{\
			numberOfAvailableRBs[CE_REGION] =\
				numberOfAvailableRBs[CC_CE_REGION];\
		}\
	}\
	else\
	{\
		numberOfAvailableRBs[CC_CE_REGION] -= requiredRB;\
		dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated = CE_REGION;\
	}\
}

        /*    ICIC changes ends     */ 

typedef enum dlQosStrategyEventT {
    DL_QOS_EVENT_DRX_OFF,
    DL_QOS_EVENT_DRX_ON,
    DL_QOS_EVENT_MEAS_GAP_OFF,
    DL_QOS_EVENT_MEAS_GAP_ON,
    DL_QOS_EVENT_SYNC_LOSS_OFF,
    DL_QOS_EVENT_SYNC_LOSS_ON,
    DL_QOS_MAX_EVENT
}dlQosStrategyEvent;

typedef enum dlQosTraversalTypeT {
    NEW_TRAVERSAL = 1,
    RETRAVERSAL
}dlQosTraversalType;


#define MAX_NUM_CELL_PREFERRED           MAX_NUM_CELL
#define INVALID_UE_PREFERRED_LIST_INDEX  MAX_NUM_UE_PREFERRED
/*spr 11009 fix start*/
#define MAX_NUM_UE_PREFERRED             MAX_UE_SUPPORTED
/*spr 11009 fix end*/

typedef struct PreferredCellListInfoT
{
    InternalCellIndex cellIndex;
}PreferredCellListInfo;

typedef struct PreferredCellListT
{
    UInt16  ueIndex ;
    UInt8   count;
    UInt8   isPrefRequired;
    PreferredCellListInfo preferredCellListInfo[MAX_NUM_CELL_PREFERRED];
}PreferredCellList;


typedef struct DlQoSPreferredCellListT
{
/* It will determine the UE count in preferredCellList */
    /* +- SPR 18268 */
UInt16 preferredCellListCount;
/* +- SPR 18268 */
PreferredCellList preferredCellList[MAX_NUM_UE_PREFERRED]; 
}DlQoSPreferredCellList;

extern DlQoSPreferredCellList dlQoSPreferredCellList_g[];
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#define STRATEGY_MAX_CONTAINER_INDEX 2

extern DLStrategyScheduleInfo dlStrategyScheduleInfo_g[STRATEGY_MAX_CONTAINER_INDEX][MAX_NUM_CELL];

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/*Complexity Start*/
void updateTopSchedulableVictimUEInfoList(InternalCellIndex internalCellIndex,UInt8 reStartTraversing,UInt8* localArr);
UInt8 eicicFindNumOfBitsinABSPattern(InternalCellIndex internalCellIndex);

void eicicUpdateUEtypeToSchedule(InternalCellIndex internalCellIndex,UInt8 absSubFrameNum,UInt8* numDlUEsToBeScheduled);
/*Complexity End*/
/* ICIC changes start */
MacRetType dlQosStrategyMakeScheduleUeQueue(SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
                                            /* SPR 15909 fix start */
                                            tickType_t   currentGlobalTTITickCount,
                                            /* SPR 15909 fix end */
                                            UInt8*   numDlUEsToBeScheduled,
/*+ SPR 22819*/
/*- SPR 22819*/
                                            InternalCellIndex internalCellIndex);
/* ICIC changes ends  */
/*Complexity Start*/
DLQosStrategyLCNode* eicicUpdatePreferredCellList(
	UInt8 isCaEnabled,
	InternalCellIndex internalCellIndex,
	UInt8   reStartTraversing
	);
/*Complexity End*/
void  dlQosGetTransmissionMode (DLStrategyTxNode * dlStrategyTxNode_p,
                                DLUEContext      * ueDLContext_p,
                                DLHARQProcess    * harqProcess_p,
                                /* + TM7_8 Changes Start */
                                /* SPR 15909 fix start */
                                tickType_t currentGlobalTTITickCount,
                                /* SPR 15909 fix end */
                                InternalCellIndex internalCellIndex);
                                /* - TM7_8 Changes End */


SInt16 dlQosGetResBlksForScheduledBytes(
                                       /* SPR 4543 changes start */
                                       UInt16 maxTBSize, 
                                       /* SPR 4543 changes end */
                                       UInt32 minTBSize, 
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
                                       /* SPR 10143 fix end */
                                       InternalCellIndex internalCellIndex
                                       /* +- SPR 17777 */
                                       );

void dlQosUpdateStrategyNodes( InternalCellIndex internalCellIndex,
                               /* SPR 15909 fix start */
                                tickType_t currentGlobalTTITickCount);
                                /* SPR 15909 fix end */
/* SPR 15909 fix start */
UInt32 dlQosProcessSchedulerStrategy(tickType_t currentGlobalTTITickCount,
 /* SPR 15909 fix end */
        UInt32 dlDelay,InternalCellIndex internalCellIndex);

void dlQosProcessZeroNonZeroTriggerQueue(
        SInt16     zeroNonZeroTriggerQueueCount,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount);
        /* SPR 15909 fix end */

void dlQosProcessUeForPrioritisedLcList(
        DLUEContext *ueDLContext_p,
        ZeroNonZeroTriggerNode * node_p,
        InternalCellIndex internalCellIndex,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount);
        /* SPR 15909 fix end */

void dlQosStrategyProcessTAQueueForNonCAUes( UInt32 timeAdvanceQueueCount, 
                            SInt32 * numberOfAvailableRBs_p,
                            /* SPR 15909 fix start */
                            tickType_t currentGlobalTTITickCount,
                            /* SPR 15909 fix end */
                            TA_Q_TYPE * tempTimeAdvanceQueue_p,
                            UInt8   *numTAToSchedule_p,
                            UInt8   *numDlUEsToBeScheduled_p,
                            /*SPR 6333 Fix Start*/
                            FlagForPendingTADRX flagForTADRXPendingFresh,
                            /*SPR 6333 Fix End*/
                            InternalCellIndex internalCellIndex);
/*START : DRX_MAC_CE*/
void dlQosStrategyProcessDRXQueue( UInt32 drxQueueCount,
                            SInt32 * numberOfAvailableRBs_p,
                            /* SPR 15909 fix start */
                            tickType_t currentGlobalTTITickCount,
                            /* SPR 15909 fix end */
                            DRX_Q_TYPE * tempDRXQueue_p,
                            UInt8   *numDlUEsToBeScheduled_p,
                            InternalCellIndex internalCellIndex);
/*END   : DRX_MAC_CE*/

void dlQosStrategyProcessDRXTrigQueue( SInt16 maxDrxTriggerCount, 
                                       InternalCellIndex internalCellIndex,
                                       /* SPR 15909 fix start */
                                       tickType_t currentGlobalTTITickCount);
                                       /* SPR 15909 fix end */


void dlQosStrategyProcessMeasGapTrigQueue( SInt16 measGapTriggerCount,
                                           InternalCellIndex internalCellIndex,
                                           /* SPR 15909 fix start */
                                           tickType_t
                                           currentGlobalTTITickCount);
                                           /* SPR 15909 fix end */

void dlQosStrategyProcessPdcchOrderQueue( SInt16 pdcchOrderCount, 
					  /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
					  UInt32 dlDelay, 
#endif
					  /* - SPS_TDD_Changes */
                                          InternalCellIndex internalCellIndex,
                                          /* SPR 15909 fix start */
                                          tickType_t currentGlobalTTITickCount);
                                          /* SPR 15909 fix end */
/*QOS_CYCLIC_COMPLEXITY_CHG*/
DLStrategyTxNode* dlQosStrategyUpdateNewTxUEList(DLUEContext *ueDLContext_p,
                                                 MsgType eDlTxMsgType,
                                                 dlQosTraversalType traversalType,
                                                 /* + TM7_8 Changes Start */
                                                 /* SPR 15909 fix start */
                                                 tickType_t currentGlobalTTITickCount,
                                                 /* SPR 15909 fix end */
                                                 InternalCellIndex internalCellIndex);
                                                 /* - TM7_8 Changes End */
/*QOS_CYCLIC_COMPLEXITY_CHG*/
void dlQosPutEntryInPrioritisedLcTree( DLQosStrategyLCNode *dlQosStrategyLCNode_p,
                                       InternalCellIndex internalCellIndex,
                                       /* SPR 15909 fix start */
                                       tickType_t currentGlobalTTITickCount);
                                       /* SPR 15909 fix end */

void dlQosDeleteEntryFromPrioritisedLcTree(
            DLQosStrategyLCNode *dlQosStrategyLCNode_p,
            InternalCellIndex internalCellId,
            /* SPR 15909 fix start */
            tickType_t currentGlobalTTITickCount);
            /* SPR 15909 fix end */
/* ICIC changes start    */
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
        );
/* ICIC changes ends */
/* Retransmission related fucntions prototypes 
 */
/*Complexity Start*/
DLQosStrategyLCNode*  eicicUpdate_ueTypeToScheduleDL_g(
	UInt8 isCaEnabled,
	InternalCellIndex internalCellIndex);
/*Complexity End*/
/* SPR# 5189 Changes Starts */
void handleMIMORetxForOneTBInQos(DLUEContext * ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode * dlStrategyTxNode_p);
/* SPR# 5189 Changes Ends */

void handleMIMORetxForOneTBInQos(DLUEContext * ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        DLStrategyTxNode * dlStrategyTxNode_p);
/* SPR# 5189 Changes Ends  */

/*QOS_RB_RESTICTION_CHANGE*/
void initDlQosRbRestriction(InternalCellIndex internalCellIndex);
/*QOS_RB_RESTICTION_CHANGE*/
void dlQosStrategyProcessExplicitDCIDLQueue( UInt32 explicitDlDciQCount,
                            SInt32 * numberOfAvailableRBs_p,
                            /* SPR 15909 fix start */
                            tickType_t currentGlobalTTITickCount,
                            /* SPR 15909 fix end */
                            EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE * tempExplicitDLDCIScheduleQueue_p,
                            UInt8   *numDlUEsToBeScheduled_p,
                            InternalCellIndex internalCellIndex);
/*ca chngs start*/
void updateTopSchedulableUEInfoList(InternalCellIndex internalCellIndex);
 MacRetType checkTopSchedulableUEInfoList(
                                InternalCellIndex cellId,
                                UInt32 ueId,
                                /* SPR 16116 fix start */
                                UInt32 totLCWeightWithLP);
                                /* SPR 16116 fix end */
/*ca chngs end*/

 MacRetType isUeEligibleForCaScheduling(DLUEContext* ueDLContext_p);
/*SPR 21948 Changes Start */
/* Changes under spr 21762 have been reverted */
/*SPR 21948 Changes End */

void dlQosStrategyProcessTAQueueOfAllCells( 
                                SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
                                /* SPR 15909 fix start */
                                tickType_t currentGlobalTTITickCount,
                                /* SPR 15909 fix end */
                                TA_Q_TYPE** tempTimeAdvanceQueue_p,
                                UInt8   *numTAToSchedule_p,
                                UInt8   *numDlUEsToBeScheduled_p,
                                FlagForPendingTADRX flagForTADRXPendingFresh,
                                InternalCellIndex internalCellIndex);

void dlQosStrategyProcessTAQueueForCAUes( 
                                UInt32 timeAdvanceQueueCount, 
                                SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
                                /* SPR 15909 fix start */
                                tickType_t currentGlobalTTITickCount,
                                /* SPR 15909 fix end */
                                TA_Q_TYPE* tempTimeAdvanceQueue_p,
                                UInt8   *numTAToSchedule_p,
                                UInt8   *numDlUEsToBeScheduled,
                                FlagForPendingTADRX flagForTADRXPendingFresh,
                                InternalCellIndex internalCellIndex);

void dlQosStrategyProcessDRXQueueOfAllCells( 
                                SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
                                /* SPR 15909 fix start */
                                tickType_t currentGlobalTTITickCount,
                                /* SPR 15909 fix end */
                                DRX_Q_TYPE** tempDRXQueue_p,
                                UInt8   *numDlUEsToBeScheduled_p,
                                InternalCellIndex internalCellIndex);

void dlQosStrategyProcessDRXQueueForCAUes( 
                                UInt32 drxQueueCount,
                                SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
                                /* SPR 15909 fix start */
                                tickType_t currentGlobalTTITickCount,
                                /* SPR 15909 fix end */
                                DRX_Q_TYPE * tempDRXQueue_p,
                                UInt8   *numDlUEsToBeScheduled_p,
                                InternalCellIndex internalCellIndex);

void dlQosStrategyProcessExplicitDCIDLQueueOfAllCells( 
                                SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
                                /* SPR 15909 fix start */
                                tickType_t currentGlobalTTITickCount,
                                /* SPR 15909 fix end */
                                EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE** tempExplicitDLDCIScheduleQueue_p,
                                UInt8   *numDlUEsToBeScheduled_p,
                                InternalCellIndex internalCellIndex);

void dlQosStrategyProcessExplicitDCIDLQueueForCAUes( 
                                UInt32 explicitDlDciQCount,
                                SInt32 numberOfAvailableRBs[][MAX_USER_LOCATION], 
                                /* SPR 15909 fix start */
                                tickType_t currentGlobalTTITickCount,
                                /* SPR 15909 fix end */
                                EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE * tempExplicitDLDCIScheduleQueue_p,
                                UInt8   *numDlUEsToBeScheduled_p,
                                InternalCellIndex internalCellIndex);

#endif  /* LTE_MAC_DL_QOS_STRATEGY_H */
