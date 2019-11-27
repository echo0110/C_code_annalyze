/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacExecutionLegs.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacExecutionLegs.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 * Revision 1.22.24.9  2010/11/11 12:41:19  gur23792
 * added tdd flag
 *
 * Revision 1.22.24.8  2010/11/11 12:38:33  gur23792
 * tb size determination
 *
 * Revision 1.22.24.7  2010/10/23 12:41:33  gur19413
 * self review fixes
 *
 * Revision 1.22.24.6  2010/10/21 12:23:56  gur24420
 * 1.Comments updated in TDD manager file.
 * 2.DL Strategy function call during the special subframe is
 *   handeled based on Special subframe configuration.
 *
 * Revision 1.22.24.5  2010/10/18 06:49:31  gur23792
 * *** empty log message ***
 *
 * Revision 1.22.24.4  2010/09/29 07:25:01  gur24420
 * Bugs fixed in UT
 *
 * Revision 1.22.24.3  2010/09/23 05:27:43  gur24420
 * cosmetic changes
 *
 * Revision 1.22.24.2  2010/09/20 13:50:55  gur19413
 * bug fixed
 *
 * Revision 1.22.24.1  2010/08/27 13:00:52  gur24420
 * Added function prototypes, macro's and structure definition for TDD
 *
 * Revision 1.22  2009/12/30 16:12:33  gur15697
 * PROCESS_UL_RA_BSR changed to PROCESS_UL_RA
 *
 * Revision 1.21  2009/12/27 12:09:33  gur11912
 * added FOUR_FLOW_ONE_DELAY_ALTERNATE model
 *
 * Revision 1.20  2009/12/18 10:46:51  gur11912
 * added DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY
 *
 * Revision 1.19  2009/11/29 09:22:45  gur11912
 * modified NUM_DEMUX_MSGS_TO_PROCESS to 1
 *
 * Revision 1.18  2009/11/01 11:39:00  gur11912
 * defined NUM_CQI_NODES
 *
 * Revision 1.17  2009/10/25 11:01:42  gur11912
 * separated PROCESS_SCHEDULING_STATS into DL and UL
 *
 * Revision 1.16  2009/10/25 08:44:32  gur11912
 * modified NUM_STATS_NODES to 1
 *
 * Revision 1.15  2009/10/15 03:51:25  gur11912
 * modified OAM RRC msgs to be processed
 *
 * Revision 1.14  2009/08/30 19:41:37  gur19413
 * FOUR_FLOW_MODEL as default
 *
 * Revision 1.13  2009/07/17 10:15:01  gur11912
 * Added four flow one delay model
 *
 * Revision 1.12  2009/07/14 12:00:00  gur18569
 * removed compilation errors for 2 and 4 flows
 *
 * Revision 1.11  2009/07/02 15:11:03  gur18569
 * added leg to only send data msgs
 *
 * Revision 1.10  2009/07/02 07:53:44  gur18569
 * added leg process pending delete UE
 *
 * Revision 1.9  2009/06/29 04:12:30  gur18569
 * enabled one flow execution scheduler
 *
 * Revision 1.8  2009/06/25 09:29:11  gur18569
 * process CQI if time left was defined twice
 *
 * Revision 1.7  2009/06/22 06:45:15  gur18569
 * changed subFrame and sysFrame data size to 32 bits
 *
 * Revision 1.6  2009/06/17 05:02:13  gur18569
 * added new leg enums (stats and cqi)
 *
 * Revision 1.5  2009/06/15 09:42:42  gur18569
 * changes in test exec flow for testing
 *
 * Revision 1.4  2009/06/10 11:29:29  gur18569
 * incorporation of comments
 *
 * Revision 1.3  2009/06/09 13:11:30  gur18569
 * incorporated review comments
 *
 * Revision 1.2  2009/06/03 10:42:38  gur18569
 * incorporated comments
 *
 * Revision 1.1  2009/05/29 14:27:09  gur18569
 * Initial version
 *
 *
 *
 ****************************************************************************/

#ifndef EXECUTION_SCHEDULER_LEG_H
#define EXECUTION_SCHEDULER_LEG_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteCommonStatsManager.h"
#include "lteMacTypes.h"
#include "lteMacExecutionScheduler.h"
/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
//#include "lteMacULResourceAllocator.h"
#endif
/*TDD Config 0 Changes End*/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define MAX_EXECUTION_FLOWS               16
#define NUM_DEMUX_MSGS_TO_PROCESS          1 
#define NUM_RRC_OAM_MSGS_TO_PROCESS        1
#define NUM_STATS_NODES                    1
#define NUM_PHY_MSGS_TO_PROCESS           10
#define WAIT_AFTER_REG_REQ_SENT     20000000
/* + CQI_4.1 */
#define NUM_CQI_NODES                      15
/* - CQI_4.1 */
 /* SPR 4073 Start*/
#define NUM_PERIODIC_REPORT_NODES          MAX_UE_SUPPORTED
 /* SPR 4073 End*/
 
#ifdef TDD_CONFIG
#define DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY 0
#elif FDD_CONFIG
/* START SPR 1350 CHANGES */
/*SPR _8144_fix_start*/
#define DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY ((4) - (PHY_DL_DELAY) )
/*SPR _8144_fix_end*/
/* END SPR 1350 CHANGES */
#endif

#define EXEC_LEG_MANAGER "EXEC_LEG_MANAGER"
/*SPR 16855 +-*/
#ifdef DL_UL_SPLIT
#define NUM_OF_THREADS ( NUM_OF_EXEC_FLOWS + 1 )
#define RRC_OAM_FLOW_ID (NUM_OF_THREADS - 1)
#endif

/* The current flow model*/

#ifdef TDD_CONFIG
#define TDD_MANAGER               "TDD_MANAGER"
#ifdef DL_UL_SPLIT_TDD
#ifndef NUM_OF_EXEC_FLOWS
#define NUM_OF_MAC_EXEC_FLOWS         2
#define NUM_OF_EXEC_FLOWS             NUM_OF_MAC_EXEC_FLOWS+1
#endif
#else
#define NUM_OF_EXEC_FLOWS             1
#endif
#define MAX_DELAY_PATH                1
#define NUM_OF_DEMUX_THREADS          1
#elif FDD_CONFIG
#ifndef DL_UL_SPLIT
#define ONE_FLOW_ONE_DELAY
#endif
#endif

#ifdef DL_UL_SPLIT
#ifndef NUM_OF_EXEC_FLOWS
/*SPR 16855 Fix Start*/
/*SPR 16855 Fix Start*/
#define NUM_OF_EXEC_FLOWS             3
/*SPR 16855 Fix End*/
#endif
#define MAX_DELAY_PATH                1
#define NUM_OF_DEMUX_THREADS          1
#define DELAY_DUE_TO_SPLIT            1
#endif

#ifdef ONE_FLOW_ONE_DELAY

#define NUM_OF_EXEC_FLOWS             1
#define MAX_DELAY_PATH                1
#define NUM_OF_DEMUX_THREADS          1
/** CA 2 Core Changes Start **/
#define MIN_NUM_EXEC_FLOW             1
/** CA 2 Core Changes End **/

#endif 


/****************************************************************************
 * Exported Types
 ****************************************************************************/
#ifdef TDD_CONFIG
typedef enum TypeSubFrameT
{
    NORMAL_SUB_FRAME,
    SPECIAL_SUB_FRAME
}TypeSubFrame;
#endif

typedef struct ExecutionLegInfoT
{
    ExecutionLeg executionLeg;
    UInt32 legSpecificArg1;
    UInt32 legSpecificArg2;
    UInt32 legSpecificArg3;
}ExecutionLegInfo;

typedef struct ExecutionLegFunctionArgT
{
    UInt8 execFlowIndex;
    /* CA Changes start */
    InternalCellIndex cellIndex;
    /* CA Changes start */
    UInt32 subFrame;
    UInt32 sysFrame;
/* SPR 21660 changes start_vishal */
/* SPR 21660 changes end_vishal */ 
    /* SPR 15909 fix start */
    tickType_t globalTick;
    /* SPR 15909 fix end */
    UInt32 delayToPhy;
#ifdef TDD_CONFIG    
    /* SPR 15909 fix start */
    tickType_t advanceDLGlobalTick;
    /* SPR 15909 fix end */
    UInt32 advanceDLSubframe;
    UInt32 advanceDLSystemframe;
#endif    
    UInt32 execLegSpecificArg1;
    UInt32 execLegSpecificArg2;
    UInt32 execLegSpecificArg3;

    UInt8* execLegSpecificArg4;
    SInt32 execLegSpecificArg5;
#ifdef TDD_CONFIG
    UInt8* execLegSpecificArg6;
    SInt32 execLegSpecificArg7;
/* TDD DLTG inside Fix */
/* SPR 21804 Fix Start */
#if defined(LAYER2_DLTG_THREAD) && !defined(PDCP_GTPU_INTF)
/* SPR 21804 Fix End */
    UInt8* execLegSpecificArg8;
    SInt32 execLegSpecificArg9;
    UInt8* execLegSpecificArg10;
    SInt32 execLegSpecificArg11;
#endif
/* TDD DLTG inside Fix */
#endif
    TimeSpec * executionFlowStart_p;
}ExecutionLegFunctionArg;

typedef void (* ExecutionLegFunctionPtr)(ExecutionLegFunctionArg *);

#ifdef TDD_CONFIG
typedef void (* TddPdcchFunctionPtr)( UInt32 withMuxFlag,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        UInt32 SFN,
        UInt32 subFrameNum,
        /* SPR 15909 fix end */
        UInt8  ulSubFrameNum,
        InternalCellIndex internalCellIndex,
        /* TDD Config 0 Changes Start */
        UInt8 ulSubframeNumForDlAckNack,
        /* TDD Config 0 Changes End */
        ExecutionLegFunctionArg* execArgs
	);
typedef UInt32 (*TddDLSchedulerStrategyFtnPtr) (
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 dlDelay,
        InternalCellIndex internalCellIndex
          );

#ifdef LTE_EMBMS_SUPPORTED
typedef UInt32 (*EmbmsDLSchedulerStrategyFtnPtr) (
        UInt32 dlDelay,
        InternalCellIndex internalCellIndex
          );
#endif

typedef MacRetType (* TddPuschFunctionPtr)( UInt8 ulSubFrameNum,
                                      /* SPR 15909 fix start */
                                      tickType_t currentGlobalTick,
                                      /* SPR 15909 fix start */
                                      UInt16 SFN,
                                      UInt8  subFrameNum,
                                      InternalCellIndex internalCellIndex,
                                      ExecutionLegFunctionArg* execArgs);
typedef struct TDDSubFrameInfoT
{
    UInt8  subFrameExecCount;
#ifdef OVERSHOOT_CORRECTION
    UInt8 mandatoryExecutionLegCount;    
#endif /*OVERSHOOT_CORRECTION*/
    ExecutionLegInfo executionLegArr[MAX_EXECUTION_LEGS];
}TDDSubFrame;

/* + SPS_TDD_Changes */
/*coverity 87552:Fix*/
typedef void (*TddDLSPSSchedulerStrategyFtnPtr) (
        UInt32 currentGlobalTTITickCount,
        UInt32 dlDelay,
	InternalCellIndex internalCellIndex
          );
/* - SPS_TDD_Changes */
#endif
/** El Sync Changes Start **/
#define MAX_ELS_PER_SCH_FN 26
#define MAX_EL_SUB_GROUP 20

typedef struct ExecFlowFunctionalitySetT{
 UInt32 mandatoryElCount;
 UInt32 optionalElCount;
#ifdef FDD_CONFIG
 ExecutionLegInfo executionLegArr[MAX_ELS_PER_SCH_FN]; 
#else
 ExecutionLegInfo schSetConstituentELsAndArgs[MAX_ELS_PER_SCH_FN]; 
#endif
 }execFlowFunctionalitySet,SchedulerFunctionalitySet;


typedef struct ExecutionFlowArgumentsT
{
	ExecutionLeg startExecutionLeg;
	ULong32 threadBindMask;

	UInt32  threadIndex;
#ifdef TDD_CONFIG
	UInt8 maxSubFrameLoopCount;
#ifdef DL_UL_SPLIT_TDD
    UInt16 coreId;
#endif
    TDDSubFrame subFrameInfo[MAX_SUB_FRAME];
/*CA Changes start*/
    UInt8 internalCellIndex;
/*CA Changes start*/
#elif FDD_CONFIG
	UInt8 dlPathDelay;
    /** EL Sync Changes Start **/
    UInt8 internalCellIndex;
#if (!defined(DL_UL_SPLIT) && !defined(DL_DATA_SEPARATION)) 
	UInt8 executionFlowSubGruopCount;
    execFlowFunctionalitySet execFlowFuncSubGroup[MAX_EL_SUB_GROUP] ;
#elif defined (DL_DATA_SEPARATION) || defined (DL_UL_SPLIT)
	UInt8 executionLegCount;
#ifdef DL_UL_SPLIT
/*DL_UL_SPLIT FIX:START*/
#ifdef OVERSHOOT_CORRECTION
/*DL_UL_SPLIT FIX:END*/
	UInt8 mandatoryExecutionLegCount;    
/*DL_UL_SPLIT FIX:START*/
#endif
/*DL_UL_SPLIT FIX:END*/
#endif /*DL_UL_SPLIT*/
	ExecutionLegInfo executionLegArr[MAX_EXECUTION_LEGS];
#endif
/** El Sync Changes End **/
#endif /*TDD_CONFIG or FDD_CONFIG*/

}ExecutionFlowArguments;

typedef struct ExecutionFlowThreadCreationParamT
{
    ULong32 threadId;
    SInt32  threadPriority;
    ExecutionFlowArguments threadArguments;
}ExecutionFlowCreationParam;


#ifdef DL_DATA_SEPARATION
typedef struct EncoderExecutionFlowArgumentsT
{
    ExecutionLeg startExecutionLeg;
    ULong32 threadBindMask;
    UInt32  threadIndex;
    UInt8 dlPathDelay;
    UInt8 executionLegCount;
    /* CA Changes start */
    InternalCellIndex cellIndex;
    /* CA Changes end */
	ExecutionLegInfo executionLegArr[MAX_EXECUTION_LEGS];

}EncoderExecutionFlowArguments;

typedef struct EncoderExecutionFlowCreationParamT
{
    ULong32 threadId;
    SInt32  threadPriority;
    EncoderExecutionFlowArguments threadArguments;
}EncoderExecutionFlowCreationParam;

#endif


/* DL_UL_SPLIT_AB+ */
#ifdef DL_UL_SPLIT_TDD 
typedef struct ExecutionFlowArgumentsRlcT
{
    ExecutionLeg startExecutionLeg;
    ULong32 threadBindMask;
    UInt32  threadIndex;
    UInt8 dlPathDelay;
    UInt8 executionLegCount;
    UInt16 coreId;
    ExecutionLegInfo executionLegArr[MAX_EXECUTION_LEGS];
}ExecutionFlowArgumentsRlc;

typedef struct ExecutionFlowThreadCreationParamRlcT
{
    ULong32 threadId;
    SInt32  threadPriority;
    ExecutionFlowArgumentsRlc threadArguments;
}ExecutionFlowCreationParamRlc;
#endif
/* DL_UL_SPLIT_AB- */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*This is posted by Transmitter after sending control messages to phy*/
/* This is an array of execution leg function pointers*/
#ifdef FDD_CONFIG
extern ExecutionLegFunctionPtr executionFunctionArr_g[MAX_EXECUTION_LEGS];
#elif TDD_CONFIG
extern ExecutionLegFunctionPtr  executionFunctionArr_g[MAX_EXECUTION_LEGS];
#endif

/* If the value of this global is true it means UL scheduler strategy is 
 * already executed*/
extern UInt8 checkIfUlStrategyAlreadyDone_g;

/* This semaphore is to send trigger to PUSCH from PDCCH that PDCCH 
 * processing is done*/


/* ulRlcWaitForSubFrameIndSem_g is a global  of semaphores used in
 * execution flow which need to wait of sub frame indication.In this array
 * the semaphore on index 0 will receive sub frame change indication upon which
 * the rest of semaphores are posted.*/
/* CA Start changes */
extern SEM_T ulRlcWaitForSubFrameIndSem_g;
/* This global tick is used for Log/HSM */
 /* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
/* CA end changes */
#ifdef DL_UL_SPLIT

#elif DL_UL_SPLIT_TDD
extern sem_t waitForSubFrameIndSemArr_g;
#ifdef FAPI_CONTROL_FLOW
extern sem_t waitForFAPIMsgControlFlowInd_g;
#endif
#else
extern LTE_SEM waitForSubFrameIndSemArr_g[NUM_OF_EXEC_FLOWS];
#endif


#ifdef FDD_CONFIG
extern ExecutionFlowCreationParam executionFlowCreationParamArr_g[NUM_OF_EXEC_FLOWS];
/** EL Sync Changes Start **/
#if (!defined(DL_UL_SPLIT) && !defined(DL_DATA_SEPARATION))
extern SchedulerFunctionalitySet SchFnSet_g[MAX_EL_SUB_GROUP] ;


#endif
/** EL Sync Changes End **/
/* SPR 21660 changes start_vishal */
/* SPR 21660 changes end_vishal */

#elif TDD_CONFIG
/* SPR 13768 Changes Start */
extern ExecutionFlowCreationParam *executionFlowCreationParamArr_g[MAX_NUM_CELL];
/* SPR 13768 Changes End */
#endif

#ifdef DL_DATA_SEPARATION
extern EncoderExecutionFlowCreationParam
    executionFlowCreationParamArrForDLDataThread_g[NUM_OF_EXEC_FLOWS];
#endif

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
#ifdef FDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
/* + SPR 17439 */
void initExecutionLegFunctionArr(void);
/* - SPR 17439 */
/* SPR 5599 changes end (ZIP ID 129059) */
#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
/* + SPR 17439 */
void initTddExecutionLegFunctionArr(void);
/* - SPR 17439 */
/* SPR 5599 changes end (ZIP ID 129059) */
#endif
/*rel4-drop2 changes */
/*SPR 16855 Fix Start*/
void* callFlowThreadExecutionSchedular(void * param);
/*SPR 16855 Fix End*/
/* This function is not calling in FAPI case Need to be added FAPI flag here*/
/* + SPR 17439 */
void registerWithPhyForSubFrameInd(void);
/* - SPR 17439 */
/* SPR 609 changes start */   
#ifdef TDD_CONFIG
 /* SPR 15909 fix start */
MacRetType expireAllNodeFromULHarqTimerList( tickType_t recvTTI,
 /* SPR 15909 fix start */
        UInt16 sfn,
        UInt8  prevULSubFrame,
        UInt8  ulSfnFactor,
        UInt8 freeHarqOnly,
        /* SPR 11110 fix start */
        UInt8 allHarq,
        /* SPR 11110 fix end */
        InternalCellIndex cellIndex
        );
#endif
/*T2200 specific changes start*/
void prepareULConfigMsg (
#ifdef TDD_CONFIG
        UInt8  newSf,
#else
        UInt32 phyDelay,
#endif
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        UInt32 systemFrameNum,
        UInt8 subFrameNum,
        InternalCellIndex internalCellIndex);
		
/*T2200 specific changes end*/		
/* SPR 5346 changes start */                                 
/* +- SPR 17777 */
void dummyPrepareULConfigMsg(
        UInt32 systemFrameNum,
        UInt8 subFrameNum,
        InternalCellIndex internalCellIndex);
/* SPR 5346 changes end */                                 
/* +- SPR 17777 */
void txULConfigMsg (
        UInt32 systemFrameNum,
        UInt8 subFrameNum,
        InternalCellIndex internalCellIndex);
/* FDD_SRS */
#ifdef FDD_CONFIG
/*CA Changes start  */
 /* SPR 15909 fix start */
void processSrsReports(UInt32 currSubFrameNum, UInt8 ulDelay, tickType_t globalTick, InternalCellIndex cellIndex);
/*CA Changes end  */
#elif TDD_CONFIG
void processSrsForUplinkSubframes(UInt8 ulSubFrameNum, tickType_t globalTick, InternalCellIndex internalCellIndex);
void processSrsForSpecialSubframes(UInt8 ulSubFrameNum, tickType_t globalTick, InternalCellIndex internalCellIndex);
 /* SPR 15909 fix end */
#endif
/* FDD_SRS */
 /* SPR 15909 fix start */
extern void processDRXForUEs(tickType_t globalTick, InternalCellIndex internalCellIndex);  /* DRX_CHG */
 /* SPR 15909 fix end */
/*CA Changes start  */
 /* SPR 15909 fix start */
extern void processAllMeasGapOffsetMaps (tickType_t globalAdvanceTick, InternalCellIndex internalCellIndex); /* MEAS_GAP_CHG */ 
 /* SPR 15909 fix end */
/*CA Changes end  */
                             /* MEAS_GAP_CHG */
/* + SPR 17439 */
UInt32 rlcHandleRxExpiredTimer(void);
/* - SPR 17439 */
void cleanupLayer2(void);
void pdcpElTxProcessSocket(UInt8 *buf,UInt32 length);
UInt32 rlcRcvMsgOnSocket(void);
void* pdcpAdapterThreadExecutionSchedular(void * param);

#ifdef LTE_EMBMS_SUPPORTED
void* syncAdapterThreadExecutionSchedular(void * param);
#endif
/* SPR 609 changes end */   

/* ICIC changes start */
void processDLRrmReconfiguration(InternalCellIndex internalCellIndex);
void processULRrmReconfiguration(InternalCellIndex internalCellIndex);
void processEicicConfiguration(InternalCellIndex internalCellIndex); /* EICIC +-*/
/*Complexity Start*/
void processEicicConfigurationforAggressor(InternalCellIndex internalCellIndex);
void processEicicConfigurationforVictim(InternalCellIndex internalCellIndex);
/*Complexity End*/
/* ICIC changes end */
 /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
/* + SPR 17439 */
void lteMacUpdateKpiStatsIPThpUlOnDequeue(void);
/* - SPR 17439 */
#endif

/** El Sync Changes Start **/
/* + SPR_14897 */
#ifndef DL_UL_SPLIT
/* - SPR_14897 */
 void ElSetStrategyFd (ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElCheckStrategyFd (ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElSetPdcchPdschFd (ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElCheckPdcchPdschFd (ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElSetMuxFd (ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElCheckMuxFd (ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElSetEncoderFd (ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElCheckEncoderFd (ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElProcessPendingEventFds (ExecutionLegFunctionArg * execLegFuncArg_p);
#endif

/** El Sync Changes End **/
#ifdef LTE_EMBMS_SUPPORTED
 void ElEmbmsStrategy(ExecutionLegFunctionArg * execLegFuncArg_p);
 void ElProcesRlcPreOpportunity(ExecutionLegFunctionArg * execLegFuncArg_p);
#endif

 /* 32.425 Events code changes end*/
#endif  /* EXECUTION_SCHEDULER_LEG_H */
