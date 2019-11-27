/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ***************************************************************************
 *
 *  $Id: lteMacRRMPeriodicMgr.c
 *
 ****************************************************************************
 *
 *  File Description : This file contains function for processing periodic report
 *                     and storing its configuration. 
 *
 ************************************************************************/
/**************************************************************************
 * Standard Library Includes
 *************************************************************************/
/**************************************************************************
 * Project Includes
 *************************************************************************/
#include "lteMacDLUEContext.h"
#include "lteMacULUEContext.h"
#include "lteMacRRMPeriodicMgr.h"
#include "lteMacRRMInterface.h"
#include "lteMacRRMIpcPort.h"
#include "lteMessageQIntf.h"
#include "lteMacCellConfMgr.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacDLPowerControlData.h"
#include "lteMacRRMIntfInfo.h"

/**************************************************************************
 * Private Definitions
 *************************************************************************/
/**************************************************************************
 * Global types
 *************************************************************************/
#define DL_UE_PERIODIC_REPORT_SHM_KEY 444444
#define UL_UE_PERIODIC_REPORT_SHM_KEY 444446
/* +DYNAMIC_ICIC */
#define L1_REPORT_SHM_KEY 444448
#define DL_ICIC_REPORT_SHM_KEY 444450
#define UL_ICIC_REPORT_SHM_KEY 444452
/* -DYNAMIC_ICIC */

/*Complexity Start*/
void updatePeriodicReportInfoforVictim( 
			PeriodicReportInfo *periodicReportInfo_p,
			DLPeriodicReportInfo *dlPeriodicReportInfo_p, 
			DLUEContext *ueDLContext_p);


/*Complexity End*/
void init_periodicReportPool(InternalCellIndex internalCellIndex);
/*ca changes start*/
RRMMACPeriodicMgrCellGlobal *rrmMacPeriodicMgrCellGlobal_gp[MAX_NUM_CELL];
/*ca changes end*/
/* FD for periodic reoport opportuinity */
MQD_T periodicReportFd_g;

/* RNTP threshold range : enumerated {-INFINITY,-11,-10...3} */
SInt32 rntpThresholdMap_g[16] = {INT_MIN, -11, -10, -9, -8, -7, -6, -5,
    -4, -3, -2, -1, 0, 1, 2, 3};
/* -DYNAMIC_ICIC */
/* TTIB_Code Start */
    /* SPR 8104 */
/*****************************************************************************
 * Function Name  : updateULPHR
 * Inputs         : ueIndex,PHR,internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for UE's uplink PHR 
 ******************************************************************************/
//CODE_COMMENT_L10_FIXED
/* SPR 18641_18843 start */
                            /* + SPR 19724 */
void updateULPHR(UInt16 ueIndex, UInt32 PRB, 
                            /* - SPR 19724 */
                InternalCellIndex  internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ulUEPeriodicReport. \
                            /* + SPR 19724 */
                            ulPeriodicReportInfo_p[ueIndex].uplinkPRB += PRB;
                            /* - SPR 19724 */
}
/* SPR 18641_18843 end */
    /* SPR 8104 */
/* TTIB_Code End */

/*****************************************************************************
 * Function Name  : initSHMForRRMPeriodicReport 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function initializes the shared memory which is used
 *                  for both DL and UL UE's. It also attaches the shared memory
 *                  to global structure.
 *****************************************************************************/
UInt32 initSHMForRRMPeriodicReport(InternalCellIndex internalCellIndex)
{


     rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport.dlPeriodicReportInfo_p = 
        (DLPeriodicReportInfo *)getMemFromPool(sizeof(DLPeriodicReportInfo)* MAX_UE_SUPPORTED,PNULL);
     if(NULL == rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport.dlPeriodicReportInfo_p)
     {
         ltePanic("memory allocation failed for dlPeriodicReportInfo_p %s",__func__);
     }

     rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ulUEPeriodicReport.ulPeriodicReportInfo_p = 
        (ULPeriodicReportInfo *)getMemFromPool(sizeof(ULPeriodicReportInfo)* MAX_UE_SUPPORTED,PNULL);
     if(NULL == rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ulUEPeriodicReport.ulPeriodicReportInfo_p)
     {
         ltePanic("memory allocation failed for ulPeriodicReportInfo_p %s",__func__);
     }
    /* +DYNAMIC_ICIC */
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p =
            (Layer1ReportInfo *)getMemFromPool(sizeof(Layer1ReportInfo), PNULL);
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p,
            0,sizeof(Layer1ReportInfo));
    if (NULL == rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p)
    {
        ltePanic("memory allocation failed for Layer1ReportInfo %s",__func__);
    }

    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.dlICICReportInfo_p = 
        (DLICICReportInfo *)getMemFromPool((sizeof(DLICICReportInfo)* 2), PNULL);
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.dlICICReportInfo_p,
            0,(sizeof(DLICICReportInfo)* 2));
    if (NULL == rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.dlICICReportInfo_p)
    {
        ltePanic("memory allocation failed for DLICICReportInfo %s",__func__);
    }

    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.ulICICReportInfo_p = 
        (ULICICReportInfo *)getMemFromPool((sizeof(ULICICReportInfo)* 2), PNULL);
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.ulICICReportInfo_p,
            0,(sizeof(ULICICReportInfo)* 2));
    if (NULL == rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.ulICICReportInfo_p)
    {
        ltePanic("memory allocation failed for ULICICReportInfo %s",__func__);
    }

    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.absCountInfo_p = 
        (ABSCountInfo *)getMemFromPool((sizeof(ABSCountInfo)* 2), PNULL);
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.absCountInfo_p,
            0,(sizeof(ABSCountInfo)* 2));
    if (NULL == rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.absCountInfo_p)
    {
        ltePanic("memory allocation failed for ULICICReportInfo %s",__func__);
    }
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p = 
        (DLEICICReportInfo *)getMemFromPool((sizeof(DLEICICReportInfo)* 2), PNULL);
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p,
            0,(sizeof(DLEICICReportInfo)* 2));
    if (NULL == rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p)
    {
        ltePanic("memory allocation failed for DLEICICReportInfo %s",__func__);
    }
    /* -DYNAMIC_ICIC */

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : init_periodicReportPool
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This functions create the memory pools.
 *****************************************************************************/
void init_periodicReportPool(InternalCellIndex internalCellIndex)
{
    /* Memory handling changes start */
    createMemPool(sizeof(DLPeriodicReportInfo)* MAX_UE_SUPPORTED,1);
    createMemPool(sizeof(ULPeriodicReportInfo)* MAX_UE_SUPPORTED,1);
    createMemPool(sizeof(Layer1ReportInfo),1);
    createMemPool(sizeof(DLICICReportInfo)*2,1);
    createMemPool(sizeof(ULICICReportInfo)*2,1);
    createMemPool(sizeof(ABSCountInfo)*2,1);
    /* Memory handling changes end */
    /* CA changes start*/
    createMemPool(MAX_MSG_SIZE,3);
    createMemPool(sizeof(RRMMACPeriodicMgrCellGlobal),1);
    GET_MEM_FROM_POOL(RRMMACPeriodicMgrCellGlobal,
                       rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex],\
                        sizeof(RRMMACPeriodicMgrCellGlobal),PNULL );
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex],0,
                            sizeof(RRMMACPeriodicMgrCellGlobal));
    GET_MEM_FROM_POOL(UInt8,rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                            bufferForPeriodicReport_p,MAX_MSG_SIZE,PNULL);
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForPeriodicReport_p,0,
                            MAX_MSG_SIZE);
    GET_MEM_FROM_POOL(UInt8,rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                                bufferForICICReport_p,MAX_MSG_SIZE,PNULL);
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForICICReport_p,
                            0,MAX_MSG_SIZE);
    GET_MEM_FROM_POOL(UInt8,rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                                bufferForLayer1Report_p,MAX_MSG_SIZE,PNULL);
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForLayer1Report_p,
                            0,MAX_MSG_SIZE);
    /*EICIC */
    GET_MEM_FROM_POOL(UInt8,rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                                bufferForEICICReport_p,MAX_MSG_SIZE,PNULL);
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForEICICReport_p,
                            0,MAX_MSG_SIZE);	
   /*EICIC*/	
	
    /* CA changes end*/
}

/*****************************************************************************
 * Function Name  : processPeriodicReport 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is executed in every tick and if the current
 *                  tick is eligible for Periodic report processing, function
 *                  for DL/UL is called to process the report.
 *****************************************************************************/
void processPeriodicReport(InternalCellIndex  internalCellIndex)
{
    UInt32 numOfUE = 0;
    UInt32 index = 0;
    /* +- SPR 18268 */
    UInt16 ueIndex = 0;
    /* +- SPR 18268 */
    RrmMacPeriodicReportsInd *periodicReportsInd_p = PNULL;
    PeriodicReportInfo *periodicReportInfo_p = PNULL;
    DLPeriodicReportInfo *dlPeriodicReportInfo_p = PNULL;
    ULPeriodicReportInfo *ulPeriodicReportInfo_p = PNULL;
    UInt32 msgSize = 0;
    L2RrmMsg *l2RrmMsg_p = PNULL;
    /* + TM7_8 Changes Start */
    UInt8 count = 0;
    DLUEContextInfo *ueDLContextInfo_p = PNULL;
    DLUEContext *ueDLContext_p = PNULL;
    ULUEContextInfo *ueULContextInfo_p = PNULL;
    ULUEContext *ueULContext_p = PNULL;
    /* - TM7_8 Changes End */
    /* CA changes start*/
    UInt8 scellIndex                = START_SCELL_INDEX;
    CellConfigParams *cellParams_p  = NULL;
    DLUESCellContext *dlScellContext_p =PNULL;
    cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p \
        [internalCellIndex]->cellParams_p;
    /* EICIC_Coverity CID 69404 Start */
    UserType		currentUserType = NON_VICTIM_UE;
    /* EICIC_Coverity CID 69404 End */
    /* CA changes end*/   

    l2RrmMsg_p = (L2RrmMsg *)rrmMacPeriodicMgrCellGlobal_gp \
        [internalCellIndex]->bufferForPeriodicReport_p;
    if (GET_CURRENT_TICK_FOR_CELL(internalCellIndex) >= 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextPeriodicReportOppor)
    {
        periodicReportsInd_p = (RrmMacPeriodicReportsInd *)l2RrmMsg_p->data; 
        numOfUE = 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            periodicReportUEBatch[rrmMacPeriodicMgrCellGlobal_gp \
            [internalCellIndex]->periodicReportCurrentBatchIndex].numOfUE; 

        while(index < numOfUE)
        {
            periodicReportInfo_p = 
                (PeriodicReportInfo *)(periodicReportsInd_p->reportInfo + index);
            /* updating periodic report indication message from 
             * shared memory structure */
            ueIndex = 
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                periodicReportUEBatch[rrmMacPeriodicMgrCellGlobal_gp \
                [internalCellIndex]->periodicReportCurrentBatchIndex].ueID[index];

            /* + TM7_8 Changes Start */
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex]; 
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
            /* EICIC_Coverity CID 69404 Start */
            if(ueDLContext_p)
            {
                currentUserType = ueDLContext_p->userType;
            }
            /* EICIC_Coverity CID 69404 End */
            ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex]; 
            ueULContext_p = ueULContextInfo_p->ulUEContext_p;
            /* - TM7_8 Changes End */
            periodicReportInfo_p->ueIndex = ueIndex;
            dlPeriodicReportInfo_p = 
                &rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex];
            ulPeriodicReportInfo_p = 
                &rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                ulUEPeriodicReport.ulPeriodicReportInfo_p[ueIndex];
            /* EICIC +*/
            /* Update reporting variables for victim UE*/
            if (VICTIM_UE==currentUserType)
            {

                periodicReportInfo_p->downlinkBlerValueCW0 = 
                    dlPeriodicReportInfo_p->downlinkBlerValueCW0;
                periodicReportInfo_p->downlinkBlerValueCW1 = 
                    dlPeriodicReportInfo_p->downlinkBlerValueCW1;

                periodicReportInfo_p->timingAdvanceValue = 
                    dlPeriodicReportInfo_p->timingAdvanceValue;
                /*4x4 DL MIMO CHG START*/
                periodicReportInfo_p->downlinkSINRValueCW0Layers2 = 0;
                periodicReportInfo_p->downlinkSINRValueCW1Layers2 = 0;
                periodicReportInfo_p->downlinkBlerValueCW0Layers2 = 0;
                periodicReportInfo_p->downlinkBlerValueCW1Layers2 = 0;
                /*4x4 DL MIMO CHG END*/
                /*spr 5033  changes start */
                periodicReportInfo_p->pucchSINRValue = dlPeriodicReportInfo_p->pucchSINRValue;
                periodicReportInfo_p->pucchBlerValue = dlPeriodicReportInfo_p->pucchBlerValue;
                /* spr 5033 changes end */

                periodicReportInfo_p->uplinkSINRValue = 
                    ulPeriodicReportInfo_p->uplinkSINRValue;
                periodicReportInfo_p->uplinkBlerValue = 
                    ulPeriodicReportInfo_p->uplinkBlerValue;
                periodicReportInfo_p->uplinkPathLossValue = 
                    ulPeriodicReportInfo_p->uplinkPathlossValue;
                /* TTIB_Code Start */
                /* SPR 8104 */
                /* SPR 18641_18843 start */
                /* + SPR 19724 */
                periodicReportInfo_p->uplinkPRB =
                    (ulPeriodicReportInfo_p->uplinkPRB/rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams.periodicity);
                if (  periodicReportInfo_p->uplinkPRB == 0 )
                    periodicReportInfo_p->uplinkPRB++;

                ulPeriodicReportInfo_p->uplinkPRB = 0;
                /* - SPR 19724 */
                /* SPR 18641_18843 end */
                /* SPR 8104 */
                /* TTIB_Code End */
                /* CA Changes start */
                /*Initialise the bitmaks and  scellReportBitmask fields for each UE*/
                periodicReportInfo_p->bitMask = 0;
                periodicReportInfo_p->scellReportBitMask = 0;
                /* SPR 13347 fix start */
                if(ueDLContext_p)
                {	
                    /*Complexity Start*/
                    updatePeriodicReportInfoforVictim( 
                            periodicReportInfo_p,
                            dlPeriodicReportInfo_p, 
                            ueDLContext_p);
                    /*Complexity End*/
                    /* SPR 13347 fix end */
                    /* SPR 13347 fix start */
                }
                /* SPR 13347 fix end */
                if(periodicReportInfo_p->scellReportBitMask)
                {
                    /*scellReportBitMask is set, this implies that
                     *scell exists, hence set bitmap field to indicate
                     * presence of valid scellReport*/
                    periodicReportInfo_p->bitMask =  
                        periodicReportInfo_p->bitMask   | 0x000001;
                }
                /* CA Changes end*/
                /* + TM7_8 Changes Start */
                /* SPR 13347 fix start */
                if(ueDLContext_p)
                {	
                    /* SPR 13347 fix end */
                    for (count = 0; count < cellParams_p->numOfTxAnteenas; count++)
                    {
                        periodicReportInfo_p->AoA[count] = ueDLContext_p->AoA[count]; 
                    }
                    /* - TM7_8 Changes End */
                    /* SPR 13347 fix start */
                }
                /* SPR 13347 fix end */
                /*+ EICIC*/	
                /* Set the bitmask and check if UE is RELEASE 10 then report the SINR in _2 variables , else 
                   report the SINR in normal variables*/	

                periodicReportInfo_p->bitMask=periodicReportInfo_p->bitMask  | 0x000002; 

                if  ((ueULContext_p->ueComplianceRelease == RELEASE_10) && (ueULContext_p->pucchConfigInfo.cqiPmiConfigIndex_2 != MAC_MAX_CQI_PMI_CONFIG_INDEX + 1))
                {
                    periodicReportInfo_p->csiSubframeSet2Report.downlinkSINRValueCW0_2=dlPeriodicReportInfo_p->downlinkSINRValueCW0;
                    periodicReportInfo_p->csiSubframeSet2Report.downlinkSINRValueCW1_2=dlPeriodicReportInfo_p->downlinkSINRValueCW1;
                    periodicReportInfo_p->downlinkSINRValueCW0=dlPeriodicReportInfo_p->downlinkSINRValueCW0_2;
                    periodicReportInfo_p->downlinkSINRValueCW1=dlPeriodicReportInfo_p->downlinkSINRValueCW1_2;

                }
                else
                {
                    periodicReportInfo_p->csiSubframeSet2Report.downlinkSINRValueCW0_2=dlPeriodicReportInfo_p->downlinkSINRValueCW0_2;
                    periodicReportInfo_p->csiSubframeSet2Report.downlinkSINRValueCW1_2=dlPeriodicReportInfo_p->downlinkSINRValueCW1_2;	
                    periodicReportInfo_p->downlinkSINRValueCW0 = dlPeriodicReportInfo_p->downlinkSINRValueCW0;
                    periodicReportInfo_p->downlinkSINRValueCW1 =dlPeriodicReportInfo_p->downlinkSINRValueCW1;

                }

                periodicReportInfo_p->csiSubframeSet2Report.downlinkBlerValueCW0_2=dlPeriodicReportInfo_p->downlinkBlerValueCW0;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkBlerValueCW1_2=dlPeriodicReportInfo_p->downlinkBlerValueCW1;

                periodicReportInfo_p->csiSubframeSet2Report.downlinkSINRValueCW0Layers2_2=0;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkSINRValueCW1Layers2_2=0;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkBlerValueCW0Layers2_2=0;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkBlerValueCW1Layers2_2=0;
                msgSize += sizeof(PeriodicReportInfo);
                ++index;

            }

            else if (NON_VICTIM_UE==currentUserType)
            {
                periodicReportInfo_p->downlinkSINRValueCW0 = 
                    dlPeriodicReportInfo_p->downlinkSINRValueCW0;
                periodicReportInfo_p->downlinkSINRValueCW1 = 
                    dlPeriodicReportInfo_p->downlinkSINRValueCW1;

                periodicReportInfo_p->downlinkBlerValueCW0 = 
                    dlPeriodicReportInfo_p->downlinkBlerValueCW0;
                periodicReportInfo_p->downlinkBlerValueCW1 = 
                    dlPeriodicReportInfo_p->downlinkBlerValueCW1;

                periodicReportInfo_p->timingAdvanceValue = 
                    dlPeriodicReportInfo_p->timingAdvanceValue;
                /*4x4 DL MIMO CHG START*/
                periodicReportInfo_p->downlinkSINRValueCW0Layers2 = 0;
                periodicReportInfo_p->downlinkSINRValueCW1Layers2 = 0;
                periodicReportInfo_p->downlinkBlerValueCW0Layers2 = 0;
                periodicReportInfo_p->downlinkBlerValueCW1Layers2 = 0;
                /*4x4 DL MIMO CHG END*/
                /*spr 5033  changes start */
                periodicReportInfo_p->pucchSINRValue = dlPeriodicReportInfo_p->pucchSINRValue;
                periodicReportInfo_p->pucchBlerValue = dlPeriodicReportInfo_p->pucchBlerValue;
                /* spr 5033 changes end */

                periodicReportInfo_p->uplinkSINRValue = 
                    ulPeriodicReportInfo_p->uplinkSINRValue;
                periodicReportInfo_p->uplinkBlerValue = 
                    ulPeriodicReportInfo_p->uplinkBlerValue;
                periodicReportInfo_p->uplinkPathLossValue = 
                    ulPeriodicReportInfo_p->uplinkPathlossValue;
                /* TTIB_Code Start */
                /* SPR 8104 */
                /* SPR 18641_18843 start */
                /* + SPR 19724 */
                periodicReportInfo_p->uplinkPRB =
                    (ulPeriodicReportInfo_p->uplinkPRB/rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams.periodicity);
                if (  periodicReportInfo_p->uplinkPRB == 0 )
                    periodicReportInfo_p->uplinkPRB++;

                ulPeriodicReportInfo_p->uplinkPRB = 0;
                /* - SPR 19724 */
                /* SPR 18641_18843 end */
                /* SPR 8104 */
                /* TTIB_Code End */
                /* CA Changes start */
                /*Initialise the bitmaks and  scellReportBitmask fields for each UE*/
                periodicReportInfo_p->bitMask = 0;
                periodicReportInfo_p->scellReportBitMask = 0;
                /* SPR 13347 fix start */
                if(ueDLContext_p)
                {	
                    /* SPR 13347 fix end */
                    for(scellIndex = 0; scellIndex <= MAX_NUM_SCELL-1; scellIndex++)
                    {
                        /*check if scell is configure for corresponding to this scellIndex*/
                        dlScellContext_p  = ueDLContext_p->dlUeScellContext_p[scellIndex];
                        if(PNULL != dlScellContext_p)
                        {
                            periodicReportInfo_p->scellReport[scellIndex]. \
                                downlinkSINRValueCW0 = 
                                dlPeriodicReportInfo_p->dlSCellPeriodicReport[scellIndex]. \
                                downlinkSINRValueCW0;
                            periodicReportInfo_p->scellReport[scellIndex]. \
                                downlinkSINRValueCW1 = 
                                dlPeriodicReportInfo_p->dlSCellPeriodicReport[scellIndex]. \
                                downlinkSINRValueCW1;
                            periodicReportInfo_p->scellReport[scellIndex]. \
                                downlinkBlerValueCW0 = 
                                dlPeriodicReportInfo_p->dlSCellPeriodicReport[scellIndex]. \
                                downlinkBlerValueCW0;
                            periodicReportInfo_p->scellReport[scellIndex]. \
                                downlinkBlerValueCW1 = 
                                dlPeriodicReportInfo_p->dlSCellPeriodicReport[scellIndex]. \
                                downlinkBlerValueCW1;
                            /* MIMO CHG START*/
                            /*these counter values are currently not used by RRM,
                              set them to zero*/
                            periodicReportInfo_p->scellReport[scellIndex]. \
                                downlinkSINRValueCW0Layers2 = 0;
                            periodicReportInfo_p->scellReport[scellIndex]. \
                                downlinkSINRValueCW1Layers2 = 0;
                            periodicReportInfo_p->scellReport[scellIndex]. \
                                downlinkBlerValueCW0Layers2 = 0;
                            periodicReportInfo_p->scellReport[scellIndex]. \
                                downlinkBlerValueCW1Layers2 = 0;
                            /*4x4 DL MIMO CHG END*/
                            /*set the scellReportbitmak*/   
                            periodicReportInfo_p->scellReportBitMask = 
                                (periodicReportInfo_p->scellReportBitMask | (1 << scellIndex));
                        }
                    }
                    /* SPR 13347 fix start */
                }
                /* SPR 13347 fix end */
                if(periodicReportInfo_p->scellReportBitMask)
                {
                    /*scellReportBitMask is set, this implies that
                     *scell exists, hence set bitmap field to indicate
                     *presence of valid scellReport*/
                    periodicReportInfo_p->bitMask =  
                        periodicReportInfo_p->bitMask   | 0x000001;
                }
                /* CA Changes end*/
                /* + TM7_8 Changes Start */
                /* SPR 13347 fix start */
                if(ueDLContext_p)
                {	
                    /* SPR 13347 fix end */
                    for (count = 0; count < cellParams_p->numOfTxAnteenas; count++)
                    {
                        periodicReportInfo_p->AoA[count] = ueDLContext_p->AoA[count]; 
                    }
                    /* - TM7_8 Changes End */
                    /* SPR 13347 fix start */
                }
                /* SPR 13347 fix end */

                /*+ EICIC*/		

                /* Update the _2 for non victim UE */

                periodicReportInfo_p->bitMask=periodicReportInfo_p->bitMask | 0x000002;

                periodicReportInfo_p->csiSubframeSet2Report.downlinkSINRValueCW0_2=dlPeriodicReportInfo_p->downlinkSINRValueCW0_2;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkSINRValueCW1_2=dlPeriodicReportInfo_p->downlinkSINRValueCW1_2;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkBlerValueCW0_2=dlPeriodicReportInfo_p->downlinkBlerValueCW0_2;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkBlerValueCW1_2=dlPeriodicReportInfo_p->downlinkBlerValueCW1_2;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkSINRValueCW0Layers2_2=0;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkSINRValueCW1Layers2_2=0;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkBlerValueCW0Layers2_2=0;
                periodicReportInfo_p->csiSubframeSet2Report.downlinkBlerValueCW1Layers2_2=0;
                msgSize += sizeof(PeriodicReportInfo);
                ++index;
            }
        }


        /* Log for Periodic Report Indication */
        LOG_UT(MAC_RRM_PERIODIC_REPORT_IND,LOGDEBUG,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportCurrentBatchIndex,
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextPeriodicReportOppor,
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams.batchInterval,
                LINE_NUMBER,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");


        /* Increment the batch number */
        ++rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportCurrentBatchIndex;
        if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportCurrentBatchIndex == 
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams.numOfBatches)
        {
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportCurrentBatchIndex = 0;
        }

        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextPeriodicReportOppor += 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams.batchInterval;

        if(numOfUE)
        {

            /* calculating total message length 
             * total lenght = header len + sizeof(numUe) + sizeof variable len array*/ 
            msgSize += sizeof(UInt32) + L2_RRM_API_HEADER_LEN;

            periodicReportsInd_p->numOfUE = numOfUE;

            /* sector ID should be set to globally stored sectoer ID */
            prepareMACRRMApiHeader(l2RrmMsg_p,RRM_MAC_PERIODIC_REPORT_IND,
                    L2_RRM_IND_TRANS_ID,msgSize,internalCellIndex);
            /* Send the confimration back to RRM */
            if(MAC_SEND_ERROR ==  
                    sendMsgToRRM(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                        bufferForPeriodicReport_p, msgSize
                        ))
            {
                LOG_MAC_MSG(MAC_RRM_MESSAGE_SEND_FAIL,LOGERROR,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        LINE_NUMBER,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");


            }
        }
    }
}


/*****************************************************************************
 * Function Name  : updatePeriodicReportInfoforVictim 
 * Inputs         : periodicReportInfo_p
 *                  dlPeriodicReportInfo_p
 *                  ueDLContext_p
 * Outputs        : periodic report
 * Returns        : void
 * Description    : This function updates the periodic report info for the
 *                  victim UE.
 *****************************************************************************/
void updatePeriodicReportInfoforVictim( 
        PeriodicReportInfo *periodicReportInfo_p,
        DLPeriodicReportInfo *dlPeriodicReportInfo_p, 
        DLUEContext *ueDLContext_p)
{
    UInt8 scellIndex = 0;
    DLUESCellContext *dlScellContext_p =PNULL;

    /* SPR 13347 fix end */
    for(scellIndex = 0; scellIndex <= MAX_NUM_SCELL -1; scellIndex++)
    {
        /*check if scell is configure for corresponding to this scellIndex*/
        dlScellContext_p  = ueDLContext_p->dlUeScellContext_p[scellIndex];
        if(PNULL != dlScellContext_p)
        {
            periodicReportInfo_p->scellReport[scellIndex]. \
                downlinkSINRValueCW0 = 
                dlPeriodicReportInfo_p->dlSCellPeriodicReport[scellIndex]. \
                downlinkSINRValueCW0;
            periodicReportInfo_p->scellReport[scellIndex]. \
                downlinkSINRValueCW1 = 
                dlPeriodicReportInfo_p->dlSCellPeriodicReport[scellIndex]. \
                downlinkSINRValueCW1;
            periodicReportInfo_p->scellReport[scellIndex]. \
                downlinkBlerValueCW0 = 
                dlPeriodicReportInfo_p->dlSCellPeriodicReport[scellIndex]. \
                downlinkBlerValueCW0;
            periodicReportInfo_p->scellReport[scellIndex]. \
                downlinkBlerValueCW1 = 
                dlPeriodicReportInfo_p->dlSCellPeriodicReport[scellIndex]. \
                downlinkBlerValueCW1;
            /* MIMO CHG START*/
            /*these counter values are currently not used by RRM,
              set them to zero*/
            periodicReportInfo_p->scellReport[scellIndex]. \
                downlinkSINRValueCW0Layers2 = 0;
            periodicReportInfo_p->scellReport[scellIndex]. \
                downlinkSINRValueCW1Layers2 = 0;
            periodicReportInfo_p->scellReport[scellIndex]. \
                downlinkBlerValueCW0Layers2 = 0;
            periodicReportInfo_p->scellReport[scellIndex]. \
                downlinkBlerValueCW1Layers2 = 0;
            /*4x4 DL MIMO CHG END*/
            /*set the scellReportbitmak*/   
            periodicReportInfo_p->scellReportBitMask = 
                (periodicReportInfo_p->scellReportBitMask | (1 << scellIndex));
        }
    }
    /* SPR 13347 fix start */

}

/*Complexity End*/
/**************************************************************************
 * Function Name : processLayer1Report
 * Inputs        : internalCellIndex
 * Outputs       : none
 * Returns       : None
 * Description   : 
 *
 * *********************************************************************/
void processLayer1Report(InternalCellIndex internalCellIndex)
{
    UInt32 msgSize = 0;
    UInt8 count = 0;
    MacRrmLayer1MeasReportInd *layer1ReportInd_p = PNULL;

    L2RrmMsg *l2RrmMsg_p = PNULL;
    l2RrmMsg_p = (L2RrmMsg *)rrmMacPeriodicMgrCellGlobal_gp
        [internalCellIndex]->bufferForLayer1Report_p;
    /*ca changes start*/
    CellConfigParams *cellParams_p = NULL;
    cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* ca changes end */
    /* Checking whether current tick is eligible for L1 report processing */
    if(GET_CURRENT_TICK_FOR_CELL(internalCellIndex) >= rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextL1ReportOppor)
    {
        /* Typecasting the sending buffer to the structure for sending the indication */
        layer1ReportInd_p = (MacRrmLayer1MeasReportInd*)l2RrmMsg_p->data;
        /* resetting the bitmask value */
        layer1ReportInd_p->bitmask = 0;

        /* Filling the L1 report Api structure depending upon the configuration from RRM */

        /* Setting the corresponding bitmask value */
        layer1ReportInd_p->bitmask |= INTERFERENCE_PER_PRB_BITMASK;
        for(count =0; count < cellParams_p->ulAvailableRBs; count++)
        {
            layer1ReportInd_p->interferencePerPRB[count] =
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->interferencePerPRB[count];
        }


        layer1ReportInd_p->bitmask |= THERMAL_NOISE_BITMASK;
        layer1ReportInd_p->thermalNoise =
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->thermalNoise;

        layer1ReportInd_p->bitmask |= RS_TX_POWER_BITMASK;
        layer1ReportInd_p->dlRsTxPower =
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportInd.layer1ReportInfo_p->dlRsTxPower;

        /* Calculating the total message lenth */
        msgSize = sizeof(MacRrmLayer1MeasReportInd)+L2_RRM_API_HEADER_LEN;

        /* Incrementing the global for next interference report opportunity */
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextL1ReportOppor = 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextL1ReportOppor + 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportConfigParams.periodicity;

        /* Prepares API headers for sending L1 indication to RRM with appropriate msgSize */
        prepareMACRRMApiHeader(l2RrmMsg_p,RRM_MAC_L1_REPORT_IND,0,msgSize, internalCellIndex);

        /* Sending message to RRM */
        if(MAC_SEND_ERROR == 
                sendMsgToRRM(rrmMacPeriodicMgrCellGlobal_gp \
                    [internalCellIndex]->bufferForLayer1Report_p, msgSize
                    ))
        {
            LOG_MAC_MSG(MAC_RRM_MESSAGE_SEND_FAIL,LOGERROR,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
        }
    }
    return;
}


/************************************************************************
 * Function Name : processICICReport
 * Inputs        : internalCellIndex
 * Outputs       : none
 * Returns       : None
 * Description   : 
 *
 * *********************************************************************/
void processICICReport(InternalCellIndex internalCellIndex)
{
    UInt32 msgSize = 0;
    UInt8 count = 0;
    MacRrmLayer2IcicMeasReportInd *icicReportInd_p = PNULL;
    /*ca changes start*/
    CellConfigParams *cellParams_p = NULL;
    cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p \
                   [internalCellIndex]->cellParams_p;
    /* ca changes end */

    L2RrmMsg *l2RrmMsg_p = PNULL;
    l2RrmMsg_p = (L2RrmMsg *)rrmMacPeriodicMgrCellGlobal_gp \
        [internalCellIndex]->bufferForICICReport_p;
    /* SPR 14482 Fix Start */
    /* code removed */   
    /* SPR 14482 Fix End */

    /* L2 is writing on activeReportIndex array index, and low priroity
     * thread reads from complement index */

    UInt8 passiveIndex = !rrmMacPeriodicMgrCellGlobal_gp \
                        [internalCellIndex]->activeReportIndex;
    
    /* SPR 21199 fix start */
    /* if clause removed */
    /* calculation of nextICICReportOppor is moved to HP Thread */
    /* SPR 21199 fix end */
    /* Checking whether current tick is eligible for ICIC report processing */
    /* Typecasting the sending buffer to the structure for sending the indication */
    icicReportInd_p = 
        (MacRrmLayer2IcicMeasReportInd*)&(rrmMacPeriodicMgrCellGlobal_gp \
                [internalCellIndex]->bufferForICICReport_p[L2_RRM_API_HEADER_LEN]);
    /* resetting the bitmask value */
    icicReportInd_p->bitmask = 0;

    /* Filling the ICIC report Api structure depending upon the configuration from RRM */
    if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
    {
        /* Setting the corresponding bitmask value */
        icicReportInd_p->bitmask |= RNTP_VAL_BITMASK;
        for(count =0;count < cellParams_p->dlAvailableRBs; count++)
        {
            icicReportInd_p->rntpValuePerPRB[count] = 
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                icicReportInd.dlICICReportInfo_p[passiveIndex].rntpValuePerPRB[count];
            /* This is temporary change to give RNTP value as 0 or 1 only 
             * to RRM. This must be removed after requirement for 
             * cummulative value agreed between MAC & RRM and updated in 
             * API doc 
             */
            if (icicReportInd_p->rntpValuePerPRB[count])
            {
                icicReportInd_p->rntpValuePerPRB[count] = 1;
            }
        }
    }

    /* Process Prb usage for both UL and DL if required */
    if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            icicReportConfigParams.cellEdgePrbUsageRequired)
    {
        /* Setting the corresponding bitmask value */
        icicReportInd_p->bitmask |= DL_CE_PRB_USAGE_BITMASK;
        icicReportInd_p->dlCEPrbUsage = 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            icicReportInd.dlICICReportInfo_p[passiveIndex].dlCEPrbUsage;

        /* Setting the corresponding bitmask value */
        icicReportInd_p->bitmask |= UL_CE_PRB_USAGE_BITMASK;
        icicReportInd_p->ulCEPrbUsage = 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            icicReportInd.ulICICReportInfo_p[passiveIndex].ulCEPrbUsage;
        /* Setting the corresponding bitmask value */
        icicReportInd_p->bitmask |= ABS_SUBFRAME_COUNT_BITMASK;
        icicReportInd_p->absSubframeCount = 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            icicReportInd.absCountInfo_p[passiveIndex].absSubframeCount;
    }


    /* Calculating the total message lenth */
    msgSize = sizeof(MacRrmLayer2IcicMeasReportInd) + L2_RRM_API_HEADER_LEN;

    /* SPR 21199 fix start */
    /* calculation of nextICICReportOppor is moved to fuction
     * checkForRRMPerriodicReportOpp */
    /* SPR 21199 fix end */

    /* Prepares API headers for sending ICIC report indication to RRM with appropriate msgSize */
    prepareMACRRMApiHeader(l2RrmMsg_p, RRM_MAC_ICIC_REPORT_IND,0,msgSize, internalCellIndex);

    /* Sending message to RRM */
    if(MAC_SEND_ERROR == sendMsgToRRM(rrmMacPeriodicMgrCellGlobal_gp \
                [internalCellIndex]->bufferForICICReport_p, msgSize
                ))
    {
        //Log msg
    }

    /* Resetting the global structure for passive index */
    memSet(&rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            icicReportInd.dlICICReportInfo_p[passiveIndex],0,
            sizeof(DLICICReportInfo)); 
    memSet(&rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            icicReportInd.ulICICReportInfo_p[passiveIndex],0,
            sizeof(ULICICReportInfo));
    memSet(&rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            icicReportInd.absCountInfo_p[passiveIndex],0,
            sizeof(ABSCountInfo));
}


/*****************************************************************************
 * Function Name  : updateULSINR 
 * Inputs         : ueIndex, averageSINR, internalCellIndex
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for UL SINR
 *****************************************************************************/
 void updateULSINR(UInt32 ueIndex, UInt8 averageSINR,
                            InternalCellIndex internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ulUEPeriodicReport. \
        ulPeriodicReportInfo_p[ueIndex].uplinkSINRValue = averageSINR;

    LOG_MAC_MSG(MAC_UPDATE_UL_RRM_RPT,LOGINFO,MAC_UL_HARQ,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            ueIndex,averageSINR,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            __func__,"UPDATE UL SINR");
}

/*****************************************************************************
 * Function Name  : updateULBler 
 * Inputs         : ueIndex, puschBler, internalCellIndex
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for DL UE's
 *****************************************************************************/
 void updateULBler(UInt32 ueIndex, SInt8 puschBler,
                            InternalCellIndex internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ulUEPeriodicReport. \
        ulPeriodicReportInfo_p[ueIndex].uplinkBlerValue  = puschBler;

    LOG_MAC_MSG(MAC_UPDATE_UL_RRM_RPT,LOGINFO,MAC_UL_HARQ,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            ueIndex,puschBler,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            __func__,"UPDATE UL BLER");
}
/*****************************************************************************
 * Function Name  : updateULPathLoss 
 * Inputs         : ueIndex,pathLoss, internalCellIndex
 * Outputs        : None
 * Returns        : none
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for DL UE's
 *****************************************************************************/
 void updateULPathLoss(UInt32 ueIndex, SInt16 pathLoss,
                                InternalCellIndex internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ulUEPeriodicReport. \
        ulPeriodicReportInfo_p[ueIndex].uplinkPathlossValue = pathLoss;

    LOG_MAC_MSG(MAC_UPDATE_UL_RRM_RPT,LOGINFO,MAC_UL_HARQ,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            ueIndex,pathLoss,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            __func__,"UPDATE UL pathloss");
}

/*****************************************************************************
 * Function Name  : updateDLSINRCW0 
 * Inputs         : ueIndex, cqiValue, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for DL SINR for CW0
 *****************************************************************************/
 void updateDLSINRCW0(UInt16 ueIndex,UInt8 cqiValue,
                                InternalCellIndex internalCellIndex)
{   
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport. \
        dlPeriodicReportInfo_p[ueIndex].downlinkSINRValueCW0 = 
            getDLSinrValueFromCqi(cqiValue);
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,getDLSinrValueFromCqi(cqiValue),
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE DL SINR CW0");
}

/*****************************************************************************
 * Function Name  : updateDLSINRCW1 
 * Inputs         : ueIndex, cqiValue, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for DL SINR for CW 1
 *****************************************************************************/
 void updateDLSINRCW1(UInt16 ueIndex,UInt8 cqiValue,
                                InternalCellIndex internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport. \
        dlPeriodicReportInfo_p[ueIndex].downlinkSINRValueCW1 = 
            getDLSinrValueFromCqi(cqiValue);
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,getDLSinrValueFromCqi(cqiValue),
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE DL SINR CW1");
}
/*+EICIC */
/*****************************************************************************
 * Function Name  : updateDLSINRCW0_2 
 * Inputs         : ueIndex, cqiValue, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for DL SINR for CW0
 *****************************************************************************/
 void updateDLSINRCW0_2(UInt16 ueIndex,UInt8 cqiValue,
                                InternalCellIndex internalCellIndex)
{   
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex].downlinkSINRValueCW0_2 = 
        getDLSinrValueFromCqi(cqiValue);
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,getDLSinrValueFromCqi(cqiValue),
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE DL SINR CW0_2");
}

/*****************************************************************************
 * Function Name  : updateDLSINRCW1_2 
 * Inputs         : ueIndex, cqiValue, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for DL SINR for CW 1
 *****************************************************************************/
 void updateDLSINRCW1_2(UInt16 ueIndex,UInt8 cqiValue,
                                InternalCellIndex internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex].downlinkSINRValueCW1_2 = 
            getDLSinrValueFromCqi(cqiValue);
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,getDLSinrValueFromCqi(cqiValue),
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE DL SINR CW1_2");
}
/*-EICIC */
/*****************************************************************************
 * Function Name  : updateDLBlerCW0 
 * Inputs         : ueIndex, bler, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is responsible for updating DL BLER for CW0.
 *****************************************************************************/

 void updateDLBlerCW0(UInt32 ueIndex, SInt8 bler,
                                InternalCellIndex internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport. \
        dlPeriodicReportInfo_p[ueIndex].downlinkBlerValueCW0  = bler; 

	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,bler,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE CW0 DL BLER");

}
/* EICIC Change Phase1 */
 void updateDLBlerCW0_2(UInt32 ueIndex, SInt8 bler,
                                InternalCellIndex internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex].downlinkBlerValueCW0_2 = bler; 

	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,bler,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE CW0_2 DL BLER");
}
/* EICIC Change Phase1 */

/*****************************************************************************
 * Function Name  : updateDLBlerCW1 
 * Inputs         : ueIndex, bler, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is responsible for updating DL BLER for CW1.
 *****************************************************************************/
 void updateDLBlerCW1(UInt32 ueIndex, SInt8 bler,
                                InternalCellIndex internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport. \
        dlPeriodicReportInfo_p[ueIndex].downlinkBlerValueCW1 = bler; 

	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,bler,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE CW1 DL BLER");
}
/* EICIC Change Phase1 */
/*****************************************************************************
 * Function Name  : updateDLBlerCW1_2 
 * Inputs         : ueIndex, bler, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is responsible for updating DL BLER for CW1_2.
 *****************************************************************************/
 void updateDLBlerCW1_2(UInt32 ueIndex, SInt8 bler,
                                InternalCellIndex internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport. \
        dlPeriodicReportInfo_p[ueIndex].downlinkBlerValueCW1_2 = bler; 

	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,bler,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE CW1_2 DL BLER");
}
/* EICIC Change Phase1 */
/*****************************************************************************
 * Function Name  : updateDLTA 
 * Inputs         : ueIndex,timingAdvanceValue, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for DL UE's
 *****************************************************************************/
 void updateDLTA(UInt32 ueIndex, UInt16 timingAdvanceValue,
                            InternalCellIndex internalCellIndex)
{
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport. \
        dlPeriodicReportInfo_p[ueIndex].timingAdvanceValue = timingAdvanceValue; 
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,timingAdvanceValue,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE DL TA");
}
/* SPR 5033 changes start */
/*****************************************************************************
 * Function Name  : updatePucchSINR 
 * Inputs         : ueIndex, pucchSINR, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is responsible for updating dlUEPeriodicReport_g
 *                  for PUCCH SINR 
 *****************************************************************************/
 void updatePucchSINR(UInt16 ueIndex,UInt8 pucchSINR,
                                InternalCellIndex internalCellIndex)
{   
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport. \
        dlPeriodicReportInfo_p[ueIndex].pucchSINRValue = pucchSINR;
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,pucchSINR,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE pucchSINR");
}

/*****************************************************************************
 * Function Name  : updatePucchBler 
 * Inputs         : ueIndex, pucchBler, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is responsible for updating dlUEPeriodicReport_g
 *                  for PUCCH BLER
 *****************************************************************************/
 void updatePucchBler(UInt16 ueIndex,SInt8 pucchBler,
                                InternalCellIndex internalCellIndex)
{   
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport. \
        dlPeriodicReportInfo_p[ueIndex].pucchBlerValue = pucchBler;
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
			ueIndex,pucchBler,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE pucchBler");
}
/* SPR 5033 changes end */
/* CA changes start*/
/*****************************************************************************
 * Function Name  : macUupdateSCellDLSINRCW0 
 * Inputs         : ueIndex, cqiValue, internalCellIndex,scellIndex
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function is responsible for updating downlinkSINRValueCW0
 *                  for SCell DL SINR for CW0
 *****************************************************************************/
void macUupdateSCellDLSINRCW0(UInt16 ueIndex, UInt8 cqiValue,
        /* +- SPR 17777 */
                                UInt8 scellIndex)
{   
    DlSCellPeriodicReport *sCellReport_p = PNULL;
    /*SPR 16030 fix start, added pCellIndex */
    InternalCellIndex pCellIndex = INVALID_8_VAL; 
    pCellIndex = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->internalCellIndex;

    sCellReport_p =  (DlSCellPeriodicReport*)&rrmMacPeriodicMgrCellGlobal_gp[pCellIndex]-> \
        dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex]. \
        dlSCellPeriodicReport[scellIndex];
    /*SPR 16030 fix end */
    /* +- SPR 17777 */
    sCellReport_p->downlinkSINRValueCW0 = getDLSinrValueFromCqi(cqiValue);
    /* +- SPR 17777 */
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(scellIndex),
			ueIndex,getDLSinrValueFromCqi(cqiValue),
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE Scell DL SINR CW0");
}
/*+EICIC */
/*****************************************************************************
 * Function Name  : macUupdateSCellDLSINRCW0_2 
 * Inputs         : ueIndex, cqiValue, internalCellIndex,scellIndex
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function is responsible for updating downlinkSINRValueCW0_2
 *                  for SCell DL SINR for CW0
 *****************************************************************************/
 void macUupdateSCellDLSINRCW0_2(UInt16 ueIndex, UInt8 cqiValue,
                                InternalCellIndex internalCellIndex, 
                                UInt8 scellIndex)
{   
    DlSCellPeriodicReport *sCellReport_p = PNULL;
    sCellReport_p =  (DlSCellPeriodicReport*)&rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
        dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex]. \
        dlSCellPeriodicReport[scellIndex];
    /* +- SPR 17777 */
    sCellReport_p->downlinkSINRValueCW0_2 = getDLSinrValueFromCqi(cqiValue);
    /* +- SPR 17777 */
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(scellIndex),
			ueIndex,getDLSinrValueFromCqi(cqiValue),
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE Scell DL SINR CW0_2");
}
/*-EICIC */
/*****************************************************************************
 * Function Name  : macUpdateSCellDLSINRCW1 
 * Inputs         : ueIndex, cqiValue scellIndex
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for SCell DL SINR for CW1
 *****************************************************************************/
void macUpdateSCellDLSINRCW1(UInt16 ueIndex,UInt8 cqiValue,
        /* +- SPR 17777 */
                                        UInt8 scellIndex)
{
    DlSCellPeriodicReport *sCellReport_p = PNULL;
    /*SPR 16030 fix start, added pCellIndex */
    InternalCellIndex pCellIndex = INVALID_8_VAL; 
    pCellIndex = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->internalCellIndex;
    sCellReport_p = (DlSCellPeriodicReport*)&rrmMacPeriodicMgrCellGlobal_gp[pCellIndex]-> \
        dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex]. \
        dlSCellPeriodicReport[scellIndex];
    /*SPR 16030 fix end */
    /* +- SPR 17777 */
    sCellReport_p->downlinkSINRValueCW1 = getDLSinrValueFromCqi(cqiValue);
    /* +- SPR 17777 */
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(scellIndex),
			ueIndex,getDLSinrValueFromCqi(cqiValue),
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE Scell DL SINR CW1");
}
/*+EICIC */
/*****************************************************************************
 * Function Name  : macUpdateSCellDLSINRCW1_2 
 * Inputs         : ueIndex, cqiValue ,internalCellIndex, scellIndex
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function is responsible for updating uePeriodicReport_g
 *                  for SCell DL SINR for CW1
 *****************************************************************************/
 void macUpdateSCellDLSINRCW1_2(UInt16 ueIndex,UInt8 cqiValue,
                                        InternalCellIndex internalCellIndex,
                                        UInt8 scellIndex)
{
    DlSCellPeriodicReport *sCellReport_p = PNULL;
    sCellReport_p = (DlSCellPeriodicReport*)&rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
        dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex]. \
        dlSCellPeriodicReport[scellIndex];
    /* +- SPR 17777 */
    sCellReport_p->downlinkSINRValueCW1_2 = getDLSinrValueFromCqi(cqiValue);
    /* +- SPR 17777 */
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(scellIndex),
			ueIndex,getDLSinrValueFromCqi(cqiValue),
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE Scell DL SINR CW1_2");
}
/*-EICIC */
/*****************************************************************************
 * Function Name  : macUpdateSCellDLBlerCW0 
 * Inputs         : ueIndex, bler, scellIndex
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function is responsible for updating SCell DL BLER for CW0
 *****************************************************************************/

void macUpdateSCellDLBlerCW0(UInt32 ueIndex, SInt8 bler,
                                    /* +- SPR 17777 */
                                    UInt8 scellIndex)
{
    DlSCellPeriodicReport *sCellReport_p = PNULL;
    /*SPR 16030 fix start, added pCellIndex */
    InternalCellIndex pCellIndex = INVALID_8_VAL; 
    pCellIndex = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->internalCellIndex;
    sCellReport_p = (DlSCellPeriodicReport *)&rrmMacPeriodicMgrCellGlobal_gp[pCellIndex]-> \
        dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex]. \
        dlSCellPeriodicReport[scellIndex];
    /*SPR 16030 fix end */
    sCellReport_p->downlinkBlerValueCW0 = bler;
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(scellIndex),
			ueIndex,bler,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE Scell DL Bler CW0");
}

/*****************************************************************************
 * Function Name  : macUpdateSCellDLBlerCW1 
 * Inputs         : ueIndex, bler, scellIndex
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function is responsible for updating SCell DL BLER for CW1
 *****************************************************************************/
void macUpdateSCellDLBlerCW1(UInt32 ueIndex, SInt8 bler,
                                    /* +- SPR 17777 */
                                    UInt8 scellIndex)
{
    DlSCellPeriodicReport *sCellReport_p = PNULL;
    /*SPR 16030 fix start, added pCellIndex */
    InternalCellIndex pCellIndex = INVALID_8_VAL; 
    pCellIndex = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->internalCellIndex;
    sCellReport_p = (DlSCellPeriodicReport*)&rrmMacPeriodicMgrCellGlobal_gp[pCellIndex]-> \
        dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex].dlSCellPeriodicReport \
        [scellIndex];
    /*SPR 16030 fix end */
    sCellReport_p->downlinkBlerValueCW1 = bler;
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(scellIndex),
			ueIndex,bler,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE Scell DL Bler CW1");
}

/*+EICIC */
/*****************************************************************************
 * Function Name  : macUpdateSCellDLBlerCW0 
 * Inputs         : ueIndex, bler, internalCellIndex,scellIndex
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function is responsible for updating SCell DL BLER for CW0
 *****************************************************************************/

 void macUpdateSCellDLBlerCW0_2(UInt32 ueIndex, SInt8 bler,
                                    InternalCellIndex internalCellIndex,
                                    UInt8 scellIndex)
{
    DlSCellPeriodicReport *sCellReport_p = PNULL;
    sCellReport_p = (DlSCellPeriodicReport *)&rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
        dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex]. \
        dlSCellPeriodicReport[scellIndex];
    sCellReport_p->downlinkBlerValueCW0_2 = bler;
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(scellIndex),
			ueIndex,bler,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE Scell DL Bler CW0_2");
}

/*****************************************************************************
 * Function Name  : macUpdateSCellDLBlerCW1 
 * Inputs         : ueIndex, bler, internalCellIndex,scellIndex
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function is responsible for updating SCell DL BLER for CW1
 *****************************************************************************/
 void macUpdateSCellDLBlerCW1_2(UInt32 ueIndex, SInt8 bler,
                                    InternalCellIndex internalCellIndex, 
                                    UInt8 scellIndex)
{
    DlSCellPeriodicReport *sCellReport_p = PNULL;
    sCellReport_p = (DlSCellPeriodicReport*)&rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
        dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex].dlSCellPeriodicReport \
        [scellIndex];
    sCellReport_p->downlinkBlerValueCW1_2 = bler;
        
	LOG_MAC_MSG(MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,
			GET_CURRENT_TICK_FOR_CELL(scellIndex),
			ueIndex,bler,
			DEFAULT_INT_VALUE,
			DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			__func__,"UPDATE Scell DL Bler CW1_2");
}
/*-EICIC */
/* CA Changes end*/
/*****************************************************************************
 * Function Name  : initPeriodicReportInfo 
 * Inputs         : ueIndex, internalCellIndex
 * Outputs        : None
 * Returns        : None 
 * Description    : This function initializes default value to periodic report 
 *                  fields.
 *****************************************************************************/

void initPeriodicReportInfo(UInt32 ueIndex, InternalCellIndex internalCellIndex)
{
    UInt8 cqiValue = 0;
    DLPeriodicReportInfo *dlPeriodicReportInfo = 
        &rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            dlUEPeriodicReport.dlPeriodicReportInfo_p[ueIndex];
    ULPeriodicReportInfo *ulPeriodicReportInfo_p = 
        &rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            ulUEPeriodicReport.ulPeriodicReportInfo_p[ueIndex];

    ULUEContext *ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    DLUEContext *dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    GET_DL_CQI_VALUE(cqiValue, dlUEContext_p);
    /* +- SPR 17777 */
    dlPeriodicReportInfo->downlinkSINRValueCW0 = 
        getDLSinrValueFromCqi(cqiValue);
    dlPeriodicReportInfo->downlinkSINRValueCW1 = 
        getDLSinrValueFromCqi(cqiValue);
    /* +- SPR 17777 */

    dlPeriodicReportInfo->downlinkBlerValueCW0 = 
        dlUEContext_p->dlCQIInfo.currentDLBLERForCW0;
    dlPeriodicReportInfo->downlinkBlerValueCW1 = 
        dlUEContext_p->dlCQIInfo.currentDLBLERForCW1;
    dlPeriodicReportInfo->timingAdvanceValue  = 
        dlUEContext_p->timingAdvanceValue;
    ulPeriodicReportInfo_p->uplinkSINRValue = 
        ulUEContext_p->csiContext.averageSINR;
    ulPeriodicReportInfo_p->uplinkBlerValue = 
        ulUEContext_p->csiContext.puschBlerInfo.puschBler;
    ulPeriodicReportInfo_p->uplinkPathlossValue = ulUEContext_p->pathLoss;
}
/*****************************************************************************
 * Function Name  : addUEPeriodicReportBatchInfo 
 * Inputs         : ueIndex, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when a new UE is added to the
 *                  global UE context variable. Function puts newly 
 *                  added UE to a specific batch and also update a map which
 *                  maintains a particular UE belongs to which map.
 *****************************************************************************/
/* Below klockwork change has fixed SPR 10683 in this file */
/*KLOCKWORK_CHANGES_START_162*/
void  addUEPeriodicReportBatchInfo(UInt32 ueIndex, 
                            InternalCellIndex  internalCellIndex)
/*KLOCKWORK_CHANGES_STOP_162*/
{
    UInt32 count = 0;
    /* SPR 20668 fix start */
    UInt16 batchCount = 0;
    UInt16 numOfBatches = rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                          periodicReportParams.numOfBatches;
    UInt16 batchSize = rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                       periodicReportParams.ueBatchSize;
    if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                        periodicReportParams.periodicReportsRequired)
    { 
        for(batchCount=0;((batchCount < numOfBatches)&&(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            periodicReportUEBatch[rrmMacPeriodicMgrCellGlobal_gp \
                        [internalCellIndex]->updateUEBatchCurrentIndex].numOfUE >= batchSize));batchCount++) 

        {
            /* can not add the UE */
            /* SPR 7932 Fix Start*/
            /*return MAC_FAILURE;*/
            ++(rrmMacPeriodicMgrCellGlobal_gp \
                    [internalCellIndex]->updateUEBatchCurrentIndex);
            /* SPR 7932 Fix End*/
                if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                        updateUEBatchCurrentIndex == numOfBatches)
                {
                    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                        updateUEBatchCurrentIndex = 0;
                }
                if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->\
                        periodicReportUEBatch[rrmMacPeriodicMgrCellGlobal_gp \
                        [internalCellIndex]->updateUEBatchCurrentIndex].numOfUE < batchSize)
                {
                    break;
                }

        }
        /* SPR 20668 fix end */

        count = rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            periodicReportUEBatch[rrmMacPeriodicMgrCellGlobal_gp \
                [internalCellIndex]->updateUEBatchCurrentIndex].numOfUE ;
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportUEBatch \
            [rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            updateUEBatchCurrentIndex].ueID[count] = ueIndex;
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportUEBatch \
            [rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                updateUEBatchCurrentIndex].numOfUE ++;

        /* updating ueId to batchId map */
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            ueIDToBatchNumMapping[ueIndex] = 
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            updateUEBatchCurrentIndex;

        ++(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            updateUEBatchCurrentIndex);
        if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            updateUEBatchCurrentIndex == rrmMacPeriodicMgrCellGlobal_gp \
                [internalCellIndex]->periodicReportParams.numOfBatches)
        {
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            updateUEBatchCurrentIndex = 0;
        }
    }
}
/*****************************************************************************
 * Function Name  : deleteUEPeriodicReportBatchInfo 
 * Inputs         : ueIndex, internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called when a existing UE is deleted 
 *                  from global UE context variable. Function removes deleted UE 
 *                  from batch in which UE belongs to.
 *****************************************************************************/
MacRetType deleteUEPeriodicReportBatchInfo(UInt32 ueIndex, 
                                            InternalCellIndex internalCellIndex)
{   
    UInt32 ueCount = 0;
    UInt32 numOfUE = 0;
    UInt16 batchIndex = 0;

    if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                    periodicReportParams.periodicReportsRequired)
    { 
        batchIndex = rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            ueIDToBatchNumMapping[ueIndex];
        /* SPR 16583 fix start */
        if( MAX_NUM_OF_BATCH <= batchIndex )
        {
             /* UE is not created yet, hence batchIndex can be Invalid value.
              * This case can be during HO when DL UE context is created but
              * MAC_CREATE_UE_REQ is not received and hence UL UE context is
              * not created. */
             return MAC_SUCCESS;
        }
        /* SPR 16583 fix end */
        numOfUE = rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            periodicReportUEBatch[batchIndex].numOfUE;

        for (ueCount = 0; ueCount < numOfUE; ueCount++)
        {
            if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                    periodicReportUEBatch[batchIndex].ueID[ueCount] == ueIndex)
            {
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                    periodicReportUEBatch[batchIndex].ueID[ueCount] = 
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                    periodicReportUEBatch[batchIndex].ueID[numOfUE-1];
                rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                    periodicReportUEBatch[batchIndex].numOfUE--;
                break;
            }
        }


    }
 
    /* SPR 8104 */
/* TTIB_Code Start */

//CODE_COMMENT_L8_FIXED
     rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ulUEPeriodicReport. \
                /* + SPR 19724 */
        ulPeriodicReportInfo_p[ueIndex].uplinkPRB = 0xFFFFFFFF;
                /* - SPR 19724 */

    /* SPR 8104 */
/* TTIB_Code End */

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : reconfigPeriodicReport 
 * Inputs         : periodicReportConfig_p, internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called when a reconfiguration of periodic 
 *                  report is requested from RRM.
 *****************************************************************************/
    /*coverity-530 CID-32293*/
RrmMacCellConfigErrorCode reconfigPeriodicReport(
                            PeriodicReportConfig *periodicReportConfig_p,
                            InternalCellIndex  internalCellIndex)
    /*coverity-530 CID-32293*/
{ 
    UInt32  bitmask = periodicReportConfig_p->bitmask;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *dlUEContext_p  = PNULL;
    UInt8 count = 0;
    UInt16 totalCount = 0;
    UInt16 ueIndex = 0;
    PeriodicReportParameters periodicReportParams = 
        (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams);
    /*ca changes start*/
    CellConfigParams *cellParams_p = NULL;
    cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p \
                                        [internalCellIndex]->cellParams_p;
    InitParams *initParams_p = cellSpecificParams_g.\
                               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    /* ca changes end */

    /* Return if a valid bitmask is not set */
    if (!bitmask)
    {
        LOG_UT(MAC_RRM_INVALID_PERIODIC_REPORT_CONFIG_BITMASK,
                LOGDEBUG,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                bitmask,LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");


        return INVALID_PERIODIC_REPORT_CONFIG_BITMASK;
    }

    /* Set the corresponding fields depending on bitmask */
    if (bitmask & PERIODIC_REPORTS_REQUIRED_BITMASK)
    {
        if (periodicReportConfig_p->periodicReportRequired > 1)
        {
            LOG_UT(MAC_RRM_INVALID_REPORT_REQUIERED_VALUE,LOGDEBUG,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    periodicReportConfig_p->periodicReportRequired,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");

            return INVALID_REPORT_REQUIRED_VALUE;
        }

        periodicReportParams.periodicReportsRequired = 
            periodicReportConfig_p->periodicReportRequired;

    }

    if (periodicReportParams.periodicReportsRequired)
    {
        if (bitmask & PERIODICITY_BITMASK)
        {
            /* Validations :-
             * 1.) Should be >= 100 ms
             * 2.) Should be multiple of 100
             */
            if ((periodicReportConfig_p->periodicity >= MIN_PERIODICITY_PERIODIC_REPORT) &&
                    ((periodicReportConfig_p->periodicity % 100) == 0))
            {
                periodicReportParams.periodicity = periodicReportConfig_p->periodicity;
            }
            else
            {
                LOG_UT(MAC_RRM_INVALID_PERIODICITY,LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        periodicReportParams.periodicity,LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");


                return INVALID_PERIODICITY;
            }
        }
        else
        {
            if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams.periodicity == 0)
            {

                LOG_UT(MAC_RRM_PERIODICITY_VALUE_NOT_RECEIVED,LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams.periodicity,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");

                return PERIODICITY_VALUE_NOT_RECEIVED;
            }
        }

        if (bitmask & UE_BATCH_SIZE_BITMASK)
        {
              /* spr 5448 changes start */

                if ((periodicReportConfig_p->ueBatchSize < MIN_UE_BATCH_SIZE) ||
                  (periodicReportConfig_p->ueBatchSize > MAX_UE_BATCH_SIZE))
                {
                    LOG_UT(MAC_RRM_INVALID_UE_BATCH_SIZE,LOGDEBUG,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    periodicReportConfig_p->ueBatchSize,LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");

                    return INVALID_UE_BATCH_SIZE;
                  }
               /* spr 5448 changes end */

            /* ueBatchSize can be reconfigured only if RRM diable Periodic Report first
             * and then enable it with new ueBacthSize. In case report is disabled, 
             * ueBatchSize becomes 0. Hence below check is with 0 */

            if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                                    periodicReportParams.ueBatchSize != 0)
            {
                LOG_UT(MAC_RRM_BATCH_SIZE_RECONFIG_NOT_ALLOWED,
                        LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                        periodicReportParams.ueBatchSize,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");

              

                if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                    periodicReportParams.ueBatchSize != 
                        periodicReportConfig_p->ueBatchSize)
                {
                    return  BATCH_SIZE_RECONFIG_NOT_ALLOWED;
                }
            }

            if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                    periodicReportParams.ueBatchSize != 
                        periodicReportConfig_p->ueBatchSize)
            {
                /* Validations :-
                 * 1.) Max Value is 25
                 * 2.) Min Value is 1
                 */
                /* spr 5448 changes start */

                    periodicReportParams.ueBatchSize = periodicReportConfig_p->ueBatchSize;
                    /* SPR 8509 Fix Start */
                    periodicReportParams.numOfBatches   = 
                        initParams_p->maxNumOfUeSupported / periodicReportParams.ueBatchSize;

                    if(initParams_p->maxNumOfUeSupported % periodicReportParams.ueBatchSize)
                    {
                        periodicReportParams.numOfBatches++;
                    }
                    /* SPR 8509 Fix End */
                /* spr 5448 changes end */
            }
        }
        else
        {
            if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                                    periodicReportParams.ueBatchSize == 0)
            {

                LOG_UT(MAC_RRM_UE_BATCH_SIZE_VALUE_NOT_RECEIVED,LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                        periodicReportParams.ueBatchSize,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");


                return UE_BATCH_SIZE_VALUE_NOT_RECEIVED;
            }
        }

        periodicReportParams.batchInterval  = 
            periodicReportParams.periodicity / periodicReportParams.numOfBatches;

        if(periodicReportParams.periodicity % periodicReportParams.numOfBatches)
        {
            periodicReportParams.batchInterval++;
        }
        /* Next periodic Report will be send after batchInterval */
        /* SPR 8509 Fix Start */
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextPeriodicReportOppor = 
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex) + 
                      + periodicReportParams.batchInterval;  
        /* SPR 8509 Fix End */

        /* In case periodic report was disabled earlier then only we need to update
         * batch info */
        if(!rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                            periodicReportParams.periodicReportsRequired)
        {
            /* Update numOfBatches, batchInterval in periodicReportParams */
            for(ueIndex = 0;ueIndex < MAX_UE_SUPPORTED && 
                    totalCount < cellParams_p->totalActiveUE; ueIndex++)
            {
                dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
                dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

                if(!dlUEContextInfo_p->pendingDeleteFlag && dlUEContext_p)
                {
                    count = rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportUEBatch \
                        [rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->updateUEBatchCurrentIndex].numOfUE ;
                    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportUEBatch[rrmMacPeriodicMgrCellGlobal_gp
                        [internalCellIndex]->updateUEBatchCurrentIndex].ueID[count] = ueIndex;
                    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportUEBatch \
                        [rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->updateUEBatchCurrentIndex].numOfUE++;

                    ++totalCount;
                    /* updating ueId to batchId map */
                    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ueIDToBatchNumMapping[ueIndex] = 
                            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->updateUEBatchCurrentIndex;

                    ++rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->updateUEBatchCurrentIndex;
                    if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->updateUEBatchCurrentIndex == 
                                    periodicReportParams.numOfBatches)
                    {
                        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->updateUEBatchCurrentIndex = 0;
                    }
                }
            }
        }
    }

    else
    {
        initReInitPeriodicReportManager(internalCellIndex);
    }
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams = periodicReportParams;

    /* Log for periodic report parameters */
    LOG_UT(MAC_RRM_PERIODIC_REPORT_PARAMS_1,LOGDEBUG,MAC_RRM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            periodicReportParams.periodicReportsRequired,
            periodicReportParams.periodicity,
            periodicReportParams.ueBatchSize,
            periodicReportParams.numOfBatches,
            periodicReportParams.batchInterval,
            (UDouble32)LINE_NUMBER,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"");

    /*coverity-530 CID-32293*/
    return RRM_MAC_SUCCESS;
    /*coverity-530 CID-32293*/
}

/***************************************************************************
 * Function Name: configureLayer1Report
 * Inputs       : layer1MeasReportConfig_p, internalCellIndex
 * Returns      : RRM_MAC_SUCCESS/RRM_MAC_FAILURE
 * Description  : This function is called when a configuration of interference
 *                related information (like thermal noise & SNR reports)
 *                is requested from RRM
 * *************************************************************************/
    /*coverity-530 CID-32291*/
RrmMacCellConfigErrorCode configureLayer1Report(Layer1MeasReportConfig *layer1MeasReportConfig_p,
                                        InternalCellIndex internalCellIndex)
    /*coverity-530 CID-32291*/
{
    UInt32 bitmask = layer1MeasReportConfig_p->bitmask;
    Layer1ReportConfigParams layer1ReportConfigParams = {0}; 

    /* Return if valid bitmask is not set */
    if(!bitmask)
    {
        return INVALID_L1_MEAS_REPORT_CONFIG_BITMASK;
    }

    if(bitmask & L1_MEAS_REPORT_REQD_BITMASK)
    {
        if(layer1MeasReportConfig_p->layer1MeasReportRequired > 1)
        {
            return INVALID_REPORT_REQUIRED_VALUE;
        }
        layer1ReportConfigParams.layer1MeasReportRequired =
            layer1MeasReportConfig_p->layer1MeasReportRequired;
    }
    else
    {
        return INVALID_L1_MEAS_REPORT_CONFIG_BITMASK;
    }


    /* On the basis of requirement of reports we will configure periodicity */

    if(layer1ReportConfigParams.layer1MeasReportRequired)
    {
        if(bitmask & L1_MEAS_REPORT_PERIODICITY_BITMASK)
        {
            /* Validation :- Should be >= 20 ms */
            if(layer1MeasReportConfig_p->periodicity >= MIN_PERIODICITY)
            {
                layer1ReportConfigParams.periodicity =
                    layer1MeasReportConfig_p->periodicity;
            }
            else
            {
                return INVALID_PERIODICITY;
            }
        }
        else
        {
            return PERIODICITY_VALUE_NOT_RECEIVED;
        }

        if(bitmask & L1_MEAS_REPORT_AVERAGING_ENABLED_BITMASK)
        {
            if(layer1MeasReportConfig_p->layer1ReportAveragingEnabled>1)
            {
                return INVALID_REPORT_REQUIRED_VALUE;
            }
            layer1ReportConfigParams.layer1ReportAveragingEnabled =
                layer1MeasReportConfig_p->layer1ReportAveragingEnabled;
        }

        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextL1ReportOppor = 
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex) + layer1ReportConfigParams.periodicity;
    }

    /* Making L1 report configuration to global so that it can be used 
     * in order to send L1 report indication to RRM , based on configuration */
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportConfigParams = 
                layer1ReportConfigParams;

    /*coverity-530 CID-32291*/
    return RRM_MAC_SUCCESS;
    /*coverity-530 CID-32291*/
}

/***************************************************************************
 * Function Name: configureEICICReport 
 * Inputs       : layer2EicicMeasReportConfig_p - pointer to structure
 *                Layer2EicicMeasReportConfig,
 * 	              internalCellIndex
 * Returns      : RRM_MAC_SUCCESS/RRM_MAC_FAILURE
 * Description  : This function is called when a configuration of EICIC
 *                related information (like ABSPRB Usage) is requested from RRM
 *
 * *************************************************************************/
/*coverity-530 CID-32292*/
RrmMacCellConfigErrorCode configureEICICReport(Layer2EicicMeasReportConfig *layer2EcicMeasReportConfig_p,
        InternalCellIndex internalCellIndex)
/*coverity-530 CID-32292*/
{

    EICICReportConfigParams eicicReportConfigParams = {0};

    if (layer2EcicMeasReportConfig_p->absUsageRequired > 1)
    {
        return INVALID_L2_EICIC_MEAS_REPORT_CONFIG;
    } 

    eicicReportConfigParams.absUsageRequired =
        layer2EcicMeasReportConfig_p->absUsageRequired;


    /* On the basis of requirement of reports we will configure periodicity */
    if(eicicReportConfigParams.absUsageRequired)
    {

        /* Validation :- Should be >= 20 ms */
        if(layer2EcicMeasReportConfig_p->periodicity >= MIN_PERIODICITY)
        {
            eicicReportConfigParams.periodicity =
                layer2EcicMeasReportConfig_p->periodicity;
        }
        else
        {
            return INVALID_PERIODICITY;
        }
    }

    /* Next periodic Report will be send after next periodicity value */    
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextEICICReportOppor = 
        GET_CURRENT_TICK_FOR_CELL(internalCellIndex)+eicicReportConfigParams.periodicity;


    /* Making EICIC report configuration to global so that it can be used in 
     * order to send EICIC report indication to RRM,based on configuration */
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportConfigParams = eicicReportConfigParams;

    /*coverity-530 CID-32292*/
    return RRM_MAC_SUCCESS;
    /*coverity-530 CID-32292*/

}
/***************************************************************************
 * Function Name: configureICICReport 
 * Inputs       : layer2IcicMeasReportConfig_p - pointer to structure
 *                Layer2IcicMeasReportConfig,
 * 	              internalCellIndex
 * Returns      : RRM_MAC_SUCCESS/RRM_MAC_FAILURE
 * Description  : This function is called when a configuration of ICIC
 *                related information (like RNTP reports
 *                DL & UL CE PRB usage required) is requested from RRM
 *
 * *************************************************************************/
    /*coverity-530 CID-32292*/
RrmMacCellConfigErrorCode configureICICReport(Layer2IcicMeasReportConfig *layer2IcicMeasReportConfig_p,
                                                InternalCellIndex internalCellIndex)
    /*coverity-530 CID-32292*/
{
    UInt32 bitmask = layer2IcicMeasReportConfig_p->bitmask;
    UInt32 rntpBitmask = layer2IcicMeasReportConfig_p->rntpReportingConfigInfo.bitmask;
    ICICReportConfigParams icicReportConfigParams = {0};

    /* Return if valid bitmask is not set */
    if(!bitmask)
    {
        return INVALID_L2_ICIC_MEAS_REPORT_CONFIG_BITMASK;
    }

    if(bitmask & CE_PRB_USAGE_REQD_BITMASK)
    {
        if(layer2IcicMeasReportConfig_p->cellEdgePrbUsageRequired > 1)
        {
            return INVALID_REPORT_REQUIRED_VALUE;
        }
        icicReportConfigParams.cellEdgePrbUsageRequired =
            layer2IcicMeasReportConfig_p->cellEdgePrbUsageRequired;
    }

    /* Depending upon RNTP bitmask value , we will configure RNTP related parameters */
    if(RNTP_CONFIG_BITMASK & bitmask)
    {
        if(!rntpBitmask)
        {
            return INVALID_RNTP_REPORT_CONFIG_BITMASK;
        }
        
        icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired =
            layer2IcicMeasReportConfig_p->rntpReportingConfigInfo.rntpReportRequired;
        
        /* Reconfigure the threshold value only if RNTP report is required */
        if(layer2IcicMeasReportConfig_p->rntpReportingConfigInfo.rntpReportRequired)
        {
            if(RNTP_THRESHOLD_BITMASK & rntpBitmask)
            {
                /* Check for RNTP threshold value range*/
                if(layer2IcicMeasReportConfig_p->rntpReportingConfigInfo.rntpThreshold <= 
                         RNTP_THRESHOLD_MAX_VAL)
                {
                    /* RNTP threshold range : enumerated {-INFINITY,-11,-10...3} */
                    icicReportConfigParams.rntpReportingConfigParams.rntpThreshold = 
                        rntpThresholdMap_g[layer2IcicMeasReportConfig_p->rntpReportingConfigInfo.rntpThreshold];
                }
                else
                {
                    return INVALID_RNTP_THRESHOLD_VALUE;
                }
            }
            else
            {
                return INVALID_RNTP_THRESHOLD_VALUE;
            }
            /* Reconfiguring EPRE value as per bitmask */
            if(MAX_NOMINAL_EPRE_BITMASK & rntpBitmask)
            {
                icicReportConfigParams.rntpReportingConfigParams.maxNominalEPRE =
                    layer2IcicMeasReportConfig_p->rntpReportingConfigInfo.maxNominalEPRE;
            }
            else
            {
                return INVALID_EPRE_BITMASK;
            }
        }
    }

    /* On the basis of requirement of reports we will configure periodicity */
    if(icicReportConfigParams.cellEdgePrbUsageRequired ||
            icicReportConfigParams.rntpReportingConfigParams.rntpReportRequired)
    {
        if(bitmask & L2_ICIC_MEAS_REPORT_PERIODICITY_BITMASK)
        {
            /* Validation :- Should be >= 20 ms */
            if(layer2IcicMeasReportConfig_p->periodicity >= MIN_PERIODICITY)
            {
                icicReportConfigParams.periodicity =
                    layer2IcicMeasReportConfig_p->periodicity;
            }
            else
            {
                return INVALID_PERIODICITY;
            }
        }
        else
        {
            return PERIODICITY_VALUE_NOT_RECEIVED;
        }

        /* Next periodic Report will be send after next periodicity value */    
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextICICReportOppor = 
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex)+icicReportConfigParams.periodicity;
    }

    /* Making ICIC report configuration to global so that it can be used in 
     * order to send ICIC report indication to RRM,based on configuration */
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams = icicReportConfigParams;

    /*coverity-530 CID-32292*/
    return RRM_MAC_SUCCESS;
    /*coverity-530 CID-32292*/

}

/*****************************************************************************
 * Function Name  : initReInitPeriodicReportManager 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to reset configuration related
 *                  periodic report manager
 *
 * ***************************************************************************/

void initReInitPeriodicReportManager(InternalCellIndex  internalCellIndex)
{   
    UInt32 ueIndex = 0;

    memSet(&rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportUEBatch,0, \
            sizeof(PeriodicReportUEBatch)*MAX_NUM_OF_BATCH);
    memSet(&rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ueIDToBatchNumMapping, \
            MAX_UE_SUPPORTED,sizeof(UInt32)*MAX_UE_SUPPORTED);
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportCurrentBatchIndex = 0;
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->updateUEBatchCurrentIndex = 0;
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams.ueBatchSize = 0;
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams.periodicity = 0;
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->dlUEPeriodicReport. \
            dlPeriodicReportInfo_p,0,sizeof(DLPeriodicReportInfo) * MAX_UE_SUPPORTED);
    memSet(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ulUEPeriodicReport. \
            ulPeriodicReportInfo_p,0,sizeof(ULPeriodicReportInfo) * MAX_UE_SUPPORTED);
    /* TTIB_Code Start */
    /* SPR 8104 */
    for ( ;ueIndex < MAX_UE_SUPPORTED ; ueIndex++)
    {
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->ulUEPeriodicReport. \
                /* + SPR 19724 */
            ulPeriodicReportInfo_p[ueIndex].uplinkPRB = 0xFFFFFFFF;
                /* - SPR 19724 */
    }
    /* SPR 8104 */
/* TTIB_Code End */
    periodicReportFd_g  = initHptoLpIndicationQueue();

    if ( periodicReportFd_g < 0)
    {
        /* SPR 7385 Changes Starts */
        ltePanic("mq_open failed for HP to LP indication queue"); 
        /* SPR 7385 Changes Ends */
    }
    else
    {
        setMsgQFd( periodicReportFd_g );
    }
}


/*****************************************************************************
 * Function Name  : checkForRRMPerriodicReportOpp() 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to check the periodic report 
 *                  opportuinity
 * ***************************************************************************/
void checkForRRMPerriodicReportOpp(InternalCellIndex internalCellIndex)
{
    /*ca changes start*/
    HpLpPeriodicReportInd buffer;
    /*Initialize the buffer*/
    buffer.internalCellIndex = INVALID_CELL_INDEX;
    buffer.periodicReportTriggerType = 0;
    /*ca changes end*/
    if (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->periodicReportParams.periodicReportsRequired && \
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex) >= rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextPeriodicReportOppor)
    {
        /*ca changes start*/
        buffer.internalCellIndex = internalCellIndex;
        /* SPR 21201 start */
        buffer.periodicReportTriggerType |= TRIGGER_PERIODIC_REPORT;
        /* SPR 21201 end   */
        /*ca changes end*/
    }
    
    /* +DYNAMIC_ICIC */
    if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->layer1ReportConfigParams.layer1MeasReportRequired && \
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex) >= rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextL1ReportOppor)
    {
        /*ca changes start*/
        buffer.internalCellIndex = internalCellIndex;
        /* SPR 21201 start */
        buffer.periodicReportTriggerType |= TRIGGER_L1_MEAS_REPORT;
        /* SPR 21201 end   */
        /*ca changes end*/
    }
    if(((rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.cellEdgePrbUsageRequired) ||
        (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams. \
            rntpReportingConfigParams.rntpReportRequired)) && (GET_CURRENT_TICK_FOR_CELL(internalCellIndex) >= 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextICICReportOppor))
    {
        /* change active report index so that high priority thread start writing on 
         * changed array index. Low priority thread will read from passive index and
         * send report to RRM
         */
       /* SPR 21199 fix start */
        /* Incrementing the global for next ICIC report opportunity */
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextICICReportOppor =
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextICICReportOppor +
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportConfigParams.periodicity;
        /* SPR 21199 fix end */
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndex = 
            !rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndex;
        /*ca changes start*/
        buffer.internalCellIndex = internalCellIndex;
        /* SPR 21201 start */
        buffer.periodicReportTriggerType |= TRIGGER_L2_ICIC_MEAS_REPORT;
        /* SPR 21201 end   */
        /*ca changes end*/
    }


   if((rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportConfigParams.absUsageRequired) &&
        (GET_CURRENT_TICK_FOR_CELL(internalCellIndex) >= 
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextEICICReportOppor))
    {
        /* change active report index so that high priority thread start writing on 
         * changed array index. Low priority thread will read from passive index and
         * send report to RRM
         */
       /* SPR 21199 fix start */
        /* Incrementing the global for next eICIC report opportunity */
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextEICICReportOppor=
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->nextEICICReportOppor+
            rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportConfigParams.periodicity;
        /* SPR 21199 fix end */

#ifdef FDD_CONFIG
	    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndexEicic = 
		    !rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndexEicic;
#endif
	    /*ca changes start*/
        buffer.internalCellIndex = internalCellIndex;
        /* SPR 21201 start   */
        buffer.periodicReportTriggerType |= TRIGGER_L2_EICIC_MEAS_REPORT;
        /* SPR 21201 end   */
        /*ca changes end*/
    }
    
    /* SPR 21201 start */
    if(buffer.periodicReportTriggerType)
    {
        lteMessageQSend(periodicReportFd_g,(UInt8 *)&buffer,sizeof(HpLpPeriodicReportInd));
    }
    /* SPR 21201 end   */ 
    /* -DYNAMIC_ICIC */
}

/* +DYNAMIC_ICIC */
/*****************************************************************************
 * Function Name  : validateULPowerMask 
 * Inputs         : ulPowerMask_p
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called to validate UL Power Mask config.
 *                  
 * ***************************************************************************/
MacRetType validateULPowerMask(ULPowerMask *ulPowerMask_p)
{
    UInt8 count = 0;
    
    if((ulPowerMask_p->cellCenterUserPowerMask > MAX_PC_MAX)||
            (ulPowerMask_p->cellCenterUserPowerMask) < MIN_PC_MAX)
    {
        return MAC_FAILURE;
    }
    if((ulPowerMask_p->cellEdgeUserPowerMask > MAX_PC_MAX)||
            (ulPowerMask_p->cellEdgeUserPowerMask < MIN_PC_MAX))
    {
        return MAC_FAILURE;
    }

    for(;count<MAX_QCI;++count)
    {
        if((ulPowerMask_p->qciDeltaPowerMask[count] > MAX_PC_MAX)||
            (ulPowerMask_p->qciDeltaPowerMask[count] < MIN_PC_MAX))
            {
                return MAC_FAILURE;
            }
    }
    return MAC_SUCCESS;
}
/* -DYNAMIC_ICIC */

/*****************************************************************************
 * Function Name  : macInterfaceProcessRRMPeriodicReports 
 * Inputs         : buffer_p - pointer to buffer
 * Outputs        : None
 * Returns        : None 
 * Description    : This function is called to process the periodic reports 
                    for RRM.
 * ***************************************************************************/
void macInterfaceProcessRRMPeriodicReports(UInt8 *buffer_p)
{
    HpLpPeriodicReportInd *hpLpPeriodicReport_p = (HpLpPeriodicReportInd *) buffer_p;

    /* SPR 21201 start */   
    if (TRIGGER_PERIODIC_REPORT & hpLpPeriodicReport_p->periodicReportTriggerType)
    /* SPR 21201 end   */    
    {
        processPeriodicReport(hpLpPeriodicReport_p->internalCellIndex);
    }
    
    /* SPR 21201 start */
    if (TRIGGER_L1_MEAS_REPORT & hpLpPeriodicReport_p->periodicReportTriggerType)
    /* SPR 21201 end   */    
    {
        processLayer1Report(hpLpPeriodicReport_p->internalCellIndex);
    }
    
    /* SPR 21201 start */
    if (TRIGGER_L2_ICIC_MEAS_REPORT & hpLpPeriodicReport_p->periodicReportTriggerType)
    /* SPR 21201 end   */    
    {
        processICICReport(hpLpPeriodicReport_p->internalCellIndex);
    }
    
    /* EICIC Change Phase1 */
    /* SPR 21201 start */
    if(TRIGGER_L2_EICIC_MEAS_REPORT & hpLpPeriodicReport_p->periodicReportTriggerType)    
    /* SPR 21201 end   */    
    {
        processEICICReport(hpLpPeriodicReport_p->internalCellIndex);
    }
    /* EICIC Change Phase1 */	
}

/** ********EICIC Change Start ******/
/************************************************************************
 * Function Name : processEICICReport
 * Inputs        : None
 * Outputs       : None
 * Returns       : None
 * Description   : This function process the EICIC Report based on 
 *                 oppurnity and sends message to RRM
 * *********************************************************************/
void processEICICReport(InternalCellIndex internalCellIndex)
{
    MacRrMEICICAbsReportInd *eicicReportInd_p=NULL;

    UInt8 msgSize=0;
    UInt8 dlAvailableRBs=0;
    UInt8 cqi = 0;
    UInt8 percentagCEPrb=0;
    UInt8 percentagCCPrb=0;

    dlAvailableRBs=cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->dlAvailableRBs;	

    L2RrmMsg *l2RrmMsg_p = PNULL;
    l2RrmMsg_p = (L2RrmMsg *)rrmMacPeriodicMgrCellGlobal_gp \
        [internalCellIndex]->bufferForEICICReport_p;	
#ifdef TDD_CONFIG 
    /* IN TDD Low priority thread does not exist and Reports are sent in 
     * high priority thread. Hence changing active index in HP thread in
     * case of TDD as it is done in FDD 
     */
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndexEicic = 
        !rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndexEicic;
#endif
    /* SPR 5983 Fix End */

    /* L2 is writing on activeReportIndex array index, and low priroity
     * thread reads from complement index */
    UInt8 passiveIndex = !rrmMacPeriodicMgrCellGlobal_gp \
        [internalCellIndex]->activeReportIndexEicic;

    /* SPR 21199 fix start */
    /* if clause removed */
    /* calculation of nextEICICReportOppor is moved to HP thread */
    /* SPR 21199 fix end */
    eicicReportInd_p = (MacRrMEICICAbsReportInd*)l2RrmMsg_p->data;
    if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportConfigParams.absUsageRequired)
    {
        for (cqi = 0; cqi < MAX_QCI; cqi++)
        {

            if (0 != ((rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].numABSoccasionsKpi)*(dlAvailableRBs)))
            {
                eicicReportInd_p->absPerQci[cqi] = (( (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].absPerQci[cqi])*100)/(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].numABSoccasionsKpi*dlAvailableRBs));   
            }
        }  

        if(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
                icicReportConfigParams.cellEdgePrbUsageRequired)
        {

            if (0!= rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].totalCCPrb)
            {
                percentagCCPrb= 
                    (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].ccPRBUsage*100)/\
                    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].totalCCPrb;
            }
            if(0!=   rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].totalCEPrb)
            {
                percentagCEPrb= 
                    (rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].cePRBUsage*100)/\
                    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].totalCEPrb;	
            }

            eicicReportInd_p->absPrbUsage =((percentagCCPrb>percentagCEPrb)?percentagCCPrb:percentagCEPrb);

        }
        else if(LTE_FALSE!=rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].numABSoccasionsKpi)
        {
            eicicReportInd_p->absPrbUsage = 
                ( rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].absPRBUsage*100)\
                /(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportInd.dlEICICReportInfo_p[passiveIndex].numABSoccasionsKpi*dlAvailableRBs);
        }
        else
        {
            eicicReportInd_p->absPrbUsage = 0;
            for (cqi = 0; cqi < MAX_QCI; cqi++)
            {
                eicicReportInd_p->absPerQci[cqi] = 0;
            }
        }
    } 

    /* Calculating the total message lenth */
    msgSize = sizeof(MacRrMEICICAbsReportInd) + L2_RRM_API_HEADER_LEN;


        
    /* SPR 21199 fix start */
    /* calculation of nextICICReportOppor is moved to fuction
     * checkForRRMPerriodicReportOpp */
    /* SPR 21199 fix end */

    /* Prepares API headers for sending EICIC report indication to RRM with appropriate msgSize */
    prepareMACRRMApiHeader(l2RrmMsg_p, RRM_MAC_EICIC_REPORT_IND,0,msgSize,internalCellIndex);

    /* Sending message to RRM */
    if(MAC_SEND_ERROR == sendMsgToRRM(rrmMacPeriodicMgrCellGlobal_gp \
                [internalCellIndex]->bufferForEICICReport_p, msgSize
                ))
    {
        /*Log*/
    } 
    memSet(&rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]-> \
            eicicReportInd.dlEICICReportInfo_p[passiveIndex],0,
            sizeof(DLEICICReportInfo));   
}
/** ********EICIC Change End******/
