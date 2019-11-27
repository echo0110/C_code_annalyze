/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLFairScheduler.c,v 1.1.1.1.6.1.4.2.2.2 2010/10/11 12:57:00 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This File is used for distributing the Downlink RBs based
                       on the basis of proportionally fair scheduling. 
                       First calculates the Weighted priority of a UE 
                       considering different factors i.e. UE Priority Factor,
                       Modulation Factor,Delay Factor and Queue Load factor 
                       and then allocatesResource blocks to the UEs based on 
                       the value of their respective weighted priorities.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacDLFairScheduler.c,v $
 * Revision 1.1.1.1.6.1.4.2.2.2  2010/10/11 12:57:00  gur10121
 * SPR #706 fix
 *
 * Revision 1.1.1.1.6.1.4.2.2.1.4.1  2010/10/08 07:42:43  gur10121
 *  SPR 706 Fix - KlockWork Warnings removed
 *
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:46:19  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.48  2009/12/02 07:10:28  gur18550
 *  Removed un-necessary function call numLCWithNonZeroQueueLoad
 *
 * Revision 1.47  2009/12/02 06:34:45  gur18550
 * Bug Solved for tempNewCalculatedRBs
 *
 * Revision 1.46  2009/11/06 06:33:09  gur11912
 * modified for HARQ free cases in case of retransmissions
 *
 * Revision 1.45  2009/09/02 06:07:50  gur20052
 * Added warning in UT_TESTING FLAG
 *
 * Revision 1.44  2009/08/07 12:24:25  gur20052
 * UT Defects is fixed
 *
 * Revision 1.43  2009/08/04 15:25:11  gur20052
 * Added more LOGS related to subnband info
 *
 * Revision 1.42  2009/08/01 09:42:18  gur20052
 * Added the implementation of TA DRX,now cheking pendingTA/DRX Ack
 *
 * Revision 1.41  2009/07/30 18:10:14  gur20052
 * UT Defects are fixed related to isAlreadyConsideredFlag
 *
 * Revision 1.40  2009/07/29 13:16:48  gur20052
 * UT Defects are fixed
 *
 * Revision 1.39  2009/07/24 12:39:34  gur20052
 * Splint Warnings are removed
 *
 * Revision 1.38  2009/07/22 14:18:50  gur20052
 * UT Defect are fixed
 *
 * Revision 1.37  2009/07/20 17:45:04  gur20052
 * UT Defects are Fixed
 *
 * Revision 1.36  2009/07/16 17:36:14  gur20052
 * Added code for Setting up of aperiodicCQIRequested flag in DL UE Context and UT Bug Fix
 *
 * Revision 1.35  2009/07/15 15:02:55  gur20052
 * changed the MacRet type
 *
 * Revision 1.34  2009/07/15 05:59:30  gur20052
 * UT Defects fixed
 *
 * Revision 1.33  2009/07/13 12:25:10  gur20052
 *
 * Revision 1.32  2009/07/10 13:50:38  gur20052
 * MACModuleLogDetail_g is replaced with the Module Specific STRINGS
 *
 * Revision 1.31  2009/07/07 13:28:42  gur20052
 * Updated for UT Defects
 *
 * Revision 1.30  2009/07/07 05:32:13  gur20052
 * UT Defects are fixed
 *
 * Revision 1.29  2009/07/02 10:57:36  gur20052
 * Enhancement Comments are incorporated
 *
 * Revision 1.28  2009/06/29 15:09:13  gur20052
 * UT Defects are removed
 *
 * Revision 1.26  2009/06/24 08:10:45  gur20052
 * Reveiw Comments are incorporated
 *
 * Revision 1.25  2009/06/23 16:55:42  gur20052
 * Review comments are incorporated
 *
 * Revision 1.23  2009/06/18 09:27:21  gur20052
 * UT Defects are fixed
 *
 * Revision 1.22  2009/06/18 06:44:20  gur20052
 * UT Defects are removed
 *
 * Revision 1.21  2009/06/11 14:52:56  gur20052
 * Removed the Linking Errors and Resource Allocator Node
 *
 * Revision 1.20  2009/06/10 14:26:07  gur20052
 * Removed the Linking errors and updated for createMemPool changes
 *
 * Revision 1.19  2009/06/08 09:59:34  gur20052
 * removed the linking errors
 *
 * Revision 1.18  2009/06/05 16:56:28  gur20052
 * Incorporated the reveiw comments
 *
 * Revision 1.17  2009/06/05 16:20:23  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.16  2009/06/05 06:33:47  gur20052
 * Incorporated the reveiw comments
 *
 * Revision 1.15  2009/06/04 07:39:50  gur20052
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
 * Revision 1.8  2009/05/21 06:24:55  gur20052
 * Added code for round robin Scheduler
 *
 * Revision 1.7  2009/05/20 16:56:04  gur20052
 * Updated the initial version
 *
 * Revision 1.6  2009/05/20 13:02:47  gur20052
 *  Added The new Enums for message Type
 *
 * Revision 1.5  2009/05/20 09:47:17  gur20052
 * incorporated the code reveiw coments
 *
 * Revision 1.4  2009/05/20 07:16:56  gur20052
 * Added Harq Process changes
 *
 * Revision 1.3  2009/05/19 14:29:31  gur20052
 * Updated with DL Fair Scheduler module specific changes
 *
 * Revision 1.2  2009/05/11 06:18:32  gur20052
 * Removed compilation errors
 *
 * Revision 1.1  2009/04/30 16:55:42  gur20052
 * Added initial version of DL Fair Scheduler
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacDLFairScheduler.h"
#include "lteMacCellConfMgr.h"
/* SPR 3083 Fix begin */
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif
/* SPR 3083 Fix end */

/******************************************************************************
 Private Definitions
 *****************************************************************************/
/* SPR 3083 Fix begin */
#ifdef TDD_CONFIG
/* The below array stores the number of DL Subframes in one radio frame 
 * in which PDSCH is  available for DL-SCH data
 *
 * dataForSpecialSubFrame = TRUE  for specialSubFramePattern other than 0 or 5 
 * dataForSpecialSubFrame = FALSE for specialSubFramePattern 0 or 5 
 */
static UInt8 tddNumOfPDSCH[MAX_UL_DL_CONFIG][2] = 
                          /*[TDD Config][dataForSpecialSubFrame]*/
{
            {2,4},
            {4,6},
            {6,8},
            {6,7},
            {7,8},
            {8,9},
            {3,5}
};
#endif
/* SPR 3083 Fix end */
/******************************************************************************
 Private Types
 ****************************************************************************/

 /*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* This function is used to compare the weighted Priority from first n
 * number of nodes of dlUENonZeroQueueLoadAndHarqList to sort top n nodes.
 */
static void dlPfsCheckRemainingNodes(
        LTE_LIST_NODE *node_p, //node of dlUENonZeroQueueLoadAndHarqList
        UInt8  maxUEsToBeScheduledDL, 
       
        /*SPR 21068 start */ 
         tickType_t dlReferenceFlag,
        /*SPR 21068 end   */
        InternalCellIndex internalCellIndex);
/* ICIC changes start */
static void dlPfsInsertCENodeInList(
                      LTE_LIST_NODE *pSortNodeT,
                      SortNodeT *pSortNodeAdd,
                      InternalCellIndex internalCellIndex);
static void dlPfsReplaceCENodeInList(
                      SortNodeT *pSortNodeT,
                      SortNodeT *pSortNodeAdd
                      ,InternalCellIndex internalCellIndex);
static void dlPfsInsertNodeAtHead(
                      SortNodeT *pSortNodeAdd,
                      DLUEContext  *ueDLContext_p,
                      UInt8 *additionalNodeToBeDeleted_p,
                      InternalCellIndex internalCellIndex);
/* ICIC changes end */

/* This function is sort top n node from dlUENonZeroQueueLoadAndHarqList List */
static void dlPfsCheckIfNodetoBeScheduled(
        LTE_LIST_NODE *node_p, //node of dlUENonZeroQueueLoadAndHarqList
        DLUEContext   *ueDLContext_p,
        UInt8  maxUeToBeSorted,
        InternalCellIndex internalCellIndex);
/* Initialisation function for DL PFS Scheduler*/
void dlPfsInitMacSchedulerData(UInt8 internalCellIndex);
/*  Initialisation of Static Memory Pool for DL PFS Scheduler */
static void dlPfsInitMemAllocElem(InternalCellIndex internalCellIndex);
/* Function for Allocation of DL PFS Static Memory Pool */
static SortNodeT *dlPfsAllocSortNodeElem(InternalCellIndex internalCellIndex );
/* Function for Deallocation of DL PFS Static Memory Pool */
static void dlPfsFreeSortNumElem (SortNodeT *pSortNode,InternalCellIndex internalCellIndex);
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
static SortNodeT *leastPriorityUe_p[MAX_NUM_CELL] = {PNULL}; 
/* ICIC changes start */
static SortNodeT *leastPriorityCEUe_p[MAX_NUM_CELL] = {PNULL}; 
/* ICIC changes end */
PFSGlobals *pfsGlobals_gp[MAX_NUM_CELL] = {PNULL};
 /* + CL-MIMO LJA*/
 /******************************************************************************************
  * Function Name  : processDLNewDataWithOneTBOnlyRIOne
  * Inputs         : allocatedRB - The Number of RBs Allocated Orginally,
  *                  availableRB_p - Pointer to the number of Avavailable RBs,
  *                  ueDLContext_p - Pointer to UE Context,
  *                  dlSchedulerDataNodeArr_p - pointer to node which contains the
  *                                             information related to the scheduling
  *                                             i.e number of RBs allocated,
  *                                             Weighted Priority, etc
  *                  currentGlobalTTITickCount - Current global TTI Tick Count,
  *                  tempStrategyInputInfoForOneTB - Temp strategy input info for TB one,
  *                  internalCellIndex - cell index used at MAC.
  * Outputs        : Process the UE for One TB with New Data
  * Returns        : MAC_SUCCESS/MAC_FAILURE
  * Description    : This function is for processing the UE for New Transmission 
  *                  For one TB only.First pick up a fresh harq process and then 
  *                  validate the RBs allocated. After which output is prepared 
  *                  and sent to PDCCH Module.
  ******************************************************************************************/
static MacRetType processDLNewDataWithOneTBOnlyRIOne(
        UInt32 allocatedRB,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        , tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#endif
         /* + CQI_5.0 */
        ,TempStrategyInputInfoForOneTB tempStrategyInputInfoForOneTB
        /* - CQI_5.0 */
        ,InternalCellIndex internalCellIndex
	)
{
    UInt32 usedRB = 0;
    UInt32 tbSize = 0;
    DLHARQProcess * harqProcess_p   = PNULL;

    UInt32 minTBS = 0;
    UInt32 maxTBS = 0;

    UInt8 numLCWithNonZeroQueueLoad = 
        tempDLSchedulerDataNodeArr_p->numLCWithNonZeroQueueLoad;
    UInt32 sumLCPriority        = tempDLSchedulerDataNodeArr_p->sumLCPriority;
    UInt32 weightedQueueLoad    = tempDLSchedulerDataNodeArr_p->weightedQueueLoad;
    MsgType msgType             = tempDLSchedulerDataNodeArr_p->msgType;
    ResourceAllocatorInput * tempResourceAllocatorInput_p = PNULL;
    DLStrategyTxNode * dlStrategyTxNode_p=PNULL;



#ifdef TDD_CONFIG
    UInt8 subFrame = currentGlobalTTITickCount % MAX_SUBFRAME;
#endif
    
    harqProcess_p = &ueDLContext_p->dlHarqContext_p->harqProcess[
        tempDLSchedulerDataNodeArr_p->harqProcessId];

    /* ICIC changes start*/
    allocatedRB += pfsGlobals_gp[internalCellIndex]->unUsedRBDL;
 /*   if (TRUE ==  tempStrategyInputInfoForOneTB.isTBOneValid)
    { */   
    usedRB = validateRBForOneTB(ueDLContext_p, allocatedRB, 
                                numLCWithNonZeroQueueLoad, &tbSize, 
                                /* SPR 17777 */
                                *availableRB_p, msgType,&minTBS,&maxTBS
#ifdef TDD_CONFIG
                                , subFrame
                                ,internalCellIndex
#endif    
                                /* SPR 17777 */
                               );


/*
    }
    else if(TRUE == tempStrategyInputInfoForOneTB.isTBTwoValid)
    {
    
        usedRB = validateRBForOneTB(ueDLContext_p, allocatedRB, 
                                    numLCWithNonZeroQueueLoad, &tbSize, 
                                    availableRB_p, msgType,TB_TWO,&minTBS,&maxTBS
#ifdef TDD_CONFIG
                                   , subFrame
#endif        
                                    );
    }
*/   

    /*if the used RBs are more than the available RBs, then clip the used RBs
      to the availabale RBs*/
    if (usedRB  > *availableRB_p)
    {
        usedRB =  *availableRB_p ;
    }

    /* Updating unUsedRBDL_g */

    if(allocatedRB > usedRB)
    {
        pfsGlobals_gp[internalCellIndex]->unUsedRBDL = allocatedRB - usedRB;
    }
    else
    {
       pfsGlobals_gp[internalCellIndex]->unUsedRBDL = 0;
    }


    /* ICIC changes end*/
    if (usedRB)
    {

        /* + CQI_5.0 */
        /*Setting TB valid based on RI and CQI conditions*/
        harqProcess_p->isTBOneValid = tempStrategyInputInfoForOneTB.isTBOneValid;
        harqProcess_p->isTBTwoValid = tempStrategyInputInfoForOneTB.isTBTwoValid;
        harqProcess_p->dlHARQTBOneInfo.txIndicator =
                                 tempStrategyInputInfoForOneTB.txIndicatorForTB1;
        harqProcess_p->dlHARQTBTwoInfo.txIndicator =
                                 tempStrategyInputInfoForOneTB.txIndicatorForTB2;
        /* - CQI_5.0 */
        /*Preparing the DL Strategy Output Node*/
        GET_MEM_FROM_POOL(DLStrategyTxNode, dlStrategyTxNode_p,\
                sizeof(DLStrategyTxNode),PNULL);
	
       /* + Coverity 24523 */
	if( PNULL == dlStrategyTxNode_p )
	{
            freeDLHarqProcess(ueDLContext_p, 
                  &ueDLContext_p->dlHarqContext_p->harqProcess
                        [harqProcess_p->harqProcessId],
                        internalCellIndex);
	    return MAC_FAILURE ;
	}
	/* - Coverity 24523 */
       
        tempResourceAllocatorInput_p = &(dlStrategyTxNode_p->resourceAllocInput);
        tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
        tempResourceAllocatorInput_p->strictAllocSubBand  = FALSE;
        dlStrategyTxNode_p->isCQIRequestedULGrantMsg      = FALSE;

        dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;
        dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId ;
        dlStrategyTxNode_p->transmissionType  = NEW_TX;
        dlStrategyTxNode_p->sumLCPriority     = sumLCPriority ;
        dlStrategyTxNode_p->weightedQueueLoad = weightedQueueLoad;
        /* SPR #3098 Changes Starts*/
        /* + CQI_5.0 */
     	dlStrategyTxNode_p->allocPushCount = 0;
        dlStrategyTxNode_p->strictAllocatedFlag = FALSE;
        dlStrategyTxNode_p->minTBSize = minTBS;
        dlStrategyTxNode_p->maxTBSize = maxTBS;
        tempResourceAllocatorInput_p->subBandPMIInfo_p = PNULL;
        tempResourceAllocatorInput_p->subbandPMI = FALSE;
        tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;       
        dlStrategyTxNode_p->msgType = SPATIAL_MUL;
        /*DCI Format Calculation*/
        dlStrategyTxNode_p->primaryDCIFormat = tempStrategyInputInfoForOneTB.primaryDCIFormat;
        dlStrategyTxNode_p->secondaryDCIFormat = tempStrategyInputInfoForOneTB.secondaryDCIFormat;
        /* - CQI_5.0 */
        /* SPR 3403 changes end*/
        /*Setting the message Type accoding to the input and change the same
          according to the Transmission . Setting the TA DRX Fields as well   */
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

        /*Finding whether Subband Report is available */
        if (ueDLContext_p->dlCQIInfo.isSubBandReportAvailable)
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
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
            /*else
            {
                harqProcess_p->dlHARQTBTwoInfo.mcsIndex  =  
                    ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
                harqProcess_p->dlHARQTBOneInfo.tbSize  =  0;
                harqProcess_p->dlHARQTBTwoInfo.tbSize  =  tbSize;
            } */  
            /* - CQI_5.0 */
        }

        tempResourceAllocatorInput_p->requiredRB = usedRB;

        harqProcess_p->assignedResourcesNB = usedRB;

            /*Updating the remaining RB available*/
            /* ICIC changes start */
            *availableRB_p -= tempResourceAllocatorInput_p->requiredRB;

            dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated = 
                ueDLContext_p->userLocationType;
        /*Updating the ResourceAllocatorInput in the DL Strategy TX Node*/

        /*putEntry of DLStrategyTXNode in PDCCH Queue */
        
        /* + Coverity 32489 */
        if( MAC_FAILURE == putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex))
	{
         freeDLHarqProcess(ueDLContext_p, 
                  &ueDLContext_p->dlHarqContext_p->harqProcess
                        [harqProcess_p->harqProcessId],
                        internalCellIndex);
         return MAC_FAILURE;
	}
        /* + Coverity 32489 */

        LOG_MAC_MSG (DL_NEWTX_ONETBONLYRIONE_UE_SCHEDULED_LOG_ID, LOGDEBUG,
                    MAC_DL_Strategy, getCurrentTick (), ueDLContext_p->ueIndex,
                    dlStrategyTxNode_p->harqProcessId,
                    /*START:6179*/
                    harqProcess_p->assignedResourcesNB,
                    /*END:6179*/
                    ueDLContext_p->ueQueueLoad,
                    dlStrategyTxNode_p->msgType,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
    }
    else
    {
        /*If the Used RB returns fail, then the Harq Process will be freed*/

        LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
                "\nThe DL HARQ Process ID ::%d  of UEID %d has been succesfully"
                " freed", harqProcess_p->harqProcessId,ueDLContext_p->ueIndex );

        /* ICIC changes start*/
         freeDLHarqProcess(ueDLContext_p, 
                  &ueDLContext_p->dlHarqContext_p->harqProcess
                        [harqProcess_p->harqProcessId],
                        internalCellIndex);
        /* ICIC changes end*/
        return MAC_FAILURE;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,"Exit:[DL-Strategy][%s]", __func__);
    return MAC_SUCCESS;
}
/* - CL-MIMO LJA*/

/*****************************************************************************
 * Function Name  : dlProportionalFairScheduler
 * Inputs         : dlReferenceFlag - Used for checking whether the US is 
 *                                    considered already or not. If UE's 
 *                                    isAlreadyConsideredFlag is equal to
 *                                    dlReferenceFlag, that means the UE is
 *                                    already considered,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  dlScheduleData_p - Pointer to DL Scheduler Data Element,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  maxUEsToBeScheduledDL - Max DL UE's to be scheduled,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Distribution of RB based on the UE's Weighted priority
 * Returns        : MAC_SUCCESS
 * Description    : This is used for distributing the RBs based on the 
 *                  proportionally fair scheduling. This function allocates 
 *                  Resource blocks to the UEs based on the value of weighted 
 *                  priorities.Weighted priority of a UE is calculated 
 *                  considering different factors i.e. UE Priority Factor,
 *                  Modulation Factor,Delay Factor and Queue Load factor
 *                  and then allocatesResource blocks to the UEs based on
 *                  the value of their respective weighted priorities.
 *****************************************************************************/
UInt32 dlProportionalFairScheduler(
        /* SPR 21068 start */ 
        tickType_t dlReferenceFlag,
        /* SPR 21068 end   */
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement *dlScheduleData_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* SPR 17777 */
        UInt8  maxUEsToBeScheduledDL, /* RETX_CHG */
        InternalCellIndex internalCellIndex)
{
    DLUEContext* ueDLContext_p  = PNULL;
    /** CA-TDD Changes Start **/
    static UInt32 qLoadWeightedMean_g[MAX_NUM_CELL] = {0};
    /** CA-TDD Changes Stop **/
    UInt32 tempTotalUE                = 0;
    UInt32 index                      = 0;
    UInt32 tempCountTADRX             = 0;
    UInt32 tempTotalActiveUE          = 0;
    UInt32 totalUeCount               = 0;

    /* ICIC changes start */
    DLHARQProcess * harqProcess_p = PNULL;
    UInt32 tempRBCount = 0;
    /* sumWeightedPriority for CC and CE User */
    UInt32 sumWeightedPriority[MAX_USER_LOCATION]  = {0,0};

    /* dlUserListPFS contains CC and CE user at index 0 and 1
     * respectively */
    /* SPR 21074 start */
    DLUserListPFS dlUserListPFS[MAX_USER_LOCATION] = {{0}};
    /* SPR 21074 end   */
    UInt8 ueLocationType = CC_USER;
    UInt8 userCount = 0;

    /* ICIC changes end */
    DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p 
        =  dlScheduleData_p->dlSchedulerDataNodeArr;
    tempCountTADRX      = dlScheduleData_p->countTADRX ;
    tempTotalActiveUE   = listCount(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]);
    UInt32 totalUEInDLList = 0;
    SortNodeT *sortNode_p = PNULL;
    LTE_LIST_NODE *node_p = PNULL;
    UInt32 i,j = 0;
    UInt32 numUeRotate = 0;
    /* SPR 2261 changes start */
    UInt32 ueCount = 0;
    /* SPR 2261 changes end */

    UInt32 tempnumberOfAvailableRBs = numberOfAvailableRBs_p[CC_CE_REGION];

    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    /* ICIC changes start */
    /* Initializing list count with 0 */
    dlUserListPFS[CC_USER].count = 0;
    dlUserListPFS[CE_USER].count = 0;

    /* ICIC changes end */
    if ( !tempTotalActiveUE 
            || (tempCountTADRX + maxUEsToBeScheduledDL) == 0 )
    {
        /*LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
          "[DL-Strategy][%s]Returning because tempTotalActiveUE is ZERO\n",
          __func__);*/
        return 0;
    }

    /* Calculate mean Qload for all UEs after every calcQloadMeanItr_g ticks */
    if ( !(pfsGlobals_gp[internalCellIndex]->calcQloadMeanItr) )
    {
        if ( cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->dlSchdConfig.dwlkUEQLoadWt )
        {
            /* 
            ** 1. Calc weightedQueueLoad for each UE present in
            ** dlUENonZeroQueueLoadAndHardlPfsFreeSortNumElemqList containing Entries of UE with
            ** Non-Zero Queue Load 
            */
            /** CA-TDD Changes Start **/
            iterateNonZeroListsAndCalculateWeightedQueueLoad
                   (dlScheduleData_p,
                    &qLoadWeightedMean_g[internalCellIndex], 
                    &tempTotalUE,
                    currentGlobalTTITickCount,
                    dlReferenceFlag,
                    internalCellIndex);
            /** CA-TDD Changes Stop **/
            if ( 0 < tempTotalUE )
            {
                /** CA-TDD Changes Start **/
                qLoadWeightedMean_g[internalCellIndex] = qLoadWeightedMean_g[internalCellIndex] /tempTotalUE;
                /** CA-TDD Changes Stop **/
            }
            else
            {
                return  ZERO;
            }
            /* 
            ** This needs to be discussed why calcQloadMeanItr_g is not
            ** equal to NUM_ITER_TO_CALC_MEAN_Q_LOAD_DL 
            */
		    pfsGlobals_gp[internalCellIndex]->calcQloadMeanItr = NUM_ITER_TO_CALC_MEAN_Q_LOAD_DL;
		    pfsGlobals_gp[internalCellIndex]->qLoadComputed = TRUE;
        }
    }
    else
    {
        pfsGlobals_gp[internalCellIndex]->calcQloadMeanItr --;
    }

    /* 2. Compute weighted priorities for TA DRX UEs */
    for (i = 0; i < dlScheduleData_p->countTADRX; i++)
    {
        ueDLContext_p = tempDLSchedulerDataNodeArr_p[i].ueDLContext_p;
        /*
        ** Calculation of Weighted priority of All TA DRX UEs present in
        ** Scheduler Array
        */
        /** CA-TDD Changes Start **/
        tempDLSchedulerDataNodeArr_p[i].weightedPriority =
            calculateDLWeightedPriority(
             ueDLContext_p,qLoadWeightedMean_g[internalCellIndex],
             &(tempDLSchedulerDataNodeArr_p[i]),
             currentGlobalTTITickCount,
             internalCellIndex);
        /** CA-TDD Changes Stop **/

        /* update weighted priority in ueContext */
        ueDLContext_p->dlSchedulerAlgoInfo.weightedPriority =  
          tempDLSchedulerDataNodeArr_p[i].weightedPriority;
    } 

    /* 
    ** 3. Calculate weighted priority for UEs scheduled in last tick
    ** Since these UEs were scheduled, their priorities now are incorrect
    */

    while ( (sortNode_p = 
               (SortNodeT *)getFirstListNode(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList))) )
    {
        ueDLContext_p = dlUECtxInfoArr_g[sortNode_p->ueIndex].dlUEContext_p;

        if ( ueDLContext_p )
        {
            /* update weighted priority in dlUENonZeroQueueLoadAndHarqList;*/
            if ( ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p )
            {
                /** CA-TDD Changes Start **/
                ueDLContext_p->dlSchedulerAlgoInfo.weightedPriority = 
                calculateDLWeightedPriority(ueDLContext_p,
                            qLoadWeightedMean_g[internalCellIndex],
                                            NULL,
                            currentGlobalTTITickCount,
                            internalCellIndex
                            );
                /** CA-TDD Changes Stop **/

            }

        }
        listDeleteNode(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList) ,(LTE_LIST_NODE *) sortNode_p);
        dlPfsFreeSortNumElem(sortNode_p,internalCellIndex);
    }

    /* 
    ** Check if maxUEsToBeScheduledDL exist
    ** compute UE priority only if any more UEs to be scheduled 
    */
    if ( (maxUEsToBeScheduledDL) && (tempTotalActiveUE) )
    {
        /* 
        ** Before preparing the list of highest priority UEs, ensure that
        ** pointers pointing to leastPriority UE and leastPriorityCE UE
        ** are NULL
        */
        leastPriorityUe_p[internalCellIndex] = PNULL;
        /* ICIC changes start */
        leastPriorityCEUe_p[internalCellIndex] = PNULL;
        /* ICIC changes end */

        /* 
        ** 4. Calculate The Weighted Priority for first eligible
        ** NUM_UES_COMPUTED_ONE_TICK_DL
        ** present in dlUENonZeroQueueLoadAndHarqList 
        */

        node_p = getListNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex],0);

        for (j=0 ,numUeRotate =0; 
                     (j < NUM_UES_COMPUTED_ONE_TICK_DL) && node_p; )
        {
            ueDLContextInfo_p = &dlUECtxInfoArr_g[\
                    ((NonZeroQueueLoadAndHarqNode*)node_p)->ueIndex];
            /* 
            ** Null check for ueDLContextInfo_p not made as it should always
            ** be non null if valid node exists in 
            ** dlUENonZeroQueueLoadAndHarqList 
            */
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
            if ( !(ueDLContextInfo_p->pendingDeleteFlag) )
            {
                /* 
                ** If the UE is Non TA/DRX with HARQ process allocated, 
                ** process it else ignore.
                */
                if ( ueDLContext_p->isAlreadyConsideredFlag != dlReferenceFlag ) 
                {
                    if ( ueDLContext_p->dlSchedulerAlgoInfo.lastCompWTPriorityTick 
                            != currentGlobalTTITickCount )
                    {   /** CA-TDD Changes Start **/ 
                        ueDLContext_p->dlSchedulerAlgoInfo.weightedPriority =
                            calculateDLWeightedPriority(ueDLContext_p,
                                    qLoadWeightedMean_g[internalCellIndex],
                                    NULL,
                                    currentGlobalTTITickCount,
                                    internalCellIndex
                                    );
                            /** CA-TDD Changes Stop **/

                    }
                    /* ICIC changes start */
                    dlPfsCheckIfNodetoBeScheduled(node_p, ueDLContext_p,
                                            maxUEsToBeScheduledDL,
                                            internalCellIndex);
                    /* ICIC changes end */
                    j++;
                }
                node_p = getNextListNode(node_p);
                numUeRotate++;
            }
            else
            {
                LTE_LIST_NODE * oldNode_p = PNULL;
                oldNode_p = getNextListNode(node_p);
                listDeleteNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex], node_p);
                freeMemPool((void *)node_p);
                /* Node is getting deleted, so mark dlUENonZeroQueueLoadAndHarqList_p
                   NULL in UE context*/
                if(ueDLContext_p)
                {
                    ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p = PNULL;
                }
                node_p= oldNode_p;
            }
        }

        /* 
        ** 5. Find maxUEsToBeScheduledDL UEs with max priorities out of all
        ** UEs
        */
        if ( node_p )
        {
            dlPfsCheckRemainingNodes(node_p, 
                                     maxUEsToBeScheduledDL, 
                                     dlReferenceFlag,
                                     internalCellIndex);
        }

        /* 6. Top n UEs are present in sortList. Pick these for scheduling */

        /* Calculate the sum weighted priority of Non TADRX UEs */
        sortNode_p = (SortNodeT *) getFirstListNode(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList));
        i = dlScheduleData_p->countTADRX;
        /* SPR 7016 Fix Start */
        while ( sortNode_p && (i < (MAX_DL_SCH_ARR * MAX_ADVANCE_DL_SUBFRAMES_ALLOC)))
        {
        /* SPR 7016 Fix End */
            ueDLContext_p = dlUECtxInfoArr_g[sortNode_p->ueIndex].dlUEContext_p;

            if ( ueDLContext_p->dlSchedulerAlgoInfo.lastCompWTPriorityTick
                    == currentGlobalTTITickCount )
            {
                tempDLSchedulerDataNodeArr_p[i].weightedPriority = 
                   ueDLContext_p->dlSchedulerAlgoInfo.weightedPriority;
                tempDLSchedulerDataNodeArr_p[i].numLCWithNonZeroQueueLoad = 
                   ueDLContext_p->dlSchedulerAlgoInfo.numLCWithNonZeroQueueLoad;
                tempDLSchedulerDataNodeArr_p[i].sumLCPriority  = 
                   ueDLContext_p->dlSchedulerAlgoInfo.sumLCPriority;
                tempDLSchedulerDataNodeArr_p[i].weightedQueueLoad = 
                   ueDLContext_p->dlSchedulerAlgoInfo.weightedQueueLoad;      
            }
            else
            {
                /** CA-TDD Changes Start **/
                tempDLSchedulerDataNodeArr_p[i].weightedPriority =
                    calculateDLWeightedPriority(
                            ueDLContext_p,qLoadWeightedMean_g[internalCellIndex],
                            &(tempDLSchedulerDataNodeArr_p[i]),
                            currentGlobalTTITickCount,
                            internalCellIndex
                            );
                /** CA-TDD Changes Start **/
            }

            tempDLSchedulerDataNodeArr_p[i].ueDLContext_p = ueDLContext_p;

            /* Update the last scheduled TICK */

            i++;

            /* update weighted priority in dlUENonZeroQueueLoadAndHarqList */
            sortNode_p = 
                (SortNodeT *)getNextListNode((LTE_LIST_NODE *)sortNode_p);
        }

        dlScheduleData_p->dataNodeArrTotalUE = i;

        /*
        ** 7. Put computed UEs at end of list dlUENonZeroQueueLoadAndHarqList
        ** to enable round robin selection of UEs to be computed
        */

        for ( i = 0; i< numUeRotate  ;i++ )
        {
            node_p = getFirstListNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]);
            listDeleteNode (dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex], node_p);
            listPushNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex], node_p);
        }
    }
    else
    {
        /*
        ** If all Scheduled UEs are TA/DRX then array needs to be 
        ** added with value NUM_BUFFER_UES_DL in order to make 
        ** below for() loop consistent for all Non-TA/DRX, TA/DRX
        ** scenarios.
        */
        dlScheduleData_p->dataNodeArrTotalUE = 
             dlScheduleData_p->dataNodeArrTotalUE + NUM_BUFFER_UES_DL;
    }
     pfsGlobals_gp[internalCellIndex]->qLoadComputed = FALSE;

    if ( tempTotalActiveUE < (UInt32)(maxUEsToBeScheduledDL + NUM_BUFFER_UES_DL) )
    {
        totalUeCount = maxUEsToBeScheduledDL;

        /*SPR 6986 Fix Start*/
        if(dlScheduleData_p->dataNodeArrTotalUE > tempTotalActiveUE)
        {
            dlScheduleData_p->dataNodeArrTotalUE = tempTotalActiveUE;
        }
        /*SPR 6986 Fix End*/
    }
    else
    {
        totalUeCount = 
             dlScheduleData_p->dataNodeArrTotalUE - NUM_BUFFER_UES_DL;
    }

    /* calculate the sum weighted priority of TADRX UEs */
 
    /* SPR 21074 start */
    for (index= 0; ((ueCount < totalUeCount) 
          && (index < dlScheduleData_p->dataNodeArrTotalUE))||(index < tempCountTADRX); index++)
    /* SPR 21074 end   */ 
 
   {
        ueDLContext_p = tempDLSchedulerDataNodeArr_p[index].ueDLContext_p;
        ueLocationType = ueDLContext_p->userLocationType;
        
        /* 
        ** The following check is for Non TA/DRX elements. Allocating
        ** HARQ Process to Non TA/DRX with Data 
        */
        if(index >= tempCountTADRX)
        {
            /* \code
             * if schType == SPS_SCHEDULING, then do not schedule
             * this UE as it is already considered by SPS.
             * */
            if( SPS_SCHEDULING == ueDLContext_p->schType )
	        {
	            continue;
	        }
            /* 
            ** UE does not have TA/DRX with DATA.Hence HARQ process has
            ** to be allocated 
            */
            harqProcess_p = checkForFreeHarqProcess
                (ueDLContext_p, ueDLContext_p->dlHarqContext_p, 
                DRA_SCHEDULING, internalCellIndex);
            if(!harqProcess_p)
            {
                /* 
                ** Harq process is not free for this UE. Hence skipping
                ** UE 
                */
                continue;
            }
            tempDLSchedulerDataNodeArr_p[index].harqProcessId =
                harqProcess_p->harqProcessId;
        }
        sumWeightedPriority[ueLocationType] += 
          tempDLSchedulerDataNodeArr_p[index].weightedPriority;
        userCount = dlUserListPFS[ueLocationType].count;
        dlUserListPFS[ueLocationType].indexToDlSchedulerDataElement
            [userCount]= index;
        dlUserListPFS[ueLocationType].count++;
        ueCount++;
    }

    /*Updating the sumWeightedPriority in the dlScheduleData_p*/
    dlScheduleData_p->sumWeightedPriority[CC_USER] = sumWeightedPriority[CC_USER];
    dlScheduleData_p->sumWeightedPriority[CE_USER] = sumWeightedPriority[CE_USER];

    /* ICIC changes start */
    /* Processing CE user list */
    userCount = dlUserListPFS[CE_USER].count;
    totalUEInDLList = userCount + dlUserListPFS[CC_USER].count;
#ifdef LOG_PRINT_ENABLED
    UInt8 initialUsertCount = totalUEInDLList; 
#endif
    if( userCount )
    {
        /* Resetting unUsedRBDL */
        pfsGlobals_gp[internalCellIndex]->unUsedRBDL = 0;
        tempRBCount = numberOfAvailableRBs_p[CE_REGION];

        /*Allocating RBs to CE users*/
        /* SPR 2261 changes start */
         allocDLRBsToSortedUEs( 
            &numberOfAvailableRBs_p[CE_REGION],
            dlScheduleData_p,
#ifdef TDD_CONFIG
            currentGlobalTTITickCount,
#endif
            &dlUserListPFS[CE_REGION],
            &totalUEInDLList 
            ,internalCellIndex
            );
        /* SPR 2261 changes end */

        /* Updating CC_CE_REGION RB count after allocating resources
         * to all of CE users*/
        numberOfAvailableRBs_p[CC_CE_REGION] -= 
            tempRBCount - numberOfAvailableRBs_p[CE_USER];
    } 


    /* Processing CC user list */
    userCount = dlUserListPFS[CC_USER].count;
    if( userCount )
    {
        /* As unUsedRBDL_g for Cell Edge RBs is already added to initial
         * available RB for Cell Center. Hence resetting this to 0 */
        pfsGlobals_gp[internalCellIndex]->unUsedRBDL = 0;

        /*Allocating RBs to CC users*/
        /* SPR 2261 changes start */
        allocDLRBsToSortedUEs( 
            &numberOfAvailableRBs_p[CC_CE_REGION],
            dlScheduleData_p,
#ifdef TDD_CONFIG
            currentGlobalTTITickCount,
#endif
            &dlUserListPFS[CC_CE_REGION],
            &totalUEInDLList
            ,internalCellIndex
            );
        /* SPR 2261 changes end */

        /* Updating CE Region RB count*/
        if( numberOfAvailableRBs_p[CC_CE_REGION] < 
                numberOfAvailableRBs_p[CE_REGION] )
        {
            numberOfAvailableRBs_p[CE_REGION] = 
                numberOfAvailableRBs_p[CC_CE_REGION];
        }
    }

#ifdef LOG_PRINT_ENABLED
    LOG_MAC_MSG (DL_MAX_UE_SCHEDULED_AND_AVAILABLE_RB_PER_TICK, LOGINFO,
                   MAC_DL_Strategy, getCurrentTick (),
                   tempTotalActiveUE,
                   tempnumberOfAvailableRBs,
                   dlUserListPFS[CC_USER].count,
                   0,0,
                   DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
#endif
 
    return MAC_SUCCESS ;
}

/*****************************************************************************
 * Function Name  : iterateNonZeroListsAndCalculateWeightedQueueLoad 
 * Inputs         : dlScheduleData_p - Pointer to DL Scheduler Data Element,
 *                  qLoadWeightedMean_p - pointer to the weighted Queue Load 
 *                                        Mean,
 *                  totalUECount - total UE Count,
 *                  currentGlobalTTITickCount - Current Global TTI Tick,
 *                  dlReferenceFlag - Used for checking whether the US is
 *                                    considered already or not. If UE's
 *                                    isAlreadyConsideredFlag is equal to
 *                                    dlReferenceFlag, that means the UE is
 *                                    already considered,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : It calculate the Wegted Queue Load of all the UE present 
 *                  in Non- Zero List
 * Returns        : None
 * Description    : This function iterates through non zero queueload and non 
 *                  Zero Nack Queue lists and calculates their weightedQueue 
 *                  loads and also returns Total weightedQueueload for all 
 *                  the UEs.
 ******************************************************************************/
void iterateNonZeroListsAndCalculateWeightedQueueLoad(
        DLSchedulerDataElement * dlScheduleData_p,
        UInt32 *qLoadWeightedMean_p,
        UInt32 *totalUECount,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* SPR 21068 start */
        tickType_t dlReferenceFlag,
        /* SPR 21068 end   */
        InternalCellIndex internalCellIndex)
{
    LTE_LIST_NODE * node_p          = PNULL;
    DLUEContext* ueDLContext_p      = PNULL;  
    UInt32 ueIndex;
    /* DLHARQEntity *ueharqDLContext_p = PNULL ; TDD warning Fix */
    UInt32 i = 0;
    DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p 
        =  dlScheduleData_p->dlSchedulerDataNodeArr;
    DLUEContextInfo * ueDLContextInfo_p = PNULL;

    /*
    ** Calculating the Weighted Queue Load of All the UEs which have TA DRX
    ** in the Scheduler Info Arr
    */
    for (i = dlScheduleData_p->countTADRX ; i--;)
    {
        ueDLContext_p = tempDLSchedulerDataNodeArr_p[i].ueDLContext_p;
        LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY, 
          "[DL-Strategy][%s:]Calculating Weighted Priority DL of UE Index %d\
          with TA DRX\n",__func__,  
          ueDLContext_p->ueIndex);

        /*
        ** Calculating the Weighted Queue Load of all UEs and update
        ** sumLCPriority,num LC With NonZero QueueLoad, and 
        ** lcId With NonZero QueueLoad
        */
        calculateDLWeightedQueueLoad(ueDLContext_p, qLoadWeightedMean_p);
        (*totalUECount)++;
        ueDLContext_p->isAlreadyConsideredFlag = dlReferenceFlag;
    }

    node_p = getListNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex],0);
    while (node_p)
    {
        ueIndex = ((NonZeroQueueLoadAndHarqNode*)node_p)->ueIndex;
        ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

        LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
                "[%s]UE ID %d's pendingDeleteFlag is %d\n",
                __func__,ueIndex,
                ueDLContextInfo_p->pendingDeleteFlag);

        if ( !(ueDLContextInfo_p->pendingDeleteFlag) )
        {
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

            /* Checking if Already being considered for RB Distribution */
            if ( (ueDLContext_p->isAlreadyConsideredFlag != dlReferenceFlag )
                && (UL_SYNC_LOSS_OFF == 
                    checkUlSyncLossForUE(ueDLContext_p, currentGlobalTTITickCount, internalCellIndex)) )
            {
                /* RETX_CHG */
                if ( 0 == ueDLContext_p->ueQueueLoad )
                /* RETX_CHG */
                {
                    node_p = getNextListNode(node_p);
                    /* SPR 2261 changes start */
                    if ( !ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p )
                    {
                        lteWarning("UE not present in non zero lists\n");
                    }
                    /* SPR 2261 changes end */
                    continue;
                }
                else
                /*RETX_CHG*/
                {
                    if ( NET_UE_QL(ueDLContext_p) <= 0 )
                /*RETX_CHG*/
                    {
                        node_p = getNextListNode(node_p);
                        continue;
                    }
                }
                
                /* calculating The Weighted Queue Load */
                calculateDLWeightedQueueLoad(ueDLContext_p, 
                                             qLoadWeightedMean_p);
                (*totalUECount)++;

                node_p = getNextListNode(node_p);
            }
            /* 
            ** End of already considered flag check, The UE which are
            ** already considered will be deleted automatically 
            */
            else
            {
                LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
                        "[DL-Strategy][%s]:UE is already considered, hence NOT \
                        considering UE ",__func__);
                node_p = getNextListNode(node_p);
            }
        }
        else
        {
            LTE_LIST_NODE * oldNode_p = PNULL;
            LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,"[%s]"
                    "UE ID %d's Context is NULL or Pending to delele\n",
                    __func__,ueIndex );
            oldNode_p = getNextListNode(node_p);
            
            /*delete the node from non ZERO QUeue Load and Harq List and dont
              consider this UE goto next UE in the non Zero BSR LIST*/
            LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
                    "[%s] UE ID  %d is deleled from the Non Zero List\n",
                    __func__,ueIndex);
            listDeleteNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex], node_p);
            freeMemPool((void *)node_p);
            /* SPR #3953 changes start */
	     if(PNULL != ueDLContextInfo_p->dlUEContext_p)
            {

            ueDLContextInfo_p->dlUEContext_p->dlUENonZeroQueueLoadAndHarqList_p
                                                              = PNULL;
	    }
            /* SPR #3953 changes end */
                      
            node_p= oldNode_p;
        }
    }
    
    return;
}

/********************************************************************************
 * Function Name  : calculateDLWeightedPriority
 * Inputs         : ueDLContext_p - UE Context of the UE for which the Weighted
 *                                  priority needs to be calculated,
 *                  qLoadWeightedMean - Its Mean of Weighted Queue Load of UE,
 *                  dlSchedulerDataNodeArr_p - pointer to node which contains
 *                                             the information related to the
 *                                             scheduling i.e number of RBs 
 *                                             allocated, Weighted Priority, etc
 *                  currentGlobalTTITickCount - Current Gloabal Tick,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Weighted priority of the UE
 * Returns        : weightedPriorityReturned - The Weighted priority of the UE
 *                                             After considering all the factors
 * Description    : This function calculates the weighted priority for
 *                  the UEContext passed as parameter and return the weighted 
 *                  priority.Weighted priority of a UE is calculated
 *                  considering different factors i.e. UE Priority Factor,
 *                  Modulation Factor,Delay Factor and Queue Load factor.
 *****************************************************************************/
UInt32 calculateDLWeightedPriority(
        DLUEContext* ueDLContext_p ,
        UInt32 qLoadWeightedMean_g,
        DLSchedulerDataNodeInfo *dlSchedulerDataNodeArr_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex
        )
{
    UInt32 weightedPriorityReturned  = 0;
    UInt32 modulationScheme          = 0;
    UInt32 fUeQl                     = 0;    /*Queue load factor*/
    UInt32 fUeDelay                  = 0;    /*Delay load factor*/
    UInt32 percentUeQueueLoad        = 0;
    UInt8  j                         = 0;          
    UInt32 weightedQueueLoad         = 0;
    UInt32 sumLCPriority             = 0;
    UInt8  numLCWithNonZeroQueueLoad = 0;
    InitParams *initParams_p = PNULL;
/* SPR 3083 Fix begin */
#ifdef TDD_CONFIG
    UInt8 numOfDLSf;
#endif
/* SPR 3083 Fix end */
    /* Coverity 60232 Fix Start*/
    initParams_p = cellSpecificParams_g.\
                   cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    if(PNULL == initParams_p)
    {
        return 0;
    }
    DLSchedulerStrategyConfig *dlSchdConfig_p = &(initParams_p->dlSchdConfig);
    /* Coverity 60232 Fix End*/

    /************* UE Priority Factor / Delay Factor******************
      UE Priority Factor is only calculated if the UE Priority factor 
      is not calculated before */
    if ( !ueDLContext_p->dlSchedulerAlgoInfo.uePriorityFactor )
    {
        ueDLContext_p->dlSchedulerAlgoInfo.uePriorityFactor = 
          initParams_p->dlSchdConfig.dwlkUEPriorityWt * (
                    MAX_WEIGHTED_UE_PRIORITY -  ueDLContext_p->uePriority);
    }
#ifdef FDD_CONFIG        
    
    /* Delay Factor Calculation */
    fUeDelay = (dlSchdConfig_p->dwlkUEDelayWt) *
                 (currentGlobalTTITickCount - 
                  ueDLContext_p->lastScheduledTick);
/* SPR 3083 Fix begin */
#elif TDD_CONFIG
    numOfDLSf = tddNumOfPDSCH[cellSpecificParams_g.cellConfigAndInitParams_p\
    [internalCellIndex]->cellParams_p->subFrameAssign]
                                      [dataForSpecialSubFrame];    

    /* Delay Factor Calculation considering the number of DL subframes */
    fUeDelay = (dlSchdConfig_p->dwlkUEDelayWt) *
                (((currentGlobalTTITickCount - 
                ueDLContext_p->lastScheduledTick) * numOfDLSf) / 10);
#endif
/* SPR 3083 Fix end */


    /**********************Queue Load Factor***********************
      If the Weighted Queue Load mean is present, Then the UE Queue 
      Load Factor need to be calculated */
    if ( qLoadWeightedMean_g )
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
                "[DL-Strategy][%s]UE ID: %d :: percent Queue Load %u, "
                ,__func__,ueDLContext_p->ueIndex, 
                ((dlSchedulerDataNodeArr_p->weightedQueueLoad * 100)
                 /qLoadWeightedMean_g)
                 );
        if (pfsGlobals_gp[internalCellIndex]->qLoadComputed )
        {
            weightedQueueLoad =
              ueDLContext_p->dlSchedulerAlgoInfo.weightedQueueLoad;
        }
        else
        {
            for (j = 0 ;  j < MAX_NUMBER_OF_LOGICAL_CHANNEL; j++ )
            {
                if ( (ueDLContext_p->logicalChannel[j].logicalChannelId !=
                                                              INVALID_LCID)
                                         && 
                    (ueDLContext_p->logicalChannel[j].queueLoad) )
                {
                    weightedQueueLoad +=  ((MAX_WEIGHTED_LC_PRIORITY -
                    ueDLContext_p->logicalChannel[j].logicalChannelPriority)
                       * (ueDLContext_p->logicalChannel[j].queueLoad));

                    sumLCPriority +=
                    ueDLContext_p->logicalChannel[j].logicalChannelPriority;
                    numLCWithNonZeroQueueLoad++;
                    /* p_lc1 * ql_lc1 + p_lc2 * ql_lc2 of each UE*/
                }
            }
      
            ueDLContext_p->dlSchedulerAlgoInfo.numLCWithNonZeroQueueLoad =
                        numLCWithNonZeroQueueLoad;
            ueDLContext_p->dlSchedulerAlgoInfo.sumLCPriority = sumLCPriority;
      
            ueDLContext_p->dlSchedulerAlgoInfo.weightedQueueLoad =
                                                         weightedQueueLoad;

   
            if( dlSchedulerDataNodeArr_p )
            {
                dlSchedulerDataNodeArr_p->numLCWithNonZeroQueueLoad =
                          numLCWithNonZeroQueueLoad;
                dlSchedulerDataNodeArr_p->sumLCPriority = sumLCPriority ;
                dlSchedulerDataNodeArr_p->weightedQueueLoad = weightedQueueLoad;
            }
        }
               

        percentUeQueueLoad = ((
                    weightedQueueLoad * 100)/qLoadWeightedMean_g);

        if ( percentUeQueueLoad > 1 )
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
                    "[DL-Strategy][%s]Queue Load Factor(>1) is = %f",
                    __func__,(dlSchdConfig_p->dwlkUEQLoadWt)*
                    ((((UDouble32)logint(percentUeQueueLoad)))/1024));

            fUeQl = (dlSchdConfig_p->dwlkUEQLoadWt)* 
                ((((UDouble32)logint(percentUeQueueLoad)))/1024);
            LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
                    "[DL-Strategy][%s]Queue Load Factor(>1) is = %d",
                    __func__,fUeQl) ;
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
                    "[DL-Strategy][%s]Queue Load Factor(<=1) is = %d",
                    __func__,(dlSchdConfig_p->dwlkUEQLoadWt));

            fUeQl = initParams_p->dlSchdConfig.dwlkUEQLoadWt;
        }
    }

    /**********************Modulation Scheme Factor*******************/ 
    /*This getDLModulationScheme will give the Modulation scheme 
      of the UE  */
    /*SPR 6424 Fix Start*/
    modulationScheme = getDLModulationScheme(ueDLContext_p);

    LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,"[%s]UE Index:%d"
            "Modulation Scheme codeword one %d, codeword two %d"
            " Calculated Modulation Scheme:%d\n",
            __func__, ueDLContext_p->ueIndex,
            ueDLContext_p->dlCQIInfo.modulationSchemeCodeWordOne,
            ueDLContext_p->dlCQIInfo.modulationSchemeCodeWordTwo,
            modulationScheme);

    if ( modulationScheme  == QPSK )                       
    {
        ueDLContext_p->dlSchedulerAlgoInfo.modulationSchemeFactor = 
            (dlSchdConfig_p->dwlkMSWt) * 
            (dlSchdConfig_p->dlMsQPSKFactorWt );
    }
    else if ( modulationScheme == SIXTEENQAM )           
    {
        ueDLContext_p->dlSchedulerAlgoInfo.modulationSchemeFactor =
            (dlSchdConfig_p->dwlkMSWt) * 
            (dlSchdConfig_p->dlMs16QAMFactorWt );
    }
    else if ( modulationScheme == SIXFOURQAM )          
    {
        ueDLContext_p->dlSchedulerAlgoInfo.modulationSchemeFactor = 
            (dlSchdConfig_p->dwlkMSWt) * 
            (dlSchdConfig_p->dlMs64QAMFactorWt);
    }
    /*SPR 6424 Fix End*/

    /*****************************************/    
    ueDLContext_p->dlSchedulerAlgoInfo.lastCompWTPriorityTick = 
             currentGlobalTTITickCount;

    LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,"[%s]UE Index:%d,"
            " Current Global tick=%d\n UE Priority:%d,UE Priority Weight:%d"
            " UE Priority Factor:%d\n Last Scheduled Tick:%d,"
            "UE Delay WeightL:%d,UE Delay Factor:%d\n"
            "UE Weighted Queue Load:%d,Queue Load Weight:%d,"
            "Queue Load Factor:%d\n"
            "UE modulation Scheme:%d,MS Weight:%d,Mod Scheme Factor:%d\n"
            "Weighted Priority of UE:%d\n",__func__,ueDLContext_p->ueIndex,
            currentGlobalTTITickCount,ueDLContext_p->uePriority,
            initParams_p->dlSchdConfig.dwlkUEPriorityWt,
            ueDLContext_p->dlSchedulerAlgoInfo.uePriorityFactor,
            ueDLContext_p->lastScheduledTick,dlSchdConfig_p->dwlkUEDelayWt,
            fUeDelay,qLoadWeightedMean_g,dlSchdConfig_p->dwlkUEQLoadWt,fUeQl,
            modulationScheme,dlSchdConfig_p->dwlkMSWt,
            ueDLContext_p->dlSchedulerAlgoInfo.modulationSchemeFactor,
            ueDLContext_p->dlSchedulerAlgoInfo.uePriorityFactor+  
            ueDLContext_p->dlSchedulerAlgoInfo.modulationSchemeFactor+fUeQl 
            +fUeDelay);


    /*****************WEIGHTED PRIORITY CALCULATION************************/    

    /* weighted Priority=Sum Of(uePriorityFactor + modulationSchemeFactor+ 
       QueueLoad Factor + Delay Factor ) */
    weightedPriorityReturned = 
        ueDLContext_p->dlSchedulerAlgoInfo.uePriorityFactor +
        ueDLContext_p->dlSchedulerAlgoInfo.modulationSchemeFactor + 
        fUeQl + 
        fUeDelay ;

    /***********************************************************************/    
    LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
            "[DL-Strategy][%s]UE ID :%d 's Priority:%u QueueLoad:%d"
            "MS:%d UEdelay:%d wtPriority:%d\n"
            ,__func__, ueDLContext_p->ueIndex,
            ((MAX_WEIGHTED_UE_PRIORITY -  ueDLContext_p->uePriority)
             * initParams_p->dlSchdConfig.dwlkUEPriorityWt) ,fUeQl,
            ueDLContext_p->dlSchedulerAlgoInfo.modulationSchemeFactor,fUeDelay,
            weightedPriorityReturned);
    return weightedPriorityReturned;
}


/****************************************************************************
 * Function Name  : calculateDLWeightedQueueLoad
 * Inputs         : UEContext* ueContext_p 
 * Outputs        : weightedQueueLoad - weightedQueueLoad of the UE,
 *                  sumLCPriority - sum of LC priorities of all LC of a UE,
 *                  numLCWithNonZeroQueueLoad - number of LCs with non zero 
 *                                              QueueLoad
 * Returns        : None 
 * Description    : This functions calc weightedQueue load of the UE whose UE
 *                  context is passed.
 ****************************************************************************/
void calculateDLWeightedQueueLoad(
        DLUEContext *ueContext_p,
        UInt32 *weightedQueueLoad)
{
    UInt32 j = 0;
    UInt32 numLCWithNonZeroQueueLoad = 0;
    UInt32 weightedQLoad = 0;
    UInt32 sumLCPriority = 0;
    /* 
    ** Set the ueContextInUse flag to indicate the UE context is in use by
    ** the scheduler 
    */

    for (j = 0 ; j < MAX_NUMBER_OF_LOGICAL_CHANNEL; j++)
    {
        if ( (ueContext_p->logicalChannel[j].logicalChannelId != INVALID_LCID) &&
                (ueContext_p->logicalChannel[j].queueLoad) )
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
                    "[DL-Strategy][%s]UE ID: %u  "
                    "Logical Ch.Priority = %d  Queue Load = %u  \n",__func__,
                    ueContext_p->ueIndex,
                    ueContext_p->logicalChannel[j].logicalChannelPriority,
                    ueContext_p->logicalChannel[j].queueLoad);

            weightedQLoad +=  ((MAX_WEIGHTED_LC_PRIORITY - 
                        ueContext_p->logicalChannel[j].logicalChannelPriority)       
                    * (ueContext_p->logicalChannel[j].queueLoad)); 
            sumLCPriority += ueContext_p->logicalChannel[j].logicalChannelPriority;
            numLCWithNonZeroQueueLoad++;
            /* p_lc1 * ql_lc1 + p_lc2 * ql_lc2 of each UE */
        }
    }
    
    ueContext_p->dlSchedulerAlgoInfo.numLCWithNonZeroQueueLoad 
                                  = numLCWithNonZeroQueueLoad;
    ueContext_p->dlSchedulerAlgoInfo.sumLCPriority = sumLCPriority ;
    ueContext_p->dlSchedulerAlgoInfo.weightedQueueLoad = weightedQLoad;

    *weightedQueueLoad +=weightedQLoad;

    /* Update the weightedQueueLoad calculated in the ueContext */
    LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
            "[DL-Strategy][%s]UE ID: %d :: Weighted Queue Load %u,  "
            "sumLCPriority = %d,  No. Of LC With Zero Queue Load = %u,"
            "\n",
            __func__,ueContext_p->ueIndex,weightedQLoad,sumLCPriority,
            numLCWithNonZeroQueueLoad);
}

/************************************************************************************
 * Function Name  : allocDLRBsToSortedUEs
 * Inputs         : numberOfAvailableRBs_p - Total RBs to be distributed,
 *                  dlScheduleData_p - The array contains weightedPriority and 
 *                                     UE index of all UEs with non zero Queue load,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  dlUserListPFS - Pointer to DLUserListPFS,
 *                  totalUEInDLList - Total UE's in DL List,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : It Sort the UEs depending 
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is used to sorts the  dlScheduleData_p array 
 *                  based on weightedPriority and then assigns RBs based on
 *                  their weightedPriorities.
 ************************************************************************************/
UInt32 allocDLRBsToSortedUEs (
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement *dlScheduleData_p,
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
#endif
        DLUserListPFS *dlUserListPFS,
        UInt32 *totalUEInDLList
        ,InternalCellIndex internalCellIndex
        )
{
    UInt32 rbsAllocated     = 0;

    /* SPR 2261 changes start */
    /* ICIC changes start */
    UInt32 sumWeightedPriority[MAX_USER_LOCATION] = {0,0};
    UInt8 ueLocationType = 0;
    /* ICIC changes end */
    /* SPR 2261 changes end */

    /* ICIC changes start */
    UInt32 ueCount = 0;
    DLSchedulerDataNodeInfo *tempDLSchedulerDataNodeArr_p
        =dlScheduleData_p->dlSchedulerDataNodeArr;
    DLUEContext *ueDLContext_p = PNULL;
    UInt32 listIndex = 0;
    UInt32 initialAvailableRBs = *numberOfAvailableRBs_p;
    UInt32 rbWeightage = 0;
    /* ICIC changes end */
    UInt32 index = 0;

    sumWeightedPriority[CC_USER] = dlScheduleData_p->sumWeightedPriority[CC_USER];
    sumWeightedPriority[CE_USER] = dlScheduleData_p->sumWeightedPriority[CE_USER];


    /* Now iterate through the sorted dlScheduleData_p array and assign
     * RBs to each UE . Here we need to iterate two arrays ,UE with
     * TA array and UE without TA array*/

    ueCount = dlUserListPFS->count;

    while(listIndex < ueCount)
    {
        (*totalUEInDLList)--;
        /*If there are no more RBs available to distribute*/
        if (*numberOfAvailableRBs_p < MIN_RB_REQUIRED_FOR_SCHEDULING)
        {
            while(listIndex < ueCount)

            {
                index = dlUserListPFS->indexToDlSchedulerDataElement[listIndex];
                ueDLContext_p =
                    tempDLSchedulerDataNodeArr_p[index].ueDLContext_p;   
                 /* Cyclomatic_complexity_changes_start */
                pushInPendingQueueAsPerMsgType(
                        dlScheduleData_p->dlSchedulerDataNodeArr[index].msgType,
                        /* SPR 17777 */
                        ueDLContext_p,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
                freeDLHarqProcess(ueDLContext_p, 
                        &ueDLContext_p->dlHarqContext_p->harqProcess
                        [tempDLSchedulerDataNodeArr_p[index].harqProcessId],
                        internalCellIndex);
                ++listIndex;

            }
            return MAC_FAILURE;
        }

        index = dlUserListPFS->indexToDlSchedulerDataElement[listIndex];
        ueDLContext_p =
            tempDLSchedulerDataNodeArr_p[index].ueDLContext_p;   
        ueLocationType = ueDLContext_p->userLocationType;
#ifdef FLOAT_OPERATIONS
        rbsAllocated=
            ceilf_wrapper((((UDouble32)dlScheduleData_p->dlSchedulerDataNodeArr[index].
                            weightedPriority)*(
                                initialAvailableRBs
                                ))/
                    sumWeightedPriority[ueLocationType]);
#else
        rbWeightage = dlScheduleData_p->dlSchedulerDataNodeArr[index].weightedPriority
            * initialAvailableRBs;
        rbsAllocated = rbWeightage / sumWeightedPriority[ueLocationType];

        if (rbWeightage % sumWeightedPriority[ueLocationType])
        { 
            rbsAllocated++;
        }

        if (rbsAllocated < MIN_RB_REQUIRED_FOR_SCHEDULING)
        {
            rbsAllocated = MIN_RB_REQUIRED_FOR_SCHEDULING;
        }
#endif
        /* ICIC changes start */
        if (rbsAllocated > *numberOfAvailableRBs_p)
        {
            if ((*totalUEInDLList == 0) ||
                (tempDLSchedulerDataNodeArr_p[index].msgType == INVALID_TA_DRX_MSG))
            {
                rbsAllocated = *numberOfAvailableRBs_p;
            }
            else if (tempDLSchedulerDataNodeArr_p[index].msgType == TPC_WITHOUT_DATA)
            {
                rbsAllocated = MIN_RB_FOR_TPC_ONLY;
            }
            else if (tempDLSchedulerDataNodeArr_p[index].msgType == TA_DRX_MSG)
            {
                rbsAllocated = MIN_RB_FOR_BOTH_TA_DRX_ONLY;
            }
            else
            {
                rbsAllocated = MIN_RB_FOR_TA_DRX_ONLY;
            }
        }
        /* ICIC changes end */

        /*SPR 2446 Fix Begins*/
        if (!(assignDLRBsAndSendToPDCCH(
                        (&(dlScheduleData_p->dlSchedulerDataNodeArr[index])),
                        rbsAllocated, numberOfAvailableRBs_p
#ifdef TDD_CONFIG
                        ,currentGlobalTTITickCount
#endif
                        ,internalCellIndex
                        )))
            /*SPR 2446 Fix Ends*/
        {
            /* If allocation fails for TA or Explicit DCI message then push entry in TA or
             * Explicit DCI failure Queue*/
             /* Cyclomatic_complexity_changes_start */
            pushInPendingQueueAsPerMsgType(
                    dlScheduleData_p->dlSchedulerDataNodeArr[index].msgType,
                    /* SPR 17777 */
                    ueDLContext_p,
                    internalCellIndex);
             /* Cyclomatic_complexity_changes_end */
        }
        listIndex++;
    }

    return MAC_SUCCESS;
}

/***************************************************************************************
 * Function Name  : assignDLRBsAndSendToPDCCH
 * Inputs         : tempDLSchedulerDataNodeArr_p - pointer to node which contains the
 *                                                 information related to the scheduling
 *                                                 i.e number of RBs allocated, Weighted
 *                                                 Priority, etc
 *                  allocatedRB - The Number of RBs Allocated Orginally,
 *                  availableRB_p - Pointer to the number of Avavailable RBs,
 *                  currentGlobalTTITickCount - Global Tick Count,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : DL Startegy Node to PDCCH Module
 * Returns        : MAC_FAILURE/MAC_SUCCESS
 * Description    : This Function checks whether the UEs is having the 
 *                  Queue Load Avavailable. Depending upon the
 *                  Availability of the either of the Queue Load, and depepnding
 *                  upon the transmission Mode, UE are validated and Allocated
 *                  Finally The DL Strategy Node is been prepared an provided 
 *                  to PDCCH module.
 *****************************************************************************/
/*SPR 2446 Fix Begins*/
MacRetType  assignDLRBsAndSendToPDCCH( 
        DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p,
        UInt32 allocatedRB,
        UInt32 * availableRB_p
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
       ,tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#endif
       ,InternalCellIndex internalCellIndex
        )
/*SPR 2446 Fix Ends*/
{
    /*UInt32 tempTransmissionMode = 0; TDD Warning Fix */
    DLUEContext* ueDLContext_p = tempDLSchedulerDataNodeArr_p->ueDLContext_p;

   /*  tempTransmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;
       TDD warning Fix */
    LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,"[DL-Strategy][%s]"
            "RB Allocated by Fair Scheduler Strategy for UE ID:%d  = %d RBs\n",
            __func__,ueDLContext_p->ueIndex,allocatedRB);
        return processDLFairSchedulerNewDataGeneral(
                    allocatedRB,
                    availableRB_p,ueDLContext_p,
                    tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
		    , currentGlobalTTITickCount 
#endif				    
                    ,internalCellIndex
		    );		    
}

/*************************************************************************************
 * Function Name  : processDLFairSchedulerNewDataGeneral
 * Inputs         : allocatedRB - The Number of RBs Allocated Orginally,
 *                  availableRB_p - Pointer to the number of Avavailable RBs,
 *                  ueDLContext_p - Pointer to UE Context,
 *                  tempDLSchedulerDataNodeArr_p - Pointer to DLSchedulerDataNodeInfo,
 *                  currentGlobalTTITickCount - Current Global TTI Tick,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Process the UE with New Data
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is for processing the UE for New Transmission
 *                  First pick up a fresh harq process and then
 *                  validate the RBs allocated. After which output is prepared
 *                  and sent to PDCCH Module after checking the Transmission Mode
 ************************************************************************************/
/* SPR 5599 changes start (ZIP ID 129096) */
MacRetType processDLFairSchedulerNewDataGeneral(
/* SPR 5599 changes end (ZIP ID 129096) */
        UInt32 allocatedRB,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
	, tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#endif	
        ,InternalCellIndex internalCellIndex
	)
{
    /* + CL-MIMO LJA*/
    /* +  SPR 1476 , 1515*/
    UInt32 tb1only_sendFlag           = 0;
    /* -  SPR 1476 , 1515*/
    /* - CL-MIMO LJA*/    
    UInt32 tempTransmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;
    /* + CQI_5.0 */
    TempStrategyInputInfoForOneTB tempStrategyInputInfoForOneTB = {0};
    UInt8 mcsIndexCW1 = 0;
    UInt8 mcsIndexCW2 = 0;
    UInt8 mcsThreshold = 0;
    /*4x4 DL MIMO CHG START*/
    UInt8 mcsThresholdLayer3 = 0;
    UInt8 mcsThresholdLayer4 = 0;  
    DLHARQProcess * harqProcess_p = PNULL;
    harqProcess_p = &ueDLContext_p->dlHarqContext_p->harqProcess[
          tempDLSchedulerDataNodeArr_p->harqProcessId]; 
    /*4x4 DL MIMO CHG END*/ 
    /* - CQI_5.0 */
    /*Calculating the NET Queue load of the UE, and if that become zero or
      less than return with 0 RBs */

    LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
            "[#############DL-Strategy][Fair-Scheduler]"
            "[%s] UE ID %d (NET_UE_QL %d ] Entry\n",
            __func__,ueDLContext_p->ueIndex, DEFAULT_INT_VALUE );

    /*SPR 6978 Fix Start*/
    if(ueDLContext_p->srbQloadAvailbl == TRUE)
    {
        /*DCI Format Calculation*/
        ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;
        return  processDLNewDataWithOneTBOnly
            ( allocatedRB,availableRB_p,
              ueDLContext_p,
              tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
              , currentGlobalTTITickCount
#endif
              ,internalCellIndex
            );
    }
    else
    {
        /*SPR 6978 Fix End*/
        /*Two TBs can be sent if TX_MODE_3 or TX_MODE_4*/
        if ((TX_MODE_3 == tempTransmissionMode)||
                (TX_MODE_4 == tempTransmissionMode)||
                /* + TM7_8 Changes Start */
                (((TX_MODE_7 == tempTransmissionMode) ||
                  (TX_MODE_8 == tempTransmissionMode)) && 
                 (1 < cellSpecificParams_g.cellConfigAndInitParams_p\
                    [internalCellIndex]->cellParams_p->numOfTxAnteenas)))
            /* - TM7_8 Changes End */
        {
            /* + CQI_5.0 */
            mcsIndexCW1 = ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne;
            mcsIndexCW2 = ueDLContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
	 /* SPR 7981 changes start */	
            mcsThreshold = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                  ->initParams_p->dlSchdConfig.mcsThreshold[ueDLContext_p->userLocationType];
	  /* SPR 7981 changes end */
            /* SPR 7188 Changes Starts */
            if( (RANK_INDICATOR_1 == ueDLContext_p->dlMIMOInfo.dlRi)
                || ( (mcsIndexCW1 <=  mcsThreshold)|| (mcsIndexCW2 <= mcsThreshold)) 
                /* SPR 7188 Changes ends */
               ||  /* + TM7_8 Changes Start */
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
                        internalCellIndex);

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
            /*4x4 DL MIMO CHG START*/
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
            }
            /*4x4 DL MIMO CHG END*/
            
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

            if(tb1only_sendFlag)
            {
                /* -  SPR 1476 , 1515*/
                /*If ( (reported CQI < CQI threshold))
                 *     OR (reported RI is 1)
                 *     )
                 * Then transmit 1 TB only
                 */
                /* SPR 14163 Changes Starts */
                ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;
                /* SPR 14163 Changes Starts */

                return  processDLNewDataWithOneTBOnlyRIOne
                    ( allocatedRB,availableRB_p,
                      ueDLContext_p,
                      tempDLSchedulerDataNodeArr_p
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
                /* SPR 14163 Changes Starts */
                ueDLContext_p->maxTBSize = ueDLContext_p->maxMIMOTBSize;
                /* SPR 14163 Changes Starts */


                return processDLNewDataWithTwoTB(allocatedRB,
                        availableRB_p,ueDLContext_p,
                        tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
                        , currentGlobalTTITickCount
#endif			
                        /*4x4 DL MIMO CHG START*/
                        ,harqProcess_p 
                        /*4x4 DL MIMO CHG END*/ 
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
                /* - TM7_8 Changes End */
                internalCellIndex);
	    tempStrategyInputInfoForOneTB.primaryDCIFormat = ueDLContext_p->tm6DCIFormat;
	    tempStrategyInputInfoForOneTB.secondaryDCIFormat = DCI_FORMAT_1A;

        /* SPR 14163 Changes Starts */
        ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;
        /* SPR 14163 Changes Starts */
            return  processDLNewDataWithOneTBOnlyRIOne
                ( allocatedRB,availableRB_p,
                  ueDLContext_p,
                  tempDLSchedulerDataNodeArr_p
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
            /* SPR 14163 Changes Starts */
            ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;
            /* SPR 14163 Changes Starts */
            return  processDLNewDataWithOneTBOnly
                ( allocatedRB,availableRB_p,
                  ueDLContext_p,
                  tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
                  , currentGlobalTTITickCount
#endif		      
                  ,internalCellIndex
                );
        }
        /*SPR 6978 Fix Start*/
    }
    /*SPR 6978 Fix End*/
}


/*****************************************************************************
 * Function Name  : processDLNewDataWithOneTBOnly
 * Inputs         : allocatedRB - The Number of RBs Allocated Orginally,
 *                  availableRB_p - Pointer to the number of Avavailable RBs,
 *                  ueDLContext_p - Pointer to UE Context,
 *                  dlSchedulerDataNodeArr_p - pointer to node which contains
 *                                             the information related to the
 *                                             scheduling i.e number of RBs
 *                                             allocated, Weighted Priority, etc
 *                  currentGlobalTTITickCount -Current Global TTI Tick,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Process the UE for One TB with New Data
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is for processing the UE for New Transmission 
 *                  For one TB only.First pick up a fresh harq process and then 
 *                  validate the RBs allocated. After which output is prepared 
 *                  and sent to PDCCH Module.
 *****************************************************************************/
MacRetType processDLNewDataWithOneTBOnly(
        UInt32 allocatedRB,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        ,tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix start */
#endif	
        ,InternalCellIndex internalCellIndex
	)
{
    UInt32 usedRB = 0;
    UInt32 tbSize = 0;
    DLHARQProcess * harqProcess_p   = PNULL;
  
    UInt32 minTBS = 0;
    UInt32 maxTBS = 0;
    UInt32 numLCWithNonZeroQueueLoad = 
        tempDLSchedulerDataNodeArr_p->numLCWithNonZeroQueueLoad;
    UInt32 sumLCPriority        = tempDLSchedulerDataNodeArr_p->sumLCPriority;
    UInt32 weightedQueueLoad    = tempDLSchedulerDataNodeArr_p->weightedQueueLoad;
    MsgType msgType             = tempDLSchedulerDataNodeArr_p->msgType;
    ResourceAllocatorInput * tempResourceAllocatorInput_p = PNULL;
    DLStrategyTxNode * dlStrategyTxNode_p=PNULL;

    LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
            "Entry in [DL-Strategy][%s]for UEID %d\n",
            __func__,ueDLContext_p->ueIndex);

#ifdef TDD_CONFIG
    UInt8 subFrame = currentGlobalTTITickCount % MAX_SUBFRAME;
#endif
    
    harqProcess_p = &ueDLContext_p->dlHarqContext_p->harqProcess[
        tempDLSchedulerDataNodeArr_p->harqProcessId];

    /* ICIC changes start*/
    allocatedRB += pfsGlobals_gp[internalCellIndex]->unUsedRBDL;
    /* ICIC changes end*/
    /*Validating the allocated RBs against the Queue Load */
    usedRB = validateRBForOneTB(ueDLContext_p,allocatedRB,
            /* SPR 17777 */
                numLCWithNonZeroQueueLoad,&tbSize,*availableRB_p,msgType,&minTBS,&maxTBS
#ifdef TDD_CONFIG
	       , subFrame
           ,internalCellIndex
#endif	    
           /* SPR 17777 */
	    );

    /*if the used RBs are more than the available RBs, then clip the used RBs
      to the availabale RBs*/
    if (usedRB  > *availableRB_p)
    {
        usedRB =  *availableRB_p ;
    }

    /* ICIC changes start */
    /* Updating unUsedRBDL_g */
    if(allocatedRB > usedRB)
    {
         pfsGlobals_gp[internalCellIndex]->unUsedRBDL = allocatedRB - usedRB;
    }
    else
    {
         pfsGlobals_gp[internalCellIndex]->unUsedRBDL = 0;
    }
    /* ICIC changes end */

    if (usedRB)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
                       "[################DL-Strategy][%s][TB_ONE:NEW DATA],[TB_TWO:INVALID]\n"
                       "UE ID %d Total RBs Allocated after Validate is %d, Total "
                       "Available RBs %d Cell Edge RBs %d\n",__func__,ueDLContext_p->ueIndex,
                       usedRB,*availableRB_p 
                      );
        /*Setting only TBOne as valid and other INVALID*/
        harqProcess_p->isTBOneValid = TRUE;
        harqProcess_p->isTBTwoValid = FALSE;
        harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;

        /*Preparing the DL Strategy Output Node*/
        GET_MEM_FROM_POOL(DLStrategyTxNode,dlStrategyTxNode_p,sizeof(DLStrategyTxNode),PNULL);
	
	/* + Coverity 24522 */
	if( PNULL == dlStrategyTxNode_p )
	{
            freeDLHarqProcess(ueDLContext_p, 
                  &ueDLContext_p->dlHarqContext_p->harqProcess
                        [harqProcess_p->harqProcessId],
                        internalCellIndex);
	    return MAC_FAILURE ;
	}
	/* - Coverity 24522 */

        tempResourceAllocatorInput_p = &(dlStrategyTxNode_p->resourceAllocInput);
        tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
        tempResourceAllocatorInput_p->strictAllocSubBand  = FALSE;
        dlStrategyTxNode_p->isCQIRequestedULGrantMsg      = FALSE;
        /* + CQI_5.0 */
	    dlStrategyTxNode_p->allocPushCount = 0;
        dlStrategyTxNode_p->strictAllocatedFlag = FALSE;
        dlStrategyTxNode_p->minTBSize = minTBS;
        dlStrategyTxNode_p->maxTBSize = maxTBS;
        /* - CQI_5.0 */
        dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;
        dlStrategyTxNode_p->harqProcessId = harqProcess_p->harqProcessId ;
        dlStrategyTxNode_p->transmissionType  = NEW_TX;
        dlStrategyTxNode_p->sumLCPriority     = sumLCPriority ;
        dlStrategyTxNode_p->weightedQueueLoad = weightedQueueLoad;

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

        /*DCI Format Calculation*/
         /*SPR 6978 Fix Start*/
        if(FALSE == ueDLContext_p->srbQloadAvailbl)
        {
            dciFormatSelector (ueDLContext_p,dlStrategyTxNode_p,
                    HARQ_FREE_TX_OTHER_THAN_3_4,internalCellIndex);
        }
        else
        {
            ueDLContext_p->srbQloadAvailbl = FALSE;
            dlStrategyTxNode_p->primaryDCIFormat = DCI_FORMAT_1A;
            dlStrategyTxNode_p->secondaryDCIFormat = DCI_FORMAT_1A;
        }
         /*SPR 6978 Fix End*/

        /*Finding whether Subband Report is available */
        if (ueDLContext_p->dlCQIInfo.isSubBandReportAvailable)
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
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

        tempResourceAllocatorInput_p->requiredRB = usedRB;

        harqProcess_p->assignedResourcesNB = usedRB;

        /*Updating the remaining RB available*/
        /* ICIC changes start */
        *availableRB_p -= tempResourceAllocatorInput_p->requiredRB;
        dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated =
                                          ueDLContext_p->userLocationType;
        /* ICIC changes end */
        /*Updating the ResourceAllocatorInput in the DL Strategy TX Node*/

        /*putEntry of DLStrategyTXNode in PDCCH Queue */

        /* + Coverity 32488 */
        if( MAC_FAILURE == putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex))
	{
         freeDLHarqProcess(ueDLContext_p, 
                  &ueDLContext_p->dlHarqContext_p->harqProcess
                        [harqProcess_p->harqProcessId],
                        internalCellIndex);
         return MAC_FAILURE;
	}
        /* + Coverity 32488 */

        LOG_MAC_MSG (DL_NEWTX_ONETBONLY_UE_SCHEDULED_LOG_ID, LOGDEBUG,
                   MAC_DL_Strategy, getCurrentTick (), ueDLContext_p->ueIndex,
                   dlStrategyTxNode_p->harqProcessId,
                   /*START:6179*/
                   harqProcess_p->assignedResourcesNB,
                   /*END:6179*/
                   ueDLContext_p->ueQueueLoad,
                   dlStrategyTxNode_p->msgType,
                   DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
    }
    else
    {
        /*If the Used RB returns fail, then the Harq Process will be freed*/

        LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
                "\nThe DL HARQ Process ID ::%d  of UEID %d has been succesfully"
                " freed", harqProcess_p->harqProcessId,ueDLContext_p->ueIndex );
        
        /* ICIC changes start*/
        freeDLHarqProcess(ueDLContext_p, 
                  &ueDLContext_p->dlHarqContext_p->harqProcess
                        [harqProcess_p->harqProcessId],
                        internalCellIndex);
        /* ICIC changes end*/

        return MAC_FAILURE;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,"Exit:[DL-Strategy][%s]",
            __func__);
    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : processDLNewDataWithTwoTB
 * Inputs           allocatedRB - The Number of RBs Allocated Orginally,
 *                  availableRB_p - Pointer to the number of Avavailable RBs,
 *                  ueDLContext_p - Pointer to UE Context,
 *                  dlSchedulerDataNodeArr_p - pointer to node which contains
 *                                             the information related to the
 *                                             scheduling i.e number of RBs
 *                                             allocated, Weighted Priority, etc
 *                  currentGlobalTTITickCount -Current Global TTI Tick,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : Process the UE for Two TB with New Data
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is for processing the UE for New Transmission 
 *                  For TWO TBs .First pick up a New harq process and then 
 *                  validate the RBs allocated. After which output is prepared 
 *                  and sent to PDCCH Module. Set the message type Accordingly.
 ********************************************************************************/
MacRetType processDLNewDataWithTwoTB(
        UInt32 allocatedRB,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        , tickType_t currentGlobalTTITickCount
         /* SPR 15909 fix end */
#endif
        /*4x4 DL MIMO CHG START*/
        ,DLHARQProcess* harqProcess_p
        /*4x4 DL MIMO CHG END*/
        ,InternalCellIndex internalCellIndex
        )     
{
    UInt32 usedRB       = 0;
    UInt32 tbOneSize    = 0;
    UInt32 tbTwoSize    = 0;
    UInt32 minTBS = 0;
    UInt32 maxTBS = 0;


    UInt32 numLCWithNonZeroQueueLoad = 
        tempDLSchedulerDataNodeArr_p->numLCWithNonZeroQueueLoad;
    UInt32 sumLCPriority        = tempDLSchedulerDataNodeArr_p->sumLCPriority;
    UInt32 weightedQueueLoad    = tempDLSchedulerDataNodeArr_p->weightedQueueLoad;
    MsgType msgType             = tempDLSchedulerDataNodeArr_p->msgType;
    ResourceAllocatorInput * tempResourceAllocatorInput_p   = PNULL;
    DLStrategyTxNode * dlStrategyTxNode_p                   = PNULL;
    /* + CQI_5.0 */
    UInt8 transmissionMode = ueDLContext_p->dlMIMOInfo.transmissionMode;
    /* - CQI_5.0 */

#ifdef TDD_CONFIG
    UInt8 subFrame = currentGlobalTTITickCount % MAX_SUBFRAME;
#endif
  /* klockwork warning fix start */  

     if ( transmissionMode >= INVALID_TX_MODE )
     {
            return MAC_FAILURE ;
     }       
  /* klockwork warning fix end */   
     /*Finding the free Harq process for New Transmission*/
    /*if (PNULL == (harqProcess_p = checkForFreeHarqProcess
                (ueDLContext_p, ueDLContext_p->dlHarqContext_p, 
                DRA_SCHEDULING, internalCellIndex)))
    {
        LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
                "[DL-Strategy][%s]No free Harq process for ueid %d\n",
                __func__,ueDLContext_p->ueIndex);
        return MAC_FAILURE ;
    }*/

    /* ICIC changes start*/
     allocatedRB += pfsGlobals_gp[internalCellIndex]->unUsedRBDL;
    /* ICIC changes end */
    /*Validating the allocated RBS against the Queue Load considering TWO TBs*/
    /*4x4 DL MIMO CHG START*/
    if ((SInt64)(tbOneSize = determineTBSize(ueDLContext_p->dlCQIInfo.mcsIndexCodeWordOne,
                               allocatedRB,TB_MAPPED_TO_ONE_LAYER)) < NET_UE_QL(ueDLContext_p))
    /*4x4 DL MIMO CHG END*/
    {
        usedRB = validateRBForTwoTB(ueDLContext_p,allocatedRB,
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
        usedRB = validateRBForOneTB(ueDLContext_p, allocatedRB,
                numLCWithNonZeroQueueLoad, &tbOneSize,
                /* SPR 17777 */
                *availableRB_p, msgType,&minTBS,&maxTBS
#ifdef TDD_CONFIG
                , subFrame
                ,internalCellIndex
#endif
                /* SPR 17777 */
                );
    }

    
     /*if the used RBs are more than the available RBs, then clip the used RBs
       to the availabale RBs*/
    if (usedRB  > *availableRB_p)
    {
        usedRB =  *availableRB_p;
    }

    /*ICIC changes start*/
    /* Updating unUsedRBDL_g */
    if(allocatedRB > usedRB)
    {
        pfsGlobals_gp[internalCellIndex]->unUsedRBDL = allocatedRB - usedRB;
    }
    else
    {
        pfsGlobals_gp[internalCellIndex]->unUsedRBDL = 0;
    }
    /* ICIC changes end */

    if (usedRB)
    {
        GET_MEM_FROM_POOL(DLStrategyTxNode,dlStrategyTxNode_p,sizeof(DLStrategyTxNode),PNULL);
	
	/* + Coverity 24524 */
	if( PNULL == dlStrategyTxNode_p )
	{
            freeDLHarqProcess(ueDLContext_p, 
                  &ueDLContext_p->dlHarqContext_p->harqProcess
                        [harqProcess_p->harqProcessId],
                        internalCellIndex);
	    return MAC_FAILURE ;
	}
	/* - Coverity 24524 */

        LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
                "[DL-Strategy][%s][TB_ONE:NEW DATA],[TB_TWO:NEW DATA]\n"
                "validateRBForTwoTB returned %d RBs for UEID %d ,"
                " tbOneSize:%d,tbTwoSize:%d",__func__,
                usedRB,ueDLContext_p->ueIndex,tbOneSize,tbTwoSize );

        /* Setting  both TBOne and TBTwo as valid */
        harqProcess_p->isTBOneValid = TRUE;
        harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;

        if (tbTwoSize)        
        {
            harqProcess_p->isTBTwoValid = TRUE;
            harqProcess_p->dlHARQTBTwoInfo.txIndicator = NEW_TX;
            /*DCI Format Calculation*/
            /* + TM7_8 Changes Start */
            dciFormatSelector (ueDLContext_p,dlStrategyTxNode_p,
                    HARQ_FREE_TX_3_4_8,internalCellIndex);
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
                /** klockwork Changes End  **/
            }
        }

        dlStrategyTxNode_p->msgType = SPATIAL_MUL;
            /* - CQI_5.0 */
        /*Preparing the DL Strategy Output Node*/
        tempResourceAllocatorInput_p = &(dlStrategyTxNode_p->resourceAllocInput);
        tempResourceAllocatorInput_p->subBandReportInfo_p = PNULL;
        tempResourceAllocatorInput_p->strictAllocSubBand = FALSE;
        dlStrategyTxNode_p->isCQIRequestedULGrantMsg = FALSE;
        /* + CQI_5.0 */
        tempResourceAllocatorInput_p->subBandPMIInfo_p = PNULL;
        tempResourceAllocatorInput_p->subbandPMI = FALSE;
        tempResourceAllocatorInput_p->aperiodicMode12 = FALSE;        
	    dlStrategyTxNode_p->allocPushCount = 0;
        dlStrategyTxNode_p->strictAllocatedFlag = FALSE;
        dlStrategyTxNode_p->minTBSize = minTBS;
        dlStrategyTxNode_p->maxTBSize = maxTBS;
        /* - CQI_5.0 */

        dlStrategyTxNode_p->ueIndex = ueDLContext_p->ueIndex;
        dlStrategyTxNode_p->harqProcessId = harqProcess_p-> harqProcessId ;
        dlStrategyTxNode_p->transmissionType = NEW_TX;

        /*Setting the message Type accoding to the input and change the same
          according to the Transmission . Setting the TA DRX Fields as well   */
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

        /*Finding whether Subband Report is available */
        if (ueDLContext_p->dlCQIInfo.isSubBandReportAvailable)
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,DL_PF_STRATEGY,
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

        tempResourceAllocatorInput_p->requiredRB = usedRB;
        harqProcess_p->assignedResourcesNB = usedRB;

        /*Updating the remaining RB available*/
            /* ICIC changes start */
            *availableRB_p -= tempResourceAllocatorInput_p->requiredRB;
            dlStrategyTxNode_p->resourceAllocInput.resourceRegionAllocated =
                ueDLContext_p->userLocationType;
            /* ICIC changes end */

        dlStrategyTxNode_p->sumLCPriority = sumLCPriority;
        dlStrategyTxNode_p->weightedQueueLoad = weightedQueueLoad;

        /*putEntry of DLStrategyTXNode in PDCCH Queue */
        /* + Coverity 32490 */
        if( MAC_FAILURE == putEntryInDLSchedulerNewTxQueue(dlStrategyTxNode_p,internalCellIndex))
	{
         freeDLHarqProcess(ueDLContext_p, 
                  &ueDLContext_p->dlHarqContext_p->harqProcess
                        [harqProcess_p->harqProcessId],
                        internalCellIndex);
         return MAC_FAILURE;
	}
        /* + Coverity 32490 */

        LOG_MAC_MSG (DL_NEWTX_TWOTBONLY_UE_SCHEDULED_LOG_ID, LOGDEBUG,
                    MAC_DL_Strategy, getCurrentTick (), ueDLContext_p->ueIndex,
                    dlStrategyTxNode_p->harqProcessId,
                    /* SPR 5147 changes start */
                    harqProcess_p->assignedResourcesNB,
                    /* SPR 5147 changes end */
                    ueDLContext_p->ueQueueLoad,
                    dlStrategyTxNode_p->msgType,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");

    }
    else
    {
        /*If the Used RB returns fail, then the Harq Process will be freed*/
         LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,
                 "\nThe DL HARQ Process ID ::%d  of UEID %d has been succesfully"
                 " freed", harqProcess_p->harqProcessId,ueDLContext_p->ueIndex );
         /* ICIC changes start*/
         freeDLHarqProcess(ueDLContext_p, 
                  &ueDLContext_p->dlHarqContext_p->harqProcess
                        [harqProcess_p->harqProcessId],
                        internalCellIndex);
         /* ICIC changes end*/
         return MAC_FAILURE;
    }
    LTE_MAC_UT_LOG(LOG_INFO,DL_PF_STRATEGY,"Exit:[DL-Strategy][%s]",__func__);
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : dlSwap
 * Inputs         : data - scheduler Data, 
 *                  dex1 - element one,
 *                  dex2 - element two
 * Outputs        : Swap of the two Elements
 * Returns        : None
 * Description    : swaps two elements.
 *****************************************************************************/
void dlSwap(
        DLSchedulerDataNodeInfo data[],
        UInt32 dex1, 
        UInt32 dex2)
{
    DLSchedulerDataNodeInfo temp = data[dex1];
    data[dex1]= data[dex2];
    data[dex2]= temp;
}

/*****************************************************************************
 * Function Name  : dlMedianOf3
 * Inputs         : data - array to be sorted,
 *                  left - start index of array to be sorted,
 *                  right - end index of array to be sorted
 * Outputs        : It calculates means value
 * Returns        : median value
 * Description    : Called by dlRecQuickSort to select a pivot element.
 *****************************************************************************/
UInt32 dlMedianOf3(
        DLSchedulerDataNodeInfo data[],
        UInt32 left, 
        UInt32 right) 
{
    UInt32 center = (left + right) / 2;
    /* order left & center*/
    if (data[left].weightedPriority < data[center].weightedPriority)
        dlSwap(data,left, center);
    /* order left & right*/
    if (data[left].weightedPriority < data[right].weightedPriority)
        dlSwap(data,left, right);
    /* order center & right*/
    if (data[center].weightedPriority < data[right].weightedPriority)
        dlSwap(data,center, right);

    /*put pivot on right */
    dlSwap(data,center, right - 1);
    /* return median value*/
    return data[right - 1].weightedPriority;
}


/*****************************************************************************
 * Function Name  : dlPartitionIt
 * Inputs         : data - array to be sorted,
 *                  left - start index of array to be sorted,
 *                  right - end index of array to be sorted
 * Outputs        : Finding out the Pivot position
 * Returns        : pivot location
 * Description    : Called by dlRecQuickSort to fix position of one element.
 *****************************************************************************/
UInt32 dlPartitionIt(
        DLSchedulerDataNodeInfo data[],
        UInt32 left, 
        UInt32 right, 
        UInt32 pivot)
{
    /*right of first element*/
    UInt32 leftPtr = left; 
    /*left of pivot */
    UInt32 rightPtr = right - 1;

    while (1) 
    {
        /* finding  bigger */
        while (leftPtr <= right && data[++leftPtr].weightedPriority > pivot)
            ;
        /* finding smaller*/
        while (rightPtr >= left && data[--rightPtr].weightedPriority < pivot)
            ;
        /*if pointers cross, partition done */
        if (leftPtr >= rightPtr) 
        {
            break;    
        }
        else
        {
            /*If NOT Crosses then dlSwap elements */
            dlSwap(data,leftPtr, rightPtr);
        }
    }
    /*restore pivot */
    dlSwap(data,leftPtr, right - 1);
    /*return pivot location */
    return leftPtr;
}


/*****************************************************************************
 * Function Name  : dlManualSort
 * Inputs         : data - array to be sorted,
 *                  left - start index of array to be sorted,
 *                  right - end index of array to be sorted
 * Outputs        : Sorts the input array
 * Returns        : None
 * Description    : It does the sorting of the array from the start and and of 
 *                   array index.
 *****************************************************************************/
void dlManualSort(
        DLSchedulerDataNodeInfo data[],
        UInt32 left, 
        UInt32 right)
{
    /*Calculating the size to be sort*/
    UInt32 size = right - left + 1;

    /*if Size is equal to or Less than 1, then no sort  is necessary */
    if (size <= 1)
    {
        return;
    }
    /**if Size is equal to 2, then sort left and right*/
    if (size == 2) 
    {
        if (data[left].weightedPriority < data[right].weightedPriority)
            dlSwap(data,left, right);
        return;
    }
    /*if Size is equal to 3,then sort left, center, & right */
    else 
    { 
         /*left, center*/
        if (data[left].weightedPriority < data[right - 1].weightedPriority)
            dlSwap(data,left, right - 1);
        /* left, right */
        if (data[left].weightedPriority < data[right].weightedPriority)
            dlSwap(data,left, right);
        /*center, right */
        if (data[right - 1].weightedPriority < data[right].weightedPriority)
            dlSwap(data,right - 1, right);
    }
}

/*****************************************************************************
 * Function Name  : getDLModulationScheme
 * Inputs         : ueDLContext_p:Pointer to UE Context
 * Outputs        : gives modulation Scheme
 * Returns        : Modulation Scheme
 * Description    : It gives the modulation scheme value.
 *****************************************************************************/
UInt32 getDLModulationScheme (
        DLUEContext * ueDLContext_p)
{
    return (ueDLContext_p->dlCQIInfo.modulationSchemeCodeWordOne);
}

/********************************************************************************
 * Function Name  : dlPfsCheckIfNodetoBeScheduled
 * Inputs         : node_p - Pointer to LTE_LIST_NODE,
 *                  ueDLContext_p - Pointer to DLUEContext,
 *                  maxUeToBeSorted - Max UE's to be sorted,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This func prepares a list with first n elements as
 *                  the top n. n is same as the max UEs to be scheduled.
 *                  The min of top n is tracked. Any new element is checked
 *                  else ignore. This further optimizes sorting of top n. Note
 *                  that only CE UEs having highest Priorities and equal to 
 *                  maxCellEdgeUEsToBeScheduledDL_g will be present in this list.
 ********************************************************************************/
static void  dlPfsCheckIfNodetoBeScheduled(
                            LTE_LIST_NODE *node_p, 
                                  //node of dlUENonZeroQueueLoadAndHarqList
                            DLUEContext  *ueDLContext_p,
                            UInt8  maxUeToBeSorted,
                            InternalCellIndex internalCellIndex)
{
    /* ICIC changes start */
    SortNodeT    *pSortNodeT = PNULL;
    SortNodeT    *pSortNodeAdd = PNULL;
    UInt32        weightedPriority;
    /* Flag indicating if additional node added in list needs to be deleted */
    UInt8 additionalNodeToBeDeleted = FALSE;
    /* Pointer to store the location of least Priority UE in the list */
    pSortNodeT = leastPriorityUe_p[internalCellIndex];
    UInt16 counter = listCount(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList));
    /* 
    ** If the incoming UE is CE and no CE UE is allowed in the List,
    ** skip the CE UE.
    */
    if ( (ueDLContext_p->userLocationType == CE_USER) 
          && (!maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]) 
          && (!leastPriorityCEUe_p[internalCellIndex]) )
    {
        /* There is no need to process CE UE */
        return;
    }
    else
    {
        /* 
        ** Add NUM_BUFFER_UES_DL Ues to maxUeToBeSorted to hold two extra 
        ** Ues in lastTickScheduledList_g List. This is done to 
        ** avoid less UE scheduling.
        */
        maxUeToBeSorted = maxUeToBeSorted + NUM_BUFFER_UES_DL; 
        weightedPriority = ueDLContext_p->dlSchedulerAlgoInfo.weightedPriority;
        /* If new node is smaller than smallest of n, then ignore */
        if ( counter < maxUeToBeSorted )
        {
            pSortNodeAdd = dlPfsAllocSortNodeElem(internalCellIndex);
            pSortNodeAdd->weightedPriority = weightedPriority;
            pSortNodeAdd->ueIndex = 
                    ((NonZeroQueueLoadAndHarqNode*)node_p)->ueIndex;
            /* ICIC changes start */
            /* 
            ** A new parameter userLocationType is added to store
            ** location Type of UE
            */
            pSortNodeAdd->userLocationType = ueDLContext_p->userLocationType;
            /* ICIC Changes end */

            if ( !leastPriorityUe_p[internalCellIndex] )
            {
                listInsertNodeAtHead(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList), 
                                     (LTE_LIST_NODE *)pSortNodeAdd);
                leastPriorityUe_p[internalCellIndex] = pSortNodeAdd;

                /* 
                ** If first incoming UE is CE, its address is stored.
                ** Scenario: Now if another CE UE is arrived which has
                ** more Priority then the already existing CE UE in list and 
                ** maxCellEdgeUEsToBeScheduledDL_g = 0,then this CE UE is
                ** placed according to its Priority weightage while the CE UE
                ** having priority more than the lowest priority CE UE will be
                ** marked as lowest Priority CE UE and earlier lowest priority
                ** CE UE in list will be deleted.
                */
                if ( (ueDLContext_p->userLocationType == CE_USER )
                /*+++530-coverity-32866*/
                   &&(maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex])) 
                /*+++530-coverity-32866*/
                {
                    leastPriorityCEUe_p[internalCellIndex] = pSortNodeAdd;
                    maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;
                }
                return;
            }

            if ( weightedPriority > leastPriorityUe_p[internalCellIndex]->weightedPriority )
            {
                /* Check the place of insertion of the Node */
                while ( (pSortNodeT = 
                  (SortNodeT  *)getPrevListNode((LTE_LIST_NODE *)pSortNodeT)) )
                {
                    if ( pSortNodeT->weightedPriority >= weightedPriority )
                    {
                        if ( ueDLContext_p->userLocationType == CE_USER ) 
                        {
                            if ( maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] )
                            {
                                /* Insert CE UE in List */
                                dlPfsInsertCENodeInList(
                                              (LTE_LIST_NODE *) pSortNodeT,
                                              pSortNodeAdd,
                                               internalCellIndex);
                            }
                            else
                            {
                                /* Replace CE UE in List */
                                dlPfsReplaceCENodeInList(
                                              pSortNodeT,
                                              pSortNodeAdd
                                              ,internalCellIndex
                                        );
                            }
                        }
                        else
                        {
                            listInsertNodeAfter(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                                               (LTE_LIST_NODE *) pSortNodeT,
                                               (LTE_LIST_NODE *) pSortNodeAdd);
                        }
                        break;
                    }
                }

                if ( PNULL == pSortNodeT )
                {
                    /* Insert Node at head of List */
                    dlPfsInsertNodeAtHead(pSortNodeAdd,
                                          ueDLContext_p,
                            &additionalNodeToBeDeleted,
                            internalCellIndex
                            );
                }
            }
            else
            {

                /* Since List has space to accomodate UEs, flag will be ZERO */
                additionalNodeToBeDeleted = FALSE;
                /* 
                ** If the incoming lower priority UE is CE then some calc.
                ** is required as mentioned below Else CC UE can be added in list
                ** & pointer pointing to lowest priority UE is adjusted accordingly
                */
                if ( ueDLContext_p->userLocationType == CE_USER )
                {
                    /* 
                    ** If new UE is CE, it is added in list and pointers
                    ** pointing to lowest CE and CC UEs will point to this CE UE
                    */
                    if ( maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] )
                    {
                        listInsertNodeAfter(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList) ,
                                            (LTE_LIST_NODE *) pSortNodeT,
                                            (LTE_LIST_NODE *) pSortNodeAdd);
                        leastPriorityCEUe_p[internalCellIndex] = pSortNodeAdd;
                        leastPriorityUe_p[internalCellIndex] = pSortNodeAdd;
                        maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;
                    }
                    else
                    {
                        /*
                        ** If the new UE is CE and has lower priority than the
                        ** lowest priority UE of the existing list then there 
                        ** is no need to add it in the list as 
                        ** maxCellEdgeUEsToBeScheduledDL_g = 0.
                        */
                        dlPfsFreeSortNumElem(pSortNodeAdd,internalCellIndex);
                    }
                }
                else
                {
                    listInsertNodeAfter(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList) ,
                          (LTE_LIST_NODE *)pSortNodeT, (LTE_LIST_NODE *) pSortNodeAdd);
                    leastPriorityUe_p[internalCellIndex] = pSortNodeAdd;
                }
            }
        }
        else
        {
            if ( weightedPriority > leastPriorityUe_p[internalCellIndex]->weightedPriority )
            {
                pSortNodeAdd = dlPfsAllocSortNodeElem(internalCellIndex);
                pSortNodeAdd->weightedPriority = weightedPriority;
                pSortNodeAdd->ueIndex = ((NonZeroQueueLoadAndHarqNode*)node_p)->ueIndex;
                /* ICIC changes start */
                /* 
                ** A new parameter userLocationType is added to store
                ** location Type of UE 
                */
                pSortNodeAdd->userLocationType = ueDLContext_p->userLocationType;
                /* ICIC Changes end */

                /* 
                ** Keep track of smallest of top n - so that new nodes are compared with
                ** smallest only.
                */

                while ( (pSortNodeT = 
                   (SortNodeT  *)getPrevListNode((LTE_LIST_NODE *)pSortNodeT)) )
                {
                    if ( pSortNodeT->weightedPriority >= weightedPriority )
                    {
                        if ( ueDLContext_p->userLocationType == CE_USER ) 
                        {
                            if ( maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] )
                            {
                                /* Insert CE UE in List */
                                dlPfsInsertCENodeInList(
                                              (LTE_LIST_NODE *) pSortNodeT,
                                              pSortNodeAdd,
                                               internalCellIndex);
                                additionalNodeToBeDeleted = TRUE;
                            }
                            else
                            {
                                /* Replace CE UE in List */
                                dlPfsReplaceCENodeInList(
                                              pSortNodeT,
                                              pSortNodeAdd,
                                              internalCellIndex
                                        );
                                additionalNodeToBeDeleted = FALSE;
                            }
                        }
                        else
                        {
                            listInsertNodeAfter(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                                                (LTE_LIST_NODE *) pSortNodeT,
                                                (LTE_LIST_NODE *) pSortNodeAdd);
                            additionalNodeToBeDeleted = TRUE;
                        }
                        break;
                    }
                }

                if ( PNULL == pSortNodeT )
                {
                    additionalNodeToBeDeleted = TRUE;
                    /* Insert Node at head of List */
                    dlPfsInsertNodeAtHead(pSortNodeAdd,
                                          ueDLContext_p,
                            &additionalNodeToBeDeleted,
                            internalCellIndex
                            );
                }
            }
        }
    
        if ( additionalNodeToBeDeleted )
        {
            leastPriorityUe_p[internalCellIndex] = 
             (SortNodeT *) getLastListNode(( LTE_LIST *)&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList));
            listDeleteNode(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                           (LTE_LIST_NODE *) leastPriorityUe_p[internalCellIndex]);
            dlPfsFreeSortNumElem(leastPriorityUe_p[internalCellIndex],internalCellIndex);
            leastPriorityUe_p[internalCellIndex] = 
                        (SortNodeT *) getLastListNode(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList));
        }
    }
    /* ICIC changes end */
}

/*****************************************************************************
 * Function Name  : dlPfsCheckRemainingNodes
 * Inputs         : node_p - Pointer to LTE_LIST_NODE,
 *                  maxUEsToBeScheduledDL - Maximum nnumber of UE to be Scheduled
 *                                          in Downlink,
 *                  dlReferenceFlag - used for checking whether the UE is 
 *                                    considered already or not. If UE's 
 *                                    isAlreadyConsideredFlag is equal to
 *                                    dlReferenceFlag, that means the UE is
 *                                    already considered,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : dlUENonZeroQueueLoadAndHarqList is parsed to identify UEs to be
 *                  computed, the nodes are also checked against top n and 
 *                  sorted if they are in top n. The below function runs through 
 *                  the remaining nodes of dlUENonZeroQueueLoadAndHarqList to 
 *                  complete finding top n out of all UEs.
 ******************************************************************************/
static void dlPfsCheckRemainingNodes(
              LTE_LIST_NODE *node_p, //node of dlUENonZeroQueueLoadAndHarqList
              UInt8  maxUEsToBeScheduledDL, 
        /*SPR 21068 start */
        tickType_t dlReferenceFlag,
        /*SPR 21068 end   */
        InternalCellIndex internalCellIndex
        )
{
	DLUEContext  *ueDLContext_p = PNULL;
	DLUEContextInfo * ueDLContextInfo_p = PNULL;
	while (node_p )
	{
		ueDLContextInfo_p = &dlUECtxInfoArr_g[\
				    ((NonZeroQueueLoadAndHarqNode*)node_p)->ueIndex];
        ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
		if ( !(ueDLContextInfo_p->pendingDeleteFlag) )
		{

			if ( ueDLContext_p->isAlreadyConsideredFlag != dlReferenceFlag )
			{
				/* ICIC changes start */
				dlPfsCheckIfNodetoBeScheduled(node_p,
						ueDLContext_p,
                        maxUEsToBeScheduledDL,
                        internalCellIndex);
				/* ICIC changes end */
			}
			node_p = getNextListNode(node_p);
		}
		else
        {
            LTE_LIST_NODE * oldNode_p = PNULL;
            oldNode_p = getNextListNode(node_p);

            listDeleteNode(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex], node_p);
            freeMemPool((void *)node_p);
            /* Node is getting deleted, so mark dlUENonZeroQueueLoadAndHarqList_p
               NULL in UE context*/
            if(ueDLContext_p)
            {
                ueDLContext_p->dlUENonZeroQueueLoadAndHarqList_p = PNULL;
            }
            node_p= oldNode_p;

        }

	}
}

/*****************************************************************************
 * Function Name  : dlPfsInitMacSchedulerData
 * Inputs         : internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to initialise the data structure
 *                  used in LTE MAC DL Fair Scheduler.
 *****************************************************************************/
void dlPfsInitMacSchedulerData(UInt8 internalCellIndex)
{
    listInit(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList));
    dlPfsInitMemAllocElem(internalCellIndex);
}
/*****************************************************************************
 * Function Name  : dlPfsInitMemAllocElem
 * Inputs         : internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to initialise the
 *                  dlSortNodeMemAllocElem_g for memory allocation.
 ******************************************************************************/
static void dlPfsInitMemAllocElem(InternalCellIndex internalCellIndex)
{
    UInt16 idx = 0;
    pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.freeHeadIdx = 0;
    for (idx = 0 ; idx < NUM_UES_COMPUTED_ONE_TICK ; idx++ )
    {
        pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.sortListNode[idx].ueIndex = 0xFFFF;
        pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.sortListNode[idx].weightedPriority = 0;
        pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.sortListNode[idx].Idx =  idx;
        pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.sortListNode[idx].nxtIdx =  idx + 1 ;
        if ( NUM_UES_COMPUTED_ONE_TICK  == (idx + 1) )
        {
           pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.sortListNode[idx].nxtIdx = 0XFFFF;
        }
    }

}

/*****************************************************************************
 * Function Name  : dlPfsAllocSortNodeElem
 * Inputs         : internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : SortNodeT 
 * Description    : This function is used for memory allocation from Array.
 ******************************************************************************/
static SortNodeT *dlPfsAllocSortNodeElem(InternalCellIndex internalCellIndex)
{
    UInt8 freeHeadIndex  = pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.freeHeadIdx;
    pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.freeHeadIdx =
    pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.sortListNode
        [freeHeadIndex].nxtIdx;

    return &(pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.
        sortListNode[freeHeadIndex]);
}

/*****************************************************************************
 * Function Name  : dlPfsFreeSortNumElem
 * Inputs         : pSortNode - Pointer to SortNodeT,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to free the memory of nodes of Array
 *                  and Harq processes.
 *****************************************************************************/
static void dlPfsFreeSortNumElem (SortNodeT *pSortNode,InternalCellIndex internalCellIndex)
{
    pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.sortListNode
        [pSortNode->Idx].nxtIdx =
            pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.freeHeadIdx;
    pfsGlobals_gp[internalCellIndex]->dlSortNodeMemAllocElem.freeHeadIdx = pSortNode->Idx;
}

/* ICIC changes start */
/*********************************************************************************
 * Function Name  : dlPfsInsertCENodeInList
 * Inputs         : pSortNodeT - node in list after which CE Node is to be added,
 *                  pSortNodeAdd - new CE Node to be added,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function inserts CE Node after a Node in linked List.
 *********************************************************************************/
static void dlPfsInsertCENodeInList(
                      LTE_LIST_NODE *pSortNodeT,
                      SortNodeT *pSortNodeAdd,
                      InternalCellIndex internalCellIndex)
{
    listInsertNodeAfter((&pfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                        (LTE_LIST_NODE *) pSortNodeT, 
                        (LTE_LIST_NODE *) pSortNodeAdd);

    /* Make this as least CE UE if it is first one */
    if ( !leastPriorityCEUe_p[internalCellIndex] )
    {
        leastPriorityCEUe_p[internalCellIndex] = pSortNodeAdd;    
    }
    /* 
    ** Change the pointer to lowest Priority CE UE to this new
    ** UE as it has lower Priority then already existing 
    ** Lowest Priority CE UE. 
    */
    else if ( leastPriorityCEUe_p[internalCellIndex]->weightedPriority > 
                              pSortNodeAdd->weightedPriority )
    {
        leastPriorityCEUe_p[internalCellIndex] = pSortNodeAdd;
    }

    maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex]--;
}
/* ICIC changes end */

/* ICIC changes start */
/*********************************************************************************
 * Function Name  : dlPfsReplaceCENodeInList
 * Inputs         : pSortNodeT - node in list after which CE Node is to be added,
 *                  pSortNodeAdd - new CE Node to be added,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function replace CE Node in linked List. It adds new
 *                  high priority CE node and remove low Priority CE node 
 *                  from the list.
 *********************************************************************************/
static void dlPfsReplaceCENodeInList(
                      SortNodeT *pSortNodeT,
        SortNodeT *pSortNodeAdd
        ,InternalCellIndex internalCellIndex)
{
    SortNodeT    *lowestPriorityCEUe_p = PNULL;
    /* 
    ** If the new UE is CE having less priority than
    ** the least Priority CE UE and there is no space
    ** to accomodate CE UE anymore it is discarded
    */
    if ( leastPriorityCEUe_p[internalCellIndex]->weightedPriority >= 
                           pSortNodeAdd->weightedPriority )
    {
        /* Discard this CE UE as it has low priority */
        dlPfsFreeSortNumElem(pSortNodeAdd,internalCellIndex);
    }
    else
    {
        /* 
        ** Since this CE UE has high priority than 
        ** lowest Priority CE UE, this has to be
        ** added in list while least Priority 
        ** CE UE has to be deleted
        */
        listInsertNodeAfter(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                        (LTE_LIST_NODE *) pSortNodeT, 
                        (LTE_LIST_NODE *) pSortNodeAdd);
        lowestPriorityCEUe_p = leastPriorityCEUe_p[internalCellIndex];
    
        /* 
        ** Now search for next lowest priority CE UE
        ** before deleting this lowest priority CE UE
        */
        pSortNodeT = leastPriorityCEUe_p[internalCellIndex];
 
        if (leastPriorityCEUe_p[internalCellIndex] == leastPriorityUe_p[internalCellIndex])
        {
            leastPriorityUe_p[internalCellIndex] = (SortNodeT *) leastPriorityUe_p[internalCellIndex]->node.previous;     
        }

        while ( (pSortNodeT = 
                (SortNodeT *)getPrevListNode((LTE_LIST_NODE *)pSortNodeT)))
        {
            if ( pSortNodeT->userLocationType == CE_USER )
            {
                leastPriorityCEUe_p[internalCellIndex] = pSortNodeT;
                break;
            }
        }
        listDeleteNode(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList), 
                       (LTE_LIST_NODE *)lowestPriorityCEUe_p);
        dlPfsFreeSortNumElem(lowestPriorityCEUe_p,internalCellIndex);
    }
}
/* ICIC changes end */

/* ICIC changes start */
/*********************************************************************************
 * Function Name  : dlPfsInsertNodeAtHead
 * Inputs         : pSortNodeAdd - new CE Node to be added,
 *                  ueDLContext_p - Pointer to DLUEContext,
 *                  additionalNodeToBeDeleted_p - flag indicating additional node,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : additionalNodeToBeDeleted_p - flag indicating additional node
 *                                                in the list to be deleted or not
 * Returns        : None
 * Description    : This function adds new Node at the head of linked List.
 *                  It also returns a flag which indicates whether additional 
 *                  node from the list needs to be deleted or not.
 **********************************************************************************/
static void dlPfsInsertNodeAtHead(
                      SortNodeT *pSortNodeAdd,
                      DLUEContext  *ueDLContext_p,
                      UInt8 *additionalNodeToBeDeleted_p,
                      InternalCellIndex internalCellIndex)
{
    SortNodeT    *lowestPriorityCEUe_p = PNULL;
    SortNodeT    *pSortNodeT = PNULL;
    listInsertNodeAtHead(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList) ,
                         (LTE_LIST_NODE *)pSortNodeAdd);
    if ( ueDLContext_p->userLocationType == CE_USER )
    {
        if ( maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex] )
        {
            if ( !leastPriorityCEUe_p[internalCellIndex] )
            {
                leastPriorityCEUe_p[internalCellIndex] = pSortNodeAdd;    
            }

            (maxCellEdgeUEsToBeScheduledDL_g[internalCellIndex])--;
        }
        else
        {
            if ( !leastPriorityCEUe_p[internalCellIndex] )
            {
                listDeleteNode(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList), 
                           (LTE_LIST_NODE *)pSortNodeAdd);
                dlPfsFreeSortNumElem(pSortNodeAdd,internalCellIndex);
                ltePanic("This case never occur\n");
            }
            else
            {
                /* 
                ** This is an additional CE UE. Delete lowest Priority
                ** CE UE from list then get the pointer pointing to lowest
                ** CE UE.
                */
                lowestPriorityCEUe_p = leastPriorityCEUe_p[internalCellIndex];
                pSortNodeT = leastPriorityCEUe_p[internalCellIndex];

                if (leastPriorityCEUe_p[internalCellIndex] == leastPriorityUe_p[internalCellIndex])
                {
                    leastPriorityUe_p[internalCellIndex] = 
                    (SortNodeT *) leastPriorityUe_p[internalCellIndex]->node.previous;     
                }

                while ( (pSortNodeT = 
                  (SortNodeT *)getPrevListNode((LTE_LIST_NODE *)pSortNodeT)) )
                {
                    /* Get the pointer pointing to lowest CE UE in list */
                    if ( pSortNodeT->userLocationType == CE_USER )
                    {
                        leastPriorityCEUe_p[internalCellIndex] = pSortNodeT;
                        break;
                    }
                }
                listDeleteNode(&(pfsGlobals_gp[internalCellIndex]->lastTickScheduledList), 
                           (LTE_LIST_NODE *)lowestPriorityCEUe_p);
                dlPfsFreeSortNumElem(lowestPriorityCEUe_p,internalCellIndex);
            }
            *additionalNodeToBeDeleted_p = FALSE;
        }
    }
}
/* ICIC changes end */
