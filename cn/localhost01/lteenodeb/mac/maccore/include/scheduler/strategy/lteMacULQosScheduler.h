/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULQosScheduler.h ,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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

#ifndef LTE_MAC_UL_QOS_SCHEDULER_H
#define LTE_MAC_UL_QOS_SCHEDULER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacUtilityAndSharedStructs.h"
#include "lteMacULUEContext.h"
/* COVERITY 10215 10585 10587 69273-69282 START */
#include "lteMacPDCCHMgr.h"
/* COVERITY 10215 10585 10587 69273-69282 END */
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


typedef enum ulQosTraversalTypeT {
        UL_NEW_TRAVERSAL = 1,
           UL_RETRAVERSAL
}ulQosTraversalType;

/* Indicates the minimum GBR Bytes to be scheduled per TTI */
/*CA Changes start  */
extern UInt32 ulAggregateGbrPerTTI_g[MAX_NUM_CELL][MAX_VALID_UE_TYPE];
/*CA Changes end  */


typedef enum ulQosStrategyEventT {
    UL_QOS_EVENT_DRX_OFF,
    UL_QOS_EVENT_DRX_ON,
    UL_QOS_EVENT_MEAS_GAP_OFF,
    UL_QOS_EVENT_MEAS_GAP_ON,
    UL_QOS_EVENT_SYNC_LOSS_OFF,
    UL_QOS_EVENT_SYNC_LOSS_ON,
    UL_QOS_MAX_EVENT
}ulQosStrategyEvent;


                    
#define UL_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(ulEligibleLcNodeVoid_p,\
                                                 ulEligibleLcNode_p) \
            ulEligibleLcNode_p =  (ULQosStrategyLCNode *) \
                  ((UInt8 *) ulEligibleLcNodeVoid_p - \
                   offsetof(ULQosStrategyLCNode , eligibleLcNodeAnchor));
                            
#define UL_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(ulStrategyLcNodeVoid_p,\
                                                  ulStrategyLcNode_p) \
           ulStrategyLcNode_p =  (ULQosStrategyLCNode *) \
                  ((UInt8 *) ulStrategyLcNodeVoid_p - \
                   offsetof(ULQosStrategyLCNode , scheduledLcNodeAnchor));

/*CA Changes start  */
#define UL_QOS_UPDATE_SCHEDULE_FLAG(flag_p, Event, cellIdx) \
         *flag_p = ulQosStrategyScheduleSM_g[cellIdx][*flag_p][Event];
/*CA Changes end  */


/* These are used for compare function of a search tree*/
#define FIRST_IS_LESS_THAN_SECOND           1
#define FIRST_IS_GREATER_THAN_SECOND      -1
#define FIRST_IS_EQUAL_TO_SECOND            0
#define ERROR_IN_COMPARE                    0
#define MIN_TB_SIZE_FOR_TA_ONLY         1
#define INFINITE_PRIORITY           1000

/* SPR 12793 + 13086 Start */
#ifndef REAL_UE_ENVIORNMENT
#define SR_DROP_PREVENT_TIMER 500
#endif
/* SPR 12793 + 13086 End */

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* SPR 19600 Changes start */
typedef struct UlUeCategoryInfoT
{
    UInt8  ueCategory;
    UInt8  is64QAMSupported;
    UInt16 maxBytesULSCHTb; /* Number of bytes which can be scheduled in one ULSCH PDU */
} UlUeCategoryInfo;
/* SPR 19600 Changes end */

extern UlUeCategoryInfo ulUeCategoryInfo_g[MAX_UE_CATEGORY];

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern LTE_TREE  uLQosStrategyLCTree_g;

/*CA Changes start  */
extern LTE_SQUEUE* ZeroNonZeroBSRTriggerQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/*UL Force Grant Changes Start */
void checkAndProvideForceGrant( UInt8 *deleteUEFlag,
        ULUEContext *ueULContext_p, InternalCellIndex internalCellIndex );
 /*UL Force Grant Changes End */

/*CA Changes start  */
void processBSRZeroNonZeroTriggerQueueQos(InternalCellIndex internalCellIndex, ExecutionLegFunctionArg* execArgs);
/*CA Changes end  */

    /*UL Force Grant Changes Start */
void ulQosPutEntryInNonZeroBSRList(
        ULUEContext* ueULContext_p,
        UInt8 lcgId,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex,
        ExecutionLegFunctionArg* execArgs, UInt8 *forceGrantFlag_p );
 /*UL Force Grant Changes End */
void ulQosCalcGbrLCWeightAndInsert(
        ULUEContext *ueULContext_p,
        UInt8 lcIndex,
        UInt16 drxOnDurationDelay,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex);
        /*CA Changes end  */

void ulQosCalcNgbrLCWeightAndInsert(
        ULUEContext *ueULContext_p,
        UInt16 drxOnDurationDelay,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex,
         /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs);

/* SPR 5599 changes start (ZIP ID 129072) */
 void ulQosPutEntryInPrioritisedLcTree(
/* SPR 5599 changes end (ZIP ID 129072) */
        ULQosStrategyLCNode *ulStrategyTxNode_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
        /*CA Changes end  */


/* SPR 5599 changes start (ZIP ID 129072) */
void ulQosDeleteEntryInPrioritisedLcTree(
/* SPR 5599 changes end (ZIP ID 129072) */
        ULQosStrategyLCNode *ulStrategyTxNode_p);


/* SPR 5599 changes start (ZIP ID 129072) */
 void ulQosInitLcTree (
/* SPR 5599 changes end (ZIP ID 129072) */
        /* Pointer to Tree */
        LTE_TREE *tree_p,
        /* Pointer to compare function :
         *          * it returns 0 if A==B, 1 if A<B and -1 if A>B
         *                   */
        SInt32 (*compare_p)(const void *, const void *),

        /* Pointer to function returning Key
         *          *        */
        const void *(*keyOf_p)(const LTE_TREE_NODE *));


/* SPR 5599 changes start (ZIP ID 129072) */
 LTE_TREE_NODE * ulQosGetFirstLcNode (
/* SPR 5599 changes end (ZIP ID 129072) */
        /* Pointer to Tree */
        LTE_TREE *tree_p);

/* SPR 5599 changes start (ZIP ID 129072) */
 LTE_TREE_NODE * ulQosGetNextLcNode (
/* SPR 5599 changes end (ZIP ID 129072) */
        /* Pointer to Tree Node*/
        LTE_TREE_NODE *treeNode_p);

/* SPR 5599 changes start (ZIP ID 129072) */
 void ulQosDeleteLcNode (
/* SPR 5599 changes end (ZIP ID 129072) */
        /* Pointer to Tree */
        LTE_TREE *tree_p,
        /* Pointer to node being inserted */
        LTE_TREE_NODE *entry_p);

/* SPR 5599 changes start (ZIP ID 129072) */
 void ulQosInsertLcNode (
/* SPR 5599 changes end (ZIP ID 129072) */
        /* Pointer to Tree */
        LTE_TREE *tree_p,
        /* Pointer to node being inserted */
        LTE_TREE_NODE *entry_p);


SInt32 ulQosCompareLcNode (
        /* Pointer to first element being compared */
        const void * firstElem_p ,
        /* Pointer to second element being compared */
        const void * secondElem_p
        );


/* SPR 5599 changes start (ZIP ID 129072) */
 const void * ulQosKeyOfLcNode (
/* SPR 5599 changes end (ZIP ID 129072) */
        /* Pointer to Tree node */
        const YTNODE * node_p
        );

/* SPR 5599 changes start (ZIP ID 129072) */
void ulQosDeleteEntryFromNonZeroBSRList(
/* SPR 5599 changes end (ZIP ID 129072) */
        ULUEContext* ueULContext_p,
        UInt8 lcgId,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex);
        /*CA Changes end  */
/* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
void ulQosUpdateScheduledTokens(ULUEContext *ulUEContext_p, 
                                SInt16 tbSize,
                                UInt8 tempHarqProcessId,
                                InternalCellIndex internalCellIndex,
                                ExecutionLegFunctionArg* execArgs);
#elif TDD_CONFIG
void ulQosUpdateScheduledTokens(ULUEContext *ulUEContext_p, 
        SInt16 tbSize,
        UInt8 tempHarqProcessId,
        InternalCellIndex internalCellIndex,
        UInt8 ulSubFrameNum,
        UInt8 tokenUpdateFromStr,
        ExecutionLegFunctionArg* execArgs);
void deleteScheduledLcNode(UInt8 ulGrantSubFrameNum,InternalCellIndex internalCellIndex);
#endif


/*CA Changes start  */
void ulQosUpdateTokens(ULUEContext *ulUEContext_p, InternalCellIndex internalCellIndex,ExecutionLegFunctionArg* execArgs);

/* SPR 5599 changes start (ZIP ID 129072) */
 void ulQosCalcGbrLCTknValue (ULQosInfo *qosInfo_p, InternalCellIndex internalCellIndex);

 void ulQosCalcNonGbrLCTknValue (ULAmbrInfo *ulAmbrInfo_p, InternalCellIndex internalCellIndex);
/*CA Changes end  */

 UInt16 ulQosGetQloadPriority(UInt32 *lcQlTable_p,
/* SPR 5599 changes end (ZIP ID 129072) */
        UInt32 qload
        );

/* SPR 5599 changes start (ZIP ID 129072) */
        /* SPR 15909 fix start */
 UInt16 ulQosGetPdbPriority(tickType_t currentTick,
/* SPR 5599 changes end (ZIP ID 129072) */
        UInt32 *lcPdbTable_p,
        tickType_t headOfLinePktDly,
        /* SPR 15909 fix end */
        UInt16 drxOnDurationDelay
        );

/* SPR 5599 changes start (ZIP ID 129072) */
 UInt16 ulQosGetTknPriority(UInt32   currScheduledTkn,
/* SPR 5599 changes end (ZIP ID 129072) */
        UInt32 *lcTknTable_p,
        UInt32  totSchdTkn);


/* SPR 5599 changes start (ZIP ID 129072) */
 /* SPR 11081 changes start */
 UInt16 ulQosGetQciPriority (UInt16 qciConfigPriority);
 /* SPR 11081 changes end */

 UInt16 ulQosGetMcsPriority (UInt16 mcs);

 void ulQosUpdateQloadTableForLcPriority(UInt32 *lcQlTable_p,
/* SPR 5599 changes end (ZIP ID 129072) */
/* L2_PERFORMANCE_OPT_CHANGES_START */
        UInt32 bitRate,
/* L2_PERFORMANCE_OPT_CHANGES_END */
        UInt16 pktDelayBgt);


/* SPR 5599 changes start (ZIP ID 129072) */
 void ulQosUpdateTknTableForLcPriority (UInt32 *lcTknTable_p,
/* SPR 5599 changes end (ZIP ID 129072) */
/* L2_PERFORMANCE_OPT_CHANGES_START */
        /*CA Changes start  */
        UInt32 bitRate, InternalCellIndex internalCellIndex);
        /*CA Changes end  */
/* L2_PERFORMANCE_OPT_CHANGES_END */

/* SPR 5599 changes start (ZIP ID 129072) */
 void ulQosUpdatePdbTableForLcPriority (UInt32 *lcPdbTable_p,
/* SPR 5599 changes end (ZIP ID 129072) */
        UInt16 pktDelayBgt);

void ulQosStrategyMakeScheduledUeQueue(UInt32  *numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        SInt16  *numUlUEsToBeScheduled_p
#ifdef FDD_CONFIG
        ,UInt8 ulDelay
#endif
        /* CA Changes start */
        ,InternalCellIndex internalCellIndex
        /* CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

void ulQosReTraversePriorityLcList(SInt16  *numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        SInt16  *numUlUEsToBeScheduled_p
#ifdef FDD_CONFIG
        ,UInt8 ulDelay
        /* TDD Config 0 Changes Start */
#elif TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
            /* TDD Config 0 Changes End */

        ,InternalCellIndex internalCellIndex
        /* SPR 22539 Fix + */
        ,UInt8 allowAllocBitRate 
        /* SPR 22539 Fix - */
        );

/*QOS_CYCLIC_COMPLEXITY_CHG*/
ULStrategyTxNode * ulQosStrategyUpdateNewTxUEList(
        UInt16 ueId,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        ulQosTraversalType traversalType
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );
/*QOS_CYCLIC_COMPLEXITY_CHG*/

/*CA Changes start  */
void ulQosStrategyProcessDRXTrigQueue(InternalCellIndex  internalCellIndex ,ExecutionLegFunctionArg* execArgs);

void ulQosStrategyProcessMeasGapTrigQueue(InternalCellIndex  internalCellIndex ,ExecutionLegFunctionArg* execArgs);
void ulQosStrategyProcessPdcchOrderQueue(InternalCellIndex  internalCellIndex  ,ExecutionLegFunctionArg* execArgs);
/*CA Changes end  */


UInt32 ulQosScheduler(UInt32  *numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        SInt16  numUlUEsToBeScheduled_p
#ifdef FDD_CONFIG
        ,UInt8 ulDelay
#endif
        /* CA Changes start */
        ,InternalCellIndex internalCellIndex
        /* CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );
/* SPR 15909 fix start */
void ulQosProcessTokenUpdate(/* +- SPR 17777 */
/* SPR 15909 fix end */        
        /* CA Changes start */
        InternalCellIndex internalCellIndex
        /* CA Changes end */
        ,ExecutionLegFunctionArg* execArgs);

/* SPR 7435 Changes Starts */
/* SPR 5599 changes start (ZIP ID 129072) */
 void ulQosDeleteEntryFromPrioritisedLcTree(
/* SPR 5599 changes end (ZIP ID 129072) */
/* SPR 7435 Changes Ends */
        ULQosStrategyLCNode *ulStrategyLcNode_p,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex);
        /*CA Changes end  */

/*CA Changes start  */
void initUlQosStrategyScheduleSM(UInt8 numOfCells);

/*QOS_RB_RESTICTION_CHANGE*/
void initUlQosRbRestriction(InternalCellIndex  internalCellIndex);
/*CA Changes end  */
/*QOS_RB_RESTICTION_CHANGE*/
#endif  /* LTE_MAC_UL_QOS_SCHEDULER_H */
