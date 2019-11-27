/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLPowerControlData.c
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
/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacDLPowerControlData.h"
#include "lteMacPucchPowerControl.h"  
/******************************************************************************
 Private Definitions
 *****************************************************************************/
/******************************************************************************
 Private Types
 *****************************************************************************/
/******************************************************************************
  * Private Function Prototypes
******************************************************************************/
/******************************************************************************
 * Private Constants
 *****************************************************************************/
/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/* Following structure stores scheduler configuration for DL */
MacReconfigSchedulerParamsDL* macReconfigSchedulerParamsDL_gp[MAX_NUM_CELL] = {PNULL};
UInt8 schParamsIndexDL_g[MAX_NUM_CELL];
/******************************************************************************
 *  *                        PUCCH POWER CONTROL                              *
 *  **************************************************************************/

/* This is the wait time for which UE waits in tpcTriggerQueue while waiting
 * to be scheduled in DCI Format 1x/2x 
 */

/* 36.213, Table 5.1.2.1-1 and Table 5.1.2.1-2. 
 * Values allowed for TPC power  command for PUCCH in DCI format 
 * 1A/1B/1D/1/2A/2/3 and 3A are -1, 0, 1 and 3.
 * If Delta PUCCH BLER is in the range [-100,10], this is mapped to  TPC 
 * command -1  for PUCCH. If it is in the range [11,30], then it implies 
 * no change in PUCCH power due to delta BLER and so on.
 */
static DeltaBLERToTPCMapForPUCCH deltaBlerToTPCMapForPUCCH_g = 
{
    10, /*pucchBLERTarget*/
    4,  /* Number of tuples*/
    {
        /*startDeltaBLER, endDeltaBLER, TPC*/
        {      -10,   -5,    -1},
        {       0  ,   2,     0},
        {       3 ,   20,     1},
        {       21 ,   90,    3}
    }
};
static DeltaSINRToTPCMapForPUCCH deltaSINRToTPCMapForPUCCH_g = 
{
    138, /*pucchSINRTarget*/
    4,   /* Number of tuples*/
    {
        /*startDeltaSINR, endDeltaSINR, TPC*/
        {      -255,    -26,  3},
        {      -25 ,    -10,  1},
        {      -9  ,     10,  0},
        {      11  ,     255, -1}
    }
};

/******************************************************************************
 *                        PDCCH POWER CONTROL                                 *
 *****************************************************************************/
/* For PDCCH power control MAC shall support change of aggregation level or/and
 * power level based on CQI feedback reported from UE. The aggregation level 
 * will also depend on whether the DCI size is categorized as small, medium or 
 * big. Refer constants DCI_SIZE_THRESHOLD_SMALL and DCI_SIZE_THRESHOLD_MEDIUM.
 * The list of Aggregation level and delta transmission power offset can have
 * maximum of 4 sets to allow all the aggregation levels to be tried for 
 * allocation in worst scenario. No two sets in the list can have same
 * aggregation level mapping.
 * The sets of aggregation level and delta transmission power offset should be
 * according the priority of aggregation level.
 */
/* ICIC changest start*/
PdcchAggregationAndPowerOffset pdcchAggregationAndPowerOffset_g =
{
        {
            { /* Small DCI Size*/
                /*Count,  {"Count" number of sets of {aggregation level, power offset}} */
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  1*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  2*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  3*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  4*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  5*/
                { 2,  { {2,6000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  6*/
                { 2,  { {2,6000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  7*/
                { 2,  { {2,6000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  8*/
                { 2,  { {2,6000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI =  9*/
                { 2,  { {2,6000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 10*/
                { 2,  { {2,6000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 11*/
                { 2,  { {2,6000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 12*/
                { 2,  { {2,6000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 13*/
                { 2,  { {2,6000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 14*/
                { 2,  { {2,6000}, {1, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 15*/
            },
            { /*Medium DCI Size */
                /*Count,  {"Count" number of sets of {aggregation level, power offset}} */
                { 2,  { {8,6000}, {4, 7000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},           /* CQI =  1*/
                { 2,  { {8,6000}, {4, 7000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},           /* CQI =  2*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},       /* CQI =  3*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF, 0xFF}, {0xFF, 0xFF}}},       /* CQI =  4*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},       /* CQI =  5*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  6*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  7*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  8*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}},    /* CQI =  9*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 10*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 11*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 12*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 13*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 14*/
                { 2,  { {4,6000}, {2, 7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 15*/

            },
            { /*Big DCI Size */
                /*Count,  {"Count" number of sets of {aggregation level, power offset}} */
                { 2,  { {8,6000},  {4,8000},  {0xFF,0xFF}, {0xFF,0xFF}}},          /* CQI =  1*/
                { 2,  { {8,6000},  {4,8000},  {0xFF,0xFF}, {0xFF,0xFF}}},          /* CQI =  2*/
                { 2,  { {8,6000},  {4,7000},  {0xFF,0xFF}, {0xFF,0xFF}}},          /* CQI =  3*/
                { 2,  { {8,6000},  {4,7000},  {0xFF,0xFF}, {0xFF,0xFF}}},       /* CQI =  4*/
                { 2,  { {4,6000},  {2,8000},  {0xFF,0xFF}, {0xFF,0xFF}}},       /* CQI =  5*/
                { 2,  { {4,6000},  {2,8000},  {0xFF,0xFF}, {0xFF,0xFF}}},       /* CQI =  6*/
                { 2,  { {4,6000},  {2,7000},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI =  7*/
                { 2,  { {4,6000},  {2,7000},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI =  8*/
                { 2,  { {4,6000},  {2,7000},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI =  9*/
                { 2,  { {4,6000},  {2,7000},  {0xFF,0xFF}, {0xFF,0xFF}}},    /* CQI = 10*/
                { 2,  { {4,6000},  {2,7000},  {0xFF,0xFF}, {0xFF, 0xFF}}},   /* CQI = 11*/
                { 2,  { {4,6000},  {2,7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 12*/
                { 2,  { {4,6000},  {2,7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 13*/
                { 2,  { {4,6000},  {2,7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 14*/
                { 2,  { {4,6000},  {2,7000}, {0xFF,0xFF}, {0xFF, 0xFF}}}, /* CQI = 15*/
            }
        }

};
/* ICIC changes end */

/* SPR 3100 START */
// Stores CQI to DL SNR mapping. The maximun SINR value is stored at the
// For the SinrtoCqi refer sinrToCQI_g in lteMacULCSImanager.c
 UInt8 cqiToDLSinr_g[MAX_CQI_VAL] =
{
    122, /*CQI Index 1*/
    127, /*CQI Index 2*/
    130, /*CQI Index 3*/
    136, /*CQI Index 4*/
    141, /*CQI Index 5*/
    146, /*CQI Index 6*/
    149, /*CQI Index 7*/
    156, /*CQI Index 8*/
    160, /*CQI Index 9*/
    163, /*CQI Index 10*/
    166, /*CQI Index 11*/
    170, /*CQI Index 12*/
    174, /*CQI Index 13*/
    179, /*CQI Index 14*/
    180  /*CQI Index 15*/
};

/* SPR 3100 END */
/******************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/******************************************************************************
 *             PDCCH POWER CONTROL OPEN INTERFACE                             *
 *****************************************************************************/
/*****************************************************************************
 * Function Name  : getDCISizeCategory 
 * Inputs         : dciSize - Size in bits of user information in a DCI Format
 * Outputs        : Category of the size(small, medium or big)
 * Returns        : DCI_STEP_SIZE 
 * Variables      : None
 * Description    : Based on the dciSize in bits, it determines whether this
 *                  falls in small, medium or big size category.
 *****************************************************************************/
UInt8 getDCISizeCategory(UInt8 dciSize)
{
    if (dciSize <= DCI_SIZE_THRESHOLD_SMALL)
    {
        return SMALL_DCI_STEP_SIZE;
    }
    else if (dciSize <= DCI_SIZE_THRESHOLD_MEDIUM)
    {
        return MEDIUM_DCI_STEP_SIZE;
    }
    else
    {
        return BIG_DCI_STEP_SIZE;
    }
}

/*****************************************************************************
 * Function Name  : getDCIBitSize 
 * Inputs         : dciFormat - Input DCI format for which size in bits is reqd,
 *                  numOfTxAntennas - Number of Tx Antennae configured in the 
 *                                    cell,
 *                  dlAvailableRBs - Number of UL RBs configured in the cell.
 * Outputs        : Number of bits of user information in a DCI Format based
 *                  on DCI format, NumOfTxAntennas and dlAvailableRBs
 * Returns        : DCIBitSize
 * Variables      : None
 * Description    : This function first check for the number of configured
 *                  RBs in uplink and then based on DCI Format and number
 *                  of Tx Antenna, returns the number of bits of user
 *                  information in the DCI format.
 *****************************************************************************/
UInt8 getDCIBitSize(
        UInt8 dciFormat, 
        UInt8 numOfTxAntennas, 
        UInt8 dlAvailableRBs) 
{
    /*+-  SPR 18481 */
    switch(dlAvailableRBs)
    {
        case DL_RES_BLKS_6RBS:
            return dciSize_g[dciFormat][DL_BW_1DOT4_MHZ][numOfTxAntennas-1];
        case DL_RES_BLKS_15RBS:
            return dciSize_g[dciFormat][DL_BW_3_MHZ][numOfTxAntennas-1];
        case DL_RES_BLKS_25RBS:
            return dciSize_g[dciFormat][DL_BW_5_MHZ][numOfTxAntennas-1];
        case DL_RES_BLKS_50RBS:
            return dciSize_g[dciFormat][DL_BW_10_MHZ][numOfTxAntennas-1];
        case DL_RES_BLKS_75RBS:
            return dciSize_g[dciFormat][DL_BW_15_MHZ][numOfTxAntennas-1];
        case DL_RES_BLKS_100RBS:
            return dciSize_g[dciFormat][DL_BW_20_MHZ][numOfTxAntennas-1];
        default:
            return MAC_FAILURE;/*Compilation fix*/
    /*+-  SPR 18481 */
    }
}

/* ICIC changes start */
/* ICIC changes end */

/******************************************************************************
 *             PHICH POWER CONTROL OPEN INTERFACE                             *
 *****************************************************************************/
/******************************************************************************
 * Function Name  : getPHICHPowerOffset 
 * Inputs         : widebandCqi - Based on which PHICH Power Offset is 
 *                                calculated
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : Delta PHICH Power Offset
 * Returns        : PHICHPowerOffset
 * Variables      : None
 * Description    : Return the CQI feedback based PHICH delta transmission
 * power (delta power by which  PHICH power should be incremented/reduced)
 * on top of PHICH power offset configured from higher layers.
 *****************************************************************************/
UInt16 getPHICHPowerOffset(UInt8 widebandCqi, InternalCellIndex internalCellId)
{
    /* CR changes start */
    return (macReconfigSchedulerParamsDL_gp[internalCellId] + schParamsIndexDL_g[internalCellId])->\
        cqiToPHICHPower.cqiToPHICHPowerInfo[widebandCqi - 1];
    /* CR changes end */
}

/* SPR 3100 START */
/******************************************************************************
 * FUNCTION NAME : getDLSinrValueFromCqi
 * INPUTS        : cqiIndex, internalCellIndex
 * OUTPUTS       : SINR based on the cqi index
 * RETURNS       : DLSinr Value
 * DESCRIPTION   : This method returns the SINR value based on the received
 *                 cqi Index.
 ******************************************************************************/
/* +- SPR 17777 */
UInt8 getDLSinrValueFromCqi(UInt8 cqiIndex)
/* +- SPR 17777 */
{
        return cqiToDLSinr_g[cqiIndex-1];
}

/* SPR 3100 END */

/*****************************************************************************
 * Function Name  : initDLSchedulerParams
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Variables      : None
 * Description    : This function initialize DL scheduler Parameters.
 *****************************************************************************/
/* CA changes start*/
void initDLSchedulerParams(InternalCellIndex internalCellIndex)
{
    UInt8 index = 0;
    MacReconfigSchedulerParamsDL *schedulerParamsDL_p = PNULL;
  
    GET_MEM_FROM_POOL(MacReconfigSchedulerParamsDL, macReconfigSchedulerParamsDL_gp[internalCellIndex],
                      sizeof(MacReconfigSchedulerParamsDL) * MAX_DL_SCH_PARAM_INDEX, PNULL);

    schParamsIndexDL_g[internalCellIndex] = 0;
    schedulerParamsDL_p = (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex]);

    schedulerParamsDL_p->pdcchAggregationAndPowerOffset[CC_USER] = \
                                                                   pdcchAggregationAndPowerOffset_g;
    schedulerParamsDL_p->pdcchAggregationAndPowerOffset[CE_USER] = \
                                                                   pdcchAggregationAndPowerOffset_g;

    schedulerParamsDL_p->tpcTriggerWaitTimeForPUCCHDCI1x2x = 4;

    schedulerParamsDL_p->coolOffPeriodForBLERTriggeredPUCCHTPC = 0;

    schedulerParamsDL_p->coolOffPeriodForPUCCHTPC = 0;
    memSet(&schedulerParamsDL_p->cqiToPHICHPower,
            0,sizeof(CqiToPHICHPower));
    schedulerParamsDL_p->deltaSinrToTPCMapForPUCCH = deltaSINRToTPCMapForPUCCH_g;

    initSinrToTpcMap(&deltaSINRToTPCMapForPUCCH_g,schedulerParamsDL_p);
    schedulerParamsDL_p->deltaBlerToTPCMapForPUCCH = deltaBlerToTPCMapForPUCCH_g;
    initHarqBlerToTPCMap(&deltaBlerToTPCMapForPUCCH_g,schedulerParamsDL_p);
    /* SPS_CHG */
    schedulerParamsDL_p->spsDlSchInfoPerTti.maxDlSpsDciPerTti = 2;
    schedulerParamsDL_p->spsDlSchInfoPerTti.maxDlSpsOccasionPerTti = 4;
    schedulerParamsDL_p->spsDlSchInfoPerTti.maxDlSpsRbsPerTti = 20;
    for (index = 0; index < MAX_SPS_INTERVALS; index++)
    {
        schedulerParamsDL_p->spsDlSchInfoPerTti.maxDlSpsRbsPerTtiPerInterval[index] = 20;
    }

    LTE_MAC_UPDATE_SPS_DL_SCH_INFO_PER_TTI_STATS(&(schedulerParamsDL_p->spsDlSchInfoPerTti),
            internalCellIndex);

    /* SPS_CHG */
    /* +DYNAMIC_ICIC */
    schedulerParamsDL_p->paQpsk = 1; /* 1 corresponds to -4.77 dB */
    /* -DYNAMIC_ICIC */
    /* + TM7_8 Changes End */ 
    schedulerParamsDL_p->bfVectorPresentFlag = BF_ALGO_NOT_AVAILABLE; 
    /* - TM7_8 Changes End */ 
    schedulerParamsDL_p->caParams.bitMask = 0;
    schedulerParamsDL_p->caParams.loadCalcualtionWindowSize = 1000;
    schedulerParamsDL_p->caParams.loadDisparityThreshold = 40;
    schedulerParamsDL_p->caParams.mcsDiffThreshold = 5;
    schedulerParamsDL_p->caParams.prbUsageWaterMark = 80;
    schedulerParamsDL_p->caParams.dlDeactCommandThresh = 8;
    schedulerParamsDL_p->caParams.dlDeactMCSLowThresh = 6;
    schedulerParamsDL_p->caParams.dlQLoadUpThresh = 100 * RRM_SCELL_QLOAD_THRESH_MULTIPLIER;
    schedulerParamsDL_p->caParams.dlQLoadLowThresh = 50 * RRM_SCELL_QLOAD_THRESH_MULTIPLIER;
    schedulerParamsDL_p->caParams.actDeactTriggerCountThresh = 10; 
    /* SPR 16422 START */
    /* Initialize with value 0 i.e., infinity */
    schedulerParamsDL_p->caParams.cqiValidityTimer = 0; 
    /* SPR 16422 END */
    /* CA changes end*/
#ifdef LTE_EMBMS_SUPPORTED
    schedulerParamsDL_p->eMbmsReservedCellPowerOffset = 1;  /* 1db */
#endif
}

