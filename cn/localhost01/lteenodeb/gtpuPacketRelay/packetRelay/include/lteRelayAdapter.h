/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRelayAdapter.h,v 1.1.1.1 2010/02/11 04:51:36 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains variable declarations for Packet
 *                   : Relay and PDCP Adapter Interface.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRelayAdapter.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:36  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 ****************************************************************************/

#ifndef LTE_RELAY_ADAPTER_H
#define LTE_RELAY_ADAPTER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "port.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteMsgPool.h"
#include "lteTypes.h"
/*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
/*SPR 21188 -*/
#include "ltePdcpContext.h"
#else
#include "lteSQueue.h"
#endif
#include "lteLog.h"
#ifndef UE_SIM_TESTING
#include "logging.h"
#include "alarm.h"
#endif

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
 #define BUFFERSIZE 2000
 #define API_HEADER_LEN 5
 //#define MAX_UE 99
 #define MIN_DRB_LC 3
 #define MAX_DRB_LC 10
 #define MAX_LC 10
 #define MIN_UE 0
 #define MIN_LC 1

/****************************************************************************
 * Exported Types
 ****************************************************************************/
 /*SPR 2030 fix Start*/
 /* SPR 15909 fix start */
 extern tickType_t relayGtpuCurrentTick_g;
 /* SPR 15909 fix end */
 /*SPR 2030 fix End*/
 /* SPR #2017 fix Start */
 #define GTP_MIN_HDR_LEN 8
 #define GTP_MAX_HDR_LEN 12
/* + SPR 20276 */
#define GTP_HDR_ALL_OPT_FIELD_SET 0x05
#define GTP_HDR_SEQ_NUM_BIT 0x02
/* - SPR 20276 */
#define ADAPTER_MAX_UE   MAX_UE_SUPPORTED

typedef struct
{
   UInt32         tId; /* Tunnel Id*/
   UInt16         ueId;   /* UE Index*/
   UInt8          lcId;   /* LC Id*/
   UInt8          flag;   /* Tunnel Validity*/
}adapterDataBase_t;
/* + SPR 17439 */
void initGtpuGlueSocket(void);
/* - SPR 17439 */
/* Start of change for VARIBLE Declaration */
/* End of change for VARIBLE Declaration */

typedef struct filestruct{
    UInt16 trafficGeneratorPort;
    Char8 trafficGeneratorIP[20];
    struct hostent *pTrafficGeneratorHost;
    Char8 adapterIP[20];
    /*IPV6_CHANGE - START*/
    UInt8 adapterIP6[20];
    UInt16 addrPresenceSt;
    /*IPV6_CHANGE - END*/
    UInt8  adapterMode;
    UInt8  adapterDelay;
    UInt16 adapterRxPort;
    //UInt16 adapterTxPort;
}ConfigFileFormat,*LP_ConfigFileFormat;

/* SPR 18458 Fix Start */
#define MAX_NUM_EMBMS_AREA_INDEX 16
#define DEQUEUE_AREA_NODE(prQueue)\
    deQueue(prQueue)
/* SPR 18458 Fix End */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

#define INVALID_PDCP_SN   0xFFFF 

/****************************************************************************
 * Function Name  : handleRelayAdapterDataReq
 * Inputs         : ueIndex : UE Index for Unique Identification of UE.
 *                : lcId : Logical Channel Identifier.
 *                : payloadLen : Length of payload.
 *                : payload_p : Actual Payload.
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion handle Data Req from Adapter.
 ****************************************************************************/

UInt32 handleRelayAdapterDataReq
(
    UInt16 ueIndex,
    UInt8 lcId,
    UInt16 payloadLen, 
    UInt8 *payload_p
);

/****************************************************************************
 * Function Name  : handleRelayAdapterDataInd
 * Inputs         : transId : Transaction Identifier.
 *                : payloadLen : Lenght of payload
 *                : payload_p : Actual Payload
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion handle Data Indication Req from GTP-U.
 ****************************************************************************/

UInt32 handleRelayAdapterDataInd
(
    UInt32 transId,
    UInt16 pdcpNo, 
    UInt16 payloadLen,
    UInt8 *payload_p
);

/****************************************************************************
 * Function Name  : processGtpuAdapterDataInd
 * Inputs         : gtpuDataReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process DATA Ind from GTP-U.
 ****************************************************************************/

UInt32 processGtpuAdapterDataInd
(
    UInt8 *gtpuDataInd_p,
    UInt16 msgLen
);

/****************************************************************************
 * Function Name  : processGtpuAdapterDataReq
 * Inputs         : gtpuDataReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process DATA Req from Adapter.
 ****************************************************************************/

UInt32 processGtpuAdapterDataReq 
(
    UInt8 *gtpuDataReq_p,
    UInt16 msgLen
);


/****************************************************************************
 * Function Name  : handleRelayAdapterDlDataRelocReq
 * Inputs         : ueIndex : UE Index for Unique Identification of UE.
 *                : lcId : Logical Channel Identifier.
 *                : payloadLen : Length of payload.
 *                : payload_p : Actual Payload.
 *                : pdcpSeqNo : PDCP Sequence No
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion handle Data Req from Adapter.
 ****************************************************************************/

UInt32 handleRelayAdapterDlDataRelocReq
(UInt16 ueIndex,UInt8 lcId,UInt16 payloadLen,UInt8 *gtpuMsg_p,UInt16 pdcpSeqNo
);

/****************************************************************************
 * Function Name  : processGtpuAdapterDlDataRelocReq
 * Inputs         : gtpuDataReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process DL DATA RELOC Req from Adapter.
 ****************************************************************************/

UInt32 processGtpuAdapterDlDataRelocReq(UInt8 *gtpuDataReq_p,UInt16 pdcpSeqNo);


/****************************************************************************
 * Function Name  : handleRelayAdapterUlDataRelocReq
 * Inputs         : ueIndex : UE Index for Unique Identification of UE.
 *                : lcId : Logical Channel Identifier.
 *                : payloadLen : Length of payload.
 *                : payload_p : Actual Payload.
 *                : pdcpSeqNo : PDCP Sequence No
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion handle UL RELOC Data Req from Adapter.
 ****************************************************************************/

UInt32 handleRelayAdapterUlDataRelocReq
(UInt16 ueIndex,UInt8 lcId,UInt16 payloadLen,UInt8 *gtpuMsg_p,UInt16 pdcpSeqNo
);

/****************************************************************************
 * Function Name  : processGtpuAdapterUlDataRelocReq
 * Inputs         : gtpuDataReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process UL DATA RELOC Req from Adapter.
 ****************************************************************************/

UInt32 processGtpuAdapterUlDataRelocReq(UInt8 *gtpuDataReq_p,UInt16 pdcpSeqNo);

/****************************************************************************
 * Function Name  : processAdapterEndOfDataFwdInd
 * Inputs         : ueIndex, lcId,isEndMarkerIndRecvd
 * Outputs        : None
 * Description    : This funtion receives EndOfDataFwdInd from PDCP Adapter .
 ****************************************************************************/
void processAdapterEndOfDataFwdInd 
(
    UInt16 ueIndex,
    UInt8  lcId,
/* SPR 3085 Fix Start */
    UInt8  isEndMarkerIndRecvd
/* SPR 3085 Fix End*/ 
);

UInt32 processGtpuAdapterEndMarkerInd 
(
    UInt8 *gtpuEndMarkerInd_p,
    UInt16 msgLen
);
extern void pdcpDataReq( UInt16 ueIndex, UInt16 lcId, UInt32  transactionId,
        void* data_p, 
/*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
/*SPR 21188 -*/
        void *msgBuffer_p,
#endif
        UInt16 size 
        ,UInt8 instanceId
        );
/* + SPR 17439 */
void initPdcpAdapter(void);
void cleanupPdcpAdapter(void);
/* - SPR 17439 */

void pdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, UInt16 size);
void pdcpDataStatusInd( UInt16 ueIndex, UInt16 lcId, UInt16 deliveryStatus );

extern UInt32 processGtpuAdapterDataReq(UInt8 *,UInt16);
/* SPR 21804 code deleted */


/****************************************************************************
 * * Function Name  : pdcpSourceDlDataRelocInd
 * * Description    : The PDCP layer calls this API to forward Downlink SDUs
 * *                  to upper layer at source eNodeB in case of handover
 *****************************************************************************/
UInt16 pdcpSourceDlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
    void *data_p, UInt32 size);

/*****************************************************************************
 * * Function Name  : pdcpSourceUlDataRelocInd
 * * Description    : The PDCP layer calls this API to forward Uplink SDUs
 * *                  to upper layer at source eNodeB in case of handover
 *****************************************************************************/
UInt16 pdcpSourceUlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
    void *data_p, UInt32 size
#ifdef KPI_STATS
    ,InternalCellIndex internalCellIndex
#endif    
    );

/****************************************************************************
 * * Function Name  : processGtpuAdapterDlDataRelocReq
 * * Description    : This funtion is implemented by packet relay.It is used to
 * *                  forward DL PDCP SDUs to packet relay in case of handover.
 *****************************************************************************/
extern UInt32 processGtpuAdapterDlDataRelocReq(
    UInt8 *gtpuDataReq_p,
    UInt16 pdcp_seq_no);

/****************************************************************************
 * * Function Name  : processGtpuAdapterUlDataRelocReq
 * * Description    : This funtion is implemented by packet relay.It is used to
 * *                  forward UL PDCP SDUs to packet relay in case of handover.
 *****************************************************************************/
extern UInt32 processGtpuAdapterUlDataRelocReq(
    UInt8 *gtpuDataReq_p,
    UInt16 pdcp_seq_no);
/* SPR 3085 Fix Start */
void pdcpEndOfDataForwardingInd(UInt16 ueIndex, UInt8 lcId,
                                           UInt8 isEndMarkerIndRecvd);
extern void processAdapterEndOfDataFwdInd(UInt16 ueIndex, UInt8 lcId,
                                          UInt8 isEndMarkerIndRecvd );
/* SPR 3085 Fix End */
/* + SPR 17439 */
void msgAndMemPoolUsage(void);
/* - SPR 17439 */

#endif
