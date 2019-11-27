/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpRxQueues.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : PDCP RX queues definitions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpRxQueues.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:18  gur20548
 * Changed header file name
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

#ifndef _LIB_PDCP_RX_QUEUE_DEF_
#define _LIB_PDCP_RX_QUEUE_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include <lteSQueue.h>
#include "lteQueue.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/* This queue will be data interface from the RLC layer to the PDCP layer. */
extern volatile LTE_SQUEUE pdcpRxPendingDeletionQ_g;
#ifdef PDCP_ASYNC_INTERFACE
extern LTE_SQUEUE pdcpRxDecipheredQ_g;
#endif

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*wrapper macros definitions for PDCP Data indication RX queue*/
/* + SPR 19525 */
#define QINIT_PDCP_RX_DATA_IND_Q(pdcpContext_p) \
   queueInit( &((*entity_p)->pdcpRxDataIndQ) );
/* - SPR 19525 */

#define QCOUNT_PDCP_RX_DATA_IND_Q(pdcpContext_p)\
        queueCount( &(pdcpContext_p->pdcpRxDataIndQ) )

#define ENQUEUE_PDCP_RX_DATA_IND_Q(pdcpContext_p, rxDataInd_p)\
       enQueue( &(pdcpContext_p->pdcpRxDataIndQ), &(rxDataInd_p->node) )

#define DEQUEUE_PDCP_RX_DATA_IND_Q(pdcpContext_p, rxDataInd_p){\
    LTE_QUEUE_NODE * snode = deQueue( &(pdcpContext_p->pdcpRxDataIndQ) );\
    rxDataInd_p = MEMBEROF( PdcpDataInd, node, snode );\
}

/* + SPR 19525 */
#define DEINIT_PDCP_RX_DATA_IND_Q(pdcpContext_p) \
   semDestroy((LTE_SEM *)&(pdcpContext_p->pdcpRxDataIndQ).sem_lock )
/* - SPR 19525 */

/*
** wrapper macros definitions for PDCP RX storage queue (at each UE and LC
** level)
*/
#define QCOUNT_PDCP_RX_STORAGE_Q(pdcpEntityContext_p)\
    queueCount(&(pdcpEntityContext_p->pdcpUlReorderingQ) )

#define ENQUEUE_PDCP_RX_STORAGE_Q(pdcpEntityContext_p, rxDataInd_p){\
    enQueue( &(pdcpEntityContext_p->pdcpUlReorderingQ), \
             &(rxDataInd_p->node) ); \
}

#define DEQUEUE_PDCP_RX_STORAGE_Q(pdcpEntityContext_p, rxDataInd_p){\
    rxDataInd_p = (LP_PdcpDataInd)deQueue( &(pdcpEntityContext_p-> \
                                             pdcpUlReorderingQ) );\
}

#define INSERT_BEFORE_PDCP_RX_STORAGE_Q(pdcpEntityCtx_p, nextNode_p, node_p){\
    queueInsertBefore(&(pdcpEntityCtx_p->pdcpUlReorderingQ), \
                      &(nextNode_p->node), &(node_p->node) ); \
}

#define GET_NEXT_PDCP_RX_STORAGE_Q(pdcpCtx_p, rxCurrDataInd_p, rxNextDataInd_p){\
    rxNextDataInd_p = (LP_PdcpDataInd)getNextQueueNode( \
                                &(pdcpCtx_p->pdcpUlReorderingQ), \
                                &(rxCurrDataInd_p->node) );\
}

#define GET_PREV_PDCP_RX_STORAGE_Q(rxDataInd_p, rxPrevDataInd_p){\
    rxPrevDataInd_p = (LP_PdcpDataInd)getPrevQueueNode( \
                      &(rxDataInd_p->node) );\
}

#define GET_FIRST_PDCP_RX_STORAGE_Q(pdcpEntityContext_p, rxDataInd_p){\
    rxDataInd_p = (LP_PdcpDataInd)getFirstQueueNode( &(pdcpEntityContext_p-> \
                                                       pdcpUlReorderingQ) );\
}

#define DELETE_NODE_PDCP_RX_STORAGE_Q(pdcpCtx_p, rxDataInd_p){\
    delQueueNode( &(pdcpCtx_p->pdcpUlReorderingQ), \
                  &(rxDataInd_p->node));\
}

/* wrapper macros definitions for PDCP RX pending deletion queue */
#define QINIT_PDCP_RX_PENDING_DELETION_Q()\
        sQueueInit((LTE_SQUEUE*) &pdcpRxPendingDeletionQ_g )


#define QCOUNT_PDCP_RX_PENDING_DELETION_Q()\
        sQueueCount((LTE_SQUEUE *) &pdcpRxPendingDeletionQ_g )


#define ENQUEUE_PDCP_RX_PENDING_DELETION_Q(rxPendingDeletionInd_p)\
       pushNode((LTE_SQUEUE*) &pdcpRxPendingDeletionQ_g, &(rxPendingDeletionInd_p)->sNode ) 


#define DEQUEUE_PDCP_RX_PENDING_DELETION_Q(rxPendingDeletionInd_p){\
    LTE_SNODE * snode = popNode((LTE_SQUEUE*) &pdcpRxPendingDeletionQ_g );\
    rxPendingDeletionInd_p = MEMBEROF(PdcpRxPendingDeletionReq, sNode, snode );\
}

#ifdef PDCP_ASYNC_INTERFACE
/* wrapper macros definitions for PDCP RX deciphered data queue for RoHC 
   packets */
#define QINIT_PDCP_DECIPHERED_RX_Q()\
        sQueueInit(&pdcpRxDecipheredQ_g );

#define QCOUNT_PDCP_DECIPHERED_RX_Q()\
        sQueueCount(&pdcpRxDecipheredQ_g );

#define ENQUEUE_PDCP_DECIPHERED_RX_Q(rxDataInd_p)\
       pushNode(&pdcpRxDecipheredQ_g, (LTE_SNODE *)&(rxDataInd_p)->node ) 

#define DEQUEUE_PDCP_DECIPHERED_RX_Q(rxDataInd_p){\
    rxDataInd_p = (LP_PdcpDataInd)popNode(&pdcpRxDecipheredQ_g );\
}
#endif
        /*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
        /*SPR 21188*/
extern LTE_CIRCQUEUE pdcpGtpDataIndQ_g;
/* wrapper macros definitions for PDCP data Ind Queue for GTP UL packets */
#define QINIT_PDCP_GTP_DATA_IND_Q() \
   circQInit((LTE_CIRCQUEUE*)&pdcpGtpDataIndQ_g, MAX_UE_SUPPORTED*MAX_LOGICAL_CHANNEL);

#define QCOUNT_PDCP_GTP_DATA_IND_Q() \
   circQueueCount((LTE_CIRCQUEUE*)&pdcpGtpDataIndQ_g)

#define ENQUEUE_PDCP_GTP_DATA_IND_Q(node_p) \
   pushNodeCircQ((LTE_CIRCQUEUE*)&pdcpGtpDataIndQ_g, node_p)

#define DEQUEUE_PDCP_GTP_DATA_IND_Q(node_p) \
   popNodeCircQ((LTE_CIRCQUEUE*)&pdcpGtpDataIndQ_g, (void **)&node_p); \
   node_p = (LP_PdcpUeData)node_p;

/* Following structure is to store the PDCP UL SDUs which can be sent at a time
 * after DMA copy */
typedef struct PdcpUlSduListForGtpT
{
    /* Pointer to UL shared memory pointer */
    UInt8 *sdu_p;
    /* Pointer to UL ZCB data */
    UInt8 *zcbSdu_p;
    /* Size of UL shared memory pointer */
    UInt16 size;
} PdcpUlSduListForGtp, *LP_PdcpUlSduListForGtp;
#endif

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
typedef enum PdcpRlcPacketStateT
{
    NORMAL_PACKET         = 0,
    RE_ESTABLISHED_PACKET = 1,
}PdcpRlcPacketState;

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*typedef for data request between the PDCP user and PDCP layer*/
typedef struct PdcpDataIndT 
{
    LTE_QUEUE_NODE     node;
    void*              data_p;
    UInt32             count;
    /* Head packet TS for UL packets */
    /* SPR 15909 fix start */
    tickType_t         headPktTS;
    /* SPR 15909 fix end */
    UInt16             sn;
    UInt16             size;
    UInt16             ueIndex;
    UInt16             lcId;
    UInt8              packetForReEstablishment;
    /* MAC UL State in case of UL */
    UInt8              macState;
#ifdef PDCP_ASYNC_INTERFACE
    /* Flag to indicate if the packet needs to be discarded */
    UInt8              discardAfterDecompression;
#endif
    /* SPR 3774 change start */
#ifdef L2_FAST_PATH
    /*Flag to indicate slow path or fast path for packet */
    UInt8           pktProcessing;
#endif
    /* SPR 3774 change end */
    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    tickType_t       pktTimeStamp;
    /* SPR 15909 fix end */
#endif
    /* 32.425 Events code changes end*/
} PdcpDataInd, *LP_PdcpDataInd;

#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
typedef struct PdcpRlcDataIndT
{
    UInt16 ueIndex;
    UInt16 lcId;
    UInt8* data_p;
    UInt16 size;
    UInt8 packetForReEstablishment;
    /* Head packet TS for UL packets */
    /* SPR 15909 fix start */
    tickType_t        headPktTS;
    /* SPR 15909 fix end */
    /* MAC UL State in case of UL */
    UInt8              macState;
    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    tickType_t         pktTimeStamp;
    /* SPR 15909 fix end */
#endif
    /* 32.425 Events code changes end*/
} PdcpRlcDataInd, *LP_PdcpRlcDataInd;
#endif

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : rxQueuesCreateMemPool 
 * Inputs         : maxUeNumber - Maximum number of supported UEs
                    rxReservationCoef - Number of blocks for RX purposes
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code.
 * Description    : Reserve memory in pools for PDCP RX queuing purposes.
 ****************************************************************************/
/* +- SPR 17777*/
UInt16 rxQueuesCreateMemPool(void);

/****************************************************************************
 * Function Name  : rxQueuesInit 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It inits the set of PDCP RX queues which are used for data
 *                  processing on TX path.
 ****************************************************************************/
void rxQueuesInit(void);

/****************************************************************************
 * Function Name  : rxQueuesCleanup 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It cleanup the set of PDCP RX queues which are used 
                    for data processing on TX path.
 ****************************************************************************/
void rxQueuesCleanup(void);

/****************************************************************************
 * Function Name  : pdcpFreeRxDataInd
 * Inputs         : LP_PdcpDataReq *dataReq_p   
 * Outputs        : None.
 * Returns        : None.
 * Description    : To free LP_PdcpDataReq
 ****************************************************************************/ 
void pdcpFreeRxDataInd(LP_PdcpDataInd dataInd_p);

/****************************************************************************
 * Function Name  : pdcpAllocateRxDataInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
  * Outputs        : None.
 * Returns        : LP_PdcpDataInd pointer on RX data indication structure or NULL.
 * Description    : To allocate and init LP_PdcpDataInd
 ****************************************************************************/ 
LP_PdcpDataInd pdcpAllocateRxDataInd( UInt16 ueIndex, UInt16 lcId, 
        UInt16 sn, UInt16 size, UInt8 packetForReEstablishment, 
        /* SPR 15909 fix start */
        tickType_t headPktTS, UInt8 macState
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        ,tickType_t packetTime
        /* SPR 15909 fix end */
#endif
    /*32.425 Events code changes end*/
        );

#endif /* _LIB_PDCP_RX_QUEUE_DEF_ */
