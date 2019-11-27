/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacExecutionScheduler.c,v 1.4.6.1.4.2.2.4 2010/10/25 09:36:33 gur23971 Exp $
 *
 ******************************************************************************
 *
 *  File Description : 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacExecutionScheduler.c,v $
 * Revision 1.4.6.1.4.2.2.4  2010/10/25 09:36:33  gur23971
 * warning removed
 *
 * Revision 1.4.6.1.4.2.2.3  2010/09/29 05:09:25  gur04640
 * Thread bind moved to start - removed from execution thread
 *
 * Revision 1.4.6.1.4.2.2.2  2010/09/27 14:37:29  gur10694
 * UE config Fix and Review comments
 *
 * Revision 1.4.6.1.4.2.2.1  2010/09/21 15:46:05  gur20491
 * FAPI changes
 *
 * Revision 1.4.6.1.2.2  2010/08/16 04:02:56  gur19836
 * Integration Issues Fixed
 *
 * Revision 1.4.6.1.2.1  2010/08/11 12:05:06  gur19836
 * Compilation Issues resolved
 *
 * Revision 1.4.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.22.26.1  2010/07/21 09:27:17  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. Changed thread scheduling poilcy to Real Time First In First Out(SCHED_FIFO) from default policy for MAC threads.
 *
 * Revision 1.22  2009/11/29 09:20:07  gur11912
 * modified legCount to 0 in printTimingStats
 *
 * Revision 1.21  2009/11/25 12:45:55  gur11912
 * modified printTimingStats functionality
 *
 * Revision 1.20  2009/11/25 09:24:19  gur11912
 * used peakTimeInMicroSec instead of avgTimeInMicroSec
 *
 * Revision 1.19  2009/11/12 12:46:52  gur19413
 * added UT_TESTING FLAG
 *
 * Revision 1.18  2009/08/30 19:43:33  gur19413
 * Warnings removed
 *
 * Revision 1.17  2009/08/28 12:52:23  gur19413
 * process all subframe in the msg queue.
 *
 * Revision 1.16  2009/08/18 16:19:00  gur11912
 * modified for thread synchronization
 *
 * Revision 1.15  2009/07/31 06:30:44  gur15697
 * cleanup Bugfix
 *
 * Revision 1.14  2009/07/24 06:54:13  gur19413
 * added new func call processOnlySubFrameInd.
 *
 * Revision 1.13  2009/07/16 14:03:34  gur18569
 * changes for waiting for tick logic
 *
 * Revision 1.12  2009/07/15 04:32:35  gur18569
 * changed log file name
 *
 * Revision 1.11  2009/07/10 12:07:12  gur18569
 * removed compilation warnings
 *
 * Revision 1.10  2009/07/02 14:37:50  gur18569
 * change in log file name
 *
 * Revision 1.9  2009/06/30 14:10:27  gur18569
 * enabled shm logging registration
 *
 * Revision 1.7  2009/06/18 07:50:32  gur18569
 * init sem pdcchTriggerToPuschSem_g and dlControlMsgSentTriggerSem_g to 0
 *
 * Revision 1.6  2009/06/17 06:30:24  gur18569
 * waiting for cell conf before calling legs
 *
 * Revision 1.5  2009/06/15 09:42:03  gur18569
 * changes in test exec flow for testing
 *
 * Revision 1.4  2009/06/10 12:20:31  gur18569
 * removed linking error
 *
 * Revision 1.3  2009/06/10 11:28:38  gur18569
 * incorporation of comments
 *
 * Revision 1.2  2009/06/03 10:42:45  gur18569
 * incorporated comments
 *
 * Revision 1.1  2009/05/29 14:26:27  gur18569
 * Initial version
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacExecutionScheduler.h"
#include "lteMacExecutionLegs.h"
#include "lteMacCellConfMgr.h"
#include "lteMacComPorting.h"
#include "lteMacReceiver.h"
/* SPR 19288 change start */
#include "lteMacSFNMgr.h"

#ifdef FLEXRAN
#include "lteMacFAPIApi.h"
#endif
/* SPR 19288 change end */
#include "lteShmLogger.h"
#include "lteMacParseOAMMsg.h"
#include "lteCommonStatsManager.h" 
/* SPR 21804 Fix Start */
#if defined( LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
#include "lteLayer2DLTG.h"
#endif
#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"
#endif
#ifdef FLEXRAN
#include <sys/prctl.h>
#endif

/*stores thread specific arguments for thread creation in for loop for each cell*/
#ifdef DL_UL_SPLIT
ExecutionFlowAndCellIndex execSchedulerParams[MAX_NUM_CELL][NUM_OF_EXEC_FLOWS];
/** CA 2 Core Changes Start **/
#else
ExecutionFlowAndCellIndex execSchedulerParams[MAX_NUM_CELL];
/** CA 2 Core Changes End **/
#endif
/******************************************************************************
 Private Definitions
 *****************************************************************************/

#ifdef FLEXRAN
#ifdef FLEXRAN_FAPI_LOGGING
extern SInt32 flexranFapiLoggingEnable_g;
#endif
extern UInt32 g_flexran_channelId[MAX_NUM_CELL] ; 
extern SInt32 flexranWiresharkFd_g [MAX_NUM_CELL] ;
extern SockAddr flexranTx_g[MAX_NUM_CELL];
extern void do_tap_fun(void* data, uint32_t len, InternalCellIndex cellId);
extern SInt32 flexranFapiLoggingEnable_g;
#endif

/*FAPI L2 AS MASTER START */
/* SPR 19288 change start */
/* SPR 19288 change end */
#ifdef FAPI_L2_MASTER
/*SPR 13047 fix*/
UInt8 firstTickRecvFlag_g[MAX_NUM_CELL] = {0};
extern UInt8 startExecutionAfterPhyStart_g[MAX_NUM_CELL];
#endif
/*FAPI L2 AS MASTER END */
/* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
extern LTE_CIRCQUEUE rlcEmbmsLpToHpQ_g;
extern void rlcProcessEmbmsHpHandler(LTE_CIRCQUEUE *rlcEmbmsReqRespQ_p);
#endif
/* eMBMS Code Changes End */
/* SPR 19288 change start */
/* SPR 19288 change end */
/* CA Changes start */
/* SPR 8712 changes */
extern MAC_UE_OPPOR_IND_CIRC_TYPE* macUeOpporIndCircQ_gp[MAX_NUM_CELL];
/* SPR 8712 changes */
/* Represent PHY state at MAC */
/* SPR 5147 changes start */

/* This is used for cell level global tick parameter */
/* + SPR_14080 */
__thread InternalCellIndex cellIndexMac_g = 0;
/* - SPR_14080 */
/*SPR 16855 +-*/
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
SEM_T waitForCellSetupIndSemArr_g[NUM_OF_EXEC_FLOWS * MAX_NUM_CELL];
#endif
#ifdef TDD_CONFIG 
extern void initUlDLExecutionFlow(InternalCellIndex internalCellIndex);
extern void initMacTDDConfig(InternalCellIndex internalCellIndex);
/* CA Changes end */
extern void assignTddPdschTxMapOfDLHarqPointer(UInt8 ulDlConfiguration, InternalCellIndex internalCellIndex);
#endif

/* CA Changes start */
#ifndef FLEXRAN
extern SInt32 phyRxSockFD_g[MAX_NUM_CELL];
#endif
/* CA Changes end */
/*SPR 16855 +-*/
/* SPR 21804 Fix Start */
#if defined( LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
extern  ULong32  dlTgExecutionSchedulerThId_g;
extern  void * dlTgThreadExecutionSchedular(void * param);
#endif
/* + SPR 17439 */
extern MacRetType startPhy(void);
/* - SPR 17439 */
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

#ifdef DL_DATA_SEPARATION
extern SEM_T waitForPdschInd_g[MAX_NUM_CELL];
void* encoderSchedulerFlowThread(void * executionFlowThreadArgs_p,
                         InternalCellIndex internalCellIndex);
/* CA Stats Changes Start */
extern void lteMacUpdateEncoderStatsPerfEnd(InternalCellIndex internalCellIndex);
/* CA Stats Changes End */
/* SPR 8712 changes */
SEM_T waitForCellSetupIndSemEncThrd[MAX_NUM_CELL];
/* SPR 8712 changes */
void* encoderScheduler(void *param);
#endif
/*CA Changes start  */
extern LTE_SQUEUE* recvDataFromPhyQ_gp[MAX_NUM_CELL];
extern MacRetType sendStartReqToPhy(UInt8 internalCellIndex);
extern UInt8 * receiveDataFromPhy(SInt32 *bytesRead_output, 
                                  InternalCellIndex internalCellIndex);
/*CA Changes end  */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
sem_t waitForCellSetupIndSemArr_g[NUM_OF_EXEC_FLOWS];
MacRetType createExecutionFlowThreads( ExecutionFlowCreationParam *
                                                    executionFlowThreadParamArr_p);
#endif
#ifdef DL_UL_SPLIT_TDD
extern ExecutionFlowCreationParamRlc executionFlowCreationParamRlc_g ;
#endif

/*SPR 16855 +-*/
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
#define NUM_OF_THREADS ( NUM_OF_EXEC_FLOWS + 1 )                      /* +1 for RRC/OAM thread*/ 
/* SPR 15640 fix start */
//#define RRC_OAM_FLOW_ID (NUM_OF_THREADS - 1)
/* SPR 15640 fix end */
#endif
#ifdef FDD_CONFIG

#ifdef OVERSHOOT_CORRECTION
#define STABLE_LOAD_TIME 5
#define OAM_REPORTING_TIME 20
#endif
/* SPR 19288 change start */
/* SPR 19288 change end */
/******************************************************************************
 Private Types
 *****************************************************************************/

#define EXE_SCH  "Exec_Scheduler"
/*rel4-drop2 changes*/
/*SPR 16855 +-*/
#ifndef DL_UL_SPLIT
ULong32 callFlowExecutionSchedulerThId_g;
#endif
/*rel4-drop2 changes*/

#define RLC_FAILURE 0
extern UInt16 pdcpContextIsInitialized(void);
extern UInt16 rlcLayerInitialized_g;
/* RLC Queues for Pending Deletion and Reestablishment of Entities */
/* SPR 3805 Fix Start */
extern LTE_CIRCQUEUE rlcRxUePendingDelQ_g;
extern LTE_CIRCQUEUE rlcTxUePendingDelQ_g;
/* SPR 3805 Fix End */

/* + FDD_COMPILATION_WARNING */
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
extern LTE_SQUEUE rlcTxPendingDelQ_g;
extern LTE_SQUEUE rlcRxPendingDelQ_g;
extern LTE_SQUEUE rlcTxReEstabQ_g;
extern LTE_SQUEUE rlcRxReEstabQ_g;
extern UInt32 rlcHdlrPenDelQ(LTE_SQUEUE *pendDelQ_p);
/* SPR 3805 Fix Start */
extern UInt32 rlcHdlrPenUeDelQ(LTE_CIRCQUEUE *rlcUePendingDelQ);
/* SPR 3805 Fix Start */
extern UInt32 rlcHdlrReEstabQ(LTE_SQUEUE *reEstabQ_p);
#else
extern LTE_CIRCQUEUE rlcRxPendingDelQ_g;
extern LTE_CIRCQUEUE rlcTxReEstabQ_g[MAX_NUM_CELL];
extern LTE_CIRCQUEUE rlcRxReEstabQ_g;
/** SPR 13293 Changes start **/
extern LTE_CIRCQUEUE rlcTxPendingDelQ_g[MAX_NUM_CELL];
extern UInt32 rlcProcessDelMsg(LTE_CIRCQUEUE *rlcPendingDelQ_p);
/** SPR 13293 Changes end **/
#endif

extern LTE_SQUEUE rlcTmrExTxsQ_g ;
extern LTE_SQUEUE rlcControlPduQ_g ;
extern LTE_SQUEUE macUeHarqFailureIndQ_g;
#ifndef DL_DATA_SEPARATION
/** CA Changes Start : **/
extern UInt32 rlcHandleTxOppurtunity(UInt8 cellIndex);
/** CA Changes Start : End **/
#endif

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
extern SInt32 unlinkMsgQueueForICC(const char *name);
#endif

extern void normalModeForMacExecSchedular( void );
extern void handleUserData(void);

/* CA Changes start */
#ifndef FLEXRAN
SInt32 receiveStartPhyRes(UInt8 *msgBuf,InternalCellIndex internalCellIndex);
#endif
void postCellSetupInd(InternalCellIndex internalCellIndex);
/* CA Changes end */
UInt32 handleStartPhyRes(UInt8 *msgBuf,SInt32 bytesRead);

extern UInt32 rlcCheckTimerExpiry(InternalCellIndex internalCellIndex);
extern void initUeConfigMsgQueue (UInt8 numOfCells);
#ifdef UTFWK_SIMULATION
extern void initueLCinQos();
#endif
/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/

extern UInt32 rlcProcessReEstabMsg(LTE_CIRCQUEUE *reEstabQ_p);
extern MacRetType callExecutionFlowThreads( ExecutionFlowCreationParam *
                                                    executionFlowThreadParamArr_p
/*SPR 18490 +-*/                                                    
#ifndef DL_UL_SPLIT                                                    
                                                    ,ExecutionFlowAndCellIndex execFlowAndindex
#endif                                                    
/*SPR 18490 +-*/                                                    
                                                    );


 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
#ifdef OVERSHOOT_CORRECTION
/** CA2 Changes Start **/
void setOverLoadLevel(OverLoadLevel tickOverLoadLevel,InternalCellIndex internalCellIndex);
/** CA2 Changes End **/
#endif

#ifdef FDD_CONFIG
#ifdef DL_UL_SPLIT
/* CA Changes start */ 
extern SInt32 puschSignalQueueIdDL_g[MAX_NUM_CELL];
extern SInt32 puschSignalQueueIdUL_g[MAX_NUM_CELL];
extern SInt32 ulStrategyTxQueueIdDL_g[MAX_NUM_CELL];
extern SInt32 ulStrategyTxQueueIdUL_g[MAX_NUM_CELL];
extern SInt32 ackNackQueueIdDL_g[MAX_NUM_CELL];
extern SInt32 ackNackQueueIdUL_g[MAX_NUM_CELL];
extern SInt32 macUeOppnIndQIdUL_g[MAX_NUM_CELL];
extern SInt32 macUeOppnIndQIdDL_g[MAX_NUM_CELL];
/*CA Changes end  */

#ifdef CYCLE_COUNT_MAC_RLC_PDCP
__thread UInt64 prof_start_count_split_g[NUM_OF_THREADS], prof_end_count_split_g[NUM_OF_THREADS];
__thread UInt64 prof_cycle_count_used_split_g[NUM_OF_THREADS];
__thread TimeSpec prof_start_clock_split_g[NUM_OF_THREADS],prof_end_clock_split_g[NUM_OF_THREADS];
#endif

/*Change DROP1*/
LTE_SEM waitForDLPerfStatsUpd_g[MAX_NUM_CELL];
/*Change DROP1*/
/* semaphone to serialize unsplit legs.. would be removed after spliting the respective legs */
/*CA Changes start  */
/* +- SPR 17777 */
extern void initDLAckNackQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex);
extern void initUlStrategyQueue( 
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex);
extern void initPUSCHSignalQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
extern void initMACUEOppertunityIndQ(SInt32 * qId, InternalCellIndex cellIndex);
extern void initULContainerNackQueue ( SInt32 *qId, InternalCellIndex cellIndex, UInt8 containerIndex);
extern void initULContainerNackQueueForTempUE( SInt32 *qId, InternalCellIndex cellIndex, UInt8 containerIndex);
/*CA Changes end  */
/* + SPR 17439 */
extern void closeULContainerNackQueue(UInt32 max_num_of_container);
extern void closeEncoderQueue(UInt32 max_encoder_containers);
/* - SPR 17439 */
#endif
#endif
extern void ElprocessOAMAndRRCMsg(UInt8 numOfMsg, UInt32 delayToPhy);
/*Cyclomatic Complexity changes - starts here*/
#ifdef DL_UL_SPLIT
static  void initDlUlThreadParams(ExecutionFlowArguments * threadArg_p);
#endif
/*SPR 16855 +-*/
#ifndef DL_UL_SPLIT
static  void executionWhileStoppedProcessingFlagIsFalse(ExecutionLegFunctionArg *execLegFuncArg,
                                                              ExecutionLegInfo *executionLegArr_p,
                                                              ExecutionFlowArguments *threadArg_p);
STATIC  void checkForRlcTimerExpiry(InternalCellIndex internalCellIndex);
#endif
/*Cyclomatic Complexity changes - ends here*/
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/*Timing stats per Leg*/
ExecutionStatsInfo timingStatsExecLegArr_g[NUM_OF_EXEC_FLOWS][MAX_EXECUTION_LEGS];

/*Timing stats per flow */
ExecutionStatsInfo timingStatsExecFlowArr_g[NUM_OF_EXEC_FLOWS];

/*****************************************************************************
 * Function Name  : siddiffInMicroSec
 * Inputs         : start - timespec recorded at start of function or 
 *                          code whose time is being measured.
 *                  end - timespec recorded at end of funtion or code 
 *                         whose time is being measured.
 * Outputs        : None
 * Returns        : time difference in microseconds.
 * Description    : The function returns difference of Start and End time 
 *                  in microseconds.
 *****************************************************************************/
UInt32 siddiffInMicroSec(TimeSpec *  start, TimeSpec *  end)
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

    return (diffInMicroseconds);
}


/*****************************************************************************
 * Function Name  : printTimingStats
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : The function prints the timing stats per flow and all the
 *                  legs in that flow.
 *****************************************************************************/
/* + SPR 17439 */
void printTimingStats(void)
 /* - SPR 17439 */
{
}

#ifdef DL_UL_SPLIT
/*****************************************************************************
 * Function Name  : executionFlowThread
 * Inputs         : executionFlowThreadArgs_p
 * Outputs        : None
 * Returns        : None
 * Description    : The thread executes execution legs passed to it through
 *                  arguments.
 *****************************************************************************/
void* executionFlowThread(void * executionFlowThreadArgs_p)
{
    ExecutionLegFunctionArg execLegFuncArg = {0}; 
    ExecutionFlowAndCellIndex *execSchedulerParams_p = (ExecutionFlowAndCellIndex *)executionFlowThreadArgs_p;

    ExecutionFlowArguments * threadArg_p =
            &(executionFlowCreationParamArr_g[execSchedulerParams_p->executionFlowCount].threadArguments);
	ExecutionLegInfo * executionLegArr_p = threadArg_p->executionLegArr;
    /*SPR 18490 +-*/
	UInt8 executionLegCount =0;
    /*SPR 18490 +-*/
    /* SPR 15909 fix start */
	tickType_t  currentTick = 0;
    /* SPR 15909 fix end */
    /* Coverity 60275 Fix Start*/
    CellState cellState = CELL_INIT;
    /* Coverity 60275 Fix End*/
	fd_Set  readfds;
	timeVal timeout;
#ifdef EL_TIMESTAMPS
    /* SPR 15816 fix start */
	UInt32 t2t1 = 0;
    /* SPR 15816 fix end */
	TimeSpec t1,t2, tElStart[20],tElEnd[20];
#endif
	UInt8 logger_file_name[LTE_MAX_FILE_NAME_SIZE] = {0};
#ifdef OVERSHOOT_CORRECTION
    UInt64 rem_cycles = 0;
#endif

    threadArg_p->internalCellIndex = execSchedulerParams_p->cellIndex;
    threadArg_p->threadIndex = execSchedulerParams_p->executionFlowCount;

    /*CA Changes start*/
    execLegFuncArg.execFlowIndex = execSchedulerParams_p->executionFlowCount;
    execLegFuncArg.delayToPhy = threadArg_p->dlPathDelay;
    execLegFuncArg.cellIndex = execSchedulerParams_p->cellIndex;

    /* This will be populated with cell index of cell for 
     *  which this thread is getting created.*/
    cellIndexMac_g = execLegFuncArg.cellIndex;

	/* Bind this thread to the core number specified in the structure definition */
    if (INIT_DL_THREAD_PARAMS == threadArg_p->startExecutionLeg)
    {
        THREAD_TO_BIND_SET_CORE_NO(threadSelf(),
                lteLayer2CoreInfo_g.lteLayer2DLCoreNum[execLegFuncArg.cellIndex]);
    }
    else if (INIT_UL_THREAD_PARAMS == threadArg_p->startExecutionLeg) 
    {
        THREAD_TO_BIND_SET_CORE_NO(threadSelf(),
                lteLayer2CoreInfo_g.lteLayer2ULCoreNum[execLegFuncArg.cellIndex]);
    }
    else if (INIT_RLC_PARAMS == threadArg_p->startExecutionLeg)
    {
         THREAD_TO_BIND_SET_CORE_NO(threadSelf(),lteLayer2CoreInfo_g.lteLayer2ULRLCCoreNum);

    }   
    /*CA Changes end*/
    threadSetRtPriority(SCHED_FIFO, executionFlowCreationParamArr_g[execLegFuncArg.execFlowIndex].threadPriority);

    /* Initialize logging for execution flows */
    /*Coverity 26339 Fix Start*/ 
    /* CA Logging Changes Start */
    /* +SPR_10381 Changes */
    if(INIT_RLC_PARAMS == threadArg_p->startExecutionLeg)
    {
        snprintf((Char8 *)logger_file_name, LTE_MAX_FILE_NAME_SIZE-1, "%s%d_", LTE_L2_LOG_REGION_FLOW, threadArg_p->threadIndex);
    }
    else
    {
        prepareLoggerFileName((Char8 *)logger_file_name,LTE_L2_LOG_REGION_FLOW,threadArg_p->threadIndex,LTE_MAX_FILE_NAME_SIZE-1,
                threadArg_p->internalCellIndex);
    }
    /* -SPR_10381 Changes */
    /*Coverity 26339 Fix End*/
    logger_init((LTE_L2_LOG_REGION_MAX+ execLegFuncArg.execFlowIndex), ( Char8*)logger_file_name, l2Input.shmSize);

    startExecSchedLogReader(&l2Input, ( Char8*)logger_file_name);
    /* CA Logging Changes End */
    /*Cyclomatic Complexity changes - starts here*/
    initDlUlThreadParams(threadArg_p);
    /*Cyclomatic Complexity changes - ends here*/
    /* this is the main loop of the thread. This will keep looping till the thread is alive.*/
    while(1)
    {
        /* wait for cell setup indication */
        semWaitDefault(&waitForCellSetupIndSemArr_g[(execLegFuncArg.cellIndex * NUM_OF_EXEC_FLOWS) + 
                execLegFuncArg.execFlowIndex]);
        /* SPR 4915 : Start*/
        /* CA Changes start */
        cellState = cellSpecificParams_g.\
            cellConfigAndInitParams_p[execLegFuncArg.cellIndex]->cellState;
        while(CELL_PHY_RUNNING == cellState || 
                CELL_PHY_RUNNING_CONFIG_RECVD  == cellState)
            /* CA Changes end */
            /* SPR 4915 : End*/
        {
            if(INIT_UL_THREAD_PARAMS == threadArg_p->startExecutionLeg)
            {
                /* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
                /*
                 ** As we have updated MAC tick with L2 tick when we received
                 ** PHY Start message from L1 Layer. Check whether
                 ** startExecutionAfterPhyStart_g is set.
                 */
                if (startExecutionAfterPhyStart_g[execLegFuncArg.cellIndex] == TRUE)
                {
                    startExecutionAfterPhyStart_g[execLegFuncArg.cellIndex] = FALSE;
                }
#endif
                /* FAPI L2 AS MASTER END */

                timeout.tv_sec  = 1;
                timeout.tv_usec = 0;
                lteFD_ZERO(&readfds);
                /* SPR 19288 change start */
                    /* CA Changes start */
                    lteFD_SET(phyRxSockFD_g[execLegFuncArg.cellIndex], &readfds);
                if(0 == select_wrapper(phyRxSockFD_g[execLegFuncArg.cellIndex] + 1, 
                            &readfds, PNULL, PNULL, &timeout))
                    /* CA Changes end */
                    /* SPR 19288 change end */
                {
                    continue;
                }
            }
#ifdef OVERSHOOT_CORRECTION
            currTickOverLoadDL_g = OVERLOAD_LEVEL_NORMAL;
            currTickOverLoadUL_g = OVERLOAD_LEVEL_NORMAL;
            noTimeToProcessflag_g = 0;
#endif

            /* The first leg wait for TICK indication , after 
             * tick reception we get SF and SFN*/
#ifdef EL_TIMESTAMPS
            clock_gettime_real_time(&tElStart[0]);
#endif 
            RDTSC_AND_CLOCK_START_SPLIT_FDD( execLegFuncArg.execFlowIndex,executionLegArr_p[0].executionLeg);
            /* CA Changes start */
            currentTick = GET_CURRENT_TICK_FOR_CELL(execLegFuncArg.cellIndex);
            /* CA Changes end */
            execLegFuncArg.execLegSpecificArg1 = 
                executionLegArr_p[0].legSpecificArg1;

            execLegFuncArg.execLegSpecificArg2 = 
                executionLegArr_p[0].legSpecificArg2;

            /* execute handler for first EL, this will help to get Subframe indication & tick */
            executionFunctionArr_g[executionLegArr_p[0].executionLeg]
                (&execLegFuncArg);

            /* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
            if (INIT_UL_THREAD_PARAMS == threadArg_p->startExecutionLeg)
            {
                /*
                 ** Checks whether new TICK has been received from the system.
                 ** If yes, then process the remaining ELs.
                 */
                if (currentTick != getCurrentTickFrmL2Tick())
                {
                    /*
                     ** Set the currentTick, systemFrame and subframe globals
                     ** for MAC.
                     ** Initialize some other global variables which is required
                     ** before starting ELs execution.
                     */
                    processAndValidateL2TickBeforeELExecution(threadArg_p->internalCellIndex);
                }
                else
                {
                    /* Waiting for the Tick from system */
                    continue;
                }
            }
#endif
            /* FAPI L2 AS MASTER END */

#ifdef EL_TIMESTAMPS
            clock_gettime_real_time(&tElEnd[0]);
#endif 

            if ((INIT_UL_THREAD_PARAMS == threadArg_p->startExecutionLeg) &&  
                    (currentTick == GET_CURRENT_TICK_FOR_CELL(execLegFuncArg.cellIndex)))
                /* CA Changes end */
            {
                continue;
            }
#ifdef CYCLE_COUNT_MAC_RLC_PDCP
            if(INIT_UL_THREAD_PARAMS == threadArg_p->startExecutionLeg)
            {
                /* CA Stats Changes Start */
                lteIncrementL2Cycles(execLegFuncArg.cellIndex);
                /* CA Stats Changes End */
            }
#endif 
            /* CA Changes start */
            execLegFuncArg.globalTick =  GET_CURRENT_TICK_FOR_CELL(execLegFuncArg.cellIndex);
            getSFAndSFN(&execLegFuncArg.sysFrame, &execLegFuncArg.subFrame,
                    execLegFuncArg.cellIndex);
            /* CA Changes end */
            /* CA Stats Changes Start */
            RDTSC_AND_CLOCK_END_SPLIT_FDD( execLegFuncArg.execFlowIndex,executionLegArr_p[0].executionLeg, 
                    execLegFuncArg.cellIndex);
            /* CA Stats Changes End */

            executionLegCount = 1;

#ifdef EL_TIMESTAMPS
            clock_gettime_real_time(&t1);
#endif 
            /*Execute Mandatory ELs*/
#ifdef OVERSHOOT_CORRECTION
            while(executionLegCount <  threadArg_p->mandatoryExecutionLegCount)
#else
                while(executionLegCount <  threadArg_p->executionLegCount)
#endif /*OVERSHOOT_CORRECTION*/
                {
#ifdef EL_TIMESTAMPS
                    clock_gettime_real_time(&tElStart[executionLegCount]);
#endif 
                    RDTSC_AND_CLOCK_START_SPLIT_FDD( execLegFuncArg.execFlowIndex,executionLegArr_p[executionLegCount].executionLeg);
                    execLegFuncArg.execLegSpecificArg1 = 
                        executionLegArr_p[executionLegCount].legSpecificArg1;

                    execLegFuncArg.execLegSpecificArg2 = 
                        executionLegArr_p[executionLegCount].legSpecificArg2;

                    executionFunctionArr_g[executionLegArr_p[executionLegCount]
                        .executionLeg] (&execLegFuncArg);
                    /* CA Stats Changes Start */
                    RDTSC_AND_CLOCK_END_SPLIT_FDD( execLegFuncArg.execFlowIndex,executionLegArr_p[executionLegCount].executionLeg,
                            execLegFuncArg.cellIndex);
                    /* CA Stats Changes End */
#ifdef EL_TIMESTAMPS
                    clock_gettime_real_time(&tElEnd[executionLegCount]);
#endif 
                    executionLegCount++;
                }


            /*Execute Optional ELs*/
#ifdef OVERSHOOT_CORRECTION
            /*DL Thread*/
            if (threadArg_p->startExecutionLeg == INIT_DL_THREAD_PARAMS)
            {
                /*SPR 18490 +-*/
                while(executionLegCount <  (UInt8)(threadArg_p->executionLegCount - 1))
                    /*SPR 18490 +-*/
                { 
                    GET_PROCESSING_CYCLES_LEFT(rem_cycles);
                    if (( !noTimeToProcessflag_g ) && 
                            (rem_cycles >= reqElProcessingCyclesDL[ueSchdIndxInCurrTick_g][executionLegCount]))
                    { 
#ifdef EL_TIMESTAMPS
                        clock_gettime_real_time(&tElStart[executionLegCount]);
#endif 
                        RDTSC_AND_CLOCK_START_SPLIT_FDD( execLegFuncArg.execFlowIndex,executionLegArr_p[executionLegCount].executionLeg);
                        execLegFuncArg.execLegSpecificArg1 = 
                            executionLegArr_p[executionLegCount].legSpecificArg1;

                        execLegFuncArg.execLegSpecificArg2 = 
                            executionLegArr_p[executionLegCount].legSpecificArg2;

                        executionFunctionArr_g[executionLegArr_p[executionLegCount]
                            .executionLeg] (&execLegFuncArg);
                        /* CA Stats Changes Start */
                        RDTSC_AND_CLOCK_END_SPLIT_FDD( execLegFuncArg.execFlowIndex,executionLegArr_p[executionLegCount].executionLeg,
                                execLegFuncArg.cellIndex);
                        /* CA Stats Changes End */
#ifdef EL_TIMESTAMPS
                        clock_gettime_real_time(&tElEnd[executionLegCount]);
#endif 
                    }
                    else
                    {
                        /*Set the overload level*/
                        currTickOverLoadDL_g = OVERLOAD_LEVEL_MEDIUM;
                        LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                                L2_SYS_FAIL,
                                /* CA Changes start */
                                GET_CURRENT_TICK_FOR_CELL(execLegFuncArg.cellIndex),
                                /* CA Changes end */
                                __LINE__,
                                currTickOverLoadDL_g,
                                /** CA-TDD Changes Start **/
                                macOverLoadLevel_g[execLegFuncArg.cellIndex],
                                /** CA-TDD Changes Stop **/
                                ueSchdIndxInCurrTick_g, 0, 0, 0 ,__func__,
                                "Overload level changed.");
                        break;
                    }
                    executionLegCount++;
                }

            }
            /*UL Thread*/
            if (threadArg_p->startExecutionLeg == INIT_UL_THREAD_PARAMS)
            {
                /*SPR 18490 +-*/
                while(executionLegCount <  (UInt8)(threadArg_p->executionLegCount - 1))
                    /*SPR 18490 +-*/
                { 
                    GET_PROCESSING_CYCLES_LEFT(rem_cycles);
                    if (( !noTimeToProcessflag_g ) && 
                            (rem_cycles >= reqElProcessingCyclesUL[ueSchdIndxInCurrTick_g][executionLegCount]))
                    {                    
#ifdef EL_TIMESTAMPS
                        clock_gettime_real_time(&tElStart[executionLegCount]);
#endif 
                        RDTSC_AND_CLOCK_START_SPLIT_FDD( execLegFuncArg.execFlowIndex,executionLegArr_p[executionLegCount].executionLeg);
                        execLegFuncArg.execLegSpecificArg1 = 
                            executionLegArr_p[executionLegCount].legSpecificArg1;

                        execLegFuncArg.execLegSpecificArg2 = 
                            executionLegArr_p[executionLegCount].legSpecificArg2;

                        executionFunctionArr_g[executionLegArr_p[executionLegCount]
                            .executionLeg] (&execLegFuncArg);
                        /* CA Stats Changes Start */
                        RDTSC_AND_CLOCK_END_SPLIT_FDD( execLegFuncArg.execFlowIndex,executionLegArr_p[executionLegCount].executionLeg,
                                execLegFuncArg.cellIndex);
                        /* CA Stats Changes End */
#ifdef EL_TIMESTAMPS
                        clock_gettime_real_time(&tElEnd[executionLegCount]);
#endif 
                    }
                    else
                    {
                        /*Set the overload level*/
                        if(currTickOverLoadUL_g != OVERLOAD_LEVEL_CRITICAL)
                        {
                            if (executionLegCount == 6)
                            {/*Demux*/
                                currTickOverLoadUL_g = OVERLOAD_LEVEL_HIGH;

                                LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                                        L2_SYS_FAIL,
                                        /* CA Changes start */
                                        GET_CURRENT_TICK_FOR_CELL(execLegFuncArg.cellIndex),
                                        /* CA Changes end */
                                        __LINE__,
                                        currTickOverLoadUL_g,
                                        /** CA-TDD Changes Start **/
                                        macOverLoadLevel_g[execLegFuncArg.cellIndex],
                                        /** CA-TDD Changes Stop **/
                                        ueSchdIndxInCurrTick_g, 0, 0, 0 ,__func__,
                                        "Overload level changed.");
                            }
                            else
                            {
                                currTickOverLoadUL_g = OVERLOAD_LEVEL_MEDIUM;
                                LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                                        L2_SYS_FAIL,
                                        /* CA Changes start */
                                        GET_CURRENT_TICK_FOR_CELL(execLegFuncArg.cellIndex),
                                        /* CA Changes end */
                                        __LINE__,
                                        currTickOverLoadUL_g,
                                        /** CA-TDD Changes Start **/
                                        macOverLoadLevel_g[execLegFuncArg.cellIndex],
                                        /** CA-TDD Changes Stop **/
                                        ueSchdIndxInCurrTick_g, 0, 0, 0 ,__func__,
                                        "Overload level changed.");
                            }
                        }
                        break;
                    }
                    executionLegCount++;
                }
            }

            /*Execute Last EL of DL and UL threads*/
            if((threadArg_p->startExecutionLeg == INIT_UL_THREAD_PARAMS) ||
                    (threadArg_p->startExecutionLeg == INIT_DL_THREAD_PARAMS))
            {
#ifdef EL_TIMESTAMPS
                clock_gettime_real_time(&tElStart[threadArg_p->executionLegCount - 1]);
#endif 
                RDTSC_AND_CLOCK_START_SPLIT_FDD( execLegFuncArg.execFlowIndex,executionLegArr_p[threadArg_p->executionLegCount - 1].executionLeg);
                execLegFuncArg.execLegSpecificArg1 = 
                    executionLegArr_p[threadArg_p->executionLegCount - 1].legSpecificArg1;

                execLegFuncArg.execLegSpecificArg2 = 
                    executionLegArr_p[threadArg_p->executionLegCount - 1].legSpecificArg2;

                executionFunctionArr_g[executionLegArr_p[threadArg_p->executionLegCount - 1]
                    .executionLeg] (&execLegFuncArg);
                /* CA Stats Changes Start */
                RDTSC_AND_CLOCK_END_SPLIT_FDD( execLegFuncArg.execFlowIndex,executionLegArr_p[threadArg_p->executionLegCount - 1].executionLeg,
                        execLegFuncArg.cellIndex);
                /* CA Stats Changes End */


#ifdef EL_TIMESTAMPS
                clock_gettime_real_time(&tElEnd[threadArg_p->executionLegCount - 1]);
#endif 
            }
#endif /*OVERSHOOT_CORRECTION*/

#ifdef EL_TIMESTAMPS
            clock_gettime_real_time(&t2);
            t2t1 = siddiffInMicroSec(&t1, &t2);
            if(t2t1 >= 1000)
            {
                UInt32 diff = 0, count = 1, diffTick = 0; 
                fprintf( stderr, "Tick,UL-DL, EL number, EL start time (sec), EL start time (usec), "\
                        "Total time, EL end time (sec),	EL end time (usec), Difference from "\
                        "tick, Tick time (sec), Tick time(usec)\n");

                for(count = 0; count<threadArg_p->executionLegCount; count++)
                {
                    diff =  siddiffInMicroSec(&tElStart[count], &tElEnd[count]);
                    /* CA Stats Changes Start */
                    diffTick =  siddiffInMicroSec(&(gMacStats.pStats->perfStatsSplit[
                                execLegFuncArg.cellIndex][0].processingStartTime), &tElEnd[count]);
                    /*SPR 15909 Fix Start*/
                    /* SPR 15816 fix start */
                    /* SPR 16593 fix start */
                    /*SPR 19078 +-*/
                    /*GCC warning fix+*/    
                    fprintf( stderr, "%llu, %d, %d, %d, %ld, %d,  %d, %ld, %d, %d, %ld\n",  \
                            GET_CURRENT_TICK_FOR_CELL(execLegFuncArg.cellIndex), execLegFuncArg.execFlowIndex, count,
                            (UInt32)tElStart[count].tv_sec, (tElStart[count].tv_nsec/1000),
                            diff, (UInt32)tElEnd[count].tv_sec, (tElEnd[count].tv_nsec/1000),
                            diffTick, (UInt32)gMacStats.pStats->perfStatsSplit[execLegFuncArg.cellIndex][0].processingStartTime.tv_sec,
                            (gMacStats.pStats->perfStatsSplit[execLegFuncArg.cellIndex][0].processingStartTime.tv_nsec/1000));
                    /*GCC warning fix+*/    
                    /*SPR 15909 Fix End*/
                    /* SPR 15816 fix end */
                    /* SPR 16593 fix end */
                    /* CA Stats Changes End */

                }
            }
#endif 


/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/
        }
    }
	return PNULL;
}
/*Cyclomatic Complexity changes - starts here*/
/*****************************************************************************
 * Function Name  : initDlUlThreadParams 
 * Inputs         : threadArg_p - pointer to ExecutionFlowArguments
 * Outputs        : None
 * Returns        : None
 * Description    : This function will init various ul and dl queues.
 *****************************************************************************/
static  void initDlUlThreadParams(ExecutionFlowArguments * threadArg_p)
{
/* GCC_4.9_WARNINGS_FIX_START */
#if (!defined(CIRC_QUEUE_IMPL) && !defined(DL_DATA_SEPARATION))
 /* GCC_4.9_WARNINGS_FIX_END */
    UInt32 encoderContainerCount =0 ;
    UInt8 cellIndex = 0;
    UInt8 cellIndex2 = 0;
    UInt8 numberOfCells = cellSpecificParams_g.numOfCells;
#endif
    if (threadArg_p->startExecutionLeg == INIT_DL_THREAD_PARAMS)
    {
#ifndef CIRC_QUEUE_IMPL
        // DL Thread
        /*CA Changes start  */
        initDLAckNackQueue(&ackNackQueueIdDL_g[threadArg_p->internalCellIndex], threadArg_p->internalCellIndex);
        initUlStrategyQueue(&ulStrategyTxQueueIdDL_g[threadArg_p->internalCellIndex], threadArg_p->internalCellIndex);
        initPUSCHSignalQueue(&puschSignalQueueIdDL_g[threadArg_p->internalCellIndex], threadArg_p->internalCellIndex);
/* SPR 8712 changes */
#ifndef DL_DATA_SEPARATION
        initMACUEOppertunityIndQ(&macUeOppnIndQIdDL_g[threadArg_p->internalCellIndex], threadArg_p->internalCellIndex);
#endif        
/* SPR 8712 changes */
        UInt8 i = 0;
        for ( i = 0; i < MAX_NUMBER_OF_CONTAINER ; i++)
        {
            initULContainerNackQueue(&((ulContainerPool_gp[threadArg_p->internalCellIndex] + i)->ulContainerQIdDL), 
                         threadArg_p->internalCellIndex, i);
            initULContainerNackQueueForTempUE(&((ulContainerPool_gp[threadArg_p->internalCellIndex] + i)->ulContainerNackQueueForTempUEIdDL), 
                         threadArg_p->internalCellIndex, i);
		}
                
        /* Removed the code as waitForULNackQueue_g and waitForULNackForTempUE_g 
         * are defined in CIRC_QUEUE_IMPL */

/* SPR 8712 changes */
#ifndef DL_DATA_SEPARATION
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
        for (cellIndex2 = 0; cellIndex2 < numberOfCells; cellIndex2++)
        {
            for (encoderContainerCount=0; encoderContainerCount < MAX_ENCODER_CONTAINERS;encoderContainerCount++)
            {
                initEncoderQueue(&(encoderContainerArr_gp[cellIndex][cellIndex2] + encoderContainerCount)->\
                    encoderNode.encoderQueueIdDL, 
                    encoderContainerCount,cellIndex, cellIndex2);
            }
        }
    }
#endif        
/* SPR 8712 changes */
#endif
    }
    else if (threadArg_p->startExecutionLeg == INIT_UL_THREAD_PARAMS)
    {
#ifndef CIRC_QUEUE_IMPL

        // UL Thread
        /*CA Changes start  */
		initDLAckNackQueue(&ackNackQueueIdUL_g[threadArg_p->internalCellIndex], threadArg_p->internalCellIndex);
		initUlStrategyQueue(&ulStrategyTxQueueIdUL_g[threadArg_p->internalCellIndex], threadArg_p->internalCellIndex);
		initPUSCHSignalQueue(&puschSignalQueueIdUL_g[threadArg_p->internalCellIndex], threadArg_p->internalCellIndex);
		initMACUEOppertunityIndQ(&macUeOppnIndQIdUL_g[threadArg_p->internalCellIndex], threadArg_p->internalCellIndex);

		UInt8 i = 0;
		for ( i = 0; i < MAX_NUMBER_OF_CONTAINER ; i++)
		{
            initULContainerNackQueue(&((ulContainerPool_gp[threadArg_p->internalCellIndex] + i)->ulContainerQIdUL),
                         threadArg_p->internalCellIndex, i);
            initULContainerNackQueueForTempUE(&((ulContainerPool_gp[threadArg_p->internalCellIndex] + i)->ulContainerNackQueueForTempUEIdUL), 
                         threadArg_p->internalCellIndex, i);
		}

        /* Removed the code as waitForULNackQueue_g and waitForULNackForTempUE_g 
         * are defined in CIRC_QUEUE_IMPL */
        /*CA Changes end  */

/* SPR 8712 changes */
#ifndef DL_DATA_SEPARATION
    for (cellIndex = 0; cellIndex < numberOfCells; cellIndex++)
    {
        for (cellIndex2 = 0; cellIndex2 < numberOfCells; cellIndex2++)
        {
            for (encoderContainerCount=0; encoderContainerCount < MAX_ENCODER_CONTAINERS;encoderContainerCount++)
            {
                initEncoderQueue(&(encoderContainerArr_gp[cellIndex][cellIndex2] + encoderContainerCount)->\
                    encoderNode.encoderQueueIdUL, 
                    encoderContainerCount,cellIndex, cellIndex2);
            }
        }
    }
#endif
/* SPR 8712 changes */
#endif
    }
}
/*Cyclomatic Complexity changes - ends here*/

#else

/*****************************************************************************
 * Function Name  : waitOnSelect
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : The function populates the executionStatsInfo_p with
 *                  peak and average time in microseconds.
 *****************************************************************************/

/*void waitOnSelect()
{
   fd_Set readfd;
   lteFD_ZERO(&readfd);
   lteFD_SET(phyRxSockFD_g, &readfd);
   if (-1 == select_wrapper(1024, &readfd, NULL, NULL, NULL))
   {
     LTE_MAC_UT_LOG(LOG_INFO, EXE_SCH,"2.select failed, errno = %d\n", errno);
   }
}*/

/*****************************************************************************
 * Function Name  : executionFlowThread
 * Inputs         : executionFlowThreadArgs_p, execFlowAndIndex
 * Outputs        : None
 * Returns        : NULL
 * Description    : The thread executes execution legs passed to it through
 *                  arguments.
 *****************************************************************************/
void* executionFlowThread(void * executionFlowThreadArgs_p,ExecutionFlowAndCellIndex *execFlowAndIndex)
{
    ExecutionLegFunctionArg execLegFuncArg = {0};
    ExecutionFlowArguments * threadArg_p =
        (ExecutionFlowArguments *)executionFlowThreadArgs_p;
#if (!defined(DL_UL_SPLIT) && !defined(DL_DATA_SEPARATION))      
    /** El Sync Changes Start **/
    ExecutionLegInfo * executionLegArr_p = threadArg_p->execFlowFuncSubGroup[0].executionLegArr;
    /** El Sync Changes End **/
#elif DL_DATA_SEPARATION
    ExecutionLegInfo * executionLegArr_p = threadArg_p->executionLegArr;
#endif
    /*SPR 16855 +-*/
    UInt32 nAvg = 0;
    /* SPR 15909 fix start */
    tickType_t  currentTick = 0;
    /* SPR 15909 fix end */

    /** CA 2 Core Changes Start **/
    cellIndexMac_g = execFlowAndIndex->cellIndex;
    /** CA 2 Core Changes End **/
    do
    {
#ifdef CYCLE_COUNT_MAC_RLC_PDCP
        lteIncrementL2Cycles(cellIndexMac_g);
#endif

        /*SPR 16855 +-*/
        execLegFuncArg.execFlowIndex = threadArg_p->threadIndex;
        execLegFuncArg.delayToPhy = threadArg_p->dlPathDelay;
 /** CA 2 Core Changes Start **/
        execLegFuncArg.cellIndex  = execFlowAndIndex->cellIndex;
 /** CA 2 Core Changes End **/

        /* CSR : 00020794 Start */

        if(CELL_PHY_RUNNING != cellSpecificParams_g.\
                cellConfigAndInitParams_p[execFlowAndIndex->cellIndex]->cellState)
        {
            return PNULL;
        }
        /* CSR : 00020794 End */
        do
        {
            /* The first leg wait for TICK indication , after 
             * tick reception we get SF and SFN*/
#ifdef OVERSHOOT_CORRECTION 
            currTickOverLoad_g[execFlowAndIndex->cellIndex] = OVERLOAD_LEVEL_NORMAL;
            noTimeToProcessflag_g = 0;
#endif
#ifndef UTFWK_SIMULATION
            RDTSC_AND_CLOCK_START_FDD(0);
#endif
            currentTick = getCurrentTick();

            execLegFuncArg.execLegSpecificArg1 = 
                executionLegArr_p[0].legSpecificArg1;

            execLegFuncArg.execLegSpecificArg2 = 
                executionLegArr_p[0].legSpecificArg2;

            executionFunctionArr_g[executionLegArr_p[0].executionLeg]
                (&execLegFuncArg);

#ifndef UTFWK_SIMULATION
           /* SPR 10852 fix start */
            /* CA Stats Changes Start */
            RDTSC_AND_CLOCK_END_MAC(0, cellIndexMac_g);
            /* CA Stats Changes End */
           /* SPR 10852 fix end */
#endif
            /* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
            /*
             ** As we have updated MAC tick with L2 tick when we received
             ** PHY Start message from L1 Layer. Check whether
             ** startExecutionAfterPhyStart_g is set.
             */
            /*SPR 13047 fix start*/   
            if (startExecutionAfterPhyStart_g[execLegFuncArg.cellIndex] == TRUE)
            {
                startExecutionAfterPhyStart_g[execLegFuncArg.cellIndex] = FALSE;

                /* SPR 1059 Fix Start */
                if ( FALSE == firstTickRecvFlag_g[execLegFuncArg.cellIndex])
                {
                    return PNULL;
                }
                else
                {
                    firstTickRecvFlag_g[execLegFuncArg.cellIndex] = FALSE;
                }
                /* SPR 1059 Fix End */
            }
            /*SPR 13047 fix end*/   
            /*
             ** Checks whether new TICK has been received from the system.
             ** If yes, then process the remaining ELs.
             */
            else if (currentTick != getCurrentTickFrmL2Tick(execLegFuncArg.cellIndex))
            {
                /*
                 ** Set the currentTick, systemFrame and subframe globals
                 ** for MAC.
                 ** Initialize some other global variables which is required
                 ** before starting ELs execution.
                 */
                processAndValidateL2TickBeforeELExecution(execLegFuncArg.cellIndex);
            }
            else
            {
                /* Waiting for the Tick from system */
                return PNULL;
            }
#else

            if (currentTick == getCurrentTick())
            {
                return PNULL;
            }
#endif
            /* FAPI L2 AS MASTER END */
/* SPR 10852 fix start */
#ifdef CYCLE_COUNT_MAC_RLC_PDCP
            lteIncrementL2Cycles(cellIndexMac_g);
#endif
            RDTSC_AND_CLOCK_END_MAC(0, cellIndexMac_g);
/* SPR 10852 fix end */

            execLegFuncArg.globalTick = getCurrentTick();
/*CA Changes start  */
            getSFAndSFN(&execLegFuncArg.sysFrame, &execLegFuncArg.subFrame, execLegFuncArg.cellIndex);

            
            /*CA Changes end  */
            /** CA 2 Core Changes Start **/
            if(CELL_PHY_INIT <= cellSpecificParams_g.\
                    cellConfigAndInitParams_p[execFlowAndIndex->cellIndex]->cellState)
            /** CA 2 Core Changes End **/
            {
                /* non blocking adapter function call to recv packet on UDP socket */
                /* MAC processing 
                   PROCESS_DL_SCH_STRATEGY
                   PROCESS_UL_SCH_STRATEGY
                   PROCESS_PDCCH_PDSCH_WITH_MUX
                   */
                /* SPR 990 */
#ifndef UTFWK_SIMULATION
#ifdef OVERSHOOT_CORRECTION
                /* + PRS_CHANGES */
/** CA 2 Core Changes Start **/
#define EL_COUNT_MAX   15
                /* - PRS_CHANGES */          
#else
/* SPR 13311 Fix : Start */
#ifdef DL_DATA_SEPARATION
#define EL_COUNT_MAX   25
#else
/* SPR 13311 Fix : End */
#define EL_COUNT_MAX   21
/* SPR 13311 Fix : Start */
#endif
/* SPR 13311 Fix : End */
/** CA 2 Core Changes End **/
#endif
#else
#define EL_COUNT_MAX   threadArg_p->executionLegCount
#endif
                /*Cyclomatic Complexity changes - starts here*/  
                executionWhileStoppedProcessingFlagIsFalse(&execLegFuncArg,
                        executionLegArr_p,
                        threadArg_p);
                /*Cyclomatic Complexity changes - ends here*/  
            }

#ifdef OVERSHOOT_CORRECTION
            /** CA2 Changes Start **/
            setOverLoadLevel(currTickOverLoad_g[execFlowAndIndex->cellIndex],
                    execFlowAndIndex->cellIndex);
            /** CA2 Changes End **/
#endif
/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/
            /*break from the loop and wait for the init req from OAM*/
            if(CELL_PHY_INIT > cellSpecificParams_g.\
                    cellConfigAndInitParams_p[execFlowAndIndex->cellIndex]->cellState)
            {
                break;
            }
            nAvg++;
        }
        while(0);
    }
    while(0);
    return PNULL;
}

/*SPR 16855 +-*/
/*****************************************************************************
 * Function Name  : executionWhileStoppedProcessingFlagIsFalse 
 * Inputs         : execLegFuncArg - pointer to ExecutionLegFunctionArg,
 *                : executionLegArr_p - pointer to ExecutionLegInfo,
 *                : threadArg_p - pointer to ExecutionFlowArguments
 * Outputs        : None
 * Returns        : None
 * Description    : This will take various actions when StoppedProcessingFlag_g is FALSE.
 *****************************************************************************/
#if defined(MSPD_WATCH_DOG)
TimeSpec tElEnterTime={0,0};
TimeSpec tElExitTime={0,0};
UInt32 tElEnterIndex=0;
UInt32 tElExitIndex=0;
#endif
static  void executionWhileStoppedProcessingFlagIsFalse(
        ExecutionLegFunctionArg *execLegFuncArg,
        ExecutionLegInfo * executionLegArr_p,
        ExecutionFlowArguments * threadArg_p)
{
#if (!defined(DL_UL_SPLIT) && !defined(DL_DATA_SEPARATION))
    UInt32 executionLegCount = 0;
    UInt32 executionLegStatsCount = 0; 
    UInt8 execFlowSubGroupCount =0;
#elif DL_DATA_SEPARATION
    UInt32 executionLegCount = 1;
#endif
    /* SPR 1781 changes start */
    UInt64 count = 0;
    /* SPR 1781 changes end */
    /** CA2 Changes Start **/
    static  UInt32 ncount[MAX_NUM_CELL] = {0};
    /** CA2 Changes End **/
#ifdef OVERSHOOT_CORRECTION
    UInt64 rem_cycles =0;
#endif
/** SPR 13293 Changes start **/

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    UInt8 cellCount = 0;
#endif
/** SPR 13293 Changes end **/
    /**  El Sync Changes Start **/
#if (!defined(DL_UL_SPLIT) && !defined(DL_DATA_SEPARATION))
    while(execFlowSubGroupCount < threadArg_p->executionFlowSubGruopCount)
    {
        if(execFlowSubGroupCount == 0)
        {
            executionLegCount = 1;/* Sub Frame Indication Already  executed */
            executionLegStatsCount = 1;
        }
        while(threadArg_p->execFlowFuncSubGroup[execFlowSubGroupCount].
                executionLegArr[executionLegCount].executionLeg != MAX_EXECUTION_LEGS )
        {
            RDTSC_AND_CLOCK_START_FDD(executionLegStatsCount);
            execLegFuncArg->execLegSpecificArg1 = 
                executionLegArr_p[executionLegCount].legSpecificArg1;
            execLegFuncArg->execLegSpecificArg2 = 
                executionLegArr_p[executionLegCount].legSpecificArg2;
            executionFunctionArr_g[executionLegArr_p[executionLegCount]
                .executionLeg] (execLegFuncArg);
            /* CA Stats Changes Start */
            RDTSC_AND_CLOCK_END_MAC(executionLegStatsCount, execLegFuncArg->cellIndex); 
            /* CA Stats Changes End */
            executionLegCount++;
            executionLegStatsCount++;
        }
        if((threadArg_p->execFlowFuncSubGroup[execFlowSubGroupCount].optionalElCount) > 0 )
        {
            while(threadArg_p->execFlowFuncSubGroup[execFlowSubGroupCount].
                    executionLegArr[executionLegCount +1].executionLeg != MAX_EXECUTION_LEGS )
            {
                /* OVERSHOOT_CORRECTION_CHG_START */
#ifdef OVERSHOOT_CORRECTION
                GET_PROCESSING_CYCLES_LEFT(rem_cycles);
                if (( !noTimeToProcessflag_g ) && 
                        (rem_cycles >= reqElProcessingCycles[execLegFuncArg->cellIndex]\
                         [reqIndexForUeSchdInDlUl_g][executionLegCount]))
                {
#endif
                    /* OVERSHOOT_CORRECTION_CHG_END */
                    RDTSC_AND_CLOCK_START_FDD(executionLegStatsCount);
                    execLegFuncArg->execLegSpecificArg1 = 
                        executionLegArr_p[executionLegCount+1].legSpecificArg1;

                    execLegFuncArg->execLegSpecificArg2 = 
                        executionLegArr_p[executionLegCount +1].legSpecificArg2;


                    executionFunctionArr_g[executionLegArr_p[executionLegCount+1]
                        .executionLeg] (execLegFuncArg);
                    /* CA Stats Changes Start */
                    RDTSC_AND_CLOCK_END_MAC(executionLegStatsCount, execLegFuncArg->cellIndex);
                    /* CA Stats Changes End */
                    executionLegCount++;
                    executionLegStatsCount++;
                    /* OVERSHOOT_CORRECTION_CHG_START */
#ifdef OVERSHOOT_CORRECTION 
                }
                else
                {
                    if(currTickOverLoad_g[execLegFuncArg->cellIndex] != 
                            OVERLOAD_LEVEL_CRITICAL)
                    {
                        /* + FDD_COMPILATION_WARNING */
                        if (executionLegArr_p[executionLegCount].executionLeg == PROCESS_DEMUX_QUEUE)
                            /* - FDD_COMPILATION_WARNING */
                        {
                            currTickOverLoad_g[execLegFuncArg->cellIndex] = 
                                OVERLOAD_LEVEL_HIGH;

                            LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                                    L2_SYS_FAIL,
                                    getCurrentTick(),
                                    __LINE__,
                                    currTickOverLoad_g[execLegFuncArg->cellIndex],
                                    /** CA-TDD Changes Start **/
                                    macOverLoadLevel_g[execLegFuncArg->cellIndex],
                                    /** CA-TDD Changes Stop **/
                                    /*+ Dyn UEs To Be SchPerTTI +*/
                                    dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 ,__func__,
                                    /*- Dyn UEs To Be SchPerTTI -*/
                                    "Overload level changed.");
                        }
                        else
                        {
                            currTickOverLoad_g[execLegFuncArg->cellIndex] = 
                                OVERLOAD_LEVEL_MEDIUM;
                            LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                                    L2_SYS_FAIL,
                                    getCurrentTick(),
                                    __LINE__,
                                    currTickOverLoad_g[execLegFuncArg->cellIndex],
                                    /** CA-TDD Changes Start **/
                                    macOverLoadLevel_g[execLegFuncArg->cellIndex],
                                    /** CA-TDD Changes Stop **/
                                    /*+ Dyn UEs To Be SchPerTTI +*/
                                    dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 ,__func__,
                                    /*- Dyn UEs To Be SchPerTTI -*/
                                    "Overload level changed.");
                        }
                    }

                    break;
                }
#endif
                /* OVERSHOOT_CORRECTION_CHG_END */
            }
        }
        execFlowSubGroupCount++;
        executionLegCount = 0;

        executionLegArr_p = &threadArg_p->execFlowFuncSubGroup[execFlowSubGroupCount].executionLegArr[executionLegCount];
    }
    /** El Sync Changes End **/
#elif DL_DATA_SEPARATION
    while(executionLegCount < EL_COUNT_MAX)
    { 
        RDTSC_AND_CLOCK_START_FDD(executionLegCount);
        execLegFuncArg->execLegSpecificArg1 = 
            executionLegArr_p[executionLegCount].legSpecificArg1;

        execLegFuncArg->execLegSpecificArg2 = 
            executionLegArr_p[executionLegCount].legSpecificArg2;

        executionFunctionArr_g[executionLegArr_p[executionLegCount]
            .executionLeg] (execLegFuncArg);
        /* CA Stats Changes Start */
        RDTSC_AND_CLOCK_END_MAC(executionLegCount, threadArg_p->
                internalCellIndex);
        /* CA Stats Changes End */
        executionLegCount++;
    }

#ifndef UTFWK_SIMULATION
#ifdef OVERSHOOT_CORRECTION
    /* + PRS_CHANGES */
    while(executionLegCount < 16)
        /* - PRS_CHANGES */
    {
        RDTSC_AND_CLOCK_START_FDD(executionLegCount);
        execLegFuncArg->execLegSpecificArg1 = 
            executionLegArr_p[executionLegCount].legSpecificArg1;

        execLegFuncArg->execLegSpecificArg2 = 
            executionLegArr_p[executionLegCount].legSpecificArg2;

        executionFunctionArr_g[executionLegArr_p[executionLegCount]
            .executionLeg] (execLegFuncArg);
        /* CA Stats Changes Start */
        RDTSC_AND_CLOCK_END_MAC(executionLegCount,
                threadArg_p->internalCellIndex);
        /* CA Stats Changes End */
        executionLegCount++;

    }
#endif
#endif


    while(executionLegCount < threadArg_p->executionLegCount)
    {
        /*FDD_SRS*/
#ifdef OVERSHOOT_CORRECTION
        GET_PROCESSING_CYCLES_LEFT(rem_cycles);
        if (( !noTimeToProcessflag_g ) && 
                /*+ Dyn UEs To Be SchPerTTI +*/
                /** CA2 Changes Start **/
                (rem_cycles >= reqElProcessingCycles[execLegFuncArg->cellIndex]\
                 [reqIndexForUeSchdInDlUl_g][executionLegCount]))
            /** CA2 Changes End **/
            /*+ Dyn UEs To Be SchPerTTI +*/
        {
#endif
            RDTSC_AND_CLOCK_START_FDD(executionLegCount);
            execLegFuncArg->execLegSpecificArg1 = 
                executionLegArr_p[executionLegCount].legSpecificArg1;

            execLegFuncArg->execLegSpecificArg2 = 
                executionLegArr_p[executionLegCount].legSpecificArg2;

            executionFunctionArr_g[executionLegArr_p[executionLegCount]
                .executionLeg] (execLegFuncArg);
            /* CA Stats Changes Start */
            RDTSC_AND_CLOCK_END_MAC(executionLegCount, 
                    threadArg_p->internalCellIndex);
            /* CA Stats Changes End */
            executionLegCount++;
#ifdef OVERSHOOT_CORRECTION 
        }
        else
        {
            if(currTickOverLoad_g[execLegFuncArg->cellIndex] != 
                    OVERLOAD_LEVEL_CRITICAL)
            {
                /* + FDD_COMPILATION_WARNING */
                if (executionLegArr_p[executionLegCount].executionLeg == PROCESS_DEMUX_QUEUE)
                    /* - FDD_COMPILATION_WARNING */
                {
                    currTickOverLoad_g[execLegFuncArg->cellIndex] = 
                        OVERLOAD_LEVEL_HIGH;

                    LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                            L2_SYS_FAIL,
                            getCurrentTick(),
                            __LINE__,
                            currTickOverLoad_g[execLegFuncArg->cellIndex],
                            /** CA-TDD Changes Start **/
                            macOverLoadLevel_g[execLegFuncArg->cellIndex],
                            /** CA-TDD Changes Stop **/
                            /*+ Dyn UEs To Be SchPerTTI +*/
                            dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 ,__func__,
                            /*- Dyn UEs To Be SchPerTTI -*/
                            "Overload level changed.");
                }
                else
                {
                    currTickOverLoad_g[execLegFuncArg->cellIndex] = 
                        OVERLOAD_LEVEL_MEDIUM;
                    LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                            L2_SYS_FAIL,
                            getCurrentTick(),
                            __LINE__,
                            currTickOverLoad_g[execLegFuncArg->cellIndex],
                            /** CA-TDD Changes Start **/
                            macOverLoadLevel_g[execLegFuncArg->cellIndex],
                            /** CA-TDD Changes Stop **/
                            /*+ Dyn UEs To Be SchPerTTI +*/
                            dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 ,__func__,
                            /*- Dyn UEs To Be SchPerTTI -*/
                            "Overload level changed.");
                }
            }

            break;
        }
#endif
    }
#endif //ifdnef DL_UL_DPLIT and DL_DATA_SEPERATION End
    /*Cyclomatic Complexity changes - starts here*/
    checkForRlcTimerExpiry(execLegFuncArg->cellIndex);
    /*Cyclomatic Complexity changes - ends here*/

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    /** SPR 13293 Changes start **/
    for(cellCount = 0; cellCount < ( cellSpecificParams_g.numOfCells
                     ); 
       cellCount++)
    {
        if(circQueueCount(&rlcTxPendingDelQ_g[cellCount]))
        {
            rlcProcessDelMsg(&rlcTxPendingDelQ_g[cellCount]);
        }
    }
    if(circQueueCount(&rlcRxPendingDelQ_g))
    {
        rlcProcessDelMsg(&rlcRxPendingDelQ_g);
    }
    /** SPR 13293 Changes end **/
    /* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
    if(circQueueCount(&rlcEmbmsLpToHpQ_g))
    {
        rlcProcessEmbmsHpHandler(&rlcEmbmsLpToHpQ_g);
    }
#endif
    /* eMBMS Code Changes End */
#endif

    count++;
/** CA2 Changes Start **/
    ncount[execLegFuncArg->cellIndex]++;

    if ( 50 == ncount[execLegFuncArg->cellIndex] ) 
/** CA2 Changes End **/
    {
        /*
         ** START_SPR_998_FIX : Introduced the functionality of timer
         ** expiry for ROHC.
         */
        /* SPR 3054 changes start */
        /*RLC TM*/

#ifdef OVERSHOOT_CORRECTION
        GET_PROCESSING_CYCLES_LEFT(rem_cycles);
        if(( !noTimeToProcessflag_g ) && 
                /*+ Dyn UEs To Be SchPerTTI +*/
                /** CA2 Changes Start **/
                (rem_cycles >= reqElProcessingCycles[execLegFuncArg->cellIndex]\
                 [reqIndexForUeSchdInDlUl_g][RLC_HANDLE_RESTAB_Q]))
            /** CA2 Changes End **/
            /*- Dyn UEs To Be SchPerTTI -*/
        {
            /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
            RDTSC_AND_CLOCK_START_FDD(RLC_HDLR_REESTABLISH_Q);
            rlcHdlrReEstabQ(&rlcTxReEstabQ_g);
            /* CA Stats Changes Start */
            RDTSC_AND_CLOCK_END_RLC(RLC_HDLR_REESTABLISH_Q, 
                    threadArg_p->internalCellIndex);
            /* CA Stats Changes End */
#endif
        }
        else
        {
            if(currTickOverLoad_g[execLegFuncArg->cellIndex] < OVERLOAD_LEVEL_MEDIUM)
            {
                currTickOverLoad_g[execLegFuncArg->cellIndex] = 
                    OVERLOAD_LEVEL_MEDIUM;
                LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                        L2_SYS_FAIL,
                        getCurrentTick(),
                        __LINE__,
                        currTickOverLoad_g[execLegFuncArg->cellIndex],
                        /** CA-TDD Changes Start **/
                        macOverLoadLevel_g[execLegFuncArg->cellIndex],
                        /** CA-TDD Changes Stop **/
                        /*+ Dyn UEs To Be SchPerTTI +*/
                        dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 , __func__,
                        /*- Dyn UEs To Be SchPerTTI -*/
                        "Overload level changed.");
            }
        }
        GET_PROCESSING_CYCLES_LEFT(rem_cycles);
        if(( !noTimeToProcessflag_g ) &&
                /*+ Dyn UEs To Be SchPerTTI +*/
                /** CA2 Changes Start **/
                (rem_cycles >= reqElProcessingCycles[execLegFuncArg->cellIndex]\
                 [reqIndexForUeSchdInDlUl_g][RLC_HANDLE_RESTAB_Q]))
            /** CA2 Changes End **/
            /*- Dyn UEs To Be SchPerTTI -*/
        {
            /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING 
            RDTSC_AND_CLOCK_START_FDD(RLC_HDLR_REESTABLISH_Q);
            rlcHdlrReEstabQ(&rlcRxReEstabQ_g);
            /* CA Stats Changes Start */
            RDTSC_AND_CLOCK_END_RLC(RLC_HDLR_REESTABLISH_Q, 
                    threadArg_p->internalCellIndex);
            /* CA Stats Changes End */
#endif
        }
        else
        {
            if(currTickOverLoad_g[execLegFuncArg->cellIndex] < 
                    OVERLOAD_LEVEL_MEDIUM)
            {
                currTickOverLoad_g[execLegFuncArg->cellIndex] = 
                    OVERLOAD_LEVEL_MEDIUM;
                LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                        L2_SYS_FAIL,
                        getCurrentTick(),
                        __LINE__,
                        currTickOverLoad_g[execLegFuncArg->cellIndex],
                        /** CA-TDD Changes Start **/
                        macOverLoadLevel_g[execLegFuncArg->cellIndex],
                        /** CA-TDD Changes Stop **/

                        /*+ Dyn UEs To Be SchPerTTI +*/
                        dlUeSchdIndxInCurrTick_g,ulUeSchdIndxInCurrTick_g, 0, 0 , __func__,
                        /*- Dyn UEs To Be SchPerTTI -*/
                        "Overload level changed.");
            }
        }

#else
        /*SPR_16111 Fix start*/
#ifdef UE_SIM_TESTING  
        rlcHdlrReEstabQ(&rlcTxReEstabQ_g);
        rlcHdlrReEstabQ(&rlcRxReEstabQ_g);
#endif
        /*SPR_16111 Fix end*/
#endif
        /** CA2 Changes Start **/
        ncount[execLegFuncArg->cellIndex] = 0;
        /** CA2 Changes End **/
    }
}
/*****************************************************************************
 * Function Name  : checkForRlcTimerExpiry 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : checks whether to expire rlc timer or not.
 *****************************************************************************/
STATIC  void checkForRlcTimerExpiry(InternalCellIndex internalCellIndex)
{
#ifdef OVERSHOOT_CORRECTION
    UInt64 rem_cycles = 0;
#endif
    if(RLC_FAILURE != rlcLayerInitialized_g 
                    &&  pdcpContextIsInitialized()
                      )
                      
    {
        /* RLC TIMER SEG FAULT CHG Start */
        /* Following RLC TX functionality need to be called from MAC thread*/
#ifdef OVERSHOOT_CORRECTION
        GET_PROCESSING_CYCLES_LEFT(rem_cycles);
        if(( !noTimeToProcessflag_g ) && 
           /*+ Dyn UEs To Be SchPerTTI +*/
           /** CA2 Changes Start **/
           (rem_cycles >= reqElProcessingCycles[internalCellIndex][reqIndexForUeSchdInDlUl_g][RLC_TIMER_EXPIRY]))
           /** CA2 Changes End **/
           /*- Dyn UEs To Be SchPerTTI -*/
        {
            rlcCheckTimerExpiry(internalCellIndex);
        }
        else
        {
            if(currTickOverLoad_g[internalCellIndex] < OVERLOAD_LEVEL_MEDIUM)
            {
                currTickOverLoad_g[internalCellIndex] = OVERLOAD_LEVEL_MEDIUM;
                LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                        L2_SYS_FAIL,
                        getCurrentTick(),
                        __LINE__,
                        currTickOverLoad_g[internalCellIndex],
                        /** CA-TDD Changes Start **/
                        macOverLoadLevel_g[internalCellIndex],
                        /** CA-TDD Changes Stop **/
                        /*+ Dyn UEs To Be SchPerTTI +*/
                        dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 ,__func__,
                        /*- Dyn UEs To Be SchPerTTI -*/
                        "Overload level changed.");
            }
        }
#else
        rlcCheckTimerExpiry(internalCellIndex);
#endif
        /* RLC TIMER SEG FAULT CHG End */
    }
}
/*Cyclomatic Complexity changes - ends here*/
#endif

#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
/*****************************************************************************
 * Function Name  : createExecutionFlowThreads
 * Inputs         : executionFlowThreadParamArr_p - pointer to 
 *                                                  ExecutionFlowCreationParam
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function creates Execution Flow Threads.
 *****************************************************************************/
MacRetType createExecutionFlowThreads( 
        ExecutionFlowCreationParam * executionFlowThreadParamArr_p)
{
    UInt32 execFlowCount = 0;
    MacRetType retType = MAC_SUCCESS;
    UInt8 cellCount = 0;
    ULong32 threadId = 0;
    /* CA Changes start */
    /* NUM_OF_EXEC_FLOWS = 3 for DL_UL_SPLIT and 
     * For CA, UL RLC will be 1.
     */  
#ifdef FDD_CONFIG
     UInt8 numMacFlows=NUM_OF_EXEC_FLOWS;
#elif DL_UL_SPLIT_TDD
     UInt8 numMacFlows=NUM_OF_MAC_EXEC_FLOWS;
#endif
    while (cellCount <  cellSpecificParams_g.numOfCells)
    { 
        execFlowCount = 0;
        for(; execFlowCount < numMacFlows; execFlowCount++)  
        {
            execSchedulerParams[cellCount][execFlowCount].cellIndex = cellCount;
            execSchedulerParams[cellCount][execFlowCount].executionFlowCount = execFlowCount;
            if ((cellCount == 0)  
                    ||
                 ((cellCount != 0) && (executionFlowThreadParamArr_p[execFlowCount].threadArguments.startExecutionLeg != INIT_RLC_PARAMS))
                 )
            {
                threadId =  executionFlowThreadParamArr_p[execFlowCount].threadId + (cellCount * NUM_OF_EXEC_FLOWS);
        if(0 != threadCreate
                (
                        &(threadId),
                THREAD_CREATE_DETACHED,
                SCHED_FIFO,
                executionFlowThreadParamArr_p[execFlowCount].threadPriority,
                THREAD_INHERIT_SCHED_DEFAULT,
                THREAD_SCOPE_SYSTEM, 
#ifdef FDD_CONFIG                
                executionFlowThread,
#elif DL_UL_SPLIT_TDD
                executionTDDFlowThread,
#endif
                        (void *)&execSchedulerParams[cellCount][execFlowCount]))
                {
                    ltePanic("Execution flow thread creation failed\n");
                    retType = MAC_FAILURE;
                }
            }

            /* CA Changes start */
            /*coreNum = lteLayer2CoreNum_g;
            THREAD_TO_BIND_SET_CORE_NO(executionFlowThreadParamArr_p[execFlowCount].threadId,coreNum);*/
            /* Core binding is done in executionFlowThread */
        }
        cellCount++;
     }
#ifdef DL_UL_SPLIT_TDD
    executionFlowCreationParamRlc_g.threadArguments.threadIndex = execFlowCount;
    /* Separate RLC ThreadCraetion in TDD*/
    if(0 != threadCreate
            (
             &(executionFlowCreationParamRlc_g.threadId),
             THREAD_CREATE_DETACHED,
             SCHED_FIFO,
             executionFlowCreationParamRlc_g.threadPriority,
             THREAD_INHERIT_SCHED_DEFAULT,
             THREAD_SCOPE_SYSTEM,
             executionFlowThreadRlc,
             (void*)(&(executionFlowCreationParamRlc_g.
                     threadArguments))))
    {
        ltePanic("Execution flow thread creation failed\n");
        retType = MAC_FAILURE;
    }
    coreNum = lteLayer2CoreNum_g;
    THREAD_TO_BIND_SET_CORE_NO(executionFlowCreationParamRlc_g.threadId,
                               coreNum);
#endif
    return retType;

/* CA Changes end */
}
#endif 

/*****************************************************************************
 * Function Name  : initExecutionScheduler
 * Inputs         : numOfCells
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This functions does the initializations necessary for 
 *                  execution scheduler which includes creation of socket/msgQ
 *                  interfaces b/w MAC-RRC/OAM and MAC-PHY.It calls 
 *                  createExecutionFlowThreads which creates execution flow
 *                  threads.
 *****************************************************************************/
MacRetType initExecutionScheduler(UInt8 numOfCells)
{
    MacRetType returnValue = MAC_SUCCESS;
    UInt32 flowCount = 0;

    initUeConfigMsgQueue(numOfCells);
#ifdef UTFWK_SIMULATION
    initueLCinQos();
#endif

    /** KlocWork Changes Start **/ 
    if(numOfCells > MAX_NUM_CELL)
    {
        numOfCells = MAX_NUM_CELL;
    }
    /** KlocWork Changes End **/ 

    /* Initialise ulRlcWaitForSubFrameIndSem_g , execution flow threads
     * wait on their correspondind index semaphore for tick indication*/
    /* CA Changes start */  
    semInitDefault(&ulRlcWaitForSubFrameIndSem_g, 0);
    for (flowCount = 0; flowCount < 
                NUM_OF_EXEC_FLOWS * numOfCells ; flowCount++)
    {
        semInitDefault(&waitForCellSetupIndSemArr_g[flowCount], 0);
    }

#ifdef DL_UL_SPLIT
    for (flowCount =0; flowCount < numOfCells; flowCount++)
    {
#ifdef DL_DATA_SEPARATION    
        semInitDefault(&waitForCellSetupIndSemEncThrd[flowCount],0);
#endif    
        semInit(&waitForDLPerfStatsUpd_g[flowCount],1);
    }
    /*Change DROP1*/
#endif
    initExecutionLegFunctionArr();

#ifdef DL_UL_SPLIT
/* SPR 4915 */
    unlinkMsgQueueForICC("/AckNackQueue");
    if (errno == ENOENT)
    {
    }

/* SPR 4915 */
    unlinkMsgQueueForICC("/ULStrategyTxQueue");
    if (errno == ENOENT)
    {
    }

/* SPR 4915 */
    unlinkMsgQueueForICC("/PUSCHSignalQ");

    if (errno == ENOENT)
    {
    }

/* SPR 4915 */
    unlinkMsgQueueForICC("/MACUeOpporIndQ");
    if (errno == ENOENT)
    {
    }
    closeULContainerNackQueue(MAX_NUMBER_OF_CONTAINER);
    closeEncoderQueue(MAX_ENCODER_CONTAINERS);
    /*create the Execution flow threads*/
#endif
/* CA Changes start */
    return returnValue;
}
#endif /* FDD_CONFIG*/


#ifdef DL_DATA_SEPARATION
InternalCellIndex execSchedulerThreadParams_g[MAX_NUM_CELL] = {0};
/*****************************************************************************
 * Function Name  : createEncoderThreads
 * Inputs         : 
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function creates Execution Flow Threads.
 *****************************************************************************/
/* + SPR 17439 */
MacRetType createEncoderThreads(void)
/* - SPR 17439 */    
{
    MacRetType retType = MAC_SUCCESS;
    UInt8 cellCount = 0;
    ULong32 threadId = 0;


      while (cellCount <  cellSpecificParams_g.numOfCells)
	  {
		  threadId = 0;
          execSchedulerThreadParams_g[cellCount] = cellCount;
          if(0 != threadCreate(&(threadId), THREAD_CREATE_DETACHED,
					  THREAD_SCHED_DEFAULT, 0,
					  THREAD_INHERIT_SCHED_DEFAULT,
					  THREAD_SCOPE_SYSTEM,
					  encoderScheduler,
                      (void*) (&execSchedulerThreadParams_g[cellCount])))
		  {
              ltePanic(" Unable to initialise the Layer2 because"
                      "encoder thread creation Failed.\n");
			  retType = MAC_FAILURE;
		  }
		  cellCount++;
	  }


    return retType;
}
#endif
/*****************************************************************************
 * Function Name  : createExecutionThreads
 * Inputs         : 
 * Outputs        : 
 * Returns        : MacRetType
 * Description    : This functions does the initializations necessary for 
 *                  execution scheduler which includes creation of socket/msgQ
 *                  interfaces b/w MAC-RRC/OAM and MAC-PHY.It calls 
 *                  createExecutionFlowThreads which creates execution flow
 *                    threads.
 *****************************************************************************/
/* + SPR 17439 */
MacRetType createExecutionThreads(void)
/* - SPR 17439 */    
{ 
 
    MacRetType returnValue = MAC_SUCCESS;
    UInt32 retTh = 0;
#ifndef DL_UL_SPLIT
    UInt8 cellCount = 0;
#endif

#ifndef UTFWK_SIMULATION
#ifndef DL_UL_SPLIT
    ULong32 threadId = 0;
    /* Compile Fix */
    while (cellCount < cellSpecificParams_g.numOfCells
     )
    {
         execSchedulerParams[cellCount].cellIndex = cellCount;
        /* In case of CA, thread id needs to be changed */
        retTh = threadCreate(&(threadId), THREAD_CREATE_DETACHED,
                THREAD_SCHED_DEFAULT, 0,
                THREAD_INHERIT_SCHED_DEFAULT,
                THREAD_SCOPE_SYSTEM,
                callFlowThreadExecutionSchedular,
                (void*)(&execSchedulerParams[cellCount]));
        if (retTh != 0) {
            ltePanic("Unable to create MAC and RLC DL thread\n");
            return MAC_FAILURE; 
        }
        cellCount++;
    }
#endif
#endif

/* SPR 21804 Fix Start */
#if defined( LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
    retTh = threadCreate(&dlTgExecutionSchedulerThId_g, THREAD_CREATE_DETACHED,
            THREAD_SCHED_DEFAULT, 0,
            THREAD_INHERIT_SCHED_DEFAULT,
            THREAD_SCOPE_SYSTEM,
            dlTgThreadExecutionSchedular, PNULL);
    if (retTh != 0) {
        ltePanic(" Unable to initialise the Layer2 because DL TG thread creation Failed.\n");
        return MAC_FAILURE;
    }
#endif

#ifdef FDD_CONFIG
#ifdef LAYER2_PHYSIM_THREAD
    retTh = threadCreate(&physimExecutionSchedulerThId_g, THREAD_CREATE_DETACHED,
            THREAD_SCHED_DEFAULT, 0,
            THREAD_INHERIT_SCHED_DEFAULT,
            THREAD_SCOPE_SYSTEM,
            physimThreadExecutionSchedular, PNULL);
    if (retTh != 0) {
        ltePanic(" Unable to initialise the Layer2 because PHYSIM thread creation Failed.\n");
        return;
    }
#endif
#endif
#ifdef DL_UL_SPLIT
    retTh = createExecutionFlowThreads(
                              executionFlowCreationParamArr_g);
    if (MAC_FAILURE == retTh)
    {
        ltePanic("[%s]Error in execution flow thread creation",__func__);
    }
#endif

#ifdef DL_DATA_SEPARATION
    retTh = createEncoderThreads();
    if (MAC_FAILURE == retTh)
    {
        ltePanic("[%s]Error in encoder  thread creation",__func__);
    }
#endif  

    return returnValue;
}

/****************************************************************************
 * Function Name  : postCellSetupInd
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called once cell setup is done. It
 *                  unblocks MAC (RT) Thread.
 ****************************************************************************/
/* CA Changes start */
void postCellSetupInd(InternalCellIndex internalCellIndex)
{
    CellConfigParams *cellParams_p = PNULL;
        cellParams_p = cellSpecificParams_g.\
                                      cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    cellParams_p->macCellState = MAC_RUNNING;

    /* + SPR_10009 */ 
    /* SPR 5346 changes start */   
    UInt32 currentSFN = 0;
    UInt32 currentSF = 0;
    /* SPR 5946 changes start */
    getSFAndSFN(&currentSFN,&currentSF,internalCellIndex);
    /* SPR 5946 changes end */

#ifdef TDD_CONFIG 
    /* +- SPR 17777 */
    dummyTxDLControlMsg(DL_ADVANCE_TICK,
            currentSFN, currentSF, internalCellIndex);
#else   
/*changes cell_setup_13April start*/
/*changes cell_setup_13April ends*/
    dummyTxDLControlMsg(PHY_DL_DELAY,
            currentSFN, currentSF, internalCellIndex);
/*changes cell_setup_13April start*/
/*changes cell_setup_13April ends*/
    /* +- SPR 17777 */
#endif
    /* SPR 5346 changes end */   
#ifdef TDD_CONFIG
    initUlDLExecutionFlow(internalCellIndex);
    initMacTDDConfig(internalCellIndex);

    /* + SPR 14364 Changes */
#ifdef OVERSHOOT_CORRECTION
    memSet(&reqdProcessingTimeAtMuxTDD_g[internalCellIndex], 0,
            (sizeof(UInt32) * MAX_NUMBER_OF_SUBFRAME_IN_SYSTEMFRAME * MAX_UES_SCHEDULED_INDX));
#endif
    /* - SPR 14364 Changes */

    populateTddELSchedulerArray(cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p, internalCellIndex);
    assignTddPdschTxMapOfDLHarqPointer(cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign, internalCellIndex);
#endif  

    /* SPR 4915 : Start*/
    if(CELL_PHY_RUNNING ==  
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellState)
        /* SPR 4915 : End*/
    {
/* SPR 19288 change start */
        UInt32 count = 0;
        UInt32  totalExecutionFlow = NUM_OF_EXEC_FLOWS; 
        while (count < totalExecutionFlow)
        {
            semPostDefault(&waitForCellSetupIndSemArr_g[(internalCellIndex*NUM_OF_EXEC_FLOWS)+count]);
            count++;
        }
/* SPR 19288 change end */
    }
    /* - SPR_10009 */ 
}

/*SPR 16855 +-*/
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))

/****************************************************************************
 * Function Name  : callFlowThreadExecutionSchedular
 * Inputs         : param pointer
 * Outputs        : None
 * Returns        : NULL
 * Description    : This function is initialization function for  
 *                  thread.
 ****************************************************************************/
void* callFlowThreadExecutionSchedular(void * param)
{
#ifndef FLEXRAN   
    fd_Set  readfds;
    TimeSpec timeout;
#endif	
    /* SPR 15909 fix start */
    tickType_t currentTick = 0;
    /* SPR 15909 fix end */
    /** CA 2Core  Changes Start **/
    CellState cellState = CELL_INIT;
    ExecutionFlowAndCellIndex *execFlowAndIndex_p = (ExecutionFlowAndCellIndex *)param;
    ExecutionFlowAndCellIndex execFlowAndindex = {0};
    execFlowAndindex.cellIndex = execFlowAndIndex_p->cellIndex;
    cellIndexMac_g = execFlowAndindex.cellIndex; 
    /** CA 2Core  Changes End **/
    THREAD_TO_BIND_SET_CORE_NO(threadSelf(),
            lteLayer2CoreInfo_g.lteLayer2DLCoreNum[execFlowAndindex.cellIndex]);
#ifdef FLEXRAN
    if (execFlowAndindex.cellIndex)
    {
         prctl(PR_SET_NAME, "lte_mac_1", NULL, NULL, NULL);
    }
    else
    {
         prctl(PR_SET_NAME, "lte_mac_0", NULL, NULL, NULL);
    }
#endif
    /*SPR 20199 fix start*/
#ifdef WATCHDOG_THREAD

    threadSetRtPriority(SCHED_FIFO, 98);
#else
    threadSetRtPriority(SCHED_FIFO, 99);
#endif
    /*SPR 20199 fix end*/

#ifndef FLEXRAN    
    UInt32 retval = 0;
    /* Porting Changes Start*/
    UInt32 receivePhyMsg =0;
#endif
    /* Porting Changes End*/
    ReceivePhyDataQueueNode *recvPhyDataQueueNode_p = PNULL;
    /* CA Logging Changes Start */
    Char8 shmRegionName[NAME_MAX] = {0};
    /* CA Logging Changes End */

/*SPR 21653 changes start*/
    prepareFileName(shmRegionName, NAME_MAX, LTE_L2_LOG_REGION_ONE_NAME, 
            execFlowAndindex.cellIndex);
    logger_init((LTE_L2_LOG_REGION_ONE_ID + execFlowAndindex.cellIndex),
            shmRegionName, l2Input.shmSize);
/*SPR 21653 changes end*/
    /* -SPR_10792 Changes */

    startExecSchedLogReader(&l2Input, shmRegionName);
    /* CA Logging Changes End */
    while(1)
    {
       /** CA 2 Core Changes Start **/
        cellState = cellSpecificParams_g.\
            cellConfigAndInitParams_p[execFlowAndindex.cellIndex]->cellState;
        if(CELL_PHY_RUNNING != cellState && 
                CELL_PHY_RUNNING_CONFIG_RECVD  != cellState)
        /** CA 2 Core Changes End **/
        {
            /* Porting Changes Start*/
            /* wait for cell setup indication */
            /* Memory cleanup changes start*/
            /* Cleanup L2 startup queues */
            cleanupLayer2StartupQueues(execFlowAndindex.cellIndex);
		    /* Memory cleanup changes end*/
		    /** CA2 Changes Start **/
/* SPR 19288 change start */
/* SPR 19288 change end */
            semWaitDefault(&waitForCellSetupIndSemArr_g[execFlowAndindex.cellIndex]);
		    /** CA2 Changes End **/
		   

#ifndef FLEXRAN            
            lteFD_ZERO(&readfds);
/* SPR 19288 change start */
/*SPR 21001 Changes start */
            if(0 < phyRxSockFD_g[execFlowAndindex.cellIndex])
            {
                lteFD_SET(phyRxSockFD_g[execFlowAndindex.cellIndex],
                        &readfds);
            }
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
#ifdef TDD_CONFIG
#ifdef OVERSHOOT_CORRECTION
            GET_FIRST_CC();
            GET_FIRST_SFI_CC();
#endif
#endif
        }
        else
        {

#ifdef TDD_CONFIG
            /* + SPR 7521 + */
            /* CA TDD Changes Start */

            executionSingleTDDFlowThread(execFlowAndindex.cellIndex);
            /* CA TDD Changes End */
            /* - SPR 7521 - */
#else
/* SPR 21660 changes start_vishal */
            if (MAC_FAILURE == callExecutionFlowThreads(executionFlowCreationParamArr_g
#ifndef DL_UL_SPLIT                        
                        ,execFlowAndindex /*SPR 18490 +-*/
#endif                        
                        ))
            {
                ltePanic("[%s]Error in execution flow thread creation",__func__);
            }

#endif
            /* SPR 5599 changes end (ZIP ID 129245) */     
            if(currentTick != getCurrentTick())
            {
                /* CA Stats Changes Start */
/* SPR 19288 change start */
                lteMacUpdateStatsPerfEnd(execFlowAndindex.cellIndex);
/* SPR 19288 change end */
                /* CA Stats Changes End */
                currentTick = getCurrentTick();
            }
#ifndef FLEXRAN	    
            receivePhyMsg = 20;
            /* Porting Changes End*/
            timeout.tv_sec = 1;
            timeout.tv_nsec = 0;
#endif
/* SPR 19288 change start */
#if   FLEXRAN
			flexran_ipc_wait_phy_ready(execFlowAndindex.cellIndex);
#else
            retval = pselect_wrapper(phyRxSockFD_g[execFlowAndindex.cellIndex] + 1, &readfds, PNULL, PNULL, &timeout, PNULL);
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
            /* SPR 4915 : Start*/
#ifndef FLEXRAN
            if(retval)
#endif
            {
/* SPR 19288 change start */
/*SPR 21001 Changes start */

#ifdef FLEXRAN

				UInt8 fapiShmaChId;
				/* number of messages read on a channel in a particular iteration */
				UInt8 msgCountForChannel = 0;

				/* Denotes if buffer is being allocated first time in this receive loop */
				UInt8 firstBufferAllocation = TRUE;
				void *msgBuf_p = PNULL;
				void *msg_p    = PNULL;

				for (fapiShmaChId = FAPI_SHMA_CH_ERROR_IND; fapiShmaChId <= FAPI_SHMA_CH_RX_CQI_IND; fapiShmaChId++)
				{
                                        firstBufferAllocation = TRUE;
					SInt32 ret = 0;
					//FAPI_l1ApiMsg_st* pL1ApiMsg = NULL;
					//uint32_t fapiLength = 0;

					/* check if there was no msg on last scannel channel*/ 
					if(msgCountForChannel == 0 && firstBufferAllocation != TRUE)
					{
						//no Buffer allocation, let it write on previously allocated buffer as that is still unused.
					}
					else
					{
                                                recvPhyDataQueueNode_p = NULL;
						//New Buffer allocation for this channel's msg
						GET_MEM_FROM_POOL(ReceivePhyDataQueueNode,
						                  recvPhyDataQueueNode_p,
								          sizeof(ReceivePhyDataQueueNode),PNULL);
						if(PNULL == recvPhyDataQueueNode_p)
						{
							continue;
						}

                                                recvPhyDataQueueNode_p->recvDataBuffer_p = NULL;

						msgBuf_p = msgAlloc(PNULL,0,0,0);
						if( PNULL == msgBuf_p)
						{
							freeMemPool(recvPhyDataQueueNode_p);
							recvPhyDataQueueNode_p = PNULL;
							continue;
						}
						msg_p = msgReserve(msgBuf_p,0,BUFFLEN);
						if (PNULL == msg_p )
						{

							freeMemPool(recvPhyDataQueueNode_p);
							recvPhyDataQueueNode_p = PNULL;
							
							msgFree( msgBuf_p);
							msgBuf_p = PNULL;
							lteWarning("Error In msgReserve\n");
							continue;
						}

						firstBufferAllocation = FALSE;

						/* Start the msg count for this channel */ 
						msgCountForChannel = 0;
					}
                                        UInt8 msgCountSFI = 0;
					while(1) //use while loop to flush old data
					{
						ret = flexran_ipc_recv_msg(execFlowAndindex.cellIndex,fapiShmaChId,msg_p,FLEXRAN_MAX_MSG_SIZE);

						if (ret <= 0)
						{
							/* Nothing on channel OR error in reading */
							break;
						}
						else
						{
						    msgCountForChannel++;
						    recvPhyDataQueueNode_p->byteRead         = ret;
						    recvPhyDataQueueNode_p->recvDataBuffer_p = msgBuf_p;
						    if (FAPI_SHMA_CH_SUBFRAME_IND == fapiShmaChId)
						    {
							msgCountSFI++;
						    }

#ifdef FLEXRAN_FAPI_LOGGING
						    if(0 != flexranFapiLoggingEnable_g)
						    {
							    do_tap_fun(msg_p, ret,execFlowAndindex.cellIndex );
						    }
#endif
						}

						//ysl add 0726: for subframe.ind, if there are more than one at one time,
						//              process them one by one instead of drop first one(s)
						//note every subframe.ind is accompanied by a PHY semaphore
						if (FAPI_SHMA_CH_SUBFRAME_IND == fapiShmaChId)
						{
							break;
						}
					}
					if (msgCountSFI > 1)
					{
						fprintf(stderr,"\nOvershoot by %d ticks on cell %d\n",(msgCountSFI - 1),execFlowAndindex.cellIndex);
					}
					if (msgCountForChannel == 0)
					{
						//means there was no msg for last scanned channel
						// Free the buffer allocated for last channel messages

						if (recvPhyDataQueueNode_p)
						{
							if(recvPhyDataQueueNode_p->recvDataBuffer_p)
							{
								msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
								recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
							}
							freeMemPool(recvPhyDataQueueNode_p);
							recvPhyDataQueueNode_p = PNULL;
							msgFree( msgBuf_p);
							msgBuf_p = PNULL;
						}
						//nothing on this channel,scan next channel
						continue;
					}
					else
					{
						// push the data node in Local queue
						if(pushNode(recvDataFromPhyQ_gp[execFlowAndindex.cellIndex], 
									&recvPhyDataQueueNode_p->nodeAnchor))
						{
							if(recvPhyDataQueueNode_p->recvDataBuffer_p)
							{
								msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
								recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
							}
							freeMemPool(recvPhyDataQueueNode_p);
							recvPhyDataQueueNode_p = PNULL;

						}

					}

					// save the data in pointer pL1ApiMsg ....
				}
#else	
                if (lteFD_ISSET(phyRxSockFD_g[execFlowAndindex.cellIndex], &readfds))
                {
                    /* Porting Changes Start*/
                    while(receivePhyMsg--)
                    {
                        /* Porting Changes End*/
                        /* SPR 5599 changes start (ZIP ID 129245) */            
                        /* + SPR 7521 + */
                        //Removed currentTick check as it wasnt required
                        /* - SPR 7521 - */
                        /* SPR 5599 changes start (ZIP ID 129245) */            

                        /* RX_IND_PORTING Changes */
                        GET_MEM_FROM_POOL(ReceivePhyDataQueueNode,recvPhyDataQueueNode_p,
                                sizeof(ReceivePhyDataQueueNode), PNULL);
                        /*++klockwork warning */
                        if(PNULL == recvPhyDataQueueNode_p)
                        {
                            continue;
                        }
                        /*--klockwork warning */
                        recvPhyDataQueueNode_p->recvDataBuffer_p
                            = (UInt8 *)receiveDataFromPhy(&(recvPhyDataQueueNode_p->byteRead),
                                    execFlowAndindex.cellIndex);
                        if( 0 >= recvPhyDataQueueNode_p->byteRead )
                        {
                            if(recvPhyDataQueueNode_p->recvDataBuffer_p)
                            {
                                msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
                                recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
                            }
                            freeMemPool(recvPhyDataQueueNode_p);
                            recvPhyDataQueueNode_p = PNULL;
                            /* Porting Changes Start*/
                            break;
                        }
                        /*CA Changes start  */
                        /* + Coverity Id - 63446 */ 
                        if(pushNode(recvDataFromPhyQ_gp[execFlowAndindex.cellIndex], 
                                    &recvPhyDataQueueNode_p->nodeAnchor))
                        {
                            if(recvPhyDataQueueNode_p->recvDataBuffer_p)
                            {
                                msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
                                recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
                            }
                            freeMemPool(recvPhyDataQueueNode_p);
                            recvPhyDataQueueNode_p = PNULL;

                        }
                        /* - Coverity Id - 63446 */
                        /*CA Changes end  */
                        /* RX_IND_PORTING Changes */
                    }
                    /* Porting Changes End*/
                }
/*SPR 21001 Changes end */
#endif
/* SPR 19288 change end */
            }
                    /* Porting Changes End*/
            /* SPR 4915 : End*/
        }
    }
} 
#endif

/* SPR 8712 changes */
#ifdef DL_DATA_SEPARATION
#if defined DL_UL_SPLIT
#define ENCODER_SCH_EL_COUNT_MAX   6
#else
#define ENCODER_SCH_EL_COUNT_MAX   5
#endif
/* SPR 8712 changes */

/****************************************************************************
 * Function Name  : encoderScheduler 
 * Inputs         : param_p
 * Outputs        : None
 * Returns        : NULL
 * Description    : This function is initialization function for  
 *                  thread.
 ****************************************************************************/
void* encoderScheduler(void *param_p)
{
    ExecutionLegFunctionArg execLegFuncArg={0};
    char logger_file_name[LTE_MAX_FILE_NAME_SIZE] = {0};

    InternalCellIndex internalCellIndex = *((InternalCellIndex *)param_p);

    THREAD_TO_BIND_SET_CORE_NO(threadSelf(), 
                  lteLayer2CoreInfo_g.lteLayer2EncoderCoreNum[internalCellIndex]);
#ifdef FLEXRAN
    if (internalCellIndex)
    {
         prctl(PR_SET_NAME, "lte_encoder_1", NULL, NULL, NULL);
    }
    else
    {
         prctl(PR_SET_NAME, "lte_encoder_0", NULL, NULL, NULL);
    }
#endif
    threadSetRtPriority(SCHED_FIFO, 99); 

    execLegFuncArg.globalTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);

    /*CA Changes start  */
    getSFAndSFN(&execLegFuncArg.sysFrame, &execLegFuncArg.subFrame,internalCellIndex);
    /*CA Changes end  */
    execLegFuncArg.delayToPhy = PHY_DL_DELAY;
    UInt32 name_len = LTE_MAX_FILE_NAME_SIZE-strlen_wrapper("MAC_RLC_ENCODER_REGION")-1;
    /* +SPR_10381 Changes */
    prepareEncoderLoggerFileName (logger_file_name, name_len, "MAC_RLC_ENCODER_REGION", internalCellIndex);
    logger_file_name[LTE_MAX_FILE_NAME_SIZE-1]= '\0';
    logger_init((LTE_L2_LOG_ENCODER_REGION_ID ), ( Char8*)logger_file_name, l2Input.shmSize);

    startEncoderLogReader(&l2Input, ( Char8*)logger_file_name);
    /* -SPR_10381 Changes */

    semInitDefault(&waitForPdschInd_g[internalCellIndex], 0);
/* SPR 8712 changes */
    /* wait for cell setup indication */
    semWaitDefault(&waitForCellSetupIndSemEncThrd[internalCellIndex]);
/* SPR 8712 changes */
    while(1)
    {
        encoderSchedulerFlowThread((void*)&(executionFlowCreationParamArrForDLDataThread_g[0]\
                                   .threadArguments),internalCellIndex);
    }
}

/*****************************************************************************
 * Function Name  : encoderSchedulerFlowThread
 * Inputs         : executionFlowThreadArgs_p, internalCellIndex
 * Outputs        : None
 * Returns        : NULL
 * Description    : The thread executes execution legs passed to it through
 *                  arguments.
 *****************************************************************************/
/* CA changes start */
extern SInt32 ev_fd_g[MAX_NUM_CELL];
void* encoderSchedulerFlowThread(void * executionFlowThreadArgs_p,
                                  InternalCellIndex internalCellIndex)
/* CA changes end */
{
    eventFD_t tempfd;
    UInt32 executionLegCount =0;
    ExecutionLegFunctionArg execLegFuncArg = {0};
    EncoderExecutionFlowArguments * threadArg_p =
                     (EncoderExecutionFlowArguments *)executionFlowThreadArgs_p;
    ExecutionLegInfo * executionLegArr_p = threadArg_p->executionLegArr;
    /* CA changes start */
    cellIndexMac_g = internalCellIndex;
    execLegFuncArg.cellIndex = internalCellIndex;
    /* CA changes end */
    execLegFuncArg.execFlowIndex = threadArg_p->threadIndex;
    execLegFuncArg.delayToPhy = threadArg_p->dlPathDelay;

/* SPR 8712 changes */
    UInt32 oppIndCnt = 0;
    while(!oppIndCnt)
    {
        eventfd_read_wrapper(ev_fd_g[internalCellIndex],&tempfd);
        oppIndCnt = COUNT_MAC_UE_OPPOR_IND_CIRC_Q(internalCellIndex);
    }
/* SPR 8712 changes */

    /* CA Stats Changes Start */
    lteMacUpdateDLDataSepStatsPerfStart(internalCellIndex);
    /* CA Stats Changes End */

    while(executionLegCount < ENCODER_SCH_EL_COUNT_MAX )
    { 
        execLegFuncArg.execLegSpecificArg1 = 
            executionLegArr_p[executionLegCount].legSpecificArg1;

        execLegFuncArg.execLegSpecificArg2 = 
                            executionLegArr_p[executionLegCount].legSpecificArg2;
        RDTSC_AND_CLOCK_START_ENCODER(executionLegCount);
#ifdef FDD_CONFIG
        executionFunctionArr_g[executionLegArr_p[executionLegCount]
                            .executionLeg] (&execLegFuncArg);

#elif TDD_CONFIG
        executionFunctionArrTdd_g[executionLegArr_p[executionLegCount]
                            .executionLeg] (&execLegFuncArg);

#endif                            
        /* CA Stats Changes Start */
        RDTSC_AND_CLOCK_END_ENCODER(executionLegCount, internalCellIndex);
        executionLegCount++;
    }

    lteMacUpdateEncoderStatsPerfEnd(internalCellIndex);
    /* CA Stats Changes End */
        
    return PNULL;
}
#endif

/* Memory cleanup changes start*/
/*****************************************************************************
 * Function Name  : cleanupLayer2StartupQueues
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function cleanup all the queues, created at the time 
 *                  of L2 startup.
 *****************************************************************************/
void cleanupLayer2StartupQueues(InternalCellIndex internalCellIndex)
{

    UInt64 countNode=0;
    ReceivePhyDataQueueNode *recvPhyDataQueueNode_p = PNULL;

    /*cleaup PHY Receiver Queue, created at initLayer2 function */
    countNode=sQueueCount(recvDataFromPhyQ_gp[internalCellIndex]);

    while(countNode--)
    {
         recvPhyDataQueueNode_p = (ReceivePhyDataQueueNode *)popNode(recvDataFromPhyQ_gp[internalCellIndex]);
         if(recvPhyDataQueueNode_p)
         {
             if(recvPhyDataQueueNode_p->recvDataBuffer_p)
             {
                 msgFree(recvPhyDataQueueNode_p->recvDataBuffer_p);
                 recvPhyDataQueueNode_p->recvDataBuffer_p = PNULL;
             }
             freeMemPool(recvPhyDataQueueNode_p);
             recvPhyDataQueueNode_p = PNULL;
         }
    }
}    
/* Memory cleanup changes end*/
