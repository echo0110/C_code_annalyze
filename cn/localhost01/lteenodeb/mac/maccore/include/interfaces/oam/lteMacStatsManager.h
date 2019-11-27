/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMacStatsManager.h
 *
 ****************************************************************************
 *
 *  File Description : contains the function prototypes and data structures
 *  needed for stats manager to dump the stats info into the stats file.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef INCLUDED_STATS_MANAGER_H
#define INCLUDED_STATS_MANAGER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteMacTypes.h"
#include "lteMacTBSize.h"
#include "lteMacULUEContext.h"
#include "lteMacDLUEContext.h"
#include "lteMacExecutionLegs.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
****************************************************************************/
/****************************************************************************
 * Exported Types
 ***************************************************************************/
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129059) */
extern void fillDLStatsInfo(
/* SPR 5599 changes end (ZIP ID 129059) */
                              DLUEContext* ueContext_p,
                              DciCCEInfo *dciCCEInfo_p,
                              DLHARQProcess *harqProcess_p,
                              /* SPR 15909 fix start */
                              tickType_t scheduledTick
                              /* SPR 15909 fix end */
                              
                            ) ;
/* SPR 5599 changes start (ZIP ID 129059) */
extern void fillULStatsInfo(
/* SPR 5599 changes end (ZIP ID 129059) */
                             ULUEContext* ueContext_p,
                              /* SPR 15909 fix start */
                             tickType_t scheduledTick,
                              /* SPR 15909 fix end */
                             DciCCEInfo *dciCCEInfo_p,
                             UInt32 numRBsAllocated,
                             UInt32 ulGrantRequestType,
                             UInt32 startOffsetForPRACHAndPUCCH,
                             UInt32 advanceReserveRbsForPRACH
                          );
/* + SPR 17439 */
void initStatsManager(void);
void resetStatsInfo(UInt8 flag, UInt8 fromScheduler);
/* SPR 5599 changes start (ZIP ID 129059) */
void incrementDLStatsTickIndex(void);
void incrementULStatsTickIndex(void);
/* - SPR 17439 */
/* SPR 5599 changes end (ZIP ID 129059) */
/* ULA_CHG */
/* CA Stats Changes Start */
void lteMacUpdateStatsWriteTTIIndex(InternalCellIndex internalCellIndex);
void lteMacResetPuschStats(InternalCellIndex internalCellIndex);
/* FAPI2.4 Changes start */
 void lteMacUpdateFapiHarqIndStats(void *msg_p,
		InternalCellIndex internalCellIndex);
 void lteMacUpdateFapiHarqIndStatsR8(void *msg_p,
		InternalCellIndex internalCellIndex);
 void lteMacUpdateFapiHarqIndStatsR9(void *msg_p,
		InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
 void lteMacUpdateFapiRxUlschStats(UInt8 *pBufferToSend,
		InternalCellIndex internalCellIndex);
 void lteMacUpdateFapiRxUlschStatsR9(UInt8 *pBufferToSend,
		InternalCellIndex internalCellIndex);
 void lteMacUpdateFapiCqiIndStats(UInt8 *pBufferToSend,
		InternalCellIndex internalCellIndex);
 void lteMacUpdateFapiCqiIndStatsR9(UInt8 *pBufferToSend,
		InternalCellIndex internalCellIndex);
 void lteMacUpdateFapiRachIndStats(UInt8 *pBufferToSend,
		InternalCellIndex internalCellIndex);
 void lteMacUpdateFapiRachIndStatsR9(UInt8 *pBufferToSend,
		InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
/* FAPI2.4 Changes end*/
/* CA Stats Changes End */
#ifdef SSI_DEBUG_STATS
/* UL_MU_MIMO_CHG_START */
/* CA Stats Changes Start */
#define LTE_MAC_UPDATE_FAILURE_CAUSE(buddyPairFailureCause, internalCellIndex)\
{\
    LteMacPuschTTIStats *puschTTIStats_p = PNULL;\
    if (PNULL != gMacStats.pStats)\
    {\
       puschTTIStats_p = &gMacStats.pStats->puschTTIStats[(internalCellIndex)]\
                          [gMacStats.pStats->writeTTIIndex[(internalCellIndex)]];\
       puschTTIStats_p->buddyPairFailurePerCases[buddyPairFailureCause] +=1;\
    }\
}

#define LTE_MAC_UPDATE_TOTAL_RB_SHARED(allocatedRb, internalCellIndex)\
{\
    LteMacPuschTTIStats *puschTTIStats_p = PNULL;\
    if (PNULL != gMacStats.pStats)\
    {\
       puschTTIStats_p = &gMacStats.pStats->puschTTIStats[(internalCellIndex)]\
                          [gMacStats.pStats->writeTTIIndex[(internalCellIndex)]];\
       puschTTIStats_p->totalRbShared +=allocatedRb;\
    }\
}

#define LTE_MAC_UPDATE_TOTAL_BUDDY_PAIR_SUCCESS(internalCellIndex)\
{\
    LteMacPuschTTIStats *puschTTIStats_p = PNULL;\
    if (PNULL != gMacStats.pStats)\
    {\
       puschTTIStats_p = &gMacStats.pStats->puschTTIStats[(internalCellIndex)]\
                          [gMacStats.pStats->writeTTIIndex[(internalCellIndex)]];\
       puschTTIStats_p->totalBuddyPairSuccess +=1;\
    }\
}
#define LTE_MAC_UPDATE_UL_MIMO_UE_RETX(internalCellIndex)\
{\
    LteMacPuschTTIStats *puschTTIStats_p = PNULL;\
    if (PNULL != gMacStats.pStats)\
    {\
        puschTTIStats_p = &gMacStats.pStats->puschTTIStats[(internalCellIndex)]\
                           [gMacStats.pStats->writeTTIIndex[(internalCellIndex)]];\
        puschTTIStats_p->ulMuMimoTotalUeRetx  +=1;\
    }\
}
/* UL_MU_MIMO_CHG_END */
#define LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(ueIndex,currentTTIGlobal,\
                                          transmissionType,harqIndex,\
                                           numRBsAllocated,startRBPosition,\
                                           ulGrantRequestType,oppGranted,\
                                           frequencySeletiveScheduling,\
                                           selectedMCS,allocationFailureCause,\
                                           tpc,\
                                           /* UL_MU_MIMO_CHG_START */\
                                           ulMUMIMOStatus,buddyUeIndex,\
                                           userLocationType,nDmrs,\
                                           modulationScheme,\
                                           /* UL_MU_MIMO_CHG_END */\
                                           internalCellIndex)\
{\
    UInt8 numScheduledUETotal;\
    MuMimoStatsPerUe *muMimoStatsPerUe_p = PNULL;\
    LteMacPuschTTIStats *puschTTIStats_p = PNULL;\
    GET_MEM_FROM_POOL(MuMimoStatsPerUe,muMimoStatsPerUe_p,sizeof(MuMimoStatsPerUe), PNULL);\
    if (PNULL != gMacStats.pStats)\
    {\
        puschTTIStats_p = &gMacStats.pStats->puschTTIStats[(internalCellIndex)]\
                          [gMacStats.pStats->writeTTIIndex[(internalCellIndex)]];\
        puschTTIStats_p->absoluteTTI = currentTTIGlobal;\
        if (transmissionType == NEW_TX)\
        {\
            puschTTIStats_p->numScheduledUENewTx++;\
        }\
        else if((RE_TX == transmissionType)||\
                (ADAPTIVE_RE_TX == transmissionType)||\
                (NON_ADAPTIVE_RE_TX == transmissionType))\
        {\
            puschTTIStats_p->numScheduledUERetx++;\
        }\
        numScheduledUETotal = puschTTIStats_p->numScheduledUETotal;\
        muMimoStatsPerUe_p->ulMUMIMOStat = ulMUMIMOStatus;\
        muMimoStatsPerUe_p->buddyIndex = buddyUeIndex;\
        muMimoStatsPerUe_p->userLocType = userLocationType;\
        muMimoStatsPerUe_p->nDmrsValue = nDmrs;\
        muMimoStatsPerUe_p->modScheme = modulationScheme;\
        lteMacUpdateStatsPuschPerTTIPerUe(numScheduledUETotal,ueIndex,\
                transmissionType,harqIndex,\
                numRBsAllocated,startRBPosition,\
                ulGrantRequestType,oppGranted,\
                frequencySeletiveScheduling,\
                selectedMCS,allocationFailureCause, tpc,\
                /* UL_MU_MIMO_CHG_START */\
                muMimoStatsPerUe_p, internalCellIndex);\
                /* UL_MU_MIMO_CHG_END */\
        lteMacUpdateStatsUeCommonInfo(ueIndex,numScheduledUETotal,currentTTIGlobal);\
        puschTTIStats_p->numScheduledUETotal++;\
    }\
    freeMemPool(muMimoStatsPerUe_p);\
}
#else
#define LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(ueIndex,currentTTIGlobal,\
                                           transmissionType,harqIndex,\
                                           numRBsAllocated,startRBPosition,\
                                           ulGrantRequestType,oppGranted,\
                                           frequencySeletiveScheduling,\
                                           selectedMCS,allocationFailureCause, tpc,\
                                           /* UL_MU_MIMO_CHG_START */\
                                           ulMUMIMOStatus,buddyUeIndex,userLocationType,\
                                           nDmrs,modulationScheme,\
                                           /* UL_MU_MIMO_CHG_END */\
                                           internalCellIndex)
/* CA Stats Changes End */

#endif /* SSI_DEBUG_STATS */

#ifdef SSI_DEBUG_STATS
void lteMacUpdateStatsPuschPerTTIPerUe(UInt8 numScheduledUETotal,
        UInt16 ueIndex,
        UInt8 transmissionType,
        UInt8 harqIndex,
        UInt8 numRBsAllocated,
        UInt8 startRBPosition,
        UInt8 ulGrantRequestType,
        UInt32 oppGranted,
        UInt8 frequencySeletiveScheduling,
        UInt8 selectedMCS,
        UInt8 allocationFailureCause,
        SInt8 tpc
        /* UL_MU_MIMO_CHG_START */
        ,MuMimoStatsPerUe *muMimoStatsPerUe_p,
        /* UL_MU_MIMO_CHG_END */
        /* CA Stats Changes Start */
        InternalCellIndex internalCellIndex
        /* CA Stats Changes End */
        );

void lteMacUpdateStatsUeCommonInfo(
        /* +- SPR 18268 */
        UInt16 ueIndex,
        /* +- SPR 18268 */
        UInt16 ueIndexPos,
        UInt64 currentAbsoluteTTI
        );
#endif /* SSI_DEBUG_STATS*/

/* + SPR 17404 */
#ifdef KPI_STATS
void macUpdateKPIForDelTrigger( UInt8 numLcForDelTrig,
        DLUEContext* dlUeCtx_p, UInt8 *delTrigLCNodeForKpi);
#endif
/* - SPR 17404 */

#endif  /* INCLUDED_STATS_MANAGER_H */
