/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRelayCommon.h,v 1.1.1.1 2010/02/11 04:51:36 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains variable declarations for Packet
 *                   : Relay and other Interface.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRelayCommon.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:36  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 ****************************************************************************/

#ifndef LTE_RELAY_COMMON_H
#define LTE_RELAY_COMMON_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"
#include "lteRelayS1Sap.h"
#ifdef LAYER2_DLTG_THREAD
#include "lteLayer2DLTG.h"
#endif

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
extern void addTunnelIdentier(UInt32, UInt16, UInt8);
/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/******************************************************************************
  Private Definitions
 *****************************************************************************/
void appendApiHeader
(
    UInt16 moduleId,
    UInt16 apiId,
    UInt16 transId,
    UInt16 msgLen,
    UInt8 *respBuff_p,
/*  + Layer2 NON CA Changes */  
    RrcCellIndex cellId
/*  - Layer2 NON CA Changes */  
);

/****************************************************************************
 * Function Name  : processRelayInitDbCnf
 * Inputs         : respMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelayInitDbCnf(UInt8 *respMsg_p,UInt16 transId,UInt16 totalLen);

/****************************************************************************
 * Function Name  : processRelaySetTimerCnf
 * Inputs         : respMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelaySetTimerCnf(UInt8 *respMsg_p,UInt16 transId,UInt16 totalLen);

/****************************************************************************
 * Function Name  : processRelayGetTimerCnf
 * Inputs         : respMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelayGetTimerCnf(UInt8 *respMsg_p,UInt16 transId,UInt16 totalLen);

/****************************************************************************
 * Function Name  : processRelayGetMaxRetriesCnf
 * Inputs         : respMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelayGetMaxRetriesCnf(UInt8 *respMsg_p,UInt16 transId, UInt16 totalLen);

/****************************************************************************
 * Function Name  : processRelaySetMaxRetriesCnf
 * Inputs         : respMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelaySetMaxRetriesCnf( UInt8 *respMsg_p, UInt16 transId, UInt16 totalLen);

/****************************************************************************
 * Function Name  : processRelayInitTraceLvlCnf
 * Inputs         : respMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelayInitTraceLvlCnf( UInt8 *respMsg_p,UInt16 transId, UInt16 totalLen);

/****************************************************************************
 * Function Name  : processRelaySetTraceLvlCnf
 * Inputs         : respMsg_p,ueContext_p,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelaySetTraceLvlCnf(UInt8 *respMsg_p,UInt16 transId, UInt16 totalLen);

/****************************************************************************
 * Function Name  : processRelayGetTraceLvlCnf
 * Inputs         : respMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelayGetTraceLvlCnf( UInt8 *respMsg_p, UInt16 transId, UInt16 totalLen);

/****************************************************************************
 * Function Name  : processRelaySetupSapCnf
 * Inputs         : inMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to S1-AP.
 ****************************************************************************/

UInt32 processRelaySetupSapCnf(UInt8 *inMsg_p,UInt16 transId,UInt16 totalLen);


/****************************************************************************
 * Function Name  : processRelayReleaseSapCnf
 * Inputs         : respMsg_p
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to S1-AP.
 ****************************************************************************/

UInt32 processRelayReleaseSapCnf(UInt8 *respMsg_p,UInt16 transId,UInt16 totalLen);

/****************************************************************************
 * Function Name  : processRelayCpErrorSapInd
 * Inputs         : respMsg_p
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to S1-AP.
 ****************************************************************************/

UInt32 processRelayCpErrorSapInd(UInt8 *inMsg_p,UInt16 transId, UInt16 totalLen);

/****************************************************************************
 * Function Name  : processRelayPathFailureInd
 * Inputs         : respMsg_p, transId, totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion sends Path Failure Ind to S1-AP.
 ****************************************************************************/

UInt32 processRelayPathFailureInd(UInt8 *respMsg_p,UInt16 transId, UInt16 totalLen);

/*+SPR #3456*/
/****************************************************************************
 * Function Name  : processRelayPathSuccessInd
 * Inputs         : respMsg_p, transId, totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion sends Path Success Ind to S1-AP.
 ****************************************************************************/

UInt32 processRelayPathSuccessInd(UInt8 *respMsg_p,UInt16 transId, UInt16 totalLen);
/*-SPR #3456*/

UInt32 processRelayCpEndMarkerInd(UInt8 *respMsg_p,UInt16 transId, UInt16 totalLen);
UInt32 processRelayPdcpEndMarkerInd(UInt8 *respMsg_p,UInt16 transId, UInt16 totalLen);


/****************************************************************************
 * Function Name  : processRelayDataInd
 * Inputs         : inMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send data to Pdcp Apapter.
 ****************************************************************************/

UInt32 processRelayDataInd(UInt8 *inMsg_p,UInt16 transId,UInt16 totalLen);

/****************************************************************************
 * Function Name  : appendCsplHeader
 * Inputs         : moduleId,apiId,totalLen,msgBuff_p
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion append CSPL Header.
 ****************************************************************************/

void appendCsplHeader(UInt16 moduleId,UInt16 apiId,UInt16 totalLen,UInt8* msgBuff_p);

/*SPR 3587 Fix Start*/
/****************************************************************************
 * Function Name  : appendCsplHeaderDL
 * Inputs         : moduleId,apiId,totalLen,msgBuff_p
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion append CSPL Header in case of DL data.
 ****************************************************************************/

void appendCsplHeaderForDL(UInt16 moduleId,UInt16 apiId,UInt16 totalLen,UInt8* msgBuff_p);
/*SPR 3587 Fix End*/

/****************************************************************************
 * Function Name  : prepareAndSendErrorReportToLM
 * Inputs         : transId,apiId
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send faliure Rsp to LM.
 ****************************************************************************/

UInt32 prepareAndSendErrorReportToLM(UInt16 transId,UInt16 apiId);

/****************************************************************************
 * Function Name  : prepareAndSendErrorReportToCP
 * Inputs         : sapReq_p,transId
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send faliure Rsp to CP.
 ****************************************************************************/

UInt32 prepareAndSendErrorReportToCP(relayGtpuSetupSapReq *sapReq_p,UInt16 transId);
void sendPacketToPeerGtpu(UInt8 *data_p);
void* udpSendToAdapter(char *buff_p, int len);
void* udpSendToS1ap(char *buff_p, int len);
void* udpSendToOam(char *buff_p, int len);
/*SPR 21188 +*/
#if defined PDCP_GTPU_INTF
void* udpSendToPdcp(char *buff_p, int len, UInt8 instanceId);
#endif
/*SPR 21188 -*/

void populateTransportLayerAddress(UInt8 *pOutMsg,UInt16 *pCurrent);

/* SPR 2030 fix Start */
/* Logging API declaration Start */

UInt32 processRelayGetLogLevelResp(UInt8 *respMsg_p,UInt16 transId,
        UInt16 totalLen);
UInt32 processRelayGetLogCategoryResp(UInt8 *respMsg_p,UInt16 transId,
        UInt16 totalLen);
void pdcpFirstSubFrameInd(UInt32 sysFrameNum, UInt32 subFrameNum);
/* + SPR 17439 */
void pdcpSubFrameInd(void);
/* - SPR 17439 */
/* SPR 2030 fix Start */
/* SPR 3085 fix Start */
/* SPR 5481 Fix Start */
int triggerDeleteUeEntityReq(UInt16 ueIndex);
/* SPR 5481 Fix End */
/* SPR 3085 fix end  */
/* LIPA changes start */
/****************************************************************************
 * Function Name  : prepareAndSendErrorReportToCP
 * Inputs         : sapReq_p,transId
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send faliure Rsp to CP.
 ****************************************************************************/
#ifndef EPC_SETUP
UInt8 sendMessageToDlTgForSecondaryTunnel(UInt16 ueIndex,
                                          UInt8 lc_id,
                                          UInt32 teid_self,
                                          UInt32 teid_peer,
                                          UInt8 tunnelType);

UInt8 sendMessageToDlTg (UInt16 ueIndex,
        UInt8 lc_id,
        UInt32 teid_self,
        UInt32 teid_peer,
        UInt8 * outMsgTG_p,
        UInt16 currentTG,
        UInt8 tunnelType,
        UInt8 enbType,
        /* SPR 20646 fix start */           
        UInt16 Addr_len                
        /* SPR 20646 fix end */
        );
#ifdef ENDC_ENABLED
UInt8 sendTunnelInfoToDlTg (UInt16 ueIndex,UInt8 lc_id,UInt32 teid_self, 
        UInt32 teid_peer,UInt8 tunnelType, UInt8 enbType, 
        UInt16 Addr_len,
        UInt8 *transportAddr
        );
#endif

#endif
/* LIPA changes end */
/* SPR 7085 fix start */
/****************************************************************************
 * Function Name  : processRelayGetStatsResp
 * Inputs         : respMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send stats resp to OAM.
 ****************************************************************************/
UInt32 processRelayGetStatsResp(UInt8 *respMsg_p,
                                UInt16 transId,
                                UInt16 totalLen);

/* SPR 7085 fix end */
/*spr 22474 start*/
/****************************************************************************
 * Function Name  : processRelayGetDebugInfoResp
 * Inputs         : respMsg_p - pointer to the response message buffer,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/
UInt32 processRelayGetDebugInfoResp
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
);
/*spr 22474 end*/

/* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
#ifndef UE_SIM_TESTING
/* SPR 15909 fix start */
extern void pdcpFirstSubFrameIndFrmL2Tick(tickType_t currentTickFrmL2Tick);
/* SPR 15909 fix end */
#endif
#endif
/* FAPI L2 AS MASTER END */
#ifdef ENDC_ENABLED
void sendMessageToX2uRelay(UInt8 *msgBuf_p, SInt32 bytesread);
#endif
#endif
