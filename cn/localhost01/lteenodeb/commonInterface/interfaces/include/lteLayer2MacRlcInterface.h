/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2MacRlcInterface.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:45:44 gur20491 Exp $
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
 * $Log: lteLayer2MacRlcInterface.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:45:44  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.4.1.2.1  2009/09/11 12:49:21  gur12140
 * modified to integrate with MAC 1.2
 *
 * Revision 1.2.4.1  2009/06/09 12:05:07  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/04/15 15:56:10  gur19140
 * rlc latency related changes
 *
 * Revision 1.2  2009/04/02 14:36:21  gur19140
 * rlc get tick from mac changes
 *
 * Revision 1.1  2009/03/23 08:37:17  gur19140
 * Integration changes first waves
 *
 * Revision 1.8  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
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
 * Revision 1.4  2009/02/09 14:23:35  gur19140
 * Fixed not able to de-queue problem
 *
 * Revision 1.3  2009/02/06 10:59:03  gur19836
 * CR comments incorporated
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

#ifndef MACRLCINTERFACE_H
#define MACRLCINTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteTypes.h"
#include "lteLayer2CommanTypes.h"
#include "lteQueue.h"
#ifndef UE_SIM_TESTING
#endif




/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#define INVALID_SEQ_NUM 65535
#define INVALID_SOS_START 65535
#define INVALID_SOS_END 65535
/** SPR 3657 Changes Start **/
#define MAX_NUM_UL_PKTS 106
/* SPR 4375 changes start */
#define MAX_RETX_PDU 100
/* SPR 4375 changes end */
/** SPR 3657 Changes End **/
/* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
#define RLC_MAX_DFE_IN_PDU   72
#endif

/* KPI Changes End */


 
#define LTE_NUM_OF_SRB 2
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*typedefs for data transfer between MAC and RLC */
typedef struct RLCUplinkDataInfoT {
    UInt8 * rlcdataBuffer; /*pointer to RLC data*/
    UInt32  dataLength;     /*length of RLC data*/
    UInt8   lcId;   /*LC id for which data is present*/
#ifndef UE_SIM_TESTING   
    UInt8 spsState; /*SPR 20666 fix*/
    /* SPR 15909 fix start */
    tickType_t pktTimeStamp;
    /* SPR 15909 fix end */
#endif
    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    tickType_t packetTime;
    /* SPR 15909 fix end */
    /* 32.425 Events code changes end*/
#endif
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    UInt8 resetSNFlag;
#endif
} RLCUplinkDataInfo;

typedef struct RLCUplinkDataT {
    /** SPR 3657 Changes Start **/
    RLCUplinkDataInfo rlcDataArr [MAX_NUM_UL_PKTS]; /*each index indicate a lc id */
    /** SPR 3657 Changes End **/
    UInt8   numLCInfo ; /*number of logical channels info present in rlcDataArr */
} RLCUplinkData;

/** SPR 3657 Changes Start **/
typedef struct RLCReTxPduInfoT {
    LTE_QUEUE_NODE qN;
    UInt8 *retxRlcPdu_p;
    UInt32 retxPduLen;
    UInt16 retxPduSeqNum;
}rlcReTxPduNode;

#define ENQUEUE_AMD_RETX_PDU_Q(reTxRlcPduQ,rlcReTxPduNode_p)\
    enQueue(&reTxRlcPduQ,&rlcReTxPduNode_p->qN);

#define QUEUE_COUNT_AMD_RETX_PDU_Q(reTxRlcPduQ)\
    queueCount(&reTxRlcPduQ);

#define GET_QUEUE_NODE_AMD_RETX_PDU_Q(rlcReTxPduNode_p, reTxRlcPduQ){\
    rlcReTxPduNode_p = (rlcReTxPduNode *)getQueueNode(&reTxRlcPduQ);\
}

#define SET_NEXT_AMD_RETX_PDU_Q(reTxRlcPduQ,rlcReTxPduNode_p){\
    rlcReTxPduNode_p = (rlcReTxPduNode *)getNextQueueNode(&reTxRlcPduQ,&rlcReTxPduNode_p->qN);\
}

#define DEQUEUE_AMD_RETX_PDU_Q(rlcReTxPduNode_p,reTxRlcPduQ){\
    rlcReTxPduNode_p = (rlcReTxPduNode *)deQueue(&reTxRlcPduQ);\
}



typedef struct rlcLastPduInfoT {
    UInt32 lastPduLen;
	//Encoder_Fix
    UInt32 lastIndex;
    UInt32 rlcPduCount;
}rlcDataInfo;
#define NEW_RLC_PDU 0x04
#define RETX_RLC_PDU 0x02
#define STATUS_RLC_PDU 0x01

/** SPR 3657 Changes End **/ 

typedef struct RLCDataBufferT {
    UInt8 *statusPdu_p;
    LTE_QUEUE reTxRlcPduQ; 
    UInt8 *newRlcPdu_p;
    UInt8 *retxRlcPdu_p;
    
    UInt32 statusPduLen;
    UInt32 newPduLen;
    UInt32 totalPduLen;   
    UInt32 retxPduLen;        /* NOTE: RLC doesn't fill it, retxPduLen is in each reTxRlcPduQ */
    
    UInt32 requestedDataSize; /* RLC PDU buffer length requested*/
    UInt32 remainingDataSize; /* Remaining Queue load for logical channel */
    UInt16 newPduSeqNum;
    /** SPR 3657 Changes Start **/
    UInt16 retxPduSeqNum;
    /** SPR 3657 Changes End **/

    SInt8  lcID;  /* Logical Channel ID */
#ifndef UE_SIM_TESTING
    /*Following info is sent to MAC in RLC PDU for PacketDelay KPI
     * Implementation @ MAC*/
    UInt8  sduCount;
    UInt8  qci;
    /* SPR 15909 fix start */
    tickType_t  pktArrTime[MAX_RLC_SDU_IN_PDU_FOR_KPI]; /*pkt Arrival time of each SDU in RLC PDU*/
    /* SPR 15909 fix end */
#endif
#if defined(KPI_STATS) || defined(PERF_STATS)
    /* It will store the sdu count for number of  completely encoded 
     * RLC SDU and last segment of partially encoded SDU.
     * If there is any SDU in RLC PDU which is either first segment or 
     * any mid segment of SDU, that will not be counted */
    UInt8 completeEncodedSduCount;
    /* This array will be used to store the size of individual PDCP SDU */
    UInt16 pdcpSduSize[RLC_MAX_DFE_IN_PDU];
#endif
} RLCDataBuffer;

/** Rel 3.1 Changes Start **/
typedef enum RlcAmStatusT
{
    /* This is to handle Normal case*/
    RLC_NORMAL_DATA,
    /* This for Window Stall case*/
    RLC_WAIT_FOR_STATUS_PDU
}RlcAmStatus;

typedef struct LCQueueLoadT {
    UInt32 lcQueueLoad;
    SInt8 lcId;
    /* Data Bytes Scheduled in this MAC oppurtunity
    ** In case of Fresh Data Transmission only
    ** Will be Zero for Control and Retxn Data
    **/
    UInt32 scheduledDataBytes;
    /* Arrival Time stamp of the Head of the line Packet */
    UInt64 headPktTS;
    /* This will be the retransmission PDU Size
    ** The First PDU to be retransmited.
    **/
    UInt32 retxnPduSize;
    /* The Size of the Status PDU to be sent to peer Entity */
    UInt16 ctrlQueueLoad;
    /* Qload Flag, Indicates the qload belong to Control,
    ** Retxn Data or Fresh Data
    ** Also Indicates about the window stall, when RLC is waiting for
    ** Status PDU from PEER Entity
    **/
    RlcAmStatus ctrlFlag;
    /** Rel 3.1 Changes End **/

#ifndef UE_SIM_TESTING
    /* Following flag is to get IPv4/Ipv6 packet type information */
    UInt8  ipPktType; 
    UInt8  numOfSdu;
    /* Flag to voice or silence by PDCP */
    UInt8  voiceSilenceType;
    /* Flag to indicate that RLC has sent segmented PDU in DL*/
    UInt8  RLCPduInd;
    /* Average Running SDU size for re-activation */
    UInt16 avgRunningSduSize;
    UInt32 headPktQueueLoad;
#endif
    
} LCQueueLoad;

/* SPR 1273 Start */
typedef enum BOTypeT
{
    RLC_BO_TYPE_NEW,
    RLC_BO_TYPE_UPDATE
}BOType;
/* SPR 1273 End */

#ifndef UE_SIM_TESTING

typedef struct RlcMacSegPduIndT {
    UInt32 ueIndex;
    UInt8 lcId;
    UInt8 rlcPduInd;
} RlcMacSegPduInd;

/* Following enums defined for flags to be given to MAC in BO request */
typedef enum RlcPduIndT
{
    RLC_NO_IND = 0,
    RLC_PDU_SEG_IND = 1,
    RLC_EXCESS_OPP_IND = 2
} RlcPduInd ;
#endif



typedef struct ARQInfoT
{
    /* Sequence number of the RLC PDU */
    UInt16  seqNumber;
    SInt8   lcID;
}ARQInfo;

/** SPR 3657 Changes Start **/
typedef struct RLCHarqFailureReTxInfoT {
    UInt16  retxPduSeqNum;
    UInt16  SOStart;   /* RLC PDU buffer Segment Offset Start */
    UInt16  SOEnd; 
}rlcHarqFailureReTxInfo;
/** SPR 3657 Changes End **/

typedef struct RLCHarqFailureInfoT {
    UInt8   lcId;
    UInt8   harqFailureReporting;   /* Flag Indicates whether harq 
                                      failure indication required or not */
    UInt16  newPduSeqNum;
    /** SPR 3657 Changes Start **/
    UInt32 reTxPduCount;
    rlcHarqFailureReTxInfo harqFailReTxInfo[MAX_RETX_PDU]; 
    /** SPR 3657 Changes End **/
    UInt16  SOStart;   
    UInt16  SOEnd;
} RLCHarqFailureInfo;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* RLC Layer has to implement these indications */
UInt32  macComChDataInd (UInt16 comChType, UInt16 rnti, UInt8* data_p,UInt32 size) ;
UInt32  macComChOpportunityInd(UInt16 comChType, UInt16 rnti, UInt32 size) ;
UInt32  macUeDataInd(UInt16 ueIdx, RLCUplinkData* dataToRlc) ;
UInt32  macUeOpportunityInd(UInt16 ueIndex,UInt8 lcCount,
        RLCDataBuffer* rlcBuffer_p,UInt32 requestedSubFrame 
        ,InternalCellIndex internalCellIndex
         );

/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
UInt32 macSubFrameInd(InternalCellIndex internalCellIndex);
/* SPR 1535 Changes Start*/
void  macFirstSubFrameInd(UInt32 sysFrameNum, UInt32 subFrameNum,InternalCellIndex internalCellIndex);
/* SPR 1535 Changes End*/
#else
/* + SPR 17439 */
UInt32 macSubFrameInd(void);
/* - SPR 17439 */
/* SPR 1535 Changes Start*/
void  macFirstSubFrameInd(UInt32 sysFrameNum, UInt32 subFrameNum);
/* SPR 1535 Changes End*/
#endif
/*  - Layer2 NON CA Changes */

/* +- SPR 17777 */
UInt32 rlcGetUeLogicalChannelBO(void);

UInt32 rlcSendARQIndication (void);
/* +- SPR 17777 */


/* MAC Layer has to implement these requests */
/*coverity 25519 Fix*/
/* + SPR 17439 */
UInt32 macComChUpdateBufferOccupancyReq(void);
/* - SPR 17439 */
/*coverity 25519 Fix*/
/* +- SPR 17777 */
UInt32 macComChDataReq(void);
/* SPR 1273 Start */
UInt32 macUpdateBufferOccupancyReq(UInt16 ueIndex, UInt16 lcCount, 
           LCQueueLoad * lcQueueLoadArr_p,BOType boFlag, InternalCellIndex internalCellIndex
           );
/* SPR 1273 End */
UInt32 macUeDataReq(UInt16 ueIndex,UInt8 lcCount,RLCDataBuffer * rlcBuffer_p, UInt8 sucessOrFailure,
			RLCHarqFailureInfo* rlcHarqFailureInfo_p
#ifndef UE_SIM_TESTING
#ifdef DL_DATA_SEPARATION
            /* SPR 15909 fix start */
            , tickType_t currentTick
            /* SPR 15909 fix end */
#endif 
#endif 
            ,InternalCellIndex internalCellIndex           
            ) ;


UInt32 macUeHarqFailureInd (UInt16 ueIndex, 
    RLCHarqFailureInfo* rlcHarqFailureInfo);  

void rlcErrorDataInd( SInt8 lcID,
                      UInt16 seqNumber,
                      UInt32 bufferLength,
                      UInt8*  rlcBuffer_p);


#endif  /* INCLUDED_MACINTERFACE_H */
