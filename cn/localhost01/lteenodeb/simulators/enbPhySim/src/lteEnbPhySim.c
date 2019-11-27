 /***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteEnbPhySim.c,v 1.10.6.2.4.2.2.2 2011/12/22 06:30:30 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the Interface between the
 *                     MAC and Phy Simulator Layer in a LTE system. Also
 *                     contain the Interface between Phy Simulator & UeSim.
 *
 ****************************************************************************
 *
 * Revision Details :
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <time.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/eventfd.h>



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMisc.h"
#include "lteLog.h"
#include "mempool.h"
#include "lteLayer2CommanTypes.h"
#include "lteLayer2Types.h"
#include "lteEnbPhySim.h"
#include "lteFapiMacPhyInterface.h"
#include "lteFapiControlMsgHandler.h"
#include "lteMsgPool.h"

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/




/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/* variable for IP Address */
UChar8 l2ModuleIP[HOSTNAME_MAX_LEN];
UChar8 ueSimIPAddress_g[HOSTNAME_MAX_LEN];

UInt32 ltePhySimCoreNum_g;
SInt32 txUESimCtrlSocketFD_g = -1;
SInt32 rxUESimCtrlSocketFD_g = -1;
SInt32 macDataEventFd_g = -1;
SInt32 macPhyEventFd_g = -1;
//SInt32 tickGenFd_g = -1;
SInt32 macControlEventFd_g = -1;
//SInt32 tickGenFd_g = -1;
/* +CA_NA */
SInt32 txEnodebRRCSocketFD_g[MAX_NUM_CELLS] = {-1, -1};
SInt32 rxEnodebRRCSocketFD_g[MAX_NUM_CELLS] = {-1, -1};
SockAddr txEnodebRRC_addr_g[MAX_NUM_CELLS];
SockAddr rxEnodebRRC_addr_g[MAX_NUM_CELLS];
/* -CA_NA */
SInt32 enbPhySimRxSockfd_g = -1;
SInt32 enbPhySimTxSockfd_g = -1;
SInt32 rxUeSimSockFD_g = -1;
SInt32 txUeSimSockFD_g = -1;
SockAddr txMacLayerAddr_g;
SockAddr rxMacLayerAddr_g;
SockAddr macLayer_g;
SockAddr txUeSim_addr_g;
SockAddr rxUeSim_addr_g;
SockAddr txUESimCtrl_addr_g;
SockAddr rxUESimCtrl_addr_g;
UInt8 recvDataBuffer_g[RECV_BUFF_SIZE];
volatile UInt16 SFN_g  = MAX_SFN - 1;
volatile UInt8  SF_g   = MAX_SUBFRAME - 2;
/*SPR 15909 FIx Start*/
tickType_t tickValue_g = TICK_IN_MS * 1000000;
/*SPR 15909 FIx end*/
LTE_SQUEUE macPhyDataQ_g[MAX_SUBFRAME];
ULong32 enbPhySimExecThread_g;
ULong32 enbPhySimTickGenThread_g;
UInt16   sfnsf_g = 0;
UInt8 startThread_g = 0;
pthread_mutex_t exec_mutex;
pthread_cond_t exec_cv;

#ifdef LAYER2_PHYSIM_THREAD
#define PHYSIM_CORE_NUM 0
LTE_CIRCQUEUE dlL2PhysimDataQ_g ;
LTE_CIRCQUEUE dlL2PhysimControlQ_g ;
LTE_CIRCQUEUE ulL2PhysimDataQ_g ;
LTE_CIRCQUEUE ulL2PhysimControlQ_g ;
struct timespec timeStampPrevSFI;
LTE_SEM layer2ReceiveSem_g;
LTE_SEM phySimReceiveSem_g;
LTE_SEM tickGenSem_g;
#define BUFFLEN             20000
#endif
/*****************************************************************************
 * Function Name  : sendDataToUe
 * Inputs         : None
 * Outputs        : None
 * Returns        :
 * Description    : This function send data to Ue Sim Phy
 *****************************************************************************/
void sendDataToUe(UInt8 *buffer_p, SInt32 msgLen)
{
    SInt32 bytesSend = 0;

    bytesSend = sendto(txUeSimSockFD_g,(unsigned char *)buffer_p,msgLen,\
                           0,(const struct sockaddr *)&txUeSim_addr_g, sizeof (txUeSim_addr_g));
    if (bytesSend < 0)
    {
        ltePanic("Error in sending data on socket:: errno=%d",errno);
    }

}
/*****************************************************************************
 * Function Name  : sendSFIToEnbMac
 * Inputs         : None
 * Outputs        : None
 * Returns        :
 * Description    : This function send data to eNB MAC
 *****************************************************************************/
void sendSFIToEnbMac(UInt8 *buffer_p, SInt32 msgLen)
{
    SInt32 bytesSend = 0;
    macPhyMsg *ulMessage=PNULL;
#ifdef LAYER2_PHYSIM_THREAD
    ulMessage = (macPhyMsg *)getMemFromPool(sizeof(macPhyMsg),PNULL);
    if ( PNULL == ulMessage )
    {
        ltePanic("memory allocation failed for ulMessage %s",__func__);
    }
    ulMessage->len=msgLen;

        ulMessage->buffer = buffer_p;
    ENQUEUE_L2_PHYSIM_Q(ulL2PhysimControlQ_g,ulMessage);

#else
    bytesSend = sendto(enbPhySimTxSockfd_g,(unsigned char *)buffer_p,msgLen,\
            0,(const struct sockaddr *)&txMacLayerAddr_g, sizeof (txMacLayerAddr_g));
    if (bytesSend < 0)
    {
        ltePanic("Error in sending data on socket:: errno=%d",errno);
    }
    freeMemPool(buffer_p);
#endif

}

/*****************************************************************************
 * Function Name  : sendDataToPeerMac
 * Inputs         : None
 * Outputs        : None
 * Returns        :
 * Description    : This function send data to eNB MAC
 *****************************************************************************/
void sendDataToPeerMac(UInt8 *buffer_p, SInt32 msgLen)
{
    SInt32 bytesSend = 0;
    macPhyMsg *ulMessage=PNULL;
#ifdef LAYER2_PHYSIM_THREAD
    ulMessage = (macPhyMsg *)getMemFromPool(sizeof(macPhyMsg),PNULL);
    if ( PNULL == ulMessage )
    {
        ltePanic("memory allocation failed for ulMessage %s",__func__);
    }
    ulMessage->len=msgLen;

    ulMessage->buffer = buffer_p;
    ENQUEUE_L2_PHYSIM_Q(ulL2PhysimDataQ_g,ulMessage);

#else
    bytesSend = sendto(enbPhySimTxSockfd_g,(unsigned char *)buffer_p,msgLen,\
            0,(const struct sockaddr *)&txMacLayerAddr_g, sizeof (txMacLayerAddr_g));
    if (bytesSend < 0)
    {
        ltePanic("Error in sending data on socket:: errno=%d",errno);
    }
    freeMemPool(buffer_p);
#endif

}
/*****************************************************************************
 * Function Name  : sendControlToEnbMac
 * Inputs         : None
 * Outputs        : None
 * Returns        :
 * Description    : This function send control to eNB MAC
 *****************************************************************************/

#ifdef LAYER2_PHYSIM_THREAD
void sendControlToEnbMac(UInt8 *buffer_p, UInt32 msgLen)
{
    macPhyMsg *ulMessage=PNULL;
    ulMessage = (macPhyMsg *)getMemFromPool(sizeof(macPhyMsg),PNULL);
    if ( PNULL == ulMessage )
    {
        ltePanic("memory allocation failed for ulMessage %s",__func__);
    }
    ulMessage->len=msgLen;
    ulMessage->buffer= (void *) getMemFromPool(msgLen,PNULL);
    memcpy (ulMessage->buffer, buffer_p, msgLen);
    ENQUEUE_L2_PHYSIM_Q(ulL2PhysimControlQ_g,ulMessage);

}
#endif
/****************************************************************************
 * Function Name  : sendSFI
 * Inputs         : NONE 
 * Outputs        : NONE
 * Returns        : 
 * Variables      :
 * Description    : to send SFI to l2 & UeSim
 ****************************************************************************/

FAPI_subFrameIndication_st *sfInd_g;
FAPI_l1ApiMsg_st sfIndMessage_g;
void sendSFI()
{
	UInt8                      *sendBuffer_p = PNULL;
	FAPI_l1ApiMsg_st           *l1ApiMsg_p = PNULL;
	FAPI_subFrameIndication_st *sfInd_p = PNULL;
	UInt32                     l1ApiMsgSize = 0;
	UInt32                     l1ApiMsgBodySize = 0;

	l1ApiMsgBodySize = FAPI_SUBFRAME_INDICATION_MSG_BODY_SIZE;
	l1ApiMsgSize = L1_PARTIAL_API_MSG_SIZE + l1ApiMsgBodySize;

	sendBuffer_p = (UInt8 *) getMemFromPool( l1ApiMsgSize, PNULL );
	if ( PNULL == sendBuffer_p )
	{
		ltePanic(" Error!! in fapiSendSubFrameStartIndication allocation"
				"of buffer sendBuffer_p \n");
		return;
	}

	l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) sendBuffer_p;

	l1ApiMsg_p->msgId = PHY_UL_SUBFRAME_INDICATION;
	l1ApiMsg_p->lenVendorSpecific = 0;
	l1ApiMsg_p->msgLen = l1ApiMsgBodySize;

	sfInd_p = ( FAPI_subFrameIndication_st *) &(l1ApiMsg_p->msgBody[0]);
	sfInd_p->sfnsf = sfnsf_g;
	sendDataToUe( sendBuffer_p, l1ApiMsgSize );

	sendDataToPeerMac( sendBuffer_p, l1ApiMsgSize );
#ifndef LAYER2_PHYSIM_THREAD
	freeMemPool(sendBuffer_p);
	sendBuffer_p = PNULL;
#endif
}

/****************************************************************************
 * Function Name  : processUeSimData
 * Inputs         : NONE 
 * Outputs        : NONE
 * Returns        : 
 * Variables      :
 * Description    : process the queues for data from UeSim.
 ****************************************************************************/
void processUeSimData()
{
    UInt32 numPdu = 0;
    UInt8* recvDataBuffer_p = PNULL;
    macPhyDataNode *data_p = PNULL;
#ifdef LAYER2_PHYSIM_THREAD
    macPhyMsg *ulMessage = PNULL;
#endif
    UInt8 qIndex = 0;

    if (0 == SF_g)
    {
        qIndex = 8;
    }
    else if (1 == SF_g)
    {
        qIndex = 9;
    }
    else
    {
        qIndex = SF_g - MAC_PHY_DELAY;
    }

    numPdu = sQueueCount(&macPhyDataQ_g[qIndex]);
    while (numPdu--)
    {
        data_p = (macPhyDataNode *) popNode (&macPhyDataQ_g[qIndex]);
        if (NULL != data_p)
        {
            if (NULL == (recvDataBuffer_p = (UInt8*)data_p->msg))
            {
                freeMemPool (data_p);
                data_p = NULL;
                continue;
            }
            sendDataToPeerMac((UInt8 *)data_p->msg, data_p->len);
#ifndef LAYER2_PHYSIM_THREAD
            if(data_p->msg)
            {
                freeMemPool (data_p->msg);
                data_p->msg = NULL;
            }
#endif
            if(data_p)
            {
                freeMemPool(data_p);
                data_p = NULL;
            }
        }
    }
}
void enbPhySimTickGen(int signum)
{
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

	sfnsf_g = ( (SFN_g & 0x0FFF)<< FAPI_SFN_VALUE_SHIFT )
		| (SF_g & 0x000F);
#if 0
	sfInd_g=( FAPI_subFrameIndication_st *) &(sfIndMessage_g.msgBody[0]);
	sfInd_g->sfnsf = sfnsf_g;
	sendDataToUe((UInt8 *)&sfIndMessage_g,L1_PARTIAL_API_MSG_SIZE + FAPI_SUBFRAME_INDICATION_MSG_BODY_SIZE);
#endif
	//eventfd_write(macPhyEventFd_g,1);
	//semPost(&layer2ReceiveSem_g);
	semPost(&tickGenSem_g);

} 
/****************************************************************************
 * Function Name  : enbPhySimTickGenThread
 * Inputs         : NONE 
 * Outputs        : NONE
 * Returns        : 
 * Variables      :
 * Description    : Thread for generating 1MS tick
 ****************************************************************************/
 struct timespec phy_time[1000];
UInt32 phycounter=0;
void * enbPhySimTickGenThread(void * arg)
{
	struct sigaction sa; 
	struct itimerval timer; 
	struct timespec timeStamp;
	void *zeroBufCpy_p     = PNULL;
	QSEGMENT segment;
	UInt8 *ulBuffer = PNULL;
	FAPI_l1ApiMsg_st           *l1ApiMsg_p = PNULL;
	FAPI_subFrameIndication_st *sfInd_p = PNULL;

	sigset_t set, oset;
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	sigprocmask(SIG_UNBLOCK, &set, &oset);
	PROCESS_TO_BIND_SET_CORE_NO(syscall(SYS_gettid),ltePhySimCoreNum_g);
	threadSetRtPriority(SCHED_FIFO, 85);

	/* Install timer_handler as the signal handler for SIGVTALRM.  */ 
	memset (&sa, 0, sizeof (sa)); 
	sa.sa_handler = &enbPhySimTickGen; 
	sigaction (SIGALRM, &sa, NULL); 
	/* Configure the timer to expire after 950 msec...  */ 
	timer.it_value.tv_sec = 0; 
	timer.it_value.tv_usec = 995 * TICK_IN_MS; 
	/* ... and every 950 msec after that.  */ 
	timer.it_interval.tv_sec = 0; 
	//timer.it_interval.tv_usec = 950 * TICK_IN_MS; 
	timer.it_interval.tv_usec = 0;
	/* Start a virtual timer. It counts down whenever this process is 
	 *      executing.  */ 
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

	sfnsf_g = ( (SFN_g & 0x0FFF)<< FAPI_SFN_VALUE_SHIFT )
		| (SF_g & 0x000F);
	sendSFI();
	do
	{
		setitimer (ITIMER_REAL, &timer, NULL); 
		semWait(&tickGenSem_g);
		zeroBufCpy_p=msgAlloc(PNULL,0,0,0);
		msgReserve(zeroBufCpy_p,0,BUFFLEN);
		if (PNULL ==  msgSegNext(zeroBufCpy_p, 0, &segment))
		{
			ltePanic("Error In msgSegNext %s\n",__func__);
		}
		ulBuffer = segment.base;

		l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) ulBuffer;
		l1ApiMsg_p->msgId = PHY_UL_SUBFRAME_INDICATION;
		l1ApiMsg_p->lenVendorSpecific = 0;
		l1ApiMsg_p->msgLen = L1_PARTIAL_API_MSG_SIZE + FAPI_SUBFRAME_INDICATION_MSG_BODY_SIZE;

		sfInd_p = ( FAPI_subFrameIndication_st *) &(l1ApiMsg_p->msgBody[0]);
		sfInd_p->sfnsf = sfnsf_g;

		clock_gettime(CLOCK_REALTIME, &timeStamp);
		phy_time[phycounter].tv_sec=timeStamp.tv_sec;
		phy_time[phycounter].tv_nsec=timeStamp.tv_nsec;
		phycounter++;
		if(phycounter>=1000)
			phycounter=0;

		sfInd_g=( FAPI_subFrameIndication_st *) &(sfIndMessage_g.msgBody[0]);
		sfInd_g->sfnsf = sfnsf_g;

		sendDataToPeerMac(zeroBufCpy_p,l1ApiMsg_p->msgLen);
		eventfd_write(macPhyEventFd_g,1);
		sendDataToUe((UInt8 *)&sfIndMessage_g,L1_PARTIAL_API_MSG_SIZE + FAPI_SUBFRAME_INDICATION_MSG_BODY_SIZE);

	}
	while(1);
}
/****************************************************************************
 * Function Name  : enbPhySimExecThread
 * Inputs         : NONE 
 * Outputs        : NONE
 * Returns        : 
 * Variables      :
 * Description    : to process the eNbPhySim
 ****************************************************************************/
void * enbPhySimExecThread(void * arg)
{
    struct timespec timeout, timeStamp1, timeStamp2, timeStamp3, timeStamp4;
    long long x2, x1;

    {
        PROCESS_TO_BIND_SET_CORE_NO(syscall(SYS_gettid),ltePhySimCoreNum_g);
        threadSetRtPriority(SCHED_FIFO, 45);
        {
            clock_gettime(CLOCK_REALTIME, &timeStamp1);
            timeout.tv_nsec = tickValue_g;
            timeout.tv_sec = 0;
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

            //fprintf(stderr,"\n SFN = %d  SF = %d\n",SFN_g, SF_g);
            sfnsf_g = ( (SFN_g & 0x0FFF)<< FAPI_SFN_VALUE_SHIFT )
                | (SF_g & 0x000F);
            sendSFI();
            processUeSimData();
            clock_gettime(CLOCK_REALTIME, &timeStamp2);

            if (timeStamp2.tv_sec > timeStamp1.tv_sec)
            {
                timeout.tv_sec = timeout.tv_sec - (timeStamp2.tv_sec - timeStamp1.tv_sec);
            }
            else
            {
                timeout.tv_sec = timeout.tv_sec - (timeStamp1.tv_sec - timeStamp2.tv_sec);
            }

            if (timeStamp2.tv_nsec > timeStamp1.tv_nsec)
            {
                timeout.tv_nsec = timeout.tv_nsec - (timeStamp2.tv_nsec - timeStamp1.tv_nsec);
            }
            else
            {
                timeout.tv_nsec = timeout.tv_nsec - (timeStamp1.tv_nsec - timeStamp2.tv_nsec);
            }

            if (timeout.tv_sec >= 1)
            {
                fprintf(stderr,"\n The sec is : %u\n",timeout.tv_sec);
                exit(0);
            }
            else if (timeout.tv_nsec > 1000000 * TICK_IN_MS)
            {
                fprintf(stderr,"\n The nsec is : %u\n",timeout.tv_nsec);
                exit(0);
            }
            //fprintf(stderr,"\n The nsec is : %u\n",timeout.tv_nsec); 
            clock_gettime(CLOCK_REALTIME, &timeStamp3);
            pselect(0, NULL, NULL, NULL, &timeout, NULL);
            clock_gettime(CLOCK_REALTIME, &timeStamp4);

            x1 = timeStamp3.tv_sec * 1000000000 + timeStamp3.tv_nsec;
            x2 = timeStamp4.tv_sec * 1000000000 + timeStamp4.tv_nsec;
            if ((x2 - x1) > 1100000 * TICK_IN_MS)
            {
                fprintf(stderr,"########## pselect takes ns = %d \n", (x2-x1) );
            }
        }
    }
} 

/*****************************************************************************
 * Function Name  :  createSocketsForRrcPhySimInterface
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    :  This function will create sockets for communication between
 *                   Enodeb PHY SIM <----> Enodeb RRC.
 *                   In case of Enodeb RRC <----> Enodeb PHY interface we are
 *                   opening two sockets for both way transmission.
 *****************************************************************************/
void createSocketsForRrcPhySimInterface(UInt16 rxEnodebRRCPort, UInt16 txEnodebRRCPort )
{

/* ##################  ENODEB RRC TX SOCKET #################### */
/*
**
**  The following socket will be used to transmit data to Enodeb RRC
**
*/
    if ((txEnodebRRCSocketFD_g[MAIN_CELL] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ltePanic(" Socket creation failed for Tx to Enodeb RRC.\n");
        exit(0);
    }

    txEnodebRRC_addr_g[MAIN_CELL].sin_family = AF_INET;
    txEnodebRRC_addr_g[MAIN_CELL].sin_port   = htons(txEnodebRRCPort);

    if (inet_aton((const char *)l2ModuleIP, &txEnodebRRC_addr_g[MAIN_CELL].sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }


/* ##################  ENODEB RRC RX SOCKET #################### */
/*
**
**  The following socket will listen to any data from Enodeb RRC
**
*/

    /* Nidhi Changes Start */
    if ((rxEnodebRRCSocketFD_g[MAIN_CELL] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ltePanic("Error while opening the receiver socket For Enodeb RRC.\n");
        exit(0);
    }
    fprintf(stderr, "RRC-PHY sockfd = %d\n", rxEnodebRRCSocketFD_g[MAIN_CELL]);

    rxEnodebRRC_addr_g[MAIN_CELL].sin_family = AF_INET;
    rxEnodebRRC_addr_g[MAIN_CELL].sin_port = htons (rxEnodebRRCPort);

    printf("control RX Port = %d, control TX Port = %d\n",rxEnodebRRCPort, txEnodebRRCPort);
    if (inet_aton((const char *)l2ModuleIP,&rxEnodebRRC_addr_g[MAIN_CELL].sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }

    if ((bind(rxEnodebRRCSocketFD_g[MAIN_CELL], (struct sockaddr *)&rxEnodebRRC_addr_g[MAIN_CELL], sizeof(rxEnodebRRC_addr_g[MAIN_CELL]))) == -1)
    {
        ltePanic ("Unable to bind the receiver socket with  Enodeb RRC.\n");
        exit(0);
    }
    /* Nidhi Changes End */
}

/*****************************************************************************
 * Function Name  :  createSocketsForPhySimUECntrlInterface
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    :  This function will create sockets for communication between
 *                   Enodeb PHY SIM <----> UE.
 *****************************************************************************/
void createSocketsForPhySimUECntrlInterface(UInt16 rxCntrlPortUeSim, UInt16 txCntrlPortUeSim )
{

/* ##################  ENB PHY SIM CTRL TX SOCKET #################### */
/*
**
**  The following socket will be used to transmit ctrl data to UE
**
*/
    if ((txUESimCtrlSocketFD_g = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ltePanic(" Socket creation failed for Tx to Enodeb RRC.\n");
        exit(0);
    }

    txUESimCtrl_addr_g.sin_family = AF_INET;
    txUESimCtrl_addr_g.sin_port   = htons(txCntrlPortUeSim);
printf("\n UEsim ctrl port %d\n",txUESimCtrl_addr_g.sin_port);

    if (inet_aton((const char *)ueSimIPAddress_g, &txUESimCtrl_addr_g.sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }


/* ##################  ENB PHY SIM CTRL RX SOCKET #################### */
/*
**
**  The following socket will listen to any ctrl data from UE
**
*/
    if ((rxUESimCtrlSocketFD_g = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ltePanic("Error while opening the receiver socket For Enodeb RRC.\n");
        exit(0);
    }
    /* COVERITY 10015 */
    if(-1 == fcntl(rxUESimCtrlSocketFD_g, F_SETFL, O_NONBLOCK))    
    {
      ltePanic("Error while setting F_SETFL \n");
      exit(0);
    }
    /* COVERITY 10015 */
    rxUESimCtrl_addr_g.sin_family = AF_INET;
    rxUESimCtrl_addr_g.sin_port = htons (rxCntrlPortUeSim);

    if (inet_aton((const char *)l2ModuleIP,&rxUESimCtrl_addr_g.sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }

    if ((bind(rxUESimCtrlSocketFD_g, (struct sockaddr *)&rxUESimCtrl_addr_g, sizeof(rxUESimCtrl_addr_g))) == -1)
    {
        ltePanic ("Unable to bind the receiver socket with  Enodeb RRC.\n");
        exit(0);
    }
}

/****************************************************************************
 * Function Name  : initRxSocketAddress
 * Inputs         : sockaddr_in, socketFileDescriptor,Port Number
 * Outputs        :
 * Returns        : status (0-Failure,1-Successful)
 * Variables      :
 * Description    : to intialize the Socket address with specific socket file
 descriptor and specific port
 ****************************************************************************/
static UInt8 initRxSocketAddress(struct sockaddr_in *s,SInt32 *sockfd,UInt16 port)
{
    struct hostent *host_p;
    host_p= (struct hostent *) gethostbyname((char *)l2ModuleIP);
    if((*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("creating socket error");
        return 0;
    }
    (*s).sin_family = AF_INET;
    (*s).sin_port = htons(port);
    (*s).sin_addr = *((struct in_addr *)host_p->h_addr);
    //printf("\n The Ip address of enbPhySim is : %s",(*s).sin_addr.s_addr);
    bzero(&((*s).sin_zero),8);
    return 1;
}

/****************************************************************************
 * Function Name  : initTxSocketAddress
 * Inputs         : sockaddr_in, socketFileDescriptor,Port Number
 * Outputs        :
 * Returns        : status (0-Failure,1-Successful)
 * Variables      :
 * Description    : to intialize the Socket address with specific socket file
 descriptor and specific port
******************************************************************************/
static UInt8 initTxSocketAddress(struct sockaddr_in *s,SInt32 *sockfd,UInt16 port)
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
 * Function Name  : fillIpAndPortInfo
 * Inputs         : pointer of :
 *                      Enb MAC receiving port, Enb MAC tx port,  UeSim rx port
 *                       UeSim tx port
 * Outputs        : success (1) or failure (0)
 * Returns        :
 * Variables      :
 * Description    : set all the passed port to value read from file

 ****************************************************************************/
static UInt8 fillIpAndPortInfo(UInt16 *rxPortEnbMAC,
                          UInt16 *txPortEnbMAC,
                          UInt16 *rxPortUeSim,
                          UInt16 *txPortUeSim,
                          UInt16 *rxEnodebRRCPort,
                          UInt16 *txEnodebRRCPort,
                          UInt16 *txCntrlPortUeSim,
                          UInt16 *rxCntrlPortUeSim)
{
    FILE *fp;
    char portConfigFileName[256] ;
    char buf[101] = {0,};
    UInt32 port=0;

    GET_ENB_PHY_SIM_PORT_CONFIG_FILE(portConfigFileName);
    fp = fopen(portConfigFileName,"r");
    if(fp == PNULL) 
    {
        LTE_LOG(LOG_WARNING, PNULL,
            "Fail to open the port config file ");
        printf("[%s] Fail to open the port config file %s\n",
               __func__,portConfigFileName);
        return 0;
    }
    
    while( fgets(buf,100,fp))
    {
        if(sscanf(buf,"ENB_MAC_RX_PORT =%d",&port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(rxPortEnbMAC) = (UInt16)port;
                fprintf(stderr," rxPortEnbMAC = %d\n",*(rxPortEnbMAC));
            }
        }
        else if(sscanf(buf,"ENB_MAC_TX_PORT =%d",&port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(txPortEnbMAC) = (UInt16)port;
                fprintf(stderr," txPortEnbMAC = %d\n",*(txPortEnbMAC));
            }
        }
        else if(sscanf(buf,"UESIM_RX_PORT =%d",&port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(rxPortUeSim) = (UInt16)port;
                fprintf(stderr," rxPortUeSim = %d\n",*(rxPortUeSim));
            }
        }
        else if(sscanf(buf,"UESIM_TX_PORT =%d",&port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(txPortUeSim) = (UInt16)port;
                fprintf(stderr," txPortUeSim = %d\n",*(txPortUeSim));
            }
        }
        else if(sscanf(buf,"ENB_RRC_TX_PORT =%d",&port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(txEnodebRRCPort) = (UInt16)port;
                fprintf(stderr," txEnodebRRCPort = %d\n",*(txEnodebRRCPort));
            }
        }
        else if(sscanf(buf,"ENB_RRC_RX_PORT =%d",&port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(rxEnodebRRCPort) = (UInt16)port;
                fprintf(stderr," rxEnodebRRCPort = %d\n",*(rxEnodebRRCPort));
            }
        }
        else if(sscanf(buf,"UESIM_RRC_TX_PORT =%d",&port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(txCntrlPortUeSim) = (UInt16)port;
                fprintf(stderr," txCntrlPortUeSim = %d\n",*(txCntrlPortUeSim));
            }
        }
        else if(sscanf(buf,"UESIM_RRC_RX_PORT =%d",&port) > 0)
        {
            if(port > 0 && port < 65535 )
            {
                *(rxCntrlPortUeSim) = (UInt16)port;
                fprintf(stderr," rxCntrlPortUeSim = %d\n",*(rxCntrlPortUeSim));
            }
        }
        else if (sscanf(buf,"UE_PHYSIM_IP_ADDRESS=%s",ueSimIPAddress_g) > 0)
        {
            fprintf(stderr," UE_PHYSIM_IP_ADDRESS = %s\n",(ueSimIPAddress_g));
        }
        else if (sscanf(buf,"ENB_PHYSIM_IP_ADDRESS=%s",l2ModuleIP) > 0)
        {
            fprintf(stderr," ENB_PHYSIM_IP_ADDRESS = %s\n",(l2ModuleIP));
        }
    }
    fclose(fp);
    return 1;
}

/*****************************************************************************
 * Function Name  :  createSocketsForEnbPhySimUeSimInterface
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    :  This function will create sockets for communication between
 *                   ENB PHY SIM<----> UE SIM.
 *****************************************************************************/
void createSocketsForEnbPhySimUeSimInterface(UInt16 rxPortUeSim, UInt16 txPortUeSim )
{

/* ##################  UE SIM TX SOCKET #################### */
/*
**
**  The following socket will be used to transmit data to UE SIM
**
*/
    if ((txUeSimSockFD_g = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ltePanic(" Socket creation failed for Tx to UE SIM.\n");
        exit(0);
    }

    txUeSim_addr_g.sin_family = AF_INET;
    txUeSim_addr_g.sin_port   = htons(txPortUeSim);

    if (inet_aton((const char *)ueSimIPAddress_g, &txUeSim_addr_g.sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }

/* ##################  UE SIM RX SOCKET #################### */
/*
**
**  The following socket will listen to any data from UE SIM
**
*/

    if ((rxUeSimSockFD_g = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ltePanic("Error while opening the receiver socket For UE SIM.\n");
        exit(0);
    }
    /* COVERITY 10014 */
    if(-1 == fcntl(rxUeSimSockFD_g, F_SETFL, O_NONBLOCK))
    {
    ltePanic("Error while setting F_SETFL \n");
    exit(0);
    }
    /* COVERITY 10014 */
    rxUeSim_addr_g.sin_family = AF_INET;
    rxUeSim_addr_g.sin_port = htons (rxPortUeSim);

    //if (inet_aton((const char *)ueSimIPAddress_g,&rxUeSim_addr_g.sin_addr) == 0)
    if (inet_aton((const char *)l2ModuleIP,&rxUeSim_addr_g.sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }

    if ((bind(rxUeSimSockFD_g, (struct sockaddr *)&rxUeSim_addr_g, sizeof(rxUeSim_addr_g))) == -1)
    {
        ltePanic ("Unable to bind the receiver socket with  UE SIM.\n");
        exit(0);
    }

}

/****************************************************************************
 * Function Name  : initEnbPhy
 * Inputs         :
 *
 *
 * Outputs        :
 * Returns        :
 * Description    :
 *
 ****************************************************************************/
void initEnbPhy()
{
    UInt16 rxPortEnbMAC;
    UInt16 txPortEnbMAC;
    UInt16 rxPortUeSim;
    UInt16 txPortUeSim;
    UInt16 rxEnodebRRCPort;
    UInt16 txEnodebRRCPort;
    UInt16 txCntrlPortUeSim;
    UInt16 rxCntrlPortUeSim;
#if 0
    UInt16 rxPortEnbMAC = 8889;
    UInt16 txPortEnbMAC = 9998;
    UInt16 rxPortUeSim = 9999;
    UInt16 txPortUeSim = 8888;
    UInt16 rxEnodebRRCPort = 9879;
    UInt16 txEnodebRRCPort = 19879;
    UInt16 txCntrlPortUeSim = 9876;
    UInt16 rxCntrlPortUeSim = 19876;
#endif
    //UInt16 txSFIPortUeSim = 22059;
    UInt16 queueInitLoop = 0;

    //strcpy((char *)l2ModuleIP,"172.16.120.12");
    //strcpy((char *)l2ModuleIP,"127.0.0.1");
    //strcpy((char *)ueSimIPAddress_g,"127.0.0.1");
    //strcpy((char *)ueSimIPAddress_g,"172.16.120.20");

    for (queueInitLoop = 0; queueInitLoop < MAX_SUBFRAME; queueInitLoop++)
    {
        sQueueInit(&macPhyDataQ_g[queueInitLoop]);
    }
    //initMemPool();
    createMemPool(100,100);
    createMemPool(100,100);
    createMemPool(10000,100);

    fillIpAndPortInfo(&rxPortEnbMAC,\
                      &txPortEnbMAC,\
                      &rxPortUeSim,\
                      &txPortUeSim,\
                      &rxEnodebRRCPort,\
                      &txEnodebRRCPort,\
                      &txCntrlPortUeSim,\
                      &rxCntrlPortUeSim);


    initRxSocketAddress(&rxMacLayerAddr_g,&enbPhySimRxSockfd_g,rxPortEnbMAC);
    //setSocketBlockingEnabled(enbPhySimRxSockfd_g,1);
    
    if ( bind(enbPhySimRxSockfd_g, (struct sockaddr*)&rxMacLayerAddr_g,
                sizeof(SockAddr)) == -1)
    {
        ltePanic("Bind Failed ENB-PHY RX PORT = %d", rxPortEnbMAC);
        return;
    }
    initTxSocketAddress(&txMacLayerAddr_g,&enbPhySimTxSockfd_g,txPortEnbMAC);

    createSocketsForEnbPhySimUeSimInterface(rxPortUeSim, txPortUeSim);
    createSocketsForRrcPhySimInterface(rxEnodebRRCPort, txEnodebRRCPort);
    createSocketsForPhySimUECntrlInterface(rxCntrlPortUeSim, txCntrlPortUeSim);
#ifdef LAYER2_PHYSIM_THREAD
QUEUE_INIT_L2_PHYSIM_Q(dlL2PhysimDataQ_g);
QUEUE_INIT_L2_PHYSIM_Q(dlL2PhysimControlQ_g);
QUEUE_INIT_L2_PHYSIM_Q(ulL2PhysimDataQ_g);
QUEUE_INIT_L2_PHYSIM_Q(ulL2PhysimControlQ_g);
semInit(&layer2ReceiveSem_g,0);
semInit(&phySimReceiveSem_g,0);
semInit(&tickGenSem_g,0);
macControlEventFd_g = eventfd(0,0);
macDataEventFd_g = eventfd(0,0);
macPhyEventFd_g = eventfd(0,0);
//tickGenFd_g = eventfd(0,0);
sfIndMessage_g.msgId=PHY_UL_SUBFRAME_INDICATION;
sfIndMessage_g.lenVendorSpecific=0;
sfIndMessage_g.msgLen=FAPI_SUBFRAME_INDICATION_MSG_BODY_SIZE;
#endif
}

/*****************************************************************************
 * Function Name  :  rxTxFromEnbAndUeSim
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Description    : This function will create the Sockets needed for comm.
 *                  between Enodeb PHY <-> Enodeb MAC.
 *****************************************************************************/
void rxTxFromEnbAndUeSim()
{
	SInt32 maxFd    = 0;
	int ret = 0;
	FAPI_subFrameIndication_st * recvSfnSf = 0;
	UInt16 recvSFN = 0;
	UInt16 recvSF = 0;
	UInt8 loopCount = 0;
	UInt8 *buffer_p = PNULL;
	SInt32 maxFDArray[MAX_FD_ELEMENTS] = {0};
	SInt32 bytesRead  = 0;
	FAPI_l1ApiMsg_st    *pL1Api = PNULL;
	eventfd_t tempfd;

#ifdef LAYER2_PHYSIM_THREAD
	UInt8 *ulBuffer = PNULL;
	UInt8 *msg_p = PNULL;
	struct timeval timeout;
	macPhyMsg *dlMessage=PNULL;
	macPhyMsg *ulMessage=PNULL;
	void *zeroBufCpy_p     = PNULL;
	QSEGMENT segment;
#endif
	macPhyDataNode *dataNode_p = PNULL;
	socklen_t length  = sizeof(struct sockaddr_in);

#ifndef LAYER2_PHYSIM_THREAD
/*Nidhi Changes Start */
	maxFDArray[0] = rxEnodebRRCSocketFD_g[MAIN_CELL];
/* Nidhi Changes End */
	maxFDArray[1] = enbPhySimRxSockfd_g;
	maxFDArray[2] = rxUESimCtrlSocketFD_g;
	maxFDArray[3] = rxUeSimSockFD_g; 

#else
	maxFDArray[0] = rxUeSimSockFD_g; 
	maxFDArray[1] = macDataEventFd_g;
	maxFDArray[2]= macControlEventFd_g;
	maxFDArray[3] = rxUESimCtrlSocketFD_g;
	//    maxFDArray[4] = tickGenFd_g;

#endif
	for(loopCount = 0; loopCount < MAX_FD_ELEMENTS; loopCount++)
	{
		if (maxFDArray[loopCount] > maxFd )
		{
			maxFd = maxFDArray[loopCount];
		}
	}
	maxFd += 1;
	fd_set readFds;

	while (1)
	{
		FD_ZERO(&readFds);
#ifndef LAYER2_PHYSIM_THREAD
/* Nidhi Changes Start */
		FD_SET(rxEnodebRRCSocketFD_g[MAIN_CELL], &readFds);
/* Nidhi Changes End */
		FD_SET(enbPhySimRxSockfd_g, &readFds);
#else
		FD_SET(macDataEventFd_g,&readFds);
		FD_SET(macControlEventFd_g,&readFds);
		//	FD_SET(tickGenFd_g,&readFds);

#endif
		FD_SET(rxUESimCtrlSocketFD_g, &readFds);
		FD_SET(rxUeSimSockFD_g, &readFds);
		if (select  (maxFd, &readFds, NULL, NULL, NULL) == -1)
		{
			lteWarning("select failed.....!!!!\n");
			continue;
		}

#ifndef LAYER2_PHYSIM_THREAD
        /* Nidhi Changes Start */
		if(FD_ISSET(rxEnodebRRCSocketFD_g[MAIN_CELL], &readFds))
		{
			fprintf(stderr,"Control packets received from enbMac\n ");
			if ((bytesRead = recvfrom(rxEnodebRRCSocketFD_g[MAIN_CELL], recvDataBuffer_g, RECV_BUFF_SIZE, 0,
							(struct sockaddr*)&rxEnodebRRC_addr_g, &length))
					== -1)
			{
				ltePanic ("Error in receive message.\n");
				continue;
			}

			if (sendto(txUESimCtrlSocketFD_g,(unsigned char *)recvDataBuffer_g,bytesRead,\
						0,(const struct sockaddr *)&txUESimCtrl_addr_g, sizeof (txUESimCtrl_addr_g)) < 0)
			{
				ltePanic("Error in sending data on socket:: errno=%d",errno);
			}

			FD_CLR(rxEnodebRRCSocketFD_g[MAIN_CELL], &readFds);
		}
        /* Nidhi Changes End */
#endif
		if(FD_ISSET(macDataEventFd_g,&readFds))
		{
			eventfd_read(macDataEventFd_g,&tempfd);
			while(QUEUE_COUNT_L2_PHYSIM_Q(dlL2PhysimDataQ_g))
			{
				DEQUEUE_L2_PHYSIM_Q(dlL2PhysimDataQ_g,dlMessage);

				if(dlMessage->buffer)
				{
					sendDataToUe(dlMessage->buffer, dlMessage->len);
					freeMemPool(dlMessage->buffer);
					dlMessage->buffer=PNULL;
				}
				freeMemPool(dlMessage);
				dlMessage=PNULL;
			}
			FD_CLR(macDataEventFd_g, &readFds);
		}
		if(FD_ISSET(rxUeSimSockFD_g, &readFds))
		{
			do
			{
				//printf("\nData packets received from MAC Emulator\n ");
				zeroBufCpy_p=msgAlloc(PNULL,0,0,0);
				msgReserve(zeroBufCpy_p,0,BUFFLEN);
				if (PNULL ==  msgSegNext(zeroBufCpy_p, 0, &segment))
				{
					ltePanic("Error In msgSegNext %s\n",__func__);
					//printf("\n Error In msgSegNext %s\n",__func__);
					msgFree(zeroBufCpy_p);
					break;
				}
				ulBuffer = segment.base;

				if ((bytesRead = recvfrom(rxUeSimSockFD_g, ulBuffer, RECV_BUFF_SIZE, 0,
								(struct sockaddr*)&rxUeSim_addr_g, &length))
						<= 0)
				{
					//                printf("\n msg free %s\n",__func__);
					msgFree(zeroBufCpy_p);
					continue;
				}

				pL1Api = (FAPI_l1ApiMsg_st *)ulBuffer;
				if (pL1Api->msgId == PHY_UL_SUBFRAME_INDICATION)
				{
					msgFree(zeroBufCpy_p);

				}
				else if ( pL1Api->msgId == PHY_ERROR_INDICATION )
				{
					//	    fprintf(stderr,"\n err ind received\n");
					msgFree(zeroBufCpy_p);
					zeroBufCpy_p=PNULL;
				}
				else if ( pL1Api->msgId  < 0x8b  || pL1Api->msgId > 0x84 ) 
				{
					//printf("\n message received 0x%x\n", pL1Api->msgId);

					recvSfnSf = ( FAPI_subFrameIndication_st *) &( pL1Api->msgBody[0]);
					recvSFN = (UInt16)(recvSfnSf->sfnsf & FAPI_SFN_MASK);
					recvSFN >>= FAPI_SFN_VALUE_SHIFT;
					recvSF = (UInt8)(recvSfnSf->sfnsf & FAPI_SF_MASK);
/*
					printf("\n data received from UE msgid 0x%x going to mac %u %u recv %u %u\n",
						pL1Api->msgId,SFN_g,SF_g,recvSFN,recvSF);
*/
					sendDataToPeerMac(zeroBufCpy_p, bytesRead);
#if 0
					if (((recvSFN * MAX_SUBFRAME) + recvSF) - ((SFN_g * MAX_SUBFRAME) + SF_g) > MAC_PHY_DELAY)
						//			    if(0)
					{
						printf("\n data received from UE msgid 0x%x going to mac %u %u recv %u %u\n",
								pL1Api->msgId,SFN_g,SF_g,recvSFN,recvSF);
						sendDataToPeerMac(zeroBufCpy_p, bytesRead);
					}
					else
					{
						printf("\n data received from UE going to node cur %u %u recv %u %u %d\n",
								SFN_g,SF_g,recvSFN,recvSF,bytesRead);
						dataNode_p = (macPhyDataNode*)getMemFromPool(sizeof(macPhyDataNode), PNULL);
						if ( PNULL == dataNode_p)
						{
							ltePanic("memory allocation failed for dataNode_p while recevied data from ueSim\n");
						}
						dataNode_p->len = bytesRead;

						dataNode_p->msg = zeroBufCpy_p;

						pushNode (&macPhyDataQ_g[recvSF], &dataNode_p->anchor);
					}
#endif

				}
#ifdef LAYER2_PHYSIM_THREAD
				else
				{
					msgFree(zeroBufCpy_p);
				}
#endif
			}while(0);
			FD_CLR(rxUeSimSockFD_g, &readFds);
			//	    processUeSimData();
		}


		if(FD_ISSET(macControlEventFd_g, &readFds))
		{
			eventfd_read(macControlEventFd_g,&tempfd);
			printf("\n control message received from eNB mac\n");

			while(QUEUE_COUNT_L2_PHYSIM_Q(dlL2PhysimControlQ_g))
			{
				DEQUEUE_L2_PHYSIM_Q(dlL2PhysimControlQ_g,dlMessage);

				printf("\n type 0x%02x length %d %s\n",((FAPI_l1ApiMsg_st *)dlMessage->buffer)->msgId,
						dlMessage->len,__func__);
				if(dlMessage->buffer)
				{
					if (sendto(txUESimCtrlSocketFD_g,(unsigned char *)dlMessage->buffer,dlMessage->len,\
								0,(const struct sockaddr *)&txUESimCtrl_addr_g, sizeof (txUESimCtrl_addr_g)) < 0)
					{
						ltePanic("Error in sending data on socket:: errno=%d",errno);
					}
					if(PHY_START_REQUEST == ((FAPI_l1ApiMsg_st *)dlMessage->buffer)->msgId)
					{
						ret = threadCreate(&enbPhySimTickGenThread_g,
								THREAD_CREATE_DETACHED,
								/*THREAD_SCHED_FIFO,*/
								SCHED_FIFO,
								/*0,*/
								EXECSCHEDULER_THREAD_PRIORITY,
								THREAD_EXPLICIT_SCHED,
								THREAD_SCOPE_DEFAULT,
								enbPhySimTickGenThread,
								(void *) NULL );

						if ( ret != 0)
						{
							ltePanic("Error in creating enbPhySimExecThread thread");
						}
					}
					freeMemPool(dlMessage->buffer);
					dlMessage->buffer=PNULL;
				}
				freeMemPool(dlMessage);
				dlMessage=PNULL;
			}
			FD_CLR(macControlEventFd_g, &readFds);

		}

		if(FD_ISSET(rxUESimCtrlSocketFD_g, &readFds))
		{
			fprintf(stderr,"Control packets received from MAC Emulator\n ");
			if ((bytesRead = recvfrom(rxUESimCtrlSocketFD_g, recvDataBuffer_g, RECV_BUFF_SIZE, 0,
							(struct sockaddr*)&rxUESimCtrl_addr_g, &length))
					== -1)
			{
				ltePanic ("Error in receive message.\n");
			}

#ifdef LAYER2_PHYSIM_THREAD
			sendControlToEnbMac(recvDataBuffer_g,bytesRead);
#else
			sendto(txEnodebRRCSocketFD_g[MAIN_CELL],(unsigned char *)recvDataBuffer_g,bytesRead,\
					0,(const struct sockaddr *)&txEnodebRRC_addr_g[MAIN_CELL], sizeof (txEnodebRRC_addr_g[PCELL_INDEX]));
#endif
			FD_CLR(rxUESimCtrlSocketFD_g, &readFds);
		}



#ifndef LAYER2_PHYSIM_THREAD

		if(FD_ISSET(enbPhySimRxSockfd_g, &readFds))
		{
			if ( ( bytesRead = recvfrom(enbPhySimRxSockfd_g, recvDataBuffer_g,
							RECV_BUFF_SIZE, 0,
							(struct sockaddr*)&rxMacLayerAddr_g,&length)) <= 0)
			{
				if (errno != EAGAIN)
				{
					perror("recv from error in receiver");
					ltePanic ("lte panic due to recv from error\n" );
				}
			}
			sendDataToUe(recvDataBuffer_g, bytesRead);
			FD_CLR(enbPhySimRxSockfd_g, &readFds);
		}
#endif
	}

}
/*****************************************************************************
 * Function Name  : main
 * Inputs         : None
 * Outputs        : None
 * Returns        : 
 * Description    : This function is a main function of enbPhy
 *
 *****************************************************************************/

#ifndef LAYER2_PHYSIM_THREAD
int main(int argc, char **argv)
#else
void* physimThreadExecutionSchedular(void* param) 
#endif
{
    int ret = 0;
#ifndef LAYER2_PHYSIM_THREAD
    if(argc < 2 )
    {
        fprintf(stderr,"Usage is ./l2PhySim <core no> \n");
    }
    else
    {
        ltePhySimCoreNum_g = atoi(argv[1]);
    }
#else
    ltePhySimCoreNum_g = PHYSIM_CORE_NUM;
#endif
   
    PROCESS_TO_BIND_SET_CORE_NO(syscall(SYS_gettid),ltePhySimCoreNum_g);
#ifndef LAYER2_PHYSIM_THREAD
    threadSetRtPriority(SCHED_FIFO, 45);
#else
    threadSetRtPriority(SCHED_FIFO, 70);
    //threadSetRtPriority(SCHED_FIFO, 50);
#endif
    initEnbPhy();
#if 0
    ret = threadCreate(&enbPhySimExecThread_g,
            THREAD_CREATE_DETACHED,
            /*THREAD_SCHED_FIFO,*/
            SCHED_FIFO,
            /*0,*/
            EXECSCHEDULER_THREAD_PRIORITY,
            THREAD_EXPLICIT_SCHED,
            THREAD_SCOPE_DEFAULT,
            enbPhySimExecThread,
            (void *) NULL );

    if ( ret != 0)
    {
        ltePanic("Error in creating enbPhySimExecThread thread");
    }
#endif

    fprintf(stderr,"\n Thread Created \n");
    rxTxFromEnbAndUeSim();

    return 0;
}
