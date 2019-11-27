/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcPdcpInterface.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:47 gur20491 Exp $
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
 * $Log: lteRlcPdcpInterface.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:47  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:30  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.11.2.3  2010/07/19 09:08:15  gur19836
 * Review Comments Incorporated
 *
 * Revision 1.11.2.2  2010/07/17 12:52:54  gur21148
 * RLC_UM_MERGES
 *
 * Revision 1.11.2.1  2010/06/21 08:20:25  gur23054
 * Rlc AM Base Code Merged
 *
 * Revision 1.11  2010/06/14 11:59:35  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.10  2009/04/22 04:33:38  gur19836
 * memory leak related changes
 *
 * Revision 1.9  2009/03/23 08:37:17  gur19140
 * Integration changes first waves
 *
 * Revision 1.8  2009/03/06 12:44:42  gur19140
 * Review comments fixes
 *
 * Revision 1.7  2009/03/05 14:12:16  gur19140
 * Peer to Peer changes
 *
 * Revision 1.6  2009/03/02 09:10:54  gur19140
 * Changes reqarding Zero copy,pdcp discard and execution legs.
 *
 * Revision 1.5  2009/03/02 05:34:08  gur19836
 * Updated for OAM Enhancement
 *
 * Revision 1.4  2009/02/13 18:23:28  gur20294
 * changes to run Tx cases
 *
 * Revision 1.3  2009/02/09 11:30:46  gur19140
 * Changes to make Tx Rx thread wait at Sema
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef PDCPINTERFACE_H 
#define PDCPINTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteRlcThread.h"
#include "lteRlcMem.h"
/* +COVERITY 10221 */
//#include "lteRlcUmdSdu.h"
/* -COVERITY 10221 */
#include "lteLayer2RlcPdcpInterface.h"
#include "lteCircQueue.h"

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* Rlc Rel 2.0 change start */
#define MAX_SDU_REACHED_Q_FULL_ERR      4
/* Rlc Rel 2.0 change end */


#define QINIT_RLC_UEDATA_REQ_Q()\
        queueInit(&rlcUeDataReqQ_g)
        
#define QDEINIT_RLC_UEDATA_REQ_Q(){\
    RlcUeDataReq *node_p = RLC_NULL; \
    while((node_p = (RlcUeDataReq *)deQueue(&rlcUeDataReqQ_g)) != RLC_NULL) \
        freeMemPool(node_p);\
    /*SPR 51542 Start*/\
    semDestroy((LTE_SEM *)&(rlcUeDataReqQ_g.sem_lock));\
    /*SPR 51542 End*/\
}    

#define QCOUNT_RLC_UEDATA_REQ_Q()\
        queueCount(&rlcUeDataReqQ_g)
        
#define DEQUEUE_RLC_UEDATA_REQ_Q(rlcUeDataReqNode_p){ \
    rlcUeDataReqNode_p = (RlcUeDataReq *)deQueue(&rlcUeDataReqQ_g);\
}    

#define ENQUEUE_RLC_UEDATA_REQ_Q(inRlcUeDataReq_p) \
       enQueue(&rlcUeDataReqQ_g,&inRlcUeDataReq_p->qN) 
       

#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
#define QINIT_PDCP_RLC_UEDATA_REQ_Q()\
         circQInit(&pdcpRlcUeDataReqQ_g,MAX_NODES)

#define DEQUEUE_PDCP_RLC_UEDATA_REQ_Q(dataReq)\
       popNodeCircQ( &pdcpRlcUeDataReqQ_g, (void *)&dataReq)

#define ENQUEUE_PDCP_RLC_UEDATA_REQ_Q(inPdcpRlcUeDataReq_p){ \
        pushNodeCircQ(&pdcpRlcUeDataReqQ_g,inPdcpRlcUeDataReq_p);\
}

#define QCOUNT_PDCP_RLC_UEDATA_REQ_Q()\
        circQueueCount(&pdcpRlcUeDataReqQ_g)

#define QDEINIT_PDCP_RLC_UEDATA_REQ_Q(){\
    void *cnode = RLC_NULL;\
    DEQUEUE_PDCP_RLC_UEDATA_REQ_Q(cnode);\
    while(cnode  != RLC_NULL){\
        freeMemPool(cnode);\
        if (RLC_SUCCESS != DEQUEUE_PDCP_RLC_UEDATA_REQ_Q(cnode)){\
            cnode = RLC_NULL;}} /*COVERITY 26452*/\
    circQDeInit(&pdcpRlcUeDataReqQ_g);\
}

/* SPR 4497 Changes Start */
#define QINIT_PDCP_RLC_UEDATA_DISCARD_REQ_Q()\
         circQInit(&pdcpRlcUeDataDiscardReqQ_g,MAX_NODES_TO_DISCARD)

#define DEQUEUE_PDCP_RLC_UEDATA_DISCARD_REQ_Q(dataReq)\
       popNodeCircQ( &pdcpRlcUeDataDiscardReqQ_g, (void *)&dataReq)

/* + SPR 9253 */
#define ENQUEUE_PDCP_RLC_UEDATA_DISCARD_REQ_Q(inPdcpRlcUeDataReq_p)\
        pushNodeCircQ(&pdcpRlcUeDataDiscardReqQ_g,(void *)inPdcpRlcUeDataReq_p)

#define QCOUNT_PDCP_RLC_UEDATA_DISCARD_REQ_Q()\
        circQueueCount(&pdcpRlcUeDataDiscardReqQ_g)

#define QDEINIT_PDCP_RLC_UEDATA_DISCARD_REQ_Q(){\
    void *cnode = RLC_NULL;\
    DEQUEUE_PDCP_RLC_UEDATA_DISCARD_REQ_Q(cnode);\
    while(cnode  != RLC_NULL){\
        freeMemPool(cnode);\
        if (RLC_SUCCESS != DEQUEUE_PDCP_RLC_UEDATA_DISCARD_REQ_Q(cnode)){\
        cnode = RLC_NULL;}}/* COVERITY 26452*/\
    circQDeInit(&pdcpRlcUeDataDiscardReqQ_g);\
}
/* SPR 4497 Changes End */

#endif
       
#define MULTIPLY_BY_TWO(x) ((x) << 1)
       
/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct RlcUeDataReqT {
    tickType_t pktArrTime;
    /* SPR 15909 fix end */
    /** Rel 3.1 Changes End **/
   
    /* SPR 2812 changes start */
    void * transId ; /* transaction Id to be associated with delivery status*/
    /* SPR 2812 changes end */
    UInt16 ueIndex ;   /* user equipment index.  */
    UInt16 lcId ;      /* Logical channel Id.  */
    UInt16 size ;   /* Length of data pointed by data_p below  */
    /*serviceRequested is used to request RLC 
      the delivery status on success/failure */
    UInt16 serviceRequested ;
    UInt8 *data_p ;    /* Pointer to data to be transmitted*/
    UInt8 transactionIdPresent; /*It indicate whether transid is presentor not*/
    UInt8 ipPktType;   /* IPv4 or IpV6 */
    UInt8 voiceSilenceType; /* Flag for Silence/Voice detected by PDCP */
    UInt32 avgSduSize;
} RlcUeDataReq; 



/* This queue will be data interface from PDCP layer to RLC layer. */ 
extern LTE_QUEUE rlcUeDataReqQ_g ;
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
extern LTE_CIRCQUEUE pdcpRlcUeDataReqQ_g;
#endif
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* + SPR 17439 */
void rlcInitPdcpInterface(void) ;

void rlcDeInitPdcpInterface(void) ;
/* - SPR 17439 */
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
UInt32 rlcHandleUeDataReq(UInt32 cQCount);
#endif



#ifndef UE_SIM_TESTING
void sendRlcFirstSubframeIndToPdcp ( 
        UInt32 sysFrameNum, 
        UInt32 subFrameNum,
        InternalCellIndex activeCell,
        InternalCellIndex internalCellIndex);

void sendRlcSubframeIndToPdcp (InternalCellIndex internalCellIndex);
#endif

void sendRlcUeDataIndToPdcp ( UInt16  ueIndex,
                              UInt16  lcId,
                              UInt8 * data_p,
                              UInt16  size,
                              UInt8 packetForReEstablishment,
                              tickType_t headPktTS,
                              UInt8 macState
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                              ,tickType_t packetTime
#endif
                            );

#ifdef ENDC_ENABLED
#define BUFFERSIZE 2000
#define RELAY_MODULE_ID         4
void handlePdcpDataFromX2u();
void sendRlcDeliveryStatusToX2U(UInt16 ueIndex, UInt8 lcId, UInt32 rlcStatusBufferCount,
		RlcStatusBuffer *rlcQStatusBuffer_p);
void sendUeDataIndToX2U(UInt16  ueIndex,
        UInt16  lcId,
        UInt8 * data_p,
        UInt16  size,
        UInt8 packetForReEstablishment,
        tickType_t headPktTS,
        tickType_t pktTimeStamp);
extern UInt32 rlcX2uDlDataDeliveryStatus(
        UInt16   ueIndex,
        UInt8  lcId,
        UInt32  count,
        RlcStatusBuffer *deliveryStatus_p
        );
extern UInt32 rlcX2uUlDataInd(UInt16 ueIndex,
        UInt8  lcId,
        void*  data_p,
        UInt16 size);
#endif

#endif  /* PDCPINTERFACE_H */
