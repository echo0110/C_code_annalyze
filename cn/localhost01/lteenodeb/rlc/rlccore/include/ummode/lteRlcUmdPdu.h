/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmdPdu.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:59 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcUmdPdu.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:59  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:37  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.7.2.2  2010/07/17 11:40:55  gur23054
 * Review Comments fixed
 *
 * Revision 1.7.2.1  2010/06/17 10:59:49  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.7  2010/06/14 12:00:57  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.6  2009/04/15 16:46:58  gur19140
 * checking NULL
 *
 * Revision 1.5  2009/04/15 16:04:22  gur19140
 * rlc latency + logging related changes
 *
 * Revision 1.4  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.3  2009/03/06 12:44:42  gur19140
 * Review comments fixes
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

#ifndef UMDPDU_H
#define UMDPDU_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcGlobalData.h"
#include "lteQueue.h"
#include "lteRlcBuffer.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define SET_SN_FIELD_LEN_UMDH(UmdH_p, en_snLen) \
     UmdH_p->snLen = ui_snLen;
     
#define GET_SN_FIELD_LEN_UMDH(UmdH_p) \
    UmdH_p->snLen;
    
#define SET_SN_UMDH(UmdH_p, ui_sn) \
    UmdH_p->sn = ui_sn;

#define GET_SN_UMDH(UmdH_p) \
    UmdH_p->sn;
 
#define SET_SZ_UMDH(UmdH_p, ui_sz) \
    UmdH_p->sz = ui_sz;
  
#define GET_SZ_UMDH(UmdH_p) \
    UmdH_p->sz;
   
#define SET_STATUS_UMDDFE(UmdDFE_p, en_status) \
    UmdDFE_p->status = en_status;
   
#define GET_STATUS_UMDDFE(UmdDFE_p) \
    UmdDFE_p->status;
   
#define QUEUE_INIT_UMDDFE_Q(UmdDFEQ_p){ \
    if((UmdDFEQ_p->pQ = (LTE_QUEUE*)getMemFromPool(sizeof(LTE_QUEUE),RLC_NULL))\
                            == RLC_NULL ){ \
        ltePanic("Memory Allocation Failure");\
    }\
    queueInit(UmdDFEQ_p->pQ);\
}


#define QUEUE_DE_INIT_UMDDFE_Q(UmdDFEQ_p){ \
    EMPTY_UMDDFE_Q(UmdDFEQ_p);\
    /* CSR 51542 fix start */\
    semDestroy((LTE_SEM *)&UmdDFEQ_p->pQ->sem_lock);\
    /* CSR 51542 fix end */\
    freeMemPool(UmdDFEQ_p->pQ);\
}

#define EMPTY_UMDDFE_Q(UmdDFEQ_p){ \
    UmdDFE *dfe = RLC_NULL ;\
    DE_QUEUE_UMDDFE_Q(UmdDFEQ_p,dfe);\
    while(dfe != RLC_NULL ){\
        FREE_UMD_DFE(dfe);\
        DE_QUEUE_UMDDFE_Q(UmdDFEQ_p,dfe);\
    }\
}     

#define QUEUE_COUNT_UMDDFE_Q(UmdDFEQ_p) \
    queueCount(UmdDFEQ_p->pQ);\

#define GET_QUEUE_NODE_UMDDFE_Q(UmdDFE_p, UmdDFEQ_p){ \
    LTE_QUEUE_NODE *pN = RLC_NULL;\
    pN = getQueueNode(UmdDFEQ_p->pQ);\
    UmdDFE_p = MEMBEROF(UmdDFE,qN,pN);\
}
    
#define EN_QUEUE_UMDDFE_Q(UmdDFEQ_p,UmdDFE_p){ \
    enQueue(UmdDFEQ_p->pQ,&UmdDFE_p->qN);\
}    

#define DE_QUEUE_UMDDFE_Q(UmdDFEQ_p,UmdDFE_p){ \
     LTE_QUEUE_NODE *pN = RLC_NULL;\
     pN = deQueue(UmdDFEQ_p->pQ) ;\
     UmdDFE_p = MEMBEROF(UmdDFE,qN,pN);\
}     

#define QUEUE_INIT_UMD_RAW_PDU_Q(UmdRawPduQ_p){ \
    if((UmdRawPduQ_p->pQ = (LTE_QUEUE*)getMemFromPool(sizeof(LTE_QUEUE),RLC_NULL))\
                                 == RLC_NULL){ \
        ltePanic("Memory Allocation Failure");\
    }\
    queueInit(UmdRawPduQ_p->pQ);\
}       

#define QUEUE_DE_INIT_UMD_RAW_PDU_Q(UmdRawPduQ_p){ \
    EMPTY_UMD_RAW_PDU_Q(UmdRawPduQ_p);\
    /* CSR 51542 fix start */\
    semDestroy((LTE_SEM *)&UmdRawPduQ_p->pQ->sem_lock);\
    /* CSR 51542 fix end */\
    freeMemPool(UmdRawPduQ_p->pQ);\
}

#define EMPTY_UMD_RAW_PDU_Q(UmdRawPduQ_p){ \
    UmdRawPdu *pdu = RLC_NULL ;\
    DE_QUEUE_UMD_RAW_PDU_Q(UmdRawPduQ_p,pdu);\
    while(pdu != RLC_NULL ){\
        FREE_UMD_RAW_PDU(pdu);\
        DE_QUEUE_UMD_RAW_PDU_Q(UmdRawPduQ_p,pdu);\
    }\
}

#define QUEUE_COUNT_UMD_RAW_PDU_Q(UmdRawPduQ_p) \
    queueCount(UmdRawPduQ_p->pQ);

#define GET_QUEUE_NODE_UMD_RAW_PDU_Q(UmdRawPduQ_p, UmdRawPdu_p){ \
    LTE_QUEUE_NODE *pN = RLC_NULL ;\
    pN = getQueueNode(UmdRawPduQ_p->pQ) ;\
    UmdRawPdu_p = MEMBEROF(UmdRawPdu,qN,pN);\
}    

#define EN_QUEUE_UMD_RAW_PDU_Q(UmdRawPduQ_p, UmdRawPdu_p){ \
    enQueue(UmdRawPduQ_p->pQ,&UmdRawPdu_p->qN);\
}   

#define DE_QUEUE_UMD_RAW_PDU_Q(UmdRawPduQ_p, UmdRawPdu_p){ \
    LTE_QUEUE_NODE *pN = RLC_NULL ;\
    pN = deQueue(UmdRawPduQ_p->pQ) ;\
    UmdRawPdu_p = MEMBEROF(UmdRawPdu,qN,pN);\
}

#define CREATE_UMD_RAW_PDU(UmdRawPdu_p, ui32_size){ \
    if((UmdRawPdu_p = (UmdRawPdu *)getMemFromPool(sizeof(UmdRawPdu),RLC_NULL)) == RLC_NULL ){ \
        ltePanic("Memory Allocation Failure");\
    }\
    else{ \
        (UmdRawPdu_p)->pRawPdu = rlcCreateBuffer(ui32_size);\
    }\
}

#define CREATE_UMD_PDU_BUFFER(UmdRawPdu_p){ \
    if((UmdRawPdu_p = (UmdRawPdu *)getMemFromPool(sizeof(UmdRawPdu),RLC_NULL)) == RLC_NULL ){ \
        ltePanic("Memory Allocation Failure");\
    }\
    if(( (UmdRawPdu_p)->pRawPdu=(RlcBuffer *)getMemFromPool(sizeof(RlcBuffer) ,RLC_NULL)) == RLC_NULL){ \
        ltePanic("Memory Allocation Failure"); \
    } \
    (UmdRawPdu_p)->pRawPdu->data_p = RLC_NULL; \
    (UmdRawPdu_p)->pRawPdu->copyType = RLC_ZERO_COPY_BUFFER ; \
    (UmdRawPdu_p)->pRawPdu->size = 0 ; \
    (UmdRawPdu_p)->pRawPdu->szUsed = 0 ;\
}

#define FREE_UMD_PDU_BUFFER(UmdRawPdu_p){\
    if(UmdRawPdu_p && UmdRawPdu_p->pRawPdu){    \
        freeMemPool(UmdRawPdu_p->pRawPdu);\
    }  \
    if(UmdRawPdu_p ){ \
        freeMemPool(UmdRawPdu_p);\
    }\
}

#define FREE_UMD_RAW_PDU(UmdRawPdu_p){ \
    if(UmdRawPdu_p ){ \
        rlcFreeBuffer(UmdRawPdu_p->pRawPdu);\
        freeMemPool(UmdRawPdu_p);\
    }\
}    

#define GET_SIZE_UMD_RAW_PDU(UmdRawPdu_p) \
    GET_BUFF_SIZE(&(UmdRawPdu_p->pRawPdu));

#define CREATE_UMD_DFE(UmdDFE_p, ui32_size){ \
    if(( UmdDFE_p = (UmdDFE *)getMemFromPool(sizeof(UmdDFE),RLC_NULL)) == RLC_NULL ){ \
        ltePanic("Memory allocation failure");\
    }\
    else {\
    rlcSetBuffer(ui32_size, (void *)&((UmdDFE_p)->pDFE));\
    (UmdDFE_p)->status = PDU_MAP_SDU_FULL ; \
    }\
}    

#define FREE_UMD_DFE(UmdDFE_p){ \
    msgFree(UmdDFE_p->pDFE.data_p) ;\
    /* SPR 20985 fix start */ \
    UmdDFE_p->pDFE.data_p = RLC_NULL; \
    /* SPR 20985 fix end */ \
    freeMemPool(UmdDFE_p);\
} 

#define GET_SIZE_UMDDFE(UmdDFE_p){ \
    GET_BUFF_SIZE(UmdDFE_p->pDFE)\
}    

#define CREATE_UMD_PDU(UmdPdu_p, ui32_size){ \
    if((UmdPdu_p = (UmdPdu *)getMemFromPool(sizeof(UmdPdu),RLC_NULL)) == RLC_NULL ){ \
    ltePanic("Memory allocation failure");\
    }\
    else{ \
    UmdPdu_p->h.sz = ui32_size;\
    QUEUE_INIT_UMDDFE_Q((&UmdPdu_p->df));\
    }\
}    

#define FREE_UMD_PDU(UmdPdu_p){ \
    QUEUE_DE_INIT_UMDDFE_Q((&UmdPdu_p->df));\
    freeMemPool(UmdPdu_p);\
}    
/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef enum RlcPduStatusT {
    PDU_MAP_SDU_FULL = 0 ,
    PDU_MAP_SDU_START ,
    PDU_MAP_SDU_MID ,
    PDU_MAP_SDU_END 
} RlcPduStatus ;

typedef struct UmdRawPduT { 
    LTE_QUEUE_NODE  qN ;
    RlcBuffer pRawPdu ;
} UmdRawPdu ;

typedef struct UmdRawPduQT {
    LTE_QUEUE *  pQ ;
} UmdRawPduQ ;

typedef struct UmdHeaderT {
    SnFieldLength snLen ;
    RlcPduSz sz ;
    RlcSn sn ;
    RlcFi fi ;
    RlcExt e; 	
} UmdH;

typedef struct UmdDataFieldElementT {
    LTE_QUEUE_NODE  qN ;
    RlcPduStatus status ;
    RlcBuffer pDFE ;
#ifndef UE_SIM_TESTING    
    /* SPR 15909 fix start */
    tickType_t pktTimeStamp;
    /* SPR 15909 fix end */
    UInt8 spsState;
#endif
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    tickType_t packetTime;
    /* SPR 15909 fix end */
#endif
    /*32.425 Events code changes end*/
} UmdDFE ;

typedef struct UmdDataFieldElementQT {
    LTE_QUEUE *  pQ ;
} UmdDFEQ;

typedef struct UmdPduT {
    /*Umd header*/
    UmdH h ;
    /* Data Field elements list   */
    UmdDFEQ df;
} UmdPdu ;
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/


#endif  /* UMDPDU_H */
