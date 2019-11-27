/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULPowerControlData.c
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
/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacULUEContext.h"
#include "lteMacCellConfMgr.h"
#include "lteMacULPowerControlData.h"
#include "lteMacDLPowerControlData.h"
#include "lteMacPuschPowerControl.h"

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
/*CA Changes start */
MacReconfigSchedulerParamsUL* macReconfigSchedulerParamsUL_gp[MAX_NUM_CELL] = {PNULL};
UInt8 schParamsIndexUL_g[MAX_NUM_CELL];
/*CA Changes end */

/* This is the wait time in 1ms ticks for sending triggered TPC in DCI0 before 
 * DCI 3/3A is scheduled 
 */



/* Flag to turn on/off CLPC PUSCH explicit trigger queue processing */
UInt8  explicitTpcTriggerEnabled_g[MAX_NUM_CELL] = {FALSE};

/* ICIC changes start */
static AlphaBasedPathLossToTargetSINRMap alphaBasedPathLossToTargetSINRMap_g = 
{
    DEFAULT_PATHLOSS,
    {
        {/* alpha = 0*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/ 
            {{          0,          80,             178},
                {          81,        130,             160},
                {          131,        200,             146}
            }
        },
        {/* alpha = 0.4*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/ 
            {{          0,          80,             178},
                {          81,         130,             160},
                {          131,        200,             146}
            }
        },
        {/* alpha = 0.5*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          0,          80,             178},
                {          81,         130,             160},
                {          131,        200,             146}
            }
        }, 
        {/* alpha = 0.6*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          0,          80,             178},
                {          81,        130,             160},
                {          131,        200,             146}
            }
        }, 
        {/* alpha = 0.7*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          0,          80,             178},
                {          81,        130,             160},
                {          131,        200,             146}
            }
        }, 
        {/* alpha = 0.8*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          0,          80,             178},
                {          81,        130,             160},
                {          131,        200,             146}
            }   
        }, 
        {/* alpha = 0.9*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          0,          80,             178},
                {          81,        130,             160},
                {          131,        200,             146}
            }      
        }, 
        {/* alpha = 1*/
            3,/* Number of tuples*/
            /*start pathloss, end path loss, target SINR*/
            {{          0,          80,             178},
                {          81,        130,             160},
                {          131,        200,             146}
            }      
        } 
    }
};

/* ICIC changes end */

static DeltaSINRToTPCMapForPUSCH deltaSINRToTPCMapForPUSCH_g =
{
    4,/* Number of tuples*/
    {
        /*startDeltaSINR, endDeltaSINR, TPC*/
        {      -255,    -26,  3},
        {      -25 ,    -10,  1},
        {      -9  ,     10,  0},
        {      11  ,    255, -1}
    }
};

TpcTriggeredToPowerCorrection tpcToPowerCorrectionForAccumForPusch_g[] =
{
    /*delta PUSCH power, TPC Command*/
    {POWER_VAL_MINUS_1, TPC_VALUE_0}, /* -1 */
    {      POWER_VAL_0, TPC_VALUE_1}, /* 0 */
    {      POWER_VAL_1, TPC_VALUE_2}, /* 1 */
    {      POWER_VAL_1, TPC_VALUE_2}, /* 2 */
    {      POWER_VAL_3, TPC_VALUE_3}, /* 3 */
};

TpcTriggeredToPowerCorrection tpcToPowerCorrectionForAbs_g[] =
{
    /*delta PUSCH power, TPC Command*/
    {POWER_VAL_MINUS_4, TPC_VALUE_0}, /* For tpcTriggered = -4 */
    {POWER_VAL_MINUS_4, TPC_VALUE_0}, /* -3 */
    {POWER_VAL_MINUS_1, TPC_VALUE_1}, /* -2 */
    {POWER_VAL_MINUS_1, TPC_VALUE_1}, /* -1 */
    {POWER_VAL_MINUS_1, TPC_VALUE_1}, /* 0 */
    {      POWER_VAL_1, TPC_VALUE_2}, /* 1 */
    {      POWER_VAL_1, TPC_VALUE_2}, /* 2 */
    {      POWER_VAL_4, TPC_VALUE_3}, /* 3 */
    {      POWER_VAL_4, TPC_VALUE_3}  /* 4 */
};

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

TpcTriggeredToPowerCorrection tpcToPowerCorrectionForAccumForPucch_g[] =
{
    /*delta PUCCH power, TPC Command*/
    {POWER_VAL_MINUS_1, TPC_VALUE_0}, /* -1 */
    {      POWER_VAL_0, TPC_VALUE_1}, /* 0 */
    {      POWER_VAL_1, TPC_VALUE_2}, /* 1 */
    {      POWER_VAL_1, TPC_VALUE_2}, /* 2 */
    {      POWER_VAL_3, TPC_VALUE_3}, /* 3 */
};
/******************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/


/******************************************************************************
 *             MSG3 POWER CONTROL OPEN INTERFACE                              *
 *****************************************************************************/
/******************************************************************************
 * Function Name  : getTPCForMsg3 
 * Inputs         : timingAdvance - may be used to calculate the TPC to be sent
 *                                  in RAR for Msg3 transmission.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : TPC command to be sent in RAR for Msg3
 * Returns        : TPCForMsg3
 * Description    : Current implementation returns the value in global variable
 *                  tpcForMsg3_g. This is sent in RAR for Msg3 Power control.
 *****************************************************************************/
/*CA Changes start  */
/* +- SPR 17777 */
 SInt8 getTPCForMsg3(InternalCellIndex internalCellIndex)
    /* +- SPR 17777 */
{
    return (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->tpcForMsg3;
}
/*CA Changes end  */

/******************************************************************************
 *             PUSCH POWER CONTROL OPEN INTERFACE                             *
 *****************************************************************************/
/******************************************************************************
 * Function Name  : getAbsolutePowerForPusch
 * Inputs         : tpcTriggered - New PUSCH power correction triggered,
 *                  currPowerCorrectionForPusch - PUSCH power correction 
 *                                                already signalled to the UE.
 * Outputs        : Tuple containing delta PUSCH power correction(absolute)
 *                  and the TPC command corresponding to it
 * Returns        : TpcTriggeredToPowerCorrection  
 * Description    : Based on the new PUSCH power correction triggered and the
 *                  PUSCH power correction already signalled to the UE, this
 *                  function returns a tuple containing the new delta PUSCH 
 *                  power correction(absolute)
 *                  and the TPC command corresponding to it.
 *****************************************************************************/
TpcTriggeredToPowerCorrection getAbsolutePowerForPusch(
        SInt8 tpcTriggered, 
        SInt8 *currPowerCorrectionForPusch)
{
    /* first the TPC command is trimmed to the supported range. Eg, in DCI 
       Format 0 maximum and minimum value of TPC that can be sent is -4 and 4
       respectively when accumulation is disabled. 
       Thus if TPC triggered is more than 4 then it is assumed to be 4 or if 
       TPC triggered is less than -4, then it is assumed to be -4.  */
    tpcTriggered += *currPowerCorrectionForPusch;

    if (tpcTriggered < POWER_VAL_MINUS_4)
    {
        tpcTriggered = POWER_VAL_MINUS_4;
    }
    else if (tpcTriggered > POWER_VAL_4)
    {
        tpcTriggered = POWER_VAL_4;
    }
    
    /* return the tuple corresponding to the triggered TPC value.
     * This tuple contains the mapping of delta PUSCH power correction and
     * the TPC command to be used for it.
     */
    return (tpcToPowerCorrectionForAbs_g[tpcTriggered + POWER_VAL_4]);
}

/*****************************************************************************
 * Function Name  : getAccumulatedPowerForPusch
 * Inputs         : tpcTriggered - New PUSCH power correction triggered
 *                  dciFormat - The DCI Format using which TPC command will be 
 *                              sent
 * Outputs        : Tuple containing delta PUSCH power correction(accumulated)
 *                  and the TPC command corresponding to it
 * Returns        : TpcTriggeredToPowerCorrection
 * Description    : Based on the new PUSCH power correction triggered, this
 *                  function returns a tuple containing the new delta PUSCH 
 *                  power correction(accumulated)
 *                  and the TPC command corresponding to it.
 *****************************************************************************/
TpcTriggeredToPowerCorrection getAccumulatedPowerForPusch(
        SInt8 tpcTriggered, 
        UInt8 dciFormat)
{
    /* According the the DCI Format using which TPC needs to be sent, first the
     * TPC command is trimmed to the supported range. For example, in DCI 
     * Format 0 minimum and maximum value of TPC that can be sent is -1 and 3
     * respectively when accumulation is enabled. 
     * Thus if TPC triggered is more than 3 then it is assumed to be 3 or if 
     * TPC triggered is less than -1, then it is assumed to be -1.  
     */
    switch (dciFormat)
    {
        case DCI_FORMAT_0:
        case DCI_FORMAT_3:
        {
            if (tpcTriggered < POWER_VAL_MINUS_1)
            {
                tpcTriggered = POWER_VAL_MINUS_1;
            }
            else if (tpcTriggered > POWER_VAL_3)
            {
                tpcTriggered = POWER_VAL_3;
            }
        }
        break;
        case DCI_FORMAT_3A:
        {
            if ( tpcTriggered <= POWER_VAL_0)
            {
                tpcTriggered = POWER_VAL_MINUS_1;
            }
            else
            {
                tpcTriggered = POWER_VAL_0;
            }
        }
        break;
        default:
        tpcTriggered = POWER_VAL_0;
    }

    /* return the tuple corresponding to the triggered TPC value.
     * This tuple contains the mapping of delta PUSCH power correction and
     * the TPC command to be used for it.
     */
    return (tpcToPowerCorrectionForAccumForPusch_g\
            [tpcTriggered + POWER_VAL_1]);
}

/*****************************************************************************
 * Function Name  : checkTPCForPUSCHWidebandSINR
 * Inputs         : pathLoss - Based on this target SINR is calculated
 *                  averageSinr - Wideband average SINR in uplink
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : delta PUSCH Power correction
 * Returns        : PUSCH SINR 
 * Description    : This function fetches the target SINR based on the pathloss
 *                  in uplink. Based on the diff of target SINR and the average 
 *                  wideband SINR in uplink, delta PUSCH power correction is
 *                  returned.
 *****************************************************************************/
 SInt8 checkTPCForPUSCHWidebandSINR(SInt16 pathLoss, UInt8 averageSinr,
                                          /*CA Changes start  */
                                          InternalCellIndex internalCellIndex)
                                          /*CA Changes end  */
{
    SInt16 deltaSINR = 0;
    /* ICIC changes start */
/* SPR_4201 Start */
    UInt8 targetSINR = *(macPathlossToTargetSINRMapForPUSCH_p[internalCellIndex] + pathLoss);
    /* ICIC changes end */
/* SPR_4201 End */

    deltaSINR = (SInt16)averageSinr - targetSINR;
    
    /* return the delta PUSCH power correction that needs to be triggered
     * corresponding to the difference in uplink average wideband SINR and
     * the target SINR.
     */
    /* ICIC changes start */
    /*CA Changes start  */
    return (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
        macDeltaSINRToTPCMapForPUSCH[deltaSINR + MAC_MAX_SINR];
    /*CA Changes end  */
    /* ICIC changes end */
}

/*****************************************************************************
 * Function Name  : checkTPCForPUSCHWidebandSINRForSps
 * Inputs         : pathLoss - Based on this target SINR is calculated
 *                  averageSinr - Wideband average SINR in uplink
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : delta PUSCH Power correction
 * Returns        : PUSCH SINR for SPS 
 * Description    : This function fetches the target SINR based on the pathloss
 *                  in uplink. Based on the diff of target SINR and the average 
 *                  wideband SINR in uplink, delta PUSCH power correction is
 *                  returned.
 *****************************************************************************/
 SInt8 checkTPCForPUSCHWidebandSINRForSps(SInt16 pathLoss, UInt8 averageSinr,
                                                /*CA Changes start  */
                                                InternalCellIndex internalCellIndex)
                                                /*CA Changes end  */
{
    SInt16 deltaSINR = 0;
    UInt8 targetSINR = *(macPathlossToTargetSINRMapForSpsPUSCH_p[internalCellIndex] + pathLoss);

    deltaSINR = (SInt16)averageSinr - targetSINR;
    
    /* return the delta PUSCH power correction that needs to be triggered
     * corresponding to the difference in uplink average wideband SINR and
     * the target SINR.
     */
    /*CA Changes start  */
    return (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
        macDeltaSINRToTPCMapForPUSCH[deltaSINR + MAC_MAX_SINR];
    /*CA Changes end  */
}



/*****************************************************************************
 * Function Name  : checkTPCForPUSCHSubband
 * Inputs         : None
 * Outputs        : delta Pusch Power correction
 * Returns        : 0 
 * Description    : Implementation not supported currently
 *****************************************************************************/
/* + SPR 17439 */
SInt8 checkTPCForPUSCHSubband(void){ return 0;}
/* - SPR 17439 */
/******************************************************************************
 *             PUCCH POWER CONTROL OPEN INTERFACE                             *
 *****************************************************************************/
/*****************************************************************************
 * Function Name  : getAccumulatedPowerForPucch
 * Inputs         : tpcTriggered - New PUCCH power correction triggered
 *                  dciFormat - The DCI Format using which TPC command will be 
 *                              sent
 * Outputs        : Tuple containing delta PUCCH power correction
 *                  and the TPC command corresponding to it.
 * Returns        : TpcTriggeredToPowerCorrection
 * Description    : Based on the new PUCCH power correction triggered, this
 *                  function returns a tuple containing the new delta PUCCH 
 *                  power correction(accumulated)
 *                  and the TPC command corresponding to it.
 *****************************************************************************/
TpcTriggeredToPowerCorrection getAccumulatedPowerForPucch(
        SInt8 tpcTriggered, 
        UInt8 dciFormat)
{
    /* According the the DCI Format using which TPC needs to be sent, first the
     * TPC command is trimmed to the supported range. 
     * For example, the minimum and maximum value of TPC that can be sent is 
     * -1 and 3 respectively. 
     * Thus if TPC triggered is more than 3 then it is assumed to be 3 or if 
     * TPC triggered is less than -1, then it is assumed to be -1.  
     */
    switch (dciFormat)
    {
        case DCI_FORMAT_1:
        case DCI_FORMAT_1A:
        case DCI_FORMAT_1B:
        case DCI_FORMAT_1D:
        case DCI_FORMAT_2:
        case DCI_FORMAT_2A:
        /* + TM7_8 Changes Start */
        case DCI_FORMAT_2B:
        /* - TM7_8 Changes End */
        case DCI_FORMAT_3:
        {
            if ( tpcTriggered < POWER_VAL_MINUS_1)
                tpcTriggered = POWER_VAL_MINUS_1;
            else if (tpcTriggered > POWER_VAL_3)
                tpcTriggered = POWER_VAL_3;
            break;
        }
        case DCI_FORMAT_3A:
        {
            if ( tpcTriggered <= POWER_VAL_0)
                tpcTriggered = POWER_VAL_MINUS_1;
            else
                tpcTriggered = POWER_VAL_0;
            break;
        }
        default:
        tpcTriggered = POWER_VAL_0;
    }

    /* return the tuple corresponding to the triggered TPC value.
     * This tuple contains the mapping of delta PUCCH power correction and
     * the TPC command to be used for it.
     */
    return (tpcToPowerCorrectionForAccumForPucch_g\
            [tpcTriggered + POWER_VAL_1]);
}

/*****************************************************************************
 * Function Name  : checkTPCForPUCCHHarqBler
 * Inputs         : bler - PUCCH Bler from UE Context,
 *                  currentTick - Current TTI which may be used for logging
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : delta PUCCH Power correction
 * Returns        : BLER 
 * Description    : This function takes the input BLER and maps it to 
 *                  a TPC that needs to be triggered for PUCCH Power Control 
 *                  if required.
 *****************************************************************************/
/* SPR 15909 fix start */ 
 SInt8 checkTPCForPUCCHHarqBler(UInt8 bler,/* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
        /* SPR 15909 fix end */
{
    /* ICIC changes start */
    /*CA Changes start  */
    return (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
        deltaHarqBlerToPucchTpcMap[bler];
    /*CA Changes end  */
    /* ICIC changes end */
}
/*****************************************************************************
 * Function Name  : checkTPCForPUCCHCqiSinr
 * Inputs         : sinrReportedForPucch - SINR reported for PUCCH,
 *                  currentTick - Current Tick which may be used for logging
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : delta PUCCH Power correction
 * Returns        : PUCCH SINR reported 
 * Description    : This function takes the inpuut PUCCH SINR and maps it to
 *                  a TPC that needs to be triggered for PUCCH Power control
                    if required.
 *****************************************************************************/
 SInt8 checkTPCForPUCCHCqiSinr(UInt8 sinrReportedForPucch,
                            /* +- SPR 17777 */
                            InternalCellIndex internalCellIndex)
{
    /*CA Changes start  */
    return (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])-> \
        deltaSinrToPucchTpcMap[sinrReportedForPucch];
    /*CA Changes end  */
}

/*****************************************************************************
 * Function Name  : initULSchedulerParams
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Variables      : None
 * Description    : This function initialize UL scheduler Parameters.
 *****************************************************************************/
/*CA Changes start  */
MacRetType initULSchedulerParams(InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt8 index = 0;
    MacReconfigSchedulerParamsUL *schedulerParamsUL_p = PNULL;

    /*CA Changes start  */
    GET_MEM_FROM_POOL(MacReconfigSchedulerParamsUL, macReconfigSchedulerParamsUL_gp[internalCellIndex], 
                          sizeof(MacReconfigSchedulerParamsUL) * MAX_UL_SCH_PARAM_INDEX, PNULL);

    schParamsIndexUL_g[internalCellIndex] = 0;
    schedulerParamsUL_p = macReconfigSchedulerParamsUL_gp[internalCellIndex] +
                       schParamsIndexUL_g[internalCellIndex];
    /*CA Changes end  */
     UInt8 alpha = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
             cellParams_p->commonULPowerControlInfo.alpha;
    schedulerParamsUL_p->tpcForMsg3 = 3;
    schedulerParamsUL_p->srsSubBandGroupSize = 4;
    schedulerParamsUL_p->coolOffPeriodForPUSCHTPC = 0;
    schedulerParamsUL_p->alphaBasedPathLossToTargetSINRMap = \
        alphaBasedPathLossToTargetSINRMap_g;
    schedulerParamsUL_p->tpcTriggerWaitTimeForPUSCHDCI0 = 4;
    populatePathlossToTargetSINRMapForPUSCH(&alphaBasedPathLossToTargetSINRMap_g,
    schedulerParamsUL_p,LTE_FALSE); /* SPS_FLAG FALSE for NON SPS */
    /* SPS_CHG */
    schedulerParamsUL_p->spsUlSchInfoPerTti.maxUlSpsDciPerTti = 2; 
    schedulerParamsUL_p->spsUlSchInfoPerTti.maxUlSpsOccasionPerTti = 4; 
    schedulerParamsUL_p->spsUlSchInfoPerTti.maxUlSpsRbsPerTti = 20;
    for (index = 0; index < MAX_SPS_INTERVALS; index++)
    {
        schedulerParamsUL_p->spsUlSchInfoPerTti.maxUlSpsRbsPerTtiPerInterval[index] = 20;
    }
    schedulerParamsUL_p->alphaBasedPathLossToTargetSINRMapSps = \
        alphaBasedPathLossToTargetSINRMap_g; 
    populatePathlossToTargetSINRMapForPUSCH(&alphaBasedPathLossToTargetSINRMap_g,
    schedulerParamsUL_p,LTE_TRUE); /* SPS_FLAG TRUE for SPS */
    /* SPS_CHG */
    populateSinrToTpcMapForPUSCH(&deltaSINRToTPCMapForPUSCH_g,schedulerParamsUL_p);
    schedulerParamsUL_p->minRbForPlPhrCalc = 1;
    /* Possible values of alpha are 0,4,5,6,7,8,9 and 10. so if alpha comes
     * is less than 3 use index 0. However values 1,2 and 3 should not come 
     */
    if(alpha <= 3)
    {
        /*CA Changes start  */
        /* ICIC changes start */
        macPathlossToTargetSINRMapForPUSCH_p[internalCellIndex] = 
            (macReconfigSchedulerParamsUL_gp[internalCellIndex] +
            schParamsIndexUL_g[internalCellIndex])->macPathlossToTargetSINRMapForPUSCH[0];
        /* ICIC changes end */
        macPathlossToTargetSINRMapForSpsPUSCH_p[internalCellIndex] = 
            (macReconfigSchedulerParamsUL_gp[internalCellIndex] + 
            schParamsIndexUL_g[internalCellIndex])->macPathlossToTargetSINRMapForSpsPUSCH[0];
    }
    else if (alpha <= 10)
    {
        /* ICIC changes start */
        macPathlossToTargetSINRMapForPUSCH_p[internalCellIndex] = \
            (macReconfigSchedulerParamsUL_gp[internalCellIndex] + 
            schParamsIndexUL_g[internalCellIndex])->macPathlossToTargetSINRMapForPUSCH \
            [alpha - 3];
        /* ICIC changes end */
        macPathlossToTargetSINRMapForSpsPUSCH_p[internalCellIndex] = \
            (macReconfigSchedulerParamsUL_gp[internalCellIndex] + 
            schParamsIndexUL_g[internalCellIndex])->macPathlossToTargetSINRMapForSpsPUSCH \
            [alpha - 3];
        /*CA Changes end  */
    }
    else
    {
        return MAC_FAILURE;
    }
    
    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_SPS_UL_SCH_INFO_PER_TTI_STATS(&(schedulerParamsUL_p->spsUlSchInfoPerTti),
            internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  resetEntryInTpcSentMap
 * Inputs         :  mapIdx, internalCellIndex
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will reset the tpcSentMap_g for PUCCH/PUSCH
 *****************************************************************************/
/* SPR 23393 Changes Start */
 void resetEntryInTpcSentMap( UInt8 mapIdx , InternalCellIndex internalCellIndex)
/* SPR 23393 Changes End */
{
    /* Reset the TPC map entry before actually filling in */
    tpcSentMap_g[internalCellIndex][mapIdx].numUe = 0;
    tpcSentMap_g[internalCellIndex][mapIdx].tick  = 0;
}

/*****************************************************************************
 * Function Name  :  putEntryInTpcSentMap
 * Inputs         :  currentTick
 *                   mapIdx
 *                   rnti, powerCorrectionVal
 *                   powerCtrlType
 *                   internalCellIndex
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will update the tpcSentMap_g for PUCCH/PUSCH
 *****************************************************************************/
/* SPR 15909 fix start */
void putEntryInTpcSentMap( tickType_t currentTick, UInt8 mapIdx,
        /* SPR 15909 fix end */
        UInt16 rnti, SInt8 powerCorrectionVal, 
        ErrIndPowerControlType powerCtrlType,
        InternalCellIndex internalCellIndex)
{
    UInt8 count = 0;
    /* Reset the num of UE if the function is called for first time */
    if (currentTick != tpcSentMap_g[internalCellIndex][mapIdx].tick)
    {
        tpcSentMap_g[internalCellIndex][mapIdx].numUe = 0;
    }
    count = tpcSentMap_g[internalCellIndex][mapIdx].numUe;
    tpcSentMap_g[internalCellIndex][mapIdx].tick = currentTick;
    tpcSentMap_g[internalCellIndex][mapIdx].tpcSentInfo[count].rnti = rnti;
    tpcSentMap_g[internalCellIndex][mapIdx].tpcSentInfo[count].pcType = powerCtrlType;
    tpcSentMap_g[internalCellIndex][mapIdx].tpcSentInfo[count].powerCorrectionVal = 
        powerCorrectionVal;
    tpcSentMap_g[internalCellIndex][mapIdx].numUe++;
     
    LOG_MAC_MSG( MAC_PUT_ENTRY_IN_TPC, LOGDEBUG, MAC_UL_PC,
             getCurrentTick(), mapIdx, rnti, powerCtrlType,
             powerCorrectionVal, tpcSentMap_g[internalCellIndex][mapIdx].numUe, 0,0,
             __func__, "");
}
