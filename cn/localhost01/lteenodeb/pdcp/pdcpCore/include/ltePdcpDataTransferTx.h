/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpDataTransferTx.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Data transfer TX module definitions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpDataTransferTx.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.3  2009/07/20 08:21:52  gur19479
 * Changes done for SRB handling
 *
 * Revision 1.1.2.2  2009/07/13 10:24:27  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:17  gur20548
 * Changed header file name
 *
 * Revision 1.4  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _LIB_PDCP_DATA_TRANSFER_TX_DEF_
#define _LIB_PDCP_DATA_TRANSFER_TX_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "ltePdcpTxQueues.h"
#include "ltePdcpContext.h"

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

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpRetransmitTx
 * Inputs         : ueIndex, lcId, state, event, PDCP Context
 * Outputs        : None.
 * Returns        : None.
 * Description    : Performs Retransmission of SDUs for which ACK is not 
 *                  recvd from RLC.
 ****************************************************************************/
UInt32 pdcpRetransmitTx( UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p );

/****************************************************************************
 * Function Name  : processDeliveryStatusInd 
 * Inputs         : deliveryInd_p
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function handles delivery status information from RLC layer.
 ****************************************************************************/ 
void processDeliveryStatusInd(LP_PdcpDeliveryInd deliveryInd_p); 

/****************************************************************************
 * Function Name  : pdcpProcessDrbTx  
 * Inputs         : ctx_p - Pointer to UE and LC context, 
 *                  dataReq_p - Data to be processed
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : single function for DRB TX Leg
 ****************************************************************************/
UInt32 pdcpProcessDrbTx(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p);

/****************************************************************************
 * Function Name  : pdcpProcessSrbTx
 * Inputs         : ctx_p - Pointer to UE and LC context, 
 *                  dataReq_p - Data to be processed
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : single function for SRB TX Leg
 ****************************************************************************/
UInt32 pdcpProcessSrbTx(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p);

/****************************************************************************
 * Function Name  : pdcpProcessDataFwdTx
 * Inputs         : ctx_p - Pointer to UE and LC context, 
 *                  dataReq_p - Data to be processed
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : single function for DRB TX Leg for fwded packets
 ****************************************************************************/
UInt32 pdcpProcessDataFwdTx(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p);

/****************************************************************************
 * Function Name  : pdcpProcessControlPduIndTx  
 * Inputs         : controlPduInd_p - pointer on PDCP Control PDU indication structure.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Processes signle PDCP Control PDU indication 
 ****************************************************************************/
void pdcpProcessControlPduIndTx(LP_PdcpControlPduInd controlPduInd_p);

/****************************************************************************
 * Function Name  : pdcpAssociateSnHfn 
 * Inputs         : ctx_p - Pointer to UE and LC context, 
 *                  dataReq_p - Data to be processed
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : Associates Count = HFN + SN to a packet
 ****************************************************************************/
void pdcpAssociateSnHfn(LP_PdcpEnityContext ctx_p, 
        LP_PdcpCommDataReq dataReq_p);

/****************************************************************************
 * Function Name  : pdcpFinishPduTx
 * Inputs         : LP_PdcpDataReq dataReq_p,LP_PdcpEnityContext ctx_p,
 *                  void *data_p
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : Stores and transmit PDU to RLC, depending on AM/UM mode.
 ****************************************************************************/
UInt32 pdcpDrbFinishPduTx(LP_PdcpDataReq dataReq_p, 
        LP_PdcpEnityContext ctx_p,
        void *data_p);

/****************************************************************************
 * Function Name  : pdcpSrbFinishPduTx
 * Inputs         : LP_PdcpSrbDataReq dataReq_p,LP_PdcpEnityContext ctx_p,
 *                  void *data_p
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : Stores and transmit PDU to RLC, depending on AM/UM mode.
 ****************************************************************************/
UInt32 pdcpSrbFinishPduTx(LP_PdcpSrbDataReq dataReq_p, 
        LP_PdcpEnityContext ctx_p,
        void *data_p);

#ifdef PDCP_ASYNC_INTERFACE
/****************************************************************************
 * Function Name  : pdcpDrbPktForSec
 * Inputs         : entity context, Packet Context, data
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE
 * Description    : Appends PDCP header to DRB packets and send the packet to
 *                  security engine
 ****************************************************************************/
UInt32 pdcpDrbPktForSec( LP_PdcpEnityContext ctx_p, 
        LP_PdcpDataReq dataReq_p, void *data_p );

/****************************************************************************
 * Function Name  : pdcpSrbPktForSec
 * Inputs         : entity context, Packet Context, data
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE
 * Description    : Appends PDCP header to SRB packets and send the packet to
 *                  security engine
 ****************************************************************************/
UInt32 pdcpSrbPktForSec( LP_PdcpEnityContext ctx_p, 
        LP_PdcpSrbDataReq dataReq_p, void *data_p );
#endif

#ifdef L2_FAST_PATH
UInt32 pdcpProcessSduTx(LP_PdcpEnityContext ctx_p,
        LP_PdcpDataReq dataReq_p, void *data_p);
#endif

/****************************************************************************
 * Function Name  : sendrlcUeDataReqToRlc
 * Inputs         : ueIndex - UE Identifier of a UE
 *                     lcId - Logical Channel ID
 *                     data_p - Pointer to the data received from PDCP
 *                     size - Size of data received from PDCP
 *                     transId - Transaction ID
 *                     serviceRequested - Service Requested from PDCP
 * Outputs        :
 * Returns        : Success or Failure
 * Variables      :
 * Description    : This function shall send RlcUeDataReq to RLC.
 *******************************************************************************/
UInt32 sendrlcUeDataReqToRlc ( UInt16 ueIndex,
                      UInt16  lcId,
                      UInt8 * data_p,
                      UInt16  size,
                      UInt8 transactionIdPresent,
                      /* SPR 2812 changes start */
/* SPACC_OPTIMIZATION */
                      void *  transId,
/* SPACC_OPTIMIZATION */
                      /* SPR 2812 changes end */
                      UInt16  serviceRequested,
                      UInt32  pktArrTime,
                      UInt8   ipPktType,
                      UInt8   voiceSilenceType,
                      UInt32  avgPduSize);
/* SPR 5178 changes ends here */
#endif /* _LIB_PDCP_DATA_TRANSFER_TX_DEF_ */
