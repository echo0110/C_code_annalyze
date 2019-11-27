/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPuschPowerControl.h
 *
 ****************************************************************************
 *
 *  File Description : This header file contains declarations required for 
 *                     PUSCH Power Control feature in LTE MAC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * Revision 1.1.1  2012/07/20 14:56:27  gur31292
 * Changes related to DCI Format3A
 ****************************************************************************/

#ifndef LTE_MAC_PUSCH_POWER_CONTROL_H
#define LTE_MAC_PUSCH_POWER_CONTROL_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacULUEContext.h"
#include "lteMacPowerControl.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacULResourceAllocator.h"
#include "lteMacULCSIManager.h"
#include "lteMacULPowerControlData.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_TPC_TRIGGER_NODES_SPLIT MAX_UE_SUPPORTED
/* CLPC_CHG */
/* The maximum possible MCS index in UL direction */
#define MAX_MCS_UL 28
#define MIN_POWER_CORRECTION_FOR_PUSCH      -63
#define MAX_POWER_CORRECTION_FOR_PUSCH      63
#define MAX_NUM_OF_PUSCH_BLER_SAMPLES     100
/* SPR 7981 changes start */
#define MIN_PUSCH_SINR_CORRECTION_FACTOR          (-20 * UL_RESOLUTION_FACTOR)
#define MAX_PUSCH_SINR_CORRECTION_FACTOR          (20 * UL_RESOLUTION_FACTOR)
/* SPR 7981 changes end */
/* represents no change */
#define INVALID_TPC_RNTI 0


//Macro to rangebound the currPowerCorrectionForPusch it should lie between -63 to 63
#define CHECK_AND_UPDATE_POWER_CORRECTION_FOR_PUSCH(powerCorrectionValue)\
{\
    if(MIN_POWER_CORRECTION_FOR_PUSCH > powerCorrectionValue) \
    {\
        powerCorrectionValue = MIN_POWER_CORRECTION_FOR_PUSCH;\
    }\
    else if(MAX_POWER_CORRECTION_FOR_PUSCH < powerCorrectionValue)\
    {\
        powerCorrectionValue = MAX_POWER_CORRECTION_FOR_PUSCH;\
    } \
}

#define CHECK_AND_UPDATE_FI_CORRECTION(ueMaxPowerFlag,ueMinPowerFlag,powerCorrectionAndTpc,ulUEContext_p)\
{\
    if (   ( ueMaxPowerFlag == FALSE  &&\
             ueMinPowerFlag == FALSE  ) || \
           ( ueMinPowerFlag == TRUE   && \
             powerCorrectionAndTpc.powerCorrectionVal > 0  ) || \
           ( ueMaxPowerFlag == TRUE   && \
             powerCorrectionAndTpc.powerCorrectionVal < 0  )  \
       )\
    {\
        if (ulUEContext_p->dedicatedULPowerControlInfo.\
                accumulationEnabled)\
        {\
            ulUEContext_p->currPowerCorrectionForPusch +=\
                powerCorrectionAndTpc.powerCorrectionVal;\
            CHECK_AND_UPDATE_POWER_CORRECTION_FOR_PUSCH(\
                    ulUEContext_p->currPowerCorrectionForPusch);\
        }\
        else\
        {\
            ulUEContext_p->currPowerCorrectionForPusch =\
                powerCorrectionAndTpc.powerCorrectionVal;\
        }\
        ulUEContext_p->powerControlPuschTPCValue =\
                       powerCorrectionAndTpc.powerCorrectionVal;\
    }\
}


#define CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ulUEContext_p,regionPowerMask)\
{\
    SInt32 pcMax = 0;\
    SInt16 count = 0;\
    UInt8 lcgId = LCGID_INVALID_VALUE;\
    pcMax = ulUEContext_p->pcMax;\
    for( count  = ulUEContext_p->qciDeltaPowerMaskInfo.numOfQciInfo - 1;\
         count >= 0 ; \
         count--)\
    {\
        lcgId =  ulUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[count].lcgId;\
        if ( lcgId != LCGID_INVALID_VALUE )\
        {\
            if(ulUEContext_p->bsrNet[lcgId] != 0)\
            {\
                /* for updating stats */\
                ulUEContext_p->maxQciDeltaPowerMask = ulUEContext_p->qciDeltaPowerMaskInfo.\
                                     deltaPowerMaskInfo[count].maxQciDeltaPowerMask;\
                regionPowerMask += ulUEContext_p->qciDeltaPowerMaskInfo.\
                             deltaPowerMaskInfo[count].maxQciDeltaPowerMask ;\
                break;\
            }\
        }\
    }\
    if(pcMax < regionPowerMask)\
    {\
        regionPowerMask = pcMax;\
    }\
}



// Macro to reset the lastTpcSinrCoolOffCounter 0
// It will be called whenever the lastAppliedTick is Updated so that
// the reports are discarded before new TPC is applied.
#define RESET_PUSCH_TPC_COOLOFF_COUNTER(ulUeContext_p)\
{\
    ulUeContext_p->tpcInfoForPusch.lastTpcSinrCoolOffCounter = 0;\
} 

/* This shall be used to check and trigger TPC if it can be triggered based 
   on average SINR reported by the UE. Wideband power control for PUSCH shall 
   be triggered if subbandCLPCEnabled is false or if subbandCLPCEnabled is 
   true and queueload for the UE is zero and SRS is configured */
#define CHECK_TPC_FOR_WIDEBAND_SINR(\
        ulUeContext_p,\
        pathLoss, \
        averageSinr, \
        currentTick, \
        /*CA Changes start  */ \
        internalCellIndex) \
        /*CA Changes end  */ \
{\
    /* SPR 4609_4610 Start */ \
    SInt8 localTpcTriggered = TPC_NO_CHANGE;\
    /* SPR 4609_4610 End */ \
    if (ulUeContext_p->tpcInfoForPusch.lastAppliedTick <= currentTick) \
    {   \
        /*CA Changes start  */ \
        if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->powerControlEnableInfo.clpcPuschEnable)\
        /*CA Changes end  */ \
        {\
            /*CA Changes start  */ \
            if( (!cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->powerControlEnableInfo.clpcPuschfreqSelectiveEnable) ||\
            /*CA Changes end  */ \
                    (!ulUEContext_p->bsrNetTotal && \
                     ulUEContext_p->srsConfigInfo.srsConfigType == \
                     SRS_DEDICATED_CONFIG_TYPE_SETUP))\
            {\
                if( ulUEContext_p->tpcInfoForPusch.lastTpcSinrCoolOffCounter++ \
                        /*CA Changes start  */ \
                        >= (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->coolOffPeriodForPUSCHTPC )\
                        /*CA Changes end  */ \
                {\
                    /* Reset the Counter since cooloff expired */ \
                    ulUEContext_p->tpcInfoForPusch.lastTpcSinrCoolOffCounter = 0; \
                    if (ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue == \
                            TPC_NO_CHANGE)\
                    {\
                        if(SPS_ACTIVATED == ulUEContext_p->spsUlInfo.spsState) \
                        {\
                            localTpcTriggered = checkTPCForPUSCHWidebandSINRForSps(pathLoss,averageSinr, \
                                                         /*CA Changes start  */ \
                                                         internalCellIndex);\
                                                         /*CA Changes end  */ \
                        } \
                        else \
                        {\
                            localTpcTriggered = checkTPCForPUSCHWidebandSINR(pathLoss,averageSinr,\
                                                         /*CA Changes start  */ \
                                                         internalCellIndex);\
                                                         /*CA Changes end  */ \
                        }\
                        /* SPR 4609_4610 Start */ \
                        if ( localTpcTriggered != TPC_NO_CHANGE )  \
                        { \
                            if ( !( ( ( ulUEContext_p->tpcInfoForPusch.maxPowerReachedFlag == TRUE )&& \
                                            ( localTpcTriggered > 0 ) ) ||  \
                                        ( ( ulUEContext_p->tpcInfoForPusch.minPowerReachedFlag == TRUE )&& \
                                          ( localTpcTriggered < 0 ) )  \
                                  )\
                               ) \
                            /* SPR 4609_4610 End */ \
                            {\
                                addEntryToTpcTriggerQueueForPusch (\
                                        ulUEContext_p->ueIndex,\
                                        localTpcTriggered,\
                                        &ulUEContext_p->tpcInfoForPusch,\
                                        currentTick, \
                                        /*CA Changes start  */ \
                                        internalCellIndex);\
                                        /*CA Changes end  */ \
                            }\
                        }\
                    }\
                }\
            }\
        }\
    }\
}
           
/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* This structure will be populated at initialization phase and will provide a 
 * direct indexing of PRB number to SINR Group Index. This shall also indicate
 * the PRB number of the start of next group which will be used in calculating 
 * the number of PRBs in the partially filled groups */
typedef struct PRBGroupInfoT
{
    /* Indicate groupIndex corresponding to a PRB number */
    UInt8 groupIndex;

    /* Indicate the PRB number of the next group start */
    UInt8 nextGroupStartRb;

}PRBGroupInfo;
/* CLPC_CHG END */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt8 tenLogMFactor_g[MAX_NUM_RBS];
extern SInt8 mcsToDeltaTF_g[MAX_MCS_INDEX];
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt8 validatePRBForATB(
                 UInt32       *ueMaxPowerFlag,
                 SInt8        deltaTF,
                 SInt16       tenLogMAllocated,
                 UInt8        mAllocated, 
                 SInt16       puschPowerControlFactor,
                 SInt32       regionPowerMask,
                 InternalCellIndex internalCellIndex);
/* + SPR 17439 */
void populatePreferredMcsRbTableForATB(void);
/* - SPR 17439 */
UInt8 validateNprbFromATB(
        UInt8 inputRBs,
        /* Time Averaging changes start */
        UInt8 mcs,
        ULUEContext *ulUEContext_p,
        SInt8 tpc,
        UInt32 maxRBRestrict,
        UInt32* ueMaxPowerFlag,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex);
        /* Time Averaging changes end */


UInt8 validateATB(
        UInt8       inputRBs, 
        UInt8       *mcs, 
        ULUEContext *ulUEContext_p,
        SInt8       tpc,
        UInt32      *ueMaxPowerFlag,
        UInt32      *minUePowerFlag,
        SInt32      regionPowerMask,
        UInt8       txType,
        /*CA Changes start */
        InternalCellIndex internalCellIndex);
        /*CA Changes end */


MacRetType validateNprbFromATBForSps(
        UInt8 inputRBs,
        UInt8 mcs,
        ULUEContext *ulUEContext_p,
        SInt8 tpc,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
        /*CA Changes end  */

/* CLPC_CHG */
/*SPR_PH_MAPPING_START*/
/* SPR 5599 changes start (ZIP ID 129085) */
 SInt16 calculatePathlossFromPHR(UInt16 pcMax,UInt8 alpha, SInt8 p0PUSCH,
/* SPR 5599 changes end (ZIP ID 129085) */
        SInt8  powerControlInfo, SInt8  pwrHeadRoom, SInt8  fI);
/*SPR_PH_MAPPING_END*/

/* This function shall process the tpcTriggerQueueForPusch. For each entry
 * present in Queue, it shall trigger TPC commands for PUSCH power control,
 * either using DCI format 3 /3A for the UE if TPC-RNTI is configured else
 * using explicit DCI Format 0. */
/* SPR 15909 fix start */
void  triggerDciForPuschPowerControl(tickType_t currentTick, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */

/* This function shall return the appropriate TPC value that can be sent in 
 * DCI Format 0 by taking into considerations different triggers due to 
 * wideband and sub band power control conditions */
TpcTriggeredToPowerCorrection getTpcForPusch (
        ULUEContext *ulUeContext_p,
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        /*CA Changes Start*/
        InternalCellIndex internalCellIndex
        /*CA Changes End*/
        /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG        
        ,UInt8 ulSubFrameNum
#endif        
        /*TDD Config 0 Changes End*/
        );

/*  This function shall add the entry of UE in tpcTriggerQueueForPusch after 
 *  the wideband power control for PUSCH is triggered.*/
void addEntryToTpcTriggerQueueForPusch (
        UInt16 ueIndex, 
        SInt8 tpcTriggered,
        TpcInfoForPusch *tpcInfoForPusch_p,
        /* SPR 15909 fix start */
        tickType_t currentTick,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
        /*CA Changes end  */

/* This fucntion shall encode the tpc bitmap for PUSCH power control depending
 * upon the DCI FORMAT 3/3A scheduled fro the TPC PUSCH RNTI */
void encodeTpcBitmapForPusch(
        UInt8 containerIndex,
        PdcchTpcMsgInfoForPusch *pdcchTpcMsgInfoForPusch_p,
        /* SPR 15909 fix start */
        tickType_t  currentTick,
        /* SPR 15909 fix end */
        /*CA Changes start */
        InternalCellIndex internalCellIndex
        /*CA Changes end */
        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG        
        ,UInt8 ulSubFrameNum
#endif        
        /*TDD Config 0 Changes End*/ 
        );

/* It deletes a UEs entry from triggered UE List maintained in TPC RNTI 
   Context if UE is scheduled for TPC using DCI Format 3/3A. */
void deleteEntryFromTriggeredUeList(
        TriggeredUEListNode* ueListNodeInTpcRntiContext_p,
        TpcRntiContext * tpcRntiContext_p);

/* It deletes a UEs entry from configured UE List maintained in TPC RNTI 
   Context if UE is scheduled for TPC using DCI Format 3A. */
void deleteEntryFromConfiguredUeList(
        ConfiguredUEListNode* ueListNodeInTpcRntiContext_p,
        TpcRntiContext * tpcRntiContext_p);

/* This function shall add the entry of TPC RNTI  to the 
   scheduledFormat3Or3AQueueForPusch_g */
void putEntryOfTpcIndexToScheduledQueueForPusch(UInt16 tpcRntiIndex,
                               InternalCellIndex internalCellIndex);

void initTpcRntiContextForPusch(InternalCellIndex internalCellIndex);
void cleanupTpcRntiContextForPusch(InternalCellIndex internalCellIndex);
void deInitTpcRntiContextForPusch(InternalCellIndex internalCellIndex);
void populateSinrToTpcMapForPUSCH(DeltaSINRToTPCMapForPUSCH 
                                    *deltaSINRToTPCMapForPUSCH_p,
                                    MacReconfigSchedulerParamsUL *schedulerParams_p);

void populatePathlossToTargetSINRMapForPUSCH(AlphaBasedPathLossToTargetSINRMap 
                *alphaBasedPathLossToTargetSINRMap_p,
                MacReconfigSchedulerParamsUL *schedulerParams_p,
                UInt8 sps_flag);

extern TriggeredUEListNode* addEntryInTriggeredUeList(UInt16 ueIndex,
        LTE_LIST * triggeredUeList
        ,TriggeredUEListNode* node_p
        );

/*CA Changes start  */
void putEntryInExplicitULDCIAndPHRScheduleQueue(UInt16 ueIndex, ULGrantRequestType ulRequestType, InternalCellIndex internalCellIndex);
/*CA Changes end  */

void checkAndUpdateFiCorrection( UInt32 ueMaxPowerFlag,
        UInt32 ueMinPowerFlag,
        TpcTriggeredToPowerCorrection powerCorrectionAndTpc,
        ULUEContext* ulUEContext_p);
#endif /*LTE_MAC_PUSCH_POWER_CONTROL_H*/
