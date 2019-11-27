/************************************************************************
 * 			SYSTEM INCLUDES					*
 ***********************************************************************/
#define _GNU_SOURCE
#include <sched.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>      
#include <arpa/inet.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <math.h>
#include <stdarg.h>
/* SPR 20526 Changes Start*/
#include "lteTypes.h"
/* SPR 20526 Changes End*/

/*************************************************************************
 * 			PRIVATE DEFINITIONS				 *
 *************************************************************************/
/*+ Colour Coding */
#define CLR_BLACK   0
#define CLR_RED     1
#define CLR_GREEN   2
#define CLR_YELLOW  3
#define CLR_BLUE    4
#define CLR_MAGENTA 5
#define CLR_CYAN    6
#define CLR_WHITE   7

#define ATTR_NONE       0
#define ATTR_BOLD       1
#define ATTR_DIM        2
#define ATTR_UNDERLINE  4
#define ATTR_BLINK      5
#define ATTR_REVERSE    7
/*- Colour Coding */
#define SEQ_MISMATCH_VALIDATION
#define GTP_HDR 5

/*+ Packet Related */
#define PACKET_SIZE 64000
#define ADAPTER_HDR 5
#define HEADER_LEN 8
#define MAX_PAYLOAD_SIZE 64000
#define PNULL NULL
/*- Packet Related */
#define MAX_SECTORS 8
/* +- SPR 18268 */
#define MAX_MULTI_UE  260 
/* +- SPR 18268 */
#define MAX_LC_ID 8
#define MAX_PAYLOAD_SIZE 64000
#define TIME_TO_BURST_IN_SECONDS 10
#define BURST_FLAG 1

#define MAX_PROF 5
/*+ RX_Throughput Calculation*/
/* SPR 20441 Changes Start*/
#define CALC_LOAD_RX(x,y)    ueDataFlow_g[x][y].rxData *=1000;\
/* SPR 20441 Changes End*/\
                           ueDataFlow_g[x][y].rxData *=8;\
                           ueDataFlow_g[x][y].rxData /= diff
/*- RX_Throughput Calculation*/

/*+ TX Throughput Calculation*/
/* SPR 20441 Changes Start*/
#define CALC_LOAD_TX(x,y)    ueDataFlow_g[x][y].txData *=1000;\
/* SPR 20441 Changes End*/\
                           ueDataFlow_g[x][y].txData *=8;\
                           ueDataFlow_g[x][y].txData /= diff
/*- TX Throughput Calculation*/

/*+ SPR 2987 Fix*/
#define CALC_ERROR_RATE(x,y)  (ueDataFlow_g[x][y].rxPkts)?(((UDouble32)ueDataFlow_g[x][y].rxMiss) / (UDouble32)(ueDataFlow_g[x][y].rxPkts + ueDataFlow_g[x][y].rxMiss)) *100.0000f:0.00f
/*- SPR 2987 Fix*/
#define CALC_ERROR_RATE_PER_DISPLAY(x,y)  (ueDataFlow_g[x][y].rxPkts-ueDataFlow_g[x][y].rxOldPkt)?(((UDouble32)ueDataFlow_g[x][y].rxMiss_per_display) / (UDouble32)(ueDataFlow_g[x][y].rxPkts-ueDataFlow_g[x][y].rxOldPkt+ueDataFlow_g[x][y].rxMiss_per_display)) *100.0000f:0.00f

/*+ Printing of Throughput and relative Params*/
#define MbPS_LOAD(a,x,y)  if( x%2 == 0)\
			{\
			cprintf(CLR_MAGENTA,ATTR_UNDERLINE,-1,"SEC_ID: %d UEID: %02x LC-ID %d\n",a,x,y);\
			printf ("%c[%dm", 27, 0);\
			}\
			else \
			{\
			cprintf(CLR_BLUE,ATTR_UNDERLINE,-1,"SEC_ID: %d UEID: %02x LC-ID %d\n",a,x,y);\
			printf ("%c[%dm", 27, 0);\
			}\
		      CALC_LOAD_RX(x,y);\
                      printf("RX: ThrPut: %.3f  MissSeqperDisp:%2ld  ErrorPerDisp:   %.4f   PktsSinceLastDisp: %4ld  |  TotalPkts:%5ld  MissSeq:%08x  ErrorSession:%.4f  \n",ueDataFlow_g[x][y].rxData,ueDataFlow_g[x][y].rxMiss_per_display,CALC_ERROR_RATE_PER_DISPLAY(x,y),ueDataFlow_g[i_ueId][y].rxPkts-ueDataFlow_g[i_ueId][y].rxOldPkt,ueDataFlow_g[x][y].rxPkts,ueDataFlow_g[x][y].rxMiss,CALC_ERROR_RATE(x,y));\
                      ueDataFlow_g[x][y].rxMiss_per_display = 0;\
                     CALC_LOAD_TX(x,y);\
                      printf("TX: ThrPut: %.3f  NoOfLostTicks:%3d  LostTime:%9ld[ns]   PktsSinceLastDisp: %4ld  |  TotalPkts:%5ld \n",ueDataFlow_g[x][y].txData,missTickCount_g,missTimeCount_g,ueDataFlow_g[i_ueId][y].txPkts-ueDataFlow_g[i_ueId][y].txOldPkt,ueDataFlow_g[x][y].txPkts);\
                     ueDataFlow_g[x][y].rxData =0;ueDataFlow_g[x][y].txData = 0;missTickCount_g = 0;missTimeCount_g =0;ueDataFlow_g[x][y].txOldPkt = ueDataFlow_g[x][y].txPkts;ueDataFlow_g[x][y].rxOldPkt=ueDataFlow_g[x][y].rxPkts
/*- Printing of Throughput and relative Params*/

/*+ Printing Time */
#define PRINT_TIME()\
{\
    char buff[100];\
    time_t now = time (0);\
    strftime (buff, 100, "%d-%m-%Y %H:%M:%S", localtime (&now));\
    printf ("%s\n", buff);\
}
/*- Printing Time */
#define LTE_SET_U16BIT(p_buff, val)\
    do {\
        *(p_buff) = (UInt8)((val) >> 8) ;\
        *(p_buff + 1) = (UInt8)(val);\
    } while (0)

#define LTE_GET_U16BIT(p_buff)  (UInt16)(*(p_buff) << 8) | (UInt16)(*(p_buff + 1))
#define PREPARE_SENDBUFFER(sendBuffer, headerStart,packet_size) {\
    UInt8               version = 0, proto = 0;\
    UInt8 		ipHeaderLen = 0;\
    UInt8 		ipLenOffsetMsb = 0;\
    UInt8 		ipLenOffsetLsb = 0;\
    UInt8 		udpHLenMsb = 0;\
    UInt8 		udpHLenLsb = 0;\
    UInt8 		udpChecksumMsb = 0;\
    UInt8 		udpChecksumLsb = 0;\
    UInt8 		ipChecksumMsb = 0;\
    UInt8 		ipChecksumLsb = 0;\
    version = (sendBuffer[5] >> 4) &0xF; \
    if(version == 4)             \
    {                            \
        ipHeaderLen = 20;       \
        ipLenOffsetMsb = 2; \
        ipLenOffsetLsb = 3; \
        udpHLenMsb = 24;\
        udpHLenLsb = 25; \
        udpChecksumMsb = 26;\
        udpChecksumLsb = 27;\
        ipChecksumMsb = 10;\
        ipChecksumLsb = 11;\
        sendBuffer[headerStart+ipLenOffsetMsb] = \
        ((UInt16)packet_size>>8) & (0xFF);\
        sendBuffer[headerStart+ipLenOffsetLsb] = \
        ((UInt16)packet_size & 0xFF);  \
    }\
    else  \
    {     \
        proto = sendBuffer[11];    \
        ipHeaderLen = 40; \
        ipLenOffsetMsb = 4;\
        ipLenOffsetLsb = 5;\
        udpHLenMsb = 44; \
        udpHLenLsb = 45; \
        udpChecksumMsb = 46;\
        udpChecksumLsb = 47;\
        /* In case of IPv6 we fill only payload length */  \
        sendBuffer[headerStart+ipLenOffsetMsb] =  \
        ((UInt16)(packet_size - ipHeaderLen)>>8) & (0xFF); \
        sendBuffer[headerStart+ipLenOffsetLsb] = \
        ((UInt16)(packet_size - ipHeaderLen) & 0xFF); \
    }\
    sendBuffer[headerStart+udpHLenMsb] = \
        ((UInt16)(packet_size - ipHeaderLen )>>8) & (0xFF); \
    sendBuffer[headerStart+udpHLenLsb] = \
        ((UInt16)(packet_size - ipHeaderLen) & 0xFF); \
    if (proto == LTE_IP_TCP_PROTO) \
    {\
        if (version == 4)         \
        {                          \
            udpChecksumMsb = 36;   \
            udpChecksumLsb = 37;   \
        }\
        else  \
        {     \
            udpChecksumMsb = 56;    \
            udpChecksumLsb = 57;    \
        }\
    }\
    udpChecksum = udpCksumCalc(    \
            (UInt16)(packet_size - ipHeaderLen), \
            sendBuffer,headerStart+ipHeaderLen, version);\
    sendBuffer[headerStart+udpChecksumMsb] = \
        (udpChecksum >>8) & (0xFF); \
    sendBuffer[headerStart+udpChecksumLsb] =  \
        (udpChecksum & 0xFF);  \
    /* IP Checksum required in case of Ipv4 only */\
    if(version == 4) \
    {  \
        ipChecksum =  ipCksumCalc( \
                (UInt16) ipHeaderLen, \
                sendBuffer,\
                headerStart);\
        sendBuffer[headerStart+ipChecksumMsb] = \
             (ipChecksum >>8) & (0xFF);\
        sendBuffer[headerStart+ipChecksumLsb] = \
             (ipChecksum & 0xFF); \
    }\
}

/************************************************************************
 * 			TYPEDEFS					*
 ***********************************************************************/
/*+ Variables */
/* SPR 20526 Changes Start*/
/* Code Removed*/
/* SPR 20526 Changes End*/
/*- Variables */
/* Number of different packet sizes that can be used in single traffic 
 * profile seperated by comas */
#define MAX_PACKET_TYPE 3
#ifdef SUPPORT_64_UE                                            
/* LOAD DISTRIBUTION */
UInt32 totalLcCount       = 0;
/* LOAD DISTRIBUTION */
#endif


// RoHC test enhancements
#define TS_PERIOD 20
#define LTE_IP_UDP_PROTO 0x11
#define LTE_IP_TCP_PROTO 0x06

#define COUNTDOWN 1000
#define DSCP_COUNT 53
#define PUSH_COUNT 101
#define URGENT_COUNT 211
#define ACK_COUNT 307
#define ECN_COUNT 401

UInt8 TSandSNValueChange_g = 0;
UInt8 IPIDValueChange_g = 0; /* 1-Random, 2-Sequence 3-Byte Swapped */

/*- Variables */
#define MAX_PROFILES 10
/* SPR 20526 Fix Start */
#define MAX_UE  MAX_UE_SUPPORTED	
/* SPR 20526 Fix End */

UInt32 processSpsDLPacket(void *ipAddr);
typedef struct SpsProfile_t
{
    UInt16  pktSize;	      /* Packet Size */
    UInt8  interPktArvlTm;    /* Inter Packet Arrival Time */
    UInt16  repetition;	      /* Number of repetition */
    UInt8  *SendBuffer;
}SpsProfile_t;

typedef struct lcInfo_t
{
    UInt8	lc_id;
    UInt8	numPkt;
    UInt8	data;
    UInt32    packet_size;
    UInt8     *SendBuffer;
}lcInfo_t;
typedef struct ueInfo_t
{
    UInt8     num_lc;
    UInt16     ue_id;
    lcInfo_t  lcInfo[8]; 
}ueInfo_t;
typedef struct spsTraffic_t
{
    UInt8 	 data_present;
    /* +- SPR 18268 */
    UInt16    num_ue;
    /* +- SPR 18268 */
    ueInfo_t   ueInfo[MAX_UE];
}spsTraffic_t;

UInt8 lcId_g;

UInt16 g_offset_tracker;
spsTraffic_t spsTraffic[1000];
//#endif

/*+ Structures */ 			
typedef struct TgProfileInfoT
{
UInt32 packetSize[MAX_PROF];
UDouble32   throughput[MAX_PROF];
UInt32  varPacketSize[MAX_PROF];
UDouble32 totalNoOfPktVar[MAX_PROF];
UDouble32 fixedPktPerTick[MAX_PROF];
UInt32 remainingPkt[MAX_PROF];
UInt32 profileTimer;
UInt16 mixProfileCount;
UInt32 profileTimerCount;
UInt8  *SendBuffer[MAX_PACKET_TYPE];
}TgProfileInfo;

typedef struct RbInfoT 
{
    UInt32 totalExecTime;
    UInt8   tpCount;
    UInt8 isLCTimerPresent;
    UInt16 profileIdx;
    TgProfileInfo tgProfileInfo[MAX_PROF];
    UInt8 txActiveFlag;
    UInt8 lcId;
    UInt32 tId; /* Tunnel Id*/
    UInt8 	tpId[MAX_PROF];
    UInt8	tp_count;
    UInt8 	sps_lc;
    UDouble32 throughput[MAX_PACKET_TYPE];
    UDouble32 totalNoOfPktVar[MAX_PACKET_TYPE];
    UDouble32 fixedPktPerTick[MAX_PACKET_TYPE];
    UDouble32 token;
    UInt8	maxPkt;
    UInt8	minPkt;
    UInt8  PktInCurrTTI ;
    UInt32 noOfTicks;
    UInt32 extraBurstCounter;
    UInt64 txTickCount;
    UInt64 lastDisplayTick;
    UInt8  bitRateFlag;
    UInt32 varPacketSize[MAX_PACKET_TYPE];
    UInt8  extraBurstFlag;
    UInt8  upDownFlag;
#ifndef NO_GTPU_STACK
    UInt32 tId_self; /*Tunnel id */
#endif
    SpsProfile_t  spsProfile[MAX_PROFILES];
#ifdef SUPPORT_64_UE                                            
    /* SPR 5857 : LOAD DISTRIBUTION */
    UInt32 remainingPktSent;
    /* SPR 5857 : LOAD DISTRIBUTION */
#endif
    /* RoHC enhancement */
    UInt32 packet_size;
    UInt16 ipId_ip;
    UInt16 sn_rtp;
    UInt32 ts_rtp;
    UInt8 synack_tcp;           /* Variable to keep a count of number of tcp packets */
    UInt32 totalBytes;          /* total bytes sent, no handling done for wrap around in UE Sim */
}RbInfo;

typedef struct
{
   UInt16         ueId;   /* UE Index*/
   UInt16         lcCnt;
   RbInfo         rbInfo[MAX_LC_ID];   /* LC Id*/
   SInt64         timeDiff;
}ueDataBase_t;

typedef struct
{
    UDouble64 txData;
    UDouble64 rxData;
    UInt64 rxPkts;/* Received Pkts*/
    UInt64 txPkts;/*Transmitted Pkts*/
    UInt64 txOldPkt;/*old tx Pkts till */   
    UInt64 rxOldPkt;/*old rx pkts till */
    UInt32 rxMiss;  /* Missed Packets*/
    UInt32 rxMiss_per_display;  /* Missed Packets Per Display*/
    UInt32 txSeq;   /* Transmission Sequence*/
    UInt32 rxSeq;   /* Reception Sequence*/
}t_ueStats;

/* TCP structure for modifying different field behavior in TCP header */
typedef struct rohcProfile6T
{
    UInt8 isInitiator;          /* Set if UL is the initiator of data connection */
    UInt8 isFin;                /* Set if UL is the initiator of data termination */
    UInt8 isGenerator;          /* It keeps a check on the behavior of data type (genarator/consumer/both) */
}rohcProfile6;
/*- Structures */ 		
/*- Structures */ 			

/*+ Enums */
enum tgModeT
{
    TG_DL_MODE,
    TG_UL_MODE,
    TG_UL_DL_MODE
}tgMode;

enum socketTypeT
{
    OPEN_TX_SOCKET,
    OPEN_RX_SOCKET
}socketType;
/*- Enums */

/************************************************************************
 * 			PRIVATE VARIABLES				*
 ***********************************************************************/

ueDataBase_t 		gtpDb_g[MAX_MULTI_UE];
/*Sampling Database*/
t_ueStats 		ueDataFlow_g[MAX_MULTI_UE][MAX_LC_ID]={0};
UInt32 			firstBlockReceived_ultg[MAX_MULTI_UE][MAX_LC_ID] = {0};
rohcProfile6    profile6Params_g;
struct timeval 		t1;

/*+Scheduled UE*/
UInt16    	ueTurn_g = 0;
UInt16    	ueStart_g = 0;
FILE    	*MissCntrs_g = NULL;
/*-Scheduled UE*/

UInt32 		expectedSegNum_g;
UInt32 		seqNum_g;
UInt32 		numMissedSeq_g = 0;
SInt16 		ueId_g = 0;
UInt32 		throughputDisplayVar_g;
UDouble32  	totalThrputVar_g , totalNoOfPktVar_g ;
UInt32 		packetSize_g, noOfPktVar_g,timeInMs_g;
/*SPR 15909 Fix Start*/
UInt32 		fixedPktPerTick_g, remainingPkt_g, extraBurstCounter_g; pthread_t ulThreadId_g;
UInt64 noOfTicks_g;
/*SPR 15909 Fix end*/
UInt32 		trafficGeneratorAdapterRxPort_g[MAX_SECTORS]={0};
UInt32 		adapterPort_g[MAX_SECTORS] = {0};
UInt32 		trafficProfileVar_g = 0;
UInt32 		pktCounter_g = 0;
UInt32 		missTickCount_g =0;
UInt32 		mode_g;
UInt64 		missTimeCount_g = 0;
UInt64 		tickCount_g =0;
/* +- SPR 18268 */
UInt16 		ueCount_g = 0;
/* +- SPR 18268 */
UDouble32 	load_variation_g = 0;
/*+ TG Changes for QOS Testing */
UDouble32 	burst_time_g = 0;
UDouble32 	total_duration_g = 0;
/*- TG Changes for QOS Testing */
UInt8 		var_counter = 0;
long 		timer_g = 0;
/*+ For Multisector */
UInt8		current_sector = 0;
UInt8		sector_count_g = 0;
UInt8		sector_id_g = 0;
/* SPR 4671 Fix */
UInt8		index_g = 0;
/* SPR 4671 Fix */
struct sockaddr_in 	sockAddr_g;
SInt8 			sockRxFd_g;


/********************************************************************************
 * 				FUNCTION DECLARATIONS				*
 *******************************************************************************/

UInt16 
ipCksumCalc
( 
	UInt16 	lenIPHeader, 
	UInt8 	buffIP[], 
	UInt16 	headerStart
);

UInt16
udpCksumCalc
(
	UInt16 	lenUDPHeader, 
	UInt8 	buffIP[],
	UInt16 	headerStart,
        UInt8   version
);
 
void 
setSchedulePolicy 
(
	int priority,
	int policy
);
 
static 
UInt32 
diffInMilliSecPhy
(
	struct timespec *start, 
	struct timespec *  end
);

void retrive_trafficGenInfo_from_configfile();

UInt32 
appendGtpHeader
(
	UInt8 	*gtp_packet,
	UInt32 	length,
	UInt8 	lcId
);
 
void 
createDownlinkPacket
(
	UInt8 *buff,
	UInt32 length,
	UInt8 lcId
); 
 
UInt32 
openUdpSocket
(
	UInt8 			flag, 
	SInt8 			*dstIPAddr_p, 
	SInt8 			*srcIPAddr_p,
	SInt8			*sockFd_p, 
	struct sockaddr_in 	*sockAddr
);
 
void 
initDownlinkTrafficGen
(
	void 	*srcAddr, 
	void 	*dstAddr, 
	UInt32 	tgMode
);

UInt32 
processDLPacket
(
	void *ipAddr
);

void* 
processULPacket
(
	void *ipAddr
);

void
setSYNnACKinTCPPkt
(
    /* +- SPR 18268 */
    UInt16 ueIndex,
    /* +- SPR 18268 */
    UInt8 lcIndex,
    UInt8 sendBuffer[],
    UInt32 bytes
);

void sendDATAinTCPPkt
(
    /* +- SPR 18268 */
    UInt16 ueIndex,
    /* +- SPR 18268 */
    UInt8 lcIndex,
    UInt8 sendBuffer[],
    UInt32 bytesSent,
    UInt32 counter
);

#ifdef ROHC_PROFILE_6
void initProfile6Params(void);
#endif

/********************************************************************************
 * 				FUNCTION DEFINITIONS				*
 *******************************************************************************/
/*+ Colour Coding */
void cprint_init(int fg, int attr, int bg)
{
    if(bg != -1){
        printf("%c[%d;%d;%dm",27,attr,(30+fg),(40+bg));
    }
    else{
        printf("%c[%d;%dm",27,attr,(30+fg));
    }
}
void cprint_rst(void)
{
    printf("%c[%dm", 27, 0);
}

/*COVERITY FIX-23205 START*/
void cprintf(int fg, int a, int bg, char * fmt, ...)
/*COVERITY FIX-23205 END*/
{
    va_list args;
    va_start(args, fmt);

    cprint_init(fg,a,bg);
    vprintf(fmt, args);
    cprint_rst();

    va_end(args);
}
/*- Colour Coding */

/******************************************************************************
 *   FUNCTION NAME : ipCksumCalc
 *   INPUT         : lenIPHeader,buffIP[]
 *   OUTPUT        : None
 *   RETURNS       : IPcksum
 *   DESCRIPTION   : This function calculates the 16 bit IP checksum
 *
 **************************************************************************/
UInt16 
ipCksumCalc
( 
	UInt16 	lenIPHeader, 
	UInt8 	buffIP[], 
	UInt16 	headerStart
) 
{
	UInt16 word;
	UInt32 sum=0;
	UInt16 index;

        /* To Nullify Checksum */
        buffIP[headerStart + 10] = buffIP[headerStart + 11] = 0;

	for (index=headerStart; index < lenIPHeader+headerStart; index = index +2)
	{
		word =((buffIP[index]<<8) & 0xFF00) + (buffIP[index+1] & 0xFF);
		sum = sum + (UInt32)word;
	}
	while (sum>>16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	/* one's complement the result */
	sum = ~sum;
	return ((UInt16) sum);
}

/******************************************************************************
 **   FUNCTION NAME : udpCksumCalc
 **   INPUT         : lenUDPHeader,buffIP[]
 **   OUTPUT        : None
 **   RETURNS       : UDPcksum
 **   DESCRIPTION   : This function calculates the 16 bit UDP checksum   
 ***************************************************************************/
 
UInt16
udpCksumCalc
(
	UInt16 	lenUDPHeader, 
	UInt8 	buffIP[],
	UInt16 	headerStart,
        UInt8 version
)
{
#ifndef ROHC_PROFILE_6
        UInt16 prot=LTE_IP_UDP_PROTO;
#else
        UInt16 prot=LTE_IP_TCP_PROTO;
#endif
	UInt16 word;
	UInt32 udpSum = 0 ;
	UInt16 index ;
	UInt8 srcAddrIndex ;
	UInt8 destAddrIndex ;
	UInt8 sizeOfAddr ;
	for (index = headerStart; index < lenUDPHeader; index = index+2)
	{
		word =((buffIP[index]<<8)&0xFF00)+(buffIP[index + 1]&0xFF);
		udpSum = udpSum + (unsigned long)word;
	}
	if(version ==4)
	{
		srcAddrIndex = 17;
		destAddrIndex = 21;
		sizeOfAddr = 4;
	}
	else
	{
		srcAddrIndex = 13;
		destAddrIndex = 29;
		sizeOfAddr = 16;
	}
	/* adding the UDP pseudo header which contains the IP source and destinationn addresses */
	for (index = srcAddrIndex; index < destAddrIndex; index = index +2)
	{
		word =(((buffIP[index]<<8)&0xFF00)+(buffIP[index +1]&0xFF));
		udpSum=udpSum+word;
	}
	for (index = destAddrIndex; index < destAddrIndex+sizeOfAddr; index = index +2)
	{
		word =(((buffIP[index ]<<8)&0xFF00)+(buffIP[index +1]&0xFF));
		udpSum=udpSum+word;
	}
	udpSum = udpSum + prot + lenUDPHeader;
	/* keeping last 16 bits of calculated sum and adding the carries */
	while (udpSum>>16)
		udpSum = (udpSum & 0xFFFF)+(udpSum >> 16);
	udpSum = ~udpSum;
	return ((UInt16) udpSum);
   }
/******************************************************************************
 *   FUNCTION NAME : setSchedulePolicy
 *   INPUT         : priority,policy
 *   OUTUT         : None
 *   RETURNS       : None
 *   DESCRIPTION   : This function sets the scheduling priority according to
 *                   policy and priority entered
 * ***************************************************************************/
void 
setSchedulePolicy 
(
	int priority,
	int policy
) 
{
#ifdef HIGH_GEN_PRIORITY
	struct sched_param param;
	param.sched_priority = priority;

	if (pthread_setschedparam(pthread_self(),policy, &param) != 0)
	{
		fprintf(stderr,"setting prioity failed\n "); 
	}
#endif
 }

/****************************************************************************
 ** Function Name  : diffInMilliSecPhy
 ** Inputs         : None
 ** Outputs        : None
 ** Returns        : diffInMicroseconds
 ** Description    : This function calculate the differene in Miliseconds
 *****************************************************************************/

static 
UInt32 
diffInMilliSecPhy
(
	struct timespec *start, 
	struct timespec *  end
) 
{
	UInt32 diffInMicroseconds = 0;
	if (end->tv_sec  > start->tv_sec )
	{
		diffInMicroseconds = (end->tv_sec - start->tv_sec ) * 1000000;
	}
	if (start->tv_nsec >  end->tv_nsec)
	{
		diffInMicroseconds -= (start->tv_nsec -  end->tv_nsec)/1000;
	}
	else
	{
		diffInMicroseconds += (end->tv_nsec -  start->tv_nsec)/1000;
	}
	return (diffInMicroseconds/1000);
}

/****************************************************************************
 * Function Name  : retrive_trafficGenInfo_from_configfile
 * Inputs         : configfile_p : pointer to trafficGen config file
 * Outputs        :
 * Returns        : Zero on success
 * Description    : function retrive simulator Adapter port
 *                  from config file for traffic generator.
 ****************************************************************************/
void retrive_trafficGenInfo_from_configfile()
{
	FILE* fp;
	UInt32 port = 0;
	UInt8  buf[101] = {0,};
	UInt8  index = 0;

	UInt8 *configfile_p = "../cfg/UESIM_Configuration.cfg";    

	fp = fopen(configfile_p,"rb");
	if(fp == PNULL)
	{
		fprintf(stderr,"\nError: Unable to open Config file for PDCP Sim \n");
		exit(0);
	}
	while(fgets(buf,100,fp))
	{
		if(sscanf(buf,"UL_TG_RX_PORT =%d",&port) > 0)
		{
			trafficGeneratorAdapterRxPort_g[0] = (UInt16)port;
		}
		else if(sscanf(buf,"UL_TG_RX_PORT_1=%d",&port) > 0)
		{
			trafficGeneratorAdapterRxPort_g[1] = (UInt16)port;
		}
		else if(sscanf(buf,"UL_TG_RX_PORT_2=%d",&port) > 0)
		{
			trafficGeneratorAdapterRxPort_g[2] = (UInt16)port;
		}
		else if(sscanf(buf,"UE_ADAPTER_RX_PORT=%d",&port) > 0)
		{
			adapterPort_g[index] = (UInt16)port;
		}
		else if(sscanf(buf,"UE_ADAPTER_RX_PORT_1=%d",&port) > 0)
		{
			index++;
			adapterPort_g[index] = (UInt16)port;
		}
		else if(sscanf(buf,"UE_ADAPTER_RX_PORT_2=%d",&port) > 0)
		{
			index++;
			adapterPort_g[index] = (UInt16)port;
		}
	}
	fclose(fp);
}
/****************************************************************************
 ** Function Name  : appendGtpHeader 
 ** Inputs         : None
 ** Outputs        : None
 ** Returns        : gtp_packet and length
 ** Description    : This function append the GTP Header in the front of packet 
 *****************************************************************************/
UInt32 
appendGtpHeader
(
	UInt8 	*gtp_packet,
	UInt32 	length,
	UInt8 	lcId
) 
{
	UInt32 current = 0;
	LTE_SET_U16BIT(&gtp_packet[current],ueTurn_g);
	current +=2;
	gtp_packet[current++] = lcId;
	gtp_packet[current++] = (length & 0xFF00)>>8;
	gtp_packet[current++] = (length & 0x00FF)>>0;
	return current;
}

/****************************************************************************
 ** Function Name  : createDownlinkPacket
 ** Inputs         : None
 ** Outputs        : None
 ** Returns        : diffInMicroseconds
 ** Description    : This function send the downlink packet to GTP-U
 *****************************************************************************/
void 
createDownlinkPacket
(
	UInt8 *buff,
	UInt32 length,
	UInt8 lcId
) 
{
	UInt32 current = 0;
	current = appendGtpHeader(buff,length,lcId);
}    
/****************************************************************************
 * * Function Name  : openUdpSocket
 * * Inputs         : None
 * * Outputs        : None
 * * Returns        : 0 on success 
 * * Description    : This function open udp socket for sending and receving
 * ****************************************************************************/

UInt32 
openUdpSocket
(
	UInt8 			flag, 
	SInt8 			*dstIPAddr_p, 
	SInt8 			*srcIPAddr_p,
	SInt8			*sockFd_p, 
	struct sockaddr_in 	*sockAddr
) 
{
	UInt8 index = 0;

	if(OPEN_TX_SOCKET == flag)
	{
		assert(dstIPAddr_p);
		if((*sockFd_p = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			perror("SOCKET CREATION ERROR :\n");
			return 0;
		}
		sockAddr->sin_family = AF_INET;
		/* SPR 4671 Fix */
		while(index_g != index)
		{
		/* SPR 4671 Fix */
			index++;
		}
		sockAddr->sin_port = htons(adapterPort_g[index]);
		sockAddr->sin_addr.s_addr = inet_addr(dstIPAddr_p);
		printf("\nTX PORT = %d\n",adapterPort_g[index]);
	}
	else if (OPEN_RX_SOCKET == flag) 
	{
		assert(srcIPAddr_p);
		if((*sockFd_p = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			perror("SOCKET CREATION ERROR :\n");
			return 0;
		}
		sockAddr->sin_family = AF_INET;
		sockAddr->sin_port = htons(trafficGeneratorAdapterRxPort_g[index]);
		sockAddr->sin_addr.s_addr = inet_addr(srcIPAddr_p);
		while(bind(*sockFd_p,(struct sockaddr *)sockAddr, sizeof(struct sockaddr_in)) < 0)
		{
			index++;
			sockAddr->sin_port = htons(trafficGeneratorAdapterRxPort_g[index]);
		}
		fprintf(stderr,"\nUL TG instance %d started\n",index+1);
	}
	else
	{
		assert(srcIPAddr_p && dstIPAddr_p);
		if((*sockFd_p = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			perror("SOCKET CREATION ERROR :\n");
			return 0;
		}
		sockAddr->sin_family = AF_INET;
		sockAddr->sin_port = htons(adapterPort_g[index]);
		sockAddr->sin_addr.s_addr = inet_addr(dstIPAddr_p);
		if((*sockFd_p = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			perror("SOCKET CREATION ERROR :\n");
			return 0;
		}
		sockAddr->sin_family = AF_INET;
		sockAddr->sin_port = htons(trafficGeneratorAdapterRxPort_g[index]);
		sockAddr->sin_addr.s_addr = inet_addr(srcIPAddr_p);
		if(bind(*sockFd_p,(struct sockaddr *)sockAddr, sizeof(struct sockaddr_in)))
			abort(); 
	}
	return 1;
}

/****************************************************************************
 ** Function Name  : initDownlinkTrafficGen
 ** Inputs         : None
 ** Outputs        : None
 ** Returns        : None
 ** Description    : This function calls ul and dl processing based on mode. 
 *****************************************************************************/

void 
initDownlinkTrafficGen
(
	void 	*srcAddr, 
	void 	*dstAddr, 
	UInt32 	tgMode
)
{
 	mode_g = tgMode;
	switch(tgMode)
	{
		case TG_DL_MODE:
			processDLPacket(dstAddr);
			break;

		case TG_UL_MODE:
			/*processULPacket(srcAddr);
			break;*/
		
		case TG_UL_DL_MODE:
			if(pthread_create(&ulThreadId_g,PNULL,processULPacket,srcAddr)< 0)
			{
				abort();
			}
			processDLPacket(dstAddr);
			break;
		default:
			fprintf(stderr, "Invalid TG Mode received");
			exit(0);
	}
}

/* Function to vary the IPID value in IP Header */
    /* +- SPR 18268 */
void setIPIDinIPPkt(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[])
    /* +- SPR 18268 */
{
#ifndef UE_IPV6_ADDR
    UInt16 headerStart = 5, ipid = 0, ipid_lsb;
    UInt16          ipChecksum = 0;
    /* if RoHC test enhancements set */
    if (IPIDValueChange_g)
    {
        /* Set IPID value in IP header and increment by 1 */
        sendBuffer[headerStart+4] = ( gtpDb_g[ueIndex].rbInfo[lcIndex].ipId_ip >> 8) & 0xFF;
        sendBuffer[headerStart+5] =  gtpDb_g[ueIndex].rbInfo[lcIndex].ipId_ip & 0xFF;

        /* Generate new IP-ID field */
        // RANDOM
        if (IPIDValueChange_g == 1)
        {
          ipid = (UInt16)random();
        }
        // Sequence
        else if (IPIDValueChange_g == 2)
        {
          ipid = gtpDb_g[ueIndex].rbInfo[lcIndex].ipId_ip + 1;
        }
        // Byte Swapped but in sequence
        else if (IPIDValueChange_g == 3)
        {
          ipid = gtpDb_g[ueIndex].rbInfo[lcIndex].ipId_ip;
          ipid_lsb= ipid & 0xFF;
          ipid = ((ipid >> 8) & 0xFF) | ((ipid_lsb << 8) & 0xFF00);
          ipid++;
          ipid_lsb= ipid & 0xFF;
          ipid = ((ipid >> 8) & 0xFF) | ((ipid_lsb << 8) & 0xFF00);
        }
        else if ((IPIDValueChange_g == 4))
        {
            ipid = 0;
        }
        ipChecksum =  ipCksumCalc( 20, sendBuffer, headerStart);
        sendBuffer[headerStart+10 ] = (ipChecksum >>8) & (0xFF);
        sendBuffer[headerStart+11] = (ipChecksum & 0xFF);

        gtpDb_g[ueIndex].rbInfo[lcIndex].ipId_ip = ipid;
    }
    /* otherwise no change */
#endif
    return;
}

/* Function to vary the TS and SN values in RTP header */
    /* +- SPR 18268 */
void setSNnTSinRTPPkt(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[])
    /* +- SPR 18268 */
{
#ifdef ROHC_PROFILE_1
    UInt16 headerStart = 5;
    UInt8 version = (sendBuffer[headerStart] >> 4) & 0xF;

    /* Add IP and UDP header length */
    if(version == 4)
    {
        headerStart = headerStart + 20 + 8;
    }
    else /* IPv6 */
    {
        headerStart = headerStart + 40 + 8;
    }


    /* if RoHC test enhancements set */
    if (TSandSNValueChange_g == 1)
    {
        /* Set SN value in RTP header and increment the SN by 1 in RTP header */
        sendBuffer[headerStart+2] = (gtpDb_g[ueIndex].rbInfo[lcIndex].sn_rtp >> 8) & 0xFF;
        sendBuffer[headerStart+3] =  gtpDb_g[ueIndex].rbInfo[lcIndex].sn_rtp & 0xFF;

        /* Set TS value in RTP header and increment the TS by TS_PERIOD in RTP header */
        sendBuffer[headerStart+4] = ( gtpDb_g[ueIndex].rbInfo[lcIndex].ts_rtp >> 24) & 0xFF;
        sendBuffer[headerStart+5] = ( gtpDb_g[ueIndex].rbInfo[lcIndex].ts_rtp >> 16) & 0xFF;
        sendBuffer[headerStart+6] = ( gtpDb_g[ueIndex].rbInfo[lcIndex].ts_rtp >> 8) & 0xFF;
        sendBuffer[headerStart+7] =  gtpDb_g[ueIndex].rbInfo[lcIndex].ts_rtp & 0xFF;

        gtpDb_g[ueIndex].rbInfo[lcIndex].sn_rtp++;
        gtpDb_g[ueIndex].rbInfo[lcIndex].ts_rtp += TS_PERIOD;
    }
#endif
    return;
}
/*Inserting data part in packet after TCP options*/
    /* +- SPR 18268 */
void addDataAtEnd(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[])
    /* +- SPR 18268 */
{
    #ifdef ROHC_PROFILE_6
    UInt16 headerStart = 5;
    UInt8 version = (sendBuffer[headerStart] >> 4) & 0xF;
    UInt8 offset = 0;
    UInt8 pos = 0, maxPos = 0;
    /* Add IP header length */
    if(version == 4)
    {
        headerStart = headerStart + 20;
    }
    else /* IPv6 */
    {
        headerStart = headerStart + 40;
    }
    offset = ((sendBuffer[headerStart + 12]>>4)*4);
    pos = headerStart + offset;
    maxPos = pos + 10;
    sendBuffer[pos++] = 0x61; 
    sendBuffer[pos++] = 0x6e; 
    sendBuffer[pos++] = 0x69;
    sendBuffer[pos++] = 0x6c;
    while(pos != maxPos)
    {
        sendBuffer[pos++] = 0x00;
    }
    #endif
}

/* Function to call control flow functions for TCP header */
    /* +- SPR 18268 */
void controlflowCallforTCP(UInt16 ueIndex, UInt8 lcIndex, UInt8 SendBuffer[], UInt32 bytes)
    /* +- SPR 18268 */
{
#ifdef ROHC_PROFILE_6
        /* Set SYN and ACK fields in TCP header */
        setSYNnACKinTCPPkt(ueIndex, lcIndex,SendBuffer, bytes);

#endif
    return;
}


/* Function to set SYN and ACK fields in TCP header */
    /* +- SPR 18268 */
void setSYNnACKinTCPPkt(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[], UInt32 bytes)
    /* +- SPR 18268 */
{
#ifdef ROHC_PROFILE_6
    UInt16 headerStart = 5;
    UInt8 version = (sendBuffer[headerStart] >> 4) & 0xF;
    UInt8 flag = 0;
    static UInt16 counter = 0;
    static UInt8 wrap_around_case = 0;
    if (counter == 0)
    {
        gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp = 0; /* FOR FIRST PACKET */
        gtpDb_g[ueIndex].rbInfo[lcIndex].totalBytes = 0;
    }

    /* Add IP header length */
    if(version == 4)
    {
        headerStart = headerStart + 20;
    }
    else /* IPv6 */
    {
        headerStart = headerStart + 40;
    }

    if (COUNTDOWN == gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp)
    {
        if (1 != profile6Params_g.isFin)
        {
            gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp++;
        }
    }


    /* if UL is the initiator for a TCP connection */
    if (1 == profile6Params_g.isInitiator)
    {
        if (0 == gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp)
        {
            /* Set SYN flag in TCP header */
            sendBuffer[headerStart+13] =  0x02;
            gtpDb_g[ueIndex].rbInfo[lcIndex].totalBytes += gtpDb_g[ueIndex].rbInfo[lcIndex].packet_size;
        }
        else
        {
            if (1 == gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp)
            {
                /* Set ACK flag in TCP header */
                sendBuffer[headerStart+13] = 0x10;
                /* increment the SN by 1 for ACK packet */
                sendBuffer[headerStart+7]++;
                sendBuffer[headerStart+11]++;
                gtpDb_g[ueIndex].rbInfo[lcIndex].totalBytes += gtpDb_g[ueIndex].rbInfo[lcIndex].packet_size;
            }
        }
    }
    else
    {
        if (0 == gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp)
        {
            /* Set ACK and SYN flags in TCP header */
            sendBuffer[headerStart+13] = 0x12;
            /* ACK byte increameneted by one */
            sendBuffer[headerStart+11]++;
            gtpDb_g[ueIndex].rbInfo[lcIndex].totalBytes += gtpDb_g[ueIndex].rbInfo[lcIndex].packet_size;
        }
    }
    if (COUNTDOWN <= gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp)
    {
        switch(gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp)
        {
            case COUNTDOWN:
            /* Set FIN flag in TCP header */
            sendBuffer[headerStart+13] =  0x01;
            gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp += 2;
            break;
            case COUNTDOWN+1:
            /* Set ACK flag in TCP header */
            sendBuffer[headerStart+13] =  0x10;
            break;
            case COUNTDOWN+2:
            /* Set FIN flag in TCP header */
            sendBuffer[headerStart+13] =  0x01;
            flag = 1;
            break;
            case COUNTDOWN+3:
            /* Set ACK flag in TCP header */
            sendBuffer[headerStart+13] =  0x10;
            flag = 1;
            break;
        }
    }
    else
    {
        /* Data Part of TCP */
        sendDATAinTCPPkt(ueIndex, lcIndex, sendBuffer, bytes, counter);
    }

    /* ADDING OPTIONS IN TCP PACKET START */
    if (wrap_around_case)    /*IN case of wrap around , TCP options are being reinitialized */
    {
        char option_Buffer[] ={0x02, 0x04, 0x05, 0xb4,
            0x04, 0x02, 
            0x08, 0x0a, 0xe2, 0x08, 0x2f, 0x90,0x00, 0x00, 0x00, 0x00, 
            0x01, 
            0x03, 0x03, 0x07};
        memcpy(&sendBuffer[headerStart+20],option_Buffer,20);
        UInt8 offset = 0x0A;
        sendBuffer[headerStart+12] &= 0x0f;
        sendBuffer[headerStart+12]|= offset <<4;
        wrap_around_case = 0;
    }
    if(1 == counter)
    {
        /* change window scale option */
        sendBuffer[headerStart+39] =  0x08;
    }
    if( counter >= 10 && counter <50)
    {
        /* set SACK option in tcp header */
        sendBuffer[headerStart+40] =  0x05; /*sack kind*/
        sendBuffer[headerStart+41] =  0x0a; /*sack length*/
        UInt32 ref_val;
        UInt8 offset;

        memcpy(&ref_val,&sendBuffer[headerStart+8],4);
        ref_val = ntohl(ref_val);
        ref_val = ref_val + 0x1f4;
        ref_val = ntohl(ref_val);
        memcpy(&sendBuffer[headerStart+42],&ref_val,4);
        ref_val = ntohl(ref_val);
        ref_val = ref_val + 0x1f4;
        ref_val = ntohl(ref_val);
        memcpy(&sendBuffer[headerStart+46],&ref_val,4);

        /* set EOL option */
        sendBuffer[headerStart+50] = 0x00;
        sendBuffer[headerStart+51] = 0x00;
        if(10 == counter)
        {
            /* set new data offset */
            offset = (sendBuffer[headerStart+12]&0xf0)>>4;
            offset = offset + 3;
            sendBuffer[headerStart+12] &= 0x0f;
            sendBuffer[headerStart+12]|= offset <<4;
        }
        if(counter >=20 && counter <30)
        {
            /* set 2 SACK blocks */
            sendBuffer[headerStart+41] =  0x12; /*sack length*/
            ref_val = ntohl(ref_val);
            ref_val = ref_val + 0x1f4;
            ref_val = ntohl(ref_val);
            memcpy(&sendBuffer[headerStart+50],&ref_val,4);
            ref_val = ntohl(ref_val);
            ref_val = ref_val + 0x1f4;
            ref_val = ntohl(ref_val);
            memcpy(&sendBuffer[headerStart+54],&ref_val,4);
            /* adjust EOL option */
            sendBuffer[headerStart+58] = 0x00;
            sendBuffer[headerStart+59] = 0x00;
            /* change data offset value */

            if(20 == counter)
            {
                offset = (sendBuffer[headerStart+12]&0xf0)>>4;
                offset = offset + 2;
                sendBuffer[headerStart+12] &= 0x0f;
                sendBuffer[headerStart+12]|= offset <<4;
            }
        }
        if(counter >=30)
        {
            /* List structure changes and 3 SACK blocks+ 2 EOL  are sent */
            offset = 12;
            sendBuffer[headerStart+12] &= 0x0f;
            sendBuffer[headerStart+12]|= offset <<4;
            /* set SACK option in tcp header */
            sendBuffer[headerStart+20] =  0x05; /*sack kind*/
            sendBuffer[headerStart+21] =  0x1a; /*sack length*/

            memcpy(&ref_val,&sendBuffer[headerStart+8],4);
            ref_val = ntohl(ref_val);
            ref_val = ref_val + 0x1f4;
            ref_val = ntohl(ref_val);
            memcpy(&sendBuffer[headerStart+22],&ref_val,4);

            ref_val = ntohl(ref_val);
            ref_val = ref_val + 0x1f4;
            ref_val = ntohl(ref_val);
            memcpy(&sendBuffer[headerStart+26],&ref_val,4);
            /* 2nd sack block */
            ref_val = ntohl(ref_val);
            ref_val = ref_val + 0x1f4;
            ref_val = ntohl(ref_val);
            memcpy(&sendBuffer[headerStart+30],&ref_val,4);
            ref_val = ntohl(ref_val);
            ref_val = ref_val + 0x1f4;
            ref_val = ntohl(ref_val);
            memcpy(&sendBuffer[headerStart+34],&ref_val,4);
            /* 3rd sack block */
            ref_val = ntohl(ref_val);
            ref_val = ref_val + 0x1f4;
            ref_val = ntohl(ref_val);
            memcpy(&sendBuffer[headerStart+38],&ref_val,4);
            ref_val = ntohl(ref_val);
            ref_val = ref_val + 0x1f4;
            ref_val = ntohl(ref_val);
            memcpy(&sendBuffer[headerStart+42],&ref_val,4);
            /* set EOL option */
            sendBuffer[headerStart+46] = 0x00;
            sendBuffer[headerStart+47] = 0x00;
            if(counter >=40)
            {
                /* send 4 sack blocks */
                sendBuffer[headerStart+21] =  0x22; /*sack length*/
                /* set 4th SACK Block */
                ref_val = ntohl(ref_val);
                ref_val = ref_val + 0x1f4;
                ref_val = ntohl(ref_val);
                memcpy(&sendBuffer[headerStart+46],&ref_val,4);
                ref_val = ntohl(ref_val);
                ref_val = ref_val + 0x1f4;
                ref_val = ntohl(ref_val);
                memcpy(&sendBuffer[headerStart+50],&ref_val,4);

                /* set EOL option */
                sendBuffer[headerStart+54] = 0x00;
                sendBuffer[headerStart+55] = 0x00;
                /* set updated offset*/
                offset = 14;
                sendBuffer[headerStart+12] &= 0x0f;
                sendBuffer[headerStart+12]|= offset <<4;
            }
        }
        if(counter <30)
        {
            /* change TS option */
            ref_val = 0;
            memcpy(&ref_val,&sendBuffer[headerStart+28],4);
            ref_val = ntohl(ref_val);
            ref_val = ref_val + 0x3e8; /* increment ts val by 1000*/
            ref_val = ntohl(ref_val);
            memcpy(&sendBuffer[headerStart+28],&ref_val,4);
            /* change ts echo */
            ref_val = 0;
            memcpy(&ref_val,&sendBuffer[headerStart+32],4);
            ref_val = ntohl(ref_val);
            ref_val = ref_val + 0x1f4; /* increment ts echo by 500*/
            ref_val = ntohl(ref_val);
            memcpy(&sendBuffer[headerStart+32],&ref_val,4);
        }
    }
    if(counter >= 50 )
    {
        /* List structure changes TS+SACK PERMITTED+SACK+ALTERNATE CKSUM REQ+WS */
        UInt8 offset;
        UInt32 ts_val = 0x10023890;
        UInt32 ts_echo = 0xc20fb5b4;
        ts_val = ntohl(ts_val);
        ts_echo = ntohl(ts_echo);
        offset = 12;
        sendBuffer[headerStart+12] &= 0x0f;
        sendBuffer[headerStart+12]|= offset <<4;
        /* set TS option */
        sendBuffer[headerStart+20] =  0x08; /* TS kind*/
        sendBuffer[headerStart+21] =  0x0a; /* TS length*/
        memcpy(&sendBuffer[headerStart+22],&ts_val,4);
        memcpy(&sendBuffer[headerStart+26],&ts_echo,4);
        /* set SACK Permitted option */
        sendBuffer[headerStart+30] =  0x04; /* SP kind*/
        sendBuffer[headerStart+31] =  0x02; /* SP length*/
        /* set SACK option */
        sendBuffer[headerStart+32] =  0x05; /*sack kind*/
        sendBuffer[headerStart+33] =  0x0a; /*sack length*/
        UInt32 ref_val;
        memcpy(&ref_val,&sendBuffer[headerStart+8],4);
        ref_val = ntohl(ref_val);
        ref_val = ref_val + 0x1f4;
        ref_val = ntohl(ref_val);
        memcpy(&sendBuffer[headerStart+34],&ref_val,4);
        ref_val = ntohl(ref_val);
        ref_val = ref_val + 0x1f4;
        ref_val = ntohl(ref_val);
        memcpy(&sendBuffer[headerStart+38],&ref_val,4);
        /* set ALternate CKSUM REQUEST Option */
        sendBuffer[headerStart+42] =  0x0e; /* ALT CKSUM REQ kind*/
        sendBuffer[headerStart+43] =  0x03; /* ALT CKSUM REQ length*/
        sendBuffer[headerStart+44] =  0x21; /* value */

        /* set window scale Option */
        sendBuffer[headerStart+45] =  0x03; /* WS kind*/
        sendBuffer[headerStart+46] =  0x03; /* WS length*/
        sendBuffer[headerStart+47] =  0x24; /* value */
        if(counter >= 60 && counter <70)
        {
            /* add alt cksum data + eol option */
            sendBuffer[headerStart+45] =  0x0f; /* ALT CKSUM DATA kind*/
            sendBuffer[headerStart+46] =  0x04; /* ALT CKSUM DATA length*/
            sendBuffer[headerStart+47] =  0x17; /* ALT CKSUM DATA value */
            sendBuffer[headerStart+48] =  0x18; /* ALT CKSUM DATA value */
            sendBuffer[headerStart+49] =  0x00; /* EOL KIND */
            sendBuffer[headerStart+50] =  0x00;
            sendBuffer[headerStart+51] =  0x00;
            /* set new offset */
            offset = 13;
            sendBuffer[headerStart+12] &= 0x0f;
            sendBuffer[headerStart+12]|= offset <<4;
        }

        if(counter >= 70 && counter < 80)
        {
            /* add alt cksum data with changed length + eol option */
            sendBuffer[headerStart+45] =  0x0f; /* ALT CKSUM DATA kind*/
            sendBuffer[headerStart+46] =  0x06; /* ALT CKSUM DATA length*/
            sendBuffer[headerStart+47] =  0x17; /* ALT CKSUM DATA value */
            sendBuffer[headerStart+48] =  0x18; /* ALT CKSUM DATA value */
            sendBuffer[headerStart+49] =  0x19; /* ALT CKSUM DATA value */
            sendBuffer[headerStart+50] =  0x20; /* ALT CKSUM DATA value */
            sendBuffer[headerStart+51] =  0x00; /* EOL KIND */
            sendBuffer[headerStart+52] =  0x00;
            sendBuffer[headerStart+53] =  0x00;
            sendBuffer[headerStart+54] =  0x00;
            sendBuffer[headerStart+55] =  0x00;
            /* set new offset */
            offset = 14;
            sendBuffer[headerStart+12] &= 0x0f;
            sendBuffer[headerStart+12]|= offset <<4;
        }
        if(counter >= 80 && counter <90)
        {
            /* rainy day case: add one more generic option other than 2 known ones
             * with kind 16 */
            sendBuffer[headerStart+45] =  0x0f; /* ALT CKSUM DATA kind*/
            sendBuffer[headerStart+46] =  0x06; /* ALT CKSUM DATA length*/
            sendBuffer[headerStart+47] =  0x17; /* ALT CKSUM DATA value */
            sendBuffer[headerStart+48] =  0x18; /* ALT CKSUM DATA value */
            sendBuffer[headerStart+49] =  0x19; /* ALT CKSUM DATA value */
            sendBuffer[headerStart+50] =  0x20; /* ALT CKSUM DATA value */
            sendBuffer[headerStart+51] =  0x10; /* generic opt kind 16 */
            sendBuffer[headerStart+52] =  0x04;
            sendBuffer[headerStart+53] =  0x30;
            sendBuffer[headerStart+54] =  0x31;
            sendBuffer[headerStart+55] =  0x00; /* EOL KIND */
            sendBuffer[headerStart+56] =  0x00;
            sendBuffer[headerStart+57] =  0x00;
            sendBuffer[headerStart+58] =  0x00;
            sendBuffer[headerStart+59] =  0x00;
            /* set new offset */
            offset = 15;
            sendBuffer[headerStart+12] &= 0x0f;
            sendBuffer[headerStart+12]|= offset <<4;
        }
        if(counter >= 90)
        {
            /* change structure of options and add NOP also in between */

            sendBuffer[headerStart+45] =  0x01; /* NOP kind*/
            sendBuffer[headerStart+46] =  0x10; /* gen opt kind 16*/
            sendBuffer[headerStart+47] =  0x03;
            sendBuffer[headerStart+48] =  0x18;
            sendBuffer[headerStart+49] =  0x01; /* NOP kind */
            sendBuffer[headerStart+50] =  0x0f; /* ALT CKSUM DATA kind */
            sendBuffer[headerStart+51] =  0x06;
            sendBuffer[headerStart+52] =  0x04;
            sendBuffer[headerStart+53] =  0x30;
            sendBuffer[headerStart+54] =  0x31;
            sendBuffer[headerStart+55] =  0x00;
            /* set new offset */
            offset = 14;
            sendBuffer[headerStart+12] &= 0x0f;
            sendBuffer[headerStart+12]|= offset <<4;
            if(95 == counter)
            {
                /* chnage value of generic option */

                sendBuffer[headerStart+48] =  0x19;
                sendBuffer[headerStart+52] =  0x05;
                sendBuffer[headerStart+53] =  0x31;
                sendBuffer[headerStart+54] =  0x32;
                sendBuffer[headerStart+55] =  0x01;
            }
        }
    }
    /* ADDING OPTIONS IN TCP PACKET END */
    addDataAtEnd(ueIndex,lcIndex,sendBuffer);
    gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp++;
    if (counter == 65535)
    {
        wrap_around_case = 1;
    }
    counter ++;

    if (1 == flag)
    {
        /* Reset the value so that again TCP connection procedure starts */
        gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp = 0;
        gtpDb_g[ueIndex].rbInfo[lcIndex].totalBytes = 0;
        counter = 0;
        /* Reset of Connection */
    }
    return;
#endif
}

/* TO CHANGE SN more than 16bits */
    /* +- SPR 18268 */
void setAckNum_morethan16bits(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[])
    /* +- SPR 18268 */
{
    UInt32 ACKvalue = 0;
    UInt32 *sendBuffer_p;
    sendBuffer_p = (UInt32 *)&sendBuffer[0];
    *sendBuffer_p = ntohl(*sendBuffer_p);
    ACKvalue = 131072  + *sendBuffer_p;
    /* Set SN field of the TCP header */
    sendBuffer[0] = (ACKvalue >> 24) & 0xFF;
    sendBuffer[1] = (ACKvalue >> 16) & 0xFF;
    sendBuffer[2] = (ACKvalue >> 8) & 0xFF;
    sendBuffer[3] = (ACKvalue) & 0xFF;
}

/* TO CHANGE SN more than 16bits */
    /* +- SPR 18268 */
void setSeqNum_morethan16bits(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[])
    /* +- SPR 18268 */
{
    UInt32 SNvalue = 0;
    UInt32 *sendBuffer_p;
    sendBuffer_p = (UInt32 *)&sendBuffer[0];
    *sendBuffer_p = ntohl(*sendBuffer_p);
    SNvalue = 131072  + *sendBuffer_p;
    /* Set SN field of the TCP header */
    sendBuffer[0] = (SNvalue >> 24) & 0xFF;
    sendBuffer[1] = (SNvalue >> 16) & 0xFF;
    sendBuffer[2] = (SNvalue >> 8) & 0xFF;
    sendBuffer[3] = (SNvalue) & 0xFF;
}

    /* +- SPR 18268 */
void setUnscaledSeqNum(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[])
    /* +- SPR 18268 */
{
    UInt32 unScaledSNvalue = 0;
    UInt32 *sendBuffer_p;
    sendBuffer_p = (UInt32 *)&sendBuffer[0];
    *sendBuffer_p = ntohl(*sendBuffer_p);
    unScaledSNvalue = gtpDb_g[ueIndex].rbInfo[lcIndex].totalBytes + *sendBuffer_p;
    /* Set SN field of the TCP header */
    sendBuffer[0] = (unScaledSNvalue >> 24) & 0xFF;
    sendBuffer[1] = (unScaledSNvalue >> 16) & 0xFF;
    sendBuffer[2] = (unScaledSNvalue >> 8) & 0xFF;
    sendBuffer[3] = (unScaledSNvalue) & 0xFF;
}

    /* +- SPR 18268 */
void setScaledSeqNum(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[])
    /* +- SPR 18268 */
{
    UInt32 scaledSNvalue = 0;
    UInt32 *sendBuffer_p;
    UInt8   ipHeaderLen = 20; /*FOR IPV4 (need to change for IPV6)*/
    sendBuffer_p = (UInt32 *)&sendBuffer[0];
    *sendBuffer_p = ntohl(*sendBuffer_p);
    scaledSNvalue = gtpDb_g[ueIndex].rbInfo[lcIndex].totalBytes - (GTP_HDR + ipHeaderLen + ((sendBuffer[8]>>4)*4));
    scaledSNvalue += *sendBuffer_p;
    /* Set SN field of the TCP header */
    sendBuffer[0] = (scaledSNvalue >> 24) & 0xFF;
    sendBuffer[1] = (scaledSNvalue >> 16) & 0xFF;
    sendBuffer[2] = (scaledSNvalue >> 8) & 0xFF;
    sendBuffer[3] = (scaledSNvalue) & 0xFF;
}

    /* +- SPR 18268 */
void setUnscaledAckNum(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[])
    /* +- SPR 18268 */
{
    UInt32 *sendBuffer_p;
    UInt32 unScaledAckValue = 0;
    sendBuffer_p = (UInt32 *)&sendBuffer[0];
    *sendBuffer_p = ntohl(*sendBuffer_p);
    unScaledAckValue = (*sendBuffer_p + gtpDb_g[ueIndex].rbInfo[lcIndex].totalBytes ) + (*sendBuffer_p % 10);

    sendBuffer[0] = (unScaledAckValue >> 24) & 0xFF;
    sendBuffer[1] = (unScaledAckValue >> 16) & 0xFF;
    sendBuffer[2] = (unScaledAckValue >> 8) & 0xFF;
    sendBuffer[3] = (unScaledAckValue) & 0xFF;

}

    /* +- SPR 18268 */
void setScaledAckNum(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[])
    /* +- SPR 18268 */
{
    UInt32 *sendBuffer_p;
    UInt32 scaledAckValue = 0;
    sendBuffer_p = (UInt32 *)&sendBuffer[0];
    *sendBuffer_p = ntohl(*sendBuffer_p);
    scaledAckValue = *sendBuffer_p + gtpDb_g[ueIndex].rbInfo[lcIndex].totalBytes;
    sendBuffer[0] = (scaledAckValue >> 24) & 0xFF;
    sendBuffer[1] = (scaledAckValue >> 16) & 0xFF;
    sendBuffer[2] = (scaledAckValue >> 8) & 0xFF;
    sendBuffer[3] = (scaledAckValue) & 0xFF;

}

    /* +- SPR 18268 */
void alterWindowSize(UInt16 ueIndex, UInt8 lcIndex,UInt8 sendBuffer[])
    /* +- SPR 18268 */
{
    /*ALTER THE WINDOW SIZE*/
    sendBuffer[6] = (sendBuffer[3]) & 0xFF;
    sendBuffer[7] = (sendBuffer[2]) & 0xFF;
}

/* Function to send Data in TCP header */
    /* +- SPR 18268 */
void sendDATAinTCPPkt(UInt16 ueIndex, UInt8 lcIndex, UInt8 sendBuffer[],UInt32 bytesSent,UInt32 counter)
    /* +- SPR 18268 */
{
    #ifdef ROHC_PROFILE_6
    UInt16 headerStart = 5;
    UInt8 version = (sendBuffer[headerStart] >> 4) & 0xF;

    gtpDb_g[ueIndex].rbInfo[lcIndex].totalBytes = bytesSent;

    /* Add IP header length to reach at start of TCP header */
    if(version == 4)
    {
        headerStart = headerStart + 20;
    }
    else /* IPv6 */
    {
        headerStart = headerStart + 40;
    }

    /* Data Part of TCP */
    if (0 !=  profile6Params_g.isGenerator)
    {
        if (0 == gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp % PUSH_COUNT)
        {
            /* Set push flag after every 11th packet and increment push
             * count by 1*/
            //sendBuffer[headerStart+13] =  0x08;
            /* TOGGLE PUSH FLAG */
            sendBuffer[headerStart+13] = sendBuffer[headerStart + 13] ^ (1 << 3);

        }
        else
        {
            sendBuffer[headerStart+13] &= 0xF7;
        }

        if (0 == gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp % URGENT_COUNT)
        {
            /* Set urgent flag after every 17th packet and increment urgent
             * count by 1*/
            /*TOGGLE URG FLAG */
            sendBuffer[headerStart+13] = sendBuffer[headerStart + 13] ^ (1 << 5);
            if (sendBuffer[headerStart+13] & 0x20 )
            {
                sendBuffer[headerStart+18] =  (counter % 255);
                sendBuffer[headerStart+19] =  (counter / 255);
            }
        }
        else
        {
            sendBuffer[headerStart+13] &= 0xDF;
            sendBuffer[headerStart+18] =  0x00;
            sendBuffer[headerStart+19] =  0x00;           
        }            

        if (0 == gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp % ACK_COUNT)
        {
            /*TOGGLE ACK FLAG */
            sendBuffer[headerStart + 13] = sendBuffer[headerStart + 13] ^ (1 << 4);
        }
        else
        {
            sendBuffer[headerStart + 13] &= 0xEF;    
        }
        
        /* TOGGLING DSCP */
        if (0 == gtpDb_g[ueIndex].rbInfo[lcIndex].synack_tcp % DSCP_COUNT)
        {
            sendBuffer[GTP_HDR + 1] |= ((counter % 10) << 3);
            UInt16 ipChecksum =  ipCksumCalc((UInt16) 20,
                                            sendBuffer,
                                            GTP_HDR);

            sendBuffer[GTP_HDR+10] = (ipChecksum >>8) & (0xFF);
            sendBuffer[GTP_HDR+11] = (ipChecksum & 0xFF);
            
        }
        
    }
    /* GENERATE DIFFERNT RND PACKET TYPES */
    if (counter <= 100 )
    {
        /*MAKING CO_COMMON (SN >16bits change and TTL-HOP CHANGE)leading to CO_COMMON */
        if (counter >10 && counter <25)
        {
            /* ONLY TTL CHANGE */
            sendBuffer[GTP_HDR + 8] = counter;
            UInt16 ipChecksum =  ipCksumCalc((UInt16) 20,
                                            sendBuffer,
                                            GTP_HDR);

            sendBuffer[GTP_HDR+10] = (ipChecksum >>8) & (0xFF);
            sendBuffer[GTP_HDR+11] = (ipChecksum & 0xFF);

        }
        else if (counter >25 && counter < 35)
        {
            /* TTL AND SN > 16 bits CHANGE leading to CO_COMMON*/
            setSeqNum_morethan16bits(ueIndex,lcIndex,&sendBuffer[headerStart + 4]);
                        sendBuffer[GTP_HDR + 8] = counter;
            UInt16 ipChecksum =  ipCksumCalc((UInt16) 20,
                                            sendBuffer,
                                            GTP_HDR);

            sendBuffer[GTP_HDR+10] = (ipChecksum >>8) & (0xFF);
            sendBuffer[GTP_HDR+11] = (ipChecksum & 0xFF);

        }
        else
        {
        /* GENERATE REST RND1 PACKETS */
        setUnscaledSeqNum(ueIndex,lcIndex,&sendBuffer[headerStart + 4]);
        }
    }
    else if ( counter > 100 && counter <= 200)
    {
        if (counter > 120 && counter < 135 )
        {
            /*CHANGE RSF FLAG ONLY leading to CO_COMMON*/
            if (counter % 2)
            {
                sendBuffer[headerStart + 13] = sendBuffer[headerStart + 13] ^ (1);
                sendBuffer[headerStart + 13] = sendBuffer[headerStart + 13] & (0xF9);
            }
            else if (counter % 3)
            {
                sendBuffer[headerStart + 13] = sendBuffer[headerStart + 13] ^ (1<<1);
                sendBuffer[headerStart + 13] = sendBuffer[headerStart + 13] & (0xFA);
            }
            else
            {
                sendBuffer[headerStart + 13] = sendBuffer[headerStart + 13] ^ (1<<2);
                sendBuffer[headerStart + 13] = sendBuffer[headerStart + 13] & (0xFC);
            }
        }
        else if (counter > 135 && counter < 145)
        {
            /* TTL AND SN > 16 bits CHANGE leading to CO_COMMON*/
            setAckNum_morethan16bits(ueIndex,lcIndex,&sendBuffer[headerStart + 8]);
                        sendBuffer[GTP_HDR + 8] = counter;
            UInt16 ipChecksum =  ipCksumCalc((UInt16) 20,
                                            sendBuffer,
                                            GTP_HDR);

            sendBuffer[GTP_HDR+10] = (ipChecksum >>8) & (0xFF);
            sendBuffer[GTP_HDR+11] = (ipChecksum & 0xFF);


        }
        else
        {
            /* GENERATE REST RND2 */
            setScaledSeqNum(ueIndex,lcIndex,&sendBuffer[headerStart + 4]);
        }
    }
    else if (counter > 200 && counter <= 300)
    {
        /*CHANGE in ECN oly to generate CO_COMMON */
        if (counter > 210 && counter < 220)
        {
             if (counter % 2)
            {
                /*Set ECN bits to 11*/
                sendBuffer[headerStart + 13] |= 0xc0;
                sendBuffer[GTP_HDR + 1] |= 0x03;
            }
            else
            {
                /*Set ECN bits to 00*/
                sendBuffer[headerStart + 13] &= 0x3f;
                sendBuffer[GTP_HDR + 1] &= 0xfc;
            }
            //ip checksum calculate
                UInt16 ipChecksum =  ipCksumCalc((UInt16) 20,
                                                  sendBuffer,
                                                  GTP_HDR);

                sendBuffer[GTP_HDR+10] = (ipChecksum >>8) & (0xFF);
                sendBuffer[GTP_HDR+11] = (ipChecksum & 0xFF);

        }
        else
        {
            /* GENERATE 100 PACKETS RND3 */
            setUnscaledAckNum(ueIndex,lcIndex,&sendBuffer[headerStart + 8]);
        }
    }
    else if (counter > 300 && counter <= 400)
    {
        /*CHANGING DF FLAG TO GENERATE CO_COMMON*/
        if (counter > 310 && counter < 321)
        {
            sendBuffer[GTP_HDR + 6] = sendBuffer[GTP_HDR + 6] ^ (1<<6);
            /*
            if (sendBuffer[GTP_HDR + 6] & 0x20)
            {
                sendBuffer[GTP_HDR + 7] = counter % 255;
            }
            else
            {
                sendBuffer[GTP_HDR + 6] &= 0x60;
                sendBuffer[GTP_HDR + 7] = 0x00;
            }*/
                UInt16 ipChecksum =  ipCksumCalc((UInt16) 20,
                                                  sendBuffer,
                                                  GTP_HDR);

                sendBuffer[GTP_HDR+10] = (ipChecksum >>8) & (0xFF);
                sendBuffer[GTP_HDR+11] = (ipChecksum & 0xFF);
         }
         else
         {
             /* GENERATE 100 PACKETS RND4 */
            setScaledAckNum(ueIndex,lcIndex,&sendBuffer[headerStart + 8]);
         }
    }
    else if (counter > 400 && counter <= 500)
    {
        /* GENERATE 100 PACKETS RND5 */
        setUnscaledSeqNum(ueIndex,lcIndex,&sendBuffer[headerStart + 4]);
        setUnscaledAckNum(ueIndex,lcIndex,&sendBuffer[headerStart + 8]);
    }
    else if (counter > 500 && counter <= 600)
    {
        /* GENERATE 100 PACKETS RND6 */
       setScaledSeqNum(ueIndex,lcIndex,&sendBuffer[headerStart + 4]);
       setUnscaledAckNum(ueIndex,lcIndex,&sendBuffer[headerStart + 8]);
    }
    else if (counter > 600 && counter <= 700)
    {
        /* GENERATE 100 PACKETS RND7 */
        setUnscaledAckNum(ueIndex,lcIndex,&sendBuffer[headerStart + 8]);
        alterWindowSize(ueIndex,lcIndex,&sendBuffer[headerStart + 8]);
    }
    else if (counter > 700 && counter <= 800)
    {
        setUnscaledAckNum(ueIndex,lcIndex,&sendBuffer[headerStart + 8]);
        /* GENERATE 100 PACKETS RND8 */
        if (counter < 730)
        {
            /*TOGGLE RESET FLAG and reset SYN and FIN */
           sendBuffer[headerStart + 13] = sendBuffer[headerStart + 13] ^ (1 << 2);
           sendBuffer[headerStart + 13] = sendBuffer[headerStart + 13] & 0xFC;
        }
        else if ( counter < 760)
        {
            /*change ttl hop*/
            sendBuffer[GTP_HDR + 8 ]++;
            //ip checksum calculate
            UInt16 ipChecksum =  ipCksumCalc((UInt16) 20,
                                            sendBuffer,
                                            GTP_HDR);
            sendBuffer[GTP_HDR+10] = (ipChecksum >>8) & (0xFF);
            sendBuffer[GTP_HDR+11] = (ipChecksum & 0xFF);
        }
        else if ( counter < 798)
        {
            /*ECN flag*/
            if (counter % 2)
            {
                /*Set ECN bits to 11*/
                sendBuffer[headerStart + 13] |= 0xc0;
                sendBuffer[GTP_HDR + 1] |= 0x03;
            }
            else
            {
                /*Set ECN bits to 00*/
                sendBuffer[headerStart + 13] &= 0x3f;
                sendBuffer[GTP_HDR + 1] &= 0xfc;
            }
            //ip checksum calculate
                UInt16 ipChecksum =  ipCksumCalc((UInt16) 20,
                                                  sendBuffer,
                                                  GTP_HDR);

                sendBuffer[GTP_HDR+10] = (ipChecksum >>8) & (0xFF);
                sendBuffer[GTP_HDR+11] = (ipChecksum & 0xFF);

        }
    }
    return;
    #endif
}

void initProfile6Params()
{
    profile6Params_g.isInitiator = 1;       /* Set if UL is the initiator for TCP connection */
    profile6Params_g.isFin = 1;             /* Set if UL is the first to send FIN request */
    profile6Params_g.isGenerator = 1;       /* 0 - Data Consumer, 1 - Data Generator, 2 - Both */
}


/****************************************************************************
 ** Function Name  : processULPacket
 ** Inputs         : None
 ** Outputs        : None
 ** Returns        : bytes
 ** Description    : This function process UL packets.
 *****************************************************************************/

void* processULPacket(void *ipAddr)
{
    UInt16 profileTimerCount = 0;
    UInt16 profileIdx = 0;
    UInt16 mixProfileCount = 0;
    UInt16 packetType = 0;
    UInt32 loopVar[MAX_PACKET_TYPE] ={0};
    UInt16 count = 0;
    UInt8  mulB = 0;

    UDouble64 		datavolume;
    UInt8 			lcId;
    static struct 		timespec currRxTime, lastDispTime;
    UInt64 			loadCalcTimeDiff =0;

    static struct timespec 	lasttime;
    static struct timespec 	currtime;
    struct sockaddr_in 	sockAddr;
    SInt8 			sockRxFd = -1;
    UInt32 			recvdSeqNum = 0; 
    UInt32 			diff = 0,addrLen =0;
    SInt32 			bytesRead =0;
    UInt64 			lastDisplayTickUL=0 ;
    UInt8 			recvBuffer[MAX_PAYLOAD_SIZE] = {0};
    UInt16 			i_ueId = 0;
    fd_set          	fdSock;
    SInt32          	selectReturn;
    struct timeval  	wTime;

    UInt16 	     		loop_counter=0;
    UInt16 	     		lc_counter=0;

    sockAddr = sockAddr_g;
    sockRxFd = sockRxFd_g;
    addrLen = sizeof(struct sockaddr_in);

    printf("Starting Time\n");
    PRINT_TIME();

    setSchedulePolicy (9,SCHED_RR);

    while(1)
    {
        wTime.tv_sec  = 0;                        /* Changed Required at ULTG */
        wTime.tv_usec = 200;
        FD_ZERO( &fdSock );
        FD_SET(sockRxFd, &fdSock );
        selectReturn = select( FD_SETSIZE - 1, &fdSock, 0, 0, &wTime );
        if( selectReturn < 0 )
        {
            perror("MU_ULTG Glue Select Failed!");
        }
        if( FD_ISSET(sockRxFd, &fdSock) && (mode_g >= 1) )
        {
            bytesRead = recvfrom( sockRxFd, 
                    recvBuffer, 
                    MAX_PAYLOAD_SIZE, 0,
                    (struct sockaddr *)&sockAddr, 
                    &addrLen);
            if(bytesRead > 0)
            {
                /*Last 2 Bytes of GTPU header is UE ID*/
                i_ueId = LTE_GET_U16BIT(&recvBuffer[0]);
                lcId = recvBuffer[2];
                if(i_ueId < ueId_g)
                {
                    ueDataFlow_g[i_ueId][lcId].rxData += (bytesRead - ADAPTER_HDR);
                    ueDataFlow_g[i_ueId][lcId].rxPkts++;
                }
            }
            if ( bytesRead == -1  || bytesRead == 0)
            {
                fprintf(stderr,"UL TG read Error bytesRead = %d\n",bytesRead);
            }
            memcpy ( &recvdSeqNum, (recvBuffer + bytesRead - 4), 4);  
#ifdef ULTG_BIG_ENDIAN
            tempPtr = (UInt8 *)&recvdSeqNum;
            recvdSeqNum =  GET_U32BIT(tempPtr);
#endif
            /* LIPA changes start */
#ifdef LIPA_GW_SEQ_CHK
            /* in case of LIPA, the sec tunnel will generate the seq num as 0xFFFFFFFF
             * so that sequence number is not checked */
            if((i_ueId < ueId_g) && (recvdSeqNum != 0xFFFFFFFF))
            /* LIPA changes end */
#else
            if(i_ueId < ueId_g)
#endif
            /* LIPA changes end */
            {
                if((ueDataFlow_g[i_ueId][lcId].rxSeq != recvdSeqNum))
                {
                    if(MissCntrs_g)
                    {
                        fprintf(MissCntrs_g,"\n [%01x] ==>[0x%04x != 0x%04x][0x%04x/0x%04x],%d",
                                i_ueId,
                                ueDataFlow_g[i_ueId][lcId].rxSeq,
                                recvdSeqNum,
                                ueDataFlow_g[i_ueId][lcId].rxMiss,
                                ueDataFlow_g[i_ueId][lcId].rxPkts,
                                recvdSeqNum - ueDataFlow_g[i_ueId][lcId].rxSeq);
                    }
                    if((firstBlockReceived_ultg[i_ueId][lcId]) && 
                            ((recvdSeqNum - ueDataFlow_g[i_ueId][lcId].rxSeq)<100000))
                    {
                        ueDataFlow_g[i_ueId][lcId].rxMiss += 
                            (recvdSeqNum - ueDataFlow_g[i_ueId][lcId].rxSeq);
                        /* SPR 3323 & 2987 Fix */
                        ueDataFlow_g[i_ueId][lcId].rxMiss_per_display += 
                            (recvdSeqNum - ueDataFlow_g[i_ueId][lcId].rxSeq);
                        /* SPR 3323 & 2987 Fix */
                    }
                    else
                    {
                        ueDataFlow_g[i_ueId][lcId].rxMiss++;
                        /* SPR 3323 & 2987 Fix */
                        ueDataFlow_g[i_ueId][lcId].rxMiss_per_display++;
                        /* SPR 3323 & 2987 Fix */
                    }
                    ueDataFlow_g[i_ueId][lcId].rxSeq = recvdSeqNum + 1;
                }
                else
                {
                    ueDataFlow_g[i_ueId][lcId].rxSeq++;
                }
                firstBlockReceived_ultg[i_ueId][lcId]= 1;
            }

            datavolume += (bytesRead - GTP_HDR);
        }
        clock_gettime(CLOCK_REALTIME,&currRxTime);
        loadCalcTimeDiff = ((currRxTime.tv_sec - lastDispTime.tv_sec)*1000 + 
                ((currRxTime.tv_nsec - lastDispTime.tv_nsec)/1000000));
        diff = loadCalcTimeDiff;
        /*20868 +*/
        for(loop_counter = ueStart_g; loop_counter< ueId_g ; loop_counter++)
        /*20868 -*/
        {
            for(lc_counter = 0; lc_counter <gtpDb_g[loop_counter].lcCnt ; lc_counter++)
            {				
                if (loadCalcTimeDiff >= (throughputDisplayVar_g/timeInMs_g))
                {
                    /* SPR 20441 Changes Start*/
                    datavolume *= 1000;
                    /* SPR 20441 Changes End*/
                    datavolume *= 8 ;
                    datavolume /= diff;

                    /* Printing Statistics*/
                    MbPS_LOAD(current_sector,loop_counter,gtpDb_g[loop_counter].rbInfo[lc_counter].lcId);

                    datavolume =0;
                    gtpDb_g[loop_counter].rbInfo[lc_counter].lastDisplayTick = 
                        gtpDb_g[loop_counter].rbInfo[lc_counter].txTickCount;
                    lastDispTime.tv_sec = currRxTime.tv_sec;
                    lastDispTime.tv_nsec = currRxTime.tv_nsec;
                }
            }
        }
    }		
    return NULL;
}
/****************************************************************************
 ** Function Name  : processDLPacket
 ** Inputs         : None
 ** Outputs        : None
 ** Returns        : return 0 on success
 ** Description    : This function process DL Packets.  
 *****************************************************************************/

UInt32 processDLPacket(void *ipAddr)
{
    UInt32 currentTick = 0;
    UInt16 packetType = 0;
    UInt16 profileIdx = 0;
    UInt16 profileTimerCount = 0;
    UInt8 			lcId =0,temp_lcId=0;
    UDouble64 		datavolume;
    static struct timespec 	lasttime;
    static struct timespec 	currtime;
    struct timespec 	timer;
    struct sockaddr_in 	sockAddr;
    struct timespec 	oldStartTime, startTime, endTime;
    UInt32 			bytesSend = 0,loopVar[MAX_PACKET_TYPE] ={0};
    UInt32 			diff = 0, tempVar =0;
    SInt8 			sockTxFd = -1;
    UInt8 			sendBuffer[MAX_PAYLOAD_SIZE] = {0};
    UInt8 			sendBuffer1[MAX_PAYLOAD_SIZE] = {0};
    UInt8 			sendBuffer2[MAX_PAYLOAD_SIZE] = {0};
    UInt32 			mytickcount =0 ;
    UInt32	 		mypktcount =0 ;
    UInt32 			startflag_g =1;
    UInt64 			lastDisplayTickDL=0 ;
    UInt64 			adaptiveDiff =0;
    UInt64 			sleepTime =0;
    UInt16 			ipChecksum = 0;
    UInt16 			udpChecksum = 0;
    UInt16 			headerStart = 5;
    UInt32 			i =0 ;
    UInt32 			j =0;
    static UInt16           tick_tracker = 0;
    UInt8 pktcount=0;
    struct timeval 		t2;
    /* SPR 5857 : LOAD DISTRIBUTION */
#ifdef SUPPORT_64_UE                                            
    UInt32 maxNumOfPktPerTick = 100;
    UInt32 remainPktLcCount   = 0;
#endif
    /* SPR 5857 : LOAD DISTRIBUTION */


    UInt16 			i_ueCnt = 0;
    FILE  *sps_logs;

    sps_logs = fopen("ul_packet_trace.log","w+");
    if(sps_logs == NULL)
    {
        fprintf(stderr,"Unable to open the file dl_packet_trace for writing.......\n");
        exit(0);
    }

    openUdpSocket(OPEN_TX_SOCKET,(char *)ipAddr,NULL,&sockTxFd,&sockAddr);

    fprintf(stderr,"Temp Val is %u\n",tempVar); 
    sleepTime = 1000000;
    setSchedulePolicy (10,SCHED_FIFO);
    /* SPR 5857 : LOAD DISTRIBUTION */
#ifdef SUPPORT_64_UE                                          
    UInt32 totalNumOfPktPerTick=0;
    for( i_ueCnt = ueStart_g; i_ueCnt < ueId_g;i_ueCnt++)
    {
        totalLcCount += gtpDb_g[i_ueCnt].lcCnt;
        for(temp_lcId=0; temp_lcId< gtpDb_g[i_ueCnt].lcCnt; temp_lcId++)
        {
            if(gtpDb_g[i_ueCnt].rbInfo[temp_lcId].sps_lc != 1)
            {
                for (profileIdx = 0; profileIdx < gtpDb_g[i_ueCnt].rbInfo[temp_lcId].tpCount)
                {
                    for( packetType =0; packetType < gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].mixProfileCount; packetType++)
                    {
                        totalNumOfPktPerTick += gtpDb_g[i_ueCnt].rbInfo[temp_lcId].tgProfileInfo[profileIdx].remainingPkt[packetType];
                    }
                }
            }
        }
        maxNumOfPktPerTick = ((totalNumOfPktPerTick) / gtpDb_g[0].rbInfo[0].noOfTicks)+2;
        fprintf(stderr, "totalLcCount=%d, fixedPktPerTick=%f, maxNumOfPktPerTick=%d\n",totalLcCount, gtpDb_g[0].rbInfo[0].tgProfileInfo[profileIdx].fixedPktPerTick[packetType], maxNumOfPktPerTick);
    }

    //maxNumOfPktPerTick = ((totalNumOfPktPerTick) / gtpDb_g[0].rbInfo[0].noOfTicks)+2;
    //fprintf(stderr, "totalLcCount=%d, fixedPktPerTick=%f, maxNumOfPktPerTick=%d\n",totalLcCount, gtpDb_g[0].rbInfo[0].fixedPktPerTick[packetType], maxNumOfPktPerTick);
#endif
    /* SPR 5857 : LOAD DISTRIBUTION */

    while(1)
    {
        if(mode_g != 1)
        {
            clock_gettime(CLOCK_REALTIME,&startTime);
            if(0==startflag_g)
            {
                if(startTime.tv_sec == oldStartTime.tv_sec)
                {
                    adaptiveDiff = (startTime.tv_nsec - oldStartTime.tv_nsec) - sleepTime;
                }
                else if (startTime.tv_sec > oldStartTime.tv_sec)
                {
                    adaptiveDiff = 1000000000 - (oldStartTime.tv_nsec - startTime.tv_nsec) - sleepTime;
                }

                if(adaptiveDiff > 700000)
                {
                    missTimeCount_g = missTimeCount_g + adaptiveDiff;
                    adaptiveDiff =0;
                    missTickCount_g ++;
                }
            }
            else
            {
                adaptiveDiff =0;
                startflag_g =0;
            }
            sleepTime = 1000000*timeInMs_g-adaptiveDiff;

            oldStartTime.tv_sec = startTime.tv_sec;
            oldStartTime.tv_nsec = startTime.tv_nsec;
            timer.tv_sec = 0;
            timer.tv_nsec = sleepTime;

            /* SPR 5857 : LOAD DISTRIBUTION */
#ifdef SUPPORT_64_UE                                            
            remainPktLcCount=0;
#endif
            /* SPR 5857 : LOAD DISTRIBUTION */
            for( i_ueCnt = ueStart_g; i_ueCnt < ueId_g;i_ueCnt++)
            {
                ueTurn_g = i_ueCnt;
                gettimeofday(&t2,NULL);
                gtpDb_g[ueTurn_g].timeDiff = t2.tv_sec - timer_g;
                for(temp_lcId=0;temp_lcId<gtpDb_g[ueTurn_g].lcCnt;temp_lcId++) 
                {
                    if(gtpDb_g[i_ueCnt].rbInfo[temp_lcId].sps_lc == 1)
                    {
                        if(spsTraffic[tick_tracker].data_present == 1)
                        {
                            if(spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].data == 1)
                            {
                                for(pktcount=0;pktcount<=spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].numPkt; pktcount++)
                                {
                                    ueTurn_g = spsTraffic[tick_tracker].ueInfo[i_ueCnt].ue_id;
                                    lcId_g = spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].lc_id;
                                    createDownlinkPacket(spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].SendBuffer,
                                            spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].packet_size,
                                            spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].lc_id
                                            );
                                    memcpy(( spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].SendBuffer +
                                                spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].packet_size + ADAPTER_HDR - 4  ),
                                            &ueDataFlow_g[ueTurn_g][lcId_g].txSeq, 4);
                                    ueDataFlow_g[ueTurn_g][lcId_g].txSeq++;
                                    bytesSend = sendto(sockTxFd,
                                            spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].SendBuffer,
                                            spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].packet_size +
                                            ADAPTER_HDR,0,
                                            (struct sockaddr*)&sockAddr,
                                            sizeof(struct sockaddr_in));
                                    fprintf(sps_logs,"\nSENT DATA PACKET SIZE = %d",
                                            spsTraffic[tick_tracker].ueInfo[i_ueCnt].lcInfo[temp_lcId].packet_size);
                                    if ( bytesSend == -1 )
                                    {
                                        fprintf(stderr,"\nDL TG send to failed\n");
                                    }
                                    ueDataFlow_g[ueTurn_g][lcId_g].txData += bytesSend - ADAPTER_HDR ;
                                    datavolume += (bytesSend - ADAPTER_HDR);
                                    seqNum_g++;
                                    ueDataFlow_g[ueTurn_g][lcId_g].txPkts++;

                                }

                            }
                        }
                    }
                    else
                    {
                             //fprintf (stderr, "Going in NON SPS LC in DL packer process TICK TRKR VAL : %u\n", tick_tracker);
                        profileIdx = gtpDb_g[ueTurn_g].rbInfo[temp_lcId].profileIdx;

                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].txTickCount = 
                            gtpDb_g[ueTurn_g].rbInfo[temp_lcId].txTickCount +1;

                        for(packetType =0; packetType < gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].mixProfileCount; packetType++)
                        {
                            if(noOfPktVar_g != 0)
                            { 
                                loopVar[packetType] = noOfPktVar_g;
                                gtpDb_g[ueTurn_g].rbInfo[temp_lcId].extraBurstCounter = 0;
                            }
                            else
                            {
                                loopVar[packetType] = gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].fixedPktPerTick[packetType]+1;
                                gtpDb_g[ueTurn_g].rbInfo[temp_lcId].extraBurstCounter = 
                                    gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].remainingPkt[packetType];
                            }
                            createDownlinkPacket(gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].SendBuffer[packetType], 
                                    gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].packetSize[packetType], 
                                    gtpDb_g[ueTurn_g].rbInfo[temp_lcId].lcId);

                            lcId = gtpDb_g[ueTurn_g].rbInfo[temp_lcId].lcId;
                            if( noOfPktVar_g == 0)
                            {

                                /* SPR 5857 : LOAD DISTRIBUTION */
#ifdef SUPPORT_64_UE                                            
                                UInt32 pktSentForLc = gtpDb_g[i_ueCnt].rbInfo[temp_lcId].remainingPktSent;
                                if(remainPktLcCount<= maxNumOfPktPerTick && 
                                        pktSentForLc < gtpDb_g[i_ueCnt].rbInfo[temp_lcId].tgProfileInfo[profileIdx].remainingPkt[packetType])       
#else                                                
                                    if((gtpDb_g[ueTurn_g].rbInfo[temp_lcId].txTickCount%
                                                gtpDb_g[ueTurn_g].rbInfo[temp_lcId].noOfTicks)<= 
                                            gtpDb_g[ueTurn_g].rbInfo[temp_lcId].extraBurstCounter)
#endif
                                    {
                                        loopVar[packetType] = gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].fixedPktPerTick[packetType] +1;
#ifdef SUPPORT_64_UE                                            
                                        remainPktLcCount++;
                                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].remainingPktSent++;
#endif
                                        /* SPR 5857 : LOAD DISTRIBUTION */
                                    }
                                    else
                                    {
                                        loopVar[packetType] = gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].fixedPktPerTick[packetType];
                                    }
                            }
                            else
                            {
                                loopVar[packetType] = noOfPktVar_g;
                            }
                            /*+ TG Changes for Qos testing */
                            if(gtpDb_g[ueTurn_g].rbInfo[temp_lcId].upDownFlag % 2  == 0 )
                            {
                                if(( gtpDb_g[ueTurn_g].timeDiff >= total_duration_g) && BURST_FLAG )
                                {

                                    if( (gtpDb_g[ueTurn_g].rbInfo[temp_lcId].bitRateFlag == 1)  &&  
                                            (gtpDb_g[ueTurn_g].rbInfo[temp_lcId].extraBurstFlag == 1))
                                    {
                                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].packetSize[packetType] += 
                                            gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].varPacketSize[packetType];
                                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].upDownFlag++;
                                        timer_g = t2.tv_sec;
                                    }
                                }
                            }
                            else if (gtpDb_g[ueTurn_g].rbInfo[temp_lcId].upDownFlag % 2  == 1)
                            {
                                if((gtpDb_g[ueTurn_g].timeDiff >= burst_time_g) && BURST_FLAG )
                                {
                                    if( (gtpDb_g[ueTurn_g].rbInfo[temp_lcId].bitRateFlag == 1)  &&  
                                            (gtpDb_g[ueTurn_g].rbInfo[temp_lcId].extraBurstFlag == 1))
                                    {
                                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].packetSize[packetType] -= 
                                            gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].varPacketSize[packetType];
                                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].upDownFlag++;
                                        timer_g = t2.tv_sec;

                                    }
                                }
                            }
                        }
                        /*- TG Changes for Qos testing */
                        for(packetType = 0;packetType < gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].mixProfileCount; packetType++)
                        {
                            for(tempVar = loopVar[packetType];tempVar;tempVar--) 
                            {
                                memcpy ((gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].SendBuffer[packetType] + 
                                            gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].packetSize[packetType] + ADAPTER_HDR - 4 ),
                                        &ueDataFlow_g[ueTurn_g][lcId].txSeq, 4);

                                /*SPR 8293 Fix Start*/
                                /*Prepare GTPU Header*/
                                createDownlinkPacket(gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].SendBuffer[packetType], 
                                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].packetSize[packetType], 
                                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].lcId);
                                /*SPR 8293 Fix End*/
                                /* Set TS and SN value in RTP header */
                                setSNnTSinRTPPkt(ueTurn_g,temp_lcId,gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].SendBuffer[packetType]);
                               /* Set SYN and ACK fields in TCP header */
                              controlflowCallforTCP(ueTurn_g,temp_lcId,
                              gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].SendBuffer[packetType],
                              bytesSend);

                                /* Set IP-ID value in IP header */
                                setIPIDinIPPkt(ueTurn_g,temp_lcId,gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].SendBuffer[packetType]);
                                ueDataFlow_g[ueTurn_g][lcId].txSeq++;
                                bytesSend = sendto(sockTxFd,
                                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].SendBuffer[packetType], 
                                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].packetSize[packetType] + ADAPTER_HDR,
                                        0,(struct sockaddr*)&sockAddr,
                                        sizeof(struct sockaddr_in));

                                lcId = gtpDb_g[ueTurn_g].rbInfo[temp_lcId].lcId;
                                ueDataFlow_g[ueTurn_g][lcId].txData += bytesSend - ADAPTER_HDR; 

                                if ( bytesSend == -1 )
                                {
                                    perror("\n :DL TG send to failed\n");
                                }
                                datavolume += (bytesSend - ADAPTER_HDR);

                                seqNum_g++;
                                if (trafficProfileVar_g) 
                                {
                                    bytesSend = sendto(sockTxFd,
                                            sendBuffer1,
                                            136 + ADAPTER_HDR,0,
                                            (struct sockaddr*)&sockAddr,
                                            sizeof(struct sockaddr_in));
                                    bytesSend = sendto(sockTxFd,
                                            sendBuffer2,
                                            40 + ADAPTER_HDR,0,
                                            (struct sockaddr*)&sockAddr,
                                            sizeof(struct sockaddr_in));
                                    bytesSend = sendto(sockTxFd,
                                            sendBuffer2,
                                            40 + ADAPTER_HDR,0,
                                            (struct sockaddr*)&sockAddr,
                                            sizeof(struct sockaddr_in));
                                }
                                ueDataFlow_g[i_ueCnt][lcId].txPkts++;
                                pktCounter_g++;
                            }
                        }



                        if((gtpDb_g[ueTurn_g].rbInfo[temp_lcId].txTickCount % 
                                    gtpDb_g[ueTurn_g].rbInfo[temp_lcId].noOfTicks)==0)

                        {
#ifdef PACKETS_SENT_PER_SEC
                            fprintf(stderr,"No of packets sent in %d Ticks(1sec): %d \n",
                                    gtpDb_g[ueTurn_g].rbInfo[temp_lcId].noOfTicks,
                                    pktCounter_g);
#endif
                            pktCounter_g = 0;
#ifdef SUPPORT_64_UE                                            
                            /* SPR 5857 : LOAD DISTRIBUTION */
                            gtpDb_g[ueTurn_g].rbInfo[temp_lcId].remainingPktSent=0;
                            /* SPR 5857 : LOAD DISTRIBUTION */
#endif
                        }
                        if(gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].profileTimerCount == 0)
                        {
                            profileIdx++;

                            if(profileIdx == gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tpCount)
                            {
                                profileIdx = 0;
                            }
                            gtpDb_g[ueTurn_g].rbInfo[temp_lcId  ].tgProfileInfo[profileIdx].profileTimerCount = 
                                gtpDb_g[ueTurn_g].rbInfo[temp_lcId  ].tgProfileInfo[profileIdx].profileTimer;

                        }
                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].profileTimerCount--;
                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].profileIdx = profileIdx;

                        /* resetting the entries if an LC timer equals to total ecexution time */
                        if (gtpDb_g[ueTurn_g].rbInfo[temp_lcId].isLCTimerPresent)
                        {
                            if(gtpDb_g[ueTurn_g].rbInfo[temp_lcId].totalExecTime == 0)
                            {
                                for (profileIdx = 0; profileIdx < gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tpCount; profileIdx++)
                                {
                                    for( packetType = 0; 
                                            packetType < gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].mixProfileCount; packetType++)
                                    {
                                        gtpDb_g[ueTurn_g].rbInfo[temp_lcId].tgProfileInfo[profileIdx].packetSize[packetType]= 0;     
                                    }
                                }
                            }
                            else
                            {
                                gtpDb_g[ueTurn_g].rbInfo[temp_lcId].totalExecTime--;
                            }
                        }
                    }
                }
            }
            tick_tracker++;
            if( tick_tracker == 1000)
                tick_tracker = 0;
            ueTurn_g = 0; /*Reset Round Robin ID to 0*/
            mytickcount++;
            mytickcount%=1000;
            clock_gettime(CLOCK_REALTIME,&endTime);
            if(timer.tv_nsec > 
                    (((endTime.tv_sec - startTime.tv_sec)*1000000000)+
                     (endTime.tv_nsec - startTime.tv_nsec)))
            { 
                timer.tv_nsec = timer.tv_nsec -(((endTime.tv_sec - startTime.tv_sec)*1000000000)+
                        (endTime.tv_nsec - startTime.tv_nsec));
            }
            else
            {
                timer.tv_nsec = 1000000*timeInMs_g -  ((((endTime.tv_sec - startTime.tv_sec)*1000000000)+
                            (endTime.tv_nsec - startTime.tv_nsec)) - timer.tv_nsec); 
            }
            if(timer.tv_nsec > 1000000*timeInMs_g)
            {
                timer.tv_nsec = 1000000*timeInMs_g;
            }
        }
        else
        {
            timer.tv_sec = 1;
        }

#ifndef CAVIUM_BOARD
        pselect(0,NULL,NULL,NULL,&timer,NULL);
#else
        struct timespec rem_time = {0};
        SInt32 ret_value = -1;
        do
        {
            ret_value = nanosleep(&timer,&rem_time);
            if(ret_value < 0)
            {
                timer.tv_nsec = rem_time.tv_nsec;
                timer.tv_sec = rem_time.tv_sec;
            }
        }
        while(ret_value < 0);
#endif
    }
    fclose(sps_logs);
}

/****************************************************************************
 ** Function Name  : main
 ** Inputs         : None
 ** Outputs        : None
 ** Returns        : return 0. 
 ** Description    : This function is entry point for downlink TG. It takes 
 **                : runtime paramters for thread binding and other processing. 
 *****************************************************************************/
SInt32 main(SInt32 argc, SInt8 *argv[])
{
    FILE 		*fp = NULL;
    FILE 		*temp_fp;
    FILE 		*traffic_profile_fp = NULL;
    UInt8		spsIndex = 0;
    char 		buf[100],*current,*timerCurrent = NULL;
    char 		str1[100],str2[100];
    UInt32 		loopvar,lc_counter;
    UInt16 		ipChecksum = 0;
    UInt16 		udpChecksum = 0;
    UInt16 		headerStart = 5;
    UInt8 		tp_counter = 0;
    UInt8 		lc_count = 0;
    UInt8 		lcId = 0;
    UInt32 		temp=0;
    UInt16 		ueIndex =0;
    UInt16 		no_of_ue = 0;
    cpu_set_t 	cpuSetSch;
    UInt64 		coreNum = 0;
    pid_t 		p = 0;
    struct sockaddr_in 	sockAddr;
    SInt8 			sockRxFd = -1;
    UInt8		index = 0;
    UInt16 count = 0;
    UInt16 i = 0;
    UInt16 tpCounter = 0;
    UInt16 packetType = 0; 
    char * tempCurrent = NULL;
    char * currentPktSz = NULL;
    char * currentThruput = NULL;
    UInt8 s_count=0 ; 
    UInt16 repetition = 0;
    UInt8 interPktArvlTm = 0;
    UInt16 offset = 0;

	fprintf(stderr,"UL-Traffic Generator is starting......................\n");
	if (argc < 11) 
	{
		fprintf(stderr,"Usage is ./ulTrafficGen <core num to bind> <source ip address>"
				" <destination ip address> <traffic gen mode> <number of packets>"
				" <Time in ms to pump> <throuhput display in ms> <traffic-profile>"
				" <lastUE><start_ueID> \n"); 
		return 0;
	}
	
	coreNum = atoi(argv[1]);
	CPU_ZERO(&cpuSetSch);
	CPU_SET(coreNum ,&cpuSetSch);
	CPU_SET(coreNum ,&cpuSetSch);
	assert( -1 != sched_setaffinity(p, sizeof(cpu_set_t), &cpuSetSch));
	
	noOfPktVar_g = atoi(argv[5]);
	timeInMs_g = atoi(argv[6]);
	throughputDisplayVar_g = atoi(argv[7]);
	trafficProfileVar_g = atoi(argv[8]); 
        no_of_ue = atoi(argv[9]);

        if(timeInMs_g == 0)
        {
		fprintf(stderr, " \nTime in Ms to pump cannot be equal to 0\n");
		return 0;

        } 	
        if(noOfPktVar_g != 0)
        {
		fprintf(stderr," \nNo of Packets should be equal to zero as it is"
				" calculated from throughput mentioned in TGConfig.txt\n");
		return 0;    
    
        }
        /*20868 +*/
	if(argc > 9)
	{
		ueId_g = atoi(argv[9]);
		if(ueId_g < 0 || ueId_g > MAX_MULTI_UE)
		{
			ueId_g = 0;
		}
	}
	MissCntrs_g= fopen("DL_dropped_seq.txt","w+");
	if(argc > 10)
	{
		ueStart_g = atoi(argv[10]);
		if(ueStart_g < 0 || ueStart_g >= MAX_MULTI_UE)
		{
			ueStart_g = ueId_g;
		}
		if(ueId_g < ueStart_g)
		{
			ueStart_g = ueId_g;
		}
	}
        /*20868 -*/

	/* Read Configuration info from UESimConfiguration.cfg file */		
	retrive_trafficGenInfo_from_configfile();

	/* Open Rx Socket to receive data and identify the UL TG Instance */
	openUdpSocket(OPEN_RX_SOCKET,NULL,(char *)argv[2],&sockRxFd,&sockAddr);
	
	/* SPR 4671 Fix */
	while((sockAddr.sin_port) != htons(trafficGeneratorAdapterRxPort_g[index_g]))
	{
		index_g ++;
	}
	/* SPR 4671 Fix */
	sockRxFd_g = sockRxFd;
	sockAddr_g = sockAddr;	
	
	gettimeofday(&t1,NULL);
	printf("the time before while is %ld\t",t1.tv_sec);
	timer_g = t1.tv_sec;
    /* SPR 13326 Changes Start */
/*SPR 21653 changes start*/
    /* SPR 13326 Changes End */
	fp = fopen("../cfg/ulTrafficGen.txt","rb");
/*SPR 21653 changes end*/
        if(fp == PNULL)
        {
                fprintf(stderr,"\nError: Unable to open Config file for UL-TG \n");
                exit(0);
        }
	/* SPR 4671 Fix */
	if( index_g == 0)
	{
		while(fgets(buf,100,fp))
        	{
                	if(sscanf(buf,"SECTOR-ID=%d",&temp) > 0)
                	{
				current_sector = temp;
				break;
			}
		}

	}
	else if( index_g == 1)
	{
		UInt8 loop_count = 0;
		while(fgets(buf,100,fp))
        	{
                	if(sscanf(buf,"SECTOR-ID=%d",&temp) > 0)
                	{
				if(loop_count == 1)
				{
					current_sector = temp;
					break;
				}
			loop_count++;
			}
		}
	}
	else if( index_g == 2)
	{
		UInt8 loop_count = 0;
		while(fgets(buf,100,fp))
        	{
                	if(sscanf(buf,"SECTOR-ID=%d",&temp) > 0)
                	{
				if(loop_count == 2)
				{
					current_sector = temp;
					break;
				}
			loop_count++;
			}
		}
	}
	rewind(fp);
	printf("\n ,current_sector = %d\n",current_sector); 
	/* SPR 4671 Fix */
        while(fgets(buf,100,fp))
        {
		if(sscanf(buf,"SECTOR-COUNT=%d",&temp) > 0)
		{
			sector_count_g = temp;
			printf("\n SECTOR-COUNT=%d",sector_count_g);	
		}
		else if(sscanf(buf,"SECTOR-ID=%d",&temp) > 0)
		{
			sector_id_g = temp;
			if( (sector_count_g > 1) && (sector_id_g != current_sector))
			{
				while( temp != current_sector)
				{
					memset(buf,100,sizeof(buf));
					fgets(buf,100,fp);
					sscanf(buf,"SECTOR-ID=%d",&temp);
				}
				sector_id_g = temp;
			}
			printf("\n SECTOR-ID=%d",sector_id_g);	
		}
                else if(sscanf(buf,"UE-COUNT=%d",&temp) > 0)
                {
                    ueCount_g = temp;
                    /*SPR 20868 +-*/
                    printf("\n UE-COUNT=%d",ueCount_g);	
                }
                else if (sscanf(buf,"UE-ID=%d",&temp) > 0)
                {
                    ueIndex = gtpDb_g[temp].ueId = temp;
		    printf("\n UE-ID=%d",ueIndex);	
                }
                else if (sscanf(buf,"LC-COUNT=%d",&temp) > 0)
                {

                    gtpDb_g[ueIndex].lcCnt = temp;
		    printf("\n LC-COUNT=%d", gtpDb_g[ueIndex].lcCnt);	
                    for (lcId=0;lcId<gtpDb_g[ueIndex].lcCnt;lcId++)
                    {
                        UInt8 isTimerPresent = 0;
                        memset(buf,0,sizeof(buf));
                        fgets(buf,100,fp);
                        if (sscanf(buf,"LC-ID=%d",&temp) > 0)
                        {
                            gtpDb_g[ueIndex].rbInfo[lcId].lcId = (UInt8)temp;
                            printf("\n LC-ID=%d",gtpDb_g[ueIndex].rbInfo[lcId].lcId);	
                            current=strtok(buf," ");
                            current=strtok(NULL," ");
                            strcpy(str1,current);

                            if( (strncmp(str1,"SPS",3)) == 0)
                            {
                                gtpDb_g[ueIndex].rbInfo[lcId].sps_lc = 1;
                                current=strtok(NULL," ");
                                strcpy(str1,current);
                            }

                            current=strtok(NULL," ");
                            if (current!= '\n')
                            {
                                strcpy(str2,current);
                                isTimerPresent = 1;
                            }

                            sscanf(str1,"TP-ID=%d",&temp);
                            current = strtok(str1,",");
                            gtpDb_g[ueIndex].rbInfo[lcId].tpId[count] = temp;
                            printf("\n TP-ID=%d",gtpDb_g[ueIndex].rbInfo[lcId].tpId[count]);	
                            current = strtok(NULL,",");
                            count++;
                            while(current != NULL)
                            {
                                gtpDb_g[ueIndex].rbInfo[lcId].tpId[count] = atoi(current);
                                printf("\n TP-ID=%d",gtpDb_g[ueIndex].rbInfo[lcId].tpId[count]);	
                                current = strtok(NULL,",");
                                count++;
                            }
                            gtpDb_g[ueIndex].rbInfo[lcId].tpCount = count;
                            count = 0;
                            // Only if timer is present
                            if (isTimerPresent)
                            {
                                /* processing for LC-TIMER */
                                sscanf(str2,"LC-TIMER=%d",&temp);
                                gtpDb_g[ueIndex].rbInfo[lcId].totalExecTime = temp;
                                if(temp)
                                {
                                    gtpDb_g[ueIndex].rbInfo[lcId].isLCTimerPresent = 1;
                                    printf ("lc timer \n");
                                }
                                printf("\n LC-TIMER=%d\n",gtpDb_g[ueIndex].rbInfo[lcId].totalExecTime);
                            }
                            else
                            {
                                gtpDb_g[ueIndex].rbInfo[lcId].totalExecTime = 0;
                                gtpDb_g[ueIndex].rbInfo[lcId].isLCTimerPresent = 0;
                            }

                            memset(str1,0,sizeof(str1));
                            if(current != NULL)
                            {
                                strcpy(str1,current);
                            }
                            temp = strncmp(str1,"NON-GBR",7);

                            if(temp == 0)
                            {
                                gtpDb_g[ueIndex].rbInfo[lcId].bitRateFlag = 1;	
                                printf("Bit rate flag is set for LC-ID : %d\n", gtpDb_g[ueIndex].rbInfo[lcId].lcId);

                            }
                            else  
                            {
                                gtpDb_g[ueIndex].rbInfo[lcId].bitRateFlag = 0;
                                printf("nothing for LC-ID= %d\n",gtpDb_g[ueIndex].rbInfo[lcId].lcId);
                            }
                        }
                        else 
                        {
                            fprintf(stderr,"File Reading Error.............\n");
                            exit(0);
                        }
                    }
              }
	      else if (sscanf(buf,"Load Variation Percentage=%d",&temp))
	      {
	      		load_variation_g = temp;
			printf("load variation is %f\n",load_variation_g);
			if(load_variation_g == 0 )
			{
				gtpDb_g[ueIndex].rbInfo[lcId].bitRateFlag = 0;
			}
	      }	
	      else if (sscanf(buf,"Time for the burst(in seconds)=%d",&temp))
	      {
	      		burst_time_g = temp;
			printf("burst_time_g is %d\n",burst_time_g);
			if(burst_time_g == 0 )
			{
				gtpDb_g[ueIndex].rbInfo[lcId].bitRateFlag = 0;
			}
	      }
	      /*+ TG Changes for QOS Testing */
	      else if(sscanf(buf,"Burst Periodicity(in seconds)=%d",&temp))
	      {
		      total_duration_g = temp;
		      printf("total_duration_g is %d\n",total_duration_g);
		      if(total_duration_g == 0)
		      {
			      gtpDb_g[ueIndex].rbInfo[lc_count].bitRateFlag = 0;
		      }
		      if( sector_id_g == current_sector)
		      {
			      break;
		      }
	      }
	      /*- TG Changes for QOS Testing */
	      	
		
        }
	
	traffic_profile_fp = fopen("../cfg/TGConfig.txt","rb");
	if (traffic_profile_fp == PNULL)
	{
		 fprintf(stderr,"unable to open the configuration file TGConfig.txt............EXITING NOW ");
		 exit(0);
	}
	temp_fp = traffic_profile_fp;
	memset(buf,0,sizeof(buf));
    /*SPR 20868 +*/
	for (loopvar=ueStart_g; loopvar< ueId_g;loopvar++)
    /*SPR 20868 -*/
    {
        rewind(traffic_profile_fp);
        fgets(buf,sizeof(buf),traffic_profile_fp);
        fgets(buf,sizeof(buf),traffic_profile_fp);
        for(lc_counter= 0;lc_counter <gtpDb_g[loopvar].lcCnt; lc_counter++)
        {
            offset =0;
            interPktArvlTm = 0;
            udpChecksum = 0;
            ipChecksum = 0;
            rewind(traffic_profile_fp);
            fgets(buf,sizeof(buf),traffic_profile_fp);
            fgets(buf,sizeof(buf),traffic_profile_fp);
            while(fgets(buf,sizeof(buf),traffic_profile_fp))
            {	
                if(sscanf(buf,"TP-ID=%d",&temp)>0 )
                {
                    for(tpCounter = 0;tpCounter<gtpDb_g[loopvar].rbInfo[lc_counter].tpCount;tpCounter++)
                    {
                        count = 0;
                        if(gtpDb_g[loopvar].rbInfo[lc_counter].tpId[tpCounter] == temp)
                        {
                            current= strtok_r(buf," ",&buf);
                            current= strtok_r(NULL," ",&buf); //Moving current to next substring i.e. packetSize
                            current= strtok_r(NULL," ",&buf);
                            current= strtok_r(NULL," ",&buf);
                            current= strtok_r(NULL," ",&buf);
                            
                            strcpy(str1,current);
                            tempCurrent = strtok(str1,",");
                            while(tempCurrent != NULL)
                            {
                                tempCurrent = strtok(NULL,",");
                                count++;
                            }
                            printf("count = %d",count);
                            gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].mixProfileCount = count;
                            currentPktSz = strtok(current,",");
                            
                            if (gtpDb_g[loopvar].rbInfo[lc_counter].sps_lc == 1)
                            {
                                gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[spsIndex].pktSize = atoi(currentPktSz);
                            }
                            else
                            {
                                for(i = 0;i<count&&(currentPktSz != NULL);i++)
                                {
                                    gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].packetSize[i] = atoi(currentPktSz);
                                    currentPktSz = strtok(NULL,",");
                                }
                            }
                            current = strtok_r(NULL," ",&buf); //Moving current to next substring i.e. thruput
                            currentThruput = strtok(current,",");
                            for(i = 0;i<count&&(currentThruput != NULL);i++)
                            {
                                gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].throughput[i] = atof(currentThruput);
                                currentThruput = strtok(NULL,",");
                            }
                            current = strtok_r(NULL," ",&buf); //Moving current to next substring i.e. profileTimer
                            if (gtpDb_g[loopvar].rbInfo[lc_counter].sps_lc == 1)
                            {
                                gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[spsIndex].interPktArvlTm = atoi(current);
                            }
                            else
                            {
                                gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].profileTimer = atoi(current);
                                /* Initialize the profileTimerCount */
                                gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].profileTimerCount = 
                                    gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].profileTimer;
                            }
                            if (gtpDb_g[loopvar].rbInfo[lc_counter].sps_lc == 1)
                            {
                                gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[spsIndex].repetition = gtpDb_g[loopvar].rbInfo[lc_counter].totalExecTime;
                                if (gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[spsIndex].repetition == 0)
                                {
                                    gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[spsIndex].repetition = 1000;
                                }
                            }
                            if (gtpDb_g[loopvar].rbInfo[lc_counter].sps_lc == 1)
                            {
                                 fprintf (stderr, "Now going in sps_LC******\n");
#ifdef ROHC_PROFILE_6
#ifndef UE_IPV6_ADDR
								 /*  This buffer is  used as a hardcoded value which contains IP/TCP header
								  *  for UE (containing UE IP address). The same packet is sent for multiple UE
								  *  (w/o change in UE IP Address)*/
								 UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
										 0x45,0x02,0x00,0x00,0x23,0x21,0x40,0x00, /* IP header */
										 0xff,0x06,0x00,0x00,0xe0,0x00,0x00,0xfb,
										 0xac,0x10,0x51,0x15,
										 0x14, 0xe9, 0x14, 0xea, 0x03, 0x46, 0xd0, 0x0a, /* TCP Header */
										 0x10, 0x02, 0x30, 0x04, 0xa0, 0x00, 0x39, 0x08,
										 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x05, 0xb4,
										 0x04, 0x02, 0x08, 0x0a, 0xe2, 0x08, 0x2f, 0x90,
										 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07,
										 0x61,0x6e,0x69,0x6c};  /* Payload */



#else

								 UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
										 0x60,0x20,0x00,0x00,0x00,0x00,0x06,0xff, /* IPv6 Header */
										 0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
										 0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x14,
										 0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
										 0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x13,
										 0x14, 0xe9, 0x14, 0xea, 0x03, 0x46, 0xd0, 0x0a, /* TCP Header */
										 0x10, 0x02, 0x30, 0x04, 0x50, 0x00, 0x39, 0x08,
										 0x00, 0x00, 0x00, 0x00, /*0x02, 0x04, 0x05, 0xb4,
										 0x04, 0x02, 0x08, 0x0a, 0xe2, 0x08, 0x2f, 0x90,
										 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07,*/
										 0x61,0x6e,0x69,0x6c};  /* Payload */


#endif
#elif ROHC_PROFILE_1
#ifndef UE_IPV6_ADDR
                                UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
                                    0x45,0x00,0x00,0x00,0x23,0x21,0x40,0x00, /* IP header */
                                    0xff,0x11,0x00,0x00,0xe0,0x00,0x00,0xfb,
                                    0xac,0x10,0x51,0x15,
                                    0x14,0xe9,0x14,0xea,0x00,0x00,0x00,0x00,/* UDP header */
                                    0x80,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x5d,0x7c,0x95,0x7c,/* RTP Header */
                                    0x61,0x6e,0x69,0x6c};
#else
                                UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
                                    0x60,0x20,0x00,0x00,0x00,0x00,0x11,0xff, /* IPv6 Header */
                                    0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x14,
                                    0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x13,
                                    0x14,0xe9,0x14,0xea,0x00,0x00,0x00,0x00, /* UDP Header */
                                    0x80,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x5d,0x7c,0x95,0x7c, /* RTP Header */
                                    0x61,0x6e,0x69,0x6c}; /* Payload */
#endif

#else
#ifndef UE_IPV6_ADDR
                                UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
                                    0x45,0x00,0x00,0x00,0x23,0x21,0x40,0x00, /* IP header */
                                    0xff,0x11,0x00,0x00,0xe0,0x00,0x00,0xfb,
                                    0xac,0x10,0x51,0x15,
                                    0x14,0xe9,0x14,0xea,0x00,0x00,0x00,0x00,/* UDP header */
                                    0x61,0x6e,0x69,0x6c};
#else
                                UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
                                    0x60,0x20,0x00,0x00,0x00,0x00,0x11,0xff, /* IPv6 Header */
                                    0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x14,
                                    0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x13,
                                    0x14,0xe9,0x14,0xea,0x00,0x00,0x00,0x00, /* UDP Header */
                                    0x61,0x6e,0x69,0x6c}; /* Payload */

#endif
#endif
                                /*SPS Buffer changes start*/
                                UInt8 spsBuffer[1024];
                                memcpy(spsBuffer,sendBuffer3,60);
                                gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[tp_counter].SendBuffer = spsBuffer;                                
                                PREPARE_SENDBUFFER(spsBuffer, headerStart,
                                        gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[spsIndex].pktSize);                                
                                /*SPS Buffer changes stop*/

                                for(repetition=0; repetition < gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[tp_counter].repetition ; repetition++)
                                {
                                    spsTraffic[offset].ueInfo[loopvar].lcInfo[lc_counter].data = 1;
                                    offset = offset + interPktArvlTm;
                                    if (offset >= 1000)
                                    {
                                        break;
                                    }
                                    spsTraffic[offset].data_present = 1;
                                    spsTraffic[offset].num_ue = loopvar +1;
                                    spsTraffic[offset].ueInfo[loopvar].ue_id = loopvar;
                                    spsTraffic[offset].ueInfo[loopvar].num_lc = lc_counter + 1;
                                    spsTraffic[offset].ueInfo[loopvar].lcInfo[lc_counter].lc_id = lc_counter+3;
                                    spsTraffic[offset].ueInfo[loopvar].lcInfo[lc_counter].packet_size =
                                        gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[tp_counter].pktSize;
                                    spsTraffic[offset].ueInfo[loopvar].lcInfo[lc_counter].SendBuffer =
                                        gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[tp_counter].SendBuffer ;
                                    printf("\nrepetition = %d spsTraffic[%d].sectorInfo[%d].ueInfo[%d].lcInfo[%d].packet_size=%d\n",repetition,offset,s_count,loopvar,lc_counter,spsTraffic[offset].ueInfo[loopvar].lcInfo[lc_counter].packet_size);
                                    interPktArvlTm = gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[tp_counter].interPktArvlTm;
                                    if(interPktArvlTm == 0)
                                    {
                                        spsTraffic[offset].ueInfo[loopvar].lcInfo[lc_counter].numPkt+=1;
                                    }
                                }
                                interPktArvlTm = gtpDb_g[loopvar].rbInfo[lc_counter].spsProfile[tp_counter].interPktArvlTm;
                                spsIndex++;
                                /* just strored the value as it being  used in 64 UE SUPPORT flag*/
                                gtpDb_g[loopvar].rbInfo[lc_counter].noOfTicks = 1000/timeInMs_g;
                            }
                            else
                            {
                                gtpDb_g[loopvar].rbInfo[lc_counter].noOfTicks = 1000/timeInMs_g;
                                gtpDb_g[loopvar].rbInfo[lc_counter].lastDisplayTick = 0;
                                gtpDb_g[loopvar].rbInfo[lc_counter].upDownFlag = 0;
                                gtpDb_g[loopvar].rbInfo[lc_counter].txTickCount = 0;
                                gtpDb_g[loopvar].rbInfo[lc_counter].extraBurstFlag = 1;
                                if (gtpDb_g[loopvar].rbInfo[lc_counter].bitRateFlag == 1)
                                {

                                    for(i = 0;i < count; i++)
                                    {
                                        gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].varPacketSize[i] = 
                                            roundf((gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].packetSize[i])*
                                                    (load_variation_g/100));
                                    }
                                }	
                                fprintf(stderr,"For LC-ID [%d]\nTP-ID is [%d]\nPKT-SIZE -- [%d] "
                                        "THROUGHTPUT -- [%d] TIMEINMS -- [%d]\n",
                                        gtpDb_g[loopvar].rbInfo[lc_counter].lcId, 
                                        gtpDb_g[loopvar].rbInfo[lc_counter].tpId[tpCounter], 
                                        gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].packetSize[i], 
                                        gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].throughput[i], 
                                        timeInMs_g);

                                if( noOfPktVar_g == 0 )
                                {
                                    for(i = 0;i < count;i++)
                                    {
                                    /* SPR 20441 Changes Start*/
                                        gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].totalNoOfPktVar[i] = 
                                            roundf((gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].throughput[i]*1000 *1000)/
                                                    (gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].packetSize[i]*8));
                                    /* SPR 20441 Changes End*/
                                        fprintf(stderr," total no of packets for UE-ID=[%d] LC-ID=[%d] is %f \n",
                                                gtpDb_g[loopvar].ueId, 
                                                gtpDb_g[loopvar].rbInfo[lc_counter].lcId, 
                                                gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].totalNoOfPktVar[i]);

                                        gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].fixedPktPerTick[i] = 
                                            gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].totalNoOfPktVar[i]/
                                            gtpDb_g[loopvar].rbInfo[lc_counter].noOfTicks;
                                        fprintf(stderr,"fixed pkt per tick for UE-ID=[%d] LC-ID=[%d] is %f \n",
                                                gtpDb_g[loopvar].ueId, 
                                                gtpDb_g[loopvar].rbInfo[lc_counter].lcId, 
                                                gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].fixedPktPerTick[i]);

                                        gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].remainingPkt[i] = 
                                            (UInt32)gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].totalNoOfPktVar[i]%
                                            gtpDb_g[loopvar].rbInfo[lc_counter].noOfTicks;
                                        fprintf(stderr,"remaining pkt per tick for UE-ID=[%d] LC-ID=[%d] is %d \n",
                                                gtpDb_g[loopvar].ueId, 
                                                gtpDb_g[loopvar].rbInfo[lc_counter].lcId, 
                                                gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].remainingPkt[i]);
                                    }
                                    fprintf(stderr,"Scheduled packet per tick = %f\n",(gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].fixedPktPerTick[0]+gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].fixedPktPerTick[1]));
                                }
#ifdef ROHC_PROFILE_6 // profile TCP
#ifndef UE_IPV6_ADDR
								/*  This buffer is  used as a hardcoded value which contains IP/TCP header
								 *  for UE (containing UE IP address). The same packet is sent for multiple UE
								 *  (w/o change in UE IP Address)*/

								UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
										0x45,0x02,0x00,0x00,0x23,0x21,0x40,0x00, /* IP header */
										0xff,0x06,0x00,0x00,0xe0,0x00,0x00,0xfb,
										0xac,0x10,0x51,0x15,
										0x14, 0xe9, 0x14, 0xea, 0x03, 0x46, 0xd0, 0x0a, /* TCP Header */
										0x10, 0x02, 0x30, 0x04, 0xa0, 0x00, 0x39, 0x08,
										0x00, 0x00, 0x00, 0x00,0x02, 0x04, 0x05, 0xb4,
										0x04, 0x02, 0x08, 0x0a, 0xe2, 0x08, 0x2f, 0x90,
										0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07,
										0x61,0x6e,0x69,0x6c};  /* Payload */

#else

								UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
										0x60,0x20,0x00,0x00,0x00,0x00,0x06,0xff, /* IPv6 Header */
										0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
										0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x14,
										0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
										0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x13,
										0x14, 0xe9, 0x14, 0xea, 0x03, 0x46, 0xd0, 0x0a, /* TCP Header */
										0x10, 0x02, 0x30, 0x04, 0xa0, 0x00, 0x39, 0x08,
										0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x05, 0xb4,
										0x04, 0x02, 0x08, 0x0a, 0xe2, 0x08, 0x2f, 0x90,
										0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07,
										0x61,0x6e,0x69,0x6c};  /* Payload */

#endif
#elif ROHC_PROFILE_1
                                 /*  This buffer is  used as a hardcoded value which contains IP/UDP/RTP header
                                 *  for UE (containing UE IP address). The same packet is sent for multiple UE
                                 *  (w/o change in UE IP Address)*/
#ifndef UE_IPV6_ADDR
                                UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
                                    0x45,0x02,0x00,0x00,0x23,0x21,0x40,0x00, /* IP header */
                                    0xff,0x11,0x00,0x00,0xe0,0x00,0x00,0xfb,
                                    0xac,0x10,0x51,0x15,
                                    0x14,0xe9,0x14,0xea,0x00,0x00,0x00,0x00,/* UDP header */
                                    0x80,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x5d,0x7c,0x95,0x7c,/* RTP Header */
                                    0x61,0x6e,0x69,0x6c};  /* Payload */
#else
                                UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
                                    0x60,0x20,0x00,0x00,0x00,0x00,0x11,0xff, /* IPv6 Header */
                                    0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x14,
                                    0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x13,
                                    0x14,0xe9,0x14,0xea,0x00,0x00,0x00,0x00, /* UDP Header */
                                    0x80,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x5d,0x7c,0x95,0x7c, /* RTP Header */
                                    0x61,0x6e,0x69,0x6c}; /* Payload */
#endif

#else                                
                                /*  This buffer is  used as a hardcoded value which contains IP/UDP header
                                 *  for UE (containing UE IP address). The same packet is sent for multiple UE
                                 *  (w/o change in UE IP Address)*/
#ifndef UE_IPV6_ADDR
                                UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
                                    0x45,0x02,0x00,0x00,0x23,0x21,0x40,0x00, /* IP header */
                                    0xff,0x11,0x00,0x00,0xe0,0x00,0x00,0xfb,
                                    0xac,0x10,0x51,0x15,
                                    0x14,0xe9,0x14,0xea,0x00,0x00,0x00,0x00,/* UDP header */
                                    0x61,0x6e,0x69,0x6c};  /* Payload */
#else
                                UInt8 sendBuffer3[MAX_PAYLOAD_SIZE] = {0x00,0x00,0x03,0x05,0x70,
                                    0x60,0x20,0x00,0x00,0x00,0x00,0x11,0xff, /* IPv6 Header */
                                    0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x14,
                                    0x07,0xD1,0x07,0xDA,0x00,0x00,0x00,0x00,
                                    0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x13,
                                    0x14,0xe9,0x14,0xea,0x00,0x00,0x00,0x00, /* UDP Header */
                                    0x61,0x6e,0x69,0x6c}; /* Payload */
#endif
#endif
                                for( packetType =0; packetType < gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].mixProfileCount; packetType++)
                                {
                                    gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].SendBuffer[packetType] = sendBuffer3;

                                    if(gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].packetSize[packetType] !=70)
                                    { 
                                        PREPARE_SENDBUFFER(gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].SendBuffer[packetType], headerStart,
                                                gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].packetSize[packetType]);
                                    }
                                    else
                                        if(gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].packetSize[packetType] == 70)
                                        {
#ifdef ROHC_PROFILE_1
                                            PREPARE_SENDBUFFER(gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].SendBuffer[packetType], headerStart,
                                                    gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].packetSize[packetType]);
#else
                                            memset(gtpDb_g[loopvar].rbInfo[lc_counter].tgProfileInfo[tpCounter].SendBuffer[packetType],0,MAX_PAYLOAD_SIZE);
#endif   

                                        }
                                }
                                break ; 
                            }
                        }
                    }

                }
            }		
            spsIndex = 0;
            offset = 0;
        }
    }
#ifdef ROHC_PROFILE_6
initProfile6Params();
#endif
    /*SPR 20868 +-*/
	fprintf(stderr,"\n UL TG Started %d to %d UE \n", ueStart_g,ueId_g);
  	fprintf(stderr,"\n !!!!!!!!!!!!!!!!PLEASE IGNORE THE FIRST DISPLAY AT THE CONSOLE!!!!!!!!!!!!!!!!!!!");
	initDownlinkTrafficGen(argv[2],argv[3],atoi(argv[4])); 
	return 0;
}
