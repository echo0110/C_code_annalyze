/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacPhyInterface.c,v 1.1.1.1.6.1.4.2.2.3 2010/10/25 08:36:36 gur23971 Exp $
 *
 ******************************************************************************
 *
 *  File Description : 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacPhyInterface.c,v $
 * Revision 1.1.1.1.6.1.4.2.2.3  2010/10/25 08:36:36  gur23971
 * stats related changes
 *
 * Revision 1.1.1.1.6.1.4.2.2.2  2010/10/22 12:44:10  gur04640
 * Statistics code merged from branch BRFramework_RRC1_0_RLC_AM_MAC_8_8_FAPI_optimizations
 *
 * Revision 1.1.1.1.6.1.4.2.2.1.2.1  2010/10/13 10:54:45  gur04640
 * Changes done for implementing Stats using shared memory
 *
 * Revision 1.1.1.1.6.1.4.2.2.1.2.1  2010/10/12 02:45:30  gur04640
 * Changes done for implementing Stats using shared memory
 *
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:46:08  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.25.26.1  2010/07/21 09:34:58  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. Created memory pool for UL data
 * 2. Added a warning log statement
 * 3. Commented few lines
 * 4. Endianness fix.
 * 5. Changed data type used for port number variable from u32 to u16
 *
 * Revision 1.25  2009/12/31 08:48:27  gur19413
 * removed comment fcntl(phyRxSockFD_g,F_SETFL,O_NONBLOCK);
 *
 * Revision 1.24  2009/12/02 12:00:03  gur19413
 * bug fixed
 *
 * Revision 1.23  2009/12/01 15:35:24  gur19413
 * added new function
 *
 * Revision 1.22  2009/10/27 07:10:43  gur19413
 * reverted RAW Socket changes
 *
 * Revision 1.21  2009/10/26 13:33:51  gur11912
 * removed warnings
 *
 * Revision 1.20  2009/10/26 13:17:14  gur11912
 * removed warnings
 *
 * Revision 1.19  2009/10/26 13:06:27  gur11912
 * modified for RAW Sockets
 *
 * Revision 1.18  2009/10/26 10:31:21  gur19413
 * remove ifindex to 2
 *
 * Revision 1.17  2009/10/26 10:16:01  gur19413
 * modified socket functionality
 *
 * Revision 1.16  2009/10/21 05:06:54  gur19413
 * Socket Functionality Modified
 *
 * Revision 1.15  2009/10/05 08:07:32  gur11912
 * optimization changes
 *
 * Revision 1.14  2009/09/02 17:27:23  gur19413
 * return type changed UInt32 to SInt32
 *
 * Revision 1.13  2009/08/24 05:48:40  gur19413
 * added UT_TESTING FLAG
 *
 * Revision 1.12  2009/07/22 06:58:42  gur19413
 * added socket related changes.
 *
 * Revision 1.11  2009/07/16 08:05:42  gur19413
 * added logs in func  msgsend and msgrcv
 *
 * Revision 1.10  2009/07/13 09:25:24  gur19413
 * removed commented code
 *
 * Revision 1.9  2009/07/13 04:34:52  gur19413
 * changed log type
 *
 * Revision 1.8  2009/07/10 11:16:10  gur19413
 * remove warnings
 *
 * Revision 1.7  2009/06/22 08:18:09  gur19413
 * remove warnings
 *
 * Revision 1.6  2009/06/17 00:14:26  gur19413
 * bugs fixed regarding shared memory
 *
 * Revision 1.5  2009/06/10 12:41:16  gur19413
 * comments incorportated
 *
 * Revision 1.4  2009/06/08 15:34:56  gur19413
 * added new func sendtoPhy
 *
 * Revision 1.3  2009/06/05 12:22:42  gur19413
 * added new function for DL
 *
 * Revision 1.2  2009/06/05 09:45:11  gur19413
 * remove compilation error
 *
 * Revision 1.1  2009/05/28 08:46:21  gur19413
 * Initial Version
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacComPorting.h"
#include "lteMacReceiver.h"
#include "lteMacPhyInterface.h"
#include "lteMacFAPIApi.h"
#include "lteMacTimingAdvanceMgr.h"
#include "lteMacCellConfMgr.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteCommonStatsManager.h"
#ifdef LAYER2_PHYSIM_THREAD
#include "lteEnbPhySim.h"
extern LTE_SEM phySimReceiveSem_g;
extern LTE_SEM rxTxSem_g;
extern  SInt32 macControlEventFd_g;
extern  SInt32 macDataEventFd_g;

#endif
#ifdef DL_UL_SPLIT
#include "lteMacDLSchedulerStrategy.h"
/*CA Changes start  */
extern SInt32 ackNackQueueIdDL_g[MAX_NUM_CELL];
extern SInt32 ackNackQueueIdUL_g[MAX_NUM_CELL];
/*CA Changes end  */
/*warning removed*/
extern SInt32 closeIccQ(SInt32 mqdes);
#endif
#ifndef DL_UL_SPLIT
extern LTE_SQUEUE* ackNackQueue_gp[MAX_NUM_CELL];
#endif

extern CellConfigPhy *cellConfigPhy_gp[MAX_NUM_CELL];
/******************************************************************************
  Private Definitions
 *****************************************************************************/


/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* SPR 19288 change start */
/* SPR 19288 change end */
void initPhySocketInterface(UInt8 numCellsConfigured);
STATIC SInt32 sendMsgBySocket(UInt8 *bufferToSend_p,UInt32 bufferLen,
                           InternalCellIndex internalCellIndex);
/*SPR 21001 Changes end */
/* +- SPR 17777 */
extern void * prepare_FAPI_logs(UInt32 logId, SInt32 msgSize, const char* funcName);
/* +- SPR 17777 */


#ifdef FLEXRAN
extern SInt32 flexranFapiLoggingEnable_g;
extern UInt32 g_flexran_channelId[2] ; 
SInt32 flexranWiresharkFd_g [MAX_NUM_CELL] = {0};
SockAddr flexranTx_g[MAX_NUM_CELL];
void initWiresharkLoggingInterface(InternalCellIndex internalCellIndex);
int open_raw_sock(const char* src_ip, const char* dst_ip);
extern void do_tap_fun(void* data, uint32_t len, InternalCellIndex cellId);
extern void initWiresharkLoggingInterfaceLocal(InternalCellIndex internalCellIndex);
#endif

/* Fixed for common l2 bin for 9131/9132 */
/* SPR 19288 change start */

/* SPR 19288 change end */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*Socket file descriptor used in creating Socket */
/*CA Changes start  */
#ifndef FLEXRAN
SInt32 phyRxSockFD_g[MAX_NUM_CELL] = {0};
/*Socket file descriptor used in creating Socket */
SInt32 phyTxSockFD_g[MAX_NUM_CELL] = {0};
#endif
#ifdef DL_UL_SPLIT_TDD
SInt32 phyRxSockUlFD_g = 0;
UInt16 phyRxPortUl_g = 0;
#endif


/*CA Changes end  */
/* SPR 19288 change start */
/* SPR 19288 change end */

/* PHY Recv Port*/
UInt16 phyRxPort_g[MAX_NUM_CELL] = {0};

/* PHY Tx Port*/
UInt16 phyTxPort_g[MAX_NUM_CELL] = {0};

/*Share Message Queue ID for Uplink*/
SInt32 shMsgQRXId_g;

/*Share Message Queue ID for Downlink*/
SInt32 shMsgQTXId_g;

/* Socket creation for Receiving data from PHY Layer */
SockAddr phyRxAddr_g[MAX_NUM_CELL];
/* Socket creation for Transmitting data to PHY Layer */
SockAddr phyTxAddr_g[MAX_NUM_CELL];

/* CA Changes start */
#ifndef FLEXRAN
SInt32 phyControlSockFD_g[MAX_NUM_CELL] = {0};
#endif
UInt16 phyControlRxPort_g[MAX_NUM_CELL] = {0};
UInt16 phyControlTxPort_g[MAX_NUM_CELL] = {0};

/* Socket creation for Transmitting control info to PHY Layer */
SockAddr phyControlTxAddr_g[MAX_NUM_CELL];
/* CA Changes end */
SInt32 phyFrameIndSockFD_g = 0;
UInt16 phyFrameIndPort_g = 0;
/* Socket creation for ireceiving frame ind from PHY Layer */
SockAddr phyFrameIndAddr_g;
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/* SPR 19288 change start */
/* SPR 19288 change end */

/****************************************************************************
 * Function Name  : initPHYInterface
 * Inputs         : numCellsConfigured
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialize the PHY Reciever Intterface
 *                   and Timing Advance Manager & Transmitter.
 ****************************************************************************/
/*CA Changes start  */
void initPHYInterface(UInt8 numCellsConfigured)
/*CA Changes end  */
{

    UInt8 cellCount = 0;
    /*CA Changes end  */
    initPHYReceiver(numCellsConfigured);
    initTimingAdvanceMgr(numCellsConfigured);
    /*CA Changes end  */
    /* Memory handling changes start */
#ifdef LAYER2_PHYSIM_THREAD
    createMemPool(sizeof(macPhyMsg), (dlSchdConfig_g.maxUEsToBeScheduledDL));
#endif    
    createMemPool(FAPI_CTRL_MSG_MAX_BUF_SZ,numCellsConfigured);
    /* Memory handling changes end */

    createMemPool(sizeof(CellConfigPhy), numCellsConfigured);
    for (cellCount = 0 ; cellCount < numCellsConfigured ; cellCount++)
    {
        GET_MEM_FROM_POOL(CellConfigPhy, cellConfigPhy_gp[cellCount],
                sizeof(CellConfigPhy),PNULL);
    }
}

#ifdef FLEXRAN
extern char macIPAddress_g[64];
void initWiresharkLoggingInterface(InternalCellIndex internalCellIndex)
{
	//char src_ip[] = "192.168.1.132";
	//char dst_ip[] = "192.168.1.132";
	
    open_raw_sock(macIPAddress_g, macIPAddress_g);
    
    fprintf(stderr,"\n----------------------INTERFACE IS OPEN, src_ip %s, dst_ip %s-------------------------\n",
    		macIPAddress_g, macIPAddress_g);
    //initWiresharkLoggingInterfaceLocal(internalCellIndex);
}
#endif
/****************************************************************************
 * Function Name  : initPhySocketInterface
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to open a Non Blocking Sockets Rx/Tx
 between the MAC layer and the Physical Layer.
 ****************************************************************************/
#ifndef FLEXRAN
    /* CA Changes start */
void initPhySocketInterface(InternalCellIndex internalCellIndex)
    /* CA Changes end */
{
    LTE_MAC_UT_LOG(LOG_DETAIL,PHY_INTERFACE,"%s Entry.\n",__func__);

    /* CA Changes start */
    initComPhySocketInterface(internalCellIndex);
    /* CA Changes end */

    LTE_MAC_UT_LOG(LOG_INFO,PHY_INTERFACE,"phyControlRxPort_g %d\n", 
            phyControlRxPort_g[internalCellIndex]);

    LTE_MAC_UT_LOG(LOG_INFO,PHY_INTERFACE,"phyFrameIndPort_g%d\n", phyFrameIndPort_g);

    LTE_MAC_UT_LOG(LOG_DETAIL,PHY_INTERFACE,"%s Exit.\n",__func__);
}
#endif	

/* CHG PURIFY Start*/
/****************************************************************************
 * Function Name  : recvDataFromPhysicalByLength
 * Inputs         : msgBuf_p - It points to the message Buffer to be filled
 *                  with received buffer from PHY
 *                  recvLen - Length of message to be read.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : bytesRead - number of bytes read or 0
 * Description    : Function for receiving the data from the Physical Layer .
 ***************************************************************************/
/* CA Changes start */
#ifndef FLEXRAN
SInt32 recvDataFromPhysicalByLength (void* msgBuf_p,UInt32 recvLen,
        InternalCellIndex internalCellIndex)
    /* CA Changes end */
{
    /** CA-TDD Changes Start **/
    static SInt32 bytesRead[MAX_NUM_CELL] = {0}; 
    /** CA-TDD Changes Stop **/
    /*Default maximum size in bytes of a message queue: 16384 bytes.  
      The super-user can increase the size of a message queue beyond MSGMNB by
      a msgctl system call.*/
    RDTSC_AND_CLOCK_START_SOCKET;
    /* CA Changes start*/
    /* SPR 6949 Fix Start */
    /** CA-TDD Changes Start **/
    if ((bytesRead[internalCellIndex] = recvfrom_wrapper(phyRxSockFD_g[internalCellIndex],
                    (UInt8 *)msgBuf_p,recvLen,0,PNULL,0)) == MAC_RECV_ERROR)
        /** CA-TDD Changes Stop **/
        /* CA Changes end */
        /* SPR 6949 Fix End */
    {
        /* CA Stats Changes Start */
#ifndef TDD_CONFIG
        RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex);
#endif
        /* CA Stats Changes End */
        if ( errno != EAGAIN )
        {
            ALARM_MSG (MAC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            /* SPR 7385 Changes Starts */
            /* SPR 7976 */
            lteWarning ("Error in receive message.\n");
            /* SPR 7976 */
            /* SPR 7385 Changes Ends */
        }
        return 0;
    }
#ifndef TDD_CONFIG
    RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex);
#endif
    /** CA-TDD Changes Start **/
    lteMacUpdateStatsPhyRx(msgBuf_p, bytesRead[internalCellIndex], internalCellIndex);

    return bytesRead[internalCellIndex];
    /** CA-TDD Changes Stop **/
}
#endif
#ifdef LAYER2_PHYSIM_THREAD

/****************************************************************************
 * Function Name  : validateMessageWrtDelay
 * Inputs         : systemFrameNum - system frame number and
 *                  subFrameNum - sub frame number
 *                  delay - Downlink Delay
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Function for Validating Message.
 ***************************************************************************/
static  MacRetType validateMessageWrtDelay( UInt32 systemFrameNum, 
        UInt32 subFrameNum,
        UInt8 delay
        )
{ 
    UInt32 curSysFrameNum = 0;
    UInt32 curSubFrameNum = 0;

    /* get current System Number and SubFrame Number */
    /*CA Changes start  */
    getSFAndSFN(&curSysFrameNum, &curSubFrameNum, internalCellIndex);
    /*CA Changes end  */

    if  (systemFrameNum == curSysFrameNum)
    {
        if ((curSubFrameNum - subFrameNum) < delay)
        {
            return MAC_FAILURE;
        }

    }
    {
        if(((10 - subFrameNum) + curSubFrameNum) < delay)
        {
            return MAC_FAILURE;
        }
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : recvDataFromPhySim
 * Inputs         : msgBuf_p - It points to the message Buffer To be filled 
 *                  with buffer received from PHY SIM
 * Outputs        : None
 * Returns        : bytesRead - number of bytes read
 * Description    : Function for receiving the data from the PhySim.
 ***************************************************************************/
SInt32 recvDataFromPhySim (void** msgBuf_p)
{
    SInt32 bytesRead = 0; 
    UInt32 qCount = 0;
    macPhyMsg *ulMessage = PNULL;
    UInt16 sfnsf=0;
    UInt16 sysFrameNum=0;
    UInt8 subFrameNum=0;
    UInt16 snfsf=0;
    QSEGMENT segment;
    UInt8  *msg_p        = PNULL;
    FAPI_subFrameIndication_st *pSubFrameInd;
    qCount = QUEUE_COUNT_L2_PHYSIM_Q (ulL2PhysimDataQ_g);
    if(qCount)
    {
        DEQUEUE_L2_PHYSIM_Q(ulL2PhysimDataQ_g,ulMessage);
        if(ulMessage)
        {
            *msgBuf_p=ulMessage->buffer;
            bytesRead = ulMessage->len;
            freeMemPool(ulMessage);
        }
    }
    else
    {
        *msgBuf_p=PNULL;
    }
    return bytesRead;
}
#endif

/****************************************************************************
 * Function Name  : recvDataFromPhysical
 * Inputs         : msgBuf_p - It points to the message Buffer To be filled 
 *                  with buffer received from PHY 
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : bytesRead - number of bytes read or 0
 * Description    : Function for receiving the data from the Physical Layer.
 ***************************************************************************/
SInt32 recvDataFromPhysical (void* msgBuf_p,InternalCellIndex internalCellIndex)
{
    /** CA-TDD Changes Start **/
    static UInt32 bytesRead[MAX_NUM_CELL] = {0}; 
    /** CA-TDD Changes Stop **/
/* SPR 19288 change start */
#if   FLEXRAN
      RDTSC_AND_CLOCK_START_SOCKET;
      	
	bytesRead[internalCellIndex] = flexran_ipc_recv_msg(internalCellIndex,g_flexran_channelId[internalCellIndex],msgBuf_p,FLEXRAN_MAX_MSG_SIZE);

	if (bytesRead[internalCellIndex] <= 0)
	{
		printf("\n\nRecvd Nothing or ERROR !!! May be problem in ipc rcv from PHY for Channel ID[%d], BytesRead[%d]\n\n",
		       g_flexran_channelId[internalCellIndex],bytesRead[internalCellIndex]);
		return 0;
	}

	switch(g_flexran_channelId[internalCellIndex])
	{
		case FAPI_SHMA_CH_PARA_RSP :
			{
				fprintf(stderr,"RECEIVED PHY_PARAM_RESPONSE\n\n");
				break ;
			}
		case FAPI_SHMA_CH_CFG_RSP :
			{
				fprintf(stderr,"RECEIVED PHY_CELL_CONFIG_RESPONSE\n\n");
				break ;
			}
		case FAPI_SHMA_CH_STOP_IND:
			{
				fprintf(stderr,"RECEIVED PHY_STOP_INDICATION\n\n");
				break ;
			}
		case FAPI_SHMA_CH_ERROR_IND :
			{
				fprintf(stderr,"RECEIVED PHY_ERROR_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_RACH_IND :
			{
				fprintf(stderr,"RECEIVED RACH_IND\n\n");
				break ;
			}
	/****** TBD: Comment all below cases after intial debugging as those may come per tick and proved to be very heavy logging ****/		
		case FAPI_SHMA_CH_SUBFRAME_IND :
			{
				fprintf(stderr,"RECEIVED PHY_SUBFRAME_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_HARQ_IND :
			{
				fprintf(stderr,"RECEIVED PHY_HARQ_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_CRC_IND :
			{
				fprintf(stderr,"RECEIVED PHY_CRC_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_RX_ULSCH_IND :
			{
				fprintf(stderr,"RECEIVED PHY_RX_ULSCH_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_SRS_IND :
			{
				fprintf(stderr,"RECEIVED PHY_SRS_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_RX_SR_IND :
			{
				fprintf(stderr,"RECEIVED PHY_RX_SR_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_RX_CQI_IND :
			{
				fprintf(stderr,"RECEIVED PHY_RX_CQI_IND\n\n");
				break ;
			}
		default :
			{
				//fprintf(stderr, "Msg received on Channel %d, bytes %d\n",g_flexran_channelId,bytesRead);
				break;
			}
	}
	
	return bytesRead[internalCellIndex];
#else
#ifdef LAYER2_PHYSIM_THREAD
    UInt32 qCount = 0;
    /* SPR 3444 changes end */
    macPhyMsg *ulMessage = PNULL;
    /*Default maximum size in bytes of a message queue: 16384 bytes.  
      The super-user can increase the size of a message queue beyond MSGMNB by
      a msgctl system call.*/
    qCount = QUEUE_COUNT_L2_PHYSIM_Q (ulL2PhysimDataQ_g);
    if(qCount)
    {
    DEQUEUE_L2_PHYSIM_Q(ulL2PhysimDataQ_g,ulMessage);
        if(ulMessage->buffer)
        {
            memCpy (msgBuf_p, ulMessage->buffer, ulMessage->len);
            /** CA-TDD Changes Start **/
            bytesRead[internalCellIndex] = ulMessage->len;
            /** CA-TDD Changes Stop **/
            freeMemPool(ulMessage->buffer);
            ulMessage->buffer = PNULL;
        }
        freeMemPool(ulMessage);
        ulMessage = PNULL;
    }
    else
    {
        return 0;
    }
#else
    RDTSC_AND_CLOCK_START_SOCKET;
    /* SPR 6949 Fix Start */
    /*CA Changes start  */
    /** CA-TDD Changes Start **/
    if ((bytesRead[internalCellIndex] = recvfrom_wrapper(phyRxSockFD_g[internalCellIndex],
          (UInt8 *)msgBuf_p,SH_MESSAGE_UL_BUF_SIZE,0,PNULL,0)) == MAC_RECV_ERROR)
    /** CA-TDD Changes Stop **/
    /*CA Changes end  */
    /* SPR 6949 Fix End */
    {
    /* CA Stats Changes Start */
#ifndef TDD_CONFIG
    RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex);
#endif
    /* CA Stats Changes End */
        if ( errno != EAGAIN )
        {
            ALARM_MSG (MAC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            /* SPR 7385 Changes Starts */
            /* SPR 7976 */
            pError("recvfrom");
            ltePanic ("Error in receive message.\n");
            /* SPR 7976 */
            /* SPR 7385 Changes Ends */
        }
        return 0;
    }
#ifndef TDD_CONFIG
    RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex);
#endif
    /** CA-TDD Changes Start **/
    lteMacUpdateStatsPhyRx(msgBuf_p, bytesRead[internalCellIndex], internalCellIndex);
#endif
#endif
    /* SPR 19288 change end */
    /*SPR 18187 workaroud fix end*/    
    return bytesRead[internalCellIndex];
    /** CA-TDD Changes Stop **/
}
/* SPR 19288 change start */
/* SPR 19288 change end */


#ifdef DL_UL_SPLIT_TDD
/****************************************************************************
 * Function Name  : recvUlDataFromPhysical
 * Inputs         : msgBuf_p - Its point to the message Buffer 
 * Outputs        : 
 * Returns        : Number of Bytes Recieved. 
 * Description    : UL thread Function for receiving the data from the 
 *                  Physical Layer. For DL thread recvDataFromPhysical is 
 *                  used 
 ***************************************************************************/
SInt32 recvUlDataFromPhysical (void* msgBuf_p)
{
    static SInt32 bytesRead = 0; 
    /*Default maximum size in bytes of a message queue: 16384 bytes.  
      The super-user can increase the size of a message queue beyond MSGMNB by
      a msgctl system call.*/
    RDTSC_AND_CLOCK_START_SOCKET;
    if ((bytesRead = recvfrom_wrapper(phyRxSockUlFD_g,(UInt8 *)msgBuf_p,SH_MESSAGE_UL_BUF_SIZE,0,PNULL,0)) == MAC_RECV_ERROR)
    {
        RDTSC_AND_CLOCK_END_SOCKET;
        if ( errno != EAGAIN )
        {
            ALARM_MSG (MAC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            pError("recvfrom");
            ltePanic ("Error in receive message.\n");
        }
        return 0;
    }
    RDTSC_AND_CLOCK_END_SOCKET;
    lteMacUpdateUlStatsPhyRx(msgBuf_p, bytesRead);

    return bytesRead;
}
#endif


/****************************************************************************
 * Function Name  : recvControlInfoFromPhysical
 * Inputs         : msgBuf_p - It points to the message Buffer To be filled 
 *                  with buffer received from PHY 
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : bytesRead - number of bytes read or 0
 * Description    : Function for receiving the Control Info from the Physical Layer.
 ***************************************************************************/
#if   FLEXRAN 
SInt32 recvControlInfoFromPhysical(void* msgBuf_p,InternalCellIndex internalCellIndex)
{
	SInt32 bytesRead = 0;
	//SInt32 bytesSend = 0;
	
	bytesRead = flexran_ipc_recv_msg(internalCellIndex,g_flexran_channelId[internalCellIndex],msgBuf_p,FLEXRAN_MAX_MSG_SIZE);

	if (bytesRead <= 0)
	{
		printf("\n\nRecvd Nothing or ERROR !!! May be problem in ipc rcv from PHY for Channel ID[%d], BytesRead[%d]\n\n",
		       g_flexran_channelId[internalCellIndex],bytesRead);
		return 0;
	}

	switch(g_flexran_channelId[internalCellIndex])
	{
		case FAPI_SHMA_CH_PARA_RSP :
			{
				fprintf(stderr,"RECEIVED PHY_PARAM_RESPONSE\n\n");
				break ;
			}
		case FAPI_SHMA_CH_CFG_RSP :
			{
				fprintf(stderr,"RECEIVED PHY_CELL_CONFIG_RESPONSE\n\n");
				break ;
			}
		case FAPI_SHMA_CH_STOP_IND:
			{
				fprintf(stderr,"RECEIVED PHY_STOP_INDICATION\n\n");
				break ;
			}
		case FAPI_SHMA_CH_ERROR_IND :
			{
				fprintf(stderr,"RECEIVED PHY_ERROR_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_RACH_IND :
			{
				fprintf(stderr,"RECEIVED RACH_IND\n\n");
				break ;
			}
	/****** TBD: Comment all below cases after intial debugging as those may come per tick and proved to be very heavy logging ****/		
		case FAPI_SHMA_CH_SUBFRAME_IND :
			{
				fprintf(stderr,"RECEIVED PHY_SUBFRAME_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_HARQ_IND :
			{
				fprintf(stderr,"RECEIVED PHY_HARQ_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_CRC_IND :
			{
				fprintf(stderr,"RECEIVED PHY_CRC_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_RX_ULSCH_IND :
			{
				fprintf(stderr,"RECEIVED PHY_RX_ULSCH_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_SRS_IND :
			{
				fprintf(stderr,"RECEIVED PHY_SRS_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_RX_SR_IND :
			{
				fprintf(stderr,"RECEIVED PHY_RX_SR_IND\n\n");
				break ;
			}
		case FAPI_SHMA_CH_RX_CQI_IND :
			{
				fprintf(stderr,"RECEIVED PHY_RX_CQI_IND\n\n");
				break ;
			}
		default :
			{
				//fprintf(stderr, "Msg received on Channel %d, bytes %d\n",g_flexran_channelId,bytesRead);
				break;
			}
	}

	return bytesRead;
}
#else
SInt32 recvControlInfoFromPhysical(void* msgBuf_p,InternalCellIndex internalCellIndex)
{

/* SPR 19288 change start */
    SInt32 bytesRead = 0;

#ifdef LAYER2_PHYSIM_THREAD
    macPhyMsg *ulMessage = PNULL;
#else
#endif
#ifdef LAYER2_PHYSIM_THREAD
    semPost(&phySimReceiveSem_g);
if (QUEUE_COUNT_L2_PHYSIM_Q (ulL2PhysimControlQ_g))
  {
   DEQUEUE_L2_PHYSIM_Q (ulL2PhysimControlQ_g, ulMessage);
    memCpy (msgBuf_p, ulMessage->buffer, ulMessage->len);
    bytesRead = ulMessage->len;
    freeMemPool (ulMessage->buffer);
    ulMessage->buffer = PNULL;
    freeMemPool (ulMessage);
    ulMessage = PNULL;
  }
#else

    /* spr 1768 changes start */   
    /* SPR 6949 Fix Start */
    /* CA Changes start*/
    if ((bytesRead = recvfrom_wrapper(phyControlSockFD_g[internalCellIndex],
         (UInt8 *)msgBuf_p,FAPI_CTRL_MSG_MAX_BUF_SZ,0,PNULL,0)) == MAC_RECV_ERROR)
    /* CA Changes end*/
    /* SPR 6949 Fix End */
    /* spr 1768 changes end */ 
    {
        if ( errno != EAGAIN )
        {
            ALARM_MSG (MAC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            /* SPR 7385 Changes Starts */
            /* SPR 7976 */
            lteWarning ("Error in receive message.\n");
            /* SPR 7976 */
            /* SPR 7385 Changes Ends */
        }
        return 0;
    }
#endif
   /*-------SPR 2066-------*/
    /* CA Stats Changes Start */
     lteMacUpdateStatsPhyRx(msgBuf_p, bytesRead, internalCellIndex);
    /* CA Stats Changes End */
    /*-------SPR 2066-------*/
/*SPR 21001 Changes end */
    return bytesRead;
}
#endif	

/****************************************************************************
 * Function Name  : recvFrameIndFromPhysical
 * Inputs         : msgBuf_p - It points to the message Buffer To be filled 
 *                  with buffer received from PHY 
 * Outputs        : None
 * Returns        : bytesRead - number of bytes read or 0
 * Description    : Function for receiving the frame from the Physical Layer.
 ***************************************************************************/
SInt32 recvFrameIndFromPhysical(void* msgBuf_p)
{
    SInt32 bytesRead = 0; 
    /* SPR 6949 Fix Start */
    if ((bytesRead = recvfrom_wrapper(phyFrameIndSockFD_g,(UInt8 *)msgBuf_p,SH_MESSAGE_UL_BUF_SIZE,0,PNULL,0)) == MAC_RECV_ERROR)
    /* SPR 6949 Fix End */
    {
        if ( errno != EAGAIN )
        {
            ALARM_MSG (MAC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            LOG_MAC_MSG(L2_SOCKET_RECVFROM_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                    getCurrentTick(),\
                    __LINE__,0,0,0,0,\
                    0.0,0.0,__FILE__,__func__);

            ltePanic ("Error in receive message.\n");
        }
        return 0;
    }
    return bytesRead;
}

/****************************************************************************
 * Function Name  : recvDataFromPhysicalNonBlocking
 * Inputs         : msgBuf_p - It points to the message Buffer To be filled 
 *                  with buffer received from PHY 
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : bytesRead - number of Bytes Recieved or 0
 * Description    : Function for receiving the data from the Physical Layer
 *                  in Non-Blocking mode.
 ***************************************************************************/
SInt32 recvDataFromPhysicalNonBlocking (void* msgBuf_p, InternalCellIndex internalCellIndex)
{
    /** CA-TDD Changes Start **/
    static SInt32 bytesRead[MAX_NUM_CELL] = {0};
#if   FLEXRAN
 //Nothing to do for FLEXRAN
#else
    /** CA-TDD Changes Stop **/
    /*Default maximum size in bytes of a message queue: 16384 bytes.
      The super-user can increase the size of a message queue beyond MSGMNB by
      a msgctl system call.*/
    /* SPR 6949 Fix Start */
    /** CA-TDD Changes Start **/
    if ((bytesRead[internalCellIndex] = recvfrom_wrapper(phyRxSockFD_g[internalCellIndex],(UInt8 *)msgBuf_p,
                       SH_MESSAGE_UL_BUF_SIZE,0,PNULL,0)) == MAC_RECV_ERROR)
    /** CA-TDD Changes Stop **/
    /* SPR 6949 Fix Start */
    {
        /* socket is nonblocking in this case the value -1 is returned
           and the external variable errno set to EAGAIN.*/
        if ( errno != EAGAIN )
        {
            ALARM_MSG (MAC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            ltePanic ("Error in receive message.\n");
        }

        /*Fix for cpu hogging */
#ifdef FDD_CONFIG
        if (CELL_WAIT_FOR_PHY_START >= cellSpecificParams_g.\
                cellConfigAndInitParams_p[internalCellIndex]->cellState)
        {
            /* +COVERITY 10008 */
            fcntl_f_setfl(phyRxSockFD_g[internalCellIndex]);
            /* -COVERITY 10008 */
        }
#endif
        return 0;
    }
/*SPR 21001 Changes end */
#endif
   /** CA-TDD Changes Start **/
   return bytesRead[internalCellIndex];
   /** CA-TDD Changes Stop **/
}

/****************************************************************************
 * Function Name  : sendDataToPhysical
 * Inputs         : bufferToSend - The buffer stream that needs to be 
 *                  transmitted to the physical layer simulator.
 *                  bufferLen    - Length in bytes of the message to be sent
 *                  on socket.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : bufferLen - The number of bytes sent on socket.
 * Description    : This function is used to send the message to the physical
 *                  layer.
 *                  (For testing purposes in phase 1) - The function calls 
 *                  another method sendMsgBySocket() to send the message over
 *                  socket to the physical layer simulator. 
 ****************************************************************************/
SInt32 sendDataToPhysical (UInt8 *bufferToSend_p, UInt32 bufferLen,
                           InternalCellIndex internalCellId)
{
#ifdef LAYER2_PHYSIM_THREAD
    macPhyMsg *dlMessage=PNULL;
#endif
    //This is for testing purposes where the message is send over socket to 
    //physical layer on port 5555. This needs to be replaced later by message
    //queue/shared memory interface.
#ifdef LAYER2_PHYSIM_THREAD
    dlMessage = (macPhyMsg *)getMemFromPool(sizeof(macPhyMsg),PNULL);
    if ( PNULL == dlMessage )
    {
        ltePanic("memory allocation failed for dlMessage %s",__func__);
    }
    dlMessage->len = bufferLen;
    dlMessage->buffer = bufferToSend_p;

    ENQUEUE_L2_PHYSIM_Q(dlL2PhysimDataQ_g,dlMessage); 
    eventfd_write_wrapper(macDataEventFd_g, 1);
    return bufferLen;
#else
    /* CA Stats Changes Start */
    lteMacUpdateStatsPhyTx(bufferToSend_p, bufferLen, internalCellId);
    /* CA Stats Changes End */
#ifdef FLEXRAN_FAPI_LOGGING
    if(0 != flexranFapiLoggingEnable_g)
    {
	    do_tap_fun(bufferToSend_p,bufferLen, internalCellId);
    }
#endif

    return sendMsgBySocket(bufferToSend_p,bufferLen,internalCellId);
#endif
}

/****************************************************************************
 * Function Name  : sendMsgBySocket
 * Inputs         : bufferToSend_p - Pointer to the buffer stream to be sent
 *                                   over socket on port 5555 and
 *                  bufferLen    - Length in bytes of the message to be sent
 *                                 on socket
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : bytesSend - The number of bytes sent on socket.
 * Description    : Function for sending the message over socket to the 
 *                  physical layer on port 5555.
 ****************************************************************************/
STATIC SInt32 sendMsgBySocket(UInt8 *bufferToSend_p,UInt32 bufferLen,
              InternalCellIndex internalCellIndex)
/*SPR 21001 Changes endif*/
{
    /** CA-TDD Changes Start **/
    static SInt32 bytesSend[MAX_NUM_CELL] = {0};
    /** CA-TDD Changes Stop **/
        RDTSC_AND_CLOCK_START_SOCKET; 
/* SPR 19288 change start */
#if   FLEXRAN

	bytesSend[internalCellIndex] = flexran_ipc_send_msg(internalCellIndex, 
			(FAPI_SHMA_CH_T)g_flexran_channelId[internalCellIndex], 
			(char*)bufferToSend_p, 
			bufferLen);
	if (bytesSend[internalCellIndex] < 0)
	{
		printf("WARNING !!! flexran_ipc_send_msg fails for Channel [%d] !; %s:%d \n", 
				g_flexran_channelId[internalCellIndex],
				__func__,
				__LINE__);

		bytesSend[internalCellIndex]  =0;
	}
#else 
    /** CA-TDD Changes Start **/
    bytesSend[internalCellIndex] = sendto_wrapper(phyTxSockFD_g[internalCellIndex],(UInt8 *)bufferToSend_p,bufferLen,0,
            (const SockAddr *)&phyTxAddr_g[internalCellIndex],sizeof (SockAddr));
    /** CA-TDD Changes Stop **/
    /* CA Stats Changes Start */
#ifndef TDD_CONFIG
    RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex);    
#endif
    /* CA Stats Changes End */
    /** CA-TDD Changes Start **/
    if ( bytesSend[internalCellIndex] == MAC_SEND_ERROR)
    /** CA-TDD Changes Stop **/
    {
        /* In non-blocking mode it would return EAGAIN */
        if ( errno != EAGAIN )
        {
            ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            /* SPR 7385 Changes Starts */
            /* SPR 7976 */
            lteWarning("Sending DL Msg Error (socket)");
            /* SPR 7976 */
            /* SPR 7385 Changes Starts */
        }
    /** CA-TDD Changes Start **/
        bytesSend[internalCellIndex] = 0;


    }
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
    return bytesSend[internalCellIndex];
    /** CA-TDD Changes Stop **/
}
/****************************************************************************
 * Function Name  : sendControlInfoToPhy
 * Inputs         : bufferToSend_p - Pointer to the buffer stream to be sent
 *                                   over socket on port 5555 and
 *                  bufferLen    - Length in bytes of the message to be sent
 *                                 on socket
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : bytesSend - The number of bytes sent on socket or NULL pointer.
 * Description    : Function for sending the message over socket to the 
 *                  physical layer on port 5555.
 ****************************************************************************/
/* CA Changes start */
SInt32 sendControlInfoToPhy(UInt8 *bufferToSend_p,UInt32 bufferLen, 
		           InternalCellIndex internalCellIndex)
/* CA Changes end */
{
#ifdef LAYER2_PHYSIM_THREAD
    macPhyMsg *dlMessage=PNULL;
#endif
#ifdef LAYER2_PHYSIM_THREAD
    dlMessage = (macPhyMsg *)getMemFromPool(sizeof(macPhyMsg),PNULL);
    if ( PNULL == dlMessage )
    {
        ltePanic("memory allocation failed for dlMessage %s",__func__);
    }
    dlMessage->len = bufferLen;
    dlMessage->buffer = (UInt8 *) getMemFromPool(bufferLen,PNULL);
     memCpy (dlMessage->buffer,bufferToSend_p,bufferLen);
    ENQUEUE_L2_PHYSIM_Q(dlL2PhysimControlQ_g,dlMessage); 

    eventfd_write_wrapper(macControlEventFd_g, 1);
    return bufferLen;
#else
  SInt32 bytesSend = 0;
/* SPR 19288 change start */
#if   FLEXRAN

	bytesSend = flexran_ipc_send_msg(internalCellIndex, 
			(FAPI_SHMA_CH_T)g_flexran_channelId[internalCellIndex], 
			(char*)bufferToSend_p, 
			bufferLen);
	if (bytesSend < 0)
	{
		fprintf(stderr,"WARNING !!! flexran_ipc_send_msg fails for Channel [%d] !; %s:%d \n", 
				g_flexran_channelId[internalCellIndex],
				__func__,
				__LINE__);

		bytesSend =0;
	}
	else
	{
#ifdef FLEXRAN_FAPI_LOGGING
		if(0 != flexranFapiLoggingEnable_g)
		{
			do_tap_fun(bufferToSend_p,bufferLen, internalCellIndex);
		}
#endif

		// Send NOTIFY to PHY immediately after sending any control message.
		flexran_ipc_notify_phy_ready(internalCellIndex);

		switch(g_flexran_channelId[internalCellIndex])
		{
			case FAPI_SHMA_CH_PARA_REQ :
				printf("SENT PHY_PARAM_REQUEST to PHY on channel [%d]\n\n",g_flexran_channelId[internalCellIndex]);
				break ;

			case FAPI_SHMA_CH_CFG_REQ:
				printf("SENT PHY_CELL_CONFIG_REQUEST to PHY on channel [%d]\n\n",g_flexran_channelId[internalCellIndex]);;
				break ;

			case FAPI_SHMA_CH_START_REQ:
				printf("\n\nSENT PHY_START_REQUEST to PHY on channel [%d]\n\n",g_flexran_channelId[internalCellIndex]);
				break ;

			case FAPI_SHMA_CH_STOP_REQ:
				printf("\n\nSENT PHY_STOP_REQUEST to PHY on channel [%d]\n\n",g_flexran_channelId[internalCellIndex]);
				break ;

			default:
				printf("\n\nSENT Unknown message to PHY on channel[%d]\n\n",g_flexran_channelId[internalCellIndex]);
				break ;


		}
	}
#else

  /* CA Changes start */
  phyControlTxAddr_g[internalCellIndex].sin_port = htons_wrapper(phyControlTxPort_g[internalCellIndex]);
  /* CA Changes end */
  RDTSC_AND_CLOCK_START_SOCKET;
  /*bytesSend = sendto_wrapper(phyControlSockFD_g,(UInt8 *)bufferToSend_p,bufferLen,0,
      (const SockAddr *)&phyControlAddr_g,sizeof (phyControlAddr_g));*/
  /* CA Changes start */
  bytesSend = sendto_wrapper(phyControlSockFD_g[internalCellIndex],
                   (UInt8 *)bufferToSend_p,bufferLen,0,
      (const SockAddr *)&phyControlTxAddr_g[internalCellIndex],
    sizeof (phyControlTxAddr_g[internalCellIndex]));
  /* CA Changes end */
#ifndef TDD_CONFIG
  RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex);
#endif
  if ( bytesSend == MAC_SEND_ERROR)
  {
      /* In non-blocking mode it would return EAGAIN */
      if ( errno != EAGAIN )
      {
          ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
          /* SPR 7385 Changes Starts */
          /* SPR 7976 */
          lteWarning("Sending DL Msg Error (socket)");
          /* SPR 7976 */
          /* SPR 7385 Changes Starts */
      }
      bytesSend = 0;
  }
  /*---SPR 2066  FAPI stats----*/
  /* CA Stats Changes Start */
  lteMacUpdateStatsPhyTx(bufferToSend_p, bufferLen, internalCellIndex);
  /*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
  /* CA Stats Changes End */
  /*---SPR 2066  FAPI stats----*/
  return bytesSend;
#endif
}


/****************************************************************************
 * Function Name  : receiveDataFromPhy
 * Inputs         : bytesRead_output,internalCellIndex
 * Outputs        : None
 * Returns        : PNULL
 * Description    : Function for receiving the data from the Physical Layer.
 ***************************************************************************/
UInt8 * receiveDataFromPhy(SInt32 *bytesRead_output,InternalCellIndex internalCellIndex)
{
        SInt32 bytesRead     = 0;
        UInt8 *msgBuf_p = PNULL;
#ifndef LAYER2_PHYSIM_THREAD
        UInt8 *msg_p        = PNULL;
#endif

#ifndef LAYER2_PHYSIM_THREAD
        msgBuf_p = msgAlloc(PNULL,0,0,0);
        /* + coverity 31401 */
        if( PNULL == msgBuf_p )
        {
            return PNULL;
        }
        /* - coverity 31401 */
        msg_p = msgReserve(msgBuf_p,0,BUFFLEN);
        if (PNULL == msg_p )
        {
            /* + coverity 31401 */
            msgFree( msgBuf_p );
            /* - coverity 31401 */
            lteWarning("Error In msgSegNext receiveDataFromPhy\n");
            return PNULL;
        }
#endif

#ifndef LAYER2_PHYSIM_THREAD
        /*CA Changes start  */
        bytesRead = recvDataFromPhysical (msg_p, internalCellIndex);
        /*CA Changes end  */
#else
        bytesRead = recvDataFromPhySim (&msgBuf_p);
#endif

        *bytesRead_output = bytesRead;
        return msgBuf_p;
}


/****************************************************************************
 * Function Name  : cleanupPHYInterface
 * Inputs         : internalCellIndex
 * Outputs        : It Frees the Memory
 * Returns        : None
 * Description    : This function is used to free the memory used by 
 *                  PHY Receiver and PHY Transmitter queue, list etc.
 ****************************************************************************/
/*CA Changes start  */
void cleanupPHYInterface(InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
#if defined DL_UL_SPLIT && defined CIRC_QUEUE_IMPL
	LTE_CNODE *node_p = PNULL;
#else
	LTE_SNODE *node_p = PNULL;
#endif
	LTE_SNODE *nodeToFree_p = PNULL;
	/* +GCC 4.7 */
	/* -GCC 4.7 */
	/****** Reciever Related Queue***************************/ 
 #ifdef FDD_CONFIG
    /* Check Downlink Ack Nack Queue */
#ifdef DL_UL_SPLIT
#ifdef CIRC_QUEUE_IMPL
    /* +COVERITY 20870 */
    /*CA Changes start  */
    while((DEQUEUE_ACK_NACK_CIRC_Q(AckNackQueueNode, (void**)&node_p, internalCellIndex)))
    /* -COVERITY 20870 */
    {
        FREE_MEM_NODE_ACK_NACK_CIRC_Q(node_p);
    }   
#else
    closeIccQ(ackNackQueueIdUL_g[internalCellIndex]);
    closeIccQ(ackNackQueueIdDL_g[internalCellIndex]);
#endif
#else
    while((node_p = popNode(ackNackQueue_gp[internalCellIndex]))) 
    {
        freeMemPool(node_p);
        node_p = PNULL;
    }   
#endif
/*
#ifndef DL_UL_SPLIT
    while((node_p = popNode(ackNackQueue_gp[internalCellIndex]))) {
        freeMemPool(node_p);
        node_p = PNULL;
    }   
#else
    closeIccQ(ackNackQueueIdUL_g[internalCellIndex]);
    closeIccQ(ackNackQueueIdDL_g[internalCellIndex]);
#endif
    */
#elif TDD_CONFIG
    /* Check Downlink Ack Nack Queue */
     /** CA-TDD Changes Start:06 **/
    while((node_p = popNode(&tddAckNackQueue_g[internalCellIndex]))) {
    /** CA-TDD Changes End:06 **/
        freeMemPool(node_p);
        node_p = PNULL;
    }   

#endif
    /* Check CQI Report Queue */
    /*CA Changes start  */
    DEQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo, (void**)&node_p, internalCellIndex);
    /*CA Changes end  */
    while(node_p)
    {
        FREE_MEM_NODE_DL_SCHEDULER_REPORT_QUEUE_Q(node_p);
        /*CA Changes start  */
        DEQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(UEReportedCQINodeInfo, (void**)&node_p, internalCellIndex);
        /*CA Changes end  */
    }   

    /* Check globalSrQueue_g */
    /*CA Changes start  */
    while((nodeToFree_p = popNode(ulGrantGlobalSrQueue_gp[internalCellIndex]))) {
    /*CA Changes end  */
        freeMemPool(nodeToFree_p);
        nodeToFree_p = PNULL;
    }   
    /* Check timing Advance Node */
    while(1){
        /*CA Changes start  */
        DEQUEUE_TA_Q(dlTimeAdvanceQueue_gp[internalCellIndex],LTE_CNODE ,(void **)&node_p);
        /*CA Changes end  */
        if(node_p)
        {
            FREE_MEM_NODE_TA_Q(node_p);
        }
        else
        {
            break;
        }
    }

    /* Closing receive socket */
        /* CA Changes start */
#ifndef FLEXRAN
    if (phyRxSockFD_g[internalCellIndex] > 0) {
        LTE_MAC_UT_LOG(LOG_DETAIL,PHY_INTERFACE, 
                 "PHY Receive socket opened, closing it now\n");
        /*Coverity 95473 Fix Start*/
        if(SOCKET_ERROR == close_wrapper(phyRxSockFD_g[internalCellIndex]))
        {
            LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                    getCurrentTick(),\
                    __LINE__,0,0,0,0,\
                    0.0,0.0,__FILE__,__func__);
        }
        /*Coverity 95473 Fix End*/
        phyRxSockFD_g[internalCellIndex] = 0;
    }

    /* Closing Transmit socket */
    if (phyTxSockFD_g[internalCellIndex] > 0) {
        LTE_MAC_UT_LOG(LOG_DETAIL,PHY_INTERFACE,
              "PHY Transmit socket opened, closing it now\n");
        /*Coverity 95473 Fix Start*/
        if(SOCKET_ERROR == close_wrapper(phyTxSockFD_g[internalCellIndex]))
        {
            LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                    getCurrentTick(),\
                    __LINE__,0,0,0,0,\
                    0.0,0.0,__FILE__,__func__);
        }
        /*Coverity 95473 Fix End*/
        phyTxSockFD_g[internalCellIndex] = 0;
    }
#endif
        /* CA Changes end */

}
/* SPR 19288 change start */
/* SPR 19288 change end */

/*coverity changes CID-54611*/
/****************************************************************************
 * Function Name  : deInitPHYInterface
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to deInit the CirQueues  used by 
 *                  PHY Receiver and PHY Transmitter queue, list etc.
 ****************************************************************************/
void deInitPHYInterface(InternalCellIndex internalCellIndex)
{
#ifdef DL_UL_SPLIT
#ifdef CIRC_QUEUE_IMPL
    QDEINIT_NODE_ACK_NACK_CIRC_Q(internalCellIndex);    
#endif    
#endif  
    /* CA Changes start */
    deinitCQIMgr(internalCellIndex);  
    /* CA Changes end */
    /*CA Changes start  */
    QDEINIT_NODE_TA_Q(dlTimeAdvanceQueue_gp[internalCellIndex]);
    /*CA Changes end  */
}
/*coverity changes CID-54611*/
/****************************************************************************
 * Function Name  : initMemForSubFrameInd
 * Inputs         : numCellsConfigured
 * Outputs        : None
 * Returns        : None
 * Description    : This function is allocates and initialise the memory for 
 *                  SUBFRAME Ind which will be pushed into l1C Queue.
 ****************************************************************************/
