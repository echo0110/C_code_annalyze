/***************************************************************************
*
* ARICENT -
*
* Copyright (C) 2014 Aricent Inc . All Rights Reserved.
*
****************************************************************************
*
* \$Id: lteMacComPorting.c
*
****************************************************************************
*
* File Description: This file contains data structures and APIs for
*                   porting MAC layer to different platforms.
*
****************************************************************************
* Revision Details
* ----------------------
*
***************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include "lteMisc.h"
#include "lteMacComPorting.h"
#include "lteLog.h"
/* SPR 20430 Changes Start*/
#include "lteTypes.h"
/* SPR 20430 Changes End*/
#include <mqueue.h>
/******************************************************************************
 Private Definitions
 *****************************************************************************/



/******************************************************************************
 Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
/******************************************************************************
 * Private Constants
 *****************************************************************************/
#define QNAME_SIZE 20
#define QNAME_SIZE_PR 256
/******************************************************************************
 * Exported Variables
 *****************************************************************************/
#ifdef FLEXRAN
void initWiresharkLoggingInterfaceLocal(InternalCellIndex internalCellIndex);
extern SInt32 flexranWiresharkFd_g [MAX_NUM_CELL]; 
extern SockAddr flexranTx_g[MAX_NUM_CELL];
#endif
extern SInt32 rxSockFD_g;
extern UInt16 rxPortMAC_g;
extern UInt16 txPortMacOAM_g;
extern UInt16 txPortMacRRC_g;
extern SInt32 txSockFD_g;
extern SInt32 txOAMSockFD_g;
/*SPR 20863 FIX*/
#ifndef UE_SIM_TESTING
extern SInt32 txRRMSockFD_g;
extern SInt16 txPortRRM_g;
extern Char8 rrmIPAddress_g[HOSTNAME_MAX_LEN];
extern SockAddr txRRM_addr_g;
#endif
/*SPR 20863 FIX*/
extern Char8 macIPAddress_g[HOSTNAME_MAX_LEN];
extern Char8 rrcIPAddress_g[HOSTNAME_MAX_LEN];
extern Char8 oamIPAddress_g[HOSTNAME_MAX_LEN];

/*Socket file descriptor used in creating Socket */
/* CA Changes start */
extern Char8 phyIPAddress_g[MAX_NUM_CELL][HOSTNAME_MAX_LEN];
#ifndef FLEXRAN
extern SInt32 phyRxSockFD_g[MAX_NUM_CELL];

/*Socket file descriptor used in creating Socket */
extern SInt32 phyTxSockFD_g[MAX_NUM_CELL] ;
#endif

#ifdef FLEXRAN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#define DO_TAP_BUF_SIZE (8192)
#define ETHER_HDR_LEN1  14
#define IP_HDR_LEN      20
#define UDP_HDR_LEN     8

struct iphdr1
{
   uint8_t  ver_hl;     /* version and header length default 0x45*/
   uint8_t  tos;        /* type of service */
   uint16_t len;        /* total length */
   uint16_t id;         /* identification */
   uint16_t offset;     /* fragment offset */
   uint8_t  ttl;        /* time to live */
   uint8_t  protocol;       /* protocol */
   uint16_t checksum;       /* checksum */
   uint32_t src;                /* source IP address */
   uint32_t dst;            /* destination IP address */
} __attribute__ ((packed));

/*UDP header*/
struct udphdr1
{
   uint16_t   source_port;      /* source port */
   uint16_t   dest_port;        /* dest port */
   uint16_t   len;              /* length of packet */
   uint16_t   checksum;         /* checsum */
} __attribute__ ((packed));

int wireshark_enable = 1;
uint32_t dst_ipaddr = -1;
uint32_t local_ipaddr = -1;
int raw_sock = -1;
int do_tap = 0;
unsigned char g_do_tap_buf[DO_TAP_BUF_SIZE];
uint8_t macPhyBuffer[10000];

extern int open_raw_sock(const char* src_ip, const char* dst_ip);
void do_tap_fun(void* data, uint32_t len, InternalCellIndex cellId);
extern SInt32 flexranFapiLoggingEnable_g;
#endif
extern UInt16 phyRxPort_g[MAX_NUM_CELL] ;
extern UInt16 phyTxPort_g[MAX_NUM_CELL] ;

/* Socket creation for Transmitting data to PHY Layer */


#ifdef DL_UL_SPLIT_TDD
extern SInt32 phyRxSockUlFD_g[MAX_NUM_CELL] ;
extern SInt32 phyRxPortUl_g[MAX_NUM_CELL] ;
#endif


/* Socket creation for Transmitting data to PHY Layer */
#ifndef FLEXRAN
extern SInt32 phyControlSockFD_g[MAX_NUM_CELL] ;
#endif

extern UInt16 phyControlRxPort_g[MAX_NUM_CELL];
extern SockAddr phyRxAddr_g[MAX_NUM_CELL];
extern SockAddr phyTxAddr_g[MAX_NUM_CELL];
SockAddr phyControlAddr_g[MAX_NUM_CELL];
extern SockAddr phyControlTxAddr_g[MAX_NUM_CELL];
/* CA Changes end */

extern SInt32 phyFrameIndSockFD_g ;
extern UInt16 phyFrameIndPort_g ;

extern SockAddr phyFrameIndAddr_g;

extern SockAddr tx_addr_g;
extern SockAddr txOAM_addr_g;

#ifdef DL_UL_SPLIT_TDD
SockAddr phyRxAddrUl_g;
#endif
/* SPR 4915 : Start */
/*SPR 21545 Start*/
extern UInt8 instanceL2_g;
/*SPR 21545 End*/
/* SPR 4915 : End */


/*SPR 20908 Start*/
extern SInt32 pdcpRxSockFD_g;
/*SPR 21366 +*/
extern Int InitGtpuPdcpRxSocket(Int moduleId, UInt8 isBlocking);
/*SPR 21366 -*/
/*SPR 20908 End*/
/*****************************************************************************
 * Global variables 
 ****************************************************************************/
 /*SPR 16855 +- */

/*****************************************************************************
 * Function Name  : initMsgQueueForICC
 * Inputs         : name, oFlag
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the ICC Queue
 *****************************************************************************/
SInt32 initMsgQueueForICC(const char *name, SInt32 oflag)
{
    struct mq_attr *p_attr = NULL;
#ifndef LINUX_PC_TEST
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 200;
    attr.mq_msgsize = 400;
    attr.mq_curmsgs = 200;

    p_attr = &attr;
#endif

    return mq_open_wrapper(name, (int)oflag, 0, p_attr);
}
/* SPR 4915 : Start */
/*****************************************************************************
 * Function Name  : unlinkMsgQueueForICC
 * Inputs         : name
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the UL ICC Queue
 *****************************************************************************/
SInt32 unlinkMsgQueueForICC(const char *name)
{
    char qname[MAX_MQ_NAME_LEN];
/*SPR 21653 changes start*/
    snprintf(qname, MAX_MQ_NAME_LEN, "%s", name);
/*SPR 21653 changes end*/
    return mq_unlink(qname);
}
/* SPR 4915 : End */

/*****************************************************************************
 * Function Name  : sendMsgWithIccQ
 * Inputs         : messageQueues Descriptor
 *                  Messsage Pointer
 *                  Messsage Length
 *                  Messsage Priority
 * Outputs        : None
 * Returns        : None
 * Description    : Send message with ICC Queue
 *****************************************************************************/
SInt32 sendMsgWithIccQ(SInt32 mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio)
{
    SInt32 retVal = mq_send(mqdes, msg_ptr, msg_len,  msg_prio);
    if (retVal == -1)
    {
    }
    return retVal;
}
    
/*****************************************************************************
 * Function Name  : recvMsgWithIccQ
 * Inputs         : messageQueues Descriptor
 *                  Messsage Pointer
 *                  Messsage Length
 *                  Messsage Priority
 * Outputs        : None
 * Returns        : None
 * Description    : Receive message with ICC Queue
 *****************************************************************************/
/* +- SPR 17777 */
SInt32 recvMsgWithIccQ(SInt32 mqdes, const void *msg_ptr)
/* +- SPR 17777 */
{
    struct mq_attr attr;
    mq_getattr(mqdes, &attr);
    unsigned int priority;
    SInt32 retVal = mq_receive(mqdes, (char *)msg_ptr, attr.mq_msgsize,  &priority);

    if (retVal == -1)
    {
        fprintf(stderr, "Error in receive from mesg q: errorno = %d, MsgQ descripter = %d\n", errno, mqdes);
    }
    return retVal;
}

/*****************************************************************************
 * Function Name  : closeIccQ
 * Inputs         : messageQueues Descriptor
 * Outputs        : None
 * Returns        : None
 * Description    : Close ICC Queue
 *****************************************************************************/
SInt32 closeIccQ(SInt32 mqdes)
{
   return mq_close(mqdes);
}
 /*SPR 16855 +- */

#ifdef DL_UL_SPLIT

/*****************************************************************************
 * Function Name  : initULContainerNackQueue
 * Inputs         : Queue ID and index
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the UL container NACK queue
 *****************************************************************************/
void initComULContainerNackQueue(SInt32 *qId, UInt8 index)
{
    const char qName[] = "/ULContainerNackQueue_";
    char name[33] = {0};
    sprintf(name, "%s%d",qName , index);
    *qId = initMsgQueueForICC(name, O_RDWR|O_CREAT);
    if (-1 == *qId)
    {
        ltePanic(" Failure in opening ULContainerNackQueue, Error no : %d\n",errno );

    }
}

/*****************************************************************************
 * Function Name  : initULContainerNackQueueForTempUE
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the UL container NACK queue
 *                  queue
 *****************************************************************************/
void initComULContainerNackQueueForTempUE( SInt32 *qId, UInt8 index)
{
    const char qName[] = "/ULContainerTempNackQ_";
    char name[33] = {0};
    sprintf(name, "%s%d",qName , index);
    *qId = initMsgQueueForICC(name, O_RDWR|O_CREAT);
    if (-1 == *qId)
    {
        ltePanic(" Failure in opening ULContainerNackQueue, Error no : %d\n",errno );
    }
}


/*****************************************************************************
 * Function Name  : closeULContainerNackQueue
 * Inputs         : Number of containers
 * Outputs        : None
 * Returns        : None
 * Description    : Close the UL container NACK queue
 *****************************************************************************/
void closeULContainerNackQueue(UInt32 max_num_of_container )
{
    /* + SPR 17439 */
    UInt32 i =0;
    /* - SPR 17439 */
    for (i=0; i < max_num_of_container;i++)
    {
        {
            const char qName[] = "/ULContainerNackQueue_";
            char name[33] = {0};
            sprintf(name, "%s%d",qName , i);
/* SPR 4915 : Start */
            unlinkMsgQueueForICC(name);
/* SPR 4915 : End */
            if (errno == ENOENT)
            {
                // Port for ERROR 
            }
        }

        {
            const char qName[] = "/ULContainerTempNackQ_";
            char name[33] = {0};
            sprintf(name, "%s%d",qName , i);
/* SPR 4915 : Start */
            unlinkMsgQueueForICC(name);
/* SPR 4915 : End */
            if (errno == ENOENT)
            {
                // Port for ERROR 
            }
        }
    }
}

/*****************************************************************************
 * Function Name  : initComEncoderQueue
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the encoder queue
 *****************************************************************************/
void initComEncoderQueue ( SInt32 *qId, UInt8 encoderContainerCount, InternalCellIndex internalCellIndex)
{
    const char qName[] = "/EncoderQueue_Container_";
    /* SPR 9750 Fix Start */
    char name[40] = {0};
    /* SPR 9750 Fix End */
    sprintf(name, "%s%d",qName , encoderContainerCount);
    sprintf(name, "%s%s",name , "Thread_");
    sprintf(name, "%s%d",name , internalCellIndex);

    *qId = initMsgQueueForICC(name, O_RDWR|O_CREAT);
    if (-1 == *qId)
    {
        ltePanic(" Failure in opening EncoderQueue, Error no : %d\n",errno );

    }
}

/*****************************************************************************
 * Function Name  : closeEncoderQueue
 * Inputs         : Number of containers
 * Outputs        : None
 * Returns        : None
 * Description    : Close the Encoder queue
 *****************************************************************************/
void closeEncoderQueue(UInt32 max_encoder_containers)
{
		UInt32 encoderContainerCount =0 ;
		for (encoderContainerCount=0; encoderContainerCount < max_encoder_containers;encoderContainerCount++)
		{
				const char qName[] = "/EncoderQueue_Container_";
				/* SPR 9750 Fix Start */
				char name[40] = {0};
				/* SPR 9750 Fix End */
				sprintf(name, "%s%d",qName , encoderContainerCount);

				/* SPR 4915 : Start */    
				unlinkMsgQueueForICC(name);
				/* SPR 4915 : End */
				if (errno == ENOENT)
				{
						// Port for ERROR
				}
		}
}
#endif 

/****************************************************************************
 * Function Name  : initComRRCAndOAMInterface
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API will create sockets for receiving/transmitting data
 *                  to/from RRC and OAM.
 ****************************************************************************/
/* + SPR 17439 */
void initComRRCAndOAMInterface(void)
/* - SPR 17439 */    
{
    /* Socket creation for Receiving data from RRC Layer */
    SockAddr rx_addr;
    struct hostent *host_p;
    host_p= (struct hostent *) gethostbyname_wrapper((char *)macIPAddress_g);
    if((rxSockFD_g = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("creating socket error in %s",__FUNCTION__);
    }
    /* SPR#706 Changes Start */
    if(host_p == NULL)
    {
        ltePanic( "Invalid Address to resolve in %s",__FUNCTION__);
        /*cov 26293*/
        exit(0);
        /*cov 26293*/
    }
    /* SPR#706 Changes End */

    rx_addr.sin_family = AF_INET;
    rx_addr.sin_port = htons_wrapper(rxPortMAC_g);
rx_addr.sin_addr = *((struct in_addr *)host_p->h_addr);
/* SPR 21001 Changes end */
    bzero(&(rx_addr.sin_zero),8);
    if ((bind(rxSockFD_g, (struct sockaddr *)&rx_addr, sizeof(rx_addr))) == -1)
    {
        ltePanic ("Unable to bind the receiver socket with RRC.\n");
        exit(0);
    }

    /*Coverity 99393 Fix Start*/
    SInt32 flags = 0; 
    if((flags = fcntl2_wrapper(rxSockFD_g, F_GETFL)) < 0)
    {
        ltePanic ("fcntl failed for receiver socket with L2.\n");
    }
    /*Coverity 99393 Fix End*/

    flags |= O_NONBLOCK;
    /*SPR 4017 PH2 Start*/
    if (-1 == fcntl3_wrapper(rxSockFD_g, F_SETFL, flags))
    {
        ltePanic ("fcntl failed for receiver socket with RRC.\n");
    }
    /*SPR 4017 PH2 End*/

    /* Socket creation for Transmission of data to RRC Layer */
    if ((txSockFD_g = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic(" Socket creation failed.\n");
        exit(0);
    }

    tx_addr_g.sin_family = AF_INET;
    tx_addr_g.sin_port = htons_wrapper(txPortMacRRC_g);
    if (Inet_aton_wrapper(rrcIPAddress_g,&tx_addr_g.sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }

    /* Socket creation for Transmission of data to OAM Layer */
    if ((txOAMSockFD_g = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic(" Socket creation failed for Tx to OAM.\n");
        exit(0);
    }

    txOAM_addr_g.sin_family = AF_INET;
    txOAM_addr_g.sin_port = htons_wrapper(txPortMacOAM_g);
    if (Inet_aton_wrapper( oamIPAddress_g,&txOAM_addr_g.sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }
    /*SPR 20863 FIX*/
#ifndef UE_SIM_TESTING
    /* Socket creation for Transmission of data to RRM Layer */
    if ((txRRMSockFD_g = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic(" Socket creation failed for Tx to OAM.\n");
        exit(0);
    }

    txRRM_addr_g.sin_family = AF_INET;
    txRRM_addr_g.sin_port = htons_wrapper(txPortRRM_g);
    if (Inet_aton_wrapper( rrmIPAddress_g,&txRRM_addr_g.sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }
#endif
    /*SPR 20863 FIX*/
}

/****************************************************************************
 * Function Name  : initComPhySocketInterface()
 * Inputs         : void
 * Outputs        : 
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This API is used to open a Non Blocking Sockets Rx/Tx
                    between the MAC layer and the Physical Layer.
 ****************************************************************************/
/* CA Changes start */
#ifndef FLEXRAN
void initComPhySocketInterface(InternalCellIndex cellIndex)
{
    if (( phyRxSockFD_g[cellIndex] = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("Error while opening the receiver socket.\n");
        exit(0);
    }
    phyRxAddr_g[cellIndex].sin_family = AF_INET;
    phyRxAddr_g[cellIndex].sin_port = htons_wrapper(phyRxPort_g[cellIndex]);

    if (Inet_aton_wrapper(macIPAddress_g,&phyRxAddr_g[cellIndex].sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }
#ifndef LAYER2_PHYSIM_THREAD
    /* +COVERITY 10008 */
    if(fcntl3_wrapper(phyRxSockFD_g[cellIndex],F_SETFL,O_NONBLOCK) == -1)
    {
        ltePanic("Error while setting file status flag \n");
        exit(0);
    }
    /* -COVERITY 10008 */
    if ((bind_wrapper(phyRxSockFD_g[cellIndex],
                    (const SockAddr *)&phyRxAddr_g[cellIndex],
                    sizeof(phyRxAddr_g[cellIndex]))) 
            == -1)
    {
        ltePanic ("Unable to bind the receiver socket with PHY.\n");
        exit(0);
    }
#endif 
    setFd( phyRxSockFD_g[cellIndex]);
#ifdef DL_UL_SPLIT_TDD
  /*Scoket Creation for data reception on UL thread*/
  if (( phyRxSockUlFD_g[cellIndex] = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("Error while opening the receiver socket for TDD UL thread.\n");
        exit(0);
    }

    phyRxAddrUl_g[cellIndex].sin_family = AF_INET;
    phyRxAddrUl_g[cellIndex].sin_port = htons_wrapper(phyRxPortUl_g[cellIndex]);

    if (Inet_aton_wrapper(macIPAddress_g,&phyRxAddrUl_g[cellIndex].sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }
    if(fcntl3_wrapper(phyRxSockUlFD_g[cellIndex],F_SETFL,O_NONBLOCK) == -1)
    {
        ltePanic("Error while setting file status flag \n");
        exit(0);
    }
    if ((bind_wrapper(phyRxSockUlFD_g[cellIndex],(const SockAddr *)&phyRxAddrUl_g[cellIndex],
            sizeof(phyRxAddrUl_g[cellIndex]))) == -1)
    {
        ltePanic ("Unable to bind the receiver socket with PHY for TDD UL thread.\n");
        exit(0);
    }


#endif
    /* Socket creation for Transmission of data to PHY Layer */
    if ((phyTxSockFD_g[cellIndex] = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic(" Socket creation failed.\n");
        exit(0);
    }
    phyTxAddr_g[cellIndex].sin_family = AF_INET;
    phyTxAddr_g[cellIndex].sin_port = htons_wrapper(phyTxPort_g[cellIndex]);
    
    if (Inet_aton_wrapper(phyIPAddress_g[cellIndex],&phyTxAddr_g[cellIndex].sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }
    /* +COVERITY 10008 */
    if(fcntl3_wrapper(phyTxSockFD_g[cellIndex],F_SETFL,O_NONBLOCK) == -1)
    {
        ltePanic("Error while setting file status flag \n");
        exit(0);
    }
    /* -COVERITY 10008 */
    /*Control info socket creation*/
    if (( phyControlSockFD_g[cellIndex] = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("Error while opening the control info socket.\n");
        exit(0);
    }
    phyControlAddr_g[cellIndex].sin_family = AF_INET;

    phyControlAddr_g[cellIndex].sin_port = htons_wrapper(phyControlRxPort_g[cellIndex]);
    if (Inet_aton_wrapper(macIPAddress_g,&phyControlAddr_g[cellIndex].sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }
#ifndef LAYER2_PHYSIM_THREAD
    /* +COVERITY 10008 */
    if(fcntl3_wrapper(phyControlSockFD_g[cellIndex],F_SETFL,O_NONBLOCK) == -1)
    {
        ltePanic("Error while setting file status flag \n");
        exit(0);
    }
    /* -COVERITY 10008 */
    if ((bind_wrapper(phyControlSockFD_g[cellIndex],
                    (const SockAddr *)&phyControlAddr_g[cellIndex],
                    sizeof(phyControlAddr_g[cellIndex]))) 
            == -1)
    {
        ltePanic ("Unable to bind the receiver socket with PHY.\n");
        exit(0);
    }
    setFd( phyControlSockFD_g[cellIndex] );
#endif
    phyControlTxAddr_g[cellIndex].sin_family = AF_INET;
    if (Inet_aton_wrapper(phyIPAddress_g[cellIndex],&phyControlTxAddr_g[cellIndex].sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }

    /* CA Changes end */
    /* Frame Ind socket creation*/
    if (( phyFrameIndSockFD_g = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("Error while opening the control info socket.\n");
        exit(0);
    }
    phyFrameIndAddr_g.sin_family = AF_INET;
    phyFrameIndAddr_g.sin_port = htons_wrapper(phyFrameIndPort_g);
    if (Inet_aton_wrapper(macIPAddress_g,&phyFrameIndAddr_g.sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }
    /* keeping this socket blocking */
    /* +COVERITY 10008 */
    if(fcntl3_wrapper(phyFrameIndSockFD_g,F_SETFL,O_NONBLOCK) == -1)
    {
        ltePanic("Error while setting file status flag \n");
        exit(0);
    }
    /* -COVERITY 10008 */
    if ((bind_wrapper(phyFrameIndSockFD_g,(const SockAddr *)&phyFrameIndAddr_g,sizeof(phyFrameIndAddr_g))) 
            == -1)
    {
        ltePanic ("Unable to bind the receiver socket with PHY.\n");
        exit(0);
    }
}
#endif
/****************************************************************************
 * Function Name  : dumpDLStatsToFile
 * Inputs         : Index of statsInfoArr to be written to file
 * Outputs        :
 * Returns        : 
 * Description    : The function writes down the data of array whose index is
 *                  provided as an argument.
 ****************************************************************************/
inline  void dumpDLStatsToFile( UInt8 statsInfoArrIndex,
                                      UInt32 statsTickIndex,dlStatsInfo * statsDLInfoArr_p
                                    )
{
    /*SPR 15909 fix start*/
    tickType_t tickCounter;
    /*SPR 15909 fix end*/
    UInt32 rbIndex;
    Char8 filename[100];
    static UInt16 dlFileNum=0;
    FILE *dlStatsDumpFile_g            =PNULL;    

    /* +COVERITY 10256 */
    snprintf(filename,sizeof(filename),"downlinkstatsInfo%d",dlFileNum++);
    /* +COVERITY 10256 */

    dlStatsDumpFile_g = fopen(filename,"w");
    /* SPR#706 Changes Start */
    if (dlStatsDumpFile_g == PNULL)
    {
        LTE_LOG(LOG_WARNING,"Error opening file %s:",filename);
        pError("\n File Sys Error:");
        return;
    }
    /* SPR#706 Changes End */   
    
    fprintf(dlStatsDumpFile_g,"<CURRENT_TICK><UEID><UE_PRIORITY><UE_QUEUE_LOAD>"
        "<TRANSMISSION_MODE><DCI_FORMAT><AGGREGATION_LEVEL>"
        "<RA_POLICY><RBS><PRB_BITMAP><TBS><MODULATION_SCHEME_CODEWOED0>"
        "<MODULATION_SCHEME_CODEWORD1><LAST_SCHEDULED_TICK>\n");

    for (tickCounter=0; (tickCounter <= statsTickIndex && tickCounter < MAX_STATS_TICKS);tickCounter++)
    {
        UInt32 lastRBIndex = statsDLInfoArr_p[statsInfoArrIndex].statsDLInfoRB[tickCounter].currentRBIndex;
        for (rbIndex =0 ; rbIndex < lastRBIndex; rbIndex++)
        {
            dlUePRBTTIInfo *rbAllocPerTTI_p = &statsDLInfoArr_p[statsInfoArrIndex].statsDLInfoRB[tickCounter].rbAllocPerTTI[rbIndex];
                        /* SPR 15909 fix start */
                        /* CID 111675 Fix Start */
            fprintf(dlStatsDumpFile_g,"%llu %5u %5u   %5u %5u %5u %5u %5u %5u %5x %5u %5u %5u %llu\n"
                        /* CID 111675 Fix End */
                        /* SPR 15909 fix end */
                        ,rbAllocPerTTI_p->currentTick
                        ,rbAllocPerTTI_p->ueId
                        ,rbAllocPerTTI_p->uePriority
                        ,rbAllocPerTTI_p->ueQueueLoad
                       /* ,rbAllocPerTTI_p->typeOfAllocation*/
                        ,rbAllocPerTTI_p->transmissionMode
                        ,rbAllocPerTTI_p->dciFormat
                        ,rbAllocPerTTI_p->aggregationLevel
                        ,rbAllocPerTTI_p->raPolicy
                        ,rbAllocPerTTI_p->numOfRb
                        ,rbAllocPerTTI_p->rbBitMap
                        ,rbAllocPerTTI_p->numberOfTBs
                        ,rbAllocPerTTI_p->modulationSchemeCodeword0
                        ,rbAllocPerTTI_p->modulationSchemeCodeword1
                        ,rbAllocPerTTI_p->lastScheduledTick);
        }
        statsDLInfoArr_p[statsInfoArrIndex].statsDLInfoRB[tickCounter].currentRBIndex = 0;
    }
    fClose(dlStatsDumpFile_g);
}
/****************************************************************************
 * Function Name  : dumpULStatsToFile
 * Inputs         : Index of statsInfoArr to be written to file
 * Outputs        :
 * Returns        : 
 * Description    : The function writes down the data of array whose index is
 *                  provided as an argument.
 ****************************************************************************/
inline void dumpULStatsToFile( UInt8 statsInfoArrIndex,
                                      UInt32 statsTickIndex,ulStatsInfo * statsULInfoArr_p,UInt32 invalid_ue_id
                                    )
{
    /*SPR 15909 fix start*/
    tickType_t tickCounter;
    /*SPR 15909 fix end*/
    UInt32 rbIndex;
    Char8 filename[100];
    static UInt16 ulFileNum=0;

    FILE *ulStatsDumpFile_g            =PNULL;    
    /* COVERITY 10257 */
    snprintf(filename,sizeof(filename),"uplinkStatsInfo%d", ulFileNum++);
    /* COVERITY 10257 */
    ulStatsDumpFile_g = fopen(filename,"w");
    
    /* SPR#706 Changes Start */
    if (ulStatsDumpFile_g == PNULL)
    {
        LTE_LOG(LOG_WARNING,"Error opening file %s:",filename);
        pError("\n File Sys Error:");
        return;
    }
    /* SPR#706 Changes End */   
    
    fprintf(ulStatsDumpFile_g,"TTI - Current TTI \nULGrantRequestType : - \n\t\t 0 - SR ULGRANT REQ MSG \n\t\t 1 - CR ULGRANT REQ MSG \n\t\t 2 - CQI WITH DATA ULGRANT REQ MSG \n\t\t 3 - CQI WITHOUT DATA ULGRANT REQ MSG \n\t\t 4 - NEW ULGRANT REQ MSG \n\t\t 5 - CRC NACK UL GRANT REQ MSG \n\t\t 6 - PRACH Resources\n\t\t 7 - PUCCH Reports ( Resource Allocated to Periodic CQI/RI, SR Report, DL Ack/Nack: One slot combination from start and end of Resource Map i.e. 2* start). \n\n BSR  - NET BSR ALL LCG Group,\n start- Starting PRB Index from the Resource Map.\n RBS  - Number of RBs Allocated \n RIV  - Allocated PRB Bitmap. \n Mod  - MODULATION SCHEME  2 - QPSK,  4 - 16QAM, 6 - 64QAM\n SCH TTI  - SubFrame in which UE was scheduled before this tick \n\n\n");
    
    fprintf(ulStatsDumpFile_g,"  <TTI> <UEID> <PRI>    <NET BSR> <ULGrantRequestType> <Start> <RBS>  <RIV>   <MOD> <SCH - TTI> \n");
    
    for (tickCounter=0; (tickCounter <= statsTickIndex && tickCounter < MAX_STATS_TICKS);tickCounter++)
    {
        UInt32 lastRBIndex =statsULInfoArr_p[statsInfoArrIndex].statsULInfoRB[tickCounter].currentRBIndex;
        for (rbIndex = 0 ; rbIndex < lastRBIndex ;rbIndex++)
        {
            ulUePRBTTIInfo *rbAllocPerTTI_p = &statsULInfoArr_p[statsInfoArrIndex].statsULInfoRB[tickCounter].rbAllocPerTTI[rbIndex];         
            if ( invalid_ue_id != rbAllocPerTTI_p->ueId )
            {
                            /* SPR 15909 fix start */
                            /* CID 111691 Fix Start */
                fprintf(ulStatsDumpFile_g,"%llu  %5u %5u %13u %15u %10u %7u   %5x  %5u   %llu\n"   
                            /* CID 111691 Fix End */
                            /* SPR 15909 fix end */
                            ,rbAllocPerTTI_p->currentTick
                            ,rbAllocPerTTI_p->ueId
                            ,rbAllocPerTTI_p->uePriority
                            ,rbAllocPerTTI_p->ueQueueLoad
                            ,rbAllocPerTTI_p->typeOfAllocation
                            ,rbAllocPerTTI_p->startRBIndex
                            ,rbAllocPerTTI_p->numOfRb
                            ,rbAllocPerTTI_p->rbBitMap
                            ,rbAllocPerTTI_p->modulationScheme
                            ,rbAllocPerTTI_p->lastScheduledTick);
            }
            else
            {
                            /* SPR 15909 fix start */
                            /* CID 111694 Fix Start */
                 fprintf(ulStatsDumpFile_g,"%llu  %5s %5u %13u %15u %10u %7u   %5u  %5u   %llu\n"  
                            /* CID 111694 Fix End */
                            /* SPR 15909 fix end */
                             ,rbAllocPerTTI_p->currentTick
                             ,"N.A"
                             , 0 /* UE Priority*/
                             , 0 /* ueQueueLoad */
                             , rbAllocPerTTI_p->typeOfAllocation
                             , rbAllocPerTTI_p->startRBIndex
                             , rbAllocPerTTI_p->numOfRb
                             , 0 /*rbBitMap*/
                             , 0 /*Modulation Scheme*/
                             ,rbAllocPerTTI_p->lastScheduledTick);
             }
        }
        statsULInfoArr_p[statsInfoArrIndex].statsULInfoRB[tickCounter].currentRBIndex = 0;
    }
    fClose(ulStatsDumpFile_g);
}

/****************************************************************************
* Function Name  : initMacToRrmMsgQueue
* Inputs         : Id: Layer 2 instance number
* Outputs        :
* Returns        : Fd of created message queue
* Description    : The function creates message queues for Tx (MAC to RRM) and
*                    returns file discriptor
*****************************************************************************/
MQD_T initMacToRrmMsgQueue (UInt32 Id)
{
    char qName[QNAME_SIZE];
    snprintf(qName,sizeof(qName),"/MacToRrm%d",Id);
    return lteInitMessageQTx(qName);
}
/****************************************************************************
* Function Name  : initRrmToMacMsgQueue
* Inputs         : Id: Layer 2 instance number
* Outputs        :
* Returns        : Fd of created message queue
* Description    : The function creates message queues for Rx (RRM to MAC) and
*                    returns file discriptor
*****************************************************************************/
MQD_T initRrmToMacMsgQueue (UInt32 Id)
{
    char qName[QNAME_SIZE];
    snprintf(qName,sizeof(qName),"/RrmToMac%d",Id);
    return lteInitMessageQRx(qName);
}

/****************************************************************************
* Function Name  : initHptoLpIndicationQueue
* Inputs         : None
* Outputs        :
* Returns        : Fd of created message queue
* Description    : The function creates message queues for sending periodic 
*                  report indication to Lp thread from Hp thread.
****************************************************************************/
/* + SPR 17439 */
MQD_T initHptoLpIndicationQueue(void)
/* - SPR 17439 */    
{
  MQD_T periodicReportFd = 0;
    char qName[] = "/HptoLPIndicationQ";
    if(0>lteMessageQUnlink(qName))
    {
        pError("mq_unlink fail\n");
    }
    periodicReportFd  = lteInitMessageQTx(qName);
   return periodicReportFd;
}
#ifdef FLEXRAN
static unsigned short int csum_fold(unsigned int sum)
{
    while ( sum >> 16 ) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return ~sum;
}

static unsigned short ip_fast_csum(uint8_t *iph, unsigned int ihl)
{
   unsigned int *word = (unsigned int *) iph;
   unsigned int *stop = word + ihl;
   unsigned int csum;
   int carry;

   csum = word[0];
   csum += word[1];
   carry = (csum < word[1]);
   csum += carry;

   csum += word[2];
   carry = (csum < word[2]);
   csum += carry;

   csum += word[3];
   carry = (csum < word[3]);
   csum += carry;

   word += 4;
   do {
           csum += *word;
           carry = (csum < *word);
           csum += carry;
           word++;
   } while (word != stop);

   return csum_fold(csum);
}

static uint8_t *build_ip_header(uint8_t *ptr, uint32_t src_ip, uint32_t dst_ip, uint16_t len)
{
    struct iphdr1* iphdr = (struct iphdr1*)ptr;
    iphdr->ver_hl = 0x45;
    iphdr->tos = 0;
    iphdr->len = htons(len);
    iphdr->id = 0;
    iphdr->offset = 0x0000;
    iphdr->ttl = 0xff;
    iphdr->protocol = 0x11;
    iphdr->checksum = 0x0000;
    iphdr->src = htonl(src_ip);
    iphdr->dst = htonl(dst_ip);

    iphdr->checksum = ip_fast_csum(ptr, 5);

    return (uint8_t*)(iphdr + 1);
}

extern tickType_t currentTick_g[MAX_NUM_CELL];
static uint8_t *build_udp_header(uint8_t *ptr, uint16_t src_port, uint16_t dst_port, uint16_t len)
{
    struct udphdr1 *udphdr = (struct udphdr1*)ptr;

    udphdr->source_port = htons(src_port);
    udphdr->dest_port = htons(dst_port);
    udphdr->len = htons(len);
	//show hsfn in wireshark log
    udphdr->checksum = htons((currentTick_g[0]/10240)&0xFFFF);//0x0000;

    return (uint8_t*)(udphdr + 1);
}

void do_tap_fun(void* data, uint32_t len, InternalCellIndex cellId)
{
    if(0 == flexranFapiLoggingEnable_g)
    {
        return;
    }

    uint32_t total_len = len + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN;

        if (len >= DO_TAP_BUF_SIZE)
        {
                printf("do_tap_fun ERROR: invalid total_len = %d, len = %d\n", total_len, len);
                return;
        }

    uint8_t* ptr = build_ip_header(g_do_tap_buf+ETHER_HDR_LEN1, local_ipaddr, dst_ipaddr, total_len - ETHER_HDR_LEN1);
    build_udp_header(ptr, (40001 + cellId + 10*instanceL2_g), (8888 ), total_len - ETHER_HDR_LEN1 - IP_HDR_LEN);
    memcpy(g_do_tap_buf + ETHER_HDR_LEN1 + IP_HDR_LEN + UDP_HDR_LEN, data, len);

    if (raw_sock != -1) {
        struct iphdr1* iphdr = (struct iphdr1*)(g_do_tap_buf+ETHER_HDR_LEN1);
        iphdr->src = local_ipaddr;
        iphdr->dst = dst_ipaddr;

        iphdr->checksum = ip_fast_csum((uint8_t*)iphdr, 5);

        struct sockaddr_in addr;
        addr.sin_addr.s_addr = dst_ipaddr;
        addr.sin_family = AF_INET;
        addr.sin_port = ntohs(8888);
        sendto(raw_sock, (void*)iphdr, ntohs(iphdr->len), 0, (struct sockaddr *)&addr, sizeof(addr));
    }
}

int open_raw_sock(const char* src_ip, const char* dst_ip)
{
    raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_sock == -1)
        return -1;

    char on = 1;
    if (setsockopt(raw_sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
    {
        raw_sock = -1;
        return -1;
    }

    local_ipaddr = (uint32_t)inet_addr(src_ip);
    dst_ipaddr = (uint32_t)inet_addr(dst_ip);

    return 0;
}

void initWiresharkLoggingInterfaceLocal(InternalCellIndex internalCellIndex)
{
    if (( flexranWiresharkFd_g[internalCellIndex] = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("Error while opening the receiver socket.\n");
        exit(0);
    }
    flexranTx_g[internalCellIndex].sin_family = AF_INET;
    flexranTx_g[internalCellIndex].sin_port = htons(8888);
}
#endif

/*****************************************************************************
 * Function Name  : prepareshmRegionName
 * Inputs         : Name, length, string, L2 Instance, cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : Prepare Shared memory region name for logging
 *****************************************************************************/
void prepareshmRegionName(Char8 * shmRegionName, UInt32 name_len,Char8 *str, SInt32 instanceL2, 
        InternalCellIndex internalCellIndex)
{
    snprintf(shmRegionName, name_len, "%s_I%d_C%d", str, instanceL2, 
            internalCellIndex);
}

/*Coverity 26339 Fix Start*/
/*****************************************************************************
 * Function Name  : prepareLoggerFileName
 * Inputs         : file Name, string, L2 Instance, size, cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : Prepare logger file name for logging
 *****************************************************************************/
void prepareLoggerFileName(Char8 * logger_file_name, Char8 *str, SInt32 instanceL2, UInt32 size, 
        InternalCellIndex internalCellIndex)
{
    snprintf((char *)logger_file_name,size,"%s%d_C%d",str , instanceL2, 
            internalCellIndex);
}
/*Coverity 26339 Fix End*/

/*****************************************************************************
 * Function Name  : preparePdcpShmRegionName
 * Inputs         : Name, length, string, L2 Instance, cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : Prepare PDCP Shared memory region name for logging
 *****************************************************************************/
void preparePdcpShmRegionName(Char8 * shmRegionName, UInt32 name_len, Char8 *str, SInt32 instanceL2)
{
    snprintf(shmRegionName, name_len, "%s_I%d", str, instanceL2);
}

/*****************************************************************************
 * Function Name  : prepareFileName
 * Inputs         : Shm Name, length, string, cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : Prepare shared memory name for logging
 *****************************************************************************/
void prepareFileName(Char8 * shmRegionName, UInt32 name_len, Char8 *str, 
        InternalCellIndex internalCellIndex)
{
    snprintf(shmRegionName, name_len, "%s_C%d", str, internalCellIndex);
}
/* CA Logging Changes End */

/* +SPR_10381 Changes */
/* Porting changes - start */
/*****************************************************************************
 * Function Name  : prepareEncoderLoggerFileName
 * Inputs         : file Name, length, string, cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : Prepare encoder logger file name for logging
 *****************************************************************************/
void prepareEncoderLoggerFileName(Char8 * logger_file_name, UInt32 name_len, Char8 *str,
        InternalCellIndex internalCellIndex)
{
    snprintf((char *)logger_file_name, name_len, "%s_C%d", str, internalCellIndex);
}
/* Porting changes - end */
/* -SPR_10381 Changes */

/****************************************************************************
 * Function Name  : getMACBuildConfiguration
 * Inputs         :  buff_p , msgLen_p
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : prints the MAC build info and returns it to OAM 
 ****************************************************************************/
UInt16 getMACBuildConfiguration(UInt8 *buff_p, UInt16 *msgLen_p)
{
    FILE *buildFile_p;
    Char8 *buildStamp_p = PNULL;
/* SPR 20636 Changes Start*/
    size_t size = 0; 
/* SPR 20636 Changes End*/
    /* + Coverity 32403, 32407 */
    /* UInt16 retVal = 0; */
    /* - Coverity 32403, 32407 */
    UInt16 i = 0;

    buildFile_p = fopen("buildStamp","r");
    if(buildFile_p == PNULL)
    {
        LTE_LOG(LOG_WARNING,"Product Build Version File Not Found \n");
        return 0;
    }
    /* + Coverity 32403, 32407 */
    if((getline(&buildStamp_p, &size, buildFile_p)) != -1)
    /* - Coverity 32403, 32407 */
    {
        printf("Product Build: %s\n",buildStamp_p);
        while(buildStamp_p[i] != '\n') {
            buff_p[*msgLen_p] = buildStamp_p[i];
            i++;
            *msgLen_p += 1; 
        }    
        buff_p[*msgLen_p] = '\0';
        *msgLen_p += 1; 
    }
    else
    {
        printf("buildStamp file doesn't exist in the current path\n");
    }
    if(buildStamp_p)
        free(buildStamp_p);
    
    fClose(buildFile_p);
    return 1;
}

/*****************************************************************************
 * Function Name  : fcntl_f_setfl
 * Inputs         : Socket FD
 * Outputs        : None
 * Returns        : None
 * Description    : Set socket control information
 *****************************************************************************/
void fcntl_f_setfl(SInt32 phyRxSockFD)
{
	if (fcntl3_wrapper( phyRxSockFD, F_SETFL, 0) == -1)
	{
		ltePanic(" Error while setting F_SETFL\n");
	}
}

/* + coverity 55797 */
/* - coverity 55797 */
/*SPR 16855 +-*/
/******************************************************************************
 * Function Name: initMsgQFdFrmHpThread
 * Inputs       : None
 * Outputs      : Nome
 * Return       : Message queue FD
 * Description  : It creates message queue between LP and HP thread and 
 *                returns its FD.
******************************************************************************/
/* + SPR 17439 */
MQD_T initMsgQFdFrmHpThread(void)
/* - SPR 17439 */    
{
    MQD_T recvMsgFrmHPThdFd = 0;

    if(0>lteMessageQUnlink( RECV_MSG_FRM_HP_THREAD_Q_NAME ))
    {
        pError("mq_unlink fail\n");
    }
    recvMsgFrmHPThdFd = lteInitMessageQTx( RECV_MSG_FRM_HP_THREAD_Q_NAME );
    
    return recvMsgFrmHPThdFd;
}
/*SPR 16855 +-*/
/****************************************************************************
* Function Name  : initStats
* Inputs         : none
* Outputs        : none
* Returns        : none
* Description    : Increments the uplink tick index when all the RB info is
*                  filled by the fillULstatsinfo function for a tick.
*
****************************************************************************/
/* + SPR 17439 */
UInt32 initStats(void)
/* - SPR 17439 */
{
/* SPR 5022 Change Start */
    SInt32 shmId;
    /* CA Stats Changes Start */
    InternalCellIndex cellIndex = 0;
    /* CA Stats Changes End */

    /* Clean the Shared memory */
    shmId = shmget_wrapper(STATS_SHM_KEY + instanceL2_g , 0, 0666);
    /*START:SR:CSR:00024693:SPR5275*/
    if(shmId >= 0)
    {
        shmctl_wrapper_ipc_rmid(shmId);
    }
    /*END:SR:CSR:00024693:SPR5275*/
    /* Get the shared memory ID */
    gMacStats.shmId = shmget_wrapper(STATS_SHM_KEY + instanceL2_g , sizeof(LteMacStats),
                            STATS_SHM_FLAGS);

/* SPR 5022 Change End */

    if (0 > gMacStats.shmId)
    {
        pError("SHM get failed");
        return 0;
    }

    /* Map Shared memory */
    gMacStats.pStats = shmat_wrapper(gMacStats.shmId, NULL, 0);
    if ((void *)-1 == gMacStats.pStats)
    {
        pError("SHM attach failed");
        return 0;
    }

    /* Initialize shared memory */
    memSet(gMacStats.pStats, 0, sizeof(LteMacStats));
    /* CA Stats Changes Start */
    for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
    {
#if defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
        clock_gettime_real_time(
                &gMacStats.pStats->perfStatsSplit[cellIndex][DL_OVERSHOOT_STATS].
                lastSfiTimeStamp);
        clock_gettime_real_time(
                &gMacStats.pStats->perfStatsSplit[cellIndex][UL_OVERSHOOT_STATS].
                lastSfiTimeStamp);
#else
        clock_gettime_real_time(
                &gMacStats.pStats->perfStats[cellIndex].lastSfiTimeStamp);
#endif
    }
    /* CA Stats Changes End */
    return 1;
}
