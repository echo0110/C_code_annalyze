/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMacParseOAMMsg.c
 *
 ****************************************************************************
 *
 *  File Description : This file provide APIs to receive request from OAM Layer, 
 *                     configure MAC and send a response to OAM Layer.
 *                     Presently communication with OAM is socket based.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
  $Log: lteMacParseOAMMsg.c,v $
  Revision 1.1.1  2012/07/20 14:56:27  gur31292
  Changes related to DCI Format3A
 
  Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
  FrameworkSetupWith1.0_MAC1.2.4

  Revision 1.34.28.1  2010/07/21 09:29:19  gur24420
  Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
  Overall Changes Cover:-
  1. Merged Endian fixes for receiving values from RRC.

  Revision 1.34  2009/11/15 12:38:08  gur11912
  initDLHarq called

  Revision 1.33  2009/11/12 13:32:35  gur19413
  customize changed

  Revision 1.32  2009/11/11 08:13:39  gur19413
  added printf for initialize mac layer

  Revision 1.31  2009/11/11 07:46:20  gur19413
  modify the max ue support validation

  Revision 1.30  2009/11/11 07:38:01  gur19413
  added validation for MAX UE Support

  Revision 1.29  2009/08/24 12:55:16  gur11912
  stoppedProcessing_g defined as volatile

  Revision 1.28  2009/08/03 08:17:27  gur20052
  Added second param for UT_LOGS

  Revision 1.27  2009/07/31 06:31:22  gur15697
  cleanup Bugfix

  Revision 1.26  2009/07/29 08:05:16  gur11083
  check for (PNULL !=rntiInfo)and change name MAX_DATA_SIZE to MAX_ARRAY_DATA_SIZE

 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacParseOAMMsg.h"
#include "lteMacCellConfMgr.h"
#include "lteOamInterface.h"
#include "lteMacTimingAdvanceMgr.h"
#include "lteMacULCSIManager.h"
#include "lteMacPuschPowerControl.h"
/* + CQI_5.0 */
#include "lteMacPDCCHMgr.h"
/* - CQI_5.0 */
#include "lteMacParseOAMMsg.h"
#include "ltePerfStats.h"
/* + E_CID_5.3 */
#include "lteMacECIDMgr.h"
/* - E_CID_5.3 */
/* Coverity CID 54639 */
#include "lteMacULSpsScheduler.h"
#include "lteMacULResourceInfoMgr.h"
#include "lteMacPUSCH.h"
#include "lteMacFAPIApi.h"
#include "lteMacULFairScheduler.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacDLFairScheduler.h"
#include "lteMacRandomAccess.h"
/* Coverity CID 54632 */

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/
#define VALIDATE_COMMCH_WITH_MIB_ALLOWED(macInitCommChWithMIBAllowed,initParams_p)\
{\
  if(macInitCommChWithMIBAllowed > MAX_COMMCHWITHMIBALLOWED_VALUE)\
  { \
     LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of commChWithMIBAllowed = %d received",\
                    macInitCommChWithMIBAllowed);\
     return MAC_SYNTAX_ERROR; \
  }\
  else\
  {\
    initParams_p->commChWithMIBAllowed = macInitCommChWithMIBAllowed;\
  }\
}

#define VALIDATE_DLSCH_WITH_MIB_ALLOWED(macInitDlSchWithMIBAllowed, initParams_p)\
{\
   if (macInitDlSchWithMIBAllowed > MAX_DLSCHWITHMIBALLOWED_VALUE)\
   {\
       LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of dlSchWithMIBAllowed = %d received",\
                    macInitDlSchWithMIBAllowed);\
       return MAC_SYNTAX_ERROR;\
   }\
   else \
   {\
       initParams_p->dlSchWithMIBAllowed = macInitDlSchWithMIBAllowed; \
   } \
}


#define VALIDATE_QOS_STRATEGY_WEIGHTS_IN_INIT_CELL_REQ(qosStrategyWeights,\
                       initQosStrategyWeights)\
{\
    if (qosStrategyWeights.pdbWeight > MAX_PDB_WEIGHT)\
    {\
        LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of pdbWeight = %d received",\
                   qosStrategyWeights.pdbWeight);\
    }\
    else\
    {\
        initQosStrategyWeights.pdbWeight = qosStrategyWeights.pdbWeight;\
    }\
\
    if (qosStrategyWeights.qciWeight > MAX_QCI_WEIGHT)\
    {\
        LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of qciWeight = %d received",\
                   qosStrategyWeights.qciWeight);\
    }\
    else \
    {\
        initQosStrategyWeights.qciWeight = qosStrategyWeights.qciWeight;\
    }    \
\
    if (qosStrategyWeights.tokenWeight > MAX_TOKEN_WEIGHT)\
    {\
        LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of tokenWeight = %d received",\
                   qosStrategyWeights.tokenWeight);\
    }\
    else \
    {\
        initQosStrategyWeights.tokenWeight = qosStrategyWeights.tokenWeight;\
    } \
\
    if (qosStrategyWeights.qLoadWeight > MAX_QLOAD_WEIGHT)\
    {\
        LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of qLoadWeight = %d received",\
                   qosStrategyWeights.qLoadWeight);\
    }\
    else \
    {\
        initQosStrategyWeights.qLoadWeight = qosStrategyWeights.qLoadWeight;\
    } \
\
    if (qosStrategyWeights.mcsWeight > MAX_MCS_WEIGHT)\
    {\
        LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of mcsWeight = %d received",\
                   qosStrategyWeights.mcsWeight);\
    }\
    else \
    {\
        initQosStrategyWeights.mcsWeight = qosStrategyWeights.mcsWeight;\
    }  \
}


#define VALIDATE_QOS_STRATEGY_WEIGHTS_IN_INIT_LAYER_REQ(qosStrategyWeights,\
                       initQosStrategyWeights)\
{\
    if (qosStrategyWeights.pdbWeight > MAX_PDB_WEIGHT)\
    {\
        LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of pdbWeight = %d received so setting"\
                      " default value",\
                   qosStrategyWeights.pdbWeight);\
        initQosStrategyWeights.pdbWeight =  DEFAULT_PDB_WEIGHT;\
    }\
    else\
    {\
        initQosStrategyWeights.pdbWeight = qosStrategyWeights.pdbWeight;\
    }\
\
    if (qosStrategyWeights.qciWeight > MAX_QCI_WEIGHT)\
    {\
        LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of qciWeight = %d received so setting"\
                    " default value",\
                   qosStrategyWeights.qciWeight);\
        initQosStrategyWeights.qciWeight = DEFAULT_QCI_WEIGHT;\
    }\
    else \
    {\
        initQosStrategyWeights.qciWeight = qosStrategyWeights.qciWeight;\
    }    \
\
    if (qosStrategyWeights.tokenWeight > MAX_TOKEN_WEIGHT)\
    {\
        LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of tokenWeight = %d received so setting"\
                    " default value",\
                   qosStrategyWeights.tokenWeight);\
        initQosStrategyWeights.tokenWeight = DEFAULT_TOKEN_WEIGHT;\
    }\
    else \
    {\
        initQosStrategyWeights.tokenWeight = qosStrategyWeights.tokenWeight;\
    } \
\
    if (qosStrategyWeights.qLoadWeight > MAX_QLOAD_WEIGHT)\
    {\
        LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of qLoadWeight = %d received so"\
                   "setting deafault value",\
                   qosStrategyWeights.qLoadWeight);\
        initQosStrategyWeights.qLoadWeight = DEFAULT_QLOAD_WEIGHT;\
    }\
    else \
    {\
        initQosStrategyWeights.qLoadWeight = qosStrategyWeights.qLoadWeight;\
    } \
\
    if (qosStrategyWeights.mcsWeight > MAX_MCS_WEIGHT)\
    {\
        LOG_MAC_WARNING( MAC_OAM_INF, "Invalid value of mcsWeight = %d received"\
                    " so setting default value",\
                   qosStrategyWeights.mcsWeight);\
        initQosStrategyWeights.mcsWeight = DEFAULT_MCS_WEIGHT;\
    }\
    else \
    {\
        initQosStrategyWeights.mcsWeight = qosStrategyWeights.mcsWeight;\
    }  \
}
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
SInt32 sendMsgToOAM( UInt8 *cnfBuff_p, UInt16 msgLen
        );
/* moved to RRM interface */
/* SPR 21683 +-*/
static void macValidateAndUpdateIntervalList(UInt8 MacIntList[],UInt8 intList[]);
extern void freePendingMemoryPools(InternalCellIndex internalCellIndex);
extern PENDING_DL_MEMORY_POOL_FREE_TYPE *pendingDLMemoryPoolFreeQ_gp[MAX_NUM_CELL];
extern void deInitMacRRMInterface(InternalCellIndex internalCellIndex);
extern ULUESchedulerInfo* ulScheduleInfo_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* ulCellEdgeUserPriorityQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* ZeroNonZeroBSRTriggerQueue_gp[MAX_NUM_CELL];
extern UL_STRATEGY_TX_CIRC_TYPE* ulStrategyTxCircQueue_gp[MAX_NUM_CELL];
extern DeMuxThreadInfo* deMuxThreadPool_gp[MAX_NUM_CELL];
extern PUSCH_SIGNAL_CIRC_TYPE* puschSignalCircQueue_gp[MAX_NUM_CELL];
extern EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE* explicitULPuschTpcScheduleQueue_gp[MAX_NUM_CELL];
extern EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE* explicitULPuschTpcScheduleFailureQueue_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsActQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsDeactQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsReactQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsResRelQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsResRsvQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsStatusQ_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* ZeroNonZeroBSRTriggerQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* ulCellEdgeUserPriorityQueue_gp[MAX_NUM_CELL];
extern ULUESchedulerInfo* ulScheduleInfo_gp[MAX_NUM_CELL];
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
extern DemuxProcessMap* demuxProcessMap_gp[MAX_NUM_CELL];
#endif
extern RRMMACCellGlobal *rrmMacCellGlobal_gp[MAX_NUM_CELL];
extern BuddyPairInfo* buddyPairInfo_gp[MAX_NUM_CELL];
extern ULPfsGlobals *ulPfsGlobals_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* dlUEneroNonZeroQueueLoadTriggerQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE *dlStrategyNewTxQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE   *dlStrategyReTxQueue_gp[MAX_NUM_CELL];
#ifdef DL_UL_SPLIT
extern LTE_SQUEUE *ulStrategyReTxQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE *ulStrategyNewTxQueue_gp[MAX_NUM_CELL];
#endif
extern LTE_SQUEUE *ulStrategyAdvanceTxQueue_gp[MAX_NUM_CELL];
extern PDCCH_ORDER_TYPE *pdcchOrderQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE *pdcchOrderAdvanceQueue_gp[MAX_NUM_CELL];
extern UL_SYNC_LOSS_TRIGGERQ_UL_TYPE* ulSyncLossTriggerQUL_gp[MAX_NUM_CELL];
extern UL_SYNC_LOSS_TRIGGERQ_DL_TYPE* ulSyncLossTriggerQDL_gp[MAX_NUM_CELL];
extern DciCCEContainerInfo *dciCCEContainerInfo_gp[MAX_NUM_CELL];
extern PdcchULOutputInfo *pdcchULOutputInfo_gp[MAX_NUM_CELL];
extern drxExpiryTimerList* drxExpiryTimerList_gp[MAX_NUM_CELL];
extern LTE_SQUEUE *advanceAllocInfoQueue_gp[MAX_NUM_CELL];
extern EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE* explicitDLPucchTpcScheduleQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* macResetQueue_gp[MAX_NUM_CELL];
extern UInt8            *pHIDCICfgMsgBuff_gp[MAX_NUM_CELL] ;
extern UInt8            *pDLCfgMsgBuff_gp[MAX_NUM_CELL]    ;
extern UInt8            *pTxReqMsgBuff_gp[MAX_NUM_CELL]    ;
/* SPR 20445 Fix Start */
extern UInt8 *pULConfigMsg_gp[MAX_NUM_CELL][MAX_SUB_FRAME] ;
/* SPR 20445 Fix End */
extern LTE_SQUEUE* ccchQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE *pendingCcchQueue_gp[MAX_NUM_CELL];
extern PFSGlobals *pfsGlobals_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* ulHighPriorityGrantQueue_gp[MAX_NUM_CELL];
extern void l2_map_ext_loglevel_to_intern_loglevel(UInt8 *p_log_level);
/* SPR23194 Start */
extern UInt8 kpiLcToQciMapping_g[MAX_LCID_THP];
/* SPR23194 End */
//phy release fix
#ifdef LTE_EMBMS_SUPPORTED
FAPI_paramRespParam_stT phyParamResp_g[MAX_NUM_CELL];
void macUpdateSyncInit(UInt32 embmsSyncSeqDuration,
                       UInt16 embmsMaxSyncPeriod,
                       UInt16 embmsSyncMarginTimer);

#endif
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
extern mocnOperatorParams mocnOperatorParams_g;
/* MOCN Changes End */
/* SPR 23402 Changes End */
/* SPR 5531 changes start */
/* CA Changes start */
#ifdef FDD_CONFIG
/* +- SPR 17777 */
void initDLAckNackQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
#endif
#ifdef MAC_AUT_TEST
extern UInt8 macAutFlag ;
#endif
/* + SPR 17439 */
MacRetType createExecutionThreads(void);
/* - SPR 17439 */
/* SPR 5531 changes end */
#ifndef DL_UL_SPLIT_TDD
/* +- SPR 17777 */
void initPUSCHSignalQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex);
void initUlStrategyQueue( 
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex);
/* +- SPR 17777 */

void initMACUEOppertunityIndQ(SInt32 * qId, InternalCellIndex internalCellIndex);
void initULContainerNackQueue ( SInt32 *qId, InternalCellIndex internalCellIndex, UInt8 containerIndex);
void initULContainerNackQueueForTempUE( SInt32 *qId, InternalCellIndex internalCellIndex, UInt8 containerIndex);
void initEncoderQueue ( SInt32 *qId, UInt8 encoderContainerCount,
        InternalCellIndex internalCellIndex,
        InternalCellIndex internalCellIndex2);
#endif
/* + SPR 17439 */
static MacRetType handleMACHealthMonitorStatusReq(void);
/* - SPR 17439 */
STATIC  void processMACInitCellReq(UInt8 *msgBuf_p, 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex);
extern void macInitDLResourceInfoMgr(InternalCellIndex numOfCells);
MacParamsAllCell macParams_g;
ServingCellSpecificParams cellSpecificParams_g;

extern SEM_T waitForULNackQueue_g[MAX_NUM_CELL];
extern SEM_T waitForULNackForTempUE_g[MAX_NUM_CELL];

/* Cyclomatic Complexity changes - starts here */
static  void macVaildateAndUpdateDlSchCfgParams(
        MacInitReqParams *macInitReqParams_p, 
        DLSchedulerStrategyConfig *dlSchdConfig_p
        /* CA changes Start */
        ,InternalCellIndex internalCellIndex
        /* CA changes End */
        );
static  void macVaildateAndUpdateUlSchCfgParams(
        MacInitReqParams *macInitReqParams_p,
        ULSchedulerStrategyConfig *ulSchdConfig_p);
static  void macPrintDlUlSchdConfig (InitParams *initParams_p);
static  void macValidateAndUpdateSpsParams(
        MacInitReqParams *macInitReqParams_p,
        InitParams *initParams_p,
        InternalCellIndex internalCellIndex );
static  void macValidateAndUpdateQciSpsList( UInt8 macQciSpsList[],
        UInt8 qciSpsList[], InternalCellIndex internalCellIndex );
static  void macValidateAndUpdateComIntervalProfile(
        MacInitReqParams   *macInitReqParams_p,
        LP_SpsSysWideParam sysParam_p,
        SpsCommIntervalProfile *spsCommIntervalProfile_p);
static  void macValidateAndUpdateDlIntervalProfile(
        MacInitReqParams   *macInitReqParams_p,
        LP_SpsSysWideParam sysParam_p,
        DLSchedulerStrategyConfig *dlSchdConfig_p);
static  void macValidateAndUpdateUlIntervalProfile(
        MacInitReqParams   *macInitReqParams_p,
        LP_SpsSysWideParam sysParam_p,
        ULSchedulerStrategyConfig *ulSchdConfig_p);
static  void macUpdateQosRelatedParams(
        MacInitReqParams *macInitReqParams_p,
        InitParams *initParams_p);
STATIC  void processMACInitLayerReq( UInt8 *msgBuf_p, 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        /* CA changes Start */
        InternalCellIndex internalCellIndex
        /* CA changes End */
        );
static  void processMACCleanupLayerReq( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
static  void processMACSetLogLevelReq( UInt8 *msgBuf_p, 
        UInt16 msgId,
        InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
static  void processMACGetStatusReq( UInt8 *msgBuf_p, 
        /* +- SPR 17777 */
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex);
static  void processMACResetStatsReq( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex);
static  void processMACGetStatsReq( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex);
#ifdef KPI_STATS
static  void processMACConfigureKpiStatsReq (UInt8 *msgBuf_p, 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        UInt16 msgId,InternalCellIndex  internalCellIndex);
static  void processMACGetKpiStatsReq( UInt8 *msgBuf_p, 
        /* +- SPR 17777 */
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        UInt16 msgId,InternalCellIndex  internalCellIndex);
/* + stats_review_comments_ipc_new */
/* +- SPR 17777 */
static  void sendMacGetKpiThpStatsCnf( UInt8 *msgBuf_p,
        UInt8  *cnfBuff_p,
        UInt16 transactionId,
        UInt8 modCounter,
		InternalCellIndex  internalCellIndex);
/* +- SPR 17777 */
/*spr 19296 fix start*/
/* SPR 22296 Fix Start */
STATIC  void macGetAndSendKpiThpStats( UInt8 *responseBuf, UInt16 transactionId,
        UInt16 kpiBitmap, UInt8 moduleThpId, InternalCellIndex internalCellIndex,UInt16 respMsgId ,UInt8 resetKpi);
/* SPR 22296 Fix End */
/*spr 19296 fix end*/
/* - stats_review_comments_ipc_new */
#endif
static  void processMACGetKpiReq ( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex);
static  void processMACGetLogLevelReq ( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex);
static  void processMACGetLogCategoryReq ( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
       InternalCellIndex  internalCellIndex);
/* +- SPR 17777 */
static  void processMACModifyLayerReq( UInt8 *msgBuf_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex);
static  void processMACUeSinrTaReq( UInt8 *msgBuf_p, 
        UInt16 transactionId,
        InternalCellIndex  internalCellIndex);
/* +- SPR 17777 */
static  void macInterfaceUpdateDlschParamsInInitCellReq(
        MacInitDLPFSParams *macInitDLPFSParams_p, 
        DLSchedulerStrategyConfig *dlSchdConfig_p);
static  void macInterfaceUpdateUlschParamsInInitCellReq(
        MacInitULPFSParams *macInitULPFSParams_p,
        ULSchedulerStrategyConfig *ulSchdConfig_p);
static  void macInterfaceUpdateICICParamsInInitCellReq(
     MacInitICICParams *macInitICICParams_p,
     DLSchedulerStrategyConfig *dlSchdConfig_p,
     ULSchedulerStrategyConfig *ulSchdConfig_p,
     InternalCellIndex internalCellIndex);
static  void macInterfacePrintDLULParamsInInitCellReq(
            DLSchedulerStrategyConfig *dlSchdConfig_p,
            ULSchedulerStrategyConfig *ulSchdConfig_p); 
static  void  macInterfaceUpdateSPSParamsInInitCellReq(
                     MacInitSPSParams *macInitSPSParams_p,
                              InitParams *initParams_p);
static  void  macInterfaceUpdateCommonSPSInfo(
                     MacInitSPSParams *macInitSPSParams_p,
                              InitParams *initParams_p);
static  void  macInterfaceUpdateDlUlSPSIntervalProfileInfo(
                    MacInitSPSParams *macInitSPSParams_p,
                              InitParams *initParams_p);

static  void  macInterfaceUpdateQOSParamsInInitCellReq(
                                       MacInitDLQOSParams *macInitDLQOSParams_p,
                                       DLSchedulerStrategyConfig  *dlSchdConfig_p,
                                       ULSchedulerStrategyConfig  *ulSchdConfig_p);
/* + SPR 17439 */
 UInt8 macCommonGetNumberOfInitReceived(void);
/* - SPR 17439 */
extern void  cleanupMacRRMInterface(InternalCellIndex internalCellIndex);
STATIC  void updateInfoForInitReqRcvd( UInt16 resp,
        InternalCellIndex internalCellIndex);
/** SPR 13173 Fix: Start **/ 
/* Coverity 66974 66975 Start */
/* +- SPR 17777 */
extern MacRetType validateModifyQosParamters(MacModifyQosPamams *p_modifyPamams);
/* Coverity 66974 66975 End */
/** SPR 13173 Fix: End **/ 
/* Cyclomatic Complexity changes - ends here */


/** SPR 13599 Changes Start **/
extern TransmissionAndRLCData *transmissionAndRLCData_g[MAX_NUM_CELL];
/** SPR 13599 Changes End **/
#ifdef LTE_EMBMS_SUPPORTED
extern void initEmbmsMux(UInt8 numOfCells);
extern void initEmbmsEncoder(UInt8 numOfCells);
extern void initEmbmsSync(void);
extern void initMbmsSubFrameMaps(UInt8 numberOfCells);
#endif
/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern void initPUSCH(UInt8 numOfCells);
extern void initPUSCHPowerControl(UInt8 numOfCells);
/* CA Changes end */
/* + SPR 17439 */
#ifdef UTFWK_SIMULATION
extern void updateUlBsrload(void);
extern UInt8 updateQload(void);
extern void updateDlQload(void);
#endif
/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
void initHDGlobals(void);
#endif
/* + SPR 17439 */
/*HD FDD Changes End*/
#ifdef SSI_DEBUG_STATS
/* CA Stats Changes Start */
extern void updateSchConfigStats(InitParams *initParams_p, 
        InternalCellIndex internalCellIndex);
/* CA Stats Changes End */
#endif
extern UInt32 numOfrachInd_g[MAX_NUM_CELL];
#ifdef KPI_STATS
extern UInt16 handleMACConfigureKpiStatReq(UInt8 *msgBuf_p,  UInt16 transactionId_p, UInt8 moduleSrc);
extern kpiScanner g_kpiScannerObj;
/* SPR 10579 changes start*/
extern void updatePdcpKpiContainer(UInt32 passiveIndexForKpiStats, UInt32 moduleId,InternalCellIndex internalCellIndex);
/*spr 15692 fix start*/
extern void updatePdcpKpiThpContainer(UInt32 passiveIndexForKpiThpStats, UInt32 moduleId,InternalCellIndex internalCellIndex);
/*spr 15692 fix end*/
/* SPR 10579 changes end*/
#endif
#ifdef PERF_STATS
/*Global Perf Stats Container*/
extern PerfStatsScanner perfStatsScannerObj_g; 
#endif

/* CA Changes start */
UInt16 setConfigParams(MacInitReqParams *macInitReqParams_p,
        InitParams *initParams_p
        /* CA changes Start */
        ,InternalCellIndex internalCellIndex
        /* CA changes End */
#ifdef LTE_EMBMS_SUPPORTED
                                 ,UInt32 embmsSyncSeqDuration,
                                 UInt16 embmsMaxSyncPeriod,
                                 UInt16 embmsSyncMarginTimer
#endif                                 
        );
UInt16 macInterfaceSetConfigParamsInInitCellReq(
        MacInitCellReqParams *macInitCellReqParams_p,
        InitParams *initParams_p,
        InternalCellIndex internalCellIndex); 

/* Start 128 UE: Memory Changes */
extern void initULConfigMsg(UInt8 numOfCells);
/* End 128 UE: Memory Changes */
extern void init_periodicReportPool(InternalCellIndex internalCellIndex);
MacRetType initializeMAC(UInt8 numOfCells
        );
/*L2_Fix_24*/

extern StatsInfo statsInfo_g[MAX_NUM_CELL];
extern UDouble32 ulBandwidth_g[MAX_NUM_CELL];
extern UDouble32 dlBandwidth_g[MAX_NUM_CELL];
extern UInt32 totalMacUllinkBytesSend_g[MAX_NUM_CELL];
/* CA Changes end */
extern UInt32 totalMacDllinkBytesSend_g;

#ifdef UTFWK_SIMULATION
extern UInt16 rlcLayerInitialized_g;
extern UInt16 parsePeriodicTrafficReq(UInt8 *msgBuf_p);
extern UInt16 parseAperiodicDLTrafficReq(UInt8 *msgBuf_p);
extern UInt16 parseAperiodicULTrafficReq(UInt8 *msgBuf_p);
extern UInt16 parseDrxReq(UInt8 *msgBuf_p);
extern void processDrxReq();
extern UInt16 parseMeasGapReq(UInt8 *msgBuf_p);
extern void processMeasGapReq();
extern UInt16 parseUlSyncLossReq(UInt8 *msgBuf_p);
extern void processUlSyncLossReq();
extern UInt16 parseSRReq(UInt8 *msgBuf_p);
extern void processSRReq();

/* ATB Changes Start */
/* CA Changes start */
extern void populatePreferredMcsRbTableForATB();
/* CA Changes end */
#ifdef PERF_STATS
/* + stats_review_comments_ipc */
void ueMacFillPerfStats(UInt8 *cnfBuff, UInt32 *length_p, 
     UInt32 uePerfStatsBitMap, UInt32 *ueIndex,UInt32 periodicFlag, InternalCellIndex cellIndex);
/* - stats_review_comments_ipc */
#endif
extern UInt16 parseTaReq(UInt8 *msgBuf_p);
UInt8 parseTaFlag_g;
UInt8 recvFromOamDone_g = 0;
/* TDD One Tick Pdu */
#ifdef TDD_CONFIG
UInt16 noOfTickToBeProcess_g = 0;
#endif
#endif

#ifdef KPI_STATS
#ifdef TDD_CONFIG
/** CA-TDD Changes Start **/
extern RCVD_OAM_KPI_MSG_TYPE rcvdOAMKPIMsg_g[MAX_NUM_CELL];
/** CA-TDD Changes Stop **/
extern SInt32 pdcpOamMsgEventFd_g;
#endif
#endif

extern MacRetType fapiSendPhyParamReq(InternalCellIndex internalCellIndex );
/* CA Changes end */
/* SPR 16091 fix start */
extern UInt16 phyInitTransactionId_g;
/* SPR 16091 fix end */

/* SPR 23402 Changes Start */
/* MOCN CHANGES START */
extern void populateMocnParams(MacInitReqParams *macInitReqParams_p);
/* MOCN CHANGES END */
/* SPR 23402 Changes End */

/* Coverity CID 54632 Start */
extern UL_GRANT_APERIODIC_CQI_TYPE ulGrantAperiodicCqiQueue_g;
/* CA Changes start */
extern EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE* explicitULPuschTpcScheduleQueue_gp[MAX_NUM_CELL];
extern EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE* explicitULPuschTpcScheduleFailureQueue_gp[MAX_NUM_CELL];
/* Coverity CID 54632 End */
/* Coverity 54650 */
extern MAC_UE_OPPOR_IND_CIRC_TYPE *macUeOpporIndCircQ_gp[MAX_NUM_CELL];
/* Coverity 54650 */

/*Coverity-5.3.0 CID-54636*/
extern UL_GRANT_PUSCH_QUEUE_TYPE *ulGrantPUSCHFailureQueue_gp[MAX_NUM_CELL];
/*Coverity-5.3.0 CID-54637*/

/*Coverity-5.3.0 CID-54637*/
extern UL_GRANT_PUSCH_QUEUE_TYPE *ulGrantPendingQueue_gp[MAX_NUM_CELL];
/*Coverity-5.3.0 CID-54637*/
/*Coverity-5.3.0 CID-54639*/
extern LTE_CIRCQUEUE* ulSpsActQ_gp[MAX_NUM_CELL];
/*Coverity-5.3.0 CID-54639*/

/*Coverity-5.3.0 CID-54640*/
extern LTE_CIRCQUEUE* ulSpsDeactQ_gp[MAX_NUM_CELL];
/*Coverity-5.3.0 CID-54640*/

/*Coverity-5.3.0 CID-54641*/
extern LTE_CIRCQUEUE* ulSpsReactQ_gp[MAX_NUM_CELL];
/*Coverity-5.3.0 CID-54641*/

/*Coverity-5.3.0 CID-54642*/
extern LTE_CIRCQUEUE* ulSpsResRelQ_gp[MAX_NUM_CELL];
/*Coverity-5.3.0 CID-54642*/

/*Coverity-5.3.0 CID-54643*/
extern LTE_CIRCQUEUE* ulSpsResRsvQ_gp[MAX_NUM_CELL];
/*Coverity-5.3.0 CID-54643*/

/*Coverity-5.3.0 CID-54644*/
extern LTE_CIRCQUEUE* ulSpsStatusQ_gp[MAX_NUM_CELL];
/*Coverity-5.3.0 CID-54644*/

extern UInt16 rlcNumOfSupportedAmLC;
extern UInt16 rlcNumOfSupportedUmLC;
extern UInt16 pdcpNumOfSupportedLC;

extern DeMuxThreadInfo* deMuxThreadPool_gp[MAX_NUM_CELL];
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
extern DemuxProcessMap* demuxProcessMap_gp[MAX_NUM_CELL];
#endif
extern BuddyPairInfo* buddyPairInfo_gp[MAX_NUM_CELL];
extern CellConfigPhy *cellConfigPhy_gp[MAX_NUM_CELL];

#ifdef MAC_AUT_TEST
 extern UInt8  msgBufferForOAMAndRRCMsgs[MAX_OAM_RESP_SIZE];
#endif 

/****************************************************************************
 * Function Name  : initMacLayer
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function initializes the MAC Layer globals 
 ****************************************************************************/
 /* + SPR 17439 */
void initMacLayer(void)
/* - SPR 17439 */    
{
    UInt8 index = 0;
    UInt8 isSocketTobeOpened = FALSE; 
    /* Initialize the parameters */
    memSet(&macParams_g, 0, sizeof(MacParamsAllCell));
    memSet(&cellSpecificParams_g, 0, sizeof(ServingCellSpecificParams));

    for (index = 0; index <= MAX_TRANSMISSION_MODES; index++)
    {
        macParams_g.preferredPrimaryDCIFormatForSingleTB[index] = MAX_NUM_DCI_FORMAT;
        macParams_g.preferredSecondaryDCIFormatForSingleTB[index] = MAX_NUM_DCI_FORMAT;
    }

    macParams_g.numCellsConfigured = INVALID_CELL_INDEX;
    macParams_g.macMaxNumOfLcSupported = 1 + LTE_NUM_OF_SRB;/* 2 SRBs + 1 DRB */
    macParams_g.lowestRunningCellIndex = INVALID_CELL_INDEX;
    /* + SPR_11589 */  
    macParams_g.isInitDone = FALSE;
    /* - SPR_11589 */ 
    /* Get and Initialize memory for first cell */	
    GET_MEMORY_FOR_CELL_INIT_PARAMS(macParams_g.numInitReceived,isSocketTobeOpened);
    if (isSocketTobeOpened)
    { 
#ifndef FLEXRAN
        initPhySocketInterface(macParams_g.numInitReceived);
#endif
    }
    
    
    return;
}
/* CA Changes end */

/****************************************************************************
 * Function Name  : parseMACInitLayerReq
 * Inputs         : msgBuf_p - Pointer to msg read from socket
 *                  internalCellIndex - internalCellIndex of UE
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API parse the init MAC Layer request and initalizes
 *                  MAC Layer after validating the request.
 ****************************************************************************/
UInt16 parseMACInitLayerReq(UInt8 *msgBuf_p
        /* CA changes Start */
        ,InternalCellIndex internalCellIndex
        /* CA changes End */
        )
{
    /* Start AM and UM LC parameters changes */
    UInt8 avgNumUmLcPerUe = 0;
    UInt8 avgNumAmLcPerUe = 0;
    /* End AM and UM LC parameters changes */
#ifdef LTE_EMBMS_SUPPORTED
    UInt32 embmsSyncSeqDuration = 0 ;
    UInt16 embmsMaxSyncPeriod = 0;
    UInt16 embmsSyncMarginTimer = 0;
#endif

    MacInitReqParams *macInitReqParams_p = 
        (MacInitReqParams *)(msgBuf_p + MAC_API_HEADER_LEN);
    /* CA Changes start */
    InitParams *initParams_p = cellSpecificParams_g.\
        cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    UInt16 numOfUe = 0, rntiStartRange = 0, rntiEndRange = 0;
    UInt16 resp = MAC_SUCCESS;

    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,"parseMACInitLayerReq :" 
            "numOfUE = %d\nrntiStartRange = %d\nrntiEndRange = %d\n"
            "dlUeQueueLoadWt = %d\ndlLcQueueLoadWt = %d\ndlMsWt = %d\n"
            "dlMs64QAMFactorWt = %d\ndlMs16QAMFactorWt = %d\n"
            "dlMsQPSKFactorWt = %d\ndlUePriorityWt = %d\n"
            "dlLcPriorityWt =%d\ndlUeDelayWt = %d\ndlLcDelayWt = %d\n"
            "ulMsWt =%d\nulMs64QAMFactorWt=%d\nulMs16QAMFactorWt=%d\nulMsQPSKFactorWt=%d\n"
            "ulUePriorityWt=%d\nulUeDelayWt=%d\nulUeBsrLoadWt=%d\nulLcgPriorityWt=%d\nlogLevel=%d\n"
            "dlSchedulerStrategy=%d\nulSchedulerStrategy=%d\nmaxUEsToBeScheduledDL=%d\n"
            "maxUEsToBeScheduledUL=%d\nulConfigAdvanceSubFrame=%d\nCQIThreshold=%d\n"
            ,macInitReqParams_p->numOfUE,macInitReqParams_p->rntiStartRange,macInitReqParams_p->rntiEndRange,
            macInitReqParams_p->dlUeQueueLoadWt,macInitReqParams_p->dlLcQueueLoadWt,
            macInitReqParams_p->dlMsWt,macInitReqParams_p->dlMs64QAMFactorWt,macInitReqParams_p->dlMs16QAMFactorWt,
            macInitReqParams_p->dlMsQPSKFactorWt,macInitReqParams_p->dlUePriorityWt,macInitReqParams_p->dlLcPriorityWt,
            macInitReqParams_p->dlUeDelayWt,macInitReqParams_p->dlLcDelayWt,
            macInitReqParams_p->ulMsWt,macInitReqParams_p->ulMs64QAMFactorWt,macInitReqParams_p->ulMs16QAMFactorWt,
            macInitReqParams_p->ulMsQPSKFactorWt,macInitReqParams_p->ulUePriorityWt,macInitReqParams_p->ulUeDelayWt,
            macInitReqParams_p->ulUeBsrLoadWt,macInitReqParams_p->ulLcgPriorityWt,macInitReqParams_p->logLevel,
            macInitReqParams_p->dlSchedulerStrategy,macInitReqParams_p->ulSchedulerStrategy,
            macInitReqParams_p->maxUEsToBeScheduledDL,macInitReqParams_p->maxUEsToBeScheduledUL,
            macInitReqParams_p->ulConfigAdvanceSubFrame,macInitReqParams_p->CQIThreshold);   

    if (macInitReqParams_p->fieldBitMask & NUM_OF_CELLS_RECEIVED )
    {
        if ( !(macInitReqParams_p->numCells) ||
                (macInitReqParams_p->numCells > MAX_NUM_CELL))
        {
            LOG_MAC_ERROR( MAC_OAM_INF, "MAC_Init_Layer_Req: Invalid numCells=%u",
                    macInitReqParams_p->numCells);

            return MAC_INIT_LAYER_REQ_INVALID_NUM_OF_CELLS;
        }
        /* update configured  number of cells in macParams */
        macParams_g.numCellsConfigured = macInitReqParams_p->numCells;
#ifdef FLEXRAN
	UInt8 cellIndex = 0;
	for (cellIndex = 0; cellIndex < macParams_g.numCellsConfigured /* bhudev */ ;cellIndex++)
	{

		flexran_ipc_init(macParams_g.numCellsConfigured, //bhudev
				1,//IPC_USED_BY_MAC,
				cellIndex);
	}
#endif
    }
#ifdef FLEXRAN
    else //single cell setup
    {
	UInt8 cellIndex = 0;
        macParams_g.numCellsConfigured = 1;
	for (cellIndex = 0; cellIndex < macParams_g.numCellsConfigured ;cellIndex++)
	{

		flexran_ipc_init(macParams_g.numCellsConfigured, 
				1,//IPC_USED_BY_MAC,
				cellIndex);
	}
    }
#endif

    if (macInitReqParams_p->fieldBitMask & IS_CA_SUPPORTED_RECEIVED)
    {
	/*KLOCKWORK_CHANGES_START_97*/
        if ((macInitReqParams_p->isCASupported != CA_NOT_SUPPORTED) && 
                (macInitReqParams_p->isCASupported != CA_SUPPORTED)) 
        {
	/*KLOCKWORK_CHANGES_STOP_97*/
            lteWarning ("Invalid value of isCASupported from  OAM\n");
            LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                    GET_CURRENT_TICK_FOR_CELL(0), __LINE__, macInitReqParams_p->isCASupported,
                    0,0,0,0,0, __func__, "INVALID VAL OF CA_SUPPORTED");
            return MAC_FAILURE;   
        }
        if(macInitReqParams_p->isCASupported)
        {
            /* CA TDD Changes Start */ 
            /* Code Removed */
            /* CA TDD Changes End */ 
#if (defined(DL_UL_SPLIT)) 
            lteWarning ("Invalid config  received from OAM, CA enabled with DL_UL_SPLIT Flag\n");
            LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                    GET_CURRENT_TICK_FOR_CELL(0), __LINE__, DL_UL_SPLIT,
                    0,0,0,0,0, __func__, "CA enabled with DL_UL_SPLIT Flag");
            return MAC_FAILURE;
#endif
            /*coverity fix  61381 < Dead Code corresponding to UDL_UL_SPLIT removed>*/
            if((DL_QOS_BASED_SCHEDULING != macInitReqParams_p->dlSchedulerStrategy) ||
                    (UL_QOS_SCHEDULING != macInitReqParams_p->ulSchedulerStrategy))
            {
                lteWarning ("Invalid config  received from OAM, CA is enabled. DLSCH startegy =%u ULSCH startegy =%u \n",
                        macInitReqParams_p->dlSchedulerStrategy,
                        macInitReqParams_p->ulSchedulerStrategy);

                LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                        GET_CURRENT_TICK_FOR_CELL(0), __LINE__, macInitReqParams_p->dlSchedulerStrategy,
                        macInitReqParams_p->ulSchedulerStrategy,0,0,0,0, __func__, 
                        "CA enabled with INVALID DL or UL QOS SCHEDULING");

                return MAC_INIT_LAYER_REQ_CA_INVALID_SCH_STRATEGY;
            }
            /*coverity fix  61381*/

        /** SPR 13147 Fix: Start **/ 
#ifndef FAPI_4_0_COMPLIANCE
            //return MAC_FAILURE;
        /** SPR 13147 Fix: End **/ 
#endif
        }

        macParams_g.isCASupported = macInitReqParams_p->isCASupported;
    }
    numOfUe = macInitReqParams_p->numOfUE;
    /* As per section 8.6 in 36.213, there is no MCS limit for a UE that does
     * not support QAM64. So we can use MCS higher than 20. But there is a
     * limit that above MCS 23 the coding rate makes it impossible to decode in a single
     * transmission which requires retransmissions which degrades the UL rate. */
    if( macInitReqParams_p->maxMcs16QAM <= UL_MCS_INDEX_64QAM_23 )
    {
        initParams_p->maxMcs16QAM = macInitReqParams_p->maxMcs16QAM;
    }
    else
    {
        lteWarning ("Invalid macMcs16QAM received from OAM\n");
        LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(0), __LINE__, macInitReqParams_p->maxMcs16QAM,
                0,0,0,0,0, __func__, "Invalid macMcs16QAM");
        return MAC_SYNTAX_ERROR;
    }
    /*SPR 3600 End*/

    rntiStartRange = macInitReqParams_p->rntiStartRange;

    rntiEndRange = macInitReqParams_p->rntiEndRange;
    /*SPR 20119 fix start */
    if((rntiStartRange < 0x01) || (rntiStartRange > 0xFFF2) ||
            (rntiEndRange < 0x01) || (rntiEndRange > 0xFFF2) ||
            (rntiEndRange < rntiStartRange)) 
        /*SPR 20119 fix End */
    {
        lteWarning ("Invalid RNTI Range\n");
        LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(0), __LINE__, rntiStartRange,
                rntiEndRange,0,0,0,0, __func__, "Invalid RNTI Range");
        return MAC_SYNTAX_ERROR;
    }    

    /* UL Config value from OAM */
    macParams_g.ulConfigAdvanceSubFrames = 
        macInitReqParams_p->ulConfigAdvanceSubFrame;
    /* SPR 1571 changes start */
    macParams_g.siSchedulingRATPolicy = macInitReqParams_p->siSchedulingRATPolicy;
    /* SPR 3306 changes start */

    VALIDATE_COMMCH_WITH_MIB_ALLOWED(macInitReqParams_p->commChWithMIBAllowed,
            initParams_p);

    VALIDATE_DLSCH_WITH_MIB_ALLOWED( macInitReqParams_p->dlSchWithMIBAllowed,
            initParams_p);
    /* SPR 3306 changes end */
    /* HSM_CHG_START */
    macParams_g.macHealthMonitoringTimeInterval = 
        ((macInitReqParams_p->healthMonitoringTimeInterval)*1000);

    if(0 == macParams_g.macHealthMonitoringTimeInterval)
    {
        LOG_MAC_MSG(HSM_OAM_MAC_INIT_FAILURE, LOGFATAL, L2_SYS_FAIL,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                macParams_g.macHealthMonitoringTimeInterval,
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"MAC OAM Health Status Init Failure");

        return MAC_FAILURE;
    }
    /* HSM_CHG_END */

#ifndef QOS_SI_FWK_TEST

    /* SPR 22539 Fix + */
	if (macInitReqParams_p->enableAllocAfterAllowedBitrate > ENABLE_ALLOW_ALLOC_BITRATE_TOKEN_BASED)
    {
        lteWarning("Invalid value of enableAllocAfterAllowedBitrate = %d"
				" received.. Setting value as ENABLE_ALLOW_ALLOC_BITRATE_QLOAD_BASED",
                macInitReqParams_p->enableAllocAfterAllowedBitrate);

         initParams_p->enableAllocAfterAllowedBitrate = ENABLE_ALLOW_ALLOC_BITRATE_QLOAD_BASED;
         /* SPR 22539 Fix - */
    }
    else
    { 
        initParams_p->enableAllocAfterAllowedBitrate =
            macInitReqParams_p->enableAllocAfterAllowedBitrate;
    }

    if((macInitReqParams_p->dlRBRestrictionFactor >= 
                MIN_DL_RB_RESTRICTION_FACTOR) && 
            (macInitReqParams_p->dlRBRestrictionFactor <= 
             MAX_DL_RB_RESTRICTION_FACTOR))
    {
        initParams_p->dlRBRestrictionFactor = 
            macInitReqParams_p->dlRBRestrictionFactor;
    }
    else
    {
        lteWarning ("Invalid value of dlRBRestrictionFactor = %d received\n"
                ,macInitReqParams_p->dlRBRestrictionFactor);
        LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(0), __LINE__, macInitReqParams_p->dlRBRestrictionFactor,
                0,0,0,0,0, __func__, "Invalid value of DL RBRestrictionFactor");
        return MAC_SYNTAX_ERROR;
    }

    if((macInitReqParams_p->ulRBRestrictionFactor >= 
                MIN_UL_RB_RESTRICTION_FACTOR) && 
            (macInitReqParams_p->ulRBRestrictionFactor <= 
             MAX_UL_RB_RESTRICTION_FACTOR))
    {
        initParams_p->ulRBRestrictionFactor = 
            macInitReqParams_p->ulRBRestrictionFactor;
    }
    else
    {
        lteWarning ("Invalid value of ulRBRestrictionFactor = %d received\n"
                ,macInitReqParams_p->ulRBRestrictionFactor);
        LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(0), __LINE__, macInitReqParams_p->ulRBRestrictionFactor,
                0,0,0,0,0, __func__, "Invalid value of UL RBRestrictionFactor");
        return MAC_SYNTAX_ERROR;
    }
#else
    /* SPR 22539 Fix + */
    initParams_p->enableAllocAfterAllowedBitrate = ENABLE_ALLOW_ALLOC_BITRATE_QLOAD_BASED;
    /* SPR 22539 Fix - */
    initParams_p->dlRBRestrictionFactor = 3;
    initParams_p->ulRBRestrictionFactor = 3;
#endif
    /* Spr 23393 Changes Start */
    if((macInitReqParams_p->maxRarPduPerTTI > 0)&&
        (macInitReqParams_p->maxRarPduPerTTI <= MAX_RAR_PDU_PER_TTI)) 
    {
        macParams_g.rarPduPerTTI = macInitReqParams_p->maxRarPduPerTTI;
    }
    else
    {
        lteWarning ("Invalid value for RAR per TTI reveived from OAM, rarPduPerTTI = %d\n",
                macInitReqParams_p->maxRarPduPerTTI);
            macParams_g.rarPduPerTTI = MAX_RAR_PDU_PER_TTI;
    }
    /* Spr 23393 Changes End */
#ifdef LTE_EMBMS_SUPPORTED
    embmsSyncSeqDuration = macInitReqParams_p->embmsSyncSeqDuration;
    embmsMaxSyncPeriod   = macInitReqParams_p->embmsMaxSyncPeriod;
    embmsSyncMarginTimer = macInitReqParams_p->embmsSyncMarginTimer;
#endif
    if( (RAT_LOCALIZED != macParams_g.siSchedulingRATPolicy) 
            && (RAT_DISTRIBUTED != macParams_g.siSchedulingRATPolicy))
    {
        lteWarning ("Invalid value for RAT Policy from OAM, siSchedulingRATPolicy = %d\n"
                ,macParams_g.siSchedulingRATPolicy);
        LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(0), __LINE__, macParams_g.siSchedulingRATPolicy,
                0,0,0,0,0, __func__, "Invalid value for RAT Policy");
        return MAC_SYNTAX_ERROR;
    }
    /* SPR 1571 changes end */

    if (macParams_g.ulConfigAdvanceSubFrames > MAX_UL_CONFIG_ADVANCE_SUB_FRAMES)
    {
        lteWarning ("Invalid value for UL Config Advance SubFrames = [%d]."
                " Setting default value - [%d].\n",
                macParams_g.ulConfigAdvanceSubFrames,
                DEFAULT_UL_CONFIG_ADVANCE_SUB_FRAMES);
        LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(0), __LINE__, macParams_g.ulConfigAdvanceSubFrames,
                0,0,0,0,0, __func__, "Invalid UL ConfigAdvanceSubFrames, set to default");
        macParams_g.ulConfigAdvanceSubFrames =
            DEFAULT_UL_CONFIG_ADVANCE_SUB_FRAMES;
    }

    if ( numOfUe <= MAX_UE_SUPPORTED)
    {
        /* Start 128 UE: Memory Changes */
        initParams_p->maxNumOfUeSupported = numOfUe;
        initParams_p->macMaxNumOfUeSupported = (numOfUe * LTE_CREATE_MEMPOOL_EXTRA_BUF_FACTOR);
        /* End 128 UE: Memory Changes */
    }
    else
    {
        /* Start AM and UM LC parameters changes */
        lteWarning("Invalid Configuration against MAC UE SUPPORTED.  From OAM, Received numOfUe [%d]\n", numOfUe);
        LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(0), __LINE__,numOfUe,
                0,0,0,0,0, __func__, "Invalid number of UE");

        resp = MAC_FAILURE;
        /* End AM and UM LC parameters changes */
    }
    /* Start AM and UM LC parameters changes */
    if (MAC_SUCCESS == resp )
    {
        /*
         * Total of avgNumUmLcPerUE and avgNumAmLcPerUE should be greater than or
         * equal to 1 and less than or equal to 8.
         */
        if ((MIN_TOTAL_LC_PER_UE_FOR_MEM <= (macInitReqParams_p->avgNumAmLcPerUE +
                        macInitReqParams_p->avgNumUmLcPerUE)) &&
                (MAX_TOTAL_LC_PER_UE_FOR_MEM >= (macInitReqParams_p->avgNumAmLcPerUE +
                                                 macInitReqParams_p->avgNumUmLcPerUE)))
        {
            /* Value of  MAX_AM_LC_PER_UE_FOR_MEM  and MAX_TOTAL_LC_PER_UE_FOR_MEM
             * are same but in future values might be changed so below check is required.
             */  
            /* Parsing of AM LC parameter */
            /*COVERITY FIX-31421 START*/
            if (macInitReqParams_p->avgNumAmLcPerUE <= MAX_AM_LC_PER_UE_FOR_MEM)
            /*COVERITY FIX-31421 END*/
            {
                avgNumAmLcPerUe = macInitReqParams_p->avgNumAmLcPerUE;

                /* Parsing of UM LC parameter */
                /*COVERITY FIX-31421 START*/
                if (macInitReqParams_p->avgNumUmLcPerUE <= MAX_UM_LC_PER_UE_FOR_MEM)
                /*COVERITY FIX-31421 END*/
                {
                    avgNumUmLcPerUe = macInitReqParams_p->avgNumUmLcPerUE;
                    /* Both parameters are valid. Populate macMaxNumOfLcSupported_g variable */
                    macParams_g.macMaxNumOfLcSupported = 
                        avgNumAmLcPerUe + avgNumUmLcPerUe + LTE_NUM_OF_SRB;
                }
                else
                {
                    lteWarning("Invalid value for avgNumUmLcPerUe."
                            " From OAM, Received avgNumUmLcPerUe [%u]\n", 
                            macInitReqParams_p->avgNumUmLcPerUE);
                    LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                            GET_CURRENT_TICK_FOR_CELL(0),
                            __LINE__,macInitReqParams_p->avgNumUmLcPerUE,
                            0,0,0,0,0, __func__, "Invalid AvgNumUmLcPerUe");

                    resp = MAC_FAILURE;
                }
            }
            else
            {
                lteWarning("Invalid value for avgNumAmLcPerUe."
                        " From OAM, Received avgNumAmLcPerUe [%u]\n",
                        macInitReqParams_p->avgNumAmLcPerUE);
                LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                        GET_CURRENT_TICK_FOR_CELL(0), __LINE__,
                        macInitReqParams_p->avgNumAmLcPerUE,0,
                        0,0,0,0, __func__, "Invalid AvgNumAmLcPerUe");

                resp = MAC_FAILURE;
            }
        }
        else
        {
            lteWarning("Invalid value for avgNumUmLcPerUe and avgNumAmLcPerUe."
                    " Received avgNumUmLcPerUe_g [%u] and avgNumAmLcPerUe [%u]\n",
                    macInitReqParams_p->avgNumUmLcPerUE, macInitReqParams_p->avgNumAmLcPerUE);

            LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                    GET_CURRENT_TICK_FOR_CELL(0), __LINE__,
                    macInitReqParams_p->avgNumAmLcPerUE,
                    macInitReqParams_p->avgNumUmLcPerUE,
                    0,0,0,0, __func__, "Invalid Sum of AvgNumUmLcPerUe and AvgNumAmLcPerUe");
            resp = MAC_FAILURE;
        }
    }

    if (MAC_SUCCESS == resp)
    {
        /*
         ** Populate the LC global variable of RLC and PDCP,
         ** Not being used in RLC and PDCP before  initializing
         ** here.
         */
        /* RLC */
        rlcNumOfSupportedUmLC_g = avgNumUmLcPerUe;
        rlcNumOfSupportedAmLC_g = avgNumAmLcPerUe + LTE_NUM_OF_SRB;
        /* PDCP */
        pdcpNumOfSupportedLC_g  = 
            avgNumUmLcPerUe + avgNumAmLcPerUe + LTE_NUM_OF_SRB;
        /* End AM and UM LC parameters changes */

        initParams_p->lowerRNTIBound = rntiStartRange;
        initParams_p->upperRNTIBound = rntiEndRange;
        initParams_p->crntiRange = 
            initParams_p->upperRNTIBound - initParams_p->lowerRNTIBound;

        /* Enabling Logs by default */
        enableLog_g = 1;
        setConfigParams( macInitReqParams_p, initParams_p, 
                internalCellIndex 
#ifdef LTE_EMBMS_SUPPORTED
                ,embmsSyncSeqDuration,
                embmsMaxSyncPeriod,
                embmsSyncMarginTimer
#endif
                );

#ifdef UTFWK_SIMULATION
        rlcLayerInitialized_g = 1;
#endif

        /* + CQI_5.0 */
        /* Validating and storing operator preferred primary DCI format for SISO */
        /* Cyclomatic_complexity_changes_start */
        if(MAC_FAILURE == storePrimaryDCIFormat(macInitReqParams_p)){
            LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                    GET_CURRENT_TICK_FOR_CELL(0), __LINE__,
                    MAC_FAILURE,
                    0,0,0,0,0, __func__, "StorePrimaryDCIFormat returned failed");
            return MAC_FAILURE;
        }     
        /* Cyclomatic_complexity_changes_end */

        /* ATB Changes Start */
        populatePreferredMcsRbTableForATB();
        /* SPR 23402 Changes Start */
        /* MOCN CHANGES START */
        populateMocnParams(macInitReqParams_p);
        /* MOCN CHANGES END */
        /* SPR 23402 Changes End */
    }

    return resp;
}

/* CA Changes start */
/****************************************************************************
 * Function Name  : macInterfaceParseMACInitCellReq
 * Inputs         : msgBuf_p - Pointer to msg read from socket
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS or FAILURE
 * Variables      :
 * Description    : This API parse the init Cell request for other cells 
 *                  and initalizes MAC Layer after validating the request.
 ****************************************************************************/
UInt16 macInterfaceParseMACInitCellReq(UInt8 *msgBuf_p,
        InternalCellIndex internalCellIndex)
{
    MacInitCellReqParams *macInitCellReqParams_p = 
        (MacInitCellReqParams *)(msgBuf_p + MAC_API_HEADER_LEN);

    InitParams *initParams_p = cellSpecificParams_g.
        cellConfigAndInitParams_p[internalCellIndex]->initParams_p;

    InitParams *initLayerParams_p = cellSpecificParams_g.
        cellConfigAndInitParams_p[0]->initParams_p;

    /* Copying all the paramters from MAC INIT Layer request to this.
     * In case any wrong value comes then default value will be from 
     * MAC INIT Layer request.
     */ 
    memCpy(initParams_p, initLayerParams_p, sizeof(InitParams));

    UInt16 numOfUe = 0, rntiStartRange = 0, rntiEndRange = 0;
    UInt16 resp = MAC_SUCCESS;
    UInt32 fieldBitMask =  macInitCellReqParams_p->fieldBitMask;


    if (macInitCellReqParams_p->isLastAttempt > MAX_IS_LAST_ATTEMPT)
    {
        initParams_p->isLastAttempt = MIN_IS_LAST_ATTEMPT;     
    } 
    else 
    {
        initParams_p->isLastAttempt = macInitCellReqParams_p->isLastAttempt;
    }

    numOfUe = macInitCellReqParams_p->numOfUE;

    /*MCS index received from OAM should be minimum 20*/
    if((macInitCellReqParams_p->maxMcs16QAM >= UL_MCS_INDEX_16QAM_20)
            && ( macInitCellReqParams_p->maxMcs16QAM <=  UL_MCS_INDEX_64QAM_28))
    {
        initParams_p->maxMcs16QAM = macInitCellReqParams_p->maxMcs16QAM;
    }
    else
    {
        LOG_MAC_ERROR( MAC_OAM_INF, "Invalid macMcs16QAM[%u] received from OAM",
                macInitCellReqParams_p->maxMcs16QAM );
        return MAC_SYNTAX_ERROR;
    }

    rntiStartRange = macInitCellReqParams_p->rntiStartRange;

    rntiEndRange = macInitCellReqParams_p->rntiEndRange;

    if((rntiStartRange < 0x01) || (rntiStartRange > 0xFFF2) ||
            (rntiEndRange < 0x01) || (rntiEndRange > 0xFFF2) ||
            (rntiEndRange <= rntiStartRange)) 
    {
        LOG_MAC_ERROR( MAC_OAM_INF, "Invalid RNTI Range: Start[%u] End[%u]",
                rntiStartRange, rntiEndRange );
        return MAC_SYNTAX_ERROR;
    }    

    /* If CA is enabled then RNTIs range should not clash */
    if ((macParams_g.isCASupported) &&
            !( (rntiEndRange < initLayerParams_p->lowerRNTIBound) ||
                (rntiStartRange > initLayerParams_p->upperRNTIBound)))
    { 
        return MAC_INIT_CELL_REQ_RNTI_RANGE_CLASHED;
    }

    initParams_p->lowerRNTIBound = rntiStartRange;
    initParams_p->upperRNTIBound = rntiEndRange;
    initParams_p->crntiRange = 
        initParams_p->upperRNTIBound - initParams_p->lowerRNTIBound;
    VALIDATE_COMMCH_WITH_MIB_ALLOWED(macInitCellReqParams_p->commChWithMIBAllowed,
            initParams_p);

    VALIDATE_DLSCH_WITH_MIB_ALLOWED(macInitCellReqParams_p->dlSchWithMIBAllowed,
            initParams_p);

#ifndef QOS_SI_FWK_TEST

    if((macInitCellReqParams_p->dlRBRestrictionFactor >= 
                MIN_DL_RB_RESTRICTION_FACTOR) && 
            (macInitCellReqParams_p->dlRBRestrictionFactor <= 
             MAX_DL_RB_RESTRICTION_FACTOR))
    {
        initParams_p->dlRBRestrictionFactor = 
            macInitCellReqParams_p->dlRBRestrictionFactor;
    }
    else
    {
        lteWarning ("Invalid value of dlRBRestrictionFactor = %d received"
                " setting same value as received in MAC_INIT_LAYER_REQ\n",
                macInitCellReqParams_p->dlRBRestrictionFactor);
    }

    if((macInitCellReqParams_p->ulRBRestrictionFactor >= 
                MIN_UL_RB_RESTRICTION_FACTOR) && 
            (macInitCellReqParams_p->ulRBRestrictionFactor <= 
             MAX_UL_RB_RESTRICTION_FACTOR))
    {
        initParams_p->ulRBRestrictionFactor = 
            macInitCellReqParams_p->ulRBRestrictionFactor;
    }
    else
    {
        lteWarning ("Invalid value of ulRBRestrictionFactor = %d received"
                " setting same value as received in MAC_INIT_LAYER_REQ\n",
                macInitCellReqParams_p->ulRBRestrictionFactor);
    }
#else
    /* SPR 22539 Fix + */
    initParams_p->enableAllocAfterAllowedBitrate = ENABLE_ALLOW_ALLOC_BITRATE_QLOAD_BASED;
    /* SPR 22539 Fix - */
    initParams_p->dlRBRestrictionFactor = 3;
    initParams_p->ulRBRestrictionFactor = 3;
#endif

    if ((numOfUe >= MIN_UE_SUPPORTED) && (numOfUe <= MAX_UE_SUPPORTED))
    {
        initParams_p->maxNumOfUeSupported = numOfUe;
        initParams_p->macMaxNumOfUeSupported = (numOfUe * LTE_CREATE_MEMPOOL_EXTRA_BUF_FACTOR);
    }
    else
    {
        lteWarning("Invalid Configuration against MAC UE SUPPORTED."
                " From OAM, Received numOfUe [%d]\n", numOfUe);
        LOG_MAC_MSG(OAM_MAC_INVALID_PARAM_RECV,LOGERROR,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex), __LINE__,numOfUe,
                0,0,0,0,0, __func__, "INV UE VAL");

        resp = MAC_FAILURE;
    }

    /* Setting  ul scheduler strategy */
    if (UL_SCHEDULAR_STRATEGY & fieldBitMask)
    {
        if (macInitCellReqParams_p->ulSchedulerStrategy == UL_FAIR_SCHEDULING ||
                macInitCellReqParams_p->ulSchedulerStrategy == UL_ROUND_ROBIN_SCHEDULING ||
                macInitCellReqParams_p->ulSchedulerStrategy == UL_QOS_SCHEDULING)
        {
            initParams_p->ulSchdConfig.ulSchedulerStrategy =
                (ULSchedulerStrategyType)macInitCellReqParams_p->ulSchedulerStrategy;
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for ulSchedulerStrategy "
                    "Value will be set same as received in MAC_INIT_LAYER_REQ\n");
        }
    }

    /* Setting  dl scheduler strategy */
    if ((DL_SCHEDULAR_STRATEGY & fieldBitMask) &&  (!macParams_g.isCASupported))
    {
        if (macInitCellReqParams_p->dlSchedulerStrategy == DL_FAIR_SCHEDULING ||
                macInitCellReqParams_p->dlSchedulerStrategy == DL_ROUND_ROBIN_SCHEDULING ||
                macInitCellReqParams_p->dlSchedulerStrategy == DL_QOS_BASED_SCHEDULING)
        {
            initParams_p->dlSchdConfig.dlSchedulerStrategy =
                (DLSchedulerStrategyType)macInitCellReqParams_p->dlSchedulerStrategy;
        }
        else
        {
            /* Value from first cell will be set */
            LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for dlSchedulerStrategy "
                    "Value will be set same as received in MAC_INIT_LAYER_REQ\n");
        }
    }

    if ((macInitCellReqParams_p->maxUEsToBeScheduledUL > MAX_UL_UE_SCHEDULED) ||
            (macInitCellReqParams_p->maxUEsToBeScheduledUL < MIN_UL_UE_SCHEDULED))
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK UE TO BE SCHEDULED "
                "Setting its max value - %d \n", MAX_UL_UE_SCHEDULED);
    }
    else
    {
        initParams_p->ulSchdConfig.maxUEsToBeScheduledUL =
            macInitCellReqParams_p->maxUEsToBeScheduledUL;
    }

    if ((macInitCellReqParams_p->maxUEsToBeScheduledDL > MAX_DL_UE_SCHEDULED) ||
            ( macInitCellReqParams_p->maxUEsToBeScheduledDL < MIN_DL_UE_SCHEDULED))
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for DOWNLINK UE TO BE SCHEDULED "
                "Setting its max value - %d.\n",
                MAX_DL_UE_SCHEDULED);
    }
    else
    {
        initParams_p->dlSchdConfig.maxUEsToBeScheduledDL =
            macInitCellReqParams_p->maxUEsToBeScheduledDL;
    }
    /* DESIGN REVIEW CHANGES INCORPORATION */
    /* SPR 12975 Changes Start */
    if(macInitCellReqParams_p->dlGbrTokenLimitFactor < 
            (SInt8) initParams_p->dlSchdConfig.gbrBktFactor)
    {
        initParams_p->dlSchdConfig.gbrTokenLimitFactor = 
            macInitCellReqParams_p->dlGbrTokenLimitFactor;
    }
    else
    {
        lteWarning ("Invalid value of dlGbrTokenLimitFactor = %d received,"
                " setting same value which is = %d as received in MAC_INIT_LAYER_REQ\n",
                macInitCellReqParams_p->dlGbrTokenLimitFactor,
                initParams_p->dlSchdConfig.gbrTokenLimitFactor);
    }
    if(macInitCellReqParams_p->ulGbrTokenLimitFactor < 
            (SInt8) initParams_p->ulSchdConfig.gbrBktFactor)
    {
        initParams_p->ulSchdConfig.gbrTokenLimitFactor = 
            macInitCellReqParams_p->ulGbrTokenLimitFactor;
    }
    else
    {
        lteWarning ("Invalid value of ulGbrTokenLimitFactor = %d received,"
                " setting same value which is = %d as received in MAC_INIT_LAYER_REQ\n",
                macInitCellReqParams_p->ulGbrTokenLimitFactor,
                initParams_p->ulSchdConfig.gbrTokenLimitFactor);
    }
    if(macInitCellReqParams_p->dlAmbrTokenLimitFactor < 
            (SInt8) initParams_p->dlSchdConfig.ambrBktFactor)
    {
        initParams_p->dlSchdConfig.ambrTokenLimitFactor = 
            macInitCellReqParams_p->dlAmbrTokenLimitFactor;
    }
    else
    {
        lteWarning ("Invalid value of dlAmbrTokenLimitFactor = %d received,"
                " setting same value which is = %d as received in MAC_INIT_LAYER_REQ\n",
                macInitCellReqParams_p->dlAmbrTokenLimitFactor,
                initParams_p->dlSchdConfig.ambrTokenLimitFactor);
    }
    if(macInitCellReqParams_p->ulAmbrTokenLimitFactor < 
            (SInt8) initParams_p->ulSchdConfig.ambrBktFactor)
    {
        initParams_p->ulSchdConfig.ambrTokenLimitFactor = 
            macInitCellReqParams_p->ulAmbrTokenLimitFactor;
    }
    else
    {
        lteWarning ("Invalid value of ulAmbrTokenLimitFactor = %d received,"
                " setting same value which is = %d as received in MAC_INIT_LAYER_REQ\n",
                macInitCellReqParams_p->ulAmbrTokenLimitFactor,
                initParams_p->ulSchdConfig.ambrTokenLimitFactor);
    }
    /* SPR 12975 Changes End */

    if ((MIN_CQI_INDEX > macInitCellReqParams_p->CQIThreshold) ||
            (macInitCellReqParams_p->CQIThreshold > MAX_CQI_INDEX)) 
    {
        lteWarning ("Invalid value of CQIThreshold = %d received "
                " setting same value as received in MAC_INIT_LAYER_REQ\n",
                macInitCellReqParams_p->CQIThreshold);
        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                macInitCellReqParams_p->CQIThreshold,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For CC_CE Region");
    }
    else 
    { 
        initParams_p->dlSchdConfig.cqiThreshold[CC_CE_REGION] = 
            macInitCellReqParams_p->CQIThreshold;
    }

    if ( (macInitCellReqParams_p->CQIThresholdLayer3 < CQI_MIN) || 
            (macInitCellReqParams_p->CQIThresholdLayer3 > CQI_MAX) )
    {
        lteWarning ("Invalid value for CQI ThresHold for layer 3= %d"
                " Setting value as received in MAC_INIT_LAYER_REQ - .\n",
                macInitCellReqParams_p->CQIThresholdLayer3);
        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                macInitCellReqParams_p->CQIThresholdLayer3,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For Layer3 CC_CE Region");
    }
    else
    {
        initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer3[CC_CE_REGION] = 
            macInitCellReqParams_p->CQIThresholdLayer3;
    }

    if ( (macInitCellReqParams_p->CQIThresholdLayer4 < CQI_MIN) || 
            (macInitCellReqParams_p->CQIThresholdLayer4 > CQI_MAX))
    {
        lteWarning ("Invalid value for CQI Threshold for layer 4= %d"
                " Setting value as received in MAC_INIT_LAYER_REQ .\n",
                macInitCellReqParams_p->CQIThresholdLayer4);

        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                macInitCellReqParams_p->CQIThresholdLayer4,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For Layer4 CC_CE Region");
    }
    else 
    {
        initParams_p->dlSchdConfig.cqiThresholdCellCenterLayer4[CC_CE_REGION] = 
            macInitCellReqParams_p->CQIThresholdLayer4;
    }

    if (MAC_SUCCESS == resp)
    {
        macInterfaceSetConfigParamsInInitCellReq(macInitCellReqParams_p,
                initParams_p,internalCellIndex);
    }
    return resp;
}

/****************************************************************************
 * Function Name  : setConfigParams
 * Inputs         : macInitReqParams_p - Pointer to initialization parameters,
 *                  initParams_p,
 *                  internalCellIndex - internalCellIndex  of UE
 * Outputs        : None
 * Returns        : MAc_SUCCESS or reason for failure
 * Description    : This API initialize the config parameters received from 
 *                  OAM at init time.
 ****************************************************************************/
/* CA Changes start */
UInt16 setConfigParams(MacInitReqParams *macInitReqParams_p,
                       InitParams *initParams_p
                       /* CA changes Start */
                       ,InternalCellIndex internalCellIndex
                       /* CA changes End */
#ifdef LTE_EMBMS_SUPPORTED
                       ,UInt32 embmsSyncSeqDuration,
                       UInt16 embmsMaxSyncPeriod,
                       UInt16 embmsSyncMarginTimer
#endif
        )
{
    UInt32 logLevel=0;
    
    /* Cyclomatic Complexity changes - starts here */
    /* Validate and Update DL Scheduler Config parameters */
    macVaildateAndUpdateDlSchCfgParams (macInitReqParams_p,&(initParams_p->dlSchdConfig)
                       /* CA changes Start */
                       ,internalCellIndex
                       /* CA changes End */
    );
    /* Validate and Update UL Scheduler Config parameters */
    macVaildateAndUpdateUlSchCfgParams (macInitReqParams_p,&(initParams_p->ulSchdConfig));
    /* Cyclomatic Complexity changes - ends here */

    /*  SPR  3444 changes start */
    /* 
       if(macInitReqParams_p->logLevel > MAX_LOG_LEVEL)
       {
       logLevel = MAX_LOG_LEVEL ;
       lteWarning ("Invalid value for Log Level\
       Setting its max value - %d.\n",
       MAX_LOG_LEVEL );

       } 
       else 
       {
       logLevel = macInitReqParams_p->logLevel;
       }    
     */
    logLevel = macInitReqParams_p->logLevel;
    /*  SPR  3444 changes end */ 
    l2_map_ext_loglevel_to_intern_loglevel((UInt8 *)&logLevel);
    lteShmSetModuleLogLevel(MAC_MODULE,logLevel);

    /* Cyclomatic Complexity changes - starts here */
    /* UT logs to print DL and UL scheduler config parameters */
    macPrintDlUlSchdConfig (initParams_p);
    /* Validate and Update SPS parameters */
    macValidateAndUpdateSpsParams(macInitReqParams_p,initParams_p, 
            internalCellIndex);
    /* Update Qos related parameters */
    macUpdateQosRelatedParams (macInitReqParams_p,initParams_p);
    /* Cyclomatic Complexity changes - ends here */
#ifdef LTE_EMBMS_SUPPORTED
    macUpdateSyncInit(embmsSyncSeqDuration ,embmsMaxSyncPeriod ,embmsSyncMarginTimer);
#endif    
    return MAC_SUCCESS;
}

/* CA Changes end */
/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : macVaildateAndUpdateDlSchCfgParams 
 * Inputs         : macInitReqParams_p - Pointer to initialization parameters,
 *                  dlSchdConfig_p,
 *                  internalCellIndex - internalCellIndex of UE
 * Outputs        : None
 * Returns        : None 
 * Variables      : None
 * Description    : This function validates and updates DL Scheduler config
 *                  parameters from Init Request received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void macVaildateAndUpdateDlSchCfgParams(
        MacInitReqParams *macInitReqParams_p,
        DLSchedulerStrategyConfig *dlSchdConfig_p
        ,InternalCellIndex internalCellIndex
        )
{
    if(macInitReqParams_p->dlUeQueueLoadWt > MAC_SCH_MAX_DWLK_UE_QL_WT)
    {
        dlSchdConfig_p->dwlkUEQLoadWt = MAC_SCH_MAX_DWLK_UE_QL_WT;
        lteWarning ("Invalid value for Downlink UE QLoad weight.\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_DWLK_UE_QL_WT);
    } else {
        dlSchdConfig_p->dwlkUEQLoadWt = macInitReqParams_p->dlUeQueueLoadWt;
    }    

    if(macInitReqParams_p->dlMsWt > MAC_SCH_MAX_DWLK_MS_WT)
    {
        dlSchdConfig_p->dwlkMSWt = MAC_SCH_MAX_DWLK_MS_WT;
        lteWarning ("Invalid value for Downlink ModeScheme weight.\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_DWLK_MS_WT);
    } else {
        dlSchdConfig_p->dwlkMSWt = macInitReqParams_p->dlMsWt;
    }    

    if(macInitReqParams_p->dlLcQueueLoadWt > MAC_SCH_MAX_DWLK_LC_QL_WT)
    {
        dlSchdConfig_p->dwlkLCQLoadWt = MAC_SCH_MAX_DWLK_LC_QL_WT;
        lteWarning ("Invalid value for Downlink LC QLoad weight.\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_DWLK_LC_QL_WT);
    } else {
        dlSchdConfig_p->dwlkLCQLoadWt = macInitReqParams_p->dlLcQueueLoadWt;
    }    

    if(macInitReqParams_p->dlUePriorityWt > MAC_SCH_MAX_DWLK_UE_PR_WT)
    {
        dlSchdConfig_p->dwlkUEPriorityWt = MAC_SCH_MAX_DWLK_UE_PR_WT ;
        lteWarning ("Invalid value for Downlink UE Priority weight\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_DWLK_UE_PR_WT);
    } else {
        dlSchdConfig_p->dwlkUEPriorityWt = macInitReqParams_p->dlUePriorityWt;
    }    

    if(macInitReqParams_p->dlLcPriorityWt > MAC_SCH_MAX_DWLK_LC_PR_WT)
    {
        dlSchdConfig_p->dwlkLCPriorityWt = MAC_SCH_MAX_DWLK_LC_PR_WT ;
        lteWarning ("Invalid value for Downlink LC Priority weight\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_DWLK_LC_PR_WT);
    } else {
        dlSchdConfig_p->dwlkLCPriorityWt = macInitReqParams_p->dlLcPriorityWt;
    }    

    if(macInitReqParams_p->dlUeDelayWt > MAC_SCH_MAX_DWLK_UE_DELAY_WT)
    {
        dlSchdConfig_p->dwlkUEDelayWt = MAC_SCH_MAX_DWLK_UE_DELAY_WT;
        lteWarning ("Invalid value for Downlink UE Delay weight\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_DWLK_UE_DELAY_WT);
    } else {
        dlSchdConfig_p->dwlkUEDelayWt = macInitReqParams_p->dlUeDelayWt;
    }    

    if(macInitReqParams_p->dlLcDelayWt > MAC_SCH_MAX_DWLK_LC_DELAY_WT)
    {
        dlSchdConfig_p->dwlkLCDelayWt = MAC_SCH_MAX_DWLK_LC_DELAY_WT;
        lteWarning ("Invalid value for Downlink LC Delay weight\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_DWLK_LC_DELAY_WT);
    } else {
        dlSchdConfig_p->dwlkLCDelayWt = macInitReqParams_p->dlLcDelayWt;
    }    
    /* + CL-MIMO LJA*/
    /* ICIC chnages start */
    /*4x4 DL MIMO CHG START*/ 
    if(macInitReqParams_p->CQIThresholdLayer3 > CQI_MIN && 
            macInitReqParams_p->CQIThresholdLayer3 <= CQI_MAX)
    {
        dlSchdConfig_p->cqiThresholdCellCenterLayer3[CC_CE_REGION]= 
            macInitReqParams_p->CQIThresholdLayer3;
    }
    else
    {
        dlSchdConfig_p->cqiThresholdCellCenterLayer3[CC_CE_REGION] = CQI_THRESHOLD_DEFAULT;
        lteWarning ("Invalid value for CQI ThresHold \
                Setting its max value - %d.\n",
                CQI_THRESHOLD_DEFAULT);
        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                dlSchdConfig_p->cqiThresholdCellCenterLayer3[CC_CE_REGION],
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For Layer3 CC_CE Region");
    }

    if(macInitReqParams_p->CQIThresholdLayer4 > CQI_MIN &&
            macInitReqParams_p->CQIThresholdLayer4 <= CQI_MAX)
    {
        dlSchdConfig_p->cqiThresholdCellCenterLayer4[CC_CE_REGION]=
            macInitReqParams_p->CQIThresholdLayer4;
    }
    else
    {
        dlSchdConfig_p->cqiThresholdCellCenterLayer4[CC_CE_REGION] = CQI_THRESHOLD_DEFAULT;
        lteWarning ("Invalid value for CQI ThresHold \
                Setting its max value - %d.\n",
                CQI_THRESHOLD_DEFAULT);
        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                dlSchdConfig_p->cqiThresholdCellCenterLayer4[CC_CE_REGION],
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For Layer4 CC_CE Region");
    }

    if(macInitReqParams_p->CQIThresholdCellEdgeLayer3 >= CQI_MIN &&
            macInitReqParams_p->CQIThresholdCellEdgeLayer3 <= CQI_MAX)
    { 
        dlSchdConfig_p->cqiThresholdCellCenterLayer3[CE_REGION]= 
            macInitReqParams_p->CQIThresholdCellEdgeLayer3; 
    }
    else
    {
        dlSchdConfig_p->cqiThresholdCellCenterLayer3[CE_REGION] = CQI_THRESHOLD_DEFAULT;
        lteWarning ("Invalid value for CQI ThresHold \
                Setting its max value - %d.\n",
                CQI_THRESHOLD_DEFAULT);
        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                dlSchdConfig_p->cqiThresholdCellCenterLayer3[CE_REGION], 
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For layer3 CE Region");
    }

    if(macInitReqParams_p->CQIThresholdCellEdgeLayer4 >= CQI_MIN && 
            macInitReqParams_p->CQIThresholdCellEdgeLayer4 <= CQI_MAX)
    {
        dlSchdConfig_p->cqiThresholdCellCenterLayer4[CE_REGION]= 
            macInitReqParams_p->CQIThresholdCellEdgeLayer4; 
    }
    else
    {
        dlSchdConfig_p->cqiThresholdCellCenterLayer4[CE_REGION] = CQI_THRESHOLD_DEFAULT;
        lteWarning ("Invalid value for CQI ThresHold \
                Setting its max value - %d.\n",
                CQI_THRESHOLD_DEFAULT);
        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                dlSchdConfig_p->cqiThresholdCellCenterLayer4[CE_REGION], 
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For Layer4 CE Region");
    }
    /*4x4 DL MIMO CHG END*/
    /* SPR 9656 changes start */
    if( (macInitReqParams_p->CQIThreshold >= CQI_MIN) &&
            (macInitReqParams_p->CQIThreshold <= CQI_MAX) ) 
    {    
        dlSchdConfig_p->cqiThreshold[CC_CE_REGION] = macInitReqParams_p->CQIThreshold;
    }
    else
    {
        dlSchdConfig_p->cqiThreshold[CC_CE_REGION] = CQI_THRESHOLD_DEFAULT ;
        lteWarning("In %s, Invalid value for macInitReqParams_p->CQIThreshold = %d\n",
                __func__,macInitReqParams_p->CQIThreshold);
        return;
    }   
    /* SPR 9656 changes end */
    /* + SPR_11306 */
    if ((macInitReqParams_p->cellEdgeUECQIThreshold < MIN_CQI_INDEX) ||
            (macInitReqParams_p->cellEdgeUECQIThreshold > (MAX_CQI_INDEX - 1))) 
    {
        dlSchdConfig_p->cqiThreshold[CE_REGION] = MAX_CQI_INDEX - 1;
        /* - SPR_11306 */
        lteWarning ("Invalid value of CQIThreshold = %d received "
                " setting max value\n",macInitReqParams_p->cellEdgeUECQIThreshold);
        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(1),
                macInitReqParams_p->cellEdgeUECQIThreshold,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For CC_CE Region");
    }
    else 
    { 
        dlSchdConfig_p->cqiThreshold[CE_REGION] = macInitReqParams_p->
            cellEdgeUECQIThreshold;
    }
    /* ICIC chnages end */
    /* - CL-MIMO LJA*/
    /* SPR 7981 changes start */
    if(!dlSchdConfig_p->cqiThreshold[CC_CE_REGION])
    {
        dlSchdConfig_p->mcsThreshold[CC_CE_REGION] = 0;
    }
    else
    {	
        dlSchdConfig_p->mcsThreshold[CC_CE_REGION] = getMCSFromCQI(
                dlSchdConfig_p->cqiThreshold[CC_CE_REGION]
                );
    }
    if(!dlSchdConfig_p->cqiThreshold[CE_REGION])
    {
        dlSchdConfig_p->mcsThreshold[CE_REGION] = 0;
    }
    else
    {
        dlSchdConfig_p->mcsThreshold[CE_REGION] = getMCSFromCQI(
                dlSchdConfig_p->cqiThreshold[CE_REGION]
                );
    }
    /* SPR 7981 changes end */
    /* setting Downlink CQi Wt */
    dlSchdConfig_p->dwlkcqiWt = MAC_SCH_DEFAULT_DWLK_CQI_WT;

    if (macInitReqParams_p->maxUEsToBeScheduledDL > MAX_DL_UE_SCHEDULED )
    {
        dlSchdConfig_p->maxUEsToBeScheduledDL = MAX_DL_UE_SCHEDULED  ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, 
                "Invalid value for DOWNLINK UE TO BE SCHEDULED\
                Setting its max value - %d.\n",
                MAX_DL_UE_SCHEDULED);
    }
    else
        dlSchdConfig_p->maxUEsToBeScheduledDL =
            macInitReqParams_p->maxUEsToBeScheduledDL;
    /* ICIC changes start */
    if (macInitReqParams_p->maxCellEdgeUEsToBeScheduledDL >
            dlSchdConfig_p->maxUEsToBeScheduledDL )
    {
        dlSchdConfig_p->maxCellEdgeUEsToBeScheduledDL = 
            dlSchdConfig_p->maxUEsToBeScheduledDL;
    }
    else
    {
        dlSchdConfig_p->maxCellEdgeUEsToBeScheduledDL =
            macInitReqParams_p->maxCellEdgeUEsToBeScheduledDL;
    }
    /* ICIC changes end */

    /* Setting  dl scheduler strategy*/
    if (macInitReqParams_p->dlSchedulerStrategy == DL_FAIR_SCHEDULING ||
            macInitReqParams_p->dlSchedulerStrategy == DL_ROUND_ROBIN_SCHEDULING ||
            macInitReqParams_p->dlSchedulerStrategy == DL_QOS_BASED_SCHEDULING)
    {
        dlSchdConfig_p->dlSchedulerStrategy =
            /* + coverity 32187 */
            (DLSchedulerStrategyType) macInitReqParams_p->dlSchedulerStrategy  ;
        /* - coverity 32187 */
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for dlSchedulerStrategy\n");
    }

    /* Setting Downlink Modulation Scheme Weight for 64QAM*/
    if (macInitReqParams_p->dlMs64QAMFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        dlSchdConfig_p->dlMs64QAMFactorWt = MAC_MAX_MODULATION_SCHEME_WT  ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "Invalid value for DL 64 QAM mod scheme Setting its max value - %d.\n",
                MAC_MAX_MODULATION_SCHEME_WT);
    }
    else
        dlSchdConfig_p->dlMs64QAMFactorWt =
            macInitReqParams_p->dlMs64QAMFactorWt;

    /* Setting Downlink Modulation Scheme Weight for 16QAM*/
    if (macInitReqParams_p->dlMs16QAMFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        dlSchdConfig_p->dlMs16QAMFactorWt = MAC_MAX_MODULATION_SCHEME_WT  ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "Invalid value for DL 16 QAM mod scheme Setting its max value - %d.\n",
                MAC_MAX_MODULATION_SCHEME_WT);
    }
    else
        dlSchdConfig_p->dlMs16QAMFactorWt =
            macInitReqParams_p->dlMs16QAMFactorWt;

    /* Setting Downlink Modulation Scheme Weight for QPSK*/
    if (macInitReqParams_p->dlMsQPSKFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        dlSchdConfig_p->dlMsQPSKFactorWt = MAC_MAX_MODULATION_SCHEME_WT  ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,  
                "Invalid value for DL 16 QAM mod scheme Setting its max value - %d.\n",
                MAC_MAX_MODULATION_SCHEME_WT);
    }
    else
        dlSchdConfig_p->dlMsQPSKFactorWt =
            macInitReqParams_p->dlMsQPSKFactorWt;

    if (DL_ROUND_ROBIN_SCHEDULING == dlSchdConfig_p->dlSchedulerStrategy )
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "dlSchedulerStrategy = DL_ROUND_ROBIN_SCHEDULING \n");
    }
    else if(DL_FAIR_SCHEDULING == dlSchdConfig_p->dlSchedulerStrategy)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "dlSchedulerStrategy  = DL_FAIR_SCHEDULING\n");
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "dlSchedulerStrategy  = DL_QOS_BASED_SCHEDULING\n");
    }
}

/* CA Changes end */
/* CA Changes start */
/****************************************************************************
 * Function Name  : macVaildateAndUpdateUlSchCfgParams 
 * Inputs         : macInitReqParams_p - Pointer to initialization parameters
 *                  ulSchdConfig_p -
 * Outputs        : None
 * Returns        : None 
 * Variables      : None
 * Description    : This function validates and updates UL Scheduler config
 *                  parameters from Init Request received from OAM.
 ****************************************************************************/

static  void macVaildateAndUpdateUlSchCfgParams(
        MacInitReqParams *macInitReqParams_p,
        ULSchedulerStrategyConfig *ulSchdConfig_p)
{
    if (macInitReqParams_p->ulUeBsrLoadWt > MAC_SCH_MAX_UPLK_UE_BSR_WT)
    {
        ulSchdConfig_p->uplkUEBSRLoadWt = MAC_SCH_MAX_UPLK_UE_BSR_WT;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK BSR WEIGHT\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_UPLK_UE_BSR_WT);
    }
    else
    {
        ulSchdConfig_p->uplkUEBSRLoadWt = macInitReqParams_p->ulUeBsrLoadWt;
    }

    /* Setting Uplink UE Delay Wt */
    /* Coverity 65024 Start */
    if (macInitReqParams_p->ulUeDelayWt > MAC_SCH_MAX_UPLK_UE_DELAY_WT)
    /* Coverity 65024 End */
    {
        ulSchdConfig_p->uplkUEDelayWt = MAC_SCH_MAX_UPLK_UE_DELAY_WT;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK UE DELAY WEIGHT\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_UPLK_UE_DELAY_WT);
    }
    else
    {
        ulSchdConfig_p->uplkUEDelayWt = macInitReqParams_p->ulUeDelayWt;
    }
    /* Setting Uplink UE Priority Wt */
    /* Coverity 65024 Start */
    if (macInitReqParams_p->ulUePriorityWt > MAC_SCH_MAX_UPLK_UE_PR_WT)
    /* Coverity 65024 End */
    {
        ulSchdConfig_p->uplkUEPriorityWt = MAC_SCH_MAX_UPLK_UE_PR_WT ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK UE PRIORITY WEIGHT\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_UPLK_UE_PR_WT);
    }
    else
    {
        ulSchdConfig_p->uplkUEPriorityWt = macInitReqParams_p->ulUePriorityWt;
    }

    /* Setting Uplink MS Wt */
    /* Coverity 65024 Start */
    if (macInitReqParams_p->ulMsWt > MAC_SCH_MAX_UPLK_MS_WT )
    /* Coverity 65024 End */
    {
        ulSchdConfig_p->uplkMSWt = MAC_SCH_MAX_UPLK_MS_WT ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK UE MS WEIGHT\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_UPLK_MS_WT);
    }
    else
    {
        ulSchdConfig_p->uplkMSWt = macInitReqParams_p->ulMsWt;
    }
    /* Setting Uplink LCG Priority Wt */
    /* No Need to check MIN value of ulLcgPriorityWt as MAC_SCH_MIN_UPLK_MS_WT is Zero */ 
    /* Coverity 65024 Start */
    if (macInitReqParams_p->ulLcgPriorityWt > MAC_SCH_MAX_UPLK_LCG_PR_WT )
    /* Coverity 65024 End */
    {
        ulSchdConfig_p->uplkLCGPriorityWt = MAC_SCH_MAX_UPLK_LCG_PR_WT  ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK LCG PRIORITY WEIGHT\
                Setting its max value - %d.\n",
                MAC_SCH_MAX_UPLK_LCG_PR_WT);
    }
    else
    {
        ulSchdConfig_p->uplkLCGPriorityWt = macInitReqParams_p->ulLcgPriorityWt;
    }

    if (macInitReqParams_p->maxUEsToBeScheduledUL > MAX_UL_UE_SCHEDULED )
    {
        ulSchdConfig_p->maxUEsToBeScheduledUL = MAX_UL_UE_SCHEDULED  ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK UE TO BE SCHEDULED\
                Setting its max value - %d.\n",
                MAX_UL_UE_SCHEDULED);
    }
    else
        ulSchdConfig_p->maxUEsToBeScheduledUL =
            macInitReqParams_p->maxUEsToBeScheduledUL;
    /* ICIC changes start */
    if (macInitReqParams_p->maxCellEdgeUEsToBeScheduledUL >
            ulSchdConfig_p->maxUEsToBeScheduledUL )
    {
        ulSchdConfig_p->maxCellEdgeUEsToBeScheduledUL =
            ulSchdConfig_p->maxUEsToBeScheduledUL ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK UE TO BE SCHEDULED\
                Setting its max value - %d.\n",
                ulSchdConfig_p->maxUEsToBeScheduledUL);
    }
    else
    {
        ulSchdConfig_p->maxCellEdgeUEsToBeScheduledUL =
            macInitReqParams_p->maxCellEdgeUEsToBeScheduledUL;
    }
    /* ICIC changes end */

    /* Setting  ul scheduler strategy*/
    if (macInitReqParams_p->ulSchedulerStrategy == UL_FAIR_SCHEDULING ||
            macInitReqParams_p->ulSchedulerStrategy == UL_ROUND_ROBIN_SCHEDULING ||
            macInitReqParams_p->ulSchedulerStrategy == UL_QOS_SCHEDULING)
    {
        ulSchdConfig_p->ulSchedulerStrategy =
        /* + coverity 32188 */
           (ULSchedulerStrategyType) macInitReqParams_p->ulSchedulerStrategy  ;
        /* - coverity 32188 */
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for ulSchedulerStrategy\n");
    }

    /* Setting uplink Modulation Scheme Weight for 64QAM*/
    if (macInitReqParams_p->ulMs64QAMFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        ulSchdConfig_p->ulMs64QAMFactorWt = MAC_MAX_MODULATION_SCHEME_WT  ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "Invalid value for UL 64 QAM mod scheme Setting its max value - %d.\n",
                MAC_MAX_MODULATION_SCHEME_WT);
    }
    else
        ulSchdConfig_p->ulMs64QAMFactorWt =
            macInitReqParams_p->ulMs64QAMFactorWt;

    /* Setting uplink Modulation Scheme Weight for 16QAM*/
    if (macInitReqParams_p->ulMs16QAMFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        ulSchdConfig_p->ulMs16QAMFactorWt = MAC_MAX_MODULATION_SCHEME_WT  ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, 
                "Invalid value for UL 16 QAM mod scheme Setting its max value - %d.\n",
                MAC_MAX_MODULATION_SCHEME_WT);
    }
    else
        ulSchdConfig_p->ulMs16QAMFactorWt =
            macInitReqParams_p->ulMs16QAMFactorWt;

    /* Setting uplink Modulation Scheme Weight for QPSK*/
    if (macInitReqParams_p->ulMsQPSKFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        ulSchdConfig_p->ulMsQPSKFactorWt = MAC_MAX_MODULATION_SCHEME_WT  ;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, 
                "Invalid value for UL 64 QAM mod scheme Setting its max value - %d.\n",
                MAC_MAX_MODULATION_SCHEME_WT);
    }
    else
        ulSchdConfig_p->ulMsQPSKFactorWt =
            macInitReqParams_p->ulMsQPSKFactorWt;

}
/* CA Changes end */
/* CA Changes start */
/****************************************************************************
 * Function Name  : macPrintDlUlSchdConfig
 * Inputs         : initParams_p- pointer to cell init context
 * Outputs        : None
 * Returns        : None 
 * Description    : This function prints DL and UL scheduler config parameters.
 ****************************************************************************/

static  void macPrintDlUlSchdConfig (InitParams *initParams_p)
{

    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_UE_QL_Wt     : %d\n",initParams_p->dlSchdConfig.dwlkUEQLoadWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_MS64_QAM_FACTOR_WT : %d\n",initParams_p->dlSchdConfig.dlMs64QAMFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_MS16_QAM_FACTOR_WT : %d\n",initParams_p->dlSchdConfig.dlMs16QAMFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_MS_QPSK_FACTOR_WT : %d\n",initParams_p->dlSchdConfig.dlMsQPSKFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_MS_WT        : %d\n",initParams_p->dlSchdConfig.dwlkMSWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_LC_QL_WT     : %d\n",initParams_p->dlSchdConfig.dwlkLCQLoadWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_UE_PR_WT     : %d\n",initParams_p->dlSchdConfig.dwlkUEPriorityWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_LC_PR_WT     : %d\n",initParams_p->dlSchdConfig.dwlkLCPriorityWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_UE_DEL_WT    : %d\n",initParams_p->dlSchdConfig.dwlkUEDelayWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_LC_DEL_WT    : %d\n",initParams_p->dlSchdConfig.dwlkLCDelayWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DOWNLINK UE TO BE SCHEDULED = %d\n",initParams_p->dlSchdConfig.maxUEsToBeScheduledDL);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_MS64_QAM_FACTOR_WT     : %d\n",initParams_p->ulSchdConfig.ulMs64QAMFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_MS16_QAM_FACTOR_WT : %d\n",initParams_p->ulSchdConfig.ulMs16QAMFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_MS_QPSK_FACTOR_WT : %d\n",initParams_p->ulSchdConfig.ulMsQPSKFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_UE_PR_WT     : %d\n",initParams_p->ulSchdConfig.uplkUEPriorityWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_MS_WT        : %d\n",initParams_p->ulSchdConfig.uplkMSWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_UE_DEL_WT    : %d\n",initParams_p->ulSchdConfig.uplkUEDelayWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_LCG_PR_WT    : %d\n",initParams_p->ulSchdConfig.uplkLCGPriorityWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UPLINK UE TO BE SCHEDULED = %d\n",initParams_p->ulSchdConfig.maxUEsToBeScheduledUL);
    /* +- SPR 17777 */
    /* Coverity 96710 Fix Start*/
  LTE_GCC_UNUSED_PARAM(initParams_p)
    /* Coverity 96710 Fix End*/
    /* +- SPR 17777 */
}

/****************************************************************************
 * Function Name  : macValidateAndUpdateSpsParams 
 * Inputs         : macInitReqParams_p - Pointer to initialization parameters
 *                  initParams_p- pointer to cell init context
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None 
 * Description    : This function validates and updates SPS parameters from
 *                  Init Request received from OAM.
 ****************************************************************************/
static  void macValidateAndUpdateSpsParams(
        MacInitReqParams *macInitReqParams_p,
        InitParams *initParams_p, 
        InternalCellIndex internalCellIndex )
{
    SpsSysWideParam   *spsSysWideParam_p =  &initParams_p->spsSysWideParam;
    SpsCommIntervalProfile *spsCommIntervalProfile_p =
                     initParams_p->spsCommIntervalProfile;
    LP_SpsSysWideParam sysParam_p = PNULL;
    /* Parsing and storing SPS Parameters */
    sysParam_p = &macInitReqParams_p->spsSysWideParams;
    if( LTE_TRUE <  sysParam_p->enableSps)
    {
        /* Assigning defulat as SPS DISABLE */
        spsSysWideParam_p->enableSps = LTE_FALSE;
    }
    else
    {
        spsSysWideParam_p->enableSps = sysParam_p->enableSps;
    }

    /* Validate and Update DL interval List */
    macValidateAndUpdateIntervalList(
            spsSysWideParam_p->supportedSpsDlIntervalList,
            sysParam_p->supportedSpsDlIntervalList);

    /* Validate and Update UL interval List */
    macValidateAndUpdateIntervalList(
            spsSysWideParam_p->supportedSpsUlIntervalList,
            sysParam_p->supportedSpsUlIntervalList);

    /* Validate and Update maxSpsDciAttempt */
    if( (MAC_MIN_SPS_DCI_ATTEMPT > sysParam_p->maxSpsDciAttempt) || 
            ( MAC_MAX_SPS_DCI_ATTEMPT < sysParam_p->maxSpsDciAttempt ) ) 
    {
        spsSysWideParam_p->maxSpsDciAttempt = MAC_DEFAULT_SPS_DCI_ATTEMPT;
    }
    else
    {
        spsSysWideParam_p->maxSpsDciAttempt = sysParam_p->maxSpsDciAttempt; 
    }

    /* Validate and Update qciSpslist */
    macValidateAndUpdateQciSpsList(
            spsSysWideParam_p->qciSpslist,
            sysParam_p->qciSpslist, internalCellIndex );

    /* Validate and Update Common Interval Profile */
    macValidateAndUpdateComIntervalProfile (macInitReqParams_p, sysParam_p,
                                            spsCommIntervalProfile_p);

    /* Validate and Update DL Interval Profile */
    macValidateAndUpdateDlIntervalProfile (macInitReqParams_p, sysParam_p,
                                          &initParams_p->dlSchdConfig);

    /* Validate and Update UL Interval Profile */
    macValidateAndUpdateUlIntervalProfile (macInitReqParams_p, sysParam_p,
                                            &initParams_p->ulSchdConfig);
}

/****************************************************************************
 * Function Name  : macValidateAndUpdateQciSpsList
 * Inputs         : macQciSpsList - MAC QCI SPS list and
 *                  qciSpsList - QCI SPS list provided by OAM
 *                  internalCellIndex - cell Index for which OAM request is 
 *                                      received
 * Outputs        : MAC QCI SPS Lists.
 * Returns        : None.
 * Description    : Validates and updates Qci SPS List as provided by OAM.
 ****************************************************************************/
static  void macValidateAndUpdateQciSpsList(
        UInt8 macQciSpsList[],
        UInt8 qciSpsList[],
        InternalCellIndex internalCellIndex )
{
    UInt8 spsQciIndex  = 0;

    /* Validate and Update qciSpslist */
    for (spsQciIndex = 0; spsQciIndex < MAX_QCI; spsQciIndex++)
    {
        if (LTE_TRUE < qciSpsList[spsQciIndex])
        {
            /* By default SPS shall be enabled for qci 1 and 
             * disabled for other qci */
            if (spsQciIndex == 0)
            {
                macQciSpsList[spsQciIndex] = LTE_TRUE;
            }
            else
            {
                macQciSpsList[spsQciIndex] = LTE_FALSE;
            }
        }
        else
        {
            /* SPR 12055 Fix Start */
            /* SPS will be enabled only for  QCI 1 */
            /* Check the QCI */
            if (spsQciIndex >= SPS_QCI)
            {
                /* Disable the SPS */
                macQciSpsList[spsQciIndex] = LTE_FALSE;
                /* Log Warning message */
                LOG_MAC_MSG( OAM_MAC_INVALID_SPS_QCI_MAP, LOGERROR, MAC_OAM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        __LINE__, (spsQciIndex + 1), 0, 0, 0, 
                        0, 0, __func__, "" ); 
                continue;
            }
            /* SPR 12055 Fix End */

            macQciSpsList[spsQciIndex] = qciSpsList[spsQciIndex];
        }
    }
}
/* CA Changes start */
/******************************************************************************
 * Function Name  : macValidateAndUpdateComIntervalProfile
 * Inputs         : macInitReqParams_p - Pointer to initialization parameters 
 *                : sysParam_p - SPS system parameters provided by OAM
 *                  spsCommIntervalProfile_p -
 * Outputs        : None
 * Returns        : None
 * Description    : Validates and updates SPS common interval profile as 
 *                  provided by OAM.
 ******************************************************************************/
static  void macValidateAndUpdateComIntervalProfile(
        MacInitReqParams   *macInitReqParams_p,
        LP_SpsSysWideParam sysParam_p,
        SpsCommIntervalProfile *spsCommIntervalProfile_p)
{
    UInt8 commSpsIndex = 0;
    /* Validate and Update Common Interval Profile */ 

    for(commSpsIndex = 0; commSpsIndex <SPS_INTERVAL_LIST; commSpsIndex++)
    {
        if ((LTE_FALSE != sysParam_p->supportedSpsUlIntervalList[commSpsIndex]) || 
                (LTE_FALSE != sysParam_p->supportedSpsDlIntervalList[commSpsIndex]))
        {
            /* Validate and Update overrideSpsOccassionForOtherUe */
            if (LTE_TRUE < macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                    overrideSpsOccassionForOtherUe)
            {
                spsCommIntervalProfile_p[commSpsIndex].overrideSpsOccassionForOtherUe = 
                    LTE_FALSE;
            }
            else
            {
                spsCommIntervalProfile_p[commSpsIndex].overrideSpsOccassionForOtherUe  = 
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].
                    overrideSpsOccassionForOtherUe ;
            }

            /* Validate and Update strictSpsResourceUsageForSpsLc */
            if (LTE_TRUE < macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                    strictSpsResourceUsageForSpsLc)
            {
                spsCommIntervalProfile_p[commSpsIndex].strictSpsResourceUsageForSpsLc = 
                    LTE_TRUE;
            }
            else
            {
                spsCommIntervalProfile_p[commSpsIndex].strictSpsResourceUsageForSpsLc = 
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].
                    strictSpsResourceUsageForSpsLc;
            }

            /*  Validate and Update enableSilenceDetection */
            if (LTE_TRUE < macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                    enableSilenceDetection)
            {
                spsCommIntervalProfile_p[commSpsIndex].enableSilenceDetection = 
                    LTE_FALSE;
            }
            else
            {
                spsCommIntervalProfile_p[commSpsIndex].enableSilenceDetection = 
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                    enableSilenceDetection;
            }

            /*  Validate and Update interPktDelayForSilenceDetection */
            if (MAC_MIN_INTERPKT_DELAY_FOR_SILENCE_DETECTION > 
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                    interPktDelayForSilenceDetection || 
                    MAC_MAX_INTERPKT_DELAY_FOR_SILENCE_DETECTION < \
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                    interPktDelayForSilenceDetection)
            {
                spsCommIntervalProfile_p[commSpsIndex].interPktDelayForSilenceDetection = 
                    MAC_DEFAULT_INTERPKT_DELAY_FOR_SILENCE_DETECTION ;
            }
            else
            {
                spsCommIntervalProfile_p[commSpsIndex].interPktDelayForSilenceDetection =
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                    interPktDelayForSilenceDetection;
            }

            /*  Validate and Update maxPdcpPduSizeIpv4[ROHC_PROFILE0] */
            spsCommIntervalProfile_p[commSpsIndex].maxPdcpPduSizeIpv4[ROHC_PROFILE0] =
                macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv4[ROHC_PROFILE0];

            /*  Validate and Update maxPdcpPduSizeIpv4[ROHC_PROFILE1] */
            spsCommIntervalProfile_p[commSpsIndex].maxPdcpPduSizeIpv4[ROHC_PROFILE1] =
                macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv4[ROHC_PROFILE1];

            /*  Validate and Update maxPdcpPduSizeIpv4[ROHC_PROFILE2] */
            spsCommIntervalProfile_p[commSpsIndex].maxPdcpPduSizeIpv4[ROHC_PROFILE2] =
                macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv4[ROHC_PROFILE2];

            /*  Validate and Update maxPdcpPduSizeIpv4[NO_ROHC_PROFILE] */
            spsCommIntervalProfile_p[commSpsIndex].maxPdcpPduSizeIpv4[MAC_ROHC_PROFILE - 1] =
                macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv4[NO_ROHC_PROFILE];

            /*  Validate and Update maxPdcpPduSizeIpv6[ROHC_PROFILE0] */
            spsCommIntervalProfile_p[commSpsIndex].maxPdcpPduSizeIpv6[ROHC_PROFILE0] =
                macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv6[ROHC_PROFILE0];

            /*  Validate and Update maxPdcpPduSizeIpv6[ROHC_PROFILE1] */
            spsCommIntervalProfile_p[commSpsIndex].maxPdcpPduSizeIpv6[ROHC_PROFILE1] =
                macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv6[ROHC_PROFILE1];

            /*  Validate and Update maxPdcpPduSizeIpv6[ROHC_PROFILE2] */
            spsCommIntervalProfile_p[commSpsIndex].maxPdcpPduSizeIpv6[ROHC_PROFILE2] =
                macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv6[ROHC_PROFILE2];

            /*  Validate and Update maxPdcpPduSizeIpv6[NO_ROHC_PROFILE] */
            spsCommIntervalProfile_p[commSpsIndex].maxPdcpPduSizeIpv6[MAC_ROHC_PROFILE - 1] =
                macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv6[NO_ROHC_PROFILE];

            /* Validate and Update  minRbSavedReactivationThres */
            /* Coverity 54580 */
            if ( MAC_MAX_RB_SAVED_REACTIVATION_THRES < 
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].minRbSavedReactivationThres)
            /* Coverity 54580 */
            {
                spsCommIntervalProfile_p[commSpsIndex].minRbSavedReactivationThres = 
                    MAC_DEFAULT_RB_SAVED_REACTIVATION_THRES;
            }
            else
            {
                spsCommIntervalProfile_p[commSpsIndex].minRbSavedReactivationThres = 
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].minRbSavedReactivationThres;
            }

            /* Validate and Update spsBlerThres */
            if (macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].spsBlerThres < 
                    MAC_MIN_SPS_BLER_THRES || 
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].spsBlerThres > 
                    MAC_MAX_SPS_BLER_THRES)
            {
                spsCommIntervalProfile_p[commSpsIndex].spsBlerThres = 
                    MAC_DEFAULT_SPS_BLER_THRES;
            }
            else
            {
                spsCommIntervalProfile_p[commSpsIndex].spsBlerThres =  
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].spsBlerThres;
            }

            /* Validate and Update auditInterval */
            if (macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].auditInterval > 
                    MAX_AUDIT_INTERVAL)
            {
                spsCommIntervalProfile_p[commSpsIndex].auditInterval = 
                    MAC_DEFAULT_AUDIT_INTERVAL;
            }
            else
            {
                spsCommIntervalProfile_p[commSpsIndex].auditInterval =  
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].auditInterval;
            }

            /* Validate and Update spsResAllocTtiAdv */
            if (macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].spsResAllocTtiAdv < 
                    SPS_MIN_RES_ALLOC_TTI_ADV || 
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].spsResAllocTtiAdv > 
                    SPS_MAX_RES_ALLOC_TTI_ADV)
            {
                spsCommIntervalProfile_p[commSpsIndex].spsResAllocTtiAdv = 
                    SPS_DEFAULT_RES_ALLOC_TTI_ADV;
            }
            else
            { 
                spsCommIntervalProfile_p[commSpsIndex].spsResAllocTtiAdv =  
                    macInitReqParams_p->spsCommIntervalProfile[commSpsIndex].spsResAllocTtiAdv;
            }
        }
    }
}
/* CA Changes end */

/******************************************************************************
 * Function Name  : macValidateAndUpdateDlIntervalProfile
 * Inputs         : macInitReqParams_p - Pointer to initialization parameters 
 *                : sysParam_p - SPS system parameters provided by OAM,
 *                  dlSchdConfig_p
 * Outputs        : None
 * Returns        : None
 * Description    : Validates and updates SPS DL interval profile as 
 *                  provided by OAM.
 ******************************************************************************/
static  void macValidateAndUpdateDlIntervalProfile(
        MacInitReqParams   *macInitReqParams_p,
        LP_SpsSysWideParam sysParam_p,
        DLSchedulerStrategyConfig *dlSchdConfig_p)
{
    UInt8 dlSpsIndex   = 0;

    /* Validate and Update DL SPS INTERVAL PROFILE Parameters */
    for(dlSpsIndex = 0; dlSpsIndex < SPS_INTERVAL_LIST; dlSpsIndex++)
    {
        if(LTE_FALSE != sysParam_p->supportedSpsDlIntervalList[dlSpsIndex])
        {
            /* Validate and Update dlMinMcsSps */
            if( macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].dlMinMcsSps > 
                    MAC_MAX_DL_MIN_MCS_SPS)
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlMinMcsSps = 
                    MAC_DEFAULT_DL_MIN_MCS_SPS;
            }
            else
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlMinMcsSps = 
                    macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].dlMinMcsSps;
            }

            /* Validate and Update dlMcsMargin */
            if( macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].dlMcsMargin > 
                    MAC_MAX_DL_MIN_MCS_MARGIN )
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlMcsMargin = 
                    MAC_DEFAULT_DL_MIN_MCS_MARGIN;
            }
            else
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlMcsMargin = 
                    macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].dlMcsMargin;
            }

            /* Validate and Update dlTbsizeMarginSps */
            if( macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].dlTbsizeMarginSps > 
                    MAC_MAX_DL_TBSIZE_MARGIN_SPS)
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlTbsizeMarginSps = 
                    MAC_DEFAULT_DL_TBSIZE_MARGIN_SPS;
            }
            else
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlTbsizeMarginSps = 
                    macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].dlTbsizeMarginSps;
            }

            /* Validate and Update dlJitterHandlingOption */
            if (macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].dlJitterHandlingOption >= 
                    MAX_JITTER_HANDLING_OPT)
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlJitterHandlingOption = 
                    NO_HANDLING;
            }
            else
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlJitterHandlingOption =
                    macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].dlJitterHandlingOption;
            }

            /* Validate and Update rlcSduThresForDra */
            if (macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].rlcSduThresForDra < 
                    MAC_MIN_RLC_SDU_THRES_FOR_DRA || 
                    macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].rlcSduThresForDra > 
                    MAC_MAX_RLC_SDU_THRES_FOR_DRA)
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].rlcSduThresForDra = 
                    MAC_DEFAULT_RLC_SDU_THRES_FOR_DRA;
            }
            else
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].rlcSduThresForDra = 
                    macInitReqParams_p->spsDLIntervalProfile[dlSpsIndex].rlcSduThresForDra;
            }
        }
    }
}

/******************************************************************************
 * Function Name  : macValidateAndUpdateUlIntervalProfile
 * Inputs         : macInitReqParams_p - Pointer to initialization parameters 
 *                : sysParam_p - SPS system parameters provided by OAM,
 *                  ulSchdConfig_p
 * Outputs        : None
 * Returns        : None
 * Description    : Validates and updates SPS UL interval profile as 
 *                  provided by OAM.
 ******************************************************************************/
static  void macValidateAndUpdateUlIntervalProfile(
        MacInitReqParams   *macInitReqParams_p,
        LP_SpsSysWideParam sysParam_p,
        ULSchedulerStrategyConfig *ulSchdConfig_p)
{
    UInt8 ulSpsIndex   = 0;

    /* Validate and Update UL Interval Profile Parameters */
    for (ulSpsIndex = 0; ulSpsIndex < SPS_INTERVAL_LIST; ulSpsIndex++)
    {
        if(LTE_FALSE != sysParam_p->supportedSpsUlIntervalList[ulSpsIndex])
        {
            /*  Validate and Update ulMinMcsSps */
            if( macInitReqParams_p->spsULIntervalProfile[ulSpsIndex].ulMinMcsSps > 
                    MAC_MAX_UL_MIN_MCS_SPS)
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulMinMcsSps = 
                    MAC_DEFAULT_UL_MIN_MCS_SPS;
            }
            else
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulMinMcsSps = 
                    macInitReqParams_p->spsULIntervalProfile[ulSpsIndex].ulMinMcsSps;
            }

            /*  Validate and Update ulMcsMargin */
            if( macInitReqParams_p->spsULIntervalProfile[ulSpsIndex].ulMcsMargin > 
                    MAC_MAX_UL_MIN_MCS_MARGIN)
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulMcsMargin = 
                    MAC_DEFAULT_UL_MIN_MCS_MARGIN;
            }
            else
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulMcsMargin = 
                    macInitReqParams_p->spsULIntervalProfile[ulSpsIndex].ulMcsMargin;
            }

            /*  Validate and Update ulTbSizeMarginSps */
            if( macInitReqParams_p->spsULIntervalProfile[ulSpsIndex].ulTbSizeMarginSps >
                    MAC_MAX_UL_TBSIZE_MARGIN_SPS)
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulTbSizeMarginSps = 
                    MAC_DEFAULT_UL_TBSIZE_MARGIN_SPS;
            }
            else
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulTbSizeMarginSps = 
                    macInitReqParams_p->spsULIntervalProfile[ulSpsIndex].ulTbSizeMarginSps;
            }
        }
    }
}

/* CA Changes start */
/****************************************************************************
 * Function Name  : macUpdateQosRelatedParams
 * Inputs         : macInitReqParams_p - Pointer to initialization parameters,
 *                  initParams_p
 * Outputs        : None
 * Returns        : None 
 * Variables      : None
 * Description    : This function updates Qos related parameters.
 ****************************************************************************/
static  void macUpdateQosRelatedParams(
        MacInitReqParams *macInitReqParams_p,
        InitParams *initParams_p)
{
     DLSchedulerStrategyConfig *dlSchdConfig_p = &(initParams_p->dlSchdConfig);
     ULSchedulerStrategyConfig *ulSchdConfig_p = &(initParams_p->ulSchdConfig);
    /*QOS_KLOCKWORK_FIX*/
#ifndef QOS_SI_FWK_TEST
    UInt8  qciIndex = 0;
    /*QOS_KLOCKWORK_FIX*/

    /*Parse Qos related params*/
	VALIDATE_QOS_STRATEGY_WEIGHTS_IN_INIT_LAYER_REQ(
			macInitReqParams_p->ulQosStrategyWeights,
			ulSchdConfig_p->ulQosStrategyWeights);

	VALIDATE_QOS_STRATEGY_WEIGHTS_IN_INIT_LAYER_REQ(
			macInitReqParams_p->dlQosStrategyWeights,
			dlSchdConfig_p->dlQosStrategyWeights);
    /*+- SPR 20896 Fix*/
	if (macInitReqParams_p->qosStrategyOverallocFactor <= 
		                  	 MAX_QOS_STRATEGY_OVER_ALLOC_FACTOR)
    /*+- SPR 20896 Fix*/
	{
		dlSchdConfig_p->strategyBufferRB  = macInitReqParams_p->qosStrategyOverallocFactor;
		ulSchdConfig_p->strategyBufferRB  = macInitReqParams_p->qosStrategyOverallocFactor;
	}
	else 
	{
		lteWarning("Invalid value  %d of qosStrategyOverallocFactor received setting "
				"default value \n",
				macInitReqParams_p->qosStrategyOverallocFactor);
		dlSchdConfig_p->strategyBufferRB  = DEFAULT_QOS_STRATEGY_OVER_ALLOC_FACTOR;
		ulSchdConfig_p->strategyBufferRB  = DEFAULT_QOS_STRATEGY_OVER_ALLOC_FACTOR;
	}

	if ((macInitReqParams_p->ambrBktDepthFactor >= MIN_AMBR_RKT_DEPTH_FACTOR)
			&& (macInitReqParams_p->ambrBktDepthFactor <= MAX_AMBR_RKT_DEPTH_FACTOR)) 
	{ 
		dlSchdConfig_p->ambrBktFactor  = macInitReqParams_p->ambrBktDepthFactor;
		ulSchdConfig_p->ambrBktFactor  = macInitReqParams_p->ambrBktDepthFactor;
	}
	else
	{
		lteWarning("Invalid value %d of ambrBktDepthFactor received setting"
				" default value\n",
				macInitReqParams_p->ambrBktDepthFactor);
		dlSchdConfig_p->ambrBktFactor  = DEFAULT_AMBR_RKT_DEPTH_FACTOR;
		ulSchdConfig_p->ambrBktFactor  = DEFAULT_AMBR_RKT_DEPTH_FACTOR;
	}

	if ( (macInitReqParams_p->gbrBktDepthFactor >= MIN_GBR_RKT_DEPTH_FACTOR )
			&& (macInitReqParams_p->gbrBktDepthFactor <= MAX_GBR_RKT_DEPTH_FACTOR))
	{ 
		dlSchdConfig_p->gbrBktFactor  = macInitReqParams_p->gbrBktDepthFactor;
		ulSchdConfig_p->gbrBktFactor  = macInitReqParams_p->gbrBktDepthFactor;
	}
	else
	{
		lteWarning("Invalid value %d of gbrBktDepthFactor received setting"
    				"default value\n",
				macInitReqParams_p->gbrBktDepthFactor);
		dlSchdConfig_p->gbrBktFactor  = DEFAULT_GBR_RKT_DEPTH_FACTOR;
		ulSchdConfig_p->gbrBktFactor  = DEFAULT_GBR_RKT_DEPTH_FACTOR;
	}

	if ((macInitReqParams_p->tokenPeriodicity >= MIN_TOKEN_PERIODICITY) &&
			(macInitReqParams_p->tokenPeriodicity <= MAX_TOKEN_PERIODICITY))
	{
		dlSchdConfig_p->dlTokenPeriodicity = macInitReqParams_p->tokenPeriodicity;
		ulSchdConfig_p->ulTokenPeriodicity = macInitReqParams_p->tokenPeriodicity;
	}
	else
	{
		lteWarning("Invalid value %d of tokenPeriodicity received setting"
				" default value\n",
				macInitReqParams_p->tokenPeriodicity);
		dlSchdConfig_p->dlTokenPeriodicity = DEFAULT_TOKEN_PERIODICITY;
		ulSchdConfig_p->ulTokenPeriodicity = DEFAULT_TOKEN_PERIODICITY;
        
	}
    /* SPR 12975 Changes Start */ 
    if (macInitReqParams_p->dlGbrTokenLimitFactor < (SInt8) macInitReqParams_p->gbrBktDepthFactor) 
    {
        dlSchdConfig_p->gbrTokenLimitFactor = macInitReqParams_p->dlGbrTokenLimitFactor;
    }
    else
    {
        lteWarning("Invalid value %d of dlGbrTokenLimitFactor received, setting"
                " default value: %d\n",
                macInitReqParams_p->dlGbrTokenLimitFactor,
                DEFAULT_DL_GBR_TOKEN_LIMIT_FACTOR);
        dlSchdConfig_p->gbrTokenLimitFactor = DEFAULT_DL_GBR_TOKEN_LIMIT_FACTOR;
    }
    if (macInitReqParams_p->ulGbrTokenLimitFactor < (SInt8) macInitReqParams_p->gbrBktDepthFactor) 
    {
        ulSchdConfig_p->gbrTokenLimitFactor = macInitReqParams_p->ulGbrTokenLimitFactor;
    }
    else
    {
        lteWarning("Invalid value %d of ulGbrTokenLimitFactor received, increasing"
                " gbrBktDepthFactor to its default value: %d\n",
                macInitReqParams_p->ulGbrTokenLimitFactor,
                DEFAULT_GBR_RKT_DEPTH_FACTOR);
        /* Since ulGbrTokenLimitFactor must be less that gbrBktDepthFactor 
         * but the default value of ulGbrTokenLimitFactor is 1 so there is 
         * possibilty that gbrBktDepthFactor and ulGbrTokenLimitFactor
         * both have value 1, so in that case we have changed the 
         * gbrBktDepthFactor to its default value which is 2
         */
        macInitReqParams_p->gbrBktDepthFactor = DEFAULT_GBR_RKT_DEPTH_FACTOR;
    }
    if (macInitReqParams_p->dlAmbrTokenLimitFactor < (SInt8) macInitReqParams_p->ambrBktDepthFactor) 
    {
        dlSchdConfig_p->ambrTokenLimitFactor = macInitReqParams_p->dlAmbrTokenLimitFactor;
    }
    else
    {
        lteWarning("Invalid value %d of dlAmbrTokenLimitFactor received, setting"
                " default value: %d\n",
                macInitReqParams_p->dlAmbrTokenLimitFactor,
                DEFAULT_DL_AMBR_TOKEN_LIMIT_FACTOR);
        dlSchdConfig_p->ambrTokenLimitFactor = DEFAULT_DL_AMBR_TOKEN_LIMIT_FACTOR;
    }
    if (macInitReqParams_p->ulAmbrTokenLimitFactor < (SInt8) macInitReqParams_p->ambrBktDepthFactor) 
    {
        ulSchdConfig_p->ambrTokenLimitFactor = macInitReqParams_p->ulAmbrTokenLimitFactor;
    }
    else
    {
        lteWarning("Invalid value %d of ulAmbrTokenLimitFactor received, increasing"
                " ambrBktDepthFactor to its default value: %d\n",
                macInitReqParams_p->ulAmbrTokenLimitFactor,
                DEFAULT_AMBR_RKT_DEPTH_FACTOR);
        /* Since ulAmbrTokenLimitFactor must be less that ambrBktDepthFactor 
         * but the default value of ulAmbrTokenLimitFactor is 1 so there is 
         * possibilty that ambrBktDepthFactor and ulAmbrTokenLimitFactor
         * both have value 1, so in that case we have changed the 
         * ambrBktDepthFactor to its default value which is 2
         */
        macInitReqParams_p->ambrBktDepthFactor = DEFAULT_AMBR_RKT_DEPTH_FACTOR;
    }
    /* SPR 12975 Changes End */ 

    for (qciIndex = 0; qciIndex < MAX_QCI; ++ qciIndex)
    {
        macParams_g.qosQciInfo[qciIndex] =
            macInitReqParams_p->qosQciInfo[qciIndex];
    }

#else 
    /*Parse Qos related params*/
    ulSchdConfig_p->ulQosStrategyWeights.pdbWeight   = 5;
    ulSchdConfig_p->ulQosStrategyWeights.qciWeight   = 5;
    ulSchdConfig_p->ulQosStrategyWeights.tokenWeight = 5;
    ulSchdConfig_p->ulQosStrategyWeights.qLoadWeight = 5;
    ulSchdConfig_p->ulQosStrategyWeights.mcsWeight   = 5;
    dlSchdConfig_p->dlQosStrategyWeights.pdbWeight   = 5;
    dlSchdConfig_p->dlQosStrategyWeights.qciWeight   = 5;
    dlSchdConfig_p->dlQosStrategyWeights.tokenWeight = 5;
    dlSchdConfig_p->dlQosStrategyWeights.qLoadWeight = 5;
    dlSchdConfig_p->dlQosStrategyWeights.mcsWeight   = 5;

    ulSchdConfig_p->ulTokenPeriodicity = 20;
    ulSchdConfig_p->strategyBufferRB   = (cellConfig_g.ulAvailableRBs * 1 )/ 2;
    ulSchdConfig_p->ambrBktFactor      = 1;
    ulSchdConfig_p->gbrBktFactor       = 1;

/*    ulSchdConfig_p->qosQciInfo[1].QCI            = 2;
    ulSchdConfig_p->qosQciInfo[1].eResType       = 0;
    ulSchdConfig_p->qosQciInfo[1].priority       = 4;
    ulSchdConfig_p->qosQciInfo[1].pktDlyBdgt     = 150;
    ulSchdConfig_p->qosQciInfo[1].pktErrLossRate = 1;

    dlSchdConfig_p->qosQciInfo[1].QCI            = 2;
    dlSchdConfig_p->qosQciInfo[1].eResType       = 0;
    dlSchdConfig_p->qosQciInfo[1].priority       = 4;
    dlSchdConfig_p->qosQciInfo[1].pktDlyBdgt     = 150;
    dlSchdConfig_p->qosQciInfo[1].pktErrLossRate = 1;
    dlSchdConfig_p->qosQciInfo[8].QCI            = 9;
    dlSchdConfig_p->qosQciInfo[8].eResType       = 1;
    dlSchdConfig_p->qosQciInfo[8].priority       = 9;
    dlSchdConfig_p->qosQciInfo[8].pktDlyBdgt     = 300;
    dlSchdConfig_p->qosQciInfo[8].pktErrLossRate = 100;

    ulSchdConfig_p->qosQciInfo[8].QCI            = 9;
    ulSchdConfig_p->qosQciInfo[8].eResType       = 1;
    ulSchdConfig_p->qosQciInfo[8].priority       = 9;
    ulSchdConfig_p->qosQciInfo[8].pktDlyBdgt     = 300;
    ulSchdConfig_p->qosQciInfo[8].pktErrLossRate = 100;
*/

    dlSchdConfig_p->dlTokenPeriodicity = 20;
    dlSchdConfig_p->strategyBufferRB   = (cellConfig_g.dlAvailableRBs * 1 )/ 2;
    dlSchdConfig_p->ambrBktFactor      = 1;
    dlSchdConfig_p->gbrBktFactor       = 1;
    /* SPR 12975 Changes Start */ 
    dlSchdConfig_p->gbrTokenLimitFactor    = -1;
    dlSchdConfig_p->ambrTokenLimitFactor   = -1;
    ulSchdConfig_p->gbrTokenLimitFactor    = -1;
    ulSchdConfig_p->ambrTokenLimitFactor   = -1;
    /* SPR 12975 Changes Start */ 
#endif
    /* DESIGN REVIEW CHANGES INCORPORATION */
    /* SPR 11115 Fix Start */
    /* Code deleted SPR 12975 */ 
    /* SPR 11115 Fix End */
}
/* Cyclomatic Complexity changes - ends here */
/* CA Changes start */

/****************************************************************************
 * Function Name  : macInterfaceUpdateQOSParamsInInitCellReq
 * Inputs         : macInitDLQOSParams_p - Pointer to received DL QOS parameters
 *                  dlSchdConfig_p - Pointer to dlSchdConfig
 *                  ulSchdConfig_p - Pointer to ulSchdConfig
 * Outputs        : None
 * Returns        : None 
 * Variables      : None
 * Description    : This function updates Qos related parameters.
 ****************************************************************************/
static  void  macInterfaceUpdateQOSParamsInInitCellReq(
                                       MacInitDLQOSParams *macInitDLQOSParams_p,
                                       DLSchedulerStrategyConfig  *dlSchdConfig_p,
                                       ULSchedulerStrategyConfig  *ulSchdConfig_p)
{
	/* Parse Qos related params */

    /*+- SPR 20896 Fix*/
	if (macInitDLQOSParams_p->qosStrategyOverallocFactor <= 
		                  	 MAX_QOS_STRATEGY_OVER_ALLOC_FACTOR)
    /*+- SPR 20896 Fix*/
	{
		dlSchdConfig_p->strategyBufferRB  = macInitDLQOSParams_p->qosStrategyOverallocFactor;
		ulSchdConfig_p->strategyBufferRB  = macInitDLQOSParams_p->qosStrategyOverallocFactor;
	}
	else 
	{
		lteWarning("Invalid value  %d of qosStrategyOverallocFactor received setting "
				"value as received  in MAC_INIT_LAYER_REQ\n",
				macInitDLQOSParams_p->qosStrategyOverallocFactor);
	}

	if ((macInitDLQOSParams_p->ambrBktDepthFactor >= MIN_AMBR_RKT_DEPTH_FACTOR)
			&& (macInitDLQOSParams_p->ambrBktDepthFactor <= MAX_AMBR_RKT_DEPTH_FACTOR)) 
	{ 
		dlSchdConfig_p->ambrBktFactor  = macInitDLQOSParams_p->ambrBktDepthFactor;
		ulSchdConfig_p->ambrBktFactor  = macInitDLQOSParams_p->ambrBktDepthFactor;
	}
	else
	{
		lteWarning("Invalid value %d of ambrBktDepthFactor received setting"
				" value as received in MAC_INIT_LAYER_REQ\n",
				macInitDLQOSParams_p->ambrBktDepthFactor);
	}

	if ( (macInitDLQOSParams_p->gbrBktDepthFactor >= MIN_GBR_RKT_DEPTH_FACTOR )
			&& (macInitDLQOSParams_p->gbrBktDepthFactor <= MAX_GBR_RKT_DEPTH_FACTOR))
	{ 
		dlSchdConfig_p->gbrBktFactor  = macInitDLQOSParams_p->gbrBktDepthFactor;
		ulSchdConfig_p->gbrBktFactor  = macInitDLQOSParams_p->gbrBktDepthFactor;
	}
	else
	{
		lteWarning("Invalid value %d of gbrBktDepthFactor received setting"
				" value as received in MAC_INIT_LAYER_REQ\n",
				macInitDLQOSParams_p->gbrBktDepthFactor);
	}

	if ((macInitDLQOSParams_p->tokenPeriodicity >= MIN_TOKEN_PERIODICITY) &&
			(macInitDLQOSParams_p->tokenPeriodicity <= MAX_TOKEN_PERIODICITY))
	{
		dlSchdConfig_p->dlTokenPeriodicity = macInitDLQOSParams_p->tokenPeriodicity;
		ulSchdConfig_p->ulTokenPeriodicity = macInitDLQOSParams_p->tokenPeriodicity;
	}
	else
	{
		lteWarning("Invalid value %d of tokenPeriodicity received setting"
				" value as received in MAC_INIT_LAYER_REQ\n",
				macInitDLQOSParams_p->tokenPeriodicity);
	}

    VALIDATE_QOS_STRATEGY_WEIGHTS_IN_INIT_CELL_REQ(
                macInitDLQOSParams_p->dlQosStrategyWeights,
                                  dlSchdConfig_p->dlQosStrategyWeights);
}
/****************************************************************************
 * Function Name  : macInterfaceSetConfigParamsInInitCellReq
 * Inputs         : macInitCellReqParams_p - Pointer to received parameters
 *                : initReqParams_p - Pointer to initialization parameters
 *                : internalCellIndex - cell id  
 * Outputs        : 
 * Returns        : MAC_SUCCESS or reason for failure
 * Variables      :
 * Description    : This API initialize the config parameters received from 
 *                  OAM at init time.
 ****************************************************************************/

UInt16 macInterfaceSetConfigParamsInInitCellReq(
                                   MacInitCellReqParams *macInitCellReqParams_p,
                                   InitParams *initParams_p,
                                   InternalCellIndex internalCellIndex 
)
{
    UInt32 fieldBitMask = macInitCellReqParams_p->fieldBitMask;
    ULSchedulerStrategyConfig *ulSchdConfig_p = &initParams_p->ulSchdConfig;
    DLSchedulerStrategyConfig *dlSchdConfig_p = &initParams_p->dlSchdConfig;
    MacInitDLPFSParams *macInitDLPFSParams_p = &macInitCellReqParams_p->macInitDLPFSParams;
    MacInitULPFSParams *macInitULPFSParams_p = &macInitCellReqParams_p->macInitULPFSParams;
    MacInitDLQOSParams *macInitDLQOSParams_p =  &macInitCellReqParams_p->macInitDLQOSParams;
    MacInitULQOSParams *macInitULQOSParams_p =  &macInitCellReqParams_p->macInitULQOSParams;
    MacInitICICParams *macInitICICParams_p = &macInitCellReqParams_p->macInitICICParams;
    MacInitSPSParams *macInitSPSParams_p = &macInitCellReqParams_p->macInitSPSParams;


    if ((fieldBitMask & DL_PFS_PARAMS) && (!macParams_g.isCASupported)) 
    {
        macInterfaceUpdateDlschParamsInInitCellReq(macInitDLPFSParams_p, 
                dlSchdConfig_p);
    }
    else if ((!(fieldBitMask & DL_PFS_PARAMS) && (!macParams_g.isCASupported)) ||
            ((macParams_g.isCASupported) && (fieldBitMask & DL_PFS_PARAMS)))
    {
        // LOG_MAC_MSG
    }
    if (fieldBitMask & UL_PFS_PARAMS)
    {
        macInterfaceUpdateUlschParamsInInitCellReq(macInitULPFSParams_p,
                ulSchdConfig_p);
    }

    if (fieldBitMask & ICIC_PARAMS)
    {
        macInterfaceUpdateICICParamsInInitCellReq(macInitICICParams_p,
                dlSchdConfig_p,
                ulSchdConfig_p,
              internalCellIndex);
    }
    macInterfacePrintDLULParamsInInitCellReq(dlSchdConfig_p,ulSchdConfig_p);
    if (fieldBitMask & SPS_PARAMS)
    {
        macInterfaceUpdateSPSParamsInInitCellReq(macInitSPSParams_p,initParams_p);
    }


#ifndef QOS_SI_FWK_TEST
    if ((fieldBitMask & DL_QOS_PARAMS) && (!macParams_g.isCASupported))
    {
       /* Parse Qos related params */
        macInterfaceUpdateQOSParamsInInitCellReq(macInitDLQOSParams_p,
                                         dlSchdConfig_p,ulSchdConfig_p);
     
        /* SPR 22539 Fix + */
        if (macInitDLQOSParams_p->enableAllocAfterAllowedBitrate > ENABLE_ALLOW_ALLOC_BITRATE_TOKEN_BASED)
		{
            /* SPR 22539 Fix - */

			lteWarning("Invalid value of enableAllocAfterAllowedBitrate = %d"
					" received.. Setting value as received in MAC_INIT_LAYER_REQ.",
					macInitDLQOSParams_p->enableAllocAfterAllowedBitrate);
		}
        else
        { 
            initParams_p->enableAllocAfterAllowedBitrate = macInitDLQOSParams_p->\
                                                 enableAllocAfterAllowedBitrate;
        }
    }
    if (fieldBitMask & UL_QOS_PARAMS)
    {
        VALIDATE_QOS_STRATEGY_WEIGHTS_IN_INIT_CELL_REQ(
             macInitULQOSParams_p->ulQosStrategyWeights,
                                  ulSchdConfig_p->ulQosStrategyWeights);
    }
#else 
    /* Parse Qos related paramsi */
    ulSchdConfig_p->ulQosStrategyWeights.pdbWeight   = 5;
    ulSchdConfig_p->ulQosStrategyWeights.qciWeight   = 5;
    ulSchdConfig_p->ulQosStrategyWeights.tokenWeight = 5;
    ulSchdConfig_p->ulQosStrategyWeights.qLoadWeight = 5;
    ulSchdConfig_p->ulQosStrategyWeights.mcsWeight   = 5;
    dlSchdConfig_p->dlQosStrategyWeights.pdbWeight   = 5;
    dlSchdConfig_p->dlQosStrategyWeights.qciWeight   = 5;
    dlSchdConfig_p->dlQosStrategyWeights.tokenWeight = 5;
    dlSchdConfig_p->dlQosStrategyWeights.qLoadWeight = 5;
    dlSchdConfig_p->dlQosStrategyWeights.mcsWeight   = 5;

    ulSchdConfig_p->ulTokenPeriodicity = 20;
    ulSchdConfig_p->strategyBufferRB   = (cellConfig_g.ulAvailableRBs * 1 )/ 2;
    ulSchdConfig_p->ambrBktFactor      = 1;
/*
    ulSchdConfig_p->qosQciInfo[1].QCI            = 2;
    ulSchdConfig_p->qosQciInfo[1].eResType       = 0;
    ulSchdConfig_p->qosQciInfo[1].priority       = 4;
    ulSchdConfig_p->qosQciInfo[1].pktDlyBdgt     = 150;
    ulSchdConfig_p->qosQciInfo[1].pktErrLossRate = 1;

    dlSchdConfig_p->qosQciInfo[1].QCI            = 2;
    dlSchdConfig_p->qosQciInfo[1].eResType       = 0;
    dlSchdConfig_p->qosQciInfo[1].priority       = 4;
    dlSchdConfig_p->qosQciInfo[1].pktDlyBdgt     = 150;
    dlSchdConfig_p->qosQciInfo[1].pktErrLossRate = 1;

    dlSchdConfig_p->qosQciInfo[8].QCI            = 9;
    dlSchdConfig_p->qosQciInfo[8].eResType       = 1;
    dlSchdConfig_p->qosQciInfo[8].priority       = 9;
    dlSchdConfig_p->qosQciInfo[8].pktDlyBdgt     = 300;
    dlSchdConfig_p->qosQciInfo[8].pktErrLossRate = 100;

    ulSchdConfig_p->qosQciInfo[8].QCI            = 9;
    ulSchdConfig_p->qosQciInfo[8].eResType       = 1;
    ulSchdConfig_p->qosQciInfo[8].priority       = 9;
    ulSchdConfig_p->qosQciInfo[8].pktDlyBdgt     = 300;
    ulSchdConfig_p->qosQciInfo[8].pktErrLossRate = 100;
*/

    dlSchdConfig_p->dlTokenPeriodicity = 20;
    dlSchdConfig_p->strategyBufferRB   = (cellConfig_g.dlAvailableRBs * 1 )/ 2;
    dlSchdConfig_p->ambrBktFactor      = 1;
    dlSchdConfig_p->gbrBktFactor       = 1;
#endif

    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : macInterfaceUpdateDlschParamsInInitCellReq 
 * Inputs         : macInitDLPFSParams_p - Pointer to DL PFS paramters
 *                  dlSchdConfig_p - Pointer to init structure DL PFS params
 * Outputs        : 
 * Returns        : None
 * Variables      :
 * Description    : This function will update DL PFS parameters
 ****************************************************************************/
static  void macInterfaceUpdateDlschParamsInInitCellReq(
                              MacInitDLPFSParams *macInitDLPFSParams_p,
                               DLSchedulerStrategyConfig *dlSchdConfig_p)
{

    if(macInitDLPFSParams_p->dlUeQueueLoadWt > MAC_SCH_MAX_DWLK_UE_QL_WT)
    {
        lteWarning ("Invalid value for Downlink UE QLoad weight.\
                Value will set from MAC_INIT_LAYER_REQ");
    } else {
        dlSchdConfig_p->dwlkUEQLoadWt = macInitDLPFSParams_p->dlUeQueueLoadWt;
    }    

    if(macInitDLPFSParams_p->dlMsWt > MAC_SCH_MAX_DWLK_MS_WT)
    {
        lteWarning ("Invalid value for Downlink ModeScheme weight.\
                Value will set from MAC_INIT_LAYER_REQ");
    } else {
        dlSchdConfig_p->dwlkMSWt = macInitDLPFSParams_p->dlMsWt;
    }    

    if(macInitDLPFSParams_p->dlLcQueueLoadWt > MAC_SCH_MAX_DWLK_LC_QL_WT)
    {
        lteWarning ("Invalid value for Downlink LC QLoad weight.\
                Value will set from MAC_INIT_LAYER_REQ");
    } else {
        dlSchdConfig_p->dwlkLCQLoadWt = macInitDLPFSParams_p->dlLcQueueLoadWt;
    }    

    if(macInitDLPFSParams_p->dlUePriorityWt > MAC_SCH_MAX_DWLK_UE_PR_WT)
    {
        lteWarning ("Invalid value for Downlink UE Priority weight\
                Value will set from MAC_INIT_LAYER_REQ");
    } else {
        dlSchdConfig_p->dwlkUEPriorityWt = macInitDLPFSParams_p->dlUePriorityWt;
    }    

    if(macInitDLPFSParams_p->dlLcPriorityWt > MAC_SCH_MAX_DWLK_LC_PR_WT)
    {
        lteWarning ("Invalid value for Downlink LC Priority weight\
                Value will set from MAC_INIT_LAYER_REQ");
    } else {
        dlSchdConfig_p->dwlkLCPriorityWt = macInitDLPFSParams_p->dlLcPriorityWt;
    }    

    if(macInitDLPFSParams_p->dlUeDelayWt > MAC_SCH_MAX_DWLK_UE_DELAY_WT)
    {
        lteWarning ("Invalid value for Downlink UE Delay weight\
                Value will set from MAC_INIT_LAYER_REQ");
    } else {
        dlSchdConfig_p->dwlkUEDelayWt = macInitDLPFSParams_p->dlUeDelayWt;
    }    

    if(macInitDLPFSParams_p->dlLcDelayWt > MAC_SCH_MAX_DWLK_LC_DELAY_WT)
    {
        lteWarning ("Invalid value for Downlink LC Delay weight\
                Value will set from MAC_INIT_LAYER_REQ");
    } else {
        dlSchdConfig_p->dwlkLCDelayWt = macInitDLPFSParams_p->dlLcDelayWt;
    }    
    /* Setting Downlink Modulation Scheme Weight for 64QAM*/
    if (macInitDLPFSParams_p->dlMs64QAMFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "Invalid value for DL 64 QAM mod scheme \
                Value will set from MAC_INIT_LAYER_REQ");
    }
    else
        dlSchdConfig_p->dlMs64QAMFactorWt =
            macInitDLPFSParams_p->dlMs64QAMFactorWt;

    /* Setting Downlink Modulation Scheme Weight for 16QAM*/
    if (macInitDLPFSParams_p->dlMs16QAMFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "Invalid value for DL 16 QAM mod scheme\
                Value will set from MAC_INIT_LAYER_REQ");
    }
    else
        dlSchdConfig_p->dlMs16QAMFactorWt =
            macInitDLPFSParams_p->dlMs16QAMFactorWt;


    /* Setting Downlink Modulation Scheme Weight for QPSK*/
    if (macInitDLPFSParams_p->dlMsQPSKFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,  
                "Invalid value for DL 16 QAM mod scheme\
                Value will set from MAC_INIT_LAYER_REQ");
    }
    else
        dlSchdConfig_p->dlMsQPSKFactorWt =
            macInitDLPFSParams_p->dlMsQPSKFactorWt;

    /* setting Downlink CQi Wt */
    dlSchdConfig_p->dwlkcqiWt = MAC_SCH_DEFAULT_DWLK_CQI_WT;

    return;
} 
/****************************************************************************
 * Function Name  : macInterfaceUpdateUlschParamsInInitCellReq 
 * Inputs         : macInitULPFSParams_p - Pointer to UL PFS paramters
 *                  ulSchdConfig_p - Pointer to init structure UL PFS params
 * Outputs        : 
 * Returns        : None
 * Variables      :
 * Description    : This function will update UL PFS parameters. 
 ****************************************************************************/
static  void macInterfaceUpdateUlschParamsInInitCellReq(
                              MacInitULPFSParams *macInitULPFSParams_p,
                              ULSchedulerStrategyConfig *ulSchdConfig_p)
{

    if (macInitULPFSParams_p->ulUeBsrLoadWt > MAC_SCH_MAX_UPLK_UE_BSR_WT)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK BSR WEIGHT.\
                Value will set from MAC_INIT_LAYER_REQ");
    }
    else
        ulSchdConfig_p->uplkUEBSRLoadWt = macInitULPFSParams_p->ulUeBsrLoadWt;

    /* Setting Uplink UE Delay Wt */
    /* Coverity 66969 Start */
    if (macInitULPFSParams_p->ulUeDelayWt > MAC_SCH_MAX_UPLK_UE_DELAY_WT)
    /* Coverity 66969 End */
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK UE DELAY WEIGHT\
                Value will set from MAC_INIT_LAYER_REQ");
    }
    else
        ulSchdConfig_p->uplkUEDelayWt = macInitULPFSParams_p->ulUeDelayWt;

    /* Setting Uplink UE Priority Wt */
    /* Coverity 66969 Start */
    if (macInitULPFSParams_p->ulUePriorityWt > MAC_SCH_MAX_UPLK_UE_PR_WT)
    /* Coverity 66969 End */
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK UE PRIORITY WEIGHT\
                Value will set from MAC_INIT_LAYER_REQ");
    } 
    else
        ulSchdConfig_p->uplkUEPriorityWt = macInitULPFSParams_p->ulUePriorityWt;

    /* Setting Uplink MS Wt */
    /* Coverity 66969 Start */
    if (macInitULPFSParams_p->ulMsWt > MAC_SCH_MAX_UPLK_MS_WT )
    /* Coverity 66969 End */
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK UE MS WEIGHT\
                Value will set from MAC_INIT_LAYER_REQ");
    }
    else
        ulSchdConfig_p->uplkMSWt = macInitULPFSParams_p->ulMsWt;

    /* Setting Uplink LCG Priority Wt */
    /* Coverity 66969 Start */
    if (macInitULPFSParams_p->ulLcgPriorityWt > MAC_SCH_MAX_UPLK_LCG_PR_WT )
    /* Coverity 66969 End */
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, "Invalid value for UPLINK LCG PRIORITY WEIGHT\
                Value will set from MAC_INIT_LAYER_REQ");
    }
    else
        ulSchdConfig_p->uplkLCGPriorityWt = macInitULPFSParams_p->ulLcgPriorityWt;


    /* Setting uplink Modulation Scheme Weight for 64QAM*/
    if (macInitULPFSParams_p->ulMs64QAMFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "Invalid value for UL 64 QAM mod scheme\
                Value will set from MAC_INIT_LAYER_REQ"); 
    }
    else
        ulSchdConfig_p->ulMs64QAMFactorWt =
            macInitULPFSParams_p->ulMs64QAMFactorWt;

    /* Setting uplink Modulation Scheme Weight for 16QAM*/
    if (macInitULPFSParams_p->ulMs16QAMFactorWt > MAC_MAX_MODULATION_SCHEME_WT )
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, 
                "Invalid value for UL 16 QAM mod scheme"
                "Value will set from MAC_INIT_LAYER_REQ");
    }
    else
        ulSchdConfig_p->ulMs16QAMFactorWt =
            macInitULPFSParams_p->ulMs16QAMFactorWt;

    /* Setting uplink Modulation Scheme Weight for QPSK*/
    if (macInitULPFSParams_p->ulMsQPSKFactorWt > MAC_MAX_MODULATION_SCHEME_WT)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,  
                "Invalid value for UL 64 QAM mod scheme\
                Value will set from MAC_INIT_LAYER_REQ");
    }
    else
        ulSchdConfig_p->ulMsQPSKFactorWt =
            macInitULPFSParams_p->ulMsQPSKFactorWt;
    return;
}

/****************************************************************************
 * Function Name  : macInterfaceUpdateICICParamsInInitCellReq 
 * Inputs         : macInitULICICParams_p - Pointer to ICIC PFS paramters
 *                  dlSchdConfig_p - Pointer to init structure DL PFS params
 *                  ulSchdConfig_p - Pointer to init structure UL PFS params
 *                  internalCellIndex - cell id  
 * Outputs        : 
 * Returns        : None
 * Variables      :
 * Description    : This function will update ICIC parameters. 
 ****************************************************************************/
static  void macInterfaceUpdateICICParamsInInitCellReq(
     MacInitICICParams *macInitICICParams_p,
     DLSchedulerStrategyConfig *dlSchdConfig_p,
     ULSchedulerStrategyConfig *ulSchdConfig_p,
     InternalCellIndex internalCellIndex)
{

    if((macInitICICParams_p->CQIThresholdCellEdgeLayer3 > CQI_MAX) ||
            (macInitICICParams_p->CQIThresholdCellEdgeLayer3 < CQI_MIN))
    {
        lteWarning ("Invalid value for CQI ThresHold = %d",
                macInitICICParams_p->CQIThresholdCellEdgeLayer3);
        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                macInitICICParams_p->CQIThresholdCellEdgeLayer3, 
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For Layer 3 CE Region");
    }
    else
    {

        dlSchdConfig_p->cqiThresholdCellCenterLayer3[CE_REGION] = 
            macInitICICParams_p->CQIThresholdCellEdgeLayer3;
    }
    if((macInitICICParams_p->CQIThresholdCellEdgeLayer4 > CQI_MAX)  ||
            (macInitICICParams_p->CQIThresholdCellEdgeLayer4 < CQI_MIN))
    {
        lteWarning ("Invalid value for CQI ThresHold = %d",
                macInitICICParams_p->CQIThresholdCellEdgeLayer4);
        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                macInitICICParams_p->CQIThresholdCellEdgeLayer4, 
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For Layer4 CE Region");
    }
    else
    {
        dlSchdConfig_p->cqiThresholdCellCenterLayer4[CE_REGION] = 
            macInitICICParams_p->CQIThresholdCellEdgeLayer4;
    }

    if ( (macInitICICParams_p->cellEdgeUECQIThreshold > MAX_CQI_INDEX)||
            (macInitICICParams_p->cellEdgeUECQIThreshold < MIN_CQI_INDEX)  )
    {
        lteWarning ("Invalid value of CQIThreshold = %d received "
                " setting same value as received in MAC_INIT_LAYER_REQ\n",
                macInitICICParams_p->cellEdgeUECQIThreshold);
        LOG_MAC_MSG(OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(1),
                macInitICICParams_p->cellEdgeUECQIThreshold,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"Invalid Cqi Threshold Value For CC_CE Region");
    }
    else 
    { 
        dlSchdConfig_p->cqiThreshold[CE_REGION] = macInitICICParams_p->
            cellEdgeUECQIThreshold;
    }

    /* SPR 7981 changes start */
    dlSchdConfig_p->mcsThreshold[CC_CE_REGION] = getMCSFromCQI(
            dlSchdConfig_p->cqiThreshold[CC_CE_REGION]
            );
    dlSchdConfig_p->mcsThreshold[CE_REGION] = getMCSFromCQI(
            dlSchdConfig_p->cqiThreshold[CE_REGION]
            );
    /* SPR 7981 changes end */
    if ((macInitICICParams_p->maxCellEdgeUEsToBeScheduledUL < 
                MIN_UL_CE_USERS_SCHEDULED)
            || (macInitICICParams_p->maxCellEdgeUEsToBeScheduledUL >
                ulSchdConfig_p->maxUEsToBeScheduledUL ))
    {
        ulSchdConfig_p->maxCellEdgeUEsToBeScheduledUL =
            ulSchdConfig_p->maxUEsToBeScheduledUL ;

        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "Invalid value for UPLINK UE TO BE SCHEDULED\
                Setting its max value of maxUEsToBeScheduledUL - %d.\n",
                ulSchdConfig->maxUEsToBeScheduledUL);
    }
    else
    {
        ulSchdConfig_p->maxCellEdgeUEsToBeScheduledUL =
            macInitICICParams_p->maxCellEdgeUEsToBeScheduledUL;
    }
    /* ICIC changes start */
    if ((macInitICICParams_p->maxCellEdgeUEsToBeScheduledDL <
                MIN_DL_CE_USERS_SCHEDULED)
            || (macInitICICParams_p->maxCellEdgeUEsToBeScheduledDL >
                dlSchdConfig_p->maxUEsToBeScheduledDL) )
    {
        dlSchdConfig_p->maxCellEdgeUEsToBeScheduledDL = 
            dlSchdConfig_p->maxUEsToBeScheduledDL;
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG, 
                "Invalid value for DOWNPLINK UE TO BE SCHEDULED\
                Setting its max value of maxUEsToBeScheduledDL - %d.\n",
                ulSchdConfig_p->maxUEsToBeScheduledDL);
    }
    else
    {
        dlSchdConfig_p->maxCellEdgeUEsToBeScheduledDL =
            macInitICICParams_p->maxCellEdgeUEsToBeScheduledDL;
    }
    /* ICIC chnages end */
}
/****************************************************************************
 * Function Name  : macInterfacePrintDLULParamsInInitCellReq 
 * Inputs         : dlSchdConfig_p - Pointer to init structure DL PFS params
 *                  ulSchdConfig_p - Pointer to init structure UL PFS params
 * Outputs        : 
 * Returns        : None
 * Variables      :
 * Description    : This function will print updated UL/DL parameters. 
 ****************************************************************************/
static  void macInterfacePrintDLULParamsInInitCellReq(
            DLSchedulerStrategyConfig *dlSchdConfig_p,
            ULSchedulerStrategyConfig *ulSchdConfig_p) 
{

    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_UE_QL_Wt     : %d\n",dlSchdConfig_p->dwlkUEQLoadWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_MS64_QAM_FACTOR_WT : %d\n",dlSchdConfig_p->dlMs64QAMFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_MS16_QAM_FACTOR_WT : %d\n",dlSchdConfig_p->dlMs16QAMFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_MS_QPSK_FACTOR_WT : %d\n",dlSchdConfig_p->dlMsQPSKFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_MS_WT        : %d\n",dlSchdConfig_p->dwlkMSWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_LC_QL_WT     : %d\n",dlSchdConfig_p->dwlkLCQLoadWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_UE_PR_WT     : %d\n",dlSchdConfig_p->dwlkUEPriorityWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_LC_PR_WT     : %d\n",dlSchdConfig_p->dwlkLCPriorityWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_UE_DEL_WT    : %d\n",dlSchdConfig_p->dwlkUEDelayWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DL_LC_DEL_WT    : %d\n",dlSchdConfig_p->dwlkLCDelayWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "DOWNLINK UE TO BE SCHEDULED = %d\n",dlSchdConfig_p->maxUEsToBeScheduledDL);
    if (DL_ROUND_ROBIN_SCHEDULING == dlSchdConfig_p->dlSchedulerStrategy )
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "dlSchedulerStrategy = DL_ROUND_ROBIN_SCHEDULING \n");
    }
    else if(DL_FAIR_SCHEDULING == dlSchdConfig_p->dlSchedulerStrategy)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "dlSchedulerStrategy  = DL_FAIR_SCHEDULING\n");
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "dlSchedulerStrategy  = DL_QOS_BASED_SCHEDULING\n");
    }
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "NUM_ENCODER_THDS  : %d\n",NUM_OF_ENCODER_THREADS);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_MS64_QAM_FACTOR_WT     : %d\n",ulSchdConfig_p->ulMs64QAMFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_MS16_QAM_FACTOR_WT : %d\n",ulSchdConfig_p->ulMs16QAMFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_MS_QPSK_FACTOR_WT : %d\n",ulSchdConfig_p->ulMsQPSKFactorWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_UE_PR_WT     : %d\n",ulSchdConfig_p->uplkUEPriorityWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_MS_WT        : %d\n",ulSchdConfig_p->uplkMSWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_UE_DEL_WT    : %d\n",ulSchdConfig_p->uplkUEDelayWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UL_LCG_PR_WT    : %d\n",ulSchdConfig_p->uplkLCGPriorityWt);
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "UPLINK UE TO BE SCHEDULED = %d\n",ulSchdConfig_p->maxUEsToBeScheduledUL);
    if (UL_ROUND_ROBIN_SCHEDULING == ulSchdConfig_p->ulSchedulerStrategy )
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
             "ulSchedulerStrategy = UL_ROUND_ROBIN_SCHEDULING \n");
    }
    else if(UL_FAIR_SCHEDULING == ulSchdConfig_p->ulSchedulerStrategy)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "ulSchedulerStrategy = UL_FAIR_SCHEDULING\n");
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
                "ulSchedulerStrategy = UL_QOS_SCHEDULING\n");
    }
   return;
}

/****************************************************************************
 * Function Name  : macInterfaceUpdateSPSParamsInInitCellReq
 * Inputs         : macInitSPSParams_p - Pointer to SPS  params
 *                  initParams_p - Pointer to init structure 
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : This function will  update SPS parameters.
 ****************************************************************************/
static  void  macInterfaceUpdateSPSParamsInInitCellReq(
                     MacInitSPSParams *macInitSPSParams_p,
                              InitParams *initParams_p)
{
    UInt8 spsQciIndex  = 0;
    SpsSysWideParam   *spsSysWideParam_p =  &initParams_p->spsSysWideParam;
    /* Parsing and storing SPS Parameters */
    SpsSysWideParam *sysParam_p = &macInitSPSParams_p->spsSysWideParams;
    if(LTE_TRUE <  sysParam_p->enableSps)
    {
        /* Assigning defulat as SPS DISABLE */
        spsSysWideParam_p->enableSps = LTE_FALSE;
    }
    else
    {
        spsSysWideParam_p->enableSps = sysParam_p->enableSps;
    }
    if (LTE_TRUE == spsSysWideParam_p->enableSps)
    {
        /* Validate and Update DL interval List */
        macValidateAndUpdateIntervalList(
                spsSysWideParam_p->supportedSpsDlIntervalList,
                sysParam_p->supportedSpsDlIntervalList);

        /* Validate and Update UL interval List */
        macValidateAndUpdateIntervalList(
                spsSysWideParam_p->supportedSpsUlIntervalList,
                sysParam_p->supportedSpsUlIntervalList);
        /* Validate and Update maxSpsDciAttempt */
        if( (MAC_MIN_SPS_DCI_ATTEMPT > sysParam_p->maxSpsDciAttempt) || 
                ( MAC_MAX_SPS_DCI_ATTEMPT < sysParam_p->maxSpsDciAttempt ) ) 
        {
            spsSysWideParam_p->maxSpsDciAttempt = MAC_DEFAULT_SPS_DCI_ATTEMPT;
        }
        else
        {
            spsSysWideParam_p->maxSpsDciAttempt = sysParam_p->maxSpsDciAttempt; 
        }

        /* Validate and Update qciSpslist */
        for (spsQciIndex = 0; spsQciIndex < MAX_QCI; spsQciIndex++)
        {
            if (LTE_TRUE < sysParam_p->qciSpslist[spsQciIndex])
            {
                /* By default SPS shall be enabled for qci 1 and disabled for other qci */
                if (spsQciIndex == 0)
                {
                    spsSysWideParam_p->qciSpslist[spsQciIndex] = LTE_TRUE;
                }
                else
                {
                    spsSysWideParam_p->qciSpslist[spsQciIndex] = LTE_FALSE;
                }
            }
            else
            {
                spsSysWideParam_p->qciSpslist[spsQciIndex] = 
                    sysParam_p->qciSpslist[spsQciIndex];
            }
        }
        macInterfaceUpdateCommonSPSInfo(macInitSPSParams_p,initParams_p);
        macInterfaceUpdateDlUlSPSIntervalProfileInfo(macInitSPSParams_p,
                initParams_p);
    }
}

/****************************************************************************
 * Function Name  : macInterfaceUpdateCommonSPSInfo
 * Inputs         : macInitSPSParams_p - Pointer to SPS  params
 *                  initParams_p - Pointer to init structure 
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : This function will  update SPS parameters.
 ****************************************************************************/
static  void  macInterfaceUpdateCommonSPSInfo(
                     MacInitSPSParams *macInitSPSParams_p,
                              InitParams *initParams_p)
{
    UInt8 commSpsIndex = 0;
    SpsCommIntervalProfile *spsCommIntervalProfile =
        initParams_p->spsCommIntervalProfile;

    /* Validate and Update Common Interval Profile */ 
    for(commSpsIndex = 0; commSpsIndex <SPS_INTERVAL_LIST; commSpsIndex++)
    {
        if ((LTE_FALSE != initParams_p->spsSysWideParam.supportedSpsUlIntervalList[commSpsIndex]) || 
                (LTE_FALSE != initParams_p->spsSysWideParam.supportedSpsDlIntervalList[commSpsIndex]))
        {
            /* Validate and Update overrideSpsOccassionForOtherUe */
            if (LTE_TRUE < macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                    overrideSpsOccassionForOtherUe)
            {
                spsCommIntervalProfile[commSpsIndex].overrideSpsOccassionForOtherUe = 
                    LTE_FALSE;
            }
            else
            {
                spsCommIntervalProfile[commSpsIndex].overrideSpsOccassionForOtherUe  = 
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].overrideSpsOccassionForOtherUe ;
            }

            /* Validate and Update strictSpsResourceUsageForSpsLc */
            if (LTE_TRUE < macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                    strictSpsResourceUsageForSpsLc)
            {
                spsCommIntervalProfile[commSpsIndex].strictSpsResourceUsageForSpsLc = 
                    LTE_TRUE;
            }
            else
            {
                spsCommIntervalProfile[commSpsIndex].strictSpsResourceUsageForSpsLc = 
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].strictSpsResourceUsageForSpsLc;
            }

            /*  Validate and Update enableSilenceDetection */
            if (LTE_TRUE < macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                    enableSilenceDetection)
            {
                spsCommIntervalProfile[commSpsIndex].enableSilenceDetection = 
                    LTE_FALSE;
            }
            else
            {
                spsCommIntervalProfile[commSpsIndex].enableSilenceDetection = 
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                    enableSilenceDetection;
            }

            /*  Validate and Update interPktDelayForSilenceDetection */
            if (MAC_MIN_INTERPKT_DELAY_FOR_SILENCE_DETECTION > 
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                    interPktDelayForSilenceDetection || 
                    MAC_MAX_INTERPKT_DELAY_FOR_SILENCE_DETECTION < \
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                    interPktDelayForSilenceDetection)
            {
                spsCommIntervalProfile[commSpsIndex].interPktDelayForSilenceDetection = 
                    MAC_DEFAULT_INTERPKT_DELAY_FOR_SILENCE_DETECTION ;
            }
            else
            {
                spsCommIntervalProfile[commSpsIndex].interPktDelayForSilenceDetection =
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                    interPktDelayForSilenceDetection;
            }

            /*  Validate and Update maxPdcpPduSizeIpv4[ROHC_PROFILE_0] */
            spsCommIntervalProfile[commSpsIndex].maxPdcpPduSizeIpv4[ROHC_PROFILE0] =
                macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv4[ROHC_PROFILE0];

            /*  Validate and Update maxPdcpPduSizeIpv4[ROHC_PROFILE_1] */
            spsCommIntervalProfile[commSpsIndex].maxPdcpPduSizeIpv4[ROHC_PROFILE1] =
                macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv4[ROHC_PROFILE1];

            /*  Validate and Update maxPdcpPduSizeIpv4[ROHC_PROFILE_2] */
            spsCommIntervalProfile[commSpsIndex].maxPdcpPduSizeIpv4[ROHC_PROFILE2] =
                macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv4[ROHC_PROFILE2];

            /*  Validate and Update maxPdcpPduSizeIpv4[NO_ROHC_PROFILE] */
            spsCommIntervalProfile[commSpsIndex].maxPdcpPduSizeIpv4[MAC_ROHC_PROFILE - 1] =
                macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv4[NO_ROHC_PROFILE];

            /*  Validate and Update maxPdcpPduSizeIpv6[ROHC_PROFILE_0] */
            spsCommIntervalProfile[commSpsIndex].maxPdcpPduSizeIpv6[ROHC_PROFILE0] =
                macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv6[ROHC_PROFILE0];

            /*  Validate and Update maxPdcpPduSizeIpv6[ROHC_PROFILE_1] */
            spsCommIntervalProfile[commSpsIndex].maxPdcpPduSizeIpv6[ROHC_PROFILE1] =
                macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv6[ROHC_PROFILE1];

            /*  Validate and Update maxPdcpPduSizeIpv6[ROHC_PROFILE_2] */
            spsCommIntervalProfile[commSpsIndex].maxPdcpPduSizeIpv6[ROHC_PROFILE2] =
                macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv6[ROHC_PROFILE2];

            /*  Validate and Update maxPdcpPduSizeIpv6[NO_ROHC_PROFILE] */
            spsCommIntervalProfile[commSpsIndex].maxPdcpPduSizeIpv6[MAC_ROHC_PROFILE - 1] =
                macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].\
                maxPdcpPduSizeIpv6[NO_ROHC_PROFILE];

            /* Validate and Update  minRbSavedReactivationThres */
            if(  MAC_MAX_RB_SAVED_REACTIVATION_THRES < 
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].minRbSavedReactivationThres)
            {
                spsCommIntervalProfile[commSpsIndex].minRbSavedReactivationThres = 
                    MAC_DEFAULT_RB_SAVED_REACTIVATION_THRES;
            }
            else
            {
                spsCommIntervalProfile[commSpsIndex].minRbSavedReactivationThres = 
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].minRbSavedReactivationThres;
            }

            /* Validate and Update spsBlerThres */
            if (macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].spsBlerThres < 
                    MAC_MIN_SPS_BLER_THRES || 
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].spsBlerThres > 
                    MAC_MAX_SPS_BLER_THRES)
            {
                spsCommIntervalProfile[commSpsIndex].spsBlerThres = 
                    MAC_DEFAULT_SPS_BLER_THRES;
            }
            else
            {
                spsCommIntervalProfile[commSpsIndex].spsBlerThres =  
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].spsBlerThres;
            }

            /* Validate and Update auditInterval */
            if (macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].auditInterval > 
                    MAX_AUDIT_INTERVAL)
            {
                spsCommIntervalProfile[commSpsIndex].auditInterval = 
                    MAC_DEFAULT_AUDIT_INTERVAL;
            }
            else
            {
                spsCommIntervalProfile[commSpsIndex].auditInterval =  
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].auditInterval;
            }

            /* Validate and Update spsResAllocTtiAdv */
            if (macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].spsResAllocTtiAdv < 
                    SPS_MIN_RES_ALLOC_TTI_ADV || 
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].spsResAllocTtiAdv > 
                    SPS_MAX_RES_ALLOC_TTI_ADV)
            {
                spsCommIntervalProfile[commSpsIndex].spsResAllocTtiAdv = 
                    SPS_DEFAULT_RES_ALLOC_TTI_ADV;
            }
            else
            { 
                spsCommIntervalProfile[commSpsIndex].spsResAllocTtiAdv =  
                    macInitSPSParams_p->spsCommIntervalProfile[commSpsIndex].spsResAllocTtiAdv;
            }
        }
    }

    return;

}
/****************************************************************************
 * Function Name  : macInterfaceUpdateDlUlSPSIntervalProfileInfo
 * Inputs         : macInitSPSParams_p - Pointer to SPS  params
 *                  initParams_p - Pointer to init structure 
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : This function will  update SPS ul/dl interval parameters.
 ****************************************************************************/
static  void  macInterfaceUpdateDlUlSPSIntervalProfileInfo(
                    MacInitSPSParams *macInitSPSParams_p,
                              InitParams *initParams_p)

{
    UInt8 dlSpsIndex   = 0;
    UInt8 ulSpsIndex   = 0;
    ULSchedulerStrategyConfig *ulSchdConfig_p = &initParams_p->ulSchdConfig;
    DLSchedulerStrategyConfig *dlSchdConfig_p = &initParams_p->dlSchdConfig;
    SpsSysWideParam   *sysParam_p =   &initParams_p->spsSysWideParam;
    /* Validate and Update DL SPS INTERVAL PROFILE Parameters */
    for(dlSpsIndex = 0; dlSpsIndex < SPS_INTERVAL_LIST; dlSpsIndex++)
    {
        if(LTE_FALSE != sysParam_p->supportedSpsDlIntervalList[dlSpsIndex])
        {
            /* Validate and Update dlMinMcsSps */
            if( macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].dlMinMcsSps > 
                    MAC_MAX_DL_MIN_MCS_SPS)
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlMinMcsSps = 
                    MAC_DEFAULT_DL_MIN_MCS_SPS;
            }
            else
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlMinMcsSps = 
                    macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].dlMinMcsSps;
            }

            /* Validate and Update dlMcsMargin */
            if( macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].dlMcsMargin > 
                    MAC_MAX_DL_MIN_MCS_MARGIN )
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlMcsMargin = 
                    MAC_DEFAULT_DL_MIN_MCS_MARGIN;
            }
            else
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlMcsMargin = 
                    macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].dlMcsMargin;
            }

            /* Validate and Update dlTbsizeMarginSps */
            if( macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].dlTbsizeMarginSps > 
                    MAC_MAX_DL_TBSIZE_MARGIN_SPS)
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlTbsizeMarginSps = 
                    MAC_DEFAULT_DL_TBSIZE_MARGIN_SPS;
            }
            else
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlTbsizeMarginSps = 
                    macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].dlTbsizeMarginSps;
            }

            /* Validate and Update dlJitterHandlingOption */
            if (macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].dlJitterHandlingOption >= 
                    MAX_JITTER_HANDLING_OPT)
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlJitterHandlingOption = 
                    NO_HANDLING;
            }
            else
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].dlJitterHandlingOption =
                    macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].dlJitterHandlingOption;
            }

            /* Validate and Update rlcSduThresForDra */
            if (macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].rlcSduThresForDra < 
                    MAC_MIN_RLC_SDU_THRES_FOR_DRA || 
                    macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].rlcSduThresForDra > 
                    MAC_MAX_RLC_SDU_THRES_FOR_DRA)
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].rlcSduThresForDra = 
                    MAC_DEFAULT_RLC_SDU_THRES_FOR_DRA;
            }
            else
            {
                dlSchdConfig_p->spsDlIntervalProfile[dlSpsIndex].rlcSduThresForDra = 
                    macInitSPSParams_p->spsDLIntervalProfile[dlSpsIndex].rlcSduThresForDra;
            }
        }
    }
    /* Validate and Update UL Interval Profile Parameters */
    for (ulSpsIndex = 0; ulSpsIndex < SPS_INTERVAL_LIST; ulSpsIndex++)
    {
        if(LTE_FALSE != sysParam_p->supportedSpsUlIntervalList[ulSpsIndex])
        {
            /*  Validate and Update ulMinMcsSps */
            if( macInitSPSParams_p->spsULIntervalProfile[ulSpsIndex].ulMinMcsSps > 
                    MAC_MAX_UL_MIN_MCS_SPS)
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulMinMcsSps = 
                    MAC_DEFAULT_UL_MIN_MCS_SPS;
            }
            else
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulMinMcsSps = 
                    macInitSPSParams_p->spsULIntervalProfile[ulSpsIndex].ulMinMcsSps;
            }

            /*  Validate and Update ulMcsMargin */
            if( macInitSPSParams_p->spsULIntervalProfile[ulSpsIndex].ulMcsMargin > 
                    MAC_MAX_UL_MIN_MCS_MARGIN)
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulMcsMargin = 
                    MAC_DEFAULT_UL_MIN_MCS_MARGIN;
            }
            else
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulMcsMargin = 
                    macInitSPSParams_p->spsULIntervalProfile[ulSpsIndex].ulMcsMargin;
            }

            /*  Validate and Update ulTbSizeMarginSps */
            if( macInitSPSParams_p->spsULIntervalProfile[ulSpsIndex].ulTbSizeMarginSps >
                    MAC_MAX_UL_TBSIZE_MARGIN_SPS)
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulTbSizeMarginSps = 
                    MAC_DEFAULT_UL_TBSIZE_MARGIN_SPS;
            }
            else
            {
                ulSchdConfig_p->spsUlIntervalProfile[ulSpsIndex].ulTbSizeMarginSps = 
                    macInitSPSParams_p->spsULIntervalProfile[ulSpsIndex].ulTbSizeMarginSps;
            }
        }
    }
    return;
}

/***************************************************************************
 * Function Name  : macCommonGetNumberOfInitReceived
 * Inputs         : None
 * Outputs        : None
 * Returns        : Number of cell init received.
 * Variables      :
 * Description    : This function will return number of init received on
 *                  MAC.  
 ****************************************************************************/
/* + SPR 17439 */
 UInt8 macCommonGetNumberOfInitReceived(void)
/* - SPR 17439 */    
{
    return macParams_g.numInitReceived;
}
/* CA Changes end */
/*SPR 16855 +-*/


/****************************************************************************
 * Function Name  : initializeMAC 
 * Inputs         : numberOfCells
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API will reset all the statistics maintained at MAC. 
 ****************************************************************************/
/* CA Changes start */
MacRetType initializeMAC(UInt8 numberOfCells
        )
{
    InternalCellIndex cellIndex = 0;
#ifdef CIRC_QUEUE_IMPL
#ifdef DL_UL_SPLIT
#ifndef DL_DATA_SEPARATION
    /* + SPR 11209 Changes */
    InternalCellIndex cellIndex2 = 0;
    /* - SPR 11209 Changes */
#endif
    UInt8 containerIndex = 0;
#endif
#endif
    UInt8 totalNumOfCells = 0;

    /* coverity fix 27-JUN-2014 fix start */   
    /* SPR 22075 Changes Start */
    /* Removed initialisation of initParams_p as it is being unused */    
    /* SPR 22075 Changes End */
    /* coverity fix 27-JUN-2014 fix end */    

    /* init function call to initialise the ul scheduler strategy */ 
    if(numberOfCells)
    { 
    initDLSchedulerStrategy(numberOfCells);

    /* init function call to initialise the dl scheduler strategy */ 
    initULSchedulerStrategy(numberOfCells);
    initDRXMgr(numberOfCells); /* DRX_CHG */
    initMeasGapMgr(numberOfCells); /* MEAS_GAP_CHG */
#ifdef LTE_EMBMS_SUPPORTED
    initEmbmsMux(numberOfCells);
    initEmbmsEncoder(numberOfCells);
    initEmbmsSync();
    initMbmsSubFrameMaps(numberOfCells);
#endif
    /* + E_CID_5.3 */
    initECidMgr(numberOfCells); 
    /* - E_CID_5.3 */
    initUEConfMgr(numberOfCells);
    /*CA Changes start JAVED */
    initULResourceInfo(numberOfCells);
    initPUSCHPowerControl(numberOfCells);
    /*CA Changes end JAVED */
    /* Stage-III SCELL Deactivation Timer Related Changes Start */
    scellStateMngrInitScellDeactTimer(numberOfCells);
    /* Stage-III SCELL Deactivation Timer Related Changes End */
    /*initPUCCH(numberOfCells);*/

    /*FDD_SRS*/
    /*SRS is supported for TDD and FDD  mode*/
    /* SRS_CHG */
    /*This initializes the srs specific UE subframe maps 
      in TDD and FDD modes*/
    initSrsUESubFrameMaps(numberOfCells);
    /* SRS_CHG */
    /*FDD_SRS*/
    initPUSCH(numberOfCells);

    initQosToken(numberOfCells);

    /* init function call to initialise the PCCH manager */
    initPCCHMgr(numberOfCells);
    /* init function call to initialise the BCCH manager */ 
    initBCCHMgr(numberOfCells);

    initRACHMgr(numberOfCells);
    /* init function call to initialise the DL Resource manager */
    macInitDLResourceInfoMgr(numberOfCells);
    /* init function call to initialise the phy inetrface */ 
    initEncoderModule(numberOfCells);
    initDLHarq(numberOfCells);
#ifndef TDD_CONFIG  
#ifdef ULHARQ_TIMER_PROC
    initULHarq(numberOfCells);
    /*SPR_3305_FIX_START*/
    initULHarqForMsg3(numberOfCells);
    /*SPR_3305_FIX_END*/
#endif
#endif 
    initStatsManager();
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)    
    {        
        enableUplinkThroughput(1, cellIndex);
        enableDownlinkThroughput(1, cellIndex);
    }        
    /* parameter to keep retraversal off */

    initULCSIManager(numberOfCells); /* ULA_CHG */
    /*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
    init_dl_msgbuff(numberOfCells);
    /*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
        init_periodicReportPool(cellIndex);
    }

    init_ul_msgbuff(numberOfCells);

    /* HD FDD changes start */
#ifdef HD_FDD_CONFIG
    initHDGlobals();
#endif
    /* HD FDD changes end */

    /* SPR 5531 changes start */
    /* SPR 5570 changes start */
#if (((!defined(DL_UL_SPLIT)) && (!defined(CIRC_QUEUE_IMPL))) || defined(DL_UL_SPLIT_TDD))
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
        /* +- SPR 17777 */
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        initDLAckNackQueue(0, cellIndex);
#else
        initDLAckNackQueue(cellIndex);
#endif
        /* +- SPR 17777 */
    }
#endif
    /* SPR 5570 changes end */
    /* SPR 5531 changes end */

    /* Start 128 UE: Memory Changes*/
    initULConfigMsg(numberOfCells);
    /* End 128 UE: Memory Changes*/

#ifdef PCTEST_MACONLY
	//initMACUEOppertunityIndQ missed in mac only pctest, add here
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
        initMACUEOppertunityIndQ(0,cellIndex);
    }
#endif

#ifndef DL_UL_SPLIT_TDD
#ifdef CIRC_QUEUE_IMPL
    /* SPR 5570 changes start */
    /*CA Changes start JAVED */
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
        /* SPR 5570 changes end */
        /* +- SPR 17777 */
        initUlStrategyQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
                0,
#endif
                cellIndex);
        initPUSCHSignalQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
                0,
#endif
                cellIndex);
        /* +- SPR 17777 */
    }
#ifdef DL_UL_SPLIT
    createMemPool(sizeof(NACK_CIRC_TYPE), numberOfCells); 
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
        for (containerIndex = 0; containerIndex < MAX_NUMBER_OF_CONTAINER; containerIndex++)
        {
            initULContainerNackQueue(
                    (&(ulContainerPool_gp[cellIndex] + containerIndex)->ulContainerQIdUL),
                    cellIndex, containerIndex);
            initULContainerNackQueueForTempUE(
                    (&(ulContainerPool_gp[cellIndex] + containerIndex)->ulContainerNackQueueForTempUEIdUL), 
                    cellIndex, containerIndex);
        }
        semInitDefault(&waitForULNackQueue_g[cellIndex], 0); 
        semInitDefault(&waitForULNackForTempUE_g[cellIndex], 0);
    }
#endif
    /*CA Changes end JAVED */
    /* SPR 8712 changes */
#ifndef DL_DATA_SEPARATION
    /* OPP Ind Queue is getting init from RLC-MAC i/f */
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
        initMACUEOppertunityIndQ(0,cellIndex);
    }

#ifdef DL_UL_SPLIT
    UInt32 encoderContainerCount =0 ;
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
    /* + SPR 11209 Changes */
        for (cellIndex2 = 0; cellIndex2 < numberOfCells; cellIndex2++)
        {
            for (encoderContainerCount=0; encoderContainerCount < MAX_ENCODER_CONTAINERS;encoderContainerCount++)
            {
                initEncoderQueue(&(encoderContainerArr_gp[cellIndex][cellIndex2] + encoderContainerCount)->\
                    encoderNode.encoderQueueIdUL, 
                    encoderContainerCount,cellIndex, cellIndex2);
            }
        }
    /* - SPR 11209 Changes */
    }
#endif
#endif    
    /* SPR 8712 changes */
#endif
#else
    initUlStrategyQueue(0);
    initPDCCHSignalQueue();
#endif
    
    /* Review comment fix start MAC_AT_8 */
    /* calling function to initialize MAC-RRM interface */
    /* CA Changes start */
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
        initMacRRMInterface(cellIndex);
    }
    }
    totalNumOfCells = numberOfCells;
    /* CA Changes end */
    /* Review comment fix end MAC_AT_8 */

    /* Rel 5.3: Coverity 24640 Fix Start */
    /*
     ** Created Memory to reduce Stack utilization in
     ** parseMacReconfigureUEEntityReq() and macReconfigUeRequestHandler() 
     */
    createMemPool(sizeof(UeContextForUplink), NUM_OF_UPLNK);
    createMemPool(sizeof(UeContextForDownlink), NUM_OF_DWNLNK);
    /* Rel 5.3: Coverity 24640 Fix End */

    /* Memory handling changes start */
    createMemPool(8,200);
    /* *************************************
     * These generic 200 memory pools are used for 
     * UEULMUMIMOInfoNode
     * UENumOfLayerNode
     * UEBeamformingAlgoNode
     * ContentionResAckNode
     * RRCRachInterfaceNode
     * RRCRachChangeCRNTIInterface
     * RRCRachHOInterfaceNode
     * ConnHOEntityFailureInterfaceNode
     * RRCRachDeleteUeInterface
     * RRCRachDeleteUeHoInterfacePDCCHRachNode
     * PDCCHRachNode
     * *************************************/
    createMemPool(16,75);
    /* *************************************
     * These generic 75 memory pools are used for 
     * CCCHQueueNode
     * PendingCCCHQueueNode
     * ContentionTimerNode
     * *************************************/
    /* Memory handling changes end */
    /*******Memory Handling changes 64UE Start****************************/
/* coverity fix 27-JUN-2014 fix start */    
#ifndef MEM_LEAK_DEBUG
/* coverity fix 27-JUN-2014 fix end */  
    /** SPR 13295 Changes Start **/
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
        /* SPR 22075 Changes Start */
        /* Extending msg pool for 16K buffers irrespective of 64 or 128 UEs */
        /* SPR 22075 Changes End */
        msgPoolExtend(16384,NUM_16K_MSG_BUF_EXT_2);
    }
    /** SPR 13295 Changes End **/
 
#endif
    if(totalNumOfCells)
    {
        initMUX(totalNumOfCells);
        initDeMuxMgr( NUM_OF_DEMUX_THREADS,totalNumOfCells);
        initPHYInterface(totalNumOfCells);
        initEncoderModule(totalNumOfCells);
        init_dl_msgbuff(totalNumOfCells);
        init_ul_msgbuff(totalNumOfCells);
    }



/* SPR 11109 16-JUN-2014 Fix end */
    /*******Memory Handling changes 64UE end****************************/
    /* SPR 5062 Changes End */
    /* + SPR_11589 */  
    /*Create Memory pools only for NB cell */
    macParams_g.isInitDone = TRUE;
    /* - SPR_11589 */  
    return MAC_SUCCESS;
}
 
/* HD FDD changes start */
#ifdef HD_FDD_CONFIG
/****************************************************************************
 * Function Name  : initHDGlobals
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API will init all the globals used in HD at MAC.
 ****************************************************************************/
/* + SPR 17439 */
void initHDGlobals(void)
/* - SPR 17439 */    
{
    UInt16 ue;
    UInt8 ctr=0;

    hdUeContextInfo_g.numHdUe = 0;

    for (ue=0; ue<MAX_UE_SUPPORTED; ue++)
    {
      hdUeContextInfo_g.hdUeIndex[ue] = INVALID_UE_INDEX;
      hdUeContextInfo_g.configChange[ue] = FALSE;
      hdUeContextInfo_g.ulSrPresentFlag[ue] = FALSE;

      for (ctr=0; ctr<HD_UE_SCHED_MAP_SIZE; ctr++)
      {
        hdUeContextInfo_g.hdfddUeSchedMap[ctr].hdfddUeSfMap[ue] = 0;
      }
      for (ctr=0; ctr<HD_UE_NUM_OF_REPORTS; ctr++)
      {
        HD_UE_RESET_REPORT_INFO (ue, ctr);
      }
    }

    return;
}
#endif
/* HD FDD changes end */

/****************************************************************************
 * Function Name  : macResetStats
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API will reset all the statistics maintained at MAC. 
 ****************************************************************************/
/* CA Changes start */
UInt16 macResetStats(InternalCellIndex  internalCellIndex)
{
    numOfrachInd_g[internalCellIndex] = 0;   
    /* Reset Uplink Throughput Statistics */
    statsInfo_g[internalCellIndex].ulThroughPutEnable = 0;
    statsInfo_g[internalCellIndex].totalUplinkBytesSend = 0;
    statsInfo_g[internalCellIndex].ulStartThroughputTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
    statsInfo_g[internalCellIndex].ulStartSpecEffTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
    statsInfo_g[internalCellIndex].ulThroughPutEnable = 1;
    
    /* Reset Downlink Throughput Statistics */
    statsInfo_g[internalCellIndex].dlThroughPutEnable = 0;
    statsInfo_g[internalCellIndex].totalDlBytesSend = 0;
    statsInfo_g[internalCellIndex].dlStartThroughputTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
    statsInfo_g[internalCellIndex].dlStartSpecEffTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
    statsInfo_g[internalCellIndex].dlThroughPutEnable = 1;
    totalMacUllinkBytesSend_g[internalCellIndex] = 0;
    totalMacDllinkBytesSend_g = 0;
    /* SPR 14829 Fix Start */
    statsInfo_g[internalCellIndex].lastMacUlThroughput = 0;
    statsInfo_g[internalCellIndex].lastMacDlThroughput = 0;
    /* SPR 14829 Fix End */
    return MAC_SUCCESS;
}
/* CA Changes end */


/****************************************************************************
 * Function Name  : parseGetStatusReq
 * Inputs         : msgBuf_p - Pointer to request buffer received from OAM.
 *                  cnfBuff_p - Pointer to confirmation buffer.
 *                  current_p - Pointer to current idx of cnfBuff_p.
 *                  Internal cell index for which data is requested
 * Outputs        : None
 * Returns        : MAC_SUCCESS or an error code
 * Description    : This API will parse the get UE Status request and 
 *                  prepares a confirmation message. 
 ****************************************************************************/
 /* +- SPR 17777 */
UInt16 parseGetStatusReq(UInt8 *msgBuf_p,
                         UInt8 *cnfBuff_p, UInt16 *currentBuf_p,
                         InternalCellIndex cellIndex)
 /* +- SPR 17777 */
{
    UInt16 ueIndex = MAX_UE_SUPPORTED;
    UInt16 rrcUeIndex = INVALID_RRC_UE_INDEX;
    MacUEStatusReq *macUEStatusReq_p =
        (MacUEStatusReq *)(msgBuf_p + MAC_API_HEADER_LEN); 
    UInt16 resp = MAC_SUCCESS;

    if(TRUE == macUEStatusReq_p->isUeIdPresent )
    {
        rrcUeIndex = macUEStatusReq_p->ueId;
        /* get UE index from RRC UE Index */
        ueIndex = getUeIdxFromRrcUeIdx(rrcUeIndex);

        if(ueIndex >= MAX_UE_SUPPORTED)
        {
            /* SPR 2153 Fix Start */
            *currentBuf_p += sizeof(MacUEStatusResp);
            /* SPR 2153 Fix End */
            return MAC_INVALID_UE_ID;
        }    
        resp = fillUeStatus(cnfBuff_p, currentBuf_p, &ueIndex, cellIndex);
    }
    else 
    {
        resp = fillUeStatus(cnfBuff_p, currentBuf_p, PNULL, cellIndex); 
    }

    return resp;
}


/****************************************************************************
 * Function Name  : macGetStats
 * Inputs         : cnfBuff_p - Pointer to confirmation buffer
 *                  current_p - Pointer to the current idx of cnfBuff_p.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS or an error code
 * Description    : This API will fill the stats in the confirmation buffer. 
 ****************************************************************************/
/* CA Changes start */
void macGetStats(UInt8 *cnfBuff, UInt16 *current_p,
                 InternalCellIndex  internalCellIndex)
/* CA Changes end */
{
    MacStatsResp * macStatsResp_p =
          (MacStatsResp *)(cnfBuff + MAC_API_HEADER_LEN);

    /* CA Changes start */
    if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
         cellState <= CELL_PHY_INIT)
    /* CA Changes end */
    {
        macStatsResp_p->responseCode = MAC_LAYER_NOT_INITIALIZED;
        return;
    }

    macStatsResp_p->responseCode = MAC_SUCCESS;
    macStatsResp_p->dlSpectralEfficiency = getDlSpectralEfficiency(internalCellIndex);
    macStatsResp_p->ulSpectralEfficiency = getUlSpectralEfficiency(internalCellIndex);
    /* SPR 20675 FIX + */
    macStatsResp_p->dlThroughPut = (macStatsResp_p->dlSpectralEfficiency)*
                                                 ((dlBandwidth_g[internalCellIndex])*1000000);
    macStatsResp_p->ulThroughPut = (macStatsResp_p->ulSpectralEfficiency)*
                                                 ((ulBandwidth_g[internalCellIndex])*1000000);
    /* SPR 20675 FIX - */
    *current_p += sizeof(MacStatsResp);

    /* CA Changes start */
    LOG_MAC_MSG(OAM_MAC_STAT_INFO_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            macStatsResp_p->ulThroughPut,macStatsResp_p->dlThroughPut,
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellState, macStatsResp_p->dlSpectralEfficiency,
            macStatsResp_p->ulSpectralEfficiency,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"OAM_MAC_STAT_INFO");
    /* CA Changes end */

    return ;
}

#ifdef KPI_STATS
/*spr 19296 fix start*/
/****************************************************************************
 * Function Name  : MacUpdateKpiThpStatsForLc
 * Inputs         : ueId - UE Identifier
 *                  lcId - LC identifier
 *                  entry - entry in response buffer
 *                  qci - QCI value for this LC
 *                  pLtePdcpKPIThpStats - shared memory container
 * Outputs        : cnfBuff - Pointer to confirmation buffer
 * Returns        : None.
 * Description    : This function fills the Throughput stats.
 ****************************************************************************/
void MacUpdateKpiThpStatsForLc( UInt16 ueId, UInt8 lcId, UInt8 entry, 
        UInt8 qci, LteMacKPIThpStats *pLteMacKPIThpStats, 
        MacKpiThpStatsIndParams * cnfBuff )
{
    cnfBuff->ueIndex[entry] = getRrcUeIdxFromUeIdx( ueId );
    cnfBuff->qci[entry] = qci;

    cnfBuff->thpVolDl[entry] = INVALID_THP_STATS_ENTRY;
    cnfBuff->thpVolUl[entry] = INVALID_THP_STATS_ENTRY;
    cnfBuff->thpTimeDl[entry] = INVALID_THP_ENTRY;
    cnfBuff->thpTimeUl[entry] = INVALID_THP_ENTRY;
    cnfBuff->latTime[entry] = INVALID_THP_ENTRY;
    cnfBuff->latSample[entry] = INVALID_THP_ENTRY;
/* + SPR 20962 */
    /*SPR 21551 Fix Start*/
    if(( 0 != pLteMacKPIThpStats->scheduledIPThpVolDL[ueId][lcId]) && ((INVALID_THP_STATS_ENTRY) > pLteMacKPIThpStats->scheduledIPThpVolDL[ueId][lcId]))
    /*SPR 21551 Fix End*/
/* - SPR 20962 */
    {
        /* thpVolDl is sent in bytes to OAM */
        cnfBuff->thpVolDl[entry] = 
            pLteMacKPIThpStats->scheduledIPThpVolDL[ueId][lcId];
    }
/* + SPR 20962 */
    else
    {
        cnfBuff->thpVolDl[entry] = 0;
    }
    /*SPR 21551 Fix Start*/
    if(( 0 != pLteMacKPIThpStats->scheduledIPThpVolUL[ueId][lcId]) && ((INVALID_THP_STATS_ENTRY) > pLteMacKPIThpStats->scheduledIPThpVolUL[ueId][lcId]))
    /*SPR 21551 Fix End*/
/* - SPR 20962 */
    {
        /* thpVolUl is sent in bytes to OAM */
        cnfBuff->thpVolUl[entry] = 
            pLteMacKPIThpStats->scheduledIPThpVolUL[ueId][lcId];
    }
/* + SPR 20962 */
    else
    {
        cnfBuff->thpVolUl[entry] = 0;
    }
    /*SPR 21551 Fix Start*/
    if(( 0 != pLteMacKPIThpStats->scheduledIPThpTimeDL[ueId][lcId]) && ((INVALID_THP_ENTRY) > pLteMacKPIThpStats->scheduledIPThpTimeDL[ueId][lcId]))
    /*SPR 21551 Fix End*/
/* - SPR 20962 */
    {
        /* thpTimeDl is sent in miliseconds to OAM */
        cnfBuff->thpTimeDl[entry] = 
            pLteMacKPIThpStats->scheduledIPThpTimeDL[ueId][lcId];
    }
/* + SPR 20962 */
    else
    {
        cnfBuff->thpTimeDl[entry] = 0;
    }
    /*SPR 21551 Fix Start*/
    if(( 0 != pLteMacKPIThpStats->scheduledIPThpTimeUL[ueId][lcId]) && ((INVALID_THP_ENTRY) > pLteMacKPIThpStats->scheduledIPThpTimeUL[ueId][lcId]))
    /*SPR 21551 Fix End*/
/* - SPR 20962 */
    {
        /* thpTimeUl is sent in miliseconds to OAM */
        cnfBuff->thpTimeUl[entry] = 
            pLteMacKPIThpStats->scheduledIPThpTimeUL[ueId][lcId];
    }
/* + SPR 20962 */
    else
    {
        cnfBuff->thpTimeUl[entry] = 0;
    }
    /*SPR 21551 Fix Start*/
    if(( 0 != pLteMacKPIThpStats->sharedMemoryLatSample[ueId][lcId]) && ((INVALID_THP_ENTRY) > pLteMacKPIThpStats->sharedMemoryLatSample[ueId][lcId]))
    /*SPR 21551 Fix End*/
/* - SPR 20962 */
    {
        /* latSample is number of samples to be sent */
        cnfBuff->latSample[entry] = 
            pLteMacKPIThpStats->sharedMemoryLatSample[ueId][lcId];
        /* latTime is sent in miliseconds to OAM */
        cnfBuff->latTime[entry] = 
            pLteMacKPIThpStats->SharedMemoryLatTime[ueId][lcId];
    }
/* + SPR 20962 */
    else
    {
        cnfBuff->latSample[entry] = 0;
        cnfBuff->latTime[entry] = 0;
    }
/* - SPR 20962 */
}
/*spr 19296 fix end*/

/****************************************************************************
 * Function Name  : macGetKpiThpStats
 * Inputs         : cnfBuff - Pointer to confirmation buffer
 *                  current_p - Pointer to the current idx of cnfBuff_p.       
 *                  kpiBitmap - Bitmap indicating KPI's to be pegged.
 *                  moduleId
 * Outputs        : None
 * Returns        : None
 * Description    : This API will fill the Throughput stats in the confirmation buffer.
 ****************************************************************************/
/*spr19296 fix start*/
	 /*code deleted*/
/*spr 19296 fix end*/
/****************************************************************************
 * Function Name  : macGetKpiStats
 * Inputs         : cnfBuff - Pointer to confirmation buffer,
 *                  current_p - Pointer to the current idx of cnfBuff_p,
 *                  kpiBitmap - Bitmap indicating KPI's to be pegged,
 *                  pLteMacKPIStats - Stats to be filled,
 *                  durationTTI - duration of TTI
 * Outputs        : None
 * Returns        : None
 * Description    : This API will fill the stats in the confirmation buffer for OAM.
 ****************************************************************************/
void macGetKpiStats(UInt8 *cnfBuff, UInt16 *current_p, UInt16 kpiBitmap, LteMacKPIStats  *pLteMacKPIStats, UInt32 durationTTI)
{
    /* SPR 5464 */         
    MacGetKpiStatsResp * macGetKpiStatsResp_p = (MacGetKpiStatsResp *)(cnfBuff);
    /* SPR 5464 */         

    UInt8 qci =0;
    UInt8 cqi = 0;
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    UInt8 operatorId = 0;
    UInt8 numOfOperator = mocnOperatorParams_g.numOfOperator;
    /* MOCN Changes End */
    /* SPR 10409 changes start */
    memSet(macGetKpiStatsResp_p, 0, sizeof(MacGetKpiStatsResp));
    /* SPR 10409 changes end */
    macGetKpiStatsResp_p->DurationTTI = durationTTI;
    /* MOCN Changes Start */
    macGetKpiStatsResp_p->numOfOperators = numOfOperator;
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
    /* SPR 23499 Fix +- */
    if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_TOTAL_PRB_USAGE_DL))
    {
        macGetKpiStatsResp_p->totalPRBUsageDL = pLteMacKPIStats->totalDLPRBUsage;
    }
    if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_TOTAL_PRB_USAGE_UL))
    {
        macGetKpiStatsResp_p->totalPRBUsageUL = pLteMacKPIStats->totalUplinkPRBUsage;
    }
    if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_RCVD_RANDOM_ACCESS_PREAMBLES))
    {
        macGetKpiStatsResp_p->rcvdRandomAccessPreamblesContentionFree = pLteMacKPIStats->totalRcvdRandomAccessPreamblesContentionFree;
        macGetKpiStatsResp_p->rcvdRandomAccessPreamblesGroupA = pLteMacKPIStats->totalRcvdRandomAccessPreamblesGroupA;
        macGetKpiStatsResp_p->rcvdRandomAccessPreamblesGroupB = pLteMacKPIStats->totalRcvdRandomAccessPreamblesGroupB;
    }
    /* SPR 13492 Changes Start */
    if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_IN_SESSION_ACTIVITY_TIME))
    {
        macGetKpiStatsResp_p->totalSessionTimeUE = pLteMacKPIStats->totalSessionTimeUE;
    }
    /* SPR 13492 Changes End */

    /*  KPI changes start */
    for (cqi = 0; cqi < MAX_CQI_INDEX; cqi++)
    {
        if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_MAC_CQI_RECEIVED))
        {
            macGetKpiStatsResp_p->totalCQICW1Reported[cqi] = pLteMacKPIStats->totalCQICW1Reported[cqi];
            macGetKpiStatsResp_p->totalCQICW2Reported[cqi] = pLteMacKPIStats->totalCQICW2Reported[cqi];
        }
    }     

    /* KPI changes end  */
    /* all the counters that require loop upto MAX_QCI are clubbed together */
    for (qci = 0; kpiBitmap && (qci < MAX_QCI); qci++)
    {
        if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_TOTAL_PRB_USAGE_PER_QCI_DL))
        {
            /* SPR 10021 fix start */
            /* Value is left shifted in encoder while filling, hence filling the
             * value after right shifting */
            macGetKpiStatsResp_p->totalPRBUsagePerQciDL[qci] = 
                pLteMacKPIStats->totalDLPRBUsagePerQCI[qci] >> 
                SCALE_UP_MULTIPLYING_FACTOR;
            if( 0 == macGetKpiStatsResp_p->totalPRBUsagePerQciDL[qci] &&
                    0 != pLteMacKPIStats->totalDLPRBUsagePerQCI[qci] )
            {
                macGetKpiStatsResp_p->totalPRBUsagePerQciDL[qci] = 1;
            }
            /* SPR 10021 fix end */
        }
        if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_TOTAL_PRB_USAGE_PER_QCI_UL))
        {
            /* SPR 10021 fix start */
            macGetKpiStatsResp_p->totalPRBUsagePerQciUL[qci] = 
                pLteMacKPIStats->totalULPRBUsagePerQCI[qci] >> 
                SCALE_UP_MULTIPLYING_FACTOR;
            if( 0 == macGetKpiStatsResp_p->totalPRBUsagePerQciUL[qci] &&
                    0 != pLteMacKPIStats->totalULPRBUsagePerQCI[qci] )
            {
                macGetKpiStatsResp_p->totalPRBUsagePerQciUL[qci] = 1;
            }
            /* SPR 10021 fix end */
        }
        if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_DL))
        {
            macGetKpiStatsResp_p->numberOfActiveUEPerQciDL[qci] = pLteMacKPIStats->totalActiveUsersPerQCIDLCumulative[qci];
        }
        if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_UL))
        {
            macGetKpiStatsResp_p->numberOfActiveUEPerQciUL[qci] = pLteMacKPIStats->totalActiveUsersPerQCIULCumulative[qci];
        }
        if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_PKT_DELAY_PER_QCI_DL))
        {
            macGetKpiStatsResp_p->totalPktDelayPerQciDL[qci] = pLteMacKPIStats->totalPktDelayInDLPerQCI[qci];
            macGetKpiStatsResp_p->totalPktAckedInDLPerQci[qci] = pLteMacKPIStats->totalPktAckedInDLPerQCI[qci]; 
        }
        /* SPR 13492 Changes Start */
        if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_IN_SESSION_ACTIVITY_TIME))
        {
            macGetKpiStatsResp_p->totalSessionTimePerQci[qci] = pLteMacKPIStats->totalSessionTimePerQCI[qci];
        }
        /* SPR 13492 Changes End */
        /*spr 19296 fix start*/
        if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_TOTAL_DL_DRB_UU_LOSS_RATE))
        {
            macGetKpiStatsResp_p->totalDlDrbUuLossRatePerQci[qci] =
                pLteMacKPIStats->totalDlDrbUuLossRatePerQci[qci];
            macGetKpiStatsResp_p->totalSduAckRcvdDlperQci[qci] =
                pLteMacKPIStats->totalSduAckRcvdDlperQci[qci];
        } 
        /*spr 19296 fix end*/
        /* SPR 23402 Changes Start */
           /* MOCN Changes Start */
        for(operatorId=0; operatorId < MAX_NUM_MOCN_OPERATOR; operatorId++)
        {
            if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_TOTAL_PRB_USAGE_PER_QCI_DL))
            {
                if (operatorId<numOfOperator)
                { 
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].totalPRBUsagePerQciDL[qci] = 
                        pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[operatorId][qci] >> SCALE_UP_MULTIPLYING_FACTOR;
                    if (0 == macGetKpiStatsResp_p->kpiPerOperator[operatorId].totalPRBUsagePerQciDL[qci] &&
                            0 != pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[operatorId][qci])
                    {
                        macGetKpiStatsResp_p->kpiPerOperator[operatorId].totalPRBUsagePerQciDL[qci] = 1;
                    }

                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].operatorId = operatorId;
                }
                else
                {
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].operatorId = INVALID_OPERATOR_ID;
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].totalPRBUsagePerQciDL[qci] = 0;   
                }
            }

            if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_TOTAL_PRB_USAGE_PER_QCI_UL))
            {
                if (operatorId<numOfOperator)
                {
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].totalPRBUsagePerQciUL[qci] = 
                        pLteMacKPIStats->totalULPRBUsagePerOperatorPerQCI[operatorId][qci] >> SCALE_UP_MULTIPLYING_FACTOR;
                    if (0 == macGetKpiStatsResp_p->kpiPerOperator[operatorId].totalPRBUsagePerQciUL[qci] &&
                            0 != pLteMacKPIStats->totalULPRBUsagePerOperatorPerQCI[operatorId][qci])
                    {
                        macGetKpiStatsResp_p->kpiPerOperator[operatorId].totalPRBUsagePerQciUL[qci] = 1;
                    }

                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].operatorId = operatorId;
                }
                else
                {
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].operatorId = INVALID_OPERATOR_ID;
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].totalPRBUsagePerQciUL[qci] = 0;
                }
            }

            if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_DL))
            {
                if (operatorId<numOfOperator)
                {
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].numberOfActiveUEPerQciDL[qci] = 
                        pLteMacKPIStats->totalActiveUsersPerOperatorPerQCIDLCumulative[operatorId][qci];

                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].operatorId = operatorId;
                }
                else
                {
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].operatorId = INVALID_OPERATOR_ID;
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].numberOfActiveUEPerQciDL[qci] = 0;
                }
            }

            if(IS_KPI_MAC_BITSET(kpiBitmap, KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_UL))
            {
                if (operatorId<numOfOperator)
                { 
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].numberOfActiveUEPerQciUL[qci] = 
                        pLteMacKPIStats->totalActiveUsersPerOperatorPerQCIULCumulative[operatorId][qci];

                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].operatorId = operatorId;
                }
                else
                {
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].operatorId = INVALID_OPERATOR_ID;
                    macGetKpiStatsResp_p->kpiPerOperator[operatorId].numberOfActiveUEPerQciUL[qci] = 0;
                }  
            }
        }
		/* MOCN Changes End */
        /* SPR 23402 Changes End */
    }
    /*EICIC Change*/	
    macGetKpiStatsResp_p->AbsSubframeCount=pLteMacKPIStats->AbsSubframeCount;
    /*EICIC Change*/

    *current_p +=sizeof(MacGetKpiStatsResp);

    return;
}

/****************************************************************************
 * Function Name  : sendMacKpiThpStatsInd
 * Inputs         : respMsgId - Message in whih response is to be sent,
 *                  moduleCounter
 * Outputs        : None
 * Returns        : None
 * Description    : This API will send MAC_KPI_STATS_IND with KPI cumulative
 *                  counters maintained at MAC.
 ****************************************************************************/
void sendMacKpiThpStatsInd(UInt32 moduleCounter)
{
    UInt8           *responseBuf         = PNULL;   /* Memory buffer for response */
    UInt32          moduleBitmap         = 0;
    LteKPIThpStatus *pLteMacKPIThpStatus = PNULL;
    LteMacKPIConfig *pLteMacKPIConfig    = PNULL;
    /* SPR 22296 Fix Start */
    UInt8 cellCount = 0;
    UInt8 numCellsConfigured = cellSpecificParams_g.numOfCells;
    UInt8 resetKpi = LTE_FALSE;
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        responseBuf = (UInt8 *)getMemFromPool( (L2_RRM_API_HEADER_LEN + \
                    sizeof(MacKpiThpStatsIndParams)), PNULL);
        if ( PNULL == responseBuf ) 
        {
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic ("getMemPool error in %s", __func__);
            return;
        }

        pLteMacKPIThpStatus = &(g_kpiScannerObj.kpiStatsContainer[moduleCounter].status.lteMacKPIThpStatus);
        moduleBitmap = pLteMacKPIThpStatus->moduleBitmap; 

        pLteMacKPIConfig = &(g_kpiScannerObj.kpiStatsContainer[moduleCounter].config.lteMacKPIConfig);
        if(!(pLteMacKPIConfig->kpiBitmap))
        {
            freeMemPool ( responseBuf );
            return;
        }

        if(cellCount == (numCellsConfigured-1))
        {
            resetKpi = LTE_TRUE;
        }

        if(moduleBitmap & (0x01 << moduleCounter) && (pLteMacKPIConfig->kpiBitmap))
        {
            macGetAndSendKpiThpStats( responseBuf, 
                    pLteMacKPIConfig->transactionId, pLteMacKPIConfig->kpiBitmap,
                    moduleCounter,cellCount,g_kpiModuleInfo[moduleCounter].periodicThpIndMsgType, resetKpi);
        }
        freeMemPool ( responseBuf );
    }
    /* SPR 22296 Fix End */
}
/*spr 19296 fix end*/

/****************************************************************************
 * Function Name  : macResetKpiCounters
 * Inputs         : passiveIndexForMacStats, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API will reset all the KPI cumulative counters maintained at MAC.
 ****************************************************************************/
void  macResetKpiCounters(UInt32 passiveIndexForMacStats, InternalCellIndex internalCellIndex)
{
    if(gMacStats.pStats)
        memSet(&(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[passiveIndexForMacStats]), 0, sizeof(LteMacKPIStats));
    return;
}
/* SPR23194 Start */
/****************************************************************************
 * Function Name  : macFillStatsOfDeletedLc
 * Inputs         : moduleThpId, cnfBuff and entry
 * Outputs        : idx_p - index till which entries for Deleted UE is filled
 * Returns        : None
 * Description    : This function will update container for deleted RABs.;
 ****************************************************************************/
void  macFillStatsOfDeletedLc(UInt8 moduleThpId,
        MacKpiThpStatsIndParams *cnfBuff, UInt8 *idx_p)
{
    UInt8 lcId = 0;
    UInt16 deleteUeIndex = MAX_UE_SUPPORTED;
    UInt64 tempscheduledIPThpVolDL = 0;
    UInt64 tempscheduledIPThpVolUL = 0;
    UInt64 tempscheduledIPThpTimeDL = 0;
    UInt64 tempscheduledIPThpTimeUL = 0;
    UInt64 tempsharedMemoryLatSample = 0;
    UInt64 tempSharedMemoryLatTime = 0;
    LteMacKPIThpStats *pLteMacKPIThpStats =
       &(g_kpiScannerObj.kpiStatsContainer[moduleThpId].stats.lteMacKPIThpStats); 

    for (lcId = 0; lcId < MAX_LCID_THP; lcId++)
    {
        if( (0 != pLteMacKPIThpStats->scheduledIPThpVolDL[deleteUeIndex][lcId]) ||
                (0 != pLteMacKPIThpStats->scheduledIPThpVolUL[deleteUeIndex][lcId]))
        {
            tempscheduledIPThpVolDL = pLteMacKPIThpStats->scheduledIPThpVolDL[deleteUeIndex][lcId];
            tempscheduledIPThpVolUL = pLteMacKPIThpStats->scheduledIPThpVolUL[deleteUeIndex][lcId];
            tempscheduledIPThpTimeDL = pLteMacKPIThpStats->scheduledIPThpTimeDL[deleteUeIndex][lcId];
            tempscheduledIPThpTimeUL = pLteMacKPIThpStats->scheduledIPThpTimeUL[deleteUeIndex][lcId];
            tempsharedMemoryLatSample = pLteMacKPIThpStats->sharedMemoryLatSample[deleteUeIndex][lcId];
            tempSharedMemoryLatTime = pLteMacKPIThpStats->SharedMemoryLatTime[deleteUeIndex][lcId];

            cnfBuff->ueIndex[*idx_p] = deleteUeIndex;
            /* storing the qci corresponding to the lcId */
            cnfBuff->qci[*idx_p] = kpiLcToQciMapping_g[lcId];
            
            /* Resetting the kpiLcToQciMapping_g index */
            kpiLcToQciMapping_g[lcId] = 0;

            cnfBuff->thpVolDl[*idx_p] = tempscheduledIPThpVolDL;
            cnfBuff->thpVolUl[*idx_p] = tempscheduledIPThpVolUL;
            cnfBuff->thpTimeDl[*idx_p] = tempscheduledIPThpTimeDL;
            cnfBuff->thpTimeUl[*idx_p] = tempscheduledIPThpTimeUL;
            cnfBuff->latTime[*idx_p] = tempSharedMemoryLatTime;
            cnfBuff->latSample[*idx_p] = tempsharedMemoryLatSample;
            
            /* Updating the KPI index for next stat */
            (*idx_p)++;
        }
    }
}

/****************************************************************************
 * Function Name  : macNumQciEntries
 * Inputs         : moduleThpId
 * Outputs        : None
 * Returns        : count - number of entries
 * Description    : This function will return the number of entries to update
 *                  the KPI stats information for deleted RABs.
 ****************************************************************************/
UInt8  macNumQciEntries(UInt8 moduleThpId)
{
    UInt8 lcId = 0;
    UInt8 count = 0;
    UInt16 deleteUeIdx = MAX_UE_SUPPORTED;
    LteMacKPIThpStats *pLteMacKPIThpStats =
        &(g_kpiScannerObj.kpiStatsContainer[moduleThpId].stats.lteMacKPIThpStats);

    for (lcId = 0; lcId < MAX_LCID_THP; lcId++)
    {
        if( (0 != pLteMacKPIThpStats->scheduledIPThpVolDL[deleteUeIdx][lcId]) ||
                (0 != pLteMacKPIThpStats->scheduledIPThpVolUL[deleteUeIdx][lcId]) )
        {
            count++;
        }
    }
    return count;
}


/* SPR23194 End */
/****************************************************************************
 * Function Name  : updateMacKpiContainer
 * Inputs         : passiveIndexForKpiStats and moduleId
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function will update KPI Container for OAM.
 ****************************************************************************/
void updateMacKpiContainer(UInt32 passiveIndexForKpiStats, UInt32 moduleId,
			InternalCellIndex internalCellIndex)
{
    UInt8 qci = 0;
    UInt8 cqi = 0;
    /* SPR 23402 Changes Start */ 
    /* MOCN Changes Start */
    UInt8 operatorId = 0;
    UInt8 numOfOperator = mocnOperatorParams_g.numOfOperator;
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
    LteMacKPIStats *pMacOamKPIStats = 
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.lteMacKPIStats[internalCellIndex]);
    LteMacKPIConfig *pMacOamKpiConfig = 
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].config.lteMacKPIConfig);
    LteMacKPIStats *pLteMacKPIStats =
        &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[passiveIndexForKpiStats]);

    pMacOamKPIStats->lastCalculatedTTI += pLteMacKPIStats->lastCalculatedTTI;
    if (!pMacOamKpiConfig->kpiBitmap)
    {
        return ;
    }

    if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_TOTAL_PRB_USAGE_DL))
    {
        pMacOamKPIStats->totalDLPRBUsage += pLteMacKPIStats->totalDLPRBUsage;
    }
    if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_TOTAL_PRB_USAGE_UL))
    {
        pMacOamKPIStats->totalUplinkPRBUsage += pLteMacKPIStats->totalUplinkPRBUsage;
    }
    if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_RCVD_RANDOM_ACCESS_PREAMBLES))
    {
        pMacOamKPIStats->totalRcvdRandomAccessPreamblesContentionFree += 
            pLteMacKPIStats->totalRcvdRandomAccessPreamblesContentionFree;
        pMacOamKPIStats->totalRcvdRandomAccessPreamblesGroupA += 
            pLteMacKPIStats->totalRcvdRandomAccessPreamblesGroupA;
        pMacOamKPIStats->totalRcvdRandomAccessPreamblesGroupB += 
            pLteMacKPIStats->totalRcvdRandomAccessPreamblesGroupB;
    }
    /* SPR 13492 Changes Start */
    if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_IN_SESSION_ACTIVITY_TIME))
    {
        pMacOamKPIStats->totalSessionTimeUE += pLteMacKPIStats->totalSessionTimeUE;
    }
    /* SPR 13492 Changes End */

    if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_MAC_CQI_RECEIVED))
    {
        for (cqi = 0; cqi < MAX_CQI_INDEX; cqi++)
        {
            pMacOamKPIStats->totalCQICW1Reported[cqi] += 
                pLteMacKPIStats->totalCQICW1Reported[cqi];
            pMacOamKPIStats->totalCQICW2Reported[cqi] += 
                pLteMacKPIStats->totalCQICW2Reported[cqi];
        }
    }

    /* all the counters that require loop upto MAX_QCI are clubbed together */
    for (qci = 0; qci < MAX_QCI; qci++)
    {
        if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_TOTAL_PRB_USAGE_PER_QCI_DL))
        {
            pMacOamKPIStats->totalDLPRBUsagePerQCI[qci] += 
                pLteMacKPIStats->totalDLPRBUsagePerQCI[qci];
        }
        if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_TOTAL_PRB_USAGE_PER_QCI_UL))
        {
            pMacOamKPIStats->totalULPRBUsagePerQCI[qci] += 
                pLteMacKPIStats->totalULPRBUsagePerQCI[qci];
        }
        if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_DL))
        {
            pMacOamKPIStats->totalActiveUsersPerQCIDLCumulative[qci] += 
                pLteMacKPIStats->totalActiveUsersPerQCIDLCumulative[qci];
        }
        if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_UL))
        {
            pMacOamKPIStats->totalActiveUsersPerQCIULCumulative[qci] += 
                pLteMacKPIStats->totalActiveUsersPerQCIULCumulative[qci];
        }
        if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_PKT_DELAY_PER_QCI_DL))
        {
            pMacOamKPIStats->totalPktDelayInDLPerQCI[qci] += 
                pLteMacKPIStats->totalPktDelayInDLPerQCI[qci];
            pMacOamKPIStats->totalPktAckedInDLPerQCI[qci] += 
                pLteMacKPIStats->totalPktAckedInDLPerQCI[qci]; 
        }
        /* SPR 13492 Changes Start */
        if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_IN_SESSION_ACTIVITY_TIME))
        {
            pMacOamKPIStats->totalSessionTimePerQCI[qci] += 
                pLteMacKPIStats->totalSessionTimePerQCI[qci];
        }
        /* SPR 13492 Changes End */
        /*spr 19296 fix start*/
        if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_TOTAL_DL_DRB_UU_LOSS_RATE))
        {
            pMacOamKPIStats->totalDlDrbUuLossRatePerQci[qci] +=
                pLteMacKPIStats->totalDlDrbUuLossRatePerQci[qci];
            pMacOamKPIStats->totalSduAckRcvdDlperQci[qci] +=
                pLteMacKPIStats->totalSduAckRcvdDlperQci[qci];
        }
        /*spr 19296 fix end*/
        /* SPR 23402 Changes Start */
        /* MOCN Changes Start */
        for(operatorId=0; operatorId < MAX_NUM_MOCN_OPERATOR; operatorId++)
        {	
            if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_TOTAL_PRB_USAGE_PER_QCI_DL))
            {
                if (operatorId<numOfOperator)
                {	
                    pMacOamKPIStats->totalDLPRBUsagePerOperatorPerQCI[operatorId][qci] += pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[operatorId][qci];	
                }
                else
                {
                    pMacOamKPIStats->totalDLPRBUsagePerOperatorPerQCI[operatorId][qci]  = 0;
                }
            }
            if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_TOTAL_PRB_USAGE_PER_QCI_UL))
            {
                if (operatorId<numOfOperator)
                {
                    pMacOamKPIStats->totalULPRBUsagePerOperatorPerQCI[operatorId][qci] += pLteMacKPIStats->totalULPRBUsagePerOperatorPerQCI[operatorId][qci];

                }
                else
                {
                    pMacOamKPIStats->totalULPRBUsagePerOperatorPerQCI[operatorId][qci] = 0;
                }
            }
            if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_DL))
            {
                if (operatorId<numOfOperator)
                {
                    pMacOamKPIStats->totalActiveUsersPerOperatorPerQCIDLCumulative[operatorId][qci] += 
                        pLteMacKPIStats->totalActiveUsersPerOperatorPerQCIDLCumulative[operatorId][qci];
                }
                else
                {
                    pMacOamKPIStats->totalActiveUsersPerOperatorPerQCIDLCumulative[operatorId][qci] = 0;
                }
            }
            if(IS_KPI_MAC_BITSET(pMacOamKpiConfig->kpiBitmap, KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_UL))
            {
                if (operatorId<numOfOperator)
                {
                    pMacOamKPIStats->totalActiveUsersPerOperatorPerQCIULCumulative[operatorId][qci] += 
                        pLteMacKPIStats->totalActiveUsersPerOperatorPerQCIULCumulative[operatorId][qci];
                }
                else
                {
                    pMacOamKPIStats->totalActiveUsersPerOperatorPerQCIULCumulative[operatorId][qci] = 0;
                }
            }
        }
        /* MOCN Changes end */
        /* SPR 23402 Changes End */
    }
	/* EICIC +*/
    pMacOamKPIStats->AbsSubframeCount = pLteMacKPIStats->AbsSubframeCount;
    /* EICIC -*/
	
    return;
}

/* SPR 10579 changes start*/
/****************************************************************************
 * Function Name  : updateMacKpiThpContainer
 * Inputs         : passiveIndexForKpiThpStats and moduleId
 * Outputs        : None
 * Returns        : None
 * Description    : This function will update KPI Container for OAM.
 ****************************************************************************/
void updateMacKpiThpContainer(UInt32 passiveIndexForKpiThpStats, UInt32 moduleId)
{
    /* SPR23194 Start */
    UInt16 deleteUeIndex = MAX_UE_SUPPORTED;
    /* SPR23194 End */
    LteMacKPIThpStats *pMacOamKPIThpStats = 
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.lteMacKPIThpStats);
    LteMacKPIConfig *pMacOamKpiThpConfig  = 
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].config.lteMacKPIConfig);

    LteMacKPIThpStats *pLteMacKPIThpStats =
        &(gMacStats.pStats->lteCommonUeKPIStats.lteMacKPIThpStats[passiveIndexForKpiThpStats]);

    LteKPIThpStatus *pLteMacKPIThpStatus  =
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].status.lteMacKPIThpStatus);

    if (!pMacOamKpiThpConfig->kpiBitmap)
    {
        return ;
    }

    DLUEContext           *ueContext_p = PNULL;
    DLLogicalChannelInfo  *dlLogicalChannel_p = PNULL;
    UInt8 lcId =0;
    /* +- SPR 18268 */
    UInt16 ueId = 0;
    /* +- SPR 18268 */

    pMacOamKPIThpStats->durationTTI    += pLteMacKPIThpStats->durationTTI;

    for (ueId = pLteMacKPIThpStatus->startUeIdNxtBatch; ueId < MAX_UE_SUPPORTED; ueId++)
    {
        ueContext_p = dlUECtxInfoArr_g[ueId].dlUEContext_p;
        if(!ueContext_p || dlUECtxInfoArr_g[ueId].pendingDeleteFlag)
        {
            continue;
        }
        for (lcId = 0; lcId < MAX_LCID_THP; lcId++)
        {
            dlLogicalChannel_p = &ueContext_p->logicalChannel[lcId+3];
            if(INVALID_LCID == dlLogicalChannel_p->logicalChannelId)
            {
                continue;
            }
/*spr 19296 fix start*/
	    /*Code deleted totalDlLcThp KPI discarded at MAC*/
/*spr 19296 fix end*/
/*spr 19296 fix start*/
/* coverity 108713 +*/
            if(pLteMacKPIThpStats->scheduledIPThpVolDL[ueId][lcId]!= 0)
            {
                pMacOamKPIThpStats->scheduledIPThpVolDL[ueId][lcId] += pLteMacKPIThpStats->scheduledIPThpVolDL[ueId][lcId];
                pMacOamKPIThpStats->scheduledIPThpTimeDL[ueId][lcId] += pLteMacKPIThpStats->scheduledIPThpTimeDL[ueId][lcId];
            }
            if(pLteMacKPIThpStats->scheduledIPThpVolUL[ueId][lcId]!= 0)
            {
                pMacOamKPIThpStats->scheduledIPThpVolUL[ueId][lcId] += pLteMacKPIThpStats->scheduledIPThpVolUL[ueId][lcId];
                pMacOamKPIThpStats->scheduledIPThpTimeUL[ueId][lcId] += pLteMacKPIThpStats->scheduledIPThpTimeUL[ueId][lcId];
            }

            if(pLteMacKPIThpStats->sharedMemoryLatSample[ueId][lcId]!= 0)
            {
                pMacOamKPIThpStats->sharedMemoryLatSample[ueId][lcId] += pLteMacKPIThpStats->sharedMemoryLatSample[ueId][lcId];
                pMacOamKPIThpStats->SharedMemoryLatTime[ueId][lcId] += pLteMacKPIThpStats->SharedMemoryLatTime[ueId][lcId];
            }
/* coverity 108713 -*/
/*spr 19296 fix end*/
        } /* end of for (lcid) */
    }/* end of for (ueId) */
    /* SPR23194 Start */
    for (lcId = 0; lcId < MAX_LCID_THP; lcId++)
    {
        if(pLteMacKPIThpStats->scheduledIPThpVolDL[deleteUeIndex][lcId]!= 0)
        {
            pMacOamKPIThpStats->scheduledIPThpVolDL[deleteUeIndex][lcId] += pLteMacKPIThpStats->scheduledIPThpVolDL[deleteUeIndex][lcId];
        }
        if(pLteMacKPIThpStats->scheduledIPThpVolUL[deleteUeIndex][lcId]!= 0)
        {
            pMacOamKPIThpStats->scheduledIPThpVolUL[deleteUeIndex][lcId] += pLteMacKPIThpStats->scheduledIPThpVolUL[deleteUeIndex][lcId];
        }
        if(pLteMacKPIThpStats->scheduledIPThpTimeDL[deleteUeIndex][lcId]!= 0)
        {
            pMacOamKPIThpStats->scheduledIPThpTimeDL[deleteUeIndex][lcId] += pLteMacKPIThpStats->scheduledIPThpTimeDL[deleteUeIndex][lcId];
        }
        if(pLteMacKPIThpStats->scheduledIPThpTimeUL[deleteUeIndex][lcId]!= 0)
        {
            pMacOamKPIThpStats->scheduledIPThpTimeUL[deleteUeIndex][lcId] += pLteMacKPIThpStats->scheduledIPThpTimeUL[deleteUeIndex][lcId];
        }
        if(pLteMacKPIThpStats->sharedMemoryLatSample[deleteUeIndex][lcId]!= 0)
        {
            pMacOamKPIThpStats->sharedMemoryLatSample[deleteUeIndex][lcId] += pLteMacKPIThpStats->sharedMemoryLatSample[deleteUeIndex][lcId];
        }
        if(pLteMacKPIThpStats->SharedMemoryLatTime[deleteUeIndex][lcId]!= 0)
        {
            pMacOamKPIThpStats->SharedMemoryLatTime[deleteUeIndex][lcId] += pLteMacKPIThpStats->SharedMemoryLatTime[deleteUeIndex][lcId];
        }
    }
    /* SPR23194 End */
}
/* SPR 10579 changes end*/
/* + SPR 10579 Merge Fix */
/* Code removed */
/* + SPR 10579 Merge Fix */
/****************************************************************************
 * Function Name  : updateStatsForAllModules
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function will update KPI Containers.
 ****************************************************************************/
 /* + KPI_CA */
void updateStatsForAllModules(InternalCellIndex internalCellIndex)
{
    UInt32 modCounter                 = 0; 
    UInt32 passiveIndexForMacStats = (gMacStats.pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    UInt32 passiveIndexForPdcpStats = (gMacStats.pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    /* SPR 10579 changes start*/
    UInt32 passiveIndexForMacThpStats = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats;
    UInt32 passiveIndexForPdcpThpStats = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats;

    /* SPR 10579 changes end*/

    /* Switch the buffer from passive to active area */
    (gMacStats.pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats) = 
        passiveIndexForMacStats ^0x01;
    (gMacStats.pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats) = 
        passiveIndexForPdcpStats ^0x01;
    /* SPR 10579 changes start*/
    (gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats) = 
        passiveIndexForMacThpStats ^0x01;
    (gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats) = 
        passiveIndexForPdcpThpStats ^0x01;
    /* SPR 10579 changes end*/

    /* Update all the containers */
    for(modCounter=0;modCounter<MAX_KPI_CONTAINER;modCounter++)
    {
        if(g_kpiScannerObj.module_bitmask & (1<<modCounter))
        {
            if( MAC_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats)
            {
                updateMacKpiContainer(passiveIndexForMacStats, modCounter, internalCellIndex);
            }
            else if ( PDCP_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats)
            {
                updatePdcpKpiContainer(passiveIndexForPdcpStats, modCounter, internalCellIndex);
            }
            /* SPR 10579 changes start*/
            else if ( MAC_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats)
            {
                updateMacKpiThpContainer(passiveIndexForMacThpStats, modCounter);
            }
            else if ( PDCP_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats)
            {
                /*SPR 15692 fix start*/
                updatePdcpKpiThpContainer(passiveIndexForPdcpThpStats, modCounter,internalCellIndex);
                /*SPR 15692 fix end*/
            }
            /* SPR 10579 changes end*/
        }
    }

    macResetKpiCounters(passiveIndexForMacStats, internalCellIndex);
    pdcpResetKpiCounters(passiveIndexForPdcpStats,internalCellIndex);
    /* SPR 10579 changes start*/
    resetMacThpStats(passiveIndexForMacThpStats);
    resetPdcpThpStats(passiveIndexForPdcpThpStats);
    /* SPR 10579 changes end*/
    return;
}
 /* - KPI_CA */

/****************************************************************************
 * Function Name  : processMACGetKpiStatReq
 * Inputs         : macGetKpiStatsReqParams_p - Pointer to get stats received from OAM.
 *                  modCounter - Module id from where req is received. like RRM, OAM, etc.
 *                  cnfBuff_p - Pointer to confirmation buffer.
 *                  current_p - Pointer to current idx of cnfBuff_p.
 *                  internalCellIndex - cell id
 * Outputs        : None
 * Returns        : None
 * Description    : This API parses the Get KPI requests and sends response message
 *                  with indicated KPI's
 ****************************************************************************/
void processMACGetKpiStatReq(MacGetKpiStatsReqParams *macGetKpiStatsReqParams_p, UInt8 modCounter, 
        UInt8 *cnfBuff_p, UInt16 *currentBuf_p, InternalCellIndex internalCellIndex)
{
    UInt16 kpiBitmap                     = 0;
    /* 100MS_SUPPORT : start */
    UInt8 resetStats                     = 0;
    /* 100MS_SUPPORT : end */
    LteMacKPIConfig * pLteMacKPIConfig   = NULL;
    LteMacKPIStats  *pLteMacKPIStats     = &(g_kpiScannerObj.kpiStatsContainer[modCounter].stats.lteMacKPIStats[internalCellIndex]);
    pLteMacKPIConfig                     = &(g_kpiScannerObj.kpiStatsContainer[modCounter].config.lteMacKPIConfig);

    /* Update the KPI Stats Containers */
    /* 100MS_SUPPORT : start */
    kpiBitmap  = (pLteMacKPIConfig->kpiBitmap & macGetKpiStatsReqParams_p->kpiToReportBitMap);
    if ((pLteMacKPIConfig->kpiConfigured) && kpiBitmap)
    {
        /* SPR 5464 */
        updateStatsForAllModules(internalCellIndex);
/* EICIC Change Phase1 */
        macGetKpiStats(cnfBuff_p+g_kpiModuleInfo[modCounter].hdrLen, 
                currentBuf_p, 
                kpiBitmap, 
                pLteMacKPIStats,
                pLteMacKPIStats->lastCalculatedTTI);
        /* 100MS_SUPPORT : end */
        /* SPR 5464 */         
/* EICIC Change Phase1 */		
    }

    resetStats = macGetKpiStatsReqParams_p->resetStats;
    if ((pLteMacKPIConfig->kpiConfigured)&& (0 != resetStats ))
    {
        resetKpiContainer(modCounter, internalCellIndex);
    }
    /* 10409 SPR fix end */
    return;
}

/****************************************************************************
 * Function Name  : parseMACGetKpiStatReq
 * Inputs         : msgBuf_p - Pointer to request buffer received from OAM.
 *                  cnfBuff_p - Pointer to confirmation buffer,
 *                  current_p - Pointer to current idx of cnfBuff_p,
 *                  modCounter 
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 *                  internalCellIndex - cell id
 * Description    : This API parses the Get KPI requests and sends response message
 *                  with indicated KPI's
 ****************************************************************************/
/* CA Changes start */
void parseMACGetKpiStatReq(UInt8 *msgBuf_p,
                           UInt8 *cnfBuff_p, UInt16 *currentBuf_p, UInt8 modCounter,
                           InternalCellIndex  internalCellIndex)
/* CA Changes end */
{
    MacGetKpiStatsReqParams *macGetKpiStatsReqParams_p = NULL;
    if(KPI_CONTAINER_RRM_MAC == modCounter)
    {
        macGetKpiStatsReqParams_p = (MacGetKpiStatsReqParams *)(msgBuf_p);
    }
    else
    {
        macGetKpiStatsReqParams_p = (MacGetKpiStatsReqParams *)(msgBuf_p + 
                    g_kpiModuleInfo[modCounter].hdrLen);
    }
    /* CA Changes start */
    processMACGetKpiStatReq(macGetKpiStatsReqParams_p, modCounter,
            cnfBuff_p, currentBuf_p, internalCellIndex);

    return;
}
#endif

#ifdef PERF_STATS

/****************************************************************************
 * Function Name  : resetMacPerfContainer
 * Inputs         : resetId
 * Outputs        : None
 * Returns        : None
 * Description    : This function resets Cell and UE Containers at MAC
 *******************************************************************************/
void resetMacPerfContainer(UInt8 resetId)
{  
    switch(resetId)
    {
	case PERF_STATS_CONTAINER_OAM_MAC_RESET_CELL_CONTAINER:
            
            memSet(&(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].
                     perfStats.lteMacPerfStats.lteMacCellPerfStats), 0 ,sizeof(CellMacPerfStatsResp));
            break;
            
	case PERF_STATS_CONTAINER_OAM_MAC_RESET_UE_CONTAINER:

            memSet(&(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].
                       perfStats.lteMacPerfStats.lteMacUePerfStats), 0 ,PERF_MAX_UE_SUPPORTED * sizeof(UeMacPerfStatsResp));
	    break;
        /* Perf stats RRM-MAC start */
        case PERF_STATS_CONTAINER_RRM_MAC_RESET_CELL_CONTAINER:
            memSet(&(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_RRM_MAC].
                     perfStats.lteMacPerfStats.lteMacCellPerfStats), 0 ,sizeof(CellMacPerfStatsResp));
            break;
        case PERF_STATS_CONTAINER_RRM_MAC_RESET_UE_CONTAINER:
            memSet(&(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_RRM_MAC].
                       perfStats.lteMacPerfStats.lteMacUePerfStats), 0 ,PERF_MAX_UE_SUPPORTED * sizeof(UeMacPerfStatsResp));
	    break;
        /* Perf stats RRM-MAC end */
	default:
	    break;
    }
}

/****************************************************************************
 *  Function Name  : macResetUePerfStatCounters
 *  Inputs         : uePerfStats - Pointer to UE Mac perf stats
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This API will reset all the Perf Stats cumulative
 *                   counters maintained at MAC.
 ******************************************************************************/
void  macResetUePerfStatCounters(LteUePerfStats *uePerfStats_p)
{
    /* SPR 5449 changes start */
    UInt32 index = 0;
    if(gPerfStats.perfStats)
    {
        for (index = 0; index < PERF_MAX_UE_SUPPORTED; index++)
        {
            if ((PNULL !=dlUECtxInfoArr_g[index].dlUEContext_p)&& \
                    (PNULL != ulUECtxInfoArr_g[index].ulUEContext_p))
            {
                memSet (&uePerfStats_p[index].lteUeMacULPerfStats, 0,\
                        (sizeof(LteUeMacULPerfStats)));
                memSet (&uePerfStats_p[index].lteUeMacDLPerfStats, 0,\
                        (sizeof(LteUeMacDLPerfStats)));
               /* SPR_9403_FIX_START */
                memSet (&uePerfStats_p[index].lteUeMacSchedULPerfStats, 0,\
                        (sizeof(LteUeMacSchedULPerfStats)));
                memSet (&uePerfStats_p[index].lteUeMacSchedDLPerfStats, 0,\
                        (sizeof(LteUeMacSchedDLPerfStats)));
               /* SPR_9403_FIX_END */

            }
        }
    }
    /* SPR 5449 changes end */
    return;
}

/****************************************************************************
 *  Function Name  : macResetCellPerfStatCounters
 *  Inputs         : cellPerfStats_p - pointer to LteCellPerfStats
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This API will reset all the Perf Stats cumulative 
 *                   counters maintained at MAC.
 ******************************************************************************/
void  macResetCellPerfStatCounters(LteCellPerfStats *cellPerfStats_p)
{
    if(gPerfStats.perfStats)
    {
        memSet(&cellPerfStats_p->lteCellMacULPerfStats , 0, sizeof(LteCellMacULPerfStats));
        memSet(&cellPerfStats_p->lteCellMacDLPerfStats , 0, sizeof(LteCellMacDLPerfStats));
    }
}

/****************************************************************************
 *  Function Name  : ueMacFillPerfStats
 *  Inputs         : cnfBuff_p - Pointer to confirmation buffer
 *                   length_p - Pointer to the current idx of cnfBuff_p.
 *                   uePerfStatsBitMap - Stats to be filled
 *                   ueIndex - stores the UE index to be filled next
 *                   and periodicFlag
 *                   internalCellIndex - cell id  
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This API will fill the UE perf stats in the confirmation buffer for OAM.
 ******************************************************************************/
/* + stats_review_comments_ipc */
void ueMacFillPerfStats(UInt8 *cnfBuff,
                        UInt32 *length_p,
                        UInt32 uePerfStatsBitMap, 
                        UInt32 *ueIndex,
                        UInt32 periodicFlag,
						InternalCellIndex cellIndex)
/* - stats_review_comments_ipc */
{
    UInt32 cqi = 0;


    UInt32 rank = 0;
    UInt32 rankIndex = 0;
    UInt32 sinrIndex = 0;
    UInt32 modScheme = 0;


    UInt32 ueCount = 0;
    UInt32 length = 0;
    UInt32 firstBitSet = 0;
    UInt32 localKpiBitmap = 0;

    UeMacPerfStatsResp *ueMacPerfStatsResp_p                 = PNULL;
    MacUePerfStatsIndParams *macUePerfStatsIndParams_p       = PNULL;

    MacGetUePerfStatsCnfParams *macGetUePerfStatsCnfParams_p = PNULL;
    CqiRIPerfStats            *pCqiRIPerfStats            = PNULL;
    HarqPerfStats             *pHarqPerfStats             = PNULL;
    UeCapabilityStats         *pUeCapabilityStats         = PNULL;
    UePowerLimitedStats       *pUePowerLimitedStats       = PNULL;
    UeTransmissionModeRIStats *pUeTransmissionModeRIStats = PNULL;
    UeSINRPerfStats           *pUeSINRPerfStats           = PNULL;
    UePRBUsageStats           *pUePRBUsageStats           = PNULL;
    RadioThpVolStats          *pRadioThpVolStats          = PNULL;

    UeMacPerfStatsResp *pLteUeMacPerfStats = PNULL;

    length = MAC_API_HEADER_LEN;

    if(periodicFlag)
    {
        macUePerfStatsIndParams_p = (MacUePerfStatsIndParams *)&cnfBuff[length];
        macUePerfStatsIndParams_p->duration = g_perfStatsScannerObj.\
         perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].perfStats.lteMacPerfStats.lastMACCalculatedTTI;
    }
    else
    {
        macGetUePerfStatsCnfParams_p = (MacGetUePerfStatsCnfParams *)&cnfBuff[length];
        macGetUePerfStatsCnfParams_p->duration = g_perfStatsScannerObj.\
           perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].perfStats.lteMacPerfStats.lastMACCalculatedTTI;
    }
    for( ueCount = 0; ((ueCount < MAX_UE_PER_BATCH) && (*ueIndex < PERF_MAX_UE_SUPPORTED)); (*ueIndex)++ )
    {
		/* Check for existance of UE context*/
		if ((PNULL ==dlUECtxInfoArr_g[*ueIndex].dlUEContext_p)&& \
			(PNULL == ulUECtxInfoArr_g[*ueIndex].ulUEContext_p))
		{
	   		continue;
		}
		/* + stats_review_comments_ipc */
		/* handling of ue stats  ind for all ues */
		if (API_HEADER_DO_NOT_CARE_CELL_INDEX != cellIndex )
		{
			/* handling of ue stats attched to cell */
			if(dlUECtxInfoArr_g[*ueIndex].dlUEContext_p->internalCellIndex != cellIndex)
			{
				continue;
			}
		}
		/* - stats_review_comments_ipc */
	
        /* finding the pointer to Perf Stats resp acc to periodic flag*/
        if(periodicFlag)
        {
            ueMacPerfStatsResp_p = (UeMacPerfStatsResp *) (macUePerfStatsIndParams_p->ueMacPerfStatsResp + ueCount);
			/* + SPR 11670 Fix */
            ueMacPerfStatsResp_p->ueindex = getRrcUeIdxFromUeIdx(*ueIndex);
			/* - SPR 11670 Fix */
        }
        else
        {
            ueMacPerfStatsResp_p = (UeMacPerfStatsResp *) (macGetUePerfStatsCnfParams_p->ueMacPerfStatsResp + ueCount);
			/* + SPR 11670 Fix */
            ueMacPerfStatsResp_p->ueindex = getRrcUeIdxFromUeIdx(*ueIndex);
			/* - SPR 11670 Fix */
        }
	/* Pointer to store the current UE stats */
	pLteUeMacPerfStats = &(g_perfStatsScannerObj.
		perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].perfStats.lteMacPerfStats.lteMacUePerfStats[*ueIndex]);

	pCqiRIPerfStats = &(pLteUeMacPerfStats->cqiRIPerfStats);
	pHarqPerfStats  = &(pLteUeMacPerfStats->harqPerfStats);
	pUeCapabilityStats  = &(pLteUeMacPerfStats->ueCapabilityStats);
        pUePowerLimitedStats = &(pLteUeMacPerfStats->uePowerLimitedStats);
	pUeTransmissionModeRIStats = &(pLteUeMacPerfStats->ueTransmissionModeRIStats);
	pUeSINRPerfStats = &(pLteUeMacPerfStats->ueSINRPerfStats);
	pUePRBUsageStats = &(pLteUeMacPerfStats->uePRBUsageStats);
	pRadioThpVolStats = &(pLteUeMacPerfStats->radioThpVolStats);

	localKpiBitmap = uePerfStatsBitMap;
	/* fills the response Msg for the requested performace events per UE*/
    /* SPR 7385 Changes Starts */
	firstBitSet = FFS(localKpiBitmap);    
    /* SPR 7385 Changes Ends */
	while (firstBitSet)
	{
	    switch (firstBitSet - 1)
	    {
		case PERF_STATS_CQI:
		    for( cqi = 0; cqi < MAX_CQI_INDEX; cqi++ )
		    {
			ueMacPerfStatsResp_p->cqiRIPerfStats.totalCQICW1Reported[cqi] =
			    pCqiRIPerfStats->totalCQICW1Reported[cqi];

			ueMacPerfStatsResp_p->cqiRIPerfStats.totalCQICW2Reported[cqi] =
			    pCqiRIPerfStats->totalCQICW2Reported[cqi];
		    }
		    for( rank=0; rank < MAX_RANK_INDICATION; rank++ )
		    {
			ueMacPerfStatsResp_p->cqiRIPerfStats.totalRankReported[rank] =
			    pCqiRIPerfStats->totalRankReported[rank];
		    }
		    break;

		case PERF_STATS_HARQ:
		    for( modScheme = 0; modScheme < MOD_SCHEME; modScheme++ )
		    {
			ueMacPerfStatsResp_p->harqPerfStats.totalDLHarqAck[modScheme] =
			    pHarqPerfStats->totalDLHarqAck[modScheme];
 
			ueMacPerfStatsResp_p->harqPerfStats.totalDLHarqNack[modScheme] =
			    pHarqPerfStats->totalDLHarqNack[modScheme]; 

			ueMacPerfStatsResp_p->harqPerfStats.totalULHarqSucc[modScheme] =
			    pHarqPerfStats->totalULHarqSucc[modScheme] ;

			ueMacPerfStatsResp_p->harqPerfStats.totalULHarqFail[modScheme] = 
			    pHarqPerfStats->totalULHarqFail[modScheme];
		    }
		    ueMacPerfStatsResp_p->harqPerfStats.totalHarqFail = 
			pHarqPerfStats->totalHarqFail;
		    break;

		case PERF_STATS_UE_CAPABILITY:
		    ueMacPerfStatsResp_p->ueCapabilityStats.totalTimeRestrictUeInDlbyUeCap =
			pUeCapabilityStats->totalTimeRestrictUeInDlbyUeCap;

		    ueMacPerfStatsResp_p->ueCapabilityStats.totalTimeRestrictUeInUlbyUeCap = 
			pUeCapabilityStats->totalTimeRestrictUeInUlbyUeCap;
		    break;

		case PERF_STATS_POWER_LIMITED:
		    ueMacPerfStatsResp_p->uePowerLimitedStats.totalTbScheduledInUlOnPowerRestriction =
			pUePowerLimitedStats->totalTbScheduledInUlOnPowerRestriction;

		    ueMacPerfStatsResp_p->uePowerLimitedStats.totalTbScheduledInUlOnNoPowerRestriction = 
			pUePowerLimitedStats->totalTbScheduledInUlOnNoPowerRestriction;
		    break;

		case PERF_STATS_TX_MODE_RI:
		    ueMacPerfStatsResp_p->ueTransmissionModeRIStats.totalDLTransmissionsWithTX2RI1 = 
			pUeTransmissionModeRIStats->totalDLTransmissionsWithTX2RI1;

		    for( rankIndex = 0; rankIndex < MAX_RANK_INDICATION; rankIndex++ )
		    {
			ueMacPerfStatsResp_p->ueTransmissionModeRIStats.totalDLTransmissionsWithTX3RI[rankIndex] = 
			    pUeTransmissionModeRIStats->totalDLTransmissionsWithTX3RI[rankIndex];

			ueMacPerfStatsResp_p->ueTransmissionModeRIStats.totalDLTransmissionsWithTX4RI[rankIndex] =
			    pUeTransmissionModeRIStats->totalDLTransmissionsWithTX4RI[rankIndex] ;
		    }

		    break;

		case PERF_STATS_PUCCH_PUSCH_SINR:
		    for( sinrIndex = 0; sinrIndex< MAX_SINR_DIFF_INDEX; sinrIndex++ )
		    {
			ueMacPerfStatsResp_p->ueSINRPerfStats.totalPUCCHSINRReported[sinrIndex] =
			    pUeSINRPerfStats->totalPUCCHSINRReported[sinrIndex];
 
			ueMacPerfStatsResp_p->ueSINRPerfStats.totalPUSCHSINRReported[sinrIndex] = 
			    pUeSINRPerfStats->totalPUSCHSINRReported[sinrIndex];
		    }
		    break;

		case PERF_STATS_DATA_VOLUME:
		    ueMacPerfStatsResp_p->radioThpVolStats.totalUlByteReceived = 
			pRadioThpVolStats->totalUlByteReceived;

		    ueMacPerfStatsResp_p->radioThpVolStats.totalDlBytesSent = 
			pRadioThpVolStats->totalDlBytesSent;

		    ueMacPerfStatsResp_p->radioThpVolStats.totalUlThpPerUeTrunk = 
			pRadioThpVolStats->totalUlThpPerUeTrunk;
		    break;

		case PERF_STATS_UE_PRB_USAGE:
		    ueMacPerfStatsResp_p->uePRBUsageStats.totalDLREsUsage =
			pUePRBUsageStats->totalDLREsUsage;

		    ueMacPerfStatsResp_p->uePRBUsageStats.totalREsUsedInUL = 
			pUePRBUsageStats->totalREsUsedInUL;
		    break;

		default :
		    break;
	    }
	    localKpiBitmap = localKpiBitmap & (~(1<<(firstBitSet - 1)));
        /* SPR 7385 Changes Starts */
	    firstBitSet = FFS(localKpiBitmap);
        /* SPR 7385 Changes Ends */
	}
	ueCount++;
	length +=sizeof(UeMacPerfStatsResp);
    }
    if(periodicFlag)
    {
	/* The reason for increasing the value of length_p by 8 is that in case of periodic reporting
	 * in conf structure we only duration and no_of_UEs fiels of UInt32 type except the response struct
	 * so we need to increase the size by 8 bytes */
	*length_p = length + 8;
	/* Number of UEs sent in current batch*/
	macUePerfStatsIndParams_p->numOfUePresent = ueCount;
    }
    else
    {
	/* The reason for increasing the value of length_p by 8 is that in case of non periodic reporting
	 * in Indcation structure we have duration and no_of_UEs and response codefiels of UInt32 type 
	 * except the response struct so we need to increase the size by 12 bytes */
	*length_p = length + 12;
	/* Number of UEs sent in current batch*/
	macGetUePerfStatsCnfParams_p->numOfUePresent = ueCount;
    }
}

/****************************************************************************
 *  Function Name  : cellMacFillPerfStats
 *  Inputs         : cnfBuff_p - Pointer to confirmation buffer,
 *                   perfBitmap - Perf Stats to be filled in confirmation buffer,
 *                   periodicFlag,
 *                   internalCellIndex - cell id  
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This API will fill cell stats in the confirmation buffer for OAM.
 *******************************************************************************/
/* + PERF_CA */
void cellMacFillPerfStats(UInt8 *cnfBuff_p, UInt32 perfBitmap, 
			UInt32 periodicFlag, InternalCellIndex internalCellIndex)
{
    UInt32 qci = 0;
    UInt32 range = 0;
    UInt32 srb = 0;
    UInt32 utilIndex = 0;
    UInt32 cfi = 0;
    UInt32 localKpiBitmap = 0;
    UInt8 firstBitSet = 0;

    CellMacPerfStatsResp  *cellMacPerfStatsResp_p = PNULL;
    CellMacPerfStatsResp  *pLteMacCellPerfStats   = PNULL;
    CellHarqPerfStats     *pCellHarqPerfStats     = PNULL;
    RachSuccFailPerfStats *pRachSuccFailPerfStats = PNULL;
    PdcchUtilizationStats *pPdcchUtilizationStats = PNULL;
    CellPRBUsagePerfStats *pCellPRBUsagePerfStats = PNULL;

    pLteMacCellPerfStats   = &(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].\
                               perfStats.lteMacPerfStats.lteMacCellPerfStats[internalCellIndex]);
    pCellHarqPerfStats     = &(pLteMacCellPerfStats->cellHarqPerfStats);
    pRachSuccFailPerfStats = &(pLteMacCellPerfStats->rachSuccFailPerfStats);
    pPdcchUtilizationStats = &(pLteMacCellPerfStats->pdcchUtilizationStats);
    pCellPRBUsagePerfStats = &(pLteMacCellPerfStats->cellPRBUsagePerfStats);

    MacCellPerfStatsIndParams *macCellPerfStatsIndParams_p = PNULL;
    MacGetCellPerfStatsCnfParams *macGetCellPerfStatsCnfParams_p = PNULL;
    localKpiBitmap = perfBitmap;
    /*Set the pointer required cnt/Ind acc. to periodic flag*/
    /* Set the pointer required cnt/Ind acc. to periodic flag*/
    if(periodicFlag)
    {
        macCellPerfStatsIndParams_p = (MacCellPerfStatsIndParams *)&cnfBuff_p[MAC_API_HEADER_LEN];
        macCellPerfStatsIndParams_p->cellMacPerfStatsResp.durationTTI = 
             gPerfStats.perfStats->macLastCalculatedTTI;
        cellMacPerfStatsResp_p = (CellMacPerfStatsResp*)(&macCellPerfStatsIndParams_p->cellMacPerfStatsResp);
    }
    else
    {
        macGetCellPerfStatsCnfParams_p = (MacGetCellPerfStatsCnfParams *)&cnfBuff_p[MAC_API_HEADER_LEN];
        macGetCellPerfStatsCnfParams_p->cellMacPerfStatsResp.durationTTI =  \
                                                                            gPerfStats.perfStats->macLastCalculatedTTI;
        cellMacPerfStatsResp_p = (CellMacPerfStatsResp*)(&macGetCellPerfStatsCnfParams_p->cellMacPerfStatsResp);
    }
    /* SPR 7385 Changes Starts */
    firstBitSet = FFS( localKpiBitmap );
    /* SPR 7385 Changes Ends */
    /* Fills the response buffer according to the requested bitmap*/
    while (firstBitSet)
    {
	switch (firstBitSet - 1)
	{
	    case PERF_STATS_PRB_USAGE_HARQ:
		for (qci = 0; qci < MAX_QCI; qci++)
		{
		    cellMacPerfStatsResp_p->cellHarqPerfStats.totalDLPRBUsageforHarq[qci] = 
			pCellHarqPerfStats->totalDLPRBUsageforHarq[qci];
		}
		cellMacPerfStatsResp_p->cellHarqPerfStats.totalDLPRBUsageforHarqRetx =
		    pCellHarqPerfStats->totalDLPRBUsageforHarqRetx;
	    break;

	    case PERF_STATS_RACH_SUCC_FAIL:
		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalRcvdMsg3RandomAccessPreamblesCBRA = 
		    pRachSuccFailPerfStats->totalRcvdMsg3RandomAccessPreamblesCBRA;

		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalRcvdMsg3RandomAccessPreamblesCFRA = 
		    pRachSuccFailPerfStats->totalRcvdMsg3RandomAccessPreamblesCFRA;

		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalFailedRandomAccessPreamblesMsg2CBRA = 
		    pRachSuccFailPerfStats->totalFailedRandomAccessPreamblesMsg2CBRA;

		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalFailedRandomAccessPreamblesMsg2CFRA = 
		    pRachSuccFailPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA;

		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalAllocatedRandomAccessPreambles = 
		    pRachSuccFailPerfStats->totalAllocatedRandomAccessPreambles;

		for (range = 0; range < MAX_TA_RANGE; range++)
		{
		    cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalSuccRandomAccessTA[range] = 
			pRachSuccFailPerfStats->totalSuccRandomAccessTA[range];
		}
		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalUnassignedFalseCFRA = 
		    pRachSuccFailPerfStats->totalUnassignedFalseCFRA;

		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalUnassignedSumCFRA = 
		    pRachSuccFailPerfStats->totalUnassignedSumCFRA;
	    break;

	    case PERF_STATS_PDCCH_UTILIZATION:
		for (utilIndex = 0; utilIndex < MAX_CCE_UTIL_INDEX; utilIndex++)
		{
		    cellMacPerfStatsResp_p->pdcchUtilizationStats.totalPdcchCceUtil[utilIndex] = 
			pPdcchUtilizationStats->totalPdcchCceUtil[utilIndex];
		}
		for (cfi = 0; cfi < MAX_CFI; cfi++)
		{
		    cellMacPerfStatsResp_p->pdcchUtilizationStats.noOfSubFrameWithPdcchCfi[cfi] = 
			pPdcchUtilizationStats->noOfSubFrameWithPdcchCfi[cfi];
		}
		cellMacPerfStatsResp_p->pdcchUtilizationStats.noOfSubFrameWithCceUtilMoreThanZero = 
		    pPdcchUtilizationStats->noOfSubFrameWithCceUtilMoreThanZero;

		cellMacPerfStatsResp_p->pdcchUtilizationStats.totalCceAvail = 
		    pPdcchUtilizationStats->totalCceAvail;
	    break;

	    case PERF_STATS_PUCCH_UTILIZATION:
		cellMacPerfStatsResp_p->totalPUCCHResAllocationAttempts = 
		    pLteMacCellPerfStats->totalPUCCHResAllocationAttempts;
	    break;

	    case PERF_STATS_CELL_PRB_USAGE:

	    /* + SPR_5711 */
               cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalULPRBUsageSRB[0] = 
		pCellPRBUsagePerfStats->totalULPRBUsageSRB[0];

	       cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalDLPRBUsageSRB[0] = \
				  pCellPRBUsagePerfStats->totalDLPRBUsageSRB[0];
             for(srb=1; srb<MAX_SRB; srb++)
	     {
	          cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalULPRBUsageSRB[srb] = \
		   pCellPRBUsagePerfStats->totalULPRBUsageSRB[srb] >> SCALE_UP_MULTIPLYING_FACTOR;
		  cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalDLPRBUsageSRB[srb] = \
		     pCellPRBUsagePerfStats->totalDLPRBUsageSRB[srb] >> SCALE_UP_MULTIPLYING_FACTOR;
       	    /* - SPR_5711 */
	     }
	     cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalDLPRBUsagePCCH =
		pCellPRBUsagePerfStats->totalDLPRBUsagePCCH;

	     cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalDLPRBUsageBCCH = 
		    pCellPRBUsagePerfStats->totalDLPRBUsageBCCH;
                  
             cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalDLPRBAvailable  = \
                                            pCellPRBUsagePerfStats->totalDLPRBAvailable;
             cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalULPRBAvailable  = \
                                              pCellPRBUsagePerfStats->totalULPRBAvailable;
	    break;

	    default :
		break;
	}
	localKpiBitmap = localKpiBitmap & (~(1<<(firstBitSet - 1)));
    /* SPR 7385 Changes Starts */
	firstBitSet = FFS(localKpiBitmap);
    /* SPR 7385 Changes Ends */
    }
}
/* - PERF_CA */


/****************************************************************************
 *  Function Name  : macProcessPerfCounter
 *  Inputs         : None
 *  Outputs        : None
 *  Returns        : None
 *  Description    : Process Performance Counters in every tick.
 *****************************************************************************/
/* + PERF_CA */
/* + SPR 17439 */
void macProcessPerfCounter(void)
/* - SPR 17439 */    
{
    InternalCellIndex cellIndex = 0;
    LtePerfStatsConfig *lteMacPerfStatsConfig_p =  &(g_perfStatsScannerObj.\
            perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].perfConfig.lteMacPerfStatsConfig);
    UInt32 perfStatsUeIndex = 0;
    UInt32 current = 0;
    /* SPR 10577 changes start */
    MacUePerfStatsIndParams *macUePerfStatsIndParams_p = PNULL;
    /* SPR 10577 changes end */
    UInt8* cnfBuff_p = PNULL;

    if(lteMacPerfStatsConfig_p->periodicReporting)
    {   
        if (lteMacPerfStatsConfig_p->cellPerfStatsToReportBitMap)  
        {
            /* + PERF_CA */
            for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
            {
                cnfBuff_p = (UInt8 *)getMemFromPool(MAX_OAM_PERF_STATS_SIZE,NULL);

                if(PNULL == cnfBuff_p)
                {
                    lteWarning("In %s, MEMORY ALLOC FAILURE\n",__func__);
                    return;
                } 
                cellMacFillPerfStats( cnfBuff_p
                        , lteMacPerfStatsConfig_p->cellPerfStatsToReportBitMap,
                        SET_PERIODIC_FLAG, cellIndex );
                current = MAC_API_HEADER_LEN + sizeof(MacCellPerfStatsIndParams);
                /* Fill header information */
                prepareMacOamApiHeader(cnfBuff_p
                        , MAC_CELL_PERF_STATS_IND,
                        lteMacPerfStatsConfig_p->transactionId, current,
                        macCommonGetRRCCellIndex(cellIndex));
                if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff_p, current
                    ))
                {
                    ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
                }
                freeMemPool(cnfBuff_p);
            }
		/* - PERF_CA */
        }

        if (lteMacPerfStatsConfig_p->uePerfStatsToReportBitMap)
        {
            cnfBuff_p = (UInt8 *)getMemFromPool(MAX_OAM_PERF_STATS_SIZE,NULL);

            if(PNULL == cnfBuff_p)
            {
                lteWarning("In %s, MEMORY ALLOC FAILURE\n",__func__);
                return;
            } 
            current = 0;
            for (perfStatsUeIndex = 0; perfStatsUeIndex < PERF_MAX_UE_SUPPORTED;)
            {
		/* + stats_review_comments_ipc */
                ueMacFillPerfStats(cnfBuff_p
                        , &current, 
                        lteMacPerfStatsConfig_p->uePerfStatsToReportBitMap, 
                        &perfStatsUeIndex,SET_PERIODIC_FLAG, API_HEADER_DO_NOT_CARE_CELL_INDEX);
		/* - stats_review_comments_ipc */
		/* SPR 10577 changes start */
                macUePerfStatsIndParams_p =          
                    (MacUePerfStatsIndParams*)&cnfBuff_p[MAC_API_HEADER_LEN];
                macUePerfStatsIndParams_p->endIndicator = FALSE;
                if((perfStatsUeIndex >= PERF_MAX_UE_SUPPORTED) ||
                        (macUePerfStatsIndParams_p->numOfUePresent >= MAX_UE_PER_BATCH))
                {
                    /* endIndicator is set TRUE, as this is the end of the message */
                    macUePerfStatsIndParams_p->endIndicator = TRUE;
                }
                /* SPR 10577 changes end */

                /* Fill header information */
                prepareMacOamApiHeader(cnfBuff_p
                        , MAC_UE_PERF_STATS_IND,
                        lteMacPerfStatsConfig_p->transactionId, current,
                        macCommonGetRRCCellIndex(API_HEADER_DO_NOT_CARE_CELL_INDEX));
                current += MAC_API_HEADER_LEN;
                if(MAC_SEND_ERROR == sendMsgToOAM( cnfBuff_p, current
                            ))
                {
                    ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
                }

                current = 0;
            }
            freeMemPool(cnfBuff_p);
        }
    }

    resetMacPerfContainer(PERF_STATS_CONTAINER_OAM_MAC_RESET_CELL_CONTAINER);
    resetMacPerfContainer(PERF_STATS_CONTAINER_OAM_MAC_RESET_UE_CONTAINER);

    g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].
        perfStats.lteMacPerfStats.lastMACCalculatedTTI = 0;

}
/* - PERF_CA */
/****************************************************************************
 * Function Name  : handleMACConfigureOAMPerfStatsReq 
 * Inputs         : msgBuf_p - poiter to buffer recevied in request,
 *                  transactionId,
 *                  moduleSrc - Received Module ID
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will handle Perf Stats Config Req from OAM.
 ******************************************************************************/
/* + coverity 52872 */
/* CA Changes start */
/* +- SPR 17777 */
MacRetType handleMACConfigureOAMPerfStatsReq(UInt8 *msgBuf_p,  UInt16 transactionId,
                                     InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
/* CA Changes end */
{
    MacRetType respCode    = MAC_SUCCESS;
/* - coverity 52872 */
	/* + PERF_CA */
	InternalCellIndex cellIndex = 0;
	/* - PERF_CA */
    UInt32 loModuleSrc = PERF_STATS_CONTAINER_OAM_MAC;
    /* SPR 15909 fix start */
    tickType_t macLastCalculatedTTI = 0;
    /* SPR 15909 fix end */
    UInt8 passiveIndexForMacStats = gPerfStats.perfStatsControlInfo->macActiveIndex; 




    MacConfigurePerfStatsReqParams *macConfigurePerfStatsReqParams_p =
	(MacConfigurePerfStatsReqParams *)(msgBuf_p + MAC_API_HEADER_LEN);
    LtePerfStatsConfig  *pOamMacPerfStatsConfig = &(g_perfStatsScannerObj.\
	    perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].perfConfig.lteMacPerfStatsConfig);

    if(!(g_perfStatsScannerObj.module_bitmask))
    {
        g_perfStatsScannerObj.prevPerfStats_tick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);

    }

    if ( (macConfigurePerfStatsReqParams_p->duration > MAC_MAX_VAL_PERF_TIMER_T ) ||
            (macConfigurePerfStatsReqParams_p->duration < MAC_MIN_VAL_PERF_TIMER_T ))
    {
        /* + coverity 52872 */
        respCode = MAC_SYNTAX_ERROR_VAL;
        /* - coverity 52872 */
    }
    else
    {
	if ((macConfigurePerfStatsReqParams_p->UePerfStatsToReportBitMap)
		|| (macConfigurePerfStatsReqParams_p->CellPerfStatsToReportBitMap))
	{
           
	    g_perfStatsScannerObj.module_bitmask |= PERF_STATS_CONTAINER_OAM_MAC_BITMASK;
	    pOamMacPerfStatsConfig->duration = (macConfigurePerfStatsReqParams_p->duration) * 1000;
	    g_perfStatsScannerObj.timeoutTTI[loModuleSrc] = pOamMacPerfStatsConfig->duration;
	    pOamMacPerfStatsConfig->periodicReporting = macConfigurePerfStatsReqParams_p->periodicReporting;
	    pOamMacPerfStatsConfig->uePerfStatsToReportBitMap = macConfigurePerfStatsReqParams_p->UePerfStatsToReportBitMap;
	    pOamMacPerfStatsConfig->cellPerfStatsToReportBitMap = macConfigurePerfStatsReqParams_p->CellPerfStatsToReportBitMap;
	    pOamMacPerfStatsConfig->transactionId = transactionId;
            g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].macOrPdcpOrRlcStats = MAC_PERF_STATS;

/* Perf stats RRM-MAC start */
	    macLastCalculatedTTI = gPerfStats.perfStats[passiveIndexForMacStats].macLastCalculatedTTI;
	    if (macLastCalculatedTTI)
	    {
		    /* Switching index */
		    gPerfStats.perfStatsControlInfo->macActiveIndex =  passiveIndexForMacStats ^0x01;

		    /* Switching counter position from passive to active */
			/* + PERF_CA */
			for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
		    {
				gMacCellPerfStats_p[cellIndex] = 
			    	&gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->macActiveIndex].lteCellPerfStats[internalCellIndex];
			}
			/* - PERF_CA */
		    gMacUePerfStats_p = 
			    gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->macActiveIndex].lteUePerfStats;
	    }

	    /*Update RRM-MAC container */
	    if(g_perfStatsScannerObj.module_bitmask & PERF_STATS_CONTAINER_RRM_MAC_BITMASK)
	    {
		    if(MAC_PERF_STATS == g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_RRM_MAC].macOrPdcpOrRlcStats)
		    {
			    updateMacPerfContainer(PERF_STATS_CONTAINER_RRM_MAC);
		    }
	    }


/* Perf stats RRM-MAC end */

	    /*reset shm stats */
		/* + PERF_CA */
		for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
	    {
	    	macResetCellPerfStatCounters(&gPerfStats.perfStats[passiveIndexForMacStats].lteCellPerfStats[cellIndex]);
		}
		/* - PERF_CA */
	    macResetUePerfStatCounters(gPerfStats.perfStats[passiveIndexForMacStats].lteUePerfStats);
	    gPerfStats.perfStats[passiveIndexForMacStats].macLastCalculatedTTI = 0;
	    
            /*reset container stats */
	    resetMacPerfContainer(PERF_STATS_CONTAINER_OAM_MAC_RESET_UE_CONTAINER);
	    resetMacPerfContainer(PERF_STATS_CONTAINER_OAM_MAC_RESET_CELL_CONTAINER);
	    g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].
		perfStats.lteMacPerfStats.lastMACCalculatedTTI = 0;
	}
        else
        {
             g_perfStatsScannerObj.module_bitmask &= ~(PERF_STATS_CONTAINER_OAM_MAC_BITMASK);
        }

    }
    return respCode;
}

#endif

/* HSM_CHG_START */
/****************************************************************************
 *  Function Name  : handleMACHealthMonitorStatusReq
 *  Inputs         : None
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS/MAC_FAILURE
 *  Description    : This function will handle Health status monitor Req from OAM.
 *******************************************************************************/
/* + SPR 17439 */
MacRetType handleMACHealthMonitorStatusReq(void)
/* - SPR 17439 */    
{
    MacRetType respCode  = MAC_FAILURE;
    UInt8  threadIndx = 0;
    /* If cell is not running, only MAC Main thread will be checked for supervision */
    UInt8 threadCnt = 1;
    /* CA Changes start */
    InternalCellIndex cellIndex = 0;

	UInt8 num_of_cells =  cellSpecificParams_g.numOfCells;
    /* Check the thread status of each configured cell */
    for (cellIndex = 0; cellIndex < num_of_cells; cellIndex++)
    {
        /* Check if the corresponding thread is running or not by checking the cell state */

        if ((CELL_PHY_RUNNING == cellSpecificParams_g.\
                    cellConfigAndInitParams_p[cellIndex]->cellState) ||
                (CELL_PHY_RUNNING_CONFIG_RECVD == cellSpecificParams_g.\
                 cellConfigAndInitParams_p[cellIndex]->cellState))
        {
            /* Check for all threads as cell is in running state */
            threadCnt = MAC_MAX_THREAD_ID;
        }
        else
        {
            /** SPR 12637 Fix : Start **/  
            respCode = MAC_SUCCESS;
            continue;
            /** SPR 12637 Fix : End **/  
        }

        for (threadIndx = 0; threadIndx < threadCnt; threadIndx++)
        {
            if (macTTITickCount_g - 
                    l2ThreadHealthStoreTick_g.rlcMacThread[cellIndex].macThread[threadIndx] <
                    macParams_g.macHealthMonitoringTimeInterval)
                /* CA Changes end */
            {
                respCode = MAC_SUCCESS;
            }
            else
            {
                LOG_MAC_MSG(HSM_MAC_OAM_REQ_FAILURE, LOGFATAL, L2_SYS_FAIL,
                        macTTITickCount_g,threadIndx,
                        DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        __func__,"MAC OAM Health Status Failure");
                return MAC_FAILURE;
            }
        }
    }
    return respCode;
}
/* HSM_CHG_END */

/****************************************************************************
 * Function Name  : macGetKpi 
 * Inputs         : cnfBuff_p - Pointer to confirmation buffer
 *                  current_p - Pointer to the current idx of cnfBuff_p.
 *                  internalCellIndex - cell id
 * Outputs        : None
 * Returns        : None
 * Description    : This API will fill the structure for MAC_GET_KPI_CNF API.
 ****************************************************************************/
/* CA Changes start */
void macGetKpi(UInt8 *cnfBuff, UInt16 *current_p, InternalCellIndex internalCellIndex)
{
    MacKpiResp *macKpiResp_p = (MacKpiResp *)(cnfBuff + MAC_API_HEADER_LEN);
    /* CA Changes start */
    /* As of now cell index 0 state is checked.
     * This will be changed after CA KPI design.
     */ 
     internalCellIndex = 0;
    if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
         cellState <= CELL_PHY_INIT)
    {  
        macKpiResp_p->responseCode = MAC_LAYER_NOT_INITIALIZED;
        return;
     }

    macKpiResp_p->responseCode = MAC_SUCCESS;
    macKpiResp_p->numOfActiveUE = cellSpecificParams_g.\
          cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->totalActiveUE;
    /* CA Changes end */
    macKpiResp_p->numOfRachInd = numOfrachInd_g[internalCellIndex];

    *current_p +=sizeof(MacKpiResp);
    return;
}

/****************************************************************************
 * Function Name  : parseSetLogLevel
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket 
 * Outputs        : None
 * Returns        : None
 * Description    : This function will parse the log level recevied from OAM
 *                  and LogLevel will be enabled for the entire L2
 *                  and not just MAC.
 ****************************************************************************/
void parseSetLogLevel(UInt8 *msgBuf_p)
{
    MacLogLevel_Req *macLogLevel_Req_p = (MacLogLevel_Req *)
        (msgBuf_p + MAC_API_HEADER_LEN);
    enable_log_level(macLogLevel_Req_p->logLevel);
}

/****************************************************************************
 * Function Name  : cleanupAllMemoryToPool
 * Inputs         : internalCellIndex - CellId
 * Outputs        : None
 * Returns        : None
 * Description    : This API will cleanup all dynamic memory allocated by 
 *                  MAC to memory pool. 
 *                  This is called at MAC_CLEANUP_LAYER_REQ from OAM
 ****************************************************************************/
/* CA Changes start */
void cleanupAllMemoryToPool(InternalCellIndex internalCellIndex)
{
    /* SPR 20445 Fix Start */
    UInt8 SFIndex = 0;
    /* SPR 20445 Fix End */
    cleanupDLSchedulerStrategy(internalCellIndex);
    /* ++coverity--530-54624,25*/
    cleanAndDeInitDRXQueue(internalCellIndex);
    /* ++coverity--530-54624,25*/
    /* Stage-III SCELL Deactivation Timer Related Changes Start */
    scellStateMngrCleanUpScellDeactTimer(internalCellIndex);
    /* Stage-III SCELL Deactivation Timer Related Changes End */

    cleanupPDCCH(internalCellIndex);

    cleanupULSchedulerStrategy(internalCellIndex);

    freeMemPool(ulGrantContentionResQueue_gp[internalCellIndex]);
    ulGrantContentionResQueue_gp[internalCellIndex]= PNULL;
    freeMemPool(ZeroNonZeroBSRTriggerQueue_gp[internalCellIndex]);
    ZeroNonZeroBSRTriggerQueue_gp[internalCellIndex]= PNULL;
    freeMemPool(nonZeroBsrList_gp[internalCellIndex]);
    nonZeroBsrList_gp[internalCellIndex]= PNULL;
    freeMemPool(ulCellEdgeUserPriorityQueue_gp[internalCellIndex]);
    ulCellEdgeUserPriorityQueue_gp[internalCellIndex]= PNULL;
    freeMemPool(ulScheduleInfo_gp[internalCellIndex]);
    ulScheduleInfo_gp[internalCellIndex] = PNULL;

    /* ++coverity--530-54652*/
    cleanAndDeInitECidQueue(internalCellIndex);
    /* ++coverity--530-54652*/

    /* clean up function to clean up the memory resource of PUCCH manager */
    cleanupPUCCH(internalCellIndex);

    cleanUpMUXQueue(internalCellIndex);

    cleanupEncoderModule(internalCellIndex);

    /* clean up function to clean up the memory resource of PHY */
    cleanupPHYInterface(internalCellIndex);
    /* clean up function to clean up the memory resource of BCCH manager */
    cleanupBCCHMgr(internalCellIndex);
    /* clean up function to clean up the memory resource of PCCH manager */
    cleanupPCCHMgr(internalCellIndex);
    /* clean up function to clean up the memory resource of CCCH manager */
    cleanupCCCHMgr(internalCellIndex);
    /* clean up function to clean up the memory resource of Rach manager */
    cleanupRACHMgr(internalCellIndex);
    /* clean up function to clean up the memory resource of Demux manager */
    cleanupDeMuxMgr(NUM_OF_DEMUX_THREADS,internalCellIndex);

    freeMemPool(deMuxThreadPool_gp[internalCellIndex]);
    deMuxThreadPool_gp[internalCellIndex] = PNULL;
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
    freeMemPool(demuxProcessMap_gp[internalCellIndex]);
    demuxProcessMap_gp[internalCellIndex] = PNULL;
#endif
    /* clean up function to clean up the memory resource of uplink queue load  manager */
    cleanupUEConfMgr(internalCellIndex);
    freeMemPool(pendingUEDeleteQueue_gp[internalCellIndex]);
    pendingUEDeleteQueue_gp[internalCellIndex] = PNULL;

    /*cleanup memory allocated for SRS cell and UE maps*/
    /* Coverity 54650 */
    freeMemPool(macUeOpporIndCircQ_gp[internalCellIndex]);
    macUeOpporIndCircQ_gp[internalCellIndex] = PNULL;

    /* Coverity  54650 */
    /* + SPR_10825 */ 
    cleanupSRSMgr(internalCellIndex);
    /* - SPR_10825 */ 
    /*Coverity Fix*/
    freePendingMemoryPools(internalCellIndex);
    circQDeInit(pendingDLMemoryPoolFreeQ_gp[internalCellIndex]);
    /*Coverity-5.3.0 CID-54598, 54599, 54600*/
    deInitRACHMgr(internalCellIndex);

    /*Coverity-5.3.0 CID-54613*/
    cleanupULCSIManager(internalCellIndex); 
    freeMemPool(sinrReportQueue_gp[internalCellIndex]);   
    sinrReportQueue_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54632*/
    /* SPR 8702 changes start */
    deInitDLSchedulerStrategyQueues(internalCellIndex);
    cleanupMacRRMInterface(internalCellIndex);
    deInitMacRRMInterface(internalCellIndex);
    deInitUEConfMgr(internalCellIndex);
    /* SPR 8702 changes end*/
    QDEINIT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_FAILURE_Q(internalCellIndex);
    freeMemPool(explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex]);   
    explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54633*/
    QDEINIT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(internalCellIndex);
    freeMemPool(explicitULPuschTpcScheduleQueue_gp[internalCellIndex]);   
    explicitULPuschTpcScheduleQueue_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54634*/
    QDEINIT_UL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex);
    freeMemPool(ulDRXOnOffTriggerQueue_gp[internalCellIndex]);   
    ulDRXOnOffTriggerQueue_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54635*/
    QDEINIT_UL_GRANT_APERIODIC_CQI_Q(internalCellIndex);
    freeMemPool(ulGrantAperiodicCqiQueue_gp[internalCellIndex]);   
    ulGrantAperiodicCqiQueue_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54636*/
    QDEINIT_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPUSCHFailureQueue_gp[internalCellIndex]);
    freeMemPool(ulGrantPUSCHFailureQueue_gp[internalCellIndex]);   
    ulGrantPUSCHFailureQueue_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54637*/
    QDEINIT_UL_GRANT_PUSCH_QUEUE_Q(ulGrantPendingQueue_gp[internalCellIndex]);
    freeMemPool(ulGrantPendingQueue_gp[internalCellIndex]);   
    ulGrantPendingQueue_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54638*/
    QDEINIT_UL_MEAS_GAPON_OFF_TRIGGER_Q(internalCellIndex);
    freeMemPool(ulMeasGapOnOffTriggerQueue_gp[internalCellIndex]);   
    ulMeasGapOnOffTriggerQueue_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54639*/
    QDEINIT_MAC_UL_SPS_Q(ulSpsActQ_gp[internalCellIndex]);
    freeMemPool(ulSpsActQ_gp[internalCellIndex]);   
    ulSpsActQ_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54640*/
    QDEINIT_MAC_UL_SPS_Q(ulSpsDeactQ_gp[internalCellIndex]);
    freeMemPool(ulSpsDeactQ_gp[internalCellIndex]);   
    ulSpsDeactQ_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54641*/
    QDEINIT_MAC_UL_SPS_Q(ulSpsReactQ_gp[internalCellIndex]);
    freeMemPool(ulSpsReactQ_gp[internalCellIndex]);   
    ulSpsReactQ_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54642*/
    QDEINIT_MAC_UL_SPS_Q(ulSpsResRelQ_gp[internalCellIndex]);
    freeMemPool(ulSpsResRelQ_gp[internalCellIndex]);   
    ulSpsResRelQ_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54643*/
    QDEINIT_MAC_UL_SPS_Q(ulSpsResRsvQ_gp[internalCellIndex]);
    freeMemPool(ulSpsResRsvQ_gp[internalCellIndex]);   
    ulSpsResRsvQ_gp[internalCellIndex] = PNULL;
    /*Coverity-5.3.0 CID-54644*/
    QDEINIT_MAC_UL_SPS_Q(ulSpsStatusQ_gp[internalCellIndex]);
    freeMemPool(ulSpsStatusQ_gp[internalCellIndex]);   
    ulSpsStatusQ_gp[internalCellIndex] = PNULL;

    cleanupUlStrategyQueue(internalCellIndex);
#if !defined DL_UL_SPLIT && defined CIRC_QUEUE_IMPL    
    /* CA TDD commented for Compilation Start*/
    //    freeMemPool(ulStrategyTxCircQueue_gp[internalCellIndex]);
    //    ulStrategyTxCircQueue_gp[internalCellIndex] = PNULL;
    /* CA TDD commented for Compilation End*/
#endif

    /*CA changes start: freeing the globals those were given the memory at run time*/
    freeMemPool(measGap40msInfo_gp[internalCellIndex]);
    measGap40msInfo_gp[internalCellIndex] = PNULL;
    freeMemPool(measGap80msInfo_gp[internalCellIndex]);
    measGap80msInfo_gp[internalCellIndex] = PNULL;
#ifdef ENDC_ENABLED
	/*Meas_Gap_Changes_Start*/
    freeMemPool(measGap20msInfo_gp[internalCellIndex]);
    measGap20msInfo_gp[internalCellIndex] = PNULL;
    freeMemPool(measGap160msInfo_gp[internalCellIndex]);
    measGap160msInfo_gp[internalCellIndex] = PNULL;
    /*Meas_Gap_Changes_End*/
#endif
    /* clean up function to clean up the memory resource of uplink resource
     * allocator */
    deInitULResourceInfo (internalCellIndex);

    cleanupTpcRntiContextForPusch(internalCellIndex);
    deInitTpcRntiContextForPusch(internalCellIndex);

    freeMemPool(shortPeriodicSRSMapInfo_gp[internalCellIndex]);
    shortPeriodicSRSMapInfo_gp[internalCellIndex] = PNULL;
    freeMemPool(longPeriodicSRSMapInfo_gp[internalCellIndex]);
    longPeriodicSRSMapInfo_gp[internalCellIndex] = PNULL;
    freeMemPool(buddyPairInfo_gp[internalCellIndex]);
    buddyPairInfo_gp[internalCellIndex] = PNULL;

    /* +- SPR 17777 */
    deInitQosToken(internalCellIndex);

    deInitPHYInterface(internalCellIndex);
    if (PNULL != dlSchedulerReportQueue_gp[internalCellIndex])
    {
        freeMemPool(dlSchedulerReportQueue_gp[internalCellIndex]);
        dlSchedulerReportQueue_gp[internalCellIndex] = PNULL;
    }
    deInitTimingAdvanceMgr (internalCellIndex);

    freeMemPool(cellConfigPhy_gp[internalCellIndex]);
    cellConfigPhy_gp[internalCellIndex] = PNULL;
    freeMemPool(ulPfsGlobals_gp[internalCellIndex]);
    ulPfsGlobals_gp[internalCellIndex] = PNULL;
    /* + SPR 11209 Changes */
    freeMemPool(encoderContainerArr_gp[internalCellIndex][internalCellIndex]);
    encoderContainerArr_gp[internalCellIndex][internalCellIndex] = PNULL;
    freeMemPool(encoderContainerArr_gp[internalCellIndex][!(internalCellIndex)]);
    encoderContainerArr_gp[internalCellIndex][!(internalCellIndex)] = PNULL;
    /* - SPR 11209 Changes */
    freeMemPool(ulGrantGlobalSrQueue_gp[internalCellIndex]);
    ulGrantGlobalSrQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(drxExpiryTimerList_gp[internalCellIndex]);
    drxExpiryTimerList_gp[internalCellIndex] = PNULL;
    freeMemPool(dlRetransmissionFailureQueue_gp[internalCellIndex]);
    dlRetransmissionFailureQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(dlRetransmissionQueue_gp[internalCellIndex]);
    dlRetransmissionQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(pendingCcchQueue_gp[internalCellIndex]);
    pendingCcchQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(ccchQueue_gp[internalCellIndex]);
    ccchQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(pTxReqMsgBuff_gp[internalCellIndex]);
    pTxReqMsgBuff_gp[internalCellIndex] = PNULL;
    freeMemPool(pDLCfgMsgBuff_gp[internalCellIndex]);
    pDLCfgMsgBuff_gp[internalCellIndex] = PNULL;
    freeMemPool(pHIDCICfgMsgBuff_gp[internalCellIndex]);
    pHIDCICfgMsgBuff_gp[internalCellIndex] = PNULL;
    /* SPR 20445 Fix Start */
    for(SFIndex = 0; SFIndex < MAX_SUB_FRAME; SFIndex++)
    {
        freeMemPool(pULConfigMsg_gp[internalCellIndex][SFIndex]);
        pULConfigMsg_gp[internalCellIndex][SFIndex] =PNULL;
    }
    /* SPR 20445 Fix End */
    freeMemPool(macResetQueue_gp[internalCellIndex]);
    macResetQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(dlPriorityQueue_gp[internalCellIndex]);
    dlPriorityQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex]);
    explicitDLPucchTpcScheduleFailureQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(explicitDLPucchTpcScheduleQueue_gp[internalCellIndex]);
    explicitDLPucchTpcScheduleQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(dlPriorityLcTree_gp[internalCellIndex][0]); /* EICIC +*/
    dlPriorityLcTree_gp[internalCellIndex][0] = PNULL;
    freeMemPool(dlPriorityLcTree_gp[internalCellIndex][1]);
    dlPriorityLcTree_gp[internalCellIndex][1] = PNULL; /* EICIC -*/
    freeMemPool(dlDRXOnOffTriggerQueue_gp[internalCellIndex]);
    dlDRXOnOffTriggerQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(dlCellEdgeUserPriorityQueue_gp[internalCellIndex]);
    dlCellEdgeUserPriorityQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex]);
    dlUENonZeroQueueLoadAndHarqList_gp[internalCellIndex] = PNULL;
    if(DL_FAIR_SCHEDULING == (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
                initParams_p->dlSchdConfig.dlSchedulerStrategy))
    {
        freeMemPool(pfsGlobals_gp[internalCellIndex]);
        pfsGlobals_gp[internalCellIndex] = PNULL;
    }        
    freeMemPool(dlMeasGapOnOffTriggerQueue_gp[internalCellIndex]);
    dlMeasGapOnOffTriggerQueue_gp[internalCellIndex] = PNULL;

    freeMemPool(ulQosStrategyLCTree_gp[internalCellIndex][0]); /*EICIC +*/
    ulQosStrategyLCTree_gp[internalCellIndex][0] = PNULL;
    freeMemPool(ulQosStrategyLCTree_gp[internalCellIndex][1]);
    ulQosStrategyLCTree_gp[internalCellIndex][1] = PNULL; /* EICIC -*/
    freeMemPool(ulHighPriorityGrantQueue_gp[internalCellIndex]);
    ulHighPriorityGrantQueue_gp[internalCellIndex] = PNULL;
    freeMemPool(ulStrategyScheduleInfo_gp[internalCellIndex]);
    ulStrategyScheduleInfo_gp[internalCellIndex] = PNULL;
    freeMemPool(aperiodicModesRBInfo_gp[internalCellIndex]);
    aperiodicModesRBInfo_gp[internalCellIndex] = PNULL;
    freeMemPool(dummyDCI0SendFailureList_gp[internalCellIndex]);
    dummyDCI0SendFailureList_gp[internalCellIndex] = PNULL;
    freeMemPool(ulContainerPool_gp[internalCellIndex]);
    ulContainerPool_gp[internalCellIndex] = PNULL;
    freeMemPool(hiPduContainer_gp[internalCellIndex]);
    hiPduContainer_gp[internalCellIndex] = PNULL;

    /* Here all memPool should also be deleted */


    return;
}

/****************************************************************************
 * Function Name  : sendMsgToOAM
 * Inputs         : cnfBuff_p - Pointer to buffer to be send to OAM,
 *                  msgLen
 * Outputs        : None
 * Returns        : Msg Length on Success or 0 on Failure
 * Description    : This API sends the confirmation message to OAM. 
 ****************************************************************************/
SInt32 sendMsgToOAM( UInt8 *cnfBuff_p, UInt16 msgLen
        )
{
    /* Fill msgLen Now */
    SInt32 retval = 0;
    LTE_SET_U16BIT(cnfBuff_p 
            + API_HEADER_MSGLEN_OFFSET,  msgLen);

#ifdef MAC_AUT_TEST
    if (macAutFlag)
    {
        memCpy(msgBufferForOAMAndRRCMsgs,cnfBuff_p,msgLen);
    }
    else
#endif 
    {
        retval = sendto_wrapper(txOAMSockFD_g, cnfBuff_p, msgLen, 0,
                (const SockAddr *)&txOAM_addr_g, sizeof (txOAM_addr_g));
    }
    /* SPR 5599 changes end (ZIP ID 129071) */
    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "Function %s Exit\n", __func__);
    return retval;
}

/*Coverity Fix 10266 Start*/
/*Due to Stack Overuse coverity warning, this varibale is moved from local scope of 
  parseOAMMsg to global scope. In case of memory optimization, dynamic memory can be
  allocated for this variable.*/
UInt8 cnfBuff[MAX_OAM_RESP_SIZE] = {0};
/*Coverity Fix 10266 End*/

/****************************************************************************
 * Function Name  : parseOAMMsg
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket.
 *                  msgId - Message Id received in the buffer header.
 *                  msgLen_p - Pointer to the message length received.
 *                  transactionId - transactionId of the request received.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : SUCCESS or FAILURE
 * Description    : This API parse the OAm message received on socket and send 
 *                  confirmation if required.
 ****************************************************************************/
/* CA Changes start */
/* +- SPR 17777 */
UInt8 parseOAMMsg(UInt8 *msgBuf_p, UInt16 msgId,
        /* UL-SUBFRAME Code Changes Start */
        /* UL-SUBFRAME Code Changes End */
                  UInt16 transactionId, InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
/* CA Changes end */
{
    MacEnableLogCategoryReq *macEnableLogCategoryReq_p = PNULL;
    MacDisableLogCategoryReq *macDisableLogCategoryReq_p = PNULL;
#ifdef PERF_STATS
    MacConfigurePerfStatsCnfParams *macConfigurePerfStatsCnfParams_p = PNULL;
#endif
    MacRetType resp = MAC_SUCCESS;
    UInt16 current = 0;
    /* CA Changes start */
    MacInitRespParams *macInitRespParams_p = PNULL;
    UInt8 *cnfBuff_p = &cnfBuff[0];  
    InternalCellIndex cellIndex = 0;

    /* + SPR_10412 */
    LOG_MAC_MSG(OAM_MAC_PARSE_MSG_ID,LOGWARNING,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            msgId,transactionId,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"OAM_MAC_PARSE_MSG_ID");
    /* - SPR_10412 */

    /* CA Changes end */
    /*Coverity Fix 10266 Start*/
    /*531 - coverity 57491*/
    memSet(cnfBuff,0,MAX_OAM_RESP_SIZE);
    /*531 - coverity 57491*/
    /*Coverity Fix 10266 End*/
    switch(msgId) 
    {
        case MAC_INIT_LAYER_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            /* +- SPR 17777 */
            processMACInitLayerReq (msgBuf_p, cnfBuff, transactionId,
                    internalCellIndex);

            /* +- SPR 17777 */
             /* Cyclomatic Complexity changes - ends here */
            break;
        }
        /* CA Changes start */
        case MAC_INIT_CELL_REQ:
        {
            if (macParams_g.numInitReceived == macParams_g.numCellsConfigured)
            {
                /* Prepare a response and send it to OAM*/
                current += MAC_API_HEADER_LEN;

                macInitRespParams_p = (MacInitRespParams *)(cnfBuff_p + current);
                macInitRespParams_p->responseCode = MAC_LAYER_CELL_IGNORE;
                current += sizeof(MacInitRespParams);

                prepareMacOamApiHeader(cnfBuff_p, MAC_INIT_CELL_CNF,
                        transactionId, current,
                        macCommonGetRRCCellIndex(internalCellIndex));

                LOG_UT(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        msgId,MAC_INIT_CELL_CNF,resp,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

                if(MAC_SEND_ERROR ==  sendMsgToOAM( cnfBuff, current
                            ))
                {
                    ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
                }
            }
            else
            {
                processMACInitCellReq(msgBuf_p, cnfBuff, transactionId, 
                        /* +- SPR 17777 */
                        internalCellIndex);
            }
            break;
        }
        case MAC_CLEANUP_LAYER_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            processMACCleanupLayerReq (cnfBuff, transactionId,
                    /* +- SPR 17777 */
                    internalCellIndex);  
            /* Cyclomatic Complexity changes - ends here */
            break;
        }
        case MAC_SET_LOG_LEVEL_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            /* +- SPR 17777 */
            processMACSetLogLevelReq (msgBuf_p,
                    msgId,internalCellIndex);  
            /* +- SPR 17777 */
            /* Cyclomatic Complexity changes - ends here */
            break;
        }
        case MAC_GET_STATUS_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            /* +- SPR 17777 */
            processMACGetStatusReq (msgBuf_p, cnfBuff, 
                    /* +- SPR 17777 */
                    transactionId,internalCellIndex);  
            /* +- SPR 17777 */
            /* Cyclomatic Complexity changes - ends here */
            break;
        }
        case MAC_RESET_STATS_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            processMACResetStatsReq (cnfBuff, transactionId, 
                    /* +- SPR 17777 */
                    internalCellIndex);  
            /* Cyclomatic Complexity changes - ends here */
            break;
        }
        case MAC_GET_STATS_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            processMACGetStatsReq (cnfBuff, transactionId, 
                    /* +- SPR 17777 */
                    internalCellIndex);
            /* Cyclomatic Complexity changes - ends here */
            break;
        }
        /* CA Changes end */
#ifdef KPI_STATS
        case MAC_CONFIGURE_KPI_STATS_REQ:
        {
            /* CA Changes start */
            processMACConfigureKpiStatsReq (msgBuf_p, cnfBuff, transactionId,
                    msgId,internalCellIndex);  
            /* CA Changes end */
            break;
        }
        case MAC_GET_KPI_STATS_REQ:
        {
            /* CA Changes start */
            /* Stats will be not sent for all Cells - served only requested cell */
            /* +- SPR 17777 */
            processMACGetKpiStatsReq (msgBuf_p, cnfBuff, 
                    transactionId, msgId,internalCellIndex);
            /* Reset the buffer to send the THP stats */
            memSet(cnfBuff,0,MAX_OAM_RESP_SIZE);

            /* +- SPR 17777 */
            sendMacGetKpiThpStatsCnf(msgBuf_p, cnfBuff,
                    transactionId,KPI_CONTAINER_THP_OAM_MAC,internalCellIndex);
            /* +- SPR 17777 */
            /* CA Changes end */
            break;            
        }
#endif
        case MAC_GET_KPI_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            /* CA Changes start */
            /* +- SPR 17777 */
            processMACGetKpiReq (cnfBuff, transactionId,internalCellIndex);  
            /* +- SPR 17777 */
            /* CA Changes end */
            /* Cyclomatic Complexity changes - ends here */
            break;
        }
        case MAC_GET_LOG_LEVEL_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            /* CA Changes start */
            /* +- SPR 17777 */
            processMACGetLogLevelReq (cnfBuff, transactionId,internalCellIndex);  
            /* +- SPR 17777 */
            /* CA Changes end */
            /* Cyclomatic Complexity changes - ends here */
            break;
        }
        case MAC_ENABLE_LOG_CATEGORY_REQ:
        {
            macEnableLogCategoryReq_p = (MacEnableLogCategoryReq *)(msgBuf_p + MAC_API_HEADER_LEN);
            enable_log_category(macEnableLogCategoryReq_p->logCategory);
            break;
        }
        case MAC_DISABLE_LOG_CATEGORY_REQ:
        {
            macDisableLogCategoryReq_p = (MacDisableLogCategoryReq *)(msgBuf_p + MAC_API_HEADER_LEN);
            disable_log_category(macDisableLogCategoryReq_p->logCategory);
            break;
        }
        /* CA Changes start */
        case MAC_GET_LOG_CATEGORY_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            processMACGetLogCategoryReq (cnfBuff, transactionId, 
                    /* +- SPR 17777 */
                    internalCellIndex);  
                    /* +- SPR 17777 */
            /* Cyclomatic Complexity changes - ends here */
            break;
        }
         /* QOS_OAM_MODIFY_CHANGES_start */   
        case MAC_MODIFY_LAYER_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            /* +- SPR 17777 */
            processMACModifyLayerReq (msgBuf_p,
                    transactionId,internalCellIndex);  
            /* +- SPR 17777 */
            /* Cyclomatic Complexity changes - ends here */
            break;
        }   
        /* CA Changes start */
         /* QOS_OAM_MODIFY_CHANGES_end */   
#ifdef UTFWK_SIMULATION
        case PERIODIC_TRAFFIC_REQ:
        resp = parsePeriodicTrafficReq(msgBuf_p);
        break;
        case ONE_TICK_REQ:
#ifdef FDD_CONFIG
        recvFromOamDone_g = 1;
        UInt16 loop = 0;
        if (recvFromOamDone_g == 1)
        {
            UInt8 *buffer_p = (UInt8 *)(msgBuf_p + MAC_API_HEADER_LEN);
            UInt16 noOfTick = *((UInt8 *)(buffer_p));
            noOfTick = (noOfTick & 0x00FF) << 8;
            noOfTick = noOfTick | *((UInt8 *)(buffer_p + 1));

            for (loop = 0; loop < noOfTick; loop++)
            {
                if (MAC_FAILURE == callExecutionFlowThreads(
                            executionFlowCreationParamArr_g))
                {
                    ltePanic("[%s]Error in execution flow thread creation",__func__);
                }
            }
        }
#else
        /* Getting the number of ticks received in Tick Pdu in case of TDD */
        recvFromOamDone_g = 1;
        UInt8 *buffer_p = (UInt8 *)(msgBuf_p + MAC_API_HEADER_LEN);
        UInt16 noOfTick = *((UInt8 *)(buffer_p));
        noOfTick = (noOfTick & 0x00FF) << 8;
        noOfTick = noOfTick | *((UInt8 *)(buffer_p + 1));
        noOfTickToBeProcess_g = noOfTick;

#endif
        break;
        case ONE_DL_PDU_REQ:
        recvFromOamDone_g = 1;
        resp = parseAperiodicDLTrafficReq(msgBuf_p);
        break;
        case ONE_UL_PDU_REQ:
        recvFromOamDone_g = 1;
        resp = parseAperiodicULTrafficReq(msgBuf_p);
        break;
        case DRX_PDU_REQ:
        recvFromOamDone_g = 1;
        resp = parseDrxReq(msgBuf_p);
        if (resp)
        {
            processDrxReq();
        } 
        break;
        case MEAS_GAP_REQ:
        recvFromOamDone_g = 1;
        resp = parseMeasGapReq(msgBuf_p);
        if (resp)
        {
            processMeasGapReq();
        } 
        break;
        case UL_SYNC_LOSS_REQ:
        recvFromOamDone_g = 1;
        resp = parseUlSyncLossReq(msgBuf_p);
        if (resp)
        {
            processUlSyncLossReq();
        } 
        break;
        case SR_PDU_REQ:
        recvFromOamDone_g = 1;
        resp = parseSRReq(msgBuf_p);
        if (resp)
        {
            processSRReq();
        } 
        break;
        case TA_PDU_REQ:
        resp = parseTaReq(msgBuf_p);
        if (resp)
        {
            parseTaFlag_g = 1;
        }
        break;
        case UL_GRANT_PUSCH_FAILURE_QUEUE_REQ:
        resp = parseULGrantPUSCHFailureQueueReq(msgBuf_p);
        if(resp)
        {
            processULGrantPUSCHFailureQueueReq();
        }
        break;

        case UL_GRANT_PUSCH_PENDING_QUEUE_REQ:
        resp = parseULGrantPUSCHPendingQueueReq(msgBuf_p);
        if(resp)
        {
            processULGrantPUSCHPendingQueueReq();
        }
        break;

        case CR_QUEUE_REQ:
        resp = parseULGrantCRQueueReq(msgBuf_p);
        if(resp)
        {
            processULGrantCRQueueReq();
        }
        break;
        case APERIODIC_CQI_QUEUE_REQ:
        resp = parseULGrantAPeriodicCQIQueueReq(msgBuf_p);
        if(resp)
        {
            processULGrantAPeriodicCQIQueueReq();
        }
        break;
#endif

        /*CLPC_CHG*/
        /* Moved to RRM interface */
        case MAC_UE_SINR_TA_REQ:
        {
            /* Cyclomatic Complexity changes - starts here */
            /* CA Changes start */
            /* +- SPR 17777 */
            processMACUeSinrTaReq (msgBuf_p,
                    transactionId,internalCellIndex);  
            /* CA Changes end */
            /* Cyclomatic Complexity changes - ends here */
            break;
        }
        /*CLPC_CHG*/
#ifdef PERF_STATS
        case MAC_CONFIGURE_PERF_STATS_REQ:
        {
            /* CA Changes start */
            /* +- SPR 17777 */
            resp = handleMACConfigureOAMPerfStatsReq(msgBuf_p, transactionId,
                    internalCellIndex);
            /* +- SPR 17777 */
            /* CA Changes end */

            current = MAC_API_HEADER_LEN;
            macConfigurePerfStatsCnfParams_p = (MacConfigurePerfStatsCnfParams *)&cnfBuff[current];
            macConfigurePerfStatsCnfParams_p->responseCode = resp;
            current += sizeof(MacConfigurePerfStatsCnfParams);

            /* Fill header information */
            prepareMacOamApiHeader(cnfBuff
                    , MAC_CONFIGURE_PERF_STATS_RESP,
                    transactionId, current,
                    macCommonGetRRCCellIndex(internalCellIndex));

            if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff, current
                        ))
            {
                ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            }
            break;
        }
        case MAC_GET_PERF_STATS_REQ:
        {
            /* CA Changes start */
            handleMACGetOAMPerfStatsReq(msgBuf_p,cnfBuff,transactionId,
                    internalCellIndex);
            /* CA Changes end */
            break;
        }
#endif
        case MAC_PROC_SUP_REQ:
        {
            /*CA_HSM_CHG */
            /* Update control thread for all cells regardless of the cell configured
             * The configured cells will be checked when responding OAM supervision */
            for (cellIndex=0; cellIndex < MAX_NUM_CELL; cellIndex++)
            {
                l2ThreadHealthStoreTick_g.rlcMacThread[cellIndex].macThread[MAC_MAIN_THREAD] = 
                    macTTITickCount_g;
            }
            /* The cell index received in the message is not used, instead the status of all
             * configured cells is informed to OAM */
            resp = handleMACHealthMonitorStatusReq();
            /* CA Changes end */
            if (MAC_SUCCESS == resp)
            {
                current = MAC_API_HEADER_LEN;

                /* Fill header information */
                prepareMacOamApiHeader(cnfBuff_p, MAC_PROC_SUP_RESP,
                        transactionId, current,
				/*SPR 21683 Start*/                        
                        macCommonGetRRCCellIndex(internalCellIndex));
		/*SPR 21683 End*/                        

                if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff, current
                            ))
                {
                    ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
                }
            }
            break;
        }
        /*spr 22474 start*/
        case OAM_MAC_GET_DEBUG_INFO_REQ:
        {
            /*+CID 147575*/
            handleMACGetDebugInfoReq(msgBuf_p, transactionId, internalCellIndex);
            /*-CID 147575*/
            break;
        }
        /*spr 22474 end*/
        default:
        {
            lteWarning("Wrong API Id Received.\n");
            LOG_MAC_MSG(OAM_MAC_INVALID_MSG_ID,LOGWARNING,MAC_OAM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    msgId,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"OAM_MAC_INVALID_MSG_ID");

            break;
        }
    }
    return MAC_SUCCESS;
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : processMACInitLayerReq
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket.
 *                  cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - internalCellIndex of UE
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Init Layer Request
 *                  received from OAM.
 ****************************************************************************/
STATIC  void processMACInitLayerReq(UInt8 *msgBuf_p, 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        /* CA changes Start */
        InternalCellIndex internalCellIndex)
        /* CA changes End */
{
    UInt16 resp    = MAC_SUCCESS;
#ifndef FLEXRAN
    UInt16 current = 0;
    MacInitRespParams *macInitRespParams_p = PNULL;
#endif
            /* SPR 22936 Start */
#ifndef DL_DATA_SEPARATION
            UInt8 cellIndex = 0; 
#endif
            /* SPR 22936 End */

    /* CA Changes start */
   if (PNULL == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex])
   {
         resp = MAC_INIT_LAYER_REQ_MEMORY_FAILURE; 
   } 
   else if(CELL_INIT == cellSpecificParams_g.\
             cellConfigAndInitParams_p[internalCellIndex]->cellState) 
    /* CA Changes end */
    {
        /* Parse the MAC Init Layer Request */
        resp = parseMACInitLayerReq(msgBuf_p,
                /* CA changes Start */
                internalCellIndex
            /* CA changes End */
                );
        /*L2_Fix_22_23start*/
        if (resp == MAC_SUCCESS)
        /*L2_Fix_22_23_end*/
        {
            /* SPR 16091 fix start */
            LOG_MAC_INFO( MAC_OAM_INF, "Parsed init layer req successfully!" );

            phyInitTransactionId_g = transactionId;
             /* SPR 16091 fix end */           
            /* CA Changes start */
            /* send PHY_PARAM_REQUEST and WAIT for response */
#ifndef UTFWK_SIMULATION
/* SPR 23215 CHANGES START*/            
#ifndef INTEL_NOCA_L1 
/* SPR 23215 CHANGES END*/            
            resp = fapiSendPhyParamReq(internalCellIndex);
#endif
/* SPR 23215 CHANGES START*/            
#endif	
/* SPR 23215 CHANGES END*/            
            /* CA Changes end */

#ifdef SSI_DEBUG_STATS
            InitParams *initParams_p = PNULL;
            SpsSysWideParam     *spsSysWideParam_p  = PNULL;
            UInt8 intCtr = 0;
            initParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                           initParams_p;
            spsSysWideParam_p = &initParams_p->spsSysWideParam; 
            /* CA Stats Changes Start */
            updateSchConfigStats(initParams_p, internalCellIndex);
            LTE_MAC_UPDATE_SYSTEM_WIDE_STATS( spsSysWideParam_p, 
                    internalCellIndex);
            for( intCtr = 0; intCtr < SPS_INTERVAL_LIST; intCtr++ )
            {
                if( spsSysWideParam_p->supportedSpsDlIntervalList[intCtr] )
                {
                    LTE_MAC_UPDATE_DL_INTERVAL_STATS(
                            initParams_p->dlSchdConfig.spsDlIntervalProfile[intCtr],
                            intCtr, internalCellIndex);
                }
                if( spsSysWideParam_p->supportedSpsUlIntervalList[intCtr] )
                {
                    LTE_MAC_UPDATE_UL_INTERVAL_STATS(
                            initParams_p->ulSchdConfig.spsUlIntervalProfile[intCtr],
                            intCtr, internalCellIndex);
                }
                if( spsSysWideParam_p->supportedSpsUlIntervalList[intCtr]  ||
                        spsSysWideParam_p->supportedSpsDlIntervalList[intCtr] )
                {
                    LTE_MAC_UPDATE_COMM_INTERVAL_STATS(
                            initParams_p->spsCommIntervalProfile[intCtr], intCtr,
                            internalCellIndex);
                }
                /* CA Stats Changes End */
            }
#endif
        }
    } 
    else 
    {
        resp = MAC_LAYER_ALREADY_INITIALIZED;
    }    
    if(resp != MAC_SUCCESS)
    {
#ifndef FLEXRAN
        UInt8 *cnfBuf_p = cnfBuff_p;  
        /* MAC PLUGGABLE SCHEDULER BEGIN */
        /*Prepare a response and send it to OAM*/
        current += MAC_API_HEADER_LEN;
        macInitRespParams_p = (MacInitRespParams *)(cnfBuf_p + current);
        macInitRespParams_p->responseCode = resp;
        current += sizeof(MacInitRespParams);

        /* Fill header information */
        prepareMacOamApiHeader(cnfBuf_p, MAC_INIT_LAYER_CNF,
                transactionId, current,
                macCommonGetRRCCellIndex(internalCellIndex));

        LOG_MAC_ERROR( MAC_OAM_INF, "MAC-OAM: Failure response[%u] cellId[%u]",
                resp, internalCellIndex  );

        if(MAC_SEND_ERROR ==  sendMsgToOAM(cnfBuff_p, current
                    ))
        {
            ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        }
#endif
        if ((MAC_LAYER_ALREADY_INITIALIZED != resp) &&
                (MAC_INIT_LAYER_REQ_MEMORY_FAILURE != resp))
        {
            /*LTE_REG_FIX*/
            FREE_MEMORY_FOR_CELL_INIT_PARAMS(internalCellIndex);
        }
    }
    /* MAC PLUGGABLE SCHEDULER END */
    else
    {
        /* CA Changes start */
        /* Update the cell state */
#ifndef FLEXRAN
        /* Update the cell state */
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellState = 
            CELL_WAITING_FOR_PHY_CAPABILITY;
/* SPR 19288 change start */
#endif
/* SPR 19288 change end */ 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce =
            LTE_FALSE;
        macParams_g.numInitReceived++;
        if (INVALID_CELL_INDEX == macParams_g.numCellsConfigured)
        {
            macParams_g.numCellsConfigured = 1;
        }    
        if (macParams_g.numInitReceived == macParams_g.numCellsConfigured)
        {
            /* All memory pools, queues based on number of configured cells*/
            initializeMAC(cellSpecificParams_g.numOfCells
                    );
            initExecutionScheduler(cellSpecificParams_g.numOfCells
                    );
            createExecutionThreads();
            /** SPR 13599 Changes Start **/
            /* SPR 22936 Start */
#ifndef DL_DATA_SEPARATION
            for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
            {
               GET_MEM_FROM_POOL(TransmissionAndRLCData,transmissionAndRLCData_g[cellIndex],sizeof(TransmissionAndRLCData),PNULL);
            }
#endif
            /* SPR 22936 End */
            /** SPR 13599 Changes End **/
    /* - SPR_11589 */  
			/* + stats_review_comments_ipc */
#ifdef SSI_DEBUG_STATS
            gMacStats.pStats->numActiveCells = cellSpecificParams_g.numOfCells;
#endif
			/* - stats_review_comments_ipc */
        }
        /* CA Changes end */
    }
    return;
}
/* CA Changes start */
/****************************************************************************
 * Function Name  : processMACInitCellReq
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket.
 *                  cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Variables      : None
 * Description    : This function processes the MAC Init Layer Request
 *                  received from OAM.
 ****************************************************************************/
STATIC  void processMACInitCellReq(UInt8 *msgBuf_p, 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
       InternalCellIndex internalCellIndex)
{
    UInt16 resp = MAC_SUCCESS;
    UInt16 current = 0;
    UInt8 isSocketTobeOpened = FALSE;
    MacInitRespParams *macInitRespParams_p = PNULL;

    /* Fetch and initialize the memory for second cell */
    /* Fetch memory only if the cell has not been allocated memory yet */
    if (PNULL == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex])
    {
        GET_MEMORY_FOR_CELL_INIT_PARAMS(internalCellIndex,isSocketTobeOpened);
    }
#ifdef MAC_AUT_TEST
    if (macAutFlag)
    {
        isSocketTobeOpened = 0;
    }
#endif
    if (isSocketTobeOpened)
    { 
/*  SPR 23215 CHANGES  START */
#ifndef FLEXRAN        
        initPhySocketInterface(internalCellIndex);
#endif 
/*  SPR 23215 CHANGES  END */
    }

    /* If memory allocation failed */
    if (PNULL == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex])
    {
        resp = MAC_INIT_CELL_REQ_MEMORY_FAILURE;
    }
    else if(CELL_INIT == 
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellState)
    {
        resp = macInterfaceParseMACInitCellReq(msgBuf_p,internalCellIndex);
        if (MAC_SUCCESS == resp)
        {
            LOG_MAC_INFO( MAC_OAM_INF, "Parsed Init cell Req successfully!" );
            /* send PHY_PARAM_REQUEST and WAIT for response */
#ifndef UTFWK_SIMULATION
            resp = fapiSendPhyParamReq(internalCellIndex);
#endif
#ifdef SSI_DEBUG_STATS
            InitParams *initParams_p = PNULL;
            SpsSysWideParam *spsSysWideParam_p = PNULL;
            UInt8 intCtr = 0;
            initParams_p = cellSpecificParams_g.\
                           cellConfigAndInitParams_p[macParams_g.numInitReceived]->initParams_p;
            spsSysWideParam_p = &initParams_p->spsSysWideParam; 
            /* CA Stats Changes Start */
            updateSchConfigStats(initParams_p, internalCellIndex);
            LTE_MAC_UPDATE_SYSTEM_WIDE_STATS( spsSysWideParam_p,
                    internalCellIndex);
            for( intCtr = 0; intCtr < SPS_INTERVAL_LIST; intCtr++ )
            {
                if( spsSysWideParam_p->supportedSpsDlIntervalList[intCtr] )
                {
                    LTE_MAC_UPDATE_DL_INTERVAL_STATS(
                            initParams_p->dlSchdConfig.spsDlIntervalProfile[intCtr],
                            intCtr, internalCellIndex);
                }
                if( spsSysWideParam_p->supportedSpsUlIntervalList[intCtr] )
                {
                    LTE_MAC_UPDATE_UL_INTERVAL_STATS(
                            initParams_p->ulSchdConfig.spsUlIntervalProfile[intCtr],
                            intCtr, internalCellIndex);
                }
                if( spsSysWideParam_p->supportedSpsUlIntervalList[intCtr]  ||
                        spsSysWideParam_p->supportedSpsDlIntervalList[intCtr] )
                {
                    LTE_MAC_UPDATE_COMM_INTERVAL_STATS(
                            initParams_p->spsCommIntervalProfile[intCtr], intCtr,
                            internalCellIndex);
                }
                /* CA Stats Changes End */
            }
#endif
        }
    }
    else 
    {
        resp = MAC_LAYER_ALREADY_INITIALIZED;
    }    

    if(resp != MAC_SUCCESS)
    {
        UInt8 *cnfBuf_p = cnfBuff_p;
        /*Prepare a response and send it to OAM*/
        current += MAC_API_HEADER_LEN;
        macInitRespParams_p = (MacInitRespParams *)(cnfBuf_p + current);
        macInitRespParams_p->responseCode = resp;
        current += sizeof(MacInitRespParams);

        /* Fill header information */
        prepareMacOamApiHeader(cnfBuf_p, MAC_INIT_CELL_CNF,
                transactionId, current,
                macCommonGetRRCCellIndex(internalCellIndex));
        
        updateInfoForInitReqRcvd(resp, internalCellIndex);
        LOG_MAC_INFO( MAC_OAM_INF, "MAC_INIT_CELL: Response[%u]", resp );

        if(MAC_SEND_ERROR ==  sendMsgToOAM(cnfBuff_p, current
                    ))
        {
            ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        }
    }
    else
    {
        /* Update the cell state */
#ifndef FLEXRAN
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellState = 
            CELL_WAITING_FOR_PHY_CAPABILITY;
#endif	
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce =
            LTE_FALSE;

        macParams_g.numInitReceived++;

        if (macParams_g.numInitReceived == macParams_g.numCellsConfigured)
        {
            /* All memory pools, queues based on number of configured cells*/
            initializeMAC(cellSpecificParams_g.numOfCells
                    );
            initExecutionScheduler(cellSpecificParams_g.numOfCells 
                    );
            createExecutionThreads();
        }
    }
    return;
}
/* CA Changes end */

/****************************************************************************
 * Function Name  : processMACCleanupLayerReq
 * Inputs         : cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Cleanup Layer Request
 *                  received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void processMACCleanupLayerReq( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
/* CA Changes end */
{
    UInt8 *cnfBuf_p = cnfBuff_p;
    UInt16 resp    = MAC_SUCCESS;
    UInt16 current = 0;
    /* CA Changes start */
     resp = MAC_FAILURE;   
    /*Prepare a response and send it to OAM*/
    current += MAC_API_HEADER_LEN;
    LTE_SET_U16BIT(cnfBuf_p + current, resp );
    current += 2;

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuf_p, MAC_CLEANUP_LAYER_CNF,
                                  transactionId, current,
                                  macCommonGetRRCCellIndex(internalCellIndex));

    /* +- SPR 17777 */
    LOG_UT(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            0,MAC_CLEANUP_LAYER_CNF,resp,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    /*Send confirmation to OAM */
    if(MAC_SEND_ERROR ==  sendMsgToOAM(cnfBuff_p, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}

/****************************************************************************
 * Function Name  : processMACSetLogLevelReq 
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket.
 *                  cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Set Log Level Request
 *                  received from OAM.
 ****************************************************************************/
/* CA Changes start */
/* +- SPR 17777 */
static  void processMACSetLogLevelReq( UInt8 *msgBuf_p, 
        UInt16 msgId,
        InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
/* CA Changes end */
{
    /*Parse and enable the Log Level received from OAM for entire L2 currently, not just MAC*/
    parseSetLogLevel(msgBuf_p);
    /* +- SPR 17777 */
    LOG_MAC_MSG(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            msgId,MAC_SET_LOG_LEVEL_CNF,MAC_SUCCESS,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");
}

/****************************************************************************
 * Function Name  : processMACGetStatusReq 
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket.
 *                  msgLen_p - Pointer to message length received.
 *                  cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Get Status Request
 *                  received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void processMACGetStatusReq( UInt8 *msgBuf_p, 
        /* +- SPR 17777 */
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex)
/* CA Changes end */
{
    UInt8 *cnfBuf_p = cnfBuff_p;
    UInt16 resp    = MAC_SUCCESS;
    UInt16 current = 0;
    MacUEStatusResp *macUEStatusResp_p = PNULL;

    current += MAC_API_HEADER_LEN;

    /* +- SPR 17777 */
    resp = parseGetStatusReq(msgBuf_p,cnfBuf_p, 
            &current, internalCellIndex);
    /* +- SPR 17777 */
    macUEStatusResp_p = (MacUEStatusResp *)(cnfBuf_p + MAC_API_HEADER_LEN); 
    macUEStatusResp_p->responseCode = resp;
    /* SPR 2153 Fix Start */
    if( resp != MAC_SUCCESS)
    {
        macUEStatusResp_p->numOfUe = 0;
    }
    /* SPR 2153 Fix End */

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuf_p, MAC_GET_STATUS_CNF,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));

    LOG_UT(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            0,MAC_GET_STATUS_CNF,resp,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    /* Fill Msg Len in the API header */
    /*Send confirmation to OAM */
    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff_p, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}

/****************************************************************************
 * Function Name  : processMACResetStatsReq
 * Inputs         : cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Reset Stats Request
 *                  received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void processMACResetStatsReq( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex)
/* CA Changes end */
{
    UInt8 *cnfBuf_p = cnfBuff_p;
    UInt16 resp    = MAC_SUCCESS;
    UInt16 current = 0;
    MacResetStatsResp *macResetStatsResp_p = PNULL;

    current += MAC_API_HEADER_LEN;
    /* CA Changes start*/
    resp = macResetStats(internalCellIndex);
    /* CA Changes end */
    macResetStatsResp_p = (MacResetStatsResp *)(cnfBuf_p + current);
    macResetStatsResp_p->responseCode = resp;
    current += sizeof(MacResetStatsResp);

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuf_p, MAC_RESET_STATS_CNF,
                           transactionId, current,
                           macCommonGetRRCCellIndex(internalCellIndex));

    LOG_UT(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            0,MAC_RESET_STATS_CNF,resp,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff_p, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}

/****************************************************************************
 * Function Name  : processMACGetStatsReq
 * Inputs         : cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Get Stats Request
 *                  received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void processMACGetStatsReq ( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex)
/* CA Changes end */
{
    UInt8 *cnfBuf_p = cnfBuff_p;
    UInt16 current = MAC_API_HEADER_LEN;

    /* CA Changes start */
    macGetStats(cnfBuf_p, &current,internalCellIndex);

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuf_p, MAC_GET_STATS_CNF,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));
    /* CA Changes end */

    LOG_UT(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            0,MAC_RESET_STATS_CNF,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    /* Fill Msg Len in the API header */
    /*Send confirmation to OAM */
    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff_p, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}

#ifdef KPI_STATS
/****************************************************************************
 * Function Name  : processMACConfigureKpiStatsReq
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket.
 *                  cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - cell id 
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC configure KPI Stats
 *                  Request received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void processMACConfigureKpiStatsReq( UInt8 *msgBuf_p, 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        UInt16 msgId,
        InternalCellIndex  internalCellIndex)
    /* CA Changes end */
{
    UInt8 *cnfBuf_p = cnfBuff_p;
    UInt16 resp    = MAC_SUCCESS;
    UInt16 current = MAC_API_HEADER_LEN;
    MacConfigureKpiStatsResp *macConfigureKpiStatsResp_p = PNULL;

    /* CA Changes start */
    resp = handleMACConfigureKpiStatReq(msgBuf_p, transactionId, KPI_CONTAINER_OAM_MAC);

    macConfigureKpiStatsResp_p = (MacConfigureKpiStatsResp *)
        (cnfBuf_p + MAC_API_HEADER_LEN);
    macConfigureKpiStatsResp_p->responseCode = resp;
    current += sizeof(MacConfigureKpiStatsResp);

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuf_p, MAC_CONFIGURE_KPI_STATS_CNF,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));

    LOG_MAC_MSG(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            msgId,MAC_CONFIGURE_KPI_STATS_CNF,resp,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    /* Fill Msg Len in the API header */
    /*Send confirmation to OAM */
    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff_p, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
    return;
}

/****************************************************************************
 * Function Name  : processMACGetKpiStatsReq 
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket.
 *                  msgLen_p - Pointer to message length received.
 *                  cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - Cell id 
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Get KPI Stats Request
 *                  received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void processMACGetKpiStatsReq( UInt8 *msgBuf_p, 
        /* +- SPR 17777 */
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        UInt16 msgId,
        InternalCellIndex  internalCellIndex)
{
    UInt8 *cnfBuf_p = cnfBuff_p;
    UInt16 current = MAC_API_HEADER_LEN;

    parseMACGetKpiStatReq(msgBuf_p, cnfBuf_p, &current, 
            KPI_CONTAINER_OAM_MAC,internalCellIndex);

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuf_p, MAC_GET_KPI_STATS_CNF,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));
    /* CA Changes end */

    LOG_MAC_MSG(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            msgId,MAC_GET_KPI_STATS_CNF,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    /* Fill Msg Len in the API header */
    /*Send confirmation to OAM */
    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff_p, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}
#endif

/****************************************************************************
 * Function Name  : processMACGetKpiReq 
 * Inputs         : cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - Cell id
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Get KPI Request
 *                  received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void processMACGetKpiReq( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
/* CA Changes end */
{
    UInt8 *cnfBuf_p = cnfBuff_p;
    UInt16 current = MAC_API_HEADER_LEN;

    macGetKpi(cnfBuf_p, &current,internalCellIndex);

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuf_p, MAC_GET_KPI_CNF,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));

    LOG_UT(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            0,MAC_GET_KPI_CNF,
            (UInt32) *((UInt16 *)(cnfBuf_p + MAC_API_HEADER_LEN)) ,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff_p, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}

/****************************************************************************
 * Function Name  : processMACGetLogLevelReq 
 * Inputs         : cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Get Log level Request
 *                  received from OAM and fill response with log level 
 *                  enabled earlier for entire L2 and not just MAC.
 ****************************************************************************/
/* CA Changes start */
static  void processMACGetLogLevelReq( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex)
/* CA Changes end */
{
    UInt16 current = 0;
    UInt8 *cnfBuf_p = cnfBuff_p;
    MacGetLogLevelResp *macGetLogLevelResp_p = PNULL;

    current += MAC_API_HEADER_LEN;
    macGetLogLevelResp_p = (MacGetLogLevelResp *)(cnfBuf_p + MAC_API_HEADER_LEN);
    /*L2 Log Level which was enabled earlier will filled up in Response Msg*/
    macGetLogLevelResp_p->logLevel = get_log_level();
    current += sizeof(MacGetLogLevelResp);

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuf_p, MAC_GET_LOG_LEVEL_RESP,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));

    LOG_UT(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            0,MAC_GET_KPI_CNF,(UInt32) *((UInt16*)(cnfBuf_p + MAC_API_HEADER_LEN)) ,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff_p, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}

/****************************************************************************
 * Function Name  : processMACGetLogCategoryReq 
 * Inputs         : cnfBuff_p - Pointer to the buffer to be sent to OAM.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Get Log Category Request
 *                  received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void processMACGetLogCategoryReq( 
        UInt8  *cnfBuff_p, 
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex)
/* CA Changes end */
{
    UInt8 *cnfBuf_p = cnfBuff_p;
    UInt16 current = 0;
    MacGetLogCategoryResp *macGetLogCategoryResp_p = PNULL;

    current += MAC_API_HEADER_LEN;
    macGetLogCategoryResp_p = (MacGetLogCategoryResp *)
        (cnfBuf_p + MAC_API_HEADER_LEN);
    macGetLogCategoryResp_p->logCategory = get_log_category();     
    current += sizeof(MacGetLogCategoryResp);

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuf_p, MAC_GET_LOG_CATEGORY_RESP,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));

    LOG_UT(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            0,MAC_GET_KPI_CNF,(UInt32) *((UInt16*)(cnfBuf_p + MAC_API_HEADER_LEN)) ,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff_p, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}

/****************************************************************************
 * Function Name  : processMACModifyLayerReq 
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket.
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC Modify Layer Request
 *                  received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void processMACModifyLayerReq( UInt8 *msgBuf_p, 
        /* +- SPR 17777 */
        UInt16 transactionId,
        /* +- SPR 17777 */
        InternalCellIndex  internalCellIndex)
/* CA Changes end */
{
    UInt8 *cnfBuf_p = cnfBuff;
    MacRetType resp = MAC_SUCCESS;
    UInt16 current = 0;
    /* QOS_OAM_MODIFY_CHANGES_start */
    MacModifyQosPamams *modifyPamams_p = PNULL;
    MacQoSModifyparamsResp *macQoSModifyparamsResp_p = PNULL;
    /* QOS_OAM_MODIFY_CHANGES_end */

    modifyPamams_p = (MacModifyQosPamams *)(msgBuf_p + MAC_API_HEADER_LEN);

    /* CA Changes start */
    /** SPR 13173 Fix : Start **/
    /* +- SPR 17777 */
    resp = validateModifyQosParamters(modifyPamams_p);
    if (MAC_SUCCESS == resp)
    {
        resp = qosModifyQosParameters(modifyPamams_p,internalCellIndex);
    }
    /** SPR 13173 Fix : End **/
    /* CA Changes end */
    /*Prepare a response and send it to OAM*/
    current += MAC_API_HEADER_LEN;

    macQoSModifyparamsResp_p = (MacQoSModifyparamsResp *)( cnfBuf_p + current);
    macQoSModifyparamsResp_p->responseCode = resp;
    current += sizeof(MacQoSModifyparamsResp);

    /* Fill header information */
    prepareMacOamApiHeader( cnfBuf_p, MAC_MODIFY_LAYER_CNF,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));

    LOG_UT(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            0,MAC_MODIFY_LAYER_CNF,resp,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    if(MAC_SEND_ERROR ==  sendMsgToOAM(cnfBuff, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}

/****************************************************************************
 * Function Name  : processMACUeSinrTaReq 
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket.
 *                  transactionId - Stores transactionId.
 *                  internalCellIndex - cell index for which OAM requests stats
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the MAC UE SINR TA Request
 *                  received from OAM.
 ****************************************************************************/
/* CA Changes start */
static  void processMACUeSinrTaReq( UInt8 *msgBuf_p, 
        /* +- SPR 17777 */
        UInt16 transactionId,
        InternalCellIndex  internalCellIndex)
{
    UInt16 resp    = MAC_SUCCESS;
    UInt16 current = 0;
    /*CLPC_CHG*/
    UInt16 ueIndex = 0;
    UInt16 rrcUeIndex = 0;
    MacUeSinrTaResp *macUeSinrTaResp = PNULL;
    /*CLPC_CHG*/
    UInt8 *cnfBuf_p = cnfBuff;

    UInt16 *buffer_p = (UInt16 *)(msgBuf_p + MAC_API_HEADER_LEN);
    rrcUeIndex = *buffer_p;

    current += MAC_API_HEADER_LEN;

    macUeSinrTaResp = (MacUeSinrTaResp *)(cnfBuf_p + current);
    /* SPR 4501 fix */
    macUeSinrTaResp->responseCode = resp; 
    /* SPR 4501 fix */

    /* fill the ueIndex recvd from RRM in response */
    macUeSinrTaResp->ueIndex = rrcUeIndex;

    /* Now get the internal cell index */
    ueIndex = getUeIdxFromRrcUeIdx(rrcUeIndex);
    /* The validity of internal UE index is checked in the below function
     * call to fill the data */

    /* Fill the data for requested UE */
    fillMACUeSinrTaReq (ueIndex, internalCellIndex, macUeSinrTaResp);

    current += sizeof(MacUeSinrTaResp);

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuf_p, MAC_UE_SINR_TA_RESP,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));

    if(MAC_SEND_ERROR ==  sendMsgToOAM(cnfBuff, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
    return;
}

/* CA Changes end */
/* Cyclomatic Complexity changes - ends here */


/****************************************************************************
 * Function Name  : macValidateAndUpdateIntervalList
 * Inputs         : MacIntList and
 *                  intList - interval list provided by OAM
 * Outputs        : MAC Interval List.
 * Returns        : None
 * Description    : Validates and update interval List array as provided by OAM.
 ****************************************************************************/
static void macValidateAndUpdateIntervalList(UInt8 MacIntList[],UInt8 intList[])
{
    UInt8  idx    = 0;

    /* Compare every index provided by OAM */
    for( idx = 0; idx < SPS_INTERVAL_LIST; idx++)
    {
        /* Compare with interval list as specified in 36.331, if different
         * */
        if( LTE_TRUE < intList[idx])
        {
            /* By default only 20ms shall be enabled  
             * and rest of the values disabled */
            if (idx == SPS_ITTERVAL_20)
            {
                 MacIntList[idx] = LTE_TRUE;
            }
            else
            {
                 MacIntList[idx] = LTE_FALSE;
            }
        }
        else
        {
            MacIntList[idx] = intList[idx];
        }
    }
}

#ifdef PERF_STATS
/****************************************************************************
 * Function Name  : ueFillMacOamPerfContainer
 * Inputs         : pMacUeOamPerfStats - pointer to UeMacPerfStatsResp,
 *                  passiveIndexForMacStats and
 *                  uePerfStatsBitMap
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the MAC UE Perf Container as 
 *                  received from OAM.
 *****************************************************************************/
void ueFillMacOamPerfContainer (UeMacPerfStatsResp *pMacUeOamPerfStats, 
                                UInt32 passiveIndexForMacStats,
 				UInt32 uePerfStatsBitMap)
{
    UInt32 count = 0;
    UInt32 ueIndex = 0;
    UInt32 firstBitSet = 0;
    UInt32 localBitmap = 0; 

    LteUePerfStats *pLteUeMacPerfStats = 
        &((gPerfStats.perfStats[passiveIndexForMacStats].lteUePerfStats[0]));
    LteUeMacULPerfStats *pLteUeMacULPerfStats = PNULL; 
    LteUeMacDLPerfStats *pLteUeMacDLPerfStats =  PNULL;
    LteUeMacSchedDLPerfStats *pLteUeMacSchedDLPerfStats = PNULL;
    LteUeMacSchedULPerfStats *pLteUeMacSchedULPerfStats = PNULL;

    for (ueIndex = 0; ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
    {
        localBitmap = uePerfStatsBitMap;
        pLteUeMacULPerfStats = &(pLteUeMacPerfStats[ueIndex].lteUeMacULPerfStats);
        pLteUeMacDLPerfStats = &(pLteUeMacPerfStats[ueIndex].lteUeMacDLPerfStats);
        pLteUeMacSchedDLPerfStats = &(pLteUeMacPerfStats[ueIndex].lteUeMacSchedDLPerfStats);
        pLteUeMacSchedULPerfStats = &(pLteUeMacPerfStats[ueIndex].lteUeMacSchedULPerfStats);

        /* fills the response Msg for the requested performace events bitwise per UE*/
        /* SPR 7385 Changes Starts */
        firstBitSet = FFS(localBitmap);
        /* SPR 7385 Changes Ends */
        while (firstBitSet)
        {
            switch (firstBitSet - 1)
            {
                case PERF_STATS_CQI:
                for (count = 0; count < MAX_CQI_INDEX; count++)
                {
                    pMacUeOamPerfStats[ueIndex].cqiRIPerfStats.totalCQICW1Reported[count] += 
                        pLteUeMacULPerfStats->totalCQICW1Reported[count];

                    pMacUeOamPerfStats[ueIndex].cqiRIPerfStats.totalCQICW2Reported[count] +=
                        pLteUeMacULPerfStats->totalCQICW2Reported[count];
                }
                for (count = 0; count < MAX_RANK_INDICATION; count++)
                {
                    pMacUeOamPerfStats[ueIndex].cqiRIPerfStats.totalRankReported[count] +=
                        pLteUeMacULPerfStats->totalRankReported[count];
                }
                break;

                case  PERF_STATS_HARQ:
                for (count = 0; count < MOD_SCHEME; count++)
                {
                    pMacUeOamPerfStats[ueIndex].harqPerfStats.totalDLHarqAck[count] +=
                        pLteUeMacDLPerfStats->totalDLHarqAck[count];

                    pMacUeOamPerfStats[ueIndex].harqPerfStats.totalDLHarqNack[count] +=
                        pLteUeMacDLPerfStats->totalDLHarqNack[count];

                    pMacUeOamPerfStats[ueIndex].harqPerfStats.totalULHarqSucc[count] += 
                        pLteUeMacULPerfStats->totalULHarqSucc[count];

                    pMacUeOamPerfStats[ueIndex].harqPerfStats.totalULHarqFail[count] +=
                        pLteUeMacULPerfStats->totalULHarqFail[count];
                }
                pMacUeOamPerfStats->harqPerfStats.totalHarqFail += 
                    pLteUeMacDLPerfStats->totalHarqFail;
                break;

                case PERF_STATS_UE_CAPABILITY:
                pMacUeOamPerfStats[ueIndex].ueCapabilityStats.totalTimeRestrictUeInDlbyUeCap +=
                    pLteUeMacSchedDLPerfStats->totalTimeRestrictUeInDlbyUeCap;

                pMacUeOamPerfStats[ueIndex].ueCapabilityStats.totalTimeRestrictUeInUlbyUeCap +=
                    pLteUeMacSchedULPerfStats->totalTimeRestrictUeInUlbyUeCap;
                break;

                case PERF_STATS_POWER_LIMITED:
                pMacUeOamPerfStats[ueIndex].uePowerLimitedStats.totalTbScheduledInUlOnPowerRestriction +=
                    pLteUeMacSchedULPerfStats->totalTbScheduledOnPowerRestriction;
                pMacUeOamPerfStats[ueIndex].uePowerLimitedStats.totalTbScheduledInUlOnNoPowerRestriction +=
                    pLteUeMacSchedULPerfStats->totalTbScheduledInUlOnNoPowerRestriction;
                break;

                case PERF_STATS_TX_MODE_RI:
                pMacUeOamPerfStats[ueIndex].ueTransmissionModeRIStats.totalDLTransmissionsWithTX2RI1 +=
                    pLteUeMacDLPerfStats->totalDLTransmissionsWithTX2RI1;

                for (count = 0; count < MAX_RANK_INDICATION; count++)
                {
                    pMacUeOamPerfStats[ueIndex].ueTransmissionModeRIStats.totalDLTransmissionsWithTX3RI[count] +=
                        pLteUeMacDLPerfStats->totalDLTransmissionsWithTX3RI[count];

                    pMacUeOamPerfStats[ueIndex].ueTransmissionModeRIStats.totalDLTransmissionsWithTX4RI[count] +=
                        pLteUeMacDLPerfStats->totalDLTransmissionsWithTX4RI[count];
                }
                break;

                case PERF_STATS_PUCCH_PUSCH_SINR:
                for (count = 0; count < MAX_SINR_DIFF_INDEX; count++)
                {
                    pMacUeOamPerfStats[ueIndex].ueSINRPerfStats.totalPUCCHSINRReported[count] +=
                        pLteUeMacULPerfStats->totalPUCCHSINRReported[count];

                    pMacUeOamPerfStats[ueIndex].ueSINRPerfStats.totalPUSCHSINRReported[count] +=
                        pLteUeMacULPerfStats->totalPUSCHSINRReported[count];
                }
                break;

                case PERF_STATS_DATA_VOLUME:
                pMacUeOamPerfStats[ueIndex].radioThpVolStats.totalUlByteReceived += 
                    pLteUeMacULPerfStats->totalUlBytesReceived;

                pMacUeOamPerfStats[ueIndex].radioThpVolStats.totalDlBytesSent += 
                    pLteUeMacDLPerfStats->totalDlBytesSent;

                pMacUeOamPerfStats[ueIndex].radioThpVolStats.totalUlThpPerUeTrunk += 
                    pLteUeMacULPerfStats->totalUlThpPerUeTrunk;
                break;

                case PERF_STATS_UE_PRB_USAGE:
                pMacUeOamPerfStats[ueIndex].uePRBUsageStats.totalDLREsUsage += 
                    pLteUeMacDLPerfStats->totalDLREsUsage;

                pMacUeOamPerfStats[ueIndex].uePRBUsageStats.totalREsUsedInUL += 
                    pLteUeMacULPerfStats->totalREsUsedInUL;
                break;

                default:
                break;
            }
            localBitmap = localBitmap & (~(1<<(firstBitSet - 1)));
            /* SPR 7385 Changes Starts */
            firstBitSet = FFS(localBitmap);
            /* SPR 7385 Changes Ends */
        }
    }
}

/****************************************************************************
 * Function Name  : cellFillMacOamPerfContainer 
 * Inputs         : pMacCellOamPerfStats - pointer to CellMacPerfStatsResp,
 *                  passiveIndexForMacStats,
 *                  cellPerfStatsBitMap,
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the MAC Cell Perf Container as
 *                  received from OAM.
 *****************************************************************************/
/* + PERF_CA */
void cellFillMacOamPerfContainer (CellMacPerfStatsResp *pMacCellOamPerfStats, 
                                  UInt32 passiveIndexForMacStats,
				  				  UInt32 cellPerfStatsBitMap,
								  InternalCellIndex internalCellIndex ) 
/* - PERF_CA */
{
    UInt32 count = 0;
    UInt32 firstBitSet = 0;
    UInt32 localBitmap = 0;

	/* + PERF_CA */
    LteCellMacDLPerfStats  *pLteCellMacDLPerfStats = 
        &(gPerfStats.perfStats[passiveIndexForMacStats].lteCellPerfStats[internalCellIndex].lteCellMacDLPerfStats);
    LteCellMacULPerfStats  *pLteCellMacULPerfStats = 
        &(gPerfStats.perfStats[passiveIndexForMacStats].lteCellPerfStats[internalCellIndex].lteCellMacULPerfStats);
    LteCellMacSchedDLPerfStats *pLteCellMacSchedDLPerfStats = 
        &(gPerfStats.perfStats[passiveIndexForMacStats].lteCellPerfStats[internalCellIndex].lteCellMacSchedDLPerfStats);
    LteCellMacSchedULPerfStats *pLteCellMacSchedULPerfStats = 
        &(gPerfStats.perfStats[passiveIndexForMacStats].lteCellPerfStats[internalCellIndex].lteCellMacSchedULPerfStats);
	/* - PERF_CA */

    localBitmap = cellPerfStatsBitMap;
    /* SPR 7385 Changes Starts */
    firstBitSet = FFS (localBitmap);
    /* SPR 7385 Changes Ends */
    /* Fills the response buffer according to the requested bitmap*/
    while (firstBitSet)
    {
        switch (firstBitSet - 1)
        {
            case PERF_STATS_PRB_USAGE_HARQ: 
            for(count = 0; count < MAX_QCI; count++)
            {
                pMacCellOamPerfStats->cellHarqPerfStats.totalDLPRBUsageforHarq[count] += 
                    pLteCellMacDLPerfStats->totalDLPRBUsageforHarq[count];
            }
            pMacCellOamPerfStats->cellHarqPerfStats.totalDLPRBUsageforHarqRetx +=
                pLteCellMacSchedDLPerfStats->totalDLPRBUsageforHarqRetx;
            break;

            case PERF_STATS_RACH_SUCC_FAIL:
            pMacCellOamPerfStats->rachSuccFailPerfStats.totalRcvdMsg3RandomAccessPreamblesCBRA += 
                pLteCellMacULPerfStats->totalRcvdMsg3RandomAccessPreamblesCBRA;

            pMacCellOamPerfStats->rachSuccFailPerfStats.totalRcvdMsg3RandomAccessPreamblesCFRA += 
                pLteCellMacULPerfStats->totalRcvdMsg3RandomAccessPreamblesCFRA;

            pMacCellOamPerfStats->rachSuccFailPerfStats.totalFailedRandomAccessPreamblesMsg2CBRA += 
                pLteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CBRA;

            pMacCellOamPerfStats->rachSuccFailPerfStats.totalFailedRandomAccessPreamblesMsg2CFRA += 
                pLteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA;

            pMacCellOamPerfStats->rachSuccFailPerfStats.totalAllocatedRandomAccessPreambles += 
                pLteCellMacULPerfStats->totalAllocatedRandomAccessPreambles;

            for(count = 0; count < MAX_TA_RANGE; count++)
            {
                pMacCellOamPerfStats->rachSuccFailPerfStats.totalSuccRandomAccessTA[count] += 
                    pLteCellMacULPerfStats->totalSuccRandomAccessTA[count];
            }
            pMacCellOamPerfStats->rachSuccFailPerfStats.totalUnassignedFalseCFRA += 
                pLteCellMacULPerfStats->totalUnassignedFalseCFRA;

            pMacCellOamPerfStats->rachSuccFailPerfStats.totalUnassignedSumCFRA += 
                pLteCellMacULPerfStats->totalUnassignedSumCFRA;
            break;

            case PERF_STATS_PDCCH_UTILIZATION:
            for( count = 0; count < MAX_CCE_UTIL_INDEX; count++ )
            {
                pMacCellOamPerfStats->pdcchUtilizationStats.totalPdcchCceUtil[count] += 
                    pLteCellMacSchedDLPerfStats->totalPdcchCceUtil[count];
            }
            for( count = 0; count < MAX_CFI; count++ )
            {
                pMacCellOamPerfStats->pdcchUtilizationStats.noOfSubFrameWithPdcchCfi[count] +=
                    pLteCellMacDLPerfStats->noOfSubFrameWithPdcchCfi[count];

                pMacCellOamPerfStats->pdcchUtilizationStats.noOfSubFrameWithCceUtilMoreThanZero += 
                    pLteCellMacDLPerfStats->noOfSubFrameWithPdcchCfi[count];
            }
            pMacCellOamPerfStats->pdcchUtilizationStats.totalCceAvail += 
                pLteCellMacSchedDLPerfStats->totalCceAvail;
            break;

            case PERF_STATS_PUCCH_UTILIZATION:
            pMacCellOamPerfStats->totalPUCCHResAllocationAttempts +=
                pLteCellMacSchedULPerfStats->totalPUCCHResAllocationAttempts;
            break;

            case PERF_STATS_CELL_PRB_USAGE:
            for( count = 0; count < MAX_SRB; count++)
            {
                pMacCellOamPerfStats->cellPRBUsagePerfStats.totalULPRBUsageSRB[count] +=
                    pLteCellMacULPerfStats->totalULPRBUsageSRB[count];

                pMacCellOamPerfStats->cellPRBUsagePerfStats.totalDLPRBUsageSRB[count] += 
                    pLteCellMacDLPerfStats->totalDLPRBUsageSRB[count];
            }
            pMacCellOamPerfStats->cellPRBUsagePerfStats.totalDLPRBUsagePCCH += 
                pLteCellMacSchedDLPerfStats->totalDLPRBUsagePCCH;

            pMacCellOamPerfStats->cellPRBUsagePerfStats.totalDLPRBUsageBCCH += 
                pLteCellMacSchedDLPerfStats->totalDLPRBUsageBCCH;
            pMacCellOamPerfStats->cellPRBUsagePerfStats.totalDLPRBAvailable  = \
                                                                               pLteCellMacSchedDLPerfStats->totalDLPRBAvailable;
            pMacCellOamPerfStats->cellPRBUsagePerfStats.totalULPRBAvailable  = \
                                                                               pLteCellMacSchedULPerfStats->totalULPRBAvailable;
            break;

            default:
            break;
        }
        localBitmap = localBitmap & (~(1<<(firstBitSet - 1)));
        /* SPR 7385 Changes Starts */
        firstBitSet = FFS(localBitmap);
        /* SPR 7385 Changes Ends */
    }
}


/****************************************************************************
 * Function Name  : updateMacOamPerfContainer
 * Inputs         : intId
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates the MAC Perf Container as
 *                  received from OAM.
 *****************************************************************************/
void updateMacPerfContainer(UInt32 intId)
{
	/* + PERF_CA */
	InternalCellIndex cellIndex = 0;
	/* - PERF_CA */
    /* SPR 15909 fix start */
    tickType_t macLastCalculatedTTI    =  0;
    /* SPR 15909 fix end */
    UInt32 passiveIndexForMacStats    = 0;
    UInt32 activeIndexForMacStats     =  gPerfStats.perfStatsControlInfo->macActiveIndex;
    passiveIndexForMacStats     = activeIndexForMacStats ^ 0x01; 
    /* Pointer to store the bit map */
    LtePerfStatsConfig *pLteMacPerfStatsConfig =  &(g_perfStatsScannerObj.\
            perfStatsContainer[intId].perfConfig.lteMacPerfStatsConfig);

    macLastCalculatedTTI = gPerfStats.perfStats[passiveIndexForMacStats].macLastCalculatedTTI;
    if (macLastCalculatedTTI)
    {
        /* Update all the counters. 
         * For future use: if another containers are also configured then those are  also
         * updated here.
         */

		/* + PERF_CA */
		for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
       	{ 
        cellFillMacOamPerfContainer(&(g_perfStatsScannerObj.\
            	        perfStatsContainer[intId].perfStats.lteMacPerfStats.lteMacCellPerfStats[cellIndex]),
                passiveIndexForMacStats, 
                	pLteMacPerfStatsConfig->cellPerfStatsToReportBitMap,
					cellIndex);
		}
		/* - PERF_CA */
        ueFillMacOamPerfContainer(&(g_perfStatsScannerObj.\
                    perfStatsContainer[intId].perfStats.lteMacPerfStats.lteMacUePerfStats[0]),
                passiveIndexForMacStats, 
                pLteMacPerfStatsConfig->uePerfStatsToReportBitMap);

        g_perfStatsScannerObj.perfStatsContainer[intId].perfStats.lteMacPerfStats.
            lastMACCalculatedTTI += macLastCalculatedTTI;

    }
}
/* Perf stats RRM-MAC end */

/****************************************************************************
 * Function Name  : handleMACGetOAMPerfStatsReq
 * Inputs         : msgBuf_p, transactionId and
 *                  moduleSrc,
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function will handle Perf Stats Get Req from OAM
 ******************************************************************************/
/* CA Changes start */
void  handleMACGetOAMPerfStatsReq(UInt8 *msgBuf_p,UInt8 *cnfBuff, 
                                  UInt16 transactionId,
                                  InternalCellIndex internalCellIndex)
/* CA Changes end */

{
    UInt8 *cnfBuf_p = cnfBuff;
    MacGetCellPerfStatsCnfParams *macGetCellPerfStatsCnfParams_p = PNULL;
    MacGetUePerfStatsCnfParams *macGetUePerfStatsCnfParams_p = PNULL;
    MacGetPerfStatsReqParams *macGetPerfStatsReqParams_p = PNULL;

    UInt32 perfStatsUeIndex = 0;
    UInt32 uePerfStatsBitmap =0;
    UInt32 cellPerfStatsBitmap = 0;
    UInt32 resetStats =0;
    UInt32 current = 0;

    UInt32 activeIndexForMacStats     =  gPerfStats.perfStatsControlInfo->macActiveIndex;
    UInt32 passiveIndexForMacStats    = 0;
    /* SPR 15909 fix start */
    tickType_t macLastCalculatedTTI       = 0;
    /* SPR 15909 fix end */
    /* CA Changes start */ 
    CellState cellState = cellSpecificParams_g.\
        cellConfigAndInitParams_p[internalCellIndex]->cellState; 
    /* CA Changes end */ 
    passiveIndexForMacStats     = activeIndexForMacStats;

    macGetCellPerfStatsCnfParams_p = (MacGetCellPerfStatsCnfParams *)&cnfBuf_p[MAC_API_HEADER_LEN];

    macGetPerfStatsReqParams_p = (MacGetPerfStatsReqParams *)(msgBuf_p + MAC_API_HEADER_LEN);
    resetStats = macGetPerfStatsReqParams_p->resetStats;

    cellPerfStatsBitmap  = ((macGetPerfStatsReqParams_p->cellPerfStatsToGetBitMap) & 
            (g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].\
             perfConfig.lteMacPerfStatsConfig.cellPerfStatsToReportBitMap));

    uePerfStatsBitmap  =((macGetPerfStatsReqParams_p->uePerfStatsToGetBitMap) &
            (g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].\
             perfConfig.lteMacPerfStatsConfig.uePerfStatsToReportBitMap));
    if ( cellPerfStatsBitmap || uePerfStatsBitmap ) 
    {
        /* Perf stats RRM-MAC start */
        /*Switching the buffer, Toggle */
        macLastCalculatedTTI = gPerfStats.perfStats[passiveIndexForMacStats].macLastCalculatedTTI;
        if (macLastCalculatedTTI)
        {
            gPerfStats.perfStatsControlInfo->macActiveIndex =
                passiveIndexForMacStats ^0x01;

            /* Switching counter position from passive to active */
			/* + stats_review_comments_ipc */
            gMacCellPerfStats_p[internalCellIndex] =
                &gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->macActiveIndex].lteCellPerfStats[internalCellIndex];
			/* - stats_review_comments_ipc */
            gMacUePerfStats_p =
                gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->macActiveIndex].lteUePerfStats;

            updateMacPerfContainer(PERF_STATS_CONTAINER_OAM_MAC);

            /*Update RRM-MAC container */
            if(g_perfStatsScannerObj.module_bitmask &
                    PERF_STATS_CONTAINER_RRM_MAC_BITMASK)
            {
                if(MAC_PERF_STATS ==
                        g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_RRM_MAC].macOrPdcpOrRlcStats)
                {
                    updateMacPerfContainer(PERF_STATS_CONTAINER_RRM_MAC);
                }
            }

            /* + stats_review_comments_ipc */
            macResetCellPerfStatCounters(&(gPerfStats.perfStats[passiveIndexForMacStats].lteCellPerfStats[internalCellIndex]));
            /* - PERF_CA */
            macResetUePerfStatCounters(gPerfStats.perfStats[passiveIndexForMacStats].lteUePerfStats);
            /* - stats_review_comments_ipc */
            gPerfStats.perfStats[passiveIndexForMacStats].macLastCalculatedTTI = 0;
        }

        /* Perf stats RRM-MAC end */

    }   

    /* + stats_review_comments_ipc */

    if (!cellPerfStatsBitmap)
    {
        macGetCellPerfStatsCnfParams_p->responseCode = MAC_FAILURE;
    }
    else 
    {
        /* CA Changes start */
        if(cellState >= CELL_PHY_INIT)
            /* CA Changes end */
        {
            cellMacFillPerfStats( cnfBuf_p, cellPerfStatsBitmap, 
                    UNSET_PERIODIC_FLAG, internalCellIndex);

            macGetCellPerfStatsCnfParams_p->responseCode = MAC_SUCCESS;
        }
        else 
        {
            return;
        }

    }

    current = MAC_API_HEADER_LEN + sizeof(MacGetCellPerfStatsCnfParams);
    /* Fill header information */
    prepareMacOamApiHeader( cnfBuf_p, MAC_GET_CELL_PERF_STATS_RESP,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));

    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
    /* - stats_review_comments_ipc */

    if (uePerfStatsBitmap)
    {
        for (perfStatsUeIndex = 0; perfStatsUeIndex < PERF_MAX_UE_SUPPORTED;)
        {

            /* CA Changes start */
            if(cellState >= CELL_PHY_INIT)
            {
                /* + stats_review_comments_ipc */
                ueMacFillPerfStats( cnfBuf_p,  &current, uePerfStatsBitmap, 
                        &perfStatsUeIndex, UNSET_PERIODIC_FLAG,internalCellIndex);
                /* - stats_review_comments_ipc */
                macGetUePerfStatsCnfParams_p = 
                    (MacGetUePerfStatsCnfParams*)&cnfBuf_p[MAC_API_HEADER_LEN];
                macGetUePerfStatsCnfParams_p->responseCode = MAC_SUCCESS;
                /* SPR 10577 changes start */
                macGetUePerfStatsCnfParams_p->endIndicator = FALSE;
                if((perfStatsUeIndex >= PERF_MAX_UE_SUPPORTED) || 
                        (macGetUePerfStatsCnfParams_p->numOfUePresent >= MAX_UE_PER_BATCH))
                {
                    /* endIndicator is set TRUE, as this is the end of the message */
                    macGetUePerfStatsCnfParams_p->endIndicator = TRUE;
                }
                /* SPR 10577 changes end */
            }
            else 
            { 
                return;
            }
            current += MAC_API_HEADER_LEN;

            /* Fill header information */
            prepareMacOamApiHeader( cnfBuf_p, MAC_GET_UE_PERF_STATS_RESP,
                    transactionId, current,
                    macCommonGetRRCCellIndex(internalCellIndex));

            if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff, current
                        ))
            {
                ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            }
            current = 0;
        }
    }
    else
    {
        current = MAC_API_HEADER_LEN;
        macGetUePerfStatsCnfParams_p = 
            (MacGetUePerfStatsCnfParams*)&cnfBuf_p[MAC_API_HEADER_LEN];
        macGetUePerfStatsCnfParams_p->responseCode = MAC_FAILURE;
        /* SPR 10577 changes start */
        macGetUePerfStatsCnfParams_p->endIndicator = TRUE;
        /* SPR 10577 changes end */
        current += sizeof(MacGetUePerfStatsCnfParams);

        /* Fill header information */
        prepareMacOamApiHeader(cnfBuf_p, MAC_GET_UE_PERF_STATS_RESP,
                transactionId, current,
                macCommonGetRRCCellIndex(internalCellIndex));

        if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff, current
                    ))
        {
            ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        }
    }  

    if (resetStats && (uePerfStatsBitmap || cellPerfStatsBitmap))
    {
        resetMacPerfContainer(PERF_STATS_CONTAINER_OAM_MAC_RESET_CELL_CONTAINER);
        resetMacPerfContainer(PERF_STATS_CONTAINER_OAM_MAC_RESET_UE_CONTAINER);
        g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].\
            perfStats.lteMacPerfStats.lastMACCalculatedTTI = 0;
    }
}

/****************************************************************************
 * Function Name  : updatePerfStatsForAllModules
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function will update PERF Containers.
 ****************************************************************************/
	/* + PERF_CA */
void updatePerfStatsForAllModules()
{
	InternalCellIndex cellIndex = 0;
	/* - PERF_CA */
    UInt32 modCounter                 = 0;
    /* SPR 15909 fix start */
    tickType_t macLastCalculatedTTI       = 0;
    tickType_t pdcpLastCalculatedTTI      = 0;
    tickType_t rlcLastCalculatedTTI       = 0;
    /* SPR 15909 fix end */

    UInt32 activeIndexForMacStats     =  gPerfStats.perfStatsControlInfo->macActiveIndex;
    UInt32 activeIndexForPdcpStats    =  gPerfStats.perfStatsControlInfo->pdcpActiveIndex;
    UInt32 activeIndexForRlcStats  = gPerfStats.perfStatsControlInfo->rlcDLActiveIndex;

    UInt32 passiveIndexForMacStats    = 0;
    UInt32 passiveIndexForPdcpStats   = 0;
    UInt32 passiveIndexForRlcStats    = 0;


    passiveIndexForMacStats     = activeIndexForMacStats;
    passiveIndexForPdcpStats    = activeIndexForPdcpStats;
    passiveIndexForRlcStats  = activeIndexForRlcStats;

    /*Switching the buffer, Toggle */
    macLastCalculatedTTI = gPerfStats.perfStats[passiveIndexForMacStats].macLastCalculatedTTI;
    if (macLastCalculatedTTI)
    {
        gPerfStats.perfStatsControlInfo->macActiveIndex =
            passiveIndexForMacStats ^0x01;

        /* Switching counter position from passive to active */
	
		/* + PERF_CA */
		for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
        {
			gMacCellPerfStats_p[cellIndex] =
            &gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->macActiveIndex].lteCellPerfStats[cellIndex];
		}
		/* - PERF_CA */
        gMacUePerfStats_p =
            gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->macActiveIndex].lteUePerfStats;

    }

    pdcpLastCalculatedTTI =
        gPerfStats.perfStats[passiveIndexForPdcpStats].pdcpLastCalculatedTTI;
    if (pdcpLastCalculatedTTI)
    {
        /*Switching the buffer, Toggle */
        (gPerfStats.perfStatsControlInfo->pdcpActiveIndex) =
            passiveIndexForPdcpStats ^0x01;

        /* Switching counter position from passive to active */
		/* + PERF_CA */
		for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
        {
			gPdcpCellPerfStats_p[cellIndex] =
            	&gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->pdcpActiveIndex].lteCellPerfStats[cellIndex];
		}
		/* - PERF_CA */
        gPdcpUePerfStats_p =
            gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->pdcpActiveIndex].lteUePerfStats;
    }

    rlcLastCalculatedTTI = gPerfStats.perfStats[passiveIndexForRlcStats].rlcDLLastCalculatedTTI;

    if (rlcLastCalculatedTTI)
    {
        /*Switching the buffer, Toggle */
        (gPerfStats.perfStatsControlInfo->rlcDLActiveIndex) =
            passiveIndexForRlcStats ^0x01;

        /* Switching counter position from passive to active */
		/* + PERF_CA */
		for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
        {
			gRlcCellPerfStats_p[cellIndex] =
         	   &gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->rlcDLActiveIndex].lteCellPerfStats[cellIndex];
		}
		/* - PERF_CA */
        gRlcUePerfStats_p =
            gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->rlcDLActiveIndex].lteUePerfStats;
    }

    /*Update all the containers */
    for(modCounter=0;modCounter<MAX_PERF_STATS_CONTAINER;modCounter++)
    {
        if(g_perfStatsScannerObj.module_bitmask & (1<<modCounter))
        {
            if(MAC_PERF_STATS == g_perfStatsScannerObj.perfStatsContainer[modCounter].macOrPdcpOrRlcStats)
            {
                updateMacPerfContainer(modCounter);
            }
            else if(PDCP_PERF_STATS == g_perfStatsScannerObj.perfStatsContainer[modCounter].macOrPdcpOrRlcStats)
            {
                updatePdcpPerfContainer();
            }
            else if(RLC_PERF_STATS == g_perfStatsScannerObj.perfStatsContainer[modCounter].macOrPdcpOrRlcStats)
            {
                updateRlcPerfContainer();
            }
        }
    }

	/* + PERF_CA */
	for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
    {
		macResetCellPerfStatCounters(&(gPerfStats.perfStats[passiveIndexForMacStats].lteCellPerfStats[cellIndex]));
		pdcpResetCellPerfCounters(&(gPerfStats.perfStats[passiveIndexForPdcpStats].lteCellPerfStats[cellIndex]));
    }
	macResetUePerfStatCounters(gPerfStats.perfStats[passiveIndexForMacStats].lteUePerfStats);
    gPerfStats.perfStats[passiveIndexForMacStats].macLastCalculatedTTI = 0;

    pdcpResetUePerfCounters(gPerfStats.perfStats[passiveIndexForPdcpStats].lteUePerfStats);
    gPerfStats.perfStats[passiveIndexForPdcpStats].pdcpLastCalculatedTTI = 0;
	/* - PERF_CA */

    rlcResetUePerfCounters(gPerfStats.perfStats[passiveIndexForRlcStats].lteUePerfStats);
    gPerfStats.perfStats[passiveIndexForRlcStats].rlcDLLastCalculatedTTI = 0;
    gPerfStats.perfStats[passiveIndexForRlcStats].rlcULLastCalculatedTTI = 0;


}
#endif
/* Perf stats RRM-MAC end */
/* Review comment fix start RJ25 */
/****************************************************************************
 * Function Name  : sendMacInitLayerRes
 * Inputs         : transactionId ,response
 *                  internalCellIndex - CellIndex requested
 * Outputs        : None
 * Returns        : None
 * Description    : This function send the MAC_INIT_LAYER_CNF message to OAM.
 *****************************************************************************/
/* CA Changes start */
void sendMacInitLayerRes(UInt16 transactionId, UInt32 response,
                         InternalCellIndex internalCellIndex)
{
/* CA Changes end */
    UInt16 current = 0, apiId = 0;
    /* + Coverity_Stack Fix */
    UInt8 *cnfBuff_p = &cnfBuff[0];

    /*+COVERITY 5.3.0 - 32106*/
    MacInitRespParams *macInitRespParams_p = PNULL;


    /*Prepare a response and send it to OAM*/
    if (0 == internalCellIndex)
    {
        apiId = MAC_INIT_LAYER_CNF;
    }
    else 
    {
        apiId = MAC_INIT_CELL_CNF;
    }
    current += MAC_API_HEADER_LEN;
    {
    macInitRespParams_p = (MacInitRespParams *)(cnfBuff_p + current);
    macInitRespParams_p->responseCode = response;
//phy release fix
        current += sizeof(MacInitRespParams);
    }

#ifdef LTE_EMBMS_SUPPORTED
    macInitRespParams_p->phyMbsfnCapability = phyParamResp_g[internalCellIndex].phyMbsfnCapability;
#endif
    /* Fill Msg Len in the API header */

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuff_p, apiId,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));

    LOG_MAC_INFO( MAC_OAM_INF, "MAC sends response to OAM for "
            "api[%u] resp[%u] cellId[%u]", apiId, response, 
            internalCellIndex );

    if(MAC_SEND_ERROR ==  sendMsgToOAM( 
                cnfBuff, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }

}
/* Review comment fix end RJ25 */

/* CA changes Start */
/****************************************************************************
 * Function Name  : prepareMacOamApiHeader
 * Inputs         : msgBuf - Buffer to be filled
 *                  msgId - Msg Type to be send to OAM
 *                  transactionId - Transaction Id
 *                  msgLength - Length of the message inclusing header
 *                  cellId - OAM configured cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This API takes the msgId & buffer to be filled by msg
 *                  header as input to be sent to OAM
****************************************************************************/
 void prepareMacOamApiHeader(
                UInt8 *msgBuf_p, 
                UInt16 msgId,
                UInt16 transactionId, 
                UInt16 msgLength, 
                RrcCellIndex cellId)
{
    UInt16 current = 0;

    LTE_SET_U16BIT(msgBuf_p, transactionId);
    current += U16BIT_LEN;
    LTE_SET_U16BIT(msgBuf_p + current, MAC_MODULE_ID);
    current += U16BIT_LEN;
    LTE_SET_U16BIT(msgBuf_p + current, OAM_MODULE_ID);
    current += U16BIT_LEN;
    LTE_SET_U16BIT(msgBuf_p + current, msgId);
    current += U16BIT_LEN;
    LTE_SET_U16BIT(msgBuf_p + current, msgLength);
    current += U16BIT_LEN;
    msgBuf_p[current] = cellId;

    return;
}
/* CA Changes end */

/****************************************************************************
 * Function Name  : sendMacPhyOamErrIndication
 * Inputs         : errorType, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function send the MAC_PHY_OAM_ERROR_IND message to 
 *                  OAM in case of any subframe error is received from L1.
 *****************************************************************************/
void sendMacPhyOamErrIndication( MacPhyOamSubframeErrorType errorType,
        InternalCellIndex internalCellIndex)
{
    UInt16 current = 0;
    UInt8 *indBuff_p = &cnfBuff[0];
    LP_MacPhyOamErrorInd oamErrInd_p = PNULL;
    
    /* Prepare a response message */
    current += MAC_API_HEADER_LEN;

    /* Point to actual payload */
    oamErrInd_p = (LP_MacPhyOamErrorInd)(indBuff_p + current);

    /* Fill the message body */
    oamErrInd_p->typeOfError = errorType;
    current += sizeof(MacPhyOamErrorInd);

    /* Fill header information */
    prepareMacOamApiHeader(indBuff_p, MAC_PHY_OAM_ERROR_IND,
                MAC_PHY_OAM_ERR_IND_TRANS_ID, current,
                macCommonGetRRCCellIndex(internalCellIndex));

    LOG_MAC_MSG( MAC_OAM_SEND_ERR_IND, LOGDEBUG, MAC_OAM_INF,
            getCurrentTick(), __LINE__, MAC_PHY_OAM_ERROR_IND, 
            current, oamErrInd_p->typeOfError, 0, 0,0,
            __func__, "");
    
    /* Fill the length of the message in API header and send it to OAM */
    if( MAC_SEND_ERROR == sendMsgToOAM( cnfBuff, current
        ) )
    {
        LOG_MAC_MSG( L2_SOCKET_SENDTO_FAIL_ID, LOGERROR, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, current, 0,0,0, 
                0,0, __FILE__, __func__);
        ALARM_MSG( MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
}

/****************************************************************************
 * Function Name  : sendMacInitIndication
 * Inputs         : internalCellIndex, IndBuff_p, Length of data
 * Outputs        : None
 * Returns        : None
 * Description    : This function constructs and send MAC_INIT_LAYER_IND 
 *                  message to OAM.
 *****************************************************************************/
void sendMacInitIndication( InternalCellIndex internalCellIndex,
   UInt8 *IndBuff_p, UInt16 *currentLen )
{
    UInt16 transactionId = 0;

    if( (PNULL == cellSpecificParams_g.cellConfigAndInitParams_p[0]) ||
            (CELL_INIT == cellSpecificParams_g.cellConfigAndInitParams_p[0]->cellState))
    {
        /* Set the current length of INIT_IND */
        *currentLen = MAC_API_HEADER_LEN;
        /* Fill header information */
        prepareMacOamApiHeader(IndBuff_p, MAC_INIT_LAYER_IND,
                transactionId, *currentLen, internalCellIndex );

        LOG_MAC_INFO( MAC_OAM_INF, "Sending MAC Init Indication to OAM" );
    }
}

#ifdef KPI_STATS
/* + stats_review_comments_ipc_new */
/****************************************************************************
 * Function Name  : sendMacGetKpiThpStatsCnf
 * Inputs         : msgBuf_p,
 *                  msgLen_p - Pointer to message length received,
 *                  cnfBuff_p - Pointer to the buffer to be sent to OAM,
 *                  transactionId - Stores transactionId.
 *                  msgId - Message Id received in the buffer header.
 *                  moduleTHPId
 *                  internalCellIndex - Cell id 
 * Outputs        : None
 * Returns        : None
 * Description    : This function send Throughput UE Stats to OAM attched 
 * 					to a Cell for Aperiodic reports
 *****************************************************************************/
/* +- SPR 17777 */
STATIC  void sendMacGetKpiThpStatsCnf(UInt8 *msgBuf_p,UInt8  *cnfBuff_p,
	UInt16 transactionId,UInt8 moduleTHPId,InternalCellIndex  internalCellIndex)
/* +- SPR 17777 */
{
    UInt16 kpiThpBitmap                  = 0;
    LteMacKPIConfig * pLteMacKPIConfig   = NULL;
    /* 100MS_SUPPORT : start */
    LteMacKPIConfig * pLteMacThpKPIConfig   = NULL;
    /* 100MS_SUPPORT : end */
    MacGetKpiStatsReqParams *macGetKpiStatsReqParams_p = NULL;

    pLteMacKPIConfig = &(g_kpiScannerObj.kpiStatsContainer[moduleTHPId].config.lteMacKPIConfig);
    pLteMacThpKPIConfig = &(g_kpiScannerObj.kpiStatsContainer[moduleTHPId].config.lteMacKPIConfig);
    /* + SPR_10579 Merge Fix */
    LteKPIThpStatus *pLteMacKPIThpStatus = &(g_kpiScannerObj.kpiStatsContainer[moduleTHPId].status.lteMacKPIThpStatus);
    /* - SPR_10579 Merge Fix */

    if(KPI_CONTAINER_THP_RRM_MAC == moduleTHPId)
    {
        macGetKpiStatsReqParams_p = (MacGetKpiStatsReqParams *)(msgBuf_p);
    }
    else
    {
        macGetKpiStatsReqParams_p =
            (MacGetKpiStatsReqParams *)(msgBuf_p + g_kpiModuleInfo[moduleTHPId].hdrLen);
    }

    kpiThpBitmap  = (pLteMacThpKPIConfig->kpiBitmap & macGetKpiStatsReqParams_p->kpiToReportBitMap);
    if((pLteMacKPIConfig->kpiConfigured) &&
            IS_KPI_MAC_BITSET(kpiThpBitmap, KPI_MAC_DL_LC_THROUGHPUT) &&
            (MAC_KPI_THP_STATS_IND != pLteMacKPIThpStatus->servingRespId))
    {
        /*spr 19296 fix start*/
        /* SPR 22296 Fix Start */
        macGetAndSendKpiThpStats( cnfBuff_p, transactionId, kpiThpBitmap, 
                moduleTHPId, internalCellIndex,
                g_kpiModuleInfo[moduleTHPId].aperiodicThpCnfMsgType, LTE_TRUE);
        /* SPR 22296 Fix End */
        /*spr 19296 fix end*/
    }
    return;
}
#endif	

/*SPR 11244 Changes Start */
/****************************************************************************
 * Function Name  : macIsCellRunning
 * Inputs         : Internal cell Index 
 * Outputs        : None
 * Returns        : Return Success if PHY is running, otherwise failure
 * Description    : This function checks if the PHY is runnning or not
 *****************************************************************************/
UInt32 macIsCellRunning(InternalCellIndex cellIndex)
{
    if ((CELL_PHY_RUNNING == cellSpecificParams_g.\
                cellConfigAndInitParams_p[cellIndex]->cellState) ||
            (CELL_PHY_RUNNING_CONFIG_RECVD == cellSpecificParams_g.\
             cellConfigAndInitParams_p[cellIndex]->cellState))
        return MAC_SUCCESS;
    else
        return MAC_FAILURE;
}
/*SPR 11244 Changes End */
/****************************************************************************
 * Function Name  : macSendOamFailResponse
 * Inputs         : Message ID  
 *                : Transaction Id  
 *                : Internal cell Index 
 * Outputs        : None
 * Returns        : Return Success
 * Description    : This function sends the failure response for OAM messages
 *****************************************************************************/
UInt32 macSendOamFailResponse
(
   UInt16 msgId,
   UInt16 transactionId,
   RrcCellIndex cellIndex
)
{
    UInt16 resp = MAC_LAYER_NOT_INITIALIZED;
    UInt16 current = MAC_API_HEADER_LEN, msgRespId = 0;
    UInt8 *cnfBuff_p = &cnfBuff[0];

    LOG_MAC_MSG(OAM_MAC_PARSE_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            0xFFFFFFFF,
            msgId,transactionId,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"OAM_MAC_PARSE_MSG_ID");

    memSet(cnfBuff,0,MAX_OAM_RESP_SIZE);

    /* Send response message as failure, only message to be handled for which there
     * is a response API */
    switch(msgId)
    {
        case MAC_GET_STATUS_REQ:
        {
            MacUEStatusResp *macUEStatusResp_p = (MacUEStatusResp *)(cnfBuff_p + current);
            macUEStatusResp_p->responseCode = resp;

            msgRespId = MAC_GET_STATUS_CNF;
            current += sizeof(MacUEStatusResp);
            break;
        }
        case MAC_RESET_STATS_REQ:
        {
            MacResetStatsResp *macResetStatsResp_p = (MacResetStatsResp *)(cnfBuff_p + current);
            macResetStatsResp_p->responseCode = resp;

            msgRespId = MAC_RESET_STATS_CNF;
            current += sizeof(MacResetStatsResp);
            break;
        }
        case MAC_GET_STATS_REQ:
        {
            MacStatsResp * macStatsResp_p = (MacStatsResp *)(cnfBuff_p + current);
            macStatsResp_p->responseCode = resp;

            msgRespId = MAC_GET_STATS_CNF;
            current += sizeof(MacStatsResp);
            break;
        }
        case MAC_GET_LOG_LEVEL_REQ:
        {
            MacGetLogLevelResp *macGetLogLevelResp_p = (MacGetLogLevelResp *)(cnfBuff_p + current);
            /* no response field in confirm API, set get the log level and send */
            macGetLogLevelResp_p->logLevel = get_log_level();

            msgRespId = MAC_GET_LOG_LEVEL_RESP;
            current += sizeof(MacGetLogLevelResp);
            break;
        }
        case MAC_GET_LOG_CATEGORY_REQ:
        {
            MacGetLogCategoryResp *macGetLogCategoryResp_p = (MacGetLogCategoryResp *)(cnfBuff_p + current);
            /* no response field in confirm API, set get the log level and send */
            macGetLogCategoryResp_p->logCategory = get_log_category();

            msgRespId = MAC_GET_LOG_CATEGORY_RESP;
            current += sizeof(MacGetLogCategoryResp);
            break;
        }
        case MAC_UE_SINR_TA_REQ:
        {
            MacUeSinrTaResp *macUeSinrTaResp_p = (MacUeSinrTaResp *)(cnfBuff_p + current);
            /* no response field in confirm API, set get the log level and send */
            macUeSinrTaResp_p->responseCode = resp;

            msgRespId = MAC_UE_SINR_TA_RESP;
            current += sizeof(MacUeSinrTaResp);
            break;
        }
#ifdef KPI_STATS
        case MAC_CONFIGURE_KPI_STATS_REQ:
        {
            MacConfigureKpiStatsResp *macConfigureKpiStatsResp_p = (MacConfigureKpiStatsResp *)(cnfBuff_p + current);
            macConfigureKpiStatsResp_p->responseCode = resp;

            msgRespId = MAC_CONFIGURE_KPI_STATS_CNF;
            current += sizeof(MacConfigureKpiStatsResp);
            break;
        }
        case MAC_GET_KPI_STATS_REQ:
        {
            MacGetKpiStatsResp *macGetKpiStatsResp_p = (MacGetKpiStatsResp *)(cnfBuff_p + current);
            /* no response field in confirm API, set all counters to 0 */
            memSet(macGetKpiStatsResp_p, 0, sizeof(MacGetKpiStatsResp));

            msgRespId = MAC_GET_KPI_STATS_CNF;
            current += sizeof(MacGetKpiStatsResp);
            break;
        }
#endif
#ifdef PERF_STATS
        case MAC_CONFIGURE_PERF_STATS_REQ:
        {
            MacConfigurePerfStatsCnfParams *macConfigurePerfStatsCnfParams_p = (MacConfigurePerfStatsCnfParams *)(cnfBuff_p + current);
            macConfigurePerfStatsCnfParams_p->responseCode = resp;

            msgRespId = MAC_CONFIGURE_PERF_STATS_RESP;
            current += sizeof(MacConfigurePerfStatsCnfParams);
            break;
        }
        case MAC_GET_PERF_STATS_REQ:
        {
            MacGetUePerfStatsCnfParams *macGetUePerfStatsCnfParams_p = (MacGetUePerfStatsCnfParams *)(cnfBuff_p + current);
            macGetUePerfStatsCnfParams_p->responseCode = resp;

            msgRespId = MAC_GET_CELL_PERF_STATS_RESP;
            current += sizeof(MacGetUePerfStatsCnfParams);
            break;
        }
#endif
        default:
        return MAC_SUCCESS;
    }
    /* Fill header information */
    prepareMacOamApiHeader(cnfBuff_p, msgRespId, transactionId, current, cellIndex);

    LOG_UT(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            0xFFFFFFFF,
            msgId, MAC_GET_STATUS_CNF, resp/*ERROR_INVALID_CELL_OR_STATE*/,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

    /* Send confirmation to OAM */
    if(MAC_SEND_ERROR == sendMsgToOAM( cnfBuff, current
                ))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }

    return MAC_SUCCESS;
}

#ifdef KPI_STATS
/*spr 19296 fix start*/
/****************************************************************************
 * Function Name  : macGetAndSendKpiThpStats
 * Inputs         : responseBuf - Pointer to confirmation buffer
 *                  transactionId - to be sent in API Header
 *                  kpiBitmap - Bitmap indicating KPI's to be pegged.
 *                  moduleTHPId - THP container Id
 *                  internalCellIndex - cell Id
 *                  respMsgId - Response API ID to be filled in API Header
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will fill the Throughput stats in the 
 *                  confirmation buffer for Aperiodic KPI THP reports.
 ****************************************************************************/
void macGetAndSendKpiThpStats( UInt8 *responseBuf, UInt16 transactionId,
        UInt16 kpiBitmap, UInt8 moduleThpId, InternalCellIndex internalCellIndex,
        /* SPR 22296 Fix Start */
        UInt16 respMsgId ,UInt8 resetKpi)
/* SPR 22296 Fix End */
{
    UInt8 *cnfBuf_p = responseBuf;
    DLUEContext           *ueContext_p = PNULL;
    DLLogicalChannelInfo  *dlLogicalChannel_p = PNULL;
    UInt16 current = 0;
    InternalCellIndex cellIndex = 0;
    UInt8 isAllUEInfoSent = LTE_FALSE;
    UInt8 lcId = 0;
    UInt8 entry = 0;
    UInt16 ueId = 0;

    LteKPIThpStatus *pLteMacKPIThpStatus = 
        &( g_kpiScannerObj.kpiStatsContainer[moduleThpId].status.
                lteMacKPIThpStatus );
    LteMacKPIThpStats *pLteMacKPIThpStats = 
        &( g_kpiScannerObj.kpiStatsContainer[moduleThpId].stats.
                lteMacKPIThpStats );
    /*Prepare pointer to response message data */
    MacKpiThpStatsIndParams * cnfBuff = 
        (MacKpiThpStatsIndParams *)( cnfBuf_p + 
                g_kpiModuleInfo[moduleThpId].hdrLen);

    cnfBuff->avgDurationT =  pLteMacKPIThpStats->durationTTI;
    cnfBuff->endIndicator = LTE_TRUE;

    if(!IS_KPI_MAC_BITSET(kpiBitmap, KPI_MAC_DL_LC_THROUGHPUT))
    {
        return;
    }

    do
    {
        entry = 0;

        /* Check for each UE if its eligible for KPI reporting */
        for( ;ueId < MAX_UE_SUPPORTED; ueId++ )
        {
            ueContext_p = dlUECtxInfoArr_g[ueId].dlUEContext_p;
            /* Check if the UE context is active or not */
            if(!ueContext_p || dlUECtxInfoArr_g[ueId].pendingDeleteFlag )
            {
                continue;
            }

            /* Thp stats cnf will send stats for ues attched to this cell */
            if(ueContext_p->internalCellIndex != internalCellIndex)
            {
                continue;
            }

            /* if this message is full of entries then break */
            if( ( entry >= MAX_KPI_INDEX ) ||
                    ( MAX_KPI_INDEX - entry) < MAX_LCID_THP )
            {
                /* As entry is greater than MAX_KPI_INDEX, it means
                 * more data is pending for the message.
                 * Hence, setting endIndicator as FALSE */
                cnfBuff->endIndicator = LTE_FALSE;
                break;
            }

            for (lcId = 0; lcId < MAX_LCID_THP; lcId++)
            {
                dlLogicalChannel_p = &ueContext_p->logicalChannel[lcId+3];
                if(INVALID_LCID == dlLogicalChannel_p->logicalChannelId)
                {
                    continue;
                }
                /* Fill the KPI values for this LC */
                MacUpdateKpiThpStatsForLc( ueId, lcId, entry, dlLogicalChannel_p->dlLcQosInfo.QCI,
                        pLteMacKPIThpStats, cnfBuff);
                entry++;
            }
        }
        /* SPR23194 start */
        if ( KPI_CONTAINER_THP_OAM_MAC == moduleThpId )
        {
            /* Fill the stats for Deleted LC which cannot be captured in above
             * loop as the UE context is not available */
            if ( ueId >= MAX_UE_SUPPORTED )
            {
                UInt8 numDelEntry = macNumQciEntries( moduleThpId );
                if( 0 != numDelEntry )
                {
                    /* Since entry value is already incremented so check if
                     * (numDelEntry - 1) entries can be admitted */
                    if( MAX_KPI_INDEX >= ( entry + numDelEntry ) )
                    {
                        macFillStatsOfDeletedLc(moduleThpId, cnfBuff, &entry);
                        isAllUEInfoSent = LTE_TRUE;
                    }
                    else
                    {
                        /* As entry is greater than MAX_KPI_INDEX and deleted
                         * UE stats are still not sent, set endIndicator to
                         * FALSE */
                        cnfBuff->endIndicator = LTE_FALSE;
                    }
                }
                else
                {
                    /* No deleted RAB entry and all UE information is sent */
                    isAllUEInfoSent = LTE_TRUE;
                }
            }
        }
        else
        {
            /* Incase the THP stats are for RRM, and for all UEs data is sent,
             * set isAllUEInfoSent 
             * Check if for all UEs data is sent, set isAllUEInfoSent */
        if( MAX_UE_SUPPORTED <= ueId )
        {
            isAllUEInfoSent = LTE_TRUE;
        }
        }

        /* SPR23194 End */

        /* Set the last entry as INVALID to demarkate the end */
        if( MAX_KPI_INDEX > entry )
        {
            memSet( (void *)(&cnfBuff->thpVolDl[entry]), 0xff,
                    ( sizeof(UInt64) * (MAX_KPI_INDEX-entry) ) );
            memSet( (void *)(&cnfBuff->thpVolUl[entry]), 0xff,
                    ( sizeof(UInt64) * (MAX_KPI_INDEX-entry) ) );
            memSet( (void *)(&cnfBuff->thpTimeDl[entry]), 0xff,
                    ( sizeof(UInt32) * (MAX_KPI_INDEX-entry) ) );
            memSet( (void *)(&cnfBuff->thpTimeUl[entry]), 0xff,
                    ( sizeof(UInt32) * (MAX_KPI_INDEX-entry) ) );
            memSet( (void *)(&cnfBuff->latTime[entry]), 0xff,
                    ( sizeof(UInt32) * (MAX_KPI_INDEX-entry) ) );
            memSet( (void *)(&cnfBuff->latSample[entry]), 0xff,
                    ( sizeof(UInt32) * (MAX_KPI_INDEX-entry) ) );
        }

        /* Reset the KPI Container when all the information is sent for all 
         * UEs */
        /* SPR 22296 Fix Start */
        /* SPR 22882 Start */
        if  (LTE_TRUE == isAllUEInfoSent) 
        {
            cnfBuff->endIndicator = LTE_TRUE;

        }
        /* SPR 22882 End */
        if ( (LTE_TRUE == isAllUEInfoSent) && (LTE_TRUE == resetKpi) )
        /* SPR 22296 Fix End */
            {
                pLteMacKPIThpStatus->servingRespId     = 0;
                pLteMacKPIThpStatus->moduleBitmap &= ~(0x01 << moduleThpId);
                /* Reset THP stats does not use cell index, so pass any value */
                for(cellIndex = 0 ; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
                {
                    resetKpiContainer( moduleThpId,cellIndex );
                }

            /* The end indicator in cnfBuff is set TRUE here which means that 
             * this is the end of the message  */
        /* SPR 22882 +- */
            /* code removed */
        }

        /* Fill Msg Len in the API header */
        current += sizeof(MacGetKpiThpStatsResp);
        current += g_kpiModuleInfo[moduleThpId].hdrLen;

        /* Fill header information */
        prepareMacOamApiHeader( cnfBuf_p, respMsgId,
                transactionId, g_kpiModuleInfo[moduleThpId].hdrLen,
                macCommonGetRRCCellIndex(internalCellIndex) );

        LOG_MSG(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                MAC_GET_KPI_STATS_REQ,MAC_GET_KPI_THP_STATS_CNF,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");

        /*Send confirmation to OAM */
        if(MAC_SEND_ERROR == sendMsgToOAM((UInt8 *)responseBuf, current
                    ))
        {
            ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        }

    } while( LTE_FALSE == cnfBuff->endIndicator );
}
/*spr 19296 fix end*/
#endif

/*****************************************************************************
 * Function Name  : storePrimaryDCIFormat
 * Inputs         : macInitReqParams_p - mac parameter
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : Store th DCI fomat in mac parameters
 ******************************************************************************/
UInt32 storePrimaryDCIFormat(MacInitReqParams *macInitReqParams_p)
{
        
    /* + CQI_5.0 */
    UInt8 counter = 0;
    UInt8 primaryDCIFormat = 0;
    /* - CQI_5.0 */
	primaryDCIFormat =
		macInitReqParams_p->preferredPrimaryDCIFormatForSingleTB[counter];

	counter++;
	switch (primaryDCIFormat)
	{
		case 1:
			macParams_g.preferredPrimaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1;
			macParams_g.preferredSecondaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1A;

			break;

		case 2:
			macParams_g.preferredPrimaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1A;
			macParams_g.preferredSecondaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1;
			break;

		default:
			lteWarning ("Received invalid primary DCI format value in SISO"
					"from OAM,preferredPrimaryDCIFormatForSingleTB = %d\n",
					primaryDCIFormat);
			return MAC_FAILURE;
			break;
	}


	/* Validating and storing operator preferred primary DCI format for transmit
	 *          * diversity*/
	primaryDCIFormat =
		macInitReqParams_p->preferredPrimaryDCIFormatForSingleTB[counter];

	counter++;
	switch (primaryDCIFormat)
	{
		case 1:
			macParams_g.preferredPrimaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1;
			macParams_g.preferredSecondaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1A;
			break;

		case 2:
			macParams_g.preferredPrimaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1A;
			macParams_g.preferredSecondaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1;
			break;

		default:
			lteWarning ("Received invalid primary DCI format value in Transmit"
					"diversity from OAM,preferredPrimaryDCIFormatForSingleTB = %d\n",
					primaryDCIFormat);
			return MAC_FAILURE;
			break;
	}


	/* Validating and storing operator preferred primary DCI format for open
	 *          * loop MIMO */
	primaryDCIFormat =
		macInitReqParams_p->preferredPrimaryDCIFormatForSingleTB[counter];

	counter++;

	switch (primaryDCIFormat)
	{
		case 2:
			macParams_g.preferredPrimaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1A;
			macParams_g.preferredSecondaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_2A;
			break;

		case 7:
			macParams_g.preferredPrimaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_2A;
			macParams_g.preferredSecondaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1A;
			break;

		default:
			lteWarning ("Received invalid primary DCI format value in open loop"
					"MIMO from OAM, preferredPrimaryDCIFormatForSingleTB = %d\n",
					primaryDCIFormat);
			return MAC_FAILURE;
			break;
	}


	/* Validating and storing operator preferred primary DCI format for Closed
	 *          * loop MIMO */
	primaryDCIFormat =
		macInitReqParams_p->preferredPrimaryDCIFormatForSingleTB[counter];

	counter++;
	switch (primaryDCIFormat)
	{
		case 2:
			macParams_g.preferredPrimaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1A;
			macParams_g.preferredSecondaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_2;
			break;

		case 6:
			macParams_g.preferredPrimaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_2;
			macParams_g.preferredSecondaryDCIFormatForSingleTB[counter] =
				DCI_FORMAT_1A;
			break;

		default:
			lteWarning ("Received invalid primary DCI format value in closed loop"
					"MIMO from OAM, preferredPrimaryDCIFormatForSingleTB = %d\n",
					primaryDCIFormat);
			return MAC_FAILURE;
			break;

    }
        return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : updateInfoForInitReqRcvd
 * Inputs         : resp : Response after process API
 *                  internalCellIndex : Internal cell Id
 * Outputs        : None
 * Returns        : None 
 * Description    : This function will check resp type and then
 *                  revert cell mapping if  last attempt to
 *                  API is failed. This will start schedular and take memory for 
 *                  successful inits.
 ******************************************************************************/
STATIC  void   updateInfoForInitReqRcvd(UInt16 resp,
                                              InternalCellIndex internalCellIndex)
{
    if ((resp != MAC_LAYER_ALREADY_INITIALIZED) && 
            (resp != MAC_INIT_CELL_REQ_MEMORY_FAILURE))
    {
        if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->isLastAttempt == 1)
        {
            FREE_MEMORY_FOR_CELL_INIT_PARAMS(internalCellIndex);
            macCommonRevertCellIndexMapping(internalCellIndex);
        }

        if ((cellSpecificParams_g.numOfCells) && 
                (macParams_g.numInitReceived == (cellSpecificParams_g.numOfCells 
                                                 )))
        {
            /* All memory pools, queues based on number of configured cells*/
            initializeMAC(cellSpecificParams_g.numOfCells
                    );
            initExecutionScheduler(cellSpecificParams_g.numOfCells 
                    );
            createExecutionThreads();
        }
    }
}


/* Cyclomatic_complexity_changes_end */
/*spr 22474 start*/
/****************************************************************************
 *  Function Name  : handleMACGetDebugInfoReq 
 *  Inputs         : msgBuf_p - poiter to buffer recevied in request,
 *                   transactionId,
 *                   internalCellIndex - cell id  
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS/MAC_FAILURE
 *  Description    : This function will handle Debug info request from OAM
 *  **************************************************************************/
MacRetType handleMACGetDebugInfoReq(
    UInt8 *msgBuf_p,
    UInt16 transactionId,
    InternalCellIndex internalCellIndex)
{
    MacRetType respCode    = MAC_SUCCESS;
    
    MacGetDebugInfoReqParams *macGetDebugInfoReqParams_p =
        (MacGetDebugInfoReqParams *)(msgBuf_p + MAC_API_HEADER_LEN);

    switch(macGetDebugInfoReqParams_p->debugType)
    {
        case GET_DEBUG_INFO_MEM_STATS:
        {
            respCode = macSendMemPoolStats(transactionId, internalCellIndex);
            break;
        }
        case GET_DEBUG_INFO_MSG_STATS:
        {
            respCode = macSendMsgPoolStats(transactionId,internalCellIndex);
            break;
        }        
        default:
        {
            LOG_MAC_WARNING( MAC_OAM_INF, 
                    "Invalid value of debugType:%u received in MAC_GET_DEBUG_INFO_REQ",
                    macGetDebugInfoReqParams_p->debugType);
            MacGetDebugInfoReqParams *macGetDebugInfoReqParams_p =
                        (MacGetDebugInfoReqParams *)(msgBuf_p + MAC_API_HEADER_LEN);
            MacGetDebugInfoRespParams *macGetDebugInfoRespParams_p = PNULL;
            UInt16 current = MAC_API_HEADER_LEN; 
            macGetDebugInfoRespParams_p = (MacGetDebugInfoRespParams *)(cnfBuff + current);
            macGetDebugInfoRespParams_p->responseCode = MAC_FAILURE;
            macGetDebugInfoRespParams_p->debugType = macGetDebugInfoReqParams_p->debugType; 
            prepareMacOamApiHeader(cnfBuff, MAC_OAM_GET_DEBUG_INFO_RESP,
                    transactionId, current,
                    macCommonGetRRCCellIndex(internalCellIndex));
            if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff, current))
            {
                ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
                respCode = MAC_FAILURE;
            }

            break;
        }
    }

    return respCode;
}

/****************************************************************************
 *  * Function Name  : macSendMemPoolStats
 *  * Inputs         : transactionId
 *  * Outputs        : sends the mempool stats to OAM
 *  * Returns        : SUCCESS/FAILURE
 *  * Description    : This function updates the stats of mempool
 *  ****************************************************************************/
MacRetType macSendMemPoolStats
(
    UInt16 transactionId,
    InternalCellIndex internalCellIndex
)
{
    MacRetType retVal = MAC_SUCCESS;
    UInt16 current = MAC_API_HEADER_LEN;
    MacGetDebugInfoRespParams *macGetDebugInfoRespParams_p = PNULL;
    
    macGetDebugInfoRespParams_p = (MacGetDebugInfoRespParams *)(cnfBuff + current);
    macGetDebugInfoRespParams_p->debugType = GET_DEBUG_INFO_MEM_STATS;

    /* Get the mempool stats */
    L2PrepareMemPoolStats (&(macGetDebugInfoRespParams_p->u.csplMemPoolStats));
	
    current += sizeof(MacGetDebugInfoRespParams);
    macGetDebugInfoRespParams_p->responseCode = retVal;

    /* Fill header information */
    prepareMacOamApiHeader(cnfBuff, MAC_OAM_GET_DEBUG_INFO_RESP,
            transactionId, current,
            macCommonGetRRCCellIndex(internalCellIndex));

    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff, current))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        retVal = MAC_FAILURE;
    }

    return retVal;
}

/****************************************************************************
 *  * Function Name  : macSendMsgPoolStats
 *  * Inputs         : transactionId
 *  * Outputs        : sends the mempool stats to OAM
 *  * Returns        : SUCCESS/FAILURE
 *  * Description    : This function updates the stats of msgpool
 *  ****************************************************************************/
MacRetType macSendMsgPoolStats
(
    UInt16 transactionId,
    InternalCellIndex internalCellIndex
)
{
    MacRetType retVal = MAC_SUCCESS;
    UInt16 current = MAC_API_HEADER_LEN;
    //UInt32 poolIndex=0, bufSize = 0, validPool = 0;
    MacGetDebugInfoRespParams *macGetDebugInfoRespParams_p = PNULL;

    macGetDebugInfoRespParams_p = (MacGetDebugInfoRespParams *)(cnfBuff + current);
    macGetDebugInfoRespParams_p->debugType = GET_DEBUG_INFO_MSG_STATS;

    /* Get the msgpool stats */
    L2PrepareMsgPoolStats (&(macGetDebugInfoRespParams_p->u.csplMsgPoolStats));

    current += sizeof(MacGetDebugInfoRespParams);
    macGetDebugInfoRespParams_p->responseCode = retVal;
    
    /* Fill header information */
    prepareMacOamApiHeader(cnfBuff, MAC_OAM_GET_DEBUG_INFO_RESP,
                transactionId, current,
                macCommonGetRRCCellIndex(internalCellIndex));
    
    if(MAC_SEND_ERROR == sendMsgToOAM(cnfBuff, current))
    {
        ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        retVal = MAC_FAILURE;
    }

    return retVal;
}
/*spr 22474 end*/
