/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTrafficGenEntities.c,v 1.3 2010/03/11 13:41:53 gur19479 Exp $
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
 * $Log: lteTrafficGenEntities.c,v $
 * Revision 1.3  2010/03/11 13:41:53  gur19479
 * core binding changes.
 *
 * Revision 1.2  2010/02/18 07:31:49  gur19479
 * updated for endian-ness
 *
 * Revision 1.1.2.10.2.2  2009/12/05 13:02:53  gur19479
 * updated for thread binding
 *
 * Revision 1.1.2.10.2.1  2009/09/24 21:39:13  gur19479
 * updated for audio and video processing
 *
 * Revision 1.1.2.9  2009/08/17 06:52:27  gur20435
 * Changed binding info
 *
 * Revision 1.1.2.8  2009/08/14 15:30:45  gur19140
 * eNBSim 14 aug delivery changes
 *
 * Revision 1.1.2.7  2009/08/12 08:23:55  gur19836
 * Graceful shutdown of traffic generator code added
 *
 * Revision 1.1.2.6  2009/08/11 08:01:21  gur20435
 * Changed core affinity of traffic gen thread to 0
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
 * Revision 1.1.2.2  2009/07/13 07:54:56  gur19479
 * updated for traffic gen
 *
 * Revision 1.1.2.1  2009/07/12 09:33:36  gur19479
 * added files to make traffic gen
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTrafficGenEntities.h"
#include "lteTrafficGenExtInterface.h"
#include "lteRrcInterface.h"
#include "lteLayer2CommanTypes.h"

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

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* Init Traffic generator flag */
UInt8 initializeTrafficGen_g = 0;

/* Start/Stop Traffic generator flag */
UInt8 startTrafficGenProcessing_g = 0;
/* Start/Stop Non Periodic traffic flag */
UInt8 startNonPeriodicTraffic_g = 0;
/* Traffic Generator Thread */
ULong32 trafficGenThread_g; 
ULong32 timerTrafficGenThread_g; 
extern SInt32    ueCheckTable_g[MAX_NUM_OF_USERS][MAX_NUM_LOGICAL_CHANNELS][RB_DIR_SUPPORTED];
extern UeData *indexData_g[1000];
extern void initRrcFuncPtr();

extern SInt8 TrafficGenAdapterSocket_g;
extern SInt8 trafficGTxSocket_g;
extern SInt8 TrafficGenOamRrcSocket_g;
extern UInt8 dataProfilesFlag_g;
extern trafficGenAudioVideoData trafficGenAudioVideo_g[MAX_UE][MAX_LC];
extern UInt32 coreNum1,coreNum2;
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
extern UInt32 executeTrafficGenConfigFile(void);
extern UInt32 parseTrafficGenInfoPerUeNonPeriodic(void);
extern UInt32 parseTrafficGenInfoPerUe(void);
/****************************************************************************
 * Function Name  : initTrafficGen
 * Inputs         : None 
 * Outputs        : None
 * Returns        : Success on completion
 * Variables      : 
 * Description    : This function initializes the Traffic Generator.
 ****************************************************************************/
UInt16 initTrafficGen()
{
    LTE_LOG( LOG_DETAIL,PNULL, "In Function - init Traffic Gen() ");  
    UInt32    retVal      = TRAFFIC_GEN_SUCCESS ;
    UInt16 ueId = 0, lcId =0, rbDirection = 0;

    executeTrafficGenConfigFile();
    memset(trafficGenAudioVideo_g,0,sizeof(trafficGenAudioVideo_g));
    retVal = parseTrafficGenInfoPerUe();
    if (retVal == TRAFFIC_GEN_FAILURE)
    {
        ltePanic("Failed reading UE config file");
        return TRAFFIC_GEN_FAILURE;
    }

   if (dataProfilesFlag_g)
   {
        /* read UE Queueload information generation data from file */
        retVal = parseTrafficGenInfoPerUeNonPeriodic();
        if (retVal == TRAFFIC_GEN_FAILURE)
        {               
            ltePanic("Failed reading Non Periodic UE Queueload file");
            return TRAFFIC_GEN_FAILURE;
        }
        memset(indexData_g,0,sizeof(UeData*) *1000);
        for ( ueId = 0; ueId <= 310 ; ueId++)
            for (lcId = 0; lcId<=10;lcId++)
                for(rbDirection = 0; rbDirection < 2; rbDirection++)
                    ueCheckTable_g[ueId][lcId][rbDirection] = -1;
        /* Spawn thread that generates UE data load on expiry of timer. */
        if (!trafficGenTimer_g)
           trafficGenTimer_g = 1;
        t1_g.tv_sec = 0;
        t1_g.tv_nsec = (trafficGenTimer_g * 1000000);
        LTE_LOG(LOG_DETAIL,PNULL,"t1_g.tv_sec = %d", t1_g.tv_sec);
        LTE_LOG(LOG_DETAIL,PNULL, "t1_g.tv_nsec = %d", t1_g.tv_nsec);
        LTE_LOG(LOG_DETAIL,PNULL, "Exit - initTrafficGen() ");
   }
   initRrcFuncPtr();
   return retVal;  

} /* end init Traffic Gen */


/****************************************************************************
 * Function Name  : createTrafficGenEntity
 * Inputs         : None 
 * Outputs        : None
 * Returns        : Success on completion
 * Variables      : 
 * Description    : This function initializes the downlink entity of RLC layer.
 ****************************************************************************/

static void uSleep(UInt32 usec)
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = usec;
  select(0,NULL,NULL,NULL,&tv);
}

/****************************************************************************
 * Function Name  : void timerTrafficGenThread_cleanup_handler(void *arg)
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function will be called when timerTrafficGenThread
 *                  is cancelled.
 ****************************************************************************/
void timerTrafficGenThread_cleanup_handler(void *arg)
{
    //fprintf(stderr, "In TG Timer Thread - cleanup handler\n");
}

    /* SPR 15909 fix start */
extern tickType_t trafficGenTick_g;
    /* SPR 15909 fix end */
void * timerTrafficGenThread (void *arg)
{
    threadSetCancelState(THREAD_CANCEL_ENABLE, PNULL);
    threadSetCancelType(THREAD_CANCEL_ASYNCHRONOUS, PNULL);

    threadCleanupPush(&timerTrafficGenThread_cleanup_handler, PNULL);
#if 0
    do
    {
      uSleep(1000);        
      trafficGenTick_g++;
    }while(1);
#endif    
    while(initializeTrafficGen_g)
    {
      uSleep(1000);        
      trafficGenTick_g++;
    }
    
    threadCleanupPop(1);
    return PNULL;
}


UInt16 createTrafficGenEntity()
{ 
  /* begin createTrafficGenEntity */
    UInt32 retVal         = TRAFFIC_GEN_SUCCESS;
    /* allocate space to memory pool */
    createMemPool( (UInt32) MEM_POOL_SIZE_UE_DATA, /* Size of one block of data */
            MAX_UE );                      /* Number of such blocks to be allocated */

    createMemPool( (UInt32) SIZE_OF_PDU, /* Size of one block of data */
            MAX_UE * 8);  

    /* Spawn Traffic Gen  thread */
    retVal = threadCreate(&trafficGenThread_g, THREAD_CREATE_DETACHED,
        THREAD_SCHED_FIFO,0,
        THREAD_EXPLICIT_SCHED,
        THREAD_SCOPE_SYSTEM, trafficGenThread, PNULL);
    if (retVal)
    {
        ltePanic("Error creating trafficGenThread_g in Fn createTrafficGenEntity()");
        return TRAFFIC_GEN_FAILURE;         
    }

    retVal = threadCreate(&timerTrafficGenThread_g, THREAD_CREATE_DETACHED,
        THREAD_SCHED_FIFO,0,
        THREAD_EXPLICIT_SCHED,
        THREAD_SCOPE_SYSTEM, timerTrafficGenThread, PNULL);

    if (retVal)
    {
        ltePanic("Error creating timertrafficGenThread_g in Fn createTrafficGenEntity()");
        return TRAFFIC_GEN_FAILURE;         
    }
    THREAD_TO_BIND_SET_CORE_NO(trafficGenThread_g,coreNum1);
    THREAD_TO_BIND_SET_CORE_NO(timerTrafficGenThread_g,coreNum2);
#ifdef THREAD_BIND_ENABLE
    pid_t p = 0;
    THREAD_TO_BIND_SET_CORE_NO(p,0);
    THREAD_TO_BIND_SET_CORE_NO(p,0);
#endif

    LTE_LOG( LOG_DETAIL,PNULL,"In Function - createTrafficGenEntity() ");

    return TRAFFIC_GEN_SUCCESS;
} /* end createTrafficGenEntity */


/****************************************************************************
 * Function Name  : cleanUpTrafficGen
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This function clean up all memory and threads spawned by RLC
 ****************************************************************************/
void cleanUpTrafficGen()
{ 
    /* begin cleanUpTrafficGen */
    LTE_LOG( LOG_DETAIL,PNULL,"In Function - cleanUpTrafficGen \n");
    UInt32 retTh = 0;
#if 0
    if(trafficGenThread_g) {
        retTh = threadCancel(trafficGenThread_g);
        trafficGenThread_g = 0;
    }    
#endif   
    //initializeTrafficGen_g = 0;
    if(timerTrafficGenThread_g) {
        retTh = threadCancel(timerTrafficGenThread_g);
        timerTrafficGenThread_g = 0;
    }   
    
    startTrafficGenProcessing_g = 0;
    startNonPeriodicTraffic_g = 0;
    if(TrafficGenAdapterSocket_g) {
        close(TrafficGenAdapterSocket_g);
        TrafficGenAdapterSocket_g = 0;
    }

    if(trafficGTxSocket_g) {
        close(trafficGTxSocket_g);
        trafficGTxSocket_g = 0;
    }

    if(TrafficGenOamRrcSocket_g) {
        close(TrafficGenOamRrcSocket_g);
        TrafficGenOamRrcSocket_g = 0;
    }

    cleanupMemPool();

    LTE_LOG( LOG_DETAIL,PNULL,"Exit - cleanUpTrafficGen\n");
    exit(0);
} /* end cleanUpTrafficGen */
