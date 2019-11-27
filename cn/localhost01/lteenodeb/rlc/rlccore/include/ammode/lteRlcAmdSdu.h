/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmdSdu.h,v 1.1.6.2.2.1 2010/09/21 15:49:28 gur20491 Exp $ 
 *
 ****************************************************************************
 *
 *  File Description : 
 *                  This file declares the SDU operations of AM Entity
 *
 ****************************************************************************
 *  Revision Details
 * ------------------
 *
 *  $Log: lteRlcAmdSdu.h,v $
 *  Revision 1.1.6.2.2.1  2010/09/21 15:49:28  gur20491
 *  FAPI changes
 *
 *  Revision 1.1.2.2  2010/08/16 04:03:17  gur19836
 *  Integration Issues Fixed
 *
 *  Revision 1.1.2.1  2010/08/10 09:37:45  gur19836
 *  Files added for RLC AM Release
 *
 *  Revision 1.1.6.11  2010/07/28 04:18:22  gur19836
 *  UT Bug Fixes
 *
 *  Revision 1.1.6.10  2010/07/22 15:10:16  gur23054
 *  UT bug fixes for Macro calculate vrheader
 *
 *  Revision 1.1.6.9  2010/07/17 11:33:48  gur23054
 *  Review Comments fixed
 *
 *  Revision 1.1.6.8  2010/07/05 06:38:09  gur23054
 *  Compilation errors removed
 *
 *  Revision 1.1.6.7  2010/06/25 09:51:04  gur23054
 *  Macros Modified
 *
 *  Revision 1.1.6.6  2010/06/25 04:30:28  gur23054
 *  Macro added
 *
 *  Revision 1.1.6.5  2010/06/24 06:53:25  gur19836
 *  RLC AM: Feature Enh - Filling DFEs Info in Retx Array while forming PDU
 *
 *  Revision 1.1.6.4  2010/06/22 11:54:17  gur23054
 *  Compilation Errors Removed
 *
 *  Revision 1.1.6.3  2010/06/22 09:14:20  gur19836
 *  Compilation errors removed
 *
 *  Revision 1.1.6.2  2010/06/21 08:07:32  gur23054
 *  Rlc Amd base code merged
 *
 *  Revision 1.1.6.1  2010/06/17 10:59:39  gur19836
 *  RLC AM Base Code Merged
 *
 *  Revision 1.1.2.9  2009/12/18 17:05:23  gur19140
 *  Addding sn in tid queue
 *
 *  Revision 1.1.2.8  2009/12/15 17:20:22  gur19140
 *  stat variable issue resolved
 *
 *  Revision 1.1.2.7  2009/12/07 12:34:06  gur19140
 *  Uplink Issue
 *
 *  Revision 1.1.2.6  2009/12/04 07:44:54  gur19140
 *  Changes regarding negative cases
 *
 *  Revision 1.1.2.5  2009/11/10 15:02:38  gur19140
 *  Minor changes
 *
 *  Revision 1.1.2.4  2009/11/10 09:08:17  gur19140
 *  Macro updated
 *
 *  Revision 1.1.2.3  2009/10/28 09:01:42  gur19140
 *  Minor changes for file headers
 *
 *
 ****************************************************************************/

#ifndef AMDSDU_H
#define AMDSDU_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcGlobalData.h"
#include "lteRlcTypes.h"
#include "lteRlcPdcpInterface.h"
#include "lteRlcMacInterface.h"
#include "lteRlcArithmetic.h"
#include "lteCircQueue.h"


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*********** SPR 1215 Start, As MAC can give MAX opportunity of 18000(Aricent Assumption),and packet size can be minimize to 30 bytes(in worst case )***/
/***** so need to concatenate mumtiple SDUs, so its Aricent assumtion that In worst case, not more than 1500 SDUs will be concatenate. *****/
#define MAX_DFE_COUNT 1500  
/*****SPR 1215 End ***/
#define LI_BITS 11
#define E_BITS 1
#define MAX_TB_COUNT 4

#define SDU_TID_Q_SIZE 2048
/* + SPR 19808 */
/* The size of AM SDU Queue is taken as 1024 to achieve the full throughput
 * of 150 Mbps in DL for 2X2 MIMO for single UE */
#define MAX_AM_RAW_SDU_Q_SIZE 1024
/* - SPR 19808 */

/*AMD RAW SDU QUEUE INIT*/
#define QUEUE_INIT_AMD_RAW_SDU_Q(AmdRawSduQ)\
   if ((AmdRawSduQ.pQ = ((LTE_CIRCQUEUE *)getMemFromPool(sizeof(LTE_CIRCQUEUE),NULL)))) \
    /* + SPR 19808 */\
    circQInitAfterCellSetup( AmdRawSduQ.pQ, MAX_AM_RAW_SDU_Q_SIZE)
    /* - SPR 19808 */

#define DEQUEUE_TX_AMD_RAW_SDU_Q(AmdRawSdu_p, AmdRawSduQ_p){\
    popNodeCircQ(AmdRawSduQ_p->pQ,(void *)&(AmdRawSdu_p));\
    AmdRawSdu_p = (RlcUeDataReq *)AmdRawSdu_p;\
}

#define ENQUEUE_AMD_RAW_SDU_Q(AmdRawSduQ_p, AmdRawSdu_p)\
    pushNodeCircQ(AmdRawSduQ_p->pQ,AmdRawSdu_p)

#define QUEUE_COUNT_AMD_RAW_SDU_Q(AmdRawSduQ) \
    circQueueCount((AmdRawSduQ).pQ)


/*AM NACK LIST QUEUE INIT*/

#define QUEUE_INIT_AM_NACK_LIST_Q(AmNackListQ) \
   if ((AmNackListQ.pQ = ((LTE_CIRCQUEUE *)getMemFromPool(sizeof(LTE_CIRCQUEUE),NULL)))) \
    /* 128 UE Memory Optimization Start */\
    circQInitAfterCellSetup( AmNackListQ.pQ,MAX_NODES)
    /* 128 UE Memory Optimization End */

#define QUEUE_COUNT_AM_NACK_LIST_Q(AmNackListQ) \
    circQueueCount((AmNackListQ).pQ)

#define ENQUEUE_AMD_NACK_LIST_Q(AmNackListQ, AmNackList_p)\
    pushNodeCircQ(AmNackListQ.pQ,AmNackList_p)

#define DEQUEUE_AMD_NACK_LIST_Q(AmNackList_p, AmNackListQ_p){\
    popNodeCircQ(AmNackListQ_p->pQ,(void *)&(AmNackList_p));\
    AmNackList_p = (AmNackListNode*)AmNackList_p;\
}
#define QUEUE_DE_INIT_TX_AM_NACK_LIST(AmNackListQ){\
     EMPTY_TX_AM_NACK_LIST(AmNackListQ);\
     circQDeInit(AmNackListQ.pQ);\
     freeMemPool(AmNackListQ.pQ);\
     AmNackListQ.pQ = RLC_NULL;\
}


#define EMPTY_TX_AM_NACK_LIST(AmNackListQ) {\
    AmNackListNode* amNackList_p = RLC_NULL;\
    DEQUEUE_AMD_NACK_LIST_Q(amNackList_p,(&AmNackListQ));\
    while(amNackList_p != RLC_NULL ){\
        freeMemPool(amNackList_p);\
        amNackList_p = RLC_NULL;\
        DEQUEUE_AMD_NACK_LIST_Q(amNackList_p,(&AmNackListQ));\
    }\
}


#define CREATE_AM_NACK_LIST_NODE(amNackList_p,nackSn,Sostart,Soend){\
    if((amNackList_p = (AmNackListNode *)\
                getMemFromPool(sizeof(AmNackListNode),RLC_NULL)) == RLC_NULL){\
        LOG_RLC_MSG(LTE_RLC_GET_MEM_POOL_FAILED, LOGERROR, L2_SYS_FAIL,\
                globalTTITickCount_g, __LINE__, sizeof(AmNackListNode), 0,0,0,\
                0,0,__func__,__FILE__);\
    }\
    else{\
        amNackList_p->seqNum = nackSn;\
        amNackList_p->Sostart = Sostart;\
        amNackList_p->Soend = Soend;\
    }\
}

/*AM NACK ARRAY APPROCH*/


/*AM SDU TID QUEUE INIT*/

#define QUEUE_INIT_AM_SDU_TID_Q(SduTidInfoQ) \
   if ((SduTidInfoQ.pQ = ((LTE_CIRCQUEUE *)getMemFromPool(sizeof(LTE_CIRCQUEUE),NULL)))) \
    /* 128 UE Memory Optimization Start */\
    circQInitAfterCellSetup( SduTidInfoQ.pQ, SDU_TID_Q_SIZE)
    /* 128 UE Memory Optimization End */

#define QUEUE_COUNT_AM_SDU_TID_Q(SduTidInfoQ) \
    circQueueCount((SduTidInfoQ).pQ)

#define ENQUEUE_AM_SDU_TID_Q(SduTidInfoQ_p, SduTidInfo_p)\
        pushNodeCircQ(SduTidInfoQ_p->pQ,SduTidInfo_p)    

#define DEQUEUE_AMD_SDU_TID_Q(SduTidInfo_p, SduTidInfoQ_p){\
    popNodeCircQ(SduTidInfoQ_p->pQ,(void *)&(SduTidInfo_p));\
    SduTidInfo_p = (SduTidInfo *)SduTidInfo_p;\
}

#define QUEUE_DE_INIT_TX_AM_SDU_TID_Q(SduTidInfoQ){\
     EMPTY_TX_AM_SDU_TID_Q(SduTidInfoQ);\
     circQDeInit(SduTidInfoQ.pQ);\
     freeMemPool(SduTidInfoQ.pQ);\
     SduTidInfoQ.pQ = RLC_NULL;\
}
 

#define EMPTY_TX_AM_SDU_TID_Q(SduTidInfoQ) {\
    SduTidInfo* SduTidInfo_p = RLC_NULL;\
    DEQUEUE_AMD_SDU_TID_Q(SduTidInfo_p,(&SduTidInfoQ));\
    while(SduTidInfo_p != RLC_NULL ){\
        freeMemPool(SduTidInfo_p);\
        SduTidInfo_p = RLC_NULL;\
        DEQUEUE_AMD_SDU_TID_Q(SduTidInfo_p,(&SduTidInfoQ));\
    }\
}

/** RLC Rel 3.0 Changes Start **/
#define EMPTY_RLC_CONTROL_PDU_Q(){\
    RlcControlPduInd * rlcCtrlPduInd_p = RLC_NULL;\
    rlcCtrlPduInd_p = DEQUEUE_RLC_CONTROL_PDU_Q();\
    while(rlcCtrlPduInd_p != RLC_NULL){\
	freeMemPool(rlcCtrlPduInd_p);\
	rlcCtrlPduInd_p = RLC_NULL;\
	rlcCtrlPduInd_p = DEQUEUE_RLC_CONTROL_PDU_Q();\
    }\
} 
/** RLC Rel 3.0 Changes End **/

#define QUEUE_DE_INIT_AMD_RAW_SDU_Q(AmdRawSduQ) {\
    EMPTY_TX_AMD_RAW_SDU_Q(AmdRawSduQ);\
    circQDeInit( AmdRawSduQ.pQ);\
    freeMemPool(AmdRawSduQ.pQ);\
    AmdRawSduQ.pQ = RLC_NULL;\
}


#define EMPTY_TX_AMD_RAW_SDU_Q(AmdRawSduQ) {\
    RlcUeDataReq* sdu_p = RLC_NULL;\
    DEQUEUE_TX_AMD_RAW_SDU_Q(sdu_p,(&AmdRawSduQ));\
    while(sdu_p != RLC_NULL ){\
        msgFree(sdu_p->data_p);\
        sdu_p->data_p = RLC_NULL;\
        freeMemPool(sdu_p);\
        sdu_p = RLC_NULL;\
        DEQUEUE_TX_AMD_RAW_SDU_Q(sdu_p,(&AmdRawSduQ));\
    }\
}

/* Rlc Rel 2.0 Change start*/
/* To empty AMD_RAW_SDU_Q and also set the status buffers */
#define EMPTY_TX_AMD_RAW_SDU_Q_SET_STATUS_BUFFER(AmdRawSduQ ,rlcStatusBuffer ,rlcStatusBufferCount) {\
    RlcUeDataReq* sdu_p = RLC_NULL;\
    UInt16 deliveryStatus = RLC_FAILURE;\
    DEQUEUE_TX_AMD_RAW_SDU_Q(sdu_p,(&AmdRawSduQ));\
    while(sdu_p != RLC_NULL ){\
        FILL_AM_RAW_STATUS_BUFFER(rlcStatusBuffer[rlcStatusBufferCount] ,\
                                                sdu_p ,deliveryStatus);\
        rlcStatusBufferCount++;\
        msgFree(sdu_p->data_p);\
        sdu_p->data_p = RLC_NULL;\
        freeMemPool(sdu_p);\
        sdu_p = RLC_NULL;\
        DEQUEUE_TX_AMD_RAW_SDU_Q(sdu_p,(&AmdRawSduQ));\
    }\
}

#define FILL_AM_RAW_STATUS_BUFFER(rlcStatusBuffer ,sdu_p ,deliveryStatus){\
    rlcStatusBuffer.transId =sdu_p->transId;\
    rlcStatusBuffer.deliveryStatus =  deliveryStatus;\
}

#define EMPTY_TX_AM_RETX_Q_SET_STATUS_BUFFER(ReTxQueue ,\
    SduTidInfoQ,ueIndex ,rlcStatusBuffer ,rlcStatusBufferCount){\
    UInt32 Idx = 0;\
    UInt16 snTidCount = 0;\
    AmReTransmission * amReTxPdu_p = RLC_NULL;\
    UInt16 deliveryStatus = RLC_FAILURE;\
    SduTidInfo* SduTidInfo_p = RLC_NULL;\
    for(;Idx < AM_WINDOW_SIZE;Idx++){\
       amReTxPdu_p = ReTxQueue[Idx] ;\
       if(amReTxPdu_p){\
           snTidCount = amReTxPdu_p->dfeInfo.dfeCount;\
           for(;snTidCount ;snTidCount--){\
               DEQUEUE_AMD_SDU_TID_Q(SduTidInfo_p,(&SduTidInfoQ));\
               if(SduTidInfo_p){\
               rlcStatusBuffer[rlcStatusBufferCount].transId  = SduTidInfo_p->tId;\
               rlcStatusBuffer[rlcStatusBufferCount].deliveryStatus =  deliveryStatus;\
               rlcStatusBufferCount++;\
               freeMemPool(SduTidInfo_p);\
               SduTidInfo_p = RLC_NULL;\
               }\
           }\
           FREE_AM_RETX_PDU(amReTxPdu_p);\
           ReTxQueue[Idx] = RLC_NULL;\
       }\
    }\
}

/* Get PDU from ReTxQ with SN as seqNum */
#define GET_IN_PDU_FROM_RETXQ(ReTxQueue, seqNum, amReTxPdu_p){\
    amReTxPdu_p = ReTxQueue[seqNum];\
}

/* Calculate the variable header size*/
#define GET_VARIABLE_HEADER_SIZE(sduCount, vrHeaderSize){\
    /* SPR 18122 Changes Start*/ \
    vrHeaderSize = (DIVIDE_BY_EIGHT((E_BITS + LI_BITS) * (sduCount - 1))) + ((sduCount - 1) & 1);\
    /* SPR 18122 Changes End*/ \
}
/* Rlc Rel 2.0 Change end*/

#define CREATE_AM_SDU_TID_INFO_NODE(sduTidInfo_p){\
    if((sduTidInfo_p = (SduTidInfo *)\
                getMemFromPool(sizeof(SduTidInfo),RLC_NULL)) == RLC_NULL){\
        ltePanic("Fail to allocate the memory");\
    }\
}


#define CREATE_AM_RETX_PDU(amReTxPdu_p){\
    if((amReTxPdu_p = (AmReTransmission *)\
                getMemFromPool(sizeof(AmReTransmission),RLC_NULL)) == RLC_NULL){\
        ltePanic("Fail to allocate the memory");\
    }else {\
        amReTxPdu_p->data_size = 0;\
        amReTxPdu_p->seqNum = 0;\
        amReTxPdu_p->reTxCount = 0xFFFF;\
        amReTxPdu_p->snTidCount = 0;\
        amReTxPdu_p->pdu_data_p = RLC_NULL;\
    }\
}


#define FREE_AM_RETX_PDU(AmReTxPdu_p){\
    if (AmReTxPdu_p->pdu_data_p != RLC_NULL){\
    msgFree(AmReTxPdu_p->pdu_data_p);\
    AmReTxPdu_p->pdu_data_p = RLC_NULL;\
    }\
    freeMemPool(AmReTxPdu_p);\
    AmReTxPdu_p = RLC_NULL;\
}

#define EMPTY_TX_AM_RETX_Q(ReTxQueue) {\
    UInt32 Idx = 0;\
    AmReTransmission * reTxQueue = RLC_NULL;\
    for(;Idx < AM_WINDOW_SIZE;Idx++){\
       reTxQueue = ReTxQueue[Idx] ;\
       if(reTxQueue){ \
           FREE_AM_RETX_PDU(reTxQueue);\
           ReTxQueue[Idx] = RLC_NULL;\
       } \
    }\
}

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct AmdRawSduT { 
    LTE_QUEUE_NODE  qN ;
    UInt32  transId ;
    RlcAmBuffer pRawSdu ;
} AmdRawSdu ;

typedef struct SduTidInfoT {
    /* SPR 2812 changes start */
    void *tId;
    /* SPR 2812 changes end */
    RlcSn seqNum ;
    UInt16 tIdSnCount ;
}SduTidInfo; 

typedef struct AmdRawSduInT {
    RlcSn sn ;
    SduTidInfo* tIDSegSduNode_p;
    RlcAmBuffer rlcAmRawSdu ;
/* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
    /* This will store the size of SDU before segmentation */
    UInt16 unsegSduSize;
    /* + SPR 12783 */
    /* This variable will store the PktArrivalTime for incomplete SDU */
    tickType_t pktArrivalTime;
    /* - SPR 12783 */
#endif
/* KPI Changes End */
} AmdRawSduIn ;

typedef struct AmdRawSduQT {
    LTE_CIRCQUEUE *  pQ ;
    /* SPR 17554 Start */ 
    UInt32 enqueueCount;
    /* SPR 17554 End */ 
} AmdRawSduQ ;

typedef struct SduTidInfoQT {
    LTE_CIRCQUEUE *  pQ ;
} SduTidInfoQ ;

typedef struct AmNackListQT {
    LTE_CIRCQUEUE *  pQ ;
} AmNackListQ ;

typedef struct AmNackListNodeT {
    RlcSn seqNum ;
    UInt16 Sostart ;
    UInt16 Soend ;
}AmNackListNode;

typedef struct DfeInfoT {
    UInt8 fi;
    UInt16 sizeOfDfe[RLC_MAX_DFE_IN_PDU];
    UInt16 dfeCount;
} DfeInfo;

typedef struct AmReTransmissionT {
    void * pdu_data_p ;
    UInt16 data_size;
    UInt16 pdu_size;
    RlcSn seqNum;
    RlcVr reTxCount ;
    UInt16 snTidCount ;
    DfeInfo dfeInfo; /*Header Info for DFEs in this PDU*/
/** Rel 4.0 changes start **/
#ifndef UE_SIM_TESTING
    UInt8  sduCountForPdbKPI; /*sduCount for Pkt Delay KPI not > MAX_RLC_SDU_IN_PDU_FOR_KPI*/
    /* SPR 15909 fix start */
    tickType_t pktArrTime[MAX_RLC_SDU_IN_PDU_FOR_KPI];
    /* SPR 15909 fix end */
#endif
/** Rel 4.0 changes end **/

} AmReTransmission;


typedef struct RlcAmTbInfoT{
   UInt8 tbCounter;
   UInt8 *tb[MAX_TB_COUNT];
}RlcAmTbInfo;

typedef struct AmdReTxPduInT {
    RlcSn sn ;
    UInt16 Sostart;
    UInt16 Soend;
    RlcAmBuffer pRawPdu ;
/** SPR 1203 Start **/
    RlcAmTbInfo pTbInfo; 
    UInt8 fi;
 /** SPR 1203 End **/
} AmdReTxPduIn ;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* AmdRawSdu Memory operations */
UInt32 rlcSearchNdeleteAmdSduFromQ
(
    AmdRawSduQ *q,
    /* SPR 2812 changes start */
    void *transId
    /* SPR 2812 changes end */
);

#endif  /* SDU_H */
