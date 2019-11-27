/************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ************************************************************************
 *
 *  $Id :lteMacRRMInterface.c 
 *
 ***********************************************************************
 *
 *  File Description : This file contains the functions for processing RRM
 *                     related configuration.
 ************************************************************************/

/************************************************************************
 * Standard Library Includes
 ***********************************************************************/
/************************************************************************
 * Project Includes
 ***********************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacRRMInterface.h"
#include "lteMacULResourceInfoMgr.h"
#include "lteMacDLResourceInfoMgr.h"
#include "lteMacPuschPowerControl.h"
#include "lteMacRRMPeriodicMgr.h"
#include "lteMacPowerControl.h"
#include "lteMacDLPowerControlData.h"
#include "lteMacPucchPowerControl.h"
#include "lteMacRRMIpcPort.h"
#include "lteMacULQosScheduler.h"
/* Perf stats RRM-MAC start */
#ifdef PERF_STATS
#include "ltePerfStats.h"
#endif
/* Perf stats RRM-MAC end */
#include "lteMacPRSMgr.h"
/***********************************************************************
 * Private Definitions
 ***********************************************************************/

/************************************************************************
 * Global types
 ************************************************************************/
/*ca changes start*/
RRMMACCellGlobal *rrmMacCellGlobal_gp[MAX_NUM_CELL];
/*ca changes end*/
/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
/*Default reduced MCS valuses stoed in global context for HD UE */
UInt8 hdFddReducedMcsTableArr_g[MAX_MCS_VALUES] = 
{0,0,1,2,3,4,5,6,7,8,10,10,10,12,13,13,13,17,17,17,18,19,20,21,21,23,24,24,27,29,30,31};
#endif

/*EICIC +*/
UInt8 absPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN]= {{{0,}}}; /* Aggressor*/
UInt8 usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN] = {{{0,}}}; /* Victim*/
UInt8 absPatternIndex_g[MAX_NUM_CELL] = {0};
UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL] = {0};
UInt8 isEicicEnabled_g[MAX_NUM_CELL] = {LTE_FALSE};
UInt8 startAbsDelay_g[MAX_NUM_CELL] = {LTE_FALSE};
UInt8 applyAbsConfiguration_g[MAX_NUM_CELL] = {LTE_FALSE};
UInt8 isConfigureAbsPatternSet_g[MAX_NUM_CELL]= {LTE_FALSE};
UInt8 isUsablePatternSet_g[MAX_NUM_CELL] = {LTE_FALSE};
UInt8 usableAbsCountOld_g[MAX_NUM_CELL] = {ZERO};
UInt8 usableAbsCountNew_g[MAX_NUM_CELL] = {ZERO};
UInt8 absCountOld_g[MAX_NUM_CELL] = {ZERO};
UInt8 absCountNew_g[MAX_NUM_CELL] = {ZERO};

extern LTE_TREE* dlPriorityLcTree_gp[MAX_NUM_CELL][MAX_VALID_UE_TYPE]; 
extern LTE_TREE*  ulQosStrategyLCTree_gp[MAX_NUM_CELL][MAX_VALID_UE_TYPE];
extern void updateTopSchedulableUEInfoList(InternalCellIndex internalCellIndex);
extern UInt32 dlAggregateGbrPerTTI_g[MAX_NUM_SCELL + MAX_NUM_PCELL][MAX_VALID_UE_TYPE];
extern UInt32 ulAggregateGbrPerTTI_g[MAX_NUM_CELL][MAX_VALID_UE_TYPE] ;
extern UInt32 totalDlAggregateGbrPerTTI_g[MAX_VALID_UE_TYPE];
/* EICIC -*/

void putEntryInExplicitULDCIAndPHRScheduleQueue(UInt16 ueIndex, 
                                ULGrantRequestType ulRequestType, 
                                    InternalCellIndex internalCellIndex);
/*spr 13013 fix start*/
void insertNodeInDlCAEventQueue(UEScellSchedulingInfoNode * ueScellSchedulingInfoNode_p,
                                DLUEContext *ueDLContext_p);
/*spr 13013 fix start*/
extern UInt32 minMCSIndexForATB_g[MAX_NUM_CELL];
extern UInt32 minPRBValForATB_g[MAX_NUM_CELL];
extern UInt16 getUeIdxFromRrcUeIdx(UInt16 rrcUeIdx);

#ifdef PERF_STATS
extern void updateMacPerfContainer(UInt32 intId);
#endif

static  MacRetType macReconfigPathLossSinrAndBlerParams (
        UInt32                     schParamBitmapVal,
        RrmMacReconfigSchedulerReq *rrmMacReconfigSchedulerReq_p);
static  MacRetType macReconfigPdcchAggAndPowerOffsetParams (
        UInt32                     schParamBitmapVal,
        RrmMacReconfigSchedulerReq *rrmMacReconfigSchedulerReq_p);
static  MacRetType macReconfigSpsSchedulingInfo (
        UInt32                     schParamBitmapVal,
        RrmMacReconfigSchedulerReq *rrmMacReconfigSchedulerReq_p);
static  UInt32 macUpdateULSchedulerReconfigParams (
        UInt32                       schParamBitmapVal,
        RrmMacReconfigSchedulerReq   *rrmMacReconfigSchedulerReq_p,
        MacReconfigSchedulerParamsUL *schedulerParamsUL_p,
        InternalCellIndex  internalCellIndex);
STATIC  UInt32 macUpdateDLSchedulerReconfigParams (
        UInt32                       schParamBitmapVal,
        RrmMacReconfigSchedulerReq   *rrmMacReconfigSchedulerReq_p,
        MacReconfigSchedulerParamsDL *schedulerParamsDL_p,
        InternalCellIndex  internalCellIndex);

/* EICIC +*/
RrmMacCellConfigErrorCode  macReconfigureDlUeLCTree(
  InternalCellIndex  internalCellIndex, 
  DLUEContext  *ueDLContext_p
  );

RrmMacCellConfigErrorCode  macReconfigureUlUeLCTree(
  InternalCellIndex  internalCellIndex, 
  ULUEContext  *ueULContext_p
  );

RrmMacCellConfigErrorCode  macReconfigureUlUeGBR(
  InternalCellIndex  internalCellIndex,
  ULUEContext  *ueULContext_p
  );

RrmMacCellConfigErrorCode  macReconfigureDlUeGBR(
  InternalCellIndex  internalCellIndex,
  DLUEContext  *ueDLContext_p
  );

void eicicUpdate_dlAggregateGbrPerTTI_oldUeType(
	InternalCellIndex sCellIndex, 
	InternalCellIndex pCellIndex,
	DLUEContext  *ueDLContext_p,
	UInt8 oldUeType,
    /* +- SPR 17777 */
	DlLcQosInfo *dlLcQosInfo_p,
	UInt16 numOfTicksPerSecond,
	UInt8 pCellBandWidthDist,
	UInt8 sCellBandWidthDist,
	UDouble32 absFactorOne);

void eicicUpdate_dlAggregateGbrPerTTI_newUeType(
	InternalCellIndex sCellIndex, 
	InternalCellIndex pCellIndex,
	DLUEContext  *ueDLContext_p,
	UserType newUeType,
	DlLcQosInfo *dlLcQosInfo_p,
	UInt16 numOfTicksPerSecond,
	UInt8 pCellBandWidthDist,
	UInt8 sCellBandWidthDist,
	UDouble32 absFactorTwo);

RrmMacCellConfigErrorCode configureABSpattern (AbsPatternServingCellConfig *absPatternServingCellConfig_p, 
        InternalCellIndex internalCellIndex);

UInt8 calculateAbsBits (InternalCellIndex internalCellIndex, UInt8 absTypeFlag);

RrmMacCellConfigErrorCode configureUsableABSpattern (AbsPatternServingCellConfig *usableAbsPatternServingCellConfig_p,
    InternalCellIndex internalCellIndex);

extern MacRetType validateAndUpdateMaxUeToBeSchdDlUl(UInt8 maxUeToBeSchdDL, 
                                        UInt8 MaxUeToBeSchdUL,
                                        InternalCellIndex  internalCellIndex);
ModifyValidate  modifyMaxUeToBeSchdDlUl_g[MAX_NUM_CELL];

void checkCellBitMask(UInt32 rrmUeIndex,
        UInt8 cellBitMask,
        UELocationInfo *ueLocationInfo_p,
        DLUEContext *dlUEContext_p,
        UInt8 *scellLocationUpdateFlag_p,
        UInt32 index,
        UELocationError *ueLocationError_p,
        UInt32 *numOfUeWithError_p,
        InternalCellIndex internalCellIndex);

/****************************************************************************
 * Function Name  : validateSrsSubBandGroupSize
 * Inputs         : srsSubBandGroupSize
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates srsSubBandGroupSize
 ****************************************************************************/
MacRetType validateSrsSubBandGroupSize(UInt8 srsSubBandGroupSize)
{
    if((srsSubBandGroupSize == 1) ||
       (srsSubBandGroupSize == 2) ||
       (srsSubBandGroupSize == 4) ||
       (srsSubBandGroupSize == 8))
    {
        return MAC_SUCCESS;
    }
    else
    {
        return MAC_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : validateStartEndPL
 * Inputs         : startPL,endPL
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates startEndPL
 ****************************************************************************/
MacRetType validateStartEndPL(SInt16 startPL, SInt16 endPL)
{
    if(startPL <= endPL)
    {
        if((startPL >= MIN_PATHLOSS && startPL <= MAX_PATHLOSS) &&
                (endPL >= MIN_PATHLOSS && endPL <= MAX_PATHLOSS))
        {
            return MAC_SUCCESS;
        }
        else
        {
            return MAC_FAILURE;
        }
    }
    else
    {
        return MAC_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : validateStartEndSINR
 * Inputs         : startSINR,endSINR
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates startEndSINR
 ****************************************************************************/
MacRetType validateStartEndSINR(SInt16 startSINR, SInt16 endSINR)
{
    if(startSINR <= endSINR)
    {
        if((startSINR >= -255 && startSINR <= 255) &&
                (endSINR >= -255 && endSINR <= 255))
        {
            return MAC_SUCCESS;
        }
        else
        {
            return MAC_FAILURE;
        }
    }
    else
    {
        return MAC_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : validatePuschTpc
 * Inputs         : puschTpc
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates puschTpc
 ****************************************************************************/
MacRetType validatePuschTpc(SInt8 puschTpc)
{
    if((puschTpc >= -4) && (puschTpc <= 4))
    {
        return MAC_SUCCESS;
    }
    else
    {
        return MAC_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : validatePowerOffset 
 * Inputs         : powerOffset
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates powerOffset
 ****************************************************************************/
MacRetType validatePowerOffset(UInt16 powerOffset)
{
    if(powerOffset <= 10000)
    {
        return MAC_SUCCESS;
    }
    else
    {
        return MAC_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : validateStartEndBler
 * Inputs         : startBler,endBler
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates startEndBler
 ****************************************************************************/
MacRetType validateStartEndBler(SInt8 startBler, SInt8 endBler)
{
    if(startBler <= endBler)
    {
        if((startBler >= -100 && startBler <= 100)&&
                (endBler >= -100 && endBler <= 100))
        {
            return MAC_SUCCESS;
        }
        else
        {
            return MAC_FAILURE;
        }
    }
    else
    {
        return MAC_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : validatePucchTpc
 * Inputs         : pucchTpc
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates pucchTpc
 ****************************************************************************/
MacRetType validatePucchTpc(SInt8 pucchTpc)
{
    if((pucchTpc >= -1) && (pucchTpc <= 3))
    {
        return MAC_SUCCESS;
    }
    else
    {
        return MAC_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : validateAggLevel
 * Inputs         : aggLevel
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates aggLevel
 ****************************************************************************/
MacRetType validateAggLevel(UInt8 aggLevel)
{
    if((aggLevel == 1)
            || (aggLevel == 2)
            || (aggLevel == 4)
            || (aggLevel == 8))
    {
        return MAC_SUCCESS;
    }
    else
    {
        return MAC_FAILURE;
    }
}
/* +DYNAMIC_ICIC */
/****************************************************************************
 * Function Name  : validateAtbInRrmIntf 
 * Inputs         : minMCSIndex, minPRBVal 
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates ATB configuration parameters
 ****************************************************************************/
 /* SPR 22099 Fix + */
MacRetType validateAtbInRrmIntf(UInt32 minMCSIndex,UInt32 minPRBVal)
 /* SPR 22099 Fix - */
{
    if((minMCSIndex <= 28) && (minPRBVal >= 1) && (minPRBVal <= 100))
    {
        return MAC_SUCCESS;
    }
    else
    {
        return MAC_FAILURE;
    }
}
/* Ca changes start */
/****************************************************************************
 * Function Name  : macValidateCAParams
 * Inputs         : pointer caQoSStrategyParams_p
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : Validates CA QoS Strategy parameters
 ****************************************************************************/
MacRetType macValidateCAParams(CAParams *caParams_p, InternalCellIndex internalCellIndex)
{
    UInt32 bitMask = 0;
    bitMask = caParams_p->bitMask;
    if(!bitMask)
    {
        LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                caParams_p->bitMask,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        return MAC_FAILURE;
    }
    if(bitMask & LOAD_CAL_WINDOW_SIZE)
    {
        if(caParams_p->loadCalcualtionWindowSize < MIN_LOAD_CAL_WINDOW_SIZE || 
                caParams_p->loadCalcualtionWindowSize > MAX_LOAD_CAL_WINDOW_SIZE)
        {
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    bitMask,
                    caParams_p->loadCalcualtionWindowSize,
                    DEFAULT_INT_VALUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            return MAC_FAILURE;
        }
    }
    if(bitMask & LOAD_DISPARITY_THRESHOLD)
    {
        if(caParams_p->loadDisparityThreshold > MAX_LOAD_DISPARITY_THRESHOLD)
        {
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    bitMask,
                    caParams_p->loadDisparityThreshold,
                    DEFAULT_INT_VALUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            return MAC_FAILURE;
        }
    }
    if(bitMask & MCS_DIFF_THRESHOLD)
    {
        if(caParams_p->mcsDiffThreshold > MAX_MCS_DIFF_THRESHOLD)
        {
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    bitMask,
                    caParams_p->mcsDiffThreshold,
                    DEFAULT_INT_VALUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            return MAC_FAILURE;
        }
    }
    if(bitMask & PRB_USAGE_WATER_MARK)
    {
        if(caParams_p->prbUsageWaterMark > MAX_PRB_USAGE_WATER_MARK)
        {
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    bitMask,
                    caParams_p->prbUsageWaterMark,
                    DEFAULT_INT_VALUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            return MAC_FAILURE;
        }
    }
    if(bitMask & DL_DEACT_MCS_LOW_THRESH)
    {
        if(caParams_p->dlDeactMCSLowThresh > MAX_DL_DEACT_MCS_LOW_THRESH)
        {
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    bitMask,
                    caParams_p->dlDeactMCSLowThresh,
                    DEFAULT_INT_VALUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            return MAC_FAILURE;
        }
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validateSpsDlSchedulingInfoPerTti
 * Inputs         : pointer spsDlSchedulingInfoPerTti_p
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates SpsDlSchedulingInfoPerTti parameters
 ****************************************************************************/
MacRetType validateSpsDlSchedulingInfoPerTti(
               SpsDlSchedulingInfoPerTti *spsDlSchedulingInfoPerTti_p)
{
    UInt8 index = 0;

    /*Checking the range of the maxDlSpsDciPerTti.*/
    if (spsDlSchedulingInfoPerTti_p->maxDlSpsDciPerTti <  
            MAX_DL_SPS_DCI_PER_TTI_MIN_VALUE || 
            spsDlSchedulingInfoPerTti_p->maxDlSpsDciPerTti >
            MAX_DL_SPS_DCI_PER_TTI_MAX_VALUE)
    {
        return MAC_FAILURE;
    }

    /*Checking the range of the maxDlSpsOccasionPerTti.*/
    if (spsDlSchedulingInfoPerTti_p->maxDlSpsOccasionPerTti <
            MAX_DL_SPS_OCCASION_PER_TTI_MIN_VALUE ||
            spsDlSchedulingInfoPerTti_p->maxDlSpsOccasionPerTti >
            MAX_DL_SPS_OCCASION_PER_TTI_MAX_VALUE)
    {
        return MAC_FAILURE;
    }

    /*Checking the range of the maxDlSpsRbsPerTti.*/
    if (spsDlSchedulingInfoPerTti_p->maxDlSpsRbsPerTti <
            MAX_DL_SPS_RBS_PER_TTI_MIN_VALUE ||
            spsDlSchedulingInfoPerTti_p->maxDlSpsRbsPerTti >
            MAX_DL_SPS_RBS_PER_TTI_MAX_VALUE)
    {
        return MAC_FAILURE;
    }

    /*Checking the range of the  maxDlSpsRbsPerTtiPerInterval.*/
    for (index = 0; index < MAX_SPS_INTERVALS; index++)
    {
        if (spsDlSchedulingInfoPerTti_p->maxDlSpsRbsPerTtiPerInterval[index] <
                MAX_DL_SPS_RBS_PER_TTI_PER_INTV_MIN_VALUE ||
                spsDlSchedulingInfoPerTti_p->maxDlSpsRbsPerTtiPerInterval[index] >
                MAX_DL_SPS_RBS_PER_TTI_PER_INTV_MAX_VALUE)
        {
            return MAC_FAILURE;
        }
    }
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validateSpsUlSchedulingInfoPerTti
 * Inputs         : pointer spsUlSchedulingInfoPerTti_p
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Validates SpsUlSchedulingInfoPerTti parameters
 ****************************************************************************/
MacRetType validateSpsUlSchedulingInfoPerTti(
               SpsUlSchedulingInfoPerTti *spsUlSchedulingInfoPerTti_p)
{
    UInt8 index = 0;

    /*Checking the range of the maxUlSpsDciPerTti.*/
    if (spsUlSchedulingInfoPerTti_p->maxUlSpsDciPerTti <  
            MAX_UL_SPS_DCI_PER_TTI_MIN_VALUE || 
            spsUlSchedulingInfoPerTti_p->maxUlSpsDciPerTti >
            MAX_UL_SPS_DCI_PER_TTI_MAX_VALUE)
    {
        return MAC_FAILURE;
    }

    /*Checking the range of the maxUlSpsOccasionPerTti.*/
    if (spsUlSchedulingInfoPerTti_p->maxUlSpsOccasionPerTti <
            MAX_UL_SPS_OCCASION_PER_TTI_MIN_VALUE ||
            spsUlSchedulingInfoPerTti_p->maxUlSpsOccasionPerTti >
            MAX_UL_SPS_OCCASION_PER_TTI_MAX_VALUE)
    {
        return MAC_FAILURE;
    }

    /*Checking the range of the maxUlSpsRbsPerTti.*/
    if (spsUlSchedulingInfoPerTti_p->maxUlSpsRbsPerTti <
            MAX_UL_SPS_RBS_PER_TTI_MIN_VALUE ||
            spsUlSchedulingInfoPerTti_p->maxUlSpsRbsPerTti >
            MAX_UL_SPS_RBS_PER_TTI_MAX_VALUE)
    {
        return MAC_FAILURE;
    }

    /*Checking the range of the  maxUlSpsRbsPerTtiPerInterval.*/
    for (index = 0; index < MAX_SPS_INTERVALS; index++)
    {
        if (spsUlSchedulingInfoPerTti_p->maxUlSpsRbsPerTtiPerInterval[index] <
                MAX_UL_SPS_RBS_PER_TTI_PER_INTV_MIN_VALUE ||
                spsUlSchedulingInfoPerTti_p->maxUlSpsRbsPerTtiPerInterval[index] >
                MAX_UL_SPS_RBS_PER_TTI_PER_INTV_MAX_VALUE)
        {
            return MAC_FAILURE;
        }
    }
    return MAC_SUCCESS;
}

/************************************************************************
 * Function Name    : checkResourceOverlapping
 * Inputs           : prevStartRB, prevEndRB, startRB, endRB
 * Outputs          : None
 * Returns          : overLappedRBCount 
 * Description      : This function is used to chek the resource overlapping
 *                    if any between CC-CC and CE-CC regions
 ***********************************************************************/
UInt32 checkResourceOverlapping(UInt32 prevStartRB,
        UInt32 prevEndRB,
        UInt32 startRB,
        UInt32 endRB)
{
    UInt32 overLappedRBCount = 0;
    /* Check for overlapping
       Case 1: start Rb of CC Region i falls within start Rb of CC Region j
       Case 2: endRb of CC Region i within CC region j
       Case 3: startRb and endRb of CC Region i falls within CC Region j
       Case 4: CC Region j falls within CC region i
     */

    if ((startRB > prevStartRB) &&
            (startRB <= prevEndRB) &&
            (endRB >= prevEndRB))
    {
        overLappedRBCount = prevEndRB - startRB +1;
    }

    else if ((startRB >= prevStartRB) && 
            (endRB <= prevEndRB))
    {
        overLappedRBCount = endRB - prevStartRB +1;

    }
    else if ((startRB <= prevStartRB) && 
            (endRB >= prevStartRB) &&
            (endRB < prevEndRB)) 
    {
        overLappedRBCount = endRB - startRB +1;
    }

    else if ((startRB < prevStartRB) && (endRB > prevEndRB))  
    {
        overLappedRBCount = prevEndRB - prevStartRB +1;

    }

    return overLappedRBCount;
}

/************************************************************************
 * Function Name    : initMacDLResourceInfo
 * Inputs           : dlResourcePartitionInfo_p,dlResourceConfigInfo_p,
 *                    internalCellIndex
 * Returns          : MAC_FAILURE or MAC_SUCCESS
 * Description      : This function processes resource partition information 
 *                    for cell center and cell edge regions in DL side
 ************************************************************************/
UInt32 initMacDLResourceInfo(ResourcePartitionInfo *dlResourcePartitionInfo_p,
        DLResourcePartitionConfigInfo *dlResourceConfigInfo_p,
        InternalCellIndex  internalCellIndex)
{
    UInt32     bitmask           = 0;
    UInt32     index             = 0;
    UInt32     totalRBCount      = 0;
    UInt32     edgeRBCount       = 0;
    UInt32     numRB             = 0;
    MacRetType retVal            = MAC_SUCCESS;
    UInt32     numOfCCRegion     =0;  
    UInt32     numOfCERegion     =0;  
    UInt32     startRB      =0;  
    UInt32     endRB      =0;  
    UInt32 overLappedRBCountCERegion = 0;
    UInt32 overLappedRBCountCCRegion = 0;
    UInt32 prevStartRB = 0;
    CellConfigParams *cellParams_p = NULL;
    cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p \
                   [internalCellIndex]->cellParams_p;

    bitmask=dlResourcePartitionInfo_p->bitmask;
    numOfCCRegion=dlResourcePartitionInfo_p->numOfCellCenterRegion;   


    if ((numOfCCRegion > MAX_REGIONS) || (numOfCCRegion == 0))
    {
        LOG_MAC_MSG(MAC_RRM_INVALID_NUM_OF_CC_REGION,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                numOfCCRegion,LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        return INVALID_NUMBER_OF_CC_REGION;
    }

    if(CE_RESOURCE_INFO_BITMASK & bitmask )
    { 
        numOfCERegion=dlResourcePartitionInfo_p->numOfCellEdgeRegion;  

        if(numOfCERegion > MAX_CE_REGION) 
        {
            LOG_MAC_MSG(MAC_RRM_INVALID_NUM_OF_CE_REGION,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    numOfCERegion,LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            return INVALID_NUMBER_OF_CE_REGION;
        }

        if(rrmMacCellGlobal_gp[internalCellIndex]-> \
                totalCellEdgeUserCount > 0 && numOfCERegion == 0)
        {
            return CE_USER_EXIST_CE_REGION_IS_ZERO; 
        } 

        if(0 != numOfCERegion)
        {
            numRB = dlResourcePartitionInfo_p->CellEdgeRegion.numOfRB;
            startRB = dlResourcePartitionInfo_p->CellEdgeRegion.startRB;

            if ((numRB > cellParams_p->dlAvailableRBs ) ||
                    (startRB > (UInt32)(cellParams_p->dlAvailableRBs - 1 ))|| 
                    (startRB + numRB > cellParams_p->dlAvailableRBs)||numRB < 3)
            {
                LOG_MAC_MSG(MAC_RRM_INVALID_RB_VALUE,LOGWARNING,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        numRB,startRB,LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");

                return PARAMETER_OUT_OF_RANGE;
            }
            /* Process Cell edge Resource Info For DL **/
            dlResourceConfigInfo_p->ceResourceInfo.startRBIndex = startRB;
            dlResourceConfigInfo_p->ceResourceInfo.endRBIndex   =
                startRB + numRB - 1;
            dlResourceConfigInfo_p->cellEdgeRegionCount = numOfCERegion;
            totalRBCount += numRB;
            edgeRBCount   = numRB;
        }
    }

    /* Process count for cell center and cell edge region For DL **/
    dlResourceConfigInfo_p->cellCenterRegionCount = numOfCCRegion;
    dlResourceConfigInfo_p->cellEdgeRegionCount   = numOfCERegion;

    /* Process Cell center Resource Info For DL */
    for(; index < numOfCCRegion; index++)
    {  
        numRB = dlResourcePartitionInfo_p->CellCenterRegion[index].numOfRB;
        startRB = dlResourcePartitionInfo_p->CellCenterRegion[index].startRB;

        if ( startRB < prevStartRB)
        {
            return START_RB_NOT_IN_INCREASING_ORDER;
        }

        if ((numRB > cellParams_p->dlAvailableRBs ) ||
                (startRB > (UInt32)(cellParams_p->dlAvailableRBs - 1))|| 
                /*SPR:7263:START*/
                (startRB + numRB > cellParams_p->dlAvailableRBs)|| numRB < 3)
            /*SPR:7263:END*/
        {
            LOG_MAC_MSG(MAC_RRM_INVALID_RB_VALUE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    numRB,startRB,LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");

            return PARAMETER_OUT_OF_RANGE;
        }
        dlResourceConfigInfo_p->ccResourceInfo[index].startRBIndex = startRB;
        endRB = startRB + numRB -1;
        dlResourceConfigInfo_p->ccResourceInfo[index].endRBIndex  = endRB;

        if (numOfCERegion)
        {
            /* Checking Overlapping With Cell Edge */
            /* SPR 6065 Fix Start */
            overLappedRBCountCERegion = checkResourceOverlapping(dlResourceConfigInfo_p->ceResourceInfo.startRBIndex,
                    dlResourceConfigInfo_p->ceResourceInfo.endRBIndex,
                    startRB,
                    endRB);
            if (overLappedRBCountCERegion)
            {
                return CC_REGION_OVERLAPPING;
            }
        }
        /* Checking Overlapping with Cell Center */
        if(index)
        {
            overLappedRBCountCCRegion = checkResourceOverlapping(dlResourceConfigInfo_p->
                    ccResourceInfo[index-1].startRBIndex,
                    dlResourceConfigInfo_p->ccResourceInfo[index-1].endRBIndex,
                    startRB,
                    endRB);

            if (overLappedRBCountCCRegion)
            {
                return CC_REGION_OVERLAPPING;
            }
        }
        totalRBCount += numRB;
        prevStartRB = startRB;
        /* SPR 6065 Fix End */
    }

    /* Validation of total no of RB received with total DL RB*/
    if (totalRBCount > cellParams_p->dlAvailableRBs)
    {
        LOG_MAC_MSG(MAC_RRM_INVALID_TOTAL_RB_VALUE,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                totalRBCount,LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");


        return INVALID_TOTAL_RB_COUNT;
    }
    else
    {
        dlResourceConfigInfo_p->totalCellEdgeRBDL   = edgeRBCount;
        dlResourceConfigInfo_p->totalAvailableRBDL  = totalRBCount;
    }

    return retVal;
}


/************************************************************************
 * Function Name    : initMacULResourceInfo
 * Inputs           : ulResourcePartitionInfo_p,ulResourceConfigInfo_p
 *                    internalCellIndex
 * Returns          : MAC_FAILURE or MAC_SUCCESS
 * Description      : This function processes for resource partition 
 *                    information  for cell center and cell edge regions 
 *                    on UL side
 ************************************************************************/
UInt32 initMacULResourceInfo(ResourcePartitionInfo *ulResourcePartitionInfo_p,
        ULResourcePartitionConfigInfo *ulResourceConfigInfo_p,
        InternalCellIndex  internalCellIndex)
{
    UInt32       bitmask       = 0;
    UInt32       index         = 0;
    UInt32       numRB         = 0;
    UInt32       totalRBCount  = 0;
    UInt32       edgeRBCount   = 0;
    MacRetType   retVal        = MAC_SUCCESS;
    UInt32       numOfCCRegion = 0;
    UInt32       numOfCERegion = 0;
    UInt32       startRB       = 0;
    UInt32       endRB       = 0;
    UInt32 prevStartRB = 0;
    UInt32 overLappedRBCountCERegion = 0;
    UInt32 overLappedRBCountCCRegion = 0;
    CellConfigParams *cellParams_p = NULL;
    cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p \
                   [internalCellIndex]->cellParams_p;

    bitmask = ulResourcePartitionInfo_p->bitmask;
    numOfCCRegion=ulResourcePartitionInfo_p->numOfCellCenterRegion;  

    if(CE_RESOURCE_INFO_BITMASK & bitmask)
    {
        numOfCERegion=ulResourcePartitionInfo_p->numOfCellEdgeRegion; 

        if(numOfCERegion > MAX_CE_REGION) 
        {
            LOG_MAC_MSG(MAC_RRM_INVALID_RB_VALUE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    numOfCERegion,LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            return INVALID_NUMBER_OF_CE_REGION;
        }

        if(rrmMacCellGlobal_gp[internalCellIndex]-> \
                totalCellEdgeUserCount > 0 && numOfCERegion == 0)
        {
            return CE_USER_EXIST_CE_REGION_IS_ZERO; 
        }

        if(0 != numOfCERegion)
        {
            numRB = ulResourcePartitionInfo_p->CellEdgeRegion.numOfRB;
            startRB = ulResourcePartitionInfo_p->CellEdgeRegion.startRB;
            /* SPR 21130 fix start */
            if ((numRB > cellParams_p->ulAvailableRBs ) ||
                    (startRB > (UInt32)(cellParams_p->ulAvailableRBs - 1 ))||
                    (startRB + numRB > cellParams_p->ulAvailableRBs))
                /* SPR 21130 fix end */
            {
                LOG_MAC_MSG(MAC_RRM_INVALID_RB_VALUE,LOGWARNING,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        numRB,startRB,LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
                return PARAMETER_OUT_OF_RANGE;
            }
            /* Process Cell Edge Resource Info For UL */
            ulResourceConfigInfo_p->ceResourceInfo.startRBIndex = startRB;
            ulResourceConfigInfo_p->ceResourceInfo.endRBIndex = startRB + numRB - 1;
            ulResourceConfigInfo_p->cellEdgeRegionCount   = numOfCERegion;
            endRB = startRB + numRB - 1;
            totalRBCount +=  numRB;
            edgeRBCount   = numRB;
        }
    }

    if ((numOfCCRegion > MAX_REGIONS)||(numOfCCRegion == 0))
    {
        LOG_MAC_MSG(MAC_RRM_INVALID_NUM_OF_CC_REGION,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                numOfCCRegion,LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        return INVALID_NUMBER_OF_CC_REGION;
    }

    /* Process count for cell center and cell edge region For UL **/
    ulResourceConfigInfo_p->cellCenterRegionCount = numOfCCRegion;
    ulResourceConfigInfo_p->cellEdgeRegionCount = numOfCERegion;

    /* Process Cell Center Resource Info For UL */
    for(; index < numOfCCRegion; index++)
    { 
        numRB = ulResourcePartitionInfo_p->CellCenterRegion[index].numOfRB;
        startRB = ulResourcePartitionInfo_p->CellCenterRegion[index].startRB;

        if ( startRB < prevStartRB)
        {
            return START_RB_NOT_IN_INCREASING_ORDER;
        }

        if ((numRB > cellParams_p->ulAvailableRBs ) ||
                (startRB > (UInt32)(cellParams_p->ulAvailableRBs -1))|| 
                (startRB + numRB > cellParams_p->ulAvailableRBs)||numRB < 3)  
        { 
            LOG_MAC_MSG(MAC_RRM_INVALID_RB_VALUE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    numRB,startRB,LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");

            return PARAMETER_OUT_OF_RANGE;
        }

        ulResourceConfigInfo_p->ccResourceInfo[index].startRBIndex =  startRB;
        endRB = startRB + numRB -1;
        ulResourceConfigInfo_p->ccResourceInfo[index].endRBIndex = endRB;

        if (numOfCERegion)
        {
            /* Checking Overlapping With Cell Edge */
            /* SPR 6065 Fix Start */
            overLappedRBCountCERegion = checkResourceOverlapping(ulResourceConfigInfo_p->ceResourceInfo.startRBIndex,
                    ulResourceConfigInfo_p->ceResourceInfo.endRBIndex,
                    startRB,
                    endRB);
            if (overLappedRBCountCERegion)
            {
                return CC_REGION_OVERLAPPING;
            }
        }
        /* Checking Overlapping with Cell Center */
        if(index)
        {
            overLappedRBCountCCRegion = checkResourceOverlapping(ulResourceConfigInfo_p->
                    ccResourceInfo[index-1].startRBIndex,
                    ulResourceConfigInfo_p->ccResourceInfo[index-1].endRBIndex,
                    startRB,
                    endRB);

            if (overLappedRBCountCCRegion)
            {
                return CC_REGION_OVERLAPPING;
            }
        }
        totalRBCount += numRB;
        prevStartRB = startRB;
        /* SPR 6065 Fix End */
    }

    /* Validation of total no of RB received with total DL RB*/
    if (totalRBCount > cellParams_p->ulAvailableRBs)
    { 
        LOG_MAC_MSG(MAC_RRM_INVALID_TOTAL_RB_VALUE,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                totalRBCount,LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        return INVALID_TOTAL_RB_COUNT;
    }
    else
    {
        ulResourceConfigInfo_p->totalCellEdgeRBUL  = edgeRBCount;
        ulResourceConfigInfo_p->totalAvailableRBUL = totalRBCount;
    }

    return retVal;
}

/* EICIC +*/
/************************************************************************
 * Function Name    : getABSPattern
 * Inputs           : absPattern
 * Returns          : absCount
 * Description      : This function processes converts bitwise ABS pattern to byte 
 *                  : pattern in global absPattern_g
 ************************************************************************/
UInt8 getABSPattern(UInt8 absPattern[], InternalCellIndex internalCellIndex, UInt8 absTypeFlag)
{
    UInt8 j_loop  = 0;
    UInt8 absCount = 0;
    UInt8 numBitsAbsPattern;
    UInt8	absPatternIndexTemp = !absPatternIndex_g[internalCellIndex];
    UInt8	usableAbsPatternIndexTemp = !usableAbsPatternIndex_g[internalCellIndex];

#ifdef FDD_CONFIG
    numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
    }
    else
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
    }
#endif

    if (ABS_PATTERN_TYPE == absTypeFlag)
    {
        for (j_loop = 0 ; j_loop < numBitsAbsPattern ; j_loop++)
        {
            if ((absPattern[(j_loop/NUMBER_OF_BITS_IN_1BYTE)] & (0x80 >> (j_loop%NUMBER_OF_BITS_IN_1BYTE) )) == LTE_FALSE)
            {              
                absPattern_g[internalCellIndex][absPatternIndexTemp][j_loop] = LTE_FALSE;
            }
            else
            {
                absPattern_g[internalCellIndex][absPatternIndexTemp][j_loop] = LTE_TRUE;
                absCount++;
            }
        }
    }
    else if ( USABLE_ABS_PATTERN_TYPE == absTypeFlag)
    {
        for (j_loop = 0 ; j_loop < numBitsAbsPattern ; j_loop++)
        {
            if ((absPattern[(j_loop/NUMBER_OF_BITS_IN_1BYTE)] & (0x80 >> (j_loop%NUMBER_OF_BITS_IN_1BYTE) )) == LTE_FALSE)
            {              
                usableAbsPattern_g[internalCellIndex][usableAbsPatternIndexTemp][j_loop] = LTE_FALSE;
            }
            else
            {
                usableAbsPattern_g[internalCellIndex][usableAbsPatternIndexTemp][j_loop] = LTE_TRUE;
                absCount++;
            }
        }
    }

    return absCount;
}


/************************************************************************
 * Function Name    : calculateAbsBits
 * Inputs           : absPattern
 * Returns          : absCount
 * Description      : This function processes converts bitwise ABS pattern to byte 
 *                  : pattern in global absPattern_g
 ************************************************************************/
UInt8 calculateAbsBits(InternalCellIndex internalCellIndex, UInt8 absTypeFlag)
{
    UInt8 j_loop  = 0;
    UInt8 absCount = 0;
    UInt8 numBitsAbsPattern;

#ifdef FDD_CONFIG
    numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
    }
    else
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
    }
#endif

    if (ABS_PATTERN_TYPE == absTypeFlag)
    {
        for (j_loop = 0 ; j_loop < numBitsAbsPattern ; j_loop++)
        {
            if ((absPattern_g[internalCellIndex][absPatternIndex_g[internalCellIndex]][j_loop]) == LTE_TRUE)
            {              
                absCount++;
            }  
        }
    }
    else if ( USABLE_ABS_PATTERN_TYPE == absTypeFlag)
    {
        for (j_loop = 0 ; j_loop < numBitsAbsPattern ; j_loop++)
        {
            if ((usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][j_loop]) == LTE_TRUE)
            {              
                absCount++;
            }  
        }
    }
    return absCount;
}


/************************************************************************
 * Function Name    : configureABSpattern
 * Inputs           : absPatternServingCellConfig_p
 * Returns          : RrmMacCellConfigErrorCode
 * Description      : This function processes ABS Pattern received from RRM
 ************************************************************************/
RrmMacCellConfigErrorCode configureABSpattern(AbsPatternServingCellConfig *absPatternServingCellConfig_p, InternalCellIndex internalCellIndex)
{
    UInt8 absCount = 0;
    InternalCellIndex cellIndex = 0;
    UInt8 absPatternLoop = 0;
    UInt8	absPatternIndexTemp = !absPatternIndex_g[internalCellIndex];

    absCount = getABSPattern((absPatternServingCellConfig_p->absPatternServingCell), internalCellIndex,ABS_PATTERN_TYPE);

    if (IS_CA_ENABLED())
    {
        for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
        {
            if (cellIndex != internalCellIndex)
            {
                if (absCount == absCountNew_g[cellIndex])
                {
                    for ( absPatternLoop =0 ; absPatternLoop < NUM_BITS_ABS_PATTERN ; absPatternLoop++)
                    {
                        if (absPattern_g[internalCellIndex][absPatternIndexTemp][absPatternLoop] == 
                                absPattern_g[cellIndex][absPatternIndex_g[cellIndex]][absPatternLoop])
                        {
                            continue;
                        }
                        else
                        {
                            lteWarning("EICIC : Mismatch in ABS pattern between internalCellIndex:%d "
                                    "and cellIndex:%d with abscount as %d  and abscountloop %d\n",
                                     internalCellIndex, cellIndex, absCount,absPatternLoop);
                        }
                    }
                }	
                else
                {
                    if (absCountNew_g[cellIndex] > 0)
                        lteWarning("EICIC : Mismatch in ABS pattern between internalCellIndex:%d "
                                "and cellIndex:%d with abscount as %d\n",
                                internalCellIndex, cellIndex, absCount);
                }
            }     			
        }
    }

    if (absCount > 0)
    {
        isConfigureAbsPatternSet_g[internalCellIndex] = LTE_TRUE;
    }
    else
    {
        isConfigureAbsPatternSet_g[internalCellIndex] = LTE_FALSE;
    }

    return RRM_MAC_SUCCESS;
}

/************************************************************************
 * Function Name    : configureUsableABSpattern
 * Inputs           : usableAbsPatternServingCellConfig_p
 * Returns          : RrmMacCellConfigErrorCode
 * Description      : This function processes Usable ABS Pattern received from RRM
 ************************************************************************/
RrmMacCellConfigErrorCode configureUsableABSpattern(AbsPatternServingCellConfig *usableAbsPatternServingCellConfig_p, InternalCellIndex internalCellIndex )

{
    UInt8 usableAbsCount = 0;
    InternalCellIndex cellIndex = 0;
    UInt8 absPatternLoop = 0;
    UInt8	usableAbsPatternIndexTemp = !usableAbsPatternIndex_g[internalCellIndex];

    usableAbsCount = getABSPattern((usableAbsPatternServingCellConfig_p->absPatternServingCell),internalCellIndex,USABLE_ABS_PATTERN_TYPE); 

    if (IS_CA_ENABLED())
    {
        for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells ; cellIndex++)
        {
            if (cellIndex != internalCellIndex)
            {
                if (usableAbsCount == usableAbsCountNew_g[cellIndex])
                {
                    for ( absPatternLoop =0 ; absPatternLoop < NUM_BITS_ABS_PATTERN ; absPatternLoop++)
                    {
                        if (usableAbsPattern_g[internalCellIndex][usableAbsPatternIndexTemp][absPatternLoop] == 
                                usableAbsPattern_g[cellIndex][usableAbsPatternIndex_g[cellIndex]][absPatternLoop])
                        {
                            continue;
                        }
                        else
                        {
                            lteWarning("EICIC : Mismatch in ABS pattern between internalCellIndex : %d"
                                    "and cellIndex : %d with usableAbsCount as %d \n",internalCellIndex,cellIndex,usableAbsCount);   
                        }
                    }
                }	
                else
                {
                    if (usableAbsCountNew_g[cellIndex] > 0)
                        lteWarning("EICIC : Mismatch in ABS pattern between internalCellIndex : %d"
                                "and cellIndex : %d with usableAbsCount as %d \n",internalCellIndex,cellIndex,usableAbsCount);   
                }
            }     			
        }
    }

    if (usableAbsCount > 0)
    {
        isUsablePatternSet_g[internalCellIndex] = LTE_TRUE;
    }
    else
    {
        isUsablePatternSet_g[internalCellIndex] = LTE_FALSE;
    }	

    return RRM_MAC_SUCCESS;
}

/************************************************************************
 * Function Name    : macReconfigureLCtree
 * Inputs           : 
 * Returns          : 
 * Description      : 
 *
 ************************************************************************/
RrmMacCellConfigErrorCode  macReconfigureLCtree (InternalCellIndex  internalCellIndex)
{
    DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
    ULQosStrategyLCNode *ulQosStrategyLCNode_p = PNULL;

    /* Traverse the Victim tree, delete the LC node and insert it back to non Victm tree */
    /* DL UE */
    dlQosStrategyLCNode_p = (DLQosStrategyLCNode *)dlQosGetFirstLcNode (dlPriorityLcTree_gp[internalCellIndex][VICTIM_UE]);

    while (NULL != dlQosStrategyLCNode_p)
    {
        if ( dlQosStrategyLCNode_p->lcInfo_p->isLCNodePresent == TRUE)
        {
            dlQosDeleteLcNode(dlPriorityLcTree_gp[internalCellIndex][VICTIM_UE] , &dlQosStrategyLCNode_p->lcTreeAnchor);
            dlQosInsertLcNode(dlPriorityLcTree_gp[internalCellIndex][NON_VICTIM_UE] , &dlQosStrategyLCNode_p->lcTreeAnchor);
        }
        dlQosStrategyLCNode_p = (DLQosStrategyLCNode *)
            dlQosGetNextLcNode(
                    &dlQosStrategyLCNode_p->lcTreeAnchor);
    }

    /* UL UE */
    /* Traverse the Victim tree, delete the LC node and insert it back to non Victm tree */

    ulQosStrategyLCNode_p = (ULQosStrategyLCNode *)dlQosGetFirstLcNode (ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE]);

    while (NULL != ulQosStrategyLCNode_p)
    {
        if ((QOS_GBR == ulQosStrategyLCNode_p->channelType) || 
                (SRB == ulQosStrategyLCNode_p->channelType))
        {
            if(TRUE == ulQosStrategyLCNode_p->isLCNodePresent)
            {
                ulQosDeleteLcNode(ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE] , &ulQosStrategyLCNode_p->lcTreeNodeAnchor);

                ulQosInsertLcNode(ulQosStrategyLCTree_gp[internalCellIndex][NON_VICTIM_UE] , &ulQosStrategyLCNode_p->lcTreeNodeAnchor);
            }
        }
        else if (NON_GBR == ulQosStrategyLCNode_p->channelType)
        {
            if (TRUE == ulQosStrategyLCNode_p->ulAmbrInfo_p->isLCNodePresent)
            {
                ulQosDeleteLcNode(ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE] , &ulQosStrategyLCNode_p->lcTreeNodeAnchor);

                ulQosInsertLcNode(ulQosStrategyLCTree_gp[internalCellIndex][NON_VICTIM_UE] , &ulQosStrategyLCNode_p->lcTreeNodeAnchor);
            }
        }
        ulQosStrategyLCNode_p = (ULQosStrategyLCNode *)
            dlQosGetNextLcNode(
                    &ulQosStrategyLCNode_p->lcTreeNodeAnchor);
    }

    return RRM_MAC_SUCCESS;
}

/************************************************************************
 * Function Name    : macReconfigureDlUeLCTree
 * Inputs           : 
 * Returns          : 
 * Description      : 
 *
 ************************************************************************/
RrmMacCellConfigErrorCode  macReconfigureDlUeLCTree (InternalCellIndex  internalCellIndex, DLUEContext  *ueDLContext_p)
{
    void                    *dlEligibleLcNodeVoid_p = NULL;
    DLQosStrategyLCNode     *dlEligibleLcNode_p     = NULL;

    if (VICTIM_UE == ueDLContext_p->userType)
    {	
        dlEligibleLcNodeVoid_p = (void*) getFirstListNode(
                &ueDLContext_p->dlEligibleLcList);

        while(PNULL != dlEligibleLcNodeVoid_p)
        {
            DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                    dlEligibleLcNodeVoid_p, dlEligibleLcNode_p);

            if ( dlEligibleLcNode_p->lcInfo_p->isLCNodePresent == TRUE)
            {

                dlQosDeleteLcNode(dlPriorityLcTree_gp[internalCellIndex][NON_VICTIM_UE] , &dlEligibleLcNode_p->lcTreeAnchor);
                dlQosInsertLcNode(dlPriorityLcTree_gp[internalCellIndex][VICTIM_UE] , &dlEligibleLcNode_p->lcTreeAnchor);
            }
            dlEligibleLcNodeVoid_p = (void*) getNextListNode(&(dlEligibleLcNode_p->eligibleLcAnchor));
        }
    }
    else if (NON_VICTIM_UE== ueDLContext_p->userType)
    {
        dlEligibleLcNodeVoid_p = (void*) getFirstListNode(
                &ueDLContext_p->dlEligibleLcList);
        while(PNULL != dlEligibleLcNodeVoid_p)
        {
            DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                    dlEligibleLcNodeVoid_p, dlEligibleLcNode_p);

            if ( dlEligibleLcNode_p->lcInfo_p->isLCNodePresent == TRUE)
            {
                dlQosDeleteLcNode(dlPriorityLcTree_gp[internalCellIndex][VICTIM_UE] , &dlEligibleLcNode_p->lcTreeAnchor);
                dlQosInsertLcNode(dlPriorityLcTree_gp[internalCellIndex][NON_VICTIM_UE] , &dlEligibleLcNode_p->lcTreeAnchor);
            }
            dlEligibleLcNodeVoid_p = (void*) getNextListNode(&(dlEligibleLcNode_p->eligibleLcAnchor));
        }
    }
    return RRM_MAC_SUCCESS;
}


/************************************************************************
 * Function Name    : macReconfigureUlUeLCTree
 * Inputs           : 
 * Returns          : 
 * Description      : 
 *
 ************************************************************************/
RrmMacCellConfigErrorCode  macReconfigureUlUeLCTree (InternalCellIndex  internalCellIndex, ULUEContext  *ueULContext_p)
{
    void                    *ulEligibleLcNodeVoid_p = NULL;
    ULQosStrategyLCNode     *ulEligibleLcNode_p     = NULL;

    if (VICTIM_UE == ueULContext_p->userType)
    {
        ulEligibleLcNodeVoid_p = (void*) getFirstListNode(
                &ueULContext_p->ulEligibleLcList);

        while(PNULL != ulEligibleLcNodeVoid_p)
        {
            UL_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                    ulEligibleLcNodeVoid_p, ulEligibleLcNode_p);

            if ((QOS_GBR == ulEligibleLcNode_p->channelType) || 
                    (SRB == ulEligibleLcNode_p->channelType))
            {
                if(TRUE == ulEligibleLcNode_p->isLCNodePresent)
                {
                    ulQosDeleteLcNode(ulQosStrategyLCTree_gp[internalCellIndex][NON_VICTIM_UE] , &ulEligibleLcNode_p->lcTreeNodeAnchor);
                    ulQosInsertLcNode(ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE] , &ulEligibleLcNode_p->lcTreeNodeAnchor);
                }
            }
            else if (NON_GBR == ulEligibleLcNode_p->channelType)
            {
                if (TRUE == ulEligibleLcNode_p->ulAmbrInfo_p->isLCNodePresent)
                {
                    ulQosDeleteLcNode(ulQosStrategyLCTree_gp[internalCellIndex][NON_VICTIM_UE] , &ulEligibleLcNode_p->lcTreeNodeAnchor);
                    ulQosInsertLcNode(ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE] , &ulEligibleLcNode_p->lcTreeNodeAnchor);
                }
            }
            ulEligibleLcNodeVoid_p = (void*) getNextListNode(&(ulEligibleLcNode_p->eligibleLcNodeAnchor));
        }
    }
    else if (NON_VICTIM_UE== ueULContext_p->userType)
    {
        ulEligibleLcNodeVoid_p = (void*) getFirstListNode(
                &ueULContext_p->ulEligibleLcList);
        while(PNULL != ulEligibleLcNodeVoid_p)
        {
            UL_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(
                    ulEligibleLcNodeVoid_p, ulEligibleLcNode_p);

            if ((QOS_GBR == ulEligibleLcNode_p->channelType) || 
                    (SRB == ulEligibleLcNode_p->channelType))
            {
                if(TRUE == ulEligibleLcNode_p->isLCNodePresent)
                {
                    ulQosDeleteLcNode(ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE] , &ulEligibleLcNode_p->lcTreeNodeAnchor);
                    ulQosInsertLcNode(ulQosStrategyLCTree_gp[internalCellIndex][NON_VICTIM_UE] , &ulEligibleLcNode_p->lcTreeNodeAnchor);
                }
            }
            else if (NON_GBR == ulEligibleLcNode_p->channelType)
            {
                if (TRUE == ulEligibleLcNode_p->ulAmbrInfo_p->isLCNodePresent)
                {
                    ulQosDeleteLcNode(ulQosStrategyLCTree_gp[internalCellIndex][VICTIM_UE] , &ulEligibleLcNode_p->lcTreeNodeAnchor);
                    ulQosInsertLcNode(ulQosStrategyLCTree_gp[internalCellIndex][NON_VICTIM_UE] , &ulEligibleLcNode_p->lcTreeNodeAnchor);
                }
            }
            ulEligibleLcNodeVoid_p = (void*) getNextListNode(&(ulEligibleLcNode_p->eligibleLcNodeAnchor));
        }
    }

    return RRM_MAC_SUCCESS;
}


/************************************************************************
 * Function Name    : macReconfigureUlUeGBR
 * Inputs           : 
 * Returns          : 
 * Description      : 
 *
 ************************************************************************/
RrmMacCellConfigErrorCode  macReconfigureUlUeGBR (InternalCellIndex  internalCellIndex, ULUEContext *ULUEContext)
{

    UserType newUeType = ULUEContext->userType;
    UInt8 oldUeType = (newUeType == NON_VICTIM_UE?VICTIM_UE:NON_VICTIM_UE);
    UInt8 absCount = 0;
    UDouble32 absFactorOne = 0;
    UDouble32 absFactorTwo = 0;

    InitParams *initParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    UInt8    lcCount   = 0;

#ifdef FDD_CONFIG
    UInt16 numOfTicksPerSecond = NUM_TICKS_PER_SECOND;
#else
    UInt16 numOfTicksPerSecond = getNumTicksPerSecond(DL, internalCellIndex);
#endif

    if (usableAbsCountNew_g[internalCellIndex] > 0)
        absCount = usableAbsCountNew_g[internalCellIndex];
    else
        absCount = 0;

    if  (( VICTIM_UE == oldUeType) && (0 < absCount ) && ( NON_VICTIM_UE == newUeType))
    {
        absFactorOne = (UDouble32) NUM_BITS_ABS_PATTERN/ ( UDouble32 )absCount;
        absFactorTwo = (UDouble32 ) NUM_BITS_ABS_PATTERN/ ( UDouble32) (NUM_BITS_ABS_PATTERN - absCount);
    }
    else if  (( NON_VICTIM_UE == oldUeType) && (0 < absCount ) && ( VICTIM_UE == newUeType))
    {
        absFactorOne = (UDouble32)NUM_BITS_ABS_PATTERN/ ( UDouble32)(NUM_BITS_ABS_PATTERN - absCount);
        absFactorTwo = (UDouble32 )NUM_BITS_ABS_PATTERN/ (UDouble32) absCount;
    }
    else
    {
        return RRM_MAC_SUCCESS;
    }

    if(UL_QOS_SCHEDULING == initParams_p->ulSchdConfig.ulSchedulerStrategy)
    {
        lcCount = 1;
        while((lcCount < MAX_LCID) && (ULUEContext->numLogicalChannels)) 
        {
            if(INVALID_LCID != ULUEContext->lcInfo[lcCount].lchId)
            {
                if(QOS_GBR == ULUEContext->lcInfo[lcCount].ulQosInfo.channelType)
                {
                    if( ulAggregateGbrPerTTI_g[internalCellIndex][oldUeType]<
                            (ULUEContext->lcInfo[lcCount].ulQosInfo.GBR / 
                             numOfTicksPerSecond ) * absFactorOne)
                    {
                        ulAggregateGbrPerTTI_g [internalCellIndex][oldUeType]= 0;
                    }
                    else
                    {
                        ulAggregateGbrPerTTI_g[internalCellIndex][oldUeType] -= 
                            (ULUEContext->lcInfo[lcCount].ulQosInfo.GBR /           
                             numOfTicksPerSecond) * absFactorOne;
                    }
                    ulAggregateGbrPerTTI_g[internalCellIndex][newUeType] += 
                        (ULUEContext->lcInfo[lcCount].ulQosInfo.GBR /           
                         numOfTicksPerSecond) * absFactorTwo;
                }
            }
            lcCount++;
        }
    }

    return RRM_MAC_SUCCESS;

}


/************************************************************************
 * Function Name    : macReconfigureDlUeGBR
 * Inputs           : 
 * Returns          : 
 * Description      : 
 *
 ************************************************************************/
RrmMacCellConfigErrorCode  macReconfigureDlUeGBR (InternalCellIndex  internalCellIndex, DLUEContext  *ueDLContext_p)
{
    SInt8 gbrLcCount = ueDLContext_p->gbrLcCount;
    UInt8 *gbrLcInfo_p = ueDLContext_p->gbrLcInfo;
    DlLcQosInfo *dlLcQosInfo_p = PNULL;
    InternalCellIndex pCellIndex = ueDLContext_p->internalCellIndex;
    InternalCellIndex sCellIndex = INVALID_CELL_INDEX;
    UInt8 pCellBandWidthDist ;
    UInt8 sCellBandWidthDist ;
    UserType newUeType = ueDLContext_p->userType;
    UInt8 oldUeType = (newUeType == NON_VICTIM_UE?VICTIM_UE:NON_VICTIM_UE);
    UInt8 absCount = 0;
    UDouble32 absFactorOne = 0;
    UDouble32 absFactorTwo = 0;
    UInt32 dlAggGbrOnCell = 0;

#ifdef FDD_CONFIG
    UInt16 numOfTicksPerSecond = NUM_TICKS_PER_SECOND;
#else
    UInt16 numOfTicksPerSecond = getNumTicksPerSecond(DL, internalCellIndex);
#endif

    if (usableAbsCountNew_g[internalCellIndex] > 0)
        absCount = usableAbsCountNew_g[internalCellIndex];
    else
        absCount = 0;


    if  (( VICTIM_UE == oldUeType) && (0 < absCount ) && ( NON_VICTIM_UE == newUeType))
    {
        absFactorOne = (UDouble32) NUM_BITS_ABS_PATTERN/ ( UDouble32 )absCount;
        absFactorTwo = (UDouble32 ) NUM_BITS_ABS_PATTERN/ ( UDouble32) (NUM_BITS_ABS_PATTERN - absCount);
    }
    else if  (( NON_VICTIM_UE == oldUeType) && (0 < absCount ) && ( VICTIM_UE == newUeType))
    {
        absFactorOne = (UDouble32)NUM_BITS_ABS_PATTERN/ ( UDouble32)(NUM_BITS_ABS_PATTERN - absCount);
        absFactorTwo = (UDouble32 )NUM_BITS_ABS_PATTERN/ (UDouble32) absCount;
    }
    else
    {
        return RRM_MAC_SUCCESS;
    }

    while(gbrLcCount--)
    {
        if(ueDLContext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].logicalChannelId != INVALID_LCID)
        {
            dlLcQosInfo_p = 
                &ueDLContext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].dlLcQosInfo;

            pCellBandWidthDist = dlLcQosInfo_p->bandWidthDistribution[PCELL_SERV_CELL_IX];
            sCellBandWidthDist = dlLcQosInfo_p->bandWidthDistribution[START_SCELL_INDEX];

            if( (IS_CA_ENABLED()) && 
                    (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]))
            {
                sCellIndex = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex;
            }

            eicicUpdate_dlAggregateGbrPerTTI_oldUeType(
                    sCellIndex, 
                    pCellIndex,
                    ueDLContext_p,
                    oldUeType,
                    /* +- SPR 17777 */
                    dlLcQosInfo_p,
                    numOfTicksPerSecond,
                    pCellBandWidthDist,
                    sCellBandWidthDist,
                    absFactorOne);
            dlAggGbrOnCell = (dlLcQosInfo_p->GBR / NUM_TICKS_PER_SECOND) * (absFactorOne);
            if(totalDlAggregateGbrPerTTI_g[oldUeType] < dlAggGbrOnCell)
            {
                totalDlAggregateGbrPerTTI_g[oldUeType] = 0;
            }
            else
            {
                totalDlAggregateGbrPerTTI_g[oldUeType] -= dlAggGbrOnCell;
            }

            eicicUpdate_dlAggregateGbrPerTTI_newUeType(
                    sCellIndex, 
                    pCellIndex,
                    ueDLContext_p,
                    newUeType,
                    dlLcQosInfo_p,
                    numOfTicksPerSecond,
                    pCellBandWidthDist,
                    sCellBandWidthDist,
                    absFactorTwo);
            totalDlAggregateGbrPerTTI_g[newUeType] += (dlLcQosInfo_p->GBR / numOfTicksPerSecond)* (absFactorTwo);
        }
    }

    return RRM_MAC_SUCCESS;
}


/************************************************************************
 * Function Name    : eicicUpdate_dlAggregateGbrPerTTI_oldUeType
 * Inputs           : 
 * Returns          : None
 * Description      : 
 *
 ************************************************************************/
void eicicUpdate_dlAggregateGbrPerTTI_oldUeType(
        InternalCellIndex sCellIndex, 
        InternalCellIndex pCellIndex,
        DLUEContext  *ueDLContext_p,
        UInt8 oldUeType,
        /* +- SPR 17777 */
        DlLcQosInfo *dlLcQosInfo_p,
        UInt16 numOfTicksPerSecond,
        UInt8 pCellBandWidthDist,
        UInt8 sCellBandWidthDist,
        UDouble32 absFactorOne)
{
    UInt32 dlAggGbrOnCell =0;
    if ( (sCellIndex != INVALID_CELL_INDEX)  &&  IS_SCELL_ACTIVE(ueDLContext_p->ueIndex, START_SCELL_INDEX) )    
    {
        dlAggGbrOnCell = (((dlLcQosInfo_p->GBR / numOfTicksPerSecond) * pCellBandWidthDist)/100 ) * (absFactorOne);
        if(dlAggregateGbrPerTTI_g[pCellIndex][oldUeType] < dlAggGbrOnCell)
        {
            dlAggregateGbrPerTTI_g[pCellIndex][oldUeType] = 0;
        }
        else
        {
            dlAggregateGbrPerTTI_g[pCellIndex][oldUeType] -= dlAggGbrOnCell;
        }
        dlAggGbrOnCell = (((dlLcQosInfo_p->GBR / numOfTicksPerSecond) * sCellBandWidthDist)/100 ) * (absFactorOne);
        if(dlAggregateGbrPerTTI_g[sCellIndex][oldUeType] < dlAggGbrOnCell)
        {
            dlAggregateGbrPerTTI_g[sCellIndex][oldUeType] = 0;
        }
        else
        {
            dlAggregateGbrPerTTI_g[sCellIndex][oldUeType] -= dlAggGbrOnCell;
        }
    }
    else
    {
        dlAggGbrOnCell = (dlLcQosInfo_p->GBR / numOfTicksPerSecond)* (absFactorOne);
        if(dlAggregateGbrPerTTI_g[pCellIndex][oldUeType] < dlAggGbrOnCell)
        {
            dlAggregateGbrPerTTI_g[pCellIndex][oldUeType] = 0;
        }
        else
        {
            dlAggregateGbrPerTTI_g[pCellIndex][oldUeType] -= dlAggGbrOnCell;
        }
    }
}


/************************************************************************
 * Function Name    : eicicUpdate_dlAggregateGbrPerTTI_newUeType
 * Inputs           : 
 * Returns          : None
 * Description      : 
 *
 ************************************************************************/
void eicicUpdate_dlAggregateGbrPerTTI_newUeType(
        InternalCellIndex sCellIndex, 
        InternalCellIndex pCellIndex,
        DLUEContext  *ueDLContext_p,
        UserType newUeType,
        DlLcQosInfo *dlLcQosInfo_p,
        UInt16 numOfTicksPerSecond,
        UInt8 pCellBandWidthDist,
        UInt8 sCellBandWidthDist,
        UDouble32 absFactorTwo)
{
    if( (sCellIndex != INVALID_CELL_INDEX) && IS_SCELL_ACTIVE(ueDLContext_p->ueIndex, START_SCELL_INDEX))    
    {
        dlAggregateGbrPerTTI_g[pCellIndex][newUeType] += (((dlLcQosInfo_p->GBR / numOfTicksPerSecond) *
                    pCellBandWidthDist)/100 ) * (absFactorTwo);
        dlAggregateGbrPerTTI_g[sCellIndex][newUeType] += (((dlLcQosInfo_p->GBR / numOfTicksPerSecond) *
                    sCellBandWidthDist)/100 ) * (absFactorTwo);
    }
    else
    {
        dlAggregateGbrPerTTI_g[pCellIndex][newUeType] += (dlLcQosInfo_p->GBR / numOfTicksPerSecond) * (absFactorTwo);

    }
}

/* EICIC -*/


/************************************************************************
 * Function Name    : initResourceInfo
 * Inputs           : rrmMacCellConfigReq_p pointer to structure RrmMacCellConfigReq
 *                    internalCellIndex
 * Outputs          :
 * Returns          : MAC_SUCCESS 
 * Description      : This function invokes two functions for DL and UL
 *                    resource information as per bitmask
 ************************************************************************/
UInt32 initResourceInfo(RrmMacCellConfigReq *rrmMacCellConfigReq_p,
                           InternalCellIndex  internalCellIndex)
{   
    UInt32 retVal = MAC_SUCCESS;
    /*EICIC +*/
    UInt32 bitmask =rrmMacCellConfigReq_p->bitmask;
    UInt8	absPatternIndexTemp = !absPatternIndex_g[internalCellIndex];
    UInt8	usableAbsPatternIndexTemp = !usableAbsPatternIndex_g[internalCellIndex];
    /*EICIC -*/

    DLResourcePartitionConfigInfo resourseConfigInfoDL = {0};
    ULResourcePartitionConfigInfo resourseConfigInfoUL = {0};

    if(bitmask == 0)
    {
        LOG_MAC_MSG(MAC_RRM_INVALID_CELL_CONFIG_BITMASK,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                bitmask,LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        return INVALID_CELL_CONFIG_BITMASK;
    }

    if(DL_RESOURCE_INFO_BITMASK & rrmMacCellConfigReq_p->bitmask) 
    {
        /* if previous reconfiguration is not complete yet then return error*/
        if(rrmMacCellGlobal_gp[internalCellIndex]->dlResourceReconfigFlag)
        {
            return RETRY_AFTER_SOME_TIME;
        }

        retVal = initMacDLResourceInfo(
                &rrmMacCellConfigReq_p->dlResourcePartitionInfo,
                &resourseConfigInfoDL, internalCellIndex);

        if(MAC_SUCCESS != retVal)
        {
            return retVal;
        }

        /* configuring DL RA based on partition info */
        retVal = configureDLResourceAllocationInfo(&resourseConfigInfoDL, internalCellIndex);

        if(MAC_SUCCESS != retVal)
        {
            return retVal;
        }
    }

    if(UL_RESOURCE_INFO_BITMASK & rrmMacCellConfigReq_p->bitmask)
    {
        /*If previous reconfiguration is not complete then return error*/
        if(rrmMacCellGlobal_gp[internalCellIndex]->ulResourceReconfigFlag)
        {
            return RETRY_AFTER_SOME_TIME;
        }
        retVal = initMacULResourceInfo(
                &rrmMacCellConfigReq_p->ulResourcePartitionInfo,
                &resourseConfigInfoUL, internalCellIndex);

        if(MAC_SUCCESS != retVal)
        {
            return retVal;
        }

        retVal = configureULResourceAllocationInfo(&resourseConfigInfoUL,
                internalCellIndex);

        if(MAC_SUCCESS != retVal)
        {
            return retVal;
        }
    }

    if(PERIODIC_REPORT_CONFIG_BITMASK & rrmMacCellConfigReq_p->bitmask)
    {
        /* Coverity 54143 */
        if ( RRM_MAC_SUCCESS != reconfigPeriodicReport(
                    &rrmMacCellConfigReq_p->periodicReportConfig, 
                    internalCellIndex))
        {
            return MAC_SUCCESS;
        }
        /* Coverity 54143 */
    }

    /* +DYNAMIC_ICIC */
    if(L1_REPORT_CONFIG_BITMASK & rrmMacCellConfigReq_p->bitmask)
    {
        if ( RRM_MAC_SUCCESS != configureLayer1Report(
                    &rrmMacCellConfigReq_p->layer1MeasReportConfig, 
                    internalCellIndex))
        {
            return MAC_SUCCESS;
        }
    }

    if(ICIC_REPORT_CONFIG_BITMASK & rrmMacCellConfigReq_p->bitmask)
    {
        if ( RRM_MAC_SUCCESS != configureICICReport(
                    &rrmMacCellConfigReq_p->layer2IcicMeasReportConfig, 
                    internalCellIndex))
        {
            return MAC_SUCCESS;
        }
    }

    if(CONFIG_UL_POWER_BITMASK & rrmMacCellConfigReq_p->bitmask)
    {
        retVal = validateULPowerMask(&(rrmMacCellConfigReq_p->ulPowerMask));
        if(MAC_SUCCESS == retVal)
        {
            if(MAC_FAILURE == 
                    configULPowerMaskInfo(&(rrmMacCellConfigReq_p->ulPowerMask), internalCellIndex))
            {
                return MAC_FAILURE;
            }
        }
        else
        {
            return PARAMETER_OUT_OF_RANGE;
        }

    }
    /* -DYNAMIC_ICIC */
    if(DL_RESOURCE_INFO_BITMASK & rrmMacCellConfigReq_p->bitmask) 
    {
        /* Copying Local structure to the global one using memCpy */
        memCpy(&dlResourcePartitionConfig_g[internalCellIndex],&resourseConfigInfoDL,sizeof(DLResourcePartitionConfigInfo));

        /* LOG for CC region */
        LOG_UT(MAC_RRM_DL_CC_INFO,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                dlResourcePartitionConfig_g[internalCellIndex].cellCenterRegionCount,
                dlResourcePartitionConfig_g[internalCellIndex].ccResourceInfo[0].startRBIndex,
                dlResourcePartitionConfig_g[internalCellIndex].ccResourceInfo[0].endRBIndex,
                dlResourcePartitionConfig_g[internalCellIndex].ccResourceInfo[1].startRBIndex,
                dlResourcePartitionConfig_g[internalCellIndex].ccResourceInfo[1].endRBIndex,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        /* LOG for total available RB*/
        LOG_UT(MAC_RRM_DL_CC_INFO_TOTAL_RB,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                dlResourcePartitionConfig_g[internalCellIndex].totalAvailableRBDL,
                LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        /* LOG for CE region */
        LOG_UT(MAC_RRM_DL_CE_INFO,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                dlResourcePartitionConfig_g[internalCellIndex].cellEdgeRegionCount,
                dlResourcePartitionConfig_g[internalCellIndex].ceResourceInfo.startRBIndex,
                dlResourcePartitionConfig_g[internalCellIndex].ceResourceInfo.endRBIndex,
                dlResourcePartitionConfig_g[internalCellIndex].totalCellEdgeRBDL,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        /* Set the bitmask indicating that fields successfully updated */
        rrmMacCellGlobal_gp[internalCellIndex]->dlResourceReconfigFlag = TRUE;
    }

    if(UL_RESOURCE_INFO_BITMASK & rrmMacCellConfigReq_p->bitmask)
    {
        /* Copying Local structure to the global one using memCpy() */
        memCpy(ulResourcePartitionConfig_gp[internalCellIndex],&resourseConfigInfoUL,sizeof(ULResourcePartitionConfigInfo));

        /* LOG for CC region */
        LOG_UT(MAC_RRM_UL_CC_INFO,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                ulResourcePartitionConfig_gp[internalCellIndex]->cellCenterRegionCount,
                ulResourcePartitionConfig_gp[internalCellIndex]->ccResourceInfo[0].startRBIndex,
                ulResourcePartitionConfig_gp[internalCellIndex]->ccResourceInfo[0].endRBIndex,
                ulResourcePartitionConfig_gp[internalCellIndex]->ccResourceInfo[1].startRBIndex,
                ulResourcePartitionConfig_gp[internalCellIndex]->ccResourceInfo[1].endRBIndex,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        /* LOG for total available RB*/
        LOG_UT(MAC_RRM_UL_TOTAL_RB,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                ulResourcePartitionConfig_gp[internalCellIndex]->totalAvailableRBUL,
                LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        /* LOG for CE region */
        LOG_UT(MAC_RRM_UL_CE_INFO,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                ulResourcePartitionConfig_gp[internalCellIndex]->cellEdgeRegionCount,
                ulResourcePartitionConfig_gp[internalCellIndex]->ceResourceInfo.startRBIndex,
                ulResourcePartitionConfig_gp[internalCellIndex]->ceResourceInfo.endRBIndex,
                ulResourcePartitionConfig_gp[internalCellIndex]->totalCellEdgeRBUL,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        /* Set the bitmask indicating that fields successfully updated */
        rrmMacCellGlobal_gp[internalCellIndex]->ulResourceReconfigFlag = TRUE;
    }
    /*EICIC +*/
    retVal = eicicInitReportInfo(
            internalCellIndex,
            absPatternIndexTemp,
            usableAbsPatternIndexTemp,
            rrmMacCellConfigReq_p);
    /*EICIC -*/
    return retVal;
}


/************************************************************************
 * Function Name    : eicicInitReportInfo 
 * Inputs           : absPatternIndexTemp,usableAbsPatternIndexTemp,rrmMacCellConfigReq_p
 *                    internalCellIndex
 * Outputs          : report info 
 * Returns          : MAC_SUCCESS 
 * Description      : This function initialize the report info for 
 *                    resource information as per bitmask
 ************************************************************************/
UInt32 eicicInitReportInfo(
	InternalCellIndex internalCellIndex,
	UInt8 absPatternIndexTemp,
	UInt8 usableAbsPatternIndexTemp,
	RrmMacCellConfigReq *rrmMacCellConfigReq_p)
{
    UInt32 retVal = MAC_SUCCESS;
    UInt8 absPatternLoop;
    if(EICIC_REPORT_CONFIG_BITMASK & rrmMacCellConfigReq_p->bitmask)
    {
        retVal = configureEICICReport(
                &rrmMacCellConfigReq_p->layer2EicicMeasReportConfig, 
                internalCellIndex);

        if ( RRM_MAC_SUCCESS != retVal)
        {
            return retVal;
        }
    }
    if(ABS_PATTERN_CONFIG_BITMASK & rrmMacCellConfigReq_p->bitmask)
    {

        if (rrmMacCellGlobal_gp[internalCellIndex]->configureAbsPatternFlag)
        {
            return RETRY_AFTER_SOME_TIME;
        }
        if ( RRM_MAC_SUCCESS != configureABSpattern(
                    &rrmMacCellConfigReq_p->absPatternServingCellConfig, internalCellIndex))
        {

            for ( absPatternLoop =0; absPatternLoop < NUM_BITS_ABS_PATTERN; absPatternLoop++)
                absPattern_g[internalCellIndex][absPatternIndexTemp][absPatternLoop] =  0;
            return INVALID_ABS_PATTERN_CONFIG;
        }


        if (!((LTE_FALSE == isEicicEnabled_g[internalCellIndex] ) && !(isConfigureAbsPatternSet_g[internalCellIndex])))
        {
            rrmMacCellGlobal_gp[internalCellIndex]->configureAbsPatternFlag = TRUE;
        }
    }

    if(USABLE_ABS_PATTERN_CONFIG_BITMASK & rrmMacCellConfigReq_p->bitmask)
    {
        if (rrmMacCellGlobal_gp[internalCellIndex]->configureUsableAbsPatternFlag)
        {
            return RETRY_AFTER_SOME_TIME;
        }
        if ( RRM_MAC_SUCCESS != configureUsableABSpattern((
                        &rrmMacCellConfigReq_p->usableAbsPatternServingCellConfig), internalCellIndex))
        {
            for ( absPatternLoop =0; absPatternLoop < NUM_BITS_ABS_PATTERN; absPatternLoop++)
                usableAbsPattern_g[internalCellIndex][usableAbsPatternIndexTemp][absPatternLoop] = 0;
            return INVALID_USABLE_ABS_PATTERN_CONFIG;
        }

        if (!((LTE_FALSE == isEicicEnabled_g[internalCellIndex] ) && !(isUsablePatternSet_g[internalCellIndex])))
        {
            rrmMacCellGlobal_gp[internalCellIndex]->configureUsableAbsPatternFlag = TRUE;
        }

    }
    if ((isUsablePatternSet_g[internalCellIndex])  || (isConfigureAbsPatternSet_g[internalCellIndex]))
    { 
        if (LTE_FALSE == isEicicEnabled_g[internalCellIndex])
        {
            applyAbsConfiguration_g[internalCellIndex] = LTE_FALSE; 
            startAbsDelay_g[internalCellIndex] = LTE_TRUE; 
        }
    }
    return retVal;
}

/*UL_MU_MIMO_CHG Start*/

/************************************************************************
 * Function Name    : reconfigUEUlMUMIMO
 * Inputs           : rrmMacUeReconfigReq_p,buffer_p,internalCellIndex 
 * Returns          : MAC_SUCCESS/MAC_FAILURE 
 * Description      : This function validates UEcontext and Pending Delete
 *                    check and reconfigures UEs MUMIMO Status               
 ************************************************************************/
UInt32 reconfigUEUlMUMIMO(RrmMacUeReconfigReq *rrmMacUeReconfigReq_p,
                              UInt8 *buffer_p, 
                              InternalCellIndex  internalCellIndex)
{
    UEULMUMIMOErrorInfo *ueULMUMIMOErrorInfo_p  = PNULL;
    UEULMUMIMOInfoList *ueULMUMIMOInfoList_p    = PNULL;
    ULUEContextInfo  *ueULContextInfo_p         = PNULL;
    ULUEContext *ueULContext_p                  = PNULL;
    UEULMUMIMOError *ulUEMUMMIMOError_p         = PNULL;
    UEULMUMIMOInfoNode *ulUEMUMIMONode_p        = PNULL;
    RrmMacUEReConfigCnf *rrmMacUEReconfigCnf_p  = PNULL;

    UInt32 numOfUeWithMUMIMOError               = 0;
    UInt32 index                                = 0;
    UInt32 ueIndex                              = 0;
    UInt32 rrmUeIndex                           = 0;
    UInt32 ueMUMIMOStatus                       = UL_MU_MIMO_DISABLED;
    UInt32 retVal                                = MAC_SUCCESS;
    /*ca changes start*/
    CellConfigParams *cellParams_p = NULL;
    cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p \
                   [internalCellIndex]->cellParams_p;
    /* ca changes end */
    ueULMUMIMOInfoList_p = &rrmMacUeReconfigReq_p->ueUlMUMIMOInfoList;
    rrmMacUEReconfigCnf_p = (RrmMacUEReConfigCnf *) buffer_p;
    ueULMUMIMOErrorInfo_p = (UEULMUMIMOErrorInfo *) &rrmMacUEReconfigCnf_p->ueULMUMIMOErrorInfo;

    /*return INVALID_NUM_OF_UE if numOfUE is greater than MAX_NUM_OF_UE*/
    if (rrmMacUeReconfigReq_p->ueUlMUMIMOInfoList.numOfUE > MAX_NUM_OF_UE)
    {
        return INVALID_NUM_OF_UE;
    }
    for (index = 0; index < rrmMacUeReconfigReq_p->ueUlMUMIMOInfoList.numOfUE; index++)
    {
        ulUEMUMMIMOError_p = (UEULMUMIMOError *)(ueULMUMIMOErrorInfo_p->ueULMUMIMOError + 
                numOfUeWithMUMIMOError);
        rrmUeIndex = ueULMUMIMOInfoList_p->ueUlMUMIMOInfo[index].ueIndex;
        /*Fix start of spr5940*/
        ueIndex = getUeIdxFromRrcUeIdx(rrmUeIndex);
        /*Fix ends of spr5940*/        
        ueMUMIMOStatus = ueULMUMIMOInfoList_p->ueUlMUMIMOInfo[index].ulMUMIMOStatus;
        /*check for Invlaid UE Index*/
        if (ueIndex >= MAX_UE_SUPPORTED)
        {
            ulUEMUMMIMOError_p->ueIndex   = rrmUeIndex;
            ulUEMUMMIMOError_p->errorCode = INVALID_UE_INDEX_ERROR;
            numOfUeWithMUMIMOError++;

            lteWarning ("Invalid UE index \n");
            continue;
        }

        ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        ueULContext_p = ueULContextInfo_p->ulUEContext_p;

        /*If Pending flag is set for this UE, than put this UE in MUMIMOErrorInfo*/
        if (!ueULContext_p || ueULContextInfo_p->pendingDeleteFlag || \
                internalCellIndex != ueULContext_p->internalCellIndex)
        {
            ulUEMUMMIMOError_p->ueIndex = rrmUeIndex;
            ulUEMUMMIMOError_p->errorCode = UE_DOES_NOT_EXIST_ERROR;
            LOG_UT(MAC_RRM_UE_DOES_NOT_EXIST,LOGDEBUG,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueULContextInfo_p->pendingDeleteFlag,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            /*Incerement UE MUMIMO error count*/
            numOfUeWithMUMIMOError++;
            continue;
        }

        /*check for Invalid MU MIMO Status*/
        else if (ueMUMIMOStatus > UL_MU_MIMO_ENABLED)
        {
            ulUEMUMMIMOError_p->ueIndex   = rrmUeIndex;
            ulUEMUMMIMOError_p->errorCode = INVALID_MU_MIMO_STATUS_TYPE;
            LOG_UT(MAC_RRM_INVALID_MU_MIMO_STATUS,LOGDEBUG,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueMUMIMOStatus,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            /*Incerement UE MUMIMO error count*/
            numOfUeWithMUMIMOError++;
            continue;
        }
        else if ( (NUM_OF_TX_ANTENNAS_1 == cellParams_p->numOfTxAnteenas) && 
                (UL_MU_MIMO_ENABLED == ueMUMIMOStatus) )
        {
            ulUEMUMMIMOError_p->ueIndex   = rrmUeIndex;
            ulUEMUMMIMOError_p->errorCode = INVALID_MU_MIMO_NUM_ANTENNA_CONFIG;
            LOG_UT(MAC_RRM_INVALID_MU_MIMO_NUM_ANTENNA_CONFIG,LOGDEBUG,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    cellParams_p->numOfTxAnteenas,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            /*Incerement UE MUMIMO error count*/
            numOfUeWithMUMIMOError++;
            continue;
        }

        /*If all validation gets success than push this UE in ulUEReconfigMUMIMOQueue*/
        else
        {
            GET_MEM_FROM_POOL(UEULMUMIMOInfoNode, ulUEMUMIMONode_p, sizeof(UEULMUMIMOInfoNode), PNULL);
            if( PNULL == ulUEMUMIMONode_p )
            {
                
                /* SPR 9813 START */
                ulUEMUMMIMOError_p->ueIndex   = rrmUeIndex;
                ulUEMUMMIMOError_p->errorCode = MAC_RRM_SYSTEM_ERROR;
                numOfUeWithMUMIMOError++;
                continue;
                /* SPR 9813 END */
            }
            /*-COVERITY 5.3.0 - 32115*/
            ulUEMUMIMONode_p->ueIndex =  ueIndex;
            ulUEMUMIMONode_p->ulMUMIMOStatus = ueMUMIMOStatus;
            if(MAC_FAILURE == 
                    pushNodeCircQ(&rrmMacCellGlobal_gp[internalCellIndex]-> \
                        ulUEReconfigMUMIMOQueue,ulUEMUMIMONode_p))
            {
                freeMemPool(ulUEMUMIMONode_p);
                /* SPR 9813 START */
                ulUEMUMMIMOError_p->ueIndex   = rrmUeIndex;
                ulUEMUMMIMOError_p->errorCode = MAC_RRM_SYSTEM_ERROR;
                numOfUeWithMUMIMOError++;
                continue;
                /* SPR 9813 END */
            }
        }
    }
    /* Update numOfUe for which MUMIMO reconfiguration failed */
    ueULMUMIMOErrorInfo_p->numOfUE = numOfUeWithMUMIMOError;

    /* Review Comment Fix -- This should not reurn failure 
     * if there is no UE in the list for updating the UL MIMO status */
    if (numOfUeWithMUMIMOError > 0) 
    {
        if (numOfUeWithMUMIMOError < rrmMacUeReconfigReq_p->ueUlMUMIMOInfoList.numOfUE)
        {
            retVal = MAC_PARTIAL_SUCCESS;
            /* SPR 10970 fix + */
            rrmMacUEReconfigCnf_p->bitmask |= UE_MUMIMO_RECONFIG_ERROR_INFO_BITMASK; 
            /* SPR 10970 fix - */
        }
        else
        {
            /* This is the case when all the MUMIMO reconfiguration fails */
            retVal = MAC_FAILURE;
            /* SPR 10970 fix + */
            rrmMacUEReconfigCnf_p->bitmask |= UE_MUMIMO_RECONFIG_ERROR_INFO_BITMASK; 
            /* SPR 10970 fix - */
        }
    }
    else
    {
        retVal = MAC_SUCCESS;
    }
    return retVal;
}

/*UL_MU_MIMO_CHG End*/
/************************************************************************
 * Function Name    : reconfigUELocationType
 * Inputs           : rrmMacUeReconfigReq_p,buffer_p,internalCellIndex
 * Returns          : MAC_SUCCESS/MAC_FAILURE 
 * Description      : This function validates uecontext and pending delete
 *                    check and reconfigures user location type.
 ************************************************************************/
UInt32 reconfigUELocationType(
        RrmMacUeReconfigReq *rrmMacUeReconfigReq_p, 
        UInt8 *buffer_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 retVal = MAC_SUCCESS;
    UInt32 index = 0;
    UInt32 ueIndex = 0;
    UInt32 rrmUeIndex = 0;
    UELocationInfo *ueLocationInfo_p = PNULL;
    UInt32 bitmask = 0;
    UInt32 dlUELocationType = 0;
    UInt32 ulUELocationType = 0;
    UInt32 ueUpdateFlag = FALSE;
    UInt32 numOfUeWithError = 0;

    UELocationError *ueLocationError_p = PNULL;
    UELocationNode *ueLocationNodeDL_p = PNULL; 
    UELocationNode *ueLocationNodeUL_p = PNULL; 
    DLUEContext       *dlUEContext_p = PNULL;
    DLUEContextInfo   *dlUEContextInfo_p = PNULL;
    UELocationErrorInfo *ueLocationErrorInfo_p = PNULL;
    RrmMacUEReConfigCnf *rrmMacUEReconfigCnf_p = PNULL;
    /* CA changes start*/
    UInt8 cellBitMask               = 0;
    UInt8 scellLocationUpdateFlag   = FALSE;
    UInt8 loopCount                 = 0;
    bitmask = rrmMacUeReconfigReq_p->bitmask;
    ueLocationInfo_p = &rrmMacUeReconfigReq_p->ueLocationInfo;
    /* CA changes end*/

    if (UE_LOCATION_RECONFIG_BIT & bitmask)
    {   
        rrmMacUEReconfigCnf_p = (RrmMacUEReConfigCnf *) buffer_p;
        ueLocationErrorInfo_p = (UELocationErrorInfo *) &rrmMacUEReconfigCnf_p->ueLocationErrorInfo;

        /* check for max num of UEs */
        if (rrmMacUeReconfigReq_p->ueLocationInfo.numOfUE > MAX_NUM_OF_UE)
        {
            LOG_UT(MAC_RRM_TOTAL_UE,LOGDEBUG,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    rrmMacUeReconfigReq_p->ueLocationInfo.numOfUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");

            return INVALID_NUM_OF_UE;
        }

        for (index = 0; index < rrmMacUeReconfigReq_p->ueLocationInfo.numOfUE; index++)
        {
            ueLocationError_p = (UELocationError *)(ueLocationErrorInfo_p->ueLocationError + 
                    numOfUeWithError);

            rrmUeIndex = ueLocationInfo_p->ueLocation[index].ueIndex;
            ueIndex =  getUeIdxFromRrcUeIdx(rrmUeIndex);

            if (MAX_UE_SUPPORTED <= ueIndex )
            {
                ueLocationError_p->ueIndex   = rrmUeIndex;
                ueLocationError_p->errorCode = INVALID_UE_INDEX_ERROR;
                numOfUeWithError++;

                lteWarning ("Invalid UE index \n");
                continue;
            }
            dlUELocationType = ueLocationInfo_p->ueLocation[index].ueLocationType;
            ulUELocationType = ueLocationInfo_p->ueLocation[index].ueLocationType;

            dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

            /* Validation for UE Context */
            if (!dlUEContext_p || dlUEContextInfo_p->pendingDeleteFlag || \
                    internalCellIndex != dlUEContext_p->internalCellIndex)
            {
                ueLocationError_p->ueIndex = rrmUeIndex;
                ueLocationError_p->errorCode = UE_DOES_NOT_EXIST_ERROR;
                LOG_UT(MAC_RRM_UE_DOES_NOT_EXIST,LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        dlUEContextInfo_p->pendingDeleteFlag,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");

                numOfUeWithError++;
                continue;
            }
            else if (dlUELocationType >= INVALID_USER_LOCATION_TYPE)
            {
                ueLocationError_p->ueIndex = rrmUeIndex;
                ueLocationError_p->errorCode = INVALID_USER_LOCATION_TYPE_ERROR;
                numOfUeWithError++;

                LOG_UT(MAC_RRM_INVALID_USER_LOCATION_TYPE_ERROR,LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        dlUELocationType,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");

                continue;
            }
            /* CA changes start*/
            else if(ueLocationInfo_p->ueLocation[index].cellBitMask)
            {
                cellBitMask = ueLocationInfo_p->ueLocation[index].cellBitMask;

                checkCellBitMask(rrmUeIndex,
                        cellBitMask,
                        ueLocationInfo_p,
                        dlUEContext_p,
                        &scellLocationUpdateFlag,
                        index,
                        ueLocationError_p,
                        &numOfUeWithError,
                        internalCellIndex);
                if (FALSE == scellLocationUpdateFlag)
                {
                    /*validation of UE location for secll failed,
                     * error code updated in response buffer, move to next UE*/
                    continue;
                }
            }
            else
            {
                /* ueLocationInfo_p->ueLocation[index].cellBitMask is zero,
                 * this implies that this reconfig message doesn't contain 
                 * location reconfiguration of SCELL for this UE*/
            }
            /* CA changes end */

            if ((dlUELocationType == CE_USER) && 
                    (dlResourcePartitionConfig_g[internalCellIndex].cellEdgeRegionCount == 0))
            {
                ueLocationError_p->ueIndex = rrmUeIndex;
                ueLocationError_p->errorCode = NO_DL_CELL_EDGE_REGION_EXIST;
                LOG_UT(MAC_RRM_NO_DL_CELL_EDGE_REGION_EXIST,LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        ueLocationError_p->errorCode,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
            }
            else
            {
                GET_MEM_NODE_DL_UE_RECONFIG_DATA_Q(ueLocationNodeDL_p,UELocationNode);
                if (PNULL == ueLocationNodeDL_p)
                {
                    /* SPR 9813 START */
                    ueLocationError_p->ueIndex   = rrmUeIndex;
                    ueLocationError_p->errorCode = MAC_RRM_UE_DOES_NOT_EXIST;
                    numOfUeWithError++;
                    continue;
                    /* SPR 9813 END */
                }
                ueLocationNodeDL_p->ueIndex =  ueIndex;
                ueLocationNodeDL_p->ueLocationType = dlUELocationType;
                /*CA changes start*/
                if (scellLocationUpdateFlag == TRUE)
                {
                    ueLocationNodeDL_p->cellBitMask = ueLocationInfo_p-> \
                                                      ueLocation[index].cellBitMask;
                    for(loopCount = START_SCELL_INDEX; 
                            loopCount <= MAX_NUM_SCELL; loopCount++)
                    {
                        ueLocationNodeDL_p->ueLocationPerScell[loopCount -1] =
                            ueLocationInfo_p->ueLocation[index]. \
                            ueLocationPerScell[loopCount - 1];
                    }
                }
                /*Enqueue UE location reconfig information of SCELL and PCELL 
                 * of a UE in PCELL queue,PCELL will update these information 
                 * to respective SCELLS */
                /*CA changes end*/
                /* If DL updation fails , no need to move for UL */
                if(MAC_FAILURE == ENQUEUE_DL_UE_RECONFIG_DATA_Q(
                            UELocationNode,ueLocationNodeDL_p, internalCellIndex))
                {
                    /* SPR 9813 START */
                    ueLocationError_p->ueIndex   = rrmUeIndex;
                    ueLocationError_p->errorCode = MAC_RRM_UE_DOES_NOT_EXIST;
                    numOfUeWithError++;
                    FREE_MEM_NODE_DL_UE_RECONFIG_DATA_Q(ueLocationNodeDL_p);
                    continue;
                    /* SPR 9813 END */
                }
                /* SPR 9813 START */
                ueUpdateFlag = TRUE;
                /* SPR 9813 END */
            }

            if ((ulUELocationType == CE_USER) && 
                    (ulResourcePartitionConfig_gp[internalCellIndex]->cellEdgeRegionCount == 0))
            {
                ueLocationError_p->ueIndex = rrmUeIndex;
                if (ueLocationError_p->errorCode == NO_DL_CELL_EDGE_REGION_EXIST)
                {
                    ueLocationError_p->errorCode = NO_DL_UL_CELL_EDGE_REGION_EXIST;
                }
                else
                {
                    ueLocationError_p->errorCode = NO_UL_CELL_EDGE_REGION_EXIST;
                }
                LOG_UT(MAC_RRM_NO_UL_CELL_EDGE_REGION_EXIST,LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        ueLocationError_p->errorCode,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
            }
            else
            {
                GET_MEM_NODE_UL_UE_RECONFIG_DATA_Q(ueLocationNodeUL_p,UELocationNode);
                if (PNULL == ueLocationNodeUL_p)
                {
                    /* SPR 9813 START */
                    ueLocationError_p->ueIndex   = rrmUeIndex;
                    ueLocationError_p->errorCode = MAC_RRM_UE_DOES_NOT_EXIST;
                    numOfUeWithError++;
                    continue;
                    /* SPR 9813 END */
                }
                else
                {
                    ueLocationNodeUL_p->ueIndex =  ueIndex;
                    ueLocationNodeUL_p->ueLocationType = ulUELocationType;
                    if(MAC_FAILURE == ENQUEUE_UL_UE_RECONFIG_DATA_Q(UELocationNode,ueLocationNodeUL_p,internalCellIndex))
                    {
                        ueLocationError_p->ueIndex   = rrmUeIndex;
                        ueLocationError_p->errorCode = MAC_RRM_UE_DOES_NOT_EXIST;
                        FREE_MEM_NODE_UL_UE_RECONFIG_DATA_Q(ueLocationNodeUL_p);
                        numOfUeWithError++;
                        continue;
                        /* SPR 9813 END */
                    }
                }
                ueUpdateFlag = TRUE;
            }
            if (ueLocationError_p->errorCode > 0)
            {
                numOfUeWithError++;
            }
            else
            {
                LOG_MAC_MSG(MAC_RRM_UE_LOC_INFO,LOGDEBUG,MAC_RRM_INF,\
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),\
                        ueLocationNodeDL_p->ueIndex,
                        ueLocationNodeDL_p->ueLocationType,\
                        ueLocationNodeUL_p->ueIndex,\
                        ueLocationNodeUL_p->ueLocationType,\
                        LINE_NUMBER,\
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                        __func__,"");
            }
        }

        /* Update numOfUe for which user location configuration failed */
        ueLocationErrorInfo_p->numOfUE = numOfUeWithError;

        /* sizeof(UInt32) is for size of numOfUE*/
        if (ueUpdateFlag ==  FALSE)
        {
            /* This is the case when all the ueLocation configuration fails */
            retVal = MAC_FAILURE;
            /* SPR 10970 fix + */
            rrmMacUEReconfigCnf_p->bitmask |= UE_LOCATION_ERROR_INFO_BITMASK; 
            /* SPR 10970 fix - */
        }
        else if (numOfUeWithError)
        {
            /* This is the case when some of the ueLocation configuration fails */
            retVal = MAC_PARTIAL_SUCCESS;

            /* SPR 10970 fix + */
            rrmMacUEReconfigCnf_p->bitmask |= UE_LOCATION_ERROR_INFO_BITMASK; 
            /* SPR 10970 fix - */
        }
        else
        {
            retVal = MAC_SUCCESS;
        }

    }
    return retVal;
}
/*CA changes start*/

/************************************************************************
 * Function Name    : macReconfigUECAConfigInfoList
 * Inputs           : rrmMacUeReconfigReq_p,buffer_p, internalCellIndex
 * Returns          : MAC_SUCCESS 
 * Description      : This function validates uecontext and pending delete
 *  *                    check and reconfigures user CA Config Info
 *                 
 ************************************************************************/
UInt32 macReconfigUECAConfigInfoList(
                            RrmMacUeReconfigReq *rrmMacUeReconfigReq_p, 
                            UInt8 *buffer_p,
                            InternalCellIndex internalCellIndex)
{
    UECarrierAggConfigErrorInfo *ueCarrierAggConfigErrorInfo_p  = PNULL;
    UECarrierAggConfigInfoList *ueCarrierAggConfigInfoList_p    = PNULL;
    DLUEContext *ueDLContext_p                                  = PNULL;
    DLUEContextInfo  *ueDLContextInfo_p                         = PNULL;
    UECarrierAggConfigError *ueCarrierAggConfigError_p          = PNULL;
    UEScellSchedulingInfoNode *ueScellSchedulingInfoNode_p      = PNULL;
    RrmMacUEReConfigCnf *rrmMacUEReconfigCnf_p                  = PNULL;
    UEScellSchedulingInfo *ueScellSchedulingInfo_p              = PNULL;
    UInt32 numOfUeCarrierAggConfigError                         = 0;
    UInt32 index                                                = 0;
    UInt32 ueIndex                                              = 0;
    UInt32 rrmUeIndex                                           = 0;
    UInt32 ueUpdateFlag                                         = FALSE;
    UInt32 retVal                                               = MAC_SUCCESS;
    UInt8 isMACCESendToUE                                       = 0;
    UInt8 numOfSCellSchedulingAllowed                           = 0;
    UInt8 numOfSCellSchedulingNotAllowed                        = 0;

    /*return INVALID_NUM_OF_UE if numOfUE is greater than MAX_NUM_OF_UE*/
    if (rrmMacUeReconfigReq_p->ueCarrierAggConfigInfoList.numOfUE > 
            MAX_NUM_OF_UE)
    {
        LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                rrmMacUeReconfigReq_p->ueCarrierAggConfigInfoList.numOfUE,
                LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        return INVALID_NUM_OF_UE;
    }
    ueCarrierAggConfigInfoList_p = 
        &(rrmMacUeReconfigReq_p->ueCarrierAggConfigInfoList);
    rrmMacUEReconfigCnf_p = (RrmMacUEReConfigCnf *) buffer_p;
    ueCarrierAggConfigErrorInfo_p = 
        (UECarrierAggConfigErrorInfo *) &rrmMacUEReconfigCnf_p-> \
        ueCarrierAggConfigErrorInfo;

    for (index = 0; index < rrmMacUeReconfigReq_p-> \
            ueCarrierAggConfigInfoList.numOfUE; index++)
    {
        ueCarrierAggConfigError_p = 
            (UECarrierAggConfigError *)(ueCarrierAggConfigErrorInfo_p-> \
                    ueCarrierAggConfigError + numOfUeCarrierAggConfigError);
        rrmUeIndex = 
            ueCarrierAggConfigInfoList_p->ueScellSchedulingInfo[index].ueIndex;
        ueIndex = getUeIdxFromRrcUeIdx(rrmUeIndex);
        ueScellSchedulingInfo_p = 
            (UEScellSchedulingInfo *)(ueCarrierAggConfigInfoList_p-> \
                    ueScellSchedulingInfo + index);
        /*spr 16542 fix start*/
        ueScellSchedulingInfo_p->ueIndex = ueIndex;
        /*spr 16542 fix stop*/
	isMACCESendToUE = ueScellSchedulingInfo_p->isMACCESendToUE;
        numOfSCellSchedulingAllowed =
            ueScellSchedulingInfo_p->numOfScellsSchedulingAllowed;
        numOfSCellSchedulingNotAllowed =
            ueScellSchedulingInfo_p->numOfScellsSchedulingNotAllowed;
        /*check for Invlaid UE Index*/
        if (ueIndex >= MAX_UE_SUPPORTED)
        {
            ueCarrierAggConfigError_p->ueIndex   = rrmUeIndex;
            ueCarrierAggConfigError_p->errorCode = INVALID_UE_INDEX_ERROR;
            numOfUeCarrierAggConfigError++;

            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueCarrierAggConfigError_p->ueIndex,
                    ueCarrierAggConfigError_p->errorCode,
                    numOfUeCarrierAggConfigError,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");

            continue;
        }

        ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

        /*If Pending flag is set for this UE, than put this UE in 
         *    CaConfigErrorInfo*/
        if (!ueDLContext_p || ueDLContextInfo_p->pendingDeleteFlag || \
                internalCellIndex != ueDLContext_p->internalCellIndex)
        {
            ueCarrierAggConfigError_p->ueIndex = rrmUeIndex;
            ueCarrierAggConfigError_p->errorCode = UE_DOES_NOT_EXIST_ERROR;
            /*Incerement UE CA Config error count*/
            numOfUeCarrierAggConfigError++;
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueCarrierAggConfigError_p->ueIndex,
                    ueCarrierAggConfigError_p->errorCode,
                    numOfUeCarrierAggConfigError,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            continue;
        }
        else if(numOfSCellSchedulingAllowed > MAX_NUM_SCELL)
        {
            ueCarrierAggConfigError_p->ueIndex = rrmUeIndex;
            ueCarrierAggConfigError_p->errorCode = SCELL_CONFIGURATION_INVALID;
            /*Incerement UE CA Config error count*/
            numOfUeCarrierAggConfigError++;

            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueCarrierAggConfigError_p->ueIndex,
                    ueCarrierAggConfigError_p->errorCode,
                    numOfUeCarrierAggConfigError,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            continue;
        }
        else if(numOfSCellSchedulingNotAllowed > MAX_NUM_SCELL)
        { 
            ueCarrierAggConfigError_p->ueIndex = rrmUeIndex;
            ueCarrierAggConfigError_p->errorCode = SCELL_CONFIGURATION_INVALID;
            /*Incerement UE CA Config error count*/
            numOfUeCarrierAggConfigError++;

            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueCarrierAggConfigError_p->ueIndex,
                    ueCarrierAggConfigError_p->errorCode,
                    numOfUeCarrierAggConfigError,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            continue;
        }
        else if(isMACCESendToUE > 1)
        { 
            ueCarrierAggConfigError_p->ueIndex = rrmUeIndex;
            ueCarrierAggConfigError_p->errorCode = SCELL_CONFIGURATION_INVALID;
            /*Incerement UE CA Config error count*/
            numOfUeCarrierAggConfigError++;

            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueCarrierAggConfigError_p->ueIndex,
                    ueCarrierAggConfigError_p->errorCode,
                    numOfUeCarrierAggConfigError,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            continue;
        }
        if(FALSE == macValidateScellSchedulingActDeact(ueScellSchedulingInfo_p,
                    ueCarrierAggConfigError_p,
                    rrmUeIndex,
                    internalCellIndex))
        {
            /*Incerement UE CA Config error count*/
            numOfUeCarrierAggConfigError++;
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueCarrierAggConfigError_p->ueIndex,
                    ueCarrierAggConfigError_p->errorCode,
                    numOfUeCarrierAggConfigError,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            continue;

        }
        /*If all validation gets success than push this UE in 
         *   ueReconfigScellSchedulingQueue*/
        GET_MEM_FROM_POOL(UEScellSchedulingInfoNode, 
                ueScellSchedulingInfoNode_p, 
                sizeof(UEScellSchedulingInfoNode), PNULL);
        if(PNULL == ueScellSchedulingInfoNode_p)
        {
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    internalCellIndex,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            ueCarrierAggConfigError_p->ueIndex = rrmUeIndex;
            ueCarrierAggConfigError_p->errorCode = MAC_RRM_SYSTEM_ERROR;
            /*Incerement UE CA Config error count*/
            numOfUeCarrierAggConfigError++;
            continue;
        }
        memCpy(ueScellSchedulingInfoNode_p,ueScellSchedulingInfo_p,
                sizeof(UEScellSchedulingInfoNode));
        if(MAC_FAILURE == 
                ENQUEUE_UE_RECONFIG_SCELL_SCHEDULING_Q(rrmMacCellGlobal_gp \
                    [internalCellIndex]->ueReconfigScellSchedulingQueue, 
                    ueScellSchedulingInfoNode_p,
                    internalCellIndex))
        {
            ueCarrierAggConfigError_p->ueIndex = rrmUeIndex;
            ueCarrierAggConfigError_p->errorCode = MAC_RRM_SYSTEM_ERROR;
            /*Incerement UE CA Config error count*/
            numOfUeCarrierAggConfigError++;
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    internalCellIndex,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            FREE_MEM_NODE_UE_RECONFIG_SCELL_SCHEDULING_Q(
                    ueScellSchedulingInfoNode_p);
            continue;
        }
        ueUpdateFlag = TRUE;

    }
    /* Update numOfUe for which CA reconfiguration failed */
    ueCarrierAggConfigErrorInfo_p->numOfUE = numOfUeCarrierAggConfigError;

    if (ueUpdateFlag == FALSE)
    {
        /* This is the case when all the CA reconfiguration fails */
        retVal = MAC_FAILURE;
        /* SPR 10970 fix + */
        rrmMacUEReconfigCnf_p->bitmask |= UE_CA_CONFIG_ERROR_INFO_BITMASK; 
        /* SPR 10970 fix - */
        LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                rrmMacUeReconfigReq_p->ueCarrierAggConfigInfoList.numOfUE,
                rrmMacUEReconfigCnf_p->bitmask,DEFAULT_INT_VALUE,
                LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
    }
    else if (numOfUeCarrierAggConfigError > 0 && 
            numOfUeCarrierAggConfigError < rrmMacUeReconfigReq_p-> \
            ueCarrierAggConfigInfoList.numOfUE)
    {
        retVal = MAC_PARTIAL_SUCCESS;
        /* SPR 10970 fix + */
        rrmMacUEReconfigCnf_p->bitmask |= UE_CA_CONFIG_ERROR_INFO_BITMASK; 
        /* SPR 10970 fix - */
    }
    else
    {
        retVal = MAC_SUCCESS;
    }
    return retVal;
}
/************************************************************************
 * Function Name    : macReconfigUEControlChannelInterferenceList
 * Inputs           : rrmMacUeReconfigReq_p,buffer_p, internalCellIndex
 * Returns          : MAC_SUCCESS 
 * Description      :This function validates uecontext and reconfigures UE 
 * as victim or not victim
 *                 
 ************************************************************************/
UInt32 macReconfigUEControlChannelInterferenceList(
        RrmMacUeReconfigReq *rrmMacUeReconfigReq_p, 
        UInt8 *buffer_p,
        InternalCellIndex internalCellIndex)
{
    UESevereDlControlChannelInterferenceInfo *UESevereDlControlChannelInterferenceInfo_p  = PNULL;
    DLUEContext *ueDLContext_p                                  = PNULL;
    ULUEContextInfo  *ueULContextInfo_p                         = PNULL;
    ULUEContext *ueULContext_p                                  = PNULL;
    DLUEContextInfo  *ueDLContextInfo_p                         = PNULL;
    UInt32 index                                = 0;
    UInt32 ueIndex                              = 0;
    UInt32 rrmUeIndex                           = 0;
    UInt32 retVal                               = MAC_SUCCESS;
    UserType ueCategoryType = NON_VICTIM_UE;
    UInt32 numOfUeConfigError                         = 0;
    RrmMacUEReConfigCnf *rrmMacUEReconfigCnf_p                  = PNULL;
    UEInterferenceInfoError *ueDlControlChannelInterferenceError_p = PNULL;
    UEDlControlChannelInterferenceErrorInfo *ueDlControlChannelInterferenceErrorInfo_p = PNULL;
    /* EICIC +*/
    UserType	oldUeType = NON_VICTIM_UE;
    UserType	newUeType = NON_VICTIM_UE;
    UEUserTypeNode *UEUserTypeNode_p = PNULL;
    /* EICIC -*/
    UInt32 ueUpdateFlag                         = FALSE;

    /*return INVALID_NUM_OF_UE if numOfUE is greater than MAX_NUM_OF_UE*/
    if (rrmMacUeReconfigReq_p->ueSevereDlControlChannelInterferenceInfo.numOfUE > 
            MAX_NUM_OF_UE)
    {
        LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                rrmMacUeReconfigReq_p->ueSevereDlControlChannelInterferenceInfo.numOfUE,
                LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        return INVALID_NUM_OF_UE;
    }

    UESevereDlControlChannelInterferenceInfo_p  = 
        (UESevereDlControlChannelInterferenceInfo *) &rrmMacUeReconfigReq_p-> \
        ueSevereDlControlChannelInterferenceInfo;

    rrmMacUEReconfigCnf_p = (RrmMacUEReConfigCnf *) buffer_p;
    ueDlControlChannelInterferenceErrorInfo_p = (UEDlControlChannelInterferenceErrorInfo *) &rrmMacUEReconfigCnf_p-> \
                                                ueDlControlChannelInterferenceErrorInfo;

    for (index = 0; index < rrmMacUeReconfigReq_p-> \
            ueSevereDlControlChannelInterferenceInfo.numOfUE; index++)
    {
        ueDlControlChannelInterferenceError_p = (UEInterferenceInfoError*) (ueDlControlChannelInterferenceErrorInfo_p-> \
                ueInterferenceInfoError + numOfUeConfigError);
        rrmUeIndex = 
            UESevereDlControlChannelInterferenceInfo_p->SevereDlControlChannelInterferenceInfo[index].ueIndex;
        ueIndex = getUeIdxFromRrcUeIdx(rrmUeIndex);
        /*check for Invlaid UE Index*/
        if (ueIndex >= MAX_UE_SUPPORTED)
        {

            ueDlControlChannelInterferenceError_p->ueIndex   = rrmUeIndex;
            ueDlControlChannelInterferenceError_p->errorCode = INVALID_UE_INDEX_ERROR;
            numOfUeConfigError++;
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    rrmUeIndex,
                    ueIndex,
                    index,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");

            lteWarning ("Invalid UE index \n");
            continue;
        }

        ueCategoryType = (UserType)UESevereDlControlChannelInterferenceInfo_p->SevereDlControlChannelInterferenceInfo[index].SevereDlControlChannelInterference;
        ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

        if (!ueDLContext_p || ueDLContextInfo_p->pendingDeleteFlag || \
                internalCellIndex != ueDLContext_p->internalCellIndex)
        {

            ueDlControlChannelInterferenceError_p->ueIndex   = rrmUeIndex;
            ueDlControlChannelInterferenceError_p->errorCode = UE_DOES_NOT_EXIST_ERROR;
            numOfUeConfigError++;
            continue;
        }
        else
        {
            /* EICIC +*/
            oldUeType = ueDLContext_p->userType;
            newUeType = ueCategoryType;

            if (oldUeType == newUeType)
            {
                ueUpdateFlag = TRUE;
                continue;
            }

            ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            ueULContext_p = ueULContextInfo_p->ulUEContext_p;

            if (PNULL == ueULContext_p)
            {
                lteWarning ("ueULContext_p is Null \n");
                continue;
            }

            GET_MEM_UE_RECONFIG_USER_TYPE_Q(UEUserTypeNode_p,UEUserTypeNode);
            if (PNULL == UEUserTypeNode_p)
            {
                lteWarning ("UEUserTypeNodeDL_p is Null \n");
                continue;
            }

            UEUserTypeNode_p->ueIndex =  ueIndex;
            UEUserTypeNode_p->userType = ueCategoryType;

            if(MAC_FAILURE == ENQUEUE_UE_RECONFIG_USER_TYPE_Q(
                        UEUserTypeNode,UEUserTypeNode_p, internalCellIndex))
            {
                FREE_MEM_NODE_UE_RECONFIG_USER_TYPE_Q(UEUserTypeNode_p);
                continue;
            }

            ueUpdateFlag = TRUE;
        }
    }

    if (ueUpdateFlag == FALSE)
    {
        /* This is the case when all the cqi validity timer 
         *   reconfiguration fails */
        retVal = MAC_FAILURE;
        /* SPR 10970 fix + */
        rrmMacUEReconfigCnf_p->bitmask |= 
            UE_CONTROL_CHANNEL_INTERFERENCE_ERROR_INFO_BITMASK; 
    }
    else if (numOfUeConfigError > 0 && 
            numOfUeConfigError < rrmMacUeReconfigReq_p-> \
            ueSevereDlControlChannelInterferenceInfo.numOfUE)
    {
        retVal = MAC_PARTIAL_SUCCESS;
        /* SPR 10970 fix + */
        rrmMacUEReconfigCnf_p->bitmask |= 
            UE_CONTROL_CHANNEL_INTERFERENCE_ERROR_INFO_BITMASK; 
        /* SPR 10970 fix - */
    }
    else
    {
        retVal = MAC_SUCCESS;
    }

    return retVal;

}


/************************************************************************
 * Function Name    : macReconfigUECqiValidityTimerList
 * Inputs           : rrmMacUeReconfigReq_p,buffer_p, internalCellIndex
 * Returns          : MAC_SUCCESS 
 * Description      :This function validates uecontext and pending delete
 *                     check and reconfigures cqi validity timer Info
 *                 
 ************************************************************************/
UInt32 macReconfigUECqiValidityTimerList(
                        RrmMacUeReconfigReq *rrmMacUeReconfigReq_p, 
                        UInt8 *buffer_p,
                        InternalCellIndex internalCellIndex)
{
    UECqiValidityTimerErrorInfo *ueCqiValidityTimerErrorInfo_p  = PNULL;
    UECqiValidityTimerInfoList *ueCqiValidityTimerInfoList_p    = PNULL;
    DLUEContext *ueDLContext_p                                  = PNULL;
    DLUEContextInfo  *ueDLContextInfo_p                         = PNULL;
    UECqiValidityTimerError *ueCqiValidityTimerError_p          = PNULL;
    UECqiValidityTimerInfoNode *ueCqiValidityTimerInfoNode_p    = PNULL;
    RrmMacUEReConfigCnf *rrmMacUEReconfigCnf_p                  = PNULL;
    UECqiValidityTimerInfo *ueCqiValidityTimerInfo_p            = PNULL;
    UInt8 cqiValidityTimer = 0;

    UInt32 numOfUeWithCqivalidityTimerError     = 0;
    UInt32 index                                = 0;
    UInt32 ueIndex                              = 0;
    UInt32 rrmUeIndex                           = 0;
    UInt32 ueUpdateFlag                         = FALSE;
    UInt32 retVal                               = MAC_SUCCESS;

    /*return INVALID_NUM_OF_UE if numOfUE is greater than MAX_NUM_OF_UE*/
    if (rrmMacUeReconfigReq_p->ueCqiValidityTimerInfoList.numOfUE > 
            MAX_NUM_OF_UE)
    {
        LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                rrmMacUeReconfigReq_p->ueCqiValidityTimerInfoList.numOfUE,
                LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        return INVALID_NUM_OF_UE;
    }

    ueCqiValidityTimerInfoList_p  = 
        (UECqiValidityTimerInfoList *) &rrmMacUeReconfigReq_p-> \
        ueCqiValidityTimerInfoList;
    rrmMacUEReconfigCnf_p = (RrmMacUEReConfigCnf *) buffer_p;
    ueCqiValidityTimerErrorInfo_p = 
        (UECqiValidityTimerErrorInfo *) &rrmMacUEReconfigCnf_p-> \
        ueCqiValidityTimerErrorInfo;

    for (index = 0; index < rrmMacUeReconfigReq_p-> \
            ueCqiValidityTimerInfoList.numOfUE; index++)
    {
        ueCqiValidityTimerError_p   = 
            (UECqiValidityTimerError *)(ueCqiValidityTimerErrorInfo_p-> \
                    ueCqiValidityTimerError + numOfUeWithCqivalidityTimerError);
        rrmUeIndex = 
            ueCqiValidityTimerInfoList_p->ueCqiValidityTimerInfo[index].ueIndex;
        ueIndex = getUeIdxFromRrcUeIdx(rrmUeIndex);
        /*check for Invlaid UE Index*/
        if (ueIndex >= MAX_UE_SUPPORTED)
        {
            ueCqiValidityTimerError_p->ueIndex   = rrmUeIndex;
            ueCqiValidityTimerError_p->errorCode = INVALID_UE_INDEX_ERROR;
            numOfUeWithCqivalidityTimerError++;
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueCqiValidityTimerError_p->ueIndex,
                    ueCqiValidityTimerError_p->errorCode,
                    numOfUeWithCqivalidityTimerError,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");

            lteWarning ("Invalid UE index \n");
            continue;
        }

        ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
        ueCqiValidityTimerInfo_p = 
            (UECqiValidityTimerInfo *)(ueCqiValidityTimerInfoList_p-> \
                    ueCqiValidityTimerInfo + index);
        cqiValidityTimer = ueCqiValidityTimerInfo_p->cqiValidityTimer; 

        /*If Pending flag is set for this UE, than put this UE in cqi 
         *   validity timer ErrorInfo*/
        if (!ueDLContext_p || ueDLContextInfo_p->pendingDeleteFlag || \
                internalCellIndex != ueDLContext_p->internalCellIndex)
        {
            ueCqiValidityTimerError_p->ueIndex   = rrmUeIndex;
            ueCqiValidityTimerError_p->errorCode = UE_DOES_NOT_EXIST_ERROR;
            /*Incerement UE cqi validity timer error count*/
            numOfUeWithCqivalidityTimerError++;
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueCqiValidityTimerError_p->ueIndex,
                    ueCqiValidityTimerError_p->errorCode,
                    numOfUeWithCqivalidityTimerError,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            continue;
        }
        /*If all validation gets success than push this UE in 
         *    ueReconfigCqiValidityTimerQueue_g*/
        else
        {
            GET_MEM_FROM_POOL(UECqiValidityTimerInfoNode, 
                    ueCqiValidityTimerInfoNode_p, 
                    sizeof(UECqiValidityTimerInfoNode), PNULL);
            if(PNULL == ueCqiValidityTimerInfoNode_p)
            {
                LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        internalCellIndex,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
                ueCqiValidityTimerError_p->ueIndex   = rrmUeIndex;
                ueCqiValidityTimerError_p->errorCode = MAC_RRM_SYSTEM_ERROR;
                numOfUeWithCqivalidityTimerError++;
                continue;
            }
            ueCqiValidityTimerInfoNode_p->ueIndex = ueIndex;
            ueCqiValidityTimerInfoNode_p->cqiValidityTimer = cqiValidityTimer;
            if(MAC_FAILURE == 
                    ENQUEUE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(rrmMacCellGlobal_gp \
                        [internalCellIndex]->ueReconfigCqiValidityTimerQueue,
                        ueCqiValidityTimerInfoNode_p,
                        internalCellIndex))
            {
                LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        internalCellIndex,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
                ueCqiValidityTimerError_p->ueIndex   = rrmUeIndex;
                ueCqiValidityTimerError_p->errorCode = MAC_RRM_SYSTEM_ERROR;
                numOfUeWithCqivalidityTimerError++;
                FREE_MEM_NODE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(
                        ueCqiValidityTimerInfoNode_p);
                continue;
            }
            ueUpdateFlag = TRUE;
        }
    }
    /* Update numOfUe for which reconfiguration of Cqi validity timer failed */
    ueCqiValidityTimerErrorInfo_p->numOfUE = numOfUeWithCqivalidityTimerError;

    if (ueUpdateFlag == FALSE)
    {
        /* This is the case when all the cqi validity timer 
         *   reconfiguration fails */
        retVal = MAC_FAILURE;
        /* SPR 10970 fix + */
        rrmMacUEReconfigCnf_p->bitmask |= 
            UE_CQI_VALIDITY_TIMER_ERROR_INFO_BITMASK; 
        /* SPR 10970 fix - */
        LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                ueCqiValidityTimerErrorInfo_p->numOfUE,
                rrmMacUEReconfigCnf_p->bitmask,DEFAULT_INT_VALUE,
                LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
    }
    else if (numOfUeWithCqivalidityTimerError > 0 && 
            numOfUeWithCqivalidityTimerError < rrmMacUeReconfigReq_p-> \
            ueCqiValidityTimerInfoList.numOfUE)
    {
        retVal = MAC_PARTIAL_SUCCESS;
        /* SPR 10970 fix + */
        rrmMacUEReconfigCnf_p->bitmask |= 
            UE_CQI_VALIDITY_TIMER_ERROR_INFO_BITMASK; 
        /* SPR 10970 fix - */
    }
    else
    {
        retVal = MAC_SUCCESS;
    }
    return retVal;

}
/*CA changes end*/

/* + TM7_8 Changes Start*/ 
/************************************************************************
 * Function Name    : reconfigNumOfLayer
 * Inputs           : rrmMacUeReconfigReq_p,buffer_p,internalCellIndex
 * Returns          : MAC_SUCCESS 
 * Description      : This function validates uecontext and pending delete
 *                    check and reconfigures number of layer
 ************************************************************************/
UInt32  reconfigNumOfLayer(
          RrmMacUeReconfigReq *rrmMacUeReconfigReq_p,
          UInt8 *buffer_p, InternalCellIndex  internalCellIndex)
{
    UInt32 retVal                      = MAC_SUCCESS;
    UInt32 index                      = 0;
    UInt32 ueIndex                    = 0;
    UInt32 rrmUeIndex                 = 0;
    UInt32 bitmask                    = 0;
    UInt32 numOfUeWithNumOfLayerError = 0;
    UInt8 ueNumOfLayer                = 0;
    UENumOfLayerInfo *ueNumOfLayerInfo_p = PNULL;
    UENumOfLayerError *ueNumOfLayerError_p = PNULL;

    UInt32             ueUpdateFlag   = FALSE;
    DLUEContext       *dlUEContext_p  = PNULL;
    DLUEContextInfo   *dlUEContextInfo_p = PNULL;
    UENumOfLayerErrorInfo *ueNumOfLayerErrorInfo_p = PNULL;
    RrmMacUEReConfigCnf *rrmMacUEReconfigCnf_p = PNULL;
    UENumOfLayerNode *ueNumOfLayerNode_p = PNULL;

    bitmask = rrmMacUeReconfigReq_p->bitmask;
    ueNumOfLayerInfo_p = &rrmMacUeReconfigReq_p->ueNumOfLayerInfo;

    if(UE_NUM_OF_LAYER_RECONFIG_BIT & bitmask)
    {
        rrmMacUEReconfigCnf_p = (RrmMacUEReConfigCnf *) buffer_p;
        ueNumOfLayerErrorInfo_p = (UENumOfLayerErrorInfo *)&rrmMacUEReconfigCnf_p->ueNumOfLayerErrorInfo;

        /* return INVALID_NUM_OF_UE if numOfUE is greater than MAX_NUM_OF_UE */
        if (rrmMacUeReconfigReq_p->ueNumOfLayerInfo.numOfUE > MAX_NUM_OF_UE)
        {
            return INVALID_NUM_OF_UE;
        }

        for (index = 0; index < rrmMacUeReconfigReq_p->ueNumOfLayerInfo.numOfUE; index++)
        {
            ueNumOfLayerError_p = (UENumOfLayerError *)(ueNumOfLayerErrorInfo_p->ueNumOfLayerError + 
                    numOfUeWithNumOfLayerError);

            rrmUeIndex = ueNumOfLayerInfo_p->ueNumOfLayer[index].ueIndex;
            ueNumOfLayer  = ueNumOfLayerInfo_p->ueNumOfLayer[index].numOfLayer;

            ueIndex = getUeIdxFromRrcUeIdx(rrmUeIndex);

            /*return  INVALID_UE_INDEX_ERROR if ueIndex is Invalid*/
            if (ueIndex >= MAX_UE_SUPPORTED)
            {
                ueNumOfLayerError_p->ueIndex = rrmUeIndex;
                ueNumOfLayerError_p->errorCode = INVALID_UE_INDEX_ERROR;
                numOfUeWithNumOfLayerError++;
                lteWarning ("Invalid UE index \n");
                continue;
            }

            dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

            /* If Pending flag is set for this UE, than put this UE in NumOfLayerErrorInfo */
            if (!dlUEContext_p || dlUEContextInfo_p->pendingDeleteFlag || 
                    internalCellIndex != dlUEContext_p->internalCellIndex)
            {
                ueNumOfLayerError_p->ueIndex = rrmUeIndex;
                ueNumOfLayerError_p->errorCode = UE_DOES_NOT_EXIST_ERROR;
                numOfUeWithNumOfLayerError++;
                continue;
            }
            /*If all validation gets success than push this UE in ueReconfigNumOfLayerQueue*/
            else
            {
                GET_MEM_FROM_POOL(UENumOfLayerNode, ueNumOfLayerNode_p, sizeof(UENumOfLayerNode), PNULL);
                if(PNULL == ueNumOfLayerNode_p)
                {
                    /* SPR 9813 START */
                    ueNumOfLayerError_p->ueIndex = rrmUeIndex;
                    ueNumOfLayerError_p->errorCode = MAC_RRM_SYSTEM_ERROR;
                    numOfUeWithNumOfLayerError++;
                    /* SPR 9813 END */
                    continue; 
                }
                ueNumOfLayerNode_p->ueIndex =  ueIndex;
                ueNumOfLayerNode_p->numOfLayer  = ueNumOfLayer ;
                /* Node is pushed to the Queue for updating number of Layers */
                /* SPR 23483 Changes Start */
                if(CIRC_SUCCESS == pushNodeCircQ(&rrmMacCellGlobal_gp[internalCellIndex]-> \
                            ueReconfigNumOfLayerQueue,ueNumOfLayerNode_p))
                /* SPR 23483 Changes End */
                {
                    ueUpdateFlag = TRUE;
                }
                else
                {
                    freeMemPool(ueNumOfLayerNode_p);
                    ueNumOfLayerNode_p = PNULL;
                }
            }
        }
        ueNumOfLayerErrorInfo_p->numOfUE = numOfUeWithNumOfLayerError;
        if (ueUpdateFlag == FALSE)
        {
            retVal = MAC_FAILURE;
            /* SPR 10970 fix + */
            rrmMacUEReconfigCnf_p->bitmask |= UE_NUM_OF_LAYER_ERROR_INFO_BITMASK ;
            /* SPR 10970 fix - */
        }
        else if (numOfUeWithNumOfLayerError > 0 &&
                numOfUeWithNumOfLayerError < rrmMacUeReconfigReq_p->ueNumOfLayerInfo.numOfUE)
        {
            retVal = MAC_PARTIAL_SUCCESS;
            /* SPR 10970 fix + */
            rrmMacUEReconfigCnf_p->bitmask |= UE_NUM_OF_LAYER_ERROR_INFO_BITMASK ;
            /* SPR 10970 fix - */
        }
        else
        {
            retVal = MAC_SUCCESS;
        }
    }
    return retVal;
}

/************************************************************************
 * Function Name    : reconfigBeamFormingAlgo
 * Inputs           : rrmMacUeReconfigReq_p,buffer_p,internalCellIndex
 * Returns          : MAC_SUCCESS 
 * Description      : This function validates uecontext and pending delete.
 *                    Check and reconfigures beam forming algorithm.
                       
 ************************************************************************/
UInt32  reconfigBeamFormingAlgo(
          RrmMacUeReconfigReq *rrmMacUeReconfigReq_p,
          UInt8 *buffer_p, InternalCellIndex  internalCellIndex)
{
    UInt32 retVal                        = MAC_SUCCESS;
    UInt32 index                        = 0;
    UInt32 ueIndex                      = 0;
    UInt32 rrmUeIndex                   = 0;
    UInt32 bitmask                      = 0;
    UInt32 numOfUeWithBeamFormAlgoError = 0;
    UInt8 uebeamformingAlgoToBeUsed     = 0;
    UEBeamformingAlgoToBeUsedInfo *ueBeamformingAlgoToBeUsedInfo_p = PNULL;
    UEBeamFormingAlgoError *ueBeamFormingAlgoError_p = PNULL;

    UInt32             ueUpdateFlag   = FALSE;
    DLUEContext       *dlUEContext_p  = PNULL;
    DLUEContextInfo   *dlUEContextInfo_p = PNULL;
    UEBeamformingAlgoErrorInfo *ueBeamFormingAlgoErrorInfo_p = PNULL;
    RrmMacUEReConfigCnf *rrmMacUEReconfigCnf_p = PNULL;
    UEBeamformingAlgoNode *ueBeamformingAlgoNode_p = PNULL;

    bitmask = rrmMacUeReconfigReq_p->bitmask;
    ueBeamformingAlgoToBeUsedInfo_p = &rrmMacUeReconfigReq_p->ueBeamformingAlgoToBeUsedInfo;

    if(UE_BEAM_FORMING_ALGO_RECONFIG_BIT & bitmask)
    {
        rrmMacUEReconfigCnf_p = (RrmMacUEReConfigCnf *)buffer_p;
        ueBeamFormingAlgoErrorInfo_p = (UEBeamformingAlgoErrorInfo *)&rrmMacUEReconfigCnf_p->ueBeamFormingAlgoErrorInfo;

        /* return INVALID_NUM_OF_UE if numOfUE is greater than MAX_NUM_OF_UE */
        if (rrmMacUeReconfigReq_p->ueBeamformingAlgoToBeUsedInfo.numOfUE > MAX_NUM_OF_UE)
        {
            return INVALID_NUM_OF_UE;
        }

        for (index = 0; index < rrmMacUeReconfigReq_p->ueBeamformingAlgoToBeUsedInfo.numOfUE; index++)
        {
            ueBeamFormingAlgoError_p = 
                (UEBeamFormingAlgoError *)(ueBeamFormingAlgoErrorInfo_p->
                        ueBeamFormingAlgoError + numOfUeWithBeamFormAlgoError);

            rrmUeIndex = ueBeamformingAlgoToBeUsedInfo_p->ueBeamformingAlgo[index].ueIndex;
            uebeamformingAlgoToBeUsed = 
                    ueBeamformingAlgoToBeUsedInfo_p->ueBeamformingAlgo[index].beamformingAlgoToBeUsed;

            ueIndex = getUeIdxFromRrcUeIdx(rrmUeIndex);

            /* return  UE_ID_INVALID_ERROR if ueIndex is Invalid */
            if (ueIndex >= MAX_UE_SUPPORTED)
            {
                ueBeamFormingAlgoError_p->ueIndex = rrmUeIndex;
                ueBeamFormingAlgoError_p->errorCode = INVALID_UE_INDEX_ERROR;
                numOfUeWithBeamFormAlgoError++;
                lteWarning ("Invalid UE index \n");
                continue;
            }

            dlUEContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

            /*If Pending flag is set for this UE, than put this UE in UEBeamFormingAlgoError*/
            if (!dlUEContext_p || dlUEContextInfo_p->pendingDeleteFlag || \
                    internalCellIndex != dlUEContext_p->internalCellIndex)
            {
                ueBeamFormingAlgoError_p->ueIndex = rrmUeIndex;
                ueBeamFormingAlgoError_p->errorCode = UE_DOES_NOT_EXIST_ERROR;
                numOfUeWithBeamFormAlgoError++;
                continue;
            }
            /*If all validation gets success than push this UE in ueReconfigNumOfLayerQueue*/
            else
            {
                GET_MEM_FROM_POOL(UEBeamformingAlgoNode, ueBeamformingAlgoNode_p, sizeof(UEBeamformingAlgoNode), PNULL);
                if(PNULL == ueBeamformingAlgoNode_p)
                {
                    /* SPR 9813 START */
                    ueBeamFormingAlgoError_p->ueIndex = rrmUeIndex;
                    ueBeamFormingAlgoError_p->errorCode = MAC_RRM_SYSTEM_ERROR;
                    numOfUeWithBeamFormAlgoError++;
                    /* SPR 9813 END */
                    continue; 
                }
                ueBeamformingAlgoNode_p->ueIndex =  ueIndex;
                ueBeamformingAlgoNode_p->beamformingAlgoToBeUsed  = uebeamformingAlgoToBeUsed;
                /* Node is pushed to the Queue for updating Beam forming Algo. */
                /* SPR 23483 Changes Start */
                if(CIRC_SUCCESS == pushNodeCircQ(&rrmMacCellGlobal_gp[internalCellIndex]-> \
                            ueReconfigBeamFormingAlgoQueue, ueBeamformingAlgoNode_p))
                /* SPR 23483 Changes End */
                {
                    ueUpdateFlag = TRUE;
                }
                else
                {
                    freeMemPool(ueBeamformingAlgoNode_p);
                    ueBeamformingAlgoNode_p = PNULL;
                    ueBeamFormingAlgoError_p->ueIndex = rrmUeIndex;
                    ueBeamFormingAlgoError_p->errorCode = MAC_RRM_SYSTEM_ERROR;
                    numOfUeWithBeamFormAlgoError++;
                }
            }
        }
        ueBeamFormingAlgoErrorInfo_p->numOfUE = numOfUeWithBeamFormAlgoError;
        if (ueUpdateFlag == FALSE)
        {
            retVal = MAC_FAILURE;
            /* SPR 10970 fix + */
            rrmMacUEReconfigCnf_p->bitmask |= UE_BEAM_FORMING_ALGO_ERROR_INFO_BITMASK;
            /* SPR 10970 fix - */
        }
        else if (numOfUeWithBeamFormAlgoError > 0 &&
                numOfUeWithBeamFormAlgoError < rrmMacUeReconfigReq_p->ueBeamformingAlgoToBeUsedInfo.numOfUE)
        {
            retVal = MAC_PARTIAL_SUCCESS;
            /* SPR 10970 fix + */
            rrmMacUEReconfigCnf_p->bitmask |= UE_BEAM_FORMING_ALGO_ERROR_INFO_BITMASK;
            /* SPR 10970 fix - */
        }
        else
        {
            retVal = MAC_SUCCESS;
        }
    }
    return retVal;
}

/* - TM7_8 Changes End*/ 
/************************************************************************
 * Function Name    : macReconfigSchedulerParams 
 * Inputs           : rrmMacReconfigSchedulerReq_p pointer to structure RrmMacReconfigSchedulerReq 
 *                    internalCellIndex
 * Returns          : MAC_SUCCESS or MAC_FAILURE
 * Description      : This function configures different parameters for MAC scheduler.
 ************************************************************************/
UInt32 macReconfigSchedulerParams(
        RrmMacReconfigSchedulerReq *rrmMacReconfigSchedulerReq_p,
        InternalCellIndex  internalCellIndex)
{
    UInt32  schParamBitmapVal                      = 0;
    UInt32  srsSubBandGroupSize                    = 0;
    UInt32  tpcNumUeThresholdForPUSCHDCI3A          = 0;
    UInt32  tpcNumUeThresholdForPUCCHDCI3A         = 0;
    UInt32  retVal                  = MAC_SUCCESS;
    UInt32 ulFlag = FALSE;
    UInt32 dlFlag = FALSE;
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    UInt8   cntMaxMcs               = 0;
#endif
    /*HD FDD Changes End*/
    /*START : DYN_PDCCH*/
    UInt8 aggrLvlCount              = 0;
    /*END   : DYN_PDCCH*/

    if(rrmMacCellGlobal_gp[internalCellIndex]->ulSchedulerReconfigFlag || 
            rrmMacCellGlobal_gp[internalCellIndex]->dlSchedulerReconfigFlag)
    {
        return RETRY_SCHEDULER_RECONFIG_AFTER_SOME_TIME;
    }

    /*CA Changes start  */
    MacReconfigSchedulerParamsUL *schedulerParamsUL_p = 
        &macReconfigSchedulerParamsUL_gp[internalCellIndex][!schParamsIndexUL_g[internalCellIndex]];
    /*CA Changes end  */

    MacReconfigSchedulerParamsDL *schedulerParamsDL_p = 
        &macReconfigSchedulerParamsDL_gp[internalCellIndex][!schParamsIndexDL_g[internalCellIndex]];

    /*CA Changes start  */
    /* Updating passive index from active index */
    *schedulerParamsUL_p = macReconfigSchedulerParamsUL_gp[internalCellIndex][schParamsIndexUL_g[internalCellIndex]];
    /*CA Changes end  */

    *schedulerParamsDL_p = macReconfigSchedulerParamsDL_gp[internalCellIndex][schParamsIndexDL_g[internalCellIndex]];

    /*Read schedulerParamBitMap Value*/
    schParamBitmapVal = rrmMacReconfigSchedulerReq_p->schedulerParamBitMap;

    /*Read the tpcForMsg3 Value*/
    /*    if(schParamBitmapVal & TPC_FOR_MSG3_BIT)
          {
          tpcForMsg3 = rrmMacReconfigSchedulerReq_p->tpcForMsg3;
          }TDD Warning Fix*/

    /* Assuming that even if the tpcForMsg3_g is not present means that the value is not valid
     * so in any case pointer needs to be incremented*/
    /* Use 0x01,0x02,0x04 instead of shifting */
    /*Read the srsSubBandGroupSize Value*/
    if(schParamBitmapVal & SRS_SUBBAND_GRP_SIZE_BIT)
    {
        srsSubBandGroupSize = rrmMacReconfigSchedulerReq_p->srsSubBandGroupSize;
        retVal = validateSrsSubBandGroupSize(srsSubBandGroupSize);
        if(retVal == MAC_FAILURE)
        {
            return MAC_FAILURE;
        }
    }

    /*Read the tpcTriggerWaitTimeForPUSCHDCI0 Value*/
    /*    if(schParamBitmapVal & TPC_TRIGGER_WAIT_TIME_PUSCH_DCI0_BIT)
          {
          tpcTriggerWaitTimeForPUSCHDCI0 = 
          rrmMacReconfigSchedulerReq_p->tpcTriggerWaitTimeForPUSCHDCI0;
          } TDD Warning Fix*/

    /*Read the tpcTriggerWaitTimeForPUCCHDCI1x2x Value*/
    /*    if(schParamBitmapVal & TPC_TRIGGER_WAIT_TIME_PUCCH_DCI1x2x_BIT)
          {
          tpcTriggerWaitTimeForPUCCHDCI1x2x = 
          rrmMacReconfigSchedulerReq_p->tpcTriggerWaitTimeForPUCCHDCI1x2x;
          } TDD Warning Fix*/

    /*Read the tpcNumUeThresholdForPUSCHDCI3A Value*/
    if(schParamBitmapVal & TPC_NUM_UE_THRESHOLD_PUSCH_DCI3A)
    {
        tpcNumUeThresholdForPUSCHDCI3A = 
            rrmMacReconfigSchedulerReq_p->tpcNumUeThresholdForPUSCHDCI3A;
        if (tpcNumUeThresholdForPUSCHDCI3A > 100)
        {
            return MAC_FAILURE;
        }
    }

    /*Read the tpcNumUeThresholdForPUCCHDCI3A Value*/
    if(schParamBitmapVal & TPC_NUM_UE_THRESHOLD_PUCCH_DCI3A)
    {
        tpcNumUeThresholdForPUCCHDCI3A = 
            rrmMacReconfigSchedulerReq_p->tpcNumUeThresholdForPUCCHDCI3A;
        if (tpcNumUeThresholdForPUCCHDCI3A > 100)
        {
            return MAC_FAILURE;
        }
    }

    /* Cyclomatic Complexity changes - starts here */
    if (MAC_SUCCESS != macReconfigPathLossSinrAndBlerParams (
                schParamBitmapVal, rrmMacReconfigSchedulerReq_p))
    {
        return MAC_FAILURE;
    }

    if (MAC_SUCCESS != macReconfigPdcchAggAndPowerOffsetParams (
                schParamBitmapVal, rrmMacReconfigSchedulerReq_p))
    {
        return MAC_FAILURE;
    }

    if (MAC_SUCCESS != macReconfigSpsSchedulingInfo (
                schParamBitmapVal, rrmMacReconfigSchedulerReq_p))
    {
        return MAC_FAILURE;
    }
    /* Cyclomatic Complexity changes - ends here */

    /* +DYNAMIC_ICIC */
    if(schParamBitmapVal & ATB_CONFIG_BIT)
    {
         /* SPR 22099 Fix + */
        retVal = validateAtbInRrmIntf(rrmMacReconfigSchedulerReq_p->atbConfig.minMCSIndexForATB,
                rrmMacReconfigSchedulerReq_p->atbConfig.minPRBValForATB);
         /* SPR 22099 Fix - */
        if(MAC_FAILURE == retVal)
        {
            return MAC_FAILURE;
        }
    }

    if(schParamBitmapVal & PA_QPSK_BIT)
    {
        if (rrmMacReconfigSchedulerReq_p->paQpsk > MAX_PA_VALUE) 
        {
            return MAC_FAILURE;
        }
    }
    /* -DYNAMIC_ICIC */
    /*+ Dyn UEs To Be SchPerTTI +*/
    if(schParamBitmapVal & UES_TO_BE_SCHEDULED_DL_UL_PER_TTI)
    {
        /*+ Dyn UEs To Be SchPerTTI:CR +*/
        modifyMaxUeToBeSchdDlUl_g[internalCellIndex] = VALIDATE_ONLY;
        /*- Dyn UEs To Be SchPerTTI:CR -*/
        if(MAC_FAILURE == validateAndUpdateMaxUeToBeSchdDlUl
                (rrmMacReconfigSchedulerReq_p->maxUEsToBeScheduledDlUl.maxUEsToBeScheduledDL,
                 rrmMacReconfigSchedulerReq_p->maxUEsToBeScheduledDlUl.maxUEsToBeScheduledUL,
                 internalCellIndex))
        {
            return MAC_FAILURE;
        }
    }
    /*- Dyn UEs To Be SchPerTTI -*/
    /* CA changes start */
    if(schParamBitmapVal & CA_PARAMS_BIT)
    {
        retVal = macValidateCAParams(
                &(rrmMacReconfigSchedulerReq_p->caParams), 
                internalCellIndex);
        if(MAC_FAILURE == retVal)
        {
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    schParamBitmapVal,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            return MAC_FAILURE;
        }
    }
    /* CA changes end*/
#ifdef LTE_EMBMS_SUPPORTED
    if(schParamBitmapVal & EMBMS_PARAM_BIT)
    {
        if (rrmMacReconfigSchedulerReq_p->eMbmsReservedCellPowerOffset > \
            MAX_RESERVED_CELL_POWER_OFFSET) 
        {
            return MAC_FAILURE;
        }
    }
#endif

    if(retVal == MAC_SUCCESS)
    {
        /* Cyclomatic Complexity changes - starts here */
        ulFlag = macUpdateULSchedulerReconfigParams ( schParamBitmapVal, 
                rrmMacReconfigSchedulerReq_p, 
                schedulerParamsUL_p, 
                internalCellIndex);

        dlFlag = macUpdateDLSchedulerReconfigParams ( schParamBitmapVal, 
                rrmMacReconfigSchedulerReq_p, 
                schedulerParamsDL_p,
                internalCellIndex);
        /* Cyclomatic Complexity changes - ends here */

        /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
        /*Read the hdReducedMcsTable for HD FDD UE's from RRM */
        if(schParamBitmapVal & HD_REDUCED_MCS_TABLE_BIT)
        {
            /* Store the reduced MCS values into global context which gets from RRM
             * Here only 29 values are stored and rest of the values not used
             * so MAX_MCS_INDEX is used instead of MAX_MCS_VALUES in loop */

            for(cntMaxMcs=0; cntMaxMcs<MAX_MCS_INDEX; ++cntMaxMcs)
            {
                /* RRM should set the MCS index either less than or equal to original 
                 * value of MCS at that position */
                if(rrmMacReconfigSchedulerReq_p->hdReducedMcsTable[cntMaxMcs] <
                        hdFddReducedMcsTableArr_g[cntMaxMcs])
                {
                    hdFddReducedMcsTableArr_g[cntMaxMcs] = 
                        rrmMacReconfigSchedulerReq_p->hdReducedMcsTable[cntMaxMcs];
                }

                /* A defensive check applied if the value is greater than the index,
                 * apply the same index */
                if (hdFddReducedMcsTableArr_g[cntMaxMcs] > cntMaxMcs)
                {
                    hdFddReducedMcsTableArr_g[cntMaxMcs] = cntMaxMcs;
                }
            }
        }
#endif
        /*HD FDD Changes End*/
        /*START : DYN_PDCCH*/
        if(schParamBitmapVal & DYN_PDCCH_EXTENSION_PARAMS_BIT)
        {
            for(aggrLvlCount = 0;aggrLvlCount < MAX_AGGREGATION_LEVELS ;aggrLvlCount++)
            {
                rrmMacCellGlobal_gp[internalCellIndex]-> \
                    cceCorrectionStepUpFactor[aggrLvlCount] = 
                    rrmMacReconfigSchedulerReq_p->dynamicCFIExtensionParams \
                    .cceCorrectionStepUpFactor[aggrLvlCount];
                rrmMacCellGlobal_gp[internalCellIndex]-> \
                    cceCorrectionStepDownFactor[aggrLvlCount]=
                    rrmMacReconfigSchedulerReq_p->dynamicCFIExtensionParams \
                    .cceCorrectionStepDownFactor[aggrLvlCount];
                rrmMacCellGlobal_gp[internalCellIndex]-> \
                    cceAdjustFactor[aggrLvlCount]=
                    rrmMacReconfigSchedulerReq_p->dynamicCFIExtensionParams \
                    .cceAdjustFactor[aggrLvlCount];
            }
        }
        /*END   : DYN_PDCCH*/
    } 
    else
    {
        return MAC_FAILURE;
    }

    /* Set the bitmask indicating that fields successfully updated */
    if(TRUE == ulFlag)
    {
        rrmMacCellGlobal_gp[internalCellIndex]->ulSchedulerReconfigFlag = TRUE;
    }
    if(TRUE == dlFlag)
    {
        rrmMacCellGlobal_gp[internalCellIndex]->dlSchedulerReconfigFlag = TRUE;
    }

    return retVal;
}

/******************************************************************************
 * Function Name    : macReconfigPathLossSinrAndBlerParams 
 * Inputs           : schParamBitmapVal - Scheduler param bitmap value
 *                    rrmMacReconfigSchedulerReq_p - Pointer to structure
 *                    RrmMacReconfigSchedulerReq 
 * Output           : None        
 * Returns          : MAC_SUCCESS or MAC_FAILURE
 * Description      : This function reconfigures Path Loss, SINR and BLER 
 *                    Scheduler parameters
 *****************************************************************************/
static  MacRetType macReconfigPathLossSinrAndBlerParams (
        UInt32                     schParamBitmapVal,
        RrmMacReconfigSchedulerReq *rrmMacReconfigSchedulerReq_p)
{
    UInt32  count                   = 0;
    SInt32  startPL                 = 0;
    SInt32  endPL                   = 0;
    SInt32  startDeltaSINR          = 0;
    SInt32  endDeltaSINR            = 0;
    SInt32  puschTpc                = 0;
    SInt32  startBler               = 0;
    SInt32  endBler                 = 0;
    SInt32  pucchTpc                = 0;
    SInt32  startSINR               = 0;
    SInt32  endSINR                 = 0;
    UInt32  retVal                  = MAC_SUCCESS;
    UInt32  index                   = 0;
    UInt32  idx                     = 0;
    UInt32  pucchBLERTarget         = 0;
    UInt32  pucchSINRTarget         = 0;

    /*Read the alphaBasedPathLossToTargetSINRMap Value*/
    if(schParamBitmapVal & ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_BIT)
    {
        for(index = 0; index < MAX_NUM_ALPHA_COUNT; index++)
        {
            count = rrmMacReconfigSchedulerReq_p->alphaBasedPathLossToTargetSINRMap.
                pathLossToTargetSINRMap[index].count;
            if((count <= MAX_NUM_PATHLOSS_TO_SINR_COUNT))
            {
                for(idx = 0; idx < count; idx++)
                {
                    startPL = rrmMacReconfigSchedulerReq_p->alphaBasedPathLossToTargetSINRMap.
                        pathLossToTargetSINRMap[index].pathLossToTargetSINRMapInfo[idx].
                        startPL;
                    endPL = rrmMacReconfigSchedulerReq_p->alphaBasedPathLossToTargetSINRMap.
                        pathLossToTargetSINRMap[index].pathLossToTargetSINRMapInfo[idx].
                        endPL ;
                    retVal = validateStartEndPL(startPL,endPL);
                    if(MAC_FAILURE == retVal)
                    {
                        return MAC_FAILURE;
                    }
                    /*   targetSINR = rrmMacReconfigSchedulerReq_p->alphaBasedPathLossToTargetSINRMap.
                         pathLossToTargetSINRMap[index].pathLossToTargetSINRMapInfo[idx].
                         targetSINR; TDD Warning Fix */
                }
            }
            else
            {
                return MAC_FAILURE;
            }
        }
    } 

    /*Read the deltaSINRToTPCMapForPUSCH Value*/
    if(schParamBitmapVal & DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_BIT)
    {
        count = rrmMacReconfigSchedulerReq_p->deltaSINRToTPCMapForPUSCH.count;
        if(count <= MAX_NUM_TPC_FOR_PUSCH)
        {
            for(index = 0; index < count; index++)
            {
                startDeltaSINR = rrmMacReconfigSchedulerReq_p->deltaSINRToTPCMapForPUSCH.
                    deltaSINRToTPCMapForPUSCHInfo[index].startDeltaSINR;
                endDeltaSINR = rrmMacReconfigSchedulerReq_p->deltaSINRToTPCMapForPUSCH.
                    deltaSINRToTPCMapForPUSCHInfo[index].endDeltaSINR;
                retVal = validateStartEndSINR(startDeltaSINR,endDeltaSINR);
                if(MAC_FAILURE == retVal)
                {
                    return MAC_FAILURE;
                }
                puschTpc = rrmMacReconfigSchedulerReq_p->deltaSINRToTPCMapForPUSCH.
                    deltaSINRToTPCMapForPUSCHInfo[index].puschTPC;
                retVal = validatePuschTpc(puschTpc);
                if(MAC_FAILURE == retVal)
                {
                    return MAC_FAILURE;
                }
            }
        }
        else
        {
            return MAC_FAILURE;
        }
    }

    /*Read the blerToTPCMapForPUCCH Value*/
    if(schParamBitmapVal & BLER_TO_TPC_MAP_FOR_PUCCH)
    {
        pucchBLERTarget = rrmMacReconfigSchedulerReq_p->deltaBlerToTPCMapForPUCCH.
            pucchBLERTarget;
        if(pucchBLERTarget <= MAX_PUCCH_BLER_TARGET)
        {
            count = rrmMacReconfigSchedulerReq_p->deltaBlerToTPCMapForPUCCH.count;
            if(count <= MAX_NUM_TPC_FOR_PUCCH)
            {
                for(index = 0; index < count; index++)
                {
                    startBler = rrmMacReconfigSchedulerReq_p->deltaBlerToTPCMapForPUCCH.
                        deltaBlerToTPCMapForPUCCHInfo[index].startBLER;
                    endBler = rrmMacReconfigSchedulerReq_p->deltaBlerToTPCMapForPUCCH.
                        deltaBlerToTPCMapForPUCCHInfo[index].endBLER;
                    retVal = validateStartEndBler(startBler,endBler);
                    if(MAC_FAILURE == retVal)
                    {
                        return MAC_FAILURE;
                    }
                    pucchTpc = rrmMacReconfigSchedulerReq_p->deltaBlerToTPCMapForPUCCH.
                        deltaBlerToTPCMapForPUCCHInfo[index].pucchTPC;
                    retVal = validatePucchTpc(pucchTpc);
                    if(MAC_FAILURE == retVal)
                    {
                        return MAC_FAILURE;
                    }
                }
            }
            else
            {
                return MAC_FAILURE;
            }
        }
        else
        {
            return MAC_FAILURE;
        }
    }

    /*Read the deltaSINRToTPCMapForPUCCH Value*/
    if(schParamBitmapVal & DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_BIT)
    {
        pucchSINRTarget = rrmMacReconfigSchedulerReq_p->deltaSINRToTPCMapForPUCCH.pucchSINRTarget;
        if(pucchSINRTarget <= MAX_PUCCH_SINR_TARGET)
        {
            count = rrmMacReconfigSchedulerReq_p->deltaSINRToTPCMapForPUCCH.count;
            if(count <= MAX_NUM_TPC_FOR_PUCCH)
            {
                for(index = 0; index < count; index++)
                {
                    startSINR = rrmMacReconfigSchedulerReq_p->deltaSINRToTPCMapForPUCCH.
                        deltaSinrToTPCMapForPUCCHInfo[index].startSINR;
                    endSINR = rrmMacReconfigSchedulerReq_p->deltaSINRToTPCMapForPUCCH.
                        deltaSinrToTPCMapForPUCCHInfo[index].endSINR;
                    retVal = validateStartEndSINR(startSINR,endSINR);
                    if(retVal == MAC_FAILURE)
                    {
                        return MAC_FAILURE;
                    }
                    pucchTpc = rrmMacReconfigSchedulerReq_p->deltaSINRToTPCMapForPUCCH.
                        deltaSinrToTPCMapForPUCCHInfo[index].pucchTPC;
                    retVal = validatePucchTpc(pucchTpc);
                    if(retVal == MAC_FAILURE)
                    {
                        return MAC_FAILURE;
                    }
                }
            }
            else
            {
                return MAC_FAILURE;
            }
        }
        else
        {
            return MAC_FAILURE;
        }
    }
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name    : macReconfigPdcchAggAndPowerOffsetParams 
 * Inputs           : schParamBitmapVal - Scheduler param bitmap value
 *                    rrmMacReconfigSchedulerReq_p - Pointer to structure
 *                    RrmMacReconfigSchedulerReq 
 * Output           : None        
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function reconfigures PDCCH aggregation and Power 
 *                    Offset Scheduler parameters
 *****************************************************************************/
static  MacRetType macReconfigPdcchAggAndPowerOffsetParams (
        UInt32                     schParamBitmapVal,
        RrmMacReconfigSchedulerReq *rrmMacReconfigSchedulerReq_p)
{
    UInt32  count                   = 0;
    UInt32  retVal                  = MAC_SUCCESS;
    UInt32  index                   = 0;
    UInt32  idx                     = 0;
    UInt32  id                      = 0;
    UInt32  aggLevel                = 0;
    UInt32  powerOffset             = 0;
    /*Read the PdcchAggregationAndPowerOffset Structure Value*/
    if(schParamBitmapVal & PDCCH_AGG_AND_POW_OFF_BIT)
    {
        for(index = 0; index < NUM_DCI_SIZE_CATEGORY; index++)
        {
            for(idx = 0; idx < MAX_CQI_VAL; idx++)
            {
                count = rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.
                    aggregationPowerOffsetCCUser[index][idx].count;
                if(count <= MAX_NUM_TPC_FOR_PUCCH)
                {
                    for(id = 0; id < count; id++)
                    {
                        aggLevel = rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.
                            aggregationPowerOffsetCCUser[index][idx].
                            aggregationPowerOffsetTuples[id].aggregationLevel;
                        retVal = validateAggLevel(aggLevel);
                        if(MAC_FAILURE == retVal)
                        {
                            return MAC_FAILURE;
                        }
                        powerOffset = rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.
                            aggregationPowerOffsetCCUser[index][idx].
                            aggregationPowerOffsetTuples[id].powerOffset;
                        retVal = validatePowerOffset(powerOffset);
                        if(MAC_FAILURE == retVal)
                        {
                            return MAC_FAILURE;
                        }
                    }
                }
                else
                {
                    return MAC_FAILURE;
                }
            }

            /*Read the PdcchAggregationAndPowerOffset Structure Value for CE user */ 
            if(rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.bitmask & 
                    AGGREGATION_POWER_OFFSET_CE_USER)
            {
                for(index = 0; index < NUM_DCI_SIZE_CATEGORY; index++)
                {
                    for(idx = 0; idx < MAX_CQI_VAL; idx++)
                    {
                        count = rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.
                            aggregationPowerOffsetCEUser[index][idx].count;
                        if(count <= MAX_NUM_TPC_FOR_PUCCH)
                        {
                            for(id = 0; id < count; id++)
                            {
                                aggLevel = rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.
                                    aggregationPowerOffsetCEUser[index][idx].
                                    aggregationPowerOffsetTuples[id].aggregationLevel;
                                retVal = validateAggLevel(aggLevel);
                                if(MAC_FAILURE == retVal)
                                {
                                    return MAC_FAILURE;
                                }
                                powerOffset = rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.
                                    aggregationPowerOffsetCEUser[index][idx].
                                    aggregationPowerOffsetTuples[id].powerOffset;
                                retVal = validatePowerOffset(powerOffset);
                                if(MAC_FAILURE == retVal)
                                {
                                    return MAC_FAILURE;
                                }
                            }
                        }
                        else
                        {
                            return MAC_FAILURE;
                        }
                    }
                }
            }
        }
    }
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name    : macReconfigSpsSchedulingInfo 
 * Inputs           : schParamBitmapVal - Scheduler param bitmap value
 *                    rrmMacReconfigSchedulerReq_p - Pointer to structure
 *                    RrmMacReconfigSchedulerReq 
 * Output           : None        
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function reconfigures SPS scheduling information 
 *                    Scheduler parameters
 *****************************************************************************/
static  MacRetType macReconfigSpsSchedulingInfo (
        UInt32                     schParamBitmapVal,
        RrmMacReconfigSchedulerReq *rrmMacReconfigSchedulerReq_p)
{
    UInt32  count                   = 0;
    UInt32  retVal                  = MAC_SUCCESS;
    UInt32  index                   = 0;
    UInt32  idx                     = 0;
    SInt32  startPL                 = 0;
    SInt32  endPL                   = 0;

    /*Read the SpsDlSchedulingInfoPerTti Structure Value*/
    if(schParamBitmapVal & SPS_DL_SCHEDULING_INFO_PER_TTI_BIT)
    {
        retVal = validateSpsDlSchedulingInfoPerTti(
                &(rrmMacReconfigSchedulerReq_p->spsDlSchInfoPerTti));
        if(MAC_FAILURE == retVal)
        {
            return MAC_FAILURE;
        }
    }
    /*Read the SpsUlSchedulingInfoPerTti Structure Value*/
    if(schParamBitmapVal & SPS_UL_SCHEDULING_INFO_PER_TTI_BIT)
    {
        retVal = validateSpsUlSchedulingInfoPerTti(
                &(rrmMacReconfigSchedulerReq_p->spsUlSchInfoPerTti));
        if(MAC_FAILURE == retVal)
        {
            return MAC_FAILURE;
        }
    }
    /*Read the alphaBasedPathlossToTargetSINRMapSps Structure Value*/
    if(schParamBitmapVal & ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_SPS_BIT)
    {
        for(index = 0; index < MAX_NUM_ALPHA_COUNT; index++)
        {
            count = rrmMacReconfigSchedulerReq_p->alphaBasedPathLossToTargetSINRMapSps.\
                    pathLossToTargetSINRMap[index].count;
            if((count <= MAX_NUM_PATHLOSS_TO_SINR_COUNT))
            {
                for(idx = 0; idx < count; idx++)
                {
                    startPL = rrmMacReconfigSchedulerReq_p->alphaBasedPathLossToTargetSINRMapSps.\
                              pathLossToTargetSINRMap[index].pathLossToTargetSINRMapInfo[idx].
                              startPL;
                    endPL = rrmMacReconfigSchedulerReq_p->alphaBasedPathLossToTargetSINRMapSps.\
                            pathLossToTargetSINRMap[index].pathLossToTargetSINRMapInfo[idx].
                            endPL ;
                    retVal = validateStartEndPL(startPL,endPL);
                    if(MAC_FAILURE == retVal)
                    {
                        return MAC_FAILURE;
                    }
                }
            }
            else
            {
                return MAC_FAILURE;
            }
        }
    }
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name    : macUpdateULSchedulerReconfigParams
 * Inputs           : schParamBitmapVal - Scheduler param bitmap value
 *                    rrmMacReconfigSchedulerReq_p - Pointer to structure
 *                    RrmMacReconfigSchedulerReq 
 *                    schedulerParamsUL_p - Pointer to structure
 *                    MacReconfigSchedulerParamsUL,internalCellIndex
 *                    internalCellIndex - Cell-Index at MAC
 * Output           : None        
 * Returns          : TRUE/FALSE 
 * Description      : This function updates UL Scheduler reconfig parameters.
 *****************************************************************************/
static  UInt32 macUpdateULSchedulerReconfigParams (
        UInt32                       schParamBitmapVal,
        RrmMacReconfigSchedulerReq   *rrmMacReconfigSchedulerReq_p,
        MacReconfigSchedulerParamsUL *schedulerParamsUL_p,
        InternalCellIndex  internalCellIndex)
{
    UInt32 ulFlag = FALSE;
    if(schParamBitmapVal & TPC_FOR_MSG3_BIT)
    {
        schedulerParamsUL_p->tpcForMsg3 = rrmMacReconfigSchedulerReq_p->tpcForMsg3;
        ulFlag = TRUE;
    }
    if(schParamBitmapVal & SRS_SUBBAND_GRP_SIZE_BIT)
    {
        schedulerParamsUL_p->srsSubBandGroupSize =
            rrmMacReconfigSchedulerReq_p->srsSubBandGroupSize;
        updatePrbToSINRGroupIndexMap(schedulerParamsUL_p->srsSubBandGroupSize,
                /*CA Changes start  */
                !schParamsIndexUL_g[internalCellIndex], internalCellIndex);
        /*CA Changes end  */
        ulFlag = TRUE;
    }
    if(schParamBitmapVal & TPC_TRIGGER_WAIT_TIME_PUSCH_DCI0_BIT)
    {
        schedulerParamsUL_p->tpcTriggerWaitTimeForPUSCHDCI0 =
            rrmMacReconfigSchedulerReq_p->tpcTriggerWaitTimeForPUSCHDCI0;
        ulFlag = TRUE;
    }
    if(schParamBitmapVal & TPC_NUM_UE_THRESHOLD_PUSCH_DCI3A)
    {
        schedulerParamsUL_p->tpcNumUeThresholdForPUSCHDCI3A =
            rrmMacReconfigSchedulerReq_p->tpcNumUeThresholdForPUSCHDCI3A;
        ulFlag = TRUE;
    }
    if(schParamBitmapVal & COOL_OFF_PERIOD_PUSCH_BIT)
    {
        schedulerParamsUL_p->coolOffPeriodForPUSCHTPC =
            rrmMacReconfigSchedulerReq_p->coolOffPeriodForPUSCHTPC;
        ulFlag = TRUE;
    }
    if(schParamBitmapVal & ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_BIT)
    {
        schedulerParamsUL_p->alphaBasedPathLossToTargetSINRMap =
            rrmMacReconfigSchedulerReq_p->alphaBasedPathLossToTargetSINRMap;
        populatePathlossToTargetSINRMapForPUSCH(&rrmMacReconfigSchedulerReq_p->
                alphaBasedPathLossToTargetSINRMap,schedulerParamsUL_p,LTE_FALSE);
        ulFlag = TRUE;
    }
    if(schParamBitmapVal & DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_BIT)
    {
        populateSinrToTpcMapForPUSCH(&rrmMacReconfigSchedulerReq_p->
                deltaSINRToTPCMapForPUSCH,schedulerParamsUL_p);
        ulFlag = TRUE;
    }
    if (schParamBitmapVal & MIN_RB_FOR_PL_PHR_CALC_BIT)
    {
        schedulerParamsUL_p->minRbForPlPhrCalc = rrmMacReconfigSchedulerReq_p->minRbForPlPhrCalc;
        ulFlag = TRUE;
    }

    /*UL_MU_MIMO_CHG_START*/
    /*Check if the UPLINK_MU_MIMO_TYPE_BIT is set*/
    if(schParamBitmapVal & UPLINK_MU_MIMO_TYPE_BIT)
    {
        if (rrmMacReconfigSchedulerReq_p->ulMUMIMOType >= MAX_UL_MU_MIMO_TYPE_VALUE)
        {
            return MAC_FAILURE;
        }
        rrmMacCellGlobal_gp[internalCellIndex]->ulMUMIMORrmConfigInfo.ulMUMIMOType = 
            rrmMacReconfigSchedulerReq_p->ulMUMIMOType;
        ulFlag = TRUE;
    }

    /*Check if the UPLINK_MU_MIMO_MCS_THRESHOLD_BIT is set*/
    if(schParamBitmapVal & UPLINK_MU_MIMO_MCS_THRESHOLD_BIT)
    {
        /*UL MU MIMO Review Comment Fix*/
        if(rrmMacReconfigSchedulerReq_p->mcsThresholdULMUMIMO > MAX_MCS_UL)
        {
            return MAC_FAILURE;
        }
        rrmMacCellGlobal_gp[internalCellIndex]->ulMUMIMORrmConfigInfo. \
            ulMUMIMOMcsThreshold = rrmMacReconfigSchedulerReq_p->mcsThresholdULMUMIMO;

        ulFlag = TRUE;
    }
    /*UL_MU_MIMO_CHG_END*/

    /* +DYNAMIC_ICIC */
    if(schParamBitmapVal & ATB_CONFIG_BIT)
    {
        schedulerParamsUL_p->atbConfigUplink.minMCSIndexForATB =
            rrmMacReconfigSchedulerReq_p->atbConfig.minMCSIndexForATB;
        schedulerParamsUL_p->atbConfigUplink.minPRBValForATB =
            rrmMacReconfigSchedulerReq_p->atbConfig.minPRBValForATB;
        ulFlag = TRUE;

        minMCSIndexForATB_g[internalCellIndex] = schedulerParamsUL_p->atbConfigUplink.minMCSIndexForATB;
        minPRBValForATB_g[internalCellIndex]   = schedulerParamsUL_p->atbConfigUplink.minPRBValForATB;
    }
    /* -DYNAMIC_ICIC */
    if(schParamBitmapVal & SPS_UL_SCHEDULING_INFO_PER_TTI_BIT)
    {
        memCpy(&(schedulerParamsUL_p->spsUlSchInfoPerTti),
                &(rrmMacReconfigSchedulerReq_p->spsUlSchInfoPerTti),
                sizeof(SpsUlSchedulingInfoPerTti));
        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_SPS_UL_SCH_INFO_PER_TTI_STATS(&(schedulerParamsUL_p->\
                    spsUlSchInfoPerTti), internalCellIndex);
        /* CA Stats Changes End */
        ulFlag = TRUE;
    }
    if(schParamBitmapVal & ALPHA_BASED_PATH_LOSS_TO_SINR_MAP_SPS_BIT)
    {
        schedulerParamsUL_p->alphaBasedPathLossToTargetSINRMapSps =
            rrmMacReconfigSchedulerReq_p->alphaBasedPathLossToTargetSINRMapSps;
        populatePathlossToTargetSINRMapForPUSCH(&rrmMacReconfigSchedulerReq_p->
                alphaBasedPathLossToTargetSINRMapSps,schedulerParamsUL_p,LTE_TRUE);
        ulFlag = TRUE;
    }
    return ulFlag;
}

/******************************************************************************
 * Function Name    : macUpdateDLSchedulerReconfigParams 
 * Inputs           : schParamBitmapVal - Scheduler param bitmap value
 *                    rrmMacReconfigSchedulerReq_p - Pointer to structure
 *                    RrmMacReconfigSchedulerReq 
 *                    schedulerParamsDL_p - Pointer to structure
 *                    MacReconfigSchedulerParamsDL
 *                    internalCellIndex - Cell-Index at MAC
 * Output           : None        
 * Returns          : TRUE/FALSE 
 * Description      : This function updates DL Scheduler reconfig parameters.
 *****************************************************************************/
STATIC  UInt32 macUpdateDLSchedulerReconfigParams (
        UInt32                       schParamBitmapVal,
        RrmMacReconfigSchedulerReq   *rrmMacReconfigSchedulerReq_p,
        MacReconfigSchedulerParamsDL *schedulerParamsDL_p,
        InternalCellIndex  internalCellIndex)
{
    UInt32 dlFlag = FALSE;
    UInt16 count  = 0;
    /* CA changes start */
    UInt32 bitmask = 0;
    /* CA changes end*/

    if(schParamBitmapVal & TPC_TRIGGER_WAIT_TIME_PUCCH_DCI1x2x_BIT)
    {
        schedulerParamsDL_p->tpcTriggerWaitTimeForPUCCHDCI1x2x =
            rrmMacReconfigSchedulerReq_p->tpcTriggerWaitTimeForPUCCHDCI1x2x;
        dlFlag = TRUE;
    }
    if(schParamBitmapVal & TPC_NUM_UE_THRESHOLD_PUCCH_DCI3A)
    {
        schedulerParamsDL_p->tpcNumUeThresholdForPUCCHDCI3A =
            rrmMacReconfigSchedulerReq_p->tpcNumUeThresholdForPUCCHDCI3A;
        dlFlag = TRUE;
    }

    if(schParamBitmapVal & COOL_OFF_PRD_FOR_BLER_BIT)
    {
        schedulerParamsDL_p->coolOffPeriodForBLERTriggeredPUCCHTPC =
            rrmMacReconfigSchedulerReq_p->coolOffPeriodForBLERTriggeredPUCCHTPC;
        dlFlag = TRUE;
    }
    if(schParamBitmapVal & COOL_OFF_PERIOD_FOR_PUCCH_BIT)
    {
        schedulerParamsDL_p->coolOffPeriodForPUCCHTPC =
            rrmMacReconfigSchedulerReq_p->coolOffPeriodForPUCCHTPC;
        dlFlag = TRUE;
    }
    if(schParamBitmapVal & BLER_TO_TPC_MAP_FOR_PUCCH)
    {
        schedulerParamsDL_p->deltaBlerToTPCMapForPUCCH.count = 
            rrmMacReconfigSchedulerReq_p->deltaBlerToTPCMapForPUCCH.count;
        schedulerParamsDL_p->deltaBlerToTPCMapForPUCCH.pucchBLERTarget
            = rrmMacReconfigSchedulerReq_p->deltaBlerToTPCMapForPUCCH.pucchBLERTarget;

        memCpy(schedulerParamsDL_p->deltaBlerToTPCMapForPUCCH.deltaBlerToTPCMapForPUCCHInfo,
                rrmMacReconfigSchedulerReq_p->deltaBlerToTPCMapForPUCCH.deltaBlerToTPCMapForPUCCHInfo,
                sizeof(DeltaBLERToTPCMapForPUCCHInfo)*MAX_NUM_TPC_FOR_PUCCH);

        initHarqBlerToTPCMap(&rrmMacReconfigSchedulerReq_p->
                deltaBlerToTPCMapForPUCCH,schedulerParamsDL_p);
        dlFlag = TRUE;
    }
    if(schParamBitmapVal & DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_BIT)
    {
        schedulerParamsDL_p->deltaSinrToTPCMapForPUCCH = 
            rrmMacReconfigSchedulerReq_p->deltaSINRToTPCMapForPUCCH;
        initSinrToTpcMap(&rrmMacReconfigSchedulerReq_p->
                deltaSINRToTPCMapForPUCCH,schedulerParamsDL_p);
        dlFlag = TRUE;
    }
    if(schParamBitmapVal & PDCCH_AGG_AND_POW_OFF_BIT)
    {
        memCpy(schedulerParamsDL_p->pdcchAggregationAndPowerOffset[CC_USER].aggregationPowerOffset,
                rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.
                aggregationPowerOffsetCCUser,
                sizeof(AggregationPowerOffset) * NUM_DCI_SIZE_CATEGORY * MAX_CQI_VAL);
        if(rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.bitmask & 
                AGGREGATION_POWER_OFFSET_CE_USER)
        {
            memCpy(schedulerParamsDL_p->pdcchAggregationAndPowerOffset[CE_USER].aggregationPowerOffset,
                    rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.
                    aggregationPowerOffsetCEUser,
                    sizeof(AggregationPowerOffset) * NUM_DCI_SIZE_CATEGORY * MAX_CQI_VAL);
        }
        else
        {
            memCpy(schedulerParamsDL_p->pdcchAggregationAndPowerOffset[CE_USER].aggregationPowerOffset,
                    rrmMacReconfigSchedulerReq_p->pdcchAggregationAndPowerOffset.
                    aggregationPowerOffsetCCUser,
                    sizeof(AggregationPowerOffset) * NUM_DCI_SIZE_CATEGORY * MAX_CQI_VAL);
        }
        dlFlag = TRUE;
    }
    if(schParamBitmapVal & CQI_TO_PHICH_POW_BIT)
    {
        schedulerParamsDL_p->cqiToPHICHPower = rrmMacReconfigSchedulerReq_p->cqiToPHICHPower;
        dlFlag = TRUE;
    }

    /* +DYNAMIC_ICIC */
    if(schParamBitmapVal & PA_QPSK_BIT)
    {
        schedulerParamsDL_p->paQpsk = rrmMacReconfigSchedulerReq_p->paQpsk;
        dlFlag = TRUE;
    }
    /* -DYNAMIC_ICIC */

    if(schParamBitmapVal & SPS_DL_SCHEDULING_INFO_PER_TTI_BIT)
    {
        memCpy(&(schedulerParamsDL_p->spsDlSchInfoPerTti),
                &(rrmMacReconfigSchedulerReq_p->spsDlSchInfoPerTti),
                sizeof(SpsDlSchedulingInfoPerTti));
        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_SPS_DL_SCH_INFO_PER_TTI_STATS(&(schedulerParamsDL_p->\
                    spsDlSchInfoPerTti), internalCellIndex);
        /* CA Stats Changes End */
        dlFlag = TRUE;
    }

    /* + TM7_8 Changes Start */
    if(schParamBitmapVal & AOA_TO_BF_VECTOR_BIT)
    {
        for (count = 0; count < MAX_NUM_BF_VECTORS_TO_AOA_VAL; count++)
        {
            schedulerParamsDL_p->angleOfArrivalToBeamFormingVectorMapping[count] =
                rrmMacReconfigSchedulerReq_p->
                angleOfArrivalToBeamFormingVectorMapping[count];
        }
        schedulerParamsDL_p->bfVectorPresentFlag = SINR_BASED_BF_ALGO; 
        dlFlag = TRUE;
    }

    if(schParamBitmapVal & SINR_TO_BF_VECTOR_BIT)
    {
        for (count = 0; count < MAX_NUM_BF_VECTORS_TO_SINR_VAL; count++)
        {
            schedulerParamsDL_p->
                sinrToBeamFormingVectorMapping[count].beamFormingVectorForA1 =
                rrmMacReconfigSchedulerReq_p->
                sinrToBeamFormingVectorMapping[count].beamFormingVectorForA1 ;
        }
        schedulerParamsDL_p->bfVectorPresentFlag = SINR_BASED_BF_ALGO; 
        dlFlag = TRUE;
    }
    /* - TM7_8 Changes Start */
    /*+ Dyn UEs To Be SchPerTTI:CR +*/
    if(schParamBitmapVal & UES_TO_BE_SCHEDULED_DL_UL_PER_TTI)
    {
        schedulerParamsDL_p->maxUEsToBeScheduledDlUl.maxUEsToBeScheduledDL = 
            rrmMacReconfigSchedulerReq_p->maxUEsToBeScheduledDlUl.maxUEsToBeScheduledDL;
        schedulerParamsDL_p->maxUEsToBeScheduledDlUl.maxUEsToBeScheduledUL =
            rrmMacReconfigSchedulerReq_p->maxUEsToBeScheduledDlUl.maxUEsToBeScheduledUL;
        modifyMaxUeToBeSchdDlUl_g[internalCellIndex] = MODIFY_ONLY;
        dlFlag = TRUE;
    }
    /*- Dyn UEs To Be SchPerTTI:CR -*/

    /* CA Changes Start */
    /*Check if the CA_PARAMS_BIT is set*/
    if(schParamBitmapVal & CA_PARAMS_BIT)
    {
        bitmask = rrmMacReconfigSchedulerReq_p->caParams.bitMask;
        if(!bitmask)
        {
            dlFlag = FALSE;
        }
        else 
        {
            if(bitmask & LOAD_CAL_WINDOW_SIZE)
            {
                schedulerParamsDL_p->caParams.loadCalcualtionWindowSize =
                    rrmMacReconfigSchedulerReq_p->caParams.loadCalcualtionWindowSize;
                dlFlag = TRUE;
            }
            if(bitmask & LOAD_DISPARITY_THRESHOLD)
            {
                schedulerParamsDL_p->caParams.loadDisparityThreshold =
                    rrmMacReconfigSchedulerReq_p->caParams.loadDisparityThreshold;
                dlFlag = TRUE;
            }
            if(bitmask & MCS_DIFF_THRESHOLD)
            {
                schedulerParamsDL_p->caParams.mcsDiffThreshold =
                    rrmMacReconfigSchedulerReq_p->caParams.mcsDiffThreshold;
                dlFlag = TRUE;
            }
            if(bitmask & PRB_USAGE_WATER_MARK)
            {
                schedulerParamsDL_p->caParams.prbUsageWaterMark =
                    rrmMacReconfigSchedulerReq_p->caParams.prbUsageWaterMark;
                dlFlag = TRUE;
            }
            if(bitmask & DL_DEACT_COMMON_THREAD)
            {
                schedulerParamsDL_p->caParams.dlDeactCommandThresh=
                    rrmMacReconfigSchedulerReq_p->caParams.dlDeactCommandThresh;
                dlFlag = TRUE;
            }
            if(bitmask & DL_DEACT_MCS_LOW_THRESH)
            {
                schedulerParamsDL_p->caParams.dlDeactMCSLowThresh=
                    rrmMacReconfigSchedulerReq_p->caParams.dlDeactMCSLowThresh;
                dlFlag = TRUE;
            }
            if(bitmask & DL_QLOAD_UP_THRESH)
            {
                /*spr 12463 fix start*/
                schedulerParamsDL_p->caParams.dlQLoadUpThresh=
                    rrmMacReconfigSchedulerReq_p->caParams.dlQLoadUpThresh * RRM_SCELL_QLOAD_THRESH_MULTIPLIER;
                /*spr 12463 fix end*/
                dlFlag = TRUE;
            }
            if(bitmask & DL_QLOAD_DOWN_THRESH)
            {
                /*spr 12463 fix start*/
                schedulerParamsDL_p->caParams.dlQLoadLowThresh=
                    rrmMacReconfigSchedulerReq_p->caParams.dlQLoadLowThresh * RRM_SCELL_QLOAD_THRESH_MULTIPLIER;
                /*spr 12463 fix end*/    
                dlFlag = TRUE;
            }
            if(bitmask & ACT_DEACT_TIG_COUNT_THRESH)
            {
                schedulerParamsDL_p->caParams.actDeactTriggerCountThresh =
                    rrmMacReconfigSchedulerReq_p->caParams.actDeactTriggerCountThresh;
                dlFlag = TRUE;
            }
            /* SPR 16422 START */
            if(bitmask & CQI_VALIDITY_TIMER)
            {
                schedulerParamsDL_p->caParams.cqiValidityTimer =
                    rrmMacReconfigSchedulerReq_p->caParams.cqiValidityTimer;
                dlFlag = TRUE;
            }
            /* SPR 16422 END */
        }
    }
    /* CA Changes End */
#ifdef LTE_EMBMS_SUPPORTED
    if(schParamBitmapVal & EMBMS_PARAM_BIT)
    {
        schedulerParamsDL_p->eMbmsReservedCellPowerOffset = \
        rrmMacReconfigSchedulerReq_p->eMbmsReservedCellPowerOffset;
        dlFlag = TRUE;
    }
#endif


    return dlFlag;
}

/*UL_MU_MIMO_CHG Start*/

/************************************************************************
 * Function Name    : processULUeMUMIMOQueue 
 * Inputs           : queueCount - count of nodes in queue
 *                    internalCellIndex
 * Returns          : MAC_SUCCESS
 * Description      : This function updates the MU MIMO Status UE in UL UE 
 *                    context
 ************************************************************************/
MacRetType processULUeMUMIMOQueue(UInt32 queueCount, 
                    InternalCellIndex  internalCellIndex)
{
    UEULMUMIMOInfoNode *ulUEMUMIMONode_p = PNULL;
    ULUEContext *ueULContect_p = PNULL;
    ULUEContextInfo *ueULContextInfo_p = PNULL;

    while(queueCount--)
    {
        popNodeCircQ(&rrmMacCellGlobal_gp[internalCellIndex]-> \
                ulUEReconfigMUMIMOQueue, (void *) &ulUEMUMIMONode_p);
        if(ulUEMUMIMONode_p != PNULL)
        {
            ueULContextInfo_p = &ulUECtxInfoArr_g[ulUEMUMIMONode_p->ueIndex];
            ueULContect_p = ueULContextInfo_p->ulUEContext_p;
            /*Update the MU MIMO Status in UL UE Context*/
            if (!(ueULContextInfo_p->pendingDeleteFlag) || (ueULContect_p &&  \
                        internalCellIndex == 
                        ueULContect_p->internalCellIndex))
            {
                ueULContect_p->ulMUMIMOInfo.ulMUMIMOStatus = 
                    ulUEMUMIMONode_p->ulMUMIMOStatus;
            }

            freeMemPool(ulUEMUMIMONode_p);
            ulUEMUMIMONode_p = PNULL; 
        }
    }
    return MAC_SUCCESS;
}

/*UL_MU_MIMO_CHG End*/


/************************************************************************
 * Function Name    : processDLUELocationTypeQueue 
 * Inputs           : queueCount - count of nodes in queue
 *                    internalCellIndex
 * Returns          : MAC_SUCCESS
 * Description      : This function updates the user location in DL UE 
 *                    context
 ************************************************************************/
MacRetType processDLUELocationTypeQueue(UInt32 queueCount, 
                            InternalCellIndex  internalCellIndex)
{
    UELocationNode *ueLocationNode_p = PNULL;
    DLUEContext *ueDLContext_p          = PNULL;
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    /*ca changes start*/
    UInt8 cellBitMask = 0;
    UInt8 index =0;
    InternalCellIndex internalCellIndexOfScell = 0;
    /*ca changes end*/

    while(queueCount--)
    {
        /* DL UE Context Update */
        DEQUEUE_DL_UE_RECONFIG_DATA_Q(UELocationNode, 
                (void *) &ueLocationNode_p, 
                internalCellIndex);
        if (ueLocationNode_p != PNULL)
        {
            /* updating user location type in DL Context */
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueLocationNode_p->ueIndex];
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
            if (!(ueDLContextInfo_p->pendingDeleteFlag) || (ueDLContext_p && \
                        internalCellIndex == ueDLContext_p->internalCellIndex))
            {
                /* In case CC User becomes CE then increment totalCellEdgeUserCount*/ 
                if(ueLocationNode_p->ueLocationType == CE_USER
                        &&  ueDLContext_p->userLocationType == CC_USER)
                {
                    rrmMacCellGlobal_gp[internalCellIndex]->totalCellEdgeUserCount++;
                }

                /* In case CE User becomes CC then decrement totalCellEdgeUserCount*/ 
                if(ueLocationNode_p->ueLocationType == CC_USER
                        &&  ueDLContext_p->userLocationType == CE_USER)
                {
                    rrmMacCellGlobal_gp[internalCellIndex]->totalCellEdgeUserCount--;
                }

                /*ca changes start*/
                cellBitMask = ueLocationNode_p->cellBitMask;
                if(cellBitMask)
                {
                    for(index = START_SCELL_INDEX; index <= MAX_NUM_SCELL; index++)
                    {
                        /*Check the bit of CellBitmask to get the cell index 
                         *   for which location info is to be applied bit 0 is 
                         *   not used as Scell index valid from 1 to 7 */
                        if(cellBitMask & (1 << index))
                        {
                            DLUESCellContext *dlScellContext_p = 
                                (ueDLContext_p->dlUeScellContext_p[index]);
                            if(NULL != dlScellContext_p)
                            {
                                internalCellIndexOfScell = 
                                    dlScellContext_p->internalCellIndex;
                                /* In case CC User becomes CE then increment 
                                 *   totalCellEdgeUserCount*/ 
                                if(ueLocationNode_p->ueLocationPerScell[index] ==
                                        CE_USER &&
                                        dlScellContext_p->userLocationType == 
                                        CC_USER)
                                {
                                    rrmMacCellGlobal_gp \
                                        [internalCellIndexOfScell]-> \
                                        totalCellEdgeUserCount++;
                                }
                                /* In case CE User becomes CC then decrement 
                                 *   totalCellEdgeUserCount */ 
                                if(ueLocationNode_p->ueLocationPerScell[index] ==
                                        CC_USER &&
                                        dlScellContext_p->userLocationType == 
                                        CE_USER)
                                {
                                    rrmMacCellGlobal_gp[ \
                                        internalCellIndexOfScell]-> \
                                        totalCellEdgeUserCount--;
                                }
                                dlScellContext_p->userLocationType = 
                                    ( UserLocation ) ueLocationNode_p-> \
                                    ueLocationPerScell[index];
                            }
                        }
                    } 
                }
                /*ca changes end*/

                /* + Coverity 54164 */
                ueDLContext_p->userLocationType = ( UserLocation ) ueLocationNode_p->ueLocationType;
                /* - Coverity 54164 */
                LOG_MAC_MSG(MAC_RRM_DL_USER_LOC_TYPE,LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        ueLocationNode_p->ueIndex,
                        ueLocationNode_p->ueLocationType,
                        rrmMacCellGlobal_gp[internalCellIndex]->totalCellEdgeUserCount,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
            }
            FREE_MEM_NODE_DL_UE_RECONFIG_DATA_Q(ueLocationNode_p);
        }
    }
    return MAC_SUCCESS;
}

/* EICIC +*/
/************************************************************************
 * Function Name    : processUEUserTypeQueue 
 * Inputs           : queueCount - count of nodes in queue
 *                    internalCellIndex
 * Returns          : MAC_SUCCESS
 * Description      : This function updates the user Type in DL \ UL UE context
 ************************************************************************/
MacRetType processUEUserTypeQueue(UInt32 queueCount, 
        InternalCellIndex  internalCellIndex)
{
    UEUserTypeNode *UEUserTypeNode_p = PNULL;
    DLUEContext *ueDLContext_p          = PNULL;
    ULUEContext *ueULContext_p          = PNULL;
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    ULUEContextInfo * ueULContextInfo_p = PNULL;
    ULSchedulerStrategyConfig *ulSchdConfig_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig);
    DLSchedulerStrategyConfig  *dlSchdConfig_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->dlSchdConfig);

    while(queueCount--)
    {
        /* DL UE Context Update */
        DEQUEUE_UE_RECONFIG_USER_TYPE_Q(UEUserTypeNode, 
                (void *) &UEUserTypeNode_p, 
                internalCellIndex);

        if (UEUserTypeNode_p != PNULL)
        {
            /* updating user location type in DL Context */
            ueDLContextInfo_p = &dlUECtxInfoArr_g[UEUserTypeNode_p->ueIndex];
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
            ueULContextInfo_p = &ulUECtxInfoArr_g[UEUserTypeNode_p->ueIndex];
            ueULContext_p = ueULContextInfo_p->ulUEContext_p;

            if (!(ueDLContextInfo_p->pendingDeleteFlag) || (ueDLContext_p && \
                        internalCellIndex == ueDLContext_p->internalCellIndex))
            {
                ueDLContext_p->userType = ( UserType ) UEUserTypeNode_p->userType;
                ueULContext_p->userType = ( UserType ) UEUserTypeNode_p->userType;

                if (DL_QOS_BASED_SCHEDULING  == dlSchdConfig_p->dlSchedulerStrategy)
                {
                    macReconfigureDlUeLCTree(internalCellIndex, ueDLContext_p);
                    macReconfigureDlUeGBR(internalCellIndex, ueDLContext_p);
                }
                if (UL_QOS_SCHEDULING == ulSchdConfig_p->ulSchedulerStrategy)
                {
                    macReconfigureUlUeLCTree(internalCellIndex, ueULContext_p);
                    macReconfigureUlUeGBR(internalCellIndex, ueULContext_p);
                }
            }

            FREE_MEM_NODE_DL_UE_RECONFIG_DATA_Q(UEUserTypeNode_p);
        }
    }
    return MAC_SUCCESS;
}

/* EICIC -*/

/************************************************************************
 * Function Name    : processULUELocationTypeQueue 
 * Inputs           : queueCount - count of nodes in queue 
 *                    internalCellIndex
 * Returns          : MAC_SUCCESS
 * Description      : This function updates user location in UL UE
 *                    context
 ************************************************************************/
MacRetType processULUELocationTypeQueue(UInt32 queueCount, 
                            InternalCellIndex internalCellIndex)
{
    UELocationNode *ueLocationNode_p = PNULL;
    ULUEContext *ueULContext_p          = PNULL;
    ULUEContextInfo * ueULContextInfo_p = PNULL;

    UInt32 noOfExpiredHops = 0;
    /* SPR 15909 fix start */
    tickType_t currentTick = 0;
    /* SPR 15909 fix end */
    while(queueCount--)
    {
        /* UL UE Context Update */
        DEQUEUE_UL_UE_RECONFIG_DATA_Q(UELocationNode, 
                (void *) &ueLocationNode_p, 
                internalCellIndex);
        if (ueLocationNode_p != PNULL)
        {
            /* updating user location type in UL Context */

            ueULContextInfo_p = &ulUECtxInfoArr_g[ueLocationNode_p->ueIndex];
            ueULContext_p = ueULContextInfo_p->ulUEContext_p;
            if (!(ueULContextInfo_p->pendingDeleteFlag) || (ueULContext_p && \
                        internalCellIndex == ueULContext_p->internalCellIndex))
            {
                ueULContext_p->userLocationType = ueLocationNode_p->ueLocationType;
                /* +DYNAMIC_ICIC */
                if (INVALID_PHR_GRANT != ueULContext_p->phrGrant) 
                { 
                    currentTick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex) + 
                        PHR_GUARD_TIMER; 
                    if ( currentTick >= ueULContext_p->phrProhibitTimerExpiryTick )
                    {
                        if(ueULContext_p->phrTimerCfg.prohibitPHRTimer)
                        {
                            /* Calculate the next possible Expiry Tick */
                            noOfExpiredHops = 
                                ((currentTick - ueULContext_p->phrProhibitTimerExpiryTick)/
                                 ueULContext_p->phrTimerCfg.prohibitPHRTimer);
                            noOfExpiredHops = noOfExpiredHops + 1;
                            ueULContext_p->phrProhibitTimerExpiryTick = 
                                ueULContext_p->phrProhibitTimerExpiryTick + \
                                (noOfExpiredHops * ueULContext_p->phrTimerCfg.prohibitPHRTimer);
                        }
                        else
                        {
                            ueULContext_p->phrProhibitTimerExpiryTick = currentTick + 1;
                        }

                    }
                    /*CA Changes start  */
                    putEntryInExplicitULDCIAndPHRScheduleQueue(
                            ueULContext_p->ueIndex, 
                            UL_EXPLICIT_PHR_WITHOUT_DATA, 
                            internalCellIndex);
                    /*CA Changes end  */
                }
            }
            FREE_MEM_NODE_UL_UE_RECONFIG_DATA_Q(ueLocationNode_p);
        }
    }

    return MAC_SUCCESS;
}

/* + TM7_8 Changes Start*/ 
/************************************************************************
 * Function Name    : processUeNumOfLayerQueue 
 * Inputs           : queueCount -> count of node in queue. 
 *                    internalCellIndex 
 * Returns          : MAC_SUCCESS
 * Description      : This function updates num of layer in UE
 *                    context
 ************************************************************************/
MacRetType processUeNumOfLayerQueue (UInt32 queueCount, 
                        InternalCellIndex  internalCellIndex)
{
    UENumOfLayerNode  *ueNumOfLayerNode_p = PNULL;
    DLUEContext *ueDLContext_p          = PNULL;
    DLUEContextInfo * ueDLContextInfo_p = PNULL;

    while(queueCount--)
    {
        /* Pop the Node to update the number of Layers */
        popNodeCircQ(&rrmMacCellGlobal_gp[internalCellIndex]-> \
                ueReconfigNumOfLayerQueue, (void *) &ueNumOfLayerNode_p);
        if(ueNumOfLayerNode_p!= PNULL)
        {
            /* Update the NumOf Layer in DL UE Context */
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueNumOfLayerNode_p->ueIndex];
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

            if ( !(ueDLContextInfo_p->pendingDeleteFlag) || (ueDLContext_p &&\
                        internalCellIndex == ueDLContext_p->internalCellIndex))
            {
                if ( (TX_MODE_8 == ueDLContext_p->dlMIMOInfo.transmissionMode) &&
                        (INVALID_PMI_RI_CONF_V920 == ueDLContext_p->isPmiRiConfigured_V920) )
                {
                    ueDLContext_p->dlMIMOInfo.dlRi = ueNumOfLayerNode_p->numOfLayer;
                }
            }
            freeMemPool(ueNumOfLayerNode_p);
            ueNumOfLayerNode_p = PNULL;
        }
    }
    return MAC_SUCCESS;
}

/************************************************************************
 * Function Name    : processUeBeamFormingAlgoQueue 
 * Inputs           : queueCount - count of nodes in queue 
 *                    internalCellIndex
 * Returns          : MAC_SUCCESS
 * Description      : This function updates Beam forming algo in UE
 *                    context
 ************************************************************************/
MacRetType processUeBeamFormingAlgoQueue (UInt32 queueCount,
                                       InternalCellIndex  internalCellIndex )
{
    UEBeamformingAlgoNode  *ueBeamformingAlgoNode_p = PNULL;
    DLUEContext *ueDLContext_p          = PNULL;
    DLUEContextInfo *ueDLContextInfo_p = PNULL;

    while(queueCount--)
    {
        /* Pop the Node to update the Beam Forming Algo */
        popNodeCircQ(&rrmMacCellGlobal_gp[internalCellIndex]-> \
                ueReconfigBeamFormingAlgoQueue, (void *) &ueBeamformingAlgoNode_p);
        if(ueBeamformingAlgoNode_p!= PNULL)
        {
            /* Update the NumOf Layer in DL UE Context */
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueBeamformingAlgoNode_p->ueIndex];
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

            if ((!(ueDLContextInfo_p->pendingDeleteFlag) || (ueDLContext_p && \
                            internalCellIndex == ueDLContext_p->internalCellIndex)))
            {
                ueDLContext_p->beamFormingAlgoToBeUsed = 
                    ueBeamformingAlgoNode_p->beamformingAlgoToBeUsed;
            }
            freeMemPool(ueBeamformingAlgoNode_p);
            ueBeamformingAlgoNode_p = PNULL;
        }
    }
    return MAC_SUCCESS;
}
/* - TM7_8 Changes End*/ 

/* ca changes start*/
/************************************************************************
 * Function Name    : macProcessUeReconfigScellSchedulingQueue 
 * Inputs           : queueCount -> count of node in queue 
 *                    internalCellIndex
 * Returns          : MAC_SUCCESS
 * Description      : This function updates Ue Reconfig Scell Scheduling Info 
 *                    in Ue context
 ************************************************************************/
MacRetType macProcessUeReconfigScellSchedulingQueue (UInt32 queueCount,
                                       InternalCellIndex  internalCellIndex )
{
    UEScellSchedulingInfoNode   *ueScellSchedulingInfoNode_p    = PNULL;
    DLUEContext                 *ueDLContext_p                  = PNULL;
    DLUEContextInfo             *ueDLContextInfo_p              = PNULL;
    UInt8                       startIndex                      = 0;

    while(queueCount--)
    {
        /* Pop the Node to update the UE Reconfig Scell Scheduling Queue */
        DEQUEUE_UE_RECONFIG_SCELL_SCHEDULING_Q(
                rrmMacCellGlobal_gp[internalCellIndex]-> \
                ueReconfigScellSchedulingQueue,
                (void *) &ueScellSchedulingInfoNode_p,
                internalCellIndex);
        if(ueScellSchedulingInfoNode_p!= PNULL)
        {
            /* Update the UE Reconfig Scell Scheduling in DL UE Context */
            ueDLContextInfo_p = 
                &dlUECtxInfoArr_g[ueScellSchedulingInfoNode_p->ueIndex];
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

            if ((!(ueDLContextInfo_p->pendingDeleteFlag) || (ueDLContext_p && \
                            internalCellIndex == ueDLContext_p->internalCellIndex)))
            {
                ueDLContext_p->isMACCESendToUE = 
                    ueScellSchedulingInfoNode_p->isMACCESendToUE;
                ueDLContext_p->numOfScellsSchedulingAllowed = 
                    ueScellSchedulingInfoNode_p->numOfScellsSchedulingAllowed;
                ueDLContext_p->numOfScellsSchedulingNotAllowed = 
                    ueScellSchedulingInfoNode_p->numOfScellsSchedulingNotAllowed;
                for(startIndex = 0; 
                        startIndex < ueScellSchedulingInfoNode_p->numOfScellsSchedulingAllowed;
                        startIndex++)
                {
                    ueDLContext_p->schedulingActivateScell[startIndex] = 
                        ueScellSchedulingInfoNode_p->schedulingActivateScell[startIndex];
                }
                for(startIndex = 0; 
                        startIndex < ueScellSchedulingInfoNode_p->numOfScellsSchedulingNotAllowed;
                        startIndex++)
                {
                    ueDLContext_p->schedulingDeactivateScell[startIndex] = 
                        ueScellSchedulingInfoNode_p->schedulingDeactivateScell[startIndex];
                }
                /*spr 13013 fix start*/
                insertNodeInDlCAEventQueue(ueScellSchedulingInfoNode_p,ueDLContext_p);
                /*spr 13013 fix end*/
            }
            FREE_MEM_NODE_UE_RECONFIG_SCELL_SCHEDULING_Q(
                    ueScellSchedulingInfoNode_p);
        }
    }
    return MAC_SUCCESS;
}
/************************************************************************
 * Function Name    : insertNodeInDlCAEventQueue 
 * Inputs           : ueScellSchedulingInfoNode_p - UEScellSchedulingInfoNode 
 *                    DLUEContext *ueDLContext_p - dl ue context ptr 
 * Returns          : NONE
 * Description      : This function prepare and insert the event in CA Event Q
 ************************************************************************/
void insertNodeInDlCAEventQueue(UEScellSchedulingInfoNode * ueScellSchedulingInfoNode_p,
                                DLUEContext *ueDLContext_p)
{
    DLCAEventQueueNode * eventQNode_p = PNULL;
    UInt8                startIndex   = 0;

    /*prepare and insert Scell state transition event in scell event Q*/
    /*spr 12658 fix start*/
    for(startIndex = 0; startIndex < MAX_NUM_SCELL; startIndex++)
    {
        /*spr 21110 fix start*/
        if((ueScellSchedulingInfoNode_p->numOfScellsSchedulingNotAllowed) && (ueScellSchedulingInfoNode_p->schedulingDeactivateScell[startIndex]))
        {
            /*spr 13013 fix start*/
            if(ueDLContext_p->dlUeScellContext_p[startIndex + 1] != NULL)
            {
				ueDLContext_p->dlUeScellContext_p[startIndex + 1]->isScellActivateStatus = LTE_FALSE;
            }                                                         
            /*spr 13013 fix end*/ 
            GET_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p,DLCAEventQueueNode);  
            if(eventQNode_p != PNULL) 
            {        
                eventQNode_p->ueId = ueScellSchedulingInfoNode_p->ueIndex;
                eventQNode_p->scellId = startIndex + 1; 
				eventQNode_p->dlCAQosEvent = DL_CA_EVENT_RRM_SCELL_DEACTIVATION;
                /*spr 13013 fix start*/
                if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueDLContext_p->internalCellIndex],(void *)eventQNode_p))
                {
                    FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
                }
                /*spr 13013 fix end*/
            }
        }
/*spr 21110 start*/
		else if((ueScellSchedulingInfoNode_p->numOfScellsSchedulingAllowed) && (ueScellSchedulingInfoNode_p->schedulingActivateScell[startIndex]))
/*spr 21110 end*/
        {
            /*spr 13013 fix start*/
            if(ueDLContext_p->dlUeScellContext_p[startIndex + 1] != NULL)
            {
				ueDLContext_p->dlUeScellContext_p[startIndex + 1]->isScellActivateStatus = LTE_TRUE;   
            }
            /*spr 13013 fix end*/
            GET_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p,DLCAEventQueueNode);
            if(eventQNode_p != PNULL)
            {
                eventQNode_p->ueId = ueScellSchedulingInfoNode_p->ueIndex;
                eventQNode_p->scellId = startIndex + 1; 
				/*spr 12658 fix end*/
				eventQNode_p->dlCAQosEvent = DL_CA_EVENT_RRM_SCELL_ACTIVATION;    
                /*spr 13013 fix start*/
                if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueDLContext_p->internalCellIndex],(void *)eventQNode_p))
                {
                    FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
                }
                /*spr 13013 fix end*/
            }
        } 
        /*spr 21110 fix end*/
        else
        {   
            continue;
        }
    }
}
/************************************************************************
 * Function Name    : macProcessUeReconfigCqiValidityTimerQueue 
 * Inputs           : queueCount -> count of node in queue 
 *                    internalCellIndex
 * Returns          : MAC_SUCCESS
 * Description      : This function updates Ue Reconfig Cqi Validity Timer
 *                    Info in UE context
 ************************************************************************/
MacRetType macProcessUeReconfigCqiValidityTimerQueue (UInt32 queueCount,
                                           InternalCellIndex  internalCellIndex )
{
    UECqiValidityTimerInfoNode  *ueCqiValidityTimerInfoNode_p   = PNULL;
    DLUEContext *ueDLContext_p                                  = PNULL;
    DLUEContextInfo *ueDLContextInfo_p                          = PNULL;
    /* SPR 16422 START */
    UInt8 scell = 0;
    /* SPR 16422 END */
    while(queueCount--)
    {
        /* Pop the Node to update the UE Reconfig CQI Validity Timer Queue */
        DEQUEUE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(
                rrmMacCellGlobal_gp[internalCellIndex]->ueReconfigCqiValidityTimerQueue,
                (void *) &ueCqiValidityTimerInfoNode_p,
                internalCellIndex);
        if(ueCqiValidityTimerInfoNode_p!= PNULL)
        {
            /* Update the UE Reconfig CQI Validity Timer Queue in DL UE Context */
            ueDLContextInfo_p = &dlUECtxInfoArr_g[ueCqiValidityTimerInfoNode_p->ueIndex];
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;

            if ((!(ueDLContextInfo_p->pendingDeleteFlag) || (ueDLContext_p && \
                            internalCellIndex == ueDLContext_p->internalCellIndex)))
            {
                    /* SPR 16422 START */
                    /* Set the timer for all configured Scell */
                    for (scell = 0; scell < (MAX_NUM_SCELL + MAX_NUM_PCELL); scell++)
                    {
                            /* Check if SCell is configured at UE */
                            if (ueDLContext_p->dlUeScellContext_p[scell])
                            {
                                    ueDLContext_p->dlUeScellContext_p[scell]->cqiValidityTimer =
                                            ueCqiValidityTimerInfoNode_p->cqiValidityTimer;
                            }
                    }
                    /* SPR 16422 END */
            }
            FREE_MEM_NODE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(
                    ueCqiValidityTimerInfoNode_p);
        }
    }
    return MAC_SUCCESS;
}

/* CA Changes start */
/************************************************************************
 * Function Name    : initMacRRMInterface 
 * Inputs           : internalCellIndex 
 * Returns          : none
 * Description      : This function is used to init IPC interface and
 *                    circular queue to reconfig UE for both UL and DL
 *                    This is called at MAC_INIT_LAYER_REQ
 ************************************************************************/
void initMacRRMInterface(InternalCellIndex  internalCellIndex)
{
    /* CA Changes Start*/
    /*Creating Memory pool for UE Location Node */
    createMemPool(sizeof(UELocationNode),MAX_NODES*2);
    /*Creating Memory pool for DL/UL SchedulerParams */
    /* EICIC +*/
     createMemPool(sizeof(UEUserTypeNode),MAX_NODES*2);
    /* EICIC -*/
    createMemPool(sizeof(MacReconfigSchedulerParamsDL) * MAX_DL_SCH_PARAM_INDEX, 1);
    createMemPool(sizeof(MacReconfigSchedulerParamsUL) * MAX_UL_SCH_PARAM_INDEX, 1);
    /*Creating Memory pool for RRMMACCellGlobal */
    createMemPool(sizeof(RRMMACCellGlobal),1);

    GET_MEM_FROM_POOL(RRMMACCellGlobal, 
            rrmMacCellGlobal_gp[internalCellIndex],
            sizeof(RRMMACCellGlobal), PNULL);
    if( PNULL == rrmMacCellGlobal_gp[internalCellIndex] )
    {
        LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                internalCellIndex,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
    }
    /* SPR 11109 Fix start */
     memSet(rrmMacCellGlobal_gp[internalCellIndex],0,sizeof(RRMMACCellGlobal));
     /* SPR 11109 Fix end */

    /* Initializing SHM for Periodic Report */
    initSHMForRRMPeriodicReport(internalCellIndex);

    /* Initializing circular Queue */
    circQInit(&rrmMacCellGlobal_gp[internalCellIndex]->ulUEReconfigDataQueue,MAX_NODES);
    INIT_DL_UE_RECONFIG_DATA_Q(internalCellIndex);

    /* EICIC +*/
    INIT_UE_RECONFIG_USER_TYPE_Q(internalCellIndex);
    /* EICIC -*/

    /*UL_MU_MIMO_CHG Start*/
    circQInit(&rrmMacCellGlobal_gp[internalCellIndex]-> \
            ulUEReconfigMUMIMOQueue,MAX_UE_SUPPORTED);
    /*UL_MU_MIMO_CHG End*/
    /* + TM7_8 Changes Start */
    circQInit(&rrmMacCellGlobal_gp[internalCellIndex]-> \
            ueReconfigNumOfLayerQueue, MAX_UE_SUPPORTED);
    circQInit(&rrmMacCellGlobal_gp[internalCellIndex]-> \
            ueReconfigBeamFormingAlgoQueue, MAX_UE_SUPPORTED);
    /* - TM7_8 Changes End */

    INIT_UE_RECONFIG_SCELL_SCHEDULING_Q(internalCellIndex);
    INIT_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(internalCellIndex);

    /* Initializing paramters related to periodic reports */
#ifndef MAC_AUT_TEST    
    initReInitPeriodicReportManager(internalCellIndex);
#endif
    initULSchedulerParams(internalCellIndex);
    initDLSchedulerParams(internalCellIndex);
}

/************************************************************************
 * Function Name    : cleanupMacRRMInterface 
 * Inputs           : internalCellIndex
 * Returns          : none
 * Description      : This function is used to cleanup 
 *                    circular queue which were created for  reconfig UE for both UL and DL
 *                    This is called at Cell stop
 ************************************************************************/
void cleanupMacRRMInterface(InternalCellIndex internalCellIndex)
{
    UInt32 count=0,loopVar=0;
    UELocationNode *ueLocationNodeUL_p = PNULL; 
    UELocationNode *ueLocationNodeDL_p = PNULL; 
    UEUserTypeNode   *ueUserTypeNodeDL_p = PNULL;  /* EICIC -*/
    UEULMUMIMOInfoNode *ulUEMUMIMONode_p = PNULL;
    UENumOfLayerNode  *ueNumOfLayerNode_p = PNULL;
    UEBeamformingAlgoNode  *ueBeamformingAlgoNode_p = PNULL;
    UEScellSchedulingInfoNode  *ueScellSchedulingInfoNode_p = PNULL;
    UECqiValidityTimerInfoNode  *ueCqiValidityTimerInfoNode_p = PNULL;
    /* deallocating circular Queue ulUEReconfigDataQueue_g*/
    count = COUNT_UL_UE_RECONFIG_DATA_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_UL_UE_RECONFIG_DATA_Q(UELocationNode, 
                (void *)&ueLocationNodeUL_p, internalCellIndex);

        if (ueLocationNodeUL_p)
        {
            FREE_MEM_NODE_UL_UE_RECONFIG_DATA_Q(ueLocationNodeUL_p);
        }
    }

    /* deallocating circular Queue dlUEReconfigDataQueue_g*/
    count = COUNT_DL_UE_RECONFIG_DATA_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_DL_UE_RECONFIG_DATA_Q(UELocationNode, 
                (void *)&ueLocationNodeDL_p, internalCellIndex);

        if (ueLocationNodeDL_p)
        {
            FREE_MEM_NODE_DL_UE_RECONFIG_DATA_Q(ueLocationNodeDL_p);
        }
    }

    /* EICIC +-*/

    /* deallocating circular Queue dlUEReconfigDataQueue_g*/
    count = COUNT_UE_RECONFIG_USER_TYPE_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_UE_RECONFIG_USER_TYPE_Q(UEUserTypeNode, 
                (void *)&ueUserTypeNodeDL_p, internalCellIndex);

        if (ueUserTypeNodeDL_p)
        {
            FREE_MEM_NODE_UE_RECONFIG_USER_TYPE_Q(ueUserTypeNodeDL_p);
        }
    }

    /* EICIC -*/

    /* deallocating circular Queue ulUEReconfigMUMIMOQueue_g */
    count = COUNT_UL_UE_MUMIMO_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_UL_UE_MUMIMO_Q(UEULMUMIMOInfoNode, 
                (void *)&ulUEMUMIMONode_p, internalCellIndex);

        if (ulUEMUMIMONode_p)
        {
            FREE_MEM_NODE_UL_UE_MUMIMO_Q(ulUEMUMIMONode_p);
        }
    }
    /* deallocating circular Queue ueReconfigNumOfLayerQueue_g */
    count = COUNT_UE_NUM_OF_LAYER_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_UE_NUM_OF_LAYER_Q(UENumOfLayerNode, 
                (void *)&ueNumOfLayerNode_p, internalCellIndex);

        if (ueNumOfLayerNode_p)
        {
            FREE_MEM_NODE_UE_NUM_OF_LAYER_Q(ueNumOfLayerNode_p);
        }
    }
    /* deallocating circular Queue ueReconfigBeamFormingAlgoQueue_g */
    count = COUNT_UE_BEAM_FORMING_ALGO_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_UE_BEAM_FORMING_ALGO_Q(UEBeamformingAlgoNode, 
                (void *)&ueBeamformingAlgoNode_p, internalCellIndex);

        if (ueBeamformingAlgoNode_p)
        {
            FREE_MEM_NODE_UE_BEAM_FORMING_ALGO_Q(ueBeamformingAlgoNode_p);
        }
    }
    /* ca changes start */
    /* deallocating circular Queue ueScellSchedulingInfoNode_p */
    count = COUNT_UE_RECONFIG_SCELL_SCHEDULING_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_UE_RECONFIG_SCELL_SCHEDULING_Q(UEScellSchedulingInfo, 
                (void *)&ueScellSchedulingInfoNode_p, 
                internalCellIndex);

        if (ueScellSchedulingInfoNode_p)
        {
            FREE_MEM_NODE_UE_RECONFIG_SCELL_SCHEDULING_Q(
                    ueScellSchedulingInfoNode_p);
        }
    }
    /* deallocating circular Queue ueCqiValidityTimerInfoNode_p */
    count = COUNT_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(UECqiValidityTimerInfo, 
                (void *)&ueCqiValidityTimerInfoNode_p, 
                internalCellIndex);

        if (ueCqiValidityTimerInfoNode_p)
        {
            FREE_MEM_NODE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(
                    ueCqiValidityTimerInfoNode_p);
        }
    }
}

/************************************************************************
 * Function Name    : deInitMacRRMInterface 
 * Inputs           : internalCellIndex 
 * Returns          : none
 * Description      : This function is used to DeInit  
 *                    circular queue which were created for  reconfig UE for both UL and DL
 *                    This is called at MAC_CLEANUP_LAYER_REQ
 ************************************************************************/
void deInitMacRRMInterface(InternalCellIndex  internalCellIndex)
{
    circQDeInit(&rrmMacCellGlobal_gp[internalCellIndex]->ulUEReconfigDataQueue);
    circQDeInit(&rrmMacCellGlobal_gp[internalCellIndex]->dlUEReconfigDataQueue);
    circQDeInit(&rrmMacCellGlobal_gp[internalCellIndex]->ueReconfigUserTypeQueue); /*EICIC +-*/
    circQDeInit(&rrmMacCellGlobal_gp[internalCellIndex]->ulUEReconfigMUMIMOQueue);
    circQDeInit(&rrmMacCellGlobal_gp[internalCellIndex]->ueReconfigNumOfLayerQueue);
    circQDeInit(&rrmMacCellGlobal_gp[internalCellIndex]->ueReconfigBeamFormingAlgoQueue);

    QDEINIT_NODE_UE_RECONFIG_SCELL_SCHEDULING_Q(internalCellIndex);
    QDEINIT_NODE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(internalCellIndex);

    freeMemPool(rrmMacCellGlobal_gp[internalCellIndex]);
    rrmMacCellGlobal_gp[internalCellIndex] = PNULL;

    freeMemPool(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForPeriodicReport_p);
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForPeriodicReport_p = PNULL;
    freeMemPool(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForICICReport_p);
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForICICReport_p = PNULL;
    freeMemPool(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForLayer1Report_p);
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForLayer1Report_p = PNULL;
    /*EICIC */
    freeMemPool(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForEICICReport_p);
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->bufferForEICICReport_p = PNULL;
    /*EICIC*/

    freeMemPool(rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]);
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex] = PNULL;

    freeMemPool(macReconfigSchedulerParamsDL_gp[internalCellIndex]);
    macReconfigSchedulerParamsDL_gp[internalCellIndex] = PNULL;
	
    freeMemPool(macReconfigSchedulerParamsUL_gp[internalCellIndex]);
    macReconfigSchedulerParamsUL_gp[internalCellIndex] = PNULL;

    /* CA changes end*/
}

/************************************************************************
 * Function Name    : processULRrmReconfiguration 
 * Inputs           : internalCellIndex
 * Returns          : none
 * Description      : This function is used to update the bitmask flag 
 *                    based on the value either index or structure is 
 *                    updated for Uplink side.
 ************************************************************************/
void processULRrmReconfiguration(InternalCellIndex  internalCellIndex)
{  
    UInt32 queueCount = 0;
    /*ca changes start*/
    CellConfigParams *cellParams_p = NULL;
    cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p \
                   [internalCellIndex]->cellParams_p;
    /* ca changes end */

    if(rrmMacCellGlobal_gp[internalCellIndex]->ulResourceReconfigFlag)
    {
        /*CA Changes start  */
        ulRAInfoIndex_g[internalCellIndex] = !ulRAInfoIndex_g[internalCellIndex];
        /*CA Changes end  */
        rrmMacCellGlobal_gp[internalCellIndex]->ulResourceReconfigFlag = FALSE;
    }

    if(rrmMacCellGlobal_gp[internalCellIndex]->ulSchedulerReconfigFlag)
    {
        /*CA Changes start  */
        schParamsIndexUL_g[internalCellIndex] = 
            !schParamsIndexUL_g[internalCellIndex];
        /*CA Changes end  */
        rrmMacCellGlobal_gp[internalCellIndex]->ulSchedulerReconfigFlag = FALSE;
    }

    /* processing UE Location Reconfiguration */
    queueCount = COUNT_UL_UE_RECONFIG_DATA_Q(internalCellIndex);
    if(queueCount)
    {
        /* function call for updating UE context for UL*/
        processULUELocationTypeQueue(queueCount, internalCellIndex);
    }

    /*UL_MU_MIMO_CHG Start*/
    queueCount = COUNT_UL_UE_MUMIMO_Q(internalCellIndex);
    if(queueCount)
    {
        processULUeMUMIMOQueue(queueCount, internalCellIndex);
    }
    /*UL_MU_MIMO_CHG End*/
    /*BUG:8042:START*/
    if(cellParams_p->commonULPowerControlInfo.alpha <= 3)
    {
        macPathlossToTargetSINRMapForPUSCH_p[internalCellIndex] =
            macReconfigSchedulerParamsUL_gp[internalCellIndex] \
            [schParamsIndexUL_g[internalCellIndex]].macPathlossToTargetSINRMapForPUSCH[0];
    }
    else if (cellParams_p->commonULPowerControlInfo.alpha <= 10)
    {
        macPathlossToTargetSINRMapForPUSCH_p[internalCellIndex] =
                                                                  macReconfigSchedulerParamsUL_gp[internalCellIndex] \
                                                                  [schParamsIndexUL_g[internalCellIndex]].macPathlossToTargetSINRMapForPUSCH \
                                                                  [cellParams_p->commonULPowerControlInfo.alpha - 3];
    }
    /*BUG:8042:END*/
}


/************************************************************************
 * Function Name    : processDLRrmReconfiguration 
 * Inputs           : internalCellIndex 
 * Returns          : none
 * Description      : This function is used to update the bitmask flag 
 *                    based on the value either index or structure is 
 *                    updated for Downlink side.                    
 ************************************************************************/
void processDLRrmReconfiguration(InternalCellIndex  internalCellIndex)
{
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    UInt32 queueCount = 0;

    if(rrmMacCellGlobal_gp[internalCellIndex]->dlResourceReconfigFlag)
    {
        dlRAInfoIndex_g[internalCellIndex] = !dlRAInfoIndex_g[internalCellIndex];
        /* + PRS_CHANGES */
        cellParams_p->prsConfigInfo.dlPRSRATPoliciesIndex = !(cellParams_p->prsConfigInfo.dlPRSRATPoliciesIndex);
        /* - PRS_CHANGES */
        rrmMacCellGlobal_gp[internalCellIndex]->dlResourceReconfigFlag = FALSE;
    }
    if(rrmMacCellGlobal_gp[internalCellIndex]->dlSchedulerReconfigFlag)
    {
        schParamsIndexDL_g[internalCellIndex] = !schParamsIndexDL_g[internalCellIndex];
        rrmMacCellGlobal_gp[internalCellIndex]->dlSchedulerReconfigFlag = FALSE;
    }

    /* processing UE Location Reconfiguration */
    queueCount = COUNT_DL_UE_RECONFIG_DATA_Q(internalCellIndex);
    if(queueCount)
    {
        /* function call for updating UE context for DL */
        processDLUELocationTypeQueue(queueCount, internalCellIndex);
    }

    /* + TM7_8 Changes Start*/ 
    queueCount = circQueueCount(&rrmMacCellGlobal_gp[internalCellIndex]->ueReconfigNumOfLayerQueue);
    if(queueCount)
    {
        processUeNumOfLayerQueue(queueCount, internalCellIndex);
    }

    queueCount = circQueueCount(&rrmMacCellGlobal_gp[internalCellIndex]->ueReconfigBeamFormingAlgoQueue);
    if(queueCount)
    {
        processUeBeamFormingAlgoQueue(queueCount, internalCellIndex);
    }
    /* - TM7_8 Changes End*/ 
    /*+ Dyn UEs To Be SchPerTTI:CR +*/
    validateAndUpdateMaxUeToBeSchdDlUl(macReconfigSchedulerParamsDL_gp[internalCellIndex] \
            [schParamsIndexDL_g[internalCellIndex]].
            maxUEsToBeScheduledDlUl.maxUEsToBeScheduledDL,
            macReconfigSchedulerParamsDL_gp[internalCellIndex] \
            [schParamsIndexDL_g[internalCellIndex]].
            maxUEsToBeScheduledDlUl.maxUEsToBeScheduledUL,
            internalCellIndex);
    /*- Dyn UEs To Be SchPerTTI:CR -*/
    /* ca changes start*/
    queueCount = 
        COUNT_UE_RECONFIG_SCELL_SCHEDULING_Q(internalCellIndex);
    if(queueCount)
    {
        macProcessUeReconfigScellSchedulingQueue(queueCount, internalCellIndex);
    }
    queueCount =
        COUNT_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(internalCellIndex);
    if(queueCount)
    {
        macProcessUeReconfigCqiValidityTimerQueue(queueCount, internalCellIndex);
    } 
    /* CA CHANGES END*/
}

/* EICIC +*/

/************************************************************************
 * Function Name    : processDLRrmReconfiguration 
 * Inputs           : internalCellIndex 
 * Returns          : none
 * Description      : This function is used to update the bitmask flag 
 *                    based on the value either index or structure is 
 *                    updated for Downlink side.                    
 ************************************************************************/
void processEicicConfiguration (InternalCellIndex  internalCellIndex)
{
    UInt32 queueCount = 0;

    if(rrmMacCellGlobal_gp[internalCellIndex]->configureAbsPatternFlag)
    {
        processEicicConfigurationforAggressor(internalCellIndex);
    }

    if(rrmMacCellGlobal_gp[internalCellIndex]->configureUsableAbsPatternFlag)
    {
        processEicicConfigurationforVictim(internalCellIndex);
    }

    /* processing UE Location Reconfiguration */
    queueCount = COUNT_UE_RECONFIG_USER_TYPE_Q(internalCellIndex);   

    if(queueCount)
    {
        /* function call for updating UE context for DL/UL */
        processUEUserTypeQueue(queueCount, internalCellIndex);
    }
}

/* EICIC -*/

/*CA changes start*/

/************************************************************************
 * Function Name    : macValidateScellSchedulingActDeact 
 * Inputs           : ueScellSchedulingInfo_p, ueCarrierAggConfigError_p,
 *                    rrmUeIndex,  internalCellIndex
 * Returns          : TRUE/FALSE
 * Description      : This function validates secondary cell scheduling 
 *                      activation and deactivation.
 ************************************************************************/
UInt8 macValidateScellSchedulingActDeact(
                        UEScellSchedulingInfo *ueScellSchedulingInfo_p,
                        UECarrierAggConfigError *ueCarrierAggConfigError_p,
                        UInt32 rrmUeIndex,
                        InternalCellIndex internalCellIndex)
{
    UInt8 activcId = 0;
    /* SPR 12706 fix start*/
    DLUEContextInfo *ueDLContextInfo_p = PNULL;
    DLUEContext     *ueDLContext_p     = PNULL;
    /* SPR 12706 fix end*/

    for(activcId = 0;
        activcId < ueScellSchedulingInfo_p->numOfScellsSchedulingAllowed; 
        activcId++)
    {
        if( ((ueScellSchedulingInfo_p->schedulingActivateScell[activcId])
                    < START_SCELL_INDEX) || 
            ((ueScellSchedulingInfo_p->schedulingActivateScell[activcId]) 
                    > MAX_SCELL_INDEX))
        {
            ueCarrierAggConfigError_p->ueIndex = rrmUeIndex;
            ueCarrierAggConfigError_p->errorCode = 
                SCELL_CONFIGURATION_INVALID;
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueCarrierAggConfigError_p->ueIndex,
                    ueCarrierAggConfigError_p->errorCode,
                    DEFAULT_INT_VALUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            return FALSE;
        }

        /*SPR 12706 fix start, reestb scenario, return failure in case activation is 
        received for already activated Scell*/ 
        /*Also removed the similar checking from parsing of 
        mac_reconfigure_ue API*/ 
        if((ueScellSchedulingInfo_p->schedulingActivateScell[activcId]))
        {
            ueDLContextInfo_p = 
                              &dlUECtxInfoArr_g[ueScellSchedulingInfo_p->ueIndex];
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
			if(ueDLContext_p && ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX])
			{
            if(!(CELL_PHY_RUNNING == 
                cellSpecificParams_g.cellConfigAndInitParams_p
            [ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex]->cellState))
            {
                    ueCarrierAggConfigError_p->ueIndex = rrmUeIndex;
                    ueCarrierAggConfigError_p->errorCode = 
                        SCELL_CONFIGURATION_INVALID;
                    LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                            ueCarrierAggConfigError_p->ueIndex,
                            ueCarrierAggConfigError_p->errorCode,
                            DEFAULT_INT_VALUE,
                            LINE_NUMBER,DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"");
                    return FALSE;
            }
        }
			else
			{
                /*SPR 18667 start*/
				 ueCarrierAggConfigError_p->ueIndex = rrmUeIndex;
                ueCarrierAggConfigError_p->errorCode = SCELL_CONFIGURATION_INVALID;
                /*SPR 18667 end*/
				LOG_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
						GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
						ueCarrierAggConfigError_p->ueIndex,
						ueCarrierAggConfigError_p->errorCode,
						DEFAULT_INT_VALUE,
						LINE_NUMBER,DEFAULT_INT_VALUE,
						DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
						FUNCTION_NAME,"");
				return FALSE;
			}
		}
    }
   /*SPR 12706 fix end*/

    for(activcId = 0; 
        activcId < ueScellSchedulingInfo_p->numOfScellsSchedulingNotAllowed;
        activcId++)
    {
        if( ((ueScellSchedulingInfo_p->schedulingDeactivateScell[activcId])
                    < START_SCELL_INDEX) || 
            ((ueScellSchedulingInfo_p->schedulingDeactivateScell[activcId]) 
                    > MAX_SCELL_INDEX))
        {
            ueCarrierAggConfigError_p->ueIndex = rrmUeIndex;
            ueCarrierAggConfigError_p->errorCode = 
                SCELL_CONFIGURATION_INVALID;
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    ueCarrierAggConfigError_p->ueIndex,
                    ueCarrierAggConfigError_p->errorCode,
                    DEFAULT_INT_VALUE,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
            return FALSE;
        }
    }
    return TRUE;
}
/*CA changes end*/
/* Perf stats RRM-MAC start */
#ifdef PERF_STATS
/************************************************************************
 * Function Name    : processRrmMacConfigPerfStatsReq
 * Inputs           : rrmMacConfigPerfStatsReq_p - pointer to structure
 *                    transactionId  
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function configures the perf stats request of RRM.
 ************************************************************************/
UInt8 processRrmMacConfigPerfStatsReq(
                          RrmMacConfigPerfStatsReq *rrmMacConfigPerfStatsReq_p,
                          UInt16 transactionId,
                          InternalCellIndex internalCellIndex)
{
    /* + PERF_CA */
    InternalCellIndex cellIndex = 0;
    /* + PERF_CA */
    UInt32 loModuleSrc = PERF_STATS_CONTAINER_RRM_MAC;
    UInt8 passiveIndexForMacStats = gPerfStats.perfStatsControlInfo->macActiveIndex;
    /* SPR 15909 fix start */
    tickType_t macLastCalculatedTTI = 0;
    /* SPR 15909 fix end */

    LtePerfStatsConfig  *pRRMMacPerfStatsConfig = &(g_perfStatsScannerObj.\
            perfStatsContainer[loModuleSrc].perfConfig.lteMacPerfStatsConfig);

    if(!(g_perfStatsScannerObj.module_bitmask))
    {
        g_perfStatsScannerObj.prevPerfStats_tick = GET_CURRENT_TICK_FOR_CELL(internalCellIndex);

    }

    if ( (rrmMacConfigPerfStatsReq_p->duration > MAC_MAX_VAL_PERF_TIMER_T ) ||
            (rrmMacConfigPerfStatsReq_p->duration < MAC_MIN_VAL_PERF_TIMER_T ))
    {
        return MAC_FAILURE;
    }
    else
    {
        if ((rrmMacConfigPerfStatsReq_p->uePerfStatsToReportBitMap)
                || (rrmMacConfigPerfStatsReq_p->cellPerfStatsToReportBitMap ) )
        {

            g_perfStatsScannerObj.module_bitmask |= PERF_STATS_CONTAINER_RRM_MAC_BITMASK;
            pRRMMacPerfStatsConfig->duration = (rrmMacConfigPerfStatsReq_p->duration) * 1000;
            g_perfStatsScannerObj.timeoutTTI[loModuleSrc] = pRRMMacPerfStatsConfig->duration;


            pRRMMacPerfStatsConfig->periodicReporting = rrmMacConfigPerfStatsReq_p->periodicReporting;
            pRRMMacPerfStatsConfig->uePerfStatsToReportBitMap = rrmMacConfigPerfStatsReq_p->uePerfStatsToReportBitMap;
            pRRMMacPerfStatsConfig->cellPerfStatsToReportBitMap = rrmMacConfigPerfStatsReq_p->cellPerfStatsToReportBitMap;
            pRRMMacPerfStatsConfig->transactionId = transactionId;

            /* Perf stats RRM-MAC start */
            g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_RRM_MAC].macOrPdcpOrRlcStats = MAC_PERF_STATS;

            macLastCalculatedTTI = gPerfStats.perfStats[passiveIndexForMacStats].macLastCalculatedTTI;
            if (macLastCalculatedTTI)
            {
                /* Switching index */
                gPerfStats.perfStatsControlInfo->macActiveIndex =  passiveIndexForMacStats ^0x01;

                /* Switching counter position from passive to active */
                /* + PERF_CA */
                for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
                {
                    gMacCellPerfStats_p[cellIndex] =
                        &gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->macActiveIndex].lteCellPerfStats[cellIndex];
                }
                /* - PERF_CA */
                gMacUePerfStats_p =
                    gPerfStats.perfStats[gPerfStats.perfStatsControlInfo->macActiveIndex].lteUePerfStats;

            }

            /*Update OAM-MAC container */
            if(g_perfStatsScannerObj.module_bitmask & PERF_STATS_CONTAINER_OAM_MAC_BITMASK)
            {
                if(MAC_PERF_STATS == g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].macOrPdcpOrRlcStats)
                {
                    updateMacPerfContainer(PERF_STATS_CONTAINER_OAM_MAC);
                }
            }
            /* Perf stats RRM-MAC end */

            /*reset shm stats */
            /* + PERF_CA */
            for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
            {
                macResetCellPerfStatCounters(&gPerfStats.perfStats[passiveIndexForMacStats].lteCellPerfStats[cellIndex]);
            }
            /* - PERF_CA */
            macResetUePerfStatCounters(gPerfStats.perfStats[passiveIndexForMacStats].lteUePerfStats);
            gPerfStats.perfStats[passiveIndexForMacStats].macLastCalculatedTTI = 0;

            /*reset container stats */
            resetMacPerfContainer(PERF_STATS_CONTAINER_RRM_MAC_RESET_UE_CONTAINER);
            resetMacPerfContainer(PERF_STATS_CONTAINER_RRM_MAC_RESET_CELL_CONTAINER);
            g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_RRM_MAC].
                perfStats.lteMacPerfStats.lastMACCalculatedTTI = 0;
        }
        else
        {
            g_perfStatsScannerObj.module_bitmask &= ~(PERF_STATS_CONTAINER_RRM_MAC_BITMASK);
        }

    }
    return MAC_SUCCESS;
}
#endif
/* Perf stats RRM-MAC end */

/************************************************************************
 * Function Name    : checkCellBitMask
 * Inputs           : rrmUeIndex
 *                    cellBitMask
 *                    ueLocationInfo_p
 *                    dlUEContext_p
 *                    scellLocationUpdateFlag_p
 *                    index
 *                    ueLocationError_p
 *                    numOfUeWithError_p
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function parses cell bit mask.
 ************************************************************************/
void checkCellBitMask(UInt32 rrmUeIndex,
        UInt8 cellBitMask,
        UELocationInfo *ueLocationInfo_p,
        DLUEContext *dlUEContext_p,
        UInt8 *scellLocationUpdateFlag_p,
        UInt32 index,
        UELocationError *ueLocationError_p,
        UInt32 *numOfUeWithError_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 loopIndex = 0;
    UInt8 locationType = 0;
    InternalCellIndex internalCellIndexOfScell = 0;

    for(loopIndex = START_SCELL_INDEX; 
            loopIndex <= MAX_NUM_SCELL; loopIndex++)
    {
        /* Check the bit of CellBitmask to get the cell index for 
         * which location rconfig info is to be applied bit 0 is 
         * not used as Scell index valid from 1 to 7 */
        if(cellBitMask & (1 << loopIndex))
        {
            locationType = 
                ueLocationInfo_p->ueLocation[index].ueLocationPerScell[loopIndex - 1];

            DLUESCellContext *dlScellContext_p = 
                (dlUEContext_p->dlUeScellContext_p[loopIndex]);
            if(NULL == dlScellContext_p)
            {
                ueLocationError_p->ueIndex = rrmUeIndex;
                ueLocationError_p->errorCode = 
                    INVALID_USER_LOCATION_TYPE_ERROR;
                (*numOfUeWithError_p)++;

                /* +- SPR 17777 */
                LOG_MAC_MSG(MAC_RRM_INVALID_USER_LOCATION_TYPE_ERROR,
                        LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        locationType,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
                *scellLocationUpdateFlag_p = FALSE; 
                continue;
            }
            internalCellIndexOfScell  =  
                dlScellContext_p->internalCellIndex;
            if(INVALID_USER_LOCATION_TYPE <= locationType)
            {
                ueLocationError_p->ueIndex = rrmUeIndex;
                ueLocationError_p->errorCode = 
                    INVALID_USER_LOCATION_TYPE_ERROR;
                (*numOfUeWithError_p)++;

                LOG_UT(MAC_RRM_INVALID_USER_LOCATION_TYPE_ERROR,
                        LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        locationType,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");
                *scellLocationUpdateFlag_p = FALSE; 
                continue;
            }
            else if(locationType == CE_USER && 
                    (dlResourcePartitionConfig_g[internalCellIndexOfScell].cellEdgeRegionCount 
                     == 0))
            {
                ueLocationError_p->ueIndex = rrmUeIndex;
                ueLocationError_p->errorCode = 
                    NO_DL_CELL_EDGE_REGION_EXIST;
                (*numOfUeWithError_p)++;
                LOG_UT(MAC_RRM_NO_DL_CELL_EDGE_REGION_EXIST,
                        LOGDEBUG,MAC_RRM_INF,
                        GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                        ueLocationError_p->errorCode,
                        LINE_NUMBER,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"");

                *scellLocationUpdateFlag_p = FALSE; 
                continue;
            }
            *scellLocationUpdateFlag_p = TRUE;
        }
    }

    return;
}

/************************************************************************
 * Function Name    : processEicicConfigurationforAggressor 
 * Inputs           : internalCellIndex  - Cell-Index at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtion process the Eicic Configuration for Aggressor.
 ************************************************************************/
void processEicicConfigurationforAggressor(InternalCellIndex internalCellIndex)
{
    UInt8 absCount = 0;
    UDouble32 absFactor = 0;
    ULSchedulerStrategyConfig *ulSchdConfig_p = & cellSpecificParams_g.
        cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig;
    DLSchedulerStrategyConfig   *dlSchdConfig_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
            initParams_p->dlSchdConfig);

    if (LTE_TRUE == applyAbsConfiguration_g[internalCellIndex])
    {
        absPatternIndex_g[internalCellIndex] = !absPatternIndex_g[internalCellIndex];
        absCount = calculateAbsBits(internalCellIndex,ABS_PATTERN_TYPE); 

        if (absCountNew_g[internalCellIndex] != absCount)
        {
            absCountOld_g[internalCellIndex] = absCountNew_g[internalCellIndex];
            absCountNew_g[internalCellIndex] = absCount;

            if ((absCountNew_g[internalCellIndex] > 0 )&& (absCountOld_g[internalCellIndex] > 0))
            {
                absFactor = (UDouble32 )(NUM_BITS_ABS_PATTERN - absCountOld_g[internalCellIndex]) / (UDouble32)(NUM_BITS_ABS_PATTERN - absCountNew_g[internalCellIndex]);
                if (DL_QOS_BASED_SCHEDULING  == dlSchdConfig_p->dlSchedulerStrategy)
                {
                    dlAggregateGbrPerTTI_g[internalCellIndex][NON_VICTIM_UE] *= absFactor;
                    totalDlAggregateGbrPerTTI_g[NON_VICTIM_UE]  *= absFactor;

                }
                if (UL_QOS_SCHEDULING == ulSchdConfig_p->ulSchedulerStrategy)
                {
                    ulAggregateGbrPerTTI_g[internalCellIndex][NON_VICTIM_UE] *= absFactor;
                }
            }
        }

        if ((isUsablePatternSet_g[internalCellIndex])  || (isConfigureAbsPatternSet_g[internalCellIndex]))
        { 
            if (LTE_FALSE == isEicicEnabled_g[internalCellIndex])
            {
                isEicicEnabled_g[internalCellIndex] = LTE_TRUE;
            }
        }	
        else
        {
            if (isEicicEnabled_g[internalCellIndex])
            {
                isEicicEnabled_g[internalCellIndex] = LTE_FALSE;
            }
        }
        rrmMacCellGlobal_gp[internalCellIndex]->configureAbsPatternFlag = FALSE;
    }
    return;
}


/************************************************************************
 * Function Name    : processEicicConfigurationforVictim 
 * Inputs           : internalCellIndex  - Cell-Index at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This funtion process the Eicic Configuration for Aggressor.
 ************************************************************************/
void processEicicConfigurationforVictim(InternalCellIndex internalCellIndex)
{
    UInt8 usableAbsCount = 0;
    ULSchedulerStrategyConfig *ulSchdConfig_p = & cellSpecificParams_g.
        cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig;
    DLSchedulerStrategyConfig   *dlSchdConfig_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
            initParams_p->dlSchdConfig);
    UDouble32 absFactorNonVictim = 0;
    UDouble32 absFactorVictim = 0;

    if (LTE_TRUE == applyAbsConfiguration_g[internalCellIndex])
    {
        usableAbsPatternIndex_g[internalCellIndex] = !usableAbsPatternIndex_g[internalCellIndex];
        usableAbsCount = calculateAbsBits(internalCellIndex,USABLE_ABS_PATTERN_TYPE); 

        if (usableAbsCountNew_g[internalCellIndex] != usableAbsCount)
        {
            usableAbsCountOld_g[internalCellIndex] = usableAbsCountNew_g[internalCellIndex];
            usableAbsCountNew_g[internalCellIndex] = usableAbsCount;

            if ((usableAbsCountNew_g[internalCellIndex] > 0 )&& (usableAbsCountOld_g[internalCellIndex] > 0))
            {
                absFactorNonVictim = (UDouble32) (NUM_BITS_ABS_PATTERN - usableAbsCountOld_g[internalCellIndex]) / (UDouble32) (NUM_BITS_ABS_PATTERN - usableAbsCountNew_g[internalCellIndex]);
                absFactorVictim = (UDouble32) (usableAbsCountOld_g[internalCellIndex] / (UDouble32) usableAbsCountNew_g[internalCellIndex]);

                if (DL_QOS_BASED_SCHEDULING  == dlSchdConfig_p->dlSchedulerStrategy)
                {
                    dlAggregateGbrPerTTI_g[internalCellIndex][NON_VICTIM_UE] *= absFactorNonVictim;
                    dlAggregateGbrPerTTI_g[internalCellIndex][VICTIM_UE]  *= absFactorVictim; 
                    totalDlAggregateGbrPerTTI_g[NON_VICTIM_UE]  *= absFactorNonVictim;
                    totalDlAggregateGbrPerTTI_g[VICTIM_UE]  *= absFactorVictim;

                }
                if (UL_QOS_SCHEDULING == ulSchdConfig_p->ulSchedulerStrategy)
                {
                    ulAggregateGbrPerTTI_g[internalCellIndex][NON_VICTIM_UE] *= absFactorNonVictim;
                    ulAggregateGbrPerTTI_g[internalCellIndex][VICTIM_UE]  *= absFactorVictim; 
                }
            }
        }
        if ((isUsablePatternSet_g[internalCellIndex])  || (isConfigureAbsPatternSet_g[internalCellIndex]))
        { 
            if (LTE_FALSE == isEicicEnabled_g[internalCellIndex])
            {
                isEicicEnabled_g[internalCellIndex] = LTE_TRUE;
            }
        }
        else
        {
            if (isEicicEnabled_g[internalCellIndex])
            {
                isEicicEnabled_g[internalCellIndex] = LTE_FALSE;
            }
        }
        rrmMacCellGlobal_gp[internalCellIndex]->configureUsableAbsPatternFlag = FALSE;
    }

}
/****************************************************************************
 * Function Name  : initNbMacRRMInterface 
 * Inputs         : Number of cells configured
 * Outputs        : Initialize the Queue's & List
 * Returns        : None 
 * Description    : This function is used to initialize the Queue's and List
 ****************************************************************************/
