/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcEntityDef.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:32 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This files declares the high-level structures of RLC entities.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcEntityDef.h,v $
 *
 *
 ****************************************************************************/

#ifndef DATASTRUCT_H 
#define DATASTRUCT_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcTypes.h"
#include "lteRlcDefs.h"
#include "lteRlcUmdSdu.h"
/*RLC Rel 2.0 Changes Start*/
#include "lteRlcAmdSdu.h"
#include "lteRlcAmdRing.h"
/*RLC Rel 2.0 Changes End*/
#include "lteRlcTmdSdu.h"
#include "lteRlcUmdRing.h"
#include "lteRlcStat.h"
//#include "lteRlcAmdArqOperation.h"

/****************************************************************************
 * Exported Types
 ****************************************************************************/
#define RLC_QUEUE_SIZE 3000
#define NUM_SDU_FOR_RUNNING_AVG 10
#define MAX_RLC_PDU_PER_TICK 100

/***********************************************************
 * Structures for TM mode entity .
 **********************************************************/

/* Timer Nodes */
typedef struct RlcTimerNodeT
{
    /*Node Anchor of the Node*/
    LTE_LIST_NODE   nodeAnchor;
    UInt16 ueIndex;
    UInt8 isValid;
    UInt8  lcId;
    /* SPR 11571 changes start */
    UInt16 pollSN;
    /* SPR 11571 changes end */
}RlcTimerNode;

typedef struct RlcPRTmT
{
    UInt16 prDuration;
    UInt16 prExpiryIdx;
    RlcTimerNode* prTimerNode_p; 
} RlcPRTm;
typedef struct RlcSPTmT
{
    UInt16 spDuration;
    UInt16 spExpiryIdx;
    RlcTimerNode* spTimerNode_p;
}RlcSPTm;

typedef struct RlcROTmT
{
    UInt16 roDuration;
    UInt16 roExpiryIdx;
    RlcTimerNode *roTimerNode_p;
}RlcROTm;
typedef struct TxTMEntityT 
{
    UInt8 comChType ;
    RlcEntityState rlcEntityState ;
    TmdRawSduQ txSDUQueue ;
    RlcQSz txQueueSize ;   
} TxTMEntity ;

typedef struct RxTMEntityT 
{
    UInt8 comChType ;
} RxTMEntity ;

typedef struct TMEntityT 
{
    TxTMEntity* txTMEntity_p ; /* Pointer to the transmitting TM Entity. */
    RxTMEntity* rxTMEntity_p ; /* Pointer to the receiving TM Entity. */
} TMEntity ;

/***********************************************************
 * Structures for UM mode entity .
 **********************************************************/
typedef struct TxUMEntityT 
{
    /*This field indicates the logical channel identifier corresponding to this RLC entity.*/
    RlcLcId lcId ;
    /* The size of Fixed Header can be 1 byte or 2 byte based upon the SN lengh */
    UInt8 szFHdr;
    /* This field indicates the value of sequence number to be assigned to next newly generated UMD PDU. 
    It will be initially set to 0 and will be incremented every time an UMD PDU is delivered. */
    RlcVt US ;
    /* This field indicates the unique identifier assigned to the UE.*/
    UInt32 ueIndex ; 
    /* This field indicates the size in bytes of the SDU queue.
    This variable shall be updated with updating of the SDU queue. 
    MAC will be notified of any change in the SDU queue size for MAC scheduling purposes.*/
    RlcQSz txQueueSize ;
    /* Indicates the UM RLC SN field size in bits. Its value is either 5 or 10.*/
    SnFieldLength snFieldLength ;

    UInt32 modValForWrapRound; 
   /* This queue stores the SDUs received from the PDCP layer. 
    The SDUs will be stored in the queue until MAC asks for data. 
    Based on the size of PDU asked for, SDUs will be segmented or 
    concatenated before transmission to MAC.*/
    UmdRawSduQ txSDUQueue;
    
    UmdRawSduIn txSduIn;
    /*SPR 8544 FIX START*/
    UInt32 rlcQueueSize;
    /*SPR 8544 FIX END*/
    /*SPR 16936 fix*/
    UInt64 headPktTS; 
    /*SPR 16936 fix*/
    /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    //UInt32 rlcQueueSize;
    UInt8 qci;
    UInt32 bitRateTx;
    /* Sets if SPS is enabled on this LC */
    UInt8 isSpsLc;
    /* Counter to maintain num of seg in DL */
    UInt32 rlcTxSegCounter;
    /* previous Avg SDU size : used in case of BO Update */
    UInt32 prevAvgRunningSduSize;
#endif
    /** Rel 4.0 changes End **/
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
    /*SCG Bearer or SCG Split Bearer*/
    DCBearerType dcBearerType;
#endif
} TxUMEntity ;

typedef struct RxUMEntityT 
{
    /*This field indicates the logical channel identifier corresponding to this RLC entity.*/
    RlcLcId lcId ;

    UInt8 szFHdr;
    /* This field indicates the value of the sequence number of 
    the earliest UMD PDU that is still considered for reordering.*/
    RlcVr UR;
    /* This field indicates the value of the sequence number following 
    the sequence number of the UMD PDU which triggered the reordering timer.*/
    RlcVr UX;
    /* This field indicates the value of the sequence number indicating 
    the highest edge window considered for reordering.*/
    RlcVr UH ;
    /* Window Size corresponding to teh SN Field Len */
    UInt16 wSize;
    UInt16 modValForWrapRound;
    /* This field indicates the unique identifier assigned to the UE.*/
    UInt32 ueIndex ; 
   /* This ring stores the PDUs received from the MAC layer.
    These PDUs will be reordered before delivering them to the PDCP layer.*/
    UmdPduRing rxUmdPduRing ;
    /* this queue is used to re-assamble SDUs from PDU ring */
    UmdRawSduQ rxReSduQ ;
    /* This field indicates whether the reorder timer is running for this RLC entity or not.*/
    /* Indicates the UM RLC SN field size in bits. Its value is either 5 or 10.*/
    SnFieldLength snFieldLength ;
    
    /* This field indicates whether the reorder timer is running for this RLC entity or not. */
    RlcBool isTimerRunning ;
    RlcROTm umROTimerInfo; 
    UmdRawSduIn rxSduIn ;
    /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    UInt8 qci;
    UInt32 bitRateRx;
    /* Sets if SPS is enabled on this LC */
    UInt8 isSpsLc;
    /* Counter to maintain num of seg in UL */
    UInt32 rlcRxSegCounter;
#endif
    /** Rel 4.0 changes End **/
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    UInt8 mchEntity;
#endif
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
    /*SCG Bearer or SCG Split Bearer*/
    DCBearerType dcBearerType;
#endif
} RxUMEntity ;

typedef struct UMEntityT 
{
    /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    RlcEntityState rxEntityState ;
    RlcEntityState txEntityState ;
#endif
    TxUMEntity* txUMEntity_p ; /* Pointer to the transmitting UM Entity. */
    RxUMEntity* rxUMEntity_p ; /* Pointer to the receiving UM Entity. */
} UMEntity ;

/***********************************************************
 * Structures for AM mode entity . 
 **********************************************************/
/*RLC Rel 2.0 Changes Start*/
#ifdef DEBUG_STATS
#define MAX_RLC_RX_STATUS 10   

typedef struct NackStatusRecvdT {
    UInt16 count;
    UInt16 ack_Sn;
    UInt16 size;
    UInt16 nack_sn[MAX_RLC_RX_STATUS];
} NackStatusRecvd;

typedef struct RlcRxStatusT 
{
    NackStatusRecvd statusRecvd;
    UInt32 statusTti;
}RlcRxStatus;

#endif
typedef struct TxAMEntityT 
{
    /*This field indicates the logical channel identifier corresponding to this RLC entity.*/
    RlcLcId lcId ;
    UInt8 pollBit;
    /*This var is used as flag, when it is set than send status report on tx opp*/
    UInt8 sendStatusInd;
    /* This field indicates the unique identifier assigned to the UE.*/
    UInt16 ueIndex ; 
    RlcSn statusReportAck ;
    /*Acknowledgement state variable*/
    RlcVr vtA;
    /*Maximum send state variable*/
    RlcVr vtMS;
    /*Send state variable*/
    RlcVr vtS;
    /*Poll send state variable*/
    RlcVr pollSN;
    /* This queue stores the SDUs received from the PDCP layer. 
       The SDUs will be stored in the queue until MAC asks for data. 
       Based on the size of PDU asked for, SDUs will be segmented or 
       concatenated before transmission to MAC.*/
    AmdRawSduQ txSDUQueue ;
    /* This field indicates the size in bytes of the SDU queue.
       This variable shall be updated with updating of the SDU queue. 
       MAC will be notified of any change in the SDU queue size for MAC scheduling purposes.*/
    RlcQSz txQueueSize ;

    /*AM RLC entity shall maintain the following counters*/
    /**** Data Type of PollPdu and pollByte is unsigned int, so pduWithoutPoll,byteWithoutPoll should be of same type **/
    UInt32 pduWithoutPoll;
    UInt32 byteWithoutPoll;
    /*Configurable Parameters from RRC*/ 
    MaxRetxThreshold maxRetxThreshold;
    PollPDU pollPDU ;
    /*--coverity--530--54183*/
    UInt32 pollByte;
    /*--coverity--530--54183*/

    /* This field indicates whether the retransmit timer is running for this RLC entity or not. */
    RlcBool istRetransmitTimerRunning ;
    /*NACK list maintain the nack list received from status pdu*/
    AmNackListQ nackList ;
    RlcQSz reTxQueueSize ;
    /*Rx Status PDU size*/
    RlcQSz rxStatusPduSize;

    /*for sending delivery indication msg to pdcp*/
    SduTidInfoQ tidQueue;


    RlcPRTm prTimerInfo;
    AmdRawSduIn txSduIn ;
    /*Re-transmission Q containing PDUs*/
    AmdReTxPduIn txPduIn ;
    AmReTransmission* reTxQueue[AM_WINDOW_SIZE];
    UInt8 *statusPdu_p;   
    /*SPR 8544 FIX START*/ 
    UInt32 rlcQueueSize;
    /*SPR 8544 FIX END*/
    /*SPR 16936 fix*/
    UInt64 headPktTS; 
    /*SPR 16936 fix*/
/*SPR 8448  FIX Start */    
/*    LTE_SEM rlcLock; */
/*SPR 8448  FIX end */    
#ifndef UE_SIM_TESTING
    /** Rel 4.0 Changes Start **/
    //UInt32 rlcQueueSize;
    UInt8 qci;
    UInt32 bitRateTx;
    /** Rel 4.0 Changes End **/
#endif
#ifdef DEBUG_STATS
    UInt8 prTimerExpiryCount;
    UInt8 statusCount;
    RlcRxStatus rlcRxStatus[MAX_RLC_RX_STATUS];
    UInt16 invalidCptInHdr;
    UInt16 invalidAckRecvd;
    UInt16 oowAckRecvd;
    UInt16 enQFailedSduQ;
    UInt16 InValidInEntity;
    UInt16 encodingFailedStatus;
    UInt16 encodingFailedReTx;
    UInt16 encodingFailedNewTx;
#endif
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
	    /*SCG Bearer or SCG Split Bearer*/
	        DCBearerType dcBearerType;
#endif

} TxAMEntity ;
                                                                                                             

typedef struct AmRxNackListQT 
{
    LTE_CIRCQUEUE *pQ ;
} AmRxNackListQ ;

typedef struct RxAMEntityT 
{
    /*This field indicates the logical channel identifier corresponding to this RLC entity.*/
    RlcLcId lcId ;
    /*This var is used as flag, when it is set than send status report on tx opp*/
    UInt8 sendStatusInd;
	
    UInt8 pollDelayInd;//bug[003]jsm
	UInt16 pollDelaySN;//bug[003]jsm
    UInt8 RLCRESETFLAG;
    /* This field indicates the unique identifier assigned to the UE.*/
    UInt16 ueIndex ; 
    //RlcQSz rxStatusPduSize ;
    /*Receive state variable*/
    RlcVr vrR;
    /*Maximum acceptable receive state variable*/
    RlcVr  vrMR;
    /*t-Reordering state variable*/
    RlcVr vrX;
    /*Maximum STATUS transmit state variable*/
    RlcVr vrMS;
    /*Highest received state variable*/
    RlcVr vrH;
    
    RlcBool isRTimerRunning ;
 
    RlcBool isSPTimerRunning ;

    AmRxNackListQ amRxNackList;  
    /* This ring stores the PDUs received from the MAC layer.
       These PDUs will be reordered before delivering them to the PDCP layer.*/
    AmdPduRing rxAmdPduRing ;
    RlcROTm amROTimerInfo;
    RlcSPTm spTimerInfo;
    /* this queue is used to re-assamble SDUs from PDU ring */
    AmdRawPdu amdRawPdu_p;
    AmdRawSduIn rxSduIn ;
    /** Rel 4.0 chanegs Start **/
#ifndef UE_SIM_TESTING
    UInt8 qci;
    UInt32 bitRateRx;
#endif
   /** Rel 4.0 chanegs End **/
#ifdef DEBUG_STATS
    UInt16 statusCount;
    NackStatusRecvd rxRlcStatus[MAX_RLC_RX_STATUS];
    UInt32 ttiForStatus[MAX_RLC_RX_STATUS];
    UInt16 numPktDropOOW;
    UInt16 numHdrDecFailed;
    UInt16 numPktDropDuplicate;
    UInt16 segPktHandlingFailed;
    UInt16 segPktHandlingFailed_2;
    UInt16 reSegPktHandlingFailed;
    UInt16 firstReSegPktHandlingFailed;
    UInt16 enQFailNackList;
    UInt16 deQFailNackList;
    UInt16 RO_timerExpired;
#endif
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
	/*SCG Bearer or SCG Split Bearer*/
	DCBearerType dcBearerType;
#endif

} RxAMEntity ; 

/*RLC Rel 2.0 Changes End*/

typedef struct AMEntityT {
    /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    RlcEntityState rxEntityState ;
    RlcEntityState txEntityState ;
#endif
    TxAMEntity* txAMEntity_p ; /* Pointer to the transmitting AM Entity. */
    RxAMEntity* rxAMEntity_p ; /* Pointer to the receiving AM Entity. */
} AMEntity ;

/***********************************************************
 * General structures for holding varios types of entities 
 **********************************************************/

typedef struct RLCEntityInfoT {
    /* This decides the mode of the RLC entity i.e. RLC_ENTITY_TM, RLC_ENTITY_UM or RLC_ENTITY_AM. */
    RlcEntityMode rlcEntityMode ; 
    /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    RlcEntityState rlcEntityState ;
#endif
    /* This field indicates whether this entity is transmitting or receiving.
       This field is required to verify that the request received is for transmit type entity.*/
    RlcEntityType entityType ;
    union {
              /* Based on the rlcEntityMode parameter, a respective RLCEntity 
              shall be selected for configuration, reconfiguration and updating.*/
              TMEntity* tmEntity_p;
              UMEntity* umEntity_p;
              AMEntity* amEntity_p;
    } rlcEntity ;
} RLCEntityInfo ;
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
void rlcInitTxTMEntity(TxTMEntity* pE);
void rlcInitRxTMEntity(RxTMEntity* pE);
void rlcInitTxUMEntity(TxUMEntity* pE);
void rlcInitRxUMEntity(RxUMEntity* pE);
void rlcInitTxAMEntity(TxAMEntity* pE);
void rlcInitRxAMEntity(RxAMEntity* pE);

void rlcDeInitTxTMEntity(TxTMEntity* pE);
void rlcDeInitRxTMEntity(RxTMEntity* pE);
void rlcDeInitTxUMEntity(TxUMEntity* pE);
void rlcDeInitRxUMEntity(RxUMEntity* pE);
void rlcDeInitTxAMEntity(TxAMEntity* pE);
void rlcDeInitRxAMEntity(RxAMEntity* pE);

RLCEntityInfo* rlcCreateRLCEntityInfo
(
    RlcEntityMode rlcEntityMode,
    RlcEntityType eT
);
UInt32 rlcFreeRLCEntityInfo
(
    RLCEntityInfo *rlcEntityInfo, 
    RlcEntityType entityType
);

UMEntity* rlcCreateUMEntity(RlcEntityType eT);
void rlcFreeUMEntity(UMEntity *e, RlcEntityType entityType) ;

void rlcFreeTMEntity(TMEntity *e) ;

TMEntity* rlcCreateTMEntity(RlcEntityType eT);

AMEntity* rlcCreateAMEntity(RlcEntityType eT);
void rlcFreeAMEntity(AMEntity *e, RlcEntityType entityType);

/* SPR 9911 Fix */
#ifndef UE_SIM_TESTING 
typedef struct RlcTriggerStatusReportNodeT
{
    LTE_SNODE nodeAnchor;
    UInt16 ueIndex;
    UInt8  lcId;
}RlcTriggerStatusReportNode;
#endif
/* SPR 9911 Fix */

#endif  /* DATASTRUCT_H */
