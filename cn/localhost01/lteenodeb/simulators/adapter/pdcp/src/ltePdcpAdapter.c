/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpAdapter.c,v 1.3.2.2.4.2.2.8 2010/10/29 15:54:08 gur20056 Exp $
 *
 ******************************************************************************
 *
 *  File Description : PDCP Adapter implementation
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpAdapter.c,v $
 * Revision 1.3.2.2.4.2.2.8  2010/10/29 15:54:08  gur20056
 * rupesh - multi attach detach fix in pdcpadapter ULTG DLTG pdcp and mac
 *
 * Revision 1.3.2.2.4.2.2.7  2010/10/28 11:05:56  gur20056
 * UL data corruption fix
 *
 * Revision 1.3.2.2.4.2.2.6  2010/10/26 16:39:29  gur20056
 * anil: pdcp crash fix
 *
 * Revision 1.3.2.2.4.2.2.5  2010/10/25 09:00:56  gur21010
 * Warnings Removed
 *
 * Revision 1.3.2.2.4.2.2.4  2010/10/12 12:40:19  gur20439
 * fix for removal of gtpu header from adapter and reduce the stack size to 2000 at present of 64k
 *
 * Revision 1.3.2.2.4.2.2.3  2010/10/07 13:15:01  gur23054
 * Fix for SPR 713
 *
 * Revision 1.3.2.2.4.2.2.2  2010/09/29 06:29:19  gur04640
 * Change priority of PDCP adapter thread to 90 and policy to SCHED_FIFO
 *
 * Revision 1.3.2.2.4.2.2.1  2010/09/21 15:53:11  gur20491
 * FAPI changes
 *
 * Revision 1.3.2.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.11  2010/03/15 10:19:09  gur20439
 * commented all clock_getTime system calls
 *
 * Revision 1.10  2010/03/11 08:33:27  gur18550
 * Higher MIMO Config
 *
 * Revision 1.9  2010/03/09 09:50:14  gur15697
 * 4by4 MIMO changes
 *
 * Revision 1.8  2010/03/09 06:57:16  gur20439
 * core binding chnages
 *
 * Revision 1.7  2010/03/08 13:23:44  gur19479
 * updated for pdcp adapter
 *
 * Revision 1.6  2010/03/08 07:55:54  gur19479
 * updated for optimization
 *
 * Revision 1.5  2010/03/02 12:20:28  gur15697
 * Optimization changes to avoid memset
 *
 * Revision 1.4  2010/02/18 09:52:44  gur18550
 * Core Binding no updated
 *
 * Revision 1.3  2010/02/18 07:38:33  gur19479
 * updated for UE TG
 *
 * Revision 1.1.2.9.2.6  2009/12/05 13:05:38  gur19479
 * updated for thread binding
 *
 * Revision 1.1.2.9.2.5  2009/11/25 10:06:35  gur19479
 * updated for GTP-U
 *
 * Revision 1.1.2.9.2.4  2009/11/19 10:19:26  gur19479
 * updated for PPP Sock_SEQSTREAM Protocol
 *
 * Revision 1.1.2.9.2.3  2009/11/18 14:21:51  gur19479
 * *** empty log message ***
 *
 * Revision 1.1.2.9.2.2  2009/11/18 14:19:52  gur19479
 * updated for ppp unix sockets
 *
 * Revision 1.1.2.9.2.1  2009/09/24 21:21:59  gur19479
 * updated for endianess issue
 *
 * Revision 1.1.2.9  2009/08/11 07:58:46  gur20435
 * Changed core affinity to core 0
 *
 * Revision 1.1.2.8  2009/08/07 07:12:37  gur19140
 * resolve rx issue
 *
 * Revision 1.1.2.7  2009/08/06 13:26:15  gur19836
 * Graceful Exit code added
 *
 * Revision 1.1.2.6  2009/07/30 08:28:56  gur19140
 * performance changes
 *
 * Revision 1.1.2.5  2009/07/28 15:36:45  gur19836
 * Downlink Issues Resolved, Latency Measurement code added
 *
 * Revision 1.1.2.4  2009/07/16 12:40:02  gur11318
 * updated to remove compilation issues
 *
 * Revision 1.1.2.3  2009/07/15 14:10:17  gur19836
 * Changes done for setting default params if config file not present
 *
 * Revision 1.1.2.2  2009/07/03 09:19:38  gur19479
 * updated for  reading conf file
 *
 * Revision 1.1.2.1  2009/06/30 12:44:29  gur19836
 * Initial Version
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpAdapter.h"
#include "lteMisc.h"
#include "lteMessageQIntf.h"
#include "lteLayer2CommanTypes.h"
#ifdef LAYER2_DLTG_THREAD
#include "lteLayer2DLTG.h"
#endif
#include "lteCommonStatsManager.h"
/* SPR 2241 changes start */
#include "lteRelay.h"
/* SPR 2241 changes end */
#ifdef PDCP_ASYNC_INTERFACE
#include "ltePdcpSecEngine.h"
#endif


#include "lteConfigFileHdlr.h"

#ifdef L2_FAST_PATH
#ifndef UE_SIM_TESTING
#include "egtpu_glob.h"
#include "egtpu_msgtypes.h"
#include "egtpu_ietypes.h" 
#include "egtpu_glob_struct.h"
#include "egtpu_apihdr.h"
#include "ltePdcpContext.h"
#include "egtpu_cdb.h"
#include "ltePdcpPdcpUserSap.h"
#endif
#endif

 /* ECN changes start */
#include "ltePdcpRRMInterface.h"
#include "ltePdcpRRMUtils.h"
#include "lteMessageQIntf.h"
/* ECN changes end */

/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
/******************************************************************************
  Private Definitions
 *****************************************************************************/
#ifdef L2_FAST_PATH
#ifndef UE_SIM_TESTING
extern egtpu_global_t *p_global;
extern PdcpContext pdcpContext;
extern MSGPOOL MsgPool_g;
#endif
#endif

#ifdef L2_FAST_PATH
void lteUserPlaneFastPath(UInt8 *recvData_p, UInt32 bytesRead);
#endif


/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
/* TODO sync changes */

/******************************************************************************
  Private Types
 *****************************************************************************/

#ifdef PDCP_ASYNC_INTERFACE
#define MAX_PACKETS_TX_SEC_Q 500
#endif

#define START_OFFSET_UL_GTP 16
/*SPR 3587 Fix Start*/
#define START_OFFSET_DL_GTP 64
/*SPR 3587 Fix End*/

#define PDCP_HEADER_SPACE 2
#define PDCP_TRAILER_SPACE 4
 
#define PDCP_ADAPTER_BUFFER_SIZE        2048
#define PDCP_ADAPTER_MAX_BUFFERS        5000
#define MSG_POOL_HIGH_WATERMARK         70 
#define MSG_POOL_LOW_WATERMARK          60 
#define MEM_POOL_HIGH_WATERMARK         70 
#define MEM_POOL_LOW_WATERMARK          60 
/* Fix for SPR 2008 start */
#ifndef UE_SIM_TESTING
#define MAX_PACKETS_TO_SERVE             400
#define MAX_QUEUE_LOAD_TO_SERVE        40000
#endif
/* Fix for SPR 2008 end */
static ULong32 recvThId_g;
static UInt8 ueContext_g[MAX_UE+1][MAX_LC+1];
//static MSGPOOL pdcpAdapterMsgPool_g;
UInt32 rlcTxQueueSize_g = 0;
#ifdef UE_SIM_TESTING
static SockAddr sockAddrTx_g;
#endif
/*SPR 3587 Fix Start*/
SockAddr sockAddrRx_g;
/*SPR 3587 Fix End*/
/*IPV6_CHANGE - START*/
struct sockaddr_in6 sockAddrRx6_g;
UInt8 sockAddrType;
/*IPV6_CHANGE - END*/
static SInt8 sockTx_g;
SInt8 sockRx_g;
SInt8 sockRx6_g;
LP_ConfigFileFormat configData = PNULL;
#ifdef UE_SIM_TESTING
static int retTh;
#endif

SInt32 sockRxTid_g = -1;/* Socket Descriptor for Tunnel ID reception*/

/* Table for storing the tunnel id corresponding to ue id */
adapterDataBase_t adapterDb_g[ADAPTER_MAX_UE];
SInt16 lastUeInSystem_g = -1;
static UInt8 pumpFlag = ALLOWED;

/* SPR 2241 changes start */
extern Int pdcpSocketRx;
extern relayConfigInfo relayConfigInfo_g;
extern void pdcpElTxProcessSocket(void);
/* SPR 2241 changes end */

/* ECN changes start */
extern mqd_t fdForReceivingPdcpMsgFromRRM_g;
/* ECN changes end */

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
#ifdef UE_SIM_TESTING
static void printConfig(LP_ConfigFileFormat configData);
static void writeToSocket(SInt8 fd, void*msgPtr, UInt16 msgLen, 
                          Char8 *hostname_p, UInt16 port);
#endif
static void setConfig(LP_ConfigFileFormat);
static UInt8 initSocketAddress(SockAddr *s,SInt8 *sockfd,char *ipAddr,UInt16 port);
static UInt8 initSocketAddress6(struct sockaddr_in6 *s,SInt8 *sockfd,UInt8 *ipAddr, UInt16 port);
void * normalMode(void*arg);
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern UInt32 processPeerGtpuDataInd(UInt8 *data_p, UInt32 trans_id, UInt32 totalLen);
UInt8 adapterInitialized_g = 0;
UInt16 pdcpAdapterMode_g = 1;
UInt16 pdcpAdapterDelay_g = 5;
#ifdef UE_SIM_TESTING
extern int pdcpAdaptorCoreNum_g; 
#endif
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
UInt64 pdcpUserTxThroughput_g = 0;
UInt64 pdcpUserRxThroughput_g = 0;
UInt32 pdcpUserNumOfPackets_g = 0;

#ifndef GTPU_UT_FRWK
  /* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g;
  /* SPR 15909 fix end */
#endif
extern void diffLatency(TimeSpec start, TimeSpec end, UInt8 index, UInt64 count);

/****************************************************************************
 * Functions implementation
 ****************************************************************************/
#ifdef L2_FAST_PATH
/****************************************************************************
* Function Name  : freeMemPool2Args
* Inputs         : Pointer to be freed
* Outputs        : None
* Returns        : Void
* Variables      :
* Description    : This function free's the pointer. New fuction required to
*                  support two aruguments required by CSPL
******************************************************************************/
void freeMemPool2Args (void *p , void *q)
{
  qvFree(q);
  return;
}
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
#endif


#ifndef GTPU_UT_FRWK
void getPdcpUserThroughput()
{
  /* SPR 15909 fix start */
  static tickType_t pdcpTxTickThroughput_g = 0;
  tickType_t diffTxTime_g = pdcpCurrentTime_g - pdcpTxTickThroughput_g;
  /* SPR 15909 fix end */
  UDouble64 pdcpUserTxThrougput = 0;
  UDouble64 pdcpUserRxThrougput = 0;
  LTE_LOG(LOG_INFO,PNULL,"Number of packets send by PDCP user = %d\n",pdcpUserNumOfPackets_g);

  if(pdcpCurrentTime_g > pdcpTxTickThroughput_g){
    diffTxTime_g = pdcpCurrentTime_g  - pdcpTxTickThroughput_g;
  }
  else {
    /* SPR 15909 fix start */
    diffTxTime_g = (0xFFFFFFFFFFFFFFFF  - pdcpTxTickThroughput_g) + (pdcpCurrentTime_g + 1);
      /* SPR 15909 fix end */
  }

  if(diffTxTime_g){
    pdcpUserTxThrougput = ((UDouble64)(pdcpUserTxThroughput_g*8))/diffTxTime_g;
    pdcpUserTxThrougput = (pdcpUserTxThrougput*1000)/(1024*1024);
    LTE_LOG(LOG_INFO,PNULL,"PDCP Adapter Tx Throughput is = %6f Mbps\n\n",pdcpUserTxThrougput);

    pdcpUserRxThrougput = ((UDouble64)(pdcpUserRxThroughput_g*8))/diffTxTime_g;
    pdcpUserRxThrougput = (pdcpUserRxThrougput*1000)/(1024*1024);
    LTE_LOG(LOG_INFO,PNULL,"PDCP Adapter Rx Throughput is = %6f Mbps\n\n",pdcpUserRxThrougput);
  }
  else {
    LTE_LOG(LOG_INFO,PNULL,"PDCP Adapter Throughput is = 0.000000 Mbps\n\n");
    LTE_LOG(LOG_INFO,PNULL,"PDCP Tx Throughput is = 0.000000 Mbps\n");
    LTE_LOG(LOG_INFO,PNULL,"PDCP Rx Throughput is = 0.000000 Mbps\n");
  }

  pdcpUserTxThroughput_g = 0;
  pdcpUserRxThroughput_g = 0;
  pdcpTxTickThroughput_g = pdcpCurrentTime_g;
}
#endif
/****************************************************************************
 * Function Name  : cleanupPdcpAdapter
 * Inputs         : None
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function will cleanup the PDCP Adapter.
 ****************************************************************************/
void cleanupPdcpAdapter()
{
    if(adapterInitialized_g == 0){
        /* SPR 2030 fix Start*/
#ifndef UE_SIM_TESTING
        LOG_MSG(PR_ADAPTER_NOT_INITIALIZED, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g,__LINE__,
                0,0,0,0, 0,0, __func__, "");
#endif
        /* SPR 2030 fix End*/
        return;
    }
    
    threadCancel(recvThId_g);
    close_wrapper(sockRx_g);
    sockRx_g = 0;
    close_wrapper(sockTx_g);
    sockTx_g = 0;
#ifdef BUILD_EXE
    cleanupMemPool();
    cleanupMsgPool();
#endif
    adapterInitialized_g = 0;
}

/****************************************************************************
 * Function Name  : initInternetSocket
 * Inputs         : None
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function is the initialization function for opening 
 *                : Internet Sockets.
 ****************************************************************************/
void initInternetSocket(LP_ConfigFileFormat configData)
{
    /* Initialize Rx and Tx Sockets to communicate with traffic generator */
    /*IPV6_CHANGE - START*/
    if (IPV4_ADDR_PRESENT & configData->addrPresenceSt)
    {
        if(initSocketAddress(&sockAddrRx_g,&sockRx_g,configData->adapterIP,configData->adapterRxPort) == 0)
        {
            /* SPR 2030 fix Start*/
#ifdef UE_SIM_TESTING
            ltePanic("Rx socket creation fail:IPv4\n");
#else         
            LOG_MSG(L2_CREATE_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0 ,
                    0,0, 0,0, __func__, "");
#endif
            /* SPR 2030 fix End*/
        }
        sockAddrRx_g.sin_addr.s_addr = inet_addr_wrapper(configData->adapterIP);
        LTE_LOG(LOG_INFO,PNULL," configData->adapterIP = %s port = %d\n",configData->adapterIP,configData->adapterRxPort);
/*SPR 21653 changes start*/
	if(bind_wrapper(sockRx_g,&sockAddrRx_g,sizeof(SockAddr))== -1)
	{
		/* SPR 2030 fix Start*/
#ifdef UE_SIM_TESTING
		ltePanic("Bind Failed for PDCP Adapter Rx socket:IPv4\n");
#else
		LOG_MSG(L2_BIND_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
				relayGtpuCurrentTick_g,__LINE__,
				0,0 ,
				0,0, 0,0, __func__, "");

#endif
		/* SPR 2030 fix End*/
	}
/*SPR 21653 changes end*/
#if (!defined UE_SIM_TESTING || defined UESIM_PDCP_THREAD )
        if(fcntl3_wrapper(sockRx_g, F_SETFL, O_NONBLOCK) < 0)
        {
            ltePanic("fcntl()");
        }
#endif
    }
    if (IPV6_ADDR_PRESENT & configData->addrPresenceSt)
    {
        if(initSocketAddress6(&sockAddrRx6_g,&sockRx6_g,configData->adapterIP6,configData->adapterRxPort) == 0)
        {
#ifdef UE_SIM_TESTING
            ltePanic("Rx socket creation fail:IPv6\n");
#else         
            LOG_MSG(L2_CREATE_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0 ,
                    0,0, 0,0, __func__, "");
#endif
        }
/*SPR 21653 changes start*/

	if(bind(sockRx6_g,(struct sockaddr*)&sockAddrRx6_g,sizeof(struct sockaddr_in6))== -1)
	{
#ifdef UE_SIM_TESTING
		ltePanic("Bind Failed for PDCP Adapter Rx socket:IPv6\n");
#else
		LOG_MSG(L2_BIND_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
				relayGtpuCurrentTick_g,__LINE__,
				0,0 ,
				0,0, 0,0, __func__, "");

#endif
	}

/* MULTI_SECTOR */
/*SPR 21653 changes end*/
#ifndef UE_SIM_TESTING
        if(fcntl(sockRx6_g, F_SETFL, O_NONBLOCK) < 0)
        {
            ltePanic("fcntl()");
        }
#endif
    }
/*IPV6_CHANGE - END*/
#ifdef UE_SIM_TESTING
    if(initSocketAddress(&sockAddrTx_g,&sockTx_g,configData->trafficGeneratorIP,configData->trafficGeneratorPort) == 0)
    {
        ltePanic("Tx socket creation fail\n");
    }
#endif
}

/****************************************************************************
 * Function Name  : initPdcpAdapter
 * Inputs         : None
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function is the initialization function for PDCP 
 *                  Adapter.
 ****************************************************************************/
void initPdcpAdapter()
{
    if(adapterInitialized_g == 1) {
        /* SPR 2030 fix Start*/
#ifdef UE_SIM_TESTING
         LTE_LOG(LOG_WARNING, PNULL, "Adapter already initialized\n");
#else
        LOG_MSG(PR_ADAPTER_ALREADY_INITIALIZED, LOGINFO, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g,__LINE__,
                0,0 ,
                0,0, 0,0, __func__, "");
#endif
        /* SPR 2030 fix End*/
        return;
    }
    
    /* 255 UE changes */
    createMemPool(BUFFERSIZE, 15000);
    /* Start 128 UE: Memory Changes */
    createMemPool(sizeof(ConfigFileFormat), 1);
#ifdef LAYER2_DLTG_THREAD
    createMemPool(sizeof(dlTgDataPkt), MAX_PKTS_IN_QUE + 35000);
#endif
    /* End 128 UE: Memory Changes */
    
    Char8 * adapterConfigFileName = "../ltePdcpAdapterConfig.cfg";
    configData = 
        (LP_ConfigFileFormat)getMemFromPool(sizeof(ConfigFileFormat),PNULL);
    if(configData == PNULL){
        /*SPR fix 2030 Start */
#ifdef UE_SIM_TESTING
        ltePanic("Fail to malloc for adapter config structure");
#else
        LOG_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, 
                sizeof(ConfigFileFormat), 0,0,0,0,0, __func__,
                "ADAPTER" );
        ALARM_MSG(PACKET_RELAY_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
#endif
        /*SPR fix 2030 End */
        return;
    }
    /* create msg pool to be used */
    /*pdcpAdapterMsgPool_g =
            msgPoolCreate(PDCP_ADAPTER_BUFFER_SIZE, PDCP_ADAPTER_MAX_BUFFERS);*/
    ReadConfigFile(adapterConfigFileName,configData->trafficGeneratorIP,&pdcpAdapterMode_g,&pdcpAdapterDelay_g);
    /* Set Config Parameters into config structure */
    setConfig(configData);
    /*SPR 2030 fix Start*/
#ifdef UE_SIM_TESTING 
    printConfig(configData);
    /* SPR Start 2188 */
    initInternetSocket(configData);
    /* SPR End 2188 */
    void * (*func_p)(void *);
#endif
    /*SPR 2030 fix End*/
    //initInternetSocket(configData);
    /* Select a mode, normal or loopback, in which PDCP Adapter will be running 
       based on the config param, adapterMode,  read from config file */
    UInt16 ueIndex = 0 ,lcId = 0;
    for(ueIndex = 0 ; ueIndex <= MAX_UE ; ueIndex++) {
        for(lcId = 1 ; lcId <= MAX_LC ; lcId++) {
            ueContext_g[ueIndex][lcId] = 0;
        }
    }
    switch(configData->adapterMode)
    {
        case NORMAL_MODE:
#ifdef UE_SIM_TESTING
            func_p = &normalMode ;
#endif
            break;
        default:
            /* Set to NORMAL_MODE by default*/
            configData->adapterMode = NORMAL_MODE;
#ifdef UE_SIM_TESTING
            func_p = &normalMode ;
#endif
    }
#ifdef UE_SIM_TESTING
#ifndef UESIM_PDCP_THREAD
    void *arg_p = PNULL;
    retTh = threadCreate(&recvThId_g, THREAD_CREATE_DETACHED,
        THREAD_SCHED_FIFO,0,
        THREAD_EXPLICIT_SCHED,
        THREAD_SCOPE_SYSTEM, func_p, arg_p);
    if (retTh != 0)
    {
      ltePanic(" Unable to initialise the Adapter receiver thread.\n");
      return;
    }
    int coreNum = 1;
    coreNum = pdcpAdaptorCoreNum_g;
    THREAD_TO_BIND_SET_CORE_NO(recvThId_g,coreNum);
#endif /* ifndef UESIM_PDCP_THREAD */

#ifdef TWO_UE_WORK
    retTh = threadCreate(&zppp2_thId_g, THREAD_CREATE_DETACHED,
        THREAD_SCHED_FIFO,0,
        THREAD_EXPLICIT_SCHED,
        THREAD_SCOPE_SYSTEM, &zppp2_handling_thread, arg_p);
    if (retTh != 0)
    {
      ltePanic(" Unable to initialise the zppp receiver thread.\n");
      return;
    }
    LTE_LOG(LOG_INFO,PNULL,"\n\n zppp2 thread bind at core number 0\n\n\n");
    THREAD_TO_BIND_SET_CORE_NO(zppp2_thId_g,0);

#endif

#endif

    adapterInitialized_g = 1;

}

/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

/****************************************************************************
 * Function Name  : normalMode
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This is a receiver thread function when working in normal 
 *                  mode.
 ****************************************************************************/
void * normalMode(void*arg)
{
    /* To recv data from socklet */
    SInt32 bytesRead = -1;
    UInt8 recvBuffer[BUFFERSIZE];
    UInt8 *recvData_p = PNULL; 
    UInt32 transactionId = 0;
    LP_Header tlvMsg = PNULL; 
    UInt16 ueIndex = 0;
    UInt8 lcId = 0;
    UInt16 size = 0;
    /*As per SPR 713, change start*/
    UInt8 count = 0;
    /*As per SPR 713, change start*/

#ifndef UESIM_PDCP_THREAD
    threadSetRtPriority(SCHED_FIFO, 45);
    /* Rotate in while loop for receiving messages on socket */
    while(1)
#else 
    SInt32 packetInSingleCycle = 50;
    uSleep(500);
    while( packetInSingleCycle-- )
#endif
    {
        count = 0;
        /* SPR 6949 Fix Start */
        bytesRead = recvfrom_wrapper(sockRx_g, recvBuffer, BUFFERSIZE, 0,PNULL,0);
        /* SPR 6949 Fix End */
        if (bytesRead > 0)
        {
            recvData_p = recvBuffer;
            tlvMsg = (LP_Header)recvData_p;
            ueIndex = LTE_GET_U16BIT(&recvBuffer[count]);
            count +=2;
            lcId = recvBuffer[count++];
            size = LTE_GET_U16BIT(&recvBuffer[count]);

            /* Following check is inserted to avoid UE SIM crash issue that comes 
             * when more than allowed data is pumped on UL*/
            if(MSG_POOL_HIGH_WATERMARK < qvMsgPoolGetUsagePercentage() ||
                    MEM_POOL_HIGH_WATERMARK < qvMemPoolGetUsagePercentage())
            {
                continue;
            }

            /* Convert data received to ZCB and send it to PDCP Layer */
            /* SPR 2624 changes start */
            UInt8* data_p = (UInt8 *)msgAlloc(PNULL, size+PDCP_HEADER_SPACE,
                    PDCP_TRAILER_SPACE, 0);
            /* SPR 2624 changes end*/

            if ( PNULL == data_p ) 
            {                
                /*SPR fix 2030 Start */
#ifdef UE_SIM_TESTING
                ltePanic("msgAlloc failed in %s",__FUNCTION__);
#else
                LOG_MSG(PR_DL_DATA_MSG_ALLOC_FAIL, LOGFATAL, PR_GTPU_DL_DATA,
                        relayGtpuCurrentTick_g, __LINE__, 
                        size, 0,0,0,0,0, __func__,
                        " ADAPTER" );
#endif
                /*SPR fix 2030 End */
            }
            if(PNULL == msgInsert(data_p, 0, tlvMsg->data, size))
            {
                /*SPR fix 2030 Start */
#ifdef UE_SIM_TESTING
                ltePanic("msgInsert failed in %s",__FUNCTION__);
#else
                LOG_MSG(PR_DL_DATA_MSG_INSERT_FAIL, LOGFATAL, PR_GTPU_DL_DATA,
                        relayGtpuCurrentTick_g, __LINE__, 
                        size, 0,0,0,0,0, __func__,
                        " ADAPTER" );
#endif
                /*SPR fix 2030 End */
            }
            pdcpDataReq(ueIndex, lcId, transactionId++, data_p, size,0);
        } 
        else
        {  
#ifdef UESIM_PDCP_THREAD
            break;
#else
            continue;
#endif
        }
    }
    return PNULL;
}

#ifndef UE_SIM_TESTING
/******************************************************************************
 *
 *   FUNCTION NAME : addTunnelIdentier
 *   INPUT         : tunnelId, ueId, lcId
 *   OUTPUT        : Nothing
 *   RETURNS       : void
 *   DESCRIPTION   : This function Stores the Tunnel Id, Ue Id and Lc Id 
 *                   In a Table
 *
 * ***************************************************************************/

  void addTunnelIdentier(UInt32 tunnelId,UInt16 ueId,UInt8 lcId)
  {
      if(ueId >= ADAPTER_MAX_UE)
      {
          /*SPR 2030 fix Start*/
          LOG_MSG(PR_ADAPTER_INV_UEID_RECVD,LOGWARNING,PR_GTPU_DL_DATA,
                  relayGtpuCurrentTick_g,__LINE__,
                  ueId,0,0,0,0,0,__func__,"");
          /*SPR 2030 fix End*/
          return;
      }
      if(adapterDb_g[ueId].flag)
      {
          /*SPR 2030 fix Start*/
          LOG_MSG(PR_ADAPTER_TUNNEL_ID_ALREADY_RCVD, LOGWARNING, PR_GTPU_DL_DATA,
                  relayGtpuCurrentTick_g,__LINE__,
                  tunnelId,0,0,0,0,0,__func__,"");
          /*SPR 2030 fix End*/
          return;
      }
      if(lastUeInSystem_g < ueId)
      {
          lastUeInSystem_g = ueId;
      }

      /* Inserting the values into the Table */
      /*SPR 2030 fix Start*/
      LOG_MSG(PR_ADAPTER_INSERT_TEID_UEID_LCID_IN_DB, LOGINFO, PR_GTPU_DL_DATA,
              relayGtpuCurrentTick_g,__LINE__,
              tunnelId,ueId,lcId,0,0,0,__func__,"");
      /*SPR 2030 fix End*/
      adapterDb_g[ueId].ueId = ueId;
      adapterDb_g[ueId].lcId = lcId;
      adapterDb_g[ueId].tId = tunnelId;

      /* Setting the flag on */
      adapterDb_g[ueId].flag = 1;

  }

#endif

#ifndef UE_SIM_TESTING
#ifndef LAYER2_DLTG_THREAD

UInt8 * dlTempMsgPtr = PNULL;

void normalModeForMacExecSchedular( void )
{
    static UInt8 msgPoolOverFlow = 1 ;
    static UInt8 memPoolOverFlow = 1;
/* SPR 10724 */
  static UInt8 qmsgPoolOverFlow = 1;
  static UInt8 qpctlPoolOverFlow = 1;
/* SPR 10724 */
#ifdef PDCP_ASYNC_INTERFACE
    static UInt8 secQOverflow = 0;
#endif
    extern UInt8 higherMimoConfigurationFlag; 
    unsigned long  packetInSingleCycle = 0;
    TimeSpec tp1,tp2;
    long long nsec;
    /* To recv data from socklet */
    SInt32 bytesRead = -1;
    UInt8 * recvBuffer = PNULL;
    /* Code change - eGTPU integration */
    UInt8 *recvData_p = recvBuffer;
#ifndef L2_FAST_PATH
    UInt32 transactionId = 0;
#endif
    SInt32          selectReturn;
    fd_Set          fdSock;
    UInt32          highFd = 0;

    timeVal    wTime;
    UInt32 msgPoolUsage = 0;
    UInt32 memPoolUsage = 0;
/* SPR 10724 */
     UInt32 qmsgPoolUsage = 0;
     UInt32 qpctlPoolUsage = 0;
/* SPR 10724 */
#ifndef GTPU_UT_FRWK
     msgPoolUsage = qvMsgPoolGetUsagePercentage();
     if ((MSG_POOL_HIGH_WATERMARK < msgPoolUsage) && (msgPoolOverFlow == 0))
     {
         msgPoolOverFlow = 1;
         LOG_UT(PR_MSG_POOL_HIGH_WATERMARK, LOGDEBUG,PR_GTPU_DL_DATA,
                  relayGtpuCurrentTick_g ,msgPoolUsage,0,0,0,0,
                  0,0,__func__,"Stopping traffic");
     }
     else if ((MSG_POOL_LOW_WATERMARK > msgPoolUsage) && (msgPoolOverFlow == 1))
     {
         msgPoolOverFlow = 0;
         LOG_UT(PR_MSG_POOL_LOW_WATERMARK, LOGDEBUG, PR_GTPU_DL_DATA,
                  relayGtpuCurrentTick_g ,msgPoolUsage,0,0,0,0,
                  0,0,__func__,"Restarting traffic");

     }
     memPoolUsage = qvMemPoolGetUsagePercentage();
     if ((MEM_POOL_HIGH_WATERMARK < memPoolUsage) && (memPoolOverFlow == 0))
     {
         memPoolOverFlow = 1;
         LOG_UT(PR_MEM_POOL_HIGH_WATERMARK, LOGDEBUG, PR_GTPU_DL_DATA,
                  relayGtpuCurrentTick_g ,memPoolUsage,0,0,0,0,
                  0,0,__func__,"Stopping traffic");

     }
     else if ((MEM_POOL_LOW_WATERMARK > memPoolUsage) && (memPoolOverFlow == 1))
     {
         memPoolOverFlow = 0;
         LOG_UT(PR_MEM_POOL_LOW_WATERMARK, LOGDEBUG, PR_GTPU_DL_DATA,
                  relayGtpuCurrentTick_g ,memPoolUsage,0,0,0,0,
                  0,0,__func__,"Restrating traffic");

     }
/* SPR 10724 */
     qmsgPoolUsage = qmsgGetUsagePercentage();
     if ((MSG_POOL_HIGH_WATERMARK < qmsgPoolUsage) && (qmsgPoolOverFlow == 0))
     {
         qmsgPoolOverFlow = 1;
     }
     else  if ((MSG_POOL_LOW_WATERMARK > qmsgPoolUsage) && (qmsgPoolOverFlow == 1))
     {
         qmsgPoolOverFlow = 0;
     }

     qpctlPoolUsage = qpctlGetUsagePercentage();
     if ((MSG_POOL_HIGH_WATERMARK < qpctlPoolUsage) && (qpctlPoolOverFlow == 0))
     {
         qpctlPoolOverFlow = 1;
     }
     else  if ((MSG_POOL_LOW_WATERMARK > qpctlPoolUsage) && (qpctlPoolOverFlow == 1))
     {
         qpctlPoolOverFlow = 0;
     }
/* SPR 10724 */
#endif
#ifdef PDCP_ASYNC_INTERFACE
    UInt64 sQCount = QCOUNT_PDCP_SEC_INPUT_TX_Q();
    if ( MAX_PACKETS_TX_SEC_Q < sQCount )
    {
        LOG_MSG(PR_SEC_Q_FULL, LOGWARNING, PR_GTPU_DL_DATA, 
                relayGtpuCurrentTick_g, __LINE__, sQCount, 0,0,0, 0,0,
                __func__,"Stopping traffic");
        //fprintf(stderr, "Number of packets in SEC TX queue = %d\n", sQCount);
        secQOverflow = 1;
    }
    else
    {
        secQOverflow = 0;
    }
#endif
    /*SPR 2791 Fix Start*/
    while(1)
    {
/*SPR 21653 changes start*/
        /*SPR 2791 Fix End*/
        /* FD for listening the Tid Info */
        lteFD_ZERO( &fdSock );
        /*SPR 2412 changes start*/
        if (0 < sockRx_g)
        {
            lteFD_SET(sockRx_g, &fdSock );
            highFd = sockRx_g;
        }
        if (0 < sockRx6_g)
        {
            lteFD_SET(sockRx6_g, &fdSock);
            if (sockRx6_g > highFd) highFd = sockRx6_g;
        }
/*SPR 21653 changes end*/
        lteFD_SET(pdcpSocketRx,&fdSock);if (pdcpSocketRx > highFd) highFd = pdcpSocketRx;
        lteFD_SET(relayConfigInfo_g.s1ApTxFd,&fdSock);
        if (relayConfigInfo_g.s1ApTxFd > highFd) highFd = relayConfigInfo_g.s1ApTxFd;
        /* SPR 2241 changes end */
        /* ECN changes start*/
        if (fdForReceivingPdcpMsgFromRRM_g > 0)
        {
            lteFD_SET(fdForReceivingPdcpMsgFromRRM_g,&fdSock);
            if(fdForReceivingPdcpMsgFromRRM_g > highFd) 
            {
                highFd = fdForReceivingPdcpMsgFromRRM_g;
            }
        } 
        /* ECN changes end*/
        clock_gettime_real_time( &tp1);

        /*SPR 2791 Fix Start*/
        /* tempFdSet = fdSock;*/
        /*SPR 2791 Fix End*/
        /* Time Out value 1 micro second */
        wTime.tv_sec  = 0;
        wTime.tv_usec = 200;

        while( (selectReturn = select_wrapper( highFd + 1, &fdSock, 0, 0, &wTime )) < 0)
        {
            if (errno == EINTR)
                continue;
            /*SPR 2030 fix Start*/
            LOG_MSG(PR_MU_ADAPTER_GLUE_SELECT_FAILED,LOGWARNING, PR_GTPU_DL_DATA,
                    relayGtpuCurrentTick_g,__LINE__,0,0,0,0,
                    0,0,__func__,"");

            /*SPR 2030 fix End*/
            pError("MU_Adapter Glue Select Failed!");
            return;
        }
        if(!selectReturn)
        {
            return;
        }
/*SPR 21653 changes start*/
        if (dlTempMsgPtr != PNULL)
        {
            freeMemPool(dlTempMsgPtr);
            dlTempMsgPtr = PNULL;
        }
        recvData_p = (UInt8 *)getMemFromPool(BUFFERSIZE,PNULL);
        if(PNULL == recvData_p)
        {
            ltePanic("%s: can't allocate buffer",__FUNCTION__);
            /* FDD_Warning_Fix */
            //return 0;
            /* FDD_Warning_Fix */
        }
        recvBuffer = recvData_p;
        dlTempMsgPtr = recvData_p;
/*SPR 21653 changes end*/
        /* SPR 2241 changes start */
        if (lteFD_ISSET(pdcpSocketRx,&fdSock))
        {
            /* SPR 1539 changes start */
            //LTE_LOG(LOG_INFO,PNULL,"Received message for PDCP Layer\n");
            /* SPR 1539 changes end */
            pdcpElTxProcessSocket();
            lteFD_CLR(pdcpSocketRx,&fdSock);
        }
        if (lteFD_ISSET(relayConfigInfo_g.s1ApTxFd,&fdSock))
        {
            /* SPR 1539 changes start */
            //LTE_LOG(LOG_INFO,PNULL,"[RECEIVED] message for eGTPU from OAM/S1AP\n");
            /* SPR 1539 changes start */
            lteRelaySockHandler(RELAY_S1AP_MODULE_ID);
            lteFD_CLR(relayConfigInfo_g.s1ApTxFd,&fdSock);
        }
        /* SPR 2241 changes end */
        /* ECN Changes start */
        if (lteFD_ISSET(fdForReceivingPdcpMsgFromRRM_g,&fdSock))
        {  
            /* Function call for receiving message from RRM, 
             * parse the msgId and call the corresponding API to deal with the message.*/
            processPdcpRRMMsg();
            lteFD_CLR(fdForReceivingPdcpMsgFromRRM_g,&fdSock);
        }
        /* ECN Changes End */

        UInt16 size = 0;
        /*IPV6_CHANGE - START*/
/*SPR 21653 changes start*/
        SInt8 sockFd = 0;
/*SPR 21653 changes end*/
        sockAddrType = IPV4_ADDR_PRESENT;
        /*IPV6_CHANGE - END*/
/*SPR 21653 changes start*/
            /*IPV6_CHANGE - START*/
            if( lteFD_ISSET(sockRx_g, &fdSock) ) {
                sockFd = sockRx_g;
                sockAddrType = IPV4_ADDR_PRESENT;
            }
            else if (lteFD_ISSET(sockRx6_g, &fdSock) ) {
                sockFd = sockRx6_g;
                sockAddrType = IPV6_ADDR_PRESENT;
            }
        if (0 != sockFd)
        /*IPV6_CHANGE - END*/
/*SPR 21653 changes end*/
        {
            /* Code change - eGTPU integration */

            RDTSC_AND_CLOCK_START_SOCKET;
/*SPR 21653 changes start*/
            /* SPR 2058 changes start */
            /*SPR 3587 Fix Start*/
            /* SPR 4357 Fix Start */
            /* SPR 6949 Fix Start */
            bytesRead = recvfrom_wrapper(sockFd, &recvData_p[START_OFFSET_DL_GTP], BUFFERSIZE - START_OFFSET_DL_GTP,0,PNULL,0);
            /* SPR 6949 Fix End */
            /* SPR 4357 Fix End */
            /*SPR 3587 Fix End*/
            /* SPR 2058 changes end */

/*SPR 21653 changes end*/

            /* CA Stats Changes Start */
            RDTSC_AND_CLOCK_END_SOCKET(0);
            /* CA Stats Changes End */

            if ( -1 == bytesRead )
            {
                if ( errno != EAGAIN )
                {
                    /*SPR 2030 fix Start*/
                    LOG_MSG(L2_SOCKET_RECVFROM_FAIL_ID,LOGWARNING, L2_SYS_FAIL,
                            relayGtpuCurrentTick_g,__LINE__,0,0,0,0,
                            0,0,__func__,"");
                    /*SPR 2030 fix End*/
                    ltePanic ("Error in receive message.\n");
                }
                else
                    return;
            }

   /* Memory usage has hit high water mark, drop the packet */
/* SPR 10724 */
     if ((1 == msgPoolOverFlow) || (1 == memPoolOverFlow) || (1 == qmsgPoolOverFlow) || (1 == qpctlPoolOverFlow)
/* SPR 10724 */
#ifdef PDCP_ASYNC_INTERFACE
                    || (1 == secQOverflow )
#endif
               )
            {
                /* SPR 4029 Fix Start */
                if (recvData_p != NULL)
                {
                    freeMemPool(recvData_p);
                    /* SPR 6262 fix start */ 
                    dlTempMsgPtr = NULL;
                    /* SPR 6262 fix end */ 
                }
                /* SPR 4029 Fix End */
                return;
            }

            if (bytesRead > 0)
            {  
                /* Code change - eGTPU integration */
#ifdef L2_FAST_PATH /* Undefine this */
                /*Review comment incorporated*/
                lteUserPlaneFastPath(recvData_p, bytesRead + START_OFFSET_DL_GTP);
                /* In Fast Path it is not used, setting it as NULL */
                /* In Slow Path via Fast Path, It should be NULL after its Usage */
                dlTempMsgPtr = NULL;
#else
                /*Review comments incorporated*/
                processPeerGtpuDataInd(recvData_p, 
                        transactionId++, 
                        bytesRead+START_OFFSET_DL_GTP);
#endif
                if (dlTempMsgPtr != PNULL)
                {
                    freeMemPool(dlTempMsgPtr);
                    dlTempMsgPtr = PNULL;
                }
                pdcpUserNumOfPackets_g++;
                pdcpUserTxThroughput_g += size;
            } 
            else
            {   
                /*SPR fix 2030 Start */
                LOG_MSG(PR_ADAPTER_LESS_BYTES_RCVD, LOGFATAL, PR_GTPU_DL_DATA,
                        relayGtpuCurrentTick_g, __LINE__, 
                        bytesRead, 0,0,0,0,0, __func__,
                        "" );
                /*SPR fix 2030 End */
            }
            /* If sufficient packets are received in 1 ms then break */
            packetInSingleCycle += bytesRead;
            if (higherMimoConfigurationFlag)
            {
                if ( packetInSingleCycle >= 2*18500 )
                    return;
            }
            else
            {
                if ( packetInSingleCycle >= 18500 )
                    return;
            }    
        }
        clock_gettime_real_time( &tp2);
        nsec = tp2.tv_sec * 1000000000ULL + tp2.tv_nsec - tp1.tv_sec*1000000000ULL - tp1.tv_nsec;
        if (nsec > 200000)
            return;
    } /* End of While */
    return;
}
#endif
#endif

/****************************************************************************
 * Function Name  : initSocketAddress
 * Inputs         : sockaddr_in, socketFileDescriptor,Port Number
 * Outputs        :
 * Returns        : status (0-Failure,1-Successful)
 * Variables      :
 * Description    : To intialize the Socket address with specific socket file
 *                  descriptor and specific port
 ****************************************************************************/
static UInt8 initSocketAddress(SockAddr *s,SInt8 *sockfd,char *ipAddr, UInt16 port)
{
    /*As per SPR 713, change end*/
    if((*sockfd = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("creating socket error\n");
        return 0;
    }
    (*s).sin_family = AF_INET;
    (*s).sin_port = htons(port);
    if (Inet_aton_wrapper((ipAddr),&((*s).sin_addr)) == 0)
    {
        ltePanic ("Error while converting the address.");
    }
    return 1;
}

/*IPV6_CHANGE - START*/
/****************************************************************************
 * Function Name  : initSocketAddress6
 * Inputs         : sockaddr_in, socketFileDescriptor,Port Number
 * Outputs        :
 * Returns        : status (0-Failure,1-Successful)
 * Variables      :
 * Description    : To intialize the Socket address with specific socket file
 *                  descriptor and specific port for IPv6 socket
 ****************************************************************************/
static UInt8 initSocketAddress6(struct sockaddr_in6 *s,SInt8 *sockfd,UInt8 *ipAddr, UInt16 port)
{
    if((*sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("creating socket error\n");
        return 0;
    }
    (*s).sin6_family = AF_INET6;
    (*s).sin6_port = htons(port);
    memcpy((*s).sin6_addr.s6_addr, ipAddr, IPV6_ADDR_LEN);
    (*s).sin6_flowinfo = 0;
    (*s).sin6_scope_id = 0;

    return 1;
}

/*IPV6_CHANGE - END*/
/****************************************************************************
 * Function Name  : setConfig
 * Inputs         : configData - Initialization Configuration Data 
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : To set the default config values in config structre 
 ****************************************************************************/
static void  setConfig(LP_ConfigFileFormat configData)
{
    configData->trafficGeneratorPort= trafficGeneratorPort;
    strcpy(configData->trafficGeneratorIP,trafficGeneratorIP);
    strcpy(configData->adapterIP,adapterIP);
    /*IPV6_CHANGE - START*/
    configData->addrPresenceSt = IPV4_ADDR_PRESENT;
    /*IPV6_CHANGE - END*/
    configData->adapterMode= pdcpAdapterMode_g;
    configData->adapterDelay= pdcpAdapterDelay_g;
    configData->adapterRxPort = pdcpAdapterRxPort;
}

/****************************************************************************
 * Function Name  : printConfig
 * Inputs         : configData - Initialization Configuration Data
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : To print the initialization config values 
 ****************************************************************************/
#ifdef UE_SIM_TESTING
static void  printConfig(LP_ConfigFileFormat configData)
{
    LTE_LOG( LOG_INFO,PNULL,"TG Port = %d\n",configData->trafficGeneratorPort);
    LTE_LOG( LOG_INFO,PNULL,"TG IP = %s\n",configData->trafficGeneratorIP);
    LTE_LOG( LOG_INFO,PNULL,"Mode = %d\n",configData->adapterMode);
    LTE_LOG( LOG_INFO,PNULL,"Delay =%d\n", configData->adapterDelay);
    LTE_LOG( LOG_INFO,PNULL,"Adpt Port =%d\n", configData->adapterRxPort);
    //    LTE_LOG( LOG_INFO,PNULL,"Adpt Port =%d\n", configData->adapterTxPort);
}
#endif

/* Intefaces */
/****************************************************************************
 * Function Name  : pdcpDataInd
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  data_p  - pointer to PDCP SDU data in Zero-copy
 *                            buffering format.
 *                  size    - size of data.
 * Outputs        : None.
 * Returns        : None.
 * Description    : The PDCP layer application has to implement this function.
 *                  It's used for passing SDU form the PDCP layer to the PDCP
 *                  user (upper layers).
 ****************************************************************************/
void  pdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, UInt16 size)
{
    UInt8 *msgPtr = PNULL;
    UInt16 msgLen = 0;
#if defined UE_SIM_TESTING
    UInt8 indx = 0;
#endif

    if(!data_p) {

        /*SPR fix 2030 Start */
#ifndef UE_SIM_TESTING
        LOG_MSG(PR_ADAPTER_EMPTY_UL_DATA, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                ueIndex, lcId,0,0,0,0, __func__,
                "" );
#endif
        /*SPR fix 2030 End */
    }    

    /* PDCP_32.425 code changes start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 20831 Changes Start */
    UInt32 activeIndexForPdcpThpStats = (gMacStats.
            pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats);
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats = &(gMacStats.pStats->lteCommonUeKPIStats.
            ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);
    /* SPR 20831 Changes End */
    pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueIndex][lcId - 3] += size;
#endif
    /* PDCP_32.425 code changes end */

    pdcpUserRxThroughput_g += size ;
    /* We receive SDU from the PDCP layer, so pass it to upper layer in some way */
    if(BUFFERSIZE < size )
    {
        msgFree(data_p);
        data_p = PNULL;
        /*SPR fix 2030 Start */
#ifndef UE_SIM_TESTING
        LOG_MSG(PR_ADAPTER_UL_DATA_GREATER_SIZE, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                BUFFERSIZE, size,0,0,0,0, __func__,
                "" );
#endif
        /*SPR fix 2030 End */
        return ;
    }
#ifdef L2_FAST_PATH
#ifndef UE_SIM_TESTING
    /*
     * Get memory for GTP Header + Payload received from PDCP
     */
    msgLen = size+GTP_MIN_HDR_LEN;
    msgPtr = msgReserve(data_p, 0, GTP_MIN_HDR_LEN);
    if(PNULL == msgPtr)
    {
        ltePanic("%s: can't allocate buffer",__FUNCTION__);
        return;
    }
#endif
#endif
#ifndef UE_SIM_TESTING
#ifndef L2_FAST_PATH
    msgLen = size+64;
    msgPtr = (UInt8 *)getMemFromPool(msgLen, PNULL);
    if(PNULL == msgPtr)
    {
        /*SPR fix 2030 Start */
        LOG_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, 
                msgLen, 0,0,0,0,0, __func__,
                "ADAPTER" );
        /*SPR fix 2030 End */
        return;
    }
    msgExtract(data_p, 0, msgPtr+64, size);
#endif /*L2_FAST_PATH */
#else
    /* SPR #2017 fix Start */
    msgLen = size+5;
    /* SPR #2017 fix Start */
    msgPtr = (UInt8 *)getMemFromPool(msgLen, PNULL);
    if(PNULL == msgPtr)

    {
        /*SPR fix 2030 Start */
#ifdef UE_SIM_TESTING
        ltePanic("%s: can't allocate buffer",__FUNCTION__);
#else

        LOG_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, 
                msgLen, 0,0,0,0,0, __func__,
                "ADAPTER" );
#endif
        /*SPR fix 2030 End */
        ltePanic("%s: can't allocate buffer",__FUNCTION__);
        return;
    }
    /* SPR #2017 fix Start */
    msgExtract(data_p, 0, msgPtr+5, size);
    /* SPR #2017 fix Start */
#endif
#ifndef L2_FAST_PATH
    /* not freeing memory here as ZCB used */
    msgFree(data_p);
    data_p = PNULL;
#endif

#ifdef L2_FAST_PATH
#ifndef UE_SIM_TESTING
    egtpu_rab_ctxt_t    *p_rab_ctxt = NULL;
    /* SPR_3736 start */
    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(
            p_global, relayMapBasedOnUeIndexLcId_g[ueIndex][lcId-3].tunnelId);
    if(NULL != p_rab_ctxt)
    {
        /*
         *  * Populate the GTPU Header of 8 Bytes
         *   */
        UInt8 gtp_hdr_indx = 0;

        /* Octet 1
         ** Version - 1
         ** Protocol Type - 1
         ** All other fields are unset
         **/
        msgPtr[gtp_hdr_indx++]= 0x30;
        /*
         ** Octet 2
         **
         ** Message Type = T-PDU = 255
         **/
        msgPtr[gtp_hdr_indx++]= 0xff;

        /*
         ** Octet 3 & 4
         **
         ** Size of Payload
         **/
        msgPtr[gtp_hdr_indx++]= (size & 0xFF00)>>8;
        msgPtr[gtp_hdr_indx++]= (size & 0x00FF)>>0;


        /*
         ** Octet 5, 6, 7 & 8
         **
         ** Tunnel ID
         **/
        LTE_SET_U32BIT(&msgPtr[gtp_hdr_indx],p_rab_ctxt->teid_peer);
    }
    else
    {
        LTE_LOG(LOG_INFO,"\n[ERROR] NULL rab ctxt - error in Relay Map\n");
        msgFree(data_p);
        data_p = NULL;
        return;
    }
    /*SPR_3736 change end */
#endif
#else
    //
#endif

#ifndef UE_SIM_TESTING
#ifndef L2_FAST_PATH
    LTE_SET_U16BIT(&msgPtr[59],ueIndex);
    msgPtr[61] = lcId;
    LTE_SET_U16BIT(&msgPtr[62], (msgLen-64));
#endif
#else
    indx = 0;
    /* GTP_HEADER of 8 bytes*/
    LTE_SET_U16BIT(&msgPtr[indx],ueIndex);
    indx +=2;
    msgPtr[indx] = lcId;    
    indx += 1;
    LTE_SET_U16BIT(&msgPtr[indx],size);
    indx +=2;    
#endif

#ifdef L2_FAST_PATH
#ifndef UE_SIM_TESTING
    SInt8               socketfd = 0; 
    /*IPV6_CHANGE - START*/
    UInt8  addr_len;
    struct sockaddr_in6 sockaddr6;
    void *pSockAddr = NULL;
    UInt16 sockAddrLen = 0;
    /*IPV6_CHANGE - END*/
    /*
     * Fetch the RAB Context on the Basis of the Tunnel ID
     */
    /*SPR_3736 change start*/
    /*    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(
          p_global, relayMapBasedOnUeIndexLcId_g[ueIndex][lcId-3].tunnelId);

          if ((NULL != p_rab_ctxt) &&
          (NULL != p_rab_ctxt->p_transport_addr))
     */
    if (NULL != p_rab_ctxt->p_transport_addr)
        /*SPR_3736 change end */
    {
      /*IPV6_CHANGE - START*/  
      addr_len = RELAY_GTPU_GET_U16BIT(&(p_rab_ctxt->p_transport_addr->entity_addr[0]));
        if (RELAY_ADDR_LEN_IPV4 == addr_len)
        {
            /*
             * Populate the information for sending the packet to Peer GTPU Entity
             */
            relayConfigInfo_g.gtpuToAddr.sin_family = AF_INET;
            relayConfigInfo_g.gtpuToAddr.sin_port = htons(EGTPU_UDP_PORT);
            relayConfigInfo_g.gtpuToAddr.sin_addr.s_addr 
                = htonl(RELAY_GTPU_GET_U32BIT(&(p_rab_ctxt->p_transport_addr->entity_addr[2])));        
            /* COVERITY 10446 */ 
            if (p_rab_ctxt->qos_id >= MAX_DSCP_VALUES)
                /* COVERITY 10446 */ 
            {
                socketfd = sockRx_g;
            } 
            else
            {
                socketfd = relayDscpToSockFdMap_g[p_rab_ctxt->qos_id].sockfd;
                if (socketfd == RELAY_INVALID_FD)
                {
                    socketfd = sockRx_g;
                }
            }
            pSockAddr = &relayConfigInfo_g.gtpuToAddr;
            sockAddrLen = sizeof(relayConfigInfo_g.gtpuToAddr);
        }
        else
        {
            memset(&sockaddr6,0,sizeof(sockaddr6));
            memcpy(sockaddr6.sin6_addr.s6_addr, &(p_rab_ctxt->p_transport_addr->entity_addr[2]), addr_len);
            sockaddr6.sin6_family = AF_INET6;
            sockaddr6.sin6_port = htons(EGTPU_UDP_PORT);

            if (p_rab_ctxt->qos_id >= MAX_DSCP_VALUES)
            {
                socketfd = sockRx6_g;
            } 
            else
            {
                socketfd = relayDscpToSockFdMap6_g[p_rab_ctxt->qos_id].sockfd;
                if (socketfd == RELAY_INVALID_FD)
                {
                    socketfd = sockRx6_g;
                }
            }
            pSockAddr = &sockaddr6;
            sockAddrLen = sizeof(sockaddr6); 
        }
/*IPV6_CHANGE - END*/
#ifdef LAYER2_DLTG_THREAD
        /*
         * Pass the GTPu Payload to DLTG 
         */
        lteMacprocessULPacketInDLTG (data_p,size + GTP_MIN_HDR_LEN,
                p_rab_ctxt->teid_peer);
#else
        /*
         * Flatten buffer before writing to socket
         */
        UInt8 *flatbuff = PNULL;
        flatbuff = (UInt8 *)getMemFromPool(msgLen, PNULL);
        if(PNULL == flatbuff)
        {
            ltePanic("%s: can't allocate buffer",__FUNCTION__);
            return;
        }
        msgExtract(data_p, 0, flatbuff, msgLen);
        if ( -1 ==
                sendto_wrapper(socketfd,&flatbuff[0],msgLen,
                    0, pSockAddr,sockAddrLen ))
        {
            LTE_LOG(LOG_INFO,"\n[ERROR] Not able to send data to DL-TG\n");
        }

        freeMemPool(flatbuff);
        flatbuff = NULL;

#endif
    }
#else
    /* FREE UL PACKET AT PDCP */
#ifndef DLPKT_DROP_UE_PDCP
    writeToSocket(sockTx_g, &msgPtr[0], msgLen, configData->trafficGeneratorIP,
            configData->trafficGeneratorPort);
#endif
    /* FREE UL PACKET AT PDCP */
#endif
#else

#ifndef UE_SIM_TESTING
    processGtpuAdapterDataReq(msgPtr, msgLen);
#else
    /* FREE UL PACKET AT PDCP */
#ifndef DLPKT_DROP_UE_PDCP
    writeToSocket(sockTx_g, &msgPtr[0], msgLen, configData->trafficGeneratorIP,
            configData->trafficGeneratorPort);
#endif
    /* FREE UL PACKET AT PDCP */
#endif

#endif
#ifdef L2_FAST_PATH
    /* freeing memory here, not freed earlier ZCB used */
    msgFree(data_p);
    data_p = NULL;
#ifdef UE_SIM_TESTING
    freeMemPool(msgPtr);
    msgPtr = NULL;
#endif
#else
    freeMemPool(msgPtr);
    msgPtr = NULL;
#endif
}

/****************************************************************************
 * Function Name  : writeToTrafficGeneratorSocket
 * Inputs         : fd - Socket fd.
 *                  msgPtr - Ptr to message to be sent
 *                  msgLen -  Message Length
 *                  hostname_p - Hostname of machine 
 *                  port - Port on which data needs to be sent
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to write data on socket for 
 *                  traffic generator.
 ****************************************************************************/
#ifdef UE_SIM_TESTING
static void writeToSocket(SInt8 fd, void *msgPtr, UInt16 msgLen, 
                          Char8 *hostname_p, UInt16 port)
{
#ifdef PDCP_ADAPTER_TESTING
    static int packetCount = 1;
#endif
    Int res_len = -1;
    /* write To Socket */
    struct hostent *host_p = PNULL;
    SockAddr toSockAddr;
    host_p= (struct hostent *) gethostbyname_wrapper(hostname_p);
    /*As per SPR 713, change start*/
    if(((struct hostent *)0) == host_p)
    {
       /* SPR 2030 fix Start*/
#ifdef UE_SIM_TESTING
      ltePanic("%s: error getting host by name",__FUNCTION__);
#else
       LOG_MSG(L2_GET_HOSTBYNAME_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                relayGtpuCurrentTick_g,__LINE__,
                0,0 ,
                0,0, 0,0, __func__, "");
#endif
        /* SPR 2030 fix End*/
      return;
    }
    /*As per SPR 713, change end*/
    toSockAddr.sin_family = AF_INET;
    toSockAddr.sin_port = htons(port);
    toSockAddr.sin_addr = *((struct in_addr *)host_p->h_addr);
    //LTE_LOG(LOG_INFO,PNULL,"Packet is successfully sent to GTPU\n");
    RDTSC_AND_CLOCK_START_SOCKET;
    res_len = sendto_wrapper(fd,(UChar8*)msgPtr,msgLen,0,
            &toSockAddr,sizeof(SockAddr));

    /* CA Stats Changes Start */
    RDTSC_AND_CLOCK_END_SOCKET(0);
    /* CA Stats Changes End */
    if(res_len == msgLen)
    {
#ifdef PDCP_ADAPTER_TESTING
        LTE_LOG(LOG_INFO,PNULL,"PDCP Adapater sent packet to gtpu of size %d and packetCount %d\n",msgLen, packetCount++);
#endif
    }
    else
        if (res_len == -1)
        {
            /*SPR 2030 fix Start*/
#ifdef UE_SIM_TESTING
            ltePanic("Error No- %d Desp: %s \n",errno,strerror(errno));
#else
            LOG_MSG(PR_ADAPTER_ERR_IN_SENDING_PACKET_TO_TG, LOGWARNING, PR_GTPU_UL_DATA,
                    relayGtpuCurrentTick_g,__LINE__,0,0,0,0,
                    0,0,__func__,"");
#endif
            /*SPR 2030 fix End*/
        }
}
#endif

/****************************************************************************
 * Function Name  : pdcpDataStatusInd
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  deliveryStatus - SDU delivery status
 * Outputs        : None.
 * Returns        : None.
 * Description    : The PDCP layer application has to implement this function.
 *                  It used for sending SDU delivery indictaion to the PDCP
 *                  user (upper layers) in case of Discard_timer expires for
 *                  SDU or PDU delivery is failed (for RLC AM).
 ****************************************************************************/
void pdcpDataStatusInd( UInt16 ueIndex, UInt16 lcId, UInt16 deliveryStatus )
{
    //PDCP_LOG(LOG_DUMP, "PDCP_STATUS_IND\t: ueIndex[%u] lcId[%u] transactionId[%u], deliveryStatus[%u]",
    //      ueIndex, lcId, transactionId,  deliveryStatus);

    /* We receive SDU delivery indication from the PDCP layer, so pass it to upper layer in some way */
}

void setPumping(UInt8 flag)
{
    pumpFlag = flag; 
}
/* interfaces end*/

#ifdef BUILD_EXE
int main()
{
    initMemPool();
    initMsgPool();
    initPdcpAdapter();
    
    while(1); 

    return 0;
}
#endif 

/****************************************************************************
 * Function Name  : pdcpSourceDlDataRelocInd
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  sn      - Sequence Number
 *                  data_p  - pointer to PDCP SDU data in Zero-copy
 *                            buffering format.
 *                  size    - size of payload
 * Outputs        : None.
 * Returns        : None.
 * Description    : The PDCP layer calls this API to forward Downlink SDUs 
 *                  to upper layer at source eNodeB in case of handover 
 ****************************************************************************/

UInt16 pdcpSourceDlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
    void *data_p, UInt32 size)
{

    UInt8 *msgPtr = PNULL;
    if(!data_p) 
    {
       
#ifndef UE_SIM_TESTING
        LOG_MSG(PR_ADAPTER_EMPTY_UL_DATA, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                ueIndex, lcId,0,0,0,0, __func__,
                "" );
#endif
    }    

    /* PDCP_32.425 code changes start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 20831 Changes Start */
    UInt32 activeIndexForPdcpThpStats = (gMacStats.
            pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats);
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats = &(gMacStats.pStats->lteCommonUeKPIStats.
            ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);
    /* SPR 20831 Changes End */

    /*SPR 21291 Start*/
    if(pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] >= size)
    {
        pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] -= size;
    }
    else
    {
        pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] = 0;
    }
    /*SPR 21291 End*/

#endif
    /* PDCP_32.425 code changes end */

    /* We receive SDU from the PDCP layer, so forward it to upper layer */
    if(BUFFERSIZE < size )
    {
//        msgFree(data_p);
//        data_p = PNULL;
#ifndef UE_SIM_TESTING
        LOG_MSG(PR_ADAPTER_UL_DATA_GREATER_SIZE, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                BUFFERSIZE, size,0,0,0,0, __func__,
                "" );
#endif
	    return 0;
    }
    #ifndef UE_SIM_TESTING
         UInt16 msgLen = size+64;
         msgPtr = (UInt8 *)getMemFromPool(msgLen, PNULL);
         if(PNULL == msgPtr)
         {
             LOG_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                     relayGtpuCurrentTick_g, __LINE__, 
                     msgLen, 0,0,0,0,0, __func__,
                     "ADAPTER" );
            return 0;
         }
         msgExtract(data_p, 0, msgPtr+64, size);
    #else
        UInt16 msgLen = size+GTP_MIN_HDR_LEN;
        msgPtr = (UInt8 *)getMemFromPool(msgLen, PNULL);
        if(PNULL == msgPtr)
            
        {
#ifdef UE_SIM_TESTING
           ltePanic("%s: can't allocate buffer",__FUNCTION__);
#else

             LOG_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                     relayGtpuCurrentTick_g, __LINE__, 
                     msgLen, 0,0,0,0,0, __func__,
                     "ADAPTER" );
#endif
             ltePanic("%s: can't allocate buffer",__FUNCTION__);
             return 0;
        }
        msgExtract(data_p, 0, msgPtr+GTP_MIN_HDR_LEN, size);
    #endif

#ifndef GTPU_UT_FRWK
    //msgFree(data_p);
    // data_p = PNULL;
#endif

#ifndef UE_SIM_TESTING
    LTE_SET_U16BIT(&msgPtr[59],ueIndex);
    msgPtr[61] = lcId;
    LTE_SET_U16BIT(&msgPtr[62], (msgLen-64));
#else
   UInt8 indx = 0;
   /* GTP_HEADER of 8 bytes*/
   msgPtr[indx++]= 0x30;
   msgPtr[indx++]= 0xff;
   msgPtr[indx++]= (size & 0xFF00)>>8;
   msgPtr[indx++]= (size & 0x00FF)>>0;
   msgPtr[indx++]= 0x00;
   msgPtr[indx++]= 0x00;
   LTE_SET_U16BIT(&msgPtr[indx],ueIndex);
#endif

#ifndef UE_SIM_TESTING
    /*Forward DL PDCP SDUs to packet Relay*/
    processGtpuAdapterDlDataRelocReq(msgPtr,sn);
#else
    writeToSocket(sockTx_g, &msgPtr[0], msgLen, configData->trafficGeneratorIP,
               configData->trafficGeneratorPort);
#endif

    freeMemPool(msgPtr);
    msgPtr = NULL;
    return 1;
}


/****************************************************************************
 * Function Name  : pdcpSourceUlDataRelocInd
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  sn      - Sequence Number
 *                  data_p  - pointer to PDCP SDU data in Zero-copy
 *                            buffering format.
 *                  size    - size of payload
 * Outputs        : None.
 * Returns        : None.
 * Description    : The PDCP layer calls this API to forward uplink SDUs 
 *                  to upper layer at source eNodeB in case of handover 
 ****************************************************************************/

UInt16 pdcpSourceUlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
    void *data_p, UInt32 size
#ifdef KPI_STATS
    ,InternalCellIndex internalCellIndex
#endif    
)
{

    UInt8 *msgPtr = PNULL;
    if(!data_p) 
    {
       
#ifndef UE_SIM_TESTING
        LOG_MSG(PR_ADAPTER_EMPTY_UL_DATA, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                ueIndex, lcId,0,0,0,0, __func__,
                "" );
#endif
    }    
    /* We receive SDU from the PDCP layer, so forward it to upper layer */
    if(BUFFERSIZE < size )
    {
        // msgFree(data_p);
        // data_p = PNULL;
#ifndef UE_SIM_TESTING
        LOG_MSG(PR_ADAPTER_UL_DATA_GREATER_SIZE, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                BUFFERSIZE, size,0,0,0,0, __func__,
                "" );
#endif
	    return 0;
    }
    #ifndef UE_SIM_TESTING
         UInt16 msgLen = size+64;
         msgPtr = (UInt8 *)getMemFromPool(msgLen, PNULL);
         if(PNULL == msgPtr)
         {
             LOG_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                     relayGtpuCurrentTick_g, __LINE__, 
                     msgLen, 0,0,0,0,0, __func__,
                     "ADAPTER" );
            return 0;
         }
         msgExtract(data_p, 0, msgPtr+64, size);
    #else
        UInt16 msgLen = size+GTP_MIN_HDR_LEN;
        msgPtr = (UInt8 *)getMemFromPool(msgLen, PNULL);
        if(PNULL == msgPtr)
            
        {
#ifdef UE_SIM_TESTING
           ltePanic("%s: can't allocate buffer",__FUNCTION__);
#else

             LOG_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                     relayGtpuCurrentTick_g, __LINE__, 
                     msgLen, 0,0,0,0,0, __func__,
                     "ADAPTER" );
#endif
             ltePanic("%s: can't allocate buffer",__FUNCTION__);
             return 0;
        }
        msgExtract(data_p, 0, msgPtr+GTP_MIN_HDR_LEN, size);
    #endif

#ifndef GTPU_UT_FRWK
    // msgFree(data_p);
    // data_p = PNULL;
#endif

#ifndef UE_SIM_TESTING
    LTE_SET_U16BIT(&msgPtr[59],ueIndex);
    msgPtr[61] = lcId;
    LTE_SET_U16BIT(&msgPtr[62], (msgLen-64));
#else
   UInt8 indx = 0;
   /* GTP_HEADER of 8 bytes*/
   msgPtr[indx++]= 0x30;
   msgPtr[indx++]= 0xff;
   msgPtr[indx++]= (size & 0xFF00)>>8;
   msgPtr[indx++]= (size & 0x00FF)>>0;
   msgPtr[indx++]= 0x00;
   msgPtr[indx++]= 0x00;
   LTE_SET_U16BIT(&msgPtr[indx],ueIndex);
#endif

#ifndef UE_SIM_TESTING
    /*Forward DL PDCP SDUs to packet Relay*/
    processGtpuAdapterUlDataRelocReq(msgPtr,sn);
#else
    writeToSocket(sockTx_g, &msgPtr[0], msgLen, configData->trafficGeneratorIP,
               configData->trafficGeneratorPort);
#endif

    freeMemPool(msgPtr);
    msgPtr = NULL;
    return 1;
}

/****************************************************************************
 * Function Name  : pdcpEndOfDataForwardingInd
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  isEndMarkerIndRecvd - Flag for EMI Received
 * Outputs        : None.
 * Returns        : None.
 * Description    : The PDCP layer indicates end of forwarding SDUs in HO
 *                  Scenario 
 ****************************************************************************/
/* SPR 3085 Fix Start*/
void pdcpEndOfDataForwardingInd(UInt16 ueIndex, UInt8 lcId,UInt8 isEndMarkerIndRecvd)
/* SPR 3085 Fix End*/
{
#ifndef UE_SIM_TESTING
    if((ueIndex < MAX_UE) && (lcId <= MAX_DRB_LC) && (lcId >= MIN_DRB_LC))
        /* SPR 3085 Fix Start */
        processAdapterEndOfDataFwdInd(ueIndex,lcId,isEndMarkerIndRecvd);
       /* SPR 3085 Fix End */
    else
        LOG_MSG(PR_ADAPTER_INV_UEID_LCID, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                ueIndex, lcId,0,0,0,0, __func__,
                "" );
#endif
}

