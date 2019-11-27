
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULSpsScheduler.c,v  $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the functionality for UL SPS 
 *                     scheduler.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacULSpsScheduler.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacULQosScheduler.h"
#include "lteMacUEConfMgr.h"
#include "lteMacPHICH.h"
#include "lteMacSpsPUSCHmgr.h"
#include "lteMacPuschPowerControl.h"

/******************************************************************************
 Private Definitions
*****************************************************************************/

/******************************************************************************
 Private Types
*****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
*****************************************************************************/

/****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*CA Changes start  */
LTE_CIRCQUEUE* ulSpsResRsvQ_gp[MAX_NUM_CELL] = {PNULL};
LTE_CIRCQUEUE* ulSpsResRelQ_gp[MAX_NUM_CELL] = {PNULL};
LTE_CIRCQUEUE* ulSpsStatusQ_gp[MAX_NUM_CELL] = {PNULL};
LTE_CIRCQUEUE* ulSpsActQ_gp[MAX_NUM_CELL] = {PNULL};
LTE_CIRCQUEUE* ulSpsDeactQ_gp[MAX_NUM_CELL] = {PNULL};
LTE_CIRCQUEUE* ulSpsReactQ_gp[MAX_NUM_CELL] = {PNULL};
LTE_CIRCQUEUE* ulSpsUeExclude_gp[MAX_NUM_CELL] = {PNULL};

/* + SPS_TDD_Changes */
SpsULStrIntervalTTIContext* spsULStrTTIMap_gp[MAX_NUM_CELL] = {PNULL};
#ifdef TDD_CONFIG
extern UlGrantHIPduSubFrameInfo *ulGrantPhichTableInfo_p[MAX_NUM_CELL];
#endif
/* - SPS_TDD_Changes */

/* Global variable for SPS System Wide Parameter*/
extern ULStrategyScheduleInfo* ulStrategyScheduleInfo_gp[MAX_NUM_CELL];

/* Number of RBs consumed by SPS strategy in a TTI so that it can be subtracted
 * from DRA strategies */
UInt8 numUlSpsRbsInTti_g[MAX_NUM_CELL] = {0};
/* Number of UEs that needs to be scheduled by SPS strategy in a TTI so that it 
 * can be added in total UEs to be scheduled in DRA strategies */
UInt8 numUlSpsUeInTti_g[MAX_NUM_CELL] = {0};
/*CA Changes end  */

/* EICIC +*/
extern UInt8 absPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN] ; /* Aggressor*/
extern UInt8 absPatternIndex_g[MAX_NUM_CELL]  ;


/* EICIC -*/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
STATIC MacRetType ulSpsNotValidEvent( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy, InternalCellIndex internalCellIndex);
STATIC MacRetType ulSpsImplicitRelease( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy, InternalCellIndex internalCellIndex );
STATIC MacRetType ulSpsUeImplicitRelPuschResp( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, ULUEContext *ulUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex);
STATIC MacRetType ulSpsUeImplicitReleaseinSdl( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, ULUEContext *ulUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex);
STATIC MacRetType ulSpsImplicitReleaseinOccList( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, ULUEContext *ulUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex );
STATIC MacRetType ulSpsConfigured( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy, InternalCellIndex internalCellIndex);
STATIC MacRetType ulSpsResRsvReq( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy, InternalCellIndex internalCellIndex );
STATIC MacRetType ulSpsActivationPuschSuccess( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy, InternalCellIndex internalCellIndex );
STATIC MacRetType ulSpsActivationPuschFail( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy, InternalCellIndex internalCellIndex);
STATIC MacRetType ulSpsActivated( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy, InternalCellIndex internalCellIndex);
STATIC MacRetType ulSpsMaxDciAttemptReached( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy, InternalCellIndex internalCellIndex);
STATIC MacRetType ulSpsMaxDciAttemptReachedForDeact( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy, InternalCellIndex internalCellIndex);
STATIC MacRetType ulSpsDeactInit( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy, InternalCellIndex internalCellIndex );
STATIC MacRetType ulSpsDeactPdcchFail( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy, InternalCellIndex internalCellIndex);
STATIC MacRetType processULSpsOccList( 
        LP_SpsULStrIntervalTTIContext spsUlStrIntervalTTI_p,
        /* +- SPR 17777 */
        UInt8 spsOccasionUeCount,UInt32 delayToPhy,
        /* +- SPR 17777 */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
        /*CA Changes end  */
STATIC MacRetType processULSpsSdl( LP_SpsULStrIntervalTTIContext spsULTTIContext_p,
        /*CA Changes start  */
        UInt8 sdlCount, UInt32 delayToPhy, InternalCellIndex internalCellIndex );
        /*CA Changes end  */
STATIC MacRetType assignULRBsAndSendToPDCCHFromSpsOccasion(
        ULUEContext *ueULContext_p,
        UInt32 delayToPhy //SPS_TDD_Changes
#ifdef FDD_CONFIG
        ,UInt8 isTTIBundle
#endif       
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );
/* CA Stats Changes Start */
STATIC void ulAddUeInSdl( 
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        SInt8 subframe_offset,
        UInt16 ttiIndex,
        UInt16 ulSpsInterval,
#endif
        /* - SPS_TDD_Changes */
        UInt16 ueIndex, 
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p,
        LP_SpsUlInfo spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
        ,InternalCellIndex   internalCellIndex
#endif
        );
STATIC void ulDeleteUeFromSdl(
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        SInt8 subframeoffset,
        UInt16 ulSpsInterval,
        UInt16 index,
#endif
        /* - SPS_TDD_Changes */
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p,
        /* +- SPR 17777 */
        LP_SpsUlInfo spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
        ,InternalCellIndex internalCellIndex
#endif
        );
/* +- SPR 17777 */
STATIC void ulAddUeInOccasionList(
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        SInt8 subframe_offset,
        UInt16 ulSpsInterval,
#endif
        /* - SPS_TDD_Changes */
        UInt16 ueIndex, 
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p,
        LP_SpsUlInfo spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
        ,InternalCellIndex internalCellIndex
#endif
        );
STATIC void ulDeleteUeFromOccasionList( 
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        SInt8 subframe_offset,
#endif
        UInt16 ulSpsInterval, 
        /* - SPS_TDD_Changes */
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p,
        /* +- SPR 17777 */
        LP_SpsUlInfo  spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
        ,InternalCellIndex internalCellIndex
#endif
        );
/* +- SPR 17777 */
/* CA Stats Changes End */
/* + SPS_TDD_Changes */
void ulSpsResetOccSchType( LP_SpsULStrIntervalTTIContext ttiContext_p,
    UInt8 count );
void ulSpsResetSdlSchType( LP_SpsULStrIntervalTTIContext ttiContext_p,
    UInt8 count );
/* - SPS_TDD_Changes */
STATIC MacRetType ulSpsUeImplicitReleaseinSdlInDeactInit( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, ULUEContext *ulUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex);
/* + SPS_TDD_Changes */
void ulSpsStrategySendPdcchTrigger( ULUEContext *ueULContext_p, 
        ULStrategyTxNode *ulStrNode_p, 
#ifdef TDD_CONFIG
        UInt32 ulSubFrame,
#endif
        /* - SPS_TDD_Changes */
        InternalCellIndex internalCellIndex );
/* +- SPR 17777 */
STATIC void ulSpsSaveHarqInCtxForSdl( ULUEContext *ueULContext_p
#ifdef FDD_CONFIG
        ,UInt32 delayToPhy
#endif
        );
/* +- SPR 17777 */
/* + SPS_TDD_Changes */
STATIC LP_MacULSpsOccList  ulAddUeInOccListAtPeriodicity(UInt16 ueIndex,
        LP_SpsULStrIntervalTTIContext ttiContext_p);
STATIC LP_MacULSpsDciListTTI  ulAddUeInSdlAtPeriodicity(UInt16 ueIndex,
        LP_SpsULStrIntervalTTIContext ttiContext_p);
STATIC MacRetType checkSpsOccPerTTI(
#ifdef TDD_CONFIG
        SInt8 subframe_offset,
        UInt16 index,
        UInt16 ulSpsInterval,
#endif
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        void * spsCtx_p,
        InternalCellIndex internalCellIndex);

STATIC MacRetType checkSpsDciPerTTI(
#ifdef TDD_CONFIG
        SInt8 subframe_offset,
        UInt16 index,
        UInt16 ulSpsInterval,
#endif
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        void * spsCtx_p,
        InternalCellIndex internalCellIndex);
/* - SPS_TDD_Changes */

UInt8 checkSpsOccasionTti( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex);

/*CA Changes start  */
void triggerSpsExplicitRelease(ULUEContext *ulCntxt_p, SpsCause event, InternalCellIndex internalCellIndex);
void triggerSpsImplicitRelease(ULUEContext *ulCntxt_p, SpsCause event, InternalCellIndex internalCellIndex);
/*CA Changes end  */


/* State machine to handle startegy actions for various states */
macULStrategyHandlerFuncT ulStateMachine[MAX_SPS_STATE][MAX_SPS_CAUSE + 1] =
{
    { /* SPS_IDLE */
        ulSpsNotValidEvent  /* UE_SYNC_LOSS_ON  */,
        ulSpsNotValidEvent  /* UE_SYNC_LOSS_OFF */,
        ulSpsConfigured     /* SPS_CONFIG_RECVD */,
        ulSpsNotValidEvent  /* FIRST_NON_ZERO_QUEUE_LOAD */,
        ulSpsNotValidEvent  /* VOICE_PACKET */,
        ulSpsNotValidEvent  /* SPS_PUSCH_SUCCESS */,
        ulSpsNotValidEvent  /* SPS_PUSCH_FAILURE */,
        ulSpsNotValidEvent  /* FIRST_ACK_NACK */,
        ulSpsNotValidEvent  /* PDCCH_CCE_ALLOC_FAILURE */,
        ulSpsNotValidEvent  /* HARQ_TIMER_EXPIRED */,
        ulSpsNotValidEvent  /* SILENCE_DETECTED */,
        ulSpsNotValidEvent  /* MCS_CHANGED */,
        ulSpsNotValidEvent  /* BLER_THRESHOLD */,
        ulSpsNotValidEvent  /* PDU_SEG_IND */,
        ulSpsNotValidEvent  /* MAX_DCI_ATTEMPT */,
        ulSpsNotValidEvent  /* MAX_EMPTY_TX */,
        ulSpsNotValidEvent  /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        ulSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
	/* + SPS_TDD_Changes */
        ulSpsNotValidEvent  /* MEAS_GAP_RECONFIG */,
        ulSpsNotValidEvent  /* DRX_RECONFIG */,
	/* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        ulSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_CONFIGURED */
        ulSpsNotValidEvent /* UE_SYNC_LOSS_ON  */,
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        ulSpsImplicitRelease /* SPS_CONFIG_RECVD */,
        ulSpsResRsvReq       /* FIRST_NON_ZERO_QUEUE_LOAD */,
        ulSpsNotValidEvent   /* VOICE_PACKET */,
        ulSpsNotValidEvent   /* SPS_PUSCH_SUCCESS */,
        ulSpsNotValidEvent   /* SPS_PUSCH_FAILURE */,
        ulSpsNotValidEvent   /* FIRST_ACK_NACK */,
        ulSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        ulSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        ulSpsNotValidEvent   /* SILENCE_DETECTED */,
        ulSpsNotValidEvent   /* MCS_CHANGED */,
        ulSpsNotValidEvent   /* BLER_THRESHOLD */,
        ulSpsNotValidEvent   /* PDU_SEG_IND */,
        ulSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        ulSpsNotValidEvent   /* MAX_EMPTY_TX */,
        ulSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        ulSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
	/* + SPS_TDD_Changes */
        ulSpsNotValidEvent  /* MEAS_GAP_RECONFIG */,
        ulSpsNotValidEvent  /* DRX_RECONFIG */,
	/* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        ulSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_RES_RSV_REQ_INITIATED */
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_ON  */,
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        ulSpsUeImplicitRelPuschResp /* SPS_CONFIG_RECVD */,
        ulSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        ulSpsNotValidEvent   /* VOICE_PACKET */,
        ulSpsActivationPuschSuccess /* SPS_PUSCH_SUCCESS */,
        ulSpsActivationPuschFail    /* SPS_PUSCH_FAILURE */,
        ulSpsNotValidEvent   /* FIRST_ACK_NACK */,
        ulSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        ulSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        ulSpsNotValidEvent   /* SILENCE_DETECTED */,
        ulSpsNotValidEvent   /* MCS_CHANGED */,
        ulSpsNotValidEvent   /* BLER_THRESHOLD */,
        ulSpsNotValidEvent   /* PDU_SEG_IND */,
        ulSpsNotValidEvent /* MAX_DCI_ATTEMPT */,
        ulSpsNotValidEvent   /* MAX_EMPTY_TX */,
        ulSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        ulSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
	/* + SPS_TDD_Changes */
        ulSpsNotValidEvent  /* MEAS_GAP_RECONFIG */,
        ulSpsNotValidEvent  /* DRX_RECONFIG */,
	/* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        ulSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_ACTIVATION_INITIATED */
        ulSpsNotValidEvent  /* UE_SYNC_LOSS_ON  */,
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        ulSpsUeImplicitReleaseinSdl /* SPS_CONFIG_RECVD */,
        ulSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        ulSpsNotValidEvent   /* VOICE_PACKET */,
        ulSpsNotValidEvent   /* SPS_PDSCH_PUSCH_SUCCESS */,
        ulSpsNotValidEvent   /* SPS_PDSCH_PUSCH_FAILURE */,
        ulSpsActivated       /* FIRST_ACK_NACK */,
        ulSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        ulSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        ulSpsNotValidEvent   /* SILENCE_DETECTED */,
        ulSpsNotValidEvent   /* MCS_CHANGED */,
        ulSpsNotValidEvent   /* BLER_THRESHOLD */,
        ulSpsNotValidEvent   /* PDU_SEG_IND */,
        ulSpsMaxDciAttemptReached /* MAX_DCI_ATTEMPT */,
        ulSpsNotValidEvent   /* MAX_EMPTY_TX */,
        ulSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        ulSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
	/* + SPS_TDD_Changes */
        ulSpsUeImplicitReleaseinSdl /* MEAS_GAP_RECONFIG */,
        ulSpsUeImplicitReleaseinSdl /* DRX_RECONFIG */,
	/* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        ulSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_ACTIVATED */
        ulSpsNotValidEvent /* UE_SYNC_LOSS_ON */,
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        ulSpsImplicitReleaseinOccList /* SPS_CONFIG_RECVD */,
        ulSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        ulSpsNotValidEvent   /* VOICE_PACKET */,
        ulSpsNotValidEvent   /* SPS_PUSCH_SUCCESS */,
        ulSpsNotValidEvent   /* SPS_PUSCH_FAILURE */,
        ulSpsNotValidEvent   /* FIRST_ACK_NACK */,
        ulSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        ulSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        ulSpsDeactInit       /* SILENCE_DETECTED */,
        ulSpsDeactInit   /* MCS_CHANGED */,
        ulSpsNotValidEvent   /* BLER_THRESHOLD */,
        ulSpsDeactInit       /* PDU_SEG_IND */,
        ulSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        ulSpsImplicitReleaseinOccList       /* MAX_EMPTY_TX */,
        ulSpsDeactInit   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG 
        ulSpsDeactInit  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
	/* + SPS_TDD_Changes */
        ulSpsDeactInit  /* MEAS_GAP_RECONFIG */,
        ulSpsDeactInit  /* DRX_RECONFIG */,
	/* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        ulSpsDeactInit  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_DEACTIVATION_INITIATED */
        ulSpsNotValidEvent  /* UE_SYNC_LOSS_ON */,
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        ulSpsUeImplicitReleaseinSdlInDeactInit  /* SPS_CONFIG_RECVD */,
        ulSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        ulSpsNotValidEvent   /* VOICE_PACKET */,
        ulSpsNotValidEvent   /* SPS_PUSCH_SUCCESS */,
        ulSpsNotValidEvent   /* SPS_PUSCH_FAILURE */,
        ulSpsNotValidEvent     /* FIRST_ACK_NACK */,
        ulSpsDeactPdcchFail  /* PDCCH_CCE_ALLOC_FAILURE */,
        ulSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        ulSpsNotValidEvent   /* SILENCE_DETECTED */,
        ulSpsNotValidEvent   /* MCS_CHANGED */,
        ulSpsNotValidEvent   /* BLER_THRESHOLD */,
        ulSpsNotValidEvent   /* PDU_SEG_IND */,
        ulSpsMaxDciAttemptReachedForDeact /* MAX_DCI_ATTEMPT */,
        ulSpsNotValidEvent   /* MAX_EMPTY_TX */,
        ulSpsNotValidEvent     /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        ulSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
	/* + SPS_TDD_Changes */
        ulSpsNotValidEvent  /* MEAS_GAP_RECONFIG */,
        ulSpsNotValidEvent  /* DRX_RECONFIG */,
	/* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        ulSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_DEACTIVATED */
        ulSpsNotValidEvent /* UE_SYNC_LOSS_ON  */,
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        ulSpsImplicitRelease /* SPS_CONFIG_RECVD */,
        ulSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        ulSpsResRsvReq       /* VOICE_PACKET */,
        ulSpsNotValidEvent   /* SPS_PUSCH_SUCCESS */,
        ulSpsNotValidEvent   /* SPS_PUSCH_FAILURE */,
        ulSpsNotValidEvent   /* FIRST_ACK_NACK */,
        ulSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        ulSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        ulSpsNotValidEvent   /* SILENCE_DETECTED */,
        ulSpsNotValidEvent   /* MCS_CHANGED */,
        ulSpsNotValidEvent   /* BLER_THRESHOLD */,
        ulSpsNotValidEvent   /* PDU_SEG_IND */,
        ulSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        ulSpsNotValidEvent   /* MAX_EMPTY_TX */,
        ulSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        ulSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
	/* + SPS_TDD_Changes */
        ulSpsNotValidEvent  /* MEAS_GAP_RECONFIG */,
        ulSpsNotValidEvent  /* DRX_RECONFIG */,
	/* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        ulSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_REACTIVATION_INITIATED */
        ulSpsNotValidEvent /* UE_SYNC_LOSS_ON */,
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        ulSpsUeImplicitReleaseinSdl /* SPS_CONFIG_RECVD */,
        ulSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        ulSpsNotValidEvent   /* VOICE_PACKET */,
        ulSpsNotValidEvent   /* SPS_PUSCH_SUCCESS */,
        ulSpsNotValidEvent   /* SPS_PUSCH_FAILURE */,
        ulSpsNotValidEvent   /* FIRST_ACK_NACK */,
        ulSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        ulSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        ulSpsNotValidEvent   /* SILENCE_DETECTED */,
        ulSpsNotValidEvent   /* MCS_CHANGED */,
        ulSpsNotValidEvent   /* BLER_THRESHOLD */,
        ulSpsNotValidEvent   /* PDU_SEG_IND */,
        ulSpsMaxDciAttemptReached /* MAX_DCI_ATTEMPT */,
        ulSpsNotValidEvent   /* MAX_EMPTY_TX */,
        ulSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        ulSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
	/* + SPS_TDD_Changes */
        ulSpsNotValidEvent  /* MEAS_GAP_RECONFIG */,
        ulSpsNotValidEvent  /* DRX_RECONFIG */,
	/* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        ulSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* UE_NOT_READY */
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_ON  */,
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        ulSpsNotValidEvent   /* SPS_CONFIG_RECVD */,
        ulSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        ulSpsNotValidEvent   /* VOICE_PACKET */,
        ulSpsNotValidEvent   /* SPS_PUSCH_SUCCESS */,
        ulSpsNotValidEvent   /* SPS_PUSCH_FAILURE */,
        ulSpsNotValidEvent   /* FIRST_ACK_NACK */,
        ulSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        ulSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        ulSpsNotValidEvent   /* SILENCE_DETECTED */,
        ulSpsNotValidEvent   /* MCS_CHANGED */,
        ulSpsNotValidEvent   /* BLER_THRESHOLD */,
        ulSpsNotValidEvent   /* PDU_SEG_IND */,
        ulSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        ulSpsNotValidEvent   /* MAX_EMPTY_TX */,
        ulSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        ulSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
	/* + SPS_TDD_Changes */
        ulSpsNotValidEvent  /* MEAS_GAP_RECONFIG */,
        ulSpsNotValidEvent  /* DRX_RECONFIG */,
	/* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        ulSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    /* SPR 8244 fix start */
    { /* INVALID_STATE  */
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_ON  */,
        ulSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        ulSpsNotValidEvent   /* SPS_CONFIG_RECVD */,
        ulSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        ulSpsNotValidEvent   /* VOICE_PACKET */,
        ulSpsNotValidEvent   /* SPS_PUSCH_SUCCESS */,
        ulSpsNotValidEvent   /* SPS_PUSCH_FAILURE */,
        ulSpsNotValidEvent   /* FIRST_ACK_NACK */,
        ulSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        ulSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        ulSpsNotValidEvent   /* SILENCE_DETECTED */,
        ulSpsNotValidEvent   /* MCS_CHANGED */,
        ulSpsNotValidEvent   /* BLER_THRESHOLD */,
        ulSpsNotValidEvent   /* PDU_SEG_IND */,
        ulSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        ulSpsNotValidEvent   /* MAX_EMPTY_TX */,
        ulSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        ulSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
	/* + SPS_TDD_Changes */
        ulSpsNotValidEvent  /* MEAS_GAP_RECONFIG */,
        ulSpsNotValidEvent  /* DRX_RECONFIG */,
	/* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        ulSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    /* SPR 8244 fix end */
};

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* SPR 16916 FIX START */
#ifdef KPI_STATS
extern void freeULHarqProcess(ULHarqInfo* tempULHarqInfo, UInt16 ueIndex,InternalCellIndex internalCellIndex);
#else
extern void freeULHarqProcess(ULHarqInfo* tempULHarqInfo);
#endif
/* SPR 16916 FIX END */

/****************************************************************************
 * Function Definitions
 ****************************************************************************/

/****************************************************************************
 * Function 	:  macUlSpsProcessActQ
 * Inputs 		:  count - number of nodes in queue,
 * 				   delayToPhy - Delay to PHY,
 *                 internalCellIndex - cell index used at MAC.
 * Output		:
 * Return  		: 
 * Description  :  This function processes the activation queue nodes.
****************************************************************************/
/*CA Changes start  */
UInt16 macUlSpsProcessActQ( UInt32 count, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
/*CA Changes end  */
{
    LP_MacULSpsActReq         macUlSpsActReq_p = PNULL;
    LP_SpsUlInfo              spsUlInfo_p      = PNULL;
    LP_SpsULIntervalProfile   ulSpsIntProf_p   = PNULL;
    ULUEContext               *ulUeCtx_p       = PNULL;
    UInt16                    ulSpsInterval    = 0;
    UInt8                    pendingDeleteFlag = 0;
    /* +- SPR 17777 */

    /*!
     * \code 
     * Dequeue number of nodes in Activation queue, ulSpsActQ_g.
     * Call ulStateMachine for appropriate handling.
     * \endcode
     * */
    /*EICIC +*/
    UInt32 sysFrameNum 			= 0;
    UInt32 subFrameNum 			= 0;
    UInt8  absSubFrameNum 		= 0;
    UInt8  isCurrentABSSubFrame = LTE_FALSE;
    UInt8 numBitsAbsPattern = 0;


    getSFAndSFN(&sysFrameNum, &subFrameNum,internalCellIndex);

    /*Calculate the SFN and SF by considering the delay to PHY*/
    subFrameNum = subFrameNum + delayToPhy;
    
    if(subFrameNum >= MAX_SUB_FRAME)
    {
        subFrameNum -= MAX_SUB_FRAME;
        sysFrameNum++;
        if (sysFrameNum == MAX_SFN_VALUE)
        {
            sysFrameNum = 0;
        }
    }

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

    absSubFrameNum = ((((sysFrameNum )*MAX_SUB_FRAME) +(subFrameNum)) % numBitsAbsPattern);

  
    if (LTE_TRUE == absPattern_g[internalCellIndex][absPatternIndex_g[internalCellIndex]][absSubFrameNum]) 
    {
 	    isCurrentABSSubFrame = LTE_TRUE;
    }
  
	/* EICIC -*/
    while ( count-- )
    {
        /* Dequeue the node from Activation queue */
        /*CA Changes start  */
        DEQUEUE_MAC_UL_SPS_Q( ulSpsActQ_gp[internalCellIndex], macUlSpsActReq_p );
        /*CA Changes end  */

        if( PNULL == macUlSpsActReq_p )
        {
            continue;
        }
        pendingDeleteFlag =
            ulUECtxInfoArr_g[macUlSpsActReq_p->ueIndex].pendingDeleteFlag;
        /* SPR 6884 changes start */
        ulUeCtx_p = 
            ulUECtxInfoArr_g[macUlSpsActReq_p->ueIndex].ulUEContext_p;
        if( PNULL == ulUeCtx_p )
        {
            freeMemPool( macUlSpsActReq_p );
            macUlSpsActReq_p = PNULL;

            continue;
        }
        /* SPR 6884 changes end */

    
        /* EICIC +*/
        if (LTE_TRUE == isCurrentABSSubFrame)
        {
            if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsActQ_gp[internalCellIndex], macUlSpsActReq_p ))
            {
                freeMemPool( macUlSpsActReq_p );
                macUlSpsActReq_p = PNULL;
            }
            continue;
        }/* EICIC -*/

        if ( !pendingDeleteFlag )
        {
            spsUlInfo_p = &ulUeCtx_p->spsUlInfo;

            if( PNULL != spsUlInfo_p->spsUlSetupInfo_p )  
            {
                /* Check for MCS if it is in valid range */
                ulSpsInterval = spsUlInfo_p->spsUlSetupInfo_p->\
                                semiPersistentSchedIntervalUL;
                /*CA Changes start  */
                ulSpsIntProf_p = &cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig. \
                                  spsUlIntervalProfile[ulSpsInterval];
                /*CA Changes end  */

                if( ulSpsIntProf_p->ulMinMcsSps + ulSpsIntProf_p->ulMcsMargin 
                        <= ulUeCtx_p->mcsIndex )
                {
                    /* +- SPR 17777 */
                    /* Call state machine for appropriate handling */
                    if ( MAC_SUCCESS == ulStateMachine[spsUlInfo_p->spsState]
                            [macUlSpsActReq_p->cause](
                                macUlSpsActReq_p->ueIndex, spsUlInfo_p->spsState, 
                                macUlSpsActReq_p->cause, ulUeCtx_p, macUlSpsActReq_p, 
                                /*CA Changes start  */
                                delayToPhy, internalCellIndex ) )
                                /*CA Changes end  */
                    {
                        /* +- SPR 17777 */
                        LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                                 getCurrentTick(), __LINE__, macUlSpsActReq_p->ueIndex, spsUlInfo_p->spsState, macUlSpsActReq_p->cause,
                                 spsUlInfo_p->spsState, 0,0, __func__, "UlStateMachine");

                        /* Free the memory for Activation Queue node */
                        freeMemPool( macUlSpsActReq_p );
                        macUlSpsActReq_p = PNULL;
                    }
                    else
                    {
                        /* +- SPR 17777 */
                        LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                                 getCurrentTick(), __LINE__, macUlSpsActReq_p->ueIndex,spsUlInfo_p->spsState, macUlSpsActReq_p->cause,
                                 spsUlInfo_p->spsState, 0,0, __func__, "UlStateMachine");
                    }
                }
                else
                {
                    /* UE cannot be activated as MCS is not good. Enqueue back to
                     * Activation queue so that it can be tried in next tick */
                    /*CA Changes start  */
                    /*Cov_fix_30April_63543_start*/
                    if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsActQ_gp[internalCellIndex], macUlSpsActReq_p ))
                    {
                        freeMemPool( macUlSpsActReq_p );
                        macUlSpsActReq_p = PNULL;
                    }
                    /*Cov_fix_30April_63543_start*/
                    /*CA Changes end  */
                }
            }
            else
            {
                /* Free the memory for Activation Queue node */
                freeMemPool( macUlSpsActReq_p );
                macUlSpsActReq_p = PNULL;
            }
        }
        else
        {
            /* UE has moved to Pending delete so clean up the SPS context */
            /*CA Changes start  */
            ulSpsPendingDeleteHandling( ulUeCtx_p, internalCellIndex );
            /*CA Changes end  */

            /* Free the memory for Activation Queue node */
            freeMemPool( macUlSpsActReq_p );
            macUlSpsActReq_p = PNULL;
            continue;
        }
    }

    return MAC_SUCCESS;
}

/****************************************************************************
 *	Function Name: macUlSpsProcessDeactQ
 *  Inputs   	 : count - number of nodes in queue,
 * 				   delayToPhy - Delay to PHY,
 *                 internalCellIndex - cell index used at MAC.
 *	Returns		 : MAC_SUCCESS/MAC_FAILURE
 *  Description  : This function processes the activation queue nodes.
 * 
 ***************************************************************************/
/*CA Changes start  */
UInt16 macUlSpsProcessDeactQ( UInt32 count, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
/*CA Changes end  */
{
    UInt8                pendingDeleteFlag = 0;
    LP_MacULSpsDeactReq  macUlSpsDectReq_p = PNULL;
    LP_SpsUlInfo         spsUlInfo_p       = PNULL;
    LP_MacULSpsOccList   occNode_p         = PNULL;
    ULUEContext          *ulUeCtx_p        = PNULL;
    /* +- SPR 17777 */

    /*!
     * \code 
     * Dequeue number of nodes in Deactivation queue, ulSpsDeactQ_g.
     * Call ulStateMachine for appropriate handling.
     * \endcode
     * */
    while ( count-- )
    {
        /* Dequeue the node from Deactivation queue */
        /*CA Changes start  */
        DEQUEUE_MAC_UL_SPS_Q( ulSpsDeactQ_gp[internalCellIndex], macUlSpsDectReq_p );
        /*CA Changes end  */

        if( PNULL == macUlSpsDectReq_p )
        {
            continue;
        }
        pendingDeleteFlag =
            ulUECtxInfoArr_g[macUlSpsDectReq_p->ueIndex].pendingDeleteFlag;
        /* SPR 6884 changes start */
        ulUeCtx_p = ulUECtxInfoArr_g[macUlSpsDectReq_p->ueIndex].ulUEContext_p;
        if( PNULL == ulUeCtx_p )
        {

            /* Free the memory for Deactivation Queue node */
            freeMemPool( macUlSpsDectReq_p );
            macUlSpsDectReq_p = PNULL;
            continue;
        }
        /* SPR 6884 changes end */
        if ( !pendingDeleteFlag )
        {
            spsUlInfo_p = &ulUeCtx_p->spsUlInfo;

            occNode_p = (LP_MacULSpsOccList)spsUlInfo_p->sdlOccNode_p;            
            if( PNULL != occNode_p )
            {
                /* +- SPR 17777 */
                /* Call state machine for appropriate handling */
                /* SPR 16722 Fix Start */
                if( MAC_FAILURE == ulStateMachine[spsUlInfo_p->spsState]
                        [macUlSpsDectReq_p->cause](
                            macUlSpsDectReq_p->ueIndex, spsUlInfo_p->spsState,
                            macUlSpsDectReq_p->cause, ulUeCtx_p, macUlSpsDectReq_p, 
                            /*CA Changes start  */
                            delayToPhy, internalCellIndex ) )
                    /*CA Changes end  */
                    /* SPR 16722 Fix End */
                {
                        /* +- SPR 17777 */
                    LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                             getCurrentTick(), __LINE__, macUlSpsDectReq_p->ueIndex,spsUlInfo_p->spsState, macUlSpsDectReq_p->cause,
                             spsUlInfo_p->spsState, 0,0, __func__, "UlStateMachine");
                    /* Enqueue the node back to deactivation queue to be
                     * processed in next tick */
                    /*CA Changes start  */
                    /*Cov_fix_30April_63544_start*/
                    if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsDeactQ_gp[internalCellIndex], macUlSpsDectReq_p ))
                    {
                        freeMemPool( macUlSpsDectReq_p );
                        macUlSpsDectReq_p = PNULL;
                    }
                    /*Cov_fix_30April_63544_stop*/
                    /* Update Q Stats */
                    /* CA Stats Changes Start */
                    LTE_MAC_QUEUE_USED( UL_SPS_DEACT_Q,  
                            QCOUNT_MAC_UL_SPS_Q(ulSpsDeactQ_gp[internalCellIndex]), 
                            internalCellIndex );
                    /* CA Stats Changes End */
                    /*CA Changes end  */
                }
                else
                {            
                        /* +- SPR 17777 */
                    LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                             getCurrentTick(), __LINE__, macUlSpsDectReq_p->ueIndex,spsUlInfo_p->spsState, macUlSpsDectReq_p->cause,
                             spsUlInfo_p->spsState, 0,0, __func__, "UlStateMachine");
                    /* Free the memory for Deactivation Queue node */
                    freeMemPool( macUlSpsDectReq_p );
                    macUlSpsDectReq_p = PNULL;
                }
            }
            else
            {
                /* Free the memory for Deactivation Queue node */
                freeMemPool( macUlSpsDectReq_p );
                macUlSpsDectReq_p = PNULL;
            }
        }
        else
        {
            /* UE has moved to Pending delete so clean up the SPS context */
            /*CA Changes start  */
            ulSpsPendingDeleteHandling( ulUeCtx_p, internalCellIndex );
            /*CA Changes end  */

            /* Free the memory for Deactivation Queue node */
            freeMemPool( macUlSpsDectReq_p );
            macUlSpsDectReq_p = PNULL;
            continue;
        }
    }
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : macUlSpsProcessStatusQ
 * Inputs         : count - number of nodes in queue,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function processes UL status queue nodes.
 *****************************************************************************/

UInt16 macUlSpsProcessStatusQ( UInt32 count, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
{
    UInt8             pendingDeleteFlag = 0;
    UInt16            retVal            = MAC_SUCCESS;
    LP_MacULSpsStatus macUlSpsStatus_p  = PNULL;
    LP_SpsUlInfo      spsUlInfo_p       = PNULL; 
    ULUEContext       *ulUeCtx_p        = PNULL;
    /* +- SPR 17777 */

    /*!
     * \code 
     * Dequeue number of nodes in UL Status queue, ulSpsStatusQ_g.
     * Call ulStateMachine for appropriate handling.
     * \endcode
     * */
    while ( count-- )
    {
        /* Dequeue the node from Status queue */
        /*CA Changes start  */
        DEQUEUE_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex], macUlSpsStatus_p );
        /*CA Changes end  */

        if( PNULL == macUlSpsStatus_p )
        {
            continue;
        }

        pendingDeleteFlag =
            ulUECtxInfoArr_g[macUlSpsStatus_p->ueIndex].pendingDeleteFlag;
        /* SPR 6884 changes start */
        ulUeCtx_p = ulUECtxInfoArr_g[macUlSpsStatus_p->ueIndex].ulUEContext_p;
        if( PNULL == ulUeCtx_p )
        {

            /* Free the memory for Status Queue node */
            freeMemPool( macUlSpsStatus_p );
            macUlSpsStatus_p = PNULL;
            continue;
        }
        /* SPR 6884 changes end */
        if ( !pendingDeleteFlag )
        {
            spsUlInfo_p = &ulUeCtx_p->spsUlInfo;
            /* +- SPR 17777 */
            /* Call state machine for appropriate handling */
            if( MAC_SUCCESS == ulStateMachine[spsUlInfo_p->spsState]
                    [macUlSpsStatus_p->response](
                        macUlSpsStatus_p->ueIndex, spsUlInfo_p->spsState, 
                        macUlSpsStatus_p->response, ulUeCtx_p, macUlSpsStatus_p, 
                        /*CA Changes start  */
                        delayToPhy, internalCellIndex ) )
                        /*CA Changes end  */
            {
                        /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                         getCurrentTick(), __LINE__, macUlSpsStatus_p->ueIndex,spsUlInfo_p->spsState, macUlSpsStatus_p->response,
                         spsUlInfo_p->spsState, 0,0, __func__, "UlStateMachine");
                /* Free the memory */
                freeMemPool( macUlSpsStatus_p );
                macUlSpsStatus_p = PNULL;
            }
            else
            {
                        /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                         getCurrentTick(), __LINE__, macUlSpsStatus_p->ueIndex,spsUlInfo_p->spsState, macUlSpsStatus_p->response,
                         spsUlInfo_p->spsState, 0,0, __func__, "UlStateMachine");
            }
        }
        else
        {
            /* UE has moved to Pending delete so clean up the SPS context */
            /*CA Changes start  */
            ulSpsPendingDeleteHandling( ulUeCtx_p, internalCellIndex );
            /*CA Changes end  */

            /* Free the memory for Status Queue node */
            freeMemPool( macUlSpsStatus_p );
            macUlSpsStatus_p = PNULL;
            continue;
        }
    }    
    return retVal;
}

/********************************************************************************
 * Function Name  : ulSpsConfigured
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called when SPS Config is received by RRC.
 ********************************************************************************/
MacRetType ulSpsConfigured( UInt16 ueIndex, SpsStates state, SpsCause cause, 
        /*CA Changes start  */
        ULUEContext *ulUeCtx_p, void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    LP_SpsUlInfo  spsUlInfo_p  = PNULL;
    UInt16 ulSpsInterval          = 0;
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    UInt16 spsInterval = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt16 spsInterval = MAX_UL_SPS_INTERVAL;
#endif
    /* - SPS_TDD_Changes */
    LP_SpsULIntervalProfile   ulSpsIntervalProfile_p = PNULL;
    ulSpsInterval =ulUeCtx_p->spsUlInfo.spsUlSetupInfo_p->\
                    semiPersistentSchedIntervalUL;
    ulSpsIntervalProfile_p = &cellSpecificParams_g.\
        cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
        ulSchdConfig.spsUlIntervalProfile[ulSpsInterval] ;


    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;

    /* Set the state accordingly */
    spsUlInfo_p->spsState = SPS_CONFIGURED;

    /* Set SDL or Occasion node pointer in UE context as NULL */
    spsUlInfo_p->sdlOccNode_p = PNULL;
    /* Initialize TTI index with invalid value in UE context */

    /* + SPS_TDD_Changes */
    spsUlInfo_p->ttiIndex = spsInterval;
    spsUlInfo_p->actTtiIndex = spsInterval;
    /* - SPS_TDD_Changes */

    /* Initialize vars in UE context in UE context */
    spsUlInfo_p->numOfSureSpsEmptyTx  = 0;
    spsUlInfo_p->numOfUnsureSpsEmptyTx  = 0;
    spsUlInfo_p->avgRlcSduSize = 0;
    if(ulUeCtx_p->mcsIndex >= ulSpsIntervalProfile_p->ulMinMcsSps)
    {
        spsUlInfo_p->isUeUlWbMcsBelowThresh = LTE_FALSE;
    }
    else 
    {
        spsUlInfo_p->isUeUlWbMcsBelowThresh = LTE_TRUE;
    }
    /* SPR 19038 Fix Start */
    spsUlInfo_p->isUeMeasGapCollisionWithSpsOcc = LTE_FALSE;
    /* SPR 19038 Fix End */

    spsUlInfo_p->hpId[0].harqId = INVALID_HARQ_ID;
    spsUlInfo_p->hpId[1].harqId = INVALID_HARQ_ID;
    spsUlInfo_p->hpId[0].isDataReceived = FALSE;
    spsUlInfo_p->hpId[1].isDataReceived = FALSE;

    if (ulUeCtx_p->maxUlHARQTx <= MAX_SPS_HARQ_RETX_NUM)
    {
        spsUlInfo_p->maxSpsUlHARQTx = ulUeCtx_p->maxUlHARQTx;
    }
    else
    {
         spsUlInfo_p->maxSpsUlHARQTx = MAX_SPS_HARQ_RETX_NUM;
    }
    /* CA Stats Changes Start */
    /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
            getCurrentTick(), __LINE__,internalCellIndex,delayToPhy,sizeof(spsCtx_p),cause,
            state,ueIndex, __func__, "UlStateMachine");
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause,  0, internalCellIndex); 
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : ulSpsResRsvReq
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Configured
 *                  state oe Deactivated state and First Non-Zero Q load/ voice
 *                  packet is received.
 ********************************************************************************/
MacRetType ulSpsResRsvReq( UInt16 ueIndex, SpsStates state, SpsCause cause, 
        /*CA Changes start  */
        ULUEContext *ulUeCtx_p, void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    LP_SpsUlInfo      spsUlInfo_p      = PNULL;
    LP_MacULSpsResReq spsUlResReq_p    = PNULL;


    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;

    /*!
     * \code 
     * Change the SPS state to SPS_RES_RSV_REQ_INITIATED.
     * Enqueue node to PUSCH for resource reservation in ulSpsResRsvQ_g.
     * \endcode
     * */
    spsUlResReq_p = (LP_MacULSpsResReq)getMemFromPool( 
            sizeof(MacULSpsResReq), PNULL );
    if( PNULL == spsUlResReq_p )
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacULSpsResReq),delayToPhy,sizeof(spsCtx_p),cause,
                state,0, __FILE__, __func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
	    /* Klockwork warning Changes Start */
	    return MAC_FAILURE;
	    /* Klockwork warning Changes End */
    }

    /* Fill the structure for PUSCH and enqueue in ulSpsResRsvQ_g */
    spsUlResReq_p->ueIndex = ueIndex;
    spsUlResReq_p->requestType = MAC_SPS_RES_RSV_REQ_ACT;
    spsUlResReq_p->avgRlcSduSize = spsUlInfo_p->avgRlcSduSize;

    /* Set the state accordingly */
    spsUlInfo_p->spsState = SPS_RES_RSV_REQ_INITIATED;
    /* Enqueue into ulSpsResRsvQ_g */
    /*CA Changes start  */
    /* + Coverity Id - 32613 */
    if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsResRsvQ_gp[internalCellIndex], spsUlResReq_p  ))
    {
        freeMemPool(spsUlResReq_p);
    }
    /* - Coverity Id - 32613 */

    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( UL_SPS_RES_RSV_REL_Q,  
            QCOUNT_MAC_UL_SPS_Q( ulSpsResRsvQ_gp[internalCellIndex] ), 
            internalCellIndex );
    /*CA Changes end  */

    /* Update Stats */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause,  0, internalCellIndex); 
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : ulSpsActivationPuschSuccess
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called when PUSCH SUCCESS received in
 *                  SPS_RES_RSV_REQ_INITIATED.
 ********************************************************************************/
MacRetType ulSpsActivationPuschSuccess( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p, 
        /*CA Changes start  */
        UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(state)
    LTE_GCC_UNUSED_PARAM(cause)
    LTE_GCC_UNUSED_PARAM(delayToPhy)
    /* +- SPR 17777 */
    /* + SPS_TDD_Changes */
    UInt16               nextTTIIndex   = 0;
    SInt8                subframe_offset =0;
    /* - SPS_TDD_Changes */
    MacRetType           retVal         = MAC_SUCCESS;
    LP_SpsUlInfo         spsUlInfo_p    = PNULL;
    LP_MacULSpsStatus    spsUlStatus_p  = (LP_MacULSpsStatus)spsCtx_p;
    LP_SpsULStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContextPeriodicity20_p  = PNULL;
    UInt8 lcId                           = INVALID_LCID;
#ifdef KPI_STATS
    UInt8 qci = 0;
#endif

    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
#endif
    /* - SPS_TDD_Changes */

    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;
    
    /* Get the TTI index in which SPS activation initiated */
    /*CA Changes start  */
    ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + spsUlStatus_p->index;
    /*CA Changes end  */
    /* + SPS_TDD_Changes */
    /* If twoIntervalConfig is enabled, get the subFrameoffset */
#ifdef TDD_CONFIG
    if(TRUE == spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
    {
        subframe_offset = getSubFrameOffsetForSpsTDD((spsUlStatus_p->index % MAX_SUBFRAME),internalCellIndex);
    }
#endif
    nextTTIIndex = (spsUlStatus_p->index + subframe_offset + SPS_INTERVAL_20) % ulSpsInterval ;
    ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex;
    /* - SPS_TDD_Changes */

    /*!
     * \code 
     * Put entry in SPS DCI List, SDL if number of DCIs has not reached max 
     * value.
     * Change the SPS state to SPS_ACTIVATION_INITIATED.
     * \endcode
     * */
    do
    {
        /* Check if max number of DCIs that can be scheduled has reached */

        /* + SPS_TDD_Changes */
        retVal = checkSpsDciPerTTI(
#ifdef TDD_CONFIG
                subframe_offset,
                spsUlStatus_p->index,
                ulSpsInterval,
#endif
                ttiContext_p,
                spsCtx_p,internalCellIndex);
        if(MAC_FAILURE == retVal)
        {

            LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
                    cause,  0,internalCellIndex );
            /* Update Stats */
            LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
                    ttiContextPeriodicity20_p, getCurrentTick(), 
                    spsUlInfo_p->spsUlSetupInfo_p->\
                    semiPersistentSchedIntervalUL, internalCellIndex );
            break;
        }
        /* - SPS_TDD_Changes */

        /* Check if interval is of 20 ms, then next SPS occasion entry in SDL
         * has reached max value */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[spsUlInfo_p->spsUlSetupInfo_p->\
                semiPersistentSchedIntervalUL] )
        {
            /* + SPS_TDD_Changes */
            retVal = checkSpsDciPerTTI(
#ifdef TDD_CONFIG
                    subframe_offset,
                    nextTTIIndex,
                    ulSpsInterval,
#endif
                    ttiContextPeriodicity20_p,
                    spsCtx_p,internalCellIndex);
            if(MAC_FAILURE == retVal)
            {
                /* Update Stats */
                LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
                        cause,  0, internalCellIndex); 

                LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
                        ttiContextPeriodicity20_p, getCurrentTick(), 
                        spsUlInfo_p->spsUlSetupInfo_p->\
                        semiPersistentSchedIntervalUL, internalCellIndex);
                break;
            }
            /* - SPS_TDD_Changes */
        }

        /* Put entry in SDL and update the TTI Context Map */
        ulAddUeInSdl( 
                /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                subframe_offset,
                spsUlStatus_p->index,
                ulSpsInterval,
#endif
                /* - SPS_TDD_Changes */
                ueIndex, 
                ttiContext_p,
                ttiContextPeriodicity20_p, 
                spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
                ,internalCellIndex
#endif
                );
        /* CA Stats Changes End */

        spsUlInfo_p->numDciAttempts = 0;

        /* Save the tti index and allocated RBs returned for SDL */
        spsUlInfo_p->actTtiIndex = spsUlStatus_p->index;
        spsUlInfo_p->ulResAllocInfo.allocatedRB = spsUlStatus_p->numRbs;
        spsUlInfo_p->spsOpp = spsUlStatus_p->spsOpp;

        /* Set the state accordingly */
        spsUlInfo_p->spsState = SPS_ACTIVATION_INITIATED;
        /* Generate Delete trigger to QOS strategy */
        /* Discussion to be done */
        if(UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->initParams_p->ulSchdConfig.ulSchedulerStrategy)
        {
            lcId = spsUlInfo_p->lcId;
            if (TRUE == ulUeCtx_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isPresentInEligibleList)
            {
#ifdef KPI_STATS
                qci = ulUeCtx_p->lcInfo[lcId].ulQosInfo.QCI;
                if (qci)
                {
                    /* SPR 10925 Start */
                    /* SPR 16548 fix start */
                    if ((ulUeCtx_p->qciInfoFlag[--qci]) &&
                            ( gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci] > 0 ))
                    {
                        /* SPR 16548 fix end */
                        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQCIUL[qci]--;
                        ulUeCtx_p->qciInfoFlag[qci] = 0;
                    }
                    /* SPR 10925 End */
                }   
#endif
                /* Delete the node from the Eligible List */
                listDeleteNode(&(ulUeCtx_p->ulEligibleLcList),
                            &(ulUeCtx_p->lcInfo[lcId].ulQosInfo.
                                ulQosStrategyLCNode_p->eligibleLcNodeAnchor));

                if(TRUE == ulUeCtx_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isLCNodePresent)
                {
                    /*LC of the UE in the prioritized LC List*/
                    ulQosDeleteEntryFromPrioritisedLcTree(
                          ulUeCtx_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p,
                          /*CA Changes start  */
                          internalCellIndex);
                          /*CA Changes end  */
                    ulUeCtx_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isLCNodePresent = FALSE;
                }

                ulUeCtx_p->lcInfo[lcId].ulQosInfo.
                    ulQosStrategyLCNode_p->isPresentInEligibleList = FALSE;
                /*debug stats*/
                LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(
                            ulUeCtx_p->ueIndex);

                /*debug stats*/
                LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(
                            ulUeCtx_p->ueIndex);
            }
        }
        /* CA Stats Changes Start */
        /* Update Stats */
        LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
                cause, spsUlInfo_p->numDciAttempts, internalCellIndex);
        /* CA Stats Changes End */
    } while(0);
    
    return retVal;
}

/********************************************************************************
 * Function Name  : ulSpsActivationPuschFail
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called when PUSCH FAILURE received in
 *                  SPS_RES_RSV_REQ_INITIATED.
 ********************************************************************************/
MacRetType ulSpsActivationPuschFail( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p, 
        /*CA Changes start  */
        UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    LP_SpsUlInfo       spsUlInfo_p    = PNULL;

    /*!
     * \code 
     * Change the SPS state to SPS_DEACTIVATED.
     * \endcode
     * */
    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;
        
    /* Set the state accordingly */
    spsUlInfo_p->spsState = SPS_DEACTIVATED;
    

    /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
            getCurrentTick(), __LINE__,internalCellIndex,delayToPhy,sizeof(spsCtx_p),cause,
            state,ueIndex, __func__, "UlStateMachine");
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause, 0, internalCellIndex); 
    /* CA Stats Changes End */
   
    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : ulSpsActivated
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Activation
 *                  Initiated state and First ACK/NACK received.
 ********************************************************************************/
MacRetType ulSpsActivated( UInt16 ueIndex, SpsStates state, SpsCause cause, 
        /*CA Changes start  */
        ULUEContext *ulUeCtx_p, void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(state)
    LTE_GCC_UNUSED_PARAM(cause)
    LTE_GCC_UNUSED_PARAM(delayToPhy)
    /* +- SPR 17777 */
    MacRetType                   retVal        = MAC_SUCCESS;
    LP_SpsUlInfo                 spsUlInfo_p   = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContextPeriodicity20_p = PNULL;
    /* + SPS_TDD_Changes */
    SInt8                        subframe_offset   = 0;
    UInt16                       ttiIndex          = 0;
    UInt16                       nextTTIIndex      = 0;
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
#endif
    /* - SPS_TDD_Changes */

    /*!
     * \code 
     * Remove entry from SPS DCI List, SDL.
     * Put entry in PUSCH occasion list.
     * Change the SPS state to SPS_ACTIVATED.
     * \endcode
     * */
    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;

    ttiIndex = spsUlInfo_p->actTtiIndex;
    
#ifdef FDD_CONFIG
    UInt8 bundleTick = 0;
    UInt8 numofTicks = 1;
    if(ulUeCtx_p->ttiBundlingEnabled)
    {
        numofTicks = TTI_BUNDLE_SIZE;
    }
#endif    
    /* Get the TTI index in which SPS activation initiated */
    /*CA Changes start  */
    ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + ttiIndex;
    /*CA Changes end  */
    /* + SPS_TDD_Changes */
    /* If twoIntervalConfig is enabled, get the subFrameoffset */
#ifdef TDD_CONFIG
    if(TRUE == spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
    {
        subframe_offset = getSubFrameOffsetForSpsTDD((ttiIndex % MAX_SUBFRAME),internalCellIndex);
    }
#endif
    nextTTIIndex = (ttiIndex + SPS_INTERVAL_20 + subframe_offset ) % ulSpsInterval ;
    ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex;
    /* - SPS_TDD_Changes */

    do
    {
        /* Check if max number of DCIs that can be scheduled has reached */
        /* + SPS_TDD_Changes */
        retVal = checkSpsOccPerTTI(
#ifdef TDD_CONFIG
                subframe_offset,
                ttiIndex,
                ulSpsInterval,
#endif
                ttiContext_p, spsCtx_p ,internalCellIndex );
        if(retVal == MAC_FAILURE)
        {
            /* Update Stats */
            LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
                    ttiContextPeriodicity20_p, getCurrentTick(), 
                    spsUlInfo_p->spsUlSetupInfo_p->\
                    semiPersistentSchedIntervalUL, internalCellIndex);
            break;
        }
        /* - SPS_TDD_Changes */

        /* Check if interval is of 20 ms, then next SPS occasion entry in SDL
         * has reached max value */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[spsUlInfo_p->spsUlSetupInfo_p->\
                                               semiPersistentSchedIntervalUL] )
        {
            /* + SPS_TDD_Changes */
            retVal = checkSpsOccPerTTI(
#ifdef TDD_CONFIG
                    subframe_offset,
                    nextTTIIndex,
                    ulSpsInterval,
#endif
                    ttiContextPeriodicity20_p,
                    spsCtx_p,
                    internalCellIndex);
            if(retVal == MAC_FAILURE)
            {
                /* Update Stats */
                LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
                        ttiContextPeriodicity20_p, getCurrentTick(), 
                        spsUlInfo_p->spsUlSetupInfo_p->\
                        semiPersistentSchedIntervalUL, internalCellIndex);
               break;
            }
	    /* - SPS_TDD_Changes */
        }
    
        /* CA Stats Changes Start */
        /* Delete SDL node from SDL and update TTI Context*/
        /* +- SPR 17777 */
        /* + SPS_TDD_Changes */
        ulDeleteUeFromSdl(
#ifdef TDD_CONFIG
                subframe_offset,
                ulSpsInterval,
                ttiIndex,
#endif
                ttiContext_p, 
                ttiContextPeriodicity20_p, 
                spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
                ,internalCellIndex
#endif
                );
        /* - SPS_TDD_Changes */
        /* +- SPR 17777 */
        
        /* Put entry in occasion list and update TTI context */
        /* + SPS_TDD_Changes */
        ulAddUeInOccasionList(
#ifdef TDD_CONFIG
                subframe_offset,
                ulSpsInterval,
#endif
                ueIndex,
                ttiContext_p, 
                ttiContextPeriodicity20_p, 
                spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
                ,internalCellIndex
#endif
                );
        /* - SPS_TDD_Changes */
        /* CA Stats Changes End */
#ifdef FDD_CONFIG
        /* numULSpsOccasionPerTti and numULSpsTTIBUePerTti will be incremented in all 4 ticks 
        whereas ocasion node is added only once for one occasion */
        for(bundleTick = 1; bundleTick < numofTicks ; bundleTick++)
        {
            /* + SPS_TDD_Changes */
            /*CA Changes start  */
            (spsULStrTTIMap_gp[internalCellIndex] + ((ttiIndex + bundleTick) % ulSpsInterval))->numULSpsOccasionPerTti++;
            (spsULStrTTIMap_gp[internalCellIndex] + ((ttiIndex + bundleTick) % ulSpsInterval))->numULSpsTTIBUePerTti++;
            /*CA Changes end  */


            if( SPS_INTERVAL_20 == spsIntervalMap_g[spsUlInfo_p->spsUlSetupInfo_p->\
                    semiPersistentSchedIntervalUL] )
            {
                /*CA Changes start  */
                (spsULStrTTIMap_gp[internalCellIndex] + ((ttiIndex + SPS_INTERVAL_20 +bundleTick)\
                 % ulSpsInterval))->numULSpsOccasionPerTti++;
                (spsULStrTTIMap_gp[internalCellIndex] + ((ttiIndex + SPS_INTERVAL_20 +bundleTick)\
                 % ulSpsInterval))->numULSpsTTIBUePerTti++;
                /*CA Changes end  */

            }
            /* - SPS_TDD_Changes */

        }
#endif        
        /* Set the state accordingly */
        spsUlInfo_p->spsState = SPS_ACTIVATED;
    } while( 0 );

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return retVal;
}

/********************************************************************************
 * Function Name  : ulSpsMaxDciAttemptReached
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Activation
 *                  Initiated state and DCI Attempts have reached Maximum value.
 ********************************************************************************/
MacRetType ulSpsMaxDciAttemptReached( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p,
        /*CA Changes start  */
        UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    UInt8                tempHarqId           = INVALID_HARQ_ID;
    LP_SpsUlInfo         spsUlInfo_p    = PNULL;
    LP_MacULSpsResReq    spsUlResRelReq_p = PNULL;
    ULHarqInfo        *tempULHarqInfo_p = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContextPeriodicity20_p = PNULL;
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
#endif
    SInt8 subFrameoffset = 0;
    /* - SPS_TDD_Changes */

    /*!
     * \code 
     * Delete entry from SDL and decrement numUlSpsDciPerTti.
     * Enqueue node to PUSCH for resource release in ulSpsResRelQ_g.
     * Change the SPS state to SPS_DEACTIVATED.
     * \endcode
     * */
    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;

    /* Get the TTI index in which SPS activation initiated */
    /*CA Changes start  */
    ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + spsUlInfo_p->actTtiIndex;
    /*CA Changes end  */
    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    if (TRUE == spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
    {
        subFrameoffset = getSubFrameOffsetForSpsTDD((spsUlInfo_p->actTtiIndex % MAX_SUBFRAME),internalCellIndex);
    }
#endif
    ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + 
        ((spsUlInfo_p->actTtiIndex + subFrameoffset + SPS_INTERVAL_20) % ulSpsInterval);
    /* - SPS_TDD_Changes */

    /*! \code
     * Free HARQ process ID reserved for SPS Activation/Deactivation
     * as per 36.213 (Table 9.2-1) as SPS cannot be activated and
     * MAX_DCI_ATTEMPT has reached.
     * \endcode
     * */

    tempHarqId =  spsUlInfo_p->hpId[0].harqId;
    if(INVALID_HARQ_ID != tempHarqId)
    {
        tempULHarqInfo_p = &(ulUeCtx_p->ulresInfo[tempHarqId]);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,ulUeCtx_p->ueIndex,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
            updateTTIHarqMap(tempHarqId,tempULHarqInfo_p,ulUeCtx_p->ttiHarqMap);
#endif
        spsUlInfo_p->hpId[0].harqId = INVALID_HARQ_ID;
    }
    tempHarqId =  spsUlInfo_p->hpId[1].harqId;
    if(INVALID_HARQ_ID != tempHarqId)
    {
        tempULHarqInfo_p = &(ulUeCtx_p->ulresInfo[tempHarqId]);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,ulUeCtx_p->ueIndex,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
            updateTTIHarqMap(tempHarqId,tempULHarqInfo_p,ulUeCtx_p->ttiHarqMap);
#endif
        spsUlInfo_p->hpId[1].harqId = INVALID_HARQ_ID;
    }
    /* +- SPR 17777 */
    else
    {
        LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                getCurrentTick(), __LINE__,delayToPhy,sizeof(spsCtx_p),cause,
                state,ueIndex,0, __func__, "UlStateMachine");
    }
    /* +- SPR 17777 */

    /* CA Stats Changes Start */
    /* Delete SDL node from SDL and update TTI Context*/
    /* +- SPR 17777 */
    /* + SPS_TDD_Changes */
    ulDeleteUeFromSdl(
#ifdef TDD_CONFIG
            subFrameoffset,
            ulSpsInterval,
            spsUlInfo_p->actTtiIndex,
#endif
            ttiContext_p,
            ttiContextPeriodicity20_p,
            spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
            ,internalCellIndex
#endif
            );
    /* - SPS_TDD_Changes */
    /* CA Stats Changes End */

    /* Allocate memory for Resource Release Request */
    spsUlResRelReq_p = (LP_MacULSpsResReq)getMemFromPool( 
            sizeof(MacULSpsResReq), PNULL );
    if( PNULL == spsUlResRelReq_p )
    {
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacULSpsResReq), 0,0,0,
                0,0, __FILE__, __func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
	    /* Klockwork warning Changes Start */
	    return MAC_FAILURE;
	    /* Klockwork warning Changes End */
    }

    /* Fill the structure for PUSCH for release request */
    spsUlResRelReq_p->ueIndex = ueIndex;
    spsUlResRelReq_p->requestType = MAC_SPS_RES_REL_REQ;

    /* Enqueue into ulSpsResRelQ_g */
    /*CA Changes start  */
    /* + Coverity Id - 32612 */
    if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex], spsUlResRelReq_p ))
    {
        freeMemPool(spsUlResRelReq_p);
    }
    /* - Coverity Id - 32612 */

    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( UL_SPS_RES_RSV_REL_Q,  
            QCOUNT_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex] ), 
            internalCellIndex);
    /* CA Stats Changes End */
    /*CA Changes end  */
    /* Set the state accordingly */
    spsUlInfo_p->spsState = SPS_DEACTIVATED;

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : ulSpsMaxDciAttemptReachedForDeact
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Deactivation
 *                  Initiated state and DCI Attempts have reached Maximum value.
 ********************************************************************************/
MacRetType ulSpsMaxDciAttemptReachedForDeact( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p,
        /*CA Changes start  */
        UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    UInt8                tempHarqId          = INVALID_HARQ_ID;
    LP_SpsUlInfo         spsUlInfo_p    = PNULL;
    ULHarqInfo        *tempULHarqInfo_p = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContextPeriodicity20_p = PNULL;
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
#endif
    /* - SPS_TDD_Changes */

    /*!
     * \code 
     * Delete entry from SDL and decrement numUlSpsDciPerTti.
     * Change the SPS state to SPS_DEACTIVATED.
     * \endcode
     * */
    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;

    /* Get the TTI index in which SPS activation initiated */
    /*CA Changes start  */
    ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + spsUlInfo_p->ttiIndex;
    /*CA Changes end  */
    /* + SPS_TDD_Changes */
    ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + 
         ((spsUlInfo_p->ttiIndex + SPS_INTERVAL_20) % ulSpsInterval);

    /*! \code
     * Free HARQ process ID  reserved for SPS Activation/Deactivation
     * as per 36.213 (Table 9.2-1) as SPS cannot be activated and
     * MAX_DCI_ATTEMPT has reached.
     * \endcode
     * */
    /* CA Stats Changes Start */
    /* Delete SDL node from SDL and update TTI Context*/
    /* +- SPR 17777 */
    ulDeleteUeFromSdl(
#ifdef TDD_CONFIG
            0,
            ulSpsInterval,
            spsUlInfo_p->ttiIndex,
#endif
            ttiContext_p,
            ttiContextPeriodicity20_p,
            spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
            ,internalCellIndex
#endif
            );
    /* - SPS_TDD_Changes */
    /* CA Stats Changes End */

    tempHarqId =  spsUlInfo_p->hpId[0].harqId;
    if(INVALID_HARQ_ID != tempHarqId)
    {
        tempULHarqInfo_p = &(ulUeCtx_p->ulresInfo[tempHarqId]);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,ulUeCtx_p->ueIndex,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
            updateTTIHarqMap(tempHarqId,tempULHarqInfo_p,ulUeCtx_p->ttiHarqMap);
#endif
        spsUlInfo_p->hpId[0].harqId = INVALID_HARQ_ID;
    }
    tempHarqId =  spsUlInfo_p->hpId[1].harqId;
    if(INVALID_HARQ_ID != tempHarqId)
    {
        tempULHarqInfo_p = &(ulUeCtx_p->ulresInfo[tempHarqId]);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,ulUeCtx_p->ueIndex,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
            updateTTIHarqMap(tempHarqId,tempULHarqInfo_p,ulUeCtx_p->ttiHarqMap);
#endif
        spsUlInfo_p->hpId[1].harqId = INVALID_HARQ_ID;
    }
    /* +- SPR 17777 */
    else
    {
        LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                getCurrentTick(), __LINE__,delayToPhy,sizeof(spsCtx_p),cause,
                state,ueIndex,0, __func__, "UlStateMachine");
    }
  
    /* Store Deactivation tti Index for SDL deletion */
    /* + SPS_TDD_Changes */
    spsUlInfo_p->ttiIndex = ulSpsInterval;
    /* - SPS_TDD_Changes */
 
    /* Set the state accordingly */
    spsUlInfo_p->spsState = SPS_DEACTIVATED;

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */
    
    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : ulSpsDeactInit
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Activation
 *                  state and number of Empty Transmissions reached max value.
 ********************************************************************************/
MacRetType ulSpsDeactInit( UInt16 ueIndex, SpsStates state, SpsCause cause, 
        /*CA Changes start  */
        ULUEContext *ulUeCtx_p, void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */

{
    UInt16              ttiIndex    = 0;
    UInt8               lcId        = 0;
    MacRetType          retVal      = MAC_SUCCESS;
    ULQosInfo           *qosInfo_p  = PNULL;
    LP_SpsUlInfo        ulSpsInfo_p = PNULL;
    LP_MacULSpsResReq   spsUlResRelReq_p = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContextPeriodicity20_p  = PNULL;    
    /* + SPS_TDD_Changes */
    SInt8               subframe_offset = 0;
    UInt16              nextTTIIndex    = 0;
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#else 
    UInt8                        subFrameNum         = 0;
    UInt8                        ulSubFrame          = 0;
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
    UInt32 ulTTI = 0;
    UInt32 sysFrameNum = (MODULO_ONEZEROTWOFOUR(getCurrentTick() / MAX_SUB_FRAME));
    UInt32 ulSysFrameNum    = sysFrameNum;
#endif
    /* - SPS_TDD_Changes */
    
    /*! \code
     * Remove Entry from the Occasion List.
     * Decrement numUlSpsOccasionPerTti value.
     * Send Resource Release Request to PUSCH in ulSpsResRelQ_g.
     * Put the entry in SDL.
     * Increment numUlSpsDciPerTti.
     * Change the State to SPS_DEACTIVATION_INITIATED.
     * \endcode
     * */       
    ulSpsInfo_p = &ulUeCtx_p->spsUlInfo;

/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
	ttiIndex = (getCurrentTick() + delayToPhy) % ulSpsInterval;
#else
	subFrameNum = (getCurrentTick() + delayToPhy) % MAX_SUBFRAME;
	ulSubFrame = (subFrameNum + getExactULSubFrame(subFrameNum,internalCellIndex))% MAX_SUBFRAME;
	if (ulSubFrame < subFrameNum)
	{
			ulSysFrameNum++;
			if (MAX_SYS_FRAME_NUM < ulSysFrameNum)
			{
					ulSysFrameNum = 0;
			}
	}
	ulTTI = ulSysFrameNum * MAX_SUBFRAME + ulSubFrame;
	ttiIndex = ulTTI % ulSpsInterval;
#endif
/* - SPS_TDD_Changes */

    /* SPR 19038 Fix Start */
    /* Check if UE is not in DRX or Meas Gap */
    if( DRX_STATE_ON == ulUeCtx_p->drxCurrentState ||
            (ulUeCtx_p->spsUlInfo.ueMeasGapTTI[ttiIndex] == MEAS_GAP_ON))
    {
        LOG_MAC_WARNING( MAC_UL_SPS_STRATEGY, "UE[%d] is in DRX/meas gap "
                "state, unable to initiate Deactivation, ttiIndex[%d] "
                "DRXState[%d] MeasGapTTI[%d]", ueIndex, ttiIndex, 
                ulUeCtx_p->drxCurrentState,
                ulUeCtx_p->spsUlInfo.ueMeasGapTTI[ttiIndex] );
        return MAC_FAILURE;
    }
    /* SPR 19038 Fix End */

#ifdef FDD_CONFIG
    UInt8 bundleTick = 0;
    UInt8 bundleIndex = 0;
    UInt8 currentHarqId = 0;
    if(ulUeCtx_p->ttiBundlingEnabled)
    {
        /*for sending DCI for SPS_DEATIVATION check whether HARQ Map is free at +4,+5,+6,+7
        where n is current Tick */
        for (bundleTick = 0 ; bundleTick < TTI_BUNDLE_SIZE; bundleTick++)
        {
            bundleIndex = (ttiIndex + bundleTick + FDD_HARQ_OFFSET) %
                TTI_BUNDLING_RTT ;
                currentHarqId = MODULO_EIGHT(ttiIndex + bundleTick + FDD_HARQ_OFFSET);
            /* If any HARQ process is Busy and that is due to SPS occasion then ignore that
                HARQ busy and schedule DCI for DEACTIVATION as SPS HQRQ is not used after state chnages to 
                SPS_DEACTIVATED but kept busy until SPS is DEACTIVATED */
                /*required in 20ms SPS periodicity and SPS is activated due to TTI Bundling enabled */
            if(( currentHarqId  == MODULO_EIGHT(ulSpsInfo_p->actTtiIndex + FDD_HARQ_OFFSET )) ||
                  (currentHarqId == ( MODULO_EIGHT( (ulSpsInfo_p->actTtiIndex + FDD_HARQ_OFFSET) + SPS_INTERVAL_20)) ) )
             {
                continue;
             }
            if( TTI_MAP_FREE != ulUeCtx_p->ttiHarqMap[bundleIndex] )
            {
                return   MAC_FAILURE;
            }
         }
    }
#endif     
    /* Get the TTI index in which SPS deactivation initiated */
    /*CA Changes start  */
    ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + ttiIndex;
    nextTTIIndex = (ttiIndex + SPS_INTERVAL_20) % ulSpsInterval ; 
    /* + SPS_TDD_Changes */
    ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 
    /* - SPS_TDD_Changes */
    /*CA Changes end  */

    do
    {
        /* Check if max number of DCIs that can be scheduled has reached */

        /* + SPS_TDD_Changes */
        retVal = checkSpsDciPerTTI(
#ifdef TDD_CONFIG
                0,
                ttiIndex,
                ulSpsInterval,
#endif
                ttiContext_p,
                spsCtx_p,internalCellIndex);
        if(MAC_FAILURE == retVal)
        {
            /* Update Stats */
            /* CA Stats Changes Start */
            LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
                    ttiContextPeriodicity20_p, getCurrentTick(), 
                    ulSpsInfo_p->spsUlSetupInfo_p->\
                    semiPersistentSchedIntervalUL, internalCellIndex);
            /* CA Stats Changes End */

            break;
        }
        /* + SPS_TDD_Changes */

        /* Check if interval is of 20 ms, then next SPS occasion entry in SDL
         * has reached max value */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[ulSpsInfo_p->spsUlSetupInfo_p->\
                semiPersistentSchedIntervalUL] )
        {
            /* + SPS_TDD_Changes */
            retVal = checkSpsDciPerTTI(
#ifdef TDD_CONFIG
                    0,
                    nextTTIIndex,
                    ulSpsInterval,
#endif
                    ttiContextPeriodicity20_p,
                    spsCtx_p,internalCellIndex);
            if(MAC_FAILURE == retVal)
            {
                /* Update Stats */
                LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
                        ttiContextPeriodicity20_p, getCurrentTick(), 
                        ulSpsInfo_p->spsUlSetupInfo_p->\
                        semiPersistentSchedIntervalUL, internalCellIndex);
                /* CA Stats Changes End */
                break;
            }
            /* - SPS_TDD_Changes */
        }

        /* Get the TTI index in which SPS was activated */
        /*CA Changes start  */
        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + ulSpsInfo_p->actTtiIndex;
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        if(TRUE == ulSpsInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
        {
            subframe_offset = getSubFrameOffsetForSpsTDD((ulSpsInfo_p->actTtiIndex % MAX_SUBFRAME),internalCellIndex);
        }
#endif
        nextTTIIndex = (ulSpsInfo_p->actTtiIndex + subframe_offset +SPS_INTERVAL_20) % ulSpsInterval ;
        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex;
        /* - SPS_TDD_Changes */
        /*CA Changes end  */
#ifdef FDD_CONFIG
                /* Allocate memory for Resource Release Request */
        spsUlResRelReq_p = (LP_MacULSpsResReq)getMemFromPool(              
                sizeof(MacULSpsResReq), PNULL );
        LP_MacULSpsOccList   occNode_p = PNULL; 
        occNode_p = (LP_MacULSpsOccList)ulSpsInfo_p->sdlOccNode_p;
        if( PNULL == spsUlResRelReq_p )
        {
            /* +- SPR 17777 */
            LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                    getCurrentTick(), __LINE__, sizeof(MacULSpsResReq),sizeof(spsCtx_p),cause,state,
                    0,0, __FILE__, __func__);
            ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            ltePanic("getMemFromPool fails %s", __func__);
	        /* cov_warning 61000 Changes Start */
	        return MAC_FAILURE;
	        /* cov_warning 61000 Changes End */
        }

        /* Fill the structure for PUSCH for release request */
        spsUlResRelReq_p->ueIndex = ueIndex;
        if(occNode_p->isTTIBundle)
        {
            spsUlResRelReq_p->requestType = MAC_SPS_TTIB_RES_REL_REQ;
        }
        else
        {
           spsUlResRelReq_p->requestType = MAC_SPS_RES_REL_REQ;
            
        }
        LTE_MAC_UT_LOG(LOG_INFO, UL_STRATEGY,
                "[%s] [%d] ueIndex %d requestType %d OccNodeTTIB %d ttiIndex %d actTtiIndex %d    \n",
                        __func__,getCurrentTick(),spsUlResRelReq_p->ueIndex,
                            spsUlResRelReq_p->requestType,occNode_p->isTTIBundle,
                            ttiIndex,ulSpsInfo_p->actTtiIndex);
        if(occNode_p->isTTIBundle)
        {
            /* If occasion node to be deleted is TTI Bundled that is for TTIB Ue,
            then check whether at the time of deletion if current tick and next three
            is SPS occasion,If yes then return FAilure and will be taken care in next tick */
            if((ttiIndex < ulSpsInfo_p->actTtiIndex + TTI_BUNDLE_SIZE) 
                    && (ttiIndex >= ulSpsInfo_p->actTtiIndex))
            {
             /* coverity 31812 Start */
         		freeMemPool(spsUlResRelReq_p);
             /* coverity 31812 End */
//Coverity_36235 fixed
                return MAC_FAILURE;        
            }
            /* for 20 ms Periodicity SPS */
            if(( SPS_INTERVAL_20 == spsIntervalMap_g[ulSpsInfo_p->spsUlSetupInfo_p->\
                        semiPersistentSchedIntervalUL] ) &&
                    (ttiIndex < (ulSpsInfo_p->actTtiIndex + SPS_INTERVAL_20 + TTI_BUNDLE_SIZE) % MAX_SPS_INTERVAL) && 
                    (ttiIndex >= (ulSpsInfo_p->actTtiIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL) )
            {
             /* coverity 31812 Start */
         		freeMemPool(spsUlResRelReq_p);
             /* coverity 31812 End */
//Coverity_36235 fixed
                return MAC_FAILURE;        
            }
        }
        UInt8 bundleTick  = 0;
        /* Decrement numULSpsOccasionPerTti and numULSpsTTIBUePerTti for consecutive 4 ticks
            for TTIB UE */
        if(occNode_p->isTTIBundle)
        {
            for( bundleTick = (TTI_BUNDLE_SIZE - 1);bundleTick > 0; bundleTick--)
            {
                /*CA Changes start  */
                /* + SPS_TDD_Changes */
                ((spsULStrTTIMap_gp[internalCellIndex] + ((ulSpsInfo_p->actTtiIndex + bundleTick) %
                 ulSpsInterval))->numULSpsOccasionPerTti)--;
                ((spsULStrTTIMap_gp[internalCellIndex] + ((ulSpsInfo_p->actTtiIndex + bundleTick) %
                 ulSpsInterval))->numULSpsTTIBUePerTti)--;
                /*CA Changes end  */

                if(SPS_INTERVAL_20 == spsIntervalMap_g[ulSpsInfo_p->spsUlSetupInfo_p->\
                        semiPersistentSchedIntervalUL] )
                {
                    /*CA Changes start  */
                    ((spsULStrTTIMap_gp[internalCellIndex] + ((ulSpsInfo_p->actTtiIndex + SPS_INTERVAL_20 + bundleTick) %
                     ulSpsInterval))->numULSpsOccasionPerTti)--;                                      
                    ((spsULStrTTIMap_gp[internalCellIndex] + ((ulSpsInfo_p->actTtiIndex + SPS_INTERVAL_20 + bundleTick) %
                     ulSpsInterval))->numULSpsTTIBUePerTti)--;                                      
                    /*CA Changes end  */
                }
                /* - SPS_TDD_Changes */
            }
        }

#endif       
        /* Delete the node from occasion list */
        /* CA Stats Changes Start */
        /* +- SPR 17777 */
        /* + SPS_TDD_Changes */
        ulDeleteUeFromOccasionList(
#ifdef TDD_CONFIG
                subframe_offset,
#endif
                ulSpsInterval,
                /* - SPS_TDD_Changes */
                ttiContext_p, 
                ttiContextPeriodicity20_p, 
                ulSpsInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
                ,internalCellIndex
#endif
                );
        /* CA Stats Changes End */

        /* Get the TTI index in which SPS deactivation initiated */
        /*CA Changes start  */
        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + ttiIndex;
        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + 
		/* + SPS_TDD_Changes */
                     ((ttiIndex + SPS_INTERVAL_20) % ulSpsInterval);
        /* - SPS_TDD_Changes */
        /*CA Changes end  */

        /* Store new Deactivation index as it needs to be processed in immediate
         * tick */
        ulSpsInfo_p->ttiIndex = ttiIndex;

        ulSpsInfo_p->numDciAttempts = 0;
#ifndef FDD_CONFIG
        /* Allocate memory for Resource Release Request */
        spsUlResRelReq_p = (LP_MacULSpsResReq)getMemFromPool(              
                sizeof(MacULSpsResReq), PNULL );
        if( PNULL == spsUlResRelReq_p )
        {
            /* +- SPR 17777 */
            LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                    getCurrentTick(), __LINE__, sizeof(MacULSpsResReq),
                    sizeof(spsCtx_p),cause,state,
                    /* +- SPR 17777 */
                    0,0, __FILE__, __func__);
            ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            ltePanic("getMemFromPool fails %s", __func__);
            return MAC_FAILURE; /*SPR 19339 fix*/
        }

        /* Fill the structure for PUSCH for release request */
        spsUlResRelReq_p->ueIndex = ueIndex;
        spsUlResRelReq_p->requestType = MAC_SPS_RES_REL_REQ;
#endif
        /* Enqueue into ulSpsResRelQ_g */
        /*CA Changes start  */
        /* + Coverity Id - 31812 */
        if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex], spsUlResRelReq_p ))
        {
            freeMemPool(spsUlResRelReq_p);
        }
        /* - Coverity Id - 31812 */

        /* Update Q Stats */
        /* CA Stats Changes Start */
        LTE_MAC_QUEUE_USED(UL_SPS_RES_RSV_REL_Q,  
                QCOUNT_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex] ),
                internalCellIndex);
        /* CA Stats Changes End */
        /*CA Changes end  */
 

        /* Change state to SPS_DEACTIVATION_INITIATED */
        ulSpsInfo_p->spsState = SPS_DEACTIVATION_INITIATED;

        /* Reset the Flag */
        ulSpsInfo_p->isUeUlWbMcsBelowThresh = LTE_FALSE;

        /* SPR 19038 Fix Start */
        ulSpsInfo_p->isUeMeasGapCollisionWithSpsOcc = LTE_FALSE;
        /* SPR 19038 Fix End */

        /* CA Stats Changes Start */
        /* Put entry in SDL and update the TTI Context Map */
        /* + SPS_TDD_Changes */
        /* In case of Deactivation subframe_offset shall be 0*/
        ulAddUeInSdl(
#ifdef TDD_CONFIG
                0,
                ttiIndex,
                ulSpsInterval,
#endif
                /* - SPS_TDD_Changes */
                ueIndex,
                ttiContext_p,
                ttiContextPeriodicity20_p, 
                ulSpsInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
                ,internalCellIndex
#endif
                );
        /* CA Stats Changes End */

        /* Generate Add trigger to QOS strategy */
        if(UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
              ulSchdConfig.ulSchedulerStrategy)
        {
            lcId = ulSpsInfo_p->lcId ;
            if (FALSE ==  ulUeCtx_p->lcInfo[lcId].ulQosInfo.ulQosStrategyLCNode_p->isPresentInEligibleList)
            {
                qosInfo_p =&ulUeCtx_p->lcInfo[lcId].ulQosInfo;

                /* Add the node in the Eligible List */
                listInsertNode(&(ulUeCtx_p->ulEligibleLcList),
                        &(qosInfo_p->ulQosStrategyLCNode_p->eligibleLcNodeAnchor));

                if(UL_QOS_NO_TRANSMISSION < ulUeCtx_p->scheduleFlag)
                {
                    /* calculate the weight of the LC and insert it in the 
                     * priortised LC list 
                     */

                    qosInfo_p->tokenPriority
                        = ulQosGetTknPriority(
                                qosInfo_p->scheduledToken[qosInfo_p->curTknWindowInd],
                                qosInfo_p->lcTknTable,
                                qosInfo_p->totSchdTknInWindow);


                    /*Calculate the aggregate weight of the LC*/
                    /*CA Changes start  */
                    ulQosCalcGbrLCWeightAndInsert(ulUeCtx_p,lcId,0,internalCellIndex);
                    /*CA Changes end  */
                }
                qosInfo_p->ulQosStrategyLCNode_p->isPresentInEligibleList = TRUE;
            }
        }

        /* CA Stats Changes Start */
        /* Update Stats */
        LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
                cause, ulSpsInfo_p->numDciAttempts, internalCellIndex);
        /* CA Stats Changes End */
    } while (0);

    return retVal;
}


/********************************************************************************
 * Function Name  : ulSpsDeactPdcchFail
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Deactivation
 *                  Initiated state and CCE Alloc failure is returned from PDCCH.
 ********************************************************************************/
MacRetType ulSpsDeactPdcchFail( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p, 
        /*CA Changes start  */
        UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(state)
    LTE_GCC_UNUSED_PARAM(cause)
    /* +- SPR 17777 */
    UInt8               numDciAttempts    = 0;
    /* Coverity 32350 Fix Start */
    MacRetType          retVal            = MAC_SUCCESS;
    /* Coverity 32350 Fix End */
    UInt16              ttiIndex          = 0;
    LP_SpsUlInfo        ulSpsInfo_p       = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContext_p = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;    
    /* + SPS_TDD_Changes */
    UInt16              nextTTIIndex          = 0;
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
#endif
    /* - SPS_TDD_Changes */

    /*! \code
     * if CCE Allocation fail is returned from PDCCH.
     *    - Retry at next available TTI.
     * \endcode
     * */
    ulSpsInfo_p = &ulUeCtx_p->spsUlInfo;

    do
    {
        /* + SPS_TDD_Changes */
        ttiIndex = (getCurrentTick() + delayToPhy) % ulSpsInterval;
        /* - SPS_TDD_Changes */
        numDciAttempts = ulSpsInfo_p->numDciAttempts;        

        /* Since current ttiIndex is same as ttiIndex stored in UE conext,
         * so break the loop for it to process in SDL processing. */
        if(( ttiIndex == ulSpsInfo_p->ttiIndex )
                /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                || (ttiIndex == ( ulSpsInfo_p->ttiIndex + SPS_INTERVAL_40) % ulSpsInterval)
#endif
          )
            /* - SPS_TDD_Changes */
        {
            /* CA Stats Changes Start */
            /* Update Stats */                
            LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
                    cause, ulSpsInfo_p->numDciAttempts, internalCellIndex);
            break;
        }

        /* Check if interval is of 20 ms, at next periodic interval as well */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[ulSpsInfo_p->
                spsUlSetupInfo_p->semiPersistentSchedIntervalUL] )
        {
            if(( ttiIndex == ( ulSpsInfo_p->ttiIndex + SPS_INTERVAL_20) % ulSpsInterval) 
                    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                    || (ttiIndex == ( ulSpsInfo_p->ttiIndex + SPS_INTERVAL_40 + SPS_INTERVAL_20) % ulSpsInterval)
#endif
              )
                /* - SPS_TDD_Changes */
            {
                /* Update Stats */                
                LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
                        cause, ulSpsInfo_p->numDciAttempts, internalCellIndex);
                /* CA Stats Changes End */
                break;
            }
        }
        /* Get the TTI index in which SPS Deactivation to be initiated */
        /*CA Changes start  */
        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + ttiIndex;
        /* + SPS_TDD_Changes */
        nextTTIIndex = ((ttiIndex + SPS_INTERVAL_20) % ulSpsInterval) ;
        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 
        /* - SPS_TDD_Changes */

        /* Check if max number of DCIs that can be scheduled has reached */
        /* + SPS_TDD_Changes */
        retVal = checkSpsDciPerTTI(
#ifdef TDD_CONFIG
                0,
                ttiIndex,
                ulSpsInterval,
#endif
                ttiContext_p,
                spsCtx_p , internalCellIndex);
        if(MAC_FAILURE == retVal)
        {
            /* Update Stats */                
            LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
                    cause, ulSpsInfo_p->numDciAttempts, internalCellIndex);

            LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
                    ttiContextPeriodicity20_p, getCurrentTick(), 
                    ulSpsInfo_p->spsUlSetupInfo_p->\
                    semiPersistentSchedIntervalUL, internalCellIndex);
            break;
        }
        /* - SPS_TDD_Changes */

        /* Check if interval is of 20 ms, then next SPS occasion entry in SDL
         * has reached max value */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[ulSpsInfo_p->
                spsUlSetupInfo_p->semiPersistentSchedIntervalUL] )
        {
            /* + SPS_TDD_Changes */
            retVal = checkSpsDciPerTTI(
#ifdef TDD_CONFIG
                    0,
                    nextTTIIndex,
                    ulSpsInterval,
#endif
                    ttiContextPeriodicity20_p, 
                    spsCtx_p ,internalCellIndex);
            if(MAC_FAILURE == retVal)
            {
                LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
                        cause, ulSpsInfo_p->numDciAttempts, internalCellIndex);

                LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
                        ttiContextPeriodicity20_p, getCurrentTick(), 
                        ulSpsInfo_p->spsUlSetupInfo_p->\
                        semiPersistentSchedIntervalUL, internalCellIndex);
                break;
            }
            /* - SPS_TDD_Changes */
        }

        /* Get the TTI index in which SPS Deactivation to be initiated */
        /*CA Changes start  */
        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + ulSpsInfo_p->ttiIndex;

        /* + SPS_TDD_Changes */
        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + 
            ((ulSpsInfo_p->ttiIndex + SPS_INTERVAL_20) % ulSpsInterval);
        /* - SPS_TDD_Changes */
        /*CA Changes end  */
        /* CA Stats Changes Start */
        /* Delete the node from SDL and update the TTI Context */
        /* +- SPR 17777 */
        ulDeleteUeFromSdl(
                /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                0,
                ulSpsInterval,
                ulSpsInfo_p->ttiIndex,
#endif
                ttiContext_p, 
                ttiContextPeriodicity20_p, 
                ulSpsInfo_p 
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
                ,internalCellIndex
#endif
                );
                /* - SPS_TDD_Changes */
        /* +- SPR 17777 */
        /* CA Stats Changes Start */

        /* Store new Deactivation index as it needs to be processed in 
         * immediate tick */
        ulSpsInfo_p->ttiIndex = ttiIndex;

        /* Get the TTI index in which SPS Deactivation to be initiated */
        /*CA Changes start  */
        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + ttiIndex;
        /* + SPS_TDD_Changes */
        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + 
            ((ttiIndex + SPS_INTERVAL_20) % ulSpsInterval);
        /* - SPS_TDD_Changes */
        /*CA Changes end  */

        ulSpsInfo_p->numDciAttempts = numDciAttempts;
        /* CA Stats Changes Start */
        /* Put entry in SDL and update the TTI Context Map */
        /* In case of Deactivation subframe_offset shall be 0*/
        /* + SPS_TDD_Changes */
        ulAddUeInSdl(
#ifdef TDD_CONFIG
                0,
                ttiIndex,
                ulSpsInterval,
#endif
                /* - SPS_TDD_Changes */
                ueIndex,
                ttiContext_p, 
                ttiContextPeriodicity20_p, 
                ulSpsInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
                ,internalCellIndex
#endif
                );

        /* Update Stats */
        LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
                cause, ulSpsInfo_p->numDciAttempts, internalCellIndex);
        /* CA Stats Changes End */
    } while( 0 );

    return retVal;
}

/********************************************************************************
 * Function Name  : ulSpsImplicitRelease
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called when implicit release takes place and UE
 *                  is in DRA.
 ********************************************************************************/
MacRetType ulSpsImplicitRelease( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, ULUEContext *ulUeCtx_p, void * spsCtx_p, 
        /*CA Changes start  */
        UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    LP_SpsUlInfo  spsUlInfo_p = PNULL;
    
    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;
    
    /* Change state of the UE to SPS_IDLE */
    spsUlInfo_p->spsState = SPS_IDLE;
    
    
    /* CA Stats Changes Start */
    /* Update Stats */
    /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
            getCurrentTick(), __LINE__,internalCellIndex,delayToPhy,sizeof(spsCtx_p),cause,
            state,ueIndex, __func__, "UlStateMachine");
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */
    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : ulSpsUeImplicitRelPuschResp
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care of use cases when SPS_RELEASE
 *                  triggered by L3.
 ********************************************************************************/
MacRetType ulSpsUeImplicitRelPuschResp( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, ULUEContext *ulUeCtx_p, 
        /*CA Changes start  */
        void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    LP_SpsUlInfo      spsUlInfo_p    = PNULL;
    LP_MacULSpsResReq spsUlResRelReq_p = PNULL;

    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;

    /* Allocate memory for Resource Release Request */
    spsUlResRelReq_p = (LP_MacULSpsResReq)getMemFromPool(   
                               sizeof(MacULSpsResReq), PNULL );
    if( PNULL == spsUlResRelReq_p )
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacULSpsResReq),delayToPhy,sizeof(spsCtx_p),cause,
                state,ueIndex, __func__, "UlStateMachine");
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
	    /* Klockwork warning Changes Start */
	    return MAC_FAILURE;
	    /* Klockwork warning Changes End */
    }
    
    /* Fill the structure for PUSCH for release request */
    spsUlResRelReq_p->ueIndex = ueIndex;
    spsUlResRelReq_p->requestType = MAC_SPS_RES_REL_REQ;
    
    /* Enqueue into ulSpsResRelQ_g */
    /*CA Changes start  */
    /* + Coverity Id - 32614 */
    if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex], spsUlResRelReq_p ))
    {
        freeMemPool(spsUlResRelReq_p);
    }
    /* - Coverity Id - 32614 */

    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( UL_SPS_RES_RSV_REL_Q,  
            QCOUNT_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex] ), 
            internalCellIndex);
    /* CA Stats Changes End */
    /*CA Changes end  */
    
    /* Change state of the UE to SPS_IDLE */
    spsUlInfo_p->spsState = SPS_IDLE;
    
    
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : ulSpsUeImplicitReleaseinSdlInDeactInit
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care of uese cases when implicit release
 *                  received from L3 and node is residing in SDL in deactivation
 *                  initiated state.
 ********************************************************************************/
MacRetType ulSpsUeImplicitReleaseinSdlInDeactInit( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, ULUEContext *ulUeCtx_p, 
        /*CA Changes start  */
        void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    LP_SpsUlInfo        spsUlInfo_p = PNULL;
    ULHarqInfo*   tempULHarqInfo_p = PNULL;    
    UInt8 tempHarqId = INVALID_HARQ_ID;
    LP_SpsULStrIntervalTTIContext ttiContext_p = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;    
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
#endif
    /* - SPS_TDD_Changes */

    /*! \code 
     * Call ulDeleteUeFromSdl.
     * Change the SPS state to SPS_IDLE.
     * \endcode
     * */
    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;
        
    /* Get the TTI index in which SPS Deactivation to be initiated */
    /*CA Changes start  */
    ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + spsUlInfo_p->ttiIndex;
    /* + SPS_TDD_Changes */
    ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + 
         ((spsUlInfo_p->ttiIndex + SPS_INTERVAL_20) % ulSpsInterval);
    /* - SPS_TDD_Changes */
    /*CA Changes end  */

    /* CA Stats Changes Start */
    /* Delete Node from SDL */
    /* +- SPR 17777 */
    /* + SPS_TDD_Changes */
    ulDeleteUeFromSdl(
#ifdef TDD_CONFIG
            0,
            ulSpsInterval,
            spsUlInfo_p->ttiIndex,
#endif
            /* - SPS_TDD_Changes */
            ttiContext_p, 
            ttiContextPeriodicity20_p, 
            spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
            ,internalCellIndex
#endif
            );
    /* CA Stats Changes End */

    tempHarqId =  spsUlInfo_p->hpId[0].harqId;
    if(INVALID_HARQ_ID != tempHarqId)
    {
        tempULHarqInfo_p = &(ulUeCtx_p->ulresInfo[tempHarqId]);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,ulUeCtx_p->ueIndex,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
            updateTTIHarqMap(tempHarqId,tempULHarqInfo_p,ulUeCtx_p->ttiHarqMap);
#endif
        spsUlInfo_p->hpId[0].harqId = INVALID_HARQ_ID;
    }
    tempHarqId =  spsUlInfo_p->hpId[1].harqId;
    if(INVALID_HARQ_ID != tempHarqId)
    {
        tempULHarqInfo_p = &(ulUeCtx_p->ulresInfo[tempHarqId]);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,ulUeCtx_p->ueIndex,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
            updateTTIHarqMap(tempHarqId,tempULHarqInfo_p,ulUeCtx_p->ttiHarqMap);
#endif
        spsUlInfo_p->hpId[1].harqId = INVALID_HARQ_ID;
    }
    /* +- SPR 17777 */
    else
    {
        LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                getCurrentTick(), __LINE__,tempHarqId,delayToPhy,sizeof(spsCtx_p),cause,
                state,ueIndex, __func__, "UlStateMachine");
    }

    /* Change state of the UE to SPS_IDLE */
    spsUlInfo_p->spsState = SPS_IDLE;
    
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : ulSpsUeImplicitReleaseinSdl
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care of uese cases when implicit release
 *                  received from L3 and node is residing in SDL i.e state is
 *                  either SPS_ACTIVATION_INITIATED or SPS_DEACTIVATION_INITIATED
 *                  or SPS_REACTIVATION_INITIATED.
 ********************************************************************************/
MacRetType ulSpsUeImplicitReleaseinSdl( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, ULUEContext *ulUeCtx_p, 
        /*CA Changes start  */
        void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    /* + SPS_TDD_Changes */
    SInt8               subFrameoffset = 0;
    /* - SPS_TDD_Changes */
    LP_SpsUlInfo        spsUlInfo_p = PNULL;
    LP_MacULSpsResReq   resRelReq_p = PNULL;
    ULHarqInfo*   tempULHarqInfo_p = PNULL;    
    UInt8 tempHarqId = INVALID_HARQ_ID;
    LP_SpsULStrIntervalTTIContext ttiContext_p = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;    
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
#endif
    /* - SPS_TDD_Changes */

    /*! \code 
     * Call ulDeleteUeFromSdl.
     * Send Resource release request to PUSCH in ulSpsResRelQ_g.
     * Change the SPS state to SPS_IDLE.
     * \endcode
     * */
    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;
        
    /* Get the TTI index in which SPS Deactivation to be initiated */
    /*CA Changes start  */
    ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + spsUlInfo_p->actTtiIndex;
    /*CA Changes end  */

    /* + SPS_TDD_Changes */
    /* Delete Node from SDL */
#ifdef TDD_CONFIG
    if(TRUE ==  spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
    {
        subFrameoffset = getSubFrameOffsetForSpsTDD((spsUlInfo_p->actTtiIndex % MAX_SUBFRAME),internalCellIndex );
    }
#endif
    ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] +
         ((spsUlInfo_p->actTtiIndex + subFrameoffset + SPS_INTERVAL_20) % ulSpsInterval);
    /* CA Stats Changes Start */
    /* +- SPR 17777 */
    ulDeleteUeFromSdl(
#ifdef TDD_CONFIG
            subFrameoffset,
            ulSpsInterval,
            spsUlInfo_p->actTtiIndex,
#endif
            ttiContext_p, 
            ttiContextPeriodicity20_p, 
            spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
            ,internalCellIndex
#endif
            );
    /* - SPS_TDD_Changes */
    /* CA Stats Changes End */

    /* Allocate memory for release request to PUSCH */
    resRelReq_p = (LP_MacULSpsResReq)getMemFromPool( 
                              sizeof(MacULSpsResReq), PNULL );
    if( PNULL == resRelReq_p )
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacULSpsResReq),delayToPhy,sizeof(spsCtx_p),cause,
                state,0, __FILE__, __func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
	    /* Klockwork warning Changes Start */
	    return MAC_FAILURE;
	    /* Klockwork warning Changes End */
    }
    /* Fill information for release of resources */
    resRelReq_p->ueIndex = ueIndex;
    resRelReq_p->requestType = MAC_SPS_RES_REL_REQ;

    tempHarqId =  spsUlInfo_p->hpId[0].harqId;
    if(INVALID_HARQ_ID != tempHarqId)
    {
        tempULHarqInfo_p = &(ulUeCtx_p->ulresInfo[tempHarqId]);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,ueIndex,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
            updateTTIHarqMap(tempHarqId,tempULHarqInfo_p,ulUeCtx_p->ttiHarqMap);
#endif
        spsUlInfo_p->hpId[0].harqId = INVALID_HARQ_ID;
    }
    tempHarqId =  spsUlInfo_p->hpId[1].harqId;
    if(INVALID_HARQ_ID != tempHarqId)
    {
        tempULHarqInfo_p = &(ulUeCtx_p->ulresInfo[tempHarqId]);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,ueIndex,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
            updateTTIHarqMap(tempHarqId,tempULHarqInfo_p,ulUeCtx_p->ttiHarqMap);
#endif
        spsUlInfo_p->hpId[1].harqId = INVALID_HARQ_ID;
    }

    /* Enqueue in PUSCH resource release request queue, ulSpsResRelQ_g */
    /*CA Changes start  */
    /* + Coverity Id - 32615 */
    if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex], resRelReq_p ))
    {
        freeMemPool(resRelReq_p);
    }
    /* - Coverity Id - 32615 */
    
    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( UL_SPS_RES_RSV_REL_Q,  
            QCOUNT_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex] ),
            internalCellIndex);
    /*CA Changes end  */
    
    /* Change state of the UE to SPS_IDLE */
    spsUlInfo_p->spsState = SPS_IDLE;
    
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : ulSpsImplicitReleaseinOccList
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care of uese cases when implicit release
 *                  received from L3 when in SPS_ACTIVATED state.
 ********************************************************************************/
MacRetType ulSpsImplicitReleaseinOccList( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, ULUEContext *ulUeCtx_p, 
        /*CA Changes start  */
        void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    LP_SpsUlInfo      spsUlInfo_p   = PNULL;
    LP_MacULSpsResReq resRelReq_p   = PNULL;
    ULHarqInfo*   tempULHarqInfo_p = PNULL;    
    UInt8 tempHarqId = INVALID_HARQ_ID;
    LP_SpsULStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContextPeriodicity20_p  = PNULL;    

    /* + SPS_TDD_Changes */
    SInt8 subframe_offset           = 0;
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
#endif
    /* - SPS_TDD_Changes */
    
    /*! \code 
     * Call ulDeleteUeFromOccasionList.
     * Send Resource release request to PUSCH in ulSpsResRelQ_g.
     * Change the SPS state to SPS_IDLE.
     * \endcode
     * */
    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;
    
    /* Get the TTI index in which SPS was activated */
    /*CA Changes start  */
    ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + spsUlInfo_p->actTtiIndex;
    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    if(TRUE == spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
    {
        subframe_offset = getSubFrameOffsetForSpsTDD((spsUlInfo_p->actTtiIndex % MAX_SUBFRAME),internalCellIndex);
    }
#endif
    ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] +
        ((spsUlInfo_p->actTtiIndex + subframe_offset + SPS_INTERVAL_20) % ulSpsInterval); 
    /* - SPS_TDD_Changes */
    /*CA Changes end  */

    /* Delete the node from occasion list */
#ifdef FDD_CONFIG
    /* TTIB_Code Start */
    UInt8 bundleTick  = 0;
    LP_MacULSpsOccList   occNode_p = PNULL;
    
    /* + SPS_TDD_Changes */
    UInt8 ttiIndex = (getCurrentTick() + delayToPhy) % ulSpsInterval;
    /* - SPS_TDD_Changes */
    resRelReq_p = (LP_MacULSpsResReq)getMemFromPool(
                          sizeof(MacULSpsResReq), PNULL );
    if( PNULL == resRelReq_p )
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacULSpsResReq),sizeof(spsCtx_p),state,0,
                0,0, __FILE__, __func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
	    /* Klockwork warning Changes Start */
	    return MAC_FAILURE;
	    /* Klockwork warning Changes End */
    }
    /* Fill information for release of resources */
    resRelReq_p->ueIndex = ueIndex;
    /* Fill the structure for PUSCH for release request */
    resRelReq_p->ueIndex = ueIndex;
    occNode_p = (LP_MacULSpsOccList)spsUlInfo_p->sdlOccNode_p;
    if(occNode_p->isTTIBundle)
    {
        resRelReq_p->requestType = MAC_SPS_TTIB_RES_REL_REQ;
    }
    else
    {
        resRelReq_p->requestType = MAC_SPS_RES_REL_REQ;

    }
    if(occNode_p->isTTIBundle)
    {
        if((ttiIndex < spsUlInfo_p->actTtiIndex + TTI_BUNDLE_SIZE) 
                && (ttiIndex >= spsUlInfo_p->actTtiIndex))
        {
             /* coverity 31813 Start */
		     freeMemPool(resRelReq_p);
             /* coverity 31813 End */
             return MAC_FAILURE;        
        }
        if(( SPS_INTERVAL_20 == spsIntervalMap_g[spsUlInfo_p->spsUlSetupInfo_p->\
                    semiPersistentSchedIntervalUL] ) &&
                /* + SPS_TDD_Changes */
                (ttiIndex < (spsUlInfo_p->actTtiIndex + SPS_INTERVAL_20 + TTI_BUNDLE_SIZE) % ulSpsInterval) && 
                (ttiIndex >= (spsUlInfo_p->actTtiIndex + SPS_INTERVAL_20) % ulSpsInterval) )
            /* - SPS_TDD_Changes */
        {
             /* coverity 31813 Start */
	        freeMemPool(resRelReq_p);
             /* coverity 31813 End */
            return MAC_FAILURE;        
        }
        for( bundleTick = (TTI_BUNDLE_SIZE - 1);bundleTick > 0; bundleTick--)
        {
            /*CA Changes start  */
            ((spsULStrTTIMap_gp[internalCellIndex] + ((spsUlInfo_p->actTtiIndex + bundleTick) %
            /* + SPS_TDD_Changes */
             ulSpsInterval))->numULSpsOccasionPerTti)--;
            ((spsULStrTTIMap_gp[internalCellIndex] + ((spsUlInfo_p->actTtiIndex + bundleTick) %
             ulSpsInterval))->numULSpsTTIBUePerTti)--;
            /*CA Changes end  */

            if(SPS_INTERVAL_20 == spsIntervalMap_g[spsUlInfo_p->spsUlSetupInfo_p->\
                    semiPersistentSchedIntervalUL] )
            {
                /*CA Changes start  */
                ((spsULStrTTIMap_gp[internalCellIndex] +
                 ((spsUlInfo_p->actTtiIndex + SPS_INTERVAL_20 + bundleTick) %
                 ulSpsInterval))->numULSpsOccasionPerTti)--;                                      
                ((spsULStrTTIMap_gp[internalCellIndex] + 
                 ((spsUlInfo_p->actTtiIndex + SPS_INTERVAL_20 + bundleTick) %
                 ulSpsInterval))->numULSpsTTIBUePerTti)--;                                      
                /*CA Changes end  */
            }
            /* - SPS_TDD_Changes */
        }
    }

    /* TTIB_Code End */
#endif

   
    /* CA Stats Changes Start */
    /* +- SPR 17777 */
    /* + SPS_TDD_Changes */
    ulDeleteUeFromOccasionList(
#ifdef TDD_CONFIG
            subframe_offset,
#endif
            ulSpsInterval,
            /* - SPS_TDD_Changes */
            ttiContext_p, 
            ttiContextPeriodicity20_p, 
            spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
            ,internalCellIndex
#endif
            );
    /* +- SPR 17777 */
    /* CA Stats Changes End */
#ifndef FDD_CONFIG
    /* Allocate memory for release request to PUSCH */
    resRelReq_p = (LP_MacULSpsResReq)getMemFromPool( 
                              sizeof(MacULSpsResReq), PNULL );
    if( PNULL == resRelReq_p )
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacULSpsResReq),delayToPhy,sizeof(spsCtx_p),state,
                0,0, __FILE__, __func__);
        /* +- SPR 17777 */
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
        return MAC_FAILURE; /*SPR 19339 fix*/
    }
    /* Fill information for release of resources */
    resRelReq_p->ueIndex = ueIndex;
    resRelReq_p->requestType = MAC_SPS_RES_REL_REQ;
#endif                
    /* Enqueue in PUSCH resource release request queue, ulSpsResRelQ_g */
    /*CA Changes start  */
    /* + Coverity Id - 31813 */
    if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex], resRelReq_p ))
    {
        freeMemPool(resRelReq_p);
    }
    /* - Coverity Id - 31813 */
    
    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( UL_SPS_RES_RSV_REL_Q,  
            QCOUNT_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex] ),
            internalCellIndex);
    /* CA Stats Changes End */
    /*CA Changes end  */
      
    tempHarqId =  spsUlInfo_p->hpId[0].harqId;
    if(INVALID_HARQ_ID != tempHarqId)
    {
        tempULHarqInfo_p = &(ulUeCtx_p->ulresInfo[tempHarqId]);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,ueIndex,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
            updateTTIHarqMap(tempHarqId,tempULHarqInfo_p,ulUeCtx_p->ttiHarqMap);
#endif
        spsUlInfo_p->hpId[0].harqId = INVALID_HARQ_ID;
    }
    tempHarqId =  spsUlInfo_p->hpId[1].harqId;
    if(INVALID_HARQ_ID != tempHarqId)
    {
        tempULHarqInfo_p = &(ulUeCtx_p->ulresInfo[tempHarqId]);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,ueIndex,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
         updateTTIHarqMap(tempHarqId,tempULHarqInfo_p,ulUeCtx_p->ttiHarqMap);
#endif
        spsUlInfo_p->hpId[1].harqId = INVALID_HARQ_ID;
    }

    if(MAX_EMPTY_TX == cause ) 
    {
        /* Change state of the UE to SPS_IDLE */
        spsUlInfo_p->spsState = SPS_DEACTIVATED;
    }
    else
    {
        /* Change state of the UE to SPS_IDLE */
        spsUlInfo_p->spsState = SPS_IDLE;
    }

    

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

    
/********************************************************************************
 * Function Name  : ulSpsNotValidEvent
 * Inputs         : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  ulUeCtx_p - UL UE Context pointer,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called when wrong cause is called in different
 *                  state and logs and ignore the call.
 ********************************************************************************/
MacRetType ulSpsNotValidEvent( UInt16 ueIndex, SpsStates state, SpsCause cause, 
        /*CA Changes start  */
        ULUEContext *ulUeCtx_p, void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    /* CA Stats Changes Start */
    /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_UL_STRATEGY_ERROR_ID, LOGERROR, MAC_UL_SPS_STRATEGY,
            getCurrentTick(),ueIndex,
            state,cause,sizeof(ulUeCtx_p),sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "Invalid event");
    /* Update stats */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, getCurrentTick() ,
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : processULSpsOccasion
 * Inputs         : currentGlobalTTITickCount - current Global Tick,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks for SPS occasion and calls to process
 *                  SDL and occasion list.
 ********************************************************************************/
 /* SPR 15909 fix start */
UInt32 processULSpsOccasion( tickType_t currentGlobalTTITickCount, 
/* SPR 15909 fix end */
        UInt32 delayToPhy,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    UInt8                        spsOccasionUeCount  = 0;
    UInt8                        sdlCount            = 0;
    UInt16                       index               = 0;
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#else 
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
    UInt32 ulTTI = 0;
    UInt32 sysFrameNum = (MODULO_ONEZEROTWOFOUR(currentGlobalTTITickCount / MAX_SUB_FRAME));
    UInt32 ulSysFrameNum    = sysFrameNum;
    UInt8                        subFrameNum         = 0;
    UInt8                        ulSubFrame          = 0;
#endif
    LP_SpsULStrIntervalTTIContext  spsUlStrIntervalTTI_p = PNULL;
#ifdef FDD_CONFIG
    UInt16                       prevIndex           = 0;
    LP_SpsULStrIntervalTTIContext  prevTtiCtx_p = PNULL;
#endif
    /* - SPS_TDD_Changes */

    /*! 
     * \code
     * - Find the Interval TTI Index. 
     * - Find the TTI Context
     * \endcode 
     * */
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    index = (currentGlobalTTITickCount + delayToPhy) % ulSpsInterval;
#else
    subFrameNum = currentGlobalTTITickCount % MAX_SUBFRAME;
    ulSubFrame = (subFrameNum + getExactULSubFrame(subFrameNum,internalCellIndex))% MAX_SUBFRAME;
    if (ulSubFrame < subFrameNum)
    {
        ulSysFrameNum++;
        if (MAX_SYS_FRAME_NUM < ulSysFrameNum)
        {
            ulSysFrameNum = 0;
        }
    }
  ulTTI = ulSysFrameNum * MAX_SUBFRAME + ulSubFrame;
  index = ulTTI % ulSpsInterval;
#endif
  /* - SPS_TDD_Changes */

    /*CA Changes start  */
    spsUlStrIntervalTTI_p = spsULStrTTIMap_gp[internalCellIndex] + index;

    /* Assign RBs reserved for this TTI for DRA startegy */
    numUlSpsRbsInTti_g[internalCellIndex] = spsUlStrIntervalTTI_p->numULSpsRbsPerTti;

    /* Initialize SPS UEs to 0 for this tick */
    numUlSpsUeInTti_g[internalCellIndex] = 0;
    /*CA Changes end  */

    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    /* Reset schType of each UE present in SDL and occasion list in previous 
     * ttiIndex */
    if( 0 == index  )
    {
        prevIndex = MAX_SPS_INTERVAL - 1; 
    }
    else
    {
       prevIndex = index - 1;
    }
    /*CA Changes start  */
    prevTtiCtx_p =  spsULStrTTIMap_gp[internalCellIndex] + prevIndex;
    /*CA Changes end  */
    spsOccasionUeCount = prevTtiCtx_p->numULSpsOccasionPerTti;
    /* occasion node is added once for each occasion of SPS but 
     numULSpsOccasionPerTti is updated for 4 ticks (for TTIB UE)so subtract 
     numULSpsTTIBUePerTti so as to find exact number of occasion node */
    spsOccasionUeCount = spsOccasionUeCount - prevTtiCtx_p->numULSpsTTIBUePerTti;
    if (0 != spsOccasionUeCount)
    {
        ulSpsResetOccSchType( prevTtiCtx_p, spsOccasionUeCount );
    }
    
    sdlCount = prevTtiCtx_p->numULSpsDciPerTti;
    if( 0 != sdlCount )
    {
        ulSpsResetSdlSchType( prevTtiCtx_p, sdlCount );
    }
#endif    
    /* - SPS_TDD_Changes */

    /*! 
     * \code
     * Get the Number of SPS UE's for which Occasion has to be processed in the
     * TTI index. 
     * If spsOccasionUeCount > 0, then Process All The Ue in the List Till 
     * maxUlSpsOccasionPerTti
     * \endcode 
     * */
    spsOccasionUeCount = spsUlStrIntervalTTI_p->numULSpsOccasionPerTti;
#ifdef FDD_CONFIG
    spsOccasionUeCount = spsOccasionUeCount - spsUlStrIntervalTTI_p->numULSpsTTIBUePerTti;
#endif    
    if( 0 != spsOccasionUeCount )
    {
        /* Process occasion list for the TTI context */
        /* +- SPR 17777 */
        processULSpsOccList( spsUlStrIntervalTTI_p, spsOccasionUeCount,
                /* +- SPR 17777 */
                delayToPhy,
                /*CA Changes start  */
                internalCellIndex);
                /*CA Changes end  */                
    }

    /*! 
     * \code
     * Get the Number of SPS UE's for which SDL has to be processed in this TTI.
     * if sdlCount > 0, then Process All The UE in the SDL.
     * \endcode 
     * */
    sdlCount = spsUlStrIntervalTTI_p->numULSpsDciPerTti;
    if( 0 != sdlCount )
    {
        /* Process SDL for the TTI context */
        /*CA Changes start  */
        processULSpsSdl( spsUlStrIntervalTTI_p, sdlCount, delayToPhy, internalCellIndex );
        /*CA Changes end  */
    }
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name  : ulSpsStrategySendPdcchTrigger
 * Inputs         : ueULContext_p - Pointer to UL UE Context
 *                  ulStrNode_p - Pointer to UL strategy node
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None 
 * Description    : This function will enqueue the node in PDCCH queue 
 *                  depending on DRA strategy used.
 *****************************************************************************/
void ulSpsStrategySendPdcchTrigger( ULUEContext *ueULContext_p, 
        ULStrategyTxNode *ulStrNode_p, 
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        UInt32 ulSubFrame,
#endif
        /* - SPS_TDD_Changes */
        InternalCellIndex internalCellIndex )
{
    ULUeScheduleInfo *ulUeSchedInfo_p = PNULL;
    UInt8  ulSchedulerStrategy = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
        initParams_p->ulSchdConfig.ulSchedulerStrategy;

    if( UL_QOS_SCHEDULING == ulSchedulerStrategy )
    {
        /*putEntry of DLStrategyTXNode in DL Strategy Schedule Info */     
        ulUeSchedInfo_p = &(ulStrategyScheduleInfo_gp[internalCellIndex]->
                ulUeScheduleInfo[ulStrategyScheduleInfo_gp[internalCellIndex]->
                scheduledUeCount]);

        ulUeSchedInfo_p->ueULContext_p = ueULContext_p;
        ulUeSchedInfo_p->ulStrategyTxNode_p = ulStrNode_p;

        /* Increament the scheduled UE count */
        ulStrategyScheduleInfo_gp[internalCellIndex]->scheduledUeCount++;
    }
    else
    {
        /* PutEntry of ULStrategyTXNode in PDCCH Queue */
        if ( !(putEntryInULSchedulerNewTxQueue( ulStrNode_p, 
                        internalCellIndex 
                        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                        ,getCurrentTick()
                        /* + SPS_TDD_Changes */
                        ,ulSubFrame 
                        /* - SPS_TDD_Changes */
                        ,ueULContext_p
#endif
                        /* TDD Config 0 Changes End */
                        ) ) )
        {
            LOG_MAC_MSG( MAC_UL_STRATEGY_ERROR_ID, LOGERROR, MAC_UL_SPS_STRATEGY,
                    getCurrentTick(), ulStrNode_p->ueIndex,
                    0,0,0,0, 0,0, __func__, "EntryFailed");
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
            FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrNode_p);
#else
            freeMemPool(ulStrNode_p);
#endif
        }
    }
}

/******************************************************************************
 * Function Name  : ulSpsSaveHarqInCtxForSdl
 * Inputs         : ueULContext_p - Pointer to UL UE Context
 * Outputs        : None
 * Returns        : None
 * Description    : This function saves the HARQ in SPS context for SDL 
 *                  processing.
 *****************************************************************************/
/* +- SPR 17777 */
void ulSpsSaveHarqInCtxForSdl( ULUEContext *ueULContext_p
#ifdef FDD_CONFIG
        , UInt32 delayToPhy
#endif
        )
/* +- SPR 17777 */
{
    LP_SpsUlInfo spsUlInfo_p = &(ueULContext_p->spsUlInfo);

    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsUlInfo_p->spsUlSetupInfo_p->
            semiPersistentSchedIntervalUL])
    {
#ifdef FDD_CONFIG
        UInt8 tempUlHarqProcessId  = INVALID_HARQ_ID;
        ULHarqInfo *tempUlHarqInfo_p = PNULL;
        if(ueULContext_p->ttiBundlingEnabled)
        {
            tempUlHarqProcessId = GET_TTIB_HARQ_ID( 
                    getCurrentTick() + delayToPhy + FDD_HARQ_OFFSET +
                    SPS_INTERVAL_20);
        }
        else
        {
            tempUlHarqProcessId = MODULO_EIGHT( 
                    getCurrentTick() + delayToPhy + FDD_HARQ_OFFSET +
                    SPS_INTERVAL_20);
        }

        tempUlHarqInfo_p = &ueULContext_p->ulresInfo[tempUlHarqProcessId];
        tempUlHarqInfo_p->harqStatus = HARQ_PROCESS_IN_USE;

        /* Save in SPS UE Context */
        if( SPS_ACTIVATION_INITIATED == spsUlInfo_p->spsState )
        {
            spsUlInfo_p->hpId[1].harqId = tempUlHarqInfo_p->ulHarqProcessId;
        }
        /* hpId[1] is not in case of SPS for TTI Bundling UE .HARQ process is
         * SPS enabled is identified by parameter isSpsEnabled in UL HARQ 
         * conetxt */
        if( ueULContext_p->ttiBundlingEnabled && 
                (SPS_ACTIVATION_INITIATED == spsUlInfo_p->spsState) )
        {
            spsUlInfo_p->hpId[1].harqId = INVALID_HARQ_ID;
        }
#endif
    }
    else
    {
        /* 40 Periodicity, hence index 1 is invalid */
        spsUlInfo_p->hpId[1].harqId = INVALID_HARQ_ID;
    }
}

/******************************************************************************
 * Function Name  : processULSpsSdl
 * Inputs         : spsUlTTIContext_p - TTI context for current tick,
 *                  sdlCount - Number of UEs eligible for SPS occasion in SDL,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function processes the SPS DCI list on the SPS occasion
 *                  and sends DCI information on that occasion for activation.
 *****************************************************************************/
MacRetType processULSpsSdl( LP_SpsULStrIntervalTTIContext spsUlTTIContext_p, 
        UInt8 sdlCount, UInt32 delayToPhy, InternalCellIndex internalCellIndex )
{
    /* Coverity 32349 Fix Start */
    MacRetType retVal           = MAC_SUCCESS;
    /* Coverity 32349 Fix End */
    UInt8                ulHarqProcessId  = INVALID_HARQ_ID;
    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    SInt8 subFrameoffset = 0;
    UInt8 newUlsubframe = 0; 
    UInt8 newDlsubFrame = 0;
    UInt8 ulSubFrame = 0;
    UInt8 tempUlHarqProcessId  = INVALID_HARQ_ID;
#endif
    /* - SPS_TDD_Changes */
    ULGrantRequestType              msgType          = UL_SPS_DEACTIVATION;
    LP_MacULSpsDciListTTI  sdlNode_p        = PNULL;
    LP_MacULSpsDciListTTI  tempSdlNode_p    = PNULL;
    ULStrategyTxNode     *ulStrNode_p     = PNULL;
    ULUEContext          *ueULContext_p   = PNULL;
    LP_SpsUlInfo         spsUlInfo_p      = PNULL;
    ULHarqInfo           *ulHarqInfo_p   = PNULL;
    HIPduArray* tempHIPdu_p  = PNULL;
    /* +- SPR 17777 */

    /*! 
     * \code
     * Process for each node in TTI context.
     * Get the first SDL node in the TTI context.
     * \endcode 
     * */
    GET_FIRST_MAC_SPS_UE_LIST_PER_TTI(spsUlTTIContext_p->spsULDciList, sdlNode_p);

    /* + Coverity 25236 */
    while( PNULL != sdlNode_p && sdlCount-- )
        /* - Coverity 25236 */
    {      
        /* + SPS_TDD_Changes */
        msgType = UL_SPS_DEACTIVATION;
        /* - SPS_TDD_Changes */
        ueULContext_p = ulUECtxInfoArr_g[sdlNode_p->ueIndex].ulUEContext_p;
        if( PNULL == ueULContext_p )
        {
            retVal = MAC_FAILURE;
            continue;            
        }
        spsUlInfo_p = &(ueULContext_p->spsUlInfo);

        /*! \code
         * If UE has moved to Pending delete, Clean the SPS context 
         * \endcode
         * */
        if( 0 != ulUECtxInfoArr_g[sdlNode_p->ueIndex].pendingDeleteFlag )
        {
            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlTTIContext_p->spsULDciList,
                    sdlNode_p);

            /* UE has moved to Pending delete so clean up the SPS context */
            /*CA Changes start  */
            ulSpsPendingDeleteHandling( ueULContext_p, internalCellIndex );
            /*CA Changes end  */
            continue;
        }

        /*! \code
         * If Maximum Number of DCI Attempts reached 
         * \endcode
         * */
        if( spsUlInfo_p->numDciAttempts == 
                cellSpecificParams_g.cellConfigAndInitParams_p[
                internalCellIndex]->initParams_p->spsSysWideParam.
                maxSpsDciAttempt )
        {
            /*! \code
             *  call ulSpsMaxDciAttemptReached and continue 
             * \endcode
             * */
            /* Store the node that needs to be deleted from queue */
            tempSdlNode_p = sdlNode_p;        
            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlTTIContext_p->spsULDciList,
                    sdlNode_p);
            /* Call state machine for deletion of temporary node from queue and
             * do the desired handling */
            /* +- SPR 17777 */
            ulStateMachine[spsUlInfo_p->spsState][MAX_DCI_ATTEMPT](
                    tempSdlNode_p->ueIndex, spsUlInfo_p->spsState,
                    MAX_DCI_ATTEMPT, ueULContext_p, tempSdlNode_p, 
                    delayToPhy, internalCellIndex );

            /* +- SPR 17777 */
            LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, tempSdlNode_p->ueIndex,spsUlInfo_p->spsState, MAX_DCI_ATTEMPT,
                     spsUlInfo_p->spsState, 0,0, __func__, "UlStateMachine");
            continue;
        }

        /*! \code
         * Fill HARQ Process to be pushed in PDCCH Queue.
         * \endcode
         * */
        /* Overwrite msgType with SPS_DEACTIVATION if state is 
         * SPS_DEACTIVATION_INITIATED */
        if( SPS_ACTIVATION_INITIATED == spsUlInfo_p->spsState )
        {
            msgType = UL_SPS_ACTIVATION; 
        }

        /*! \code
         * Get the Harq Process and check if it is available
         * \endcode
         * */          
#ifdef FDD_CONFIG
        /* TTIB_Code Start */
        if(ueULContext_p->ttiBundlingEnabled)
        {
            ulHarqProcessId = GET_TTIB_HARQ_ID(getCurrentTick() + delayToPhy + FDD_HARQ_OFFSET);
        }
        else
        {
            ulHarqProcessId = MODULO_EIGHT(getCurrentTick() + delayToPhy + FDD_HARQ_OFFSET);
        }
        /* TTIB_Code End */
#else
        /* + SPS_TDD_Changes */
        UInt8 subframe = (getCurrentTick() + delayToPhy)%MAX_SUB_FRAME;
        ulSubFrame = (subframe + getExactULSubFrame(subframe,internalCellIndex))% MAX_SUBFRAME; 
        /* Get the free ID in ulHarqProcessId  */
        GET_UL_HARQ_ID_FOR_TDD (subframe, ulHarqProcessId, delayToPhy, internalCellIndex); 
        /* - SPS_TDD_Changes */
#endif
        ulHarqInfo_p = &ueULContext_p->ulresInfo[ulHarqProcessId];
        /* For TTIB UE,HARQ process Id is not same at every occasion so check not required ,
           (HARQ will be 0 or 2 /1 or 3 alternately in case of TTIB-SPS 40 periocity 
           and 0,1,2,3 for SPS 20 periodicity )
           also check is present in strategy for checking that no DCI is scheduled 
           at  SPS occasion .*/
        /* + Bug 21368 */
        if((ulHarqInfo_p->riLength)
        /* - Bug 21368 */

#ifdef FDD_CONFIG
                &&(!ueULContext_p->ttiBundlingEnabled)
#endif
          )
        {
            if(((SPS_ACTIVATION_INITIATED == spsUlInfo_p->spsState) || 
                        (SPS_DEACTIVATION_INITIATED == spsUlInfo_p->spsState))
                    /* + SPS_TDD_Changes */
                    /*&&(ulHarqInfo_p->newDataIndicator == 0)*/)
                /* - SPS_TDD_Changes */
            {            
                UInt8 containerTick = ((getCurrentTick() + delayToPhy)% MAX_PDCCH_CONTAINER);

#ifdef FDD_CONFIG
                /* SPR 19704 + */
                UInt8 resetHiIndex = ((getCurrentTick() + delayToPhy) % MAX_PHICH_CONTAINER);
                /* SPR 19704 - */
#endif
                /*CA Changes start  */
                tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
                /*CA Changes end  */

                addEntryTOHIPduArray(
                        ulHarqInfo_p,
                        HI_ACK,
                        ueULContext_p->crnti,
#ifdef FDD_CONFIG
                        ueULContext_p,
                        resetHiIndex,
#endif
                        tempHIPdu_p
                        /*CA Changes start  */
                        ,internalCellIndex
                        /*CA Changes end  */
                        /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                        /* + SPS_TDD_Changes */
                        ,ulSubFrame
                        /* - SPS_TDD_Changes */
#endif
                        /*TDD Config 0 Changes End*/ 
                        );
                /* SPR 19704 + */
#ifdef FDD_CONFIG
                phichSequenceNoPerPhichGroup_g[internalCellIndex][resetHiIndex][ulHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << ulHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][resetHiIndex][ulHarqInfo_p->nPhichGrp].count++;
#endif
                /* SPR 19704 - */
            }
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
            freeULHarqProcess( ulHarqInfo_p,ueULContext_p->ueIndex,internalCellIndex);
#else
            freeULHarqProcess( ulHarqInfo_p );
#endif
            /* SPR 16916 FIX END */
        }
        /*! \code
         * Store HARQ Id in SDL context.
         * \endcode
         * */
#ifdef FDD_CONFIG
        if(!(ueULContext_p->ttiBundlingEnabled))
#endif  
        {
            ulHarqInfo_p->harqStatus = HARQ_PROCESS_IN_USE;
        }
        /* + SPS_TDD_Changes */
        /* Save in SPS UE Context */
        if( SPS_ACTIVATION_INITIATED == spsUlInfo_p->spsState )
        {
#ifdef FDD_CONFIG
            spsUlInfo_p->hpId[0].harqId = ulHarqInfo_p->ulHarqProcessId;
#elif TDD_CONFIG
            spsUlInfo_p->hpId[0].harqId = ulHarqProcessId;
            /* Check if twoIntervalsConfig is enabled then 2 harqId will be used for SPS TDD */
            if(TRUE == spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
            {
                subFrameoffset = getSubFrameOffsetForSpsTDD(ulSubFrame,internalCellIndex);
                newUlsubframe = (ulSubFrame + subFrameoffset)% MAX_SUBFRAME;
                newDlsubFrame = (newUlsubframe +
                        ulGrantPhichTableInfo_p[internalCellIndex][newUlsubframe].ulGrantPhichInfo.ulPHICHSubFrameNum) % MAX_SUBFRAME;

                /* Get the free ID in ulHarqProcessId  */
                GET_UL_HARQ_ID_FOR_TDD (newDlsubFrame, tempUlHarqProcessId, delayToPhy, internalCellIndex); 

                spsUlInfo_p->hpId[1].harqId = tempUlHarqProcessId;
            }
            else
            {
                spsUlInfo_p->hpId[1].harqId = ulHarqProcessId;
            }  
#endif
        }
        /* - SPS_TDD_Changes */
        /* Save the HARQ in UL SPS Context */
        /* +- SPR 17777 */
        ulSpsSaveHarqInCtxForSdl( ueULContext_p
#ifdef FDD_CONFIG
                , delayToPhy 
#endif
                );
        /* +- SPR 17777 */

        /* set SchType to SPS_SCHEDULING as SPS needs to be scheduled in this
         * tick */ 
        ueULContext_p->schType = SPS_SCHEDULING;
        /* Increment UE for which DCI needs to be sent */
        /*CA Changes start  */
        numUlSpsUeInTti_g[internalCellIndex]++; 

        /* Update HARQ process Id Stats */
        LTE_MAC_UPDATE_UL_HARQ_PROCESS_ID_USED( ueULContext_p->ueIndex,
                spsUlInfo_p->hpId[0].harqId, internalCellIndex);
        /* +- SPR 17777 */
        ulStrNode_p = fillUlSpsDciInfo( ueULContext_p,
                msgType, internalCellIndex );
        /* +- SPR 17777 */
        /*CA Changes end  */
        /*cov_warning 60999 fix start*/
        if(PNULL == ulStrNode_p)
        {
            LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                    getCurrentTick(), __LINE__, sizeof(ULStrategyTxNode), 
                    0, 0, 0, 0,0, __FILE__, __func__);
            ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            ltePanic("getMemFromPool fails %s", __func__);
#ifdef FDD_CONFIG
            if(!(ueULContext_p->ttiBundlingEnabled))
#endif  
            {
                ulHarqInfo_p->harqStatus = HARQ_PROCESS_FREE;
            }
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess( ulHarqInfo_p,ueULContext_p->ueIndex,internalCellIndex);
#else
            freeULHarqProcess( ulHarqInfo_p);
#endif
            /* SPR 16916 FIX END */
            numUlSpsUeInTti_g[internalCellIndex]--;
            ueULContext_p->schType = DRA_SCHEDULING;
            spsUlInfo_p->hpId[0].harqId = INVALID_HARQ_ID;
            spsUlInfo_p->hpId[1].harqId = INVALID_HARQ_ID;
            continue;
        }
        /*cov_warning 60999 fix end*/
        /* + SPR_16901 */
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        ulStrNode_p->ulHarqProcessId = ulHarqProcessId; 
        ulStrNode_p->scheduledCount = 0; 
#endif
        /* - SPS_TDD_Changes */
        /* - SPR_16901 */
        if (msgType != UL_SPS_DEACTIVATION)
        {
#ifdef FDD_CONFIG
            /* TTIB_Code Start */
            if (ueULContext_p->ttiBundlingEnabled)
            {
                ulStrNode_p->minDataSize = determineUlTBSize(ulStrNode_p->avgMcs,MAX_TTI_BUNDLE_RB);
                ulStrNode_p->maxDataSize = ulStrNode_p->minDataSize;
            }
            else
#endif
            {
                ulStrNode_p->minDataSize = spsUlInfo_p->spsOpp;
                ulStrNode_p->maxDataSize = spsUlInfo_p->spsOpp;
            }
        }
        ulStrNode_p->strictAllocRB = TRUE ;

        /* Send the trigger further to PDCCH/QOS */
        ulSpsStrategySendPdcchTrigger( ueULContext_p, ulStrNode_p, 
                /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                ulSubFrame,
#endif
                /* - SPS_TDD_Changes */
                internalCellIndex );
        
        /* Update SPS Strategy Stats */
        LTE_MAC_UPDATE_SPS_UL_STR_STATS(sdlNode_p->ueIndex,
                ueULContext_p, getCurrentTick() , SDL_PROCESSED, 
                spsUlInfo_p->numDciAttempts, internalCellIndex);

        GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlTTIContext_p->spsULDciList,
                sdlNode_p );
    }
    return retVal ;
}

/********************************************************************************
 * Function Name  : processULSpsOccList
 * Inputs         : spsUlStrIntervalTTI_p - TTI context for current tick,
 *                  spsOccasionUeCount - Number of UEs eligible for SPS occasion in
 *                                       occasion list,
 *                  ttiIndex - Index from TTI Map in which SPS occasion is
 *                             processed,
 *                  delayToPhy - Delay to PHY,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : In this function scheduler Strategy will allocate the RBs to TA transmission,
 *                  DRX transmission and to UE depending upon the number of
 *                  sub-frames in advance for which strategy is run to schedule
 *                  allocations and depending upon the available queue load
 *                  of the UEs in the downlink.
 *                  The strategy uses either the proportional fair or round
 *                  robin algorithm to distribute the RBs.
 ********************************************************************************/
MacRetType processULSpsOccList( LP_SpsULStrIntervalTTIContext  spsUlStrIntervalTTI_p,
        /*CA Changes start  */
        /* +- SPR 17777 */
        UInt8 spsOccasionUeCount,UInt32 delayToPhy, InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
    
        /*CA Changes end  */
{
    /* Coverity 32350 Fix Start */
    MacRetType retVal           = MAC_SUCCESS;
    /* Coverity 32350 Fix End */
    LP_SpsUlInfo              spsUlInfo_p       = PNULL;
    ULUEContext*              ueULContext_p     = PNULL;
    LP_MacULSpsOccList        occNode_p         = PNULL;
    LP_MacULSpsOccList        tempOccNode_p     = PNULL;
    LP_MacULSpsDeactReq       macUlSpsDectReq_p = PNULL;
    /* +- SPR 17777 */

    /*! 
     * \code
     * - Get the SPS Occasion list for the TTI index. 
     * \endcode 
     * */
    GET_FIRST_MAC_SPS_UE_LIST_PER_TTI(spsUlStrIntervalTTI_p->spsULOccasionList,
            occNode_p);

    /* + Coverity 25235 */
    while( PNULL != occNode_p && spsOccasionUeCount-- )
        /* - Coverity 25235 */
    {
        /*! \code
         * Get the Ue Context of the Ue.
         * \endcode
         **/
        ueULContext_p   = ulUECtxInfoArr_g[occNode_p->ueIndex].ulUEContext_p;
        if( PNULL == ueULContext_p )
        {
            retVal = MAC_FAILURE;
            continue;            
        }

        /*! \code
         * If UE has moved to Pending delete, Clean the SPS context 
         * \endcode
         * */
        if( 0 != ulUECtxInfoArr_g[occNode_p->ueIndex].pendingDeleteFlag )
        {
            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( 
                    spsUlStrIntervalTTI_p->spsULOccasionList, occNode_p );

            /* UE has moved to Pending delete so clean up the SPS context */
            /*CA Changes start  */
            /* + SPS_TDD_Changes*/
            //ulSpsPendingDeleteHandling( ueULContext_p, internalCellIndex );
            /* - SPS_TDD_Changes*/
            /*CA Changes end  */
            continue;
        }

        spsUlInfo_p = &(ueULContext_p->spsUlInfo);
        /*! \code
         * - If WB MCS drops below threshold OR PUSCH power is exceeded on current UL allocation
         *      Trigger SPS Deactivation 
         * - Else
         *     assignULRBsAndSendToPDCCHFromSpsOccasion 
         *     Reset numOfSpsEmptyTx
         * \endcode
         * */
        if( ( LTE_TRUE == spsUlInfo_p->isUeUlWbMcsBelowThresh ) ||
                ( MAC_FAILURE == validateNprbFromATBForSps(
                                                           spsUlInfo_p->ulResAllocInfo.allocatedRB, 
                                                           spsUlInfo_p->ulResAllocInfo.allocatedMCS, 
                                                           /*CA Changes start  */
                                                           ueULContext_p, 0, internalCellIndex) ) )  
            /*CA Changes end  */
        {
            tempOccNode_p = occNode_p;

            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI(spsUlStrIntervalTTI_p->spsULOccasionList,
                    occNode_p);

            /* +- SPR 17777 */
            if( MAC_FAILURE == ulStateMachine[spsUlInfo_p->spsState]
                    [MCS_CHANGED](
                        tempOccNode_p->ueIndex, spsUlInfo_p->spsState, 
                        MCS_CHANGED, ueULContext_p, tempOccNode_p, 
                        /*CA Changes start  */
                        delayToPhy, internalCellIndex ) )
                /*CA Changes end  */
            {
                /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                         getCurrentTick(), __LINE__, tempOccNode_p->ueIndex,spsUlInfo_p->spsState, MCS_CHANGED,
                         spsUlInfo_p->spsState, 0,0, __func__, "UlStateMachine");
                /* Allocate the memory for deactivation queue node */
                macUlSpsDectReq_p = (LP_MacULSpsDeactReq)getMemFromPool(
                        sizeof(MacULSpsDeactReq), PNULL);
                if( PNULL == macUlSpsDectReq_p )
                {
                    LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, 
                            L2_SYS_FAIL, getCurrentTick(), __LINE__, 
                            sizeof(MacULSpsDeactReq), 0, 0, 0, 0,0, 
                            __FILE__, __func__);
                    ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                            CRITICAL_ALARM);
                    ltePanic("getMemFromPool fails %s", __func__);
                    /* Klockwork warning Changes Start */
                    return MAC_FAILURE;
                    /* Klockwork warning Changes End */
                }

                /* Fill deactivation queue node */
                macUlSpsDectReq_p->ueIndex = tempOccNode_p->ueIndex;
                macUlSpsDectReq_p->cause = MCS_CHANGED;
                /* Enqueue the node to deactivation queue so that it 
                 * can be processed in next tick */
                /*CA Changes start  */
                /* + Coverity Id - 32609 */
                if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsDeactQ_gp[internalCellIndex], 
                            macUlSpsDectReq_p ))
                {
                    freeMemPool(macUlSpsDectReq_p);
                    macUlSpsDectReq_p = PNULL;
                }
                /* - Coverity Id - 32609 */

                /* Update Q Stats */
                /* CA Stats Changes Start */
                LTE_MAC_QUEUE_USED( UL_SPS_DEACT_Q,  
                        QCOUNT_MAC_UL_SPS_Q(ulSpsDeactQ_gp[internalCellIndex]),
                        internalCellIndex);
                /* CA Stats Changes End */
                /*CA Changes end  */
            }
            else
            {
                /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_UL_SPS_CURR_STATE, LOGDEBUG, MAC_UL_SPS_STRATEGY,
                         getCurrentTick(), __LINE__, tempOccNode_p->ueIndex,spsUlInfo_p->spsState, MCS_CHANGED,
                         spsUlInfo_p->spsState, 0,0, __func__, "UlStateMachine");
            }
            continue;
        }
/* SPR 19038 Fix Start */
        /* If Meas Gap is configured after SPS Activation 
         * Below code will initiate the deactivation process 
         * If Ue's Meas Gap collides with SPS occasion 
         */
        else if( LTE_TRUE == spsUlInfo_p->isUeMeasGapCollisionWithSpsOcc )
        {
            tempOccNode_p = occNode_p;

            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI(spsUlStrIntervalTTI_p->spsULOccasionList,
                    occNode_p);

            if( MAC_FAILURE == ulStateMachine[spsUlInfo_p->spsState]
                    [MEAS_GAP_COL_WITH_SPS_OCC](
                        tempOccNode_p->ueIndex, spsUlInfo_p->spsState, 
                        MEAS_GAP_COL_WITH_SPS_OCC, ueULContext_p, tempOccNode_p, 
                        delayToPhy,internalCellIndex ) )
            {
                /* Allocate the memory for deactivation queue node */
                macUlSpsDectReq_p = (LP_MacULSpsDeactReq)getMemFromPool(
                        sizeof(MacULSpsDeactReq), PNULL);
                if( PNULL == macUlSpsDectReq_p )
                {
                    LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, 
                            L2_SYS_FAIL, getCurrentTick(), __LINE__, 
                            sizeof(MacULSpsDeactReq), 0, 0, 0, 0,0, 
                            __FILE__, __func__);
                    ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                            CRITICAL_ALARM);
                    ltePanic("getMemFromPool fails %s", __func__);
                }

                /* Fill deactivation queue node */
                macUlSpsDectReq_p->ueIndex = tempOccNode_p->ueIndex;
                macUlSpsDectReq_p->cause = MEAS_GAP_COL_WITH_SPS_OCC;
                /* Enqueue the node to deactivation queue so that it 
                 * can be processed in next tick */
                /* SPR 23483 Changes Start */
                if (CIRC_FAILURE == ENQUEUE_MAC_UL_SPS_Q( ulSpsDeactQ_gp[internalCellIndex], 
                        macUlSpsDectReq_p ))
                /* SPR 23483 Changes End */
                {
                    LOG_MAC_WARNING( MAC_UL_SPS_STRATEGY, "[%d] Unable to "
                            "enqueue the node for Deactivation for UE[%d]",
                            getCurrentTick(), macUlSpsDectReq_p->ueIndex );
                    freeMemPool(macUlSpsDectReq_p);
                }

                /* Update Q Stats */
                LTE_MAC_QUEUE_USED( UL_SPS_DEACT_Q,  
                        QCOUNT_MAC_UL_SPS_Q( ulSpsDeactQ_gp[internalCellIndex]),internalCellIndex );
            }
            continue;
        }
/* SPR 19038 Fix End */
        else
        {
            /* +- SPR 17777 */
            if(MAC_FAILURE == assignULRBsAndSendToPDCCHFromSpsOccasion(ueULContext_p
                        ,delayToPhy
#ifdef FDD_CONFIG
                        /* +- SPR 17777 */
                        ,occNode_p->isTTIBundle
#endif                                                    
                        /*CA Changes start  */
                        ,internalCellIndex
                        /*CA Changes end  */
                        ))
            {
                continue;
            }

            /* set SchType to SPS_SCHEDULING as SPS needs to be scheduled in 
             * this tick */ 
            ueULContext_p->schType = SPS_SCHEDULING;

            /* CA Stats Changes Start */
            /* Update Stats */
            LTE_MAC_UPDATE_SPS_UL_STR_STATS(occNode_p->ueIndex,
                    ueULContext_p, getCurrentTick(),OCC_PROCESSED, 0,
                    internalCellIndex);
            /* CA Stats Changes End */
        }
        GET_NEXT_MAC_SPS_UE_LIST_PER_TTI(spsUlStrIntervalTTI_p->spsULOccasionList,
                occNode_p);
    }
    return retVal;
}

/********************************************************************************
 * Function Name  : ulDeleteUeFromOccasionList
 * Inputs         : ttiContext_p - tti Context for ttiIndex for Occasion list,
 *                  ttiContextPeriodicity20_p - tti Context for next periodicity
 *                                              on case of 20 ms interval,
 *                  spsUlInfo_p - SPS UL Info of an UE,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function deletes UE from occasion list and updates
 *                  the TTI Context Map.
 ********************************************************************************/
/* CA Stats Changes Start */
/* + SPS_TDD_Changes*/
void ulDeleteUeFromOccasionList(
#ifdef TDD_CONFIG
        SInt8 subframe_offset,
#endif
        UInt16 ulSpsInterval,
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p,
        /* +- SPR 17777 */
        LP_SpsUlInfo  spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
        ,InternalCellIndex internalCellIndex
#endif
        /* - SPS_TDD_Changes*/
        )
        /* +- SPR 17777 */
{
    /* CA Stats Changes End */
    LP_MacULSpsOccList   occNode_p = PNULL;
    UInt8    semiPersSchedIntervalUL   = 0;
    /* + SPS_TDD_Changes*/
#ifdef TDD_CONFIG
    UInt16 nextTTIIndex = 0;
    LP_SpsULStrIntervalTTIContext  ttiContextNextPeriodicity_p = PNULL;
#endif
    /* - SPS_TDD_Changes*/

    semiPersSchedIntervalUL = 
        spsUlInfo_p->spsUlSetupInfo_p->semiPersistentSchedIntervalUL;
    
    occNode_p = (LP_MacULSpsOccList)spsUlInfo_p->sdlOccNode_p; 
    /* Remove the entry from the SPS occasion List */
    DELETE_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsULOccasionList,
                                   occNode_p );
    
    /* Free the memory for occasion node */
    freeMemPool( occNode_p );
    occNode_p = PNULL;

    /* Decrement numUlSpsOccasionPerTti value */
    (ttiContext_p->numULSpsOccasionPerTti)--;

    /* + SPS_TDD_Changes*/
#ifdef TDD_CONFIG
    if(SPS_INTERVAL_40 == spsIntervalMap_g[semiPersSchedIntervalUL])
    {
        ttiContextNextPeriodicity_p = (spsULStrTTIMap_gp[internalCellIndex] + (
            (spsUlInfo_p->actTtiIndex + subframe_offset + SPS_INTERVAL_40) % ulSpsInterval)); 
        occNode_p = (LP_MacULSpsOccList)spsUlInfo_p->sdlOccNode40_p; 
        DELETE_MAC_SPS_UE_LIST_PER_TTI(ttiContextNextPeriodicity_p->spsULOccasionList,
                occNode_p );
        /* Free the memory for occasion node */
        freeMemPool( occNode_p );
        occNode_p = PNULL;
        (ttiContextNextPeriodicity_p->numULSpsOccasionPerTti)--;
        spsUlInfo_p->sdlOccNode40_p = PNULL;
    }
#endif
    /* - SPS_TDD_Changes*/
    /* Remove entry from next occasion also from TTI Map if 20 ms periodicity 
     * is configured */
    if( SPS_INTERVAL_20 == spsIntervalMap_g[semiPersSchedIntervalUL])
    {
        occNode_p = (LP_MacULSpsOccList)spsUlInfo_p->sdlOccNode20_p; 
        DELETE_MAC_SPS_UE_LIST_PER_TTI(
               ttiContextPeriodicity20_p->spsULOccasionList, occNode_p);

        /* Free the memory for occasion node */
        freeMemPool( occNode_p );
        occNode_p = PNULL;

        /* Decrement numUlSpsOccasionPerTti value */
        (ttiContextPeriodicity20_p->numULSpsOccasionPerTti)--;

        /* + SPS_TDD_Changes*/
#ifdef TDD_CONFIG
        nextTTIIndex = (spsUlInfo_p->actTtiIndex + SPS_INTERVAL_40 ) % ulSpsInterval;
        ttiContextNextPeriodicity_p = PNULL;
        ttiContextNextPeriodicity_p = (spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex);
        occNode_p = (LP_MacULSpsOccList)spsUlInfo_p->sdlOccNode40_p; 
        DELETE_MAC_SPS_UE_LIST_PER_TTI(ttiContextNextPeriodicity_p->spsULOccasionList,
                occNode_p );
        /* Free the memory for occasion node */
        freeMemPool( occNode_p );
        occNode_p = PNULL;
        (ttiContextNextPeriodicity_p->numULSpsOccasionPerTti)--;
        spsUlInfo_p->sdlOccNode40_p = PNULL;

        ttiContextNextPeriodicity_p = PNULL;
        nextTTIIndex = (spsUlInfo_p->actTtiIndex + subframe_offset + 
			SPS_INTERVAL_20 + SPS_INTERVAL_40 ) % ulSpsInterval;
        ttiContextNextPeriodicity_p = (spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex);
        occNode_p = (LP_MacULSpsOccList)spsUlInfo_p->sdlOccNode60_p; 
        DELETE_MAC_SPS_UE_LIST_PER_TTI(ttiContextNextPeriodicity_p->spsULOccasionList,
                occNode_p );
        /* Free the memory for occasion node */
        freeMemPool( occNode_p );
        occNode_p = PNULL;
        (ttiContextNextPeriodicity_p->numULSpsOccasionPerTti)--;
        spsUlInfo_p->sdlOccNode60_p = PNULL;
#endif
        /* - SPS_TDD_Changes*/
    }
    /* Set Occasion node pointer in UE context as NULL */
    spsUlInfo_p->sdlOccNode_p = PNULL;
    spsUlInfo_p->sdlOccNode20_p = PNULL;
    /* Initialize TTI index with invalid value in UE context */
    /* + SPS_TDD_Changes*/
    spsUlInfo_p->ttiIndex = ulSpsInterval;
    /* - SPS_TDD_Changes*/
    /* Reset numOfSpsEmptyTx in UE context */
    spsUlInfo_p->numOfSureSpsEmptyTx  = 0;
    spsUlInfo_p->numOfUnsureSpsEmptyTx  = 0;
   
    /* Update Stats */
    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
            ttiContextPeriodicity20_p, getCurrentTick(), 
            semiPersSchedIntervalUL, internalCellIndex);
    /* CA Stats Changes End */
}

/********************************************************************************
 * Function Name  : ulAddUeInOccasionList
 * Inputs         : ueIndex - UE to be added in occasion list,
 *                  ttiContext_p - tti Context for ttiIndex for Occasion list,
 *                  ttiContextPeriodicity20_p - tti Context for next periodicity
 *                                              on case of 20 ms interval,
 *                  spsUlInfo_p - SPS UL Info of an UE,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function adds UE from occasion list and updates
 *                  the TTI Context Map.
 ********************************************************************************/
/* CA Stats Changes Start */
/* + SPS_TDD_Changes */
void ulAddUeInOccasionList(
#ifdef TDD_CONFIG
        SInt8 subframe_offset,
        UInt16 ulSpsInterval,
#endif
        UInt16 ueIndex,
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p,
        LP_SpsUlInfo       spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
        ,InternalCellIndex  internalCellIndex
#endif
        )
{
    /* CA Stats Changes End */
    LP_MacULSpsOccList occNode_p = PNULL; 
    UInt8 semiPersSchedIntervalUL = spsIntervalMap_g[
	    spsUlInfo_p->spsUlSetupInfo_p->semiPersistentSchedIntervalUL] ;
#ifdef TDD_CONFIG
    UInt16 nextTTIIndex = 0;
    LP_SpsULStrIntervalTTIContext  ttiContextNextPeriodicity_p = PNULL;
#endif
    occNode_p = ulAddUeInOccListAtPeriodicity(ueIndex, ttiContext_p );
    /* Set Occasion node pointer in UE context as the node inserted */
    spsUlInfo_p->sdlOccNode_p = occNode_p;
#ifdef TDD_CONFIG
    if(SPS_INTERVAL_40 == semiPersSchedIntervalUL)
    {
        occNode_p = PNULL;
        nextTTIIndex = (spsUlInfo_p->actTtiIndex + subframe_offset + SPS_INTERVAL_40) % ulSpsInterval ;
        /* Allocate node for PDSCH Occasion List entry and insert in occasion list  */
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 

        occNode_p = ulAddUeInOccListAtPeriodicity(ueIndex, ttiContextNextPeriodicity_p ); 
        /* Set Occasion node pointer in UE context as the node inserted */
        spsUlInfo_p->sdlOccNode40_p = occNode_p;
    }
#endif

    /* Put entry in occasion list for next occasion for 20 ms periodicity */
    /* + SPS_TDD_Changes */
    if( SPS_INTERVAL_20 == semiPersSchedIntervalUL)
    /* _ SPS_TDD_Changes */
    {
        occNode_p = PNULL;
        occNode_p = ulAddUeInOccListAtPeriodicity(ueIndex, ttiContextPeriodicity20_p ); 
        /* Set Occasion node pointer in UE context as the node inserted */
        spsUlInfo_p->sdlOccNode20_p = occNode_p;

#ifdef TDD_CONFIG
        /* Inserting occasion node of a UE at 3rd index in TTI Map*/  
        occNode_p = PNULL ;
        ttiContextNextPeriodicity_p = PNULL;
        nextTTIIndex = (spsUlInfo_p->actTtiIndex + SPS_INTERVAL_40) % ulSpsInterval ;
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 

        occNode_p = ulAddUeInOccListAtPeriodicity(ueIndex, ttiContextNextPeriodicity_p ); 
        /* Set Occasion node pointer in UE context as the node inserted */
        spsUlInfo_p->sdlOccNode40_p = occNode_p;
        occNode_p = PNULL ;

        /* Inserting occasion node of a UE at 4th index in TTI Map*/  
        ttiContextNextPeriodicity_p = PNULL;
        nextTTIIndex = (spsUlInfo_p->actTtiIndex + subframe_offset + SPS_INTERVAL_20 + SPS_INTERVAL_40) % ulSpsInterval ;
        /* Allocate node for PDSCH Occasion List entry and insert in occasion list  */
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 

        occNode_p = ulAddUeInOccListAtPeriodicity(ueIndex, ttiContextNextPeriodicity_p ); 
        /* Set Occasion node pointer in UE context as the node inserted */
        spsUlInfo_p->sdlOccNode60_p = occNode_p;
#endif
        /* - SPS_TDD_Changes */
    }
    
    /* Update Stats */
    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
            ttiContextPeriodicity20_p, getCurrentTick(), 
            semiPersSchedIntervalUL, internalCellIndex);
    /* CA Stats Changes End */
}

/********************************************************************************
 * Function Name  : ulDeleteUeFromSdl
 * Inputs         : ttiContext_p - tti Context for ttiIndex for Occasion list,
 *                  ttiContextPeriodicity20_p - tti Context for next periodicity
 *                                              on case of 20 ms interval,
 *                  spsUlInfo_p - SPS UL Info of an UE,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function deletes UE from SDL and updates the 
 *                  TTI Context Map
 ********************************************************************************/
/* CA Stats Changes Start */
/* + SPS_TDD_Changes */
void ulDeleteUeFromSdl(
#ifdef TDD_CONFIG
        SInt8 subframeoffset,
        UInt16 ulSpsInterval,
        UInt16 ttiIndex,
#endif
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p,
        /* +- SPR 17777 */
        LP_SpsUlInfo spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
        ,InternalCellIndex internalCellIndex
#endif
        )
        /* +- SPR 17777 */
{
    /* + SPS_TDD_Changes */
    UInt8 spsPeriodicity             = 0;                   
    LP_MacULSpsDciListTTI  sdlNode_p = PNULL;
#ifdef TDD_CONFIG
    UInt16 nextTTIIndex = 0;
    LP_SpsULStrIntervalTTIContext  ttiContextNextPeriodicity_p = PNULL;
#endif
    spsPeriodicity = spsUlInfo_p->spsUlSetupInfo_p->semiPersistentSchedIntervalUL;
    /* - SPS_TDD_Changes */
    sdlNode_p = (LP_MacULSpsDciListTTI)spsUlInfo_p->sdlOccNode_p;

    /* Delete the node from SPS DCI List */
    DELETE_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsULDciList, sdlNode_p);
    /* Decrement number of SPS DCI in the TTI */
    (ttiContext_p->numULSpsDciPerTti)--;

    /* Free the memory for occasion node */
    freeMemPool( sdlNode_p );
    sdlNode_p = PNULL;

    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    /* - SPS_TDD_Changes */
    if(SPS_INTERVAL_40 == spsIntervalMap_g[spsPeriodicity])
    {
        nextTTIIndex = (ttiIndex + subframeoffset + SPS_INTERVAL_40) % ulSpsInterval ;
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex]+ nextTTIIndex;
        sdlNode_p = (LP_MacULSpsDciListTTI)spsUlInfo_p->sdlOccNode40_p;
        /* Delete the node from SPS DCI List */
        DELETE_MAC_SPS_UE_LIST_PER_TTI( ttiContextNextPeriodicity_p->spsULDciList, sdlNode_p);
        /* Decrement number of SPS DCI in the TTI */
        (ttiContextNextPeriodicity_p->numULSpsDciPerTti)--;

        /* Free the memory for occasion node */
        freeMemPool( sdlNode_p );
        sdlNode_p = PNULL;
        spsUlInfo_p->sdlOccNode40_p = PNULL;
    }
#endif
    /* Check for next TTI index in if 20 ms periodicity in TTI Map */
    /* + SPS_TDD_Changes */
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsPeriodicity])
    /* - SPS_TDD_Changes */
    {
        sdlNode_p = (LP_MacULSpsDciListTTI)spsUlInfo_p->sdlOccNode20_p;
        DELETE_MAC_SPS_UE_LIST_PER_TTI(
                    ttiContextPeriodicity20_p->spsULDciList, sdlNode_p );        
        /* Decrement number of SPS DCI in the TTI */
        (ttiContextPeriodicity20_p->numULSpsDciPerTti)--;

        /* Free the memory for SDL node */
        freeMemPool( sdlNode_p );
        sdlNode_p = PNULL;

        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        /* Check the third index in 20 ms periodicity */
        nextTTIIndex = (ttiIndex + SPS_INTERVAL_40)% ulSpsInterval;
        ttiContextNextPeriodicity_p = PNULL;
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 
        sdlNode_p = (LP_MacULSpsDciListTTI)spsUlInfo_p->sdlOccNode40_p;
        DELETE_MAC_SPS_UE_LIST_PER_TTI(
                ttiContextNextPeriodicity_p->spsULDciList, sdlNode_p );        
        /* Decrement number of SPS DCI in the TTI */
        (ttiContextNextPeriodicity_p->numULSpsDciPerTti)--;

        /* Free the memory for SDL node */
        freeMemPool( sdlNode_p );
        sdlNode_p = PNULL;

        /* Check the fourth index in 20 ms periodicity */
        ttiContextNextPeriodicity_p = PNULL;
        nextTTIIndex = (ttiIndex + subframeoffset + SPS_INTERVAL_20 + SPS_INTERVAL_40) % ulSpsInterval ;
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 
        sdlNode_p = (LP_MacULSpsDciListTTI)spsUlInfo_p->sdlOccNode60_p;
        DELETE_MAC_SPS_UE_LIST_PER_TTI(
                ttiContextNextPeriodicity_p->spsULDciList, sdlNode_p );        
        /* Decrement number of SPS DCI in the TTI */
        (ttiContextNextPeriodicity_p->numULSpsDciPerTti)--;

        /* Free the memory for SDL node */
        freeMemPool( sdlNode_p );
        sdlNode_p = PNULL;

        /* Mark sdl node in cotext as NULL */
        spsUlInfo_p->sdlOccNode40_p = PNULL;
        spsUlInfo_p->sdlOccNode60_p = PNULL;
#endif
        /* - SPS_TDD_Changes */
    }
    
    /* Set SDL pointer in UE context as NULL */
    spsUlInfo_p->sdlOccNode_p = PNULL;
    spsUlInfo_p->sdlOccNode20_p = PNULL;
    
    /* Update Stats */
    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
            ttiContextPeriodicity20_p, getCurrentTick(),
            spsUlInfo_p->spsUlSetupInfo_p->semiPersistentSchedIntervalUL, 
            internalCellIndex);
    /* CA Stats Changes End */
}

/********************************************************************************
 * Function Name  : ulAddUeInSdl
 * Inputs         : ueIndex - UE to be added in occasion list,
 *                  ttiContext_p - tti Context for ttiIndex for Occasion list,
 *                  ttiContextPeriodicity20_p - tti Context for next periodicity
 *                                              on case of 20 ms interval,
 *                  spsUlInfo_p - SPS UL Info of an UE,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function adds UE in SDL list and updates
 *                  the TTI Context Map.
 ********************************************************************************/
/* CA Stats Changes Start */
/* + SPS_TDD_Changes */
void ulAddUeInSdl(
#ifdef TDD_CONFIG
        SInt8 subframe_offset,
        UInt16 ttiIndex,
        UInt16 ulSpsInterval,
#endif
        UInt16 ueIndex,
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p,
        LP_SpsUlInfo        spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
        ,InternalCellIndex   internalCellIndex
#endif
        )
{
    /* CA Stats Changes End */
    LP_MacULSpsDciListTTI  sdlNode_p = PNULL;
    /* + SPS_TDD_Changes */
    UInt8 spsPeriodicity             = 0;
#ifdef TDD_CONFIG
    UInt16 nextTTIIndex = 0;
    LP_SpsULStrIntervalTTIContext  ttiContextNextPeriodicity_p = PNULL;
#endif
    spsPeriodicity = spsUlInfo_p->spsUlSetupInfo_p->semiPersistentSchedIntervalUL ;
    /* Allocate node for SDL List entry and enqeue in SDL */
    sdlNode_p = ulAddUeInSdlAtPeriodicity(ueIndex, ttiContext_p );
    /* - SPS_TDD_Changes */

    /* Set SDL node pointer in UE context as the node inserted */
    spsUlInfo_p->sdlOccNode_p = sdlNode_p;

    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    if(SPS_INTERVAL_40 == spsIntervalMap_g[spsPeriodicity]) 
    {
        sdlNode_p = PNULL;
        nextTTIIndex = (ttiIndex + subframe_offset + SPS_INTERVAL_40 ) % ulSpsInterval;
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex;

        sdlNode_p = ulAddUeInSdlAtPeriodicity(ueIndex, ttiContextNextPeriodicity_p );
        spsUlInfo_p->sdlOccNode40_p = sdlNode_p;
    }
#endif
    /* Put entry in SDL for next occasion for 20 ms periodicity */
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsPeriodicity])
    {
        sdlNode_p = PNULL;
        /* Allocate node for SDL List entry and enqeue in SDL */
        sdlNode_p = ulAddUeInSdlAtPeriodicity(ueIndex, ttiContextPeriodicity20_p );

        /* Set SDL node pointer in UE context as the node inserted */
        spsUlInfo_p->sdlOccNode20_p = sdlNode_p;

#ifdef TDD_CONFIG
        /* Put entry in SDL at 3rd index for 20 ms periodicity */
        sdlNode_p = PNULL;
        nextTTIIndex = (ttiIndex + SPS_INTERVAL_40 ) % ulSpsInterval;
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 

        sdlNode_p = ulAddUeInSdlAtPeriodicity(ueIndex, ttiContextNextPeriodicity_p );
        spsUlInfo_p->sdlOccNode40_p = sdlNode_p;

        /* Put entry in SDL at 4th index for 20 ms periodicity */
        sdlNode_p = PNULL;
        ttiContextNextPeriodicity_p = PNULL;

        nextTTIIndex = (ttiIndex + subframe_offset + SPS_INTERVAL_40 + SPS_INTERVAL_20) % ulSpsInterval;
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 

        sdlNode_p = ulAddUeInSdlAtPeriodicity(ueIndex, ttiContextNextPeriodicity_p );
        spsUlInfo_p->sdlOccNode60_p = sdlNode_p;
#endif
	/* - SPS_TDD_Changes */
    }

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiContext_p, 
        ttiContextPeriodicity20_p, getCurrentTick(),
        spsUlInfo_p->spsUlSetupInfo_p->semiPersistentSchedIntervalUL ,
        internalCellIndex);
    /* CA Stats Changes End */
}

/**********************************************************************************
 * Function Name  : ulSpsPendingDeleteHandling
 * Inputs         : ulUeCtx_p - UL UE Context,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles pending delete UE request and clears the
 *                  TTI context stored by SPS scheduler. This API clears resources
 *                  reserved by SPS PUSCH, SPS occasion list, SDL. Release SPS-RNTI
 *                  and also releases UL Setup Info pointer.
 **********************************************************************************/
void ulSpsPendingDeleteHandling( ULUEContext *ulUeCtx_p, InternalCellIndex internalCellIndex  )
{
    LP_SpsUlInfo      spsUlInfo_p  = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContextPeriodicity20_p  = PNULL;   
    /* + SPS_TDD_Changes */
    SInt8 subFrameOffset          = 0;            
#ifdef FDD_CONFIG
    UInt16 ulSpsInterval = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt16 ulSpsInterval = MAX_UL_SPS_INTERVAL;
#endif
    /* - SPS_TDD_Changes */

    UInt32 qCount = 0;

    /* + SPS_TDD_Changes*/
    if(PNULL == ulUeCtx_p  || ((PNULL == ulUeCtx_p->spsUlInfo.spsUlSetupInfo_p) 
         && ((SPS_IDLE == ulUeCtx_p->spsUlInfo.spsState) || (INVALID_STATE == ulUeCtx_p->spsUlInfo.spsState)))) 
    /* - SPS_TDD_Changes*/
    {
        return;   
    }
    spsUlInfo_p = &ulUeCtx_p->spsUlInfo;

    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    if(TRUE == spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
    {
        subFrameOffset = getSubFrameOffsetForSpsTDD((spsUlInfo_p->actTtiIndex % MAX_SUBFRAME),internalCellIndex);
    }
#endif
    /* - SPS_TDD_Changes */
    /*! \code
     * if state == SPS_ACTIVATED
     *    Delete UE node from SPS Occasion list
     * else if state == SPS_ACTIVATION_INITIATED or SPS_DEACTIVATION_INITIATED
     *    Delete UE node from SPS DCI List (SDL)
     * \endcode
     * */
    if( SPS_ACTIVATED == spsUlInfo_p->spsState )
    {
        /* Get the TTI index in which SPS was activated */
        /*CA Changes start  */
        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + spsUlInfo_p->actTtiIndex;
        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] +
		/* + SPS_TDD_Changes */
            ((spsUlInfo_p->actTtiIndex + subFrameOffset + SPS_INTERVAL_20) % 
            ulSpsInterval);
		/* - SPS_TDD_Changes */
        /*CA Changes end  */
        qCount =  QCOUNT_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsULOccasionList);
        if(qCount !=0)
        {
#ifdef FDD_CONFIG
            /* TTIB_Code Start */
            LP_MacULSpsOccList   occNode_p = PNULL;
            UInt8 bundleTick  = 0;

            occNode_p = (LP_MacULSpsOccList)spsUlInfo_p->sdlOccNode_p;
             if(occNode_p->isTTIBundle)
            {
                for( bundleTick = (TTI_BUNDLE_SIZE - 1);bundleTick > 0; bundleTick--)
                {
                    /* + SPS_TDD_Changes */
                    /*CA Changes start  */
                    ((spsULStrTTIMap_gp[internalCellIndex] + ((spsUlInfo_p->actTtiIndex + bundleTick) %
                                    ulSpsInterval))->numULSpsOccasionPerTti)--;
                    ((spsULStrTTIMap_gp[internalCellIndex] + ((spsUlInfo_p->actTtiIndex + bundleTick) %
                                    ulSpsInterval))->numULSpsTTIBUePerTti)--;
                    /*CA Changes end  */

                    if(SPS_INTERVAL_20 == spsIntervalMap_g[spsUlInfo_p->spsUlSetupInfo_p->\
                     semiPersistentSchedIntervalUL] )
                    {
                      /*CA Changes start  */
                      ((spsULStrTTIMap_gp[internalCellIndex] +
                                     ((spsUlInfo_p->actTtiIndex + SPS_INTERVAL_20 + bundleTick) %
                                      ulSpsInterval))->numULSpsOccasionPerTti)--;                                      
                      ((spsULStrTTIMap_gp[internalCellIndex] +
                                     ((spsUlInfo_p->actTtiIndex + SPS_INTERVAL_20 + bundleTick) %
                                      ulSpsInterval))->numULSpsTTIBUePerTti)--;                                      
                      /*CA Changes end  */
                      /* - SPS_TDD_Changes */
                    }
                }
            }
            /* TTIB_Code End */
           
#endif
            /* CA Stats Changes Start */
             /* + SPS_TDD_Changes */ 
             ulDeleteUeFromOccasionList(
#ifdef TDD_CONFIG
                     subFrameOffset,
#endif
                     ulSpsInterval,
                     /* - SPS_TDD_Changes */ 
                     ttiContext_p, 
                     /* +- SPR 17777 */
                     ttiContextPeriodicity20_p,
                     spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
                     ,internalCellIndex
#endif
                     );
             /* +- SPR 17777 */
            /* CA Stats Changes End */
        }
    }
    else if( SPS_ACTIVATION_INITIATED == spsUlInfo_p->spsState )
    {
        /* Get the TTI index in which SPS was activated */
        /*CA Changes start  */
        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + spsUlInfo_p->actTtiIndex;
        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] +
            /* + SPS_TDD_Changes */
            ((spsUlInfo_p->actTtiIndex + subFrameOffset + SPS_INTERVAL_20) % 
                     ulSpsInterval);
            /* - SPS_TDD_Changes */
        /*CA Changes end  */
        qCount =  QCOUNT_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsULDciList);
        if(qCount !=0)
        {
            /* CA Stats Changes Start */
            /* +- SPR 17777 */
            /* + SPS_TDD_Changes */
            ulDeleteUeFromSdl(
#ifdef TDD_CONFIG
                    subFrameOffset,
                    ulSpsInterval,
                    spsUlInfo_p->actTtiIndex,
#endif 
                    /* - SPS_TDD_Changes */
                    ttiContext_p, 
                    ttiContextPeriodicity20_p, 
                    spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
                    ,internalCellIndex
#endif
                    );
            /* +- SPR 17777 */
        }
    }
    else if( SPS_DEACTIVATION_INITIATED == spsUlInfo_p->spsState )
    {
        /* Get the TTI index in which SPS was deactivated */
        /*CA Changes start  */
        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + spsUlInfo_p->ttiIndex;
        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] +
                     ((spsUlInfo_p->ttiIndex + SPS_INTERVAL_20) % 
                     MAX_SPS_INTERVAL);
        /*CA Changes end  */
        qCount =  QCOUNT_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsULDciList);
        if(qCount !=0)
        {
            /* +- SPR 17777 */
            ulDeleteUeFromSdl(
                    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                    0,
                    ulSpsInterval,
                    spsUlInfo_p->ttiIndex,
#endif 
                    /* - SPS_TDD_Changes */
                    ttiContext_p,
                    ttiContextPeriodicity20_p, 
                    spsUlInfo_p
#if defined(TDD_CONFIG) || defined(SSI_DEBUG_STATS)
                    ,internalCellIndex
#endif
                    );
            /* +- SPR 17777 */
            /* CA Stats Changes End */
        }
    }
    else if (SPS_RES_RSV_REQ_INITIATED == spsUlInfo_p->spsState )
    {
        /*CA Changes start  */
        UInt8 count = QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ) ;
        /*CA Changes end  */
        LP_MacULSpsStatus macUlSpsStatus_p  = PNULL;
        while( count-- )
        {
            /*CA Changes start  */
            DEQUEUE_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex], macUlSpsStatus_p );
            /*CA Changes end  */
            if(ulUeCtx_p->ueIndex ==  macUlSpsStatus_p->ueIndex)
            {
                freeMemPool( macUlSpsStatus_p );
                macUlSpsStatus_p = PNULL;
            }
            else
            {
                /*CA Changes start  */
                /*Cov_fix_30April_63532_start*/
                if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex], macUlSpsStatus_p))
                {
                    freeMemPool( macUlSpsStatus_p );
                    macUlSpsStatus_p = PNULL;
                }
                /*Cov_fix_30April_63532_stop*/
                /*CA Changes end  */
            }

        }    

    }
    
    /*! \code
     * Clears resources reserved by SPS PUSCH if allocatedRB is not 0 
     * \endcode
     * */
    if( 0 != spsUlInfo_p->ulResAllocInfo.allocatedRB )
    {   
        processULResourceReleaseRequest( ulUeCtx_p 
#ifdef FDD_CONFIG
                                        ,SPS_PENDING_DELETE
#endif
                                        /*CA Changes start  */
                                        ,internalCellIndex
                                        /*CA Changes end  */
                                       );
    }
    /* Set SPS state to INVALID_STATE  */
    spsUlInfo_p->spsState = INVALID_STATE;
    /* Set SPS Crnti to INVALID_SPS_CRNTI  */
    spsUlInfo_p->SpsCrnti = INVALID_SPS_CRNTI;
    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_SPS_UL_STR_STATS(ulUeCtx_p->ueIndex, ulUeCtx_p, getCurrentTick(),
            INVALID_CAUSE,  0 , internalCellIndex); 
    /* CA Stats Changes End */
   
    /*! \code
     * Releases UL Setup Info pointer
     * \endcode
     * */
    freeMemPool( spsUlInfo_p->spsUlSetupInfo_p );
    spsUlInfo_p->spsUlSetupInfo_p = PNULL;
}

/********************************************************************************
 * Function Name  : spsUlSort
 * Inputs         : sortedArray - Array to be sorted,
 *                  startingIndex - starting index in array,
 *                  lastIndex - last index in array,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function sorts the array in ascending order using
 *                  insertion sort.
 ********************************************************************************/
void spsUlSort( SpsULStrIntervalTTISorted sortedArray[],
        /*CA Changes start  */
        UInt8 startingIndex, UInt8 lastIndex, InternalCellIndex internalCellIndex )
        /*CA Changes end  */
{
    SInt8 iter = 0;
    SInt8 jter = 0;

    for( iter = startingIndex + 1; iter < lastIndex; iter++  )
    {
        UInt8 currentElement = sortedArray[iter].numSpsUeinRm;
        UInt8 idx =  sortedArray[iter].index;

        jter = iter-1;
        while( (0 <= jter) && 
               (sortedArray[jter].numSpsUeinRm > currentElement) )
        {
            sortedArray[jter+1].numSpsUeinRm =
                                sortedArray[jter].numSpsUeinRm;
            sortedArray[jter+1].index =  sortedArray[jter].index;
            /* Update position in actual array */
            /*CA Changes start  */
            (spsULStrTTIMap_gp[internalCellIndex] + sortedArray[jter+1].index)->\
                indexInSortedArray = jter+1;
            /*CA Changes end  */
            jter--;
        }
        sortedArray[jter+1].numSpsUeinRm = currentElement;
        sortedArray[jter+1].index = idx;
        /* Update position in actual array */
        /*CA Changes start  */
        (spsULStrTTIMap_gp[internalCellIndex] + sortedArray[jter+1].index)->\
            indexInSortedArray = jter+1;
        /*CA Changes end  */
    }
}
/********************************************************************************
 * Function Name  : assignULRBsAndSendToPDCCHFromSpsOccasion
 * Inputs         : ueULContext_p - Pointer to Ue Context,
 *                  delayToPhy - Delay to PHY,
 *                  isTTIBundle - Flag to show status of TTIB enable/disable,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : ULStrategyTxNode - is inserted into the PDCCH New Tx queues
 *                                     which will be used by the PDCCH for CCE
 *                                     distribution
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function assigns Uplink RB's for SPS Occasion and
 *                  prepares the Strategy Node.
 ********************************************************************************/
/* +- SPR 17777 */
MacRetType assignULRBsAndSendToPDCCHFromSpsOccasion(ULUEContext *ueULContext_p 
        /* + SPS_TDD_Changes */
                                                    ,UInt32 delayToPhy 
                                                    /* - SPS_TDD_Changes */
#ifdef FDD_CONFIG
                                                    /* +- SPR 17777 */
                                                    ,UInt8 isTTIBundle
#endif
                                                    /*CA Changes start  */
                                                    ,InternalCellIndex internalCellIndex
                                                    /*CA Changes end  */
                                                    )
{ 
    /* Coverity 32350 Fix Start */
    MacRetType retVal           = MAC_SUCCESS;
    /* Coverity 32350 Fix End */
    /* + SPS_TDD_Changes */
    UInt8             ulHarqProcessId  = INVALID_HARQ_ID;
    /* - SPS_TDD_Changes */
    LP_SpsUlInfo      spsUlInfo_p      = PNULL;
    ULHarqInfo     *ulHarqInfo_p   = PNULL;
    ULStrategyTxNode  *ulStrategyTxNode_p     = PNULL;
    UInt8               lcgId= 0;
    UInt8               containerTick = 0;    
    /* + SPS_TDD_Changes */
    HIPduArray* tempHIPdu_p  = PNULL ;
    spsUlInfo_p = &ueULContext_p->spsUlInfo;
    /* + SPS_TDD_Changes */

    /*! \code
     * - Get the Harq Process and check if it is available
     * \endcode
     * */
    do
    {
        /* TTIB_Code Start */
#ifdef FDD_CONFIG
        if(isTTIBundle)
        {
            ulHarqProcessId = GET_TTIB_HARQ_ID(getCurrentTick() + delayToPhy  + FDD_HARQ_OFFSET);
        }
        else
        {
            ulHarqProcessId = MODULO_EIGHT(getCurrentTick() + delayToPhy  + FDD_HARQ_OFFSET);
        }
        ulHarqInfo_p = &ueULContext_p->ulresInfo[ulHarqProcessId];

        /*Coverity 65586 fix end*/ 

        if(isTTIBundle)
        {
            ulHarqInfo_p->isTTIBundled = TRUE;
        }
        /*condition will not hit in TTIB as check is present in strategy for 
        SPS occasion in current tick and next three ticks  */
        if((HARQ_PROCESS_FREE != ulHarqInfo_p->harqStatus) && (!isTTIBundle) )
        {
            containerTick = ((getCurrentTick() + delayToPhy)% MAX_PDCCH_CONTAINER);
            /* SPR 19704 + */
            UInt8 resetHiIndex = ((getCurrentTick() + delayToPhy) % MAX_PHICH_CONTAINER); 
            /* SPR 19704 - */
            tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick ;

            addEntryTOHIPduArray(
                    ulHarqInfo_p,
                    HI_ACK,
                    ueULContext_p->crnti,
                    ueULContext_p,
                    resetHiIndex,
                    tempHIPdu_p,
                    internalCellIndex);
            /* SPR 16916 FIX START */
            /* SPR 19704 + */
            phichSequenceNoPerPhichGroup_g[internalCellIndex][resetHiIndex][ulHarqInfo_p->nPhichGrp].bitMask &=
                ~(ONE << ulHarqInfo_p->nPhichSeq);
            phichSequenceNoPerPhichGroup_g[internalCellIndex][resetHiIndex][ulHarqInfo_p->nPhichGrp].count++;
            /* SPR 19704 - */
#ifdef KPI_STATS            
            freeULHarqProcess( ulHarqInfo_p,ueULContext_p->ueIndex,internalCellIndex);
#else
            freeULHarqProcess( ulHarqInfo_p);
#endif
            /* SPR 16916 FIX END */
            updateTTIHarqMap(ulHarqProcessId ,ulHarqInfo_p ,ueULContext_p->ttiHarqMap);
        }

        if(!(isTTIBundle))
        {
            /*SPR 6690 fix Start */
            ulHarqInfo_p->harqStatus = HARQ_PROCESS_IN_USE;
            /*SPR 6690 fix End */
        }
        /* save the HARQ Id assigned to hpId[0] in case of TTIB as 
        it will change for TTI Bundled UE for every occasion */
        if(isTTIBundle)
        {
            spsUlInfo_p->hpId[0].harqId = ulHarqProcessId;
        }

        if (ulHarqProcessId ==  spsUlInfo_p->hpId[0].harqId)
        {
            spsUlInfo_p->hpId[0].isDataReceived = FALSE; 
        }
        else if (ulHarqProcessId ==  spsUlInfo_p->hpId[1].harqId)
        {
            spsUlInfo_p->hpId[1].isDataReceived = FALSE; 
        }
        /* CA Stats Changes Start */
        /* Update HARQ process Id Stats */
        LTE_MAC_UPDATE_UL_HARQ_PROCESS_ID_USED( ueULContext_p->ueIndex,
                ulHarqProcessId, internalCellIndex);
        /* CA Stats Changes End */
#elif TDD_CONFIG 
        /* + SPS_TDD_Changes */
        UInt8 subframe = (getCurrentTick() + delayToPhy)%MAX_SUB_FRAME;
        UInt32 sysFrameNum = (MODULO_ONEZEROTWOFOUR(getCurrentTick() / MAX_SUB_FRAME));
        UInt32 ulSubFrame = (subframe + getExactULSubFrame(subframe,internalCellIndex))% MAX_SUBFRAME; 
        /* Get the free ID in ulHarqProcessId  */
        GET_UL_HARQ_ID_FOR_TDD (subframe, ulHarqProcessId, delayToPhy, internalCellIndex); 

        ulHarqInfo_p = &ueULContext_p->ulresInfo[ulHarqProcessId];

        if(HARQ_PROCESS_FREE != ulHarqInfo_p->harqStatus) 
        {
            containerTick = ((getCurrentTick() + delayToPhy)% MAX_PDCCH_CONTAINER);

            tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick ;

            addEntryTOHIPduArray(
                    ulHarqInfo_p,
                    HI_ACK,
                    ueULContext_p->crnti,
                    tempHIPdu_p,
                    /*CA Changes start  */
                    internalCellIndex,
                    /*CA Changes end  */
                    /*TDD Config 0 Changes Start*/ 
                    ulSubFrame 
                    /*TDD Config 0 Changes End*/ 
                    );
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
            freeULHarqProcess(ulHarqInfo_p,ueULContext_p->ueIndex,internalCellIndex);
#else
            freeULHarqProcess(ulHarqInfo_p);
#endif
            /* SPR 16916 FIX END */
            resetHarqId( getULGrantSubframeNum(subframe, 0, internalCellIndex),
                    ueULContext_p, internalCellIndex,
                    sysFrameNum 
                    );
        }
        if (ulHarqProcessId ==  spsUlInfo_p->hpId[0].harqId)
        {
            spsUlInfo_p->hpId[0].isDataReceived = FALSE; 
        }
        if (ulHarqProcessId ==  spsUlInfo_p->hpId[1].harqId)
        {
            spsUlInfo_p->hpId[1].isDataReceived = FALSE; 
        }
        /* - SPS_TDD_Changes */
#endif
        /* TTIB_Code End */

        /*! \code
         * - Fill the values in the ulStrategyTxNode 
         * - put Entry In ULSchedulerNewTxQueue
         * \endcode
         * */

        /*Preparing the UL Strategy Output Node*/
#ifdef FDD_CONFIG
        if(isTTIBundle)
        {
            /* new grant type is added for new SPS+TTIB  grant */
            ulStrategyTxNode_p = fillUlSpsDciInfo( ueULContext_p,/* +- SPR 17777 */
                    TTIB_UL_SPS_OCCASION,
                    /*CA Changes start  */
                    internalCellIndex);  
                    /*CA Changes end  */
        }
        else
#endif
        {
            ulStrategyTxNode_p = fillUlSpsDciInfo( ueULContext_p,/* +- SPR 17777 */
                    UL_SPS_OCCASION, 
                    /*CA Changes start  */
                    internalCellIndex);  
                    /*CA Changes end  */
        }
        /*cov warning 60998 fixed start*/
        if(PNULL == ulStrategyTxNode_p)
        {
            LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                    getCurrentTick(), __LINE__, sizeof(ULStrategyTxNode), 
                    0, 0, 0, 0,0, __FILE__, __func__);
            ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            ltePanic("getMemFromPool fails %s", __func__);

            /* coverity 64946 28June 2014 */ 
            /*Coverity ID <65218> Fix Changes Start*/
#ifdef FDD_CONFIG
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess( ulHarqInfo_p,ueULContext_p->ueIndex,internalCellIndex);
#else
            freeULHarqProcess( ulHarqInfo_p);
#endif
            /* SPR 16916 FIX END */
#endif
            return MAC_FAILURE;
        }
        /*cov warning 60998 fixed end*/
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessId;
        ulStrategyTxNode_p->scheduledCount = 0; 
#endif
        /* - SPS_TDD_Changes */
#ifdef FDD_CONFIG
        if (isTTIBundle)
        {
            ulStrategyTxNode_p->minDataSize = determineUlTBSize(ulStrategyTxNode_p->avgMcs,MAX_TTI_BUNDLE_RB);
            ulStrategyTxNode_p->maxDataSize = ulStrategyTxNode_p->minDataSize;
        }
        else
#endif
        {
            ulStrategyTxNode_p->minDataSize            = spsUlInfo_p->spsOpp;
            ulStrategyTxNode_p->maxDataSize            = spsUlInfo_p->spsOpp;
        }
        ulStrategyTxNode_p->strictAllocRB = TRUE ;
        lcgId = ueULContext_p->lcInfo[spsUlInfo_p->lcId].lcGId;
        /*Subtracting schedulable bytes to keep the remaining Qload*/
        if( ueULContext_p->ulLCGInfo[lcgId].qLoad >  spsUlInfo_p->spsOpp )
            ueULContext_p->ulLCGInfo[lcgId].qLoad -= spsUlInfo_p->spsOpp;
        else
            ueULContext_p->ulLCGInfo[lcgId].qLoad = 0;

        /* Send the trigger further to PDCCH/QOS */
        ulSpsStrategySendPdcchTrigger( ueULContext_p, ulStrategyTxNode_p, 
		/* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                ulSubFrame,
#endif
		/* - SPS_TDD_Changes */
                internalCellIndex );

        numUlSpsUeInTti_g[internalCellIndex]++;
    }while( 0 );

    return retVal;
}

/********************************************************************************
 * Function Name  : fillUlSpsDciInfo
 * Inputs         : ueULContext_p - Pointer to UE context in UL,
 *                  ulHarqInfo_p - Pointer to HARQ node,
 *                  msgType - MsgType used,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : Pointer to DCI node for PDCCH.
 * Description    : This function allocates and fills information for DCI.
 ********************************************************************************/
ULStrategyTxNode * fillUlSpsDciInfo( 
        ULUEContext   *ueULContext_p, 
        /* +- SPR 17777 */
        ULGrantRequestType       msgType,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)  
        /*CA Changes end  */
{
    ULStrategyTxNode       *ulStrNode_p           = PNULL;
    ULQosStrategyLCNode    *ulQosStrategyLCNode_p = PNULL;
    ULLogicalChannelInfo   *lcInfo_p              = PNULL;
    LP_SpsUlInfo           spsUlInfo_p            = PNULL;

    spsUlInfo_p = &ueULContext_p->spsUlInfo;

    /*! \code
     * Allocate node for CCE allocations for PDCCH
     * \endcode 
     * */
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
        GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrNode_p, ULStrategyTxNode);
#else
    ulStrNode_p = (ULStrategyTxNode *)getMemFromPool(
            sizeof(ULStrategyTxNode), PNULL );
#endif
    if( PNULL == ulStrNode_p )
    {
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(ULStrategyTxNode), 
                0, 0, 0, 0,0, __FILE__, __func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
    	/* Klockwork warning Changes Start */
        /*cov_war_60999 fixed start*/
    	return PNULL;
        /*cov_war_60999 fixed end*/
    	/* Klockwork warning Changes End */
    }

    /*! \code
     * Fill node for CCE allocations for PDCCH 
     * \endcode
     * */
    ulStrNode_p->ueIndex = ueULContext_p->ueIndex;
    ulStrNode_p->ulGrantRequestType = msgType;
    ulStrNode_p->minDataSize = 0;
    ulStrNode_p->maxDataSize = 0;
    ulStrNode_p->avgMcs = spsUlInfo_p->ulResAllocInfo.allocatedMCS; 
    ulStrNode_p->ttiCounter = getCurrentTick() ;
#ifdef FDD_CONFIG
    if ( ueULContext_p->ttiBundlingEnabled && (ulStrNode_p->avgMcs > MAX_TTI_BUNDLE_MCS))
    {
        ulStrNode_p->avgMcs = MAX_TTI_BUNDLE_MCS;    
    }
#endif
    if ( UL_SPS_DEACTIVATION != msgType)
        ulStrNode_p->requiredRB =  spsUlInfo_p->ulResAllocInfo.allocatedRB;

    /*CA Changes start  */
    if( UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
        initParams_p->ulSchdConfig.ulSchedulerStrategy &&
    /*CA Changes end  */
            UL_SPS_DEACTIVATION != msgType )
    {
        lcInfo_p = &ueULContext_p->lcInfo[spsUlInfo_p->lcId];

        if( PNULL != lcInfo_p )
        {
            ulQosStrategyLCNode_p = 
                lcInfo_p->ulQosInfo.ulQosStrategyLCNode_p;
            if( PNULL != ulQosStrategyLCNode_p) 
            {
                /* SPR 21925 Fix Start */
                if (FALSE == ulQosStrategyLCNode_p->isLCScheduled)
                {
                    /* SPR 21925 Fix End */
                    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                    ulQosStrategyLCNode_p->scheduledBytes = 
                        spsUlInfo_p->spsOpp;
#endif
                    /* TDD Config 0 Changes Start */
                    ulQosStrategyLCNode_p->ueId = 
                        ueULContext_p->ueIndex;
                    ulQosStrategyLCNode_p->isLCScheduled
                        = TRUE;

                    /* Insert the LC Node in the scheduled LC list */
                    listInsertNode(&(ueULContext_p->ulScheduledLcList),
                            &(ulQosStrategyLCNode_p->scheduledLcNodeAnchor));

                    (ueULContext_p->scheduledLcCount)++;

                    /* State change for QOS to UL_QOS_ALREADY_TRANSMITED */
                    ueULContext_p->scheduleFlag = UL_QOS_ALREADY_TRANSMITED;
                    /* SPR 21925 Fix Start */
                }
                /* SPR 21925 Fix End */
            }
        }
    }
    return ulStrNode_p;
}

/********************************************************************************
 * Function Name  : ulSpsResetOccSchType
 * Inputs         : ttiContext_p - tti Context for ttiIndex for Occasion list,
 *                  count - Number of UEs eligible for SPS occasion in SDL
 * Outputs        : None
 * Returns        : None
 * Description    : This function reset the schType for the UEs in occasion list
 *                  in previous tick.
 ********************************************************************************/
void ulSpsResetOccSchType( LP_SpsULStrIntervalTTIContext ttiContext_p,
    UInt8 count )
{
    LP_MacULSpsOccList  occNode_p      = PNULL;
    ULUEContext       *ueULContext_p = PNULL;
    /*! 
     * \code
     * Reset schType for each node in UL TTI context.
     * \endcode 
     * */
    GET_FIRST_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsULOccasionList, occNode_p );

    /* + Coverity 25237 */
    while( PNULL != occNode_p && count-- )
    /* - Coverity 25237 */
    {        
        ueULContext_p = ulUECtxInfoArr_g[occNode_p->ueIndex].ulUEContext_p;
        if( PNULL == ueULContext_p )
        {
            continue;            
        }
        ueULContext_p->schType = DRA_SCHEDULING;

        /* Get next node of queue */
        GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsULOccasionList,
                                          occNode_p );
    }
}

/********************************************************************************
 * Function Name  : ulSpsResetSdlSchType
 * Inputs         : ttiContext_p - tti Context for ttiIndex for Occasion list,
 *                  count - Number of UEs eligible for SPS occasion in SDL
 * Outputs        : None
 * Returns        : None
 * Description    : This function reset the schType for the UEs in SDL list
 *                  in previous tick.
 ********************************************************************************/
void ulSpsResetSdlSchType( LP_SpsULStrIntervalTTIContext ttiContext_p,
    UInt8 count )
{
    LP_MacULSpsDciListTTI  sdlNode_p        = PNULL;
    ULUEContext          *ueULContext_p   = PNULL;
    /*! 
     * \code
     * Reset schType for each node in TTI context.
     * \endcode 
     * */
    GET_FIRST_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsULDciList, sdlNode_p);

    /* + Coverity 25238 */
    while( PNULL != sdlNode_p && count-- )
    /* - Coverity 25238 */
    {        
        ueULContext_p = ulUECtxInfoArr_g[sdlNode_p->ueIndex].ulUEContext_p;
        if( PNULL == ueULContext_p )
        {
            continue;            
        }
        ueULContext_p->schType = DRA_SCHEDULING;

        /* Get next node of queue */
        GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsULDciList,
                                          sdlNode_p );
    }
}

/********************************************************************************
 * Function Name  : checkSpsBsrOnlyReceived
 * Inputs         : ulCntxt_p - Pointer to ULUEContext
 * Outputs        : None
 * Returns        : flag - TRUE/FALSE
 * Description    : This function checks SPS BSR only Received.
 ********************************************************************************/
UInt8 checkSpsBsrOnlyReceived(ULUEContext* ulCntxt_p)
{    
    UInt8 lcgIndex  = 0;
    UInt8 spsLcg  = 0;
    UInt8 lcId      = 1;
    UInt8 flag=FALSE;
    LP_SpsUlInfo spsUlInfo_p = PNULL;

    spsUlInfo_p = &ulCntxt_p->spsUlInfo;
    if( (SPS_ACTIVATION_INITIATED == spsUlInfo_p->spsState )||
        (SPS_ACTIVATED == spsUlInfo_p->spsState ) )
    { 
	    for (lcId=1; lcId < MAX_NUMBER_OF_LOGICAL_CHANNEL; lcId++)
	    {
		    if(ulCntxt_p->lcInfo[lcId].isSpsLc)
		    {
			    spsLcg=ulCntxt_p->lcInfo[lcId].lcGId;
			    break;
		    }
	    }

	    if ((ulCntxt_p)->bsrNet[spsLcg])
	    {
		    flag=TRUE;
	    }


	    for(lcgIndex = 0; lcgIndex < NUM_OF_LCG; lcgIndex++)
	    {
		    if (lcgIndex == spsLcg)
			    continue;

		    if(ulCntxt_p->lcCount[lcgIndex] && (ulCntxt_p)->bsrNet[lcgIndex])
		    {       
			    flag=FALSE; 
                            break; 
		    }    
	    }
    }
   
    return flag;
}

/********************************************************************************
 * Function Name  : checkIsSpsDataReceivedOnOccasion
 * Inputs         : ulCntxt_p - Pointer to ULUEContext,
 *                  harqIndex - Index of HARQ,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks whether SPS data received on SPS occasion.
 *                  ,and process the implicit release in case of 
 *                  maxumum SPS empty Transmissions happened
 ********************************************************************************/
void checkIsSpsDataReceivedOnOccasion( ULUEContext *ulCntxt_p,
                                       UInt8  harqIndex,
                                       /* + SPS_TDD_Changes */
                                       UInt16 recvdRNTI,
                                       /* - SPS_TDD_Changes */
                                       /*CA Changes start  */
                                       InternalCellIndex internalCellIndex)
                                       /*CA Changes end  */
{
    UInt8 harqArrayIndex=0;
    /* Coverity CID 54669 Start */
    /* + SPS_TDD_Changes */
    if ((PNULL != ulCntxt_p) &&
            (ulCntxt_p->spsUlInfo.SpsCrnti == recvdRNTI))
        /* - SPS_TDD_Changes */
    {
        /* Coverity CID 54669 End */
        /*check if Harq processId is TTI Bundled and SPS enabled */
#ifndef FDD_CONFIG
        if (harqIndex == ulCntxt_p->spsUlInfo.hpId[0].harqId)
#else
            /** SPR 12364 fix Starts**/
            if(((ulCntxt_p->ulresInfo[harqIndex].ulSchReqInfo.isTTIBundled) &&   
                        (ulCntxt_p->ulresInfo[harqIndex].ulSchReqInfo.isSpsEnabled)) ||  
                    (!(ulCntxt_p->ulresInfo[harqIndex].ulSchReqInfo.isTTIBundled)&&
                     (harqIndex == ulCntxt_p->spsUlInfo.hpId[0].harqId)) )
                /** SPR 12364 fix Ends **/
#endif
            {
                harqArrayIndex = 0;
            }
#ifndef FDD_CONFIG
            else if(harqIndex == ulCntxt_p->spsUlInfo.hpId[1].harqId)
#else
                /** SPR 12364 fix Starts**/
            else if(((ulCntxt_p->ulresInfo[harqIndex].ulSchReqInfo.isTTIBundled) &&
                        (ulCntxt_p->ulresInfo[harqIndex].ulSchReqInfo.isSpsEnabled)) ||
                    (!(ulCntxt_p->ulresInfo[harqIndex].ulSchReqInfo.isTTIBundled)&&
                     (harqIndex == ulCntxt_p->spsUlInfo.hpId[1].harqId)) )
                /** SPR 12364 fix Ends **/
#endif
            {
                harqArrayIndex = 1;

            }

        /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
        if ((checkSpsOccasionTti(ulCntxt_p, harqIndex)) &&
#else
        if ((((ulCntxt_p->spsUlInfo.spsState == SPS_ACTIVATED) ||
                (ulCntxt_p->spsUlInfo.spsState == SPS_ACTIVATION_INITIATED) ) &&
                ((ulCntxt_p->spsUlInfo.hpId[0].harqId == harqIndex) ||
                (ulCntxt_p->spsUlInfo.hpId[1].harqId == harqIndex ) ) ) &&
#endif
        /* - SPS_TDD_Changes */
            (ulCntxt_p->spsUlInfo.hpId[harqArrayIndex].isDataReceived == FALSE) )
        {
            ulCntxt_p->spsUlInfo.numOfSureSpsEmptyTx++;

            if( (ulCntxt_p->spsUlInfo.numOfSureSpsEmptyTx + ulCntxt_p->spsUlInfo.numOfUnsureSpsEmptyTx)
                    >= ulCntxt_p->spsUlInfo.spsUlSetupInfo_p->implicitReleaseAfter )
            {
                if (ulCntxt_p->spsUlInfo.numOfSureSpsEmptyTx >= 
                    ulCntxt_p->spsUlInfo.spsUlSetupInfo_p->implicitReleaseAfter)
                {
                    /*CA Changes start  */
                    triggerSpsImplicitRelease(ulCntxt_p, MAX_EMPTY_TX, internalCellIndex);
                } 
                else
                {
                    triggerSpsExplicitRelease(ulCntxt_p, MAX_SPS_HARQ_RETX, internalCellIndex);
                    /*CA Changes end  */
                }
            }
        }
    }
}

/********************************************************************************
 * Function Name  : handleMaxSpsHarqRetxOnOccasion
 * Inputs         : ulCntxt_p - Pointer to ULUEContext,
 *                  harqIndex - Index of HARQ,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles maximum SPS HARQ Re Transmission 
 *                  on SPS occasion,and process the implicit release in case of 
 *                  maxumum SPS empty Transmissions happened
 ********************************************************************************/
void handleMaxSpsHarqRetxOnOccasion( ULUEContext *ulCntxt_p,
        UInt8  harqIndex,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
{
    UInt8 harqArrayIndex=0;
    /* Coverity CID 54670 Start */
    if (PNULL != ulCntxt_p)
    {
        /* Coverity CID 54670 End */
#ifndef FDD_CONFIG
        if (harqIndex == ulCntxt_p->spsUlInfo.hpId[0].harqId)
#else
            if(((ulCntxt_p->ulresInfo[harqIndex].isTTIBundled) &&
                        (ulCntxt_p->ulresInfo[harqIndex].isSpsEnabled)) ||
                    (!(ulCntxt_p->ulresInfo[harqIndex].isTTIBundled)&&
                     (harqIndex == ulCntxt_p->spsUlInfo.hpId[0].harqId)) )
#endif
            {
                harqArrayIndex = 0;
            }
#ifndef FDD_CONFIG
            else if(harqIndex == ulCntxt_p->spsUlInfo.hpId[1].harqId)
#else
            else if(((ulCntxt_p->ulresInfo[harqIndex].isTTIBundled) &&
                        (ulCntxt_p->ulresInfo[harqIndex].isSpsEnabled)) ||
                    (!(ulCntxt_p->ulresInfo[harqIndex].isTTIBundled)&&
                     (harqIndex == ulCntxt_p->spsUlInfo.hpId[1].harqId)) )
#endif
            {
                harqArrayIndex = 1;

            }

        if (checkSpsOccasionTti(ulCntxt_p, harqIndex) &&
                (ulCntxt_p->spsUlInfo.hpId[harqArrayIndex].isDataReceived == FALSE) )
        {
            ulCntxt_p->spsUlInfo.numOfUnsureSpsEmptyTx++;

            if( (ulCntxt_p->spsUlInfo.numOfSureSpsEmptyTx + ulCntxt_p->spsUlInfo.numOfUnsureSpsEmptyTx)
                    >= ulCntxt_p->spsUlInfo.spsUlSetupInfo_p->implicitReleaseAfter )
            {
                if (ulCntxt_p->spsUlInfo.numOfSureSpsEmptyTx >= 
                        ulCntxt_p->spsUlInfo.spsUlSetupInfo_p->implicitReleaseAfter)
                {
                    /*CA Changes start  */
                    triggerSpsImplicitRelease(ulCntxt_p, MAX_EMPTY_TX, internalCellIndex);
                } 
                else
                {
                    triggerSpsExplicitRelease(ulCntxt_p, MAX_SPS_HARQ_RETX, internalCellIndex);
                    /*CA Changes end  */
                }
            }
        }
    }
}

/********************************************************************************
 * Function Name  : triggerSpsImplicitRelease
 * Inputs         : ulCntxt_p - Pointer to ULUEContext,
 *                  event - SPS Cause,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the SPS implicit release and the Deactivate 
 *                  node into Deactivate Queue
 ********************************************************************************/
void triggerSpsImplicitRelease(ULUEContext *ulCntxt_p, SpsCause event, InternalCellIndex internalCellIndex)
{
    LP_MacULSpsDeactReq       macUlSpsDectReq_p = PNULL;

	/* Allocate the memory for deactivation queue node */
    macUlSpsDectReq_p = (LP_MacULSpsDeactReq)getMemFromPool(
    sizeof(MacULSpsDeactReq), PNULL);
    if( PNULL == macUlSpsDectReq_p )
    {
 	    LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL,
				L2_SYS_FAIL, getCurrentTick(), __LINE__,
				sizeof(MacULSpsDeactReq), 0, 0, 0, 0,0,
				__FILE__, __func__);
	    ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
				CRITICAL_ALARM);
	    ltePanic("getMemFromPool fails %s", __func__);
    	/* Klockwork warning Changes Start */
    	return ;
    	/* Klockwork warning Changes End */
    }
    /* Fill deactivation queue node */
    macUlSpsDectReq_p->ueIndex = ulCntxt_p->ueIndex;
    macUlSpsDectReq_p->cause = event;
   /* Enqueue the node to deactivation queue so that it
    *                      * can be processed in next tick */
    /*CA Changes start  */
    /* + Coverity Id - 32611 */
    if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsDeactQ_gp[internalCellIndex],
			macUlSpsDectReq_p ))
    {
        freeMemPool(macUlSpsDectReq_p);
    }
    /* - Coverity Id - 32611 */

    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( UL_SPS_DEACT_Q,
    		QCOUNT_MAC_UL_SPS_Q( ulSpsDeactQ_gp[internalCellIndex] ),
            internalCellIndex);
    /* CA Stats Changes End */
    /*CA Changes end  */

}

/********************************************************************************
 * Function Name  : triggerSpsExplicitRelease
 * Inputs         : ulCntxt_p - Pointer to ULUEContext,
 *                  event - SPS Cause,
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function triggers SPS Explicit release.
 ********************************************************************************/
void triggerSpsExplicitRelease(ULUEContext *ulCntxt_p, SpsCause event, InternalCellIndex internalCellIndex)
{
    LP_MacULSpsDeactReq       macUlSpsDectReq_p = PNULL;

    macUlSpsDectReq_p = (LP_MacULSpsDeactReq)getMemFromPool(
    sizeof(MacULSpsDeactReq), PNULL);
    if( PNULL == macUlSpsDectReq_p )
    {
 	    LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL,
				L2_SYS_FAIL, getCurrentTick(), __LINE__,
				sizeof(MacULSpsDeactReq), 0, 0, 0, 0,0,
				__FILE__, __func__);
	    ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
				CRITICAL_ALARM);
	    ltePanic("getMemFromPool fails %s", __func__);
    	/* Klockwork warning Changes Start */
    	return ;
    	/* Klockwork warning Changes End */
    }
    /* Fill deactivation queue node */
    macUlSpsDectReq_p->ueIndex = ulCntxt_p->ueIndex;
    macUlSpsDectReq_p->cause = event;
   /* Enqueue the node to deactivation queue so that it
    * can be processed in next tick */
    /*CA Changes start  */
    /* + Coverity Id - 32610 */
    if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsDeactQ_gp[internalCellIndex],
			macUlSpsDectReq_p ))
    {
        freeMemPool(macUlSpsDectReq_p);
    }
    /* - Coverity Id - 32610 */

    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( UL_SPS_DEACT_Q,
    		QCOUNT_MAC_UL_SPS_Q( ulSpsDeactQ_gp[internalCellIndex] ),
            internalCellIndex);
    /* CA Stats Changes End */
    /*CA Changes end  */

}

/** SPR 12364 fix Starts**/
/******************************************************************************
 * Function Name  : checkSpsOccasionTtiFrmDemux
 * Inputs         : ulCntxt_p - Pointer to ULUEContext,
 *                  harqIndex - Index of HARQ
 * Outputs        : None
 * Returns        : TRUE/FALSE
 * Description    : This function checks SPS occasion TTI from local backup 
 *                  created when CRC Ack is received.
 *****************************************************************************/
UInt8 checkSpsOccasionTtiFrmDemux( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex)
{
#ifdef FDD_CONFIG
    /* identify that Harq is for sps occasion by isSpsEnabled */
    if(ulCntxt_p->ulresInfo[harqIndex].ulSchReqInfo.isTTIBundled)   
    {
        if(ulCntxt_p->spsUlInfo.spsState == SPS_ACTIVATED)
        {
            return ulCntxt_p->ulresInfo[harqIndex].ulSchReqInfo.isSpsEnabled; 
        }
        else
        {
            return FALSE;
        }
    }
    else
#endif        
    { 
        if ( (ulCntxt_p->spsUlInfo.spsState == SPS_ACTIVATED) &&
                ( (ulCntxt_p->spsUlInfo.hpId[0].harqId == harqIndex) || 
                  (ulCntxt_p->spsUlInfo.hpId[1].harqId == harqIndex ) ) )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}
/** SPR 12364 fix Ends **/  

/********************************************************************************
 * Function Name  : checkSpsOccasionTti
 * Inputs         : ulCntxt_p - Pointer to ULUEContext,
 *                  harqIndex - Index of HARQ
 * Outputs        : None
 * Returns        : TRUE/FALSE
 * Description    : This function checks SPS occasion TTI.
 ********************************************************************************/
UInt8 checkSpsOccasionTti( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex)
{
#ifdef FDD_CONFIG
    /* identify that Harq is for sps occasion by isSpsEnabled */
    if(ulCntxt_p->ulresInfo[harqIndex].isTTIBundled)
    {
        if(ulCntxt_p->spsUlInfo.spsState == SPS_ACTIVATED)
        {
            return ulCntxt_p->ulresInfo[harqIndex].isSpsEnabled;
        }
        else
        {
           return FALSE;
        }
    }
    else
#endif        
    { 
        if ( (ulCntxt_p->spsUlInfo.spsState == SPS_ACTIVATED) &&
                ( (ulCntxt_p->spsUlInfo.hpId[0].harqId == harqIndex) || 
                (ulCntxt_p->spsUlInfo.hpId[1].harqId == harqIndex ) ) )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

/********************************************************************************
 * Function Name  : checkSpsActTti
 * Inputs         : ulCntxt_p - Pointer to ULUEContext,
 *                  harqIndex - Index of HARQ
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks SPS activation TTI.
 ********************************************************************************/
UInt8 checkSpsActTti( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex)
{
#ifdef FDD_CONFIG
    /* identify that Harq is for sps occasion by isSpsEnabled */
    if(ulCntxt_p->ulresInfo[harqIndex].isTTIBundled)
    {
        if(ulCntxt_p->spsUlInfo.spsState == SPS_ACTIVATION_INITIATED )
        {
            return ulCntxt_p->ulresInfo[harqIndex].isSpsEnabled;
        }
        else
        {
            return FALSE;
        }
    }
    else
#endif        
    { 
        if ( (ulCntxt_p->spsUlInfo.spsState == SPS_ACTIVATION_INITIATED) &&
                ( (ulCntxt_p->spsUlInfo.hpId[0].harqId == harqIndex) || 
                (ulCntxt_p->spsUlInfo.hpId[1].harqId == harqIndex ) ) )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}
/* + SPS_TDD_Changes */
/********************************************************************************
 * Function Name  :checkSpsDciPerTTI 
 * Inputs         : index: TTI Context Map index for TDD
 *                   ulSpsInterval: map size
 *                  ttiContext_p - ttiContext
 *                  spsCtx_p : statusQ node
                     
 * Outputs        : retVal = MAC_SUCCESS or MAC_FAILURE
 * Returns        : None
 * Description    : This function checks if SDL node can be inserted or not.
 ********************************************************************************/
MacRetType checkSpsDciPerTTI(
#ifdef TDD_CONFIG
        SInt8 subframe_offset,
        UInt16 index,
        UInt16 ulSpsInterval,
#endif
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        void * spsCtx_p,
       InternalCellIndex internalCellIndex)
{
    /* + coverity 32350 &  87545 */
    MacRetType retVal = MAC_FAILURE;
    /* - coverity 32350 & 87545 */
    UInt8 maxUlSpsDciPerTti = 0;
    LP_MacULSpsStatus    spsUlStatus_p  = (LP_MacULSpsStatus)spsCtx_p;
#ifdef TDD_CONFIG
    UInt8 spsPeriodicity = 0;
    UInt16 nextTTIIndex = 0;
    LP_SpsULStrIntervalTTIContext  ttiContextNextPeriodicity_p  = PNULL;
    LP_SpsUlInfo spsUlInfo_p = PNULL;
    spsUlInfo_p = &ulUECtxInfoArr_g[spsUlStatus_p->ueIndex].ulUEContext_p->spsUlInfo ;
    spsPeriodicity = spsUlInfo_p->spsUlSetupInfo_p->semiPersistentSchedIntervalUL;
    /* For 20ms, the third occurence is same as initial occurence irrespective of twoIntervalConfig
       therefore subFrameOffset is not considered */ 
    if(SPS_INTERVAL_20 == spsIntervalMap_g[spsPeriodicity])
    {
        nextTTIIndex = (index + SPS_INTERVAL_40) % ulSpsInterval ;
    }
    else
    {
        nextTTIIndex = (index + subframe_offset + SPS_INTERVAL_40) % ulSpsInterval ;
    }
    ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 
#endif
    maxUlSpsDciPerTti = (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                        spsUlSchInfoPerTti.maxUlSpsDciPerTti;
    if(( ttiContext_p->numULSpsDciPerTti == maxUlSpsDciPerTti)
#ifdef TDD_CONFIG
            || (ttiContextNextPeriodicity_p->numULSpsDciPerTti == maxUlSpsDciPerTti) 
#endif
      )
    {
        /* Enqueue back to status queue as this node cannot be processed
         * in this TTI. */
        /*CA Changes start  */
        /*Cov_Fix_5May_63535_Start */
        /* SPR 23483 Changes Start */
        if(CIRC_SUCCESS == ENQUEUE_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex], spsUlStatus_p))
        /* SPR 23483 Changes End */
        {    
            /*Cov_Fix_5May_63535_Stop */
            /* Update Q Stats */
            /* CA Stats Changes Start */
            LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q, 
                    QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ),
                    internalCellIndex);
            /*CA Changes end  */
        }
        return retVal ;
    }
    retVal = MAC_SUCCESS;
    return retVal;
}
/********************************************************************************
 * Function Name  :checkSpsOccPerTTI 
 * Inputs         : index: TTI Context Map index for TDD
 *                   ulSpsInterval: map size
 *                  ttiContext_p - ttiContext
 *                  spsCtx_p : statusQ node
                     
 * Outputs        : retVal = MAC_SUCCESS or MAC_FAILURE
 * Returns        : None
 * Description    : This function checks if occasion list node can be inserted or not.
 ********************************************************************************/
MacRetType checkSpsOccPerTTI(
#ifdef TDD_CONFIG
        SInt8 subframe_offset,
        UInt16 index,
        UInt16 ulSpsInterval,
#endif
        LP_SpsULStrIntervalTTIContext ttiContext_p,
        void * spsCtx_p,
       InternalCellIndex internalCellIndex)
{
    /*Coverity 32350: Fix*/
    MacRetType retVal = MAC_SUCCESS;
    /*Coverity 32350: Fix*/
    UInt8 maxUlSpsOccasionPerTti = 0;
    LP_MacULSpsStatus    spsUlStatus_p  = (LP_MacULSpsStatus)spsCtx_p;
#ifdef TDD_CONFIG
    UInt8 spsPeriodicity = 0;
    UInt16 nextTTIIndex = 0;
    LP_SpsULStrIntervalTTIContext  ttiContextNextPeriodicity_p  = PNULL;
    LP_SpsUlInfo spsUlInfo_p = PNULL;
    spsUlInfo_p = &ulUECtxInfoArr_g[spsUlStatus_p->ueIndex].ulUEContext_p->spsUlInfo ;
    spsPeriodicity = spsUlInfo_p->spsUlSetupInfo_p->semiPersistentSchedIntervalUL;
    /* For 20ms, the third occurence is same as initial occurence irrespective of twoIntervalConfig
     therefore subFrameOffset is not considered */ 
    if(SPS_INTERVAL_20 == spsIntervalMap_g[spsPeriodicity])
    {
        nextTTIIndex = (index + SPS_INTERVAL_40) % ulSpsInterval ;
    }
    else
    {
        nextTTIIndex = (index + subframe_offset + SPS_INTERVAL_40) % ulSpsInterval ;
    }
    /* Getting the ttiContext for nextIndex */
    ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIIndex; 
#endif
    maxUlSpsOccasionPerTti = (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                        spsUlSchInfoPerTti.maxUlSpsOccasionPerTti;
    if(( ttiContext_p->numULSpsOccasionPerTti == maxUlSpsOccasionPerTti)
#ifdef TDD_CONFIG
            || (ttiContextNextPeriodicity_p->numULSpsOccasionPerTti == maxUlSpsOccasionPerTti) 
#endif
      )
    {
        /* Enqueue back to status queue as this node cannot be processed
         * in this TTI. */
        /*CA Changes start  */
        /*Cov_Fix_5May_63533_Start*/
        /* SPR 23483 Changes Start */
        if(CIRC_SUCCESS ==  ENQUEUE_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex], spsUlStatus_p))
        /* SPR 23483 Changes End */
        {    
            /*Cov_Fix_5May_63533_Stop*/
            /* Update Q Stats */
            LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q,  
                    QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ),
                    internalCellIndex );            
            /*CA Changes end  */
        }
    /*Coverity 87545: Fix*/
        retVal = MAC_FAILURE;
    /*Coverity 87545: Fix*/
    }
    return retVal;
}
/********************************************************************************
 * Function Name  :ulAddUeInSdlAtPeriodicity 
 * Inputs         : ueIndex 
 *                  ttiContext_p : TTI Context
                     
 * Outputs        : sdlNode_p : Pointer to the inserted node 
 * Returns        : None
 * Description    : This function inserts the SDL node at specific TTI index in TTI Map
 ********************************************************************************/
LP_MacULSpsDciListTTI  ulAddUeInSdlAtPeriodicity(UInt16 ueIndex, LP_SpsULStrIntervalTTIContext ttiContext_p)
{
    LP_MacULSpsDciListTTI  sdlNode_p = PNULL;

    /* Allocate node for SDL List entry */
    sdlNode_p = (LP_MacULSpsDciListTTI)getMemFromPool( 
                         sizeof(MacULSpsDciListTTI), PNULL );
    /*CoverityId:87595; SPR:19339 fix*/
    if( PNULL != sdlNode_p )
    {
        sdlNode_p->ueIndex = ueIndex;
        /* Enqueue in SDL */
        ENQUEUE_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsULDciList, sdlNode_p);

        /* Increment number of SPS DCI in the TTI */
        (ttiContext_p->numULSpsDciPerTti)++;

        return sdlNode_p;  
    }
    else
    {
       LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
               getCurrentTick(), __LINE__, sizeof(MacULSpsDciListTTI), 0,0,
               0, 0,0, __FILE__, __func__);
       ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
       ltePanic("getMemFromPool fails %s", __func__);
       return PNULL;
    }
    /*CoverityId:87595; SPR:19339 fix*/

        
}
/********************************************************************************
 * Function Name  :ulAddUeInOccListAtPeriodicity 
 * Inputs         : ueIndex 
 *                  ttiContext_p : TTI Context
                     
 * Outputs        : occNode_p: pointer to the node inserted
 * Returns        : None
 * Description    : This function inserts the SDL node at specific TTI index in TTI Map
 ********************************************************************************/
LP_MacULSpsOccList  ulAddUeInOccListAtPeriodicity(UInt16 ueIndex, LP_SpsULStrIntervalTTIContext ttiContext_p)
{
    LP_MacULSpsOccList occNode_p = PNULL; 
    /* Allocate node for PDSCH Occasion List entry */
    occNode_p = (LP_MacULSpsOccList)getMemFromPool( 
            sizeof(MacULSpsOccList), PNULL );
    /*CoverityId:87596; SPR:19339 fix*/
    if( PNULL != occNode_p )
    {
        occNode_p->ueIndex = ueIndex;
        ENQUEUE_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsULOccasionList, occNode_p);
#ifdef FDD_CONFIG
        /* TTIB_Code Start */
        /* If UE for which occ node is to be added is TTI Bundling 
           then set it true */
        if(ulUECtxInfoArr_g[ueIndex].ulUEContext_p->ttiBundlingEnabled)
        {
            occNode_p->isTTIBundle = TRUE;
        }
        else
        {
            occNode_p->isTTIBundle = FALSE;
        }
        /* TTIB_Code End */
#endif    
        /* Increment number of SPS occasions in the TTI */
        (ttiContext_p->numULSpsOccasionPerTti)++;

        return occNode_p;
    }
    else
    {
        LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacULSpsOccList), 0,0,
                0, 0,0, __FILE__, __func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM );
        ltePanic("getMemFromPool fails %s", __func__);
        return PNULL;
    }
    /*CoverityId:87596; SPR:19339 fix*/

}    
/* - SPS_TDD_Changes */
