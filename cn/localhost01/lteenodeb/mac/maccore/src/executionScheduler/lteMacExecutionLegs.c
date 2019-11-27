/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacExecutionLegs.c,v 1.3.6.1.8.1.14.1 2010/11/08 06:06:21 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacExecutionLegs.c,v $
 * Revision 1.3.6.1.8.1.14.1  2010/11/08 06:06:21  gur10121
 * Semi static changes merging
 *
 * Revision 1.3.6.1.8.1.8.2  2010/10/29 15:09:38  gur10121
 * PHY_DL_DELAY introduced in execution LEG
 *
 * Revision 1.3.6.1.8.1.8.1  2010/10/26 11:09:08  gur10121
 * *** empty log message ***
 *
 * Revision 1.3.6.1.8.1  2010/09/21 15:46:04  gur20491
 * FAPI changes
 *
 * Revision 1.3.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.59  2009/12/30 16:13:49  gur15697
 * PROCESS_UL_RA_BSR changed to PROCESS_UL_RA
 *
 * Revision 1.58  2009/12/28 05:58:39  gur11912
 * modified triggerSubFrameInd
 *
 * Revision 1.57  2009/12/27 12:09:14  gur11912
 * added FOUR_FLOW_ONE_DELAY_ALTERNATE model
 *
 * Revision 1.56  2009/12/23 11:13:29  gur11912
 * modified FOUR_FLOW_ONE_DELAY model
 *
 * Revision 1.55  2009/12/18 10:45:46  gur11912
 * added DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY
 *
 * Revision 1.54  2009/11/29 09:25:33  gur11912
 * called PROCESS_N_DEMUX_QUEUE instead of PROCESS_DEMUX_QUEUE
 *
 * Revision 1.53  2009/11/20 06:12:47  gur11912
 * passed system Frame and Sub Frame in txDLControl, data and common channel legs
 *
 * Revision 1.52  2009/11/02 08:25:55  gur11912
 * modified FOUR_FLOW_ONE_DELAY model
 *
 * Revision 1.51  2009/11/01 11:38:30  gur11912
 * NUM_CQI_NODES added
 *
 * Revision 1.50  2009/10/25 11:06:07  gur11912
 * DL and UL stats functionality separated
 *
 * Revision 1.49  2009/10/15 03:50:54  gur11912
 * modified four thread model
 *
 * Revision 1.48  2009/09/22 16:24:36  gur19413
 * *** empty log message ***
 *
 * Revision 1.47  2009/09/22 10:03:49  gur19413
 * changed in execution legs.
 *
 * Revision 1.46  2009/09/14 09:42:48  gur11912
 * modified for UE addition at startup
 *
 * Revision 1.45  2009/09/14 07:08:52  gur19413
 * modified cellConfigured to cellConfigured_g
 *
 * Revision 1.44  2009/09/14 05:23:21  gur11912
 * Updated Four Thread model
 *
 * Revision 1.43  2009/08/30 23:17:21  gur19413
 * added bind mask
 *
 * Revision 1.42  2009/08/30 12:54:27  gur19413
 * added execution legs for stats
 *
 * Revision 1.41  2009/08/27 14:34:16  gur11912
 * Removed Leg TX_COMMON_CHANNEL_DATA
 *
 * Revision 1.40  2009/08/18 16:19:00  gur11912
 * modified for thread synchronization
 *
 * Revision 1.39  2009/08/03 06:15:39  gur18550
 * Expire All Tick Functionality Updated
 *
 * Revision 1.38  2009/07/28 15:09:27  gur11912
 * Updated FOUR Thread Model
 *
 * Revision 1.37  2009/07/27 15:13:35  gur19413
 * Num of subframes changed in HarqTimerExpiry Legs.
 *
 * Revision 1.36  2009/07/23 15:26:58  gur18569
 * made first argument to PROCESS harq timer expiry leg to 1
 *
 * Revision 1.35  2009/07/23 14:02:36  gur18569
 * changes in four thread flow
 *
 * Revision 1.34  2009/07/23 12:30:11  gur15697
 * Execution leg for Stats added
 *
 * Revision 1.33  2009/07/23 08:59:12  gur18569
 * changes in FOUR Flow one delay
 *
 * Revision 1.32  2009/07/19 12:34:38  gur15697
 * TWO_FLOW_ONE_DELAY thread model modified
 *
 * Revision 1.31  2009/07/17 12:25:30  gur15697
 * TWO_FLOW_ONE_DELAY execution legs modified
 *
 * Revision 1.30  2009/07/17 10:14:20  gur11912
 * Added four flow one delay model
 *
 * Revision 1.29  2009/07/16 14:03:34  gur18569
 * changes for waiting for tick logic
 *
 * Revision 1.28  2009/07/14 11:59:50  gur18569
 * removed compilation errors for 2 and 4 flows
 *
 * Revision 1.27  2009/07/14 06:09:18  gur18550
 * Added PDCCH_DELAY_THREE
 *
 * Revision 1.25  2009/07/10 12:07:05  gur18569
 * removed compilation warnings
 *
 * Revision 1.24  2009/07/07 15:14:29  gur18569
 * processPUSCHLogicForDelayPath2 passing execLegFuncArg_p->delayToPhy instead of 1 and 2
 *
 * Revision 1.23  2009/07/07 12:57:05  gur18569
 * changes in exec legs on one flow
 *
 * Revision 1.22  2009/07/07 12:05:10  gur18569
 * removed leg PROCESS_RX_CONTROL_DATA_WITHOUT_DEMUX from one flow since we are calling RX until subframe ind
 *
 * Revision 1.21  2009/07/06 11:58:09  gur18569
 * calling tx common channel data leg before tx data msg leg
 *
 * Revision 1.20  2009/07/06 10:02:49  gur18569
 * made PROCESS_RX_UNTIL_SUBFRAME_IND leg as the last one
 *
 * Revision 1.19  2009/07/02 15:11:21  gur18569
 * added leg to send only data msgs
 *
 * Revision 1.18  2009/07/02 08:03:56  gur18569
 * changed phy delay in one thread implementation to 0
 *
 * Revision 1.17  2009/07/02 07:53:27  gur18569
 * added legs tx common channel data and process pending deleteUE
 *
 * Revision 1.16  2009/06/30 10:28:48  gur18569
 * added nonzero check before decrementing numOfSubFrames for strategy legs
 *
 * Revision 1.15  2009/06/29 09:45:35  gur18569
 * changes start exec leg for ONE flow model
 *
 * Revision 1.14  2009/06/29 04:12:52  gur18569
 * disabled test flow
 *
 * Revision 1.13  2009/06/25 13:31:03  gur18569
 * adjustments in numOfSubframes after which strategy is to be called
 *
 * Revision 1.12  2009/06/24 10:31:40  gur18569
 * change in signature of processOAMAndRRCMessages ,hence change in invoking statement
 *
 * Revision 1.11  2009/06/24 08:40:46  gur18569
 * changes regarding numberOfSubFrames in harq timer expiry leg
 *
 * Revision 1.10  2009/06/22 06:44:29  gur18569
 * added DL flow legs in TEST flow
 *
 * Revision 1.9  2009/06/18 07:50:50  gur18569
 * changes in test execlegs
 *
 * Revision 1.8  2009/06/17 05:00:55  gur18569
 * added an arg to mux leg , passing it delayto phy also
 *
 * Revision 1.7  2009/06/15 09:42:17  gur18569
 * changes in test exec flow for testing
 *
 * Revision 1.6  2009/06/11 08:36:43  gur18569
 * changes in signature of txDLControlMsg
 *
 * Revision 1.5  2009/06/10 11:40:03  gur18569
 * removed linking error
 *
 * Revision 1.4  2009/06/10 11:28:38  gur18569
 * incorporation of comments
 *
 * Revision 1.3  2009/06/09 13:11:04  gur18569
 * incorporated comments
 *
 * Revision 1.2  2009/06/03 10:42:49  gur18569
 * incorporated comments
 *
 * Revision 1.1  2009/05/29 14:26:27  gur18569
 * Initial version
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/




/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacExecutionLegs.h"
#include "lteMisc.h"
#include "lteMacEncoder.h"
#include "lteMacDemux.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacDLHarqMgr.h"
#include "lteMacCQI.h"
#include "lteMacRandomAccess.h"
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacDLQosScheduler.h"
#include "lteMacReceiver.h"
#include "lteMacTransmitter.h"
#include "lteMacMultiplexer.h"
#include "lteMacPUSCH.h"
#include "lteShmLogger.h"
#include "lteMacStatsManager.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacULCSIManager.h" 
#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"
#include "lteMacTypes.h"
#include "lteMacTDDConfig.h"
#endif
#include "lteRelay.h"
/* SPR 21804 Fix Start */
#if (defined (LAYER2_DLTG_THREAD)) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
#include "lteLayer2DLTG.h"
#endif
#include "lteMacPhyCellReconfig.h"
#include "lteCircQueue.h"
#include "lteMacDLSpsScheduler.h"
#include "lteMacULSpsScheduler.h"
#include "lteMacSpsPDSCHmgr.h"
#include "lteMacSpsPUSCHmgr.h"
#include "lteMacRRMPeriodicMgr.h"
#include "lteMacRRMIpcPort.h"
#include "logging.h"
#include "lteMacPRSMgr.h"

#include "lteMacULQosScheduler.h"
#ifdef LTE_EMBMS_SUPPORTED
#include "lteLayer2EmbmsMacRlcInterface.h"
#include "lteMacEmbmsEncoder.h"
#endif
#ifdef DL_DATA_SEPARATION
#include "lteRlcErrorCode.h"
#endif
/* SPR 23402 Changes Start */
/* MOCN CHANGES START */
#include "lteMacOAMInterface.h"
/* MOCN CNANGES END */
/* SPR 23402 Changes End */
/******************************************************************************
 Private Definitions
 *****************************************************************************/

/*** SPR 2934 **/
#define MAC_MEM_POOL_HIGH_WATERMARK         90
#define MAC_MEM_POOL_LOW_WATERMARK          80
#define MAC_MSG_POOL_HIGH_WATERMARK         90
#define MAC_MSG_POOL_LOW_WATERMARK          80

#define SOCKET_BUFFER_SIZE 4*1024

/*** SPR 2934 **/
/******************************************************************************
 Private Types
 *****************************************************************************/
/* CA changes start */
extern RRMMACCellGlobal *rrmMacCellGlobal_gp[MAX_NUM_CELL];
/* CA changes end */
/*Changes DROP1*/
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
extern LTE_CIRCQUEUE rcvdRRCOAMMsg_g;
#endif
/*SPR 16855 +-*/
extern RRC_OAM_LPTOHP_THDH_PRIO_TYPE rrcOamLPtoHPThdHPrioQ_g[MAX_NUM_CELL][MAX_THD];
extern RRC_OAM_HPTOLP_THDH_PRIO_TYPE rrcOamHPtoLPThdHPrioQ_g[MAX_NUM_CELL][MAX_THD];
/*SPR 16855 +-*/
#ifdef OVERSHOOT_CORRECTION
/** CA2 Changes Start **/
void setOverLoadLevel(OverLoadLevel tickOverLoadLevel,InternalCellIndex cellIndex);
/** CA2 Changes Start **/
#endif
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
extern mocnOperatorParams mocnOperatorParams_g;
extern void calculateMocnPRBUsageHistory(InternalCellIndex internalCellIndex);
/* MOCN Changes End */
/* SPR 23402 Changes End */
 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
UInt32 checkForPDCCHReadyTrigger(void);
UInt32 checkDLControlMsgTrigger(void);
void triggerSubframeInd(void);
/* - SPR 17439 */
/* SPR 5599 changes end (ZIP ID 129063) */


/* TDD Config 0 changes start */
#ifdef TDD_CONFIG
void ElInvokeMergeULStrategyNodes(ExecutionLegFunctionArg * execLegFuncArg_p);
#endif
/* TDD Config 0 changes end */

/* TDD UL Stall Fix Starts */
/* + SPR 17439 */
extern UInt32 rlcCheckTimerExpiry(InternalCellIndex internalCellIndex);
/* - SPR 17439 */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
/** CA Changes Start **/
extern void rlcCheckTxTimerExpiry(InternalCellIndex cellIndex);
extern void rlcCheckRxTimerExpiry(InternalCellIndex cellIndex);
/** CA Changes End **/
/*Change DROP1*/
extern SInt32 rxSockFD_g;
/*CA Changes start  */
extern LTE_SQUEUE *macResetQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE* pendingUEDeleteQueue_gp[MAX_NUM_CELL];

/*CA Changes end  */
UInt8 processRRCOAMFlag_g = FALSE;
UInt8 processUeDelFlag_g[MAX_NUM_CELL] = {FALSE};
UInt8 processUeResetFlag_g[MAX_NUM_CELL] = {FALSE};
#endif
MacProcessRrcOamMsgEvent rrcOamDlEventId_g[MAX_NUM_CELL];
MacProcessRrcOamMsgEvent rrcOamUlEventId_g[MAX_NUM_CELL];

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
extern UInt32 rlcProcessReEstabMsg(LTE_CIRCQUEUE *reEstabQ_p);
extern UInt32 rlcProcessTxReconfigMsg(LTE_CIRCQUEUE *rlcReconfigTxEntityQ_p);
#endif

extern UInt16 totalNumberOfTickDiffDl_g[MAX_NUM_CELL]; 
extern UInt16 totalNumberOfTickDiffUl_g[MAX_NUM_CELL]; 
extern UInt16 totalTickMissErrIndDl_g[MAX_NUM_CELL];
extern UInt16 totalTickMissErrIndUl_g[MAX_NUM_CELL];

#ifdef TDD_CONFIG
UInt8 specialSubFrameConfig_g;
UInt8 ulDlCofiguration_g;
void ElPdcpTxMessage(ExecutionLegFunctionArg * execLegFuncArg_p);
extern UInt32 ElRlcProcessOAMAndRRCMsgFSM(UInt8* recvBuffer_p, SInt32 byteRead);
#endif

/*CA Changes start  */
extern ULStrategyScheduleInfo* ulStrategyScheduleInfo_gp[MAX_NUM_CELL];
/*CA Changes end  */
#ifdef UTFWK_SIMULATION
TimeSpec     prev_currentTime;
UInt32 ctr1 = 0;
UInt32 ctr2 = 0;
UInt32 ctr3 = 0;
#endif

/* + CQI_4.1 */
/*CA Changes start  */
DL_SCHEDULER_REPORT_QUEUE_TYPE* dlSchedulerReportQueue_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */
/* - CQI_4.1 */

/*** SPR 2934 **/
UInt8 memoryOverflow_g;
UInt8 msgOverflow_g;
/*** SPR 2934 **/
/** CA Phase2 Changes Start **/
#if (!defined (DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
void resetPrevTickEventFdContainer(InternalCellIndex cellIndex);

#if (!defined(MUTEX_SYNC) && !defined(SEM_SYNC))
/* + SPR 13643 Changes */
extern SInt32 highestEventFD_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
/* - SPR 13643 Changes */
#endif
extern LTE_SEM cellSyncSem_g;
#endif

/** CA Phase2 Changes End **/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern void ElProcessOAMAndRRCMsgFSM(UInt32 delayToPhy, UInt8* recvBuffer_p, UInt16 byteRead);

extern void lteRelaySockHandler(UInt8 *rcvBuffer,SInt32 bytesRead);
extern UInt32 ReadFromSocket(UInt8 *buf, UInt16 moduleId);
extern SInt32 recvFromRelaySockHandler(UInt8 *rcvBuffer,UInt32 rcvBuffer_len, UInt32 moduleId,SockAddr *sockAddr);
extern relayConfigInfo relayConfigInfo_g;

 /* TDD FrameWork Changes Start */
#ifdef TDD_CONFIG
extern UInt32 rlcHandleSocketMsgs( UInt8 *, SInt32);
extern UInt8 * rlcReceiveSocketMsgs(SInt32* bytesRead);
#endif

#define RLC_FAILURE 0

extern void handleProcessRxSubFrameInd(UInt32 dlDelay,UInt32 threadIndex);

extern UInt16 pdcpContextIsInitialized(void);
/* CA Changes start */
extern MAC_UE_OPPOR_IND_CIRC_TYPE* macUeOpporIndCircQ_gp[MAX_NUM_CELL];
extern MAC_UE_DATA_IND_TYPE macUeDataIndQ_g[MAX_NUM_CELL];
/* CA Changes end */
extern UInt16 rlcLayerInitialized_g;
/* + SPR 17439 */
extern UInt32 pdcpElTxProcessSrbMessage(void);
extern UInt32 handleExpiredTimer(void);
extern UInt32 handleDataFromMac(UInt32 count);
extern LTE_SQUEUE rlcUeDataReqQ_g;
extern void timerUpdateTickCheckForTmrEx(void);
/* - SPR 17439 */
/*SPR 16855 Fix Start*/
#ifdef UE_SIM_TESTING
/* SPR 3805 Fix start */
extern LTE_CIRCQUEUE rlcRxUePendingDelQ_g;
extern LTE_CIRCQUEUE rlcTxUePendingDelQ_g;
/* SPR 3805 Fix end */
/* RLC Queues for Pending Deletion and Reestablishment of Entities */
extern LTE_SQUEUE rlcRxPendingDelQ_g;
extern LTE_SQUEUE rlcRxReEstabQ_g;
extern LTE_SQUEUE rlcTxPendingDelQ_g;
extern LTE_SQUEUE rlcTxReEstabQ_g;
#else
extern LTE_CIRCQUEUE rlcRxPendingDelQ_g;
extern LTE_CIRCQUEUE rlcRxReEstabQ_g;
extern LTE_CIRCQUEUE rlcTxPendingDelQ_g[MAX_NUM_CELL];
extern LTE_CIRCQUEUE rlcTxReEstabQ_g[MAX_NUM_CELL];
extern LTE_CIRCQUEUE rlcReconfigTxEntityQ_g;
extern LTE_CIRCQUEUE rlcReconfigRxEntityQ_g;
#endif
/*SPR 16855 Fix End*/
extern LTE_SQUEUE rlcTmrExTxsQ_g ;
extern LTE_SQUEUE rlcControlPduQ_g ;
extern LTE_SQUEUE macUeHarqFailureIndQ_g;

/* + SPR 9911 Fix */
#ifndef UE_SIM_TESTING 
extern LTE_CIRCQUEUE rlcTriggerStatusReportQueue_g[MAX_NUM_CELL];
extern void processRlcTriggerStatusReport(UInt32 cQcount,InternalCellIndex cellIndex);
#endif
/* - SPR 9911 Fix */
/* + SPR 17439 */
/** CA Changes Start **/
#ifdef UE_SIM_TESTING 
UInt32 rlcHandleTxOppurtunity(void);
/* - SPR 17439 */
#else
#ifndef DL_DATA_SEPARATION
UInt32 rlcHandleTxOppurtunity(InternalCellIndex cellIndex);
#else
/* SPR 15909 fix start */
UInt32 rlcHandleTxOppurtunity(tickType_t currentTick, InternalCellIndex cellIndex);
/* SPR 15909 fix end */
#endif
#endif
/** CA Changes End **/
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
extern UInt32 rlcHdlrPenDelQ(LTE_SQUEUE *pendDelQ_p);
/* SPR 3805 Fix Start */
extern UInt32 rlcHdlrPenUeDelQ(LTE_CIRCQUEUE *rlcUePendingDelQ);
/* SPR 3805 Fix End */
extern UInt32 rlcHdlrReEstabQ(LTE_SQUEUE *reEstabQ_p);
#endif

/*coverity-530-32868 */
#ifdef OBSELETE_REL_8
/*coverity-530-32868 */
extern UInt32 rlcHandleHarqFailureInd(UInt32 count);
#endif
/* CA 2 Core Changes Start */
extern UInt32 rlcHandleDataFromMac(UInt32 count,InternalCellIndex internalCellIndex);
/* CA 2 Core Changes End */
/* + SPR 17439 */
extern UInt32 rlcHandleTxExpiredTimer(void);
extern void initUeConfigMsgQueue (void);
/* - SPR 17439 */
void ElProcessRLCAndPDCPMsgs(ExecutionLegFunctionArg * execLegFuncArg_p);

#ifdef UTFWK_SIMULATION
extern void updateTickAndSendAckNack (UInt32 subFrame);
/* + SPR 17439 */
extern UInt32 rlcHandleTxOppurtunityQOS(void);
extern UInt8 updateQload(void);
/* - SPR 17439 */
extern UInt8 parseTaFlag_g;
#endif
 /* TDD FrameWork Changes End */
/* This is an array of execution leg function pointers*/
#ifdef TDD_CONFIG
/*SPR 18691 Fix Start*/
#ifdef LTE_EMBMS_SUPPORTED
extern LTE_CIRCQUEUE rlcEmbmsLpToHpQ_g;
extern void rlcProcessEmbmsHpHandler(LTE_CIRCQUEUE *rlcEmbmsReqRespQ_p);
#endif
/*SPR 18691 Fix End*/

ExecutionLegFunctionPtr executionFunctionArrTdd_g[MAX_EXECUTION_LEGS];

TddPdcchFunctionPtr pdcchFunctionArrTdd_g[MAX_PDCCH_PDSCH_TYPE] = 
{
      allocPdcchPdschForDCIFormat0AndDLInNormalSF,
      allocPdcchPdschForDCIFormat0AndDLInSpecialSF,
      allocPdcchPdschForDLInNormalSF,
      allocPdcchPdschForDLInSpecialSF,
      allocPdcchPdschForDCIFormat0,
      dummyAllocForSpecialSF  /* For special SF config 0,1,5,6 */
};

TddDLSchedulerStrategyFtnPtr dlSchedulerStrategyFtnPtrTable[MAX_PDCCH_PDSCH_TYPE] = 
{
    processDLSchedulerStrategy,
    processDLSchedulerStrategy,
    processDLSchedulerStrategy,
    processDLSchedulerStrategy,
    processDummyDlSchedulerStrategy, /* For special SF config 0,1,5,6 */
    processDummyDlSchedulerStrategy, /* For special SF config 0,1,5,6 */
};

TddDLSchedulerStrategyFtnPtr dlQosSchedulerStrategyFtnPtrTable[MAX_PDCCH_PDSCH_TYPE] = 
{
    dlQosProcessSchedulerStrategy,
    dlQosProcessSchedulerStrategy,
    dlQosProcessSchedulerStrategy,
    dlQosProcessSchedulerStrategy,
    processDummyDlSchedulerStrategy, /* For special SF config 0,1,5,6 */
    processDummyDlSchedulerStrategy, /* For special SF config 0,1,5,6 */
};
/* + SPS_TDD_Changes */
TddDLSPSSchedulerStrategyFtnPtr dlSPSSchedulerStrategyFtnPtrTable[MAX_PDCCH_PDSCH_TYPE] = 
{
    processSPSDLScheduling,
    processSPSDLScheduling,
    processSPSDLScheduling,
    processSPSDLScheduling,
    processDummyDlSPSSchedulerStrategy, /* For special SF config 0,1,5,6 */
    processDummyDlSPSSchedulerStrategy, /* For special SF config 0,1,5,6 */
};
/* - SPS_TDD_Changes */
#ifdef LTE_EMBMS_SUPPORTED
EmbmsDLSchedulerStrategyFtnPtr  
    eMbmsSchedulerStrategyFtnPtrTable[MAX_EMBMS_SCH_TYPE] = \
{
    processEmbmsSchedulerStrategy,
    processDummyEmbmsSchedulerStrategy 
};
#endif
#elif FDD_CONFIG
ExecutionLegFunctionPtr  executionFunctionArr_g[MAX_EXECUTION_LEGS];
#endif
#ifdef FDD_CONFIG
/* If the value of this global is true it means UL scheduler strategy is 
 * already executed*/
UInt8 checkIfUlStrategyAlreadyDone_g;

/*This semaphore is posted when PDSCH completes downlink resource assignment 
  of a subframe so that transmitter can start with the formation of DL 
  Control Message*/
LTE_SEM dlControlMsgToFormTriggerSem_g;

/* ulRlcWaitForSubFrameIndSem_g is a global of semaphores used in
 * execution flow which need to wait of sub frame indication.In this array
 * the semaphore on index 0 will receive sub frame change indication upon which
 * the rest of semaphores are posted.*/
SEM_T ulRlcWaitForSubFrameIndSem_g;

#ifdef DL_UL_SPLIT
extern LTE_SEM rlcTxOppertunityIndSem_g;

#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
extern LTE_SEM waitForDLPerfStatsUpd_g[MAX_NUM_CELL];
#elif DL_UL_SPLIT_TDD
sem_t waitForSubFrameIndSemArr_g;
#endif
/* SPR 4915 Change Start */
extern SEM_T waitForCellSetupIndSemArr_g[NUM_OF_EXEC_FLOWS];
/* SPR 4915 Change End */

#endif
/* semaphone to serialize unsplit legs.. would be removed after spliting the respective legs */
#endif

/* SPR 5599 changes start (ZIP ID 129063) */
extern UInt32 rlcProcessDelMsg(LTE_CIRCQUEUE *rlcPendingDelQ_p);
extern UInt32 rlcProcessRxReconfigMsg(LTE_CIRCQUEUE *rlcReconfigRxEntityQ_p);
/* SPR 5599 changes end (ZIP ID 129063) */
/*****************************************************************************
 * Declaration of Functions
 *****************************************************************************/
/** CA 2 Core Changes Start **/
/* + SPR 17439 */
void ElRlcHandleTxOpportunity(ExecutionLegFunctionArg * execLegFuncArg_p);
/* - SPR 17439 */
/** CA 2 Core Changes End  **/
#ifdef DL_DATA_SEPARATION
 void ElRlcHandleTxOpportunityWithEncoder(ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElRlcUpdateBuffOccToMac(ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElDLSyncEncoderThread(ExecutionLegFunctionArg * execLegFuncArg_p);
#endif


/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/*****************************************************************************
 * Function Name  : ElEmbmsStrategy
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes is the starting function for
 *                  embms strategy
*****************************************************************************/

#ifdef LTE_EMBMS_SUPPORTED
void ElEmbmsStrategy(ExecutionLegFunctionArg * execLegFuncArg_p)
{
#ifdef FDD_CONFIG
    processEmbmsSchedulerStrategy(
            execLegFuncArg_p->delayToPhy, 
            execLegFuncArg_p->cellIndex);
#elif TDD_CONFIG
    eMbmsSchedulerStrategyFtnPtrTable 
	    [tddMacEmbmsFtnPtrIndexer_p[execLegFuncArg_p->cellIndex]
	    [execLegFuncArg_p->execLegSpecificArg1]]
            (DL_ADVANCE_TICK,
            execLegFuncArg_p->cellIndex);
#endif

}
#endif

/*****************************************************************************
 * Function Name  : ElProcessDLOamAndRRcMsg 
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the OAM and RRC message of 
                    Downlink .
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessDLOamAndRRcMsg(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    InternalCellIndex  internalCellIndex = execLegFuncArg_p->cellIndex;
    UInt32                hpDlOAMRRCNodeCount    = 0;
    RrcOamHiPrioQNode *dlRrcOamQNodeReq_p     = PNULL;
    RrcOamHiPrioQNode *dlRrcOamQNodeResp_p    = PNULL;
    static UInt8       delay_response[MAX_NUM_CELL]         = {0};
    static UInt16      ueIndexMacReset[MAX_NUM_CELL]        = {0};

    if(delay_response[internalCellIndex])
    {
        delay_response[internalCellIndex]--;
        if((MAC_PENDING_RESET - UE_CONTEXT_DEL_DELAY) == delay_response[internalCellIndex])
        {
            delay_response[internalCellIndex] = 0;
            GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(
                    dlRrcOamQNodeResp_p, RrcOamHiPrioQNode);
            if(!dlRrcOamQNodeResp_p)
            {
                ltePanic("allocation fail for dlRrcOamQNodeResp_p");
            }
            /** Klockwork Changes Start **/
            else
            {
            /* FIXED for memory corruption happening in RECONFIG UE : start*/
                memSet(dlRrcOamQNodeResp_p, 0, sizeof(RrcOamHiPrioQNode));
            /* FIXED for memory corruption happening in RECONFIG UE : end*/
                dlRrcOamQNodeResp_p->respCode = MAC_SUCCESS;
                dlRrcOamQNodeResp_p->rrcMsgId = MAC_RESET_UE_ENTITY_REQ;
                dlRrcOamQNodeResp_p->eventId  = rrcOamDlEventId_g[internalCellIndex]; 
                dlRrcOamQNodeResp_p->ueIndex = ueIndexMacReset[internalCellIndex];
                /* Coverity 25565 Fix Start */
                if(!ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(
                            &(rrcOamHPtoLPThdHPrioQ_g[internalCellIndex][DL_THD]),
                            RrcOamHiPrioQNode, dlRrcOamQNodeResp_p))
                {
                    LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                            execLegFuncArg_p->globalTick, 0, 0, 0 ,0 ,0, 0 ,0, __func__, "");
                    FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(dlRrcOamQNodeResp_p);
                }
            /* Review comment fix start MAC_RESET_13 */
                if(PNULL != dlRrcOamQNodeResp_p)
                /* Coverity 25565 Fix End */
                {
                    LOG_MAC_MSG(MAC_RRC_OAM_HPTOLP_THDH_ID,LOGDEBUG,MAC_RRC_INF,
                        /* CA Changes start */
                            execLegFuncArg_p->globalTick,
                        /* CA Changes end */
                            recvMsgFrmHPThdFd_g,
                            dlRrcOamQNodeResp_p->respCode,
                            dlRrcOamQNodeResp_p->rrcMsgId, 
                            dlRrcOamQNodeResp_p->eventId,
                            dlRrcOamQNodeResp_p->ueIndex, 
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME, "DL Thread");
                }
            /* Review comment fix end MAC_RESET_13 */
                /* Send to the LP thread to process it */
                /* CA Changes Start */
                lteMessageQSend( recvMsgFrmHPThdFd_g, 
                        (UInt8*)&(internalCellIndex), sizeof(InternalCellIndex));
                /* CA Changes End */
                ueIndexMacReset[internalCellIndex] = 0;
                rrcOamDlEventId_g[internalCellIndex] = MAC_CTRL_MAX_EV;
            }
            /** Klockwork Changes End **/
        }
    }
    /* Remove sleep MAC RESET changes end*/

    hpDlOAMRRCNodeCount = COUNT_RRC_OAM_LPTOHP_THDH_PRIO_Q(&rrcOamLPtoHPThdHPrioQ_g[internalCellIndex][DL_THD]);

    if(hpDlOAMRRCNodeCount > 0)
    {
        DEQUEUE_RRC_OAM_LPTOHP_THDH_PRIO_Q(&rrcOamLPtoHPThdHPrioQ_g[internalCellIndex][DL_THD],
                RrcOamHiPrioQNode , (void **)&dlRrcOamQNodeReq_p);
    }

    if(dlRrcOamQNodeReq_p)
    {
        GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(dlRrcOamQNodeResp_p, RrcOamHiPrioQNode);
        if(!dlRrcOamQNodeResp_p)
        {
            ltePanic("allocation fail for dlRrcOamQNodeResp_p");
            /** Klockwork Changes Start **/
            FREE_MEM_NODE_RRC_OAM_LPTOHP_THDH_PRIO_Q(dlRrcOamQNodeReq_p);
            return;
            /** Klockwork Changes End **/
        }
        /* FIXED for memory corruption happening in RECONFIG UE : start*/
        memSet(dlRrcOamQNodeResp_p, 0, sizeof(RrcOamHiPrioQNode));
        /* FIXED for memory corruption happening in RECONFIG UE : end*/
        dlRrcOamQNodeResp_p->rrcMsgId = dlRrcOamQNodeReq_p->rrcMsgId;
        dlRrcOamQNodeResp_p->ueIndex = dlRrcOamQNodeReq_p->ueIndex;
        dlRrcOamQNodeResp_p->eventId = dlRrcOamQNodeReq_p->eventId; 
        if(dlApiHandlerFunc[dlRrcOamQNodeReq_p->rrcMsgId])
        {
            dlRrcOamQNodeResp_p->respCode = \
                dlApiHandlerFunc[dlRrcOamQNodeReq_p->rrcMsgId](dlRrcOamQNodeReq_p,\
                        dlRrcOamQNodeResp_p, internalCellIndex);
        }
        else
        {
            dlRrcOamQNodeResp_p->respCode = MAC_FAILURE;
        }
        /* Remove sleep MAC RESET changes start*/
        if(MAC_REQ_MSG_IN_USE == dlRrcOamQNodeResp_p->respCode)
        {
            FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(dlRrcOamQNodeResp_p);

        }
        else if(MAC_SUCCESS == dlRrcOamQNodeResp_p->respCode)
        {
            FREE_MEM_NODE_RRC_OAM_LPTOHP_THDH_PRIO_Q(dlRrcOamQNodeReq_p);
            /* CA Changes start */
            /* + Coverity Id - 32424 */
            if(!ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(
                        &(rrcOamHPtoLPThdHPrioQ_g[internalCellIndex][DL_THD]),
                        RrcOamHiPrioQNode, dlRrcOamQNodeResp_p ))
            {
                freeMemPool(dlRrcOamQNodeResp_p);
                dlRrcOamQNodeResp_p = PNULL;
            }
            /* - Coverity Id - 32424 */
            else
            {
                /* Review comment fix start MAC_RESET_13 */
                LOG_MAC_MSG(MAC_RRC_OAM_HPTOLP_THDH_ID,LOGDEBUG,MAC_RRC_INF,
                        execLegFuncArg_p->globalTick,
                        recvMsgFrmHPThdFd_g,
                        dlRrcOamQNodeResp_p->respCode,
                        dlRrcOamQNodeResp_p->rrcMsgId, 
                        dlRrcOamQNodeResp_p->eventId,
                        dlRrcOamQNodeResp_p->ueIndex, 
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME, "DL Thread");
                /* Review comment fix end MAC_RESET_13 */
            }
            /* Send to the LP thread to process it */
            lteMessageQSend( recvMsgFrmHPThdFd_g, 
                    (UInt8*)&(internalCellIndex), sizeof(InternalCellIndex));
        }
        else
        {
            ueIndexMacReset[internalCellIndex] = dlRrcOamQNodeResp_p->ueIndex;
            delay_response[internalCellIndex] = dlRrcOamQNodeResp_p->respCode;
            rrcOamDlEventId_g[internalCellIndex] = dlRrcOamQNodeResp_p->eventId;
            /* CA Changes End */
            FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(dlRrcOamQNodeResp_p);
            FREE_MEM_NODE_RRC_OAM_LPTOHP_THDH_PRIO_Q(dlRrcOamQNodeReq_p);
        }
        /* Remove sleep MAC RESET changes end*/
    }
}


 /*rel4-drop2 changes*/
/*****************************************************************************
 * Function Name  : ElProcessULOamAndRRcMsg
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Uplink OAM and RRC message.
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessULOamAndRRcMsg(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    InternalCellIndex internalCellIndex = execLegFuncArg_p->cellIndex;
    /*SPR 16855 Fix Start*/
    UInt32                hpUlOAMRRCNodeCount    = 0;
    RrcOamHiPrioQNode *ulRrcOamQNodeReq_p     = PNULL;
    RrcOamHiPrioQNode *ulRrcOamQNodeResp_p    = PNULL;
    /* Remove sleep MAC RESET changes start*/
    /** CA-TDD Changes Start **/
    static UInt8       delay_response[MAX_NUM_CELL]         = {0};
    static UInt16      ueIndexMacReset[MAX_NUM_CELL]        = {0};
    if(delay_response[internalCellIndex])
    {
        delay_response[internalCellIndex]--;
        if((MAC_PENDING_RESET - UE_CONTEXT_DEL_DELAY) == delay_response[internalCellIndex])
        {
            delay_response[internalCellIndex] = 0;
            /** CA-TDD Changes End **/

            GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(
                    ulRrcOamQNodeResp_p, RrcOamHiPrioQNode);
            if(!ulRrcOamQNodeResp_p)
            {
                ltePanic("allocation fail for ulRrcOamQNodeResp_p");
            }
            /** Klockwork Changes Start **/
            else
            {
                /* FIXED for memory corruption happening in RECONFIG UE : start*/
                memSet(ulRrcOamQNodeResp_p, 0, sizeof(RrcOamHiPrioQNode));
                /* FIXED for memory corruption happening in RECONFIG UE : end*/
                ulRrcOamQNodeResp_p->respCode = MAC_SUCCESS;
                /* CA Changes Start */
                ulRrcOamQNodeResp_p->ueIndex = ueIndexMacReset[internalCellIndex];
                ulRrcOamQNodeResp_p->rrcMsgId = MAC_RESET_UE_ENTITY_REQ;
                ulRrcOamQNodeResp_p->eventId  = rrcOamUlEventId_g[internalCellIndex]; 
                /* Coverity 25566 Fix Start */
                if(!ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q( 
                            &(rrcOamHPtoLPThdHPrioQ_g[internalCellIndex][UL_THD]), 
                            RrcOamHiPrioQNode, ulRrcOamQNodeResp_p))
                {
                    LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                            execLegFuncArg_p->globalTick, 0, 0, 0 ,0 ,0, 0 ,0, __func__, "");
                    FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNodeResp_p);
                }

                /* Review comment fix start MAC_RESET_13 */
                if(PNULL != ulRrcOamQNodeResp_p)
                    /* Coverity 25566 Fix End */
                {
                    LOG_MAC_MSG(MAC_RRC_OAM_HPTOLP_THDH_ID,LOGDEBUG,MAC_RRC_INF,
                            execLegFuncArg_p->globalTick,
                            recvMsgFrmHPThdFd_g,
                            ulRrcOamQNodeResp_p->respCode,
                            ulRrcOamQNodeResp_p->rrcMsgId, 
                            ulRrcOamQNodeResp_p->eventId,
                            ulRrcOamQNodeResp_p->ueIndex, 
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME, "UL Thread");
                }
                /* Review comment fix end MAC_RESET_13 */
                /* Send to the LP thread to process it */
                
                /** SPR 12320 Changes Start **/
#ifdef FDD_CONFIG
                longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex = 
                    !longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
                longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex = 
                    !longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex;
#endif
                /* Send to the LP thread to process it */
                /** SPR 12320 Changes End **/
                lteMessageQSend( recvMsgFrmHPThdFd_g, 
                        (UInt8*)&(internalCellIndex), sizeof(InternalCellIndex));
                ueIndexMacReset[internalCellIndex] = 0;
                rrcOamUlEventId_g[internalCellIndex] = MAC_CTRL_MAX_EV;
            }
            /** Klockwork Changes End **/
        }
    }
    /* Remove sleep MAC RESET changes end*/

    hpUlOAMRRCNodeCount = COUNT_RRC_OAM_LPTOHP_THDH_PRIO_Q(&rrcOamLPtoHPThdHPrioQ_g[internalCellIndex][UL_THD]);
    if(hpUlOAMRRCNodeCount > 0)
    {
        DEQUEUE_RRC_OAM_LPTOHP_THDH_PRIO_Q(&rrcOamLPtoHPThdHPrioQ_g[internalCellIndex][UL_THD],
                RrcOamHiPrioQNode, (void **)&ulRrcOamQNodeReq_p);
    }
    /* CA Changes End */
    if(ulRrcOamQNodeReq_p)
    {
        GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNodeResp_p,RrcOamHiPrioQNode);
        if(!ulRrcOamQNodeResp_p)
        {
            FREE_MEM_NODE_RRC_OAM_LPTOHP_THDH_PRIO_Q(ulRrcOamQNodeReq_p);
            ltePanic("allocation fail for ulRrcOamQNodeResp_p");
            return ;
            /** Klockwork Changes End **/
        }
        /* FIXED for memory corruption happening in RECONFIG UE : start*/
        memSet(ulRrcOamQNodeResp_p, 0, sizeof(RrcOamHiPrioQNode));
        /* FIXED for memory corruption happening in RECONFIG UE : end*/
        ulRrcOamQNodeResp_p->rrcMsgId = ulRrcOamQNodeReq_p->rrcMsgId;
        ulRrcOamQNodeResp_p->ueIndex = ulRrcOamQNodeReq_p->ueIndex;
        ulRrcOamQNodeResp_p->eventId = ulRrcOamQNodeReq_p->eventId; 
        if(ulApiHandlerFunc[ulRrcOamQNodeReq_p->rrcMsgId])
        {
            ulRrcOamQNodeResp_p->respCode = 
                ulApiHandlerFunc[ulRrcOamQNodeReq_p->rrcMsgId](ulRrcOamQNodeReq_p,
                        ulRrcOamQNodeResp_p,internalCellIndex);
        }
        else
        {
            ulRrcOamQNodeResp_p->respCode = MAC_FAILURE;
        }
        if(MAC_REQ_MSG_IN_USE == ulRrcOamQNodeResp_p->respCode)
        {
            FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNodeResp_p);
        }
        /* Remove sleep MAC RESET changes start*/ 
        else if(MAC_SUCCESS == ulRrcOamQNodeResp_p->respCode)
        {
            FREE_MEM_NODE_RRC_OAM_LPTOHP_THDH_PRIO_Q(ulRrcOamQNodeReq_p);
            /* CA Changes start */
            if(!ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(
                        &(rrcOamHPtoLPThdHPrioQ_g[internalCellIndex][UL_THD]),
                        RrcOamHiPrioQNode, ulRrcOamQNodeResp_p))
            {
                freeMemPool(ulRrcOamQNodeResp_p);
                ulRrcOamQNodeResp_p = PNULL;
            }
            else
            {
                LOG_MAC_MSG(MAC_RRC_OAM_HPTOLP_THDH_ID,LOGDEBUG,MAC_RRC_INF,
                        execLegFuncArg_p->globalTick,
                        recvMsgFrmHPThdFd_g,
                        ulRrcOamQNodeResp_p->respCode,
                        ulRrcOamQNodeResp_p->rrcMsgId, 
                        ulRrcOamQNodeResp_p->eventId,
                        ulRrcOamQNodeResp_p->ueIndex, 
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME, "UL Thread");
            }
            /* Send to the LP thread to process it */
            lteMessageQSend( recvMsgFrmHPThdFd_g, 
                    (UInt8*)&(internalCellIndex), sizeof(InternalCellIndex));
        }
        else
        {
            ueIndexMacReset[internalCellIndex] = ulRrcOamQNodeResp_p->ueIndex;
            delay_response[internalCellIndex] = ulRrcOamQNodeResp_p->respCode;
            rrcOamUlEventId_g[internalCellIndex]        = ulRrcOamQNodeResp_p->eventId; 
            /*CA Changes End */
            FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNodeResp_p);
            FREE_MEM_NODE_RRC_OAM_LPTOHP_THDH_PRIO_Q(ulRrcOamQNodeReq_p);
        }
        /* Remove sleep MAC RESET changes end*/
    }
    /*SPR 16855 Fix End*/
}
/*****************************************************************************
 * Function Name  : ElProcessULPendingDeleteUE 
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Uplink pending delete UE requests.
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessULPendingDeleteUE(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    /*SPR 16855 Fix Start*/
    /* SPR 11215 Changes Start */
#if defined(FAPI_1_1_COMPLIANCE)  || defined(FAPI_4_0_COMPLIANCE)
    /* SPR 11215 Changes End */
    /* SPR 15909 fix start */
    tickType_t currentGlobalTick=0;
    /* SPR 15909 fix end */
#ifdef FDD_CONFIG
    currentGlobalTick=execLegFuncArg_p->globalTick;
    /* +- SPR 17777 */
    deleteUEContextUlHiPrioThd( execLegFuncArg_p->delayToPhy, 
            currentGlobalTick,execLegFuncArg_p->cellIndex );
#elif TDD_CONFIG
    currentGlobalTick=execLegFuncArg_p->advanceDLGlobalTick;
    /* +- SPR 17777 */
    deleteUEContextUlHiPrioThd(
            currentGlobalTick,execLegFuncArg_p->cellIndex );
#endif
    /* +- SPR 17777 */
#else
    deleteUEContextUlHiPrioThd (execLegFuncArg_p->cellIndex);
#endif
    /*SPR 16855 Fix End*/
}

 /*rel4-drop2 changes*/
 /*rel4-drop2 changes*/
#if (defined (DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
 /*rel4-drop2 changes*/
/*****************************************************************************
 * Function Name  : ElRlcDLCheckTimerExpiry 
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks the downlink timer expiry of RLC.
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElRlcDLCheckTimerExpiry(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    if(RLC_FAILURE != rlcLayerInitialized_g &&
            pdcpContextIsInitialized())
    {
       /** CA Changes Start **/
       rlcCheckTxTimerExpiry(execLegFuncArg_p->cellIndex);
       /** CA Changes End **/
    }
}
/*****************************************************************************
 * Function Name  : ElRlcDLHdlrReestablishQ 
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the RLC Re establishment message queue for downlink. 
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElRlcDLHdlrReestablishQ(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    rlcHdlrReEstabQ(&rlcTxReEstabQ_g);
#else    
    /** CA Changes Start **/ 
    rlcProcessReEstabMsg(&rlcTxReEstabQ_g[execLegFuncArg_p->cellIndex]);
    /** CA Changes End **/ 
#endif
}
/*****************************************************************************
 * Function Name  : ElRlcDLHdlrPenDelQ
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the RLC pending Delete message Queue for Downlink.
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElRlcDLHdlrPenDelQ(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    if(sQueueCount(&rlcTxPendingDelQ_g))
    {
        rlcHdlrPenDelQ(&rlcTxPendingDelQ_g);
    }
#else
    /** CA Changes Start **/ 
    if(circQueueCount(&rlcTxPendingDelQ_g[execLegFuncArg_p->cellIndex]))
    {
        rlcProcessDelMsg(&rlcTxPendingDelQ_g[execLegFuncArg_p->cellIndex]);
    }
    if(circQueueCount(&rlcReconfigTxEntityQ_g))
    {
        rlcProcessTxReconfigMsg(&rlcReconfigTxEntityQ_g);
    }
    /** CA Changes End **/ 
#endif
}
/*rel4-drop2 changes*/
/*rel4-drop2 changes*/
/*****************************************************************************
 * Function Name  : ElRlcULCheckTimerExpiry
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks the uplink timer expiry for RLC.
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElRlcULCheckTimerExpiry(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    if(RLC_FAILURE != rlcLayerInitialized_g &&
            pdcpContextIsInitialized())
    {
       rlcCheckRxTimerExpiry(execLegFuncArg_p->cellIndex);
    }
}
/*****************************************************************************
 * Function Name  : ElRlcULHdlrReestablishQ
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the RLC Re establishment message queue for uplink.
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElRlcULHdlrReestablishQ(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    /*SPR 16855 +-*/
    (void)execLegFuncArg_p;/*SPR 18490+-*/
#ifdef UE_SIM_TESTING
    rlcHdlrReEstabQ(&rlcRxReEstabQ_g);
#else
    rlcProcessReEstabMsg(&rlcRxReEstabQ_g);
#endif
}
/*****************************************************************************
 * Function Name  : ElRlcULHdlrPenDelQ 
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the RLC pending Delete message Queue for Uplink.
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElRlcULHdlrPenDelQ(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    (void)execLegFuncArg_p;/*SPR 18490+-*/
    /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    if(sQueueCount(&rlcRxPendingDelQ_g))
    {
        rlcHdlrPenDelQ(&rlcRxPendingDelQ_g);
    }
#else
    if(circQueueCount(&rlcRxPendingDelQ_g))
    {
        rlcProcessDelMsg(&rlcRxPendingDelQ_g);
    }
    if(circQueueCount(&rlcReconfigRxEntityQ_g))
    {
        rlcProcessRxReconfigMsg(&rlcReconfigRxEntityQ_g);
    }
#endif
}
/*****************************************************************************
 * Function Name  : ElProcessULPendingResetUE 
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes Uplink Pending Reset requests of UE.
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessULPendingResetUE(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    (void)execLegFuncArg_p;/*SPR 18490+-*/
/* Content will come after split of this EL */
}

/* SPR 9911 fix start */
/*****************************************************************************
 * Function Name  : ElRlcHandleStatusReport 
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles RLC data from MAC.
*****************************************************************************/
void ElRlcHandleStatusReport(ExecutionLegFunctionArg * execLegFuncArg_p)
{
    UInt32 cQcount = COUNT_RLC_TRIGGER_STATUS_REPORT_Q(execLegFuncArg_p->cellIndex);
    if((RLC_FAILURE != rlcLayerInitialized_g &&
                pdcpContextIsInitialized()) && cQcount)
    {
        processRlcTriggerStatusReport(cQcount,execLegFuncArg_p->cellIndex);
    }
}
/* SPR 9911 fix end */

/*****************************************************************************
 * Function Name  : ElRlcHandleDataFromMac 
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles RLC data from MAC.
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElRlcHandleDataFromMac(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    (void)execLegFuncArg_p;/*SPR 18490+-*/
    /* CA Changes Start */
    UInt32 sQcount = 0 ;
    InternalCellIndex cellIndex = 0;
    /* HSM_CHG_START */
    for (cellIndex=0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        l2ThreadHealthStoreTick_g.rlcMacThread[cellIndex].rlcThread[RLC_UL_THREAD] = globalTTITickCount_g;
    }
    /* HSM_CHG_END */
    for(;cellIndex < MAX_NUM_CELL;cellIndex++)
    {
        sQcount = COUNT_MAC_UE_DATA_IND_Q(cellIndex);
        if(sQcount)
        {
            rlcHandleDataFromMac(sQcount,cellIndex);
        }
    }
    /* CA Changes end */
}
/*****************************************************************************
 * Function Name  : ElProcessPerformanceStatsUpdate
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the performance stats update.
*****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void  ElProcessPerformanceStatsUpdate(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    UInt32 statsExecutionFlowId = 0;
    InternalCellIndex internalCellIndex = execLegFuncArg_p->cellIndex;
#ifdef OVERSHOOT_CORRECTION
    OverLoadLevel currenTickOverLoadLevel = (currTickOverLoadUL_g > currTickOverLoadDL_g)?currTickOverLoadUL_g :currTickOverLoadDL_g;
#endif
    statsExecutionFlowId = execLegFuncArg_p->execLegSpecificArg1;
    /* CA Stats Changes Start */
    lteMacUpdateStatsPerfEnd(statsExecutionFlowId, execLegFuncArg_p->cellIndex);
    /* CA Stats Changes End */
/*Change DROP1*/
    if(DL_OVERSHOOT_STATS == statsExecutionFlowId)
    {
#ifndef DL_UL_SPLIT_TDD
        semPost(&waitForDLPerfStatsUpd_g[internalCellIndex]);
#endif
    }
    else
    {
        /* to ensure that DL is finished before UL is wrap around to first EL */
#ifndef DL_UL_SPLIT_TDD
        semWait(&waitForDLPerfStatsUpd_g[internalCellIndex]);
#endif
#ifdef OVERSHOOT_CORRECTION

        /** CA Changes Start **/
        setOverLoadLevel(currenTickOverLoadLevel,internalCellIndex);
        /** CA Changes End **/
#endif
#ifndef DL_UL_SPLIT_TDD
        semPost(&waitForDLPerfStatsUpd_g[internalCellIndex]);
#endif
    }
/*Change DROP1*/
}
#endif

/** EL Sync Changes Start **/
#ifdef FDD_CONFIG
#if (!defined(DL_UL_SPLIT) && !defined(DL_DATA_SEPARATION))

/*****************************************************************************
 * Function Name  : initExecutionFlowElGroups
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None
 * Description    : This function initializes the EL flows
*****************************************************************************/
/* + SPR 17439 */
void initExecutionFlowElGroups(void)
/* - SPR 17439 */    
{
    UInt32 subGroupCount = 0;
    UInt32 execElCount = 0;
    UInt32 mandatoryElCount = 0;
    UInt32 optionalElCount = 0;
    ExecutionFlowCreationParam *executionFlowThreadParamArr_p = &executionFlowCreationParamArr_g[0];
    ExecutionLegInfo *executionLegArr_p = PNULL;
    SchedulerFunctionalitySet *execElFunctionalityGroup = &SchFnSet_g[subGroupCount];

    while((subGroupCount < MAX_EL_SUB_GROUP) && 
        (execElFunctionalityGroup->executionLegArr[execElCount].executionLeg != MAX_LAST_EL)) /* Coverity CID 28-JUN-2014 Fix */
    {
        executionLegArr_p = &executionFlowThreadParamArr_p->threadArguments.execFlowFuncSubGroup[subGroupCount].
                            executionLegArr[execElCount];
        while(execElFunctionalityGroup->executionLegArr[execElCount].executionLeg != MAX_EXECUTION_LEGS)
        {
            executionLegArr_p[execElCount].executionLeg = 
                execElFunctionalityGroup->executionLegArr[execElCount].executionLeg;
            executionLegArr_p[execElCount].legSpecificArg1 = 
                execElFunctionalityGroup->executionLegArr[execElCount].legSpecificArg1;
            executionLegArr_p[execElCount].legSpecificArg2 = 
                execElFunctionalityGroup->executionLegArr[execElCount].legSpecificArg2;
            executionLegArr_p[execElCount].legSpecificArg3 = 
                execElFunctionalityGroup->executionLegArr[execElCount].legSpecificArg3;
            execElCount++;
        }
        mandatoryElCount = execElCount;
        executionFlowThreadParamArr_p->threadArguments.execFlowFuncSubGroup[subGroupCount].mandatoryElCount = mandatoryElCount;

        executionLegArr_p[execElCount].executionLeg = MAX_EXECUTION_LEGS;
        while(execElFunctionalityGroup->executionLegArr[execElCount +1 ].executionLeg != MAX_EXECUTION_LEGS)
        {
            executionLegArr_p[execElCount+1].executionLeg = 
                execElFunctionalityGroup->executionLegArr[execElCount +1].executionLeg;
            executionLegArr_p[execElCount+1].legSpecificArg1 = 
                execElFunctionalityGroup->executionLegArr[execElCount+1].legSpecificArg1;
            executionLegArr_p[execElCount+1].legSpecificArg2 = 
                execElFunctionalityGroup->executionLegArr[execElCount +1].legSpecificArg2;
            executionLegArr_p[execElCount+1].legSpecificArg3 = 
                execElFunctionalityGroup->executionLegArr[execElCount +1].legSpecificArg3;
            execElCount++;
            optionalElCount++;
        }
        executionFlowThreadParamArr_p->threadArguments.
            execFlowFuncSubGroup[subGroupCount].optionalElCount = optionalElCount;

        executionLegArr_p[execElCount +1].executionLeg = MAX_EXECUTION_LEGS;
        execElCount = 0;
        mandatoryElCount = 0;
        optionalElCount = 0;
        subGroupCount++;
        execElFunctionalityGroup = &SchFnSet_g[subGroupCount];
    }
    executionFlowThreadParamArr_p->threadArguments.executionFlowSubGruopCount = subGroupCount;
    return;
}
/* SPR 21660 changes start_vishal */


/* mac thread changes + */
SchedulerFunctionalitySet SchFnSet_g[MAX_EL_SUB_GROUP] =
{
    {
        0,0,
        { {PROCESS_RX_UNTIL_SUBFRAME_IND,0,0,0},
          {PROCESS_DL_HARQ_ACK_NACK_QUEUE,0,0,0},
          {PROCESS_DL_HARQ_TIMER_EXPIRY,1,0,0},
          {PROCESS_DL_CA_TIMER_EXPIRY,1,0,0},/*CA timer expiry EL*/
          {PROCESS_DL_SPS_STRATEGY,0,0,0 }, /* SPS DL Strategy */
          { MAX_EXECUTION_LEGS,0,0,0 }, /* indicates End of Mandatory Els of current Sub-Group */
          { MAX_EXECUTION_LEGS,0,0,0 } /* indicates End of Optional  Els of current Sub-Group */
    }},
    {
        0,0,
        {{PROCESS_PENDING_EVENT_FD,0,0,0},
         {PROCESS_SET_STRATEGY_FD,0,0,0,},
         {PROCESS_CHECK_STRATEGY_FD,0,0,0,},
         { MAX_EXECUTION_LEGS,0,0,0 },/* indicates End of Mandatory Els of current Sub-Group */
         { MAX_EXECUTION_LEGS,0,0,0 }} /* indicates End of Optional  Els of current Sub-Group */
    },
#ifdef LTE_EMBMS_SUPPORTED
    {
        0,0,
       {{PROCESS_EMBMS_STRATEGY,0,0,0 }, /* indicates End of El Sub-groups */
       { MAX_EXECUTION_LEGS,0,0,0 },/* indicates End of Mandatory Els of current Sub-Group */
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */

    },
#endif
    {
        0,0,
        {{PROCESS_DL_SCH_STRATEGY,0,0,0},
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    { 
        0,0,
        {{PROCESS_SET_PDCCH_PDSCH_FD,0,0,0,},
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    {
        0,0,
        {
#ifndef OVERSHOOT_CORRECTION
         {PROCESS_DEMUX_QUEUE,0,0,0},
#endif
         {PROCESS_UL_HARQ_TIMER_EXPIRY,1,0,0},
         {PROCESS_UL_SPS_STRATEGY, 0,0,0}, /* SPS UL Strategy */
         {PROCESS_UL_SCH_STRATEGY,0,0,0},
         {PROCESS_PRS_TRANSMISSION,0,0,0}, /* PRS_CHANGES */
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    {
        0,0,
        {{PROCESS_CHECK_PDCCH_PDSCH_FD,0,0,0,},
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    {
        0,0,
        {{PROCESS_PDCCH_PDSCH_WITH_MUX,0,0,0},
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    {
        0,0,
        {{PROCESS_SET_MUX_FD,0,0,0,},
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    {
        0,0,
        {{PROCESS_CHECK_MUX_FD,0,0,0,},
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    /* + SPR 13643 Changes */
    {
        0,0,
        {{PROCESS_MUX_QUEUE,0,0,0 },
         {RLC_HANDLE_TX_OPPORTUNITY,0,0,0},
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    {
        0,0,
        {{PROCESS_SET_ENCODER_FD,0,0,0,},
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    {
        0,0,
        {{PROCESS_PUSCH_FOR_DELAY_ONE,0,0,0,},
        {PROCESS_SRS_REPORTS,0,0,0}, /* FDD_SRS */
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    /* - SPR 13643 Changes */
    {
        0,0,
        {{PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA,0,0,0},
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },
    {
        0,0,
         {{PROCESS_CHECK_ENCODER_FD,0,0,0,},
         { MAX_EXECUTION_LEGS,0,0,0 },
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */
    },

    {
        0,0,
        {{PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS,0,0,0},
         {TX_DL_CONTROL_MSG,0,0,0},
         {TX_DL_DATA_MSG,0,0,0},
         {PROCESS_DRX_FOR_ALL_UE,0,0,0}, /* DRX_CHG */
         {PROCESS_MEAS_GAP_FOR_ALL_UE,0,0,0}, /* MEAS_GAP_CHG */
         {PROCESS_DELETE_SCHEDULE_LC_LIST,0,0,0}, /*CA_CHG_START*/
         { MAX_EXECUTION_LEGS,0,0,0 },
#ifdef OVERSHOOT_CORRECTION
         {PROCESS_DEMUX_QUEUE,0,0,0},
#endif
         { MAX_EXECUTION_LEGS,0,0,0 }}
    },
    {
        0,0,
        {
         /* OVERSHOOT_CORRECTION_CHG */
         {MAX_EXECUTION_LEGS,0,0,0},
         {PROCESS_UL_RA,0,0,0},
         {PROCESS_CQI_REPORTS,0,0,0}, /* SS_S2 */
         {PROCESS_UL_CSI_SINR_REPORTS,0,0,0}, /* ULA_CHG */
         {PROCESS_SPS_PDSCH, 0,0,0 }, /* SPS PDSCH */
         {PROCESS_SPS_PUSCH,0,0,0 }, /* SPS PUSCH */
         {PROCESS_E_CID_REPORTS,0,0,0}, /*E-CID REPORTS*/
         {UPDATE_DL_TOP_SCH_LIST_SCELL_STATE,0,0,0}, /*update top schedulable list*/
         {PROCESS_UL_OAM_AND_RRC_MSG,0,0,0},
         {PROCESS_DL_OAM_AND_RRC_MSG,0,0,0},
         {PROCESS_UL_PENDING_DELETE_UE,0,0,0},
         {PROCESS_PENDING_DELETE_UE,0,0,0},
         {PROCESS_PENDING_RESET_UE,0,0,0},
	 /* +- SPR 22494 */
         {PROCESS_MISC_UPDATES,0,0,0},
#ifdef LTE_EMBMS_SUPPORTED
         {RLC_HANDLE_PRE_OPPORTUNITY,0,0,0},
#endif
         { MAX_EXECUTION_LEGS,0,0,0 }}
    },
    {
        0,0,
       {{ MAX_LAST_EL,0,0,0 }, /* indicates End of El Sub-groups */ /* Coverity CID 28-JUN-2014 Fix */
       { MAX_EXECUTION_LEGS,0,0,0 },/* indicates End of Mandatory Els of current Sub-Group */
         { MAX_EXECUTION_LEGS,0,0,0 }}/* indicates End of Optional  Els of current Sub-Group */

    }

};
/* SPR 21660 changes start_vishal */
/* SPR 21660 changes end_vishal */
/* mac thread changes +- */
#endif
#endif	
     
/** EL Sync Changes End **/


#ifdef TDD_CONFIG 
/* SPR 13768 Changes Start */
ExecutionFlowCreationParam *executionFlowCreationParamArr_g[MAX_NUM_CELL];
/* SPR 13768 Changes End */
#endif

#ifdef DL_UL_SPLIT

/*CA Changes start*/
/*6.0 DL_UL_SPLIT Fix, correcting order of Cell Index and execution leg count*/
ExecutionFlowCreationParam executionFlowCreationParamArr_g[NUM_OF_EXEC_FLOWS]
                = {
                    {
                        1,     /*Thread id*/
                        99,     /*Thread Priority */
                        {
                            INIT_DL_THREAD_PARAMS,
                            0, /* Thread bind mask */
                            1,
                            PHY_DL_DELAY, /* delay to phy */
                             0, /* Cell Index */
               /* PDCCH_PDSCH_WITHOUT_MUX Start */
                            20,/* execution leg count 17 to 18*/
               /* PDCCH_PDSCH_WITHOUT_MUX End */
#ifdef OVERSHOOT_CORRECTION
                            13, /*mandatory ELs count*/
#endif

                            {
                             {PROCESS_DL_HARQ_ACK_NACK_QUEUE,0,0,0},
                             {PROCESS_DL_HARQ_TIMER_EXPIRY,1,0,0},
                             /*ca chngs phase 3 start*/
                             {PROCESS_DL_CA_TIMER_EXPIRY,1,0,0},/*CA timer expiry EL*/
                             /*ca chngs phase 3 end*/  
                             {PROCESS_DL_SPS_STRATEGY,0,0,0},
                             {PROCESS_DL_SCH_STRATEGY,0,0,0},
                             {PROCESS_PRS_TRANSMISSION,0,0,0}, /* PRS_CHANGES */
                             {PROCESS_PDCCH_PDSCH_WITH_MUX,0,0,0},
               /* PDCCH_PDSCH_WITHOUT_MUX Start */
                             {PROCESS_MUX_QUEUE,0,0,0},
               /* PDCCH_PDSCH_WITHOUT_MUX End */
/* SPR 8712 changes */
#ifndef DL_DATA_SEPARATION
                             {PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA,0,0,0},
#endif                             
                             {TX_DL_CONTROL_MSG,0,0,0},
#ifndef DL_DATA_SEPARATION
                             {TX_DL_DATA_MSG,0,0,0},
#endif                             
                             {PROCESS_DRX_FOR_ALL_UE,0,0,0},
                             {PROCESS_MEAS_GAP_FOR_ALL_UE,0,0,0},
                             {PROCESS_CQI_REPORTS,0,0,0},
                             {PROCESS_SPS_PDSCH,0,0,0},
                             {PROCESS_DL_OAM_AND_RRC_MSG,0,0,0},
                             /*ca chngs */
                             {UPDATE_DL_TOP_SCH_LIST_SCELL_STATE,0,0,0},
                             /*ca chngs */
#ifdef DL_DATA_SEPARATION
                            { PROCESS_DL_SYNC_TX_DATA,0,0,0},
                            { RLC_UPDATE_BUFF_OCC_TO_MAC,0,0,0},
#endif
                             {PROCESS_DELETE_SCHEDULE_LC_LIST,0,0,0}, /*CA_CHG_START* add EL*/
/* SPR 8712 changes */
                             {PROCESS_PENDING_DELETE_UE,0,0,0},
                             {PROCESS_DL_PERFORMANCE_STATS_UPDATE,DL_OVERSHOOT_STATS,0,0}
                            }
                        }
                     },
                     {  /* 2nd */
                        2, /*Thread id*/
                        99, /* thread priority */
                        {
                            INIT_UL_THREAD_PARAMS,
                            0, /* Thread bind mask */
                            2,
                            PHY_DL_DELAY, /* delay to phy */
                            0, /* Cell Index */
/* SPR 8712 changes */
			    /* + SPR 22494 */
#ifndef DL_DATA_SEPARATION
#ifdef OBSELETE_REL_8
                            20, /* execution leg count */
#else                            
                            19, /* execution leg count */ 
#endif                            
#else
#ifdef OBSELETE_REL_8
                            18, /* execution leg count */
#else                            
                            17, /* execution leg count */
#endif                            
#endif
/* SPR 8712 changes */
			     /* - SPR 22494 */

#ifdef OVERSHOOT_CORRECTION 
                            6,  /* mandatory execution leg count */
#endif
                            
                            {
                             {PROCESS_RX_UNTIL_SUBFRAME_IND,0,0,0},
                             {PROCESS_UL_HARQ_TIMER_EXPIRY,1,0,0},
                             {PROCESS_UL_SPS_STRATEGY,0,0,0},
                             {PROCESS_UL_SCH_STRATEGY,0,0,0},
                             {PROCESS_PUSCH_FOR_DELAY_ONE,0,0,0},
                             {PROCESS_SRS_REPORTS,0,0,0},
/* SPR 8712 changes */
#ifndef DL_DATA_SEPARATION                             
                             {PROCESS_RLC_HANDLE_TX_OPPORTUNITY,0,0,0},
#endif                             
/* SPR 8712 changes */
                             {PROCESS_DEMUX_QUEUE,0,0,0},
                             {PROCESS_UL_CSI_SINR_REPORTS,0,0,0},
                             {PROCESS_SPS_PUSCH,0,0,0},
                             {PROCESS_E_CID_REPORTS,0,0,0}, /*E-CID REPORTS*/
                             {PROCESS_UL_RA,0,0,0},
#ifdef OBSELETE_REL_8
                             {PROCESS_RLC_HANDLE_HARQ_FAILURE_IND,0,0,0},
#endif                             
/* SPR 8712 changes */
#ifndef DL_DATA_SEPARATION                             
                             {PROCESS_RLC_DL_CHECK_TIMER_EXPIRY,0,0,0},
#endif                             
/* SPR 8712 changes */
                             {PROCESS_RLC_DL_HDLR_REESTABLISH_Q,0,0,0},
                             {PROCESS_RLC_DL_HDLR_PEN_DEL_Q,0,0,0},
                             /*Change DROP1*/
                             {PROCESS_UL_OAM_AND_RRC_MSG,0,0,0},
                             {PROCESS_UL_PENDING_DELETE_UE,0,0,0},
                             {PROCESS_MISC_UPDATES,0,0,0},
                             /*Change DROP1*/
			     /* +- SPR 22494 */
                             {PROCESS_UL_PERFORMANCE_STATS_UPDATE,UL_OVERSHOOT_STATS,0,0}
                            }
                        }
                     },
                     {
                        3, /* thread id */
                        50, /* thread priority */
                        {
#ifdef UE_SIM_TESTING
                            DEFAULT_STARTUP_EXEC_LEG,
#else
                            INIT_RLC_PARAMS,
#endif
                            0,
                            3,
                            PHY_DL_DELAY, /* delay to phy */
                            0, /* Cell Index */
                            6,
#ifdef OVERSHOOT_CORRECTION                            
                            6,  /* mandatory execution leg count */
#endif /*OVERSHOOT_CORRECTION*/                              
                            
 
                            {
                             {WAIT_FOR_SUBFRAME_IND,0,0,0},
                             /* SPR 9911 fix start */
                             {PROCESS_RLC_HANDLE_STATUS_REPORT,0,0,0},
                             /* SPR 9911 fix end */
                             {PROCESS_RLC_HANDLE_DATA_FROM_MAC,0,0,0},
                             {PROCESS_RLC_UL_CHECK_TIMER_EXPIRY,0,0,0},
                             {PROCESS_RLC_UL_HDLR_REESTABLISH_Q,0,0,0},
                             {PROCESS_RLC_UL_HDLR_PEN_DEL_Q,0,0,0}
                            }
                        }
                     }
                  };
#endif

#ifdef ONE_FLOW_ONE_DELAY

/* CA changes start */
/** El Sync Changes Start **/

#if (!defined(DL_UL_SPLIT) && !defined(DL_DATA_SEPARATION)) 
#define THREAD_PRIORITY 95
#define LEG_COUNT 0

ExecutionFlowCreationParam executionFlowCreationParamArr_g[NUM_OF_EXEC_FLOWS]
                = {
                    {
                        0, /*Thread id*/
        THREAD_PRIORITY, /* thread priority */
        {
            REGISTER_WITH_PHY_FOR_SUBFRAME_IND_AFTER_INIT_MAC,
            0, /* Thread bind mask */
            0, /* Thread Index */
            PHY_DL_DELAY , /* delay to phy */
            0, /* cell Index */
            0, /* El Sub Group Count */
            {
                    {0,0,{{PROCESS_RX_UNTIL_SUBFRAME_IND,0,0,0},}},
            }
        }
    }
};

/* SPR 21660 changes start_vishal */
/* SPR 21660 changes end_vishal */
#elif DL_DATA_SEPARATION
/* SPR 22617 Start */
#define LEG_COUNT_MIN     38
/* SPR 22617 End */
#ifndef UTFWK_SIMULATION
#define THREAD_PRIORITY 0
#define NUM_UTFWK_EL      0
#else /* UTFWK_SIMULATION */
#define THREAD_PRIORITY 95
#define NUM_UTFWK_EL 2
#endif /* UTFWK_SIMULATION */

#define NUM_SPLIT_EL 3 

#define LEG_COUNT (LEG_COUNT_MIN + NUM_UTFWK_EL + NUM_SPLIT_EL)

ExecutionFlowCreationParam executionFlowCreationParamArr_g[NUM_OF_EXEC_FLOWS]
   = {
      {
        0, /*Thread id*/
        THREAD_PRIORITY, /* thread priority */
        {
            REGISTER_WITH_PHY_FOR_SUBFRAME_IND_AFTER_INIT_MAC,
            0, /* Thread bind mask */
            0, /* Thread Index */
            PHY_DL_DELAY , /* delay to phy */
            0,/* cell index */
            LEG_COUNT,
               {
                {PROCESS_RX_UNTIL_SUBFRAME_IND,0,0,0},
#ifdef UTFWK_SIMULATION
                {RLC_UPDATE_QLOAD,0,0,0},
#endif
                {PROCESS_DL_HARQ_ACK_NACK_QUEUE,0,0,0},
                {PROCESS_DL_HARQ_TIMER_EXPIRY,1,0,0},
                {PROCESS_DL_CA_TIMER_EXPIRY,1,0,0},/*CA timer expiry EL*/
                {PROCESS_DL_SPS_STRATEGY,0,0,0 }, /* SPS DL Strategy */
#ifdef LTE_EMBMS_SUPPORTED
                {PROCESS_EMBMS_STRATEGY,0,0,0},
#endif
	        {PROCESS_PENDING_EVENT_FD,0,0,0},
                {PROCESS_SET_STRATEGY_FD,0,0,0,},
                {PROCESS_CHECK_STRATEGY_FD,0,0,0,},
                {PROCESS_DL_SCH_STRATEGY,0,0,0},
                {PROCESS_SET_PDCCH_PDSCH_FD,0,0,0,},
#ifndef OVERSHOOT_CORRECTION
                {PROCESS_DEMUX_QUEUE,0,0,0},
#endif
                {PROCESS_UL_HARQ_TIMER_EXPIRY,1,0,0},
                {PROCESS_UL_SPS_STRATEGY, 0,0,0}, /* SPS UL Strategy */
                {PROCESS_UL_SCH_STRATEGY,0,0,0},
                {PROCESS_PRS_TRANSMISSION,0,0,0}, /* PRS_CHANGES */
                {PROCESS_CHECK_PDCCH_PDSCH_FD,0,0,0,},
                {PROCESS_PDCCH_PDSCH_WITH_MUX,0,0,0},
                {PROCESS_SET_MUX_FD,0,0,0,},
                {PROCESS_CHECK_MUX_FD,0,0,0,},
                {PROCESS_MUX_QUEUE,0,0,0 },
                {PROCESS_PUSCH_FOR_DELAY_ONE,0,0,0},
                {PROCESS_SRS_REPORTS,0,0,0}, /* FDD_SRS */
                {TX_DL_CONTROL_MSG,0,0,0},
                {PROCESS_DRX_FOR_ALL_UE,0,0,0}, /* DRX_CHG */
                {PROCESS_MEAS_GAP_FOR_ALL_UE,0,0,0}, /* MEAS_GAP_CHG */
#ifdef OVERSHOOT_CORRECTION
                {PROCESS_DEMUX_QUEUE,0,0,0},
#endif
                {PROCESS_DELETE_SCHEDULE_LC_LIST,0,0,0}, /*CA_CHG_START*/
                {PROCESS_UL_RA,0,0,0},
                {PROCESS_CQI_REPORTS,0,0,0}, /* SS_S2 */
                {PROCESS_SPS_PDSCH, 0,0,0 }, /* SPS PDSCH */
                {PROCESS_UL_CSI_SINR_REPORTS,0,0,0}, /* ULA_CHG */
                {PROCESS_SPS_PUSCH,0,0,0 }, /* SPS PUSCH */
                {PROCESS_E_CID_REPORTS,0,0,0}, /*E-CID REPORTS*/
                { PROCESS_DL_SYNC_TX_DATA,0,0,0},
                {UPDATE_DL_TOP_SCH_LIST_SCELL_STATE,0,0,0}, /*update top schedulable list*/
#ifdef UTFWK_SIMULATION
                {RLC_HANDLE_TX_OPPORTUNITY_QOS, 0,0,0},
#endif
                { RLC_UPDATE_BUFF_OCC_TO_MAC,0,0,0},
                {PROCESS_UL_OAM_AND_RRC_MSG,0,0,0},
                {PROCESS_DL_OAM_AND_RRC_MSG,0,0,0},
                {PROCESS_UL_PENDING_DELETE_UE,0,0,0},
                {PROCESS_PENDING_DELETE_UE,0,0,0},
                {PROCESS_PENDING_RESET_UE,0,0,0},
		/* +- SPR 22494 */
                {PROCESS_MISC_UPDATES, 0, 0, 0}
#ifdef LTE_EMBMS_SUPPORTED
                ,{RLC_HANDLE_PRE_OPPORTUNITY, 0, 0, 0}
#endif                
            }
        }
    }
};
#endif    /* End If of  DL_UL_SPLIT and DL_DATA_SEPERATION Not Defined*/

#endif //ONE_FLOW_ONE_DELAY

/*****************************************************************************
 * Function Name  : ElProcessPdcchAndPdschWithMuxAndTrigger
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg whn called does the functionality of 
 *                  processing PDCCH/PDSCH and sending Trigger to PUSCH.Also 
 *                  the functionality of Multiplexing is executed in this leg
 *                  itself.This trigger to PUSCH is necessary if PUSCH is in
 *                  another execution flow and it needs to wait until PDCCH
 *                  processing is complete.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessPdcchAndPdschWithMuxAndTrigger(ExecutionLegFunctionArg * 
/* SPR 5599 changes end (ZIP ID 129063) */
                                                              execLegFuncArg_p)
{
#ifdef FDD_CONFIG /* SPR# 9916 fix */
    allocPdcchPdschForDL(1, /*With mux*/
                          /* +- SPR 17777 */
                          execLegFuncArg_p->delayToPhy, 
                          execLegFuncArg_p->globalTick,
                          execLegFuncArg_p->subFrame
                          /*CA Changes start*/
                          , execLegFuncArg_p->cellIndex
                          /*CA Changes end*/
                          ,execLegFuncArg_p
                            );
#else
    /* +- SPR 17777 */

  LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
#endif                          
}


/*****************************************************************************
 * Function Name  : ElProcessPdcchAndPdschWithoutMuxAndTrigger
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg whn called does the functionality of 
 *                  processing PDCCH/PDSCH and sending Trigger to PUSCH.
 *                  This trigger to PUSCH is necessary if PUSCH is in
 *                  another execution flow and it needs to wait until PDCCH
 *                  processing is complete.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessPdcchAndPdschWithoutMuxAndTrigger(ExecutionLegFunctionArg * 
/* SPR 5599 changes end (ZIP ID 129063) */
                                                              execLegFuncArg_p)
{
#ifdef FDD_CONFIG /* SPR# 9916 fix */
    allocPdcchPdschForDL(0, /*With mux*/
                          /* +- SPR 17777 */
                          execLegFuncArg_p->delayToPhy, 
                          execLegFuncArg_p->globalTick
                          , execLegFuncArg_p->subFrame
                          , execLegFuncArg_p->cellIndex
                          ,execLegFuncArg_p
                          ); 
#else
    /* +- SPR 17777 */

  LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
#endif
}


/*****************************************************************************
 * Function Name  : ElProcessPdcchAndPdschWithMux
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg whn called does the functionality of 
 *                  processing PDCCH/PDSCH and also Multiplexing.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessPdcchAndPdschWithMux(ExecutionLegFunctionArg* execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef TDD_CONFIG
    /** CA-TDD Changes Start **/
    pdcchFunctionArrTdd_g [(tddMacPdcchPdschFtnPtrIndexer_p
            [execLegFuncArg_p->cellIndex][execLegFuncArg_p->execLegSpecificArg1])]
        (1, 
         execLegFuncArg_p->advanceDLGlobalTick,
         execLegFuncArg_p->advanceDLSystemframe,
         execLegFuncArg_p->advanceDLSubframe,
         /* TDD Config 0 Changes Start */
         execLegFuncArg_p->execLegSpecificArg3,
         /* TDD Config 0 Changes End */
         execLegFuncArg_p->cellIndex,
         execLegFuncArg_p->execLegSpecificArg2,
         execLegFuncArg_p
        ); 
    /** CA-TDD Changes End **/
#elif FDD_CONFIG

    /* PDCCH_PDSCH_WITHOUT_MUX Start */
    /* CA_phase2_mux start */
    UInt32 withMuxFlag = 1;
    /* CA_phase2_mux end */
    /* PDCCH_PDSCH_WITHOUT_MUX End */

    allocPdcchPdschForDL(withMuxFlag , /*With mux*/
            /* +- SPR 17777 */
            execLegFuncArg_p->delayToPhy, 
            execLegFuncArg_p->globalTick,
            execLegFuncArg_p->subFrame,
            /*CA Changes start*/
            execLegFuncArg_p->cellIndex
            /*CA Changes end*/
            ,execLegFuncArg_p
            );

#endif
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,
              "[%s] Exit . SubFrame No [%d]",__func__,execLegFuncArg_p->subFrame);
}




/*****************************************************************************
 * Function Name  : ElProcessPdcchAndPdschWithoutMux
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg whn called does the functionality of 
 *                  only processing PDCCH/PDSCH.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessPdcchAndPdschWithoutMux(ExecutionLegFunctionArg * 
/* SPR 5599 changes end (ZIP ID 129063) */
                                                              execLegFuncArg_p)
{
#ifdef FDD_CONFIG /* SPR# 9916 fix */
    allocPdcchPdschForDL(0, /*With mux*/
                          /* +- SPR 17777 */
                          execLegFuncArg_p->delayToPhy, 
                          execLegFuncArg_p->globalTick
                          , execLegFuncArg_p->subFrame 
                          , execLegFuncArg_p->cellIndex  
                          , execLegFuncArg_p
                          );
#else
    /* +- SPR 17777 */

  LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
#endif
}


/*****************************************************************************
 * Function Name  : ElProcessMuxQueue
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This funciton whn called process a mux Queue whose index
 *                  is specified by execLegFuncArg_p->execLegSpecificArg1.It
 *                  calls processMultiplexerQueue passing it's third argument
 *                  as 0 , which means processMultiplexerQueue process mux
 *                  queue based on queueCount. The execLegSpecificArg2 will
 *                  provide delay to phy of PDSCH.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessMuxQueue(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef LTE_EMBMS_SUPPORTED
     /*check if there is occassion of EMBMS or not*/
    if( isEmbmsValidSf_g[execLegFuncArg_p->cellIndex] == TRUE)
    {
        processEmbmsMultiplexerQueue(execLegFuncArg_p->cellIndex,
#ifdef FDD_CONFIG
                                     execLegFuncArg_p->delayToPhy
#elif TDD_CONFIG
				                     DL_ADVANCE_TICK
#endif
);
    }
    /* PDCCH_PDSCH_WITHOUT_MUX Start */
    else 
    {    
#endif        
    if(IS_CA_ENABLED())
    {
            /* + SPR 13999 Changes */        
#ifdef FDD_CONFIG        
        processMultiplexerQueue(execLegFuncArg_p->execLegSpecificArg2 + 
                                execLegFuncArg_p->globalTick,
                                0,execLegFuncArg_p->delayToPhy,
                                execLegFuncArg_p->cellIndex);
#elif TDD_CONFIG        
        processMultiplexerQueue(execLegFuncArg_p->execLegSpecificArg2 + 
                                execLegFuncArg_p->advanceDLGlobalTick,
                                0,execLegFuncArg_p->delayToPhy,
                                execLegFuncArg_p->cellIndex);
#endif
            /* - SPR 13999 Changes */        
        }
#ifdef LTE_EMBMS_SUPPORTED
    }
#endif
    /* PDCCH_PDSCH_WITHOUT_MUX End */
}


/*****************************************************************************
 * Function Name  : ElProcessMuxQueueUntilLastNode 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This funciton whn called process a mux Queue whose index
 *                  is specified by execLegFuncArg_p->execLegSpecificArg1.It
 *                  calls processMultiplexerQueue passing it's third argument
 *                  as 1 , which means processMultiplexerQueue process mux
 *                  queue based last node to be processed indication from
 *                  scheduler.The execLegSpecificArg2 will provide delay to
 *                  phy of PDSCH.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessMuxQueueUntilLastNode(ExecutionLegFunctionArg * 
/* SPR 5599 changes end (ZIP ID 129063) */
                                                               execLegFuncArg_p)
{
#ifdef FDD_CONFIG
    processMultiplexerQueue(execLegFuncArg_p->execLegSpecificArg2 + 
            execLegFuncArg_p->globalTick,
            1,execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->cellIndex);
    /* +- SPR 17777 */
#else

  LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
    /* +- SPR 17777 */
#endif                            
}


/*****************************************************************************
 * Function Name  : ElProcessEncoderQueueWithTxUntilLastData
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : When this execution leg is called it processes the
 *                  encoding requests.The function also includes the processing
 *                  of transmission of each encoded TB.The function 
 *                  processEncoderQueue is called with poll till last flag set.
 *                  This causes processEncoderQueue to process until it receives
 *                  a dummy node.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessEncoderQueueWithTxUntilLastData(ExecutionLegFunctionArg * 
/* SPR 5599 changes end (ZIP ID 129063) */
                                                              execLegFuncArg_p)
{
#ifdef FDD_CONFIG
    processEncoderQueue( execLegFuncArg_p->globalTick , 
                         execLegFuncArg_p->delayToPhy,
                         /* +- SPR 17777 */
                         execLegFuncArg_p->subFrame,
                         1, /*is transmitter functionality added */
                         /* +- SPR 17777 */
                         execLegFuncArg_p->cellIndex,
                         /* + SPR 11209 Changes */
                         execLegFuncArg_p->cellIndex);
                        /* - SPR 11209 Changes */
#elif TDD_CONFIG
    processEncoderQueue( 
                         execLegFuncArg_p->advanceDLGlobalTick,
                         execLegFuncArg_p->delayToPhy,
                         execLegFuncArg_p->advanceDLSubframe,
                         1,/* is transmitter functionality added*/
                         execLegFuncArg_p->cellIndex,
                        /* + SPR 11209 Changes */
                         execLegFuncArg_p->cellIndex);
                        /* - SPR 11209 Changes */
#endif
}


/*****************************************************************************
 * Function Name  : ElProcessEncoderQueueWithTx
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : When this execution leg is called it processes the
 *                  encoding requests.The function also includes the processing
 *                  of transmission of each encoded TB.The function 
 *                  processEncoderQueue is called with poll till last flag unset.
 *                  In this case processEncoderQueue doesn't wait for a dummy
 *                  node but processes the encoder queue only queue count number
 *                  of times.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessEncoderQueueWithTx(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Entry. SubFrame No [%d]",__func__,execLegFuncArg_p->subFrame);
#ifdef FDD_CONFIG
    processEncoderQueue( execLegFuncArg_p->globalTick , 
                         execLegFuncArg_p->delayToPhy,
                         /* +- SPR 17777 */
                         execLegFuncArg_p->subFrame,
                         1, /* is transmitter functionality added*/
                         /* +- SPR 17777 */
                         execLegFuncArg_p->cellIndex,
                        /* + SPR 11209 Changes */
                         execLegFuncArg_p->cellIndex);
                        /* - SPR 11209 Changes */
#elif TDD_CONFIG
    processEncoderQueue( 
                         execLegFuncArg_p->advanceDLGlobalTick,
                         execLegFuncArg_p->delayToPhy,
                         execLegFuncArg_p->advanceDLSubframe,
                         1,/* is transmitter functionality added*/
                         execLegFuncArg_p->cellIndex,
                        /* + SPR 11209 Changes */
                         execLegFuncArg_p->cellIndex);
                        /* - SPR 11209 Changes */
#endif
                         
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit . SubFrame No [%d]",__func__,execLegFuncArg_p->subFrame);
}

    /**EL Sync Changes Start **/ 
     /* + SPR_14897 */
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
     /* - SPR_14897 */
/*****************************************************************************
 * Function Name  : ElProcessEncoderQueueOfOtherCells
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : When this execution leg is called it processes the
 *                  encoding requests.The function doesn't include the processing
 *                  of transmission of each encoded TB.The function 
 *                  processEncoderQueue is called with poll till last flag set.
 *                  This causes processEncoderQueue to process until it receives
 *                  a dummy node.
 *****************************************************************************/
void ElProcessEncoderQueueOfOtherCells(ExecutionLegFunctionArg * 
                                                               execLegFuncArg_p)
{
     /* + SPR_14897 */
     /* - SPR_14897 */
    if(IS_CA_ENABLED() && CHECK_CELL_STATE(!(execLegFuncArg_p->cellIndex)))
    {
/* + SPR 13999 Changes */        
#ifdef FDD_CONFIG        
        /* process EncoderQueue for other cell as well*/
        processEncoderQueue( execLegFuncArg_p->globalTick , 
                             execLegFuncArg_p->delayToPhy,
                             /* +- SPR 17777 */
                             execLegFuncArg_p->subFrame,
                             0,/* is transmitter functionality added*/
                             /* +- SPR 17777 */
                             /*CA Changes start*/
                             execLegFuncArg_p->cellIndex,
                             !(execLegFuncArg_p->cellIndex));
                             /*CA Changes end*/
#elif TDD_CONFIG
        processEncoderQueue( 
                             execLegFuncArg_p->advanceDLGlobalTick,
                             execLegFuncArg_p->delayToPhy,
                             execLegFuncArg_p->advanceDLSubframe,
                             0,/* is transmitter functionality added*/
                             execLegFuncArg_p->cellIndex,
                             !(execLegFuncArg_p->cellIndex));
#endif
/* - SPR 13999 Changes */        
    }
     /* + SPR_14897 */
    /* +- SPR 17777 */
     /* - SPR_14897 */
}
#endif
/**EL Sync Changes End **/

/*****************************************************************************
 * Function Name  : ElProcessEncoderQueueWithoutTxUntilLastData
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : When this execution leg is called it processes the
 *                  encoding requests.The function doesn't include the processing
 *                  of transmission of each encoded TB.The function 
 *                  processEncoderQueue is called with poll till last flag set.
 *                  This causes processEncoderQueue to process until it receives
 *                  a dummy node.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessEncoderQueueWithoutTxUntilLastData(ExecutionLegFunctionArg * 
/* SPR 5599 changes end (ZIP ID 129063) */
                                                               execLegFuncArg_p)
{
#ifdef LTE_EMBMS_SUPPORTED
   /*check if there is occassion of EMBMS or not*/
    if( isEmbmsValidSf_g[execLegFuncArg_p->cellIndex] == TRUE)
    {
        processEmbmsEncoderQueue( execLegFuncArg_p->cellIndex,
                                  execLegFuncArg_p->cellIndex);
    }
    else
    {
#endif
    CHECK_QOS();
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Entry. SubFrame No [%d]",__func__,execLegFuncArg_p->subFrame);
#ifdef FDD_CONFIG
    processEncoderQueue( execLegFuncArg_p->globalTick , 
                         execLegFuncArg_p->delayToPhy,
                         /* +- SPR 17777 */
                         execLegFuncArg_p->subFrame,
                         0,/* is transmitter functionality added*/
                         /* +- SPR 17777 */
                         /*CA Changes start*/
                         execLegFuncArg_p->cellIndex,
                        /* + SPR 11209 Changes */
                         execLegFuncArg_p->cellIndex);
                        /* - SPR 11209 Changes */
                         /*CA Changes end*/
#elif TDD_CONFIG
    processEncoderQueue( 
                         execLegFuncArg_p->advanceDLGlobalTick,
                         execLegFuncArg_p->delayToPhy,
                         execLegFuncArg_p->advanceDLSubframe,
                         0,/* is transmitter functionality added*/
                         execLegFuncArg_p->cellIndex, 
                         /* + SPR 11209 Changes */
                         execLegFuncArg_p->cellIndex);
                         /* - SPR 11209 Changes */
#endif
#ifdef LTE_EMBMS_SUPPORTED
    }
#endif
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit . SubFrame No [%d]",__func__,execLegFuncArg_p->subFrame);
}


/*****************************************************************************
 * Function Name  : ElProcessEncoderQueueWithoutTx
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : When this execution leg is called it processes the
 *                  encoding requests.The function doesn't include the processing
 *                  of transmission of each encoded TB.In this function
 *                  processEncoderQueue is called with poll till last flag unset.
 *                  In this case processEncoderQueue doesn't wait for a dummy
 *                  node but processes the encoder queue only queue count number
 *                  of times.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessEncoderQueueWithoutTx(ExecutionLegFunctionArg *
/* SPR 5599 changes end (ZIP ID 129063) */
                                                               execLegFuncArg_p)
{
#ifdef LTE_EMBMS_SUPPORTED
   /*check if there is occassion of EMBMS or not*/
    if( isEmbmsValidSf_g[execLegFuncArg_p->cellIndex] == TRUE)
    {
        processEmbmsEncoderQueue( execLegFuncArg_p->cellIndex,
                                  execLegFuncArg_p->cellIndex);
    }
    else
    {
#endif
    CHECK_QOS();
#ifdef FDD_CONFIG
    processEncoderQueue( execLegFuncArg_p->globalTick , 
                         execLegFuncArg_p->delayToPhy,
                         /* +- SPR 17777 */
                         execLegFuncArg_p->subFrame,
                         0,/* is transmitter functionality added*/
                         /* +- SPR 17777 */
                         execLegFuncArg_p->cellIndex,
                         /* + SPR 11209 Changes */
                         execLegFuncArg_p->cellIndex);
                         /* - SPR 11209 Changes */
#elif TDD_CONFIG
    processEncoderQueue( 
                         execLegFuncArg_p->advanceDLGlobalTick,
                         execLegFuncArg_p->delayToPhy,
                         execLegFuncArg_p->advanceDLSubframe,
                         0,/* is transmitter functionality added*/
                         execLegFuncArg_p->cellIndex,
                         /* + SPR 11209 Changes */
                         execLegFuncArg_p->cellIndex);
                         /* - SPR 11209 Changes */
#endif
#ifdef LTE_EMBMS_SUPPORTED
    }
#endif
}


/*****************************************************************************
 * Function Name  : ElProcessDLHarqAckNackQueue
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : The execution leg function whn called does the processing 
 *                  of Ack Nack queue.Process ack nack queue is to be called
 *                  periodicallly after certain number of subframes.This
 *                  number of subframes is provided by execLegSpecificArg1.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessDLHarqAckNackQueue(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    /* CA Changes start */
    InternalCellIndex internalCellIndex = execLegFuncArg_p->cellIndex;

    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Entry. SubFrame No [%d]",__func__,execLegFuncArg_p->subFrame);

    l2ThreadHealthStoreTick_g.rlcMacThread[internalCellIndex].macThread[MAC_DL_THREAD] = macTTITickCount_g;
#if (!defined (DL_UL_SPLIT) || (!defined DL_DATA_SEPARATION))  
    l2ThreadHealthStoreTick_g.rlcMacThread[internalCellIndex].rlcThread[RLC_DL_THREAD] = globalTTITickCount_g;
#endif

#ifdef FDD_CONFIG
    static UInt32 numberOfSubFrames[MAX_NUM_CELL];
    if (numberOfSubFrames[execLegFuncArg_p->cellIndex] == execLegFuncArg_p->execLegSpecificArg1)
    {
        /* +- SPR 17777 */
        processDLAckNackQueue(
                execLegFuncArg_p->globalTick,  
                execLegFuncArg_p->cellIndex);
        numberOfSubFrames[execLegFuncArg_p->cellIndex] = 0;
    }
    else
    {
        numberOfSubFrames[execLegFuncArg_p->cellIndex]++;
    }
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit .",__func__);

#elif TDD_CONFIG
    /* +- SPR 17777 */
    processDLAckNackQueue(execLegFuncArg_p->globalTick, execLegFuncArg_p->cellIndex);
#endif
    /* CA Changes end */
}
#ifdef UTFWK_SIMULATION
/*****************************************************************************
 * Function Name  : ElrlcUpdateQload
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates RLC load Queue.
 *****************************************************************************/
void ElrlcUpdateQload(ExecutionLegFunctionArg * execLegFuncArg_p)
{
   if(execLegFuncArg_p->execLegSpecificArg1) 
     {
#ifdef TDD_CONFIG
     if((SPECIAL_SUB_FRAME_CONFIG_0 != cellConfig_g.splSubFramePatterns)
        && (SPECIAL_SUB_FRAME_CONFIG_5 != cellConfig_g.splSubFramePatterns))
        {
            updateQload();
        }
#endif
     }
     else
     {
        updateQload();
     } 
}
/*****************************************************************************
 * Function Name  : ElupdateTickAndSendAckNack
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates Tick and Sends Ack or NACK.
 *****************************************************************************/
void ElupdateTickAndSendAckNack(ExecutionLegFunctionArg * execLegFuncArg_p)
{

    updateTickAndSendAckNack(execLegFuncArg_p->subFrame);
}
/*****************************************************************************
 * Function Name  : ElRlcHandleTxOpportunityQOS
 * Inputs         : execLegFuncArg_p - pointer to ExecutionLegFunctionArg
 * Outputs        : None
 * Returns        : None
 * Description    : This function Process the Oppurtunity indication. 
 *****************************************************************************/
void ElRlcHandleTxOpportunityQOS(ExecutionLegFunctionArg * execLegFuncArg_p)
{
    rlcHandleTxOppurtunityQOS();
}
#endif

/*****************************************************************************
 * Function Name  : ElProcessDLCQIReports
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : The execution leg function when called, process CQI reports.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessDLCQIReports(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    /* + CQI_4.1 */
    UInt8 numOfCQINodesToBeProcessed = 0;
    /*CA Changes start  */
    numOfCQINodesToBeProcessed = COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(execLegFuncArg_p->cellIndex);
    /*CA Changes end  */
    if(numOfCQINodesToBeProcessed) 
    {    
        if(numOfCQINodesToBeProcessed > NUM_CQI_NODES)
        {
            numOfCQINodesToBeProcessed = NUM_CQI_NODES;
        }    

        /*CA Changes start  */
        processFeedBackReportQueue(numOfCQINodesToBeProcessed, execLegFuncArg_p->cellIndex);
        /*CA Changes end  */
    }
    /* - CQI_4.1 */
}


/*****************************************************************************
 * Function Name  : ElProcessNDLCQIReports
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : The execution leg function whn called , processes CQI reports.
 *                  Once per certain number of subframes specified by
 *                  execLegSpecificArg1.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessNDLCQIReports(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    /** CA-TDD Changes Start **/
    static UInt32 numberOfSubFrames[MAX_NUM_CELL];

    if (numberOfSubFrames[execLegFuncArg_p->cellIndex] == execLegFuncArg_p->execLegSpecificArg1)
    {
        processFeedBackReportQueue(NUM_CQI_NODES,  execLegFuncArg_p->cellIndex);
        numberOfSubFrames[execLegFuncArg_p->cellIndex]  = 0;
    }
    else
    {
        numberOfSubFrames[execLegFuncArg_p->cellIndex]++;
    }
    /** CA-TDD Changes Stop **/
}


/*****************************************************************************
 * Function Name  : ElProcessDLCQIReportsIfTimeLeft
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : The execution leg function whn called does the processing 
 *                  of CQI reports only if sufficient time left in a tick.
 *                  The minimum time required to execute this leg such that
 *                  the execution flow completes whole one loop in one tick is
 *                  specified through execLegSpecificArg1.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessDLCQIReportsIfTimeLeft(ExecutionLegFunctionArg * 
/* SPR 5599 changes end (ZIP ID 129063) */
                                                              execLegFuncArg_p)
{
    /*CA Changes start  */
    processFeedBackReportQueue(NUM_CQI_NODES, execLegFuncArg_p->cellIndex);
    /*CA Changes end  */
}

/* ULA_CHG */
/*****************************************************************************
 * Function Name  : ElProcessSINRReports
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : The execution leg function processes SINR reports from 
 *                  ULSCH or SRS.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessSINRReports(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    /*CA Changes start  */
    processSINRReports(execLegFuncArg_p->globalTick, execLegFuncArg_p->cellIndex);
#else
    /*coverity-530-32087*/
    SInt32 sinRepCount = 0;
    sinRepCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[execLegFuncArg_p->cellIndex]);
    /*CA Changes end  */
    if(sinRepCount)
    {
        processSINRReports(execLegFuncArg_p->globalTick,sinRepCount, execLegFuncArg_p->cellIndex);
    }
    /*coverity-530-32087*/
#endif
}
/* ULA_CHG */

/*****************************************************************************
 * Function Name  : ElProcessDLHarqTimerExpiry 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function when called processes timer expiry of DL
 *                  harq processes.execLegSpecificArg1 will provide number of
 *                  frames after wich expireAllNodeFromHarqTimerList should 
 *                  be invoked.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessDLHarqTimerExpiry(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef FDD_CONFIG
    UInt16 maxTickDiff = MAC_MAX_VAL( 
            totalTickMissErrIndDl_g[execLegFuncArg_p->cellIndex],
            totalNumberOfTickDiffDl_g[execLegFuncArg_p->cellIndex] );
    if ( maxTickDiff )
    {
#ifdef FDD_CONFIG
        /* + SPR 17733 */
        expireAllNodeFromHarqTimerListForMsg4( maxTickDiff,
                execLegFuncArg_p->subFrame,
                DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                execLegFuncArg_p->cellIndex);
        /* - SPR 17733 */
#endif
        expireAllNodeFromHarqTimerList( maxTickDiff ,
                execLegFuncArg_p->subFrame,
                DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                execLegFuncArg_p->cellIndex);
        /* Reset totalTickMissErrIndDl_g at the end of EL */
        totalTickMissErrIndDl_g[execLegFuncArg_p->cellIndex] = 0;
    } 
    else
    {
        static UInt32 numberOfSubFrames[MAX_NUM_CELL];
        if (numberOfSubFrames[execLegFuncArg_p->cellIndex] ==
                (execLegFuncArg_p->execLegSpecificArg1 -1))
        {
#ifdef FDD_CONFIG
            /* + SPR 17733 */
            expireAllNodeFromHarqTimerListForMsg4(execLegFuncArg_p->execLegSpecificArg1,
                    execLegFuncArg_p->subFrame,
                    DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                    execLegFuncArg_p->cellIndex);
            /* - SPR 17733 */
#endif
            expireAllNodeFromHarqTimerList(execLegFuncArg_p->execLegSpecificArg1,
                    execLegFuncArg_p->subFrame,
                    DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                    execLegFuncArg_p->cellIndex);
            numberOfSubFrames[execLegFuncArg_p->cellIndex] = 0;
        }
        else
        {
            numberOfSubFrames[execLegFuncArg_p->cellIndex]++;
        }
    }
#elif TDD_CONFIG
    tddExpireAllNodeFromHarqTimerList(execLegFuncArg_p->execLegSpecificArg1,
            execLegFuncArg_p->advanceDLSubframe,
            DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
            execLegFuncArg_p->advanceDLSystemframe
            /* contains FALSE value for freeHarqOnly if HARQ expiry was pending on current tick*/
            ,execLegFuncArg_p->execLegSpecificArg2
            ,execLegFuncArg_p->cellIndex
            );
    /* CA Changes end  */
#endif
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit .",__func__);
}
/*ca chngs phase 3 start*/

/*****************************************************************************
 * Function Name  : ElProcessDLCATimerExpiry 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function when called processes timer expiry of DL
 *                  harq processes.execLegSpecificArg1 will provide number of
 *                  frames after wich expireAllNodeFromCATimerList should 
 *                  be invoked.
 *****************************************************************************/
void ElProcessDLCATimerExpiry(ExecutionLegFunctionArg * execLegFuncArg_p)
{
    if(IS_CA_ENABLED()) 
    {
        /* +- SPR 17777 */
#ifdef FDD_CONFIG
#ifdef OVERSHOOT_CORRECTION
    expireAllNodeFromCATimerList(totalNumberOfTickDiffDl_g[execLegFuncArg_p->cellIndex] ,
            execLegFuncArg_p->subFrame,
            DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
            execLegFuncArg_p->cellIndex);
#else
    /* SPR 14611 */
    expireAllNodeFromCATimerList(1,
                execLegFuncArg_p->subFrame,
                DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                execLegFuncArg_p->cellIndex);
    /* SPR 14611 */
#endif /*OVERSHOOT_CORRECTION*/
    /* +- SPR 17777 */
#else
#ifdef OVERSHOOT_CORRECTION
    expireAllNodeFromCATimerList(totalNumberOfTickDiffDl_g[execLegFuncArg_p->cellIndex] ,
            execLegFuncArg_p->cellIndex);
#else
    /* SPR 14611 */
    expireAllNodeFromCATimerList(1,
                execLegFuncArg_p->cellIndex);
    /* SPR 14611 */
#endif /*OVERSHOOT_CORRECTION*/
#endif
/* SPR 14619 */
/* SPR 14619 */
#if defined (TDD_CONFIG)  && !defined SEM_SYNC
    if(CHECK_CELL_STATE(!execLegFuncArg_p->cellIndex) && cellElSyncTracker[!execLegFuncArg_p->cellIndex] == INVALID_SYNC) 
    {
         if(getCurrentTickForCell(!execLegFuncArg_p->cellIndex) == getCurrentTickForCell(execLegFuncArg_p->cellIndex))
         {
                eventfd_write_wrapper(ev_fd_sfi_g[execLegFuncArg_p->cellIndex],1);
         }
    }
#endif    
/* SPR 14619 */
/* SPR 14619 */
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit .",__func__);
    }
}
/*ca chngs phase 3 end*/
/*****************************************************************************
 * Function Name  : ElProcessULHarqTimerExpiry
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function when called processes timer expiry of DL
 *                  harq processes.execLegSpecificArg1 will provide number of
 *                  frames after wich expireAllNodeFromHarqTimerList should
 *                  be invoked.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessULHarqTimerExpiry(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef TDD_CONFIG
    /* SPR 11110 fix start */
    UInt8 allHarqFree = LTE_FALSE;
    /* SPR 11110 fix End */

    UInt8 prevULSubFrame = execLegFuncArg_p->execLegSpecificArg1;
    /* SPR 15909 fix start */
    tickType_t processingTTI = execLegFuncArg_p->advanceDLGlobalTick;
    /* SPR 15909 fix end */
    UInt16 processingSFN = execLegFuncArg_p->advanceDLSystemframe;
    UInt16 hiDLSF = execLegFuncArg_p->advanceDLSubframe;
    UInt16 maxTickDiff = MAC_MAX_VAL( 
            totalTickMissErrIndUl_g[execLegFuncArg_p->cellIndex], 
            totalNumberOfTickDiffUl_g[execLegFuncArg_p->cellIndex] );
    if ( maxTickDiff )
    {
        /* SPR 15909 fix start */
        tickType_t ulTTI = processingTTI - getHiPduSubframeNum( prevULSubFrame, 
                execLegFuncArg_p->cellIndex);
        /* SPR 15909 fix end */
        UInt8 loopNumberOfSubFrames = maxTickDiff;
        /* SPR 11230 fix start */
        UInt8 freeHarqOnly = execLegFuncArg_p->execLegSpecificArg2;

        /* SPR 11110 fix start */
        /* Check if it is the error indication scenario and 
         * above threshold. In case of above threshold value, freeHarqOnly 
         * will be set to TRUE else it will be FALSE as 
         * calculateHarqExpTickMissAndArgs will not be called. */
        if( 0 < totalTickMissErrIndUl_g[execLegFuncArg_p->cellIndex] && 
            LTE_TRUE == freeHarqOnly )
        {
            /* Need to free all the nodes */
            loopNumberOfSubFrames = MAX_NUM_OF_TICK_MISSED;
            allHarqFree = LTE_TRUE;
        }
        else if( 0 == totalTickMissErrIndUl_g[execLegFuncArg_p->cellIndex] )
        {
            if( maxTickDiff >= MAX_NUM_OF_TICK_MISSED)
            {
                freeHarqOnly = LTE_TRUE;
                allHarqFree = LTE_TRUE;
                /* SPR 11110 fix end */
                loopNumberOfSubFrames = MAX_NUM_OF_TICK_MISSED;
            }
            else
            {
                /* Put the entry for re-transmissions as the value is tick
                 * miss diff is less than thresold */
                freeHarqOnly = LTE_FALSE;
            }
        }
        /* SPR 11230 fix end */

        do
        {
            if (UL == getSubframeType(prevULSubFrame, 
                        execLegFuncArg_p->cellIndex))
            {
                processingTTI = ulTTI + getHiPduSubframeNum(prevULSubFrame, 
                        execLegFuncArg_p->cellIndex);
                processingSFN = MODULO_ONEZEROTWOFOUR( 
                        processingTTI/MAX_SUB_FRAME);
                hiDLSF = (processingTTI)%MAX_SUB_FRAME;
                expireAllNodeFromULHarqTimerList( 
                        processingTTI, processingSFN, prevULSubFrame,
                        hiDLSF, freeHarqOnly, 
                        /* SPR 11110 fix start */
                        allHarqFree ,
                        /* SPR 11110 fix end */
                        execLegFuncArg_p->cellIndex);
            }

            if ( prevULSubFrame == 0)
            { 
                prevULSubFrame = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - 1;
            }
            else
            {
                --prevULSubFrame;
            }
            ulTTI--;

        }while(--loopNumberOfSubFrames);
    }
    else
    {
        expireAllNodeFromULHarqTimerList ( 
                processingTTI,
                processingSFN,
                prevULSubFrame,
                hiDLSF, 
                execLegFuncArg_p->execLegSpecificArg2,
                /* SPR 11110 fix start */
                allHarqFree,
                /* SPR 11110 fix end */
                execLegFuncArg_p->cellIndex);
    }

#elif FDD_CONFIG
/*SPR_11288_Fix Start */
#ifdef ULHARQ_TIMER_PROC
/*SPR_11288_Fix End */
    UInt16 maxTickDiff = MAC_MAX_VAL( 
            totalTickMissErrIndUl_g[execLegFuncArg_p->cellIndex], 
            totalNumberOfTickDiffUl_g[execLegFuncArg_p->cellIndex] );
    if ( maxTickDiff )
    {
        expireAllNodeFromULHarqTimerForMsg3List( maxTickDiff,
                execLegFuncArg_p->subFrame,
                DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                execLegFuncArg_p->globalTick,
                execLegFuncArg_p->sysFrame,
                execLegFuncArg_p->cellIndex);

        expireAllNodeFromULHarqTimerList( maxTickDiff,
                execLegFuncArg_p->subFrame,
                DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                execLegFuncArg_p->globalTick,
                execLegFuncArg_p->sysFrame,
                execLegFuncArg_p->cellIndex);
    }
    else
    {
        expireAllNodeFromULHarqTimerForMsg3List(
                execLegFuncArg_p->execLegSpecificArg1,
                execLegFuncArg_p->subFrame,
                DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                execLegFuncArg_p->globalTick,
                execLegFuncArg_p->sysFrame,
                execLegFuncArg_p->cellIndex);

        expireAllNodeFromULHarqTimerList(execLegFuncArg_p->execLegSpecificArg1,
                execLegFuncArg_p->subFrame,
                DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                execLegFuncArg_p->globalTick,
                execLegFuncArg_p->sysFrame,
                execLegFuncArg_p->cellIndex);
    }
            
#endif 
#endif /*FDD_CONFIG*/
    /* Reset totalTickMissErrIndUl_g at the end of EL */
    totalTickMissErrIndUl_g[execLegFuncArg_p->cellIndex] = 0;
}                                               

/*****************************************************************************
 * Function Name  : ElProcessDLHarqTimerExpiryIfTimeLeft 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function whn called processes timer expiry of DL
 *                  harq processes, only if sufficient time left in a tick.
 *                  The minimum time required to execute this leg such that
 *                  the execution flow completes whole one loop in one tick is
 *                  specified through execLegSpecificArg1.execLegSpecificArg1 
 *                  will provide number of frames after wich 
 *                  expireAllNodeFromHarqTimerList should be invoked.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessDLHarqTimerExpiryIfTimeLeft(ExecutionLegFunctionArg * 
/* SPR 5599 changes end (ZIP ID 129063) */
                                                               execLegFuncArg_p)
{
#ifdef FDD_CONFIG
    /** CA-TDD Changes Start **/
    static UInt32 numberOfSubFrames[MAX_NUM_CELL];
    if (numberOfSubFrames[execLegFuncArg_p->cellIndex] == execLegFuncArg_p->execLegSpecificArg1 -1)
    {
        expireAllNodeFromHarqTimerList(execLegFuncArg_p->execLegSpecificArg1,
                                                 execLegFuncArg_p->subFrame,
                DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                execLegFuncArg_p->cellIndex);
        numberOfSubFrames[execLegFuncArg_p->cellIndex] = 0;
    }
    else
    {
        numberOfSubFrames[execLegFuncArg_p->cellIndex]++;
    }
#elif TDD_CONFIG
 tddExpireAllNodeFromHarqTimerList(execLegFuncArg_p->execLegSpecificArg1,
                                          execLegFuncArg_p->subFrame,
                                          DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY,
                                          execLegFuncArg_p->sysFrame
            /* contains FALSE value for freeHarqOnly if HARQ expiry was pending on current tick*/
            ,execLegFuncArg_p->execLegSpecificArg2
            ,execLegFuncArg_p->cellIndex
                                         );
#endif
}

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : ElSendPDCCHTriggerToUL 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends PDCCH Trigger to Uplink.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElSendPDCCHTriggerToUL(UInt32 pdcchContainer)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(pdcchContainer)
    /* +- SPR 17777 */
    /*SPR_7217_changes_start*/
    //semPost(&pdcchTriggerToPuschSem_g);
    /*SPR_7217_changes_end*/
}


/*****************************************************************************
 * Function Name  : checkForPDCCHReadyTrigger 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : semVal
 * Description    : This function checks for PDCCH Ready Trigger.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
/* + SPR 17439 */
UInt32 checkForPDCCHReadyTrigger(void)
/* - SPR 17439 */    
/* SPR 5599 changes end (ZIP ID 129063) */
{
    SInt32 semVal = -1;
    /*SPR_7217_changes_start*/
    //semGetVal(&pdcchTriggerToPuschSem_g,&semVal);
    /*SPR_7217_changes_end*/

    return semVal;
}


/*****************************************************************************
 * Function Name  : ElWaitForPDCCHReadyTrigger
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion waits for the PDCCH Trigger. 
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElWaitForPDCCHReadyTrigger(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
   /* +- SPR 17777*/
   (void)execLegFuncArg_p;
   /* +- SPR 17777*/
    /*SPR_7217_changes_start*/
    //semWait(&pdcchTriggerToPuschSem_g);
    /*SPR_7217_changes_end*/
}

#endif
/*****************************************************************************
 * Function Name  : ElProcessUlRandomAccess
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This will calculate the number of RAR to be schedulted in
 *                  this TTI based on the available RBs and calls the 
 *                  processRaReqQueue() and processNegRaRspQueue().
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessUlRandomAccess(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT_TDD)
#ifdef TDD_CONFIG
     ULDLSPFrame frameType = UL;
#endif
#endif

    CHECK_QOS();

#ifdef FDD_CONFIG
    processRAReqAndNegRARspQueue(execLegFuncArg_p->globalTick,
                                  /* SPR 4692 Fix */                     
                                   MAC_RACH_PDCCH_DELAY,
                                  /* SPR 4692 Fix */
                                  execLegFuncArg_p->sysFrame,
                                  execLegFuncArg_p->subFrame,
                                  /*CA Changes start  */
                                  execLegFuncArg_p->cellIndex,
                                  execLegFuncArg_p);
                                  /*CA Changes end  */
#elif TDD_CONFIG
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT_TDD)
     if (execLegFuncArg_p->execLegSpecificArg1 <= 1)
     {
         frameType = DL;
     }
     else
     {
         frameType = UL;
     }
#endif 
     processRAReqAndNegRARspQueue(execLegFuncArg_p->advanceDLGlobalTick,
                 // execLegFuncArg_p->delayToPhy,
                 execLegFuncArg_p->execLegSpecificArg1,
                 execLegFuncArg_p->advanceDLSystemframe,
                 execLegFuncArg_p->advanceDLSubframe
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT_TDD)
                 ,frameType
#endif
                 ,execLegFuncArg_p->cellIndex
                 ,execLegFuncArg_p);

#endif
    
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit",__func__);
}


/*****************************************************************************
 * Function Name  : ElProcessPUSCHForDelayOne
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : Invokes PUSCH processing and should be called from
 *                  the flow whose delay path is 1.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessPUSCHForDelayOne(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef FDD_CONFIG
    processPUSCHLogicForDelayPath2( execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->globalTick,execLegFuncArg_p->subFrame,execLegFuncArg_p->cellIndex,execLegFuncArg_p);
#elif TDD_CONFIG
    /* TDD Config 0 changes start */
    processPUSCHLogicForDelayPath2
        ( execLegFuncArg_p->execLegSpecificArg1,
          execLegFuncArg_p->advanceDLGlobalTick,
          execLegFuncArg_p->advanceDLSystemframe,
          execLegFuncArg_p->advanceDLSubframe,
          execLegFuncArg_p->cellIndex,
          execLegFuncArg_p
        );

    /* In case of TDD Config 0 PUSCH will be called again
     * for resourcse allocation of another U subframe(+7) */
    if (UL_DL_CONFIG_0 == getTddConfig(execLegFuncArg_p->cellIndex))
    {
        processPUSCHLogicForDelayPath2
            ( execLegFuncArg_p->execLegSpecificArg2,
              execLegFuncArg_p->advanceDLGlobalTick,
              execLegFuncArg_p->advanceDLSystemframe,
              execLegFuncArg_p->advanceDLSubframe,
              execLegFuncArg_p->cellIndex,
              execLegFuncArg_p
            );
    }
    /* TDD Config 0 changes end */
#endif
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit . SubFrame No [%d]",__func__,execLegFuncArg_p->subFrame);
}
                        

/*****************************************************************************
 * Function Name  : ElProcessPUSCHForDelayTwo
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : Invokes PUSCH processing and should be called from
 *                  the flow whose delay path is 2.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessPUSCHForDelayTwo(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef FDD_CONFIG
processPUSCHLogicForDelayPath2( execLegFuncArg_p->delayToPhy, 
                                        execLegFuncArg_p->globalTick,
                                        execLegFuncArg_p->subFrame,
                                        execLegFuncArg_p->cellIndex,
                                        execLegFuncArg_p);
#elif TDD_CONFIG
processPUSCHLogicForDelayPath2( execLegFuncArg_p->delayToPhy,
                                execLegFuncArg_p->globalTick,
                                execLegFuncArg_p->sysFrame,
                                execLegFuncArg_p->subFrame,
                                execLegFuncArg_p->cellIndex,
                                execLegFuncArg_p);
#endif                                        
}

/*****************************************************************************
 * Function Name  : ElInvokeDLSpsSchStrategy
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg invokes downlink SPS scheduler strategy.
 *****************************************************************************/
void ElInvokeDLSpsSchStrategy( 
        ExecutionLegFunctionArg * execLegFuncArg_p )
{
    /* + SPS_TDD_Changes */
    UInt32 currentGlobalTTITickCount = 0;
    currentGlobalTTITickCount = getCurrentTick() ;
#ifdef FDD_CONFIG
    processSPSDLScheduling(currentGlobalTTITickCount, 
            execLegFuncArg_p->delayToPhy, execLegFuncArg_p->cellIndex);
#elif TDD_CONFIG
    /* During the special subframe the DL SPS starategy need to be done based 
     * on Special Subframe configuration. Hence a function pointer table 
     * is formed. The DL SPS startegy will not be performed during the special
     * subframe configuration 0,1,5,6 and a dummy function wil be called.
     * For this operation the PDSCH Indexer is used.
     */
    dlSPSSchedulerStrategyFtnPtrTable 
        [tddMacPdcchPdschFtnPtrIndexer_p[execLegFuncArg_p->cellIndex]
        [execLegFuncArg_p->execLegSpecificArg1]]
        (currentGlobalTTITickCount,
         DL_ADVANCE_TICK,
         execLegFuncArg_p->cellIndex);
#endif
    /* - SPS_TDD_Changes */
}

/*****************************************************************************
 * Function Name  : ElupdateDlTopSchListScellState 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg whn called does the functionality of 
 *                  updating DL top schedulable list for corresponding cell.
 *****************************************************************************/
/*ca chngs phase 3 start*/
 void ElupdateDlTopSchListScellState(ExecutionLegFunctionArg *execLegFuncArg_p)
{
    /* CA - Ph2 Integration Fix Start */
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
    if(IS_CA_ENABLED()) 
    {
        InternalCellIndex isLowestCellIndex = 0;
        semWait(&cellSyncSem_g);   
        isLowestCellIndex = (execLegFuncArg_p->cellIndex == macParams_g.lowestRunningCellIndex) ;
        semPost(&cellSyncSem_g);
        if (isLowestCellIndex)
        {
            updateTopSchedulableUEInfoList(execLegFuncArg_p->cellIndex);
        }
        scellStateMngrExpireAllScellDeactTimerOfCurrentTick((getCurrentTick() + PHY_DL_DELAY), execLegFuncArg_p->cellIndex/*internalCellIndex*/);
        RESET_SCELL_UE_QLOAD_CHECKLIST(execLegFuncArg_p->cellIndex);
        scellStateMngrProcessFreeHarqBufferQueue(execLegFuncArg_p->cellIndex);
    }
#else 
    (void)execLegFuncArg_p;/*SPR 18490+-*/
#endif    
    /* CA - Ph2 Integration Fix End */

    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit . SubFrame No [%d] global tick [%d]",__func__, \
                              execLegFuncArg_p->subFrame,execLegFuncArg_p->globalTick);
}
/*ca chngs phase 3 end*/
/*****************************************************************************
 * Function Name  : ElProcessUlSpsStrategy
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg invokes uplink SPS scheduler strategy.
 *****************************************************************************/
void ElProcessUlSpsStrategy(
        ExecutionLegFunctionArg * execLegFuncArg_p )
{
    UInt32 qCount = 0;
   /* SPR 15909 fix start */
    tickType_t currentGlobalTTITickCount = 0;
    /* SPR 15909 fix end */

    /*! \code 
     * Process the SPS startegy queues in following order:
     * 1. UL Status Queue, ulSpsStatusQ_g.
     * 2. Deactivation Queue, ulSpsDeactQ_g
     * 3. Activation Queue, ulSpsActQ_g
     * 4. Re-activation Queue, ulSpsReactQ_g
     * 5. SPS Occassion List and SDL
     * \endcode
     * */
    
    /* Initialize the scheduled UE count in the UL QOS Strategy Schedule Info
     */
    /* CA Changes start */
    ulStrategyScheduleInfo_gp[execLegFuncArg_p->cellIndex]->scheduledUeCount = 0;
    currentGlobalTTITickCount = execLegFuncArg_p->globalTick;
    if(cellSpecificParams_g.cellConfigAndInitParams_p[execLegFuncArg_p->cellIndex]\
        ->initParams_p->ulSchdConfig.ulSchedulerStrategy == UL_QOS_SCHEDULING)
    {
        processBSRZeroNonZeroTriggerQueueQos(execLegFuncArg_p->cellIndex,execLegFuncArg_p);
    }
    else
    {
        /*SPR 7470 Fix start */
        static UInt32 processBSRZeroNonZeroQueueCnt[MAX_NUM_CELL] = {0};
        if((processBSRZeroNonZeroQueueCnt[execLegFuncArg_p->cellIndex] == 0))
        {
            processBSRZeroNonZeroTriggerQueue(currentGlobalTTITickCount, execLegFuncArg_p->cellIndex);
            UInt32 tempTotalActiveUE = listCount(nonZeroBsrList_gp[execLegFuncArg_p->cellIndex]);

            if(tempTotalActiveUE)
            {
                if(1 < cellSpecificParams_g.cellConfigAndInitParams_p\
                   [execLegFuncArg_p->cellIndex]->initParams_p->ulSchdConfig.\
                    maxUEsToBeScheduledUL)
                {
                    processBSRZeroNonZeroQueueCnt[execLegFuncArg_p->cellIndex] = 
                        ((tempTotalActiveUE -1) / 
                         cellSpecificParams_g.cellConfigAndInitParams_p[execLegFuncArg_p->cellIndex]->initParams_p->ulSchdConfig.maxUEsToBeScheduledUL);
                }
                else
                {
                    processBSRZeroNonZeroQueueCnt[execLegFuncArg_p->cellIndex] = (tempTotalActiveUE -1);
                }
            }
        }
        else
        {
            processBSRZeroNonZeroQueueCnt[execLegFuncArg_p->cellIndex]--;
        }
        /*SPR 7470 Fix end */
    }
    if (cellSpecificParams_g.cellConfigAndInitParams_p[execLegFuncArg_p->cellIndex]->initParams_p->spsSysWideParam.enableSps)
    {
        qCount = QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[execLegFuncArg_p->cellIndex] );
        if ( 0 != qCount )
        {
            macUlSpsProcessStatusQ( qCount 
                    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                    ,execLegFuncArg_p->delayToPhy 
                    ,execLegFuncArg_p->cellIndex
#else
                    ,DL_ADVANCE_TICK,
                    execLegFuncArg_p->cellIndex
#endif
                    /* - SPS_TDD_Changes */
                    );
        }
        qCount = QCOUNT_MAC_UL_SPS_Q( ulSpsDeactQ_gp[execLegFuncArg_p->cellIndex] );
        if ( 0 != qCount )
        {
            macUlSpsProcessDeactQ( qCount 
                    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                    ,execLegFuncArg_p->delayToPhy 
                    ,execLegFuncArg_p->cellIndex
#else
                    ,DL_ADVANCE_TICK,
                    execLegFuncArg_p->cellIndex
#endif
                    /* - SPS_TDD_Changes */
                    );
        }
        qCount = QCOUNT_MAC_UL_SPS_Q( ulSpsActQ_gp[execLegFuncArg_p->cellIndex] );
        if ( 0 != qCount )
        {
            macUlSpsProcessActQ( qCount 
                    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                    ,execLegFuncArg_p->delayToPhy 
                    ,execLegFuncArg_p->cellIndex
#else
                    ,DL_ADVANCE_TICK,
                    execLegFuncArg_p->cellIndex
#endif
                    /* - SPS_TDD_Changes */
                    );
        }
        /* Process Occasion List and SDL */
        processULSpsOccasion( 
                /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                currentGlobalTTITickCount,
                execLegFuncArg_p->delayToPhy,
                execLegFuncArg_p->cellIndex
#else
                execLegFuncArg_p->advanceDLGlobalTick, 
                DL_ADVANCE_TICK ,
                execLegFuncArg_p->cellIndex
#endif
                );
        /* - SPS_TDD_Changes */
    }
    /* CA Changes end */
}

/*****************************************************************************
 * Function Name  : ElProcessSpsPdsch
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg invokes SPS PDSCH.
 *****************************************************************************/
void ElProcessSpsPdsch( ExecutionLegFunctionArg * execLegFuncArg_p )
{
    UInt32 qCount = 0;
    InternalCellIndex internalCellIndex = execLegFuncArg_p->cellIndex;
    if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
            spsSysWideParam.enableSps)
    {
        qCount = QCOUNT_MAC_DL_SPS_Q( dlSpsResRsvQ_gp[internalCellIndex] );
        qCount = (qCount < MAX_DL_RES_RSV_REQ_PROCESS) ? 
                           qCount : MAX_DL_RES_RSV_REQ_PROCESS;
        if( 0 != qCount )
        {
            dlSpsResourceReserveHandler( qCount,internalCellIndex );
        }

        qCount = QCOUNT_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex] );
        if( 0 != qCount )
        {
            dlSpsResourceReleaseHandler( qCount ,internalCellIndex);
        }
    }
}

/****************************************************************************
* Function Name: ElProcessEcidReports
* Inputs       : execLegFuncArg_p - This contains paramters like global tick,
*                                   delay to phy , sub frame number etc 
*                                   and also few arguments specifically
*                                   necessary for certain execution legs.
* Outputs      : None
* Returns      : None
* Description  : This leg invokes E-CID reports.
****************************************************************************/
void ElProcessEcidReports( ExecutionLegFunctionArg * execLegFuncArg_p )
{

    /*CA Changes start  */
    processEcidReports(execLegFuncArg_p->cellIndex/*execLegFuncArg_p->subFrame,execLegFuncArg_p->delayToPhy,*/
            /*execLegFuncArg_p->globalTick*/);
    /*CA Changes end  */

}
/* + PRS_CHANGES */
/****************************************************************************
* Function Name: ElProcessPrsTransmission
* Inputs       : execLegFuncArg_p - This contains paramters like global tick,
*                                   delay to phy , sub frame number etc and 
*                                   also few arguments specifically necessary 
*                                   for certain execution legs.
* Outputs      : None
* Returns      : None
* Description  : This leg invokes PRS Manager.
****************************************************************************/
void ElProcessPrsTransmission( ExecutionLegFunctionArg * execLegFuncArg_p )
{
    processPrsTransmission(/*execLegFuncArg_p->subFrame,execLegFuncArg_p->delayToPhy,*/
            /*CA Changes start  */
            execLegFuncArg_p->globalTick, execLegFuncArg_p->cellIndex);
            /*CA Changes end  */

}
/* - PRS_CHANGES */

/*****************************************************************************
 * Function Name  : ElProcessSpsPusch
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg invokes SPS PUSCH.
 *****************************************************************************/
void ElProcessSpsPusch( ExecutionLegFunctionArg * execLegFuncArg_p )
{
    UInt32 qCount = 0;
    /* CA Changes start */
    if (cellSpecificParams_g.cellConfigAndInitParams_p[execLegFuncArg_p->cellIndex]-> \
            initParams_p->spsSysWideParam.enableSps)
    {
        qCount = QCOUNT_MAC_UL_SPS_Q(ulSpsResRsvQ_gp[execLegFuncArg_p->cellIndex]);
        qCount = (qCount < MAX_UL_RES_RSV_REQ_PROCESS) ? 
                           qCount : MAX_UL_RES_RSV_REQ_PROCESS;
        if( 0 != qCount )
        {
            ulSpsResourceReserveHandler( qCount 
                    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                    ,execLegFuncArg_p->delayToPhy 
                    ,execLegFuncArg_p->cellIndex
#elif TDD_CONFIG
                    ,DL_ADVANCE_TICK
                    ,execLegFuncArg_p->advanceDLSubframe,
                    execLegFuncArg_p->cellIndex
#endif
                    );
            /* - SPS_TDD_Changes */
        }

        qCount = QCOUNT_MAC_UL_SPS_Q(ulSpsResRelQ_gp[execLegFuncArg_p->cellIndex]);
        if( 0 != qCount )
        {
            ulSpsResourceReleaseHandler(qCount, execLegFuncArg_p->cellIndex);
        }
        /* CA Changes end */
    }
}

/*****************************************************************************
 * Function Name  : ElInvokeDLSCHStrategy
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg invokes downlink scheduler strategy.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElInvokeDLSCHStrategy(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef LTE_EMBMS_SUPPORTED
    if(isEmbmsValidSf_g[execLegFuncArg_p->cellIndex])
    {
        /* Return if current sub frame is eMBMS Sub frame */
        return ;
    }
#endif 
    if(DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p\
            [execLegFuncArg_p->cellIndex]->initParams_p->dlSchdConfig.\
            dlSchedulerStrategy)
    {
	    /** SPR 21608 Changes Start **/
	    /** SPR 13599 Changes Start **/
	    UInt8 isCaEnable = IS_CA_ENABLED();
	    /** SPR 13599 Changes End **/
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
	    UInt8 isLowestCellIndex =  0;
	    UInt8 isStrategyToBeCalled = 0;
	    /** SPR 21608 Changes End **/
	    /** SPR 13599 Changes Start **/
	    if (isCaEnable)
	    { 
		    semWait(&cellSyncSem_g);
		    isLowestCellIndex = (macParams_g.lowestRunningCellIndex == execLegFuncArg_p->cellIndex); 
		    isStrategyToBeCalled = (macParams_g.lastStrategyExecutionTTI != execLegFuncArg_p->globalTick);
		    semPost(&cellSyncSem_g);
	    }
#endif 
	    if (!isCaEnable 
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
			    || (isLowestCellIndex && isStrategyToBeCalled)
#endif
	       )
	    {
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
		    if (isCaEnable)
			    /** SPR 13599 Changes End **/
		    {
			    semWait(&cellSyncSem_g);
			    macParams_g.lastStrategyExecutionTTI = execLegFuncArg_p->globalTick;
			    semPost(&cellSyncSem_g);
		    }
#endif
#ifdef FDD_CONFIG
		    dlQosProcessSchedulerStrategy (
				    execLegFuncArg_p->globalTick,
				    execLegFuncArg_p->delayToPhy,
				    execLegFuncArg_p->cellIndex);

#elif TDD_CONFIG
		    /* During the special subframe the DL starategy need to be done based 
		     * on Special Subframe configuration. Hence a function pointer table 
		     * is formed. The DL startegy will not be performed during the special
		     * subframe configuration 0,1,5,6 and a dummy function wil be called.
		     * For this operation the PDSCH Indexer is used.
		     * 
		     * For QOS a Different funtion pointer Table is maintained 
		     */
		    /** CA-TDD Changes Start **/
#if 0
                    if(getCurrentTickForCell(0) != getCurrentTickForCell(1))
                      fprintf(stderr,"cellIndex [%d] tick mismatch cell_0 %llu and cell_1 %llu \n",execLegFuncArg_p->cellIndex,getCurrentTickForCell(0),getCurrentTickForCell(1));
#endif
		    dlQosSchedulerStrategyFtnPtrTable 
			    [tddMacPdcchPdschFtnPtrIndexer_p[execLegFuncArg_p->cellIndex]
			    [execLegFuncArg_p->execLegSpecificArg1]]
			    (execLegFuncArg_p->advanceDLGlobalTick,
			     execLegFuncArg_p->delayToPhy
			     ,execLegFuncArg_p->cellIndex
			    );
		    /** CA-TDD Changes End **/
#endif
	    }
    }
    else
    {
#ifdef FDD_CONFIG
        static UInt8 numOfSubframes;

        if (!numOfSubframes)
        {
            numOfSubframes = processDLSchedulerStrategy(
                                execLegFuncArg_p->globalTick,
                    execLegFuncArg_p->delayToPhy,
                    /*CA Chnages start*/
                    execLegFuncArg_p->cellIndex);
            /*CA Changes end*/
        if(numOfSubframes)
        {
            numOfSubframes--;
        }
    }
    else
    {
        numOfSubframes--;
    }
#elif TDD_CONFIG
    /* During the special subframe the DL starategy need to be done based 
     * on Special Subframe configuration. Hence a function pointer table 
     * is formed. The DL startegy will not be performed during the special
     * subframe configuration 0,1,5,6 and a dummy function wil be called.
     * For this operation the PDSCH Indexer is used.
     */
        /** CA-TDD Changes Start **/
        dlSchedulerStrategyFtnPtrTable 
            [tddMacPdcchPdschFtnPtrIndexer_p[execLegFuncArg_p->cellIndex]
                [execLegFuncArg_p->execLegSpecificArg1]]
        (execLegFuncArg_p->advanceDLGlobalTick,
         execLegFuncArg_p->delayToPhy
         ,execLegFuncArg_p->cellIndex
        );
        /** CA-TDD Changes End **/
#endif
    }
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit",__func__);
}

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : ElProcessUlSchStrategyIfNotAlreadyDone
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg invokes processULSchedulerStrategy if 
 *                  checkIfUlStrategyAlreadyDone_g is not set. This flag
 *                  is set by processUlSchStrategyIfPdcchTriggerIsNotReady.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessUlSchStrategyIfNotAlreadyDone
/* SPR 5599 changes end (ZIP ID 129063) */
                           (ExecutionLegFunctionArg * execLegFuncArg_p)
{ 
    static UInt8 numOfSubframes;

    if (!checkIfUlStrategyAlreadyDone_g)
    {

        if (!numOfSubframes)
        {
            numOfSubframes = processULSchedulerStrategy(execLegFuncArg_p->globalTick,
                                  execLegFuncArg_p->delayToPhy,execLegFuncArg_p->cellIndex,execLegFuncArg_p);
            if (numOfSubframes)
            {
                numOfSubframes--;
            }
        }
        else
        {
            numOfSubframes--;
        }
    }
    else
    {
        checkIfUlStrategyAlreadyDone_g = 0;
    }
}
#endif

/*****************************************************************************
 * Function Name  : ElInvokeULSCHStrategy
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg invokes Uplink Scheduler Strategy which
 *                  allocates the Uplink RBs to UE for one or more than one 
 *                  sub-frame. processULSchedulerStrategy returs number of 
 *                  subframes for which it is scheduling data , hence we
 *                  need not invoke this function until those many number of
 *                  sub frames.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElInvokeULSCHStrategy(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Entry. SubFrame No [%d]" ,__func__,execLegFuncArg_p->subFrame);
/*SPR 22919 +-*/

/*SPR 22919 Start*/
#ifdef FDD_CONFIG
    static UInt8 numOfSubframes;
/*SPR 22919 End*/


    if (!numOfSubframes)
    {
        numOfSubframes = processULSchedulerStrategy(execLegFuncArg_p->globalTick, 
                                                    execLegFuncArg_p->delayToPhy,
                                                    /*CA Changes start  */
                                                    execLegFuncArg_p->cellIndex,
                                                    execLegFuncArg_p);
                                                    /*CA Changes end  */
        if (numOfSubframes)
        {
            numOfSubframes--;
        }
    }
    else
    {
        numOfSubframes--;
    }
#elif TDD_CONFIG
processULSchedulerStrategy(execLegFuncArg_p->advanceDLGlobalTick,
                                      execLegFuncArg_p->delayToPhy,
                                      execLegFuncArg_p->cellIndex,
                                      execLegFuncArg_p,
                                      /* TDD Config 0 changes start */
                                      execLegFuncArg_p->execLegSpecificArg1);
                                      /* TDD Config 0 changes end */


#endif
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit . SubFrame No [%d]",__func__,execLegFuncArg_p->subFrame);

}

/*****************************************************************************
 * Function Name  : ElProcessMiscUpdates
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg processes misc updates like reconfig, etc.
 ******************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessMiscUpdates(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    InternalCellIndex internalCellIndex = execLegFuncArg_p->cellIndex;
    processCellReconfigUpdate( execLegFuncArg_p->delayToPhy , internalCellIndex);

    /* process RRM Reconfiguration */
    processDLRrmReconfiguration( internalCellIndex );
    processULRrmReconfiguration( internalCellIndex );

    processEicicConfiguration(internalCellIndex); /* EICIC +-*/

    if ( rrmMacCellGlobal_gp[internalCellIndex]->ulPowerMaskValUpdated)
    {
        /*CA Changes start  */
        ulPowerMaskIndex_g[execLegFuncArg_p->cellIndex] = !ulPowerMaskIndex_g[execLegFuncArg_p->cellIndex];
        /*CA Changes end  */
        rrmMacCellGlobal_gp[internalCellIndex]->ulPowerMaskValUpdated = FALSE;
    }
    /* ICIC changes start */
    /* prepare periodic report and send to RRM*/
    checkForRRMPerriodicReportOpp(internalCellIndex);
    /* ICIC changes end */
/* SPR 23402 Changes Start */
    /* MOCN Changes Start */
#ifdef KPI_STATS
    if (mocnOperatorParams_g.isMocnEnabled && (getCurrentTick()%mocnOperatorParams_g.prbUsageMeasResetPeriod == 0))
    {
        calculateMocnPRBUsageHistory(internalCellIndex);
    } 
#endif
    /* MOCN Changes End */
/* SPR 23402 Changes End */

    /*SPR 16855 +-*/
}


/*****************************************************************************
 * Function Name  : ElProcesRlcPreOpportunity
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     cell index ,delay to phy
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg performs Pre opportunity node creation
 *                  by RLC to be used by MAC in embms strategy
 *****************************************************************************/
#ifdef LTE_EMBMS_SUPPORTED
void ElProcesRlcPreOpportunity(ExecutionLegFunctionArg *execLegFuncArg_p)
{
    execLegFuncArg_p=PNULL;
}
#endif

/*****************************************************************************
 * Function Name  : ElProcessRxUntilSubFrameInd
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg processes messages received to MAC from 
 *                  PHY ,until subframe indication is received.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessRxUntilSubFrameInd(ExecutionLegFunctionArg * execLegFuncArg_p)
/* SPR 5599 changes end (ZIP ID 129063) */
{
    /*** SPR 2934 **/
    UInt8 memUsage = 0;
    /* SPR 17903 Fix Start */
    UInt8 msgUsage = 0;
    /* SPR 17903 Fix End */
    /*** SPR 2934 **/
#ifdef DL_UL_SPLIT
    /* CA Changes start */
    InternalCellIndex internalCellIndex = execLegFuncArg_p->cellIndex;

    /* HSM_CHG_START */
    l2ThreadHealthStoreTick_g.rlcMacThread[internalCellIndex].macThread[MAC_UL_THREAD] = macTTITickCount_g;
    /* CA Changes end */
#endif
    /* HSM_CHG_END */
#ifdef UTFWK_SIMULATION
    TimeSpec currentTime;
    TimeSpec t1;
    UInt8  timer_ret=0;
    long nsecs;
    long tickTime = 1 * 988750;

    clock_gettime_real_time(&currentTime);
    nsecs = currentTime.tv_nsec - prev_currentTime.tv_nsec;
    ctr1++;
    if(tickTime > nsecs)
    {
        ctr2++;
        t1.tv_sec = 0;
        t1.tv_nsec = (tickTime - nsecs);
        timer_ret = nanoSleep(&t1,NULL);
    }
    if(nsecs >= tickTime)
        ctr3++;
    clock_gettime_real_time(&prev_currentTime);

    RDTSC_AND_CLOCK_START_FDD(MAC_PROCESS_RX_UNTIL_SUBFRAME_IND);
    updateTickAndSendAckNack(execLegFuncArg_p->subFrame);
    PROCESS_TA_REQ();
    /* CA Stats Changes Start */
    RDTSC_AND_CLOCK_END_MAC(MAC_PROCESS_RX_UNTIL_SUBFRAME_IND,
            execLegFuncArg_p->cellIndex);
#ifdef SSI_DEBUG_STATS 
    INCREMENT_WRITE_TTI_INDEX( execLegFuncArg_p->globalTick,
            execLegFuncArg_p->cellIndex);
    /* CA Stats Changes End */
#endif
    return;
#endif
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
    if(IS_CA_ENABLED())
    {
        semWait(&cellSyncSem_g);
        cellElSyncTracker[execLegFuncArg_p->cellIndex] = STRATEGY_SYNC; 
        semPost(&cellSyncSem_g);
    }
#endif
    /* RX_IND_PORTING Changes */
    processRxSubFrameInd(execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->execLegSpecificArg1,
            /*CA Changes start  */       
            execLegFuncArg_p->cellIndex);
    /*CA Changes start  */       

    /*SPR 16855 +-*/
    /*** SPR 2934 **/
    memUsage = qvMemPoolGetUsagePercentage();

    if(MAC_MEM_POOL_LOW_WATERMARK >= memUsage)
    {
        memoryOverflow_g = FALSE;    
    }
    else if(MAC_MEM_POOL_HIGH_WATERMARK <= memUsage)
    {
        memoryOverflow_g = TRUE;     
        LOG_MAC_MSG( L2_MEM_POOL_HIGH_WATERMARK, LOGWARNING, L2_SYS_FAIL,
                execLegFuncArg_p->globalTick, memUsage, 0, 0 ,0 ,0, 0 ,0, __func__, "");
    }

    /* SPR 17903 Fix Start */
    msgUsage = qvMsgPoolGetUsagePercentage();
    if(MAC_MSG_POOL_LOW_WATERMARK >= msgUsage)
    {
        msgOverflow_g = FALSE;
    }
    else if(MAC_MSG_POOL_HIGH_WATERMARK <= msgUsage)
    {
        msgOverflow_g = TRUE;
        LOG_MSG( L2_MSG_POOL_HIGH_WATERMARK, LOGWARNING, L2_SYS_FAIL,
                execLegFuncArg_p->globalTick, msgUsage, 0, 0 ,0 ,0, 0 ,0, __func__, "");
    }
    /* SPR 17903 Fix End */
    /*** SPR 2934 **/



#ifdef FDD_CONFIG                                  
#ifndef ONE_FLOW_ONE_DELAY
    triggerSubframeInd();
#endif    
#endif
}


/*****************************************************************************
 * Function Name  : ElProcessRxControlAndNumDataMsgsWithDemux
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg processes certain number of messages
 *                  received to MAC from PHY , which is specified by
 *                  NUM_PHY_MSGS_TO_PROCESS.This leg excludes demux processing,
 *                  demux queues are not populated.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessRxControlAndNumDataMsgsWithDemux(ExecutionLegFunctionArg * 
        /* SPR 5599 changes end (ZIP ID 129063) */
        execLegFuncArg_p)
{
#ifdef UTFWK_SIMULATION
    updateTickAndSendAckNack(execLegFuncArg_p->subFrame);
#else
    /*CA Changes start  */
    if(cellSpecificParams_g.cellConfigAndInitParams_p[execLegFuncArg_p->cellIndex]->\
            cellState == CELL_PHY_RUNNING)
        /*CA Changes end  */
    {
        /* + SPR 13822 Changes */        
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
        if(IS_CA_ENABLED())
        {
            semWait(&cellSyncSem_g);
            cellElSyncTracker[execLegFuncArg_p->cellIndex] = STRATEGY_SYNC; 
            semPost(&cellSyncSem_g);
        }
#endif
        /* - SPR 13822 Changes */        
        processRxSubFrameInd(execLegFuncArg_p->delayToPhy,
                execLegFuncArg_p->execLegSpecificArg1,
                /*CA Changes start  */
                execLegFuncArg_p->cellIndex);
        /*CA Changes end  */

    }    
    else 
    {    
        return;
    }
    /* RX_IND_PORTING Changes */
    /* SPR 3087 Fix begin */
    /*SPR 16855 +-*/
    /* SPR 3087 Fix end */
#endif //UTFWK_ENDIF

    /*Semaphore post for Sub frame Ind to ULRLC Thread*/
#ifdef DL_UL_SPLIT_TDD
    semPostDefault(&waitForSubFrameIndSemArr_g);
#endif
} 

#ifdef DL_UL_SPLIT_TDD
/*****************************************************************************
 * Function Name  : ElProcessSubFrameIndUl 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : 
 * Returns        : 
 * Description    : This execution leg processes messages received to UL MAC
 *                  Thread from PHY ,until subframe indication is received.
 *****************************************************************************/
void ElProcessSubFrameIndUl(ExecutionLegFunctionArg *execLegFuncArg_p)
{
    processRxSubFrameIndUl(execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->execLegSpecificArg1);
}

/*****************************************************************************
 * Function Name  : ElTxHiDci0Msg
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : 
 * Returns        : 
 * Description    : This execution leg tarnsmit HI DCI0 messages received to to
 *                  PHY.
 *****************************************************************************/
void ElTxHiDci0Msg(ExecutionLegFunctionArg *execLegFuncArg_p)
{
    txDLHiDci0Msg(execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->advanceDLGlobalTick,
            execLegFuncArg_p->advanceDLSystemframe,
            execLegFuncArg_p->advanceDLSubframe);

}

/*****************************************************************************
 * Function Name  : ElProcessUlPdcchWithMux
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : 
 * Returns        : 
 * Description    : This execution leg allocate CCE for UL UE. This EL executes
 *                  on UL thread.
 *****************************************************************************/
void ElProcessUlPdcchWithMux(ExecutionLegFunctionArg *execLegFuncArg_p)
{
    allocPdcchForUl(1, /*With mux*/
            execLegFuncArg_p->advanceDLGlobalTick,
            execLegFuncArg_p->advanceDLSystemframe,
            execLegFuncArg_p->advanceDLSubframe,
            execLegFuncArg_p->execLegSpecificArg2,
            execLegFuncArg_p); 

}

/*****************************************************************************
 * Function Name  : ElProcessDlPdcchSync
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : 
 * Returns        : 
 * Description    : This execution leg allocate CCE for UL UE. This EL executes
 *                  on UL thread.
 *****************************************************************************/
void ElProcessDlPdcchSync(ExecutionLegFunctionArg *execLegFuncArg_p)
{
    /*Semaphore post will be done by DL Thread after CCE allocation in DL*/
    PdcchSignalNodeP * node_p = PNULL;

    semWaitDefault(&waitForPDCCHSignalInd_g);
    popNodeCircQ(&pdcchSignalCircQueue_g, (void *)&node_p);

    if(node_p == NULL)
    {
        ltePanic("DL Thread should always insert Node\n");
    }
    else
    {
        freeMemPool(node_p);
    }



}
#ifdef FAPI_CONTROL_FLOW
/*****************************************************************************
 * Function Name  : ElProcessDlPdcchSync
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : void 
 * Returns        : void
 * Description    : This execution leg post semaphore for FAPI message control flow.
 *                  This EL executes on DL thread.
 *****************************************************************************/
void ElFapiDLFlowSyncControl(ExecutionLegFunctionArg *execLegFuncArg_p)
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
    semPostDefault(&waitForFAPIMsgControlFlowInd_g);
}
/*****************************************************************************
 * Function Name  : ElProcessDlPdcchSync
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : 
 * Returns        : 
 * Description    : This execution leg waits on semaphore for FAPI message control flow.
 *                  This EL executes on UL thread.
 *****************************************************************************/
void ElFapiULFlowSyncControl(ExecutionLegFunctionArg *execLegFuncArg_p)
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
    semWaitDefault(&waitForFAPIMsgControlFlowInd_g);
}
#endif
#endif


/*****************************************************************************
 * Function Name  : ElProcessRxControlDataWithoutDemux
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg processes certain number of messages
 *                  received to MAC from PHY , which is specified by
 *                  NUM_PHY_MSGS_TO_PROCESS.This leg excludes demux processing,
 *                  it rather pushes demux requests into demux Queues.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessRxControlDataWithoutDemux(ExecutionLegFunctionArg * 
        /* SPR 5599 changes end (ZIP ID 129063) */
        execLegFuncArg_p)
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
} 


/*****************************************************************************
 * Function Name  : ElProcessNumDemuxMsgs 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function when called processes the demux queue at 
 *                  index equal to execLegSpecificArg1.The second argument 
 *                  passed to processDemuxQueue specifies Number of demuxing
 *                  requests to be processed.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessNumDemuxMsgs(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    processDemuxQueue(execLegFuncArg_p->execLegSpecificArg1,
            NUM_DEMUX_MSGS_TO_PROCESS,
            execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->cellIndex,
            execLegFuncArg_p);
}


/*****************************************************************************
 * Function Name  : ElProcessDemuxQueue 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function when called processes the demux queue at 
 *                  index equal to execLegSpecificArg1.The second argument 
 *                  passed to processDemuxQueue specifies Number of demuxing
 *                  requests to be processed.If this is zero Then the 
 *                  function processDemuxQueue will process Queue count number of
 *                  messages.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessDemuxQueue(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    CHECK_QOS();
    processDemuxQueue(execLegFuncArg_p->execLegSpecificArg1,0,
            execLegFuncArg_p->delayToPhy,
            /*CA Changes start  */
            execLegFuncArg_p->cellIndex,
            execLegFuncArg_p);
    /*CA Changes end  */
}


/*****************************************************************************
 * Function Name  : ElProcessDemuxMsgsIfTimeLeft 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : The execution leg function whn called does the process
 *                  of demultiplexing only if sufficient time left in a tick.
 *                  The minimum time required to execute this leg such that
 *                  the execution flow completes whole one loop in one tick is
 *                  specified through execLegSpecificArg1.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessDemuxMsgsIfTimeLeft(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{

    processDemuxQueue(execLegFuncArg_p->execLegSpecificArg2,0,
            execLegFuncArg_p->delayToPhy,execLegFuncArg_p->cellIndex,execLegFuncArg_p);
}

/*****************************************************************************
 * Function Name  : ElProcessNumRRCAndOAMMsgs 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg processes certain number of RRC/OAM
 *                  messages specified by NUM_RRC_OAM_MSGS_TO_PROCESS.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessNumRRCAndOAMMsgs(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
    /*SPR 16855 +-*/
}


/*****************************************************************************
 * Function Name  : ElProcessNumRRCAndOAMMsgsIfTimeLeft 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : The execution leg function whn called processes received
 *                  RRC and OAM messages ,only if sufficient time left in a tick.
 *                  The minimum time required to execute this leg such that
 *                  the execution flow completes whole one loop in one tick is
 *                  specified through execLegSpecificArg1.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessNumRRCAndOAMMsgsIfTimeLeft(ExecutionLegFunctionArg * 
        /* SPR 5599 changes end (ZIP ID 129063) */
        execLegFuncArg_p)
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
}


/*****************************************************************************
 * Function Name  : ElTransmitDLControMsg 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg transmits only control messages.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElTransmitDLControMsg(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef FDD_CONFIG
    txDLControlMsg( execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->globalTick,
            execLegFuncArg_p->sysFrame,
            execLegFuncArg_p->subFrame,
            execLegFuncArg_p->cellIndex);
#elif TDD_CONFIG 
    LTE_MAC_UT_LOG(LOG_DETAIL,TDD_MANAGER,
            "[%s] Entry. SubFrame No [%d]",__func__,execLegFuncArg_p->subFrame);

    txDLControlMsg( execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->advanceDLGlobalTick,
            execLegFuncArg_p->advanceDLSystemframe,
            execLegFuncArg_p->advanceDLSubframe,
            execLegFuncArg_p->cellIndex
            );
#endif            
    LTE_MAC_UT_LOG(LOG_DETAIL,EXEC_LEG_MANAGER,"[%s] Exit . SubFrame No [%d]",__func__,execLegFuncArg_p->subFrame);
}

#ifdef PERF_STATS
/*****************************************************************************
 * Function Name  : ElInvokeMacPerfStats
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function calls the processPERFCounter for MAC Perf Stats
 *                  Calculations.
 *****************************************************************************/
 void ElInvokeMacPerfStats(ExecutionLegFunctionArg * execLegFuncArg_p)
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
    /* + PERF_CA */
    macProcessPerfCounter();
    /* - PERF_CA */
}
#endif


#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : ElWaitForDLControMsgSentTrigger 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to wait for trigger that
 *                  control messages are already sent.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElWaitForDLControMsgSentTrigger(ExecutionLegFunctionArg * 
        /* SPR 5599 changes end (ZIP ID 129063) */
        execLegFuncArg_p)
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
    /*SPR_7217_changes_start*/
    //semWait(&dlControlMsgSentTriggerSem_g);
    /*SPR_7217_changes_end*/
}

/*****************************************************************************
 * Function Name  : ElRxTriggerToFormDLControMsg 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to wait for trigger to be sent from
 *                  PDSCH so that control messages can be formed.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElRxTriggerToFormDLControMsg(ExecutionLegFunctionArg * 
        /* SPR 5599 changes end (ZIP ID 129063) */
        execLegFuncArg_p)
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
    semWait(&dlControlMsgToFormTriggerSem_g);
}

/*****************************************************************************
 * Function Name  : ElSendTriggerToFormDLControlMsg 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function post the semaphore so that DL control message 
 *                  to be sent to PHY can be formed. 
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElSendTriggerToFormDLControlMsg(ExecutionLegFunctionArg * 
        /* SPR 5599 changes end (ZIP ID 129063) */
        execLegFuncArg_p)
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
    semPost(&dlControlMsgToFormTriggerSem_g);
}

/*****************************************************************************
 * Function Name  : ElTxDLControMsgAndSendTrigger
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function transmits DL control messages to PHY and 
 *                  semposts dlControlMsgSentTriggerSem_g , which acts as 
 *                  trigger to the which transmits DL msgs only after control
 *                  msgs are sent.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElTxDLControMsgAndSendTrigger(ExecutionLegFunctionArg * 
        /* SPR 5599 changes end (ZIP ID 129063) */
        execLegFuncArg_p)
{
    txDLControlMsg( execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->globalTick,
            execLegFuncArg_p->sysFrame,
            execLegFuncArg_p->subFrame,
            execLegFuncArg_p->cellIndex);
}


/*****************************************************************************
 * Function Name  : checkDLControlMsgTrigger
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Check if dlControlMsgSentTriggerSem_g is posted, this 
 *                  semaphore is posted only after sending control messages.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
/* + SPR 17439 */
UInt32 checkDLControlMsgTrigger(void)
    /* - SPR 17439 */    
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    SInt32 semVal = -1;
    /*SPR_7217_changes_start*/
    //semGetVal(&dlControlMsgSentTriggerSem_g,&semVal);
    /*SPR_7217_changes_end*/

    return semVal;
}

#endif

/*****************************************************************************
 * Function Name  : ElTxDLDataMsg 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function transmits DL data Msgs.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElTxDLDataMsg(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef FDD_CONFIG
    txDLDataMsgReq(
            execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->globalTick,
            execLegFuncArg_p->sysFrame,
            execLegFuncArg_p->subFrame,
            execLegFuncArg_p->cellIndex);
#elif TDD_CONFIG
    txDLDataMsgReq(
            execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->advanceDLGlobalTick,
            execLegFuncArg_p->advanceDLSystemframe,
            execLegFuncArg_p->advanceDLSubframe,
            execLegFuncArg_p->cellIndex
            );

#endif      

}

/*****************************************************************************
 * Function Name  : ElTxCommonChannelData 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg is called to transmit common channel data.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElTxCommonChannelData(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
}


/*****************************************************************************
 * Function Name  : ElProcessTxQueue 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes all the tranmission queues,queue
 *                  count number of times. 
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessTxQueue(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
}


/*****************************************************************************
 * Function Name  : ElProcessTxUntilLastNode 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes all the tranmission queues until
 *                  end of processing is indicated. ( By an invalid CCE Index).
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessTxUntilLastNode(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
}

/*****************************************************************************
 * Function Name  : triggerSubframeInd 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Upon reception of subframe indication from PHY post all 
 *                  the semaphores in ulRlcWaitForSubFrameIndSem_g , which acts
 *                  a trigger to all the execution flows waiting on their
 *                  semaphores for subframe indication.
 *****************************************************************************/
#ifdef FDD_CONFIG
/*Rel_523_Coverity_10047 Fix Start */
#ifndef ONE_FLOW_ONE_DELAY
/* SPR 5599 changes start (ZIP ID 129063) */
void triggerSubframeInd()
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    semPostDefault(&ulRlcWaitForSubFrameIndSem_g);
}
#endif
/*Rel_523_Coverity_10047 Fix End */
#endif

/*****************************************************************************
 * Function Name  : ElProcessPendingDeleteUE  
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg process pending delete UE list , and deletes
 *                  those UE's contexts which are pending to be deleted.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessPendingDeleteUE(ExecutionLegFunctionArg * execLegFuncArg_p) /*SPR 4016*/
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /*SPR 16855 Fix Start*/
    /*CA Changes start  */
    deleteUEContextDlHiPrioThd (execLegFuncArg_p->cellIndex);
    /*CA Changes end  */
    /*SPR 16855 Fix End*/
}
/* +- SPR 22494 */
/*****************************************************************************
 * Function Name  : ElProcessPendingResetUE 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This leg process pending reset UE list , and reset
 *                  those UE's contexts' which are pending to be reseted.
 ******************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessPendingResetUE(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    if(processUeResetFlag_g[execLegFuncArg_p->cellIndex])
#endif
    {
        /*CA Changes start  */
        processPendingResetUE(execLegFuncArg_p->globalTick, execLegFuncArg_p->cellIndex);
        /*CA Changes end  */
    }

}
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : ElwaitForSubframeInd 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : When this execution leg is called from an execution flow
 *                  it means that , the execution flow has a necessity to
 *                  wait for next tick before it starts its processing.The
 *                   ulRlcWaitForSubFrameIndSem_g is posted upon
 *                  reception of new tick or tick change indication from PHY.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElWaitForSubframeInd(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* +- SPR 17777*/
    (void)execLegFuncArg_p;
    /* +- SPR 17777*/
    semWaitDefault(&ulRlcWaitForSubFrameIndSem_g); 
}
#endif

/* For Semi static changes */
#ifdef TDD_CONFIG
#ifdef DL_UL_SPLIT_TDD
/*****************************************************************************
 * Function Name  : ElWaitForSubframeInd 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : When this execution leg is called from an execution flow
 *                  it means that , the execution flow has a necessity to
 *                  wait for next tick before it starts its processing.The
 *                  flow index of the execution flow is used as index into
 *                  waitForSubFrameIndSemArr_g , where each of the semaphore
 *                  in the array waitForSubFrameIndSemArr_g is posted upon
 *                  reception of new tick or tick change indication from PHY.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElWaitForSubframeInd(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    semWaitDefault(&waitForSubFrameIndSemArr_g); 
}
#endif

/*****************************************************************************
 * Function Name  : ElProcessUlConfig 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : When this execution leg is called from an execution flow.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessUlConfig(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* TDD Config 0 Changes Start */
    /* Check If the next 4th subframe is not DL subframe 
     * If so then no need to call prepareULConfigMsg */ 
    if ((MAX_SUB_FRAME != execLegFuncArg_p->execLegSpecificArg1) 
            && (DL != getSubframeType(execLegFuncArg_p->execLegSpecificArg1,execLegFuncArg_p->cellIndex))
       )
    {
        prepareULConfigMsg(execLegFuncArg_p->execLegSpecificArg1, //The U SF for which processing is to be done
                execLegFuncArg_p->advanceDLGlobalTick,
                execLegFuncArg_p->advanceDLSystemframe,
                execLegFuncArg_p->advanceDLSubframe,
                execLegFuncArg_p->cellIndex);
    }
    /* TDD Config 0 Changes End */
}

/*****************************************************************************
 * Function Name  : ElTransmitULConfig 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : When this execution leg is called from an execution flow.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElTransmitULConfig(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* CA Changes start */
    UInt8 subFrame = execLegFuncArg_p->subFrame + macParams_g.ulConfigAdvanceSubFrames;
    /* CA Changes end */
    /* SPR 11082 Fix Start */
    subFrame = subFrame % MAX_SUB_FRAME;
    /* SPR 11082 Fix Ends */
    if(DL != getSubframeType(subFrame, execLegFuncArg_p->cellIndex))
    {
        /* +- SPR 17777 */
        txULConfigMsg(
                /* +- SPR 17777 */
                execLegFuncArg_p->sysFrame,
                execLegFuncArg_p->subFrame,
                execLegFuncArg_p->cellIndex);
    }


#ifdef FLEXRAN
    /* Notify the PHY for DL Messages */
    flexran_ipc_notify_phy_ready(execLegFuncArg_p->cellIndex);
#endif

}
/* End Semi static changes */

/*Start SRS changes*/
/*****************************************************************************
 * Function Name  : ElProcessSrsForUplinkSubframe 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg processes the SRS transmissions for 
 *                  uplink subframes.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessSrsForUplinkSubframe(ExecutionLegFunctionArg * 
        /* SPR 5599 changes end (ZIP ID 129063) */
        execLegFuncArg_p)
{
    CHECK_QOS();
    /*SPR 1115 CHG*/
    /* TDD Config 0 changes Start */
    /* Check If the next 4th subframe is UL subframe 
     * If so then no need to call processSrsForUplinkSubframes 
     * As It will be processed from ElProcessSrsForSpecialSubframe */
    if ((MAX_SUB_FRAME != execLegFuncArg_p->execLegSpecificArg1) &&
            (UL == getSubframeType(execLegFuncArg_p->execLegSpecificArg1,execLegFuncArg_p->cellIndex)))
    {
        processSrsForUplinkSubframes(execLegFuncArg_p->execLegSpecificArg1,
                execLegFuncArg_p->advanceDLGlobalTick,
                execLegFuncArg_p->cellIndex);
    }
    /* TDD Config 0 changes End */
}

/*****************************************************************************
 * Function Name  : ElProcessSrsForSpecialSubframe 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg processes the SRS transmissions for 
 *                  special subframes.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessSrsForSpecialSubframe(ExecutionLegFunctionArg * 
        /* SPR 5599 changes end (ZIP ID 129063) */
        execLegFuncArg_p)
{
    CHECK_QOS();
    /*SPR 1115 CHG*/
    processSrsForSpecialSubframes(execLegFuncArg_p->execLegSpecificArg1,
            execLegFuncArg_p->advanceDLGlobalTick,
            execLegFuncArg_p->cellIndex);
}

/*****************************************************************************
 * Function Name  : ElProcessPrachForSpecialSubframe
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg processes the PRACH transmissions for
 *                  special subframes.
 *****************************************************************************/
void ElProcessPrachForSpecialSubframe( ExecutionLegFunctionArg *
        execLegFuncArg_p)
{
    processPrachReportForSpecialSubframes(
            execLegFuncArg_p->execLegSpecificArg1,
            execLegFuncArg_p->advanceDLGlobalTick,
            execLegFuncArg_p->cellIndex );
}

#elif FDD_CONFIG
/*****************************************************************************
 * Function Name  : ElProcessSrsForSubframe 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg processes the SRS transmissions for 
 *                  subframes.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessSrsForSubframe(ExecutionLegFunctionArg *execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    CHECK_QOS();
    /*SPR 1115 CHG*/
    processSrsReports(execLegFuncArg_p->subFrame,execLegFuncArg_p->delayToPhy,
            execLegFuncArg_p->globalTick,             
            /*CA Changes start  */
            execLegFuncArg_p->cellIndex);
    /*CA Changes end  */
}
#endif
/*FDD_SRS*/
/*End SRS changes*/

/* DRX_CHG */
/*****************************************************************************
 * Function Name  : ElProcessDRXForUEs 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg checks for the DRX time durations of the
 *                  UE. This will check for the expiry of all DRX timers and
 *                  check for DL and UL scheduler events.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessDRXForUEs(ExecutionLegFunctionArg *execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* SPR 15909 fix start */
    tickType_t   globalAdvanceTick = 0;
    /* SPR 15909 fix end */

#ifdef FDD_CONFIG
    /** DRX MEAS_GAP EL Changes Start **/
    globalAdvanceTick = execLegFuncArg_p->globalTick + 1 + execLegFuncArg_p->delayToPhy;
    /** DRX MEAS_GAP EL Changes End **/

#elif TDD_CONFIG
    /* SPR 11406 fix start */
    globalAdvanceTick = execLegFuncArg_p->advanceDLGlobalTick + DL_ADVANCE_TICK;
    /* SPR 11406 fix end */
#endif

    processDRXForUEs(globalAdvanceTick ,execLegFuncArg_p->cellIndex);
}
/*CA_CHG_START*/
/* DRX_CHG */
/*****************************************************************************
 *  * Function Name  : ElProcessDeleteScheduleLcList
 *  * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *  *                                     delay to phy , sub frame number etc and
 *  *                                     also few arguments specifically
 *  *                                     necessary for certain execution legs.
 *  * Outputs        : None
 *  * Returns        : None
 *  * Description    : This execution leg delete the list of LCs scheduled 
 *  *                  scheduled in current tick and also reset schdeuledUeCount
 *  *                  in dlStrategyScheduleInfo_g 
 *  *****************************************************************************/
void ElProcessDeleteScheduleLcList(ExecutionLegFunctionArg *execLegFuncArg_p)
{
    SInt8  countDLUENewTx = 0;
    DLUEContext   *ueDLContext_p = PNULL;
    DLUeScheduleInfo *dlUeScheduleInfo_p = PNULL;
    UInt8  internalCellIndex = execLegFuncArg_p->cellIndex;

    UInt8 strategyContainerIndex = (execLegFuncArg_p->globalTick % STRATEGY_MAX_CONTAINER_INDEX); 
    /* + Critical section changes */
    /*6.0 DL_UL_SPLIT Fix, adding condition*/
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
    UInt8 isOtherCellStateRunning = 0;
    UInt8 isLowestCellIndex = 0;
    if (IS_CA_ENABLED())
    {
        semWait(&cellSyncSem_g);
        isOtherCellStateRunning = CHECK_CELL_STATE(!macParams_g.lowestRunningCellIndex);
        isLowestCellIndex = (macParams_g.lowestRunningCellIndex == execLegFuncArg_p->cellIndex);
        semPost(&cellSyncSem_g);
    }
#endif 
    /* - Critical section changes */
    /* + SPR 11470 Changes */
    /* + SPR_11598 */
    /*6.0 DL_UL_SPLIT Fix, adding condition*/
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
    if((cellSpecificParams_g.numOfCells == 1) || !IS_CA_ENABLED()|| (!isOtherCellStateRunning))
#else
        if((cellSpecificParams_g.numOfCells == 1) || !IS_CA_ENABLED()|| (macParams_g.lowestRunningCellIndex == execLegFuncArg_p->cellIndex))
#endif 
            /* - Critical section changes */
            /* - SPR_11598 */
        {
            while( countDLUENewTx < dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount )
            {
        /*fprintf(stderr,"Info at %s %d %llu: strategyContainerIndex %d servingCellIndex %d .schdeuledUeCount %d \n",__func__,__LINE__,getCurrentTick(),strategyContainerIndex,internalCellIndex,dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount);*/
                dlUeScheduleInfo_p = &(dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].dlUeScheduleInfo
                        [countDLUENewTx]);
                ueDLContext_p = dlUeScheduleInfo_p->ueDLContext_p;
                if(PNULL != ueDLContext_p )
                {
                    deleteScheduledLCList(ueDLContext_p, internalCellIndex);
                }
                countDLUENewTx++;
            }
            dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount = 0; 
        }
    /*6.0 DL_UL_SPLIT Fix, adding condition*/
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
    /* + Critical section changes */
        if(!isLowestCellIndex) 
            /* - Critical section changes */
        {
            while( countDLUENewTx < dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount )
            {
        /*fprintf(stderr,"Info at %s %d %llu : strategyContainerIndex %d servingCellIndex %d .schdeuledUeCount %d \n",__func__,__LINE__,getCurrentTick(),strategyContainerIndex,internalCellIndex,dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount);*/
                dlUeScheduleInfo_p = &(dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].dlUeScheduleInfo
                        [countDLUENewTx]);
                ueDLContext_p = dlUeScheduleInfo_p->ueDLContext_p;
                if(PNULL != ueDLContext_p )
                {
                    deleteScheduledLCList(ueDLContext_p, internalCellIndex);
                }
                countDLUENewTx++;
            }
            dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount = 0; 

            countDLUENewTx = 0;
            while( countDLUENewTx < dlStrategyScheduleInfo_g[strategyContainerIndex][!(internalCellIndex)].schdeuledUeCount )
            {
        /*fprintf(stderr,"Info at %s %d %llu : strategyContainerIndex %d servingCellIndex %d .schdeuledUeCount %d \n",__func__,__LINE__,getCurrentTick(),strategyContainerIndex,!internalCellIndex,dlStrategyScheduleInfo_g[strategyContainerIndex][!internalCellIndex].schdeuledUeCount);*/

                dlUeScheduleInfo_p = &(dlStrategyScheduleInfo_g[strategyContainerIndex][!(internalCellIndex)].dlUeScheduleInfo
                        [countDLUENewTx]);
                ueDLContext_p = dlUeScheduleInfo_p->ueDLContext_p;
                if(PNULL != ueDLContext_p )
                {
                    deleteScheduledLCList(ueDLContext_p, !(internalCellIndex));
                }
                countDLUENewTx++;
            }
            dlStrategyScheduleInfo_g[strategyContainerIndex][!(internalCellIndex)].schdeuledUeCount = 0; 
        }
#endif
    /* - SPR 11470 Changes */
}

/*CA_CHG_END*/
/*****************************************************************************
 * Function Name  : ElProcessMeasGapForUEs 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg shall check the meas gap offset map 
 *                  and send triggers of meas period ON/ OFF to DL and UL
 *                  scheduler strategy.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElProcessMeasGapForUEs(ExecutionLegFunctionArg *execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* SPR 15909 fix start */
    tickType_t  globalAdvanceTick = 0;
    /* SPR 15909 fix end */

#ifdef FDD_CONFIG
    /** DRX MEAS_GAP EL Changes Start **/
    globalAdvanceTick = execLegFuncArg_p->globalTick + 1 + execLegFuncArg_p->delayToPhy; 
    /** DRX MEAS_GAP EL Changes End **/

#elif TDD_CONFIG
    /* SPR 11406 fix start */
    globalAdvanceTick = execLegFuncArg_p->advanceDLGlobalTick + DL_ADVANCE_TICK;
    /* SPR 11406 fix end */
#endif

    /*CA Changes start  */
    processAllMeasGapOffsetMaps(globalAdvanceTick, execLegFuncArg_p->cellIndex);
    /*CA Changes end  */
}
/* MEAS_GAP_CHG */

/* Framework Integration Changes Start */
#ifdef TDD_CONFIG
/* TDD Optimization changes */
/* SPR 5599 changes start (ZIP ID 129063) */
extern void freePendingMemPools(UInt32 ratioOfMemFree);
/* TDD Optimization changes */

/*****************************************************************************
 * Function Name  : ElProcessMemCleanup 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes memory clean up.
 *****************************************************************************/
void ElProcessMemCleanup(ExecutionLegFunctionArg * execLegFuncArg_p)
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
        /* +- SPR 17777 */
}
#endif //TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129063) */

/*****************************************************************************
 * Function Name  : ElRlcHandleTxOpportunity 
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function .
 *****************************************************************************/
void ElRlcHandleTxOpportunity(ExecutionLegFunctionArg * execLegFuncArg_p) // RLC_HANDLE_TX_OPPORTUNITY  /*SPR 4016*/
    /* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef DL_UL_SPLIT
#ifndef DL_DATA_SEPARATION
    InternalCellIndex internalCellIndex = execLegFuncArg_p->cellIndex;
    /* CA_HSM_CHG */
    l2ThreadHealthStoreTick_g.rlcMacThread[internalCellIndex].rlcThread[RLC_DL_THREAD] = globalTTITickCount_g;

    /* HSM_CHG_END */
    /* Handle Opportunity Ind from MAC */
    if(RLC_FAILURE != rlcLayerInitialized_g 
            /*SPR 16236 TDD Merge start*/
#if !defined TDD_CONFIG
            && pdcpContextIsInitialized()
#endif
            /*SPR 16236 end*/

      )
    {
        /* CA Changes Start */
        rlcHandleTxOppurtunity(execLegFuncArg_p->cellIndex);
        /* CA Changes End */
    }
#endif
    /** CA 2 Core Changes Start **/
#else
    if(RLC_FAILURE != rlcLayerInitialized_g
            /*SPR 16236 TDD Merge start*/
#if !defined TDD_CONFIG
            && pdcpContextIsInitialized()
#endif
            /*SPR 16236 end*/
      )
        {
             UInt32 sCount = 0;
#ifdef LTE_EMBMS_SUPPORTED
       if(isEmbmsValidSf_g[execLegFuncArg_p->cellIndex])
       {
           sCount = circQueueCount(macEmbmsOpporIndCircQ_gp[execLegFuncArg_p->cellIndex]);
           while( sCount-- )
           {
               rlcHandleTxEmbmsOppurtunity(execLegFuncArg_p->cellIndex);
           }
       }
       else
#endif
       {
            sCount = COUNT_MAC_UE_OPPOR_IND_CIRC_Q(execLegFuncArg_p->cellIndex);
            RDTSC_AND_CLOCK_START_FDD(RLC_HANDLE_TRX_OPPORTUNITY);
            while(sCount--)
            {
#ifndef DL_DATA_SEPARATION
                rlcHandleTxOppurtunity(execLegFuncArg_p->cellIndex);
#else
                /*Coverity 114738 Start*/
                (void)rlcHandleTxOppurtunity(execLegFuncArg_p->globalTick,execLegFuncArg_p->cellIndex);
               /*Coverity 114738 End*/
#endif
            }

            RDTSC_AND_CLOCK_END_RLC(RLC_HANDLE_TRX_OPPORTUNITY, execLegFuncArg_p->cellIndex);
        }
        }

    /** CA 2 Core Changes End **/


#endif
}

#if defined(TDD_CONFIG) || defined(DL_UL_SPLIT)
/*coverity-530-32868 */
#ifdef OBSELETE_REL_8
/*coverity-530-32868 */
/*****************************************************************************
 * Function Name  : ElRlcHandleHarqFailInd  
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the RLC Harq failure indication message process the RLC Harq failure indication messagess 
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void ElRlcHandleHarqFailInd(ExecutionLegFunctionArg * execLegFuncArg_p)  //RLC_HANDLE_HARQ_FAIL_IND
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    UInt32 count;

    /* waiting for PROCESS_DL_OAM_AND_RRC_MSG leg to finish */
    /*semWaitDefault(&waitForDLOamAtHarqFailInd_g);*/
    /* waiting for PROCESS_DL_OAM_AND_RRC_MSG leg to finish */

    /*Execution Leg to handle harq failure indication from MAC Layer*/
    count = sQueueCount(&macUeHarqFailureIndQ_g);
    if(count)
    {
        rlcHandleHarqFailureInd(count);
    }
}
#endif
#endif

#ifdef TDD_CONFIG

/* SPR 5599 changes start (ZIP ID 129063) */
/* + coverity 32562 */
/*****************************************************************************
 * Function Name  : ElPdcpTxMessage
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the PDCP TX messages 
 *****************************************************************************/
void ElPdcpTxMessage(ExecutionLegFunctionArg * execLegFuncArg_p) //PDCP_TX_PROCESS_MESSAGE
    /* - coverity 32562 */
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
    /* +- SPR 17777 */
}

/* SPR 5599 changes start (ZIP ID 129063) */
/* + coverity 32563 */

/*****************************************************************************
 * Function Name  : ElRlcHandleControlPdu
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function Handles RLC Control PDU.
 *****************************************************************************/
void ElRlcHandleControlPdu(ExecutionLegFunctionArg * execLegFuncArg_p)  //RLC_HANDLE_CTRL_PDU_RLC_AM
    /* - coverity 32563 */
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
        /* +- SPR 17777 */
}

/* SPR 5599 changes start (ZIP ID 129063) */

/*****************************************************************************
 * Function Name  : ElRlcPdcpHandleData
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles RLC PDCP Data.
 *****************************************************************************/
void ElRlcPdcpHandleData(ExecutionLegFunctionArg * execLegFuncArg_p) //RLC_PDCP_HANDLE_DATA_FRM_MAC  /*SPR 4016*/
    /* SPR 5599 changes end (ZIP ID 129063) */
{   
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
        /* +- SPR 17777 */
        /** CA 2 Core Changes Start **/ 
        UInt32 count;
    InternalCellIndex index = 0;

    /* SPR 609 changes start */  
    for(;index < MAX_NUM_CELL;index++)
    {
        count = COUNT_MAC_UE_DATA_IND_Q(index);
        if( count )
            /* SPR 609 changes end */  
        {
            rlcHandleDataFromMac(count,index);
        }
    }
    /** CA 2 Core Changes End **/ 
}

/* RLC TIMER SEG FAULT CHG Start */
//MAC Thread
/* SPR 5599 changes start (ZIP ID 129063) */

/*****************************************************************************
 * Function Name  : ElRlcTimerUpdateTickChkForTx
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the TX on RLC timer expiry.
 *****************************************************************************/
void ElRlcTimerUpdateTickChkForTx(ExecutionLegFunctionArg * execLegFuncArg_p) //RLC_TIMER_CHK_FOR_TX_EXPIRY  /*SPR 4016*/
    /* SPR 5599 changes end (ZIP ID 129063) */
{  
    /* TDD UL Stall Fix Starts*/
    /* SPR 11528 Start */
    rlcCheckTimerExpiry(execLegFuncArg_p->cellIndex);  
    /* SPR 11528 End */
    /* TDD UL Stall Fix Ends*/
}

/* SPR 5599 changes start (ZIP ID 129063) */

/*****************************************************************************
 * Function Name  : ElRlcTimerUpdateTickChk
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks the RLC Timer Update Tick.
 *****************************************************************************/
void ElRlcTimerUpdateTickChk(ExecutionLegFunctionArg * execLegFuncArg_p) //RLC_TIMER_CHK_FOR_EXPIRY  /*SPR 4016*/
    /* SPR 5599 changes end (ZIP ID 129063) */
{   
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
        /* +- SPR 17777 */
        CHECK_QOS();
}
/* RLC TIMER SEG FAULT CHG End */

/* SPR 5599 changes start (ZIP ID 129063) */

/*****************************************************************************
 * Function Name  : ElPdcpTxProcessDeliveryInd
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function PDCP PDU messages
 *****************************************************************************/
void ElPdcpTxProcessDeliveryInd(ExecutionLegFunctionArg * execLegFuncArg_p) //PDCP_TX_PROCESS_DELIVERY_IN  /*SPR 4016*/
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
    /* +- SPR 17777 */
}

/* SPR 5599 changes start (ZIP ID 129063) */
/*****************************************************************************
 * Function Name  : ElRlcHandleCommonChnlAndPendingQueue
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles RLC Common Channel and Pending Queues.
 *****************************************************************************/
void ElRlcHandleCommonChnlAndPendingQueue(ExecutionLegFunctionArg * execLegFuncArg_p) //RLC_HANDLE_COMMON_CHANNEL_AND_PENDING_QUEUE  /*SPR 4016*/   
    /* SPR 5599 changes end (ZIP ID 129063) */
{   
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
        /* +- SPR 17777 */
}

/* SPR 5599 changes start (ZIP ID 129063) */
/*****************************************************************************
 * Function Name  : ElProcessRLCAndPDCPMsgs
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes RLC and PDCP messages.
 *****************************************************************************/
void ElProcessRLCAndPDCPMsgs(ExecutionLegFunctionArg * execLegFuncArg_p)  /*SPR 4016*/
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    if (execLegFuncArg_p->execLegSpecificArg6)
    {
        if(0x2FFF == ElRlcProcessOAMAndRRCMsgFSM(execLegFuncArg_p->execLegSpecificArg6,execLegFuncArg_p->execLegSpecificArg7))
        {
            /*SPR 6931 Fix start*/
            //freeMemPool(execLegFuncArg_p->execLegSpecificArg6);
            /*SPR 6931 Fix end*/
            execLegFuncArg_p->execLegSpecificArg6 = PNULL;
            cleanupLayer2();
            return;
        }
        /*SPR 6931 Fix start*/
        //freeMemPool(execLegFuncArg_p->execLegSpecificArg6);
        /*SPR 6931 Fix end*/

        execLegFuncArg_p->execLegSpecificArg6 = PNULL;
        execLegFuncArg_p->execLegSpecificArg7 = 0;

    }
}

/* SPR 13373 fix start */
/*****************************************************************************
 * Function Name  : ElRlcHandlePendingDeleteUe
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes Pending delete queue for RX and TX .
 *****************************************************************************/

void ElRlcHandlePendingDeleteUe(ExecutionLegFunctionArg * execLegFuncArg_p) 
{

    /* +- SPR 17777 */
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
        /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        UInt8 cellCount = 0;
    for(cellCount = 0; cellCount < cellSpecificParams_g.numOfCells; cellCount++)
    {
        if(circQueueCount(&rlcTxPendingDelQ_g[cellCount]))
        {
            rlcProcessDelMsg(&rlcTxPendingDelQ_g[cellCount]);
        }
    }
    if(circQueueCount(&rlcRxPendingDelQ_g))
    {
        rlcProcessDelMsg(&rlcRxPendingDelQ_g);
    }
/* SPR 18691 Fix Start*/
#ifdef LTE_EMBMS_SUPPORTED
    if(circQueueCount(&rlcEmbmsLpToHpQ_g))
    {
        rlcProcessEmbmsHpHandler(&rlcEmbmsLpToHpQ_g);
    }
#endif
/* SPR 18691 Fix End*/
#endif
}
/* SPR 13373 fix end */
#endif

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : initExecutionLegFunctionArr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function initializes executionFunctionArr_g.
 *                  Each element of this array is a function pointer to
 *                  an execution leg.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
/* + SPR 17439 */
void initExecutionLegFunctionArr(void)
    /* - SPR 17439 */
    /* SPR 5599 changes end (ZIP ID 129063) */
{
#ifdef DL_UL_SPLIT
    executionFunctionArr_g[WAIT_FOR_SUBFRAME_IND] = ElWaitForSubframeInd;
    executionFunctionArr_g[PROCESS_DL_HARQ_ACK_NACK_QUEUE] = 
        ElProcessDLHarqAckNackQueue;     
    executionFunctionArr_g[PROCESS_DL_HARQ_TIMER_EXPIRY] = 
        ElProcessDLHarqTimerExpiry;
    /*ca chngs phase 3 start*/ 
    executionFunctionArr_g[PROCESS_DL_CA_TIMER_EXPIRY] = 
        ElProcessDLCATimerExpiry;
    /*ca chngs phase 3 end*/ 
    executionFunctionArr_g[PROCESS_DL_SCH_STRATEGY] = 
        ElInvokeDLSCHStrategy;     
    executionFunctionArr_g[PROCESS_DL_SPS_STRATEGY] = 
        ElInvokeDLSpsSchStrategy; 
    executionFunctionArr_g[PROCESS_PRS_TRANSMISSION] = ElProcessPrsTransmission; /* PRS_CHANGES */
    executionFunctionArr_g[PROCESS_PDCCH_PDSCH_WITH_MUX] = 
        ElProcessPdcchAndPdschWithMux;      
    /* PDCCH_PDSCH_WITHOUT_MUX Start */
    executionFunctionArr_g[PROCESS_MUX_QUEUE] = ElProcessMuxQueue;
    /* PDCCH_PDSCH_WITHOUT_MUX End */
    executionFunctionArr_g[PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA] = 
        ElProcessEncoderQueueWithoutTxUntilLastData;      
    executionFunctionArr_g[TX_DL_CONTROL_MSG] = 
        ElTransmitDLControMsg;      
    executionFunctionArr_g[TX_DL_DATA_MSG] = 
        ElTxDLDataMsg;      
    executionFunctionArr_g[PROCESS_DRX_FOR_ALL_UE] = 
        ElProcessDRXForUEs;      
    executionFunctionArr_g[PROCESS_MEAS_GAP_FOR_ALL_UE] = 
        ElProcessMeasGapForUEs;      
    executionFunctionArr_g[PROCESS_DL_OAM_AND_RRC_MSG] = 
        ElProcessDLOamAndRRcMsg;                                               
    executionFunctionArr_g[PROCESS_PENDING_DELETE_UE] = 
        ElProcessPendingDeleteUE;      
    executionFunctionArr_g[PROCESS_PENDING_RESET_UE] = 
        ElProcessPendingResetUE;      
    executionFunctionArr_g[PROCESS_RLC_HANDLE_DATA_FROM_MAC] = 
        ElRlcHandleDataFromMac;      
    /* SPR 9911 fix start */
    executionFunctionArr_g[PROCESS_RLC_HANDLE_STATUS_REPORT] = 
        ElRlcHandleStatusReport;      
    /* SPR 9911 fix end */
    executionFunctionArr_g[PROCESS_RLC_DL_CHECK_TIMER_EXPIRY] = 
        ElRlcDLCheckTimerExpiry;      
    executionFunctionArr_g[PROCESS_RLC_DL_HDLR_REESTABLISH_Q] = 
        ElRlcDLHdlrReestablishQ;      
    executionFunctionArr_g[PROCESS_RLC_DL_HDLR_PEN_DEL_Q] = 
        ElRlcDLHdlrPenDelQ;      
    executionFunctionArr_g[PROCESS_RX_UNTIL_SUBFRAME_IND] = 
        ElProcessRxUntilSubFrameInd;      
    executionFunctionArr_g[PROCESS_UL_HARQ_TIMER_EXPIRY] =
        ElProcessULHarqTimerExpiry;
    executionFunctionArr_g[PROCESS_UL_SCH_STRATEGY] = 
        ElInvokeULSCHStrategy;     
    executionFunctionArr_g[PROCESS_UL_SPS_STRATEGY] = ElProcessUlSpsStrategy; 
    executionFunctionArr_g[PROCESS_PUSCH_FOR_DELAY_ONE] = 
        ElProcessPUSCHForDelayOne;      
#ifndef DL_DATA_SEPARATION
    executionFunctionArr_g[PROCESS_RLC_HANDLE_TX_OPPORTUNITY] = 
        ElRlcHandleTxOpportunity;      
#endif
    executionFunctionArr_g[PROCESS_DEMUX_QUEUE] = 
        ElProcessDemuxQueue;      
    executionFunctionArr_g[PROCESS_SRS_REPORTS] = 
        ElProcessSrsForSubframe;      
    executionFunctionArr_g[PROCESS_UL_CSI_SINR_REPORTS] = 
        ElProcessSINRReports;      
    executionFunctionArr_g[PROCESS_CQI_REPORTS] = 
        ElProcessDLCQIReports;     
    executionFunctionArr_g[PROCESS_SPS_PDSCH]   = ElProcessSpsPdsch;
    executionFunctionArr_g[PROCESS_SPS_PUSCH]   = ElProcessSpsPusch; 
    executionFunctionArr_g[PROCESS_E_CID_REPORTS] = ElProcessEcidReports;
    executionFunctionArr_g[PROCESS_UL_RA] = 
        ElProcessUlRandomAccess;      
    executionFunctionArr_g[PROCESS_UL_OAM_AND_RRC_MSG] = 
        ElProcessULOamAndRRcMsg;      
    /*coverity-530-32868 */
#ifdef OBSELETE_REL_8
    /*coverity-530-32868 */
    executionFunctionArr_g[PROCESS_RLC_HANDLE_HARQ_FAILURE_IND] = 
        ElRlcHandleHarqFailInd;     
#endif
    executionFunctionArr_g[PROCESS_RLC_UL_CHECK_TIMER_EXPIRY] = 
        ElRlcULCheckTimerExpiry;      
    executionFunctionArr_g[PROCESS_RLC_UL_HDLR_REESTABLISH_Q] = 
        ElRlcULHdlrReestablishQ;      
    executionFunctionArr_g[PROCESS_RLC_UL_HDLR_PEN_DEL_Q] = 
        ElRlcULHdlrPenDelQ;      
    executionFunctionArr_g[PROCESS_UL_PENDING_DELETE_UE] = 
        ElProcessULPendingDeleteUE;      
    executionFunctionArr_g[PROCESS_UL_PENDING_RESET_UE] = 
        ElProcessULPendingResetUE;      
    /* +- SPR 22494 */
    executionFunctionArr_g[PROCESS_DL_PERFORMANCE_STATS_UPDATE] = 
        ElProcessPerformanceStatsUpdate;
    executionFunctionArr_g[PROCESS_UL_PERFORMANCE_STATS_UPDATE] = 
        ElProcessPerformanceStatsUpdate;
    executionFunctionArr_g[PROCESS_MISC_UPDATES] = ElProcessMiscUpdates;
    executionFunctionArr_g[PROCESS_DELETE_SCHEDULE_LC_LIST] = ElProcessDeleteScheduleLcList; /*CA_CHG_START*/
    /*ca chngs phase 3 start*/
    executionFunctionArr_g[UPDATE_DL_TOP_SCH_LIST_SCELL_STATE] = ElupdateDlTopSchListScellState;
    /*ca chngs phase 3 end*/

#else

    /* Initialize execution leg function array */
    executionFunctionArr_g[PROCESS_HARQ_TIMER_EXPIRY_IF_TIME_LEFT] = 
        ElProcessDLHarqTimerExpiryIfTimeLeft;
    executionFunctionArr_g[PROCESS_CQI_REPORTS_IF_TIME_LEFT] =
        ElProcessDLCQIReportsIfTimeLeft;
    executionFunctionArr_g[PROCESS_DEMUX_QUEUE_IF_TIME_LEFT] =
        ElProcessDemuxMsgsIfTimeLeft;
    executionFunctionArr_g[PROCESS_N_RRC_OAM_MSG_IF_TIME_LEFT] = 
        ElProcessNumRRCAndOAMMsgsIfTimeLeft;
    executionFunctionArr_g[WAIT_FOR_PDCCH_READY_TRIGGER] = ElWaitForPDCCHReadyTrigger;
    executionFunctionArr_g[WAIT_FOR_SUBFRAME_IND] = ElWaitForSubframeInd;
    executionFunctionArr_g[PROCESS_PRS_TRANSMISSION] = ElProcessPrsTransmission; /* PRS_CHANGES */
    executionFunctionArr_g[PROCESS_PDCCH_PDSCH_WITH_MUX] = 
        ElProcessPdcchAndPdschWithMux;
    executionFunctionArr_g[PROCESS_PDCCH_PDSCH_WITHOUT_MUX] = 
        ElProcessPdcchAndPdschWithoutMux;
    executionFunctionArr_g[PROCESS_PDCCH_PDSCH_WITH_MUX_AND_TRIGGER_PUSCH] = 
        ElProcessPdcchAndPdschWithMuxAndTrigger;
    executionFunctionArr_g[PROCESS_PDCCH_PDSCH_WITHOUT_MUX_AND_TRIGGER_PUSCH] = 
        ElProcessPdcchAndPdschWithoutMuxAndTrigger;
    executionFunctionArr_g[PROCESS_MUX_QUEUE] = ElProcessMuxQueue;
    executionFunctionArr_g[PROCESS_MUX_QUEUE_UNTIL_LAST_NODE] = 
        ElProcessMuxQueueUntilLastNode;
    executionFunctionArr_g[PROCESS_ENCODER_QUEUE_WITH_TX_UNTIL_LAST_DATA] =
        ElProcessEncoderQueueWithTxUntilLastData;
    executionFunctionArr_g[PROCESS_ENCODER_QUEUE_WITH_TX] = 
        ElProcessEncoderQueueWithTx;
    executionFunctionArr_g[PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA] = 
        ElProcessEncoderQueueWithoutTxUntilLastData;
    executionFunctionArr_g[PROCESS_ENCODER_QUEUE_WITHOUT_TX] = 
        ElProcessEncoderQueueWithoutTx;
    executionFunctionArr_g[PROCESS_DL_HARQ_ACK_NACK_QUEUE] = 
        ElProcessDLHarqAckNackQueue;
    executionFunctionArr_g[PROCESS_CQI_REPORTS] = ElProcessDLCQIReports;
    executionFunctionArr_g[PROCESS_SPS_PDSCH]   = ElProcessSpsPdsch;
    executionFunctionArr_g[PROCESS_SPS_PUSCH]   = ElProcessSpsPusch;
    executionFunctionArr_g[PROCESS_E_CID_REPORTS] = ElProcessEcidReports;
    executionFunctionArr_g[PROCESS_N_CQI_REPORTS] = ElProcessNDLCQIReports;
    executionFunctionArr_g[PROCESS_CQI_REPORTS_IF_TIME_LEFT] = ElProcessDLCQIReportsIfTimeLeft;
    executionFunctionArr_g[PROCESS_UL_CSI_SINR_REPORTS] = ElProcessSINRReports; /* ULA_CHG */
    /** CA 2 Core Changes Start **/
    executionFunctionArr_g[PROCESS_DL_HARQ_TIMER_EXPIRY] = 
        ElProcessDLHarqTimerExpiry;
    executionFunctionArr_g[PROCESS_UL_HARQ_TIMER_EXPIRY] =
        ElProcessULHarqTimerExpiry;
#ifndef DL_DATA_SEPARATION
    executionFunctionArr_g[RLC_HANDLE_TX_OPPORTUNITY] = ElRlcHandleTxOpportunity;
#endif
    /** CA 2 Core Changes Start **/
    executionFunctionArr_g[PROCESS_UL_RA] = ElProcessUlRandomAccess;
    executionFunctionArr_g[PROCESS_PUSCH_FOR_DELAY_ONE] = 
        ElProcessPUSCHForDelayOne;
    executionFunctionArr_g[PROCESS_PUSCH_FOR_DELAY_TWO] = 
        ElProcessPUSCHForDelayTwo;
    executionFunctionArr_g[PROCESS_DL_SCH_STRATEGY] = ElInvokeDLSCHStrategy;
    executionFunctionArr_g[PROCESS_DL_SPS_STRATEGY] = ElInvokeDLSpsSchStrategy;
    executionFunctionArr_g[PROCESS_UL_SCH_STRATEGY_IF_NOT_ALREADY_DONE] =
        ElProcessUlSchStrategyIfNotAlreadyDone;
    executionFunctionArr_g[PROCESS_UL_SCH_STRATEGY] = ElInvokeULSCHStrategy;
    executionFunctionArr_g[PROCESS_RX_CONTROL_DATA_WITH_DEMUX] =
        ElProcessRxControlAndNumDataMsgsWithDemux;
    executionFunctionArr_g[PROCESS_RX_UNTIL_SUBFRAME_IND] =
        ElProcessRxUntilSubFrameInd;
    executionFunctionArr_g[PROCESS_RX_CONTROL_DATA_WITHOUT_DEMUX] = 
        ElProcessRxControlDataWithoutDemux;
    executionFunctionArr_g[PROCESS_N_DEMUX_QUEUE] = ElProcessNumDemuxMsgs;
    executionFunctionArr_g[PROCESS_DEMUX_QUEUE] = ElProcessDemuxQueue;
    executionFunctionArr_g[PROCESS_UL_SPS_STRATEGY] = ElProcessUlSpsStrategy;
    executionFunctionArr_g[PROCESS_N_RRC_OAM_MSG] =  
        ElProcessNumRRCAndOAMMsgs;
    executionFunctionArr_g[TX_DL_CONTROL_MSG] = 
        ElTransmitDLControMsg;
    executionFunctionArr_g[TX_DL_CONTROL_MSG_AND_TRIGGER] = 
        ElTxDLControMsgAndSendTrigger;
#ifdef UTFWK_SIMULATION
    executionFunctionArr_g[UPDATE_TICK_RX_MSQ] = 
        ElupdateTickAndSendAckNack;
    executionFunctionArr_g[RLC_UPDATE_QLOAD] = 
        ElrlcUpdateQload;
    executionFunctionArr_g[RLC_HANDLE_TX_OPPORTUNITY_QOS] = ElRlcHandleTxOpportunityQOS;
#endif

    /*FDD_SRS*/
    executionFunctionArr_g[PROCESS_SRS_REPORTS] = ElProcessSrsForSubframe;
    /*FDD_SRS*/
    /* DRX_CHG */
    executionFunctionArr_g[PROCESS_DRX_FOR_ALL_UE] = ElProcessDRXForUEs;
    executionFunctionArr_g[PROCESS_MEAS_GAP_FOR_ALL_UE] = ElProcessMeasGapForUEs;
    /* DRX_CHG */
    executionFunctionArr_g[PROCESS_DELETE_SCHEDULE_LC_LIST] = ElProcessDeleteScheduleLcList; /*CA_CHG_START*/
    executionFunctionArr_g[TX_DL_DATA_MSG] =  ElTxDLDataMsg;
    executionFunctionArr_g[PROCESS_TX_QUEUE_UNTIL_LAST_NODE] = ElProcessTxUntilLastNode;
    executionFunctionArr_g[PROCESS_TX_QUEUE] = ElProcessTxQueue;
    executionFunctionArr_g[TX_COMMON_CHANNEL_DATA] = ElTxCommonChannelData;
    executionFunctionArr_g[WAIT_FOR_CONTROL_MSG_SENT_TRIGGER] = 
        ElWaitForDLControMsgSentTrigger;
    executionFunctionArr_g[PROCESS_PENDING_DELETE_UE] = 
        ElProcessPendingDeleteUE;
    executionFunctionArr_g[PROCESS_PENDING_RESET_UE] = 
        ElProcessPendingResetUE;
    /* +- SPR 22494 */
    executionFunctionArr_g[RX_CONTROL_TRIGGER] = ElRxTriggerToFormDLControMsg;
    executionFunctionArr_g[SEND_CONTROL_TRIGGER] = ElSendTriggerToFormDLControlMsg;
    executionFunctionArr_g[PROCESS_MISC_UPDATES] = ElProcessMiscUpdates;
    /*SPR 16855 +-*/
    /*rel4-drop2 changes*/
    executionFunctionArr_g[PROCESS_DL_OAM_AND_RRC_MSG] = 
        ElProcessDLOamAndRRcMsg;                                              

    executionFunctionArr_g[PROCESS_UL_OAM_AND_RRC_MSG] = 
        ElProcessULOamAndRRcMsg;   
    executionFunctionArr_g[PROCESS_UL_PENDING_DELETE_UE] =
        ElProcessULPendingDeleteUE; 
    /*SPR 16855 +-*/
    /*rel4-drop2 changes*/

    /* Initialize execution leg function array */
    /*ca chngs start*/
    executionFunctionArr_g[UPDATE_DL_TOP_SCH_LIST_SCELL_STATE] = ElupdateDlTopSchListScellState;
    /*ca chngs end*/ 
    /* CA-Ph2 Integration fix start */
    executionFunctionArr_g[PROCESS_DL_CA_TIMER_EXPIRY] = 
        ElProcessDLCATimerExpiry;
    /* CA-Ph2 Integration fix end */
/* SPR 21660 changes start_vishal */
/* SPR 21660 changes end_vishal */
#endif

#ifdef DL_DATA_SEPARATION
    executionFunctionArr_g[PROCESS_RLC_HANDLE_TX_OPPORTUNITY_WITH_ENCODER_PER_UE] = ElRlcHandleTxOpportunityWithEncoder;
    executionFunctionArr_g[RLC_UPDATE_BUFF_OCC_TO_MAC] = ElRlcUpdateBuffOccToMac; 
    executionFunctionArr_g[PROCESS_DL_SYNC_TX_DATA] = ElDLSyncEncoderThread; 
#endif
    /**EL Sync Changes Start **/ 
#if (!defined(DL_UL_SPLIT))
    executionFunctionArr_g[PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS] = 
        ElProcessEncoderQueueOfOtherCells;      
    executionFunctionArr_g[PROCESS_SET_STRATEGY_FD] = ElSetStrategyFd;
    executionFunctionArr_g[PROCESS_CHECK_STRATEGY_FD] = ElCheckStrategyFd; 
    executionFunctionArr_g[PROCESS_SET_PDCCH_PDSCH_FD] = ElSetPdcchPdschFd; 
    executionFunctionArr_g[PROCESS_CHECK_PDCCH_PDSCH_FD] = ElCheckPdcchPdschFd;
    executionFunctionArr_g[PROCESS_SET_MUX_FD] = ElSetMuxFd; 
    executionFunctionArr_g[PROCESS_CHECK_MUX_FD] = ElCheckMuxFd; 
    executionFunctionArr_g[PROCESS_SET_ENCODER_FD] = ElSetEncoderFd; 
    executionFunctionArr_g[PROCESS_CHECK_ENCODER_FD] = ElCheckEncoderFd;
    executionFunctionArr_g[PROCESS_PENDING_EVENT_FD] = ElProcessPendingEventFds;
#endif
    /**EL Sync Changes End **/ 
#ifdef LTE_EMBMS_SUPPORTED
    executionFunctionArr_g[RLC_HANDLE_PRE_OPPORTUNITY] = ElProcesRlcPreOpportunity;
    executionFunctionArr_g[PROCESS_EMBMS_STRATEGY] = ElEmbmsStrategy;
#endif
}
#elif TDD_CONFIG
/*****************************************************************************
 * Function Name  : initTddExecutionLegFunctionArr 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function initializes executionFunctionArrTdd_g.
 *                  Each element of this array is a function pointer to
 *                  an execution leg.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129063) */
void initTddExecutionLegFunctionArr()
    /* SPR 5599 changes end (ZIP ID 129063) */
{
    LTE_MAC_UT_LOG(LOG_INFO,TDD_MANAGER,"[%s] Entry.",__func__);

    /* Initialize execution leg function array */
#ifdef DL_UL_SPLIT_TDD
    executionFunctionArrTdd_g[PROCESS_SUB_FRAME_IND_UL] = ElProcessSubFrameIndUl; 
    executionFunctionArrTdd_g[PROCESS_RLC_DL_CHECK_TIMER_EXPIRY] = 
        ElRlcDLCheckTimerExpiry;      
    executionFunctionArrTdd_g[PROCESS_RLC_DL_HDLR_REESTABLISH_Q] = 
        ElRlcDLHdlrReestablishQ;      
    executionFunctionArrTdd_g[PROCESS_RLC_DL_HDLR_PEN_DEL_Q] = 
        ElRlcDLHdlrPenDelQ;      
    executionFunctionArrTdd_g[PROCESS_DL_PERFORMANCE_STATS_UPDATE] = 
        ElProcessPerformanceStatsUpdate;
    executionFunctionArrTdd_g[PROCESS_UL_PERFORMANCE_STATS_UPDATE] = 
        ElProcessPerformanceStatsUpdate;
    /*UL RLC ELs*/
    executionFunctionArrTdd_g[WAIT_FOR_SUBFRAME_IND] = ElWaitForSubframeInd;
    executionFunctionArrTdd_g[PROCESS_RLC_HANDLE_DATA_FROM_MAC] = 
        ElRlcHandleDataFromMac;      
    executionFunctionArrTdd_g[PROCESS_RLC_UL_CHECK_TIMER_EXPIRY] = 
        ElRlcULCheckTimerExpiry;      
    executionFunctionArrTdd_g[PROCESS_RLC_UL_HDLR_REESTABLISH_Q] = 
        ElRlcULHdlrReestablishQ;      
    executionFunctionArrTdd_g[PROCESS_RLC_UL_HDLR_PEN_DEL_Q] = 
        ElRlcULHdlrPenDelQ;      
    executionFunctionArrTdd_g[PROCESS_UL_OAM_AND_RRC_MSG] = 
        ElProcessULOamAndRRcMsg; 
    executionFunctionArrTdd_g[TX_HI_DCI0_MSG] =ElTxHiDci0Msg;  
    executionFunctionArrTdd_g[PROCESS_UL_PDCCH_WITH_MUX] = 
        ElProcessUlPdcchWithMux;
    executionFunctionArrTdd_g[PROCESS_DL_PDCCH_SYNC] = 
        ElProcessDlPdcchSync;
#ifdef FAPI_CONTROL_FLOW
    executionFunctionArrTdd_g[FAPI_DL_FLOW_SYNC] = ElFapiDLFlowSyncControl;
    executionFunctionArrTdd_g[FAPI_UL_FLOW_SYNC] = ElFapiULFlowSyncControl;
#endif
#else

    /* SPR 11364 Start */
    executionFunctionArrTdd_g[PROCESS_DELETE_SCHEDULE_LC_LIST] = ElProcessDeleteScheduleLcList; 
    /* SPR 11364 End */

    executionFunctionArrTdd_g[PROCESS_HARQ_TIMER_EXPIRY_IF_TIME_LEFT] = 
        ElProcessDLHarqTimerExpiryIfTimeLeft;
    executionFunctionArrTdd_g[PROCESS_CQI_REPORTS_IF_TIME_LEFT] =
        ElProcessDLCQIReportsIfTimeLeft;
    executionFunctionArrTdd_g[PROCESS_DEMUX_QUEUE_IF_TIME_LEFT] =
        ElProcessDemuxMsgsIfTimeLeft;
    executionFunctionArrTdd_g[PROCESS_N_RRC_OAM_MSG_IF_TIME_LEFT] = 
        ElProcessNumRRCAndOAMMsgsIfTimeLeft;
    executionFunctionArrTdd_g[PROCESS_PDCCH_PDSCH_WITHOUT_MUX] = 
        ElProcessPdcchAndPdschWithoutMux;
    executionFunctionArrTdd_g[PROCESS_MUX_QUEUE] = ElProcessMuxQueue;
    executionFunctionArrTdd_g[PROCESS_MUX_QUEUE_UNTIL_LAST_NODE] = 
        ElProcessMuxQueueUntilLastNode;
    executionFunctionArrTdd_g[PROCESS_ENCODER_QUEUE_WITH_TX_UNTIL_LAST_DATA] =
        ElProcessEncoderQueueWithTxUntilLastData;
    executionFunctionArrTdd_g[PROCESS_ENCODER_QUEUE_WITH_TX] = 
        ElProcessEncoderQueueWithTx;
    executionFunctionArrTdd_g[PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA] = 
        ElProcessEncoderQueueWithoutTxUntilLastData;
    executionFunctionArrTdd_g[PROCESS_SPS_PDSCH]   = ElProcessSpsPdsch;
    executionFunctionArrTdd_g[PROCESS_SPS_PUSCH]   = ElProcessSpsPusch;
    executionFunctionArrTdd_g[PROCESS_N_CQI_REPORTS] = ElProcessNDLCQIReports;
    executionFunctionArrTdd_g[PROCESS_CQI_REPORTS_IF_TIME_LEFT] = ElProcessDLCQIReportsIfTimeLeft;
    executionFunctionArrTdd_g[PROCESS_PUSCH_FOR_DELAY_TWO] = 
        ElProcessPUSCHForDelayTwo;
    executionFunctionArrTdd_g[PROCESS_DL_SPS_STRATEGY]    = ElInvokeDLSpsSchStrategy;
    /* TDD Config 0 changes start */
    executionFunctionArrTdd_g[PROCESS_MERGE_UL_STARTEGY_NODES] = ElInvokeMergeULStrategyNodes;
    /* TDD Config 0 changes end */
    executionFunctionArrTdd_g[PROCESS_UL_SPS_STRATEGY]    = ElProcessUlSpsStrategy;
    executionFunctionArrTdd_g[PROCESS_RX_CONTROL_DATA_WITH_DEMUX] =
        ElProcessRxControlAndNumDataMsgsWithDemux;
    executionFunctionArrTdd_g[PROCESS_RX_UNTIL_SUBFRAME_IND] =
        ElProcessRxUntilSubFrameInd;
    executionFunctionArrTdd_g[PROCESS_RX_CONTROL_DATA_WITHOUT_DEMUX] = 
        ElProcessRxControlDataWithoutDemux;
    executionFunctionArrTdd_g[PROCESS_N_DEMUX_QUEUE] = ElProcessNumDemuxMsgs;
    executionFunctionArrTdd_g[PROCESS_N_RRC_OAM_MSG] =  
        ElProcessNumRRCAndOAMMsgs;
    executionFunctionArrTdd_g[PROCESS_TX_QUEUE_UNTIL_LAST_NODE] = ElProcessTxUntilLastNode;
    executionFunctionArrTdd_g[PROCESS_TX_QUEUE] = ElProcessTxQueue;
    executionFunctionArrTdd_g[TX_COMMON_CHANNEL_DATA] = ElTxCommonChannelData;

#ifdef UTFWK_SIMULATION
    executionFunctionArrTdd_g[RLC_HANDLE_TX_OPPORTUNITY_QOS] = ElRlcHandleTxOpportunityQOS;
    executionFunctionArrTdd_g[RLC_UPDATE_QLOAD] = 
        ElrlcUpdateQload;
#endif
    executionFunctionArrTdd_g[PDCP_TX_PROCESS_MESSAGE] = ElPdcpTxMessage;
    executionFunctionArrTdd_g[RLC_HANDLE_CTRL_PDU_RLC_AM] = ElRlcHandleControlPdu;
    executionFunctionArrTdd_g[RLC_PDCP_HANDLE_DATA_FRM_MAC] = ElRlcPdcpHandleData;
    executionFunctionArrTdd_g[RLC_TIMER_CHK_FOR_EXPIRY] = ElRlcTimerUpdateTickChk;
    executionFunctionArrTdd_g[RLC_TIMER_CHK_FOR_TX_EXPIRY] = ElRlcTimerUpdateTickChkForTx;
    /* SPR 609 changes start */    
    executionFunctionArrTdd_g[PDCP_TX_PROCESS_DELIVERY_IND] = ElPdcpTxProcessDeliveryInd;
    /* SPR 609 changes end */    
    executionFunctionArrTdd_g[RLC_HANDLE_COMMON_CHANNEL_AND_PENDING_QUEUE] = ElRlcHandleCommonChnlAndPendingQueue;
    /* SPR 13373 fix start */
    executionFunctionArrTdd_g[RLC_HANDLE_PENDING_DELETE_UE] =
        ElRlcHandlePendingDeleteUe;
    /* SPR 13373 fix end */
    executionFunctionArrTdd_g[PROCESS_RLC_PDCP_MSG] = ElProcessRLCAndPDCPMsgs;
#endif
    executionFunctionArrTdd_g[PROCESS_PRACH_FOR_S_SUBFRAME] = 
        ElProcessPrachForSpecialSubframe;
    /* Framework Integration Changes End*/
    executionFunctionArrTdd_g[PROCESS_DL_HARQ_ACK_NACK_QUEUE] = 
        ElProcessDLHarqAckNackQueue;
    executionFunctionArrTdd_g[WAIT_FOR_TDD_SUBFRAME_IND] = ElProcessRxControlAndNumDataMsgsWithDemux;
    executionFunctionArrTdd_g[PROCESS_HARQ_TIMER_EXPIRY] = 
        ElProcessDLHarqTimerExpiry;
    executionFunctionArrTdd_g[PROCESS_ENCODER_QUEUE_WITHOUT_TX] = 
        ElProcessEncoderQueueWithoutTx;
    /* For semi static changes */
    executionFunctionArrTdd_g[PROCESS_UL_CONFIG_MSG] =
        ElProcessUlConfig;
    executionFunctionArrTdd_g[TX_UL_CONFIG_MSG] =
        ElTransmitULConfig;
    executionFunctionArrTdd_g[PROCESS_SRS_FOR_U_SUBFRAME] = ElProcessSrsForUplinkSubframe;
    executionFunctionArrTdd_g[PROCESS_SRS_FOR_S_SUBFRAME] = ElProcessSrsForSpecialSubframe;
    executionFunctionArrTdd_g[RLC_HANDLE_TX_OPPORTUNITY] = ElRlcHandleTxOpportunity;
    /*coverity-530-32868 */
#ifdef OBSELETE_REL_8
    /*coverity-530-32868 */
    executionFunctionArrTdd_g[RLC_HANDLE_HARQ_FAIL_IND] = ElRlcHandleHarqFailInd;
#endif    
    executionFunctionArrTdd_g[PROCESS_MEM_CLEANUP] = ElProcessMemCleanup;
    executionFunctionArrTdd_g[PROCESS_MISC_UPDATES] = ElProcessMiscUpdates;
    executionFunctionArrTdd_g[PROCESS_PENDING_RESET_UE] = ElProcessPendingResetUE;  
    executionFunctionArrTdd_g[PROCESS_DL_SCH_STRATEGY] = ElInvokeDLSCHStrategy;
    executionFunctionArrTdd_g[PROCESS_UL_SCH_STRATEGY] = ElInvokeULSCHStrategy;
    executionFunctionArrTdd_g[PROCESS_PDCCH_PDSCH_WITH_MUX] = 
        ElProcessPdcchAndPdschWithMux;
    executionFunctionArrTdd_g[TX_DL_CONTROL_MSG] = 
        ElTransmitDLControMsg;
    executionFunctionArrTdd_g[TX_DL_DATA_MSG] =  ElTxDLDataMsg;
    executionFunctionArrTdd_g[PROCESS_DRX_FOR_ALL_UE] = ElProcessDRXForUEs;
    executionFunctionArrTdd_g[PROCESS_MEAS_GAP_FOR_ALL_UE] = ElProcessMeasGapForUEs;
    executionFunctionArrTdd_g[PROCESS_UL_HARQ_TIMER_EXPIRY] = 
        ElProcessULHarqTimerExpiry;

    executionFunctionArrTdd_g[PROCESS_PUSCH_FOR_DELAY_ONE] = 
        ElProcessPUSCHForDelayOne;
    executionFunctionArrTdd_g[PROCESS_DEMUX_QUEUE] = ElProcessDemuxQueue;
    executionFunctionArrTdd_g[PROCESS_UL_CSI_SINR_REPORTS] = ElProcessSINRReports; /* ULA_CHG */
    executionFunctionArrTdd_g[PROCESS_UL_RA] = ElProcessUlRandomAccess;
    executionFunctionArrTdd_g[PROCESS_PENDING_DELETE_UE] = 
        ElProcessPendingDeleteUE;  
    /* +- SPR 22494 */
    executionFunctionArrTdd_g[PROCESS_CQI_REPORTS] = ElProcessDLCQIReports;
    executionFunctionArrTdd_g[PROCESS_PRS_TRANSMISSION] = ElProcessPrsTransmission; /* PRS_CHANGES */
    /*SPR 16855 +-*/
    executionFunctionArrTdd_g[PROCESS_DL_OAM_AND_RRC_MSG] = ElProcessDLOamAndRRcMsg;
    executionFunctionArrTdd_g[PROCESS_UL_OAM_AND_RRC_MSG] = ElProcessULOamAndRRcMsg;
    executionFunctionArrTdd_g[PROCESS_UL_PENDING_DELETE_UE]  = ElProcessULPendingDeleteUE; 
    /*SPR 16855 +-*/
#ifdef DL_DATA_SEPARATION
    executionFunctionArrTdd_g[PROCESS_RLC_HANDLE_TX_OPPORTUNITY_WITH_ENCODER_PER_UE] = ElRlcHandleTxOpportunityWithEncoder;
    executionFunctionArrTdd_g[RLC_UPDATE_BUFF_OCC_TO_MAC] = ElRlcUpdateBuffOccToMac; 
    executionFunctionArrTdd_g[PROCESS_DL_SYNC_TX_DATA] = ElDLSyncEncoderThread; 
#endif
    executionFunctionArrTdd_g[PROCESS_E_CID_REPORTS] = ElProcessEcidReports;
    /* + SPR_14897 */
#if (!defined(DL_UL_SPLIT)  && !defined(DL_UL_SPLIT_TDD))
    /* - SPR_14897 */
    executionFunctionArrTdd_g[UPDATE_DL_TOP_SCH_LIST_SCELL_STATE] = ElupdateDlTopSchListScellState;
    executionFunctionArrTdd_g[PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS] = 
        ElProcessEncoderQueueOfOtherCells;      
    executionFunctionArrTdd_g[PROCESS_SET_STRATEGY_FD] = ElSetStrategyFd;
    executionFunctionArrTdd_g[PROCESS_CHECK_STRATEGY_FD] = ElCheckStrategyFd; 
    executionFunctionArrTdd_g[PROCESS_SET_PDCCH_PDSCH_FD] = ElSetPdcchPdschFd; 
    executionFunctionArrTdd_g[PROCESS_CHECK_PDCCH_PDSCH_FD] = ElCheckPdcchPdschFd;
    executionFunctionArrTdd_g[PROCESS_SET_MUX_FD] = ElSetMuxFd; 
    executionFunctionArrTdd_g[PROCESS_CHECK_MUX_FD] = ElCheckMuxFd; 
    executionFunctionArrTdd_g[PROCESS_SET_ENCODER_FD] = ElSetEncoderFd; 
    executionFunctionArrTdd_g[PROCESS_CHECK_ENCODER_FD] = ElCheckEncoderFd;
    executionFunctionArrTdd_g[PROCESS_PENDING_EVENT_FD] = ElProcessPendingEventFds;
    executionFunctionArrTdd_g[PROCESS_DL_CA_TIMER_EXPIRY] =
        ElProcessDLCATimerExpiry;
#endif
#ifdef LTE_EMBMS_SUPPORTED
    executionFunctionArrTdd_g[RLC_HANDLE_PRE_OPPORTUNITY] = ElProcesRlcPreOpportunity;
    executionFunctionArrTdd_g[PROCESS_EMBMS_STRATEGY] = ElEmbmsStrategy;
#endif
/*SPR 22919 Start*/
/*SPR 22919 End*/
    LTE_MAC_UT_LOG(LOG_INFO,TDD_MANAGER,"[%s] Exit .",__func__);
}
#endif

#ifdef DL_DATA_SEPARATION
EncoderExecutionFlowCreationParam executionFlowCreationParamArrForDLDataThread_g[NUM_OF_EXEC_FLOWS]
= {
    {
        0, /* thread id */
        99, /* thread priority */
        {
            DEFAULT_STARTUP_EXEC_LEG,
            0,
            0,
            PHY_DL_DELAY, /* delay to phy */
            5,
            0, /* Cell Index */
            {
                {PROCESS_RLC_HANDLE_TX_OPPORTUNITY_WITH_ENCODER_PER_UE,0,0,0},
		{PROCESS_SET_ENCODER_FD,0,0,0,},
		{PROCESS_CHECK_ENCODER_FD,0,0,0,},
		{PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS,0,0,0},
                /* SPR 8712 changes */
#ifdef DL_UL_SPLIT                             
                {PROCESS_RLC_DL_CHECK_TIMER_EXPIRY,0,0,0},
#endif                             
                /* SPR 8712 changes */
                {TX_DL_DATA_MSG,0,0,0},
            }
        }
    }
};

/* SPR 8712 changes */
extern SInt32 ev_fd_g[MAX_NUM_CELL];

/*****************************************************************************
 * Function Name  : ElRlcHandleTxOpportunityWithEncoder
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description     : Update qload of the UEs for which oppurtiunity indication 
 *                  has been received and process the MAC encoder queue. 
 *****************************************************************************/
/** CA Phase2 Changes Start **/
 void ElRlcHandleTxOpportunityWithEncoder(ExecutionLegFunctionArg * execLegFuncArg_p)
{
    eventFD_t tempfd;
    UInt8 lastUeFlagPCell=FALSE;   
    UInt16 retVal = MAC_SUCCESS;
    UInt32 qCount = 0;
    UInt8  expectedDummyNode=0;

    if(RLC_FAILURE != rlcLayerInitialized_g &&
            pdcpContextIsInitialized())
        {
            /* SPR 15909 fix start */
            tickType_t currentTick ;
            /* SPR 15909 fix end */
#ifdef FDD_CONFIG    
            /* CA Changes start */
            execLegFuncArg_p->globalTick= GET_CURRENT_TICK_FOR_CELL(
                    execLegFuncArg_p->cellIndex);
            getSFAndSFN(&execLegFuncArg_p->sysFrame, &execLegFuncArg_p->subFrame,
                    execLegFuncArg_p->cellIndex);

            l2ThreadHealthStoreTick_g.rlcMacThread[execLegFuncArg_p->cellIndex].rlcThread[RLC_DL_THREAD] = globalTTITickCount_g;
            /* CA Changes end */
            currentTick = execLegFuncArg_p->globalTick;
#else
            execLegFuncArg_p->advanceDLGlobalTick = getCurrentTick() + DL_ADVANCE_TICK;

            getSFAndSFN(&execLegFuncArg_p->advanceDLSystemframe, &execLegFuncArg_p->advanceDLSubframe, execLegFuncArg_p->cellIndex);
            execLegFuncArg_p->advanceDLSubframe += DL_ADVANCE_TICK; /* DL_ADVANCE_TICK 2 */
            if ( execLegFuncArg_p->advanceDLSubframe >= MAX_SUB_FRAME)
            {
                execLegFuncArg_p->advanceDLSubframe -= MAX_SUB_FRAME;
                execLegFuncArg_p->advanceDLSystemframe++;

                if ( execLegFuncArg_p->advanceDLSystemframe == MAX_SFN_VALUE)
                {
                    execLegFuncArg_p->advanceDLSystemframe = 0;
                }
            } 
            currentTick = execLegFuncArg_p->advanceDLGlobalTick;
#endif
            while(1)
            {
                /* CA Changes start  */
                qCount = COUNT_MAC_UE_OPPOR_IND_CIRC_Q(execLegFuncArg_p->cellIndex);
                while( qCount-- )
                {
                    if(LTE_RLC_PUSH_DUMMY_NODE ==  rlcHandleTxOppurtunity(currentTick + execLegFuncArg_p->delayToPhy,execLegFuncArg_p->cellIndex))
                    {
                        expectedDummyNode++;
                    }
                    /* CA Changes end  */
#ifdef FDD_CONFIG
                    retVal = processRlcEncoderQueuePerUE( currentTick, 
                            execLegFuncArg_p->delayToPhy,
                            /* +- SPR 18490 */
                            execLegFuncArg_p->subFrame,
                            0,/* is transmitter functionality added*/
                            &lastUeFlagPCell,
                            execLegFuncArg_p->cellIndex,
                            execLegFuncArg_p->cellIndex);
#elif TDD_CONFIG
                    retVal = processRlcEncoderQueuePerUE( 
                            currentTick, 
                            execLegFuncArg_p->delayToPhy,
                            /* +- SPR 18490 */
                            execLegFuncArg_p->advanceDLSubframe,
                            0,/* is transmitter functionality added*/
                            &lastUeFlagPCell,
                            execLegFuncArg_p->cellIndex,
                            execLegFuncArg_p->cellIndex);
#endif
                    if(retVal != MAC_SUCCESS)
                    {
                        /* Log "Tick[%u ]Func [processRlcEncoderQueuePerUE] returned errorCode [%d]\n", getCurrentTick(), retVal); */
                    }

                    if(TRUE == lastUeFlagPCell)
                    {
                        break;
                    }   
                }
                if(TRUE == lastUeFlagPCell)
                {
                    break;
                }
                eventfd_read_wrapper(ev_fd_g[execLegFuncArg_p->cellIndex],&tempfd);
            }
            /* In case of CA two dummy nodes are expected. All noded from MUX 
               of PCELL and SCELL are terminated by these nodes */
            while(IS_CA_ENABLED() && 
                  CHECK_CELL_STATE(!execLegFuncArg_p->cellIndex) && 
                  expectedDummyNode < macParams_g.numCellsConfigured)
            {
	       qCount = COUNT_MAC_UE_OPPOR_IND_CIRC_Q(execLegFuncArg_p->cellIndex);
	       while( qCount-- )
	       {
		  if(LTE_RLC_PUSH_DUMMY_NODE ==  rlcHandleTxOppurtunity(currentTick + execLegFuncArg_p->delayToPhy,execLegFuncArg_p->cellIndex))
                  {
                     expectedDummyNode++;
                     continue; 
                  }
		  eventfd_read_wrapper(ev_fd_g[execLegFuncArg_p->cellIndex],&tempfd);
               }
            }
#ifdef FDD_CONFIG
            processMACEncoderQueue( execLegFuncArg_p->globalTick , 
                    execLegFuncArg_p->delayToPhy,
                    /* +- SPR 18490 */
                    execLegFuncArg_p->subFrame,
                    0,/* is transmitter functionality added*/
                    /* +- SPR 18490 */
                    execLegFuncArg_p->cellIndex,
                    execLegFuncArg_p->cellIndex);
#elif TDD_CONFIG
            processMACEncoderQueue( 
                    execLegFuncArg_p->advanceDLGlobalTick,
                    execLegFuncArg_p->delayToPhy,
                    /* +- SPR 18490 */
                    execLegFuncArg_p->advanceDLSubframe,
                    0,/* is transmitter functionality added*/
                    /* +- SPR 18490 */
                    execLegFuncArg_p->cellIndex,
                    execLegFuncArg_p->cellIndex);
#endif
        }
}
/* SPR 8712 changes */

/*****************************************************************************
 * Function Name  : ElRlcUpdateBuffOccToMac
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
 void ElRlcUpdateBuffOccToMac(ExecutionLegFunctionArg * execLegFuncArg_p)
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(execLegFuncArg_p)
    /* +- SPR 17777 */
}
extern SInt32 ev_fd_tx_g[MAX_NUM_CELL];

/*****************************************************************************
 * Function Name  : ElDLSyncEncoderThread
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles Downlink Sync Encoder.
 *****************************************************************************/
 void ElDLSyncEncoderThread(ExecutionLegFunctionArg * execLegFuncArg_p)
{
    eventFD_t tempfd={0};
    eventfd_read_wrapper(ev_fd_tx_g[execLegFuncArg_p->cellIndex],&tempfd);
}

#endif
/* TDD Config 0 changes start */
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : ElInvokeMergeULStrategyNodes
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This execution leg invoke mergeULStrategyNode function 
 *                  to merge the two UL Strtegy out put node Lists.
 *                  The Lists are populated by ULStartegy called for ulSubframe
 *                  +k and +7 respectively.
 *                  It provides the merged output node to PDCCH.
 *****************************************************************************/
void ElInvokeMergeULStrategyNodes(ExecutionLegFunctionArg * execLegFuncArg_p)
{
    mergeULStrategyNodes(execLegFuncArg_p->advanceDLGlobalTick,
            /* +- SPR 17777 */
            execLegFuncArg_p->cellIndex);
}
#endif
/* TDD Config 0 changes end */


/** El Sync Changes Start **/
/* + SPR_14897 */
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
/* - SPR_14897 */
/*****************************************************************************
 * Function Name  : ElProcessPendingEventFds
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles event FDs pending in container.
 *****************************************************************************/
 void ElProcessPendingEventFds (ExecutionLegFunctionArg * execLegFuncArg_p)
{
#if 0	//ysl remove in 0729 for overshoot deadlock: TTI among cells are not synchronized, so we shall not clear sync semaphores before TTI processing
    if(IS_CA_ENABLED())
    {
        resetPrevTickEventFdContainer(execLegFuncArg_p->cellIndex);
    }
#endif
}
/*****************************************************************************
 * Function Name  : ElSetStrategyFd
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles Downlink Sync Strategy.
 *****************************************************************************/
 void ElSetStrategyFd (ExecutionLegFunctionArg * execLegFuncArg_p)
{
    if(IS_CA_ENABLED())
    {
        CELL_STRATEGY_COMP_IND_FOR_OTHER_CELLS(execLegFuncArg_p->cellIndex)
    }
    return;
}

/*****************************************************************************
 * Function Name  : ElCheckStrategyFd
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function check and wait Downlink Sync strategy.
 *****************************************************************************/
 void ElCheckStrategyFd (ExecutionLegFunctionArg * execLegFuncArg_p)
{   
    if(IS_CA_ENABLED())
    {
        CHECK_AND_WAIT_FOR_OTHER_CELLS_STRATEGY(execLegFuncArg_p->cellIndex);

        semWait(&cellSyncSem_g);
        cellElSyncTracker[execLegFuncArg_p->cellIndex] = PDSCH_SYNC; 
        semPost(&cellSyncSem_g);
    }
    return;
}

/*****************************************************************************
 * Function Name  : ElSetPdcchPdschFd
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles Downlink Sync pdcch-pdsch.
 *****************************************************************************/
 void ElSetPdcchPdschFd (ExecutionLegFunctionArg * execLegFuncArg_p)
{
    /* + Critical section changes */
    UInt8 isLowestCellIndex = 0;

    if (IS_CA_ENABLED())
    {
        semWait(&cellSyncSem_g);
        isLowestCellIndex = (macParams_g.lowestRunningCellIndex == execLegFuncArg_p->cellIndex);
        semPost(&cellSyncSem_g);

        if(isLowestCellIndex)
        {
            CELL_PDCCH_PDSCH_COMP_IND_FOR_OTHER_CELLS(execLegFuncArg_p->cellIndex)
        }
    }
    /* - Critical section changes */
    return;
}

/*****************************************************************************
 * Function Name  : ElCheckPdcchPdschFd
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function check and wait Downlink Sync pdcch-pdsch.
 *****************************************************************************/
 void ElCheckPdcchPdschFd (ExecutionLegFunctionArg * execLegFuncArg_p)
{
    if(IS_CA_ENABLED())
    {
        CHECK_AND_WAIT_FOR_OTHER_CELLS_PDSCH(execLegFuncArg_p->cellIndex);
        semWait(&cellSyncSem_g); 
        cellElSyncTracker[execLegFuncArg_p->cellIndex] = MUX_SYNC; 
        semPost(&cellSyncSem_g); 
    }
    return;
}

/*****************************************************************************
 * Function Name  : ElSetMuxFd
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles Downlink Sync mux.
 *****************************************************************************/
 void ElSetMuxFd (ExecutionLegFunctionArg * execLegFuncArg_p)
{
    if(IS_CA_ENABLED())
    {
        CELL_MUX_COMP_IND_FOR_OTHER_CELLS(execLegFuncArg_p->cellIndex)
    }
    return;
}

/*****************************************************************************
 * Function Name  : ElCheckMuxFd
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function check and wait Downlink Sync mux.
 *****************************************************************************/
 void ElCheckMuxFd (ExecutionLegFunctionArg * execLegFuncArg_p)
{
    if(IS_CA_ENABLED())
    {
        CHECK_AND_WAIT_FOR_OTHER_CELLS_MUX(execLegFuncArg_p->cellIndex)
            /* + Critical section changes */
            semWait(&cellSyncSem_g);
        cellElSyncTracker[execLegFuncArg_p->cellIndex] = ENCODER_SYNC;
        semPost(&cellSyncSem_g);
        /* - Critical section changes */
    }
    return;
}

/*****************************************************************************
 * Function Name  : ElSetEncoderFd
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles Downlink Sync encoder.
 *****************************************************************************/
 void ElSetEncoderFd (ExecutionLegFunctionArg * execLegFuncArg_p)
{
    if(IS_CA_ENABLED())
    {
        CELL_ENCODER_COMP_IND_FOR_OTHER_CELLS(execLegFuncArg_p->cellIndex)
    }
    return;
}

/*****************************************************************************
 * Function Name  : ElCheckEncoderFd
 * Inputs         : execLegFuncArg_p - This contains paramters like global tick,
 *                                     delay to phy , sub frame number etc and
 *                                     also few arguments specifically
 *                                     necessary for certain execution legs.
 * Outputs        : None
 * Returns        : None
 * Description    : This function check and wait Downlink Sync Encoder.
 *****************************************************************************/
 void ElCheckEncoderFd (ExecutionLegFunctionArg * execLegFuncArg_p)
{
    if(IS_CA_ENABLED())
    {
        CHECK_AND_WAIT_FOR_OTHER_CELLS_ENCODER(execLegFuncArg_p->cellIndex)
            /* + Critical section changes */
            semWait(&cellSyncSem_g);
        cellElSyncTracker[execLegFuncArg_p->cellIndex] = FALSE_SYNC;
        semPost(&cellSyncSem_g);
        /* - Critical section changes */
    }
    return;
}

/*****************************************************************************
 * Function Name  : resetPrevTickEventFdContainer
 * Inputs         : cellIndex
 * Outputs        :
 * Returns        :
 * Description    : This func reset all the event fds set for this cell on 
 *                  previous tick.
 *****************************************************************************/
void resetPrevTickEventFdContainer(InternalCellIndex cellIndex)
{
    /******** By Default EVENT FDs are used for sync threads. Mutex & semaphores are 
      implemented but not fully tested. **********/
#ifdef SEM_SYNC

#elif MUTEX_SYNC

#else
    eventFD_t tempfd = {0};
    fd_Set  readfds;
    SInt32 highestFD = 0;
    TimeSpec timer;
    /* SPR 13132 Changes Start */
    /* SPR 18122 Changes Start*/
    UInt8 fdContainerIndex = !MODULO((getCurrentTickForCell(cellIndex)),2);
    /* SPR 18122 Changes End*/
    /* SPR 13132 Changes End  */

    timer.tv_nsec = 0;
    timer.tv_sec = 0;
    lteFD_ZERO(&readfds );
    lteFD_SET(ev_fd_encoder_g[!cellIndex][fdContainerIndex], &readfds);
    lteFD_SET(ev_fd_pdsch_g[!cellIndex][fdContainerIndex], &readfds);
    lteFD_SET(ev_fd_mux_g[!cellIndex][fdContainerIndex], &readfds);
    lteFD_SET(ev_fd_strategy_g[!cellIndex][fdContainerIndex], &readfds);

    /* + SPR 13643 Changes */
    highestFD = highestEventFD_g[!cellIndex][fdContainerIndex];
    /* - SPR 13643 Changes */

    if (0 <  pselect_wrapper(highestFD +1 , &readfds, PNULL, PNULL, &timer, PNULL))
    {
        if(lteFD_ISSET(ev_fd_strategy_g[!cellIndex][fdContainerIndex], &readfds))
        {
            eventfd_read_wrapper(ev_fd_strategy_g[!cellIndex][fdContainerIndex],&tempfd);
        }
        if(lteFD_ISSET(ev_fd_pdsch_g[!cellIndex][fdContainerIndex], &readfds))
        {
            eventfd_read_wrapper(ev_fd_pdsch_g[!cellIndex][fdContainerIndex],&tempfd);
        }
        if(lteFD_ISSET(ev_fd_mux_g[!cellIndex][fdContainerIndex], &readfds))
        {
            eventfd_read_wrapper(ev_fd_mux_g[!cellIndex][fdContainerIndex],&tempfd);
        }
        if(lteFD_ISSET(ev_fd_encoder_g[!cellIndex][fdContainerIndex], &readfds))
        {
            eventfd_read_wrapper(ev_fd_encoder_g[!cellIndex][fdContainerIndex],&tempfd);
        }
    }
#endif
}
/* +- SPR 21950 */
/* +- SPR_14897 */
#endif
/** El Sync Changes End **/




