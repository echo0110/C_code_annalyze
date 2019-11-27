/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPucchPowerControl.h
 *
 ****************************************************************************
 *
 *  File Description : This header file contains declarations required for 
 *                     PUCCH Power Control feature in LTE MAC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * Revision 1.1.1  2012/07/20 14:55:27  gur31292
 * Changes related to DCI Format3A
 ****************************************************************************/

#ifndef LTE_MAC_PUCCH_POWER_CONTROL_H
#define LTE_MAC_PUCCH_POWER_CONTROL_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacDLUEContext.h"
#include "lteMacPowerControl.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacULPowerControlData.h"
#include "lteMacDLPowerControlData.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/****************************************************************************
 * Exported Types
 ****************************************************************************/
/****************************************************************************
 * Exported Constants
 ****************************************************************************/
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt8 pucchBlerUnsureWeightage_g[MAX_NUM_CELL];
extern UInt8 pucchBlerSureWeightage_g[MAX_NUM_CELL];


/* ICIC changes start */
extern UInt16 pucchUciUlschMap_g[MAX_UE_SUPPORTED];
/* ICIC changes end */
/*SPR 3856 Fix Start*/
/* IIR Coefficient to calculate time moving average SINR in case of PUCCH. Value represents the
 * weight given to last average SINR out of 1024. Value range is from 0 to 1024
 */
#define MAC_ALPHA_PUCCH_COEFFICIENT_IIR 820
/*SPR 3856 Fix End*/

#define MAX_NUM_OF_PUCCH_BLER_SAMPLES     10

/* Bler changes start */
#define MIN_PUCCH_SINR_CORRECTION_FACTOR    -255
#define MAX_PUCCH_SINR_CORRECTION_FACTOR    255
/* Bler changes end */
#define MIN_EFFECTIVE_PUCCH_SINR 50
#define MAX_EFFECTIVE_PUCCH_SINR 200

/* Macro to check and update the tpc triggered value based on CQI SINR */
/* Time Averaging changes start */ \
#define CHECK_AND_UPDATE_CQI_SINR(dlUEContext_p, sinr,                      \
        subFrameNum, currentTTI, internalCellIndex)                                            \
/* Time Averaging changes end */ \
{                                                                           \
    /* SPR 5033 changes start*/\
    if ((0!= (pucchUciUlschMap_g[dlUEContext_p->ueIndex] &  \
            bitmask[subFrameNum]))) \
   /* SPR 5033 changes end*/\
    {                                                               \
        /* Time Averaging changes start */ \
        updateAndCheckCqiSinr(dlUEContext_p, sinr, currentTTI,internalCellIndex);     \
        /* Time Averaging changes end */ \
    }                                                               \
}

/* MACRO to check if the HARQ BLER is received on PUCCH or not */
#define CHECK_IS_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNumber,       \
                isHarqOnPucch)                                              \
{                                                                           \
    isHarqOnPucch =                                                         \
   (0!= (pucchUciUlschMap_g[ueIndex] & bitmask[subFrameNumber]))?TRUE:FALSE; \
}

/* Set the pucchUciUlschMap_g[ueIndex] bitmask to indicate that the 
 * Harq BLER is to be received on the PUCCH in that particular tick */
#define SET_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNum)            \
{                                                                           \
    pucchUciUlschMap_g[ueIndex] |= bitmask[subFrameNum];                     \
}

/* Reset the pucchUciUlschMap_g[ueIndex] bitmask */
#define RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNum)          \
{                                                                           \
    pucchUciUlschMap_g[ueIndex] =                                           \
        pucchUciUlschMap_g[ueIndex] & resetBitmask[subFrameNum];            \
}

// Macro to reset the blerCalculated and blerCalculatedCounter to 0 
// It will be called whenever the lastAppliedTick is Updated
// Case : If blerCalculatedCounter is 5 and blerCalculated = 55
// and TPC is triggered due to CQI SINR, Then in this case when 
// lastAppliedTick >= currentTick TPC will start calculating from 
// blerCalculatedCounter value 5 and blerCalculated 55
// (which considers reports, before TPC applied )
// However it should ideally be w.r.t reports received after latest 
// TPC applied i.e. from blerCalculatedCounter = 5. 
// So to avoid such scenario it should always be reset to 0
// Also reset the coolOff Counters of HARQ BLER and SINR so that
// no one can trigger the TPC immediately after another.
#define RESET_COOLOFFS_AND_BLER_CALCULATE_COUNTER(dlUEContext_p)                  \
{                                                                    \
    dlUEContext_p->blerInfoForPUCCH.blerCalculatedCounter = 0;       \
    dlUEContext_p->blerInfoForPUCCH.blerCalculated = 0;              \
    dlUEContext_p->blerInfoForPUCCH.lastBlerCoolOffCounter = 0;      \
    dlUEContext_p->lastSINRCoolOffCounter = 0;        \
}

/* Bler changes start */ 
#define VALIDATE_PUSCH_SINR_CORRECTION_VALUE(sinrCorrectionFactor) \
{\
    if(sinrCorrectionFactor < MIN_PUSCH_SINR_CORRECTION_FACTOR)  \
    {\
       sinrCorrectionFactor = MIN_PUSCH_SINR_CORRECTION_FACTOR; \
    }\
    else if(sinrCorrectionFactor > MAX_PUSCH_SINR_CORRECTION_FACTOR)  \
    {\
       sinrCorrectionFactor =  MAX_PUSCH_SINR_CORRECTION_FACTOR;\
    }\
}
/* Bler changes end */ 

/* This Macro Check if the PUCCH CLPC BLER calculation is enabled or not           
If enabled then check if the cool off period is expired              
then check the BLER for PUCCH and trigger TPC for PUCCH if required 
*/                                                                     
/*SPR_4001_CHANGE_START*/       
#define UPDATE_AND_CHECK_HARQ_BLER_FOR_PUCCH(dlUEContext_p, oprTb, currentTick, internalCellIndex)  \
{                                                                           \
    SInt8 tpcValue = TPC_NO_CHANGE;                                         \
    if(dlUEContext_p->tpcInfoForPucch.lastAppliedTick <= currentTick)      \
    {                                                                        \
        if (oprTb > 2) /*it means that operation > 2 */                          \
        {                                                                        \
            /* Operation is other than ACK/NACK */                               \
            dlUEContext_p->blerInfoForPUCCH.blerCalculated                       \
            += PUCCH_BLER_UNSURE_STEP_SIZE(internalCellIndex); \
        }                                                                        \
        else                                                                     \
        {                                                                        \
            /* Its an ACK or NACK, decrement the bler by PUCCH_BLER_STEP_SIZE */ \
            dlUEContext_p->blerInfoForPUCCH.blerCalculated -= PUCCH_BLER_SURE_STEP_SIZE(internalCellIndex);\
        }                                                                        \
        \
        dlUEContext_p->blerInfoForPUCCH.blerCalculatedCounter++;                \
        \
        if (dlUEContext_p->blerInfoForPUCCH.blerCalculatedCounter                \
               >= MAX_NUM_OF_PUCCH_BLER_SAMPLES)                                      \
        {                                                                        \
            dlUEContext_p->blerInfoForPUCCH.pucchBler =                      \
            (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
            deltaBlerToTPCMapForPUCCH.pucchBLERTarget +                \
            /* SPR 3530 START */ \
            (SInt8)(dlUEContext_p->blerInfoForPUCCH.blerCalculated/             \
                   /* SPR 3530 END */ \
                    MAX_NUM_OF_PUCCH_BLER_SAMPLES);  \
            if (dlUEContext_p->blerInfoForPUCCH.pucchBler < 1)               \
            {                                                                \
                dlUEContext_p->blerInfoForPUCCH.pucchBler = 0;               \
            }                                                                \
            else if (dlUEContext_p->blerInfoForPUCCH.pucchBler > 100)        \
            {                                                                \
                dlUEContext_p->blerInfoForPUCCH.pucchBler = 100;             \
            }                                                                \
            dlUEContext_p->blerInfoForPUCCH.blerCalculatedCounter = 0;       \
            dlUEContext_p->blerInfoForPUCCH.blerCalculated = 0;              \
    /* SPR 5033 changes start */\
            updatePucchBler(dlUEContext_p->ueIndex, \
                         dlUEContext_p->blerInfoForPUCCH.pucchBler,internalCellIndex);\
    /* SPR 5033 changes end */\
        }\
    }\
    /* SPR 5033 changes start*/\
    if ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
        cellParams_p->powerControlEnableInfo.harqBlerClpcPucchEnable) &&\
       (dlUEContext_p->tpcInfoForPucch.lastAppliedTick <= currentTick)&&\
            (dlUEContext_p->blerInfoForPUCCH.lastBlerCoolOffCounter++ >=  \
                (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->coolOffPeriodForBLERTriggeredPUCCHTPC ))                 \
    /* SPR 5033 changes end*/\
    {                                                                      \
        tpcValue = checkTPCForPUCCHHarqBler(                               \
                dlUEContext_p->blerInfoForPUCCH.pucchBler,                  \
                internalCellIndex);                                        \
        if ((tpcValue != TPC_NO_CHANGE) &&                                 \
                (dlUEContext_p->tpcInfoForPucch.tpcTriggeredValue == TPC_NO_CHANGE))\
        {                                                                   \
            addEntryToTpcTriggerQueueForPucch(tpcValue, dlUEContext_p->ueIndex,\
                   &(dlUEContext_p->tpcInfoForPucch),                      \
                    currentTick, internalCellIndex);                        \
        }                                                                   \
        \
       /* Reset the blerInfoForPUCCH.lastBlerCoolOffCounter to 0 */     \
       dlUEContext_p->blerInfoForPUCCH.lastBlerCoolOffCounter = 0;      \
    }\
/*SPR_4001_CHANGE_END*/\
    LOG_MSG(HARQ_BLER_CALC_PUCCH_ID, LOGDEBUG, MAC_UL_PC,                   \
            currentTick,                                                    \
            dlUEContext_p->ueIndex,                                         \
            dlUEContext_p->blerInfoForPUCCH.pucchBler,                      \
            tpcValue,                                                       \
            DEFAULT_INT_VALUE,                                              \
            DEFAULT_INT_VALUE,                                              \
            DEFAULT_FLOAT_VALUE,                                            \
            DEFAULT_FLOAT_VALUE,                                            \
            FUNCTION_NAME,                                                  \
            "CHECK_HARQ_BLER_FOR_PUCCH");                                   \
}                                                                           



/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* CLPC_CHG */

/* This function shall process the tpcTriggerQueueForPucch. For each entry 
   present in Queue, it shall trigger TPC commands for PUCCH power control, 
   either using DCI format 3 /3A for the UE if TPC-RNTI is configured else 
   using explicit DCI Format 1x /2x */
void triggerDciForPucchPowerControl(
        /* SPR 15909 fix start */
        tickType_t currentTick, 
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);


/* This function shall return the appropriate TPC value that can be sent in 
 * DCI Format 1x/2x by taking into considerations different triggers due to
 * power control conditions */
TpcTriggeredToPowerCorrection getTPCForPUCCH(
        DLUEContext *dlUeContext_p,
        /* SPR 15909 fix start */
        tickType_t currentTick, 
        /* SPR 15909 fix end */
        UInt8 dciFormat,
        InternalCellIndex internalCellIndex);

/*  This function shall add the entry of UE in tpcTriggerQueueForPucch after 
 *  the wideband power control for PUCCH is triggered.*/
void addEntryToTpcTriggerQueueForPucch (
        SInt8 tpcTriggered, 
        UInt16 ueIdx,
        TpcInfoForPucch *tpcInfoForPucch_p,
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);

/* This fucntion shall encode the tpc bitmap for PUCCH power control depending
 * upon the DCI FORMAT 3/3A scheduled fro the TPC PUCCH RNTI */
void encodeTpcBitmapForPucch(UInt32 count,
        TpcMsgInfoForPucch * tpcMsgInfoPucch_p,
        DciCCEContainerInfo* dciCCEContainerInfo_p,
        /* SPR 15909 fix start */
        tickType_t tick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);

/* Bler changes start */
/* SPR 15909 fix start */
void updateAndCheckCqiSinr(DLUEContext* dlUeContext_p, SInt32 sinr, tickType_t currentTick,
/* SPR 15909 fix end */        
                           InternalCellIndex internalCellIndex);
/* Bler changes end */ 

TriggeredUEListNode* addEntryInTriggeredUeList(UInt16 ueIndex,
                               LTE_LIST * triggeredUeList
/*#ifdef HD_FDD_CONFIG*/                               
                               ,TriggeredUEListNode* node_p
/*#endif*/
                               );

ConfiguredUEListNode* addEntryInConfiguredUeList(UInt16 ueIndex, 
                               LTE_LIST * configuredUeList);

void putEntryInTpcTriggeredQueueForPucch(UInt16 ueIndex, 
                                         /* SPR 15909 fix start */
                                         tickType_t expiryTick,
                                          /* SPR 15909 fix end */
                                         InternalCellIndex internalCellIndex);

void putEntryInScheduledFormat3Or3AQueueForPucch(UInt16 tpcRntiIndex,
                                         InternalCellIndex internalCellIndex);

void initTpcRntiContextForPucch(InternalCellIndex internalCellIndex );
/* Rel 5.3.1: Memory related changes start*/
void cleanupTpcRntiContextForPucch (InternalCellIndex internalCellIndex );
void deInitTpcRntiContextForPucch(InternalCellIndex internalCellIndex);
/* Rel 5.3.1: Memory related changes end*/
void initHarqBlerToTPCMap(
         DeltaBLERToTPCMapForPUCCH * deltaBLERToTPCMapForPUCCH_p,
         MacReconfigSchedulerParamsDL *schedulerParams_p);

void initSinrToTpcMap(DeltaSINRToTPCMapForPUCCH * deltaSINRToTPCMapForPUCCH_p,
        MacReconfigSchedulerParamsDL *schedulerParams_p);

/* + SPR 22354 */
/* This function checks in the TPC configured list, if the UE is Victim or
 * Non-Victim */
MacRetType macCheckIfAnyVictimUe( TpcRntiContext *tpcRntiContext_p );
/* - SPR 22354 */
#endif /*LTE_MAC_PUCCH_POWER_CONTROL_H*/
