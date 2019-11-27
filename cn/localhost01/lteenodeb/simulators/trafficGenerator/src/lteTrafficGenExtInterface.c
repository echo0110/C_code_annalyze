/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTrafficGenExtInterface.c,v 1.3 2010/03/10 11:28:20 gur19479 Exp $
 *
 ****************************************************************************
 *
 *  File Description:
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteTrafficGenExtInterface.c,v $
 * Revision 1.3  2010/03/10 11:28:20  gur19479
 * ip address to betaken from config file.
 *
 * Revision 1.2  2010/02/18 07:29:22  gur19479
 * updated for UL Traffic
 *
 * Revision 1.1.2.11.2.6  2009/11/09 07:12:53  gur20439
 * UT bug fix during uplink flow.
 *
 * Revision 1.1.2.11.2.5  2009/10/09 05:48:18  gur11318
 * Chages for oam and rrc host name added.
 *
 * Revision 1.1.2.11.2.4  2009/09/24 22:21:21  gur19479
 * renamed the config files of traffic generator
 *
 * Revision 1.1.2.11.2.3  2009/09/24 22:09:16  gur19479
 * updated to remove affinity issue
 *
 * Revision 1.1.2.11.2.2  2009/09/24 21:37:55  gur19479
 * updated for simultaneous support of data, video and audio
 *
 * Revision 1.1.2.11.2.1  2009/09/24 21:30:36  gur19479
 * updated for audio and video processing
 *
 * Revision 1.1.2.9  2009/08/14 15:30:45  gur19140
 * eNBSim 14 aug delivery changes
 *
 * Revision 1.1.2.8  2009/08/10 11:02:11  gur19140
 * mimo changes in tg and memory leak fixes
 *
 * Revision 1.1.2.7  2009/08/04 12:30:36  gur20435
 * Made trafficGenTick_g amd lastTick volatile to resolve problem with O3
 *
 * Revision 1.1.2.6  2009/07/30 08:28:56  gur19140
 * performance changes
 *
 * Revision 1.1.2.5  2009/07/28 15:36:58  gur19836
 * Downlink Issues Resolved, Latency Measurement code added
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
#include "lteTrafficGenExtInterface.h"
#include "lteRrcInterface.h"
#include "lteTrafficGenRrcInterface.h"
#include "lteTrafficGenDatabase.h"
#include "lteMsgPool.h"
#include <assert.h>
#include "lteLayer2CommanTypes.h"

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
  Private Definitions
 ****************************************************************************/
extern UInt8 startTrafficGenProcessing_g;
extern UInt8 startNonPeriodicTraffic_g;
extern SInt32 ueCheckTable_g[MAX_NUM_OF_USERS][MAX_NUM_LOGICAL_CHANNELS][RB_DIR_SUPPORTED];
extern UeData *indexData_g[1000];
extern UInt64 ueIndexToIdMapTable_g[MAX_UE];
extern UInt32 diffInMiliSec_g;
SInt8 TrafficGenAdapterSocket_g = TRAFFIC_GEN_INVALID_ID;
SInt8 trafficGTxSocket_g = TRAFFIC_GEN_INVALID_ID;
SInt8 vlcTxSocketFd_g = TRAFFIC_GEN_INVALID_ID;
SInt8 TrafficGenOamRrcSocket_g = TRAFFIC_GEN_INVALID_ID;
UInt8 dataProfilesFlag_g = TRAFFIC_GEN_FAILURE;
UInt8 videoProfilesFlag_g = TRAFFIC_GEN_FAILURE;

extern UInt8 setSocketBlocking(UInt8 sockfd, UInt8 blocking);
/* For Generic Traffic Generator */
struct sockaddr_in TrafficGenAdapterSockaddrFrom_g;
struct sockaddr_in PdcpWsharkSockaddrTo_g;
struct sockaddr_in TrafficGenAdapterSockaddrTo_g;
struct sockaddr_in TrafficGenRrcSockaddrTo_g;
struct sockaddr_in TrafficGenOamSockaddrTo_g;
struct sockaddr_in TrafficGenRrcSockaddrFrom_g;
static TrafficGenConfigFileStruct TrafficGenConfigFileStruct_g;

/* Max number of entries actually read from Non Periodic PDCP info file */
UInt64 MaxPdcpDataRead_NPg;

/* Array containing PDCP Data load Info */
trafficGenRateData trafficGenRateForData_g[MAX_PDCP_DATA];

/* Array containing PDCP Video load Info */
trafficGenAudioVideoData trafficGenAudioVideo_g[MAX_UE][MAX_LC];

/* Array containing Non Periodic PDCP load Info */
trafficGenRateNonPeriodic trafficGenRate_NPg[MAX_PDCP_DATA];

/* End Index of timeIntVal Bucket */
UInt64  maxTimeInterval_g = 0;

/* Start Index of timeIntVal Bucket */
UInt64  minTimeInterval_g = MAX_TIME_INTERVAL_BUCKETS;

UInt64  maxTimeIntervalNP_g;
timeInterval_t  *timeIntvalBucket_g[MAX_TIME_INTERVAL_BUCKETS+1];

timeInterval_t  *timeIntvalBucket_NPg[MAX_TIME_INTERVAL_BUCKETS+1];

/* Timer value to use for time tick calculation  */
UInt64             trafficGenTimer_g ;

/* thread id for video profile data */
ULong32 trafficGenVideoThread_g = 0;
struct timespec t1_g,t2_g;

/* Added for calculating throughput at TG */
    /* SPR 15909 fix start */
tickType_t trafficGenTick_g;
    /* SPR 15909 fix end */

UInt32 numOfPacketsSent_g;
UInt64 totalBytesPumped_g;

/* For Pcap file data read. */
UInt8 PcapPacketDataRead_g[2500];
UInt8 PcapFileReadOnceFlag_g = 1;
UInt32 pcapPacketDataLength_g;
#define HEADER_LEN 5

/* Number of Packets to be sent to adapter per UE per tick */
#define NUM_OF_PACKETS_PER_UE 2
UInt8 packetCount_g = NUM_OF_PACKETS_PER_UE;


/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void send_to_socket(void *msgBuf, UInt32 msgLen);
UInt32 retrive_trafficGenInfo_from_configfile();
UInt32 executeTrafficGenConfigFile();
static SInt32 create_bind_socket(TrafficGenConfigFileStruct *);
static UInt32 handleAudioVideoData(UInt32 flagToCreateThread);
static void * trafficGenAudioVideoProcessing();
static void trafficGenProcessAudioVideoFrames(trafficGenAudioVideoData * sendReceiveVideo_p);
void update_lc_traffic_NP(void);
void SendUeDataToAdapter_NP(UInt64 ueIndex);

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Exported functions
 ****************************************************************************/

void diff(struct timespec start, struct timespec end)
{
  struct timespec temp;

#if 1  
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  printf("Difference: SECONDS %ld NANO SECONDS %ld\n", temp.tv_sec, temp.tv_nsec);
  printf("time in ms for one loop %f \n\n",(((UDouble32)temp.tv_sec
          * 1000000000 + temp.tv_nsec)/(1))/1000000);
#else  
  if ((end.tv_usec-start.tv_usec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_usec = 1000000+end.tv_usec-start.tv_usec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_usec = end.tv_usec-start.tv_usec;
  }
  printf("Difference: SECONDS %ld MICRO SECONDS %ld\n", temp.tv_sec, temp.tv_usec);
  printf("time in ms for one loop %f \n\n",(((UDouble32)temp.tv_sec
          * 1000000 + temp.tv_usec)/(1))/1000);
#endif  
  return ;
}

#if 0
void getTrafficGenThroughput()
{
  static UInt32 tgTickTxThroughput_g = 0;
  UInt32 diffTxTime_g = trafficGenTick_g - tgTickTxThroughput_g;
  static FILE * file_p  = NULL;
  if(!file_p){
     file_p = fopen("/root/tg_througput.txt","w");
     if(!file_p)
        return ;
  }
  UDouble32 tgTxThroughput = 0;
  //fprintf(file_p,"Number of packets send by Traffic Generator = %d\n",numOfPacketsSent_g);
  if(diffTxTime_g){
    tgTxThroughput = ((UDouble32)(totalBytesPumped_g*8))/diffTxTime_g;
    tgTxThroughput = (tgTxThroughput*1000)/(1024*1024);
    //fprintf(file_p,"Traffic Generator Throughput is = %6f Mbps\n\n",tgTxThroughput);
  }
  else {
   // fprintf(file_p,"PDCPUSER Throughput is = 0.000000 Mbps\n\n");
    //fprintf(file_p,"PDCP Tx Throughput is = 0.000000 Mbps\n");
  }
  fflush(file_p);
  totalBytesPumped_g = 0;
  tgTickTxThroughput_g = trafficGenTick_g;
}
#else 
void getTrafficGenThroughput()
{
    /* SPR 15909 fix start */
  static tickType_t tgTickTxThroughput_g = 0;
  tickType_t diffTxTime_g = trafficGenTick_g - tgTickTxThroughput_g;
    /* SPR 15909 fix end */
   UDouble32 tgTxThroughput = 0;
  fprintf(stderr,"Number of packets send by Traffic Generator = %d\n",numOfPacketsSent_g);
  if(diffTxTime_g){
    tgTxThroughput = ((UDouble32)(totalBytesPumped_g*8))/diffTxTime_g;
    tgTxThroughput = (tgTxThroughput*1000)/(1024*1024);
    fprintf(stderr,"Traffic Generator Throughput is = %6f Mbps\n\n",tgTxThroughput);
  }
  else {
    fprintf(stderr,"PDCPUSER Throughput is = 0.000000 Mbps\n\n");
    fprintf(stderr,"PDCP Tx Throughput is = 0.000000 Mbps\n");
  }
  totalBytesPumped_g = 0;
  tgTickTxThroughput_g = trafficGenTick_g;
}
#endif


/****************************************************************************
 * Function Name  : parseTrafficGenInfoPerUe
 * Inputs         : None 
 * Outputs        : None
 * Returns        : Success on completion
 * Variables      : 
 * Description    : This function parses the PDCP data simulator file
 ****************************************************************************/
UInt32 parseTrafficGenInfoPerUe(void)
{ 
    /* begin parseTrafficGenInfoPerUe */

    FILE  *infoFile_p    = NULL;
    UInt64 count         = 0;
    UInt64 dataProfileCounter  = 0;
    UInt64 videoProfileCounter = 0;
    UInt32 UeId          = 0;
    UInt32 LcId          = 0;
    UInt32 periodic      = 0;   /* 0 - peridic: 1 - bursty */
    UInt64 DataToPump    = 0;
    UInt32 time_interval = 0; 
    UInt32 init_delay    = 0; 
    UInt32 profile       = 0; 
    UInt16 tickTimer     = 0;
    UChar8 line[1000];
    UChar8 txIpAddress[20] = {0};
    UChar8 rxIpAddress[20] = {0};
    UInt32 txPortNumber = 0, rxPortNumber = 0;
    LTE_LOG(LOG_INFO,PNULL,"In Function - parseTrafficGenInfoPerUe()\n");

    /* get timer value from file */
    if ( trafficGenTimer_g == 0) 
    {
        trafficGenTimer_g = 1; 
    }

    /* open file for reading PDCP data generation information */
    infoFile_p = fopen("../cfg/PdcpUeInfo.txt","r");

    if (infoFile_p == NULL)
    {
        lteWarning("File Not Found. - parseTrafficGenInfoPerUe()");
        return TRAFFIC_GEN_FAILURE;
    }

    while ( (fgets (line, 100, infoFile_p))  &&
            (count < MAX_PDCP_DATA) )
    {
        if ( '#' == line[0])
            continue;

        sscanf( line,
                "%u %u %llu %u %u %u %u %s %u %s %u", 
                &UeId, &LcId, &DataToPump, &periodic, 
                &profile, &time_interval, &init_delay, txIpAddress, &txPortNumber, rxIpAddress, &rxPortNumber);

        if ((time_interval > MAX_TIME_INTERVAL_BUCKETS)||
                (time_interval < 0)) 
        {
            lteWarning("bad time Interval, it should be in range 0 to %d",
                    MAX_TIME_INTERVAL_BUCKETS);
            continue;
        }

        if ( profile > TRAFFIC_GEN_VIDEO_PROFILE)
        {
            lteWarning("Invalid profile %d", profile);
            continue;
        }
        if (profile == TRAFFIC_GEN_DATA_PROFILE)
        {
            trafficGenRateForData_g[count].UeId = UeId;
            trafficGenRateForData_g[count].LcId          = (SInt8)LcId;
            trafficGenRateForData_g[count].profile       = profile;
            trafficGenRateForData_g[count].DataToPump    = DataToPump;
            trafficGenRateForData_g[count].periodicity   = periodic;
            trafficGenRateForData_g[count].time_interval = time_interval;
            trafficGenRateForData_g[count].init_delay    = init_delay;
            trafficGenRateForData_g[count].times = 
              (trafficGenRateForData_g[count].time_interval/trafficGenTimer_g);
            trafficGenRateForData_g[count].tickTimer     = tickTimer;
            trafficGenRateForData_g[count].ueIndex       = TRAFFIC_GEN_INVALID_ID ;
            insert_IndexNodeInList( trafficGenRateForData_g[count].time_interval, 
                trafficGenRateForData_g[count].init_delay, count);
            dataProfileCounter++; 
            LTE_LOG(LOG_DETAIL,PNULL,
                "UeId = [%u] LcId [%u] DataToPump[%u] profile [%u] time_interval [%u] init_delay [%u]\n",
                UeId, LcId, DataToPump, profile, time_interval, init_delay);
        }
        else
        {
            trafficGenAudioVideo_g[UeId][LcId].UeId  = UeId;
            trafficGenAudioVideo_g[UeId][LcId].LcId  = (SInt8)LcId;
            strcpy(trafficGenAudioVideo_g[UeId][LcId].txIpAddress,txIpAddress);
            strcpy(trafficGenAudioVideo_g[UeId][LcId].rxIpAddress,rxIpAddress);
            trafficGenAudioVideo_g[UeId][LcId].txPortNumber = txPortNumber;
            trafficGenAudioVideo_g[UeId][LcId].rxPortNumber = rxPortNumber;
            trafficGenAudioVideo_g[UeId][LcId].isConfigured = TRAFFIC_GEN_UE_ADDED;
            videoProfileCounter++;
            LTE_LOG(LOG_DETAIL,PNULL, "UeId = [%u] LcId [%u] profile [%u] source ip address [%s] \
                destination ip address [%s] source port number [%u] destination port number [%u] \n", \
                    UeId, LcId, DataToPump, profile, txIpAddress, rxIpAddress, txPortNumber, rxPortNumber);
        }
        count++;     
    }
    dataProfilesFlag_g = videoProfilesFlag_g = TRAFFIC_GEN_FAILURE;
    if (dataProfileCounter)
        dataProfilesFlag_g = TRAFFIC_GEN_SUCCESS;
    if (videoProfileCounter)
        videoProfilesFlag_g = TRAFFIC_GEN_SUCCESS;
    fclose(infoFile_p);
    return TRAFFIC_GEN_SUCCESS;

} /* end parseTrafficGenInfoPerUe */

/****************************************************************************
 * Function Name  : parseTrafficGenInfoPerUeNonPeriodic 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : Success on completion
 * Variables      : 
 * Description    : This function parses the Non Periodic PDCP data simulator 
 file
 ****************************************************************************/
UInt32 parseTrafficGenInfoPerUeNonPeriodic(void)
{ 
    /* begin parseTrafficGenInfoPerUeNonPeriodic */

    FILE       * infoFile_p    = NULL;
    UInt64       count         = 0;
    UInt32       UeId          = 0;
    UInt32       LcId          = 0;
    UInt64       DataToPump    = 0;
    UInt32       profile       = 0; 
    UChar8       line[1000] = {0};

    LTE_LOG( LOG_INFO,PNULL,
            "In Function - parseTrafficGenInfoPerUeNonPeriodic()");

    /* get timer value from file */
    if ( trafficGenTimer_g == 0) 
    {
        trafficGenTimer_g = 1; 
    }

    /* open file for reading PDCP data generation information */
    infoFile_p = fopen("../cfg/PdcpUeInfoNonPeriodic.txt","r");

    if (infoFile_p == NULL)
    {
        lteWarning("File Not Found. - parseTrafficGenInfoPerUeNonPeriodic()");
        return TRAFFIC_GEN_FAILURE;
    }

    while ( (fgets (line, 100, infoFile_p))  &&
            (count < MAX_PDCP_DATA) )
    {
        if ( '#' == line[0])
            continue;

        sscanf( line,
                "%u %u %llu %u ", 
                &UeId, &LcId, &DataToPump, &profile);

        if ( profile > TRAFFIC_GEN_DATA_PROFILE)
        {
            lteWarning("Invalid profile %d",
                    profile);
            continue;
        }
        trafficGenRate_NPg[count].UeId          = UeId;
        trafficGenRate_NPg[count].LcId          = (SInt8)LcId;
        trafficGenRate_NPg[count].DataToPump    = DataToPump;
        trafficGenRate_NPg[count].profile       = profile;
        trafficGenRate_NPg[count].ueIndex       = -1 ;


        insert_IndexNodeInList_NP( 0, 0, count);
        LTE_LOG(LOG_DETAIL,PNULL,
                "UeId = [%u] LcId [%u] DataToPump[%u] profile [%u]\n",
                UeId, LcId, DataToPump, profile);         

        count++;     
    }

    /* store Number of records read in global variable for later use */
    MaxPdcpDataRead_NPg  = count;

    fclose(infoFile_p);

    return TRAFFIC_GEN_SUCCESS;

} /* end parseTrafficGenInfoPerUeNonPeriodic */

/****************************************************************************
 * Function Name  : IncrementQueueLoad
 * Inputs         : None 
 * Outputs        : None
 * Returns        : Success on completion
 * Variables      : 
 * Description    : This function parses the PDCP data simulator file
 ****************************************************************************/
void *IncrementQueueLoad(void)
{ 
    /* begin IncrementQueueLoad */
    if (startTrafficGenProcessing_g & dataProfilesFlag_g)
    {
        /* Send Non Periodic Data One Time at Startup */
        if (startNonPeriodicTraffic_g) {
            update_lc_traffic_NP();
            startNonPeriodicTraffic_g = 0;
            /* Delay between periodic and non periodic data */
            usleep(DELAY_AFTER_NP_DATA);
        }
        /* Send Periodic data continuously until there is a stop command 
           from oamrrc simulator which resets startTrafficGenProcessing_g flag */
        update_lc_traffic();
    }
    return PNULL;
} /* end IncrementQueueLoad */

/****************************************************************************
 * Function Name  : trafficGenThread
 * Inputs         : None 
 * Outputs        : None
 * Returns        : Success on completion
 * Variables      : 
 * Description    : This function generates data load from time to time.
 ****************************************************************************/
void * trafficGenThread(void * arg)
{
    struct timespec timer;
    fd_set rfds;
    SInt8 ret = 0;
    /* SPR 15909 fix start */
    tickType_t lastTick = 0;
    /* SPR 15909 fix end */
    timer.tv_nsec = 500000;
    timer.tv_sec = 0;
    FD_ZERO (&rfds);
    FD_SET (0, &rfds);
    lastTick = trafficGenTick_g;
    fprintf(stderr,"trafficGenThread\n");	
    LTE_LOG(LOG_INFO,PNULL , "\n In trafficGenThread()\n");
    if ( trafficGenTimer_g == 0)
    {
        trafficGenTimer_g = 1;
    }

    if (dataProfilesFlag_g & videoProfilesFlag_g)
    {
      handleAudioVideoData(TRAFFIC_GEN_VIDEO_THREAD_CREATE);
      while (1)
      {
            do {
            ret = pselect(1, &rfds, NULL, NULL, &timer, NULL);
            if (ret == -1) {
                 perror("select failed\n");
            }
          }while(lastTick == trafficGenTick_g);

            lastTick = trafficGenTick_g;

            if((trafficGenTick_g%diffInMiliSec_g) == 0)
            {
                getTrafficGenThroughput();
            }    
          IncrementQueueLoad();
      }
    }
    else if (dataProfilesFlag_g)
    {
        while(1)
        {
            do{
            ret = pselect(1, &rfds, NULL, NULL, &timer, NULL);
            if (ret == -1) {
                  perror("select failed\n");
               }
            }while(lastTick == trafficGenTick_g);

            lastTick = trafficGenTick_g;

            if((trafficGenTick_g%diffInMiliSec_g) == 0)
            {
                getTrafficGenThroughput();
            }    
            IncrementQueueLoad();
        }
    }
    else     
        handleAudioVideoData(TRAFFIC_GEN_VIDEO_NO_THREAD_CREATE);

   return PNULL;
} /* end trafficGenThread */


/****************************************************************************
 * Function Name  : handleAudioVideoData 
 * Inputs         : flagToCreateThread
 * Outputs        : None
 * Returns        : Success on completion
 * Variables      : 
 * Description    : This function work either create Video/Audio processing
 *                : thread or uses the calls the same Video/Audio processing
 *                : function. 
 ****************************************************************************/

static UInt32 handleAudioVideoData(UInt32 flagToCreateThread)
{
   if (TRAFFIC_GEN_VIDEO_THREAD_CREATE == flagToCreateThread)
   {
      cpu_set_t cpuSetSch;
      UInt16 retVal = TRAFFIC_GEN_SUCCESS;
      Int res = 0 ;
      Int coreNum = 0 ;
      retVal = threadCreate(&trafficGenVideoThread_g, THREAD_CREATE_DETACHED,
        THREAD_SCHED_FIFO,0,
        THREAD_EXPLICIT_SCHED,
        THREAD_SCOPE_SYSTEM, trafficGenAudioVideoProcessing, PNULL);
      if (retVal)
      {
          ltePanic("Error creating trafficGenVideoThread_g in Fn handleAudioVideoData()");
          return TRAFFIC_GEN_FAILURE;
      }
     CPU_ZERO(&cpuSetSch);
     coreNum =  0;
     CPU_SET(coreNum,&cpuSetSch);
     res = pthread_setaffinity_np(trafficGenVideoThread_g,
         sizeof(cpuSetSch), &cpuSetSch);
     if (res != 0 )
     {
       ltePanic(" Unable to set affinity for Traffic Gen thread.\n");
       return TRAFFIC_GEN_FAILURE;
     } 
      //fprintf(stderr, "##traffic Gen video thread masked at coreNum %d\n", coreNum);
   }
   else
       trafficGenAudioVideoProcessing(); 
   return TRAFFIC_GEN_SUCCESS;
} 

/****************************************************************************
 * Function Name  : trafficGenAudioVideoProcessing 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : PNULL 
 * Variables      : 
 * Description    : This function is either used as a thread or as a funciton
 *                  for Video or Audio Processing.
 ****************************************************************************/

static void * trafficGenAudioVideoProcessing()
{
   UInt32 ueIndex = 0;
   UInt64 ueId = 0;
   UInt8 lcId = 0;
   UInt8 maxFileDes = 0;
   ueLcMappingForAudioVideo ueLcMappingOnVlcFd[500];
   SInt8 audioVideoFileDes[100] = {0};
   UInt16 counter = 0, startVal = 0;
   fd_set readFD;
   FD_ZERO(&readFD);
   memset(ueLcMappingOnVlcFd,0,sizeof(ueLcMappingOnVlcFd));
   while(1)
   {
      if(startTrafficGenProcessing_g)
      {  
         for(;ueIndex < MAX_UE; ueIndex++)
          {
            for(lcId =0;lcId < MAX_LC; lcId++)
            {
                ueId = ueIndexToIdMapTable_g[ueIndex];
                if(TRAFFIC_GEN_SUCCESS == ((trafficGenAudioVideo_g[ueId][lcId].isConfigured & TRAFFIC_GEN_UE_CONFIGURED)>>1))
                {
                   FD_SET(trafficGenAudioVideo_g[ueId][lcId].vlcSocketFd,&readFD);
                   ueLcMappingOnVlcFd[trafficGenAudioVideo_g[ueId][lcId].vlcSocketFd].ueIndex = ueIndex;
                   ueLcMappingOnVlcFd[trafficGenAudioVideo_g[ueId][lcId].vlcSocketFd].lcId = lcId;
                   audioVideoFileDes[counter++] = maxFileDes =  trafficGenAudioVideo_g[ueId][lcId].vlcSocketFd;
                   if ((counter >= 2) && (audioVideoFileDes[counter-1] > audioVideoFileDes[counter-2]))
                       maxFileDes = audioVideoFileDes[counter-1];
                } 
             }	
          }
          while (1) 
          {
              if (select(maxFileDes+1 ,&readFD,PNULL,PNULL,PNULL) == -1)
                 lteWarning("Server-select error\n ");
              else
              {
                 for (startVal = 0; startVal <counter ; startVal++)
                 {
                     if (FD_ISSET(audioVideoFileDes[startVal],&readFD))
                     {
                          //fprintf(stderr,"Data received from fd %d and Ue Id is %lld and lcId is %d",audioVideoFileDes[startVal],ueIndexToIdMapTable_g[ueLcMappingOnVlcFd[audioVideoFileDes[startVal]].ueIndex],ueLcMappingOnVlcFd[audioVideoFileDes[startVal]].lcId); 
                          trafficGenProcessAudioVideoFrames(&trafficGenAudioVideo_g[ueIndexToIdMapTable_g[ueLcMappingOnVlcFd[audioVideoFileDes[startVal]].ueIndex]][ueLcMappingOnVlcFd[audioVideoFileDes[startVal]].lcId]);
                     }
                 }
             }
             FD_ZERO(&readFD);
             for (startVal = 0; startVal < counter ; startVal++)
                FD_SET(audioVideoFileDes[startVal],&readFD); 
         }
      }
      sleep(1); // sleep for 1 second to avoid 100% CPU Usage till configuration is not completed
   }
   return PNULL; 
}

/****************************************************************************
 * Function Name  : trafficGenProcessAudioVideoFrames
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function handles receive and send request for Audio 
 *                : and Video. 
 ****************************************************************************/
static void trafficGenProcessAudioVideoFrames(trafficGenAudioVideoData * sendReceiveVideo_p)
{
    SInt32 bytesRead = -1;
    UInt8 recvBuffer[BUFFERSIZE] = {0};
    SInt32 ret = -1;
    socklen_t addrLen = 0;
    UInt16 counter = 0;
    //static UInt64 noOfPacktes = 0;
    addrLen = sizeof(sendReceiveVideo_p->videoSockaddrFrom);
    bzero(recvBuffer,sizeof(recvBuffer));
    LTE_SET_U16BIT(&recvBuffer[counter],sendReceiveVideo_p->ueIndex);
    counter +=2;
    recvBuffer[counter++] = sendReceiveVideo_p->LcId;
    bytesRead= recvfrom(sendReceiveVideo_p->vlcSocketFd,recvBuffer+counter+2,BUFFERSIZE,0,
            (struct sockaddr *)&(sendReceiveVideo_p->videoSockaddrFrom), &addrLen);
    //fprintf(stderr,"T.G. => Bytes Received from Upper Layers is %d Ue Index %d and lcId %d\n",bytesRead,sendReceiveVideo_p->ueIndex, sendReceiveVideo_p->LcId);
    LTE_SET_U16BIT(&recvBuffer[counter], bytesRead);
    counter +=2;
    ret = sendto(trafficGTxSocket_g, recvBuffer, counter+bytesRead,\
               0,(struct sockaddr*)&(TrafficGenAdapterSockaddrTo_g),\
                   sizeof(TrafficGenAdapterSockaddrTo_g)); 
    //fprintf(stderr,"T.G => Number of Packets sent to Adapter is %lld\n",noOfPacktes++); 
    //fprintf(stderr,"T.G. => Bytes sent to PDCP Adapter is %d\n",ret);
    if(-1 == ret)
        ltePanic("sendto failed to Pdcp Adapter Sock addr...........\n");
}

/****************************************************************************
 * Function Name  : update_lc_traffic_NP
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function generates PDCP load one time for Non
 *                  Periodic Users at startup.
 ****************************************************************************/
void update_lc_traffic_NP(void)
{
    UInt64 timeInterval = 0;
    initialDelay_t *initialDelayNode = PNULL;
    indexListNode_t *indexNode  = PNULL;

    initialDelayNode = getInitialDelayList_NP(timeInterval);
    if (PNULL != initialDelayNode)
    {
        indexNode = 
            (indexListNode_t *) getListNode( &(initialDelayNode->indexList), 0);
        while(PNULL != indexNode)
        {
            SendUeDataToAdapter_NP(indexNode->index);
            LTE_LOG(LOG_DETAIL,PNULL,"[%s]UEID [%d] Lcid [%d] PdcpIdx [%d]",__func__,
                    trafficGenRate_NPg[indexNode->index].UeId,
                    trafficGenRate_NPg[indexNode->index].LcId,
                    indexNode->index);

            indexNode = 
                (indexListNode_t *) getNextListNode( &(indexNode->nodeAnchor));
        }
    }
}

/****************************************************************************
 * Function Name  : update_lc_traffic
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function generates PDCP load periodically for Periodic
 *                  Users at runtime.
 ****************************************************************************/
UInt64 update_lc_traffic(void)
{
    UInt64               timeInterval = 0;
    initialDelay_t       *initialDelayNode = PNULL;
    indexListNode_t  *indexNode  = PNULL;

    for( timeInterval = minTimeInterval_g; timeInterval <= maxTimeInterval_g; timeInterval++)
    {
        if (PNULL != timeIntvalBucket_g[timeInterval])
        {
            initialDelayNode = getInitialDelayList(timeInterval);

            if (PNULL != initialDelayNode)
            {
                indexNode = (indexListNode_t *) getListNode( &(initialDelayNode->indexList), 0);
                while(PNULL != indexNode)
                {
                    SendUeDataToAdapter(indexNode->index);

                    indexNode =
                        (indexListNode_t *) getNextListNode( &(indexNode->nodeAnchor));
                }
            }
            timeIntvalBucket_g[timeInterval]->initialDelay++;

            if ( timeIntvalBucket_g[timeInterval]->initialDelay >= 
                    timeIntvalBucket_g[timeInterval]->maxInitialDelay)
                timeIntvalBucket_g[timeInterval]->initialDelay = 0;
        }
    }
    return 1;
}

/****************************************************************************
 * Function Name  : getInitialDelayList
 * Inputs         : timeInterval - index in bucket for which list ptr required 
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function finds and return the initialDelayList Ptr
 *                  depending upon timeInterval sent.
 ****************************************************************************/
initialDelay_t *getInitialDelayList(UInt64 timeInterval)
{
    initialDelay_t  *initialDelayNode = PNULL;

    if (PNULL == timeIntvalBucket_g[timeInterval])
        return PNULL;

    initialDelayNode = 
        (initialDelay_t *)getListNode(&(timeIntvalBucket_g[timeInterval]->initialDelayList), 0);

    while(PNULL != initialDelayNode)
    {
        if (timeIntvalBucket_g[timeInterval]->initialDelay == initialDelayNode->initialDelay)
            return initialDelayNode;

        initialDelayNode = 
            (initialDelay_t  *) getNextListNode( &(initialDelayNode->nodeAnchor));
    }

    return initialDelayNode;
}

/****************************************************************************
 * Function Name  : getInitialDelayList_NP
 * Inputs         : timeInterval - index in bucket for which list ptr required 
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function finds and return the initialDelayList Ptr
 *                  for non periodic load.
 ****************************************************************************/
initialDelay_t *getInitialDelayList_NP(UInt64 timeInterval)
{
    initialDelay_t  *initialDelayNode = PNULL;

    if (PNULL == timeIntvalBucket_NPg[timeInterval])
        return PNULL;

    initialDelayNode =
        (initialDelay_t *)getListNode(&(timeIntvalBucket_NPg[timeInterval]->initialDelayList), 0);

    while(PNULL != initialDelayNode)
    {
        if (timeIntvalBucket_NPg[timeInterval]->initialDelay == initialDelayNode->initialDelay)
            return initialDelayNode;

        initialDelayNode =
            (initialDelay_t  *) getNextListNode( &(initialDelayNode->nodeAnchor));
    }

    return initialDelayNode;
}


/****************************************************************************
 * Function Name  : insert_IndexNodeInList
 * Inputs         : time_val - index in bucket at which node to be added.
 *                  initDelay - initial delay for the user.
 *                  index - ueIndex of the user.
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function inserts node in periodic bucket
 ****************************************************************************/
void insert_IndexNodeInList(UInt32 time_val, UInt16 initDelay, UInt64 index)
{
    UInt32            numNodesInlist = 0;
    UInt16            node_found       = 0;
    initialDelay_t    *initDelayNode  = PNULL;

    initDelay = ((initDelay < (trafficGenRateForData_g[index].times))?(initDelay):(0));

    if ( PNULL == timeIntvalBucket_g[time_val])
    {
        timeIntvalBucket_g[time_val] = (timeInterval_t *) getMemFromPool( sizeof(timeInterval_t), PNULL);
        if (PNULL == timeIntvalBucket_g[time_val])
        {
            ltePanic(" [%s]getMemFromPool() failed \n",__func__);
            return;
        }

        timeIntvalBucket_g[time_val]->ticks = (time_val/trafficGenTimer_g);
        timeIntvalBucket_g[time_val]->maxInitialDelay  = 0;
        timeIntvalBucket_g[time_val]->initialDelay = 0;
        if (timeIntvalBucket_g[time_val]->ticks)
            timeIntvalBucket_g[time_val]->maxInitialDelay = 
                (timeIntvalBucket_g[time_val]->ticks);

        listInit(&timeIntvalBucket_g[time_val]->initialDelayList);

        maxTimeInterval_g = 
            ((time_val > maxTimeInterval_g) ? (time_val) : (maxTimeInterval_g));

        minTimeInterval_g = 
            ((time_val < minTimeInterval_g) ? (time_val) : (minTimeInterval_g));
    }

    if ((numNodesInlist = 
                listCount( &(timeIntvalBucket_g[time_val]->initialDelayList))))
    {  
        UInt32 i = 0;

        for ( i = 0; i < numNodesInlist; i++)
        {
            initDelayNode = (initialDelay_t *) getListNode( &(timeIntvalBucket_g[time_val]->initialDelayList), i);

            if (initDelayNode->initialDelay == initDelay)
            {
                /* list for the given initial delay is already created. Insert 
                   the trafficGen index node in the initialdelay list  */
                if (TRAFFIC_GEN_FAILURE == insert_NodeInlist( initDelayNode, index))
                {
                    return;
                }      
                node_found = 1;
                break;
            }
        }
    }

    if (!node_found)
    {
        /* list for the given initial delay not created. Create the list 
           for given initial delay */
        initDelayNode = 
            insert_initDelayNodeInTimeIntvalList( time_val, initDelay);
        if (PNULL == initDelayNode)
        {
            lteWarning("%s :Ignoring the node",__func__);
            return;
        }
        if (TRAFFIC_GEN_FAILURE == insert_NodeInlist( initDelayNode, index))
        {
            lteWarning("%s :Ignoring the node",__func__);      
            return;
        }
    }
}

/****************************************************************************
 * Function Name  : insert_IndexNodeInList_NP
 * Inputs         : time_val - index in bucket at which node to be added.
 *                  initDelay - initial delay for the user.
 *                  index - ueIndex of the user.
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function inserts node in non periodic bucket
 ****************************************************************************/
void insert_IndexNodeInList_NP(UInt32 time_val, UInt16 initDelay, UInt64 trafficGenIndex)
{
    UInt32 numNodesInlist = 0;
    UInt16 node_found     = 0;
    initialDelay_t *initDelayNode = PNULL;

    if ( PNULL == timeIntvalBucket_NPg[time_val])
    {
        timeIntvalBucket_NPg[time_val] 
            = (timeInterval_t *) getMemFromPool( sizeof(timeInterval_t), PNULL);
        if (PNULL == timeIntvalBucket_NPg[time_val])
        {
            ltePanic(" [%s]getMemFromPool() failed \n",__func__);
            return;
        }

        timeIntvalBucket_NPg[time_val]->ticks = (time_val/trafficGenTimer_g);
        timeIntvalBucket_NPg[time_val]->maxInitialDelay  = 0;
        timeIntvalBucket_NPg[time_val]->initialDelay = 0;
        if (timeIntvalBucket_NPg[time_val]->ticks)
            timeIntvalBucket_NPg[time_val]->maxInitialDelay =
                (timeIntvalBucket_NPg[time_val]->ticks);

        listInit( &timeIntvalBucket_NPg[time_val]->initialDelayList);

        maxTimeIntervalNP_g =
            ((time_val > maxTimeIntervalNP_g) ? (time_val) : (maxTimeIntervalNP_g));
    }

    if ((numNodesInlist =
                listCount( &(timeIntvalBucket_NPg[time_val]->initialDelayList))))
    {
        UInt32 i = 0;

        for ( i = 0; i < numNodesInlist; i++)
        {
            initDelayNode = (initialDelay_t *) getListNode( &(timeIntvalBucket_NPg[time_val]->initialDelayList), i);

            if (initDelayNode->initialDelay == initDelay)
            {
                /* list for the given initial delay is already created. Insert
                   the trafficGen index node in the initialdelay list  */
                if (TRAFFIC_GEN_FAILURE == insert_NodeInlist( initDelayNode, trafficGenIndex))
                {
                    //log
                    return;
                }
#ifdef TRAFFIC_GEN_LOG
                LTE_LOG(LOG_DETAIL,PNULL,"[RLC][%s]inserted trafficGenIndex %d with init delay %d \n",trafficGenIndex,initDelay);
#endif
                node_found = 1;
                break;
            }
        }
    }

    if (!node_found)
    {
        /* list for the given initial delay not created. Create the list
           for given initial delay */
#ifdef TRAFFIC_GEN_LOG
        LTE_LOG(LOG_DETAIL,PNULL,"[RLC][%s]inserting first NODE at time_val %d initdelay %d\n",
                __func__, time_val,initDelay);
#endif
        initDelayNode =
            insert_initDelayNodeInTimeIntvalList_NP( time_val, initDelay);
        if (PNULL == initDelayNode)
        {
            lteWarning("%s :Ignoring the node",__func__);
            return;
        }
        if (TRAFFIC_GEN_FAILURE == insert_NodeInlist( initDelayNode, trafficGenIndex))
        {
            lteWarning("%s :Ignoring the node",__func__);
            return;
        }
    }
}

/****************************************************************************
 * Function Name  : insert_NodeInlist
 * Inputs         : initialDelayNode - Ptr to initial delay node.
 *                  index - Index at which node to be inserted.
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function inserts node in indexList
 ****************************************************************************/
UInt16 insert_NodeInlist(initialDelay_t *initialDelayNode, UInt32 index)
{
    indexListNode_t  *inxNode    = PNULL;

    inxNode = (indexListNode_t *) getMemFromPool( sizeof (indexListNode_t), PNULL);
    if (PNULL == inxNode)
    {
        ltePanic(" [%s]getMemFromPool() failed \n",__func__);
        return TRAFFIC_GEN_FAILURE;
    }
    inxNode->index = index;

    listInsertNode( &(initialDelayNode->indexList), 
            &(inxNode->nodeAnchor));
    return TRAFFIC_GEN_SUCCESS;
}

/****************************************************************************
 * Function Name  : insert_initDelayNodeInTimeIntvalList
 * Inputs         : time_val - index in bucket at which node to be added.
 *                  initDelay - initial delay for the user.
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function inserts node in Time Interval periodic bucket
 ****************************************************************************/
initialDelay_t * insert_initDelayNodeInTimeIntvalList(UInt32 time_val, UInt16 initDelay)
{
    initialDelay_t  *initDelayNode = PNULL;

    initDelayNode = (initialDelay_t *) getMemFromPool( sizeof (initialDelay_t), PNULL);
    if (PNULL == initDelayNode)
    {
        ltePanic(" [%s]getMemFromPool() failed \n",__func__);
        return initDelayNode;
    }

    initDelayNode->initialDelay = initDelay;
    listInit( &(initDelayNode->indexList));

    listInsertNode( &(timeIntvalBucket_g[time_val]->initialDelayList), 
            &(initDelayNode->nodeAnchor));

    return initDelayNode;
}

/****************************************************************************
 * Function Name  : insert_initDelayNodeInTimeIntvalList_NP
 * Inputs         : time_val - index in bucket at which node to be added.
 *                  initDelay - initial delay for the user.
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function inserts node in Time Interval non periodic bucket
 ****************************************************************************/
initialDelay_t * insert_initDelayNodeInTimeIntvalList_NP(UInt32 time_val, UInt16 initDelay)
{
    initialDelay_t  *initDelayNode = PNULL;

    initDelayNode = (initialDelay_t *) getMemFromPool( sizeof (initialDelay_t), PNULL);
    if (PNULL == initDelayNode)
    {
        ltePanic(" [%s]getMemFromPool() failed \n",__func__);
        return initDelayNode;
    }

    initDelayNode->initialDelay = initDelay;
    listInit( &(initDelayNode->indexList));

    listInsertNode( &(timeIntvalBucket_NPg[time_val]->initialDelayList),
            &(initDelayNode->nodeAnchor));

    return initDelayNode;
}

/****************************************************************************
 * Function Name  : retrive_trafficGenInfo_from_configfile
 * Inputs         : configfile_p : pointer to trafficGen config file
 * Outputs        :
 * Returns        : Zero on success
 * Description    : function retrive wireshark port,simulator port , host name
 *                  from config file for traffic generator.
 ****************************************************************************/
UInt32 retrive_trafficGenInfo_from_configfile()
{
    FILE* fp;
    UInt32 port = 0;
    UChar8  buf[101] = {0,};
    UChar8 getstring[FILENAMESIZE] = {0,};

    //UChar8 *configfile_p = "../../../lteLayer2PortConfig.cfg";    
    UChar8 *configfile_p = "../cfg/lteUeSimPortIpConfig.cfg";    
    memset(&TrafficGenConfigFileStruct_g,0,sizeof(TrafficGenConfigFileStruct));

    fp = fopen(configfile_p,"rb");
    if(fp == PNULL)
    {
        ltePanic("\nError: Unable to open Config file for PDCP Sim \n");
        return TRAFFIC_GEN_FAILURE;
    }

    while(fgets(buf,100,fp))
    {
        if(sscanf(buf,"UESIM_L2_MODULE_HOST_NAME=%s",getstring) > 0)
        {
            memcpy(TrafficGenConfigFileStruct_g.adapterHostName, getstring,
                    sizeof(TrafficGenConfigFileStruct_g.adapterHostName));
        }
        else if(sscanf(buf,"WiresharkPort=%d",&port) > 0)
        {
            TrafficGenConfigFileStruct_g.wireSharkPort = (unsigned short)port;
        }
        else if(sscanf(buf,"TRAFFIC_GENERATOR_ADAPTER_RX_PORT=%d",&port) > 0)
        {
            TrafficGenConfigFileStruct_g.trafficGeneratorAdapterRxPort = (unsigned short)port;
        }
        else if(sscanf(buf,"TRAFFIC_GENERATOR_PORT=%d",&port) > 0)
        {
            TrafficGenConfigFileStruct_g.trafficGeneratorOamRrcRxPort = (unsigned short)port;
        }
        else if(sscanf(buf,"UESIM_ADAPTER_RX_PORT=%d",&port) > 0)
        {
            TrafficGenConfigFileStruct_g.adapterPort = (unsigned short)port;
        }
        else if(sscanf(buf,"OAM_MODULE_HOST_NAME=%s",getstring) > 0)
        {
            memcpy(TrafficGenConfigFileStruct_g.oamHostName, getstring,
                    sizeof(TrafficGenConfigFileStruct_g.oamHostName));
        }
        else if(sscanf(buf,"UESIM_RRC_MODULE_HOST_NAME=%s",getstring) > 0)
        {
            memcpy(TrafficGenConfigFileStruct_g.rrcHostName, getstring,
                    sizeof(TrafficGenConfigFileStruct_g.rrcHostName));
        }
        else if(sscanf(buf,"RRC_SIMULATOR_PORT=%d",&port) > 0)
        {
            TrafficGenConfigFileStruct_g.rrcSimPort = (unsigned short)port;
        }
        else if(sscanf(buf,"OAM_SIMULATOR_PORT=%d",&port) > 0)
        {
            TrafficGenConfigFileStruct_g.oamSimPort = (unsigned short)port;
        }
    }
    fclose(fp);
    LTE_LOG( LOG_DETAIL,PNULL,"configfile Read successfully for Traffic Generator\n\n");
    return TRAFFIC_GEN_SUCCESS;
}

/****************************************************************************
 * Function Name  : executeTrafficGenConfigFile
 * Inputs         : configfile_p - pointer to trafficGen config file
 *                  trafficGeneratorRxPort - TrafficGeneratorRxPort port define in config file
 *                  wireSharkPort - wireSharkPort define in config file
 *                  hostName - hostname for wireshark
 * Outputs        :
 * Returns        : Zero on success
 * Description    : function read config file for trafficGen simulator ,
 *                  create and bind socket for wireshark
 ****************************************************************************/
UInt32 executeTrafficGenConfigFile()
{

    if(!retrive_trafficGenInfo_from_configfile())
    {
        ltePanic("Failed To retrive_siminfo_from_configfile\n");
        return TRAFFIC_GEN_FAILURE;
    }

    if(!TrafficGenConfigFileStruct_g.trafficGeneratorAdapterRxPort)
        TrafficGenConfigFileStruct_g.trafficGeneratorAdapterRxPort = DEFAULT_SIM_PORT;

    if(!TrafficGenConfigFileStruct_g.trafficGeneratorOamRrcRxPort)
        TrafficGenConfigFileStruct_g.trafficGeneratorOamRrcRxPort = DEFAULT_SIM_PORT+1;

    if(!TrafficGenConfigFileStruct_g.rrcSimPort)
        TrafficGenConfigFileStruct_g.rrcSimPort = DEFAULT_SIM_PORT+2;

    if(!TrafficGenConfigFileStruct_g.oamSimPort)
        TrafficGenConfigFileStruct_g.oamSimPort = DEFAULT_SIM_PORT+3;

    if(!TrafficGenConfigFileStruct_g.wireSharkPort)
        TrafficGenConfigFileStruct_g.wireSharkPort = DEFAULT_WIRESHARK_PORT;

    if(!TrafficGenConfigFileStruct_g.adapterPort)
        TrafficGenConfigFileStruct_g.adapterPort = DEFAULT_PDCPADAPTER_PORT;

    if(!strlen(TrafficGenConfigFileStruct_g.adapterHostName))
        strcpy(TrafficGenConfigFileStruct_g.adapterHostName,"127.0.0.1");
    if(!strlen(TrafficGenConfigFileStruct_g.oamHostName))
        strcpy(TrafficGenConfigFileStruct_g.oamHostName,"127.0.0.1");
    if(!strlen(TrafficGenConfigFileStruct_g.rrcHostName))
        strcpy(TrafficGenConfigFileStruct_g.rrcHostName,"127.0.0.1");
    if(create_bind_socket(&TrafficGenConfigFileStruct_g))
    {
        ltePanic("Error: create_bind_socket Failed for PDCP Sim\n");
        return TRAFFIC_GEN_FAILURE;
    }

    return TRAFFIC_GEN_SUCCESS;
}

/****************************************************************************
 * Fuctionn Name  : send_to_socket
 * Inputs         : msgBuf: data to be send on socket for wireshark 
 *                  msgLen: length of data
 *                 
 * Outputs        :
 * Returns        :
 * Description    : function send data to adapter and wireshark on socket
 ****************************************************************************/
static void send_to_socket(void *msgPtr, UInt32 msgLen)
{
    Int ret = 0;
    ret = sendto(trafficGTxSocket_g, msgPtr, msgLen, 0,
            (struct sockaddr*)&(TrafficGenAdapterSockaddrTo_g),
            sizeof(TrafficGenAdapterSockaddrTo_g));

    if(-1 == ret)
        ltePanic("sendto failed to Pdcp Adapter Sock addr...........\n");
    //fprintf(stderr,"Packets sent by TG to Adapter %d",ret);
    numOfPacketsSent_g += 1;
    totalBytesPumped_g += (msgLen - HEADER_LEN);

#ifdef SEND_TO_WIRESHARK
    ret = sendto(TrafficGenAdapterSocket_g, msgPtr, msgLen, 0, 
            (struct sockaddr*)&(PdcpWsharkSockaddrTo_g),
            sizeof(PdcpWsharkSockaddrTo_g));
#endif    
    if(-1 == ret)
        ltePanic("sendto failed to Pdcp Wireshark Sock addr...........\n");
}

/****************************************************************************
 * Fuctionn Name  : createSocketForAudioVideo
 * Inputs         : trafficGenAudioVideoData structure pointer, direction 
 * Outputs        : None
 * Returns        : -1, 0 or 1
 * Description    : Function used to create sockets for Video and Audio profiles
 ****************************************************************************/
SInt32 createSocketForAudioVideo(trafficGenAudioVideoData * configInfo_p,UInt8 dir)
{
    struct hostent *host_p;
    int ret = 0;

    if (TRAFFIC_GEN_RX_DIR == dir || TRAFFIC_GEN_RX_TX_DIR == dir)
    {
        if((configInfo_p->vlcSocketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            ltePanic("Error:creating socket error\n");
            ret = -1;
        } 
        else
        {
            configInfo_p->videoSockaddrFrom.sin_family = AF_INET;
            configInfo_p->videoSockaddrFrom.sin_port
               = htons(configInfo_p->rxPortNumber);
            configInfo_p->videoSockaddrFrom.sin_addr.s_addr
               = inet_addr(configInfo_p->adapterHostName);
            bzero(&((configInfo_p->videoSockaddrFrom).sin_zero),8);

            if(bind(configInfo_p->vlcSocketFd,
               (struct sockaddr*)&configInfo_p->videoSockaddrFrom,
                  sizeof(struct sockaddr))== -1)
            {
               ltePanic("Error: Socket bind failed with (%u) port\n",
                  configInfo_p->rxPortNumber);
               ret = -1;
            }
        }
    }
    if (TRAFFIC_GEN_TX_DIR == dir || TRAFFIC_GEN_RX_TX_DIR == dir)
    {
        if(TRAFFIC_GEN_INVALID_ID == vlcTxSocketFd_g) 
        {
           if((vlcTxSocketFd_g = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
           {
              ltePanic("Error:creating socket error\n");
              ret = -1;
           }
        }    
        host_p = (struct hostent *) gethostbyname(configInfo_p->txIpAddress);
        configInfo_p->videoSockaddrTo.sin_family = AF_INET;
        configInfo_p->videoSockaddrTo.sin_port = htons(configInfo_p->txPortNumber);
        configInfo_p->videoSockaddrTo.sin_addr = *((struct in_addr *)host_p->h_addr);
    }
    return TRAFFIC_GEN_SUCCESS;
}

/****************************************************************************
 * Fuctionn Name  : create_bind_socket
 * Inputs         : trafficGeneratorRxPort:trafficGeneratorRxPort port defined in config file
 *                  wireSharkPort:wireSharkPort port defined in config file
 * Outputs        :
 * Returns        : Zero on success
 * Description    : function create and bind socket for simualator and wireshark
 ****************************************************************************/
static SInt32 create_bind_socket(TrafficGenConfigFileStruct *configInfo)
{
    struct hostent *host_p;
    int ret = 0;

    if((TrafficGenAdapterSocket_g = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("Error:creating socket error\n");
        ret = -1;
    }
    else 
    {

      if((trafficGTxSocket_g = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
      {
         ltePanic("Error:creating socket error\n");
         ret = -1;
      }
      host_p = (struct hostent *) gethostbyname(configInfo->adapterHostName);

      TrafficGenAdapterSockaddrFrom_g.sin_family = AF_INET;
      TrafficGenAdapterSockaddrFrom_g.sin_port 
        = htons(configInfo->trafficGeneratorAdapterRxPort);
      TrafficGenAdapterSockaddrFrom_g.sin_addr.s_addr
        = inet_addr(configInfo->adapterHostName);
      bzero(&((TrafficGenAdapterSockaddrFrom_g).sin_zero),8);

      setSocketBlocking(TrafficGenAdapterSocket_g,1);
      if(bind(TrafficGenAdapterSocket_g, 
            (struct sockaddr*)&TrafficGenAdapterSockaddrFrom_g,
            sizeof(struct sockaddr))== -1)
      {
        ltePanic("Error: Socket bind failed with (%u) port\n",
            configInfo->trafficGeneratorAdapterRxPort);
        ret = -1;
      }

      TrafficGenAdapterSockaddrTo_g.sin_family = AF_INET;
      TrafficGenAdapterSockaddrTo_g.sin_port = htons(configInfo->adapterPort);
      TrafficGenAdapterSockaddrTo_g.sin_addr = *((struct in_addr *)host_p->h_addr);   

      PdcpWsharkSockaddrTo_g.sin_family = AF_INET;
      PdcpWsharkSockaddrTo_g.sin_port = htons(configInfo->wireSharkPort);
      PdcpWsharkSockaddrTo_g.sin_addr = *((struct in_addr *)host_p->h_addr);
    }

    if((TrafficGenOamRrcSocket_g = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("Error:creating socket error\n");
        ret = -1;
    }
    else
    {
        TrafficGenRrcSockaddrFrom_g.sin_family = AF_INET;
        TrafficGenRrcSockaddrFrom_g.sin_port 
            = htons(configInfo->trafficGeneratorOamRrcRxPort);
        TrafficGenRrcSockaddrFrom_g.sin_addr.s_addr
            = inet_addr(configInfo->adapterHostName);
        bzero(&((TrafficGenRrcSockaddrFrom_g).sin_zero),8);


        setSocketBlocking(TrafficGenOamRrcSocket_g,1);
        if(bind(TrafficGenOamRrcSocket_g, 
                    (struct sockaddr*)&TrafficGenRrcSockaddrFrom_g,
                    sizeof(struct sockaddr))== -1)
        {
            ltePanic("Error: Socket bind failed with (%u) port\n",
                    configInfo->trafficGeneratorOamRrcRxPort);
            ret = -1;
        }

        host_p = (struct hostent *) gethostbyname(configInfo->rrcHostName);
        TrafficGenRrcSockaddrTo_g.sin_family = AF_INET;
        TrafficGenRrcSockaddrTo_g.sin_port = htons(configInfo->rrcSimPort);
        TrafficGenRrcSockaddrTo_g.sin_addr = *((struct in_addr *)host_p->h_addr);

        host_p = (struct hostent *) gethostbyname(configInfo->oamHostName);
        TrafficGenOamSockaddrTo_g.sin_family = AF_INET;
        TrafficGenOamSockaddrTo_g.sin_port = htons(configInfo->oamSimPort);
        TrafficGenOamSockaddrTo_g.sin_addr = *((struct in_addr *)host_p->h_addr);
    }

    return ret ;
}

/****************************************************************************
 * Function Name  : trafficGenSwap32
 * Inputs         : UInt32 value
 * Outputs        : None.
 * Returns        : value with swapped bytes
 * Description    : calculates value with swapped bytes
 ****************************************************************************/
UInt32 trafficGenSwap32(UInt32 value)
{
    return ((value&0xFF)<<24) |
        (((value>>8)&0xFF)<<16) |
        (((value>>16)&0xFF)<<8) |
        ((value>>24)&0xFF);
}

/***********************************************************************************
 * FUNCTION NAME    :   userFileGetPacketSize
 * DESCRIPTION      :   Returns length of next packet in wireshark file
 * PARAMATERS       :   [IN] FILE* - file descriptor
 * RETURN VALUE     :   [OUT] UInt32 - length of next packet in bytes
 *                      (without ethernet header)
 ***********************************************************************************/
UInt32 userFileGetPacketSize(FILE* fd)
{
    /*structure, which represents header of each packet in wireshark`s format*/
    WsPacketHeader    pack_head;

    UInt32              len_packet;
    SInt32              fp;

    assert(fd);

    fp = ftell(fd);

    /*skip wireshark`s file header*/
    if(0 == fp)
    {
        fseek(fd, LEN_WS_FILE_HEADER, SEEK_SET);
    }

    /*read packet`s header*/
    if(fread(&pack_head, 1, sizeof(pack_head), fd) != sizeof(pack_head))
    {
        return 0;
    }
    len_packet = ntohl(trafficGenSwap32(pack_head.len_capt));

    /*length can`t be bigger than 0xFFFF*/
    if( (len_packet == 0) || (len_packet > 0xffff) )
    {
        return 0;
    }

    /*length without ethernet header*/
    return len_packet - LEN_ETHERNET_HEADER;
}

/***********************************************************************************
 * FUNCTION NAME    : userFileGetPacket
 * DESCRIPTION      : Reads one packet from file
 * PARAMATERS       : [IN] FILE* - file descriptor
 *                    [OUT] UInt8* buf - buffer where packet`s data will be stored
 *                    [IN] UInt32 len_buf - length of input data
 * RETURN VALUE     : None
 ***********************************************************************************/
void userFileGetPacket(FILE* fd, UInt8* buf, UInt32 len_buf)
{
    SInt32 fp;

    assert(fd);
    assert(buf);    

    fp = ftell(fd);

    /*skip ethernet header of packet*/
    fp += LEN_ETHERNET_HEADER;
    fseek(fd, fp, SEEK_SET);

    /*read packet*/
    if(fread(buf, 1, len_buf, fd) != len_buf) {
        assert(0);
    }
}

/****************************************************************************
 * Fuctionn Name  : SendUeDataToAdapter
 * Inputs         : trafficGenIndex
 * Outputs        :
 * Returns        : void
 * Description    : function to send data to RLC for a Ue and LcId pair
 ****************************************************************************/
void SendUeDataToAdapter (UInt64     index)
{
    SInt32      ueIndex          = -1; 
    UInt32      DataToPump       = 0;
    UInt64      UeId             = 0;
    SInt8       LcId             = 0;
    UInt64      count            = 0;
    UeData  *ueData = PNULL;
    UInt8 data_p[2500] = {0};
    FILE   * infoFile_p    = NULL;
    UInt16 rbDirection  = 0;
    UInt32 i= 0,loopCount = packetCount_g, current = 0;
    count = index;
    UeId  = (UInt64) trafficGenRateForData_g[count].UeId;
    LcId  = trafficGenRateForData_g[count].LcId; 
    DataToPump = trafficGenRateForData_g[count].DataToPump;

    /* PCAP file Read */
    if(PcapFileReadOnceFlag_g == 1)
    {
        infoFile_p = fopen("../cfg/PcapFile.pcap","rb");

        if (infoFile_p == NULL)
        {
            lteWarning("Pcap File Not Found. - SendUeDataToAdapter()");
            pcapPacketDataLength_g = DataToPump;
        }
        else
        {
            pcapPacketDataLength_g  = userFileGetPacketSize(infoFile_p);
            if(pcapPacketDataLength_g)
                userFileGetPacket(infoFile_p, data_p, pcapPacketDataLength_g);
        }
        memcpy(PcapPacketDataRead_g + HEADER_LEN, data_p, pcapPacketDataLength_g);
        PcapFileReadOnceFlag_g = 0;
    }

    ueIndex = ueCheckTable_g[UeId][LcId][rbDirection];
    if (ueIndex == -1)
        return;

    trafficGenRateForData_g[count].ueIndex = ueIndex;

    ueData = indexData_g[trafficGenRateForData_g[count].ueIndex];
    if (ueData == NULL)
    {
        trafficGenRateForData_g[count].ueIndex = -1;
        return;
    }

    LTE_SET_U16BIT(&PcapPacketDataRead_g[current],ueIndex);
    current +=2;
    PcapPacketDataRead_g[current++] = LcId;
    LTE_SET_U16BIT(&PcapPacketDataRead_g[current],pcapPacketDataLength_g);
    current +=2;

    //fprintf(stderr,"Ready to send data to socket for ueIdx = %d, lcId = %d, NumOfBytes = %d\n", ueIndex, LcId, pcapPacketDataLength_g);
    for(i=0;i<loopCount;i++)
        send_to_socket(PcapPacketDataRead_g, pcapPacketDataLength_g + HEADER_LEN);
}

/****************************************************************************
 * Fuctionn Name  : SendUeDataToAdapter_NP
 * Inputs         : trafficGenIndex
 * Outputs        :
 * Returns        : void
 * Description    : function to send data to RLC for a Ue and LcId pair
 ****************************************************************************/
void SendUeDataToAdapter_NP (UInt64     trafficGenIndex)
{
    SInt32      ueIndex          = -1; 
    UInt32      DataToPump       = 0;
    UInt64      UeId             = 0;
    SInt8       LcId             = 0;
    UInt64      count            = 0;
    UeData  *ueData = PNULL;
    UInt8 data_p[2500] = {0};
    FILE   * infoFile_p    = NULL;
    UInt16 rbDirection = 0;
    UInt32 i= 0, current =0;
    //UInt32 idx = 0;
    
    count = trafficGenIndex;
    UeId  = (UInt64) trafficGenRate_NPg[count].UeId;
    LcId  = trafficGenRate_NPg[count].LcId; 
    DataToPump = trafficGenRate_NPg[count].DataToPump;
    //for ( idx = 0; idx < DataToPump; idx++ )
       // data_p[idx] = idx + 1;
    /* PCAP file Read */
    if(PcapFileReadOnceFlag_g == 1)
    {
        infoFile_p = fopen("../cfg/PcapFile.pcap","rb");

        if (infoFile_p == NULL)
        {
            lteWarning("Pcap File Not Found. - SendUeDataToAdapter_NP(), datatoPump = %d\n", DataToPump);
            pcapPacketDataLength_g = DataToPump;
        }
        else
        {
            pcapPacketDataLength_g  = userFileGetPacketSize(infoFile_p);
            if(pcapPacketDataLength_g)
                userFileGetPacket(infoFile_p, data_p, pcapPacketDataLength_g);
        }
        memcpy(PcapPacketDataRead_g + HEADER_LEN, data_p, pcapPacketDataLength_g);
        PcapFileReadOnceFlag_g = 0;
    }

    ueIndex = ueCheckTable_g[UeId][LcId][rbDirection];
    if (ueIndex == -1)
        return;

    trafficGenRate_NPg[count].ueIndex = ueIndex;

    ueData = indexData_g[trafficGenRate_NPg[count].ueIndex];
    if (ueData == NULL)
    {
        trafficGenRate_NPg[count].ueIndex = -1;
        return;
    }

    LTE_SET_U16BIT(&PcapPacketDataRead_g[current],ueIndex);
    current +=2;
    PcapPacketDataRead_g[current++] = LcId;
    LTE_SET_U16BIT(&PcapPacketDataRead_g[current],pcapPacketDataLength_g);
    current +=2;

    //fprintf(stderr,"Ready to send data from NP to socket for ueIndex = %d, lcId = %d\n", ueIndex, LcId);
    for(i=0;i<packetCount_g;i++)
        send_to_socket(PcapPacketDataRead_g, pcapPacketDataLength_g + HEADER_LEN);
}
