/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULCSIManager.h Exp $
 *
 ****************************************************************************
 *
 *  File Description : This header file contains information about the Uplink 
 *                     Channel Information Manager. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * 
 ****************************************************************************/

#ifndef LTE_MAC_UL_CSI_MANAGER_H
#define LTE_MAC_UL_CSI_MANAGER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacTBSize.h"
#include "lteMacSFNMgr.h"
#include "lteMacULUEContext.h"
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
#include "lteCircQueue.h"
#endif
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
 
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/*For three UL modulation schemes, namely, QPSK, 16QAM and 64QAM*/
#define MAC_MAX_NUM_MOD_SCHEME  3

/* Maximum Coding rate when expressed in percentage*/
#define MAC_MAX_CODE_RATE 100

/* ueBLER is expressed in percentage ranging from 0 to 100. 
 * Size of BLER correction table required is thus 100 + 1.
 */
#define MAC_BLER_CORRECTION_FACTOR_TABLE_SIZE 101
 
/* Used in calculation of effectiveMCS as
 * EffectiveMCS  = MCS+(MAC_TARGET_BLER-ueBler)*MAC_BLER_CORRECTION_PARAM_ALPHA/10
 */
/* SPR 3269 changes start */
/* SPR 7981 changes start */
#define MAC_BLER_CORRECTION_PARAM_ALPHA       6
#define UL_RESOLUTION_FACTOR 10
/* SPR 7981 changes end */
/* SPR 3269 changes end */

/* Bler changes start *
 * MAC_MAX_PUSCH_EFFECTIVE_SINR value maps to
 * highest 64QAM *
 */
#define MAC_MAX_PUSCH_EFFECTIVE_SINR 178
/* Bler changes end */

/* When UL ACK is received PUSCH BLER is reduced by this factor */
#define PUSCH_BLER_ACK_STEP_SIZE 1

/* When UL NACK is received PUSCH BLER is increased by this factor */
#define PUSCH_BLER_NACK_STEP_SIZE (9*PUSCH_BLER_ACK_STEP_SIZE)

/* Specifies the maximum number of SINR reports(either on ULSCH or on SRS) 
 * that can be processed by eNB MAC in one TTI.
 */
/*#define MAC_MAX_SINR_REPORTS_TO_PROCESS_PER_TTI (MAX_UL_UE_SCHEDULED<<1)*/
#define MAC_MAX_SINR_REPORTS_TO_PROCESS_PER_TTI 20

/* Specifies the maximum number of reports that can be present in SINR 
 * queue at any given time pending for processing.
 */
/*#define MAC_MAX_SINR_REPORTS_QUEUE_LENGTH  \
   ((MAC_MAX_SINR_REPORTS_TO_PROCESS_PER_TTI)*3)*/
#define MAC_MAX_SINR_REPORTS_QUEUE_LENGTH  ((MAX_UL_UE_SCHEDULED<<1)*3)

#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
#define MAC_MAX_SINR_REPORT_QUEUE_LENGTH_SPLIT MAX_UE_SUPPORTED
#endif
/* IIR Coefficient to calculate time moving average SINR. Value represents the 
 * weight given to last average SINR out of 1024. Value range is from 0 to 1024
 */
#define MAC_ALPHA_PUSCH_COEFFICIENT_IIR  820
/*SPR 3856 Fix End*/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef struct MCSGroupingTableT
{
    UInt8 previousMCS;
    UInt8 nextMCS;
    UInt8 mcsForGroup;
}MCSGroupingTable;

typedef enum SINRReportTypeT
{
  SINR_REPORT_TYPE_MIN,                        /*0*/
  SINR_REPORT_TYPE_SRS = SINR_REPORT_TYPE_MIN, /*0*/
  SINR_REPORT_TYPE_ULSCH,                      /*1*/
  SINR_REPORT_TYPE_ULSCH_NACK,                 /*2*/
  SINR_REPORT_TYPE_MAX=255
}SINRReportType;

typedef struct SINRReportNodeT
{
    LTE_SNODE   nodeAnchor;
    /*Specifies whether the reportType is SRS or ULSCH*/
    SINRReportType reportType;
    /*Specifies the UE index for which this SINR report is received*/
    UInt16 ueIndex;
    /*Specifies the starting PRB for which the report has been received*/
    UInt8 startPRB;
    /*Specifies the number of PRBs on which the report has been received. 
     *For SRS type reports, this will depend on configured SRS-Bandwidth. 
     *For ULSCH reports, it will correspond to the grant which was given 
     *for this ULSCH transmission
     */
    UInt8 numOfPRB;
    /* Time Averaging shanges start */
    /* SubFrameNumber on which report is received */
    /* SPR 15909 fix start */
    tickType_t recvTTI;
    /* SPR 15909 fix end */
    /* Time Averaging shanges end */
    /*Contains the SINR values reported by L1*/
    UInt8 sinrArray[MAC_MAX_RESOURCE_BLOCKS];
}SINRReportNode;

typedef enum MCSGroupingTypeT
{
  MAC_MCS_GROUP_BY_MOD_SCHEME, /*0*/
  MAC_MCS_GROUP_BY_GROUP_SIZE,       /*1*/
}MCSGroupingType;

#define MAC_MCS_GROUPING_STRATEGY MAC_MCS_GROUP_BY_GROUP_SIZE
#define MAC_MCS_GROUP_SIZE 1

/* SPR 3531 START */
/* Min SINR required so that TPC can be calculated in casoe of ULSCH NACK */
#define MIN_SINR_FOR_TPC_CALC_IN_NACK 50
/* SPR 3531 END */

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt8 sinrToCodeRate_g[MAC_MAX_SINR+1];

extern UInt8 modSchemeCodeRateToCQI_g[MAC_MAX_NUM_MOD_SCHEME][MAC_MAX_CODE_RATE+1];

extern MCSGroupingTable mcsGroupingTable_g[MAX_MCS_INDEX];
/* +- SPR 22382 Changes */
/*CA Changes start  */
extern SINR_REPORT_TYPE* sinrReportQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */

/* Time Averaging chages start */
extern UInt8 mcsSINRArr_g[MAX_MCS_INDEX_ARRAY];
/* Time Averaging chages end */
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
/*CA Changes start  */
/* SPR 15909 fix start */
void processSINRReports(tickType_t currentTTI, InternalCellIndex cellIndex);
#else
void processSINRReports(tickType_t currentTTI, SInt32 sinrReportQueueCount, InternalCellIndex cellIndex);
/* SPR 15909 fix end */
#endif

void initULCSIManager(UInt8 numCellsConfigured);
/*CA Changes end  */

void printMCSMapping(UInt8 highestReportedMCS, UInt8 lowestReportedMCS, MCSToPrbMapInfo *mcsToPrbMapInfo);

/* +- SPR 22382 Changes */

void updatePrbToSINRGroupIndexMap (UInt32 groupSize,UInt8 index,
                                   /*CA Changes start  */
                                   InternalCellIndex internalCellIndex
                                   /*CA Changes end  */
                                  );
UInt8 getCQIFromSINR(UInt8 sinr);
/* SPR 5599 changes start (ZIP ID 129059) */
/*Coverity 73519 Fix Start*/
UInt8 calculateAverageSINR (UInt32 deltaAverage, \
/* SPR 5599 changes end (ZIP ID 129059) */
                                          UInt8 lastAvgSINR);
/*Coverity 73519 Fix End*/
/*coverity-5.3.0 CID-54613*/
extern void cleanupULCSIManager(InternalCellIndex internalCellIndex);
/*coverity-5.3.0 CID-54613*/
#endif /*LTE_MAC_UL_CSI_MANAGER_H*/
