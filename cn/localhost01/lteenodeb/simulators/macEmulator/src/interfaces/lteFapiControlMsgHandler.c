/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteFapiControlMsgHandler.c,v 1.1.4.3 2010/10/20 06:29:03 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the Interface API between the 
 *                     MAC and Phy Simulator Layer in a LTE system. 
 *
 ****************************************************************************
 *
 * Revision Details :
 * $Log: lteFapiControlMsgHandler.c,v $
 * Revision 1.1.4.3  2010/10/20 06:29:03  gur20491
 * Removed magic number in for loop
 *
 * Revision 1.1.4.2  2010/10/12 13:40:04  gur20439
 * first tick sending logic was wrong
 *
 * Revision 1.1.4.1  2010/09/21 15:53:30  gur20491
 * FAPI changes
 *
 * Revision 1.1.2.1  2010/09/07 11:21:08  gur20439
 * new files added for FAPI changes
 *1,v $
 ****************************************************************************/

#include "lteFapiControlMsgHandler.h"
#include "lteFapiMacPhyInterface.h"
#include "phySimulatorApiHandler.h"
#include "lteCommonStatsManager.h"
/* SSI Changes Start */
#if (defined(LTE_EMBMS_SUPPORTED) && !defined(LAYER2_DLTG_THREAD))
#include "lteMisc.h"
#endif
/* SSI Changes End */

/* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
extern UInt8 firstDlConfigAfterPhyStart_g[MAX_NUM_CELLS];
#endif
/* FAPI L2 AS MASTER END */

/*SPR 18697 fix start*/ 
#ifdef LTE_EMBMS_SUPPORTED
extern tickType_t embmsPlatformTick_g;
#endif
/*SPR 18697 fix end*/ 
/* + SPR_6009 */
extern UInt8 l1MeasReportsToSend;
/* - SPR_6009 */
extern volatile UInt8 macRegFlag_g[MAX_NUM_CELLS];
extern volatile UInt8 paramReqrecvd[MAX_NUM_CELLS];
FAPI_phyStates_en phyState_g[MAX_NUM_CELLS] = {FAPI_IDLE, FAPI_IDLE};
UInt16   sfnsf_g[MAX_NUM_CELLS] = {0,0};
extern inline void sendDatatoPeerMac( UInt8 *sendBuffer_p, UInt32 msgLen,
		UInt8 cellIndex);
/*FAPI_ERROR_INDICATION_CHANGES_START*/
extern UInt16 phy_err_msg_type_g[MAX_NUM_CELLS];
extern UInt16 phy_err_error_type_g[MAX_NUM_CELLS];
extern UInt16 phy_err_pdu_type_g[MAX_NUM_CELLS];
extern UInt16 phy_err_sub_err_code_g[MAX_NUM_CELLS];
extern UInt16 phy_err_crnti_g[MAX_NUM_CELLS];
extern UInt16 phy_err_rb_start_g[MAX_NUM_CELLS];
extern UInt16 phy_err_pdu_index_g[MAX_NUM_CELLS];
extern UInt16 errorCount_g[MAX_NUM_CELLS];
extern UInt16 phy_err_config_g[MAX_NUM_CELLS];
/*FAPI_ERROR_INDICATION_CHANGES_END*/
extern inline void sendDatatoPeerMac( UInt8 *sendBuffer_p, UInt32 msgLen, UInt8 cellIndex);
extern UInt8 validateSFNandSFAndgetDelay( UInt16 recvSFN, UInt8 recvSF, UInt16 currentSFN, UInt8 currentSF );
extern CellConfig cellConfigUeSim_g[MAX_NUM_CELLS];
/* SPR 21660 changes start_Sahil */
/* SPR 21660 changes end_Sahil */
extern volatile UInt8 cellId_g;
#ifdef HO_SETUP_CHANGES
static UInt8 paramReqRecvFlag[MAX_NUM_CELLS] = {0, 0};
UInt8 cellConfigRecvFlag[MAX_NUM_CELLS] = {0, 0};
UInt8 startRecvFlag[MAX_NUM_CELLS] = {0, 0};
UInt8 activeCell_g = INVALID_CELL_INDEX;
/* SPR 11646*/
UInt8 isCellOfTargetEnb[MAX_NUM_CELLS] = {0, 0};
/* SPR 11646*/
extern inline void sendSFItoSourceENBMac( UInt8 *sendBuffer_p, UInt32 msgLen, UInt8 cellIndex);
extern void resetGlobals(UInt8 cellIndex);
/* + SPR_11130 */
#define CELL_START_AFTER_INIT 0
#define CELL_START_AFTER_CELL_STOP 1
UInt8 cellStateAfterCellStop[MAX_NUM_CELLS] = {CELL_START_AFTER_INIT,CELL_START_AFTER_INIT};
UInt8 cellStopTriggered[MAX_NUM_CELLS] = {FALSE,FALSE};
/* - SPR_11130 */
/* + SPR_11574 */
 /* + SPR_11585 */
extern SInt32 ev_fd_SFI_g;
 /* - SPR_11585 */
/* - SPR_11574 */
/* + SPR_11914 */
extern UInt8 isEvFdRead[MAX_NUM_CELLS];
/* - SPR_11914 */
/* + SPR_11907_11908 */
extern SInt32 ev_fd_g[MAX_NUM_CELLS][MAX_NUM_CELLS];
/* - SPR_11907_11908 */

#ifdef LTE_EMBMS_SUPPORTED
/* eMBMS Globals */
extern UInt8 eMBMSEnable_g[MAX_NUM_CELLS];
/* SSI Changes Start */
#ifndef LAYER2_DLTG_THREAD
UInt16                tempSfn = 0;
#endif
/* SSI 7.0 Changes End */
#endif

/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
UInt8 rbgSize_g[MAX_NUM_CELLS] = {0,0};
UInt8 numRBGConfigured_g[MAX_NUM_CELLS] = {0,0};

#define NUM_OF_RBG_BW1DOT4                          6
#define NUM_OF_RBG_BW3                              8
#define NUM_OF_RBG_BW5                              13
#define NUM_OF_RBG_BW10                             17
#define NUM_OF_RBG_BW15                             19
#define NUM_OF_RBG_BW20                             25

#define RBG_SIZE_FOR_BW1DOT4                        1
#define RBG_SIZE_FOR_BW3                            2
#define RBG_SIZE_FOR_BW5                            2
#define RBG_SIZE_FOR_BW10                           3
#define RBG_SIZE_FOR_BW15                           4
#define RBG_SIZE_FOR_BW20                           4
#endif
/* SPR 21958 PUSCH RAT1 Support End */

/* + Support for toggling configured Nack percentage to 100% ack and vice-versa */
/* First tick will increament currentTick_g to zero */
UInt32 currentTick_g=-1;
/* - Support for toggling configured Nack percentage to 100% ack and vice-versa */
/***************************************************************************
 * Function Name  : setActiveCell
 * Inputs         : cell Index
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function will return internal cell index update
 *                  to update the tick
****************************************************************************/
void setActiveCell(UInt8 cellIndex)
{
    activeCell_g = cellIndex;
}

/***************************************************************************
 * Function Name  : getActiveCell
 * Inputs         : None
 * Outputs        : None
 * Returns        : internal index corresponding which is used to update the ticks
 * Variables      :
 * Description    : This function will return internal cell index update
 *                  to update the tick
****************************************************************************/
UInt8 getActiveCell()
{
    return activeCell_g;
}

void sendMsgtoTargetL2Config (UInt8 *receivedBuffer_p,
                                UInt32 msgLen,
                                UInt8 cellIndex)
{
    SInt32 ret = 0;
    ret =  sendto(txTargetEnbControlsocketFD_g[cellIndex], receivedBuffer_p,msgLen, 0,
            (struct sockaddr*)&txtargetEnodebRRC_addr_g[cellIndex], sizeof(SockAddr));
    if (ret == -1)
    {
        MAC_EMULATOR_LOG(LOG_INFO,"Error No- %d Desp: %s  in %s at Line no %d\n",\
                errno,strerror(errno), __func__, __LINE__);
    }
}
#endif
/*****************************************************************************
 * Function Name  :  sendMsgtoL2Config
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    :
 *****************************************************************************/
void sendMsgtoL2Config (UInt8 *receivedBuffer_p,
                                UInt32 msgLen, UInt8 cellIndex)
{
    SInt32 ret = 0;
    ret =  sendto(txEnodebRRCSocketFD_g[cellIndex], receivedBuffer_p,msgLen, 0,
            (struct sockaddr*)&txEnodebRRC_addr_g[cellIndex], sizeof(SockAddr));
    if (ret == -1)
    {
        MAC_EMULATOR_LOG(LOG_INFO,"Error No- %d Desp: %s  in %s at Line no %d\n",\
                errno,strerror(errno), __func__, __LINE__);
    }

}

/*****************************************************************************
 * Function Name  : fapiSendParamResp 
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    : to send Param.Response from Phy running in FDD mode and 
 *                  in Idle state
 *****************************************************************************/
void fapiSendParamResp( UInt8 **paramRes_p, UInt32 *paramRspmsgSize,
    FAPI_l1ErrorCodes_en errCode,  UInt8 cellIndex )
{
    FAPI_l1ApiMsg_st       *paramResp_p = (FAPI_l1ApiMsg_st *)(*paramRes_p);
    FAPI_paramResponse_st  *pRspMsgBody_p = PNULL;
    UInt16                 paramRspmsgBodySize = 0;

    *paramRspmsgSize = L1_PARTIAL_API_MSG_SIZE;
    paramRspmsgBodySize = PHY_PARAM_RESP_MSG_SIZE;
    *paramRspmsgSize += paramRspmsgBodySize;

    paramResp_p = ( FAPI_l1ApiMsg_st *) getMemFromPool( *paramRspmsgSize, PNULL );
    if ( PNULL == paramResp_p )
    {
        MAC_EMULATOR_LOG(LOG_INFO,"Error in getting memory for PHY_PARM_RESP\n");
        return;
    }

    paramResp_p->msgId = PHY_PARAM_RESPONSE;
    paramResp_p->lenVendorSpecific = 0;
    paramResp_p->msgLen = MAC_PHY_CONVERT_16((UInt16 ) paramRspmsgBodySize);
    pRspMsgBody_p = (FAPI_paramResponse_st *) (&(paramResp_p->msgBody[0]));

    /* fapi changes start */
    pRspMsgBody_p->errCode = errCode;
    if( FAPI_MSG_OK == errCode )
    {
//#if 0
#ifdef FAPI_4_0_COMPLIANCE
#ifdef LTE_EMBMS_SUPPORTED        
        pRspMsgBody_p->numOfTlv = 8;
#else
        pRspMsgBody_p->numOfTlv = 7;
#endif        
#else
        pRspMsgBody_p->numOfTlv = 6;
#endif
        pRspMsgBody_p->tlvs[0].tag = FAPI_PHY_STATE;
        /*SPR 7095 change start */ 
        /*   pRspMsgBody_p->tlvs[0].tagLen = 4; */
        pRspMsgBody_p->tlvs[0].tagLen = sizeof(FAPI_cellConfigParam_un);
        /*SPR 7095 change end */    /* done for every tagLen present at differnt index */ 
        /* SPR 9798 fix start */
        pRspMsgBody_p->tlvs[0].configParam.phyState = MAC_PHY_CONVERT_16( FAPI_IDLE );
        /* SPR 9798 fix end */
        pRspMsgBody_p->tlvs[1].tag = FAPI_DL_BANDWIDTH_SUPPORT;
        /*    pRspMsgBody_p->tlvs[1].tagLen = 4; */
        pRspMsgBody_p->tlvs[1].tagLen = sizeof(FAPI_cellConfigParam_un);
        /* spr 1308 changes start */
        /* spr 3213 changes start */
        pRspMsgBody_p->tlvs[1].configParam.dlBandWidthSupport = MAC_PHY_CONVERT_16((0x01 << 5) |
                (0x01 << 4)|
                (0x01 << 3)|
                (0x01 << 2)|
                (0x01 << 1)|
                (0x01));
        /* spr 3213 changes end */
        /* spr 1308 changes end */
        pRspMsgBody_p->tlvs[2].tag = FAPI_UL_BANDWIDTH_SUPPORT;
        /*    pRspMsgBody_p->tlvs[2].tagLen = 4; */
        pRspMsgBody_p->tlvs[2].tagLen = sizeof(FAPI_cellConfigParam_un);
        /* spr 1308 changes start */
        /* spr 3213 changes start */
        pRspMsgBody_p->tlvs[2].configParam.ulBandWidthSupport = MAC_PHY_CONVERT_16((0x01 << 5) |
                (0x01 << 4)|
                (0x01 << 3)|
                (0x01 << 2)|
                (0x01 << 1)|
                (0x01));
        /* spr 3213 changes end */
        /* spr 1308 changes end */
        pRspMsgBody_p->tlvs[3].tag = FAPI_DL_MODULATION_SUPPORT;
        /*    pRspMsgBody_p->tlvs[3].tagLen = 4; */
        pRspMsgBody_p->tlvs[3].tagLen = sizeof(FAPI_cellConfigParam_un);
        pRspMsgBody_p->tlvs[3].configParam.dlModulationSupport = 0x00;
        pRspMsgBody_p->tlvs[4].tag = FAPI_UL_MODULATION_SUPPORT;
        /*    pRspMsgBody_p->tlvs[4].tagLen = 4;   */
        pRspMsgBody_p->tlvs[4].tagLen = sizeof(FAPI_cellConfigParam_un);
        pRspMsgBody_p->tlvs[4].configParam.ulModulationSupport = 0x00;
        pRspMsgBody_p->tlvs[5].tag = FAPI_PHY_ANTENNA_CAPABILITY;
        /*    pRspMsgBody_p->tlvs[5].tagLen = 4;  */
        pRspMsgBody_p->tlvs[5].tagLen = sizeof(FAPI_cellConfigParam_un);
        /* SPR 5165 changes start */
        //pRspMsgBody_p->tlvs[5].configParam.phyAntennaCapability = MAC_PHY_CONVERT_16(2); /* 4x2 MIMO Changes*/
        pRspMsgBody_p->tlvs[5].configParam.phyAntennaCapability = MAC_PHY_CONVERT_16(4);
        /* SPR 5165 changes end */
        /* send data to L2/L3 software */
        /*FAPI2_1 changes start*/
#ifdef FAPI_4_0_COMPLIANCE
        pRspMsgBody_p->tlvs[6].tag = FAPI_PHY_RELEASE_CAPABILITY;
        pRspMsgBody_p->tlvs[6].tagLen =  sizeof(FAPI_cellConfigParam_un);
#ifdef FAPI_RELEASE8
        pRspMsgBody_p->tlvs[6].configParam.releaseCapability=RELEASE8_MASK ;
#endif
#ifdef FAPI_RELEASE9
        pRspMsgBody_p->tlvs[6].configParam.releaseCapability=RELEASE9_MASK;
#endif
#ifdef FAPI_RELEASE10
        pRspMsgBody_p->tlvs[6].configParam.releaseCapability=RELEASE10_MASK;
#endif
#endif        
#ifdef LTE_EMBMS_SUPPORTED
        pRspMsgBody_p->tlvs[7].tag = FAPI_PHY_MBSFN_CAPABILITY;
        pRspMsgBody_p->tlvs[7].tagLen =  sizeof(FAPI_cellConfigParam_un);
        pRspMsgBody_p->tlvs[7].configParam.mbsfnCapability=MAC_PHY_CONVERT_16(eMBMSEnable_g[cellIndex]);

#endif        
        /*FAPI2_1 changes end*/
//#endif
    }
    *paramRes_p= paramResp_p;

    paramReqrecvd[cellIndex] = TRUE;
}


/*****************************************************************************
 * Function Name  :  fapiparseAndCreateUeContext 
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    :
 *****************************************************************************/
FAPI_l1ErrorCodes_en fapiparseAndCreateUeContext( 
                FAPI_ueConfigRequest_st *ueConfigReq_p, 
                FAPI_ueConfig_st *missingTLVArr_p, 
                FAPI_ueConfig_st *invalid_unSupportedTLVArr_p, 
                UInt8 *missingTLVCount_p, 
                UInt8 *invalid_unsupportedTLVCount_p)

{
    FAPI_ueConfig_st           *ueConfigTlv_p = PNULL;
    UInt8                      *configTlv_p = PNULL;
    FAPI_l1ErrorCodes_en errCode = FAPI_MSG_OK;
    UInt8                      missingTagArr[20] = {0,};
    UInt32 i = 0;
    phyUeContext_st         phyUeContext;
    UInt8 invalid_unsuppCount = *invalid_unsupportedTLVCount_p; 

    configTlv_p = ( UInt8 *)&(ueConfigReq_p->tlvs[0]);
    ueConfigTlv_p = ( FAPI_ueConfig_st *) &(ueConfigReq_p->tlvs[0]);
    for ( i = 0; i < ueConfigReq_p->numOfTlv; i++)
    {
        ueConfigTlv_p = (  FAPI_ueConfig_st *) configTlv_p;
        MAC_EMULATOR_LOG(LOG_INFO,"tag = %d\n", ueConfigTlv_p->tag);
        switch ( ueConfigTlv_p->tag )
        {
            case FAPI_HANDLE:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 8 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        memCpy( (void *) &phyUeContext.handle, 
                                (void *)&(ueConfigTlv_p->value[0]), 
                                sizeof(UInt32) ); 
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_RNTI:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                           *ueConfigTlv_p;
                    }
                    else
                    {
                        memCpy( (void *) &phyUeContext.rnti, 
                                (void *) &(ueConfigTlv_p->value[0]), 
                                sizeof(UInt16) ); 
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_CQI_PUCCH_RESOURCE_INDEX:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        memCpy( (void *) &phyUeContext.cqiPucchResourceIndex, 
                                (void *) &(ueConfigTlv_p->value[0]), 
                                sizeof(UInt16) ); 
                        if ( phyUeContext.cqiPucchResourceIndex > 1185 )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_CQI_PMI_CONFIG_INDEX:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        memCpy( (void *) &phyUeContext.cqiPmiConfigIndex, 
                                (void *) &(ueConfigTlv_p->value[0]), 
                                sizeof(UInt16) ); 
                        if ( phyUeContext.cqiPmiConfigIndex > 1023 ) 
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_CQI_RI_CONFIG_INDEX:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        memCpy( (void *) &phyUeContext.cqiRiConfigIndex, 
                                (void *) &(ueConfigTlv_p->value[0]), 
                                sizeof(UInt16) ); 
                        if ( phyUeContext.cqiRiConfigIndex > 1023 )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }

                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_CQI_SIMULTANEOUS_ACK_NACK_CQI:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }                       
                    else
                    {
                        phyUeContext.simultaneousAckNackAndCqi = 
                                                    ueConfigTlv_p->value[0];
                        if ( ( phyUeContext.simultaneousAckNackAndCqi != 1) &&
                                ( phyUeContext.simultaneousAckNackAndCqi != 0 ) )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_AN_REPETITION_FACTOR:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        phyUeContext.ackNacknRepetitionFactor = 
                                                   ueConfigTlv_p->value[0]; 
                        if ( (phyUeContext.ackNacknRepetitionFactor != 2)
                                && (phyUeContext.ackNacknRepetitionFactor != 4)
                                && (phyUeContext.ackNacknRepetitionFactor != 6))
                        {     
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }

                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_AN_N1_PUCCH_AN_REP:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        memCpy( (void *) &phyUeContext.anN1PUCCHANRep, 
                                (void *) &(ueConfigTlv_p->value[0]), 
                                sizeof(UInt16) );
                        if ( phyUeContext.anN1PUCCHANRep > 2047 )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_TDD_ACK_NACK_FEEDBACK:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        phyUeContext.tddAckNackFeedbackMode =  
                            ueConfigTlv_p->value[0] ;
                        if ( ( phyUeContext.tddAckNackFeedbackMode != 0 ) &&
                                ( phyUeContext.tddAckNackFeedbackMode != 1 ) )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_SRS_BANDWIDTH:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        phyUeContext.srSBandWidth =
                            ueConfigTlv_p->value[0]; 
                        if ( phyUeContext.srSBandWidth > 3 )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_SRS_HOPPING_BANDWIDTH:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        phyUeContext.srsHoppingBandwidth = 
                            ueConfigTlv_p->value[0]; 
                        if ( phyUeContext.srsHoppingBandwidth > 3 )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_FREQUENCY_DOMAIN_POSITION:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        phyUeContext.freqDomainPosition = 
                            ueConfigTlv_p->value[0]; 
                        if ( phyUeContext.freqDomainPosition > 23 )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_SRS_DURATION:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        phyUeContext.srsDuration =
                            ueConfigTlv_p->value[0]; 
                        if ( (phyUeContext.srsDuration != 0) &&
                                (phyUeContext.srsDuration != 1))
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_ISRS_SRS_CONFIG_INDEX:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        memCpy( (void *) &phyUeContext.srsConfigIndex, 
                                (void *) &(ueConfigTlv_p->value[0]), 
                                sizeof(UInt16) ); 
                        if ( phyUeContext.srsConfigIndex > 1023 )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_TRANSMISSION_COMB:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        phyUeContext.txComb  =
                            ueConfigTlv_p->value[0];
                        if ( ( phyUeContext.txComb != 0 ) && 
                                ( phyUeContext.txComb != 1 ) )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_SOUNDING_REFERENCE_SYCLIC_SHIFT:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        phyUeContext.soundingRefCyclicShift  =
                            ueConfigTlv_p->value[0]; 
                        if ( phyUeContext.soundingRefCyclicShift > 7)
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_SR_PUCCH_RESOURCE_INDEX:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        memCpy( (void *) &phyUeContext.srPucchResourceIndex, 
                                (void *) &(ueConfigTlv_p->value[0]), 
                                sizeof(UInt16) ); 
                        if ( phyUeContext.srPucchResourceIndex > 2047 )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_SR_CONFIG_INDEX:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        phyUeContext.srConfigIndex =
                            ueConfigTlv_p->value[0]; 
                        if ( phyUeContext.srConfigIndex > 154 )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_SPS_DL_CONFIG_SCHD_INTERVAL:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        memCpy( (void *) &phyUeContext.spsDlConfigSchedInterval, 
                                (void *) &(ueConfigTlv_p->value[0]), 
                                sizeof(UInt16) ); 
                        if ( ( phyUeContext.spsDlConfigSchedInterval != 10 ) && 
                                ( phyUeContext.spsDlConfigSchedInterval != 20 ) &&
                                ( phyUeContext.spsDlConfigSchedInterval != 32 ) &&
                                ( phyUeContext.spsDlConfigSchedInterval != 40 ) &&
                                ( phyUeContext.spsDlConfigSchedInterval != 64 ) &&
                                ( phyUeContext.spsDlConfigSchedInterval != 80 ) &&
                                ( phyUeContext.spsDlConfigSchedInterval != 128 ) &&
                                ( phyUeContext.spsDlConfigSchedInterval != 160 ) &&
                                ( phyUeContext.spsDlConfigSchedInterval != 320 ) &&
                                ( phyUeContext.spsDlConfigSchedInterval != 640 ) )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            case FAPI_SPS_DL_N1_PUCCH_AN_PERSISTENT:
                {
                    missingTagArr[i] = 1;
                    if ( ueConfigTlv_p->tagLen != 4 )
                    {
                        invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                    }
                    else
                    {
                        memCpy( (void *) &phyUeContext.spsDln1PUCCHANPersistent, 
                                (void *) &(ueConfigTlv_p->value[0]), 
                                sizeof(UInt16) ); 
                        if ( phyUeContext.spsDln1PUCCHANPersistent > 2047 )
                        {
                            invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                          *ueConfigTlv_p;
                        }
                    }
                    configTlv_p += ueConfigTlv_p->tagLen;
                }
                break;
            default:
                {
                    MAC_EMULATOR_LOG(LOG_INFO,"\n INVALID TLV (0x%02x)",ueConfigTlv_p->tag );
                    invalid_unSupportedTLVArr_p[invalid_unsuppCount++] = 
                                                     *ueConfigTlv_p;
                }
                break;
        }
    }

    for ( i = 0; i< ueConfigReq_p->numOfTlv; i++ )/*UE_CONFIG_ERROR RRC_2_0_1*/
    {
        if ( missingTagArr[i] != 1 )
        {
            MAC_EMULATOR_LOG(LOG_INFO,"missig tag = %d\n", i+100);
            errCode = FAPI_MSG_INVALID_CONFIG;
            missingTLVArr_p[i].tag = i + 100; 
            missingTLVArr_p[i].tagLen = 4;
            (*missingTLVCount_p)++;
        }
    }
    *invalid_unsupportedTLVCount_p = invalid_unsuppCount;
    if ( (*invalid_unsupportedTLVCount_p))
    {
        MAC_EMULATOR_LOG(LOG_INFO,"invalid or unsupported tag count = %d\n", 
                                   *invalid_unsupportedTLVCount_p);
        errCode = FAPI_MSG_INVALID_CONFIG;
    }

    return errCode;

}
/****************************************************************************
 * Function Name  : fapiCheckMandatoryTLVandSendUeConfigResponse 
 * Inputs         :  
 * Outputs        :  
 * Returns        :  
 * Description    : 
 ****************************************************************************/
void fapiCheckMandatoryTLVandSendUeConfigResponse (FAPI_l1ErrorCodes_en errCode,
                                        FAPI_ueConfigRequest_st *ueConfigReq_p, UInt8 cellIndex)
{
    FAPI_l1ApiMsg_st           *l1ApiMsg_p = PNULL;
    FAPI_phyErrorIndication_st *errInd_p   = PNULL;
    FAPI_phyUeConfigResp_st    *ueConfigRsp_p = PNULL;
    UInt32                     l1ApiMsgSize = 0;  
    UInt16                     l1ApiMsgBodySize = 0;
    FAPI_ueConfig_st           invalid_unSupportedTLVArr[20];
    FAPI_ueConfig_st           missingTLVArr[20];
    UInt8                     missingTLVCount = 0, invalid_unsupportedTLVCount = 0;

    memSet ( (void *) invalid_unSupportedTLVArr, 0, 20 * sizeof(FAPI_ueConfig_st));
    memSet ( (void *) missingTLVArr, 0, 20 * sizeof(FAPI_ueConfig_st));

    if ( FAPI_MSG_INVALID_STATE == errCode )
    {
        l1ApiMsgSize = L1_PARTIAL_API_MSG_SIZE + sizeof(FAPI_phyErrorIndication_st);

        l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) getMemFromPool( l1ApiMsgSize, PNULL );
        if ( PNULL == l1ApiMsg_p )
        {
            MAC_EMULATOR_LOG(LOG_INFO," Error...!! in fapiCheckMandatoryTLVandSendUeConfigResponse" 
                    "allocation of buffer l1ApiMsg_p for FAPI_MSG_INVALID_STATE \n");
            return;
        }
        l1ApiMsg_p->msgId = PHY_ERROR_INDICATION;
        l1ApiMsg_p->lenVendorSpecific = 0;
        l1ApiMsg_p->msgLen = MAC_PHY_CONVERT_16(sizeof(FAPI_phyErrorIndication_st));

        errInd_p = (FAPI_phyErrorIndication_st*)l1ApiMsg_p->msgBody;

        errInd_p->msgId = PHY_UE_CONFIG_RESPONSE;
        errInd_p->errorCode = MAC_PHY_CONVERT_32(errCode);

        MAC_EMULATOR_LOG(LOG_INFO,"sendig error %d to MAC on Control Port\n",errCode);
        MAC_EMULATOR_LOG(LOG_INFO,"sendig msg of size %d to MAC on Control Port\n",l1ApiMsgSize);
        /* send data to L2/L3 software */
        sendMsgtoL2Config( (UInt8 *)l1ApiMsg_p, l1ApiMsgSize, cellIndex );
        freeMemPool(l1ApiMsg_p);
        l1ApiMsg_p = PNULL;
    }
    else
    {
        /* check for mandatory TLV */
        errCode = fapiparseAndCreateUeContext(ueConfigReq_p, missingTLVArr, 
                invalid_unSupportedTLVArr, &missingTLVCount, 
                &invalid_unsupportedTLVCount);
        MAC_EMULATOR_LOG(LOG_INFO,"------errorcode = %d ------\n",errCode);

        l1ApiMsgBodySize = sizeof(FAPI_phyUeConfigResp_st) + 
                          ((missingTLVCount + invalid_unsupportedTLVCount) 
                           * sizeof (FAPI_ueConfig_st));

        l1ApiMsgSize = L1_PARTIAL_API_MSG_SIZE + l1ApiMsgBodySize;

        l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) getMemFromPool( l1ApiMsgSize, PNULL );
        if ( PNULL == l1ApiMsg_p )
        {
            MAC_EMULATOR_LOG(LOG_INFO," Error!! in fapiCheckMandatoryTLVandSendUeConfigResponse allocation" 
                    "of buffer l1ApiMsg_p \n");
            return;
        }
        l1ApiMsg_p->msgId = PHY_UE_CONFIG_RESPONSE;
        l1ApiMsg_p->lenVendorSpecific = 0;
        l1ApiMsg_p->msgLen = MAC_PHY_CONVERT_16(l1ApiMsgBodySize);

        ueConfigRsp_p = ( FAPI_phyUeConfigResp_st *) &(l1ApiMsg_p->msgBody[0]); 
        
        ueConfigRsp_p->errorCode = MAC_PHY_CONVERT_32(errCode);
        ueConfigRsp_p->numOfInvalidOrUnsupportedTLV = invalid_unsupportedTLVCount;
        ueConfigRsp_p->numOfMissingTLV = missingTLVCount;
        MAC_EMULATOR_LOG(LOG_INFO,"invalid_unsupportedTLVCount %d missingTLVCount %d\n", 
                               invalid_unsupportedTLVCount, missingTLVCount);

        if ( invalid_unsupportedTLVCount )
        {
            memCpy ( ( void *) &ueConfigRsp_p->listOfTLV[0],
                     ( void *) &invalid_unSupportedTLVArr[0],
                     invalid_unsupportedTLVCount);
        }
        if ( missingTLVCount )
        {
            memCpy ( ( void *) &ueConfigRsp_p->listOfMissingTlv[0],
                     ( void *) &missingTLVArr[0],
                     missingTLVCount);
        }

        /* send data to L2/L3 software */
        MAC_EMULATOR_LOG(LOG_INFO,"sent UE config resp msg of size = %d on control port\n",
                                                 l1ApiMsgSize);
        sendMsgtoL2Config( (UInt8 *)l1ApiMsg_p, l1ApiMsgSize, cellIndex );
        freeMemPool(l1ApiMsg_p);
        l1ApiMsg_p = PNULL;
    }
}
/****************************************************************************
 * Function Name  : fapiHandleUeConfigRequest 
 * Inputs         :  
 * Outputs        :  
 * Returns        :  
 * Description    : 
 ****************************************************************************/
void fapiHandleUeConfigRequest (FAPI_l1ApiMsg_st *recvMsg_p, UInt8 cellIndex)
{
    FAPI_ueConfigRequest_st *ueConfigreq_p = PNULL;
    FAPI_l1ErrorCodes_en  errCode = FAPI_MSG_OK;

    ueConfigreq_p = ( FAPI_ueConfigRequest_st *) &(recvMsg_p->msgBody[0]);
    if ( PNULL == ueConfigreq_p )
    {
        MAC_EMULATOR_LOG(LOG_INFO," ue config request recevied with no msg body \n");
        return;
    }

    if ( FAPI_RUNNING != phyState_g[cellIndex] )
    {
        errCode = FAPI_MSG_INVALID_STATE;
    }

    /* check for mandatory TLV for FDD  and send resp back to L2/L3*/
    fapiCheckMandatoryTLVandSendUeConfigResponse(errCode, ueConfigreq_p, cellIndex);
}

/****************************************************************************
 * Function Name  : fapiSendSubFrameStartIndication
 * Inputs         :  
 * Outputs        :  
 * Returns        :  
 * Description    : 
 ****************************************************************************/
void fapiSendSubFrameStartIndication ( UInt8 cellIndex )
{
    UInt8                       *sendBuffer_p = PNULL;
    FAPI_l1ApiMsg_st           *l1ApiMsg_p = PNULL;
    FAPI_subFrameIndication_st *sfInd_p = PNULL;
    UInt32                     l1ApiMsgSize = 0;  
    UInt32                     l1ApiMsgBodySize = 0;
    /* SSI Changes Start */
#if (defined(LTE_EMBMS_SUPPORTED) && !defined(LAYER2_DLTG_THREAD))
    UInt16                 sfn = 0;
    UInt8                  *sendBuff_p = sendBuff;
    TG_sfn_cellIndex_st    *tgSFNCellIndexMsg_p = PNULL;
    UInt32                 tgApiMsgSize = 0; 
    static UInt32                 currentTti = 0;
#endif
    /* SSI Changes End */
    l1ApiMsgBodySize = FAPI_SUBFRAME_INDICATION_MSG_BODY_SIZE;
    l1ApiMsgSize = L1_PARTIAL_API_MSG_SIZE + l1ApiMsgBodySize;
    sendBuffer_p = (UInt8 *) getMemFromPool( l1ApiMsgSize, PNULL );
    if ( PNULL == sendBuffer_p )
    {
        MAC_EMULATOR_LOG(LOG_INFO," Error!! in fapiSendSubFrameStartIndication allocation" 
                "of buffer sendBuffer_p \n");
        return;
    }
    l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) sendBuffer_p;
    l1ApiMsg_p->msgId = PHY_UL_SUBFRAME_INDICATION;
    l1ApiMsg_p->lenVendorSpecific = 0;
    l1ApiMsg_p->msgLen = MAC_PHY_CONVERT_16(l1ApiMsgBodySize);

   /* SPR 22578 fix start*/
  if (cellStopTriggered[cellIndex] == TRUE)
   {
       sfnsf_g[cellIndex] = 0;
       SFN_g[cellIndex]   = 0;
       SF_g[cellIndex]    = 0;
       cellStopTriggered[cellIndex] = FALSE;
   }
   /* SPR 22578 fix end*/

 
    sfInd_p = ( FAPI_subFrameIndication_st *) &(l1ApiMsg_p->msgBody[0]); 
    sfInd_p->sfnsf = MAC_PHY_CONVERT_16(sfnsf_g[cellIndex]);
    /* + SPR_6009 */
    if (sfInd_p->sfnsf == 1)
    {
        l1MeasReportsToSend = 1;
    }
    /* - SPR_6009 */
#ifdef HO_SETUP_CHANGES
    sendSFItoSourceENBMac( sendBuffer_p, l1ApiMsgSize, cellIndex);
    if ( startRecvFlag[cellIndex] == 2 )
    {
        sendDataToTargetEnbMac( sendBuffer_p, l1ApiMsgSize, cellIndex );
    }
#else
    sendDatatoPeerMac( sendBuffer_p, l1ApiMsgSize, cellIndex );
#endif
    /* SSI Changes Start */
#if (defined(LTE_EMBMS_SUPPORTED) && !defined(LAYER2_DLTG_THREAD))
    /* Get SFN from sfnsf */
    sfn = ((sfnsf_g[cellIndex] & FAPI_SFN_MASK) >> FAPI_SFN_VALUE_SHIFT);  

    /* Send SFN to TG after every 10 ms when new SFN is received */
    if (tempSfn != sfn) 
    {
	tempSfn = sfn;    
	tgApiMsgSize = TG_SFN_IND_CEll_INDEX_MSG_BODY_SIZE;

	SEND_BUFF_TO_TG_SET_U16BIT(&sendBuff_p[0],sfn);
	SEND_BUFF_TO_TG_SET_U16BIT(&sendBuff_p[2],(UInt16)cellIndex);
/*SPR 18697 fix start*/ 
		    SEND_BUFF_TO_TG_SET_U32BIT(&sendBuff_p[4],embmsPlatformTick_g);
/*SPR 18697 fix end*/ 
	//fprintf(stderr,"\nSweta sfn:[0x%x - 0x%x ] cellId:[0x%x - 0x%x]\n",sendBuff_p[0],sendBuff_p[1],sendBuff_p[2], sendBuff_p[3]);
	sendDataToTG(sendBuff_p, tgApiMsgSize);
    }
#endif
    /* SSI Changes End */
    freeMemPool(sendBuffer_p);
    sendBuffer_p = PNULL;
#ifdef OVERSHOOT_CORRECTION
    GET_CYCLE_COUNT(phyTickSend_g);
#endif

   /* + Support for toggling configured Nack percentage to 100% ack and vice-versa */
   currentTick_g++;
   /* - Support for toggling configured Nack percentage to 100% ack and vice-versa */
}

/****************************************************************************
 * Function Name  : fapiSendCellConfigRspInRunningState
 * Inputs         :  
 * Outputs        :  
 * Returns        :  
 * Description    : 
 ****************************************************************************/
void fapiSendCellConfigRspInRunningState ( UInt8 cellIndex)
{
    FAPI_l1ApiMsg_st           *configResp_p = PNULL;
    FAPI_phyCellConfigResp_st  *configRspMsgBody_p = PNULL;
    FAPI_cellConfig_st         *tlv_p = PNULL; 
    UInt32                     configRspmsgSize = 0;  
    UInt16                     l1ApiMsgBodySize = 0;

    l1ApiMsgBodySize = FAPI_RUNNING_STATE_CELL_CONFIG_RESP_MSG_BODY_SIZE;
    configRspmsgSize = L1_PARTIAL_API_MSG_SIZE + l1ApiMsgBodySize;

    configResp_p = ( FAPI_l1ApiMsg_st *) getMemFromPool( configRspmsgSize, PNULL );
    if ( PNULL == configResp_p )
    {
        MAC_EMULATOR_LOG(LOG_INFO," Error!! in fapiSendCellConfigRspInRunningState allocation" 
                "of buffer configResp_p \n");
        return;
    }

    configResp_p->msgId = PHY_CELL_CONFIG_RESPONSE;
    configResp_p->lenVendorSpecific = 0;
    configResp_p->msgLen = MAC_PHY_CONVERT_16(l1ApiMsgBodySize);
    configRspMsgBody_p = (FAPI_phyCellConfigResp_st *) (&(configResp_p->msgBody[0]));
    configRspMsgBody_p->errorCode = FAPI_MSG_OK;
    configRspMsgBody_p->numOfInvalidOrUnsupportedTLV = 0;
    configRspMsgBody_p->numOfMissingTLV = 0;
#if 0
    tlv_p = ( FAPI_cellConfig_st *) &(configRspMsgBody_p->listOfMissingTlv[0]);
    tlv_p->tag = FAPI_SFN_SF;
    tlv_p->tagLen = 4;
    tlv_p->configParam.sfnsf = sfnsf_g;
#endif

    /* send data to L2/L3 software on config port */
    sendMsgtoL2Config( (UInt8 *)configResp_p, configRspmsgSize, cellIndex);

    freeMemPool(configResp_p);
    configResp_p = PNULL;

}

/****************************************************************************
 * Function Name  : fapiCheckMandatoryTlvForFddAndSendResp 
 * Inputs         :  
 * Outputs        :  
 * Returns        :  
 * Description    : 
 ****************************************************************************/
void fapiCheckMandatoryTlvForFddAndSendResp ( 
                            FAPI_phyCellConfigRequest_st *cellConfigReq_p, UInt8 cellIndex )
{    
    FAPI_phyCellConfigResp_st  *configRspBody_p = PNULL;
    FAPI_l1ApiMsg_st           *l1ApiMsg_p = PNULL;
#ifdef FAPI_4_0_COMPLIANCE
    FAPI_cellConfig_st         mandatoryTlvForFdd[241]; 
    FAPI_cellConfig_st         misisngTagArr[241];
    FAPI_cellConfig_st         invalid_unSuppArr[241];
#else
    FAPI_cellConfig_st         mandatoryTlvForFdd[51]; 
    FAPI_cellConfig_st         misisngTagArr[51];
    FAPI_cellConfig_st         invalid_unSuppArr[51];
#endif
    FAPI_cellConfig_st         *configTlv_p = PNULL;
    UInt32                     i = 0, missingTagCount = 0, 
                               invalid_unsupTagCount = 0;
	UInt32                     l1ApiMsgSize = 0;
	UInt16                     l1ApiMsgBodySize = 0;
#ifdef FAPI_4_0_COMPLIANCE
	UInt8                      mandatoryTagArr[241] = {0};
#else
	UInt8                      mandatoryTagArr[51] = {0};
#endif
	FAPI_l1ErrorCodes_en       errCode = FAPI_MSG_OK;

#ifdef FAPI_4_0_COMPLIANCE
	memSet ( ( void *) mandatoryTlvForFdd, 0 , 241 * sizeof(FAPI_cellConfig_st));
	memSet ( ( void *) misisngTagArr, 0 , 241 * sizeof(FAPI_cellConfig_st));
	memSet ( ( void *) invalid_unSuppArr, 0 , 241 * sizeof(FAPI_cellConfig_st));
#else
	memSet ( ( void *) mandatoryTlvForFdd, 0 , 51 * sizeof(FAPI_cellConfig_st));
	memSet ( ( void *) misisngTagArr, 0 , 51 * sizeof(FAPI_cellConfig_st));
	memSet ( ( void *) invalid_unSuppArr, 0 , 51 * sizeof(FAPI_cellConfig_st));
#endif
	//Nitin
	cellConfigUeSim_g[cellIndex].prachConfigIndex = MAC_PHY_CONVERT_16(cellConfigReq_p->configtlvs[16].configParam.configurationIndex);
	MAC_EMULATOR_LOG(LOG_INFO, "cellConfigUeSim_g[cellIndex].prachConfigIndex %d in Cell config \n", cellConfigUeSim_g[cellIndex].prachConfigIndex);

	/* + CQI_4.1 */
	cellConfigUeSim_g[cellIndex].dlAvailableRBs = 
		cellConfigReq_p->configtlvs[5].configParam.dlBandWidthSupport;
     cellConfigUeSim_g[cellIndex].prachConfigIndex = MAC_PHY_CONVERT_16(cellConfigReq_p->configtlvs[16].configParam.configurationIndex);
     MAC_EMULATOR_LOG(LOG_INFO, "cellConfigUeSim_g[cellIndex].prachConfigIndex %d in Cell config \n", cellConfigUeSim_g[cellIndex].prachConfigIndex);
    /* + SPR_11013 */ 
    /*cellConfigUeSim_g[cellIndex].dlAvailableRBs = */
    /* - SPR_11013 */ 
    fprintf(stderr,"cellConfigUeSim_g.dlAvailableRBs %d in Cell config for cell %d \n",cellConfigUeSim_g[cellIndex].dlAvailableRBs, cellIndex);
    /* - CQI_4.1 */
    /* ULA_CHG */
    cellConfigUeSim_g[cellIndex].ulAvailableRBs = 
        MAC_PHY_CONVERT_16(cellConfigReq_p->configtlvs[6].configParam.ulBandWidthSupport);
    fprintf(stderr,"cellConfigUeSim_g.ulAvailableRBs %d in Cell config for cell %d\n",cellConfigUeSim_g[cellIndex].ulAvailableRBs, cellIndex);
    /* ULA_CHG */

#ifdef UESIM_ENHANCEMENTS
    cellConfigUeSim_g[cellIndex].n1PUCCH_AN = MAC_PHY_CONVERT_16(cellConfigReq_p->configtlvs[27].configParam.n1PucchAn);
#endif    

#ifdef TDD_CONFIG
     /* UL/DL Configuration and Special Sub Frame Configuration is being parsed
     */

    cellConfigUeSim_g[cellIndex].subFrameAssignment =
        MAC_PHY_CONVERT_16(cellConfigReq_p->configtlvs[36].configParam.subFrameAssignment);
    MAC_EMULATOR_LOG(LOG_INFO, "cellConfigUeSim_g[cellIndex].subFrameAssignment %d in Cell config \n",
        cellConfigUeSim_g[cellIndex].subFrameAssignment);
    

    /* Adding value to global variable used to determine UL/DL configuration.
     */
    /* CA TDD Changes Start */
    tddUlDlConfig_g[cellIndex] = cellConfigUeSim_g[cellIndex].subFrameAssignment;
    /* CA TDD Changes End */

    cellConfigUeSim_g[cellIndex].specialSubFramePattern =
        MAC_PHY_CONVERT_16(cellConfigReq_p->configtlvs[37].configParam.specialSubFramePatterns);
    MAC_EMULATOR_LOG(LOG_INFO, "cellConfigUeSim_g[cellIndex].specialSubFramePatterns %d in Cell config \n",
        cellConfigUeSim_g[cellIndex].specialSubFramePattern);
#endif
 
#ifdef FAPI_4_0_COMPLIANCE
	/* For FDD tags from 36 onwards or not coming so setting all to 1 */
	for ( i = 6; i <=9; i++ )
	{
		mandatoryTagArr[i] = 1;
	}
	for ( i = 15; i <=19; i++ )
	{
		mandatoryTagArr[i] = 1;
	}
	for ( i = 23; i <=29; i++ )
	{
		mandatoryTagArr[i] = 1;
	}
	for ( i = 33; i <=39; i++ )
	{
		mandatoryTagArr[i] = 1;
	}
	for ( i = 45; i <=49; i++ )
	{
		mandatoryTagArr[i] = 1;
	}
	for ( i = 53; i <=59; i++ )
	{
		mandatoryTagArr[i] = 1;
	}
	for ( i = 64; i <=69; i++ )
	{
		mandatoryTagArr[i] = 1;
	}
	for ( i = 74; i <=79; i++ )
	{
		mandatoryTagArr[i] = 1;
	}
	for ( i = 83; i <=240; i++ )
	{
		mandatoryTagArr[i] = 1;
	}
	
#else
	/* For FDD tags from 36 onwards or not coming so setting all to 1 */
	for ( i = 36; i <=50; i++ )
	{
		mandatoryTagArr[i] = 1;
	}
#endif
	configTlv_p = ( FAPI_cellConfig_st *) &(cellConfigReq_p->configtlvs[0]);
    /*FAPI_ERROR_INDICATION_CHANGES_START*/
    /*Send Fapi error FAPI_MSG_INVALID_CONFIG in case of unsuccessful
     * configuration*/
    if ((1 == phy_err_config_g[cellIndex])&&(errorCount_g[cellIndex] == 0))
    {
        if ((PHY_CELL_CONFIG_REQUEST == phy_err_msg_type_g[cellIndex]) && (FAPI_MSG_INVALID_CONFIG == phy_err_error_type_g[cellIndex]))
        {
            configTlv_p[0].tag = 60;
            mandatoryTagArr[1] = 2;
            errorCount_g[cellIndex]++;
        }
    }
    /*FAPI_ERROR_INDICATION_CHANGES_END*/
	for ( i = 0; i < cellConfigReq_p->numOfTlv; i++)
	{

#ifdef FAPI_4_0_COMPLIANCE	

		if ( configTlv_p[i].tag > 0 && configTlv_p[i].tag <= 240 )
		{
			mandatoryTlvForFdd[configTlv_p[i].tag] = configTlv_p[i];
			mandatoryTagArr[configTlv_p[i].tag] = 1;
		}
		else
		{
			errCode = FAPI_MSG_INVALID_CONFIG;
			invalid_unSuppArr[invalid_unsupTagCount++] = configTlv_p[i];
		}
#else

		if ( configTlv_p[i].tag > 0 && configTlv_p[i].tag <= 50 )
		{
			mandatoryTlvForFdd[configTlv_p[i].tag] = configTlv_p[i];
			mandatoryTagArr[configTlv_p[i].tag] = 1;
		}
		else
		{
			errCode = FAPI_MSG_INVALID_CONFIG;
			invalid_unSuppArr[invalid_unsupTagCount++] = configTlv_p[i];
		}

#endif
	}
#ifdef FAPI_4_0_COMPLIANCE
	for ( i = 1; i <=240; i++ )
	{
		if ( mandatoryTagArr[i] != 1 )
		{
			errCode = FAPI_MSG_INVALID_CONFIG;
			misisngTagArr[missingTagCount++] = mandatoryTlvForFdd[i];
		}
	}
#else
	for ( i = 1; i <=50; i++ )
	{
		if ( mandatoryTagArr[i] != 1 )
		{
			errCode = FAPI_MSG_INVALID_CONFIG;
			misisngTagArr[missingTagCount++] = mandatoryTlvForFdd[i];
		}
	}


#endif


    l1ApiMsgBodySize = sizeof(FAPI_paramResponse_st) + 
                       ((missingTagCount + invalid_unsupTagCount ) 
                           * sizeof(FAPI_cellConfig_st));
    l1ApiMsgSize = L1_PARTIAL_API_MSG_SIZE + l1ApiMsgBodySize;
    

    l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) getMemFromPool ( l1ApiMsgSize, PNULL);
    if ( PNULL == l1ApiMsg_p )
    {
        MAC_EMULATOR_LOG(LOG_INFO,"Error!! in ifunction fapiCheckMandatoryTlvForFddAndSendResp" 
                "allocation of buffer for phyCellConfigResp\n");
        return;
    }

    l1ApiMsg_p->msgId = PHY_CELL_CONFIG_RESPONSE;
    l1ApiMsg_p->lenVendorSpecific = 0;
    l1ApiMsg_p->msgLen = MAC_PHY_CONVERT_16(l1ApiMsgBodySize);
    
    configRspBody_p = ( FAPI_phyCellConfigResp_st *) &(l1ApiMsg_p->msgBody[0]); 

    configRspBody_p->errorCode = MAC_PHY_CONVERT_32(errCode);
    configRspBody_p->numOfInvalidOrUnsupportedTLV = invalid_unsupTagCount;
    configRspBody_p->numOfMissingTLV = missingTagCount;

    if ( invalid_unsupTagCount )
    {
        MAC_EMULATOR_LOG(LOG_INFO,"number of invalid_unsupported tag is %d\n", invalid_unsupTagCount);
        memCpy ( ( void *) &(configRspBody_p->listOfTLV[0]),
                 ( void *) &invalid_unSuppArr[0],
                 invalid_unsupTagCount);
    }
    if ( missingTagCount )
    {
        MAC_EMULATOR_LOG(LOG_INFO,"number of missing tag is %d\n", missingTagCount);
        memCpy ( ( void *) &(configRspBody_p->listOfMissingTlv[0]),
                 ( void *) &misisngTagArr[0],
                 missingTagCount);
    }

    /* send MSG to MAC on config port */
    MAC_EMULATOR_LOG(LOG_INFO,"Sending Cell configure Response to MAC: State FAPI_CONFIGURED \n");

#ifdef HO_SETUP_CHANGES
    /* +CA-SJ */
    if ( cellConfigRecvFlag[cellIndex] == 0 )
    {
        sendMsgtoL2Config( ( UInt8 *)l1ApiMsg_p, l1ApiMsgSize, cellIndex );
        cellConfigRecvFlag[cellIndex] = 1;
    }
    else if ( cellConfigRecvFlag[cellIndex] == 1 )
    {
        sendMsgtoTargetL2Config( ( UInt8 *)l1ApiMsg_p, l1ApiMsgSize, cellIndex );
    }
    /* -CA-SJ */
#else
    sendMsgtoL2Config( ( UInt8 *)l1ApiMsg_p, l1ApiMsgSize, cellIndex );
#endif

    freeMemPool(l1ApiMsg_p);
    l1ApiMsg_p = PNULL;
    /* send phy state to configured */
    phyState_g[cellIndex] = FAPI_CONFIGURED;

}

/* Cell Start Stop Chg */
/****************************************************************************
 * Function Name  : fapiConfigPhyCellForFddAndSendResp
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    : This function Parse PHY Config Msg while PHY is in Config
 *                  state
 ****************************************************************************/
void fapiConfigPhyCellForFddAndSendResp(
                            FAPI_phyCellConfigRequest_st *cellConfigReq_p, UInt8 cellIndex )
{
    FAPI_phyCellConfigResp_st  *configRspBody_p = PNULL;
    FAPI_l1ApiMsg_st           *l1ApiMsg_p      = PNULL;
    UInt8                      i                = 0;
    UInt32                     l1ApiMsgSize     = 0;
    UInt16                     l1ApiMsgBodySize = 0;
    FAPI_l1ErrorCodes_en       errCode          = FAPI_MSG_OK;
    UInt8                      numTlv           = cellConfigReq_p->numOfTlv;

    for(i = 0; i <= numTlv; i++)
    {
        switch(cellConfigReq_p->configtlvs[i].tag)
        {
            case FAPI_UL_CHANNEL_BANDWIDTH:
                cellConfigUeSim_g[cellIndex].ulAvailableRBs =
                    MAC_PHY_CONVERT_16(cellConfigReq_p->configtlvs[i].configParam.ulBandWidthSupport);
                i++;
                break;
            case FAPI_CONFIGURATION_INDEX:
                cellConfigUeSim_g[cellIndex].prachConfigIndex =
                    MAC_PHY_CONVERT_16(cellConfigReq_p->configtlvs[i].configParam.configurationIndex);
                i++;
                break;
            default:
                i++;
                break;
        }
    }
    l1ApiMsgBodySize = sizeof(FAPI_paramResponse_st);
    l1ApiMsgSize = L1_PARTIAL_API_MSG_SIZE + l1ApiMsgBodySize;

     l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) getMemFromPool ( l1ApiMsgSize, PNULL);  

    if ( PNULL == l1ApiMsg_p )
    {
        MAC_EMULATOR_LOG(LOG_INFO,"Error!! in ifunction "
                "allocation of buffer for fapiConfigPhyCellForFddAndSendResp phyCellConfigResp\n");
        return;
    }

    l1ApiMsg_p->msgId = PHY_CELL_CONFIG_RESPONSE;
    l1ApiMsg_p->lenVendorSpecific = 0;
    l1ApiMsg_p->msgLen = MAC_PHY_CONVERT_16(l1ApiMsgBodySize);

    configRspBody_p = ( FAPI_phyCellConfigResp_st *) &(l1ApiMsg_p->msgBody[0]);

    configRspBody_p->errorCode = MAC_PHY_CONVERT_32(errCode);
    configRspBody_p->numOfInvalidOrUnsupportedTLV = 0;
    configRspBody_p->numOfMissingTLV = 0;

     /* send MSG to MAC on config port */
    MAC_EMULATOR_LOG(LOG_INFO,"Sending Cell configure Response to MAC: State FAPI_CONFIGURED \n");

#ifdef HO_SETUP_CHANGES
    /* +CA-SJ */
    if ( cellConfigRecvFlag[cellIndex] == 0 )
    {
        sendMsgtoL2Config( ( UInt8 *)l1ApiMsg_p, l1ApiMsgSize, cellIndex );
        cellConfigRecvFlag[cellIndex] = 1;
    }
    else if ( cellConfigRecvFlag[cellIndex] == 1 )
    {
        sendMsgtoTargetL2Config( ( UInt8 *)l1ApiMsg_p, l1ApiMsgSize, cellIndex );
    }
    /* -CA-SJ */
#else
    sendMsgtoL2Config( ( UInt8 *)l1ApiMsg_p, l1ApiMsgSize , cellIndex);
#endif

    freeMemPool(l1ApiMsg_p);
    l1ApiMsg_p = PNULL;
}
/* Cell Start Stop Chg */

/****************************************************************************
 * Function Name  : fapiHandlePhyCellConfigRequest 
 * Inputs         :  
 * Outputs        :  
 * Returns        :  
 * Description    : 
 ****************************************************************************/
void fapiHandlePhyCellConfigRequest ( FAPI_l1ApiMsg_st *recvMsg_p , UInt8 cellIndex)
{
   FAPI_phyCellConfigRequest_st *cellConfigReq_p = PNULL;

   cellConfigReq_p = ( FAPI_phyCellConfigRequest_st *) &(recvMsg_p->msgBody[0]);

    /*FAPI_ERROR_INDICATION_CHANGES_START*/
    /*Do not send fapi cell config response incase of timer expiry*/
    if (1 == phy_err_config_g[cellIndex])
    {
        if ((PHY_CELL_CONFIG_REQUEST == phy_err_msg_type_g[cellIndex]) && (FAPI_TIMER_EXPIRY == phy_err_error_type_g[cellIndex]))
        {
            return;
        }
    }
    /*FAPI_ERROR_INDICATION_CHANGES_END*/

#ifdef HO_SETUP_CHANGES
   uint8_t numTlv = cellConfigReq_p->numOfTlv;

    /* Inter Cell L2 Ho Change Start */
    /* SPR 6143 changes start */
#ifdef INTER_CELL_HO
   if ( paramReqRecvFlag[cellIndex] == 1 )
   {
           phyState_g[cellIndex] = FAPI_IDLE;
   }
#endif
    /* SPR 6143 changes end */
   /* Inter Cell L2 Ho Change End */
   
   /* On the basis of number of Tlv check for the case of Cell Reconfig */ 
   /*if (numTlv == 3 && FAPI_RUNNING == phyState_g)
   {
       fapiSendCellConfigRspInRunningState(); 
   }*/
   /*PHY_RECONF_CHG*/
   if (FAPI_CONFIGURED == phyState_g[cellIndex] || FAPI_RUNNING == phyState_g[cellIndex])
   {
       fapiConfigPhyCellForFddAndSendResp(cellConfigReq_p, cellIndex);
   }
   /*PHY_RECONF_CHG*/
   else if(FAPI_IDLE == phyState_g[cellIndex])
   {   
       fapiCheckMandatoryTlvForFddAndSendResp (cellConfigReq_p, cellIndex);
   } 
   
#else
   if ( FAPI_RUNNING != phyState_g[cellIndex] )
   {
       /* check for mandatory TLV for FDD  and send resp back to L2/L3*/
       fapiCheckMandatoryTlvForFddAndSendResp (cellConfigReq_p, cellIndex);
   }
   else
   {
       fapiSendCellConfigRspInRunningState(cellIndex);
   }
/* SPR 21660_2 changes end */
#endif
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
	if(DL_RES_BLKS_100RBS == cellConfigUeSim_g[cellIndex].dlAvailableRBs)    /* For 20 MHz Bandwidth */
	{
		numRBGConfigured_g[cellIndex] = NUM_OF_RBG_BW20;
		rbgSize_g[cellIndex] = RBG_SIZE_FOR_BW20;
	}
	else if(DL_RES_BLKS_75RBS == cellConfigUeSim_g[cellIndex].dlAvailableRBs)  /* For 15 MHz Bandwidth */
	{
		numRBGConfigured_g[cellIndex] = NUM_OF_RBG_BW15;
		rbgSize_g[cellIndex] = RBG_SIZE_FOR_BW15;
	}
	else if(DL_RES_BLKS_50RBS == cellConfigUeSim_g[cellIndex].dlAvailableRBs) /* For 10 MHz Bandwidth */
	{
		numRBGConfigured_g[cellIndex] = NUM_OF_RBG_BW10;
		rbgSize_g[cellIndex] = RBG_SIZE_FOR_BW10;
	}
	else if(DL_RES_BLKS_25RBS == cellConfigUeSim_g[cellIndex].dlAvailableRBs) /* For 5 MHz Bandwidth */
	{
		numRBGConfigured_g[cellIndex] = NUM_OF_RBG_BW5;
		rbgSize_g[cellIndex] = RBG_SIZE_FOR_BW5;
	}
	else if(DL_RES_BLKS_15RBS == cellConfigUeSim_g[cellIndex].dlAvailableRBs) /* For 3 MHz Bandwidth */
	{
		numRBGConfigured_g[cellIndex] = NUM_OF_RBG_BW3;
		rbgSize_g[cellIndex] = RBG_SIZE_FOR_BW3;
	}
	else if(DL_RES_BLKS_6RBS == cellConfigUeSim_g[cellIndex].dlAvailableRBs) /* For 1.4 MHz Bandwidth */
	{
		numRBGConfigured_g[cellIndex] = NUM_OF_RBG_BW1DOT4;
		rbgSize_g[cellIndex] = RBG_SIZE_FOR_BW1DOT4;
	}
	else
	{
		fprintf(stderr,"PUSCH RAT1 DEBUG: Incorrect Bandwidth Configuration \n");
	}

#endif
/* SPR 21958 PUSCH RAT1 Support End */
}

/****************************************************************************
 * Function Name  : fapiStopIndication  
 * Inputs         :  
 * Outputs        :  
 * Returns        :  
 * Description    : 
 ****************************************************************************/
void fapiStopIndication ( UInt8 ** stopResp_pp , UInt32 * stopRspmsgSize_p, UInt8 cellIndex)
{
    UInt8               *sendBuffer_p = PNULL;
    FAPI_l1ApiMsg_st    *stopInd_p = PNULL;
    UInt16              apiMsgLen = 0;

    apiMsgLen = L1_PARTIAL_API_MSG_SIZE;
    sendBuffer_p = ( UInt8 *) getMemFromPool( apiMsgLen, PNULL );
    if ( PNULL == sendBuffer_p )
    {
        MAC_EMULATOR_LOG(LOG_INFO," Error..!! in allocation of buffer for fapiStopIndication \n");
        return;
    }
    stopInd_p = ( FAPI_l1ApiMsg_st *) sendBuffer_p;
    stopInd_p->msgId = PHY_STOP_INDICATION;
    stopInd_p->lenVendorSpecific = 0;
    stopInd_p->msgLen = 0;
    /* fapi changes start */
    *stopResp_pp = sendBuffer_p;
    *stopRspmsgSize_p= apiMsgLen;
    /* fapi changes end */   

    /* send MSG to MAC on data port */
    resetGlobals(cellIndex); 
    sendMsgtoL2Config(sendBuffer_p, apiMsgLen, cellIndex);  
    freeMemPool(sendBuffer_p);
    sendBuffer_p = PNULL;
}

/****************************************************************************
 * Function Name  :  fapiSendErrorIndication 
 * Inputs         :  Error Codes
 * Outputs        :  
 * Returns        :  
 * Description    : 
 ****************************************************************************/
void fapiSendErrorIndication( FAPI_l1ErrorCodes_en errCode, 
        FAPI_msgIdType_t msgId,
        UInt16 recvdSfnSf, UInt16 expectedSfnSf, 
        UInt8 **paramRes_p, UInt32 *l1ApiMsgSize ,UInt8 cellIndex )
{
    FAPI_phyErrorIndication_st *errInd_p = PNULL;
    FAPI_l1ApiMsg_st *l1ApiMsg_p = PNULL;
    UInt8  *sendBuffer_p = *paramRes_p;
    UInt16 errIndSize = 0;

    errIndSize = sizeof(FAPI_phyErrorIndication_st);
    *l1ApiMsgSize = L1_PARTIAL_API_MSG_SIZE + errIndSize;
    sendBuffer_p = ( UInt8 *) getMemFromPool ( *l1ApiMsgSize, PNULL );
    if ( PNULL == sendBuffer_p )
    {
        MAC_EMULATOR_LOG(LOG_INFO," Error...!! in fapiSendErrorIndication" 
                "allocation of buffer for errCode = %d, msgId = %d \n",
                errCode, msgId);
        return;
    }
    l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) (sendBuffer_p);
    l1ApiMsg_p->msgId = PHY_ERROR_INDICATION;
    l1ApiMsg_p->lenVendorSpecific = 0;
    l1ApiMsg_p->msgLen = MAC_PHY_CONVERT_16(errIndSize); 

    errInd_p = (FAPI_phyErrorIndication_st*)l1ApiMsg_p->msgBody;
    errInd_p->msgId = msgId;
    errInd_p->errorCode = MAC_PHY_CONVERT_32(errCode);
    *paramRes_p=(UInt8 *)(l1ApiMsg_p);

    switch ( errCode )
    {
        case FAPI_MSG_INVALID_STATE:
        case FAPI_MSG_BCH_MISSING:
        {
            break;
        }

        case FAPI_MSG_INVALID_SFN:
        case FAPI_SFN_OUT_OF_SYNC:
        {
            errInd_p->errMsgBody.msgBody1.recvdSfnSf = MAC_PHY_CONVERT_16( recvdSfnSf );
            errInd_p->errMsgBody.msgBody1.expectedSfnSf = MAC_PHY_CONVERT_16( expectedSfnSf );
            break;
        }

        case FAPI_MSG_SUBFRAME_ERR:
        {
            errInd_p->errMsgBody.msgBody2.subErrCode = phy_err_sub_err_code_g[cellIndex];
            errInd_p->errMsgBody.msgBody2.rnti = phy_err_crnti_g[cellIndex];
            errInd_p->errMsgBody.msgBody2.pduType = phy_err_pdu_type_g[cellIndex];
            if (PHY_DL_CONFIG_REQUEST == msgId)
            {
                errInd_p->errMsgBody.msgBody2.direction = DL_SUBFRAME_CONFIGURATION;
            }
            else 
            {
                errInd_p->errMsgBody.msgBody2.direction = UL_SUBFRAME_CONFIGURATION;
            }
            break;
        }
        case FAPI_MSG_HI_ERR:
        {
            errInd_p->errMsgBody.msgBody3.subErrCode = phy_err_sub_err_code_g[cellIndex];
            errInd_p->errMsgBody.msgBody3.phichLowestulRbIndex = phy_err_rb_start_g[cellIndex];
            break;
        }
        case FAPI_MSG_TX_ERR:
        {
            errInd_p->errMsgBody.msgBody4.subErrCode = phy_err_sub_err_code_g[cellIndex];
            errInd_p->errMsgBody.msgBody4.pduIndex = phy_err_pdu_index_g[cellIndex];
            break;
        }
        default:
        {
            break;
        }
    }

#ifdef UE_SIM_ENABLE_PRINTF_LOGS
    fprintf(stderr, "L1 prepared error indication, errCode[%d]\n", errCode );
#endif
}

/****************************************************************************
 * Function Name  :  fapiHandlePhyStopRequest 
 * Inputs         :  STOP request 
 * Outputs        :  
 * Returns        :  
 * Description    : This function will validate the stop request recv from
 *                  peer MAC, create the response and send back to MAC.
 ****************************************************************************/
void fapiHandlePhyStopRequest ( FAPI_l1ApiMsg_st *recvMsg_p, UInt8 cellIndex )
{
    FAPI_l1ErrorCodes_en errCode = FAPI_MSG_OK;
    UInt8 *stopResp_p           = PNULL;
    UInt32 stopRspmsgSize       = 0;
    UInt8  ix                   = 0;

    if ( recvMsg_p->msgLen != 0 )
    {
        MAC_EMULATOR_LOG(LOG_INFO," Error..!! Stop Request Msg Body legth = %d" 
                "is non zero \n", recvMsg_p->msgLen);
        return;
    }

    /*Do not send fapi stop response incase of timer expiry*/
    if (1 == phy_err_config_g[cellIndex])
    {
        if ( ( PHY_STOP_REQUEST == phy_err_msg_type_g[cellIndex] ) && 
                ( FAPI_TIMER_EXPIRY == phy_err_error_type_g[cellIndex] ) )
        {
            return;
        }
    }
    /* Send error indication for Invalid phy state*/
    if ((1 == phy_err_config_g[cellIndex])&&(errorCount_g[cellIndex] == 0))
    {
        if ( ( PHY_STOP_REQUEST == phy_err_msg_type_g[cellIndex] ) &&
                ( FAPI_MSG_INVALID_STATE == phy_err_error_type_g[cellIndex] ) )
        {
            phyState_g[cellIndex] = FAPI_CONFIGURED;
            errorCount_g[cellIndex]++;
        }
    }

    if (  FAPI_RUNNING == phyState_g[cellIndex] )
    {
        /* set the registration flag to REG_SUCCESS */
        phyState_g[cellIndex] = FAPI_CONFIGURED;
        /* SPR 22578 fix start*/
        cellStopTriggered[cellIndex] = TRUE ;
        /* SPR 22578 fix end*/
        fapiStopIndication(&stopResp_p,&stopRspmsgSize, cellIndex);
        /* + SPR_11130 */
        cellStateAfterCellStop[cellIndex] = CELL_START_AFTER_CELL_STOP;
        /* - SPR_11130 */
        for (ix=0; ix<MAX_NUM_CELLS; ix++)
        {
            /* + SPR_11100 */
            if ((!(stoppedProcessing_g[ix])) &&
                    (cellConfigured_g[ix]) && (macRegFlag_g[ix]) )
            /* - SPR_11100 */
             {  /* This is required so that if another cell uis waiting for syn then 
                 *from here it got the post*/
                /* + SPR_11585 */
                if ( cellIndex == getActiveCell())
                {
                    /* + SPR_11574 */
                    eventfd_write_wrapper(ev_fd_SFI_g,1);
                    /* - SPR_11574 */
                } 
                /* + SPR_11914 */
                if (1 == isEvFdRead[ix])
                {
                   eventfd_write_wrapper(ev_fd_g[ix][cellIndex],1);
                }  
                /* -+ SPR_11914 */
                /* - SPR_11585 */
                setActiveCell(ix);
                break;
            }
        }
        /*Start: Fix for SPR 10963, commenting the below code so that 
        RLC/PDCP delete/pending queues can be processed in the same tick*/
        /*
        if (MAX_NUM_CELLS == ix)
        {
            setActiveCell(INVALID_CELL_INDEX);
        }
        */
        /*End: Fix for SPR 10963*/ 
    }
    else
    {
	    errCode = FAPI_MSG_INVALID_STATE;
	    fapiSendErrorIndication(errCode, recvMsg_p->msgId, 0,0, &stopResp_p, 
			    &stopRspmsgSize, cellIndex);
    /* + SPR_10930 */
#ifdef HO_SETUP_CHANGES
	    sendMsgtoTargetL2Config( (UInt8 *)stopResp_p, stopRspmsgSize, cellIndex );
#else
	    sendMsgtoL2Config( (UInt8 *)stopResp_p, stopRspmsgSize, cellIndex );
#endif
    /* - SPR_10930 */
    }

    /* send data to L2/L3 software */

    freeMemPool(stopResp_p);
    stopResp_p = PNULL;
}


/****************************************************************************
 * Function Name  :  fapiHandlePhyParamRequest 
 * Inputs         :  PARAM request 
 * Outputs        :  
 * Returns        :  
 * Description    : 
 ****************************************************************************/
void fapiHandlePhyParamRequest( FAPI_l1ApiMsg_st *recvMsg_p , UInt8 cellIndex)
{
    FAPI_l1ErrorCodes_en errCode = FAPI_MSG_OK;
    UInt8 *paramResp_p           = PNULL;
    UInt32 paramRspmsgSize       = 0;

    /*Do not send fapi cell config response incase of timer expiry*/
    if ((1 == phy_err_config_g[cellIndex])&&(PHY_PARAM_REQUEST == phy_err_msg_type_g[cellIndex]))
    {
        if (FAPI_TIMER_EXPIRY == phy_err_error_type_g[cellIndex])
        {
            return;
        }
    }
    /* Send error indication for Invalid phy state*/
    if ((1 == phy_err_config_g[cellIndex])&&(errorCount_g[cellIndex] == 0) &&
            (PHY_PARAM_REQUEST == phy_err_msg_type_g[cellIndex]))
    {
        if (FAPI_MSG_INVALID_STATE == phy_err_error_type_g[cellIndex])
        {
            phyState_g[cellIndex] = FAPI_RUNNING;
            errCode = FAPI_MSG_INVALID_STATE;
            errorCount_g[cellIndex]++;
        }
    }

    /* Do not consider the PHY state for target eNB as in HO, L1 is common */
    if( 0 == paramReqRecvFlag[cellIndex] )
    {
        if ( FAPI_RUNNING == phyState_g[cellIndex] )
        {
            errCode = FAPI_MSG_INVALID_STATE;
        }
    }
    
    fapiSendParamResp( &paramResp_p, &paramRspmsgSize, errCode, cellIndex );

    /* send data to L2/L3 software */
#ifdef HO_SETUP_CHANGES
    if ( paramReqRecvFlag[cellIndex] == 0 )
    {
        sendMsgtoL2Config( (UInt8 *)paramResp_p, paramRspmsgSize, cellIndex );
        paramReqRecvFlag[cellIndex] = 1;
    }
    else if ( paramReqRecvFlag[cellIndex] == 1 )
    {
/* SPR 11646*/
        isCellOfTargetEnb[cellIndex] = 1;
/* SPR 11646*/
        sendMsgtoTargetL2Config( (UInt8 *)paramResp_p, paramRspmsgSize, 
                cellIndex );
    }
    /* tweak added ineed to remove*/
    /* Inter Cell L2 Ho Change Start */
    /* SPR 6143 changes start */
#ifndef INTER_CELL_HO
    phyState_g[cellIndex] = FAPI_IDLE;
#endif
    /* SPR 6143 changes end */
   /* Inter Cell L2 Ho Change End */
#else
    sendMsgtoL2Config( (UInt8 *)paramResp_p, paramRspmsgSize, cellIndex );
#endif
    freeMemPool(paramResp_p);
    paramResp_p = PNULL;
}

/****************************************************************************
 * Function Name  :  fapiHandlePhyStartRequest 
 * Inputs         :  START request 
 * Outputs        :  
 * Returns        :  
 * Description    : This function will validate the start request recv from
 *                  peer MAC, create the response and send back to MAC.
 ****************************************************************************/
void fapiHandlePhyStartRequest ( FAPI_l1ApiMsg_st *recvMsg_p, UInt8 cellIndex )
{
    UInt8 *startResp_p           = PNULL;
    UInt32 startRspmsgSize       = 0;
    FAPI_l1ErrorCodes_en errCode;
    /* + SPR_11130 */
    UInt8 cellCount = 0;
    UInt8 toBeReset = 0;
    /* - SPR_11130 */

    if ( recvMsg_p->msgLen != 0 )
    {
        MAC_EMULATOR_LOG(LOG_INFO," Error..!! Start Request Msg Body legth = %d "
                "is non zero \n", recvMsg_p->msgLen);
        return;
    }

    /*Reset global variables in case of system recovery for subframe errors*/
    if ((1 == phy_err_config_g[cellIndex]) && errorCount_g[cellIndex] &&
            (FAPI_MSG_INVALID_STATE == phy_err_error_type_g[cellIndex]) && 
            (PHY_DL_CONFIG_REQUEST <= phy_err_msg_type_g[cellIndex] ))
    {
        resetGlobals(cellIndex);
    }
    /*Do not send subframe indication incase of timer expiry*/
    if (1 == phy_err_config_g[cellIndex])
    {
        if ((PHY_START_REQUEST == phy_err_msg_type_g[cellIndex]) &&
                (FAPI_TIMER_EXPIRY == phy_err_error_type_g[cellIndex]))
        {
            return;
        }
    }
    /* Send error indication for Invalid phy state*/
    if ((1 == phy_err_config_g[cellIndex]) && (errorCount_g[cellIndex] == 0))
    {
        if ((PHY_START_REQUEST == phy_err_msg_type_g[cellIndex]) &&
                (FAPI_MSG_INVALID_STATE == phy_err_error_type_g[cellIndex]))
        {   
            phyState_g[cellIndex] = FAPI_RUNNING;
            errorCount_g[cellIndex]++;
        }
    }

    if (  FAPI_CONFIGURED == phyState_g[cellIndex] )
    {
        /* set the registration flag to REG_SUCCESS */
        macRegFlag_g[cellIndex] = REG_SUCCESS;
        MAC_EMULATOR_LOG(LOG_INFO,"Start Communication with MAC Layer is Successful.\n");
        phyState_g[cellIndex] = FAPI_RUNNING;
        cellConfigured_g[cellIndex] = TRUE;
        /* + SPR_11914 */
        if (INVALID_CELL_INDEX == getActiveCell())
     	{ 
	         setActiveCell(cellIndex);
	    } 
        /* + SPR_11914 */
        /* + SPR_11130 */
        for (cellCount = 0 ; cellCount < MAX_NUM_CELLS; cellCount++)
        {
            if (cellStateAfterCellStop[cellCount] == CELL_START_AFTER_CELL_STOP)
            {
                toBeReset++;
            }

        }
        if (toBeReset == MAX_NUM_CELLS)
        {
             setActiveCell(cellIndex);
        } 
        /* - SPR_11130 */
        /* + SPR_11585 */
        cellStateAfterCellStop[cellIndex] = CELL_START_AFTER_INIT; 
        /* - SPR_11585 */
#ifdef FAPI_L2_MASTER
        firstDlConfigAfterPhyStart_g[cellIndex] = 0;
#endif

#ifdef HO_SETUP_CHANGES
        if ( startRecvFlag[cellIndex] == 0 )
            startRecvFlag[cellIndex] = 1;
        else if ( startRecvFlag[cellIndex] == 1 )
            startRecvFlag[cellIndex] = 2;

#endif
#ifdef UE_SIM_SPLIT
        fapiSendSubFrameStartIndication ( );
#endif
    }
    else
    {
        errCode = FAPI_MSG_INVALID_STATE;
        fapiSendErrorIndication(errCode, recvMsg_p->msgId, 0,0, &startResp_p, &startRspmsgSize, cellIndex);
#ifdef HO_SETUP_CHANGES
        sendMsgtoTargetL2Config( (UInt8 *)startResp_p, startRspmsgSize, cellIndex );
#else
        sendMsgtoL2Config( (UInt8 *)startResp_p, startRspmsgSize, cellIndex );
#endif
        /* Free the memory for error response code */
        freeMemPool(startResp_p);
        startResp_p = PNULL;
    }
}

/****************************************************************************
 * Function Name  : fapiProcessConfigMsg  
 * Inputs         :  
 * Outputs        :  
 * Returns        :  
 * Description    : 
 ****************************************************************************/
void fapiProcessConfigMsg ( UInt8 *recvBuf_p, SInt32 bytesRead, UInt8 cellIndex )
{
    FAPI_l1ApiMsg_st *l1RcvMsg_p =  ( FAPI_l1ApiMsg_st *) recvBuf_p;

    switch ( l1RcvMsg_p->msgId )
    {
        case PHY_PARAM_REQUEST:    
            {
                MAC_EMULATOR_LOG(LOG_INFO," PHY_PARAM_REQUEST recived \n");
                fapiHandlePhyParamRequest( l1RcvMsg_p, cellIndex );
            }
            break;
        case PHY_START_REQUEST:
            {
                MAC_EMULATOR_LOG(LOG_INFO," PHY_START_REQUEST recived \n");
                fapiHandlePhyStartRequest( l1RcvMsg_p, cellIndex );
            }
            break;

        case PHY_STOP_REQUEST:
            {
                MAC_EMULATOR_LOG(LOG_INFO," PHY_STOP_REQUEST recived \n");
                fapiHandlePhyStopRequest( l1RcvMsg_p, cellIndex );
            }
            break;

        case PHY_CELL_CONFIG_REQUEST:
            {
                MAC_EMULATOR_LOG(LOG_INFO," PHY_CELL_CONFIG_REQUEST recvd\n");
                fapiHandlePhyCellConfigRequest(l1RcvMsg_p, cellIndex);
            }
            break;

        case PHY_UE_CONFIG_REQUEST:
            {
                MAC_EMULATOR_LOG(LOG_INFO," PHY_UE_CONFIG_REQUEST recvd\n");
                fapiHandleUeConfigRequest(l1RcvMsg_p, cellIndex );
            }
            break;
        default:
            MAC_EMULATOR_LOG(LOG_INFO,"unsupportd Msg Id = %d recevied from L2/L3\n", l1RcvMsg_p->msgId);
            break;

    }

}

/* SPR 21660 changes end_Sahil */
