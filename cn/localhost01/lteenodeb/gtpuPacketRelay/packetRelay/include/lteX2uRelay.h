#ifndef _LTE_X2U_RELAY_H_
#define _LTE_X2U_RELAY_H_

/******************************************************************************
 *
 *   ARICENT -
 *
 *   Copyright (C) 2015 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *   File Name   : lteX2uRelay.h
 *
 *   Description : This file contains declaration of public APIs used by X2U
 *                 Relay module.
 *
 *****************************************************************************/
#ifdef ENDC_ENABLED
#include "stdio.h"
#include "lteMemPool.h"
#include "lteMsgPool.h" 
#include "lteLayer2CommanTypes.h"
#include "lteLayer2RlcPdcpInterface.h"
#include "lteCircQueue.h"
#include "lteSQueue.h"
#include "ltePdcpOamInterface.h"
#include "lteQueue.h"
#include "lteList.h"
#include "lteRelayOamInterface.h"
#ifndef PDCP_GTPU_INTF
#include "ltePdcpPdcpUserSap.h"
#endif

/***********************************************************
 * MACRO DEFINATIONS
 ***********************************************************/

/* Min-Max ranges used by X2U*/
#define MAX_X2U_BUFFER_SIZE                1000000
#define HIGHEST_X2U_SN                     0xFFF
#define MAX_LCID_PER_UE                          8
#define MAX_SN_WINDOW_LENGTH                  2048
#define DL_USER_DATA_PDU_TYPE                    0
#define MIN_RAN_CONTAINER_EXTN_LEN               8
#define X2U_RAN_CONTAINER_MIN_LEN               20
#define X2U_RAN_CONTAINER_PDU_TYPE_1_MIN_LEN     6
#define MAX_NUM_OF_LOST_X2U_PKT_RANGES         255
#define MIN_DELIVERY_STATUS_THRESHOLD           10
#define MAX_DELIVERY_STATUS_THRESHOLD        65535
#define PDCP_MAX_HDR_SIZE                        2
#define NRU_SN_WINDOW_SIZE                  132000
#define HIGHEST_12BIT_PDCP_SN                 4096
#define HIGHEST_18BIT_PDCP_SN               262144
#define DESIRED_BUFFER_SIZE_RESET_TIMER     1000
#define PDCP_SN_18_BIT                          18
#define PDCP_SN_12_BIT                          12
#define NR_PDCP_TS_PDU_DRB_SN_SMALL_MASK   0xF
#define NR_PDCP_TS_PDU_DRB_SN_BIG_MASK_HI  0x3
/*SN size in bits*/
#define PDCP_TS_SRB_SN_SIZE         5
#define PDCP_TS_DRB_SN_SIZE_BIG     12
#define PDCP_TS_DRB_SN_SIZE_SMALL   7

/* Possible values for serviceRequested parameter in rlcUeDataReq function */
#define X2U_RLC_REQ_ACK_TRUE               LTE_TRUE
#define X2U_RLC_REQ_ACK_FALSE              LTE_FALSE
#define X2U_RLC_TRANSACTION_ID_VALID       LTE_TRUE
#define X2U_RLC_TRANSACTION_ID_INVALID     LTE_FALSE


/* Possible values for deliveryStatus parameter in rlcDeliveryStatusInd function
* */
#define X2U_RLC_ACK    LTE_TRUE
#define X2U_RLC_NACK   LTE_FALSE

#define X2U_RLC_Q_FULL   2

/* RLC-X2U APIs*/
#define X2U_RLC_DL_DATA_REQ 12
#define RLC_X2U_DL_DATA_DELIVERY_STATUS 13
#define RLC_X2U_UL_DATA_IND 14
#define X2U_RLC_DATA_DISCARD_REQ 15


#define GET_LC_INDEX(lcId) (lcId - LTE_DRB_LC_ID_LOW_BOUND)
#define GET_BYTESIZE_FROM_BITSIZE(bitSize_par) ( (UInt16)((DIVIDE_BY_EIGHT((bitSize_par)-1))+1) )

#define GET_X2U_UE_CONTEXT(ueIndex)\
x2uRelayContext_g.x2uRelayUeInfo[ueIndex].x2uUeContext_p

#define GET_UE_DRB_CONTEXT(ueIndex, lcId)\
x2uRelayContext_g.x2uRelayUeInfo[ueIndex].drbEntityContexts[lcId - LTE_DRB_LC_ID_LOW_BOUND]

#ifndef PDCP_GTPU_INTF
#define GET_X2U_CURRENT_TICK(x2uTick)\
{\
    x2uTick = pdcpCurrentTime_g;\
}
#else
#define GET_X2U_CURRENT_TICK(x2uTick)\
{\
    x2uTick = relayGtpuCurrentTick_g;\
}
#endif

#define UPDATE_AND_RESET_TRIGGER_PARAMS(currentTick)\
{\
    x2uRelayContext_g.timerEndTTI = currentTick + \
    x2uRelayContext_g.deliveryStatusTriggerTimer;\
    x2uRelayContext_g.totalPacketCount = 0;\
    x2uRelayContext_g.totalPacketSize = 0;\
}

/*wrapper macros definitions for X2U TX storage queue (at each UE and LC level)*/
#define QCOUNT_X2U_TX_STORAGE_Q(x2uDrbEntityContext_p)\
    queueCount(&(x2uDrbEntityContext_p->x2uTxStorageQ) )

#define ENQUEUE_X2U_TX_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    enQueue( &(x2uDrbEntityContext_p->x2uTxStorageQ), \
                           &(txDataReq_p->node) ); \
}
#define DEQUEUE_X2U_TX_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_X2uDataReq)deQueue( &(x2uDrbEntityContext_p-> \
                                                 x2uTxStorageQ) );\
}
#define GET_NEXT_X2U_TX_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_X2uDataReq)getNextQueueNode( \
                                    &(x2uDrbEntityContext_p->x2uTxStorageQ), \
                                    &(txDataReq_p->node ));\
}
#define GET_PREV_X2U_TX_STORAGE_Q(txDataReq_p){\
    txDataReq_p = (LP_X2uDataReq)getPrevQueueNode( \
                             &(txDataReq_p->node) );\
}
#define GET_FIRST_X2U_TX_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_X2uDataReq)getFirstQueueNode( &(x2uDrbEntityContext_p->x2uTxStorageQ) );\
}
#define DELETE_NODE_X2U_TX_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    if(queueCount(&(x2uDrbEntityContext_p->x2uTxStorageQ)))\
                    delQueueNode( &(x2uDrbEntityContext_p->x2uTxStorageQ), \
                                                  &(txDataReq_p->node) );\
}
/*********************************************************************************/



/*********************************************************************************/
/*wrapper macros definitions for X2U RX storage queue (at each UE and LC level)***/

#define QCOUNT_X2U_RX_STORAGE_Q(x2uDrbEntityContext_p)\
    queueCount(&(x2uDrbEntityContext_p->x2uRxStorageQ) )

#define ENQUEUE_X2U_RX_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    enQueue( &(x2uDrbEntityContext_p->x2uRxStorageQ), \
                           &(txDataReq_p->node) ); \
}
#define DEQUEUE_X2U_RX_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_X2uUeDataInd)deQueue( &(x2uDrbEntityContext_p-> \
                                                 x2uRxStorageQ) );\
}
#define GET_NEXT_X2U_RX_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_X2uUeDataInd)getNextQueueNode( \
                                    &(x2uDrbEntityContext_p->x2uRxStorageQ), \
                                    &(txDataReq_p->node ));\
}
#define GET_PREV_X2U_RX_STORAGE_Q(txDataReq_p){\
    txDataReq_p = (LP_X2uUeDataInd)getPrevQueueNode( \
                             &(txDataReq_p->node) );\
}
#define GET_FIRST_X2U_RX_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_X2uUeDataInd)getFirstQueueNode( &(x2uDrbEntityContext_p-> \
                                                       x2uRxStorageQ) );\
}
#define DELETE_NODE_X2U_RX_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    if(queueCount(&(x2uDrbEntityContext_p->x2uRxStorageQ)))\
                    delQueueNode( &(x2uDrbEntityContext_p->x2uRxStorageQ), \
                                                  &(txDataReq_p->node) );\
}
/***********************************************************************************/


/***********************************************************************************/
#if 0
/*Macro definitions for X2U Lost pkt SN storage queue*/
#define QCOUNT_X2U_LOST_SN_STORAGE_Q(x2uDrbEntityContext_p)\
    queueCount(&(X2uDrbEntityContext_p->x2uLostSnStorageQ) )

#define ENQUEUE_X2U_LOST_SN_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    enQueue( &(x2uDrbEntityContext_p->x2uLostSnStorageQ), \
                           &(txDataReq_p->node) ); \
}
#define DEQUEUE_X2U_LOST_SN_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_X2uDataReq)deQueue( &(x2uDrbEntityContext_p-> \
                                                 x2uLostSnStorageQ) );\
}
#define GET_NEXT_X2U_LOST_SN_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_X2uDataReq)getNextQueueNode( \
                                    &(x2uDrbEntityContext_p->x2uLostSnStorageQ), \
                                    &(txDataReq_p->node ));\
}
#define GET_PREV_X2U_LOST_SN_STORAGE_Q(txDataReq_p){\
    txDataReq_p = (LP_X2uDataReq)getPrevQueueNode( \
                             &(txDataReq_p->node) );\
}
#define GET_FIRST_X2U_LOST_SN_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_X2uDataReq)getFirstQueueNode( &(x2uDrbEntityContext_p-> \
                                                       x2uLostSnStorageQ) );\
}
#define DELETE_NODE_X2U_LOST_SN_STORAGE_Q(x2uDrbEntityContext_p, txDataReq_p){\
    if(queueCount(&(x2uDrbEntityContext_p->x2uLostSnStorageQ)))\
                    delQueueNode( &(x2uDrbEntityContext_p->x2uLostSnStorageQ), \
                                                  &(txDataReq_p->node) );\
}
#endif
/***********************************************************************************/


/***********************************************************************************/
/*Macro definitions RLC DL data delivery status storage queue*/
#define QINIT_RLC_TX_DELIVERY_IND_Q()\
\
   circQInit((LTE_CIRCQUEUE*)&rlcTxDeliveryIndQ_g, MAX_UE_SUPPORTED * MAX_LOGICAL_CHANNEL )\
   /

#define QCOUNT_RLC_TX_DELIVERY_IND_Q()\
    circQueueCount((LTE_CIRCQUEUE*)&rlcTxDeliveryIndQ_g )

#define ENQUEUE_RLC_TX_DELIVERY_IND_Q(txDeliveryInd_p)\
    pushNodeCircQ((LTE_CIRCQUEUE*)&rlcTxDeliveryIndQ_g, txDeliveryInd_p )

#define DEQUEUE_RLC_TX_DELIVERY_IND_Q(txDeliveryInd_p){\
    popNodeCircQ( (LTE_CIRCQUEUE*)&rlcTxDeliveryIndQ_g, \
                      (void **)&txDeliveryInd_p);\
                          txDeliveryInd_p = (LP_RlcDeliveryInd)txDeliveryInd_p;\
}
#define QDEINIT_RLC_TX_DELIVERY_IND_Q()\
   circQDeInit((LTE_CIRCQUEUE*)&rlcTxDeliveryIndQ_g)
/***********************************************************************************/

/****************************************************************************
 * Exported Definitions
 ***************************************************************************/

/**************************************************************************
 * ENUMS
 * ************************************************************************/
typedef enum
{
    X2U_FAILURE,
    X2U_SUCCESS
}x2uResultType;
typedef enum
{
    RLC_UM_MODE,
    RLC_AM_MODE
}x2uRlcMode;
typedef enum
{
    X2U_UE_CONNECTED,
    X2U_UE_SUSPENDED
}x2uUeState;
typedef enum 
{
    X2U_FALSE,
    X2U_TRUE
}x2uRelay_bool_et;
typedef enum
{
    X2U_DATA_NOT_FWDED,
    X2U_DATA_FWDING,
    X2U_DATA_FWDED
}x2uDataFwdingState;
typedef enum
{
    DISCARD_TIMER_INFINITY = 0,
    DISCARD_TIMER_50MS = 50,
    DISCARD_TIMER_100MS = 100,
    DISCARD_TIMER_150MS = 150,
    DISCARD_TIMER_300MS = 300,
    DISCARD_TIMER_500MS = 500,
    DISCARD_TIMER_750MS = 750,
    DISCARD_TIMER_1500MS = 1500
}xdcpSduDiscardTimer;
typedef struct X2uUeDataIndT
{
    LTE_QUEUE_NODE   node;
    UInt8  packetForReEstablishment;
    UInt16 ueIndex;
    UInt16 lcId;
    UInt16 sn;
    UInt16 dataLen;
    tickType_t tickAssociated;
    void   *data_p;

}X2uUeDataInd, *LP_X2uUeDataInd;

#ifndef PDCP_GTPU_INTF
typedef struct X2uDataForwardReqT
{
    UInt16  ueIndex;    
    UInt8 count;
    UInt8 PaddingBit;   /* Padding bit reqd to make structure byte alligned*/
    dataForwardingInfo dataFwdInfo[8];
}X2uDataForwardReq, *LP_X2uDataForwardReq;
#endif

/*typedef for delivery status indication between the RLC layer and PDCP layer*/
typedef struct RlcDeliveryIndT {
    LTE_SNODE   sNode;
    UInt32      transactionId;
    UInt16      count;
    UInt16      ueIndex;
    UInt16      lcId;
    RlcStatusBuffer  *deliveryStatusArr;
} RlcDeliveryInd, *LP_RlcDeliveryInd;

/* Information stored in each node of storage queue */
typedef struct X2uStorageQNodeT
{
    LTE_QUEUE_NODE   node;

    /* Logical channel Identifier */
    UInt8            lcId;

    /* UE Index */
    UInt16           ueIndex;


    /* Tick associated with this packet */
    tickType_t       tickAssociated;

    /* Opaque pointer prvided by PDCP */
    void*            dlDataReq;

}X2uStorageQNode, *LP_X2uStorageQNode;
/* Structure containing common information for the packets */
typedef struct X2uCommDataReqT
{
    UInt8          lcId;
    UInt16         ueIndex;
    UInt16         size;
    UInt32         sn;
    UInt32         count;
}X2uCommDataReq , *LP_X2uCommDataReq;
/* Structure for DRB packets */
typedef struct X2uDataReqT {
    LTE_QUEUE_NODE   node;

    void*            data_p;

    tickType_t       tickAssociated;

    X2uCommDataReq  commDataReq;

}X2uDataReq, *LP_X2uDataReq;

/* Structure for Lost packets SN ranges*/
typedef struct X2uLostSNT {
    LTE_LIST_NODE           nodeAnchor;;
    
    /*Stores the start SN of lost range*/
    UInt32 x2uLostSN_Start;    

    /*Stores the end SN of lost range*/
    UInt32 x2uLostSN_End;     

}X2uLostSN, *LP_X2uLostSN;
/* X2U DRB Entity Context */
typedef struct X2uDrbEntityContextT
{
    /* Storage queue to hold information of packets sent to RLC */
    LTE_QUEUE                x2uTxStorageQ;

    /* Storage queue to hold information of UpLink packets */
    LTE_QUEUE                x2uRxStorageQ;
    
    /* Storage queue to hold information of lost packets SN ranges */

    LTE_LIST                 x2uSLostSnRangeList_g;

    /* It indicats availability of status report*/
    UInt8                    isStatusReportAvailable;

    UInt8                    isFirstPktFromMeNB;

    /* it used to store num of lost SN ranges */
    UInt8                    lostX2uRangeReported;

    /* it indicats final frame incase of bearer release*/
    UInt8                    finalFrameInd;

    UInt8                    snSize;
    
    UInt8                    rlcMode;

    /* It store start SN of lost range*/
    UInt32                   snWindowStartIndex;
    
    /* It store end SN of lost range*/
    UInt32                   snWindowEndIndex;

    /*DisacrdTimer value,  0 = no discard*/
    UInt16                   discardTimer;


    /* Available buffer size for this DRB at SeNB */
    UInt32                   drbDesiredBufferSize;

    /* Last Assigned NR PDCP SN to a DL USER DATA PDU */
    UInt32                   highestSuccessfullyDeliveredSN;  
   
   /* Last Assigned NR PDCP to a DL USER DATA PDU */
    UInt32                   highestTransmittedSN;  
    
    /* Last Assigned NR PDCP SN to a DL USER DATA PDU */
    UInt32                   nextExpected_SN;  

    UInt64                   desiredBufferResetTimer;
    
    

}X2uDrbEntityContext, *LP_X2uDrbEntityContext;

typedef struct X2uUeContext
{
    /* Indicate UE CONNECTED/SUSPENDED state */
    UInt8      ueState;

    UInt8      dataFwdState;
    /* Count of active LCs */
    UInt8      activeLcCount;
    
    /* UE Index */
    UInt16     ueIndex;

    UInt16     dataFwdGuardTimerExpiryTTI;

    /* Array of active LC Ids */
    UInt16     activeLc[MAX_LCID_PER_UE];
    
    /* TickAss to trigger UE DELETE*/
    tickType_t               tickAss;

}X2uUeContext, *LP_X2uUeContext;

/* Structure for storing UE context */
typedef struct X2uRelayUeInfoT
{
    /* UE context */
    X2uUeContext *x2uUeContext_p;

    /* DRB Entity contexts array */
    LP_X2uDrbEntityContext drbEntityContexts[(LTE_DRB_LC_ID_HIGH_BOUND - LTE_DRB_LC_ID_LOW_BOUND + 1)];

}X2uRelayUeInfo, *LP_X2uRelayUeInfo;

/* X2U Global context */
typedef struct X2uRelayContextT
{
    /* Initialization flag */
    UInt8                  isInitialized;  

    /* Count of active UEs */
    UInt16                 activeUeCount;

    /*Timer to triggering the DL data delivery status */
    UInt16                 deliveryStatusTriggerTimer;

    /*Threshold pkt count to trigger DL data delivery status*/
    UInt16                 deliveryStatusTriggerPktCnt;
    
    /*Threshold Pkt size to trigger DL data delivery status*/
    UInt16                 deliveryStatusTriggerPktSize;

    /* time interval for retain bearer context
     * while data forwarding is going*/
    UInt16                 dataForwardingGuardTimerInterval;

    /* Total num of DL PDCP packets accumulated at X2U */
    UInt16                 totalPacketCount;

    /* Total size of DL PDCP packets  at X2U */
    UInt16                 totalPacketSize;
    
    /* Stores UE index in active UE list*/
    UInt16                 activeUeIdx[MAX_UE_SUPPORTED];

    /* Stores UE position in active UE list*/
    UInt16                 activeUePos[MAX_UE_SUPPORTED];
    
    /* star TTI of timer for triggering status report*/
    tickType_t             timerStartTTI;

    /* end TTI of timer for triggering status report*/
    tickType_t             timerEndTTI;

    /* GTPU supports NR Header enabled for X2U data*/
    UInt8                  isGtpuNRHeaderEnabled;

    /* Array of UE contexts */
    X2uRelayUeInfo         x2uRelayUeInfo[MAX_UE_SUPPORTED];
}x2uRelayContext;

/****************************************************************************
 *  Functions Declaration
 ***************************************************************************/
void initX2uRelayContext();
void triggerDeliveryStatusReport();
void processRlcDeliveryStatusReport();
void prepareAndSendDLDeliveryStatus();
void freeDrbEntityTxRxQ(LP_X2uDrbEntityContext drbEntity_p);
void prepareAndSendDeliveryStatus(tickType_t currentTick);
void processDiscardPdcpSdu(tickType_t currentTick);
inline LP_X2uDataReq allocateAndCreateDataReq(
              UInt16 ueIndex, 
              UInt16 lcId,
              UInt16 size);
UInt8 x2uRelayUpdateDataFwdTimerAndThresholds(
      x2uDeliveryStatusTriggerThreshold deliveryStatusTimer,
      UInt16 dataForwardingGuardTimerInterval,
      UInt8 isGtpuNRHeaderEnabled);
UInt8 x2uRelayCreateBearerContext(
      UInt16 ueIndex, 
      UInt8 lcId,
      UInt8 snSize,
      UInt8 rlcMod);
UInt8 x2uRelayDeleteBearerContext(
      UInt16 ueIndex, 
      UInt8  lcId); 
UInt8 prX2UdlDataReq(
        UInt16 ueIndex,
        UInt8  lcId,
        UInt8  *data_p,
        UInt32 size);
#if defined (ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
UInt32 rlcX2uUlDataInd(UInt16 ueIndex, UInt8 lcId,
        UInt8  packetForReEstablishment,
        UInt16 size,
        tickType_t headPktTS,
        tickType_t pktTimeStamp,
        void*  data_p);
#elif ENDC_ENABLED
UInt32 rlcX2uUlDataInd(UInt16 ueIndex, UInt8 lcId,
        void*  data_p, UInt16 size);
#endif
void checkForLostPacketRange(
     LP_X2uDrbEntityContext drbEntity_p, 
     UInt32 receivedX2U_SN);
void sendDatatoRlc(
     UInt16 lcId,
     UInt16 ueIndex,
     UInt16 dataLen,
     UInt16 transLen,
     UInt16 serviceRequest,
     UInt8  transactionPresent,
     void   *data_p,
     void   *transId,
     tickType_t pktArrTime);
extern void appendCsplHeader
(
 UInt16 moduleId,
 UInt16 apiId,
 UInt16 totalLen,
 UInt8* msgBuff_p
 );
extern UInt32 rlcX2uDlDataDeliveryStatus(
        UInt16   ueIndex,
        UInt8  lcId,
        UInt32  count,
        RlcStatusBuffer *deliveryStatus_p
        );
extern UInt32 rlcDataDiscardReq ( 
              UInt16 ueIndex,
              UInt16 lcId,
              void *transactionId);
UInt16 sendRlcDiscardReq(UInt16 ueIndex, UInt16 lcId, void *transactionId, UInt16 transIdLen);
/*S1U_DELETE_FIX*/
void updateTickAssAtX2U(UInt16 ueIndex);
/*S1U_DELETE_FIX*/
void sendDeliveryStatusInd(UInt16 ueIndex, UInt8 lcId, LP_X2uDrbEntityContext drbEntity_p);
void pdcpFreeTxX2UDataReq(LP_X2uDataReq dataReq_p);
void resetDesiredBufferSize(tickType_t currentTick);
#endif 
#endif
