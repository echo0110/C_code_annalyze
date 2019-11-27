/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmdArqOperation.h,v 1.1.6.2.2.1 2010/09/21 15:49:23 gur20491 Exp $ 
 *
 ****************************************************************************
 *
 *  File Description : 
 *                  This file declares the Control Operations for AM entity
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *  $Log: lteRlcAmdArqOperation.h,v $
 *  Revision 1.1.6.2.2.1  2010/09/21 15:49:23  gur20491
 *  FAPI changes
 *
 *  Revision 1.1.2.1  2010/08/10 12:12:35  gur19836
 *  RLC Rel 2.0 Code Merged
 *
 *  Revision 1.1.6.7  2010/08/08 14:21:18  gur19479
 *  updated for status pdu
 *
 *  Revision 1.1.6.6  2010/08/04 04:07:17  gur23054
 *  Bug Fixes
 *
 *  Revision 1.1.6.5  2010/07/17 11:39:59  gur23054
 *  Review Comments fixed
 *
 *  Revision 1.1.6.4  2010/07/03 06:04:10  gur19479
 *  updated for resegmentation
 *
 *  Revision 1.1.6.3  2010/06/28 10:22:40  gur23054
 *  rlcActionOnNackReceiveStatusReport-argument added function
 *
 *  Revision 1.1.6.2  2010/06/28 09:17:51  gur23054
 *  macro added
 *
 *  Revision 1.1.6.1  2010/06/17 10:59:38  gur19836
 *  RLC AM Base Code Merged
 *
 *  Revision 1.1.2.7  2009/11/17 11:49:43  gur19140
 *  Review Comments incorporation
 *
 *  Revision 1.1.2.6  2009/11/13 15:00:18  gur19140
 *  Improve logging and self review different function
 *
 *  Revision 1.1.2.5  2009/11/10 09:04:28  gur19140
 *  Status Encoding related changes
 *
 *  Revision 1.1.2.4  2009/10/28 09:01:42  gur19140
 *  Minor changes for file headers
 *
 *
 *
 ****************************************************************************/
#ifndef AMDARQOPERATION_H
#define AMDARQOPERATION_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcTypes.h"
#include "lteRlcAmdPdu.h"
#include "lteRlcAmdSdu.h"
#include "lteLayer2MacRlcInterface.h"
#include "lteRlcAmTxOperation.h"
/* +COVERITY 10217/10218 */
/** SPR 1753 Changes Start **/
//#include "lteRlcAmRxOperation.h"
/** SPR 1753 Changes End **/
/* -COVERITY 10217/10218 */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
extern LTE_SQUEUE rlcControlPduQ_g ;
/** SPR 1753 Changes Start **/
extern LTE_SQUEUE amdRawPduIndQ_g; 
/** SPR 1753 Changes End **/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#define STATUS_PDU_DC_BIT   0x00
#define STATUS_PDU_CPT_BIT  0x00
#define RLC_AM_CPT_MASK     0x70
#define BIT_MASK            0x01
#define BYTE_MASK           0xFF
#define RLC_BYTE            0x08
#define SO_END_SPC_MASK     0x7FFF

#define RLC_TWOBYTE         0x10

#define QINIT_RLC_CONTROL_PDU_Q()\
    sQueueInit(&rlcControlPduQ_g)

#define QDEINIT_RLC_CONTROL_PDU_Q(){\
    RlcControlPduInd *snode = RLC_NULL;\
    while((snode = (RlcControlPduInd *)popNode(&rlcControlPduQ_g)) != RLC_NULL) \
    freeMemPool(snode);\
}

#define QCOUNT_RLC_CONTROL_PDU_Q()\
    sQueueCount(&rlcControlPduQ_g)

#define QGETFIRST_RLC_CONTROL_PDU_Q()\
    getFirstNode(&rlcControlPduQ_g)

#define QGETNEXT_RLC_CONTROL_PDU_Q()\
    getNextNode(&rlcControlPduQ_g)

#define DEQUEUE_RLC_CONTROL_PDU_Q()\
    (RlcControlPduInd*)popNode(&rlcControlPduQ_g)

#define ENQUEUE_RLC_CONTROL_PDU_Q(rlcControlPdu_p)\
    pushNode(&rlcControlPduQ_g,&(rlcControlPdu_p)->sNode)

#define RLC_AM_INIT_NACK_LIST_SEG_COUNT(nackList_p)\
    do {\
        UInt16 loopCnt=0;\
        for(;loopCnt<AM_WINDOW_SIZE;loopCnt++)\
            nackList_p->noOfSeg=0;\
    }while(0)


/** SPR 1753 Changes Start **/
#define QINIT_RLC_MULTI_DATA_PDU_Q()\
    sQueueInit(&amdRawPduIndQ_g)

#define QDEINIT_RLC_MULTI_DATA_PDU_Q(){\
    AmdRawPduInd *snode = RLC_NULL;\
    while((snode = (AmdRawPduInd *)popNode(&amdRawPduIndQ_g)) != RLC_NULL) \
    freeMemPool(snode);\
}


#define ENQUEUE_RLC_MULTI_DATA_PDU_Q(amdRawPduInd_p)\
    pushNode(&amdRawPduIndQ_g,&(amdRawPduInd_p)->sNode)
#define DEQUEUE_RLC_MULTI_DATA_PDU_Q()\
        (AmdRawPduInd*)popNode(&amdRawPduIndQ_g)

#define QCOUNT_RLC_MULTI_DATA_PDU_Q()\
        sQueueCount(&amdRawPduIndQ_g)
/** SPR 1753 Changes End **/

/** SPR 5604 Start **/
#define QUEUE_INIT_AMD_RX_NACK_LIST_Q(AmRxNackListQ) \
    if((AmRxNackListQ.pQ = ((LTE_CIRCQUEUE *)getMemFromPool(sizeof(LTE_CIRCQUEUE),RLC_NULL)))) \
        /* Start 128 UE: Memory Changes */ \
        circQInitAfterCellSetup(AmRxNackListQ.pQ, 2);
        /* End 128 UE: Memory Changes */
/** SPR 5604 End **/

/*SPR 5622 START*/
#define QUEUE_DEINIT_AMD_RX_NACK_LIST_Q(AmRxNackListQ) {\
        AmRxNackListNode* amRxNackList_p = RLC_NULL;\
        DEQUEUE_AMD_RX_NACK_LIST_Q(amRxNackList_p,(AmRxNackListQ));\
        while(amRxNackList_p != RLC_NULL ){\
                    freeMemPool(amRxNackList_p);\
                    amRxNackList_p = RLC_NULL;\
                    DEQUEUE_AMD_RX_NACK_LIST_Q(amRxNackList_p,(AmRxNackListQ));\
                }\
        circQDeInit((LTE_CIRCQUEUE *)AmRxNackListQ.pQ);\
        freeMemPool(AmRxNackListQ.pQ);\
        AmRxNackListQ.pQ = RLC_NULL;\
}
/*SPR 5622 END*/


/** RLC Nack List Changes Start **/
#define CREATE_AMD_RX_NACK_LIST_NODE(amRxNackListNode_p,size){\
       if((amRxNackListNode_p = (AmRxNackListNode *)\
                   getMemFromPool(((sizeof(AmRxNackListNode))+((sizeof(NackList))* size)),RLC_NULL)) == RLC_NULL){\
           LOG_RLC_MSG(LTE_RLC_GET_MEM_POOL_FAILED,LOGERROR,L2_SYS_FAIL,\
                   globalTTITickCount_g, __LINE__, sizeof(AmRxNackListNode), 0,0,0,\
                   0,0, __func__, __FILE__);\
       }\
}
/** RLC Nack List Changes End **/

/* Coverity + 32529 */
#define ENQUEUE_AMD_RX_NACK_LIST_Q(AmRxNackListQ,amRxNackListNode_p) \
        pushNodeCircQ(AmRxNackListQ.pQ,&amRxNackListNode_p->qN)
/* Coverity - 32529 */

#define DEQUEUE_AMD_RX_NACK_LIST_Q(amRxNackListNode_p,AmRxNackListQ){ \
        popNodeCircQ(AmRxNackListQ.pQ, (void *)&(amRxNackListNode_p)); \
                amRxNackListNode_p = (AmRxNackListNode *) amRxNackListNode_p; \
}\


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

typedef enum RlcStatusPduDirT {
    RLC_STATUS_PDU_TX = 0,
    RLC_STATUS_PDU_RX
} RlcStatusPduDir ;

typedef struct RlcControlPduIndT {
    LTE_SNODE sNode ;
    RlcStatusPduDir direction ;
    UInt32  dataLength;     /*length of RLC data*/
    UInt8 * rlcControlPduBuf_p; /*pointer to RLC data*/
    UInt16 ueIndex ;
    RlcLcId lcId ;
} RlcControlPduInd ;

typedef struct RlcSegmentT 
{
    UInt8 e2 ;
    UInt16 soStart;
    UInt16 soEnd;
}RlcSegment;

typedef struct NackListT {
    UInt16 noOfSeg;
    UInt16 nack_sn ;
    UInt8 e1 ;
    RlcSegment rlcSeg[MAX_SEG_PDU];
} NackList ;

/** RLC Nack List Changes Start **/
typedef struct NackListArrT {
    UInt16 count;
    UInt16 ack_Sn;
    UInt16 size;
    NackList nackListArray[0]; 
}NackListArr;
/** RLC Nack List Changes Start **/


typedef struct AmRxNackListNodeT {
    LTE_CNODE qN;
    NackListArr nackListArr;
}AmRxNackListNode;




/****************************************************************************
 * Exported Functions
 ****************************************************************************/


UInt8 rlcPollBitForNewAmdPdu 
(
    TxAMEntity* pAmTxE,
    UInt32 byteWithoutPoll
);
UInt8 rlcPollBitForReTxAmdPdu 
(
    TxAMEntity* pAmTxE
);
UInt32 rlcTxStartPollRetransmitTimer
(
    TxAMEntity* pAmTxE
);
UInt32 rlcRxStartStatusProhibitTimer
(
    RxAMEntity* pAmRxE
);
UInt32 rlcActionOnExpiryOfPollRetransmit
(
    TxAMEntity* pAmTxE
);
/*** SPR 1357 & 1364 Start **/
UInt32 rlcActionOnNackReceiveStatusReport
(
    TxAMEntity* pAmTxE,
    UInt8 *controlPdu_p,
    UInt32 bitPos,
    RlcVr *firstNackRcv
);
/*** SPR 1357 & 1364 End **/
UInt32 rlcActionOnReceiveStatusReport
(
    TxAMEntity* pAmTxE, 
    AmdRawPdu *amdRawPdu_p
);
UInt32 rlcFreeReTxBufferDuringDelInd
( 
    TxAMEntity* pAmTxE,
    RlcVr leastNackRcv,
    UInt16* rlcStatusBufferCount_p,
    RlcStatusBuffer * rlcStatusBuffer_p 
);

UInt32 rlcSendSuccessfulDeliveryInd
(
    TxAMEntity* pAmTxE,
    RlcVr leastNackRcv
);
UInt32 rlcEncodeStatusPdu
(
    RxAMEntity* amRxE_p ,
    RlcPduBuffer* pduFormed_p,
    UInt32 requestedDataSize,
    UInt32 *prlcStatusPduSendSts
);
UInt32 rlcBuildStatusPdu
(
    UInt32 ackSn,
    UInt8 **status_pdu_p,
    NackListArr *nackListArray_p,
    UInt32 cnt
);

#define RLC_AM_NACK_SN_PART_IN  0
#define RLC_AM_NACK_SN_ALL_IN   1

#endif  /* AMDARQOPERATION_H */
