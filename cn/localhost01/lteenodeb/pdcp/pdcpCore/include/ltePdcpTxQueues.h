/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpTxQueues.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:11 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : PDCP TX queues definitions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpTxQueues.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:11  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:11:44  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.4  2009/07/20 08:21:52  gur19479
 * Changes done for SRB handling
 *
 * Revision 1.2.2.3  2009/07/08 08:42:38  gur19836
 * Tx flow Optimization changes
 *
 * Revision 1.2.2.2  2009/07/07 11:37:15  gur19140
 * *** empty log message ***
 *
 * Revision 1.2.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/21 07:13:43  gur19140
 * fixes
 *
 * Revision 1.5  2009/05/15 18:43:22  gur19140
 * fixes
 *
 * Revision 1.4  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _LIB_PDCP_TX_QUEUE_DEF_
#define _LIB_PDCP_TX_QUEUE_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include <lteSQueue.h>
#include <lteQueue.h>
#include "lteCircQueue.h"
#include "lteLayer2RlcPdcpInterface.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/
extern volatile LTE_SQUEUE pdcpTxPendingDeletionQ_g;
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
extern volatile LTE_CIRCQUEUE pdcpRlcDataIndQ_g;
extern volatile LTE_CIRCQUEUE rlcDeliveryStatusIndQ_g;
#endif
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define PDCP_MAX_DELIVERY_IND 4096
/*wrapper macros definitions for PDCP TX queue (at each UE and LC level)*/
#define QCOUNT_PDCP_TX_DATA_REQ_Q(pdcpEntityContext_p)\
    queueCount(&(pdcpEntityContext_p->pdcpTxDataReqQ) )

#define ENQUEUE_PDCP_TX_DATA_REQ_Q(pdcpEntityContext_p, txDataReq_p){\
    enQueue(&(pdcpEntityContext_p->pdcpTxDataReqQ), \
            &(txDataReq_p->node ) );\
}
#define DEQUEUE_PDCP_TX_DATA_REQ_Q(pdcpEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpDataReq)deQueue( \
            &(pdcpEntityContext_p->pdcpTxDataReqQ) );\
}
#define GET_FIRST_PDCP_TX_DATA_REQ_Q(pdcpCtx_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpDataReq)getFirstQueueNode(&(pdcpCtx_p->pdcpTxDataReqQ));\
}
#define GET_NEXT_PDCP_TX_DATA_REQ_Q(pdcpCtx_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpDataReq)getNextQueueNode( \
                                   &(pdcpCtx_p->pdcpTxDataReqQ), \
                                   &(txDataReq_p->node) );\
}
#define DEQUEUE_PDCP_SRB_TX_DATA_REQ_Q(pdcpEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpSrbDataReq)deQueue( \
            &(pdcpEntityContext_p->pdcpTxDataReqQ) );\
}
#define GET_FIRST_PDCP_SRB_TX_DATA_REQ_Q(pdcpCtx_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpSrbDataReq)getFirstQueueNode(&(pdcpCtx_p->pdcpTxDataReqQ));\
}
#define GET_NEXT_PDCP_SRB_TX_DATA_REQ_Q(pdcpCtx_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpSrbDataReq)getNextQueueNode( \
                                   &(pdcpCtx_p->pdcpTxDataReqQ), \
                                   &(txDataReq_p->node) );\
}

/*
** wrapper macros definitions for PDCP TX storage queue (at each UE and LC 
** level)
*/
#define QCOUNT_PDCP_TX_STORAGE_Q(pdcpEntityContext_p)\
    queueCount(&(pdcpEntityContext_p->pdcpStorageQ) )
#define ENQUEUE_PDCP_TX_STORAGE_Q(pdcpEntityContext_p, txDataReq_p){\
    enQueue( &(pdcpEntityContext_p->pdcpStorageQ), \
             &(txDataReq_p->node) ); \
}
#define DEQUEUE_PDCP_TX_STORAGE_Q(pdcpEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpDataReq)deQueue( &(pdcpEntityContext_p-> \
                                             pdcpStorageQ) );\
}
#define DEQUEUE_PDCP_SRB_TX_STORAGE_Q(pdcpEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpSrbDataReq)deQueue( &(pdcpEntityContext_p-> \
                                             pdcpStorageQ) );\
}
#define GET_NEXT_PDCP_TX_STORAGE_Q(pdcpCtx_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpDataReq)getNextQueueNode( \
                                &(pdcpCtx_p->pdcpStorageQ), \
                                &(txDataReq_p->node ));\
}
#define GET_PREV_PDCP_TX_STORAGE_Q(txDataReq_p){\
    txDataReq_p = (LP_PdcpDataReq)getPrevQueueNode( \
                                &(txDataReq_p->node) );\
}
#define GET_FIRST_PDCP_TX_STORAGE_Q(pdcpEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpDataReq)getFirstQueueNode( &(pdcpEntityContext_p-> \
                                                       pdcpStorageQ) );\
}
/* + SPR_17858_46_CHANGES */
#define DELETE_NODE_PDCP_TX_STORAGE_Q(pdcpEntityContext_p, txDataReq_p){\
    if(queueCount(&(pdcpEntityContext_p->pdcpStorageQ)))\
            delQueueNode( &(pdcpEntityContext_p->pdcpStorageQ), \
                              &(txDataReq_p->node) );\
}
/* - SPR_17858_46_CHANGES */
#define GET_NEXT_PDCP_SRB_TX_STORAGE_Q(pdcpCtx_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpSrbDataReq)getNextQueueNode( \
                                &(pdcpCtx_p->pdcpStorageQ), \
                                &(txDataReq_p->node ));\
}
#define GET_PREV_PDCP_SRB_TX_STORAGE_Q(txDataReq_p){\
    txDataReq_p = (LP_PdcpSrbDataReq)getPrevQueueNode( \
                                &(txDataReq_p->node) );\
}
#define GET_FIRST_PDCP_SRB_TX_STORAGE_Q(pdcpEntityContext_p, txDataReq_p){\
    txDataReq_p = (LP_PdcpSrbDataReq)getFirstQueueNode( &(pdcpEntityContext_p-> \
                                                       pdcpStorageQ) );\
}

/** CA 2 Core Changes Start **/
#ifdef UE_SIM_TESTING

/*wrapper macros definitions for PDCP Delivery indication TX queue*/ 
#define QINIT_PDCP_TX_DELIVERY_IND_Q()\
/* + SPR 9253 */\
   circQInit((LTE_CIRCQUEUE*)&pdcpTxDeliveryIndQ_g, MAX_UE_SUPPORTED * MAX_LOGICAL_CHANNEL )\
/* - SPR 9253 */

#define QCOUNT_PDCP_TX_DELIVERY_IND_Q()\
    circQueueCount((LTE_CIRCQUEUE*)&pdcpTxDeliveryIndQ_g )

#define ENQUEUE_PDCP_TX_DELIVERY_IND_Q(txDeliveryInd_p)\
    pushNodeCircQ((LTE_CIRCQUEUE*)&pdcpTxDeliveryIndQ_g, txDeliveryInd_p ) 

#define DEQUEUE_PDCP_TX_DELIVERY_IND_Q(txDeliveryInd_p){\
    popNodeCircQ( (LTE_CIRCQUEUE*)&pdcpTxDeliveryIndQ_g, \
                  (void **)&txDeliveryInd_p);\
    txDeliveryInd_p = (LP_PdcpDeliveryInd)txDeliveryInd_p;\
}
/*++530-coverity-54621*/
#define QDEINIT_PDCP_TX_DELIVERY_IND_Q()\
   circQDeInit((LTE_CIRCQUEUE*)&pdcpTxDeliveryIndQ_g)
/*++530-coverity-54621*/

#else


#define QINIT_PDCP_TX_DELIVERY_IND_Q(internalCellIndex)\
/* + SPR 9253 */\
   circQInit((LTE_CIRCQUEUE*)&pdcpTxDeliveryIndQ_g[internalCellIndex], MAX_UE_SUPPORTED * MAX_LOGICAL_CHANNEL )\
/* - SPR 9253 */

#define QCOUNT_PDCP_TX_DELIVERY_IND_Q(internalCellIndex)\
    circQueueCount((LTE_CIRCQUEUE*)&pdcpTxDeliveryIndQ_g[internalCellIndex] )

#define ENQUEUE_PDCP_TX_DELIVERY_IND_Q(txDeliveryInd_p,internalCellIndex)\
    pushNodeCircQ((LTE_CIRCQUEUE*)&pdcpTxDeliveryIndQ_g[internalCellIndex], txDeliveryInd_p )

#define DEQUEUE_PDCP_TX_DELIVERY_IND_Q(txDeliveryInd_p,internalCellIndex){\
    popNodeCircQ( (LTE_CIRCQUEUE*)&pdcpTxDeliveryIndQ_g[internalCellIndex], \
                  (void **)&txDeliveryInd_p);\
    txDeliveryInd_p = (LP_PdcpDeliveryInd)txDeliveryInd_p;\
}
/*++530-coverity-54621*/
#define QDEINIT_PDCP_TX_DELIVERY_IND_Q(internalCellIndex)\
   circQDeInit((LTE_CIRCQUEUE*)&pdcpTxDeliveryIndQ_g[internalCellIndex])
/*++530-coverity-54621*/

#endif
/** CA 2 Core Changes End **/


/*wrapper macros definitions for PDCP TX Pending Deletion queue*/
#define QINIT_PDCP_TX_PENDING_DELETION_Q()\
    sQueueInit((LTE_SQUEUE*)&pdcpTxPendingDeletionQ_g )

#define QCOUNT_PDCP_TX_PENDING_DELETION_Q()\
    sQueueCount((LTE_SQUEUE*)&pdcpTxPendingDeletionQ_g )


#define ENQUEUE_PDCP_TX_PENDING_DELETION_Q(txPendingDeletionInd_p)\
    pushNode((LTE_SQUEUE*)&pdcpTxPendingDeletionQ_g, &(txPendingDeletionInd_p)->sNode ) 


#define DEQUEUE_PDCP_TX_PENDING_DELETION_Q(txPendingDeletionInd_p){\
    txPendingDeletionInd_p = (LP_PdcpTxPendingDeletionReq)popNode((LTE_SQUEUE*) &pdcpTxPendingDeletionQ_g );\
}

/*wrapper macros definitions for PDCP Data Forwarding Downlink queue*/
#define QCOUNT_PDCP_DATA_FWDING_DL_Q(ueCtx_p)\
    queueCount(&(ueCtx_p->pdcpDataForwardingDlQ) )

#define ENQUEUE_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dlDataReq_p){\
    enQueue(&(ueCtx_p->pdcpDataForwardingDlQ), \
             &(dlDataReq_p->node)) ;\
}
#define DEQUEUE_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dlDataReq_p){\
    dlDataReq_p = (LP_PdcpDataReq)deQueue( &(ueCtx_p-> \
                pdcpDataForwardingDlQ) );\
}
/* SRP 3608 changes start */
#define GET_FIRST_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dlDataReq_p){\
        dlDataReq_p = (LP_PdcpDataReq)getFirstQueueNode( &(ueCtx_p-> \
                    pdcpDataForwardingDlQ) );\
}
#define GET_NEXT_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dlDataReq_p){\
	dlDataReq_p = (LP_PdcpDataReq)getNextQueueNode( \
                        &(ueCtx_p->pdcpDataForwardingDlQ),\
		              &(dlDataReq_p->node ));\
}

#define DELETE_NODE_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dlDataReq_p){\
    delQueueNode(  &(ueCtx_p->pdcpDataForwardingDlQ), \
                  &(dlDataReq_p->node) );\
}
/* SPR 3608 changes end */

#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)

/* Queue operations for RLC_DATA_IND_Q */ 
/* + SPR 9253 */ 
/* Dimensioned for Maximum expected PDCP PDUs for 32 Active LCs 
* simultaneously. Normally usage should be much lower than this*/ 
#define MAXIMUM_PDCP_RLC_DATA_IND_NUM 1024*32
/* - SPR 9253 */ 

#define QINIT_PDCP_RLC_DATA_IND_Q()\
   /* + SPR 9253 */ \
   circQInit((LTE_CIRCQUEUE*)&pdcpRlcDataIndQ_g, MAXIMUM_PDCP_RLC_DATA_IND_NUM)\
   /* - SPR 9253 */

#define QCOUNT_PDCP_RLC_DATA_IND_Q()\
    circQueueCount((LTE_CIRCQUEUE*)&pdcpRlcDataIndQ_g )

#define ENQUEUE_PDCP_RLC_DATA_IND_Q(inPdcpRlcDataInd_p)\
    pushNodeCircQ((LTE_CIRCQUEUE*)&pdcpRlcDataIndQ_g, inPdcpRlcDataInd_p) 

#define DEQUEUE_PDCP_RLC_DATA_IND_Q(pdcpRlcDataInd_p){\
    popNodeCircQ((LTE_CIRCQUEUE*)&pdcpRlcDataIndQ_g, (void **)&pdcpRlcDataInd_p);\
    pdcpRlcDataInd_p = (PdcpRlcDataInd * )pdcpRlcDataInd_p;\
}

#define QDEINIT_PDCP_RLC_DATA_IND_Q()\
   circQDeInit((LTE_CIRCQUEUE*)&pdcpRlcDataIndQ_g)

/*Queue Operations for RLC_DELIVERY_STAUS_IND_Q */
#define QINIT_RLC_DELIVERY_STATUS_IND_Q()\
    /* + SPR 9253 */ \
    circQInit((LTE_CIRCQUEUE*)&rlcDeliveryStatusIndQ_g, MAX_UE_SUPPORTED*MAX_LOGICAL_CHANNEL )\
    /* - SPR 9253 */

#define QCOUNT_RLC_DELIVERY_STATUS_IND_Q()\
    circQueueCount((LTE_CIRCQUEUE*)&rlcDeliveryStatusIndQ_g )

#define ENQUEUE_RLC_DELIVERY_STATUS_IND_Q(inRlcDeliveryStatusInd_p)\
    pushNodeCircQ((LTE_CIRCQUEUE*)&rlcDeliveryStatusIndQ_g, inRlcDeliveryStatusInd_p) 

#define DEQUEUE_RLC_DELIVERY_STATUS_IND_Q(rlcDeliveryStatusInd_p){\
    popNodeCircQ((LTE_CIRCQUEUE*)&rlcDeliveryStatusIndQ_g, (void **)&rlcDeliveryStatusInd_p);\
    rlcDeliveryStatusInd_p = (RlcDeliveryStatusInd *) rlcDeliveryStatusInd_p;\
}

#define QDEINIT_RLC_DELIVERY_STATUS_IND_Q()\
   circQDeInit((LTE_CIRCQUEUE*)&rlcDeliveryStatusIndQ_g)

#endif

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* PDCP Control PDU types */
enum PdcpControlPduIndEnumT
{
    PDCP_TX_CONTROL_PDU_IND = 0,    /* Control PDU for transmission to RLC layer */
    PDCP_RX_CONTROL_PDU_IND,        /* Control PDU for handling at PDCP layer */
};

enum PdcpRadioBearerEnumT
{
    PDCP_DATA_RADIO_BEARER = 0,    /* Data Radio Bearer */
    PDCP_SIG_RADIO_BEARER,         /* Sig Radio Bearer */
    PDCP_RETRANS_DATA,             /* Retransmission Case */
};

/* Structure containing common information for the packets */
typedef struct PdcpCommDataReqT
{
    UInt32         count;
    UInt16         sn;
    UInt16         ueIndex;
    UInt16         size;
    UInt8         lcId;
    /* SPR 3406 changes start */
#ifdef PDCP_ASYNC_INTERFACE
    UInt8          isPacketWithSecEngine;
#endif
    /* SPR 20570 Fix Start */
    UInt8  voiceSilenceType;
    /* SPR 20570 Fix End */
    /* SPR 3406 changes end */
} *LP_PdcpCommDataReq, PdcpCommDataReq;

/* Structure for SRB packets */
typedef struct PdcpSrbDataReqT {
    LTE_QUEUE_NODE node;
    void*          data_p;
    /* SPR 15909 fix start */
    tickType_t     tickAssociated;
    /* SPR 15909 fix end */
/* SPR 23260 + */
#ifndef __x86_64__
    UInt32         transactionId;
#else
    UInt64         transactionId;
#endif
/* SPR 23260 - */
    PdcpCommDataReq  commDataReq;
    UInt8            srbServiceReqested;
} PdcpSrbDataReq, *LP_PdcpSrbDataReq;

/* Structure for DRB packets */
typedef struct PdcpDataReqT {
    LTE_QUEUE_NODE   node;
    void*            data_p;
    /* SPR 15909 fix start */
    tickType_t       tickAssociated;
    /* SPR 15909 fix end */

    PdcpCommDataReq  commDataReq;
    /* +- SPR 20570 FIx */
} PdcpDataReq, *LP_PdcpDataReq;

typedef struct PdcpStatusBufferT {
    /* SPR 2812 changes start */
    void * transactionId;
    /* SPR 2812 changes end */
/* FIXED SPR 869 ---- START */
    UInt32 deliveryStatus;
}PdcpStatusBuffer;

/*typedef for delivery status indication between the RLC layer and PDCP layer*/
typedef struct PdcpDeliveryIndT {
    LTE_SNODE   sNode;
    UInt16      count;
/* SPR 23260 + */
#ifndef __x86_64__
    UInt32         transactionId;
#else
    UInt64         transactionId;
#endif
/* SPR 23260 - */
    UInt16      ueIndex;
    UInt16      lcId;
    /*SPR 925 FIXED -----START*/
    PdcpStatusBuffer  *deliveryStatusArr;
    /*SPR 925 FIXED -----END*/
} PdcpDeliveryInd, *LP_PdcpDeliveryInd;

/*typedef for PDCP Control PDU indication */
typedef struct PdcpControlPduIndT {
    LTE_SNODE   sNode;
    UInt16		type;   /* control PDU type (PDCP_TX_CONTROL_PDU, PDCP_RX_CONTROL_PDU)*/
    UInt16      ueIndex;
    UInt16      lcId;
    /* SRP 3608 changes start */
    UInt16      ctrl_type;
    /* SPR 3608 changes end */
    /* SPR 15909 fix start */
    tickType_t   tickAssociated;
    /* SPR 15909 fix end */
    void*       data_p;
} PdcpControlPduInd, *LP_PdcpControlPduInd;

#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
typedef struct RlcDeliveryStatusIndT
{
    UInt32      count;
    UInt16      ueIndex;
    UInt16      lcId;
    RlcStatusBuffer* deliveryStatus_p;
} RlcDeliveryStatusInd;
#endif

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* This queue is data interface from the RLC layer to the PDCP layer 
   for Delivery status indication reception.
 */
/** CA 2 Core Changes Start **/
#ifdef UE_SIM_TESTING
extern volatile LTE_CIRCQUEUE pdcpTxDeliveryIndQ_g;
#else
extern volatile LTE_CIRCQUEUE pdcpTxDeliveryIndQ_g[MAX_NUM_CELL];
#endif
/** CA 2 Core Changes End **/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : txQueuesCreateMemPool 
 * Inputs         : maxUeNumber - Maximum number of supported UEs
 *                  txReservationCoef - Number of blocks for TX purposes
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code.
 * Description    : Reserve memory in pools for PDCP TX queuing purposes.
 ****************************************************************************/
  /* +- SPR 17777 */
UInt16 txQueuesCreateMemPool(
        );
/* +- SPR 17777 */

/****************************************************************************
 * Function Name  : txQueuesInit 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It inits the set of PDCP TX queues which are used for
 *                  data processing on TX path.
 ****************************************************************************/
/* + SPR 17439 */
void txQueuesInit(void);

/****************************************************************************
 * Function Name  : txQueuesCleanup 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It cleanup the set of PDCP TX queues which are used 
 for data processing on TX path.
 ****************************************************************************/ 
void txQueuesCleanup(void);
/* - SPR 17439 */
/****************************************************************************
 * Function Name  : pdcpFreeTxDataReq 
 * Inputs         : LP_PdcpDataReq *dataReq_p   
 * Outputs        : None.
 * Returns        : None.
 * Description    : To free LP_PdcpDataReq
 ****************************************************************************/ 
void pdcpFreeTxDataReq(LP_PdcpDataReq dataReq_p);

/****************************************************************************
 * Function Name  : pdcpFreeSrbTxDataReq 
 * Inputs         : LP_PdcpSrbDataReq *dataReq_p   
 * Outputs        : None.
 * Returns        : None.
 * Description    : To free LP_PdcpDataReq
 ****************************************************************************/ 
void pdcpFreeSrbTxDataReq(LP_PdcpSrbDataReq dataReq_p);

/****************************************************************************
 * Function Name  : pdcpAllocateSrbTxDataReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  transactionId - Transaction Id
 * Outputs        : None.
 * Returns        : LP_PdcpSrbDataReq pointer on TX data request structure or NULL.
 * Description    : To allocate and init LP_PdcpDataReq
 ****************************************************************************/ 
LP_PdcpSrbDataReq pdcpAllocateSrbTxDataReq( UInt16 ueIndex, UInt16 lcId, 
        UInt32  transactionId, UInt16 sn,UInt16 size, UInt8 serviceRequested );

/****************************************************************************
 * Function Name  : pdcpAllocateTxDataReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  transactionId - Transaction Id
 * Outputs        : None.
 * Returns        : LP_PdcpDataReq pointer on TX data request structure or NULL.
 * Description    : To allocate and init LP_PdcpDataReq
 ****************************************************************************/ 
LP_PdcpDataReq pdcpAllocateTxDataReq( UInt16 ueIndex, UInt16 lcId, 
        UInt16 sn,UInt16 size );

/****************************************************************************
 * Function Name  : pdcpAllocateTxDeliveryInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  transactionId  - will define the transaction uniquely. 
 *                  deliveryStatus - the delivery status.
 * Outputs        : None.
 * Returns        : LP_PdcpDeliveryInd pointer on TX delivery status 
 *                  indication structure or NULL.
 * Description    : To allocate and init PdcpDeliveryInd
 ****************************************************************************/ 
LP_PdcpDeliveryInd pdcpAllocateTxDeliveryInd( UInt16 ueIndex, UInt16 lcId, 
        UInt32 count, PdcpStatusBuffer *deliveryStatus );

/* SRP 3608 changes start */
/****************************************************************************
 * Function Name  : pdcpStoreControlPduInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  type    - control PDU type (PDCP_TX_CONTROL_PDU, 
 *                            PDCP_RX_CONTROL_PDU)
 *                  controlPdu_p - pointer on Control PDU ZCB buffer
 *                  ctrl_type - Control PDU type (PSR/ROHC FEEDBCK)
 * Outputs        : None.
 * Returns        : LP_PdcpControlPduInd pointer on PDCP Control PDU
 *                  indication structure or NULL.
 * Description    : It allocates and stores PdcpControlPduInd structure in
 *                  PDCP Control PDU TX queue.
 ****************************************************************************/ 
LP_PdcpControlPduInd pdcpStoreControlPduInd( UInt16 ueIndex, UInt16 lcId,
        UInt16 type, void*  controlPdu_p, UInt8 ctrl_type);
/* SRP 3608 changes end */

/****************************************************************************
 * Function Name  : pdcpGetControlPduInd 
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : LP_PdcpControlPduInd pointer on PDCP Control PDU
 *                  indication structure or NULL.
 * Description    : It dequeues PdcpControlPduInd structure form PDCP Control
 *                  PDU TX queue.
 ****************************************************************************/ 
LP_PdcpControlPduInd pdcpGetControlPduInd( void );

/****************************************************************************
 * Function Name  : pdcpFreeControlPduInd 
 * Inputs         : controlPduInd_p - pointer on PDCP Control PDU indication
 *                                    structure.
 * Outputs        : None.
 * Returns        : None.
 * Description    : It frees memory allocated for PdcpControlPduInd structure.
 ****************************************************************************/ 
void pdcpFreeControlPduInd( LP_PdcpControlPduInd controlPduInd_p );

#endif /* _LIB_PDCP_TX_QUEUE_DEF_ */
