/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmdPdu.h,v 1.1.6.2.2.1 2010/09/21 15:49:26 gur20491 Exp $ 
 *
 ****************************************************************************
 *
 *  File Description : 
 *                  This file declares the Pdu operations of AM Entity
 *
 ****************************************************************************
 *  Revision Details
 * ------------------
 *  
 *  $Log: lteRlcAmdPdu.h,v $
 *  Revision 1.1.6.2.2.1  2010/09/21 15:49:26  gur20491
 *  FAPI changes
 *
 *  Revision 1.1.2.1  2010/08/10 09:37:45  gur19836
 *  Files added for RLC AM Release
 *
 *  Revision 1.1.6.6  2010/08/05 15:51:08  gur19479
 *  updated for resegmentation
 *
 *  Revision 1.1.6.3  2010/07/15 14:42:44  gur19479
 *  updated for status update in reception buffer
 *
 *  Revision 1.1.6.2  2010/07/03 06:03:29  gur19479
 *  updated for resegmentation
 *
 *  Revision 1.1.6.1  2010/06/17 10:59:39  gur19836
 *  RLC AM Base Code Merged
 *
 *  Revision 1.1.2.4  2009/11/10 09:06:24  gur19140
 *  RLC AM updated macro
 *
 *  Revision 1.1.2.3  2009/10/28 09:01:42  gur19140
 *  Minor changes for file headers
 *
 *
 ****************************************************************************/

#ifndef AMDPDU_H
#define AMDPDU_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcGlobalData.h"
#include "lteQueue.h"
#include "lteRlcBuffer.h"
#include "lteTypes.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_SEG_PDU         0x0F
#define QUEUE_INIT_AMDPDUSEGMENT_Q(AmdDFEQ_p){ \
    if((AmdDFEQ_p->pQ = (LTE_QUEUE*)getMemFromPool(sizeof(LTE_QUEUE),RLC_NULL))\
            == RLC_NULL ){ \
        ltePanic("Memory Allocation Failure");\
    }\
    queueInit(AmdDFEQ_p->pQ);\
}
/*TI_TICKOVERSHOOT Start*/
#define CREATE_AMD_PDU(AmdPdu_p){ \
    if((AmdPdu_p = (AmdPdu *)getMemFromPool(sizeof(AmdPdu),RLC_NULL)) == RLC_NULL ){ \
        ltePanic("Memory allocation failure");\
    }\
    else{ \
        AmdPdu_p->p = 0;\
        AmdPdu_p->lsf = 0; \
        AmdPdu_p->status = PDU_AM_COMPLETE; \
        /*AmdPdu_p->pQ = (LTE_QUEUE*)&(AmdPdu_p->pQ);*/ \
        queueInit(&(AmdPdu_p->LQ));\
    }\
}  
/*TI_TICKOVERSHOOT End*/

#define CREATE_AMD_PDU_SEGMENT(amdPduSeg_p){\
    if((amdPduSeg_p = \
                (AmdPduSegment*)getMemFromPool(sizeof(AmdPduSegment),RLC_NULL))\
            == RLC_NULL ){ \
        LOG_RLC_MSG(LTE_RLC_GET_MEM_POOL_FAILED,LOGERROR,L2_SYS_FAIL,\
                        globalTTITickCount_g, __LINE__, sizeof(AmdPduSegment), 0,0,0,\
                                        0,0, __func__, __FILE__);\
    }\
    else{\
        if((amdPduSeg_p->dfeQ.pQ = \
                    (LTE_QUEUE*)getMemFromPool(sizeof(LTE_QUEUE),RLC_NULL))\
                == RLC_NULL ){ \
            freeMemPool(amdPduSeg_p);\
            amdPduSeg_p = RLC_NULL;\
            LOG_RLC_MSG( LTE_RLC_GET_MEM_POOL_FAILED,LOGERROR,L2_SYS_FAIL,\
                    globalTTITickCount_g, __LINE__, sizeof(LTE_QUEUE), 0,0,0,\
                    0,0, __func__, __FILE__);\
        }\
        else{\
            queueInit(amdPduSeg_p->dfeQ.pQ);\
            amdPduSeg_p->soStart = 0;\
            amdPduSeg_p->soEnd = 0xFFFF;\
        }\
    }\
}

#define CREATE_AMD_DFE(AmdDFE_p){ \
    if((AmdDFE_p = (AmdDFE *)getMemFromPool(sizeof(AmdDFE),RLC_NULL)) == RLC_NULL ){ \
        ltePanic("Memory allocation failure");\
    }\
    else {\
        (AmdDFE_p)->status = AM_PDU_MAP_SDU_FULL ; \
        (AmdDFE_p)->pDFE.data_p = RLC_NULL;\
        (AmdDFE_p)->pDFE.size = 0;\
        (AmdDFE_p)->pDFE.RLCRESTBFLAG = 0;\
    }\
}    

#define DEQUEUE_AMD_PDU_DFE_Q(AmdDfeQ,amdDfe_p){ \
    LTE_QUEUE_NODE *pN = RLC_NULL;\
    pN = deQueue(AmdDfeQ.pQ) ;\
    amdDfe_p = MEMBEROF(AmdDFE,qN,pN);\
} 

#define ENQUEUE_AMD_PDU_DFE_Q(AmdDFEQ_p,AmdDFE_p){ \
    enQueue(AmdDFEQ_p->pQ,&AmdDFE_p->qN);\
}    


/*TI_TICKOVERSHOOT Start*/
#define DEQUEUE_AMD_PDU_SEGMENT_Q(AmdDFEQ_p,amdPduSeg_p){ \
    LTE_QUEUE_NODE *pN = RLC_NULL;\
    pN = deQueue(&(AmdDFEQ_p->LQ)) ;\
    amdPduSeg_p = MEMBEROF(AmdPduSegment,qN,pN);\
}   

#define ENQUEUE_AMD_PDU_SEGMENT_Q(AmdDFEQ_p,amdPduSeg_p){ \
    enQueue(&(AmdDFEQ_p->LQ),&amdPduSeg_p->qN);\
}    
/*TI_TICKOVERSHOOT End*/


#define RLC_NOT_IN_USE   0x00
#define RLC_SO_START_ADD 0x01
#define RLC_SO_END_SUB   0x02
/****************************************************************************
 * Global Variables
 ****************************************************************************/
/* Holding SN Status for Reception Buffer */

/*SPR 6363 FIX Start */
extern UInt8 rlcSnStatusArr_g[RLC_MAX_STATUS_BUFFER];
/*SPR 6363 FIX END */

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef enum RlcSduStatusT {
    AM_PDU_MAP_SDU_FULL = 0 ,
    AM_PDU_MAP_SDU_START ,
    AM_PDU_MAP_SDU_MID ,
    AM_PDU_MAP_SDU_END 
} RlcSduStatus ;

typedef enum RlcAmPduStatusT {
    PDU_AM_COMPLETE = 0,
    PDU_AM_SEGMENT,
    PDU_AM_SEGMENT_COMPLETE
} RlcAmPduStatus ; 

typedef enum rlcReptBufferStatusT {
    RLC_STATUS_UNPROCESSED,
    RLC_STATUS_PROCESSED,
    RLC_STATUS_FAILURE
}rlcReptBufferStatus;


typedef enum RlcPduSegmentStatusT {
    PDU_SEGMENT_COMPLETE = 0,
    PDU_SEGMENT_START ,
    PDU_SEGMENT_MID ,
    PDU_SEGMENT_END 
} RlcPduSegmentStatus ; 


typedef struct AmdRawPduT { 
    LTE_QUEUE_NODE  qN ;
    RlcAmBuffer pRawPdu ;
} AmdRawPdu ;

typedef struct AmdDfeArrT {
    UInt8 *payload_p;
    UInt8 fi;
    UInt8 lsf;
    UInt32 nodeSoStart;
    UInt32 nodeSoEnd;
    UInt32 li;
}AmdDfeArr;

typedef struct AmdDfeArrInfoT {
    UInt16 numOfDfe;
    /* + SPR SPR 5498 */
    AmdDfeArr dfeArr[RLC_MAX_DFE_IN_PDU_UL];
    /* - SPR SPR 5498 */
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    tickType_t packetTime;
    /* SPR 15909 fix end */
#endif
    /*32.425 Events code changes end*/
}AmdDfeArrInfo;

typedef struct AmdDFET {
    LTE_QUEUE_NODE  qN ;
    RlcSduStatus status ;
    RlcAmBuffer pDFE ;
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    tickType_t packetTime;
    /* SPR 15909 fix end */
#endif
    /*32.425 Events code changes end*/
} AmdDFE ;

typedef struct AmdDFEQT {
    LTE_QUEUE *  pQ ;
} AmdDFEQ;


typedef struct AmdPduSegmentT {
    LTE_QUEUE_NODE  qN ;
    UInt32 soStart ;
    UInt32 soEnd ;
    /*AmdDFE*/
    AmdDFEQ dfeQ ;
    UInt8 lsf;
} AmdPduSegment ;

typedef struct AmdPduSegmentNodeT {
    LTE_QUEUE_NODE *qN;
    UInt32 soStart ;
    UInt32 soEnd ;
}AmdPduSegmentNode;

typedef struct AmdPduSegmentInfoT {
    UInt8 *payload_p;
    UInt8 *header_p;
    UInt8 soFlag;
    UInt32 soStart;
    UInt32 soEnd;
    UInt16 posToInsert;
}AmdPduSegmentInfo;

typedef struct AmdHeaderT {
    UInt8  dc;
    UInt8  rf;
    UInt8  p;
    UInt8  fi;
    UInt8  e;
    UInt8  lsf;
    RlcSn  sn;
    UInt16 so;
    UInt16 soEnd;
} AmdH;

typedef struct AmdPduT {
    /*Amd header*/
    UInt8  p;
    UInt8  lsf;
    RlcAmPduStatus status ;
/*TI_TICKOVERSHOOT Start*/
    /* AmdPduSegment   */
    LTE_QUEUE LQ;
    //LTE_QUEUE *  pQ ;
    /*AmdDFEQ pduSegmentQ;*/
/*TI_TICKOVERSHOOT End*/
} AmdPdu ;

typedef struct AmdPduInT {
    RlcSn sn ;
    UInt8  *pRawPdu ;
    UInt32 Sostart;
    UInt32 Soend;
} AmdPduIn ;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/


#endif  /* AMDPDU_H */
