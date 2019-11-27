/******************************************************************************
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpExecutionLegs.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : The PDCP layer Execution Legs implementation 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpExecutionLegs.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.8.2.5  2009/07/20 08:21:56  gur19479
 * Changes done for SRB handling
 *
 * Revision 1.8.2.4  2009/07/13 12:14:10  gur19479
 *
 * Changed NULL to PNULL
 *
 * Revision 1.8.2.3  2009/07/13 11:03:41  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.8.2.2  2009/07/07 11:37:15  gur19140
 * *** empty log message ***
 *
 * Revision 1.8.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.7  2009/05/21 07:10:46  gur19140
 * memory leak fixes
 *
 * Revision 1.6  2009/05/20 13:40:56  gur20548
 * header file name changed
 *
 * Revision 1.5  2009/05/15 10:05:18  gur19836
 * CR Comments Incorporated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpLteFramework.h"
#include "ltePdcpExecutionLegs.h"
#include "ltePdcpStatistic.h"
#include "ltePdcpRlcSap.h"
#include "ltePdcpPdcpUserSap.h"
#include "ltePdcpTxQueues.h"
#include "ltePdcpRxQueues.h"
#include "ltePdcpDataTransferTx.h"
#include "ltePdcpDataTransferRx.h"
#include "ltePdcpDiscardTimer.h"
#include "ltePdcpExtScheduler.h"
#include "ltePdcpProtocolTools.h"
#include "logging.h"
#include "alarm.h"
#include "ltePdcpOamSap.h"
#include "ltePdcpRrcSap.h"
#include "ltePdcpTlvUtils.h"
#include "lteOamInterface.h"
#include "ltePdcpErrors.h"
/*Warning Fix*/
#include "ltePdcpOamSapCommands.h"

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
#define RLC_FAILURE 0

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern PdcpContext pdcpContext;
extern tickType_t pdcpCurrentTime_g;
extern void rohcProcessTimers(void);

/*SPR 16855 +-*/
#ifdef L2_FAST_PATH
extern UInt8 pdcpIsSrbDataReceivedInTxQ_g;
extern UInt8 pdcpIsDrbDataReceivedInTxQ_g;
#endif
#ifdef UE_SIM_TESTING
extern LTE_CIRCQUEUE rlcRxUePendingDelQ_g;
extern LTE_CIRCQUEUE rlcTxUePendingDelQ_g;
extern UInt32 rlcHdlrPenUeDelQ(LTE_CIRCQUEUE *rlcUePendingDelQ_p);
extern UInt32 rlcHdlrPenDelQ(LTE_SQUEUE *pendDelQ_p);
#endif 
/* SPR 15909 fix start */
extern tickType_t relayGtpuCurrentTick_g;
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
#ifdef PDCP_ASYNC_INTERFACE
ULong32 pdcpAsyncExecutionSchedulerThId_g;
#endif
#if defined PDCP_ASYNC_INTERFACE && defined PDCP_ASYNC_INTR
extern PdcpSecConfig pdcpSecConfig_g;
#endif

/* SPR 5449 changes start */
#ifdef PERF_STATS
extern UInt8 ueResponseBuf_g[40000];
#endif
/* SPR 5449 changes end */

extern UInt16 rlcLayerInitialized_g;
extern UInt8 thr_count_g;
extern UInt32 threadId_g[MAX_THREAD_ID];
/*SPR 21545 Start*/
extern UInt8 instanceL2_g;
/*SPR 21545 End*/
extern MAC_UE_DATA_IND_TYPE macUeDataIndQ_g[MAX_NUM_CELL];

/* RLC Queues for Pending Deletion and Reestablishment of Entities */
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
extern LTE_CIRCQUEUE rlcRxPendingDelQ_g;
extern LTE_CIRCQUEUE rlcRxReEstabQ_g;
extern LTE_CIRCQUEUE rlcTxPendingDelQ_g[MAX_NUM_CELL];
extern LTE_CIRCQUEUE rlcTxReEstabQ_g[MAX_NUM_CELL];
extern LTE_CIRCQUEUE rlcReconfigTxEntityQ_g;
extern LTE_CIRCQUEUE rlcReconfigRxEntityQ_g;
extern UInt32 rlcHandleDataFromMac(UInt32 count,InternalCellIndex internalCellIndex);
extern UInt32 rlcProcessReEstabMsg(LTE_CIRCQUEUE *reEstabQ_p);
extern UInt32 rlcHdlrReEstabQ(LTE_SQUEUE *reEstabQ_p);
/* +- SPR 21950 */
/*SPR 21188 +*/
static inline void processPdcpRlcQueuesForNcountFifty(UInt32 *ncount);
/* Coverity 107121 Fix Start */
/* Coverity 107121 Fix End */
static inline void processesSuccessfulRlcPdcpInitialization(void);
static inline void handlePktFromRLC(void);
STATIC inline void handleULPktFromMac(void);
/* Coverity 107121 Fix Start */
/* Coverity 107121 Fix End */
extern void initPacketRelay(void);
extern UInt32 initPDCPLayer (void);
extern void initPdcpAdapter(void);
extern void handleUserData(void);
extern void egtpu_init (void);
extern RLC_TRIGGER_STATUS_REPORT_Q rlcTriggerStatusReportQueue_g[MAX_NUM_CELL]; 
extern void expireUMROTimer(InternalCellIndex internalCellIndex);
extern void expireAMROTimer(InternalCellIndex internalCellIndex);
extern UInt8 numOfRlcCellConfigured_g;
#endif
void freePendingMemoryPools(InternalCellIndex internalCellIndex);
extern void preparePdcpShmRegionName(Char8 * shmRegionName, UInt32 name_len,
        Char8 *str, SInt32 instanceL2);
extern UInt32 rlcProcessRxReconfigMsg(LTE_CIRCQUEUE *rlcReconfigRxEntityQ_p);
/* SPR_9432_Fix Start */
/* + SPR 17439 */
extern void egtpu_check_echo_timer_expiry(void);
/* - SPR 17439 */
/* SPR_9432_Fix End */
extern void *pdcpAsyncExecutionSchedular( void *param );
extern UInt32 rlcProcessReEstabMsg(LTE_CIRCQUEUE *reEstabQ_p);
void processRlcTriggerStatusReport(UInt32 statusReportsQueueCount,InternalCellIndex cellIndex);

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
extern UInt32 rlcProcessTxReconfigMsg(LTE_CIRCQUEUE *rlcReconfigTxEntityQ_p);
/*SPR 15692 fix start*/
extern UInt8 pdcpNumQciEntries( UInt8 moduleId );
/*SPR 15692 fix end*/
#endif

#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
/* + SPR 17439 */
extern void lteMacUpdateKpiStatsIPThpUlOnDequeue(void);
/* - SPR 17439 */
#endif
/* SPR 21804 Fix Start */
#if defined(LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
extern void normalModeForMacExecSchedular(void); 
#endif


/*+ SPR 21121*/
extern LTE_CIRCQUEUE rlcUlSchedToLPCircQ_g;
extern LTE_CIRCQUEUE rlcLPtoRlcUlSchedCircQ_g;
typedef struct RlcRrcOamHiPrioQNodeT
{
    UInt8 rlcState;
    UInt8 processRrcOamMsgEvent;
    /* CA Changes start */
    RrcCellIndex cellId;
    UInt16 transactionId;
    /* CA Changes end */
    UInt16 ueIndex;
}RlcRrcOamHiPrioQNode;
/*- SPR 21121*/
#ifdef ENDC_ENABLED
extern void handlePdcpDataFromX2u();
#endif
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* State machine to handle data packets for SRB and DRB at TX side */
pdcpHandlerFuncT pdcpTxStateMachine[PDCP_TX_MAX_STATE][PDCP_MAX_TX_EVENT] =
{
    {   /* Idle State */
        pdcpLogandIgnore,
        pdcpLogandIgnore,
        pdcpLogandIgnore,
        pdcpLogandIgnore,
    },
    {   /* Connected State */
        pdcpProcessSrbTx,
        pdcpProcessDrbTx,
        pdcpLogandIgnore,
        pdcpProcessDataFwdTx,
    },
    {   /* Suspended State */
        pdcpLogandIgnore,
        pdcpLogandIgnore,
        pdcpLogandIgnore,
        pdcpLogandIgnore,
    },
    {   /* Resume State */
        pdcpProcessSrbTx,
        pdcpRetransmitTx,
        pdcpLogandIgnore,
        pdcpLogandIgnore,
    },
    {   /* HO State */
        pdcpProcessSrbTx,
        pdcpLogandIgnore,
        pdcpFwdDlFreshSdu,
        pdcpLogandIgnore,
    },
    {   /* Awaited State */
        pdcpProcessSrbTx,
        pdcpLogandIgnore,
        pdcpLogandIgnore,
        pdcpLogandIgnore,
    }
};

/* State machine to handle data packets for SRB and DRB at RX side */
pdcpHandlerFuncT pdcpRxStateMachine[PDCP_RX_MAX_STATE][PDCP_MAX_RX_EVENT] =
{
    {   /* Idle State */
        pdcpLogandIgnore,
        pdcpLogandIgnore,
    },
    {   /* Connected State */
        pdcpProcessSrbRx,
        pdcpProcessDrbRx,
    },
    {   /* HO State */
        pdcpProcessSrbRx,
        /* SPR 2739 changes start */
        pdcpProcessDrbRx,
        /* SPR 2739 changes end */
    }
};

extern LTE_SQUEUE pdcpTxControlPduIndQ_g;

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpAdapterThreadExecutionSchedular
 * Inputs         : void * param
 * Outputs        : None
 * Returns        : None
 * Description    : This function is initialization function for PDCP Adapter 
 *                  thread.
 ****************************************************************************/
extern UInt32 lastTickSent_g;
#ifdef MSPD_WATCH_DOG
ULong32 pdcpThdId;
ULong32 pdcpPId;
#endif
extern void rlcPdcpRateStastics(void);

/* +- SPR 17777 */
void* pdcpAdapterThreadExecutionSchedular(void *param)
/* +- SPR 17777 */
{
    LTE_GCC_UNUSED_PARAM(param);
    UInt32 ncount = 0;
    UInt8 fCount = 0;
#if !defined (RLC_PDCP_IPC_INTF ) || defined (PDCP_ASYNC_INTERFACE)
    UInt32 sQcount = 0;
#endif
#if !defined (L2_FAST_PATH) || !defined (DL_UL_SPLIT)|| defined (PDCP_ASYNC_INTERFACE)
    UInt8 cellCount = 0;
#endif
    InternalCellIndex cellIndex = 0;
#ifdef PDCP_ASYNC_INTERFACE
    SInt32 retTh = 0;
#endif
#ifdef MSPD_WATCH_DOG
        pdcpThdId=threadSelf();
        pdcpPId=syscall(SYS_gettid);
#endif
    /* SPR 4422 Fix Start */
    if (thr_count_g < MAX_THREAD_ID)
    {
        threadId_g[thr_count_g] = threadSelf();
        thr_count_g++;
    }
    /* SPR 4422 Fix End */
#ifdef LOG_PRINT_ENABLED

/*SPR 21653 changes start*/
    logger_init(LTE_L2_LOG_REGION_RLC_PDCP_ID, LTE_L2_LOG_REGION_TWO_NAME, l2Input.shmSize );
/*SPR 21653 changes end*/
    /* SPR 11453 Changes End */
#endif
    /*SPR 16855 +-*/
    /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
    /*SPR 21188 -*/
    initPdcpAdapter();
    egtpu_init();
#endif
    /* SPR 2991 change start */
    PROCESS_TO_BIND_SET_CORE_NO(Syscall_SYS_gettid(),
            lteLayer2CoreInfo_g.lteLayer2PdcpCoreNum);
    /*SPR 2991 change end*/

    /*SPR 2253 changes end */
    //Set priority of this thread lower than the main thread
#ifdef LAYER2_PHYSIM_THREAD
    threadSetRtPriority(SCHED_FIFO, 45); 
#else
#ifdef LAYER2_DLTG_THREAD
    threadSetRtPriority(SCHED_FIFO, 15); 
#else
    threadSetRtPriority(SCHED_FIFO, 20); 
#endif	
#endif

#ifdef PDCP_ASYNC_INTERFACE
    retTh = threadCreate( &pdcpAsyncExecutionSchedulerThId_g, 
            THREAD_CREATE_DETACHED, THREAD_SCHED_DEFAULT, 0, 
            THREAD_INHERIT_SCHED_DEFAULT, THREAD_SCOPE_SYSTEM,
            pdcpAsyncExecutionSchedular, PNULL);
    if( 0 != retTh ) 
    {
        ltePanic(" Unable to initialise the Layer2.\n");
        return PDCP_FAIL; 
    }
#endif
    while(1)
    {
#ifdef DL_UL_SPLIT
        if(!gMacStats.pStats)
        {
            uSleep(2);
            continue;
        }
#endif
		rlcPdcpRateStastics();

        l2ThreadHealthStoreTick_g.pdcpThread[PDCP_THREAD] = pdcpCurrentTime_g;
        /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
        /*SPR 21188 +*/
        l2ThreadHealthStoreTick_g.egtpuThread[GTPU_THREAD] = 
            relayGtpuCurrentTick_g;
#endif
#ifndef DL_UL_SPLIT
/*SPR 21727 Changes Start*/
#ifndef RLC_PDCP_IPC_INTF
/*SPR 21727 Changes End*/
        /* Update the ULRLC thread for all cells */
        for (cellIndex = 0; cellIndex< MAX_NUM_CELL; cellIndex++)
        {
            l2ThreadHealthStoreTick_g.rlcMacThread[cellIndex].rlcThread[RLC_UL_THREAD] = globalTTITickCount_g;
        }
#endif
#endif

        /*SPR 3774 change start */
        /* +- SPR 15075 */
        /* non blocking adapter function call to recv packet on UDP socket */
        RDTSC_AND_CLOCK_START_FDD(NORMAL_MODE_FOR_MAC_EXEC_SCH );

/* SPR 21804 Fix Start */
#if defined(LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
        normalModeForMacExecSchedular();
#else           
        handleUserData();         
#endif


        for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
        {
#ifdef DL_UL_SPLIT
            RDTSC_AND_CLOCK_END_PDCP(NORMAL_MODE_FOR_MAC_EXEC_SCH, cellCount);
#else
            RDTSC_AND_CLOCK_END_MAC(NORMAL_MODE_FOR_MAC_EXEC_SCH, cellCount);
#endif
        }
        /* CA Stats Changes End */
        /* +- SPR 15075 */
        /* Receive packet from PDCP Adapter */
        if(RLC_FAILURE != rlcLayerInitialized_g 
                && pdcpContextIsInitialized()
                )
        {
            processesSuccessfulRlcPdcpInitialization();
        }   

        /* +- SPR 15075 */


        /* + SPR 9911 Fix */
/*SPR 21727 Changes Start*/
#if !(DL_UL_SPLIT) && !(RLC_PDCP_IPC_INTF)    
/*SPR 21727 Changes End*/
        if((RLC_FAILURE != rlcLayerInitialized_g &&
                    pdcpContextIsInitialized()) )
        {
            for( cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
            {
                UInt32 cQcount = COUNT_RLC_TRIGGER_STATUS_REPORT_Q(cellCount);
                if(cQcount)
                    processRlcTriggerStatusReport(cQcount,cellCount);
            }
        }
#endif
#ifndef RLC_PDCP_IPC_INTF
        /* SPR 21121 Fix Start */
        sQcount = COUNT_RLC_LP_TO_RLC_UL_SCH_Q();
        while(sQcount--)
        {
            RlcRrcOamHiPrioQNode *recvMsgBuf_p  = PNULL;
            DEQUEUE_RLC_LP_TO_RLC_UL_SCH_Q(RlcRrcOamHiPrioQNode, (void**)&recvMsgBuf_p);
            if(recvMsgBuf_p)
            {
                if(!(ENQUEUE_RLC_UL_SCH_TO_LP_Q(RlcRrcOamHiPrioQNode, recvMsgBuf_p)))
                {
                    fprintf(stderr," Failed to Enqueue node in rlcUlSchedToLPCircQ_g for UE[%d]\n", recvMsgBuf_p->ueIndex);
                }
            }
        }
#endif
        /* SPR 21121 Fix End */
        /*RLC_PDCP: Handle uplink packet from MAC */
        handleULPktFromMac();
        /* RLC_PDCP: Handle packet from RLC */
        handlePktFromRLC();

#ifdef PDCP_ASYNC_INTERFACE
        sQcount = sQueueCount(&pdcpRxDecipheredQ_g );
        if ( sQcount )
        {
            RDTSC_AND_CLOCK_START_FDD(PDCP_RX_PROCESS_DECIPHERED_PDU);
            pdcpElRxProcessDecipheredPdu( sQcount );
            for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
            {
                RDTSC_AND_CLOCK_END_PDCP(PDCP_RX_PROCESS_DECIPHERED_PDU, 
                        cellCount); 
            }
        }
        RDTSC_AND_CLOCK_START_FDD(PDCP_POLL_SECURITY_ENGINE);
        pdcpElPollSecurityEngine();
        for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
        {
            RDTSC_AND_CLOCK_END_PDCP(PDCP_POLL_SECURITY_ENGINE, cellCount);
        }
#endif

        /* SPR 13825 fix start */
        /* If number of UE < 4 call forwarding leg each time else call this leg
         * once in 4 ticks */
        if ( 4 > pdcpNumActiveUe() )
        {
            /* Check for forwarded packets leg */
            pdcpElTxProcessFwdedMessage();
        }
        else
        {
            fCount++;
            if ( 0 == (fCount % 4) )
            {
                /* Check for forwarded packets leg */
                pdcpElTxProcessFwdedMessage();
                fCount = 0;
            }
        }
        /* SPR 13825 fix end */
        /*SPR 21188 +*/

        /* SPR [BUG 879] FIX START */
        /*SPR 16855 +-*/
/*SPR 21727 Changes Start*/
#ifndef RLC_PDCP_IPC_INTF
/*SPR 21727 Changes End*/
        for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
        {
            RDTSC_AND_CLOCK_START_FDD(RLC_HDLR_REESTABLISH_Q);
            if ( circQueueCount(&rlcTxReEstabQ_g[cellCount]))
            {
                rlcProcessReEstabMsg(&rlcTxReEstabQ_g[cellCount]);
            }
            RDTSC_AND_CLOCK_END_RLC(RLC_HDLR_REESTABLISH_Q,cellCount);
        }
        RDTSC_AND_CLOCK_START_FDD(RLC_HDLR_REESTABLISH_Q);
        if ( circQueueCount(&rlcRxReEstabQ_g) )
        {
            rlcProcessReEstabMsg(&rlcRxReEstabQ_g); 
        }
        RDTSC_AND_CLOCK_END_RLC(RLC_HDLR_REESTABLISH_Q,0/*cellCount*/); 
        /* SPR [BUG 879] FIX END */

#endif
        if (ncount == 50)
        {
            processPdcpRlcQueuesForNcountFifty(&ncount);

        }       
/*SPR 21727 Changes Start*/
#if !(DL_UL_SPLIT) && !(RLC_PDCP_IPC_INTF) 
/*SPR 21727 Changes End*/
        RDTSC_AND_CLOCK_START_FDD(RLC_TIMER_UPD_TICKCHECK_FOR_TIMER_EXE);
        /* ----SPR 3625 start--- */
        for(cellIndex = 0;cellIndex < numOfRlcCellConfigured_g;cellIndex++)
        {
            expireUMROTimer(cellIndex);
            expireAMROTimer(cellIndex);
        }

        /* ----SPR 3625 end--- */
        for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
        {
            RDTSC_AND_CLOCK_END_RLC(RLC_TIMER_UPD_TICKCHECK_FOR_TIMER_EXE, 
                    cellCount);
        }
#endif

        /* SPR_9432_Fix Start */
        /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
        /*SPR 21188 -*/
        /* This func is moved from UL_Thread to Main_Thread.
         * To reduce overload on UL Thread */
#ifndef __x86_64__
        egtpu_check_echo_timer_expiry();
#endif
#endif
        /* SPR_9432_Fix End */
        for(cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
        {
            freePendingMemoryPools(cellIndex);
        }
        /* SPR 7914 changes end */

        ncount++;
        /* +- SPR  13888 */
#ifdef KPI_STATS
        /* Dequeue the Ue Id and Lc Id from the Circular Queue and update the
         * Shared Memory */
        lteMacUpdateKpiStatsIPThpUlOnDequeue();
#endif
    }
}

/*****************************************************************************
 * Function Name  : processesSuccessfulRlcPdcpInitialization
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes Successful RLC PDCP Initialization.
 *****************************************************************************/
/* + SPR 17439 */
static inline void processesSuccessfulRlcPdcpInitialization(void)
/* - SPR 17439 */
{    
    UInt32 sQcount = 0;
    UInt8 cellCount = 0;
/*SPR 21773 +- :CID 114622*/
  /*
     ** SPR_1195_FIX_START: PDCP Control PDU indications shall be
     ** processed at High priority basis. So process all available
     ** PDCP Control PDU indications here.
     */
    /* 
     ** SPR 1781 changes start: On RDB board the function 
     ** pdcpGetControlPduCount() was returning 0, due to
     ** optimization as the prototype was not declared in
     ** our code base, as was being assumed to return U32 instead 
     ** of U64 by the compiler. To fix this, the function 
     ** pdcpGetControlPduCount() and its invocations have 
     ** been removed from the code which was not required as such.
     ** This is to fix the problem as well as optimize the procesing
     ** to remove extra invocation of this call.
     */
    sQcount =  sQueueCount((LTE_SQUEUE*)&pdcpTxControlPduIndQ_g);
    if(sQcount != 0)
    {
        RDTSC_AND_CLOCK_START_FDD(PDCP_PROCESS_TX_CNTRL_PDU_IND);
        pdcpElTxProcessControlPduInd(sQcount);
        for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
        {
            RDTSC_AND_CLOCK_END_PDCP(PDCP_PROCESS_TX_CNTRL_PDU_IND, cellCount);
        }
        /* CA Stats Changes End */
    }
    /* SPR_1195_FIX_END */

    /* +- SPR 15075 */
    RDTSC_AND_CLOCK_START_FDD(PDCP_PROCESS_DRB_TX);
    pdcpElTxProcessDrbMessage();
    for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
    {
        RDTSC_AND_CLOCK_END_PDCP(PDCP_PROCESS_DRB_TX, cellCount);
    }
/*SPR 21727 Changes Start*/
/*SPR 21773 +- :CID 114622*/
/*SPR 21727 Changes Start*/
    /*Execution Leg to handle harq failure indication from MAC Layer*/
#if OBSELETE_REL_8 && !(RLC_PDCP_IPC_INTF)
/*SPR 21727 Changes End*/
    /*coverity-530-32868 */
#ifndef DL_UL_SPLIT
    sQcount = sQueueCount(&macUeHarqFailureIndQ_g);
    RDTSC_AND_CLOCK_START_FDD(RLC_HANDLE_HARQ_FAILURE_IND );
    if(sQcount)
    {
        rlcHandleHarqFailureInd(sQcount);
    }
    for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
    {
        RDTSC_AND_CLOCK_END_RLC(RLC_HANDLE_HARQ_FAILURE_IND, cellCount);
    }
#endif
#endif
#if defined(ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
    handlePdcpDataFromX2u();
#endif
}

/* +- SPR 15075 */
/* Coverity 107115 Fix Start */
/* Coverity 107115 Fix End */
/*****************************************************************************
 * Function Name  : handleULPktFromMac
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles Uplink Packets from MAC.
 *****************************************************************************/
/* + SPR 17439 */
STATIC inline void handleULPktFromMac(void)
/* - SPR 17439 */
{
    UInt8 cellCount = 0;
/*SPR 21727 Changes Start*/
#if  !RLC_PDCP_IPC_INTF
/*SPR 21727 Changes End*/
    UInt32 sQcount = 0;
    InternalCellIndex index = 0;
    /** SPR 13599 Changes Start **/
    for(;index < pdcpContext.sCellCount ;index++)
    {
        RDTSC_AND_CLOCK_START_FDD(RLC_HANDLE_DATA_FROM_MAC);
        sQcount = COUNT_MAC_UE_DATA_IND_Q(index);
        if(sQcount)
        {
            rlcHandleDataFromMac(sQcount,index);
        }
        RDTSC_AND_CLOCK_END_RLC(RLC_HANDLE_DATA_FROM_MAC,index);
        /** SPR 13599 Changes End **/
    }
#endif
    RDTSC_AND_CLOCK_END_RLC(RLC_HANDLE_DATA_FROM_MAC,0);
    /** SPR 13599 Changes End **/
    RDTSC_AND_CLOCK_START_FDD(PDCP_PROCESS_TX_DELIVERY_STATUS_IND);
    pdcpElTxProcessDeliveryInd();
    for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
    {
        RDTSC_AND_CLOCK_END_PDCP(PDCP_PROCESS_TX_DELIVERY_STATUS_IND, 
                cellCount);
    }
}
/* Coverity 107115 Fix Start */
/* Coverity 107115 Fix End */
/* Coverity 107124 Fix Start */
/* Coverity 107124 Fix End */
/*****************************************************************************
 * Function Name  : handlePktFromRLC
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles Packets from RLC.
 *****************************************************************************/
/* + SPR 17439 */
static inline void handlePktFromRLC(void)
/* - SPR 17439 */    
{
    UInt8 cellCount = 0;
    /* RLC_PDCP: Handle packet from RLC */
#if RLC_PDCP_IPC_INTF && !defined (UE_SIM_TESTING)
    UInt32 cQcount = 0;
    cQcount = circQueueCount((LTE_CIRCQUEUE*)&pdcpRlcDataIndQ_g);
    if(cQcount)
    {
        rlcHandleUeDataInd(cQcount);
    }
    cQcount = circQueueCount((LTE_CIRCQUEUE*)&rlcDeliveryStatusIndQ_g);
    if(cQcount)
    {
        rlcHandleDeliveryStatusInd(cQcount);
    }
#endif

    RDTSC_AND_CLOCK_START_FDD(PDCP_PROCESS_RX);
    pdcpElRxProcessMessage();
    for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
    {
        RDTSC_AND_CLOCK_END_PDCP(PDCP_PROCESS_RX, cellCount);        
    }

    RDTSC_AND_CLOCK_START_FDD(PDCP_PROCESS_SRB_TX);
    pdcpElTxProcessSrbMessage();
    for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
    {
        RDTSC_AND_CLOCK_END_PDCP(PDCP_PROCESS_SRB_TX, cellCount);                      
    }
}
/* Coverity 107124 Fix Start */
/* Coverity 107124 Fix End */
/*****************************************************************************
 * Function Name  : processPdcpRlcQueuesForNcountFifty
 * Inputs         : *ncount
 * Outputs        : None
 * Returns        : None
 * Description    : This function .
 *****************************************************************************/
static inline void processPdcpRlcQueuesForNcountFifty(UInt32 *ncount)
{
    UInt8 cellCount = 0;
    /* SPR 3054 changes start */
    RDTSC_AND_CLOCK_START_FDD(PDCP_ROHC_PROCESS_TIMERS);
    rohcProcessTimers();
    for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
    {
        RDTSC_AND_CLOCK_END_PDCP(PDCP_ROHC_PROCESS_TIMERS, cellCount);
    }
    /* SPR 3054 changes end */

    /* SPR 2241 changes start */
    RDTSC_AND_CLOCK_START_FDD(PDCP_PROCESS_DISCARD_TIMERS);
    pdcpElTxProcessDiscardTimers();
    for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
    {
        RDTSC_AND_CLOCK_END_PDCP(PDCP_PROCESS_DISCARD_TIMERS, cellCount);
    }
    /* SPR 2241 changes end */
/*SPR 21727 Changes Start*/
#if  !RLC_PDCP_IPC_INTF
/*SPR 21727 Changes End*/
#ifndef DL_UL_SPLIT
    RDTSC_AND_CLOCK_START_FDD(RLC_HDLR_PEN_DEL_Q);
    if(circQueueCount(&rlcReconfigTxEntityQ_g))
    {
        rlcProcessTxReconfigMsg(&rlcReconfigTxEntityQ_g);
    }
    for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
    {
        RDTSC_AND_CLOCK_END_RLC(RLC_HDLR_PEN_DEL_Q, cellCount);
    }

    RDTSC_AND_CLOCK_START_FDD(RLC_HDLR_PEN_DEL_Q);
    /*SPR 16855 +-*/
    if(circQueueCount(&rlcReconfigRxEntityQ_g))
    {
        rlcProcessRxReconfigMsg(&rlcReconfigRxEntityQ_g);
    }
    for(cellCount = 0; cellCount < pdcpContext.sCellCount; cellCount++)
    {
        RDTSC_AND_CLOCK_END_RLC(RLC_HDLR_PEN_DEL_Q, cellCount);
    }
#endif
#endif
    
    /* + SPR 19066 */
    /*check and audit the inactive UEs for all the cells*/
    for(cellCount = 0; cellCount < MAX_NUM_CELL; cellCount++)
    {
        pdcpElAuditSendInactiveUE(cellCount);
    }
    /* - SPR 19066 */
    
    *ncount = 0;
}
#endif /* UE_SIM_TESTING */

/****************************************************************************
 * Function Name  : pdcpElTxProcessSrbMessage  
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : UInt32
 * Description    : It takes one message form the PDCP TX queue, finds 
 *                  corresponding PDCP entity and processes it according to
 *                  rules which are defined for SRB entity.
 ****************************************************************************/
/* + SPR 17439 */
UInt32 pdcpElTxProcessSrbMessage(void)
/* - SPR 17439 */    
{
    UInt32              result   = 0;
    UInt16              ueIndex  = 0;
    UInt64              count    = 0;
    LP_PdcpUeContext    ueCtx_p  = PNULL;
    LP_PdcpEnityContext ctx_p    = PNULL;
#ifdef LOG_PRINT_ENABLED
    UInt32              state    = 0;
#endif
#ifdef L2_FAST_PATH
    if(LTE_FALSE == pdcpIsSrbDataReceivedInTxQ_g)
    {
        /* No data is available for SRB, so no need to go and check for all the
         * UES and there LCs */
        return 1;  
    } 

    /* The control comes here only in the case if SRB data is available to be 
     * processed in the Tx Q i.e. pdcpIsSrbDataReceivedInTxQ_g was TRUE.
     * So, setting it to FALSE. */

    pdcpIsSrbDataReceivedInTxQ_g = LTE_FALSE;
#endif

    while ( ueIndex < pdcpContext.numOfActiveUe)
    {
        /*
         ** activeUeInfo consists only of active UEs, hence valid UE context
         ** will always be available. Therefore, NULL check not required
         */
        /* SPR 5485 changes start */
        ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(pdcpContext.activeUeInfo[ueIndex]);

        /* Check for SRB1 */
        ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                LTE_SRB_LC_ID_LOW_BOUND,
                PDCP_ENTITY_RB_TX );       
        /* SPR 5485 changes end */
        if ( PNULL == ctx_p )
        {
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, 
                    pdcpContext.activeUeInfo[ueIndex],
                    LTE_SRB_LC_ID_LOW_BOUND, 0,0, 0,0,__func__,"");
        }
        else
        {
            count = QCOUNT_PDCP_TX_DATA_REQ_Q( ctx_p );
            while ( count-- )
            {
#ifdef LOG_PRINT_ENABLED
                state  = ueCtx_p->pdcpTxSrb1State;
#endif
                result =
                    pdcpTxStateMachine[ueCtx_p->pdcpTxSrb1State][PDCP_SRB_TX](
                            pdcpContext.activeUeInfo[ueIndex],
                            LTE_SRB_LC_ID_LOW_BOUND, ueCtx_p->pdcpTxSrb1State,
                            PDCP_SRB_TX, ctx_p);           
                LOG_PDCP_MSG( PDCP_TX_EVENT_RCVD, LOGDEBUG, PDCP_TX, pdcpCurrentTime_g,
                         __LINE__, 
                         pdcpContext.activeUeInfo[ueIndex], 
                         state, PDCP_SRB_TX, ueCtx_p->pdcpTxSrb1State, 
                         LTE_SRB_LC_ID_LOW_BOUND,0, __func__, "PdcpTxStateMachine");
            }
        }

        /* Check for SRB2 */
        /* SPR 5485 changes start */
        ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                LTE_SRB_LC_ID_HIGH_BOUND,
                PDCP_ENTITY_RB_TX );
        /* SPR 5485 changes end */
        if ( PNULL == ctx_p )
        {
            /*SPR 4390 changes start */
            LOG_UT( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, 
                    pdcpContext.activeUeInfo[ueIndex],
                    LTE_SRB_LC_ID_HIGH_BOUND, 0,0, 0,0,__func__,"");
            /*SPR 4390 changes end */
        }
        else
        {
            count = QCOUNT_PDCP_TX_DATA_REQ_Q( ctx_p );
            while ( count-- )
            {
#ifdef LOG_PRINT_ENABLED
                state  = ueCtx_p->pdcpTxSrb2DrbState;
#endif
                result =
                    pdcpTxStateMachine[ueCtx_p->pdcpTxSrb2DrbState][PDCP_SRB_TX](
                            pdcpContext.activeUeInfo[ueIndex],
                            LTE_SRB_LC_ID_HIGH_BOUND, ueCtx_p->pdcpTxSrb2DrbState,
                            PDCP_SRB_TX, ctx_p);
                LOG_PDCP_MSG( PDCP_TX_EVENT_RCVD, LOGDEBUG, PDCP_TX, pdcpCurrentTime_g,
                         __LINE__, 
                         pdcpContext.activeUeInfo[ueIndex], 
                         state, PDCP_SRB_TX,ueCtx_p->pdcpTxSrb2DrbState, 
                         LTE_SRB_LC_ID_HIGH_BOUND,0, __func__, "PdcpTxStateMachine");
            }
        }
        ueIndex++;
    }
    return result;
}

/****************************************************************************
 * Function Name  : pdcpElTxProcessFwdedMessage  
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : UInt32
 * Description    : It takes one message form the PDCP TX Fwded queue, finds 
 *                  corresponding PDCP entity and processes it according to
 *                  rules which are defined for DRB entity. This function is 
 *                  only available at target PDCP during HO
 ****************************************************************************/
/* + SPR 17439 */
UInt32 pdcpElTxProcessFwdedMessage(void)
/* - SPR 17439 */    
{
    UInt32              result   = 0;
    UInt16              ueIndex  = 0;
    UInt8               lcId     = 0;
    UInt64              count    = 0;
    LP_PdcpUeContext    ueCtx_p  = PNULL;
    LP_PdcpEnityContext ctx_p    = PNULL;
#ifdef LOG_PRINT_ENABLED
    UInt32              state    = 0;
#endif

    while ( ueIndex < pdcpContext.numOfActiveUe)
    {
       /*
       ** activeUeInfo consists only of active UEs, hence valid UE context
       ** will always be available. Therefore, NULL check not required
       */
       /* SPR 5485 changes start */
       ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(pdcpContext.activeUeInfo[ueIndex]);
       /* SPR 5485 changes end */
       
       /* Following piece of code at target side */
       count = QCOUNT_PDCP_DATA_FWDING_DL_Q(ueCtx_p);

       count = (count < MAX_FWDED_PACKET_TRANSFER_TX) ? count :
                                            MAX_FWDED_PACKET_TRANSFER_TX;
       while ( count-- )
       {
#ifdef LOG_PRINT_ENABLED
           state  = ueCtx_p->pdcpTxSrb2DrbState;
#endif
           result = pdcpTxStateMachine[ueCtx_p->pdcpTxSrb2DrbState]
                      [PDCP_TARGET_DRB_FWD_TX](
                           pdcpContext.activeUeInfo[ueIndex], 
                           0, 
                           ueCtx_p->pdcpTxSrb2DrbState, PDCP_TARGET_DRB_FWD_TX, 
                           ueCtx_p);
           LOG_PDCP_MSG( PDCP_TX_EVENT_RCVD, LOGDEBUG, PDCP_TX, pdcpCurrentTime_g,
                 __LINE__, 
                 pdcpContext.activeUeInfo[ueIndex],
                 state,PDCP_TARGET_DRB_FWD_TX,ueCtx_p->pdcpTxSrb2DrbState,
                 0,0, __func__,
                 "PdcpTxStateMachine");
       }
 
       /* Following piece of code at source side */
       if ( LTE_TRUE == ueCtx_p->isFwdingApiRecvd ) 
       {
           /* SPR 10894 fix start*/
           lcId = ueCtx_p->numOfActiveDrbLc;
           while ( 0 < lcId )
               /* SPR 10894 fix end */
           {
               /*
               ** activeLcInfo consists only of active DRBs in an UE, hence 
               ** valid PDCP context will always be available. Therefore, NULL
               ** check not required
               */
               /* SPR 5485 changes start */
               ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p,
                                             /* SPR 10894 fix start */
                                             ueCtx_p->activeLcInfo[lcId - 1], 
                                             /* SPR 10894 fix end */
                                             PDCP_ENTITY_RB_TX);
               /* SPR 5485 changes end */
               /* SPR 3225 changes start */
               /*SPR 10111 Fix Start */
               /* SPR 16770 fix start */
               if ( (LTE_FALSE == ctx_p->isEMIFwded ) && 
                       ( LTE_TRUE == ctx_p->isFwdingEnabled ) )
                   /* SPR 16770 fix end */
               /*SPR 10111 Fix End */
               /* SPR 3225 changes end */
               {
#ifdef LOG_PRINT_ENABLED
                   state = ueCtx_p->pdcpTxSrb2DrbState;
#endif
                   /* Forwards all the freshly arrivesd SDUs from Packet Relay 
                   */
                   result = pdcpTxStateMachine[ueCtx_p->pdcpTxSrb2DrbState]
                                              [PDCP_SOURCE_DRB_FWD_TX](
                                          pdcpContext.activeUeInfo[ueIndex], 
                                          /* SPR 10894 fix start */
                                          ueCtx_p->activeLcInfo[lcId - 1], 
                                          /* SPR 10894 fix end */
                                          ueCtx_p->pdcpTxSrb2DrbState,
                                          PDCP_SOURCE_DRB_FWD_TX, ctx_p );
                   LOG_PDCP_MSG( PDCP_TX_EVENT_RCVD, LOGDEBUG, PDCP_TX, pdcpCurrentTime_g,
                            __LINE__, 
                            pdcpContext.activeUeInfo[ueIndex],
                            state,PDCP_SOURCE_DRB_FWD_TX,
                            ueCtx_p->pdcpTxSrb2DrbState, ueCtx_p->activeLcInfo[lcId - 1], 0, __func__,
                            "PdcpTxStateMachine");
               }
               /* SPR 10894 fix start */
               lcId--;
               /* SPR 10894 fix end */
           }
       }
       ueIndex++;
    }
    return result;
}

/****************************************************************************
 * Function Name  : pdcpElTxProcessDrbMessage  
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : UInt32
 * Description    : It takes one message form the PDCP TX queue, finds 
 *                  corresponding PDCP entity and processes it according to
 *                  rules which are defined for DRB entity.
 ****************************************************************************/
/* + SPR 17439 */
UInt32 pdcpElTxProcessDrbMessage(void)
/* - SPR 17439 */    
{
    UInt32              result   = 0;
    UInt16              ueIndex  = 0;
    UInt8               lcId     = 0;
    UInt64              count    = 0;
    LP_PdcpUeContext    ueCtx_p  = PNULL;
    LP_PdcpEnityContext ctx_p    = PNULL;
    /* SPR 10721 */
    UInt8 storageQueueProcessed = LTE_TRUE;  
    /* SPR 10721 */
#ifdef LOG_PRINT_ENABLED
    UInt32 state = 0;
#endif
    /*
    ** SPR_1195_FIX_START: Removed invocation of 
    ** "pdcpElTxProcessControlPduInd" function from here and moved to
    ** lteMacExecutionScheduler.c file.
    */

#ifdef L2_FAST_PATH
    UInt8               pdcpIsDrbTxQEmpty = LTE_TRUE;
    if(LTE_FALSE == pdcpIsDrbDataReceivedInTxQ_g)
    {
       /* As the DRB data would have been already processed in the fast path
        * there is nothing to be processed. */
       /* We don't check the return condition of this function. */
       return result;
    }
#endif

    /* SPR_1195_FIX_END */
    while ( ueIndex < pdcpContext.numOfActiveUe)
    {
       /*
       ** activeUeInfo consists only of active UEs, hence valid UE context
       ** will always be available. Therefore, NULL check not required
       */
       /* SPR 5485 changes start */
       ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(pdcpContext.activeUeInfo[ueIndex]);
       /* SPR 5485 changes end */
       count = QCOUNT_PDCP_DATA_FWDING_DL_Q(ueCtx_p);
       if ( 0 < count )
       {
           /* 
           ** Do not process fresh SDUs coming from SGW till this queue becomes
           ** empty
           */
           LOG_PDCP_MSG( PDCP_FRESH_SDU_NOT_PROCESSED, LOGDEBUG, PDCP_TX,
                   pdcpCurrentTime_g, __LINE__, 
                   pdcpContext.activeUeInfo[ueIndex], 
                   0,0,0, 0,0, __func__,"" );

#ifdef L2_FAST_PATH
           /*SPR 21108 Fix Start */
           pdcpIsDrbTxQEmpty = LTE_FALSE;
           /*SPR 21108 Fix End */
#endif
       }
       else
       {        
           while ( lcId < ueCtx_p->numOfActiveDrbLc)
           {
               /*
               ** activeLcInfo consists only of active DRBs in an UE, hence 
               ** valid PDCP context will always be available. Therefore, NULL
               ** check not required
               */
               /* SPR 5485 changes start */
               ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p,
                                             ueCtx_p->activeLcInfo[lcId], 
                                             PDCP_ENTITY_RB_TX);
               /* SPR 5485 changes end */


               if ( PDCP_TX_STATE_CONNECTED == ueCtx_p->pdcpTxSrb2DrbState )
               {
                   count = QCOUNT_PDCP_TX_DATA_REQ_Q( ctx_p );
                   /*SPR 2739 changes start*/
                   count = (count < MAX_PACKET_TRANSFER_TX ) ?
                                      count : MAX_PACKET_TRANSFER_TX;
                   /*SPR 2739 changes end*/
               }
               else
               {
                   /* SPR 10721 */
                   count = (ctx_p->storageCount < MAX_PACKET_TRANSFER_TX ) ?
                       ctx_p->storageCount : MAX_PACKET_TRANSFER_TX;
                   ctx_p->storageCount = ctx_p->storageCount - count;
                   if (0 != ctx_p->storageCount)
                   {
                       storageQueueProcessed = LTE_FALSE;
                   }
                   /* SPR 10721 */
               }
               while ( count-- )
               {
#ifdef LOG_PRINT_ENABLED
                   state  = ueCtx_p->pdcpTxSrb2DrbState;
#endif
                   result =
                   pdcpTxStateMachine[ueCtx_p->pdcpTxSrb2DrbState][PDCP_DRB_TX](
                           pdcpContext.activeUeInfo[ueIndex],
                           ueCtx_p->activeLcInfo[lcId], 
                           ueCtx_p->pdcpTxSrb2DrbState,
                           PDCP_DRB_TX, ctx_p);
                   LOG_PDCP_MSG( PDCP_TX_EVENT_RCVD, LOGDEBUG, PDCP_TX, pdcpCurrentTime_g,
                            __LINE__, 
                            pdcpContext.activeUeInfo[ueIndex],
                            state, PDCP_DRB_TX,
                            ueCtx_p->pdcpTxSrb2DrbState, ueCtx_p->activeLcInfo[lcId], 0, __func__,
                            "PdcpTxStateMachine");
               }
               lcId++;
#ifdef L2_FAST_PATH
               count = QCOUNT_PDCP_TX_DATA_REQ_Q( ctx_p );
               /*+ SPR 22399 changes */
               if(count != 0 ||  LTE_FALSE == storageQueueProcessed )
               /*- SPR 22399 changes */               
               {
                  pdcpIsDrbTxQEmpty = LTE_FALSE;
               }
#endif
           }
       }
       /* All PDCP SDUs re-transmitted, hence change the state from RESUME to
        * Connected */
       /* SPR 2922 changes start */
       /* SPR 10721 */
       if (( PDCP_TX_STATE_RESUME == ueCtx_p->pdcpTxSrb2DrbState) && 
           (LTE_TRUE == storageQueueProcessed))
           /* SPR 10721 */
       {
#ifdef LOG_PRINT_ENABLED
           state  = ueCtx_p->pdcpTxSrb2DrbState;
#endif
           /* SPR 2922 changes end */
           result = pdcpRrcTxStateMachine[ueCtx_p->pdcpTxSrb2DrbState]
               [ueCtx_p->event] ( 
               pdcpContext.activeUeInfo[ueIndex], 
                            LTE_SRB_LC_ID_HIGH_BOUND, 
                            ueCtx_p->pdcpTxSrb2DrbState, ueCtx_p->event, 
                            ueCtx_p );
            LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                          __LINE__, 
                          pdcpContext.activeUeInfo[ueIndex],
                          state, ueCtx_p->event,
                          ueCtx_p->pdcpTxSrb2DrbState, LTE_SRB_LC_ID_HIGH_BOUND, 0, __func__,
                          "PdcpRrcTxStateMachine");
       }
       ueIndex++;
       lcId = 0;
    }
#ifdef L2_FAST_PATH
    if(LTE_TRUE == pdcpIsDrbTxQEmpty)
    {
       pdcpIsDrbDataReceivedInTxQ_g = LTE_FALSE;
    }
#endif
    return result;
}

/****************************************************************************
 * Function Name  : pdcpElRxProcessMessage  
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : It takes one message form the PDCP RX queue, finds 
 *                  corresponding PDCP entity and processes it according to
 *                  rules which are defined for selected PDCP entity type.
 ****************************************************************************/
/* + SPR 17439 */
void pdcpElRxProcessMessage(void)
/* - SPR 17439 */    
{ 
    LP_PdcpUeContext    ueCtx_p = PNULL;
    LP_PdcpEnityContext ctx_p   = PNULL;
    UInt16              ueIndex = 0;
    UInt64              count   = 0;
    UInt8               lcId    = 0;
#ifdef LOG_PRINT_ENABLED
    UInt32              state   = 0;
#endif
    /* SPR 13338 Fixed - Removed code */
    while ( ueIndex < pdcpContext.numOfActiveUe)
    {
        /*
         ** activeUeInfo consists only of active UEs, hence valid UE context
         ** will always be available. Therefore, no need to check for NULL check
         */
        /* SPR 5485 changes start */
        ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(pdcpContext.activeUeInfo[ueIndex]);
        /* SPR 5485 changes end */

        /* Process SRB packets for high priority */
        for ( lcId = LTE_SRB_LC_ID_LOW_BOUND; 
                lcId <= LTE_SRB_LC_ID_HIGH_BOUND; lcId++ )
        {
            /* SPR 5485 changes start */
            ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, lcId, 
                    PDCP_ENTITY_RB_RX );
            /* SPR 5485 changes end */
            if (PNULL == ctx_p)
            {
                /* SPR 4390 changes start*/
                LOG_UT( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RX,
                        pdcpCurrentTime_g, __LINE__, 
                        pdcpContext.activeUeInfo[ueIndex], 
                        lcId, 0,0, 0,0,
                        __func__,"");
                /* SPR 4390 changes end*/
                continue;
            }
            count = QCOUNT_PDCP_RX_DATA_IND_Q(ctx_p);
            
            count = (count < MAX_PACKET_TRANSFER_RX) ? 
                count : MAX_PACKET_TRANSFER_RX;
            while ( count-- )
            {
#ifdef LOG_PRINT_ENABLED
                /* Process PDU*/
                state = ueCtx_p->pdcpRxState;
#endif
                (void)pdcpRxStateMachine[ueCtx_p->pdcpRxState][PDCP_SRB_RX](
                        pdcpContext.activeUeInfo[ueIndex], 
                        lcId, 
                        ueCtx_p->pdcpRxState, PDCP_SRB_RX, ctx_p);

                LOG_PDCP_MSG( PDCP_RX_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                            __LINE__, 
                            pdcpContext.activeUeInfo[ueIndex], 
                            state, PDCP_SRB_RX, ueCtx_p->pdcpRxState, 
                            lcId,0, __func__, 
                            "PdcpRxStateMachine");
            }

        }
        /* Process fresh DRB packets */
        lcId = 0;            
        while ( lcId < ueCtx_p->numOfActiveDrbLc )
        {
            /*
             ** activeLcInfo consists only of active DRBs in an UE, hence
             ** valid PDCP context will always be available. Therefore, 
             ** no need to check for NULL check
             */
            /* SPR 5485 changes start */
            ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                    ueCtx_p->activeLcInfo[lcId],
                    PDCP_ENTITY_RB_RX );
            /* SPR 5485 changes end */
            count = QCOUNT_PDCP_RX_DATA_IND_Q(ctx_p);
            
            count = (count < MAX_PACKET_TRANSFER_RX) ? 
                count : MAX_PACKET_TRANSFER_RX;
            while ( count-- )
            {
#ifdef LOG_PRINT_ENABLED
                /* Process PDU*/
                state = ueCtx_p->pdcpRxState;
#endif
                (void)pdcpRxStateMachine[ueCtx_p->pdcpRxState][PDCP_DRB_RX](
                        pdcpContext.activeUeInfo[ueIndex], 
                        ueCtx_p->activeLcInfo[lcId], ueCtx_p->pdcpRxState,
                        PDCP_DRB_RX, ctx_p);
                LOG_PDCP_MSG( PDCP_RX_EVENT_RCVD, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                            __LINE__, 
                            pdcpContext.activeUeInfo[ueIndex], 
                            state, PDCP_DRB_RX, ueCtx_p->pdcpRxState,
                            ueCtx_p->activeLcInfo[lcId],0, __func__, 
                            "PdcpRxStateMachine");
            }
            lcId++;
        }
        ueIndex++;
    }
    return;
}

/****************************************************************************
 * Function Name  : pdcpElTxProcessDiscardTimers  
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Processing queues with timers
 ****************************************************************************/
void pdcpElTxProcessDiscardTimers(void)
{
#ifndef UE_SIM_TESTING
    pdcpProcessDeleteUeTimers();
#endif
    pdcpProcessDiscardTimers();
    /* SPR 3608 changes start */
    pdcpProcessPSRTimers();    
    /* SPR 3608 changes end */
}

/****************************************************************************
 * Function Name  : pdcpElTxProcessDeliveryInd  
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Process one RLC data delivery indication (PDCP PDU) message
 *                  from PDCP delivery indication TX queue
 ****************************************************************************/
void pdcpElTxProcessDeliveryInd(void)
{
    LP_PdcpDeliveryInd deliveryInd_p = PNULL;
    
    UInt32 sQCount = 0;
#ifndef UE_SIM_TESTING
    InternalCellIndex  index = 0;
    for(; index < pdcpContext.sCellCount ;index++)
    {
        sQCount = QCOUNT_PDCP_TX_DELIVERY_IND_Q(index);
#else

        sQCount = QCOUNT_PDCP_TX_DELIVERY_IND_Q();
#endif

        while(sQCount--) 
        {
            /*Get User Data request (SDU) from PDCP delivery indication TX queue*/
#ifndef UE_SIM_TESTING
            DEQUEUE_PDCP_TX_DELIVERY_IND_Q(deliveryInd_p,index);
#else
            DEQUEUE_PDCP_TX_DELIVERY_IND_Q(deliveryInd_p);
#endif
            if ( PNULL == deliveryInd_p )
            {
                /*Nothing to process*/
                LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                        __LINE__, 0,0,0,0, 0,0, __func__,"pdcpTxDeliveryIndQ_g");

                break;
            }
            /*process it*/
            processDeliveryStatusInd( deliveryInd_p );
            /*free deliveryInd_p*/
            /*SPR Fixed 925 --- START*/
            freeMemPool ( deliveryInd_p->deliveryStatusArr );
            /*SPR 2498 changes start*/
            deliveryInd_p->deliveryStatusArr = PNULL;
            /*SPR 2498 changes end*/ 
            freeMemPool( deliveryInd_p );
            deliveryInd_p = PNULL;
            /*SPR Fixed 925 --- END*/
        }
#ifndef UE_SIM_TESTING
    }
#endif
}

/****************************************************************************
 * Function Name  : pdcpElTxProcessControlPduInd
 * Inputs         : count
 * Outputs        : None
 * Returns        : None
 * Description    : Process all available PDCP Control PDU indications from 
 *                  PDCP Control PDU TX queue.
 ****************************************************************************/
void pdcpElTxProcessControlPduInd(UInt64 count)
{  
    LP_PdcpControlPduInd controlPduInd_p = PNULL;

    while (count--)
    {
        controlPduInd_p = pdcpGetControlPduInd();
        if ( PNULL != controlPduInd_p )
        {
            pdcpProcessControlPduIndTx( controlPduInd_p );
        }
        else
        {
            LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, 0,0,0,0, 0,0, __func__,"pdcpTxControlPduIndQ_g");
        }
    }
}

#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : pdcpGetKpiStats
 * Inputs         : cnfBuff - confirmation buffer
 *                  kpiBitmap - Bitmap indicating KPI's to be pegged.
 *                  pLtePdcpKPIStats - Stats to be filled
 * Outputs        : None
 * Returns        :
 * Variables      :
 * Description    : This API will fill the stats in the confirmation buffer.
 ****************************************************************************/
void pdcpGetKpiStats(LP_PdcpKpiStatsIndParams cnfBuff, 
    UInt8 kpiBitmap, LtePdcpKPIStats *pLtePdcpKPIStats)
{
    UInt8 qci            = 0;
    cnfBuff->durationTTI = pLtePdcpKPIStats->lastCalculatedTTI;

    for (qci = 0; qci < MAX_QCI; qci++)
    {
        if(IS_KPI_PDCP_BITSET(kpiBitmap, KPI_TOTAL_DL_DRB_DISCARD_RATE))
        {
             cnfBuff->totalDlDrbDiscardRatePerQci[qci] = 
                 pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci] ;
             cnfBuff->totalSduRcvdDlperQci[qci] = 
                 pLtePdcpKPIStats->totalDlDrbSduRcvd[qci];
        }
        if(IS_KPI_PDCP_BITSET(kpiBitmap, KPI_TOTAL_UL_DRB_LOSS_RATE))
        {
             cnfBuff->totalUlLossRatePerQci[qci] = 
                pLtePdcpKPIStats->totalUlLossRatePerQci[qci];
             cnfBuff->totalSduRcvdUlperQci[qci] = 
                pLtePdcpKPIStats->totalUlDrbSduRcvd[qci];
        }
    } /* end of for (qci) */

	if(IS_KPI_PDCP_BITSET(kpiBitmap, KPI_PDCP_DL_CTR_SDU_BITRATE))
    { 
             cnfBuff->dlCtrlPdcpSduBitRate = 
                 pLtePdcpKPIStats->dlCtrlPdcpSduBitRate;
    }
    
	if(IS_KPI_PDCP_BITSET(kpiBitmap, KPI_PDCP_UL_CTR_SDU_BITRATE))
	{
             cnfBuff->ulCtrlPdcpSduBitRate = 
                 pLtePdcpKPIStats->ulCtrlPdcpSduBitRate;
    }
}

/****************************************************************************
 * Function Name  : sendPdcpKpiStatsInd
 * Inputs         : pLtePdcpKPIStats - Stats to be filled
 *                  pLtePdcpKPIConfig -
 *                  pKpiModuleInfo -
 *                  internalCellIndex - CellIndex of PCELL or SCELL
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API will send PDCP_KPI_STATS_IND with KPI cumulative
 *                  counters maintained at PDCP.
 ****************************************************************************/
void sendPdcpKpiStatsInd(LtePdcpKPIStats  *pLtePdcpKPIStats, 
                         LtePdcpKPIConfig *pLtePdcpKPIConfig,
                         kpiModuleInfo    *pKpiModuleInfo,
                         InternalCellIndex internalCellIndex) 
{
    LP_PdcpKpiStatsIndParams cnfBuff = PNULL;
    UInt8        *responseBuf;   /* Memory buffer for response */
    LP_TlvMessage       responseMsg_p = PNULL;
    /* SPR 5443 */
    UInt16              msgSize       = 0;

    responseBuf = (UInt8 *)getMemFromPool( ( pKpiModuleInfo->hdrLen + 
                sizeof(PdcpKpiStatsIndParams)), PNULL);
    /* SPR 5443 */
    if ( PNULL == responseBuf ) 
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + \
                sizeof(PdcpKpiStatsIndParams), 0,0,0, 0,0, __func__,
                "PDCP_KPI_STATS_IND");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }
    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            pLtePdcpKPIConfig->transactionId,
            pKpiModuleInfo->dstModuleId,
            pKpiModuleInfo->periodicIndMsgType,
            sizeof(PdcpKpiStatsIndParams),
            /* + Layer2 NON CA Changes */
            layer2CommonGetRRCCellIndex(internalCellIndex));
            /* - Layer2 NON CA Changes */


    /*Prepare pointer to response message data */
    cnfBuff = (LP_PdcpKpiStatsIndParams)responseMsg_p->data;
    /* SPR 10409 fix start */
    memSet(cnfBuff,0,sizeof(PdcpKpiStatsIndParams));
    /* SPR 10409 fix end */

    pdcpGetKpiStats(cnfBuff, pLtePdcpKPIConfig->kpiBitmap, pLtePdcpKPIStats);

    /*Convert response fields to network byte order */
    /* SPR 5443 */
    if(RRM_MODULE_ID != pKpiModuleInfo->dstModuleId)
    {
        tlvPrepareHeaderForSending(responseMsg_p);
        msgSize = PDCP_NTOHS(responseMsg_p->header.length);
    }
    else
    {
        msgSize = responseMsg_p->header.length;
    }
    /* SPR 5443 */

    /*send response */
    if(pKpiModuleInfo->sendToModule)
    {
        /* SPR 5443 */
        pKpiModuleInfo->sendToModule( responseBuf, msgSize
                );
        /* SPR 5443 */
    }
    freeMemPool ( responseBuf );
}

/****************************************************************************
 * Function Name  : sendPdcpKpiThpStatsInd
 * Inputs         : respMsgId, moduleCounter
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API will send PDCP_KPI_THP_STATS_IND or PDCP_GET_KPI_THP_STATS_RESP 
 *                  with KPI cumulative counters maintained at PDCP.
 ****************************************************************************/
void sendPdcpKpiThpStatsInd( UInt32 moduleCounter )
{
    UInt8                      *responseBuf             = PNULL;   /* Memory buffer for response */
    /* SPR 6492 : Start*/
    UInt32                      moduleBitmap            = 0;
    /* SPR 6492 : End*/
    /* SPR 10579 changes start*/
    LteKPIThpStatus   *pLtePdcpKPIThpStatus  = PNULL;
    /* SPR 10579 changes end*/
    LtePdcpKPIConfig *pLtePdcpKPIConfig      = PNULL;
    InternalCellIndex cellIndex = 0 ;
    /* SPR 22296 Fix Start */
    UInt8 resetKpi = LTE_FALSE;
    /* SPR 5443 */
    for(cellIndex = 0 ; cellIndex < pdcpContext.sCellCount; cellIndex++)
    {
        responseBuf = (UInt8 *)getMemFromPool( (L2_RRM_API_HEADER_LEN + \
                    /* SPR 5443 */
                    sizeof(PdcpKpiThpStatsIndParams)), PNULL);
        if ( PNULL == responseBuf ) 
        {
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + \
                    sizeof(PdcpKpiThpStatsIndParams), 0,0,0, 0,0, __func__,
                    /* SPR 9680 changes start */
                    "PDCP_KPI_THP_STATS_IND or PDCP_GET_KPI_THP_STATS_RESP");
            /* SPR 9680 changes end */
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic ("getMemPool error in %s", __func__);
            return;
        }
        /* SPR 5464 */         
        /* SPR 6492 : Start*/
        /* SPR 10579 changes start*/
        pLtePdcpKPIThpStatus = &(g_kpiScannerObj.kpiStatsContainer[moduleCounter].status.ltePdcpKPIThpStatus);
        moduleBitmap = pLtePdcpKPIThpStatus->moduleBitmap; 
        /* SPR 6492 : End*/

        pLtePdcpKPIConfig = &(g_kpiScannerObj.kpiStatsContainer[moduleCounter].config.ltePdcpKPIConfig);
        if(!(pLtePdcpKPIConfig->kpiBitmap))
        {
            /*-coverity CID-63556-*/
            freeMemPool ( responseBuf );
            /*-coverity CID-63556-*/
            return;
        }

        /* SPR 10579 changes end*/
        /* SPR 6492 : Start*/
        if(cellIndex == (pdcpContext.sCellCount-1))
        {
            resetKpi = LTE_TRUE;
        }

        if(moduleBitmap & (0x01 << moduleCounter) && (pLtePdcpKPIConfig->kpiBitmap))
            /* SPR 6492 : End*/
        {
            /* + SPR 10577 */
            pdcpGetAndSendKpiThpStats( responseBuf, 
                    pLtePdcpKPIConfig->transactionId, pLtePdcpKPIConfig->kpiBitmap,
                    moduleCounter, 
                    g_kpiModuleInfo[moduleCounter].periodicThpIndMsgType,cellIndex,resetKpi);
            /* + SPR 17637 */

            /* - SPR 17637 */
            /* - SPR 10577 */
        }
        freeMemPool ( responseBuf );
    }
    for(cellIndex = 0 ; cellIndex < pdcpContext.sCellCount; cellIndex++)
    {
        resetKpiContainer(moduleCounter, cellIndex);
    }
    /* SPR 22296 Fix End */
}
#endif

#if defined (PERF_STATS)  && !defined(UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : cellFillPdcpOamPerfContainer
 * Inputs         : *pPdcpCellOamPerfStats
 *                   passiveIndexForPdcpStats
 *                   cellPerfStatsBitMap
 *                   internalCellIndex - CellIndex of PCELL or SCELL
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *****************************************************************************/
void cellFillPdcpOamPerfContainer (PdcpCellPerfStatsResp *pPdcpCellOamPerfStats, 
                                   UInt32 passiveIndexForPdcpStats,
				   UInt32 cellPerfStatsBitMap,
				   InternalCellIndex internalCellIndex)
{
    UInt32 count = 0;
    UInt32 localBitMap = 0;
    UInt32 firstBitSet = 0;
    LteCellPdcpDLPerfStats *lteCellPdcpDLPerfStats_p = 
	&(gPerfStats.perfStats[passiveIndexForPdcpStats].lteCellPerfStats[internalCellIndex].lteCellPdcpDLPerfStats);
    LteCellPdcpULPerfStats *lteCellPdcpULPerfStats_p = 
	&(gPerfStats.perfStats[passiveIndexForPdcpStats].lteCellPerfStats[internalCellIndex].lteCellPdcpULPerfStats);

    localBitMap = cellPerfStatsBitMap;
    firstBitSet =ffs_wrapper(localBitMap);
    /* Fills the response buffer according to the requested bitmap*/
    while (firstBitSet)
    {
        switch (firstBitSet - 1)
        {
            case PERF_STATS_ROHC:
            for (count = 0; count < MAX_QCI; count++)
            {
                pPdcpCellOamPerfStats->pdcpRohcPerfStats.totalRohcUnCmpHdrVolDL[count] +=
                    lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL[count];

                pPdcpCellOamPerfStats->pdcpRohcPerfStats.totalRohcCmpHdrVolDL[count] +=
                    lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL[count];

                pPdcpCellOamPerfStats->pdcpRohcPerfStats.totalRohcUnCmpHdrVolUL[count] +=
                    lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL[count];

                pPdcpCellOamPerfStats->pdcpRohcPerfStats.totalRohcCmpHdrVolUL[count] +=
                    lteCellPdcpULPerfStats_p->totalRohcCmpHdrVolUL[count];
            }
            break;

            default:
            break;
        }
        localBitMap = localBitMap & (~(1<<(firstBitSet - 1)));
        firstBitSet = ffs_wrapper(localBitMap);
    }
}

/****************************************************************************
 * Function Name  : ueFillPdcpOamPerfContainer
 * Inputs         : *pPdcpUeOamPerfStats
 *                   passiveIndexForPdcpStats
 *                   uePerfStatsBitMap
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *******************************************************************************/
void ueFillPdcpOamPerfContainer (PdcpUePerfStatsResp *pPdcpUeOamPerfStats, 
                                 UInt32 passiveIndexForPdcpStats,
				 UInt32 uePerfStatsBitMap)
{
    UInt32 ueIndex     = 0;
    UInt32 lcId        = 0;
    UInt32 localBitMap = 0;
    UInt32 firstBitSet = 0;

    LteUePerfStats *pLteUeMacPerfStats = 
	&((gPerfStats.perfStats[passiveIndexForPdcpStats].lteUePerfStats[0]));
    LteUePdcpDLPerfStats *lteUePdcpDLPerfStats_p = PNULL;  
    LteUePdcpULPerfStats *lteUePdcpULPerfStats_p= PNULL; 
    LteUePdcpRadioBearerInfoStats *lteUePdcpRadioBearerInfoStats_p = PNULL; 
    PdcpRbPerfStats *pdcpRbPerfStats_p  = PNULL;
    PdcpUeVolPerfStats *pdcpUeVolPerfStats_p = PNULL;
    PdcpUeRadioBearerInfoStats *pdcpUeRadioBearerInfoStats_p = PNULL;   

    for (ueIndex = 0; ueIndex < PERF_MAX_UE_SUPPORTED; ueIndex++)
    {
	lteUePdcpDLPerfStats_p =  &(pLteUeMacPerfStats[ueIndex].lteUePdcpDLPerfStats);    
	lteUePdcpULPerfStats_p =  &(pLteUeMacPerfStats[ueIndex].lteUePdcpULPerfStats);

	localBitMap = uePerfStatsBitMap;
	firstBitSet = ffs_wrapper(localBitMap);
	/* Fills the response buffer according to the requested bitmap per UE wise*/
	while (firstBitSet)
	{
	    switch (firstBitSet - 1)
	    {
		case PERF_STATS_PDCP_RB:
		    pdcpRbPerfStats_p =  &(pPdcpUeOamPerfStats[ueIndex].pdcpRbPerfStats);

		    pdcpRbPerfStats_p->totalDlSrbBytesSent += 
			lteUePdcpDLPerfStats_p->totalDlSrbBytesSent;

		    pdcpRbPerfStats_p->totalUlSrbBytesRcvd += 
			lteUePdcpULPerfStats_p->totalUlSrbBytesRcvd;

		    pdcpRbPerfStats_p->totalDlRbBytesSent += 
			lteUePdcpDLPerfStats_p->totalDlSrbBytesSent;

		    pdcpRbPerfStats_p->totalUlRbBytesSent += 
			lteUePdcpULPerfStats_p->totalUlSrbBytesRcvd; 

		    for (lcId = 0 ; lcId < MAX_LC_IDENTITY ; lcId ++)
		    {
			pdcpRbPerfStats_p->totalDlRbBytesSent += 
			    lteUePdcpDLPerfStats_p->totalDlDrbBytesSent[lcId];
			
                        pdcpRbPerfStats_p->totalUlRbBytesSent += 
			    lteUePdcpULPerfStats_p->totalUlDrbBytesSent[lcId];

			pdcpRbPerfStats_p->totalUlDrbBytesSent[lcId] += 
			    lteUePdcpULPerfStats_p->totalUlDrbBytesSent[lcId];

			pdcpRbPerfStats_p->totalDlDrbBytesSent[lcId] += 
			    lteUePdcpDLPerfStats_p->totalDlDrbBytesSent[lcId];

			pdcpRbPerfStats_p->numberOfUlDrbSduReceived[lcId] += 
			    lteUePdcpULPerfStats_p->numberOfUlDrbSduReceived[lcId];

			pdcpRbPerfStats_p->numberOfDlDrbSduReceived[lcId] += 
			    lteUePdcpDLPerfStats_p->numberOfDlDrbSduReceived[lcId];
		    }
                break;   

		case PERF_STATS_PDCP_RADIO_BEARER_INFO:
		    pdcpUeRadioBearerInfoStats_p = &(pPdcpUeOamPerfStats[ueIndex].\
					pdcpUeRadioBearerInfoStats[0]);
                    lteUePdcpRadioBearerInfoStats_p = (&gPdcpUePerfStats_p[ueIndex].\
                            lteUePdcpRadioBearerInfoStats[0]);

		    for (lcId = 1; lcId < MAX_PDCP_LC_ID ; lcId++)
		    {
			pdcpUeRadioBearerInfoStats_p[lcId].bearerId =
			    (lteUePdcpRadioBearerInfoStats_p+lcId)->bearerId;

			pdcpUeRadioBearerInfoStats_p[lcId].bearerType =
			    (lteUePdcpRadioBearerInfoStats_p+lcId)->bearerType;

			pdcpUeRadioBearerInfoStats_p[lcId].qci = 
			    (lteUePdcpRadioBearerInfoStats_p+lcId)->qci;

			pdcpUeRadioBearerInfoStats_p[lcId].rlcMode = 
			    (lteUePdcpRadioBearerInfoStats_p+lcId)->rlcMode;
		    }
		    break;

		case PERF_STATS_PDCP_VOLUME:
		    pdcpUeVolPerfStats_p = &(pPdcpUeOamPerfStats[ueIndex].\
				pdcpUeVolPerfStats);

		    pdcpUeVolPerfStats_p->totalDLSrbBytesSentAndAck +=
			lteUePdcpDLPerfStats_p->totalDLSrbBytesSentAndAck;			

		    for (lcId = 0 ; lcId < MAX_LC_IDENTITY ; lcId++)
		    {
			pdcpUeVolPerfStats_p->totalDlDrbSduDiscardedHO[lcId] +=
			    lteUePdcpDLPerfStats_p->totalDlDrbSduDiscardedHO[lcId];

			pdcpUeVolPerfStats_p->pdcpLastTtiVolDl[lcId] +=
			    lteUePdcpDLPerfStats_p->pdcpLastTtiTransVolDl[lcId];
		    }
		    break;

		default:
		    break;
	    }
	    localBitMap = localBitMap & (~(1<<(firstBitSet - 1)));
	    firstBitSet = ffs_wrapper(localBitMap);
	}
    }
}

/****************************************************************************
 * Function Name  : updatePdcpOamPerfContainer
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 ******************************************************************************/
/* + SPR 17439 */
void updatePdcpPerfContainer(void)
    /* - SPR 17439 */
{
    InternalCellIndex cellIndex = 0;
    /* SPR 15909 fix start */
	tickType_t pdcpLastCalculatedTTI    = 0;
    /* SPR 15909 fix end */
    UInt32 passiveIndexForPdcpStats   = 0;
    UInt32 activeIndexForPdcpStats    =  gPerfStats.perfStatsControlInfo->pdcpActiveIndex;
    passiveIndexForPdcpStats    = activeIndexForPdcpStats ^0x01;

    /* Pointer to store the bit map */
    LtePerfStatsConfig *pLtePdcpPerfStatsConfig =  &(g_perfStatsScannerObj.\
            perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].perfConfig.ltePdcpPerfStatsConfig);

    pdcpLastCalculatedTTI =
	gPerfStats.perfStats[passiveIndexForPdcpStats].pdcpLastCalculatedTTI;
    if (pdcpLastCalculatedTTI)
    {
         /* Update all the counters. 
         * For future use: if another containers are also configured then those are also
         * updated here.
         */
	for(cellIndex = 0; cellIndex < pdcpContext.sCellCount; cellIndex++)
	{
		cellFillPdcpOamPerfContainer(&(g_perfStatsScannerObj.\
		    perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].perfStats.ltePdcpPerfStats.ltePdcpCellPerfStats[cellIndex]),
		    passiveIndexForPdcpStats,
		    pLtePdcpPerfStatsConfig->cellPerfStatsToReportBitMap,
			cellIndex);
	}
	ueFillPdcpOamPerfContainer(&(g_perfStatsScannerObj.\
		    perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].perfStats.ltePdcpPerfStats.ltePdcpUePerfStats[0]),
		    passiveIndexForPdcpStats,
		    pLtePdcpPerfStatsConfig->uePerfStatsToReportBitMap);

	g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].
	    perfStats.ltePdcpPerfStats.lastPDCPCalculatedTTI += pdcpLastCalculatedTTI;

    }
}

/****************************************************************************
 * Function Name  : pdcpGetCellPerfStats
 * Inputs         : cnfBuff Pointer -
 *                  msgType -
 *                  cellPerfStatsToGetBitMap -
 *                  internalCellIndex - CellIndex of PCELL or SCELL
 * Outputs        : 
 * Returns        : 
 * Description    : 
 ****************************************************************************/
void pdcpGetCellPerfStats(void *cnfBuff, UInt32 msgType,UInt32 cellPerfStatsToGetBitMap,
					InternalCellIndex internalCellIndex)
{
    UInt8 qci = 0;
    UInt32 firstBitSet = 0;
    UInt32 localPerfBitMap = 0;

    PdcpCellPerfStatsResp *pLtePdcpCellPerfStats = PNULL;
    PdcpCellPerfStatsResp *pdcpCellPerfStatsResp = PNULL;
    PdcpRohcPerfStats     *pdcpRohcPerfStats_p   = PNULL;
    PdcpRohcPerfStats     *pPdcpRohcPerfStats    = PNULL;

    pLtePdcpCellPerfStats = &(g_perfStatsScannerObj.
            perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].perfStats.ltePdcpPerfStats.ltePdcpCellPerfStats[internalCellIndex]);
    localPerfBitMap = cellPerfStatsToGetBitMap;

    if (UNSET_PERIODIC_FLAG == msgType)
    {
        if (!cellPerfStatsToGetBitMap)
        {
            ((LP_PdcpGetCellPerfStatsCnfParams)cnfBuff)->responseCode = PDCP_FAIL;
            return;
        }
        else
        {
            pdcpCellPerfStatsResp = &(((LP_PdcpGetCellPerfStatsCnfParams)cnfBuff)->pdcpCellPerfStatsResp);
            ((LP_PdcpGetCellPerfStatsCnfParams)cnfBuff)->responseCode = PDCP_SUCCESS;
        }
    }
    else if(SET_PERIODIC_FLAG == msgType)
    {
        pdcpCellPerfStatsResp = &(((LP_PdcpCellPerfStatsIndParams)cnfBuff)->pdcpCellPerfStatsResp); 
    }
    else
    {
        return;
    }

    pdcpCellPerfStatsResp->durationt = g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].\
                                       perfStats.ltePdcpPerfStats.lastPDCPCalculatedTTI;

    firstBitSet = ffs_wrapper(localPerfBitMap);
    while (firstBitSet)
    {
        switch (firstBitSet - 1)
        {
            case PERF_STATS_ROHC:
            pdcpRohcPerfStats_p = &(pdcpCellPerfStatsResp->pdcpRohcPerfStats);
            pPdcpRohcPerfStats  = &(pLtePdcpCellPerfStats->pdcpRohcPerfStats);
            for ( qci = 0; qci < MAX_QCI; qci++)
            {
                pdcpRohcPerfStats_p->totalRohcUnCmpHdrVolDL[qci] = 
                    pPdcpRohcPerfStats->totalRohcUnCmpHdrVolDL[qci];

                pdcpRohcPerfStats_p->totalRohcCmpHdrVolDL[qci] = 
                    pPdcpRohcPerfStats->totalRohcCmpHdrVolDL[qci];

                pdcpRohcPerfStats_p->totalRohcUnCmpHdrVolUL[qci] = 
                    pPdcpRohcPerfStats->totalRohcUnCmpHdrVolUL[qci];

                pdcpRohcPerfStats_p->totalRohcCmpHdrVolUL[qci] = 
                    pPdcpRohcPerfStats->totalRohcCmpHdrVolUL[qci];
            }
            break;

            default:
            break;
        }
        localPerfBitMap = localPerfBitMap & (~(1<<(firstBitSet - 1)));
        firstBitSet = ffs_wrapper(localPerfBitMap);
    }
}

/****************************************************************************
 * Function Name  : statsPresentForUe
 * Inputs         : UeIndex
 * Outputs        : 
 * Returns        : PDCP_SUCCESS/PDCP_FAIL
 * Description    : 
 ****************************************************************************/
UInt32 statsPresentForUe( UInt16 UeIndex)
{
    LP_PdcpUeContext ueCtx_p = PNULL;
    ueCtx_p = PDCP_GET_UE_CONTEXT(UeIndex);
    if (PNULL == ueCtx_p)
        return PDCP_FAIL;
    return PDCP_SUCCESS; 
}

/***************************************************************************
 * Function Name  : pdcpGetUePerfStats
 * Inputs         : cnfBuff pointer -
 *                  uePerfStatsToGetBitMap -
 *                  xi_ueIndex -
 *                  msgType -
 *                  internalCellIndex - CellIndex of PCELL or SCELL
 * Outputs        : 
 * Returns        : UInt16
 * Description    : 
 ****************************************************************************/
/* + stats_review_comments_ipc */
UInt16 pdcpGetUePerfStats(void *cnfBuff, UInt32 uePerfStatsToGetBitMap, 
    UInt16 xi_ueIndex, UInt32 msgType, InternalCellIndex cellIndex)
/* - stats_review_comments_ipc */
{
    UInt8 lcId = 0;
    /* +- SPR 18268 */
    UInt16 ueIndex          = 0;
    /* +- SPR 18268 */
    UInt32 localPerfBitMap = 0;
    UInt32 firstBitSet    = 0;
    UInt32 *numOfUePresent = PNULL;

    PdcpRbPerfStats            *pdcpRbPerfStats_p            = PNULL;
    PdcpUeVolPerfStats         *pdcpUeVolPerfStats_p         = PNULL;
    PdcpUeRadioBearerInfoStats *pdcpUeRadioBearerInfoStats_p = PNULL;   
    PdcpUePerfStatsResp        *pdcpUePerfStatsResp          = PNULL;
    PdcpRbPerfStats            *pPdcpRbPerfStats             = PNULL;
    PdcpUeVolPerfStats         *pPdcpUeVolPerfStats          = PNULL;
    PdcpUeRadioBearerInfoStats *pPdcpUeRadioBearerInfoStats  = PNULL;
    PdcpUePerfStatsResp        *pPdcpUePerfStatsResp         = PNULL;
    /* + stats_review_comments_ipc */
    LP_PdcpUeContext    ueCtx_p  = PNULL;
    /* - stats_review_comments_ipc */
    if (UNSET_PERIODIC_FLAG == msgType)
    {
        if (!uePerfStatsToGetBitMap)
        {
            ((LP_PdcpGetUePerfStatsCnfParams)cnfBuff)->responseCode = PDCP_FAIL;
            return PERF_MAX_UE_SUPPORTED;
        }
        else
        {
            ((LP_PdcpGetUePerfStatsCnfParams)cnfBuff)->duration =  g_perfStatsScannerObj.\
                                                                   perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].perfStats.ltePdcpPerfStats.lastPDCPCalculatedTTI;
            numOfUePresent = &(((LP_PdcpGetUePerfStatsCnfParams)cnfBuff)->numOfUePresent);
            pdcpUePerfStatsResp = ((LP_PdcpGetUePerfStatsCnfParams)cnfBuff)->pdcpUePerfStatsResp;

            ((LP_PdcpGetUePerfStatsCnfParams)cnfBuff)->responseCode = PDCP_SUCCESS;
        }
    }
    else if(SET_PERIODIC_FLAG == msgType)
    {
        ((LP_PdcpUePerfStatsIndParams)cnfBuff)->duration =  g_perfStatsScannerObj.\
                                                            perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].perfStats.ltePdcpPerfStats.lastPDCPCalculatedTTI;

        numOfUePresent = &(((LP_PdcpUePerfStatsIndParams)cnfBuff)->numOfUePresent);
        pdcpUePerfStatsResp = ((LP_PdcpUePerfStatsIndParams)cnfBuff)->pdcpUePerfStatsResp;
    }
    else
    {
        return PERF_MAX_UE_SUPPORTED;
    }
    if(!uePerfStatsToGetBitMap)
    {
        return PERF_MAX_UE_SUPPORTED;
    }

    for (ueIndex=0 ; (ueIndex < MAX_UE_PER_BATCH) && (xi_ueIndex < PERF_MAX_UE_SUPPORTED); xi_ueIndex++)
    {
        pPdcpUePerfStatsResp = &(g_perfStatsScannerObj.
                perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].perfStats.
                ltePdcpPerfStats.ltePdcpUePerfStats[xi_ueIndex]);
        /* + stats_review_comments_ipc */
        ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(xi_ueIndex);
        /* SPR 12765 Changes start */
        if (PNULL == ueCtx_p)
        {
            continue;
        }
        /* SPR 12765 Changes End */
        if(API_HEADER_DO_NOT_CARE_CELL_INDEX == cellIndex)
        {	
            if (!(statsPresentForUe(xi_ueIndex)))
            {
                continue;
            }
        }
        else if (!(statsPresentForUe(xi_ueIndex) || (ueCtx_p->cellIndex != cellIndex)))
        {
            continue;
        }
        /* - stats_review_comments_ipc */
        /* + SPR 11670 Fix */
        pdcpUePerfStatsResp[ueIndex].ueIndex = getRrcUeIdxFromUeIdx(xi_ueIndex); 
        /* - SPR 11670 Fix */
        localPerfBitMap = uePerfStatsToGetBitMap;
        firstBitSet = ffs_wrapper(localPerfBitMap);
        while (firstBitSet)
        {
            switch (firstBitSet - 1)
            {
                case PERF_STATS_PDCP_RB:
                pdcpRbPerfStats_p =  &(pdcpUePerfStatsResp[ueIndex].pdcpRbPerfStats);
                pPdcpRbPerfStats = &(pPdcpUePerfStatsResp->pdcpRbPerfStats);
                pdcpRbPerfStats_p->totalDlSrbBytesSent = 
                    pPdcpRbPerfStats->totalDlSrbBytesSent;

                pdcpRbPerfStats_p->totalUlSrbBytesRcvd = 
                    pPdcpRbPerfStats->totalUlSrbBytesRcvd;

                pdcpRbPerfStats_p->totalDlRbBytesSent = 
                    pPdcpRbPerfStats->totalDlSrbBytesSent;

                pdcpRbPerfStats_p->totalUlRbBytesSent = 
                    pPdcpRbPerfStats->totalUlSrbBytesRcvd ; 

                for (lcId = 0 ; lcId < MAX_LC_IDENTITY ; lcId ++)
                {
                    pdcpRbPerfStats_p->totalDlRbBytesSent += 
                        pPdcpRbPerfStats->totalDlDrbBytesSent[lcId];

                    pdcpRbPerfStats_p->totalUlRbBytesSent += 
                        pPdcpRbPerfStats->totalUlDrbBytesSent[lcId];

                    pdcpRbPerfStats_p->totalUlDrbBytesSent[lcId] = 
                        pPdcpRbPerfStats->totalUlDrbBytesSent[lcId];

                    pdcpRbPerfStats_p->totalDlDrbBytesSent[lcId] = 
                        pPdcpRbPerfStats->totalDlDrbBytesSent[lcId];

                    pdcpRbPerfStats_p->numberOfUlDrbSduReceived[lcId] = 
                        pPdcpRbPerfStats->numberOfUlDrbSduReceived[lcId];

                    pdcpRbPerfStats_p->numberOfDlDrbSduReceived[lcId] = 
                        pPdcpRbPerfStats->numberOfDlDrbSduReceived[lcId];
                }
                break;

                case PERF_STATS_PDCP_RADIO_BEARER_INFO:
                pdcpUeRadioBearerInfoStats_p = 
                    &(pdcpUePerfStatsResp[ueIndex].pdcpUeRadioBearerInfoStats[0]);
                pPdcpUeRadioBearerInfoStats = &(pPdcpUePerfStatsResp->pdcpUeRadioBearerInfoStats[0]);
                for (lcId = 1; lcId < MAX_PDCP_LC_ID ; lcId++)
                {
                    pdcpUeRadioBearerInfoStats_p[lcId].bearerId =
                        (pPdcpUeRadioBearerInfoStats+lcId)->bearerId;

                    pdcpUeRadioBearerInfoStats_p[lcId].bearerType =
                        (pPdcpUeRadioBearerInfoStats+lcId)->bearerType;

                    pdcpUeRadioBearerInfoStats_p[lcId].qci = 
                        (pPdcpUeRadioBearerInfoStats+lcId)->qci;

                    pdcpUeRadioBearerInfoStats_p[lcId].rlcMode = 
                        (pPdcpUeRadioBearerInfoStats+lcId)->rlcMode;
                }
                break;

                case PERF_STATS_PDCP_VOLUME:
                pdcpUeVolPerfStats_p = &(pdcpUePerfStatsResp[ueIndex].pdcpUeVolPerfStats);
                pPdcpUeVolPerfStats = &(pPdcpUePerfStatsResp->pdcpUeVolPerfStats);
                pdcpUeVolPerfStats_p->totalDLSrbBytesSentAndAck =
                    pPdcpUeVolPerfStats->totalDLSrbBytesSentAndAck;			
                for (lcId = 0 ; lcId < MAX_LC_IDENTITY ; lcId ++)
                {
                    pdcpUeVolPerfStats_p->totalDlDrbSduDiscardedHO[lcId] =
                        pPdcpUeVolPerfStats->totalDlDrbSduDiscardedHO[lcId];

                    pdcpUeVolPerfStats_p->pdcpLastTtiVolDl[lcId] =
                        pPdcpUeVolPerfStats->pdcpLastTtiVolDl[lcId];
                }
                break;

                default:
                break;
            }

            localPerfBitMap = localPerfBitMap & (~(1<<(firstBitSet - 1)));
            firstBitSet = ffs_wrapper(localPerfBitMap);
        }

        ueIndex++; 
    }
    *numOfUePresent = ueIndex;
    return xi_ueIndex;
}

/****************************************************************************
 * Function Name  : sendPdcpUePerfStatsInd
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Process Performance Counters in every tick 
 ****************************************************************************/
/* + SPR 17439 */
void sendPdcpUePerfStatsInd(void)
/* - SPR 17439 */    
{
    LP_PdcpUePerfStatsIndParams cnfBuff = PNULL;
    UInt16 ueIndex = 0;
    UInt16 ueRespSize = 0;
    UInt32 uePerfStatsToReportBitMap = 0;
    LP_TlvMessage       responseMsg_p = PNULL;
    LtePerfStatsConfig *ltePdcpPerfStatConfig_p =&(g_perfStatsScannerObj.\
            perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].perfConfig.ltePdcpPerfStatsConfig);

    /* SPR 5449 changes start */
    responseMsg_p = (LP_TlvMessage)&ueResponseBuf_g[0];
    /* SPR 5449 changes end */

    while(ueIndex < PERF_MAX_UE_SUPPORTED )
    {
        cnfBuff = (LP_PdcpUePerfStatsIndParams)responseMsg_p->data;
        memSet(cnfBuff,0,sizeof(PdcpUePerfStatsIndParams));
        uePerfStatsToReportBitMap = (g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].\
                perfConfig.ltePdcpPerfStatsConfig.uePerfStatsToReportBitMap);
        /* + stats_review_comments_ipc */
        ueIndex = pdcpGetUePerfStats(cnfBuff,uePerfStatsToReportBitMap, 
                ueIndex,SET_PERIODIC_FLAG,API_HEADER_DO_NOT_CARE_CELL_INDEX);
        /* - stats_review_comments_ipc */
	/* SPR 10577 changes start */
        cnfBuff->endIndicator = LTE_FALSE;
        if((ueIndex >= PERF_MAX_UE_SUPPORTED) || 
            (ueIndex >= pdcpContext.numOfActiveUe)
          )
	{
            /* endIndicator is set TRUE, as this is the end of the message */
            cnfBuff->endIndicator = LTE_TRUE;
        }
        /* sizeof(UInt8) has been added for endIndicator and padding*/ 
        ueRespSize = (((cnfBuff->numOfUePresent) * sizeof(PdcpUePerfStatsResp)) + 3* sizeof(UInt32) + 4*sizeof(UInt8));
	/* SPR 10577 changes end */
        /* Prepare response buffer */
        tlvPrepareMessage( responseMsg_p,
                ltePdcpPerfStatConfig_p->transactionId,
                OAM_MODULE_ID,
                PDCP_UE_PERF_STATS_IND,
                ueRespSize,
                API_HEADER_DO_NOT_CARE_CELL_INDEX);

        /*Convert response fields to network byte order */
        tlvPrepareHeaderForSending(responseMsg_p);

        /*send response */
        pdcpOamSendMessage( &ueResponseBuf_g[0],
                PDCP_NTOHS(responseMsg_p->header.length)
                );
    }
}

/****************************************************************************
 * Function Name  : sendPdcpCellPerfStatsInd
 * Inputs         : internalCellIndex - CellIndex of PCELL or SCELL
 * Outputs        : None
 * Returns        : None
 * Description    : Process Performance Counters in every tick 
 ****************************************************************************/
void sendPdcpCellPerfStatsInd(InternalCellIndex internalCellIndex)
{
    LP_PdcpCellPerfStatsIndParams cnfBuff = PNULL;
    LP_TlvMessage responseMsg_p;
    UInt8        *responseBuf;
    UInt32 cellPerfStatsToReportBitMap = 0;
    LtePerfStatsConfig *ltePdcpPerfStatConfig_p = &(g_perfStatsScannerObj.\
            perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].perfConfig.ltePdcpPerfStatsConfig);

    responseBuf = (UInt8 *)getMemFromPool( (
            sizeof(TlvHeader) + sizeof(PdcpCellPerfStatsIndParams)), PNULL);

    if ( PNULL == responseBuf )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, sizeof(TlvHeader) + \
                sizeof(PdcpCellPerfStatsIndParams), 0,0,0, 0,0, __func__,
                "PDCP_CELL_PERF_STATS_IND");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }
    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            ltePdcpPerfStatConfig_p->transactionId,
            OAM_MODULE_ID,
            PDCP_CELL_PERF_STATS_IND,
            sizeof(PdcpCellPerfStatsIndParams),
            /* + Layer2 NON CA Changes */
            layer2CommonGetRRCCellIndex(internalCellIndex));
            /* - Layer2 NON CA Changes */


    /*Prepare pointer to response message data */
    cnfBuff = (LP_PdcpCellPerfStatsIndParams)responseMsg_p->data;
    memSet(cnfBuff,0,sizeof(PdcpCellPerfStatsIndParams));

    cellPerfStatsToReportBitMap = (g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].\
            perfConfig.ltePdcpPerfStatsConfig.cellPerfStatsToReportBitMap);
    /* + PERF_CA */
    pdcpGetCellPerfStats((void *)cnfBuff,SET_PERIODIC_FLAG,cellPerfStatsToReportBitMap, internalCellIndex);
    /* - PERF_CA */

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpOamSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    freeMemPool ( responseBuf );
}

/****************************************************************************
 * Function Name  : pdcpProcessPerfCounter
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Process Performance Counters in every tick 
 ****************************************************************************/
/* + SPR 17439 */
void pdcpProcessPerfCounter(void)
/* - SPR 17439 */    
{
    /* + PERF_CA */
    InternalCellIndex cellIndex = 0;
    /* - PERF_CA */
    LtePerfStatsConfig *ltePdcpPerfStatsConfig_p =  &(g_perfStatsScannerObj.\
            perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].perfConfig.ltePdcpPerfStatsConfig);

    if(ltePdcpPerfStatsConfig_p->periodicReporting) 
    {
        if (ltePdcpPerfStatsConfig_p->cellPerfStatsToReportBitMap)
        {
            /* + PERF_CA */
            for(cellIndex = 0; cellIndex < pdcpContext.sCellCount; cellIndex++)
            {    
                sendPdcpCellPerfStatsInd(cellIndex);
            }
            /* - PERF_CA */
        }

        if(ltePdcpPerfStatsConfig_p->uePerfStatsToReportBitMap)
        {
            sendPdcpUePerfStatsInd();
        }
    }
    resetPdcpPerfContainer(PERF_STATS_CONTAINER_OAM_PDCP_RESET_CELL_CONTAINER);
    resetPdcpPerfContainer(PERF_STATS_CONTAINER_OAM_PDCP_RESET_UE_CONTAINER);
    g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].\
        perfStats.ltePdcpPerfStats.lastPDCPCalculatedTTI = 0; 
}
#endif

#ifdef PDCP_ASYNC_INTERFACE
/****************************************************************************
 * Function Name  : pdcpElRxProcessDecipheredPdu
 * Inputs         : count - number of nodes in deciphered queue
 * Outputs        : None
 * Returns        : None
 * Description    : Process all available PDCP Data PDUs that are deciphered 
 *                  by security engine but decompression yet to be done. This
 *                  EL will decipher the PDUs and form SDUs and will send it
 *                  to upper layer (packet relay)
 ****************************************************************************/
void pdcpElRxProcessDecipheredPdu(UInt64 count)
{
    LP_PdcpDataInd dataInd_p = PNULL;
    while (count--)
    {
        DEQUEUE_PDCP_DECIPHERED_RX_Q(dataInd_p);
        if (PNULL != dataInd_p)
        {
            pdcpRxProcessRoHC(dataInd_p);
        }
        else
        {
            /*Nothing to process*/
            LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                    __LINE__, 0,0,0,0, 0,0, __func__, "pdcpRxDecipheredQ_g");
            return;
        }
    }
}
/****************************************************************************
 * Function Name  : pdcpElPollSecurityEngine
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This EL will poll the security engine to retrive back the 
 *                  processed packets by security engine.
 ****************************************************************************/
void pdcpElPollSecurityEngine( void )
{
#ifdef POLL_JOB_RING
    /* + SPR 17073 */
    pdcpSecPollJobRing( (UInt8 *)(&pdcpSecSrbInputTxQ_g), RETRIEVE_ALL_PACKETS,
                         PNULL );
    pdcpSecPollJobRing( (UInt8 *)(&pdcpSecSrbInputRxQ_g), RETRIEVE_ALL_PACKETS,
                         PNULL );
    /* - SPR 17073 */
    pdcpSecPollJobRing( (UInt8 *)(&pdcpSecInputTxQ_g), RETRIEVE_ALL_PACKETS,
                         PNULL );
    pdcpSecPollJobRing( (UInt8 *)(&pdcpSecInputRxQ_g), RETRIEVE_ALL_PACKETS,
                         PNULL );
#else
    /* SPR 18122 Changes Start*/
    pdcpSecPoll( NUM_PACKETS_RETRIEVED, DIVIDE_BY_TWO(NUM_PACKETS_RETRIEVED), PNULL);
    /* SPR 18122 Changes End*/
#endif
}
#endif

/* + SPR 19066 */
/****************************************************************************
 * Function Name  : pdcpElAuditSendInactiveUE
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This EL will audit for inactive UEs in all the connected 
 *                  UEs in the system.
 ****************************************************************************/
void pdcpElAuditSendInactiveUE(InternalCellIndex cellIndex)
{
    LP_PdcpUeContext    ueCtx_p  = PNULL;
    LP_PdcpEnityContext ctx_p    = PNULL;
    tickType_t inactTimeVal   = 0;
    tickType_t inactTimeValDl = 0;
    tickType_t inactTimeValUl = 0;
    UInt16 listInactiveUe[MAX_UE_SUPPORTED] = {0};
    UInt16 numInactiveUe = 0;
    UInt16 ueIndex = 0;
    UInt8  lcId    = 0;
    UInt8  flag    = LTE_FALSE;

    while ( ueIndex < pdcpContext.numOfActiveUe)
    {
        /*
         ** activeUeInfo consists only of active UEs, hence valid UE context
         ** will always be available. Therefore, NULL check not required
         */
        ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(pdcpContext.activeUeInfo[ueIndex]);

        /* Check if inactivity timer configured by L3 for this UE .
         * Also check if the UE is attached on the cellIndex being processed.
         * if not, continue and check the next UE*/
        if(( 0 == ueCtx_p->inactivityTimerVal )
#ifndef UE_SIM_TESTING
                || ( ueCtx_p->cellIndex != cellIndex)
#endif
          )
        {
            /* Inactivity value is not configured for this UE */
            ueIndex++;
            continue;
        }
	    /* SPR 22439 fix start */
        flag=LTE_TRUE;
	    /* SPR 22439 fix end */
        while ( lcId < ueCtx_p->numOfActiveDrbLc)
        {
            /*
             ** activeLcInfo consists only of active DRBs in an UE, hence 
             ** valid PDCP context will always be available. Therefore, NULL
             ** check not required
             */
            ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                    ueCtx_p->activeLcInfo[lcId], PDCP_ENTITY_RB_TX );

    	    /* SPR 22439 fix start */
            if( (0 != QCOUNT_PDCP_TX_STORAGE_Q( ctx_p )) || (0 != QCOUNT_PDCP_RX_DATA_IND_Q(ctx_p)) )
            {
                /* Data present */
                flag = LTE_FALSE;
                break;
            }
	        /* SPR 22439 fix end */
            lcId++;
        }
        if( LTE_TRUE == flag )
        {

            /* calculate inactivity time value for DL */
            /* SPR23838 Fix Start */
            if( ueCtx_p->lastSchedTickDl <= pdcpCurrentTime_g )
            /* SPR23838 Fix End */
            {
                inactTimeValDl = ueCtx_p->lapsedInactiveTime + 
                    (pdcpCurrentTime_g - ueCtx_p->lastSchedTickDl);
            }
            else
            {
                /* Wrap around condition */
                inactTimeValDl = ueCtx_p->lapsedInactiveTime + (
                        pdcpCurrentTime_g + 
                        /* SPR23838 Fix Start */
                        (MAX_UINT64_VALUE - ueCtx_p->lastSchedTickDl) + 1);
                        /* SPR23838 Fix End */
            }
            /* calculate inactivity time value for DL */
            /* SPR23838 Fix Start */
            if( ueCtx_p->lastSchedTickUl <= pdcpCurrentTime_g )
            /* SPR23838 Fix End */
            {
                inactTimeValUl = ueCtx_p->lapsedInactiveTime +
                    (pdcpCurrentTime_g - ueCtx_p->lastSchedTickUl);
            }
            else
            {
                /* Wrap around condition */
                inactTimeValUl = ueCtx_p->lapsedInactiveTime +
                    ( pdcpCurrentTime_g + 
            /* SPR23838 Fix Start */
                      (MAX_UINT64_VALUE - ueCtx_p->lastSchedTickUl) + 1 );
            /* SPR23838 Fix End */
            }

            /* Find miminum of two */
            inactTimeVal = (inactTimeValDl < inactTimeValUl) ? 
                inactTimeValDl : inactTimeValUl;
            /* Check for UE inactivity */
            if( inactTimeVal > ueCtx_p->inactivityTimerVal )
            {
                /* UE is inactive */
                listInactiveUe[numInactiveUe] = 
                    pdcpContext.activeUeInfo[ueIndex];
                LOG_PDCP_INFO(MAC_RRC_INF,
                        "AUDIT_REPORT_FOR_INACTIVE_UES[%u]",ueIndex);

                ueCtx_p->lastSchedTickDl = pdcpCurrentTime_g;
                ueCtx_p->lastSchedTickUl = pdcpCurrentTime_g;
                numInactiveUe++;
            }
        }

        /*check if the numInactiveUe for a cell is equal to 10 PDCP_MAX_UE_AUDIT_REPORT.
         * if yes, then break and send the IND to RRC, as we can only process
         * max 10 UEs in singel PDCP_RRC_INACTIVE_UES_IND*/
        if(PDCP_MAX_UE_AUDIT_REPORT == numInactiveUe)
        {
            break;
        }

        lcId = 0;
        ueIndex++;
    }
    if( 0 != numInactiveUe )
    {
        /* Inactive UEs are present. Prepare and send the Inactive UEs to L3 */
        pdcpPrepareSendReport( numInactiveUe, listInactiveUe
#ifndef UE_SIM_TESTING
                ,cellIndex
#endif
                );
    }
}
/* - SPR 19066 */

extern int g_FrameRcvCnt ;
extern  int g_FrameHdlCnt ;
extern  int g_FrameRcvGetMemFailCnt ;
TimeSpec RateStasticsLastTime = {0};
UInt64 uplinkRlcIngressDataSize   = 0;
UInt64 uplinkRlcEgressDataSize    = 0;
UInt64 uplinkRlcIngressDataNum = 0;
UInt64 uplinkRlcEgressDataNum  = 0;
UInt64 uplinkIngressDataSize   = 0;
UInt64 uplinkEgressDataSize    = 0;
UInt64 uplinkIngressDataNum = 0;
UInt64 uplinkEgressDataNum  = 0;
void rlcPdcpRateStastics(void)
{
    TimeSpec currentTime;
    TimeSpec referTime;
    TimeSpec elapseTime;

   	/* Get the time in seconds and nanoseconds */
    clock_gettime_real_time(&currentTime);
	
	/* Get the elapsed time */
	elapseTime.tv_sec = currentTime.tv_sec - RateStasticsLastTime.tv_sec;
	
 	if (5 <= elapseTime.tv_sec)
	{
		
		if (uplinkRlcIngressDataNum > 0)
		{
			fprintf(stderr, "Pdcp Uplink ingress traffic rate: %llu kbps\n", 
						(uplinkIngressDataSize*8)/(1000 * elapseTime.tv_sec));
			fprintf(stderr, "Pdcp Uplink egress traffic rate: %llu kbps\n", 
						(uplinkEgressDataSize*8)/(1000 * elapseTime.tv_sec));		
			fprintf(stderr, "Pdcp Uplink ingress traffic num: %llu\n", 
						uplinkIngressDataNum);
			fprintf(stderr, "Pdcp Uplink egress traffic num: %llu\n", 
						uplinkEgressDataNum);

		
			fprintf(stderr, "Rlc Uplink ingress traffic rate: %llu kbps\n", 
						(uplinkRlcIngressDataSize*8)/(1000 * elapseTime.tv_sec));
			fprintf(stderr, "Rlc Uplink egress traffic rate: %llu kbps\n", 
						(uplinkRlcEgressDataSize*8)/(1000 * elapseTime.tv_sec));		
			fprintf(stderr, "Rlc Uplink ingress traffic num: %llu\n", 
						uplinkRlcIngressDataNum);
			fprintf(stderr, "Rlc Uplink egress traffic num: %llu\n\n", 
						uplinkRlcEgressDataNum);
		}

		if (0 == pdcpNumActiveUe())
		{
			uplinkRlcIngressDataSize   = 0;
			uplinkRlcEgressDataSize    = 0;
			uplinkRlcIngressDataNum = 0;
			uplinkRlcEgressDataNum  = 0;
			uplinkIngressDataSize   = 0;
			uplinkEgressDataSize    = 0;
			uplinkIngressDataNum = 0;
			uplinkEgressDataNum  = 0;
			return;
		}
	
#ifdef LINUX_PC_TEST
		fprintf(stderr, "g_FrameRcvCnt %d, g_FrameHdlCnt %d,g_FrameRcvGetMemFailCnt %d\n", 
					g_FrameRcvCnt, g_FrameHdlCnt,g_FrameRcvGetMemFailCnt);
#endif		
		
		uplinkRlcIngressDataSize	 = 0;
		uplinkRlcEgressDataSize	 = 0;		
		uplinkIngressDataSize	 = 0;
		uplinkEgressDataSize	 = 0;

		RateStasticsLastTime.tv_sec = currentTime.tv_sec;
	}
}



#ifdef LINUX_PC_TEST
void l2oldcaseinitpdcp(void)
{
	pdcpContext.sCellCount = 1;
}
#endif
