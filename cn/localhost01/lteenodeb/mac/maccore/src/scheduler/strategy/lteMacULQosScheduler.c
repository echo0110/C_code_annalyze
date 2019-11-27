/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULQosScheduler.c,v 1.3.6.1 2010/08/02 08:13:42 gur21006 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file includes the functionality of distributing the
 Uplink RBs according to Quality of Service. The number 
 of RBs distributed to particular UE will be depending  
 upon the UL stratergy algorithm. 
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacULRoundRobinScheduler.h"
#include "lteMacULQosScheduler.h"
#include "lteMacCellConfMgr.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacEncoder.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#include "lteMacRandomAccess.h"
#include "lteMacPUSCH.h"
#endif
#include "lteMacStatsManager.h"
#include "lteMacULResourceInfoMgr.h"
#include "ltePerfStats.h"
/* SPR 10032 Fix Starts*/
#include "lteMacPuschPowerControl.h"
/* SPR 10032 Fix Ends*/
#include "lteMacSFNMgr.h"

/* SPR 23402 Changes Start */
/* MOCN CHANGES START */
#include "lteMacOAMInterface.h"
/* MOCN CHANGES END */
/* SPR 23402 Changes End */
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
/*A pointer to nodeAnchor of the UE in ueNonZeroQueueLoadList_g*/
LTE_LIST_NODE * ulNonZeroListStartRoundRobin_p;
/*CA Changes start  */
LTE_TREE*  ulQosStrategyLCTree_gp[MAX_NUM_CELL][MAX_VALID_UE_TYPE] = {{PNULL,PNULL},{PNULL,PNULL}}; /* EICIC +-*/
/*CA Changes start  */
UInt32 ulAggregateGbrPerTTI_g[MAX_NUM_CELL][MAX_VALID_UE_TYPE] = {{0,0},{0,0}};
/*CA Changes end  */
extern UInt8 isEicicEnabled_g[MAX_NUM_CELL];
/*Queue of type LTE_CIRCQUEUE which contains Triggers of UEs trigger to enter/to
 *    remove from / into the NonZeroQueueLoadAndHarqList */
UL_DRX_ON_OFF_TRIGGER_TYPE* ulDRXOnOffTriggerQueue_gp[MAX_NUM_CELL] = {PNULL};
UL_MEAS_GAPON_OFF_TRIGGER_TYPE* ulMeasGapOnOffTriggerQueue_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */
/* QOS_CHG */
#ifdef SSI_DEBUG_STATS
/* CA Stats Changes Start */
extern void  updateUlQosPerTTIStats(InternalCellIndex internalCellIndex);
/* CA Stats Changes End */
/* QOS_CHG */
#endif
#ifdef FDD_CONFIG
/*CA Changes start  */
extern UInt32 availableRBForTtiBundlingPerTick_g[MAX_NUM_CELL];
/*CA Changes end  */
#endif
extern UInt8 checkSpsOccasionTti( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex);
extern UInt8 checkSpsActTti( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex);
    /*UL Force Grant Changes Start */
    /*Code Removed*/
     /*UL Force Grant Changes End */

/*QOS_RB_RESTICTION_CHANGE*/
/*CA Changes start  */
STATIC UInt32 maxRestrictedRbUL_g[MAX_NUM_CELL];
/*QOS_RB_RESTICTION_CHANGE*/

STATIC UInt16 unusedULStrategyBufferRB_g[MAX_NUM_CELL];
/*CA Changes end  */
/*+ Dyn UEs To Be SchPerTTI +*/
extern UInt16 totalActiveUEsUL_g[MAX_NUM_CELL];
/*- Dyn UEs To Be SchPerTTI -*/
/* EICIC +*/
extern UInt8 ueTypeToScheduleUL_g[MAX_NUM_CELL] ;
/* EICIC -*/
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
extern mocnOperatorParams mocnOperatorParams_g;
/* MOCN Changes End */
/* SPR 23402 Changes End */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* SPR 12344 changes start */
#define        MAX_MCS_RANGE   29
/* SPR 12344 changes end */

/* SPR 19600 Changes start */
/* The below table defines the MAX TB sizes based on the UE category
 * It is derived from the refrence - Table 4.1-1 : Spec 36.306 - Rel 10 
 */
/* Uplink Strategy shall consider while scheduling that UE is not scheduled for 
   the amount of data exceeding maximum TB Size limit of the UE as per UE category.
   As per 36.306, Table 4.1-2 -->  
   UE Category     Maximum number of bits of an UL-SCH transport block 
                   transmitted within a TTI   
   Category 1              5160               = 0645 bytes 
   Category 2              25456              = 3182 bytes 
   Category 3              51024              = 6378 bytes 
   Category 4              51024              = 6378 bytes 
   Category 5              75376              = 9422 bytes  
   Category 6              51024              = 6378 bytes 
   Category 7              51024              = 6378 bytes 
   Category 8              149776             = 6378 bytes 
 */
UlUeCategoryInfo ulUeCategoryInfo_g[MAX_UE_CATEGORY] = {

    {1, 0, 645},
    {2, 0, 3182},
    {3, 0, 6378},
    {4, 0, 6378},
    {5, 1, 9422},
    {6, 0, 6378},
    {7, 0, 6378},
    {8, 1, 18722}
};
/* SPR 19600 Changes end */

UInt16 ulLcMcsTable_g[MAX_MCS_PRIORITIES] = {

        21, /* 64 QAM */
        11,  /* 16 QAM */
        0   /* QPSK   */

};

/* SPR 12344 changes start */
/* This array holds the prority values for the MCS
 * */
UInt16 ulLcMcsPriMap_g[MAX_MCS_RANGE] = {
        1,/* MCS Index 0 */
        1,/* MCS Index 1 */
        1,/* MCS Index 2 */
        2,/* MCS Index 3 */
        2,/* MCS Index 4 */
        2,/* MCS Index 5 */
        3,/* MCS Index 6 */
        3,/* MCS Index 7 */
        3,/* MCS Index 8 */
        4,/* MCS Index 9 */
        4,/* MCS Index 10 */
        4,/* MCS Index 11 */
        5,/* MCS Index 12 */
        5,/* MCS Index 13 */
        5,/* MCS Index 14 */
        6,/* MCS Index 15 */
        6,/* MCS Index 16 */
        6,/* MCS Index 17 */
        7,/* MCS Index 18 */
        7,/* MCS Index 19 */
        7,/* MCS Index 20 */
        8,/* MCS Index 21 */
        8,/* MCS Index 22 */
        8,/* MCS Index 23 */
        9,/* MCS Index 24 */
        9,/* MCS Index 25 */
        9,/* MCS Index 26 */
       10,/* MCS Index 27 */
       10,/* MCS Index 28 */
};
/* SPR 12344 changes end */

/* This array holds the prority values for the QCI
 *  * the same will be mapped by LC QCI values*/
static UInt16 ulLcQciPriMap_g[MAX_QCI_PRIORITIES] = {
    10, /* QCI priority 1 */
    9,  /* QCI priority 2 */
    8,  /* QCI priority 3 */
    7,  /* QCI priority 4 */
    6,  /* QCI priority 5 */
    5,  /* QCI priority 6 */
    4,  /* QCI priority 7 */
    3,  /* QCI priority 8 */
    2   /* QCI priority 9 */
};

/* This array holds the prority values for the Available Token
 *  * the same will be mapped by ratio of
 *   * scheduled tokens and availed tokens of LC, during last
 *    * Scheduling window(MAX_TKN_SCHD_WINDOW)
 *     * i.e for last N * TOKEN_UPDATE_PERIOD duration
 *      */

static UInt16 ulLcTknPriMap_g[MAX_TOKEN_PRIORITIES] = {
    10,  /* 0 < alloc opp < 0.5 expected alloc   */
    8,  /* 0.5 < alloc opp < 0.8 expected alloc */
    5,  /* 0.8 < alloc opp < 0.9 expected alloc */
    2,  /* 0.9  < alloc opp < expected alloc    */
    0   /*  expected alloc  <=   alloc opp      */
};


/* This array holds the prority values for the  Qload
 *  * the same will be mapped by ratio of qload by QSize of LC */
static UInt16 ulLcQloadPriMap_g[MAX_QLOAD_PRIORITIES] = {
    3, /* 0< LC Q load < 0.25 Q Size */
    5, /* 0.25 Q Size < LC Q load < 0.5 Q Size */
    8, /* 0.5 Q Size < LC  Q load < 0.75 Q Size */
    10, /* 0.75 Q Size < LC Q load */
    10 /* if max Qload */
};

/* This array holds the prority values for the PDB
 *  * the same will be mapped by ratio of
 *   * delay of head of line packet and PDB
 *    */
static UInt16 ulLcPdbPriMap_g[MAX_PDB_PRIORITIES] = {
    3,  /*  0< Delay < 0.25 PDB */
    5,  /*  0.25 < Delay < 0.5 PDB */
    8,  /*  0.5 < Delay < 0.75 PDB */
    10, /*  0.75 < Delay < 1.2 PDB */
    /* SPR 6430 changes start */
    20  /*  1.2 PDB<Delay */
    /* SPR 6430 changes end */
};

/*CA Changes start  */
/* SPR 23249 Fix + */
ULStrategyScheduleInfo* ulStrategyScheduleInfo_gp[MAX_NUM_CELL] = {PNULL,PNULL};
/* SPR 23249 Fix - */

    /*coverity-530 CID-32340*/
static ULScheduleFlag ulQosStrategyScheduleSM_g[MAX_NUM_CELL][UL_QOS_MAX_STATE][UL_QOS_MAX_EVENT] = {{{UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON}}};
    /*coverity-530 CID-32340*/
/*CA Changes end  */

void ulQosProcessUeForRBRestriction(
                           ulQosTraversalType traversalType,
                           ULUEContext        *ueULContext_p);
void ulProcessSpsUeForActDeact(ULUEContext *ulUEContext_p,
                               UInt8 lcId,
                               /*CA Changes start  */
                               InternalCellIndex internalCellIndex
                               /*CA Changes end  */                               
                               );


/*****************************************************************************
 * Functions
 *****************************************************************************/
/* Cyclomatic Complexity Changes -starts here */
STATIC  void ulQosGetNextNodeFromPriorityTree(ULQosStrategyLCNode ** ulQosStrategyLCNode_p,
                                             UInt8 * gbrSchedulingFlag,                                             
                                             /*CA Changes start  */
                                             InternalCellIndex internalCellIndex);
                                             /*CA Changes end  */

STATIC  MacRetType ulQosValidatePriorityTreeNode(ULUEContext * ueULContext_p,
                                         ULQosStrategyLCNode * ulQosStrategyLCNode_p,
                                         UInt8  gbrSchedulingFlag,
                                         UInt8 * ulHarqProcessId,
                                         /* SPR 15909 fix start */
                                         tickType_t currentGlobalTTITickCount
                                         /* SPR 15909 fix end */
#ifdef FDD_CONFIG
                                         ,UInt8 ulDelay
#endif
                                         ,InternalCellIndex internalCellIndex
                                         /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG                                         
                                         ,UInt8 ulSubFrameNum
#endif
                                         /* TDD Config 0 Changes End */

                                          );

STATIC  SInt16 ulQosCheckForOverAllocation(ULStrategyTxNode * ulStrategyTxNode_p,
                                          ULUEContext *ueULContext_p,
                                          SInt16   numAvailableRBs,
                                          SInt16 requiredRB,
                                          /*SPR 16417 Changes Start*/
                                          UInt32 schedulableBytes,
#ifdef FDD_CONFIG
                                          /* SPR 13532 Fix Start */
                                          /* SPR 15909 fix start */
                                          tickType_t currentGlobalTickWithULDelay
                                          /* SPR 13532 Fix End */
#elif TDD_CONFIG
                                          tickType_t currentGlobalTick,
                                          /* SPR 15909 fix end */
                                          UInt8 ulSubFrameNum
#endif
                                          /*SPR 16417 Changes End*/
                                          );
/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
STATIC void ulQosAdjustScheduledTokensFromPdcchPusch(
        ULUEContext *ulUEContext_p,
        ULQosStrategyLCNode *ulQosStrategyLCNode_p,
        ULHarqInfo *tempULHarqInfo_p,
        UInt16 allocatedBytes,
        UInt8 ulSubFrameIdx,
        ExecutionLegFunctionArg* execArgs,
        InternalCellIndex internalCellIndex);
STATIC void ulQosUpdateScheduledTokensFromStrategy(
        ULUEContext *ulUEContext_p,
        ULQosStrategyLCNode *ulQosStrategyLCNode_p,
        UInt16 allocatedBytes,
        /* SPR 10984 FIX START */
        UInt8 ulSubFrameIdx,
        /* SPR 10984 FIX END */
        ExecutionLegFunctionArg* execArgs,
        InternalCellIndex internalCellIndex);
#endif

/* TDD Config 0 Changes End */
/* Cyclomatic Complexity Changes -ends here */

/*****************************************************************************
 * Function Name  : processBSRZeroNonZeroTriggerQueueQos
 * Inputs         : internalCellIndex
 * Outputs        : updates the Non Zero BSR List
 * Returns        : None
 * Description    : Its main function will be to Process the 
 					BSRZeroNonZeroTriggerQueue Queue and Update the 
 					NonZeroQueueLoadAndHarq List,after adding and deleting UEs 
 					according to Trigger
 *****************************************************************************/
/*CA Changes start  */
void processBSRZeroNonZeroTriggerQueueQos(InternalCellIndex internalCellIndex,ExecutionLegFunctionArg* execArgs)
/*CA Changes end  */
{
    bsrQueueNode * node_p       = PNULL;
    ULUEContext* ueULContext_p  = PNULL;  
    UInt32 ueIndex;
    UInt8  lcgId;
    ULUEContextInfo * ueULContextInfo_p = PNULL;
    UInt32 zeroNonZeroBSRTriggerQueueCount = 0;
    UInt32 j                    = 0;
    /*UL Force Grant Changes Start */
    UInt8 forceGrantFlag = LTE_FALSE;                         
    /*UL Force Grant Changes End */

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Entry:[UL-Strategy][%s]",
            __func__);

    for ( j = NUM_OF_DEMUX_THREADS; j > 0; j-- )
    {
        /*CA Changes start  */
        zeroNonZeroBSRTriggerQueueCount =  sQueueCount
            (ZeroNonZeroBSRTriggerQueue_gp[internalCellIndex] + (j - 1));
        /*CA Changes end  */

        /*Processed for the number of  Queue Count*/
        while (zeroNonZeroBSRTriggerQueueCount)
        {
            /*CA Changes start  */
            node_p = ( bsrQueueNode * )
                popNode(ZeroNonZeroBSRTriggerQueue_gp[internalCellIndex] + (j - 1));
            /*CA Changes start  */

            /*Checking the Node for NULL*/
            if (node_p)
            {
                ueIndex = node_p->ueIdx;
                lcgId   = node_p->lcgId;
                ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
                /*Checking the UE Context & Checking for Pending 
                  delete Flag of UE*/
                if (!(ueULContextInfo_p->pendingDeleteFlag))
                {
                    ueULContext_p = ueULContextInfo_p->ulUEContext_p;
                    /*Trigger Node has flag to add the UE in NonZero List*/
                    if (ADD_TRIGGER == node_p->addDeleteTrigger )
                    {
                        LOG_UT(MAC_BSR_ADD_TRIGGER,LOGDEBUG,
                                MAC_UL_Strategy,
                                getCurrentTick(),
                                ueIndex,
                                lcgId,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"");
                        /*UL Force Grant Changes Start */
                        /* Trigger QOS Strategy including force grant */
                        ulQosPutEntryInNonZeroBSRList(ueULContext_p, lcgId,
                                internalCellIndex, execArgs, &forceGrantFlag);

                        /* Send Add trigger bsrNetTotal is non-zero */
                        if( LTE_TRUE == forceGrantFlag && 
                                0 != ueULContext_p->bsrNetTotal )
                           {
                            putEntryInBSRQueue(ueIndex, lcgId, 0, 
                                    ADD_TRIGGER, internalCellIndex);
                           }
                         }
                     /*UL Force Grant Changes End */
                    /*Trigger Node have flag is to delete the UE from
                      Non Zero List*/
                    else
                    {
                        /*check whether the UE Queue Load is also Zero,
                          if Not then UE cannot be deleted from the 
                          non Zero List*/
                        LOG_UT(MAC_BSR_DELETE_TRIGGER,LOGDEBUG,
                                MAC_UL_Strategy,
                                getCurrentTick(),
                                ueIndex,
                                lcgId,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"");
                        ulQosDeleteEntryFromNonZeroBSRList(ueULContext_p, lcgId, internalCellIndex);
                    }
                    /*+ Dyn UEs To Be SchPerTTI +*/
                    if(listCount(&(ueULContext_p->ulEligibleLcList)) > 0)
                    {
                        if (!ueULContext_p->isUEConsideredForTotalActiveUeUL)
                        {
                            totalActiveUEsUL_g[internalCellIndex]++;
                            ueULContext_p->isUEConsideredForTotalActiveUeUL = TRUE;
                        }
                    }
                    else
                    {
                        if(ueULContext_p->isUEConsideredForTotalActiveUeUL)
                        {
                            totalActiveUEsUL_g[internalCellIndex]--;
                            ueULContext_p->isUEConsideredForTotalActiveUeUL = FALSE;
                        }
                    }
                    /*+ Dyn UEs To Be SchPerTTI +*/
                }
                else
                {
                }
            }
            else
            {
                LOG_UT(MAC_QUEUE_IS_EMPTY_IN_UL_LOG_ID,LOGDEBUG,
                        MAC_UL_Strategy,
                        getCurrentTick(),
                        zeroNonZeroBSRTriggerQueueCount,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"ZeroNonZeroBSRQueue");
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
 * Function Name  : ulQosCalcGbrLCWeightAndInsert
 * Inputs         : uieULContext_p - pointer to UL UE context Info,
 *                  lcIndex - logical channel index,
 *                  drxOnDurationDelay - Delay duration when UE is in DRX on 
 *                                       state,
 *					internalCellIndex
 * Outputs        : Calculates aggregate GBR weight of the LC  
 * Returns        : None
 * Description    : It calculates aggregate GBR weight of an LC by formula
 *                  AggWeight = Pqci * Wqci + Ppdb * Wpdb + Ptkn * Wtkn + 
 *                  Pql * Wql + Pmcs * Wmcs
 *                  and inserts the node in UL Priortised LC Tree.
 *****************************************************************************/
void ulQosCalcGbrLCWeightAndInsert(
        ULUEContext *ueULContext_p,
        UInt8 lcIndex,
        UInt16 drxOnDurationDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
        /*CA Changes end  */
{
    UInt8 lcId = 0;
    UInt8 lcCount = 0;
    UInt32 totalAvailableLcgToken = 0;
    UInt32 calculatedLcQload = 0;
    UInt32 tmp_tkn_fraction=0;
    /* CA Changes start */
    ULSchedulerStrategyConfig *ulSchdConfig_p = & cellSpecificParams_g.\
                                                cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig;
    /* CA Changes end */
    /*Calculate the PDB priority*/
    ULLogicalChannelInfo *lcInfo_p = &ueULContext_p->lcInfo[lcIndex];

    ULQosInfo *ulQosInfo_p = &lcInfo_p->ulQosInfo;

    ulQosInfo_p->pdbPriority   = 
        ulQosGetPdbPriority(
                getCurrentTickUl(),
                ulQosInfo_p->lcPdbTable,
                ulQosInfo_p->headOfLinePktDelay,
                drxOnDurationDelay /*it will be zero in this case */
                );

    /* UL Qload changes start */
    for (lcCount = 0; lcCount < MAX_LOGICAL_CHANNEL; lcCount++)
    {
        if (INVALID_LCID != ueULContext_p->ulLCGInfo[lcInfo_p->lcGId].lcId[lcCount])
        {
            lcId = ueULContext_p->ulLCGInfo[lcInfo_p->lcGId].lcId[lcCount];
            totalAvailableLcgToken += ueULContext_p->lcInfo[lcId].ulQosInfo.availLcToken;
        }
    }

    if (totalAvailableLcgToken)
    {
        tmp_tkn_fraction = (lcInfo_p->ulQosInfo.availLcToken * 100)/totalAvailableLcgToken;
        calculatedLcQload = (tmp_tkn_fraction * ueULContext_p->ulLCGInfo[lcInfo_p->lcGId].qLoad)/100;
    }
    /* UL Qload changes end */

    ulQosInfo_p->qLoadPriority = 
        ulQosGetQloadPriority(
                ulQosInfo_p->lcQlTable,
                calculatedLcQload
                );

    /* SPR 12344 changes start */
    ueULContext_p->mcsPriority =
        ulQosGetMcsPriority (ueULContext_p->mcsIndex);
    /* SPR 12344 changes end */

    ulQosInfo_p->ulQosStrategyLCNode_p->totLCWght = 
        (ulQosInfo_p->pdbPriority * 
         ulSchdConfig_p->ulQosStrategyWeights.pdbWeight ) 
        +
        (ulQosInfo_p->qciPriority * 
         ulSchdConfig_p->ulQosStrategyWeights.qciWeight ) 
        +
        (ulQosInfo_p->tokenPriority * 
         ulSchdConfig_p->ulQosStrategyWeights.tokenWeight ) 
        +
        (ulQosInfo_p->qLoadPriority * 
         ulSchdConfig_p->ulQosStrategyWeights.qLoadWeight ) 
        +
        (ueULContext_p->mcsPriority * 
         ulSchdConfig_p->ulQosStrategyWeights.mcsWeight )  ;
     /* SPR 23402 Changes Start */
     /* MOCN Changes Start */
     if (mocnOperatorParams_g.isMocnEnabled)
     {
         ulQosInfo_p->ulQosStrategyLCNode_p->totLCWght += 
              mocnOperatorParams_g.ulPrbUsageDRBPriorityFactor[ueULContext_p->operatorId];
     } 
     /* MOCN Changes End */
     /* SPR 23402 Changes End */
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOTAL_LC_WIEGHT(
            ueULContext_p->ueIndex,lcIndex,
            ulQosInfo_p->ulQosStrategyLCNode_p->totLCWght);
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOKEN_PRIORITY(
            ueULContext_p->ueIndex,lcIndex,
            ulQosInfo_p->tokenPriority);
    /*NEW_LOG_REQ*/
    LOG_MAC_MSG(MAC_UL_QOS_UL_METRIC_CALC_PER_USER,LOGBRIEF,
            MAC_UL_Strategy,getCurrentTick(), ueULContext_p->ueIndex,
            lcIndex,ulQosInfo_p->ulQosStrategyLCNode_p->totLCWght, DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE, __func__,"Total LC wait for the UE per LC");


    /*Insert LC statergic node in the Priortised LC Tree*/
    /* SPR_3244_Fix */
    if(FALSE == ulQosInfo_p->ulQosStrategyLCNode_p->isLCNodePresent)
    {
        if (ueULContext_p->lcInfo[lcIndex].isSpsLc  && 
                ( (ueULContext_p->spsUlInfo.spsState == SPS_ACTIVATION_INITIATED) || 
                  (ueULContext_p->spsUlInfo.spsState == SPS_ACTIVATED) ) )
        {
            return;
        } 
        /* SPR 9203 Fix Starts*/
        if (ueULContext_p->prioritySchedulingReqUL == LTE_TRUE)
        {
            ulQosInfo_p->ulQosStrategyLCNode_p->totLCWght = 
                ueULContext_p->ulLCGInfo[ueULContext_p->lcInfo[1].lcGId].srbWeight;
            /* SPR 23402 Changes Start */ 
            /* MOCN Changes Start */
            if (mocnOperatorParams_g.isMocnEnabled)
            {
                ulQosInfo_p->ulQosStrategyLCNode_p->totLCWght += 
                    mocnOperatorParams_g.prbUsageSRBPriorityFactor;
            }
            /* MOCN Changes End */
            /* SPR 23402 Changes End */
        }
        /* SPR 9203 Fix Ends*/
        ulQosPutEntryInPrioritisedLcTree (
                ulQosInfo_p->ulQosStrategyLCNode_p,
                /*CA Changes start  */
                internalCellIndex);
        /*CA Changes end  */
        ulQosInfo_p->ulQosStrategyLCNode_p->isLCNodePresent = TRUE ;
    }
    /* SPR_3244_Fix */


}


/*****************************************************************************
 * Function Name  : ulQosCalcNgbrLCWeightAndInsert
 * Inputs         : ueULContext_p - pointer to UL UE context Info,
 *                  drxOnDurationDelay - Delay duration when UE is in DRX on 
 *                                       state,
 *					internalCellIndex
 * Outputs        : Calculates aggregate Non-GBR weight of the LC  
 * Returns        : None
 * Description    : It calculates aggregate Non-GBR weight of an LC by formula
 *                  AggWeight = Pqci * Wqci + Ppdb * Wpdb + Ptkn * Wtkn + 
 *                  Pql * Wql + Pmcs * Wmcs
 *                  and inserts the node in UL Priortised LC Tree.
 *****************************************************************************/
void ulQosCalcNgbrLCWeightAndInsert(
        ULUEContext *ueULContext_p,
        UInt16 drxOnDurationDelay,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs)
        /*CA Changes end  */
{

    UInt8 lcId = 0;
    UInt8 lcCount = 0;
    UInt32 totalAvailableLcgToken = 0;
    UInt32 calculatedLcQload = 0;
    UInt32 tmp_tkn_fraction=0;
    /* CA Changes start */
    ULSchedulerStrategyConfig *ulSchdConfig_p = & cellSpecificParams_g.\
                                                cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig;
    /* CA Changes end */

    /*Calculate the PDB priority*/
    ULAmbrInfo *ulAmbrInfo_p = &ueULContext_p->ulAmbrInfo;

    ulAmbrInfo_p->pdbPriority   = 
        ulQosGetPdbPriority(
                execArgs->globalTick,
                ulAmbrInfo_p->lcPdbTable,
                ulAmbrInfo_p->headOfLinePktDelay,
                drxOnDurationDelay /*it will be zero in this case */
                );

    /* UL Qload changes start */
    for (lcCount = 0; lcCount < MAX_LOGICAL_CHANNEL; lcCount++)
    {
        if (INVALID_LCID != ueULContext_p->ulLCGInfo[ulAmbrInfo_p->lcgId].lcId[lcCount])
        {
            lcId = ueULContext_p->ulLCGInfo[ulAmbrInfo_p->lcgId].lcId[lcCount];
            totalAvailableLcgToken += ueULContext_p->lcInfo[lcId].ulQosInfo.availLcToken;
        }
    }

    /* Add AMBR token also along with GBR tokens */
    totalAvailableLcgToken += ulAmbrInfo_p->availAmbrToken;

    if (totalAvailableLcgToken)
    {
        tmp_tkn_fraction = (ulAmbrInfo_p->availAmbrToken * 100)/totalAvailableLcgToken;
        calculatedLcQload = (tmp_tkn_fraction * ueULContext_p->ulLCGInfo[ulAmbrInfo_p->lcgId].qLoad)/100;
    }
    /* UL Qload changes end */

    ulAmbrInfo_p->qLoadPriority = 
        ulQosGetQloadPriority(
                ulAmbrInfo_p->lcQlTable,
                calculatedLcQload
                );
    /* SPR 12344 changes start */
    ueULContext_p->mcsPriority =
        ulQosGetMcsPriority (ueULContext_p->mcsIndex);
    /* SPR 12344 changes end */

    /* SPR 12344 changes start */
    ueULContext_p->mcsPriority =
        ulQosGetMcsPriority (ueULContext_p->mcsIndex);
    /* SPR 12344 changes end */

    ulAmbrInfo_p->ulQosStrategyLCNode_p->totLCWght = 
        (ulAmbrInfo_p->pdbPriority * ulSchdConfig_p->
         ulQosStrategyWeights.pdbWeight ) 
        +
        (ulAmbrInfo_p->qciPriority * ulSchdConfig_p->
         ulQosStrategyWeights.qciWeight ) 
        +
        (ulAmbrInfo_p->ambrTokenPriority * ulSchdConfig_p->
         ulQosStrategyWeights.tokenWeight ) 
        +
        (ulAmbrInfo_p->qLoadPriority * ulSchdConfig_p->
         ulQosStrategyWeights.qLoadWeight ) 
        +
        (ueULContext_p->mcsPriority * ulSchdConfig_p->
         ulQosStrategyWeights.mcsWeight );

     /* SPR 23402 Changes Start */
     /* MOCN Changes Start */
     if (mocnOperatorParams_g.isMocnEnabled)
     {
         ulAmbrInfo_p->ulQosStrategyLCNode_p->totLCWght += 
              mocnOperatorParams_g.ulPrbUsageDRBPriorityFactor[ueULContext_p->operatorId];
     }
     /* MOCN Changes End */
     /* SPR 23402 Changes End */

    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOTAL_LC_WIEGHT(
            ueULContext_p->ueIndex,ulAmbrInfo_p->lcId,
            ulAmbrInfo_p->ulQosStrategyLCNode_p->totLCWght);
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOKEN_PRIORITY(
            ueULContext_p->ueIndex,ulAmbrInfo_p->lcId,
            ulAmbrInfo_p->ambrTokenPriority);

    /* SPR 9203 Fix Starts*/
    if (ueULContext_p->prioritySchedulingReqUL == LTE_TRUE)
    {
        ulAmbrInfo_p->ulQosStrategyLCNode_p->totLCWght = 
            ueULContext_p->ulLCGInfo[ueULContext_p->lcInfo[1].lcGId].srbWeight;
       /* SPR 23402 Changes Start */
       /* MOCN Changes Start */
       if (mocnOperatorParams_g.isMocnEnabled)
       {
           ulAmbrInfo_p->ulQosStrategyLCNode_p->totLCWght += 
              mocnOperatorParams_g.prbUsageSRBPriorityFactor;
       }
       /* MOCN Changes End */
       /* SPR 23402 Changes End */
    } 
    /* SPR 9203 Fix Ends*/


    /*NEW_LOG_REQ*/
    LOG_MAC_MSG(MAC_UL_QOS_UL_METRIC_CALC_PER_USER,LOGBRIEF,
            MAC_UL_Strategy,getCurrentTick(), ueULContext_p->ueIndex,
            lcId,ulAmbrInfo_p->ulQosStrategyLCNode_p->totLCWght, DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE, __func__,"LC Wait Per LC");

    /*Insert LC statergic node in the Priortised LC Tree*/
    /* SPR_3244_Fix  */
    if(FALSE == ulAmbrInfo_p->isLCNodePresent)
    {
        ulQosPutEntryInPrioritisedLcTree (
                ulAmbrInfo_p->ulQosStrategyLCNode_p,
                /*CA Changes start  */
                internalCellIndex);
        /*CA Changes end  */
        ulAmbrInfo_p->isLCNodePresent = TRUE;
    }
    /* SPR_3244_Fix  */

}

/*****************************************************************************
 * Function Name  : ulQosPutEntryInNonZeroBSRList 
 * Inputs         : ueULContext_p - Pointer to the UL UE Context,
 *                  lcgId - Logical channel ID,
 *					internalCellIndex
 * Outputs        : Add UE in Non Zero List and update the pointer in 
 *                  UE Context
 * Returns        : None
 * Description    : It inserts the ueIndex provided as argument into 
 *                  nonZeroBsrList.
 *****************************************************************************/
void ulQosPutEntryInNonZeroBSRList(
        ULUEContext* ueULContext_p,
        UInt8 lcgId,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
    /*UL Force Grant Changes Start */
        ExecutionLegFunctionArg* execArgs, UInt8 *forceGrantFlag_p )
{
    ULQosInfo *qosInfo_p = PNULL;
    UInt8  deleteUEFlag = FALSE;                         
    UInt8 lcIndex = 1;
    UInt8 lcId = 0;
    UInt16 forceGrantSize = cellSpecificParams_g.
        cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->sizeEUL;
    /*UL Force Grant Changes End */

    UInt8 counter = ueULContext_p->ulLCGInfo[lcgId].numLC;

    for (;lcIndex < MAX_LOGICAL_CHANNEL && counter;)
    {
        lcId = ueULContext_p->ulLCGInfo[lcgId].lcId[lcIndex];
        if(lcId != INVALID_LCID)
        {
            --counter;

            /*! \fn MacRetType ulQosPutEntryInNonZeroBSRList(ULUEContext* ueULContext_p,
             *                               UInt8 lcgId)
             *  \brief Function Modification
             * */
            if((TRUE == ueULContext_p->lcInfo[lcId].isSpsLc)&&
                    (SPS_SETUP == ueULContext_p->spsUlInfo.requestType) )
            {
                /*! \code
                 * If LC is an SPS LC
                 *    Call ulProcessSpsUeForActDeact()
                 *    If UE State is SPS Activated, do not add LC into the tree
                 * \endcode
                 * */
                ulProcessSpsUeForActDeact(ueULContext_p, 
                        lcId,
                                          /*CA Changes start  */
                        internalCellIndex);
                                          /*CA Changes end  */
                if ( (SPS_ACTIVATED == ueULContext_p->spsUlInfo.spsState) ||
                        (SPS_ACTIVATION_INITIATED == ueULContext_p->spsUlInfo.spsState))
                {
                    continue;
                }
            }
            /*UL Force Grant Changes Start */
            /* Add the tokens in case of force grant */
            if (!(ueULContext_p->bsrNetTotal))
            {
                checkAndProvideForceGrant( 
                        &deleteUEFlag, ueULContext_p, 
                        internalCellIndex );	
                /* This flag differentiates the scenario of force grant from
                 * normal scenario */
                *forceGrantFlag_p = LTE_TRUE;
            }
            /*UL Force Grant Changes End */

            if(QOS_GBR == ueULContext_p->lcInfo[lcId].ulQosInfo.channelType)
            {
                /*UL Force Grant Changes Start */
                qosInfo_p =&ueULContext_p->lcInfo[lcId].ulQosInfo;
                if ( ( LTE_TRUE == *forceGrantFlag_p ) && 
                        ( FALSE == deleteUEFlag ) )
                {
                    qosInfo_p->availLcToken += forceGrantSize;
                    ueULContext_p->ulLCGInfo[lcgId].qLoad += forceGrantSize;
                }
                 /*UL Force Grant Changes End */
                /* SPR# 3147 Changes Starts */
                if (FALSE ==  ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isPresentInEligibleList)
                {
                    /* SPR# 3147 Changes Ends */
                    /*UL Force Grant Changes Start */
                    /*Code Removed */
                     /*UL Force Grant Changes End */

                    /* Add the node in the Eligible List */
                    listInsertNode(&(ueULContext_p->ulEligibleLcList),
                            &(qosInfo_p->ulQosStrategyLCNode_p->eligibleLcNodeAnchor));

                    /* CA Stats Changes Start */
                    /*debug stats*/
                    LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_ELIGIBLE_LC_COUNT(
                            internalCellIndex);
                    /* CA Stats Changes End */
                    /*debug stats*/
                    LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(
                            ueULContext_p->ueIndex);

                    /* SPR_3244_Fix */
                    if(UL_QOS_NO_TRANSMISSION < ueULContext_p->scheduleFlag)
                    {
                        /*debug stats*/
                        LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(
                                ueULContext_p->ueIndex);

                        /* calculate the weight of the LC and insert it in the 
                         * priortised LC list 
                         */
                        /*Update Head of line packet delay*/
                        qosInfo_p->headOfLinePktDelay = 
                            execArgs->globalTick;

                        /* CA Stats Changes Start */
                        /*debug stats*/
                        LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_HEAD_OF_LINE_DLY(
                                qosInfo_p->headOfLinePktDelay, internalCellIndex);
                        /* CA Stats Changes End */

                        /* 2. Calculate Priority*/
                        qosInfo_p->tokenPriority
                            = ulQosGetTknPriority(
                                    qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd],
                                    qosInfo_p->lcTknTable,
                                    qosInfo_p->totSchdTknInWindow);


                        /*Calculate the aggregate weight of the LC*/
                        /*CA Changes start  */
                        ulQosCalcGbrLCWeightAndInsert(ueULContext_p,lcId,0,internalCellIndex);
                        /*CA Changes end  */
                    }
                    /* SPR_3244_Fix */

                    qosInfo_p->ulQosStrategyLCNode_p->isPresentInEligibleList = TRUE;

                }
            }
            /*UL Force Grant Changes Start */
            else if( (PNULL != ueULContext_p->ulAmbrInfo.ulQosStrategyLCNode_p) && 
                    (NON_GBR == ueULContext_p->lcInfo[lcId].ulQosInfo.channelType))
                    {
                ULAmbrInfo *ulAmbrInfo_p = &( ueULContext_p->ulAmbrInfo );
                if ( ( LTE_TRUE == *forceGrantFlag_p ) && 
                        ( FALSE == deleteUEFlag ) )
                {
                    ulAmbrInfo_p->availAmbrToken += forceGrantSize;
                    /* adjusting credit tokens for AMBR */
                    if((UInt32)ulAmbrInfo_p->availAmbrToken > ulAmbrInfo_p->ambrTokenDepth)
                    {
                        ulAmbrInfo_p->availAmbrToken = ulAmbrInfo_p->ambrTokenDepth;
                    }
                    else if((UInt32)ulAmbrInfo_p->availAmbrToken <= ulAmbrInfo_p->ambrTokenLimit)
                    {
                        ulAmbrInfo_p->availAmbrToken = ulAmbrInfo_p->ambrTokenLimit;
                    }
                    ueULContext_p->ulLCGInfo[lcgId].qLoad += forceGrantSize;
                }

                if (FALSE == ulAmbrInfo_p->isPresentInEligibleList)
                {
                    /*UL Force Grant Changes End */
                    /*For Ngbr*/
                    /* Add the node in the Eligible List */
                    listInsertNode(&(ueULContext_p->ulEligibleLcList),
                            &(ueULContext_p->ulAmbrInfo.
                                ulQosStrategyLCNode_p->eligibleLcNodeAnchor));

                    /* CA Stats Changes Start */
                    /*debug stats*/
                    LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_ELIGIBLE_LC_COUNT(
                            internalCellIndex);
                    /* CA Stats Changes End */

                    /*debug stats*/
                    LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(
                            ueULContext_p->ueIndex);
                    /* SPR_3244_Fix */
                    if(UL_QOS_NO_TRANSMISSION < ueULContext_p->scheduleFlag)
                    {

                        /*debug stats*/
                        LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(
                                ueULContext_p->ueIndex);
                        /*Update Head of line packet delay*/
                        ueULContext_p->ulAmbrInfo.headOfLinePktDelay = 
                            execArgs->globalTick;

                        /* CA Stats Changes Start */
                        /*debug stats*/
                        LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_HEAD_OF_LINE_DLY(
                                ueULContext_p->ulAmbrInfo.headOfLinePktDelay, 
                                internalCellIndex);
                        /* CA Stats Changes End */

                        /*CA Changes start  */
                        ulQosCalcNgbrLCWeightAndInsert(ueULContext_p,0,internalCellIndex,execArgs);
                        /*CA Changes end  */
                    }
                    /* SPR_3244_Fix */

                    ueULContext_p->ulAmbrInfo.isPresentInEligibleList = TRUE;
                }

            } 
            else
            {
                /* In case of SRBs, available tokens must be equal to the 
                 * qload size*/
                if (FALSE == ueULContext_p->ulLCGInfo[lcgId].isDrbPresent && (1 == lcId))
                {
                    ueULContext_p->lcInfo[lcId].ulQosInfo.availLcToken = 
                        ueULContext_p->ulLCGInfo[lcgId].qLoad;
                    /* SPR# 3642 Changes Starts*/ 
                    if(FALSE == ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isPresentInEligibleList)
                    {
                        /* SPR# 3642 Changes Ends*/ 

                        /* Add the node in the Eligible List */
                        listInsertNode(&(ueULContext_p->ulEligibleLcList),
                                &(ueULContext_p->lcInfo[lcId].ulQosInfo.
                                    ulQosStrategyLCNode_p->eligibleLcNodeAnchor));

                        /* CA Stats Changes Start */
                        /*debug stats*/
                        LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_ELIGIBLE_LC_COUNT(
                                internalCellIndex);
                        /* CA Stats Changes End */

                        /*debug stats*/
                        LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(
                                ueULContext_p->ueIndex);

                        /* SPR_3244_Fix */
                        if(UL_QOS_NO_TRANSMISSION < ueULContext_p->scheduleFlag)
                        {
                            /*debug stats*/
                            LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(
                                    ueULContext_p->ueIndex);
                            /*Update Head of line packet delay*/
                            ueULContext_p->lcInfo[lcId].ulQosInfo.headOfLinePktDelay = 
                                execArgs->globalTick;

                            ueULContext_p->lcInfo[lcId].ulQosInfo.
                                ulQosStrategyLCNode_p->totLCWght = 
                                ueULContext_p->
                                ulLCGInfo[ueULContext_p->lcInfo[lcId].lcGId].srbWeight;
                            /* Insert it in the priortised LC list 
                            */
                            /* SPR_3244_Fix  */
                            if( FALSE == ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isLCNodePresent)
                            {
                                ulQosPutEntryInPrioritisedLcTree (
                                        ueULContext_p->lcInfo[lcId].ulQosInfo.
                                        ulQosStrategyLCNode_p,
                                        /*CA Changes start  */
                                        internalCellIndex);
                                /*CA Changes end  */
                                ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isLCNodePresent = TRUE;
                            }
                            /* SPR_3244_Fix */
                        }
                        ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isPresentInEligibleList = TRUE;
                    }
                }
                /* SPR_3244_Fix  */
            }
        }
        ++lcIndex;    
    }
}




/*****************************************************************************
 * Function Name  : ulQosPutEntryInPrioritisedLcTree
 * Inputs         : ulStrategyLcNode_p - Pointer to ULQosStrategyLCNode,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function inserts the strategic LC node in priortised 
 *                  LC Tree.
 *****************************************************************************/
 void ulQosPutEntryInPrioritisedLcTree(
        ULQosStrategyLCNode *ulStrategyLcNode_p,        
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
        /*CA Changes end  */
{
    /* EICIC +*/
    ULUEContext *ulUEContext_p = PNULL;
    ULUEContextInfo *ulUEContextInfo_p = PNULL;


    ulUEContextInfo_p = &ulUECtxInfoArr_g[ulStrategyLcNode_p->ueId];
    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;      


    /*Inserting UL strategic node in priortised LC Tree */
    ulQosInsertLcNode(
            /*CA Changes start  */
            ulQosStrategyLCTree_gp[internalCellIndex][ulUEContext_p->userType],
            /*CA Changes end  */
            &ulStrategyLcNode_p->lcTreeNodeAnchor);

    /* EICIC -*/
}

/*****************************************************************************
 * Function Name  : ulQosDeleteEntryFromPrioritisedLcTree
 * Inputs         : ulStrategyLcNode_p - Pointer to ULQosStrategyLCNode,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function deleted the strategic LC node from priortised 
 *                  LC Tree.
 *****************************************************************************/
 void ulQosDeleteEntryFromPrioritisedLcTree(
        ULQosStrategyLCNode *ulStrategyLcNode_p,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex)
        /*CA Changes end  */
{
    /* EICIC +*/
    ULUEContext *ulUEContext_p = PNULL;
    ULUEContextInfo *ulUEContextInfo_p = PNULL;


    ulUEContextInfo_p = &ulUECtxInfoArr_g[ulStrategyLcNode_p->ueId];
    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;      

    /*Deleting UL strategic node in priortised LC Tree */
    ulQosDeleteLcNode(
            /*CA Changes start  */
            ulQosStrategyLCTree_gp[internalCellIndex][ulUEContext_p->userType],
            /*CA Changes end  */
            &ulStrategyLcNode_p->lcTreeNodeAnchor);


    /* EICIC -*/
}

/*****************************************************************************
 * Function Name  : ulQosInitLcTree
 * Inputs         : tree_p - pointer to UL Priortised LC Tree,
 *                  compare_p - pointer to compare function for Priortised LC
 *                              Tree,
 *                  keyOf_p - pointer to key function
 * Outputs        : None 
 * Returns        : None
 * Description    : This function initialises the UL priortised LC Tree.
 *****************************************************************************/

 void ulQosInitLcTree (
        /* Pointer to Tree */
        LTE_TREE *tree_p, 
        /* Pointer to compare function :
         * it returns 0 if A==B, 1 if A<B and -1 if A>B
         */
        SInt32 (*compare_p)(const void *, const void *),

        /* Pointer to function returning Key
         *        */
        const void *(*keyOf_p)(const LTE_TREE_NODE *))
{
    ytInit(tree_p,compare_p,keyOf_p);
}

/*****************************************************************************
 * Function Name  : ulQosGetFirstLcNode
 * Inputs         : tree_p - pointer to UL Priortised LC Tree
 * Outputs        : Node
 * Returns        : Pointer to first node of UL Priortised LC Tree according 
 *                  to INORDER Traversal
 * Description    : This function returns the pointer to the first node of 
 *                  UL priortised LC Tree.
 *****************************************************************************/
 LTE_TREE_NODE * ulQosGetFirstLcNode (
        /* Pointer to Tree */
        LTE_TREE *tree_p)
{
    return ytFirst (tree_p, T_INORDER);
}


/*****************************************************************************
 * Function Name  : ulQosGetNextLcNode
 * Inputs         : treeNode_p - pointer to UL Priortised LC Tree.
 * Outputs        : Node  
 * Returns        : Pointer to first node of UL Priortised LC Tree according 
 *                  to INORDER Traversal
 * Description    : This function returns the pointer to the first node of 
 *                  UL priortised LC Tree.
 *****************************************************************************/
 LTE_TREE_NODE * ulQosGetNextLcNode (
        /* Pointer to Tree Node*/
        LTE_TREE_NODE *treeNode_p)
{
    return ytNext (treeNode_p, T_INORDER);
}




/*****************************************************************************
 * Function Name  : ulQosDeleteLcNode
 * Inputs         : tree_p - pointer to UL Priortised LC Tree,
 *                  entry_p - pointer to Strategy node to be deleted
 * Outputs        : None  
 * Returns        : None
 * Description    : This function deletes the UL strategic LC node from the 
 *                  priortised LC Tree.
 *****************************************************************************/
 void ulQosDeleteLcNode (
        /* Pointer to Tree */
        LTE_TREE *tree_p,

        /* Pointer to node being inserted */
        LTE_TREE_NODE *entry_p)
{
    if (entry_p != NULL)
    {
        if ( (entry_p->child[0] == NULL) && (entry_p->child[1] == NULL) && (entry_p->parent == NULL) && entry_p->red == 0)
        {
            /* Node has already been freed */
            return;
        }
        ytDelete (tree_p, entry_p);

        /* Reset the tree node */
        entry_p->child[0] = NULL;
        entry_p->child[1] = NULL;
        entry_p->parent = NULL;
        entry_p->red = 0;
    }
}

// ++ Fix for EICIC Qos.
extern __thread InternalCellIndex cellIndexMac_g;
// -- Fix for EICIC Qos.
/*****************************************************************************
* Function Name  : ulQosInsertLcNode
* Inputs         : tree_p - pointer to UL Priortised LC Tree
*                  entry_p - pointer to Strategy node to be inserted
* Outputs        : None  
 * Returns        : None
* Description    : This function inserts the strategic LC node in priortised 
 *                  LC Tree.
*****************************************************************************/
 void ulQosInsertLcNode (
        /* Pointer to Tree */
        LTE_TREE *tree_p,
        /* Pointer to node being inserted */
        LTE_TREE_NODE *entry_p )
{
    // ++ Fix for EICIC Qos.
    if(isEicicEnabled_g[cellIndexMac_g])
    {
        if((tree_p->root.child[0] == 0) && (tree_p->count != 0))
        {
            tree_p->count = 0;
        }
    }
    // -- Fix for EICIC Qos.
    ytInsertWithDuplicate(tree_p, entry_p);
}

/*****************************************************************************
 * Function Name  : ulQosCompareLcNode
 * Inputs         : firstElem_p - Pointer to first element being compared,
 *                  secondElem_p - Pointer to second element being compared
 * Outputs        : None  
 * Returns        : 0 if A == B, 1 if A < B and -1 if A > B
 * Description    : This function compares keys of two strategic LC node in 
 *                  priortised LC Tree.
 *****************************************************************************/

SInt32 ulQosCompareLcNode (
        /* Pointer to first element being compared */
        const void * firstElem_p ,

        /* Pointer to second element being compared */
        const void * secondElem_p
        )
{
    if (firstElem_p == PNULL || secondElem_p == PNULL)
    {
        return ERROR_IN_COMPARE;
    }

    if ( * ((UInt32 *) firstElem_p) == * ((UInt32 *) secondElem_p))
    {
        return FIRST_IS_EQUAL_TO_SECOND;
    }
    else if ( * ((UInt32 *) firstElem_p) < * ((UInt32 *) secondElem_p))
    {
        return FIRST_IS_LESS_THAN_SECOND;
    }
    else /*( * ((UInt32 *) firstElem_p) > * ((UInt32 *) secondElem_p))*/
    {
        return FIRST_IS_GREATER_THAN_SECOND;
    }
}

/*****************************************************************************
 * Function Name  : ulQosKeyOfLcNode
 * Inputs         : node_p - pointer to Node of Tree
 * Outputs        : None  
 * Returns        : pointer to totLCWeight
 * Description    : This function returns pointer to the key if LC node exists 
 *                  in priortised LC Tree.
 *****************************************************************************/



 const void * ulQosKeyOfLcNode (
        /* Pointer to Tree node */
        const YTNODE * node_p
        )
{
    return (node_p) ? &((( ULQosStrategyLCNode *)node_p)->totLCWght) : NULL;
}


/*****************************************************************************
 * Function Name  : ulQosDeleteEntryFromNonZeroBSRList
 * Inputs         : ueULContext_p - pointer to the UL UE context
 *                  lcgId - Logical channel id,
 *					internalCellIndex
 * Outputs        : Delete the UE from the Non Zero BSR List
 * Returns        : None
 * Description    : It deletes the ueIndex provided as argument from 
 *                  nonZeroBsrList.
 *****************************************************************************/
void  ulQosDeleteEntryFromNonZeroBSRList(        
        ULUEContext* ueULContext_p,
        UInt8 lcgId,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
        /*CA Changes end  */
{
    UInt8 lcIndex = 0;
    UInt8 lcId = 0;

    for (;lcIndex < MAX_LOGICAL_CHANNEL;++lcIndex)
    {
        lcId = ueULContext_p->ulLCGInfo[lcgId].lcId[lcIndex];
        if(lcId != INVALID_LCID)
        {

            if(QOS_GBR == ueULContext_p->lcInfo[lcId].ulQosInfo.channelType)
            {  
                /* SPR# 3642 Changes Starts*/
                if (TRUE == ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isPresentInEligibleList)
                {
                    /* SPR# 3642 Changes Ends*/
                    /* SPR 16916 FIX START */
                    /* code removed */
                    /* Now, when the bsrNet becomes zero for the lcg, the activeUECount in 
                     * UL will be decreased for the qci while freeing the Harq,to take the
                     * buffered data into consideration for which Harq transmission is ongoing
                     * as per spec TS 36.314, section 4.1.3.2.
                     */
                    /* SPR 16916 FIX END */
                    /* Delete the node from the Eligible List */
                    listDeleteNode(&(ueULContext_p->ulEligibleLcList),
                            &(ueULContext_p->lcInfo[lcId].ulQosInfo.
                                ulQosStrategyLCNode_p->eligibleLcNodeAnchor));

                    /* SPR_3244_Fix */
                    if(TRUE == ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isLCNodePresent)
                    {
                        /*LC of the UE in the prioritized LC List*/
                        ulQosDeleteEntryFromPrioritisedLcTree(
                                ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p,internalCellIndex);
                        ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
                    }
                    /* SPR_3244_Fix */

                    ueULContext_p->lcInfo[lcId].ulQosInfo.
                        ulQosStrategyLCNode_p->isPresentInEligibleList = FALSE;
                    /*debug stats*/
                    LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(
                            ueULContext_p->ueIndex);

                    /*debug stats*/
                    LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(
                            ueULContext_p->ueIndex);
                }
            }
            else if((PNULL != ueULContext_p->ulAmbrInfo.ulQosStrategyLCNode_p) && (NON_GBR == ueULContext_p->lcInfo[lcId].ulQosInfo.channelType))
            {
                if (TRUE == ueULContext_p->ulAmbrInfo.isPresentInEligibleList)
                {
                    /* SPR 10409 fix start */
                    /* SPR 16916 FIX START */
                    /* code removed */
                    /* Now, when the bsrNet becomes zero for the lcg, the activeUECount in 
                     * UL will be decreased for the qci while freeing the Harq,to take the
                     * buffered data into consideration for which Harq transmission is ongoing
                     * as per spec TS 36.314, section 4.1.3.2.
                     */
                    /* SPR 16916 FIX END */
                    /* SPR 10409 fix end */

                    /* Delete the node from the Eligible List */
                    listDeleteNode(&(ueULContext_p->ulEligibleLcList),
                            &(ueULContext_p->ulAmbrInfo.
                                ulQosStrategyLCNode_p->eligibleLcNodeAnchor));
                    /* SPR_3244_Fix */
                    if(TRUE == ueULContext_p->ulAmbrInfo.isLCNodePresent)
                    {
                        /*LC of the UE in the prioritized LC List*/
                        ulQosDeleteEntryFromPrioritisedLcTree(
                                ueULContext_p->ulAmbrInfo.ulQosStrategyLCNode_p,
                                /*CA Changes start  */
                                internalCellIndex);
                        /*CA Changes end  */
                        ueULContext_p->ulAmbrInfo.isLCNodePresent = FALSE ;
                    }
                    /* SPR_3244_Fix */
                    ueULContext_p->ulAmbrInfo.isPresentInEligibleList = FALSE;
                    /*debug stats*/
                    LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(
                            ueULContext_p->ueIndex);
                    /*debug stats*/
                    LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(
                            ueULContext_p->ueIndex);
                }
            }
            else
            { 
                if(FALSE == ueULContext_p->ulLCGInfo[lcgId].isDrbPresent && 1 == lcId) 
                {
                    /* SPR# 3642 Changes Starts*/
                    if(TRUE == ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isPresentInEligibleList)
                    {
                        /* SPR# 3642 Changes Ends*/
                        /* SPR 10409 fix start */
                        /* SPR 16916 FIX START */
                        /* code removed */
                        /* Now, when the bsrNet becomes zero for the lcg, the activeUECount in 
                         * UL will be decreased for the qci while freeing the Harq,to take the
                         * buffered data into consideration for which Harq transmission is ongoing
                         * as per spec TS 36.314, section 4.1.3.2.
                         */

                        /* SPR 16916 FIX END */
                        /* SPR 10409 fix end */
                        /* Delete the node from the Eligible List */
                        listDeleteNode(&(ueULContext_p->ulEligibleLcList),
                                &(ueULContext_p->lcInfo[lcId].ulQosInfo.
                                    ulQosStrategyLCNode_p->eligibleLcNodeAnchor));
                        /* SPR_3244_Fix */
                        if(TRUE == ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isLCNodePresent)
                        {
                            /*LC of the UE in the prioritized LC List*/
                            ulQosDeleteEntryFromPrioritisedLcTree(
                                    ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p,
                                    /*CA Changes start  */
                                    internalCellIndex);
                            /*CA Changes end  */

                            ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
                        }
                        ueULContext_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isPresentInEligibleList = FALSE;
                        /* SPR_3244_Fix */
                        /*debug stats*/
                        LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(
                                ueULContext_p->ueIndex);
                        /*debug stats*/
                        LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(
                                ueULContext_p->ueIndex);
                    }
                }
            }

        }
    }

}
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : ulQosUpdateScheduledTokens
 * Inputs         : ulUEContext_p - Pointer to ULUEContext,
 *                  tbSize,
 *                  tempHarqProcessId,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the token and weight for each UL strategic node
 *                  present in UL priortisd LC Tree.
 *****************************************************************************/
void ulQosUpdateScheduledTokens(ULUEContext *ulUEContext_p, 
        SInt16 tbSize,
        UInt8 tempHarqProcessId,
        /*CA Changes start */
        InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs)
                                /*CA Changes end */
{
    ULQosStrategyLCNode *ulQosStrategyLCNode_p = PNULL;

    void                *ulScheduledLcNodeVoid_p = PNULL; 

    UInt8 lcGId = 0;

    UInt16              allocatedBytes = 0;

    ULHarqInfo*   tempULHarqInfo_p = PNULL; 

    ulScheduledLcNodeVoid_p =(void*)
        listPopNode(&ulUEContext_p->ulScheduledLcList);


    ULQosInfo* qosInfo_p           = PNULL;
    ULAmbrInfo* ulAmbrInfo_p       = PNULL; 

    UInt32 prevBsrLcg = 0;


    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_TB_SIZE(
            ulUEContext_p->ueIndex,tbSize, internalCellIndex);
    /* CA Stats Changes End */

    /* Qos changes_v start*/
    if (tempHarqProcessId !=INVALID_HARQ_ID)
    {    
        tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
        /*QOS_KLOCKWORK_FIX*/
        if(PNULL == tempULHarqInfo_p)
        {
            ltePanic("tempULHarqInfo_p is NULL\n");
        }
        /*QOS_KLOCKWORK_FIX*/
#ifdef UTFWK_SIMULATION 
        tempULHarqInfo_p->deltabsrLoad = tbSize;
#endif 
    }
    /* Qos changes_v end*/

    while(ulScheduledLcNodeVoid_p)
    {
        /*debug stats*/
        LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_SCH_COUNT(
                ulUEContext_p->ueIndex);

        --ulUEContext_p->scheduledLcCount;

        UL_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                ulScheduledLcNodeVoid_p, ulQosStrategyLCNode_p);

        allocatedBytes = (tbSize > ulQosStrategyLCNode_p->scheduledBytes) ?
            ulQosStrategyLCNode_p->scheduledBytes : tbSize;

        lcGId = ulQosStrategyLCNode_p->lcgId;

        if(allocatedBytes)
        {
            /*Rel_523_Coverity_10081 Fix Start*/
            if((PNULL != tempULHarqInfo_p) && (QOS_GBR == ulQosStrategyLCNode_p->channelType))
                /*Rel_523_Coverity_10081 Fix End*/
            {
                /* SPR_3244_Fix */
                if(TRUE ==  ulQosStrategyLCNode_p->isLCNodePresent)
                { 
                    ulQosDeleteEntryFromPrioritisedLcTree(
                            ulQosStrategyLCNode_p,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */

                    ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
                }
                /* SPR_3244_Fix */
                qosInfo_p = &ulQosStrategyLCNode_p->lcInfo_p->ulQosInfo; 
                /* 1. Update Tokens*/

                if(qosInfo_p->availLcToken > allocatedBytes)
                {
                    qosInfo_p->availLcToken -= allocatedBytes;
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD) 
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken = allocatedBytes;
#else
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken += allocatedBytes;
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].lastScheduledToken = allocatedBytes;
#endif
                    /*Qload adjustment changes*/
                    tempULHarqInfo_p->grantPerLcg[lcGId]+= allocatedBytes;
                    /*Qload adjustment changes*/

                }
                else
                {
#if !defined (OVERSHOOT_CORRECTION) &&  !defined (DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken = qosInfo_p->availLcToken;
#else
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken += qosInfo_p->availLcToken;
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].lastScheduledToken = qosInfo_p->availLcToken;
#endif
                    /*Qload adjustment changes*/
                    tempULHarqInfo_p->grantPerLcg[lcGId]+= qosInfo_p->availLcToken;
                    /*Qload adjustment changes*/
                    qosInfo_p->availLcToken = 0;
                }
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD) 
                tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].isLCScheduled = TRUE;
#else
                tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].isLCScheduled++ ;
#endif

                qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd] += allocatedBytes;


                /* 2. Calculate Priority*/
                qosInfo_p->tokenPriority
                    = ulQosGetTknPriority(
                            qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd],
                            qosInfo_p->lcTknTable,
                            qosInfo_p->totSchdTknInWindow
                            );

                /*Update Head of line packet delay*/
                qosInfo_p->headOfLinePktDelay = 
                    execArgs->globalTick;

                /* 3. calculate the weight of the Gbr LC node */
                ulQosCalcGbrLCWeightAndInsert(ulUEContext_p,
                        ulQosStrategyLCNode_p->lchId,
                        /*CA Changes start  */
                        0, internalCellIndex); /* DrxOnDurationDelay */        
                /*CA Changes end  */ 

            }
            /*Rel_523_Coverity_10081 Fix Start*/
            else if((PNULL != tempULHarqInfo_p) && (NON_GBR == ulQosStrategyLCNode_p->channelType))
                /*Rel_523_Coverity_10081 Fix Start*/
            {
                /* All NON-GBR LC.s are considered in a single strategic LC, 
                 * and the token is updated at AMBR rate. */
                /* SPR_3244_Fix */
                if(TRUE ==  ulQosStrategyLCNode_p->ulAmbrInfo_p->isLCNodePresent)
                { 
                    ulQosDeleteEntryFromPrioritisedLcTree(
                            ulQosStrategyLCNode_p,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    ulQosStrategyLCNode_p->ulAmbrInfo_p->isLCNodePresent = FALSE;
                }
                /* SPR_3244_Fix */

                ulAmbrInfo_p = ulQosStrategyLCNode_p->ulAmbrInfo_p;

                /* 1. Update Tokens*/
                if (ulAmbrInfo_p->availAmbrToken > allocatedBytes)
                {
                    ulAmbrInfo_p->availAmbrToken -= allocatedBytes;
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD) 
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken = allocatedBytes;
#else
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken += allocatedBytes;
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].lastScheduledToken = allocatedBytes;
#endif
                    /*Qload adjustment changes*/
                    tempULHarqInfo_p->grantPerLcg[lcGId]+= allocatedBytes;
                    /*Qload adjustment changes*/
                }
                else
                {
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD) 
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken = ulAmbrInfo_p->availAmbrToken;
#else
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken += ulAmbrInfo_p->availAmbrToken;
                    tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].lastScheduledToken = ulAmbrInfo_p->availAmbrToken;
#endif
                    /*Qload adjustment changes*/
                    tempULHarqInfo_p->grantPerLcg[lcGId]+= ulAmbrInfo_p->availAmbrToken;
                    /*Qload adjustment changes*/
                    ulAmbrInfo_p->availAmbrToken = 0;
                }

                ulAmbrInfo_p->scheduledToken[ulAmbrInfo_p->curTknWindowInd] += allocatedBytes;
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD) 
                tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].isLCScheduled = TRUE;
#else
                tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].isLCScheduled++;
#endif

                /*2. Update Head of line packet delay*/
                ulAmbrInfo_p->headOfLinePktDelay = 
                    execArgs->globalTick;


                /* 3.calculate the weight of the Ambr node */
                /*CA Changes start  */
                ulQosCalcNgbrLCWeightAndInsert(ulUEContext_p,0,internalCellIndex,execArgs);
                /*CA Changes end  */

            }
            else
            {
                /*Rel_523_Coverity_10081 Fix Start*/
                if ((PNULL != tempULHarqInfo_p) && (FALSE == ulUEContext_p->
                            ulLCGInfo[ulQosStrategyLCNode_p->lcInfo_p->lcGId].isDrbPresent))
                    /*Rel_523_Coverity_10081 Fix Start*/
                {
                    /* SPR 19234 Fix Start */ 
                    /* SPR_3244_Fix */
                    if(TRUE ==  ulQosStrategyLCNode_p->isLCNodePresent)
                    { 
                        ulQosDeleteEntryFromPrioritisedLcTree(
                                ulQosStrategyLCNode_p,
                                /*CA Changes start  */
                                internalCellIndex);
                        /*CA Changes end  */
                        ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
                    }
                    /* SPR_3244_Fix */
                    /* coverity 59940 +*/
                    /* SPR 19234 Fix End */ 
                    qosInfo_p = &ulQosStrategyLCNode_p->lcInfo_p->ulQosInfo; 

                    if(qosInfo_p->availLcToken > allocatedBytes)
                    {
                        qosInfo_p->availLcToken -= allocatedBytes;
                        /*Qload adjustment changes*/
                        tempULHarqInfo_p->grantPerLcg[lcGId]+= allocatedBytes;
                        /*Qload adjustment changes*/
                    }
                    else
                    {
                        /*Qload adjustment changes*/
                        tempULHarqInfo_p->grantPerLcg[lcGId]+=
                            qosInfo_p->availLcToken;
                        /*Qload adjustment changes*/
                        qosInfo_p->availLcToken = 0;
                    }


                    qosInfo_p->curTknWindowInd += allocatedBytes;

                    qosInfo_p->ulQosStrategyLCNode_p->totLCWght = 
                        ulUEContext_p->
                        ulLCGInfo[ulQosStrategyLCNode_p->lcInfo_p->lcGId].srbWeight;
                    /* SPR 23402 Changes Start */
                     /* MOCN Changes Start */
                    if (mocnOperatorParams_g.isMocnEnabled)
                    {
                        qosInfo_p->ulQosStrategyLCNode_p->totLCWght += 
                        mocnOperatorParams_g.prbUsageSRBPriorityFactor;
                    }
                    /* MOCN Changes End */
                    /* SPR 23402 Changes End */
                    /* Insert it in the priortised LC list 
                    */
                    /* SPR_3244_Fix */
                    if(FALSE == qosInfo_p->ulQosStrategyLCNode_p->isLCNodePresent)
                    {
                        ulQosPutEntryInPrioritisedLcTree (
                                qosInfo_p->ulQosStrategyLCNode_p,
                                /*CA Changes start  */
                                internalCellIndex);
                        /*CA Changes end  */
                        qosInfo_p->ulQosStrategyLCNode_p->isLCNodePresent = TRUE;
                    }
                    /* SPR_3244_Fix */
                    /* coverity 59940 -*/
                }
            }
            /* SPR 10027 FIX START */
            /* Updating the isDrbPresent Flag in Harq To be used in case of reTx 
             * Updating the lastScheduled Tick */
            if ((ulQosStrategyLCNode_p->lchId >= MAC_LCID_3) && 
                    (ulQosStrategyLCNode_p->lchId <= MAC_LCID_10))
            {
                if (PNULL != tempULHarqInfo_p)
                {
                    tempULHarqInfo_p->isDrbPresent = TRUE;
                }
                /* Updating the lastScheduled Tick */
                ulUEContext_p->lastScheduledTick = getCurrentTick();
            }
            /* SPR 10027 FIX END */

            tbSize -= allocatedBytes;

            /*Adjust Qload changes start*/
            prevBsrLcg = ulUEContext_p->bsrNet[lcGId];
            if ( ulUEContext_p->bsrNet[lcGId] >= allocatedBytes)
            { 
                ulUEContext_p->bsrNet[lcGId] -=  allocatedBytes;

                if((0 == ulUEContext_p->bsrNet[lcGId]) && prevBsrLcg)
                {
                    /*BSR has become Zero from nonZero*/

                    /*CA Changes start  */
                    putEntryInBSRQueue(ulUEContext_p->ueIndex,lcGId,0,DELETE_TRIGGER, internalCellIndex);
                    /*CA Changes end  */
                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS

                    /* Enqueue the Ue Id and lcId in the Circular Queue */
                    /* + PERF_CA GBL */
                    /* + SPR 17404 */
                    lteMacUpdateKpiStatsIPThpUlOnEnqueue(lcGId,ulUEContext_p->ulLCGInfo,ulUEContext_p->ueIndex,internalCellIndex);
                    /* - SPR 17404 */
                    /* - PERF_CA GBL */
#endif
                    /* 32.425 Events code changes end*/

                }
            }
            else
            {
                ulUEContext_p->bsrNet[lcGId] = 0;
                if (prevBsrLcg)
                {
                    /*BSR has become Zero from nonZero*/

                    /*CA Changes start  */
                    putEntryInBSRQueue(ulUEContext_p->ueIndex,lcGId,0,DELETE_TRIGGER, internalCellIndex);
                    /*CA Changes end  */
                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS

                    /* Enqueue the Ue Id and lcId in the Circular Queue */
                    /* + PERF_CA GBL */
                    /* + SPR 17404 */
                    lteMacUpdateKpiStatsIPThpUlOnEnqueue(lcGId,ulUEContext_p->ulLCGInfo,ulUEContext_p->ueIndex,internalCellIndex);
                    /* - SPR 17404 */
                    /* - PERF_CA GBL */
#endif
                    /* 32.425 Events code changes end*/

                }

            }
            /*Adjust Qload changes end*/
        }
        ulUEContext_p->ulLCGInfo[lcGId].qLoad +=
            (ulQosStrategyLCNode_p->scheduledBytes - allocatedBytes);


        ulQosStrategyLCNode_p->scheduledBytes = 0;

        ulQosStrategyLCNode_p->isLCScheduled = FALSE;

        ulScheduledLcNodeVoid_p = 
            listPopNode(&ulUEContext_p->ulScheduledLcList);

    }

    ulUEContext_p->ulStrategyTxNode_p = PNULL;

}
#endif


/*****************************************************************************
 * Function Name  : ulQosUpdateTokens
 * Inputs         : ulUEContext_p - Pointer to ULUEContext,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the token and weight for each UL strategic node
 *                  present in UL priortisd LC Tree.
 *****************************************************************************/
/*CA Changes start  */
void ulQosUpdateTokens(ULUEContext *ulUEContext_p, InternalCellIndex internalCellIndex,ExecutionLegFunctionArg* execArgs)
/*CA Changes end  */
{
    ULQosStrategyLCNode *ulQosStrategyLCNode_p  = PNULL;
    void *ulQosStrategyLCNodeVoid_p = PNULL;

    UInt8 lcIndex = 0;

    UInt8 numLC = ulUEContext_p->numLogicalChannels;

#ifdef SSI_DEBUG_STATS
    UInt16 ueIndex = ulUEContext_p->ueIndex;
#endif    

    ULQosInfo *qosInfo_p           = PNULL;
    ULAmbrInfo *ulAmbrInfo_p       = PNULL;

    /* SPR 12793 + 13086 Start */
#ifndef REAL_UE_ENVIORNMENT
    if (((getCurrentTick()) - ulUEContext_p->advlastScheduledTick) >= SR_DROP_PREVENT_TIMER )
    {
        ulUEContext_p->bsrNetTotal = 0;
    }
#endif
    /* SPR 12793 + 13086 End */


    while(lcIndex < MAX_LOGICAL_CHANNEL && numLC )
    {
        /* SPR 21745 Fix + */
        if (INVALID_LCID == ulUEContext_p->lcInfo[lcIndex].lchId)
        {
            lcIndex++;
            continue;
        }
        /* SPR 21745 Fix + */
        qosInfo_p = &ulUEContext_p->lcInfo[lcIndex].ulQosInfo;

        if(qosInfo_p->ulQosStrategyLCNode_p)
        {
            --numLC;
            if(qosInfo_p->channelType == QOS_GBR)
            {

                /* DESIGN REVIEW CHANGES INCORPORATION  START */
                if(qosInfo_p->eTokenUpdateFlag)
                {
                    /* 1. Update Tokens */
                    qosInfo_p->availLcToken += qosInfo_p->tokenRate;
                }
                else
                {
                    if (qosInfo_p->availLcToken > qosInfo_p->tokenRate)
                    {
                        qosInfo_p->availLcToken -= qosInfo_p->tokenRate;
                    }
                    else
                    {
                        qosInfo_p->availLcToken = 0;
                    }
                }

                /* Token adjustment code start */
                /* 
                 * 2. Decrement credit tokens from availTokens if any 
                 *    And adjust credit tokens
                 */
                if (qosInfo_p->availLcToken >= qosInfo_p->creditToken)
                {
                    qosInfo_p->availLcToken -= qosInfo_p->creditToken;
                    qosInfo_p->creditToken = 0;
                }
                else
                {
                    qosInfo_p->creditToken -= qosInfo_p->availLcToken;
                    qosInfo_p->availLcToken = 0;
                }
                /* Token adjustment code end */


                if(qosInfo_p->availLcToken > qosInfo_p->tokenDepth)
                {
                    qosInfo_p->availLcToken = qosInfo_p->tokenDepth;

                    if(!ulUEContext_p->ulLCGInfo
                            [ulUEContext_p->lcInfo[lcIndex].lcGId].qLoad)
                    {
                        qosInfo_p->eTokenUpdateFlag = DECREMENT_TOKENS;

                    }

                    /*debug stats*/
                    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOKEN_OVERFLOW(
                            ueIndex,
                            lcIndex);
                }
                else if(qosInfo_p->availLcToken <= qosInfo_p->tokenLimit)
                {
                    qosInfo_p->availLcToken = qosInfo_p->tokenLimit;

                    if(ulUEContext_p->ulLCGInfo
                            [ulUEContext_p->lcInfo[lcIndex].lcGId].qLoad)
                    {
                        qosInfo_p->eTokenUpdateFlag = INCREMENT_TOKENS;
                    }
                }
                /* DESIGN REVIEW CHANGES INCORPORATION  END */

                qosInfo_p->totSchdTknInWindow +=
                    qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd];

                qosInfo_p->curTknWindowInd=
                    ((qosInfo_p->curTknWindowInd + 1 )% MAX_TKN_SCHD_WINDOW);

                qosInfo_p->totSchdTknInWindow -=
                    qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd];

                /*debug
                 * stats*/

                qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd] = 0;

                /* 2. Calculate Priority*/
                qosInfo_p->tokenPriority
                    = ulQosGetTknPriority(
                            qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd],
                            qosInfo_p->lcTknTable,
                            qosInfo_p->totSchdTknInWindow);
#ifdef UTFWK_SIMULATION
                if ( qosInfo_p->tokenPriority == 10 )
                {
                    gMacStats.pStats->ulPerfStats.gbrTputFactor_0_75++;
                }
                else if ( qosInfo_p->tokenPriority == 8 )
                {
                    gMacStats.pStats->ulPerfStats.gbrTputFactor_75_90++;
                }
                else if ( qosInfo_p->tokenPriority == 5 )
                {
                    gMacStats.pStats->ulPerfStats.gbrTputFactor_90_100++;
                }
                else if ( qosInfo_p->tokenPriority == 2 )
                {
                    gMacStats.pStats->ulPerfStats.gbrTputFactor_100_125++;
                }
                else if ( qosInfo_p->tokenPriority == 0 )
                {
                    gMacStats.pStats->ulPerfStats.gbrTputFactor_125++;
                }
                gMacStats.pStats->ulPerfStats.totalNumberOfSamples++;

#endif 

            }


        }
        lcIndex++;
    }
    /* All NON-GBR LC.s are considered in a single strategic LC, 
     * and the token is updated at AMBR rate. */
    ulAmbrInfo_p = &ulUEContext_p->ulAmbrInfo;






    ulQosStrategyLCNode_p = ulAmbrInfo_p->ulQosStrategyLCNode_p;

    /* Update AMBR Tokens */

    /* DESIGN REVIEW CHANGES INCORPORATION  START */
    if(ulAmbrInfo_p->eTokenUpdateFlag)
    {
        ulAmbrInfo_p->availAmbrToken += ulAmbrInfo_p->ambrTokenRate;
    }
    else
    {
        if (ulAmbrInfo_p->availAmbrToken > (SInt64)ulAmbrInfo_p->ambrTokenRate)
        {
            ulAmbrInfo_p->availAmbrToken -= ulAmbrInfo_p->ambrTokenRate;
        }
        else
        {
            ulAmbrInfo_p->availAmbrToken = 0;
        }
    }

    /* adjusting credit tokens for AMBR */
    if (ulAmbrInfo_p->availAmbrToken >= ulAmbrInfo_p->creditToken)
    {
        ulAmbrInfo_p->availAmbrToken -=  ulAmbrInfo_p->creditToken;
        ulAmbrInfo_p->creditToken = 0;
    }
    else
    {
        ulAmbrInfo_p->creditToken -= ulAmbrInfo_p->availAmbrToken;
        ulAmbrInfo_p->availAmbrToken = 0;
    }
    /* adjusting credit tokens for AMBR */

    if(ulAmbrInfo_p->availAmbrToken > (SInt64)ulAmbrInfo_p->ambrTokenDepth)
    {
        ulAmbrInfo_p->availAmbrToken = ulAmbrInfo_p->ambrTokenDepth;

        if(!ulUEContext_p->ulAmbrInfo.isPresentInEligibleList)
        {
            ulAmbrInfo_p->eTokenUpdateFlag = DECREMENT_TOKENS;
        }

        /*debug stats*/
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_AMBR_TOKEN_OVERFLOW(
                ueIndex);
    }
    else if(ulAmbrInfo_p->availAmbrToken <= (SInt64)ulAmbrInfo_p->ambrTokenLimit)
    {
        ulAmbrInfo_p->availAmbrToken = ulAmbrInfo_p->ambrTokenLimit;

        if(ulUEContext_p->ulAmbrInfo.isPresentInEligibleList)
        {
            ulAmbrInfo_p->eTokenUpdateFlag = INCREMENT_TOKENS;
        }
    }
    /* DESIGN REVIEW CHANGES INCORPORATION  END */


    ulAmbrInfo_p->totSchdTknInWindow +=
        ulAmbrInfo_p->scheduledToken[ulAmbrInfo_p->curTknWindowInd];


    ulAmbrInfo_p->curTknWindowInd =
        ((ulAmbrInfo_p->curTknWindowInd + 1 )% MAX_TKN_SCHD_WINDOW);

    ulAmbrInfo_p->totSchdTknInWindow -=
        ulAmbrInfo_p->scheduledToken[ulAmbrInfo_p->curTknWindowInd];

    ulAmbrInfo_p->scheduledToken[ulAmbrInfo_p->curTknWindowInd] = 0;

    /* 2. Calculate Priority*/
    if (UL_QOS_NEW_TRANSMISSION == ulUEContext_p->scheduleFlag)
    {
        ulQosStrategyLCNodeVoid_p =  (void *)
            getFirstListNode(&(ulUEContext_p->ulEligibleLcList));

        while(ulQosStrategyLCNodeVoid_p)
        {
            UL_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(ulQosStrategyLCNodeVoid_p,
                    ulQosStrategyLCNode_p);

            if(ulQosStrategyLCNode_p->channelType == QOS_GBR )
            {
                /* SPR_3244_Fix */
                if(TRUE ==  ulQosStrategyLCNode_p->isLCNodePresent)
                {

                    ulQosDeleteEntryFromPrioritisedLcTree(ulQosStrategyLCNode_p,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    ulQosStrategyLCNode_p->isLCNodePresent = FALSE;

                    ulQosCalcGbrLCWeightAndInsert(ulUEContext_p,
                            ulQosStrategyLCNode_p->lchId,
                            /*CA Changes start  */
                            ZERO_DRX_DELAY, internalCellIndex); /* DrxOnDurationDelay */        
                    /*CA Changes end  */
                }
                /* SPR_3244_Fix */

            }
            else if(ulQosStrategyLCNode_p->channelType == NON_GBR)
            {
                /* SPR_3244_Fix */
                if(TRUE ==  ulUEContext_p->ulAmbrInfo.isLCNodePresent)
                { 
                    ulQosDeleteEntryFromPrioritisedLcTree(ulQosStrategyLCNode_p,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    ulUEContext_p->ulAmbrInfo.isLCNodePresent = FALSE;
                    /* 3.calculate the weight of the Ambr node */
                    /*CA Changes start  */
                    ulQosCalcNgbrLCWeightAndInsert(ulUEContext_p,0,internalCellIndex,execArgs);
                    /*CA Changes end  */
                }

                /* SPR_3244_Fix */
            }


            ulQosStrategyLCNodeVoid_p = (void *)
                getNextListNode(&ulQosStrategyLCNode_p->eligibleLcNodeAnchor);
        }
    }

}
/*****************************************************************************
 * Function Name  : ulQosCalcGbrLCTknValue
 * Inputs         : qosInfo_p - pointer to ULQosInfo,
 *					internalCellIndex
 * Outputs        : Updates the Qos Info with token rate
 * Returns        : None
 * Description    : It updates the token rate for a GBR LC
 *                  This function will be called when a LC is created 
 *                  to update.
 *****************************************************************************/


 void ulQosCalcGbrLCTknValue (ULQosInfo *qosInfo_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    /*Calculate the token rate */

    /* The token rate(Token Value) = (GBR /  8) * TKN_UPDATE_PREIOD; 
     * as token is updated at a granularity of BYTES value 8 is used.
     */

    qosInfo_p->tokenRate = (qosInfo_p->GBR * 
            /*CA Changes start  */
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            initParams_p->ulSchdConfig.ulTokenPeriodicity) / NUM_TICKS_PER_SECOND; 
    /*CA Changes end  */

}

/*****************************************************************************
 * Function Name  : ulQosCalcNonGbrLCTknValue
 * Inputs         : ulAmbrInfo_p - pointer to ULAmbrInfo,
 *					internalCellIndex
 * Outputs        : Updates the LC Info with token rate.
 * Returns        : None
 * Description    : It updates the token rate for a Non-GBR LC.
 *                  This function will be called during the below mentioned 
 *                  scenarios:
 *                      a. when AMBR gets changed.
 *                      b. when AMBR token is updated at every token update 
 *                         periodicity.
 *                      c. When the LC gets scheduled.
 *****************************************************************************/

 void ulQosCalcNonGbrLCTknValue (ULAmbrInfo *ulAmbrInfo_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    /* Since all NON-GBR are mapped to same LCG, a single LC strategic node 
     * is maintained. Calculate the token value from AMBR and store it in LC 
     * node ment for NON-GBR LC.s. The token rate is calculated for AMBR 
     * itself.*/

    ulAmbrInfo_p->ambrTokenRate = ((ulAmbrInfo_p->AMBR) * 
            /*CA Changes start  */
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
            ulSchdConfig.ulTokenPeriodicity) / NUM_TICKS_PER_SECOND; 
    /*CA Changes end  */

}

/*****************************************************************************
 * Function Name  : ulQosGetQloadPriority
 * Inputs         : lcQlTable_p - pointer to lc Qload table
 *                  qLoad
 * Outputs        : None
 * Returns        : Qload priority
 * Description    : It maps the qload with its priority.
 *****************************************************************************/
 UInt16 ulQosGetQloadPriority(UInt32 *lcQlTable_p, 
        UInt32 qLoad )
{
    UInt8 index = 0;
    while(qLoad > lcQlTable_p[index])
    {
        index++;
        /* SPR 20257 fix start */ 
        if((MAX_QLOAD_PRIORITIES - 1) == index)
        {
            return ulLcQloadPriMap_g[index]; 
        }
        /* SPR 20257 fix end */ 
    }
    return ulLcQloadPriMap_g[index];
}

/*****************************************************************************
 * Function Name  : ulQosGetPdbPriority
 * Inputs         : currentTick,
 *                  lcPdbTable_p - pointer to lc Qload table,
 *                  pktArrivalTS,
 *                  drxOnDurationDelay
 * Outputs        : None
 * Returns        : PDB priority
 * Description    : It maps the PDB with its priority.
 *****************************************************************************/
/* SPR 15909 fix start */
 UInt16 ulQosGetPdbPriority(tickType_t currentTick, 
        UInt32 *lcPdbTable_p, 
        tickType_t pktArrivalTS, 
        /* SPR 15909 fix end */
        UInt16 drxOnDurationDelay)
{
    UInt8 index = 0;
    UInt32 headOfLinePkDly = currentTick - pktArrivalTS + drxOnDurationDelay;

    while(headOfLinePkDly > lcPdbTable_p[index])
    {
        index++;
        /* SPR 20257 fix start */ 
        if((MAX_PDB_PRIORITIES - 1) == index)
        {
            return ulLcPdbPriMap_g[index];
        }
        /* SPR 20257 fix end */ 
    }
    return ulLcPdbPriMap_g[index];
}

/*****************************************************************************
 * Function Name  : ulQosGetTknPriority
 * Inputs         : currScheduledTkn - scheduled token for current Tick,
 *                  lcTknTable_p     - pointer to LC Token Table,
 *                  totSchdTkn_p     - Pointer to sum of scheduled tokens in 
 *                                     a Token Window.
 * Outputs        : None
 * Returns        : Token priority
 * Description    : It maps the token with its priority.
 *****************************************************************************/

 UInt16 ulQosGetTknPriority(UInt32   currScheduledTkn, /* Stkn(t) */
        UInt32 *lcTknTable_p,
        UInt32  totSchdTkn /*Ttkn(t  - 1) - Stkn(t - N)*/)
{
    UInt8 index = 0;
    UInt32 newTotSchdTkn = 0; /* Ttkn(t) */

    /*Ttkn(t)     = Ttkn(t  - 1) - Stkn(t - N) +   Stkn(t)*/

    newTotSchdTkn =         totSchdTkn         + currScheduledTkn;

    /* Where , Ttkn(t )     : Sum of the token scheduled for last 
     *                        N TOKEN_UPDATE_PERIOD.s
     Ttkn(t  - 1) : Ttkn(t ) of the previous tick
     Stkn(t - N)  : Tokens scheduled at (t-N)th TOKEN_UPDATE_PERIOD
     Stkn(t )     : Tokens scheduled at (t)th (current) 
     TOKEN_UPDATE_PERIOD
     */

    while(newTotSchdTkn > lcTknTable_p[index])
    {
        index++;
        /* SPR 20257 fix start */ 
        if((MAX_TOKEN_PRIORITIES - 1) == index)
        {
            return ulLcTknPriMap_g[index];
        }
        /* SPR 20257 fix end */ 
    }
    return ulLcTknPriMap_g[index];
}

/*****************************************************************************
 * Function Name  : ulQosGetQciPriority
 * Inputs         : qci - qciConfigPriority
 * Outputs        : None
 * Returns        : QCI priority
 * Description    : It maps the configured QCI priority to the internal used qciPriority.
 *****************************************************************************/
/* SPR 11081 changes start */
 UInt16 ulQosGetQciPriority (UInt16 qciConfigPriority)
{
    return ulLcQciPriMap_g[qciConfigPriority - 1];
}
/* SPR 11081 changes end */
/*****************************************************************************
 * Function Name  : ulQosGetMcsPriority
 * Inputs         : mcs - value of Modulation Coding Scheme
 * Outputs        : None
 * Returns        : MCS priority
 * Description    : It maps the mcs with its priority.
 *****************************************************************************/
 UInt16 ulQosGetMcsPriority (UInt16 mcs)
{
    /* SPR 12344 changes start */
    return ulLcMcsPriMap_g[mcs];
    /* SPR 12344 changes end */
}

/*****************************************************************************
 * Function Name  : ulQosUpdateQloadTableForLcPriority
 * Inputs         : lcQlTable_p - pointer to LC Qload Table,
 *                  bitRate - value of bitrate in bps,
 *                  pktDelayBgt - packet Delay Budget
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the Qload table.
 *****************************************************************************/
 void ulQosUpdateQloadTableForLcPriority(UInt32 *lcQlTable_p,
        /* L2_PERFORMANCE_OPT_CHANGES_START */
        UInt32 bitRate, 
        /* L2_PERFORMANCE_OPT_CHANGES_END */
        UInt16 pktDelayBgt)
{
    /* +COVERITY 10878 */ 
    UInt64 qSize = ((UInt64)bitRate * pktDelayBgt) / NUM_TICKS_PER_SECOND;
    /* -COVERITY 10878 */ 

    /*Update qload table*/
#ifdef FLOAT_OPERATIONS
    *lcQlTable_p++  =  0.25 * qSize;
    *lcQlTable_p++  =  0.5  * qSize;
    *lcQlTable_p++  =  0.75 * qSize;
    *lcQlTable_p    =  1    * qSize;
#else

    *lcQlTable_p++  =  qSize >>2;
    *lcQlTable_p++  =  qSize >>1;
    *lcQlTable_p++  =  (3 * qSize)>>2;
    *lcQlTable_p++  =  qSize;
    /*QOS_CHG*/
    *lcQlTable_p    =  (72000000/NUM_TICKS_PER_SECOND) * 300;
    /*QOS_CHG*/
#endif

}

/*****************************************************************************
 * Function Name  : ulQosUpdateTknTableForLcPriority
 * Inputs         : lcTknTable_p - pointer to LC Token Table,
 *                  bitRate - value of bitrate in bps,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the Token table.
 *****************************************************************************/
 void ulQosUpdateTknTableForLcPriority (UInt32 *lcTknTable_p, 
        /* L2_PERFORMANCE_OPT_CHANGES_START */
        /*CA Changes start  */
        UInt32 bitRate, InternalCellIndex internalCellIndex)
/*CA Changes end  */
/* L2_PERFORMANCE_OPT_CHANGES_END */
{
    /* +COVERITY 10879 */ 
    /*CA Changes start  */
    InitParams *initParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    UInt64 mTkn  = ((UInt64)bitRate * initParams_p->ulSchdConfig.ulTokenPeriodicity * 
            MAX_TKN_SCHD_WINDOW)/NUM_TICKS_PER_SECOND;
    /*CA Changes end  */
    /* -COVERITY 10879 */ 
#ifdef FLOAT_OPERATIONS
    *lcTknTable_p++  =  0.5 * mTkn;
    *lcTknTable_p++  =  0.8 * mTkn;
    *lcTknTable_p++  =  0.9 * mTkn;
    *lcTknTable_p    =  1   * mTkn;
#else
    *lcTknTable_p++  =  mTkn >> 1;
    *lcTknTable_p++  =  (mTkn<<2)/5;
    *lcTknTable_p++  =  (9 * mTkn)/10;
    *lcTknTable_p++  =  mTkn;
    /*CA Changes start  */
    *lcTknTable_p    = 9422 * initParams_p->ulSchdConfig.ulTokenPeriodicity * MAX_TKN_SCHD_WINDOW ;
    /*CA Changes end  */

#endif


}

/*****************************************************************************
 * Function Name  : ulQosUpdatePdbTableForLcPriority
 * Inputs         : lcPdbTable_p - pointer to LC PDB Table,
 *                  pktDelayBgt - packet Delay Budget
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the PDB table.
 *****************************************************************************/
 void ulQosUpdatePdbTableForLcPriority (UInt32 *lcPdbTable_p, 
        UInt16 pktDelayBgt)
{
#ifdef FLOAT_OPERATIONS
    *lcPdbTable_p++  =  0.25 * pktDelayBgt;
    *lcPdbTable_p++  =  0.5  * pktDelayBgt;
    *lcPdbTable_p++  =  0.75 * pktDelayBgt;
    *lcPdbTable_p++  =  1    * pktDelayBgt;
    *lcPdbTable_p    =  0xffff;
#else
    *lcPdbTable_p++  =   pktDelayBgt >> 2;
    *lcPdbTable_p++  =   pktDelayBgt >> 1;
    *lcPdbTable_p++  =   (3 * pktDelayBgt) >> 2;
    *lcPdbTable_p++  =   pktDelayBgt;
    *lcPdbTable_p    =  0xffff;

#endif

}

/******************************************************************************************
 * Function Name  : ulQosScheduler
 * Inputs         : numberOfAvailableRBs_p - The Number of RBs Allocated Orginally,
 *                  currentGlobalTTITickCount - Current Global TTI Tick,
 *                  numUlUEsToBeScheduled - Number of ULUE's to be scheduled,
 *					ulDelay,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : MAX_ADVANCE_UL_SUBFRAMES_ALLOC
 * Description    : This function fetches the Strategy LC node from the 
 *                  priortised LC Tree and prepares the Scheduled UE queue. 
 *****************************************************************************/
UInt32 ulQosScheduler(UInt32  *numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        SInt16  numUlUEsToBeScheduled
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
        )
{



    ulQosStrategyMakeScheduledUeQueue(
            numberOfAvailableRBs_p,
            currentGlobalTTITickCount,
            &numUlUEsToBeScheduled
#ifdef FDD_CONFIG
            ,ulDelay
#endif
            /* CA Changes start */
            ,internalCellIndex
            /* CA Changes end */
            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
            ,ulSubFrameNum
#endif
            /* TDD Config 0 Changes End */
            );



    return MAX_ADVANCE_UL_SUBFRAMES_ALLOC;
}

/*****************************************************************************
 * Function Name  : ulQosUpdateStrategyNodes
 * Inputs         : internalCellIndex
 * Inputs         : For TDD ulSubFrameNume and ulSubFrameNum 
 *                  For FDD none.
 * Outputs        : None
 * Returns        : None
 * Description    : This fucntion updates Scheduled UE queue.
 *****************************************************************************/
/* TDD Config 0 Changes Start */
void ulQosUpdateStrategyNodes(
        InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
        /* +- SPR 17777 */
        ,UInt8 ulSubFrameNum,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix start */
        ExecutionLegFunctionArg* execArgs
#endif
        )
/* TDD Config 0 Changes Start */
{
    /*
     *  1.  Traverse through the Scheduled UE nodes
     *  2.  update the message type
     *  3.  update the harq for the scheduled RB.s
     *  4.  validate for minimum TB size
     */

    UInt8 countULUENewTx = 0;
    /* UInt16 ueIndex = 0; TDD Warning Fix */
    ULUEContext   *ueULContext_p = PNULL;
    ULStrategyTxNode *ulStrategyTxNode_p = PNULL;
    ULUeScheduleInfo *ulUeScheduleInfo_p = PNULL;
    /* SPR 5713 changes start */
    UInt8 minRBAsPerUeRelease = 0;
    /* SPR 5713 changes end */
    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    UInt8 tokenUpdateFromStr = TRUE;
#endif
    /* TDD Config 0 Changes End */

    /*CA Changes start  */
    while(countULUENewTx < ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount)
        /*CA Changes end  */
    {
        /*putEntry of DLStrategyTXNode in DL Strategy Schedule Info */
        /*CA Changes start  */
        ulUeScheduleInfo_p = &(ulStrategyScheduleInfo_gp[internalCellIndex]->ulUeScheduleInfo
                /*CA Changes end  */
                [countULUENewTx]);

        ueULContext_p   = ulUeScheduleInfo_p->ueULContext_p;

        ulStrategyTxNode_p = ulUeScheduleInfo_p->ulStrategyTxNode_p;

        if((PNULL == ueULContext_p) ||
                (PNULL == ulStrategyTxNode_p))
        {
            LOG_MAC_MSG(MAC_UL_SCHEDULED_UE_HAS_NULL_POINTER, LOGFATAL, 
                    MAC_UL_Strategy,
                    getCurrentTick(),
                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
                    "UE ctx or Strategy Tx node is NULL");
            /* + SPR_17858_558_CHANGES */
            ++countULUENewTx;
            /* - SPR_17858_558_CHANGES */
            continue;

        }

        /* SPR 5713 changes start */
        VALIDATE_UPLINK_RB(ulStrategyTxNode_p->avgMcs,minRBAsPerUeRelease,ueULContext_p->ulGrantMinDataSize,
                ueULContext_p->ueComplianceRelease);
        if( (ulStrategyTxNode_p->requiredRB) &&
                (minRBAsPerUeRelease > ulStrategyTxNode_p->requiredRB) )
        {
            ulStrategyTxNode_p->requiredRB = minRBAsPerUeRelease;
        }
        /* SPR 5713 changes end */
        /* Phase2_CSI_Strategy_Changes start */
        if(PNULL == ueULContext_p->ulUeScellContext_p[START_SCELL_INDEX])
        {
            /* non ca ue case */
            /* Phase2_CSI_Strategy_Changes end */
            if (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG ==
                    ulStrategyTxNode_p->ulGrantRequestType
                    && ulStrategyTxNode_p->minDataSize > CQI_ULGRANT_MIN_DATA_SIZE)
            {
                ulStrategyTxNode_p->ulGrantRequestType =
                    CQI_WITH_DATA_ULGRANT_REQ_MSG;
                /* + SPR 6194 Fix */
                ueULContext_p->aperiodicCQIToBeRequested = TRUE;
                /* - SPR 6194 Fix */
                /*SPR 17128 fix start*/
                ulStrategyTxNode_p->avgMcs=ueULContext_p->mcsIndex;
                /*SPR 17128 fix end*/
                /* CA Stats Changes Start */
                /*debug stats*/
                LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_DATA_CQI(
                        internalCellIndex);
                /* CA Stats Changes End */
            }
        }
        /* Phase2_CSI_Strategy_Changes start */
        else
        {
            if((ueULContext_p->aperiodicCQIToBeRequested == 
                        servingCellBitMask_g[PCELL_SERV_CELL_IX])|| (
                            ueULContext_p->aperiodicCQIToBeRequested == servingCellBitMask_g[
                            START_SCELL_INDEX]))
            {

                if (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG ==
                        ulStrategyTxNode_p->ulGrantRequestType
                        && ulStrategyTxNode_p->minDataSize > CQI_ULGRANT_MIN_DATA_SIZE)
                {
                    ulStrategyTxNode_p->ulGrantRequestType =
                        CQI_WITH_DATA_ULGRANT_REQ_MSG;
                    /*SPR 17128 fix start*/
                    ulStrategyTxNode_p->avgMcs=ueULContext_p->mcsIndex;
                    /*SPR 17128 fix end*/

                    /* CA Stats Changes Start */
                    /*debug stats*/
                    LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_DATA_CQI(
                            internalCellIndex);
                    /* CA Stats Changes End */
                }
            }
            else if((ueULContext_p->aperiodicCQIToBeRequested & 
                        servingCellBitMask_g[PCELL_SERV_CELL_IX]) && (
                            ueULContext_p->aperiodicCQIToBeRequested & servingCellBitMask_g
                            [START_SCELL_INDEX]))
            {
                /* Aperiodic cqi requested for both cells */
                if (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG ==
                        ulStrategyTxNode_p->ulGrantRequestType
                        && ulStrategyTxNode_p->minDataSize > (CQI_ULGRANT_MIN_DATA_SIZE +
                            CQI_ULGRANT_MIN_DATA_SIZE))
                {
                    ulStrategyTxNode_p->ulGrantRequestType =
                        CQI_WITH_DATA_ULGRANT_REQ_MSG;
                    /*SPR 17128 fix start*/
                    ulStrategyTxNode_p->avgMcs=ueULContext_p->mcsIndex;
                    /*SPR 17128 fix end*/

                    /* CA Stats Changes Start */
                    /*debug stats*/
                    LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_DATA_CQI(
                            internalCellIndex);
                    /* CA Stats Changes End */
                }
            }
        }
        /* Phase2_CSI_Strategy_Changes end */
        /* SPS changes start */ 
        if( (0 != ulStrategyTxNode_p->maxDataSize)||(ueULContext_p->schType == SPS_SCHEDULING))
            /* SPS changes end */ 
        {
            /*putEntry of DLStrategyTXNode in PDCCH Queue */
            if (!(putEntryInULSchedulerNewTxQueue(
                            ulStrategyTxNode_p,
                            internalCellIndex
                            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                            ,currentGlobalTTITickCount
                            ,ulSubFrameNum
                            ,ueULContext_p
#endif
                            /* TDD Config 0 Changes End */
                            )))
            {
                LOG_MAC_MSG(MAC_PUSH_IN_QUEUE_FAILURE_IN_UL_LOG_ID, LOGFATAL, 
                        MAC_UL_Strategy,
                        getCurrentTick(),
                        DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
                        "UL Strategy Tx Queue");
                freeMemPool(ulStrategyTxNode_p);
            }
            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
            else
            {
                /* Mark ulUEContext_p->ulStrategyTxNode_p as NULL after
                 * pushing the Node in strategy Output queue or List */
                ueULContext_p->ulStrategyTxNode_p = PNULL;
                /* update tokens after UL strategy processing */
                ulQosUpdateScheduledTokens(ueULContext_p,
                        ulStrategyTxNode_p->maxDataSize,
                        ulStrategyTxNode_p->ulHarqProcessId,
                        internalCellIndex,
                        ulSubFrameNum,
                        tokenUpdateFromStr,
                        execArgs);
            }
#endif
            /* TDD Config 0 Changes End */


            /* CA Stats Changes Start */
            /*debug stats*/
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_UE_INDEX(ueULContext_p->ueIndex, 
                    internalCellIndex);

            /*debug stats*/
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_MIN_TB_SIZE(
                    ulStrategyTxNode_p->minDataSize, internalCellIndex);

            /*debug stats*/
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_MAX_TB_SIZE(
                    ulStrategyTxNode_p->maxDataSize, internalCellIndex);

            /*debug stats*/
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_UL_GRANT_TYPE(
                    ulStrategyTxNode_p->ulGrantRequestType, internalCellIndex);

            /*debug stats*/
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_AVG_MCS(
                    ueULContext_p->mcsIndex, internalCellIndex);

#ifdef TDD_CONFIG
            /*debug stats*/
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_HARQ_PROCESS_ID(
                    ulStrategyTxNode_p->ulHarqProcessId, internalCellIndex);
#endif

            /*debug stats*/
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED_RB(
                    ulStrategyTxNode_p->requiredRB, internalCellIndex);

#ifdef SSI_DEBUG_STATS
            UInt16 ueIndex = ueULContext_p->ueIndex;

            /*debug stats*/
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED_LC_COUNT(
                    ueIndex, internalCellIndex);
            /* CA Stats Changes End */
#endif

            LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_NEW_TX_COUNT
                (ueULContext_p->ueIndex);

            /* CA Stats Changes Start */
            LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_TX_UE(
                    internalCellIndex);
            /* CA Stats Changes End */
            /* ICIC LOGs Start to view UE Info */
            LOG_UT(MAC_SCHDULED_UE_INFO_IN_UL,LOGDEBUG,
                    MAC_UL_Strategy,
                    getCurrentTick(),
                    ueULContext_p->ueIndex,
                    ulStrategyTxNode_p->minDataSize,
                    ulStrategyTxNode_p->maxDataSize,
                    ulStrategyTxNode_p->requiredRB,
                    ueULContext_p->userLocationType,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME, "UE info \
                    for Qos Schduling");


            /* ICIC Logs End */
        }
        else
        {
            /* SPR 23356 Fix Start */
#ifdef FDD_CONFIG
            ulQosUpdateScheduledTokens(ueULContext_p,0,INVALID_HARQ_ID,internalCellIndex,0); 
#endif
            /* SPR 23356 Fix End */
            freeMemPool(ulStrategyTxNode_p);
            ueULContext_p->ulStrategyTxNode_p = PNULL;            

        }
        ++countULUENewTx;
    }
}

/*************************************************************************************
 * Function Name  : ulQosStrategyMakeScheduledUeQueue
 * Inputs         : numberOfAvailableRBs_p - Pointer to the number of Avavailable RBs
 *                  currentGlobalTTITickCount - Current TTI Tick
 *                  numSchedulableUes_p - No. of UL UE's to be scheduled,
 *					ulDelay,
 *					internalCellIndex
 *                  ulSubFrameNum - ulSubFrameNumber for which grant is being given
 * Outputs        : None
 * Returns        : None
 * Description    : This function fetches the Strategy LC node from the 
 *                  priortised LC Tree and prepares the Scheduled UE queue. 
 *****************************************************************************/
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
        )
{
    /* Cyclomatic Complexity Changes -starts here */
    /* Cyclomatic Complexity Changes -ends here */

    /*
       1.   Traverse through the prioritized LC list and allocate RB.s for each
       LC upto 150 RB (configurable)
       2.   for each LC node until scheduled RB.s <=  150 RB (configurable)
       a.   if the LC node ==  first LC node of UE to be scheduled
       i.   And insert UE Node for each scheduled first LC node of UE,  
       in schedule UE list by calling ulQosStrategyUpdateNewTxUEList()
       b.   Else
       i.   Get  the strategy node
       c.   if (Scheduled TB Size of UE  < Max TB Size of UE (Obtained from 
       UE Category))
       i.  Obtain the schedulable bytes equal to minimum of { available 
       tokens , remaining Net-BSR of LCG } of the LC
       ii. If((Scheduled TB Size of UE +  schedulable bytes)  >  
       Max TB Size of UE)
       1.  schedulable bytes  =  
       Max TB Size of UE  - Scheduled TB Size of UE
       iii.    If scheduling for 100 RB.s is not crossed
       1.  Increment the Min TB Size with the schedulable bytes for 
       the LC
       iv. Else
       1.  Increment the Max TB Size with the schedulable bytes for 
       the LC

       d.  get the RB.s required for the LC scheduled and increment the 
       scheduled RB by it
       3.  Insert the retransmission prioritized LC list to Active prioritized LC
       list
       */

#ifdef SSI_DEBUG_STATS
    updateUlQosPerTTIStats(internalCellIndex);
#endif
    UInt16 ueIndex = 0;
    SInt32 availToken = 0;
    SInt16 numAvailableRBs[MAX_USER_LOCATION] = {0,0} ;
    /* CA Changes start */
    UInt8  strategyBufferRB  =  cellSpecificParams_g.cellConfigAndInitParams_p\
     [internalCellIndex]->initParams_p->ulSchdConfig.strategyBufferRB;
    numAvailableRBs[CC_CE_REGION] = numberOfAvailableRBs_p[CC_CE_REGION]  + strategyBufferRB;
    numAvailableRBs[CE_REGION] = numberOfAvailableRBs_p[CE_REGION] + strategyBufferRB; /* Fixed strategyBufferRB is added to CE */
    UInt8  ueLocationType = 0 ;
    unusedULStrategyBufferRB_g[internalCellIndex] = strategyBufferRB;
    /* CA Changes end */
    /* ICIC Changes End  */
    ULQosStrategyLCNode *ulQosStrategyLCNode_p = PNULL;

    ULUEContextInfo * ueULContextInfo_p = PNULL;
    ULUEContext * ueULContext_p = PNULL;
    ULLogicalChannelInfo *lcInfo_p = PNULL;
    ULAmbrInfo *ambrInfo_p = PNULL;
    UInt32 schedulableBytes = 0;
    UInt8 lcgId = 0;
    UInt8 gbrSchedulingFlag = TRUE;
    SInt16 requiredRB = 0;
    SInt16 deltaRB = 0;
    SInt16 scheduledGbrBytes = 0;
    UInt32 tempTBSize = 0;

    ULStrategyTxNode * ulStrategyTxNode_p = PNULL;

    UInt32 netQueueLoad = 0;
    UInt32 netAvlblTkns = 0;
    UInt8 insertLcNodeFlag = FALSE;
    UInt8  firstNodeFlag = FALSE; 
    /* Cyclomatic Complexity Changes -starts here */
    UInt8 retVal = MAC_SUCCESS;
    /* Cyclomatic Complexity Changes -ends here */
    /* Get first LC node from the TREE */

    /* EICIC +*/
    if (VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex])
    {
        ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)
            /*CA Changes start  */
            ulQosGetFirstLcNode(ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE]);
        /*CA Changes end  */
    }
    else if (NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex])
    {		
        ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)
            ulQosGetFirstLcNode(ulQosStrategyLCTree_gp[internalCellIndex][NON_VICTIM_UE]);
    }
    /* EICIC -*/

    UInt8 ulHarqProcessId = INVALID_HARQ_ID;

    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    UInt8 ulSubFrame2ndIdx = 0;
    /* SPR 23393 Changes Start */
    UInt8 subFrameNum = currentGlobalTTITickCount % MAX_SUB_FRAME;
    UInt8 ulSubFrameIdx = MODULO_TWO(ulSubFrameNum);
    /* SPR 23393 Changes End */
#endif
    UInt16 scheduledBytes  = 0;
    /* TDD Config 0 Changes End */
    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_ABSOLUTE_TTI(currentGlobalTTITickCount, 
            internalCellIndex);
    /* CA Stats Changes End */


    /* This is done since initial when only SRB channels are added aggGBR is 0*/
    /*CA Changes start  */
    if (VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex])
    {
        if ( !ulAggregateGbrPerTTI_g[internalCellIndex][VICTIM_UE] )
            /*CA Changes end  */
        {
            gbrSchedulingFlag = FALSE;
        } 
    }
    else
    {
        if ( !ulAggregateGbrPerTTI_g[internalCellIndex][NON_VICTIM_UE] )
            /*CA Changes end  */
        {
            gbrSchedulingFlag = FALSE;
        } 
    }
    /* ICIC Changes Start */   
   /* + SPR 24051 changes */       
    while((0 < numAvailableRBs[CC_CE_REGION]) &&
          (PNULL != ulQosStrategyLCNode_p))
   /* - SPR 24051 changes */       
        /* ICIC Changes End  */
    {
        ulStrategyTxNode_p = PNULL;

        ueIndex = ulQosStrategyLCNode_p->ueId;
        ueULContextInfo_p   = &ulUECtxInfoArr_g[ueIndex];
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        scheduledBytes = ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx];
#else
        scheduledBytes = ulQosStrategyLCNode_p->scheduledBytes;
#endif
        /* TDD Config 0 Changes End */
        if(!(ueULContextInfo_p->pendingDeleteFlag))
        {
            /* Get UE Context */
            ueULContext_p       = ueULContextInfo_p->ulUEContext_p;



            /* Cyclomatic Complexity Changes - starts here */
            retVal = ulQosValidatePriorityTreeNode(ueULContext_p,
                    ulQosStrategyLCNode_p,
                    gbrSchedulingFlag,
                    &ulHarqProcessId,
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
            if(MAC_FAILURE == retVal)
            {
                ulQosGetNextNodeFromPriorityTree(&ulQosStrategyLCNode_p, &gbrSchedulingFlag,
                        internalCellIndex);
                continue;
            }
            /* Cyclomatic Complexity Changes - ends here */
            /* ICIC Changes Start */
            ueLocationType =  ueULContext_p->userLocationType;
            /* ICIC Changes End   */

            if(QOS_GBR == ulQosStrategyLCNode_p->channelType) 
            {
                lcInfo_p   = ulQosStrategyLCNode_p->lcInfo_p;
                availToken = lcInfo_p->ulQosInfo.availLcToken;
                lcgId      = lcInfo_p->lcGId;
            }
            else if (NON_GBR == ulQosStrategyLCNode_p->channelType)
            {
                ambrInfo_p = ulQosStrategyLCNode_p->ulAmbrInfo_p;
                availToken = ambrInfo_p->availAmbrToken;
                lcgId      = ambrInfo_p->lcgId;
            }
            else if (SRB == ulQosStrategyLCNode_p->channelType)
            {
                lcInfo_p   = ulQosStrategyLCNode_p->lcInfo_p;
                lcgId      = lcInfo_p->lcGId;
                availToken = ueULContext_p->ulLCGInfo[lcgId].qLoad;
            }    
            else
            {
                /* Cyclomatic Complexity Changes - starts here */
                ulQosGetNextNodeFromPriorityTree(&ulQosStrategyLCNode_p, &gbrSchedulingFlag,
                        internalCellIndex);
                /* Cyclomatic Complexity Changes - ends here */

                continue;
            }    
            /* 4 UE FTP fix, should not put UE on priority tree if qLoad is zero */
            if (!availToken || (ueULContext_p->ulLCGInfo[lcgId].qLoad == 0)) 
            {
                LOG_UT(MAC_LC_NODE_NOT_PROCESSED_IN_UL_LOG_ID,LOGDEBUG,MAC_UL_Strategy,
                        getCurrentTick(),
                        ueIndex,ulQosStrategyLCNode_p->lchId,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME, "No Tokens Available for the LC");
                /* Cyclomatic Complexity Changes - starts here */
                ulQosGetNextNodeFromPriorityTree(&ulQosStrategyLCNode_p, &gbrSchedulingFlag,
                        internalCellIndex);
                /* Cyclomatic Complexity Changes - ends here */
                continue;
            }    
            schedulableBytes = 0; 

            if(PNULL == ueULContext_p->ulStrategyTxNode_p)
            {
                if (*numUlUEsToBeScheduled_p)
                {
                    /* ICIC Change Start */
                    /** SPR 12505 Fix ; Start **/
                    if ((!numAvailableRBs[ueLocationType]) ||
                            ((CE_USER == ueLocationType)
                             && ((numAvailableRBs[CE_REGION]<=unusedULStrategyBufferRB_g[internalCellIndex] )||
                                 /*CA Changes start  */
                                 (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] ))))
                        /*CA Changes end  */
                        /** SPR 12505 Fix :  End **/

                    {
                        LOG_UT(MAC_CE_UE_NOT_BE_SCHDULED_UL,LOGDEBUG,
                                MAC_UL_Strategy,
                                getCurrentTick(),
                                ueULContext_p->ueIndex,
                                numberOfAvailableRBs_p[ueLocationType],
                                /*CA Changes start  */
                                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex],
                                /*CA Changes end  */
                                ueLocationType,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME, "CE UEs can not be scheduled");
                        /* Cyclomatic Complexity Changes - starts here */
                        ulQosGetNextNodeFromPriorityTree(&ulQosStrategyLCNode_p, &gbrSchedulingFlag,
                                internalCellIndex);
                        /* Cyclomatic Complexity Changes - ends here */
                        continue;
                    }

                    /* ICIC change End */
                    /* TTIB_Code Start */
#ifdef FDD_CONFIG
                    if( ueULContext_p->ttiBundlingEnabled )
                    {
                        /*CA Changes start  */
                        if (( availableRBForTtiBundlingPerTick_g[internalCellIndex] >= MIN_TTI_BUNDLE_RB ) 
                                /*CA Changes end  */
                                && (MAC_SUCCESS == processMeasGapValidation(
                                        ueULContext_p,currentGlobalTTITickCount,ulDelay )))
                        {
                            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy][%s] bundled Ue [%d] is  scheduled.\n",
                                    __func__, ueULContext_p->ueIndex);
                        }    
                        else
                        {    
                            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy][%s] bundled  Ue [%d] not scheduled.\n",
                                    __func__, ueULContext_p->ueIndex);

                            /* Cyclomatic Complexity Changes - starts here */
                            ulQosGetNextNodeFromPriorityTree(&ulQosStrategyLCNode_p, &gbrSchedulingFlag,
                                    internalCellIndex);
                            /* Cyclomatic Complexity Changes - ends here */
                            continue;
                        }
                    }
#endif
                    /* TTIB_Code End */
                    /*QOS_CYCLIC_COMPLEXITY_CHG*/
                    ulStrategyTxNode_p =
                        ulQosStrategyUpdateNewTxUEList(ueIndex,
                                currentGlobalTTITickCount,
                                UL_NEW_TRAVERSAL
                                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                                ,ulSubFrameNum
#endif
                                /* TDD Config 0 Changes End */
                                );
                    /*QOS_CYCLIC_COMPLEXITY_CHG*/
                    /* Cyclomatic Complexity Changes -starts here */
                    if(PNULL == ulStrategyTxNode_p)
                    {

                        LOG_MAC_MSG(MAC_UE_CANT_BE_SCHEDULED,LOGFATAL,MAC_UL_Strategy,
                                getCurrentTick(),
                                ueULContext_p->ueIndex,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME, "Cannot schedule this UE");
                        /* Cyclomatic Complexity Changes - starts here */
                        ulQosGetNextNodeFromPriorityTree(&ulQosStrategyLCNode_p, &gbrSchedulingFlag,
                                internalCellIndex);
                        /* Cyclomatic Complexity Changes - ends here */

                        continue;
                    }
                    ueULContext_p->ulStrategyTxNode_p = ulStrategyTxNode_p;

                    --(*numUlUEsToBeScheduled_p);
                    /* ICIC Change Start */
                    if (CE_USER == ueLocationType)
                    {
                        /*CA Changes start  */
                        --maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex];
                        /*CA Changes end  */
                    }
                    /* Cyclomatic Complexity Changes -ends here */
                }    
                else
                {
                    LOG_UT(MAC_NUM_UE_PER_TICK_REACHED_IN_UL_LOG_ID,LOGDEBUG,
                            MAC_UL_Strategy,
                            getCurrentTick(),
                            ueULContext_p->ueIndex,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME, "No more UEs can be scheduled");
                    /* Cyclomatic Complexity Changes - starts here */
                    ulQosGetNextNodeFromPriorityTree(&ulQosStrategyLCNode_p, &gbrSchedulingFlag,
                            internalCellIndex);
                    /* Cyclomatic Complexity Changes - ends here */
                    continue;
                }
            }
            else
            {
                /* SPR# 4812 Changes Starts */
                ulQosProcessUeForRBRestriction(UL_NEW_TRAVERSAL,
                        ueULContext_p);
                /* SPR# 4812 Changes Ends */
                ulStrategyTxNode_p = ueULContext_p->ulStrategyTxNode_p;
                /* SPR 18918_18643_19032 Start */
                if ( CQI_WITHOUT_DATA_ULGRANT_REQ_MSG == ulStrategyTxNode_p->ulGrantRequestType )
                {
                    ulStrategyTxNode_p->ulGrantRequestType = CQI_WITH_DATA_ULGRANT_REQ_MSG;
#ifdef FDD_CONFIG
                    if( ueULContext_p->ttiBundlingEnabled )
                    {
                        if ( ulUECtxInfoArr_g[ueIndex].ulUEContext_p->mcsIndex > MAX_TTI_BUNDLE_MCS )
                        {
                            ulStrategyTxNode_p->avgMcs = MAX_TTI_BUNDLE_MCS;
                        }
                        else
                        {
                            ulStrategyTxNode_p->avgMcs = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->mcsIndex;
                        }
                    }
                    else
                    {
                        ulStrategyTxNode_p->avgMcs = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->mcsIndex;
                    }
#endif
                }
                /* SPR 18918_18643_19032 End */

            }

            if(ulStrategyTxNode_p->maxDataSize < ueULContext_p->maxTBSize)
            {
                /* Obtain the schedulable bytes equal to
                 * First Re Tx PDU Qload of the LC */
                netQueueLoad = 0;
                netAvlblTkns = 0;

                /* TDD Config 0 Changes Start */
                if ( ueULContext_p->ulLCGInfo[lcgId].qLoad  > scheduledBytes )
                {
                    /* SPR 14371 Fix Starts */
                    netQueueLoad = ueULContext_p->ulLCGInfo[lcgId].qLoad /* - scheduledBytes*/;
                    /* SPR 14371 Fix Ends  */
                }

                if ( availToken  > scheduledBytes )
                {
                    netAvlblTkns = availToken  - scheduledBytes;
                }
                /* TDD Config 0 Changes End */

                schedulableBytes =  (netAvlblTkns > netQueueLoad ) ? netQueueLoad:netAvlblTkns;

                if ( (ulStrategyTxNode_p->maxDataSize + schedulableBytes) >  ueULContext_p->maxTBSize)
                {
                    schedulableBytes = ueULContext_p->maxTBSize -  ulStrategyTxNode_p->maxDataSize;
#ifdef PERF_STATS
                    if ( ueULContext_p->maxtbSizebyUeCategory < ueULContext_p->maxRestrictedTB )
                    {
                        /*  number of ms where respective UE is limited in the
                         *  UL direction by its UE capability */
                        gMacUePerfStats_p[ueIndex].lteUeMacSchedULPerfStats.\
                            totalTimeRestrictUeInUlbyUeCap++;
                    }
#endif
                }    

                if(schedulableBytes)
                {
                    /* CA Stats Changes Start */
                    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_REMAINING_QLOAD(
                            ueULContext_p->ulLCGInfo[lcgId].qLoad, 
                            internalCellIndex);
                    /* CA Stats Changes End */


                    insertLcNodeFlag = FALSE;
                    firstNodeFlag = FALSE; 
                    /* TDD Config 0 Changes Start */
                    if (!scheduledBytes)
                        /* TDD Config 0 Changes End */
                    {
                        insertLcNodeFlag = TRUE;
                        if(TRUE == gbrSchedulingFlag )
                        {
                            if (NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex])
                            {
                                /*CA Changes start  */
                                if((scheduledGbrBytes + schedulableBytes) < ulAggregateGbrPerTTI_g[internalCellIndex][NON_VICTIM_UE]) 
                                    /*CA Changes end  */
                                {
                                    scheduledGbrBytes += schedulableBytes;
                                }
                                else                     
                                {
                                    /*CA Changes start  */
                                    schedulableBytes =  ulAggregateGbrPerTTI_g[internalCellIndex][NON_VICTIM_UE] - scheduledGbrBytes;
                                    /*CA Changes end  */

                                    gbrSchedulingFlag = FALSE;

                                    firstNodeFlag = TRUE; 
                                }    
                            }
                            else
                            {

                                /*CA Changes start  */
                                if((scheduledGbrBytes + schedulableBytes) < ulAggregateGbrPerTTI_g[internalCellIndex][VICTIM_UE]) 
                                    /*CA Changes end  */
                                {
                                    scheduledGbrBytes += schedulableBytes;
                                }
                                else                     
                                {

                                    /*CA Changes start  */
                                    schedulableBytes =  ulAggregateGbrPerTTI_g[internalCellIndex][VICTIM_UE] - scheduledGbrBytes;
                                    /*CA Changes end  */

                                    gbrSchedulingFlag = FALSE;

                                    firstNodeFlag = TRUE; 
                                }    
                            }
                        }
                    }   

                    /*Subtracting schedulable bytes to keep the remaining Qload*/
                    ueULContext_p->ulLCGInfo[lcgId].qLoad -= schedulableBytes;

                    /* Increment the Max TB Size and  Strategy LC node
                     * with the schedulable bytes for the LC
                     */
                    ulStrategyTxNode_p->maxDataSize += schedulableBytes;
                    tempTBSize = ulStrategyTxNode_p->maxDataSize;

                    requiredRB = 
                        /* SPR 5999 changes start */
                        /*CA Changes start  */
                        DETERMINE_RBs_UL(ueULContext_p->mcsIndex,
                                tempTBSize, internalCellIndex);
                    /*CA Changes end  */
                    /* SPR 5999 changes end */
                    /* TTIB_Code Start */
#ifdef FDD_CONFIG
                    if( ueULContext_p->ttiBundlingEnabled )
                    {
                        /*CA Changes start  */
                        requiredRB = DETERMINE_RBs_UL(ulStrategyTxNode_p->avgMcs,tempTBSize, internalCellIndex);
                        /*CA Changes end  */

                        if( requiredRB > MAX_TTI_BUNDLE_RB )
                        {
                            requiredRB = MAX_TTI_BUNDLE_RB;
                        }
                        /*Coverity_73443 Fix Start*/ 
                        if( 0 <  requiredRB )
                        {    
                            ulStrategyTxNode_p->maxDataSize = determineUlTBSize(ulStrategyTxNode_p->avgMcs,requiredRB);
                        }
                        /*Coverity_73443 Fix End*/ 
                        ulStrategyTxNode_p->minDataSize = determineUlTBSize(ulStrategyTxNode_p->avgMcs,MIN_TTI_BUNDLE_RB);
                    }
#endif

                    /* Cyclomatic Complexity Changes -starts here */
                    /*Coverity_73443 Fix Start*/ 
                    if( 0 <  requiredRB )
                    {
                        deltaRB = ulQosCheckForOverAllocation( ulStrategyTxNode_p,
                                ueULContext_p,
                                numAvailableRBs[ueLocationType], 
                                requiredRB,
                                /* SPR 13532 Fix Start */
                                schedulableBytes, 
#ifdef FDD_CONFIG
                                (currentGlobalTTITickCount + ulDelay)
#else
                                /*SPR 16417 Changes Start*/
                                currentGlobalTTITickCount,
                                ulSubFrameNum
                                /*SPR 16417 Changes End*/
#endif 
                                /* SPR 13532 Fix End */
                                );
                    } 
                    /*Coverity_73443 Fix End*/ 
                    /* Cyclomatic Complexity Changes -ends here */

#ifdef FDD_CONFIG
                    if( ueULContext_p->ttiBundlingEnabled )
                    {
                        if( ulStrategyTxNode_p->requiredRB > MAX_TTI_BUNDLE_RB )
                        {
                            ulStrategyTxNode_p->requiredRB = MAX_TTI_BUNDLE_RB;
                        }
                    }
#endif
                    /* ICIC Change Start */
                    LOG_UT(MAC_UE_RB_INFO_BEFORE_ALLOCATE_RB_IN_UL,LOGDEBUG,
                            MAC_UL_Strategy,
                            getCurrentTick(),
                            requiredRB,
                            ulStrategyTxNode_p->requiredRB,
                            ueLocationType,
                            deltaRB,
                            schedulableBytes,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME, "RB info \
                            for Qos Schduling");

                    LOG_UT(MAC_RB_INFO_BEFORE_ALLOCATE_RB_IN_UL,LOGDEBUG,
                            MAC_UL_Strategy,
                            getCurrentTick(),
                            numAvailableRBs[CC_CE_REGION],
                            numAvailableRBs[CE_REGION],
                            ueLocationType,
                            deltaRB,
                            schedulableBytes,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME, "RB info \
                            for Qos Schduling");


                    /*  ICIC Change End  */

                    numAvailableRBs[ueLocationType] -= deltaRB ;

                    if ( ueLocationType == CC_USER )
                    {
                        /* Number of Availbale RB of CC_CE_REGION should not be less than CE_REGION RB */
                        if (numAvailableRBs[CC_CE_REGION] <  numAvailableRBs[CE_REGION])  
                        {                                                                 
                            numAvailableRBs[CE_REGION] = numAvailableRBs[CC_CE_REGION];                                
                        } 
                    }                                                                
                    else                                      
                    {                                                                  
                        numAvailableRBs[CC_CE_REGION] -= deltaRB;                     
                    }

                    LOG_UT(MAC_RB_UPDATE_INFO_AFTER_ALLOCATE_RB_IN_UL,LOGDEBUG,
                            MAC_UL_Strategy,
                            getCurrentTick(),
                            numAvailableRBs[CC_CE_REGION],
                            numAvailableRBs[CE_REGION],
                            ueLocationType,
                            ueIndex,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME, "RB info\
                            for Qos Schduling");
                    /*  ICIC Change End  */
                    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                    ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx] += schedulableBytes;
                    /* Check if current subframe is TDD config 0, S subframe */
                    if (LTE_TRUE == isTddConfig0SsubFrame(subFrameNum,internalCellIndex))
                    {
                        /* If TDD Config S subframe then no need to add extra LC Node 
                         * if already added */
                        /* SPR 23393 Changes Start */
                        ulSubFrame2ndIdx = MODULO_TWO(ulSubFrameIdx + 1);
                        /* SPR 23393 Changes End */
                        if ( ulQosStrategyLCNode_p->scheduledBytes[ulSubFrame2ndIdx])
                        {
                            insertLcNodeFlag = FALSE;
                        }
                    }
#else
                    ulQosStrategyLCNode_p->scheduledBytes += schedulableBytes;
#endif
                    /* TDD Config 0 Changes End */

                    /* Insert the Lc Node in the scheduled LC list */
                    if (TRUE == insertLcNodeFlag)
                    {        
                        listInsertNode(&ueULContext_p->ulScheduledLcList,
                                &ulQosStrategyLCNode_p->scheduledLcNodeAnchor);

                        ++ueULContext_p->scheduledLcCount;
                    }    

                    /* CA Stats Changes Start */
                    /*debug stats*/
                    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_AVAILABLE_TOKEN(
                            availToken, internalCellIndex);
                    /*debug stats*/
                    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_SCHEDULED_BYTES(schedulableBytes, 
                            internalCellIndex);
                    /*debug stats*/
                    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_BSR_LOAD(ueULContext_p->bsrNetTotal, 
                            internalCellIndex);
                    /*debug stats*/
                    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_LC_SCHEDULED_PER_TTI_ABSOLUTE_TTI
                        (ueIndex,currentGlobalTTITickCount);
                    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_UE_ID_LC_ID(
                            ueIndex,ulQosStrategyLCNode_p->lchId, 
                            internalCellIndex);
                    /* CA Stats Changes End */
                    /*debug stats*/
                    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_LC_SCHEDULED_PER_TTI_LC_INDEX
                        (ueIndex,ulQosStrategyLCNode_p->lchId);
                    /*debug
                     * stats*/
                    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_TOTAL_LC_WEIGHT(ulQosStrategyLCNode_p->totLCWght,
                            internalCellIndex);
                    /*debug stats*/
                    LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_SCH_COUNT(ueIndex);
                    ulQosStrategyLCNode_p->isLCScheduled = TRUE;

                    if(TRUE == firstNodeFlag)
                    {
                        if  (VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex])
                        {
                            ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)
                                /*CA Changes start  */
                                ulQosGetFirstLcNode(ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE]);
                            /*CA Changes end  */
                        }
                        else if  (NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex])
                        {
                            ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)
                                /*CA Changes start  */
                                ulQosGetFirstLcNode(ulQosStrategyLCTree_gp[internalCellIndex][NON_VICTIM_UE]);
                            /*CA Changes end  */

                        }
                        continue;
                    }
                }
            }
            LOG_MAC_MSG (UL_QOS_SCHEDULED_LOG_ID, LOGDEBUG,
                    MAC_UL_Strategy, getCurrentTick (), ueULContext_p->ueIndex,
                    ulHarqProcessId,
                    ulStrategyTxNode_p->requiredRB,
                    ueULContext_p->bsrNetTotal,
                    ulStrategyTxNode_p->ulGrantRequestType,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");

        }
        else
        {
            LOG_UT(MAC_UE_DELETE_PENDING_IN_UL_LOG_ID,LOGDEBUG,
                    MAC_UL_Strategy,
                    getCurrentTick(),
                    ueIndex,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");

        }
        /* Cyclomatic Complexity Changes -starts here */
        ulQosGetNextNodeFromPriorityTree(&ulQosStrategyLCNode_p, &gbrSchedulingFlag,
                internalCellIndex);
        /* Cyclomatic Complexity Changes -ends here */
    }/*End of while*/

    /* ICIC Changes Start */
    /* CA Changes start */
    if(0 < numAvailableRBs[CC_CE_REGION] && cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
            enableAllocAfterAllowedBitrate)
        /* CA Changes end */
        /* ICIC Changes Ends */
    {
        /* ICIC Changes Start */
        ulQosReTraversePriorityLcList(numAvailableRBs,
                currentGlobalTTITickCount,
                numUlUEsToBeScheduled_p
#ifdef FDD_CONFIG
                , ulDelay
#endif
                ,internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                /* SPR 22539 Fix + */
                ,cellSpecificParams_g.
                cellConfigAndInitParams_p[internalCellIndex]->initParams_p->
                enableAllocAfterAllowedBitrate
                /* SPR 22539 Fix - */
                );

        /* ICIC Changes End */
    }

    /* ICIC Change Start */
    /* For cell center unUsed RB updated */
    numberOfAvailableRBs_p[CC_CE_REGION]    = numAvailableRBs[CC_CE_REGION]- 
        unusedULStrategyBufferRB_g[internalCellIndex];

    /* For Cell Edge unUsed RB updated */ 
    numberOfAvailableRBs_p[CE_REGION]    = numAvailableRBs[CE_REGION]-
        unusedULStrategyBufferRB_g[internalCellIndex];

    /* ICIC Change End */
}

/*************************************************************************************
 * Function Name  : ulQosReTraversePriorityLcList
 * Inputs         : numberOfAvailableRBs_p - Pointer to the number of Avavailable RBs,
 *                  currentGlobalTTITickCount - current global TTI Tick,
 *                  numUlUEsToBeScheduled_p - Number of UL UE's to be scheduled,
 *					ulDelay,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when strategy could not consume all 
 *                  the RBs while scheduling UEs. This function re-traverses 
 *                  the priortised LC Tree and appends the new Scheduled UE. 
 *****************************************************************************/
void ulQosReTraversePriorityLcList(SInt16  *numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        SInt16  *numUlUEsToBeScheduled_p
#ifdef FDD_CONFIG
        ,UInt8 ulDelay
#endif
        ,InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        /* SPR 22539 Fix + */
        ,UInt8 allowAllocBitRate
        /* SPR 22539 Fix - */
        )
{
    /* spr 2882 fix start */
/* SPR 22741 Start */
    /* Code Removed */ 
/* SPR 22741 Start */
    /* spr 2882 fix end */
    UInt16 ueIndex = 0;
    /* ICIC Changes Start */
    SInt16 numAvailableRBs[MAX_USER_LOCATION] = {0,0}; 
    numAvailableRBs[CC_CE_REGION] = numberOfAvailableRBs_p[CC_CE_REGION];
    numAvailableRBs[CE_REGION] = numberOfAvailableRBs_p[CE_REGION];
    UInt8 ueLocationType = CC_USER;
    /* ICIC Changes End */
    /*QOS ICIC fix*/
    SInt8 tempRB = 0;
    /*QOS ICIC fix*/

    ULQosStrategyLCNode *ulQosStrategyLCNode_p = PNULL;

    ULUEContextInfo * ueULContextInfo_p = PNULL;
    ULUEContext * ueULContext_p = PNULL;
    ULLogicalChannelInfo *lcInfo_p = PNULL;
    ULAmbrInfo *ambrInfo_p = PNULL;
    /*UInt32 schedulableBytes = 0;*/
    UInt32 reSchedulableBytes = 0;
    UInt8 lcgId = 0;
    UInt32 requiredRB = 0;
    SInt16 deltaRB = 0;
#ifdef FDD_CONFIG
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2 = TTI_MAP_FREE;
#endif
    UInt32 tempTBSize = 0;
    /* +- SPR 17777 */

#ifdef TDD_CONFIG
    /* SPR 23393 Changes Start */
    UInt32 sysFrameNum = 0;
    /* SPR 23393 Changes End */
#endif

    ULStrategyTxNode * ulStrategyTxNode_p = PNULL;
    /* SPR 4751 changes */
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    /* SPR 4751 changes */
    UInt8 ulHarqProcessId = INVALID_HARQ_ID;
    /* SPR 10032 Fix Starts*/
    UInt8 mcsIndex = 0;
    UInt32 ueMaxPowerFlag = 0;
    UInt32 ueMinPowerFlag = 0;
    SInt32 regionPowerMask = 0;
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + !(ulPowerMaskIndex_g[internalCellIndex]);
    /* SPR 10032 Fix Ends*/
    /* TDD Config 0 Changes Start */
    /* SPR 13532 Fix Start */
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
#ifdef TDD_CONFIG
    /*+-SPR 16417 Changes */
    /* SPR 13532 Fix End */
    /* SPR 23393 Changes Start */
    UInt8 ulSubFrameIdx = MODULO_TWO(ulSubFrameNum);
    /* SPR 23393 Changes End */
    UInt8 ulSubFrame2ndIdx = 0;
    UInt8 insertLcNodeFlag = TRUE;
    /* SPR 23393 Changes Start */
    UInt8 subFrameNum = currentGlobalTTITickCount % MAX_SUB_FRAME;
    /* SPR 23393 Changes End */
#endif
    /* TDD Config 0 Changes End */
    /* SPR 22539 Fix + */
    UInt16 scheduledBytes  = 0;
    UInt32 netQueueLoad = 0;
    UInt32 netAvlblTkns = 0;
    SInt32 availToken = 0;
    /* SPR 22539 Fix - */

#ifdef FDD_CONFIG
    ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
    ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
    ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET + MAX_HARQ_PROCESS_NB );
#endif


    /* Get first LC node from the TREE */
    if  (VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex])
    {
        ulQosStrategyLCNode_p = (ULQosStrategyLCNode* )
            /*CA Changes start  */
            ulQosGetFirstLcNode (ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE]);
        /*CA Changes end  */
    }
    else if (NON_VICTIM_UE ==  ueTypeToScheduleUL_g[internalCellIndex])
    {
        ulQosStrategyLCNode_p = (ULQosStrategyLCNode* )
            /*CA Changes start  */
            ulQosGetFirstLcNode (ulQosStrategyLCTree_gp[internalCellIndex][NON_VICTIM_UE]);
        /*CA Changes end  */
    }

    /*  ICIC Changes Starts  */
    while((0 < numAvailableRBs[CC_CE_REGION]) &&
            (NULL != ulQosStrategyLCNode_p))
        /*  ICIC Changes Ends  */
    { 
        LOG_UT(MAC_UL_QOS_STRATEGY_RETRAVERSING,LOGDEBUG,MAC_UL_Strategy,
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
                "Minimum number of RBs are not consumed by UL Qos Strategy");

        ueIndex = ulQosStrategyLCNode_p->ueId;
        ueULContextInfo_p   = &ulUECtxInfoArr_g[ueIndex];
        /* SPR 22539 Fix + */
#ifdef TDD_CONFIG
        scheduledBytes = ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx];
#else
        scheduledBytes = ulQosStrategyLCNode_p->scheduledBytes;
#endif
        /* SPR 22539 Fix - */
        /* SPR 4751 changes */
        ueDLContextInfo_p   = &dlUECtxInfoArr_g[ueIndex];
        /* SPR 4751 changes */
        if(!(ueULContextInfo_p->pendingDeleteFlag))
        {
            /* Get UE Context */
            ueULContext_p       = ueULContextInfo_p->ulUEContext_p;

            if(PNULL == ueULContext_p)
            {
                LOG_MAC_MSG(MAC_UE_CONTEXT_IS_NULL_IN_UL_LOG_ID,LOGFATAL,MAC_UL_Strategy,
                        getCurrentTick(),
                        ueIndex,ulQosStrategyLCNode_p->lchId,
                        __LINE__,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
                        "Ue Index is null and a node is present in Tree");

                /*Get the next node from the tree*/
                ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)ulQosGetNextLcNode(
                        &ulQosStrategyLCNode_p->lcTreeNodeAnchor);
                continue;
            }
            else
            {
                /* SPR 4751 changes */
                if(UL_SYNC_LOSS_ON == checkUlSyncLossForUE(ueDLContextInfo_p->dlUEContext_p,
                            currentGlobalTTITickCount,
                            internalCellIndex))
                {
                    ulQosStrategyLCNode_p = (ULQosStrategyLCNode *)
                        ulQosGetNextLcNode(&ulQosStrategyLCNode_p->lcTreeNodeAnchor);
                    continue;
                }
                /* SPR 4751 changes */
            }




            ueLocationType = ueULContext_p ->userLocationType;
#ifdef TDD_CONFIG
    /* SPR 23393 Changes Start */
            UInt32 subframeNum = 0;
            /* +- SPR 17777 */
            GET_SFN_SF_FROM_TTI(currentGlobalTTITickCount, sysFrameNum, subframeNum)
    /* SPR 23393 Changes End */
            /* Get the free ID in ulHarqProcessId 
               if all the harq id is busy the skip this node and goto the next node in 
               the list and start this procedure again.
             */
              LTE_GCC_UNUSED_PARAM(sysFrameNum)
            ulHarqProcessId = getFreeULHarqId(ueULContext_p,subframeNum,internalCellIndex,
					ulSubFrameNum, currentGlobalTTITickCount);
#endif
#ifdef FDD_CONFIG
            if ( ueULContext_p->ttiBundlingEnabled )
            {
                ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
            }
#endif
#ifdef TDD_CONFIG
            if (BUSY_ALL_HARQ == ulHarqProcessId) 
#elif FDD_CONFIG
            if((HARQ_PROCESS_FREE != ueULContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                    ||(ueULContext_p->ttiBundlingEnabled &&
                        (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,ueULContext_p)))
                    ||(!(ueULContext_p->ttiBundlingEnabled)&&
                        ((TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId1]) ||
                         ( TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId2]))) )
#endif
            {
                /*Get the next node from the tree*/
                ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)
                    ulQosGetNextLcNode(
                            &ulQosStrategyLCNode_p->lcTreeNodeAnchor);
                continue;
            }

            if((QOS_GBR == ulQosStrategyLCNode_p->channelType ||
                        SRB == ulQosStrategyLCNode_p->channelType))
            {
                lcInfo_p   = ulQosStrategyLCNode_p->lcInfo_p;
                lcgId      = lcInfo_p->lcGId;
                /* SPR 22539 Fix + */
                availToken = lcInfo_p->ulQosInfo.availLcToken;
                /* SPR 22539 Fix - */
            }
            else
            {
                ambrInfo_p = ulQosStrategyLCNode_p->ulAmbrInfo_p;
                lcgId      = ambrInfo_p->lcgId;
                /* SPR 22539 Fix + */
                availToken = ambrInfo_p->availAmbrToken;
                /* SPR 22539 Fix - */
            }
            /* SPS_CHG */
            /*! \fn MacRetType ulQosReTraversePriorityLcList(SInt16  *numberOfAvailableRBs_p,
             *                               UInt32 currentGlobalTTITickCount,
             *                               SInt8  *numUlUEsToBeScheduled)
             *  \brief Function Modification
             * */

            if( ( SPS_ACTIVATED == ueULContext_p->spsUlInfo.spsState ) &&
                    (FALSE == ueULContext_p->lcInfo[ulQosStrategyLCNode_p->lchId].isSpsLc ))
            {
                /*! \code
                 * Handling of strictSpsResourceUsageForSpsLc for Qos 
                 * If (UE State == SPS_ACTIVATED) and 
                 *    (Current TTI correspond to SPS Occasion)
                 * \endcode
                 */
                UInt8 ulSpsInterval = 0;
                LP_SpsCommIntervalProfile commSpsIntervalProfile_p =PNULL;
                ulSpsInterval = ueULContext_p->spsUlInfo.spsUlSetupInfo_p->\
                                semiPersistentSchedIntervalUL;
                commSpsIntervalProfile_p = 
                    &cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    initParams_p->spsCommIntervalProfile[ulSpsInterval];
                if(commSpsIntervalProfile_p->strictSpsResourceUsageForSpsLc)
                {
                    /*! \code
                     * If strictSpsResourceUsageForSpsLc is TRUE, 
                     *      Do not multiplex SPS LC with any other LC
                     * Else, do the normal processing
                     * \endcode
                     */
                    if( SPS_SCHEDULING == ueULContext_p->schType )
                    {
                        ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)ulQosGetNextLcNode (&ulQosStrategyLCNode_p->lcTreeNodeAnchor);
                        continue ;
                    }
                }
            }
            else if( SPS_SCHEDULING == ueULContext_p->schType ) 
            {
                ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)ulQosGetNextLcNode (&ulQosStrategyLCNode_p->lcTreeNodeAnchor);
                continue ;
            }

            if(PNULL == ueULContext_p->ulStrategyTxNode_p)
            {
                if(*numUlUEsToBeScheduled_p)
                {
                    if ((!numAvailableRBs[ueLocationType]) ||
                            ((CE_USER == ueLocationType)
                             && ((numAvailableRBs[CE_REGION]<=unusedULStrategyBufferRB_g[internalCellIndex] )||
                             (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] ))))
                    {
                        /*Get the next node from the tree*/
                        ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)ulQosGetNextLcNode(
                                &ulQosStrategyLCNode_p->lcTreeNodeAnchor);
                        continue;

                    }
                    /*UE is getting scheduled for the first time*/
#ifdef FDD_CONFIG
                    if( ueULContext_p->ttiBundlingEnabled )
                    {
                        if (( availableRBForTtiBundlingPerTick_g[internalCellIndex] >= MIN_TTI_BUNDLE_RB ) 
                                && (MAC_SUCCESS == processMeasGapValidation(
                                        ueULContext_p,currentGlobalTTITickCount,ulDelay )))
                        {
                            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-Strategy][%s] bundled Ue [%d] is  scheduled.\n",
                                    __func__, ueULContext_p->ueIndex);
                        }    
                        else
                        {    
                            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-QOS-Strategy][%s] bundled Ue [%d] not  scheduled.\n",
                                    __func__, ueULContext_p->ueIndex);
                            /*Get the next node from the tree*/
                            ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)ulQosGetNextLcNode(
                                    &ulQosStrategyLCNode_p->lcTreeNodeAnchor);
                            continue;
                        }
                    }
#endif
                    ulStrategyTxNode_p =
                        ulQosStrategyUpdateNewTxUEList(ueIndex,
                                currentGlobalTTITickCount,
                                UL_RETRAVERSAL
#ifdef TDD_CONFIG
                                ,ulSubFrameNum
#endif
                                );
                    if(PNULL != ulStrategyTxNode_p)
                    {
                        ueULContext_p->ulStrategyTxNode_p = ulStrategyTxNode_p;
                        --(*numUlUEsToBeScheduled_p);

                        if (CE_USER == ueLocationType )
                        {
                            --maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex];
                        }
                    }
                    else
                    {
                        LOG_MAC_MSG(MAC_UE_CANT_BE_SCHEDULED,LOGFATAL,MAC_UL_Strategy,
                                getCurrentTick(),
                                ueULContext_p->ueIndex,
#ifdef TDD_CONFIG
                                sysFrameNum,
#else
                                0,
#endif
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME, "Cannot schedule this UE");
                        ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)ulQosGetNextLcNode(
                                &ulQosStrategyLCNode_p->lcTreeNodeAnchor);

                        continue;
                    }
                }
                else
                {
                    LOG_UT(MAC_NUM_UE_PER_TICK_REACHED_IN_UL_LOG_ID,LOGDEBUG,MAC_UL_Strategy,
                            getCurrentTick(),
                            ueULContext_p->ueIndex,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME, "No more UEs can be scheduled");

                    ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)ulQosGetNextLcNode(
                            &ulQosStrategyLCNode_p->lcTreeNodeAnchor);

                    continue;

                }
            }
            else
            {
                /*UE has already been scheduled */
                ulQosProcessUeForRBRestriction(UL_RETRAVERSAL,
                        ueULContext_p);

                ulStrategyTxNode_p = ueULContext_p->ulStrategyTxNode_p;
                if ( CQI_WITHOUT_DATA_ULGRANT_REQ_MSG == ulStrategyTxNode_p->ulGrantRequestType )
                {
                    ulStrategyTxNode_p->ulGrantRequestType = CQI_WITH_DATA_ULGRANT_REQ_MSG;
#ifdef FDD_CONFIG
                    if( ueULContext_p->ttiBundlingEnabled )
                    {
                        if ( ueULContext_p->mcsIndex > MAX_TTI_BUNDLE_MCS )
                        {
                            ulStrategyTxNode_p->avgMcs = MAX_TTI_BUNDLE_MCS;
                        }
                        else
                        {
                            ulStrategyTxNode_p->avgMcs = ueULContext_p->mcsIndex;
                        }
                    }
                    else
                    {
                        ulStrategyTxNode_p->avgMcs = ueULContext_p->mcsIndex;
                    }
#endif
                }
            }
            /* SPR 22539 Fix + */
            if (ENABLE_ALLOW_ALLOC_BITRATE_TOKEN_BASED == allowAllocBitRate)
            {
                if(ulStrategyTxNode_p->maxDataSize < ueULContext_p->maxTBSize)
                {
                    /* Obtain the schedulable bytes equal to
                     * First Re Tx PDU Qload of the LC */
                    netQueueLoad = 0;
                    netAvlblTkns = 0;

                    /* TDD Config 0 Changes Start */
                    if ( ueULContext_p->ulLCGInfo[lcgId].qLoad  > scheduledBytes )
                    {
                        /* SPR 14371 Fix Starts */
                        netQueueLoad = ueULContext_p->ulLCGInfo[lcgId].qLoad /* - scheduledBytes*/;
                        /* SPR 14371 Fix Ends  */
                    }

                    if ( availToken  > scheduledBytes )
                    {
                        netAvlblTkns = availToken  - scheduledBytes;
                    }
                    /* TDD Config 0 Changes End */

                    reSchedulableBytes =  (netAvlblTkns > netQueueLoad ) ? netQueueLoad:netAvlblTkns;
                    //fprintf(stderr,"reSchedulableBytes %d netAvlblTkns %d netQueueLoad %d TTI %llu\n",reSchedulableBytes,netAvlblTkns,netQueueLoad,getCurrentTick());
                }

            }
            else
            {

            if(ueULContext_p->ulLCGInfo[lcgId].qLoad && 
                    (ulStrategyTxNode_p->maxDataSize < ueULContext_p->maxTBSize))
            {
                /* Obtain the schedulable bytes equal to
                 * First Re Tx PDU Qload of the LC
                 */
                /*Remaining Qload can be reScheduled*/
                reSchedulableBytes = ueULContext_p->ulLCGInfo[lcgId].qLoad; 
                }
            }
            if (reSchedulableBytes)
            {
                if((ulStrategyTxNode_p->maxDataSize + reSchedulableBytes) >
                        ueULContext_p->maxTBSize)
                {
                    reSchedulableBytes = 
                        ueULContext_p->maxTBSize - 
                        ulStrategyTxNode_p->maxDataSize;
#ifdef PERF_STATS
                    if ( ueULContext_p->maxtbSizebyUeCategory < ueULContext_p->maxRestrictedTB )
                    {
                        /*  number of ms where respective UE is limited in the
                         *  UL direction by its UE capability */
                        gMacUePerfStats_p[ueIndex].lteUeMacSchedULPerfStats.\
                            totalTimeRestrictUeInUlbyUeCap++;
                    }
#endif
                }

                if(reSchedulableBytes > 0)
                {

                    ueULContext_p->ulLCGInfo[lcgId].qLoad -= 
                        reSchedulableBytes;

                    /* Increment the Max TB Size and  Strategy LC node
                     * with the schedulable bytes for the LC
                     */
                    ulStrategyTxNode_p->maxDataSize += reSchedulableBytes;

                    tempTBSize = ulStrategyTxNode_p->maxDataSize;
#ifdef FDD_CONFIG 
                    if ( ueULContext_p->ttiBundlingEnabled )
                    {
                        /*CA Changes start  */
                        requiredRB = DETERMINE_RBs_UL(ulStrategyTxNode_p->avgMcs,tempTBSize, internalCellIndex);    
                        /*CA Changes end  */
                    }
                    else
#endif
                    {
                        /*CA Changes start  */
                        requiredRB = DETERMINE_RBs_UL(ueULContext_p->mcsIndex,tempTBSize, internalCellIndex);
                        /*CA Changes end  */
                    }
                    /* SPR 5999 changes end */

                    /* SPR 10032 Fix Starts*/
                    regionPowerMask = ulPowerMaskInfo_p->powerMask[ueULContext_p->userLocationType];
                    /* validate allocated Rb from ATB */
                    CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ueULContext_p,regionPowerMask);

                    /* ATB Changes Start */
                    /*SPR 16417 Changes Start*/
                    /* SPR 13532 Fix Start */
#ifdef FDD_CONFIG
                    powerCorrectionAndTpc = getTpcForPusch(ueULContext_p,
                            currentGlobalTTITickCount +
                            ulDelay,internalCellIndex);
#elif TDD_CONFIG
                    powerCorrectionAndTpc = getTpcForPusch(ueULContext_p,
                            currentGlobalTTITickCount,
                            ulSubFrameNum,internalCellIndex);
#endif
                    /*SPR 16417 Changes End*/

                    /* Coverity_73273 Fix Start */
                    if(0 < requiredRB)
                    {
                        requiredRB = validateATB(requiredRB,&mcsIndex,ueULContext_p,
                                powerCorrectionAndTpc.powerCorrectionVal,
                                &ueMaxPowerFlag,&ueMinPowerFlag,
                                regionPowerMask,NEW_TX,
                                internalCellIndex);
                    }   
                    /* SPR 13532 Fix End */
                    /* Coverity_73273 Fix End */

                    /* ATB Changes End */

                    /* SPR 10032 Fix Ends*/
#ifdef FDD_CONFIG
                    /* TTIB_Code Start */
                    if( ueULContext_p->ttiBundlingEnabled )
                    {
                        if( requiredRB > MAX_TTI_BUNDLE_RB )
                        {
                            requiredRB = MAX_TTI_BUNDLE_RB;
                        }
                        /* Coverity_73273 Fix Start */
                        if(0 < requiredRB)
                        {
                            ulStrategyTxNode_p->maxDataSize = determineUlTBSize(ulStrategyTxNode_p->avgMcs,requiredRB);
                        }   
                        /* Coverity_73273 Fix End */

                        ulStrategyTxNode_p->minDataSize = determineUlTBSize(ulStrategyTxNode_p->avgMcs,MIN_TTI_BUNDLE_RB);
                    }
                    /* TTIB_Code End */
#endif

                    deltaRB = requiredRB - 
                        ulStrategyTxNode_p->requiredRB;

                    /* If scheduling for 100 RB.s is not crossed */
                    /*  ICIC Changes Starts  */
                    if( numAvailableRBs[ueLocationType] > unusedULStrategyBufferRB_g[internalCellIndex])
                        /*  ICIC Changes Ends  */
                    {
                        /*QOS ICIC: 31 Aug 2012 fix*/
                        tempRB = numAvailableRBs[ueLocationType] - 
                            unusedULStrategyBufferRB_g[internalCellIndex];

                        if(tempRB >= deltaRB)
                        {
                            /* TTIB_Code Start */

                            /*SPR_5796 Start*/
                            /*ulStrategyTxNode_p->minDataSize += schedulableBytes;*/
                            /*SPR_5796 End*/
#ifdef FDD_CONFIG
                            if ( ueULContext_p->ttiBundlingEnabled )
                            {
                                /*CA Changes start  */
                                if ( (SInt64)availableRBForTtiBundlingPerTick_g[internalCellIndex] >=
                                        (requiredRB - ulStrategyTxNode_p->requiredRB))
                                {
                                    ulStrategyTxNode_p->requiredRB = requiredRB;
                                    availableRBForTtiBundlingPerTick_g[internalCellIndex] -= 
                                        (requiredRB - ulStrategyTxNode_p->requiredRB);
                                }
                                /*CA Changes end  */
                            }
                            else
#endif
                            {
                                ulStrategyTxNode_p->minDataSize += reSchedulableBytes;
                                ulStrategyTxNode_p->requiredRB = requiredRB;
                            }
                            /* update the RB.s required for the Data scheduled 
                            */

                            /* TTIB_Code End */
                        }
                        else
                        {
                            /* TTIB_Code Start */
#ifdef FDD_CONFIG
                            if( ueULContext_p->ttiBundlingEnabled )
                            {
                                if ( (ulStrategyTxNode_p->requiredRB + tempRB) > MAX_TTI_BUNDLE_RB)
                                {
                                    tempRB = (MAX_TTI_BUNDLE_RB - ulStrategyTxNode_p->requiredRB);
                                }
                                /*CA Changes start  */
                                if ((SInt64)availableRBForTtiBundlingPerTick_g[internalCellIndex] >= tempRB)
                                {
                                    ulStrategyTxNode_p->requiredRB += tempRB;
                                    availableRBForTtiBundlingPerTick_g[internalCellIndex] -= tempRB;
                                }
                                /*CA Changes end  */
                            }
                            else
#endif
                                /* TTIB_Code End */
                            {
                                ulStrategyTxNode_p->minDataSize += 
                                    determineTBSize( 
                                            ueULContext_p->mcsIndex,
                                            tempRB,
                                            /*4x4 DL MIMO CHG START*/
                                            TB_MAPPED_TO_ONE_LAYER   
                                            );/*4x4 DL MIMO CHG END*/

                                ulStrategyTxNode_p->requiredRB += tempRB;
                            }
                            if(deltaRB - tempRB > unusedULStrategyBufferRB_g[internalCellIndex])
                            {
                                unusedULStrategyBufferRB_g[internalCellIndex] = 0;
                                deltaRB = numAvailableRBs[ueLocationType]; 
                            }
                            else
                            {
                                unusedULStrategyBufferRB_g[internalCellIndex] -= (deltaRB - tempRB);
                            }

                        }

                        /*QOS ICIC fix*/
                    }
                    else 
                    {   
                        unusedULStrategyBufferRB_g[internalCellIndex] = 0; 
                        deltaRB = numAvailableRBs[ueLocationType];
                    }  
#ifdef FDD_CONFIG
                    /* TTIB_Code Start */
                    if( ueULContext_p->ttiBundlingEnabled )
                    {
                        if( ulStrategyTxNode_p->requiredRB > MAX_TTI_BUNDLE_RB )
                        {
                            ulStrategyTxNode_p->requiredRB = MAX_TTI_BUNDLE_RB;
                        }
                    }
                    /* TTIB_Code End */
#endif
                    /*  ICIC Changes Starts  */
                    LOG_UT(MAC_RB_RETRAVERSAL_INFO_BEFORE_ALLOCATE_RB_IN_UL,LOGDEBUG,
                            MAC_UL_Strategy,
                            getCurrentTick(),
                            numAvailableRBs[CC_CE_REGION],
                            numAvailableRBs[CE_REGION],
                            ueLocationType,
                            deltaRB,
                            reSchedulableBytes,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME, "RB info \
                            for Qos Schduling");


                    numAvailableRBs[ueLocationType] -= deltaRB;

                    /* Updating RB After Allocation */
                    if ( ueLocationType == CC_USER )   
                    { 
                        if (numAvailableRBs[CC_CE_REGION] <  numAvailableRBs[CE_REGION])
                        {
                            numAvailableRBs[CE_REGION] = numAvailableRBs[CC_CE_REGION];
                        }
                    }
                    else                  
                    {
                        numAvailableRBs[CC_CE_REGION] -= deltaRB;
                    }

                    /* SPR 14371 Fix Starts */
                    numberOfAvailableRBs_p[CE_REGION] =  numAvailableRBs[CE_REGION] ;
                    numberOfAvailableRBs_p[CC_CE_REGION] = numAvailableRBs[CC_CE_REGION];
                    /* SPR 14371 Fix Ends */


                    LOG_UT(MAC_RB_RETRAVERSAL_INFO_AFTER_ALLOCATE_RB_IN_UL,LOGDEBUG,
                            MAC_UL_Strategy,
                            getCurrentTick(),
                            numAvailableRBs[CC_CE_REGION],
                            numAvailableRBs[CE_REGION],
                            ueLocationType,
                            deltaRB,
                            ueIndex,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME, "RB info \
                            for Qos Schduling");
                    /*  ICIC Changes Ends  */

                    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                    ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx] += reSchedulableBytes;
#else
                    ulQosStrategyLCNode_p->scheduledBytes += reSchedulableBytes;
#endif
                    /* TDD Config 0 Changes End */
                    if (FALSE == ulQosStrategyLCNode_p->isLCScheduled )
                    {
#ifdef TDD_CONFIG
                        insertLcNodeFlag = TRUE;
                        /* Check if current subframe is TDD config 0, S subframe */
                        if (LTE_TRUE == isTddConfig0SsubFrame(subFrameNum,internalCellIndex))
                        {
                            /* If TDD Config S subframe then no need to add extra LC Node 
                             * if already added */
                            /* SPR 23393 Changes Start */
                            ulSubFrame2ndIdx = MODULO_TWO(ulSubFrameIdx + 1);
                            /* SPR 23393 Changes End */
                            if ( ulQosStrategyLCNode_p->scheduledBytes[ulSubFrame2ndIdx])
                            {
                                insertLcNodeFlag = FALSE;
                            }
                        }
                        if (TRUE == insertLcNodeFlag)
                        {
                            /* Insert the Lc Node in the scheduled LC list */
                            listInsertNode(&ueULContext_p->ulScheduledLcList,
                                    &ulQosStrategyLCNode_p->scheduledLcNodeAnchor);
                        }
#else
                        /* Insert the Lc Node in the scheduled LC list */
                        listInsertNode(&ueULContext_p->ulScheduledLcList,
                                &ulQosStrategyLCNode_p->scheduledLcNodeAnchor);
#endif

                        ++ueULContext_p->scheduledLcCount;

                        /* CA Stats Changes Start */
                        /*debug stats*/
                        LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_SCHEDULED_BYTES(reSchedulableBytes,
                                internalCellIndex);

                        /*debug stats*/
                        LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_AVAILABLE_TOKEN(
                                availToken, internalCellIndex);

                        /*debug stats*/
                        LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_BSR_LOAD(ueULContext_p->bsrNetTotal,
                                internalCellIndex);

                        /*debug stats*/
                        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_LC_SCHEDULED_PER_TTI_ABSOLUTE_TTI
                            (ueIndex,getCurrentTick());

                        LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_UE_ID_LC_ID(
                                ueIndex,(ulQosStrategyLCNode_p->lchId), 
                                internalCellIndex);

                        /*debug stats*/
                        LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_ABSOLUTE_TTI(getCurrentTick(), 
                                internalCellIndex);
                        /* CA Stats Changes End */


                        /*debug stats*/
                        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_LC_SCHEDULED_PER_TTI_LC_INDEX
                            (ueIndex,ulQosStrategyLCNode_p->lchId);

                        /* CA Stats Changes Start */
                        /*debug
                         * stats*/
                        LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_TOTAL_LC_WEIGHT(ulQosStrategyLCNode_p->totLCWght, 
                                internalCellIndex);
                        /* CA Stats Changes End */


                        /*debug stats*/
                        LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_SCH_COUNT(ueIndex);
                        ulQosStrategyLCNode_p->isLCScheduled = TRUE;
                    }

                    /*debug stats*/
                    /* CA Stats Changes Start */
                    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_RESCHEDULED_BYTES(
                            ueULContext_p->ueIndex,
                            ulQosStrategyLCNode_p->lchId,
                            reSchedulableBytes, internalCellIndex);
                    /* CA Stats Changes End */
                }
            }
            /* SPR 22539 Fix - */
        }
        else
        {
            LOG_UT(MAC_UE_DELETE_PENDING_IN_UL_LOG_ID,LOGDEBUG,
                    MAC_UL_Strategy,
                    getCurrentTick(),
                    ueIndex,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
        }

        ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)ulQosGetNextLcNode(
                &ulQosStrategyLCNode_p->lcTreeNodeAnchor);

    }/*End of while*/
}

/*****************************************************************************
 * Function Name  : ulQosStrategyUpdateNewTxUEList
 * Inputs         : ueIndex - Index of UE,
 *                  currentGlobalTTITickCount - Global TTI Tick,
 *                  traversalType - Type of traversal
 *                  ulSubFrameNum - ulSubFrameNumber for which Grant is being
 *                  given.
 * Outputs        : None
 * Returns        : Strategic UE Tx Node
 * Description    : This function fills the new Scheduled UE Tx Node.
 *****************************************************************************/
/*QOS_CYCLIC_COMPLEXITY_CHG*/
ULStrategyTxNode * ulQosStrategyUpdateNewTxUEList(
        UInt16 ueIndex,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        ulQosTraversalType traversalType
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
/*QOS_CYCLIC_COMPLEXITY_CHG*/
{

    InternalCellIndex internalCellIndex = \
                                          ulUECtxInfoArr_g[ueIndex].ulUEContext_p->internalCellIndex;
#ifdef TDD_CONFIG
    UInt8 ulHarqProcessId = BUSY_ALL_HARQ ;
    /* Get the free ID in ulHarqProcessId
       if all the harq id is busy the skip this node and goto the next node in
       the list and start this procedure again.
       */
    /* TDD Config 0 and 6 Changes Start */
    /* SPR 23393 Changes Start */
    UInt32 subframe    = 0;
    UInt32 sysFrameNum = 0;
    GET_SFN_SF_FROM_TTI(currentGlobalTTITickCount, sysFrameNum, subframe)
    /* SPR 23393 Changes End */
        /* +- SPR 17777 */
        LTE_GCC_UNUSED_PARAM(sysFrameNum)
        /* +- SPR 17777 */
        /* Get the free ID in ulHarqProcessId
           if all the harq id is busy the skip this node and goto the next node in
           the list and start this procedure again.
           */
        /* SPR 11257 Changes Start*/    
        ulHarqProcessId = getFreeULHarqId(ulUECtxInfoArr_g[ueIndex].ulUEContext_p,
                /* +- SPR 17777 */
                subframe,internalCellIndex,ulSubFrameNum, currentGlobalTTITickCount);
    /* +- SPR 17777 */
    /* SPR 11257 Changes End*/    
    /* TDD Config 0 and 6 Changes End */
    /* SPR 4844 starts */
    if(BUSY_ALL_HARQ == ulHarqProcessId)
    {
        return PNULL;
    }
    /* SPR 4844 ends */
#endif

    ULStrategyTxNode *ulStrategyTxNode_p = PNULL;

    ULUeScheduleInfo *ulUeScheduleInfo_p = PNULL;

    GET_MEM_FROM_POOL(ULStrategyTxNode,ulStrategyTxNode_p,
            sizeof(ULStrategyTxNode),PNULL);

    /* + coverity 24548 */
    if (PNULL == ulStrategyTxNode_p)
    {
        return PNULL;
    }
    /* - coverity 24548 */

    /*initialising the resource allocator input parameters*/

#ifdef FDD_CONFIG
    if( ulUECtxInfoArr_g[ueIndex].ulUEContext_p->ttiBundlingEnabled )
    {
        ulStrategyTxNode_p->ulGrantRequestType = NEW_TTIB_ULGRANT_REQ_MSG;
        if ( ulUECtxInfoArr_g[ueIndex].ulUEContext_p->mcsIndex >
                MAX_TTI_BUNDLE_MCS )
        {
            ulStrategyTxNode_p->avgMcs = MAX_TTI_BUNDLE_MCS;
        }
        else
        {
            ulStrategyTxNode_p->avgMcs = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->mcsIndex;
        }
    }
    else 
#endif
    {
        ulStrategyTxNode_p->ulGrantRequestType = NEW_ULGRANT_REQ_MSG;
        ulStrategyTxNode_p->avgMcs             =
            ulUECtxInfoArr_g[ueIndex].ulUEContext_p->mcsIndex;
    }
    ulStrategyTxNode_p->ttiCounter         = currentGlobalTTITickCount;
    ulStrategyTxNode_p->ueIndex            = ueIndex;
    ulStrategyTxNode_p->requiredRB         = 0;
    ulStrategyTxNode_p->strictAllocRB      = FALSE;
    ulStrategyTxNode_p->minDataSize          = 0;
    ulStrategyTxNode_p->maxDataSize          = 0;

#ifdef TDD_CONFIG
    ulStrategyTxNode_p->ulHarqProcessId   = ulHarqProcessId;
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
        ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

    /* Increament the scheduled UE count */
    /*CA Changes start  */
    ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount++;
    /*CA Changes end  */

    /*QOS_CYCLIC_COMPLEXITY_CHG*/
    ulQosProcessUeForRBRestriction(traversalType,
            ulUeScheduleInfo_p->ueULContext_p);
    /*QOS_CYCLIC_COMPLEXITY_CHG*/
    return ulStrategyTxNode_p;

}
/*****************************************************************************
 * Function Name  : ulQosStrategyProcessDRXTrigQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the DRX queue ON/OFF trigger queue 
 *****************************************************************************/
/*CA Changes start  */
void ulQosStrategyProcessDRXTrigQueue(InternalCellIndex  internalCellIndex,ExecutionLegFunctionArg* execArgs)
    /*CA Changes end  */
{

    UInt16                  maxDrxTriggerCount      = 0;
    UInt32                  ueIndex                 = 0; 
    ULUEContext             *ueULContext_p          = PNULL;
    ULUEContextInfo         *ueULContextInfo_p      = PNULL;
    ULQosStrategyLCNode     *ulEligibleLcNode_p     = PNULL;
    DRXOnOffTriggerNode     *drxTrigNode_p          = PNULL;
    void                    *ulEligibleLcNodeVoid_p = PNULL; 
    ULScheduleFlag          prevScheduleFlag; 
    /*QOS-DRX related changes*/
    /* coverity-530 CID-32339*/ 
    ulQosStrategyEvent      event = UL_QOS_EVENT_DRX_OFF;
    /* coverity-530 CID-32339*/ 
    /*QOS-DRX related changes*/

    /*
     * 1.   Pop the node from drxSMTriggerQueue_g  queue
     * 2.   if the Trigger is DRX_OFF
     *      a.   For each eligible LC node of  the UE
     *      b.   Remove the strategy  LC Node From the tree 
     *           ulQosDeleteEntryFromPrioritisedLcTree(ulQosStrategyLCNode_p)
     *          i.   Calculate the LC node PDB priority with effective delay 
     *               as mentioned below 
     *               DrxOnDurationDelay = next short/long DRX cycle duration
     *               PPDB = ulQosGetPdbPriority(lcNode_p, DrxOnDurationDelay)
     *          ii.  If channelType == GBR
     *               Call ulQosCalcGbrLCWeightAndInsert(
     *                       ueCtx_p,lcId, drxOnDurationDelay);
     *               Else
     *               Call ulQosCalcNgbrLCWeightAndInsert (ueCtx_p,drxOnDurationDelay);
     * 3.   if the Trigger is DRX_ON
     *      a.   Remove the LC nodes from the prioritized LC list, 
     *      ulQosDeleteEntryFromPrioritisedLcTree(ulQosStrategyLCNode_p)
     *
     */
    /*CA Changes start  */
    maxDrxTriggerCount = COUNT_UL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex);
    /*CA Changes end  */
    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Entry:[UL-Strategy][%s]:"
            "DRX Queue Count:%d",__func__,
            maxDrxTriggerCount);

    while (maxDrxTriggerCount)
    {
        /*CA Changes start  */
        DEQUEUE_UL_DRX_ON_OFF_TRIGGER_Q(DRXOnOffTriggerNode,(void *)&(drxTrigNode_p), internalCellIndex);
        /*CA Changes end  */
        if (drxTrigNode_p)
        {
            ueIndex = drxTrigNode_p->ueIndex;    
            ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            if (!(ueULContextInfo_p->pendingDeleteFlag))
            {
                ueULContext_p = ueULContextInfo_p->ulUEContext_p;

                /* EICIC +*/
                /* Pushing UE back in trigger queue.*/
                /* EICIC Optimization +- */
                if(isEicicEnabled_g[internalCellIndex])
                {
                    if ((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == ueULContext_p->userType))
                    {              
                        if(!ENQUEUE_UL_DRX_ON_OFF_TRIGGER_Q(DRXOnOffTriggerNode, 
                                    &(drxTrigNode_p->anchorNode), internalCellIndex))
                        {
                            FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxTrigNode_p);
                        }  

                        maxDrxTriggerCount--;
                        continue;
                    }
                    else if ((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == ueULContext_p->userType))
                    {           
                        if(!ENQUEUE_UL_DRX_ON_OFF_TRIGGER_Q(DRXOnOffTriggerNode, 
                                    &(drxTrigNode_p->anchorNode), internalCellIndex))
                        {
                            FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(drxTrigNode_p);
                        }

                        maxDrxTriggerCount--;			 
                        continue;
                    }
                }
                /* EICIC Optimization +- */

                /* EICIC - */

                prevScheduleFlag = ueULContext_p->scheduleFlag;

                /*START : 5352*/
                LOG_MAC_MSG(MAC_UL_DRX_TRIGGER_INFO,LOGDEBUG,
                        MAC_UL_Strategy, getCurrentTick(),
                        ueULContext_p->ueIndex, drxTrigNode_p->onOffTrigger,
                        DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        __func__,"DRX_TRIGGER_INFO_RECIEVED_INFO");
                /*END   : 5352*/


                /*QOS-DRX related changes*/
                if (DRX_ON_TO_DRX_OFF == drxTrigNode_p->onOffTrigger)
                {
                    event = UL_QOS_EVENT_DRX_OFF;
                }
                /* SPR_3244_Fix */
                else if (DRX_OFF_TO_DRX_ON == drxTrigNode_p->onOffTrigger)
                    /* SPR_3244_Fix */
                {
                    event = UL_QOS_EVENT_DRX_ON;
                }
                else
                {
                    FREE_MEM_NODE_UL_DRX_ON_OFF_TRIGGER_Q(drxTrigNode_p);
                    maxDrxTriggerCount--;
                    continue;
                }

                UL_QOS_UPDATE_SCHEDULE_FLAG(&(ueULContext_p->scheduleFlag),
                        /*CA Changes start  */
                        event, internalCellIndex);
                /*CA Changes end  */
                /*QOS-DRX related changes*/

                /*debug stats*/ 
                LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SCHEDULE_FLAG(
                        ueULContext_p->ueIndex,ueULContext_p->scheduleFlag);

                /*Checking if the trigger is for DRX-ON/DRX-OFF */
                /*QOS-DRX related changes*/
                if (DRX_ON_TO_DRX_OFF == drxTrigNode_p->onOffTrigger)
                    /*QOS-DRX related changes*/
                {
                    if (UL_QOS_NEW_TRANSMISSION == ueULContext_p->scheduleFlag)
                    {

                        /* If DRX-OFF : remove the nodes from the tree and 
                         * calculate their PDB priority with the on-duration 
                         * delay and calculate the LC weight & insert the node 
                         * back into the tree
                         */
                        ulEligibleLcNodeVoid_p = (void*)getFirstListNode(
                                &ueULContext_p->ulEligibleLcList);



                        while(PNULL != ulEligibleLcNodeVoid_p)
                        {

                            UL_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                                    ulEligibleLcNodeVoid_p, ulEligibleLcNode_p);

                            LOG_UT(MAC_LC_PRIORITY_BEFORE_ONDURATON_DELAY_CONSIDERED_IN_UL_LOG_ID,LOGDEBUG,
                                    MAC_UL_Strategy,getCurrentTick(),
                                    ulEligibleLcNode_p->totLCWght,
                                    ueIndex,ulEligibleLcNode_p->lchId,
                                    drxTrigNode_p->onOffTrigger,DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"");

                            /* Delete the strategy Node From the tree */
                            if(UL_QOS_NEW_TRANSMISSION == prevScheduleFlag)
                            {
                                ulQosDeleteEntryFromPrioritisedLcTree(
                                        ulEligibleLcNode_p,
                                        /*CA Changes start  */
                                        internalCellIndex);
                                /*CA Changes end  */
                            }
                            /* Calculate the LC node PDB priority 
                             * with effective delay 
                             * The effective delay will be passed into the 
                             * ulQosCalcGbrLCWeightAndInsert and PDB will be calclated 
                             * inside the function with effective delay
                             */

                            /* Calculate the new weight and Insert the node */
                            if (ulEligibleLcNode_p->channelType == QOS_GBR)
                            {
                                ulQosCalcGbrLCWeightAndInsert(ueULContext_p,
                                        ulEligibleLcNode_p->lchId,
                                        drxTrigNode_p->nextOnDurationTTI,
                                        /*CA Changes start  */
                                        internalCellIndex);
                                /*CA Changes end  */
                            }
                            /*QOS-DRX related changes*/
                            else if (ulEligibleLcNode_p->channelType == NON_GBR)
                            {
                                ulQosCalcNgbrLCWeightAndInsert(
                                        ueULContext_p,
                                        drxTrigNode_p->nextOnDurationTTI,
                                        /*CA Changes start  */
                                        internalCellIndex,
                                        execArgs);
                                /*CA Changes end  */
                            }
                            else
                            {
                                ueULContext_p->lcInfo[ulEligibleLcNode_p->lchId].
                                    ulQosInfo.availLcToken = 
                                    ueULContext_p->ulLCGInfo[ulEligibleLcNode_p->lcgId].qLoad;
                                ueULContext_p->lcInfo[ulEligibleLcNode_p->lchId].ulQosInfo.
                                    headOfLinePktDelay = getCurrentTick();
                                ueULContext_p->lcInfo[ulEligibleLcNode_p->lchId].ulQosInfo.
                                    ulQosStrategyLCNode_p->totLCWght = 
                                    ueULContext_p->
                                    ulLCGInfo[ulEligibleLcNode_p->lcgId].srbWeight;
                                /* SPR 23402 Changes Start */
                                 /* MOCN Changes Start */
                                if (mocnOperatorParams_g.isMocnEnabled)
                                {
                                    ueULContext_p->lcInfo[ulEligibleLcNode_p->lchId].ulQosInfo.
                                                       ulQosStrategyLCNode_p->totLCWght += 
                                                        mocnOperatorParams_g.prbUsageSRBPriorityFactor;
                                }
                                /* MOCN Changes End */
                                /* SPR 23402 Changes End */
                                /* Insert it in the priortised LC list 
                                */
                                /* SPR_3244_Fix */
                                if(FALSE == ueULContext_p->lcInfo[ulEligibleLcNode_p->lchId].ulQosInfo.ulQosStrategyLCNode_p->isLCNodePresent)
                                {
                                    ulQosPutEntryInPrioritisedLcTree (
                                            ueULContext_p->lcInfo[ulEligibleLcNode_p->lchId].ulQosInfo.
                                            ulQosStrategyLCNode_p,
                                            /*CA Changes start  */
                                            internalCellIndex);
                                    /*CA Changes end  */
                                    ueULContext_p->lcInfo[ulEligibleLcNode_p->lchId].ulQosInfo.ulQosStrategyLCNode_p->isLCNodePresent =TRUE;
                                }
                                /* SPR_3244_Fix */


                            }

                            /*QOS-DRX related changes*/
                            /* CA Stats Changes Start */
                            /*debug stats*/
                            LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_DRX_ON_DURATION_DLY(
                                    drxTrigNode_p->nextOnDurationTTI, 
                                    internalCellIndex);
                            /* CA Stats Changes End */

                            /*Get next node from the list*/
                            ulEligibleLcNodeVoid_p = 
                                (void*)getNextListNode
                                (&ulEligibleLcNode_p->eligibleLcNodeAnchor);

                        }
                    }
                    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_OFF_UL_COUNT(
                            ueULContext_p->ueIndex);
                }/*end of if DRX_OFF check */
                /*QOS-DRX related changes*/
                else if(DRX_OFF_TO_DRX_ON == drxTrigNode_p->onOffTrigger)
                    /*QOS-DRX related changes*/
                {
                    if ((UL_QOS_DRX_ON == ueULContext_p->scheduleFlag) &&
                            (UL_QOS_NEW_TRANSMISSION == prevScheduleFlag))
                    {
                        /* If DRX-ON : reomove the nodes from the tree */ 
                        ulEligibleLcNodeVoid_p = (void*)getFirstListNode(
                                &ueULContext_p->ulEligibleLcList);


                        while(PNULL != ulEligibleLcNodeVoid_p)
                        {

                            UL_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                                    ulEligibleLcNodeVoid_p, ulEligibleLcNode_p);

                            /* Delete th strategy Node From the tree */

                            /* SPR_3244_Fix */
                            if(QOS_GBR == ulEligibleLcNode_p->channelType)
                            {
                                if(TRUE == ulEligibleLcNode_p->isLCNodePresent)
                                {
                                    ulQosDeleteEntryFromPrioritisedLcTree(
                                            ulEligibleLcNode_p,
                                            /*CA Changes start  */
                                            internalCellIndex);
                                    /*CA Changes end  */
                                    ulEligibleLcNode_p->isLCNodePresent = FALSE;
                                }
                            }
                            else if(NON_GBR == ulEligibleLcNode_p->channelType)
                            {
                                if( TRUE == ueULContext_p->ulAmbrInfo.isLCNodePresent)
                                {
                                    ulQosDeleteEntryFromPrioritisedLcTree(
                                            ulEligibleLcNode_p,
                                            /*CA Changes start  */
                                            internalCellIndex);
                                    /*CA Changes end  */
                                    ueULContext_p->ulAmbrInfo.isLCNodePresent = FALSE;

                                }

                            }
                            else
                            {
                                if(TRUE == ulEligibleLcNode_p->isLCNodePresent)
                                {

                                    ulQosDeleteEntryFromPrioritisedLcTree(
                                            ulEligibleLcNode_p,
                                            /*CA Changes start  */
                                            internalCellIndex);
                                    /*CA Changes end  */
                                    ulEligibleLcNode_p->isLCNodePresent = FALSE;
                                }
                            }

                            /* SPR_3244_Fix */

                            /*debug stats*/
                            LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(
                                    ueULContext_p->ueIndex);
                            /*Get next node from the list*/
                            ulEligibleLcNodeVoid_p = (void*)getNextListNode(
                                    &ulEligibleLcNode_p->eligibleLcNodeAnchor);

                        }
                    }
                    /*debug stats*/
                    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_ON_UL_COUNT(
                            ueULContext_p->ueIndex);
                }
            }
            else
            { 
                LOG_UT(MAC_UE_DELETE_PENDING_IN_UL_LOG_ID,LOGDEBUG,
                        MAC_UL_Strategy,
                        getCurrentTick(),
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

            FREE_MEM_NODE_UL_DRX_ON_OFF_TRIGGER_Q(drxTrigNode_p);
            maxDrxTriggerCount--;
        }
        else
        {
            LOG_UT(MAC_QUEUE_IS_EMPTY_IN_UL_LOG_ID,LOGDEBUG,
                    MAC_UL_Strategy,
                    getCurrentTick(),
                    maxDrxTriggerCount,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"DRX Queue");
            break;
        }
    }

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]"
            ,__func__);
}/* End of function ulQosStrategyProcesDRXTrigQueue */

/*****************************************************************************
 * Function Name  : ulQoSDeleteStrNode
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function deletes the strategy Node From the tree.
 *****************************************************************************/
void ulQoSDeleteStrNode( ULUEContext *ueULContext_p, 
        ULQosStrategyLCNode * ulEligibleLcNode_p, 
        InternalCellIndex internalCellIndex )
{
    /* SPR_3244_Fix */
    if(QOS_GBR == ulEligibleLcNode_p->channelType)
    {
        if(TRUE == ulEligibleLcNode_p->isLCNodePresent)
        {
            ulQosDeleteEntryFromPrioritisedLcTree(
                    ulEligibleLcNode_p,
                    internalCellIndex);
            ulEligibleLcNode_p->isLCNodePresent = FALSE;
        }
    }
    else if(NON_GBR == ulEligibleLcNode_p->channelType)
    {
        if( TRUE == ueULContext_p->ulAmbrInfo.isLCNodePresent)
        {
            ulQosDeleteEntryFromPrioritisedLcTree(
                    ulEligibleLcNode_p,
                    internalCellIndex);
            ueULContext_p->ulAmbrInfo.isLCNodePresent = FALSE;
        }
    }
    else
    {
        if(TRUE == ulEligibleLcNode_p->isLCNodePresent)
        {
            ulQosDeleteEntryFromPrioritisedLcTree(
                    ulEligibleLcNode_p,internalCellIndex);
            ulEligibleLcNode_p->isLCNodePresent = FALSE;
        }
    }
    /* SPR_3244_Fix */
}

/*****************************************************************************
 * Function Name  : ulQosStrategyProcessMeasGapTrigQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the Measurement GAP ON/OFF trigger queue 
 *****************************************************************************/
void ulQosStrategyProcessMeasGapTrigQueue(InternalCellIndex internalCellIndex,ExecutionLegFunctionArg* execArgs)
{
    UInt16                  measGapTriggerCount     = 0;
    UInt32                  ueIndex                 = 0; 

    ULUEContextInfo         *ueULContextInfo_p      = PNULL;
    ULUEContext             *ueULContext_p          = PNULL;    
    ULQosStrategyLCNode     *ulEligibleLcNode_p     = PNULL;
    /* SPR 3185 : measGapTrigNode_p declared as MeasGapOnOffTriggerNode*  instead of MeasGapTriggerQueueNode*/
    MeasGapOnOffTriggerNode *measGapTrigNode_p      = PNULL;
    void                    *ulEligibleLcNodeVoid_p = PNULL;
    ULScheduleFlag          prevScheduleFlag;
    /* SPR_3244_Fix */
    /* coverity-530 CID-32337*/ 
    ulQosStrategyEvent      event = UL_QOS_EVENT_DRX_OFF;
    /* coverity-530 CID-32337*/ 
    /* SPR_3244_Fix */

    /*
     * 1.   Pop the node from measGapTriggerQueue_g  queue
     * 2.   if the Trigger is MEAS_GAP_OFF
     *      a.   For each eligible LC node of  the UE
     *      b.   Remove the strategy  LC Node From the tree, 
     *           ulQosDeleteEntryFromPrioritisedLcTree(ulQosStrategyLCNode_p)
     *           i.   Calculate the LC node weight, 
     *           ii.      If channelType == GBR
     *                   Call ulQosCalcGbrLCWeightAndInsert 
     *                        (ueCtx_p,lcId, drxOnDurationDelay);
     *                   Else
     *                   Call ulQosCalcNgbrLCWeightAndInsert 
     *                        (ueCtx_p,drxOnDurationDelay);
     *
     * 3.   if the Trigger is MEAS_GAP_ON
     *      a.   Remove the LC nodes from the prioritized LC list, 
     *           ulQosDeleteEntryFromPrioritisedLcTree(ulQosStrategyLCNode_p)
     *
     */


    /*CA Changes start  */
    measGapTriggerCount = COUNT_UL_MEAS_GAPON_OFF_TRIGGER_Q(internalCellIndex);
    /*CA Changes end  */

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Entry:[UL-Strategy][%s]:"
            "Meas Gap Trigger Queue Count:%d",__func__,
            measGapTriggerCount);

    while (measGapTriggerCount)
    {
        /* SPR 3185 : Typecasted from (MeasGapTriggerQueueNode*) to (MeasGapOnOffTriggerNode*) */
        /*CA Changes start  */
        DEQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode, (void *)&measGapTrigNode_p, internalCellIndex);
        /*CA Changes end  */

        if (measGapTrigNode_p)
        {
            ueIndex = measGapTrigNode_p->ueIndex;    
            ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            if (!(ueULContextInfo_p->pendingDeleteFlag))
            {
                ueULContext_p = ueULContextInfo_p->ulUEContext_p;

                /* EICIC +*/
                /* EICIC Optimization +- */
                if(isEicicEnabled_g[internalCellIndex])
                {
                    /* Pushing UE back in trigger queue.*/
                    if ((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == ueULContext_p->userType))
                    {              
                        if(!ENQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
                                    &(measGapTrigNode_p->anchorNode), internalCellIndex))
                        {
                            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapTrigNode_p);
                        }
                        measGapTriggerCount--;
                        continue;
                    }

                    else if ((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == ueULContext_p->userType))
                    {           
                        if(!ENQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
                                    &(measGapTrigNode_p->anchorNode), internalCellIndex))
                        {
                            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapTrigNode_p);
                        }
                        measGapTriggerCount--;
                        continue;
                    }
                }
                /* EICIC - */
                /* EICIC Optimization +- */

                prevScheduleFlag = ueULContext_p->scheduleFlag;

                /* SPR_3244_Fix */
                if (MEASUREMENT_PERIOD_OFF_UL == measGapTrigNode_p->onOffTrigger)
                {
                    event = UL_QOS_EVENT_MEAS_GAP_OFF;
                }
                else if (MEASUREMENT_PERIOD_ON_UL == measGapTrigNode_p->onOffTrigger)
                {
                    event = UL_QOS_EVENT_MEAS_GAP_ON;
                    /* Review comment fix start GG10 */
                    LOG_MAC_MSG(MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_UL_LOG_ID
                            ,LOGBRIEF, MAC_UL_Strategy, 
                            getCurrentTick(), ueULContext_p->ueIndex, 
                            measGapTrigNode_p->onOffTrigger,
                            DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                            DEFAULT_FLOAT_VALUE, __func__,"MEASUREMENT_PERIOD_ON_UL");
                    /* Review comment fix start GG10 */
                }
                else
                {
                    FREE_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(measGapTrigNode_p);
                    measGapTriggerCount--;
                    continue;
                }

                UL_QOS_UPDATE_SCHEDULE_FLAG(&(ueULContext_p->scheduleFlag),
                        /*CA Changes start  */
                        event, internalCellIndex);
                /*CA Changes end  */
                /* SPR_3244_Fix */

                /*debug stats*/ 
                LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SCHEDULE_FLAG(
                        ueULContext_p->ueIndex,ueULContext_p->scheduleFlag);

                /*Checking if Already being considered for RB Distribution*/
                /* SPR_3244_Fix */
                if (MEASUREMENT_PERIOD_OFF_UL == measGapTrigNode_p->onOffTrigger)
                    /* SPR_3244_Fix */
                {
                    if (UL_QOS_NEW_TRANSMISSION == ueULContext_p->scheduleFlag)
                    {

                        ulEligibleLcNodeVoid_p = (void*)getFirstListNode(
                                &ueULContext_p->ulEligibleLcList);

                        while(PNULL != ulEligibleLcNodeVoid_p)
                        {

                            UL_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                                    ulEligibleLcNodeVoid_p, ulEligibleLcNode_p);

                            LOG_UT(MAC_LC_PRIORITY_BEFORE_ONDURATON_DELAY_CONSIDERED_IN_UL_LOG_ID,LOGDEBUG,
                                    MAC_UL_Strategy,getCurrentTick(),
                                    ulEligibleLcNode_p->totLCWght,
                                    ueIndex,ulEligibleLcNode_p->lchId,
                                    measGapTrigNode_p->onOffTrigger,DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"");

                            /* Calculate the LC node PDB priority 
                             * with effective delay 
                             * The effective delay will be passed into the 
                             * ulQosCalcGbrLCWeightAndInsert and PDB will be calclated 
                             * inside the function with effective delay
                             */

                            if (ulEligibleLcNode_p->channelType == QOS_GBR)
                            {
                                ulQosCalcGbrLCWeightAndInsert(ueULContext_p,
                                        ulEligibleLcNode_p->lchId,
                                        /*CA Changes start  */
                                        0, internalCellIndex);
                                /*CA Changes end  */
                            }
                            /* SPR 14027 fix start */
                            else if ( ulEligibleLcNode_p->channelType == NON_GBR)
                            {
                                ulQosCalcNgbrLCWeightAndInsert(
                                        ueULContext_p,
                                        /*CA Changes start  */
                                        0, internalCellIndex,execArgs);
                                /*CA Changes end  */
                            }
                            else /* channel Type = SRB */
                            {
                                ueULContext_p->lcInfo[ulEligibleLcNode_p->lchId].
                                    ulQosInfo.availLcToken =
                                    ueULContext_p->ulLCGInfo[ulEligibleLcNode_p->lcgId].qLoad;

                                ulEligibleLcNode_p->totLCWght =
                                    ueULContext_p->ulLCGInfo[ulEligibleLcNode_p->lcgId].srbWeight;

                                /* Insert it in the priortised LC list */
                                if ( FALSE == ulEligibleLcNode_p->isLCNodePresent )
                                {
                                    ulQosPutEntryInPrioritisedLcTree ( ulEligibleLcNode_p,
                                            /*CA Changes start  */
                                            internalCellIndex);
                                    /*CA Changes end  */
                                    ulEligibleLcNode_p->isLCNodePresent = TRUE;
                                }
                            }
                            /* SPR 14027 fix end */

                            /*Get next node from the list*/
                            ulEligibleLcNodeVoid_p = 
                                (void*)getNextListNode
                                (&ulEligibleLcNode_p->eligibleLcNodeAnchor);

                        }
                    }
                    /*debug stats*/
                    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_OFF_UL_COUNT(
                            ueULContext_p->ueIndex);
                }/*end of if DRX_OFF check */
                /* SPR_3244_Fix */
                else if(MEASUREMENT_PERIOD_ON_UL == measGapTrigNode_p->onOffTrigger)
                    /* SPR_3244_Fix */
                {
                    if ((UL_QOS_MEAS_GAP_ON == ueULContext_p->scheduleFlag) &&
                        /* + SPR 21539 fix*/
			    (UL_QOS_NO_TRANSMISSION < prevScheduleFlag))
                        /* - SPR 21539 fix*/
                    {
                        ulEligibleLcNodeVoid_p = (void*)getFirstListNode(
                                &ueULContext_p->ulEligibleLcList);


                        while(PNULL != ulEligibleLcNodeVoid_p)
                        {
                            UL_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                                    ulEligibleLcNodeVoid_p, ulEligibleLcNode_p);

                            /* Delete the strategy Node From the tree */
                            ulQoSDeleteStrNode( ueULContext_p, ulEligibleLcNode_p,
                                    internalCellIndex );

                            /*debug stats*/
                            LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(
                                    ueULContext_p->ueIndex);
                            /*Get next node from the list*/
                            ulEligibleLcNodeVoid_p = (void*)getNextListNode(
                                    &ulEligibleLcNode_p->eligibleLcNodeAnchor);

                        }
                    }
                    /*debug stats*/
                    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_ON_UL_COUNT(
                            ueULContext_p->ueIndex);
                }
            }
            else
            {
                LOG_UT(MAC_UE_DELETE_PENDING_IN_UL_LOG_ID,LOGDEBUG,
                        MAC_UL_Strategy,
                        getCurrentTick(),
                        ueIndex,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
            }
            /* Also free the memory of Meas Gap Nodes*/

            FREE_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(measGapTrigNode_p);
            measGapTriggerCount--;
        }
        else
        {
            LOG_UT(MAC_QUEUE_IS_EMPTY_IN_UL_LOG_ID,LOGDEBUG,
                    MAC_UL_Strategy,
                    getCurrentTick(),
                    measGapTriggerCount,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"Meas gap Queue");
            break;
        }
    }

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]"
            ,__func__);
}/* End of function ulQosStrategyProcesMeasGapTrigQueue */

/*****************************************************************************
 * Function Name  : ulQosStrategyProcessPdcchOrderQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function Process the PDDCH order Trigger queue 
 *****************************************************************************/
void ulQosStrategyProcessPdcchOrderQueue(InternalCellIndex internalCellIndex,ExecutionLegFunctionArg* execArgs)
{
    UInt16                  pdcchOrderCount         = 0;           
    UInt32                  ueIndex                 = 0; 
    ULUEContextInfo         *ueULContextInfo_p      = PNULL;
    ULUEContext             *ueULContext_p          = PNULL;    
    ULQosStrategyLCNode     *ulEligibleLcNode_p     = PNULL;
    ULSyncLossQueueNode     *ulSyncLossNode_p       = PNULL;
    void                    *ulEligibleLcNodeVoid_p = PNULL;
    ULScheduleFlag          prevScheduleFlag;
    ulQosStrategyEvent event;
    /*
     * 1.   Pop the node from ULSyncLossTriggerQUL_g  queue
     * 2.   For each UE
     *      a.   For each eligible LC node of  the UE
     *          i.  remove the LC nodes from the prioritized LC list, 
     *          ulQosDeleteEntryFromPrioritisedLcTree(ulQosStrategyLCNode_p)
     *
     */
    /* FDD_Warning_Fix*/
    /*CA Changes start  */
    pdcchOrderCount = COUNT_UL_SYNC_LOSS_TRIGGERQ_UL_Q(internalCellIndex);
    /*CA Changes end  */
    /* FDD_Warning_Fix*/

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Entry:[UL-Strategy][%s]:"
            "PDCCH Order Count:%d",__func__,
            pdcchOrderCount);

    while (pdcchOrderCount)
    {
        /*FDD_Warning_Fix */
        /*CA Changes start  */
        DEQUEUE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ULSyncLossQueueNode, (void *)&ulSyncLossNode_p, internalCellIndex);
        /*CA Changes end  */
        /*FDD_Warning_Fix */

        if (ulSyncLossNode_p)
        {
            ueIndex = ulSyncLossNode_p->ueIndex;    
            ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            if (!(ueULContextInfo_p->pendingDeleteFlag))
            {
                ueULContext_p = ueULContextInfo_p->ulUEContext_p;
                /* EICIC +*/
                /* EICIC Optimization +- */
                if(isEicicEnabled_g[internalCellIndex])
                {

                    /* Pushing UE back in queue.*/                   
                    if ((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == ueULContext_p->userType))
                    {              
                        if(!ENQUEUE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ULSyncLossQueueNode,
                                    &(ulSyncLossNode_p->anchorNode), internalCellIndex))
                        {
                            FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossNode_p);
                        }

                        pdcchOrderCount--;   
                        continue;
                    }

                    else if ((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == ueULContext_p->userType))
                    {           
                        if(!ENQUEUE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ULSyncLossQueueNode,
                                    &(ulSyncLossNode_p->anchorNode), internalCellIndex))
                        {
                            FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossNode_p);
                        }

                        pdcchOrderCount--;   
                        continue;
                    }
                }
                /* EICIC - */
                /* EICIC Optimization +- */


                prevScheduleFlag = ueULContext_p->scheduleFlag;
                if(UL_SYNC_LOSS_OFF == ulSyncLossNode_p->onOffTrigger)
                {
                    event = UL_QOS_EVENT_SYNC_LOSS_OFF;
                }
                else if(UL_SYNC_LOSS_ON == ulSyncLossNode_p->onOffTrigger)
                {
                    event = UL_QOS_EVENT_SYNC_LOSS_ON;
                }
                else
                {
                    FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossNode_p);
                    pdcchOrderCount--;
                    continue;
                }
                /*CA Changes start  */
                UL_QOS_UPDATE_SCHEDULE_FLAG(&(ueULContext_p->scheduleFlag),event, internalCellIndex);
                /*CA Changes end  */
                /*debug stats*/ 
                LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SCHEDULE_FLAG(
                        ueULContext_p->ueIndex,ueULContext_p->scheduleFlag);

                if (UL_SYNC_LOSS_OFF == ulSyncLossNode_p->onOffTrigger)
                {
                    if (UL_QOS_NEW_TRANSMISSION == ueULContext_p->scheduleFlag)
                    {
                        /*  For each eligible LC of  the UE
                         *  remove the LC nodes from the prioritized LC list
                         */                 


                        ulEligibleLcNodeVoid_p = (void*)getFirstListNode(
                                &ueULContext_p->ulEligibleLcList);

                        while(PNULL != ulEligibleLcNodeVoid_p)
                        {

                            UL_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                                    ulEligibleLcNodeVoid_p, ulEligibleLcNode_p);

                            LOG_UT(MAC_LC_PRIORITY_BEFORE_ONDURATON_DELAY_CONSIDERED_IN_UL_LOG_ID,LOGDEBUG,
                                    MAC_UL_Strategy,getCurrentTick(),
                                    ulEligibleLcNode_p->totLCWght,
                                    ueIndex,ulEligibleLcNode_p->lchId,
                                    ulSyncLossNode_p->onOffTrigger,DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"");

                            /* Calculate the LC node PDB priority 
                             *  with effective delay 
                             * The effective delay will be passed into the 
                             * ulQosCalcGbrLCWeightAndInsert and PDB will be calclated 
                             * inside the function with effective delay
                             */

                            if (ulEligibleLcNode_p->channelType == QOS_GBR)
                            {
                                ulQosCalcGbrLCWeightAndInsert(ueULContext_p,
                                        ulEligibleLcNode_p->lchId,
                                        /*CA Changes start  */
                                        0, internalCellIndex);
                                /*CA Changes end  */
                            }
                            /* SPR 14027 fix start */
                            else if ( ulEligibleLcNode_p->channelType == NON_GBR )
                            {
                                ulQosCalcNgbrLCWeightAndInsert(
                                        ueULContext_p,
                                        /*CA Changes start  */
                                        0, internalCellIndex,execArgs);
                                /*CA Changes end  */
                            }
                            else /* channel Type = SRB */
                            {
                                ueULContext_p->lcInfo[ulEligibleLcNode_p->lchId].
                                    ulQosInfo.availLcToken =
                                    ueULContext_p->ulLCGInfo[ulEligibleLcNode_p->lcgId].qLoad;

                                ulEligibleLcNode_p->totLCWght =
                                    ueULContext_p->ulLCGInfo[ulEligibleLcNode_p->lcgId].srbWeight;

                                /* Insert it in the priortised LC list */

                                if ( FALSE == ulEligibleLcNode_p->isLCNodePresent )
                                {
                                    ulQosPutEntryInPrioritisedLcTree (ulEligibleLcNode_p,
                                            /*CA Changes start  */
                                            internalCellIndex);
                                    /*CA Changes end  */
                                    ulEligibleLcNode_p->isLCNodePresent = TRUE;
                                }
                            }
                            /* SPR 14027 fix end */

                            /*Get next node from the list*/
                            ulEligibleLcNodeVoid_p = 
                                (void*)getNextListNode
                                (&ulEligibleLcNode_p->eligibleLcNodeAnchor);

                        }
                    }
                    /* SPR 8485 FIX START */  
                    ueULContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState = 
                        UL_SYNC_LOSS_OFF;
                    /* SPR 8485 FIX END */  
                    /*debug stats*/
                    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_OFF_UL_COUNT(
                            ueULContext_p->ueIndex);
                }
                else if(UL_SYNC_LOSS_ON == ulSyncLossNode_p->onOffTrigger)
                {
                    if ((UL_QOS_SYNC_LOSS_ON == ueULContext_p->scheduleFlag) &&
                            (UL_QOS_NEW_TRANSMISSION == prevScheduleFlag))
                    {
                        /*Get First node from the list*/
                        ulEligibleLcNodeVoid_p = (void*)getFirstListNode(
                                &ueULContext_p->ulEligibleLcList);


                        while(PNULL != ulEligibleLcNodeVoid_p)
                        {

                            UL_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                                    ulEligibleLcNodeVoid_p, ulEligibleLcNode_p);

                            LOG_UT(MAC_LC_PRIORITY_BEFORE_ONDURATON_DELAY_CONSIDERED_IN_UL_LOG_ID,LOGDEBUG,
                                    MAC_UL_Strategy,getCurrentTick(),
                                    ulEligibleLcNode_p->totLCWght,
                                    ueIndex,ulEligibleLcNode_p->lchId,
                                    ulSyncLossNode_p->onOffTrigger,DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"");


                            /* Delete the strategy Node From the tree */
                            ulQoSDeleteStrNode( ueULContext_p, 
                                    ulEligibleLcNode_p, internalCellIndex );

                            /*debug stats*/
                            LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(
                                    ueULContext_p->ueIndex);
                            /*Get next node from the list*/
                            ulEligibleLcNodeVoid_p = 
                                (void*)getNextListNode
                                (&ulEligibleLcNode_p->eligibleLcNodeAnchor);

                        }

                    }
                    /* SPR 8485 FIX START */
                    /* SPR 7451 Fix start */
                    ueULContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState = 
                        UL_SYNC_LOSS_ON;
                    /* SPR 7451 Fix end */
                    /* SPR 8485 FIX END */
                    /*debug stats*/
                    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_ON_UL_COUNT(
                            ueULContext_p->ueIndex);
                }
            }
            else
            {
                LOG_UT(MAC_UE_DELETE_PENDING_IN_UL_LOG_ID,LOGDEBUG,
                        MAC_UL_Strategy,
                        getCurrentTick(),
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

            FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(ulSyncLossNode_p);
            --pdcchOrderCount;
        }
        else
        {
            LOG_UT(MAC_QUEUE_IS_EMPTY_IN_UL_LOG_ID,LOGDEBUG,
                    MAC_UL_Strategy,
                    getCurrentTick(),
                    pdcchOrderCount,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"UL Sync Loss Queue");
            break;
        }
    }

    LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"Exit:[UL-Strategy][%s]"
            ,__func__);
}/* End of function ulQosStrategyProcesDRXTrigQueue */


/*****************************************************************************
 * Function Name  : initUlQosStrategyScheduleSM
 * Inputs         : numOfCells - number of cells
 * Outputs        : None 
 * Returns        : None 
 * Description    : This function initialises the state machine for scheduling 
 *                  UEs.  
 *****************************************************************************/
/*CA Changes start  */
void initUlQosStrategyScheduleSM(UInt8 numOfCells)
{
    UInt8 cellIdx = 0;

    for (cellIdx = 0; cellIdx < numOfCells; cellIdx++)
    {        
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_DRX_OFF]= UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_DRX_ON ]= UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_MEAS_GAP_OFF]= UL_QOS_DRX_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_MEAS_GAP_ON]= UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_SYNC_LOSS_OFF]= UL_QOS_DRX_ON_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_SYNC_LOSS_ON]= UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON;

        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON][UL_QOS_EVENT_DRX_OFF]= UL_QOS_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON][UL_QOS_EVENT_DRX_ON]= UL_QOS_DRX_ON_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON][UL_QOS_EVENT_MEAS_GAP_OFF]= UL_QOS_DRX_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON][UL_QOS_EVENT_MEAS_GAP_ON]= UL_QOS_DRX_ON_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON][UL_QOS_EVENT_SYNC_LOSS_OFF]= UL_QOS_DRX_ON_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_MEAS_GAP_ON][UL_QOS_EVENT_SYNC_LOSS_ON]= UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON;

        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_SYNC_LOSS_ON][UL_QOS_EVENT_DRX_OFF]= UL_QOS_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_SYNC_LOSS_ON][UL_QOS_EVENT_DRX_ON]= UL_QOS_DRX_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_SYNC_LOSS_ON][UL_QOS_EVENT_MEAS_GAP_OFF]= UL_QOS_DRX_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_SYNC_LOSS_ON][UL_QOS_EVENT_MEAS_GAP_ON]= UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_SYNC_LOSS_ON][UL_QOS_EVENT_SYNC_LOSS_OFF]= UL_QOS_DRX_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON_SYNC_LOSS_ON][UL_QOS_EVENT_SYNC_LOSS_ON]= UL_QOS_DRX_ON_SYNC_LOSS_ON;

        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_DRX_OFF]= UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_DRX_ON]= UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_MEAS_GAP_OFF]= UL_QOS_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_MEAS_GAP_ON]= UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_SYNC_LOSS_OFF]= UL_QOS_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON][UL_QOS_EVENT_SYNC_LOSS_ON]= UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON;

        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON][UL_QOS_EVENT_DRX_OFF]= UL_QOS_NEW_TRANSMISSION;
        /*SPR 5602 Start*/
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON][UL_QOS_EVENT_DRX_ON] = UL_QOS_DRX_ON;
        /*SPR 5602 End*/
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON][UL_QOS_EVENT_MEAS_GAP_OFF]= UL_QOS_DRX_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON][UL_QOS_EVENT_MEAS_GAP_ON]= UL_QOS_DRX_ON_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON][UL_QOS_EVENT_SYNC_LOSS_OFF]= UL_QOS_DRX_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_DRX_ON][UL_QOS_EVENT_SYNC_LOSS_ON]= UL_QOS_DRX_ON_SYNC_LOSS_ON;

        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON][UL_QOS_EVENT_DRX_OFF]= UL_QOS_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON][UL_QOS_EVENT_DRX_ON]= UL_QOS_DRX_ON_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON][UL_QOS_EVENT_MEAS_GAP_OFF]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON][UL_QOS_EVENT_MEAS_GAP_ON]= UL_QOS_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON][UL_QOS_EVENT_SYNC_LOSS_OFF]= UL_QOS_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_MEAS_GAP_ON][UL_QOS_EVENT_SYNC_LOSS_ON]= UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON;

        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_SYNC_LOSS_ON][UL_QOS_EVENT_DRX_OFF]= UL_QOS_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_SYNC_LOSS_ON][UL_QOS_EVENT_DRX_ON]= UL_QOS_DRX_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_SYNC_LOSS_ON][UL_QOS_EVENT_MEAS_GAP_OFF]= UL_QOS_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_SYNC_LOSS_ON][UL_QOS_EVENT_MEAS_GAP_ON]= UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_SYNC_LOSS_ON][UL_QOS_EVENT_SYNC_LOSS_OFF]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_SYNC_LOSS_ON][UL_QOS_EVENT_SYNC_LOSS_ON]= UL_QOS_SYNC_LOSS_ON;

        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NO_TRANSMISSION][UL_QOS_EVENT_DRX_OFF]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NO_TRANSMISSION][UL_QOS_EVENT_DRX_ON]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NO_TRANSMISSION][UL_QOS_EVENT_MEAS_GAP_OFF]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NO_TRANSMISSION][UL_QOS_EVENT_MEAS_GAP_ON]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NO_TRANSMISSION][UL_QOS_EVENT_SYNC_LOSS_OFF]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NO_TRANSMISSION][UL_QOS_EVENT_SYNC_LOSS_ON]= UL_QOS_NEW_TRANSMISSION;

        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_ALREADY_TRANSMITED][UL_QOS_EVENT_DRX_OFF]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_ALREADY_TRANSMITED][UL_QOS_EVENT_DRX_ON]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_ALREADY_TRANSMITED][UL_QOS_EVENT_MEAS_GAP_OFF]= UL_QOS_NEW_TRANSMISSION;
    	/* + SPR 21539 fix*/
    	ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_ALREADY_TRANSMITED][UL_QOS_EVENT_MEAS_GAP_ON]= UL_QOS_MEAS_GAP_ON;
    	/* - SPR 21539 fix*/
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_ALREADY_TRANSMITED][UL_QOS_EVENT_SYNC_LOSS_OFF]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_ALREADY_TRANSMITED][UL_QOS_EVENT_SYNC_LOSS_ON]= UL_QOS_NEW_TRANSMISSION;

        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NEW_TRANSMISSION][UL_QOS_EVENT_DRX_OFF]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NEW_TRANSMISSION][UL_QOS_EVENT_DRX_ON]= UL_QOS_DRX_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NEW_TRANSMISSION][UL_QOS_EVENT_MEAS_GAP_OFF]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NEW_TRANSMISSION][UL_QOS_EVENT_MEAS_GAP_ON]= UL_QOS_MEAS_GAP_ON;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NEW_TRANSMISSION][UL_QOS_EVENT_SYNC_LOSS_OFF]= UL_QOS_NEW_TRANSMISSION;
        ulQosStrategyScheduleSM_g[cellIdx][UL_QOS_NEW_TRANSMISSION][UL_QOS_EVENT_SYNC_LOSS_ON]= UL_QOS_SYNC_LOSS_ON;

    }
}
/*CA Changes end  */

/*QOS_RB_RESTICTION_CHANGE*/
/* ICIC Change Start */
/*****************************************************************************
 * Function Name  : initUlQosRbRestriction
 * Inputs         : internalCellIndex 
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : 
 *****************************************************************************/
/*CA Changes start  */
void initUlQosRbRestriction(InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    /*CA Changes start  */
    InitParams *initParams_p = cellSpecificParams_g.\
                               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    /* SPR 22275 Fix + */
    UInt8 tempMaxUEsToBeScheduledUL =  MIN_BETWEEN_TWO(totalActiveUEsUL_g[internalCellIndex],(initParams_p->ulSchdConfig.maxUEsToBeScheduledUL));
    tempMaxUEsToBeScheduledUL = ((tempMaxUEsToBeScheduledUL >0)?tempMaxUEsToBeScheduledUL:1) ;

    maxRestrictedRbUL_g[internalCellIndex] = 
        ((ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->totalAvailableRB * 
         initParams_p->ulRBRestrictionFactor) / (tempMaxUEsToBeScheduledUL);
    /* SPR 22275 Fix - */
    if (maxRestrictedRbUL_g[internalCellIndex] > (ulRAInfo_gp[internalCellIndex] + 
                ulRAInfoIndex_g[internalCellIndex])->totalAvailableRB)
    {
        maxRestrictedRbUL_g[internalCellIndex] = (ulRAInfo_gp[internalCellIndex] + 
                ulRAInfoIndex_g[internalCellIndex])->totalAvailableRB;
    }
    /*CA Changes end  */
}
/* ICIC Change End */
/*QOS_RB_RESTICTION_CHANGE*/

/*****************************************************************************
 * Function Name  : ulQosProcessUeForRBRestriction
 * Inputs         : traversalType,
 *                  ueULContext_p - Pointer to ULUEContext
 * Outputs        : None
 * Returns        : None
 * Description    : This functions set the restricted TbSize for a particluar
 *                  UE.
 *****************************************************************************/
void ulQosProcessUeForRBRestriction(
        ulQosTraversalType traversalType,
        ULUEContext        *ueULContext_p)
{
    /* SPR 19600 Changes start */
    UInt32 tbSize = ulUeCategoryInfo_g[ueULContext_p->ueCategory - 1].maxBytesULSCHTb;
    /* SPR 19600 Changes end */

    /*CA Changes start  */
    InternalCellIndex internalCellIndex;
    internalCellIndex = ueULContext_p->internalCellIndex;
    /*CA Changes end  */

#ifdef PERF_STATS
    ueULContext_p->maxtbSizebyUeCategory = tbSize;
#endif

    ueULContext_p->maxRestrictedTB =
        determineUlTBSize(ueULContext_p->mcsIndex,
                /*CA Changes start  */
                maxRestrictedRbUL_g[internalCellIndex]);
    /*CA Changes end  */

    if ( UL_NEW_TRAVERSAL == traversalType )
    {
        if ( tbSize < ueULContext_p->maxRestrictedTB )
        {
            ueULContext_p->maxTBSize = tbSize;
        }
        else
        {
            ueULContext_p->maxTBSize = ueULContext_p->maxRestrictedTB;
        }
    }
    else if (UL_RETRAVERSAL == traversalType )
    {
        ueULContext_p->maxTBSize = tbSize;
    }
}

/*****************************************************************************
 * Function Name  : ulQosValidatePriorityTreeNode
 * Inputs         : ueULContext_p - Pointer to ULUEContext,
 *                  ulQosStrategyLCNode_p - Pointer to ULQosStrategyLCNode
 *                  gbrSchedulingFlag,
 *                  ulHarqProcessId,
 *                  currentGlobalTTITickCount,
 *                  ulDelay,
 *					internalCellIndex
 *                  ulSubFrameNum - ul subframe number for whcih grant
 *                  is being given.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function validate the Uplink priority node in UL 
 *****************************************************************************/
STATIC  MacRetType ulQosValidatePriorityTreeNode(ULUEContext * ueULContext_p,
        ULQosStrategyLCNode * ulQosStrategyLCNode_p,
        UInt8  gbrSchedulingFlag,
        UInt8 * ulHarqProcessId,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
        ,UInt8 ulDelay
#endif
        ,InternalCellIndex internalCellIndex
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )

{
    /* + Coverity 54557 */
    UInt16 ueIndex = 0;
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    /* TDD Config 0 Changes Start */
    UInt16 scheduledBytes = 0;
    /* TDD Config 0 Changes End */

    ueIndex = ulQosStrategyLCNode_p->ueId;
    ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

#ifdef FDD_CONFIG
    *ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
    /* TTIB_Code Start */
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2 = TTI_MAP_FREE;
    ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
    ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET + MAX_HARQ_PROCESS_NB );
    /* TTIB_Code End */
    /* TDD Config 0 Changes Start */
    scheduledBytes = ulQosStrategyLCNode_p->scheduledBytes;
#elif TDD_CONFIG
    /* SPR 23393 Changes Start */
    UInt8 ulSubFrameIdx = MODULO_TWO(ulSubFrameNum);
    /* SPR 23393 Changes End */
    scheduledBytes = ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx];
#endif
    /* TDD Config 0 Changes End */
    if(PNULL == ueULContext_p)
    {
        LOG_MAC_MSG(MAC_UE_CONTEXT_IS_NULL_IN_UL_LOG_ID,LOGFATAL,MAC_UL_Strategy,
                getCurrentTick(),
                ueIndex,ulQosStrategyLCNode_p->lchId,
                __LINE__,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
                "Ue Index is null and a node is present in Tree");
        return MAC_FAILURE;
    }
    else
    {
        /* SPR 3654 FIX Start */
        if(UL_SYNC_LOSS_ON == checkUlSyncLossForUE(ueDLContextInfo_p->dlUEContext_p,
                    currentGlobalTTITickCount,
                    internalCellIndex))
            /* SPR 3654 FIX End */
        {
            return MAC_FAILURE;
        }
    }
    if((TRUE == gbrSchedulingFlag) &&
            /* TDD Config 0 Changes Start */
            (scheduledBytes))
        /* TDD Config 0 Changes Start */
    {
        ltePanic("UL Strategy: Scheduled LC Not Freed\n");
    }

    /*CA Changes start  */
    internalCellIndex = ueULContext_p->internalCellIndex;
    /*CA Changes end  */

    /* Do not schedule UE's whose reTx is occuring */
#ifdef TDD_CONFIG
    *ulHarqProcessId = BUSY_ALL_HARQ;
    /* TDD Config 6 Changes Start */
    /* SPR 23393 Changes Start */
    UInt32 subframeNum = 0;
    UInt32 sysFrameNum = 0;
    GET_SFN_SF_FROM_TTI(currentGlobalTTITickCount, sysFrameNum, subframeNum)
    /* SPR 23393 Changes End */
        /* +- SPR 17777 */
        LTE_GCC_UNUSED_PARAM(sysFrameNum)

        /* Get the free ID in ulHarqProcessId 
           if all the harq id is busy the skip this node and goto the next node in 
           the list and start this procedure again.
           */
        /* SPR 11257 Changes Start*/    
        *ulHarqProcessId = getFreeULHarqId(ueULContext_p,subframeNum,internalCellIndex,
                /* +- SPR 17777 */
                ulSubFrameNum, currentGlobalTTITickCount);
    /* +- SPR 17777 */
    /* SPR 11257 Changes End*/    
    /* TDD Config 0 and 6 Changes End */
#elif FDD_CONFIG
    /* TTIB_Code Start */                
    if ( ueULContext_p->ttiBundlingEnabled )
    {
        *ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
    }
    /* TTIB_Code End */                
#endif 
    /* Coverity 54557 Fix Start */
#ifdef TDD_CONFIG
    if (BUSY_ALL_HARQ == *ulHarqProcessId) 
#elif FDD_CONFIG
    /* TTIB_Code Start */                
    if((HARQ_PROCESS_FREE != ueULContext_p->ulresInfo[*ulHarqProcessId].harqStatus)
         ||(ueULContext_p->ttiBundlingEnabled &&
         (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,ueULContext_p)))
         ||(!(ueULContext_p->ttiBundlingEnabled)&&
          ((TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId1]) ||
           ( TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId2])))
           /* + SPS_TDD_Changes */
           /*This code has been commented as we will not block harq process to be used for SPS only */
           /*SPR 7944 START*/
           /*
              || checkSpsOccasionTti(ueULContext_p,*ulHarqProcessId)
              || checkSpsActTti(ueULContext_p,*ulHarqProcessId)
           */
           /*SPR 7944 END*/
           /* - SPS_TDD_Changes */
      )
    /* TTIB_Code End */  
#endif
    /* Coverity 54557 Fix End */
    {
            return MAC_FAILURE;
    }
    /* End ReTx scheduling prohibit */
    else if((NON_GBR == ulQosStrategyLCNode_p->channelType) &&
            (TRUE == gbrSchedulingFlag))
    {
         return MAC_FAILURE;
    }
    if( ( SPS_ACTIVATED == ueULContext_p->spsUlInfo.spsState ) &&
            (FALSE == ueULContext_p->lcInfo[ulQosStrategyLCNode_p->lchId].isSpsLc ))
    {
        /*! \code
         * Handling of strictSpsResourceUsageForSpsLc for Qos 
         * If (UE State == SPS_ACTIVATED) and 
         *    (Current TTI correspond to SPS Occasion)
         * \endcode
         */
        UInt8 ulSpsInterval = 0;
        LP_SpsCommIntervalProfile commSpsIntervalProfile_p =PNULL;
        ulSpsInterval = ueULContext_p->spsUlInfo.spsUlSetupInfo_p->\
                        semiPersistentSchedIntervalUL;
        commSpsIntervalProfile_p = 
            &cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            initParams_p->spsCommIntervalProfile[ulSpsInterval];

        if(commSpsIntervalProfile_p->strictSpsResourceUsageForSpsLc)
        {
            /*! \code
             * If strictSpsResourceUsageForSpsLc is TRUE, 
             *      Do not multiplex SPS LC with any other LC
             * Else, do the normal processing
             * \endcode
             */
            if( SPS_SCHEDULING == ueULContext_p->schType )
            {
                return MAC_FAILURE;

            }
        }
    }
    else if( SPS_SCHEDULING == ueULContext_p->schType ) 
    {
        return MAC_FAILURE;
    }

    return MAC_SUCCESS;
    /* - Coverity 54557 */
}

/*****************************************************************************
 * Function Name  : ulQosGetNextNodeFromPriorityTree
 * Inputs         : ulQosStrategyLCNode_p - Pointer to ULQosStrategyLCNode,
 *                  gbrSchedulingFlag,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    :This function will get the next from the UL prociority tree 
 *****************************************************************************/
STATIC  void ulQosGetNextNodeFromPriorityTree(ULQosStrategyLCNode ** ulQosStrategyLCNode_p,
        UInt8 * gbrSchedulingFlag,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    /*Get the next node from the tree*/
    *ulQosStrategyLCNode_p = (ULQosStrategyLCNode*)
        ulQosGetNextLcNode(
                &(*ulQosStrategyLCNode_p)->lcTreeNodeAnchor);
    if( (TRUE == *gbrSchedulingFlag) &&
            (NULL == *ulQosStrategyLCNode_p) )
    {	
        if (VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex])
        {

            /*CA Changes start  */
            *ulQosStrategyLCNode_p = (ULQosStrategyLCNode *)
                ulQosGetFirstLcNode (ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE]);
            /*CA Changes end  */
        }
        else if (NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex])
        {
            *ulQosStrategyLCNode_p = (ULQosStrategyLCNode *)
                ulQosGetFirstLcNode (ulQosStrategyLCTree_gp[internalCellIndex][NON_VICTIM_UE]);
        }
        *gbrSchedulingFlag = FALSE;
    }
}

/*****************************************************************************
 * Function Name  : ulQosCheckForOverAllocation
 * Inputs         : ulStrategyTxNode_p - Pointer to ULStrategyTxNode,
 *                  ueULContext_p - Pointer to ULUEContext,
 *                  numAvailableRBs - No. of available RB's,
 *                  requiredRB - required RB's,
 *                  schedulableBytes
 * Outputs        : None
 * Returns        : deltaRB
 * Description    :This function checks the allocation of RB is allocated more 
 *                 for required Data.If so it returns the overallocated RB's 
 *****************************************************************************/
STATIC   SInt16 ulQosCheckForOverAllocation(
        ULStrategyTxNode * ulStrategyTxNode_p,
        ULUEContext *ueULContext_p,
        SInt16  numAvailableRBs,
        SInt16 requiredRB,
        /*SPR 16417 Changes Start*/
        UInt32 schedulableBytes,
#ifdef FDD_CONFIG
        /* SPR 13532 Fix Start */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTickWithULDelay
        /* SPR 13532 Fix End */
#elif TDD_CONFIG
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt8 ulSubFrameNum
#endif
        /*SPR 16417 Changes End*/
        )
{
    /*CA Changes start  */
    InternalCellIndex internalCellIndex;
    internalCellIndex = ueULContext_p->internalCellIndex;
    /*CA Changes end  */
    /* SPR 13532 Fix Start */ 
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    /* SPR 13532 Fix End */ 
    /*QOS ICIC fix*/
    SInt16 tempRB = 0;
    /*QOS ICIC fix*/
    SInt16 deltaRB = 0;
    /* SPR 10032 Fix Starts*/
    UInt8 mcsIndex = 0;
    UInt32 ueMaxPowerFlag = 0;
    UInt32 ueMinPowerFlag = 0;
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + 
        !(ulPowerMaskIndex_g[internalCellIndex]);
    SInt32 regionPowerMask = ulPowerMaskInfo_p->powerMask[ueULContext_p->userLocationType];

    /* validate allocated Rb from ATB */
    CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ueULContext_p,regionPowerMask);

    /* ATB Changes Start */
    /*SPR 16417 Changes Start*/
#ifdef FDD_CONFIG
    /* SPR 13532 Fix Start */
    powerCorrectionAndTpc =
        getTpcForPusch(ueULContext_p,currentGlobalTickWithULDelay,internalCellIndex);
#elif TDD_CONFIG
    powerCorrectionAndTpc =
        getTpcForPusch(ueULContext_p,currentGlobalTick,internalCellIndex,ulSubFrameNum);
#endif
    /*SPR 16417 Changes End*/
    requiredRB = validateATB(requiredRB,&mcsIndex,ueULContext_p,
            powerCorrectionAndTpc.powerCorrectionVal,
            &ueMaxPowerFlag,&ueMinPowerFlag,regionPowerMask,
            NEW_TX,internalCellIndex);
    /* SPR 13532 Fix End */
    /* ATB Changes End */
    /* SPR 10032 Fix Ends*/
    deltaRB = requiredRB -  ulStrategyTxNode_p->requiredRB;

    /* If scheduling for 100 RB.s is not crossed */
    /* ICIC Change  Start */
    if( numAvailableRBs > unusedULStrategyBufferRB_g[internalCellIndex])
        /* ICIC Change End */
    {
        /*QOS ICIC: 31 Aug 2012 fix*/
        tempRB = numAvailableRBs - unusedULStrategyBufferRB_g[internalCellIndex];
        if(tempRB >= deltaRB)
        {
            /* TTIB_Code Start */
#ifdef FDD_CONFIG
            if ( ueULContext_p->ttiBundlingEnabled )
            {
                /*CA Changes start  */
                if ( (SInt64)availableRBForTtiBundlingPerTick_g[internalCellIndex] >=
                        (requiredRB - ulStrategyTxNode_p->requiredRB))
                {
                    ulStrategyTxNode_p->requiredRB = requiredRB;
                    availableRBForTtiBundlingPerTick_g[internalCellIndex] -= 
                        (requiredRB - ulStrategyTxNode_p->requiredRB);
                }
                /*CA Changes end  */
            }
            else
#endif
            {
                ulStrategyTxNode_p->minDataSize += schedulableBytes;
                ulStrategyTxNode_p->requiredRB = requiredRB;
            }
            /* TTIB_Code End */

            /* update the RB.s required for the Data scheduled 
            */
        }
        else
        {
            /* TTIB_Code Start */
#ifdef FDD_CONFIG
            if( ueULContext_p->ttiBundlingEnabled )
            {
                if ( (ulStrategyTxNode_p->requiredRB + tempRB) > MAX_TTI_BUNDLE_RB)
                {
                    tempRB = (MAX_TTI_BUNDLE_RB - ulStrategyTxNode_p->requiredRB);
                }
                /*CA Changes start  */
                if ((SInt64)availableRBForTtiBundlingPerTick_g[internalCellIndex] >= tempRB)
                {
                    ulStrategyTxNode_p->requiredRB += tempRB;
                    availableRBForTtiBundlingPerTick_g[internalCellIndex] -= tempRB;
                }
                /*CA Changes end  */
            }
            else
#endif
            {
                ulStrategyTxNode_p->minDataSize += 
                    determineTBSize( ueULContext_p->mcsIndex, tempRB, 
                            /*4x4 DL MIMO CHG START*/
                            TB_MAPPED_TO_ONE_LAYER);
                /*4x4 DL MIMO CHG END*/

                ulStrategyTxNode_p->requiredRB += tempRB;
            }
            /* TTIB_Code End */
            if(deltaRB - tempRB > unusedULStrategyBufferRB_g[internalCellIndex])
            {
                unusedULStrategyBufferRB_g[internalCellIndex] = 0;
                deltaRB = numAvailableRBs; 
            }
            else
            {
                unusedULStrategyBufferRB_g[internalCellIndex] -= (deltaRB - tempRB);
            }
        }
        LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-QOS-Strategy][%s][%d] bundled Ue [%d] RB recalculated:%d \n",
                getCurrentTick(),__func__, ueULContext_p->ueIndex,ulStrategyTxNode_p->requiredRB);
        /*QOS ICIC fix*/
    }
    else 
    {
        unusedULStrategyBufferRB_g[internalCellIndex] = 0; 
        deltaRB = numAvailableRBs;
    }
    return deltaRB;
}                    

/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : ulQosAdjustScheduledTokensFromPdcchPusch
 * Inputs         : ulUEContext_p - Pointer to ULUEContext,
 *                  ulQosStrategyLCNode_p,
 *                  tempHarqProcessId
 *                  allocatedBytes
 *                  ulSubFrameIdx
 *                  execArgs
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the token and weight for the UL strategic node
 *                  Whose scheduled bytes are reduced at PDCCH/PUSCH
 *                  
 *****************************************************************************/
STATIC void ulQosAdjustScheduledTokensFromPdcchPusch(
        ULUEContext *ulUEContext_p,
        ULQosStrategyLCNode *ulQosStrategyLCNode_p,
        ULHarqInfo *tempULHarqInfo_p,
        UInt16 allocatedBytes,
        UInt8 ulSubFrameIdx,
        ExecutionLegFunctionArg* execArgs,
        InternalCellIndex internalCellIndex)
{
    UInt8  lcGId = ulQosStrategyLCNode_p->lcgId;
    SInt16 allocByteSchByteDiff = 0;
    /* SPR 10984 FIX START */
    UInt16 tempAllocatedBytes = 0;
    /* SPR 10984 FIX END */
    UInt32 prevBsrLcg = 0;

    ULQosInfo*    qosInfo_p        = PNULL;
    ULAmbrInfo*   ulAmbrInfo_p      = PNULL; 

    /* - allocByteSchByteDiff will be 0 if we get the required TB size from PUSCH
     * - allocByteSchByteDiff will be +ve if we get less tahn required TB size 
     *   from PUSCH
     * - As At strategy token are already updated as per scheduledBytes so for the 
     *   later case we need to revert back the updated token */
    allocByteSchByteDiff = ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx] -
        allocatedBytes;
    /* allocByteSchByteDiff less than 0 suggests that the node is inteneded for
     * 2nd UL SF LC  so no handling required */
    if (allocByteSchByteDiff > 0)
    {
        /* SPR 10824 Changes Start */
        if(QOS_GBR == ulQosStrategyLCNode_p->channelType)
        {
            if(TRUE ==  ulQosStrategyLCNode_p->isLCNodePresent)
            { 
                ulQosDeleteEntryFromPrioritisedLcTree(
                        ulQosStrategyLCNode_p,internalCellIndex);
                ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
            }
            qosInfo_p = &ulQosStrategyLCNode_p->lcInfo_p->ulQosInfo; 
            /* 1. As Tokens are already updated based on scheduled byte 
             *    we need to update it with actual allocated byte*/
            /* SPR 10984 FIX START */
            /* If allocatedBytes is less than prevAvailLcToken then need to revert */
            if (allocatedBytes < qosInfo_p->prevAvailLcToken[ulSubFrameIdx])
            {
                if (ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx] > 
                        qosInfo_p->prevAvailLcToken[ulSubFrameIdx])
                {
                    qosInfo_p->availLcToken += 
                        qosInfo_p->prevAvailLcToken[ulSubFrameIdx] - allocatedBytes;
                }
                else
                {    
                    qosInfo_p->availLcToken += allocByteSchByteDiff;
                }
                /* SPR 10984 FIX END */
            }
            /* SPR 10824 Changes End */

            qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd] -= allocByteSchByteDiff;

            /* 2. Calculate Priority*/
            qosInfo_p->tokenPriority
                = ulQosGetTknPriority(
                        qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd],
                        qosInfo_p->lcTknTable,
                        qosInfo_p->totSchdTknInWindow
                        );

            /*Update Head of line packet delay*/
            qosInfo_p->headOfLinePktDelay = 
                execArgs->globalTick;

            /* 3. calculate the weight of the Gbr LC node */
            ulQosCalcGbrLCWeightAndInsert(ulUEContext_p,
                    ulQosStrategyLCNode_p->lchId,
                    0,internalCellIndex); /* DrxOnDurationDelay */        
        }
        /* SPR 10824 Changes Start */
        else if(NON_GBR == ulQosStrategyLCNode_p->channelType)
        {
            /* All NON-GBR LC.s are considered in a single strategic LC, 
             * and the token is updated at AMBR rate. */
            if(TRUE ==  ulQosStrategyLCNode_p->ulAmbrInfo_p->isLCNodePresent)
            { 
                ulQosDeleteEntryFromPrioritisedLcTree(
                        ulQosStrategyLCNode_p,internalCellIndex);
                ulQosStrategyLCNode_p->ulAmbrInfo_p->isLCNodePresent = FALSE;
            }

            ulAmbrInfo_p = ulQosStrategyLCNode_p->ulAmbrInfo_p;

            /* 1. As Tokens are already updated based on scheduled byte 
             *    we need to update it with actual allocated byte*/
            /* SPR 10984 FIX START */
            /* If allocatedBytes is less than availAmbrToken then need to revert */
            if (allocatedBytes < ulAmbrInfo_p->prevAvailAmbrToken[ulSubFrameIdx])
            {
                if (ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx] > 
                        ulAmbrInfo_p->prevAvailAmbrToken[ulSubFrameIdx])
                {
                    ulAmbrInfo_p->availAmbrToken += 
                        ulAmbrInfo_p->prevAvailAmbrToken[ulSubFrameIdx] - allocatedBytes;
                }
                else
                {    
                    ulAmbrInfo_p->availAmbrToken += allocByteSchByteDiff;
                }
                /* SPR 10984 FIX END */
            }
            /* SPR 10824 Changes End */

            ulAmbrInfo_p->scheduledToken[ulAmbrInfo_p->curTknWindowInd] -= allocByteSchByteDiff;

            /*2. Update Head of line packet delay*/
            ulAmbrInfo_p->headOfLinePktDelay = 
                execArgs->globalTick;


            /* 3.calculate the weight of the Ambr node */
            ulQosCalcNgbrLCWeightAndInsert(ulUEContext_p,0,internalCellIndex,execArgs);                         

        }
        else
        {
            if(TRUE ==  ulQosStrategyLCNode_p->isLCNodePresent)
            { 
                ulQosDeleteEntryFromPrioritisedLcTree(
                        ulQosStrategyLCNode_p,internalCellIndex);
                ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
            }
            /* SPR 10984 FIX START */
            if ((FALSE == ulUEContext_p->
                        ulLCGInfo[ulQosStrategyLCNode_p->lcInfo_p->lcGId].isDrbPresent))
            {
                qosInfo_p = &ulQosStrategyLCNode_p->lcInfo_p->ulQosInfo; 

                /* If allocatedBytes is less than prevAvailLcToken then need to revert */
                if (allocatedBytes < qosInfo_p->prevAvailLcToken[ulSubFrameIdx])
                {
                    if (ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx] > 
                            qosInfo_p->prevAvailLcToken[ulSubFrameIdx])
                    {
                        qosInfo_p->availLcToken += 
                            qosInfo_p->prevAvailLcToken[ulSubFrameIdx] - allocatedBytes;
                    }
                    else
                    {    
                        qosInfo_p->availLcToken += allocByteSchByteDiff;
                    }
                }
                /* SPR 10984 FIX END */

                qosInfo_p->ulQosStrategyLCNode_p->totLCWght = 
                    ulUEContext_p->
                    ulLCGInfo[ulQosStrategyLCNode_p->lcInfo_p->lcGId].srbWeight;
                /* Insert it in the priortised LC list 
                */
                if(FALSE == qosInfo_p->ulQosStrategyLCNode_p->isLCNodePresent)
                {
                    ulQosPutEntryInPrioritisedLcTree (
                            qosInfo_p->ulQosStrategyLCNode_p,internalCellIndex);
                    qosInfo_p->ulQosStrategyLCNode_p->isLCNodePresent = TRUE;
                }
            }
        }
        /*Adjust Qload changes start*/
        prevBsrLcg = ulUEContext_p->bsrNet[lcGId];
        ulUEContext_p->bsrNet[lcGId] +=  allocByteSchByteDiff;

        if((0 == prevBsrLcg) && (ulUEContext_p->bsrNet[lcGId]))
        {
            /*BSR has become Zero from nonZero*/
            putEntryInBSRQueue(ulUEContext_p->ueIndex,lcGId,0,ADD_TRIGGER,internalCellIndex);
            /* 32.425 Events code changes start*/
#ifdef KPI_STATS

            /* Enqueue the Ue Id and lcId in the Circular Queue */
            lteMacUpdateKpiStatsIPThpUlOnEnqueue(lcGId,ulUEContext_p->ulLCGInfo,ulUEContext_p->ueIndex,internalCellIndex);/* +- SPR 17404 */
#endif
        }
        /* Adjust the UE Queueload */
        ulUEContext_p->ulLCGInfo[lcGId].qLoad += allocByteSchByteDiff;
    }
    /* SPR 10984 FIX START */
    /* Updating the Harq Information */
    if ((PNULL != tempULHarqInfo_p) && (allocByteSchByteDiff >= 0))
    {

        if((QOS_GBR == ulQosStrategyLCNode_p->channelType)) 
        {
            qosInfo_p = &ulQosStrategyLCNode_p->lcInfo_p->ulQosInfo; 
            /* If allocatedBytes is less than prevAvailLcToken then need to revert */
            if (allocatedBytes < qosInfo_p->prevAvailLcToken[ulSubFrameIdx])
            {
                tempAllocatedBytes = allocatedBytes;
            }
            else
            {
                tempAllocatedBytes = qosInfo_p->prevAvailLcToken[ulSubFrameIdx];
            }
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) 
            tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken = tempAllocatedBytes;
            tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].isLCScheduled = TRUE;
#else
            tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken += tempAllocatedBytes;
            tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].lastScheduledToken = tempAllocatedBytes;
            tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].isLCScheduled++;
#endif
            /*Qload adjustment changes*/
            tempULHarqInfo_p->grantPerLcg[lcGId] += tempAllocatedBytes;
            /*Qload adjustment changes*/
            /* Resetting qosInfo_p->prevAvailLcToken[ulSubFrameIdx] to 0 */
            qosInfo_p->prevAvailLcToken[ulSubFrameIdx] = 0;
        }
        else if((NON_GBR == ulQosStrategyLCNode_p->channelType))
        {
            ulAmbrInfo_p = ulQosStrategyLCNode_p->ulAmbrInfo_p;
            if (allocatedBytes < ulAmbrInfo_p->prevAvailAmbrToken[ulSubFrameIdx])
            {
                tempAllocatedBytes = allocatedBytes;
            }
            else
            {
                tempAllocatedBytes = ulAmbrInfo_p->prevAvailAmbrToken[ulSubFrameIdx];
            }
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) 
            tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken = tempAllocatedBytes;
            tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].isLCScheduled = TRUE;
#else
            tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].scheduledToken += tempAllocatedBytes;
            tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].lastScheduledToken = tempAllocatedBytes;
            tempULHarqInfo_p->ulTokenInfo[ulQosStrategyLCNode_p->lchId].isLCScheduled++;
#endif
            /*Qload adjustment changes*/
            tempULHarqInfo_p->grantPerLcg[lcGId] += tempAllocatedBytes;
            /*Qload adjustment changes*/
            /* Resetting ulAmbrInfo_p->prevAvailAmbrToken[ulSubFrameIdx] to 0 */
            ulAmbrInfo_p->prevAvailAmbrToken[ulSubFrameIdx] = 0;
        }
        else
        {
            qosInfo_p = &ulQosStrategyLCNode_p->lcInfo_p->ulQosInfo; 
            if (allocatedBytes < qosInfo_p->prevAvailLcToken[ulSubFrameIdx])
            {
                tempULHarqInfo_p->grantPerLcg[lcGId]+= allocatedBytes;
            }
            else
            {
                tempULHarqInfo_p->grantPerLcg[lcGId]+= qosInfo_p->prevAvailLcToken[ulSubFrameIdx];
            }
            /* Resetting qosInfo_p->prevAvailLcToken[ulSubFrameIdx] to 0 */
            qosInfo_p->prevAvailLcToken[ulSubFrameIdx] = 0;
        }
    }
    /* SPR 10984 FIX END */
}
/*****************************************************************************
 * Function Name  : ulQosUpdateScheduledTokensFromStrategy
 * Inputs         : ulUEContext_p - Pointer to ULUEContext,
 *                  tbSize,
 *                  tempHarqProcessId
 *                  execArgs 
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the token and weight for each UL strategic node
 *                  called from UL Strategy based on MAX TB SIZE.
 *****************************************************************************/
STATIC void ulQosUpdateScheduledTokensFromStrategy(
        ULUEContext *ulUEContext_p,
        ULQosStrategyLCNode *ulQosStrategyLCNode_p,
        UInt16 allocatedBytes,
        /* SPR 10984 FIX START */
        UInt8 ulSubFrameIdx,
        /* SPR 10984 FIX END */
        ExecutionLegFunctionArg* execArgs,
        InternalCellIndex internalCellIndex)
{
    UInt8  lcGId = ulQosStrategyLCNode_p->lcgId;
    UInt32 prevBsrLcg = 0;

    ULQosInfo*    qosInfo_p        = PNULL;
    ULAmbrInfo*   ulAmbrInfo_p      = PNULL; 

    /* SPR 10984 FIX START */
    if((QOS_GBR == ulQosStrategyLCNode_p->channelType))
    {
        /* SPR 10984 FIX END */
        if(TRUE ==  ulQosStrategyLCNode_p->isLCNodePresent)
        { 
            ulQosDeleteEntryFromPrioritisedLcTree(
                    ulQosStrategyLCNode_p,internalCellIndex);
            ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
        }
        qosInfo_p = &ulQosStrategyLCNode_p->lcInfo_p->ulQosInfo; 
        /* SPR 10984 FIX START */
        /* Storing the previouse availLcToken */
        qosInfo_p->prevAvailLcToken[ulSubFrameIdx] = qosInfo_p->availLcToken;
        /* SPR 10984 FIX END */

        /* 1. Update Tokens*/

        if(qosInfo_p->availLcToken > allocatedBytes)
        {
            qosInfo_p->availLcToken -= allocatedBytes;

        }
        else
        {
            qosInfo_p->availLcToken = 0;
        }

        qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd] += allocatedBytes;


        /* 2. Calculate Priority*/
        qosInfo_p->tokenPriority
            = ulQosGetTknPriority(
                    qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd],
                    qosInfo_p->lcTknTable,
                    qosInfo_p->totSchdTknInWindow
                    );

        /*Update Head of line packet delay*/
        qosInfo_p->headOfLinePktDelay = 
            execArgs->globalTick;

        /* 3. calculate the weight of the Gbr LC node */
        ulQosCalcGbrLCWeightAndInsert(ulUEContext_p,
                ulQosStrategyLCNode_p->lchId,
                0,internalCellIndex); /* DrxOnDurationDelay */        

    }
    /* SPR 10984 FIX START */
    else if((NON_GBR == ulQosStrategyLCNode_p->channelType))
        /* SPR 10984 FIX END */
    {
        /* All NON-GBR LC.s are considered in a single strategic LC, 
         * and the token is updated at AMBR rate. */
        if(TRUE ==  ulQosStrategyLCNode_p->ulAmbrInfo_p->isLCNodePresent)
        { 
            ulQosDeleteEntryFromPrioritisedLcTree(
                    ulQosStrategyLCNode_p,internalCellIndex);
            ulQosStrategyLCNode_p->ulAmbrInfo_p->isLCNodePresent = FALSE;
        }

        ulAmbrInfo_p = ulQosStrategyLCNode_p->ulAmbrInfo_p;
        /* SPR 10984 FIX START */
        /* Storing the previouse availLcToken */
        ulAmbrInfo_p->prevAvailAmbrToken[ulSubFrameIdx] = 
            ulAmbrInfo_p->availAmbrToken;
        /* SPR 10984 FIX END */

        /* 1. Update Tokens*/
        if (ulAmbrInfo_p->availAmbrToken > allocatedBytes)
        {
            ulAmbrInfo_p->availAmbrToken -= allocatedBytes;
        }
        else
        {
            /*Qload adjustment changes*/
            ulAmbrInfo_p->availAmbrToken = 0;
        }

        ulAmbrInfo_p->scheduledToken[ulAmbrInfo_p->curTknWindowInd] += allocatedBytes;

        /*2. Update Head of line packet delay*/
        ulAmbrInfo_p->headOfLinePktDelay = 
            execArgs->globalTick;


        /* 3.calculate the weight of the Ambr node */
        ulQosCalcNgbrLCWeightAndInsert(ulUEContext_p,0,internalCellIndex,execArgs);                         

    }
    else
    {
        if(TRUE ==  ulQosStrategyLCNode_p->isLCNodePresent)
        { 
            ulQosDeleteEntryFromPrioritisedLcTree(
                    ulQosStrategyLCNode_p,internalCellIndex);
            ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
        }
        /* SPR 10984 FIX START */
        if ((FALSE == ulUEContext_p->
                    ulLCGInfo[ulQosStrategyLCNode_p->lcInfo_p->lcGId].isDrbPresent))
            /* SPR 10984 FIX END */
        {
            qosInfo_p = &ulQosStrategyLCNode_p->lcInfo_p->ulQosInfo; 

            /* SPR 10984 FIX START */
            /* Storing the previouse availLcToken */
            qosInfo_p->prevAvailLcToken[ulSubFrameIdx] = 
                qosInfo_p->availLcToken;

            /* SPR 10984 FIX END */
            if(qosInfo_p->availLcToken > allocatedBytes)
            {
                qosInfo_p->availLcToken -= allocatedBytes;
            }
            else
            {
                /*Qload adjustment changes*/
                qosInfo_p->availLcToken = 0;
            }


            qosInfo_p->curTknWindowInd += allocatedBytes;

            qosInfo_p->ulQosStrategyLCNode_p->totLCWght = 
                ulUEContext_p->
                ulLCGInfo[ulQosStrategyLCNode_p->lcInfo_p->lcGId].srbWeight;
            /* Insert it in the priortised LC list 
            */
            if(FALSE == qosInfo_p->ulQosStrategyLCNode_p->isLCNodePresent)
            {
                ulQosPutEntryInPrioritisedLcTree (
                        qosInfo_p->ulQosStrategyLCNode_p,internalCellIndex);
                qosInfo_p->ulQosStrategyLCNode_p->isLCNodePresent = TRUE;
            }
        }
    }

    /*Adjust Qload changes start*/
    prevBsrLcg = ulUEContext_p->bsrNet[lcGId];
    if ( ulUEContext_p->bsrNet[lcGId] >= allocatedBytes)
    { 
        ulUEContext_p->bsrNet[lcGId] -=  allocatedBytes;

        if((0 == ulUEContext_p->bsrNet[lcGId]) && prevBsrLcg)
        {
            /*BSR has become Zero from nonZero*/
            putEntryInBSRQueue(ulUEContext_p->ueIndex,lcGId,0,DELETE_TRIGGER,internalCellIndex);
#ifdef KPI_STATS

            /* Enqueue the Ue Id and lcId in the Circular Queue */
            lteMacUpdateKpiStatsIPThpUlOnEnqueue(lcGId,ulUEContext_p->ulLCGInfo,ulUEContext_p->ueIndex,internalCellIndex);/* +- SPR 17404 */
#endif
        }
    }
    else
    {
        ulUEContext_p->bsrNet[lcGId] = 0;
        if (prevBsrLcg)
        {
            /*BSR has become Zero from nonZero*/
            putEntryInBSRQueue(ulUEContext_p->ueIndex,lcGId,0,DELETE_TRIGGER,internalCellIndex);
#ifdef KPI_STATS

            /* Enqueue the Ue Id and lcId in the Circular Queue */
            lteMacUpdateKpiStatsIPThpUlOnEnqueue(lcGId,ulUEContext_p->ulLCGInfo,ulUEContext_p->ueIndex,internalCellIndex);/* +- SPR 17404 */
#endif
        }

    }
    /*Adjust Qload changes end*/
}

/*****************************************************************************
 * Function Name  : ulQosUpdateScheduledTokens
 * Inputs         : ulUEContext_p - Pointer to ULUEContext,
 *                  tbSize,
 *                  tempHarqProcessId
 * Outputs        : None
 * Returns        : None
 * Description    : It updates the token and weight for each UL strategic node
 *                  present in UL priortisd LC Tree.
 *****************************************************************************/
void ulQosUpdateScheduledTokens(ULUEContext *ulUEContext_p, 
        SInt16 tbSize,
        UInt8 tempHarqProcessId,
        InternalCellIndex internalCellIndex,
        UInt8 ulSubFrameNum,
        UInt8 tokenUpdateFromStr,
        ExecutionLegFunctionArg* execArgs)
{
    ULQosStrategyLCNode *ulQosStrategyLCNode_p = PNULL;

    void                *ulScheduledLcNodeVoid_p = PNULL; 
    /* SPR 10759 Changes Start */
    void                *ulScheduledLcNodeNextVoid_p = PNULL; 
    /* SPR 10759 Changes End */

    /* SPR 23393 Changes Start */
    UInt8 ulSubFrameIdx = MODULO_TWO(ulSubFrameNum); 
    /* SPR 23393 Changes End */

    UInt16 allocatedBytes       = 0;

    ULHarqInfo*   tempULHarqInfo_p = PNULL; 

    /* Get the First List element */
    ulScheduledLcNodeVoid_p =(void*)
        getFirstListNode(&ulUEContext_p->ulScheduledLcList);


    LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_TB_SIZE(
            ulUEContext_p->ueIndex,tbSize,internalCellIndex);

    if (tempHarqProcessId !=INVALID_HARQ_ID)
    {    
        tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
        if(PNULL == tempULHarqInfo_p)
        {
            ltePanic("tempULHarqInfo_p is NULL\n");
        }
#ifdef UTFWK_SIMULATION 
        tempULHarqInfo_p->deltabsrLoad = tbSize;
#endif 
    }

    while(ulScheduledLcNodeVoid_p)
    {
        /*debug stats*/
        LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_SCH_COUNT(
                ulUEContext_p->ueIndex);


        UL_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                ulScheduledLcNodeVoid_p, ulQosStrategyLCNode_p);

        allocatedBytes = (tbSize > ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx]) ?
            ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx] : tbSize;

        if (LTE_FALSE == tokenUpdateFromStr)
        {
            /* Tokens are already updated in Strategy 
             * So update the token After Straegy (from PDCCH/PUSCH)If reqrd */
            ulQosAdjustScheduledTokensFromPdcchPusch(
                    ulUEContext_p,
                    ulQosStrategyLCNode_p,
                    tempULHarqInfo_p,
                    allocatedBytes,
                    ulSubFrameIdx,
                    execArgs,
                    internalCellIndex);
        }
        else if(allocatedBytes)
        {
            /* Tokens are updated from Strategy based on MAX TB SIZE 
             * As For TDD config 0 we need to give grant for two U subframe
             * In a DL Tick */
            ulQosUpdateScheduledTokensFromStrategy(
                    ulUEContext_p,
                    ulQosStrategyLCNode_p,
                    allocatedBytes,
                    /* SPR 10984 FIX START */
                    ulSubFrameIdx,
                    /* SPR 10984 FIX END */
                    execArgs,
                    internalCellIndex
                    );
        }

        tbSize -= allocatedBytes;

        /* - List element will be deleted after 2nd PUSCH call in case of TDD0 
         * - In othre cases It will be deleted after PUSCH El call*/
        if (LTE_FALSE == tokenUpdateFromStr)
        {
            /* In acse of TDD config 1-6 ulScheduledLcList Node 
             * will be deleted From PUSCH 
             * In case of TDD config 0 S subframe ulScheduledLcList Node will be
             * deleted from 2nd PUSCH 
             * In case of TDD config 0 D subframe ulScheduledLcList Node will be deleted 
             * from 1st PUSCH call ulSubFrameNum = 4 or 9 */

            /* Resetting the scheduledBytes to 0 */
            ulQosStrategyLCNode_p->scheduledBytes[ulSubFrameIdx] = 0;
            if (UL_DL_CONFIG_0 ==  getTddConfig(internalCellIndex))
            {
                /* SPR 10824 Changes Start */
                /*
                 ** While updated tokens in failure scenario, this can be invoked for second
                 ** ulsubframe in special subframe. Therefore, deleting node from ulScheduledLcList
                 ** list when a schedule bytes are 0 in both ulsubframe.
                 **
                 ** For D subframe in which one ulsubframe is processed, the scheduled bytes for
                 ** second subframe is always 0.
                 */
                if ((ulQosStrategyLCNode_p->scheduledBytes[((ulSubFrameIdx+1) % MAX_UL_SUBFRAME_GRANT)]) == 0)
                    /* SPR 10824 Changes End */
                {
                    /* Resetiing isLCScheduled to FALSE */
                    ulQosStrategyLCNode_p->isLCScheduled = FALSE;
                    /* SPR 10759 Changes Start */
                    ulScheduledLcNodeNextVoid_p = getNextListNode(
                            &ulQosStrategyLCNode_p->scheduledLcNodeAnchor);
                    listDeleteNode(&ulUEContext_p->ulScheduledLcList,ulScheduledLcNodeVoid_p);
                    ulScheduledLcNodeVoid_p = ulScheduledLcNodeNextVoid_p; 
                    /* SPR 10759 Changes End */
                    /* Resetting the scheduledLcCount value */
                    --ulUEContext_p->scheduledLcCount;
                }
                else
                {
                    /* Getting the Next node As PUSCH is called for 1st SF */
                    ulScheduledLcNodeVoid_p = getNextListNode(
                            &ulQosStrategyLCNode_p->scheduledLcNodeAnchor);
                }
            }
            else
            {
                /* Resetiing isLCScheduled to FALSE */
                ulQosStrategyLCNode_p->isLCScheduled = FALSE;
                /* SPR 10759 Changes Start */
                ulScheduledLcNodeNextVoid_p = getNextListNode(
                        &ulQosStrategyLCNode_p->scheduledLcNodeAnchor);
                listDeleteNode(&ulUEContext_p->ulScheduledLcList,ulScheduledLcNodeVoid_p);
                ulScheduledLcNodeVoid_p = ulScheduledLcNodeNextVoid_p; 
                /* SPR 10759 Changes End */
                /* Resetting the scheduledLcCount value */
                --ulUEContext_p->scheduledLcCount;
            }
        }
        else
        {
            /* Getting the Next node as update token is called from strategy */
            ulScheduledLcNodeVoid_p = getNextListNode(
                    &ulQosStrategyLCNode_p->scheduledLcNodeAnchor);
        }
    }
}
/*****************************************************************************
 * Function Name  : deleteScheduledLcNode
 * Inputs         : ulGrantSubFrameNum : ulSF of 1st PUSCH processing.
 * Outputs        : None
 * Returns        : None
 * Description    : It Delets the ScheduledLcNodes of 1st UL SF PUSCH scheduled
 *                  UEs when there is no ue to be scheduled at 2nd UL SF at 
 *                  PUSCH.As the ScheduledLcNodes are now deleted at 2nd UL SF
 *                  at PUSCH.
 *****************************************************************************/
void deleteScheduledLcNode(UInt8 ulGrantSubFrameNum,InternalCellIndex internalCellIndex)
{
    UInt16 count   = 0;
    UInt16 ueIndex = 0;
    RNTIInfo    *rntiInfo_p    = PNULL;
    ULUEContext *ulUEContext_p = PNULL;
    ULQosStrategyLCNode *ulQosStrategyLCNode_p = PNULL;
    void                *ulScheduledLcNodeVoid_p = PNULL; 
    ULSchUciRachPoolInfo* ulschUciRachInfo_p
        = (ulSchUciRachContainer_gp[internalCellIndex] + (ulGrantSubFrameNum % MAX_ULSCH_UCI_CONTAINER));
    /* Process All the UE's processed in previouse PUSCH */
    for (count = 0; count < ulschUciRachInfo_p->numberOfInfoPresent; count++)
    {
        /* Get ueIndex and ueContext from rnti */
        rntiInfo_p = getUeIdxFromRNTIMap(
                ulschUciRachInfo_p->ulschUciInfo[count].rnti,internalCellIndex);
        if (PNULL != rntiInfo_p )
        {
            ueIndex = rntiInfo_p->index;
            if ((ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) ||
                    (PNULL == ulUECtxInfoArr_g[ueIndex].ulUEContext_p))
            {
                continue;
            }
            ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
            ulScheduledLcNodeVoid_p = 
                listPopNode(&ulUEContext_p->ulScheduledLcList);
            /* Delete all the scheduledLcNode form UE Context */
            while(PNULL != ulScheduledLcNodeVoid_p) 
            {
                UL_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                        ulScheduledLcNodeVoid_p, ulQosStrategyLCNode_p);
                /* Resetiing isLCScheduled to FALSE */
                ulQosStrategyLCNode_p->isLCScheduled = FALSE;
                /* Resetting the scheduledLcCount value */
                --ulUEContext_p->scheduledLcCount;
                ulScheduledLcNodeVoid_p = 
                    listPopNode(&ulUEContext_p->ulScheduledLcList);
            }
        }
    }
}
#endif
/* TDD Config 0 Changes End */

/***************************************End of File ************************/
