/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacPuschPowerControl.c
 *
 ******************************************************************************
 *
 *  File Description : This file contains implementation of Power Control 
 *                     Feature in LTE_MAC.
 *                     
 ******************************************************************************
 *
 * Revision Details
 * Revision 1.1.1 2012/07/20 06:06:27  gur31292
 * Changes related to DCI Format3A
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacULPowerControlData.h"
#include "lteMacPuschPowerControl.h"
#include "lteMacULResourceAllocator.h" 
#include "lteMacUtilityAndSharedStructs.h"
#include "lteMacULCSIManager.h"
#ifdef FDD_CONFIG
#include "lteMacULSpsScheduler.h"
#endif
#ifdef TDD_CONFIG
#include    "lteMacTDDConfig.h"
#endif
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
/* CLPC_CHG */
/*CA Changes start  */
extern EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE* explicitULPuschTpcScheduleQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */
extern UInt16 pucchUciUlschMap_g[MAX_UE_SUPPORTED];
/* CA changes Start */
extern UInt8  explicitTpcTriggerEnabled_g[MAX_NUM_CELL];
/* CA changes End */

/*HD FDD Changes Start*/
extern TriggeredUEListNode*  addEntryInFailTriggeredUeList(TriggeredUEListNode* node_p,
        LTE_LIST * failTriggeredUeList,
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */);
/*HD FDD Changes End*/
/*CYCLOMATIC_CHANGES_6.1_START*/
STATIC  void calculatePreferredMcsRBForATB(UInt8 mcsloopCount,
                                                 UInt8 rbloopCount,
                                                 UInt8 dbloopCount,
                                                 SInt8 deltaTfVal,
                                                 UInt8 tenLogMAllocatedVal);
/*CYCLOMATIC_CHANGES_6.1_END*/
/*START_DYN_PDCCH_TDD_CHANGE*/
/*END_DYN_PDCCH_TDD_CHANGE*/
/*START : DYN_PDCCH*/
extern UInt8 currentCFIValue_g[MAX_NUM_CELL];
/*END   : DYN_PDCCH*/

/* ICIC changes start */

UInt8 *macPathlossToTargetSINRMapForPUSCH_p[MAX_NUM_CELL] = {PNULL};
/* ICIC changes end */
UInt8 *macPathlossToTargetSINRMapForSpsPUSCH_p[MAX_NUM_CELL] = {PNULL};
/* Pointer to the global array of TPC-PUSCH-RNTIs configured in the system*/
TpcRntiContext * tpcPuschRntiContext_gp[MAX_NUM_CELL] = {PNULL};
/* +CLPC_CHG_LJA */
/*tenLogMFactor_g is a lookup table where index represents M = number of RBs -1
 * and the value and the array index represents 10LogM(Base 10) rounded to the
 * nearest integer.
 * */
UInt8 tenLogMFactor_g[MAX_NUM_RBS] = 
{
    0, 3, 5, 6, 7, 8, 8, 9,10,10,  /* 0- 9*/
    10,11,11,11,12,12,12,13,13,13, /*10-19*/
    13,13,14,14,14,14,14,14,15,15, /*20-29*/
    15,15,15,15,15,16,16,16,16,16, /*30-39*/
    16,16,16,16,17,17,17,17,17,17, /*40-49*/
    17,17,17,17,17,17,18,18,18,18, /*50-59*/
    18,18,18,18,18,18,18,18,18,18, /*60-69*/
    19,19,19,19,19,19,19,19,19,19, /*70-79*/
    19,19,19,19,19,19,19,19,19,20, /*80-89*/
    20,20,20,20,20,20,20,20,20,20  /*90-99*/
};

/*SPR 22383 changes start*/
SInt8 mcsToDeltaTF_g[MAX_MCS_INDEX] = 
{
    -7,/*0 */
    -6,/*1 */
    -5,/*2 */
    -3,/*3 */
    -2,/*4 */
    -1,/*5 */
    0,/*6 */
    0,/*7 */
    1,/*8 */
    2,/*9 */
    2,/*10*/
    2,/*11*/
    3,/*12*/
    4,/*13*/
    5,/*14*/
    6,/*15*/
    7,/*16*/
    7,/*17*/
    7,/*18*/
    8,/*19*/
    9,/*20*/
    10,/*21*/
    11,/*22*/
    12,/*23*/
    13,/*24*/
    14,/*25*/
    15,/*26*/
    16,/*27*/
    19 /*28*/
};
/* -CLPC_CHG_LJA */


/* This table represents the deltaTf to MCS table used in MCS validation for ATB
 * It is an inverse of the mcs to delta tf table with each index corresponding to 
 * a delta TF value (range -7 to 19 ) as in the above table.
 * For a value of deltaTF which is not present in the above table lower MCS is selected 
 * and for same delta TF value having different MCS,higher MCS value is selected.
 */
UInt8 deltaTfToMCS_g[MAX_DELTA_TF_NORMALIZED_VAL] = 
{
    0, /* -7 */
    1, /* -6 */
    2, /* -5 */
    2, /* -4 */
    3, /* -3 */
    4, /* -2 */
    5, /* -1 */
    7, /*  0 */
    8, /*  1 */
    11,/*  2 */
    12,/*  3 */
    13,/*  4 */
    14,/*  5 */
    15,/*  6 */
    18,/*  7 */
    19,/*  8 */
    20,/*  9 */
    21,/* 10 */
    22,/* 11 */
    23,/* 12 */
    24,/* 13 */
    25,/* 14 */
    26,/* 15 */
    27,/* 16 */
    28,/* 17 */
    28,/* 18 */
    28,/* 19 */
};
/*SPR 22383 changes end*/


UInt8 tenLogMFactorToPrb_g[MAX_TEN_LOG_M_FACTOR_TBL_VAL] = 
{
    1, /* 0 */
    1, /* 1 */
    1, /* 2 */
    2, /* 3 */
    2, /* 4 */
    3, /* 5 */
    4, /* 6 */
    5, /* 7 */
    7, /* 8 */
    8, /* 9 */
    11,/* 10 */
    14,/* 11 */
    17,/* 12 */
    22,/* 13 */
    28,/* 14 */
    35,/* 15 */
    44,/* 16 */
    56,/* 17 */
    70,/* 18 */
    89,/* 19 */
    100/* 20 */
};

/*  SFR Changes End  */

/*CA Changes start */
LTE_SQUEUE* scheduledFormat3Or3AQueueForPusch_gp[MAX_NUM_CELL]  = {PNULL};
TPC_TRIGGER_QUEUE_FOR_PUSCH_TYPE* tpcTriggerQueueForPusch_gp[MAX_NUM_CELL] = {PNULL};
#ifdef DL_UL_SPLIT
#ifdef HD_FDD_CONFIG
/*HD FDD Changes Start*/
/* Queue for storing UE which were triggered for TPC,
 * but could not be scheduled in case of HD FDD*/
TPC_SCHED_FAIL_QUEUE_PUSCH_TYPE* tpcSchedFailQueuePusch_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end */
/*HD FDD Changes End*/
#endif
#else
#ifdef HD_FDD_CONFIG
/*HD FDD Changes Start*/
/* Queue for storing UE which were triggered for TPC,
 * but could not be scheduled in case of HD FDD*/
LTE_SQUEUE* tpcSchedFailQueuePusch_gp[MAX_NUM_CELL] = {PNULL};
/*HD FDD Changes End*/
#endif
#endif


/* + SPR 11345 */
UInt32 minMCSIndexForATB_g[MAX_NUM_CELL] = {0};
UInt32 minPRBValForATB_g[MAX_NUM_CELL] = {0};
/* - SPR 11345 */


/* CLPC_CHG END */
/* SPR 13143 Changes start */
extern UInt32 dci3RightShiftOffset_g[MAX_NUM_CELL] ;
extern UInt32 dci3ARightShiftOffset_g[MAX_NUM_CELL];
/* SPR 13143 Changes end */
/******************************************************************************
* Function Name : validateNprbFromATBForSps 
* Inputs        : inputRBs - Represents the transmission bandwidth
*                 mcs - Represents the MCS of the transmission bandwidth
*                 ulUEContext_p - pointer to  UE's UL Context
*                 tpc -
*                 internalCellIndex - Cell-Index at MAC
* Outputs       : Number of RBs less than or equal to inputRBs so that
*                 UE's max power is not exceeded.
* Returns       : MAC_SUCCESS/MAC_FAILURE 
* Description   : Based on input MCS and RBs and the totalPowerCorrection,
*                 this function shall validate that scheduled transmission
*                 bandwidth and MCS does not exceed max Power of the UE and
*                 may reduce the RBs.
****************************************************************************/
MacRetType validateNprbFromATBForSps(
        UInt8 inputRBs,
        UInt8 mcs,
        ULUEContext *ulUEContext_p,
        SInt8 tpc,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    SInt16 tenLogMMax = 0;
    UInt8 tenLogMAllocated = tenLogMFactor_g[inputRBs - 1] ;
    SInt8 deltaTF = 0;

    if(ulUEContext_p->dedicatedULPowerControlInfo.deltaMcsEnabled)
    {
        deltaTF = mcsToDeltaTF_g[mcs];
    }
    else
    {
        deltaTF = 0;
    }

    /* NOTE: Whenever using alpha remember to divide it by 10 as values of alpha 0,.4,.5,.6,.7,.8,.9,1 are stored as 0,4,5,6,7,8,9,10*/
    tenLogMMax = ulUEContext_p->pcMax
        - (ulUEContext_p->spsUlInfo.spsUlSetupInfo_p->p0PersistentInfo.\
                p0NominalPuschPersistent + ulUEContext_p->spsUlInfo.spsUlSetupInfo_p->\
                p0PersistentInfo.p0UePuschPersistent)
        /*CA Changes start  */
        - (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->commonULPowerControlInfo.alpha * ulUEContext_p->pathLoss)/10
        /*CA Changes end  */
        - deltaTF
        - (ulUEContext_p->currPowerCorrectionForPusch + tpc);

    if(tenLogMMax >= tenLogMAllocated)
    {
        return MAC_SUCCESS;
    }
    else
    {
        return MAC_FAILURE;
    }
}


/******************************************************************************
 * Function Name : validateNprbFromATB
 * Inputs        : inputRBs - Represents the transmission bandwidth
 *                 mcs - Represents the MCS of the transmission bandwidth
 *                 ulUEContext_p - pointer to  UE's UL Context
 *                 tpc -
 *                 maxRBRestrict -
 *                 ueMaxPowerFlag -
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : Number of RBs less than or equal to inputRBs so that
 *                 UE's max power is not exceeded.
 * Returns       : Uplink PRB's 
 * Description   : Based on input MCS and RBs and the totalPowerCorrection,
 *                 this function shall validate that scheduled transmission
 *                 bandwidth and MCS does not exceed max Power of the UE and
 *                 may reduce the RBs.
 ****************************************************************************/
UInt8 validateNprbFromATB(
        UInt8 inputRBs,
        /* Time Averaging changes start */
        UInt8 mcs,
        ULUEContext *ulUEContext_p,
        SInt8 tpc,
        UInt32 maxRBRestrict,
        UInt32* ueMaxPowerFlag,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex) 
/* Time Averaging changes end */
{
    SInt16 tenLogMMax = 0;
    UInt8 tenLogMAllocated = tenLogMFactor_g[inputRBs - 1] ;
    /* Time Averaging changes start */
    UInt8 tenLogMRestricted = 1;
    /* Time Averaging changes end */
    UInt8 mAllocated = inputRBs;
    SInt8 deltaTF = 0;
    UInt8 tenLogLimit = 1;
    /*UInt8 hundredLogMMax = 0; This gives better accuracy*/
    if(ulUEContext_p->dedicatedULPowerControlInfo.deltaMcsEnabled)
    {
        deltaTF = mcsToDeltaTF_g[mcs];
    }
    else
    {
        deltaTF = 0;
    }
    /* NOTE: Whenever using alpha remember to divide it by 10 as values of alpha 0,.4,.5,.6,.7,.8,.9,1 are stored as 0,4,5,6,7,8,9,10*/
    tenLogMMax = ulUEContext_p->pcMax
        - (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                commonULPowerControlInfo.p0NominalPUSCH + ulUEContext_p->dedicatedULPowerControlInfo.p0UePusch)
        - (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->
                commonULPowerControlInfo.alpha * ulUEContext_p->pathLoss)/10
        - deltaTF
        /* Time Averaging changes start */
        - (ulUEContext_p->currPowerCorrectionForPusch + tpc);
    /* Time Averaging changes end */


    if(tenLogMMax >= tenLogMAllocated)
    {
        /* Number of PRBs on the given mcs do not cause the UE to transmit
         * at more than pcMax.
         **/
        *ueMaxPowerFlag = FALSE;
        return mAllocated;
    }
    else
    {
        /* Time Averaging changes start */
        tenLogMRestricted = tenLogMFactor_g[maxRBRestrict - 1] ;

        if(tenLogMRestricted >= tenLogMAllocated)
        {
            /*This means that tenLogMRestricted >= tenLogMAllocated > tenLogMMax */
            *ueMaxPowerFlag = TRUE;
            return mAllocated;

        }
        else if(tenLogMRestricted >= tenLogMMax)
        {
            /*This means that tenLogMAllocated > tenLogMRestricted >= tenLogMMax */
            tenLogLimit = tenLogMRestricted;
            *ueMaxPowerFlag = TRUE;
        }
        else
        {
            /*This means that tenLogMAllocated > tenLogMMax > tenLogMRestricted*/
            tenLogLimit = tenLogMMax;
        }
        for(;; mAllocated--)
        {
            if(tenLogLimit >= tenLogMFactor_g[mAllocated - 1])
            {
                break;
            }
        }
        if(!mAllocated)
        {
            ltePanic("[%d]tenLogLimit = %d,  tenLogMMax = %d, *ueMaxPowerFlag = %d, tenLogMRestricted = %d",getCurrentTick(),tenLogLimit,  tenLogMMax, *ueMaxPowerFlag,tenLogMRestricted );
            /*Following line is just to prevent GCC warning*/
            *ueMaxPowerFlag = TRUE;
            return inputRBs;
        }
        else
        {
            if(tenLogMFactor_g[mAllocated - 1] > tenLogMMax)
            {
                *ueMaxPowerFlag = TRUE;
            }
            else
            {
                *ueMaxPowerFlag = FALSE;
            }
            return getUplinkMinRBsInPowerOfTwoThreeFive(mAllocated);
        }
        /* Time Averaging changes end */
    }
}



/****************************************************************************
 * Function Name  : populatePreferredMcsRbTableForATB
 * Inputs         : None
 * Returns        : None
 * Outputs        : NOne
 * Description    : This function populates the MCS and RB. 
 ****************************************************************************/
/* + SPR 17439 */
void populatePreferredMcsRbTableForATB(void)
    /* - SPR 17439 */
{
    UInt8 mcsloopCount = 0;
    UInt8 rbloopCount = 0;
    UInt8 dbloopCount = 0;

    SInt8  deltaTfVal = 0;
    UInt8  tenLogMAllocatedVal = 0;
    for ( mcsloopCount = 0;
            mcsloopCount < MAX_MCS_INDEX ;
            mcsloopCount++ )
    {

        deltaTfVal = mcsToDeltaTF_g[mcsloopCount];

        for ( rbloopCount = 1; 
                rbloopCount <= MAX_RES_BLOCKS;
                rbloopCount++ )
        {

            tenLogMAllocatedVal = tenLogMFactor_g[rbloopCount-1];

            for ( dbloopCount = 1;
                    dbloopCount <= MAX_POWER_REDUCTION_FOR_MCS_RB;
                    dbloopCount ++ )
            {
                /*CYCLOMATIC_CHANGES_6.1_START*/
                calculatePreferredMcsRBForATB(mcsloopCount,rbloopCount,dbloopCount,
                        deltaTfVal,tenLogMAllocatedVal);
                /*CYCLOMATIC_CHANGES_6.1_END*/
                if( ( macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1][dbloopCount-1].preferredMcsForATB != INVALID_MCS_VAL_FOR_ATB &&
                            macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1][dbloopCount-1].preferredMcsForATB > mcsloopCount ) ||
                        ( macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1][dbloopCount-1].preferredRbForATB  != INVALID_RB_VAL_FOR_ATB &&
                          macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1][dbloopCount-1].preferredRbForATB > rbloopCount ) )
                {
                    ltePanic("Invalid values in the table \n");
                }
            }
        }
    }
}



/******************************************************************************
 * Function Name : validatePRBForATB
 * Inputs        : deltaTF,tenLogMAllocated,
 *                 mAllocated ,puschPowerControlFactor, maxPower
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : ueMaxPowerFlag 
 * Returns       : Uplink PRB's 
 * Description   : This function validates the number of PRB for the ATB
 *                 functionality
 ****************************************************************************/
UInt8 validatePRBForATB(
        UInt32       *ueMaxPowerFlag,
        SInt8        deltaTF,
        SInt16       tenLogMAllocated,
        UInt8        mAllocated, 
        SInt16       puschPowerControlFactor,
        SInt32       maxPower,
        InternalCellIndex internalCellIndex)
{
    SInt16 tenLogMMax = 0;
    UInt8 tenLogLimit = 1;
    UInt8 inputRBs = mAllocated;

    tenLogMMax = maxPower - puschPowerControlFactor - deltaTF; 

    /* SPR 7587 Fix Start */
    if (tenLogMMax >= MAX_TEN_LOG_M_FACTOR_TBL_VAL)
    {
        return mAllocated;
    }
    /* SPR 7587 Fix End */
    if(TEN_LOG_M_ALLOCATED_RESTRICTED_VAL >= tenLogMAllocated)
    {
        /*  Does this mean we will give less than 3 PRB's */
        /*This means that tenLogMRestricted >= tenLogMAllocated > tenLogMMax */
        *ueMaxPowerFlag = TRUE;
        return mAllocated;

    }
    else if(TEN_LOG_M_ALLOCATED_RESTRICTED_VAL >= tenLogMMax)
    {
        /*This means that tenLogMAllocated > tenLogMRestricted >= tenLogMMax */
        tenLogLimit = TEN_LOG_M_ALLOCATED_RESTRICTED_VAL;
    }
    else
    {
        /*This means that tenLogMAllocated > tenLogMMax > tenLogMRestricted*/
        tenLogLimit = tenLogMMax;
    }

    mAllocated = tenLogMFactorToPrb_g[tenLogLimit];        

    if(!mAllocated)
    {
        ltePanic("Invalid scenario\n" );
        /*Following line is just to prevent GCC warning*/
        *ueMaxPowerFlag = TRUE;
        return inputRBs;
    }
    else
    {
        if(tenLogMFactor_g[mAllocated - 1] > tenLogMMax)
        {
            *ueMaxPowerFlag = TRUE;
        }
        else
        {
            *ueMaxPowerFlag = FALSE;
        }
        return getUplinkMinRBsInPowerOfTwoThreeFive(mAllocated);
    }
}

/******************************************************************************
 * Function Name : validateATB
 * Inputs        : inputRBs - Represents the transmission bandwidth
 *                 mcs - Represents the MCS of the transmission bandwidth
 *                 ulUEContext_p - UE's UL Context
 *                 tcp -
 *                 ueMaxPowerFlag -
 *                 minUePowerFlag -
 *                 maxPower -
 *                 txType -
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : UInt8: Number of RBs less than or equal to inputRBs so that
 *                 UE's max power is not exceeded.
 * Description   : Based on input MCS and RBs and the totalPowerCorrection, 
 *                 this function shall validate that scheduled transmission 
 *                 bandwidth and MCS does not exceed max Power of the UE and 
 *                 may reduce the RBs.
 ****************************************************************************/
UInt8 validateATB(
        UInt8       inputRBs, 
        UInt8       *mcs, 
        ULUEContext *ulUEContext_p,
        SInt8       tpc,
        UInt32      *ueMaxPowerFlag,
        UInt32      *minUePowerFlag,
        SInt32      maxPower,
        UInt8       txType,
        /*CA Changes start */
        InternalCellIndex internalCellIndex)
/*CA Changes end */
{
    /* Coverity_73315 Fix Start */ 
    //   UInt8 tenLogMAllocated = tenLogMFactor_g[inputRBs - 1] ;
    UInt8 tenLogMAllocated = 0;
    if(inputRBs <= MAX_NUM_RBS)
    {
        tenLogMAllocated = tenLogMFactor_g[inputRBs - 1] ;
    }
    /* Coverity_73315 Fix End */ 

    SInt8 deltaTF = 0;
    UInt8 mAllocated = inputRBs;

    UInt8 mcsLimit = 0;
    SInt32 puschPowerControlFactor = 0;
    SInt8 totalPower = 0;
    UInt8 rbLimit = 0;
    UInt8 powerDiff = 0;
    UInt8 doMcsFirstThenRBMethod = FALSE;
    SInt8 powerDiffRemaining = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* SPR 13532 Fix Start */
    /* ulUEContext_p->currPowerCorrectionForPusch contains the last calculated TPC value */
    if (ulUEContext_p->dedicatedULPowerControlInfo.accumulationEnabled)
    {
        tpc += ulUEContext_p->currPowerCorrectionForPusch;
    }

    puschPowerControlFactor = 
        ( cellParams_p->commonULPowerControlInfo.p0NominalPUSCH + ulUEContext_p->dedicatedULPowerControlInfo.p0UePusch)
        + ( cellParams_p->commonULPowerControlInfo.alpha * ulUEContext_p->pathLoss)/10
        + ( tpc);
    /* SPR 13532 Fix End */


    if(ulUEContext_p->dedicatedULPowerControlInfo.deltaMcsEnabled)        
    {
        deltaTF = mcsToDeltaTF_g[*mcs];
    }

    totalPower = puschPowerControlFactor + deltaTF+tenLogMAllocated;

    /* LOGS for Total PUSCH & PATH LOSS*/
    LOG_MAC_MSG(MAC_PUSCH_POWER_ID, LOGBRIEF, MAC_UL_PC, getCurrentTick(),ulUEContext_p->ueIndex,totalPower,*mcs,\
            maxPower,ulUEContext_p->pathLoss,DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
            __func__, "UL Total Transmission Power for PUSCH & pathLoss\n" );

    /* CA Stats Changes Start */
    /* ATB stats Changes */
    LTE_MAC_UPDATE_STATS_FOR_ICIC_SFR(ulUEContext_p, getCurrentTick(),maxPower,
            totalPower,*mcs,inputRBs, internalCellIndex);
    /* CA Stats Changes End */

    if ( totalPower < ulUEContext_p->minUePower )
    {
        *minUePowerFlag = TRUE;

        LOG_MAC_MSG 
            (MAC_VALIDATE_ATB,\
             LOGDEBUG, MAC_ATB,\
             getCurrentTick(),\
             mAllocated,
             *mcs,
             *minUePowerFlag,
             *ueMaxPowerFlag,\
             DEFAULT_INT_VALUE,\
             DEFAULT_FLOAT_VALUE,\
             DEFAULT_FLOAT_VALUE,\
             __func__, "current power is less than minimum power\n" );
        return mAllocated;
    }

    if ( totalPower <=  maxPower )
    {

        *ueMaxPowerFlag = FALSE;
        LOG_MAC_MSG 
            (MAC_VALIDATE_ATB,\
             LOGDEBUG, MAC_ATB,\
             getCurrentTick(),\
             mAllocated,
             *mcs,
             *minUePowerFlag,
             *ueMaxPowerFlag,\
             DEFAULT_INT_VALUE,\
             DEFAULT_FLOAT_VALUE,\
             DEFAULT_FLOAT_VALUE,\
             __func__, "current power is less than maximum power\n" );
        return mAllocated;
    }
    else
    {
        if ( RE_TX == txType )
        {
            *ueMaxPowerFlag = TRUE;

            LOG_MAC_MSG 
                (MAC_VALIDATE_ATB,\
                 LOGDEBUG, MAC_ATB,\
                 getCurrentTick(),\
                 mAllocated,
                 *mcs,
                 *minUePowerFlag,
                 *ueMaxPowerFlag,\
                 DEFAULT_INT_VALUE,\
                 DEFAULT_FLOAT_VALUE,\
                 DEFAULT_FLOAT_VALUE,\
                 __func__, "retransmission case for ATB\n" );

            return mAllocated;
        }

        powerDiff = totalPower - maxPower; 

        if(ulUEContext_p->dedicatedULPowerControlInfo.deltaMcsEnabled)
        {
            if ( powerDiff <= MAX_DB_REDUCED_THROUGH_TABLE )
            {
                mcsLimit = macParams_g.preferredMcsRbForATBTbl[*mcs][inputRBs-1][powerDiff-1].preferredMcsForATB;
                rbLimit = macParams_g.preferredMcsRbForATBTbl[*mcs][inputRBs-1][powerDiff-1].preferredRbForATB;
                /* + SPR 11345 */
                if( mcsLimit < minMCSIndexForATB_g[internalCellIndex] || rbLimit < minPRBValForATB_g[internalCellIndex])
                {
                    doMcsFirstThenRBMethod = TRUE;
                }
                else
                {
                    if ( mcsLimit != INVALID_MCS_VAL_FOR_ATB )
                    {
                        *mcs = mcsLimit;
                        *ueMaxPowerFlag = FALSE;

                        LOG_MAC_MSG 
                            (MAC_VALIDATE_ATB,\
                             LOGBRIEF, MAC_ATB,\
                             getCurrentTick(),\
                             rbLimit,
                             *mcs,
                             *minUePowerFlag,
                             *ueMaxPowerFlag,\
                             DEFAULT_INT_VALUE,\
                             DEFAULT_FLOAT_VALUE,\
                             DEFAULT_FLOAT_VALUE,\
                             __func__, "validate ATB MCS+RB through MCS+RB table\n" );

                        return getUplinkMinRBsInPowerOfTwoThreeFive(rbLimit);
                    }
                    else
                    {
                        if ( *mcs > minMCSIndexForATB_g[internalCellIndex] )
                        {
                            *mcs = minMCSIndexForATB_g[internalCellIndex];
                        }

                        *ueMaxPowerFlag = TRUE;

                        LOG_MAC_MSG 
                            (MAC_VALIDATE_ATB,\
                             LOGBRIEF, MAC_ATB,\
                             getCurrentTick(),\
                             minPRBValForATB_g[internalCellIndex],
                             *mcs,
                             *minUePowerFlag,
                             *ueMaxPowerFlag,\
                             DEFAULT_INT_VALUE,\
                             DEFAULT_FLOAT_VALUE,\
                             DEFAULT_FLOAT_VALUE,\
                             __func__, "can not reduce power range for these values\n" );

                        if ( inputRBs >= minPRBValForATB_g[internalCellIndex] )
                        {
                            return getUplinkMinRBsInPowerOfTwoThreeFive(minPRBValForATB_g[internalCellIndex]);
                        }
                        else
                        {
                            return inputRBs;
                        }
                    }
                }
            }
            else
            {
                doMcsFirstThenRBMethod = TRUE; 
            }

            if ( TRUE == doMcsFirstThenRBMethod )
            { 
                if ( (deltaTF - powerDiff + DELTA_TF_NORMALIZED_DB_VAL) > DELTA_TF_RESTRICTED_VAL )
                {
                    mcsLimit = deltaTfToMCS_g[deltaTF - powerDiff + DELTA_TF_NORMALIZED_DB_VAL];
                    *mcs = mcsLimit;
                    *ueMaxPowerFlag = FALSE;

                    LOG_MAC_MSG 
                        (MAC_VALIDATE_ATB,\
                         LOGBRIEF, MAC_ATB,\
                         getCurrentTick(),\
                         mAllocated,
                         *mcs,
                         *minUePowerFlag,
                         *ueMaxPowerFlag,\
                         DEFAULT_INT_VALUE,\
                         DEFAULT_FLOAT_VALUE,\
                         DEFAULT_FLOAT_VALUE,\
                         __func__, "reducing through MCs only method\n" );

                    return mAllocated;
                }
                else
                {
                    if ( minMCSIndexForATB_g[internalCellIndex] < *mcs )
                    {
                        powerDiffRemaining = powerDiff - ( deltaTF  - mcsToDeltaTF_g[minMCSIndexForATB_g[internalCellIndex]]);
                    }
                    else
                    {
                        powerDiffRemaining = powerDiff;
                    }

                    if ( (tenLogMAllocated -powerDiffRemaining)  >= TEN_LOG_M_ALLOCATED_RESTRICTED_VAL )
                    {
                        rbLimit = tenLogMFactorToPrb_g[tenLogMAllocated - powerDiffRemaining];
                        if ( minMCSIndexForATB_g[internalCellIndex] < *mcs )
                        {
                            *mcs = minMCSIndexForATB_g[internalCellIndex];
                        }
                        *ueMaxPowerFlag = FALSE;

                        LOG_MAC_MSG 
                            (MAC_VALIDATE_ATB,\
                             LOGBRIEF, MAC_ATB,\
                             getCurrentTick(),\
                             rbLimit,
                             *mcs,
                             *minUePowerFlag,
                             *ueMaxPowerFlag,\
                             DEFAULT_INT_VALUE,\
                             DEFAULT_FLOAT_VALUE,\
                             DEFAULT_FLOAT_VALUE,\
                             __func__, "performing MCS first RB Method\n" );

                        return getUplinkMinRBsInPowerOfTwoThreeFive(rbLimit);
                    }
                    else
                    {
                        if ( *mcs > minMCSIndexForATB_g[internalCellIndex] )
                        {
                            *mcs = minMCSIndexForATB_g[internalCellIndex];
                        }

                        *ueMaxPowerFlag = TRUE;

                        LOG_MAC_MSG 
                            (MAC_VALIDATE_ATB,\
                             LOGBRIEF, MAC_ATB,\
                             getCurrentTick(),\
                             minPRBValForATB_g[internalCellIndex],
                             *mcs,
                             *minUePowerFlag,
                             *ueMaxPowerFlag,\
                             DEFAULT_INT_VALUE,\
                             DEFAULT_FLOAT_VALUE,\
                             DEFAULT_FLOAT_VALUE,\
                             __func__, "can not reduce through MCs first RB method\n" );

                        if ( inputRBs >= minPRBValForATB_g[internalCellIndex] )
                        {
                            return getUplinkMinRBsInPowerOfTwoThreeFive(minPRBValForATB_g[internalCellIndex]);
                        }
                        else
                        {
                            return inputRBs;
                        }
                        /* - SPR 11345 */
                    }
                }
            }
        }
        else
        {
            mAllocated = validatePRBForATB(ueMaxPowerFlag,
                    deltaTF,tenLogMAllocated,mAllocated,
                    puschPowerControlFactor,maxPower,
                    internalCellIndex
                    );  
            LOG_MAC_MSG 
                (MAC_VALIDATE_ATB,\
                 LOGBRIEF, MAC_ATB,\
                 getCurrentTick(),\
                 mAllocated,
                 *mcs,
                 *minUePowerFlag,
                 *ueMaxPowerFlag,\
                 DEFAULT_INT_VALUE,\
                 DEFAULT_FLOAT_VALUE,\
                 DEFAULT_FLOAT_VALUE,\
                 __func__, "deltaMCS disabled reducing through RB method\n" );

            return mAllocated;
        }
    }
    return mAllocated;
}

/* CLPC_CHG */
/*****************************************************************************
 * Function Name  : putEntryInTpcTriggerQueueForPusch
 * Inputs         : ueIndex - UE Index
 *                  expiryTick - Tick when the node gets expired
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates a new TpcTriggerQueueNode and inserts this
 *                  node in the tpcTriggeredQueuePucch_g  to be processed by worker thread.
 ******************************************************************************/
/* SPR 15909 fix start */
void putEntryInTpcTriggerQueueForPusch(UInt16 ueIndex, tickType_t expiryTick,
        /* SPR 15909 fix end */        
        /*CA Changes start */
        InternalCellIndex internalCellIndex)
/*CA Changes end */
{
    LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s] ueIndex [%d] Entry. ",
            __func__, ueIndex);
    TpcTriggerQueueNode* node_p = PNULL;
    GET_MEM_NODE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(node_p,TpcTriggerQueueNode);
    if(node_p != PNULL)
    {
        node_p->ueIndex = ueIndex;
        node_p->expiryTick  = expiryTick;
        if(!(ENQUEUE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(TpcTriggerQueueNode, \
                        (void *)&(node_p->nodeAnchor), internalCellIndex)))
        {
            LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                    getCurrentTick(),\
                    __LINE__,0,0,0,0,\
                    0.0,0.0,__FILE__,__func__);

            FREE_MEM_NODE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(node_p);
        }
    }

    return ;
}

#ifdef HD_FDD_CONFIG
/*HD FDD Changes Start*/
/*****************************************************************************
 * Function Name  : putEntryInTpcSchedFailQueueForPusch
 * Inputs         : node_p - 
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates a new TpcSchedFailQnode and inserts this
 *                  node in the tpcTriggeredQueuePucch_g  to be processed by worker thread.
 ******************************************************************************/
void putEntryInTpcSchedFailQueueForPusch(TpcTriggerQueueNode* node_p,
        /*CA Changes start */
        InternalCellIndex internalCellIndex)
/*CA Changes end */
{
    LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s] ueIndex [%d] Entry. ",
            __func__, node_p->ueIndex);
    if(node_p != PNULL)
    {
#ifdef DL_UL_SPLIT
        ENQUEUE_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(TpcTriggerQueueNode, (void *)&(node_p->nodeAnchor), 
                internalCellIndex);
#else
        if( pushNode(tpcSchedFailQueuePusch_gp[internalCellIndex], &(node_p->nodeAnchor)))
        {
            /*coverity fix 25226*/

            FREE_MEM_NODE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(node_p);
            return ;
            /*coverity fix 25226*/
        }
#endif
    }
    return ;
}

/*HD FDD Changes End*/

#endif
/* +DYNAMIC_ICIC */
/*****************************************************************************
 * Function Name  : putEntryInExplicitULDCIAndPHRScheduleQueue
 * Inputs         : ueIndex - UE Index
 *                  ulRequestType -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates a new explicitULPuschTpcScheduleQueue_g 
 *                  and inserts this node in the explicitULPuschTpcScheduleQueue_g 
 *                  thread.
 *****************************************************************************/
/*CA Changes start  */
void putEntryInExplicitULDCIAndPHRScheduleQueue(UInt16 ueIndex, ULGrantRequestType ulRequestType, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    ExplicitULDCIAndPHRNode* node_p = PNULL;
    /* -DYNAMIC_ICIC */
    /* Put entry in explicit queue only if the 
     * explicitTpcTriggerEnabled_g is TRUE*/
    /* +DYNAMIC_ICIC */
    if (UL_EXPLICIT_PHR_WITHOUT_DATA == ulRequestType)
    {
        GET_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p,\
                ExplicitULDCIAndPHRNode);
        if(node_p != PNULL)
        {
            node_p->ueIndex = ueIndex;
            node_p->ulGrantRequestType = UL_EXPLICIT_PHR_WITHOUT_DATA;

            /*CA Changes start  */
            if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(explicitULPuschTpcScheduleQueue_gp[internalCellIndex],
                            /*CA Changes end  */
                            ExplicitULDCIAndPHRNode, (void *)&(node_p->nodeAnchor))))
            {
                LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                        getCurrentTick(),\
                        __LINE__,0,0,0,0,\
                        0.0,0.0,__FILE__,__func__);

                FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
            }
        }
        return;
    }

    if (UL_TPC_WITHOUT_DATA == ulRequestType)
    {
        /* -DYNAMIC_ICIC */
        /* CA changes Start */
        if (explicitTpcTriggerEnabled_g[internalCellIndex] == TRUE)
            /* CA changes End */
        {
            GET_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p,\
                    ExplicitULDCIAndPHRNode);
            if(node_p != PNULL)
            {
                node_p->ueIndex = ueIndex;
                node_p->ulGrantRequestType = UL_TPC_WITHOUT_DATA;

                /*CA Changes start  */
                if(!(ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(explicitULPuschTpcScheduleQueue_gp[internalCellIndex],
                                /*CA Changes end  */
                                ExplicitULDCIAndPHRNode, (void *)&(node_p->nodeAnchor))))
                {
                    LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                            getCurrentTick(),\
                            __LINE__,0,0,0,0,\
                            0.0,0.0,__FILE__,__func__);

                    FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p);
                }
            }
        }
    }
    return;
}
/******************************************************************************
 * Function Name : getTpcForPusch 
 * Inputs        : ulUeContext_p - Pointer to UL UL Context 
 *                                 corresponding to the UE for which TPC is to be fetched.
 *                 currentTick - Represents the current global tick.
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None 
 * Returns       : TPC value
 * Description   : This function shall return the appropriate TPC value that can
 *                 be sent in DCI Format 0 by taking into considerations 
 *                 different triggers due to wideband and sub band power control
 *                 conditions.
 ****************************************************************************/
TpcTriggeredToPowerCorrection getTpcForPusch (
        ULUEContext *ulUeContext_p,
        /* This tick should represent the dlDelayToPhy + 
           currentTick in case of FDD */
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex
        /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG        
        ,UInt8 ulSubFrameNum
#endif        
        /*TDD Config 0 Changes End*/ 
        )
{
    SInt8 tpcCommand = TPC_NO_CHANGE;
    TpcRntiContext *tpcRntiContext_p = PNULL;
    TpcInfoForPusch *tpcInfoForPusch_p = PNULL;
    TpcTriggeredToPowerCorrection powCorrection = {0};

    /* Get the Tick corresponding to the uplink sub frame to which TPC command
       shall be applied by UE */
#ifdef FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currentAppliedTick = currentTick + FDD_HARQ_OFFSET;
    /* SPR 15909 fix end */

#elif TDD_CONFIG
    /*TDD Config 0 Changes Start*/
    UInt8 subFrame = currentTick % MAX_SUB_FRAME;
    /* SPR 15909 fix start */
    tickType_t currentAppliedTick = 
        currentTick +  getUlOffset(subFrame,ulSubFrameNum);
    /* SPR 15909 fix end */
    /*TDD Config 0 Changes End*/
#endif

    tpcInfoForPusch_p = &(ulUeContext_p->tpcInfoForPusch);

    /* First check if subband Power Control is enabled. and message type 
       is not TPC_MSG as in that case subband PC should not be dene*/
    {
        /* Get the tpc command from UE Context as triggered due to wideband
           conditions */
        tpcCommand = tpcInfoForPusch_p->tpcTriggeredValue;
    }

    /* Update the UE Context and TPC RNTI Context if TPC triggered is not 
       equal to no change */
    if (tpcCommand != TPC_NO_CHANGE)
    {
#ifdef PRINT
        fprintf(stderr,"[%s] [%d] UI ID[%d] isHDFddFlag [%d] is being entertained \n", __func__, getCurrentTick(), ulUeContext_p->ueIndex, ulUeContext_p->isHDFddFlag);
#endif
        /* Set the last Apllied tick in UE Context to the current Applied 
           Tick as now new TPC command is being sent */
        tpcInfoForPusch_p->lastAppliedTick = currentAppliedTick;

        // Reset the counter
        RESET_PUSCH_TPC_COOLOFF_COUNTER(ulUeContext_p);

        /* DCI Format 3A change start */
        /*Reset fakeTpcValue flag of UE context*/
        tpcInfoForPusch_p->fakeTpcValue = FAKE_TPC_NO_VALUE;
        /* DCI Format 3A change end */

        /* Check if UE is scheduled to send TPC using DCI FORMAT 3/3A */
        if (PNULL != tpcInfoForPusch_p->triggeredUeListNodeInTpcRntiContext_p)
        {
            /* Get the tpc rnti context */
            tpcRntiContext_p = 
                (tpcPuschRntiContext_gp[internalCellIndex] + tpcInfoForPusch_p->tpcRntiIndex);

            /* Decreemnt the triggered Count and remove the entry from 
               triggered UE List */
#ifdef PRINT
            fprintf(stderr,"[%s] [%d] , Triggered count [%d] \n", __func__, getCurrentTick(), tpcRntiContext_p->triggeredCount);
#endif

            deleteEntryFromTriggeredUeList( 
                    tpcInfoForPusch_p->triggeredUeListNodeInTpcRntiContext_p,
                    tpcRntiContext_p);

            LOG_MAC_MSG (MAC_DELETE_TRIGGERED_DCI3_UE_NODE_ID, LOGDEBUG, MAC_UL_PC,\
                    currentTick,\
                    ulUeContext_p->ueIndex,\
                    tpcInfoForPusch_p->tpcRntiIndex,\
                    tpcRntiContext_p->triggeredCount,\
                    tpcCommand,\
                    DEFAULT_INT_VALUE,\
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                    __func__,"DCI0_DCI3_COLLISION");

            tpcInfoForPusch_p->triggeredUeListNodeInTpcRntiContext_p = PNULL;

        }/* PNULL != triggeredUeListNodeInTpcRntiContext_p */
#ifdef PRINT
        fprintf(stderr,"[%s] [%d] , UE ID = %d , TPCRNTI [ %d] , TPC Command = %d, tpcInfoForPusch_p->tpcTriggeredValue = %d, isConfiguredForDciFormat3Or3A [%d] \n", __func__, getCurrentTick(), ulUeContext_p->ueIndex, ulUeContext_p->tpcInfoForPusch.tpcRnti, tpcCommand, tpcInfoForPusch_p->tpcTriggeredValue, tpcRntiContext_p->isConfiguredForDciFormat3Or3A);
#endif

        LOG_MAC_MSG (MAC_TPC_SENT_USING_DCI_FORMAT_0_ID,\
                LOGDEBUG, MAC_UL_PC,\
                currentTick,\
                ulUeContext_p->ueIndex,\
                tpcCommand,\
                tpcInfoForPusch_p->lastAppliedTick,\
                tpcInfoForPusch_p->tpcTriggeredValue,\
                ulUeContext_p->dedicatedULPowerControlInfo.accumulationEnabled,\
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
                __func__, " TPC_SENT_DCI0" );

    }/* tpcCommand != TPC_NO_CHANGE */

    /* Return the TPC value according to the accumulated/absolute value */
    if (ulUeContext_p->dedicatedULPowerControlInfo.accumulationEnabled)
    {
        powCorrection =  getAccumulatedPowerForPusch(
                tpcCommand,
                DCI_FORMAT_0);
    }
    else
    {
        powCorrection = getAbsolutePowerForPusch(
                tpcCommand,
                &ulUeContext_p->currPowerCorrectionForPusch);
    }

    /* Storing the tpc information to be sent it will be used if error 
     * indication rcvd from phy */
    /* Storing at current index .
     * it will be fetched at receiver from current tick - ERROR_IND Dealy  */
    if ( powCorrection.powerCorrectionVal )
    {
        /* SPR 21631 Start */
        UInt8 mapIdx = currentTick % MAX_TPC_CONTAINER_MAP_SIZE;
        /* SPR 21631 End */
        UInt16 rnti = 0;

        if ( SPS_SCHEDULING == ulUeContext_p->schType ) 
        { 
            rnti = ulUeContext_p->spsUlInfo.SpsCrnti;    
        }
        else
        {
            rnti = ulUeContext_p->crnti;    
        }

        putEntryInTpcSentMap( currentTick, mapIdx, rnti, 
                powCorrection.powerCorrectionVal,
                ERR_IND_TPC_FOR_PUSCH,internalCellIndex );
    }
    return powCorrection;
}/*getTpcForPusch*/

/*****************************************************************************
 * Function Name  : deleteEntryFromTriggeredUeList
 * Inputs         : triggeredUeListNodeInTpcRntiContext_p - pointer to Triggerd
 *                                  UE list contains th fail list flag
 *                  tpcRntiContext_p - Represents the TPC RNTI
 *                         context corresponding to the TPC PUSCH RNTI configured for UE
 * Outputs        : None
 * Returns        : Void
 * Description    : It deletes the UEs entry from triggered UE List maintained
 *                  in TPC RNTI COntext is UE is scheduled for TPC using DCI 
 *                  Format 3/3A.
 *****************************************************************************/
void deleteEntryFromTriggeredUeList(
        TriggeredUEListNode *triggeredUeListNodeInTpcRntiContext_p,
        TpcRntiContext * tpcRntiContext_p)
{
    /*The Node in the LIST will be delelted */
    if (triggeredUeListNodeInTpcRntiContext_p->isPresentInFailList == FALSE)
    {
        tpcRntiContext_p->triggeredCount--;
        listDeleteNode(&(tpcRntiContext_p->triggeredUeList),
                &triggeredUeListNodeInTpcRntiContext_p->triggeredUeAnchor);
    }
    else
    {
        //tpcRntiContext_p->failTriggerCount--;
        listDeleteNode(&(tpcRntiContext_p->failTriggerUeList),
                &triggeredUeListNodeInTpcRntiContext_p->triggeredUeAnchor);
    }
    /*Free the Memory of the Node*/
    freeMemPool(triggeredUeListNodeInTpcRntiContext_p);
}

/*****************************************************************************
 * Function Name  : deleteEntryFromConfiguredUeList
 * Inputs         : configuredUeListNodeInTpcRntiContext_p - Represents
 *                  the pointer to UE node added in configured UE list.
 *                  tpcRntiContext_p - Represents the TPC RNTI
 *                  context corresponding to the TPC PUSCH RNTI configured for UE
 * Outputs        : None
 * Returns        : None
 * Description    : It deletes a UEs entry from configured UE List maintained
 *                  in TPC RNTI Context if UE is scheduled for TPC using DCI 
 *                  Format 3A.
 *****************************************************************************/
void deleteEntryFromConfiguredUeList(
        ConfiguredUEListNode *configuredUeListNodeInTpcRntiContext_p,
        TpcRntiContext * tpcRntiContext_p)
{
    /*The Node in the LIST will be deleted */
    listDeleteNode(&(tpcRntiContext_p->configuredUeList),
            &configuredUeListNodeInTpcRntiContext_p->configuredUeAnchor);

    /*Free the Memory of the Node*/
    freeMemPool(configuredUeListNodeInTpcRntiContext_p);
}

/*****************************************************************************
 * Function Name  : addEntryToTpcTriggerQueueForPusch
 * Inputs         : ueIndex - UE Identifier
 *                  tpcTriggered - The TPC value as triggered due to 
 *                                 wideband condtions.
 *                  tpcInfoForPusch_p -  Pointer to the tpc 
 *                                       info present in UE context.
 *                  currentTick -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall add the entry of UE in 
 *                  tpcTriggerQueueForPusch and rnti triggered list after the 
 *                  wideband power control for PUSCH is triggered.
 *****************************************************************************/
void addEntryToTpcTriggerQueueForPusch (
        UInt16 ueIndex,
        SInt8 tpcTriggered,
        TpcInfoForPusch *tpcInfoForPusch_p,
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
/* + coverity 55308*/
{
    /* SPR 15909 fix start */
    tickType_t expiryTick = 0;
    /* SPR 15909 fix end */
    TpcRntiContext *tpcRntiContext_p = PNULL;
    TriggeredUEListNode* triggeredUeNode_p = PNULL;

    /* DCI Format3A changes start*/    
    /* If TpcRnti for the UE is configured then get the tpcRntiContext */
    if (tpcInfoForPusch_p->tpcRnti != INVALID_TPC_RNTI)
    {
        /* Get the tpc rnti context */
        tpcRntiContext_p = 
            (tpcPuschRntiContext_gp[internalCellIndex] + tpcInfoForPusch_p->tpcRntiIndex);

        /* Check if TPC RNTI is configured and triggered count for tpc rnti is not 
           zero i.e. tpc rnti is not in unset state, then add the entry of the UE 
           for DCI FORMAT 3/3A, Else add the entry of the UE in 
           tpcTriggerQueueForPusch */
        /* Coverity CID 54498 Start */
        /* Code removed for coverity */
        /* Coverity CID 54498 End */
        /* Increment the triggered count in tpc rnti context and add the 
           entry of the UE in triggeredUeList */
        tpcRntiContext_p->triggeredCount++;
        triggeredUeNode_p = addEntryInTriggeredUeList(
                ueIndex,
                &(tpcRntiContext_p->triggeredUeList), PNULL);

        if (triggeredUeNode_p == PNULL)
        {
            return;
        }
        /* Update the address of node added in UE Context */
        tpcInfoForPusch_p->triggeredUeListNodeInTpcRntiContext_p = 
            triggeredUeNode_p;	    

        LOG_MAC_MSG (MAC_ADD_ENTRY_TO_TPC_RNTI_TRIGGERED_UE_LIST_ID,\
                LOGDEBUG, MAC_UL_PC,\
                currentTick,\
                ueIndex, \
                tpcInfoForPusch_p->tpcRntiIndex, \
                tpcRntiContext_p->triggeredCount,\
                tpcTriggered,\
                DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,\
                __func__, "Adding Entry for DCI 3/3A in tpcRntiContext");
        /* Coverity CID 54498 Start */
        /* Code removed for coverity */
        /* Coverity CID 54498 End */
    }
    else
    {
        // Put enry in triggerd queue only if the explicitTpcTriggerEnabled_g
        // is enabled and TPC-RNTI context exists
        /* CA changes Start */
        if (explicitTpcTriggerEnabled_g[internalCellIndex] != TRUE) 
            /* CA changes End */
        {
            /* Set the value of TPC triggered into UE COntext */
            tpcInfoForPusch_p->tpcTriggeredValue = tpcTriggered;
            return;
        }
    }
#ifdef FDD_CONFIG
    expiryTick = currentTick + 
        /*CA Changes start  */
        (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->tpcTriggerWaitTimeForPUSCHDCI0;
    /*CA Changes end  */
#elif TDD_CONFIG
    expiryTick = 
        tddConvertTimerValForUlGrant(
                (macReconfigSchedulerParamsUL_gp[internalCellIndex]+ schParamsIndexUL_g[internalCellIndex])->tpcTriggerWaitTimeForPUSCHDCI0,
                currentTick,
                internalCellIndex);
#endif
    putEntryInTpcTriggerQueueForPusch(ueIndex, expiryTick, internalCellIndex);
    LOG_MAC_MSG (MAC_ADD_ENTRY_TO_TPC_TRIGGER_QUEUE_ID,\
            LOGDEBUG, MAC_UL_PC,\
            currentTick,\
            ueIndex, \
            expiryTick,\
            /*CA Changes start  */
            (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->tpcTriggerWaitTimeForPUSCHDCI0,\
            /*CA Changes end  */
            tpcTriggered,\
            tpcInfoForPusch_p->tpcRnti,\
            DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,\
            __func__, "Adding Entry to tpcTriggereQueueForPusch");

#ifdef PRINT
    fprintf(stderr, "[%s] [%d] , UEID[%d] .. Triggerd count = %d, tpcValue[%d] \n", __func__, getCurrentTick(), ueIndex, tpcRntiContext_p->triggeredCount, tpcTriggered);
#endif
    /* Set the value of TPC triggered into UE Context */
    tpcInfoForPusch_p->tpcTriggeredValue = tpcTriggered;
}/* addEntryToTpcTriggerQueueForPusch */
/* - coverity 55308*/

/*HD FDD Changes Start*/
/*****************************************************************************
 * Function Name  : processTpcUeTriggerNodePuschForDci3A
 * Inputs         : triggeredUeListNode_p - POinter to the Trigger UE List node
 *                  tpcRntiContext_p - Represents the TPC RNTI
 *                                     context corresponding to the TPC PUSCH RNTI configured for UE
 *                  tempDciCCEInfo_p - Pointer to the DCI CCE info
 *                  currentTick - current global tick
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall process the triggered UE list node
 *                  in case of DCI 3A
 *****************************************************************************/
void processTpcUeTriggerNodePuschForDci3A
(
 TriggeredUEListNode* triggeredUeListNode_p,
 TpcRntiContext* tpcRntiContext_p,
 DciCCEInfo* tempDciCCEInfo_p,
 /* SPR 15909 fix start */
 tickType_t currentTick,
 /* SPR 15909 fix end */
 /*CA Changes start */
 InternalCellIndex internalCellIndex
 /*CA Changes end */
 )
{
    /* + Coverity 54527 */
    TpcInfoForPusch * tpcInfoForPusch_p = PNULL;
#ifdef HD_FDD_CONFIG                
    /* SPR 15909 fix start */
    tickType_t localTick = 0;
    /* SPR 15909 fix end */
#endif
    ULUEContext* ulUeContext_p = PNULL;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
#ifdef FDD_CONFIG
    UInt8 tempHarqProcessId = MODULO_EIGHT(currentTick + PHY_DL_DELAY + FDD_HARQ_OFFSET);
#endif

    if (!ulUECtxInfoArr_g[triggeredUeListNode_p->ueIndex].pendingDeleteFlag)
    {
        /* Get the tpcInfoForPusch from UE COntext using the UE ID
           present in node */ 
        ulUeContext_p = 
            ulUECtxInfoArr_g[triggeredUeListNode_p->ueIndex].\
            ulUEContext_p;
        tpcInfoForPusch_p = &(ulUeContext_p->tpcInfoForPusch);

#ifdef HD_FDD_CONFIG                
        localTick = getCurrentTick() + PHY_DL_DELAY;
#endif
#ifdef FDD_CONFIG                    
        /* TTIB_Code Start */
        if ((TRUE == ulUeContext_p->measGapContext.isMeasGapPeriodOn) ||
                (DRX_STATE_ON == ulUeContext_p->drxCurrentState)          ||
                (TRUE == checkSpsActTti(ulUeContext_p,tempHarqProcessId)))
            /* TTIB_Code End */
#elif TDD_CONFIG
            if ((TRUE == ulUeContext_p->measGapContext.isMeasGapPeriodOn) ||
                    (DRX_STATE_ON == ulUeContext_p->drxCurrentState))
#endif
            {
                /* +DYNAMIC_ICIC */
                putEntryInExplicitULDCIAndPHRScheduleQueue(
                        /* -DYNAMIC_ICIC */
                        triggeredUeListNode_p->ueIndex, UL_TPC_WITHOUT_DATA,
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */

                tpcRntiContext_p->triggeredCount--;
                /*
                   LOG_MAC_MSG (MAC_ADD_ENTRY_EXPLICIT_IN_DRX_PUSCH_ID,\
                   LOGDEBUG, MAC_UL_PC,\
                   currentTick,\
                   triggeredUeListNode_p->ueIndex,\
                   tpcMsgInfoForPusch_p->tpcRntiIndex,\
                   tpcInfoForPusch_p->tpcTriggeredValue,\
                   ulUeContext_p->measGapContext.isMeasGapPeriodOn,\
                   ulUeContext_p->drxCurrentState,\
                   DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
                   __func__, "EXPLICIT_UL_DCI" );
                   */

            }

        /* Check for collision with DCI Format 0 and DRX/MSG Gap */
            else if ((tpcRntiContext_p->appliedUlTick != tpcInfoForPusch_p->lastAppliedTick) 
#ifdef HD_FDD_CONFIG                
                    &&
                    /*HD FDD Changes Start*/
                    (((TRUE == ulUeContext_p->isHDFddFlag) &&
                      (CAN_SCHEDULE_TPC_FOR_HD_UE(localTick, ulUeContext_p->ueIndex, hdUeContextInfo_g.hdfddUeSchedMap)))||
                     (FALSE == ulUeContext_p->isHDFddFlag))
#endif                
                    )
                /*HD FDD Changes End*/
            {
                powerCorrectionAndTpc = getAccumulatedPowerForPusch(
                        tpcInfoForPusch_p->tpcTriggeredValue,
                        DCI_FORMAT_3A);


                /* SPR 13143 Changes start */
                /* Set tpcTriggerd at tpcRntiIndex in pDciPdu_p->tpcBitmap 
                 * e.g if 10Mhz ,tpcRntiIndex = 1 ,from formula (32-1) -5 = 26. 
                 * so tpcTriggered will be left shifted to 26 bits */

                UInt8 offset = 
                    (((32 - tpcInfoForPusch_p->tpcBitMapIndex)) -
                     dci3ARightShiftOffset_g[internalCellIndex]);
                /* SPR 13143 Changes end */
                /* set the bit to the tpc command */
                tempDciCCEInfo_p->dciPduInfo.tpcBitmap |= powerCorrectionAndTpc.tpcCommand <<  offset;

                if (powerCorrectionAndTpc.powerCorrectionVal)
                {
                    /* SPR 21631 Start */
                    UInt8 mapIdx = currentTick % MAX_TPC_CONTAINER_MAP_SIZE;
                    /* SPR 21631 End */
                    UInt16 rnti = 0;

                    ulUeContext_p->prevPowerCorrectionForPusch =
                        ulUeContext_p->currPowerCorrectionForPusch;
                    ulUeContext_p->currPowerCorrectionForPusch +=
                        powerCorrectionAndTpc.powerCorrectionVal;
                    CHECK_AND_UPDATE_POWER_CORRECTION_FOR_PUSCH(
                            ulUeContext_p->currPowerCorrectionForPusch);

                    if ( SPS_SCHEDULING == ulUeContext_p->schType ) 
                    { 
                        rnti = ulUeContext_p->spsUlInfo.SpsCrnti;    
                    }
                    else
                    {
                        rnti = ulUeContext_p->crnti;    
                    }

                    /* Storing the tpc information to be sent it will be used 
                     * if error indication rcvd from phy */
                    /* Storing at current index, it will be fetched at receiver
                     * from current tick - ERROR_IND Dealy  */
                    putEntryInTpcSentMap( currentTick, mapIdx, rnti, 
                            powerCorrectionAndTpc.powerCorrectionVal,
                            ERR_IND_TPC_FOR_PUSCH,
                            internalCellIndex);
                }

                /*Reset the counter*/
                RESET_PUSCH_TPC_COOLOFF_COUNTER(ulUeContext_p);

                /* Reset the fakeTpcValue*/
                tpcInfoForPusch_p->fakeTpcValue = FAKE_TPC_NO_VALUE;

                /* Set triggeredUeListNodeInTpcRntiContext_p to PNULL  */
                tpcInfoForPusch_p->triggeredUeListNodeInTpcRntiContext_p = PNULL;
#ifdef PRINT
                fprintf(stderr,"[%s] [%d] UE ID [%d] ulUeContext_p->isHDFddFlag[%d] Encoding the bitmap at offset [%d] , TPC CMD[%d] Bitmap [%d] \n", __func__, getCurrentTick(), ulUeContext_p->ueIndex, ulUeContext_p->isHDFddFlag, offset, powerCorrectionAndTpc.tpcCommand, tempDciCCEInfo_p->dciPduInfo.tpcBitmap);
#endif
                LOG_MAC_MSG (MAC_TPC_SENT_DCI_3OR3A_PUSCH_ID,\
                        LOGDEBUG, MAC_UL_PC,\
                        currentTick,\
                        triggeredUeListNode_p->ueIndex,\
                        powerCorrectionAndTpc.powerCorrectionVal,\
                        tpcInfoForPusch_p->lastAppliedTick,\
                        ulUeContext_p->prevPowerCorrectionForPusch,\
                        ulUeContext_p->currPowerCorrectionForPusch,\
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
                        __func__, "TPC_SENT_DCI3OR3A" );

                /* Free the node
                 * Pop next node from the triggerdUeList
                 */
                freeMemPool(triggeredUeListNode_p);
            }
            else
            {
#ifdef HD_FDD_CONFIG
                if((TRUE ==  ulUeContext_p->isHDFddFlag) && 
                        (tpcRntiContext_p->appliedUlTick != tpcInfoForPusch_p->lastAppliedTick))
                {
                    /*put entry into the failure trigger list*/
#ifdef PRINT
                    fprintf(stderr,"[%s] [%d], UE ID[%d] isHDFddFlag [%d] Entry put back in FailTriggeredUeList to be processed later\n", __func__, getCurrentTick(), ulUeContext_p->ueIndex, ulUeContext_p->isHDFddFlag);
#endif

                    addEntryInFailTriggeredUeList(triggeredUeListNode_p, &(tpcRntiContext_p->failTriggerUeList), internalCellIndex);
                    UPDATE_UE_DENIED_DL_RESOURCE(ulUeContext_p->ueIndex);
                }
#endif
                tpcRntiContext_p->triggeredCount--;
                /* NOTE : No Need to free the node here since same node is being used in failTriggerUeList */
            }
    }
    else
    {
        tpcRntiContext_p->triggeredCount--;
    }

    return ;
    /* - Coverity 54527 */
}

/*****************************************************************************
 * Function Name  : processTpcUeTriggerNodePuschForDci3
 * Inputs         : triggeredUeListNode_p - POinter to the Trigger UE List node
 *                  tpcRntiContext_p - Represents the TPC RNTI
 *                                     context corresponding to the TPC PUSCH RNTI configured for UE
 *                  tempDciCCEInfo_p - Pointer to the DCI CCE info
 *                  currentTick - current global tick
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall process the triggered UE list node
 *                  in case of DCI 3
 *****************************************************************************/
void processTpcUeTriggerNodePuschForDci3
(
 TriggeredUEListNode* triggeredUeListNode_p,
 TpcRntiContext* tpcRntiContext_p,
 DciCCEInfo* tempDciCCEInfo_p,
 /* SPR 15909 fix start */
 tickType_t currentTick,
 /* SPR 15909 fix end */
 /*CA Changes start */
 InternalCellIndex internalCellIndex
 /*CA Changes end */
 /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
 ,UInt8 ulSubFrameNum
#endif
 /*TDD Config 0 Changes End*/ 
 )
/* + coverity 54527 */
{
    ULUEContext* ulUeContext_p = PNULL;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    TpcInfoForPusch * tpcInfoForPusch_p = PNULL;
#ifdef FDD_CONFIG
    UInt8 tempHarqProcessId = MODULO_EIGHT(currentTick + PHY_DL_DELAY + FDD_HARQ_OFFSET);
#endif
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t localTick = 0;
    /* SPR 15909 fix end */
#endif

    /* Get the tpcInfoForPusch from UE COntext using the UE ID
       present in node */ 
    ulUeContext_p = 
        ulUECtxInfoArr_g[triggeredUeListNode_p->ueIndex].\
        ulUEContext_p;
    tpcInfoForPusch_p = &(ulUeContext_p->tpcInfoForPusch);
    /* Skip processing this UE in this TTI if it is in
     * Meas Gap state or DRX on state or SPS activation DCI needs to be sent
     * */

    /*SPR - 20318 Start*/
    if (ulUECtxInfoArr_g[triggeredUeListNode_p->ueIndex].pendingDeleteFlag)
    {
        freeMemPool(triggeredUeListNode_p);
        return;
    }
    /*SPR - 20318 End*/

#ifdef FDD_CONFIG 
    /* For TTIB_Code */
    if ((TRUE == ulUeContext_p->measGapContext.isMeasGapPeriodOn) ||
            (DRX_STATE_ON == ulUeContext_p->drxCurrentState)          ||
            (TRUE == checkSpsActTti(ulUeContext_p,tempHarqProcessId)))
#elif TDD_CONFIG
    if ((TRUE == ulUeContext_p->measGapContext.isMeasGapPeriodOn) ||
            (DRX_STATE_ON == ulUeContext_p->drxCurrentState))
#endif

    {
        /* +DYNAMIC_ICIC */
        putEntryInExplicitULDCIAndPHRScheduleQueue(
                /* -DYNAMIC_ICIC */
                triggeredUeListNode_p->ueIndex, UL_TPC_WITHOUT_DATA,
                /*CA Changes start  */
                internalCellIndex);
        /*CA Changes end  */

        tpcRntiContext_p->triggeredCount--;
        tpcInfoForPusch_p->triggeredUeListNodeInTpcRntiContext_p = PNULL;

        /*
           LOG_MAC_MSG (MAC_ADD_ENTRY_EXPLICIT_IN_DRX_PUSCH_ID,\
           LOGDEBUG, MAC_UL_PC,\
           currentTick,\
           triggeredUeListNode_p->ueIndex,\
           tpcMsgInfoForPusch_p->tpcRntiIndex,\
           tpcInfoForPusch_p->tpcTriggeredValue,\
           ulUeContext_p->measGapContext.isMeasGapPeriodOn,\
           ulUeContext_p->drxCurrentState,\
           DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
           __func__, "EXPLICIT_UL_DCI" );
           */

        freeMemPool(triggeredUeListNode_p);

        return ;
    }
#ifdef HD_FDD_CONFIG
    /*HD FDD Changes Start*/
    localTick = getCurrentTick() + PHY_DL_DELAY;
#endif
    /* Check for collision with DCI Format 0 and DRX/MSG Gap */
    if ((tpcRntiContext_p->appliedUlTick != tpcInfoForPusch_p->lastAppliedTick) 
#ifdef HD_FDD_CONFIG            
            && 
            (((TRUE == ulUeContext_p->isHDFddFlag) &&
              (CAN_SCHEDULE_TPC_FOR_HD_UE(localTick, ulUeContext_p->ueIndex, hdUeContextInfo_g.hdfddUeSchedMap)) )||
             (FALSE == ulUeContext_p->isHDFddFlag))
#endif            
       )
        /*HD FDD Changes end*/
    {
        powerCorrectionAndTpc = getAccumulatedPowerForPusch(
                tpcInfoForPusch_p->tpcTriggeredValue,
                DCI_FORMAT_3);

        /* SPR 13143 Changes start */
        /* Set tpcTriggerd at tpcRntiIndex in pDciPdu_p->tpcBitmap 
         * e.g if 10Mhz ,tpcRntiIndex = 1 then bit7 and bit8 needs to be 
         * populated so from formula (16-1 *2) -6 = 24. so 
         * tpcTriggered will be left shifted to 24 bits to set
         * the bit7 and bit 8. */

        /*+- SPR 20899 Fix*/
        UInt8 offset = 
            ((MULTIPLY_BY_TWO(16 - tpcInfoForPusch_p->tpcBitMapIndex)) -
             dci3RightShiftOffset_g[internalCellIndex]);
        /*+- SPR 20899 Fix*/

        /* SPR 13143 Changes end */

        /* clear the 2 bits from offet and then set it to the 
           tpc command */
        tempDciCCEInfo_p->dciPduInfo.tpcBitmap &= 
            (~(1 << offset));
        tempDciCCEInfo_p->dciPduInfo.tpcBitmap &= 
            (~(1 << (offset+1)));

        tempDciCCEInfo_p->dciPduInfo.tpcBitmap |=
            powerCorrectionAndTpc.tpcCommand <<  offset;

        if (powerCorrectionAndTpc.powerCorrectionVal)
        {
            /* SPR 21631 Start */
            UInt8 mapIdx = currentTick % MAX_TPC_CONTAINER_MAP_SIZE;
            /* SPR 21631 End */
            UInt16 rnti = 0;

            ulUeContext_p->prevPowerCorrectionForPusch =
                ulUeContext_p->currPowerCorrectionForPusch;
            ulUeContext_p->currPowerCorrectionForPusch +=
                powerCorrectionAndTpc.powerCorrectionVal;

            CHECK_AND_UPDATE_POWER_CORRECTION_FOR_PUSCH(
                    ulUeContext_p->currPowerCorrectionForPusch);

            if ( SPS_SCHEDULING == ulUeContext_p->schType ) 
            { 
                rnti = ulUeContext_p->spsUlInfo.SpsCrnti;    
            }
            else
            {
                rnti = ulUeContext_p->crnti;    
            }

            /* Storing the tpc information to be sent it will be used if error 
             * indication rcvd from phy */
            /* Storing at current index, it will be fetched at receiver from 
             * current tick - ERROR_IND Dealy  */
            putEntryInTpcSentMap( currentTick, mapIdx, rnti, 
                    powerCorrectionAndTpc.powerCorrectionVal,
                    ERR_IND_TPC_FOR_PUSCH,
                    internalCellIndex);
        }
        /* Set TPC value in ue context as no change */
        tpcInfoForPusch_p->tpcTriggeredValue = TPC_NO_CHANGE;

        /* Set triggeredUeListNodeInTpcRntiContext_p to PNULL  */
        tpcInfoForPusch_p->triggeredUeListNodeInTpcRntiContext_p = PNULL;

        /* Set the last Applied Tick in UE context */
#ifdef FDD_CONFIG
        tpcInfoForPusch_p->lastAppliedTick = 
            currentTick + FDD_HARQ_OFFSET;
#elif TDD_CONFIG
        /*TDD Config 0 Changes Start*/ 
        UInt8 subFrame = currentTick % MAX_SUB_FRAME;
        tpcInfoForPusch_p->lastAppliedTick =  
            currentTick +  getUlOffset(subFrame,ulSubFrameNum);
        /*TDD Config 0 Changes End*/ 
#endif
        /* Reset the counter*/
        RESET_PUSCH_TPC_COOLOFF_COUNTER(ulUeContext_p);
#ifdef PRINT
        fprintf(stderr,"[%s] [%d] UE ID [%d] ulUeContext_p->isHDFddFlag[%d] Encoding the bitmap at offset [%d] , TPC CMD[%d] Bitmap [%d] \n", __func__, getCurrentTick(), ulUeContext_p->ueIndex, ulUeContext_p->isHDFddFlag, offset, powerCorrectionAndTpc.tpcCommand, tempDciCCEInfo_p->dciPduInfo.tpcBitmap);
#endif

        LOG_MAC_MSG (MAC_TPC_SENT_DCI_3OR3A_PUSCH_ID,\
                LOGDEBUG, MAC_UL_PC,\
                currentTick,\
                triggeredUeListNode_p->ueIndex,\
                powerCorrectionAndTpc.powerCorrectionVal,\
                tpcInfoForPusch_p->lastAppliedTick,\
                ulUeContext_p->prevPowerCorrectionForPusch,\
                ulUeContext_p->currPowerCorrectionForPusch,\
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
                __func__, "TPC_SENT_DCI3OR3A" );
        /* Free the node popped from the queue */
        freeMemPool(triggeredUeListNode_p);
    }
    else
    {
#ifdef HD_FDD_CONFIG
        if((TRUE == ulUeContext_p->isHDFddFlag) && 
                (tpcRntiContext_p->appliedUlTick != tpcInfoForPusch_p->lastAppliedTick))
        {
            /*Putting into failure triggerlist and decrementing the trigger count
             * and increment the failure count*/
            /*put entry into the failure trigger list*/
#ifdef PRINT
            fprintf(stderr,"[%s] [%d], UE ID[%d] isHDFddFlag [%d] Entry put back in FailTriggeredUeList to be processed later\n", __func__, getCurrentTick(), ulUeContext_p->ueIndex, ulUeContext_p->isHDFddFlag);
#endif

            addEntryInFailTriggeredUeList(triggeredUeListNode_p, &(tpcRntiContext_p->failTriggerUeList), internalCellIndex);

            UPDATE_UE_DENIED_DL_RESOURCE(ulUeContext_p->ueIndex);
            //tpcRntiContext_p->failTriggerCount++;
        }
#endif
        tpcRntiContext_p->triggeredCount--;
        /* NOTE : No Need to free the node here since same node is being used in failTriggerUeList */
    }

    return ;
}
/* - coverity 54527 */
/*HD FDD Changes End*/

/*****************************************************************************
 * Function Name  : encodeTpcBitmapForPusch
 * Inputs         : containerIndex - container index for PDCCH
 *                  pdcchTpcMsgInfoForPusch_p - structure to be stored for 
 *                  PUSCH use
 *                  currentTick -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall encode TPC bitmap for PUSCH power control 
 *                  depending on the DCI FORMAT 3/3A scheduled for the TPC PUSCH RNTI.
 *                  The bit for non-triggered UEs is encoded with a fake TPC Value. 
 *                  A UE context variable fakeTpcValue is maintained to keep track 
 *                  of fake Value sent for the UE. The range of values for fakeTpcValue 
 *                  is 0, -1 and 1 where 0 means that the last TPC value sent was real; 
 *                  -1 means that the last TPC value sent was fake "-1"; and 1 means that 
 *                  the last TPC value sent was fake "1".
 *****************************************************************************/
void encodeTpcBitmapForPusch(
        UInt8 containerIndex,
        PdcchTpcMsgInfoForPusch *pdcchTpcMsgInfoForPusch_p, 
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        /*TDD Config 0 Changes Start*/         
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /*TDD Config 0 Changes End*/ 
        )
{
    UInt8 count = pdcchTpcMsgInfoForPusch_p->count;
    TpcMsgInfoForPusch * tpcMsgInfoForPusch_p = PNULL;
    DciCCEContainerInfo * dciCCEContainerInfo_p = PNULL;
    DciCCEInfo * tempDciCCEInfo_p = PNULL;
    TpcRntiContext * tpcRntiContext_p = PNULL;
    TriggeredUEListNode * triggeredUeListNode_p = PNULL;
    ConfiguredUEListNode * configuredUeListNode_p = PNULL;
    TpcInfoForPusch * tpcInfoForPusch_p = PNULL;
    ULUEContext * ulUeContext_p = PNULL;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
#ifdef FDD_CONFIG
    UInt8 tempHarqProcessId = MODULO_EIGHT(currentTick + PHY_DL_DELAY + FDD_HARQ_OFFSET);
#endif

#ifdef HD_FDD_CONFIG
    /*HD FDD Changes Start*/
    /* SPR 15909 fix start */
    tickType_t localTick = getCurrentTick() + PHY_DL_DELAY;
    /* SPR 15909 fix end */
#endif

    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerIndex;

    /* Process each TPC RNTI that is scheduled successfully from PDCCH for 
       PUSCH power control */
    while (count--)
    {
        tpcMsgInfoForPusch_p = 
            &(pdcchTpcMsgInfoForPusch_p->tpcMsgInfoForPusch[count]);
#ifdef PRINT
        fprintf(stderr,"[%s] [%d] Count = %d \n", __func__, getCurrentTick(), count);
#endif

        /* switch according to dciFormat configured for the tpcRnti */
        switch( tpcMsgInfoForPusch_p->dciFormat)
        {
            case DCI_FORMAT_3:
            {
                /* Get the dciPduInfo using the CCE index assigned */
                tempDciCCEInfo_p = 
                    &dciCCEContainerInfo_p->\
                    dciCCEInfo[tpcMsgInfoForPusch_p->cceIndex];

                /* Set the tpcBitmap to its default value */
                tempDciCCEInfo_p->dciPduInfo.tpcBitmap = DEFAULT_VAL_FOR_DCI3;

                /* We have get the tpc rnti context using the tpc index present
                   in the node. From the tpc rnti context, using the triggered
                   UE list, we can get the UE IDs for which TPC is triggered 
                   and corresponding value shall be set to tpcBitmap is TPC 
                   can be sent */

                /* Get the tpc rnti context */
                tpcRntiContext_p = 
                    (tpcPuschRntiContext_gp[internalCellIndex] + tpcMsgInfoForPusch_p->tpcRntiIndex);

                /* HD FDD Changes Start*/
                /* No Need to check failTriggerUeList here since failTriggerUeList
                 * is being used to repopulate triggerUEList in triggerDciForPuschPowerControl()
                 * and made empty in that function */
                /* HD FDD Changes End*/
                triggeredUeListNode_p = (TriggeredUEListNode*)listPopNode(&tpcRntiContext_p->triggeredUeList);
                while (PNULL != triggeredUeListNode_p)
                {
                    /*HD FDD Changes Start*/
                    processTpcUeTriggerNodePuschForDci3(triggeredUeListNode_p,
                            tpcRntiContext_p, tempDciCCEInfo_p, currentTick,
                            internalCellIndex
                            /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                            ,ulSubFrameNum
#endif
                            /*TDD Config 0 Changes End*/
                            );
                    triggeredUeListNode_p = (TriggeredUEListNode*)listPopNode(&tpcRntiContext_p->triggeredUeList);
                    /*HD FDD Changes End*/
                } /*  while (PNULL != triggeredUeListNode_p) */

                /* If triggered Count becomes zero, no need to send the 
                   DCI PDU */
                if( !tpcRntiContext_p->triggeredCount)
                {
                    mac_reset_cce(tempDciCCEInfo_p->dciPduInfo.aggregationLvl, 
                            tempDciCCEInfo_p, internalCellIndex) ;
                }
                break;
            } /* case DCI_FORMAT_3 */

            case DCI_FORMAT_3A:
            {
                /* Get the dciPduInfo using the CCE index assigned */
                tempDciCCEInfo_p = 
                    &dciCCEContainerInfo_p->\
                    dciCCEInfo[tpcMsgInfoForPusch_p->cceIndex];

                /* Set the tpcBitmap to its default value */
                tempDciCCEInfo_p->dciPduInfo.tpcBitmap = DEFAULT_VAL_FOR_DCI3A;

                /* HD FDD Changes Start*/
                /* No Need to check failTriggerUeList here since failTriggerUeList
                 * is being used to repopulate triggerUEList in triggerDciForPuschPowerControl()
                 * and made empty in that function */
                /* HD FDD Changes End*/
                /* We have get the tpc rnti context using the tpc index present
                   in the node. From the tpc rnti context, using the triggered
                   UE list, we can get the UE IDs for which TPC is triggered 
                   and corresponding value shall be set to tpcBitmap is TPC 
                   can be sent */

                /* Get the tpc rnti context */
                tpcRntiContext_p = 
                    (tpcPuschRntiContext_gp[internalCellIndex] + tpcMsgInfoForPusch_p->tpcRntiIndex);
                /*HD FDD Changes Start*/
                /*HD FDD Changes End*/
                /* DCI Format 3A changes start*/
                /* get the node from rnti's triggeredList*/
                triggeredUeListNode_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                /* Traversing the configuredUE list to mark the power change for triggered UEs*/
                while (PNULL != triggeredUeListNode_p)
                {
                    /*HD FDD Changes Start*/
                    processTpcUeTriggerNodePuschForDci3A(triggeredUeListNode_p,
                            tpcRntiContext_p, tempDciCCEInfo_p, currentTick,
                            internalCellIndex);
                    triggeredUeListNode_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                    /*HD FDD Changes End*/
                } /* while (PNULL != triggeredUeListNode_p) */

                /* If triggered Count becomes zero, no need to send the 
                   DCI PDU */
                if( !tpcRntiContext_p->triggeredCount)
                {
                    mac_reset_cce(tempDciCCEInfo_p->dciPduInfo.aggregationLvl, 
                            tempDciCCEInfo_p, internalCellIndex) ;
                    break;
                }

                /* get the node from rnti's configuredList*/
                configuredUeListNode_p =
                    (ConfiguredUEListNode*)getListNode(&(tpcRntiContext_p->\
                                configuredUeList),0);

                /* Traversing the configuredUE list to mark the power change for untriggered UEs*/
                while (configuredUeListNode_p != PNULL)
                {
                    if (!ulUECtxInfoArr_g[configuredUeListNode_p->ueIndex].pendingDeleteFlag)
                    {
                        /* Get the tpcInfoForPusch from UE COntext using the UE ID
                           present in node */
                        ulUeContext_p =
                            ulUECtxInfoArr_g[configuredUeListNode_p->ueIndex].\
                            ulUEContext_p;
                        tpcInfoForPusch_p = &(ulUeContext_p->tpcInfoForPusch);

                        if (tpcRntiContext_p->appliedUlTick !=
                                tpcInfoForPusch_p->lastAppliedTick)

                        {
                            if ( TPC_NO_CHANGE == ulUeContext_p->tpcInfoForPusch.tpcTriggeredValue)
                            {
                                if (0 == ulUeContext_p->tpcInfoForPusch.fakeTpcValue)
                                {
                                    /* fakeTpcValue equals 0 which means that the last TPC 
                                     * command sent for this UE was real, hence send a fake "1" this time.
                                     */
                                    ulUeContext_p->tpcInfoForPusch.fakeTpcValue = FAKE_TPC_POWER_PLUS_1;
                                    powerCorrectionAndTpc.powerCorrectionVal = FAKE_TPC_POWER_PLUS_1;
                                    powerCorrectionAndTpc.tpcCommand = 1;
                                }
                                else if(1 == ulUeContext_p->tpcInfoForPusch.fakeTpcValue)
                                {
                                    /* fakeTpcValue equals 1 which means that the last TPC  
                                     * Value sent for this UE was fake "1", hence send a fake 
                                     * "-1" this time.
                                     */
                                    ulUeContext_p->tpcInfoForPusch.fakeTpcValue = FAKE_TPC_POWER_MINUS_1;
                                    powerCorrectionAndTpc.powerCorrectionVal = FAKE_TPC_POWER_MINUS_1;
                                    powerCorrectionAndTpc.tpcCommand = 0;
                                }
                                else
                                {
                                    /* fakeTpcValue equals -1 which means that the last TPC  
                                     * Value sent for this UE was fake "-1", hence send a fake 
                                     * "1" this time.
                                     */
                                    ulUeContext_p->tpcInfoForPusch.fakeTpcValue = FAKE_TPC_POWER_PLUS_1;
                                    powerCorrectionAndTpc.powerCorrectionVal = FAKE_TPC_POWER_PLUS_1;
                                    powerCorrectionAndTpc.tpcCommand = 1;
                                }

                                ulUeContext_p->prevPowerCorrectionForPusch =
                                    ulUeContext_p->currPowerCorrectionForPusch;
                                ulUeContext_p->currPowerCorrectionForPusch +=
                                    powerCorrectionAndTpc.powerCorrectionVal;
                                CHECK_AND_UPDATE_POWER_CORRECTION_FOR_PUSCH(
                                        ulUeContext_p->currPowerCorrectionForPusch);
                                /* SPR 13143 Changes start */
                                UInt8 offset = (((32 -
                                                tpcInfoForPusch_p->tpcBitMapIndex))
                                        - dci3ARightShiftOffset_g[internalCellIndex]);
                                /* SPR 13143 Changes end */

                                /* set the bit to the tpc command */
                                tempDciCCEInfo_p->dciPduInfo.tpcBitmap |= powerCorrectionAndTpc.tpcCommand <<  offset;
                            }
                            else
                            {
                                /*Reset the tpcTriggered value only if 
                                 * measGapPeriod is false and 
                                 * DRX state is off and 
                                 * SPS Activation DCI has not been sent
                                 * */
#ifdef FDD_CONFIG                                
                                /* For TTIB_Code */
                                if ((TRUE != ulUeContext_p->measGapContext.isMeasGapPeriodOn) &&
                                        (DRX_STATE_ON != ulUeContext_p->drxCurrentState)          &&
                                        (FALSE == checkSpsActTti(ulUeContext_p,tempHarqProcessId))
#ifdef HD_FDD_CONFIG
                                        &&
                                        /*HD FDD Changes Start*/
                                        (((TRUE == ulUeContext_p->isHDFddFlag) &&
                                          (CAN_SCHEDULE_TPC_FOR_HD_UE(localTick, ulUeContext_p->ueIndex, hdUeContextInfo_g.hdfddUeSchedMap)))||
                                         (FALSE == ulUeContext_p->isHDFddFlag))
#endif
                                   )
#elif TDD_CONFIG
                                    if ((TRUE != ulUeContext_p->measGapContext.isMeasGapPeriodOn) &&
                                            (DRX_STATE_ON != ulUeContext_p->drxCurrentState)) 
#endif
                                    {
                                        ulUeContext_p->tpcInfoForPusch.tpcTriggeredValue = TPC_NO_CHANGE;
                                        /* Set the last Applied Tick in UE context */
#ifdef FDD_CONFIG
                                        tpcInfoForPusch_p->lastAppliedTick = 
                                            currentTick + FDD_HARQ_OFFSET;
#elif TDD_CONFIG
                                        /*TDD Config 0 Changes Start*/ 
                                        UInt8 subFrameNum = currentTick % MAX_SUB_FRAME;
                                        tpcInfoForPusch_p->lastAppliedTick = currentTick + 
                                            getUlOffset(subFrameNum,ulSubFrameNum);
                                        /*TDD Config 0 Changes End*/ 
#endif
                                    }
                                /*else
                                  {
                                  fprintf(stderr,"[%s] [%d] Skipping the UEID[%d] isHDFddFlag[%d] for DCI3A.. To be processed in next Tick tpcTriggeredValue[%d]  \n", __func__, getCurrentTick(), ulUeContext_p->ueIndex, ulUeContext_p->isHDFddFlag, ulUeContext_p->tpcInfoForPusch.tpcTriggeredValue);
                                  }*/

                            }
                        }
                    }
                    configuredUeListNode_p =
                        (ConfiguredUEListNode*)getNextListNode(&configuredUeListNode_p->configuredUeAnchor);
                }

            } /* case DCI_FORMAT_3A */
            break;

            default:
            {
                lteWarning ("Invalid DCI Format.\n");
                return;
            }
        }/*switch(tpcMsgInfoForPusch_p->dciFormat)*/
        /* Reset the triggered Count  and scheduledState in tpc rnti context */
        tpcRntiContext_p->triggeredCount = 0;
        tpcRntiContext_p->scheduledState = UNSET_SCHEDULED_STATE;
        /* DCI Format 3A changes end*/
    } /* while (count--) */
} /* encodeTpcBitmapForPusch */

/*HD FDD Changes Start*/
/*****************************************************************************
 * Function Name  : processTpcTriggerAndSchedFailQueueForPusch
 * Inputs         : tpcTriggerQueueNode_p - Pointer to the Trigger UE Queue node
 *                  expiryTick - Tick when the node gets expired
 *                  currentTick - current global tick
 *                  isListForFailHdUEs - list contains HD FDD UE
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        :
 * Returns        : None
 * Description    :This function process will process tpcSchedFailQueue  and
 *                 tpcTriggerQueue. First tpcSchedFailQueue will be processed
 *                 and the tpcTriggerQueue will be processed.
 *****************************************************************************/
void processTpcTriggerAndSchedFailQueueForPusch
(
 TpcTriggerQueueNode* tpcTriggerQueueNode_p,
 /* SPR 15909 fix start */
 /* +- SPR 17777 */
 tickType_t currentTick,
 /* SPR 15909 fix end */
 UInt8 isListForFailHdUEs,
 /*CA Changes start */
 InternalCellIndex internalCellIndex
 /*CA Changes end */
 )
{
    ULUEContext * ulUeContext_p = PNULL;
    TpcInfoForPusch * tpcInfoForPusch_p = PNULL;
    TpcRntiContext * tpcRntiContext_p = PNULL;
#ifdef HD_FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t localTick = 0;
    /* SPR 15909 fix end */
#endif
    TriggeredUEListNode * node_p = PNULL;
    ULUEContext * ulUeCntx_p = PNULL;
    ULUEContextInfo * ulUeContextInfo_p = PNULL;

    /* Get the tpcInfoForPusch from UE COntext using the UE ID
       present in node */
    ulUeContextInfo_p = 
        &ulUECtxInfoArr_g[tpcTriggerQueueNode_p->ueIndex];

    if (!ulUeContextInfo_p->pendingDeleteFlag)
    {
        ulUeContext_p = ulUeContextInfo_p->ulUEContext_p;
        tpcInfoForPusch_p = &(ulUeContext_p->tpcInfoForPusch);

        if (tpcInfoForPusch_p->tpcRnti == INVALID_TPC_RNTI)
        {
            if (TPC_NO_CHANGE != tpcInfoForPusch_p->tpcTriggeredValue)
            {
                /* If TPC RNTI for the UE is not configured then add the
                   entry to explicit queue to be processed by strategy.
                   Free the node and continue to process other node in the
                   tpcTriggerQueueForPusch_g */
                /* +DYNAMIC_ICIC */
                putEntryInExplicitULDCIAndPHRScheduleQueue(
                        /* -DYNAMIC_ICIC */
                        tpcTriggerQueueNode_p->ueIndex, UL_TPC_WITHOUT_DATA,
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */

                LOG_MAC_MSG (MAC_ADD_ENTRY_EXPLICIT_TPC_FOR_PUSCH_ID,\
                        LOGDEBUG, MAC_UL_PC,\
                        currentTick,\
                        tpcTriggerQueueNode_p->ueIndex,\
                        tpcInfoForPusch_p->tpcTriggeredValue,\
                        tpcInfoForPusch_p->tpcRnti,\
                        tpcTriggerQueueNode_p->expiryTick,\
                        currentTick,\
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
                        __func__, "EXPLICIT_UL_DCI" );

            }
        }
        else
        {
            tpcRntiContext_p =  
                (tpcPuschRntiContext_gp[internalCellIndex] + tpcInfoForPusch_p->tpcRntiIndex);
            /* Valid TPC-RNTI */
            if (PNULL != tpcRntiContext_p)
            {
                /*HD FDD Changes Start*/
                /* Note : HD Flag check should be done before checking tpcRntiContet->ScheduledState
                 * else failure queue process will not happen till the time next TPC will trigger for
                 * Same TPC RNTI
                 */
#ifdef HD_FDD_CONFIG
                localTick = getCurrentTick()+ PHY_DL_DELAY;
                if(TRUE == ulUeContext_p->isHDFddFlag &&
                        !(CAN_SCHEDULE_TPC_FOR_HD_UE(localTick, ulUeContext_p->ueIndex, hdUeContextInfo_g.hdfddUeSchedMap)))
                {
                    /*While putting entry in the failQ expiry tick will be same expiry tick received in the
                     * function from TPC Trigger node*/
                    /*No need to Free the node popped*/
                    /*Put node entry back to failue Q*/
                    if (TPC_NO_CHANGE != tpcInfoForPusch_p->tpcTriggeredValue)
                    {
#ifdef PRINT
                        fprintf(stderr,"[%d] Putting entry in tpcSchedFailQueuePusch_g for UEID[%d] isHDFddFlag[%d] map 0x[%x] 0x[%x] \n", getCurrentTick(), tpcTriggerQueueNode_p->ueIndex, ulUeContext_p->isHDFddFlag,
                                hdUeContextInfo_g.hdfddUeSchedMap[(localTick)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ulUeContext_p->ueIndex]
                                ,hdUeContextInfo_g.hdfddUeSchedMap[(localTick+HD_FDD_SCHED_MAP_OFFSET_FOUR)%HD_UE_SCHED_MAP_SIZE].hdfddUeSfMap[ulUeContext_p->ueIndex] );
#endif
                        putEntryInTpcSchedFailQueueForPusch(tpcTriggerQueueNode_p,
                                internalCellIndex);
                        /* Increment the statistics counter UE denied DL resources due to
                         * UL clash in the same TTI of HD FDD UE*/
                        UPDATE_UE_DENIED_DL_RESOURCE(ulUeContext_p->ueIndex);
                        return;
                    }
                }
#endif


                if ((TPC_NO_CHANGE != tpcInfoForPusch_p->tpcTriggeredValue) &&
                        (tpcRntiContext_p->scheduledState == UNSET_SCHEDULED_STATE))
                {
                    /* Check if there is atleast one UE triggered in the TPC-RNTI group
                     * Note: PendingDelete check has been deliberately skipped since it will be checked
                     * while encoding the TPC command. In the worst case, there is a possibility that
                     * the TPC-RNTI shall be added to schedule queue even if the only UE triggered in this
                     * TPC-RNTI group is pending to be deleted. However, by skipping this check here, we can
                     * handle the case where UE was marked pendingDelete and TPC-RNTI was released for the
                     * triggered UE before it could be scheduled.
                     */
                    if ((tpcRntiContext_p->configuredCount > 0) && (tpcRntiContext_p->triggeredCount > 0))
                    {
                        /*HD FDD Changes End*/
                        if ((tpcRntiContext_p->isConfiguredForDciFormat3Or3A == DCI_FORMAT_3) ||
                                (((tpcRntiContext_p->triggeredCount * 100)/
                                  tpcRntiContext_p->configuredCount) >= TPC_NUM_UE_THRESHOLD_FOR_PUSCH_DCI3A))
                        {
#ifdef PRINT
                            fprintf(stderr,"[%s] [%d], putting entry in scheduledFormat3Or3AQueueForPusch_g for TPCRNTI Index [%d], TPC RNTI [%d] \n", __func__, getCurrentTick(), tpcRntiContext_p->tpcRntiIndex, tpcRntiContext_p->tpcRnti );
#endif
                            putEntryOfTpcIndexToScheduledQueueForPusch(tpcRntiContext_p->tpcRntiIndex,
                                    internalCellIndex);
                            tpcRntiContext_p->scheduledState =
                                tpcRntiContext_p->isConfiguredForDciFormat3Or3A ==
                                DCI_FORMAT_3?SCHEDULED_FORMAT3:SCHEDULED_FORMAT3A;

                            LOG_MAC_MSG (MAC_TPC_RNTI_ENTRY_IN_SCHEDULED_3OR3A_QUEUE_ID,\
                                    LOGDEBUG,  MAC_UL_PC,\
                                    currentTick,\
                                    tpcRntiContext_p->tpcRntiIndex,\
                                    tpcRntiContext_p->scheduledState,\
                                    tpcRntiContext_p->triggeredCount,\
                                    tpcRntiContext_p->configuredCount,\
                                    DEFAULT_INT_VALUE, \
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,\
                                    __func__, "SCHEDULED_3OR3A" );
                        }
                        else
                        {
                            /* pop node from triggeredUeList */
                            if(isListForFailHdUEs == TRUE)
                            {
                                node_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->failTriggerUeList));
                            }
                            else
                            {
                                node_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                            }

                            while (PNULL != node_p)
                            {
                                ulUeContextInfo_p = &ulUECtxInfoArr_g[node_p->ueIndex];

                                if (!ulUeContextInfo_p->pendingDeleteFlag)
                                {
                                    /* +DYNAMIC_ICIC */
                                    putEntryInExplicitULDCIAndPHRScheduleQueue(
                                            /* -DYNAMIC_ICIC */
                                            node_p->ueIndex, UL_TPC_WITHOUT_DATA,
                                            /*CA Changes start  */
                                            internalCellIndex);
                                    /*CA Changes end  */
                                    /* Reset triggeredUeListNodeInTpcRntiContext_p in UE context*/
                                    ulUeCntx_p = ulUECtxInfoArr_g[node_p->ueIndex].ulUEContext_p;
                                    ulUeCntx_p->tpcInfoForPusch.triggeredUeListNodeInTpcRntiContext_p = PNULL;
                                }
#ifdef PRINT
                                fprintf(stderr,"[%s] [%d] isListForFailHdUEs [%d] Freeing Memory of the Node [%x] \n", __func__, getCurrentTick(), isListForFailHdUEs, node_p);
#endif
                                /*Decrease the triggeredCount*/
                                freeMemPool(node_p);
                                if(isListForFailHdUEs == TRUE)
                                {
                                    node_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->failTriggerUeList));
                                }
                                else
                                {
                                    tpcRntiContext_p->triggeredCount--;
                                    node_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->triggeredUeList));
                                }
                            }
                        }
                    }
                    else
                    {
                    }
                }
            }
        }
    }
    /* Free the node popped */
    FREE_MEM_NODE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(tpcTriggerQueueNode_p);
    return;

}
/*HD FDD Changes End*/

/*****************************************************************************
 * Function Name  : triggerDciForPuschPowerControl
 * Inputs         : currentTick - current global tick
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall process the tpcTriggerQueueForPusch. For
 *                  each entry present in Queue, it shall trigger TPC commands 
 *                  for PUSCH power control, either using DCI format 3 /3A for 
 *                  the UE if TPC-RNTI is configured else using explicit DCI 0.
 *****************************************************************************/
/* SPR 15909 fix start */
void  triggerDciForPuschPowerControl(tickType_t currentTick, InternalCellIndex internalCellIndex)
    /* SPR 15909 fix end */    
{
    UInt8 tpcTriggeredQueueCount = 0;
    TpcTriggerQueueNode * tpcTriggerQueueNode_p = PNULL;
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    TpcInfoForPusch * tpcInfoForPusch_p = PNULL;
    ULUEContextInfo * ulUeContextInfo_p = PNULL;
    ULUEContext * ulUeContext_p = PNULL;
    TpcRntiContext * tpcRntiContext_p = PNULL;
    TriggeredUEListNode * node_p = PNULL;
    UInt8 tpcSchedFailQueueCount = 0;
    TpcTriggerQueueNode* failQnode_p = PNULL;
#endif
    /*HD FDD Changes End*/

    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* Process the TPC Fail Queue before processing TPC Trigger Queue
     * Fail Queue will be filled for HD FDD UE when TPC is triggered for it
     * but it cannot be scheduled DL(because SF is meant for UL), 
     * hence in the subsequent ticks first
     * priority will be given to these UE's for TPC in DCI 3 and 3A*/
    /* Calculate the queueCount of tpcTriggerQueueForPusch */
#ifdef DL_UL_SPLIT
    tpcSchedFailQueueCount = COUNT_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(internalCellIndex);
#else
    tpcSchedFailQueueCount = sQueueCount(tpcSchedFailQueuePusch_gp[internalCellIndex]);
#endif
    while(tpcSchedFailQueueCount--)
    {
        /*Get the first node from the queue*/
#ifdef DL_UL_SPLIT
        /* +COVERITY 29846 */
        if (MAC_FAILURE == DEQUEUE_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(TpcTriggerQueueNode,(void *)&(failQnode_p),
                    internalCellIndex))
        {
            failQnode_p = PNULL;
        }
        /* -COVERITY 29846 */
#else
        failQnode_p = (TpcTriggerQueueNode*)popNode(tpcSchedFailQueuePusch_gp[internalCellIndex]);
#endif
        if(failQnode_p)
        {
            ulUeContextInfo_p =
                &ulUECtxInfoArr_g[failQnode_p->ueIndex];

            ulUeContext_p = ulUeContextInfo_p->ulUEContext_p;
            tpcInfoForPusch_p = &(ulUeContext_p->tpcInfoForPusch);

            tpcRntiContext_p =
                (tpcPuschRntiContext_gp[internalCellIndex] + tpcInfoForPusch_p->tpcRntiIndex);

            if (tpcRntiContext_p != PNULL)
            {
                /* Repopulate the TriggereUeList from failTriggerUeList so that only 1 list will be processed
                 * to keep complexity low.
                 */
#ifdef PRINT
                fprintf(stderr,"[%s] [%d], UE ID [%d] Going to process Failure queue tpcSchedFailQueuePusch_g failTriggerCount[%d] triggeredCount[%d], FailListCount[%d] \n", __func__, getCurrentTick(), failQnode_p->ueIndex, tpcRntiContext_p->failTriggerCount, tpcRntiContext_p->triggeredCount, listCount(&(tpcRntiContext_p->failTriggerUeList)));
#endif

                UInt32 lstCount = listCount(&(tpcRntiContext_p->failTriggerUeList));
                while(lstCount != 0)
                {
                    node_p = (TriggeredUEListNode*)listPopNode(&(tpcRntiContext_p->failTriggerUeList));
#ifdef PRINT
                    fprintf(stderr,"[%s] [%d] , UE ID [%d]  Populating Triggered UeList from failTriggeredUeList fpr TPCRNTI [%d] failCount [%d]\n", __func__, getCurrentTick(), node_p->ueIndex, tpcRntiContext_p->tpcRnti, tpcRntiContext_p->failTriggerCount);
#endif
                    /*HD FDD Coverity Fix Start*/ 
                    /* Coverity Fix start*/
                    if (node_p == PNULL)
                    {
                        // Should not come here!!
                        lstCount--;
                        continue;
                    }
                    /*HD FDD Coverity Fix End*/ 
                    node_p = addEntryInTriggeredUeList(node_p->ueIndex,
                            &(tpcRntiContext_p->triggeredUeList), node_p);

                    if (node_p == PNULL)
                    {
                        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                LOGERROR,MAC_UL_Strategy,\
                                getCurrentTick(),\
                                DEFAULT_INT_VALUE,\
                                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,__func__,
                                "node_p entry failed");
                        lstCount--;
                        continue;
                    }
                    /* Coverity Fix start*/
                    tpcRntiContext_p->triggeredCount++;
                    lstCount--;
                }
            }

            /*No need to check expiry tick for tpcSchedFailQueuePusch_g
             * queue because the nodes have been pushed into it only
             * after expiry during processing of tpcTriggeredQueuePusch_g*/
            /* +- SPR 17777 */
            processTpcTriggerAndSchedFailQueueForPusch(failQnode_p, currentTick, TRUE,
                    /*CA Changes start */
                    internalCellIndex);
            /*CA Changes end */
        }
        else
        {
            lteWarning("Error : Node popped is NULL %s" , __func__);
            break;
        }
    }
#endif
    /*HD FDD Changes End*/

    tpcTriggeredQueueCount = COUNT_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(internalCellIndex);

    /* Loop through the queue nodes */
    while(tpcTriggeredQueueCount--)
    {

        /* Get the first node from the queue */
        /* +COVERITY 26449 */
        if (MAC_FAILURE == GET_NODE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(TpcTriggerQueueNode,\
                    (void *)&(tpcTriggerQueueNode_p), internalCellIndex))
        {
            tpcTriggerQueueNode_p = PNULL;
        }
        /* -COVERITY 26449 */

        if(tpcTriggerQueueNode_p)
        {
            /* Check if node is expired or not. If not break from the while loop */
            if( tpcTriggerQueueNode_p->expiryTick  < currentTick)
            {
                /* Pop the node from the queue */

                DEQUEUE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(TpcTriggerQueueNode,\
                        (void *)&(tpcTriggerQueueNode_p), internalCellIndex);
#ifdef PRINT
                fprintf(stderr,"[%s] [%d], Going to process queue tpcTriggeredQueuePucch_g [UE ID [%d] \n", __func__, getCurrentTick(), tpcTriggerQueueNode_p->ueIndex);
#endif
                /*HD FDD Coverity Fix Start*/
                if(PNULL == tpcTriggerQueueNode_p)
                {
                    continue;
                }
                /*HD FDD Coverity Fix End*/
                processTpcTriggerAndSchedFailQueueForPusch(tpcTriggerQueueNode_p,
                        /* +- SPR 17777 */
                        currentTick,
                        FALSE,
                        /*CA Changes start */
                        internalCellIndex);
                /*CA Changes end */

            }
            else /* tpcTriggerQueueNode_p->expiryTick > currentTick */
            {
                /* Case when expiry of node is not reached. Since nodes are 
                   inserted in FIFO order of their expiry, thus no need to 
                   process other nodes also */
                break; 
            }
        }
        else
        {
            continue;
        }
    } /* while(tpcTriggeredQueueCount--) */
} /* triggerDciForPuschPowerControl */

/*****************************************************************************
 * Function Name  : putEntryOfTpcIndexToScheduledQueueForPusch
 * Inputs         : tpcRntiIndex - Represents the TPC RNTI
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall put an entry in 
 *                  scheduledFormat3Or3AQueueForPusch_g.
 *****************************************************************************/
void putEntryOfTpcIndexToScheduledQueueForPusch(UInt16 tpcRntiIndex,
        InternalCellIndex internalCellIndex)
{
    ScheduledFormat3Or3AQueueNode * node_p = PNULL;

    /* Get memory pool */
    GET_MEM_FROM_POOL(ScheduledFormat3Or3AQueueNode, node_p,
            sizeof(ScheduledFormat3Or3AQueueNode), PNULL);

    if(node_p != PNULL)
    {
        node_p->tpcRntiIndex = tpcRntiIndex;

        /* + coverity 32521 */
        if(pushNode( scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex],
                    &node_p->nodeAnchor))
        {
            freeMemPool(node_p);
        }
        /* - coverity 32521 */
    }
}

/*****************************************************************************
 * Function Name  : populatePathlossToTargetSINRMapForPUSCH 
 * Inputs         : alphaBasedPathLossToTargetSINRMap_p,sps_flag 
 * Outputs        : schedulerParams_p 
 * Returns        : None
 * Description    : This function ipopulates the path loss to Target SINR Ratio
 *                   for PUSCH
 *****************************************************************************/
void populatePathlossToTargetSINRMapForPUSCH(AlphaBasedPathLossToTargetSINRMap *alphaBasedPathLossToTargetSINRMap_p,
        MacReconfigSchedulerParamsUL *schedulerParams_p,UInt8 sps_flag)
{
    /*Populates macPathlossToTargetSINRMapForPUSCH_g*/
    UInt8 indexAlpha = 0;
    UInt8 count = 0;
    UInt8 indexPL = 0;
    SInt16 startPL = 0;
    SInt16 endPL = 0;
    PathLossToTargetSINRMap *pathLossToTargetSINRMap_p;
    // Update the defaultPathLoss 
    if (LTE_TRUE == sps_flag)
    {
        schedulerParams_p->alphaBasedPathLossToTargetSINRMapSps.defaultPathLoss = 
            alphaBasedPathLossToTargetSINRMap_p->defaultPathLoss;
    }
    else
    {
        schedulerParams_p->alphaBasedPathLossToTargetSINRMap.defaultPathLoss = 
            alphaBasedPathLossToTargetSINRMap_p->defaultPathLoss;
    }

    for(indexAlpha = 0; indexAlpha < MAX_NUM_ALPHA_COUNT;indexAlpha++)
    {
        pathLossToTargetSINRMap_p = \
                                    &alphaBasedPathLossToTargetSINRMap_p->pathLossToTargetSINRMap[indexAlpha];
        count = pathLossToTargetSINRMap_p->count;

        while(count)
        {
            count--;
            /* SPR_4201 Start */
            startPL = \
                      (pathLossToTargetSINRMap_p->pathLossToTargetSINRMapInfo[count].startPL); 
            //- MIN_PATHLOSS)/10;
            endPL =  (pathLossToTargetSINRMap_p->pathLossToTargetSINRMapInfo[count].endPL); 
            //- MIN_PATHLOSS)/10;
            /* SPR_4201 End */
            for(indexPL = startPL; indexPL <= endPL; indexPL++)
            {
                if (LTE_TRUE == sps_flag)
                {
                    schedulerParams_p->macPathlossToTargetSINRMapForSpsPUSCH[indexAlpha][indexPL] \
                        = pathLossToTargetSINRMap_p->pathLossToTargetSINRMapInfo[count].targetSINR;
                }
                else
                {
                    schedulerParams_p->macPathlossToTargetSINRMapForPUSCH[indexAlpha][indexPL] \
                        = pathLossToTargetSINRMap_p->pathLossToTargetSINRMapInfo[count].targetSINR;
                }

            }
        }
    }
}

/*Populates macDeltaSINRToTPCMapForPUSCH_g*/
/*****************************************************************************
 * Function Name  : populateSinrToTpcMapForPUSCH 
 * Inputs         : deltaSINRToTPCMapForPUSCH_p 
 * Outputs        : schedulerParams_p 
 * Returns        : None
 * Description    : This function ipopulates the SINR to TPC Ratio for PUSCH
 *****************************************************************************/
void populateSinrToTpcMapForPUSCH(DeltaSINRToTPCMapForPUSCH *deltaSINRToTPCMapForPUSCH_p,
        MacReconfigSchedulerParamsUL *schedulerParams_p)
{
    UInt32 count = deltaSINRToTPCMapForPUSCH_p->count;
    DeltaSINRToTPCMapForPUSCHInfo *deltaSINRToTPCMapForPUSCHInfo_p;
    UInt32 index = 0;
    UInt32 startIndex = 0;
    UInt32 endIndex = 0;

    while (count)
    {
        count--;
        deltaSINRToTPCMapForPUSCHInfo_p = \
                                          &(deltaSINRToTPCMapForPUSCH_p->deltaSINRToTPCMapForPUSCHInfo[count]);

        startIndex = deltaSINRToTPCMapForPUSCHInfo_p->startDeltaSINR + MAC_MAX_SINR;
        endIndex   = deltaSINRToTPCMapForPUSCHInfo_p->endDeltaSINR + MAC_MAX_SINR;

        for (index =  startIndex; index <= endIndex; index++)
        {
            schedulerParams_p->macDeltaSINRToTPCMapForPUSCH[index] = 
                deltaSINRToTPCMapForPUSCHInfo_p->puschTPC;
        }
    }
}

/*****************************************************************************
 * Function Name  : initTpcRntiContextForPusch
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function based on startTpcRntiPusch and endTpcRntiPusch
 *                   value initializes the tpc rnti pool and tpc rnti context
 *                   for pucch.
 *****************************************************************************/
void initTpcRntiContextForPusch (InternalCellIndex internalCellIndex)
{
    UInt8 tpcRntiIndex = 0;
    /* + SPR_11040 */
    UInt8 maxTpcRntiRange = 
        (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->tpcRntiRange.endTpcRntiPusch - 
         cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->tpcRntiRange.startTpcRntiPusch + 1);
    UInt16 rnti = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->tpcRntiRange.startTpcRntiPusch;
    TpcRntiContext * tpcRntiContext_p = PNULL;

    /* SPR 13088 Fix Start */
    GET_MEM_FROM_POOL(TPC_TRIGGER_QUEUE_FOR_PUSCH_TYPE, tpcTriggerQueueForPusch_gp[internalCellIndex], 
            sizeof(TPC_TRIGGER_QUEUE_FOR_PUSCH_TYPE), PNULL); 

    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        INIT_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(internalCellIndex);
    }
    else
    {
        circQInitAfterCellSetup(tpcTriggerQueueForPusch_gp[internalCellIndex],MAX_TPC_TRIGGER_NODES_SPLIT);
    }
    /* SPR 13088 Fix End */

    GET_MEM_FROM_POOL(LTE_SQUEUE,
            scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex],sizeof(LTE_SQUEUE), PNULL);

    sQueueInit (scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex]);

    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof(TPC_SCHED_FAIL_QUEUE_PUSCH_TYPE), 1);
    }
    GET_MEM_FROM_POOL(TPC_SCHED_FAIL_QUEUE_PUSCH_TYPE,
            tpcSchedFailQueuePusch_gp[internalCellIndex],sizeof(TPC_SCHED_FAIL_QUEUE_PUSCH_TYPE), PNULL);

    INIT_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(internalCellIndex)
#endif
        /*HD FDD Changes End*/

        /* Start 128 UE: Memory Changes */
        /*
         ** The memory pool for TpcRntiContext is allocated only once
         ** for complete TPC RNTI Range rather than allocate for one rnti
         ** tpc, therefore a number of block is 1.
         */
        /* The mem pool will be created only first time when cell was configured */ 
        if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
        {
            createMemPool((sizeof(TpcRntiContext) * maxTpcRntiRange), 1);
            /* End 128 UE: Memory Changes */
            createMemPool( sizeof(ScheduledFormat3Or3AQueueNode), maxTpcRntiRange);
        }

    /* - SPR_11040 */
    GET_MEM_FROM_POOL(
            TpcRntiContext,
            tpcPuschRntiContext_gp[internalCellIndex],
            (sizeof(TpcRntiContext) * maxTpcRntiRange),
            PNULL);

    /* Coverity CID 54514 Start */
    if(PNULL == tpcPuschRntiContext_gp[internalCellIndex])
    {
        ltePanic("GET_MEM_FROM_POOL failed %s[%d]",__FUNCTION__, __LINE__);
        return;
    }
    /* Coverity CID 54514 End */

    for( tpcRntiIndex = 0; tpcRntiIndex < maxTpcRntiRange; tpcRntiIndex++)
    {
        listInit( &((tpcPuschRntiContext_gp[internalCellIndex] + tpcRntiIndex)->triggeredUeList));
        /* Rel 5.3.1: Memory related changes start*/
        /* this list will not require any extar pool as triggered and fail list
         * will be exclusive to each other */
        listInit( &((tpcPuschRntiContext_gp[internalCellIndex] + tpcRntiIndex)->failTriggerUeList));
        /* Rel 5.3.1: Memory related changes end*/

        /* + SPR_11040 */
        /* The mem pool will be created only first time when cell was configured */ 
        if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
        {
            createMemPool(
                    sizeof(TriggeredUEListNode),
                    MAX_UE_SUPPORTED_FOR_DCI_3_OR_3A);
            createMemPool(
                    sizeof(ConfiguredUEListNode),
                    MAX_UE_SUPPORTED_FOR_DCI_3_OR_3A);
        }
        /* - SPR_11040 */

        /* DCI Format3A changes start*/
        listInit( &((tpcPuschRntiContext_gp[internalCellIndex] + tpcRntiIndex)->configuredUeList));
        /* DCI Format3A changes end*/

        tpcRntiContext_p = (tpcPuschRntiContext_gp[internalCellIndex] + tpcRntiIndex);

        tpcRntiContext_p->configuredCount = 0;
        tpcRntiContext_p->triggeredCount = 0;
        tpcRntiContext_p->tpcRntiIndex = tpcRntiIndex;
        tpcRntiContext_p->tpcRnti = rnti;
        tpcRntiContext_p->scheduledState = UNSET_SCHEDULED_STATE;

        rnti++;
    }
} /* initTpcRntiContextForPusch */
/* Rel 5.3.1: Memory related changes start*/
/*****************************************************************************
 * Function Name  : cleanupTpcRntiContextForPusch
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function cleans memory node allocated to TpcTriggerQueueNode 
 *                  These nodes should be deleted at cell stop/delete 
 *                   for pucch.
 *****************************************************************************/
void cleanupTpcRntiContextForPusch (InternalCellIndex internalCellIndex)
{
    UInt8 tpcRntiIndex = 0;
    TpcRntiContext * tpcRntiContext_p = PNULL;
    UInt16 loopIndex=0, count=0;
    TriggeredUEListNode *tNode_p = PNULL;
    ConfiguredUEListNode *cNode_p = PNULL;
    UInt8 maxTpcRntiRange = (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->tpcRntiRange.endTpcRntiPucch -
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->tpcRntiRange.startTpcRntiPucch + 1);

    UInt8 tpcTriggeredQueueCount = 0;
    TpcTriggerQueueNode *tpcTriggerQueueNode_p = PNULL;
    /* Memory cleanup changes start*/
    ScheduledFormat3Or3AQueueNode * scheduledFormat3Or3AQueueNode_p = PNULL;
    /*coverity 58652*/
#ifdef HD_FDD_CONFIG
    /*coverity 58652*/
    TpcTriggerQueueNode* failQnode_p = PNULL;
#endif
    /* Memory cleanup changes end*/

    tpcTriggeredQueueCount = COUNT_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(internalCellIndex);

    while (tpcTriggeredQueueCount)
    {
        DEQUEUE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(TpcTriggerQueueNode,(void *)&(tpcTriggerQueueNode_p),internalCellIndex);
        if(PNULL != tpcTriggerQueueNode_p)
        {
            FREE_MEM_NODE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(tpcTriggerQueueNode_p);
        }
        tpcTriggeredQueueCount--;
    }

    /* Memory cleanup changes start*/
    count = sQueueCount(scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex]);
    while(count)
    {
        scheduledFormat3Or3AQueueNode_p =
            (ScheduledFormat3Or3AQueueNode*)\
            popNode(scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex]);
        if (scheduledFormat3Or3AQueueNode_p)
        {
            freeMemPool(scheduledFormat3Or3AQueueNode_p);
        }
        count--;
    }
#ifdef HD_FDD_CONFIG
#ifdef DL_UL_SPLIT
    count = COUNT_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(internalCellIndex);
    while(count)
    {
        DEQUEUE_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(TpcTriggerQueueNode,(void *)&(failQnode_p),internalCellIndex);
        if(failQnode_p)
        {
            FREE_MEM_NODE_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(failQnode_p);
        }
        count--;

    }
#else
    count = sQueueCount(tpcSchedFailQueuePusch_gp[internalCellIndex]);
    while(count)
    {
        failQnode_p = (TpcTriggerQueueNode*)popNode(tpcSchedFailQueuePusch_gp
                [internalCellIndex]);
        if(failQnode_p)
        {
            freeMemPool(failQnode_p);
        }
        count--;

    }
#endif
#endif
    /* Memory cleanup changes end*/
    for( tpcRntiIndex = 0; tpcRntiIndex < maxTpcRntiRange; tpcRntiIndex++)
    {
        tpcRntiContext_p =
            (tpcPuschRntiContext_gp[internalCellIndex] + tpcRntiIndex);

        count = listCount(&(tpcRntiContext_p->configuredUeList));
        for ( loopIndex= 0; loopIndex < count; loopIndex++)
        {
            cNode_p =
                (ConfiguredUEListNode *)getFirstListNode(&(tpcRntiContext_p->configuredUeList));

            if (cNode_p)
            {
                listDeleteNode(&(tpcRntiContext_p->configuredUeList), &(cNode_p->configuredUeAnchor) );
                freeMemPool(cNode_p);
                cNode_p = PNULL;
            }
        }

        count = listCount(&(tpcRntiContext_p->triggeredUeList));
        for ( loopIndex= 0; loopIndex < count; loopIndex++)
        {
            tNode_p =
                (TriggeredUEListNode *)getFirstListNode(&(tpcRntiContext_p->triggeredUeList));

            if (tNode_p)
            {
                listDeleteNode(&(tpcRntiContext_p->triggeredUeList), &(tNode_p->triggeredUeAnchor));
                freeMemPool(tNode_p);
                tNode_p = PNULL;
            }
        }

        count = listCount(&(tpcRntiContext_p->failTriggerUeList));
        for ( loopIndex= 0; loopIndex < count; loopIndex++)
        {
            tNode_p =
                (TriggeredUEListNode *)getFirstListNode(&(tpcRntiContext_p->failTriggerUeList));
            if (tNode_p)
            {
                listDeleteNode(&(tpcRntiContext_p->failTriggerUeList), &(tNode_p->triggeredUeAnchor) );
                freeMemPool(tNode_p);
                tNode_p = PNULL;
            }
        }
    }
    return;
}

/*****************************************************************************
 * Function Name  : deInitTpcRntiContextForPusch
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function cleans memory allocated to TpcTriggerQueueNode 
 *                  This queue should be deinitilized at cell delete 
 *                  
 *****************************************************************************/
void deInitTpcRntiContextForPusch (InternalCellIndex internalCellIndex)
{
    QDEINIT_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(internalCellIndex);

#ifdef DL_UL_SPLIT
#ifdef HD_FDD_CONFIG
    QDEINIT_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(internalCellIndex);
#endif
#endif

    /* + SPR_11040 */
    if(tpcTriggerQueueForPusch_gp[internalCellIndex])
    {
        freeMemPool(tpcTriggerQueueForPusch_gp[internalCellIndex]);
        tpcTriggerQueueForPusch_gp[internalCellIndex] = PNULL;
    }

    if(scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex])
    {
        freeMemPool(scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex]);
        scheduledFormat3Or3AQueueForPusch_gp[internalCellIndex] = PNULL; 
    }

#ifdef HD_FDD_CONFIG
    if(tpcSchedFailQueuePusch_gp[internalCellIndex])
    {
        freeMemPool(tpcSchedFailQueuePusch_gp[internalCellIndex]);
        tpcSchedFailQueuePusch_gp[internalCellIndex] = PNULL;
    }
#endif
    /* - SPR_11040 */

    if(PNULL != tpcPuschRntiContext_gp[internalCellIndex])
    {
        freeMemPool(tpcPuschRntiContext_gp[internalCellIndex]);
        tpcPuschRntiContext_gp[internalCellIndex] = PNULL;
    }

    return;
}
/* Rel 5.3.1: Memory related changes end*/
/*****************************************************************************
 * Function Name  : calculatePathlossFromPHR 
 * Inputs         : pcMax,alpha,p0PUSCH,powerControlInfo,pwrHeadRoom,fI 
 * Outputs        : None
 * Returns        : Path loss
 * Description    : This function calucalte the path loss from PHR value
 *****************************************************************************/
 SInt16 calculatePathlossFromPHR(UInt16 pcMax, 
        UInt8 alpha,
        SInt8 p0PUSCH,                                
        SInt8  powerControlInfo, 
        SInt8  pwrHeadRoom,/*SPR_PH_MAPPING*/ 
        SInt8  fI   )
{
    /* alpha = 0, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1 expressed as 0,4,5,6,7,8,9,10 respectively */
    return ((pcMax - p0PUSCH - powerControlInfo - pwrHeadRoom - fI)*10/(alpha));
}

/*CA Changes start */
/*****************************************************************************
 * Function Name  : initPUSCHPowerControl
 * Inputs         :  - number Of Cells to be configured
 * Outputs        :
 * Returns        : NONE
 * Description    : This function is used to initialize the global objects of PUSCHPowerControl 
 *                  module. This is called at MAC_INIT_LAYER_REQ
 ******************************************************************************/
void initPUSCHPowerControl(UInt8 numOfCells)
{
    /* + SPR 11345 */
    InternalCellIndex internalCellIndex = 0;
    for(internalCellIndex = 0; internalCellIndex < numOfCells; internalCellIndex++)
    {
        minMCSIndexForATB_g[internalCellIndex] = MAX_MCS_THRESHOLD_FOR_ATB;
        minPRBValForATB_g[internalCellIndex] = MAX_RB_THRESHOLD_FOR_ATB;
    }
    /* - SPR 11345 */

    /* + SPR_11040 */
    createMemPool(sizeof(TPC_SCHED_FAIL_QUEUE_PUSCH_TYPE), numOfCells);
    createMemPool(sizeof(TPC_TRIGGER_QUEUE_FOR_PUSCH_TYPE), numOfCells);
    createMemPool(sizeof(LTE_SQUEUE), numOfCells);

    return;
    /* - SPR_11040 */
}
/*CA Changes end */
/*CYCLOMATIC_CHANGES_6.1_START*/
/****************************************************************************
 * * Function Name  : calculatePreferredMcsRBForATB
 * * Inputs         : mcsloopCount,rbloopCount,dbloopCount,
 * *                  deltaTfVal,tenLogMAllocatedVal
 * * Returns        : None
 * * Outputs        : Populates macParams_g with preferred mcs and rb for ATB
 * * Description    : This function populates the MCS and RB.
 *****************************************************************************/
STATIC  void calculatePreferredMcsRBForATB(
        UInt8 mcsloopCount,
        UInt8 rbloopCount,
        UInt8 dbloopCount,
        SInt8 deltaTfVal,
        UInt8 tenLogMAllocatedVal)
{
    UInt8 rbLimit = 0;
    UInt8 mcsLimit = 0;
    UInt8 dbRdctnMcs = 0;
    UInt8 dbRdctnForRB = 0;
    UInt32 tbSizeMcsRbValidation = 0;
    UInt32 maxTBSForMcsRbValidation = 0;
    UInt32 tbSizeMCSValidated = 0;
    UInt32 tbSizeRBValidated = 0;
    UInt8  mcsValForMcsValidation = 0;
    UInt8  rbValForRbValidation = 0;
    UInt8  mcsValForMcsRbValidation = 0;
    UInt8  rbValForMcsRbValidation = 0;

    rbLimit = rbloopCount;
    mcsLimit = mcsloopCount;

    macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1]
        [dbloopCount-1].preferredMcsForATB = INVALID_MCS_VAL_FOR_ATB;
    macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1]
        [dbloopCount-1].preferredRbForATB = INVALID_RB_VAL_FOR_ATB;

    if ( (deltaTfVal - dbloopCount + DELTA_TF_NORMALIZED_DB_VAL) >= 0 )
    {
        mcsLimit = deltaTfToMCS_g[deltaTfVal - dbloopCount + DELTA_TF_NORMALIZED_DB_VAL];
        tbSizeMCSValidated = determineUlTBSize(mcsLimit,rbloopCount);
        mcsValForMcsValidation = mcsLimit;
    }

    if ( (tenLogMAllocatedVal - dbloopCount) >= 0 )
    {
        rbLimit = tenLogMFactorToPrb_g[tenLogMAllocatedVal-dbloopCount];
        tbSizeRBValidated = determineUlTBSize(mcsloopCount,rbLimit);
        rbValForRbValidation = rbLimit;
    }


    for ( dbRdctnMcs = 1;
            dbRdctnMcs < dbloopCount;
            dbRdctnMcs ++)
    {
        if ( (deltaTfVal - dbRdctnMcs + DELTA_TF_NORMALIZED_DB_VAL) >= 0 )
        {
            mcsLimit = deltaTfToMCS_g[deltaTfVal - dbRdctnMcs + DELTA_TF_NORMALIZED_DB_VAL];
        }
        else
        {
            break;
        }
        dbRdctnForRB = dbloopCount - dbRdctnMcs;

        if ( (tenLogMAllocatedVal - dbRdctnForRB ) >= 0 )
        {
            rbLimit = tenLogMFactorToPrb_g[tenLogMAllocatedVal - dbRdctnForRB];
        }
        else
        {
            continue;
        }
        tbSizeMcsRbValidation = determineUlTBSize(mcsLimit,rbLimit);
        if ( tbSizeMcsRbValidation > maxTBSForMcsRbValidation )
        {
            maxTBSForMcsRbValidation = tbSizeMcsRbValidation;
            mcsValForMcsRbValidation = mcsLimit;
            rbValForMcsRbValidation  = rbLimit;
        }
    }

    tbSizeMcsRbValidation = maxTBSForMcsRbValidation;

    if ( tbSizeMcsRbValidation ||
            tbSizeMCSValidated  ||
            tbSizeRBValidated )
    {
        if ( tbSizeMcsRbValidation > tbSizeMCSValidated )
        {
            if ( tbSizeMcsRbValidation > tbSizeRBValidated )
            {
                /* MCS,RB is largest */
                macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1]
                    [dbloopCount-1].preferredMcsForATB = mcsValForMcsRbValidation;
                macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1]
                    [dbloopCount-1].preferredRbForATB = rbValForMcsRbValidation;

            }
            else
            {
                /* RB largest*/
                macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1]
                    [dbloopCount-1].preferredMcsForATB = mcsloopCount;
                macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1]
                    [dbloopCount-1].preferredRbForATB = rbValForRbValidation;
            }
        }
        else
        {
            if ( tbSizeMCSValidated > tbSizeRBValidated )
            {
                /* MCs is largest*/
                macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1]
                    [dbloopCount-1].preferredMcsForATB = mcsValForMcsValidation;
                macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1]
                    [dbloopCount-1].preferredRbForATB = rbloopCount;
            }
            else
            {
                /* RB is largest*/
                macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1]
                    [dbloopCount-1].preferredMcsForATB = mcsloopCount;
                macParams_g.preferredMcsRbForATBTbl[mcsloopCount][rbloopCount-1]
                    [dbloopCount-1].preferredRbForATB = rbValForRbValidation;
            }
        }
    }
}
/*CYCLOMATIC_CHANGES_6.1_END*/

