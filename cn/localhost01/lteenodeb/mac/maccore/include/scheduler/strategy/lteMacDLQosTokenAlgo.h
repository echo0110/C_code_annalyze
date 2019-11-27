/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLFairScheduler.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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

#ifndef LTE_MAC_DL_QOS_TOKEN_ALGO_H
#define LTE_MAC_DL_QOS_TOKEN_ALGO_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define MAX_MCS_PRIORITIES          3
#define MAX_QCI_PRIORITIES          9
  /* These are used for compare function of a search tree*/
#define FIRST_IS_LESS_THAN_SECOND           1
#define FIRST_IS_GREATER_THAN_SECOND      -1
#define FIRST_IS_EQUAL_TO_SECOND            0
#define ERROR_IN_COMPARE                    0
#define DL_QOS_MAC_HEADER_SIZE             3
#define INFINITE_PRIORITY           1000

#define TOKEN_UPDATE_PERIOD  10
#define MAX_MCS_INDEX_FOR_PRI 29
#define MAX_QOS_TOKEN_LTE_LIST 5


typedef struct TknUpdateUENodeT
{
        LTE_LIST_NODE tknUpdateAnchor;
            UInt16 ueIndex;
}TknUpdateUENode;

/* Struct for UEId mapping array node */
typedef struct TknUpdateUENodeMapT
{
    TknUpdateUENode *ueNodeAddrDl_p;
    TknUpdateUENode *ueNodeAddrUl_p;
    UInt16 tknPeriodicityArrIndxDl;
    UInt16 tknPeriodicityArrIndxUl;
}TknUpdateUENodeMap;

/*CA Changes start  */
typedef struct QosTokenT
{
    UInt16 tknUpdateUeCount;
    UInt16 ulMaxBatches; 
    UInt16 dlMaxBatches;
    /* variable to store current token periodicity value and it will be used
    * when token periodicity value will be changed */
    /* SPR7784 Fix Start */
    UInt16 previousTokenPeriodicityValDl;
    UInt16 previousTokenPeriodicityValUl;
    /* SPR7784 Fix End */
    UInt16 dlBatchListIndex;
    UInt16 ulBatchListIndex;
}QosToken;
/*CA Changes end  */



/* Struct for periodicity array list */
typedef struct TokenPeriodicityListElemT
{
    LTE_LIST_NODE elemAnchor;
    LTE_LIST tokenList;
}TokenPeriodicityLisElem;

/* Struct for mapping of token periodicity array list */
typedef struct TokenPeriodicityMapT
{
    LTE_LIST_NODE node;
    UInt16 noOfUes;
    UInt16 tknPeriodicityArrIndx;
}TokenPeriodicityMap;


#define QOS_GBR_BUCKET_DEPTH_FACTOR_BIT  0x0001
#define QOS_AMBR_BUCKET_DEPTH_FACTOR_BIT 0x0002
#define QOS_TOKEN_PERIODICITY_BIT        0x0004 
#define QOS_OVERALLOC_FACTOR_BIT         0x0008
#define QOS_DL_STRATEGY_WT_BIT           0x0010
#define QOS_UL_STRATEGY_WT_BIT           0x0020
#define QOS_MODIFY_RETRAVERSAL_FLAG_BIT  0x0040

/* QOS_OAM_MODIFY_CHANGES_end */





/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

void dlQosInitLcTree (
        /* Pointer to Tree */
        MACPriorityLcTree *tree_p, 
        /* Pointer to compare function :
         * it returns 0 if A==B, 1 if A<B and -1 if A>B
         */
        SInt32 (*compare_p)(const void *, const void *),

        /* Pointer to function returning Key
         *        */
        const void *(*keyOf_p)(const YTNODE *));


DlPriorityLcNode * dlQosGetFirstLcNode (
        /* Pointer to Tree */
        MACPriorityLcTree *tree_p);

DlPriorityLcNode * dlQosGetNextLcNode (
        /* Pointer to node being inserted */
        DlPriorityLcNode *entry_p);

void dlQosDeleteLcNode (
        /* Pointer to Tree */
        MACPriorityLcTree *tree_p,

        /* Pointer to node being inserted */
        DlPriorityLcNode *entry_p);

void dlQosInsertLcNode (
        /* Pointer to Tree */
        MACPriorityLcTree *tree_p,

        /* Pointer to node being inserted */
        DlPriorityLcNode *entry_p);

SInt32 dlQosCompareLcNode (
        /* Pointer to first element being compared */
        const void * firstElem_p ,

        /* Pointer to second element being compared */
        const void * secondElem_p
        );

const void * dlQosKeyOfLcNode (
        /* Pointer to Tree node */
        const YTNODE * node_p
        );

UInt32 dlGetTreeNodeCount (
        const MACPriorityLcTree *p_tree);

 void dlQosCalcLCWeightAndInsert(
    DLLogicalChannelInfo *dlLogicalChannelInfo_p,
    UInt8 mcsPriority,
    UInt16 drxOnDurationDelay,
    InternalCellIndex internalCellIndex);

void dlQosUpdateScheduledTokens(DLUEContext *ueDLContext_p,
InternalCellIndex internalCellIndex);

void dlQosUpdateTokens(DLUEContext *ulUEContext_p,
InternalCellIndex internalCellIndex);

 void dlQosCalcGbrLCTknRate (DlLcQosInfo *dlLcQosInfo_p
                     ,InternalCellIndex internalCellIndex);

 void dlQosCalcAmbrTknRate(DLAmbrInfo *dlAmbrInfo_p
                , UInt16 dlTokenPeriodicity);

void dlQosUpdateNonGbrLCTknValue (DLUEContext *ueDLContext_p);

 UInt16 dlQosGetQloadPriority(UInt32 *lcQlTable_p, 
        UInt32 qload
        );

                                /* SPR 15909 fix start */
UInt16 dlQosGetPdbPriority(tickType_t currentTick, 
                                  UInt16 *lcPdbTable_p, 
                                  tickType_t pktArrTs, 
                                /* SPR 15909 fix end */
        UInt16 drxOnDurationDelay
        );

 UInt16 dlQosGetTknPriority(
                    UInt32 currScheduledTkn, /* Stkn(t) */
                    UInt32 *lcTknTable_p,
                    UInt32 totSchdTkn);  /* Ttkn(t-1) - Stkn(t-N) */

 UInt16 dlQosGetQciPriority (UInt8 qci);

 UInt16 dlQosGetMcsPriority (UInt8 mcs);
/* SPR 21494 End */

 void dlQosUpdateQloadTableForLcPriority(UInt32 *lcQlTable_p, 
/* L2_PERFORMANCE_OPT_CHANGES_START */
        UInt32 bitRate, 
/* L2_PERFORMANCE_OPT_CHANGES_END */
        UInt16 pktDelayBgt);

 void dlQosUpdateTknTableForLcPriority (UInt32 *lcTknTable_p, 
/* L2_PERFORMANCE_OPT_CHANGES_START */
                                  UInt32 bitRate
                                  ,UInt16 dlTokenPeriodicity);
/* L2_PERFORMANCE_OPT_CHANGES_END */

 void dlQosUpdatePdbTableForLcPriority (UInt16 *lcPdbTable_p, 
                                              UInt16 pktDelayBgt);
/* SPR 15909 fix start */
/* +- SPR 17777 */
void dlQosProcessTokenUpdate(InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
/* SPR 15909 fix end */
void qosProcessTknBktAlgoForUeAdd(UInt16 ueIndex, InternalCellIndex internalCellIndex);
void qosProcessTknBktAlgoForUeDelete(UInt16 ueIndex, InternalCellIndex internalCellId);
/* QOS_OAM_MODIFY_CHANGES_start */
/* Coverity 66974 66975 Start */
MacRetType qosModifyQosParameters(MacModifyQosPamams *p_modifyPamams, InternalCellIndex internalCellIndex);
/* Coverity 66974 66975 End */
/* QOS_OAM_MODIFY_CHANGES_end */

/* + Phase2 -   */
void lteMacDLQoSDistributeTokenAmongCells(DLUEContext *ueDLContext_p); 
/* - Phase2 -   */
void initQosToken(UInt8 numOfCells);
void deInitQosToken(InternalCellIndex internalCellIndex);
void initTokenPeriodicityArray(InternalCellIndex internalCellIndex);
    /* +- SPR 17777 */
void deInitTokenPeriodicityArray(InternalCellIndex internalCellIndex);
#endif  /* LTE_MAC_DL_QOS_STRATEGY_H */
