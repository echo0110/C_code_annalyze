/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: gtpuMain.c  $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the main function for GTP process. 
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/
#if  defined(PDCP_GTPU_INTF)
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteThread.h"
#include "lteComOAMIntegration.h"
#include "lteRelayAdapter.h"
#include "lteRelayCommon.h"
#include "lteRelayOamSap.h"
#include "lteLayer2CommanTypes.h"
/*SPR 21653 changes start*/
#include "lteMisc.h"
/*SPR 21653 changes end*/
/* SPR 21804 Fix Start */
#if defined(LAYER2_DLTG_THREAD) && defined(PDCP_GTPU_INTF)
void* dlTgThreadExecutionSchedular(void * param);
ULong32  dlTgExecutionSchedulerThId_g;
extern void normalModeForMacExecSchedular(void);
#include "../../../../mac/maccore/src/executionScheduler/lteMacExecutionDLTG.c"
#endif
/* SPR 21804 Fix End */

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void handlePdcpUeDataInd( UInt8 *msg_p, SInt32 bytesread );
static void handlePdcpSrcDlRelocReq( UInt8 *msg_p, SInt32 bytesread );
static void handlePdcpSrcUlRelocReq( UInt8 *msg_p, SInt32 bytesread );
static void handleLocalEmi( UInt8 *msg_p, SInt32 bytesread );
/*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
static void handleGtpuRegistration( UInt8 *msg_p, SockAddr *sockAddr);
#endif
/*SPR 21366 -*/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/
#define SUCCESS                 1
#define FAILURE                 0
/* SPR 19110 Fix Start */
/* GTPU will send init indication after 1 second */
#define GTPU_INIT_IND_TIMER     1

/* Wait for 800 us so that PDCP can process the packet
 * after aound 1 ms
 */
#define GTPU_TIMEOUT            800
/* SPR 19110 Fix End */


/****************************************************************************
 * Exported Variables
 ****************************************************************************/
LteLayer2CoreInfo lteLayer2CoreInfo_g;
/* For compilation purpose */
UInt8 initIndTimer_g = 0;
ULong32 threadId_g[MAX_THREAD_ID];
UInt8  thr_count_g = 0;

/*SPR21708 code Changes Start */
UInt32 lteLayer2CoreNum_g;
/*SPR21708 code Changes end */
extern void initPacketRelay(void);
extern void egtpu_init();
extern void handleUserData(void);
/* SPR 21950 fix start */
/* SPR 21950 fix end */
extern void egtpu_check_echo_timer_expiry(void);

#if defined PDCP_GTPU_INTF
extern UInt8 gtpu_Pdcp_Instance_g[MAX_RRC_CELL_INDEX];
#endif

#ifdef LINUX_PC_TEST
#include "TestFrameApi.h"
short adapter_hdl_msg_callback(char *buf, long len);
short adapter_init_socket(void);
#endif
UInt64 uplinkRelayIngressDataSize   = 0;
UInt64 uplinkRelayEgressDataSize    = 0;
UInt64 uplinkRelayIngressDataNum = 0;
UInt64 uplinkRelayEgressDataNum  = 0;

/***************************************************************
 * Function Name  : stopOtherThread()
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : This function is responsible to stop other thread which is
 *                  running
 ****************************************************************************/
void stopOtherThread ()
{
    stopAllThread(MAX_THREAD_ID,threadId_g);
}

/****************************************************************************
 * Function Name  : getCurrentSysTime
 * Inputs         : time variable
 * Outputs        : current Time in sec/ms
 *                  Absolute time in ms
 * Returns        : None
 * Description    : This function sets the current system time
 ****************************************************************************/
void getCurrentSysTime(QTIME *now, tickType_t *timeMs_p)
{
    TimeSpec ts = {0};

    /* Get the time in seconds and nanoseconds */
    clock_gettime_real_time( &ts );
    now->s = ts.tv_sec;
    now->us = ts.tv_nsec/1000; /* microseconds */

    /* Calculate the time in milliseconds */
    *timeMs_p = (now->s * 1000) + (now->us / 1000);
}

/****************************************************************************
 * Function Name  : sendFreetoPdcp
 * Inputs         : q - Pointer to the first data.
 *                  buf_p - Pointer to data to be freed.
 * Outputs        : void
 * Returns        : None
 * Description    : This API sends indication to PDCP process to free the
 *                  buffer
 ****************************************************************************/
void sendFreetoPdcp ( void *q, void *buf_p )
{
    freeMemPool( buf_p );
}

/****************************************************************************
 * Function Name  : prPdcpProcessMessage
 * Inputs         : msgBuf_p - Pointer to message
 *                  bytesread - length of message
 * Outputs        : void
 * Returns        : None
 * Description    : This API processed the msg rcvd from PDCP over mailbox.
 ****************************************************************************/
void prPdcpProcessMessage( UInt8 *msgBuf_p, SInt32 bytesread 
#if defined PDCP_GTPU_INTF
        , SockAddr *sockAddr
#endif
        )
{
#ifdef LOG_PRINT_ENABLED
    UInt16 transactionId  = 0;
#endif
    UInt16 msgId          = 0;
    UInt16 msgLen         = 0;
    UInt16 current        = 0;

    /* Parse the Header of recieved buffer */
    /* Transaction ID   -  2 bytes
       Src Module ID    -  2 bytes
       Dest. Module ID  -  2 bytes
       API - Identifier -  2 bytes
       Length           -  2 bytes
       Padding          -  2 bytes
     */
#ifdef LOG_PRINT_ENABLED
    transactionId = LTE_GET_U16BIT(msgBuf_p + current);
#endif
    current += 6;
    msgId = LTE_GET_U16BIT(msgBuf_p + current);
    current += 2;
    msgLen = LTE_GET_U16BIT(msgBuf_p + current);
    current += 4;

    if ( bytesread != msgLen )
    {
        LOG_GTPUPR_WARNING( PR_GTPU_COMMON, "msgLength[%d] not "
                "same as byteRead[%d]transactionId[%d]", msgLen, bytesread,transactionId );

        sendFreetoPdcp( 0, msgBuf_p);
        return;
    }
/*SPR 21653 changes start*/
    msgLen -= INTERFACE_API_HEADER_LEN;
/*SPR 21653 changes end*/

    switch ( msgId )
    {

        case PDCP_PR_UE_DATA_IND:
        {
            handlePdcpUeDataInd( msgBuf_p, msgLen );
            break;
        }
        case PDCP_PR_SOURCE_DL_DATA_RELOC_REQ:
        {
            handlePdcpSrcDlRelocReq( msgBuf_p, msgLen );
            break;
        }
        case PDCP_PR_SOURCE_UL_DATA_RELOC_REQ:
        {
            handlePdcpSrcUlRelocReq( msgBuf_p, msgLen );
            break;
        }
        case PDCP_PR_LOCAL_EMI:
        {
            handleLocalEmi( msgBuf_p, msgLen );
            break;
        }
        /*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
        case GTP_REGISTER_PDCP_IND:
        {
            handleGtpuRegistration( msgBuf_p, sockAddr);
            sendFreetoPdcp( 0, msgBuf_p);
            break;
        }
#endif
        /*SPR 21366 -*/
        default:
        {
            LOG_GTPUPR_WARNING( PR_GTPU_COMMON, "Invalid msgId[%d] received",
			    msgId );
            sendFreetoPdcp( 0, msgBuf_p);
            break;
        }
    }
}

/****************************************************************************
 * Function Name  : mbRecvDataPktsFrmPdcp
 * Inputs         : None
 * Outputs        : void
 * Returns        : None
 * Description    : This API receives the packet from PDCP over mailbox.
 ****************************************************************************/
void mbRecvDataPktsFrmPdcp( )
{
#if   defined PDCP_GTPU_INTF
    UInt8 *sockData_p = PNULL;
    SInt32 bytesRead = 0;
    SockAddr sockAddr = {0};
    
    sockData_p = (UInt8 *)getMemFromPool(MAX_RECV_BUFF_SIZE, PNULL);
    if(PNULL == sockData_p)
    {
        LOG_GTPUPR_FATAL( L2_SYS_FAIL, "Memory allocation failed" );
        return;
    }
    bytesRead = recvFromRelaySockHandler( sockData_p, MAX_RECV_BUFF_SIZE, 
            PDCP_MODULE_ID, &sockAddr );
    if(bytesRead > 0)
    {
        prPdcpProcessMessage(sockData_p,bytesRead,&sockAddr);
		uplinkRelayIngressDataSize += bytesRead;
		uplinkRelayIngressDataNum ++;
    }
    else
    {
        freeMemPool(sockData_p);
    }
#endif
}
/****************************************************************************
 * Function Name  : mbSendDataPktsToPdcp
 * Inputs         : buf_p - Pointer to mailBuffer
 *                  buf_len - Length of Buffer
 * Outputs        : void
 * Returns        : None
 * Description    : This API sends the packet to PDCP over mailbox.
 ****************************************************************************/
void mbSendDataPktsToPdcp(void *buf_p, UInt16 buf_len,UInt8 instanceId)
{
    udpSendToPdcp( (Char8 *)buf_p, buf_len, instanceId );
}

/****************************************************************************
 * Function Name  : main
 * Inputs         : None
 * Outputs        : None
 * Returns        : UInt8 - return value
 * Variables      :
 * Description    : This is the starting API of the GTPU
 ****************************************************************************/
Int main(int argc, char *argv[])
{

    QTIME currentTime;
    QTIME referTime;
    QTIME elapseTime;
    tickType_t referTick = 0;
    tickType_t currTick = 0;
	QTIME LastTime = {0};


#ifdef LINUX_PC_TEST
	adapter_init_socket();
	TestInitFrame("192.168.201.61", 45199);
	TestFrameRegisterMsgHandler(adapter_hdl_msg_callback);
#endif

    /* SPR21717 Changes Start */
    UInt32 ret = 0;
/* SPR 21804 Fix Start */
#if defined(LAYER2_DLTG_THREAD) && defined(PDCP_GTPU_INTF)
    SInt32          selectReturn;
    fd_Set          fdSock;
    SInt32          highFd = 0;
    SInt16 relaySockByteRead = 0;
    UInt8 *relaySockData = PNULL;
    timeVal    wTime;
    lteFD_ZERO( &fdSock );
#endif
/* SPR 21804 Fix End */
    ret = getCommandLineOptionsSplit(argc,argv );
    if(!ret)
    {
        fprintf (stderr, "getCommandLineOptions Failed !!!!!\n");
        return FAILURE;
    }
    lteLayer2CoreNum_g = atoi(argv[1]);
    /* SPR21717 Changes Start*/

    /* Initialize internal to RRC cell index mapping */
    layer2CommonInitCellIndexMapping();
    /* Initilaize UE mapping between RRC and L2 */
    initUeIdxMap();
    /* Initialize the memory and MSG Pools */
    initMemPool();
    initMsgPool();
    PROCESS_TO_BIND_SET_CORE_NO(getpid_wrapper(),lteLayer2CoreNum_g);
    if (thr_count_g < MAX_THREAD_ID)
    {
        threadId_g[thr_count_g] = threadSelf();
        thr_count_g++;
    }

    /* SPR21717 Changes Start*/
#ifdef LOG_PRINT_ENABLED
    /* Initialize logging framework */
    initAndConfigLogFramework( );
#endif
    /* SPR21717 Changes Start*/

    /* Initialize packet relay and GTP modules */
    initPacketRelay();
    initPdcpAdapter();
    egtpu_init();

    /* Get the current system time for reference */
    getCurrentSysTime( &referTime, &referTick );
/* SPR 21804 Fix Start */
#if  defined(LAYER2_DLTG_THREAD) && defined(PDCP_GTPU_INTF)
    UInt32 retTh = 0;
    UInt16 index = 4;
    dlTgCoreNum_g = atoi(argv[index++]);
    packetSize_g = atoi(argv[index++]);
    noOfPktVar_g = atoi(argv[index++]);
    timeInMs_g = atoi(argv[index++]);
    timeInMs_g_rx = timeInMs_g;
    putDisplayVar_g = atoi(argv[index++]);
    trafficProfileVar_g = atoi(argv[index++]);
    retTh = threadCreate(&dlTgExecutionSchedulerThId_g, THREAD_CREATE_DETACHED,
            THREAD_SCHED_DEFAULT, 0,
            THREAD_INHERIT_SCHED_DEFAULT,
            THREAD_SCOPE_SYSTEM,
            dlTgThreadExecutionSchedular, PNULL);
    if (retTh != 0) {
        ltePanic(" Unable to initialise the Layer2 because DL TG thread creation Failed.\n");
        return FAILURE; 
    }

#endif
/* SPR 21804 Fix End */

    while (1)
    {
        /* Get the current system time */
        getCurrentSysTime( &currentTime, &currTick );

        /* Send Init Indications to OAM till S1U is not initailized */
        if( LTE_FALSE == s1UInitInd_g )
        {
			/* Get the elapsed time */
        	elapseTime.s = currentTime.s - referTime.s;
            /* Init indication will be sent in 1 secs interval */
            if ( GTPU_INIT_IND_TIMER <= elapseTime.s )
            {
                s1UInitIndication();
                /* overwrite current time into referTime for base reference */
                referTime.s = currentTime.s;
                referTime.us = currentTime.us;
                referTick = currTick;
            }
        }
        else
        {
            /* Store the elapsed time after converting to ms to simulate 
             * tick */
			relayGtpuCurrentTick_g += (currTick - referTick);

            /* overwrite current time into referTime for base reference */
            referTime.s = currentTime.s;
            referTime.us = currentTime.us;
            referTick = currTick;
			
			elapseTime.s = currentTime.s - LastTime.s;
			if ( 5 <= elapseTime.s )
	        {
				fprintf(stderr, "Relay Uplink ingress traffic rate: %llu kps\n", 
	                        (uplinkRelayIngressDataSize*8)/(1000*elapseTime.s));
				fprintf(stderr, "Relay Uplink egress traffic rate: %llu kbps\n", 
	                        (uplinkRelayEgressDataSize*8)/(1000*elapseTime.s));
				fprintf(stderr, "Relay Uplink ingress traffic num: %llu\n", 
	                        uplinkRelayIngressDataNum);
				fprintf(stderr, "Relay Uplink egress traffic num: %llu\n\n", 
	                        uplinkRelayEgressDataNum);
				
				uplinkRelayIngressDataSize   = 0;
				uplinkRelayEgressDataSize    = 0;
				LastTime.s = currentTime.s;
	        }
        }

        /* Check echo timer expiry */ 
/* SPR 23260 + */
#ifndef __x86_64__
        /*
 * Crash has being observed in this function in case of 64 bit platform.
 * For now, commenting out this, will fix it later
 * */
        egtpu_check_echo_timer_expiry();
#endif
/* SPR 23260 - */

        /* Call the function to receive messages from OAM/RRC/Fast socket */
/* SPR 21804 Fix Start */
#if defined( LAYER2_DLTG_THREAD) && defined(PDCP_GTPU_INTF)
        wTime.tv_sec = 0;
        wTime.tv_usec = 1;
        relaySockByteRead = 0;
        if(0 == initIndTimer_g)
        {
            lteFD_SET(relayConfigInfo_g.s1ApTxFd,&fdSock);
            if (relayConfigInfo_g.s1ApTxFd > highFd) highFd = relayConfigInfo_g.s1ApTxFd;
        }
        while( (selectReturn = select_wrapper( highFd + 1, &fdSock, 0, 0, &wTime )) < 0)
        {
            if (errno == EINTR)
                continue;
            LOG_GTPU_MSG(PR_MU_ADAPTER_GLUE_SELECT_FAILED,LOGWARNING, PR_GTPU_DL_DATA,
                    relayGtpuCurrentTick_g,__LINE__,0,0,0,0,0,0,__func__,"");
            pError("MU_Adapter Glue Select Failed!");
            return PNULL;
        }
        if( selectReturn )
        {
            relaySockData = (UInt8 *)getMemFromPool(MAX_RECV_BUFF_SIZE, PNULL);
            if(PNULL == relaySockData)
            {
                return;
            }
            if(0 == initIndTimer_g)
            {
                if (lteFD_ISSET(relayConfigInfo_g.s1ApTxFd,&fdSock))
                {
				/* SPR 15497 fix start */
                    /* Clear the Recv buffer pointer to avoid garbage values */
                    memSet(relaySockData, 0, MAX_RECV_BUFF_SIZE);
				/* SPR 15497 fix end */
                    relaySockByteRead = recvFromRelaySockHandler(relaySockData,MAX_RECV_BUFF_SIZE,RELAY_S1AP_MODULE_ID,PNULL);
                    lteFD_CLR(relayConfigInfo_g.s1ApTxFd,&fdSock);
                }
            }
            if(relaySockByteRead > 0)
            {
                lteRelaySockHandler(relaySockData,relaySockByteRead);
            }
            freeMemPool(relaySockData);
        }
        normalModeForMacExecSchedular();
#else
        /* Call the function to receive messages from OAM/RRC/Fast socket */
        handleUserData();
#endif
/* SPR 21804 Fix End */
        /* Receive the messages from PDCP over mail box */
        //mbRecvDataPktsFrmPdcp( );
    }
    return SUCCESS;
}

/* Implementation of functions from Packet Relay to PDCP */
/****************************************************************************
 * Function Name  : pdcpDataReq
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  transactionId - Transaction Id
 *                  data_p  - pointer to PDCP SDU data in Zero-copy
 *                            buffering format.
 *                  msgBuffer_p - pointer from fast socket buffer
 *                  size    - size of data.
 * Outputs        : void
 * Returns        : None
 * Description    : This function constructs PDCP_PR_DATA_REQ and passes to 
 *                  PDCP over mailbox.
 ****************************************************************************/
void pdcpDataReq( UInt16 ueIndex, UInt16 lcId, UInt32  transactionId,
        void* data_p, void *msgBuffer_p, UInt16 size, UInt8 instanceId )
{
    LP_PdcpUeData pdcpData_p = PNULL;
/*SPR 21653 changes start*/
    UInt16 hdrSize           = INTERFACE_API_HEADER_LEN;
/*SPR 21653 changes end*/
    UInt16 totSize           = hdrSize + sizeof(PdcpUeData) + size;
/* SPR 21812 Fix Start */
    UInt8 *msgStart_p        = data_p - (hdrSize + sizeof(PdcpUeData));
/* SPR 21812 Fix End */
    /* Point the pdcpData_p to point post header */
    pdcpData_p = (LP_PdcpUeData)(msgStart_p + hdrSize );

    /* Fill the structure */
    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with PDCP */
    pdcpData_p->ueIndex = 
        relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].rrcUeIndex;
    /* - SPR 19169 */
    pdcpData_p->lcId    = lcId;
    pdcpData_p->size    = size;

    /* Prepare the message */
    /* Do not care for the cell Index as PDCP will take care from its UE 
     * context */
/* SPR 21812 Fix Start */
    appendApiHeader( PDCP_MODULE_ID, PDCP_PR_DATA_REQ, transactionId, 
            totSize, msgStart_p, API_HEADER_CELL_ID_NIL );

    /* Send the message to PDCP */
    mbSendDataPktsToPdcp( msgStart_p, totSize ,instanceId );
    /* Free the memory */
    freeMemPool( msgBuffer_p );
/* SPR 21812 Fix End */
}

/****************************************************************************
 * Function Name  : pdcpSourceDataFwdReq
 * Inputs         : ueIndex - UE Id.
 *                  data_p  - pointer to PDCP SDU data in Zero-copy
 *                            buffering format.
 *                  size    - size of data.
 *                  instanceId - Instance number
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function constructs PDCP_PR_SOURCE_DATA_FWD_REQ
 *                  and passes the same to PDCP over mailbox.
 ****************************************************************************/
void pdcpSourceDataFwdReq( void* data_p, UInt16 size, UInt8 instanceId )
{
    prDataFwdReq *prDataFwdReq_p = (prDataFwdReq *)data_p;
    UInt8 *msgBuffer_p = PNULL;
    prDataFwdReq *prDataFwdReqTemp_p = PNULL;
    UInt8 idx = 0;
    /* Add PDCP-PR header size to the size passed */
/*SPR 21653 changes start*/
    size += INTERFACE_API_HEADER_LEN;
/*SPR 21653 changes end*/

    /* Memory Allocation */
    /*SPR 21188 +*/
    msgBuffer_p = (UInt8 *)getMemFromPool(size, PNULL);
    /*SPR 21188 -*/
    if(!msgBuffer_p)
    {
        LOG_GTPUPR_FATAL( L2_SYS_FAIL, "Cannot allocate memory, instanceId[%u]", instanceId );
        return;
    }
/*SPR 21653 changes start*/
    prDataFwdReqTemp_p = (prDataFwdReq *)( msgBuffer_p + 
            INTERFACE_API_HEADER_LEN );
/*SPR 21653 changes end*/

    /* Fill the structure to be sent over mailbox */
    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with PDCP */
    prDataFwdReqTemp_p->ueIndex = 
        relayMapBasedOnUeIndexLcId_g[prDataFwdReq_p->ueIndex][0].rrcUeIndex;
    /* - SPR 19169 */
    prDataFwdReqTemp_p->count = prDataFwdReq_p->count;

    /* Fill the following information for valid number of entries only,
     * since other entries won't be read by PDCP */
    for( idx = 0; idx < prDataFwdReq_p->count; idx++ )
    {
        prDataFwdReqTemp_p->dataFwdInfo[idx].lcId = prDataFwdReq_p->
            dataFwdInfo[idx].lcId;
        prDataFwdReqTemp_p->dataFwdInfo[idx].ulDataFwdFlag = 
            prDataFwdReq_p->dataFwdInfo[idx].ulDataFwdFlag;
    }

    /* Prepare the message */
    /* Do not care for the cell Index as the forwarding request is UE specific
     * for which cell information is not required */
    appendApiHeader( PDCP_MODULE_ID, PDCP_PR_SOURCE_DATA_FWD_REQ, 0xFF, 
            size, msgBuffer_p, API_HEADER_CELL_ID_NIL );

    /* Send the message to PDCP */
    mbSendDataPktsToPdcp( msgBuffer_p, size ,instanceId);
    /* Free the memory */
    freeMemPool( msgBuffer_p );
}

/****************************************************************************
 * Function Name  : pdcpSourceDataFwdReqInIntraeNB
 * Inputs         : oldUeIndex - old UE Index
 *                  newUeIndex - new UE Index
 *                  instanceId - Instance number
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function constructs PDCP_PR_INTRA_SOURCE_DATA_FWD_REQ
 *                  and passes the same to PDCP over mailbox.
 ****************************************************************************/
void pdcpSourceDataFwdReqInIntraeNB( UInt16 oldUeIndex, UInt16 newUeIndex,
        UInt8 instanceId )
{
    LP_PdcpIntraDataFwd dataFwd_p = PNULL;
    UInt8 *msgBuffer_p = PNULL;
/*SPR 21653 changes start*/
    UInt16 size = INTERFACE_API_HEADER_LEN + sizeof( PdcpIntraDataFwd );
/*SPR 21653 changes end*/

    /* Memory Allocation */
    /*SPR 21188 +*/
    msgBuffer_p = (UInt8 *)getMemFromPool(size, PNULL);
    /*SPR 21188 -*/

    if(!msgBuffer_p)
    {
        LOG_GTPUPR_FATAL( L2_SYS_FAIL, "Cannot allocate memory, instanceId[%u]", 
                instanceId );
        return;
    }
/*SPR 21653 changes start*/
    dataFwd_p = (LP_PdcpIntraDataFwd)( msgBuffer_p + INTERFACE_API_HEADER_LEN );
/*SPR 21653 changes end*/

    /* Fill the structure to be sent over mailbox */
    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with PDCP */
    dataFwd_p->oldUeIndex = 
        relayMapBasedOnUeIndexLcId_g[oldUeIndex][0].rrcUeIndex;
    dataFwd_p->newUeIndex = 
        relayMapBasedOnUeIndexLcId_g[newUeIndex][0].rrcUeIndex;
    /* - SPR 19169 */

    /* Prepare the message */
    /* Do not care for the cell Index as the forwarding request is UE specific
     * for which cell information is not required */
    appendApiHeader( PDCP_MODULE_ID, PDCP_PR_INTRA_SOURCE_DATA_FWD_REQ, 0xFF,
            size, msgBuffer_p, API_HEADER_CELL_ID_NIL );

    /* Send the message to PDCP */
    mbSendDataPktsToPdcp( msgBuffer_p, size ,instanceId);
    /* Free the memory */
    freeMemPool( msgBuffer_p );
}

/****************************************************************************
 * Function Name  : pdcpTargetDlDataRelocReq
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  sn      - PDCP PDU Sequence Number
 *                  data_p  - pointer to PDCP SDU data in Zero-copy
 *                            buffering format.
 *                  msgBuffer_p - pointer from fast socket buffer
 *                  size    - size of data.
 *                  instanceId - Instance number
 * Outputs        : void
 * Returns        : None
 * Description    : This function constructs PDCP_PR_TARGET_DL_DATA_RELOC_REQ 
 *                  and passes to PDCP over mailbox.
 ****************************************************************************/
void pdcpTargetDlDataRelocReq( UInt16 ueIndex, UInt8 lcId, UInt16 sn,
        void *data_p, void *msgBuffer_p, UInt16 size, UInt8 instanceId )
{
    LP_PdcpDataReloc dataReloc_p = PNULL;
/*SPR 21653 changes start*/
    UInt16 hdrSize     = INTERFACE_API_HEADER_LEN;
/*SPR 21653 changes end*/
    UInt16 totSize     = hdrSize + sizeof(PdcpDataReloc) + size;
    UInt8 *msgStart_p        = data_p - (hdrSize + sizeof(PdcpDataReloc));
    
    /* Point the pdcpData_p to point post header */
    dataReloc_p = (LP_PdcpDataReloc)( msgStart_p + hdrSize );
    
    /* Fill the structure */
    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with PDCP */
    dataReloc_p->ueData.ueIndex = 
        relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].rrcUeIndex;
    /* - SPR 19169 */
    dataReloc_p->ueData.lcId    = lcId;
    dataReloc_p->ueData.size    = size;
    dataReloc_p->sn             = sn;

    /* Prepare the message */
    /* Do not care for the cell Index as PDCP will take care from its UE 
     * context */
    appendApiHeader( PDCP_MODULE_ID, PDCP_PR_TARGET_DL_DATA_RELOC_REQ, 0xFF, 
            totSize, msgStart_p, API_HEADER_CELL_ID_NIL );

    /* Send the message to PDCP */
    mbSendDataPktsToPdcp( msgStart_p, totSize ,instanceId );
    /* Free the memory */
    freeMemPool( msgBuffer_p );
}

/****************************************************************************
 * Function Name  : pdcpTargetUlDataRelocReq
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  sn      - PDCP PDU Sequence Number
 *                  data_p  - pointer to PDCP SDU data in Zero-copy
 *                            buffering format.
 *                  msgBuffer_p - pointer from fast socket buffer
 *                  size    - size of data.
 *                  instanceId - Instance number
 * Outputs        : void
 * Returns        : None
 * Description    : This function constructs PDCP_PR_TARGET_UL_DATA_RELOC_REQ 
 *                  and passes to PDCP over mailbox.
 ****************************************************************************/
void pdcpTargetUlDataRelocReq( UInt16 ueIndex, UInt8 lcId, UInt16 sn,
        void *data_p, void *msgBuffer_p, UInt16 size, UInt8 instanceId )
{
    LP_PdcpDataReloc dataReloc_p = PNULL;
/*SPR 21653 changes start*/
    UInt16 hdrSize     = INTERFACE_API_HEADER_LEN;
/*SPR 21653 changes end*/
    UInt16 totSize     = hdrSize + sizeof(PdcpDataReloc) + size;
    UInt8 *msgStart_p        = data_p - (hdrSize + sizeof(PdcpDataReloc));
    
    /* Point the pdcpData_p to point post header */
    dataReloc_p = (LP_PdcpDataReloc)( msgStart_p + hdrSize );
    
    /* Fill the structure */
    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with PDCP */
    dataReloc_p->ueData.ueIndex =
        relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].rrcUeIndex;
    /* - SPR 19169 */
    dataReloc_p->ueData.lcId    = lcId;
    dataReloc_p->ueData.size    = size;
    dataReloc_p->sn             = sn;

    /* Prepare the message */
    /* Do not care for the cell Index as PDCP will take care from its UE 
     * context */
    appendApiHeader( PDCP_MODULE_ID, PDCP_PR_TARGET_UL_DATA_RELOC_REQ, 0xFF, 
            totSize, msgStart_p, API_HEADER_CELL_ID_NIL );

    /* Send the message to PDCP */
    mbSendDataPktsToPdcp( msgStart_p, totSize ,instanceId );
    /* Free the memory */
    freeMemPool( msgBuffer_p );
}

/****************************************************************************
 * Function Name  : pdcpEndMarkerInd
 * Inputs         : ueIndex, lcId.
 *                  instanceId - Instance number
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function constructs PDCP_PR_END_MARKER_IND and 
 *                  passes to PDCP over mailbox when EMI is received from SGW.
 ****************************************************************************/
void pdcpEndMarkerInd( UInt16 ueIndex, UInt8 lcId, UInt8 instanceId )
{
    LP_PdcpEmi emi_p = PNULL;
    UInt8 *msgBuffer_p = PNULL;
/*SPR 21653 changes start*/
    UInt16 size = INTERFACE_API_HEADER_LEN + sizeof( PdcpEmi );
/*SPR 21653 changes end*/

    /* Memory Allocation */
    /*SPR 21188 +*/
    msgBuffer_p = (UInt8 *)getMemFromPool(size, PNULL);
    /*SPR 21188 -*/


    if(!msgBuffer_p)
    {
        LOG_GTPUPR_ERROR(PR_GTPU_CONTROL,"Cannot allocate memory: "
                "instanceId[%d]\n", instanceId);
        return;
    }
/*SPR 21653 changes start*/
    emi_p = (LP_PdcpEmi)( msgBuffer_p + INTERFACE_API_HEADER_LEN );
/*SPR 21653 changes end*/

    /* Fill the structure to be sent over mailbox */
    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with PDCP */
    emi_p->ueIndex = 
        relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].rrcUeIndex;
    /* - SPR 19169 */
    emi_p->lcId    = lcId;

    /* Prepare the message */
    /* Do not care for the cell Index as PDCP will take care from its UE 
     * context */
    appendApiHeader( PDCP_MODULE_ID, PDCP_PR_END_MARKER_IND, 0xFF,
            size, msgBuffer_p, API_HEADER_CELL_ID_NIL );

    /* Send the message to PDCP */
    mbSendDataPktsToPdcp( msgBuffer_p, size ,instanceId);
    /* Free the memory */
    freeMemPool( msgBuffer_p );
}

/* Implementation of functions from PDCP to Packet Relay */
/****************************************************************************
 * Function Name  : handlePdcpUeDataInd
 * Inputs         : msg_p - Pointer to message
 *                  bytesread - length of message
 * Outputs        : void
 * Returns        : None
 * Description    : This API processes the PDCP_PR_UE_DATA_IND and calls the
 *                  relevant function.
 ****************************************************************************/
void handlePdcpUeDataInd( UInt8 *msg_p, SInt32 bytesread )
{
    UInt8 *data_p = PNULL;
    /* Get the message pointer from the request received from PDCP */
/*SPR 21653 changes start*/
    LP_PdcpUeData pdcpData_p = (LP_PdcpUeData)( msg_p +
            INTERFACE_API_HEADER_LEN );
/*SPR 21653 changes end*/

    do
    {
        /* Get the pointer to actual data segment */
        UInt8 *dataPl_p = (UInt8 *)pdcpData_p + sizeof( PdcpUeData );

        /* Although ZCB is not required over here, still allocating ZCB to
         * minimize the changes in pdcpDataInd funcion */
        /* Allocate the memory for ZCB  */
        data_p = msgAlloc(0,0,0,0);
        if (PNULL == data_p)
        {
            sendFreetoPdcp( 0, msg_p);            
            ltePanic("msgAlloc failed in %s", __FUNCTION__);
            break;
        }

        /* Add the memory segment received from PDCP to ZCB segment
         * buffer */
        if (0 == qvMsgInsertExternal( data_p, 0, dataPl_p,
                    pdcpData_p->size, sendFreetoPdcp, msg_p))
        {
            msgFree( data_p );
            sendFreetoPdcp( 0, msg_p);
            ltePanic("qvMsgInsertExternal failed in %s",__func__);
            break;
        }
        /* + SPR 19169 */
        /* Communication needs to be done for RRC UEIndex with PDCP */
        /*SPR 21366 +*/
        UInt16 ueIndex = getUeIdxFromRrcUeIdx( pdcpData_p->ueIndex );
        /*SPR 21366 -*/

        /* Function call to process the PdcpDataInd from PDCP */
        pdcpDataInd( ueIndex, pdcpData_p->lcId,
                data_p, pdcpData_p->size);
        /* - SPR 19169 */
    } while( 0 );
}

/****************************************************************************
 * Function Name  : handlePdcpSrcDlRelocReq
 * Inputs         : msg_p - Pointer to message
 *                  bytesread - length of message
 * Outputs        : void
 * Returns        : None
 * Description    : This API processes the PDCP_PR_SOURCE_DL_DATA_RELOC_REQ 
 *                  and calls the relevant function.
 ****************************************************************************/
void handlePdcpSrcDlRelocReq( UInt8 *msg_p, SInt32 bytesread )
{
    UInt8 *data_p = PNULL;
    /* Get the message pointer from the request received from PDCP */
/*SPR 21653 changes start*/
    LP_PdcpDataReloc relocData_p = (LP_PdcpDataReloc)( msg_p +
            INTERFACE_API_HEADER_LEN );
/*SPR 21653 changes end*/

    do
    {
        /* Get the pointer to actual data segment */
         UInt8 *dataPl_p = (UInt8 *)relocData_p + sizeof( PdcpDataReloc );
        /* Although ZCB is not required over here, still allocating ZCB to
         * minimize the changes in pdcpDataInd funcion */
        /* Allocate the memory for ZCB  */
        data_p = msgAlloc(0,0,0,0);
        if (PNULL == data_p)
        {
            sendFreetoPdcp( 0, msg_p);            
            ltePanic("msgAlloc failed in %s", __FUNCTION__);
            break;
        }

        /* Add the memory segment received from PDCP to ZCB segment
         * buffer */
        if (0 == qvMsgInsertExternal( data_p, 0, dataPl_p,
                    relocData_p->ueData.size, sendFreetoPdcp, msg_p))
        {
            msgFree( data_p );
            sendFreetoPdcp( 0, msg_p);
            ltePanic("qvMsgInsertExternal failed in %s",__func__);
            break;
        }
        /* + SPR 19169 */
        /* Communication needs to be done for RRC UEIndex with PDCP */
        /*SPR 21366 +*/
        UInt16 ueIndex = getUeIdxFromRrcUeIdx( relocData_p->ueData.ueIndex );
        /*SPR 21366 -*/
        
        /* Function call to process forwarded pkt from PDCP */
        if ( SUCCESS != pdcpSourceDlDataRelocInd( ueIndex,
                    relocData_p->ueData.lcId, relocData_p->sn, data_p,
                    relocData_p->ueData.size ) )
            /* - SPR 19169 */
        {
            /* Unable to send the data packet to peer */
            /* Log and free the memory */
            LOG_MSG( PDCP_DL_DATA_FWD_FAILS, LOGWARNING, PDCP_TX, 
                    relayGtpuCurrentTick_g, __LINE__, 
                    relocData_p->ueData.ueIndex, relocData_p->ueData.lcId,
                    relocData_p->sn, relocData_p->ueData.size, 
                    0,0, __func__,"");
        }
        /* Free the memory as the buffering needs to be done at PDCP only and
         * not at relay */
        msgFree( data_p );
    } while( 0 );
}

/****************************************************************************
 * Function Name  : handlePdcpSrcUlRelocReq
 * Inputs         : msg_p - Pointer to message
 *                  bytesread - length of message
 * Outputs        : void
 * Returns        : None
 * Description    : This API processes the PDCP_PR_SOURCE_UL_DATA_RELOC_REQ 
 *                  and calls the relevant function.
 ****************************************************************************/
void handlePdcpSrcUlRelocReq( UInt8 *msg_p, SInt32 bytesread )
{
    UInt8 *data_p = PNULL;
    /* Get the message pointer from the request received from PDCP */
/*SPR 21653 changes start*/
    LP_PdcpDataReloc relocData_p = (LP_PdcpDataReloc)( msg_p +
            INTERFACE_API_HEADER_LEN );
/*SPR 21653 changes end*/

    do
    {
        /* Get the pointer to actual data segment */
        UInt8 *dataPl_p = (UInt8 *)relocData_p + sizeof( PdcpDataReloc );
        /* Although ZCB is not required over here, still allocating ZCB to
         * minimize the changes in pdcpDataInd funcion */
        /* Allocate the memory for ZCB  */
        data_p = msgAlloc(0,0,0,0);
        if (PNULL == data_p)
        {
            sendFreetoPdcp( 0, msg_p);            
            ltePanic("msgAlloc failed in %s", __FUNCTION__);
            break;
        }

        /* Add the memory segment received from PDCP to ZCB segment buffer */
        if (0 == qvMsgInsertExternal( data_p, 0, dataPl_p,
                    relocData_p->ueData.size, sendFreetoPdcp, msg_p))
        {
            msgFree( data_p );
            sendFreetoPdcp( 0, msg_p);
            ltePanic("qvMsgInsertExternal failed in %s",__func__);
            break;
        }
        /* + SPR 19169 */
        /* Communication needs to be done for RRC UEIndex with PDCP */
        /*SPR 21366 +*/
        UInt16 ueIndex = getUeIdxFromRrcUeIdx( relocData_p->ueData.ueIndex );
        /*SPR 21366 -*/
        
        /* Function call to process forwarded pkt from PDCP */
        if ( SUCCESS != pdcpSourceUlDataRelocInd( ueIndex,
                    relocData_p->ueData.lcId, relocData_p->sn, data_p,
                    relocData_p->ueData.size
#ifdef KPI_STATS
                    , 0 /* Don't care*/
#endif
                    ) )
            /* - SPR 19169 */
        {
            /* Unable to send the data packet to peer */
            /* Log and free the memory */
            LOG_MSG( PDCP_UL_DATA_FWD_FAILS, LOGWARNING, PDCP_TX, 
                    relayGtpuCurrentTick_g, __LINE__, 
                    relocData_p->ueData.ueIndex, relocData_p->ueData.lcId,
                    relocData_p->sn, relocData_p->ueData.size, 
                    0,0, __func__,"");
        }
        /* Free the memory as the buffering needs to be done at PDCP only and
         * not at relay */
        msgFree( data_p );
    } while( 0 );
}

/****************************************************************************
 * Function Name  : handleLocalEmi
 * Inputs         : msg_p - Pointer to message
 *                  bytesread - length of message
 * Outputs        : void
 * Returns        : None
 * Description    : This API processes the PDCP_PR_LOCAL_EMI and calls the 
 *                  relevant function.
 ****************************************************************************/
void handleLocalEmi( UInt8 *msg_p, SInt32 bytesread )
{
    /* Get the message pointer from the request received from PDCP */
/*SPR 21653 changes start*/
    LP_PdcpLocalEmi pdcpData_p = (LP_PdcpLocalEmi)( msg_p +
            INTERFACE_API_HEADER_LEN );
/*SPR 21653 changes end*/

    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with PDCP */
        /*SPR 21366 +*/
    UInt16 ueIndex = getUeIdxFromRrcUeIdx( pdcpData_p->emi.ueIndex );
        /*SPR 21366 -*/
        
    /* Function call to process local EMI from PDCP */
    pdcpEndOfDataForwardingInd( ueIndex, pdcpData_p->emi.lcId,
            pdcpData_p->isEmiRecvd );
    /* - SPR 19169 */

    /* Send free notification to PDCP after processing the message */
    sendFreetoPdcp( 0, msg_p);
}
/*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
/******************************************************************************
 * Function Name  : handleGtpuRegistration 
 * Inputs         : msg_p,
 *                  sockAddr
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion create the socket for sending data to PDCP as
 *                  per given L2 instace Id.
 *****************************************************************************/
void handleGtpuRegistration( UInt8 *msg_p, SockAddr *sockAddr)
{
    LP_PdcpGtpuRegistration pdcpGtpuReg_p = (PdcpGtpuRegistration *)( msg_p +
            INTERFACE_API_HEADER_LEN );

    RrcCellIndex cellIndex = 0;

    if( RELAY_INVALID_FD != ( relayConfigInfo_g.
                gtpu_Pdcp_Ip_port[pdcpGtpuReg_p->instanceId].pdcpRxFd = 
                socket_wrapper( AF_INET, SOCK_DGRAM, 0 ) ) )
    {
        relayConfigInfo_g.gtpu_Pdcp_Ip_port[pdcpGtpuReg_p->instanceId].
            pdcpRxAddr.sin_family = AF_INET;
        relayConfigInfo_g.gtpu_Pdcp_Ip_port[pdcpGtpuReg_p->instanceId].
            pdcpRxAddr.sin_port = sockAddr->sin_port;
        relayConfigInfo_g.gtpu_Pdcp_Ip_port[pdcpGtpuReg_p->instanceId].
            pdcpRxAddr.sin_addr = sockAddr->sin_addr;
    }
    for( ; cellIndex < pdcpGtpuReg_p->cellCount; cellIndex++ )
    {
        gtpu_Pdcp_Instance_g[pdcpGtpuReg_p->cellIdList[cellIndex]] =
            pdcpGtpuReg_p->instanceId;
    }
	/*OPTION3X changes start */
#ifdef ENDC_ENABLED
	if( RELAY_INVALID_FD != ( relayConfigInfo_g.
	        	x2uRlc_Ip_Port[pdcpGtpuReg_p->instanceId].rlcRxFd =
	        	socket_wrapper( AF_INET, SOCK_DGRAM, 0 ) ) )
	{
	    relayConfigInfo_g.x2uRlc_Ip_Port[pdcpGtpuReg_p->instanceId].
	        rlcRxAddr.sin_family = AF_INET;
	    relayConfigInfo_g.x2uRlc_Ip_Port[pdcpGtpuReg_p->instanceId].
	        rlcRxAddr.sin_addr = sockAddr->sin_addr;
	}
#endif
	/*OPTION3X changes End */

}
#endif
/*SPR 21366 -*/
#endif
