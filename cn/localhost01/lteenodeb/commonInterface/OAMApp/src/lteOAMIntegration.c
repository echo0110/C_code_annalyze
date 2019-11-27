/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteOAMIntegration.c,v 1.5.2.2.4.2.2.9 2010/10/25 08:36:26 gur23971 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteOAMIntegration.c,v $
 * Revision 1.5.2.2.4.2.2.9  2010/10/25 08:36:26  gur23971
 * stats related changes
 *
 * Revision 1.5.2.2.4.2.2.8  2010/10/22 12:44:09  gur04640
 * Statistics code merged from branch BRFramework_RRC1_0_RLC_AM_MAC_8_8_FAPI_optimizations
 *
 * Revision 1.5.2.2.4.2.2.7.2.1  2010/10/13 10:47:46  gur04640
 * Changes done for implementing Stats using shared memory
 *
 * Revision 1.5.2.2.4.2.2.7.2.1  2010/10/12 02:46:54  gur04640
 * Changes done for implementing Stats using shared memory
 *
 * Revision 1.5.2.2.4.2.2.7  2010/10/01 08:08:01  gur04640
 * Changed the real time priority of l2 to 80
 *
 * Revision 1.5.2.2.4.2.2.6  2010/09/29 06:28:50  gur04640
 * Change priority of L2 to 90 and policy to SCHED_FIFO
 *
 * Revision 1.5.2.2.4.2.2.5  2010/09/29 05:07:49  gur04640
 * Core bind done at start
 *
 * Revision 1.5.2.2.4.2.2.4  2010/09/28 06:33:15  gur10121
 * Error handled in InitializeLoggerContext SPR 662
 *
 * Revision 1.5.2.2.4.2.2.3  2010/09/27 16:49:12  gur10121
 * SPR 662 Fix
 *
 * Revision 1.5.2.2.4.2.2.2  2010/09/27 14:36:41  gur10694
 * UE config Fix and Review comments
 *
 * Revision 1.5.2.2.4.2.2.1  2010/09/21 15:45:42  gur20491
 * FAPI changes
 *
 * Revision 1.5.2.2  2010/08/02 08:57:14  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.3  2010/03/09 06:51:56  gur20439
 * core binding chnages:
 *
 * Revision 1.2  2010/03/08 07:51:53  gur19479
 * updated for Ip address of adapter
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.32.2.7  2009/12/05 13:14:52  gur19479
 * updated for thread binding
 *
 * Revision 1.1.2.32.2.6  2009/11/09 06:38:09  gur20439
 * removal of sleep call as its not require.
 *
 * Revision 1.1.2.32.2.5  2009/10/23 05:31:29  gur20439
 * support to see downlink traffic.
*
* Revision 1.1.2.32.2.4  2009/10/09 05:52:35  gur11318
* Changes for oam and rrc creating seperate entity.
*
* Revision 1.1.2.32.2.3  2009/09/30 12:57:23  gur20439
* code change to support seperation of OAM and RRC ip.
*
* Revision 1.1.2.32.2.2  2009/09/30 10:36:44  gur20439
* MAC1.2 related integration changes
*
* Revision 1.1.2.32.2.1  2009/09/11 12:49:02  gur12140
* modified to integrate with MAC 1.2
*
* Revision 1.1.2.32  2009/09/03 06:15:11  gur19140
* adding cycle count code
*
* Revision 1.1.2.30  2009/08/14 15:27:28  gur19140
* l2 14 aug delivery changes
*
* Revision 1.1.2.29  2009/08/07 10:05:00  gur19836
* Graceful shutdown of PHY Sim code added
*
* Revision 1.1.2.28  2009/08/06 13:26:01  gur19836
* Graceful Exit code added
*
* Revision 1.1.2.27  2009/08/03 16:14:32  gur19140
* memory leak debuugging
*
* Revision 1.1.2.26  2009/07/30 08:28:56  gur19140
* performance changes
*
* Revision 1.1.2.25  2009/07/28 15:36:12  gur19836
* Downlink Issues Resolved, Latency Measurement code added
*
* Revision 1.1.2.24  2009/07/28 15:04:37  gur19140
* added total packets reach at pdcp
*
* Revision 1.1.2.23  2009/07/21 08:04:55  gur19140
* Bug Fixes
*
* Revision 1.1.2.22  2009/07/20 10:19:11  gur11318
* Updated to remove warnings for traffic Gen variable change to UChar8
*
* Revision 1.1.2.21  2009/07/20 08:20:32  gur19479
* Modified for SRB handling
*
* Revision 1.1.2.20  2009/07/16 05:23:11  gur19140
* remove warning
*
* Revision 1.1.2.19  2009/07/16 05:04:01  gur19140
* tick related problem resolve
*
* Revision 1.1.2.18  2009/07/15 15:23:41  gur19836
* eNodeB build with Adapter bug fix
*
* Revision 1.1.2.17  2009/07/14 14:11:37  gur20435
* Removed flag INTEL_EXEC_SCHEDULER_OP1
*
* Revision 1.1.2.16  2009/07/14 06:39:12  gur19479
* updated for pdcp user
*
* Revision 1.1.2.15  2009/07/14 05:43:11  gur20435
* PDCP Optimizations
*
* Revision 1.1.2.13  2009/07/09 09:07:11  gur20435
*
* RLC Optimizations
*
* Revision 1.1.2.12  2009/07/08 08:42:28  gur19836
* Tx flow Optimization changes
*
* Revision 1.1.2.11  2009/07/07 13:17:09  gur20435
* *** empty log message ***
*
* Revision 1.1.2.10  2009/07/07 11:39:14  gur20435
*
* Changed flag name
*
* Revision 1.1.2.9  2009/07/07 11:30:53  gur20435
*
* Made changes for Integration Scheduler thread binding etc
*
* Revision 1.1.2.8  2009/07/07 11:16:47  gur19140
* *** empty log message ***
*
* Revision 1.1.2.7  2009/07/07 08:07:24  gur19836
* Compilation Issues Resolved
*
* Revision 1.1.2.6  2009/07/03 14:05:11  gur19479
* updated for pdcp adapter
*
* Revision 1.1.2.5  2009/07/03 09:20:55  gur19479
* updated for pdcp adapter
*
* Revision 1.1.2.4  2009/06/30 22:36:10  gur19479
* updated for Integration
*
* Revision 1.1.2.3  2009/06/23 10:49:05  gur19836
* Changes Done for Execution Scheduler
*
* Revision 1.1.2.2  2009/06/19 14:28:57  gur19836
* Changes Done for Execution Scheduler
*
* Revision 1.1.2.1  2009/06/09 12:04:22  gur19140
* first wave of Integration
*
*
****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#ifdef FLEXRAN
#include <sys/prctl.h>
#endif
#include "lteLayer2CommanTypes.h"
#include "lteMacRRMInterface.h"
#include "lteMacRRMIpcPort.h"
#include "lteConfigFileHdlr.h"
#include "lteComOAMIntegration.h"
#include "lteCommonStatsManager.h"
/* SPR 21804 Fix Start */ 
#if defined( LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
#include "lteLayer2DLTG.h"
#endif
#include "lteOAMIntergration.h"
#include "lteMsgPool.h"
#include "lteMacExecutionScheduler.h"
#include "lteMacParseUtil.h"
#include "lteRelay.h"

#include "logging.h"
#include "lteCircQueue.h" 
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
#include "lteMacPhyInterface.h"
#endif
#include "lteMacComPorting.h"
#include "lteMacOAMIntf.h"
#include "lteMacRRMIntfInfo.h"
#include "lteMacRRMPeriodicMgr.h"
/* ICIC changes end */

#include "lteMacCellConfMgr.h"
#ifdef UTFWK_SIMULATION
#include <pthread.h>
#endif

/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
/* FDD_Warning_Fix */
#include "lteMessageQIntf.h"
/* FDD_Warning_Fix */
#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"/*SPR 19221 +-*/
#endif
#ifdef LAYER2_PHYSIM_THREAD
#include "lteEnbPhySim.h"

ULong32 physimExecutionSchedulerThId_g;
void* physimThreadExecutionSchedular(void* param);
extern SInt32 macPhyEventFd_g;
#endif
/* L2 Porting Start */
extern UInt32 ReadFromSocket(UInt8 *buf, UInt16 moduleId);

LTE_SQUEUE* recvDataFromPhyQ_gp[MAX_NUM_CELL] = {PNULL};
/*+ SPR 18399*/
/*- SPR 18399*/
/* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
#include "lteRlcUmTxEmbmsOperation.h"
#endif
/* eMBMS Code Changes End */

#ifdef LINUX_PC_TEST
#include "TestFrameApi.h"
short adapter_hdl_msg_callback(char *buf, long len);
short adapter_init_socket(void);
#endif

/*SPR 21727 Changes Start*/
#if  defined RLC_PDCP_IPC_INTF
/*SPR 21727 Changes End*/
ULong32 rlcULExecutionSchedulerThId_g ;
#endif

#ifdef FLEXRAN
extern int flexran_ipc_init(unsigned int cell_num, unsigned char type, int mac_cell_id);
#endif



/* SPR 21804 Fix Start */ 
#if defined( LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
extern void* dlTgThreadExecutionSchedular(void * param);
#endif
/* + SPR 17439 */
extern void rlcCheckTimerExpiry(void);
extern void startLogReader(lteL2LogOption *l2Input_p);


extern void initMacLayer(void);
extern void createPDCPAdapterThread(void);
/* - SPR 17439 */
#ifdef LTE_EMBMS_SUPPORTED
extern void createSYNCAdapterThread(void);
#endif
/* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
extern void lteRlcEmbmsLPHandler(LTE_CIRCQUEUE *rlcEmbmsHpToLpQ_p);
extern LTE_CIRCQUEUE rlcEmbmsHpToLpQ_g;
#endif
/* eMBMS Code Changes End */



/****************************************************************************
 * Private Definitions
 ****************************************************************************/
#ifdef FLEXRAN
extern UInt32 g_flexran_channelId[MAX_NUM_CELL] ; 
#endif
/* SPR 19288 change start */
/* SPR 19288 change end */
/* SPR 4506 Fix Start */
extern UInt8 egtpuLayerState_g;
/* SPR 4506 Fix End */
#if defined  (KPI_STATS) || defined  (PERF_STATS)
SInt32 pdcpOamMsgEventFd_g = -1;
#endif
#ifdef KPI_STATS 
/* + SPR 17439 */
extern void resetAndReportThpStats(void);
/* - SPR 17439 */
#endif
#define SOCKET_BUFFER_SIZE 4*1024


#define CREATE_DFLT_MEMORY_POOL_FOR_INIT \
{ \
    createMemPool(SOCKET_BUFFER_SIZE, 100); \
    createMemPool(2048, 20); \
    createMemPool(8192, 10); \
    createMemPool(128, 5); \
    /* CA Changes start */ \
    createMemPool(sizeof(CellAndInitParams), MAX_NUM_CELL); \
    createMemPool(sizeof(InitParams),MAX_NUM_CELL);\
    createMemPool(sizeof(CellConfigParams), MAX_NUM_CELL); \
    /* CA Changes end */\ 
    createMemPool(16392, 1); \
}
/* SPR 21660 changes end_Sahil */
/* Split chnages end*/
/* SPR 8737 start end */ 
/* End 128 UE: Memory Changes */

/****************************************************************************
 * Private Types
 ****************************************************************************/

/*Changes DROP1*/
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
LTE_CIRCQUEUE rcvdRRCOAMMsg_g;
#endif
/*SPR 16855 +-*/
/* High Priority Reuest Queues */
RRC_OAM_LPTOHP_THDH_PRIO_TYPE rrcOamLPtoHPThdHPrioQ_g[MAX_NUM_CELL][MAX_THD];
/* Response Queues */
RRC_OAM_HPTOLP_THDH_PRIO_TYPE rrcOamHPtoLPThdHPrioQ_g[MAX_NUM_CELL][MAX_THD];
extern SInt32 unlinkMsgQueueForICC(const char *name);
/* FD for receiving message from HP thread */
MQD_T recvMsgFrmHPThdFd_g = MQ_ERROR;


/*SPR 16855 +-*/
#ifdef TDD_CONFIG
extern void initCountofFnSets(void);
#endif

extern void lteRelaySockHandler(UInt8 *rcvBuffer,SInt32 bytesRead);
extern SInt32 recvFromRelaySockHandler(UInt8 *rcvBuffer,UInt32 rcvBuffer_len, UInt32 moduleId,SockAddr *sockAddr);
extern void initPacketRelay(void);
extern void processControlInfoFromPhysical (UInt8  * msgBuf, Int bytesRead);
extern relayConfigInfo relayConfigInfo_g;
extern MQD_T periodicReportFd_g;


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/
#define TESTING 1
#define STDIN 0
/* SPR 19288 change start */
#define HOSTNAME_MAX_LEN 64
/* SPR 19288 change end */
/****************************************************************************
 * Exported Variables
 ****************************************************************************/

#ifdef FDD_CONFIG
#if (!defined(DL_UL_SPLIT) && !defined(DL_DATA_SEPARATION))
/* + SPR 17439 */
void initExecutionFlowElGroups(void);
/* - SPR 17439 */
#endif
#endif
/* + SPR 13822 Changes */
void putEntryInPhyDataQFromLPThread(InternalCellIndex cellIndex);
extern UInt8 * receiveDataFromPhy(SInt32 *bytesRead_output, InternalCellIndex internalCellIndex);
/* - SPR 13822 Changes */

extern UInt32 tddHandleProcessRxSubFrameFlow (void);
extern UInt32 tddHandleExecutionLegsFlow (tddOamRrcReceiveInfo *oamRrcInfo_p );
extern UInt32 tddHandleCellSetupFlow (void *msgBuf, SInt32 bytesRead);


extern UInt16 pdcpContextIsInitialized(void);
extern UInt16 rlcLayerInitialized_g;

void cleanupLayer2(void);
UInt8 initLayer2(void);

/* Init Indication Chg */
void l2InitIndication(void);
/* Init Indication Chg */

/*SPR 17585 +*/
#if  !defined(PDCP_GTPU_INTF)
extern UInt32 s1UInitIndication(void);
#endif
/*SPR 17585 -*/
extern void pdcpTrigInitInd( UInt8 instanceId );
extern void setPdcpContextInitialized(UInt16 flag);
/* SPR 19288 change start */
/* SPR 19288 change end */

extern void getTimeWaitDurationForState(TimeSpec * timeout_p);

/* + SPR 17439 */
extern void ElProcessHighPrioThrResForOAMAndRRCMsgFSM(void);
/* - SPR 17439 */
extern void ElProcessReqTimerExpireHandlerFSM(InternalCellIndex internalCellIndex);
extern void ElProcessOAMAndRRCMsgFSM(UInt32 delayToPhy, UInt8* recvBuffer_p, UInt16 byteRead);
/* CA Changes start */
extern SInt32 recvControlInfoFromPhysical(void* msgBuf_p,
        InternalCellIndex internalCellIndex);
extern void ElProcessPhyResForOAMAndRRCMsgFSM(UInt8* recvBuffer_p, 
        UInt16 byteRead,UInt8 cellIndex);
#ifndef FLEXRAN
extern SInt32 receiveStartPhyRes(UInt8 *msgBuf,
        InternalCellIndex internalCellIndex);
#endif
extern UInt32 handleStartPhyRes(UInt8 *msgBuf,SInt32 bytesRead, InternalCellIndex internalCellIndex);
/* CA Changes end */


extern UInt16 rxPortMAC_g,txPortMacOAM_g,txPortMacRRC_g, txPortRRM_g;
extern UInt16 rxPortRLC_g,txPortRlcOAM_g,txPortRlcRRC_g;
extern Char8 l2ModuleHostName[HOSTNAME_MAX_LEN];

extern Char8 oamModuleHostName[HOSTNAME_MAX_LEN];
extern Char8 rrcModuleHostName[HOSTNAME_MAX_LEN];
#if defined(ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
extern Char8 x2uModuleHostName[HOSTNAME_MAX_LEN];
#endif
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
extern Char8 macIPAddress_g[HOSTNAME_MAX_LEN];
extern Char8 phyIPAddress_g[MAX_NUM_CELL][HOSTNAME_MAX_LEN];
#endif
extern Char8 rrcIPAddress_g[HOSTNAME_MAX_LEN];
extern Char8 oamIPAddress_g[HOSTNAME_MAX_LEN];
/*SPR 20863 FIX*/
extern Char8 rrmIPAddress_g[HOSTNAME_MAX_LEN];
/*SPR 20863 FIX*/
UInt8  initIndTimer_g = 0;
/*SPR 16855 +-*/
/* + SPR 17439 */
extern UInt32 initPDCPLayer(void);
/* - SPR 17439 */
/*SPR 16855 +-*/
/* CA Changes start */
extern UInt16 phyRxPort_g[MAX_NUM_CELL];
extern UInt16 phyTxPort_g[MAX_NUM_CELL];
/* CA Changes end */
#ifdef DL_UL_SPLIT_TDD
extern UInt16 phyRxPortUl_g;
#endif
extern void  getMsgPoolStats(void);
extern void  resetMsgPoolPeakVal(void);
extern void  resetMemPoolPeakVal(void);
extern Int pdcpSocketRx;
extern SInt32 readSockFD_g;
extern SInt32 rxSockFD_g;
void destroyExecutionLegs(void);
/* coverity-530 CID-24578*/
#ifdef RLC_STANDALONE_SETUP
void createExecutionLegs(void);
/* coverity-530 CID-24578*/
void* rlcpdcpThreadExecutionScheduler(void * param);
#endif
void* pdcpAdapterThreadExecutionSchedular(void * param);
#ifdef LTE_EMBMS_SUPPORTED
void* syncAdapterThreadExecutionSchedular(void * param);
#endif
#if   RLC_PDCP_IPC_INTF
/*SPR 21727 Changes End*/
extern void* rlcULExecutionScheduler(void * param);
#endif

/*rel4-drop2 changes*/
/* SPR 20199 fix start*/
/*SPR 16855 +-*/
#ifdef DL_DATA_SEPARATION
void* encoderSchedular(void * param);
#endif
extern UInt32 rlcHandleSocketMsgs( void *, SInt32);
extern UInt8 * rlcReceiveSocketMsgs(SInt32* bytesRead);
extern void pdcpElTxProcessSocket(UInt8 *buf,UInt32 length);
extern void readRRCMsgFromSocket(void);
extern SInt32 pdcpOamSendMessage(UInt8 *data_p,UInt16 size);

/*SPR 21545 Start*/
extern UInt8 instanceL2_g;
/*SPR 21545 End*/
/*SPR 21653 changes start*/
/*SPR 21653 changes end*/
/* + SPR 17439 */
extern void initUeIdxMap(void);
/* - SPR 17439 */
/*Warning Fix 27Dec*/
/* SPR 19288 change start */
/* SPR 19288 change end */

/* CA Changes start */
extern UInt16 phyControlTxPort_g[MAX_NUM_CELL];
extern UInt16 phyControlRxPort_g[MAX_NUM_CELL] ;
/* CA Changes end */
extern void stopLogReader(void);

#ifndef UE_SIM_TESTING
extern SInt32 rlcRrcCnfOutputQId_g;
extern SInt32 rlcRrcCnfInputQId_g;
extern void initRlcRrcCnfQueue ( SInt32 *);
/*+ SPR 21121*/
extern LTE_CIRCQUEUE rlcUlSchedToLPCircQ_g;
extern LTE_CIRCQUEUE rlcLPtoRlcUlSchedCircQ_g;
/*- SPR 21121*/
#endif

/* + SPR 17439 */
extern void * lteRlcRecvMsgWithIccQ(void);
/* - SPR 17439 */
extern UInt32 ElRlcProcessHPrioThrResForOAMAndRRCMsgFSM(void* recvBuffer_p, SInt32 byteRead);
extern UInt32 ElRlcProcessOAMAndRRCMsgFSM(UInt8* recvBuffer_p, SInt32 byteRead);
extern UInt32 rlcProcessOAMAndRRCMsgFSM(UInt8* recvBuffer_p, SInt32 byteRead);

/* + SPR 17439 */
extern void getPdcpUserThroughput(void);
/* - SPR 17439 */
extern void sendCleanupReqToPhySim(void);
/* + SPR 17439 */
extern void cleanupPdcpAdapter(void);
/* - SPR 17439 */
extern void registerPhyForSubframeInd(void);
LTE_SEM boSem_g[MAX_NUM_CELL];
/* SPR 21407 fix start */
/* SPR 21407 fix end */
/*SPR_7217_changes_start*/
#ifdef RLC_STANDALONE_SETUP
LTE_SEM rlcpdcpThreadSem_g;
#endif
/*SPR_7217_changes_end*/
UInt8 rlcLoop_g = FALSE;
#ifdef UTFWK_SIMULATION
ULong32 PhyTimerThread_g;
UInt32 count_mutex     = PTHREAD_MUTEX_INITIALIZER;
UInt32 condition_var   = PTHREAD_COND_INITIALIZER;
extern UInt8 recvFromOamDone_g;
#endif
#define NUM_OF_FIELDS 25

#define SQCOUNT_IDX 0
#define PDCP_EL_TX_DRB_IDX  1
#define HANDLE_TX_OPP_IDX   2
#define ROHC_TIMERS_IDX     3
#define TIMER_UPDATE_IDX    4
#define EXPIRED_TIMERS_IDX  5
#define HANDLE_MAC_DATA     6
#define PDCP_EL_RX_IDX      7
#define PDCP_EL_TX_SRB_IDX  8
#define TX_DEL_IND_IDX      9
#define TX_DISCARD_TIMERS_IDX  10
#define COM_CH_DATA_IDX        11
#define COM_CH_TX_OPP_IDX      12
#define PEN_DEL_Q_TX_IDX       13
#define REESTAB_Q_TX_IDX       14
#define PEN_DEL_Q_RX_IDX       15
#define REESTAB_Q_RX_IDX       16
/* MAC Execution Legs */
#define ENCODER_PROCESS_IDX    17
#define DL_SCH_IDX             18
#define UL_TRANS_Q_IDX         19
#define READ_MSG_FROM_SOCKET_IDX 20
#define UL_SCH_IDX             21
#define TIMER_EXPIRY_MAC_IDX   22
#define DEMUX_Q_PROCESS_IDX    23
#define TICK_IDX               24

#define  MAC_PHY_RX_PORT      8888
#define  MAC_PHY_TX_PORT      9999
/* COVERITY 10265 */
#define MAX_INIT_BUFF_SIZE    32
/* COVERITY 10265 */

Char8 *latencyArr[] = {"SQCOUNT",
    "PDCP_EL_TX_PROCESS_MESSAGE",
    "HANDLE_TX_OPPURTUNITY",
    "ROHC_TIMERS",
    "TIMER_UPDATE",
    "EXPIRED_TIMERS",
    "HANDLE_MAC_DATA",
    "PDCP_EL_RX",
    "PDCP_EL_TX_SRB",
    "TX_DEL_IND",
    "TX_DISCARD_TIMERS",
    "COM_CH_DATA",
    "COM_CH_TX_OPP",
    "PEN_DEL_Q_TX",
    "REESTAB_Q_TX",
    "PEN_DEL_Q_RX",
    "REESTAB_Q_RX",
    "ENCODER_PROCESS",
    "DL_SCH",
    "UL_TRANS_Q",
    "READ_MSG_FROM_SOCKET",
    "UL_SCH",
    "TIMER_EXPIRY_MAC",
    "DEMUX_Q_PROCESS",
    "GET_TICK"
};

typedef struct latencyFieldsT
{
    UDouble32 totalLatency;
    UDouble32 bestLatency;
    UDouble32 currentLatency;
    UDouble32 maxLatency;
    UInt64 numOfMeasurements;
}LatencyFields;

LatencyFields latencyMeas_g[NUM_OF_FIELDS];


/* SPR 4422 Fix Start */
ULong32 threadId_g[MAX_THREAD_ID];
extern UInt8 waitFromStateEvent_g ;
UInt8  thr_count_g = 0;
void handleStopEvent(int signum);
/* + SPR 17439 */
void stopOtherThread (void);
/*SPR 20908 +*/
SInt32 sendL2InitIndToOAM(UInt8 *cnfBuff_p, UInt16 msgLen,SInt32 sendSocktFD_g);
extern SInt32 pdcpRxSockFD_g;
/*SPR 20908 +*/

/* - SPR 17439 */
/* SPR 4422 Fix End */

extern void rlcSendMsgToOam(UInt8 *IndBuff_p,
        UInt16 currentLen );

#ifdef OVERSHOOT_CORRECTION
/*+ Dyn UEs To Be SchPerTTI +*/
/* + SPR 17439 */
void getIndexForReqElProcessingCycles(void);
/* - SPR 17439 */
/*- Dyn UEs To Be SchPerTTI -*/
#endif

#ifdef FDD_CONFIG

void diffLatency(TimeSpec start, TimeSpec end, UInt8 index, UInt64 count)
{
    TimeSpec temp;
    UDouble32 tempLatency = 0;

    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    /* Calculate latency in ms */
    tempLatency = (((UDouble32)temp.tv_sec * 1000000000 + temp.tv_nsec)/(1))/1000000;

    /* Increment total latency */
    latencyMeas_g[index].totalLatency += tempLatency;

    /* Replace best latency if first time or if new best figures reached */
    if((latencyMeas_g[index].bestLatency == 0) || 
            (tempLatency/count < latencyMeas_g[index].bestLatency)) 
    {
        latencyMeas_g[index].bestLatency = tempLatency/count;
    }

    if((tempLatency/count) > latencyMeas_g[index].maxLatency)
    {
        latencyMeas_g[index].maxLatency = tempLatency/count;
    }  

    latencyMeas_g[index].currentLatency = tempLatency/count;
    latencyMeas_g[index].numOfMeasurements += count;

    //  LTE_LOG(LOG_INFO,PNULL,"Difference: SECONDS %ld NANO SECONDS %ld\n", temp.tv_sec, temp.tv_nsec);
    //  LTE_LOG(LOG_INFO,PNULL,"time in ms for one loop %f \n\n",(((UDouble32)temp.tv_sec
    //          * 1000000000 + temp.tv_nsec)/(1))/1000000);
}

/****************************************************************************
 * Function Name  : rxDataFromStdIn
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API receives the data from STDIN and as per user choice
 *                  do the processing.
 ****************************************************************************/

/* + SPR 17439 */
extern void getRlcQueueLoadForAllUe(void);
/* - SPR 17439 */

extern UInt32 recvAckFromPhy_g ;
extern UInt32 popedAckatMac ;
extern UInt32 rejectByMac_g ;
extern UInt32 sduTransmitted_g  ;
extern UInt32 sduReceivedFromUl_g  ;
extern UInt32 totalPDCPRecved_g ;
extern UInt32 macUeDataReq_cnt_g;
extern UInt32 createAndSendTBToPhysical_cnt_g;
extern UInt32 updateSduAndControlInfoForTB_cnt_g;

#endif

#include "lteMacExecutionLegs.h"
#ifdef DL_UL_SPLIT
extern void* executionFlowThread(void * executionFlowThreadArgs_p);
#else
extern void* executionFlowThread(void * executionFlowThreadArgs_p,ExecutionFlowAndCellIndex *execFlowAndIndex);
#endif

extern SInt8 sockRx_g;
/* CA Changes start */
#ifndef FLEXRAN
extern SInt32 phyRxSockFD_g[MAX_NUM_CELL];
extern SInt32 phyControlSockFD_g[MAX_NUM_CELL];
#endif
/* CA Changes end */
extern SInt32 phyFrameIndSockFD_g;
/* SPR 19288 change start */
/* SPR 19288 change end */



/* SPR 8347 START */

#ifdef FDD_CONFIG
MacRetType callExecutionFlowThreads( ExecutionFlowCreationParam *
		executionFlowThreadParamArr_p
#ifndef DL_UL_SPLIT        
        ,ExecutionFlowAndCellIndex execFlowAndIndex /*SPR 18490 +-*/
#endif        
        )
{
    UInt32 execFlowCount = 0;
    MacRetType retType = MAC_SUCCESS;


    for(execFlowCount = 0; execFlowCount < NUM_OF_EXEC_FLOWS
            ; execFlowCount++)
    {
        executionFlowThreadParamArr_p[execFlowCount].threadArguments.
            threadIndex = execFlowCount;

#ifdef DL_UL_SPLIT
        executionFlowThread( (void*)(&(executionFlowThreadParamArr_p[execFlowCount].
                        threadArguments)));
#else
        executionFlowThread( (void*)(&(executionFlowThreadParamArr_p[execFlowCount].
                        threadArguments)), &execFlowAndIndex);
#endif
    }

    return retType;
}
#endif

/*Change DROP1*/
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
/****************************************************************************
 * Function Name  : recvAndValidateOAMAndRRCMsg
 * Inputs         : numOfMsg
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This func has recieves the message from the socket and does
 *                  basic validation.
 ****************************************************************************/

void recvAndValidateOAMAndRRCMsg(UInt8 numOfMsg)
{
    UInt8 *msgBuf_p = PNULL;
    SInt32 bytesRead     = 0;
    UInt16 destModuleId  = 0;
    UInt16 msgLen        = 0;
    UInt8 *msg_p         = PNULL; /* current_p access location in msg buffer*/
    UInt8 count[10];
    fd_Set readfds;
    /* Coverity 10542 fix start */
    lteFD_ZERO(&readfds);
    /* Coverity 10542 fix end */
    /* Coverity 26432 Fix */
    createMemPool(MAX_DATA_SIZE, 1);
    GET_MEM_FROM_POOL(UInt8, msgBuf_p, MAX_DATA_SIZE, PNULL);

    /* Coverity Warning : 60496 */ 
    if (PNULL == msgBuf_p)
    {
        LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
        return;
    }
    /* Coverity Warning : 60496 */ 
    memSet(msgBuf_p, 0, MAX_DATA_SIZE);

    while ( numOfMsg-- )
    {
        bytesRead = 0;
        /* SPR 6949 Fix Start */
        if (( bytesRead = recvfrom_wrapper(rxSockFD_g, msgBuf_p, MAC_API_HEADER_LEN,MSG_PEEK,PNULL,0)) == MAC_RECV_ERROR)
            /* SPR 6949 Fix End */
        {
            if ( errno != EAGAIN )
            {
                ALARM_MSG (MAC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID, MAJOR_ALARM);

                LOG_MAC_MSG(OAM_RRC_INVALID_MSG_LEN_ID,LOGWARNING,MAC_RRC_INF,
                        getCurrentTick(),
                        errno,bytesRead,LINE_NUMBER,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"INVALID_MSG");

            }
            lteFD_CLR(rxSockFD_g,&readfds);
            continue;
        }

        if ( bytesRead < MAC_API_HEADER_LEN )
        {
            lteWarning ("Wrong Message   Length Received \n");   

            LOG_MAC_MSG(OAM_RRC_INVALID_MSG_LEN_ID,LOGWARNING,MAC_RRC_INF,
                    getCurrentTick(),
                    errno,bytesRead,LINE_NUMBER,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"INVALID_MSG");


            /* SPR 6949 Fix Start */
            if( MAC_RECV_ERROR == recvfrom_wrapper( rxSockFD_g, msgBuf_p, msgLen,0,PNULL,0))
                /* SPR 6949 Fix End */
            {
                if ( errno != EAGAIN )
                {

                    ALARM_MSG (MAC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID, MAJOR_ALARM);

                    LOG_MAC_MSG(OAM_RRC_INVALID_MSG_LEN_ID,LOGWARNING,MAC_RRC_INF,
                            getCurrentTick(),
                            errno,bytesRead,LINE_NUMBER,
                            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"INVALID_MSG");

                }
            }
            lteFD_CLR(rxSockFD_g,&readfds);
            continue ;    
        }
        msg_p = msgBuf_p;
        /* Coverity 55797 */

        /*Parse the buffer header */

        msg_p += 2;
        msg_p += 2; 
        destModuleId  = LTE_GET_U16BIT(msg_p);
        msg_p += 2; 
        /* decode the msgBuffer and get APIID(2 bytes) */
        msg_p += 2;
        msgLen        = LTE_GET_U16BIT(msg_p);
        msg_p += 2;

        if ( (MAC_MODULE_ID != destModuleId) && (PHY_MODULE_ID != destModuleId))
        {
            /* SPR 6949 Fix Start */
            if(MAC_RECV_ERROR == recvfrom_wrapper( rxSockFD_g, msgBuf_p, msgLen,0,PNULL,0))
                /* SPR 6949 Fix End */
            {
                if ( errno != EAGAIN )
                {
                    ALARM_MSG (MAC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
                }
            }
            lteFD_CLR(rxSockFD_g,&readfds);
            continue;
        }
        else{

            pushNodeCircQ(&rcvdRRCOAMMsg_g,(void *)count);
        }
    }/* end of while */
    freeMemPool(msgBuf_p);
    msgBuf_p = PNULL;
}
#endif
/*Change DROP1*/

/*BUG10380 Changes Starts*/
/* <!---NMM Changes Before Cell Config Starts ---!> */
/* <!---NMM Changes Before Cell Config Ends ---!> */
/*BUG10380 Changes Ends*/

/* Global Descriptor to set FDs one time only */
/* SPR 7787 fix start */
fd_Set staticReadFds;  
/* SPR 7787 fix end */
/* Global variable to set number of FDs in select */
static Int highestFD_g = 0;
/*****************************************************************************
 * Function Name: setFd
 * Inputs       : fd - FD which needs to be set
 * Outputs      : None
 * Returns      : None
 * Description  : This function sets the FD in select to receive from socket.
 *****************************************************************************/
void setFd( SInt32 fd )
{
    /* SPR 14030 fix start */
    if ( 0 < fd )
        /* SPR 14030 fix end */
    {
        lteFD_SET(fd,&staticReadFds);
        if(fd > highestFD_g) 
        {
            highestFD_g = fd;
        }
        /* SPR 14030 fix start */
    }
    /* SPR 14030 fix end */
}

/*****************************************************************************
 * Function Name: setMsgQFd
 * Inputs       : fd - FD which needs to be set
 * Outputs      : None
 * Returns      : None
 * Description  : This function sets the message queue FD in select to receive
 *                from socket.
 *****************************************************************************/
void setMsgQFd( MQD_T fd )
{
    if ( 0 < fd )
    {
        lteFD_SET(fd,&staticReadFds);
        if(fd > highestFD_g) 
        {
            highestFD_g = fd;
        }
    }
}

/****************************************************************************
 * Function Name  : receiveMsg
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API has an infinite loop which waits for the messages
 *                  received either on Socket or STDIN
 ****************************************************************************/
void receiveMsg(void)
{
    UInt8 buffer[32]={0};
    fd_Set readfds;
    SInt32 retval = 0;
    /* CA Changes start */
    InternalCellIndex *internalCellIndex_p = PNULL;
#ifndef FLEXRAN
    InternalCellIndex cellIndex = 0;
    UInt8 numCellConfigured = 0; 
    UInt8 *msgBuf = PNULL;
    CellAndInitParams *cellParams_p = PNULL;
    /* CA Changes end */
#endif	
#ifdef KPI_STATS
    /* SPR 10579 changes start */
    UInt32 kpiRet = 0x00;
    /* SPR 10579 changes end */
#endif
#ifdef PERF_STATS
    UInt8 perfRet = 0X00;
#endif
#if (defined(KPI_STATS) || defined(PERF_STATS))
    eventFD_t kpiTempfd;
#endif

#ifndef UTFWK_SIMULATION
    TimeSpec timeout;
#else
    timeVal timeout;
#endif
    l2InitIndication();

#ifdef LAYER2_PHYSIM_THREAD
    eventfd_t tempfd;
#endif
#if (defined(KPI_STATS) ||  defined(PERF_STATS))
    timeout.tv_sec = 1; 
    timeout.tv_nsec = 0; 
    pdcpOamMsgEventFd_g = eventfd_initialize_wrapper(0,0);
#endif
    lteFD_ZERO(&readfds);
    lteFD_ZERO(&staticReadFds);

#ifndef FLEXRAN
    /* CA Changes start */
    for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {    
        setFd( phyRxSockFD_g[cellIndex] );
        setFd( phyControlSockFD_g[cellIndex] );
    }    
#endif

    /* CA Changes end */
    setFd( readSockFD_g );
    /* Set FDs statically once */
    setFd( rxSockFD_g );
    setMsgQFd( recvMsgFrmHPThdFd_g );
    setFd( rlcRrcCnfOutputQId_g );
#if (defined(KPI_STATS)|| defined(PERF_STATS))
    setFd( pdcpOamMsgEventFd_g ); 
#endif

    /* SPR 2241 changes start */
/* SPR 21804 Fix Start */ 
#if defined( LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
    /* Set PDCP FD */
    setFd( pdcpSocketRx );
    /* Set Relay FD */
    setFd( relayConfigInfo_g.s1ApTxFd );
#endif
    /* SPR 2241 changes end */
#ifndef LAYER2_PHYSIM_THREAD
#else
    setFd( macPhyEventFd_g );
#endif

    while(1)
    {    
        readfds = staticReadFds;
        /*SPR 16855 +-*/
        /* SPR 18022 Fix Start */
#if !((defined  KPI_STATS ) || (defined PERF_STATS))
        timeout.tv_sec = 1; 
        /* SPR 15754 Fix Start */
        timeout.tv_nsec = 0;
        /* SPR 15754 Fix End */
#endif	
        /* SPR 18022 Fix End */

        /*SPR 16855 +-*/
        /* Get the timeout value for select */
        getTimeWaitDurationForState(&timeout);
        /* Read Data from socket and STDIN*/
#ifndef UTFWK_SIMULATION
        retval = pselect_wrapper(highestFD_g + 1, &readfds, PNULL, PNULL, &timeout, PNULL);
#else
        retval = select_wrapper(highestFD_g + 1, &readfds, PNULL, PNULL, &timeout);
#endif
        if( -1 == retval )
        {
            if (errno == EINTR)
            {
                lteFD_ZERO(&readfds);
            }
        }
				if( 0 != retval 


                  )
                {
#ifndef FLEXRAN
/*SPR 21001 Changes start */
                    numCellConfigured = cellSpecificParams_g.numOfCells 
                        ;
                    cellIndex = 0; 
                    SInt32 bytesRead     = 0; 
                    while (numCellConfigured--) 
                    {
                        cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex];
                        /* SPR 817 changes start */
#ifndef LAYER2_PHYSIM_THREAD
                        /* SPR 817 changes end */
                        if (lteFD_ISSET(phyControlSockFD_g[cellIndex], &readfds)) 
                        {
                            msgBuf = (UInt8 *)getMemFromPool(FAPI_CTRL_MSG_MAX_BUF_SIZE, PNULL);
                            /*+COVERITY 5.3.0 - 29783*/
                            if (PNULL != msgBuf)
                                /*+COVERITY 5.3.0 - 29783*/
                            {
                                if( 0 != (bytesRead = recvControlInfoFromPhysical((void *)msgBuf, 
                                                cellIndex)))
                                {
                                    ElProcessPhyResForOAMAndRRCMsgFSM(msgBuf,bytesRead,cellIndex);
                                }
                                /* + Coverity_Stack Fix */
                                freeMemPool(msgBuf);
                                /* - Coverity_Stack Fix */
                            }
                        }
#endif
                        /* + SPR 13822 Changes */
                        else if (lteFD_ISSET(phyRxSockFD_g[cellIndex],&readfds))
                        {
                            if(cellParams_p->cellState == CELL_WAITING_FOR_FIRST_SFI)
                            {
                                msgBuf = (UInt8 *)getMemFromPool(FAPI_CTRL_MSG_MAX_BUF_SIZE, PNULL);
                                if (PNULL != msgBuf)
                                {
                                    bytesRead = receiveStartPhyRes(msgBuf,cellIndex);
                                    if(bytesRead)
                                    {
                                        ElProcessPhyResForOAMAndRRCMsgFSM(msgBuf,bytesRead,cellIndex);
                                    }
                                    freeMemPool(msgBuf);
                                }
                            }
                            else if( (cellParams_p->cellState == CELL_PHY_RUNNING) && 
                                    (cellElSyncTracker[cellIndex] == INVALID_SYNC))
                            {
                                putEntryInPhyDataQFromLPThread(cellIndex);
                            }
                        }
                        /* - SPR 13822 Changes */
                        cellIndex++;
                    }
                    /* CA Changes end */
/*SPR 21001 Changes end */
#endif
                    /*SPR 16855 +-*/
#ifdef KPI_STATS
                    /* this function should be called before processOAMAndRRCMsg function */
                    kpiRet = checkForKPIUpdateReporting();
#endif  
#ifdef PERF_STATS
                    perfRet = checkForPerfStatsUpdateReporting();
#endif  
                    /* SPR 18022 Fix Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
                    if(lteFD_ISSET(pdcpOamMsgEventFd_g, &readfds))
                    {    
                        eventfd_read_wrapper(pdcpOamMsgEventFd_g,&kpiTempfd);
                    }    
#endif 
                    /* SPR 18022 Fix End */

				/* CA Changes start */
                    if  (lteFD_ISSET(rxSockFD_g,&readfds))
                    {    
                        UInt8 mem_type      = 0; 
                        UInt8 *msgBufForRRCOAMMsgs       = PNULL;
                        UInt8 msgHdr[RRC_MAC_API_HEADER_LEN] = {0}; 
                        SInt32 bytesRead    = 0;
                        /*SPR 20863 FIX*/
                        UInt8 sourceModuleId = 0;
                        UInt8 *rrmHdr =PNULL;
                        /*SPR 20863 FIX*/

                        msgBufForRRCOAMMsgs = receiveOAMAndRRCMsg(msgHdr,&bytesRead,&mem_type);

                        /*+COVERITY 5.3.0 - 29783*/
                        if ((msgBufForRRCOAMMsgs) && (0 < bytesRead)
                           )
                            /*-COVERITY 5.3.0 - 29783*/
                        {
                            /* +- SPR 17777 */
                            /*SPR 20863 FIX*/
                            rrmHdr = (UInt8 *)&msgHdr;
                            rrmHdr += 2;
                            sourceModuleId = LTE_GET_U16BIT(rrmHdr);
                            if(sourceModuleId == RRM_MODULE_ID)
                            {
                                /*SPR 21006 +-*/
                                processRRMMsg(msgBufForRRCOAMMsgs,bytesRead);
                            }
                            else
                            {
                                processOAMAndRRCMsg(msgHdr,msgBufForRRCOAMMsgs,bytesRead);
                            }
                            /*SPR 20863 FIX*/
                            if(0 == mem_type)
                            {
                                freeMemPool(msgBufForRRCOAMMsgs);
                            }
                        }
                        /*+ SPR 18399*/
                        /*- SPR 18399*/
                    }

                    if (lteFD_ISSET(rlcRrcCnfOutputQId_g,&readfds))
                    {
                        void *recvMsgBuf_p  = PNULL;
                        recvMsgBuf_p = lteRlcRecvMsgWithIccQ();
                        if(recvMsgBuf_p)
                        {
                            ElRlcProcessHPrioThrResForOAMAndRRCMsgFSM(recvMsgBuf_p,0);
                        }
                    }
                    
                    /*+ SPR 21121*/
                    UInt32 sQcount = COUNT_RLC_UL_SCH_TO_LP_Q();
                    while(sQcount--)
                    {
                        RlcRrcOamHiPrioQNode *recvMsgBuf_p  = PNULL;
                        DEQUEUE_RLC_UL_SCH_TO_LP_Q(RlcRrcOamHiPrioQNode, (void**)&recvMsgBuf_p);
                        if(recvMsgBuf_p)
                        {
                            ElRlcProcessHPrioThrResForOAMAndRRCMsgFSM(recvMsgBuf_p,0);
                        }
                    }
                    /*- SPR 21121*/

                    /*SPR 21001 Changes start */
                    if (lteFD_ISSET(readSockFD_g,&readfds))/*cov 62252 +-*/
                    {
                        /* L2 Porting Start */
                        UInt8 *recvMsgBuf_p  = PNULL;
                        SInt32 bytesRead =0;
                        recvMsgBuf_p = rlcReceiveSocketMsgs(&bytesRead);
                        /*+COVERITY 5.3.0 - 29783*/
                        if(recvMsgBuf_p
                                && (CLEANUP_LAYER2 == rlcProcessOAMAndRRCMsgFSM(recvMsgBuf_p,bytesRead)))	                
                            /*-COVERITY 5.3.0 - 29783*/
                        {
                            /* SPR 20469 Fix + */
                            freeMemPool((void *)recvMsgBuf_p);
                            /* SPR 20469 Fix - */
                            cleanupLayer2();
                            return;
                        }
                        /* SPR 7587 Changes Start */
                        /* Moved freemempool to rlcHandleSocketMsgs()*/
                        /* SPR 7587 Changes End */
                        /* L2 Porting End */
                    }
/*SPR 21001 Changes end */

                    /* ICIC Changes start */
/*SPR 20863 FIX*/
                    if ((periodicReportFd_g > 0)&&(lteFD_ISSET(periodicReportFd_g,&readfds)))
                    {
                        /* Function call for processing periodic reports */
                        lteMessageQRecv(periodicReportFd_g,buffer);

                        macInterfaceProcessRRMPeriodicReports(buffer);
                    }
                    /*SPR 21006 +-*/
/*SPR 20863 FIX*/
                    /* ICIC changes end */

                    if( 0 < recvMsgFrmHPThdFd_g && 
                            lteFD_ISSET(recvMsgFrmHPThdFd_g, &readfds) )
                    {
                        /* Receive message in message queue from HP thread */
                        lteMessageQRecv( recvMsgFrmHPThdFd_g, buffer);

                        /* Check for data into the circular queues. */
                        /* CA Changes Start */
                        internalCellIndex_p = (InternalCellIndex *)&buffer;
                        rrcOamCheckHiPrioResp(*internalCellIndex_p);
                        /* CA Changes End */
                    }

                    /* SPR 2241 changes start */
/* SPR 21804 Fix Start */ 
#if defined( LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
                    if (lteFD_ISSET(pdcpSocketRx,&readfds))
                    {
                        /* + Coverity_Stack Fix */
                        UInt8 *buf = PNULL;
                        buf = (UInt8 *)getMemFromPool(SOCKET_BUFFER_SIZE, PNULL);
                        /* - Coverity_Stack Fix */
                        UInt32 length = 0; 
                        length = ReadFromSocket(buf, PDCP_MODULE_ID);
                        pdcpElTxProcessSocket(buf,length);
                        /* + Coverity_Stack Fix */
                        freeMemPool(buf);
                        /* - Coverity_Stack Fix */
                    }
                    if (lteFD_ISSET(relayConfigInfo_g.s1ApTxFd,&readfds))
                    {
                        SInt32 bytesRead = -1;
                        /* + Coverity 25187 */
                        UInt8 *rcvBuffer = (UInt8 *)getMemFromPool(
                                MAX_RECV_BUFF_SIZE, PNULL);
                        /* - Coverity 25187 */

                        bytesRead = recvFromRelaySockHandler( rcvBuffer, 
                                MAX_RECV_BUFF_SIZE, RELAY_S1AP_MODULE_ID, PNULL );
                        if(bytesRead)
                            lteRelaySockHandler(rcvBuffer,bytesRead);

                        /* + Coverity 25187 */
                        freeMemPool( rcvBuffer );
                        /* - Coverity 25187 */
                    }
#endif
                    /* SPR 2241 changes end */
                }

                /* SPR 15975 Fix Start */
                else if (0 == retval)
                    /* SPR 15975 Fix End */
                    {

#ifdef KPI_STATS
                        kpiRet = checkForKPIUpdateReporting();
#endif 
#ifdef PERF_STATS
                        perfRet = checkForPerfStatsUpdateReporting(); 
#endif 
                    }

#ifdef KPI_STATS
                if(kpiRet)
                {
                    distributeKPIStats(kpiRet);
                }
                resetAndReportThpStats();
                calculateBestDuration((TimeSpec*)&timeout);
#endif
#ifdef  PERF_STATS
                if(perfRet)
                {
                    distributePerfStats(perfRet);
                }
                calculateBestDurationPerfStats((TimeSpec*)&timeout);
#endif

                if (0 == retval)
                {
                    /* Call this EL for PHY messages in case response is not received
                     * from PHY */
                    /* Need to be cell-wise */
                    ElProcessReqTimerExpireHandlerFSM(0);
                }
                /* verify that Layer 2 ( MAC, RLC, PDCP, GTP) is initialized */
                if ( (CELL_PHY_INIT <= cellSpecificParams_g.cellConfigAndInitParams_p[0]->cellState)
                        && rlcLayerInitialized_g
                        &&  pdcpContextIsInitialized()
                        /* SPR 4506 Fix Start */
#if  !defined(PDCP_GTPU_INTF)
                        && egtpuLayerState_g 
#endif
                        /* SPR 4506 Fix End */
                   )
                {
                    /* No need to send L2 init indiaction as all layers of L2 are
                     * now initailized by OAM. */
                }
                else
                {
                    /* Send L2 Init indication as all the layers are not yet
                     * initialized */
                    l2InitIndication();
                }

                /*SPR 16855 +-*/
    }
}

/****************************************************************************
 * Function Name  : cleanupLayer2
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API cleanup the registered Layer2 modules. After this
 *                  initLayer2 API call is mandatory to process ahead.
 ****************************************************************************/
void cleanupLayer2(void)
{
    InternalCellIndex cellIndex = 0;  
    /* SPR 5178 changes starts here */
    rlcLayerInitialized_g = RLC_FAILURE;
    destroyExecutionLegs();
    /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
    /*SPR 21188 -*/
    cleanupPdcpAdapter();
#endif
    cleanUpRlcLayer();
    /* Coverity 54614*/ 
    for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
    { 
        circQDeInit(&rrcOamPendingQ_g[cellIndex]);
    }
    /* Coverity 54614*/ 
    cleanupMemPool();
    cleanupMsgPool();
}                    

/****************************************************************************
 * Function Name  : setPhyIpAndPort
 * Inputs         : MAC Transmitting and receiving port for PHY
 *                : Cell Index
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : set all the passed port to MAC port based on flag 

 ****************************************************************************/
void setPhyIpAndPort(UInt8 *flag_p)
{

    InternalCellIndex cellIndex = 0;
    for (cellIndex = 0;cellIndex < MAX_NUM_CELL;cellIndex++)
    {
        if(flag_p[cellIndex])
        {
            phyTxPort_g[cellIndex] = phyPortInfo_g[cellIndex].txPortPHY;
            phyRxPort_g[cellIndex] = phyPortInfo_g[cellIndex].rxPortPHY;  

            strNCpy(phyIPAddress_g[cellIndex],phyPortInfo_g[cellIndex].phyIP,sizeof(phyIPAddress_g[cellIndex]));
        }
        else
        {
            phyRxPort_g[cellIndex] = MAC_PHY_RX_PORT;
            phyTxPort_g[cellIndex] = MAC_PHY_TX_PORT;
            strNCpy(phyIPAddress_g[cellIndex],"127.0.0.1",sizeof("127.0.0.1"));
        }
    }
}

/****************************************************************************
 * Function Name  : setMacIpAndPort
 * Inputs         : MAC receiving port, OAM tx port,  RRC tx port
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : set all the passed port to MAC port based on flag 

 ****************************************************************************/
void setMacIpAndPort(UInt16 flag, UInt16 rxPortMAC, UInt16 txPortOAM, 
        UInt16 txPortRRC,Char8 *oamIp_p, Char8 *rrcIp_p,
        /*SPR 20863 FIX*/
        Char8 *macIp,
        Char8 *rrmIp,
        UInt16 txPortRRM
        /*SPR 20863 FIX*/
#ifdef DL_UL_SPLIT_TDD                     
        ,UInt16 rxPortULThreadPhy
#endif
        )
{
    if(flag)
    {
        rxPortMAC_g = rxPortMAC;
        txPortMacOAM_g = txPortOAM;
        txPortMacRRC_g = txPortRRC;
        /*SPR 20863 FIX*/
        txPortRRM_g = txPortRRM;
#ifdef DL_UL_SPLIT_TDD                     
        phyRxPortUl_g = rxPortULThreadPhy;
#endif
        strNCpy(oamIPAddress_g,oamIp_p,sizeof(oamIPAddress_g));
        strNCpy(rrcIPAddress_g,rrcIp_p,sizeof(rrcIPAddress_g));
        /*SPR 20863 FIX*/
        strNCpy(macIPAddress_g,macIp,sizeof(macIPAddress_g));
        strNCpy(rrmIPAddress_g,rrmIp,sizeof(rrmIPAddress_g));
        /*SPR 20863 FIX*/
    }
    else
    {
        rxPortMAC_g = MAC_RX_OAM_RRC_PORT;
        txPortMacOAM_g = MAC_TX_OAM_PORT;
        /*SPR 20863 FIX*/
        txPortRRM_g = MAC_TX_RRM_PORT;
        /*SPR 20863 FIX*/
        txPortMacRRC_g = MAC_TX_RRC_PORT;
        strNCpy(oamIPAddress_g,"127.0.0.1",sizeof("127.0.0.1"));
        strNCpy(rrcIPAddress_g,"127.0.0.1",sizeof("127.0.0.1"));
        strNCpy(macIPAddress_g,"127.0.0.1",sizeof("127.0.0.1"));
        /*SPR 20863 FIX*/
        strNCpy(rrmIPAddress_g,"127.0.0.1",sizeof("127.0.0.1"));
        /*SPR 20863 FIX*/
    }
}

/****************************************************************************
 * Function Name  : setRlcIpAndPort
 * Inputs         : RLC receiving port, OAM tx port,  RRC tx port,OAM RRC IP,
 *                  L2 IP
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : set all the passed port to RLC port based on flag 

 ****************************************************************************/
void setRlcIpAndPort(UInt16 flag, UInt16 rxPortRLC, UInt16 txPortOAM, 
        UInt16 txPortRRC, Char8 *oamIp_p, Char8 *rrcIp_p, 
        Char8 *l2Ip_p
#if defined(ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
        /*Dual Connectivity SeNB changes +*/
        ,UInt16 rxX2uDataPortRLC
        ,Char8  *gtpuIp_p
        ,UInt16 gtpuRxPort
        /*Dual Connectivity SeNB changes -*/
#endif

        )
{
    if(flag) {
        readPort_g = rxPortRLC;
        writePort_g = txPortRRC;
        writeOAMPort_g = txPortOAM;
#if defined(ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
        /*Dual Connectivity SeNB changes +*/
        X2uRxPort_g = gtpuRxPort;
        rlcX2uRxDataPort_g = rxX2uDataPortRLC;
        strNCpy(x2uModuleHostName,gtpuIp_p,sizeof(x2uModuleHostName));
        /*Dual Connectivity SeNB changes -*/
#endif
        strNCpy(oamModuleHostName,oamIp_p,sizeof(oamModuleHostName));
        strNCpy(rrcModuleHostName,rrcIp_p,sizeof(rrcModuleHostName));
        strNCpy(l2ModuleHostName,l2Ip_p,sizeof(l2ModuleHostName));
    } else {
        readPort_g = RLC_RX_OAM_RRC_PORT;
        writePort_g = RLC_TX_RRC_PORT;
        writeOAMPort_g = RLC_TX_OAM_PORT;
#if defined(ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
        /*Dual Connectivity SeNB changes +*/
        X2uRxPort_g = gtpuRxPort;
        rlcX2uRxDataPort_g = rxX2uDataPortRLC;
        strNCpy(x2uModuleHostName,"127.0.0.1",sizeof("127.0.0.1"));
        /*Dual Connectivity SeNB changes -*/
#endif
        strNCpy(oamModuleHostName, "127.0.0.1",sizeof("127.0.0.1"));
        strNCpy(rrcModuleHostName, "127.0.0.1",sizeof("127.0.0.1"));
        strNCpy(l2ModuleHostName,"127.0.0.1",sizeof("127.0.0.1"));
    }
}

extern ULong32  pdcpAdapterExecutionSchedulerThId_g ;
#ifdef LTE_EMBMS_SUPPORTED
extern ULong32  syncAdapterExecutionSchedulerThId_g ;
#endif
/* SPR 20199 fix start*/
/* SPR 20199 fix start*/
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
/* SPR 21804 Fix Start */ 
#if defined( LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
extern ULong32  dlTgExecutionSchedulerThId_g;
#endif

/****************************************************************************
 * Function Name  : initLayer2
 * Inputs         : None
 * Outputs        : Initialize registered Layer2 modules
 * Returns        : Success if Layer2 initialized.
 * Variables      :
 * Description    : This API initialize the Layer2 modules which are regsitered
 *                  to it.
 ****************************************************************************/
#ifdef DL_DATA_SEPARATION 
SInt32 ev_fd_g[MAX_NUM_CELL];
SInt32 ev_fd_tx_g[MAX_NUM_CELL];
/*SPR 9399 Changes Start */
#ifndef DL_CONFIG_DATA_SYNC_DISABLED
SInt32 ev_fd_tx_Control_g[MAX_NUM_CELL];
#endif
#endif
#ifdef LTE_EMBMS_SUPPORTED
SInt32 ev_fd_sync_trigger_g;
#endif
/*SPR 9399 Changes End */

/** CA Phase2 Changes Start **/
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))

#define MAX_BETWEEN_FOUR(a, b, c, d)  (MAX_BETWEEN_TWO(MAX_BETWEEN_TWO(a, b),MAX_BETWEEN_TWO(c, d)))



/******** By Default EVENT FDs are used for sync threads. Mutex & semaphores are 
  implemented but not fully tested. **********/
#ifdef SEM_SYNC
SEM_T ev_fd_strategy_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SEM_T ev_fd_pdsch_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SEM_T ev_fd_mux_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SEM_T ev_fd_encoder_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
#elif MUTEX_SYNC
LTE_SEM ev_fd_strategy_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
LTE_SEM ev_fd_pdsch_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
LTE_SEM ev_fd_mux_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
LTE_SEM ev_fd_encoder_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
#else
SInt32 ev_fd_sfi_g[MAX_NUM_CELL];
SInt32 ev_fd_strategy_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SInt32 ev_fd_pdsch_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SInt32 ev_fd_mux_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SInt32 ev_fd_encoder_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
/* + SPR 13643 Changes */
SInt32 highestEventFD_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
/* - SPR 13643 Changes */
#endif
/* + Critical section changes */
LTE_SEM cellSyncSem_g;
extern CellELSyncTrackerState cellElSyncTracker[MAX_NUM_CELL];
/* - Critical section changes */
#endif
/** CA Phase2 Changes End **/

/* SPR 8712 changes */
#if ( defined(DL_DATA_SEPARATION) && defined(DL_UL_SPLIT))    
#ifdef FDD_CONFIG
SInt32 ev_fd_pusch_g;
#endif
#endif
/* SPR 8712 changes */

UInt8 initLayer2(void)
{
    /* CA Changes start */
    InternalCellIndex cellIndex = 0;
    /* CA Changes end */
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
    UInt8 fdContainerIndex = 0;
#endif

    /* Initialize Layer 2 */
    /* SPR 1998 changes start */
    /* CA Changes start */
#ifdef DL_DATA_SEPARATION 
    for (cellIndex = 0 ; cellIndex < MAX_NUM_CELL; cellIndex ++)
    {
        ev_fd_g[cellIndex]=eventfd_initialize_wrapper(0, 0);
        ev_fd_tx_g[cellIndex]=eventfd_initialize_wrapper(0, 0);
        /*SPR 9399 Changes Start */
#ifndef DL_CONFIG_DATA_SYNC_DISABLED
        ev_fd_tx_Control_g[cellIndex] =eventfd_initialize_wrapper(0, 0);
#endif
        /*SPR 9399 Changes End */
        if (ev_fd_g[cellIndex]==-1)
            ltePanic("");
        if (ev_fd_tx_g[cellIndex]==-1)
            ltePanic("");
        /*SPR 9399 Changes Start */
#ifndef DL_CONFIG_DATA_SYNC_DISABLED
        if (ev_fd_tx_Control_g[cellIndex]==-1)
            ltePanic("");
#endif
        /*SPR 9399 Changes End */
    }
#endif
    /* CA Changes end */
    /** CA Phase2 Changes Start **/
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
    for (cellIndex = 0 ; cellIndex < MAX_NUM_CELL; cellIndex ++)
    {
        for (fdContainerIndex = 0 ; fdContainerIndex < MAX_NUM_FD_CONTAINER; fdContainerIndex++)
        {
            /******** By Default EVENT FDs are used for sync threads. Mutex & semaphores are 
              implemented but not fully tested. **********/
#ifdef SEM_SYNC
            semInitDefault(&ev_fd_strategy_g[cellIndex][fdContainerIndex],0);
            semInitDefault(&ev_fd_pdsch_g[cellIndex][fdContainerIndex],0);
            semInitDefault(&ev_fd_mux_g[cellIndex][fdContainerIndex],0);
            semInitDefault(&ev_fd_encoder_g[cellIndex][fdContainerIndex],0);
#elif MUTEX_SYNC
            semInit(&ev_fd_strategy_g[cellIndex][fdContainerIndex],1);
            semInit(&ev_fd_pdsch_g[cellIndex][fdContainerIndex],1);
            semInit(&ev_fd_mux_g[cellIndex][fdContainerIndex],1);
            semInit(&ev_fd_encoder_g[cellIndex][fdContainerIndex],1);
#else
            ev_fd_sfi_g[cellIndex] = eventfd_initialize_wrapper(0, 0);
            ev_fd_strategy_g[cellIndex][fdContainerIndex] = eventfd_initialize_wrapper(0, 0);
            ev_fd_pdsch_g[cellIndex][fdContainerIndex]= eventfd_initialize_wrapper(0, 0);
            ev_fd_mux_g[cellIndex][fdContainerIndex] = eventfd_initialize_wrapper(0, 0);
            ev_fd_encoder_g[cellIndex][fdContainerIndex] = eventfd_initialize_wrapper(0, 0);
            if (ev_fd_strategy_g[cellIndex][fdContainerIndex] == -1)
                ltePanic("");
            if (ev_fd_pdsch_g[cellIndex][fdContainerIndex] == -1)
                ltePanic("");
            if (ev_fd_mux_g[cellIndex][fdContainerIndex] == -1)
                ltePanic("");
            if (ev_fd_encoder_g[cellIndex][fdContainerIndex] == -1)
                ltePanic("");
            /* + SPR 13643 Changes */
            highestEventFD_g[cellIndex][fdContainerIndex] = 
                MAX_BETWEEN_FOUR ( ev_fd_encoder_g[cellIndex][fdContainerIndex],
                        ev_fd_pdsch_g[cellIndex][fdContainerIndex],
                        ev_fd_mux_g[cellIndex][fdContainerIndex],
                        ev_fd_strategy_g[cellIndex][fdContainerIndex]);    
            /* - SPR 13643 Changes */
#endif
        }

        /* + Critical section changes */
        cellElSyncTracker[cellIndex] = INVALID_SYNC;
    }
    semInit(&cellSyncSem_g,1); 
    /* - Critical section changes */
#endif
    enableDebugTraceback();

    cellIndexMac_g = 0; 
    /** CA Phase2 Changes End **/

    /* SPR 8712 changes */
#if ( defined(DL_DATA_SEPARATION) && defined(DL_UL_SPLIT))    
#ifdef FDD_CONFIG
    ev_fd_pusch_g =eventfd_initialize_wrapper(0, 0);
    if (ev_fd_pusch_g==-1)
        ltePanic("");
#endif
#endif
    /* SPR 8712 changes */
    /* SPR 1998 changes end */
    lteLogLevel(7);
    enableLog_g = 1;
/*SPR 21727 Changes Start*/
#if   defined RLC_PDCP_IPC_INTF || defined MSPD_WATCH_DOG
/*SPR 21727 Changes End*/
    UInt32 retTh = 0;
#endif
    /*SPR 16855 +-*/
    UInt32 maxThdCount = 0;
    /*SPR 16855 +-*/
    /* SPR 4558 Fix Start */
    initUeIdxMap();
    /* SPR 4558 Fix End */
    layer2CommonInitCellIndexMapping();
    initCurrentTick();

    /* Start 128 UE: Memory Changes */
    /*
     ** Created Default memory pool which is required
     ** to initialize layer 2 as memory pool for MAC, RLC
     ** and PDCP will be created after getting INIT request
     ** from OAM.
     */
    CREATE_DFLT_MEMORY_POOL_FOR_INIT

        /* Initialize sockets of MAC layer to receive messages from RRC/OAM/RRM 
         * interface */
    initRRCAndOAMInterface();

    /* CA Changes start */
    initMacLayer();
    /* CA Changes end */

    if(RLC_SUCCESS != initRLCLayer()) 
    {
        //  cleanupMAC();
        ltePanic("Failed to init RLC Layer\n");
    }

    /*SPR 16855 +-*/
    initPDCPLayer();
    /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
    /*SPR 21188 -*/
    initPacketRelay();
#endif
    /*SPR 16855 +-*/

#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    circQInit( &rcvdRRCOAMMsg_g, MAX_NODES_SPLIT );
#endif
    /* CA Changes start */
    createMemPool(sizeof(LTE_SQUEUE), MAX_NUM_CELL);
    for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        GET_MEM_FROM_POOL(LTE_SQUEUE, recvDataFromPhyQ_gp[cellIndex],sizeof(LTE_SQUEUE), PNULL);
        sQueueInit(recvDataFromPhyQ_gp[cellIndex]);
    }
    /* CA Changes end */
/* SPR 18036 fix start*/ 
#ifdef LTE_EMBMS_SUPPORTED
        ev_fd_sync_trigger_g = eventfd_initialize_wrapper(0, 0);
        if (ev_fd_sync_trigger_g ==-1)
            ltePanic("");
#endif
/* SPR 18036 fix end*/ 
    /*SPR 21366 +*/
#if  defined(PDCP_GTPU_INTF)
    /* Create buffers for 2K mempool as initPdcpAdapter will not be called
     * when PR-GTP is running as separate process */
#define BUFFERSIZE 2000
    createMemPool(BUFFERSIZE, 60000); /*SPR 21635 FIX*/
    /* Initialize FD to receive control messages from L1 */
    /*SPR 21366 -*/
#endif
    /*SPR 16855 +-*/
    /* SET RLIMIT of Message Queues */
    setRlimitOfMsgQueue();

    /* Initialize FD to receive messages from HP thread */
    recvMsgFrmHPThdFd_g = initMsgQFdFrmHpThread();
    if ( 0 > recvMsgFrmHPThdFd_g )
    {
        ltePanic("mq_open failed for Periodic report message queue"); 
    }
    /* SPR_10206_Fix Start */
    /* 
     * rlcRrcCnfInputQId_g will be accessed by HP Thread
     * rlcRrcCnfOutputQId_g will be accessed by 
     * layer 2 control Thread
     */
     /*Need to change the code accordingly for QC. Currently commenting the code to bypass as msgpool
     is not created */
    initRlcRrcCnfQueue(&rlcRrcCnfInputQId_g);
    initRlcRrcCnfQueue(&rlcRrcCnfOutputQId_g);
    /* SPR_10206_Fix End */

    /* Initialize queue for pending messages and message handler for RRC*/
    for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        rrcOamMsghandler[cellIndex].currentState = MAC_CTRL_IDLE_ST ;
        INIT_RRC_OAM_LP_PENDING_NODE_Q( &rrcOamPendingQ_g[cellIndex] );
        for (maxThdCount = 0; maxThdCount < MAX_THD ; maxThdCount++)
        {
            /* Init High Priority Reuest Queues */
            if(RACH_MGRQ != maxThdCount)
            {
                INIT_RRC_OAM_LPTOHP_THDH_PRIO_Q( &rrcOamLPtoHPThdHPrioQ_g[cellIndex][maxThdCount] );
            }
            /* Init Response Queues */
            INIT_RRC_OAM_HPTOLP_THDH_PRIO_Q( &rrcOamHPtoLPThdHPrioQ_g[cellIndex][maxThdCount] );
        }
    }

    unlinkMsgQueueForICC("/RlcRrcCnfQueue");
    /*SPR 16855 +-*/
#ifdef TDD_CONFIG
    initCountofFnSets();
#endif
    /** El Sync Changes Start **/
#ifdef FDD_CONFIG
#if (!defined(DL_UL_SPLIT) && !defined(DL_DATA_SEPARATION))
    initExecutionFlowElGroups();
/* SPR 21660 changes start_vishal */
/* SPR 21660 changes end_vishal */

#endif
#endif
    /** El Sync Changes End **/
    /*SPR_7217_changes_start*/
#ifdef RLC_STANDALONE_SETUP
    semInit(&rlcpdcpThreadSem_g, 0);
    /*SPR_7217_changes_end*/
#endif

    createPDCPAdapterThread();
#ifdef LTE_EMBMS_SUPPORTED
    createSYNCAdapterThread();
#endif 

/* SPR 20199 fix start*/
/* SPR 20199 fix end*/
/*SPR 21727 Changes Start*/
#if  defined RLC_PDCP_IPC_INTF
/*SPR 21727 Changes End*/
    retTh = threadCreate(&rlcULExecutionSchedulerThId_g, THREAD_CREATE_DETACHED,
            THREAD_SCHED_DEFAULT, 0,
            THREAD_INHERIT_SCHED_DEFAULT,
            THREAD_SCOPE_SYSTEM,
            rlcULExecutionScheduler, PNULL);
    if (retTh != 0) {
        ltePanic(" Unable to initialise the Layer2.\n");
        return OAM_FAILURE;
    }
#endif
    return OAM_SUCCESS;
}


#ifdef UTFWK_SIMULATION
/****************************************************************************
 ** Function Name  : phyTimerThread
 ** Inputs         :
 ** Outputs        :
 ** Returns        :
 ** Description    : This is the starting point for the timer thread.
 **                  It will increment the SFN, SF and global tick once
 **                  register with perr MAC
 **
 *****************************************************************************/
void * phyTimerThread(void * arg)
{
    TimeSpec t1, t2;
    UInt32 tempTickValue = 1*1000000;//tickValue_g;
    SInt8  timer_ret = 0;

    THREAD_TO_BIND_SET_CORE_NO( PhyTimerThread_g , 0);

    threadSetRtPriority(SCHED_FIFO, 90);

    while(1)
    {
        timer_ret=0;

        do{
            if(timer_ret==0)
            {
                t1.tv_sec = 0;
                t1.tv_nsec = tempTickValue;
            }
            else
            {
                t1.tv_nsec=t2.tv_nsec;
                t1.tv_sec=t2.tv_sec;
            }

            timer_ret = nanoSleep(&t1, &t2);

        }while(timer_ret<0);

        semPost(&count_mutex);//pthread_mutex_lock( &count_mutex );

        condSignal(&condition_var);//pthread_cond_signal( &condition_var );
    }
}
#endif
/* SPR 2242 changes start */
#ifdef MALLOC_HOOK
static void lteInitMemHook(void);
static void *lteMallocHook(size_t size, const void *caller);

static void *(*oldMallocHook)(size_t, const void *);

void (*__malloc_initialize_hook) (void) = lteInitMemHook;

static void lteInitMemHook(void)
{
    oldMallocHook = __malloc_hook;
    __malloc_hook = lteMallocHook;
}

static void *lteMallocHook(size_t size, const void *caller)
{
    void *result;
    int   nptrs = 0;
#define MAX_BT_POINTERS 100
    void *buffer[MAX_BT_POINTERS];
    int   iterator;

    /* Restore all old hooks */
    __malloc_hook = oldMallocHook;

    /* Call recursively */
    result = malloc_wrapper(size);

    /* Save underlying hooks */
    oldMallocHook = __malloc_hook;

    /* */
    fprintf(stderr, "malloc %d bytes ");

    /* get the backtrace */
    nptrs = backtrace(buffer, MAX_BT_POINTERS);

    fprintf(stderr, "BT :");
    for (iterator = 0; iterator < nptrs; iterator++)
    {
        fprintf(stderr, " 0x%x", buffer[iterator]);
    }
    fprintf(stderr, "\n");

    /* Now restore our hooks */
    __malloc_hook = lteMallocHook;

    return result;
}

#endif /* MALLOC_HOOK */
/* SPR 2242 changes end */


#ifdef OVERSHOOT_CORRECTION
/****************************************************************************
 * Function Name  : updateElcycleCount()
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Variables      :
 * Description    : This function is responsible to send init Indication
 to OAM on L2 Initialization
 ***************************************************************************/
/* + SPR 17439 */
void updateElCycleCount(void)
    /* - SPR 17439 */
{ 
    UInt32 maxCrtlLegs = 0;
    UInt32 maxUeSch = 0;
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    /*Calculate the cycle counts required for all DL and UL ELs*/
    for(maxUeSch =0; maxUeSch < MAX_UES_SCHEDULED_INDX ; maxUeSch++)
    {
        /*SPR 16855 +-*/
        for(maxCrtlLegs =0; maxCrtlLegs < 13; maxCrtlLegs++)
            /*SPR 16855 +-*/
        {
            reqElProcessingCyclesDL[maxUeSch][maxCrtlLegs] =
                (reqElProcessingTimeDL[maxUeSch][maxCrtlLegs] * ccInMs_g)/1000;
        }
    }
    for(maxUeSch =0; maxUeSch < MAX_UES_SCHEDULED_INDX ; maxUeSch++)
    {
        /*SPR 16855 +-*/
        for(maxCrtlLegs =0; maxCrtlLegs < 18; maxCrtlLegs++)
            /*SPR 16855 +-*/
        {
            reqElProcessingCyclesUL[maxUeSch][maxCrtlLegs] =
                (reqElProcessingTimeUL[maxUeSch][maxCrtlLegs] * ccInMs_g)/1000;
        }
    }

#else /*DL_UL_SPLIT*/
    /** CA2 Changes Start **/
    UInt8 index = 0;
    for(index = 0; index < MAX_NUM_CELL;index++)
    {
        for(maxUeSch =0; maxUeSch < MAX_UES_SCHEDULED_INDX ; maxUeSch++)
        {
            for(maxCrtlLegs =0; maxCrtlLegs < MAX_CRITICAL_LEGS; maxCrtlLegs++)
            {
                reqElProcessingCycles[index][maxUeSch][maxCrtlLegs] =  (reqElProcessingTime[maxUeSch][maxCrtlLegs] *ccInMs_g)/1000;
            }
        }
    }
    /** CA2 Changes End **/

#endif
}
#endif



/****************************************************************************
 ** Function Name  : main
 ** Inputs         : None
 ** Outputs        : None
 ** Returns        : UInt8 - return value
 ** Variables      :
 ** Description    : This is the starting API of the Layer2
 *****************************************************************************/
int main(int argc, char *argv[])
{
    /* SPR 18021 +- */
    UInt32 ret = 0;
#ifndef FLEXRAN
    InternalCellIndex cellIndex = 0, firstCellIndex = 0;
#else
    InternalCellIndex cellIndex = 0;
#endif
#ifdef LAYER2_PHYSIM_THREAD
    sigset_t set, oset;
    sigemptyset(&set);
    sigaddset_wrapper(&set, SIGALRM);
    sigprocmask(SIG_BLOCK, &set, &oset);
#endif
    /* CA changes start */
    instanceL2_g = 0;
    /* CA changes end */
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

#ifdef LINUX_PC_TEST
	
	adapter_init_socket();
	TestInitFrame("192.168.1.120", 45499);
	TestFrameRegisterMsgHandler(adapter_hdl_msg_callback);
#endif

    /*SPR 18021 START*/
/*SPR 21653 changes start*/
    ret =  getCommandLineOptions( argc, argv);
/*SPR 21653 changes end*/

    if( !ret )
    {
        fprintf (stderr, "getCommandLineOptions Failed !!!!!\n");
        return 0;
    }
#ifndef FLEXRAN
    /*SPR 18021 END*/

    /* Logic required to be change depending upon platform and number
     * of cores availability. Now the cores are set to value incremented
     * with corresponding cell index from first cell core Ids */
    for (cellIndex = 1; cellIndex < MAX_NUM_CELL ;cellIndex++)
    {
        lteLayer2CoreInfo_g.lteLayer2DLCoreNum[cellIndex] =
            lteLayer2CoreInfo_g.lteLayer2DLCoreNum[firstCellIndex] + cellIndex;

        lteLayer2CoreInfo_g.lteLayer2ULCoreNum[cellIndex] =
            lteLayer2CoreInfo_g.lteLayer2ULCoreNum[firstCellIndex] + cellIndex;

        lteLayer2CoreInfo_g.lteLayer2EncoderCoreNum[cellIndex] =
            lteLayer2CoreInfo_g.lteLayer2EncoderCoreNum[firstCellIndex] + cellIndex;
    }
    /* setting UL RLC core num same as of DL core num of first cell */
    lteLayer2CoreInfo_g.lteLayer2ULRLCCoreNum = lteLayer2CoreInfo_g.lteLayer2DLCoreNum[firstCellIndex];
#endif
#ifdef OVERSHOOT_CORRECTION
    getCcInMs();
    updateElCycleCount();
    /*+ Dyn UEs To Be SchPerTTI +*/
    getIndexForReqElProcessingCycles();
    /*- Dyn UEs To Be SchPerTTI -*/
#endif

/* SPR 21804 Fix Start */ 
#if defined( LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
    UInt16 index = 6;
    dlTgCoreNum_g = atoi(argv[index++]);
    packetSize_g = atoi(argv[index++]);
    noOfPktVar_g = atoi(argv[index++]);
    timeInMs_g = atoi(argv[index++]);
    timeInMs_g_rx = timeInMs_g;
    putDisplayVar_g = atoi(argv[index++]);
    trafficProfileVar_g = atoi(argv[index++]);
    UInt32 ueId = atoi(argv[index++]);
    UInt32 ueStart = atoi(argv[index++]);
    ueId_rx = ueId;
    if (1 ==lteMacCheckDLTGInput(
                ueId,
                ueStart,
                argc))
    {
        return 0;
    }
#endif
#ifdef DL_UL_SPLIT_TDD
    UInt16       rxPortULThreadPhy = 0;
#endif
    UInt16 rxPortMAC=0,txPortOAM=0,txPortRRC=0, rxPortRLC=0;
    UInt16 rxPortPDCP = 0;
    UInt16 flag = 0;
    Char8 oamIP [64] = "127.0.0.1";
    Char8 rrcIP [64] = "127.0.0.1";
/*SPR 20863 FIX*/
    UInt16 rrmTxPort = 0;
    Char8 pdcpIp [64] = "127.0.0.1";
    Char8 macIp [64] = "127.0.0.1";
    Char8 rrmIp[64] = "127.0.0.1";
/*SPR 20863 FIX*/
    /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
    Char8 gtpuIp_p[64] = "127.0.0.1";
    UInt16 gtpuRxPort = 0;
    UInt16 pdcpGtpuRxPort = 0;
#ifdef ENDC_ENABLED
    /*Dual Connectivity SeNB changes +*/
    UInt16 rxX2uDataPortRlc = 0;
    /*Dual Connectivity SeNB changes -*/
#endif
#endif
    /*SPR 21188 -*/
    /* CA Changes start */

    UInt8 phyFlag[MAX_NUM_CELL] = {0};
    /*SPR 21064 Start*/
    flag = l2CommonFillPhyIpAndPortInfo(PORT_FILE_NAME,0,&phyFlag[0]); /*InstanceId Fix*/
    /*SPR 21064 End*/

    setPhyIpAndPort(&phyFlag[0]);

    for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
        phyControlRxPort_g[cellIndex] = phyPortInfo_g[cellIndex].phyControlRxPort;

        phyControlTxPort_g[cellIndex] = phyPortInfo_g[cellIndex].phyControlTxPort;
    }
    FILE *fp;
    char buf[101] = {0,};
    Char8* str_p = PNULL;

    fp = fopen(PORT_FILE_NAME,"r");
    if(fp == PNULL) {
        LTE_LOG(LOG_INFO,"Fail to open the port config file eNodeb_Configuration.cfg");
        return 0;
    }

    /*Coverity Fix 94097 +-*/
    /*SPR 20908 +*/
    
    while( fgets(buf,100,fp)){

	    trimSpace(buf,sizeof(buf));
	    if(PNULL != (str_p = isKeyPresent(buf,"L2_INSTANCE_ID")))
	    {
		    instanceL2_g = atoi(str_p);
	    }
    }
    /*SPR 20908 -*/
    /*CID 113180 Start*/
    fclose(fp);
    /*CID 113180 End*/

    /*SPR 20863 FIX*/
    flag = fillIpAndPortInfo(&rxPortMAC, &rxPortRLC, &txPortOAM, &txPortRRC,
            &rxPortPDCP,oamIP,rrcIP
			,macIp,pdcpIp,rrmIp,&rrmTxPort
         /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
            ,gtpuIp_p,&gtpuRxPort,&pdcpGtpuRxPort
#ifdef ENDC_ENABLED
            /*Dual Connectivity SeNB changes +*/
            ,&rxX2uDataPortRlc
            /*Dual Connectivity SeNB changes -*/
#endif
#endif
#ifdef DL_UL_SPLIT_TDD
            ,&rxPortULThreadPhy
#endif
            /*SPR 21188 -*/
			,PORT_FILE_NAME,0
            );
    setMacIpAndPort(flag,rxPortMAC,txPortOAM,txPortRRC,oamIP,rrcIP
		    ,macIp
		    ,rrmIp
            ,rrmTxPort
#ifdef DL_UL_SPLIT_TDD
            ,rxPortULThreadPhy
#endif
            );
    /* CA Changes end */
	LTE_LOG( LOG_INFO,"rxPortMAC (%d) rxPortRLC (%d)  \ntxPortOAM (%d) "
			"txPortRRC (%d) rxPortPDCP (%d) oamIp (%s) rrcIp(%s) l2Ip (%s)",
			rxPortMAC,rxPortRLC,txPortOAM,txPortRRC,rxPortPDCP,oamIP,rrcIP,"l2Ip");
	setRlcIpAndPort(flag,rxPortRLC,txPortOAM,txPortRRC,oamIP,rrcIP,macIp
#if defined(ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
            /*Dual Connectivity SeNB changes +*/
            ,rxX2uDataPortRlc,
            gtpuIp_p,gtpuRxPort
            /*Dual Connectivity SeNB changes -*/
#endif
		       );
	setPdcpCommPort(flag,rxPortPDCP, txPortOAM, txPortRRC,oamIP,rrcIP,pdcpIp,rrmIp, rrmTxPort
            /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
            ,gtpuIp_p, gtpuRxPort,pdcpGtpuRxPort
#endif
            /*SPR 21188 -*/
            );
    /*SPR 20863 FIX*/
/* SPR 19288 change start */
/* SPR 19288 change end */

    initStats();
    initMemPool();

    initMsgPool();
#ifdef PERF_STATS
    initPerfStats();
#endif
    /* SPR 981 fix starts*/
    PROCESS_TO_BIND_SET_CORE_NO(getpid_wrapper(),lteLayer2CoreInfo_g.lteLayer2DLCoreNum[0]);
#ifdef FLEXRAN
    prctl(PR_SET_NAME, "Control", NULL, NULL, NULL);
#endif
/* SPR 21804 Fix Start */ 
#if defined( LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
#ifndef LAYER2_PHYSIM_THREAD
    threadSetRtPriority(SCHED_FIFO, 45);
#else
    threadSetRtPriority(SCHED_FIFO, 80);
#endif
#else
    threadSetRtPriority(SCHED_FIFO, 40);
#endif
    /*SPR 16855 +-*/
#if !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)
    threadSetRtPriority(SCHED_FIFO, 15);
#endif 
    /* SPR 981 fix ends*/
/*SPR 21653 changes start*/
    initAndConfigLogFramework();
/*SPR 21653 changes end*/

    /* SPR 4422 Fix Start */
    if (thr_count_g < MAX_THREAD_ID)
    {
        threadId_g[thr_count_g] = threadSelf();
        thr_count_g++;
    }
    /* SPR 4422 Fix End */
    for (cellIndex = 0; cellIndex < MAX_NUM_CELL ;cellIndex++)
    {
        semInit(&boSem_g[cellIndex],1);
    }
    initLayer2();

/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/


    /* SPR 2238 changes start */
    /* Initializations done, now lock memory in RAM */
    mlockall_wrapper(MCL_CURRENT|MCL_FUTURE);
    /* SPR 2238 changes end */
/* SPR 19288 change start */
/* SPR 19288 change end */

    /* SPR 817 changes start */
    /* As per our scenario, phyControlSockFD_g is initialize later when OAM INIT mac layer */
    //phyControlSockFD_g = 0;
    /* SPR 817 changes end */

    /* Infinite loop to receive input from STDIN and socket */
    receiveMsg();
    return 0;
}

/* SPR 19288 change start */
/* SPR 19288 change end */

ULong32  oneThdExecutionSchedulerThId_g = 0;
ULong32  rlcpdcpThdExecutionSchedulerThId_g = 0;
extern void* oneThreadExecutionScheduler(void * param);
extern UInt16 pdcpContextIsInitialized(void);
/* CA Changes start */
extern MAC_UE_OPPOR_IND_CIRC_TYPE* macUeOpporIndCircQ_gp[MAX_NUM_CELL];
/* CA Changes end */
/* + SPR 17439 */
extern UInt32 handleTxOppurtunity(void);
extern UInt32 handleDataFromPdcp(void);
extern UInt32 pdcpElTxProcessSrbMessage(void);
extern UInt32 pdcpElTxProcessDrbMessage(void);
extern void rohcProcessTimers(void);
extern UInt32 handleExpiredTimer(void);
/* - SPR 17439 */
extern UInt32 handleDataFromMac(UInt32 count);
/** CA 2 Core Changes Start **/
extern MAC_UE_DATA_IND_TYPE macUeDataIndQ_g[MAX_NUM_CELL];
/** CA 2 Core Changes End **/
extern LTE_QUEUE rlcUeDataReqQ_g;
/* + SPR 17439 */
extern void pdcpElRxProcessMessage(void);
extern void timerUpdateTickCheckForTmrEx(void);
/* - SPR 17439 */
/* RLC Queues for Pending Deletion and Reestablishment of Entities */
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
/* SPR 3805 Fix Start */
extern LTE_CIRCQUEUE rlcRxUePendingDelQ_g;
extern LTE_CIRCQUEUE rlcTxUePendingDelQ_g;
/* SPR 3805 Fix End */
extern LTE_SQUEUE rlcRxPendingDelQ_g;
extern LTE_SQUEUE rlcRxReEstabQ_g;
extern LTE_SQUEUE rlcTxPendingDelQ_g;
extern LTE_SQUEUE rlcTxReEstabQ_g;
#else
extern LTE_CIRCQUEUE rlcRxPendingDelQ_g;
extern LTE_CIRCQUEUE rlcRxReEstabQ_g;
#endif
/* Till Here */
extern UInt32 handleComChDataFromMac(void);
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
extern UInt32 rlcHdlrPenDelQ(LTE_SQUEUE *pendDelQ_p);
/* SPR 3805 Fix Start */
extern UInt32 rlcHdlrPenUeDelQ(LTE_CIRCQUEUE *rlcUePendingDelQ);
/* SPR 3805 Fix End */
extern UInt32 rlcHdlrReEstabQ(LTE_SQUEUE *reEstabQ_p);
#endif
extern UInt32 handleComChTxOppurtunity(void);
extern void pdcpElTxProcessDeliveryInd(void);
extern void pdcpElTxProcessDiscardTimers(void);



#define COUNT_VALUE 50
void destroyExecutionLegs(void)
{
    UInt32 retTh = 0;

    if(oneThdExecutionSchedulerThId_g > 0) {
        LTE_LOG(LOG_DETAIL,PNULL,"oneThdExecutionSchedulerThId_g thread exists, cancelling it\n");
        retTh = threadCancel(oneThdExecutionSchedulerThId_g);
        oneThdExecutionSchedulerThId_g = 0;
        LTE_LOG(LOG_DETAIL,PNULL,"oneThdExecutionSchedulerThId_g thread cancelled, retTh=%d\n", retTh);
    }
    if( 0 == retTh && rlcpdcpThdExecutionSchedulerThId_g > 0) 
    {
        LTE_LOG(LOG_DETAIL,PNULL,"rlcpdcpThdExecutionSchedulerThId_g thread exists, cancelling it\n");
        retTh = threadCancel(rlcpdcpThdExecutionSchedulerThId_g);
        rlcpdcpThdExecutionSchedulerThId_g = 0;
        LTE_LOG(LOG_DETAIL,PNULL,"rlcpdcpThdExecutionSchedulerThId_g thread cancelled, retTh=%d\n", retTh);
    }
    if (retTh != 0) {
        ltePanic(" Unable to deinitialise the oneThreadExecutionScheduler.\n");
    }

    return;
}
/* coverity-530 CID-24578*/
#ifdef RLC_STANDALONE_SETUP
/* coverity-530 CID-24578*/
void createExecutionLegs(void)
{
    UInt32 retTh = 0;
    retTh = threadCreate(&rlcpdcpThdExecutionSchedulerThId_g, THREAD_CREATE_DETACHED,
            THREAD_SCHED_FIFO,0,
            THREAD_EXPLICIT_SCHED,
            THREAD_SCOPE_SYSTEM, rlcpdcpThreadExecutionScheduler, PNULL);
    if (retTh != 0) {
        ltePanic(" Unable to initialise the oneThreadExecutionScheduler.\n");
    }
    THREAD_TO_BIND_SET_CORE_NO(rlcpdcpThdExecutionSchedulerThId_g,2);
}
#endif
/****************************************************************************
 * Function Name  : void oneThreadExecutionScheduler_cleanup_handler(void *arg)
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 *Description     : This function will be called when
 oneThreadExecutionScheduler thread is
 cancelled.
 ****************************************************************************/
void rlcpdcpThreadExecutionScheduler_cleanup_handler(void *arg)
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(arg)
    /* +- SPR 17777 */
    LTE_LOG(LOG_INFO,PNULL,"In rlcpdcpThreadExecutionSchedulerThread - cleanup handler\n");
}

/* coverity-530 CID-24578*/
#ifdef RLC_STANDALONE_SETUP
/* coverity-530 CID-24578*/
/****************************************************************************
 * Function Name  : rlcpdcpThreadExecutionScheduler
 * Inputs         : void * - Arguements
 * Outputs        : None
 * Returns        : void *
 * Variables      :
 * Description    : This function is initialization function for RLC PDCP 
 *                  thread.
 ****************************************************************************/
void* rlcpdcpThreadExecutionScheduler(void * param)
{
    UInt32 sQcount = 0; 

    threadSetCancelState(THREAD_CANCEL_ENABLE, PNULL);
    threadSetCancelType(THREAD_CANCEL_ASYNCHRONOUS, PNULL);
    threadCleanupPush(&rlcpdcpThreadExecutionScheduler_cleanup_handler, PNULL);
    /************************thread bind code start***************************/
    unsigned long new_mask = 0x01;
    pid_t p = 0;
    THREAD_TO_BIND_GET_CORE_NO(p);
    THREAD_TO_BIND_SET_CORE_NO(p,new_mask);
    /************************thread bind code end*****************************/
    while (1) {
        static UInt32 count = 0; 
        while(RLC_FAILURE == rlcLayerInitialized_g ||
                !pdcpContextIsInitialized()) {
            continue;
        }    
        LTE_LOG(LOG_INFO,PNULL,"Start Execution of RLC and PDCP\n");
        count = 0;
        while(1) {
            do{
                /* Handle Tx Opportunities as many as there in Queue at RLC Layer*/
                LTE_LOG(LOG_WARNING,rlcModuleLogDetail_p,"1pdcpElTxProcessMessage %d\n",sQcount);
                pdcpElTxProcessDrbMessage();
                pdcpElTxProcessSrbMessage();
                /*Handle Tx Opp*/
                /* CA Changes start */
                sQcount = COUNT_MAC_UE_OPPOR_IND_CIRC_Q(cellIndex);
                /* CA Changes end */
                if(sQcount)
                    LTE_LOG(LOG_WARNING,rlcModuleLogDetail_p,"3handleTxOppurtunity %d\n",sQcount);

                while(sQcount) {
                    rlcHandleTxOppurtunity();
                    sQcount--;
                }

                /* Handle all data from MAC present in RLC Rx Queue and send it 
                   to PDCP User */
                sQcount = COUNT_MAC_UE_DATA_IND_Q(); 
                if(sQcount) {
                    LTE_LOG(LOG_INFO,PNULL,"--- RLC---sqcount = %llu\n",sQcount);
                    rlcHandleDataFromMac(sQcount);
                }
                pdcpElRxProcessMessage();
            }while(rlcLoop_g == TRUE);  

            /* Process ROHC Timers at PDCP Layer*/
            rohcProcessTimers();
            /*Handle Tx Opp*/
            /* CA Changes start */
            sQcount = COUNT_MAC_UE_OPPOR_IND_CIRC_Q(cellIndex);
            /* CA Changes end */

            if(sQcount)
                LTE_LOG(LOG_WARNING,rlcModuleLogDetail_p,"3handleTxOppurtunity %d\n",sQcount);

            while(sQcount) {
                rlcHandleTxOppurtunity();
                sQcount--;
            }

            /*Process Rx Expired Timers at RLC Layer */
            rlcCheckTimerExpiry(); 
            /* Handle all data from MAC present in RLC Rx Queue and send it 
               to PDCP User */
            sQcount = COUNT_MAC_UE_DATA_IND_Q(); 
            if(sQcount) {

                rlcHandleDataFromMac(sQcount);
            }
            pdcpElRxProcessMessage();
            count++;
            /* Handle the remaining legs of PDCP and RLC here */
            if(COUNT_VALUE == count) {
                /* Handle Srb Data Request */
                pdcpElTxProcessDeliveryInd();
                pdcpElTxProcessDiscardTimers();
                /*SPR 16855 Fix Start*/
#ifdef UE_SIM_TESTING
                rlcHdlrPenDelQ(&rlcTxPendingDelQ_g);
                /*SPR 3805 Fix Start */
                rlcHdlrPenUeDelQ(&rlcTxUePendingDelQ_g);
                /*SPR 3805 Fix End */
                rlcHdlrReEstabQ(&rlcTxReEstabQ_g);
                rlcHdlrPenDelQ(&rlcRxPendingDelQ_g);
                /*SPR 3805 Fix Start */
                rlcHdlrPenUeDelQ(&rlcRxUePendingDelQ_g);
                /*SPR 3805 Fix End */
                rlcHdlrReEstabQ(&rlcRxReEstabQ_g);
#endif
                /*SPR 16855 Fix End*/
                count = 0;
            }
            if(RLC_FAILURE == rlcLayerInitialized_g ||!pdcpContextIsInitialized()) {
                break;
            }
        }
    }
    threadCleanupPop(1);
    return PNULL;
}
#endif
/****************************************************************************
 * Function Name  : l2InitIndication
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Variables      :
 * Description    : This function is responsible to send init Indication
 to OAM on L2 Initialization
 ****************************************************************************/
void l2InitIndication()
{
#ifdef UTFWK_SIMULATION
    macInitInd_g = TRUE;
    initIndTimer_g = 0;
    rlcInitInd_g = TRUE;
    pdcpInitInd_g = TRUE;
    return;
#endif

    /* COVERITY 10265 */
    UInt8 initIndBuff[MAX_INIT_BUFF_SIZE] = {0};
    /* COVERITY 10265 */

    UInt16 currentLen = 0;
    /* Send the init indication to MAC */
    sendMacInitIndication( instanceL2_g, initIndBuff
            , &currentLen );
    if( 0 != currentLen )
    {
        /*SPR 20908 +*/
        /*Rel_523_Coverity_10005 Fix start*/
        if(MAC_SEND_ERROR == sendL2InitIndToOAM( initIndBuff, currentLen,
                    rxSockFD_g))
        {
            LOG_MAC_ERROR( MAC_OAM_INF, "Failed to send init ind to OAM");
            ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        }
        /*SPR 20908 -*/
        /*Rel_523_Coverity_10005 Fix End*/
    }

    if(FALSE == rlcInitInd_g)
    {
        currentLen = 0;
        prepareRlcInitIndication( initIndBuff
                , &currentLen, instanceL2_g );


        /* Trigger RLC Init ind to OAM  */
        /*SPR 20908 +*/
        if(MAC_SEND_ERROR == sendL2InitIndToOAM( initIndBuff, currentLen,
                    readSockFD_g))
        {
            LOG_RLC_ERROR( RLC_OAM, "Unable to send the message to OAM");
            ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        }
        /*SPR 20908 -*/
    }
    if(FALSE == pdcpInitInd_g)
    {
        pdcpTrigInitInd( instanceL2_g );
    }
    /*SPR 17585 +*/
    /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
    /*SPR 21188 -*/
    if( FALSE == s1UInitInd_g )
    {
        s1UInitIndication();
    }
#endif
    if( (CELL_INIT < cellSpecificParams_g.\
                cellConfigAndInitParams_p[0]->cellState) && TRUE == rlcInitInd_g
            && TRUE == pdcpInitInd_g 
            /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
            /*SPR 21188 -*/
            && TRUE == s1UInitInd_g
#endif
      )
        /*SPR 17585 -*/
    {
        initIndTimer_g = 0;
    }   
}

/***************************************************************
 * Function Name  : stopOtherThread()
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : This function is responsible to stop other thread which is 
 *                  running
 ****************************************************************************/
/* + SPR 17439 */
void stopOtherThread (void)
	/* - SPR 17439 */
{
    stopAllThread(MAX_THREAD_ID,threadId_g); 
}

/* CA Changes start */
/***************************************************************
 * Function Name  : cellDummyExecutionScheduler
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function is responsible for generating triggers for master cell
 *                  and cleaning up the triggers generated by master cell 
 * Note: master cell is the one in which Combined DL Strategy is running                 
 ****************************************************************************/
/* + SPR 17439 */
void cellDummyExecutionScheduler(void)
	/* - SPR 17439 */
{
	/*1. send HARQ indication to the master cell
	  2. Cleanup DL Strategy -->PDCCH trigger
	  3. Send PDSCH trigger to MUX  
	  4. Cleanup PDSCH trigger to MUX  
	  5. Send RLC trigger to encoder
	  6. Cleanup RLC trigger to encoder*/
}

/***************************************************************************
 * Function Name  : createPDCPAdapterThread
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function will create PDCP adapter thread.
 ****************************************************************************/
/* + SPR 17439 */
void createPDCPAdapterThread(void)
	/* - SPR 17439 */
{
	SInt32 retTh= 0;
#ifndef UTFWK_SIMULATION
	retTh = threadCreate(&pdcpAdapterExecutionSchedulerThId_g, THREAD_CREATE_DETACHED,
			THREAD_SCHED_DEFAULT, 0,
			THREAD_INHERIT_SCHED_DEFAULT,
			THREAD_SCOPE_SYSTEM,
			pdcpAdapterThreadExecutionSchedular, PNULL);
	if (retTh != 0) {
		ltePanic(" Unable to create pdcpAdapterThreadExecutionSchedular.\n");
	}
#endif
}


/* CA Changes end */
#ifdef LTE_EMBMS_SUPPORTED
/***************************************************************************
 * Function Name  : createSYNCAdapterThread
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function will create SYNC adapter thread.
 ****************************************************************************/

void createSYNCAdapterThread(void)
{
    SInt32 retTh= 0;
#ifndef UTFWK_SIMULATION
    retTh = threadCreate(&syncAdapterExecutionSchedulerThId_g, THREAD_CREATE_DETACHED,
            THREAD_SCHED_DEFAULT, 0,
            THREAD_INHERIT_SCHED_DEFAULT,
            THREAD_SCOPE_SYSTEM,
            syncAdapterThreadExecutionSchedular, PNULL);
    if (retTh != 0) {
        ltePanic(" Unable to create syncAdapterThreadExecutionSchedular.\n");
    }
#endif
}
#endif

/* + SPR 13822 Changes */
/***************************************************************************
 * Function Name  : putEntryInPhyDataQFromLPThread 
 * Inputs         : cellIndex
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function handle data from Phy untill HP thread is 
 *                  waiting to start execution.
 ****************************************************************************/
void putEntryInPhyDataQFromLPThread(InternalCellIndex cellIndex)
{
    ReceivePhyDataQueueNode *recvPhyDataQueueNode_p = PNULL;

    GET_MEM_FROM_POOL(ReceivePhyDataQueueNode,recvPhyDataQueueNode_p,
            sizeof(ReceivePhyDataQueueNode), PNULL);

    if(PNULL != recvPhyDataQueueNode_p)
    {
        /*Coverity 86641 Fix Start*/
        memSet(recvPhyDataQueueNode_p, 0,sizeof(ReceivePhyDataQueueNode));
        /*Coverity 86641 Fix End*/
        recvPhyDataQueueNode_p->recvDataBuffer_p
            = (UInt8 *)receiveDataFromPhy(
                    &(recvPhyDataQueueNode_p->byteRead),
                    cellIndex);

        if( 0 >= recvPhyDataQueueNode_p->byteRead )
        {
            if(recvPhyDataQueueNode_p->recvDataBuffer_p)
            {
                msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
                recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
            }
            freeMemPool(recvPhyDataQueueNode_p);
            recvPhyDataQueueNode_p = PNULL;
        }
        if(pushNode(recvDataFromPhyQ_gp[cellIndex], 
                    &recvPhyDataQueueNode_p->nodeAnchor))
        {
            if(recvPhyDataQueueNode_p->recvDataBuffer_p)
            {
                msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
                recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
            }
            freeMemPool(recvPhyDataQueueNode_p);

        }
    }
}
/* - SPR 13822 Changes */

