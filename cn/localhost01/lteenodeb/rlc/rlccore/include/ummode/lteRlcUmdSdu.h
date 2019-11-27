/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmdSdu.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:01 gur20491 Exp $
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
 * $Log: lteRlcUmdSdu.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:01  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:37  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.7.2.1  2010/07/17 11:41:00  gur23054
 * Review Comments fixed
 *
 * Revision 1.7  2010/06/14 12:00:57  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.6  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.5  2009/03/06 12:44:42  gur19140
 * Review comments fixes
 *
 * Revision 1.4  2009/03/02 09:10:54  gur19140
 * Changes reqarding Zero copy,pdcp discard and execution legs.
 *
 * Revision 1.3  2009/02/12 16:45:41  gur20294
 * update for making correct some common channel entity configuration related actions
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

#ifndef SDU_H
#define SDU_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcGlobalData.h"
#include "lteRlcTypes.h"
/* +COVERITY 10220 */
//#include "lteRlcPdcpInterface.h"
/* -COVERITY 10220 */
#include "lteCircQueue.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*+ SPR_17858_532_CHANGES*/
/*+ SPR_17858_749_CHANGES*/
#define QUEUE_DE_INIT_UMD_RAW_SDU_Q(UmdRawSduQ) {\
    EMPTY_TX_UMD_RAW_SDU_Q(UmdRawSduQ);\
    circQDeInit(UmdRawSduQ.pQ);\
    freeMemPool(UmdRawSduQ.pQ);\
    UmdRawSduQ.pQ = RLC_NULL;\
}
    /*- SPR_17858_749_CHANGES*/
    /*- SPR_17858_532_CHANGES*/

/*for circular queue implementation */
/* + SPR 19808 */
/* The size of UM SDU Queue is taken as 1024 to achieve the full throughput
 * of 150 Mbps in DL for 2X2 MIMO for single UE */
#ifndef UE_SIM_TESTING
#define MAX_UM_RAW_SDU_Q_SIZE 1024
#else
/*SPR21756 Changes Start*/
#define MAX_UM_RAW_SDU_Q_SIZE 4096
/*SPR21756 Changes End*/
#endif
/* - SPR 19808 */

#define QUEUE_INIT_UMD_RAW_SDU_Q(UmdRawSduQ) \
   if ((UmdRawSduQ.pQ = (LTE_CIRCQUEUE *)getMemFromPool(sizeof(LTE_CIRCQUEUE),NULL))) \
   /* + SPR 19808 */ \
   circQInitAfterCellSetup(UmdRawSduQ.pQ, MAX_UM_RAW_SDU_Q_SIZE)
/* - SPR 19808 */

#define DE_QUEUE_UMD_SDU_Q(UmdRawSdu_p, UmdRawSduQ_p){\
    popNodeCircQ(UmdRawSduQ_p->pQ,(void *)&(UmdRawSdu_p));\
    UmdRawSdu_p = (UmdRawSdu*)UmdRawSdu_p;\
}

#define QUEUE_COUNT_UMD_RAW_SDU_Q(UmdRawSduQ) \
    circQueueCount((UmdRawSduQ).pQ)

#define EN_QUEUE_UMD_SDU_Q(UmdRawSduQ_p,UmdRawSdu_p)\
    pushNodeCircQ(UmdRawSduQ_p->pQ,UmdRawSdu_p)

#define DE_QUEUE_TX_UMD_SDU_Q(UmdRawSdu_p, UmdRawSduQ_p){\
    popNodeCircQ((LTE_CIRCQUEUE *)UmdRawSduQ_p->pQ, (void *)&(UmdRawSdu_p));\
    UmdRawSdu_p = (RlcUeDataReq *)UmdRawSdu_p;\
}



#define FREE_UMD_RAW_SDU(UmdRawSdu_p){\
    freeMemPool(UmdRawSdu_p);\
}

#define EMPTY_TX_UMD_RAW_SDU_Q(UmdRawSduQ) {\
    RlcUeDataReq* sdu_p = RLC_NULL;\
    DE_QUEUE_TX_UMD_SDU_Q(sdu_p,(&UmdRawSduQ));\
    while(sdu_p != RLC_NULL ){\
        msgFree(sdu_p->data_p);\
        sdu_p->data_p = PNULL;\
        freeMemPool(sdu_p);\
        sdu_p = PNULL;\
        DE_QUEUE_TX_UMD_SDU_Q(sdu_p,(&UmdRawSduQ));\
    }\
}

/* SPR 4497 Changes Start */
#define EMPTY_TX_UMD_RAW_SDU_Q_SET_STATUS_BUFFER(UmdRawSduQ ,rlcStatusBuffer ,rlcStatusBufferCount){\
    RlcUeDataReq* sdu_p = RLC_NULL;\
    UInt16 deliveryStatus = RLC_FAILURE;\
    DE_QUEUE_TX_UMD_SDU_Q(sdu_p,(&UmdRawSduQ));\
    while(sdu_p != RLC_NULL ){\
        FILL_UM_RAW_STATUS_BUFFER(rlcStatusBuffer[rlcStatusBufferCount] ,\
                                                sdu_p ,deliveryStatus);\
        rlcStatusBufferCount++;\
        msgFree(sdu_p->data_p);\
        sdu_p->data_p = PNULL;\
        freeMemPool(sdu_p);\
        sdu_p = PNULL;\
        DE_QUEUE_TX_UMD_SDU_Q(sdu_p,(&UmdRawSduQ));\
    }\
}

#define FILL_UM_RAW_STATUS_BUFFER(rlcStatusBuffer ,sdu_p ,deliveryStatus){\
    rlcStatusBuffer.transId =sdu_p->transId;\
    rlcStatusBuffer.deliveryStatus =  deliveryStatus;\
}
/* SPR 4497 Changes End */

#ifdef LTE_EMBMS_SUPPORTED
/*SSI FIX*/
#define EMPTY_EMBMS_RAW_SDU_Q(UmdRawSduQ) {\
    RlcEmbmsAreaDataReq * sdu_p = RLC_NULL;\
    DE_QUEUE_EMBMS_TX_UMD_SDU_Q(sdu_p,(&UmdRawSduQ));\
    while(sdu_p != RLC_NULL ){\
	if (sdu_p->data_p){\
        msgFree(sdu_p->data_p);\
        sdu_p->data_p = PNULL;}\
        freeMemPool(sdu_p);\
        sdu_p = PNULL;\
        DE_QUEUE_EMBMS_TX_UMD_SDU_Q(sdu_p,(&UmdRawSduQ));\
    }\
    REINIT_EMBMS_RAW_SDU_Q(UmdRawSduQ)\
}
#endif

#define EMPTY_UMD_RAW_SDU_Q(UmdRawSduQ) {\
    UmdRawSdu* sdu_p = RLC_NULL;\
    DE_QUEUE_UMD_SDU_Q(sdu_p,(&UmdRawSduQ));\
    while(sdu_p != RLC_NULL ){\
	if (sdu_p->pRawSdu.data_p){\
        msgFree(sdu_p->pRawSdu.data_p);\
        sdu_p->pRawSdu.data_p = PNULL;}\
        freeMemPool(sdu_p);\
        sdu_p = PNULL;\
        DE_QUEUE_UMD_SDU_Q(sdu_p,(&UmdRawSduQ));\
    }\
}
/*----for TM mode start-----*/
#define EMPTY_TX_TMD_RAW_SDU_Q(TmdRawSduQ) {\
    RlcUeDataReq* sdu_p = RLC_NULL;\
    DE_QUEUE_TX_TMD_SDU_Q(sdu_p,(&TmdRawSduQ));\
    while(sdu_p != RLC_NULL ){\
        msgFree(sdu_p->data_p);\
        sdu_p->data_p = PNULL;\
        freeMemPool(sdu_p);\
        sdu_p = PNULL;\
        DE_QUEUE_TX_TMD_SDU_Q(sdu_p,(&TmdRawSduQ));\
    }\
}
#define DE_QUEUE_TX_TMD_SDU_Q(TmdRawSdu_p, TmdRawSduQ_p){\
    TmdRawSdu_p = (RlcUeDataReq *)deQueue(TmdRawSduQ_p->pQ);\
}
/*---TM mode end-------*/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct UmdRawSduT { 
    /*SPR 4497 Changes Start */
    void*  transId ;
    /*SPR 4497 Changes End */
    UInt8 transactionIdPresent ;
    RlcBuffer pRawSdu ;
} UmdRawSdu ;

typedef struct UmdRawSduInT {
    /*SPR 4497 Changes Start */
    void*  transId ;
    /*SPR 4497 Changes End */
    RlcSn sn ;
    RlcBuffer pRawSdu ;
/* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
    /* This will store the size of SDU before segmentation */
    UInt16 unsegSduSize;
#endif
/* KPI Changes End */
} UmdRawSduIn ;

typedef struct UmdRawSduQT {
    LTE_CIRCQUEUE *  pQ ;
} UmdRawSduQ ;



/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* UmdRawSdu Memory operations */

UInt32 rlcSearchNdeleteUmdSduFromQ(UmdRawSduQ *q, 
        /* SPR 2812 changes start */
        void *transId);
        /* SPR 2812 changes end */

#endif  /* SDU_H */
