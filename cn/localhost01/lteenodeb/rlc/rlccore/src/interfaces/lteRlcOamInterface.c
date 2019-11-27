/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcOamInterface.c,v 1.1.1.1.12.2.2.3 2010/10/25 07:42:47 gur21010 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *                   
 *
 *  1. createRLCMemPools
 *  2. initRLCLayer
 *  3. cleanUpRlcLayer 
 *  4. initAllRLCThreadsAndInterfaces
 *  5. cleanupAllRLCThreadsAndInterfaces
 *  6. prepareOamCnfHeader 
 *  7. rlcOamGetStatCnf
 *  8. validateUeIdxForStatus
 *  9. fillUMStatus
 * 10. fillAMStatus 
 * 11. fillUeStatus
 * 12. rlcOamGetStatusCnf
 * 13. configInitialRlcCfg
 * 14. handleRlcOamInitLayer
 * 15. rlcOamResetStatCnf
 * 16. rlcOamGetLogLevelResp
 * 17. rlcOamGetLogCategoryResp
 * 18. rlcOamEnableLogCategory
 * 19. rlcOamDisableLogcategory
 * 20. fillCommonChannelStatus
 * 21. handleOamRequests
 * 22. setLogLevel
 * 23. getRLCBuildConfiguration
 * 24. prepareRlcInitIndHeader
 * 25. rlcInitFillSCellSpsParams
 * 26. rlcInitValidateSCellParams
 *
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcOamInterface.c,v $
*
*
****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcOamIntf.h"
#include "lteConfigFileHdlr.h"
#include "lteRlcRrcParseUtil.h"
#include "lteLayer2CommanTypes.h"
/*RLC Rel 2.0 Changes Start*/
#include "lteRlcAmdArqOperation.h"
#include "lteRlcRxProcessor.h"
#include "lteRlcTimer.h"
/*RLC Rel 2.0 Changes End*/
#include "logging.h"
#include "lteRlcStat.h"
#include "lteRlcErrorCode.h"
#include "ltePerfStats.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* + Layer2 NON CA Changes */
UInt8 numOfRlcCellConfigured_g = 1;
/* - Layer2 NON CA Changes */
/* +- SPR 17777 */
extern void l2_map_ext_loglevel_to_intern_loglevel(UInt8 *p_log_level);
/* +- SPR 17777 */
/* + SPR 17439 */
inline STATIC void createRLCMemPools(void);

inline STATIC UInt32 initAllRLCThreadsAndInterfaces(void);
/* - SPR 17439 */
inline STATIC UInt32 cleanupAllRLCThreadsAndInterfaces( UInt8  *respBuff,
                                                        UInt32 *sendInd_p
                                                        /* +- SPR 17777 */
                                                  /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                                        ,RrcCellIndex  cellId
#endif
                                                  /* - Layer2 NON CA Changes */
                                                      );

inline STATIC void prepareOamCnfHeader( UInt8 *cnfMsgBuf, 
                                        UInt16 apiId, 
                                        UInt32 *sendIdx_p
                                      );      

inline STATIC UInt32  rlcOamGetStatCnf ( UInt16 transactionId,
                                         UInt16 length
                                       /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                         ,RrcCellIndex  cellIndex
#endif
                                       /* - Layer2 NON CA Changes */
                                       );

inline STATIC UInt32 fillUMStatus ( UInt16 ueIndex,
                                    UInt8  lcId,
                                    lteRlcUEStatusCnf *lteRlcUEStatusCnf_p,
                                    /* SPR 3653 Fix Start */
                                    UInt32 statusIndex
                                    /* SPR 3653 Fix End */
                                   );

inline STATIC UInt32 fillAMStatus ( UInt16 ueIndex,
                                    UInt8 lcId,
                                    lteRlcUEStatusCnf *lteRlcUEStatusCnf_p,
                                    /* SPR 3653 Fix Start */
                                    UInt32 statusIndex
                                    /* SPR 3653 Fix End */
                                  );

inline static void  fillUeStatus (
                                  UInt16 ueIndex,
                                  lteRlcUEStatusCnf * lteRlcUEStatusCnf_p,
                                  /* SPR 3653 Fix Start */
                                  UInt32 statusIndex
                                  /* SPR 3653 Fix End */
                                 );

inline STATIC UInt32 rlcOamGetStatusCnf ( UInt8  *recvBuffer,
                                          UInt16 transactionId
                                          /* +- SPR 17777 */
                                       /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                          ,RrcCellIndex  cellId
#endif
                                       /* - Layer2 NON CA Changes */
                                        );

inline STATIC UInt32 configInitialRlcCfg( 
                                    RlcOamInitLayerReq *RlcOamInitLayerReq_p
                                    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                    ,RrcCellIndex  cellId
#endif
                                    /* - Layer2 NON CA Changes */
                                        );

inline STATIC UInt32 fillRlcInitLayerReqGlobalParams( 
                                       RlcOamInitLayerReq *RlcOamInitLayerReq_p
#ifndef UE_SIM_TESTING
                                       ,InternalCellIndex cellIndex
#endif
                                        );

/* + SPR 17439 */
inline STATIC void createMemPoolRlcNumSupportedUsers(void);
/* - SPR 17439 */
/* +- SPR 17777 */
inline STATIC UInt32 handleRlcOamInitLayer ( UInt8 *respBuff,
                                             UInt32 *sendInd_p,
                                             UInt8 *recvBuff,
                                             UInt16 length
                                           );
/* +- SPR 17777 */

inline STATIC UInt32 rlcOamResetStatCnf( UInt8 *response,
                                         UInt32 sendInd,
                                         UInt16 length
                                       /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                         ,RrcCellIndex  cellId
#endif
                                       /* - Layer2 NON CA Changes */
                                       );
/* FDD_Warning_Fix */
#ifndef UE_SIM_TESTING
inline STATIC void rlcOamGetLogLevelResp(
                                          UInt8 *respBuff,
                                          UInt32 sendInd,
                                       /* + Layer2 NON CA Changes */
                                          RrcCellIndex  cellId
                                       /* - Layer2 NON CA Changes */
                                        );

inline STATIC void rlcOamGetLogCategoryResp (
                                              UInt8 *respBuf,
                                              UInt32 startInd,
                                       /* + Layer2 NON CA Changes */
                                              RrcCellIndex  cellId
                                       /* - Layer2 NON CA Changes */
                                            );

inline static UInt32 handleRlcHealthMonitorStatusReq ( 
                                              UInt8 *respBuff,
                                              UInt32 sendInd 
                                       /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                              ,RrcCellIndex cellId 
#endif
                                            );
    /* - Layer2 NON CA Changes */ 
 
/* +- SPR 17777 */
inline STATIC void rlcOamEnableLogCategory (UInt8 *recvBuff);
inline STATIC void rlcOamDisableLogcategory (UInt8 *recvBuff);
/* +- SPR 17777 */

#endif
/* FDD_Warning_Fix */

/* Coverity Fix 97076 Start */
#ifndef UE_SIM_TESTING
inline static void  setLogLevel(
                                  UInt8  *recvBuffer
                               );
/* Coverity Fix 97075 Start */
inline STATIC UInt16 rlcValidateIntervalList(UInt16 intList[]);                 
/* Coverity Fix 97075 End */
#endif
                                
/* Coverity Fix 97076 End */
inline STATIC UInt32 getRLCBuildConfiguration( UInt8* responseBuffer_p      
                                       /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                               ,RrcCellIndex  cellId
#endif
                                             );
                                       /* - Layer2 NON CA Changes */
/* SPR 5599_129125 Fix Start */
void prepareRlcInitIndHeader ( UInt8 *msg_p,
/* SPR 5599_129125 Fix End */
                               UInt16 apiId,
                               UInt16 *msgLength
                             );
                 
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
/* + stats_review_comments_ipc */
void ueRlcFillPerfStats(UInt8 *cnfBuff, UInt32 *length_p,
                        UInt32 uePerfStatsBitMap, UInt32 *ueIndex,
                        UInt32 periodicFlag, InternalCellIndex cellIndex);
/* - stats_review_comments_ipc */
#endif

#ifdef PERF_STATS
extern void  rlcResetUePerfCounters (LteUePerfStats *pLteUePerfStats);
#endif

/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
void rlcInitFillSCellSpsParams( RlcOamInitLayerReq *RlcOamInitLayerReq_p );

void rlcInitValidateSCellParams( RlcOamInitLayerReq *RlcOamInitLayerReq_p, 
                                   UInt32 *errorCode);
#endif
/* + SPR 17439 */
extern void initRlcProcessRrcOamMsgFSM(void);
/* - SPR 17439 */
/* + stats_review_comments_ipc */
extern UInt16 getUeIdxFromRrcUeIdx(UInt16 rrcUeIdx);
/* - stats_review_comments_ipc */
/* - Layer2 NON CA Changes */

/*****************************************************************************
 * Private Constants
 ****************************************************************************/
/* Start 128 UE: Memory Changes */
#define RLC_PDCP_NUM_DATA_REQ 600 
/* End 128 UE: Memory Changes */

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

#ifdef LTE_EMBMS_SUPPORTED
extern LTE_CIRCQUEUE rlcEmbmsLpToHpQ_g;
extern LTE_CIRCQUEUE rlcEmbmsHpToLpQ_g;
#define MAX_AREA_SUPPORTED                  16
#endif


extern UInt32 activeUEs_g;
extern UEContext* rlcUEContextList_g[MAX_UE_ID];
extern UInt16 rlcLayerInitialized_g;
/* Layer2----->OAM Init Ind Changes */
UInt8 rlcInitInd_g;

UInt16 numOfSupportedUsers_g = 0;
#ifdef LTE_EMBMS_SUPPORTED
UInt16 snFieldLength_g;
#endif

/* Start 128 UE: Memory Changes */
UInt16 rlcNumOfSupportedUsers_g = 0;
/*UInt16 rlcNumOfSupportedAmLC_g = LTE_NUM_OF_SRB + 1; 
UInt16 rlcNumOfSupportedUmLC_g = 0;*/

UInt8  rlcDlMemFac_g           = 0;
UInt8  rlcUlMemFac_g           = 0;
/* SPR 16766 ++ */
#ifdef LOG_PRINT_ENABLED
extern tickType_t getCurrentTick(void);
#endif
/* SPR 16766 -- */
#define RLC_PDCP_NUM_DATA_REQ 600
#define RLC_NACK_FAC 10
#define RLC_BUF_FAC 200
/* End 128 UE: Memory Changes */
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
/*  + Layer2 NON CA Changes*/
/* SPR 15909 fix start */
extern tickType_t rlcGlobalTTITickCount_g[MAX_NUM_CELL];
/* SPR 15909 fix end */
/*  - Layer2 NON CA Changes*/

#ifndef UE_SIM_TESTING
UInt32 rlcHealthMonitoringTimeInterval_g; 
extern Char8 oamModuleHostName[HOSTNAME_MAX_LEN];
extern UInt32 rlcloglevelEnabled_g;
extern UInt64 rlclogCategoryEnabled_g;
/** Rel 4.0 changes Start **/
RlcQosQciInfo rlcQosQciInfo_g[MAX_QCI];
UInt32 qSizeFactorGBR_g;
UInt32 qSizeFactorNGBR_g;
/** Rel 4.0 changes End **/

/* + Layer2 NON CA Changes */
RlcSecondaryCellInfo secondaryCellInfo_g[MAX_NUM_CELL] = {{0}};
/* - Layer2 NON CA Changes */

#endif
void rlcSendMsgToOam(UInt8 *IndBuff_p,
        UInt16 currentLen);


/** SPR 13599 Changes Start **/
RLCHarqFailureInfo  *rlcHarqFailureInfo_g ;
/** SPR 13599 Changes End **/

/***************************************************************************
 * Function Name  : createMemPoolForCircQs
 * Inputs         : nodeNo,
 *                  numOfSupportedLc
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to create  memory pools for circQInit.
 ****************************************************************************/
inline static void createMemPoolForCircQs(UInt32 nodeNo, UInt16 numOfSupportedLc)
{
    UInt32 nodeVal = 2;
    while ( nodeVal <  nodeNo )
    {
        nodeVal = 2*nodeVal;
    }
    createMemPool((sizeof(LTE_CNODE)*nodeVal),(numOfSupportedLc*rlcNumOfSupportedUsers_g));
}


/****************************************************************************
 * Function Name  : createRLCMemPoolForCircQInitAfterCellSetup
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to create  memory pools for circQInit.
 ****************************************************************************/
/* + SPR 17439 */
inline static void createRLCMemPoolForCircQInitAfterCellSetup(void)
/* - SPR 17439 */    
{    
    /* Pool for AmdRawSduQ.pQ */
    /*SPR 20136 +*/
    createMemPoolForCircQs(MAX_AM_RAW_SDU_Q_SIZE, (rlcNumOfSupportedAmLC_g));
    /*SPR 20136 -*/

    /* AmNackListQ.pQ */    
    createMemPoolForCircQs(MAX_NODES, rlcNumOfSupportedAmLC_g);
    
    /* SduTidInfoQ.pQ */
    createMemPoolForCircQs(SDU_TID_Q_SIZE, rlcNumOfSupportedAmLC_g);    

    /* AmRxNackListQ.pQ */ 
    createMemPoolForCircQs(2, rlcNumOfSupportedAmLC_g);

    /* Pool for UmdRawSduQ.pQ for TX UM Entity */
    /*SPR 20136 +*/
    /* SPR 21445 fix start */
    /* Incrementing the count by 2 for Rx and Tx entity in UM mode*/
    createMemPoolForCircQs(MAX_UM_RAW_SDU_Q_SIZE, (rlcNumOfSupportedUmLC_g * 2));
    /* SPR 21445 fix end */
    /*SPR 20136 -*/
    
    /* Pool for UmdRawSduQ.pQ for RX UM Entity */
    createMemPoolForCircQs(MAX_NODES, (rlcNumOfSupportedUmLC_g));
}

/****************************************************************************
 * Function Name  : createRLCMemPools
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to create RLC memory pools.
 ****************************************************************************/
/* + SPR 17439 */
inline STATIC void createRLCMemPools(void)
/* - SPR 17439 */    
{
    /* Start 128 UE: Memory Changes*/
    /* 
    ** 128 UE Memory Changes: Replaced Macros which indicate
    ** the maximum number of UE Supported and LC supported with
    ** corresponding global variables to allocate memory on
    ** the basis of number of UE (sent by OAM)and LC (sent by OAM)
    ** are configured.
    */
    createMemPool(sizeof (UEContext), (rlcNumOfSupportedUsers_g));
    /* 
    ** 128 UE Memory Changes: Replaced Number of Blocks hardcoded value which was 20000
    ** with AM_WINDOW_SIZE * rlcNumOfSupportedUsers_g * rlcNumOfSupportedLC_g
    */
    createMemPool(sizeof (AmReTransmission), ((AM_WINDOW_SIZE * rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g * rlcDlMemFac_g)/100));
    createMemPool(sizeof (RLCEntityInfo), (rlcNumOfSupportedUsers_g * (rlcNumOfSupportedAmLC_g + rlcNumOfSupportedUmLC_g)));
    createMemPool(sizeof (RxUMEntity), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedUmLC_g));
    createMemPool(sizeof (TxUMEntity), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedUmLC_g));

    /* Added missing createMemPools */
    createMemPool(sizeof (AMEntity), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g));
    createMemPool(sizeof (Ring), (rlcNumOfSupportedUsers_g * (rlcNumOfSupportedUmLC_g + rlcNumOfSupportedAmLC_g)));
    /* For AmdRawSduQ */
    createMemPool(sizeof (LTE_CIRCQUEUE), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g));
    /* For AmNackListQ */
    createMemPool(sizeof (LTE_CIRCQUEUE), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g));
    /* For SduTidInfoQ */
    createMemPool(sizeof (LTE_CIRCQUEUE), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g));
    /* For AmRxNackListQ */
    createMemPool(sizeof (LTE_CIRCQUEUE), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g));
    /* Created for maximum AM Window size, it is being used for UM entity also */
    createMemPool((AM_WINDOW_SIZE*sizeof (RingN)), (rlcNumOfSupportedUsers_g * (rlcNumOfSupportedUmLC_g + rlcNumOfSupportedAmLC_g)));
    /* Moved from rlcInitTimerInterface() and 3 for 3 timers */
    createMemPool(sizeof (RlcTimerNode),
                  ((rlcNumOfSupportedUsers_g * (rlcNumOfSupportedUmLC_g + rlcNumOfSupportedAmLC_g) * 3)));

    /*RLC Rel 2.0 Changes Start*/    
    createMemPool(sizeof (RxAMEntity), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g));
    createMemPool(sizeof (TxAMEntity), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g));

    /* Start 128 UE: Memory Changes */
    /* Assuming 2 MB databuffer per LC and size of packet is 1024 */
    createMemPool(sizeof (SduTidInfo), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g * RLC_BUF_FAC));
    /* End 128 UE: Memory Changes */
   
    /* Start 128 UE: Memory Changes */ 
    /* Assuming 2% NACK at RLC */
    createMemPool(sizeof (AmNackListNode), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g * RLC_NACK_FAC));
    /* Assuming 50% of LC have status to be scheduled at any point of time */
    /* SPR 18122 Changes Start*/
    /* SPR 20899 Changes Start*/
    createMemPool(((sizeof (AmRxNackListNode)) + ((sizeof (NackList)) * AM_WINDOW_SIZE)), 
                 ((rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g) - (DIVIDE_BY_FOUR(MULTIPLY_BY_TWO(rlcNumOfSupportedUsers_g * rlcNumOfSupportedAmLC_g)))));
    /* SPR 20899 Changes End*/
    /* SPR 18122 Changes End*/
    /* End 128 UE: Memory Changes */
    /*RLC Rel 2.0 Changes End*/

    
    createMemPool(sizeof (MacUeDataInd), (rlcNumOfSupportedUsers_g));
    createMemPool(sizeof (MacUeOpporInd), (rlcNumOfSupportedUsers_g));
    createMemPool(sizeof (RlcUeDataReq), RLC_PDCP_NUM_DATA_REQ);
    createMemPool(sizeof (RlcDelReEstabQNode), (rlcNumOfSupportedUsers_g * (rlcNumOfSupportedUmLC_g + rlcNumOfSupportedAmLC_g)));
    /*SPR 1834 Start*/
     /* Size Calculate as AM_WINDOW_SIZE * nu. of LC * NO.of UE */
    /* 32 UE attach proper Fix Start */
    createMemPool(sizeof (AmdPdu),(AM_WINDOW_SIZE * rlcNumOfSupportedAmLC_g * rlcNumOfSupportedUsers_g));
    createMemPool(sizeof(LTE_QUEUE), ((AM_WINDOW_SIZE * (rlcNumOfSupportedAmLC_g + rlcNumOfSupportedUmLC_g) * rlcNumOfSupportedUsers_g * rlcUlMemFac_g)/100));
    createMemPool(sizeof(AmdPduSegment), ((AM_WINDOW_SIZE * rlcNumOfSupportedAmLC_g * rlcNumOfSupportedUsers_g * rlcUlMemFac_g)/100));
    /*SPR 1834 End*/
    /* 32 UE attach Temp Fix End */

    /* Memory handling changes start */
    createMemPool(sizeof (RlcPduHeaderInfo), 1);
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    createMemPool(sizeof (RlcPendingRrcOamMsgNode), 1);
#endif    
 /* SPR_9578_Fix Start */
  /*  Memory pool creation for RlcSpsIntervalProfile 
   *  is moved to function  configInitialRlcCfg
   *  where memory is allocating in case of SPS is enbale
   */
 /* SPR_9578_Fix End */
    createMemPool(sizeof (RlcRrcOamHiPrioQNode), 2);
    createMemPool(sizeof (rlcReTxPduNode), MAX_RLC_PDU_PER_TICK * MAX_DL_UE_SCHEDULED); 
    createMemPool(sizeof (UMEntity), (rlcNumOfSupportedUsers_g * rlcNumOfSupportedUmLC_g));
    createMemPool(sizeof (RLCHarqFailureInfo), MAX_LC_ID);
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    createMemPool(sizeof (RlcUeDelQNode), 1);
#endif    
    createMemPool(sizeof (AmdDfeArrInfo), 1);
    /* Memory handling changes end */
    
    /* Memory handling changes start */
    createMemPool((sizeof (lteRlcGetStatCnf) + sizeof(lteRlcUEStats) + OAM_RLC_API_HEADER_LEN), rlcNumOfSupportedUsers_g);
    /* Memory handling changes end */

   createRLCMemPoolForCircQInitAfterCellSetup();
}
#ifdef LTE_EMBMS_SUPPORTED
/****************************************************************************
 * Function Name  :initEmbmsRLCLayer
 * Inputs         :
 * Outputs        :UInt32
 * Returns        :RLC_SUCCESS if initialized successfully RLC_FAILURE otherwise
 * Variables      :
 * Description    :This API is used to initialize Embms RLC circQueues.
 ****************************************************************************/
UInt32 initEmbmsRLCLayer(void)
{
    circQInit(&rlcEmbmsLpToHpQ_g,MAX_AREA_SUPPORTED);
    circQInit(&rlcEmbmsHpToLpQ_g,MAX_AREA_SUPPORTED);
    return RLC_SUCCESS ;
}

/****************************************************************************
 * Function Name  :deInitEmbmsRLCLayer
 * Inputs         :
 * Outputs        :UInt32
 * Returns        :RLC_SUCCESS if initialized successfully RLC_FAILURE otherwise
 * Variables      :
 * Description    :This API is used to initialize Embms RLC circQueues.
 ****************************************************************************/
UInt32 deInitEmbmsRLCLayer(void)
{
    circQDeInit(&rlcEmbmsLpToHpQ_g);
    circQDeInit(&rlcEmbmsHpToLpQ_g);
    return RLC_SUCCESS ;
}

#endif
/****************************************************************************
 * Function Name  : initRLCLayer 
 * Inputs         : None
 * Outputs        : None
 * Returns        : RLC_SUCCESS if initialized successfully RLC_FAILURE otherwise
 * Description    : This API is used to initialize RRC interface and thread.
 ****************************************************************************/
/* SPR 5599_128125 Fix Start */
/* + SPR 17439 */
UInt32 initRLCLayer(void)
/* - SPR 17439 */    
/* SPR 5599_128125 Fix End */
{
    /* Start 128 UE: Memory Changes */
    /* 
    ** Create the Memory pool on the receipt of
    ** RLC_INIT_LAYER_REQ from OAM.
    */
    /* End 128 UE: Memory Changes */

    /* CA changes start */
    /* Init RLC state machine */
    initRlcProcessRrcOamMsgFSM();
    /* CA changes end */

    /* initializing the RRC interface thread */
    /* creating socket to send/receive data to/from RRC layer */
    createRLCSockets();
    initRlcStats();
#ifdef RLC_STANDALONE_SETUP
    initRRCInterfaceThread(RLC_NULL) ;
#endif

#ifdef LTE_EMBMS_SUPPORTED
    initEmbmsRLCLayer();
#endif

    return RLC_SUCCESS ;
}

/****************************************************************************
 * Function Name  : cleanupRLCLayer 
 * Inputs         : None
 * Outputs        : None
 * Returns        : RLC_SUCCESS if deinitialized successfully RLC_FAILURE
 *                  otherwise
 * Description    : This API is used to deinitialize RRC interface and thread.
 ****************************************************************************/

/* SPR 5599_128125 Fix Start */
/* + SPR 17439 */
UInt32 cleanUpRlcLayer(void)
/* - SPR 17439 */    
/* SPR 5599_128125 Fix End */
{
    /* destroying socket to send/receive data to/from RRC layer */
    destroySockets();
#ifdef RLC_STANDALONE_SETUP
    deInitRRCInterfaceThread();
#endif    

#ifdef LTE_EMBMS_SUPPORTED
    deInitEmbmsRLCLayer();
#endif

    return RLC_SUCCESS ;
}

/****************************************************************************
 * Function Name  : initAllRLCThreadsAndInterfaces 
 * Inputs         : None
 * Outputs        : None
 * Returns        : RLC_SUCCESS or ErrorCode 
 * Description    : This API is used to initialize all interfaces, threads and db
 *                  except for the RRC thread and interface
 *
 *                  MAC Interface Queues
 *                  a) macUeDataIndQ_g
 *                  b) macUeOpporIndQ_g
 *                  c) macComChDataIndQ_g
 *                  d) macComChOpporIndQ_g
 *                  e) rlcControlPduQ_g
 *                  f) amdRawPduIndQ_g
 *                  g) macUeHarqFailureIndQ_g
 *
 *                  RRC Interface Queues
 *                  a) rlcRxPendingDelQ_g
 *                  b) rlcTxPendingDelQ_g
 *                  c) rlcRxReEstabQ_g
 *                  d) rlcTxReEstabQ_g
 *                  e)  semInit(&processDelSem_g,1)
 *
 *                  PDCP Interface Queues
 *                  a) rlcUeDataReqQ_g 
 *                  
 *                  Optional Three Thread RX, TX, Timer threads.
 ****************************************************************************/
/* + SPR 17439 */
inline STATIC UInt32 initAllRLCThreadsAndInterfaces(void)
/* - SPR 17439 */    
{

#ifdef RLC_STANDALONE_SETUP 
    UInt32 errorCode = INVALID_ERR_CODE;

    errorCode = rlcInitRxThread(RLC_NULL) ;
    if ( ZERO != errorCode )
    {
        /* Thread Creation is failed */
        return errorCode;
    }

#endif   
    rlcDbInitDB() ;
    
    rlcInitMacInterface() ;
    rlcInitRrcInterface();

    rlcInitPdcpInterface() ;
    rlcInitTimerInterface();

    rlcLayerInitialized_g = RLC_SUCCESS;
     
    return RLC_SUCCESS ;
}

/****************************************************************************
 * Function Name  : cleanupAllRLCThreadsAndInterfaces
 * Inputs         : respBuff,
 *                  sendInd,
 *                  length
 *                  External cellId
 * Outputs        : None
 * Returns        : RLC_SUCCESS if deinitialized successfully RLC_FAILURE otherwise 
 * Description    : This API is used to deinitialize all interfaces, threads and db
                    except for the RRC thread and interface.
 ****************************************************************************/
inline STATIC UInt32 cleanupAllRLCThreadsAndInterfaces( UInt8  *respBuff,
                                                        UInt32 *sendInd_p
                                                        /* +- SPR 17777 */
                                                  /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                                       ,RrcCellIndex cellId
#endif
                                                  /* - Layer2 NON CA Changes */
                                                      )
{
    UInt32 errorCode  = INVALID_ERR_CODE;
    UInt16 cleanupRes = 0;
    UInt32 sendInd    = *sendInd_p;

     /* Commenting clean up handling and returning failure as this API
      * is not supported in 6.0.
      */
    /*
    if ( RLC_SUCCESS != rlcLayerInitialized_g )
    {
        errorCode = cleanupRes = RLC_LAYER_NOT_INITIALIZED;

        LOG_RLC_MSG ( LTE_RLC_OAM_INTERFACE_CLEANUP_ERR, LOGWARNING, RLC_OAM, 
                globalTTITickCount_g, __LINE__, sendInd, length, errorCode, 0,
                0, 0, __func__, "");

    }
    else if( RLC_API_HEADER_LEN > length )
    {
        cleanupRes = RLC_SYNTAX_ERROR;
        errorCode  = RLC_OAM_INVALID_CLEANUP_LAYER_REQ_LEN;

        LOG_RLC_MSG ( LTE_RLC_OAM_INTERFACE_CLEANUP_ERR_1, LOGWARNING, RLC_OAM, 
                globalTTITickCount_g, __LINE__, sendInd, length, errorCode, 0,
                0, 0, __func__, "");

    }
    else
    {
        rlcLayerInitialized_g = RLC_FAILURE;
        rlcDbDeInitDB();

#ifdef RLC_STANDALONE_SETUP
        rlcDeInitTmThread();
        rlcDeInitRxThread();
        rlcDeInitTxThread();
#endif        
        rlcDeInitMacInterface() ;
        rlcDeInitRrcInterface();
        rlcDeInitPdcpInterface() ;
        rlcDeInitTimerInterface();

#ifndef UE_SIM_TESTING
        InternalCellIndex internalCellIndex = layer2CommonGetInternalCellIndex(cellId);
#else
        InternalCellIndex internalCellIndex = 0;
#endif
        if (MAX_INTERNAL_CELL_INDEX >= internalCellIndex)
        {
            rlcResetStat(internalCellIndex);
        }

        cleanupRes = RLC_SUCCESS;
        errorCode  = RLC_SUCCESS;
    }*/

    cleanupRes = RLC_FAILURE;
    errorCode  = RLC_FAILURE;
    prepareOamCnfHeader(respBuff, RLC_CLEANUP_LAYER_CNF, &sendInd);
    /*SPR 6641 FIX Start*/
    sendInd = RLC_API_HEADER_LEN;
    /*SPR 6641 FIX End*/

#ifndef UE_SIM_TESTING
    /* + Layer2 NON CA Changes */
    respBuff[API_HEADER_CELL_INDEX_OFFSET] = cellId;
    /* - Layer2 NON CA Changes */
#endif
    LTE_SET_U16BIT((respBuff + sendInd), cleanupRes);
    sendInd += 2;
    LTE_SET_U16BIT( (respBuff + API_HEADER_MSGLEN_OFFSET), sendInd);

    *sendInd_p = sendInd;

    return errorCode;
}

/****************************************************************************
 * Function Name  : prepareOamCnfHeader
 * Inputs         : cnfMsgBuf - Pointer to response buffer,
 *                  apiId - API ID for the confirmation,
 *                  sendIdx_p - Pointer to current idx in response buffer
 * Outputs        : None
 * Returns        : None 
 * Description    : This API is used to prepare a message header for the 
 *                  confirmation message to be send to OAM. 
 ****************************************************************************/

inline STATIC void prepareOamCnfHeader( UInt8 *cnfMsgBuf, 
                                        UInt16 apiId, 
                                        UInt32 *sendIdx_p
                                      )
{
    UInt16 current = *sendIdx_p;

    LTE_SET_U16BIT(&cnfMsgBuf[current],RLC_MODULE_ID);
    current += 2;
    LTE_SET_U16BIT(&cnfMsgBuf[current],OAM_MODULE_ID);
    current += 2;
    LTE_SET_U16BIT(&cnfMsgBuf[current],apiId);
    current += 4;
    *sendIdx_p = current;
}

/****************************************************************************
 * Function Name  : rlcOamGetStatCnf
 * Inputs         : transactionId - Its recieved from GET STAT REQ msg,
 *                  length        - length of the recieved msg,
 *                  cellId - RRC CELL INDEX
 * Outputs        : None 
 * Returns        : RLC_SUCCESS/ErrorCode
 * Description    : This API is used to get the TM, UM, AM stats and fill it 
 *                  in the response buffer :-
 *                 1. Calculate the size and get memory from the memory pool 
 *                    16for Response buffer
 *                 2. Fill lteRlcGetStatCnf structure for TM, UM, AM Stats
 *                 3. Send the Response buffer to OAM Layer.
 *
 *****************************************************************************/
inline STATIC UInt32  rlcOamGetStatCnf( UInt16 transactionId,
        UInt16 length
                                        /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
        ,RrcCellIndex cellId
#endif
                                      /* - Layer2 NON CA Changes */
        )
{
    UInt8 *respBuffer_p = PNULL;
    UInt8 *cnfBuffer_p = PNULL;
    UInt32 size         = 0;
    UInt32 index        = 0;
    UInt32 sendIndex    = OAM_RLC_API_HEADER_LEN;
    UInt32 errorCode    = INVALID_ERR_CODE;
    UInt32 idx          = 0; 
    /* SPR 3918 Fix start */
    SInt32 activeUe     = 0;
    UInt32 statsId      = 0;
    /* SPR 3918 Fix End */

    lteRlcGetStatCnf *lteRlcGetStatCnf_p = PNULL;
    /* + PERF_CA GBL */
#ifndef UE_SIM_TESTING
    InternalCellIndex internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);
#else
    InternalCellIndex internalCellIndex = 0;
#endif
    /* - coverity fixes 62621 */
    if(MAX_INTERNAL_CELL_INDEX >= internalCellIndex)
    {	
        if (lteRlcStatInfo_g[internalCellIndex].lteUEStat.numOfActiveUE > 1)
        {
            size = ((((lteRlcStatInfo_g[internalCellIndex].lteUEStat.numOfActiveUE)-1 ) * sizeof(lteRlcUEStats)) +
                    sizeof (lteRlcGetStatCnf) + OAM_RLC_API_HEADER_LEN);
        }
        /* + PERF_CA GBL */
        else
        {
            size = sizeof (lteRlcGetStatCnf) + OAM_RLC_API_HEADER_LEN;
        }
    }
    else
    {
        errorCode = INVALID_CELL_INDEX ;
        return errorCode;
    }
    /* - coverity fixes 62621 */

    respBuffer_p = (UInt8 *)getMemFromPool(size, RLC_NULL);
    if ( RLC_NULL == respBuffer_p ) 
    {
        LOG_RLC_MSG ( LTE_RLC_OAM_GET_STATS_ERR_1, LOGFATAL, (RLC_OAM), 
                globalTTITickCount_g, __LINE__, transactionId, length, size, errorCode, 0, 
                0, __func__, "");

        return errorCode;
    }

    memSet(respBuffer_p,0,size);

    cnfBuffer_p = respBuffer_p ;

    lteRlcGetStatCnf_p = (lteRlcGetStatCnf *)(cnfBuffer_p + OAM_RLC_API_HEADER_LEN);

    if ( RLC_SUCCESS != rlcLayerInitialized_g)
    {
        lteRlcGetStatCnf_p->responseCode = RLC_LAYER_NOT_INITIALIZED;

        sendIndex += 2; 
        errorCode = RLC_LAYER_NOT_INITIALIZED;

        LOG_RLC_MSG ( LTE_RLC_OAM_GET_STATS_ERR_2, LOGWARNING, (RLC_OAM), 
                globalTTITickCount_g, __LINE__, transactionId, length, size, errorCode, 0, 
                0, __func__, "");

    }
    else 
    {
        lteRlcGetStatCnf_p->responseCode = RLC_SUCCESS;
        sendIndex += 2;


        lteRlcUMStatInfo *lteUMStat_p = &(lteRlcGetStatCnf_p->lteUMStat);
        lteRlcAMStatInfo *lteAMStat_p = &(lteRlcGetStatCnf_p->lteAMStat);

        /* + PERF_CA GBL */
        lteRlcUMStatInfo *umStat_p  = &(lteRlcStatInfo_g[internalCellIndex].lteUMStat);
        lteRlcAMStatInfo *amStat_p  = &(lteRlcStatInfo_g[internalCellIndex].lteAMStat);
        /* - PERF_CA GBL */

        /* Transparent Mode (TM) stats */
        sendIndex += sizeof(lteRlcTMStatInfo);

        /* Unacknowledgement Mode ( UM) stats */
        lteUMStat_p->rxUmdSduDiscarded = umStat_p->rxUmdSduDiscarded;
        lteUMStat_p->txUmdPdu          = umStat_p->txUmdPdu;
        lteUMStat_p->rxUmdSduTxUL      = umStat_p->rxUmdSduTxUL;
        lteUMStat_p->rxIncompleteUmdSduDiscarded = umStat_p->rxIncompleteUmdSduDiscarded;
        lteUMStat_p->rxUmdPdu          = umStat_p->rxUmdPdu;
        lteUMStat_p->rxUmdPduDropped   = umStat_p->rxUmdPduDropped;
        lteUMStat_p->umdSduReceivedFromPdcp = umStat_p->umdSduReceivedFromPdcp;
        lteUMStat_p->umdSduTransmittedToMac = umStat_p->umdSduTransmittedToMac;
        sendIndex += sizeof(lteRlcUMStatInfo);

        /* Acknowledgement Mode (AM) Stats */ 
        lteAMStat_p->amdPduTransmitted   = amStat_p->amdPduTransmitted;
        lteAMStat_p->amdPduReceived      = amStat_p->amdPduReceived;
        lteAMStat_p->amdPduDropped       = amStat_p->amdPduDropped;
        lteAMStat_p->amdIncompSduDiscard = amStat_p->amdIncompSduDiscard;
        lteAMStat_p->amdSduReceivedFromPdcp  = amStat_p->amdSduReceivedFromPdcp;
        lteAMStat_p->amdSduTransmittedToPdcp = amStat_p->amdSduTransmittedToPdcp;
        lteAMStat_p->amdSduDropped           = amStat_p->amdSduDropped;
        lteAMStat_p->amdSduTransmitedToMac   = amStat_p->amdSduTransmitedToMac;
        sendIndex += sizeof(lteRlcAMStatInfo);

        /* Filling RLC_UE_STATS */
        /* SPR 3918 Fix start */
        activeUe = activeUEs_g;
        /* SPR 3918 Fix End */
        sendIndex += 2;
        lteRlcUEStats *lteRlcUEStats_p = RLC_NULL;
        lteRlcUEStats *rlcUEStats_p    = RLC_NULL;
        /* SPR 3918 Fix Start */
        for ( (index =0,statsId = 0);
                ((index < MAX_UE_SUPPORTED) && (activeUe != 0));
                index ++)
        {
            /* If UE context is valid and exists on same cell */
            if((rlcUEContextList_g[index]) &&
               (internalCellIndex == rlcUEContextList_g[index]->cellIndex))
            {
                /* SPR 3918 Fix End */
                lteRlcUEStats_p = (lteRlcUEStats *)
                    &(lteRlcGetStatCnf_p->lteUEStat.rlcUeStats[statsId]);
                /* + PERF_CA GBL */
                rlcUEStats_p = (lteRlcUEStats *)
                    &(lteRlcStatInfo_g[internalCellIndex].lteUEStat.rlcUeStats[statsId]); 
                /* - PERF_CA GBL */

                lteRlcUEStats_p->crnti = rlcUEContextList_g[index]->ueId;

                lteRlcUEStats_p->sduReceivedFromPdcp  =
                    rlcUEStats_p->sduReceivedFromPdcp;
                lteRlcUEStats_p->pduTransmittedToMac  =
                    rlcUEStats_p->pduTransmittedToMac;
                lteRlcUEStats_p->sduTransmittedToPdcp =
                    rlcUEStats_p->sduTransmittedToPdcp;
                lteRlcUEStats_p->pduReceivedFromMac   =
                    rlcUEStats_p->pduReceivedFromMac;
                sendIndex += sizeof(lteRlcUEStats);
                /* SPR 3918 Fix Start */
                activeUe--;
                statsId++;
            }
            /* SPR 3918 Fix End */
        }
        lteRlcGetStatCnf_p->lteUEStat.numOfActiveUE = statsId;
        errorCode = RLC_SUCCESS;
    }
    /* SPR 3800 Fix start */
    sendIndex += 4;
    /* SPR 3800 Fix End */
    LTE_SET_U16BIT((cnfBuffer_p + idx),transactionId);
    idx += 2;
    prepareOamCnfHeader(cnfBuffer_p, RLC_GET_STATS_CNF, &idx);
#ifndef UE_SIM_TESTING
    /* + Layer2 NON CA Changes */
    cnfBuffer_p[API_HEADER_CELL_INDEX_OFFSET]= cellId;
    /* - Layer2 NON CA Changes */
#endif
    LTE_SET_U16BIT( (cnfBuffer_p + API_HEADER_MSGLEN_OFFSET), sendIndex);
    rlcSendMsgToOam( respBuffer_p,
            sendIndex );
    /* SPR 3943 Fix Start */
    freeMemPool(respBuffer_p);
    /* SPR 3943 Fix End */
    return errorCode;    
}

/****************************************************************************
 * Function Name  : validateUeIdxForStatus 
 * Inputs         : ueIdx - UE ID,
 *                  transactionId
 *                  cellId - RRC CELL INDEX
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode
 * Description    : This API is used to validate the ueIndex and send 
                    appropriate response. 
 *****************************************************************************/

inline STATIC UInt32 validateUeIdxForStatus ( UInt16 ueIdx, 
                                              UInt16 transactionId 
                                              /* + Layer2 NON CA Changes */
                                              /* coverity Fix 60266 starts */
#ifndef UE_SIM_TESTING
                                              ,RrcCellIndex cellId
#endif
                                              /* coverity Fix 60266 ends */
                                              /* - Layer2 NON CA Changes */

        )
{
    UInt32 sendInd = 0;
    UInt32 errorCode = INVALID_ERR_CODE;
    /*SPR8968 Fix Start*/
    /* if UE index is not in valid range */
    if ( ( ueIdx >= MAX_UE_ID )          || 
         /*SPR8968 Fix End*/            
         /* if UE context does not exists at RLC */
         ( !(rlcUEContextList_g[ueIdx])) || 
         /* UE state is not active */
         ( RLC_UECONTEXT_ACTIVE != rlcUEContextList_g[ueIdx]->rlcUEState )
#ifndef UE_SIM_TESTING
         ||
         /* Cell index does not match with cell where the UE was attached */
         ( RLC_GET_INTERNAL_CELL_INDEX(cellId) != (rlcUEContextList_g[ueIdx])->cellIndex)
#endif
         )
    {
        UInt8 response[20] = {0};
        UInt8 *respBuffer_p = PNULL;
        respBuffer_p = response; 
        lteRlcUEStatusCnf *lteRlcUEStatusCnf_p =
            (lteRlcUEStatusCnf *)(respBuffer_p + OAM_RLC_API_HEADER_LEN);
        LTE_SET_U16BIT((respBuffer_p + sendInd), transactionId);
        sendInd += 2;
        prepareOamCnfHeader(respBuffer_p , RLC_GET_STATUS_CNF, &sendInd);

        /*SPR 6641 FIX Start*/
        sendInd = RLC_API_HEADER_LEN;
#ifndef UE_SIM_TESTING
        /* + Layer2 NON CA Changes */
        /* coverity Fix 60266 starts */
        respBuffer_p[API_HEADER_CELL_INDEX_OFFSET]= cellId ;
        /* coverity Fix 60266 ends */
        /* - Layer2 NON CA Changes */
#endif
        /*SPR 6641 FIX End*/

        errorCode = lteRlcUEStatusCnf_p->responseCode = RLC_NO_ACTIVE_UE_IN_SYSTEM;
        sendInd += 2;

        LTE_SET_U16BIT( (respBuffer_p + API_HEADER_MSGLEN_OFFSET), sendInd);
        rlcSendMsgToOam( response,
        sendInd);
        return errorCode;
    }

    return RLC_SUCCESS;

}


#define VALIDATE_UM_TX_ENTITY(ueIdx)\
 ((RLC_NULL != (rlcUEContextList_g[ueIndex]-> \
                 rlcEntityInfoList[lcId]-> \
                 rlcEntity.umEntity_p->txUMEntity_p)) ?\
  (TRUE):(FALSE))               

#define VALIDATE_UM_RX_ENTITY(ueIdx)\
((RLC_NULL != (rlcUEContextList_g[ueIndex]-> \
                rlcEntityInfoList[lcId]-> \
                rlcEntity.umEntity_p->rxUMEntity_p)) ?\
                (TRUE) : (FALSE))
                
/****************************************************************************
 * Function Name  : fillUMStatus 
 * Inputs         : ueIdx,
 *                  lcId,
 *                  lteRlcUEStatusCnf_p,
 *                  statusIndex 
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode
 * Description    : This API is used to validate the ueIndex and send
 *                  appropriate response.
 *****************************************************************************/

inline STATIC UInt32 fillUMStatus( UInt16 ueIndex,
                                   UInt8 lcId,
                                   lteRlcUEStatusCnf *lteRlcUEStatusCnf_p,
                                   /* SPR 3653 change start */
                                   UInt32 statusIndex
                                   /* SPR 3653 change End */
                                 )
{
    if( VALIDATE_UM_TX_ENTITY (ueIndex)) 
    {
        /* SPR 3653 change start */
        UInt8 lcIndex = lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].\
                        numOfTxUMEntities;
        lteRlcTxUMEntityStatus *lteRlcTxUMEntityStatus_p = &( 
                lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].\
                rlcTxUMEntityStatus[lcIndex]);                            
        /* SPR 3653 change End */
        TxUMEntity* txUMEntity_p =  
            (rlcUEContextList_g[ueIndex]->rlcEntityInfoList[lcId]->\
            rlcEntity.umEntity_p->txUMEntity_p);

        lteRlcTxUMEntityStatus_p->lcId          = txUMEntity_p->lcId;
        lteRlcTxUMEntityStatus_p->snFieldLength = txUMEntity_p->snFieldLength;
        lteRlcTxUMEntityStatus_p->vtUS          = txUMEntity_p->US;
        /* SPR 3653 change start */
        lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].numOfTxUMEntities++;
        /* SPR 3653 change End */
    }
    else
    {
        /* SPR 3653 change start */
        LOG_RLC_MSG(FILL_UM_STATUS_ERR, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, ueIndex,
                statusIndex, lcId, 0, 0, 0, __func__, "");
        /* SPR 3653 change End */
    }

    if ( VALIDATE_UM_RX_ENTITY(ueIndex))
    {

        /* SPR 3653 change start */
        UInt8 lcIndex = lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].numOfRxUMEntities;
        lteRlcRxUMEntityStatus *lteRlcRxUMEntityStatus_p =
            &(lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].\
            rlcRxUMEntityStatus[lcIndex]);
        /* SPR 3653 change End */
        RxUMEntity* rxUMEntity_p =  rlcUEContextList_g[ueIndex]->rlcEntityInfoList[lcId]->
            rlcEntity.umEntity_p->rxUMEntity_p;

        lteRlcRxUMEntityStatus_p->lcId          = rxUMEntity_p->lcId;
        lteRlcRxUMEntityStatus_p->snFieldLength = rxUMEntity_p->snFieldLength;
        /* SPR 3653 change start */
        lteRlcRxUMEntityStatus_p->tReordering   = rxUMEntity_p->umROTimerInfo.roDuration;
        /* SPR 3653 change start */
        lteRlcRxUMEntityStatus_p->vrUR          = rxUMEntity_p->UR;
        lteRlcRxUMEntityStatus_p->vrUH          = rxUMEntity_p->UH;
        lteRlcRxUMEntityStatus_p->vrUX          = rxUMEntity_p->UX;

        /* SPR 3653 change start */
        lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].numOfRxUMEntities++;
        /* SPR 3653 change start */
    }
    else
    {
        /* SPR 3653 change start */
        LOG_RLC_MSG(FILL_UM_STATUS_ERR_1, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, ueIndex,
                 statusIndex, lcId, 0, 0, 0, __func__,"");
        /* SPR 3653 change start */
    }

    return RLC_SUCCESS;
}

#define VALIDATE_AM_TX_ENTITY(ueIdx)\
((RLC_NULL != (rlcUEContextList_g[ueIndex]-> \
                rlcEntityInfoList[lcId]-> \
               rlcEntity.amEntity_p->txAMEntity_p)) ?\
          (TRUE) : (FALSE))               

#define VALIDATE_AM_RX_ENTITY(ueIdx)\
(( RLC_NULL != (rlcUEContextList_g[ueIndex]-> \
               rlcEntityInfoList[lcId]-> \
          rlcEntity.amEntity_p->rxAMEntity_p)) ?\
  (TRUE) : (FALSE) )
/****************************************************************************
 * Function Name  : fillAMStatus
 * Inputs         : ueIndex,
 *                  lcId,
 *                  statusIndex 
 * Outputs        : lteRlcUEStatusCnf_p
 * Returns        : RLC_SUCCESS/ErrorCode
 * Description    : This API is used to trasnmitted AM status.
 *****************************************************************************/
inline STATIC UInt32 fillAMStatus ( UInt16 ueIndex,
                                    UInt8 lcId,
                                    lteRlcUEStatusCnf *lteRlcUEStatusCnf_p,
                                    /* SPR 3653 Fix Start */
                                    UInt32 statusIndex
                                    /* SPR 3653 Fix End */
                                  )
{
    if ( VALIDATE_AM_TX_ENTITY(ueIndex))
    {
        /* SPR 3653 Fix Start */
        UInt8 lcIndex = lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].numOfTxAMEntities;
        lteRlcTxAMEntityStatus *lteRlcTxAMEntityStatus_p = 
            &(lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].rlcTxAMEntityStatus[lcIndex]); 
        /* SPR 3653 Fix End */
        TxAMEntity* txAMEntity_p = 
            rlcUEContextList_g[ueIndex]->rlcEntityInfoList[lcId]->\
            rlcEntity.amEntity_p->txAMEntity_p;

        lteRlcTxAMEntityStatus_p->lcId   = txAMEntity_p->lcId;
        lteRlcTxAMEntityStatus_p->vtA    = txAMEntity_p->vtA;
        lteRlcTxAMEntityStatus_p->vtMS   = txAMEntity_p->vtMS;
        lteRlcTxAMEntityStatus_p->vtS    = txAMEntity_p->vtS;
        lteRlcTxAMEntityStatus_p->pollSN = txAMEntity_p->pollSN;
        lteRlcTxAMEntityStatus_p->pduWithoutPoll   = txAMEntity_p->pduWithoutPoll;
        lteRlcTxAMEntityStatus_p->maxRetxThreshold = txAMEntity_p->maxRetxThreshold;
        lteRlcTxAMEntityStatus_p->pollPDU          = txAMEntity_p->pollPDU;
        lteRlcTxAMEntityStatus_p->pollByte         = txAMEntity_p->pollByte;
        lteRlcTxAMEntityStatus_p->istRetransmitRunning = txAMEntity_p->istRetransmitTimerRunning;
        lteRlcTxAMEntityStatus_p->txQueueSize          = txAMEntity_p->txQueueSize;
        lteRlcTxAMEntityStatus_p->reTxQueueSize        = txAMEntity_p->reTxQueueSize;
        lteRlcTxAMEntityStatus_p->rxStatusPduSize      = txAMEntity_p->rxStatusPduSize;
        /* SPR 3653 Fix Start */
        lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].numOfTxAMEntities++;
        /* SPR 3653 Fix End */
    }

    else
    {
        /* SPR 3653 Fix Start */
        LOG_RLC_MSG(FILL_AM_STATUS_ERR, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, ueIndex,
                 statusIndex, lcId, 0, 0, 0, __func__, "");
        /* SPR 3653 Fix End */
    }
    
    if( VALIDATE_AM_RX_ENTITY (ueIndex)) 
    {
        /* SPR 3653 Fix Start */
        UInt8 lcIndex = lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].numOfRxAMEntities;
        lteRlcRxAMEntityStatus *lteRlcRxAMEntityStatus_p =
            &(lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].rlcRxAMEntityStatus[lcIndex]);
        /* SPR 3653 Fix Start */
        RxAMEntity* rxAMEntity_p = 
            rlcUEContextList_g[ueIndex]->rlcEntityInfoList[lcId]->\
            rlcEntity.amEntity_p->rxAMEntity_p;

        lteRlcRxAMEntityStatus_p->lcId          = rxAMEntity_p->lcId;
        lteRlcRxAMEntityStatus_p->sendStatusInd = rxAMEntity_p->sendStatusInd;
        lteRlcRxAMEntityStatus_p->vrR           = rxAMEntity_p->vrR;
        lteRlcRxAMEntityStatus_p->vrMR          = rxAMEntity_p->vrMR;
        lteRlcRxAMEntityStatus_p->vrX           = rxAMEntity_p->vrX;
        lteRlcRxAMEntityStatus_p->vrMS          = rxAMEntity_p->vrMS;
        lteRlcRxAMEntityStatus_p->vrH           = rxAMEntity_p->vrH;
        lteRlcRxAMEntityStatus_p->isRTimerRunning = rxAMEntity_p->isRTimerRunning;

        /* SPR 3653 Fix Start */
        lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].numOfRxAMEntities++;
        /* SPR 3653 Fix Start */
    }

    else
    {
        /* SPR 3653 Fix Start */
        LOG_RLC_MSG(FILL_AM_STATUS_ERR_1, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, ueIndex,
                statusIndex, lcId, 0, 0, 0, __func__, "");
        /* SPR 3653 Fix Start */
    }

    return RLC_SUCCESS;
}


/* Validate the UeIdx is exist and active*/
#define VALIDATE_UE_IDX(ueIndex)\
(((rlcUEContextList_g[ueIndex]) && \
   (RLC_UECONTEXT_ACTIVE == rlcUEContextList_g[ueIndex]->rlcUEState))? \
   (TRUE) : (FALSE)) 
/****************************************************************************
 * Function Name  : fillUeStatus 
 * Inputs         : ueIndex,
 *                  lteRlcUEStatusCnf_p,
 *                  statusIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to
 *                   1. Validate the OAM Init Parameter
 *                   2. fill the values in global variables.
 *
 *******************************************************************************/

inline static void fillUeStatus(
                               UInt16 ueIndex,
                               lteRlcUEStatusCnf * lteRlcUEStatusCnf_p,
                               /* SPR 3653 Fix Start */
                               UInt32 statusIndex
                               /* SPR 3653 Fix End */
                            )
{
    UInt8 lcId = 0;
    UInt8 mode = 0;

    /* SPR 3653 Fix Start */
    lteRlcUEStatusCnf_p->rlcUEStatus[statusIndex].crnti = 
                       rlcUEContextList_g[ueIndex]->ueId;
    /* SPR 3653 Fix End */

    for ( lcId = 0; lcId < MAX_LC_ID; lcId++)
    {
        if(RLC_NULL != rlcUEContextList_g[ueIndex]->rlcEntityInfoList[lcId] )
        {
            mode = rlcUEContextList_g[ueIndex]->
                rlcEntityInfoList[lcId]->rlcEntityMode;

            switch(mode)
            {
                case RLC_ENTITY_UM:
                    {  
                        /* SPR 3653 Fix Start */
                        fillUMStatus(ueIndex, lcId, lteRlcUEStatusCnf_p, statusIndex);
                        /* SPR 3653 Fix End */
                    }
                    break;
                case RLC_ENTITY_AM:
                    {
                        /* SPR 3653 Fix Start */
                        fillAMStatus(ueIndex, lcId, lteRlcUEStatusCnf_p, statusIndex);
                        /* SPR 3653 Fix End */
                    }
                    break;
                default:
                    /* SPR 3653 Fix Start */
                    LOG_RLC_MSG(FILL_UE_STATUS_ERR, LOGWARNING, (RLC_OAM),
                            globalTTITickCount_g, __LINE__, ueIndex,
                             statusIndex, mode, 0, 0, 0, __func__,"");
                    /* SPR 3653 Fix End */
                    break;
            }
        }
    }
    return;
}
/****************************************************************************
 * Function Name  : rlcOamGetStatusCnf
 * Inputs         : recvBuffer,
 *                  transactionId,
 *                  length,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : RLC_SUCCESS/errorCode
 * Description    : This API is used to send appropriate response and
 *                  fill it in the response buffer.
 ******************************************************************************/
inline STATIC UInt32 rlcOamGetStatusCnf ( UInt8  *recvBuffer,
                                          UInt16 transactionId
                                          /* +- SPR 17777 */
                                       /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                         ,RrcCellIndex cellId
#endif
                                       /* - Layer2 NON CA Changes */
                                        )
{
    UInt8 isUeIndexPresent = 0;
    UInt16 ueIndexStart    = 0;
    /* + stats_review_comments_ipc */
    UInt16 rrcUeIndex      = 0;
    /* - stats_review_comments_ipc */
    UInt32 errorCode       = 0;
    UInt32 sendIndex       = 0;
    UInt8 *respBuffer_p    = PNULL;
    UInt8 *cnfBuffer_p     = PNULL;
    UInt32 size            = 0;

    isUeIndexPresent = ((lteRlcUEStatusReq *)
            (recvBuffer + OAM_RLC_API_HEADER_LEN))->isUEIndexPresent;

    if ( RLC_TRUE == isUeIndexPresent ) 
    {
        /* + stats_review_comments_ipc */
        rrcUeIndex = ((lteRlcUEStatusReq *)(recvBuffer + OAM_RLC_API_HEADER_LEN))->ueIndex;
        /* + Layer2 NON CA Changes */
        ueIndexStart = getUeIdxFromRrcUeIdx(rrcUeIndex);             
        /* - stats_review_comments_ipc */
        /* coverity Fix 60266 starts */
        errorCode = validateUeIdxForStatus (ueIndexStart, transactionId
#ifndef UE_SIM_TESTING
                ,cellId
#endif
                );
        /* coverity Fix 60266 ends */

        /* - Layer2 NON CA Changes */

        if ( RLC_SUCCESS == errorCode )
        {
            size = sizeof (lteRlcUEStatusCnf ) + OAM_RLC_API_HEADER_LEN;
        }
        else
        {
            LOG_RLC_MSG( LTE_RLC_OAM_STATUS_ERR, LOGWARNING, (RLC_OAM),
                    globalTTITickCount_g, __LINE__, transactionId, ueIndexStart, errorCode, 0, 0, 0, __func__,
                    "");
            return errorCode;
        }
    }
    else
    {
        size = (((activeUEs_g - 1 ) * sizeof(lteRlcUEStatus)) +
                sizeof (lteRlcUEStatusCnf ) + OAM_RLC_API_HEADER_LEN);
    }


    respBuffer_p = (UInt8 *)getMemFromPool(size, RLC_NULL);

    if (respBuffer_p == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_OAM_STATUS_ERR_1, LOGFATAL, (RLC_OAM),
                globalTTITickCount_g, __LINE__, transactionId, ueIndexStart, size, RLC_FAILURE, 0, 0, __func__,
                "");
        return RLC_FAILURE;
    }
    memSet(respBuffer_p,0,size);
    cnfBuffer_p = respBuffer_p ;
    LTE_SET_U16BIT((cnfBuffer_p + sendIndex),transactionId);
    sendIndex += 2;
    prepareOamCnfHeader(cnfBuffer_p , RLC_GET_STATUS_CNF, &sendIndex);

    /*SPR 6641 FIX Start*/
    sendIndex = RLC_API_HEADER_LEN;
    /*SPR 6641 FIX End*/


#ifndef UE_SIM_TESTING
    /* + Layer2 NON CA Changes */
    cnfBuffer_p[API_HEADER_CELL_INDEX_OFFSET]= cellId;
    /* - Layer2 NON CA Changes */
#endif

    lteRlcUEStatusCnf *lteRlcUEStatusCnf_p = 
        (lteRlcUEStatusCnf *)(cnfBuffer_p + OAM_RLC_API_HEADER_LEN );

    if (RLC_SUCCESS != rlcLayerInitialized_g)
    {

        errorCode = lteRlcUEStatusCnf_p->responseCode = RLC_LAYER_NOT_INITIALIZED;
        sendIndex += 2;
        LOG_RLC_MSG(LTE_RLC_OAM_STATUS_ERR_2, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, transactionId, ueIndexStart, size, errorCode, 0, 0, __func__,
                "");
    }
    else
    {
        lteRlcUEStatusCnf_p->responseCode = RLC_SUCCESS;
        sendIndex += 2;
        /* this will be set to number of UEs in the cell */
        lteRlcUEStatusCnf_p->numOfUE = 0;
        sendIndex += 2;

        if ( isUeIndexPresent == RLC_TRUE )
        { 
            /* SPR 3653 Fix Start */
            if ( VALIDATE_UE_IDX (ueIndexStart))
            {
                lteRlcUEStatusCnf_p->numOfUE = 1;
                fillUeStatus(ueIndexStart, lteRlcUEStatusCnf_p,0);
                sendIndex += sizeof(lteRlcUEStatus);
            }
            /* SPR 3653 Fix End */
        }
        else
        {
            /* SPR 3653 Fix Start */
            UInt32 activeUe = activeUEs_g;
            UInt32 statusIndex = 0;
            UInt16 ueIndex = 0;
            for (ueIndex = 0;
                    ((ueIndex < MAX_UE_SUPPORTED) &&(activeUe > 0));
                    ueIndex++)
            {
                if ( VALIDATE_UE_IDX (ueIndex)
#ifndef UE_SIM_TESTING
                        &&
                        /* and if Cell index match with cell where the UE was attached */
                        (RLC_GET_INTERNAL_CELL_INDEX(cellId) == (rlcUEContextList_g[ueIndex])->cellIndex)
#endif
                   )
                {
                    fillUeStatus ( ueIndex, lteRlcUEStatusCnf_p, statusIndex);
                    sendIndex += sizeof(lteRlcUEStatus);
                    activeUe--;
                    statusIndex++;
                }
                /* SPR 3653 Fix End */
            }
            lteRlcUEStatusCnf_p->numOfUE = statusIndex;
        }
        errorCode = RLC_SUCCESS;
    }

    LTE_SET_U16BIT( (cnfBuffer_p + API_HEADER_MSGLEN_OFFSET), sendIndex);
    rlcSendMsgToOam( respBuffer_p,
            sendIndex); 
    /* SPR 3943 Fix Start */
    freeMemPool(respBuffer_p);
    return errorCode; 
}

/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : rlcInitFillSCellSpsParams
 * Inputs         : RlcOamInitLayerReq *RlcOamInitLayerReq_p
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    :This API is used to 
 *                   1. Validate the OAM Init Parameter for serving cells
 *                   
 *******************************************************************************/

void rlcInitFillSCellSpsParams( RlcOamInitLayerReq *RlcOamInitLayerReq_p )
{
    /* SPR 20874 start */ 
    UInt8 qciIndex = 0;
    /* SPR 20874 end   */
    UInt8  idx = 0;
    InternalCellIndex cellCount = 0;
    InternalCellIndex  cellIndex = INVALID_CELL_INDEX;
    LP_RlcSpsIntervalProfile spsProf_p = PNULL;

    for( ;cellCount < (RlcOamInitLayerReq_p->sCellCount);cellCount++)
    {
        /* coverity Fix 59925 starts */
        cellIndex = RLC_GET_INTERNAL_CELL_INDEX(
                RlcOamInitLayerReq_p->sCellInfo[cellCount].cellId) ;

        if( INVALID_CELL_INDEX == cellIndex )
        {
            continue;
        }
        /* coverity Fix 59925 ends */


        secondaryCellInfo_g[cellIndex].numOfUEs = 
            RlcOamInitLayerReq_p->sCellInfo[cellCount].numOfUEs;

        secondaryCellInfo_g[cellIndex].enableSps = RlcOamInitLayerReq_p->\
                                                   sCellInfo[cellCount].enableSps;
        if(LTE_TRUE == (RlcOamInitLayerReq_p->sCellInfo[cellCount].enableSps))
        {
            /* Fill SPS parameters to global structure */

            /* SPR 20874 start */
            /* memCpy( secondaryCellInfo_g[cellIndex].qciSpslist,
               RlcOamInitLayerReq_p->sCellInfo[cellCount].qciSpslist,
               MAX_QCI); */
            for (qciIndex = 0; qciIndex < MAX_QCI; qciIndex++)
            {
                if(qciIndex >= SPS_QCI)
                {
                    secondaryCellInfo_g[cellIndex].qciSpslist[qciIndex] = LTE_FALSE;
                    LOG_RLC_INFO(RLC_OAM,"LTE_RLC_INVALID_SPS_QCI_MAP");       
                    continue;
                }
                secondaryCellInfo_g[cellIndex].qciSpslist[qciIndex] = RlcOamInitLayerReq_p->sCellInfo[cellCount].qciSpslist[qciIndex];
            }

            /* SPR 20874 end */  
            memCpy( secondaryCellInfo_g[cellIndex].supportedDlIntervalList, 
                    RlcOamInitLayerReq_p->sCellInfo[cellCount].\
                    supportedDlIntervalList, SPS_INTERVAL_LIST );

            memCpy( secondaryCellInfo_g[cellIndex].supportedUlIntervalList, 
                    RlcOamInitLayerReq_p->sCellInfo[cellCount].\
                    supportedUlIntervalList, SPS_INTERVAL_LIST );

			/* SPR_9578_Fix Start */
			createMemPool(sizeof (RlcSpsIntervalProfile), SPS_INTERVAL_LIST);
			/* SPR_9578_Fix End */

			for( idx = 0; idx < SPS_INTERVAL_LIST; idx++ )
			{
				if((LTE_TRUE == secondaryCellInfo_g[cellIndex].\
							supportedDlIntervalList[idx]) || 
						(LTE_TRUE == secondaryCellInfo_g[cellIndex].\
						 supportedUlIntervalList[idx]) )
				{
					/*Allocate memory for SPS Interval context */
					spsProf_p = (LP_RlcSpsIntervalProfile)getMemFromPool(
							sizeof(RlcSpsIntervalProfile), PNULL );
					if( PNULL == spsProf_p )
					{
						/*CRITICAL! Memory error*/
						LOG_RLC_MSG(LTE_RLC_HANDLE_OAM_REQ_ERR, LOGFATAL, RLC_OAM, 
								globalTTITickCount_g, __LINE__, 0, 0, 0, 
								RLC_OAM_RX_MEM_POOL_ERR, 0, 0, __func__, " ");
						ALARM_MSG( RLC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
								CRITICAL_ALARM);
						/*ltePanic("MemAlloc fails for spsProf_p" );*/
						/*return RLC_OAM_RX_MEM_POOL_ERR;*/
						return ;
					}           
					spsProf_p->enableSegConcatSpsRlcSdu =
						RlcOamInitLayerReq_p->sCellInfo[cellCount].\
						rlcSpsIntervalProfileParams[idx].\
						enableSegConcatSpsRlcSdu;

					spsProf_p->dlJitterHandlingOption = 
						RlcOamInitLayerReq_p->sCellInfo[cellCount].\
						rlcSpsIntervalProfileParams[idx].dlJitterHandlingOption;

					secondaryCellInfo_g[cellIndex].rlcSpsIntProfileParams_p[idx]
					= spsProf_p;
				}
			}
		}
		else
		{
			/* SPS is not Enabled hence not populating SPS info */
			continue ;
		}
	}
}

/****************************************************************************
 * Function Name  : rlcInitValidateSCellParams
 * Inputs         : RlcOamInitLayerReq *RlcOamInitLayerReq_p
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    :This API is used to 
 *                   1. Validate the OAM Init Parameter for serving cells
 *                   
 ***************************************************************************/
void rlcInitValidateSCellParams( RlcOamInitLayerReq *RlcOamInitLayerReq_p,
        UInt32 *errorCode )
{
    UInt8 qciCount = 0;
    InternalCellIndex cellCount = 0;
    InternalCellIndex  cellIndex = INVALID_CELL_INDEX;

    for( ;cellCount < (RlcOamInitLayerReq_p->sCellCount); cellCount++)
    {
        cellIndex = RLC_UPDATE_AND_GET_INTERNAL_CELL_INDEX(
                RlcOamInitLayerReq_p->sCellInfo[cellCount].cellId) ;
        /* coverity Fix 59925 starts */
        if( INVALID_CELL_INDEX == cellIndex )
        {
            LOG_RLC_WARNING( RLC_OAM, "RLC receives invalid SCell cellId[%u]"
                    " cellCount[%u]", 
                    RlcOamInitLayerReq_p->sCellInfo[cellCount].cellId, 
                    cellCount );
            *errorCode = LTE_RLC_INVALID_CELL_INDEX ;
            return;
        }
        /* coverity Fix 59925 ends */

        if( LTE_TRUE < RlcOamInitLayerReq_p->sCellInfo[cellCount].enableSps )
        {
            LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                    globalTTITickCount_g, __LINE__, 
                    RlcOamInitLayerReq_p->sCellInfo[cellCount].enableSps,
                    LTE_RLC_INALID_SPS_PARAM, 0, 0, 0, 0, __func__, "");

            *errorCode =  LTE_RLC_INALID_SPS_PARAM;
            return;
        }

        if( LTE_TRUE == RlcOamInitLayerReq_p->sCellInfo[cellCount].enableSps )
        {
            for(qciCount = 0; qciCount < MAX_QCI; qciCount++)
            {
                if ( LTE_TRUE < RlcOamInitLayerReq_p->sCellInfo[cellCount].\
                        qciSpslist[qciCount] )
                 
                {              
                    LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                            globalTTITickCount_g, __LINE__, 
                            RlcOamInitLayerReq_p->sCellInfo[cellCount].\
                            qciSpslist[qciCount],
                            LTE_RLC_INALID_SPS_PARAM,
                            0, 0, 0, 0, __func__, "");
                    *errorCode = LTE_RLC_INALID_SPS_PARAM;
                    return;
                }
            }

            if( RLC_FAILURE == rlcValidateIntervalList(
                        RlcOamInitLayerReq_p->sCellInfo[cellCount].\
                        supportedDlIntervalList) )
            {
                LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                        globalTTITickCount_g, __LINE__, 
                        RlcOamInitLayerReq_p->sCellInfo[cellCount].numOfUEs,
                        LTE_RLC_INALID_SPS_PARAM, 0, 0, 0, 0, __func__, "");
                *errorCode = LTE_RLC_INALID_SPS_PARAM; 
                return;
            }

            if( RLC_FAILURE == rlcValidateIntervalList(
                        RlcOamInitLayerReq_p->sCellInfo[cellCount].\
                        supportedUlIntervalList) )
            {
                LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                        globalTTITickCount_g, __LINE__, 
                        RlcOamInitLayerReq_p->sCellInfo[cellCount].numOfUEs,
                        LTE_RLC_INALID_SPS_PARAM, 0, 0, 0, 0, __func__, "");
                *errorCode = LTE_RLC_INALID_SPS_PARAM; 
                return;
            }
        }
        else
        {
            /* SPS is not Enabled hence not validating SPS params*/
            continue ;
        }
    }
    *errorCode = RLC_SUCCESS;

}
#endif
/* - Layer2 NON CA Changes */

/****************************************************************************
 * Function Name  : configInitialRlcCfg
 * Inputs         : RlcOamInitLayerReq *RlcOamInitLayerReq_p,
 *                  cellId - RRC cell Index
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode
 * Description    : This API is used to 
 *                   1. Validate the OAM Init Parameter
 *                   2. fill the values in global variables. 
 *                   
 ***************************************************************************/
inline STATIC UInt32 configInitialRlcCfg( 
        RlcOamInitLayerReq *RlcOamInitLayerReq_p
                                       /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
        ,RrcCellIndex cellId
#endif
                                       /* - Layer2 NON CA Changes */
        ) 
{
    UInt32 errorCode = RLC_SUCCESS;
    UInt32 logLevel=0;
#ifndef UE_SIM_TESTING
    UInt32 qciCount = 0;
    /* + Layer2 NON CA Changes */
    UInt32 errCode = RLC_SUCCESS;
    InternalCellIndex cellCount = 0;
    InternalCellIndex cellIndex = RLC_GET_INTERNAL_CELL_INDEX( cellId );
    /* - Layer2 NON CA Changes */

    /* coverity_60256 Fix Start */
    /* coverity_60835 Fix Start */
    if(INVALID_CELL_INDEX == cellIndex)
    {
        LOG_RLC_WARNING( RLC_OAM, "Cannot get Internal cellId, "
                "recvd cellId[%u]", cellId );
        return LTE_RLC_INVALID_CELL_INDEX;
    }
    /* coverity_60256 Fix End */
    /* coverity_60835 Fix End */

#endif

    /*   API ID - RLC_INIT_LAYER_REQ
         1. numOfUE  Range ( 1 - 255 )
         2. rntiStartRange ( 0x0001 - 0xFFF3)
         3. rntiEndRange   ( 0x0001 - 0xFFF3) 
     */

/* + Layer2 NON CA Changes */
#ifdef UE_SIM_TESTING
/* - Layer2 NON CA Changes */
    if ( ( RlcOamInitLayerReq_p->numOfUEs < 1) 
            || ( RlcOamInitLayerReq_p->numOfUEs > MAX_UE_SUPPORTED))
    {
        LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, RlcOamInitLayerReq_p->numOfUEs,
                RLC_OAM_INVALID_SUPPORTED_USER, 0, 0, 0, 0, __func__, "");
                
        return RLC_OAM_INVALID_SUPPORTED_USER;
    }

#endif
/* + Layer2 NON CA Changes */
    numOfSupportedUsers_g = RlcOamInitLayerReq_p->numOfUEs; 
#ifndef UE_SIM_TESTING
    secondaryCellInfo_g[cellIndex].numOfUEs = numOfSupportedUsers_g;

    /* Coverity Fix 59923 starts */
    if( MAX_NUM_CELL <= (RlcOamInitLayerReq_p->sCellCount) )
    {
    /* Coverity Fix 59923 ends */
        /* This log should be updated during KPI enhancement for Carrier
         * Aggregation feature */
        LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, 
                RlcOamInitLayerReq_p->numOfUEs,
                LTE_RLC_INALID_SPS_PARAM, 0, 0, 0, 0, __func__, "");
        return LTE_RLC_INVALID_CELL_COUNT_RANGE; /* log info needed */
    }

    for( ;cellCount < (RlcOamInitLayerReq_p->sCellCount);cellCount++)
    {
        numOfSupportedUsers_g += 
               RlcOamInitLayerReq_p->sCellInfo[cellCount].numOfUEs;
    }

    if ( ( numOfSupportedUsers_g < 1) 
            || ( numOfSupportedUsers_g > MAX_UE_SUPPORTED ))
    {
        LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, numOfSupportedUsers_g,
                RLC_OAM_INVALID_SUPPORTED_USER, 0, 0, 0, 0, __func__, "");

        return RLC_OAM_INVALID_SUPPORTED_USER;
    }

    if( LTE_TRUE < RlcOamInitLayerReq_p->enableSps )
    {
        LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, RlcOamInitLayerReq_p->numOfUEs,
                LTE_RLC_INALID_SPS_PARAM, 0, 0, 0, 0, __func__, "");
        return LTE_RLC_INALID_SPS_PARAM;
    }
    
    if( LTE_TRUE == RlcOamInitLayerReq_p->enableSps )
    {
        /* Validate QCI to SPS list */
        for(qciCount = 0; qciCount < MAX_QCI; qciCount++)
        {
            if ( LTE_TRUE < RlcOamInitLayerReq_p->qciSpslist[qciCount] )
            {
                LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                        globalTTITickCount_g, __LINE__, 
                        RlcOamInitLayerReq_p->numOfUEs, 
                        LTE_RLC_INALID_SPS_PARAM,
                        0, 0, 0, 0, __func__, "");
                return LTE_RLC_INALID_SPS_PARAM;
            }
        }

        if( RLC_FAILURE == rlcValidateIntervalList(
                    RlcOamInitLayerReq_p->supportedDlIntervalList) )
        {
            LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                    globalTTITickCount_g, __LINE__, 
                    RlcOamInitLayerReq_p->numOfUEs,
                    LTE_RLC_INALID_SPS_PARAM, 0, 0, 0, 0, __func__, "");
            return LTE_RLC_INALID_SPS_PARAM; 
        }


        if( RLC_FAILURE == rlcValidateIntervalList(
                    RlcOamInitLayerReq_p->supportedUlIntervalList) )
        {
            LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                    globalTTITickCount_g, __LINE__, 
                    RlcOamInitLayerReq_p->numOfUEs,
                    LTE_RLC_INALID_SPS_PARAM, 0, 0, 0, 0, __func__, "");
            return LTE_RLC_INALID_SPS_PARAM; 
        }
    }

    /* + Layer2 NON CA Changes */
    if(RlcOamInitLayerReq_p->sCellCount)
    {
        rlcInitValidateSCellParams( RlcOamInitLayerReq_p,&errCode);
        
        if(RLC_SUCCESS != errCode)
        {
            LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),
                    globalTTITickCount_g, __LINE__, 
                    RlcOamInitLayerReq_p->numOfUEs,LTE_RLC_INVALID_SCELL_PARAM,
                    0, 0, 0, 0, __func__, "");
            return errCode;
        }

        numOfRlcCellConfigured_g += RlcOamInitLayerReq_p->sCellCount;
    }
#else

    if(RlcOamInitLayerReq_p->sCellCount)
    {
        numOfRlcCellConfigured_g += RlcOamInitLayerReq_p->sCellCount;
    }
#endif
    /* - Layer2 NON CA Changes */
    if( RLC_SUCCESS == initAllRLCThreadsAndInterfaces())
    {
        /* Cyclomatic complexity changes starts here */
        errorCode = fillRlcInitLayerReqGlobalParams(RlcOamInitLayerReq_p
#ifndef UE_SIM_TESTING
                    ,cellIndex
#endif
                );
        if(RLC_SUCCESS != errorCode)
        {
            return errorCode;
        }
        /* Cyclomatic complexity changes ends here */
    }
    else
    {
        LOG_RLC_MSG(LTE_RLC_CONFIG_INIT_ERR_2, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, RlcOamInitLayerReq_p->numOfUEs, 
                0,0, errorCode, 0, 0, __func__, 
                    "");

        return errorCode;
    }

    if ( RLC_FALSE == rlcInitInd_g)
    {
        rlcInitInd_g = RLC_TRUE;
    }

    logLevel = RlcOamInitLayerReq_p->logLevel;
    l2_map_ext_loglevel_to_intern_loglevel((UInt8 *)&logLevel);
    /* SPR 20899 Changes Start*/
    rlcloglevelEnabled_g = MULTIPLY_BY_TWO(logLevel)-1;
    /* SPR 20899 Changes End*/

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : fillRlcInitLayerReqGlobalParams
 * Inputs         : RlcOamInitLayerReq *RlcOamInitLayerReq_p,
 *                  cellIndex - InternalCellIndex
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode
 * Description    : This API is used to 
 *                  fill the values in global variables. 
 *                   
 ***************************************************************************/

inline STATIC UInt32 fillRlcInitLayerReqGlobalParams( 
                                       RlcOamInitLayerReq *RlcOamInitLayerReq_p
#ifndef UE_SIM_TESTING
                                       ,InternalCellIndex cellIndex
#endif
                                        ) 
{

#ifndef UE_SIM_TESTING
    /* SPR 20874 start */
    UInt8 qciIndex = 0;
    /* SPR 20874 end   */
    UInt32 qciCount = 0;
    UInt8 tempQci = 0;
    UInt8  idx = 0;
    LP_RlcSpsIntervalProfile spsProf_p = PNULL;
#endif
    createMemPoolRlcNumSupportedUsers();
    /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    for(qciCount = 0;qciCount < MAX_QCI ; qciCount++)
    {
        tempQci = RlcOamInitLayerReq_p->rlcQosQciInfo[qciCount].qci;
        if ( tempQci >= 1 && tempQci < MAX_QCI )
        {
            rlcQosQciInfo_g[tempQci -1].qci = RlcOamInitLayerReq_p->\
                                              rlcQosQciInfo[qciCount].qci;
            rlcQosQciInfo_g[tempQci -1].eResType = RlcOamInitLayerReq_p->\
                                                   rlcQosQciInfo[qciCount].eResType;
            rlcQosQciInfo_g[tempQci -1].priority = RlcOamInitLayerReq_p->\
                                                   rlcQosQciInfo[qciCount].priority;
            rlcQosQciInfo_g[tempQci -1].pktDlyBdgt = RlcOamInitLayerReq_p->\
                                                     rlcQosQciInfo[qciCount].pktDlyBdgt;
            rlcQosQciInfo_g[tempQci -1].pktErrLossRate = RlcOamInitLayerReq_p->\
                                                         rlcQosQciInfo[qciCount].pktErrLossRate;
        }
        tempQci = 0;
    }
    qSizeFactorGBR_g = RlcOamInitLayerReq_p->qSizeFactorGBR;
    qSizeFactorNGBR_g = RlcOamInitLayerReq_p->qSizeFactorNGBR;

    /* Fill SPS parameters to global structure */
    /* + Layer2 NON CA Changes */

    secondaryCellInfo_g[cellIndex].enableSps = 
        RlcOamInitLayerReq_p->enableSps;

    if( LTE_TRUE == RlcOamInitLayerReq_p->enableSps )
    {
        /* SPR 20874 start */
        /*memCpy( secondaryCellInfo_g[cellIndex].qciSpslist, \
          RlcOamInitLayerReq_p->qciSpslist, MAX_QCI);    */
        for (qciIndex = 0; qciIndex < MAX_QCI; qciIndex++)
        {
            if(qciIndex >= SPS_QCI)
            {
                secondaryCellInfo_g[cellIndex].qciSpslist[qciIndex] = LTE_FALSE;
                LOG_RLC_INFO(RLC_OAM,"LTE_RLC_INVALID_SPS_QCI_MAP");         
                continue;
            }
            secondaryCellInfo_g[cellIndex].qciSpslist[qciIndex] = RlcOamInitLayerReq_p->qciSpslist[qciIndex];
        }

        /* SPR 20874 end   */     
        memCpy( secondaryCellInfo_g[cellIndex].supportedDlIntervalList, 
                RlcOamInitLayerReq_p->supportedDlIntervalList, 
                SPS_INTERVAL_LIST );

        memCpy( secondaryCellInfo_g[cellIndex].supportedUlIntervalList, 
                RlcOamInitLayerReq_p->supportedUlIntervalList, 
                SPS_INTERVAL_LIST );

        /* SPR_9578_Fix Start */
        createMemPool(sizeof (RlcSpsIntervalProfile), SPS_INTERVAL_LIST);
        /* SPR_9578_Fix End */

        for( idx = 0; idx < SPS_INTERVAL_LIST; idx++ )
        {
            if( (LTE_TRUE == secondaryCellInfo_g[cellIndex].\
                        supportedDlIntervalList[idx]) || 
                    (LTE_TRUE == secondaryCellInfo_g[cellIndex].\
                     supportedUlIntervalList[idx]) )
            {
                /*Allocate memory for SPS Interval context */
                spsProf_p = (LP_RlcSpsIntervalProfile)getMemFromPool(
                        sizeof(RlcSpsIntervalProfile), PNULL );
                if( PNULL == spsProf_p )
                {
                    /*CRITICAL! Memory error*/
                    LOG_RLC_MSG(LTE_RLC_HANDLE_OAM_REQ_ERR, LOGFATAL, RLC_OAM, 
                            globalTTITickCount_g, __LINE__, 0, 0, 0, 
                            RLC_OAM_RX_MEM_POOL_ERR, 0, 0, __func__, " ");
                    ALARM_MSG( RLC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                            CRITICAL_ALARM);
                    return RLC_OAM_RX_MEM_POOL_ERR;  
                }           
                spsProf_p->enableSegConcatSpsRlcSdu =
                    RlcOamInitLayerReq_p->rlcSpsIntervalProfileParams[idx].\
                    enableSegConcatSpsRlcSdu;
                spsProf_p->dlJitterHandlingOption = 
                    RlcOamInitLayerReq_p->rlcSpsIntervalProfileParams[idx].\
                    dlJitterHandlingOption;

                secondaryCellInfo_g[cellIndex].rlcSpsIntProfileParams_p[idx]
                    = spsProf_p;
            }
        }
    }

    if (RlcOamInitLayerReq_p->sCellCount)
    {
        rlcInitFillSCellSpsParams( RlcOamInitLayerReq_p );
    }
    /* - Layer2 NON CA Changes */

    /* HSM_CHG_START */
    rlcHealthMonitoringTimeInterval_g = (RlcOamInitLayerReq_p->\
            healthMonitoringTimeInterval * 1000);
    if(ZERO == rlcHealthMonitoringTimeInterval_g)
    {
        LOG_RLC_MSG(HSM_OAM_RLC_INIT_FAILURE, LOGFATAL, L2_SYS_FAIL,
                globalTTITickCount_g,rlcHealthMonitoringTimeInterval_g,
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"RLC OAM Health Status Init Failure");

        return RLC_FAILURE;
    }
    /* HSM_CHG_END */
#endif
    /** Rel 4.0 changes End **/
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : createMemPoolRlcNumSupportedUsers
 * Inputs         : None
 * Outputs        : modifies global var.
 * Returns        : None
 * Description    : This API is used to 
 *                  allocate memory for rlcNumOfSupportedUsers_g  
 *                   
 ***************************************************************************/
/* + SPR 17439 */
inline STATIC void createMemPoolRlcNumSupportedUsers(void)
/* - SPR 17439 */                                       
{
    rlcNumOfSupportedUsers_g = ((numOfSupportedUsers_g) * 
            LTE_CREATE_MEMPOOL_EXTRA_BUF_FACTOR);

    LTE_DL_MULTIPLY_FACTOR(rlcNumOfSupportedUsers_g, rlcDlMemFac_g);
    LTE_UL_MULTIPLY_FACTOR(rlcNumOfSupportedUsers_g, rlcUlMemFac_g);

}

/****************************************************************************
 * Function Name  : handleRlcOamInitLayer 
 * Inputs         : respBuff,
 *                  sendInd,
 *                  recvBuff,
 *                  length
 * Outputs        : None
 * Returns        : RLC_SUCCESS/errorCode
 * Description    : This API is used to 
 *                   1. Parsing the OAM Init Request Message
 *                   2. Validate the parameter
 *                   3. Initialize RLC DB and Interfaces queues
 *                   4. Appropriate response and fill it in the response buffer.
 *                   
 *******************************************************************************/

/* +- SPR 17777 */
inline STATIC UInt32 handleRlcOamInitLayer ( UInt8 *respBuff,
                                             UInt32 *sendInd_p,
                                             UInt8 *recvBuff,
                                             UInt16 length
                                           )
/* +- SPR 17777 */
{
    UInt8 resp       =  RLC_SUCCESS;
    UInt32 errorCode = INVALID_ERR_CODE;
    UInt32 sendInd   = *sendInd_p; 

    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    RrcCellIndex cellId = recvBuff[API_HEADER_CELL_INDEX_OFFSET];
#endif
    /* - Layer2 NON CA Changes */

    /* Typecasting the Recieved buffer*/
    RlcOamInitLayerReq *RlcOamInitLayerReq_p = 
        (RlcOamInitLayerReq *) (recvBuff + OAM_RLC_API_HEADER_LEN);
    /* Typecasting the Response buffer*/
    RlcOamCnf *lteRlcOamInitLayerCnf_p =
        (RlcOamCnf *)(respBuff + OAM_RLC_API_HEADER_LEN);

    /* Making Header for the Response Message*/ 
    prepareOamCnfHeader(respBuff, RLC_INIT_LAYER_CNF, &sendInd);

/*SPR 6641 FIX Start*/
    sendInd = RLC_API_HEADER_LEN;
/*SPR 6641 FIX End*/
#ifndef UE_SIM_TESTING
    /* + Layer2 NON CA Changes */
    respBuff[API_HEADER_CELL_INDEX_OFFSET]= cellId;
    /* - Layer2 NON CA Changes */
#endif

    /* If RLC layer is already Initialized */
    if ( RLC_SUCCESS == rlcLayerInitialized_g)
    {
        lteRlcOamInitLayerCnf_p->responseCode = RLC_LAYER_ALREADY_INITIALIZED;
        errorCode = RLC_OAM_LAYER_ALREADY_INITIALIZED;

        LOG_RLC_MSG(LTE_RLC_HANDLE_RLC_OAM_INIT_LAYER_ERR, LOGWARNING, 
                RLC_OAM, globalTTITickCount_g, __LINE__, errorCode, 0, 0, 0,
                0, 0, __func__, "");

    } 
    else if ( length >= sizeof(RlcOamInitLayerReq))
    {
        /* Validate and Intiliaze the global variables*/
    /* + Layer2 NON CA Changes */
        errorCode = configInitialRlcCfg( RlcOamInitLayerReq_p
#ifndef UE_SIM_TESTING
                                       ,cellId
#endif
                                       );
    /* - Layer2 NON CA Changes */
        if ( RLC_SUCCESS != errorCode )
        {
            resp = RLC_FAILURE;
            LOG_RLC_MSG(LTE_RLC_HANDLE_RLC_OAM_INIT_LAYER_ERR_1, LOGWARNING, (RLC_OAM),
                    globalTTITickCount_g, __LINE__, resp, 0, 0, 0, 0, 0, __func__,
                    "");
        }
        else
        {
            /* Start 128 UE: Memory Changes */
            createRLCMemPools();
            /* End 128 UE: Memory Changes */
            /** SPR 13599 Changes Start **/
            /** Memory pool creation for HarqFailure Structure is already taken care,
            * So no need to create extra memory **/ 
            /* Coverity 67219 Fix Start */
            rlcHarqFailureInfo_g = (RLCHarqFailureInfo *)getMemFromPool(
                         (MAX_LC_ID * sizeof(RLCHarqFailureInfo)), PNULL);
            /* Coverity 67219 Fix End */
            /* Coverity 67217 Fix Start */
            if(PNULL == rlcHarqFailureInfo_g)
            {
                LOG_RLC_FATAL( RLC_OAM, "Memory allocation failure!" );
                resp = RLC_FAILURE;
            }
            /* Coverity 115205 + */
            else
            {
                /* Coverity 67217 Fix End */
                memSet(rlcHarqFailureInfo_g,0,MAX_LC_ID * sizeof(RLCHarqFailureInfo));
                /* Coverity 67219 Fix End */
            }
            /* Coverity 115205 - */
            /** SPR 13599 Changes End **/
        }
        lteRlcOamInitLayerCnf_p->responseCode = resp;
    }
    else 
    {   /* Invalid Message length recieved from OAM layer*/
        lteRlcOamInitLayerCnf_p->responseCode = RLC_SYNTAX_ERROR;    
        errorCode = RLC_OAM_INVALID_OAM_INIT_REQ_LEN;
        LOG_RLC_MSG(LTE_RLC_HANDLE_RLC_OAM_INIT_LAYER_ERR_2, LOGWARNING, (RLC_OAM),
                globalTTITickCount_g, __LINE__, errorCode, 0, 0, 0, 0, 0, __func__,
                "");

    }         
    sendInd += 2;

    LTE_SET_U16BIT( (respBuff + API_HEADER_MSGLEN_OFFSET), sendInd);

    *sendInd_p = sendInd;    

    return errorCode; 

}

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : handleRlcHealthMonitorStatusReq
 * Inputs         : respBuff,
 *                  sendInd,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to validate RLC thread is running or down. 
 ****************************************************************************/
inline static UInt32 handleRlcHealthMonitorStatusReq ( UInt8 *respBuff,
        UInt32 sendInd
        /* + Layer2 NON CA Changes */
        , RrcCellIndex cellId
        /* - Layer2 NON CA Changes */
        )
{
    UInt8 resp        =  RLC_FAILURE;
    UInt8  threadIndx = 0;
    /* SPR 15909 fix start */
    tickType_t curr_tick  = globalTTITickCount_g;
    /* SPR 15909 fix end */
    InternalCellIndex cellIndex = 0;

    /* Making Header for the Response Message*/
    prepareOamCnfHeader(respBuff, RLC_PROC_SUP_RESP, &sendInd);
    /*spr 8785 chg start*/
    /* CA changes start */
    /* Check the state of cells, if not any cell active then always return success */ 
    if(INVALID_CELL_INDEX != layer2CommonGetActiveCellTick())
    /* CA changes end */
    {
        for (threadIndx = 0; threadIndx < RLC_MAX_THREAD_ID; threadIndx++) 
        {
            /* Check for the thread status of all cells */
            for(cellIndex = 0; cellIndex < numOfRlcCellConfigured_g; cellIndex++)
            {
                /*SPR 11244 Changes Start */
                if(layer2CommonIsCellRunning(cellIndex))
                {
                    if (curr_tick - 
                            l2ThreadHealthStoreTick_g.rlcMacThread[cellIndex].rlcThread[threadIndx] <
                            rlcHealthMonitoringTimeInterval_g)
                    {
                        resp = RLC_SUCCESS;
                    }
                    else
                    {
                        LOG_RLC_MSG(HSM_RLC_OAM_REQ_FAILURE, LOGFATAL, L2_SYS_FAIL,
                                globalTTITickCount_g,threadIndx,
                                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                                __func__,"RLC OAM Health Status Failure");

                        return RLC_FAILURE;

                    }
                }
                /*SPR 11244 Changes End */
            }
        }
    }
    else
    {
        resp = RLC_SUCCESS;
    }
    /*spr 8785 chg end*/
    sendInd = OAM_RLC_API_HEADER_LEN ; 

    /* + Layer2 NON CA Changes */
    respBuff[API_HEADER_CELL_INDEX_OFFSET]= cellId;
    /* - Layer2 NON CA Changes */

    LTE_SET_U16BIT( (respBuff + API_HEADER_MSGLEN_OFFSET), sendInd);
    /*SPR 11244 Changes Start */
    if(resp == RLC_SUCCESS)
    {
        rlcSendMsgToOam(
        respBuff, 
        sendInd);
    }
    /*SPR 11244 Changes End */
    return resp;
}
#endif

/***************************************************************************
 * Function Name  : rlcOamResetStatCnf
 * Inputs         : response - Pointer to response msg,
 *                  SendInd - it points to the Response buffer,
 *                  length - Length of the recieved buffer,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode
 * Description    : This API is used to reset the Stats buffer and 
 *                  send appropriate response.
 * *************************************************************************/
inline STATIC UInt32 rlcOamResetStatCnf( UInt8 *response,
        UInt32 sendInd,
        UInt16 length
        /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
        ,RrcCellIndex cellId
#endif
        /* - Layer2 NON CA Changes */
        )
{
    RlcOamCnf *lteRlcOamResetStatCnf_p = 
                (RlcOamCnf *)(response + OAM_RLC_API_HEADER_LEN);

    UInt32 errorCode = INVALID_ERR_CODE;
    
    if ( RLC_SUCCESS != rlcLayerInitialized_g) 
    {
        errorCode = RLC_LAYER_NOT_INITIALIZED;
        lteRlcOamResetStatCnf_p->responseCode = RLC_LAYER_NOT_INITIALIZED;
        LOG_RLC_MSG( LTE_RLC_OAM_RESET_STATS_ERR, LOGWARNING, 
                   RLC_OAM, globalTTITickCount_g, __LINE__,
                   sendInd, length, errorCode, 0, 0, 0, __func__, "");
                   
    }
    else if ( length >= OAM_RLC_API_HEADER_LEN ) 
    {
        /* Reset Stats */
		/* + PERF_CA GBL */
		/* + coverity fixes 62622 */
#ifndef UE_SIM_TESTING
		InternalCellIndex internalCellIndex = layer2CommonGetInternalCellIndex(cellId);
#else
		InternalCellIndex internalCellIndex = 0;
#endif
		if (MAX_INTERNAL_CELL_INDEX >= internalCellIndex)
		{
        	rlcResetStat(internalCellIndex);
		}

		/* - coverity fixes 62622 */
		/* - PERF_CA GBL */
        errorCode = RLC_SUCCESS;
        lteRlcOamResetStatCnf_p->responseCode = RLC_SUCCESS;
    }
    else 
    {
        lteRlcOamResetStatCnf_p->responseCode = RLC_SYNTAX_ERROR;    
        errorCode = RLC_RESET_INVALID_STATS_REQ_LEN;
        LOG_RLC_MSG( LTE_RLC_OAM_RESET_STATS_ERR_1, LOGWARNING, 
                   RLC_OAM, globalTTITickCount_g, __LINE__,
                   sendInd, length, errorCode, 0, 0, 0, __func__, "");
        
    }

    prepareOamCnfHeader(response, RLC_RESET_STATS_CNF, &sendInd);

/*SPR 6641 FIX Start*/
    sendInd = RLC_API_HEADER_LEN;
/*SPR 6641 FIX End*/


#ifndef UE_SIM_TESTING
    /* + Layer2 NON CA Changes */
    response[API_HEADER_CELL_INDEX_OFFSET]= cellId;
    /* - Layer2 NON CA Changes */
#endif
    sendInd += sizeof(RlcOamCnf);
    LTE_SET_U16BIT( (response + API_HEADER_MSGLEN_OFFSET), sendInd);
    rlcSendMsgToOam(
            response,
            sendInd);
    
    return errorCode;
}

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : rlcOamGetLogLevelResp
 * Inputs         : respBuff,
 *                  sendInd,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : Fill the response with entire L2 log level enabled earlier 
 *                  and not just RLC.
 ******************************************************************************/
inline STATIC void rlcOamGetLogLevelResp( 
                                          UInt8 *respBuff,
                                          UInt32 sendInd,
                                       /* + Layer2 NON CA Changes */
                                          RrcCellIndex  cellId
                                       /* - Layer2 NON CA Changes */
                                        )
{
    RlcOamGetLogLevelResp *RlcOamGetLogLevelResp_p = 
                   (RlcOamGetLogLevelResp *)(respBuff + OAM_RLC_API_HEADER_LEN);
    /*Fill the response with entire L2 log level enabled earlier*/ 
    RlcOamGetLogLevelResp_p->logLevel = rlcloglevelEnabled_g;
    prepareOamCnfHeader(respBuff, RLC_GET_LOG_LEVEL_RESP, &sendInd);

/*SPR 6641 FIX Start*/
    sendInd = RLC_API_HEADER_LEN;
/*SPR 6641 FIX End*/

    /* + Layer2 NON CA Changes */
    respBuff[API_HEADER_CELL_INDEX_OFFSET]= cellId;
    /* - Layer2 NON CA Changes */

    sendInd += 4;
    LTE_SET_U16BIT( (respBuff + API_HEADER_MSGLEN_OFFSET), sendInd);
    rlcSendMsgToOam(
            respBuff, 
            sendInd);
    return;
} 
/****************************************************************************
 * Function Name  : rlcOamGetLogCategoryResp
 * Inputs         : respBuf,
 *                  startInd,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to send appropriate response and
 *                  fill it in the response buffer.
 ****************************************************************************/
inline STATIC void rlcOamGetLogCategoryResp ( 
                                              UInt8 *respBuf,
                                              UInt32 startInd,
                                       /* + Layer2 NON CA Changes */
                                              RrcCellIndex  cellId
                                       /* - Layer2 NON CA Changes */
                                            )
{
    RlcOamGetLogCategoryResp *RlcOamGetLogCategoryResp_p = 
        (RlcOamGetLogCategoryResp *)(respBuf + OAM_RLC_API_HEADER_LEN);

    prepareOamCnfHeader(respBuf, RLC_GET_LOG_CATEGORY_RESP,&startInd);

/*SPR 6641 FIX Start*/
    startInd = RLC_API_HEADER_LEN;
/*SPR 6641 FIX End*/

    /* + Layer2 NON CA Changes */
    respBuf[API_HEADER_CELL_INDEX_OFFSET]= cellId;
    /* + Layer2 NON CA Changes */

    RlcOamGetLogCategoryResp_p->logCategory = rlclogCategoryEnabled_g;
    startInd += 8;
    LTE_SET_U16BIT( (respBuf + API_HEADER_MSGLEN_OFFSET), startInd);
    rlcSendMsgToOam(
            respBuf,
            startInd);
    return ;
}
/****************************************************************************
 * Function Name  : rlcOamEnableLogCategory
 * Inputs         : recvBuff,
 *                  recvIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to enable teh log for specific category. 
 ************************************************************************************/

/* +- SPR 17777 */
inline STATIC void rlcOamEnableLogCategory(UInt8 *recvBuff)
/* +- SPR 17777 */
{

    RlcOamEnableLogCategoryReq *RlcOamEnableLogCategoryReq_p = 
        (RlcOamEnableLogCategoryReq *)(recvBuff+ OAM_RLC_API_HEADER_LEN);
    rlclogCategoryEnabled_g |= RlcOamEnableLogCategoryReq_p->logCategory;
    return ;
}

/****************************************************************************
 * Function Name  : rlcOamDisableLogcategory
 * Inputs         : recvBuff,
 *                  recvIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Disable the log.
 *************************************************************************************/

/* +- SPR 17777 */
inline STATIC void rlcOamDisableLogcategory(UInt8 *recvBuff) 
/* +- SPR 17777 */
{
    RlcOamDisableLogCategoryReq *RlcOamDisableLogCategoryReq_p = 
         (RlcOamDisableLogCategoryReq *) (recvBuff+ OAM_RLC_API_HEADER_LEN); 
    rlclogCategoryEnabled_g &= ~(RlcOamDisableLogCategoryReq_p->logCategory);
    return;
} 
#endif    
/* Change for UE Status End */

/****************************************************************************
 * Function Name  : handleOamRequests
 * Inputs         : recvBuffer,
 *                  bytesread 
 * Outputs        : None
 * Returns        : RLC_SUCCESS/errorcode
 * Description    : This API is used to parse the request buffer and prepare 
 *                  appropriate response and fill it in the response buffer
 *                  The followings are the  RLC API from OAM layer:-
 *                  1. RLC_INIT_LAYER_REQ 
 *                  2. RLC_CLEANUP_LAYER_REQ
 *                  3. RLC_GET_BUILD_INFO_REQ
 *                  4. RLC_RESET_STATS_REQ
 *                  5. RLC_GET_STATS_REQ
 *                  6. RLC_GET_STATUS_REQ
 *                  7. RLC_SET_LOG_LEVEL_REQ 
 *                  8. RLC_GET_LOG_LEVEL_REQ 
 *                  9. RLC_ENABLE_LOG_CATEGORY_REQ
 *                 10. RLC_DISABLE_LOG_CATEGORY_REQ
 *                 11. RLC_GET_LOG_CATEGORY_REQ
 *                 12. RLC_PACKET_DROP_REQ
 *                 13. RLC_COMMON_CHANNEL_STATUS_REQ.
 *
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
/* +- SPR 17777 */
UInt32 handleOamRequests ( UInt8* recvBuffer)
/* SPR 5599_129119 Fix End */
/* +- SPR 17777 */
{
    UInt16 apiID           = 0;
    UInt32 recvInd         = 0;
    UInt32 sendInd         = 0;
    UInt16 length          = 0;
    UInt16 transactionId   = 0;
    UInt32 errorCode       = RLC_TRUE;
    UInt8 *response_p      = PNULL;
    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    RrcCellIndex cellId = INVALID_CELL_INDEX;
#endif
    /* - Layer2 NON CA Changes */
    /* Parse the Header of recieved buffer */
    /* 
       Transaction ID   -  2 bytes
       Src Module ID    -  2 bytes
       Dest. Module ID  -  2 bytes
       API - Identifier -  2 bytes
       Length           -  2 bytes
       */
    transactionId = LTE_GET_U16BIT(recvBuffer + recvInd);
    recvInd += 6;
    apiID = LTE_GET_U16BIT(recvBuffer + recvInd);
    recvInd += 2;
    length = LTE_GET_U16BIT(recvBuffer + recvInd);
    recvInd += 2;

#ifndef UE_SIM_TESTING
    cellId = recvBuffer[recvInd];
    recvInd += 1;
    /* cellIndex will not be present at RLC_INIT_LAYER_REQ */
#endif
    response_p = (UInt8 *)getMemFromPool( MAX_REQ_DATASIZE
            , RLC_NULL);
    if ( PNULL == response_p)
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_OAM_REQ_ERR, LOGFATAL, 
                RLC_OAM, globalTTITickCount_g, __LINE__, transactionId,
                apiID, length, RLC_OAM_RX_MEM_POOL_ERR,
                0, 0, __func__, " ");
        return RLC_OAM_RX_MEM_POOL_ERR;
    }
    /* SPR 11109 Fix start */
    memSet (response_p, 0, MAX_REQ_DATASIZE
           );
    /* SPR 11109 Fix end */
    /* Fill transaction Id in response */
    LTE_SET_U16BIT((response_p
                + sendInd),transactionId);
    sendInd += 2 ;

    switch ( apiID )
    {
        case RLC_INIT_LAYER_REQ:

        /* This function initialize the RLC DB */
        errorCode =
            /* +- SPR 17777 */
            handleRlcOamInitLayer( response_p
                    , &sendInd, recvBuffer, length);
        /* +- SPR 17777 */

        rlcSendMsgToOam( response_p,
                sendInd);
        break;

        case RLC_CLEANUP_LAYER_REQ:

            /* + Layer2 NON CA Changes */
            /* +- SPR 17777 */
        errorCode = 
            cleanupAllRLCThreadsAndInterfaces( response_p
                    , &sendInd
#ifndef UE_SIM_TESTING
                    ,cellId
#endif
                    );
            /* - Layer2 NON CA Changes */
            /* +- SPR 17777 */
        rlcSendMsgToOam( response_p, 
                sendInd );
        break;

        case RLC_GET_BUILD_INFO_REQ:
            /* Discuss with OAM team */ 
            /* + Layer2 NON CA Changes */
        errorCode =  getRLCBuildConfiguration( response_p
#ifndef UE_SIM_TESTING
                ,cellId
#endif
                );
            /* - Layer2 NON CA Changes */

        break;

        case RLC_RESET_STATS_REQ:

            /* + Layer2 NON CA Changes */
        errorCode = rlcOamResetStatCnf( response_p
                , sendInd, length
#ifndef UE_SIM_TESTING
                ,cellId
#endif
                );
            /* - Layer2 NON CA Changes */

        break;

        case RLC_GET_STATS_REQ:

            /* + Layer2 NON CA Changes */
        errorCode = rlcOamGetStatCnf( transactionId, length
#ifndef UE_SIM_TESTING
                ,cellId
#endif
                );
            /* - Layer2 NON CA Changes */

        break;

        case RLC_GET_STATUS_REQ:

            /* + Layer2 NON CA Changes */
            /* +- SPR 17777 */
        errorCode  = rlcOamGetStatusCnf( recvBuffer, transactionId
#ifndef UE_SIM_TESTING
                ,cellId
#endif
                );
            /* - Layer2 NON CA Changes */
        break;
#ifndef UE_SIM_TESTING
        case RLC_SET_LOG_LEVEL_REQ:

        setLogLevel(recvBuffer);

        break;
        case RLC_GET_LOG_LEVEL_REQ:

            /* + Layer2 NON CA Changes */
        rlcOamGetLogLevelResp(response_p
                , sendInd, cellId);
            /* - Layer2 NON CA Changes */

        break;

        case RLC_ENABLE_LOG_CATEGORY_REQ:

            /* +- SPR 17777 */
        rlcOamEnableLogCategory(recvBuffer);
            /* +- SPR 17777 */
        break;
        case RLC_DISABLE_LOG_CATEGORY_REQ:

            /* +- SPR 17777 */
        rlcOamDisableLogcategory(recvBuffer);
            /* +- SPR 17777 */

        break;
        case RLC_GET_LOG_CATEGORY_REQ:

            /* + Layer2 NON CA Changes */
        rlcOamGetLogCategoryResp(response_p
                , sendInd, cellId);
            /* - Layer2 NON CA Changes */

        break;    
        case RLC_PROC_SUP_REQ:
        errorCode = 
            /* + Layer2 NON CA Changes */
            handleRlcHealthMonitorStatusReq (response_p
                    ,sendInd
#ifndef UE_SIM_TESTING
                    ,cellId 
#endif
                    );
            /* - Layer2 NON CA Changes */
        break;
        /*HSM_CHG_END*/
#endif  
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        case RLC_CONFIGURE_PERF_STATS_REQ:

			/* + PERF_CA */
        handleRlcOamConfigurePerfStatsReq( recvBuffer, transactionId,
                response_p
                ,sendInd, cellId);
			/* - PERF_CA */
        break;
        case RLC_GET_PERF_STATS_REQ:
			/* + stats_review_comments_ipc */
        handleRlcOamGetPerfStatsReq( recvBuffer,response_p
                ,sendInd 
#ifndef UE_SIM_TESTING
                ,cellId
#endif 
                ); 
			/* - stats_review_comments_ipc */
        break;
#endif
		/*spr 22474 start*/
		case OAM_RLC_GET_DEBUG_INFO_REQ:
				handleRlcOamGetDebugInfoReq( recvBuffer,transactionId,
                        response_p,
						sendInd 
#ifndef UE_SIM_TESTING
                        ,cellId
#endif
                        );
				break;
        /*spr 22474 end*/
        default:
        errorCode = RLC_OAM_RX_INVALID_MSG_ID;
        LOG_RLC_MSG(LTE_RLC_HANDLE_OAM_REQ_ERR_1, LOGWARNING, 
                RLC_OAM, globalTTITickCount_g, __LINE__, transactionId,
                apiID, length, errorCode, 0, 0, __func__, " ");
        break;
    }

    LOG_RLC_MSG(LTE_RLC_HANDLE_OAM_REQ, LOGINFO, 
            RLC_OAM, globalTTITickCount_g, __LINE__, transactionId, apiID, 
            length, errorCode, 0, 0, __func__, "");
    /* SPR 3943 Fix Start */
    freeMemPool(response_p);
    response_p = RLC_NULL;
    /* SPR 3943 Fix End */
    return errorCode;

}

/****************************************************************************
 * Function Name  : setLogLevel 
 * Inputs         : recvBuffer
 * Outputs        : None
 * Returns        : None
 * Description    : This function set the log level of entire l2 currently
                    adn not just RLC.
 ****************************************************************************/
/*coverity 47714 +*/
#ifndef UE_SIM_TESTING
inline static void  setLogLevel( 
                                  UInt8  *recvBuffer
                               )
{
    UInt8 logLevel = 0;
    RlcOamSetLogLevelReq *RlcOamSetLogLevelReq_p=
        (RlcOamSetLogLevelReq *)(recvBuffer + OAM_RLC_API_HEADER_LEN);
    logLevel = RlcOamSetLogLevelReq_p->logLevel;
    /*set the log level of entire L2*/
    if (logLevel == 0)
    {
        rlcloglevelEnabled_g = logLevel;
    }
    else
    {
        rlcloglevelEnabled_g = ((2 * logLevel) -1 );
    }

}
#endif
/*coverity 47714 -*/
/****************************************************************************
 * Function Name  : sendErrReportToOAM
 * Inputs         : responseBuffer_p,
 *                  sendInd,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : This function send the report to the OAM layer.
 ****************************************************************************/
inline STATIC void sendErrReportToOAM( UInt8* responseBuffer_p,
                                       UInt32 sendInd
#ifndef UE_SIM_TESTING
                                       /* + Layer2 NON CA Changes */
                                       ,RrcCellIndex  cellId
                                       /* - Layer2 NON CA Changes */
#endif
                                     )
{
    prepareOamCnfHeader ( responseBuffer_p, RLC_GET_BUILD_INFO_CNF, &sendInd);

/*SPR 6641 FIX Start*/
    sendInd = RLC_API_HEADER_LEN;
/*SPR 6641 FIX End*/

#ifndef UE_SIM_TESTING
    /* + Layer2 NON CA Changes */
    responseBuffer_p[API_HEADER_CELL_INDEX_OFFSET]= cellId;
    /* - Layer2 NON CA Changes */
#endif

    responseBuffer_p[sendInd] = RLC_FAILURE;
    sendInd++;
    LTE_SET_U16BIT( (responseBuffer_p + API_HEADER_MSGLEN_OFFSET), sendInd);

    rlcSendMsgToOam( 
            responseBuffer_p, 
            sendInd);
}

/****************************************************************************
 * Function Name  : getRLCBuildConfiguration
 * Inputs         : responseBuffer_p 
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode
 * Description    : This function reads the build stamp file and fills the 
 *                  build version in the array pointed by buildStamp_p.
 ****************************************************************************/

inline STATIC UInt32 getRLCBuildConfiguration( UInt8* responseBuffer_p
                                              /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                               ,RrcCellIndex cellId
#endif
                                              /* - Layer2 NON CA Changes */
                                              )
{
    UInt32 ret = 0;
    UInt32 Idx       = RLC_API_HEADER_LEN;
    UInt32 sendInd   = 2; /*Transaction ID has been already filled*/

    ret = buildConfiguration(responseBuffer_p,&Idx);
    if(! ret)
    {
        prepareOamCnfHeader ( responseBuffer_p, RLC_GET_BUILD_INFO_CNF, &sendInd);

        /*SPR 6641 FIX Start*/
        sendInd = RLC_API_HEADER_LEN;
        /*SPR 6641 FIX End*/
        LTE_SET_U16BIT( (responseBuffer_p + API_HEADER_MSGLEN_OFFSET), Idx);

        rlcSendMsgToOam( 
                responseBuffer_p, 
                sendInd);
        return RLC_SUCCESS;
    }
    else if(1 ==ret)
    {
        LOG_RLC_MSG( LTE_RLC_BUILD_CONFIGURATION_ERR, LOGWARNING, 
                RLC_OAM, globalTTITickCount_g, __LINE__, RLC_OAM_FILE_NOT_EXIST, 0, 0, 0,
                0, 0, __func__, "");

        return RLC_OAM_FILE_NOT_EXIST;
    }
    else
    { 
        LOG_RLC_MSG(LTE_RLC_BUILD_CONFIGURATION_ERR_1, LOGWARNING, 
                RLC_OAM, globalTTITickCount_g, __LINE__, RLC_OAM_FILE_INCORRECT, 0, 0, 0,
                0, 0, __func__, "");
        /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
        sendErrReportToOAM( responseBuffer_p, sendInd, cellId );
#else
        sendErrReportToOAM( responseBuffer_p, sendInd );
#endif
        /* - Layer2 NON CA Changes */
        return RLC_OAM_FILE_INCORRECT;
    }
}

/****************************************************************************
 * Function Name  : prepareRlcInitIndHeader
 * Inputs         : msg_p- Pointer to indication buffer,
 *                  apiId - API ID for the confirmation,
 *                  msgLength - Pointer to buffer length
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to prepare a message header for the 
                    OAM Indication.
 ****************************************************************************/
/* SPR 5599_129125 Fix Start */
void prepareRlcInitIndHeader ( UInt8 *msg_p, 
/* SPR 5599_129125 Fix End */
                               UInt16 apiId, 
                               UInt16 *msgLength
                             )
{
    UInt16 current = *msgLength;

    LTE_SET_U16BIT(msg_p + current, RLC_MODULE_ID);
    current +=2;
    LTE_SET_U16BIT(msg_p + current, OAM_MODULE_ID);
    current += 2;
    LTE_SET_U16BIT(msg_p + current, apiId);
    current += 2;
    LTE_SET_U16BIT(msg_p + current, RLC_API_HEADER_LEN);

    current +=2;

    *msgLength = current;
}

/****************************************************************************
 * Function Name  : prepareRlcInitIndication
 * Inputs         : msg_p - Pointer to indication buffer,
 *                  msgLength - Pointer to buffer length
 *                  internalCellIdx - internal cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to prepare RLC init indication for OAM.
 ****************************************************************************/
void prepareRlcInitIndication( UInt8 *msg_p, UInt16 *msgLength, 
        InternalCellIndex internalCellIdx )
{
    UInt16 transactionId = 0;
    LTE_SET_U16BIT( msg_p, transactionId);

    *msgLength += 2;
    prepareRlcInitIndHeader( msg_p, RLC_INIT_LAYER_IND, msgLength);

    msg_p[API_HEADER_CELL_INDEX_OFFSET] = internalCellIdx;
    *msgLength = RLC_API_HEADER_LEN;  
        
    LOG_RLC_INFO( RLC_OAM, "Sending RLC Init Indication to OAM" );
}

/***************************************************************
 * Function Name  : rlcSendMsgToOam 
 * Inputs         : ui_CnfMsgBuf_p,
 *                  ui16_msgLen
 * Outputs        : None
 * Returns        : None
 * Description    : This sends an confirmation message to the OAM 
 ****************************************************************/
void rlcSendMsgToOam(UInt8 *ui_CnfMsgBuf_p,
        UInt16 ui16_msgLen )
{
    if (ui16_msgLen != sendto_wrapper(writeOAMSockFD_g, (UInt8 *)ui_CnfMsgBuf_p, ui16_msgLen, 0,
                &(writeOAMAddr_g), sizeof(writeOAMAddr_g)))
    {
        ALARM_MSG (RLC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}

/****************************************************************************
 * Function Name  : rlcValidateIntervalList
 * Inputs         : intList - interval list provided by OAM 
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : Validates interval List array as provided by OAM.
 ****************************************************************************/
/*coverity 53009 +*/
#ifndef UE_SIM_TESTING
STATIC UInt16 rlcValidateIntervalList(UInt16 intList[])
{
    UInt8  idx    = 0;
    UInt16 retVal = RLC_SUCCESS;
    /* Compare every index provided by OAM */
    for( idx = 0; idx < SPS_INTERVAL_LIST; idx++)
    {
        /* Compare with interval list should be either enabled or disabled,
         * else return failure */
        if ( LTE_TRUE < intList[idx] )
        {
            retVal = RLC_FAILURE;
            break;
        }
    }
    return retVal;
}
#endif
/*coverity 53009 -*/



#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : resetRlcPerfContainer
 * Inputs         : resetId
 * Outputs        : None
 * Returns        : None
 * Description    : This function resets RLC PERF Container.
 *******************************************************************************/
void resetRlcPerfContainer(UInt8 resetId)
{  
    switch(resetId)
    {
	case PERF_STATS_CONTAINER_OAM_RLC_RESET_UE_CONTAINER:

            memSet(&(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].
                      perfStats.lteRlcPerfStats.lteRlcUePerfStats), 0 ,PERF_MAX_UE_SUPPORTED * sizeof(RlcUePerfStatsResp));
	    break;

	default:
	    break;
    }
}



/************************************************************************************
 *  Function Name  : ueRlcFillPerfStats
 *  Inputs         : cnfBuff_p - Pointer to confirmation buffer,
 *                   length_p - Pointer to the current idx of cnfBuff_p,
 *                   uePerfStatsBitMap - Stats to be filled,
 *                   ueIndex - stores the UE index to be filled next,
 *                   periodicFlag,
 *                   cellIndex - Internal Cell Index
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This API will fill the stats in the confirmation buffer for OAM.
 *************************************************************************************/  
/* + stats_review_comments_ipc */
void ueRlcFillPerfStats(UInt8 *cnfBuff, UInt32 *length_p, UInt32 uePerfStatsBitMap,
                        UInt32 *ueIndex, UInt32 periodicFlag, InternalCellIndex cellIndex)
{
    UInt32 lcId            = 0;
    UInt32 ueCount         = 0;
    UInt32 length          = 0;
    UInt32 firstBitSet     = 0;
    UInt32 localPerfBitMap = 0; 
    UEContext *ueContext_p         = RLC_NULL ;
/* - stats_review_comments_ipc */

    RlcUePerfStatsResp   *ueRlcPerfStatsResp_p   = PNULL;
    RlcUePerfStatsResp   *pRlcUePerfStatsResp    = PNULL;
    RlcRBPerfStats       *pRlcRBPerfStats        = PNULL;
    UeAckNackPerfStats   *pUeAckNackPerfStats    = PNULL; 
    RlcUePerfStatsInd    *rlcUePerfStatsInd_p    = PNULL;
    RlcGetUePerfStatsCnf *rlcGetUePerfStatsCnf_p = PNULL;
    length = OAM_RLC_API_HEADER_LEN;

    if(periodicFlag)
    {
        rlcUePerfStatsInd_p = (RlcUePerfStatsInd*)&cnfBuff[length];
        rlcUePerfStatsInd_p->duration = (g_perfStatsScannerObj.\
            perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].perfStats.lteRlcPerfStats.lastRLCCalculatedTTI); 
    }
    else
    {
        rlcGetUePerfStatsCnf_p = (RlcGetUePerfStatsCnf*)&cnfBuff[length];
        rlcGetUePerfStatsCnf_p->duration =  (g_perfStatsScannerObj.\
             perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].perfStats.lteRlcPerfStats.lastRLCCalculatedTTI);
    }

    for(ueCount = 0; ueCount < MAX_UE_PER_BATCH && *ueIndex < PERF_MAX_UE_SUPPORTED; (*ueIndex)++)
    {
		/* + stats_review_comments_ipc */
	    /* Check for existance of UE context*/
	    ueContext_p = DB_FETCH_UE_CONTEXT(*ueIndex);
	    if(!ueContext_p)
	    {
	        continue;
		}
		/* Check If Ind Msg, Stats will be sent for All UEs */
		if(API_HEADER_DO_NOT_CARE_CELL_INDEX != cellIndex)
		{
			/* Check if Requested Cell is Primary Cell of UE for CNF Msg */	
			if(ueContext_p->cellIndex != cellIndex)
			{
		    	continue;
			}
		}
		/* - stats_review_comments_ipc */
	/* finding the pointer to Perf Stats resp acc to periodic flag*/
	if(periodicFlag)
	{
	    ueRlcPerfStatsResp_p = (RlcUePerfStatsResp*) (rlcUePerfStatsInd_p->rlcUePerfStatsResp + ueCount);
	    /* + SPR 11670 Fix */
	    ueRlcPerfStatsResp_p->ueIndex= getRrcUeIdxFromUeIdx(*ueIndex);
	    /* - SPR 11670 Fix */
	}
	else
	{
	    ueRlcPerfStatsResp_p = (RlcUePerfStatsResp*) (rlcGetUePerfStatsCnf_p->rlcUePerfStatsResp + ueCount);
	    /* + SPR 11670 Fix */
		ueRlcPerfStatsResp_p->ueIndex= getRrcUeIdxFromUeIdx(*ueIndex);
	    /* - SPR 11670 Fix */
	}

	/* Pointer to store the current UE stats */
	pRlcUePerfStatsResp = &(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].perfStats.
                        	lteRlcPerfStats.lteRlcUePerfStats[*ueIndex]);
	localPerfBitMap = uePerfStatsBitMap;
	/* fills the response Msg for the requested performace events per UE*/
	firstBitSet = ffs_wrapper(localPerfBitMap);
	while (localPerfBitMap)
	{
	    switch (firstBitSet - 1)
	    {
		case PERF_STATS_RB:
		    pRlcRBPerfStats = &(pRlcUePerfStatsResp->rlcRBPerfStat);
		    for (lcId = 0 ; lcId < MAX_LC_IDENTITY ; lcId ++)
		    {
			ueRlcPerfStatsResp_p->rlcRBPerfStat.totalDlRLCUMSduTransmitted[lcId] =
			    pRlcRBPerfStats->totalDlRLCUMSduTransmitted[lcId];
		    }
		break;

		case PERF_STATS_ReTx_EXCEEDED:
		    ueRlcPerfStatsResp_p->totalMaxRetxExceeded = 
			pRlcUePerfStatsResp->totalMaxRetxExceeded;
		break;

		case PERF_STATS_UE_ACK_NACK:
		    pUeAckNackPerfStats = &(pRlcUePerfStatsResp->ueAckNackPerfStats);
		    ueRlcPerfStatsResp_p->ueAckNackPerfStats.totalDLRLCAcks = 
			pUeAckNackPerfStats->totalDLRLCAcks;
		    ueRlcPerfStatsResp_p->ueAckNackPerfStats.totalULRLCAcks = 
			pUeAckNackPerfStats->totalULRLCAcks;
		    ueRlcPerfStatsResp_p->ueAckNackPerfStats.totalULRLCNacks = 
			pUeAckNackPerfStats->totalULRLCNacks;
		break;

		default:
		break;
	    }
        /*coverity-32092*/
        if(firstBitSet)
        {
	        localPerfBitMap = localPerfBitMap & (~(1<<(firstBitSet - 1)));
        }
        /*coverity-32092*/
	    firstBitSet = ffs_wrapper(localPerfBitMap);

	}
	ueCount++;
	length +=sizeof(RlcUePerfStatsResp);
    }
    if(periodicFlag)
    {
         /* The reason for increasing the value of length_p by 8 is that in 
          * case of periodic reporting in conf structure we only duration and
          * no_of_UEs fiels of UInt32 type except the response structso we need
          * to increase the size by 8 bytes */
        *length_p = length + 8;
        /* Number of UEs sent in current batch*/
        rlcUePerfStatsInd_p->numOfUePresent = ueCount;
    }
    else
    {
         /* The reason for increasing the value of length_p by 8 is that in case
          * of non periodic reporting in Indcation structure we have duration 
          * and no_of_UEs and response codefiels of UInt32 type except the 
          * response struct so we need to increase the size by 12 bytes */
        *length_p = length + 12;
        /* Number of UEs sent in current batch*/
        rlcGetUePerfStatsCnf_p->numOfUePresent = ueCount;
    }
}


/****************************************************************************
 *  Function Name  : rlcResetUePerfCounters
 *  Inputs         : pLteUePerfStats - Pointer to LteUePerfStats
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This API will reset all the KPI cumulative counters
 *                   maintained at RLC.
 *********************************************************************************/
void  rlcResetUePerfCounters (LteUePerfStats *pLteUePerfStats)
{
   /* SPR 5449 changes start */
	UInt32 index = 0;
	if ( gPerfStats.perfStats )
	{
		for (index = 0; index < PERF_MAX_UE_SUPPORTED; index++)
		{
			if (DB_FETCH_UE_CONTEXT(index))
			{
				memSet (&pLteUePerfStats[index].lteUeRlcULPerfStats, 0,\
						(sizeof(LteUeRlcULPerfStats)));
				memSet (&pLteUePerfStats[index].lteUeRlcDLPerfStats, 0,\
						(sizeof(LteUeRlcDLPerfStats)));
			}
		}
	}
   /* SPR 5449 changes end */
}

/****************************************************************************
 * Function Name  : handleRlcOamConfigurePerfStatsReq 
 * Inputs         : data_p,
 *                  transactionId,
 *                  respBuff,
 *                  sendInd,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : This API handles RLC OAM Configure PERF Stats Request.
 ****************************************************************************/
	/* + PERF_CA */
void handleRlcOamConfigurePerfStatsReq( UInt8* data_p, UInt16 transactionId,
		 UInt8 *respBuff,UInt32 sendInd, RrcCellIndex  cellId)
{	
    InternalCellIndex cellIndex = 0;
	/* - PERF_CA */
    LtePerfStatsConfig *pOamRlcPerfStatsConfig = &(g_perfStatsScannerObj.\
            perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].perfConfig.lteRlcPerfStatsConfig);
    UInt32 loModuleSrc;
    UInt8  passiveIndexForRlcStats = gPerfStats.perfStatsControlInfo->rlcDLActiveIndex;
    /* Typecasting the Recieved buffer*/
    LP_RlcConfigurePerfStatsReq rlcConfigurePerfStatsReq_p = 
        (LP_RlcConfigurePerfStatsReq) (data_p + OAM_RLC_API_HEADER_LEN);
    /* Typecasting the Response buffer*/
    LP_RlcConfigurePerfStatsCnf rlcConfigurePerfStatsCnf_p =
        (LP_RlcConfigurePerfStatsCnf)(respBuff + OAM_RLC_API_HEADER_LEN);
    loModuleSrc = PERF_STATS_CONTAINER_OAM_RLC;

    if(!(g_perfStatsScannerObj.module_bitmask ))
    {
        g_perfStatsScannerObj.prevPerfStats_tick = globalTTITickCount_g;
    }

    /* Making Header for the Response Message*/ 
    prepareOamCnfHeader(respBuff, RLC_CONFIGURE_PERF_STATS_CNF, &sendInd);
	/* + PERF_CA */
    respBuff[API_HEADER_CELL_INDEX_OFFSET]= cellId;
	/* - PERF_CA */
    /* Set default values in response message */
    rlcConfigurePerfStatsCnf_p->responseCode = RLC_SUCCESS;

    if ( (rlcConfigurePerfStatsReq_p->duration > RLC_MAX_VAL_PERF_TIMER_T ) ||
            (rlcConfigurePerfStatsReq_p->duration < RLC_MIN_VAL_PERF_TIMER_T ))
    {
        rlcConfigurePerfStatsCnf_p->responseCode = RLC_SYNTAX_ERROR;
    }
    else
    {
        if(rlcConfigurePerfStatsReq_p->uePerfStatsToReportBitMap)
        {
            g_perfStatsScannerObj.module_bitmask  |= PERF_STATS_CONTAINER_OAM_RLC_BITMASK;
            pOamRlcPerfStatsConfig->periodicReporting = rlcConfigurePerfStatsReq_p->periodicReporting;
            pOamRlcPerfStatsConfig->duration = (rlcConfigurePerfStatsReq_p->duration) * 1000; 
            g_perfStatsScannerObj.timeoutTTI[loModuleSrc] = pOamRlcPerfStatsConfig->duration; 
            pOamRlcPerfStatsConfig->uePerfStatsToReportBitMap = rlcConfigurePerfStatsReq_p->uePerfStatsToReportBitMap;
            pOamRlcPerfStatsConfig->uePerfStatsToReportBitMap = rlcConfigurePerfStatsReq_p->uePerfStatsToReportBitMap;
            pOamRlcPerfStatsConfig->transactionId = transactionId;
            /* Perf stats RRM-MAC start */
            g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].macOrPdcpOrRlcStats = RLC_PERF_STATS;
            /* Perf stats RRM-MAC end */
            /*Switching the buffer, Toggle */
            (gPerfStats.perfStatsControlInfo->rlcDLActiveIndex) =
                passiveIndexForRlcStats ^0x01;

            /* Switching counter position from passive to active */
	/* + PERF_CA */
            for (cellIndex = 0; cellIndex < numOfRlcCellConfigured_g; cellIndex ++ )
            {
                gRlcCellPerfStats_p[cellIndex] = 
                    &gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->rlcDLActiveIndex].lteCellPerfStats[cellIndex];
            }
	/* - PERF_CA */
            gRlcUePerfStats_p = 
                gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->rlcDLActiveIndex].lteUePerfStats;


            /*reset shm stats */		
            rlcResetUePerfCounters(gPerfStats.perfStats[passiveIndexForRlcStats].lteUePerfStats);
            gPerfStats.perfStats[passiveIndexForRlcStats].rlcDLLastCalculatedTTI = 0;
            gPerfStats.perfStats[passiveIndexForRlcStats].rlcULLastCalculatedTTI = 0;

            /*reset container stats */
            resetRlcPerfContainer(PERF_STATS_CONTAINER_OAM_RLC_RESET_UE_CONTAINER);
            g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].
                perfStats.lteRlcPerfStats.lastRLCCalculatedTTI = 0;
        }
        else
        {
            g_perfStatsScannerObj.module_bitmask  &= ~(PERF_STATS_CONTAINER_OAM_RLC_BITMASK);
        }  
    }

    if ( RLC_SUCCESS != rlcConfigurePerfStatsCnf_p->responseCode )
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_OAM_REQ_ERR, LOGWARNING, (RLC_OAM), globalTTITickCount_g,
                __LINE__, rlcConfigurePerfStatsCnf_p->responseCode, 0,0,0,
                0,0, __func__, "RLC_CONFIGURE_PERF_STATS_CNF" );
    }

    sendInd += 4;

    LTE_SET_U16BIT( (respBuff + API_HEADER_MSGLEN_OFFSET), sendInd);
    rlcSendMsgToOam(
            respBuff, 
            sendInd);

}

/****************************************************************************
 * Function Name  : handleRlcOamGetPerfStatsReq 
 * Inputs         : data_p,
 *                  respBuff,
 *                  sendInd,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : This API handles RLC OAM Get PERF Stats Request.
 *****************************************************************************/
/* + stats_review_comments_ipc */
void handleRlcOamGetPerfStatsReq ( UInt8* data_p, UInt8 *respBuff,UInt32 sendInd
#ifndef UE_SIM_TESTING
			,RrcCellIndex  cellId
#endif
			)
{
#ifndef UE_SIM_TESTING
    InternalCellIndex internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);
#else
    InternalCellIndex internalCellIndex = 0;
#endif
/* - stats_review_comments_ipc */
    /* Typecasting the Recieved buffer*/
    LP_RlcGetPerfStatsReq rlcGetPerfStatsReq_p = 
        (LP_RlcGetPerfStatsReq) (data_p + OAM_RLC_API_HEADER_LEN);
    /* Typecasting the Response buffer*/
    LP_RlcGetUePerfStatsCnf rlcGetUePerfStatsCnf_p =
        (LP_RlcGetUePerfStatsCnf)(respBuff + OAM_RLC_API_HEADER_LEN);

    UInt32 uePerfStatsToGetBitMap = 0;
    UInt32 perfStatsUeIndex       = 0;
    UInt32 current                = 0;
    
    uePerfStatsToGetBitMap = ((rlcGetPerfStatsReq_p->uePerfStatsToGetBitMap) &
                     (g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].\
                                perfConfig.lteRlcPerfStatsConfig.uePerfStatsToReportBitMap)) ;
 
    /* COVERITY CID 64931 fix 28-JUN-2014 start */
    if ( (INVALID_CELL_INDEX != internalCellIndex) && (uePerfStatsToGetBitMap) )
    /* COVERITY CID 64931 fix 28-JUN-2014 end */
    {
         /* Perf stats RRM-MAC start */
         /* SPR 15909 fix start */
         tickType_t rlcLastCalculatedTTI       = 0;
         /* SPR 15909 fix end */
         UInt32 activeIndexForRlcStats  = gPerfStats.perfStatsControlInfo->rlcDLActiveIndex;
         UInt32 passiveIndexForRlcStats    = 0;
         passiveIndexForRlcStats  = activeIndexForRlcStats;
         rlcLastCalculatedTTI = gPerfStats.perfStats[passiveIndexForRlcStats].rlcDLLastCalculatedTTI;

         if (rlcLastCalculatedTTI)
         {
            /*Switching the buffer, Toggle */
            (gPerfStats.perfStatsControlInfo->rlcDLActiveIndex) =
            passiveIndexForRlcStats ^0x01;

            /* Switching counter position from passive to active */
			/* + stats_review_comments_ipc */
           	gRlcCellPerfStats_p[internalCellIndex] =
            		&gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->rlcDLActiveIndex].lteCellPerfStats[internalCellIndex];
			/* - stats_review_comments_ipc */
            gRlcUePerfStats_p =
            gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->rlcDLActiveIndex].lteUePerfStats;
         }
         updateRlcPerfContainer();
         rlcResetUePerfCounters(gPerfStats.perfStats[passiveIndexForRlcStats].lteUePerfStats);
         gPerfStats.perfStats[passiveIndexForRlcStats].rlcDLLastCalculatedTTI = 0;
         gPerfStats.perfStats[passiveIndexForRlcStats].rlcULLastCalculatedTTI = 0;

         /* Perf stats RRM-MAC end */
      

        for (perfStatsUeIndex = 0; perfStatsUeIndex < PERF_MAX_UE_SUPPORTED;)
        {
			/* + stats_review_comments_ipc */
            ueRlcFillPerfStats(respBuff, &current, uePerfStatsToGetBitMap, &perfStatsUeIndex, UNSET_PERIODIC_FLAG,internalCellIndex);
            rlcGetUePerfStatsCnf_p->responseCode = RLC_SUCCESS;

            /* Making Header for the Response Message*/ 
            prepareOamCnfHeader(respBuff, RLC_GET_UE_PERF_STATS_CNF, &sendInd);
			respBuff[API_HEADER_CELL_INDEX_OFFSET] = cellId;
			/* - stats_review_comments_ipc */

            /*SPR 6641 FIX Start*/
            sendInd = OAM_RLC_API_HEADER_LEN;
            /*SPR 6641 FIX End*/


            /* SPR 5453 : FIX Start */
            sendInd += current;
            /* SPR 5453 : FIX End */

            LTE_SET_U16BIT( (respBuff + API_HEADER_MSGLEN_OFFSET), sendInd);
            rlcSendMsgToOam(
            respBuff, 
            sendInd);
            current = 0;
        }
        // Reset Cumulative counters
        if (rlcGetPerfStatsReq_p->resetStats)
        {
	    /* Reset container stats */
            resetRlcPerfContainer(PERF_STATS_CONTAINER_OAM_RLC_RESET_UE_CONTAINER);
            g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].
                        perfStats.lteRlcPerfStats.lastRLCCalculatedTTI = 0;
        }
    }
    else
    {
        /* Making Header for the Response Message*/ 
        prepareOamCnfHeader(respBuff, RLC_GET_UE_PERF_STATS_CNF, &sendInd);
        /* + stats_review_comments_ipc */
        respBuff[API_HEADER_CELL_INDEX_OFFSET] = cellId;
        /* - stats_review_comments_ipc */

        /*SPR 6641 FIX Start*/
        sendInd = OAM_RLC_API_HEADER_LEN;
        /*SPR 6641 FIX End*/

        rlcGetUePerfStatsCnf_p->responseCode = RLC_FAILURE;
        sendInd += 4;

        //LTE_SET_U16BIT((respBuff + (RLC_API_HEADER_LEN - 2)), (sendInd));
        LTE_SET_U16BIT( (respBuff + API_HEADER_MSGLEN_OFFSET), sendInd);
        rlcSendMsgToOam(
                respBuff, 
                sendInd);
    }
}

/****************************************************************************
 *  Function Name  : rlcProcessPerfCounter 
 *  Inputs         : None
 *  Outputs        : None
 *  Returns        : None
 *  Description    : Process Performance Counters in every tick.
 *****************************************************************************/
/* + SPR 17439 */
void rlcProcessPerfCounter(void)
/* - SPR 17439 */    
{
    LtePerfStatsConfig *lteRlcPerfStatsConfig_p =  &(g_perfStatsScannerObj.\
	    perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].perfConfig.lteRlcPerfStatsConfig);
    UInt32 perfStatsUeIndex = 0;
    UInt32 sendInd = 0;
    UInt32 current = 0;
    UInt8 *cnfBuff_p = PNULL;
    UInt8 *resp_p = PNULL;
    UInt32 size  = MAX_OAM_PERF_STATS_SIZE;
    cnfBuff_p = (UInt8 *)getMemFromPool(size,RLC_NULL);
    
    if(PNULL == cnfBuff_p)
    {
       lteWarning("In %s, MEMORY ALLOC FAILURE\n",__func__);
       return;
    } 
    resp_p = cnfBuff_p;
    if (lteRlcPerfStatsConfig_p->periodicReporting)
    {
        if (lteRlcPerfStatsConfig_p->uePerfStatsToReportBitMap)
        {
            current = 0;
            for (perfStatsUeIndex = 0; perfStatsUeIndex < PERF_MAX_UE_SUPPORTED;)
            {
                /* + stats_review_comments_ipc */
                ueRlcFillPerfStats(resp_p
                , &current,lteRlcPerfStatsConfig_p->uePerfStatsToReportBitMap , 
                        &perfStatsUeIndex, SET_PERIODIC_FLAG,API_HEADER_DO_NOT_CARE_CELL_INDEX);
                /* - stats_review_comments_ipc */
                LTE_SET_U16BIT(resp_p, lteRlcPerfStatsConfig_p->transactionId);
                sendInd +=2;
                prepareOamCnfHeader(resp_p, RLC_UE_PERF_STATS_IND, &sendInd);
                /* + PERF_CA */
                resp_p[API_HEADER_CELL_INDEX_OFFSET]= API_HEADER_DO_NOT_CARE_CELL_INDEX ;
                /* - PERF_CA */

                /*SPR 6641 FIX Start*/
                sendInd = OAM_RLC_API_HEADER_LEN;
                /*SPR 6641 FIX End*/

                sendInd += current;
                LTE_SET_U16BIT( (resp_p + API_HEADER_MSGLEN_OFFSET), sendInd);
                rlcSendMsgToOam(
                        cnfBuff_p,
                        sendInd);
                current = 0;
            }
        }
    }

    freeMemPool(cnfBuff_p);

    resetRlcPerfContainer(PERF_STATS_CONTAINER_OAM_RLC_RESET_UE_CONTAINER);

     g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].\
		perfStats.lteRlcPerfStats.lastRLCCalculatedTTI = 0;
}
 

/****************************************************************************
 * Function Name  : ueFillRlcOamPerfContainer
 * Inputs         : pRlcUeOamPerfStats - Pointer to RlcUePerfStatsResp,
 *                  passiveIndexForRlcStats,
 *                  uePerfStatsBitMap
 * Outputs        : None
 * Returns        : None
 * Description    : This API fills UE'S RLC OAM PERF Container.
 *******************************************************************************/
void ueFillRlcOamPerfContainer (RlcUePerfStatsResp *pRlcUeOamPerfStats, 
                                UInt32 passiveIndexForRlcStats,
				UInt32 uePerfStatsBitMap)
{
    UInt32 count = 0;
    UInt32 ueIndex = 0;
    UInt32 localBitmap = 0;
    UInt32 firstBitSet = 0;

    LteUePerfStats *pLteUeMacPerfStats = 
	&((gPerfStats.perfStats[passiveIndexForRlcStats].lteUePerfStats[0]));
    LteUeRlcDLPerfStats *lteUeRlcDLPerfStats_p = PNULL; 
    LteUeRlcULPerfStats *lteUeRlcULPerfStats_p = PNULL;

    for( ueIndex = 0; ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++ )
    {  
	lteUeRlcDLPerfStats_p = &(pLteUeMacPerfStats[ueIndex].lteUeRlcDLPerfStats);
	lteUeRlcULPerfStats_p = &(pLteUeMacPerfStats[ueIndex].lteUeRlcULPerfStats);

	localBitmap = uePerfStatsBitMap;
	firstBitSet = ffs_wrapper (localBitmap);
	/* fills the response Msg for the requested performace events bitwise per UE*/
	while (firstBitSet)
	{
	    switch (firstBitSet - 1)
	    {
		case PERF_STATS_RB:
		    for( count = 0; count<MAX_LC_IDENTITY; count++ )
		    {
			pRlcUeOamPerfStats[ueIndex].rlcRBPerfStat.totalDlRLCUMSduTransmitted[count] +=
			    lteUeRlcDLPerfStats_p->totalDlRLCUMSduTransmitted[count];
		    }
		    break;

		case PERF_STATS_ReTx_EXCEEDED:
            /* +COVERITY 26461 */
		    pRlcUeOamPerfStats[ueIndex].totalMaxRetxExceeded +=
			(lteUeRlcDLPerfStats_p)->totalMaxRetxExceeded;
            /* -COVERITY 26461 */
		    break;

		case PERF_STATS_UE_ACK_NACK:
		    pRlcUeOamPerfStats[ueIndex].ueAckNackPerfStats.totalDLRLCAcks +=
			lteUeRlcDLPerfStats_p->totalDLRLCAcks;

		    pRlcUeOamPerfStats[ueIndex].ueAckNackPerfStats.totalULRLCAcks +=
			lteUeRlcULPerfStats_p->totalULRLCAcks;

		    pRlcUeOamPerfStats[ueIndex].ueAckNackPerfStats.totalULRLCNacks += 
			lteUeRlcULPerfStats_p->totalULRLCNacks;
		    break;

		default:
		    break;    
	    }
	    localBitmap = localBitmap & (~(1<<(firstBitSet - 1)));
	    firstBitSet = ffs_wrapper(localBitmap);
	}
    }
}

/* Perf stats RRM-MAC start */
/****************************************************************************
 * Function Name  : updateRlcPerfContainer
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API updates RLC Perf Container.
 ******************************************************************************/
/* + SPR 17439 */
void updateRlcPerfContainer(void)
/* - SPR 17439 */    
{
    
     /* SPR 15909 fix start */
     tickType_t rlcLastCalculatedTTI       = 0;
     /* SPR 15909 fix end */
     UInt32 passiveIndexForRlcStats    = 0;
     UInt32 activeIndexForRlcStats  = gPerfStats.perfStatsControlInfo->rlcDLActiveIndex; 
     passiveIndexForRlcStats  = activeIndexForRlcStats ^0x01;
    /* Pointer to store the bit map */
    LtePerfStatsConfig *pLteRlcPerfStatsConfig =  &(g_perfStatsScannerObj.\
            perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].perfConfig.lteRlcPerfStatsConfig);

    rlcLastCalculatedTTI = gPerfStats.perfStats[passiveIndexForRlcStats].rlcDLLastCalculatedTTI;

    if (rlcLastCalculatedTTI)
    {
         /* Update all the counters. 
         * For future use: if another containers are also configured then those are  also
         * updated here.
         */
	ueFillRlcOamPerfContainer(&(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].\
		    perfStats.lteRlcPerfStats.lteRlcUePerfStats[0]),
		    passiveIndexForRlcStats,
		    pLteRlcPerfStatsConfig->uePerfStatsToReportBitMap);

	g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].
	   perfStats.lteRlcPerfStats.lastRLCCalculatedTTI += rlcLastCalculatedTTI;

    }
}

#endif
/*spr 22474 start*/
/****************************************************************************
 * Function Name  : handleRlcOamGetDebugInfoReq 
 * Inputs         : recvBuff,
 *                  transactionId,
 *                  respBuff,
 *                  sendInd,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : This API handles RLC OAM Get Debug Info Request.
 *****************************************************************************/
void handleRlcOamGetDebugInfoReq ( UInt8* recvBuff, UInt16 transactionId, UInt8 *respBuff,UInt32 sendInd
#ifndef UE_SIM_TESTING
			,RrcCellIndex  cellId
#endif
			)
{ 
    /* Typecasting the Recieved buffer*/
    RlcGetDebugInfoReqParams *rlcGetDebugInfoReqParams_p = 
        (RlcGetDebugInfoReqParams *)(recvBuff + OAM_RLC_API_HEADER_LEN);
    /* Typecasting the Response buffer*/
    RlcGetDebugInfoRespParams *rlcGetDebugInfoRespParams_p =
                    (RlcGetDebugInfoRespParams *)(respBuff + OAM_RLC_API_HEADER_LEN);
    /* Making Header for the Response Message*/
    prepareOamCnfHeader(respBuff, RLC_OAM_GET_DEBUG_INFO_RESP, &sendInd);


    sendInd = OAM_RLC_API_HEADER_LEN;
#ifndef UE_SIM_TESTING
    respBuff[API_HEADER_CELL_INDEX_OFFSET] = cellId;
#endif
    switch(rlcGetDebugInfoReqParams_p->debugType)
    {
        case GET_DEBUG_INFO_MEM_STATS:
        {
            L2PrepareMemPoolStats (&(rlcGetDebugInfoRespParams_p->u.csplMemPoolStats));
            rlcGetDebugInfoRespParams_p->debugType = GET_DEBUG_INFO_MEM_STATS;
            rlcGetDebugInfoRespParams_p->responseCode = RLC_SUCCESS;
            break;
        }
        case GET_DEBUG_INFO_MSG_STATS:
        {
            L2PrepareMsgPoolStats (&(rlcGetDebugInfoRespParams_p->u.csplMsgPoolStats));
            rlcGetDebugInfoRespParams_p->debugType = GET_DEBUG_INFO_MSG_STATS;
            rlcGetDebugInfoRespParams_p->responseCode = RLC_SUCCESS;
            break;
        }        
        default:
        {
            LOG_RLC_WARNING( RLC_OAM, 
                    "Invalid value of debugType:%u received in RLC_GET_DEBUG_INFO_REQ",
                    rlcGetDebugInfoReqParams_p->debugType);
            rlcGetDebugInfoRespParams_p->responseCode = RLC_FAILURE;
            rlcGetDebugInfoRespParams_p->debugType = rlcGetDebugInfoReqParams_p->debugType;

            break;
        }
    }


    sendInd += sizeof(RlcGetDebugInfoRespParams);
    LTE_SET_U16BIT( (respBuff + API_HEADER_MSGLEN_OFFSET), sendInd);
    rlcSendMsgToOam(
        respBuff,
        sendInd);

}
/*spr 22474 end*/
#ifdef MAC_AUT_TEST
/****************************************************************************
 * Function Name  : fillUeStatus_local
 * Inputs         : ueIndex - Index of UE,
 *                  lteRlcUEStatusCnf_p - Pointer to lteRlcUEStatusCnf,
 *                  statusIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API fills UE Status local used for Automatiion testing.
 ******************************************************************************/
void fillUeStatus_local(
                           UInt16 ueIndex,
                           lteRlcUEStatusCnf * lteRlcUEStatusCnf_p,
                            /* SPR 3653 Fix Start */
                           UInt32 statusIndex
                           /* SPR 3653 Fix End */
			)
{
	fillUeStatus(ueIndex, lteRlcUEStatusCnf_p, statusIndex);
	
}
#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : setLogLevel_local
 * Inputs         : recvBuffer
 * Outputs        : None
 * Returns        : None
 * Description    : This API sets Log level local.used for Automatiion testing.
 ******************************************************************************/
void  setLogLevel_local( 
                          UInt8  *recvBuffer
                       )
{
		setLogLevel(recvBuffer);

}
#endif
#endif


#ifdef LINUX_PC_TEST
void l2oldcasehandleRlcOamInitLayer(void)
{
	//extern UInt8 pdcpInitInd_g;
	//extern UInt8 s1UInitInd_g;
	extern UInt8 initIndTimer_g;
	extern UInt8 rlcInitInd_g;
	extern UInt16 rrcUeIdxToUeIdx[INVALID_RRC_UE_INDEX + 1];

  	UInt16 UeIndex;
	RlcOamInitLayerReq stOamInit = {0};
	stOamInit.numOfUEs = 10;
	stOamInit.sCellCount = 1;
	stOamInit.sCellInfo[0].cellId = 0;
	stOamInit.sCellInfo[0].numOfUEs = 10;
	layer2CommonUpdateCellIndexMapping(0);
	configInitialRlcCfg(&stOamInit, 0);	
	createRLCMemPools();

	//pdcpInitInd_g = 1;
	//s1UInitInd_g = 1;
    initIndTimer_g = 0;
    rlcInitInd_g = 1;

	for (UeIndex = 0;UeIndex<10;UeIndex++)
	{
		allocateUeIdxFromRrcUeIdx(UeIndex);
		setUeIdxValid(UeIndex, UE_IDX_MAPPING_FOR_MAC);
		printf("allocateUeIdxFromRrcUeIdx %d %d \n", UeIndex,rrcUeIdxToUeIdx[UeIndex]);
	}

    return;
}

#endif


