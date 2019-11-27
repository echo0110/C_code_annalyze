/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTrafficGenExtInterface.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteTrafficGenExtInterface.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.4.2.2  2009/10/09 05:48:49  gur11318
 * Changes for oam and rrc seperate host name added.
 *
 * Revision 1.1.2.4.2.1  2009/09/24 21:45:02  gur19479
 * updated for video and audio processing
 *
 * Revision 1.1.2.4  2009/07/28 15:36:54  gur19836
 * Downlink Issues Resolved, Latency Measurement code added
 *
 * Revision 1.1.2.3  2009/07/15 14:10:53  gur19836
 * Bug Fixing
 *
 * Revision 1.1.2.2  2009/07/14 14:08:23  gur19836
 * CR Comments Disposed
 *
 * Revision 1.1.2.1  2009/07/12 09:37:31  gur19479
 * updated for traffic generator
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_LTE_TRAFFIC_GEN_EXT_INTERFACE_H
#define INCLUDED_LTE_TRAFFIC_GEN_EXT_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include <stdlib.h>
#include <lteTrafficGenDatabase.h>
#include <signal.h>

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define FILENAMESIZE 200
/* Default Ports to be used */
#define DEFAULT_SIM_PORT 2222
#define DEFAULT_WIRESHARK_PORT 2233
#define DEFAULT_PDCPADAPTER_PORT 2244

/* Delay between periodic and non periodic data */
#define DELAY_AFTER_NP_DATA 1 

/* For Pcap file and WireShark */
#define LEN_WS_FILE_HEADER      24
#define LEN_WS_PACKET_HEADER    16
#define LEN_ETHERNET_HEADER     14

#if 0
/* e.g 00001111 for  numBits = 4 */
#define GETMASK_RIGHT_ALIGNED(numBits) ((~(~0U << (numBits))))

/* set given range of bits in Byte to zero*/
#define ZERO_BYTE_FIELD(byte,sBitN,numBits) ((byte) &= ~(GETMASK_RIGHT_ALIGNED(numBits) << (sBitN)))
/* set given range of bits to value */
#define SET_BYTE_FIELD(byte,value,sBitN,numBits) (ZERO_BYTE_FIELD(byte,sBitN,numBits), \
        ((byte) |= ((value) & GETMASK_RIGHT_ALIGNED(numBits)) << (sBitN)) )
#endif

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern timeInterval_t  *timeIntvalBucket_g[MAX_TIME_INTERVAL_BUCKETS+1];

/* Structure for PDCP config file*/
typedef struct TrafficGenConfigFileStructT
{
    UChar8 adapterHostName[20]; 
    UInt16 trafficGeneratorAdapterRxPort; 
    UInt16 wireSharkPort;
    UInt16 adapterPort;		 
    #ifdef OAM_WITH_RRC
    UChar8 oamRrcHostName[20];
    #else
    UChar8 oamHostName[20];
    UChar8 rrcHostName[20];
    #endif
    UInt16 rrcSimPort;
    UInt16 oamSimPort;
    UInt16 trafficGeneratorOamRrcRxPort;
}TrafficGenConfigFileStruct;

enum threadToCreateT
{
TRAFFIC_GEN_VIDEO_NO_THREAD_CREATE,
TRAFFIC_GEN_VIDEO_THREAD_CREATE
}threadToCreate;

/************************************************************************************
 * STRUCT NAME  :   WsPacketHeader
 * DESCRIPTION  :   Structure, which represents header of each packet in wireshark 
 *                  capture file placed before each captured packet in *.pcap file
 * FIELDS       :   struct timeval tv - time of packet`s coming (with microseconds)
 *                  UInt32 len_wire - length of captured bytes
 *                  UInt32 len_capt - length of captured bytes
 ************************************************************************************/
typedef struct WsPacketHeaderT
{
    UInt32 tv_sec;
    UInt32 tv_usec;
    UInt32 len_wire;
    UInt32 len_capt;
} WsPacketHeader;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

UInt32   parsePdcpInfoPerUe(void);
void     *IncrementQueueLoad (void);
void *   trafficGenThread(void *);
void SendUeDataToAdapter(UInt64 Index);
UInt64 update_lc_traffic(void);

UInt32 insertIdx_inList(UInt8 profile, UInt16 initialDelay, UInt32 Idx);
initialDelay_t *getInitialDelayList(UInt64 timeInterval);
initialDelay_t *getInitialDelayList_NP(UInt64 timeInterval);
void insert_IndexNodeInList(UInt32 time_val, UInt16 initDelay, UInt64 index);
UInt16 insert_NodeInlist(initialDelay_t  *initialDelayNode, UInt32 index);
void insert_IndexNodeInList_NP(UInt32 time_val, UInt16 initDelay, UInt64  pdcpIndex);
initialDelay_t *insert_initDelayNodeInTimeIntvalList(UInt32 time_val, UInt16 initDelay);
initialDelay_t *insert_initDelayNodeInTimeIntvalList_NP(UInt32 time_val, UInt16 initDelay);

extern void getDataFromSocket();

#endif /* INCLUDED_LTE_TRAFFIC_GEN_EXT_INTERFACE_H */
