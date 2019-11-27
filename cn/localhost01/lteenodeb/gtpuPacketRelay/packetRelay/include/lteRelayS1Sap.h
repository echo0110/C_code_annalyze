/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRelayS1Sap.h,v 1.2 2010/02/18 07:53:34 gur19479 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains variable declarations for Packet
 *                   : Relay and S1-AP Interface. 
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRelayS1Sap.h,v $
 * Revision 1.2  2010/02/18 07:53:34  gur19479
 * updated for cause
 *
 * Revision 1.1.2.3  2010/01/20 22:37:14  gur19479
 * updated for Peer GTP packet handing
 *
 * Revision 1.1.2.2  2009/12/22 09:12:43  gur21897
 * checkin after update
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 ****************************************************************************/

#ifndef LTE_RELAY_S1_SAP_H
#define LTE_RELAY_S1_SAP_H


/****************************************************************************
 *  TAG Values Used between S1AP Interface and Packet Relay
 ****************************************************************************/

/* IE Type definitions */
#define RELAY_GTPU_IE_CAUSE                           1
#define RELAY_GTPU_IE_TRANSACTION_ID                  2
#define RELAY_GTPU_IE_SAP_FLAG                        3
#define RELAY_GTPU_IE_SEQ_NUM                         4
#define RELAY_GTPU_IE_NPDU_NUM                        5
#define RELAY_GTPU_IE_PDCP_NUM                        6
#define RELAY_GTPU_IE_PORT_NUM                        7
#define RELAY_GTPU_IE_REORDERING_REQD                 8
#define RELAY_GTPU_IE_REC_COUNT                       9
#define RELAY_GTPU_IE_SEQ_DISABLE                     10
#define RELAY_GTPU_IE_RECOVERY                        14
#define RELAY_GTPU_IE_TEID_DATA_I                     16
/* INTRA_ENB_HO Fix Start */
#define RELAY_GTPU_IE_PDCP_EXT_FLAG                   17
/* INTRA_ENB_HO Fix End */
#define RELAY_GTPU_IE_NEXT_IDX                        32
#define RELAY_GTPU_IE_GSN_ADDR                        133
#define RELAY_GTPU_IE_GSN_ADDR_WITH_FLAG              134         /* SPR 13765 fix */
#define RELAY_GTPU_IE_QOS_PROFILE                     135
#define RELAY_GTPU_IE_EXT_HDR_TYPE_LIST               141
/* SPR 10787 fix start */
#define RELAY_GTPU_IE_CELL_DATA_I                     142
/* SPR 10787 fix end */


#if  defined (ENDC_ENABLED)
#define MAX_NUM_TUNNEL_PER_LC 4
#else
#define MAX_NUM_TUNNEL_PER_LC 3
#endif
#define MAX_NUM_UL_DL_TUNNELS_PER_LC 2

typedef enum tunnelId_enT
{
    NORMAL_BEARER_TUNNEL_ID = 0x0F,
    DL_FW_TUNNEL_ID_SRC_ENB = 0x2E,
    DL_FW_TUNNEL_ID_TRG_ENB = 0x2D,
    UL_FW_TUNNEL_ID_SRC_ENB = 0x6E,
    UL_FW_TUNNEL_ID_TRG_ENB = 0x6D
}tunnelId_en;

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRelayGtpuSap.h"
#include "lteTypes.h"
#include "lteRelay.h"
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

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/* structure used for Configure Sap Request between S1 and Relay Interface */

typedef struct relaySetupSapReqTag
{
    UInt8 setupSapReqTag; //varun
    UInt16 setupSapReqTagLen; //varun

    UInt8 lcId;                    /* To identify logical channel(Mandatory)*/
    /* SPR 10787 fix start */
	InternalCellIndex cellIndex;
    /* SPR 10787 fix end */
    UInt32 qosId;                  /* To identify qos identifier (Mandatory)*/
    relayGtpuTid teidSelf;         /* To identify self tunnel id
                                     (Conditional will present after first time)*/
    //tvHeader sapFlag;              /* To indicate Sap Flag (Mandatory)*/
    tunnelInfoHeader sapFlag;
    //tlvHeader qosProfile;          /* To identify Qos Profile (Mandatory)*/
    qosProfileHeader qosProfile;
    tvSNHeader seqNumUplink;       /* To identify UL Seq Number
                                      (Optional)*/
    tvSNHeader seqNumDnlink;       /* To identify DL Seq Number
                                      (Conditional, Present if seqNumUplink is present)*/
    relayGtpuTid teidPeer;         /* To identify peer tunnel id
                                      (Optional Length is fixed that's two bytes*/
    tlvHeader transportAddr;       /* To identify transport Address
                                      (Conditional, Present if teidPeer is present)*/
    //tvHeader seqFlag;              /* To indicate Seq Flag(Optional Length is fixed that's two bytes)*/
    seqHeader seqFlag;
    //tvHeader reorderingReqd;       /* To identify reordering reqd flag(Optional Length is fixed that's two bytes*/
    reorderReqHeader reorderingReqd;
    /* INTRA_ENB_HO Fix Start */
    pdcpextFlagHeader pdcpextFlag;   /* To enable or disable the SN in Intra eNB HO */
    /* INTRA_ENB_HO Fix End */
}relaySetupSapReq;

typedef struct relayRelSapTag
{
    relayGtpuTid teidSelf;           /* To indicate self tunnel identifier (Mandatory) */
}relayRelSap;


typedef struct relayReleaseSapReqTag
{
    UInt8           releaseSapReqTag; 
    UInt16          releaseSapReqTagLen; 
    UInt8           lcId;                  /* To identify logical channel(Mandatory)*/
    relayRelSap     relSapReq;
    tlvHeader       transportAddr;         /* To identify transport Address (Optional present only if peer teid present)*/
}relayReleaseSapReq;

typedef struct relayS1SetupSapReqTag
{
    UInt16 ueIndex;                /* To identify UE uniquley (Mandatory)*/
    UInt8 lcCount;                 /* To indicate number of RABs in Req (Mandatory)*/
    relaySetupSapReq setupSap[MAX_RAB_ID];    /* To identify setup request array (Mandatory)*/ 
    relayReleaseSapReq releaseSap[MAX_RAB_ID];    /* To identify setup request array (Mandatory)*/ 
}relayGtpuSetupSapReq;


/* Structure used for Release Sap Request between S1 and Relay Interface */

typedef struct relayRelSapReqTag
{
    relayGtpuTid teidSelf;           /* To indicate self tunnel identifier (Mandatory) */
}relayRelSapReq;


typedef struct relayS1RelSapReqTag
{
    UInt8 teidCount;                /* To identify tunnel id count (Mandatory)*/
    relayRelSapReq relSapReq[MAX_RAB_ID];      /* To identify release sap request array (Mandatory)*/
}relayS1RelSapReq;

/* Structure used for Release Sap Conf between S1 and Relay Interface */

typedef struct relayReleaseSapCnfTag
{
    tvHeader cause;                   /* To indicate the cause (Mandatory)*/
    relayGtpuTrasId transactionId;    /* To identify transaction id */
    relayGtpuTid teidSelf;            /* To indicate self tunnel identifier
                                         (Conditional present if cause contains failure) */
    tvSNHeader seqNumUplink;          /* To indicate UL Sequence Number
                                         (Conditional present if cause contains success)*/
    tvSNHeader seqNumDnlink;          /* To indicate DL Sequence Number
                                         (Conditional present if cause contains success)*/
}relayRelSapCnf;

typedef struct relayS1RelSapCnfTag
{
    UInt8 teIdCount;                /* To maintain tunnel identifier counter (Mandatory) */
    relayRelSapCnf relSapCnf[MAX_RAB_ID];      /* To maintain rel sap confirmation array (Mandatory)*/
}relayGtpuRelSapCnf;


/****************************************************************************
 * Function Name  : processGtpuSetupSapReq
 * Inputs         : setupSapReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process SAP setup request.
 ****************************************************************************/

UInt32 processGtpuSetupSapReq(UInt8 *sapReq_p,UInt16 msgLen);

/****************************************************************************
 * Function Name  : processGtpuDeleteUeReq
 * Inputs         : releaseSapReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process SAP Release Request.
 ****************************************************************************/

UInt32 processGtpuDeleteUeReq(UInt8 *releaseSapReq_p,UInt16 msgLen);

/****************************************************************************
 * Function Name  : processGtpuCpErrorInd
 * Inputs         : cpErrorInd_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process error indication.
 ****************************************************************************/

UInt32 processGtpuCpErrorInd(UInt8 *cpErrorInd_p,UInt16 msgLen);

/****************************************************************************
 * Function Name  : processGtpuCpPathFailureInd
 * Inputs         : cpPathFailureInd_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process CP Path Failure Indication.
 ****************************************************************************/

UInt32 processGtpuCpPathFailureInd(UInt8 *cpPathFailureInd_p,UInt16 msgLen);

/*+SPR #3456*/
/****************************************************************************
 * Function Name  : processGtpuCpPathSuccessInd
 * Inputs         : cpPathSuccessInd_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process CP Path Success Indication.
 ****************************************************************************/

UInt32 processGtpuCpPathSuccessInd(UInt8 *cpPathSuccessInd_p,UInt16 msgLen);
/*-SPR #3456*/

/****************************************************************************
 * Function Name  : processGtpuCpEndMarkerInd
 * Inputs         : cpEndMarkerInd_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process End Marker Indication.
 ****************************************************************************/
UInt32 processGtpuCpEndMarkerInd(UInt8 *cpEndMarkerInd_p,UInt16 msgLen);

/****************************************************************************
 * Function Name  : processGtpuSetupSapCnf
 * Inputs         : setupSapCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process SAP setup confirmation.
 ****************************************************************************/

UInt32 processGtpuSetupSapCnf(UInt8 *setupSapCnf_p,UInt16 msgLen);

/****************************************************************************
 * Function Name  : processGtpuReleaseSapCnf
 * Inputs         : releaseSapCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process SAP Release Confirmation.
 ****************************************************************************/

UInt32 processGtpuReleaseSapCnf(UInt8 *releaseSapCnf_p,UInt16 msgLen);

/****************************************************************************
 * Function Name  : processPeerGtpuDataInd
 * Inputs         : data_p and totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process Peer GTP Data Indication.
 ****************************************************************************/

UInt32 processPeerGtpuDataInd(UInt8 *data_p, UInt32 trans_id, UInt32 totalLen);

 /****************************************************************************
  ** Function Name  : processCreateUeRequest
  ** Inputs         : setupSapReq_p
  ** Outputs        : None
  ** Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
  ** Description    : This funtion process SAP UE Create setup request.
  *****************************************************************************/
UInt32 processCreateUeRequest(UInt8 *sapReq_p,UInt16 msgLen);

/* Intra-eNB HO Changes Start*/
UInt32 processS1uIntraEnbDataFwdReq(UInt8 *intraDataFwdReq_p, UInt16 msgLen);
UInt32 processS1uLocalPathSwitchReq(UInt8 *localPathSwitchReq_p, UInt16 msgLen);

/* Intra-eNB HO Changes End */
/****************************************************************************
 * * Function Name  : updateRelayMapTableBasedOnTransId
 * * Inputs         : sapFlagValue
 * * Outputs        : None
 * * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * * Description    : This funtion populates tunnelType on the basis of sapFlagValue
 * *                  as input.
 * ****************************************************************************/

/* +- SPR 17777 */
UInt8 updateRelayMapTableBasedOnTransId(
         UInt16 ueIndex,UInt8 lcId,UInt32 transIdForRsp, UInt16 apiIdForRsp,
         UInt32 *pTransId,
         UInt8 sapFlagValue,UInt8 *pIsTunnelIdFlagTrue,
         UInt32 *pPrevTidIndex,
         UInt8 modifyFlag);
/* +- SPR 17777 */

/****************************************************************************
 * * Function Name  : updateRelayUeCntxMngmntBasedOnTunnelId
 * * Inputs         : ueIndex, transId
 * * Outputs        : None
 * * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * * Description    : This funtion populates tunnelInfo on the basis of tunnelId
 * ****************************************************************************/

UInt8 updateRelayUeCntxMngmntBasedOnTunnelId(relayRelSapReq *pRelSapReq,UInt8 lcIndex,
                    UInt32 transIdForRsp,UInt16 apiIdForRsp,UInt16 ueIndex,
                     UInt8 teIdCount,UInt8 startVal);


/****************************************************************************
 * Function Name  : updateUeCntxMngmntBasedOnTunnelIdForReconfigSapRel
 * Inputs         : ueIndex, transId, tunnelId
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion populates tunnelInfo on the basis of tunnelId
 ****************************************************************************/
/* +- SPR 17777 */
UInt8 updateUeCntxMngmntBasedOnTunnelIdForReconfigSapRel(
        relayRelSap *pRelSapReq,UInt32 transId,UInt16 apiIdForRsp,
        UInt16 ueIndex);
/* +- SPR 17777 */

/* LIPA changes start */
/****************************************************************************
 * Function Name  : updateRelayMapTableOnTransIdForSecondaryTunnel
 * Inputs         : ueIndex, lcId, transIdForRsp, apiIdForRsp, transId, 
                  : sapFlagValue, tunnelIDSelf
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion populates Secondary tunnelInfo on the basis 
 *                  : of tunnelId
 * ****************************************************************************/
 UInt8 updateRelayMapTableOnTransIdForSecondaryTunnel(
                                    UInt16 ueIndex,
                                    UInt8 lcId,
                                    UInt32 transIdForRsp,
                                    UInt16 apiIdForRsp,
                                    UInt32 transId,
                                    UInt8  sapFlagValue,
                                    UInt32 *tunnelIDSelf);

/****************************************************************************
 * Function Name  : handleGtpuSecondaryTunnelId
 * Inputs         : p_global, p_addr, tunnelId
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion should get the rab context basis of Secondary
 *                  : tunnelId
 * ****************************************************************************/
UInt8 handleGtpuSecondaryTunnelId(egtpu_global_t *p_global,
                                  UInt8 *p_addr,
                                  UInt32 tunnelId);

/****************************************************************************
 * Function Name  : udpSendToSecondaryTG
 * Inputs         : respMsg, current, interfaceType
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion should send to Secondary tunnelId Information
 *                  : to Secondary DL TG
 *                  : tunnelId
 * ****************************************************************************/
UInt8 udpSendToSecondaryTG(UInt8 *respMsg,
                           UInt16 current,
                           Char8 interfaceType);
/* LIPA changes end */


#endif
