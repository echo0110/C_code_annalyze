/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLRoundRobinScheduler.c,v 1.3.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacDLRoundRobinScheduler.c,v $
 * Revision 1.3.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.43  2009/11/11 14:03:46  gur11912
 * fixed when dlNonZeroListStartRoundRobin_p is PNULL
 *
 * Revision 1.42  2009/11/10 12:45:13  gur11912
 * modified for warning UE not present in non zero lists
 *
 * Revision 1.41  2009/11/06 13:04:22  gur11912
 * HARQ is not freed in case of retransmissions
 *
 * Revision 1.40  2009/10/05 08:04:49  gur11912
 * optimization changes
 *
 * Revision 1.39  2009/09/02 06:07:39  gur20052
 * Added warning in UT_TESTING FLAG
 *
 * Revision 1.38  2009/08/07 12:24:25  gur20052
 * UT Defects is fixed
 *
 * Revision 1.37  2009/08/04 15:25:12  gur20052
 * Added more LOGS related to subnband info
 *
 * Revision 1.36  2009/07/30 05:21:40  gur11912
 * updated availableRBs only when entry in retx queue is successful
 *
 * Revision 1.35  2009/07/24 12:39:34  gur20052
 * Splint Warnings are removed
 *
 * Revision 1.34  2009/07/22 14:18:50  gur20052
 * UT Defect are fixed
 *
 * Revision 1.33  2009/07/20 17:45:04  gur20052
 * UT Defects are Fixed
 *
 * Revision 1.32  2009/07/17 11:56:27  gur20052
 * UT Defects are fixed
 *
 * Revision 1.31  2009/07/17 07:30:39  gur20052
 * UT BUG Fixed for HARQ_TB_MINIMUM_SIZE_IN_BYTES
 *
 * Revision 1.30  2009/07/16 17:36:14  gur20052
 * Added code for Setting up of aperiodicCQIRequested flag in DL UE Context and UT Bug Fix
 *
 * Revision 1.29  2009/07/15 05:59:30  gur20052
 * UT Defects fixed
 *
 * Revision 1.27  2009/07/13 07:18:30  gur20052
 * removed UT Bugs
 *
 * Revision 1.26  2009/07/10 15:11:09  gur20052
 * changed the log levels from LOG_INFO into LOG_WARNING
 *
 * Revision 1.25  2009/07/10 13:50:38  gur20052
 * MACModuleLogDetail_g is replaced with the Module Specific STRINGS
 *
 * Revision 1.24  2009/07/02 10:57:36  gur20052
 * Enhancement Comments are incorporated
 *
 * Revision 1.23  2009/06/29 15:09:13  gur20052
 * UT Defects are removed
 *
 * Revision 1.22  2009/06/25 09:55:42  gur20052
 *  Updated after UT Fix
 *
 * Revision 1.21  2009/06/24 08:10:45  gur20052
 * Reveiw Comments are incorporated
 *
 * Revision 1.20  2009/06/23 16:55:42  gur20052
 * Review comments are incorporated
 *
 * Revision 1.18  2009/06/18 06:44:20  gur20052
 * UT Defects are removed
 *
 * Revision 1.17  2009/06/11 14:52:56  gur20052
 * Removed the Linking Errors and Resource Allocator Node
 *
 * Revision 1.16  2009/06/10 14:26:07  gur20052
 * Removed the Linking errors and updated for createMemPool changes
 *
 * Revision 1.15  2009/06/08 09:59:34  gur20052
 * removed the linking errors
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
 * Revision 1.5  2009/05/21 06:24:55  gur20052
 * Added code for round robin Scheduler
 *
 * Revision 1.4  2009/05/20 16:56:04  gur20052
 * Updated the initial version
 *
 * Revision 1.3  2009/05/19 14:29:31  gur20052
 * Updated with DL Fair Scheduler module specific changes
 *
 * Revision 1.2  2009/05/11 06:18:32  gur20052
 * Removed compilation errors
 *
 * Revision 1.1  2009/04/30 16:27:35  gur20052
 * Added initial version of DL Round Robin Scheduler
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
#include "lteMacDLRoundRobinScheduler.h"
/* + SPR 16430 Changes */
#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"
#endif
/* - SPR 16430 Changes */

/******************************************************************************
  Private Definitions
 *****************************************************************************/

STATIC  void allocateResourcesToUEsOnRRBasis(
        DLUserListRR *dlUserList,
        UInt32 allocatedRBFromRoundRobin,
        UInt32 *availableRB_p,
        DLUEContext *ueDLContext_p,
        MsgType msgType,
        DLHARQProcess *dlHarqProcess_p,
        InternalCellIndex internalCellIndex,
        UInt16 index);
/******************************************************************************
  Private Types
 *****************************************************************************/
static UInt32 unusedRBsDL_g[MAX_NUM_CELL] = {0};
/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 /*  ICIC changes start */
MacRetType processDLPriorityCellEdgeUserList(
         UInt32 pendingCEUserCount,
         DLUserListRR *dlUserList_p,
         UInt8 *maxUEsToBeScheduledDL_p,
         /*SPR 21068 start */
         tickType_t dlReferenceFlag,
         /*SPR 21068 end   */
         /* SPR 15909 fix start */
         /* +- SPR 17777 */
#ifdef HD_FDD_CONFIG
         tickType_t currentGlobalTTITickCount,
#endif
         /* +- SPR 17777 */
         /* SPR 15909 fix end */
         InternalCellIndex internalCellIndex);
 /*  ICIC changes end */

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/*A pointer to nodeAnchor of the UE in ueNonZeroQueueLoadList_g*/
LTE_LIST_NODE * dlNonZeroListStartRoundRobin_p[MAX_NUM_CELL] = {PNULL};

/* ICIC changes start */
/* A List to maintain pending Cell edge UE's */
 LTE_SQUEUE* dlCellEdgeUserPriorityQueue_gp[MAX_NUM_CELL] = {PNULL};
/* ICIC changes end */
/* EICIC +*/
extern UInt8 ueTypeToScheduleDL_g[MAX_NUM_CELL];
/*EICIC -*/


/* SPR 15446 Fix Start */
extern UInt16 dlSchedulableUECount_g;  
/* SPR 15446 Fix End */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/* + CL-MIMO-LJA*/
/***************************************************************************************
  * Function Name  : processDLRoundRobinNewDataWithOneTBOnlyRIOne
  * Inputs         : allocatedRBFromRoundRobin - The Number of RBs Allocated Orginally,
  *                  availableRB_p - Pointer to the number of Avavailable RBs,
  *                  ueDLContext_p - Pointer to UE Context,
  *                  harqProcess_p -  Pointer to DL Harq UE Context,
  *                  numLCWithNonZeroQueueLoad-Number of LCs with non Zero Queue load,
  *                  msgType - informs about the message type whetehr the TA DRX
  *                            is requested or not,
  *                  currentGlobalTTITickCount - Current Global TTI Count,
  *                  tempStrategyInputInfoForOneTB,
  *                  internalCellIndex - cell index used at MAC.
  * Outputs        : Process the UE for One TB with New Data
  * Returns        : MAC_SUCCESS or MAC_FAILURE
  * Description    : This function is for processing the UE for New Transmission
  *                  For one TB only for transmission mode 3 or 4 and RI=1.
  *                  First pick up a fresh harq process and then
  *                  validate the RBs allocated. After which output is prepared
  *                  and sent to PDCCH Module.
  *************************************************************************************/
static MacRetType processDLRoundRobinNewDataWithOneTBOnlyRIOne(
  UInt32 allocatedRBFromRoundRobin,
  UInt32 * availableRB_p,
  DLUEContext* ueDLContext_p,
  DLHARQProcess * harqProcess_p,
  UInt32 numLCWithNonZeroQueueLoad,
  MsgType msgType
#ifdef TDD_CONFIG
  /* SPR 15909 fix start */
  , tickType_t currentGlobalTTITickCount
  /* SPR 15909 fix end */
#endif
  /* + CQI_5.0 */
  ,TempStrategyInputInfoForOneTB tempStrategyInputInfoForOneTB
  ,InternalCellIndex internalCellIndex
  /* - CQI_5.0 */
        )
{
    UInt32 usedRB = 0;
    UInt32 allocatedRB = 0;
    UInt32 tbSize = 0;   
    UInt32 minTBS = 0;
    UInt32 maxTBS = 0;
    /* ICIC changes start */
    UInt8 userLocationType = ueDLContext_p->userLocationType;
    /* ICIC changes end */

    ResourceAllocatorInput * tempResourceAllocatorInput_p = PNULL;
    DLStrategyTxNode * dlStrategyTxNode_p=PNULL;
#ifdef TDD_CONFIG
    UInt8 subFrame = currentGlobalTTITickCount % MAX_SUBFRAME;
#endif
    allocatedRB = allocatedRBFromRoundRobin + unusedRBsDL_g[internalCellIndex];
    /*CA_CHG_END*/
    LOG_UT(MAC_RR_UNUSED_RB_ADDED_TO_ALLOCATED_RB,LOGDEBUG,MAC_DL_Strategy,
            getCurrentTick(),
            unusedRBsDL_g[internalCellIndex],allocatedRBFromRoundRobin,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,LINE_NUMBER,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"");
    usedRB = validateRBForOneTB(ueDLContext_p, 
            /* ICIC changes start */
            allocatedRB, 
            /* ICIC changes end */
            numLCWithNonZeroQueueLoad,
            &tbSize,
            *availableRB_p,
            /* SPR 17777 */
            msgType,&minTBS,&maxTBS
#ifdef TDD_CONFIG
            , subFrame
            ,internalCellIndex
#endif
            /* SPR 17777 */
            );

    /* ICIC changes start */
    if(allocatedRB > usedRB)
    {
        LOG_UT(MAC_RR_SUBTRACT_USED_RB_FROM_ALLOCATED_RB,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                usedRB,allocatedRB,
                unusedRBsDL_g[internalCellIndex],
                (allocatedRB - usedRB),LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        unusedRBsDL_g[internalCellIndex] = allocatedRB - usedRB;
    }
    else
    {
        LOG_UT(MAC_RR_MAKING_UNUSED_RB_ZERO,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                usedRB,allocatedRB,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        unusedRBsDL_g[internalCellIndex] = 0;
    }
    /* ICIC changes end */

    if (usedRB)
    {
        /* + CQI_5.0 */
        /*Setting TB valid based on RI and CQI conditions*/
        harqProcess_p->isTBOneValid = tempStrategyInputInfoForOneTB.isTBOneValid;
        harqProcess_p->isTBTwoValid = tempStrategyInputInfoForOneTB.isTBTwoValid;
        /* - CQI_5.0 */
        /*Preparing the DL Strategy Output Node*/
        dlStrategyTxNode_p =(DLStrategyTxNode*)getMemFromPool
            (sizeof(DLStrategyTxNode),PNULL);
        if (dlStrategyTxNode_p)
        {
            tempResourceAllocatorInput_p = &(dlStrategyTxNode_p->resourceAllocInput);
            tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
            tempResourceAllocatorInput_p->strictAllocSubBand  = FALSE;
            dlStrategyTxNode_p->isCQIRequestedULGrantMsg       = FALSE;
            /* +CQI_5.0 */
            tempResourceAllocatorInput_p->subBandPMIInfo_p = PNULL;
            tempResourceAllocatorInput_p->subbandPMI = FALSE;
            tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;        
            /* - CQI_5.0 */
            /* SPR 3430 */
            dlStrategyTxNode_p->allocPushCount = 0;  
            /* SPR 3430 */
            dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;
            dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId ;
            dlStrategyTxNode_p->transmissionType = NEW_TX;
            dlStrategyTxNode_p->strictAllocatedFlag = FALSE;

            dlStrategyTxNode_p->minTBSize = minTBS;
            dlStrategyTxNode_p->maxTBSize = maxTBS; 
            dlStrategyTxNode_p->harqProcess_p = harqProcess_p;


            switch (msgType){
                case TA_MSG:
                {
                    if (ueDLContext_p->drxFlag)
                    {
                        harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                            TA_DRX_MSG;
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBOneInfo.taDrxFlag = TA_MSG;
                    }
                }
                break;
                case DRX_MSG:
                {

                    if (ueDLContext_p->timingAdvanceValue!= INVALID_TA_VALUE)
                    {
                        harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                            TA_DRX_MSG;
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                            DRX_MSG;   
                    }
                }
                break;
                case TA_DRX_MSG:
                {
                    harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                        TA_DRX_MSG;
                }
                break;
                default:
                {
                }
                break;
            }
            /* + CQI_5.0 */
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
            /* - CQI_5.0 */

            /* + CQI_4.1 */
            /*Finding whether Subband Report is available */
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
                /* + CQI_5.0 */
                harqProcess_p->dlHARQTBOneInfo.mcsIndex  =  
                    ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
                harqProcess_p->dlHARQTBOneInfo.tbSize  =  tbSize;
                harqProcess_p->dlHARQTBTwoInfo.tbSize    =  0;

                /* else
                   {
                   harqProcess_p->dlHARQTBTwoInfo.mcsIndex  =  
                   ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
                   harqProcess_p->dlHARQTBOneInfo.tbSize  =  0;
                   harqProcess_p->dlHARQTBTwoInfo.tbSize  =  tbSize;
                   } */  
                /* - CQI_5.0 */

            }
            /* - CQI_4.1 */

            tempResourceAllocatorInput_p->requiredRB = usedRB;
            harqProcess_p->assignedResourcesNB       = usedRB;

            /*Updating the remaining RB available*/
            /* ICIC changes start */
            *availableRB_p  -= usedRB;
            dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated =
                userLocationType;
            /* ICIC changes end */
            dlStrategyTxNode_p->sumLCPriority     = FALSE;
            dlStrategyTxNode_p->weightedQueueLoad = FALSE;
            /* SPR #3098 Changes Starts*/
            /* + CQI_5.0 */
            dlStrategyTxNode_p->msgType = SPATIAL_MUL;
            /*DCI Format Calculation*/
            dlStrategyTxNode_p->primaryDCIFormat = 
                tempStrategyInputInfoForOneTB.primaryDCIFormat;
            dlStrategyTxNode_p->secondaryDCIFormat = 
                tempStrategyInputInfoForOneTB.secondaryDCIFormat;
            /* - CQI_5.0 */
            /* SPR 3403 changes end*/

            /*putEntry of DLStrategyTXNode in PDCCH Queue */
            /* + Coverity 25453, 25452 */
            if ( MAC_FAILURE == putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p, internalCellIndex))
            {
                freeDLHarqProcess(ueDLContext_p,harqProcess_p,internalCellIndex);
                /* SPR 21036 start */
                /* freeMemPool(dlStrategyTxNode_p); */
                /* SPR 21036 end   */ 
                return MAC_FAILURE ;
            }
            /* - Coverity 25453, 25452 */

            LOG_MAC_MSG (DL_NEWTX_ONETBONLYRIONE_UE_SCHEDULED_LOG_ID, LOGDEBUG,
                    MAC_DL_Strategy, getCurrentTick (), ueDLContext_p->ueIndex,
                    dlStrategyTxNode_p->harqProcessId,
                    harqProcess_p->assignedResourcesNB,
                    ueDLContext_p->ueQueueLoad,
                    dlStrategyTxNode_p->msgType,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
        }
        else
        {
            ltePanic("\ndlStrategyTxNode_p getMemFromPool failed");
        }
    }
    else
    {
        /*If the Used RB returns fail, then the Harq Process will be freed*/

        LTE_MAC_UT_LOG(LOG_INFO,DL_RR_STRATEGY,
                "\nThe DL HARQ Process ID ::%d  of UEID %d has been succesfully"
                " freed", harqProcess_p->harqProcessId,ueDLContext_p->ueIndex );

    freeDLHarqProcess(ueDLContext_p,harqProcess_p,internalCellIndex);

        return MAC_FAILURE;
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : processDLSchedulableList 
 * Inputs         : dlUserList - CC or CE user list,
 *                  availableRB_p - pointer to current available RBs,
 *                  allocatedRBFromRoundRobin - The number of RBs allocated as 
 *                                              per RR strategy,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Distribution of RBs among UEs on the basis of Round Robin 
 * Returns        : None 
 * Description    : This function works on Cell Edge and Cell Center user list
 *                  and allocate resources to user by calling function
 *                  processDLRoundRobinNewDataGeneral.
 *****************************************************************************/

void processDLSchedulableList(DLUserListRR *dlUserList,
        UInt32 *availableRB_p,
        UInt32 allocatedRBFromRoundRobin,
        InternalCellIndex internalCellIndex)
{

    SInt16 ueCount                  = dlUserList->count;
    /*coverity-530 CID-32316*/
    MsgType msgType                 = INVALID_MSG;
    /*coverity-530 CID-32316*/
    DLHARQProcess *dlHarqProcess_p  = PNULL;
    DLUEContext *ueDLContext_p      = PNULL;
    UInt16 index                    = 0;

    while(index < ueCount)
    {
        msgType = dlUserList->dlSchedulableUENode[index].msgType;
        ueDLContext_p = dlUserList->dlSchedulableUENode[index].ueDLContext;
        dlHarqProcess_p = dlUserList->dlSchedulableUENode[index].dlHARQProcess;


        if (*availableRB_p == 0)
        {
            while(index < ueCount)
            {
                /* Cyclomatic_complexity_changes_start */
                pushInPendingQueueAsPerMsgType(
                        dlUserList->dlSchedulableUENode[index].msgType,
                        /* SPR 17777 */
                        dlUserList->dlSchedulableUENode[index].ueDLContext,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
                LOG_UT(MAC_RR_RB_NOT_AVAILABLE_FREE_HARQ,LOGDEBUG,MAC_DL_Strategy,
                       getCurrentTick(),
                       index,dlUserList->dlSchedulableUENode[index].ueDLContext->ueIndex,
                       DEFAULT_INT_VALUE,
                       DEFAULT_INT_VALUE,LINE_NUMBER,
                       DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                       FUNCTION_NAME,"");
                /* free harq process for node */
                freeDLHarqProcess(dlUserList->dlSchedulableUENode[index].ueDLContext,
                        dlUserList->dlSchedulableUENode[index].dlHARQProcess,internalCellIndex);
                index++;
            }
            return;
        }
      /* Cyclomatic Complexity changes start */
      allocateResourcesToUEsOnRRBasis(dlUserList,allocatedRBFromRoundRobin,availableRB_p,ueDLContext_p,
                                      msgType,dlHarqProcess_p,internalCellIndex,index);
      /* Cyclomatic Complexity changes end */

        index++;
    }

    return;
}

/*****************************************************************************
 * Function Name  : allocateResourcesToUEsOnRRBasis 
 * Inputs         : allocatedRBFromRoundRobin - The number of RBs allocated as 
 *                                              per RR strategy
 *                  availableRB_p - pointer to current available RBs,
 *                  ueDLContext_p,
 *                  msgType,
 *                  dlHarqProcess_p,
 *                  internalCellIndex,
 *                  index
 * Outputs        : Distribution of RBs among UEs on the basis of Round Robin 
 * Returns        : None 
 * Description    : This function works on Cell Edge and Cell Center user list
 *                  and allocate resources to user by calling function
 *                  processDLRoundRobinNewDataGeneral.
 *****************************************************************************/

STATIC  void allocateResourcesToUEsOnRRBasis(
        DLUserListRR *dlUserList,
        UInt32 allocatedRBFromRoundRobin,
        UInt32 *availableRB_p,
        DLUEContext *ueDLContext_p,
        MsgType msgType,
        DLHARQProcess *dlHarqProcess_p,
        InternalCellIndex internalCellIndex,
        UInt16 index)
{
    if(MAC_FAILURE == processDLRoundRobinNewDataGeneral(
                allocatedRBFromRoundRobin,
                availableRB_p,
                ueDLContext_p,
                msgType,
                dlHarqProcess_p
#ifdef TDD_CONFIG
                /* + SPR 16430 Changes */
                , getCurrentTick() + DL_ADVANCE_TICK
                /* - SPR 16430 Changes */
#endif
                ,internalCellIndex
                ))
    {
        /* Only cause when allocation will fail is when RBs are not enough */
        /* Cyclomatic_complexity_changes_start */
        pushInPendingQueueAsPerMsgType(
                dlUserList->dlSchedulableUENode[index].msgType,
                /* SPR 17777 */
                dlUserList->dlSchedulableUENode[index].ueDLContext,
                internalCellIndex);
        /* Cyclomatic_complexity_changes_end */
        /* Free the HARQ process reserved */ 
        freeDLHarqProcess( 
                dlUserList->dlSchedulableUENode[index].ueDLContext, 
                dlUserList->dlSchedulableUENode[index].dlHARQProcess ,
                internalCellIndex);
    }

}

/* ICIC changes start */
/*****************************************************************************
 * Function Name  : putEntryInDLCellEdgePriorityQueue
 * Inputs         : ueIndex - Index of UE,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Its main function is to insert node in will be to process
 *                  ulCellEdgeUserPriorityQueue_g.
 *****************************************************************************/
 MacRetType putEntryInDLCellEdgePriorityQueue(UInt16 ueIndex,InternalCellIndex internalCellIndex)
{

    CEUserPriorityQueueNode *ceUserPriorityQueueNode_p = PNULL;
    GET_MEM_FROM_POOL(CEUserPriorityQueueNode,ceUserPriorityQueueNode_p,
            sizeof(CEUserPriorityQueueNode),PNULL);
        
	/* + Coverity 24530 */
    if( PNULL == ceUserPriorityQueueNode_p )
    {
	return MAC_FAILURE;
    }
        /* - Coverity 24530 */
   
    ceUserPriorityQueueNode_p->ueIndex       = ueIndex;
    ceUserPriorityQueueNode_p->isNodeInvalid = FALSE;
    dlUECtxInfoArr_g[ueIndex].dlUEContext_p->isAlreadyInCEPriorityList = TRUE;
    if(pushNode(dlCellEdgeUserPriorityQueue_gp[internalCellIndex],
            &(ceUserPriorityQueueNode_p->cellEdgePriorityQueueNodeAnchor)))
    {
        freeMemPool((void *)ceUserPriorityQueueNode_p);
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/* ICIC Change End */

/* cyclomatic changes */
/*****************************************************************************
 * Function Name  : getNextRRUEToSchedule 
 * Inputs         : node_p - pointer to the current node,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : node_p - pointer to the next node
 * Returns        : node_p
 * Description    : It retuns the pointer to the next node.
 ******************************************************************************/
static  LTE_LIST_NODE * getNextRRUEToSchedule
(
  LTE_LIST_NODE *node_p,
  InternalCellIndex internalCellIndex
)
{
     node_p = getNextListNode(node_p);
     if(PNULL == node_p)
     {
           node_p = getListNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex],0);
     }
     return node_p;
}

/*****************************************************************************
 * Function Name : validateUEForRBDistribution
 * Inputs        : ueIndex - Index of UE,
 *                 currentGlobalTTITickCount - Current global tick Count,
 *                 dlReferenceFlag - DL Refernece flag,
 *                 numberOfAvailableRBs_p - No. of avaialable RBs,
 *                 deleteUEFlag - Delete UE Flag,
 *                 internalCellIndex - cell index used at MAC.
 * Outputs       : None
 * Returns       : dlHarqProcess_p
 * Description   : This API will be validating the UE for allocation of RB in 
 *                 DL in Round Robin manner. If UE can be scheduled then it 
 *                 return the free harqprocess for scheduling, if available.
*****************************************************************************/
static  DLHARQProcess * validateUEForRBDistribution(
    UInt16 ueIndex,
    /* SPR 15909 fix start */
    tickType_t currentGlobalTTITickCount,
    /* SPR 15909 fix end */
    /* SPR 21068 start */
    tickType_t dlReferenceFlag,
    /* SPR 21068 end   */ 
    UInt32 *numberOfAvailableRBs_p,
    UInt8  *deleteUEFlag,
    InternalCellIndex internalCellIndex)
{
    DLUEContextInfo   *ueDLContextInfo_p = PNULL;
    DLUEContext       *ueDLContext_p   = PNULL;
    DLHARQProcess     *dlHarqProcess_p = PNULL;

    if (ueIndex >= MAX_UE_SUPPORTED)
    {
          return NULL;
    }
    ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
    if(NULL == ueDLContext_p)
    {
          return NULL;
    }
    if(ueDLContextInfo_p->pendingDeleteFlag)
    {
          /* SPR 15408 Fix Start */
          /* Code Removed*/
          /* SPR 15408 Fix End */
          return NULL;
    }
    if(SPS_SCHEDULING == ueDLContext_p->schType)
    {
          return NULL;
    }
    /*Soft Buffer Changes Start*/
    if((UL_SYNC_LOSS_ON == checkUlSyncLossForUE(ueDLContextInfo_p->dlUEContext_p, currentGlobalTTITickCount, internalCellIndex)) ||
       (ueDLContext_p->permissibleTBSize < HARQ_TB_MINIMUM_SIZE_IN_BYTES))
    /*Soft Buffer Changes End*/
    {
          return NULL;
    }
    #ifdef HD_FDD_CONFIG
    if((TRUE==ueDLContext_p->isHDFddFlag) && 
       (!(CAN_SCHEDULE_DL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY, ueIndex, hdUeContextInfo_g.hdfddUeSchedMap ))))
    {
          /* Increment the statistics counter UE denied DL resources due to
           * UL clash in the same TTI of HD FDD UE*/
          UPDATE_UE_DENIED_DL_RESOURCE(ueIndex);
          return NULL;
    }
    #endif
    if(ueDLContext_p->isAlreadyConsideredFlag == dlReferenceFlag)
    {
          return NULL;
    }
    if(NET_UE_QL(ueDLContext_p) <= 0)
    {
        /* SPR 15408 Fix Start */
        *deleteUEFlag = TRUE;
        /* SPR 15408 Fix End */
          return NULL;
    }
    /* ICIC changes start */
    if ((CE_USER == ueDLContext_p->userLocationType) &&
        ((numberOfAvailableRBs_p[ueDLContext_p->userLocationType] == ZERO) || (!maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex])))
    {
         if (!ueDLContext_p->isAlreadyInCEPriorityList)
         {
             putEntryInDLCellEdgePriorityQueue(ueIndex,internalCellIndex);
         }
         return NULL;
    }
    /* ICIC changes end */

    dlHarqProcess_p = checkForFreeHarqProcess(
                          ueDLContext_p,
                          ueDLContext_p->dlHarqContext_p, 
                          DRA_SCHEDULING,
			  internalCellIndex);
    return dlHarqProcess_p;
}
/*****************************************************************************
 * Function Name : processNonZeroQLoadUEsInRRManner
 * Inputs        : maxUEsToBeScheduledDL_p - Max UE's to be scheduled,
 *                 dlReferenceFlag - DL Refernece flag,
 *                 numberOfAvailableRBs_p - No. of avaialable RBs,
 *                 currentGlobalTTITickCount - Current global Tick Count,
 *                 dlUserList - DL User list,
 *                 internalCellIndex - cell index used at MAC.
 * Outputs       : None
 * Returns       : None
 * Description   : This API will be used in distributing the downlink RB in 
 *                 round robin fashion. The number of RBs distributed to 
 *                 particular UE will be fixed to a maximum value and then 
 *                 depending upon the the available queue load those 
 *                 allocated RBs are validated.
*****************************************************************************/
STATIC  void processNonZeroQLoadUEsInRRManner(
    UInt8 *maxUEsToBeScheduledDL_p,
    /* SPR 21068 start */ 
    tickType_t dlReferenceFlag,
    /* SPR 21068 end   */
    UInt32 *numberOfAvailableRBs_p,
    /* SPR 15909 fix end */
    tickType_t currentGlobalTTITickCount,
    /* SPR 15909 fix end */
    DLUserListRR *dlUserList,
    InternalCellIndex internalCellIndex)
{

    LTE_LIST_NODE     *node_p = PNULL, *newNode_p=NULL;
    UInt16            ueIndex;
    DLHARQProcess     *dlHarqProcess_p = PNULL;
    UInt8             deleteUEFlag = FALSE;
    DLUEContextInfo   *ueDLContextInfo_p = PNULL;
    DLUEContext       *ueDLContext_p   = PNULL;
    UInt8             userLocationType = CC_USER;
	/*SPR 21769 Changes Start*/
    UInt16             ueCount =0;
	/*SPR 21769 Changes Start*/
    /* SPR 14765 Fix Start */
    UInt32 	      iterationCount = 0;
    UInt32            listCnt = 0;
    /* SPR 14765 Fix End */
    
    /*Finding Out the pointer in Non Zero List from where the Round Robin sch
      need to start.It May be start of the list or the Node from where last 
      time Scheduler left scheduling */
    if (dlNonZeroListStartRoundRobin_p[internalCellIndex])
    {

          /*SPR 7470 Fix start */
            node_p = dlNonZeroListStartRoundRobin_p[internalCellIndex];
          /*SPR 7470 Fix end */
    }
    else
    {
        node_p = getListNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex],0);
        dlNonZeroListStartRoundRobin_p[internalCellIndex] = node_p ;
    }

    /* SPR 14765 Fix Start */
    listCnt = listCount((dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]));
    /*Start distributing until the RBs are exhausted */
    while((*maxUEsToBeScheduledDL_p > 0) && (listCnt >= iterationCount))
    {
        iterationCount++;
    /* SPR 14765 Fix End */
        /* 5858 changes start */
        /* + Coverity 24589 */
        if ( !(listCount(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex])) || 
             PNULL == node_p )
        /* - Coverity 24589 */
        {
            node_p = PNULL;
            dlNonZeroListStartRoundRobin_p[internalCellIndex] = PNULL;
            break;

        }
         if(newNode_p == dlNonZeroListStartRoundRobin_p[internalCellIndex] && node_p!=PNULL)
        {
            node_p= newNode_p;
            break;
        }
        /* 5858 changes end */
        ueIndex = ((NonZeroQueueLoadAndHarqNode*)node_p)->ueIndex;
        ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

        /* EICIC +*/
  	    if(!ueDLContextInfo_p->pendingDeleteFlag)
  	  	{
	       if(NULL != ueDLContext_p)
	       {
                if ((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                        (NON_VICTIM_UE == ueDLContext_p->userType))
				{              
		      			 newNode_p = getNextRRUEToSchedule(node_p, internalCellIndex);
		      			 node_p= newNode_p;
	                              deleteUEFlag = FALSE;
			                continue;
				}
                else if ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                             (VICTIM_UE == ueDLContext_p->userType))
				{    
		      			 newNode_p = getNextRRUEToSchedule(node_p, internalCellIndex);
		      			 node_p= newNode_p;
	                              deleteUEFlag = FALSE;
			                continue;
				}
	        }

           /* SPR 22515 SPR fix start */
           /* EICIC -*/

        dlHarqProcess_p = validateUEForRBDistribution(
                                 ueIndex,
                                 currentGlobalTTITickCount,
                                 dlReferenceFlag,
                                 numberOfAvailableRBs_p,
                                 &deleteUEFlag,
                                 internalCellIndex);

        if(PNULL != dlHarqProcess_p)
        {
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
               
            ueDLContext_p->isAlreadyConsideredFlag = dlReferenceFlag;
            userLocationType = ueDLContext_p->userLocationType;
            /* SPR 1831_32 Fix Start */
            /* Update Cell Edge Ue */
            ueCount = dlUserList[userLocationType].count;

            dlUserList[userLocationType].dlSchedulableUENode[ueCount].msgType = INVALID_TA_DRX_MSG;
            dlUserList[userLocationType].dlSchedulableUENode[ueCount].ueDLContext = ueDLContext_p;
            dlUserList[userLocationType].dlSchedulableUENode[ueCount].dlHARQProcess = dlHarqProcess_p;
            dlUserList[userLocationType].count++;

            *maxUEsToBeScheduledDL_p = (*maxUEsToBeScheduledDL_p) - 1;
            if (userLocationType == CE_USER)
            {
                maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;
            }
            /* ICIC Change End */
            /* SPR 1831_32 Fix End */
        }
        if(deleteUEFlag)
        {
            /*  +- SPR 17777 */
            deleteEntryFromUENonZeroQueueLoadAndHarqList(
                    ueDLContextInfo_p->dlUEContext_p, internalCellIndex);
            /*  +- SPR 17777 */
        }
        }
        newNode_p = getNextRRUEToSchedule(node_p, internalCellIndex);
        /* SPR 22515 SPR fix end */
        node_p= newNode_p;
        deleteUEFlag = FALSE;
    }
    dlNonZeroListStartRoundRobin_p[internalCellIndex] = node_p;
}
/* cyclomatic changes */


/* - CL-MIMO-LJA*/
/*****************************************************************************
 * Function Name  : dlRoundRobinScheduler
 * Inputs         : dlReferenceFlag - Used for checking whether the US is 
 *                                    considered already or not. If UE's 
 *                                    isAlreadyConsideredFlag is equal to
 *                                    dlReferenceFlag, that means the UE is
 *                                    already considered,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  dlScheduleData_p - Pointer to DL Scheduler Data Element,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  alreadyScheduledUEs - The number of UEs which are already 
 *                                        Schduled in the Main DL Scheduler
 *                                        Framework for TA/DRX,
 *                  maxUEsToBeScheduledDL - Max DL UE's to be scheduled,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Distribution of RBs among UEs on the basis of Round Robin 
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API will be used in distributing the downlink RB in 
 *                  round robin fashion. The number of RBs distributed to 
 *                  particular UE will be fixed to a maximum value and then 
 *                  depending upon the the available queue load those 
 *                  allocated RBs are validated.
 *****************************************************************************/
UInt32 dlRoundRobinScheduler(
        /*SPR 21068 start */
        tickType_t dlReferenceFlag,
        /*SPR 21068 end   */
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement *dlScheduleData_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* SPR 17777 */
        UInt8  maxUEsToBeScheduledDL,
        InternalCellIndex internalCellIndex
        ) /* RETX_CHG */
{
    DLUEContext* ueDLContext_p   = PNULL;
    UInt32 loopCountTADRX = 0;
    UInt32 tempCountTADRX    = 0;
    DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p 
        =  dlScheduleData_p->dlSchedulerDataNodeArr;
    UInt32 tempTotalActiveUE             = 0;
    /* ICIC changes start */
    UInt32 tempRBCount = 0;
	/*SPR 21769 Changes Start*/
    UInt16 ueCount =0;
	/*SPR 21769 Changes End*/
    DLHARQProcess *dlHarqProcess_p = PNULL;

    /* Stores List of Schedulable CC and CE users */
    DLUserListRR dlUserList[MAX_USER_LOCATION];
    UInt32 maxDLRBToBeAllocInRoundRobin = 0;
    /* Max cell edge region RB than can be allocated */
    UInt32 maxCellEdgeRBToBeAlloc = 0;
    UInt8 userLocationType = CC_USER;
    /* ICIC changes end */
    tempCountTADRX                      = dlScheduleData_p->countTADRX ;
    /* SPR 3608 Changes Start */
    tempTotalActiveUE                   = listCount(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]);
    /* SPR 3608 Changes End */
    /* ICIC changes start */
    UInt32 pendingCEUserCount     = 0;
    /* ICIC changes end */

    UInt32 tempnumberOfAvailableRBs = numberOfAvailableRBs_p[CC_CE_REGION];


    if (!(tempTotalActiveUE + tempCountTADRX))
    {
        return 0;
    }

    LTE_MAC_UT_LOG(LOG_DETAIL,DL_RR_STRATEGY,
            "[DL-Strategy][%s]Total UEs with TA DRX  = %d,"
            " Total UE in Non Zero List = %d \n"
            " Total Active UE  = %d ,"
            "max UE to be scheduled = %d "
            " Number Of Available RBs = %d, Cell Edge Available RBs = %d",
            __func__,tempCountTADRX, listCount(
                dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]),
            tempTotalActiveUE,
            maxUEsToBeScheduledDL,
            numberOfAvailableRBs_p[CC_CE_REGION],
            numberOfAvailableRBs_p[CE_REGION]
            );

    /* resetting unusedRBsDL to 0 */
    unusedRBsDL_g[internalCellIndex] = 0;

    /* resetting user list count to 0 */
    dlUserList[CC_USER].count = 0;
    dlUserList[CE_USER].count = 0;

    /* As RR algorithm will apply on New UE with TA/DRX and New UE without TA/DRX
       Hence Adding count of UE with TADRX to maxUEsToBeScheduledDL */
    LTE_MAC_UT_LOG(LOG_DETAIL,DL_RR_STRATEGY,
            "[DL-Strategy][%s]Total RBs to be distribute per UE :%d "
            "in Round Robin",__func__,maxDLRBToBeAllocInRoundRobin);


    /* ICIC changes end */
    /*Allocating RBs for the UE which also have TA DRX present  in the 
      Scheduler Info Arr, These UEs will be given RBs depending upon 
      the configured max RBs at a single time and then Validated those 
      RBs as per the Queue Load*/
    for ( loopCountTADRX = tempCountTADRX ; loopCountTADRX -- ; )
    {

        /*If there are no more RBs available then return */
        /*If Max UEs to Be scheduled are exhausted*/
        /* SPR_1346_START */
        /* Removed maxUEsToBeScheduledDL ckeck as this check is done in 
         * processTAQueue() */
        /* SPR_1346_END */

        /* ICIC changes start */

        ueDLContext_p = tempDLSchedulerDataNodeArr_p[loopCountTADRX].ueDLContext_p;
        dlHarqProcess_p = &ueDLContext_p->dlHarqContext_p->harqProcess[
            tempDLSchedulerDataNodeArr_p[loopCountTADRX].harqProcessId];

        /* ICIC changes start */

        userLocationType = ueDLContext_p->userLocationType;

        ueCount = dlUserList[userLocationType].count;

        dlUserList[userLocationType].dlSchedulableUENode[
            ueCount].msgType = tempDLSchedulerDataNodeArr_p[loopCountTADRX].msgType;

        dlUserList[userLocationType].dlSchedulableUENode[
            ueCount].dlHARQProcess = dlHarqProcess_p;

        dlUserList[userLocationType].dlSchedulableUENode[
            ueCount].ueDLContext = ueDLContext_p;

        dlUserList[userLocationType].count++;
        /* ICIC changes end */
    }
    if (tempTotalActiveUE < maxUEsToBeScheduledDL)
    {
        LOG_UT(MAC_TOT_ACTIVE_UE_GT_MAX_UE_SCHEDULE,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                tempTotalActiveUE,maxUEsToBeScheduledDL,tempTotalActiveUE,
                maxUEsToBeScheduledDL,LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        maxUEsToBeScheduledDL = tempTotalActiveUE;
    }
    if (tempTotalActiveUE < maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex])
    {
        LOG_UT(MAC_TOT_ACTIVE_UE_GT_MAX_CE_UE,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                tempTotalActiveUE,maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex],tempTotalActiveUE,
                maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex],LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] = tempTotalActiveUE;
     }

    /*Finding Out the pointer in Non Zero List from where the Round Robin sch
      need to start.It May be start of the list or the Node from where last 
      time Scheduler left scheduling */

    /* ICIC changes start */
    if((maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]) && 
            (numberOfAvailableRBs_p[CE_USER]))
    {
        pendingCEUserCount = sQueueCount(dlCellEdgeUserPriorityQueue_gp[internalCellIndex]);
        if (pendingCEUserCount)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_RR_STRATEGY,
                    "[DL-Strategy][%s]: Processing Pending Cell Edge List."
                    " pending Cell Edge ListCount:%d\n",
                    __func__,pendingCEUserCount);

            processDLPriorityCellEdgeUserList(pendingCEUserCount,
                    &dlUserList[CE_REGION],
                    &maxUEsToBeScheduledDL,
                    dlReferenceFlag,
                    /* +- SPR 17777 */
#ifdef HD_FDD_CONFIG
                    currentGlobalTTITickCount,
#endif
                    /* +- SPR 17777 */
                    internalCellIndex
                    );
        }
    }
    /* ICIC changes end */

 
    /*Start distributing until the RBs are exhausted */
    /* ICIC changes start */
    /* Cyclomatic changes */
    processNonZeroQLoadUEsInRRManner(&maxUEsToBeScheduledDL,
            dlReferenceFlag,
            numberOfAvailableRBs_p,
            currentGlobalTTITickCount,
            dlUserList,
            internalCellIndex);
    /* Cyclomatic changes */

    /* SPR 15446 Fix Start */
    dlSchedulableUECount_g = dlUserList[CC_USER].count + dlUserList[CE_USER].count;
    /* SPR 15446 Fix End */

    /* ICIC changes start */
    /* Processing CE users */
    ueCount = dlUserList[CE_USER].count;
    if(ueCount)
    {
#ifdef FLOAT_OPERATIONS
        /* ICIC changes start */
        maxDLRBToBeAllocInRoundRobin = ceilf_wrapper(((UDouble32)
                    (
                     numberOfAvailableRBs_p[CC_CE_REGION]
                    ))/(ueCount + dlUserList[CC_USER].count));
        /* ICIC changes end */
#else
        /* ICIC changes start */
        maxDLRBToBeAllocInRoundRobin = numberOfAvailableRBs_p[CC_CE_REGION] / (ueCount + dlUserList[CC_USER].count);
        /* ICIC changes end */
#endif    
        /* SPR 5052 Fix Start*/
#ifndef FDD_CONFIG
        /* ICIC changes start */
        if (( numberOfAvailableRBs_p[CC_CE_REGION]) % (ueCount + dlUserList[CC_USER].count))
            /* ICIC changes end */
        {
            /* ICIC changes start */
            maxDLRBToBeAllocInRoundRobin++;
            /* ICIC changes end */
        }
#endif
        /* SPR 5052 Fix end*/
        /* ICIC changes start */
        if (maxDLRBToBeAllocInRoundRobin == 0)
        {
            maxDLRBToBeAllocInRoundRobin = 1;
        }

#ifdef FLOAT_OPERATIONS
        maxCellEdgeRBToBeAlloc = ceilf_wrapper(((UDouble32)
                    (
                     numberOfAvailableRBs_p[CE_REGION]
                    ))/ueCount);
#else
        maxCellEdgeRBToBeAlloc = numberOfAvailableRBs_p[CE_REGION]/ueCount ;
#endif    

        if (maxCellEdgeRBToBeAlloc > maxDLRBToBeAllocInRoundRobin)
        {
            LOG_UT(MAC_RR_CE_MAX_RB_GT_CC_MAX_RB,LOGDEBUG,MAC_DL_Strategy,
                    getCurrentTick(),
                    maxCellEdgeRBToBeAlloc,maxDLRBToBeAllocInRoundRobin,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,LINE_NUMBER,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            maxCellEdgeRBToBeAlloc = maxDLRBToBeAllocInRoundRobin;
        }

        if (( numberOfAvailableRBs_p[CE_REGION]) % ueCount )
        {
            maxCellEdgeRBToBeAlloc++;
        }

        if (maxCellEdgeRBToBeAlloc == 0)
        {
            maxCellEdgeRBToBeAlloc = 1;
        }
        tempRBCount = numberOfAvailableRBs_p[CE_USER];
        processDLSchedulableList(&dlUserList[CE_USER],
                &numberOfAvailableRBs_p[CE_USER],
                maxCellEdgeRBToBeAlloc,
                internalCellIndex);

        /* Updating CC_CE_REGION RB count after allocating resources
         * to all of CE users*/
        numberOfAvailableRBs_p[CC_CE_REGION] -= 
            tempRBCount - numberOfAvailableRBs_p[CE_USER];
    }

    /* Processing CC users */
    ueCount = dlUserList[CC_USER].count;

    if(ueCount)
    {
        /*Resseting unused RB to 0 */
        unusedRBsDL_g[internalCellIndex] = 0;
#ifdef FLOAT_OPERATIONS
        maxDLRBToBeAllocInRoundRobin = ceilf_wrapper(((UDouble32)
                    (
                     numberOfAvailableRBs_p[CC_CE_REGION]
                    ))/ueCount);
#else
        maxDLRBToBeAllocInRoundRobin = numberOfAvailableRBs_p[CC_CE_REGION]/ueCount ;
#endif    

        /* SPR 5052 Fix Start*/
#ifndef FDD_CONFIG
        if (( numberOfAvailableRBs_p[CC_CE_REGION]) % ueCount)
        {
            maxDLRBToBeAllocInRoundRobin++;
        }
#endif
        /* SPR 5052 Fix end*/

        if (maxDLRBToBeAllocInRoundRobin == 0)
        {
            maxDLRBToBeAllocInRoundRobin = 1;
        }


        processDLSchedulableList(&dlUserList[CC_USER],
                numberOfAvailableRBs_p,
                maxDLRBToBeAllocInRoundRobin,
                internalCellIndex);

        /* Updating CE Region RB count*/
        if(numberOfAvailableRBs_p[CC_CE_REGION] < 
                numberOfAvailableRBs_p[CE_REGION])
        {
            numberOfAvailableRBs_p[CE_REGION] = 
                numberOfAvailableRBs_p[CC_CE_REGION];

        }
    }
    /* ICIC changes end */

    LOG_MAC_MSG(DL_MAX_UE_SCHEDULED_AND_AVAILABLE_RB_PER_TICK, LOGINFO,
            MAC_DL_Strategy, getCurrentTick (),
            tempTotalActiveUE,
            tempnumberOfAvailableRBs,
            dlUserList[CC_USER].count,0,0,
            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");

    return MAC_SUCCESS;
}
/*********************************************************************************
 * Function Name  : processDLRoundRobinNewDataGeneral
 * Inputs         : allocatedRBFromRoundRobin - The Number of RBs Allocated 
 *                                              Orginally,
 *                  availableRB_p - Pointer to the number of Avavailable RBs,
 *                  ueDLContext_p - Pointer to UE Context,
 *                  msgType - informs about the message type whetehr the TA DRX
 *                            is requested or not,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  currentGlobalTTITickCount - Current Global Tick Count,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Process the UE with New Data
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is for processing the UE for New Transmission
 *                  First pick up a fresh harq process and then
 *                  validate the RBs allocated. After which output is prepared
 *                  and sent to PDCCH Module after checking the Transmission Mode.
 *********************************************************************************/
 MacRetType processDLRoundRobinNewDataGeneral(
        UInt32 allocatedRBFromRoundRobin,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        MsgType msgType,
        DLHARQProcess *harqProcess_p
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
    	, tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#endif
        ,InternalCellIndex internalCellIndex
        )
{
    UInt32 tempTransmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;
    UInt32 numLCWithNonZeroQueueLoad        = 0;
    /* + CL-MIMO LJA*/
    UInt8 tb1only_sendFlag                     = 0;
    /* - CL-MIMO LJA*/
    /* + CQI_5.0 */
    TempStrategyInputInfoForOneTB tempStrategyInputInfoForOneTB = {0};
    UInt8 mcsIndexCW1 = 0;
    UInt8 mcsIndexCW2 = 0;
    UInt8 mcsThreshold = 0;
    /* - CQI_5.0 */
    /*4x4 DL MIMO CHG START*/
    UInt8 mcsThresholdLayer3 = 0;
    UInt8 mcsThresholdLayer4 = 0;
    /*4x4 DL MIMO CHG END*/
    numLCWithNonZeroQueueLoad =
        calculateNumLCWithNonZeroQueueLoad (ueDLContext_p);

    /*SPR 6978 Fix Start*/
    if(ueDLContext_p->srbQloadAvailbl == TRUE)
    {
        /*Setting only TBOne as valid and other INVALID*/
        harqProcess_p->isTBOneValid = TRUE;
        harqProcess_p->isTBTwoValid = FALSE;

        harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;
        harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;
        /*DCI Format Calculation*/

        harqProcess_p->dlHARQTBOneInfo.mcsIndex  =
            ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;

        ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;
        return processDLRoundRobinNewDataWithOneTBOnly(
                allocatedRBFromRoundRobin,
                availableRB_p,ueDLContext_p,harqProcess_p,
                numLCWithNonZeroQueueLoad,
                /* ICIC changes start */
                msgType
                /* ICIC changes end */
#ifdef TDD_CONFIG
                , currentGlobalTTITickCount
#endif
               , internalCellIndex
                );
    }
    else
    {
        /*SPR 6978 Fix End*/
        /*Two TBs can be sent if TX_MODE_3 or TX_MODE_4*/
        if ((TX_MODE_3 == tempTransmissionMode)||
                (TX_MODE_4 == tempTransmissionMode)||
                /* + TM7_8 Changes Start */
                (((TX_MODE_7 == tempTransmissionMode)||
                  (TX_MODE_8 == tempTransmissionMode))&&
                 (1 < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                         ->cellParams_p->numOfTxAnteenas)))
            /* - TM7_8 Changes End */
        {
            /*SPR 3140 CHANGES START */
            /* + SPR 1476 , 1515 */
            /* + CQI_5.0 */
            mcsIndexCW1 = ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
            mcsIndexCW2 = ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
            /* SPR 7188 Changes starts */
		/* SPR 7981 changes start */
            mcsThreshold = 
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->dlSchdConfig.mcsThreshold[ueDLContext_p->userLocationType];
		/* SPR 7981 changes end */
            if( (RANK_INDICATOR_1 == ueDLContext_p->dlMIMOInfo.dlRi)
                    || ( (mcsIndexCW1 <=  mcsThreshold)|| (mcsIndexCW2 <= mcsThreshold)) 
                    /* SPR 7188 Changes ends */
                    || /* + TM7_8 Changes Start */
                    /* if TM8 is configured with RI 2 */
                    ((TX_MODE_8 == tempTransmissionMode) && 
                     (BF_ALGO_NOT_AVAILABLE ==
                      (macReconfigSchedulerParamsDL_gp[internalCellIndex]+
                       schParamsIndexDL_g[internalCellIndex])->bfVectorPresentFlag))) 
                /* - TM7_8 Changes End */
            {
                tb1only_sendFlag = 1;
                POPULATE_INFO_FOR_ONE_TB(tempTransmissionMode,
                        1,
                        tempStrategyInputInfoForOneTB,
                        /* + TM7_8 Changes Start */
                        ueDLContext_p->beamFormingAlgoToBeUsed,
                        internalCellIndex
                        );
                /* - TM7_8 Changes End */

                /* + SPR_6854 */
		/* SPR 7981 changes start */
		if((mcsIndexCW2 <= mcsThreshold) && 
			(mcsIndexCW1 > mcsThreshold))
		{  
    		          ueDLContext_p->dlCQIInfo.cqiCorrectionFactorForCW1 = 
 	                          ueDLContext_p->dlCQIInfo.cqiCorrectionFactorForCW0;
		}
		/* SPR 7981 changes end */
                /* - SPR_6854 */
            }
            /*            
                          else if ( ( (mcsIndexCW1 == 0) && (mcsIndexCW2 > 0)
                          && (mcsIndexCW2 <= mcsThreshold))
                          )

                          {
                          tb1only_sendFlag = 1;
                          POPULATE_INFO_FOR_ONE_TB(tempTransmissionMode,
                          2,
                          tempStrategyInputInfoForOneTB);
                          }
                          */            

            /* + TM7_8 Changes End */ 
            if((6 == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                     cellParams_p->dlAvailableRBs) &&        
#ifdef TDD_CONFIG
                    (CHECK_SF_MAPPED_ON_BCH_PSS_SSS(currentGlobalTTITickCount,
                                                    tempTransmissionMode))
#else             
                (CHECK_SF_MAPPED_ON_BCH_PSS_SSS((getCurrentTick()+PHY_DL_DELAY),
                                                tempTransmissionMode))
#endif                     
               )                                 
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
            mcsThresholdLayer3 = getMCSFromCQI(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer3[ueDLContext_p->userLocationType]
              );
            mcsThresholdLayer4 = getMCSFromCQI(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer4[ueDLContext_p->userLocationType]
              );


            if ((RANK_INDICATOR_4 == ueDLContext_p->dlMIMOInfo.dlRi || RANK_INDICATOR_3 == ueDLContext_p->dlMIMOInfo.dlRi) &&
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
            }/*4X4 DL MIMO CHG END*/ 
            if(tb1only_sendFlag)
            {
                /*If ( (reported CQI < CQI threshold))
                 *     OR (reported RI is 1)
                 *     )
                 * Then transmit 1 TB only
                 */
                ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;
                return processDLRoundRobinNewDataWithOneTBOnlyRIOne(
                        allocatedRBFromRoundRobin,
                        availableRB_p,ueDLContext_p,harqProcess_p,
                        numLCWithNonZeroQueueLoad,
                        /* ICIC changes start */
                        msgType
                        /* ICIC changes end */
#ifdef TDD_CONFIG
                        , currentGlobalTTITickCount
#endif
                        ,tempStrategyInputInfoForOneTB  
                        ,internalCellIndex 
                        );
            }

            /* - CQI_5.0 */
            else
            {
                /* - CL-MIMO LJA*/
                ueDLContext_p->maxTBSize = ueDLContext_p->maxMIMOTBSize;
                return processDLRoundRobinNewDataWithTwoTB(
                        allocatedRBFromRoundRobin,
                        availableRB_p,ueDLContext_p,harqProcess_p,
                        numLCWithNonZeroQueueLoad,
                        /* ICIC changes start */
                        msgType
                        /* ICIC changes end */

#ifdef TDD_CONFIG
                        , currentGlobalTTITickCount
#endif
                        ,internalCellIndex
                        );
                /* + CL-MIMO LJA*/
            }
            /* - CL-MIMO LJA*/
        }
        /* + TM6_5.2 */
        else if(TX_MODE_6 == tempTransmissionMode)
        {
            POPULATE_INFO_FOR_ONE_TB(tempTransmissionMode,
                    1,
                    tempStrategyInputInfoForOneTB,
                    /* + TM7_8 Changes Start */
                    BF_ALGO_NOT_AVAILABLE,
                    internalCellIndex);
            /* - TM7_8 Changes End */

            tempStrategyInputInfoForOneTB.primaryDCIFormat = ueDLContext_p->tm6DCIFormat;
            tempStrategyInputInfoForOneTB.secondaryDCIFormat = DCI_FORMAT_1A;

            ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;
            return processDLRoundRobinNewDataWithOneTBOnlyRIOne(
                    allocatedRBFromRoundRobin,
                    availableRB_p,ueDLContext_p,harqProcess_p,
                    numLCWithNonZeroQueueLoad,
                    /* ICIC changes start */
                    msgType
                    /* ICIC changes end */
#ifdef TDD_CONFIG
                    , currentGlobalTTITickCount
#endif
                    ,tempStrategyInputInfoForOneTB  
                    ,internalCellIndex
                    );

        }
        /* - TM6_5.2 */

        /*Only One TB can be sent if TX_MODE_OTHER than 3 or 4*/
        else
        {
            ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;
            return processDLRoundRobinNewDataWithOneTBOnly(
                    allocatedRBFromRoundRobin,
                    availableRB_p,ueDLContext_p,harqProcess_p,
                    numLCWithNonZeroQueueLoad,
                    /* ICIC changes start */
                    msgType
                    /* ICIC changes end */
#ifdef TDD_CONFIG
                    , currentGlobalTTITickCount
#endif
                    ,internalCellIndex
                    );
        }
        /*SPR 6978 Fix start*/
    }
    /*SPR 6978 Fix End*/
}
/**************************************************************************************
 * Function Name  : processDLRoundRobinNewDataWithOneTBOnly
 * Inputs         : allocatedRBFromRoundRobin - The Number of RBs Allocated Orginally,
 *                  availableRB_p - Pointer to the number of Avavailable RBs,
 *                  ueDLContext_p - Pointer to UE Context,
 *                  harqProcess_p -  Pointer to DL Harq UE Context,
 *                  numLCWithNonZeroQueueLoad - Number of LCs with non Zero Queue
 *                                              load,
 *                  msgType - informs about the message type whetehr the TA DRX
 *                            is requested or not,
 *                  currentGlobalTTITickCount - Current Global Tick Count,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Process the UE for One TB with New Data
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is for processing the UE for New Transmission 
 *                  For one TB only.First pick up a fresh harq process and then 
 *                  validate the RBs allocated. After which output is prepared 
 *                  and sent to PDCCH Module.
 *****************************************************************************/
MacRetType processDLRoundRobinNewDataWithOneTBOnly(
        UInt32 allocatedRBFromRoundRobin,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLHARQProcess * harqProcess_p,
        UInt32 numLCWithNonZeroQueueLoad,
        MsgType msgType
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        , tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#endif
        ,InternalCellIndex internalCellIndex
        )
{
    UInt32 usedRB = 0;
    UInt32 tbSize = 0;    
    UInt32 allocatedRB = 0;
    ResourceAllocatorInput * tempResourceAllocatorInput_p = PNULL;
    DLStrategyTxNode * dlStrategyTxNode_p=PNULL;

    UInt32 minTBS = 0;
    UInt32 maxTBS = 0;

    LTE_MAC_UT_LOG(LOG_INFO,DL_RR_STRATEGY,
    "Entry in [DL-Strategy][%s]for UEID %d\n",__func__,ueDLContext_p->ueIndex);

#ifdef TDD_CONFIG
    UInt8 subFrame = currentGlobalTTITickCount % MAX_SUBFRAME;
#endif

    /* ICIC changes start */
    UInt8 userLocationType = ueDLContext_p->userLocationType;
    allocatedRB = allocatedRBFromRoundRobin + unusedRBsDL_g[internalCellIndex];
    LOG_UT(MAC_RR_UNUSED_RB_ADDED_TO_ALLOCATED_RB,LOGDEBUG,MAC_DL_Strategy,
           getCurrentTick(),
           unusedRBsDL_g[internalCellIndex],allocatedRBFromRoundRobin,
           DEFAULT_INT_VALUE,
           DEFAULT_INT_VALUE,LINE_NUMBER,
           DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
           FUNCTION_NAME,"");
    /* ICIC changes end */

    /*Validating the allocated RBS against the Queue Load */
    usedRB = validateRBForOneTB(ueDLContext_p,
    /* ICIC changes start */
    allocatedRB,
    /* ICIC changes end */
    /* SPR 17777 */
            numLCWithNonZeroQueueLoad,&tbSize,*availableRB_p,msgType,&minTBS,&maxTBS  
#ifdef TDD_CONFIG
			, subFrame
            ,internalCellIndex
#endif
            /* SPR 17777 */
	        );


    /* ICIC changes start */
    if(allocatedRB > usedRB)
    {
        LOG_UT(MAC_RR_SUBTRACT_USED_RB_FROM_ALLOCATED_RB,LOGDEBUG,MAC_DL_Strategy,
               getCurrentTick(),
               usedRB,allocatedRB,
               unusedRBsDL_g[internalCellIndex],
               (allocatedRB - usedRB),LINE_NUMBER,
               DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
               FUNCTION_NAME,"");
        unusedRBsDL_g[internalCellIndex] = allocatedRB - usedRB;
    }
    else
    {
        LOG_UT(MAC_RR_MAKING_UNUSED_RB_ZERO,LOGDEBUG,MAC_DL_Strategy,
               getCurrentTick(),
               usedRB,allocatedRB,
               DEFAULT_INT_VALUE,
               DEFAULT_INT_VALUE,LINE_NUMBER,
               DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
               FUNCTION_NAME,"");
        unusedRBsDL_g[internalCellIndex] = 0;
    }
    /* ICIC changes end */


    if (usedRB)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,DL_RR_STRATEGY,
                "[DL-Strategy][%s][TB_ONE:NEW DATA],[TB_TWO:INVALID]\n"
                "validateRBForOneTB returned %d RBs for UEID %d\n",__func__,
                usedRB,ueDLContext_p->ueIndex);

        /*Setting only TBOne as valid and other INVALID*/
        harqProcess_p->isTBOneValid = TRUE;
        harqProcess_p->isTBTwoValid = FALSE;

        /*Preparing the DL Strategy Output Node*/
        GET_MEM_FROM_POOL(DLStrategyTxNode,dlStrategyTxNode_p,sizeof(DLStrategyTxNode),PNULL);
	
	/* + Coverity 24526 */
	if( PNULL == dlStrategyTxNode_p )
	{
	  return MAC_FAILURE ;
	}
	/* - Coverity 24526 */
          
	    tempResourceAllocatorInput_p = 
                &(dlStrategyTxNode_p->resourceAllocInput);
            tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
            tempResourceAllocatorInput_p->strictAllocSubBand  = FALSE;
            dlStrategyTxNode_p->isCQIRequestedULGrantMsg       = FALSE;
/* SPR 3430 */
            dlStrategyTxNode_p->allocPushCount = 0;  
/* SPR 3430 */

            dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;
            dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId ;
//SPR FIX 8538 START
            dlStrategyTxNode_p->harqProcess_p = harqProcess_p; 
            dlStrategyTxNode_p->transmissionType = NEW_TX;
            dlStrategyTxNode_p->strictAllocatedFlag = FALSE;
//SPR FIX 8538 END

            dlStrategyTxNode_p->minTBSize = minTBS;
            dlStrategyTxNode_p->maxTBSize = maxTBS; 

            /*Setting the message Type accoding to the input and change the same
              according to the Transmission . Setting the TA DRX Fields as well   */
            switch (msgType){
                case TA_MSG:
                    {
                        if (ueDLContext_p->drxFlag)
                        {
                            dlStrategyTxNode_p->msgType = 
                                TA_DRX_WITH_DATA_NON_SPATIAL_MUL;
                            harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                                TA_DRX_MSG;
                        }
                        else
                        {
                            dlStrategyTxNode_p->msgType =
                                TA_WITH_DATA_NON_SPATIAL_MUL;
                            harqProcess_p->dlHARQTBOneInfo.taDrxFlag = TA_MSG;
                        }
                    }
                    break;
                case DRX_MSG:
                    {
                       
                        if (ueDLContext_p->timingAdvanceValue!= INVALID_TA_VALUE)
                        {
                            dlStrategyTxNode_p->msgType = 
                                TA_DRX_WITH_DATA_NON_SPATIAL_MUL ;
                            harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                                TA_DRX_MSG;
                        }
                        else
                        {
                            dlStrategyTxNode_p->msgType = 
                                DRX_WITH_DATA_NON_SPATIAL_MUL ;
                            harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                                DRX_MSG;   
                        }
                    }
                    break;
                case TA_DRX_MSG:
                    {
                        dlStrategyTxNode_p->msgType = 
                            TA_DRX_WITH_DATA_NON_SPATIAL_MUL;
                        harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                                TA_DRX_MSG;
                    }
                    break;
                default:
                    {
                        dlStrategyTxNode_p->msgType = 
                            NON_SPATIAL_MUL ;
                    }
            }

            harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;

            /*DCI Format Calculation*/
            /*SPR 6978 Fix Start*/
            if(FALSE == ueDLContext_p->srbQloadAvailbl)
            {
                dciFormatSelector (ueDLContext_p,dlStrategyTxNode_p,
                        HARQ_FREE_TX_OTHER_THAN_3_4,
                        internalCellIndex);
            }
            else
            {
                ueDLContext_p->srbQloadAvailbl = FALSE;
                dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_1A;
                dlStrategyTxNode_p->secondaryDCIFormat = DCI_FORMAT_1A;
            }
            /*SPR 6978 Fix End*/

            /* + CQI_4.1 */
            /*Finding whether Subband Report is available */
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

            }
            /* - CQI_4.1 */

            tempResourceAllocatorInput_p->requiredRB = usedRB;
            harqProcess_p->assignedResourcesNB       = usedRB;

            /*Updating the remaining RB available*/
            /* ICIC changes start */
            *availableRB_p  -= usedRB;
            dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated =
                userLocationType;
            /* ICIC changes end */

            dlStrategyTxNode_p->sumLCPriority     = FALSE;
            dlStrategyTxNode_p->weightedQueueLoad = FALSE;

            /*putEntry of DLStrategyTXNode in PDCCH Queue */
	    /* coverity fix 10076 + */
            if ( MAC_FAILURE == putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex))
            {
                freeDLHarqProcess(ueDLContext_p,harqProcess_p,internalCellIndex);
                /* SPR 21036 start */
                /* freeMemPool(dlStrategyTxNode_p);*/
                /* SPR 21036 end   */ 
                return MAC_FAILURE ;
            }
	    /* coverity fix 10076 - */
            LOG_MAC_MSG (DL_NEWTX_ONETBONLY_UE_SCHEDULED_LOG_ID, LOGDEBUG,
                   MAC_DL_Strategy, getCurrentTick (), ueDLContext_p->ueIndex,
                   dlStrategyTxNode_p->harqProcessId,
                   harqProcess_p->assignedResourcesNB,
                   ueDLContext_p->ueQueueLoad,
                   dlStrategyTxNode_p->msgType,
                   DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
    }
    else
    {
        /*If the Used RB returns fail, then the Harq Process will be freed*/

        LTE_MAC_UT_LOG(LOG_INFO,DL_RR_STRATEGY,
                "Exit[DL-Strategy][%s]validateRBForOneTB returned Zero RBs for UEID %d\n"
                "Hence NOT Scheduling this UE",__func__,ueDLContext_p->ueIndex);
        LTE_MAC_UT_LOG(LOG_INFO,DL_RR_STRATEGY,
                "\nThe DL HARQ Process ID ::%d  of UEID %d has been succesfully"
                " freed", harqProcess_p->harqProcessId,ueDLContext_p->ueIndex );

        freeDLHarqProcess(ueDLContext_p, harqProcess_p, internalCellIndex);
        
        return MAC_FAILURE;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RR_STRATEGY,
            "Exit[DL-Strategy][%s]for UEID %d\n",
                __func__,ueDLContext_p->ueIndex);
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : processDLRoundRobinNewDataWithTwoTB
 * Inputs         : allocatedRBFromRoundRobin - The Number of RBs Allocated 
 *                                              Orginally,
 *                  availableRB_p - Pointer to the number of Avavailable RBs,
 *                  ueDLContext_p - Pointer to UE Context,
 *                  harqProcess_p - Pointer to DL Harq UE Context,
 *                  numLCWithNonZeroQueueLoad - Number of LCs with non Zero Queue 
 *                                              load,
 *                  msgType - informs about the message type whetehr the TA DRX 
 *                            is requested or not,
 *                  currentGlobalTTITickCount - Current Global Tick Count,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Process the UE for Two TB with New Data
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is for processing the UE for New Transmission 
 *                   For TWO TBs .First pick up a fresh harq process and then 
 *                  validate the RBs allocated. After which output is prepared  
 *                  and sent to PDCCH Module.Set the message type Accordingly
 *****************************************************************************/
MacRetType processDLRoundRobinNewDataWithTwoTB(
        UInt32 allocatedRBFromRoundRobin,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLHARQProcess * harqProcess_p,
        UInt32 numLCWithNonZeroQueueLoad,
        MsgType msgType
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        , tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#endif
        ,InternalCellIndex internalCellIndex
        )        
{
    UInt32 usedRB       = 0;
    UInt32 tbOneSize    = 0;
    UInt32 tbTwoSize    = 0;
    ResourceAllocatorInput *tempResourceAllocatorInput_p    = PNULL;
    DLStrategyTxNode * dlStrategyTxNode_p                   = PNULL;

    UInt32 minTBS = 0;
    UInt32 maxTBS = 0;
    UInt32 allocatedRB = 0;
    /* + CQI_5.0 */
    UInt8 transmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;
    /* - CQI_5.0 */
#ifdef TDD_CONFIG
    UInt8 subFrame = currentGlobalTTITickCount % MAX_SUBFRAME;
#endif
    
    /* ICIC changes start */
    UInt8 userLocationType = ueDLContext_p->userLocationType;
    /* ICIC changes end */
    /* 4x4 DL MIMO CHG START */
    UInt8 tbMappedToLayer = 0;
    /* 4x4 DL MIMO CHG END */

    allocatedRB = allocatedRBFromRoundRobin + unusedRBsDL_g[internalCellIndex];
    LOG_UT(MAC_RR_UNUSED_RB_ADDED_TO_ALLOCATED_RB,LOGDEBUG,MAC_DL_Strategy,
           getCurrentTick(),
           unusedRBsDL_g[internalCellIndex],allocatedRBFromRoundRobin,
           DEFAULT_INT_VALUE,
           DEFAULT_INT_VALUE,LINE_NUMBER,
           DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
           FUNCTION_NAME,"");
    /* 4x4 DL MIMO CHG START */
    if (harqProcess_p->numOfLayer == 4)
        tbMappedToLayer = TB_MAPPED_TO_TWO_LAYER;
    else
        tbMappedToLayer = TB_MAPPED_TO_ONE_LAYER;
    /* 4x4 DL MIMO CHG END */

    if ((SInt64)(tbOneSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                               /* ICIC changes start */
                              allocatedRB, 
                               /* ICIC changes end */
                               /*4x4 DL MIMO CHG START*/
                               tbMappedToLayer
                               /*4x4 DL MIMO CHG END*/
                               )) < NET_UE_QL(ueDLContext_p))
    {
        /*Validating the allocated RBS against the Queue Load considering TWO TBs*/
        usedRB = validateRBForTwoTB(ueDLContext_p,
                /* ICIC changes start */
                allocatedRB,
                /* ICIC changes end */
                numLCWithNonZeroQueueLoad,&tbOneSize,&tbTwoSize, *availableRB_p,
                msgType,&minTBS,&maxTBS,
#ifdef TDD_CONFIG
                subFrame,
                /* SPR 17777 */
                internalCellIndex,
#endif
                /*4x4 DL MIMO CHG START*/
                harqProcess_p
                /*4x4 DL MIMO CHG END*/
                /* SPR 17777 */
                );


    }
    else
    {
        tbTwoSize = 0;
        usedRB = validateRBForOneTB(ueDLContext_p, 
                /* ICIC changes start */
                allocatedRB,
                /* ICIC changes end */
                numLCWithNonZeroQueueLoad, &tbOneSize,
                /* SPR 17777 */
                *availableRB_p, msgType,&minTBS,&maxTBS
#ifdef TDD_CONFIG
                , subFrame
                /* SPR 17777 */
                ,internalCellIndex
#endif
                /* SPR 17777 */
                );

    }
        /* ICIC changes start */
    if(allocatedRB > usedRB)
    {
        LOG_UT(MAC_RR_SUBTRACT_USED_RB_FROM_ALLOCATED_RB,LOGDEBUG,MAC_DL_Strategy,
               getCurrentTick(),
               usedRB,allocatedRB,
               unusedRBsDL_g[internalCellIndex],
               (allocatedRB - usedRB),LINE_NUMBER,
               DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
               FUNCTION_NAME,"");
        unusedRBsDL_g[internalCellIndex] = allocatedRB - usedRB;
    }
    else
    {
        LOG_UT(MAC_RR_MAKING_UNUSED_RB_ZERO,LOGDEBUG,MAC_DL_Strategy,
               getCurrentTick(),
               usedRB,allocatedRB,
               DEFAULT_INT_VALUE,
               DEFAULT_INT_VALUE,LINE_NUMBER,
               DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
               FUNCTION_NAME,"");
        unusedRBsDL_g[internalCellIndex] = 0;
    }
    /* ICIC changes end */

        
    if (usedRB)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,DL_RR_STRATEGY,
                "[DL-Strategy][%s][TB_ONE:NEW DATA],[TB_TWO:NEW DATA]\n"
                 "validateRBForOneTB returned %d RBs for UEID %d\n",__func__,
                usedRB,ueDLContext_p->ueIndex);

        /*Preparing the DL Strategy Output Node*/
        GET_MEM_FROM_POOL(DLStrategyTxNode,dlStrategyTxNode_p,sizeof(DLStrategyTxNode),PNULL);
        
        /* + Coverity 24527 */ 
        if( PNULL == dlStrategyTxNode_p )
	{
           return MAC_FAILURE;
	} 
	/* - Coverity 24527 */ 
	
	/* Setting  both TBOne and TBTwo as valid */
        harqProcess_p->isTBOneValid = TRUE;
        harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;
        /* SPR 3430 */
        dlStrategyTxNode_p->allocPushCount = 0;  
        /* SPR 3430 */
        if (tbTwoSize)
        {
            harqProcess_p->isTBTwoValid = TRUE;
            harqProcess_p->dlHARQTBTwoInfo.txIndicator = NEW_TX;
            /*DCI Format Calculation*/
            /* + TM7_8 Changes Start */
            dciFormatSelector (ueDLContext_p,dlStrategyTxNode_p,
                    HARQ_FREE_TX_3_4_8,
                    internalCellIndex);
            /* - TM7_8 Changes End */
        }
        else
        {
            harqProcess_p->isTBTwoValid = FALSE;
            harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;
            /* + TM7_8 Changes Start */
            if(TX_MODE_8 == transmissionMode)
            {
                dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_2B;
                dlStrategyTxNode_p->secondaryDCIFormat = DCI_FORMAT_1A;
            }
            else
            {
            /* - TM7_8 Changes End */
                /* + CQI_5.0 */
                /** klockwork Changes Start **/
                if(transmissionMode < INVALID_TX_MODE)
                {
                    dlStrategyTxNode_p->secondaryDCIFormat = 
                       macParams_g.preferredSecondaryDCIFormatForSingleTB[transmissionMode];
                    dlStrategyTxNode_p->primaryDCIFormat = 
                        macParams_g.preferredPrimaryDCIFormatForSingleTB[transmissionMode];
                }
                /** klockwork Changes End **/
            }
        }

        dlStrategyTxNode_p->msgType = SPATIAL_MUL;
        /* - CQI_5.0 */

        tempResourceAllocatorInput_p = &(dlStrategyTxNode_p->resourceAllocInput);
        tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
        tempResourceAllocatorInput_p->strictAllocSubBand  = FALSE;
        dlStrategyTxNode_p->isCQIRequestedULGrantMsg      = FALSE;
        /* + CQI_5.0 */
        tempResourceAllocatorInput_p->subBandPMIInfo_p = PNULL;
        tempResourceAllocatorInput_p->subbandPMI = FALSE;
        tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;       
        /* - CQI_5.0 */

        dlStrategyTxNode_p->ueIndex          = ueDLContext_p->ueIndex;
        dlStrategyTxNode_p->harqProcessId    = harqProcess_p->harqProcessId;
        dlStrategyTxNode_p->transmissionType = NEW_TX;
        dlStrategyTxNode_p->strictAllocatedFlag = FALSE;

        dlStrategyTxNode_p->minTBSize = minTBS;
        dlStrategyTxNode_p->maxTBSize = maxTBS;
//SPR FIX 8538 
        dlStrategyTxNode_p->harqProcess_p    = harqProcess_p; 

        /*Setting the message Type accoding to the input and change the same
          according to the Transmission . Setting the TA DRX Fields as well   
          If both TB are having new data then the TA and DRX will always be 
          sent in First TB, hence updating the Harq Fields as well*/
        switch (msgType){
            case TA_MSG:
                {
                    if (ueDLContext_p->drxFlag)
                    {
                        harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                            TA_DRX_MSG;
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBOneInfo.taDrxFlag = TA_MSG;
                    }
                }
                break;
            case DRX_MSG:
                {
                    if (ueDLContext_p->timingAdvanceValue!= INVALID_TA_VALUE)
                    {
                        harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                            TA_DRX_MSG;
                    }
                    else
                    {
                        harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                            DRX_MSG;   
                    }
                }
                break;
            case TA_DRX_MSG:
                {
                    harqProcess_p->dlHARQTBOneInfo.taDrxFlag = 
                        TA_DRX_MSG;
                }
                break;
            default:
                {
                    break;
                }
        }   

        /* + CQI_5.0 */
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
        /* - CQI_5.0 */
        /* + CQI_4.1 */
        /*Finding whether Subband Report is available */
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
        /* - CQI_4.1 */

        tempResourceAllocatorInput_p->requiredRB = usedRB;
        harqProcess_p->assignedResourcesNB          = usedRB;

            /* ICIC changes start */
            *availableRB_p -= usedRB;
            dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated =
                userLocationType;
            /* ICIC changes end */

        dlStrategyTxNode_p->sumLCPriority         = FALSE;
        dlStrategyTxNode_p->weightedQueueLoad     = FALSE;

	/*putEntry of DLStrategyTXNode in PDCCH Queue */
	/* coverity fix 10030 + */
	if ( MAC_FAILURE == putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex))
	{
		freeDLHarqProcess(ueDLContext_p,harqProcess_p,internalCellIndex);
		/* SPR 21036 start */ 
                /* freeMemPool(dlStrategyTxNode_p); */
                /* SPR 21036 end   */ 
		return MAC_FAILURE ;
	}
	/* coverity fix 10030 - */
        LOG_MAC_MSG (DL_NEWTX_TWOTBONLY_UE_SCHEDULED_LOG_ID, LOGDEBUG,
                    MAC_DL_Strategy, getCurrentTick (), ueDLContext_p->ueIndex,
                    dlStrategyTxNode_p->harqProcessId,
                    harqProcess_p->assignedResourcesNB,
                    ueDLContext_p->ueQueueLoad,
                    dlStrategyTxNode_p->msgType,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
    }
    else
    {
        /*If the Used RB returns fail, then the Harq Process will be freed*/
        LTE_MAC_UT_LOG(LOG_INFO,DL_RR_STRATEGY,
            "Exit[DL-Strategy][%s]validateRBForOneTB returned Zero "
            "RBs for UEID %d\n Hence NOT Scheduling this UE",__func__,
            ueDLContext_p->ueIndex);
        
        LTE_MAC_UT_LOG(LOG_INFO,DL_RR_STRATEGY,
                "\nThe DL HARQ Process ID ::%d  of UEID %d has been succesfully"
                " freed", harqProcess_p->harqProcessId,ueDLContext_p->ueIndex );

        freeDLHarqProcess(ueDLContext_p,harqProcess_p, internalCellIndex);
        
        return MAC_FAILURE;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_RR_STRATEGY,
            "Exit:[DL-Strategy][%s]",__func__);
    return MAC_SUCCESS;
}
/* ICIC changes start */
/*****************************************************************************
 * Function Name  : processDLPriorityCellEdgeUserList
 * Inputs         : pendingCEUserCount - This is Count of the total cell edge
 *                                       nodes which are not processed in
 *                                       earlier ticks,
 *                  dlUserList_p - Pointer to DLUserListRR,
 *                  maxUEsToBeScheduledDL_p - Max DL UE's to be scheduled,
 *                  dlReferenceFlag - Whether UE is already considered or not
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : It will allocate RBs to pending cell edge user list
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Its main function will be to process the priority list
 *                  which conatains pending Cell Edge UE. Try to allocate RBs 
 *                  to them, if failed to do so, push it again in list. 
 *****************************************************************************/
MacRetType processDLPriorityCellEdgeUserList(
         UInt32 pendingCEUserCount,
         DLUserListRR *dlUserList_p,
         UInt8 *maxUEsToBeScheduledDL_p,
         /*SPR 21068 start */
         tickType_t dlReferenceFlag,
         /*SPR 21068 end   */
         /* SPR 15909 fix start */
         /* +- SPR 17777 */
#ifdef HD_FDD_CONFIG
         tickType_t currentGlobalTTITickCount,
#endif
         /* +- SPR 17777 */
         /* SPR 15909 fix end */
         InternalCellIndex internalCellIndex)
{

    UInt8 firstCellEdgeNode       = TRUE;
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    DLUEContext* ueDLContext_p          = PNULL;
    UInt16 ueIndex = 0;
    /* Cell User Priority Q node */
    CEUserPriorityQueueNode *ceUserPriorityQueueNode_p = PNULL;
    CEUserPriorityQueueNode *ceUserPriorityQueueNodeTmp_p = PNULL;
    MeasGapContextDL *measgapContext_p = PNULL;
    UInt32 ueCount = 0;
    /* UInt8 userLocationType = CC_USER;TDD Warning Fix */
    DLHARQProcess *dlHarqProcess_p = PNULL;
    DRXContext    *drxContext_p    = PNULL;
    ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *)getFirstNode(dlCellEdgeUserPriorityQueue_gp[internalCellIndex]);
    while(ceUserPriorityQueueNode_p && pendingCEUserCount-- & maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] && 
            (*maxUEsToBeScheduledDL_p) )
    { 
        ueIndex           = ceUserPriorityQueueNode_p->ueIndex;
        ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        ueDLContext_p     = ueDLContextInfo_p->dlUEContext_p;
        if (ceUserPriorityQueueNode_p->isNodeInvalid)
        {
            if(firstCellEdgeNode)
            {
                ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) popNode (dlCellEdgeUserPriorityQueue_gp[internalCellIndex]);
                /* +COVERITY 11432 */
                if (NULL == ceUserPriorityQueueNode_p)
                {
                    break;
                }
                /* -COVERITY 11432 */

                ceUserPriorityQueueNodeTmp_p = ceUserPriorityQueueNode_p;
                ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *)
                    getNextNode ((const LTE_SNODE *) (ceUserPriorityQueueNodeTmp_p));

                freeMemPool (ceUserPriorityQueueNodeTmp_p);
                continue;
            }
            else
            {
                ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                    ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
                continue;
            }
        } 

        if (!(ueDLContextInfo_p->pendingDeleteFlag))
        {
                 /* EICIC +*/
/*Complexity Start*/
		    if (((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                  (NON_VICTIM_UE == ueDLContext_p->userType))||
                   ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && 
                   (VICTIM_UE == ueDLContext_p->userType)))
		    {              
                    firstCellEdgeNode = FALSE;
                    ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                        ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
                    continue;
		    }

/*Complexity End*/		
		/* EICIC -*/
            /* \code
             * if schType == SPS_SCHEDULING, then do not schedule
             * this UE as it is already considered by SPS.
             * */
            if( SPS_SCHEDULING == ueDLContext_p->schType )
            {
                firstCellEdgeNode = FALSE;
                ceUserPriorityQueueNode_p = 
                    (CEUserPriorityQueueNode *) getNextNode(
                            (const LTE_SNODE *) (ceUserPriorityQueueNode_p));
                continue;
            }
            measgapContext_p = &(ueDLContext_p->measGapContext);
            /* userLocationType = ueDLContext_p->userLocationType;TDD warning Fix */
            drxContext_p     = &ueDLContext_p->drxContext;

            if ((DRX_STATE_ON != CHECK_UE_DRX_STATE(drxContext_p)) &&
                    (TRUE != CHECK_UE_MEAS_GAP_PERIOD(measgapContext_p))
            /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
                    &&
                    ((FALSE == ueDLContext_p->isHDFddFlag) || 
                     ((TRUE == ueDLContext_p->isHDFddFlag) && 
                      (CAN_SCHEDULE_DL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY,ueIndex,hdUeContextInfo_g.hdfddUeSchedMap))))
#endif
            /*HD FDD Changes End*/
                    )
            {

                if (ueDLContext_p->isAlreadyConsideredFlag == dlReferenceFlag)
                {
                    firstCellEdgeNode = FALSE;
                    ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                        ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
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
                    /*SPR 21034 start */
                    ueDLContext_p->isAlreadyInCEPriorityList = FALSE;
                    /*SPR 21034 end   */
                    continue;
                }

                /* allocating harq process */
                dlHarqProcess_p = 
                    checkForFreeHarqProcess(ueDLContext_p,
                        ueDLContext_p->dlHarqContext_p, DRA_SCHEDULING ,internalCellIndex);

                /* If harq process is free then UE can be schedule */
                if (dlHarqProcess_p)
                {
                    ueDLContext_p->isAlreadyConsideredFlag = dlReferenceFlag;
                    (*maxUEsToBeScheduledDL_p)--;

                    /* Update Cell Edge Ue */
                    maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;

                    /* Updating CE user list */

                    ueCount = dlUserList_p->count;
                    dlUserList_p->dlSchedulableUENode[ueCount].msgType 
                        = INVALID_TA_DRX_MSG;
                    dlUserList_p->dlSchedulableUENode[ueCount].ueDLContext
                        = ueDLContext_p;

                    dlUserList_p->dlSchedulableUENode[ ueCount].dlHARQProcess 
                        = dlHarqProcess_p;
                    dlUserList_p->count++;

                }
                else
                {
                    firstCellEdgeNode = FALSE;
                    ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                        ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
                    /*SPR 21034 start */
                    ueDLContext_p->isAlreadyInCEPriorityList = FALSE;
                    /*SPR 21034 end   */
                    continue;
                }
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_WARNING,DL_RR_STRATEGY,
                        "[DL-Strategy][%s]:UE ID %d is"
                        " already considered, hence NOT considering UE ",
                        __func__,ueIndex);
            }
            /* + Bug 20794 */
             ueDLContext_p->isAlreadyInCEPriorityList = FALSE;
            /* - Bug 20794 */
        }
        if(firstCellEdgeNode)
        {
            ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) popNode (dlCellEdgeUserPriorityQueue_gp[internalCellIndex]);
            /* +COVERITY 11432 */
            if (NULL == ceUserPriorityQueueNode_p)
            {
                break;
            }
            /* -COVERITY 11432 */

            ceUserPriorityQueueNodeTmp_p = ceUserPriorityQueueNode_p;
            ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *)
                getNextNode ((const LTE_SNODE *) (ceUserPriorityQueueNodeTmp_p));

            freeMemPool (ceUserPriorityQueueNodeTmp_p);
        }
        else
        {
            ceUserPriorityQueueNode_p->isNodeInvalid = TRUE;
            ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
        }

            /* + Bug 20794 */
/*            code removed.*/
            /* - Bug 20794 */
    }
    return MAC_SUCCESS;
}




