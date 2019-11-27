 /***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: phySimulatorApiHandler.c,v 1.10.6.2.4.2.2.2 2010/09/29 06:30:30 gur04640 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the Interface API between the 
 *                     MAC and Phy Simulator Layer in a LTE system. Also 
 *                     contain the starting point of execution.
 *
 ****************************************************************************
 *
 * Revision Details :
 * $Log: phySimulatorApiHandler.c,v $
 * Revision 1.10.6.2.4.2.2.2  2010/09/29 06:30:30  gur04640
 * Change priority of timer thread to 90 and policy to SCHED_FIFO
 *
 * Revision 1.10.6.2.4.2.2.1  2010/09/21 15:53:37  gur20491
 * FAPI changes
 *
 * Revision 1.10.6.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.2  2010/07/01 12:07:10  gur22077
 * commented the DCI FORMAT 0 print
 *
 * Revision 1.1.1.1  2010/06/01 07:49:19  cm_netlogic
 * code import
 *
 * Revision 1.10  2010/03/15 10:24:51  gur20439
 * avoid zero Buffer Copy changes
 *
 * Revision 1.9  2010/03/10 06:22:01  gur20439
 * seg fault bug fix
 *
 * Revision 1.8  2010/03/09 07:15:54  gur20439
 * core binding changes
 *
 * Revision 1.7  2010/03/02 12:05:16  gur15697
 * UL fix
 *
 * Revision 1.6  2010/02/26 10:24:41  gur15697
 * periodic BSR
 *
 * Revision 1.5  2010/02/18 09:51:23  gur18550
 * Core number of UL_RECEIVER_THREAD_CORE_NUM Changed
 *
 * Revision 1.4  2010/02/18 08:08:04  gur15697
 * printf statements removed
 *
 * Revision 1.3  2010/02/16 07:37:32  gur15697
 * ul Dataprocess thread removed
 *
 * Revision 1.2  2010/02/13 10:48:46  gur15697
 * changes for UL flow Stability
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.103  2009/12/31 08:57:10  gur19413
 * removed warnings
 *
 * Revision 1.102  2009/12/31 08:45:40  gur19413
 * commented the  macLayerAddr_g.sin_port = htons(UPLK_PORT);
 *
 * Revision 1.101  2009/12/31 04:40:30  gur15697
 * pselect() modified to not to wait on fd 0
 *
 * Revision 1.100  2009/12/17 07:14:43  gur11912
 * added TICK_IN_MS
 *
 * Revision 1.99  2009/11/25 09:56:54  gur11912
 * tick validation applied in case of DL Data
 *
 * Revision 1.98  2009/11/15 15:00:29  gur11912
 * bzero used with random Access
 *
 * Revision 1.97  2009/11/14 08:22:42  gur11912
 * THREAD_BIND_ENABLE removed
 *
 * Revision 1.96  2009/11/12 13:34:26  gur19413
 * added UT_TESTING Flag
 *
 * Revision 1.95  2009/11/12 12:28:22  gur19413
 * added UT_TESTING FLAG
 *
 * Revision 1.94  2009/11/11 08:15:09  gur19413
 * changes in printf statement
 *
 * Revision 1.93  2009/11/11 07:42:00  gur19413
 * added UT_TESTING Flag
 *
 * Revision 1.92  2009/11/09 13:01:26  gur11912
 * modified for UL NACK cases
 *
 * Revision 1.91  2009/11/01 12:49:36  gur18550
 * totalNumParameter is changed to 2 from 3
 *
 * Revision 1.90  2009/11/01 06:33:25  gur18550
 * Updated the field of thread mask
 *
 * Revision 1.89  2009/10/30 19:23:11  gur19413
 * removed warnings
 *
 * Revision 1.88  2009/10/30 07:17:36  gur19413
 * Added Validation in RA Procedure
 *
 * Revision 1.87  2009/10/27 08:27:11  gur19413
 * wireshark related changes
 *
 * Revision 1.86  2009/10/27 07:13:03  gur19413
 * reverted RAW Socket changes
 *
 * Revision 1.85  2009/10/27 05:23:10  gur19413
 * socket related changes
 *
 * Revision 1.84  2009/10/26 13:35:18  gur11912
 * removed warnings
 *
 * Revision 1.83  2009/10/26 12:48:11  gur11912
 * Modified for RAW Sockets
 *
 * Revision 1.82  2009/10/26 10:17:57  gur19413
 * modified socket functionality
 *
 * Revision 1.81  2009/10/21 07:03:51  gur11912
 * removed warnings
 *
 * Revision 1.80  2009/10/21 05:48:00  gur19413
 * bug fixed and optimized for UE addition
 *
 * Revision 1.79  2009/10/21 05:04:47  gur19413
 * Socket Functionality Modified
 *
 * Revision 1.78  2009/10/14 10:10:29  gur11912
 * checks moved under UT_TESTING
 *
 * Revision 1.77  2009/10/05 08:10:53  gur11912
 * optimization changes
 *
 * Revision 1.76  2009/09/24 20:52:50  gur19413
 * convert dos2unix format
 *
 * Revision 1.75  2009/09/24 20:50:04  gur19413
 * handling HI PDU modified
 *
 * Revision 1.74  2009/09/23 16:05:20  gur19413
 * bug fixed
 *
 * Revision 1.73  2009/09/23 05:39:35  gur19413
 * modified sendErrorInd handling
 *
 * Revision 1.72  2009/09/22 16:29:21  gur19413
 * added validation for DL Data Msg recvd tick
 *
 * Revision 1.71  2009/09/22 13:08:25  gur19413
 * removed warnings
 *
 * Revision 1.70  2009/09/22 10:21:49  gur19413
 * removed warnings
 *
 * Revision 1.69  2009/09/22 06:14:20  gur19413
 * optimised the code
 *
 * Revision 1.68  2009/09/22 05:56:54  gur19413
 * optimised the code
 *
 * Revision 1.67  2009/09/17 13:39:49  gur11912
 * modified NumPdu when popNode returns NULL
 *
 * Revision 1.66  2009/09/17 12:40:46  gur11912
 * modified for SR
 *
 * Revision 1.65  2009/09/17 11:59:42  gur11912
 * prepareSRPdu modified
 *
 * Revision 1.64  2009/09/16 11:10:56  gur19413
 * dos2unix
 *
 * Revision 1.63  2009/09/16 11:03:03  gur19413
 * optimised changes
 *
 * Revision 1.62  2009/09/03 14:26:49  gur20439
 * ltePanic put in macro LTE_MAC_UT_LOG.
 *
 * Revision 1.61  2009/09/03 10:25:31  gur20439
 * menu option text modified.
 *
 * Revision 1.60  2009/09/02 16:49:04  gur19413
 * socket interface communication bug fix.
 *
 * Revision 1.59  2009/09/02 13:36:38  gur20439
 * UL data for NACK case not going BUG fix.
 *
 * Revision 1.58  2009/09/02 09:26:02  gur20439
 * Synchronization partial fix.
 *
 * Revision 1.57  2009/09/01 16:58:26  gur20439
 * Head Null bug fix.
 *
 * Revision 1.56  2009/08/31 16:13:28  gur20439
 * SIT test case 138 bug fix.
 *
 * Revision 1.55  2009/08/31 15:17:14  gur12140
 * Support Added For RBs OverLap
 *
 * Revision 1.54  2009/08/30 21:53:03  gur19413
 * bug fixed
 *
 * Revision 1.53  2009/08/28 07:17:27  gur20439
 * tick validation was not proper.
 *
 * Revision 1.52  2009/08/21 07:17:24  gur20439
 * SIT bug fix.
 *
 * Revision 1.51  2009/08/20 08:36:25  gur20439
 * timer value wrongly checkedin into cvs.
 *
 * Revision 1.50  2009/08/20 08:35:45  gur20439
 * timer value wrongly checked in into cvs.
 * 
 *
 ***************************************************************************/ 

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#if 0
#ifdef REAL_TIME_THREAD_SCHEDULING
#endif
#endif

#include "phySimulatorApiHandler.h"
#ifdef TDD_CONFIG
#include "lteLayer2TDDMgr.h"
#ifdef REAL_TIME_THREAD_SCHEDULING
#include <sched.h>
#endif
#endif

#include <time.h>

/****************************************************************************
 Private Definitions
 ***************************************************************************/

/* SPR 609 changes start */
#ifndef FLOW_DEBUG 
/* SPR 609 changes end */
#define FLOW_DEBUG
/* SPR 609 changes start */
#endif 
/* SPR 609 changes end */
/* SPR 609 changes start */
#ifndef FLOW_DEBUG 
/* SPR 609 changes end */
      #define FLOW_DEBUG         fprintf
/* SPR 609 changes start */
#endif 
/* SPR 609 changes end */
/* this array will store the harqs of UE on per subframe basis.*/
ULHarqTimerEntityList ulHarqTimerEntityList_g[10];
unsigned char* pduDciDataPointer=PNULL;
UInt16 freeqIndex   = 0;
UInt16 oldSF,oldSFN;
UInt16 checkCurrentSFN,checkCurrentSF;
UInt16 checkackSF,checkTIdx;

extern int execSchedulerThreadCoreNum_g[MAX_NUM_CELLS];
extern UInt8 rlfDebugFlag;


//ipv6 change
extern UChar8 dlTgIP[HOSTNAME_MAX_LEN];

volatile UInt16 cellId_g[MAX_NUM_CELLS] = {0,0};
/*
**   
**   The value for core number to which
**   the following execution threads are 
**   to be binded is to be set below:
**
**   (Please note that for UE RRC thread binding
**    has to be done at the file lteLayer2RRCInterface.c 
**    in the function initRRCIntThread())
**
*/

#define UL_DISPATCHER_THREAD_CORE_NUM 6
#define UL_DATA_PROCESS_CORE_NUM  7
#define PHY_TIMER_THREAD_CORE_NUM 5 
#define UL_RECEIVER_THREAD_CORE_NUM 4

UInt32 periodicBsrFlag_g = 0;
UInt32 z_g = 1;
extern UChar8 macIPAddress_g[40];
//extern UChar8 phyIPAddress_g[40];
extern ueContextForUplink *uplinkContextInfoMap_g[MAX_UE_SUPPORTED];
extern dynamicUEContext *dynUEContextInfoMap_g[MAX_UE_SUPPORTED];
/*FAPI_ERROR_INDICATION_CHANGES_START*/
extern UInt16 phy_err_config_g[MAX_NUM_CELLS];
/*FAPI_ERROR_INDICATION_CHANGES_END*/

/*SPR 6690 fix Start */
extern void processUlUeSimSpsOccasion( UInt8 cellIndex);
/*SPR 6690 fix End */
/* + SPS_TDD_Changes */
extern UInt8 ulSfForDCI0OrHiReceived_g[MAX_UL_DL_CONFIG][MAX_SUBFRAME];
/* - SPS_TDD_Changes */
/* SSI Changes Start */
#if (defined(LTE_EMBMS_SUPPORTED) && !defined(LAYER2_DLTG_THREAD))
SInt32          tgGlueTx = -1;
#endif
/* SSI Changes End */
/******** Thread Specific Semaphore ****************/

typedef struct TlvHeaderT
{
    UInt16      transactionId;
    UInt16      sourceModuleId;
    UInt16      destinationModuleId;
    UInt16      apiId;
    UInt16      length;
} TlvHeader, *LP_TlvHeader;
LTE_SEM ulDispatcherThreadSem_g;
LTE_SEM ulProcessThreadSem_g;
extern LTE_SEM rlcPdcprunThreadSem_g;
LTE_SEM nextTickStartSem_g;
extern LTE_SQUEUE rrcMsgQ_g;
extern LTE_SQUEUE rlcRrcOamMsgQ_g;
extern LTE_SQUEUE pdcpRrcOamMsgQ_g;
extern SInt32 rxEnodebRRCSocketFD_g;
/* +CA_NA*/
LTE_SEM   phyToRlcDataSemlock_g[MAX_NUM_CELLS][MAX_NUM_CELLS];
LTE_SEM execSchedulerThreadSem_g[MAX_NUM_CELLS];
extern SInt32 txEnodebRRCSocketFD_g[MAX_NUM_CELLS];
extern LTE_SQUEUE macPhyDataQ_g[MAX_NUM_CELLS];
extern LTE_SQUEUE enodebRrcPhyMsgQ_g[MAX_NUM_CELLS];
extern ULong32 execSchedulerThread_g[MAX_NUM_CELLS];
extern UInt8 numCells_g;
extern UInt16  startRARNTI_g;
extern UInt16  endRARNTI_g;
SInt32 sockfd_g[MAX_NUM_CELLS] = {-1, -1};
SInt32 txSockFD_g[MAX_NUM_CELLS] = {-1, -1};
SockAddr phyLayerAddr_g[MAX_NUM_CELLS];
SockAddr macLayerAddr_g[MAX_NUM_CELLS];
/* -CA_NA*/
#define MAX_RES_DATASIZE                     65535
#define RLC_MODULE_ID            6
#define CLEANUP_LAYER2  0x2FFF
#define RLC_FAILURE 0
#define RLC_SUCCESS             1
#define RLC_LAYER_NOT_INITIALIZED       14
extern volatile UInt16 rlcLayerInitialized_g;
#ifdef SKIP_BYTE_ORDER_PROCESS

/*Do not process byte order in messages between modules in case when all modules run on platforms with same byte order.
It can improve performance.
Affects only on messages between modules, all other information (like Network packets in ROHC atc.) are processed
taking care of byte order*/
#define PDCP_NTOHS(data) (data)
#define PDCP_HTONS(data) (data)
#define PDCP_NTOHL(data) (data)
#define PDCP_HTONL(data) (data)

#define CONVERT_INT16_NETWORK_TO_HOST(data)
#define CONVERT_INT16_HOST_TO_NETWORK(data)

#define tlvPrepareHeaderForSending(message)

#else /*SKIP_BYTE_ORDER_PROCESS not defined*/

/*take care of byte order in messages between modules*/
#ifdef MSG_BYTE_ORDER_MSB
/*MSB (Network) BYTE ORDER in messages between modules*/
#define PDCP_NTOHS(data) ntohs(data)
#define PDCP_HTONS(data) htons(data)
#define PDCP_NTOHL(data) ntohl(data)
#define PDCP_HTONL(data) htonl(data)

#else /*MSG_BYTE_ORDER_MSB not defined*/
/*LSB (Intel) BYTE ORDER in messages between modules - default*/
#define PDCP_NTOHS(data) pdcpSwap16(ntohs(data))
#define PDCP_HTONS(data) pdcpSwap16(htons(data))
#define PDCP_NTOHL(data) pdcpSwap32(ntohl(data))
#define PDCP_HTONL(data) pdcpSwap32(htonl(data))
#endif /*MSG_BYTE_ORDER_MSB*/

#define CONVERT_INT16_NETWORK_TO_HOST(data) { (data) = PDCP_NTOHS(data); }
#define CONVERT_INT16_HOST_TO_NETWORK(data) { (data) = PDCP_HTONS(data); }
#define tlvPrepareHeaderForSending(message) tlvPrepareHeaderForSendingInt(message)

#endif /*SKIP_BYTE_ORDER_PROCESS*/
/************Thread ID Variable***********************/

ULong32 UlDispatcherThread_g;
ULong32 ulReceiverThread_g;
ULong32 ulDataProcessThread_g;
ULong32 PhyTimerThread_g;

/********** TICK Related Parameters ************************/
/* SPR 1167 Fix start */
volatile UInt16 SFN_g[MAX_NUM_CELLS]  = {(MAX_SFN - 1),(MAX_SFN - 1)};
volatile UInt8  SF_g[MAX_NUM_CELLS]   = {(MAX_SUBFRAME - 1) ,(MAX_SUBFRAME - 1)};
extern UInt8 startRecvFlag[MAX_NUM_CELLS];
/* SPR 1167 Fix End */


/* SPR 15909 fix start */
volatile tickType_t globalTTITickCount_g = 0xFFFFFFFFFFFFFFFF;
volatile unsigned long long printLogCnt = 0;
tickType_t tickValue_g = TICK_IN_MS * 950000;
/* SPR 15909 fix end */
//UInt32 tickValue_g = TICK_IN_MS * 1000000;
UInt32 contentionAck_g = TRUE;
/*************Configuration Variable For DL/ UL ACK,NACK and BSR *****/

UInt8 nackChoiceTB1_g;
UInt8 nackChoiceTB2_g;
UInt8 uLnackChoice_g;
UInt8 bsrChoice_g;

/* Filling the position which packet reply ack or nack */
UInt8 dwlkPacketArrTB1_g[MAX_SUBFRAME];
UInt8 dwlkPacketArrTB2_g[MAX_SUBFRAME];
UInt8 ulPacketArr_g[MAX_SUBFRAME];
UInt8 bsrPacketArr_g[MAX_SUBFRAME];

static UInt32 ULpacketCounter_s = 0;
static UInt32 ULarrIndex_s      = 0;
/* SPR 609 changes start */
//static UInt8 bsrAckPacketCounter_s;
//static UInt8 bsrArrIndex_s;
/* SPR 609 changes end */

/****************Log Related Variables******************************/

/* SPR 609 changes start */
Char8 * phyLogFileName_g =  "LTE_PHY";
/* SPR 609 changes end */
ModuleLogDetail PhyModuleLogDetail_g;

/******* Registration Related Variables *****************/

UInt32 enableMenuReg_g = 1;
volatile UInt8 macRegFlag_g[MAX_NUM_CELLS] = {REG_FAILURE,REG_FAILURE};
volatile UInt8 paramReqrecvd[MAX_NUM_CELLS] = {FALSE, FALSE};
UInt16 prevSRSFN;
/********* Socket/ Wireshark Related Information *******************/
UInt32 wiresharEnable_g = 0;
SInt32 wiresharkfd_g = -1;
/* SPR 5313 changes start */
/* +CA-SJ */
SInt32 sockfdHO_g[MAX_NUM_CELLS] = {-1, -1};
SockAddr phyLayerAddrHO_g[MAX_NUM_CELLS];
/* -CA-SJ */
/* SPR 5313 changes end */
SockAddr wiresharkAddr_g;

#ifdef DL_UL_SPLIT_TDD
SockAddr macLayerAddrULThd_g[MAX_NUM_CELLS];
#endif

/** Msq Queue UL and DL***/
SInt32 shMsgQIdDL_g;
SInt32 shMsgQIdUL_g;

/**************** RA Procedure Related Parameters  ********/

RARequest raRntiArr_g[MAX_NUM_CELLS][MAX_NUM_RA_REQUEST] = {{0}, {0}};
UInt32 msg3ToBeSend_g = TRUE;
SInt8 raReqIndex_g = -1;
SInt8 totalRaReqIndex_g = 0;
UInt32 totBytes_g = 0;
UInt32 onlyCCCHFlag_g = FALSE; 

/********************************************************/



/*Contains new and current SI message information and map table*/
extern UInt32 trafficStartFlag_g; 
//static SIInfo       siMsgInfo;



extern int ulReceiverThreadCoreNum_g;
extern int uePhyTimerThreadCoreNum_g;
extern int uldispatcherThreadCoreNum_g;


UInt8 recvDataBuffer_g[RECV_BUFF_SIZE] = {0};
//UInt8 controlAndDataFlag_g[2]={0};
//UInt8 controlRecvFlag_g = 0;
QueuePointerNode ULControlAndDataArr_g[MAX_SUBFRAME];

//Pradeep
/*RARequestGroup raReqTobesendArr_g[MAX_NUM_RA_REQUEST] = {{0}};*/

UInt16 transactionId_g;
statsGeneral statsGeneralInfo_g;

UInt8 mibToBeRecievedFlag_g;
UInt8 mibToBeRecievedPrevious_g;

UInt8 siRntiMsgFlag_g;
UInt8 sibMsgFlag_g;
UInt32 taTrigger_g;
UInt8 phrFlag_g = FALSE;
UInt32 periodicEnableFlag_g = FALSE;
LTE_SQUEUE processRecvNode_g;
PeriodicCQITable periodicCQITable[12];


LTE_SQUEUE OnlyBsrQueueNode_g[MAX_SUBFRAME];

extern UInt32 rlcUeSimLoop_g;
//Pradeep
/*CA multi-thread sync fix Start*/
extern SInt32 ev_fd_g[MAX_NUM_CELLS][MAX_NUM_CELLS];
extern SInt32 ev_sync_fd_g[MAX_NUM_CELLS][MAX_NUM_CELLS];

#ifdef LTE_EMBMS_SUPPORTED
extern UInt8 mcchPduReceived_g;
extern UInt8 rlcResetSN_g;
#endif

/*CA multi-thread sync fix End*/
/* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
UInt8 isDLScheduledForHDFddUe(UInt16 ueIndex, UInt8 recvSF, UInt8 toClear, UInt8 rxCellIndex);
void clearUeScheduleMapContext(UInt8 recvSF);
#endif
/* HD FDD Changes End */
//void powerHeadRoom();
void fillCRCInfo(ControlNodeInfo *ctlInfo_p);
UInt16 selectControlPDUChoice(UInt16 oldPduChoice, UInt16 newPduChoice);
UInt16 selectDataPDUChoice(UInt16 oldPduChoice, UInt16 newPduChoice);
inline UInt8 validateSFNandSFAndgetDelay( UInt16 recvSFN, UInt8 recvSF, UInt16 currentSFN, UInt8 currentSF );
inline static void displayMsg(UInt8 *msg_p , SInt32 bytesRead);
inline void prepareControlNode( ueContextForUplink *ueContextInfo_p, 
                                       ControlNodeInfo *ctlInfo_p
                                     );
inline void prepareDataPduNode( UInt8  *pdu_p,
                                       UInt16 pduLen,
                                       UInt16 crnti,
                                       UInt8  sendSF,
                                       UInt16 sendSFN,
                                       UInt8  qIndex,
                                       UInt16 pduChoice,
                                       UInt8  cqiMode
                                     );
inline UlDataQueueNode* getULDataPduNode( UInt8  *cqiPdu_p,
                                          UInt16 cqiPduLen,
                                          UInt16 crnti,
                                          UInt8  sendSF,
                                          UInt16 sendSFN,
                                          UInt8  qIndex,
                                          UInt16 pduChoice,
                                          UInt8  cqiMode
                                       );

#ifdef UT_TESTING                                     
inline static void sendErrorInd( UInt8 sendSF, 
                                 UInt16 sendSFN, 
                                 UInt16 rnti, 
                                 UInt8 cause, 
                                 UInt8 dlErrInfoFlag, 
                                 UInt8 ulErrInfoFlag, 
                                 UInt8 harqProNum);
#endif                                 
UInt8 fillPhyPortInfo(UInt16 *rxPort,
                      UInt16 *txPort,
                      UChar8 *macIPAddress,
                      UChar8 *phyIPAddress
                    );

void prepareBSR(UInt8 **sendBufer_p,UInt8 isLongBSR,
            UInt8 bsr0,UInt8 bsr1, UInt8 bsr2, UInt8 bsr3,UInt8 lcId);


/****************************************************************************
 * Function Name  : bindThreadToCPUCore
 *
 * Inputs         : threadId   - Thread ID of the calling thread
 *                : CoreNumber - The core to which this funtionality has to 
 *                               be binded.   
 *                : threadName - A thread Identification name.
 * Outputs        : 
 * Returns        : 
 * Description    : This function will bind the calling thread to the CPU
 *                  Core as mentioned in the function argument.  
 ****************************************************************************/
#ifdef REAL_TIME_THREAD_SCHEDULING
inline void bindThreadToCPUCore (ULong32 threadId, 
                          UInt8 CoreNumber,
                          UInt8* threadName)
{
    cpu_set_t cpuSetSch;
    Int res = 0;
    Int coreNum = 0;

    CPU_ZERO(&cpuSetSch);
    coreNum = CoreNumber;
    CPU_SET(coreNum , &cpuSetSch);
    res = pthread_setaffinity_np(threadId,
                                 sizeof(cpuSetSch),
                                 &cpuSetSch);
    if (res != 0 )
    {
        ltePanic(" Unable to set affinity for the thread %s.\n", threadName);
        return;
    }
    printf("\n ## '%s' Thread Succesfully Binded at coreNum %d\n",threadName,coreNum);
    return; 
}
#endif
/****************************************************************************
 * Function Name  : getCurrentglobaltick
 *
 * Inputs         :
 * Outputs        : 
 * Returns        : current value of global tick
 * Description    : returns the current value of global tick 
 * this func called by /lteenodeb/simulators/macEmulator/src/interfaces/lteLayer2RRCInterface.c:316:
                        `getCurrentglobaltick'
 ****************************************************************************/

/* SPR 15909 fix start */
inline tickType_t getCurrentglobaltick()
{
/* SPR 15909 fix end */
    return globalTTITickCount_g;
}
/****************************************************************************
 * Function Name  : prepareCRCforMsg3 
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : 
 ****************************************************************************/
void prepareCRCforMsg3(ControlNodeInfo *ctlInfo_p)
{
    UlControlQueueNode *ulcontrolNode_p = (UlControlQueueNode *) getMemFromPool(sizeof(UlControlQueueNode), PNULL);
/* SPR 609 changes start */
 #ifdef ULCTL_HARQ_DEBUG    
/* SPR 609 changes end */
    UInt16 currentSFNSF    = 0;
    UInt16 sendSFNSF    = 0;
/* SPR 609 changes start */
#endif     
/* SPR 609 changes end */

    if ( ulcontrolNode_p == PNULL )
    {
        lteWarning("Not able to get memory in prepareCRCforMsg3\n");
        return;
    }
    ulcontrolNode_p->SF          = ctlInfo_p->sendSF; 
    ulcontrolNode_p->crnti       = ctlInfo_p->crnti;
    ulcontrolNode_p->SFN         = ctlInfo_p->sendSFN; 
    ulcontrolNode_p->PDUChoice   = ctlInfo_p->pduChoice;
    //ulcontrolNode_p->crcInd    = ctlInfo_p->crcInd; 
    ulcontrolNode_p->crc         = ctlInfo_p->crc;
    ulcontrolNode_p->twoTbFlag   = ctlInfo_p->twoTbFlag;
    ulcontrolNode_p->tb1harqInfo = ctlInfo_p->tb1harqInfo;
    ulcontrolNode_p->tb2harqInfo = ctlInfo_p->tb2harqInfo;
    ulcontrolNode_p->contentionAckFlag = TRUE;

#ifdef ULCTL_HARQ_DEBUG
    // _ULHARQ_RETX_  start  

    if(ulcontrolNode_p->SFN != SFN_g)
    {
        if ((!((ulcontrolNode_p->SFN==0)&& (SFN_g==(MAX_SFN -1)))) && (!((ulcontrolNode_p->SFN==(MAX_SFN -1) )&& (SFN_g==0))))
        {
            currentSFNSF=SFN_g * 10 + SF_g;
            sendSFNSF= ulcontrolNode_p->SFN * 10 + ctlInfo_p->qIndex;
        }
        else if((ulcontrolNode_p->SFN==0)&& (SFN_g==(MAX_SFN -1)))
        {
            currentSFNSF=0;
            sendSFNSF=(MAX_SFN - 1);
        }
        else if((ulcontrolNode_p->SFN==(MAX_SFN -1) )&& (SFN_g==0))
        {
            currentSFNSF=(MAX_SFN - 1);
            sendSFNSF=0;
        }
    }
    else
    {
        currentSFNSF=SF_g;
        sendSFNSF=ctlInfo_p->qIndex;
    }

    FLOW_DEBUG(stderr,"currentSFN %d currentSF %d\n",SFN_g, SF_g);
    FLOW_DEBUG(stderr,"In [%s] printLogCnt = %lld ctlqIndex %d ctlsendSF %d  ctlsendSFN  %d\n",__func__,++printLogCnt, ctlInfo_p->qIndex,ulcontrolNode_p->SF,ulcontrolNode_p->SFN);
    FLOW_DEBUG(stderr,"In [%s] printLogCnt = %lld currentSFNSF %d sendSFNSF %d\n",__func__,++printLogCnt,currentSFNSF,sendSFNSF);

    if (currentSFNSF<sendSFNSF)
    {   
        pushNode(&ULControlAndDataArr_g[ctlInfo_p->qIndex].controlQueue, &(ulcontrolNode_p->ulControlAnchor) );
    }
    else
    {
        FLOW_DEBUG(stderr,"In [%s] printLogCnt = %lld CNTLINITHARQRETRY\n",__func__,++printLogCnt);
    }

    // _ULHARQ_RETX_  end  
#else
        pushNode(&ULControlAndDataArr_g[ctlInfo_p->qIndex].controlQueue, &(ulcontrolNode_p->ulControlAnchor) );
#endif
}
/****************************************************************************
 * Function Name  : prepareControlNode 
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : 
 ****************************************************************************/

inline void prepareControlNode(ueContextForUplink *ueContextInfo_p, ControlNodeInfo *ctlInfo_p)
{
    UlControlQueueNode *ulControlNode_p = PNULL;
    UInt32 subFrameIndex = ctlInfo_p->sendSF;
/* SPR 609 changes start */
 #ifdef ULCTL_HARQ_DEBUG    
/* SPR 609 changes end */
    UInt16 currentSFNSF    = 0;
    UInt16 sendSFNSF    = 0;
/* SPR 609 changes start */
#endif    
/* SPR 609 changes end */

    if ( ueContextInfo_p == PNULL )
        return;
    ulControlNode_p = ueContextInfo_p->ulControlNode_p[subFrameIndex];
    if ( ulControlNode_p != PNULL )
    {
        // logic to combine the PDU choice
        ulControlNode_p->PDUChoice = selectControlPDUChoice (ulControlNode_p->PDUChoice, ctlInfo_p->pduChoice);
        if ( CRC_HARQ_PDU_INFO ==  ulControlNode_p->PDUChoice )
        {
            if ( CRC_PDU_INFO == ctlInfo_p->pduChoice ) 
            {
                ulControlNode_p->crc = ctlInfo_p->crc;                 
            }
            else if ( HARQ_PDU_INFO == ctlInfo_p->pduChoice )
            {
                ulControlNode_p->twoTbFlag = ctlInfo_p->twoTbFlag;
                ulControlNode_p->tb1harqInfo = ctlInfo_p->tb1harqInfo;
                ulControlNode_p->tb2harqInfo = ctlInfo_p->tb2harqInfo;
                ulControlNode_p->contentionAckFlag = FALSE; 
            }
        }
    }
    else 
    {
        ulControlNode_p = (UlControlQueueNode *) getMemFromPool(sizeof(UlControlQueueNode), PNULL);

        ulControlNode_p->SF                = ctlInfo_p->sendSF; 
        ulControlNode_p->crnti             = ctlInfo_p->crnti;
        ulControlNode_p->SFN               = ctlInfo_p->sendSFN; 
        ulControlNode_p->PDUChoice         = ctlInfo_p->pduChoice;
        ulControlNode_p->crc               = ctlInfo_p->crc;
        ulControlNode_p->twoTbFlag         = ctlInfo_p->twoTbFlag;
        ulControlNode_p->tb1harqInfo       = ctlInfo_p->tb1harqInfo;
        ulControlNode_p->tb2harqInfo       = ctlInfo_p->tb2harqInfo;
        ulControlNode_p->contentionAckFlag = FALSE; 
#ifdef UT_TESTING 
        printf("**current SF[%d] current SFN [%d] node SF = %d NodeSFN = %d qindex = %d \n",
                SF_g, SFN_g,ulControlNode_p->SF,ulControlNode_p->SFN, ctlInfo_p->qIndex);
        FLOW_DEBUG(stderr,"**Control node SF[%d] current SFN [%d] node SF = %d NodeSFN = %d qindex = %d \n",
                SF_g, SFN_g,ulControlNode_p->SF,ulControlNode_p->SFN, ctlInfo_p->qIndex);
#endif
#ifdef ULCTL_HARQ_DEBUG    
        // _ULHARQ_RETX_  start  
        if(ulControlNode_p->SFN != SFN_g)
        {
            if ((!((ulControlNode_p->SFN==0)&& (SFN_g==(MAX_SFN -1)))) && (!((ulControlNode_p->SFN==(MAX_SFN -1) )&& (SFN_g==0))))
            {
                currentSFNSF=SFN_g * 10 + SF_g;
                sendSFNSF= ulControlNode_p->SFN * 10 + ctlInfo_p->qIndex;
            }
            else if((ulControlNode_p->SFN==0)&& (SFN_g==(MAX_SFN -1)))
            {
                currentSFNSF=0;
                sendSFNSF=(MAX_SFN - 1);
            }
            else if((ulControlNode_p->SFN==(MAX_SFN -1) )&& (SFN_g==0))
            {
                currentSFNSF=(MAX_SFN - 1);
                sendSFNSF=0;
            }
        }
        else
        {
            currentSFNSF=SF_g;
            sendSFNSF=ctlInfo_p->qIndex;
        }

        FLOW_DEBUG(stderr,"In [%s] printLogCnt = %lld currentSFN %d currentSF %d\n",__func__,++printLogCnt,SFN_g,SF_g);
        FLOW_DEBUG(stderr,"In [%s] printLogCnt = %lld ctlqIndex %d ctlsendSF %d  ctlsendSFN  %d\n",__func__,++printLogCnt,ctlInfo_p->qIndex,ulControlNode_p->SF,ulControlNode_p->SFN);
        FLOW_DEBUG(stderr,"In [%s] printLogCnt = %lld currentSFNSF %d sendSFNSF %d\n",__func__,++printLogCnt,currentSFNSF,sendSFNSF);

        if (currentSFNSF<sendSFNSF)
        {   

            pushNode(&ULControlAndDataArr_g[ctlInfo_p->qIndex].controlQueue, &(ulControlNode_p->ulControlAnchor) );
            ueContextInfo_p->ulControlNode_p[subFrameIndex] =  ulControlNode_p;
        }
        else
        {
            FLOW_DEBUG(stderr,"In [%s] printLogCnt = %lld CNTLINITHARQRETRY\n",__func__,++printLogCnt);
        }
        // _ULHARQ_RETX_  end  
#else
        pushNode(&ULControlAndDataArr_g[ctlInfo_p->qIndex].controlQueue, &(ulControlNode_p->ulControlAnchor) );
        ueContextInfo_p->ulControlNode_p[subFrameIndex] =  ulControlNode_p;
#endif

    }
#ifdef LTE_MAC_UT_LOG
    LTE_LOG(LOG_INFO,&MACModuleLogDetail_g," prepareControlPduNode: \n");
    LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"sf = %d sfn = %d\n",ctlInfo_p->sendSF, ctlInfo_p->sendSFN);
#endif
}







#if 0
/****************************************************************************
 * Function Name  : prepareDataPduNode 
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : This function will create the data PDU node and push into 
 *                  global queue at index value(subframe) on which this data 
 *                  has to send to Peer MAC.
 ****************************************************************************/
inline void prepareDataPduNode( UInt8  *pdu_p, 
                                UInt16 pduLen, 
                                UInt16 crnti, 
                                UInt8  sendSF, 
                                UInt16 sendSFN, 
                                UInt8  qIndex, 
                                UInt16 pduChoice, 
                                UInt8  cqiMode
                              )
{
  UInt32 tbSize        = 0;
  UInt8  flag          = 0xFF; // set to invalid value
  UInt16 nodePduChoice = 0;
  UInt8 subFrame       = 0;
  RNTIInfo *rntiInfo_p = PNULL;
  
  UlDataQueueNode    *ulDataQNode_p = PNULL;
  ueContextForUplink *ueContextInfo_p = PNULL;
 

  rntiInfo_p = getRntiInfofromRNTIMap(crnti);

  ueContextInfo_p = uplinkContextInfoMap_g[rntiInfo_p->ueIndex];
  
#ifdef UT_TESTING  
  if ( ueContextInfo_p == PNULL )
  {
      lteWarning("Error!!  UE Context does not exist for crnti = %d,\
ueIndex = %d\n", crnti, rntiInfo_p->ueIndex);
      return ;
  }
#endif 

  subFrame = sendSF;
  
  ulDataQNode_p =  ueContextInfo_p->ulDataNode_p[subFrame];
  
  if ( ulDataQNode_p )
  {
    // logic to combine the PDU choice
    nodePduChoice = ulDataQNode_p->PDUChoice;
    
    if ( nodePduChoice == ULSCH_PDU && pduChoice == UCI_CQI_PDU )
    {
        flag = TRUE;
        ulDataQNode_p->PDUChoice = ULSCH_CQI_RI_PDU;
    }
    else if ( nodePduChoice == UCI_CQI_PDU && pduChoice == ULSCH_PDU )
    {
        flag = FALSE;
        ulDataQNode_p->PDUChoice = ULSCH_CQI_RI_PDU;
    }
    
    if ( flag == TRUE )
    {
      /* existing data is ULSCH_PDU and UCI_CQI_PDU received */
      UInt8 *start_p      = PNULL;
      UInt8 *sendBuffer_p = PNULL;
      UInt8 *data_p       = PNULL;
      UInt32 dataLen      = 0;
      
      start_p = sendBuffer_p = pdu_p;
      
      sendBuffer_p += pduLen;
      data_p  = ulDataQNode_p->pdu.Pdudata_p;
      dataLen = ulDataQNode_p->pdu.PDULength;
      tbSize = dataLen * 8; /* in bits */
      *sendBuffer_p     = tbSize & 0xFF;
      *(++sendBuffer_p) = ( (tbSize & 0xFF00 ) >> 8 );
      *(++sendBuffer_p) = ( (tbSize & 0xFF0000 ) >> 16 );
      *(++sendBuffer_p) = ( (tbSize & 0xFF000000 ) >> 24 );
      ++sendBuffer_p;
      if ( ueContextInfo_p->ulDataNode_p[subFrame]->pdu.Pdudata_p ) 
      {
         memcpy(sendBuffer_p, data_p, dataLen);
         sendBuffer_p += dataLen;
         freeMemPool(data_p);
         data_p = PNULL;
      }
      else
      {
#ifdef LTE_MAC_UT_LOG
          lteWarning("Error!!! data pointer in ue context node is NULL");
#endif
          return;
      }
      ulDataQNode_p->pdu.Pdudata_p = start_p;
      ulDataQNode_p->pdu.PDULength = sendBuffer_p - start_p;
    }
    else if ( flag == FALSE )
    {
      /* existing data is UCI_CQI_PDU and ULSCH_PDU received */
      UInt8 *start_p      = PNULL;
      UInt8 *sendBuffer_p = PNULL;
      
      start_p = sendBuffer_p = ulDataQNode_p->pdu.Pdudata_p;
      sendBuffer_p += ulDataQNode_p->pdu.PDULength;
      tbSize = (pduLen*8); /* in bits */
      *sendBuffer_p     = tbSize & 0xFF;
      *(++sendBuffer_p) = ( (tbSize & 0xFF00 ) >> 8 );
      *(++sendBuffer_p) = ( (tbSize & 0xFF0000 ) >> 16 );
      *(++sendBuffer_p) = ( (tbSize & 0xFF000000 ) >> 24 );
      ++sendBuffer_p; 
      memcpy(sendBuffer_p, pdu_p, pduLen);
      sendBuffer_p += pduLen;
      ulDataQNode_p->pdu.PDULength =  sendBuffer_p - start_p; 
    } 
  }
  else 
  {
    UlDataQueueNode *uldataNode_p = (UlDataQueueNode *) getMemFromPool(sizeof(UlDataQueueNode), PNULL);

    uldataNode_p->SF            = sendSF;
    uldataNode_p->cqiMode       = cqiMode;
    uldataNode_p->crnti         = crnti;
    uldataNode_p->SFN           = sendSFN;
    uldataNode_p->PDUChoice     = pduChoice;
    uldataNode_p->msg3Flag      = FALSE;
    uldataNode_p->pdu.PDULength = pduLen;
    uldataNode_p->pdu.Pdudata_p = pdu_p; 
    pushNode(&ULControlAndDataArr_g[qIndex].dataQueue, &(uldataNode_p->ulDataAnchor) );

#if 0
    fprintf(stderr,"data node pushed at sf = %d sfn = %d qIndex = %d\n",sendSF, sendSFN, qIndex);
#endif    
    ueContextInfo_p->ulDataNode_p[subFrame] =  uldataNode_p;
  }
#ifdef LTE_MAC_UT_LOG
  LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"prepareDataPduNode:");
  LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"sf = %d sfn = %d\n",sendSF, sendSFN);
#endif
}

#endif

#ifdef HO_SETUP_CHANGES
/* +CA */
SInt32 targetEnbTxFd_g[MAX_NUM_CELLS] = {-1, -1};
SockAddr targetEnbmacLayerAddr_g[MAX_NUM_CELLS];
/* -CA */
inline void sendSFItoSourceENBMac( UInt8 *sendBuffer_p, UInt32 msgLen, UInt8 cellIndex )
{
    if ( sendto(txSockFD_g[cellIndex],(UInt8 *)sendBuffer_p,msgLen, 0,
            (struct sockaddr*)&macLayerAddr_g[cellIndex], sizeof(SockAddr)) < 0 )
    {
       ltePanic("Error in sending data on socketi for SFI to source:: errno=%d",errno);
    }
#ifdef DL_UL_SPLIT_TDD
    if ( sendto(txSockFD_g[cellIndex],(UInt8 *)sendBuffer_p,msgLen, 0,
            (struct sockaddr*)&macLayerAddrULThd_g[cellIndex], sizeof(SockAddr)) < 0 )
    {
       ltePanic("Error in sending data on socketi for SFI to source:: errno=%d",errno);
    }
#endif
}
/* +CA */
inline void sendDataToTargetEnbMac( UInt8 *sendBuffer_p, UInt32 msgLen, UInt8 cellIndex)
{
    if ( sendto(targetEnbTxFd_g[cellIndex],(UInt8 *)sendBuffer_p,msgLen, 0,
            (struct sockaddr*)&targetEnbmacLayerAddr_g[cellIndex], sizeof(SockAddr)) < 0 )
    {
        ltePanic("Error in sending data on socket for sending data on target ENB:: errno=%d",errno);
    }
}
/* -CA */
#endif

/* SSI Changes Start */
#if (defined(LTE_EMBMS_SUPPORTED) && !defined(LAYER2_DLTG_THREAD))
int initTGSocket()
{
#ifndef UE_IPV6_ADDR
    if( (tgGlueTx = socket_wrapper(AF_INET, SOCK_DGRAM, 0 )) < 0 )
    {
	    MAC_EMULATOR_LOG(LOG_INFO,
			    "\n[Error]DL-TG  fd Socket Opening\n");
	    return -1;
    }
    /* Filling the Address Info for DL-TG */
    tgGlueAddr.sin_family = AF_INET;
 //       tgGlueAddr.sin_addr.s_addr = inet_addr_wrapper(TGIP);
//swapnil

  tgGlueAddr.sin_addr.s_addr = inet_addr_wrapper((const char *)dlTgIP);
    tgGlueAddr.sin_port = htons_wrapper(TGPORT);
#else
    if( (tgGlueTx = socket_wrapper(AF_INET6, SOCK_DGRAM, 0 )) < 0 )
    {
        MAC_EMULATOR_LOG(LOG_INFO,
                "\n[Error]DL-TG  fd Socket Opening\n");
        return -1;
    }
    sockAddr6.sin6_family = AF_INET6;
    sockAddr6.sin6_port = htons(TGPORT);
    if( 1 != inet_pton(AF_INET6, (const char *)dlTgIP, &sockAddr6.sin6_addr))
    {
        ltePanic ("Error while converting the address for IPv6");
    }
#endif

}
int sendDataToTG( UInt8 *sendBuff_p, UInt32 tgApiMsgSize)
{
#ifndef UE_IPV6_ADDR
    if( -1 == sendto_wrapper(tgGlueTx, sendBuff_p, tgApiMsgSize, 0, &tgGlueAddr, sizeof(SockAddr)) )
#else
    if( -1 == sendto_wrapper(tgGlueTx, sendBuff_p, tgApiMsgSize, 0, &sockAddr6, sizeof(sockAddr6)) )
#endif
    {
        fprintf(stderr,"\nFail to send tick [%d]\n",errno);
	    MAC_EMULATOR_LOG(LOG_INFO,
			    "\nsendto_wrapper for sendBuff_p failed \n");
	    return -1;
    }          
}
#endif
/* SSI Changes End */ 
// code needs for only mac testing
#if 0
/****************************************************************************
 * Function Name  : createAndSendAperiodicCQI 
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *
 ****************************************************************************/
void createAndSendAperiodicCQI(UInt16 crnti, UInt8 sendSF, UInt16 sendSFN, UInt8 qIndex)
{
    UInt8 reportingType    = 0;
    UInt8 numCQIInstances  = 0;
    UInt8 nextToSend       = 0;
    UInt8 *cqiDataBuffer_p = PNULL;
    UInt8 *sendBuffer_p    = PNULL;
    UInt8 *tagLen_p        = PNULL;
    UInt16 cqiReportLen    = 0;
    UInt8 cqiMode          = APERIODIC_MODE;
    UInt8 i                = 0;
    RNTIInfo *rntiInfo_p   = PNULL;;
    rntiInfo_p = getRntiInfofromRNTIMap(crnti);

#ifdef UT_TESTING    
    if (rntiInfo_p == PNULL) 
    {
        lteWarning("Error!!! [2]No Rnti Info present for crnti = %d\n", crnti);
        return;
    }
    if ( rntiInfo_p->aperiodiccqiInfo == PNULL )
    {
        lteWarning("Error!!! No cqi report present for crnti = %d\n", crnti);
        return;
    }
#endif    
    numCQIInstances = rntiInfo_p->aperiodiccqiInfo->numInstances;

#ifdef UT_TESTING
    if ( numCQIInstances == 0 )
    {
        lteWarning("Error!! in createAndSendAperiodicCQI: no cqi report exist for this crnti = %d\n", crnti);
        return ;
    }
#endif    

    nextToSend = (rntiInfo_p->aperiodiccqiInfo->nextToSend % numCQIInstances);

    (rntiInfo_p->aperiodiccqiInfo->nextToSend)++;

    cqiDataBuffer_p = (UInt8 *) getMemFromPool(SBUFFERSIZE, PNULL);

#ifdef UT_TESTING
    if ( cqiDataBuffer_p == PNULL )
    {
        lteWarning("Error!!! createAndSendAperiodicCQI: memory allocation failed\n");
        return;
    }
#endif    

    memset(cqiDataBuffer_p, 0, SBUFFERSIZE);
    sendBuffer_p = cqiDataBuffer_p;
    reportingType = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].reportType;

    if ( reportingType == APERIODIC_CQI_MODE_2_0)
    {
        *sendBuffer_p     = reportingType & 0x00FF;
        *(++sendBuffer_p) = ( (reportingType & 0xFF00 ) >> 8 );
        tagLen_p = sendBuffer_p;
        sendBuffer_p += 2;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].wideBandIndex;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].cqiVal;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subBandLength;

        for ( i = 0; i < rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subBandLength; i++)
        {
            *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subCqiInfo[i].subbandNum;
        }
        cqiReportLen  = sendBuffer_p - cqiDataBuffer_p + 1;
        *(++tagLen_p) = cqiReportLen & 0x00FF; 
        *(++tagLen_p) = ( (cqiReportLen & 0xFF00 ) >> 8 );
    }
    else if ( reportingType == APERIODIC_CQI_MODE_3_0)
    {
        *sendBuffer_p = reportingType & 0x00FF;
        *++sendBuffer_p = ( (reportingType & 0xFF00 ) >> 8 );

        tagLen_p = sendBuffer_p;
        sendBuffer_p += 2;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].wideBandIndex;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subBandLength;

        for ( i = 0; i < rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subBandLength; i++)
        {
            *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subCqiInfo[i].subbandNum;
            *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subCqiInfo[i].cqiIndex;
        }
        cqiReportLen  = sendBuffer_p - cqiDataBuffer_p + 1;
        *(++tagLen_p) = cqiReportLen & 0x00FF; 
        *(++tagLen_p) = ( (cqiReportLen & 0xFF00 ) >> 8 );
    }
    else if ( reportingType == APERIODIC_CQI_MODE_3_1)
    {
        *sendBuffer_p     = reportingType & 0x00FF;
        *(++sendBuffer_p) = ( (reportingType & 0xFF00 ) >> 8 );
        tagLen_p = sendBuffer_p;
        sendBuffer_p += 2;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].widebandCQIValueCodeword0;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].widebandCQIValueCodeword1;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].widebandPMI;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subBandLength;

        for ( i = 0; i < rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subBandLength; i++)
        {
            *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subCqiInfo[i].subbandNum;
            *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subCqiInfo[i].subbandCQIValueCodeword0;
            *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subCqiInfo[i].subbandCQIValueCodeword1;
        }
        cqiReportLen = sendBuffer_p - cqiDataBuffer_p + 1;
        *(++tagLen_p) = cqiReportLen & 0x00FF; 
        *(++tagLen_p) = ( (cqiReportLen & 0xFF00 ) >> 8 );
    }
    else if ( reportingType == APERIODIC_CQI_MODE_1_2)
    {
        *sendBuffer_p     = reportingType & 0x00FF;
        *(++sendBuffer_p) = ( (reportingType & 0xFF00 ) >> 8 );
        tagLen_p = sendBuffer_p;
        sendBuffer_p += 2;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].widebandCQIValueCodeword0;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].widebandCQIValueCodeword1;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subBandLength;

        for ( i = 0; i < rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subBandLength; i++)
        {
            *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subCqiInfo[i].subbandNum;
            *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subCqiInfo[i].pmiValue;
        }
        cqiReportLen = sendBuffer_p - cqiDataBuffer_p + 1;
        *(++tagLen_p) = cqiReportLen & 0x00FF; 
        *(++tagLen_p) = ( (cqiReportLen & 0xFF00 ) >> 8 );
    }
    else if ( reportingType == APERIODIC_CQI_MODE_2_2)
    {
        *sendBuffer_p     = reportingType & 0x00FF;
        *(++sendBuffer_p) = ( (reportingType & 0xFF00 ) >> 8 );
        tagLen_p = sendBuffer_p;
        sendBuffer_p += 2;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].widebandCQIValueCodeword0;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].widebandCQIValueCodeword1;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].cqiValueCodeWord0;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].cqiValueCodeWord1;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].pmiValue;
        *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subBandLength;

        for ( i = 0; i < rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subBandLength; i++)
        {
            *(++sendBuffer_p) = rntiInfo_p->aperiodiccqiInfo->aperiodicInfo[nextToSend].subCqiInfo[i].subbandNum;
        }
        cqiReportLen = sendBuffer_p - cqiDataBuffer_p + 1;
        *(++tagLen_p) = cqiReportLen & 0x00FF; 
        *(++tagLen_p) = ( (cqiReportLen & 0xFF00 ) >> 8 );
    }
    else
    {
        return;
    }
    getULDataPduNode(cqiDataBuffer_p,cqiReportLen,crnti,sendSF, sendSFN, qIndex, UCI_CQI_PDU, cqiMode);
}
#endif
#if 0
/****************************************************************************
 * Function Name  : setValueInPeriodicityCounter
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *
 ****************************************************************************/

void setValueInPeriodicityCounter(UInt32 periodicity , UInt32 crnti)
{
  SInt32 rowId = -1;
  switch ( periodicity )
  {
    case PERIODICITY_2:
      periodicCQITable[0].numOfUEs++;
      rowId = 0;
      break;
    case PERIODICITY_5:
      periodicCQITable[1].numOfUEs++;
      rowId = 1;
      break;
    case PERIODICITY_10:
      periodicCQITable[2].numOfUEs++;
      rowId = 2;
      break;
    case PERIODICITY_20:
      periodicCQITable[3].numOfUEs++;
      rowId = 3;
      break;
    case PERIODICITY_32:
      periodicCQITable[4].numOfUEs++;
      rowId = 4;
      break;
    case PERIODICITY_40:
      periodicCQITable[5].numOfUEs++;
      rowId = 5;
      break;
    case PERIODICITY_64:
      periodicCQITable[6].numOfUEs++;
      rowId = 6;
      break;
    case PERIODICITY_80:
      periodicCQITable[7].numOfUEs++;
      rowId = 7;
      break;
    case PERIODICITY_128:
      periodicCQITable[8].numOfUEs++;
      rowId = 8;
      break;
    case PERIODICITY_160:
      periodicCQITable[9].numOfUEs++;
      rowId = 9;
      break;
    case PERIODICITY_256:
      periodicCQITable[10].numOfUEs++;
      rowId = 10;
      break;
    default:
#ifdef LTE_MAC_UT_LOG
      lteWarning("Invalid Periodicity");
#endif
      rowId = -1;
  }
  if ( rowId == -1)
    return;
  periodicCQITable[rowId].ueId[periodicCQITable[rowId].numOfUEs - 1] = crnti;
}
#endif
/****************************************************************************
 * Function Name  : initSendPeridicCQIBuffer
 * Inputs         :
 *
 *
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *
 ****************************************************************************/
void initSendPeridicCQIBuffer()
{
    UInt32 row = 0;
    UInt32 periodicity[] ={2,5,10,20,32,40,64,80,128,160,256};
    for( row = 0 ; row < TOTAL_NUM_PERIODICITY ; row++)
    {
        periodicCQITable[row].periodicity = periodicity[row];
    } 

}

#if 0
/****************************************************************************
 * Function Name  :  readTAInfoFromFile 
 * Inputs         :  Configuration file provided by user/default.
 * Outputs        :
 * Returns        :
 * Description    : This file will be read on slecting user option 
 *                    for sendTAFromFile procedure testing 
 ****************************************************************************/
void readTAInfoFromFile( const UChar8 *fileName )
{
  FILE        *taInfoFile_p    = NULL;
  UInt32       timingAdvance = 0;
  UInt32       crnti = 0, taToBePresentFlag = 0;
  UChar8        line[2000];
  UInt16       count  = 0;

  taInfoFile_p = fopen(fileName,"r");

  if (taInfoFile_p == NULL)
  {
    lteWarning("Error!! File[%s] Not Found. - readTAInfoFromFile() \n", fileName );
    return ;
  }

  while ( (fgets (line, 100, taInfoFile_p))  &&
      (count < MAX_NUM_UE) )
  {
    if ( '#' == line[0])
      continue;

    sscanf( line,
        "%u %u %u", 
        &crnti, &timingAdvance, &taToBePresentFlag);
    
    ueContextForUplink *ueContextInfo_p = getUEContext(crnti);
    if ( ueContextInfo_p == PNULL )
    {
#ifdef LTE_MAC_UT_LOG
      lteWarning("Error!! in readTAInfoFromFile: UE Context does not exist for UEId = %d\n", crnti);
#endif
      continue ;
    }
    ueContextInfo_p->timingAdvance = (UInt8) timingAdvance;

    if ( taToBePresentFlag == 0 )
    {
       ueContextInfo_p->taToBeSentFlag = 1;
    }
    else if ( taToBePresentFlag == 1)
    {
       ueContextInfo_p->taToBeSentFlag = 2;
    }
    else
    {
       lteWarning("Invalid Valid");
       continue;
    }
    count++;
  }
  fclose(taInfoFile_p);
}

#endif

/****************************************************************************
 * Function Name  : fillCRCInfo 
 * Inputs         :  
 * Outputs        :  
 * Returns        :  
 * Description    :  
 *                   
 ****************************************************************************/
void fillCRCInfo(ControlNodeInfo *ctlInfo_p)
{
    UInt8  crc = CRC_INVALID;
    if(uLnackChoice_g != 10)
    {

        if (ULpacketCounter_s == TOTALPACKETS)
        {
            ULpacketCounter_s = 0;
            ULarrIndex_s = 0;
        }

        if(ulPacketArr_g[0] != 0 && 
            
            ulPacketArr_g[ULarrIndex_s] == ULpacketCounter_s + 1)
        {   
            /*NACK case */
            ULarrIndex_s++;
            crc = CRC_ERROR;
        }
        else {
            crc = CRC_CORRECT; /*ACK - 1*/
        }
        ULpacketCounter_s++;
    }
        ctlInfo_p->crc = crc;
}
       
/****************************************************************************
 * Function Name  :  prepareCCCHSDU
 * Inputs         :  
 * Outputs        :  
 * Returns        :  
 * Description    :  
 *                   
 ****************************************************************************/
void prepareCCCHSDU(UInt8 **sendBuffer_p, RARequest *raReq_p)
{
    UInt8 *sendBufer_p = *sendBuffer_p;
    UInt8 i = 0;

    if (PNULL != raReq_p)
    {
        while (i < raReq_p->rrcConReqLen)
        {
            *sendBufer_p = raReq_p->rrcConnectionReq[i++];
            sendBufer_p++;
        }
    }

    (*sendBuffer_p) = sendBufer_p;
}

/****************************************************************************
 * Function Name  :  validateSFNandSFAndgetDelay
 * Inputs         :  value of SFN and SF received from Peer MAC  
 * Outputs        :  
 * Returns        :  
 * Description    : This function will validate the SFN SF received with current
 *                  SFN SF running at phy, and returns delay. 
 *                   
 ****************************************************************************/
inline UInt8 validateSFNandSFAndgetDelay( UInt16 recvSFN, UInt8 recvSF, UInt16 currentSFN, UInt8 currentSF )
{
  UInt8 flag = 0x00, delay = 0xFF;
#if 0  
  fprintf(stderr,"recvSFN %d recvSF %d currentSFN %d currentSF %d", recvSFN, recvSF, currentSFN, currentSF);
 #endif
  /* check for system frame number */
  if ( recvSFN == currentSFN)
  {
      /* check for sub frame number */
      if ( recvSF <= currentSF )
      {
          if ( (delay = (currentSF - recvSF)) < SF_DIFF )
              flag = 0x01;    
      }
      else
      {
          //fprintf(stderr,"this should never occur\n");
          /* this should actually be PEER_MAC_ADVANCE inseted of SF_DIF*/
          if ( (delay = (recvSF - currentSF)) <= PEER_MAC_ADVANCE )
              flag = 0x01;    
      }
  }
  else if ( recvSFN > currentSFN )
  {
      if ( currentSFN == 0 && recvSFN == (MAX_SFN - 1))
      {
          if ( recvSF > currentSF )
          {
              if ( (delay = (MAX_SUBFRAME - recvSF + currentSF )) < SF_DIFF )  
                 flag = 0x01;
          }
      }
      else
      {
          if ( (delay = (MAX_SUBFRAME - currentSF + recvSF )) <= PEER_MAC_ADVANCE )
              flag = 0x01;  
      }

  }
  else if ( recvSFN < currentSFN ) 
  {
      if ( ( currentSFN - recvSFN <= 1 )  || ( (currentSFN - recvSFN) == (MAX_SFN - 1) ) )
      {
          if ( recvSF > currentSF )
          {
              if ( (delay = (MAX_SUBFRAME - recvSF + currentSF )) < SF_DIFF )  
                 flag = 0x01;
          }
#if 1
          else
          {
              if ( (delay = (MAX_SUBFRAME - currentSF + recvSF )) <= PEER_MAC_ADVANCE  )  
                 flag = 0x01;
          }
#endif
      } 
  }
  else
  {
      FLOW_DEBUG(stderr," should never come in this block \n");
  }
  
  if (flag == 0x01)
  {
    /* Increment successful Stats */
    statsGeneralInfo_g.statsSuccessSFNSynch++;
    return delay;
  }
  else
  {
    /* Increment unsuccessful Stats */
    statsGeneralInfo_g.statsUnSuccessSFNSynch++; 
//    fprintf(stderr,"Invalid recvSFN %d recvSF %d currentSFN %d currentSF %d", recvSFN, recvSF, currentSFN, currentSF);
   // fprintf(stderr, " Invalid Delay Calculated \n");
    return 0xFF; // invalid delay
  }

}

#if 0
/****************************************************************************
 * Function Name  :  displayStatistics 
 * Inputs         :    
 * Outputs        :  
 * Returns        :  
 * Description    : This function will display the statistic. 
 *                   
 ****************************************************************************/
void displayStatistics()
{
    printf("Statistics:\n\n");
    printf("RA Response Pending: %d\n", statsGeneralInfo_g.statsRaResponsePending);
    printf("Successfull Contention Resolution: %d\n", statsGeneralInfo_g.statsContentionResSuccess);
    printf("Unsuccessfull Contention Resolution: %d\n", statsGeneralInfo_g.statsContentionResUnSuccess);
    printf("Paging messages recieved: %d\n", statsGeneralInfo_g.statsPaging);
    printf("Broadcast messages recieved: %d\n", statsGeneralInfo_g.statsBroadcast);
    printf("MIB messages: %d\n", statsGeneralInfo_g.statsMIB);
    printf("Successful SFN Synchronization: %d\n", statsGeneralInfo_g.statsSuccessSFNSynch);
    printf("Unsuccessful SFN Synchronization: %d\n\n", statsGeneralInfo_g.statsUnSuccessSFNSynch);
}
#endif
/****************************************************************************
 * Function Name  :  selectControlPDUChoice 
 * Inputs         :  oldPduChoice, newPduChoice 
 * Outputs        :  combination of old and new PduChoice
 * Returns        :  combination of old and new PduChoice
 * Description    : This function will return the valid combination 
 *                  of control Pdu Choice which can be sent on UL Control API. 
 *                   
 ****************************************************************************/
UInt16 selectControlPDUChoice(UInt16 oldPduChoice, UInt16 newPduChoice)
{
    UInt16 retPduChoice = oldPduChoice;
    if ( oldPduChoice == SR_PDU_INFO )
    {
       if ( newPduChoice == HARQ_PDU_INFO )
          retPduChoice = SR_HARQ_PDU_INFO;
    }   
    else if ( oldPduChoice == CRC_PDU_INFO ) 
    {
         if ( newPduChoice == HARQ_PDU_INFO )
             retPduChoice = CRC_HARQ_PDU_INFO;
    }
    else if ( oldPduChoice == HARQ_PDU_INFO ) 
    {
         if ( newPduChoice == CRC_PDU_INFO )
             retPduChoice = CRC_HARQ_PDU_INFO;
         else if ( newPduChoice == SR_PDU_INFO )
             retPduChoice = SR_HARQ_PDU_INFO;
    }
    return retPduChoice; 
     
}
/****************************************************************************
 * Function Name  :  selectDataPDUChoice 
 * Inputs         :  oldPduChoice, newPduChoice 
 * Outputs        :  combination of old and new PduChoice
 * Returns        :  combination of old and new PduChoice
 * Description    : This function will return the valid combination 
 *                  of data Pdu Choice which can be sent on UL Data API. 
 *                   
 ****************************************************************************/
UInt16 selectDataPDUChoice(UInt16 oldPduChoice, UInt16 newPduChoice)
{
    UInt16 retPduChoice = oldPduChoice;
    if ( oldPduChoice == ULSCH_PDU && newPduChoice == UCI_CQI_PDU )
         retPduChoice = ULSCH_CQI_RI_PDU;
    else if ( oldPduChoice == UCI_CQI_PDU && newPduChoice == ULSCH_PDU )
         retPduChoice = ULSCH_CQI_RI_PDU;
    return retPduChoice; 
     
}


/*****************************************************************************
 * Function Name  : displayULRxMsg
 * Inputs         : msg_p -> Its Point to the RX message Buffer.
 *                  bytesRead -> Number of bytes Received.
 * Outputs        : print the rx message.
 * Returns        : None
 * Description    : This function decode and print the rx msg in Hex Format.
 *****************************************************************************/
inline static void displayMsg(UInt8 *msg_p , SInt32 bytesRead)
{
    printf("****decoding the data in uplink bytesRead %d******* \n",bytesRead);
    SInt32 i =0;
    UInt8 *rev_p = PNULL;
    rev_p = msg_p;
    for (i=0; i  < bytesRead ; i++)
    {
        printf("(%d) 0x%x\t",i,*rev_p++);
    }
    printf("\n\n");
}

#if 0
/****************************************************************************
 * Function Name  :  ulDataProcessThread
 * Inputs         :  void *
 * Outputs        :
 * Returns        :
 * Description    : This function is the starting point of the ulDataProcessThread.
 *
 ****************************************************************************/
void * ulDataProcessThread(void * arg)
{
    sleep(1);
    ProcessRecvQueueNode    *processRecvQueueNode_p = PNULL;
    UInt32  numOfUlPdu    = 0;
    UInt16  sendSFN       = 0;
    UInt8  sendSF        = 0;
    UInt16  crnti         = 0;
    UInt32  CQIRequest    = 0;
    UInt8  qIndex        = 0;
    UInt16  ueIndex       = 0;
    UInt32  tbSize        = 0;
    UInt8  harqIndex     = 0;
    MacRetType macRetType = MAC_FAILURE;

    fprintf(stderr, "\n=========== UL DataProcess Thread Binding =========================\n");
    THREAD_TO_BIND_SET_CORE_NO(ulDataProcessThread_g,UL_DATA_PROCESS_CORE_NUM);
    fprintf(stderr, "\n=========== Verify UL DataProcess Thread Binding ===================\n");
    THREAD_TO_BIND_GET_CORE_NO(ulDataProcessThread_g,UL_DATA_PROCESS_CORE_NUM);
    fprintf(stderr, "\n==============================================================\n");

    while (1)
    {
        /* Tick Indication Recieved from the PHY Timer Thread */
        rlcUeSimLoop_g = FALSE;
        #ifndef FLOW_MONITOR_SECTION
        semWait(&ulProcessThreadSem_g);
        #endif
        rlcUeSimLoop_g = TRUE;
        #ifndef FLOW_MONITOR_SECTION
        semPost(&rlcPdcprunThreadSem_g);
        #endif
                        
        macRetType = MAC_FAILURE;
        UlDataQueueNode *uldataNode_p = PNULL;
        numOfUlPdu = sQueueCount(&processRecvNode_g);
        FLOW_DEBUG(stderr,"In [%s] printLogCnt = %lld  currentSFN %d currentSF %d\n",__func__,++printLogCnt,SFN_g,SF_g);
        while ( numOfUlPdu--)
        {
            uldataNode_p = PNULL;
            processRecvQueueNode_p = (ProcessRecvQueueNode *)popNode(&(processRecvNode_g));

            if (processRecvQueueNode_p == PNULL)
            {
                numOfUlPdu++;
                continue;
            }
            else
            {   
                RNTIInfo *rntiInfo_p = PNULL;
                UInt32 crc = 0; 
                sendSFN        = processRecvQueueNode_p->systemFrameNum;
                sendSF         = processRecvQueueNode_p->subFrame;
                qIndex         = processRecvQueueNode_p->qIndex;
                crnti          = processRecvQueueNode_p->crnti;  
                harqIndex      = processRecvQueueNode_p->harqIndex;  
                CQIRequest     = processRecvQueueNode_p->cqiRequest;
                rntiInfo_p     = processRecvQueueNode_p->rntiInfo_p;
                tbSize         = processRecvQueueNode_p->tbSize;
                crc            = processRecvQueueNode_p->crc;  
                ueIndex        = rntiInfo_p->ueIndex;
                ueContextForUplink *ueContextInfo_p   = uplinkContextInfoMap_g[ueIndex];
                dynamicUEContext   *ulDynUEContext_p  = dynUEContextInfoMap_g[ueIndex];

                /* handling of UL Grant */
                if ( rntiInfo_p->state == SG_WAIT ) /* SR was sent for this UE*/
                {
                    rntiInfo_p->state = UE_ACTIVE;
                    prepareLongBSR(ulDynUEContext_p, crnti, sendSF, sendSFN, qIndex);
//                    fprintf(stderr,"Long BSR data Sent to MAC.....!!!!\n");
                    freeMemPool(processRecvQueueNode_p);
                    continue;
                }
#if 0                
                fprintf(stderr,"After Long BSR recv UL Grant from  MAC.....!!!!\n");
#endif                
                /*
                   As it may happen that the UE had to move to Adaptive
                   re-transmission once again during this time we should continue
                   to send the NACK/ACK  on the basis of HI PDU only.
                 */
                if ( CQIRequest == TRUE)
                {
                    // APeriodic CQI request handling
                    createAndSendAperiodicCQI(crnti, sendSF, sendSFN, qIndex);
                }
                // based on UL ACK/NACK Ratio,
                /* if it is ACK case, check for the BSR ACK/NACK ratio
                   and based upon that prepare the data with/without BSR.
                   prepare the UL data for that crnti, and push the data into queue.
                   Correspondingly prepare the ACK and push in control queue.
                   ctlInfo.crcInd = 0xFF;
                 */

                /* Check With BSR or WITHOUT BSR */
/*                
                if (bsrAckPacketCounter_s == TOTALPACKETS)
                {
                    bsrAckPacketCounter_s = 0;
                    bsrArrIndex_s = 0;
                }
*/                
// Ashu testing                
#if 0
                if(bsrPacketArr_g[0] != 0 && bsrPacketArr_g[bsrArrIndex_s]
                        ==  bsrAckPacketCounter_s + 1)
                {
                    /* Without BSR */
                    bsrArrIndex_s++;
                    ueContextInfo_p->bsrPresentFlag = 0;
                }
                else
                {
                    /* with BSR */
                    ueContextInfo_p->bsrPresentFlag = 1;
                }
#endif
                // Ashu Always send BSR
                ueContextInfo_p->bsrPresentFlag = 1;
//                bsrAckPacketCounter_s++;
                /* send to request to RLC and Prepare the TB */
                uldataNode_p = getULDataPduNode ( PNULL, /* UL Data QueueNode Ptr*/
                                                  0,    /* Length of the UL data */
                                                  crnti,  
                                                  sendSF, 
                                                  sendSFN,
                                                  qIndex,
                                                  ULSCH_PDU,
                                                  0xFF
                                                );
                macRetType = distributeRBsAndSendReqToRLC(ueIndex,
                                                          tbSize,
                                                            ueContextInfo_p->bsrPresentFlag,
                                                            uldataNode_p,
                                                            &(uldataNode_p->pdu.Pdudata_p),
                                                            &(uldataNode_p->pdu.PDULength),
                                                            FALSE
                                                          ); 
                if ( MAC_SUCCESS == macRetType )
                {
                }
                else if ( MAC_PARTIAL_SUCCESS == macRetType )
                {
                    //fprintf(stderr,"######## sendOnly Bsr If queue load is zero ########\n");
                    sendOnlyBsrIfQLoadZero( ulDynUEContext_p, 
                                            crnti, 
                                            sendSF, 
                                            sendSFN, 
                                            qIndex, 
                                            &(uldataNode_p->pdu.Pdudata_p), 
                                            &(uldataNode_p->pdu.PDULength)) ;
                  //  prepareLongBSR(ulDynUEContext_p, crnti, sendSF, sendSFN, qIndex);
                }
                else
                {
                     fprintf(stderr, "Mac ret valueInvalid value recvd\n");
                }
                freeMemPool(processRecvQueueNode_p);
            }
        }
    }
}
#endif


/****************************************************************************
 * Function Name  :  ulReceiver
 * Inputs         :  cell Index 
 * Outputs        :  
 * Returns        :  
 * Description    : This Thread will be listenign on fast path interface or MsgQ
 *                  and receive the data from the Peer MAC. 
 *                   
 ****************************************************************************/
void ulReceiver(UInt8 cellIndex)
{
    UInt8 cellIndexDup = cellIndex;
    UInt8* recvDataBuffer_p           = PNULL;
    SInt32 bytesRead                  = -1;
    /* SPR 11588 Changes Start */
    fapiMsgIdStruct msgIdArr[MAX_ARR_SIZE] = {{0}};
    /* SPR 11588 Changes End */
    UInt64 numPdu      = 0;
    UInt8  numCells = 0;
    macPhyDataNode *data_p = NULL;
    /*thread sync fix start*/
    eventFD_t tempfd[MAX_NUM_CELLS][MAX_NUM_CELLS] = {0};
    /*thread sync fix end*/
#if 0
    void     *zeroBufCpy_p     = PNULL;  
    QSEGMENT segment;
    UInt32   splitPos = 0;
#endif

    //shashank
#if 0
    fprintf(stderr, "\n=========== UL Reciever Thread Binding =========================\n");
    THREAD_TO_BIND_SET_CORE_NO(ulReceiverThread_g, ulReceiverThreadCoreNum_g);
    fprintf(stderr, "\n=========== Verify UL Receiver Thread Binding ===================\n");
    THREAD_TO_BIND_GET_CORE_NO(ulReceiverThread_g,UL_RECEIVER_THREAD_CORE_NUM);
    fprintf(stderr, "\n==============================================================\n");

#endif

    /*****************************************************************************/
#if 0
        /*****************************************************************************/

        /******************* If Socket Interface is Enabled ***************************/

        socklen_t length = sizeof(struct sockaddr_in);
        noOfBytes = 0;
        FLOW_DEBUG(stderr,"In [%s] printLogCnt = %lld currentSFN %d currentSF %d\n",__func__,++printLogCnt,SFN_g,SF_g);
        /************************ If Zero Buffer defined ************************/
        if (zeroBufCpy_p)
        {
            //  fprintf(stderr,"\nmsgFree(%p)\n",zeroBufCpy_p);
            msgFree(zeroBufCpy_p);
            zeroBufCpy_p = PNULL;
        }
        zeroBufCpy_p = msgAlloc(PNULL,0,0,0);
        msgReserve(zeroBufCpy_p,0,RECV_BUFF_SIZE);

        //fprintf(stderr,"\nmsgAlloc(%p)\n",zeroBufCpy_p);

        if (PNULL ==  msgSegNext(zeroBufCpy_p, 0, &segment))
        {
            lteWarning("Error In msgSegNext\n");
#ifdef FLOW_MONITOR_SECTION
            flowMon_TellComplete(THD_RECEIVER);
#endif
            continue;
        }
        recvDataBuffer_p = segment.base;
        noOfBytes = RECV_BUFF_SIZE;

        //bzero(recvDataBuffer_p, noOfBytes);

        if ( ( bytesRead = recvfrom(sockfd_g, recvDataBuffer_p,
                        noOfBytes, 0,
                        (struct sockaddr*)&phyLayerAddr_g,&length)) <= 0)
        {
            if (errno != EAGAIN)
            {
                perror("recv from error in receiver thread");
                ltePanic ("lte panic due to recv from error\n" );
            }
#ifdef FLOW_MONITOR_SECTION
            flowMon_TellComplete(THD_RECEIVER);
#endif
            continue;
        }
#if 0
        if ( bytesRead < 10 )
        {
            continue;
        }  
#endif
        if (stoppedProcessing_g)
        {
            //lteWarning("MAC Emulator init not happend....\n");
#ifdef FLOW_MONITOR_SECTION
            flowMon_TellComplete(THD_RECEIVER);
#endif
            return;
        }
#endif

#ifndef UESIM_MULTI_THREAD_CELL
    cellIndex = 0;
    numCells = numCells_g;
#else
    /* this is done so that loop runs for one cell only, for cellIndex passed in function */
    cellIndex = cellIndexDup;
    numCells = cellIndex+1;
#endif

    for( ; cellIndex < numCells; cellIndex++)
    {
        /* if the cell is configured because in previous while loop only one cell was up */
        /*SPR 6690 fix Start */
        numPdu = sQueueCount(&macPhyDataQ_g[cellIndex]);
        //printf ("No of PDUs in macPhyDataQ_g [%u]\n", numPdu);
        while (numPdu--)
        {
            data_p = (macPhyDataNode*) popNode (&macPhyDataQ_g[cellIndex]);
            if (NULL != data_p)
            {
                /* Parse the buffer header */
                if (NULL == (recvDataBuffer_p = (UInt8*)data_p->msg))
                {
                    freeMemPool (data_p);
                    data_p = NULL;
                    continue;
                }
                if (REG_SUCCESS == macRegFlag_g[cellIndex])
                {
                    bytesRead = data_p->len;
                /* For Wireshark */
                if(wiresharEnable_g)
                {
                    sendto(wiresharkfd_g,(UInt8 *)recvDataBuffer_p,bytesRead, 0,
                            (struct sockaddr*)&wiresharkAddr_g, sizeof(SockAddr));
                }
                /* call the fapi processing api function */
                    if ( !stoppedProcessing_g[cellIndex] && macRegFlag_g[cellIndex] )
                {
                        /* CA_NA */                    
                        fapiReadMacMsg(recvDataBuffer_p,msgIdArr,cellIndex 
                                );
                        /* CA_NA */                    
                }
                    /* + SPR 11348 Changes */
                    /*
                if(data_p->msg)
                {
                    freeMemPool (data_p->msg);
                    data_p->msg = NULL;

                }
                     */
                    /* - SPR 11348 Changes */
                }
                if(data_p)
                {
                    freeMemPool(data_p);
                    data_p = NULL;
                }
            }
        }
    }

    UInt8 cellId=0;
#ifdef UESIM_MULTI_THREAD_CELL
    for(cellId = 0; cellId < numCells_g ; cellId++)
    {
        if (cellId != cellIndex)
        {
            //if(phyState_g[cellId] == FAPI_RUNNING)
            {
                //  if((dlTxFlag[cellId] == INVALID_SYNC_VALUE) || (dlTxFlag[cellId] == 0))
                /* + SPR_11907_11908 */
                if (macRegFlag_g[cellId])
                {
                    eventfd_write_wrapper(ev_fd_g[cellId][cellIndex],1);
                }
                /* - SPR_11907_11908 */
            }
        }
    }
#endif

#ifndef UESIM_MULTI_THREAD_CELL
    cellIndex = 0;
    numCells = numCells_g;
#else
    /* this is done so that loop runs for one cell only, for cellIndex passed in function */
    cellIndex = cellIndexDup;
    numCells = cellIndex+1;
#endif

    for( ; cellIndex < numCells; cellIndex++)
    {
        if (REG_SUCCESS == macRegFlag_g[cellIndex])
        {
            /*SPR 6690 fix Start */
            numPdu = sQueueCount(&macPhyDataQ_g[cellIndex]);
            //printf ("No of PDUs in macPhyDataQ_g [%u]\n", numPdu);
            /*CA multi-thread sync fix Start*/
            FAPI_dlDataTxRequest_st *dlDataTxReq_p = PNULL;
            UInt16 recvSFN = 0, recvSfnSf = 0;
            UInt8 qIndex=0, cellId, recvSF=0;
            fapiMsgIdStruct *msgIndexArr_p = PNULL;

            //recvSfnSf = MAC_PHY_CONVERT_16(dlDataTxReq_p->sfnsf); 
            //recvSfnSf =  sfnsf_g[cellIndex] + 1;
            recvSfnSf =  sfnsf_g[cellIndex];
            recvSFN = recvSfnSf & FAPI_SFN_MASK;
            recvSFN >>= FAPI_SFN_VALUE_SHIFT;
            recvSF = recvSfnSf & FAPI_SF_MASK;
/* + SPR 14002 Changes */            
#ifdef FDD_CONFIG
/* - SPR 14002 Changes */            
            recvSF+=1; // To get advance sf
            recvSF += 4; //To get harq subframe
            if (recvSF >= MAX_SUBFRAME)
            {
                recvSF -= MAX_SUBFRAME;
                //recvSF = 0;
                recvSFN++;
                if (recvSFN >= MAX_SFN)
                {
                    recvSFN = 0;
                }
            }

            /* No need to add +4 in qIndex since recvSF has already been incremented by 4 */
            //qIndex = recvSF + 4 + PEER_MAC_DELAY;
            qIndex = recvSF + PEER_MAC_DELAY;
            if (qIndex >= MAX_SUBFRAME)
            {
                qIndex -= MAX_SUBFRAME;
            }
/* + SPR 14002 Changes */            
#endif            
/* - SPR 14002 Changes */            
#if 0
            assert(qIndex<10);
            assert(recvSF<10);
            assert(recvSFN<1024);
#endif
            UInt16 savedSFN = recvSFN;
            UInt8 savedSF = recvSF;
            UInt16 sendSfnSf = 0;
            sendSfnSf = ((recvSFN & 0x0FFF) << FAPI_SFN_VALUE_SHIFT) | (recvSF & 0x000F);

            processDlSchPduForAllCells(sendSfnSf, msgIndexArr_p, qIndex

#ifdef UESIM_ENHANCEMENTS
                    ,0/*dlSchPduLen*/
#endif
#ifdef TDD_CONFIG
                    , savedSFN, savedSF
#else
                    , (recvSfnSf & FAPI_SF_MASK)
#endif
                    , cellIndex);

        }
    }
#ifdef UESIM_MULTI_THREAD_CELL
    /*thread sync fix start*/        
    /* adding a sync after the DL packet processing*/
    for(cellId = 0 ; cellId < numCells_g ; cellId++)
    {
        if(cellId != cellIndex)
        {
            //dlTxFlag[cellIndex] = 0;
            /* + SPR_11907_11908 */
            if((phyState_g[cellId] == FAPI_RUNNING) && (phyState_g[cellIndex] == FAPI_RUNNING) )
            /* - SPR_11907_11908 */
            {
                eventfd_write_wrapper(ev_sync_fd_g[cellId][cellIndex],1);
                eventfd_read_wrapper(ev_sync_fd_g[cellIndex][cellId],&tempfd[cellIndex][cellId]);
            }
        }
    }
#endif
    /*thread sync fix end*/        
    /*for(cellId = 0 ; cellId < numCells_g ; cellId++)
      {
      if((phyState_g[(!cellIndex)] == FAPI_RUNNING) && (cellId == cellIndex))
      eventfd_read_wrapper(ev_sync_fd_g[cellIndex][cellId],&tempfd[cellIndex][cellId]);
      }*/

    /*CA multi-thread sync fix End*/
}

/* HD FDD changes Start */
#ifdef FDD_CONFIG
#ifdef HD_FDD_CONFIG
/****************************************************************************
 * Function Name  :  isDLScheduledForHDFddUe
 * Inputs         :  UInt16 
 * Outputs        :  
 * Returns        :  UInt8
 * Description    : This function will check wheather DL is scheduled for the current tick and make
 *                  N+2 frame as UL.If DL is scheduled it returns TRUE otherwise FALSE
 *                   
 ****************************************************************************/
UInt8 isDLScheduledForHDFddUe(UInt16 ueIndex, UInt8 recvSF, UInt8 toClear, UInt8 rxCellIndex)
{
    ueContextForUplink *ueContextInfo_p = PNULL;
    ueContextInfo_p = uplinkContextInfoMap_g[ueIndex];
    UInt16 SFToMarkUL = 0;
    UInt8 retVal = FALSE;
    //UInt16 currentSF = getCurrentglobaltick()%10;
    UInt16 currentSF = sfnsf_g[rxCellIndex] & FAPI_SF_MASK;
//    fprintf(stderr,"TTI = %d,  CHecking =%d for Recv \n", getCurrentglobaltick(), recvSF);
    SFToMarkUL = (recvSF+SUBFRAME_DIFF_UE_ENB)%10;
    if(!ueContextInfo_p->scheduleMap[recvSF]) /* Check present SF is allocated for DL 0:Dl 1:UL */
    {
        //SFToMarkUL = (recvSF+SUBFRAME_DIFF_UE_ENB)%10;
        ueContextInfo_p->scheduleMap[SFToMarkUL] = TRUE;
//        fprintf(stderr,"TTI = %d, Marking SendSF = %d as UL and CHecking =%d for Recv \n", getCurrentglobaltick(), recvSF+SUBFRAME_DIFF_UE_ENB, recvSF);
        retVal = TRUE;/* If DL is scheduled for current SF */
    }
    else
    {
        retVal = FALSE;/* If UL is scheduled for current SF */
        ueContextInfo_p->scheduleMap[SFToMarkUL] = FALSE;
    }
    /*
    if(toClear)
    {
        ueContextInfo_p->scheduleMap[recvSF]= FALSE;
    }
    */
//    fprintf(stderr,"[%d] Current SF = %d, Recv SF = %d, Setting advance SF = %d as %d [0:DL 1 : UL]\n", getCurrentglobaltick(), currentSF, recvSF, SFToMarkUL, ueContextInfo_p->scheduleMap[SFToMarkUL] );
    return retVal;
}
#endif
#endif
/* HD FDD Changes End */
/****************************************************************************
 * Function Name  :  fillApiHeader
 * Inputs         :  sendbuffer *, API Name, msgLength  
 * Outputs        :  
 * Returns        :  
 * Description    : This function will fill the API header. 
 *                   
 ****************************************************************************/
void fillApiHeader(UInt8 *sendBuffer_p, UInt16 apiName, UInt16 msgLen)
{
     /* transaction Id */
     *sendBuffer_p = ( transactionId_g & 0x00FF);
     sendBuffer_p++;
     *sendBuffer_p = ( transactionId_g & 0xFF00) >> 8;
     sendBuffer_p++;
     /* src Module Id */
     *sendBuffer_p = ( MAC_EMU_PHY & 0x00FF);
     sendBuffer_p++;
     *sendBuffer_p = ( MAC_EMU_PHY & 0xFF00) >> 8;
     sendBuffer_p++;
     /* dest Module Id */
     *sendBuffer_p = ( PEER_MAC & 0x00FF);
     sendBuffer_p++;
     *sendBuffer_p = ( PEER_MAC & 0xFF00) >> 8;
     sendBuffer_p++;
     /* API Identifier */
     *sendBuffer_p = ( apiName & 0x00FF);
     sendBuffer_p++;
     *sendBuffer_p = ( apiName & 0xFF00) >> 8;
     sendBuffer_p++;
     /* message Length */
     *sendBuffer_p = ( msgLen & 0x00FF);
     sendBuffer_p++;
     *sendBuffer_p = ( msgLen & 0xFF00) >> 8;
     sendBuffer_p++;
     /* increment the global transaction id */
     transactionId_g++;
}

/* HD FDD Changes start */
#ifdef FDD_CONFIG
#ifdef HD_FDD_CONFIG
/****************************************************************************
 * Function Name  :  clearUeScheduleMapContext
 * Inputs         :  UInt16 
 * Outputs        :  
 * Returns        :  UInt8
 * Description    : This function will reset the previous tick scheduleMap information of all ue's.
 *                  Setting the value 0=FALSE as it keep on listen for DL
 *                   
 ****************************************************************************/
void clearUeScheduleMapContext(UInt8 recvSF)
{
    ueContextForUplink *ueContextInfo_p = PNULL;
    UInt16 currentSF = 0;
    UInt16 clearSF = 0;
    /* +- SPR 18268 */
    UInt16 ueCount = 0;
    /* +- SPR 18268 */

    clearSF = ((getCurrentglobaltick()-1)%10);
    //clearSF = recvSF;
    //clearSF = sfnsf_g & FAPI_SF_MASK;
    for ( ueCount=0;ueCount<MAX_UE_SUPPORTED;ueCount++)
    {
        ueContextInfo_p = uplinkContextInfoMap_g[ueCount];
        if( ueContextInfo_p != NULL)
        {
          ueContextInfo_p->scheduleMap[clearSF] = FALSE;
          //fprintf(stderr," TTI = %d Clearing RecvSF = %d\n", getCurrentglobaltick(), clearSF);
        }
    }
}
#endif
#endif
/* Hd FDD changes End */
/****************************************************************************
 * Function Name  : initWireSharkSocket
 * Inputs         : sockaddr_in, socketFileDescriptor,Port Number
 * Outputs        :
 * Returns        : status (0-Failure,1-Successful)
 * Description    : to intialize the Socket address with specific socket file
 descriptor and specific port
 ****************************************************************************/
static UInt8 initWireSharkSocket(UInt32 port)
{
    if ((wiresharkfd_g = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        lteWarning("Error!! Socket Failed");
        return 0;
    }
    wiresharkAddr_g.sin_family = AF_INET;
    wiresharkAddr_g.sin_port = htons(port);
    return 1;
}

/****************************************************************************
 * Function Name  : getIPAddrFromFile()
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    :
 ****************************************************************************/
void getIPAddrFromFile()
{
    FILE   *file_p;
    UInt8 length;
    UInt32 wiresharkPort = 0;
    struct hostent *hp;
    UChar8 line[MAX_LINE_SIZE] = {0}, temp[MAX_LINE_SIZE];
    file_p = fopen("phySimConfig.txt", "r");
    if (file_p == PNULL)
    {
        LTE_LOG(LOG_CRITICAL,PNULL,"File opening error, provide correct file name.");
        return;
    }
    bzero(&(wiresharkAddr_g),sizeof(wiresharkAddr_g));
/* SPR 609 changes start */
    while ( fgets((char *)line,MAX_LINE_SIZE,file_p) != NULL)
/* SPR 609 changes end */
    {
        if ( '#' == line[0])
            continue;
        memset(temp,0,MAX_LINE_SIZE);
/* SPR 609 changes start */
        length = strlen((const char *)line);
        if(sscanf((const char *)line,"wiresharkIP =%s",temp))
/* SPR 609 changes end */
        {
            printf("wireshark IP %s\n",temp);
            hp = (struct hostent *) gethostbyname((char *)temp);
            if (hp == (struct hostent *)0) 
        {
                printf("Unknown host\n");  
                exit(1); 
            }
            memcpy((void*)&(wiresharkAddr_g.sin_addr), (void*)hp->h_addr, hp->h_length);
        }
        else if(sscanf((const char *)line,"wiresharkPort =%d",&wiresharkPort) ==0);
    }
    fclose(file_p);
    initWireSharkSocket(wiresharkPort);
}

/****************************************************************************
 * Function Name  : setSocketBlockingEnabled
 * Inputs         : socketfiledecriptor, blocking (0 - Non Blocking, 1-Blocking)
 * Outputs        : NONE
 * Returns        : status (0-Failure,1-Successful)
 * Variables      :
 * Description    : to set the specific socket blocking or non blocking
 ****************************************************************************/
static UInt8 setSocketBlockingEnabled(UInt8 sockfd, UInt8 blocking)
{
    SInt32 fileflags;
    fileflags = fcntl(sockfd, F_GETFL,0);
    if(fileflags == -1)
    {
        ltePanic("fcntl F_GETFL");
        return 0;
    }
    fileflags = blocking ? (fileflags & ~FNDELAY) : (fileflags |FNDELAY);
    return (fcntl(sockfd, F_SETFL, fileflags)== 0) ? 1 : 0 ;
}


/****************************************************************************
 * Function Name  : initSocketAddress
 * Inputs         : sockaddr_in, socketFileDescriptor,Port Number
 * Outputs        :
 * Returns        : status (0-Failure,1-Successful)
 * Variables      :
 * Description    : to intialize the Socket address with specific socket file
 descriptor and specific port
 ****************************************************************************/

static UInt8 initSocketAddress(struct sockaddr_in *s,SInt32 *sockfd,UInt16 port)
{
    struct hostent *host_p;
    host_p= (struct hostent *) gethostbyname((char *)l2ModuleIP);
    if((*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        LTE_LOG(LOG_DETAIL,PNULL,"creating socket error\n");
        return 0;
    }
    (*s).sin_family = AF_INET;
    (*s).sin_port = htons(port);
    (*s).sin_addr = *((struct in_addr *)host_p->h_addr);
    bzero(&((*s).sin_zero),8);
    return 1;
}

static UInt8 initSocketAddressTX(struct sockaddr_in *s,SInt32 *sockfd,UInt16 port)
{
    struct hostent *host_p;
    host_p= (struct hostent *) gethostbyname((char *)macIPAddress_g);
    if((*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        LTE_LOG(LOG_DETAIL,PNULL,"creating socket error\n");
        return 0;
    }
    (*s).sin_family = AF_INET;
    (*s).sin_port = htons(port);
    (*s).sin_addr = *((struct in_addr *)host_p->h_addr);
    bzero(&((*s).sin_zero),8);
    return 1;
}

/****************************************************************************
 * Function Name  : init 
 * Inputs         : 
 *                  
 *                  
 * Outputs        : 
 * Returns        : 
 * Description    : 
 *                  
 ****************************************************************************/
#ifdef UE_SIM_SPLIT
/****************************************************************************
 * Function Name  :  processFapiDLMsg 
 * Inputs         :  void * 
 * Outputs        :  
 * Returns        :  
 * Description    : This function is the starting point of the ulReceiverThread. 
 *                  This Thread will be listenign on fast path interface or MsgQ
 *                  and receive the data from the Peer MAC. 
 *                   
 ****************************************************************************/
void processFapiDLMsg ( void *arg)
{
    UInt8* recvDataBuffer_p = PNULL;
    static fapiMsgIdStruct msgIdArr[MAX_MSG_ID] = {{0}};
    UInt64 numPdu = 0;
    macPhyDataNode *data_p = PNULL;
    UInt8 sfEvenFlag = FALSE;
#if 0
    if ( SF_g % 2 == 0 )
    {
        numPdu = sQueueCount(&dlMsgStoreQueue[SF_g % 2]);
	sfEvenFlag = TRUE;
    }
    else
#endif
    {
        numPdu = sQueueCount(&dlMsgStoreQueue);
    }
    //fprintf(stderr," sf_g = %d index = %d\n", SF_g, (SF_g % 2));	
    while (numPdu--)
    {
#if 0
	if ( TRUE == sfEvenFlag )
	{
        fprintf(stderr," pop node from even queue SF_g = %d SFN_g = %d\n", SF_g, SFN_g);
	    data_p = (macPhyDataNode*) popNode (&dlMsgStoreQueue[SF_g % 2]);
	}
	else
#endif
	{
		//fprintf(stderr," pop node from queue index = %d SF_g = %d SFN_g = %d\n", SF_g % 2,SF_g, SFN_g);
	    data_p = (macPhyDataNode*) popNode (&dlMsgStoreQueue);
	}
    if (NULL != data_p)
	{
	    /* Parse the buffer header */
	    if (NULL == (recvDataBuffer_p = (UInt8*)data_p->msg))
	    {
	        freeMemPool (data_p);
		data_p = PNULL;
		continue;
	    }
	    /* call the fapi processing api function */
	    fapiReadMacMsg(recvDataBuffer_p,msgIdArr);
	    if(data_p->msg)
	    {
	        freeMemPool (data_p->msg);
		data_p->msg = PNULL;
	    }
	    if(data_p)
	    {
	        freeMemPool(data_p);
		data_p = PNULL;
	    }
        }       
    }
}
/****************************************************************************
 * Function Name  :  initIPCInterfaceUEPHYAndUEMAC 
 * Inputs         :  void * 
 * Outputs        :  
 * Returns        :  
 * Description    : This function will init the socket between UE PHY & UE MAC. 
 ****************************************************************************/
#define UE_PHY_SFI_TX_RX_PORT 22059
void initIPCInterfaceUEPHYAndUEMAC( void )
{
    initSocketAddress(&ueMacLayerAddr_g,&rxUeMacSocketFD_g, UE_PHY_SFI_TX_RX_PORT);

    if ( bind(rxUeMacSocketFD_g, (struct sockaddr*)&ueMacLayerAddr_g,
                sizeof(SockAddr)) == -1)
    {
        ltePanic("Bind Failed UE MAC SFI RX PORT = %d", UE_PHY_SFI_TX_RX_PORT);
        return;
    }
    initSocketAddress(&uePhyLayerAddr_g, &txUePhySocketFD_g, UE_PHY_SFI_TX_RX_PORT);
}
#endif
// code require for only MAC testing
void pushEntryInOnlyBsrQueue(  UInt8  *bsrPdu_p,
                               UInt16 bsrPduLen,
                               UInt16 crnti,
                               UInt8  sendSF,
                               UInt16 sendSFN,
                               UInt8  qIndex,
                               UInt16 pduChoice,
                               UInt8  cqiMode
                           ) 
{
    if ( !bsrPdu_p )
        ltePanic("Bsr PDU NULL Recvd in pushEntryInOnlyBsrQueue\n");

    UlDataQueueNode *uldataNode_p = 
        (UlDataQueueNode *) getMemFromPool(sizeof(UlDataQueueNode), PNULL);
    if (!uldataNode_p)
    {
        ltePanic("getMemPool Failed in pushEntryInOnlyBsrQueue\n");
    }
    uldataNode_p->SF            = sendSF;
    uldataNode_p->crnti         = crnti;
    uldataNode_p->SFN           = sendSFN;
    uldataNode_p->PDUChoice     = pduChoice;
    uldataNode_p->msg3Flag      = FALSE;
    uldataNode_p->qIndex        = qIndex;


    uldataNode_p->pdu.PDULength = bsrPduLen;
    uldataNode_p->pdu.Pdudata_p = bsrPdu_p;
    pushNode(&OnlyBsrQueueNode_g[qIndex], &(uldataNode_p->ulDataAnchor));
    
    // _ULHARQ_RETX_  start  

#if 0
    if(uldataNode_p->SFN != SFN_g)
    {
        if ((!((uldataNode_p->SFN==0)&& (SFN_g==(MAX_SFN -1)))) && (!((uldataNode_p->SFN==(MAX_SFN -1) )&& (SFN_g==0))))
        {
            currentSFNSF=SFN_g * 10 + SF_g;
            sendSFNSF= uldataNode_p->SFN * 10 + qIndex;
        }
        else if((uldataNode_p->SFN==0)&& (SFN_g==(MAX_SFN -1)))
        {
            currentSFNSF=0;
            sendSFNSF=(MAX_SFN - 1);
        }
        else if((uldataNode_p->SFN==(MAX_SFN -1) )&& (SFN_g==0))
        {
            currentSFNSF=(MAX_SFN - 1);
            sendSFNSF=0;
        }
    }
    else
    {
        currentSFNSF=SF_g;
        sendSFNSF=qIndex;
    }
    //fix for sync.

    fprintf(stderr,"In [%s] printLogCnt = %lld currentSFN %d currentSF %d\n",__func__,++printLogCnt,SFN_g,SF_g);
    fprintf(stderr,"In [%s] printLogCnt = %lld qIndex %d sendSF %d  sendSFN  %d\n",__func__,++printLogCnt, qIndex,sendSF,uldataNode_p->SFN);
    fprintf(stderr,"In [%s] printLogCnt = %lld currentSFNSF %d sendSFNSF %d\n",__func__,++printLogCnt,currentSFNSF,sendSFNSF);

    if (currentSFNSF<sendSFNSF)
    {   

        pushNode(&OnlyBsrQueueNode_g[qIndex], &(uldataNode_p->ulDataAnchor));
        //ueContextInfo_p->ulDataNode_p[subFrame] =  uldataNode_p;
    }
        // _ULHARQ_RETX_  end  
#endif
}

/****************************************************************************
 * Function Name  : initSHMInterface 
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    :
 *
 ****************************************************************************/
void initSHMInterface()
{
#ifdef LTE_MAC_UT_LOG
    LTE_LOG(LOG_INFO,&MACModuleLogDetail_g, "[%s] Entry ", __func__ );
#endif

    UInt32 uid=getuid();
    
    UInt32 shmMsgDlID= (uid<<16)|SHM_MSG_DL_ID;
    UInt32 shmMsgUlID= (uid<<16)|SHM_MSG_UL_ID;

#ifdef LTE_MAC_UT_LOG
    LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"Creating Shared Message Queue for Downlink");
#endif
    if ((shMsgQIdDL_g = msgget(shmMsgDlID, IPC_CREAT | IPC_EXCL|0666 )) < 0)
    {
        if(errno == 17)
        {
            shMsgQIdDL_g = msgget(shmMsgDlID, IPC_CREAT|0666 );

            if((msgctl(shMsgQIdDL_g,IPC_RMID,NULL)) ==0)
            {
                if ((shMsgQIdDL_g = msgget(shmMsgDlID, IPC_CREAT | IPC_EXCL|0666 )) < 0)
                {
#ifdef LTE_MAC_UT_LOG
                    LTE_LOG(LOG_INFO,&MACModuleLogDetail_g," msgget errno =%d",errno);
#endif
                    ltePanic("msgget failed");

                }
            }
            else
            {
#ifdef LTE_MAC_UT_LOG
                LTE_LOG(LOG_INFO,&MACModuleLogDetail_g," msgctl errno =%d",errno);
#endif
                ltePanic("msgctl failed");
            }
        }
        else
        {
#ifdef LTE_MAC_UT_LOG
            LTE_LOG(LOG_INFO,&MACModuleLogDetail_g," msgget errno =%d",errno);
#endif
            ltePanic("msgget failed");
        }
    }

#ifdef LTE_MAC_UT_LOG
    LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"Creating Shared Message Queue for Uplink");
#endif
    if ((shMsgQIdUL_g = msgget(shmMsgUlID, IPC_CREAT | IPC_EXCL|0666 )) < 0)
    {
        if(errno == 17)
        {
            shMsgQIdUL_g = msgget(shmMsgUlID, IPC_CREAT|0666 );

            if((msgctl(shMsgQIdUL_g,IPC_RMID,NULL)) ==0)
            {
                if ((shMsgQIdUL_g = msgget(shmMsgUlID, IPC_CREAT | IPC_EXCL|0666 )) < 0)
                {
#ifdef LTE_MAC_UT_LOG
                    LTE_LOG(LOG_INFO,&MACModuleLogDetail_g," msgget errno =%d",errno);
#endif
                    ltePanic("msgget failed");

                }
            }
            else
            {
#ifdef LTE_MAC_UT_LOG
                LTE_LOG(LOG_INFO,&MACModuleLogDetail_g," msgctl errno =%d",errno);
#endif
                ltePanic("msgctl failed");
            }
        }
        else
        {
#ifdef LTE_MAC_UT_LOG
            LTE_LOG(LOG_INFO,&MACModuleLogDetail_g," msgget errno =%d",errno);
#endif
            ltePanic("msgget failed");
        }
    }

#ifdef LTE_MAC_UT_LOG
    LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"[%s] Exit ", __func__ );
#endif

}

#ifndef UE_SIM_SPLIT
/****************************************************************************
 * Function Name  : phyTimerThread
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    : This is the starting point for the timer thread. 
 *                  It will increment the SFN, SF and global tick once 
 *                  register with perr MAC
 *
 ****************************************************************************/

void * phyTimerThread(void * arg)
{
    UInt8 cellIndex = 0;

/*
**
**  The funtionality for usage of timer based on 
**  pselect or nanoSleep is a COMPILE TIME choice. 
**
*/
#ifndef NANO_SLEEP_TIMER
    struct timespec timer;
    SInt32 retVal = 0;
    timer.tv_nsec = tickValue_g;
    //timer.tv_nsec = 1000000;
    timer.tv_sec = 0;
#else
    struct timespec t1, t2;
    /* SPR 15909 fix start */
    tickType_t tempTickValue = tickValue_g;
    /* SPR 15909 fix end */
    SInt8  timer_ret = 0;
#endif

/*
**  
**  The Following Binding of CORE is valid only when
**  the REAL TIME THREAD EXECUTION is not defined, as
**  in the other case we binding to core is applied just
**  after creation of thread.  
**  
*/
    //#ifndef REAL_TIME_THREAD_SCHEDULING
    fprintf(stderr, "\n=========== PHY Timer Thread Binding =========================\n");
    THREAD_TO_BIND_SET_CORE_NO( PhyTimerThread_g , uePhyTimerThreadCoreNum_g);
    fprintf(stderr, "\n=========== Verify PHY Timer Thread Binding =========================\n");
    THREAD_TO_BIND_GET_CORE_NO(PhyTimerThread_g);
    fprintf(stderr, "\n==============================================================\n");
    threadSetRtPriority(SCHED_FIFO, 45);
    fprintf(stderr, "in PhyTimerThread before while....!!!\n");
#if 0
    while (  1)
    {  
       if ( macRegFlag_g  == REG_SUCCESS)
       {
         break;
       }
#ifdef FLOW_MONITOR_SECTION
       flowMon_CheckThreads();
       flowMon_StartFlow();
#endif
       //fprintf(stderr,"in PhyTimerThread inside while....!!!\n");
    }
    /*
    while (!macRegFlag_g)
    {
          UInt32 z = 0;
          z++;
          fprintf(stderr, "in PhyTimerThread inside while....!!!\n");
    }
    */
#endif
    while(1)
    {
//manish -> checking whether thread is taking more than 80 % of the defined TTI        
#ifndef NANO_SLEEP_TIMER
        rlcUeSimLoop_g = FALSE;
        retVal = pselect(0, NULL, NULL, NULL, &timer, NULL);
#else
        rlcUeSimLoop_g = FALSE;
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
            timer_ret = nanosleep(&t1, &t2);
        }while(timer_ret<0);
#endif

        /*Shashank */
        /*
           semPost(&ulDispatcherThreadSem_g);
         */
        /* +CA_NA*/
            for (cellIndex = 0; cellIndex < numCells_g ; cellIndex++)
            semPost(&execSchedulerThreadSem_g[cellIndex]);
        /* -CA_NA*/
#if 0        
        SF_g++;
        if ( SF_g == MAX_SUBFRAME )
        {
            SF_g = 0;
            SFN_g++ ;
            
            if ( SFN_g == MAX_SFN )
            {
                SFN_g = 0;
            }
        }
        globalTTITickCount_g++;
#endif       
       // semPost(&ulDispatcherThreadSem_g);
        
       // rlcUeSimLoop_g = TRUE;
     //   semPost(&rlcPdcprunThreadSem_g);
        
     //   semPost(&ulProcessThreadSem_g);
        //semPost(&rlcPdcprunThreadSem_g);
        
        /* Update the SIB flag */
/*        if(globalTTITickCount_g % 80 == 0)
        {
            sibMsgFlag_g = TRUE;
            siRntiMsgFlag_g = TRUE;
        }

        if(mibToBeRecievedPrevious_g == TRUE)
        {
            mibToBeRecievedFlag_g = FALSE;
        }

        if(SFN_g % 4 == 0)
        {
            mibToBeRecievedFlag_g = TRUE;
            mibToBeRecievedPrevious_g = TRUE;
        }
*/        
    }
}
#endif


#if 0
UInt8 fillPhyPortInfo(UInt16 *rxPort, 
                      UInt16 *txPort,
                      UChar8 *macIPAddress,
                      UChar8 *phyIPAddress
                    )
{
    FILE *fp;
    UInt32 port=0;
    char buf[101] = {0,};
    UInt32 flag = 0;
    char portConfigFileName[256] ;
    GET_LAYER2_PORT_CONFIG_FILE(portConfigFileName);
    fp = fopen(portConfigFileName,"r");
    if(fp == NULL) {
        ltePanic("Fail to open the port config file lteLayer2PortConfig.cfg");
    }
    /*read the port numbers */
    while( fgets(buf,100,fp))
    {
        if(sscanf(buf,"PHY_RX_PORT =%d",&port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(rxPort) = (UInt16)port;
                flag = (flag << 1) | 1;
            }
        }
        else if(sscanf(buf,"PHY_TX_PORT =%d", &port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(txPort) = (UInt16)port;
                flag = (flag << 1) | 1;
            }
        }
        else if(sscanf(buf,"MAC_IP_ADDRESS=%s",macIPAddress) > 0)
        {
                   flag = (flag << 1) | 1;
        }
        else if(sscanf(buf,"PHY_IP_ADDRESS=%s",phyIPAddress) > 0)
        {
                flag = (flag << 1) | 1;
        }

    }
    if(flag ==  0xf )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#endif

/****************************************************************************
 * Function Name  : main
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    :
 *
****************************************************************************/
#if 0
int main(int argc, char *argv[])
{
    UInt64 ret =0;
    UInt16 rxPort = 0,txPort = 0;

    if (! ( fillPhyPortInfo(&rxPort, 
                            &txPort,
                            macIPAddress_g,
                            phyIPAddress_g
                            ) ) )
    {
        rxPort = PHY_PORT;
        txPort = UPLK_PORT;
        strcpy(macIPAddress_g,"127.0.0.1");
        strcpy(phyIPAddress_g,"127.0.0.1");
    }
    
    // init the global queues, structures, semaphores
    init(rxPort, txPort);
    // init the MAC emulator
    initMAC();
    // init rlc simulator
    rlcInitDbReq();
    
    submenu();
/*Creating phy Timer Thread For SubFrame Indication */
    ret = threadCreate(&PhyTimerThread_g,THREAD_CREATE_DETACHED,
            THREAD_SCHED_DEFAULT, 
            0,
            THREAD_INHERIT_SCHED_DEFAULT,
            THREAD_SCOPE_DEFAULT,
            phyTimerThread,
            (void *) NULL );
    if ( ret != 0)
        ltePanic("Error in creating Phy timer thread");

/* Creating Dispatcher Thread for Send UL Control and UL Data Msg to MAC Layer*/
    ret = threadCreate(&UlDispatcherThread_g,THREAD_CREATE_DETACHED,
            THREAD_SCHED_DEFAULT, 
            0,
            THREAD_INHERIT_SCHED_DEFAULT,
            THREAD_SCOPE_DEFAULT,
            ulDispatcherThread,
            (void *) NULL );
    if ( ret != 0)
        ltePanic("Error in creating ULDispatcher thread");

/* Creating Receiver thread for receiving the DL Control & DL Data Msg */
    ret = threadCreate(&ulReceiverThread_g,THREAD_CREATE_DETACHED,
            THREAD_SCHED_DEFAULT, 
            0,
            THREAD_INHERIT_SCHED_DEFAULT,
            THREAD_SCOPE_DEFAULT,
            ulReceiverThread,
            (void *) NULL );
    if ( ret != 0)
        ltePanic("Error in creating DL Receiver thread");

/* Creating ulDataProcessThread for process the UL Data and make UL data for MAC*/
    ret = threadCreate(&ulDataProcessThread_g,THREAD_CREATE_DETACHED,
            THREAD_SCHED_DEFAULT,
            0,
            THREAD_INHERIT_SCHED_DEFAULT,
            THREAD_SCOPE_DEFAULT,
            ulDataProcessThread,
            (void *) NULL );
    if ( ret != 0)
        ltePanic("Error in creating DL Receiver thread");

    // input from user 
    rxTxFromPhySim();

        return 0;

}

#endif


//Pradeep

/*****************************************************************************
 * Function Name  : calculateSubHeaderLength 
 * Inputs         :
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *                  
 *****************************************************************************/
UInt8* calculateSubHeaderLength(UInt8 *headerPtr_p,
                                        UInt32 *len_p,
                                        UInt32 *dataPtrPos_p)
{
    /*check whether length is greater than 128 bytes 
     *or less than 128 bytes
     */

    *len_p = 0;
    
    /*Reffer section 6.1.2 in 36321-850.doc*/
    if (*headerPtr_p & 0x80)
    {
        /*length is greater than 128 bytes*/
        *len_p = *headerPtr_p++ & 0x7F;
        *len_p <<= 8;

        *len_p |= *headerPtr_p;
        *dataPtrPos_p = *dataPtrPos_p+3;
    }
    else
    {
        /*length is less than 128 bytes*/
        *len_p = *headerPtr_p & 0x7F;
        *dataPtrPos_p = *dataPtrPos_p + 2;
    }


    return headerPtr_p;
}


/*****************************************************************************
 * Function Name  : processDLDemuxData
 * Inputs         :  
 * Outputs        : None
 * Returns        : None
 * Description    : This function segregates the data based on LC id and 
 *                  call dlDataDeMultiplexAndSend() to break data and send to
 *                  RLC
 *****************************************************************************/
/* CA_NA */                    
UInt32 processDLDemuxData( UInt8  *data_p,
                           void   *zeroBufCpy_p,
                           UInt32 dataLen,
                           UInt16 ueIndex,
                           UInt8 txCellIndex)
/* CA_NA */                    
{
    UInt8  *dataPtr_p       = PNULL;
    UInt8  *headerPtr_p     = PNULL;  
    UInt32 extnFlag         = FALSE;
    UInt32 startPaddingFlag = FALSE;
    UInt32 sucessDecodeFlag = TRUE;
    UInt32 lcId        = 0;
    UInt32 pos         = 0;
    UInt32 dataPtrPos  = 0;
    UInt32 remainingPayloadLength = 0;
    /* TA_CHG */
    UInt8  taPresent = 0; 
    /* TA_CHG */
    /*phase-3 changes start*/
    UInt8 scellActDeactFlag = 0;
    UInt8 scellMacCePresent = 0;
    /*phase-3 changes end*/
    /** SPR 3657 Changes Start **/
    DemuxDLDataBase demuxData[MAX_NUM_UL_PKTS] = {{0}};
    /* DemuxDLDataBase demuxData[MAX_NUM_CHANNELS] = {{0}}; */
    /** SPR 3657 Changes End **/
    DemuxDLDataBase *demuxData_p = PNULL;
    /* SPR FREE changes start */
    SInt32 extDataLen = 0;
    /* SPR FREE changes end */
    ueContextForUplink* ulUEContext_p = PNULL;
    
    dataPtr_p   = (UInt8 *)data_p;
    headerPtr_p = (UInt8 *)data_p;

    demuxData_p            = &demuxData[0];
    remainingPayloadLength = dataLen;

    do
    {
        /*Reffer section 6.1.2 and 6.2 of 36321-850.doc*/
        /* <------ 1 Octet ------>
         * +++++++++++++++++++++++
         * |R |R |E |   LCID     |
         * +++++++++++++++++++++++
         * R    - Reserved (1 bit)
         * E    - Extention bit (1 bit)
         * LCID - Logical channel id (5 bits)
         */
        /*Subheader with variable length*/
        /*Reffer section 6.1.2 and 6.2 of 36321-850.doc*/
        /* ++++++++++++++++++++++
         * |R|R|E|   LCID       | Oct 1
         * ++++++++++++++++++++++
         * |F|       L          | Oct 2 
         * ++++++++++++++++++++++
         * R    - Reserved (1 bit)
         * E    - Extention bit (1 bit)
         * LCID - Logical channel id (5 bits)
         * L    - Length 
         */
        /* ++++++++++++++++++++++
         * |R|R|E|   LCID       | Oct 1
         * ++++++++++++++++++++++
         * |F|       L          | Oct 2 
         * ++++++++++++++++++++++
         * |         L          | Oct 3
         * ++++++++++++++++++++++
         * R    - Reserved (1 bit)
         * E    - Extention bit (1 bit)
         * LCID - Logical channel id (5 bits)
         * L    - Length 
         */

        lcId = (*dataPtr_p & 0x1F);

        if (*dataPtr_p & UL_PDU_EXTN_FLAG)
        {
            extnFlag = TRUE;
        }
        else
        {
            extnFlag = FALSE;
        }

        switch(lcId)
        {
            case LCID_1:
            case LCID_2:
            case LCID_3:
            case LCID_4:
            case LCID_5:
            case LCID_6:
            case LCID_7:
            case LCID_8:
            case LCID_9:  
            case LCID_10:
            {
                (demuxData_p + pos)->lchId = lcId;

                if (((TRUE == startPaddingFlag) && (FALSE == extnFlag)) ||
                     (FALSE == extnFlag))
                {
                    /*This is the case with 1/2 byte padding at the start of 
                     *PDU or no padding. So the last subheader is of 1byte
                     *i.e no length field.
                     */
                    dataPtrPos++;
                    (demuxData_p + pos)->length = 
                        remainingPayloadLength - dataPtrPos;
                }
                else if (TRUE == extnFlag)
                {
                     /*Increment the pointer to point to length field*/
                     dataPtr_p++;
                     dataPtr_p = calculateSubHeaderLength(dataPtr_p, 
                                             &(demuxData_p + pos)->length,
                                             &dataPtrPos);
                }
                /* SPR FREE changes start */
                if(extDataLen < (demuxData_p + pos)->length)
                {
                    extDataLen = (demuxData_p + pos)->length;
                }            
                /* SPR FREE changes end */
                
                remainingPayloadLength = 
                   remainingPayloadLength - (demuxData_p + pos)->length;
                pos++;
            }
            break;

            /* TA_CHG */
            case TA_LCID:
                //printf("TA Element received\n");
                taPresent = 1;
                /* SPR 1380 Start*/
                dataPtrPos +=2;
                /* SPR 1380 End*/
                break;
            /* TA_CHG */
            /*ca phase-3 changes startt*/    
            case SCELL_LCID:
                scellMacCePresent = 1;
                dataPtrPos +=2;
                break;
            /*ca phase-3 changes end*/    
            case PADDING_LCH:
            {
                if (FALSE == extnFlag)
                {
                    dataPtrPos++;
                    /*case where padding is of 1 byte and present 
                     *after the last MAC PDU subheader
                     */
                    if(remainingPayloadLength > dataPtrPos)
                    {
                        (demuxData_p + pos)->length = 
                                remainingPayloadLength - dataPtrPos ;
                    }
                    else
                    {
                        (demuxData_p + pos)->length = 
                                dataPtrPos - remainingPayloadLength;
                    }

                    remainingPayloadLength = 
                           remainingPayloadLength - (demuxData_p + pos)->length;
#ifdef ZERO_BUFFER_COPY_1
#if 0
                    void *dst_p = PNULL;
                    SPLIT_ZERO_BUFFER_COPY(zeroBufCpy_p, dst_p,
                                               dataLen - (demuxData_p + pos)->length);
                    msgFree(dst_p);
#endif
                    /*setting len to 0 which is required in splitting*/
                    (demuxData_p + pos)->length = 0;
#endif
                }
                else
                {
					startPaddingFlag = TRUE;
                    dataPtrPos++;
                }
            }
            break;
            
            default:
                //printf("processDemuxData: Invalid lcId=%d\n",lcId);
            break;     
        }
        dataPtr_p++;
    }while ((TRUE == extnFlag) && (TRUE == sucessDecodeFlag));

#if 0
    /*now dataPtr_p reaches the 1st SDU data and header points the 1st MAC 
     *sub-header
     */
    if (sucessDecodeFlag)
    {
        UInt32 dataLenExlHeader = 0;
        UInt32 i = 0;

        /*Check the length received in the subheader to 
         *avoid segmentation fault
         */
        /*dataPtrPos incremented one byte extra*/
        dataLenExlHeader = dataLen - (dataPtrPos - 1);
        for (i = 0; i < pos; i++)
        {
            if ((demuxData_p + i)->length > dataLenExlHeader)
            {
                sucessDecodeFlag = 0;
                break;
            }
            dataLenExlHeader -= (demuxData_p + i)->length;
        }    
    }

    if (FALSE == sucessDecodeFlag)
    {
        return MAC_FAILURE;
    }
#endif

    /*ca phase-3 changes start*/
    if (1 == scellMacCePresent)
    { 
        ulUEContext_p = uplinkContextInfoMap_g[ueIndex];
#ifdef UT_TESTING
        fprintf(stderr, "Value of scellMacCePresent CCE = 0x%x\n", *dataPtr_p);      
#endif
        scellActDeactFlag = (*dataPtr_p & SCELL_ACT_DEACT_MASK);
        if (scellActDeactFlag)
        {
            ulUEContext_p->carrierAggrConfig.scellInfo[1].scellActDeactFlag = scellActDeactFlag;
            ulUEContext_p->carrierAggrConfig.scellInfo[1].scellDeactivationExpirytti = 0; 
        }
#if 0        
        //rajni - temp fix
        ulUEContext_p->carrierAggrConfig.scellCount++;
#ifdef TDD_CONFIG
        ulUEContext_p->ackNackMode = CHANNEL_SELECTION;
#endif       
#endif
        dataPtr_p++;
    }
    /* TA_CHG */
    if (1 == taPresent)
    {
#ifdef UT_TESTING
        fprintf(stderr, "Value of TA CCE = 0x%x\n", *dataPtr_p);
#endif
        dataPtr_p++;
    }
    /*ca phase-3 changes end*/
    /* TA_CHG */
    /* SPR FREE changes start */
    if(extDataLen > 0)
    {        
    /* SPR FREE changes end */
/* CA_NA */                    
    dlDataDeMultiplexAndSend(demuxData, 
                             zeroBufCpy_p,
                             dataPtr_p,
                             pos, 
                             dataPtrPos,
                             ueIndex,
                             FAPI_DLSCH_PDU,
                             txCellIndex);
/* CA_NA */                    
    /* SPR FREE changes start */
    }
    /* SPR FREE changes end */
#ifdef UT_TESTING
    else if(extDataLen == 0)
    {    
        lteWarning("Length received from ENB MAC is ZERO [%s] \n", __func__ );
    }    
    else 
    {    
        lteWarning("Length received from ENB MAC is Negative [%s] \n", __func__ );
    }    
#endif
#ifdef ZERO_BUFFER_COPY_1
    msgFree(zeroBufCpy_p);
#endif
    return 1;
}



/*****************************************************************************
 * Function Name  : dlDataDeMultiplexAndSend
 * Inputs         : demuxData_p - Pointer contains the LCID and length of data 
 *                        received per lcid, in the sequence received in UL pdu.
 *                  origDataPtr_p - Points to UL pdu received (including header)
 *                  dataPtr_p - Points to data section in UL pdu (no header)
 *                  ueIndex - Index corresponds to array of UE context
 *                  rcvdRNTI - received RNTI value in UL message
 *                  pos - contains the number of lcid data received in UL pdu
 *                  dataPtrPos - Position of the dataPtr_p position
 *                  ttiCounter - TTI in which UL message received
 *                  thdIndex - Thread index
 *                  dlDelay - DL delay
 * Outputs        : None
 * Returns        : None
 * Description    : This function demultiplex the data received from PHY and
 *                  send to RLC or RRC based on the LC ID data. It also fill
 *                  container's BSR queue and update the UL context based 
 *                  on the received BSR 
 *****************************************************************************/
/* CA_NA */                    
void dlDataDeMultiplexAndSend(DemuxDLDataBase *demuxData_p,
                                     UInt8   *origDataPtr_p,
                                     UInt8   *dataPtr_p,
                                     UInt32  pos,
                                     UInt32  dataPtrPos,
                                     UInt16  ueIndex,
                                     UInt8   pduType,
                                     UInt8 txCellIndex)
/* CA_NA */                    
{
    UInt8 cellCount = 0;

    UInt32 rlcFlag      = FALSE;
    UInt32 idx          = 0;        
    UInt32 lcIdx     = 0;
    UInt32 lchId     = 0;
    UInt32 length    = 0;
    RLCUplinkData         *dataToRlc_p = PNULL;        
    RLCUplinkDataInfo     *rlcLCIdData_p      = PNULL;
#ifdef ZERO_BUFFER_COPY_1
    void *org_p = (void *) origDataPtr_p;
#endif
#ifdef LTE_EMBMS_SUPPORTED
    UInt16 areaIndex;
    if(pduType==FAPI_MCH_PDU)
    {
        areaIndex=ueIndex;
    }
#endif
    for (idx=0; idx < pos; idx++)
    {
        lchId  = (demuxData_p + idx)->lchId;
        length = (demuxData_p + idx)->length;
        /*find logical channel ID*/
        if(pduType == FAPI_DLSCH_PDU && (lchId>10 || lchId==0))
        {
            ltePanic("Invalid LC ID:%d received for dlsch pdu\n",lchId);
        }   
        switch (lchId)
        {
            case LCID_0:
            case LCID_1:
            case LCID_2:
            case LCID_3:
            case LCID_4:
            case LCID_5:
            case LCID_6:
            case LCID_7:
            case LCID_8:
            case LCID_9:  
            case LCID_10:
            case LCID_11:
            case LCID_12:
            case LCID_13:
            case LCID_14:
            case LCID_15:
            case LCID_16:
            case LCID_17:
            case LCID_18:
            case LCID_19:
            case LCID_20:
            case LCID_21:
            case LCID_22:
            case LCID_23:
            case LCID_24:
            case LCID_25:
            case LCID_26:
            case LCID_27:
            case LCID_28:
            {
                /*data arrrived for RLC*/
                if (PNULL == dataToRlc_p)
                {
                    dataToRlc_p = (RLCUplinkData*)getMemFromPool(
                                        (UInt32)sizeof(RLCUplinkData),PNULL);
                    if (PNULL == dataToRlc_p)
                    {
                        ltePanic("Unable to allocate memory\n");
                    }
                }

                {
                    UInt8 *rlcData_p = NULL;

#ifdef ZERO_BUFFER_COPY_1
                    SPLIT_ZERO_BUFFER_COPY(org_p, rlcData_p, 
                                           dataPtrPos);
                    org_p = (void *)rlcData_p;
                    dataPtrPos = length;
#else
/*                    
                    rlcData_p = getMemFromPool(length, PNULL);
                    memCpy(rlcData_p, dataPtr_p,length);
                    dataPtrPos = dataPtrPos + length;
*/
            /* SPR FREE changes start */
            //rlcData_p = (UInt8 *)msgAlloc(PNULL, 0, 0, 0);
            rlcData_p = (UInt8 *)msgAlloc(PNULL, length, 0, 0);
            /* SPR FREE changes end */

            if ( PNULL == rlcData_p) 
            {
                ltePanic("#########msgAlloc failed in %s",__FUNCTION__);
            }

            if(PNULL == msgInsert(rlcData_p, 0, dataPtr_p, length))
            {
               ltePanic("#########msgInsert failed in %s",__FUNCTION__);
            }

//printf("######## Length %d msgSize %d\n",length,msgSize(zcb_p, NULL));
#endif

                    rlcLCIdData_p = &dataToRlc_p->rlcDataArr[lcIdx];
                    
                    /* fill the struct for sending data to RLC*/
                    rlcLCIdData_p->rlcdataBuffer = rlcData_p;
                    rlcLCIdData_p->dataLength    = length;
                    rlcLCIdData_p->lcId          = lchId;
#ifdef LTE_EMBMS_SUPPORTED                    
                    if(mcchPduReceived_g==TRUE)
                    {
                       rlcLCIdData_p->lcId=LCID_31;
                       mcchPduReceived_g=FALSE;
                    }
                    if(rlcResetSN_g ==TRUE)
                    {
                       rlcLCIdData_p->resetSNFlag=TRUE;
                       rlcResetSN_g = FALSE;
                    }
                    else
                    {
                        rlcLCIdData_p->resetSNFlag=FALSE;
                    }
#endif                    
                    lcIdx++;
                    dataToRlc_p->numLCInfo = lcIdx;
                    rlcFlag = TRUE;
 
                    /*
                    if (lchId == 3 )
                    {
                      totBytes_g += length;
                    fprintf(stderr, "Data Received at Ue Sim for lcId  %d of length = %d to be sent to RLC totBytes = %d\n", lchId, length, totBytes_g);
                    }*/
//Pradeep                    
#if 0 //#ifdef UT_TESTING 
                    UInt32 u = 0;
                    UInt8 *x_p = NULL;
                    QSEGMENT segment;
                    fprintf(stderr, "Data Received at Ue Sim for lcIdx  %d of length = %d to be sent to RLC\n", lcIdx, length);
#ifdef ZERO_BUFFER_COPY_1
                    /*for zero buffer copy*/
                    msgSegNext(rlcData_p, 0, &segment);

                    x_p   = segment.base;
#endif
                    fprintf(stderr, "\n");

                    while(u < length)
                    {
                      fprintf(stderr, "0X%X ", x_p[u++]);
                    }
                    fprintf(stderr, "\n");
#endif


                }

                /*increment dataPtr_p by len so that dataPtr_p reaches 
                 *to start of next RLC SDU.
                 */
                dataPtr_p += length;
            }    
            break;

            default:
            {
                /*increment data pointer to point to next SDU*/
                dataPtr_p+=length;
                dataPtrPos = dataPtrPos + length;
            }    
            break;
        }
    }

#ifdef ZERO_BUFFER_COPY_1
    UInt8 *afterRlcBuff_p;

    SPLIT_ZERO_BUFFER_COPY(org_p, afterRlcBuff_p, 
            dataPtrPos);
    msgFree(afterRlcBuff_p);
#endif

#if 0    
    if (rlcLCIdData_p && rlcLCIdData_p->rlcdataBuffer)
    {
      if(msgSize(rlcLCIdData_p->rlcdataBuffer,NULL) != rlcLCIdData_p->dataLength)
      {
        fprintf(stderr,"MAC EMU: msgSegSize %d dataLen %d\n",msgSize(rlcLCIdData_p->rlcdataBuffer,NULL),rlcLCIdData_p->dataLength);
      }
    }
#endif
    /* +CA_NA */                    
    if (TRUE == rlcFlag)
    {
        /* need to pass cellIndex in this function */
        if(pduType == FAPI_DLSCH_PDU)
        {
            if (!macUeDataInd(ueIndex, dataToRlc_p /*txCellIndex*/))
                /* -CA_NA */                    
            {
                printf("[%s]: WARNING: macUeDataInd() returned failure\n",__func__);
            }
        }
#ifdef LTE_EMBMS_SUPPORTED        
        else if(pduType == FAPI_MCH_PDU)
        {
            if(!maceMBMSDataInd(areaIndex,dataToRlc_p,txCellIndex))
            {
                printf("[%s]: WARNING: macAreaDataInd() returned failure\n",__func__);
            }
        }
#endif        
    }
    
}

#ifdef LTE_EMBMS_SUPPORTED
/*****************************************************************************
 * Function Name  : processMBMSDemuxData
 * Inputs         :  
 * Outputs        : None
 * Returns        : None
 * Description    : This function segregates the data based on LC id and 
 *                  call dlDataDeMultiplexAndSend() to break data and send to
 *                  RLC
 *****************************************************************************/
/* CA_NA */                    
UInt32 processMBMSDemuxData( UInt8  *data_p,
                           void   *zeroBufCpy_p,
                           UInt32 dataLen,
                           UInt8 areaIndex,
                           UInt8 rxCellIndex)
/* CA_NA */                    
{
    UInt8  *dataPtr_p       = PNULL;
    UInt8  *headerPtr_p     = PNULL;  
    UInt32 extnFlag         = FALSE;
    UInt32 startPaddingFlag = FALSE;
    UInt32 sucessDecodeFlag = TRUE;
    UInt32 lcId        = 0;
    UInt32 pos         = 0;
    UInt32 dataPtrPos  = 0;
    UInt32 remainingPayloadLength = 0;
    DemuxDLDataBase demuxData[MAX_NUM_UL_PKTS] = {{0}};
    DemuxDLDataBase *demuxData_p = PNULL;
    SInt32 extDataLen = 0;
    
    dataPtr_p   = (UInt8 *)data_p;
    headerPtr_p = (UInt8 *)data_p;

    demuxData_p            = &demuxData[0];
    remainingPayloadLength = dataLen;

    do
    {
        /*Reffer section 6.1.2 and 6.2 of 36321-850.doc*/
        /* <------ 1 Octet ------>
         * +++++++++++++++++++++++
         * |R |R |E |   LCID     |
         * +++++++++++++++++++++++
         * R    - Reserved (1 bit)
         * E    - Extention bit (1 bit)
         * LCID - Logical channel id (5 bits)
         */
        /*Subheader with variable length*/
        /*Reffer section 6.1.2 and 6.2 of 36321-850.doc*/
        /* ++++++++++++++++++++++
         * |R|R|E|   LCID       | Oct 1
         * ++++++++++++++++++++++
         * |F|       L          | Oct 2 
         * ++++++++++++++++++++++
         * R    - Reserved (1 bit)
         * E    - Extention bit (1 bit)
         * LCID - Logical channel id (5 bits)
         * L    - Length 
         */
        /* ++++++++++++++++++++++
         * |R|R|E|   LCID       | Oct 1
         * ++++++++++++++++++++++
         * |F|       L          | Oct 2 
         * ++++++++++++++++++++++
         * |         L          | Oct 3
         * ++++++++++++++++++++++
         * R    - Reserved (1 bit)
         * E    - Extention bit (1 bit)
         * LCID - Logical channel id (5 bits)
         * L    - Length 
         */

        lcId = (*dataPtr_p & 0x1F);

        if (*dataPtr_p & UL_PDU_EXTN_FLAG)
        {
            extnFlag = TRUE;
        }
        else
        {
            extnFlag = FALSE;
        }

        switch(lcId)
        {
            case LCID_0:
            case LCID_1:
            case LCID_2:
            case LCID_3:
            case LCID_4:
            case LCID_5:
            case LCID_6:
            case LCID_7:
            case LCID_8:
            case LCID_9:  
            case LCID_10:
            case LCID_11:
            case LCID_12:
            case LCID_13:
            case LCID_14:
            case LCID_15:
            case LCID_16:
            case LCID_17:
            case LCID_18:
            case LCID_19:
            case LCID_20:
            case LCID_21:
            case LCID_22:
            case LCID_23:
            case LCID_24:
            case LCID_25:
            case LCID_26:
            case LCID_27:
            case LCID_28:
            case MAC_CE_LCID:
            {
                (demuxData_p + pos)->lchId = lcId;

                if (((TRUE == startPaddingFlag) && (FALSE == extnFlag)) ||
                     (FALSE == extnFlag))
                {
                    /*This is the case with 1/2 byte padding at the start of 
                     *PDU or no padding. So the last subheader is of 1byte
                     *i.e no length field.
                     */
                    dataPtrPos++;
                    (demuxData_p + pos)->length = 
                        remainingPayloadLength - dataPtrPos;
                }
                else if (TRUE == extnFlag)
                {
                     /*Increment the pointer to point to length field*/
                     dataPtr_p++;
                     dataPtr_p = calculateSubHeaderLength(dataPtr_p, 
                                             &(demuxData_p + pos)->length,
                                             &dataPtrPos);
                }
                /* SPR FREE changes start */
                if(extDataLen < (demuxData_p + pos)->length)
                {
                    extDataLen = (demuxData_p + pos)->length;
                }            
                /* SPR FREE changes end */
                
                remainingPayloadLength = 
                   remainingPayloadLength - (demuxData_p + pos)->length;
                pos++;
            }
            break;
            case PADDING_LCH:
            {
                if (FALSE == extnFlag)
                {
                    dataPtrPos++;
                    /*case where padding is of 1 byte and present 
                     *after the last MAC PDU subheader
                     */
                    if(remainingPayloadLength > dataPtrPos)
                    {
                        (demuxData_p + pos)->length = 
                                remainingPayloadLength - dataPtrPos ;
                    }
                    else
                    {
                        (demuxData_p + pos)->length = 
                                dataPtrPos - remainingPayloadLength;
                    }

                    remainingPayloadLength = 
                           remainingPayloadLength - (demuxData_p + pos)->length;
#ifdef ZERO_BUFFER_COPY_1
                    /*setting len to 0 which is required in splitting*/
                    (demuxData_p + pos)->length = 0;
#endif
                }
                else
                {
					startPaddingFlag = TRUE;
                    dataPtrPos++;
                }
            }
            break;
            
            default:
                //printf("processDemuxData: Invalid lcId=%d\n",lcId);
            break;     
        }
        dataPtr_p++;
    }while ((TRUE == extnFlag) && (TRUE == sucessDecodeFlag));


    if(extDataLen > 0)
    {        
        dlDataDeMultiplexAndSend(demuxData, 
                zeroBufCpy_p,
                dataPtr_p,
                pos, 
                dataPtrPos,
                areaIndex,
                FAPI_MCH_PDU,
                rxCellIndex);
    }
#ifdef UT_TESTING
    else if(extDataLen == 0)
    {    
        lteWarning("Length received from ENB MAC is ZERO [%s] \n", __func__ );
    }    
    else 
    {    
        lteWarning("Length received from ENB MAC is Negative [%s] \n", __func__ );
    }    
#endif
#ifdef ZERO_BUFFER_COPY_1
    msgFree(zeroBufCpy_p);
#endif
    return 1;
}
#endif

void pdcpElTxProcessMsgQ(void)
{
/* SPR 609 changes start */
	Char8* str_p = PNULL;
/* SPR 609 changes end */
	//    UInt8 buf[SOCKET_BUFFER_SIZE] = {0};
	UInt8 *buf = {0};
	UInt32 length = 0;
	UInt16 apiId = 0;
	UInt16 sourceModuleId = 0;
	UInt64  numPdu = 0;
	rrcOamMsgNode *rrcOamMsg_p = NULL;
	/* Read packet from socket */
	//shashank 
	//length = ReadFromSocket( buf, PDCP_MODULE_ID );
	/* check packet */
	numPdu = sQueueCount(&pdcpRrcOamMsgQ_g);

/* SPR 609 changes start */
	printf ("No of PDUs in pdcpRrcOamMsgQ_g Q [%lld]\n", numPdu);
/* SPR 609 changes end */
	while (numPdu--)
	{
		rrcOamMsg_p = (rrcOamMsgNode*) popNode (&pdcpRrcOamMsgQ_g);
		//if(bytesRead >= RLC_API_HEADER_LEN)
		if(NULL != rrcOamMsg_p)
		{
			if (NULL == (buf = rrcOamMsg_p->msg))
			{
				freeMemPool (rrcOamMsg_p);
				rrcOamMsg_p = NULL;
				continue;
			}
			length = rrcOamMsg_p->len;
			if ( 0 < length ) 
			{
				if ((sizeof(TlvHeader) <= length)) /*packet length is checked by TLV parser */
				{
					sourceModuleId = PDCP_NTOHS(((TlvHeader *)buf)->sourceModuleId);
					if ( OAM_MODULE_ID == sourceModuleId )
					{
						str_p = "OAM-REQ RECV\t";
					}
					else if ( RRC_MODULE_ID == sourceModuleId )
					{
						str_p = "RRC-REQ RECV\t";
					}
					else 
					{
						printf(	"OAM-RRC: Incorrect Source module id. sourceModuleId[%i]", 
								sourceModuleId );
					}

					if ( PNULL != str_p )
					{
						//userPrintRawBuffer(str_p, buf, length, 0);
						apiId = PDCP_NTOHS(((TlvHeader *)buf)->apiId);
						printf("%s: AppId[%i], udpLen[%i]", 
								str_p, apiId, length);

						/* make some action */
						if ( OAM_MODULE_ID == sourceModuleId )
						{
							pdcpOamProcessMessage( buf, length );
						}
						else if ( RRC_MODULE_ID == sourceModuleId )
						{
							pdcpRrcProcessMessage( buf, length );
						}
					}
				}
				else /* incorrect packet - drop it */
				{
					printf(
							"OAM-RRC: Incorrect packet. length[%i] TLVlength[%i]", 
							length, PDCP_NTOHS(((TlvHeader *)buf)->length));
				}
			}
		}
		// Free memory
		if(rrcOamMsg_p->msg)
		{
			freeMemPool (rrcOamMsg_p->msg);
			rrcOamMsg_p->msg = NULL;
		}
		if(rrcOamMsg_p)
		{
			freeMemPool(rrcOamMsg_p);
			rrcOamMsg_p = NULL;
		}
	}
}
/* +CA_NA self review */
UInt32 rlcRcvMsgOnQ(void)
{
    //UInt8 msgBuf[MAX_REQ_DATASIZE] = {0};
    UInt8 * msgBuf = NULL;
    UInt8 response[MAX_RES_DATASIZE] = {0};
    SockAddr rrc_addr;
    SInt32 bytesRead = 0;
/* SPR 609 changes start */
    //UInt32 addr_len_rrc = sizeof(rrc_addr);
/* SPR 609 changes end */
    memSet(&rrc_addr, 0x0, sizeof(rrc_addr));
    UInt16 srcModId = 0, destModId = 0, apiId = 0;
    rrcOamMsgNode *rrcOamMsg_p = NULL;
    UInt64  numPdu = 0;
    //shashank put in q
#if 0
    /* method to get the data from socket */
    bytesRead = 0;
    if((bytesRead = READ_FROM_SOCKET(msgBuf,rrc_addr,addr_len_rrc)) == -1 )
    {
        ltePanic ("Error in receive message.\n");
    }
#endif
#ifdef UT_TESTING    
    LTE_LOG(LOG_DETAIL,rlcModuleLogDetail_p,"Recv a %d len packet from port (%d)",bytesRead,ntohs(rrc_addr.sin_port));
#endif        
    numPdu = sQueueCount(&rlcRrcOamMsgQ_g);

/* SPR 609 changes start */
    printf ("No of PDUs in rlcOAMRRC Q [%lld]\n", numPdu);
/* SPR 609 changes end */
    while (numPdu--)
    {
        rrcOamMsg_p = (rrcOamMsgNode*) popNode (&rlcRrcOamMsgQ_g);
        //if(bytesRead >= RLC_API_HEADER_LEN)
        if(NULL != rrcOamMsg_p)
        {
            if (NULL == (msgBuf = (UInt8*)rrcOamMsg_p->msg))
            {
                freeMemPool (rrcOamMsg_p);
                rrcOamMsg_p = NULL;
                printf("dat recevied is NULL\n");
                continue;
            }
            bytesRead = rrcOamMsg_p->len;

            srcModId = LTE_GET_U16BIT(msgBuf + 2);
            destModId = LTE_GET_U16BIT(msgBuf + 4);
            apiId = LTE_GET_U16BIT(msgBuf + 6);
            if(destModId != RLC_MODULE_ID) {
                 printf("destModId != RLC_MODULE_ID\n");
                return RLC_FAILURE;
            }
            if (OAM_MODULE_ID == srcModId) {
                if(CLEANUP_LAYER2 == apiId) {
                 printf("OAM_MODULE_ID == srcModId\n");
                 printf("CLEANUP_LAYER2 == apiId\n");
                    return CLEANUP_LAYER2;
                }    
                /* +- SPR 17777 */
                handleOamRequests(msgBuf);
            } else if(RRC_MODULE_ID == srcModId) {
                if(RLC_SUCCESS == rlcLayerInitialized_g) {
                    printf("calling function parseRRCMsg\n");
                    parseRRCMsg(msgBuf,bytesRead);
                }else {
                    lteWarning("RLC Layer not initialized yet\n");
                    reportErrorToRRCLayer( msgBuf, RLC_LAYER_NOT_INITIALIZED);
                }
            } else {
                lteWarning("Invalid destination module id\n");
                return RLC_FAILURE;
            }   
        } else {    
#ifdef UT_TESTING    
            LTE_LOG(LOG_WARNING,rlcModuleLogDetail_p,"Length of message read from socket \
                    is not enough to parse");
#endif                    
        }             
        // Free memory
        if(rrcOamMsg_p->msg)
        {
            freeMemPool (rrcOamMsg_p->msg);
            rrcOamMsg_p->msg = NULL;
        }
        if(rrcOamMsg_p)
        {
            freeMemPool(rrcOamMsg_p);
            rrcOamMsg_p = NULL;
        }
    }
    return RLC_SUCCESS;

}
/* -CA_NA self review */

void ueSimFreeMemoryAllocBeforeInit(UInt8 cellIndex, LTE_SQUEUE* sQueue)
{
    UInt64 numPdu      = 0;
    macPhyDataNode *data_p = NULL;
    numPdu = sQueueCount(sQueue);
    if(numPdu)
    {
        while (numPdu--)
        {
            data_p = (macPhyDataNode*) popNode (sQueue);
            if(data_p && data_p->msg)
            {
                freeMemPool (data_p->msg);
                data_p->msg = NULL;
            }
            if(data_p)
            {
                freeMemPool(data_p);
                data_p = NULL;
            }
        }
    }
    return;
}

void ueSimWaitForInitComplete (UInt8 cellIndex)
{
    /* Check if the Initialization is complete for all configured cells */
    while (  1 )
    {
       /* SPR 1472 changes start */
       usleep(100);
       /* SPR 1472 changes end */
#ifdef TDD_CONFIG
        usleep(20000);
#endif
#ifndef UESIM_MULTI_THREAD_CELL
        /* check if all layers of all configured cell initialized 
         * if yes, break from while loop */
        for(cellIndex = 0 ; cellIndex < numCells_g ; cellIndex++)
        {
#endif
            /* SPR 609 changes start */
        /* add handler function for PDCP */
            /* if any layers of cell not initialized */
            if (!(!stoppedProcessing_g[cellIndex] && 
                  rlcLayerInitialized_g &&
                pdcpContextIsInitialized() )
               )
        {
            break;
        }
#ifndef UE_SIM_SPLIT
            //       fprintf(stderr, "Ue Sim not Initialized. Ignoring MAC msg\n");
            ueSimFreeMemoryAllocBeforeInit(cellIndex, &macPhyDataQ_g[cellIndex]);
#else
	/* free the memory filled up during DL msg recevied befroe 
	 * UE Sim Initialization */
            ueSimFreeMemoryAllocBeforeInit(cellIndex, &dlMsgStoreQueue);
#endif
#ifndef UESIM_MULTI_THREAD_CELL
        }
        if (cellIndex == numCells_g)
        {
            break;
        }
#endif
        //fprintf(stderr,"in UL DISPATCHER thread inside while....!!!\n");
    }
    return;
}

void ueSimWaitForCellConfigComplete (UInt8 cellIndex)
{
    UInt8 breakFromLoop = FALSE;
    UInt16 numPdu = 0;
    while ( 1 )
    {
        usleep(100);
#ifndef UESIM_MULTI_THREAD_CELL
        /* check if all layers of all configured cell initialized 
         * if yes, break from while loop */
        for(cellIndex = 0 ; cellIndex < numCells_g ; cellIndex++)
        {
#endif
            if ( REG_SUCCESS != macRegFlag_g[cellIndex] )
            {
                numPdu = sQueueCount(&enodebRrcPhyMsgQ_g[cellIndex]);
                if(numPdu)
                {
                    processEnodebRrcPhyMsg( numPdu, cellIndex);
                }
            }
            else
            {
#ifndef UESIM_MULTI_THREAD_CELL
                breakFromLoop = TRUE;
#endif
                break;
            }
#ifndef UESIM_MULTI_THREAD_CELL
        }
        if (TRUE == breakFromLoop)
        {
            break;
        }
#endif
    }
    return;
}

/****************************************************************************
 * Function Name  : execSchedulerThread
 *
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function will process DL data data and control queues 
                    and will call pdcpandrlcMsgHandler and dispatcher to send 
                    UL Data  
 ****************************************************************************/
void * execSchedulerThread(void *arg_p)
{
#ifdef UE_SIM_SPLIT
    SInt32 bytesRead = 0;
    UInt8 buf[4];
    socklen_t length = sizeof(struct sockaddr_in);
#endif
    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    UInt8 subFrameNum = 0;
#endif
    /* - SPS_TDD_Changes */
    UInt8 cellIndex = *((UInt8 *)arg_p);
    UInt8 cellIndexDup = cellIndex;
    UInt64 numPdu      = 0;
    UInt8  turn[MAX_NUM_CELLS]        = {0,0};
    UInt8 i = 0;
    static UInt16 rachReqCount[MAX_NUM_CELLS] = {0,0}; 
    UInt8 numCells = 0 ;

    fprintf(stderr, "\n=========== Exec Schedule Thread Binding for cell[%d] =========================\n", cellIndex);
    THREAD_TO_BIND_SET_CORE_NO( execSchedulerThread_g[cellIndex], execSchedulerThreadCoreNum_g[cellIndex]); 
    THREAD_TO_BIND_GET_CORE_NO( execSchedulerThread_g[cellIndex] ) ;
    fprintf(stderr, "\n==============================================================\n");
    /******************************************************************************************/

    /* BMJ */
    /* Initializing the ACK/NACK percentage level */
#ifdef FAPI_UL_HARQ_ENABLE 
    initULHarqTimerArray();
#endif

#ifndef UESIM_MULTI_THREAD_CELL
    /* it should not hit here because threads will not be created for other than cell-0,
     * but put as a defensive chk */
    /* if it is not first cell, return from the function entry function as this 
     * thread will not be used for any processing */
    if (0 != cellIndex)
    {
        while (1)
            usleep(20000);
        return;
    }
#endif
    /* This function will be in while 1 loop till the time initialization
     * is not complete for all cells */
    ueSimWaitForInitComplete(cellIndex);
    /* This function will be in while 1 loop till the time cell Config
     * is not complete for atleast one cell */
    ueSimWaitForCellConfigComplete(cellIndex);

    threadSetRtPriority(SCHED_FIFO, 45);
    fprintf(stderr, "Execution scheduler thread before while....!!!\n");

    while (1)
    {
#ifndef UESIM_MULTI_THREAD_CELL
        cellIndex = 0;
        numCells = numCells_g;
#else
        /* this is done so that loop runs for one cell only, for cellIndex passed in function */
        numCells = cellIndex+1;
#endif

        /* Tick Indication Recieved from the PHY Timer Thread */
        /* Currently Phy sim timer thread sends the tick for all cells but 
         * with single worker thread implementation only cell index-0 will be handled */
           semWait(&execSchedulerThreadSem_g[cellIndex]);
        #ifdef UE_SIM_SPLIT
        processFapiDLMsg(NULL);
#endif

        for( ; cellIndex < numCells; cellIndex++)
        {
            /* if cell is not yet Up, process Queue */
            numPdu = sQueueCount(&enodebRrcPhyMsgQ_g[cellIndex]);
            if(numPdu)
            {
                processEnodebRrcPhyMsg( numPdu, cellIndex);
            }
        }

#ifndef UESIM_MULTI_THREAD_CELL
        cellIndex = 0;
        numCells = numCells_g;
#else
        /* this is done so that loop runs for one cell only, for cellIndex passed in function */
        cellIndex = cellIndexDup;
        numCells = cellIndex+1;
#endif

#ifndef UE_SIM_SPLIT
        ulReceiver(cellIndex);
        for( ; cellIndex < numCells; cellIndex++)
        {
            /* if the cell is configured because in previous while loop only one cell was up */
            if (REG_SUCCESS == macRegFlag_g[cellIndex])
            {
                /*SPR 6690 fix Start */
                /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                subFrameNum = globalTTITickCount_g % MAX_SUBFRAME;
                if(0 != ulSfForDCI0OrHiReceived_g[tddUlDlConfig_g[cellIndex]][subFrameNum])
                {
#endif
                    processUlUeSimSpsOccasion(cellIndex);
#ifdef TDD_CONFIG
                }
#endif
                /* - SPS_TDD_Changes */
                /*SPR 6690 fix End */
                if (macRegFlag_g[cellIndex] == REG_SUCCESS)
                {
                    rlcAndPdcpMsgHandler(cellIndex);
                }
            }
        }
#endif

#ifndef UE_SIM_SPLIT
#ifndef UESIM_MULTI_THREAD_CELL
        cellIndex = 0;
        numCells = numCells_g;
#else
        /* this is done so that loop runs for one cell only, for cellIndex passed in function */
        cellIndex = cellIndexDup;
        numCells = cellIndex+1;
#endif

        /* send tick for both cells before processing other data as the eNB
         * may be waiting for the same at same time */
        for(; cellIndex < numCells; cellIndex++)
        {
            /* Update the tick for all cells regardless of the cell state, whether running or not */
            ueSimSendAndUpdateSFI (cellIndex);
        }
#endif

#ifndef UESIM_MULTI_THREAD_CELL
        cellIndex = 0;
        numCells = numCells_g;
#else
        /* this is done so that loop runs for one cell only, for cellIndex passed in function */
        cellIndex = cellIndexDup;
        numCells = cellIndex+1;
#endif
        for(; cellIndex < numCells; cellIndex++)
        {
            /* if the cell is configured because in previous while loop only one cell was up */
            if (REG_SUCCESS == macRegFlag_g[cellIndex])
            {
                fapiulDispatcher(cellIndex);

                if (!turn[cellIndex])
                {
                    turn[cellIndex] = 100;
                }
                if(hoRachTriggerFlag[cellIndex])
                {
                    if ( turn[cellIndex] % 4 == 0  && rachReqCount[cellIndex] < ueHoCount_g[cellIndex] )
                    {
                        fprintf(stderr," rachReqCount for cell[%d] = %d RACH started raIndex = %d\n",
                                cellIndex,rachReqCount[cellIndex], hoInfo_g[cellIndex][rachReqCount[cellIndex]].raReqIndex);
                        fapiPrepareRAInfo( hoInfo_g[cellIndex][rachReqCount[cellIndex]].raReqIndex, cellIndex);
                        rachReqCount[cellIndex]++;
                    }
                    if ( rachReqCount[cellIndex] == ueHoCount_g[cellIndex] )
                    {
                        /* reset the static variables now */
                        for ( i = 0; i < ueHoCount_g[cellIndex]; i++)
                        {
                            hoInfo_g[cellIndex][i].raReqIndex = INVALID_RNTI;
                            hoInfo_g[cellIndex][i].cellId = cellId_g[cellIndex];
                        }
                        ueHoCount_g[cellIndex] = 0;
                        hoRachTriggerFlag[cellIndex] = FALSE;
                        rachReqCount[cellIndex] = 0;
                    }
                }
                turn[cellIndex]--;
            }
        }
    }
    return;
}


/********************************************************************************
  ---------------------------------------------------------------------------------*/

/********************************************************************************/

void resetGlobals(UInt8 cellIndex)
{
    ulschDataQueueNode *ulschDataNode_p = PNULL;
    FAPI_ulCrcIndicationNode_st *crcIndNode_p = PNULL;
    FAPI_harqIndQueueNode_st *harqIndNode_p = PNULL;
    UInt8 i = 0;
    UInt16 queueCount = 0; 
    macRegFlag_g[cellIndex] = REG_FAILURE;
#ifdef UESIM_MULTI_THREAD_CELL
    SFN_g[cellIndex]  = MAX_SFN - 1;
    SF_g[cellIndex]   = MAX_SUBFRAME - 1;
#endif
    sfnsf_g[cellIndex] = 0;

     /* SPR 22578 fix start*/
    SFN_g[cellIndex] = 0;
    SF_g[cellIndex] = 0;

   for(i = 0; i< MAX_SUBFRAME;i++)
   {
       queueCount = sQueueCount(&(ulDataQueue[cellIndex][i]));
       while(queueCount--)
       {
            ulschDataNode_p = ( ulschDataQueueNode *)
                popNode(&(ulDataQueue[cellIndex][i]));
            if ( PNULL == ulschDataNode_p )
            {
                continue;
            }
            freeMemPool(ulschDataNode_p->pdudata_p);
            ulschDataNode_p->pdudata_p = PNULL;
       }

      queueCount = sQueueCount(&(crcIndQueue[cellIndex][i]));
       
      while ( queueCount-- )
      {
          crcIndNode_p = ( FAPI_ulCrcIndicationNode_st * )
              popNode ( &(crcIndQueue[cellIndex][i]) );
          if ( PNULL == crcIndNode_p )
          {
              continue;
          }
          freeMemPool(crcIndNode_p);
          crcIndNode_p = PNULL;

      }
    
     queueCount = sQueueCount(&(harqQueue[cellIndex][i]));

     while ( queueCount-- )
     {
         harqIndNode_p = ( FAPI_harqIndQueueNode_st * )
             popNode ( &(harqQueue[cellIndex][i]) );
         if ( PNULL == harqIndNode_p )
         {
             continue;
         }

         freeMemPool(harqIndNode_p);
         harqIndNode_p = PNULL;

     }
   }

     /* SPR 22578 fix end*/
    /* SPR 15909 fix start */
    globalTTITickCount_g = 0xFFFFFFFFFFFFFFFF;
    /* SPR 15909 fix end */
    cellConfigured_g[cellIndex] = FALSE;
#ifdef HO_SETUP_CHANGES
    startRecvFlag[cellIndex] = 0;    
    cellConfigRecvFlag[cellIndex] = 0;
#endif
    /*FAPI_ERROR_INDICATION_CHANGES_START*/
    phy_err_config_g[cellIndex] = 0;
    /*FAPI_ERROR_INDICATION_CHANGES_END*/
    fprintf (stderr, "Reset Global data Successful , cellId = %d!\n", cellIndex);
}
