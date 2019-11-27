/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacOAMIntf.h,v 1.2.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacOAMIntf.h,v $
 * Revision 1.1.1  2012/07/20 14:56:27  gur31292
 * Changes related to DCI Format3A
 *
 * Revision 1.2.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.7  2009/12/17 07:15:27  gur11912
 * removed TICK_IN_MS
 *
 * Revision 1.6  2009/08/28 13:27:53  gur19413
 * added new macro for logger
 *
 * Revision 1.5  2009/07/14 13:02:49  gur11083
 * incorporated review comments
 *
 * Revision 1.4  2009/06/24 10:15:08  gur20470
 * included the header files lteMacParseOAMMsg.h
 *
 * Revision 1.3  2009/06/07 09:07:19  gur19413
 * added new macro
 *
 * Revision 1.2  2009/06/07 08:47:29  gur20470
 * updated for mac 1.2
 *
 * Revision 1.1  2009/05/05 11:16:38  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_OAM_INTF_H
#define LTE_MAC_OAM_INTF_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacOAMInterface.h"
#include "lteMacULUEContext.h"
#include "lteMacDLUEContext.h"
#include "lteMacParseUtil.h"
#include "lteMacRandomAccess.h"
#include "lteMacStatsManager.h"
#include "lteCommonStatsManager.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacDRXMgr.h" /* DRX_CHG */
#include "lteMacMeasGapHandler.h" /* MEAS_GAP_CHG */
#include "lteMacDLQosTokenAlgo.h" /*QOS_CHG */
/*CLPC_CHG*/
#include "lteMacULPowerControlData.h"
#include "lteMacDLPowerControlData.h"
#include "lteMacULSchedulerStrategy.h"
/*CLPC_CHG*/
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* Start AM and UM LC parameters changes */
#define MIN_TOTAL_LC_PER_UE_FOR_MEM 1
#define MAX_TOTAL_LC_PER_UE_FOR_MEM 8
#define MIN_AM_LC_PER_UE_FOR_MEM 0
#define MAX_AM_LC_PER_UE_FOR_MEM 8
#define MIN_UM_LC_PER_UE_FOR_MEM 0
#define MAX_UM_LC_PER_UE_FOR_MEM 8
/* End AM and UM LC parameters changes */

#define PERF_STATS_CONTAINER_OAM 0

/* SPR 3090 Chg */
#define MEGA_BITS_PER_SEC (1048576)
/* SPR 3090 Chg */
#define OAM_INTF_MODULE  "OAM INTF MODULE"

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* CA Changes start */

/* Error Codes */
#define MAC_LOG_CH_ERROR        30

/* SPR 22539 Fix + */
typedef enum enableAllowAllocBitrateT
{
    ENABLE_ALLOW_ALLOC_BITRATE_FALSE = 0,
    ENABLE_ALLOW_ALLOC_BITRATE_QLOAD_BASED,
    ENABLE_ALLOW_ALLOC_BITRATE_TOKEN_BASED
}EnableAllowAllocBitrate;
/* SPR 22539 Fix - */
typedef struct InitParamsT
{
    UInt8  isLastAttempt;
    /* Maximum value of MCS when 64 QAM is disabled */
    UInt8 maxMcs16QAM;
    UInt8 commChWithMIBAllowed;
    UInt8 dlSchWithMIBAllowed;
    UInt8 enableAllocAfterAllowedBitrate;
    UInt8 dlRBRestrictionFactor;
    UInt8 ulRBRestrictionFactor;
    UInt16 maxNumOfUeSupported;
    UInt16 macMaxNumOfUeSupported;
    /*Range configured for CRNTI, lower boundary & upper boundary*/
    UInt16   crntiRange;
    UInt16   spsCrntiRange;
    UInt16   lowerRNTIBound;
    UInt16   upperRNTIBound;
    ULSchedulerStrategyConfig ulSchdConfig;
    DLSchedulerStrategyConfig dlSchdConfig;
    /* Global variable for SPS System Wide Parameter*/
    SpsSysWideParam     spsSysWideParam;
    /* Global variable for SPS Common Interval Profile */
    SpsCommIntervalProfile spsCommIntervalProfile[SPS_INTERVAL_LIST];


}InitParams;

#ifdef KPI_STATS
#ifdef TDD_CONFIG
typedef struct MACGetKpiStatReqT
{
    LTE_LIST_NODE           anchorNode;
    MacGetKpiStatsReqParams macGetKpiStatsReqParams;
    UInt16                  transactionId;
    UInt8                   modCounter;
}MACGetKpiStatReq;
#endif
#endif

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt16 getMACBuildConfiguration (UInt8 *buff_p, UInt16 *msgLen_p);

void getMACUEStats (void);
    
void getMACUELogicalChannelStats (UInt16 crnti);

void enableLog( UInt8 flag);

void enableMACDLSchedulerStats( UInt8 flag);

void enableMACULSchedulerStats( UInt8 flag);

void displayMemPoolStats(void);

UDouble32 getDownlinkThroughPut(InternalCellIndex internalCellIndex);

UDouble32 getUplinkThroughPut(InternalCellIndex internalCellIndex);

UDouble32 getUlSpectralEfficiency(InternalCellIndex internalCellIndex);

UDouble32 getDlSpectralEfficiency(InternalCellIndex internalCellIndex);
/* CA Changes start */
void enableDownlinkThroughput(UInt32 enable, InternalCellIndex internalCellIndex);

void enableUplinkThroughput(UInt32 enable, InternalCellIndex internalCellIndex);

/* CA Changes end */
/* SPR 2127 changes start */
void calculateDownlinkThroughPut(InternalCellIndex internalCellIndex);
void calculateUplinkThroughPut(InternalCellIndex internalCellIndex);
/* SPR 2127 changes end */

#ifdef KPI_STATS
void processKPI(UInt8 processingAllowed);
/* + stats_review_comments_ipc_new */
/*SPR 19296 fix start*/
	/*code deleted*/
/*SPR 19296 fix end*/
/* - stats_review_comments_ipc_new */
#endif
 
#ifdef PERF_STATS
void updatePerfStatsForAllModules(void);
#endif

/* This function send the MAC_PHY_OAM_ERROR_IND message to OAM in case of any
 * subframe error is received from L1. */
void sendMacPhyOamErrIndication( MacPhyOamSubframeErrorType errorType, InternalCellIndex internalCellIndex );

/* This function constructs and send MAC_INIT_LAYER_IND message to OAM. */
void sendMacInitIndication( InternalCellIndex internalCellIndex,
   UInt8 *IndBuff_p, UInt16 *currentLen );
#endif  /* LTE_MAC_OAM_INTERFACE_H */
