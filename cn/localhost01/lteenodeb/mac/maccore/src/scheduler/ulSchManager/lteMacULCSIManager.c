/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULCSIManager.c 
 *
 ******************************************************************************
 *
 *  File Description : This file contains implementation of Uplink Channel 
 *                     State Information Manager in LTE_MAC.
 *                     
 ******************************************************************************
 *
 * Revision Details
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacULCSIManager.h"
#include "lteMacULUEContext.h"
#include "lteMacCQI.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacULPowerControlData.h"
#include "lteMacPuschPowerControl.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif

/* ICIC changes start */
#include "lteMacULResourceInfoMgr.h"
#include "lteMacRRMPeriodicMgr.h"
/* ICIC changes end */

/******************************************************************************
  Private Definitions
 *****************************************************************************/
/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 2480 Start */ 
/*SPR 22382 Changes Start*/
static UInt8 sinrToCQI_g[MAC_MAX_SINR+1] = 
{
    1,1,1,1,1,1,1,1,1,1, /*0-9*/
    1,1,1,1,1,1,1,1,1,1, /*10-19*/
    1,1,1,1,1,1,1,1,1,1, /*20-29*/
    1,1,1,1,1,1,1,1,1,1, /*30-39*/
    1,1,1,1,1,1,1,1,1,1, /*40-49*/
    1,1,1,1,1,1,1,1,1,1, /*50-59*/
    1,1,1,1,1,1,1,1,1,1, /*60-69*/
    1,1,1,1,1,1,1,1,1,1, /*70-79*/
    1,1,1,1,1,1,1,1,1,1, /*80-89*/
    1,1,1,1,1,1,1,1,1,1, /*90-99*/
    1,1,1,1,1,1,1,1,1,1, /*100-109*/
    1,1,1,1,1,1,1,1,1,1, /*110-119*/
    1,1,1,1,1,2,2,2,3,3, /*120-129*/
    3,4,4,4,4,5,5,5,6,6, /*130-139*/
    6,7,7,7,8,8,8,9,9,9, /*140-149*/
    10,10,10,11,11,11,11,12,12,12, /*150-159*/
    13,13,13,13,14,14,14,14,15,15, /*160-169*/
    15,15,15,15,15,15,15,15,15,15, /*170-179*/
    15,15,15,15,15,15,15,15,15,15, /*180-189*/
    15,15,15,15,15,15,15,15,15,15, /*190-199*/
    15,15,15,15,15,15,15,15,15,15, /*200-209*/
    15,15,15,15,15,15,15,15,15,15, /*210-219*/
    15,15,15,15,15,15,15,15,15,15, /*220-229*/
    15,15,15,15,15,15,15,15,15,15, /*230-239*/
    15,15,15,15,15,15,15,15,15,15, /*240-249*/
    15,15,15,15,15,15              /*250-255*/
};
/*SPR 22382 Changes End*/
/* SPR 2480 End */
/* SPR 22382 Changes Start */
static UInt8 sinrToMCSArr_g[MAC_MAX_SINR+1] = 
{
    0,0,0,0,0,0,0,0,0,0, /*0-9*/
    0,0,0,0,0,0,0,0,0,0, /*10-19*/
    0,0,0,0,0,0,0,0,0,0, /*20-29*/
    0,0,0,0,0,0,0,0,0,0, /*30-39*/
    0,0,0,0,0,0,0,0,0,0, /*40-49*/
    0,0,0,0,0,0,0,0,0,0, /*50-59*/
    0,0,0,0,0,0,0,0,0,0, /*60-69*/
    0,0,0,0,0,0,0,0,0,0, /*70-79*/
    0,0,0,0,0,0,0,0,0,0, /*80-89*/
    0,0,0,0,0,0,0,0,0,0, /*90-99*/
    0,0,0,0,0,0,0,0,0,0, /*100-109*/
    0,0,0,0,0,0,0,0,0,0, /*110-119*/
    0,0,0,1,1,1,1,2,3,3, /*120-129*/
    4,5,5,6,6,6,7,8,8,8, /*130-139*/
    9,9,10,10,11,12,13,14,15,16, /*140-149*/
    17,17,17,17,17,18,18,19,19,20, /*150-159*/
    21,22,23,24,25,26,27,27,27,27, /*160-169*/
    27,28,28,28,28,28,28,28,28,28, /*170-179*/
    28,28,28,28,28,28,28,28,28,28, /*180-189*/
    28,28,28,28,28,28,28,28,28,28, /*190-199*/
    28,28,28,28,28,28,28,28,28,28, /*200-209*/
    28,28,28,28,28,28,28,28,28,28, /*210-219*/
    28,28,28,28,28,28,28,28,28,28, /*220-229*/
    28,28,28,28,28,28,28,28,28,28, /*230-239*/
    28,28,28,28,28,28,28,28,28,28, /*240-249*/
    28,28,28,28,28,28              /*250-255*/
};

/* SPR 22382 Changes End */


/* The index of array represents the BLER of the UE(in the range 0 to 100%). 
 * The value of correction factor represents the quantity by which correction 
 * should be applied to MCS based on the formula 
 * correctionFactor=(MAC_TARGET_BLER-uePuschBler)*MAC_BLER_CORRECTION_PARAM_ALPHA
 */
/* SPR 3269 changes start */
/* SPR 7981 changes start */
/*CA Changes start  */
SInt16 blerCorrectionFactor_g[MAX_NUM_CELL][MAC_BLER_CORRECTION_FACTOR_TABLE_SIZE];
/*CA Changes end  */
/* SPR 7981 changes end */
/* SPR 3269 changes end */

/* Default grouping by modulation scheme. Also see function initULCSIManager()
 * its usage and significance.
 */
MCSGroupingTable mcsGroupingTable_g[MAX_MCS_INDEX] =
{
    /*previousMCS, nextMCS, mcsForGroup*/
    /*QPSK*/
    {INVALID_MCS_INDEX, 11, 0}, {INVALID_MCS_INDEX, 11, 0}, /* 0, 1*/
    {INVALID_MCS_INDEX, 11, 0}, {INVALID_MCS_INDEX, 11, 0}, /* 2, 3*/
    {INVALID_MCS_INDEX, 11, 0}, {INVALID_MCS_INDEX, 11, 0}, /* 4, 5*/
    {INVALID_MCS_INDEX, 11, 0}, {INVALID_MCS_INDEX, 11, 0}, /* 6, 7*/
    {INVALID_MCS_INDEX, 11, 0}, {INVALID_MCS_INDEX, 11, 0}, /* 8, 9*/
    {INVALID_MCS_INDEX, 11, 0},                             /*10*/
    /*16QAM*/
    {  0, 21,11},                                           /*11*/
    {  0, 21,11},               {  0, 21,11},               /*12,13*/
    {  0, 21,11},               {  0, 21,11},               /*14,15*/
    {  0, 21,11},               {  0, 21,11},               /*16,17*/
    {  0, 21,11},               {  0, 21,11},               /*18,19*/
    {  0, 21,11},                                           /*20*/
    /*64QAM*/
    {11, INVALID_MCS_INDEX,21},                             /*21*/
    {11, INVALID_MCS_INDEX,21}, { 11,INVALID_MCS_INDEX,21}, /*22,23*/
    {11, INVALID_MCS_INDEX,21}, { 11,INVALID_MCS_INDEX,21}, /*24,25*/
    {11, INVALID_MCS_INDEX,21}, { 11,INVALID_MCS_INDEX,21}, /*26,27*/
    {11, INVALID_MCS_INDEX,21}                              /*28*/
}; 

/* As now direct mapping of SINR and UL MCS will be used , so 
 * removing the table of CQI to UL MCS mapping */
/* +- SPR 22382 Changes */

/* Time Averaging chages start */
UInt8 mcsSINRArr_g[MAX_MCS_INDEX_ARRAY] = {
    122, /*0 */
    127, /*1 */
    127, /*2 */
    130, /*3 */
    130, /*4 */
    130, /*5 */
    136, /*6 */
    136, /*7 */
    141, /*8 */
    141, /*9 */
    146, /*10 */
    146, /*11 */
    146, /*12 */
    146, /*13 */
    149, /*14 */
    149, /*15 */
    149, /*16 */
    156, /*17 */
    156, /*18 */
    160, /*19 */
    160, /*20 */
    160, /*21 */
    163, /*22 */
    163, /*23 */
    166, /*24 */
    166, /*25 */
    166, /*26 */
    170, /*27 */
    MAC_MAX_PUSCH_EFFECTIVE_SINR}; /*28 */
/* Time Averaging chages end */

/*SINR reports received from L1 will be stored in the queue sinrReportQueue_g*/
/*CA Changes start  */
SINR_REPORT_TYPE* sinrReportQueue_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */

#define FREQUENCY_AVERAGE(srsSINR_p,idxSRS,srsSubBandGroupSize_g,freqAveragedSinr) \
{ \
    freqAveragedSinr=srsSINR_p[idxSRS + (srsSubBandGroupSize>>1)];\
}

/*****************************************************************************/
/* Cyclomatic Complexity Changes -starts here*/
static  void updatePrbMapForLowerReportedMcs(MCSToPrbMapInfo *mcsToPrbMap_p,
		UInt8 *lowestReportedMCS_p,
                UInt8 inputMCS,
		UInt8 numOfPRB,
		UInt8 lastIndex,
		UInt8 start
		);

static  void updatePrbMapForHigherReportedMcs(MCSToPrbMapInfo *mcsToPrbMap_p,
                                      UInt8 *highestReportedMCS_p,
                                      UInt8 inputMCS,
                                      UInt8 lastIndex,
                                      UInt8 start,
                                      UInt8 end);

/* Cyclomatic Complexity Changes -ends here*/
/******************************************************************************
 * Function Name : getCQIFromSINR 
 * Inputs        : sinr - Input sinr
 * Outputs       : None
 * Returns       : CQI corresponding to the received SINR.
 * Description   :This function maps the input SINR to CQI based on sinrToCQI_g
 *****************************************************************************/
 UInt8 getCQIFromSINR(UInt8 sinr)
{
    /* SPR 2480 Start */
    return sinrToCQI_g[sinr];
    /* SPR 2480 End */
}

/* SPR 22382 Changes Start */
/******************************************************************************
 * Function Name : getULMCSFromSINR 
 * Inputs        : sinr - Input sinr
 * Outputs       : None
 * Returns       : MCS corresponding to the received SINR.
 * Description   :This function maps the input SINR to MCS based on sinrToMCSArr_g
 *****************************************************************************/

 UInt8 getULMCSFromSINR(UInt8 sinr)
{
    return sinrToMCSArr_g[sinr]; 
}
/* SPR 22382 Changes End */


/* +- SPR 22382 Changes */
/******************************************************************************
 * Function Name : getEffectiveMCS 
 * Inputs        : sinr - SINR to be converted into MCS
 *                 ueCategory - UE Category
 *                 sinrCorrectionFactor
 *                 ueIndex
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : Effective MCS as below.
 * Description   :This function converts the received SINR into MCS by first 
 *                converting into to CQI and then to MCS. It assumes
 *                that the sinr passed to it has already taken BLER correction
 *                into account. Since UE cat 1,2,3,4 do not support 64 QAM in 
 *                UL, so when same is passed, it restricts the maximum MCS to 20
 *                Equivalent MCS for the group is looked up and returned.
 *****************************************************************************/
/* SPR 3271 changes start */
 UInt8 getEffectiveMCS(UInt8 sinr, UInt8 ueCategory,
                                         SInt32 sinrCorrectionFactor,
                                         /* +- SPR 18268 */
                                         UInt16 ueIndex,
                                         /* +- SPR 18268 */
                                         /*CA Changes start  */
                                         InternalCellIndex internalCellIndex)
                                         /*CA Changes end  */
/* SPR 3271 changes end */
{
    /*SPR_4001_CHANGES_START*/
    SInt32 effectiveSINR = 0;
    /* Bler changes start */ 
    /* Using SINR Correction Factor, calculate effective SINR*/
/* SPR 7981 changes start */
    effectiveSINR = (SInt32)sinr + (sinrCorrectionFactor / UL_RESOLUTION_FACTOR);
/* SPR 7981 changes end */
    /* Bler changes end */
    if(effectiveSINR > MAC_MAX_PUSCH_EFFECTIVE_SINR)
    {
        /* SPR 13965 fix start */
        if(sinr < MAC_MAX_PUSCH_EFFECTIVE_SINR)
        {
           /* SPR 13965 fix end */
            effectiveSINR = sinr;
        }
        else
        {
            effectiveSINR = MAC_MAX_PUSCH_EFFECTIVE_SINR;
        }
    }
    else if (effectiveSINR < 0)
    {
        effectiveSINR = 0;
    }
/*NEW_LOG_REQ*/
    
#ifdef LOG_PRINT_ENABLED
    LOG_MAC_MSG(MAC_EFFECTIVE_SINR_MCS_ID,LOGDEBUG,MAC_CQI,
            getCurrentTick(),ulUECtxInfoArr_g[ueIndex].ulUEContext_p->ueIndex,
            ulUECtxInfoArr_g[ueIndex].ulUEContext_p->dlWidebandCQICodeWordOne,
            ulUECtxInfoArr_g[ueIndex].ulUEContext_p->dlWidebandCQICodeWordTwo,effectiveSINR,
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            __func__,"EFFECTIVE_SINR");
#endif
    /* SPR 22382 Changes Start */
    UInt8 mcs = getULMCSFromSINR(effectiveSINR);
    /* SPR 22382 Changes End */

    /*SPR 3600 Start*/
    /*Check whether 64QAM is enables from OAM*/
    /*CA Changes start  */
    if ((LTE_TRUE == 
             cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->macEnable64QAM) && 
        ((UE_CATEGORY_5 == ueCategory)|| (UE_CATEGORY_8 == ueCategory)))
    {
        /* Both eNB and UE supports 64QAM, so selected MCS can be used */
    }
    else
    {
        /*Minimum of maxMcs16QAM OR effective calculated mcs*/
        if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->maxMcs16QAM > mcs)
        {
            /* MCS is less than the maximum configured value for Uplink MCS in 16 QAM */
        }
        else
        {
            mcs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->maxMcs16QAM;
        }
        /* SPR 19136 Fix Start */
        if((UE_CATEGORY_1 == ueCategory) && (mcs > MCS_INDEX_64QAM_20))
        {
            mcs = MCS_INDEX_64QAM_20; 
        }
        /* SPR 19136 Fix End */
    }
    /*CA Changes end  */
    /*SPR 3600 End*/
    return  mcsGroupingTable_g[mcs].mcsForGroup;
}


/******************************************************************************
 * Function Name : calculateAverageSINRFromSNRReportPerPRB
 * Inputs        : srsSINR_p - Pointer to SRS SINR info 
 *                 startPRB - start index of PRB
 *                 numPRB - Number of contiguous PRBs starting from 
 *                   startPRB for which input MCS is received 
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : Sum average SINR of the reported PRBs
 * Description   : This function calculates the average SINR on the reported 
 *                 PRBs. 
 ****************************************************************************/
static  UInt32 calculateAverageSINRFromSNRReportPerPRB (UInt8 *srsSINR_p,
        UInt8 numPRB,
        UInt8 startPRB,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex) 
        /*CA Changes end  */
{
    
    UInt32 avgSINR = 0;
    UInt8 startPRBIndex = 0;
    /* ICIC changes start */
    UInt8 usefulPRB = 0;
    /*CA Changes start  */
    ULResourceAllocationInfo *ulRAInfo = ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex];
    /*CA Changes end  */

    while (startPRBIndex < numPRB)
    {
        if(BLOCKED_REGION != ulRAInfo->rbMapRegion[startPRB  + startPRBIndex].regType)
        {
            avgSINR += srsSINR_p[startPRBIndex++];
            usefulPRB++;
        }
        else
        {
            startPRBIndex += 
                ulRAInfo->rbMapRegion[startPRBIndex + startPRB].blockedCount;
        }
    }

    if(0 == usefulPRB )
    {
        return 0;
    }
    else
    {
        return (avgSINR/usefulPRB);
    }
    /* ICIC changes end */
}

/******************************************************************************
 * Function Name : calculateAverageSINR
 * Inputs        : deltaAverage, 
 *                 lastAvgSINR 
 * Outputs       : None
 * Returns       : Average SINR based on IIR algorithm
 * Description   : Using IIR algorithm, this function calculates the time 
 *                 average SINR.
 *****************************************************************************/
/*Coverity 73519 Fix Start*/
 UInt8 calculateAverageSINR (UInt32 deltaAverage, \
                                          UInt8 lastAvgSINR)
/*Coverity 73519 Fix End*/
{
  /*SPR 3856 Fix Start*/
  return (MAC_ALPHA_PUSCH_COEFFICIENT_IIR*lastAvgSINR + \
          (1024 - MAC_ALPHA_PUSCH_COEFFICIENT_IIR)*deltaAverage)/1024;
  /*SPR 3856 Fix End*/
}

/* Time Averaging changes start */
/******************************************************************************
 * Function Name : calculateAverageSINRULGrant
 * Inputs        : deltaAverage, 
 *                 lastAvgSINR, 
 *                 tpcValue 
 * Outputs       : None
 * Returns       : Average SINR based on IIR algorithm
 * Description   : Using IIR algorithm, this function calculates the time 
 *                 average SINR.
 *****************************************************************************/
 UInt8 calculateAverageSINRULGrant (UInt32 deltaAverage, \
                                          /*General Comment */ 
                                          UInt8 lastAvgSINR,
                                          SInt8 tpcValue)
{
  /*SPR 3856 Fix Start*/
  return (MAC_ALPHA_PUSCH_COEFFICIENT_IIR*(lastAvgSINR + tpcValue) + \
          (1024 - MAC_ALPHA_PUSCH_COEFFICIENT_IIR)*deltaAverage)/1024;
  /*SPR 3856 Fix End*/
}
/* Time Averaging changes end */

/******************************************************************************
 * Function Name : updateMCSToPrbMapping 
 * Inputs        : inputMCS - Reported UL MCS
 *                 startPRB - starting PRB for which inputMCS is received
 *                 numOfPRB - Number of contiguous PRBs starting from 
 *                   startPRB for which input MCS is received 
 *                 lastIndex - Last PRB for which SINR report can be
 *                   received.
 *                 highestReportedMCS_p - Highest reported UL MCS 
 *                 lowestReportedMCS_p - Lowest reported UL MCS
 *                 mcsToPrbMap_p - Pointer to UE's MCS to PRB 
 *                   Map
 *                 currentTTI - Current global tick
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : None
 * Description   : This function shall be called only when FSS is ON.
 *                 This function will update the MCS to PRB set map
 *                 (mcsToPrbMap_p) for the inputMCS with startPrb and numOfPrb,
 *                 Based on input MCS, highest and lowest reported MCS may also
 *                 be updated.
 *****************************************************************************/
static void updateMCSToPrbMapping (
        UInt8 inputMCS, 
        UInt8 startPRB, 
        UInt8 numOfPRB, 
        UInt8 lastIndex,
        UInt8 *highestReportedMCS_p, 
        UInt8 *lowestReportedMCS_p, 
        MCSToPrbMapInfo *mcsToPrbMap_p,
        /* SPR 15909 fix start */
        tickType_t currentTTI,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
        /*CA Changes end  */
{
    UInt8 start = startPRB;
    UInt8 end = start + numOfPRB - 1;
    UInt8 mcsIdx = 0;

    /*CA Changes start  */
    if(end >= cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs)
    {
        end = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs - 1;
        numOfPRB = end - start + 1;
    }
    /*CA Changes end  */


    /* The loop below updates the PRB mapping of all the MCSs from inputMCS 
     * to the lowest reported MCSs
     */
    /* Cyclomatic Complexity Changes - starts here*/
    updatePrbMapForLowerReportedMcs( mcsToPrbMap_p,
            lowestReportedMCS_p,
            inputMCS,
            numOfPRB,
            lastIndex,
            startPRB);

    updatePrbMapForHigherReportedMcs( mcsToPrbMap_p,
            highestReportedMCS_p,
            inputMCS,
            lastIndex,
            startPRB,
            end);
    /* Cyclomatic Complexity Changes - ends here*/

    /*********************************************************************/
    /* highestReportedMCS_p and lowestReportedMCS_p updation */
    /*********************************************************************/
    if(inputMCS > *highestReportedMCS_p)
    {
        *highestReportedMCS_p = inputMCS;
        /*No need to update lowest reported*/
    }
    else 
    {
        if(inputMCS < *lowestReportedMCS_p)
        {
            *lowestReportedMCS_p = inputMCS;
        }

        /* It may happen that MCS represented by current highestReportedMCS_p 
         * no more has any reported PRBs. In that case, loop from current
         * highest to lowest reported MCS and find first non-zero MCS that has
         * atleast some PRBs reported.
         * This value will be the new highestReportedMCS.
         */
        for(mcsIdx = *highestReportedMCS_p; 
                (mcsIdx >= inputMCS) && (mcsIdx != INVALID_MCS_INDEX); 
                mcsIdx = mcsGroupingTable_g[mcsIdx].previousMCS)
        {
            if(0 != mcsToPrbMap_p[mcsIdx].maxContiguous)
            {
                break;
            }
        }

        if(INVALID_MCS_INDEX != mcsIdx)
        {
            *highestReportedMCS_p = mcsIdx;
        }
        else
        {
            *highestReportedMCS_p = 0;
        }
    }

    LOG_MAC_MSG(MAC_UL_CSI_UPDATE_MCS_TO_PRB_MAP_ID, LOGDEBUG, MAC_UL_CSI,\
            currentTTI, \
            startPRB, numOfPRB, inputMCS, *highestReportedMCS_p, \
            *lowestReportedMCS_p, \
            0, 0, \
            __func__,"UPDATE_MCS_TO_PRB_MAP");
}


/* Bler changes start */ 
/******************************************************************************
 * Function Name : updateCSIMapsForSRSSinr 
 * Inputs        : ueCategory - UE category(0 -5)
 *                 startPRB - starting PRB for which inputMCS is received
 *                 numOfPRB - Number of contiguous PRBs starting from 
 *                   startPRB for which input MCS is received
 *                 srsSINR_p - Pointer holding SINR information of PRBs
 *                 currentTTI - Current global tick
 *                 ueIndex  - Index to UE's UL Context
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : csiContext_p - Pointert to CSI maps to be populated for SRS SINR 
 * Returns       : None
 * Description   : This function will update the CSI maps on SINR received in the SRS 
 *                 report . It calculates average SINR, 
 *                 average MCS, updates the PRB to MCS mapping and MCS to PRB 
 *                 mapping, highest reported and lowest reported MCS. 
 *****************************************************************************/
void updateCSIMapsForSRSSinr(
    CSIContext *csiContext_p,
    UInt8 ueCategory,
    UInt8 startPRB,
    UInt8 numOfPRB,
    UInt8 *srsSINR_p,
    /* SPR 15909 fix start */
    tickType_t currentTTI,
    /* SPR 15909 fix end */
    /* +- 18268 */ 
    UInt16 ueIndex,
    /* +- 18268 */ 
    /*CA Changes start  */
    InternalCellIndex internalCellIndex)
    /*CA Changes end  */
/* Bler changes end */ 
{
    UInt8 endPRB = startPRB + numOfPRB - 1;
    UInt8 i = 0;

    /*ICIC changes start*/
    /*CA Changes start  */
    UInt8 srsSubBandGroupSize = \
        (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->srsSubBandGroupSize;
    PRBGroupConfig *prbToSINRGroupIndexMap = 
        (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->prbToSINRGroupIndexMap;
    /*CA Changes end  */
    UInt8 normalizedStart = prbToSINRGroupIndexMap[startPRB].groupIndex * srsSubBandGroupSize;
    UInt8 normalizedEnd = 0;
    UInt8 localSinr = 0;
    UInt8 effectiveMCS = 0;
    UInt32 currentMCS = 0;
    UInt8 localStartPRB = normalizedStart;
    UInt8 localNumOfPRB = srsSubBandGroupSize;
    UInt8 sinrToBeUpdated = 0;
    UInt8 freqAveragedSinr = 0;
    UInt8 idxSRS = 0;

    /*CA Changes start  */
    if(endPRB >= cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs)
    /*CA Changes end  */
    {
        /*CA Changes start  */
        endPRB = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs - 1;
        /*CA Changes end  */
    }

    normalizedEnd = prbToSINRGroupIndexMap[endPRB].groupIndex * srsSubBandGroupSize;
    /* SPR 18122 Changes Start*/
    if(endPRB < normalizedEnd + (DIVIDE_BY_TWO(srsSubBandGroupSize)))
        /* SPR 18122 Changes End*/
    {
        if(0 == normalizedEnd)
        {
            return;
        }
        else
        {
            normalizedEnd -= srsSubBandGroupSize;
        }
        if(normalizedEnd < normalizedStart)
        {
            return;
        }
    }

    FREQUENCY_AVERAGE(srsSINR_p,idxSRS,srsSubBandGroupSize,freqAveragedSinr);/*??*/
    /* Bler changes start */ 
    localSinr = calculateAverageSINR(\
                   freqAveragedSinr,/*SPR_4001_CHANGE*/\
                   csiContext_p->prbToMCSMapArray[prbToSINRGroupIndexMap[startPRB].groupIndex].reportedSINR/*old*/);

    csiContext_p->prbToMCSMapArray[prbToSINRGroupIndexMap[normalizedStart].groupIndex].reportedSINR = localSinr;

    effectiveMCS = getEffectiveMCS(localSinr,ueCategory, csiContext_p->\
                                   puschBlerInfo.sinrCorrectionFactor,ueIndex,
                                   /*CA Changes start  */
                                   internalCellIndex);
                                   /*CA Changes end  */
    
    currentMCS = effectiveMCS;
    
    localStartPRB = normalizedStart;

    for(i = normalizedStart + srsSubBandGroupSize, idxSRS = srsSubBandGroupSize; 
            i <= normalizedEnd; i += srsSubBandGroupSize, idxSRS += srsSubBandGroupSize)
    {
        FREQUENCY_AVERAGE(srsSINR_p,idxSRS,srsSubBandGroupSize,freqAveragedSinr);

        sinrToBeUpdated = calculateAverageSINR(freqAveragedSinr,/*SPR_4001_CHANGES*/\
                              csiContext_p->prbToMCSMapArray[prbToSINRGroupIndexMap[i].groupIndex].reportedSINR);

        csiContext_p->prbToMCSMapArray[prbToSINRGroupIndexMap[i].groupIndex].reportedSINR = sinrToBeUpdated;

        if(localSinr == sinrToBeUpdated)
        {
            localNumOfPRB += srsSubBandGroupSize;
        }
        else
        {
            currentMCS = getEffectiveMCS(sinrToBeUpdated,ueCategory,
                                     csiContext_p->puschBlerInfo.\
                                     sinrCorrectionFactor,ueIndex,/*SPR_4001_CHANGES*/
                                     /*CA Changes start  */
                                     internalCellIndex);
                                     /*CA Changes end  */

            localSinr = sinrToBeUpdated;

            /* Maintain set of contiguous MCS reports. Whenever the MCS
             * value is different from the previous report, call
             * updateMCSToPrbMapping() with the set and start updating 
             * a new set.
             */
            if(effectiveMCS == currentMCS)
            {
                localNumOfPRB += srsSubBandGroupSize;
            }
            else
            {
                updateMCSToPrbMapping(effectiveMCS, localStartPRB, \
                        localNumOfPRB,\
                        /*CA Changes start  */
                        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs - 1,  \
                        /*CA Changes end  */
                        &csiContext_p->highestReportedMCS,\
                        &csiContext_p->lowestReportedMCS,\
                        csiContext_p->mcsToPrbMap_p,
                        currentTTI,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */

                localStartPRB = i;
                localNumOfPRB = srsSubBandGroupSize;
                effectiveMCS = currentMCS;
            }/*effectiveMCS != currentMCS*/
        }/*localSinr != sinrToBeUpdated*/
    }/*end for*/

    updateMCSToPrbMapping(effectiveMCS, localStartPRB, \
            localNumOfPRB,\
            /*CA Changes start  */
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs - 1,  \
            /*CA Changes end  */
            &csiContext_p->highestReportedMCS,\
            &csiContext_p->lowestReportedMCS,\
            csiContext_p->mcsToPrbMap_p,
            currentTTI,
            /*CA Changes start  */
            internalCellIndex);
            /*CA Changes end  */

}



/******************************************************************************
 * Function Name : processSINRForSRSReport 
 * Inputs        : ueIndex - Index to UE's UL Context
 *                 startPRB: starting PRB for which SINR report is 
 *                  received in SRS indication 
 *                 numOfPRB - Number of PRBs from which the SINR report
 *                  is received in SRS indication
 *                 srsSINR_p - Pointer holding SINR information of PRBs
 *                 recvTTI -
 *                 currentTTI - Current global tick
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Description   : This function will process the SINR received in the SRS 
 *                 report and update CSI. It calculates average SINR, 
 *                 average MCS, updates the PRB to MCS mapping and MCS to PRB 
 *                 mapping, highest reported and lowest reported MCS. 
 *****************************************************************************/
/* +- 18268 */ 
static void  processSINRForSRSReport (UInt16 ueIndex, 
/* +- 18268 */ 
        UInt8 startPRB, 
        UInt8 numOfPRB,  
        UInt8 *srsSINR_p,
        /* SPR 15909 fix start */
	    tickType_t recvTTI,
        tickType_t currentTTI,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
        /*CA Changes end  */
{
    UInt8 endPRB = startPRB + numOfPRB - 1;    
    ULUEContext *ulUEContext_p   = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    if(0 == numOfPRB)
    {
        return;
    }
   
    /* In error cases, SINR report indexes may be such that endPRB exceeds 
     * ulAvailableRBs. The check below restricts the endPRB to maximum 
     * configured UL RB in the cell.
     */
    /*CA Changes start  */
    if(endPRB >= cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs)
    /*CA Changes end  */
    {
        /*CA Changes start  */
        endPRB = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs - 1;
        /*CA Changes end  */
        numOfPRB = endPRB - startPRB + 1;
    }
    
    /* SINR_REPORT_TYPE_SRS Fix Start */
    if((!ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && (PNULL != ulUEContext_p))
    /* SINR_REPORT_TYPE_SRS Fix End */
    {
        CSIContext *csiContext_p = &ulUEContext_p->csiContext;
        UInt8 ueCategory   = ulUEContext_p->ueCategory;
        /*SPR_4001_CHANGE_START*/
        UInt32 avgSINRFromSRSReport = 
            /*CA Changes start  */
            calculateAverageSINRFromSNRReportPerPRB(srsSINR_p, numOfPRB,startPRB, internalCellIndex);
            /*CA Changes end  */
        /*SPR_4001_CHANGE_END*/
      
        /* ICIC changes start */
        if(avgSINRFromSRSReport > 0)
        {
            LOG_UT(MAC_AVG_SINR_UPDATE,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    ulUEContext_p->ueIndex,\
                    ulUEContext_p->userLocationType,\
                    avgSINRFromSRSReport,\
                    0,0,\
                    0.0,0.0,\
                    __func__,"MAC_AVG_SINR_UPDATE");
            
            /*Effective SINR Calculation Code is removed From here Under SPR 4001 Changes*/


            /* Time Averaging Changes Start */
            if ((ulUEContext_p->powerControlPuschTPCValue) && \
                    (recvTTI >= ulUEContext_p->tpcInfoForPusch.lastAppliedTick))
            {
                csiContext_p->averageSINR = calculateAverageSINRULGrant(avgSINRFromSRSReport/*SPR_4001_CHANGES*/, \
                        csiContext_p->averageSINR, \
                        ulUEContext_p->powerControlPuschTPCValue);
                /*ICIC change start */
                /*CA Changes start  */
                updateULSINR(ulUEContext_p->ueIndex,csiContext_p->averageSINR, internalCellIndex);
                /*CA Changes end  */
                /*ICIC change end */
                ulUEContext_p->powerControlPuschTPCValue = 0;
            }
            else
            {
                csiContext_p->averageSINR = calculateAverageSINR(avgSINRFromSRSReport/*SPR_4001_CHANGES*/, \
                        csiContext_p->averageSINR);
                /*ICIC change start */
                /*CA Changes start  */
                updateULSINR(ulUEContext_p->ueIndex,csiContext_p->averageSINR, internalCellIndex);
                /*CA Changes end  */
                /*ICIC change end */

            }
            /* Time Averaging Changes End */

            ulUEContext_p->mcsIndex = getEffectiveMCS(csiContext_p->averageSINR,\
                    ueCategory,csiContext_p->puschBlerInfo.\
                    sinrCorrectionFactor,ueIndex,/*SPR_4001_CHANGES*/
                    /*CA Changes start  */
                    internalCellIndex) ;
                    /*CA Changes end  */
            /* SPS_CHG */
                if(( SPS_IDLE != ulUEContext_p->spsUlInfo.spsState   )&&
                    (INVALID_STATE != ulUEContext_p->spsUlInfo.spsState))               
                    {
                        UInt16 ulSpsInterval          = 0;
                        UInt8  mcsIndex = 0;
                        LP_SpsULIntervalProfile   ulSpsIntervalProfile_p = PNULL;
                        ulSpsInterval = ulUEContext_p->spsUlInfo.spsUlSetupInfo_p->\
                                        semiPersistentSchedIntervalUL;
                        ulSpsIntervalProfile_p = 
                            /*CA Changes start  */
                            &cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig. \
                             spsUlIntervalProfile[ulSpsInterval] ;
                            /*CA Changes end  */
                        mcsIndex = ulUEContext_p->mcsIndex ;
                        if(mcsIndex < (ulSpsIntervalProfile_p->ulMinMcsSps + ulSpsIntervalProfile_p->ulMcsMargin))
                        { 
                           ulUEContext_p->spsUlInfo.isUeUlWbMcsBelowThresh = LTE_TRUE ;
                        }
                    }
            /* SPS_CHG */

            /* Cyclomatic_complexity_changes_start */
            getUlModSchemeFromMcsIndex(ulUEContext_p->mcsIndex, \
                    &(ulUEContext_p->modulationScheme),\
                    /*CA Changes start  */
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->macEnable64QAM,\
                    /*CA Changes end  */
                    ulUEContext_p->ueCategory);
            /* Cyclomatic_complexity_changes_end */

            CHECK_TPC_FOR_WIDEBAND_SINR(ulUEContext_p,\
                    ulUEContext_p->pathLoss,\
                    csiContext_p->averageSINR,\
                    recvTTI, \
                    /*CA Changes start  */ \
                    internalCellIndex \
                    /*CA Changes end  */ \
                    );

        }
        /* ICIC changes end */
        /*CA Changes start  */
        if(FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->freqSelectiveSchUL)
        /*CA Changes end  */
        {
            updateCSIMapsForSRSSinr(
                    csiContext_p,
                    ueCategory,
                    startPRB,
                    numOfPRB,
                    srsSINR_p,
                    currentTTI,ueIndex,
                    /*CA Changes start  */
                    internalCellIndex);
                    /*CA Changes end  */
        }/*if(FREQUENCY_SELECTIVE_ON == cellConfig_g.freqSelectiveSchUL)*/
        /*CA Changes start  */
        else if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->powerControlEnableInfo.clpcPuschfreqSelectiveEnable)
        /*CA Changes end  */
        {
            /* CLPC-subband - Update PRB to MCS Array*/
        }

        
        LOG_UT(MAC_UL_CSI_PROCESS_SRS_SINR_ID, LOGDEBUG, MAC_UL_CSI,\
                currentTTI, \
                startPRB, numOfPRB, avgSINRFromSRSReport, \
                csiContext_p->averageSINR, \
                ulUEContext_p->mcsIndex, 
                0, 0, \
                __func__,"PROCESS_SRS_SINR");

    }/*if(ulUEContext_p)*/
}

/******************************************************************************
 * Function Name : updateCSIMapsForULSCHGrant 
 * Inputs        : ueCategory - UE category(0 -5)
 *                 sinrReported -
 *                 startPRB - starting PRB for which inputMCS is received
 *                 numOfPRB - Number of contiguous PRBs starting from 
 *                   startPRB for which input MCS is received
 *                 currentTTI - Current global tick
 *                 ueIndex  - Index to UE's UL Context
 *                 internalCellIndex - CellIndex at MAC
 * Outputs       : csiContext_p - Pointert to CSI maps to be populated for SRS SINR 
 * Returns       : None
 * Description   : This function will update the CSI maps in ULSCH Grant on SINR received
 *                 It calculates average SINR,average MCS, updates the PRB to 
 *                 MCS mapping and MCS to PRB mapping, highest reported and 
 *                 lowest reported MCS. 
 *****************************************************************************/
void updateCSIMapsForULSCHGrant(
    CSIContext *csiContext_p,
    UInt8 ueCategory,
    UInt8 sinrReported,/*SPR_4001_CHANGE*/
    UInt8 startPRB,
    UInt8 numOfPRB,
    /* SPR 15909 fix start */
    tickType_t currentTTI,
    /* SPR 15909 fix end */
    /* +- 18268 */ 
    UInt16 ueIndex,
    /* +- 18268 */ 
    /*CA Changes start  */
    InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    UInt8 endPRB = startPRB + numOfPRB - 1;
    UInt8 i = 0;
    /*ICIC changes start */
    /*CA Changes start  */
    UInt8 srsSubBandGroupSize = 
        (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->srsSubBandGroupSize;
    PRBGroupConfig *prbToSINRGroupIndexMap = 
        (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->prbToSINRGroupIndexMap;
    /*CA Changes end  */
        /*ICIC changes end*/
    UInt8 normalizedStart = prbToSINRGroupIndexMap[startPRB].groupIndex * srsSubBandGroupSize;
    UInt8 normalizedEnd = 0;
    UInt8 localSinr = 0;
    UInt8 effectiveMCS = 0;
    UInt32 currentMCS = 0;
    UInt8 localStartPRB = normalizedStart;
    UInt8 localNumOfPRB = srsSubBandGroupSize;
    UInt8 sinrToBeUpdated = 0;

    /*CA Changes start  */
    if(endPRB >= cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs)
    {
        endPRB = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs - 1;
    }
    /*CA Changes end  */

    normalizedEnd = prbToSINRGroupIndexMap[endPRB].groupIndex * srsSubBandGroupSize;

    localSinr = calculateAverageSINR(\
                   sinrReported,/*SPR_4001_CHANGE*/\
                   csiContext_p->prbToMCSMapArray[prbToSINRGroupIndexMap[startPRB].groupIndex].reportedSINR/*old*/);

    csiContext_p->prbToMCSMapArray[prbToSINRGroupIndexMap[normalizedStart].groupIndex].reportedSINR = localSinr;

    effectiveMCS = getEffectiveMCS(localSinr,ueCategory,csiContext_p->puschBlerInfo.\
                                   sinrCorrectionFactor,ueIndex,/*SPR_4001_CHANGES*/ \
                                   /*CA Changes start  */
                                   internalCellIndex);
                                   /*CA Changes end  */
    /*Coverity 73407 Fix Start*/ 
    /*currentMCS = effectiveMCS;*/
    /*Coverity 73407 Fix End*/ 
    
    localStartPRB = normalizedStart;

    for(i = normalizedStart + srsSubBandGroupSize; 
            i <= normalizedEnd; i += srsSubBandGroupSize)
    {
        sinrToBeUpdated = calculateAverageSINR(sinrReported,/*SPR_4001_CHANGE*/\
                              csiContext_p->prbToMCSMapArray[prbToSINRGroupIndexMap[i].groupIndex].reportedSINR);

        csiContext_p->prbToMCSMapArray[prbToSINRGroupIndexMap[normalizedStart].groupIndex].reportedSINR = sinrToBeUpdated;

        if(localSinr == sinrToBeUpdated)
        {
            localNumOfPRB += srsSubBandGroupSize;
        }
        else
        {
            currentMCS = getEffectiveMCS(sinrToBeUpdated,ueCategory, csiContext_p->puschBlerInfo.\
                                         sinrCorrectionFactor,ueIndex,/*SPR_4001_CHANGES*/ \
                                         /*CA Changes start  */
                                         internalCellIndex);
                                         /*CA Changes end  */

            localSinr = sinrToBeUpdated;

            /* Maintain set of contiguous MCS reports. Whenever the MCS
             * value is different from the previous report, call
             * updateMCSToPrbMapping() with the set and start updating 
             * a new set.
             */
            if(effectiveMCS == currentMCS)
            {
                localNumOfPRB += srsSubBandGroupSize;
            }
            else
            {
                updateMCSToPrbMapping(effectiveMCS, localStartPRB, \
                        localNumOfPRB,\
                        /*CA Changes start  */
                        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs - 1,  \
                        /*CA Changes end  */
                        &csiContext_p->highestReportedMCS,\
                        &csiContext_p->lowestReportedMCS,\
                        csiContext_p->mcsToPrbMap_p,
                        currentTTI,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */

                localStartPRB = i;
                localNumOfPRB = srsSubBandGroupSize;
                effectiveMCS = getEffectiveMCS(localSinr,ueCategory, csiContext_p->
                                               puschBlerInfo.sinrCorrectionFactor,ueIndex,
                                               /*CA Changes start  */
                                               internalCellIndex);
                                               /*CA Changes end  */

            }/*effectiveMCS != currentMCS*/
        }/*localSinr != sinrToBeUpdated*/
    }/*end for*/

    updateMCSToPrbMapping(effectiveMCS, localStartPRB, \
            localNumOfPRB,\
            /*CA Changes start  */
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs - 1,  \
            /*CA Changes end  */
            &csiContext_p->highestReportedMCS,\
            &csiContext_p->lowestReportedMCS,\
            csiContext_p->mcsToPrbMap_p,
            currentTTI,
            /*CA Changes start  */
            internalCellIndex);
            /*CA Changes end  */

}

/******************************************************************************
 * Function Name : processSINRForULSCHGrant 
 * Inputs        : ueIndex - Index to UE's UL Context
 *                 startPRB - starting PRB for which SINR report is 
 *                  received in ULSCH(obtained from HARQ information)
 *                 numOfPRB - Number of PRBs from which the SINR report
 *                  is received in ULSCH(obtained from HARQ information)
 *                 sinr - Received sinr in ULSCH indication from L1
 *                 recvTTI -
 *                 currentTTI - Current global tick
 *                 reportType - type of SINR (SRS,ULSCH)
 *                 internalCellIndex - Cell-Index used at MAc
 * Outputs       : None
 * Returns       : None
 * Description   : This function will process the SINR received in the 
 *                 ULSCH.indication and update CSI. It calculates average SINR,
 *                 average MCS, updates the PRB to MCS mapping and MCS to PRB 
 *                 mapping, highest reported and lowest reported MCS. 
 *****************************************************************************/
/* +- 18268 */ 
static void  processSINRForULSCHGrant (UInt16 ueIndex, 
/* +- 18268 */ 
        UInt8 startPRB,
        UInt8 numOfPRB,
        UInt8 sinr,
        /* SPR 15909 fix start */
/* Time Averaging changes start */
        tickType_t recvTTI,
/* Time Averaging changes end */
        tickType_t currentTTI,
        /* SPR 15909 fix end */
        /* SPR 3531 START */
        SINRReportType reportType,
        /* SPR 3531 END */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
        /*CA Changes end  */
{
    ULUEContext *ulUEContext_p   = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    /*SPR# 3537 Changes Starts*/
    CSIContext *csiContext_p = PNULL;
    BlerInfoForPUSCH *puschBlerInfo_p = PNULL;
    UInt8 ueCategory   = 0;
    /*SPR# 3537 Changes Ends */
    if(0 == numOfPRB)
    {
        return;
    }   

    /* SINR_REPORT_TYPE_ULSCH Fix Start */
    if((!ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && (PNULL != ulUEContext_p))
    /* SINR_REPORT_TYPE_ULSCH Fix End */
    {
        /*SPR# 3537 Changes Starts*/
       ueCategory   = ulUEContext_p->ueCategory;
       csiContext_p = &ulUEContext_p->csiContext;
       puschBlerInfo_p = &(csiContext_p->puschBlerInfo);
       /*SPR# 3537 Changes Ends */

        /* Calculate New BLER based on UL Ack*/
        /* SPR 3531 START */
        if (SINR_REPORT_TYPE_ULSCH == reportType) 
        {
            puschBlerInfo_p->blerCalculated -= puschBlerAckWeightage_g;
        }
        else
        {
            puschBlerInfo_p->blerCalculated += \
                                           puschBlerNackWeightage_g;
        }
        /* SPR 3531 END */

        puschBlerInfo_p->blerCalculatedCounter++;
  
        if(puschBlerInfo_p->blerCalculatedCounter >= \
               MAX_NUM_OF_PUSCH_BLER_SAMPLES)
        {
            /* SPR 3530 START */
            puschBlerInfo_p->puschBler = MAC_TARGET_BLER + \
                (SInt8)(puschBlerInfo_p->blerCalculated/MAX_NUM_OF_PUSCH_BLER_SAMPLES);
            /* SPR 3530 END */
            

            if (puschBlerInfo_p->puschBler < 1)               
            {                                                                
                puschBlerInfo_p->puschBler = 0;
            }                                                                
            else if (puschBlerInfo_p->puschBler > 100)        
            {                                                                
                puschBlerInfo_p->puschBler = 100;
            }
            
            /*ICIC change start */
            /*CA Changes start  */
            updateULBler(ueIndex,puschBlerInfo_p->puschBler, internalCellIndex);
            /*CA Changes end  */
            /*ICIC change end */
            puschBlerInfo_p->blerCalculatedCounter = 0;       
            puschBlerInfo_p->blerCalculated = 0; 

            /* Bler changes start */ 
            /* Calculate the new SINR Correction Factor when BLER is calculated */
            puschBlerInfo_p->sinrCorrectionFactor = \
                puschBlerInfo_p->sinrCorrectionFactor +  \
                /*CA Changes start  */
                blerCorrectionFactor_g[internalCellIndex][puschBlerInfo_p->puschBler];        
                /*CA Changes end  */
            VALIDATE_PUSCH_SINR_CORRECTION_VALUE(puschBlerInfo_p->sinrCorrectionFactor); 
            /* Bler changes end */ 
        }

        /* SPR 3531 START */
        if ((SINR_REPORT_TYPE_ULSCH == reportType) ||
            ((SINR_REPORT_TYPE_ULSCH_NACK == reportType) && (sinr >= MIN_SINR_FOR_TPC_CALC_IN_NACK)))
        {
        /* SPR 3531 END */
            /* Time Averaging changes start */
            /* Time averaging also ensures that TPC value is also considered with 80%.
               Previous Average SINR is treated as Effecive SINR . Rest 20% will be considered 
               with SINR Report later */


            if ((ulUEContext_p->powerControlPuschTPCValue) && \
                    (recvTTI >= ulUEContext_p->tpcInfoForPusch.lastAppliedTick))
            {  
                csiContext_p->averageSINR = calculateAverageSINRULGrant(
                        sinr,/*SPR_4001_CHANGE*/\
                        csiContext_p->averageSINR, \
                        ulUEContext_p->powerControlPuschTPCValue);
                /*ICIC change start */
                /*CA Changes start  */
                updateULSINR(ueIndex,csiContext_p->averageSINR, internalCellIndex);
                /*CA Changes end  */
                /*ICIC change end */

                ulUEContext_p->powerControlPuschTPCValue = 0;
            }
            else
            {
                csiContext_p->averageSINR = calculateAverageSINR(
                        sinr,/*SPR_4001_CHANGE*/\
                        csiContext_p->averageSINR);
                /*ICIC change start */
                /*CA Changes start  */
                updateULSINR(ueIndex,csiContext_p->averageSINR, internalCellIndex);
                /*CA Changes end  */
                /*ICIC change end */

            } 
            ulUEContext_p->mcsIndex = getEffectiveMCS(csiContext_p->averageSINR,
                             ueCategory, 
                             csiContext_p->puschBlerInfo.\
                             sinrCorrectionFactor,ueIndex,/*SPR_4001_CHANGE*/
                             /*CA Changes start  */
                             internalCellIndex);
                             /*CA Changes end  */


            /* SPS_CHG */
            if(( SPS_IDLE != ulUEContext_p->spsUlInfo.spsState   )&&
                (INVALID_STATE != ulUEContext_p->spsUlInfo.spsState))               
                {
                    UInt16 ulSpsInterval          = 0;
                    UInt8  mcsIndex = 0;
                    LP_SpsULIntervalProfile   ulSpsIntervalProfile_p = PNULL;
                    ulSpsInterval = ulUEContext_p->spsUlInfo.spsUlSetupInfo_p->\
                                    semiPersistentSchedIntervalUL;
                    ulSpsIntervalProfile_p = 
                        /*CA Changes start  */
                        &cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig.\
                        spsUlIntervalProfile[ulSpsInterval] ;
                        /*CA Changes end  */
                    mcsIndex = ulUEContext_p->mcsIndex ;
                    if(mcsIndex < (ulSpsIntervalProfile_p->ulMinMcsSps+ulSpsIntervalProfile_p->ulMcsMargin))
                    { 
                        ulUEContext_p->spsUlInfo.isUeUlWbMcsBelowThresh = LTE_TRUE ;
                    }
                }
            /* SPS_CHG */

            /*SPR 3600 Start*/
            /* Cyclomatic_complexity_changes_start */
            getUlModSchemeFromMcsIndex(ulUEContext_p->mcsIndex, \
                    &(ulUEContext_p->modulationScheme),\
                    /*CA Changes start  */
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->macEnable64QAM,\
                    /*CA Changes end  */
                    ulUEContext_p->ueCategory);
            /* Cyclomatic_complexity_changes_end */
            /*SPR 3600 End*/

            CHECK_TPC_FOR_WIDEBAND_SINR(ulUEContext_p, \
                    ulUEContext_p->pathLoss, \
                    csiContext_p->averageSINR, \
                    recvTTI, \
                    /*CA Changes start  */ \
                    internalCellIndex \
                    /*CA Changes end  */ \
                    );
            /* Time Averaging changes end */

            /* Update prbToMCSMapArray and mcsToPrbMap_p with SINR reported in 
             * ULSCH.indication only if SRS is not configured.
             */
            if (SRS_DEDICATED_CONFIG_TYPE_SETUP != \
                    ulUEContext_p->srsConfigInfo.srsConfigType)
            {
                /* Update MCS to PRB mapping only if FREQUENCY_SELECTIVE_ON*/
                /*CA Changes start  */
                if(FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->freqSelectiveSchUL)
                /*CA Changes end  */
                {
                    updateCSIMapsForULSCHGrant(
                            csiContext_p,
                            ueCategory,
                            sinr,/*SPR_4001_CHANGE*/
                            startPRB,
                            numOfPRB,
                            currentTTI,
                            ueIndex,
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */

                }
                /*CA Changes start  */
                else if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->powerControlEnableInfo. \
                        clpcPuschfreqSelectiveEnable)
                /*CA Changes end  */
                {
                    /* CLPC-subband - Update PRB to MCS Array*/
                }            
            }

        /* SPR 3531 START */
        }
        /* SPR 3531 END */
        LOG_UT(MAC_UL_CSI_PROCESS_ULSCH_SINR_ID, LOGDEBUG, MAC_UL_CSI,\
                currentTTI, \
                startPRB, numOfPRB, sinr, csiContext_p->averageSINR, \
                ulUEContext_p->mcsIndex,
                ulUEContext_p->mcsIndex , 0, \
                __func__,"PROCESS_ULSCH_SINR");

    }/*if(ulUEContext_p)*/
}

/******************************************************************************
 * Function Name : processSINRReports 
 * Inputs        : currentTTI - Current global tick
 *                 internalCellIndex - CellIndex used at MAc
 * Outputs       : None
 * Returns       : None
 * Description   : This function will dequeue the reports that were enqueued by
 *                 the receiver and depending on the type of report, it will 
 *                 call appropriate handling functions. To limit its processing
 *                 it will not process more than 
 *                 MAC_MAX_SINR_REPORTS_TO_PROCESS_PER_TTI reports per TTI.
 *****************************************************************************/
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
/*CA Changes start  */
/* SPR 15909 fix start */
void processSINRReports(tickType_t currentTTI, InternalCellIndex internalCellIndex)
#else
void processSINRReports(tickType_t currentTTI, SInt32 sinrReportQueueCount, InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */    
/*CA Changes end  */
#endif
{
    SINRReportNode *sinrReportNode_p = PNULL;
    /* Process the reports enqueued by the receiver, but do not process more 
     * than MAC_MAX_SINR_REPORTS_TO_PROCESS_PER_TTI reports per TTI.
     */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    SInt32 totalQueueCount = 0; /* TDD Warning Fix */
    SInt32 sinrReportQueueCount = 0; 
    SInt32 currentQueueCount =0;
    UInt16 sinrContainerCount ;
    for(sinrContainerCount = 0; sinrContainerCount < SINR_CONTAINER_SIZE; sinrContainerCount++)
    {
        /*CA Changes start  */
        sinrReportQueueCount  += COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + ((currentTTI - sinrContainerCount) % SINR_CONTAINER_SIZE));
    }
    /* Keep track of queue count of the queue being popped */
    currentQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + (currentTTI % SINR_CONTAINER_SIZE));
    /*CA Changes end  */
#endif
    if(sinrReportQueueCount > MAC_MAX_SINR_REPORTS_TO_PROCESS_PER_TTI)
    {
        sinrReportQueueCount = MAC_MAX_SINR_REPORTS_TO_PROCESS_PER_TTI;
    }
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    totalQueueCount = sinrReportQueueCount;
#endif
    while(sinrReportQueueCount > 0)
    {

#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
            if(sinrReportQueueCount == (totalQueueCount - currentQueueCount))
            {
                currentTTI--;
                /*CA Changes start  */
                currentQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + (currentTTI % SINR_CONTAINER_SIZE));
                /*CA Changes start  */
                totalQueueCount = sinrReportQueueCount;

            }
            /* SPR 5652 Start */
            if (!currentQueueCount)
            {
                continue;
            }
            /* SPR 5652 End */
            /*CA Changes start  */
            DEQUEUE_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + (currentTTI % SINR_CONTAINER_SIZE),
                    SINRReportNode, (void *)&sinrReportNode_p);
#else
            DEQUEUE_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex],SINRReportNode, (void *)&sinrReportNode_p);
            /*CA Changes end  */
#endif
            sinrReportQueueCount--;

            if(NULL != sinrReportNode_p)
            {
                if ( SINR_REPORT_TYPE_SRS == sinrReportNode_p->reportType )
                {
                    processSINRForSRSReport (sinrReportNode_p->ueIndex, 
                            sinrReportNode_p->startPRB, 
                            sinrReportNode_p->numOfPRB,  
                            sinrReportNode_p->sinrArray,
                            sinrReportNode_p->recvTTI,
                            currentTTI,
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */
                }
                /* SINR_REPORT_TYPE_ULSCH Fix Start */
                else if( (SINR_REPORT_TYPE_ULSCH == sinrReportNode_p->reportType) ||
                         (SINR_REPORT_TYPE_ULSCH_NACK == sinrReportNode_p->reportType))
                /* SINR_REPORT_TYPE_ULSCH Fix End */
                {
                    processSINRForULSCHGrant(sinrReportNode_p->ueIndex, 
                            sinrReportNode_p->startPRB, 
                            sinrReportNode_p->numOfPRB,  
                            sinrReportNode_p->sinrArray[0],
                            /* Time Averaging changes start */
                            sinrReportNode_p->recvTTI,
                            /* Time Averaging changes end */
                            currentTTI, 
                            /* SPR 3531 START */
                            sinrReportNode_p->reportType,
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */
                    /* SPR 3531 END */
                }

                FREE_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p);
            }/*if(NULL != sinrReportNode_p)*/
    }/*while(sinrReportQueueCount...*/
}

/******************************************************************************
 * Function Name : updatePrbToSINRGroupIndexMap
 * Inputs        : Group Size,index
 *                 internalCellIndex - Cell-Index used at MAC
 * Outputs       : None
 * Returns       : None
 * Description   : This function groups a set of PRBs of size groupSize into a
 *                 group.
 *****************************************************************************/
void updatePrbToSINRGroupIndexMap (UInt32 groupSize,UInt8 index,
                                   /*CA Changes start  */
                                   InternalCellIndex internalCellIndex)
                                   /*CA Changes end  */
{
    UInt32 quotient = (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                       cellParams_p->ulAvailableRBs + (groupSize - 1))/groupSize;
    UInt32 idx = 0;
    UInt32 tempNextGroupStartPRB = 0;
    UInt32 withinGroupLoopCount = 0;
    UInt32 tempIndex = 0;
    MacReconfigSchedulerParamsUL *schedulerParams_p=
        /*CA Changes start  */
        macReconfigSchedulerParamsUL_gp[internalCellIndex] + index;
        /*CA Changes end  */
    
    for (; idx < quotient; idx++)
    {
        /* All elements in the group shall have the same groupIndex 
         * and nextGroupStartPRB
         */
        tempNextGroupStartPRB = (idx + 1)* groupSize;
        for (withinGroupLoopCount = 0; 
             withinGroupLoopCount < groupSize; 
             withinGroupLoopCount++)
        {
            tempIndex = idx*groupSize + withinGroupLoopCount;

            schedulerParams_p->prbToSINRGroupIndexMap[tempIndex].groupIndex = idx;
            schedulerParams_p->prbToSINRGroupIndexMap[tempIndex].nextGroupStartRb = \
                tempNextGroupStartPRB;
     
        }
    }
}

/******************************************************************************
 * Function Name : initULCSIManager 
 * Inputs        : numCellsConfigured
 * Outputs       : None
 * Returns       : None
 * Description   : Initializes UL CSI related global data structures 
 *****************************************************************************/
/*CA Changes start  */
void initULCSIManager(UInt8 numCellsConfigured)
/*CA Changes end  */
{
    UInt8 idx = 0;
    UInt8 grpIdx = 0;
    UInt8 offsetIdx = 0;
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    UInt8 containerCount = 0;
#endif
#if defined DL_UL_SPLIT 
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
#endif
    UInt8 quotient=0;
    UInt8 remainder=0;    
    /* SPR 2446 Start */
    UInt8 mcsGrpStrategy = MAC_MCS_GROUPING_STRATEGY; 
    UInt8 macMcsGroupSize =  MAC_MCS_GROUP_SIZE; 
    /* Bler changes start */ 
    UInt8 uePuschBler = 0;
    /* Bler changes end */ 
    /* SPR 2446 End */
    
    /*Initializing the SINR Reports Queue*/
    createMemPool(sizeof(SINRReportNode),MAC_MAX_SINR_REPORTS_QUEUE_LENGTH);
    /*CA Changes start  */    
    CONSTRUCT_AND_INIT_SINR_REPORT_Q(numCellsConfigured);
    /*CA Changes end  */    

    /* It shall be possible to maintain MAC to PRB mapping either based on 
     * modulation schemes (grouping of MCS in to MS) or categorize multiple 
     * MCS values in to groups (based on MCS group size). 
     *
     * The lesser the group size, more processing is required to form 
     * contiguous PRB sets and more processing is required to traverse these 
     * groups during Frequency selective scheduling. However there are more 
     * chances of allocating the PRBs on the highest MCS 
     * (to increase system throughput).
     *
     * It shall be possible to define the MCS grouping strategy and algorithm
     * will be designed generic to follow the grouping For e.g.
     * In case of MS grouping, there shall be three MCS groups one for each 
     * modulation scheme  64QAM, 16QAM and QPSK. Determined MCS shall be 
     * mapped to corresponding MS and accordingly contiguous sets will be 
     * formed.
     * In case of MCS grouping, if group size is 1, there are approx 26 MCS 
     * groups and one group for each MCS. This is the least level of 
     * granularity and adds to more processing in forming the contiguous sets
     * as well as in Frequency selective scheduling algorithm 
     * 
     * In case of MCS grouping, if group size is 4, first group will consist 
     * of MCS (0,1,2,3), second group consists of MCS (4,5,6,7) and so on
     */
     
    /* Check if we need to group MCS by size*/ 
    /* SPR 2446 Start */
    if(mcsGrpStrategy == MAC_MCS_GROUP_BY_GROUP_SIZE)
    {        
        /* Check if the group size lies between 0 and MAX_MCS_INDEX */
        if((macMcsGroupSize > 0) && \
           (macMcsGroupSize <= MAX_MCS_INDEX))
        /* SPR 2446 End */
        {
            quotient = MAX_MCS_INDEX/MAC_MCS_GROUP_SIZE;
            remainder = MAX_MCS_INDEX%MAC_MCS_GROUP_SIZE;
            
            /* Loop 1: Update all entries of mcsGroupingTable_g from index 0 to
             * quotient*MAC_MCS_GROUP_SIZE - 1.
             */
            for(grpIdx = 0; grpIdx < quotient; grpIdx++)
            {
                /* All indexes of mcsGroupingTable_g belonging to the same group
                 * will have same entries. Hence the below for loop.
                 */
                for(idx = 0; idx < MAC_MCS_GROUP_SIZE; idx++)                
                {
                    /* Calculate actual index to update */
                    offsetIdx = grpIdx*MAC_MCS_GROUP_SIZE + idx; 
                    
                    /* Setting previousMCS index, nextMCS index and 
                     * equivalent MCS for the group.
                     */                    
                    mcsGroupingTable_g[offsetIdx].previousMCS \
                        = (grpIdx - 1)* MAC_MCS_GROUP_SIZE;
                        
                    mcsGroupingTable_g[offsetIdx].mcsForGroup \
                        = (grpIdx )* MAC_MCS_GROUP_SIZE;
                        
                    mcsGroupingTable_g[offsetIdx].nextMCS \
                        = (grpIdx + 1)* MAC_MCS_GROUP_SIZE;
                }
            }
            
            /* Loop2: Update the remaining entries of mcsGroupingTable_g
             * (indexes quotient*MAC_MCS_GROUP_SIZE to 
                quotient*MAC_MCS_GROUP_SIZE + remainder
               )
             */

            /*Rel_523_Coverity_10050 Fix start*/
#if (1 < MAC_MCS_GROUP_SIZE)
            for(idx = 0; idx < remainder; idx++)
            {
                /* Calculate actual index to update */
                offsetIdx = quotient*MAC_MCS_GROUP_SIZE + idx;

                /* Setting previousMCS index, nextMCS index and 
                 * equivalent MCS for the group.
                 */                
                mcsGroupingTable_g[offsetIdx].previousMCS \
                    = (grpIdx - 1)* MAC_MCS_GROUP_SIZE;
                    
                mcsGroupingTable_g[offsetIdx].mcsForGroup \
                    = (grpIdx )* MAC_MCS_GROUP_SIZE;
                    
                /* As these will be the last entries of the table, setting the 
                 * nextMCS of these to INVALID
                 */
                mcsGroupingTable_g[offsetIdx].nextMCS = INVALID_MCS_INDEX;
            }
#endif
           /*Rel_523_Coverity_10050 Fix End*/

            /* For the entries of the first group, we need to set the 
             * previousMCS to INVALID.
             */
             
            for(idx = 0; idx < MAC_MCS_GROUP_SIZE; idx++)                
            {
                mcsGroupingTable_g[idx].previousMCS = INVALID_MCS_INDEX;
            }

            /* It is still possible(lets say with MAC_MCS_GROUP_SIZE=29,
             * the nextMCS is not updated in Loop2 above. So handling this 
             * special case separately.
             */
            if(0 == remainder)
            {
                for(idx = (quotient - 1)*MAC_MCS_GROUP_SIZE; 
                    idx < MAX_MCS_INDEX; 
                    idx++)
                {
                    mcsGroupingTable_g[idx].nextMCS = INVALID_MCS_INDEX;
                }                
            }

            /* MCS 11 and 10 correspond to same I-TBS
             * Hence setting both of them equal.
             * Same for MCS 21 and 20.
             */
            mcsGroupingTable_g[11].mcsForGroup = \
                mcsGroupingTable_g[10].mcsForGroup;
            mcsGroupingTable_g[21].mcsForGroup = \
                mcsGroupingTable_g[20].mcsForGroup;
            
        }/*if((MAC_MCS_GROUP_SIZE > 0) && ..*/
    }/*if(MAC_MCS_GROUPING_STRATEGY == MAC_MCS_GROUP_BY_GROUP_SIZE)*/
   
    /* SPR 3269 changes start */
    /* Initialize the BLER Correction Factor array */
    /*CA Changes start  */
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)    
    {        
        for (uePuschBler = 0 ; uePuschBler <= 100; )
        {
            /* SPR 7981 changes start */
             blerCorrectionFactor_g[cellCount][uePuschBler] = ((((MAC_TARGET_BLER - uePuschBler) * UL_RESOLUTION_FACTOR) * \
                                                                MAC_BLER_CORRECTION_PARAM_ALPHA))/10;
             /* SPR 7981 changes end */
             uePuschBler++;
        }
        /* SPR 3269 changes end */
    }        
    /*CA Changes end  */
    
    
    LOG_MAC_MSG(MAC_UL_CSI_INIT_CSI_MANAGER_ID, LOGINFO, MAC_UL_CSI, 0/*coverity 97028 +-*/,\
                0, \
                MAC_MAX_SINR_REPORTS_QUEUE_LENGTH, MAC_MCS_GROUPING_STRATEGY,\
                MAC_MCS_GROUP_SIZE, MAC_TARGET_BLER, \
                (UDouble32)MAC_BLER_CORRECTION_PARAM_ALPHA, 0, \
                __func__,"INIT_CSI_MANAGER");
}
/* Cyclomatic Complexity Changes - starts here*/
/******************************************************************************
 * Function Name : updatePrbMapForLowerReportedMcs 
 * Inputs        : inputMCS - Reported UL MCS
 *                 numOfPRB - Number of contiguous PRBs starting from 
 *                   startPRB for which input MCS is received 
 *                 lastIndex - Last PRB for which SINR report can be
 *                   received.
 *                 start -
 * Outputs       : mcsToPrbMap_p - Pointer to UE's MCS to PRB Map 
 *                 lowestReportedMCS_p - Lowest reported UL MCS
 * Returns       : None
 * Description   : This function will update the MCS to PRB set map
 *                 (mcsToPrbMap_p) for the inputMCS with startPrb and numOfPrb,
 *                 Based on input MCS, lowest reported MCS may also
 *                 be updated.
 *****************************************************************************/
static  void updatePrbMapForLowerReportedMcs(MCSToPrbMapInfo *mcsToPrbMap_p,
		UInt8 *lowestReportedMCS_p,
                UInt8 inputMCS,
		UInt8 numOfPRB,
		UInt8 lastIndex,
		UInt8 start
		)
{
    UInt8 mcsIdx = 0;
    SInt8 prbIdx = 0;
    UInt8 prbValue = 1;
    UInt8 end = start + numOfPRB - 1;

    for(mcsIdx = inputMCS; 
	    (mcsIdx >= *lowestReportedMCS_p) && (INVALID_MCS_INDEX != mcsIdx);
	    mcsIdx = mcsGroupingTable_g[mcsIdx].previousMCS)
    {
        if (mcsToPrbMap_p[mcsIdx].prbMap[start] >= numOfPRB)
        {
 	    /* No need to update this MCS as PRBs for this MCS are already 
	     * updated
	     */
	    continue;
        }
        /*****************************************************/
        /*****************PRB Map updation *******************/
	/*****************************************************/
	if(end == lastIndex)
	{
            prbValue = 1;
	}
	else
	{
	    prbValue = mcsToPrbMap_p[mcsIdx].prbMap[end+1]+1;
	}

        for(prbIdx = end;prbIdx >= start; prbIdx--, prbValue++)
	{ 
		/* Starting from end of the reported PRB to the start of 
		 * reported PRB, the indexes are updated. Also see the 
		 * declaration of prbMap to understand how the map is stored.
		 */
		mcsToPrbMap_p[mcsIdx].prbMap[prbIdx] = prbValue;
	}/*for(prbIdx = end...*/

	if(0 != start)
	{
	     /* If the reported PRBs left overlap with an existing report, 
	     * we need to update the existing report also
	      */
	   for( ;
	       	(prbIdx>=0) &&(mcsToPrbMap_p[mcsIdx].prbMap[prbIdx]!=0);
		prbIdx--, prbValue++)
	      {
	         mcsToPrbMap_p[mcsIdx].prbMap[prbIdx] = prbValue;
	      }/*for( prbIdx = start - 1...*/
	}/*0 != start*/

        /*********************************************************************/
        /* startIdx, endIdx, maxContiguous and maxContiguousStartIdx updation*/
        /*********************************************************************/
       if(start < mcsToPrbMap_p[mcsIdx].startIdx)
       {
            mcsToPrbMap_p[mcsIdx].startIdx = start;
       }
       if (end > mcsToPrbMap_p[mcsIdx].endIdx)
       {
            mcsToPrbMap_p[mcsIdx].endIdx = end;
       }

      if(prbValue - 1 > mcsToPrbMap_p[mcsIdx].maxContiguous )
      {
            mcsToPrbMap_p[mcsIdx].maxContiguous = prbValue - 1;
            mcsToPrbMap_p[mcsIdx].maxContiguousStartIdx = prbIdx + 1;
       }
    }
}
/******************************************************************************
 * Function Name : updatePrbMapForHigherReportedMcs 
 * Inputs        : inputMCS - Reported UL MCS
 *                 lastIndex - Last PRB for which SINR report can be
 *                   received.
 *                 start,end
 * Outputs       : mcsToPrbMap_p - Pointer to UE's MCS to PRB Map 
 *                 highestReportedMCS_p - Highest reported UL MCS
 * Returns       : None
 * Description   : This function will update the MCS to PRB set map
 *                 (mcsToPrbMap_p) for the inputMCS with startPrb and numOfPrb,
 *                 Based on input MCS, highest reported MCS may also
 *                 be updated.
 *****************************************************************************/
static  void updatePrbMapForHigherReportedMcs( MCSToPrbMapInfo *mcsToPrbMap_p,
                                      UInt8 *highestReportedMCS_p,
                                      UInt8 inputMCS,
                                      UInt8 lastIndex,
                                      UInt8 start,
                                      UInt8 end)
{
   UInt8 mcsIdx = 0;
   UInt8 tempStartIdx = 0;
   UInt8 tempMaxStartIdx = 0;
   SInt8 prbIdx = 0;
   UInt8 tempVal = 0;
   UInt8 tempMax = 0;
    UInt8 prbValue = 1;

    /* If on these PRBs(startPRB to end), some reports already existed
     * with a higher MCS, 
     * then we need to purge them as now we have received a lower MCS 
     * for these PRBs.
     * For example, consider a map with values in prbMap as below
     * ..|6|   |5|   |4|   |3|   |2|   |1|.. : Existing Map
     *       ........|X|   |X|.......        : PRBs reported with lower MCS
     * ..|2|   |1|   |0|   |0|   |2|   |1|.. : Updated Map
     */

    for(mcsIdx = mcsGroupingTable_g[inputMCS].nextMCS; 
        (mcsIdx <= *highestReportedMCS_p) && (mcsIdx != INVALID_MCS_INDEX); 
        mcsIdx = mcsGroupingTable_g[mcsIdx].nextMCS )
    { 
        if((end < mcsToPrbMap_p[mcsIdx].startIdx) ||
           (start > mcsToPrbMap_p[mcsIdx].endIdx) )
        {
            /* No need to update this MCS*/
            continue;
        }
        
        /*****************************************************/
        /*****************PRB Map updation *******************/
        /*****************************************************/        
        /* Deleting the entries on the reported PRBs since a lower MCS
         * has been reported on these.
         */
        prbIdx = (end < mcsToPrbMap_p[mcsIdx].endIdx)?\
                    end:mcsToPrbMap_p[mcsIdx].endIdx;
        tempStartIdx = (start < mcsToPrbMap_p[mcsIdx].startIdx)?\
                    mcsToPrbMap_p[mcsIdx].startIdx:start;

        for(;prbIdx >= tempStartIdx; prbIdx--)
        {
            mcsToPrbMap_p[mcsIdx].prbMap[prbIdx] = 0;
        }
        
        if(0 != start)
        {
            /* If these reported PRBs lie between a set of contiguous 
             * PRBs on this MCS, due to the for loop above, the 
             * contiguousness is broken. We need to update the remaining 
             * set as well. 
             */
            for(prbIdx = start - 1, prbValue = 1; 
               (prbIdx >= 0) &&(mcsToPrbMap_p[mcsIdx].prbMap[prbIdx] != 0);
                prbIdx--, prbValue++)
            {
                mcsToPrbMap_p[mcsIdx].prbMap[prbIdx] = prbValue;
            }
        }
        
        /*********************************************************************/
        /* startIdx, endIdx, maxContiguous and maxContiguousStartIdx updation*/
        /*********************************************************************/
        /* If current startIdx lies between start and end, then update start. 
         * start looking from end in forward dir. to find first non-zero value.
         * If not found, then set to 0.
         */
        if((start <= mcsToPrbMap_p[mcsIdx].startIdx) && \
           (end >= mcsToPrbMap_p[mcsIdx].startIdx))
        {
            tempVal = 0;
            for(prbIdx = end+1 ;prbIdx <= lastIndex; prbIdx++)
            {
                if(mcsToPrbMap_p[mcsIdx].prbMap[prbIdx])
                {
                    tempVal = prbIdx;
                    break;
                }
            }
            mcsToPrbMap_p[mcsIdx].startIdx = tempVal;
        }
        
        /* If current endIdx lies between start and end, then update end. 
         * start looking from start backwards to find first non-zero value.
         * If not found, then set to 0.
         */
        if((start <= mcsToPrbMap_p[mcsIdx].endIdx) && \
           (end >= mcsToPrbMap_p[mcsIdx].endIdx))
        {
            tempVal = 0;
            for(prbIdx = start - 1; prbIdx >= 0; prbIdx--)
            {
                if(mcsToPrbMap_p[mcsIdx].prbMap[prbIdx])
                {
                    tempVal = prbIdx;
                    break;
                }
            }
            mcsToPrbMap_p[mcsIdx].endIdx = tempVal;
        }
        
        if((end < mcsToPrbMap_p[mcsIdx].maxContiguousStartIdx) || \
           (start > (mcsToPrbMap_p[mcsIdx].maxContiguousStartIdx + \
                     mcsToPrbMap_p[mcsIdx].maxContiguous - 1)) )
        {
            /*Reported PRBs do not affect maximum contiguous set*/
        }
        else
        {
            tempMaxStartIdx = 0;
            tempMax = 0;
            for(prbIdx = mcsToPrbMap_p[mcsIdx].startIdx;
                prbIdx <= mcsToPrbMap_p[mcsIdx].endIdx;
                prbIdx++)
            {
                if(tempMax < mcsToPrbMap_p[mcsIdx].prbMap[prbIdx])
                {
                    tempMax = mcsToPrbMap_p[mcsIdx].prbMap[prbIdx];
                    tempMaxStartIdx = prbIdx;
                    prbIdx += tempMax;
                }
            }
            
            mcsToPrbMap_p[mcsIdx].maxContiguous = tempMax;
            mcsToPrbMap_p[mcsIdx].maxContiguousStartIdx = tempMaxStartIdx;
            
        }
    }/* for(mcsIdx = mcsGroupingTable_g[inputMCS].nextMCS...*/
        
}
/* Cyclomatic Complexity Changes - ends here*/

/*Coverity-5.3.0 CID-54613*/
/******************************************************************************
 * Function Name : cleanupULCSIManager 
 * Inputs        : internalCellIndex - Cell-Index used at MAc
 * Outputs       : None
 * Returns       : None
 * Description   : Cleans up the sinrReportQueue_g initialized by 
 *                 initULCSIManager 
 *****************************************************************************/
/* CA Changes start */
void cleanupULCSIManager(InternalCellIndex internalCellIndex)
{
#ifdef DL_UL_SPLIT
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
#endif
    UInt32 i = 0;
    UInt32 sinrReportQueueCount = 0;
    SINRReportNode *sinrReportNode_p = PNULL;

#ifdef DL_UL_SPLIT
    sinrReportQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + (currentTTI % SINR_CONTAINER_SIZE));
#else
    sinrReportQueueCount = COUNT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex]);
#endif

    for(i=0;i<sinrReportQueueCount;i++)
    {
#ifdef DL_UL_SPLIT
        DEQUEUE_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + (currentTTI % SINR_CONTAINER_SIZE),
                SINRReportNode, (void *)&sinrReportNode_p);
#else
        DEQUEUE_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex],SINRReportNode, (void *)&sinrReportNode_p);
#endif
        if(sinrReportNode_p)
        {
            FREE_MEM_NODE_SINR_REPORT_Q(sinrReportNode_p);
        }
    }
#ifdef DL_UL_SPLIT
    QDEINIT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex] + (currentTTI % SINR_CONTAINER_SIZE));
#else
    QDEINIT_SINR_REPORT_Q(sinrReportQueue_gp[internalCellIndex]);
#endif

}
/* CA Changes end */
/*Coverity-5.3.0 CID-54613*/
/****************************************************************************
 * Function Name  : initNbULCSIManager 
 * Inputs         : Number of cells configured
 * Outputs        : Initialize the Queue's & List
 * Returns        : None 
 * Description    : This function is used to initialize the Queue's and List
 ****************************************************************************/
