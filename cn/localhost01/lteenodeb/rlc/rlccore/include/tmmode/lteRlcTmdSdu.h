/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcTmdSdu.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:53 gur20491 Exp $
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
 * $Log: lteRlcTmdSdu.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:53  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:34  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.2.28.1  2010/07/17 11:37:41  gur23054
 * Review Comments fixed
 *
 * Revision 1.2  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.1  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef TM_SDU_H
#define TM_SDU_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcGlobalData.h"
#include "lteRlcTypes.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define QINIT_TMD_RAW_SDU_Q(TmdRawSduQ) {\
    if((TmdRawSduQ.pQ = (LTE_QUEUE*)getMemFromPool(sizeof(LTE_QUEUE),RLC_NULL))\
                           == RLC_NULL ){ \
        ltePanic("Memory Allocation Failure");\
    }\
    queueInit(TmdRawSduQ.pQ);\
}

#define QDEINIT_TMD_RAW_SDU_Q(TmdRawSduQ) {\
    EMPTY_TMD_RAW_SDU_Q(TmdRawSduQ);\
    freeMemPool(TmdRawSduQ.pQ);\
}

#define EMPTY_TMD_RAW_SDU_Q(TmdRawSduQ) {\
    TmdRawSdu* sdu_p = RLC_NULL;\
    DEQUEUE_TMD_SDU_Q(sdu_p,(&TmdRawSduQ));\
    while(sdu_p != RLC_NULL ){\
        FREE_TMD_RAW_SDU(sdu_p);\
        DEQUEUE_TMD_SDU_Q(sdu_p,(&TmdRawSduQ));\
    }\
}

#define QUEUE_COUNT_TMD_RAW_SDU_Q(TmdRawSduQ) \
    queueCount(TmdRawSduQ.pQ);

#define GET_QUEUE_NODE_TMD_SDU_Q(TmdRawSdu_p, TmdRawSduQ_p){\
    LTE_QUEUE_NODE *pN = RLC_NULL;\
    pN = getQueueNode(TmdRawSduQ_p->pQ);\
    TmdRawSdu_p = MEMBEROF(TmdRawSdu,qN,pN);\
}        

#define SET_NEXT_TMD_SDU_Q(TmdRawSdu_p){\
    LTE_QUEUE_NODE *next = getNextQueueNode(&TmdRawSdu_p->qN);\
    TmdRawSdu_p =  MEMBEROF(TmdRawSdu,qN,next);\
}

#define SET_PREV_TMD_SDU_Q(TmdRawSdu_p){\
    LTE_QUEUE_NODE *previous = getPrevQueueNode(&TmdRawSdu_p->qN)\
    TmdRawSdu_p =  MEMBEROF(TmdRawSdu,qN,previous);\
}

#define ENQUEUE_TMD_SDU_Q(TmdRawSduQ_p, TmdRawSdu_p)\
    enQueue(TmdRawSduQ_p->pQ,&TmdRawSdu_p->qN);    

#define DEQUEUE_TMD_SDU_Q(TmdRawSdu_p, TmdRawSduQ_p){\
    LTE_QUEUE_NODE *pN = RLC_NULL;\
    pN = deQueue(TmdRawSduQ_p->pQ);\
    TmdRawSdu_p = MEMBEROF(TmdRawSdu,qN,pN);\
}

#define FREE_TMD_RAW_SDU(TmdRawSdu_p){\
    rlcFreeBuffer(TmdRawSdu_p->pRawSdu);\
    freeMemPool(TmdRawSdu_p);\
}

#define FREE_TMD_SDU_BUFFER(TmdRawSdu_p){\
    if(TmdRawSdu_p && TmdRawSdu_p->pRawSdu){    \
        freeMemPool(TmdRawSdu_p->pRawSdu);\
    }  \
    if(TmdRawSdu_p ){ \
        freeMemPool(TmdRawSdu_p);\
    }\
}

#define CREATE_TMD_SDU_BUFFER(TmdRawSdu_p){\
    if((TmdRawSdu_p = (TmdRawSdu *)getMemFromPool(sizeof(TmdRawSdu),RLC_NULL)) == RLC_NULL){\
        ltePanic("Fail to allocate the memory");\
    }\
    else{\
        if((TmdRawSdu_p->pRawSdu= \
                    (RlcBuffer *)getMemFromPool(sizeof(RlcBuffer),RLC_NULL))==RLC_NULL){\
            ltePanic("Memory Allocation Failure");\
        } \
        TmdRawSdu_p->pRawSdu->data_p = RLC_NULL;\
        TmdRawSdu_p->pRawSdu->copyType = RLC_ZERO_COPY_BUFFER ;\
        TmdRawSdu_p->pRawSdu->size = 0 ; \
        TmdRawSdu_p->pRawSdu->szUsed = 0 ;\
    }\
}




#define GET_SIZE_TMD_RAW_SDU(TmdRawSdu_p)\
    GET_BUFF_SIZE(TmdRawSdu_p->pRawSdu);

#define CREATE_TMD_RAW_SDU(TmdRawSdu_p, ui32_size){\
    if((TmdRawSdu_p = (TmdRawSdu *)getMemFromPool(sizeof(TmdRawSdu),RLC_NULL)) == RLC_NULL){\
        ltePanic("Fail to allocate the memory");\
    }\
    else{\
        TmdRawSdu_p->pRawSdu = rlcCreateBuffer(ui32_size);\
    }\
}


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct TmdRawSduT { 
    LTE_QUEUE_NODE  qN ;
    RlcBuffer *pRawSdu ;
    UInt16 rnti ;
} TmdRawSdu ;

typedef struct TmdRawSduQT {
    LTE_QUEUE *  pQ ;
} TmdRawSduQ ;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* TmdRawSdu Memory operations */
TmdRawSdu *rlcCreateTmdRawSdu(UInt32 size);

#endif  /* SDU_H */
