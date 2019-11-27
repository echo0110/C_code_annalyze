/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteFapiControlMsgHandler.h,v 1.1.4.1 2010/09/21 15:53:20 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the Interface API between the 
 *                     MAC and Phy Simulator Layer in a LTE system.
 *
 ****************************************************************************
 *
 * Revision Details :
 * $Log: lteFapiControlMsgHandler.h,v $
 * Revision 1.1.4.1  2010/09/21 15:53:20  gur20491
 * FAPI changes
 *
 * Revision 1.1.2.1  2010/09/07 11:21:07  gur20439
 * new files added for FAPI changes
 *1,v $
 ****************************************************************************/

#ifndef FAPI_CONTROL_MSG_HANDLER_H
#define FAPI_CONTROL_MSG_HANDLER_H


#include "lteLayer2Types.h"
#include "lteFapiMacPhyInterface.h"
#include "phySimulatorApiHandler.h"


#define FAPI_MAX_DCI_PDU 10
#define L1_PARTIAL_API_MSG_SIZE ( sizeof(FAPI_l1ApiMsg_st) - 2)
/* SPR 973 Start */
#ifdef FAPI_4_0_COMPLIANCE
#ifdef LTE_EMBMS_SUPPORTED
#define PHY_PARAM_RESP_MSG_SIZE  sizeof(uint8_t) + sizeof(FAPI_l1ErrorCodes_en) + 3/*padding*/ + (8 * sizeof(FAPI_paramResponseTLV_st))
#else
#define PHY_PARAM_RESP_MSG_SIZE  sizeof(uint8_t) + sizeof(FAPI_l1ErrorCodes_en) + 3/*padding*/ + (7 * sizeof(FAPI_paramResponseTLV_st))
#endif
#else
/*+ SPR 9285*/
#define PHY_PARAM_RESP_MSG_SIZE  sizeof(uint8_t) + sizeof(FAPI_l1ErrorCodes_en) + 2/*padding*/ + (6 * sizeof(FAPI_paramResponseTLV_st))
/*- SPR 9285*/
#endif
/* SPR 973 End */
#define FAPI_MIMO_TB_COUNT 2
#define FAPI_SUBFRAME_INDICATION_MSG_BODY_SIZE sizeof(FAPI_subFrameIndication_st)
#define FAPI_RUNNING_STATE_CELL_CONFIG_RESP_MSG_BODY_SIZE (sizeof(FAPI_phyCellConfigResp_st) - sizeof(FAPI_cellConfig_st))


/* FAPI2_1 chages start*/
#define RELEASE8_MASK                          0x0001
#define RELEASE9_MASK                          0x0002
#define RELEASE10_MASK                         0x0004
//#ifdef IPR_NBIOT
/* SPR 21660_2 changes start */
#define RELEASE13_MASK                         0x0020
/* SPR 21660_2 changes end */
//#endif
 /* FAPI2_1 chages end*/

typedef struct phyUeContext_stT
{
    uint32_t   handle;
    uint16_t   rnti;
    uint16_t   cqiPucchResourceIndex;
    uint16_t   cqiPmiConfigIndex;
    uint16_t   cqiRiConfigIndex;
    uint16_t   simultaneousAckNackAndCqi;
    uint8_t    ackNacknRepetitionFactor;
    uint16_t   anN1PUCCHANRep;
    uint8_t    tddAckNackFeedbackMode;
    uint8_t    srSBandWidth; 
    uint8_t    srsHoppingBandwidth;
    uint8_t    freqDomainPosition;
    uint8_t    srsDuration;
    uint16_t   srsConfigIndex;
    uint8_t    txComb;
    uint8_t    soundingRefCyclicShift;
    uint16_t   srPucchResourceIndex;
    uint8_t    srConfigIndex;
    uint16_t   spsDlConfigSchedInterval;
    uint16_t   spsDln1PUCCHANPersistent;

}phyUeContext_st;


void sendMsgtoL2Config (UInt8 *receivedBuffer_p,
                                UInt32 msgLen, UInt8 cellIndex);

extern SInt32 txEnodebRRCSocketFD_g[MAX_NUM_CELLS];
extern SockAddr txEnodebRRC_addr_g[MAX_NUM_CELLS];
extern void setActiveCell(UInt8 cellIndex);

#ifdef HO_SETUP_CHANGES
/* +CA-SJ */
SInt32 txTargetEnbControlsocketFD_g[MAX_NUM_CELLS];
SockAddr txtargetEnodebRRC_addr_g[MAX_NUM_CELLS];
/* -CA-SJ */
#endif

void fapiSendErrorIndication( FAPI_l1ErrorCodes_en errCode,
                               FAPI_msgIdType_t msgId,
                               UInt16 recvSfnSf,UInt16 expectedSfnSf,
                               UInt8 **,UInt32 *, UInt8 cellIndex);
void fapiHandlePhyStartRequest ( FAPI_l1ApiMsg_st *recvMsg_p, UInt8 cellIndex );
void fapiHandlePhyStopRequest ( FAPI_l1ApiMsg_st *recvMsg_p, UInt8 cellIndex );
void fapiSendSubFrameStartIndication (UInt8 cellIndex);
/*  Changes Start */
#if (defined(LTE_EMBMS_SUPPORTED) && !defined(LAYER2_DLTG_THREAD))
typedef struct TG_sfn_cellIndex_stT
{
    /* A 16-bit value SFN, range 0 . 1023 */
    UInt16   sfn;
    UInt16   cellIndex;
    UInt32   currentTti;
}TG_sfn_cellIndex_st;
#define TG_SFN_IND_CEll_INDEX_MSG_BODY_SIZE sizeof(TG_sfn_cellIndex_st)
    extern UInt16          tempSfn;
    UInt8                  sendBuff[TG_SFN_IND_CEll_INDEX_MSG_BODY_SIZE];
#endif
#define SEND_BUFF_TO_TG_SET_U16BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val) >> 8);                           \
        *(p_buff + 1 ) = (UInt8)(val);                                  \
    } while (0)
#define SEND_BUFF_TO_TG_SET_U32BIT(p_buff, val)                 \
    do {                                                \
        *(p_buff) = (UInt8)(((val) >> 24) & 0xFF) ;               \
        *(p_buff+1) = (UInt8)(((val) >> 16)& 0xFF) ;               \
        *(p_buff+2) = (UInt8)(((val) >> 8)& 0xFF) ;               \
        *(p_buff+3) = (UInt8)(val);                 \
    } while (0)
/* Sweta Changes End */

#endif /* end of FAPI_PHY_API_HANDLER_H */
