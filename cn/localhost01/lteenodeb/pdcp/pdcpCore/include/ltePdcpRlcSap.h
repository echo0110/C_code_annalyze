/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpRlcSap.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:10 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Interface definition for interaction beetwen the PDCP
 *                     layer and RLC layer (RLC SAP).
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpRlcSap.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:10  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:11:44  gur19836
 * RLC Rel 2.0 Code Merged
 *
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

#ifndef _LIB_RLC_SAP_DEF_
#define _LIB_RLC_SAP_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include "lteLayer2RlcPdcpInterface.h" 

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* Possible values for transactionIdPresent parameter in rlcUeDataReq function */
#define PDCP_RLC_TRANSACTION_ID_VALID       LTE_TRUE
#define PDCP_RLC_TRANSACTION_ID_INVALID     LTE_FALSE

/* Possible values for serviceRequested parameter in rlcUeDataReq function */
#define PDCP_RLC_REQ_ACK_TRUE       LTE_TRUE
#define PDCP_RLC_REQ_ACK_FALSE      LTE_FALSE

/* Possible values for deliveryStatus parameter in rlcDeliveryStatusInd function */
#define PDCP_RLC_ACK    LTE_TRUE
#define PDCP_RLC_NACK   LTE_FALSE

#define PDCP_RLC_Q_FULL   2

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/



/****************************************************************************
 * Function Name  : rlcUeDataReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  data_p  - pointer to PDCP PDU data in Zero-copy buffering format.
                    size - size of data (PDU)
                    transactionIdPresent - if true then transactionId parameter should be used
                    transactionId  - transaction ID.
                    serviceRequested - if true then Delivery Indication is needed.
 *                  pktArrTime - Arrival time
 *                  ipPktType - IPv4 or IpV6
 *                  pdcpSduSize - Size of PDCP SDU received from GTPU.
 * Outputs        : None.
 * Returns        : None.
 * Description    : It used for passing PDCP PDU to the RLC layer (lower layer).
                    The RLC layer has to implement this function.
 ****************************************************************************/
UInt32 rlcUeDataReq (
                UInt16  ueIndex,
                UInt16  lcId,
                UInt8*  data_p,
                UInt16  size,
                UInt8   transactionIdPresent,
                /* SPR 2812 changes start */
                void * transactionId,
                /* SPR 2812 changes end */
                UInt16  serviceRequested,
                tickType_t pktArrTime,
                UInt8   ipPktType,
                UInt8   voiceSilenceType,
                UInt32  avgPduSize
                );

/****************************************************************************
 * Function Name  : rlcDataDiscardReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
                    transactionId  - transaction ID.
 * Outputs        : None.
 * Returns        : None.
 * Description    : It used for discarding PDCP PDU on the RLC layer(lower layer).
                    The RLC layer has to implement this function.
 ****************************************************************************/
UInt32 rlcDataDiscardReq (
                UInt16  ueIndex,
                UInt16  lcId,
                void *transactionId );

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : rlcUeDataInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  data_p  - pointer to PDCP PDU data in Zero-copy buffering format..
                    size - size of data
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function receives data indication (PDCP PDU) from the RLC layer and 
                    stores its into the PDCP RX queue.
                    It's asynchronous function.
 ****************************************************************************/
UInt32 rlcUeDataInd (
        UInt16 ueIndex,
        UInt16 lcId,
        UInt8* data_p,
        UInt16 size,
        UInt8 packetForReEstablishment,
        tickType_t headPktTS,
        UInt8 macState
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        ,tickType_t packetTime
#endif
        );

/****************************************************************************
 * Function Name  : rlcDeliveryStatusInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  count  - will define the total delivery status in array. 
                    deliveryStatus - Array for delivery status.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function receives data delivery indication (PDCP PDU) from the RLC layer and 
                    stores its into the PDCP delivery indication TX queue.
                    It's asynchronous function.
 ****************************************************************************/
UInt32 rlcDeliveryStatusInd (
        UInt16  ueIndex,
        UInt16  lcId,
        UInt32  count,
        RlcStatusBuffer *deliveryStatus);


#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
UInt32 rlcHandleUeDataInd(UInt32 cQCount);
UInt32  rlcHandleDeliveryStatusInd(UInt32 cQCount);
#endif

#ifndef UE_SIM_TESTING
void pdcpFirstSubFrameInd(UInt32 sysFrameNum, UInt32 subFrameNum);
void pdcpSubFrameInd(void);
#endif

#ifdef FAPI_L2_MASTER
#ifndef UE_SIM_TESTING
extern void rlcFirstSubFrameIndFrmL2Tick(tickType_t currentTickFrmL2Tick);
extern void pdcpFirstSubFrameIndFrmL2Tick(tickType_t currentTickFrmL2Tick);
#endif
#endif



#endif /* _LIB_RLC_SAP_DEF_ */
