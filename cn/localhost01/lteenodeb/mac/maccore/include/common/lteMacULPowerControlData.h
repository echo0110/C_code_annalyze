/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULPowerControlData.h
 *
 ******************************************************************************
 *
 *  File Description : This files contains the tunable parameters for Uplink
 *                     Power Control.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * Revision 1.1.1  2012/07/20 14:56:27  gur31292
 * Changes related to DCI Format3A
 *
 * Initial Version
 * 29-01-2012         Closed Loop Power Control Feature Development
 *
 *****************************************************************************/
#ifndef _LTE_MAC_UL_POWER_CONTROL_DATA_H_
#define _LTE_MAC_UL_POWER_CONTROL_DATA_H_
/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacULCSIManager.h"
#include "lteMacParseUtil.h"
/* ICIC changes start */
#include "lteMacRRMInterface.h"
/* ICIC changes end */
/******************************************************************************
 Private Definitions
 *****************************************************************************/
#define MAX_NUM_PATHLOSS_TO_SINR_COUNT   10
#define MIN_COUNT                         0
#define MIN_PATHLOSS                      0
/* SPR_4201 Start */
#define MAX_PATHLOSS                    200
#define PATHLOSS_TABLE_SIZE              201
/* SPR_4201 End */
#define MAX_NUM_TPC_FOR_PUCCH             4 
#define MAX_NUM_TPC_FOR_PUSCH             4
#define MAX_NUM_ALPHA_COUNT               8
#define MIN_PUCCH_BLER_TARGET             0
#define MAX_PUCCH_BLER_TARGET           100
#define MAX_PUCCH_SINR_TARGET           255
#define MIN_PUCCH_SINR_TARGET             0
#define NUM_OF_SINR_TPC_VALUES          255
#define NUM_OF_BLER_TPC_VALUES          101
// SPR 3486 - START
// Macro for minimum number of RBs that must be present 
// to calculate PathLoss from received PHR on PUSCH
/*CA Changes start  */
#define MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex)  \
    (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
    schParamsIndexUL_g[internalCellIndex])->minRbForPlPhrCalc 
/*CA Changes end  */
// Macro defining maximum PHR value should be less than this value
#define MAX_PHR_VALUE           63
// Macro defining minimum PHR value should be more than this value
#define MIN_PHR_VALUE           0
// SPR 3486 - END

/* Time Averaging changes start *
 * MAX_ATB_PRB_RESTRICT indicates the maximum number of RBs, 
 * ATB functionality will reduce the uplink Grant in case 
 * of Max power condition is hit.With MCS = 0, min 3 RBs 
 * are required to serve UL_TB_MINIMUM_SIZE_IN_BYTES. Hence, 
 * least value of MAX_ATB_PRB_RESTRICT should be atleast 3
 * */
#define MAX_ATB_PRB_RESTRICT 3
/* Time Averaging changes end */
/*SPR_PH_MAPPING_START*/
#define DIFF_PH_REPORT_PH_DB 23
/*SPR_PH_MAPPING_END*/
/* ICIC changes start */

#define MAX_UL_SCH_PARAM_INDEX 2
/* ICIC changes end */
/* SPR 4609_4610 Start */
#define MAX_PHR_IN_DB  40
#define MIN_PHR_IN_DB -23
/* SPR 22495 Changes Start */
#define MAX_POWER_REACHED_PH_VAL -23
/* SPR 22495 Changes End */
#define MIN_POWER_REACHED_PH_VAL 40
#define RB_PLUS_MCS_MIN_PHR_COND_THRESHOLD -7
#define RB_MIN_PHR_COND_THRESHOLD  0
/* SPR 4609_4610 End */

/* ATB Changes 5.1 Start */ 
/* SPR 22383 Changes Start */
#define MIN_DELTA_TF_VAL                      -7
#define MAX_DELTA_TF_VAL                      19
#define MAX_DELTA_TF_NORMALIZED_VAL           27
/* SPR 22383 Changes End */
#define MAX_TEN_LOG_M_FACTOR_TBL_VAL          21
#define INVALID_MCS_VAL_FOR_ATB               255
#define INVALID_RB_VAL_FOR_ATB                255


/* SPR 22383 Changes Start */
#define DELTA_TF_NORMALIZED_DB_VAL 7 
/* SPR 22383 Changes End */
#define DELTA_TF_RESTRICTED_VAL  mcsToDeltaTF_g[minMCSIndexForATB_g[internalCellIndex]] + DELTA_TF_NORMALIZED_DB_VAL 
#define TEN_LOG_M_ALLOCATED_RESTRICTED_VAL tenLogMFactor_g[minPRBValForATB_g[internalCellIndex]-1]
#define MAX_DB_REDUCED_THROUGH_TABLE 6
#define MAX_RB_THRESHOLD_FOR_ATB 2
#define MAX_MCS_THRESHOLD_FOR_ATB 3
#define MAX_POWER_REDUCTION_FOR_MCS_RB        MAX_DB_REDUCED_THROUGH_TABLE
/* ATB Changes 5.1 End */ 
/* CA changes start */
/*DCI Format 3A changes start*/
/* This is the threshold value for DCI Format 3A */
#define TPC_NUM_UE_THRESHOLD_FOR_PUCCH_DCI3A 1
#define TPC_NUM_UE_THRESHOLD_FOR_PUSCH_DCI3A 1
/*DCI Format 3A changes end*/
/* CA changes end */

/******************************************************************************
 Private Types
 *****************************************************************************/
typedef enum TpcValueT
{
    TPC_VALUE_0 = 0,
    TPC_VALUE_1 = 1,
    TPC_VALUE_2 = 2,
    TPC_VALUE_3 = 3
}TpcValue;

typedef enum TpcPowerValT
{
    POWER_VAL_0                =   0,
    POWER_VAL_MINUS_1          =  -1,
    POWER_VAL_MINUS_2          =  -2,
    POWER_VAL_MINUS_3          =  -3,
    POWER_VAL_MINUS_4          =  -4,
    POWER_VAL_1                =   1,
    POWER_VAL_2                =   2,
    POWER_VAL_3                =   3,
    POWER_VAL_4                =   4
}TpcPowerVal;

/******************************************************************************
 *                        PUCCH POWER CONTROL                                 *
 *****************************************************************************/
typedef struct TpcTriggeredToPowerCorrectionT
{
    /* Represents the delta PUCCH value in dB*/ 
    SInt8 powerCorrectionVal;
    /* Represents the corresponding TPC Command value of powerCorrectionVal.
     * Range (0,1,2,3)
     */
    UInt8 tpcCommand;
}TpcTriggeredToPowerCorrection;

/* This structure will be populated at initialization phase and will provide a 
 * direct indexing of PRB number to SINR Group Index. This shall also indicate
 * the PRB number of the start of next group which will be used in calculating 
 * the number of PRBs in the partially filled groups */
typedef struct PRBGroupConfigT
{
    /* Indicate groupIndex corresponding to a PRB number */
    UInt8 groupIndex;

    /* Indicate the PRB number of the next group start */
    UInt8 nextGroupStartRb;

}PRBGroupConfig;

/* this structure is used to store Scheduler Reconfig Params */
typedef struct MacReconfigSchedulerParamsULT
{
    /******************************************************************************
     *                        MSG3 POWER CONTROL                                  *
     ******************************************************************************/
    /* The TPC command in  msg2 shall be used for setting the power of the 
     * PUSCH, and is interpreted according to Table 6.2-1 of 36.213.
     * Table 6.2-1: TPC Command delta msg2 for Scheduled PUSCH
     * TPC Command Value (in dB). Range 0 to 7 as below.
     * 0 -6
     * 1 -4
     * 2 -2
     * 3 0
     * 4 2
     * 5 4
     * 6 6
     * 7 8
     **/ 
    SInt8 tpcForMsg3;

    /******************************************************************************
     *                        PUSCH POWER CONTROL                                 *
     *****************************************************************************/
    /* This shall specify the group size to be used to group PRBs according to 
     * received SINR in order to reduce processing in MAC. Possible values are 
     * 1, 2, 4 and 8.
     */

    UInt8 srsSubBandGroupSize;

    /*Wait time for sending triggered TPC in DCI0 before DCI 3/3A is
     ** scheduled*/
    UInt8 tpcTriggerWaitTimeForPUSCHDCI0;
        /*Time in 1 ms ticks that must elapse before another TPC for PUSCH
     ** power control can be sent to the UE*/
    UInt16 coolOffPeriodForPUSCHTPC;
    /*This table specifies the SINR target based on a range of pathloss*/
    AlphaBasedPathLossToTargetSINRMap alphaBasedPathLossToTargetSINRMap;

    /******************************************************************************
     *                        PHICH POWER CONTROL                                 *
     ******************************************************************************/
    /* Downlink Power control of PHICH is based on the wideband channel
     * quality feedback reported from the UE.
     * The reference table below  maps CQI feedback to PHICH delta transmission
     * power (delta power by which  PHICH power should be incremented/reduced)
     * on top of PHICH power offset configured from higher layers.
     */

    // SPR 3486  - START
    UInt32  minRbForPlPhrCalc;
    // SPR 3486  - END

    SInt8 macDeltaSINRToTPCMapForPUSCH[2*MAC_MAX_SINR + 1];

    /*Maintains a map of each PRB and it group according to srsSubBandGroupSize_g*/
    PRBGroupConfig prbToSINRGroupIndexMap[MAC_MAX_RESOURCE_BLOCKS];

    UInt8 macPathlossToTargetSINRMapForPUSCH
        [MAX_NUM_ALPHA_COUNT][PATHLOSS_TABLE_SIZE];
    
    /*When DCI 3A is triggered for a UE but not triggered for UEs on
     ** the same TPC-PUSCH-RNTI/TPC-PUCCH-RNTI, MAC waits for this much
     ** time before triggering explicit DCI0/1/2x*/
    UInt32 tpcNumUeThresholdForPUSCHDCI3A;
    /* +DYNAMIC_ICIC */
    /* ATB config parameters */
    ATBConfig atbConfigUplink;
    /* -DYNAMIC_ICIC */

    /* Maximum number of SPS activation, reactivation or deactivation which
     * can be scheduled in a TTI for uplink */
    SpsUlSchedulingInfoPerTti spsUlSchInfoPerTti;
    
    /*This table specifies the SINR target based on a range of pathloss*/
    AlphaBasedPathLossToTargetSINRMap alphaBasedPathLossToTargetSINRMapSps;
    
    UInt8 macPathlossToTargetSINRMapForSpsPUSCH
        [MAX_NUM_ALPHA_COUNT][PATHLOSS_TABLE_SIZE];
    
}MacReconfigSchedulerParamsUL;

/******************************************************************************
  * Private Function Prototypes
******************************************************************************/
/* SPR 5599 changes start (ZIP ID 129059) */
/*CA Changes start  */
/* +- SPR 17777 */
SInt8 getTPCForMsg3(InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
/*CA Changes end  */
/* SPR 5599 changes end (ZIP ID 129059) */

TpcTriggeredToPowerCorrection getAccumulatedPowerForPucch(
        SInt8 tpcTriggered, UInt8  dciFormat);

/* SPR 5599 changes start (ZIP ID 129059) */
                                /* SPR 15909 fix start */
SInt8 checkTPCForPUCCHHarqBler(UInt8 bler,/* +- SPR 17777 */
                                /* SPR 15909 fix end */
                               InternalCellIndex internalCellIndex);

                              /* SPR 15909 fix start */
SInt8 checkTPCForPUCCHCqiSinr(UInt8 sinrReportedForPucch,/* +- SPR 17777 */
                              /* SPR 15909 fix end */
                              InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */

TpcTriggeredToPowerCorrection getAbsolutePowerForPusch(SInt8 tpcTriggered, 
        SInt8 *currPowerCorrectionForPusch);

TpcTriggeredToPowerCorrection getAccumulatedPowerForPusch(SInt8 tpcTriggered,  
        UInt8 dciFormat);

/*CA Changes start  */
SInt8 checkTPCForPUSCHWidebandSINR(SInt16 pathLoss, UInt8 averageSinr, InternalCellIndex internalCellIndex);

SInt8 checkTPCForPUSCHWidebandSINRForSps(SInt16 pathLoss, UInt8 averageSinr, InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* + SPR 17439 */
SInt8 checkTPCForPUSCHSubband(void);
/* - SPR 17439 */
/* ICIC changes start */
/*CA Changes start  */
MacRetType initULSchedulerParams(InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* ICIC changes end */
/******************************************************************************
 * Private Constants
 *****************************************************************************/
/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/* Following structure stores scheduler configuration for UL */

/*CA Changes start  */
extern MacReconfigSchedulerParamsUL* macReconfigSchedulerParamsUL_gp[MAX_NUM_CELL];
extern UInt8 schParamsIndexUL_g[MAX_NUM_CELL];
/*CA Changes end  */
/* SPR_4196 Start */
extern SInt8 deltaSinrToPucchTpcMap_g[NUM_OF_SINR_TPC_VALUES+1];
/* SPR_4196 End */
extern UInt8 *macPathlossToTargetSINRMapForPUSCH_p[MAX_NUM_CELL];
extern UInt8 *macPathlossToTargetSINRMapForSpsPUSCH_p[MAX_NUM_CELL];


/******************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
#endif /*_LTE_MAC_UL_POWER_CONTROL_DATA_H_*/
