/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTrafficGenRrcInterface.c,v 1.3 2010/03/11 13:41:11 gur19479 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains functions to add, delete and modify 
 *                     user entity data as provided by RRC
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteTrafficGenRrcInterface.c,v $
 * Revision 1.3  2010/03/11 13:41:11  gur19479
 * printf of throughput inserted.
 *
 * Revision 1.2  2010/02/18 07:29:53  gur19479
 * updated for endian-ness
 *
 * Revision 1.1.2.12.2.2  2009/11/26 11:40:17  gur20439
 * compilation bug fix with UT_TESTING flag on.
 *
 * Revision 1.1.2.12.2.1  2009/09/24 21:27:42  gur19479
 * updated for audio and video handling
 *
 * Revision 1.1.2.10  2009/08/12 08:23:55  gur19836
 * Graceful shutdown of traffic generator code added
 *
 * Revision 1.1.2.9  2009/08/10 11:02:11  gur19140
 * mimo changes in tg and memory leak fixes
 *
 * Revision 1.1.2.8  2009/08/04 13:33:55  gur19836
 * GUI Requirement for cleanup done
 *
 * Revision 1.1.2.7  2009/07/30 08:28:56  gur19140
 * performance changes
 *
 * Revision 1.1.2.6  2009/07/28 15:36:58  gur19836
 * Downlink Issues Resolved, Latency Measurement code added
 *
 * Revision 1.1.2.5  2009/07/20 10:30:33  gur11318
 * Updated for buffer_p to UChar8 type.
 *
 * Revision 1.1.2.4  2009/07/15 14:10:59  gur19836
 * Bug Fixing
 *
 * Revision 1.1.2.3  2009/07/14 14:08:28  gur19836
 * CR Comments Disposed
 *
 * Revision 1.1.2.2  2009/07/13 07:54:57  gur19479
 * updated for traffic gen
 *
 * Revision 1.1.2.1  2009/07/12 09:33:36  gur19479
 * added files to make traffic gen
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMisc.h"
#include "lteTrafficGenRrcInterface.h"
#include "lteRrcInterface.h"
#include "lteTrafficGenEntities.h"
#include "lteLayer2CommanTypes.h"
/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/
SInt32 ueCheckTable_g[MAX_NUM_OF_USERS][MAX_NUM_LOGICAL_CHANNELS][RB_DIR_SUPPORTED];
UeData *indexData_g[1000] = {0};
UInt64 ueIndexToIdMapTable_g[MAX_UE] = {0};
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
UInt8 setSocketBlocking(UInt8 sockfd, UInt8 blocking);

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt8 startTrafficGenProcessing_g;
extern UInt8 startNonPeriodicTraffic_g; 
extern UInt8 packetCount_g ;
extern SInt8 TrafficGenOamRrcSocket_g;
extern SInt8 TrafficGenAdapterSocket_g;
extern UInt32 diffInMiliSec_g;
extern struct sockaddr_in TrafficGenRrcSockaddrTo_g;
extern struct sockaddr_in TrafficGenOamSockaddrTo_g;
extern struct sockaddr_in TrafficGenRrcSockaddrFrom_g;
extern struct sockaddr_in TrafficGenAdapterSockaddrFrom_g; 
void cleanupTrafficGen(void);
void receiveMsgFromSocket();

/* Traffic Generator Thread */
extern ULong32 trafficGenVideoThread_g;
extern ULong32 trafficGenThread_g; 
extern ULong32 timerTrafficGenThread_g; 
extern SInt8 trafficGTxSocket_g;
extern SInt8 vlcTxSocketFd_g;
extern UInt8 dataProfilesFlag_g ;
extern UInt8 videoProfilesFlag_g ;
extern trafficGenAudioVideoData trafficGenAudioVideo_g[MAX_UE][MAX_LC];
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
extern UInt32 createSocketForAudioVideo(trafficGenAudioVideoData * configInfo_p,UInt8 dir);
extern void cleanUpTrafficGen();
/****************************************************************************
 * Function Definitions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : diffInMilliSecPhy
 * Inputs         : None
 * Outputs        : None
 * Returns        : diffInMicroseconds 
 * Description    : This function calculate the differene in Miliseconds
 ****************************************************************************/

static UInt32 diffInMilliSecPhy(struct timespec *  start, struct timespec *  end)
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
 * Function Name  : trafficGenInitDbReq
 * Inputs         : None
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This function initializes traffic generator database.
 ****************************************************************************/
UInt32 trafficGenInitDbReq(void)
{
    LTE_LOG(LOG_DETAIL,PNULL,"GRANULARITY for Traffic generator set to 1 ms\n");
    LTE_LOG(LOG_DETAIL,PNULL,"inside trafficGenInitDbReq\n");

    createTrafficGenEntity();
    return TRAFFIC_GEN_SUCCESS;
}

/****************************************************************************
 * Function Name  : trafficGenRxData
 * Inputs         : None
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This function receives data from Adapter and OAM RRC.
 ****************************************************************************/
void trafficGenRxData()
{
    SInt32 bytesRead = TRAFFIC_GEN_INVALID_ID;
    UInt8 recvBuffer[BUFFERSIZE] = {0};
    static UInt64 count = 0;
    static struct timespec lasttime;
    static struct timespec currtime;
    static unsigned long  datavolume = 0;
    socklen_t addrLen = 0;
    UInt32 ueIndex = 0 , counter = 0, ret = 0, diff = 0;
    UInt8 lcId  = 0;
    UInt64 ueId = 0;
    trafficGenAudioVideoData * trafficGenVideoStruct_p = PNULL;
    addrLen = sizeof(TrafficGenAdapterSockaddrFrom_g);
    bzero(recvBuffer,sizeof(recvBuffer));
    fd_set readFD;
    FD_ZERO(&readFD);
    FD_SET(TrafficGenOamRrcSocket_g,&readFD);
    FD_SET(TrafficGenAdapterSocket_g,&readFD);
    while(1)
    {
        if (select(TrafficGenOamRrcSocket_g+1 ,&readFD,PNULL,PNULL,PNULL) == -1)
            lteWarning("Server-select error: \n ");
        else
        {
           if (FD_ISSET(TrafficGenOamRrcSocket_g,&readFD))
                receiveMsgFromSocket();
           if (FD_ISSET(TrafficGenAdapterSocket_g,&readFD)) 
           { 
               bytesRead = recvfrom(TrafficGenAdapterSocket_g, recvBuffer, BUFFERSIZE, 0,
                   (struct sockaddr *)&TrafficGenAdapterSockaddrFrom_g, &addrLen);
               if (bytesRead > 0) 
               {
                 count++;
                 clock_gettime(CLOCK_REALTIME,&currtime);
                 datavolume += bytesRead;
                 if ((diff = diffInMilliSecPhy(&lasttime,&currtime)) > diffInMiliSec_g )
		         {
                      fprintf(stderr, "No of packets received from Adapter is %lld\n",count);
                      fprintf(stderr,"DL Throughput=%u\n",(datavolume*8*1000)/diff);
                      datavolume =0;
                      clock_gettime(CLOCK_REALTIME,&lasttime);
		         }
                 if (videoProfilesFlag_g && (vlcTxSocketFd_g != -1)) 
                 {
                    counter = 0;
                    ueIndex = LTE_GET_U16BIT(&recvBuffer[counter]);
                    counter +=2; 
                    lcId = recvBuffer[counter++];
                    ueId = ueIndexToIdMapTable_g[ueIndex]; 
                    trafficGenVideoStruct_p = &trafficGenAudioVideo_g[ueId][lcId];
                    ret = sendto(vlcTxSocketFd_g,recvBuffer+counter+2,bytesRead - counter - 2, \
                       0,(struct sockaddr*)&(trafficGenVideoStruct_p->videoSockaddrTo),sizeof(trafficGenVideoStruct_p->videoSockaddrTo));
                    if (ret < 0)
                      LTE_LOG(LOG_INFO,PNULL,"Not Able to send the packets to destination machine");
                   }
                }
            }
         }
     FD_ZERO(&readFD);
     FD_SET(TrafficGenOamRrcSocket_g,&readFD);
     FD_SET(TrafficGenAdapterSocket_g,&readFD);
     }
}
/****************************************************************************
 * Function Name  : ueDelReq
 * Inputs         : ueDeleteReq_p
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion handles UE Delete Request
 ****************************************************************************/
UInt32 ueDelReq(ueDeleteReq *ueDeleteReq_p)
{
    UInt64 ueId = 0;
    LTE_LOG(LOG_INFO,PNULL,"Delete UE Entry.\n");
    UInt16 ueIndex = 0, lcId = 0, rbDirection = 0,startValue = 0;
    ueIndex = ueDeleteReq_p->ueIndex;
    if(dataProfilesFlag_g) {
    /*Finding the data corresponding the Ue Index */
    UeData *ueToDelete = indexData_g[ueIndex];
    if ( ueToDelete == NULL )
    {
        lteWarning("\n UE not found on IndexTable - DeleteRlcUe() ");
        return TRAFFIC_GEN_FAILURE;
    }
    freeMemPool(ueToDelete);
    indexData_g[ueIndex] = PNULL;
    SET_UE_INDEX_LC_MAPPING_INVALID(ueIndex,lcId,rbDirection);
    }
    if(videoProfilesFlag_g)
    {
        for (; startValue < MAX_LC; startValue++)
        {
           ueId = ueIndexToIdMapTable_g[ueDeleteReq_p->ueIndex];
           if(TRAFFIC_GEN_SUCCESS == ((trafficGenAudioVideo_g[ueId][startValue].isConfigured & TRAFFIC_GEN_UE_CONFIGURED)>>1))
           {
             if (trafficGenAudioVideo_g[ueId][startValue].vlcSocketFd > 0)
                 close(trafficGenAudioVideo_g[ueId][startValue].vlcSocketFd);
             trafficGenAudioVideo_g[ueId][startValue].vlcSocketFd = 0;
             trafficGenAudioVideo_g[ueId][startValue].isConfigured &= TRAFFIC_GEN_UE_UNCONFIGURED; 
	   }
        }
    }
    LTE_LOG( LOG_INFO,PNULL,"ueDelReq Exit.\n");
    return TRAFFIC_GEN_SUCCESS;
}

/****************************************************************************
 * Function Name  : ueReconfigureReq 
 * Inputs         : ueReconfigReq_p
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion handles UE Reconfig Request.
 ****************************************************************************/
UInt32 ueReconfigureReq(ueReconfigReq *ueReconfigReq_p) 
{
    LTE_LOG( LOG_INFO,PNULL,"ReConfig UE Entry.\n");
    UInt16      ueIndex    = 0;
    UInt64      ueId       = 0;
    UInt8       countLC    = 0;
    UInt8       lcCount    = 0;
    UInt8       checkFlag  = 1;
    UeData      *ueToReconfig_p = PNULL;
    ueIndex = ueReconfigReq_p->ueIndex;
    if (dataProfilesFlag_g) {
    /*Finding the data corresponding the Ue Index */
    ueToReconfig_p = indexData_g[ueIndex];
    if (ueToReconfig_p == PNULL )
    {
        lteWarning("UE not found on CheckTable - ueReconfigReq ");
        return TRAFFIC_GEN_FAILURE;
    }
    }
    for(countLC = 0;countLC < ueReconfigReq_p->ueCreateCount;countLC++)
    {
        for(lcCount = 0; lcCount <ueToReconfig_p->lcCount; lcCount++)
        {
            if(((ueToReconfig_p->UeInfoData[lcCount].lcId == ueReconfigReq_p->ueAddReq[countLC].lcId)&& dataProfilesFlag_g ))
            {
                if (ueToReconfig_p->UeInfoData[lcCount].rbDirection
                        != ueReconfigReq_p->ueAddReq[countLC].rbDirection)
                {
                    ueToReconfig_p->UeInfoData[lcCount].rbDirection
                        = ueReconfigReq_p->ueAddReq[countLC].rbDirection;
                    if (TRAFFIC_GEN_RX_TX_DIR == ueReconfigReq_p->ueAddReq[countLC].rbDirection) 
                    {
                        ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueAddReq[countLC].lcId][ueReconfigReq_p->ueAddReq[countLC].rbDirection -2]
                            = ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueAddReq[countLC].lcId][ueReconfigReq_p->ueAddReq[countLC].rbDirection -1] 
                            = ueIndex;
                    }   
                    else if (TRAFFIC_GEN_TX_DIR == ueReconfigReq_p->ueAddReq[countLC].rbDirection)
                    {
                        ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueAddReq[countLC].lcId][ueReconfigReq_p->ueAddReq[countLC].rbDirection]= ueIndex;
                        ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueAddReq[countLC].lcId][ueReconfigReq_p->ueAddReq[countLC].rbDirection + TRAFFIC_GEN_RX_DIR]= TRAFFIC_GEN_INVALID_ID;
                    }
                    else 
                    {
                        ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueAddReq[countLC].lcId][ueReconfigReq_p->ueAddReq[countLC].rbDirection]= ueIndex;
                        ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueAddReq[countLC].lcId][ueReconfigReq_p->ueAddReq[countLC].rbDirection - TRAFFIC_GEN_RX_DIR]= TRAFFIC_GEN_INVALID_ID;
                    }
                    checkFlag = 0;
                    break;
                }
            }
            if (videoProfilesFlag_g)
            {
                ueId = ueIndexToIdMapTable_g[ueIndex];
                if ((TRAFFIC_GEN_SUCCESS == trafficGenAudioVideo_g[ueId][ueReconfigReq_p->ueAddReq[countLC].lcId].isConfigured) & TRAFFIC_GEN_UE_ADDED)
                {
                   if (!((trafficGenAudioVideo_g[ueId][ueReconfigReq_p->ueAddReq[countLC].lcId].isConfigured) & TRAFFIC_GEN_UE_CONFIGURED)) 
                    createSocketForAudioVideo(&trafficGenAudioVideo_g[ueId][ueReconfigReq_p->ueAddReq[countLC].lcId],ueReconfigReq_p->ueAddReq[countLC].rbDirection);
                    trafficGenAudioVideo_g[ueId][ueReconfigReq_p->ueAddReq[countLC].lcId].isConfigured |= TRAFFIC_GEN_UE_CONFIGURED;
                }
            }
        }

        if (checkFlag && dataProfilesFlag_g)
        { 
            ueToReconfig_p->UeInfoData[ueToReconfig_p->lcCount].lcId
                = ueReconfigReq_p->ueAddReq[countLC].lcId;
            ueToReconfig_p->UeInfoData[ueToReconfig_p->lcCount++].rbDirection
                = ueReconfigReq_p->ueAddReq[countLC].rbDirection;
            if (TRAFFIC_GEN_RX_TX_DIR == ueReconfigReq_p->ueAddReq[countLC].rbDirection)
                ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueAddReq[countLC].lcId][ueReconfigReq_p->ueAddReq[countLC].rbDirection -2]= ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueAddReq[countLC].lcId][ueReconfigReq_p->ueAddReq[countLC].rbDirection -1] = ueIndex;
            else
                ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueAddReq[countLC].lcId][ueReconfigReq_p->ueAddReq[countLC].rbDirection]= ueIndex;
        }
        checkFlag = 1;
    } 

    for(countLC = 0;countLC < ueReconfigReq_p->ueDeleteCount;countLC++)
    {
        for (lcCount = 0; lcCount < ueToReconfig_p->lcCount; lcCount++)
        {
            if (dataProfilesFlag_g)
            {
                if(ueReconfigReq_p->ueDelReq[countLC].lcId == 
                    ueToReconfig_p->UeInfoData[lcCount].lcId)
                {
                    if (TRAFFIC_GEN_RX_TX_DIR == ueToReconfig_p->UeInfoData[lcCount].rbDirection)
                        ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueDelReq[countLC].lcId][ueToReconfig_p->UeInfoData[lcCount].rbDirection -2 ] = ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueDelReq[countLC].lcId][ueToReconfig_p->UeInfoData[lcCount].rbDirection -1 ] = TRAFFIC_GEN_INVALID_ID;
                    else 
                        ueCheckTable_g[ueToReconfig_p->UeId][ueReconfigReq_p->ueDelReq[countLC].lcId][ueToReconfig_p->UeInfoData[lcCount].rbDirection] = TRAFFIC_GEN_INVALID_ID;
                   ueToReconfig_p->UeInfoData[lcCount].lcId = TRAFFIC_GEN_INVALID_ID;
                   ueToReconfig_p->UeInfoData[lcCount].rbDirection = TRAFFIC_GEN_INVALID_ID;
                   ueToReconfig_p->lcCount -= 1;
                }
            }
            if (videoProfilesFlag_g)
            {
                 ueId = ueIndexToIdMapTable_g[ueIndex];
                 if((TRAFFIC_GEN_SUCCESS == ((trafficGenAudioVideo_g[ueId][ueReconfigReq_p->ueDelReq[countLC].lcId].isConfigured)& TRAFFIC_GEN_UE_CONFIGURED)>>1))
                 {
                     if (trafficGenAudioVideo_g[ueId][ueReconfigReq_p->ueDelReq[countLC].lcId].vlcSocketFd > 0)
                         close(trafficGenAudioVideo_g[ueId][ueReconfigReq_p->ueDelReq[countLC].lcId].vlcSocketFd);
                       trafficGenAudioVideo_g[ueId][ueReconfigReq_p->ueDelReq[countLC].lcId].vlcSocketFd = 0;
                       trafficGenAudioVideo_g[ueId][ueReconfigReq_p->ueDelReq[countLC].lcId].isConfigured &= TRAFFIC_GEN_UE_UNCONFIGURED;
                    }
            }
        }
    }
    return TRAFFIC_GEN_SUCCESS;
}

/****************************************************************************
 * Function Name  : ueConfigureReq
 * Inputs         : ueConfigReq_p
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion handles UE Config Request. 
 ****************************************************************************/
UInt32 ueConfigureReq(ueConfigReq *ueConfigReq_p)
{
    LTE_LOG( LOG_DETAIL,PNULL,"RRC => TRAFFIC GEN CONFIG REQ\n");
    UInt8       countLC    = 0;
    UeData   *ueInfoPtr = PNULL;
    UInt32 failureCounter = 0;
    if(dataProfilesFlag_g){
    if (indexData_g[ueConfigReq_p->ueIndex]!= PNULL)
    {
        LTE_LOG(LOG_DETAIL,PNULL,"%s UE Already Exist",__func__);
        return TRAFFIC_GEN_FAILURE;
    }
    ueInfoPtr = (UeData *) getMemFromPool( sizeof(UeData), PNULL );
    if (ueInfoPtr == NULL)
    {
        ltePanic(" Could not get memory from pool. ");
        return TRAFFIC_GEN_FAILURE;
    }
    ueInfoPtr->UeId = ueConfigReq_p->ueId;
    ueInfoPtr->UeIndex = ueConfigReq_p->ueIndex;
    ueInfoPtr->lcCount = ueConfigReq_p->lcCount;
    SET_LC_ID_RB_DIR_INVAILD_FOR_UE_DATA(ueInfoPtr,countLC);
    }
    for (countLC = 0; countLC < ueConfigReq_p->lcCount; countLC++ )
    {
        if (dataProfilesFlag_g) {
        ueInfoPtr->UeInfoData[countLC].lcId = ueConfigReq_p->lcInfo[countLC].lcId ;
        ueInfoPtr->UeInfoData[countLC].rbDirection = ueConfigReq_p->lcInfo[countLC].rbDirection;
        if (TRAFFIC_GEN_RX_TX_DIR == ueConfigReq_p->lcInfo[countLC].rbDirection)
        {
            ueCheckTable_g[ueConfigReq_p->ueId][ueConfigReq_p->lcInfo[countLC].lcId][ueConfigReq_p->lcInfo[countLC].rbDirection - 2]
                = ueCheckTable_g[ueConfigReq_p->ueId][ueConfigReq_p->lcInfo[countLC].lcId][ueConfigReq_p->lcInfo[countLC].rbDirection - 1] 
                = ueConfigReq_p->ueIndex;
        }      
        else
            ueCheckTable_g[ueConfigReq_p->ueId][ueConfigReq_p->lcInfo[countLC].lcId][ueConfigReq_p->lcInfo[countLC].rbDirection] = ueConfigReq_p->ueIndex;
        }
        if (videoProfilesFlag_g && (TRAFFIC_GEN_SUCCESS == (trafficGenAudioVideo_g[ueConfigReq_p->ueId][ueConfigReq_p->lcInfo[countLC].lcId].isConfigured & TRAFFIC_GEN_UE_ADDED)))
        {
           ueIndexToIdMapTable_g[ueConfigReq_p->ueIndex] = ueConfigReq_p->ueId; 
           trafficGenAudioVideo_g[ueConfigReq_p->ueId][ueConfigReq_p->lcInfo[countLC].lcId].ueIndex
               = ueConfigReq_p->ueIndex;
           trafficGenAudioVideo_g[ueConfigReq_p->ueId][ueConfigReq_p->lcInfo[countLC].lcId].isConfigured |= TRAFFIC_GEN_UE_CONFIGURED;
           createSocketForAudioVideo(&trafficGenAudioVideo_g[ueConfigReq_p->ueId][ueConfigReq_p->lcInfo[countLC].lcId],ueConfigReq_p->lcInfo[countLC].rbDirection);
        }    
        else 
	    failureCounter++; 
    }
    if (dataProfilesFlag_g)
        indexData_g[ueConfigReq_p->ueIndex]= ueInfoPtr;
    LTE_LOG( LOG_DETAIL,PNULL,"Traffic Gen Rrc ConfigReq Exit.\n");
    if (!failureCounter) 
        return TRAFFIC_GEN_SUCCESS;
    return ((countLC == failureCounter)? TRAFFIC_GEN_FAILURE:TRAFFIC_GEN_PARTIAL_SUCCESS);

}

/****************************************************************************
 * Function Name  : setSocketBlocking
 * Inputs         : socketfiledecriptor, blocking (0 - Non Blocking, 1-Blocking)
 * Outputs        : NONE
 * Returns        : status (0-Failure,1-Successful)
 * Description    : to set the specific socket blocking or non blocking
 ****************************************************************************/
UInt8 setSocketBlocking(UInt8 sockfd, UInt8 blocking)
{
    SInt8 fileflags;
    if((fileflags = fcntl(sockfd, F_GETFL,0)) == -1)
    {
        ltePanic("set Socket bloackin Failed \n ");
        return 0;
    }
    fileflags = blocking ? (fileflags & ~FNDELAY) : (fileflags |FNDELAY);
    return (fcntl(sockfd, F_SETFL, fileflags)== 0) ? 1 : 0 ;
}


/****************************************************************************
 * Function Name  : prepareMsgHeader
 * Inputs         : buffer_p : pointer to buffer for Message from PDCP
 *                : apiId :Api Identifier for Confirmation message
 *                : transactionId:Transaction Id same as of Request message
 *                : DestModule_Id: Module ID of RRC
 * Outputs        : None 
 * Returns        : Void
 * Description    : This function is used to encode API buffer header for 
 *                : confirmation message from PDCP to RRC.
 ****************************************************************************/
static UInt16 prepareMsgHeaderForOamRrc(UChar8 *buffer_p, UInt16 apiId,
        UInt16 transactionId, UInt16 destModuleId )
{
    UInt16 current = 0;
    LTE_LOG( LOG_DETAIL,PNULL,"====================================\n");
    LTE_LOG( LOG_DETAIL,PNULL,"Transaction id is %d\n", transactionId);
    LTE_LOG( LOG_DETAIL,PNULL,"====================================\n");

    LTE_SET_U16BIT(&buffer_p[current],transactionId);
    current +=2;
    LTE_LOG( LOG_DETAIL,PNULL,"TRAFFIC GEN MODULE ID is %d\n",TRAFFIC_GEN_MODULE_ID);
    LTE_SET_U16BIT(&buffer_p[current],TRAFFIC_GEN_MODULE_ID);
    current +=2;

    LTE_SET_U16BIT(&buffer_p[current],destModuleId);
    current +=2;

    LTE_SET_U16BIT(&buffer_p[current],apiId);
    current +=2;
    return current;
}

/****************************************************************************
 * Function Name  : processReconfigUeReq
 * Inputs         : reconfigUeReq_p
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion process Reconfig UE Request.
 ****************************************************************************/
UInt32 processReconfigUeReq(UInt8 *reconfigUeReq_p)
{
    ueReconfigReq reconfigReq = {0};
    UInt16 current =0;
    UInt16 msgLen = 0;
    UInt8 lcCount = 0;
    UInt16 subTag = 0;
    msgLen = LTE_GET_U16BIT(reconfigUeReq_p + 8);
    current = 10;
    reconfigReq.ueIndex = LTE_GET_U16BIT(reconfigUeReq_p + current);
    current +=2;
    while (msgLen != current)
    {
        subTag = LTE_GET_U16BIT(reconfigUeReq_p + current);
        current += 4 ;
        if (RECONFIG_UE_ADD_REQ == subTag)
        {
            reconfigReq.ueCreateCount = *(reconfigUeReq_p + current++);
            for (lcCount = 0;lcCount < reconfigReq.ueCreateCount;lcCount++)
            {
                reconfigReq.ueAddReq[lcCount].lcId = *(reconfigUeReq_p + current++);
                reconfigReq.ueAddReq[lcCount].rbDirection = *(reconfigUeReq_p + current++);
            }
        }
        else if (RECONFIG_UE_DEL_REQ == subTag)
        {
            reconfigReq.ueDeleteCount = *(reconfigUeReq_p + current++);
            for (lcCount = 0 ;lcCount < reconfigReq.ueDeleteCount;lcCount++)
                reconfigReq.ueDelReq[lcCount].lcId = *(reconfigUeReq_p + current++);
        }
        else 
        {
            return TRAFFIC_GEN_FAILURE;
        }
    }
    return(ueReconfigureReq(&reconfigReq));
}

/****************************************************************************
 * Function Name  : processDeleteUeCnf
 * Inputs         : transId, modId, response
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This function send response of Delete UE Request. 
 ****************************************************************************/

UInt32 processDeleteUeCnf(UInt16 transId, UInt16 modId, UInt16 response, UInt16 ueIndex)
{
    UChar8 deleteUeCnf[20] = {0};
    UInt16 current = 0;
    current = prepareMsgHeaderForOamRrc(deleteUeCnf,UE_DELETE_ENTITY_CNF ,transId, modId);
    current +=2;
    LTE_SET_U16BIT(&deleteUeCnf[current],ueIndex);
    current +=2;
    LTE_SET_U16BIT(&deleteUeCnf[current],response);
    return (ueHandleResponseMsgForRrc(deleteUeCnf, current)); 
}

/****************************************************************************
 * Function Name  : processCreateUeCnf
 * Inputs         : transId, modId, response
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This function send response of Create UE Request. 
 ****************************************************************************/
UInt32 processCreateUeCnf(UInt16 transId, UInt16 modId, UInt16 response, UInt16 ueIndex)
{
    UChar8 createUeCnf[20] = {0};
    UInt16 current = 0;
    current = prepareMsgHeaderForOamRrc(createUeCnf,UE_CREATE_ENTITY_CNF, transId, modId);
    current +=2;
    LTE_SET_U16BIT(&createUeCnf[current],ueIndex);
    current +=2;
    LTE_SET_U16BIT(&createUeCnf[current],response);
    current +=2;
    return (ueHandleResponseMsgForRrc(createUeCnf, current)); 
}

/****************************************************************************
 * Function Name  : processReconfigUeCnf
 * Inputs         : transId, modId, response
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion send response of Reconfig UE Request. 
 ****************************************************************************/

UInt32 processReconfigUeCnf(UInt16 transId, UInt16 modId, UInt16 response, UInt16 ueIndex)
{
    UChar8 reconfigUeCnf[20] = {0};
    UInt16 current = 0;
    current = prepareMsgHeaderForOamRrc(reconfigUeCnf,UE_RECONFIG_ENTITY_CNF, transId, modId);
    current +=2;
    LTE_SET_U16BIT(&reconfigUeCnf[current],ueIndex);
    current +=2;
    LTE_SET_U16BIT(&reconfigUeCnf[current],response);
    current +=2;
    return (ueHandleResponseMsgForRrc(reconfigUeCnf, current)); 
}

/****************************************************************************
 * Function Name  : processTrafficGenStartStopCnf
 * Inputs         : transId, modId, response
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion process UE Traffic Start Stop Response. 
 ***************************************************************************/
UInt32 processTrafficGenStartStopCnf(UInt16 transId, UInt16 modId, UInt16 response, UInt16 ueIndex)
{
    UChar8 trafficGenCnf[20] = {0};
    UInt16 current = 0;
    current = prepareMsgHeaderForOamRrc(trafficGenCnf, TRAFFIC_GEN_START_STOP_CNF, transId, modId);
    current +=2;
    LTE_SET_U16BIT(&trafficGenCnf[current],response);
    current +=2;
    return (ueHandleResponseMsgForOam(trafficGenCnf, current)); 

}

/****************************************************************************
 * Function Name  : ueHandleResponseMsgForRrc
 * Inputs         : buffer_p, msgLen
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion for Sending response throurgh Socket. 
 ****************************************************************************/
UInt32 ueHandleResponseMsgForRrc(UChar8 *buffer_p, UInt16 msgLen)
{
    SInt32 temp = 0;
    LTE_SET_U16BIT(&buffer_p[API_HEADER_MSGLEN_OFFSET],msgLen) ;
    temp = sendto(TrafficGenOamRrcSocket_g, (UInt8 *)buffer_p, msgLen, 0,
                (struct sockaddr*)&(TrafficGenRrcSockaddrTo_g),
                 sizeof(TrafficGenRrcSockaddrTo_g));
    if(TRAFFIC_GEN_INVALID_ID == temp)
        return TRAFFIC_GEN_FAILURE;

    return TRAFFIC_GEN_SUCCESS;

}

/****************************************************************************
 * Function Name  : ueHandleResponseMsgForRrc
 * Inputs         : buffer_p, msgLen
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion for Sending response throurgh Socket.
 ****************************************************************************/
UInt32 ueHandleResponseMsgForOam(UChar8 *buffer_p, UInt16 msgLen)
{
    SInt32 temp = 0;
    LTE_SET_U16BIT(&buffer_p[API_HEADER_MSGLEN_OFFSET],msgLen) ;
    temp = sendto(TrafficGenOamRrcSocket_g, (UInt8 *)buffer_p, msgLen, 0,
            (struct sockaddr*)&(TrafficGenOamSockaddrTo_g),
             sizeof(TrafficGenOamSockaddrTo_g));
    if(TRAFFIC_GEN_INVALID_ID == temp)
        return TRAFFIC_GEN_FAILURE;

    return TRAFFIC_GEN_SUCCESS;

}

/****************************************************************************
 * Function Name  : ueTrafficStartStopReq
 * Inputs         : ueTrafficCtrlReq_p
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion process UE Traffic Start Stop Request. 
 ****************************************************************************/
UInt32 ueTrafficStartStopReq(ueTrafficCtrlReq *ueTrafficCtrlReq_p)
{
    if (ueTrafficCtrlReq_p->trafficCtrlBit)
        startTrafficGenProcessing_g = 1;
    else
        startTrafficGenProcessing_g = 0;
    if (ueTrafficCtrlReq_p->periodicCtrlBit)
        startNonPeriodicTraffic_g = 1;
    else  
        startNonPeriodicTraffic_g = 0;   

    if (ueTrafficCtrlReq_p->packetCount)
        packetCount_g = ueTrafficCtrlReq_p->packetCount;	

    return TRAFFIC_GEN_SUCCESS;
}


/****************************************************************************
 * Function Name  : processTrafficGenStartStopReq
 * Inputs         : trafficStartStopReq_p
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion process UE Traffic Start Stop Request. 
 ****************************************************************************/
UInt32 processTrafficGenStartStopReq(UInt8 *trafficStartStopReq_p)
{
    ueTrafficCtrlReq trafficGenReq = {0};
    trafficGenReq.trafficCtrlBit = *(trafficStartStopReq_p + 10);
    trafficGenReq.periodicCtrlBit = *(trafficStartStopReq_p + 11);
    trafficGenReq.packetCount  = *(trafficStartStopReq_p + 12);
    return (ueTrafficStartStopReq(&trafficGenReq)); 
}


/****************************************************************************
 * Function Name  : processTrafficGenCleanupReq
 * Inputs         : trafficGenCleanupReq_p
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion process Traffic Gen Cleanup Request. 
 ****************************************************************************/
UInt32 processTrafficGenCleanupReq(UInt8 *trafficStartStopReq_p)
{
    cleanUpTrafficGen();
    return TRAFFIC_GEN_SUCCESS; 
}



/****************************************************************************
 * Function Name  : processDeleteUeReq
 * Inputs         : deleteUeReq_p
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE
 * Description    : This funtion process Delete UE Request. 
 ****************************************************************************/
UInt32 processDeleteUeReq( UInt8 *deleteUeReq_p)
{
    ueDeleteReq deleteReq = {0};
    UInt16 current = 0;
    current = 10;
    /* getting two bytes UE Index */ 
    deleteReq.ueIndex = LTE_GET_U16BIT(deleteUeReq_p + current);
    current +=2;
    /* calling ueDeleteReq function to process delete req */
    return (ueDelReq(&deleteReq));
}
/****************************************************************************
 * Function Name  : processCreateUeReq
 * Inputs         : createUeReq_p
 * Outputs        : None
 * Returns        : TRAFFIC_GEN_SUCCESS/TRAFFIC_GEN_FAILURE.
 * Description    : This funtion process create UE Request.
 ****************************************************************************/

UInt32 processCreateUeReq(UInt8 *createUeReq_p)
{
    ueConfigReq configReq = {0};
    UInt16 current =10;
    UInt8 lcCount = 0;
    /* getting two bytes of UE ID */
    configReq.ueId = LTE_GET_U16BIT(createUeReq_p + current);
    current +=2;
    /* getting two byte of ue Index */
    configReq.ueIndex = LTE_GET_U16BIT(createUeReq_p + current); 
    current +=2;
    /* check for ueId */
    /* getting one byte lc count */
    configReq.lcCount = *(createUeReq_p + current++);
    /* getting number of lc ids based on lc count */
    for(;lcCount < configReq.lcCount;lcCount++)
    {
        configReq.lcInfo[lcCount].lcId
            = *(createUeReq_p + current++); 
        configReq.lcInfo[lcCount].rbDirection 
            = *(createUeReq_p + current++);
    }
    /* Creating UE Entity*/
    return (ueConfigureReq(&configReq));
}

/****************************************************************************
 * Function Name  : receiveMsgFromSocket
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API recive  Pdcp-Rrc message From Socket
 *                  sends confirmation back to RRC
 ****************************************************************************/
void receiveMsgFromSocket()
{
    SInt32 bytesRead = TRAFFIC_GEN_INVALID_ID;
    UInt8 recvBuffer[BUFFERSIZE] = {0};
    UInt8 *data_p = PNULL ;
    UInt16 current = 0; 
    UInt16 apiID = 0;
    UInt16 transId = 0;
    UInt16 rcvMsgLen = 0;
    UInt32 retVal = 0;
    UInt16 srcModId = 0;
    UInt16 ueIndex = 0;
    socklen_t addrLen = 0;
    addrLen = sizeof(TrafficGenRrcSockaddrFrom_g);
    bzero(recvBuffer,sizeof(recvBuffer));

    bytesRead= recvfrom(TrafficGenOamRrcSocket_g,recvBuffer,BUFFERSIZE,0,
            (struct sockaddr *)&TrafficGenRrcSockaddrFrom_g, &addrLen);

    if (bytesRead > 0)
    {
        //fprintf(stderr,"I got config request from OAM/RRC Sim\n");
        data_p = recvBuffer;
        transId = LTE_GET_U16BIT(data_p + current);
        current += 2; 
        srcModId = LTE_GET_U16BIT(data_p + current);
        current += 4;
        apiID = LTE_GET_U16BIT(data_p + current);
        current += 2;
        rcvMsgLen = LTE_GET_U16BIT(data_p + current);
        current += 2;
        ueIndex = LTE_GET_U16BIT(data_p + current);
        LTE_LOG( LOG_DETAIL,PNULL,"=========================================================\n");
        LTE_LOG( LOG_DETAIL,PNULL,"Transaction Id = %d \n",transId);
        retVal = processIncomingMessage[apiID](recvBuffer);
        sendConfirmationMessage[apiID](transId, srcModId,retVal,ueIndex);
    }
}


/****************************************************************************
 * Function Name  : cleanupTrafficGen
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API cleanup the traffic generator.
 ****************************************************************************/

void cleanupTrafficGen(void)
{
    UInt32 retTh = 0;
    if(timerTrafficGenThread_g > 0)
    {
        retTh = threadCancel(timerTrafficGenThread_g);
        timerTrafficGenThread_g = 0;
    }
    
    if(trafficGenThread_g > 0)
    {
        retTh = threadCancel(trafficGenThread_g);
        trafficGenThread_g = 0;
    }
   
    if (trafficGenVideoThread_g > 0)
    {
        retTh = threadCancel(trafficGenVideoThread_g);
        trafficGenVideoThread_g = 0;
    }
    /* Closing sockets */
    if(TrafficGenOamRrcSocket_g > 0) {
        close(TrafficGenOamRrcSocket_g);
        TrafficGenOamRrcSocket_g = 0;
    }

    if(trafficGTxSocket_g > 0) {
        close(trafficGTxSocket_g);
        trafficGTxSocket_g = 0;
    }

    if(TrafficGenAdapterSocket_g > 0) {
        close(TrafficGenAdapterSocket_g);
        TrafficGenAdapterSocket_g = 0;
    }
    
    /* Cleanup memory pools */
    cleanupMemPool();
    exit(1);
}    

