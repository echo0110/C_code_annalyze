/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacRandomAccess.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the function declarations, includes
 *                     and exported variables for Random access procedure
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacRandomAccess.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.23  2009/12/30 16:13:14  gur15697
 * comments incorporated
 *
 * Revision 1.22  2009/10/26 09:06:47  gur18550
 * Function Updated for 500 UEs
 *
 * Revision 1.21  2009/08/26 11:55:44  gur12140
 * Optimzed For DCI CCE Array
 *
 * Revision 1.20  2009/07/26 08:11:55  gur15697
 * compilation warnings removed
 *
 * Revision 1.19  2009/07/13 12:06:28  gur15697
 * RACH_MGR added
 *
 * Revision 1.18  2009/07/13 10:22:13  gur15697
 * #if 0 removed
 *
 * Revision 1.17  2009/07/01 04:22:34  gur15697
 * review comments incorporated
 *
 * Revision 1.16  2009/06/23 04:55:25  gur15697
 * review comments incorporated
 *
 * Revision 1.15  2009/06/22 10:01:55  gur15697
 * extern LTE_SQUEUE contentionResolutionMsgAckQ_g added
 *
 * Revision 1.14  2009/06/22 04:52:56  gur15697
 * review comments incorporated
 *
 * Revision 1.13  2009/06/19 13:30:11  gur15697
 * sendCCCHMsgToRRC() modified
 *
 * Revision 1.12  2009/06/19 13:24:17  gur15697
 * review comments incorporated
 *
 * Revision 1.11  2009/06/19 05:37:10  gur15697
 * review comments incorporated
 *
 * Revision 1.10  2009/06/18 09:14:21  gur15697
 * lowerCRNTIBound_g and upperCRNTIBound_g modified to  lowerRNTIBound_g and upperRNTIBound_g
 *
 * Revision 1.9  2009/06/08 08:53:07  gur15697
 * PRACH code moved to lteMacPRACH.h
 *
 * Revision 1.8  2009/06/05 08:04:52  gur15697
 * init and cleanup functions added
 *
 * Revision 1.7  2009/06/04 12:58:03  gur15697
 * review comments incorporated
 *
 * Revision 1.6  2009/06/01 06:39:23  gur18569
 * removed compilation errors
 *
 * Revision 1.5  2009/05/27 10:01:07  gur15697
 * Random access code added
 *
 * Revision 1.4  2009/05/10 09:03:37  gur15697
 * Removed compilation errors
 *
 * Revision 1.3  2009/05/08 14:39:36  gur15697
 * compilation errors removed
 *
 * Revision 1.2  2009/05/05 14:26:16  gur15697
 * new functions added
 *
 * Revision 1.1  2009/04/30 06:05:42  gur15697
 * Initial vertion
 * Gur15697
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_RANDOM_ACCESS_H
#define LTE_MAC_RANDOM_ACCESS_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacSFNMgr.h"
#include "lteMacTBSize.h"
#include "lteMacEncoder.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif
/*+ E_CID_5.3*/
#include "lteFapiMacPhyInterface.h"
 /*+ E_CID_5.3*/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/******************************** Temp UE context ***************************/
#define RACH_MGR "rach_mgr"

#define MAX_TEMP_CRNTI_CTX  MAX_UE_SUPPORTED
#define INVALID_FREE_INDEX  MAX_TEMP_CRNTI_CTX

/****************************************************************************/
/*As per 36.321 contention resolution mac control element size is 6*/
#define CONTENTION_RES_MAC_CE_SIZE    6
#define CONTENTION_RES_SUB_HEADER_LEN 1

#define RAR_SUB_HEADER_LEN 1
#define RAR_LEN            6

/*MAC will provide the UL grant in RAR for Msg3 after 6ms from tick  
 *in which RAR sent on air
 */
#define UE_RA_ADV_TTI         6

/* Maximum duration between transmission of RAR and reception of MSG3   
 * In case of TDD Configuration 5 if RAR is transmitted in SF 7 and SFN N
 * Msg3 will be recieved in SF 2 and SFN N+2. The value is 15 Milliseconds 
 */
#define MAX_TDD_RAR_MSG3_OFFSET 15 

/*As per 36.321 section-5.1.4 RA Response window which starts at the subframe 
 *that contains the end of the preamble transmission plus three subframes
 */
#define RAR_WINDOW_START_DELAY 3

/*Percent of UL available RBs to be used for RAR*/
#define PERCENTAGE_OF_UL_RB_FOR_RAR (0.1)

/* As per current design only ten percent of uplink bandwidth 
 * can be allocated to RA responses.Ten percent of uplink bandwidth 
 * will be ten RB.Each CFR RAR requires two RB so there can be only 
 * at max four RA response */

/* Based on the current design MAX_RAR_PER_PDU and MAX_RAR_PDU_PER_TTI 
 * has to be changed to four from eight and four respectively.
 * The logic is at max there can be four different RAR PDU each 
 * with single RAR response or there can be one RAR PDU with four 
 * RAR Responses So maxRAResponses should be at max equal to or 
 * less than four
 */ 

#define MAX_RAR_PDU_PER_TTI 4

#define MAX_RAR_PER_PDU   4


#define MAX_MSG3_PER_TTI  MAX_RAR_PDU_PER_TTI

#define MAX_UPDATE_TCRNTI_COUNT 4



/*UL grant in case of CRNTI MAC CE received*/
#define CONTENTION_RES_UL_GRANT 5

/*Start the RAR timer with the delay to avoid race conditions.
 *i.e. If Msg3 comes in tick x, start the timer to expire 
 *in tick (x + RAR_TIMER_EXPIRY_DELAY)
 */
/* SPR 6495 start */
#ifdef FDD_CONFIG
#define RAR_TIMER_EXPIRY_DELAY  2
#else
#define RAR_TIMER_EXPIRY_DELAY  6 /* Maximum difference between UL and HI*/
#endif


#define INVALID_RNTI 0xFFF4

 /* SPR 1600 changes start */
#ifdef FDD_CONFIG
 /* SPR 1786 changes start */
/* SPR 1827_28_29 Fix Start */
#define RAR_MSG3_FDD_OFFSET  2
/* SPR 1827_28_29 Fix End */
 /* SPR 1786 changes end */
#endif
 /* SPR 1600 changes end */

 /* SPR 1604 changes start */
#define MIN_DIFF_RA_RAR  2
 /* SPR 1604 changes end */

#define MAC_PRACH_MASK_INDEX_FOR_CONTENTION_FREE_RACH 0

/* SPR 16773 fix start */
/* This timer is to trigger delete TCRNTI when create UE is not received
 * within defined time */
#define MAC_CREATE_UE_TIMER_EXPIRY    65000 /* in ms */
/* SPR 16773 fix end */
/* + SPR 17733 */
#define FILL_HARQ_INFO_FOR_MSG4(tempUEContext_p,harqProcess_p,contentionQueueNode_p,tbSizeFound,numRB,mcsIndex)\
{\
    harqProcess_p = &(tempUEContext_p->dlHarqCtx_p->harqProcess[harqProcess_p->harqProcessId]);\
    if (harqProcess_p)\
    {\
      harqProcess_p->isTBOneValid = TRUE;\
      harqProcess_p->isTBTwoValid = FALSE;\
      harqProcess_p->dlHARQTBOneInfo.mcsIndex = mcsIndex;\
      harqProcess_p->dlHARQTBOneInfo.tbSize = tbSizeFound;\
      harqProcess_p->transmissionType = INVALID_TX;\
      harqProcess_p->assignedResourcesNB = numRB;\
      harqProcess_p->dciFormat = DCI_FORMAT_FOR_CONTENTION_MSG;\
      harqProcess_p->harqStatus = HARQ_PROCESS_IN_USE;\
      harqProcess_p->transmissionBuffer_p = contentionQueueNode_p->msg_p;\
      harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;\
      harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;\
    }\
    else\
    {\
        LOG_MSG( MAC_ALL_HARQ_ARE_BUSY,LOGFATAL,MAC_DL_Strategy, \
                getCurrentTick(), tempUEContext_p->tcRNTI, 0,0,0,0, 0,0, \
                __func__, "ContentionResMsg" );\
    }\
}\
/* - SPR 17733 */
/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef enum RNTIT
{
    CRNTI = 0,
    TCRNTI,
    FREERNTI,
    SPS_RNTI
}RNTI;

typedef enum RAStatusT
{
    RAR_SENT = 0,
    MSG3_RECEIVED,
    CONTENTION_SENT,
/* SPR 4934 changes  */
    CONTENTION_ACK_RECD,
/* SPR 4934 changes  */
}RAStatus;

typedef enum RachDemuxMsgTypeT
{
    MSG3WITHCRNTI=0,
    MSG3WITHCCCH,
    /* Contention free Rach is performed successfully 
     * and uplink data is received from the UE */
    FIRST_UPLINK_DATA_AFTER_CFR
}RachDemuxMsgType;

typedef enum RachTypeT
{
    FREE_TCRNTI = 0,
    CREATE_UE,
    HO_RACH_REQ,
    /* SPR 8379 changes start */
    INTRA_HO_RACH_REQ,
    /* SPR 8379 changes end */
    HO_RACH_REL_IND,
    /*SPR 16855 +-*/
    RACH_DELETE_UE_HO,
    RACH_DELETE_UE,
    CHANGE_CRNTI,
    /*SPR 16855 +-*/
    HO_RACH_CONN_HO_ENTITY_FAILURE
}RachType;





/***************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/******************************** RA Req ************************************/
typedef struct RAPreambleReqT
{
    UInt16 timingAdvance;
    UInt8  preamble;
    UInt8  hoppingBit;
    UInt8  tmcs;
    UInt8  tpc;
    UInt8  ulDelay;
#ifdef FAPI_4_0_COMPLIANCE
    UInt16 timingAdvanceR9;
#endif
}RAPreambleReq;

typedef struct RAReqQueueNodeT
{
    LTE_SNODE raNodeAnchor;
    UInt16    raRNTI;

    /*TTI in which RA preamble received*/
    /* SPR 15909 fix start */
    tickType_t   ttiRAReqReceived; 
    /* SPR 15909 fix end */

    /*RA preamble info received*/    
    RAPreambleReq raPreambleInfo; 
}RAReqQueueNode;


typedef struct RachDemuxQueueNodeT
{
    LTE_SNODE rachDemuxNodeAnchor;
    RachDemuxMsgType msg;
    UInt16 tcrnti;
    UInt16 crnti;
    UInt8  *receivedCCCH_Msg;
    UInt32 length;
    /* SPR 15909 fix start */
    tickType_t receivedTTI;
    /* SPR 15909 fix end */
    UInt32 dlAdvance;
    
}RachDemuxQueueNode;

typedef struct RachDLStrategyQueueNodeT
{
    LTE_SNODE rachDLStrategyNodeAnchor;
    /* +- SPR 18268 */
    UInt16 ueIndex;
    /* +- SPR 18268 */
    /* SPR 15909 fix start */
    tickType_t syncLossDetectedTTI;
    /* SPR 15909 fix end */
}RachDLStrategyQueueNode;


typedef struct PdcchOrderTimerTriggerQueueNodeT
{
    LTE_SNODE pdcchTimeTriggerNodeAnchor;
    UInt8 preambleIndex;
    UInt16 ueIndex;
    /* SPR 15909 fix start */
    tickType_t pdcchOrderTriggerTTI;
    /* SPR 15909 fix end */
}PdcchOrderTimerTriggerQueueNode;

typedef struct RachInterfaceQueueNodeT
{
    /*SPR 16855 +-*/
    MacProcessRrcOamMsgEvent eventId;
    /*SPR 16855 +-*/
    RachType msgType;
    void *rachInterface_p;
    UInt16   ueIndex;
}RachInterfaceQueueNode;

/****************************************************************************/

//fix@guardPeriod +
//fix@guardPeriod -

typedef struct RNTIInfoT
{
    /*UE specific search space info*/
    /*pdcch changes start*/
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p[MAX_NUM_CELL];
    /*pdcch changes end*/

    /*CRNTI/TCRNTI/FREERNTI*/
    RNTI    rntiFlag;

    /*Represents Index of the UE context array incase of CRNTI
     *Represent the index of the Temporary UE context incase of TCRNTI
     *index is invalid incase of FREERNTI
     */    
    UInt16  index;

//fix@guardPeriod +
//fix@guardPeriod -
}RNTIInfo;

/*Contains the Message3 info received*/
typedef struct Msg3InfoT
{
    UInt8  msgLen;
    /*Parameter for Power Control*/
    /*For keeping the SINR received in Message3*/
    UInt8 sinr;

    UInt16 crnti; /*CRNTI incase of CRNTI MAC CE received*/
    UInt8 *msg_p;
}Msg3Info;


typedef struct RAReqTobeScheduledT
{
    RAReqQueueNode *raReqQNode_p;
    UInt16 tcRNTI;
    UInt8  numRBAllocated;
    UInt8  mcsIndex;
}RAReqTobeScheduled;

/*UL grant info allocated in RAR*/
typedef struct RARspResourceInfoT
{
    UInt16 tcRNTI;
    UInt8  riStart; /*Start of the RB*/
    UInt8  riLength; /*Number of RB allocated*/
    UInt8  harqProcessId; 
    /* + CQI_5.0 */
    UInt8 isCqiWithCFRPresent;
    /* - CQI_5.0 */
    RAReqQueueNode *raReqQNode_p;
    ULHarqInfo *ulHarq;
    /*SPR 5502 changes start */
    UInt32 ueIndex;
    /*SPR 5502 changes end */
}RARspResourceInfo;

/**************************Temporary UE context*****************************/

typedef struct TempContextTimerNodeT
{
    LTE_LIST_NODE nodeAnchor;
    
    /*TTI in which timer will expires*/
    /* SPR 15909 fix start */
    tickType_t  expTTI;
    /* SPR 15909 fix end */
    UInt32  deleteFlag;
    UInt16  tcRNTI;
}TempContextTimerNode;

/* SPR 16773 fix start */
/* This structure is to delete the TCRNTI in case UE is not successfully 
 * created. This will help in recovering from TCRNTI leak in case there is any
 * scenario where L3 does not respond after the CCH message is sent */
typedef struct CreateUeContextTimerNodeT
{
    LTE_LIST_NODE nodeAnchor;
    /* Time when this TCRNTI can be deallocated and return to free pool */
    /* SPR 21068 start */
     tickType_t expTTI;
    /* SPR 21068 end   */
    /* TCRNTI allocated */
    UInt16  tcRNTI;
} CreateUeContextTimerNode, *LP_CreateUeContextTimerNode;
/* SPR 16773 fix end */

typedef struct ContentionTimerNodeT
{
    LTE_SNODE nodeAnchor;
    
    /*TTI in which timer will expires*/
    /* SPR 15909 fix start */
    tickType_t  expTTI; 
    /* SPR 15909 fix end */
    UInt32  deleteFlag;
    UInt16  tcRNTI;
}ContentionTimerNode;

typedef struct ConFreePreambleTimerNodeT
{
    LTE_LIST_NODE nodeAnchor;
    
    /*TTI in which timer will expires*/
    /* SPR 15909 fix start */
    tickType_t  expTTI; 
    /* SPR 15909 fix end */
    UInt8   preambleIndex;
}ConFreePreambleTimerNode;
typedef struct ConBasedPdcchOrderTimerNodeT
{
    LTE_LIST_NODE nodeAnchor;

    /*TTI in which timer will expires*/
    /* SPR 15909 fix start */
    tickType_t  expTTI; 
    /* SPR 15909 fix end */
    UInt16  ueIndex;
}ConBasedPdcchOrderTimerNode;

typedef struct ContentionResAckNodeT
{
    LTE_SNODE nodeAnchor;
    UInt32    ueIndex;
}ContentionResAckNode;

typedef struct TempCRNTICtxT
{
    RAStatus  status;

    /*Contains the Message3 info received*/
    Msg3Info  msg3BuffInfo;

    void  *timerNode_p;

    /*TTI in which RA Request received*/
    /* SPR 15909 fix start */
    tickType_t   ttiRAReqReceived;
    /* SPR 15909 fix end */
    UInt16   raRNTI;
    UInt16   tcRNTI;
    /* TA_CHG */
    UInt16   timingAdvanceAbs;
    /* TA_CHG */
    /* SPR 15909 fix start */
    tickType_t   lastTASentTick;
    /* SPR 15909 fix end */
    /* SPR 1600 changes start */
    ULHarqInfo ulHarq;
    /* SPR 1600 changes end */
/* SPR 3061 Chg */
    /* SPR 15909 fix start */
     tickType_t lastSyncDetectedTick;
    /* SPR 15909 fix end */
    /* + SPR 17733 */
#ifdef FDD_CONFIG
    UInt32 contentionResExp;
#endif
    /* - SPR 17733 */
/* SPR 3061 Chg */

    /*SPR_3305_FIX_START*/
/* msg3_retx_tdd_support_start */
    UInt8 ulHarqTimerNodeArrrayIndex ;
/* msg3_retx_tdd_support_end */
    UInt8 isPdcchAllocated;
     /* +- SPR 16962 */
    /* + SPR 17733 */
#if defined(FDD_CONFIG)
    DLHARQEntity *dlHarqCtx_p;
#endif
    /* - SPR 17733 */
    /*SPR_3305_FIX_END*/
    /*+ E_CID_5.3*/
    /* +- SPR 18268 */
    UInt16 exsistingUeIdx;
    /* +- SPR 18268 */
    UInt8 validReports;
    UInt16 type1TAValue;
    /* SPR 15909 fix start */
    tickType_t type1TaAOAReceivedTTI;
    UInt16 type2TAValue;
    tickType_t type2TAreceivedTTI;
    /* SPR 15909 fix end */
    UInt16 angleOfArrival[MAX_NUM_ANTENNA];
    /*- E_CID_5.3*/
    /* msg3_retx_tdd_support_start */
#ifdef TDD_CONFIG
    UeScheduledInfoForMsg3 ulUeScheduledInfoPointerArray;
#endif    
    /* msg3_retx_tdd_support_end */

}TempCRNTICtx;

typedef struct TempCRNTICtxNodeT
{
    TempCRNTICtx *tempCRNTICtx_p;
    
    /*Points to the next free index in tempCRNTICtxArray[] array*/
    UInt32 nextFreeIndex;
}TempCRNTICtxNode;

typedef struct TempCRNTICtxInfoT
{
    TempCRNTICtxNode tempCRNTICtxArray[MAX_TEMP_CRNTI_CTX];
    
    /*points to the free index in tempCRNTICtxArray[] array*/
    UInt32 freeIndex;
}TempCRNTICtxInfo;
/****************************************************************************/



/******************************** RA Rsp ************************************/
typedef struct DLRARspT
{
    UInt8   *macPduData_p;
    
    /*Expiry tick before which message should be scheduled*/
    /* SPR 15909 fix start */
    tickType_t  exptti;/*(end of rar window) - DL delay - Phy delay*/

    /*TTI in which UL RBs reserved*/
    tickType_t  tti;
    /* SPR 15909 fix end */

    /*Number of RARs present in the message*/
    UInt32  numRAR;

    UInt16  dataLen;
    UInt16  raRNTI;
    /* SPR_10231_FIX_START */  
    UInt8 tpc; 
    /* SPR_10231_FIX_END*/
    /*coverity-530 CID-32303*/
    DCIFormat   dciFormat;
    /*coverity-530 CID-32303*/
    UInt8   numRB;
    UInt8   mcsIndex;
    UInt8   payLoadPos;
    
    /* In case of RAR cannot be sent in the nth TTI
     * BIFlag,number of contention free preambles,
     * list of contention free preamble Index are updated 
     * which will be use to form RAR in n + 1th TTI */

    UInt8 BIFlag;
    
    /* + PERF_STATS_PH1D1 */
    /* Variable added for Performance stats code*/
#ifdef PERF_STATS
    UInt32 contBasedPreambleCount;
    UInt32 contFreePreambleCount;
#endif
    /* - PERF_STATS_PH1D1 */

    /*UL resource grant information*/
    RARspResourceInfo resourceInfo[MAX_RAR_PER_PDU];
}DLRARsp;

typedef struct DLRARspQueueNodeT
{
    LTE_SNODE rarNodeAnchor;

    /*RA Responce information*/
    DLRARsp   rarToDl;
    /* + Bandwidth Fix */
#ifdef TDD_CONFIG
    UInt32 index;
#endif
    /* - Bandwidth Fix */
}DLRARspQueueNode;

typedef struct ULRARspNegAckQueueNodeT
{
    LTE_SNODE rarNodeAnchor;
    DLRARspQueueNode  *raRspQnode_p;
}ULRARspNegAckQueueNode;

/****************************************************************************/

/******************************** Contention Free RACH **********************/
typedef enum ConFreeRACHTriggerT
{
	INIT_TRIGGER_VALUE,
	PDCCH_ORDER,
	CONNECTION_HANDOVER,
}ConFreeRACHTrigger;


typedef struct ContentionFreePreambleT
{
    ContentionFreePreambleStatus currStatus;
    /*coverity-530 CID-32305*/
    ConFreeRACHTrigger pdcchOrderHandOverFlag;
    /*coverity-530 CID-32305*/
    UInt16 ueIdentifier;
    UInt8 nextFreePreambleIndex;
    /* SPR_4262_FIX Start */ 
    /* PDCCH FIX */
    UInt8 contentionFreeRACHRecvd;
    /* SPR_4262_FIX End */ 
}ContentionFreePreamble;

typedef struct ContentionFreeRACHPoolT
{
    UInt8 currNumOfFreePreambles;
    UInt8 firstFreePreambleIndex;
    UInt8 lastFreePreambleIndex;
    ContentionFreePreamble 
        *contentionFreePreambleArray
        [MAC_MAX_NUM_OF_RA_PREAMBLES];	
}ContentionFreeRACHPool;

typedef struct ConFreeRACHResourcesInfoT
{
	UInt8 preambleIndex;
	UInt8 prachMaskIndex;
} ConFreeRACHResourcesInfo;

typedef enum FreeRARPduT
{
    DCI_FAILURE,
    ALLOCATE_FAILURE,
}FreeRARPdu;

typedef struct RAReqTobeScheduledInPduT{
    UInt8 noOfRAResponses;
    UInt8 BIFlag;
    UInt16 raRnti;
    /* SPR 15909 fix start */
    tickType_t exptti;
    tickType_t tti;
    /* SPR 15909 fix end */
 	UInt32 preambleMap1;
    UInt32 preambleMap2;
    RAReqTobeScheduled raReqTobeScheduled[MAX_RAR_PER_PDU];
} RAReqTobeScheduledInPdu;

typedef struct RarPduTobeScheduledInTTIT{
    UInt8 numRarPdu;
    RAReqTobeScheduledInPdu rarPduTobeScheduled[MAX_RAR_PDU_PER_TTI];
} RarPduTobeScheduledInTTI;


/****************************************************************************/
typedef struct ContentionQueueNodeT
{
    LTE_SNODE nodeAnchor;

    /*TTI in which timer expires*/
    /* SPR 15909 fix start */
    tickType_t contentionResExp;
    /* SPR 15909 fix end */
    UInt8  *msg_p;
    UInt16 tcRNTI;
    /*coverity-530 CID-32302*/
    DCIFormat  dciFormat;
    /*coverity-530 CID-32302*/
    UInt8  numRB;
    UInt8  msgLen;
    UInt8  mcsIndex;
/* SPR 3628 chg */
#ifdef TDD_CONFIG
    UInt8 numRBForSSF;
    UInt8 msgLenForSSF;
    UInt8 mcsIndexForSSF;
#endif
/* SPR 3628 chg */
}ContentionQueueNode;

typedef struct FreeRNTIPoolNodeT
{
    LTE_SNODE nodeAnchor;
    UInt16    rnti; /*RNTI value*/
}FreeRNTIPoolNode;

/* msg3 ulconfig fix start */
#ifdef TDD_CONFIG
typedef struct UlschUciInfoForULConfigForRachT
{
    LTE_SNODE nodeAnchor;
    ULHarqInfo* ulHarqInfo_p;
    UInt16 tcRNTI;
    UInt16 ueIndex;
    /* TDD Config 0 Changes Start */
    UInt8 ulSubframe;
    /* TDD Config 0 Changes End */
    UInt8 containerIndex;
    UInt8 harqProcessId;
    /*SPR 7086 Fix Start*/
    UInt8 nSRS;
    /*SPR 7086 Fix End*/
    UInt8 isCqiWithCFRPresent;
}UlschUciInfoForULConfigForRach;

/* TDD Config 0 Changes Start */
#ifdef MAC_AUT_TEST
/* 
 * This structure is added to test the function prepareAndSendRARToDlScheduler.
 */
typedef struct TestPrepareAndSendRART
{
    UInt8 subFrameForRAR;
    UInt8 dci0SubFrameForMsg3;
    UInt8 subFrameForMsg3;
    UInt16 sysFrameNumForRAR;
    UInt16 dci0SysFrameNumForMsg3;
    UInt16 sysFrameNumForMsg3;
    UInt8 harqProcessId;
}TestPrepareAndSendRAR;
#endif
/* TDD Config 0 Changes End */
#endif
/* msg3 ulconfig fix end */

/*CA Change start*/
#ifdef TDD_CONFIG
#define MAX_NUMBER_OF_CONTAINER 15
#elif FDD_CONFIG
#define MAX_NUMBER_OF_CONTAINER 8
#endif
typedef struct RACHGlobalsT
{
    UInt32  conBasedPDCCHOrderTimerValue;
    UInt32  numBufConBasedPDCCHOrderTimerMemPool;
    /* SPR 5798 Start */
#ifdef FDD_CONFIG
    UInt8 msg3ScheduledCount[MAX_SUB_FRAME];
#elif TDD_CONFIG
    UInt8 msg3ScheduledCount[MAX_NUMBER_OF_CONTAINER];
#endif
    /* SPR 5798 End */
}RACHGlobals;

#ifdef TDD_CONFIG
#define FREE_UL_RAR_RB(x, i, count, internalCellIndex)\
{\
    for (i = 0; i < count; i++)\
    {\
        freeResourceUsingRBIndex(\
                    x.resourceInfo[i].riLength,\
                    x.resourceInfo[i].riStart,\
                    ((x.tti) % getMaxULRBMaps(internalCellIndex) ),\
                    internalCellIndex);\
    }\
}
#else
#define FREE_UL_RAR_RB(x, i, count, internalCellIndex)\
{\
    for (i = 0; i < count; i++)\
    {\
        freeResourceUsingRBIndex(\
                    x.resourceInfo[i].riLength,\
                    x.resourceInfo[i].riStart,\
                    ((x.tti) % NUM_OF_RB_MAP),\
                    internalCellIndex);\
    }\
}
#endif
/*CA Changes end  */

#define SET_BIT_FOR_PREAMBLE(node, map1, map2)\
{\
    if (node->raPreambleInfo.preamble > 31)\
    {\
        map2 |=  1 << (node->raPreambleInfo.preamble - 31);\
    }\
    else\
    {\
        map1 |=  1 << node->raPreambleInfo.preamble;\
    }\
}

#define CHECK_PREAMBLE_BIT(node, map1, map2) \
        ((node->raPreambleInfo.preamble > 31)?\
         (map2 & (1 << (node->raPreambleInfo.preamble - 31))):\
         (map1 & (1 << node->raPreambleInfo.preamble)))


/* CA Changes start  */
extern RNTIInfo   *crntiToIdxTbl_g[MAX_NUM_CELL];
extern TempCRNTICtxInfo* tempCrntiCtx_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* freeRNTIQueue_gp[MAX_NUM_CELL];
/*SRP 22322 +-*/
extern LTE_SQUEUE* contentionTimerQueue_gp[MAX_NUM_CELL];;
extern LTE_SQUEUE* ulRARspNegAckQueue_gp[MAX_NUM_CELL];
extern DL_RA_RSP_QUEUE_TYPE* dlRARspQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* contentionResolutionMsgQ_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* ulGrantContentionResQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* contentionResolutionMsgAckQ_gp[MAX_NUM_CELL];
extern RACHGlobals *rachGlobals_gp[MAX_NUM_CELL];

extern UInt16 macCrntiUeNum_g[MAX_NUM_CELL];
extern UInt16 macTCrntiUeNum_g[MAX_NUM_CELL];

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
extern void initRACHMgr(UInt8 numCellsConfigured);
extern void cleanupRACHMgr(InternalCellIndex cellIndex);

/* Cyclomatic_complexity_changes_start */
/*SPR 22322 +-*/
void cleanupRAReqQueue(InternalCellIndex internalCellIndex);
void cleanupContentionFreeRAReqQueue(InternalCellIndex internalCellIndex);
void cleanupulRARspNegAckQueue(InternalCellIndex internalCellIndex);
void cleanupDLRARspQueue(InternalCellIndex internalCellIndex);
void cleanupRachDemuxQueue(InternalCellIndex internalCellIndex);
void cleanupRachDLStrategyQueue(InternalCellIndex internalCellIndex);
void cleanupPdcchOrderTimerTriggerQueue(InternalCellIndex internalCellIndex);
void cleanupRachInterfaceQueue(InternalCellIndex internalCellIndex);
/*SPR 16773 Fix Start*/
void cleanupUeTimerList(InternalCellIndex internalCellIndex);
/*SPR 16773 Fix End*/
                        /*SPR 15909 fix start*/
void processQueuesBeforeRARspQueue(tickType_t currentGlobalTTI,
                        /*SPR 15909 fix end*/
                InternalCellIndex internalCellIndex, ExecutionLegFunctionArg* execArgs);
/* Cyclomatic_complexity_changes_end */

/* Coverity + 24508 */
extern MacRetType initRNTIPoolTbl(UInt32 maxUE, InternalCellIndex internalCellIndex);
/* Coverity - 24508 */
extern void cleanupRNTIPoolTbl(InternalCellIndex internalCellIndex);

extern void initTempUECtxFreeIndexPool(UInt8 numCellsConfigured);

/* SPR 5599 changes start (ZIP ID 129059) */
/* SPR 15909 fix start */
/* +- SPR 18268 */
extern MacRetType putEntryInPdcchOrderQ(UInt8 preambleIndex, UInt16 ueIndex,
        /* +- SPR 18268 */
        tickType_t currentGlobalTTI, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */
/* SPR 5599 changes end (ZIP ID 129059) */

extern MacRetType putEntryInRAReqQueue(RAReqQueueNode *raReqQNode_p, InternalCellIndex cellIndex);

extern void putEntryInULNegAckRARspQueue(DLRARspQueueNode *raRspQnode_p, InternalCellIndex cellIndex);

extern MacRetType putEntryInRachDemuxQueue(RachDemuxMsgType msg,
                                            UInt16 tcrnti,
                                            UInt16 crnti,
                                            UInt8  *receivedCCCH_Msg,
                                            UInt32 length,
                                            /* SPR 15909 fix start */
                                            tickType_t receivedTTI,
                                            /* SPR 15909 fix end */
                                            UInt32 dlAdvance,
                                            InternalCellIndex cellIndex);

/* +- SPR 18268 */
extern MacRetType putEntryInRachDLStrategyQueue(UInt16 ueIndex, 
                                                /* SPR 15909 fix start */
                                                tickType_t syncLossDetectedTTI,
                                                /* SPR 15909 fix end */
                                                InternalCellIndex internalCellIndex);

extern MacRetType putEntryInPdcchOrderTimerTriggerQueue(UInt16 ueIndex, 
/* +- SPR 18268 */
                                                        UInt8 preambleIndex,
                                                        /* SPR 15909 fix start
                                                         * */
                                                        tickType_t pdcchOrderTriggerTTI,
                                                        /* SPR 15909 fix end */
                                                        InternalCellIndex internalCellIndex);
extern MacRetType putEntryInPdcchRachInterfaceQueue(RachType msgType,
                                                    void *rachInterface_p,
                                                    InternalCellIndex internalCellIndex);

                                            /* SPR 15909 fix start */
extern void processRAReqAndNegRARspQueue(tickType_t currentGlobalTTI,
                                           /* SPR 15909 fix end */
		UInt32 dlDelay,
		UInt32 sfn,
		UInt32 sf
        /*TDD Tick overshoot changes start*/
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT_TDD)
#ifdef TDD_CONFIG
		,ULDLSPFrame sfType
#endif
#endif
        ,InternalCellIndex cellIndex
        ,ExecutionLegFunctionArg* execArgs
		);
        /*TDD Tick overshoot changesend */
/*SPR 2446 Fix Begins*/                                  
                                /* SPR 15909 fix start */
extern void processNegRaRspQueue(tickType_t currentGlobalTTI,
                                 /* SPR 15909 fix end */
                          UInt32* freeULRBForRAR_p,
                          UInt32 *maxRAR_p,
                          UInt32 queueCount,
                          UInt32 *scheduledRARNTIBitMap1_p,
                          UInt32 *scheduledRARNTIBitMap2_p,
                          RarPduTobeScheduledInTTI *raResponsesInTTI_p,
                          InternalCellIndex internalCellIndex);
/*SPR 2446 Fix Ends*/
extern void processRaReqQueue(
                       UInt32* freeULRBForRAR_p,
                       UInt32* maxRAR_p,
                       /* SPR 15909 fix start */
                       tickType_t currentGlobalTTI,
                       /* SPR 15909 fix end */
                       UInt32 dlDelay,
                       UInt32 *scheduledRARNTIBitMap1_p,
                       UInt32 *scheduledRARNTIBitMap2_p,
                       RarPduTobeScheduledInTTI *raResponsesInTTI_p,
                       InternalCellIndex  cellIndex);

extern void processdemuxRachManagerInterfaceQueue( UInt32 rachDemuxQueueCount,
                                                   InternalCellIndex cellIndex,
                                                   ExecutionLegFunctionArg* execArgs
                                                 );
/* SPR 15909 fix start */
extern void processRachDLStrategyQueue(tickType_t currentGlobalTTI, InternalCellIndex cellIndex);
/* +- SPR 17777 */
extern void processPdcchOrderTimerTriggerQueue(InternalCellIndex internalCellIndex);
extern void processPdcchRachInterfaceQueue(InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
/* SPR 15909 fix end */
/*CA Changes end  */

extern void processRARspQueue(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                       DciCCEInfo        *dciCCEInfo_p,
                       UInt32 rarQueueCount,
                       UInt32 *availableCCE_p,
                       UInt32 *availableCommonCCE_p,
                       SInt32 *availableDLRBCount_p,
                       DciCCEContainerInfo *dciCCEContainerInfo_p,
                       /* +- SPR 17777 */
#ifdef TDD_CONFIG
                       /* SPR 15909 fix start */
                       tickType_t currentGlobalTick,
                       /* SPR 15909 fix end */
#endif
                       /* +- SPR 17777 */
                       /*CA Changes Start */
                       InternalCellIndex internalCellIndex
                       /*CA Changes End */
                       );

extern void processContentionMsgQ(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                           DciCCEInfo *dciCCEInfo_p,
                           UInt32 *availableCCE_p,
                           UInt32 *availableCommonCCE_p,
                           SInt32 *availableDLRBCount_p,
                           UInt32 contentionQueueCount,
                           UInt32 subFrameNum,
                           DciCCEContainerInfo *dciCCEContainerInfo_p,
                           PdcchULOutputInfo *pdcchULOutputInfo_p
                           /* 5492 changes start */
                           ,UInt32 *maxUeScheduledInDownlink_p
                           /* 5492 changes end */
                           ,InternalCellIndex internalCellIndex
                           );

extern MacRetType allocateCCEInCommonSpaceForRARMsg(
                                       PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                                       DciCCEInfo        *dciCCEInfo_p,
                                       DLRARspQueueNode  *raRspQnode_p,
                                       UInt32            *availableCCE_p,
                                       DciCCEContainerInfo *dciCCEContainerInfo_p,
                                       /*CA Changes start  */
                                       InternalCellIndex internalCellIndex);
                                       /*CA Changes end  */

extern MacRetType allocateCCEInUESpecificSpaceForContentionResMsg(
                            UESpecificSearchSpaceInfo  *ueSearchSpaceInfo_p,
                            PdcchDLOutputInfo   *pdcchDLOutputInfo_p,
                            DciCCEInfo          *dciCCEInfo_p,
                            ContentionQueueNode *contentionQueueNode_p,
                            UInt32 *availableCCE_p,
                            UInt32 aggregationLevel,
                            DciCCEContainerInfo *dciCCEContainerInfo_p,
                            PdcchULOutputInfo *pdcchULOutputInfo_p
                            /* + SPR 17733 */
#ifdef FDD_CONFIG
                            ,RNTIInfo *rntiInfo_p,
                            DLHARQProcess *harqProcess_p
#endif
                            /* - SPR 17733 */
#ifdef TDD_CONFIG
                            ,
                            UInt8 subFrame
#endif                            
                            /*CA Changes start  */
                            ,InternalCellIndex internalCellIndex
                            /*CA Changes end  */
                            );

extern MacRetType allocateCCEInCommonSpaceForContentionResMsg(
                                     PdcchDLOutputInfo   *pdcchDLOutputInfo_p,
                                     DciCCEInfo          *dciCCEInfo_p,
                                     ContentionQueueNode *contentionQueueNode_p,
                                     UInt32 *availableCCE_p,
                                     DciCCEContainerInfo *dciCCEContainerInfo_p,
                                     PdcchULOutputInfo *pdcchULOutputInfo_p
                                     /* SPR 21487 Fix Start(CSR 130474) */
#ifdef FDD_CONFIG
                                     ,RNTIInfo *rntiInfo_p,
                                     DLHARQProcess *harqProcess_p
#endif
                                     /* SPR 21487 Fix End(CSR 130474) */
#ifdef TDD_CONFIG
                                     ,
                                     UInt8 subFrame
#endif                                     
                                     ,InternalCellIndex internalCellIndex);

 void scheduleRACHMsg(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
                     DciCCEInfo *dciCCEInfo_p,
                     UInt32 *availableCCE_p,
                     UInt32 *availableCommonCCE_p, 
                     SInt32 *availableDLRBCount_p,
                     UInt32 subFrameNum,
                     DciCCEContainerInfo *dciCCEContainerInfo_p,
                     PdcchULOutputInfo *pdcchULOutputInfo_p,
                    /* SPR 15909 fix start */
                     /* +- SPR 17777 */
#ifdef TDD_CONFIG
                     tickType_t currentGlobalTick,
#endif
                     /* SPR 15909 fix end */
                     /* 5492 changes start */
                     UInt32 *maxUeScheduledInDownlink_p
                     /* +- SPR 17777 */
                     /* 5492 changes end */
                     /*CA Changes start  */
                     ,InternalCellIndex internalCellIndex);
                     /*CA Changes end  */

void prepareAndSendRARToDlScheduler(UInt8 rbMapNum,
                                    UInt32 dlDelay,
                                    /* SPR 15909 fix start */
                                    tickType_t currentGlobalTTI,
                                    /* SPR 15909 fix end */
                                    RAReqTobeScheduledInPdu* rarPduTobeScheduled_p,
                                    InternalCellIndex internalCellIndex,
                                    ExecutionLegFunctionArg* execArgs);

extern UInt8* fillRAR(UInt8 *payLoad_p,
                      /*SPR 1786 changes start */
                      UInt16  allocatedRiv,
                      /*SPR 1786 changes end */
                      UInt16 tempCRNTI,
                      RAReqQueueNode *raQNode_p,
                      /* + CQI_5.0 */
                      UInt8 cqiWithRAR,
                      /*CA Changes start  */
                      InternalCellIndex internalCellIndex);
                      /*CA Changes end  */
                      /* - CQI_5.0 */

                                    /* SPR 15909 fix start */
extern MacRetType modifyULGrantInRARAndSendToDL(tickType_t currentGlobalTTI,
                                    /* SPR 15909 fix end */
                                   UInt32 rbMapNum, 
                                   UInt32 dlDelay,
                                   ULRARspNegAckQueueNode *raRspNegAckQNode_p,
                                   UInt32 *maxRAR_p,
                                   UInt32 *scheduledRARNTIBitMap1_p,
                                   UInt32 *scheduledRARNTIBitMap2_p);

extern MacRetType updateRACHConfigInfo(UInt32 sfn,
                                UInt32  cellConfigReqFlag,
                                UInt32  raRspWindowSize,
                                UInt32  macContentionResTimer,
                                UInt32  prachConfigIndex,
                                UInt32  prachFrequencyOffset,
                                UInt32  numContnBasedPreamble,
                                UInt32  preambleGroupAConfigFlag,
                                UInt32  numOfPreamblesInGroupA,
                                UInt32  messageSizeGroupAIdx,
                                UInt8   preambleInitialReceivedTargetPower,
                                UInt8   maxHarqMsg3Tx,
                                /*CA Changes start  */
                                RACHConfigInfo *rachConfigInfo_p,
                                InternalCellIndex internalCellIndex);
                                /*CA Changes end  */

extern MacRetType  isValidFrameForPRACH(UInt8  subFrame,
                                        UInt16 sysFrame,
                                        UInt32 *prachFrequencyOffset_p,
                                        UInt32 *numPhysicalResources_p,
                                        /*CA Changes start  */
                                        InternalCellIndex internalCellIndex);
                                        /*CA Changes end  */

/*CA Changes start  */
extern void processContentionACK(TempCRNTICtx *tempCRNTICtx_p, InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* SPR 15909 fix start */
extern void processContentionMsgTimerList(tickType_t currentTTI);
/* SPR 15909 fix end */

/* SPR 5599 changes start (ZIP ID 129059) */
                                                /* SPR 15909 fix start */
extern ContentionTimerNode* startContentionResTimer(tickType_t  expTTI, 
                                                /* SPR 15909 fix end */
/* SPR 5599 changes end (ZIP ID 129059) */
                                                    UInt16  tcRNTI,
                                                    /*CA Changes start  */
                                                    InternalCellIndex internalCellIndex);
                                                    /*CA Changes end  */
/*SPR 22322 +-*/

extern TempCRNTICtx *createTempCRNTICtx(UInt16 tcRnti, 
                                        RAReqQueueNode *raQNode_p,
                                        /*CA Changes start  */
                                        InternalCellIndex internalCellIndex,
                                        /*CA Changes end  */
                                        ExecutionLegFunctionArg* execArgs);

/*CA Changes start  */
extern void deleteTempCRNTICtx(UInt16 tcRNTI, InternalCellIndex internalCellIndex);

extern TempCRNTICtx* getTempCRNTIContextPointer(UInt16 tcRNTI, InternalCellIndex internalCellIndex);

extern RNTIInfo* getUeIdxFromRNTIMap(UInt16 rnti, InternalCellIndex cellIndex);

extern UInt16 getFreeTempRNTI(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */

extern MacRetType freeRNTI(UInt16 rnti, InternalCellIndex internalCellIndex);
extern MacRetType freeRNTIAfterUEDeletion(UInt16 rnti, InternalCellIndex internalCellIndex);

extern MacRetType freeRNTIBeforeUETempContextCreation(UInt16 rnti, InternalCellIndex internalCellIndex);

extern MacRetType handleCRNTIMACCEReceived(TempCRNTICtx *tempUEContext_p,
                                           UInt16 tcRNTI,
                                           UInt32 dlDelay,
                                           /* SPR 15909 fix start */
                                           tickType_t receivedTTI,
                                           /* SPR 15909 fix end */
                                           InternalCellIndex internalCellIndex,
                                           ExecutionLegFunctionArg* execArgs);

extern void handleCCCHSDUReceived(TempCRNTICtx *tempUEContext_p,
                                  UInt16  tcRNTI,
                                  UInt32  dlDelay,
                                  /* SPR 15909 fix start */
                                  tickType_t  ttiReceivedi,
                                  /* SPR 15909 fix end */
                                  InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129059) */
extern void sendCCCHMsgToRRC(void  *msg_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                    UInt32 dataLen,
        UInt16 rnti,
        UInt16 timingAdvance,
        UInt8 sinr,
        InternalCellIndex internalCellIndex);
/* SPR 15909 fix start */
extern void processContentionMsgTimerQueue(tickType_t currentTTI, UInt32 count, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */

extern void processContentionResAckQueue(UInt32 count, InternalCellIndex internalCellIndex);
/*CA Changes end  */

extern void processRAResInNegRARPDU(UInt32* freeULRBForRAR,
                                   ULRARspNegAckQueueNode *raRspNegAckQNode_p,
                                   UInt32 *maxRAR_p,
                                   UInt32 *scheduledRARNTIBitMap1_p,
                                   UInt32 *scheduledRARNTIBitMap2_p,
                                   RarPduTobeScheduledInTTI *raResponsesInTTI_p,
                                   /* +- SPR 17777 */
                                   InternalCellIndex  cellIndex
#ifdef TDD_CONFIG
                                   /*SPR 15909 fix start*/
                                   ,tickType_t currentGlobalTTI
#endif
                                   );
                                   /* +- SPR 17777 */
                                   /*SPR 15909 fix start*/

extern void processContFreeRAReqQueue(
                       UInt32* freeULRBForRAR_p,
                       UInt32 *maxRAR_p,
                       /* SPR 15909 fix start */
                       tickType_t currentGlobalTTI,
                       /* SPR 15909 fix end */
                       UInt32 dlDelay,
                       UInt32 *scheduledRARNTIBitMap1_p,
                       UInt32 *scheduledRARNTIBitMap2_p,
                       RarPduTobeScheduledInTTI *raResponsesInTTI_p,
                       /*CA Changes start  */ 
                       InternalCellIndex  cellIndex);
                       /*CA Changes end  */
/*Complexity Start*/
extern void eicicFreeRaReqQNode2(
	UInt8 queueCount,
	LTE_SQUEUE  *contentionFreeRAReqQueue_p,
	InternalCellIndex internalCellIndex);
/*Complexity End*/

RAReqTobeScheduledInPdu* getRARPduInfoforRARNTI(
                        UInt16 raRnti,
                        RarPduTobeScheduledInTTI *raResponsesInTTI_p);

RAReqTobeScheduledInPdu* getNextRARPduTobeScheduled(
       RarPduTobeScheduledInTTI *raResponsesInTTI_p);

extern  ContentionFreePreamble*  getContentionFreePreambleInfo(UInt8 preambleIndex, InternalCellIndex internalCellIndex);

extern ContentionFreePreambleStatus  getPreambleStatusInfo(UInt8 preambleIndex, InternalCellIndex internalCellIndex);

/*CA Changes start  */
extern  MacRetType initContentionFreePreamblePool(UInt8 firstFreePreambleIndex, InternalCellIndex internalCellIndex);
extern MacRetType allocateDedicatedPreamble(UInt16 ueIndex, ConFreeRACHTrigger triggerFlag,
                                     ConFreeRACHResourcesInfo *rachResourceInfo, InternalCellIndex internalCellIndex);

extern MacRetType freePreambleIndex(UInt8 freePreambleIndex, InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129059) */
extern  UInt8 getFirstFreePreambleIndex(InternalCellIndex internalCellIndex);

extern  void updateFirstFreePreambleIndex(UInt8 preambleIndex, InternalCellIndex internalCellIndex);

extern UInt8 getLastFreePreambleIndex(InternalCellIndex internalCellIndex);

extern UInt8 getCurrNumOfFreePreambles(InternalCellIndex internalCellIndex);

extern  void updateLastFreePreambleIndex(UInt8 preambleIndex, InternalCellIndex internalCellIndex);

extern  void incrementNumberOfFreePrambles(InternalCellIndex internalCellIndex);

extern void decrementNumberOfFreePrambles(InternalCellIndex internalCellIndex);
/* SPR 15909 fix start */
extern ConFreePreambleTimerNode* startConFreePreambleTimer(tickType_t expTTI, UInt8 preambleIndex, InternalCellIndex internalCellIndex);

extern ConBasedPdcchOrderTimerNode* startConBasedPdcchOrderTimer(tickType_t expTTI, UInt16 ueIndex, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */

extern void stopConFreePreambleTimer(ConFreePreambleTimerNode*  preambleTimerNode_p, InternalCellIndex internalCellIndex);

extern void stopConBasedPdcchOrderTimer(ConBasedPdcchOrderTimerNode*  pdcchOrderTimerNode_p, InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */
/* SPR 15909 fix start */
extern void checkContentionFreePreambleExpiry(tickType_t currentTTI, UInt32 listNoneCount, InternalCellIndex internalCellIndex);

extern void checkConBasedPdcchOrderTimerExpiry(tickType_t currentTTI, UInt32 listNoneCount, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */

extern void sendRLFIndToRRC(UInt16 ueIndex, UInt8 rlfCause
        /* CA changes Start */
        ,InternalCellIndex internalCellIndex
        /* CA Changes end */
);
/*SPR_3065_FIX*/
extern void sendSyncIndToRRC(UInt16 ueIndex, UInt16 rnti, SyncIndId syncStatus, InternalCellIndex internalCellIndex);
/*SPR_3065_FIX*/

extern void handOverRelResources(UInt16 ueIndex,UInt16 tcRNTI, InternalCellIndex internalCellIndex);
/*CA Changes end  */

/* SPR 5599 changes start (ZIP ID 129059) */
extern void putEntryInUlSyncLossOnOffTriggerQ(UInt32 ueIndex,
/* SPR 5599 changes end (ZIP ID 129059) */
        UlSyncLossOnOff onOffTrigger,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
        /*CA Changes end  */

/* SPR 5599 changes start (ZIP ID 129059) */
/*CA Changes start  */
extern MacRetType deleteMACDLUEConnHandOverEntity(UInt16 ueIndex, InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* SPR 5599 changes end (ZIP ID 129059) */

#ifdef TDD_CONFIG
    /* This will prepares the contention resolution message and
     * inserts in contentionResolutionMsgQ_g queue
     */
extern void processULConfigForRachQueue(
                                         UInt8 currentSubFrame,
                                         UInt32 currentSFN,
                                         InternalCellIndex internalCellIndex);
/* SPR 3114 Start */
/* SPR 5599 changes start (ZIP ID 129059) */
extern void getULHarqProcessIdForCurrentSubframeForMsg3  ( 
/* SPR 5599 changes end (ZIP ID 129059) */
    UInt8  subframeNumForRAR,
    UInt8  *ulHarqProcessId,
    ULUEContext *ulUEContext_p,
    InternalCellIndex internalCellIndex,
    /* TDD Config 0 and 6 Changes Start */
    UInt16 sysFrameNum
    ,UInt8 ulSubFrameNum
    /* SPR 11257 Changes Start*/
    /*SPR 15909 fix start*/
    ,tickType_t globalTick
    /*SPR 15909 fix end*/
    /* SPR 11257 Changes End */
    );
    /* TDD Config 0 and 6 Changes End */
/* SPR 3114 End */

#endif /* TDD_CONFIG */

void prepareAndsendRARLogPrint(UInt32 flag,
                                RAReqTobeScheduledInPdu* rarPduTobeScheduled_p,
                                /* SPR 15909 fix start */
                                tickType_t currentGlobalTTI,
                                /* SPR 15909 fix end */
                                DLRARsp *dlRARsp_p);
/*CA Changes start  */
void allocateUplinkFailure(RAReqTobeScheduledInPdu* rarPduTobeScheduled_p,
                           InternalCellIndex internalCellIndex);

void freeRARBeforeULRBAllocation(UInt32 flag,
        RAReqTobeScheduledInPdu* rarPduTobeScheduled_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTI,
        /* SPR 15909 fix end */
        DLRARsp *dlRARsp_p, 
        InternalCellIndex internalCellIndex);

void freeRARAfterULRBAllocation(DLRARspQueueNode *raRspQnode_p, 
                                InternalCellIndex internalCellIndex);

void createMemPoolForCell(InternalCellIndex internalCellIndex);
void initUESecificSearchSpaceForCell(InternalCellIndex internalCellIndex);
void allocateUESpecificSearchSpace(InternalCellIndex internalCellIndex);
void deInitRACHMgr(InternalCellIndex internalCellIndex);
void deInitRNTIPoolTbl(InternalCellIndex internalCellIndex);
void cleanupUeSpecificSearchSpace( InternalCellIndex  internalCellIndex);
void cleanupUeSearchSpaceForCell(UInt8 cellIndex,UInt8 ueSearchSpaceIndex );
/*CA Changes end  */
/* SPR 12283 Changes Start */
/* Function is used to cleanup UE Context */
/*spr 13127 fix start*/
void cleanUpUEContext( UInt16 msgId, InternalCellIndex internalCellIndex );
extern void macDeleteScellCtxt(DLUEContext *ueDLContext_p,InternalCellIndex internalCellIndex);
/*spr 13127 fix end*/
/* SPR 12283 Changes End */



#endif 
