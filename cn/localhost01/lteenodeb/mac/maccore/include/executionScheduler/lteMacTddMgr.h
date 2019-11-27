/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacTddMgr.h,v 1.1.2.13 2010/10/29 11:58:28 gur24420 Exp $
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
 * $Log: lteMacTddMgr.h,v $
 * Revision 1.1.2.13  2010/10/29 11:58:28  gur24420
 * 1. Code rework changes 
 * 2. Self review changes
 *
 * Revision 1.1.2.12  2010/10/28 15:07:35  gur19413
 * Support for DL Advance and PHY delay
 *
 * Revision 1.1.2.11  2010/10/27 09:16:21  gur19413
 * self code review comments fixed
 *
 * Revision 1.1.2.10  2010/10/25 11:35:45  gur24420
 * Added pointer to hold Ack/Nack Handling function for current UL/DL configuration
 *
 * Revision 1.1.2.9  2010/10/25 08:16:19  gur19413
 * removed magic number and dead code
 *
 * Revision 1.1.2.8  2010/09/23 13:17:45  gur24420
 * Bug fixes during IT
 *
 * Revision 1.1.2.7  2010/09/23 05:27:43  gur24420
 * cosmetic changes
 *
 * Revision 1.1.2.6  2010/09/20 13:51:08  gur19413
 * bug fixed
 *
 * Revision 1.1.2.5  2010/09/09 19:01:40  gur24420
 * Testing
 *
 * Revision 1.1.2.4  2010/09/07 11:08:48  gur24420
 * Review changes for DL
 *
 * Revision 1.1.2.3  2010/09/01 10:39:31  gur24420
 * Transmitter changes
 *
 * Revision 1.1.2.2  2010/08/31 07:53:32  gur19413
 * convert dos2unix
 *
 * Revision 1.1.2.1  2010/08/27 13:06:07  gur24420
 * Added New file
 *
 *
 *
 ****************************************************************************/

#include "lteMacTypes.h"
#include "lteMacPhyInterface.h"
#include "lteMacRandomAccess.h"
#include "lteMacTDDConfig.h"
#include "lteMacCellConfMgr.h"


#ifndef LTE_MAC_TDD_MGR_H
#define LTE_MAC_TDD_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

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
extern ExecutionLegFunctionPtr executionFunctionArrTdd_g[MAX_EXECUTION_LEGS];

extern UInt8 specialSubFrameConfig_g;
extern UInt8 ulDlCofiguration_g;
extern tddPutEntryInAckNackQueueForDiffConfig tddPutEntryInAckNackQueueForCurConfig_p[MAX_NUM_CELL];
#ifdef DL_UL_SPLIT_TDD
extern sem_t waitForCellSetupIndSemArr_g[];
extern SInt32 rlcRrcCnfInputQId_g;
extern SInt32 rlcRrcCnfOutputQId_g;
extern void initRlcRrcCnfQueue ( SInt32 *);
extern void processRxFirstSubFrameIndUl(void);
#endif
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
#define MAC_TDD_DL_ADVANCE_1

#ifdef MAC_TDD_DL_ADVANCE_2
#define DL_ADVANCE_TICK 2
#endif

#ifdef MAC_TDD_DL_ADVANCE_1
#if  defined FLEXRAN
#define DL_ADVANCE_TICK 0
#else
#define DL_ADVANCE_TICK 1
#endif
#endif


/* TDD SB_CQI */
#define MAX_DELAY_TDD 1
/* TDD SB_CQI */

/*
 * Enumerations for DIfferent Special Subframe configuration
 */

typedef enum SpecialSubFrameConfigT
{
    SPECIAL_SUB_FRAME_CONFIG_0,
    SPECIAL_SUB_FRAME_CONFIG_1,
    SPECIAL_SUB_FRAME_CONFIG_2,
    SPECIAL_SUB_FRAME_CONFIG_3,   
    SPECIAL_SUB_FRAME_CONFIG_4,   
    SPECIAL_SUB_FRAME_CONFIG_5,
    SPECIAL_SUB_FRAME_CONFIG_6,    
    SPECIAL_SUB_FRAME_CONFIG_7,
    SPECIAL_SUB_FRAME_CONFIG_8,   
    SPECIAL_SUB_FRAME_CONFIG_MAX    
}SpecialSubFrameConfig;

/*
 * Enumeration for Different Types of Ack/Nack 
 * Handling in TDD for DL Data Transmission
 */
typedef enum DlAckNackModeT
{
     ACK_NACK_BUNDLING = 0, 
     ACK_NACK_MULTIPLEXING 
}DlAckNackMode;


/*
 * Enumeration for indexing different types of PDCCH & PDSCH 
 * Function pointers
 */
typedef enum PdcchPdschTypeT
{
    PDCCH_DL_AND_UL_PDSCH_NORMAL = 0,
    PDCCH_DL_AND_UL_PDSCH_TRUNCATED,
    PDCCH_DL_ONLY_PDSCH_NORMAL,
    PDCCH_DL_ONLY_PDSCH_TRUNCATED,
    PDCCH_UL_ONLY,
    PDCCH_DUMMY,     /* For special SF config 0,1,5,6 */
    MAX_PDCCH_PDSCH_TYPE
}PdcchPdschType;

#ifdef LTE_EMBMS_SUPPORTED
typedef enum EmbmsSchTypeT
{
    EMBMS_SCH = 0,
    EMBMS_DUMMY,     /* For Non EMBMS subframes  */
    MAX_EMBMS_SCH_TYPE
}EmbmsSchType;
#endif
/*
 * Enumeration for indexing function pointer for calculating 
 * the number of CCE during different scenarios
 */
typedef enum TxDlControlTypeT
{
    TX_DL_CONTROL_WITH_HI_PDU = 0,
    TX_DL_CONTROL_WITHOUT_HI_PDU,
    MAX_TX_DL_CONTROL_TYPE
}TxDlControlType;

/*
 * Enueration for indexing different types of reception 
 * functions
 */

typedef enum RxTypeT

{
    RX_WITH_COMPLETE_UL_DATA_ACK_NACK_BUNDLING,
    RX_WITH_COMPLETE_UL_DATA_ACK_NACK_MULTIPLEXING,		
    RX_WITH_SHORT_RACH_AND_SRS_ONLY,
    MAX_RX_TYPE
}RxType;

/*
 * Enumeration for indexing different kinds of 
 * ACK/NACK Processing functions
 */
typedef enum PucchAckNackNodeT
{
    PUCCH_WITH_ACK_NACK_BUNDLING,
    PUCCH_WITH_ACK_NACK_MULTIPLEXING	,
    MAX_PUCCH_ACK_NACK_NODE
}PucchAckNackNode;



/*
 * Enumeration for indexing function pointer table for
 * Uplink processing for UL-DL configuration scenarios.
 */

typedef enum UplinkHandlingPolicyT
{
    /* In case of UL/DL Configuration 0 due to the
        factor of n + 7 the uplink handling changes
        significantly hence a separate execution is required*/
    UL_HANDLING_FOR_UL_DL_CONFIG_0 = 0,
    UL_HANDLING_NORMAL,
    MAX_UPLINK_HANDLING_POLICY
}UplinkHandlingPolicy;

/*
 * Enumeration for SFN factor
 */
typedef enum SysFrameBackT
{
     SYSTEM_FRAME_BACK_0 = 0,
     SYSTEM_FRAME_BACK_1,
     SYSTEM_FRAME_BACK_2
}SysFrameBack;


typedef enum DlHarqRttTimerT
{
    DL_HARQ_RTT_TIMER_4_PLUS_4 = 8,
    DL_HARQ_RTT_TIMER_5_PLUS_4,
    DL_HARQ_RTT_TIMER_6_PLUS_4,
    DL_HARQ_RTT_TIMER_7_PLUS_4,
    DL_HARQ_RTT_TIMER_8_PLUS_4,
    DL_HARQ_RTT_TIMER_9_PLUS_4,
    DL_HARQ_RTT_TIMER_11_PLUS_4 = 15,
    DL_HARQ_RTT_TIMER_12_PLUS_4,    
    DL_HARQ_RTT_TIMER_13_PLUS_4    
}DlHarqRttTimer;

/*
 * Enumeration for Different Expiry time
 */

typedef enum DlHarqExpiryTimeT
{
    DL_HARQ_RTT_TIME_8 = 8,
    DL_HARQ_RTT_TIME_9,
    DL_HARQ_RTT_TIME_10,
    DL_HARQ_RTT_TIME_11,
    DL_HARQ_RTT_TIME_12,
    DL_HARQ_RTT_TIME_13,
    DL_HARQ_RTT_TIME_14,
    DL_HARQ_RTT_TIME_15,
    DL_HARQ_RTT_TIME_16,
    DL_HARQ_RTT_TIME_17,
    DL_HARQ_RTT_TIME_18,
    DL_HARQ_RTT_TIME_19,
    DL_HARQ_RTT_TIME_20
}DlHarqExpiryTimer;

/*
 * Pointers to store the above array's for a particular UL-DL config
 */
/** CA-TDD Changes Start **/
extern UInt8 *tddDci0DaiSfIndex_p[MAX_NUM_CELL];
extern UInt8 *tddValidateDai_p[MAX_NUM_CELL];
/** CA-TDD Changes End **/

/*
 * Pointer to hold the PDCCH and PDSCH indexer for current 
 * UL/DL config and Special Subframe configuration configured by 
 * RRC
 */
/** CA-TDD Changes Start **/
extern UInt32 *tddMacPdcchPdschFtnPtrIndexer_p[MAX_NUM_CELL];
/** CA-TDD Changes End **/
extern TddPuschFunctionPtr puschFunctionArrTdd_p;

#ifdef LTE_EMBMS_SUPPORTED
extern UInt32 *tddMacEmbmsFtnPtrIndexer_p[MAX_NUM_CELL]; 
#endif
/*
 * Prototype for the thread executes execution legs passed to it through
 * arguments.
 */
#ifndef TDD_CONFIG
void* executionFlowThread(void * executionFlowThreadArgs_p);
#else
/* CA TDD Changes Start */
void executionSingleTDDFlowThread(InternalCellIndex internalCellIndex);
/* CA TDD Changes End */
#endif
/*
 * Protype for the function which creates the execution flow thread 
 */
MacRetType createExecutionFlowThreads( ExecutionFlowCreationParam *
                                             executionFlowThreadParamArr_p);

#ifdef TDD_CONFIG

typedef enum SchedulerFunctionalityTypeT{
    FN_DL_ALLOC_ONLY,
    FN_UL_ALLOC_ONLY,
#ifndef DL_UL_SPLIT_TDD    
    FN_DL_UL_ALLOC,
#endif    
    /* TDD Config 0 Changes Start */
    /* Function set index for TDD Config 0 S Subframe handling */
    FN_DL_UL_ALLOC_S_SUBFRAME,
    /* Function set index for UL Config and SRS handling */
    FN_UL_CONFIG_SRS,
    /* TDD Config 0 Changes End */
    FN_SSF_CONFIG,
    FN_DL_HARQ,
    FN_UL_HARQ,
    /* TDD Config 0 Changes Start */
    /* Function set index for UL Harq handling on D Subframe */
    FN_UL_HARQ_D_SUBFRAME,
    /* TDD Config 0 Changes End */
    FN_MANDATORY_MISC,
#ifndef DL_UL_SPLIT_TDD    
    FN_OPTIONAL_MISC,
#else
    FN_UL_SYNC_DCI0,
    FN_OPTIONAL_MISC_DL,
    FN_UL_OPTIONAL_AND_DLRLC,
#endif
    FN_MAX
}SchedulerFunctionalityType;


typedef enum ElArgumentTypesT{
    ELARG_INVALID,
    ELARG_DL_SF_TO_WORK_FOR,
    ELARG_UL_SF_TO_WORK_FOR,
    /* TDD Config 0 changes start */
    /* EL Argument for 2nd UL SF for TDD config 0 */
    ELARG_2ND_UL_SF_FOR_CONFIG0_TO_WORK_FOR,
    /* EL Argument for 4th tick from working sf is UL SF */
    ELARG_4TH_TICK_UL_SF_TO_WORK_FOR,
    /* EL Argument for 4th tick from working sf is UL SF or SPL SF */
    ELARG_4TH_TICK_UL_SPL_SF_TO_WORK_FOR,
    /* TDD Config 0 changes end */
    ELARG_SPL_SF_TO_WORK_FOR,
    ELARG_DL_HARQ_ACKNACK_U_SF,
    ELARG_DL_HARQ_EXPIRY_U_SF,
    ELARG_UL_HARQ_EXPIRY_U_SF,
    /* TDD Config 0 Changes Start */
    /* EL Argument for UL Harq expiry for 2nd U SF */
    ELARG_UL_HARQ_EXPIRY_2ND_U_SF,
    /* TDD Config 0 Changes End */
    ELARG_CLOSEST_PDSCH_DELAY,
#ifdef DL_UL_SPLIT_TDD
    ELARG_DL_PERF_STATS,
    ELARG_UL_PERF_STATS,
#endif
    ELARG_MAX
}ElArgumentTypes;

typedef struct ElArgumentsT{
    UInt32 elArgArray[ELARG_MAX];
}ElArguments;

/** CA-TDD Changes Start:06 **/
/* DL SF for which PDCCH/PDSCH etc will work */
#define SET_ARG_DL_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex) \
do{\
    elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_DL_SF_TO_WORK_FOR] = workingSf;\
}while(0)

/* UL SF for which PUSCH/PUCCH/PRACH is to be allocated and UL Config prepared */
#define SET_ARG_UL_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex) \
do{\
    elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_UL_SF_TO_WORK_FOR] = MAX_SUB_FRAME;\
    if (ulGrantPhichTableInfo_p[workingSf].ulDlSpFrame != UL && \
            NA != ulGrantPhichTableInfo_p[workingSf].ulGrantPhichInfo.ulGrantSubFrameNum)\
    {\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_UL_SF_TO_WORK_FOR] = (workingSf + \
                ulGrantPhichTableInfo_p[workingSf].ulGrantPhichInfo.ulGrantSubFrameNum)%MAX_SUB_FRAME;\
    }\
}while(0)

/* TDD Config 0 changes start */
/* 2nd UL SF for which PUSCH/PUCCH/PRACH is to be allocated and UL Config prepared
 * For TDD Config 0,as per ipmplementaion in a TTI,
 * two new UL SF will be scheduled only in Special Subframe */
#define SET_ARG_2ND_UL_SF_FOR_CONFIG0(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex) \
do{\
    elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_2ND_UL_SF_FOR_CONFIG0_TO_WORK_FOR] = MAX_SUB_FRAME;\
    if (ulGrantPhichTableInfo_p[workingSf].ulDlSpFrame != UL )\
    {\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_2ND_UL_SF_FOR_CONFIG0_TO_WORK_FOR] = (workingSf + \
                SUB_FRAME_7) % MAX_SUB_FRAME;\
    }\
}while(0)
/* this macro stores the UL Subframe if 4th tick from  workingsf is a UL Subframe */
#define SET_ARG_4TH_TICK_UL_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex) \
do{\
    elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_4TH_TICK_UL_SF_TO_WORK_FOR] = MAX_SUB_FRAME;\
    UInt8 subFrameAfter4Tick = (workingSf + SUB_FRAME_4) % MAX_SUB_FRAME;\
    if ((ulGrantPhichTableInfo_p[subFrameAfter4Tick].ulDlSpFrame) == UL )\
    {\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_4TH_TICK_UL_SF_TO_WORK_FOR] = subFrameAfter4Tick; \
    }\
}while(0)
/* this macro stores the UL Subframe or Special subfrmae number
 *  if 4th tick from  workingsf is a UL Subframe or Special subfrmae number */
#define SET_ARG_4TH_TICK_UL_SPL_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex) \
do{\
    elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_4TH_TICK_UL_SPL_SF_TO_WORK_FOR] = MAX_SUB_FRAME;\
    UInt8 subFrameAfter4Tick = (workingSf + SUB_FRAME_4) % MAX_SUB_FRAME;\
    if ((ulGrantPhichTableInfo_p[subFrameAfter4Tick].ulDlSpFrame) != DL )\
    {\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_4TH_TICK_UL_SPL_SF_TO_WORK_FOR] = subFrameAfter4Tick; \
    }\
}while(0)
/* TDD Config 0 changes end */

/* SSF for which UL Config is to be prepared */
#define SET_ARG_SPL_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex) \
do{\
    if (workingSf < SUB_FRAME_5 && (SP == ulGrantPhichTableInfo_p[SUB_FRAME_6].ulDlSpFrame) )\
    {\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_SPL_SF_TO_WORK_FOR] = SUB_FRAME_6;\
    }\
    else\
    {\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_SPL_SF_TO_WORK_FOR] = SUB_FRAME_1;\
    }\
}while(0)

/*U subframe on which UE will be sending ACK/NACK */ 
#define SET_ARG_DL_HARQ_U_SF(sf, workingSf, ulGrantPhichTableInfo_p, dlAckNackSubFrameNumInfo_p,internalCellIndex) \
do{\
    elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_DL_HARQ_ACKNACK_U_SF] = MAX_SUB_FRAME;\
    if (dlAckNackSubFrameNumInfo_p[workingSf].dlAckNackSubFrameNumber)\
    {\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_DL_HARQ_ACKNACK_U_SF] = (workingSf + \
                dlAckNackSubFrameNumInfo_p[workingSf].dlAckNackSubFrameNumber)%MAX_SUB_FRAME;\
    }\
}while(0)

/*U subframe on which UE should have sent ACK/NACK */ 
#define SET_ARG_DL_HARQ_EXPIRY_U_SF(sf, workingSf, ulGrantPhichTableInfo_p,internalCellIndex)\
do{\
    /*FDD_HARQ_OFFSET */\
    if(workingSf >= 4) \
    {\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_DL_HARQ_EXPIRY_U_SF] = workingSf - 4;\
    }\
    else\
    {\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_DL_HARQ_EXPIRY_U_SF] = MAX_SUB_FRAME - (4 - workingSf );\
    }\
    if (ulGrantPhichTableInfo_p[elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_DL_HARQ_EXPIRY_U_SF]].ulDlSpFrame != UL)\
    {\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_DL_HARQ_EXPIRY_U_SF] = MAX_SUB_FRAME;\
    }\
}while(0)

/* The U subframe data whose CRC has not been received */
/* TDD Config 6 Changes Start */
/* this macro stores the UL Subframe number for which HARQ expiry took place due to
 * no ULSCH and PHICH needs to be send for that in workingsf */
#define SET_ARG_UL_HARQ_EXPIRY_U_SF(sf, workingSf, ulGrantPhichTableInfo_p, ulSfNumFrmPhichInfo_p,internalCellIndex)\
do{\
    elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_UL_HARQ_EXPIRY_U_SF] = MAX_SUB_FRAME;\
    if (UL != ulGrantPhichTableInfo_p[workingSf].ulDlSpFrame && \
            NA != ulSfNumFrmPhichInfo_p[workingSf])\
    {\
         elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_UL_HARQ_EXPIRY_U_SF] = ((workingSf - \
                 ulSfNumFrmPhichInfo_p[workingSf]) + MAX_SUB_FRAME)%MAX_SUB_FRAME;\
    }\
}while(0)
/* TDD Config 6 Changes End */
/* TDD Config 0 Changes Start */
/* In case of TDD config 0 The 2nd U subframe data whose CRC has not been received */
/* this macro stores the 2nd UL Subframe number for which HARQ expiry took place due to
 * no ULSCH and PHICH needs to be send for that in workingsf */
#define SET_ARG_UL_HARQ_EXPIRY_2ND_U_SF_FOR_CONFIG0(sf, workingSf, ulGrantPhichTableInfo_p, ulSfNumFrmPhichInfo_p,internalCellIndex)\
do{\
    elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_UL_HARQ_EXPIRY_2ND_U_SF] = MAX_SUB_FRAME;\
    if (DL == ulGrantPhichTableInfo_p[workingSf].ulDlSpFrame && \
            NA != ulSfNumFrmPhichInfo_p[workingSf])\
    {\
         elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_UL_HARQ_EXPIRY_2ND_U_SF] = ((workingSf - \
                 SUB_FRAME_6) + MAX_SUB_FRAME)%MAX_SUB_FRAME;\
    }\
}while(0)
/* TDD Config 0 Changes End */

/* The delay from UL_RA leg to closest DL subframe for which PDSCH will be executed*/
#define SET_ARG_PDSCH_DELAY(sf, workingSf, tddMacPdcchPdschFtnPtrIndexer_p,internalCellIndex)\
do{\
    elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_CLOSEST_PDSCH_DELAY] = 1;\
    UInt8 workingSfForPdcch = (workingSf +  elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_CLOSEST_PDSCH_DELAY])%MAX_SUB_FRAME;\
    while (PDCCH_UL_ONLY <= tddMacPdcchPdschFtnPtrIndexer_p[internalCellIndex][workingSfForPdcch])\
    {\
        ++workingSfForPdcch;\
        if(workingSfForPdcch >= MAX_SUB_FRAME)\
        {\
            workingSfForPdcch -= MAX_SUB_FRAME;\
        }\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_CLOSEST_PDSCH_DELAY]++;\
    }\
}while(0)

/** CA-TDD Changes End:06 **/
#ifdef DL_UL_SPLIT_TDD
#define SET_ARG_PERF_STATS(sf,internalCellIndex)\
do{\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_DL_PERF_STATS] = DL_OVERSHOOT_STATS;\
        elArgsTdd_g[internalCellIndex][sf].elArgArray[ELARG_UL_PERF_STATS] = UL_OVERSHOOT_STATS;\
}while(0)
#endif

void populateTddELSchedulerArray(CellConfigParams *cellConfig_p, InternalCellIndex internalCellIndex );

void initElArgs(UlGrantHIPduSubFrameInfo * ulGrantPhichTableInfo_p, 
        DLAckNackSubFrameNumInfo * dlAckNackSubFrameNumInfo_p,
        InternalCellIndex internalCellIndex,
/* TDD Config 6 Changes Start */
        UInt8 * ulSfNumFrmPhichInfo_p
/* TDD Config 6 Changes End */
        );
void initCountofFnSets(void);

#endif 
#endif  /* LTE_MAC_TDD_MGR_H */


