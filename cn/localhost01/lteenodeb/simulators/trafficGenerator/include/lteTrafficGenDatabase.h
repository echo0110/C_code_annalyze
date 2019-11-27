/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTrafficGenDatabase.h,v 1.2 2010/03/10 11:29:03 gur19479 Exp $
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
 * $Log: lteTrafficGenDatabase.h,v $
 * Revision 1.2  2010/03/10 11:29:03  gur19479
 * id address member added in config structure.
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.2.2.1  2009/09/24 21:45:29  gur19479
 * updated for video and audio processing
 *
 * Revision 1.1.2.2  2009/07/14 14:08:22  gur19836
 * CR Comments Disposed
 *
 * Revision 1.1.2.1  2009/07/12 09:37:31  gur19479
 * updated for traffic generator
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_LTE_TRAFFIC_GEN_DATABASE_H
#define INCLUDED_LTE_TRAFFIC_GEN_DATABASE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteTrafficGenDefinitions.h"
#include "lteLog.h"
#include "lteHash.h"
#include "lteThread.h"
#include "lteList.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_NUM_OF_USERS 311
#define MAX_NUM_LOGICAL_CHANNELS 10
#define RB_DIR_SUPPORTED 2

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef struct UeInfoDataT
{
    SInt8      lcId;
    SInt8      rbDirection;
} UeInfoData;

typedef struct UeDataT
{
    UInt32      UeId;  
    UInt32      UeIndex;
    UInt8       lcCount; 
    UeInfoData  UeInfoData[MAX_LC];
} UeData;

typedef struct UeNodeT
{
    LTE_HASH_NODE  anchor;
    UeData         *  data;
}UeNode;

/* Struct for Data Send to Pdcp Adapter */

typedef struct PdcpAdapterSendDataT
{
    SInt32         ueIndex;
    SInt8          LcId;
    UInt32		   bufferLength; 
    UInt8	 	   dataBuffer[2500];
} PdcpAdapterSendData;


typedef struct trafficGenRateT
{
    UInt64        UeId;
    SInt32        ueIndex;
    SInt8         LcId;
    UInt8         periodicity;   /* 0 - peridic: 1 - bursty */
    UInt32        DataRate;      /* Bytes per second */
    UInt64        DataToPump;
    UInt16        times;
    UInt32        time_interval;
    UInt16        init_delay;
    profiles_e    profile;
    UInt16        numberOfTicks;
    UInt16        tickTimer;
    UeNode       *UeNode;
} trafficGenRateData;

typedef struct trafficGenAudioVideoDataT
{
    UInt64       UeId;
    SInt32       ueIndex;
    SInt8        LcId;
    UChar8       txIpAddress[20];
    UChar8       rxIpAddress[20];
    UChar8       adapterHostName[20];
    UInt16       txPortNumber;
    UInt16       rxPortNumber;
    SInt8        vlcSocketFd;
    UInt8	 isConfigured;
    struct sockaddr_in videoSockaddrFrom;
    struct sockaddr_in videoSockaddrTo;
} trafficGenAudioVideoData;

typedef struct ueLcMappingForAudioVideoT
{
   SInt32 ueIndex;
   SInt8 lcId;
}ueLcMappingForAudioVideo;

typedef struct trafficGenRateNonPeriodicT
{
    UInt64        UeId;
    SInt32        ueIndex;
    SInt8         LcId;
    UInt64        DataToPump;
    profiles_e    profile;
} trafficGenRateNonPeriodic;

typedef struct LcModeMappingT
{
    SInt8         LcId;
    UInt16        LcType;
    UInt8         Mode;
    UInt16        priority;

} LcModeMapping;

typedef struct timeInterval
{
    LTE_LIST    initialDelayList;
    UInt16      maxInitialDelay;
    UInt16      initialDelay;
    UInt32      ticks;
}timeInterval_t;

typedef struct initialDelay
{
    LTE_LIST_NODE nodeAnchor;
    LTE_LIST      indexList;
    UInt16        initialDelay;
}initialDelay_t;

typedef struct IndexListNode
{
    /*data type of anchor, stored in every List Node*/
    LTE_LIST_NODE nodeAnchor;
    UInt64        index;
}indexListNode_t;


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/* ID of Thread to generate PDCP load */ 
extern  ULong32 trafficGenThread_g;         
extern  UInt64 MaxPdcpDataRead_g;
extern  UInt64 trafficGenTimer_g;
//extern  trafficGenRateData trafficGenRateForData_g[MAX_PDCP_DATA];

extern struct timespec t1_g,t2_g;
extern UeData *indexData_g[1000];
extern SInt32 ueCheckTable_g[MAX_NUM_OF_USERS][MAX_NUM_LOGICAL_CHANNELS][RB_DIR_SUPPORTED];

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* INCLUDED_LTE_RLC_SIM_DATABASE_H */
