/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpPdcpUserSap.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Interface definition for interaction beetwen the PDCP
 *                     layer and PDCP user (PDCP SAP).
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpPdcpUserSap.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:18  gur20548
 * Changed header file name
 *
 * Revision 1.3  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _LIB_PDCP_SAP_DEF_
#define _LIB_PDCP_SAP_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include "ltePdcpContext.h"
/*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
/*SPR 21188 -*/
#include "ltePdcpTlvUtils.h"
#endif

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* Possible values for deliveryStatus parameter in pdcpDataStatusInd function */
#define PDCP_DELIVERY_STATUS_SUCCESS    LTE_TRUE
#define PDCP_DELIVERY_STATUS_FAILURE    LTE_FALSE

/****************************************************************************
 * Private Types
 ****************************************************************************/
typedef struct dataForwardingInfoT
{
    UInt8   lcId;                /* Logical Channel Identifier*/
    UInt8   ulDataFwdFlag;       /* Uplink Data Forwarding Flag */
}dataForwardingInfo, *LP_dataForwardingInfo;

typedef struct pdcpDataForwardReqT
{
    UInt16  ueIndex;    /* UE Identifier */
    UInt8 count;
    UInt8 PaddingBit;   /* Padding bit reqd to make structure byte alligned*/
    dataForwardingInfo dataFwdInfo[8];
}PdcpDataForwardReq, *LP_PdcpDataForwardReq;

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define MAX_NODES_FWDED_AT_TIME 10

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
/*SPR 21188 -*/
extern TlvCommandTable prCmdTable_g[];
#endif

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpDataInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  data_p  - pointer to PDCP SDU data in Zero-copy buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : The PDCP layer application has to implement this function.
                    It's used for passing SDU form the PDCP layer to the PDCP user (upper layers).
 ****************************************************************************/
extern void pdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, UInt16 size);

/****************************************************************************
 * Function Name  : pdcpDataStatusInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  deliveryStatus - SDU delivery status
 * Outputs        : None.
 * Returns        : None.
 * Description    : The PDCP layer application has to implement this function.
                    It used for sending SDU delivery indictaion to the 
                    PDCP user (upper layers) in case of Discard_timer expires for SDU or 
                    PDU delivery is failed (for RLC AM).
 ****************************************************************************/
extern void pdcpDataStatusInd( UInt16 ueIndex, UInt16 lcId, UInt16 deliveryStatus );

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/****************************************************************************
 ** Function Name  : pdcpCreateDataReq
 ** Inputs         : ueIndex - UE Id.
 **                  lcId    - Logical channel Id.
 **                  data_p  - pointer to PDCP SDU data in Zero-copy
 **                            buffering format.
 **                  size    - size of data.
 ** Outputs        : None.
 ** Returns        : LP_PdcpDataReq
 ** Description    : This function allocates PDCP Data Req and sets
 *                    values
 *****************************************************************************/
LP_PdcpDataReq pdcpCreateDataReq( UInt16 ueIndex, UInt16 lcId,
        LP_PdcpEnityContext ctx_p, void* data_p, UInt16 size );

/****************************************************************************
 * Function Name  : pdcpDataReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  transactionId - Transaction Id
 *                  data_p  - pointer to PDCP SDU data in Zero-copy buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function passes SDU to the PDCP layer.
                    It's asynchronous function. It stores SDU into PDCP TX queue and returns.
 ****************************************************************************/
void pdcpDataReq( UInt16 ueIndex, UInt16 lcId, UInt32  transactionId, void* data_p, UInt16 size,UInt8 instanceId); 

#ifdef L2_FAST_PATH
/****************************************************************************
 * Function Name  : pdcpDataFPReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  ctx_p - PDCP entity context
 *                  transactionId - Transaction Id
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function passes SDU to the PDCP layer.
 *                  It directly submits the data (after PDCP processing) to RLC 
 *                  through the fast path and stores a copy of it in the 
 *                  PDCP Storage Q as well for delivery status indication
 ****************************************************************************/
void pdcpDataFpReq( UInt16 ueIndex, UInt16 lcId,
        LP_PdcpEnityContext ctx_p, UInt32  transactionId,
        void* data_p, UInt16 size );
#endif

/****************************************************************************
 * Function Name  : pdcpTargetDlDataRelocReq
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  sn      - PDCP PDU Sequence Number
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function passes Forwarded DL Data SDU to the PDCP layer.
 *                  It's asynchronous function. It stores SDU into PDCP Data 
 *                  Fwding queue and returns. Applicable only at target
 *                  PDCP.
 ****************************************************************************/
void pdcpTargetDlDataRelocReq(UInt16 ueIndex, UInt8 lcId, UInt16 sn, 
        void *data_p, UInt16 size,UInt8 instanceId);

/****************************************************************************
 * Function Name  : pdcpTargetUlDataRelocReq
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  sn      - PDCP PDU Sequence Number
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function passes Forwarded UL Data SDU to the PDCP layer.
 *                  It's asynchronous function. It stores SDU into Data Fwding 
 *                  queue and returns. Applicable only at target
 *                  PDCP.
 ****************************************************************************/
void pdcpTargetUlDataRelocReq( UInt16 ueIndex, UInt8 lcId, UInt16 sn, 
        void *data_p, UInt16 size ,UInt8 instanceId);

/****************************************************************************
 * Function Name  : pdcpSourceDataFwdReq
 * Inputs         : ueIndex - UE Id.
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function passes PDCP_DATA_FWD_REQ to the PDCP layer.
 *                  It's asynchronous function. It will forward all PDCP SDU 
 *                  with there associated SN Number and returns.
 ****************************************************************************/
void pdcpSourceDataFwdReq(void* data_p, UInt16 size, UInt8 instanceId);

/* SPR 10894 fix start*/
/****************************************************************************
 * Function Name  : pdcpSourceDataFwdReqInIntraeNB
 * Inputs         : oldUeIndex    - old UE Index
 *                  newUeIndex - new UE Index
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function receives PDCP_DATA_FWD_REQ from Packet Relay.
 *                  It's asynchronous function. It will copy all PDCP SDU 
 *                  with there associated SN Number to new context.
 ****************************************************************************/
void pdcpSourceDataFwdReqInIntraeNB( UInt16 oldUeIndex, UInt16 newUeIndex,UInt8 instanceId);
/* SPR 10894 fix end*/

/****************************************************************************
 * Function Name  : pdcpEndMarkerInd
 * Inputs         : ueIndex, lcId.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function will receive EMI from packet relay to 
 *                  indicate last packet received at source PDCP.
 ****************************************************************************/
void pdcpEndMarkerInd( UInt16 ueIndex, UInt8 lcId,UInt8 instanceId );

/****************************************************************************
 * Function Name  : pdcpFwdDlFreshSdu
 * Inputs         : ueIndex - UE Id, 
 *                  lcId - logical Channel Id, 
 *                  state, event, pointer to PdcpEntityContext Structure. 
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function will forward all the downlink packets (only
 *                  fresh SDUs).
 ****************************************************************************/
UInt32 pdcpFwdDlFreshSdu( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
        UInt32 event, void *context_p );

#ifndef UE_SIM_TESTING
void rlcSubFrameInd(InternalCellIndex internalCellIndex);
void rlcFirstSubFrameInd( UInt32 sysFrameNum, UInt32 subFrameNum,
        InternalCellIndex internalCellIndex);
#else
/* SPR 1535 Changes Start*/
/* + SPR 17439 */
void rlcSubFrameInd(void);
/* - SPR 17439 */
void rlcFirstSubFrameInd( UInt32 sysFrameNum,
UInt32 subFrameNum);
/* SPR 1535 Changes End*/
#endif

/* SPR 3085 Changes Start */
/****************************************************************************
 * Function Name  : pdcpEndOfDataForwardingInd
 * Inputs         : ueIndex - UE Id, 
 *                  lcId - logical Channel Id, 
 *                  isEndMarkerIndRecvd - If EMI received by PDCP or not. 
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function will forward local EMI when it finishes
 *                  data forwarding or is deleting its context. It will be
 *                  called only once for 1 LC context.
 ****************************************************************************/
extern void pdcpEndOfDataForwardingInd( UInt16 ueIndex, UInt8 lcId, 
        UInt8 isEndMarkerIndRecvd );
/* SPR 3085 Changes End */
/*SPR 8544 FIX START*/
#ifndef RLC_PDCP_IPC_INTF
UInt8 rlcSduQFullInd( UInt16 ueIndex, UInt8 lcId);
#endif
/*SPR 8544 FIX END*/

/*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
/*SPR 21188 -*/
/****************************************************************************
 * Function Name  : pdcpPrProcessMessage
 * Inputs         : data_p  - pointer to TLV API buffer.
 *                   size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer application for
 *                  delivering Packet Relay APIs to the PDCP layer. The APIs
 *                  message will be processed immediately.
 *****************************************************************************/
void pdcpPrProcessMessage( UInt8 * data_p, UInt16 size );

/****************************************************************************
 * Function Name  : prCmdDefaultHandler
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes unknown Packet Relay API.
 ****************************************************************************/
void prCmdDefaultHandler( UInt8* data_p, UInt16 size );
#endif

#if defined PDCP_GTPU_INTF
/****************************************************************************
 * Function Name  : sendGtpuRegisterPdcpInd
 * Inputs         : cellIdList_p,cellCount
 * Outputs        : None
 * Returns        : None
 * Description    : This function fill the gtpu register ind msg and send to 
 *                  GTPU module
 ****************************************************************************/
void sendGtpuRegisterPdcpInd(UInt8 *cellIdList_p, UInt8 cellCount);
#endif


#endif /* _LIB_PDCP_SAP_DEF_ */
