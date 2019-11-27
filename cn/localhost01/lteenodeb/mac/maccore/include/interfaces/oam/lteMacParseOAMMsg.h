/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMacParseOAMMsg.h 
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for the LTE MAC Layer
 *                     Interface with OAM Layer.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
  $Log: lteMacParseOAMMsg.h,v $
  Revision 1.1.1  2012/07/20 14:56:27  gur31292
  Changes related to DCI Format3A
 
  Revision 1.1.1.1.6.2.6.1  2010/10/25 09:34:12  gur23971
  warning removed

  Revision 1.1.1.1.6.2  2010/08/02 08:13:42  gur20491
  FrameworkSetupWith1.0_MAC1.2.4

  Revision 1.20.28.1  2010/07/21 08:51:17  gur24420
  Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
  Overall Changes Cover:-
  1. Module ID aligned as agreed with MAC and RRC teams.

  Revision 1.20  2009/11/11 07:36:23  gur19413
  changed value for numHarqProcess MAC_UE_STATUS_LENGTH 44 to 45

  Revision 1.19  2009/09/01 09:39:57  gur19413
  removed Warning related to header len macro

  Revision 1.18  2009/08/24 12:53:38  gur11912
  stoppedProcessing_g declared as volatile

  Revision 1.17  2009/08/24 05:44:24  gur19413
  added function prototype parseOAMMsg

  Revision 1.16  2009/08/03 08:19:02  gur20052
  Added hash def for second param for UT_LOGS

  Revision 1.15  2009/07/29 08:13:41  gur11083
  change MAX_DATA_SIZE to MAX_ARRAY_DATA_SIZE
   
 *
 ****************************************************************************/

#ifndef LTEMAC_PARSE_UTIL_H
#define LTEMAC_PARSE_UTIL_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteMacTypes.h"
#include "lteMemPool.h"
#include "lteMacULUEContext.h"
#include "lteMacDLUEContext.h"
#include "lteMacEncoder.h"
#include "lteMacMultiplexer.h"
#include "lteSQueue.h"
#include "lteMacReceiver.h"
#include "lteMacPhyInterface.h"
/*OPT_SPR_4119_4120_4122_4123_CHANGES_START*/
#include "lteMacTransmitter.h"
/*OPT_SPR_4119_4120_4122_4123_CHANGES_END*/
#include "lteMacTypes.h"
#include "lteShmLogger.h"
#include "lteMacParseUtil.h"
#include "lteMacBCCH.h"
#include "lteMacPCCH.h"
#include "lteMacCCCH.h"
#include "lteMacRandomAccess.h"
#include "lteMacDemux.h"
#include "lteMacULQueueLoad.h"
#include "lteMacExecutionLegs.h"
#include "lteMacUEConfMgr.h"
#include "lteMacPUCCH.h"
 
 
//#include "lteMacPHICH.h"
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

#define PARSE_OAM_MSG "parse_oam_msg"
#define FREQUENCY_SELECTIVE_OFF       0
#define FREQUENCY_SELECTIVE_ON        1

/* COVERITY 10266 */
/* SPR 4951 changes */
#define MAX_OAM_RESP_SIZE      11000 
/* SPR 4951 changes */
/* COVERITY 10266 */
/* COVERITY 24795 */
/* SPR 8737 Fix start */
#define MAX_ARRAY_DATA_SIZE   16392 
/* SPR 8737 Fix end */
/* COVERITY 24795 */
#define MSGID_SIZE             1
#define IDX_SIZE               2
#define UEID_SIZE              2
#define RSP_SIZE               1
#define PWR_HR_SIZE            2

/* Tags */
#define GET_PER_UE_STATUS       1
#define MAC_UE_STATUS           2
#define MAC_DL_LOG_CH_STATUS    3
#define MAC_UL_LOG_CH_STATUS    4
#define MAC_DL_THROUGHPUT       5
#define MAC_UL_THROUGHPUT       6

#define TAG_SIZE 2
#define LENGTH_SIZE 2
#define UL_LC_RESP_LENGTH 4
#define DL_LC_RESP_LENGTH 4
#define CRNTI_TAG_LENGTH 6
#define MIN_LC_CREATE_LENGTH 9
/* + CL-MIMO LJA*/
/*Length increased due to cqiThreshold parameter*/
#define MAC_INIT_REQ_MANDATORY_LENGTH 33
/* - CL-MIMO LJA*/
#define MAC_DL_THROUGHPUT_LENGTH 4
#define MAC_UL_THROUGHPUT_LENGTH 4
#define MAC_DL_LOG_CH_STATUS_LENGTH 10
#define MAC_UL_LOG_CH_STATUS_LENGTH 10
#define GET_PER_UE_STATUS_LENGTH 6
#define MAC_UE_STATUS_LENGTH  45

/*spr 19296 fix start*/
#define LTE_DRB_LC_ID_LOW_BOUND     3
#define LTE_DRB_LC_ID_HIGH_BOUND    10
/*spr 19296 fix end*/
/* valid ranges */

/* MAC Downlink Scheduler Weights */
#define MAC_SCH_MIN_WT                      0
#define MAC_SCH_MAX_DWLK_UE_QL_WT           20
#define MAC_SCH_DEFAULT_DWLK_UE_QL_WT       10

#define MAC_SCH_MAX_DWLK_MS_WT              20
#define MAC_SCH_DEFAULT_DWLK_MS_WT          5

#define MAC_SCH_MAX_DWLK_LC_QL_WT           20
#define MAC_SCH_DEFAULT_DWLK_LC_QL_WT       10

#define MAC_SCH_MAX_DWLK_UE_PR_WT           20
#define MAC_SCH_DEFAULT_DWLK_UE_PR_WT       10

#define MAC_SCH_MAX_DWLK_LC_PR_WT           20
#define MAC_SCH_DEFAULT_DWLK_LC_PR_WT       10

#define MAC_SCH_MAX_DWLK_UE_DELAY_WT        20
#define MAC_SCH_DEFAULT_DWLK_UE_DELAY_WT    5

#define MAC_SCH_MAX_DWLK_LC_DELAY_WT        20
#define MAC_SCH_DEFAULT_DWLK_LC_DELAY_WT    5

/* SPR 11786 changes start */
#define MAC_SCH_MIN_UPLK_UE_PR_WT           0
/* SPR 11786 changes end */
#define MAC_SCH_MAX_UPLK_UE_PR_WT           20
#define MAC_SCH_DEFAULT_UPLK_UE_PR_WT       10

/* SPR 11786 changes start */
#define MAC_SCH_MIN_UPLK_UE_DELAY_WT        0
/* SPR 11786 changes end */
#define MAC_SCH_MAX_UPLK_UE_DELAY_WT        20
#define MAC_SCH_DEFAULT_UPLK_UE_DELAY_WT    10
/* SPR 11786 changes start */
#define MAC_SCH_MIN_UPLK_MS_WT              0
/* SPR 11786 changes end */
#define MAC_SCH_MAX_UPLK_MS_WT              20
#define MAC_SCH_DEFAULT_UPLK_MS_WT          10

#define MAC_SCH_DEFAULT_UPLK_LCG_PR_WT      1
/* SPR 11786 changes start */
#define MAC_SCH_MIN_UPLK_LCG_PR_WT          0
#define MAC_SCH_MAX_UPLK_LCG_PR_WT          20
/* SPR 11786 changes end */

#define MAC_SCH_DEFAULT_DWLK_CQI_WT         10
#define MAC_SCH_DEFAULT_LOG_LEVEL           2

/*MCS Index for QPSK*/
#define MAC_MIN_MCS_INDEX_QPSK              0
#define MAC_MAX_MCS_INDEX_QPSK              9
#define MAC_DEFAULT_MCS_INDEX_QPSK          0

/*MCS Index for SIXTEENQAM*/
#define MAC_MIN_MCS_INDEX_SIXTEENQAM        10
#define MAC_MAX_MCS_INDEX_SIXTEENQAM        16
#define MAC_DEFAULT_MCS_INDEX_SIXTEENQAM    10

/*MCS Index for SIXFOURQAM*/
#define MAC_MIN_MCS_INDEX_SIXFOURQAM        17
#define MAC_MAX_MCS_INDEX_SIXFOURQAM        28
#define MAC_DEFAULT_MCS_INDEX_SIXFOURQAM    17

#define MAC_DEFAULT_NUM_RESOURCE_BLOCKS     25
#define MAC_DEFAULT_MODULATION_SCHEME       0
#define MAC_DEFAULT_HARQ_SCHEME             2

/* added for MODULATION SCHEME WT validation added in MAC Rel 1.2*/
#define MAC_MAX_MODULATION_SCHEME_WT        10

/*UL Config Advance SubFrames*/
/* SPR 3132 CHANGES START*/
#define MAX_UL_CONFIG_ADVANCE_SUB_FRAMES    2
/* SPR 3132 CHANGES END*/
#define DEFAULT_UL_CONFIG_ADVANCE_SUB_FRAMES 1 

/* MAC KPI Timer Max/Min Value */
#define MAC_MAX_VAL_KPI_TIMER_T 60*60
#ifdef PERF_STATS
#define MAC_MAX_VAL_PERF_TIMER_T 60*60
#define MAC_MIN_VAL_PERF_TIMER_T 1
#endif

#ifdef UTFWK_SIMULATION
#define MAC_MIN_VAL_KPI_TIMER_T 1
#else
#define MAC_MIN_VAL_KPI_TIMER_T 60
#endif

/*CLPC_CHG*/
#define TPC_FOR_MSG3_BIT 0x0001
#define SRS_SUBBAND_GRP_SIZE_BIT 0x0002
#define TPC_TRIGGER_WAIT_TIME_PUSCH_DCI0_BIT 0x0004 
#define TPC_TRIGGER_WAIT_TIME_PUCCH_DCI1x2x_BIT 0x0008
/*DCI Format 3A change start*/
#define TPC_NUM_UE_THRESHOLD_FOR_PUSCH_DCI3A_BIT 0x0010  
#define TPC_NUM_UE_THRESHOLD_FOR_PUCCH_DCI3A_BIT 0x0020
/*DCI Format 3A change end*/
#define COOL_OFF_PRD_FOR_BLER_BIT 0x0040
#define COOL_OFF_PERIOD_FOR_PUCCH_BIT 0x0080
#define COOL_OFF_PERIOD_PUSCH_BIT 0x0100
#define ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_BIT 0x0200
#define DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_BIT 0x0400
#define DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_BIT 0x1000
#define PDCCH_AGG_AND_POW_OFF_BIT 0x2000
#define CQI_TO_PHICH_POW_BIT 0x4000
// SPR 3486  - START
#define MIN_RB_FOR_PL_PHR_CALC_BIT 0x8000
// SPR 3486  - END
/*CLPC_CHG*/
/* Rel 5.3: Coverity 24640 Fix Start */
#define NUM_OF_UPLNK 1
#define NUM_OF_DWNLNK 1
/* Rel 5.3: Coverity 24640 Fix End */
/*4x4 DL MIMO CHG START*/
#define CQI_MIN 1
#define CQI_MAX 15
#define CQI_THRESHOLD_DEFAULT 12


typedef enum MacKpiSupportedT
{
    KPI_TOTAL_PRB_USAGE_DL             = 0, // bit 0
    KPI_TOTAL_PRB_USAGE_UL             = 1, // bit 1
    KPI_TOTAL_PRB_USAGE_PER_QCI_DL     = 2, // bit 2
    KPI_TOTAL_PRB_USAGE_PER_QCI_UL     = 3, // bit 3
    KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_DL = 4, // bit 4
    KPI_NUMBER_OF_ACTIVE_UE_PER_QCI_UL = 5, // bit 5
    KPI_RCVD_RANDOM_ACCESS_PREAMBLES   = 6, // bit 6
    KPI_PKT_DELAY_PER_QCI_DL           = 7, // bit 7
    KPI_MAC_DL_LC_THROUGHPUT           = 8, // bit 8
    KPI_MAC_CQI_RECEIVED               = 9, // bit 9
    /* SPR 13492 Changes Start */
    KPI_IN_SESSION_ACTIVITY_TIME       = 10, // bit 10 
    /* SPR 13492 Changes End */
/*spr 19296 fix start*/
    KPI_TOTAL_DL_DRB_UU_LOSS_RATE = 11, //bit 11
/*spr 19296 fix end*/
    KPI_MAX_VALUE
}MacKpiSupported;
/* SPR 13492 Changes Start */

/*spr 19296 fix start*/
#define CELL_SPECIFIC_MAC_COUNTERS_BITMAP  0xEFF
/*spr 19296 fix end*/

/* SPR 13492 Changes End */
/* 100MS_SUPPORT : end */
/* following macro will return 1 if corresponding bit is set else it will return 0*/
#define IS_KPI_MAC_BITSET(bitmap, bitplace)    (bitmap & (1<<bitplace))

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* SPR 1571 changes end */
/* Squeue which maintains free temporay crntis */
/* CA Changes start */
extern UL_GRANT_APERIODIC_CQI_TYPE* ulGrantAperiodicCqiQueue_gp[MAX_NUM_CELL];
extern void macLayerCleanup(void);
extern StatsInfo statsInfo_g[MAX_NUM_CELL];
extern UInt16 fillUeStatus(UInt8 *cnfBuff_p, UInt16 *current_p, 
            UInt16 *ueIndex_p, InternalCellIndex internalCellIndex);

/* SPR 609 changes start */
MacRetType fapiSendPhyParamReqAndWaitForResp( void );
/* SPR 609 changes end */  
/* CA Changes end */

typedef struct InitReqParamsT {
    UInt8 dlUeQueueLoadWt;
    UInt8 dlLcQueueLoadWt;
    UInt8 dlMsWt;
    UInt8 dlMs64QAMFactorWt;
    UInt8 dlMs16QAMFactorWt;
    UInt8 dlMsQPSKFactorWt;
    UInt8 dlUePriorityWt;
    UInt8 dlLcPriorityWt;
    UInt8 dlUeDelayWt;
    UInt8 dlLcDelayWt;
    UInt8 ulMsWt;
    UInt8 ulMs64QAMFactorWt;
    UInt8 ulMs16QAMFactorWt;
    UInt8 ulMsQPSKFactorWt;
    UInt8 ulUePriorityWt;
    UInt8 ulUeDelayWt;
    UInt8 ulUeBsrLoadWt;
    UInt8 ulLcgPriorityWt;
    UInt16 logLevel;
    UInt8 dlSchedulerStrategy;
    UInt8 ulSchedulerStrategy;
    UInt8 maxUEsToBeScheduledDL;
    UInt8 maxUEsToBeScheduledUL;
/* + CL-MIMO LJA*/
    UInt8 cqiThreshold;
/* - CL-MIMO LJA*/
} InitReqParams;

/* Socket FD for receiving messages */
extern SInt32 txOAMSockFD_g;
extern SockAddr txOAM_addr_g;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* UL-SUBFRAME Code Changes Start */
UInt8 parseOAMMsg( UInt8 *msgBuf_p, 
                   UInt16 msgId, 
                   /* +- SPR 17777 */
                   UInt16 transactionId,
                   InternalCellIndex internalCellIndex
                  );
/* UL-SUBFRAME Code Changes End */
                     
/* SPR 609 changes start */   
extern void initSrsUESubFrameMaps(UInt8 numOfCells);
/* SPR 609 changes end */    

/* Function Prototype*/
/* + SPR 17439 */
void macProcessPerfCounter(void);
/* - SPR 17439 */
/* CA Changes start */
void updateMacPerfContainer (UInt32 intId);
void  handleMACGetOAMPerfStatsReq(UInt8 *msgBuf_p,UInt8 *cnfBuff, UInt16 transactionId,
        InternalCellIndex internalCellIndex);
/*CLPC_CHG*/
/* + SPR 17439 */
 UInt8 macCommonGetNumberOfInitReceived(void);
/* - SPR 17439 */
 void prepareMacOamApiHeader(
                UInt8 *msgBuf_p,
                UInt16 msgId,
                UInt16 transactionId,
                UInt16 msgLength,
                RrcCellIndex cellId);

void fillMACUeSinrTaReq
(
 UInt16 ueIndex,
 InternalCellIndex internalCellIndex,
 MacUeSinrTaResp *macUeSinrTaResp
);

UInt32 macSendOamFailResponse
(
    UInt16 msgId,
    UInt16 transactionId,
    RrcCellIndex cellIndex
);

/* CA Changes end */
/* Cyclomatic_complexity_changes_start */
UInt32 storePrimaryDCIFormat(MacInitReqParams *macInitReqParams_p);
/* Cyclomatic_complexity_changes_end */

/*spr 22474 start*/
MacRetType handleMACGetDebugInfoReq(
    UInt8 *msgBuf_p,
    UInt16 transactionId,
    InternalCellIndex internalCellIndex);

MacRetType macSendMemPoolStats
(
    UInt16 transactionId,
    InternalCellIndex internalCellIndex
);

MacRetType macSendMsgPoolStats
(
    UInt16 transactionId,
    InternalCellIndex internalCellIndex
);
/*spr 22474 end*/
#endif /* Included LTEMAC_INIT_MAC_H */
