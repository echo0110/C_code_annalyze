
/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: lteMacRRMIntfInfo.h 
 *
 *************************************************************************
 *
 * File Description : This file contains the declaration of structure and 
 * 		      functions that are used for only Mac Rrm interface
 *************************************************************************/
/**************************************************************************
 * Project Includes
 *************************************************************************/

#ifndef LTE_MAC_RRM_INTF_INFO
#define LTE_MAC_RRM_INTF_INFO 

#include "lteMacDLPowerControlData.h"
#include "lteL2RRMInterface.h"
#include "lteCommonQueueDefs.h"
/* For Dissectors */
#include <lteMacTypes.h>
/**************************************************************************
 * Exported Definitions
 *************************************************************************/
#define MAX_CE_REGION 1

/*UL_MU_MIMO_CHG_START*/
// UL_MU_MIMO_ Review Comment Fix
/* #define MAX_UL_MU_MIMO_TYPE_VALUE 1 */
/*UL_MU_MIMO_CHG_END*/

/****************************************************************************
 * Exported Types
 ***************************************************************************/
typedef UELocation UELocationNode;

typedef UEUserType UEUserTypeNode; /* EICIC +-*/

/* This structure contains info of Cell edge and cell center 
 * resources for both UL and DL **/
typedef struct ResourceInfoT
{
    UInt32 startRBIndex;
    UInt32 endRBIndex;
}ResourceInfo;  


/* CA changes start*/
/*grouping all cell scpecific data structure for RRM MAC interface*/
typedef struct RRMMACCellGlobalT
{    
    DL_UE_RECONFIG_DATA_TYPE dlUEReconfigDataQueue;
    UL_UE_RECONFIG_DATA_TYPE ulUEReconfigDataQueue;
    UE_RECONFIG_USER_TYPE  	ueReconfigUserTypeQueue;  /* EICIC +-*/
    /*This queue contains those UEs whose MUMIMO Reconfiguration 
        request is sent from RRM*/
    LTE_CIRCQUEUE ulUEReconfigMUMIMOQueue;
    LTE_CIRCQUEUE ueReconfigNumOfLayerQueue;
    LTE_CIRCQUEUE ueReconfigBeamFormingAlgoQueue;
    /*This queue contains those UEs whose CA config Reconfiguration 
        request is sent from RRM*/
    LTE_CIRCQUEUE ueReconfigScellSchedulingQueue;
    /*This queue contains those UEs whose Cqi Validity Reconfiguration 
        request is sent from RRM*/
    LTE_CIRCQUEUE ueReconfigCqiValidityTimerQueue;

    /* This field is set when DL Resource Partition Reconfiguration 
        is received */
    UInt32 dlResourceReconfigFlag;

    /* This field is set when UL Resource Partition Reconfiguration 
        is received */
    UInt32 ulResourceReconfigFlag;

    /* This field is set when DL Scheduler Reconfiguration is received */
    UInt32 dlSchedulerReconfigFlag;

    /* This field is set when UL Scheduler Reconfiguration is received */
    UInt32 ulSchedulerReconfigFlag;

    UInt32 configureAbsPatternFlag;

     UInt32 configureUsableAbsPatternFlag;

    /* count maintained for total CE user in system */
    UInt32 totalCellEdgeUserCount;

    UInt32 ulPowerMaskValUpdated;

    ULMUMIMORrmConfig ulMUMIMORrmConfigInfo;
    UInt8 cceCorrectionStepUpFactor[MAX_AGGREGATION_LEVELS];
    UInt8 cceCorrectionStepDownFactor[MAX_AGGREGATION_LEVELS];
    UInt8 cceAdjustFactor[MAX_AGGREGATION_LEVELS];
}RRMMACCellGlobal;
/*CA changes end */

/****************************************************************************
 *  Exported Functions
 ****************************************************************************/
void processRRMMsg (UInt8* buffer, SInt16 bytesRead);
void prepareMACRRMApiHeader(L2RrmMsg *l2RrmMsg_p, UInt16 msgId, 
                                        UInt16 transactionId, 
                                        UInt16 length, 
                                        InternalCellIndex internalCellIndex);
UInt32 initResourceInfo(RrmMacCellConfigReq *rrmMacCellConfigReq_p,
                                        InternalCellIndex  internalCellIndex);
/*Complexity Start*/
UInt32 eicicInitReportInfo(
	InternalCellIndex internalCellIndex,
	UInt8 absPatternIndexTemp,
	UInt8 usableAbsPatternIndexTemp,
	RrmMacCellConfigReq *rrmMacCellConfigReq_p);
/*Complexity End*/

/*UL_MU_MIMO_CHG Start*/
/*+COVERITY 5.3.0 - 32278*/
UInt32 reconfigUELocationType( RrmMacUeReconfigReq *rrmMacUeReconfigReq_p,
                                        UInt8 *buffer_p,
                                        InternalCellIndex internalCellIndex);
UInt32 macReconfigSchedulerParams( 
                    RrmMacReconfigSchedulerReq *rrmMacReconfigSchedulerReq_p,
                    InternalCellIndex  internalCellIndex);
UInt32 reconfigUEUlMUMIMO(RrmMacUeReconfigReq *rrmMacUeReconfigReq_p,
                                        UInt8 *buffer_p,
                                        InternalCellIndex  internalCellIndex);
UInt32 reconfigBeamFormingAlgo(RrmMacUeReconfigReq *rrmMacUeReconfigReq_p,
                                        UInt8 *buffer_p,
                                        InternalCellIndex  internalCellIndex);
/* + TM7_8 Changes Start*/ 
UInt32 reconfigNumOfLayer(RrmMacUeReconfigReq *rrmMacUeReconfigReq_p,
                                        UInt8 *buffer_p,
                                        InternalCellIndex  internalCellIndex);
/* - TM7_8 Changes End*/ 
/*-COVERITY 5.3.0 - 32278*/
/*UL_MU_MIMO_CHG End*/

void initMacRRMInterface(InternalCellIndex  internalCellIndex);

#ifdef PERF_STATS
UInt8 processRrmMacConfigPerfStatsReq(
                          RrmMacConfigPerfStatsReq *rrmMacConfigPerfStatsReq_p,
                          UInt16 transactionId,
                          InternalCellIndex internalCellIndex);
/* + PERF_CA */
/* + SPR 17439 */
void macProcessPerfCounterforRRM(void);
/* - SPR 17439 */
/* - PERF_CA */

#endif
/* CA changes start*/
UInt32 macReconfigUECAConfigInfoList(
                                    RrmMacUeReconfigReq *rrmMacUeReconfigReq_p,
                                    UInt8 *buffer_p,
                                    InternalCellIndex internalCellIndex);

/* EICIC +*/
UInt32 macReconfigUEControlChannelInterferenceList(
                        RrmMacUeReconfigReq *rrmMacUeReconfigReq_p, 
                        UInt8 *buffer_p,
                        InternalCellIndex internalCellIndex);
/* EICIC -*/
UInt32 macReconfigUECqiValidityTimerList(
                                    RrmMacUeReconfigReq *rrmMacUeReconfigReq_p,
                                    UInt8 *buffer_p,
                                    InternalCellIndex internalCellIndex);
UInt8 macValidateScellSchedulingActDeact(
                        UEScellSchedulingInfo *ueScellSchedulingInfo_p,
                        UECarrierAggConfigError *ueCarrierAggConfigError_p,
                        UInt32 rrmUeIndex,
                        InternalCellIndex internalCellIndex);
/* CA changes end */
#endif
