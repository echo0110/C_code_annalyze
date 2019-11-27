/******************************************************************************
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacPucchPowerControl.c
 *
 ******************************************************************************
 *
 *  File Description : This file contains implementation of Power Control 
 *                     Feature in LTE_MAC.
 *                     
 *
 ******************************************************************************
 *
 * Revision Details
 * Revision 1.1.1 2012/07/20 06:06:27  gur31292
 * Changes related to DCI Format3A
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacPucchPowerControl.h"
#include "lteMacDLHarqMgr.h"
#include "lteMacULCSIManager.h"
#include "lteMacRRMPeriodicMgr.h"
#include "lteMacPDSCHMgr.h"
#ifdef TDD_CONFIG
#include    "lteMacTDDConfig.h"
#endif

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
STATIC  void initListsForTpcRntiContextForPucch(InternalCellIndex internalCellIndex,
                                                      UInt8 maxTpcRntiRange);
/******************************************************************************
 * Private Constants
 *****************************************************************************/
/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern UInt8  explicitTpcTriggerEnabled_g[MAX_NUM_CELL];
/* SPR 11700 Start */
extern UInt8 getDlAckNackOffset( UInt8 subframeNum,InternalCellIndex internalCellIndex);
/* SPR 11700 End */
/*START_DYN_PDCCH_TDD_CHANGE*/
/*END_DYN_PDCCH_TDD_CHANGE*/
/*START : DYN_PDCCH*/
extern UInt8 currentCFIValue_g[MAX_NUM_CELL];
/*END   : DYN_PDCCH*/
/* CLPC_CHG */
/* Number of TPC PUSCH RNTI configured as indicated by range configured*/
UInt8 maxTpcPucchRntiRange_g;

/* Pointer to the global array of TPC-PUSCH-RNTIs configured in the system*/
TpcRntiContext * tpcPucchRntiContext_gp[MAX_NUM_CELL] = {PNULL};

/* Queue for Triggered TPC for PUCCH */
TPC_TRIGGERED_QUEUE_PUCCH_Q* tpcTriggeredQueuePucch_gp[MAX_NUM_CELL] = {PNULL};
/*HD FDD Changes Start*/
/* Queue for storing UE which were triggered for TPC,
 * but could not be scheduled in case of HD FDD*/
TPC_SCHED_FAIL_QUEUE_PUCCH_Q* tpcSchedFailQueuePucch_gp[MAX_NUM_CELL] = {PNULL};
/*HD FDD Changes End*/

LTE_SQUEUE* scheduledFormat3Or3AQueueForPucch_gp[MAX_NUM_CELL] = {PNULL};
UInt16 pucchUciUlschMap_g[MAX_UE_SUPPORTED];

/* SPR 13196 Changes start */
extern UInt32 dci3RightShiftOffset_g[MAX_NUM_CELL] ;
extern UInt32 dci3ARightShiftOffset_g[MAX_NUM_CELL];
/* SPR 13196 Changes end */

extern void deleteEntryFromTriggeredUeList(
        TriggeredUEListNode *triggeredUeListNodeInTpcRntiContext_p,
        TpcRntiContext * tpcRntiContext_p);

/* SPR 5599 changes start (ZIP ID 129157) */
extern UInt8 putEntryInExplicitDLDCIScheduleQueue(UInt16 ueIdx, InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129157) */
/* SPR 15909 fix start */
extern SInt8 checkTPCForPUCCHHarqBler(UInt8 bler,/* +- SPR 17777 */
/* SPR 15909 fix end */        
                                      InternalCellIndex internalCellIndex);
extern SInt8 checkTPCForPUCCHCqiSinr(UInt8 sinrReportedForPucch,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
         /* SPR 15909 fix end */
/*5.1.0 Warning fix*/
extern  void updatePucchSINR(UInt16 ueIndex,UInt8 pucchSINR,
                                InternalCellIndex internalCellIndex);

/*********************************************************************************
 * Function Name  : addEntryToTpcTriggerQueueForPucch
 * Inputs         : tpcValue - TPC value,
 *                  ueIndex - UE id
 *                  tpcInfoForPucch - reference to DL Context's TpcInfoForPucch,
 *                  currentTick -
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall add the entry of UE in
 *                  tpcTriggerQueueForPucch and/or rnti triggered list after the
 *                  power control for PUCCH is triggered.
 *********************************************************************************/
void addEntryToTpcTriggerQueueForPucch(SInt8 tpcValue, UInt16 ueIndex,
                                       TpcInfoForPucch * tpcInfoForPucch, 
                                       /* SPR 15909 fix start*/
                                       tickType_t currentTick
                                       /* spr 15909 fix end */
                                       ,InternalCellIndex internalCellIndex
        )
{
    UInt16 tpcRntiIndex = tpcInfoForPucch->tpcRntiIndex;
    TriggeredUEListNode* triggeredUeNode_p = PNULL;
    TpcRntiContext * tpcRntiContext_p = PNULL;
    /* SPR 15909 fix start */
    tickType_t expiryTick = 0;
    /* SPR 15909 fix end */

    if (tpcInfoForPucch->tpcRnti != INVALID_TPC_RNTI) 
    {
        /* Get the TPC RNTI CONTEXT based on the tpcRntiIndex */
        tpcRntiContext_p = 
            (tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex); 
        /* Coverity CID 54506 Start */
        /* code removed for coverity */
        /* Coverity CID 54506 End */
        /* UE has not be added in triggeredUEList till now */
        /* Increment the triggered count*/
        tpcRntiContext_p->triggeredCount++;
        /* Add the node in Triggered UE List */
        triggeredUeNode_p = addEntryInTriggeredUeList(ueIndex, 
                &(tpcRntiContext_p->triggeredUeList), PNULL);
        if (triggeredUeNode_p == PNULL)
        {
            return;
        }
        tpcInfoForPucch->triggeredUeListNodeInTpcRntiContext_p =  triggeredUeNode_p;
        /* Coverity CID 54506 Start */
        /* code removed for coverity */
        /* Coverity CID 54506 End */
    }
    else
    {
        // put entry in tpc triggered queue only if the the 
        // explicitTpcTriggerEnabled_g is true
        if ((explicitTpcTriggerEnabled_g[internalCellIndex] != TRUE ))
        {
            tpcInfoForPucch->tpcTriggeredValue = tpcValue;    
            return;
        }
    }

    /*cov 85848 +-*/    
#ifdef FDD_CONFIG
    expiryTick = currentTick + 
        (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->tpcTriggerWaitTimeForPUCCHDCI1x2x;
#elif TDD_CONFIG
    /*cov 85848 +-*/    
    expiryTick =
        tddConvertTimerValForDlGrant(
                (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])-> \
                tpcTriggerWaitTimeForPUCCHDCI1x2x, 
                currentTick,
                internalCellIndex);
#endif
    putEntryInTpcTriggeredQueueForPucch(ueIndex, expiryTick, internalCellIndex);
    LOG_UT(TPC_TRIGGER_Q_ENTRY_PUCCH_ID, LOGDEBUG, MAC_UL_PC,
            0, ueIndex, expiryTick, tpcValue,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,
            "ADD_ENTRY_TRIGGER_QUEUE_OR_LIST");
    tpcInfoForPucch->tpcTriggeredValue = tpcValue;
}

/**************************************************************************************
* Function Name  : initSinrToTpcMap
* Inputs         : deltaSINRToTPCMapForPUCCH_p - Pointer to DeltaSINRToTPCMapForPUCCH
*                                                populated from OAM interface,
*                  schedulerParams_p - Pointer to MacReconfigSchedulerParamsDL
* Outputs        : None
* Returns        : None
* Description    : This function based on the target Sinr, populates the
*                  deltaSinrToPucchTpcMap_g map.
**************************************************************************************/
void initSinrToTpcMap(DeltaSINRToTPCMapForPUCCH * deltaSINRToTPCMapForPUCCH_p,
        MacReconfigSchedulerParamsDL *schedulerParams_p)
{
    UInt8 targetSinr = deltaSINRToTPCMapForPUCCH_p->pucchSINRTarget;
    UInt8 count = deltaSINRToTPCMapForPUCCH_p->count;
    /* SPR  3444 changes start */
    UInt16 index = 0;
    /* SPR  3444 changes end */
    SInt16 startIndex = 0;
    SInt16 endIndex = 0;

    while (count)
    {
        count--;
        DeltaSINRToTPCMapForPUCCHInfo * deltaSINRToTPCMap_p = \
            &(deltaSINRToTPCMapForPUCCH_p->\
                  deltaSinrToTPCMapForPUCCHInfo[count]);

        if (deltaSINRToTPCMap_p->startSINR > \
               (NUM_OF_SINR_TPC_VALUES - targetSinr))
        {
            continue;
        }

        if (deltaSINRToTPCMap_p->startSINR < -1*targetSinr)
        {
            startIndex = 0;
        }
        else
        {
            startIndex = deltaSINRToTPCMap_p->startSINR + targetSinr;
        }

        if (deltaSINRToTPCMap_p->endSINR > \
               (NUM_OF_SINR_TPC_VALUES - targetSinr))
        {
            /* SPR 6947 fix start */
            endIndex = NUM_OF_SINR_TPC_VALUES - 1 ;
            /* SPR 6947 fix end */
        }
        else
        {
            endIndex = deltaSINRToTPCMap_p->endSINR + targetSinr;
        }

        /*SPR 4017 Fix Start*/
        /* SPR 6947 fix start */
        for (index =  startIndex; index <= endIndex; index++)
        /* SPR 6947 fix end */
        /*SPR 4017 Fix End*/
        {

            schedulerParams_p->deltaSinrToPucchTpcMap[index] = 
                deltaSINRToTPCMap_p->pucchTPC;
        }
    } /* While End */
}


/*************************************************************************************
* Function Name  : initHarqBlerToTPCMap
* Inputs         : deltaBLERToTPCMapForPUCCH_p - Pointer to DeltaBLERToTPCMapForPUCCH
*                                                populated from OAM interface,
*                  schedulerParams_p - Pointer to MacReconfigSchedulerParamsDL
* Outputs        : None
* Returns        : None
* Description    : This function based on the target Sinr, populates the
*                  deltaHarqBlerToPucchTpcMap_g map.
*************************************************************************************/
void initHarqBlerToTPCMap(
         DeltaBLERToTPCMapForPUCCH * deltaBLERToTPCMapForPUCCH_p,
         MacReconfigSchedulerParamsDL *schedulerParams_p)

{
    UInt8 targetBler = deltaBLERToTPCMapForPUCCH_p->pucchBLERTarget;
    UInt8 count = deltaBLERToTPCMapForPUCCH_p->count;
    /* SPR  3444 changes start */
    UInt16 index = 0;
    /* SPR  3444 changes end */
    SInt16 startIndex = 0;
    SInt16 endIndex = 0;

    while (count)
    {
        count--;
        DeltaBLERToTPCMapForPUCCHInfo * deltaBLERToTPCMap_p =
            &(deltaBLERToTPCMapForPUCCH_p->\
                  deltaBlerToTPCMapForPUCCHInfo[count]);

        if (deltaBLERToTPCMap_p->startBLER > \
                (NUM_OF_BLER_TPC_VALUES - targetBler))
        {
            continue;
        }

        if (deltaBLERToTPCMap_p->startBLER < -1*targetBler)
        {
            startIndex = 0;
        }
        else
        {
            startIndex = deltaBLERToTPCMap_p->startBLER + targetBler;
        }

        if (deltaBLERToTPCMap_p->endBLER > \
               (NUM_OF_BLER_TPC_VALUES - targetBler))
        {
            endIndex = NUM_OF_BLER_TPC_VALUES ;
        }
        else
        {
            endIndex = deltaBLERToTPCMap_p->endBLER + targetBler;
        }

        /*SPR 4017 Fix Start*/
        for (index =  startIndex; index < endIndex; index++)
        /*SPR 4017 Fix End*/
        {
            schedulerParams_p->deltaHarqBlerToPucchTpcMap[index] =
                deltaBLERToTPCMap_p->pucchTPC;

        }
    } /* While End */
}


/*****************************************************************************
 * Function Name  : addEntryInTriggeredUeList
 * Inputs         : ueIndex - UE Index,
 *                  triggeredUeNode_p - Pointer to TriggeredUEListNode,
 * Outputs        : Puts the TriggeredUEListNode in passed triggeredUeList
 * Returns        : node_p - TriggeredUEListNode
 * Description    : This function creates a new TriggeredUEListNode and 
 *                  inserts thisi node in the TPCRNTI->triggeredUeList 
 *                  to be processed by worker thread.
 *****************************************************************************/
TriggeredUEListNode*  addEntryInTriggeredUeList(UInt16 ueIndex, 
                               LTE_LIST * triggeredUeList, TriggeredUEListNode* triggeredNode_p)
{
    LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s] ueIndex [%d] Entry. ",
                   __func__, ueIndex);
    TriggeredUEListNode* node_p = PNULL;
    /* HD FDD Changes Start */
    node_p = triggeredNode_p;
    if (node_p == PNULL)
    {
    /* HD FDD Changes End */
        GET_MEM_FROM_POOL(TriggeredUEListNode, node_p, 
                sizeof(TriggeredUEListNode), PNULL);
        /*+COVERITY 5.3.0 - 25230*/
        if (PNULL == node_p)
        {
            return PNULL;    
        }
        /*-COVERITY 5.3.0 - 25230*/
        node_p->ueIndex = ueIndex;
    /* HD FDD Changes Start */
    }
    /* HD FDD Changes End */
    if(node_p != PNULL)
    {
        node_p->isPresentInFailList = FALSE;
        listInsertNode(triggeredUeList, &node_p->triggeredUeAnchor);
    }
    return node_p;
}

/*DCI Format 3A changes start*/
/*****************************************************************************
 * Function Name  : addEntryInConfiguredUeList
 * Inputs         : ueIndex - UE Index,
 *                  configuredUeList - Pointer to LTE_LIST
 * Outputs        : Puts the ConfiguredUEListNode in passed configuredUeList
 * Returns        : node_p - ConfiguredUeList
 * Description    : This function creates a new ConfiguredUEListNode and 
 *                  inserts this node in the TPCRNTI->configuredUeList 
 *                  to be processed by worker thread.
 *****************************************************************************/
ConfiguredUEListNode*  addEntryInConfiguredUeList(UInt16 ueIndex, 
                               LTE_LIST * configuredUeList)
{
    LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s] ueIndex [%d] Entry. ",
                   __func__, ueIndex);
    ConfiguredUEListNode* node_p = PNULL;
    GET_MEM_FROM_POOL(ConfiguredUEListNode, node_p, 
            sizeof(ConfiguredUEListNode), PNULL);
    if(node_p != PNULL)
    {
        node_p->ueIndex = ueIndex;
        listInsertNode(configuredUeList, &node_p->configuredUeAnchor);
    }
    return node_p;
}
/*DCI Format 3A changes end*/

/*HD FDD Changes Start*/
/*****************************************************************************
 * Function Name  : addEntryInFailTriggeredUeList
 * Inputs         : ueIndex - UE Index,
 *                  failTriggeredUeList - Pointer to LTE_LIST
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : Puts the TriggeredUEListNode in passed triggeredUeList
 * Returns        : node_p - passed triggeredUeList
 * Description    : This function creates a new TriggeredUEListNode and 
 *                  inserts thisi node in the TPCRNTI->triggeredUeList 
 *                  to be processed by worker thread.
 *****************************************************************************/
TriggeredUEListNode*  addEntryInFailTriggeredUeList(TriggeredUEListNode* node_p, 
                               /* +- SPR 17777 */
                               LTE_LIST * failTriggeredUeList
                               /*CA Changes start */
#ifdef LOG_UT_ENABLED
                               ,InternalCellIndex internalCellIndex
#endif
                               /* +- SPR 17777 */
                               /*CA Changes end */)
{
    LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s] ueIndex [%d] Entry. ",
                 __func__, node_p->ueIndex);
#ifdef LOG_UT_ENABLED
    DLUEContext* dlUeContext_p = dlUECtxInfoArr_g[node_p->ueIndex].dlUEContext_p;
    if (dlUeContext_p)
    {
        TpcRntiContext * tpcRntiContext_p = (tpcPucchRntiContext_gp[internalCellIndex] + dlUeContext_p->tpcInfoForPucch.tpcRntiIndex);
        LOG_MAC_MSG(TPC_TRIGGER_UE_LIST_ENTRY_PUCCH_ID, LOGDEBUG, MAC_UL_PC,
                0, node_p->ueIndex, dlUeContext_p->tpcInfoForPucch.tpcBitMapIndex, tpcRntiContext_p->failTriggerCount,
                tpcRntiContext_p->scheduledState,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "ADD_ENTRY_TRIGGER_QUEUE_OR_LIST");
    }
#endif
    if(node_p != PNULL)
    {
        node_p->isPresentInFailList = TRUE;
        listInsertNode(failTriggeredUeList, &node_p->triggeredUeAnchor);
    }
    return node_p;
}
/*HD FDD Changes End*/

/*******************************************************************************************
 * Function Name  : putEntryInTpcTriggeredQueueForPucch
 * Inputs         : ueIndex - UE Index,
 *                  expiryTick - Tick when the node gets expired
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : Puts the entry in Tpc Triggered Queue
 * Returns        : None
 * Description    : This function creates a new TpcTriggerQueueNode and inserts this
 *                  node in the tpcTriggeredQueuePucch_g  to be processed by worker thread.
 *****************************************************************************/
/* SPR 15909 fix start */
void putEntryInTpcTriggeredQueueForPucch(UInt16 ueIndex, tickType_t expiryTick
/* SPR 15909 fix end */        
        , InternalCellIndex internalCellIndex)
{
    LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s] ueIndex [%d] Entry. ",
                   __func__, ueIndex);
    TpcTriggerQueueNode* node_p = PNULL;
    GET_MEM_NODE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(node_p, TpcTriggerQueueNode);
    if(node_p != PNULL)
    {
        node_p->ueIndex = ueIndex;
        node_p->expiryTick  = expiryTick;
        /* COVERITY CID 25448 fix start */
        if(ENQUEUE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(TpcTriggerQueueNode, 
            &(node_p->nodeAnchor), internalCellIndex))
        {
            FREE_MEM_NODE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(node_p);
        }
        /* COVERITY CID 25448 fix end */
    }

    return ;
}

/*HD FDD Changes Start*/
/********************************************************************************************
 * Function Name  : putEntryInTpcSchedFailQueueForPucch
 * Inputs         : node_p - Pointer to TpcTriggerQueueNode
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates a new TpcSchedFailQnode and inserts this
 *                  node in the tpcSchedFailQueuePucch_g  to be processed by worker thread.
 *****************************************************************************/
void putEntryInTpcSchedFailQueueForPucch(TpcTriggerQueueNode* node_p, InternalCellIndex internalCellIndex)
{
    LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s] ueIndex [%d] Entry. ",
                   __func__, node_p->ueIndex);
    if(node_p != PNULL)
    {
        /* Coverity CID 25221 FIX start */
        if( ENQUEUE_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(TpcTriggerQueueNode, 
            &(node_p->nodeAnchor), internalCellIndex) )
        {
            FREE_MEM_NODE_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ (node_p);
        }
        /* Coverity CID 25221 FIX end */
    }
    return ;
}
/*HD FDD Changes End*/

/*****************************************************************************
 * Function Name  : updateAndCheckCqiSinr
 * Inputs         : dlUeContext_p - DL UE Context pointer,
 *                  sinrReportedForPucch - reported SINR for PUCCH,
 *                  currentTick - Current Tick
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function calculates the averageSinrForPucch and
 *                  check if the coolOff period has expired then check if
 *                  TPC needs to be triggered based on the SINR.
 *****************************************************************************/
void updateAndCheckCqiSinr(DLUEContext* dlUeContext_p, 
                           /* Bler changes start */
                           SInt32 sinrReportedForPucch, 
                           /* Bler changes end */
                           /* SPR 15909 fix start */
                           tickType_t currentTick
                           /* SPR 15909 fix end */
                           ,InternalCellIndex internalCellIndex
        )
{


    /* Bler changes start */
    if (sinrReportedForPucch < MIN_EFFECTIVE_PUCCH_SINR)               
    {                                                                
        sinrReportedForPucch = MIN_EFFECTIVE_PUCCH_SINR;               
    }                                                                
    if (sinrReportedForPucch > MAX_EFFECTIVE_PUCCH_SINR)               
    {                                                                
        sinrReportedForPucch = MAX_EFFECTIVE_PUCCH_SINR;               
    }                                                                
    /* Bler changes end */
    
    UInt8 lastSinrReported = dlUeContext_p->averageSinrForPucch;

    /* Time Averaging changes start */
    if ((dlUeContext_p->powerControlPucchTpcValue) && \
        (currentTick >= dlUeContext_p->tpcInfoForPucch.lastAppliedTick) )
    {
        /*SPR 3856 Fix Start*/
        dlUeContext_p->averageSinrForPucch = ((sinrReportedForPucch << 10) + \
                       MAC_ALPHA_PUCCH_COEFFICIENT_IIR*((lastSinrReported + 
                       dlUeContext_p->powerControlPucchTpcValue) - sinrReportedForPucch)) >> 10;
        /*SPR 3856 Fix End*/

        dlUeContext_p->powerControlPucchTpcValue = 0; 
    }
    else
    {
        /*SPR 3856 Fix Start*/
        dlUeContext_p->averageSinrForPucch = ((sinrReportedForPucch << 10) + \
            MAC_ALPHA_PUCCH_COEFFICIENT_IIR*(lastSinrReported-sinrReportedForPucch)) >> 10;
        /*SPR 3856 Fix End*/

    }
    /* SPR 5033 changes start  */\
    updatePucchSINR(dlUeContext_p->ueIndex, \
                         dlUeContext_p->averageSinrForPucch,internalCellIndex);\
    /* SPR 5033 changes end  */\
    /* Time Averaging changes end */

    /* Check if the cool off period has expired, then check SINR for the PUCCH */
    /* SPR 5033 changes start */\
    if ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
          cellParams_p->powerControlEnableInfo.cqiSinrClpcPucchEnable)&&\
      (currentTick >= dlUeContext_p->tpcInfoForPucch.lastAppliedTick) && \
        ((dlUeContext_p->lastSINRCoolOffCounter++) >=
             (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->coolOffPeriodForPUCCHTPC))
    /* SPR 5033 changes end */\
    {
        // Reset the counter
        dlUeContext_p->lastSINRCoolOffCounter = 0;
        /* +- SPR 17777 */
        SInt8 tpcTriggeredValue = checkTPCForPUCCHCqiSinr(dlUeContext_p->averageSinrForPucch,
                                                        internalCellIndex);
        /* +- SPR 17777 */
        if ((tpcTriggeredValue != TPC_NO_CHANGE) &&
        (dlUeContext_p->tpcInfoForPucch.tpcTriggeredValue == TPC_NO_CHANGE))
        {
            addEntryToTpcTriggerQueueForPucch(tpcTriggeredValue,
                                dlUeContext_p->ueIndex,
                                &(dlUeContext_p->tpcInfoForPucch),
                                currentTick
                                ,internalCellIndex
                    );
        }

        LOG_MAC_MSG(CQI_SINR_CALC_PUCCH_ID,
                LOGDEBUG,
                MAC_UL_PC,
                0,
                dlUeContext_p->ueIndex,
                dlUeContext_p->averageSinrForPucch,
                dlUeContext_p->tpcInfoForPucch.tpcTriggeredValue,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "UPDATE_CQI_SINR_PUCCH");

    }
}

/*HD FDD Changes Start*/
/*****************************************************************************
 * Function Name  : processTpcUeTriggerNodeForDci3A
 * Inputs         : node_p - Pointer to TriggeredUEListNode,
 *                  tpcRntiContext_p - Pointer to TpcRntiContext,
 *                  dciCCEInfo_p - Pointer to DciCCEInfo,
 *                  tick - Current Tick
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall process the Failure/triggered UE list node 
 *                  in case of DCI 3A.
 *****************************************************************************/
void processTpcUeTriggerNodeForDci3A
(
 TriggeredUEListNode* node_p, 
 TpcRntiContext* tpcRntiContext_p,
 DciCCEInfo* dciCCEInfo_p,
 /* SPR 15909 fix start */
 tickType_t tick,
 /* SPR 15909 fix end */
 InternalCellIndex internalCellIndex
 )
{
    UInt32  tempUeIdx = 0;
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t localTick = 0;
#endif
    tickType_t  lstAppliedTick = 0;
    /* SPR 15909 fix end */
    UInt16  tpcBitMapIndex = 0;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    DLUEContext* dlUeCntxt_p = PNULL;

    if (!dlUECtxInfoArr_g[node_p->ueIndex].pendingDeleteFlag)
    {
        /* Get dlUeContext using ueIndex extracted from the node popped */
        tempUeIdx = node_p->ueIndex;

        /* Get lastAppliedTick and tpcBitMapIndex from ueContext */
        dlUeCntxt_p = dlUECtxInfoArr_g[tempUeIdx].dlUEContext_p;
        lstAppliedTick = dlUeCntxt_p->tpcInfoForPucch.lastAppliedTick;
        tpcBitMapIndex = dlUeCntxt_p->tpcInfoForPucch.tpcBitMapIndex;

        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
        localTick = getCurrentTick() + PHY_DL_DELAY;
#endif
        /*HD FDD Changes End*/
        /*First check for HD FDD UE's then process other UE's*/
        if ((DRX_STATE_ON == dlUeCntxt_p->drxContext.drxStateVariables.drxCurrentState) ||
            (TRUE == dlUeCntxt_p->measGapContext.isMeasGapPeriodOn)||
            ((SPS_ACTIVATION_INITIATED == dlUeCntxt_p->spsDlInfo.spsState) && 
             (SPS_SCHEDULING == dlUeCntxt_p->schType))
            )
        {
            /* If DRX or Meas Gap is ON then put the entry in explicit queue so that it can go when the
             * data is scheduled */
            putEntryInExplicitDLDCIScheduleQueue(tempUeIdx, internalCellIndex);
            LOG_MAC_MSG(TPC_EXPLICIT_TRIGGER_Q_ENTRY_PUCCH_ID, LOGDEBUG, MAC_UL_PC,
                    tick, tempUeIdx, dlUeCntxt_p->tpcInfoForPucch.tpcTriggeredValue,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "ADD_ENTRY_EXPLICIT_TRIGGER_QUEUE_PUCCH");

            tpcRntiContext_p->triggeredCount--;
            /*BUG:START:9048*/
            freeMemPool(node_p);
            dlUeCntxt_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL;
            /*BUG:END:9048*/

        }
        else if ((lstAppliedTick != tpcRntiContext_p->appliedUlTick) 
#ifdef HD_FDD_CONFIG                
                &&
                /*HD FDD Changes Start*/
                (((TRUE == dlUeCntxt_p->isHDFddFlag) &&
                  (CAN_SCHEDULE_TPC_FOR_HD_UE(localTick, dlUeCntxt_p->ueIndex, hdUeContextInfo_g.hdfddUeSchedMap)))||
                 (FALSE == dlUeCntxt_p->isHDFddFlag))
#endif
                )
            /*HD FDD Changes End*/
        {
            powerCorrectionAndTpc = getAccumulatedPowerForPucch(
                    dlUeCntxt_p->tpcInfoForPucch.tpcTriggeredValue,   
                    DCI_FORMAT_3A);

            if (powerCorrectionAndTpc.powerCorrectionVal)
            {
                /* SPR 21631 Start */
                 UInt8 mapIdx = tick % MAX_TPC_CONTAINER_MAP_SIZE;
                /* SPR 21631 End */
                UInt16 rnti = 0;

                dlUeCntxt_p->currPowerCorrectionForPucch += 
                    powerCorrectionAndTpc.powerCorrectionVal;

                if ( SPS_SCHEDULING == dlUeCntxt_p->schType ) 
                { 
                    rnti = dlUeCntxt_p->spsDlInfo.SpsCrnti;    
                }
                else
                {
                    rnti = dlUeCntxt_p->crnti;    
                }

                /* Storing the tpc information to be sent it will be used if
                 * error indication rcvd from phy */
                /* Storing at current index, it will be fetched at receiver 
                 * from current tick - ERROR_IND Delay */
                putEntryInTpcSentMap( tick, mapIdx, rnti, 
                        powerCorrectionAndTpc.powerCorrectionVal,
                        ERR_IND_TPC_FOR_PUCCH ,internalCellIndex);
            }
	    /* SPR 13196 Changes start */
	    /* Set tpcTriggerd at tpcRntiIndex in pDciPdu_p->tpcBitmap 
	     * e.g if 10Mhz ,tpcRntiIndex = 1 ,from formula (32-1) -5 = 26. 
	     * so tpcTriggered will be left shifted to 26 bits */
            UInt8 offset = 
                    (((32 - tpcBitMapIndex)) -
                     dci3ARightShiftOffset_g[internalCellIndex]);
	    /* SPR 13196 Changes end */ 
            /* set the bit to the tpc command */
            dciCCEInfo_p->dciPduInfo.tpcBitmap |= powerCorrectionAndTpc.tpcCommand <<  offset;

            /* Time Averaging changes start */
            if ( cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 cellParams_p->powerControlEnableInfo.cqiSinrClpcPucchEnable )
            {
                dlUeCntxt_p->powerControlPucchTpcValue = powerCorrectionAndTpc.powerCorrectionVal;
            }
            /* Time Averaging changes end */
            /* Reset the blerCalculated and blerCalculatedCounter to 0 */
            RESET_COOLOFFS_AND_BLER_CALCULATE_COUNTER(dlUeCntxt_p);
            /* Reseting the fakeTpcValue*/
            dlUeCntxt_p->tpcInfoForPucch.fakeTpcValue = FAKE_TPC_NO_VALUE;
            /* Free the node 
             * Pop next node from the triggerdUeList
             */

            freeMemPool(node_p);
            dlUeCntxt_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL;
        }
        else
        {
#ifdef HD_FDD_CONFIG
            if((TRUE ==  dlUeCntxt_p->isHDFddFlag) && (lstAppliedTick != tpcRntiContext_p->appliedUlTick))
            {
                /*Putting into failure triggerlist and decrementing the trigger count
                 * and increment the failure count*/
                /* put entry into the failure trigger list*/
                /* +- SPR 17777 */
                addEntryInFailTriggeredUeList(node_p, &(tpcRntiContext_p->failTriggerUeList)
#ifdef LOG_UT_ENABLED
                        , internalCellIndex
#endif
                        );
                /* +- SPR 17777 */

                /* Increment the statistics counter UE denied DL resources due to
                 * UL clash in the same TTI of HD FDD UE*/
                UPDATE_UE_DENIED_DL_RESOURCE(dlUeCntxt_p->ueIndex);
            }
#endif
            tpcRntiContext_p->triggeredCount--;
            /* NOTE : No Need to free the node here since same node is being used in failTriggerUeList */
        }
    }
    else
    {
        tpcRntiContext_p->triggeredCount--;
    }
    return;

}/*end of processTpcUeTriggerNodeForDci3A()*/

/*****************************************************************************
 * Function Name  : processTpcUeTriggerNodeForDci3
 * Inputs         : node_p - Pointer to TriggeredUEListNode,
 *                  tpcRntiContext_p - Pointer to TpcRntiContext,
 *                  dciCCEInfo_p - Pointer to DciCCEInfo,
 *                  tick - Current Tick
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall process the triggered UE list node 
 *                  in case of DCI 3.
 *****************************************************************************/
void processTpcUeTriggerNodeForDci3
(
    TriggeredUEListNode* node_p, 
    TpcRntiContext* tpcRntiContext_p,
    DciCCEInfo* dciCCEInfo_p,
    /* SPR 15909 fix start */
    tickType_t tick,
    /* SPR 15909 fix end */
    InternalCellIndex internalCellIndex
)
{
    UInt32  tempUeIdx = 0;
    /* SPR 15909 fix start */
    tickType_t  lstAppliedTick = 0;
    /* SPR 15909 fix end */
    UInt16  tpcBitMapIndex = 0;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    DLUEContext* dlUeCntxt_p = PNULL;
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t localTick = 0;
    /* SPR 15909 fix end */
#endif
#ifdef TDD_CONFIG
    UInt8 subframe = tick % MAX_SUB_FRAME;
#endif


    /* Get dlUeContext using ueIndex extracted from the node popped */
    tempUeIdx = node_p->ueIndex;

    /* Get lastAppliedTick and tpcBitMapIndex from ueContext */
    dlUeCntxt_p = dlUECtxInfoArr_g[tempUeIdx].dlUEContext_p;
    lstAppliedTick = dlUeCntxt_p->tpcInfoForPucch.lastAppliedTick;
    tpcBitMapIndex = dlUeCntxt_p->tpcInfoForPucch.tpcBitMapIndex;

    if ((DRX_STATE_ON == dlUeCntxt_p->drxContext.drxStateVariables.drxCurrentState) ||
        (TRUE == dlUeCntxt_p->measGapContext.isMeasGapPeriodOn)                     ||
        ((SPS_ACTIVATION_INITIATED == dlUeCntxt_p->spsDlInfo.spsState) && (SPS_SCHEDULING == dlUeCntxt_p->schType)))
    {
        /* If DRX or Meas Gap is ON then put the entry in explicit queue so that it can go when the 
         * data is scheduled */
        putEntryInExplicitDLDCIScheduleQueue(tempUeIdx, internalCellIndex);
        LOG_MAC_MSG(TPC_EXPLICIT_TRIGGER_Q_ENTRY_PUCCH_ID, LOGDEBUG, MAC_UL_PC,
                tick, tempUeIdx, dlUeCntxt_p->tpcInfoForPucch.tpcTriggeredValue,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "ADD_ENTRY_EXPLICIT_TRIGGER_QUEUE_PUCCH");

        tpcRntiContext_p->triggeredCount--;
        freeMemPool(node_p);
        /* Set triggeredUeListNodeInTpcRntiContext_p to PNULL  */
        dlUeCntxt_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL;
        return ;
    }

#ifdef HD_FDD_CONFIG            
    // HD FDD Changes
    localTick = getCurrentTick() + PHY_DL_DELAY;

#endif
    if((lstAppliedTick != tpcRntiContext_p->appliedUlTick) 
#ifdef HD_FDD_CONFIG            
            && 
            /*HD FDD Changes Start*/
            (((TRUE == dlUeCntxt_p->isHDFddFlag) && 
              (CAN_SCHEDULE_TPC_FOR_HD_UE(localTick, dlUeCntxt_p->ueIndex, hdUeContextInfo_g.hdfddUeSchedMap)) )|| 
             (FALSE == dlUeCntxt_p->isHDFddFlag))
#endif
     )
        /*HD FDD Changes End*/
    {
        powerCorrectionAndTpc = getAccumulatedPowerForPucch(
                dlUeCntxt_p->tpcInfoForPucch.tpcTriggeredValue,   
                DCI_FORMAT_3);

	/* SPR 13196 Changes start */
        /* Set tpcTriggerd at tpcRntiIndex in pDciPdu_p->tpcBitmap 
         * e.g if 10Mhz ,tpcBitMapIndex = 1 then bit7 and bit8 needs to be 
         * populated so from formula (16-1 *2) -6 = 24. so 
         * tpcTriggered will be left shifted to 24 bits to set
         * the bit7 and bit 8. */

        /*+- SPR 20899 Fix*/
        UInt8 offset = 
            ((MULTIPLY_BY_TWO(16 - tpcBitMapIndex)) -
             dci3RightShiftOffset_g[internalCellIndex]);
        /*+- SPR 20899 Fix*/

	/* SPR 13196 Changes end */
        /* clear the 2 bits from offet and then set it to the tpc command */
        dciCCEInfo_p->dciPduInfo.tpcBitmap &= ~(1 << offset);
        dciCCEInfo_p->dciPduInfo.tpcBitmap &= ~(1 << (offset+1));

        dciCCEInfo_p->dciPduInfo.tpcBitmap |= 
            powerCorrectionAndTpc.tpcCommand <<  offset;

        if (powerCorrectionAndTpc.powerCorrectionVal)
        {
            /* SPR 21631 Start */
            UInt8 mapIdx = tick % MAX_TPC_CONTAINER_MAP_SIZE;
            /* SPR 21631 End */
            UInt16 rnti = 0;

            dlUeCntxt_p->currPowerCorrectionForPucch += 
                powerCorrectionAndTpc.powerCorrectionVal;

            if ( SPS_SCHEDULING == dlUeCntxt_p->schType ) 
            { 
                rnti = dlUeCntxt_p->spsDlInfo.SpsCrnti;    
            }
            else
            {
                rnti = dlUeCntxt_p->crnti;    
            }

            /* Storing the tpc information to be sent it will be used if error 
             * indication rcvd from phy */
            /* Storing at current index, it will be fetched at receiver from 
             * current tick - ERROR_IND Delay */
            putEntryInTpcSentMap( tick, mapIdx, rnti, 
                    powerCorrectionAndTpc.powerCorrectionVal,
                    ERR_IND_TPC_FOR_PUCCH,
                    internalCellIndex);
        }
        LOG_MAC_MSG(TPC_PUCCH_CMD_ID, LOGDEBUG, MAC_UL_PC,
                0,
                dlUeCntxt_p->ueIndex,
                dlUeCntxt_p->tpcInfoForPucch.tpcRnti,
                dlUeCntxt_p->tpcInfoForPucch.tpcTriggeredValue,
                powerCorrectionAndTpc.tpcCommand,
                dlUeCntxt_p->currPowerCorrectionForPucch,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "ENCODE_DCI_FORMAT_3Or3A");

        /* reset the tpcInfoForPucch.tpcTriggeredValue to 0 
         * to indicate no_change */
        dlUeCntxt_p->tpcInfoForPucch.tpcTriggeredValue = TPC_NO_CHANGE;
        /* Set triggeredUeListNodeInTpcRntiContext_p to PNULL  */
        dlUeCntxt_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL;

#ifdef FDD_CONFIG
        dlUeCntxt_p->tpcInfoForPucch.lastAppliedTick  = 
            tick + 4 + PHY_UL_CONTROL_DELAY;
#elif TDD_CONFIG
        dlUeCntxt_p->tpcInfoForPucch.lastAppliedTick = 
/* SPR 11700 Start */
            tick + getDlAckNackOffset(subframe,internalCellIndex) + PHY_UL_CONTROL_DELAY;
/* SPR 11700 End */
#endif
        /* Time Averaging changes start */
        if ( cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
             cellParams_p->powerControlEnableInfo.cqiSinrClpcPucchEnable )
        {
            dlUeCntxt_p->powerControlPucchTpcValue = powerCorrectionAndTpc.powerCorrectionVal;
        }
        /* Time Averaging changes end */

        /* Reset the blerCalculated and blerCalculatedCounter to 0 */
        RESET_COOLOFFS_AND_BLER_CALCULATE_COUNTER(dlUeCntxt_p);
        /* Free the node 
         * Pop next node from the triggerdUeList
         */
        freeMemPool(node_p);
    }
    else
    {
#ifdef HD_FDD_CONFIG
        if((TRUE == dlUeCntxt_p->isHDFddFlag) && (lstAppliedTick != tpcRntiContext_p->appliedUlTick))
        {
            /*Putting into failure triggerlist and decrementing the trigger count
             * and increment the failure count*/
            /*put entry into the failure trigger list*/
            /* +- SPR 17777 */
            addEntryInFailTriggeredUeList(node_p, &(tpcRntiContext_p->failTriggerUeList)
#ifdef LOG_UT_ENABLED
                    , internalCellIndex
#endif
                    );
            /* +- SPR 17777 */
            /* Increment the statistics counter UE denied DL resources due to
             * UL clash in the same TTI of HD FDD UE*/
            UPDATE_UE_DENIED_DL_RESOURCE(dlUeCntxt_p->ueIndex);
        }
#endif
        tpcRntiContext_p->triggeredCount--;
        /* NOTE : No Need to free the node here since same node is being used in failTriggerUeList */
    }

    return ;
} /*end of processTpcUeTriggerNodeForDci3()*/
/*HD FDD Changes End*/

/*****************************************************************************
 * Function Name  : encodeTpcBitmapForPucch
 * Inputs         : count - count of TpcMsgInfoForPucch array,
 *                  tpcMsgInfoPucch_p - Pointer to TpcMsgInfoForPucch,
 *                  dciCCEContainerInfo_p - Pointer to the DciCCEContainerInfo,
 *                  tick - Current Tick
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall encode TPC bitmap for PUCCH power control
 *                  depending on the DCI FORMAT 3/3A scheduled for the TPC PUCCH RNTI.
 *                  The bit for non-triggered UEs is encoded with a fake TPC command.
 *                  A UE context variable fakeTpcValue is maintained to keep track
 *                  of fake commands sent for the UE. The range of values for fakeTpcValue
 *                  is 0, -1 and 1 where 0 means that the last TPC value sent was real;
 *                  -1 means that  the last TPC value sent was fake "-1"; and 1 means that
 *                  the last TPC value sent was fake "1".
 *****************************************************************************/
void encodeTpcBitmapForPucch(UInt32 count, 
        TpcMsgInfoForPucch * tpcMsgInfoPucch_p,
        DciCCEContainerInfo* dciCCEContainerInfo_p,
        /* SPR 15909 fix start */
        tickType_t tick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    TpcMsgInfoForPucch * dlTpcMsgInfo_p = tpcMsgInfoPucch_p; 
    DciCCEInfo *dciCCEInfo_p = PNULL;
    TpcRntiContext * tpcRntiContext_p = PNULL;
    TriggeredUEListNode * node_p = PNULL;
    /* SPR 22354 Start */
    ConfiguredUEListNode * configuredNode_p = PNULL;
    UInt32  tempUeIdx = 0;
    /* SPR 15909 fix start */
    tickType_t  lstAppliedTick = 0;
    /* SPR 15909 fix end */
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    UInt16  tpcBitMapIndex = 0;
    UInt8 cceIdx = 0;
#ifdef TDD_CONFIG
    /* SPR 22354 Start */
    UInt8 subframe = tick % MAX_SUB_FRAME;    
    /* SPR 22354 End */
#endif
    /*HD FDD Changes Start*/
    DLUEContext* dlUeCntxt_p = PNULL;
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t localTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif
    /*HD FDD Changes End*/
    /* SPR 22354 End */

    while(count--)
    {
        dlTpcMsgInfo_p = &(tpcMsgInfoPucch_p[count]);
        cceIdx = dlTpcMsgInfo_p->cceIndex;

        switch(dlTpcMsgInfo_p->dciFormat)
        {
            case DCI_FORMAT_3:
                {
                    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[cceIdx];
                    dciCCEInfo_p->dciPduInfo.tpcBitmap = DEFAULT_VAL_FOR_DCI3;

                    /* Get tpcRnti context using the dlTpcMsgInfo_p tpcRnti */
                    tpcRntiContext_p = (tpcPucchRntiContext_gp[internalCellIndex] + dlTpcMsgInfo_p->
                            tpcRntiIndex);

                    /* HD FDD Changes Start*/
                    /* No Need to check failTriggerUeList here since failTriggerUeList 
                     * is being used to repopulate triggerUEList in triggerDciForPucchPowerControl() 
                     * and made empty in that function */
                    /* HD FDD Changes End*/
                    /* pop node from triggeredUeList */
                    node_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                    while (node_p != NULL)
                    {
                        /* SPR 10644 changes start */
                        if(dlUECtxInfoArr_g[node_p->ueIndex].pendingDeleteFlag)
                        {
                            tpcRntiContext_p->triggeredCount--;
                            freeMemPool(node_p);
                            node_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                            continue;
                        }
                        /* SPR 10644 changes end */

                        /*HD FDD Changes Start*/
                        processTpcUeTriggerNodeForDci3(node_p, tpcRntiContext_p,
                                                       dciCCEInfo_p, tick, internalCellIndex);
                        node_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                        /*HD FDD Changes End*/
                    }

                    if (tpcRntiContext_p->triggeredCount == 0)
                    {
                        /*HD FDD Changes Start*/
                        /*Updating the triggercount with the failurecount*/
                        /*HD FDD Changes End*/
                        /* Cyclomatic Complexity changes */
                        mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                                      dciCCEInfo_p, internalCellIndex);
                    }
                    break;
                }

                /* SPR 22354 Start */
            case DCI_FORMAT_3A:
                {
                    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[cceIdx];
                    dciCCEInfo_p->dciPduInfo.tpcBitmap = DEFAULT_VAL_FOR_DCI3A;

                    /* Get tpcRnti context using the dlTpcMsgInfo_p tpcRnti */
                    tpcRntiContext_p = (tpcPucchRntiContext_gp[internalCellIndex] + dlTpcMsgInfo_p->tpcRntiIndex);

                    /*HD FDD Changes Start*/
                    /* No Need to check failTriggerUeList here since failTriggerUeList 
                     * is being used to repopulate triggerUEList in triggerDciForPucchPowerControl() 
                     * and made empty in that function */
                    /*HD FDD Changes End*/
                    /*DCI Format 3A changes start*/
                    /* pop node from triggeredUeList */
                    node_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                    while (node_p != PNULL)
                    {
                        /*HD FDD Changes Start*/
                        processTpcUeTriggerNodeForDci3A(node_p, tpcRntiContext_p,
                                                        dciCCEInfo_p, tick, internalCellIndex);
                        node_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                        /*HD FDD Changes End*/
                    }
                    /* If triggered Count becomes zero, no need to send the
                       DCI PDU */
                    if (tpcRntiContext_p->triggeredCount == 0)
                    {
                        /* Cyclomatic Complexity changes */
                        mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl, 
                                      dciCCEInfo_p, internalCellIndex);
                        break;
                    }

                    /* get the node from rnti's configuredList*/
                    configuredNode_p = (ConfiguredUEListNode*)getListNode(&(tpcRntiContext_p->\
                                configuredUeList),0);

                    /* Traversing the configuredUE list to mark the power change for untriggered UEs*/

                    while (configuredNode_p != PNULL)
                    {
                        if (!dlUECtxInfoArr_g[configuredNode_p->ueIndex].pendingDeleteFlag)
                        {
                            /* Get dlUeContext using ueIndex extracted from the node popped */
                            tempUeIdx = configuredNode_p->ueIndex;

                            /* Get lastAppliedTick and tpcBitMapIndex from ueContext */
                            dlUeCntxt_p = dlUECtxInfoArr_g[tempUeIdx].dlUEContext_p;
                            lstAppliedTick = dlUeCntxt_p->tpcInfoForPucch.lastAppliedTick;
                            tpcBitMapIndex = dlUeCntxt_p->tpcInfoForPucch.tpcBitMapIndex;

                            if (lstAppliedTick != tpcRntiContext_p->appliedUlTick)
                            {
                                if ( TPC_NO_CHANGE == dlUeCntxt_p->tpcInfoForPucch.tpcTriggeredValue)
                                {
                                    if (0 == dlUeCntxt_p->tpcInfoForPucch.fakeTpcValue)
                                    {
                                        /* fakeTpcValue equals 0 which means that the last TPC
                                         * value sent for this UE was real, hence send a fake "1" this time.
                                         */
                                        dlUeCntxt_p->tpcInfoForPucch.fakeTpcValue = FAKE_TPC_POWER_PLUS_1;
                                        powerCorrectionAndTpc.powerCorrectionVal = FAKE_TPC_POWER_PLUS_1;
                                        powerCorrectionAndTpc.tpcCommand = 1;
                                    }
                                    else if(1 == dlUeCntxt_p->tpcInfoForPucch.fakeTpcValue)
                                    {
                                        /* fakeTpcValue equals 1 which means that the last TPC
                                         * value sent for this UE was fake "1", hence send a fake
                                         * "-1" this time.
                                         */
                                        dlUeCntxt_p->tpcInfoForPucch.fakeTpcValue = FAKE_TPC_POWER_MINUS_1;
                                        powerCorrectionAndTpc.powerCorrectionVal = FAKE_TPC_POWER_MINUS_1;
                                        powerCorrectionAndTpc.tpcCommand = 0;
                                    }
                                    else
                                    {
                                        /* fakeTpcValue equals -1 which means that the last TPC
                                         * value sent for this UE was fake "-1", hence send a fake
                                         * "1" this time.
                                         */
                                        dlUeCntxt_p->tpcInfoForPucch.fakeTpcValue = FAKE_TPC_POWER_PLUS_1;
                                        powerCorrectionAndTpc.powerCorrectionVal = FAKE_TPC_POWER_PLUS_1;
                                        powerCorrectionAndTpc.tpcCommand = 1;
                                    }

                                    dlUeCntxt_p->currPowerCorrectionForPucch +=
                                        powerCorrectionAndTpc.powerCorrectionVal;

				    /* SPR 13196 Changes start */
	    			    /* Set tpcTriggerd at tpcRntiIndex in pDciPdu_p->tpcBitmap 
				     * e.g if 10Mhz ,tpcRntiIndex = 1 ,from formula (32-1) -5 = 26. 
				     * so tpcTriggered will be left shifted to 26 bits */
				    UInt8 offset = 
				              (((32 - tpcBitMapIndex)) -
                               dci3ARightShiftOffset_g[internalCellIndex]);
				    /* SPR 13196 Changes end */ 
                                    /* set the bit to the tpc command */
                                    dciCCEInfo_p->dciPduInfo.tpcBitmap |= powerCorrectionAndTpc.tpcCommand <<  offset;
                                }
                                else
                                {
                                    /*Reset the tpcTriggered valued if measGapPeriod is false and DRX state is off */
                                    if (((DRX_STATE_ON != dlUeCntxt_p->drxContext.drxStateVariables.drxCurrentState) &&
                                            (TRUE != dlUeCntxt_p->measGapContext.isMeasGapPeriodOn)) &&
                                         (!((SPS_ACTIVATION_INITIATED == dlUeCntxt_p->spsDlInfo.spsState) && 
                                        (SPS_SCHEDULING == dlUeCntxt_p->schType)))
#ifdef HD_FDD_CONFIG
                                            &&
                                            /*HD FDD Changes Start*/
                                            (((TRUE == dlUeCntxt_p->isHDFddFlag) &&
                                              (CAN_SCHEDULE_TPC_FOR_HD_UE(localTick, dlUeCntxt_p->ueIndex, hdUeContextInfo_g.hdfddUeSchedMap)))||
                                             (FALSE == dlUeCntxt_p->isHDFddFlag))
#endif
                                            )

                                    {
                                        dlUeCntxt_p->tpcInfoForPucch.tpcTriggeredValue = TPC_NO_CHANGE;
#ifdef FDD_CONFIG
                                        dlUeCntxt_p->tpcInfoForPucch.lastAppliedTick  = 
                                            tick + 4 + PHY_UL_CONTROL_DELAY;
#elif TDD_CONFIG
                                        dlUeCntxt_p->tpcInfoForPucch.lastAppliedTick = 
/* SPR 11700 Start */
                                            getDlAckNackOffset(subframe,internalCellIndex) + PHY_UL_CONTROL_DELAY;
/* SPR 11700 End */
#endif                       
                                    }  
                                    else
                                    {
                                    }
                                }
                            }
                        }
                        configuredNode_p =
                            (ConfiguredUEListNode*)getNextListNode(&configuredNode_p->configuredUeAnchor);
                    }
                    break;
                }
                /* SPR 22354 End */
            default:
                {
                    LTE_MAC_UT_LOG(LOG_INFO,PDSCH_MGR,"Format \
                            Not handled\n");
                    return;
                }
        } /* Switch end*/
        /* Reset the triggered Count in tpc rnti context */
        tpcRntiContext_p->triggeredCount = 0;
        tpcRntiContext_p->scheduledState = UNSET_SCHEDULED_STATE;
        /* DCI Format 3A changes end*/
    }/*while(count--)*/
}/*encodeTpcBitmapForPucch*/

/*HD FDD Changes Start*/
/*****************************************************************************
 * Function Name  : processTpcTriggerAndSchedFailQueueForPucch
 * Inputs         : node_p - Pointer to TriggeredUEListNode,
 *                  currentTick,
 *                  isListForFailHdUEs
*                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function process will process tpcSchedFailQueue  and 
 *                  tpcTriggerQueue. First tpcSchedFailQueue will be processed
 *                  and the tpcTriggerQueue will be processed.
 *****************************************************************************/
void processTpcTriggerAndSchedFailQueueForPucch(
    TpcTriggerQueueNode* node_p,
    /* SPR 15909 fix start */
    /* +- SPR 17777 */
    tickType_t currentTick,
    /* SPR 15909 fix end */
    UInt8 isListForFailHdUEs,
    InternalCellIndex internalCellIndex
)
{
    TpcInfoForPucch * tpcInfoForPucch_p = PNULL;
    TpcRntiContext *  tpcRntiContext_p = PNULL;
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t localTick = 0;
    /* SPR 15909 fix end */
#endif
    DLUEContext* dlUeCntxt_p = PNULL;
    TriggeredUEListNode * triggeredNode_p = PNULL;
    DLUEContext* dlUeContext_p = PNULL;

    /* Pop node from the queue */
    if (!dlUECtxInfoArr_g[node_p->ueIndex].pendingDeleteFlag)
    {
        dlUeCntxt_p = dlUECtxInfoArr_g[node_p->ueIndex].dlUEContext_p;
        tpcInfoForPucch_p = &(dlUeCntxt_p->tpcInfoForPucch);
        if (tpcInfoForPucch_p->tpcRnti == INVALID_TPC_RNTI)
        {
            if (TPC_NO_CHANGE != tpcInfoForPucch_p->tpcTriggeredValue)
            {
                /* If TPC RNTI for the UE is not configured then add the 
                   entry to explicit queue to be processed by strategy. 
                   Free the node and continue to process other node in the
                   tpcTriggerQueueForPusch_g */
                if (putEntryInExplicitDLDCIScheduleQueue(node_p->ueIndex, internalCellIndex))
                {
                    lteWarning("Error in Pushing in ExplicitDLDCI Queue %s\n",
                            __func__);
                }
                LOG_MAC_MSG(TPC_EXPLICIT_TRIGGER_Q_ENTRY_PUCCH_ID, LOGDEBUG, MAC_UL_PC,
                        currentTick, node_p->ueIndex, tpcInfoForPucch_p->tpcTriggeredValue,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "ADD_ENTRY_EXPLICIT_TRIGGER_QUEUE_PUCCH");
            }
        }
        else
        {
            tpcRntiContext_p =
                (tpcPucchRntiContext_gp[internalCellIndex] + tpcInfoForPucch_p->tpcRntiIndex);
            /* Valid TPC-RNTI */
            if (PNULL != tpcRntiContext_p)
            {
#ifdef HD_FDD_CONFIG
                /*HD FDD Changes Start*/
                /* Note : HD Flag check should be done before checking tpcRntiContet->ScheduledState 
                 * else failure queue process will not happen till the time next TPC will trigger for 
                 * Same TPC RNTI 
                 */
                localTick = getCurrentTick()+ PHY_DL_DELAY;
                /*Add a check for HD FDD UE whether TPC can be scheduled in current Tick
                 * else put the ndoe entry in the tpcFailQ*/
                if (TRUE == dlUeCntxt_p->isHDFddFlag && 
                        !(CAN_SCHEDULE_TPC_FOR_HD_UE(localTick, dlUeCntxt_p->ueIndex, hdUeContextInfo_g.hdfddUeSchedMap)))
                {
                    /*While putting entry in the failQ expiry tick will be same expiry tick received in the 
                     * function from TPC Trigger node*/
                    /* No need to Free the node popped */		
                    /*Put node entry back to failue Q*/
                    if (TPC_NO_CHANGE != tpcInfoForPucch_p->tpcTriggeredValue)
                    {
                        putEntryInTpcSchedFailQueueForPucch(node_p, internalCellIndex);
                        /* Increment the statistics counter UE denied DL resources due to
                         * UL clash in the same TTI of HD FDD UE*/
                        UPDATE_UE_DENIED_DL_RESOURCE(dlUeCntxt_p->ueIndex);
                        return;
                    }
                }
#endif
                /*HD FDD Changes End*/

                if ((TPC_NO_CHANGE != tpcInfoForPucch_p->tpcTriggeredValue) &&
                        (tpcRntiContext_p->scheduledState == UNSET_SCHEDULED_STATE))
                {
                    /* Check if there is atleast one UE triggered in the TPC-RNTI group 
                     * Note: PendingDelete check has been deliberately skipped since it will be checked 
                     * while encoding the TPC command. In the worst case, there is a possibility that
                     * the TPC-RNTI shall be added to schedule queue even if the only UE triggered in this
                     * TPC-RNTI group is pending to be deleted. However, by skipping this check here, we can
                     * handle the case where UE was marked pendingDelete and TPC-RNTI was released for the 
                     * triggered UE before it could be scheduled.
                     */
                    if ((tpcRntiContext_p->configuredCount > 0) && (tpcRntiContext_p->triggeredCount > 0))
                    {
                        if ((tpcRntiContext_p->isConfiguredForDciFormat3Or3A == DCI_FORMAT_3) ||
                                (((tpcRntiContext_p->triggeredCount*100)/
                                  tpcRntiContext_p->configuredCount) >= TPC_NUM_UE_THRESHOLD_FOR_PUCCH_DCI3A))
                        {
                            /* Add TPC RNTI entry to scheduler queue.*/
                            /* +- SPR 22354 */
                            putEntryInScheduledFormat3Or3AQueueForPucch(tpcRntiContext_p->tpcRntiIndex,
                                                                        internalCellIndex);
                            tpcRntiContext_p->scheduledState = 
                                tpcRntiContext_p->isConfiguredForDciFormat3Or3A == 
                                DCI_FORMAT_3?SCHEDULED_FORMAT3:SCHEDULED_FORMAT3A;
                        }
                        else
                        {
                            /* pop node from triggeredUeList */
                            if(isListForFailHdUEs == TRUE)
                            {
                                triggeredNode_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->failTriggerUeList));
                            }
                            else
                            {
                                triggeredNode_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                            }

                            while (PNULL != triggeredNode_p)
                            {
                                if (!dlUECtxInfoArr_g[triggeredNode_p->ueIndex].pendingDeleteFlag)
                                {
                                    putEntryInExplicitDLDCIScheduleQueue(triggeredNode_p->ueIndex, internalCellIndex);
                                    /* Reset triggeredUeListNodeInTpcRntiContext_p in UE context*/
                                    dlUeContext_p = dlUECtxInfoArr_g[triggeredNode_p->ueIndex].dlUEContext_p;
                                    dlUeContext_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL;
                                }
                                /* Free the node popped
                                 * Pop next node from the triggerdUeList
                                 */

                                freeMemPool(triggeredNode_p);


                                if(isListForFailHdUEs == TRUE)
                                {
                                    triggeredNode_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->failTriggerUeList));
                                }
                                else
                                {
                                    tpcRntiContext_p->triggeredCount--;
                                    triggeredNode_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                                }
                            }
                        }
                    }
                    else
                    {
                    }
                }
                else
                {
                }
            }
        }
    }
    else
    {
    }
    /* Free the node popped */		
    FREE_MEM_NODE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(node_p);
    return;
}
/*HD FDD Changes End*/

/*****************************************************************************
 * Function Name  : triggerDciForPucchPowerControl
 * Inputs         : currentTick - Current tick
*                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall process the tpcTriggeredQueuePucch_g. 
 *                  For each entry present in Queue, it shall trigger 
 *                  TPC commands for PUCCH power control, either using 
 *                  DCI format 3 /3A for the UE if TPC-RNTI is configured 
 *                  else using explicit DCI Format 1x /2x. 
 *****************************************************************************/
/* SPR 15909 fix start */
void triggerDciForPucchPowerControl(tickType_t currentTick,
/* SPR 15909 fix end */        
                                    InternalCellIndex internalCellIndex)
{
    /* Calculate the queueCount of tpcTriggeredQueuePucch_g */
    UInt32  qCount = COUNT_TPC_TRIGGERED_QUEUE_PUCCH_SQ(internalCellIndex);
    TpcTriggerQueueNode* node_p = PNULL;
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    UInt32 qFailCount = COUNT_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(internalCellIndex);
    TpcTriggerQueueNode* failQnode_p = PNULL;

    TpcInfoForPucch * tpcInfoForPucch_p = PNULL;
    TpcRntiContext *  tpcRntiContext_p = PNULL;
    DLUEContext* dlUeCntxt_p = PNULL;
    TriggeredUEListNode * triggeredNode_p = PNULL;

#endif
    /*HD FDD Changes End*/

    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* Process the TPC Fail Queue before processing TPC Trigger Queue
     * Fail Queue will be filled for HD FDD UE when TPC is triggered for it
     * but it cannot be scheduled for DL(because SF is meant for UL), 
     * hence in the subsequent ticks first 
     * priority will be given to these UE's for TPC in DCI 3 and 3A*/
    while(qFailCount--)
    {
        DEQUEUE_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(TpcTriggerQueueNode, failQnode_p, internalCellIndex);
        if (failQnode_p)
        {
            LOG_UT(TPC_TRIGGER_PUCCH, LOGDEBUG, MAC_UL_PC,
            0,
            currentTick, failQnode_p->expiryTick,
            qFailCount+1,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,
            "PUCCH_TRIGGER_Q_PROCESS");
            /* No need to check expiry tick for tpcSchedFailQueuePucch_g 
             * queue because the nodes have been pushed into it only 
             * after expiry during processing of tpcTriggeredQueuePucch_g*/

            dlUeCntxt_p = dlUECtxInfoArr_g[failQnode_p->ueIndex].dlUEContext_p;
            tpcInfoForPucch_p = &(dlUeCntxt_p->tpcInfoForPucch);
            tpcRntiContext_p =
                (tpcPucchRntiContext_gp[internalCellIndex] + tpcInfoForPucch_p->tpcRntiIndex);
            if (tpcRntiContext_p != PNULL)
            {
                /* Repopulate the TriggereUeList from failTriggerUeList so that only 1 list will be processed
                 * to keep complexity low.
                 */
                UInt32 lstCount = listCount(&(tpcRntiContext_p->failTriggerUeList));
                while(lstCount != 0)
                {
                    triggeredNode_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->failTriggerUeList));
                    /* Decrement the failTriggerUeList */
                    /*Coverity Fix Start*/
                    if (triggeredNode_p == PNULL)
                    {
                        // Should not come here!!
                        lstCount--;
                        continue;
                    }
                    /*HD FDD Coverity Fix End*/
                    triggeredNode_p = addEntryInTriggeredUeList(triggeredNode_p->ueIndex,
                                       &(tpcRntiContext_p->triggeredUeList), triggeredNode_p);
                    if (triggeredNode_p == PNULL)
                    {
                        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                LOGERROR,MAC_UL_Strategy,\
                                getCurrentTick(),\
                                DEFAULT_INT_VALUE,\
                                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,__func__,
                                "triggeredNode_p entry failed");
                        lstCount--;
                        continue;
                    }
                    /*Coverity Fix End*/
                    tpcRntiContext_p->triggeredCount++;
                    lstCount--;
                }
            }

            processTpcTriggerAndSchedFailQueueForPucch(failQnode_p,
                                                       /* +- SPR 17777 */
                                                       currentTick,
                                                       TRUE,
                                                       internalCellIndex);
        }
        else
        {
            // * lteWarning not used any more. 
            lteWarning("Error : Node popped is NULL %s" , __func__);
            break;
        }
    }
#endif
    /*HD FDD Changes End*/
    while (qCount--)
    {
        GET_NODE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(TpcTriggerQueueNode, node_p, internalCellIndex);
        if (node_p)
        {
            LOG_UT(TPC_TRIGGER_PUCCH, LOGDEBUG, MAC_UL_PC,
            0,
            currentTick, node_p->expiryTick,
            qCount+1,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,
            "PUCCH_TRIGGER_Q_PROCESS");
            
            if (currentTick > node_p->expiryTick)
            {
                /* Pop node from the queue */
                DEQUEUE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(TpcTriggerQueueNode, node_p, internalCellIndex);
#ifdef PRINT
                fprintf(stderr,"[%s] [%d], UE ID [%d] Going to process queue tpcTriggeredQueuePucch_g \n", __func__, getCurrentTick(), node_p->ueIndex);
#endif
                /*HD FDD Coverity Fix Start*/
                if(PNULL == node_p)
                {
                    continue;
                }
                /*HD FDD Coverity Fix End*/
                /*HD FDD Changes Start*/
                
                processTpcTriggerAndSchedFailQueueForPucch(node_p,/* +- SPR 17777 */
                                                           currentTick, FALSE,
                                                           internalCellIndex);
                /*HD FDD Changes End*/
            }
            else/* currentTick > node_p->expiryTick */
            {
                /* Case when expiry of node is not reached.
                 * Since nodes are inserted in FIFO order of their expiry,
                 * thus no need to process other nodes also */
                break;
            }
        }
        else
        {
            // *  lteWarning not used any more. 
            lteWarning("Error : Node popped is NULL " , __func__);
            continue;
        }
    }
}

/*****************************************************************************
* Function Name  : putEntryInScheduledFormat3Or3AQueueForPucch
* Inputs         : tpcRntiIndex - Represents the TPC RNTI
*                  internalCellIndex - Cell-Index used at MAC
* Outputs        : None
* Returns        : None
* Description    : This function shall put an entry in
*                  scheduledFormat3Or3AQueueForPucch_g.
*****************************************************************************/
void putEntryInScheduledFormat3Or3AQueueForPucch(UInt16 tpcRntiIndex, InternalCellIndex internalCellIndex)
{
    ScheduledFormat3Or3AQueueNode * node_p = PNULL;

    /* Get memory pool */
    GET_MEM_FROM_POOL(ScheduledFormat3Or3AQueueNode, node_p,
            sizeof(ScheduledFormat3Or3AQueueNode), PNULL);

    if(node_p != PNULL)
    {
        node_p->tpcRntiIndex = tpcRntiIndex;
        /* COVERITY CID 25447 Fix start */
        if ( pushNode( scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex],
                &(node_p->nodeAnchor)) )
        {
            lteWarning ("pushNode() failed for scheduled Format3Or3A Queue For Pucch");
            freeMemPool (node_p);
        }
        /* COVERITY CID 25447 Fix end */
    }
}

/*****************************************************************************
* Function Name  : getTPCForPUCCH
* Inputs         : dlUeContext_p - pointer to DLUE Context,
*                  currentTick- Indicates the current global tick,
*                  dciFormat - DCI format
*                  internalCellIndex - Cell-Index used at MAC
* Outputs        : None
* Returns        : Power for PUCCH
* Description    : This function will calculate the TPC value for PUCCH
*                  SINR 
*****************************************************************************/
TpcTriggeredToPowerCorrection getTPCForPUCCH(DLUEContext * dlUeContext_p,
                    /* SPR 15909 fix start */
                     tickType_t currentTick, 
                     /* SPR 15909 fix end */
                     UInt8 dciFormat
                     ,InternalCellIndex internalCellIndex)
{
    SInt8   tpcCmd = TPC_NO_CHANGE;
    TpcRntiContext *tpcRntiContext_p = PNULL;
    TriggeredUEListNode * listNode_p = PNULL;

    tpcCmd = dlUeContext_p->tpcInfoForPucch.tpcTriggeredValue;
    if (tpcCmd != TPC_NO_CHANGE)
    {
#ifdef FDD_CONFIG
        dlUeContext_p->tpcInfoForPucch.lastAppliedTick = currentTick + 4 + PHY_UL_CONTROL_DELAY;
#elif TDD_CONFIG
        UInt8 subFrameNum = currentTick % MAX_SUB_FRAME;
        dlUeContext_p->tpcInfoForPucch.lastAppliedTick = 
/* SPR 11700 Start */
            currentTick + getDlAckNackOffset(subFrameNum,internalCellIndex) + PHY_UL_CONTROL_DELAY;
/* SPR 11700 End */
#endif
        /* DCI Format 3A change start */
        /*Reset fakeTpcValue flag of UE context*/
        dlUeContext_p->tpcInfoForPucch.fakeTpcValue = FAKE_TPC_NO_VALUE; 
        /* DCI Format 3A change end */

        /* Reset tpcTriggeredValue to NO CHANGE */
        dlUeContext_p->tpcInfoForPucch.tpcTriggeredValue = TPC_NO_CHANGE;
        // Reset the blerCalculated and blerCalculatedCounter to 0
        RESET_COOLOFFS_AND_BLER_CALCULATE_COUNTER(dlUeContext_p);

        /* If  tpcRnti in tpcInfo is not invalid */
        if (dlUeContext_p->tpcInfoForPucch.tpcRnti != INVALID_TPC_RNTI)
        {
            /* Get triggeredUeListNodeInTpcRntiContext_p from dlUeContext_p */
            listNode_p = dlUeContext_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p;
            if (listNode_p)
            {
                tpcRntiContext_p = (tpcPucchRntiContext_gp[internalCellIndex] +
                                    dlUeContext_p->tpcInfoForPucch.tpcRntiIndex);

                /* Delete node triggeredUeListNodeInTpcRntiContext_p 
                 * from triggeredUeList */
                deleteEntryFromTriggeredUeList(listNode_p, tpcRntiContext_p);

                dlUeContext_p->tpcInfoForPucch.triggeredUeListNodeInTpcRntiContext_p = PNULL;

            }
        }/* End if (tpcRnti != INVALID_TPC_RNTI) */
    } /* END if(tpcCmd != TPC_NO_CHANGE) */

    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    powerCorrectionAndTpc = getAccumulatedPowerForPucch(tpcCmd, dciFormat);
    if (powerCorrectionAndTpc.powerCorrectionVal)
    {
        /* SPR 21631 Start */
        UInt8 mapIdx = currentTick  % MAX_TPC_CONTAINER_MAP_SIZE;
        /* SPR 21631 End */
        UInt16 rnti = 0;

        dlUeContext_p->currPowerCorrectionForPucch += powerCorrectionAndTpc.powerCorrectionVal;

        /* Time Averaging changes start */
        if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
              cellParams_p->powerControlEnableInfo.cqiSinrClpcPucchEnable)
        {
            /*This value will be used for time averaging of TPC when PUCCH SINR is received after lastAppliedTick*/
            dlUeContext_p->powerControlPucchTpcValue += powerCorrectionAndTpc.powerCorrectionVal; 
        }
        /* Time Averaging changes end */
    
        if ( SPS_SCHEDULING == dlUeContext_p->schType ) 
        { 
            rnti = dlUeContext_p->spsDlInfo.SpsCrnti;    
        }
        else
        {
            rnti = dlUeContext_p->crnti;    
        }

        /* Storing the tpc information to be sent it will be used if error 
         * indication rcvd from phy */
        /* Storing at current index.
         * it will be fetched at receiver from current tick - ERROR_IND Delay */
        putEntryInTpcSentMap( currentTick, mapIdx, rnti, 
                powerCorrectionAndTpc.powerCorrectionVal,
                ERR_IND_TPC_FOR_PUCCH,
                internalCellIndex);
    }

    LOG_MAC_MSG(TPC_PUCCH_CMD_ID, LOGDEBUG, MAC_UL_PC,
            getCurrentTick(),
            dlUeContext_p->ueIndex,
            dlUeContext_p->tpcInfoForPucch.tpcRnti,
            tpcCmd,
            powerCorrectionAndTpc.tpcCommand,
            dlUeContext_p->currPowerCorrectionForPucch,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,
            "GET_TPC_FOR_PUCCH");

    return powerCorrectionAndTpc;
}
                        
/********************************************************************************
* Function Name  : initTpcRntiContextForPucch
* Inputs         : internalCellIndex - Cell-Index used at MAC
* Outputs        : None
* Returns        : None
* Description    : This function based on startTpcRntiPucch and endTpcRntiPucch
*                  value initializes the tpc rnti pool and tpc rnti context
*                  for pucch.
*********************************************************************************/
void initTpcRntiContextForPucch (InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    UInt8 maxTpcRntiRange = (cellParams_p->tpcRntiRange.endTpcRntiPucch - 
            cellParams_p->tpcRntiRange.startTpcRntiPucch + 1);

    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof(LTE_SQUEUE), 1);
        createMemPool(sizeof(TPC_TRIGGERED_QUEUE_PUCCH_Q), 1);
        createMemPool(sizeof(TPC_SCHED_FAIL_QUEUE_PUCCH_Q), 1);
        /* Start 128 UE: Memory Changes */
        /*
         ** The memory pool for TpcRntiContext is allocated only once
         ** for complete TPC RNTI Range rather than allocate for one tpc
         ** rnti, therefore a number of block is 1.
         */
        createMemPool((sizeof(TpcRntiContext) * maxTpcRntiRange), 1);
        /* End 128 UE: Memory Changes */
        createMemPool( sizeof(ScheduledFormat3Or3AQueueNode), maxTpcRntiRange);
    }

    GET_MEM_FROM_POOL(LTE_SQUEUE, scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex],
            sizeof(LTE_SQUEUE), PNULL);
    sQueueInit (scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex]);

    INIT_TPC_TRIGGERED_QUEUE_PUCCH_SQ(internalCellIndex);
    /*HD FDD Changes Start*/
    INIT_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(internalCellIndex);
    /*HD FDD Changes End*/

    GET_MEM_FROM_POOL( 
            TpcRntiContext, 
            tpcPucchRntiContext_gp[internalCellIndex], 
            (sizeof(TpcRntiContext) * maxTpcRntiRange),
            PNULL);
    /* Coverity CID 54417 Start*/
    if(PNULL == tpcPucchRntiContext_gp[internalCellIndex])
    {
        ltePanic("GET_MEM_FROM_POOL failed at %s[%d]",__FUNCTION__, __LINE__);
        return;
    }    
    /* Coverity CID 54417 End*/
    /* Cyclomatic Complexity changes start */
    initListsForTpcRntiContextForPucch(internalCellIndex, maxTpcRntiRange);

    /* Cyclomatic Complexity changes end */
} /* initTpcRntiContextForPucch */

/********************************************************************************
* Function Name  : initListsForTpcRntiContextForPucch
* Inputs         : internalCellIndex - Cell-Index used at MAC
*                  maxTpcRntiRange -
* Outputs        : None
* Returns        : None
* Description    : This function  initializes the lists in tpc rnti context
*                  for pucch. This is called at cell config
*********************************************************************************/
STATIC  void initListsForTpcRntiContextForPucch(InternalCellIndex internalCellIndex,
                                                      UInt8 maxTpcRntiRange)
{
    UInt8 tpcRntiIndex = 0;
    TpcRntiContext * tpcRntiContext_p = PNULL;
    CellConfigParams *cellParams_p = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt16 rnti = cellParams_p->tpcRntiRange.startTpcRntiPucch;

    for( tpcRntiIndex = 0; tpcRntiIndex < maxTpcRntiRange; tpcRntiIndex++)
    {
        listInit( &((tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex)->triggeredUeList));
        /* Rel 5.3.1: Memory related changes start*/
        /* this list will not require any extar pool as triggered and fail list 
         * will be exclusive to each other */
        listInit( &((tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex)->failTriggerUeList));
        /* Rel 5.3.1: Memory related changes end*/

        /* The mem pool will be created only first time when cell was configured */ 
        if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
        {
            createMemPool( 
                    sizeof(TriggeredUEListNode), 
                    MAX_UE_SUPPORTED_FOR_DCI_3_OR_3A);

            createMemPool( 
                    sizeof(ConfiguredUEListNode), 
                    MAX_UE_SUPPORTED_FOR_DCI_3_OR_3A);
        }

        /* DCI Format3A changes start*/
        listInit( &((tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex)->configuredUeList));

        /* DCI Format3A changes end*/

        tpcRntiContext_p = (tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex);

        tpcRntiContext_p->configuredCount = 0;
        tpcRntiContext_p->triggeredCount = 0;
        tpcRntiContext_p->tpcRntiIndex = tpcRntiIndex;
        tpcRntiContext_p->tpcRnti = rnti;
        tpcRntiContext_p->scheduledState = UNSET_SCHEDULED_STATE;
        rnti++;
    }
}
/* Rel 5.3.1: Memory related changes start*/
/********************************************************************************
* Function Name  : cleanupTpcRntiContextForPucch
* Inputs         : internalCellIndex - Cell-Index used at MAC 
* Outputs        : None
* Returns        : None
* Description    : This function based on startTpcRntiPucch and endTpcRntiPucch
*                  value cleanup the tpc rnti pool and tpc rnti context
*                  for pucch. This is called at cell stop
*********************************************************************************/
void cleanupTpcRntiContextForPucch (InternalCellIndex internalCellIndex)
{
    UInt8 tpcRntiIndex = 0;
    TpcRntiContext * tpcRntiContext_p = PNULL;
    UInt16 loopIndex=0, count=0;
    TriggeredUEListNode *tNode_p = PNULL;
    ConfiguredUEListNode *cNode_p = PNULL;
    UInt8 maxTpcRntiRange = (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                             cellParams_p->tpcRntiRange.endTpcRntiPucch - 
                             cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                             tpcRntiRange.startTpcRntiPucch + 1);
    /* Memory cleanup changes start*/
    TpcTriggerQueueNode* node_p = PNULL;
    ScheduledFormat3Or3AQueueNode * scheduledFormat3Or3AQueueNode_p = PNULL;

    count = sQueueCount(scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex]);
    while(count)
    {
        scheduledFormat3Or3AQueueNode_p = 
            (ScheduledFormat3Or3AQueueNode*)\
            popNode(scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex]);
        if (scheduledFormat3Or3AQueueNode_p)
        {
            freeMemPool(scheduledFormat3Or3AQueueNode_p);
        }
        count--;
    }

    count = COUNT_TPC_TRIGGERED_QUEUE_PUCCH_SQ(internalCellIndex);
    while (count)
    {
        DEQUEUE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(TpcTriggerQueueNode, node_p, internalCellIndex);
        if (node_p)
        {
            FREE_MEM_NODE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(node_p);
        }
        count--;
    }

    count = COUNT_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(internalCellIndex);
    while (count)
    {
        DEQUEUE_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(TpcTriggerQueueNode, node_p, internalCellIndex);
        if (node_p)
        {
            FREE_MEM_NODE_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(node_p);
        }
        count--;
    }
    /* Memory cleanup changes end*/

    for( tpcRntiIndex = 0; tpcRntiIndex < maxTpcRntiRange; tpcRntiIndex++)
    {
        tpcRntiContext_p =
            (tpcPucchRntiContext_gp[internalCellIndex] + tpcRntiIndex);

        count = listCount(&(tpcRntiContext_p->configuredUeList));
        for ( loopIndex= 0; loopIndex < count; loopIndex++)
        {
            cNode_p =
                (ConfiguredUEListNode *)getFirstListNode(&(tpcRntiContext_p->configuredUeList));

            if (cNode_p)
            {
                listDeleteNode(&(tpcRntiContext_p->configuredUeList), &(cNode_p->configuredUeAnchor) );
                freeMemPool(cNode_p);
                cNode_p = PNULL;
            }
        }

        count = listCount(&(tpcRntiContext_p->triggeredUeList));
        for ( loopIndex= 0; loopIndex < count; loopIndex++)
        {
            tNode_p =
                (TriggeredUEListNode *)getFirstListNode(&(tpcRntiContext_p->triggeredUeList));

            if (tNode_p)
            {
                listDeleteNode(&(tpcRntiContext_p->triggeredUeList), &(tNode_p->triggeredUeAnchor));
                freeMemPool(tNode_p);
                tNode_p = PNULL;
            }
        }

        count = listCount(&(tpcRntiContext_p->failTriggerUeList));
        for ( loopIndex= 0; loopIndex < count; loopIndex++)
        {
            tNode_p =
                (TriggeredUEListNode *)getFirstListNode(&(tpcRntiContext_p->failTriggerUeList));

            if (tNode_p)
            {
                listDeleteNode(&(tpcRntiContext_p->failTriggerUeList), &(tNode_p->triggeredUeAnchor) );
                freeMemPool(tNode_p);
                tNode_p = PNULL;
            }
        }
    }
    return;
}

/********************************************************************************
* Function Name  : deInitTpcRntiContextForPucch
* Inputs         : internalCellIndex - Cell-Index used at MAC
* Outputs        : None
* Returns        : None
* Description    : This function cleans up the memory created in initTpcRntiContextForPucch
*                  should be called at the time of cell delete.
*                  
*********************************************************************************/
void deInitTpcRntiContextForPucch (InternalCellIndex internalCellIndex)
{
    if(tpcPucchRntiContext_gp[internalCellIndex])
    {
        freeMemPool(tpcPucchRntiContext_gp[internalCellIndex]);
        tpcPucchRntiContext_gp[internalCellIndex] = PNULL;
    }

    if(scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex])
    {
        freeMemPool(scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex]);
        scheduledFormat3Or3AQueueForPucch_gp[internalCellIndex] = PNULL;
    }
    if(tpcTriggeredQueuePucch_gp[internalCellIndex])
    {
        freeMemPool(tpcTriggeredQueuePucch_gp[internalCellIndex]);
        tpcTriggeredQueuePucch_gp[internalCellIndex] = PNULL;
    }
    if(tpcSchedFailQueuePucch_gp[internalCellIndex])
    {
        freeMemPool(tpcSchedFailQueuePucch_gp[internalCellIndex]);
        tpcSchedFailQueuePucch_gp[internalCellIndex] = PNULL;
    }
}
/* Rel 5.3.1: Memory related changes end*/

/* + SPR 22354 */
/*****************************************************************************
 * Function Name  : macCheckIfAnyVictimUe
 * Inputs         : tpcRntiContext_p - TPC RNTI Context
 * Outputs        : None
 * Returns        : Success/Failure
 * Description    : This function checks in the TPC configured list, if the UE
 *                  is Victim or Non-Victim
 ******************************************************************************/
MacRetType macCheckIfAnyVictimUe( TpcRntiContext *tpcRntiContext_p )
{    
    ConfiguredUEListNode *configuredNode_p = PNULL;
    MacRetType retVal = MAC_FAILURE;

    /* Get the list node for UEs for the TPC */
    configuredNode_p = (ConfiguredUEListNode*)getListNode(
            &(tpcRntiContext_p->configuredUeList), 0 );
    while( PNULL != configuredNode_p )
    {
        UInt16 ueIndex = configuredNode_p->ueIndex;
        if ( FALSE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
        {
            DLUEContext *dlUeCtx_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
            /* Check if UE is VICTIM or NON-VICTIM UE */
            if( VICTIM_UE == dlUeCtx_p->userType )
            {
                retVal = MAC_SUCCESS;
                break;
            }
        }
        configuredNode_p = (ConfiguredUEListNode*)getNextListNode(
                &configuredNode_p->configuredUeAnchor );
    }

    return retVal;
}
/* - SPR 22354 */
