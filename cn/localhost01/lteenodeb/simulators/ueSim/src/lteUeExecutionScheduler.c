/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteUeExecutionScheduler.c,v 1.7.2.2.4.2.2.3 2010/09/29 06:31:13 gur04640 Exp $
 *
 ******************************************************************************
 *
 *  File Description : 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteUeExecutionScheduler.c,v $
 * Revision 1.7.2.2.4.2.2.3  2010/09/29 06:31:13  gur04640
 * Change priority of UE SIM to 90 and policy to SCHED_FIFO
 *
 * Revision 1.7.2.2.4.2.2.2  2010/09/27 16:49:19  gur10121
 * SPR 631 Fix
 *
 * Revision 1.7.2.2.4.2.2.1  2010/09/21 16:09:22  gur20491
 * FAPI changes
 *
 * Revision 1.7.2.2  2010/08/02 08:57:19  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.5  2010/03/09 07:25:56  gur20439
 * core binding chnages
 *
 * Revision 1.4  2010/03/08 07:56:49  gur19479
 * updated for IP Address
 *
 * Revision 1.3  2010/03/02 12:21:34  gur15697
 * Thread priority changes
 *
 * Revision 1.2  2010/02/18 09:57:46  gur18550
 * Core no of RLC_AND_PDCP_THREAD_CORE_NUM Changed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include <unistd.h>
#include <fcntl.h>
#include "lteUeExecutionScheduler.h"
#include "lteFapiControlMsgHandler.h"
#include "phySimulatorApiHandler.h"
#include "lteLayer2InitMac.h"
#include "lteMsgPool.h"
#include "lteCircQueue.h"
#include "lteCommonQueueDefs.h"
/******************************************************************************
  Private Definitions
 *****************************************************************************/
#define UE_RX_PORT     31320
#define UE_TX_PORT     31321
#define ENODEB_RX_PORT 31322
#define ENODEB_TX_PORT 31323
#define MAX_FD_ELEMENTS MAX_NUM_CELLS*2 + 3
#define COUNT_VALUE 50
#define CLEANUP_LAYER2  0x2FFF
#define RLC_AND_PDCP_THREAD_CORE_NUM 1
#define MAX_STR_LENGTH 128
/******************************************************************************
  Private Types
 *****************************************************************************/
/* TA_CHG */
/* + SPR 11024 Fix */
TAValues  taValues_g[MAX_NUM_CELL] = {0};
/* - SPR 11024 Fix */
#ifdef UESIM_PDCP_THREAD
LTE_SEM boSem_g;
#endif
/* TA_CHG */

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
extern MAC_UE_OPPOR_IND_CIRC_TYPE* macUeOpporIndCircQ_gp[MAX_NUM_CELL];
UInt32 rlcUeSimLoop_g = FALSE;
extern UChar8 macIPAddress_g[40];
extern UChar8 phyIPAddress_g[40];

extern SInt32 readSockFD_g;
extern SInt32 writeSockFD_g;
extern SInt32 writeOAMSockFD_g;
extern SInt32 rxSockFD_g;
extern SInt32 txOAMSockFD_g;
extern SInt32 pdcpSocketRx;
#if 0
extern UInt8 sibMsgFlag_g;
extern UInt8 siRntiMsgFlag_g;
#endif
extern UInt8 mibToBeRecievedFlag_g;
extern UInt8 mibToBeRecievedPrevious_g;
extern volatile UInt16 rlcLayerInitialized_g;

/* +CA_NA */
#ifdef UE_SIM_SPLIT
extern LTE_SEM execSchedulerThreadSem_g;
#else
extern LTE_SEM execSchedulerThreadSem_g[MAX_NUM_CELLS];
/* + SPR_11557_11143 */
SInt32  ev_fd_SFI_g; 
/* - SPR_11557_11143 */
#endif
/* CA_START_UP_PHASE_2 */
SInt32 ev_fd_g[MAX_NUM_CELLS][MAX_NUM_CELLS];
/*thread sync fix start*/ 
SInt32 ev_sync_fd_g[MAX_NUM_CELLS][MAX_NUM_CELLS];
/*thread sync fix end*/ 
/* CA_START_UP_PHASE_2 */
extern volatile UInt8 macRegFlag_g;
extern UInt8 rlfDebugFlag;
extern SInt32 txSockFD_g[MAX_NUM_CELLS];
extern SockAddr macLayerAddr_g[MAX_NUM_CELLS];
extern SockAddr phyLayerAddr_g[MAX_NUM_CELLS];
extern UInt8 numCells_g;
extern MAC_UE_DATA_IND_TYPE macUeDataIndQ_g[MAX_NUM_CELLS];
extern SInt32 sockfd_g[MAX_NUM_CELLS];
LTE_SEM demuxDataSemlock_g[MAX_NUM_CELLS][MAX_NUM_CELLS];
LTE_SEM dlConfigSemlock_g[MAX_NUM_CELLS][MAX_NUM_CELLS];
LTE_SQUEUE macPhyDataQ_g[MAX_NUM_CELLS];
LTE_SQUEUE enodebRrcPhyMsgQ_g[MAX_NUM_CELLS];
SInt32 txEnodebRRCSocketFD_g[MAX_NUM_CELLS] = {-1,-1};
SInt32 rxEnodebRRCSocketFD_g[MAX_NUM_CELLS] = {-1, -1}; 
UInt16 rxPorteNBMac_g[MAX_NUM_CELLS] = {0, 0};
/* SPR 18819 fix start */
//UInt16 rxPorteNBMac_g[MAX_NUM_CELLS]; 
/* SPR 18819 fix end */
/* -CA_NA */
SInt32 txEnodeBMacSocketFD_g;
UInt16 rxPortMAC_g,txPortMacOAM_g,txPortMacRRC_g;
UInt16 readPort_g, writePort_g, writeOAMPort_g;

    /* SPR 15909 fix start */
extern volatile tickType_t globalTTITickCount_g;
    /* SPR 15909 fix end */
ULong32 PhyTimerThread_g;

extern UInt32 rlcRcvMsgOnSocket(void);
LTE_SEM rlcPdcprunThreadSem_g;
/*
 ** 
 **  THE FOLLOWING ARE THE DECLEARTION 
 **  USED FOR THE RLC AND PDCP THREAD.  
 ** 
 */
ULong32 rlcpdcpUeThdExecutionSchedulerThId_g;
extern LTE_SQUEUE rlcUeDataReqQ_g;

extern LTE_SQUEUE rlcControlPduQ_g ;
extern LTE_SQUEUE macUeHarqFailureIndQ_g;
/* SPR 1781 changes start */
extern LTE_SQUEUE pdcpTxControlPduIndQ_g;
/* SPR 1781 changes end */

LTE_SQUEUE rrcMsgQ_g;
LTE_SQUEUE rlcRrcOamMsgQ_g;
LTE_SQUEUE pdcpRrcOamMsgQ_g;

#define BUFFERSIZE 9500
extern UInt8 recvDataBuffer_g[BUFFERSIZE];
/* SPR 5313 changes start */
extern SockAddr phyLayerAddrHO_g[MAX_NUM_CELLS];
/* +CA-SJ */
extern SInt32 sockfdHO_g[MAX_NUM_CELLS];
/* -CA-SJ */
extern volatile UInt16 cellId_g[MAX_NUM_CELLS];
#ifdef LTE_EMBMS_SUPPORTED
extern MAC_UE_DATA_IND_TYPE macEmbmsDataIndQ_g[MAX_NUM_CELL];
#endif

/* SPR 10868 Changes Start */
UInt16 rxPorteNBMacForHO_g[MAX_NUM_CELLS];
/* SPR 10868 Changes End */
/* SPR 5313 changes end */

#define MAX_REQ_DATASIZE                     1000
#define READ_FROM_SOCKET(ui8_MsgBuf_p,sockAddr_rrcAddr,ui32_addrLen)\
	recvfrom(readSockFD_g, (ui8_MsgBuf_p), MAX_REQ_DATASIZE, 0,\
			(struct sockaddr*)&(sockAddr_rrcAddr), &(ui32_addrLen))
/* 
   RLC Queues for Pending Deletion 
   and Reestablishment of Entities
   */
   /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
/* SPR 3805 Fix Start */
extern LTE_CIRCQUEUE rlcRxUePendingDelQ_g;
extern LTE_CIRCQUEUE rlcTxUePendingDelQ_g;
/* SPR 3805 Fix Start */
extern LTE_SQUEUE rlcRxPendingDelQ_g;
extern LTE_SQUEUE rlcRxReEstabQ_g;
extern LTE_SQUEUE rlcTxPendingDelQ_g;
extern LTE_SQUEUE rlcTxReEstabQ_g;
#endif
/* Till Here */

extern UInt16 pdcpContextIsInitialized(void);
#ifdef UESIM_PDCP_THREAD
extern UInt32 rlcHandleTxOppurtunity();
#else
extern UInt32 rlcHandleTxOppurtunity(UInt8 cellIndex);
#endif
extern UInt32 rlcHandleDataFromPdcp();
extern UInt32 pdcpElTxProcessSrbMessage(); 
extern UInt32 pdcpElTxProcessDrbMessage(); 
extern void   rohcProcessTimers(void);
extern UInt32 rlcHandleTxExpiredTimer();
extern UInt32 rlcHandleRxExpiredTimer();
/* CA 2 Core Changes Start */
extern UInt32 rlcHandleDataFromMac(UInt32 count,InternalCellIndex internalCellIndex);
/* CA 2 Core Changes End */
extern void   pdcpElRxProcessMessage();
/* +CA_NA */
extern void * execSchedulerThread(UInt8 cellIndex);
extern void parseConfigFiles();
/* SPR 12148 start */
static void processPdcchOrderNode(UInt8 cellIndex);
/* SPR 12148 end */
void processRlcAndPdcpMsg(UInt8 cellIndex);

/* -CA_NA */

/* SPR_1195_FIX_START */
extern void pdcpElTxProcessControlPduInd(UInt64 count);
/* SPR_1195_FIX_END */

extern void   rlcTimerUpdateTickCheckForTmrEx();
#define HOSTNAME_MAX_LEN 64
extern UChar8 oamModuleHostName[HOSTNAME_MAX_LEN];
extern UChar8 rrcModuleHostName[HOSTNAME_MAX_LEN];
extern UChar8 l2ModuleHostName[HOSTNAME_MAX_LEN];
extern UChar8 l2ModuleIP[HOSTNAME_MAX_LEN];
extern UChar8 oamModuleIP_g[HOSTNAME_MAX_LEN];
extern UChar8 rrcModuleIP_g[HOSTNAME_MAX_LEN];

extern void setPdcpCommPort(UInt16 flag,
		UInt16 pdcpRxPort,
		UInt16 oamTxPort,
		UInt16 rrcTxPort,
		UChar8 *oam_Ip,
		UChar8 *rrc_Ip,
		UChar8 *l2Ip,UChar8 *rrmIp_p,UInt16 rrmTxPor );/*SPR 20863 FIX*/

extern UInt32 handleComChDataFromMac();
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
extern UInt32 rlcHdlrPenDelQ(LTE_SQUEUE *pendDelQ_p);
/* SPR 3805 Fix Start */
extern UInt32 rlcHdlrPenUeDelQ(LTE_CIRCQUEUE *rlcUePendingDelQ);
/* SPR 3805 Fix End */
extern UInt32 rlcHdlrReEstabQ(LTE_SQUEUE *reEstabQ_p);
#endif
extern UInt32 handleComChTxOppurtunity();
extern void   pdcpElTxProcessDeliveryInd(void);
extern void   pdcpElTxProcessDiscardTimers(void);

extern UInt32 initRLCLayer();
extern UInt32 initPDCPLayer();
extern void initPdcpAdapter();
extern UInt8 initMAC( UInt16 *rxenbMacPort, 
		UInt16 *txMacPort,
/* SPR 10868 Changes Start */
		UInt16 *rxPorteNBMacForHO, 
/* SPR 10868 Changes End */
		UInt8 cellIndex 
#ifdef DL_UL_SPLIT_TDD
      ,  UInt16 *txMacPortULThd
#endif
);
        /* SPR 5313 changes end */
extern void fapiProcessConfigMsg ( UInt8 *recvBuf_p, 
		SInt32 bytesRead , UInt8 cellIndex);
extern UInt32 wiresharEnable_g;
extern SInt32 wiresharkfd_g;
extern SockAddr wiresharkAddr_g;
/*
 **
 **  END OF DECLERATION FOR PDCP RLC THREAD
 **
 */

/*
 **
 **  Below we create variables to 
 **  be used as the Socket FD for
 **  receiveing and transmitting 
 **  to and fro from EnodeB RRC
 **
 */

SockAddr rxUeRRC_addr_g;
SockAddr txUeRRC_addr_g;
/* +CA_NA */
SockAddr txEnodebRRC_addr_g[MAX_NUM_CELLS];
SockAddr rxEnodebRRC_addr_g[MAX_NUM_CELLS];
ULong32 execSchedulerThread_g[MAX_NUM_CELLS];
/* -CA_NA */

UChar8 enodebRRCIPAddress_g[40];
#ifdef HO_SETUP_CHANGES
/* +CA */
SInt32 txTargetEnbControlsocketFD_g[MAX_NUM_CELLS] = {-1, -1};
SockAddr txtargetEnodebRRC_addr_g[MAX_NUM_CELLS];
/* -CA */
UChar8 targetEnbIp_g [64]    = "127.0.0.1";
#endif

int pdcpAdaptorCoreNum_g; 
int ulReceiverThreadCoreNum_g;
int uePhyTimerThreadCoreNum_g;
int uldispatcherThreadCoreNum_g;
int execSchedulerThreadCoreNum_g[MAX_NUM_CELLS];
int rlcAndPdcpThreadCoreNum_g;
int instanceUE_g=0;
extern UChar8 trafficGeneratorIP[HOSTNAME_MAX_LEN];
extern UChar8 adapterIP[HOSTNAME_MAX_LEN];
extern UInt16 trafficGeneratorPort;
extern UInt16 pdcpAdapterRxPort;

//extern UChar8 rrcIPAddress_g[40]; 
extern UChar8 phyIPAddress_g[40];
extern UInt32 stoppedProcessing_g[MAX_NUM_CELLS];

extern UInt8 getActiveCell();
extern UInt8  cleanupMAC();
extern UInt32 cleanUpPDCPLayer();
extern UInt32 cleanUpRlcLayer();
extern void cleanupPdcpAdapter();
extern void pdcpElTxProcessSocket(void);
extern void   setPdcpContextInitialized(UInt16 flag);
extern inline void bindThreadToCPUCore (ULong32 threadId, 
		UInt8   CoreNumber,
		UInt8*  threadName);
void createSocketsForRrcPhyInterface(UInt16 *rxEnodebRRCPort, 
        UInt16 *txEnodebRRCPort, 
        UInt8 cellIndex);
/* Coverity 59717 Fix Starts */
void initExecSchedulerQueues(UInt8 cellIndex);
/* Coverity 59717 Fix Ends */
/*************************Shashank*****************************/
extern void * phyTimerThread(void * arg);

/*LWA_Changes*/
/*LWA_Changes*/

/*****************************************************************************
 * Function Name  :  
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : 
 *****************************************************************************/
ULong32 rlcpdcpUeThdExecutionSchedulerThId_g  = 0;

/****************************************************************************
 * Function Name  : setMacPort
 * Inputs         : MAC receiving port, OAM tx port,  RRC tx port
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : set all the passed port to MAC port based on flag 

 ****************************************************************************/
void setMacIPAndPort(UInt16 flag,
		UInt16 rxPortMAC,
		UInt16 txPortOAM,
		UInt16 txPortRRC,
		UChar8 *oamIp_p,
		UChar8 *rrcIp_p,
		UChar8 * l2Ip_p,
		UChar8 * tgIp_p) //ipv6 change
{
	fprintf(stderr, " \n Flag = %d", flag);
	if(flag)
	{
		rxPortMAC_g = rxPortMAC;
		txPortMacOAM_g = txPortOAM;
		txPortMacRRC_g = txPortRRC;
		strcpy((char *)oamModuleIP_g,(const char *)oamIp_p);
		strcpy((char *)rrcModuleIP_g,(const char *)rrcIp_p);
		strcpy((char *)l2ModuleIP,(const char *)l2Ip_p);
        //ipv6 change
		strcpy((char *)dlTgIP,(const char *)tgIp_p);
	}
	else
	{
		rxPortMAC_g = MAC_RX_OAM_RRC_PORT;
		txPortMacOAM_g = MAC_TX_OAM_PORT;
		txPortMacRRC_g = MAC_TX_RRC_PORT;
		strcpy((char *)oamModuleIP_g,"127.0.0.1");
		strcpy((char *)rrcModuleIP_g,"127.0.0.1");
		strcpy((char *)l2ModuleIP,"127.0.0.1");
        //ipv6 change
		strcpy((char *)dlTgIP,"172.16.81.57");
	}
}


/****************************************************************************
 * Function Name  : setRlcPort
 * Inputs         : RLC receiving port, OAM tx port,  RRC tx port
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : set all the passed port to RLC port based on flag 

 ****************************************************************************/
void setRlcIpAndPort(UInt16 flag,
		UInt16 rxPortRLC,
		UInt16 txPortOAM,
		UInt16 txPortRRC,
		UChar8 *oamIp_p,
		UChar8 *rrcIp_p,
		UChar8 *l2Ip_p)
{
	if(flag) {
		readPort_g = rxPortRLC;
		writePort_g = txPortRRC;
		writeOAMPort_g = txPortOAM;
		strcpy((char *)oamModuleHostName,(const char *)oamIp_p);
		strcpy((char *)rrcModuleHostName,(const char *)rrcIp_p);
		strcpy((char *)l2ModuleHostName,(const char *)l2Ip_p);
	} else {
		readPort_g = RLC_RX_OAM_RRC_PORT;
		writePort_g = RLC_TX_RRC_PORT;
		writeOAMPort_g = RLC_TX_OAM_PORT;
		strcpy((char *)oamModuleHostName,"127.0.0.1");
		strcpy((char *)rrcModuleHostName,"127.0.0.1");
		strcpy((char *)l2ModuleHostName,"127.0.0.1");
	}
}


/****************************************************************************
 * Function Name  : setPdcpAdapterIpAndPort
 * Inputs         : trafficGenIP, trafficGenPort, apapterRxPort
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : set all the passed port to PDCP Adapter port based on flag
 ****************************************************************************/
void setPdcpAdapterIPAndPort(UInt16 flag,
		UChar8 *trafficGenIP,
		UChar8 *adptIP,
		UInt16 trafficGenPort,
		UInt16 adapterRxPort)
{
	if(flag)
	{
		/* SPR 609 changes start */
		strcpy((char *)trafficGeneratorIP,(const char *)trafficGenIP);
		strcpy((char *)adapterIP,(const char *)adptIP);
		/* SPR 609 changes end */
		trafficGeneratorPort = trafficGenPort;
		pdcpAdapterRxPort = adapterRxPort;
	}
	else
	{
		/* SPR 609 changes start */
		strcpy((char *)trafficGeneratorIP,"127.0.0.1");
		strcpy((char *)adapterIP,"127.0.0.1");
		/* SPR 609 changes end */
		trafficGeneratorPort = 8888;
		pdcpAdapterRxPort = 9999;
	}
}

/****************************************************************************
 * Function Name  : cleanupUePhySim
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      : None
 * Description    : Cleansup EnodeBPHY Simulator
 ****************************************************************************/
void cleanupUePhySim(
		SInt32 *rxEnodebRRCSocketFD_g,
		SInt32 *txEnodebRRCSocketFD_g)
{
	UInt8 cellIndex = 0;
	UInt32 retTh = 0;
	rlcLayerInitialized_g = 0;
	setPdcpContextInitialized(0);
	stoppedProcessing_g[MAIN_CELL] = 1;
	usleep(1000);

	if(rlcpdcpUeThdExecutionSchedulerThId_g > 0)
	{
		retTh = threadCancel(rlcpdcpUeThdExecutionSchedulerThId_g);
		rlcpdcpUeThdExecutionSchedulerThId_g = 0;
		rlcpdcpUeThdExecutionSchedulerThId_g = 0;
	}
	if(PhyTimerThread_g > 0)
	{
		retTh = threadCancel(PhyTimerThread_g);
		PhyTimerThread_g = 0;
	}

	cleanupPdcpAdapter();

	cleanUpPDCPLayer();
	cleanUpRlcLayer();
	//cleanupMAC();

	/* ############# Closing All Open Sockets #############*/
	/*
	 **
	 **  We close all the sockets which may
	 **  have been opened during the init Phase.
	 **
	 */

	/* ############## RRC PHY SOCKETS ##############*/
#if 0
	if(*rxUeRRCSocketFD > 0 )
	{
		close(*rxUeRRCSocketFD);
		*rxUeRRCSocketFD = 0;
	}
	if(*txUeRRCSocketFD > 0 )
	{
		close(*txUeRRCSocketFD);
		*txUeRRCSocketFD = 0;
	}
#endif
	for (cellIndex = 0; cellIndex < numCells_g; cellIndex++)
		if(*(rxEnodebRRCSocketFD_g + cellIndex) > 0 )
		{
			close(*(rxEnodebRRCSocketFD_g +cellIndex));
			*(rxEnodebRRCSocketFD_g + cellIndex) = 0;
		}
	if(*(txEnodebRRCSocketFD_g + cellIndex) > 0 )
	{
		close(*(txEnodebRRCSocketFD_g +cellIndex));
		(*(txEnodebRRCSocketFD_g + cellIndex)) = 0;
	}
	/* ############## MAC EMULATOR SOCKETS ##############*/
	if ( rxSockFD_g > 0)
	{
		close(rxSockFD_g);
		rxSockFD_g = 0;
	}  
	if ( txSockFD_g[cellIndex] > 0)
	{
		close(txSockFD_g[cellIndex]);
		txSockFD_g[cellIndex] = 0;
	}  
	if ( txOAMSockFD_g > 0)
	{
		close(txOAMSockFD_g);
		txOAMSockFD_g = 0;
	}  
	/* ############## RLC SOCKETS ##############*/
	if ( readSockFD_g > 0)
	{
		close(readSockFD_g);
		readSockFD_g = 0;
	}  
	if ( writeSockFD_g > 0)
	{
		close(writeSockFD_g);
		writeSockFD_g = 0;
	}  
	if ( writeOAMSockFD_g > 0)
	{
		close(writeOAMSockFD_g);
		writeOAMSockFD_g = 0;
	}  
	/* ################ PDCP SOCKETS ##############*/
	if ( pdcpSocketRx > 0)
	{
		close(pdcpSocketRx);
		pdcpSocketRx = 0;
	}  

#if 0
	if ( oamSocketRx > 0)
	{
		close(oamSocketRx);
		oamSocketRx = 0;
	}
	if ( rrcSocketRx > 0)
	{
		close(rrcSocketRx);
		rrcSocketRx = 0;
	}
	if ( pdcpSocketTx > 0)
	{
		close(pdcpSocketTx);
		pdcpSocketTx = 0;
	}  
	if ( oamSocketTx > 0)
	{
		close(oamSocketTx);
		oamSocketTx = 0;
	}  
	if ( rrcSocketTx > 0)
	{
		close(rrcSocketTx);
		rrcSocketTx = 0;
	}  
#endif    
	/* ############# SOCKETS CLOSED ##############*/ 
	usleep(4000);
	cleanupMemPool();
	cleanupMsgPool();
	exit(1);
}

/****************************************************************************
 * Function Name  : rlcAndPdcpThread
 * Inputs         : void * - Arguements
 * Outputs        : None
 * Returns        : void *
 * Variables      :
 * Description    : This function is initialization function for RLC PDCP UE
 *                  thread.
 ****************************************************************************/
//shashank
//void* rlcAndPdcpThread(void* param)
#ifdef UESIM_PDCP_THREAD
void * normalMode(void*arg);
void* pdcpThreadMsgHandler(void* param);

void * uesimPdcpThread(void*arg)
{
	fprintf(stderr, "^^^^^^^^^^ PDCP Thread ^^^^^^^^^\n");
	threadSetRtPriority(SCHED_FIFO, 25);
	while (1)
	{
		normalMode(NULL);
		pdcpThreadMsgHandler(NULL);
	}
	return NULL;
}

void uesimPdcpThreadCreate()
{
	void *arg_p = PNULL;
	UInt32 retTh = 0;
	int coreNum = 1;
	pthread_t recvThId;
	retTh = threadCreate(&recvThId, THREAD_CREATE_DETACHED,
			THREAD_SCHED_FIFO,0,
			THREAD_EXPLICIT_SCHED,
			THREAD_SCOPE_SYSTEM, uesimPdcpThread, arg_p);
	if (retTh != 0)
	{
		ltePanic(" Unable to initialise the Adapter receiver thread.\n");
		return;
	}
	coreNum = pdcpAdaptorCoreNum_g;
	THREAD_TO_BIND_SET_CORE_NO(recvThId,coreNum);
}
/***************************************************************************
 * Function Name  : rlcAndPdcpMsgHandler
 * Inputs         : 
 * Outputs        : None
 * Returns        : 
 * Variables      :
 * Description    : 
****************************************************************************/
void* rlcAndPdcpMsgHandler(void* param)
{
	UInt32 sQcount = 0;
	static UInt32 rlcPdcpfuncCount = 0;
	sQcount = COUNT_MAC_UE_OPPOR_IND_CIRC_Q(cellIndex);
	if(sQcount)
	{
		LTE_LOG(LOG_WARNING,rlcModuleLogDetail_p,"3handleTxOppurtunity %d\n",sQcount);
	}

	while(sQcount)
	{
		rlcHandleTxOppurtunity();
		sQcount--;
	}
	rlcCheckTimerExpiry();
	/* SPR 3891 Fix start */
	expireUMROTimer();
	expireAMROTimer();
	/* SPR 3891 Fix End */
	rlcPdcpfuncCount++;
	if(COUNT_VALUE == rlcPdcpfuncCount)
	{
		/* Handle Srb Data Request */
		rlcHdlrReEstabQ(&rlcTxReEstabQ_g);

		rlcHdlrReEstabQ(&rlcRxReEstabQ_g);
		rlcPdcpfuncCount = 0;

	}
	//threadCleanupPop(1);
	return PNULL;
}

void* pdcpThreadMsgHandler(void* param)
{
	UInt32 sQcount = 0;

	/* SPR 1781 changes start */
	UInt64 count = 0;
	/* SPR 1781 changes end */
	static UInt32 rlcPdcpfuncCount = 0;
        /** CA 2Core Changes Start **/
        UInt32 index = 0;
        /** CA 2Core Changes End **/

	/*
	 ** SPR_1195_FIX_START: PDCP Control PDU indications shall be
	 ** processed at High priority basis. So process all available
	 ** PDCP Control PDU indications here.
	 */
	/* 
	 ** SPR 1781 changes start: On RDB board the function 
	 ** pdcpGetControlPduCount() was returning 0, due to
	 ** optimization as the prototype was not declared in
	 ** our code base, as was being assumed to return U32 instead 
	 ** of U64 by the compiler. To fix this, the function 
	 ** pdcpGetControlPduCount() and its invocations have 
	 ** been removed from the code which was not required as such.
	 ** This is to fix the problem as well as optimize the procesing
	 ** to remove extra invocation of this call.
	 */
	count =  sQueueCount((LTE_SQUEUE*)&pdcpTxControlPduIndQ_g);
	if(count != 0)
	{
		pdcpElTxProcessControlPduInd(count);
	}
	/* SPR 1781 changes end */
	/* SPR_1195_FIX_END */

	/* Handle Tx Opportunities as many as there in Queue at RLC Layer*/
	pdcpElTxProcessDrbMessage();

/*coverity-530-32868 */
#ifdef OBSELETE_REL_8
/*coverity-530-32868 */
	/*Execution Leg to handle harq failure indication from MAC Layer*/
	count = sQueueCount(&macUeHarqFailureIndQ_g);
	if(count)
	{
		rlcHandleHarqFailureInd(count);
	}
#endif

	/* Handle all data from MAC present in RLC Rx Queue and send it
	   to PDCP User */
        /** CA 2 Core Changes Start **/
        for(index = 0;index < MAX_NUM_CELL;index++)
        {
	    sQcount = COUNT_MAC_UE_DATA_IND_Q(index);
	    if(sQcount) 
	    {
	        rlcHandleDataFromMac(sQcount,index);
	    }
            sQcount = 0;
        }    
        /** CA 2 Core Changes End **/
	pdcpElRxProcessMessage();
	pdcpElTxProcessSrbMessage();

	/* Process ROHC Timers at PDCP Layer*/
	rohcProcessTimers();
	/* Handle all data from MAC present in RLC Rx Queue and send it
	   to PDCP User */
	//sQcount = COUNT_MAC_UE_DATA_IND_Q(); 
	//rlcHandleDataFromMac(sQcount); 
	pdcpElRxProcessMessage();
	pdcpElTxProcessDeliveryInd();

	rlcPdcpfuncCount++;

	/* Handle the remaining legs of PDCP and RLC here */
	if(COUNT_VALUE == rlcPdcpfuncCount)
	{
		/* Handle Srb Data Request */
		pdcpElTxProcessDiscardTimers();
		rlcHdlrPenDelQ(&rlcTxPendingDelQ_g);
		/* SPR 3805 Fix Start */
		rlcHdlrPenUeDelQ(&rlcTxUePendingDelQ_g);
		/* SPR 3805 Fix End */
		rlcHdlrPenDelQ(&rlcRxPendingDelQ_g);
		/* SPR 3805 Fix Start */
		rlcHdlrPenUeDelQ(&rlcRxUePendingDelQ_g);
		/* SPR 3805 Fix End */
		rlcPdcpfuncCount = 0;

	}
	return PNULL;
}

#else
void uesimPdcpThreadCreate()
{
	fprintf(stderr, "########## No PDCP Thread #########\n");
}






/***************************************************************************
 * Function Name  : processRlcAndPdcpMsg
 * Inputs         : cellIndex
 * Outputs        : None
 * Returns        : 
 * Variables      :
 * Description    : This function handles PDCP and RLC msg for active cell
****************************************************************************/
void processRlcAndPdcpMsg(UInt8 cellIndex)
{
    UInt32 sQcount = 0;
    UInt64 count = 0;
    static UInt32 rlcPdcpfuncCount = 0;
        /*coverity-530-32868 */
#ifdef OBSELETE_REL_8
        /*coverity-530-32868 */
        /*Execution Leg to handle harq failure indication from MAC Layer*/
        count = sQueueCount(&macUeHarqFailureIndQ_g);
        if(count)
        {
            rlcHandleHarqFailureInd(count);
        }
#endif

        /* Handle all data from MAC present in RLC Rx Queue and send it
           to PDCP User */
        pdcpElRxProcessMessage();
        pdcpElTxProcessSrbMessage();
        /* Process ROHC Timers at PDCP Layer*/
        rohcProcessTimers();
        rlcCheckTimerExpiry();
        /* SPR 3891 Fix start */
        expireUMROTimer();
        expireAMROTimer();
        /* SPR 3891 Fix End */
        /* Handle all data from MAC present in RLC Rx Queue and send it
           to PDCP User */
        pdcpElRxProcessMessage();
        pdcpElTxProcessDeliveryInd();

        rlcPdcpfuncCount++;

        /* Handle the remaining legs of PDCP and RLC here */
        if(COUNT_VALUE == rlcPdcpfuncCount)
        {
            /* Handle Srb Data Request */
            pdcpElTxProcessDiscardTimers();
            rlcHdlrPenDelQ(&rlcTxPendingDelQ_g);
            /* SPR 3805 Fix Start */
            rlcHdlrPenUeDelQ(&rlcTxUePendingDelQ_g);
            /* SPR 3805 Fix End */
            rlcHdlrReEstabQ(&rlcTxReEstabQ_g);

            rlcHdlrPenDelQ(&rlcRxPendingDelQ_g);
            /* SPR 3805 Fix Start */
            rlcHdlrPenUeDelQ(&rlcRxUePendingDelQ_g);
            /* SPR 3805 Fix End */
            rlcHdlrReEstabQ(&rlcRxReEstabQ_g);
            rlcPdcpfuncCount = 0;

        }
}



/***************************************************************************
 * Function Name  : rlcAndPdcpMsgHandler
 * Inputs         : cellIndex
 * Outputs        : None
 * Returns        : 
 * Variables      :
 * Description    : 
****************************************************************************/
void* rlcAndPdcpMsgHandler(UInt8 cellIndex)
{
    UInt32 sQcount = 0;
    UInt64 count = 0;
    static UInt32 rlcPdcpfuncCount = 0;
    if (cellIndex == getActiveCell())
    {
        count =  sQueueCount((LTE_SQUEUE*)&pdcpTxControlPduIndQ_g);
        if(count != 0)
        {
            pdcpElTxProcessControlPduInd(count);
        }
        /* SPR 1781 changes end */
        /* SPR_1195_FIX_END */

        /* Handle Tx Opportunities as many as there in Queue at RLC Layer*/
        pdcpElTxProcessDrbMessage();
    }
    sQcount = COUNT_MAC_UE_OPPOR_IND_CIRC_Q(cellIndex);
    if(sQcount)
    {
        LTE_LOG(LOG_WARNING,rlcModuleLogDetail_p,"3handleTxOppurtunity %d\n",sQcount);
    }

    while(sQcount)
    {
        rlcHandleTxOppurtunity(cellIndex);
        sQcount--;
    }
    /** CA 2 Core Changes Start **/
    sQcount = COUNT_MAC_UE_DATA_IND_Q(cellIndex);
    if(sQcount) 
    {
        rlcHandleDataFromMac(sQcount,cellIndex);
    }   
#ifdef LTE_EMBMS_SUPPORTED
    sQcount = COUNT_MAC_EMBMS_DATA_IND_Q(cellIndex);
    if(sQcount)
    {
        rlcHandleEmbmsDataFromMac(sQcount,cellIndex);
    }
#endif
    /** CA 2 Core Changes End **/
    if (cellIndex == getActiveCell())
    {
    processRlcAndPdcpMsg(cellIndex);
    }

    //threadCleanupPop(1);
    return PNULL;
}

#endif


/****************************************************************************
 * Function Name  : fillTAInfo 
 * Inputs         : 
 * Outputs        : 
 * Returns        :
 * Variables      :
 * Description    : 

 ****************************************************************************/
/* + SPR 11024 Fix */
void fillTAInfo(UInt8 cellCount)
{
    FILE *fp;
    UInt8 buf[100] = {0};
    UInt8  length = 0;
    UInt16 temp[100] = {0};
	SInt8 cellIndex = 0;	
    UInt8 taCqiCounter = 0;

    fp = fopen(TA_CONF_FILE_NAME,"r");
    if(fp == PNULL) 
    {
        lteWarning("FAILED to open TA config file Name\n");
		for(cellIndex = 0; cellIndex<cellCount; cellIndex++)
		{
        taValues_g[cellIndex].taRach = TIMING_ADVANCE_VALUE_RACH;
        taValues_g[cellIndex].numTaUlSch = 1;
        taValues_g[cellIndex].taUlsch[0] = TIMING_ADVANCE_VALUE_ULSCH;
        taValues_g[cellIndex].numTaCqi = 1;
        taValues_g[cellIndex].taCqi[0] = TIMING_ADVANCE_VALUE_ULSCH;
        /* SRS_TA */
        taValues_g[cellIndex].numTaSrs = 1;
        taValues_g[cellIndex].taSrs[0] = TIMING_ADVANCE_VALUE_SRS;
        /* SRS_TA */
		}
        return;
    }
	cellIndex = -1;
    while(( fgets(buf,100,fp) != NULL) && cellIndex < cellCount)
    {
        if((strncmp(buf,"#",1) == 0) || (strncmp(buf,"\n",1) == 0))
        {
            continue;
        }

        trimSpaces(buf);
        length = strlen(buf);
		
		if (strncmp(buf,"[CELL]",6) == 0)
		{
			fprintf(stderr, "\n 1. [%s] cellIndex = %d", __func__,cellIndex);
			strncpy(temp,buf+6,length-6);
			cellIndex++;
		}
		if (cellIndex >= cellCount)
			break;

        if (strncmp(buf,"TA_VALUE_RACH",13) == 0)
        {
            strncpy(temp,buf+14,length-14);
            taValues_g[cellIndex].taRach = atoi(temp);
        }

        else if(strncmp(buf,"TA_VALUE_ULSCH",14) == 0)
        {
            strncpy(temp,buf+15,length-15);
            taValues_g[cellIndex].taUlsch[taValues_g[cellIndex].numTaUlSch] = atoi(temp);
            taValues_g[cellIndex].numTaUlSch++;
        }

        else if(strncmp(buf,"TA_VALUE_CQI",12) == 0)
        {
            strncpy(temp, buf+13,length-13);
            taValues_g[cellIndex].taCqi[taValues_g[cellIndex].numTaCqi] = atoi(temp);
            taValues_g[cellIndex].numTaCqi++;
        }

        else if(strncmp(buf,"TA_VALUE_SRS",12) == 0) 
        {
            strncpy(temp, buf +13,length-13);
            taValues_g[cellIndex].taSrs[taValues_g[cellIndex].numTaSrs] = atoi(temp);
            taValues_g[cellIndex].numTaSrs++; 
        } 
    }
	/* Coverity 10678 Fix Starts */
	fclose(fp);
	/* Coverity 10678 Fix ends */
    return;
}
/* - SPR 11024 Fix */
/* TA_CHG */
/****************************************************************************
 * Function Name  : createTokenStringForEnodebInUesim
 * Inputs         : pointer of :
 *                     			character buffers for storing the string
 * Outputs        : success
 * Returns        :
 * Variables      :
 * Description    : creates the runtime strings for parsing
 *
 * ****************************************************************************/
/* SPR 5313 changes start */
void createTokenStringForEnodebInUesim(char *L1_DATA_RX_PORT_str, char *L1_DATA_TX_PORT_str, char *L1_CONTROL_TX_PORT_str,
/* SPR 10868 Changes Start */
		char *L1_CONTROL_RX_PORT_str,char *L1_IP_ADDRESS_str, char *UESIM_SENB_HO_RX_PORT_str, UInt8 cellIndex 
#ifdef DL_UL_SPLIT_TDD
                 , char *L1_DATA_TX_PORT_UL_THD_str 
#endif
)
/* SPR 10868 Changes End */
	/* SPR 5313 changes end */
{
	instanceUE_g = 0;
	sprintf(L1_DATA_RX_PORT_str,"L1_DATA_RX_PORT_%d%d =%%d",instanceUE_g,cellIndex);
	sprintf(L1_DATA_TX_PORT_str,"L1_DATA_TX_PORT_%d%d =%%d",instanceUE_g,cellIndex);
#ifdef DL_UL_SPLIT_TDD
        sprintf(L1_DATA_TX_PORT_UL_THD_str,"L1_DATA_RX_UL_THREAD_PORT =%%d");                            
#endif
	sprintf(L1_CONTROL_TX_PORT_str,"L1_CONTROL_TX_PORT_%d%d =%%d",instanceUE_g,cellIndex);
	sprintf(L1_CONTROL_RX_PORT_str,"L1_CONTROL_RX_PORT_%d%d =%%d",instanceUE_g,cellIndex);
	sprintf(L1_IP_ADDRESS_str,"L1_IP_ADDRESS_%d%d =%%s",instanceUE_g,cellIndex);
	/* SPR 5313 changes start */
/* SPR 10868 Changes Start */
	sprintf(UESIM_SENB_HO_RX_PORT_str,"UESIM_SENB_HO_RX_PORT_%d%d =%%d",instanceUE_g,cellIndex);
/* SPR 10868 Changes End */
	/* SPR 5313 changes end */
}
/* +CA_NA */


/****************************************************************************
 * Function Name  : initSocketAddressTX
 * Inputs         : sockaddr_in, socketFileDescriptor,Port Number
 * Outputs        :
 * Returns        : status (0-Failure,1-Successful)
 * Variables      :
 * Description    : to intialize the Socket address with specific socket file
 descriptor and specific port
 ****************************************************************************/
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
/* CA_NA need to discuss */

/****************************************************************************
 * Function Name  : fillEnbIpAndPortConfigInfo
 * Inputs         : pointer of :
 *                      mac receiving port for data & Control,Mac Ip Address
 *                       L3 Ip Address
 * Outputs        : success (1) or failure (0)
 * Returns        :
 * Variables      :
 * Description    : set all the passed port to value read from file

 ****************************************************************************/
/* SPR 5313 changes start */
static UInt8 fillEnbIpAndPortConfigInfo( UInt16 *rxPorteNBMac, 
		UInt16 *txPorteNBMac, UInt16 *txControlPort, UInt16 *rxPorteNBRrc,
		UInt16 *rxPorteNBMacForHO, UInt8 cellIndex
#ifdef DL_UL_SPLIT_TDD
        	,UInt16 *txPorteNBMacULThd
#endif
					)
/* SPR 5313 changes end */
{
	FILE *fp;
	UInt32 port=0;
	char buf[101] = {0,};
	UInt32 flag = 0;
	char portConfigFileName[512] ;
	GET_LAYER2_PORT_CONFIG_FILE(portConfigFileName);

	/* SPR 5313 changes start */
	char L1_DATA_RX_PORT_str[MAX_STR_LENGTH],L1_DATA_TX_PORT_str[MAX_STR_LENGTH],L1_CONTROL_TX_PORT_str[MAX_STR_LENGTH],
/* SPR 10868 Changes Start */
		 L1_CONTROL_RX_PORT_str[MAX_STR_LENGTH],L1_IP_ADDRESS_str[MAX_STR_LENGTH], UESIM_SENB_HO_RX_PORT_str[MAX_STR_LENGTH];
/* SPR 10868 Changes End */

#ifdef DL_UL_SPLIT_TDD
    char L1_DATA_TX_PORT_UL_THD_str[MAX_STR_LENGTH];
#endif

	createTokenStringForEnodebInUesim( L1_DATA_RX_PORT_str, L1_DATA_TX_PORT_str, L1_CONTROL_TX_PORT_str,
/* SPR 10868 Changes Start */
			L1_CONTROL_RX_PORT_str,L1_IP_ADDRESS_str, UESIM_SENB_HO_RX_PORT_str, cellIndex 
#ifdef DL_UL_SPLIT_TDD
                                      ,L1_DATA_TX_PORT_UL_THD_str 
#endif
);
/* SPR 10868 Changes End */
	/* SPR 5313 changes end */

	fp = fopen(portConfigFileName,"r");
/*	fp = fopen("../cfg/eNodeB_Configuration_new.cfg","r");*/
	if(fp == PNULL) {
		LTE_LOG(LOG_WARNING, PNULL, 
				"Fail to open the port config file ");
		printf("[%s]Fail to open the port config file %s\n",
				__func__,portConfigFileName);
		return 0;
	}
	while( fgets(buf,100,fp)){
		if(sscanf(buf,L1_DATA_RX_PORT_str,&port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(rxPorteNBMac) = (UInt16)port;
				fprintf(stderr,"\n rxPorteNBMac = %d",*(rxPorteNBMac));
				flag = (flag << 1) | 1;
			}
		}
		else if(sscanf(buf,L1_DATA_TX_PORT_str, &port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(txPorteNBMac) = (UInt16)port;
				fprintf(stderr,"\n txPorteNBMac = %d",*(txPorteNBMac));
				flag = (flag << 1) | 1;
			}
		}
#ifdef DL_UL_SPLIT_TDD
        else if(sscanf(buf,L1_DATA_TX_PORT_UL_THD_str, &port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(txPorteNBMacULThd) = (UInt16)port;
		fprintf(stderr,"\n txPorteNBMacULThd = %d",*(txPorteNBMacULThd));
                flag = (flag << 1) | 1;
            }
        }
#endif
		/* SPR 5313 changes start */
/* SPR 10868 Changes Start */
		else if(sscanf(buf,UESIM_SENB_HO_RX_PORT_str, &port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(rxPorteNBMacForHO) = (UInt16)port;
                fprintf(stderr,"\n UESIM_SENB_HO_RX_PORT = %d \n", *(rxPorteNBMacForHO));
                flag = (flag << 1) | 1;
			}
		}
/* SPR 10868 Changes End */
		/* SPR 5313 changes end */
		else if( sscanf(buf,L1_CONTROL_TX_PORT_str,&port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(txControlPort) = (UInt16)port;
				flag = (flag << 1) | 1;
			}
		}
		else if(sscanf(buf,L1_CONTROL_RX_PORT_str,&port))
		{
			if(port > 0 && port < 65535 )
			{
				*(rxPorteNBRrc) = (UInt16)port;
				flag = (flag << 1) | 1;
			}
		}
		else if(sscanf(buf,L1_IP_ADDRESS_str,macIPAddress_g) > 0)
		{
			strcpy(enodebRRCIPAddress_g, macIPAddress_g);
			flag = (flag << 1) | 1;
		}
	} 
	fprintf(stderr,"flag = %d \n", flag);
	fclose(fp);
#ifdef DL_UL_SPLIT_TDD
    if(flag ==  0x7F )
#else
    if(flag ==  0x3F )
#endif
		return 1;
	else
	{
		printf("\n -----ENB flag = %x -----\n",flag);
		exit(1);
	}
}
/***************************************************************************
 * Function Name  : createTokenString
 * Inputs         : pointer of Strings for tokens and a shared variable for l2 instance
 * Outputs        : Creation of runtime string for tokens
 * Returns        : success
 * Variables      :
 * Description    : This function creates the runtime string for parsing
 *                  configuration file.
 *
 *****************************************************************************/
void createTokenString(char *UE_OAM_RX_PORT_str,char *UE_L3_RX_PORT_str,char *UE_RLC_RX_PORT_str,
		char * UE_MACEMULATOR_RX_PORT_str,char *UE_PDCP_RX_PORT_str,char *UE_OAM_IP_ADDRESS_str,
		char *UE_L3_IP_ADDRESS_str,char *UE_L2_IP_ADDRESS_str,char *UL_TG_IP_ADDRESS_str,
		char *UL_TG_RX_PORT_str,char *UE_ADAPTER_RX_PORT_str, char *DL_TG_IP_ADDRESS_str) //ipv6 change
{

/*SPR 21653 changes start*/
	sprintf(UE_OAM_RX_PORT_str,"UE_OAM_RX_PORT =%%d");
	sprintf(UE_L3_RX_PORT_str,"UE_L3_RX_PORT =%%d");
	sprintf(UE_RLC_RX_PORT_str,"UE_RLC_RX_PORT =%%d");
	sprintf(UE_MACEMULATOR_RX_PORT_str,"UE_MACEMULATOR_RX_PORT =%%d");
	sprintf(UE_PDCP_RX_PORT_str,"UE_PDCP_RX_PORT =%%d");
	sprintf(UE_OAM_IP_ADDRESS_str,"UE_OAM_IP_ADDRESS =%%s");
	sprintf(UE_L3_IP_ADDRESS_str,"UE_L3_IP_ADDRESS =%%s");
	sprintf(UE_L2_IP_ADDRESS_str,"UE_L2_IP_ADDRESS =%%s");
	sprintf(UL_TG_IP_ADDRESS_str,"UL_TG_IP_ADDRESS =%%s");
	sprintf(UL_TG_RX_PORT_str,"UL_TG_RX_PORT =%%d");
	sprintf(UE_ADAPTER_RX_PORT_str,"UE_ADAPTER_RX_PORT =%%d");
	sprintf(DL_TG_IP_ADDRESS_str,"DL_TG_IP_ADDRESS =%%s"); //ipv6 change

/*SPR 21653 changes end*/

}
/* -CA */


/****************************************************************************
 * Function Name  : fillPortInfo
 * Inputs         : pointer of :
 *                      RRC receiving port, PDCP tx port,  RLC tx port
 *                       MAC tx port
 * Outputs        : success (1) or failure (0)
 * Returns        :
 * Variables      :
 * Description    : set all the passed port to value read from file

 ****************************************************************************/
static UInt8 fillPortInfo(UInt16 *rxPortMAC,
		UInt16 *rxPortRLC,
		UInt16 *txPortOAM,
		UInt16 *txPortRRC,
		UInt16 *rxPortPDCP,
		UInt16 *txPorteNBRrc,
		UChar8 *oamIp_p,
		UChar8 *rrcIp_p,
		UChar8 *l2Ip_p, 
		UChar8 *trafficGenIP, 
		UInt16* trafficGenPort,
		UInt16* adapterRxPort,
		UChar8 *dlTgIP_p) //ipv6 change
{
	FILE *fp;
	UInt32 port=0;
	char buf[101] = {0,};
	UInt32 flag = 0;
	char portConfigFileName[256] ;

	char UE_OAM_RX_PORT_str[MAX_STR_LENGTH],UE_L3_RX_PORT_str[MAX_STR_LENGTH],UE_RLC_RX_PORT_str[MAX_STR_LENGTH],
		 UE_MACEMULATOR_RX_PORT_str[MAX_STR_LENGTH],UE_PDCP_RX_PORT_str[MAX_STR_LENGTH],
		 UE_OAM_IP_ADDRESS_str[MAX_STR_LENGTH],UE_L3_IP_ADDRESS_str[MAX_STR_LENGTH],UE_L2_IP_ADDRESS_str[MAX_STR_LENGTH],
		 UL_TG_IP_ADDRESS_str[MAX_STR_LENGTH],UL_TG_RX_PORT_str[MAX_STR_LENGTH],UE_ADAPTER_RX_PORT_str[MAX_STR_LENGTH],
         DL_TG_IP_ADDRESS_str[MAX_STR_LENGTH]; //ipv6 change

	createTokenString(UE_OAM_RX_PORT_str,UE_L3_RX_PORT_str,UE_RLC_RX_PORT_str,
			UE_MACEMULATOR_RX_PORT_str,UE_PDCP_RX_PORT_str,UE_OAM_IP_ADDRESS_str,
			UE_L3_IP_ADDRESS_str,UE_L2_IP_ADDRESS_str,UL_TG_IP_ADDRESS_str,
			UL_TG_RX_PORT_str,UE_ADAPTER_RX_PORT_str,DL_TG_IP_ADDRESS_str); //ipv6 change

	GET_UE_SIM_PORT_CONFIG_FILE(portConfigFileName);
	fp = fopen(portConfigFileName,"r");
	if(fp == PNULL) {
		LTE_LOG(LOG_WARNING, PNULL, 
				"Fail to open the port config file ");
		printf("[%s] Fail to open the port config file %s\n",
				__func__,portConfigFileName);
		return 0;
	}
	while( fgets(buf,100,fp)){
		if(sscanf(buf,UE_OAM_RX_PORT_str,&port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(txPortOAM) = (UInt16)port;
				flag = (flag << 1) | 1;
			}
		}
		else if(sscanf(buf,UE_L3_RX_PORT_str, &port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(txPortRRC) = (UInt16)port;
				flag = (flag << 1) | 1;
			}
			fprintf(stderr,"\n UE L3 RX Port is %d \n", *txPortRRC);
		}
		else if(sscanf(buf,UE_RLC_RX_PORT_str, &port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(rxPortRLC) = (UInt16)port;
				flag = (flag << 1) | 1;
			}
		}
		else if(sscanf(buf,UE_MACEMULATOR_RX_PORT_str, &port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(rxPortMAC) = (UInt16)port;
				flag = (flag << 1) | 1;
			}
		}
		else if(sscanf(buf,UE_PDCP_RX_PORT_str, &port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(rxPortPDCP) = (UInt16)port;
				flag = (flag << 1) | 1;
			}
		}

		else if (sscanf(buf,UE_OAM_IP_ADDRESS_str,oamIp_p) > 0)
			flag = (flag << 1) | 1;
		else if (sscanf(buf,UE_L3_IP_ADDRESS_str,rrcIp_p) > 0)
			flag = (flag << 1) | 1;
		else if (sscanf(buf,UE_L2_IP_ADDRESS_str, l2Ip_p) > 0)
			flag = (flag << 1) | 1;
		else if(sscanf(buf,UL_TG_IP_ADDRESS_str,trafficGenIP) > 0)
			flag = (flag << 1) | 1;
            //ipv6 change
		else if(sscanf(buf,DL_TG_IP_ADDRESS_str,dlTgIP_p) > 0)
			flag = (flag << 1) | 1;
		else if(sscanf(buf,UL_TG_RX_PORT_str,&port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(trafficGenPort) = (UInt16)port;
				flag = (flag << 1) | 1;
			}

		}
		else if(sscanf(buf,UE_ADAPTER_RX_PORT_str,&port) > 0)
		{
			if(port > 0 && port < 65535 )
			{
				*(adapterRxPort) = (UInt16)port;
				flag = (flag << 1) | 1;
			}
		}
#ifdef HO_SETUP_CHANGES
		else if(sscanf(buf,"TARGET_ENB_IP_ADDRESS =%s",targetEnbIp_g) > 0)
			flag = (flag << 1) | 1;
#endif

	}
	fclose(fp);
#ifndef HO_SETUP_CHANGES
//ipv6 change
//	if ( flag == 0x7FF )
	if ( flag == 0xFFF )
#else
//ipv6 change
//		if(flag ==  0xFFF )
		if(flag ==  0x1FFF )
#endif
			return 1;
		else
		{
			printf("\n ----- flag = %x -----\n",flag);
			exit(1);
		}
}


void createMemPools()
{
	initMemPool();
	initMsgPool();
	createMemPool(32,200);
	createMemPool(64,200);
	createMemPool(2048,200);

}





void initQueues()
{
	UInt8 i = 0;
	printf(" Queues initialized\n");
	sQueueInit(&rrcMsgQ_g);
	sQueueInit(&rlcRrcOamMsgQ_g);
	sQueueInit(&pdcpRrcOamMsgQ_g);

    /* SPR 12148 start */
    for (i = 0; i < numCells_g; i++)
    {        
        sQueueInit(&pdcchOrderQueue_g[i]);
    }        
    /* SPR 12148 end */
#ifdef UE_SIM_SPLIT
	sQueueInit(&dlMsgStoreQueue);
#endif  
}

/*****************************************************************************
 * Function Name  : fillIpAndPortConfigInfo 
 * Inputs         : 
 * Outputs        : 
 * Returns        : None  
 * Description    : This function will call functions to initialize comm ports
 *                  ports, to initialize the Layer2 modules, to create comm 
 *                  sockets and to iniliaze queues and semaphores
 ******************************************************************************/
void fillIpAndPortConfigInfo ( )
{
	UInt8 i = 0;
	UInt16 flag = 0;
	UInt8 cellIndex = 0;
	UInt8 cellCount = 0;
	/* SPR 18819 fix start */
	//UInt16 rxPorteNBMac_g[MAX_NUM_CELLS] = {0,0};
	/* SPR 18819 fix end */
	UInt16 txPorteNBMac[MAX_NUM_CELLS] = {0,0};
	UInt16 txControlPort[MAX_NUM_CELLS] = {0,0};
	UInt16 rxPorteNBRrc[MAX_NUM_CELLS] = {0,0};
	for (cellIndex = 0 ; cellIndex < numCells_g; cellIndex++)
	{
		flag = fillEnbIpAndPortConfigInfo ( 
				/* SPR 18819 fix start */
				&rxPorteNBMac_g[cellIndex],
				/* SPR 18819 fix end */
				&txPorteNBMac[cellIndex],
				&txControlPort[cellIndex],
				&rxPorteNBRrc[cellIndex],
/* SPR 10868 Changes Start */
				&rxPorteNBMacForHO_g[cellIndex],
				cellIndex
#ifdef DL_UL_SPLIT_TDD
        			,UInt16 *txPorteNBMacULThd
#endif
);
				/* SPR 18819 fix start */
		if ( MAC_SUCCESS != initMAC(&rxPorteNBMac_g[cellIndex], &txPorteNBMac[cellIndex],
				/* SPR 18819 fix end */
					&rxPorteNBMacForHO_g[cellIndex], cellIndex
#ifdef DL_UL_SPLIT_TDD
                			,&txPorteNBMacULThd
#endif
		))
/* SPR 10868 Changes End */
			/* SPR 5313 changes end */
		{
			ltePanic("OAM: MAC Initialization fails for cellIndex %d \n", cellIndex);
		}
		createSocketsForRrcPhyInterface(&rxPorteNBRrc[cellIndex], 
				&txControlPort[cellIndex], cellIndex);
		initExecSchedulerQueues(cellIndex);
		semInit(&execSchedulerThreadSem_g[cellIndex],0);
		for( cellCount = 0 ; cellCount < numCells_g ; cellCount++)
		{
			semInit(&demuxDataSemlock_g[cellIndex][cellCount],0);
			semInit(&dlConfigSemlock_g[cellIndex][cellCount],0);
/* CA_START_UP_PHASE_2 */
//            circQInit(&rrmMacCellGlobal_gp[internaliCellIndex]->ulUEReconfigDataQueue,MAX_NODES);
            circQInit(&dlSchPduQ[cellIndex].DlDataCircQ[cellCount],MAX_NODES);
            //ev_fd_g[cellIndex][cellCount]=eventfd_initialize_wrapper(0, 0);
 #if 0
            ev_fd_g[cellIndex][cellCount]=eventfd_initialize_wrapper(0, EFD_SEMAPHORE);
/*thread sync fix start*/ 
            ev_sync_fd_g[cellIndex][cellCount]=eventfd_initialize_wrapper(0, EFD_SEMAPHORE);
#endif
/*thread sync fix end*/ 
/* CA_START_UP_PHASE_2 */
		}
	}
}

/*****************************************************************************
 * Function Name  : initUEStack 
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function will initilise all the entities in UE Side.
 *****************************************************************************/
/* + SPR 11024 Fix */ 
void initUEStack(UInt8 cellCount)
/* - SPR 11024 Fix */ 
{
	/*
	 **
	 ** The Port Number Variables for Socket Interface.
	 **
	 */
	UInt16 flag = 0;
	UInt16 rxPortMAC = 0;
	UInt16 txPortOAM = 0;
	UInt16 txPortRRC = 0;
	UInt16 rxPortRLC = 0;
	UInt16 rxPortPDCP = 0;
#ifdef DL_UL_SPLIT_TDD
    UInt16 txPorteNBMacULThd = 0;
#endif
	UInt16 txPorteNBRrc = 0;

	/*
	 **
	 ** The IP Address Variables for Socket Interface.
	 **
	 */
	UChar8 oamIp [64]    = "127.0.0.1";
	UChar8 rrcIp [64]    = "127.0.0.1";
	UChar8 l2Ip  [64]    = "127.0.0.1";

	/* TA_CHG */
/* + SPR 11024 Fix */ 
	fillTAInfo(cellCount);
/* - SPR 11024 Fix */ 
	/* TA_CHG */

	UChar8 trafficGenIP[64] = "127.0.0.1";
	UInt16 trafficGenPort   = 0;
	UInt16 adapterRxPort    = 0;
    //ipv6 change
	UChar8 dlTrafficIP[64] = "127.0.0.1";

	flag = fillPortInfo(&rxPortMAC,
			&rxPortRLC,
			&txPortOAM,
			&txPortRRC, 
			&rxPortPDCP,
			&txPorteNBRrc,
			oamIp,
			rrcIp,
			l2Ip,
			trafficGenIP,
			&trafficGenPort,
			&adapterRxPort,
            dlTrafficIP);  //ipv6 change

	setMacIPAndPort(flag,
			rxPortMAC,
			txPortOAM,
			txPortRRC,
			oamIp,
			rrcIp,
			l2Ip,
            dlTrafficIP); //ipv6 change

	setRlcIpAndPort(flag,
			rxPortRLC,
			txPortOAM,
			txPortRRC,
			oamIp,
			rrcIp,
			l2Ip);
            
	setPdcpCommPort(flag,
			rxPortPDCP,
			txPortOAM,
			txPortRRC,
			oamIp,
			rrcIp,
			l2Ip,
            /*SPR 20863 FIX*/
            PNULL,
            PNULL);
            /*SPR 20863 FIX*/


	setPdcpAdapterIPAndPort(flag,
			trafficGenIP,l2Ip,
			trafficGenPort,
			adapterRxPort);
	/* ################## SOCKET INITIALISATION FOR ENodeb RRC, MAC ################## */

	initQueues();
	createMemPools();
	getIPAddrFromFile();
	semInit(&rlcPdcprunThreadSem_g,0);
        /* + SPR_11557_11143 */
#if 0
         ev_fd_SFI_g = eventfd_initialize_wrapper(0, EFD_SEMAPHORE);
#endif
       /* - SPR_11557_11143 */
	/* SPR 5313 changes start */

	/* Create Sockets to communicate with RRC Layer */
	createSockets(rxPortMAC,txPortOAM,txPortRRC);
    //ipv6 change
#if (defined(LTE_EMBMS_SUPPORTED) && !defined(LAYER2_DLTG_THREAD))
    initTGSocket();
#endif
	if( 1 != initRLCLayer()) 
	{
		ltePanic("Failed to init RLC Layer\n");
	}
	/*call the functions which will read the config files req.*/


#ifdef UESIM_PDCP_THREAD
	semInit(&boSem_g, 1);
#endif
	initPDCPLayer();
	initPdcpAdapter();
#ifdef UESIM_PDCP_THREAD
	uesimPdcpThreadCreate();
#endif
#ifdef UE_SIM_SPLIT
	initIPCInterfaceUEPHYAndUEMAC();
#endif
}

/*****************************************************************************
 * Function Name  :  createSocketsForRrcPhyInterface
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    :  This function will create sockets for communication between
 *                   Enodeb PHY <----> Enodeb RRC.
 *****************************************************************************/


/* +CA */
void createSocketsForRrcPhyInterface(UInt16 *rxEnodebRRCPort, 
		UInt16 *txEnodebRRCPort, UInt8 cellIndex )
{

	/* ##################  ENODEB RRC TX SOCKET #################### */
	/*
	 **
	 **  The following socket will be used to transmit data to Enodeb RRC
	 **
	 */
	if ((txEnodebRRCSocketFD_g[cellIndex] = socket(AF_INET, SOCK_DGRAM, \
					IPPROTO_UDP)) == -1)
	{
		ltePanic(" Socket creation failed for Tx to Enodeb RRC for cell \
				= %d\n", cellIndex);
		exit(0);
	}

	txEnodebRRC_addr_g[cellIndex].sin_family = AF_INET;
	txEnodebRRC_addr_g[cellIndex].sin_port   = htons(*txEnodebRRCPort);

	/* SPR 609 changes start */
	if (inet_aton((const char *)enodebRRCIPAddress_g, &txEnodebRRC_addr_g\
				[cellIndex].sin_addr) == 0)
		/* SPR 609 changes end */
	{
		ltePanic ("Error while converting the address.");
		exit(0);
	}    

#ifdef HO_SETUP_CHANGES
    /* +CA  */
	if ((txTargetEnbControlsocketFD_g[cellIndex] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		ltePanic(" Socket creation failed for Tx to Target Enodeb Control.\n");
		exit(0);
	}

	txtargetEnodebRRC_addr_g[cellIndex].sin_family = AF_INET;
	txtargetEnodebRRC_addr_g[cellIndex].sin_port   = htons(*txEnodebRRCPort);

    fprintf(stderr,"txTargetEnbControlsocketFD_g[%d] = %d, targetEnbIp_g = %s\n",cellIndex,
            txTargetEnbControlsocketFD_g[cellIndex],targetEnbIp_g);
	if (inet_aton((const char *)targetEnbIp_g, &txtargetEnodebRRC_addr_g[cellIndex].sin_addr) == 0)
	{
		ltePanic ("Error while converting the address for target ENB.");
		exit(0);
	}    
    /* -CA*/
#endif


	/* ##################  ENODEB RRC RX SOCKET #################### */
	/*
	 **
	 **  The following socket will listen to any data from Enodeb RRC
	 **
	 */

	if ((rxEnodebRRCSocketFD_g[cellIndex] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		ltePanic("Error while opening the receiver socket For Enodeb RRC, \
				cellIndex = %d.\n", cellIndex);
        exit(0);
    }
    fprintf(stderr, "RRC-PHY sockfd = %d\n", rxEnodebRRCSocketFD_g[cellIndex]);

    rxEnodebRRC_addr_g[cellIndex].sin_family = AF_INET;
    rxEnodebRRC_addr_g[cellIndex].sin_port = htons (*rxEnodebRRCPort);

    /* Coverity 59866 Fix */
    printf("control RX Port = %d, control TX Port = %d, cellIndex %d.\n",
            *rxEnodebRRCPort, *txEnodebRRCPort, cellIndex); 
    /* SPR 609 changes start */
    if (inet_aton((const char *)l2ModuleIP,&rxEnodebRRC_addr_g\
				[cellIndex].sin_addr) == 0)
        /* SPR 609 changes end */
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }

    if ((bind(rxEnodebRRCSocketFD_g[cellIndex], 
                    (struct sockaddr *)&rxEnodebRRC_addr_g[cellIndex], 
                    sizeof(rxEnodebRRC_addr_g[cellIndex]))) == -1)
    {
	    /* Coverity 59867 Fix Starts */
        ltePanic ("Unable to bind the receiver socket with  Enodeb RRC, \
                cellIndex = %d.\n", cellIndex);
	    /* Coverity 59867 Fix Ends */
        exit(0);
    }
	fprintf(stderr,"\nBind Successful, RX UE<->eNB RRC PORT = %d \
			for cell = %d \n ", *rxEnodebRRCPort, cellIndex);
}

/* -CA_NA */

/*****************************************************************************
 * Function Name  :  sendMsgtoEnodebRRC
 * Inputs         :  receivedBuffer_p - The received buffer
 *                   txEnodebRRCSocketFD_g - The socket descriptor of TX at ENB PHY
 *                   msgLen - The message length. 
 * Outputs        : 
 * Returns        : 
 * Description    :  This function will send data to the Enodeb RRC from Enodeb PHY.
 *****************************************************************************/
void sendMsgtoEnodebRRC(UInt8 *receivedBuffer_p,
        /* Coverity 59851 Fix Starts */
        SInt32 txEnodebRRCSocketFD, 
        /* Coverity 59851 Fix Ends */
        UInt32 msgLen, UInt8 cellIndex)
{
    SInt32 ret = 0;
    ret =  sendto(txEnodebRRCSocketFD, receivedBuffer_p,msgLen, 0,
            (struct sockaddr*)&txEnodebRRC_addr_g[cellIndex], sizeof(SockAddr));
    if (ret == -1)
    {
        fprintf(stderr,"Error No- %d Desp: %s  in %s at Line no %d\n",\
                errno,strerror(errno), __func__, __LINE__);
    }

}
/****************************************************************************
 * Function Name  : prepareBufferHeader
 * Inputs         : msgId - Msg Type to be send to RRC
 *                : msgBuf - Buffer to be filled
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API takes the msgId & buffer to be filled by msg
 *                  header as input.
 ****************************************************************************/
void prepareRRCHeader(UInt8 *msgBuf, UInt16 msgId, UInt16 destModuleId)
{
	UInt16 current = 2;
	LTE_SET_U16BIT(msgBuf + current, PHY_MODULE_ID);
	current += 2;
	LTE_SET_U16BIT(msgBuf + current, destModuleId);
	current += 2;
	LTE_SET_U16BIT(msgBuf + current, msgId);
	current += 2;
	return;
}
/*****************************************************************************
 * Function Name  :  handleCellSetUpReq
 * Inputs         :  msg_p - The pointer to the incoming message
 *                   transactionId - The transaction ID in incoming msg.  
 * Outputs        : 
 * Returns        : 
 * Description    : This function will be called upon arrival of PHY_CONFIG_CELL_REQ
 *                  from the Enodeb RRC to Enodeb PHY layer. 
 *****************************************************************************/

void handleCellSetUpReq(UInt8 *msg_p, 
        /* Coverity 59851 Fix Starts */
        SInt32 txEnodebRRCSocketFD,
        /* Coverity 59851 Fix Ends */
        UInt16 transactionId,
		UInt8 cellIndex)
{
	UInt32 index = 0;
	UInt32 msgLenIndex = 8;
	UInt8 physicalCellId = 0x00;
	UInt8 msgBuf[PHY_CONFIG_CELL_CNF_BUFFER_LEN] = {0};

	LTE_SET_U16BIT(msgBuf, transactionId);
	physicalCellId = *(msg_p + 2);
	prepareRRCHeader(msgBuf, PHY_CONFIG_CELL_CNF, RRC_MODULE_ID);
	index = RRC_API_HEADER_LEN;
	msgBuf[index++] = physicalCellId;
	msgBuf[index++] = SUCCESS;
	LTE_SET_U16BIT(msgBuf + msgLenIndex, index);
	sendMsgtoEnodebRRC(msgBuf, 
			txEnodebRRCSocketFD,
			index, cellIndex); 
}


/*****************************************************************************
 * Function Name  :  handleUeCreateReq
 * Inputs         :  msg_p - Pointer to the message buffer which needs to be sent.
 *                   txEnodebRRCSocketFD_g - TX socket file descriptor at Enodeb PHY.
 *                   transactionId - The transaction ID. 
 * Outputs        : 
 * Returns        : 
 * Description    :  This function will create and send the response to the 
 *                   Enodeb RRC for PHY_CREATE_UE_ENTITY_REQ 
 *****************************************************************************/

void handleUeCreateReq (UInt8 *msg_p, 
		SInt32 *txEnodebRRCSocketFD, 
		UInt16 transactionId,
		UInt8 cellIndex)
{
	UInt32 index = 0;
	UInt32 msgLenIndex = 8;
	UInt16 ueIndex = 0x00;
	UInt8 msgBuf[PHY_CREATE_UE_ENTITY_CNF_BUFFER_LEN] = {0};

	LTE_SET_U16BIT(msgBuf, transactionId);
	ueIndex = LTE_GET_U16BIT(msg_p);
	prepareRRCHeader(msgBuf, PHY_CREATE_UE_ENTITY_CNF, RRC_MODULE_ID);
	index = RRC_API_HEADER_LEN;
	LTE_SET_U16BIT(msgBuf + index, ueIndex);
	index += 2;
	msgBuf[index++] = SUCCESS;
	LTE_SET_U16BIT(msgBuf + msgLenIndex, index);
	sendMsgtoEnodebRRC(msgBuf, 
			txEnodebRRCSocketFD,
			index,cellIndex); 
}
/*****************************************************************************
 * Function Name  :  handleUeReconfigReq
 * Inputs         :  msg_p - Pointer to the message buffer which needs to be sent.
 *                   txEnodebRRCSocketFD_g - TX socket file descriptor at Enodeb PHY.
 *                   transactionId - The transaction ID. 
 * Outputs        : 
 * Returns        : 
 * Description    :  This function will create and send the response to the 
 *                   Enodeb RRC for PHY_RECONFIG_UE_ENTITY_REQ 
 *****************************************************************************/
void handleUeReconfigReq (UInt8 *msg_p,
		SInt32 *txEnodebRRCSocketFD, 
		UInt16 transactionId,
		UInt8 cellIndex)
{
	UInt32 index = 0;
	UInt16 ueIndex = 0x00;
	UInt32 msgLenIndex = 8;
	UInt8 msgBuf[PHY_RECONFIG_UE_ENTITY_CNF_BUFFER_LEN] = {0};

	LTE_SET_U16BIT(msgBuf, transactionId);
	ueIndex = LTE_GET_U16BIT(msg_p);
	prepareRRCHeader(msgBuf, PHY_RECONFIG_UE_ENTITY_CNF, RRC_MODULE_ID);
	index = RRC_API_HEADER_LEN;
	msgBuf[index++]= 0;
	msgBuf[index++]= 0;
	LTE_SET_U16BIT(msgBuf + index, ueIndex);
	index += 2;
	msgBuf[index++] = SUCCESS;
	LTE_SET_U16BIT(msgBuf + msgLenIndex, index);
	sendMsgtoEnodebRRC(msgBuf, 
			txEnodebRRCSocketFD,
			index, cellIndex); 
}


/*****************************************************************************
 * Function Name  :  handleUeDeleteReq
 * Inputs         :  msg_p - Pointer to the message buffer which needs to be sent.
 *                   txEnodebRRCSocketFD_g - TX socket file descriptor at Enodeb PHY.
 *                   transactionId - The transaction ID. 
 * Outputs        : 
 * Returns        : 
 * Description    :  This function will create and send the response to the 
 *                   Enodeb RRC for PHY_DELETE_UE_ENTITY_REQ 
 *****************************************************************************/
void handleUeDeleteReq (UInt8 *msg_p,
		SInt32 *txEnodebRRCSocketFD, 
		UInt16 transactionId,
		UInt8 cellIndex)
{
	UInt32 index = 0;
	UInt16 ueIndex = 0x00;
	UInt32 msgLenIndex = 8;
	UInt8 msgBuf[PHY_DELETE_UE_ENTITY_CNF_BUFFER_LEN] = {0};
	LTE_SET_U16BIT(msgBuf, transactionId); 
	ueIndex = LTE_GET_U16BIT(msg_p);
	prepareRRCHeader(msgBuf, PHY_DELETE_UE_ENTITY_CNF, RRC_MODULE_ID);
	index = RRC_API_HEADER_LEN;
	LTE_SET_U16BIT(msgBuf + index, ueIndex);
	index += 2;
	msgBuf[index++] = SUCCESS;
	LTE_SET_U16BIT(msgBuf + msgLenIndex, index);
	sendMsgtoEnodebRRC(msgBuf, 
			txEnodebRRCSocketFD,
			index, cellIndex); 
}

/*****************************************************************************
 * Function Name  :  processEnodebRrcPhyMsg
 * Inputs         :  The socket FD of RX/TX ports at Enodeb PHY SIM. 
 * Outputs        : 
 * Returns        : 
 * Description    :  This function will process all the messages that are 
 *                   received by the Enodeb PHY from the Enodeb RRC. 
 *****************************************************************************/


void processEnodebRrcPhyMsg(UInt64 numPdu, UInt8 cellIndex)
{
	UInt8 * msgBuf = NULL;
	SockAddr rxEnodebPhyClient;
	UInt32 addrLen = sizeof(rxEnodebPhyClient);
	SInt32 bytesRead = 0; 
	rrcOamMsgNode *rrcOamMsg_p = NULL;

	memSet(&rxEnodebPhyClient, 0, addrLen);
	//shashank put in q
#if 0
	if ((bytesRead = recvfrom(rxEnodebRRCSocketFD_g, 
					msgBuf, 
					MAX_DATA_SIZE,
					0,
					(struct sockaddr*)&rxEnodebPhyClient,
					&addrLen)) == -1)
	{
		return;
	}
#endif 
	if ( !numPdu )
	{
		//printf("numPdu is zero\n");
		return;
	}

	while (numPdu--)
	{
		printf ("No of PDUs in EnodebRrcPhy [%u]\n", numPdu+1);
		rrcOamMsg_p = (rrcOamMsgNode*) popNode (&enodebRrcPhyMsgQ_g[cellIndex]);
		//if( bytesRead >= RRC_API_HEADER_LEN )
		if(NULL != rrcOamMsg_p)
		{
			if (NULL == (msgBuf = rrcOamMsg_p->msg))
			{
				freeMemPool (rrcOamMsg_p);
				rrcOamMsg_p = NULL;
				continue;
			}
			bytesRead = rrcOamMsg_p->len; 
			if ( wiresharEnable_g )
			{
				sendto(wiresharkfd_g,msgBuf,bytesRead, 0,
						(struct sockaddr*)&wiresharkAddr_g, sizeof(SockAddr));
			}
			fapiProcessConfigMsg(msgBuf, bytesRead , cellIndex);
		}
	// Free memory
	if(msgBuf)
	{
		freeMemPool (msgBuf);
		msgBuf = NULL;

	}
	if(rrcOamMsg_p)
	{
		freeMemPool(rrcOamMsg_p);
		rrcOamMsg_p = NULL;
	}

}
}


#ifdef UE_SIM_SPLIT
/*****************************************************************************
 * Function Name  :  processDlMsgRecvFromEnbPhyOnDataSocket 
 * Inputs         :  data received on Socket 
 * Outputs        : 
 * Returns        : 
 * Description    : This function will parse the buffer to get sfn/sf received
 *                  from ENB PHY Sim. Store the first SFN/ SF value into global
 *                  and from next SFN/SF onwards, send this tick to UE MAC.
 *****************************************************************************/
struct timeval g_sfi_ts;
void processDlMsgRecvFromEnbPhyOnDataSocket ( UInt8 *data_p , UInt32 msgLen)
{
	FAPI_l1ApiMsg_st *l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) data_p;
	FAPI_subFrameIndication_st *sfnsf_p = PNULL;
	static UInt16 sfnsf = 0, sfn = 0;
	static UInt8 sf = 0;
	macPhyDataNode    *pDataQnode      = NULL;
	SInt32 ret = 0;
	UInt8 buf[4];
	unsigned long int x1,x2;
	struct timeval sfi_ts;
	static int x = 0;
	if (PHY_UL_SUBFRAME_INDICATION == l1ApiMsg_p->msgId )
	{    
		gettimeofday(&sfi_ts, NULL);
		if (x == 0)
		{
			g_sfi_ts = sfi_ts;
			x = 1;
		}
		x1 = g_sfi_ts.tv_sec * 1000000 + g_sfi_ts.tv_usec;
		x2 = sfi_ts.tv_sec * 1000000 + sfi_ts.tv_usec;

		if (x2 - x1 > 10000 * TICK_IN_MS)
		{
			fprintf(stderr,"########## recevied sfi with delay 10000 prev %d %d cur %d %d\n",g_sfi_ts.tv_sec,
					g_sfi_ts.tv_usec,sfi_ts.tv_sec, sfi_ts.tv_usec);

		}
		else if (x2 - x1 > 1800 * TICK_IN_MS)
		{
			fprintf(stderr,"########## recevied sfi with delay 1800= %d \n", (x2-x1) );
		}
		else if (x2 - x1 > 1500 * TICK_IN_MS)
		{
			fprintf(stderr,"########## recevied sfi with delay 1500= %d \n", (x2-x1) );
		} 
		else if (x2 - x1 > 1300 * TICK_IN_MS)
		{
			//fprintf(stderr,"########## recevied sfi with delay 1300= %d \n", (x2-x1) );
		} 
		else if (x2 - x1 > 1100 * TICK_IN_MS)
		{
			//fprintf(stderr,"########## recevied sfi with delay 1100= %d \n", (x2-x1) );
		} 
		g_sfi_ts = sfi_ts;

		sfnsf_p = ( FAPI_subFrameIndication_st *) &( l1ApiMsg_p->msgBody[0]);
#if 0
		sfn = (uint16_t)(MAC_PHY_CONVERT_16(sfnsf_p->sfnsf) & FAPI_SFN_MASK);
		sfn >>= FAPI_SFN_VALUE_SHIFT;
		sf = (uint8_t)(MAC_PHY_CONVERT_16(sfnsf_p->sfnsf) & FAPI_SF_MASK);
		//fprintf(stderr,"\n sfn = %d   sf = %d \n",sfn,sf);
		/* send tick to UE MAC on socket used internally */
		buf[0] = l1ApiMsg_p->msgId;
		LTE_SET_U16BIT(&buf[1], sfn);
		buf[3] = sf;

		//fprintf(stderr,"\n SFI recevied sfn = %d sf = %d\n", sfn, sf);
		ret =  sendto(txUePhySocketFD_g, buf,4, 0,
				(struct sockaddr*)&uePhyLayerAddr_g, sizeof(SockAddr));
		if (ret == -1)
		{
			fprintf(stderr,"SFI send to failed Error No- %d Desp: %s  in %s at Line no %d\n",\
					errno,strerror(errno), __func__, __LINE__);
			ltePanic("\n");
		}
#endif   
		pDataQnode     = (macPhyDataNode*)getMemFromPool(sizeof(macPhyDataNode), PNULL);
		pDataQnode->len = msgLen;
		pDataQnode->msg = (void *)getMemFromPool(msgLen, PNULL);
		sfnsf_p = ( FAPI_subFrameIndication_st *) &( l1ApiMsg_p->msgBody[0]);
		sf = (uint8_t)(MAC_PHY_CONVERT_16(sfnsf_p->sfnsf) & FAPI_SF_MASK);
		if ( PNULL == pDataQnode->msg )
		{
			ltePanic("memory allocation failed while recevied data from enb mac\n");
		}
		memcpy (pDataQnode->msg, data_p, msgLen);
		pushNode (&dlMsgStoreQueue, &pDataQnode->anchor);
		semPost(&execSchedulerThreadSem_g);
	}
	else
	{
		//fprintf(stderr,"\n Dl msg after Rach Ind sfn = %d   sf = %d \n",sfn,sf);
		pDataQnode     = (macPhyDataNode*)getMemFromPool(sizeof(macPhyDataNode), PNULL);
		pDataQnode->len = msgLen;
		pDataQnode->msg = (void *)getMemFromPool(msgLen, PNULL);
		sfnsf_p = ( FAPI_subFrameIndication_st *) &( l1ApiMsg_p->msgBody[0]);
		sfn = (uint16_t)(MAC_PHY_CONVERT_16(sfnsf_p->sfnsf) & FAPI_SFN_MASK);
		sfn >>= FAPI_SFN_VALUE_SHIFT;
		sf = (uint8_t)(MAC_PHY_CONVERT_16(sfnsf_p->sfnsf) & FAPI_SF_MASK);
		if ( PNULL == pDataQnode->msg )
		{
			ltePanic("memory allocation failed while recevied data from enb mac\n");
		}
		memcpy (pDataQnode->msg, data_p, msgLen);
#if 0
		if ( sf % 2 == 0 ) /* sf is even */
		{
			//fprintf(stderr,"\n Push Dl msg in Odd Queue sfn = %d   sf = %d at index = %d cuSFN = %d cuSF = %d\n",sfn,sf, sf % 2 + 1, SFN_g, SF_g);
			fprintf(stderr,"\n Push Dl msg in Odd Queue sfn = %d   sf = %d at index = %d cuSFN = %d cuSF = %d\n",sfn,sf, sf % 2, SFN_g, SF_g);
			//pushNode (&dlMsgStoreQueue[sf % 2 + 1], &pDataQnode->anchor);
			pushNode (&dlMsgStoreQueue[sf % 2], &pDataQnode->anchor);
		}
		else
#endif
		{
			//fprintf(stderr,"\n Push Dl msg in Even Queue sfn = %d   sf = %d  cuSFN = %d cuSF = %d\ index = %d\n",sfn,sf, SFN_g, SF_g, sf % 2 - 1);
			//fprintf(stderr,"\n Push Dl msg in Queue sfn = %d   sf = %d  cuSFN = %d cuSF = %d\ index = %d\n",sfn,sf, SFN_g, SF_g, sf % 2);
			//pushNode (&dlMsgStoreQueue[sf % 2 - 1], &pDataQnode->anchor);
			//pushNode (&dlMsgStoreQueue[sf % 2], &pDataQnode->anchor);
			pushNode (&dlMsgStoreQueue, &pDataQnode->anchor);
		}
		sfnsf = 0; sfn = 0; sf = 0;
	}

}
#endif



/*****************************************************************************
 * Function Name  : createExecSchedulerThreads 
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function is reposible for invoking Execution Scheduler
 *                  Threads for max number of active cells
 *****************************************************************************/
UInt8 cellId[MAX_NUM_CELLS] = {0};

void createExecSchedulerThreads()
{
	UInt16 ret = 0;
	UInt8 cellIndex = 0;
	for (cellIndex = 0; cellIndex < numCells_g ; cellIndex++)
	{
        cellId[cellIndex] = cellIndex; 
		ret = threadCreate(&execSchedulerThread_g[cellIndex],
				THREAD_CREATE_DETACHED,
				/*THREAD_SCHED_FIFO,*/
				//SCHED_FIFO,
				SCHED_RR, //rajni
				/*0,*/
				(EXECSCHEDULER_THREAD_PRIORITY),
				THREAD_EXPLICIT_SCHED,
				THREAD_SCOPE_DEFAULT,
				execSchedulerThread,
				(void *)&cellId[cellIndex]);
		if ( ret != 0)
		{
			ltePanic("Error in creating Execution Scheduler thread for Cell\
					[%d]", cellIndex);
		}
	}
}

/* SPR 12148 start */
static void processPdcchOrderNode(UInt8 cellIndex)
{
	RARequest raReq = {0};
	UInt8 i = 0;
	FAPI_ulPdcchOrderNode_st *ulPdcchOrderNode_p = PNULL; 
    /* SPR 12148 start */
	ulPdcchOrderNode_p = (FAPI_ulPdcchOrderNode_st*)getFirstNode(&pdcchOrderQueue_g[cellIndex]);
    /* SPR 12148 end */
	if (PNULL != ulPdcchOrderNode_p)
	{
		while(i < MAX_NUM_RA_REQUEST)
		{
			raReq = raRntiArr_g[cellIndex][i];
			if (INVALID_RNTI == raReq.RaRnti)
			{
				raReq.RaRnti = i+START_RARNTI;
				if (ulPdcchOrderNode_p->preambleIdx != 0)
				{    
					raReq.preamble = ulPdcchOrderNode_p->preambleIdx;
					raReq.rachTypeFlag = CONTENTION_FREE_PDCCH_ORDER_RACH;
				}
				else
				{
					raReq.preamble = getNextPreambleIndex();
					raReq.rachTypeFlag = CONTENTION_BASED_PDCCH_ORDER_RACH; 
				}
				raReq.rnti = MAC_PHY_CONVERT_16(ulPdcchOrderNode_p->crnti);
				raRntiArr_g[cellIndex][raReq.RaRnti - 1] = raReq;
				break;
			}
			i++;
		}
		if(MAC_SUCCESS == fapiPrepareRAInfo(raReq.RaRnti - 1, cellIndex))
		{
			ulPdcchOrderNode_p = (FAPI_ulPdcchOrderNode_st*)popNode(&pdcchOrderQueue_g[cellIndex]);
			freeMemPool(ulPdcchOrderNode_p);
			ulPdcchOrderNode_p = PNULL; 
		}
	}
	else 
	{
		fprintf(stderr, " INAVLID PdcchOrderQ Node \n");
	}
	return ;
}
/* SPR 12148 end */

/*****************************************************************************
 * Function Name  :  processRxQueues
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function will process queues for comm.
 *                  between UE PHY <-> UE RRC/OAM  
 *****************************************************************************/
void processRxQueues()
{
	static UInt8 turn                 = 0;
	UInt64 numPdu              = 0;
	UInt8 i                    = 0;
	static UInt16 rachReqCount = 0;
    UInt8 cellIndex = 0;    
#if 0
	if ( !stoppedProcessing_g[0] && rlcLayerInitialized_g &&
			pdcpContextIsInitialized() )
	{
		turn--;

	}
	else
	{
		turn = 0;
	}
	if(!turn)
	{
		turn = 100;
#endif
		/* SPR 609 changes start */
		numPdu = sQueueCount(&rrcMsgQ_g);
		if(numPdu)
			/* SPR 609 changes end */
		{
			readRRCMsgFromSocket();
		}
		numPdu = sQueueCount(&rlcRrcOamMsgQ_g);
		if(numPdu)
			/* SPR 609 changes start */
		{
			//if ( CLEANUP_LAYER2 == rlcRcvMsgOnSocket())
			if ( CLEANUP_LAYER2 == rlcRcvMsgOnQ())
			{
				cleanupUePhySim(&rxEnodebRRCSocketFD_g,
						&txEnodebRRCSocketFD_g);
			}

		}
		// Shashank for pdcp layer oam rrc msg
		/* SPR 609 changes start */
		numPdu = sQueueCount(&pdcpRrcOamMsgQ_g);
		if(numPdu)
			/* SPR 609 changes start */
		{
			pdcpElTxProcessMsgQ();
		}
		#if 0
	}
	#endif
	/* CA_NA need to discuss */
#if 0
	if ( hoRachTriggerFlag ) 
	{
		if ( turn % 4 == 0  && rachReqCount < ueHoCount_g )  
		{
			fprintf(stderr," rachReqCount = %d RACH started raIndex = %d\n",rachReqCount, hoInfo_g[rachReqCount].raReqIndex);
			fapiPrepareRAInfo( hoInfo_g[rachReqCount].raReqIndex);
			rachReqCount++;
		}
		if ( rachReqCount == ueHoCount_g )
		{
			/* reset the static variables now */
			for ( i = 0; i < ueHoCount_g; i++)
			{
				hoInfo_g[i].raReqIndex = INVALID_RNTI;
				hoInfo_g[i].cellId = cellId_g;
			}
			ueHoCount_g = 0;
			hoRachTriggerFlag = FALSE;
			rachReqCount = 0;
		}
	}
#endif
	//turn--;
    /* SPR 12148 start */
    for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {        
        numPdu = sQueueCount(&pdcchOrderQueue_g[cellIndex]);
        if (numPdu > 0)
        {
            if(0 == rlfDebugFlag)
            {		 
                processPdcchOrderNode(cellIndex);
            } 
            else
            {
                FAPI_ulPdcchOrderNode_st *ulPdcchOrderNode_p = PNULL; 
                ulPdcchOrderNode_p = (FAPI_ulPdcchOrderNode_st*)popNode(&pdcchOrderQueue_g[cellIndex]);
                freeMemPool(ulPdcchOrderNode_p);
                ulPdcchOrderNode_p = PNULL; 
            }
		}  
	}
    /* SPR 12148 end */
	/* CA_NA need to discuss */
}
/*****************************************************************************
 * Function Name  :  initExecSchedulerQueues
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function will initilaize cell specific queues
 *****************************************************************************/
void initExecSchedulerQueues(UInt8 cellIndex)
{
	sQueueInit(&enodebRrcPhyMsgQ_g[cellIndex]);
	sQueueInit(&macPhyDataQ_g[cellIndex]);
	UInt8 i = 0;
#ifdef TDD_CONFIG
	for( i = 0; i < MAX_UL_ARRAY; i++)
	{
		sQueueInit(&harqQueue[cellIndex][i]);
		sQueueInit(&rachIndQueue[cellIndex][i]);
		sQueueInit(&crcIndQueue[cellIndex][i]);
		sQueueInit(&srIndQueue[cellIndex][i]);
		sQueueInit(&onlyBsrPhrQueue[cellIndex][i]);
		sQueueInit(&ulDataQueue[cellIndex][i]);
		sQueueInit(&cqiIndQueue[cellIndex][i]);
		sQueueInit(&srsIndQueue[cellIndex][i]);
	}
#elif FDD_CONFIG
    /* SPR 11549 fix start */
    for( i = 0; i < MAX_RACH_ARRAY; i++)
    {
        sQueueInit(&rachIndQueue[i]);
    }
    /* SPR 11549 fix end */
	for( i = 0; i < MAX_SUBFRAME; i++)
	{
		sQueueInit(&harqQueue[cellIndex][i]);
		sQueueInit(&crcIndQueue[cellIndex][i]);
		sQueueInit(&srIndQueue[cellIndex][i]);
		sQueueInit(&onlyBsrPhrQueue[cellIndex][i]);
		sQueueInit(&ulDataQueue[cellIndex][i]);
		sQueueInit(&srsIndQueue[cellIndex][i]);
	}
#endif
}

/*****************************************************************************
 * Function Name  :  updateMaxFDVal
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function will update max FD value 
 *****************************************************************************/

static inline void updateMaxFDVal(UInt8 startIndex, UInt8 endIndex, SInt32 *maxFd, SInt32 *maxFDArray)
{
	UInt8 count = 0;
	for(count = startIndex; count < endIndex; count++)
	{
		if (*(maxFDArray + count) > *maxFd )
		{
			*maxFd = *(maxFDArray + count);
		}
	}
	(*maxFd) += 1;
}
/*LWA_Changes*/
/*LWA_Changes*/
/*****************************************************************************
 * Function Name  :  rxTcFromOamRrc
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function will create the Sockets needed for comm.
 *                  between UE PHY <-> UE RRC/OAM  and Enodeb PHY <-> Enodeb RRC.
 *                  Also the receving port for any communication between UE RRC/OAM
 *                  and other UE modules of MAC(emulator), RLC and PDCP is here.
 *                  and will call function which will process queues for comm.
 *                  between UE PHY <-> UE RRC/OAM  and Enodeb PHY <-> Enodeb RRC.
 *****************************************************************************/
/* + SPR 11348 Changes */
#define MAX_TX_DATA_ARR_NUM 40
 UInt8 txDataBuff[MAX_NUM_CELLS][MAX_TX_DATA_ARR_NUM][RECV_BUFF_SIZE] = {0,0,0};
 static UInt8 i[MAX_NUM_CELLS];
/* - SPR 11348 Changes */
void rxTxFromOamRrc()
{

/*LWA_Changes*/
/*LWA_Changes*/
	SInt32 maxFd    = 0;  
	UInt8 startIdx = 0;
	UInt8 flag = 1;
	UInt8 isFdSet = FALSE;
	UInt8 cellIndex = 0;
	SInt32 maxFDArray[MAX_FD_ELEMENTS] = {0}; 
	UInt8 msgBuf[MAX_DATA_SIZE];
	rrcOamMsgNode    *pQnode      = NULL;
	UInt8 lastUpdatedIdx =0;
#ifndef UE_SIM_SPLIT
	macPhyDataNode    *pDataQnode      = NULL;
#endif
	SockAddr rxEnodebPhyClient;
	UInt32 addrLen = sizeof(rxEnodebPhyClient);
	UInt32 bytesRead = 0;
	SockAddr rrc_addr;
	UInt32 addr_len = sizeof(rrc_addr);
	UInt32 addr_len_rrc = sizeof(rrc_addr);
	socklen_t length = sizeof(struct sockaddr_in);
	socklen_t len = sizeof(struct sockaddr_in);

	memSet(&rrc_addr, 0x0, sizeof(rrc_addr));
	memSet(&rxEnodebPhyClient, 0, addrLen);
#if 0
	/* ################## SOCKET INITIALISATION FOR RRC ################## */
	SInt32 txUeRRCSocketFD;
	SInt32 rxEnodebRRCSocketFD_g;
	SInt32 txEnodebRRCSocketFD_g;

	createSocketsForRrcPhyInterface(
			&rxUeRRCSocketFD,
			&txUeRRCSocketFD,
			&rxEnodebRRCSocketFD_g,
			&txEnodebRRCSocketFD_g);

#endif
	/* ################## ****************************** ################## */

	maxFDArray[0] = rxSockFD_g;         /* Read socket for MAC EMULATOR */
	maxFDArray[1] = readSockFD_g;       /* Read socket for UE RLC  */
	maxFDArray[2] = pdcpSocketRx;        /* Read socket for UE PDCP */
	// maxFDArray[3] = rxUeRRCSocketFD;    /* Read socket for UE PHY  */
	lastUpdatedIdx = 3;
	updateMaxFDVal(startIdx, lastUpdatedIdx, &maxFd, maxFDArray);
#ifdef UESIM_PDCP_THREAD
	threadSetRtPriority(SCHED_FIFO, 35);
#else
	threadSetRtPriority(SCHED_FIFO, 45);
#endif
	fd_set readFds;
	FD_ZERO(&readFds);


	while (1)
	{
		/*
		 **
		 **  We receive incoming data on socket form 
		 **  UE RRC for  (i) UE PDCP, (ii) UE RLC, 
		 **  (iii) UE MAC(Emulator),  (iv) UE PHY
		 **  and from Enodeb RRC to Enodeb PHY.
		 **
		 */ 
		FD_ZERO(&readFds);
		FD_SET(rxSockFD_g, &readFds);  
		FD_SET(pdcpSocketRx, &readFds);  
		FD_SET(readSockFD_g, &readFds);  
		//FD_SET(rxUeRRCSocketFD, &readFds);  
		/* SPR 5313 changes start */
		for (cellIndex = 0; cellIndex < numCells_g ; cellIndex++)
		{
			if( 0 == cellId_g[cellIndex] )
			{
				FD_SET(sockfd_g[cellIndex], &readFds);  
			}
            else
            {
                FD_SET(sockfdHO_g[cellIndex], &readFds);
            }
			FD_SET(rxEnodebRRCSocketFD_g[cellIndex], &readFds);
		}
		cellIndex = 0;
		/* SPR 5313 changes end */

		if (select	(maxFd, &readFds, NULL, NULL, NULL) == -1)
		{
			lteWarning("select failed.....!!!!\n");
			continue;
		}

		if(FD_ISSET(rxSockFD_g, &readFds))
		{
			fprintf(stderr,"data received for MAC Emulator\n ");
			if ((bytesRead = recvfrom(rxSockFD_g, msgBuf, MAX_DATA_SIZE, 0,
							(struct sockaddr*)&rrc_addr, &addr_len))
					== -1)
			{
				ltePanic ("Error in receive message.\n");
				continue;
			}

			pQnode     = getMemFromPool (sizeof(rrcOamMsgNode), PNULL);
			pQnode->msg = getMemFromPool(bytesRead, PNULL);
			memcpy (pQnode->msg, msgBuf, bytesRead);
			pushNode (&rrcMsgQ_g, &pQnode->anchor);
			//shashank this funcion sud be called from exec thread
			//readRRCMsgFromSocket(); 
			FD_CLR(rxSockFD_g, &readFds);  
			fprintf(stderr,"after FD CLR rxSockFD_g id %d \n", rxSockFD_g);
			isFdSet = TRUE;
		}  

		else if(FD_ISSET(readSockFD_g, &readFds))
		{
			fprintf(stderr,"data received for UESIM RLC\n ");
			if((bytesRead = READ_FROM_SOCKET(msgBuf,rrc_addr,addr_len_rrc)) == -1 )
			{
				ltePanic ("Error in receive message.\n");
				continue;
			}

			pQnode     = getMemFromPool (sizeof(rrcOamMsgNode),PNULL);
			pQnode->len = bytesRead;
			pQnode->msg = getMemFromPool(bytesRead, PNULL);
			memcpy (pQnode->msg, msgBuf, bytesRead);
			pushNode (&rlcRrcOamMsgQ_g, &pQnode->anchor);
#if 0
			if ( CLEANUP_LAYER2 == rlcRcvMsgOnSocket())
			{
				cleanupUePhySim(&rxEnodebRRCSocketFD_g,
						&txEnodebRRCSocketFD_g);
			}
#endif
			FD_CLR(readSockFD_g, &readFds);  
			isFdSet = TRUE;
		}   
		else if(FD_ISSET(pdcpSocketRx, &readFds))
		{
			fprintf(stderr,"data received for UESIM PDCP\n ");
			bytesRead = ReadFromSocket( msgBuf, PDCP_MODULE_ID );

			pQnode     = getMemFromPool (sizeof(rrcOamMsgNode), PNULL);
			pQnode->len = bytesRead;
			pQnode->msg = getMemFromPool(bytesRead, PNULL);
			memcpy (pQnode->msg, msgBuf, bytesRead);
			pushNode (&pdcpRrcOamMsgQ_g, &pQnode->anchor);
			//shashank put in Q TODO
			FD_CLR(pdcpSocketRx, &readFds);  
			isFdSet = TRUE;
#if 0
			pdcpElTxProcessSocket();
#endif
		}

		for (cellIndex = 0 ; cellIndex < numCells_g ; cellIndex++)
		{
			if(FD_ISSET(sockfd_g[cellIndex], &readFds))
			{
/*LWA_Changes*/
/*LWA_Changes*/
				if ( ( bytesRead = recvfrom(sockfd_g[cellIndex], recvDataBuffer_g,
								RECV_BUFF_SIZE, 0,
								(struct sockaddr*)&phyLayerAddr_g[cellIndex],&length)) <= 0)
/*LWA_Changes*/
/*LWA_Changes*/

				{
					if (errno != EAGAIN)
					{
						perror("recv from error in receiver thread");
						ltePanic ("lte panic due to recv from error\n" );
					}
					continue;
				}
/*LWA_Changes*/
/*LWA_Changes*/
#ifdef UE_SIM_SPLIT
				processDlMsgRecvFromEnbPhyOnDataSocket( &recvDataBuffer_g, bytesRead);
#else
				pDataQnode     = (macPhyDataNode*)getMemFromPool(sizeof(macPhyDataNode), PNULL);
				pDataQnode->len = bytesRead;
/* + SPR 11348 Changes */
				//pDataQnode->msg = (void *)getMemFromPool(bytesRead, PNULL);
			//	if ( PNULL == pDataQnode->msg ) 
				{
			//		ltePanic("memory allocation failed while recevied data from enb mac\n");
				}
				//memcpy (pDataQnode->msg, recvDataBuffer_g, bytesRead);
                memSet(txDataBuff[cellIndex][i[cellIndex]],0,RECV_BUFF_SIZE);
				memcpy (txDataBuff[cellIndex][i[cellIndex]], recvDataBuffer_g, bytesRead);
				pDataQnode->msg = (void *)txDataBuff[cellIndex][i[cellIndex]];
                i[cellIndex]++;
                i[cellIndex] = ((i[cellIndex])%MAX_TX_DATA_ARR_NUM);
/* - SPR 11348 Changes */
				pushNode (&macPhyDataQ_g[cellIndex], &pDataQnode->anchor);
#endif
				FD_CLR(sockfd_g[cellIndex], &readFds); 
				isFdSet = TRUE;
/*LWA_Changes*/
/*LWA_Changes*/
			}
			/* Communication for eNB MAC/eNBRRC*/
			if(FD_ISSET(rxEnodebRRCSocketFD_g[cellIndex], &readFds))
			{
				if ((bytesRead = recvfrom(rxEnodebRRCSocketFD_g[cellIndex], msgBuf, MAX_DATA_SIZE, 0,
								(struct sockaddr*)&rxEnodebPhyClient, &addrLen))== -1)
				{
					ltePanic ("Error in receive message.\n");
					continue;
				}
				pQnode     = getMemFromPool (sizeof(rrcOamMsgNode), PNULL);
				pQnode->len = bytesRead;
				pQnode->msg = getMemFromPool(bytesRead, PNULL);
				memcpy (pQnode->msg, msgBuf, bytesRead);
				pushNode (&enodebRrcPhyMsgQ_g[cellIndex], &pQnode->anchor);
				// processEnodebRrcPhyMsg(rxEnodebRRCSocketFD_g, txEnodebRRCSocketFD_g);
				FD_CLR(rxEnodebRRCSocketFD_g[cellIndex], &readFds);
				isFdSet = TRUE;
			}   
		/* SPR 5313 changes start */
            if(FD_ISSET(sockfdHO_g[cellIndex], &readFds))
            {
                if ( ( bytesRead = recvfrom(sockfdHO_g[cellIndex], recvDataBuffer_g,
                                RECV_BUFF_SIZE, 0,
                                (struct sockaddr*)&phyLayerAddrHO_g[cellIndex],&length)) <= 0)
                {
                    if (errno != EAGAIN)
                    {
                        perror("recv from error in receiver thread");
                        ltePanic ("lte panic due to recv from error\n" );
                    }
                    continue;
                }
#ifdef UE_SIM_SPLIT
                processDlMsgRecvFromEnbPhyOnDataSocket( &recvDataBuffer_g[0], bytesRead);
#else
                pDataQnode     = (macPhyDataNode*)getMemFromPool(sizeof(macPhyDataNode), PNULL);
                pDataQnode->len = bytesRead;
                /** SPR 14948 Changes Start **/
             //   pDataQnode->msg = (void *)getMemFromPool(bytesRead, PNULL);
               // if ( PNULL == pDataQnode->msg )
               // {
                 //   ltePanic("memory allocation failed while recevied data from enb mac\n");
               // }
                //memcpy (pDataQnode->msg, recvDataBuffer_g, bytesRead);
                memSet(txDataBuff[cellIndex][i[cellIndex]],0,RECV_BUFF_SIZE);
                memcpy (txDataBuff[cellIndex][i[cellIndex]], recvDataBuffer_g, bytesRead);
                pDataQnode->msg = (void *)txDataBuff[cellIndex][i[cellIndex]];
                i[cellIndex]++;
                i[cellIndex] = ((i[cellIndex])%MAX_TX_DATA_ARR_NUM);
                /** SPR 14948 Changes End **/
                pushNode (&macPhyDataQ_g[cellIndex], &pDataQnode->anchor);
#endif
                FD_CLR(sockfdHO_g[cellIndex], &readFds);  
                /*START: HO Fix*/
		        isFdSet = TRUE;
                /*END: HO Fix*/
            }
		/* SPR 5313 changes end */
        }
#if 0
		else if(FD_ISSET(rxUeRRCSocketFD, &readFds))
		{
			/* The function for UE side handling is yet to arrive */ 
			FD_CLR(rxUeRRCSocketFD, &readFds);  
		}
#endif
		if(TRUE != isFdSet)
		{
			fprintf(stderr,"error case\n ");
			// Error case handling
		}
		else
		{
			isFdSet = FALSE;
		}
		processRxQueues();
		if(1 == flag)
		{
			if (numCells_g)
			{
				fillIpAndPortConfigInfo();
				parseConfigFiles();
				createExecSchedulerThreads();
				for (cellIndex = 0; cellIndex < numCells_g; cellIndex++)
				{
					maxFDArray[lastUpdatedIdx] = rxEnodebRRCSocketFD_g[cellIndex];   /* read socket for Enodeb PHY */
					updateMaxFDVal(lastUpdatedIdx, lastUpdatedIdx + 1, &maxFd, maxFDArray );
					lastUpdatedIdx++;
				}

				flag = 0;
			}
		}

	} /* End of While(1)*/
}


/*****************************************************************************
 * Function Name  : main 
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function is reposible for invoking entire UE Side + ENB PHY
 *****************************************************************************/


int main(int argc, char *argv[])
{
	int ret = 0;
	UInt8 flag = 1;
	UInt64 numPdu      = 0;
	UInt8  turn        = 0;
	UInt8 i = 0;
	static UInt16 rachReqCount = 0;
	UInt8 cellCount = 0;
	UInt8 numArg = 0;
	UInt8 cellIndex = 0;
	UInt8 numFixedArg = 5;
	cellCount = atoi(argv[4]);
	fprintf(stderr,"\nCellCount = %d", cellCount);
	numArg= cellCount + numFixedArg;
	fprintf(stderr,"\nargc = %d", argc);

if (argc != numArg)
{
	fprintf(stderr,"\nWrong usage !!! \nUsage is ./ueSim <core num to bind for PDCP Adaptor>, \
			<core num to bind for UE PHY Timer>,\
			<core num to bind main thread>,\
			<number of cells supported >,\
			<core num to bind Exec scheduler 1>, \
			<core num to bind Exec scheduler 2>...\n");
	return 0;
}
#ifndef UE_SIM_SPLIT

	if (argc < 6)
	{
		fprintf(stderr,"Usage is ./ueSim <core num to bind for PDCP Adaptor>," \
				"<core num to bind for UE PHY Timer>," \				
				"<core num to bind main thread>," \
				"<number of cells supported >," \
				"<core num to bind Exec scheduler 1>", \
				"<core num to bind Exec scheduler 2>\n");
		return 0;
	}
#else
	if (argc < 5)
	{
		fprintf(stderr,"Usage is ./ueSim <core num to bind for PDCP Adaptor>," \
				"<core num to bind main thread>," \
				"<core num to bind for UE PHY Timer>," \
				"<number of cells supported >," \
				"<core num to bind Exec scheduler 1>," \
				"<core num to bind Exec scheduler 2> \n");
		return 0;
	}
#endif


	pdcpAdaptorCoreNum_g =  atoi(argv[1]);
//	execSchedulerThreadCoreNum_g[0] = atoi(argv[2]);
//	execSchedulerThreadCoreNum_g[1] = atoi(argv[3]);
	//ulReceiverThreadCoreNum_g =  atoi(argv[2]);
	//

#ifndef UE_SIM_SPLIT
	uePhyTimerThreadCoreNum_g =  atoi(argv[2]);
	//uldispatcherThreadCoreNum_g =  atoi(argv[4]);
	rlcAndPdcpThreadCoreNum_g =  atoi(argv[3]);
#else
	rlcAndPdcpThreadCoreNum_g =  atoi(argv[3]);
#endif 
	for (cellIndex = 0; cellIndex < cellCount ; cellIndex++)
	{
	execSchedulerThreadCoreNum_g[cellIndex] = atoi(argv[numFixedArg]);
    numFixedArg++;
	}
/*
#ifndef UE_SIM_SPLIT
	uePhyTimerThreadCoreNum_g =  atoi(argv[4]);
	//uldispatcherThreadCoreNum_g =  atoi(argv[4]);
	rlcAndPdcpThreadCoreNum_g =  atoi(argv[5]);
#else
	rlcAndPdcpThreadCoreNum_g =  atoi(argv[4]);
#endif*/


	PROCESS_TO_BIND_SET_CORE_NO(getpid(),rlcAndPdcpThreadCoreNum_g);
	/* ################  UE STACK INIT FUNCTIONALITY ################*/

	/* 
	   A call to the function which initialises all the
	   layers on UE side viz. Mac Emulator, PDCP, RLC
	   internally the called function will call initMAC
	   which will create the three threads required for
	   communication with Enodeb MAC and processing of 
	   data at the MAC Emulator. 
	   */
	/* + SPR 11024 Fix */ 
	initUEStack(cellCount); // this func also defined in simulators/adapter/mac/src/lteMacAdapter.c
	/* - SPR 11024 Fix */ 
	/* This function initUEStack initialize the MAC , RLC , PDCP
	   initMAC();
	   initRLCLayer();
	   initPDCPLayer();
	   initPdcpAdapter();

     */

#ifndef UE_SIM_SPLIT  
	ret = threadCreate(&PhyTimerThread_g,
			THREAD_CREATE_DETACHED,
			/*THREAD_SCHED_FIFO,*/
			SCHED_FIFO,
			/*0,*/
			TIMER_THREAD_PRIORITY, 
			THREAD_EXPLICIT_SCHED,
			THREAD_SCOPE_DEFAULT,
			phyTimerThread,
			(void *) NULL );
	if ( ret != 0)
	{
		ltePanic("Error in creating Phy timer thread");
		return 0;
	}
#endif
	if ( ret != 0)
	{
		ltePanic("Error in creating ULDispatcher thread");
	}

	rxTxFromOamRrc();
	return 0;
}/* End of MAIN */
/* -CA */
