/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLPowerControlData.h
 *
 ******************************************************************************
 *
 *  File Description : This files contains the tunable parameters for Downlink
 *                     Power Control.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * Initial Version
 * 29-01-2012         Closed Loop Power Control Feature Development
 *
 *****************************************************************************/
#ifndef _LTE_MAC_DL_POWER_CONTROL_DATA_H_
#define _LTE_MAC_DL_POWER_CONTROL_DATA_H_
/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacDLUEContext.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacULPowerControlData.h"
/******************************************************************************
 Private Definitions
 *****************************************************************************/
#define MAX_CQI_VAL                              15
#define NUM_DCI_SIZE_CATEGORY                     3
#define MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT  4
/* SPR 3267 START */
#define MAX_TX_POWER_OFFSET             10000
/* SPR 3267 END */

#define MAX_DL_SCH_PARAM_INDEX 2
/******************************************************************************
 Private Types
 *****************************************************************************/
typedef struct PdcchAggregationAndPowerOffsetT
{
    /* aggregationPowerOffset two dimensional array specifies the aggregation 
     * level and power offset tuples based on DCI category size(either of 
     * categories SMALL_DCI_STEP_SIZE, MEDIUM_DCI_STEP_SIZE and 
     * BIG_DCI_STEP_SIZE) and wideband CQI(Range 1 to 15).
     */
    AggregationPowerOffset \
        aggregationPowerOffset[NUM_DCI_SIZE_CATEGORY][MAX_CQI_VAL];

}PdcchAggregationAndPowerOffset;

/* this structure is used to store Scheduler Reconfig Params */
typedef struct MacReconfigSchedulerParamsDLT
{
    PdcchAggregationAndPowerOffset pdcchAggregationAndPowerOffset[MAX_USER_LOCATION];

    /******************************************************************************
     *                        PHICH POWER CONTROL                                 *
     ******************************************************************************/
    /* Downlink Power control of PHICH is based on the wideband channel
     * quality feedback reported from the UE.
     * The reference table below  maps CQI feedback to PHICH delta transmission
     * power (delta power by which  PHICH power should be incremented/reduced)
     * on top of PHICH power offset configured from higher layers.
     */
    CqiToPHICHPower cqiToPHICHPower;
   
    /*This table specifies the PUCCH target SINR and the mapping of delta
     ** SINR to TPC for PUCCH*/
    DeltaSINRToTPCMapForPUCCH deltaSinrToTPCMapForPUCCH;
    /*This table specifies the set of aggregation levels that need to be
     ** tried for CCE allocation based on DCI size category and wideband
     ** CQI*/
   
    /* + TM7_8 Changes Start */
    UInt8 bfVectorPresentFlag;
    /* - TM7_8 Changes End */
    /*Wait time for sending triggered TPC in DCI1x before DCI 3/3A is
     ** scheduled*/
    UInt8 tpcTriggerWaitTimeForPUCCHDCI1x2x;
    /*Time in 1 ms ticks that must elapse before another TPC for PUCCH
     ** power control can be sent to the UE due BLER conditions*/
    UInt16 coolOffPeriodForBLERTriggeredPUCCHTPC;
    /*Time in 1 ms ticks that must elapse before another TPC for PUCCH
     ** power control can be sent to the UE either due to BLER or SINR
     ** considerations*/
    UInt16 coolOffPeriodForPUCCHTPC;
    /* Map for maintaining the Delta BLER to TPC values. This array will be
     * populated through OAM interface in MAC usable format.
     */

    SInt8 deltaHarqBlerToPucchTpcMap[NUM_OF_BLER_TPC_VALUES];
    /* Map for maintaining the Delta SINR to TPC values. This array will be
     * populated through OAM interface in MAC usable format.
     */
    /* ICIC changes start */
    SInt8 deltaSinrToPucchTpcMap[NUM_OF_SINR_TPC_VALUES];
    
    /*When DCI 3A is triggered for a UE but not triggered for UEs on
     ** the same TPC-PUSCH-RNTI/TPC-PUCCH-RNTI, MAC waits for this much
     ** time before triggering explicit DCI0/1/2x*/
    
    UInt32 tpcNumUeThresholdForPUCCHDCI3A;

    DeltaBLERToTPCMapForPUCCH deltaBlerToTPCMapForPUCCH;

    /* SPS_CHG */
    /* Maximum number of SPS activation, reactivation or deactivation which
     * can be scheduled in a TTI for downlink */
    SpsDlSchedulingInfoPerTti spsDlSchInfoPerTti;
                     
    /* +DYNAMIC_ICIC */
    /* power offset from reference signal power QPSK modulation scheme */
    UInt32 paQpsk;
    /* -DYNAMIC_ICIC */

    /* + TM7_8 Changes Start */
    UInt16 angleOfArrivalToBeamFormingVectorMapping[MAX_NUM_BF_VECTORS_TO_AOA_VAL];
    sinrToBeamFormingConfig sinrToBeamFormingVectorMapping[MAX_NUM_BF_VECTORS_TO_SINR_VAL];
    /* - TM7_8 Changes End */
    /*+ Dyn UEs To Be SchPerTTI:CR +*/
    MaxUEsToBeScheduledDlUl maxUEsToBeScheduledDlUl; 
    /*- Dyn UEs To Be SchPerTTI:CR -*/
    /* CA changes start */
    CAParams caParams;
    /* CA changes end */
#ifdef LTE_EMBMS_SUPPORTED
    UInt8 eMbmsReservedCellPowerOffset;
#endif

}MacReconfigSchedulerParamsDL;


/******************************************************************************
 * Private Function Prototypes
******************************************************************************/
UInt8 getDCIBitSize(UInt8 dciFormat, 
                    UInt8 numOfTxAntennas, 
                    UInt8 ulAvailableRBs);

UInt8 getDCISizeCategory(UInt8 dciSize);

AggregationPowerOffset* getAggregationAndPowerOffset(
                            UInt8 widebandCqi, 
                            UInt8 dciSizeCategory);

/* SPR 5599 changes start (ZIP ID 129059) */
UInt16 getPHICHPowerOffset(UInt8 widebandCqi, InternalCellIndex internalCellIndex); 
/* SPR 5599 changes end (ZIP ID 129059) */
/* ICIC changes start */
void initDLSchedulerParams(InternalCellIndex internalCellIndex);
/* ICIC changes end */
/******************************************************************************
 * Private Constants
 *****************************************************************************/
/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/* Following structure stores scheduler configuration for DL */
/* CA Changes start */
extern MacReconfigSchedulerParamsDL* macReconfigSchedulerParamsDL_gp[MAX_NUM_CELL];
/* CA Changes end */
extern UInt8 schParamsIndexDL_g[MAX_NUM_CELL];


/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* SPR 3100 START */
/* SPR 5599 changes start (ZIP ID 129059) */
/* +- SPR 17777 */
UInt8 getDLSinrValueFromCqi(UInt8 cqiIndex);
/* +- SPR 17777 */
/* SPR 5599 changes end (ZIP ID 129059) */
/* SPR 3100 END */

#endif /*_LTE_MAC_DL_POWER_CONTROL_DATA_H_*/
