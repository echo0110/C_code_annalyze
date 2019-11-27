/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2RlcPdcpInterface.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:45:45 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This file describe the APIs with the PDCP.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/
#ifndef RLCPDCPINTERFACE_H 
#define RLCPDCPINTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteTypes.h"
#include "lteLayer2CommanTypes.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

typedef struct RlcStatusBufferT {                    
    /* SPR 2812 changes start */
    void * transId ;
/* SPACC_OPTIMIZATION */
    /* SPR 2812 changes end */
    UInt32 deliveryStatus ;
} RlcStatusBuffer ;





/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef enum RlcPdcpRetCodeT {
    RLC_PDCP_FAIL,
    RLC_PDCP_SUCCESS,
    RLC_PDCP_ENTITY_NOT_ACTIVE

} RlcPdcpRetCode;

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* Possible values for transactionIdPresent parameter in rlcUeDataReq function
 */
#define RLC_TRANSACTION_ID_VALID       LTE_TRUE
#define RLC_TRANSACTION_ID_INVALID     LTE_FALSE
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* RLC has to implement these request */
/* This function will be used by RLC layer to pass the voice
 * packet information for SPS LC to MAC */
UInt32 rlcUeDataReq (UInt16  ueIndex, UInt16  lcId, 
        UInt8 * data_p,UInt16  size, UInt8 transactionIdPresent,
        /* SPR 2812 changes start */
        void *transId,
        /* SPR 2812 changes end */
        UInt16 serviceRequested,
        tickType_t pktArrTime,
        UInt8   ipPktType,
        UInt8   voiceSilenceType,
        UInt32  avgPduSize 
        ) ;
                    
UInt32 rlcDataDiscardReq (UInt16  ueIndex,
        UInt16  lcId,
        void *transactionId );       

/* PDCP has to implement these indications*/
UInt32 rlcUeDataInd( UInt16  ueIndex, UInt16  lcId, 
        UInt8 * data_p, UInt16  size,
        UInt8 packetForReEstablishment,
                     /* SPR 15909 fix start */
        tickType_t headPktTS,
        UInt8 macState
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        ,tickType_t packetTime
#endif
        );

UInt32 rlcDeliveryStatusInd(UInt16  ueIndex, UInt16  lcId,
        UInt32 transId, RlcStatusBuffer* rlcStatusBuffer_p);

#ifndef UE_SIM_TESTING
void rlcSubFrameInd(InternalCellIndex internalCellIndex);

void rlcFirstSubFrameInd(
        UInt32 sysFrameNum, 
        UInt32 subFrameNum, 
        InternalCellIndex internalCellIndex);

#else

/* + SPR 17439 */
void rlcSubFrameInd(void);
/* - SPR 17439 */

void rlcFirstSubFrameInd(
             UInt32 sysFrameNum, 
             UInt32 subFrameNum);
#endif

#endif
