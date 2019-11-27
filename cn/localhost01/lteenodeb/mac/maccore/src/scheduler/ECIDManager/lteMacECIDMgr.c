
/****************************************************************************
 * *
 * *  ARICENT -
 * *
 * *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 * *
 * ****************************************************************************
 * *
 * *  $Id: lteMacECIDMgr.c,v $
 * *
 * ****************************************************************************
 * *
 * *  File Description : This file contains all the state machine
 * functions and handler functions that are required by the ECID manager
 * ****************************************************************************
 * *
 * * Revision Details
 * * ----------------
 * * $Log: lteMacECIDMgr.c,v $
 * ***************************************************************************/

/******************************************************************************
 *  * Standard Library Includes
 *  **************************************************************************/

/******************************************************************************
 *  * Project Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacULUEContext.h"
#include "lteL2RRMInterface.h"
#include "lteMacRRMIntfInfo.h"
#include "lteMacRRMInterface.h"
#include "lteMacECIDMgr.h"
#include "logging.h"
#include "lteMacRandomAccess.h"
#include "lteMacReceiver.h"
/******************************************************************************
 *  Private Definitions
 *****************************************************************************/

/******************************************************************************
 *  Private Types
 *****************************************************************************/
/*****************************************************************************
 ** Function Prototypes
 *****************************************************************************/
/******************************************************************************
 *  * Private Constants
 ******************************************************************************/
/*CA Changes start  */
E_CID_REPORT_SEND_Q* eCidReportsQueue_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */
/******************************************************************************
 *  * Exported Variables
 ******************************************************************************/

/*****************************************************************************
 *  * Private Variables 
 *****************************************************************************/
/*CA Changes start  */
LTE_LIST* dummyDCI0SendList_gp[MAX_NUM_CELL] = {PNULL};
LTE_LIST* dummyDCI0SendFailureList_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */
extern UInt8 phyReleaseCapability_g;
extern SInt32 sendMsgToRRM(UInt8 *buffer,UInt16 length
        );
static void sendResponseToRRM(UInt8 responseCode,UInt8 requiredReports, ULUEContext *ulUEContext_p,
                              /*CA Changes start  */
                              InternalCellIndex internalCellIndex);
                              /*CA Changes end  */

/******************************************************************************
 * Private Functions
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : initECidMgr
 * Inputs         : numCellsConfigured -  Number of cells configured
 * Outputs        : None
 * Returns        : None
 * Description    : This function Initialize Queues of E-CID manager.                 
 *****************************************************************************/
/* CA Changes start */
void initECidMgr (UInt8 numCellsConfigured)
{
    InternalCellIndex cellIdx = 0;

    CONSTRUCT_AND_INIT_E_CID_REPORT_Q(numCellsConfigured)

    createMemPool(sizeof(LTE_LIST), numCellsConfigured * 2);
    for (cellIdx = 0; cellIdx < numCellsConfigured; cellIdx++)
    {
        GET_MEM_FROM_POOL(LTE_LIST, dummyDCI0SendList_gp[cellIdx],sizeof(LTE_LIST), PNULL);
        listInit(dummyDCI0SendList_gp[cellIdx]);

        GET_MEM_FROM_POOL(LTE_LIST, dummyDCI0SendFailureList_gp[cellIdx],sizeof(LTE_LIST), PNULL);
        listInit(dummyDCI0SendFailureList_gp[cellIdx]);

        /* Memory handling changes start */
        createMemPool(sizeof(DummyDCI0Node),
                  cellSpecificParams_g.cellConfigAndInitParams_p[cellIdx]->initParams_p->ulSchdConfig.maxUEsToBeScheduledUL);
        /* Memory handling changes end */
    }
}
/* CA Changes end */

/*****************************************************************************
 * Function Name  : processECidMeasReportReq
 * Inputs         : rrmMacEcidMeasConfigReq_p - Buffer from RRM,
 *                  transactionId - transactionId 
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : Parse and process RRM_MAC_E_CID_MEAS_CONFIG_REQ 
 * Returns        : None
 * Description    : This function will process the E-CID measurement config info.
 *****************************************************************************/
void processECidMeasReportReq(RrmMacEcidMeasConfigReq *rrmMacEcidMeasConfigReq_p,
                              /*CA Changes start  */
                              UInt16 transactionId, InternalCellIndex internalCellIndex)
                              /*CA Changes end  */
{
    UInt16 ueIndex = INVALID_UE_INDEX;
    /* + Coverity 40853, 40854, 41641 */
    ReportTypesRequired typeofReportsRequired = NO_REPORT_TYPE_REQUIRED;
    /* - Coverity 40853, 40854, 41641 */
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    DummyDCI0Node *nodeEntry_p = PNULL;    
    UInt8 eCidMeasId = 0;

    ueIndex = rrmMacEcidMeasConfigReq_p->ueIndex;
    MacRrmEcidMeasurementResp *macRrmEcidMeasurementResp_p = PNULL;
    SInt16 current             = 0;
    L2RrmMsg *l2RrmMsgCnf_p = PNULL;

    typeofReportsRequired |= rrmMacEcidMeasConfigReq_p->type1_eNBRxTxRequested;
    typeofReportsRequired |= (rrmMacEcidMeasConfigReq_p->type2_eNBRxTxRequested << 1);
    typeofReportsRequired |= (rrmMacEcidMeasConfigReq_p->AoARequested << 2);
    eCidMeasId = rrmMacEcidMeasConfigReq_p->eCidMeasId;

    ulUEContext_p  = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    dlUEContext_p  = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    if((PNULL != ulUEContext_p && PNULL != dlUEContext_p)&& \
            (FALSE == ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && \
            ((MIN_VALUE_ECID_MEAS_ID <= eCidMeasId) &&(MAX_VALUE_ECID_MEAS_ID >= eCidMeasId)))
    {
        ulUEContext_p->eCidReportParams.eCidTransactionId = transactionId;
        ulUEContext_p->eCidReportParams.eCidMeasId = eCidMeasId;
        /*Adding Checks*/
#ifdef FAPI_4_0_COMPLIANCE
        if(phyReleaseCapability_g == RELEASE_8 )
        {
            sendResponseToRRM(MAC_FAILURE,NO_REPORT_TYPE_REQUIRED,ulUEContext_p,internalCellIndex);
            return;
        }

#endif
        /*Adding Checks*/

        /* + SPR_8082 */
        switch(typeofReportsRequired)
        {
            case AOA_REPORTS_REQUIRED:
                {
                    if(RELEASE_9 <= ulUEContext_p->ueComplianceRelease) 
                    {
                        /* CHANGE_FOR_TA1_START */
                        ulUEContext_p->eCidReportParams.rrmReqRecvTick = getCurrentTick(); 
                        /* CHANGE_FOR_TA1_START */
                        GET_MEM_FROM_POOL(DummyDCI0Node, nodeEntry_p, sizeof(DummyDCI0Node), PNULL);
                        /* + Coverity 40848 */
                        if( PNULL != nodeEntry_p )
                        {
                            nodeEntry_p->ueIndex = ueIndex;
                            nodeEntry_p->dummyDCI0ProcessTTI = 
                                ulUEContext_p->eCidReportParams.rrmReqRecvTick + MAX_WAIT_DCI0_TTI;
                            nodeEntry_p->retryCounter = 0;
                            /*CA Changes start  */
                            listInsertNode(dummyDCI0SendList_gp[internalCellIndex],&(nodeEntry_p->nodeAnchor));
                            /*CA Changes end  */

                            ulUEContext_p->eCidReportParams.typeofReportsRequired = 
                                typeofReportsRequired;
                            ulUEContext_p->eCidReportParams.reportStatus = 
                                WAIT_FOR_REPORTS_TA_1_OR_AOA; 
                            ulUEContext_p->eCidReportParams.dummyDCI0Entry_p = 
                                &(nodeEntry_p->nodeAnchor);
                            /* SPR_8672_FIX */
                            ulUEContext_p->eCidReportParams.dummyDCI0PresentFlag = PRESENT_IN_LIST;
                            /* SPR_8672_FIX */
                        }
                        /* - Coverity 40848 */
                    }  
                    else
                    {
                        /*CA Changes start  */
                        sendResponseToRRM(MAC_FAILURE,NO_REPORT_TYPE_REQUIRED,ulUEContext_p,internalCellIndex);
                        /*CA Changes end  */
                    }

                }
                break;
            case TYPE_1_TA_AOA_REPORTS_REQUIRED:
            case TYPE_1_TA_REPORT_REQUIRED:
                {
                    if ((RELEASE_9 <= ulUEContext_p->ueComplianceRelease) || 
                            (rrmMacEcidMeasConfigReq_p->targetSFNForeNBRxTx  < ENB_RX_TX_TARGET_SFN_MAX_VALUE))
                    {
                        /* CHANGE_FOR_TA1_START */
                        ulUEContext_p->eCidReportParams.rrmReqRecvTick = (getCurrentTick() + 
                                (rrmMacEcidMeasConfigReq_p->targetSFNForeNBRxTx * 10));
                        /* CHANGE_FOR_TA1_START */
                        GET_MEM_FROM_POOL(DummyDCI0Node, nodeEntry_p, sizeof(DummyDCI0Node), PNULL);
                        /* + Coverity 40848 */
                        if( PNULL != nodeEntry_p )
                        {
                            nodeEntry_p->ueIndex = ueIndex;
                            nodeEntry_p->dummyDCI0ProcessTTI = 
                                ulUEContext_p->eCidReportParams.rrmReqRecvTick + MAX_WAIT_DCI0_TTI;
                            nodeEntry_p->retryCounter = 0;
                            /*CA Changes start  */
                            listInsertNode(dummyDCI0SendList_gp[internalCellIndex],&(nodeEntry_p->nodeAnchor));
                            /*CA Changes end  */

                            ulUEContext_p->eCidReportParams.typeofReportsRequired = 
                                typeofReportsRequired;
                            ulUEContext_p->eCidReportParams.reportStatus = 
                                WAIT_FOR_REPORTS_TA_1_OR_AOA; 
                            ulUEContext_p->eCidReportParams.dummyDCI0Entry_p = 
                                &(nodeEntry_p->nodeAnchor);
                            /* SPR_8672_FIX */
                            ulUEContext_p->eCidReportParams.dummyDCI0PresentFlag = PRESENT_IN_LIST;
                            /* SPR_8672_FIX */
                        }
                        /* - Coverity 40848 */
                    }  
                    else
                    {
                        /*CA Changes start  */
                        sendResponseToRRM(MAC_FAILURE,NO_REPORT_TYPE_REQUIRED,ulUEContext_p,internalCellIndex);
                        /*CA Changes end  */
                    }

                }
                break;
            case TYPE_2_TA_REPORT_REQUIRED:
            case TYPE_1_2_TA_REPORTS_REQUIRED:
            case TYPE_2_TA_AOA_REPORTS_REQUIRED:
            case TYPE_1_2_TA_AOA_REPORTS_REQUIRED:
                {

                    ulUEContext_p->eCidReportParams.typeofReportsRequired = 
                        typeofReportsRequired;
                    if(TYPE_2_TA_REPORT_REQUIRED != 
                            ulUEContext_p->eCidReportParams.typeofReportsRequired)
                    {
                        if(RELEASE_8 ==  ulUEContext_p->ueComplianceRelease)
                        {
                            ulUEContext_p->eCidReportParams.typeofReportsRequired = 
                                TYPE_2_TA_REPORT_REQUIRED;
                            ulUEContext_p->eCidReportParams.reportStatus = 
                                WAIT_FOR_TA_2_REPORT_ONLY_FOR_RELEASE_8_UE;
                        }
                        else
                        {
                            /* CHANGE_FOR_TA1_START */
                            if(typeofReportsRequired & TYPE_1_TA_REPORT_REQUIRED)
                            {
                                if (rrmMacEcidMeasConfigReq_p->targetSFNForeNBRxTx < 
                                        ENB_RX_TX_TARGET_SFN_MAX_VALUE)
                                {
                                    ulUEContext_p->eCidReportParams.rrmReqRecvTick = (getCurrentTick() +
                                            (rrmMacEcidMeasConfigReq_p->targetSFNForeNBRxTx * 10));   
                                    /* CHANGE_FOR_TA1_END */
                                    GET_MEM_FROM_POOL(DummyDCI0Node, nodeEntry_p, sizeof(DummyDCI0Node), PNULL);
                                    /* + Coverity 40848 */
                                    if( PNULL != nodeEntry_p )
                                    {
                                        nodeEntry_p->ueIndex = ueIndex;
                                        nodeEntry_p->dummyDCI0ProcessTTI = 
                                            ulUEContext_p->eCidReportParams.rrmReqRecvTick + MAX_WAIT_DCI0_TTI;
                                        nodeEntry_p->retryCounter = 0;
                                        /*CA Changes start  */
                                        listInsertNode(dummyDCI0SendList_gp[internalCellIndex],&(nodeEntry_p->nodeAnchor));
                                        /*CA Changes end  */
                                        ulUEContext_p->eCidReportParams.dummyDCI0Entry_p = 
                                            &(nodeEntry_p->nodeAnchor);
                                        /* SPR_8672_FIX */
                                        ulUEContext_p->eCidReportParams.dummyDCI0PresentFlag = PRESENT_IN_LIST;
                                        /* SPR_8672_FIX */
                                    }
                                    /* - Coverity 40848 */
                                } 
                                else 
                                {
                                    /*CA Changes start  */
                                    sendResponseToRRM(MAC_FAILURE,NO_REPORT_TYPE_REQUIRED,ulUEContext_p,internalCellIndex);
                                    /*CA Changes end  */
                                }  
                            }
                            else
                            {
                                ulUEContext_p->eCidReportParams.rrmReqRecvTick = getCurrentTick();
                            }
                            ulUEContext_p->eCidReportParams.reportStatus = 
                                WAIT_FOR_TA_2_REPORT;
                        }
                    }
                    if(!dlUEContext_p->ulSyncLossOrHOInfoInDL.pendingPDCCHOrder)
                    {
                        /*CA Changes start  */
                        /* CA_pahse2_ulSync_code Start */
                        if(IS_PCELL_OF_UE(dlUEContext_p ,internalCellIndex))
                        {
                        /* CA_pahse2_ulSync_code End */

                            putEntryInUlSyncLossOnOffTriggerQ
                               (ueIndex, UL_SYNC_LOSS_ON, internalCellIndex);
                            putEntryInRachDLStrategyQueue
                                (ueIndex, getCurrentTick(), internalCellIndex);
                            /*CA Changes end  */
                            dlUEContext_p->
                                ulSyncLossOrHOInfoInDL.pendingPDCCHOrder = FALSE;
                            dlUEContext_p->
                                ulSyncLossOrHOInfoInDL.ulSyncLossState = UL_SYNC_LOSS_ON;
                        
                        /* CA_pahse2_ulSync_code Start */
                        }
                        /* CA_pahse2_ulSync_code End */

#ifdef KPI_STATS
                        gMacStats.pStats->lteMacEcidMeasStats[ueIndex].numOfPdcchOrders++;
#endif
                        /* + SPR_8082 */
                    }
                }
                break;
            default :
                {
                    break;

                }
        }
    }
    else
    {
        /*Coverity 5.3.0 CID-54187 */
        /* + Coverity 32707 */
        UInt8 *cnfMsg_p = PNULL;
        cnfMsg_p = (UInt8 *)getMemFromPool(MAX_MSG_SIZE,NULL);
        if(PNULL == cnfMsg_p)
        {
            LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGERROR,MAC_RRM_INF,
                    getCurrentTick(), __LINE__,
                    sizeof(MAX_MSG_SIZE), 0,0,0,
                    0.0,0.0,__FILE__,__func__);
            return;
        }
        /* - Coverity 32707 */
        /* + Coverity 32708 */
        /*Coverity 5.3.0 CID-54187 */
        l2RrmMsgCnf_p = (L2RrmMsg *)cnfMsg_p;
        current += L2_RRM_API_HEADER_LEN;
        macRrmEcidMeasurementResp_p = (MacRrmEcidMeasurementResp *)l2RrmMsgCnf_p->data;
        /* - Coverity 32708 */
        macRrmEcidMeasurementResp_p->bitmask = 0;
        /* SPR 21039 fix start */
        macRrmEcidMeasurementResp_p->ueIndex = getRrcUeIdxFromUeIdx(ueIndex);
        /* SPR 21039 fix end */
        macRrmEcidMeasurementResp_p->responseCode = MAC_FAILURE;
        macRrmEcidMeasurementResp_p->eCidMeasId = eCidMeasId; 

        current += sizeof(MacRrmEcidMeasurementResp);
        /*CA Changes start  */
        prepareMACRRMApiHeader(l2RrmMsgCnf_p,MAC_RRM_E_CID_MEAS_REPORT_RESP,
                transactionId,current, internalCellIndex);    
        /*CA Changes end  */

        /* Send the response back to RRM */
        /* + Coverity 32708 */
        if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg_p, current
                    ))
        /* - Coverity 32708 */
        {
            LOG_MAC_ERROR( MAC_RRM_INF, "Unable to send msg to RRM!" );
        }
        LOG_MAC_MSG(MAC_RRM_ECID_REPORT_RES_INFO, LOGINFO, MAC_RRM_INF,
                getCurrentTick(),\
                ueIndex,MAC_FAILURE,DEFAULT_INT_VALUE,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,__func__,"");
        /*Coverity 5.3.0 CID-54187 */
        freeMemPool(cnfMsg_p);
        /*Coverity 5.3.0 CID-54187 */

    }
    LOG_MAC_MSG(MAC_RRM_ECID_REPORT_REQ_INFO, LOGINFO, MAC_RRM_INF,
            getCurrentTick(),\
            ueIndex,typeofReportsRequired,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,__func__,"");

}
/*****************************************************************************
 * Function Name  : processEcidReports
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function will process the E-CID measurement reports.
 *****************************************************************************/
/*CA Changes start  */
void processEcidReports(InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    /* +- SPR 18268 */
    UInt16 ueIndex = INVALID_UE_INDEX;
    /* +- SPR 18268 */
    UInt8 eCidReportsQueueCount = 0;
    UInt8 responseCode = 0;
    UInt8 requiredReports = 0;
    EcidReportNode *reportNode_p = PNULL;
    ULUEContext *ulUEContext_p = PNULL;
    /*CA Changes start  */
    eCidReportsQueueCount = COUNT_E_CID_REPORT_Q(internalCellIndex);
    /*CA Changes end  */

    while(eCidReportsQueueCount)
    {
        /*CA Changes start  */
        DEQUEUE_E_CID_REPORT_Q(EcidReportNode, (void *)&reportNode_p, internalCellIndex);
        /*CA Changes end  */
        /*++klockwork warning */
        if(reportNode_p)
        /*--klockwork warning */
        {
            ueIndex         = reportNode_p->ueIndex;
            responseCode    = reportNode_p->responseCode;
            requiredReports = reportNode_p->requiredReports;

            ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
            /* If pendingDeleteFlag = 0 then only send response to RRM  
             *  and delete the node.
             * If pendingDeleteFlag = 1 then just delete the node.
             */  
            if(!ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag)
            {
                /* CA changes start */
                sendResponseToRRM(responseCode,requiredReports,ulUEContext_p, internalCellIndex);
                /* CA changes end */
            } 
            FREE_MEM_NODE_E_CID_REPORT_Q(reportNode_p);
        }
        eCidReportsQueueCount--;
    }
}      
/*****************************************************************************
 * Function Name  : sendResponseToRRM
 * Inputs         : responseCode - Response to be sent to RRM,
 *                  requiredReports - Type of reports required,
 *                  ulUEContext_p - pointer to the UL context
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : Send response to RRM
 * Returns        : None
 * Description    : This function will send the ECID measurement reports to
 *                  RRM.
 *****************************************************************************/
static void sendResponseToRRM(UInt8 responseCode,
                              UInt8 requiredReports, 
                              ULUEContext *ulUEContext_p,
                              /*CA Changes start  */
                              InternalCellIndex internalCellIndex)
                              /*CA Changes end  */
{
    SInt16 current             = 0;
    UInt8 count = 0;
    L2RrmMsg *l2RrmMsgCnf_p = PNULL;
    EcidReportParams *eCidReportParams_p  = &ulUEContext_p->eCidReportParams;
    UInt16 transactionId = eCidReportParams_p->eCidTransactionId;
    MacRrmEcidMeasurementResp *macRrmEcidMeasurementResp_p = PNULL;
    /* + Coverity 32709, 32710 */
    UInt8 *cnfMsg_p = PNULL;
    cnfMsg_p = (UInt8 *)getMemFromPool(MAX_MSG_SIZE,NULL);
    if(PNULL == cnfMsg_p)
    {
       LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGERROR,MAC_RRM_INF,
                        getCurrentTick(), __LINE__,
                        sizeof(MAX_MSG_SIZE), 0,0,0,
                        0.0,0.0,__FILE__,__func__);
       return;
    }

    l2RrmMsgCnf_p = (L2RrmMsg *)cnfMsg_p;
    current += L2_RRM_API_HEADER_LEN;
    macRrmEcidMeasurementResp_p = (MacRrmEcidMeasurementResp *)l2RrmMsgCnf_p->data;
    /* - Coverity 32709, 32710 */

    /* SPR 21039 fix start */
    macRrmEcidMeasurementResp_p->ueIndex = getRrcUeIdxFromUeIdx(ulUEContext_p->ueIndex);
    /* SPR 21039 fix end */
    macRrmEcidMeasurementResp_p->responseCode = responseCode;
    macRrmEcidMeasurementResp_p->eCidMeasId = 
        ulUEContext_p->eCidReportParams.eCidMeasId;

    if(MAC_FAILURE == responseCode)
    {
        macRrmEcidMeasurementResp_p->bitmask = 0;

        LOG_MAC_MSG(MAC_RRM_ECID_REPORT_RES_INFO, LOGINFO, MAC_RRM_INF,
                getCurrentTick(),\
                ulUEContext_p->ueIndex,MAC_FAILURE,DEFAULT_INT_VALUE,\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,__func__,"");
    }
    else
    {
        switch(requiredReports)
        {
            case TYPE_1_TA_REPORT_REQUIRED:
            {
                /* SPR 9927 fix start */
                macRrmEcidMeasurementResp_p->bitmask |= TYPE_1_TA_REPORT_BITMASK;
                /* SPR 9927 fix end */

                macRrmEcidMeasurementResp_p->type1EnbRxTxReport.type1EnbRxTxDiffValue = 
                    eCidReportParams_p->type1TAValue; 
                macRrmEcidMeasurementResp_p->type1EnbRxTxReport.systemFrameNum =
                    (eCidReportParams_p->type1TaAOAReceivedTTI / MAX_SUB_FRAME) % MAX_SFN_VALUE;
                /* SPR_8672_FIX */
                /* CA Changes start  */
                CHECK_AND_DELETE_NODE_FROM_DUMMY_DCI0_LIST(eCidReportParams_p->dummyDCI0Entry_p,
                    eCidReportParams_p->dummyDCI0PresentFlag, internalCellIndex);
                /* CA Changes end  */
                ulUEContext_p->eCidReportParams.dummyDCI0PresentFlag = FLAG_RESET; 
                /* SPR_8672_FIX */
            } 
            break;
            case AOA_REPORTS_REQUIRED:
            {
                /* SPR 9927 fix start */
                macRrmEcidMeasurementResp_p->bitmask |= AOA_REPORT_BITMASK;
                /* SPR 9927 fix end */

                for(count = 0; count <MAX_NUM_ANTENNA; count++)
                {
                    macRrmEcidMeasurementResp_p->angleOfArrivalReport.angleOfArrivalValue[count] = 
                        eCidReportParams_p->angleOfArrival[count];
                }

                macRrmEcidMeasurementResp_p->angleOfArrivalReport.systemFrameNum = 
                    (eCidReportParams_p->type1TaAOAReceivedTTI / MAX_SUB_FRAME) % MAX_SFN_VALUE;
                /* SPR_8672_FIX */
                /* CA Changes start  */
                CHECK_AND_DELETE_NODE_FROM_DUMMY_DCI0_LIST(eCidReportParams_p->dummyDCI0Entry_p,
                    eCidReportParams_p->dummyDCI0PresentFlag, internalCellIndex);
                /* CA Changes end  */
                ulUEContext_p->eCidReportParams.dummyDCI0PresentFlag = FLAG_RESET; 
                /* SPR_8672_FIX */
            }
            break;  
            case TYPE_1_TA_AOA_REPORTS_REQUIRED:
            {
                /* SPR 9927 fix start */
                macRrmEcidMeasurementResp_p->bitmask |= TYPE_1_TA_REPORT_BITMASK;
                macRrmEcidMeasurementResp_p->bitmask |= AOA_REPORT_BITMASK;
                /* SPR 9927 fix end */

                macRrmEcidMeasurementResp_p->type1EnbRxTxReport.type1EnbRxTxDiffValue = 
                    eCidReportParams_p->type1TAValue;

                for(count = 0; count <MAX_NUM_ANTENNA; count++)
                {
                    macRrmEcidMeasurementResp_p->angleOfArrivalReport.angleOfArrivalValue[count] = 
                        eCidReportParams_p->angleOfArrival[count];
                }

                macRrmEcidMeasurementResp_p->angleOfArrivalReport.systemFrameNum = 
                    macRrmEcidMeasurementResp_p->type1EnbRxTxReport.systemFrameNum = 
                    (eCidReportParams_p->type1TaAOAReceivedTTI / MAX_SUB_FRAME) % MAX_SFN_VALUE;
 
                /* SPR_8672_FIX */
                /*CA Changes start  */
                CHECK_AND_DELETE_NODE_FROM_DUMMY_DCI0_LIST(eCidReportParams_p->dummyDCI0Entry_p, 
                    eCidReportParams_p->dummyDCI0PresentFlag,internalCellIndex);
                /*CA Changes end  */
                ulUEContext_p->eCidReportParams.dummyDCI0PresentFlag = FLAG_RESET; 
                /* SPR_8672_FIX */

            }
            break;
            case TYPE_2_TA_REPORT_REQUIRED:
            {
                /* SPR 9927 fix start */
                macRrmEcidMeasurementResp_p->bitmask |= TYPE_2_TA_REPORT_BITMASK; 
                /* SPR 9927 fix end */

                macRrmEcidMeasurementResp_p->type2EnbRxTxReport.type2EnbRxTxDiffValue = 
                    eCidReportParams_p->type2TAValue;
                macRrmEcidMeasurementResp_p->type2EnbRxTxReport.systemFrameNum = 
                    (eCidReportParams_p->type2TAreceivedTTI / MAX_SUB_FRAME) % MAX_SFN_VALUE;

                eCidReportParams_p->type2TAValue = INVALID_TIMING_ADVANCE_R9_VALUE;
            }
            break;
            case TYPE_1_2_TA_REPORTS_REQUIRED:
            {
                /* SPR 9927 fix start */
                macRrmEcidMeasurementResp_p->bitmask |= TYPE_1_TA_REPORT_BITMASK;
                macRrmEcidMeasurementResp_p->bitmask |= TYPE_2_TA_REPORT_BITMASK; 
                /* SPR 9927 fix end */

                macRrmEcidMeasurementResp_p->type1EnbRxTxReport.type1EnbRxTxDiffValue = 
                    eCidReportParams_p->type1TAValue;
                macRrmEcidMeasurementResp_p->type1EnbRxTxReport.systemFrameNum =
                    (eCidReportParams_p->type1TaAOAReceivedTTI / MAX_SUB_FRAME) % MAX_SFN_VALUE; 

                macRrmEcidMeasurementResp_p->type2EnbRxTxReport.type2EnbRxTxDiffValue = 
                    eCidReportParams_p->type2TAValue;
                macRrmEcidMeasurementResp_p->type2EnbRxTxReport.systemFrameNum = 
                    (eCidReportParams_p->type2TAreceivedTTI / MAX_SUB_FRAME) %  MAX_SFN_VALUE;

                eCidReportParams_p->type2TAValue = INVALID_TIMING_ADVANCE_R9_VALUE;

            }
            break;
            case TYPE_2_TA_AOA_REPORTS_REQUIRED:
            {
                /* SPR 9927 fix start */
                macRrmEcidMeasurementResp_p->bitmask |= TYPE_2_TA_REPORT_BITMASK;
                macRrmEcidMeasurementResp_p->bitmask |= AOA_REPORT_BITMASK;
                /* SPR 9927 fix end */

                macRrmEcidMeasurementResp_p->type2EnbRxTxReport.type2EnbRxTxDiffValue = 
                    eCidReportParams_p->type2TAValue;
                macRrmEcidMeasurementResp_p->type2EnbRxTxReport.systemFrameNum = 
                    (eCidReportParams_p->type2TAreceivedTTI / MAX_SUB_FRAME) %  MAX_SFN_VALUE;

                for(count = 0; count <MAX_NUM_ANTENNA; count++)
                {
                    macRrmEcidMeasurementResp_p->angleOfArrivalReport.angleOfArrivalValue[count] = 
                        eCidReportParams_p->angleOfArrival[count];
                }
                macRrmEcidMeasurementResp_p->angleOfArrivalReport.systemFrameNum =  
                    (eCidReportParams_p->type1TaAOAReceivedTTI / MAX_SUB_FRAME) % MAX_SFN_VALUE;

                eCidReportParams_p->type2TAValue = INVALID_TIMING_ADVANCE_R9_VALUE;

            }
            break;
            case TYPE_1_2_TA_AOA_REPORTS_REQUIRED:
            {
                /* SPR 9927 fix start */
                macRrmEcidMeasurementResp_p->bitmask |= TYPE_1_TA_REPORT_BITMASK;
                macRrmEcidMeasurementResp_p->bitmask |= TYPE_2_TA_REPORT_BITMASK;
                macRrmEcidMeasurementResp_p->bitmask |= AOA_REPORT_BITMASK;
                /* SPR 9927 fix end */

                macRrmEcidMeasurementResp_p->type1EnbRxTxReport.type1EnbRxTxDiffValue = 
                    eCidReportParams_p->type1TAValue;
                macRrmEcidMeasurementResp_p->type1EnbRxTxReport.systemFrameNum =
                    (eCidReportParams_p->type1TaAOAReceivedTTI / MAX_SUB_FRAME) % MAX_SFN_VALUE;

                macRrmEcidMeasurementResp_p->type2EnbRxTxReport.type2EnbRxTxDiffValue = 
                    eCidReportParams_p->type2TAValue;
                macRrmEcidMeasurementResp_p->type2EnbRxTxReport.systemFrameNum = 
                    (eCidReportParams_p->type2TAreceivedTTI / MAX_SUB_FRAME) %  MAX_SFN_VALUE;

                for(count = 0; count <MAX_NUM_ANTENNA; count++)
                {
                    macRrmEcidMeasurementResp_p->angleOfArrivalReport.angleOfArrivalValue[count] = 
                        eCidReportParams_p->angleOfArrival[count];
                }
                macRrmEcidMeasurementResp_p->angleOfArrivalReport.systemFrameNum =
                    macRrmEcidMeasurementResp_p->type1EnbRxTxReport.systemFrameNum; 

                eCidReportParams_p->type2TAValue = INVALID_TIMING_ADVANCE_R9_VALUE;

            }
            break;
            default :
            {
                /*No report required*/
            }


        }
    }
    ulUEContext_p->eCidReportParams.typeofReportsRequired = NO_REPORT_TYPE_REQUIRED;
    ulUEContext_p->eCidReportParams.reportStatus =  INVALID_STATUS_ECID_REPORTS;
    /* Fill Msg Len in the API header */
    current += sizeof(MacRrmEcidMeasurementResp);

    /*CA Changes start  */
    /* Prepare a response and send it to RRM */
    prepareMACRRMApiHeader(l2RrmMsgCnf_p,MAC_RRM_E_CID_MEAS_REPORT_RESP,
            transactionId,current,internalCellIndex);
    /*CA Changes end  */

    /* Send the response back to RRM */
    /* + Coverity 32709, 32710 */
    if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg_p, current
                ))
    /* - Coverity 32709, 32710 */
    {
        LOG_MAC_ERROR( MAC_RRM_INF, "Unable to send the msg to RRM!" );
    }
    LOG_MAC_MSG(MAC_RRM_ECID_REPORT_RES_INFO, LOGINFO, MAC_RRM_INF,
            getCurrentTick(),\
            ulUEContext_p->ueIndex,MAC_SUCCESS,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,__func__,"");
    /*Coverity 5.3.0 CID-54186 */
    freeMemPool(cnfMsg_p);
    /*Coverity 5.3.0 CID-54186 */
}

/*****************************************************************************
 * Function Name  : putEntryInEcidMeasReportQueue
 * Inputs         : ueIndex - UE ID, 
 *                  requiredReports - Type of reports required,
 *                  responseCode - Response code to send to RRM
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None 
 * Description    : This function will put an entry in eCidReportsQueue_g queue.
 *****************************************************************************/
/* +- SPR 18268 */
void putEntryInEcidMeasReportQueue(UInt16 ueIndex,
        /* +- SPR 18268 */
                                   UInt8 requiredReports,
                                   UInt8 responseCode,
                                   /*CA Changes start  */
                                   InternalCellIndex internalCellIndex)
                                   /*CA Changes end  */
{
    EcidReportNode *reportNodeEntry_p = PNULL;

    GET_MEM_NODE_E_CID_REPORT_Q(reportNodeEntry_p,EcidReportNode);
    
    /* + Coverity 40849 */
    if( PNULL != reportNodeEntry_p )
    {
        reportNodeEntry_p->ueIndex = ueIndex;
        reportNodeEntry_p->responseCode = responseCode;
        reportNodeEntry_p->requiredReports = requiredReports;
        /*CA Changes start  */
        /*Cov_fix_30April_start*/
        /* SPR 23483 Changes Start */
        if(CIRC_SUCCESS != ENQUEUE_E_CID_REPORT_Q(EcidReportNode,&(reportNodeEntry_p->eCidReportAnchor), internalCellIndex))
        /* SPR 23483 Changes End */
        {
            FREE_MEM_NODE_E_CID_REPORT_Q(reportNodeEntry_p);
        }
        /*Cov_fix_30April_stop*/
        /*CA Changes end  */
    }
    /* - Coverity 40849 */
}

/* ++coverity--530-54652*/

/************************************************************************
 * Function Name  :  cleanupECidQueue
 * Inputs         :  internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will cleanup the eCid Queue.
 ************************************************************************/
/*CA Changes start  */
void cleanupECidQueue(InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt32 count =0; 
    UInt32 i =0;
    EcidReportNode *reportNodeEntry_p = PNULL;
    /*CA Changes start  */
    count = COUNT_E_CID_REPORT_Q(internalCellIndex);
    /*CA Changes end  */

    for (i = 0; i < count; i++)
    {
        /*CA Changes start  */
        DEQUEUE_E_CID_REPORT_Q(EcidReportNode ,(void **)&reportNodeEntry_p, internalCellIndex);
        /*CA Changes end  */

        if (reportNodeEntry_p)
        {
            FREE_MEM_NODE_E_CID_REPORT_Q(reportNodeEntry_p);
            reportNodeEntry_p = PNULL;
        }
    }

}

/************************************************************************
 * Function Name  :  deInitECidQueue
 * Inputs         :  internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will un-initialize the eCid Queue.
 ************************************************************************/
/*CA Changes start  */
void deInitECidQueue(InternalCellIndex internalCellIndex)
{
   circQDeInit(eCidReportsQueue_gp[internalCellIndex]);

   freeMemPool(eCidReportsQueue_gp[internalCellIndex]);
   eCidReportsQueue_gp[internalCellIndex] = PNULL;
}
/*CA Changes end  */
/************************************************************************
 * Function Name  :  cleanAndDeInitECidQueue
 * Inputs         :  internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function cleanup and un-initialize the eCid Queue.
 ************************************************************************/
/* CA Changes start */
void cleanAndDeInitECidQueue(InternalCellIndex internalCellIndex)
/* CA Changes end */
{
    cleanupECidQueue(internalCellIndex);
    deInitECidQueue(internalCellIndex);

}
/* ++coverity--530-54652*/
