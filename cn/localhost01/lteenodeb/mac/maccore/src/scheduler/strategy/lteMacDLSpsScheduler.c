/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLSpsScheduler.c,v  $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the functionality for SPS scheduler.
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
#include "lteMacDLSpsScheduler.h"
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacDLHarqMgr.h"
#include "lteMacSpsPDSCHmgr.h"
#include "lteMacDLQosScheduler.h"

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
LTE_CIRCQUEUE *dlSpsResRsvQ_gp[MAX_NUM_CELL];
LTE_CIRCQUEUE *dlSpsResRelQ_gp[MAX_NUM_CELL];
LTE_CIRCQUEUE *dlSpsStatusQ_gp[MAX_NUM_CELL];
LTE_CIRCQUEUE *dlSpsActQ_gp[MAX_NUM_CELL];
LTE_CIRCQUEUE *dlSpsDeactQ_gp[MAX_NUM_CELL];
LTE_CIRCQUEUE *dlSpsReactQ_gp[MAX_NUM_CELL];
LTE_CIRCQUEUE *dlSpsUeExclude_gp[MAX_NUM_CELL];

SpsStrIntervalTTIContext spsStrTTIMap_g[MAX_NUM_CELL][MAX_SPS_INTERVAL] = {{{0}}};

/* Number of RBs consumed by SPS strategy in a TTI so that it can be subtracted
 * from DRA strategies */
UInt8 numDlSpsRbsInTti_g[MAX_NUM_CELL] = {0};
/* Number of UEs that needs to be scheduled by SPS strategy in a TTI so that it 
 * can be added in total UEs to be scheduled in DRA strategies */
UInt8 numDlSpsUeInTti_g[MAX_NUM_CELL] = {0};


/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
STATIC MacRetType dlSpsNotValidEvent( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsUeSyncLossOff( UInt16 ueIndex, SpsStates state,
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsImplicitRelease( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsUeImplicitRelPdschResp( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsUeImplicitReleaseinSdl( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsUeImplicitReleaseinSdlInDeactInit( UInt16 ueIndex,
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p,
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsImplicitReleaseinOccList( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsConfigured( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsResRsvReq( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsActivationPdschSuccess( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsActivationPdschFail( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy,InternalCellIndex internalCellIndex);
STATIC MacRetType dlSpsActivated( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsUeSyncLossDetectedinSdl( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsUeSyncLossDetectedinSdlInDeactInit( UInt16 ueIndex,
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p,
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsUeSyncLossDetectedinOccList( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsUeSyncLossDetectedinDra( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsUeSyncLossDetectedPdschResp( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsMaxDciAttemptReached( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsMaxDciAttemptReachedForDeact( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsDeactInit( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex);
STATIC MacRetType dlSpsMaxHarqRetx( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsDeactivated( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType dlSpsDeactPdcchFail( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex );
STATIC MacRetType processDLSpsOccList( 
        LP_SpsStrIntervalTTIContext spsStrIntervalTTI_p,
        UInt8 spsOccasionUeCount, UInt16 ttiIndex, UInt32 delayToPhy ,
        InternalCellIndex internalCellIndex);
STATIC MacRetType processDLSpsSdl( LP_SpsStrIntervalTTIContext spsTTIContext_p,
        UInt8 sdlCount, UInt32 delayToPhy,
        InternalCellIndex internalCellIndex);
STATIC MacRetType assignDLRBsAndSendToPDCCHFromSpsOccasion(
        DLUEContext *ueDLContext_p ,InternalCellIndex internalCellIndex);
/* CA Stats Changes Start */
STATIC void dlAddUeInSdl( UInt16 ueIndex, 
        LP_SpsStrIntervalTTIContext ttiContext_p,
        LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p,
        LP_SpsDlInfo spsDlInfo_p,
        InternalCellIndex   internalCellIndex);
STATIC void dlDeleteUeFromSdl( LP_SpsStrIntervalTTIContext ttiContext_p,
        LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p,
        /* +- SPR 17777 */
        LP_SpsDlInfo spsDlInfo_p
#ifdef SSI_DEBUG_STATS
        ,InternalCellIndex   internalCellIndex
#endif
        );
/* +- SPR 17777 */
STATIC void dlAddUeInOccasionList(  UInt16 ueIndex,
        LP_SpsStrIntervalTTIContext ttiContext_p,
        LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p,
        LP_SpsDlInfo spsDlInfo_p,
        InternalCellIndex   internalCellIndex);
STATIC void dlDeleteUeFromOccasionList( 
        LP_SpsStrIntervalTTIContext ttiContext_p,
        LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p,
        LP_SpsDlInfo       spsDlInfo_p
#ifdef SSI_DEBUG_STATS
        ,InternalCellIndex   internalCellIndex
#endif
        );
/* + SPR 20756 */
STATIC void checkAndFreeSpsHarqProcess( DLUEContext *dlUeCtx_p, 
        InternalCellIndex intCellIdx );
/* - SPR 20756 */
void dlSpsResetOccSchType( LP_SpsStrIntervalTTIContext ttiContext_p,
    UInt8 count );
void dlSpsResetSdlSchType( LP_SpsStrIntervalTTIContext ttiContext_p,
    UInt8 count );

/* State machine to handle startegy actions for various states */
macStrategyHandlerFuncT dlStateMachine[MAX_SPS_STATE][MAX_SPS_CAUSE] =
{
    { /* SPS_IDLE */
        dlSpsNotValidEvent  /* UE_SYNC_LOSS_ON  */,
        dlSpsNotValidEvent  /* UE_SYNC_LOSS_OFF */,
        dlSpsConfigured     /* SPS_CONFIG_RECVD */,
        dlSpsNotValidEvent  /* FIRST_NON_ZERO_QUEUE_LOAD */,
        dlSpsNotValidEvent  /* VOICE_PACKET */,
        dlSpsNotValidEvent  /* SPS_PDSCH_PUSCH_SUCCESS */,
        dlSpsNotValidEvent  /* SPS_PDSCH_PUSCH_FAILURE */,
        dlSpsNotValidEvent  /* FIRST_ACK_NACK */,
        dlSpsNotValidEvent  /* PDCCH_CCE_ALLOC_FAILURE */,
        dlSpsNotValidEvent  /* HARQ_TIMER_EXPIRED */,
        dlSpsNotValidEvent  /* SILENCE_DETECTED */,
        dlSpsNotValidEvent  /* MCS_CHANGED */,
        dlSpsNotValidEvent  /* BLER_THRESHOLD */,
        dlSpsNotValidEvent  /* PDU_SEG_IND */,
        dlSpsNotValidEvent  /* MAX_DCI_ATTEMPT */,
        dlSpsNotValidEvent  /* MAX_EMPTY_TX */,
        dlSpsNotValidEvent  /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        dlSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
        /* + SPS_TDD_Changes */
        dlSpsNotValidEvent /*MEAS_GAP_RECONFIG */,
        dlSpsNotValidEvent/*DRX_RECONFIG */,
       /* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        dlSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_CONFIGURED */
        dlSpsUeSyncLossDetectedinDra /* UE_SYNC_LOSS_ON  */,
        dlSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        dlSpsImplicitRelease /* SPS_CONFIG_RECVD */,
        dlSpsResRsvReq       /* FIRST_NON_ZERO_QUEUE_LOAD */,
        dlSpsNotValidEvent   /* VOICE_PACKET */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_SUCCESS */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_FAILURE */,
        dlSpsNotValidEvent   /* FIRST_ACK_NACK */,
        dlSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        dlSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        dlSpsNotValidEvent   /* SILENCE_DETECTED */,
        dlSpsNotValidEvent   /* MCS_CHANGED */,
        dlSpsNotValidEvent   /* BLER_THRESHOLD */,
        dlSpsNotValidEvent   /* PDU_SEG_IND */,
        dlSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        dlSpsNotValidEvent   /* MAX_EMPTY_TX */,
        dlSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        dlSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
        /* + SPS_TDD_Changes */
        dlSpsNotValidEvent /*MEAS_GAP_RECONFIG */,
        dlSpsNotValidEvent /*DRX_RECONFIG */,
       /* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        dlSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_RES_RSV_REQ_INITIATED */
        dlSpsUeSyncLossDetectedPdschResp   /* UE_SYNC_LOSS_ON  */,
        dlSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        dlSpsUeImplicitRelPdschResp /* SPS_CONFIG_RECVD */,
        dlSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        dlSpsNotValidEvent   /* VOICE_PACKET */,
        dlSpsActivationPdschSuccess /* SPS_PDSCH_PUSCH_SUCCESS */,
        dlSpsActivationPdschFail    /* SPS_PDSCH_PUSCH_FAILURE */,
        dlSpsNotValidEvent   /* FIRST_ACK_NACK */,
        dlSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        dlSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        dlSpsNotValidEvent   /* SILENCE_DETECTED */,
        dlSpsNotValidEvent   /* MCS_CHANGED */,
        dlSpsNotValidEvent   /* BLER_THRESHOLD */,
        dlSpsNotValidEvent   /* PDU_SEG_IND */,
        dlSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        dlSpsNotValidEvent   /* MAX_EMPTY_TX */,
        dlSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        dlSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
        /* + SPS_TDD_Changes */
        dlSpsNotValidEvent /*MEAS_GAP_RECONFIG */,
        dlSpsNotValidEvent /*DRX_RECONFIG */,
       /* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        dlSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_ACTIVATION_INITIATED */
        dlSpsUeSyncLossDetectedinSdl  /* UE_SYNC_LOSS_ON  */,
        dlSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        dlSpsUeImplicitReleaseinSdl /* SPS_CONFIG_RECVD */,
        dlSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        dlSpsNotValidEvent   /* VOICE_PACKET */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_SUCCESS */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_FAILURE */,
        dlSpsActivated       /* FIRST_ACK_NACK */,
        dlSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        dlSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        dlSpsNotValidEvent   /* SILENCE_DETECTED */,
        dlSpsNotValidEvent   /* MCS_CHANGED */,
        dlSpsNotValidEvent   /* BLER_THRESHOLD */,
        dlSpsNotValidEvent   /* PDU_SEG_IND */,
        dlSpsMaxDciAttemptReached /* MAX_DCI_ATTEMPT */,
        dlSpsNotValidEvent   /* MAX_EMPTY_TX */,
        dlSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        dlSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
        /* + SPS_TDD_Changes */
        dlSpsUeImplicitReleaseinSdl /*MEAS_GAP_RECONFIG */,
        dlSpsUeImplicitReleaseinSdl /*DRX_RECONFIG */,
       /* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        dlSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_ACTIVATED */
        dlSpsUeSyncLossDetectedinOccList /* UE_SYNC_LOSS_ON */,
        dlSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        dlSpsImplicitReleaseinOccList /* SPS_CONFIG_RECVD */,
        dlSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        dlSpsNotValidEvent   /* VOICE_PACKET */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_SUCCESS */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_FAILURE */,
        dlSpsNotValidEvent   /* FIRST_ACK_NACK */,
        dlSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        dlSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        dlSpsDeactInit       /* SILENCE_DETECTED */,
        dlSpsDeactInit       /* MCS_CHANGED */,
        dlSpsDeactInit       /* BLER_THRESHOLD */,
        dlSpsDeactInit       /* PDU_SEG_IND */,
        dlSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        dlSpsDeactInit       /* MAX_EMPTY_TX */,
        dlSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        dlSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
        /* + SPS_TDD_Changes */
        dlSpsDeactInit /*MEAS_GAP_RECONFIG */,
        dlSpsDeactInit /*DRX_RECONFIG */,
       /* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        dlSpsDeactInit  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_DEACTIVATION_INITIATED */
        dlSpsUeSyncLossDetectedinSdlInDeactInit  /* UE_SYNC_LOSS_ON */,
        dlSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        dlSpsUeImplicitReleaseinSdlInDeactInit   /* SPS_CONFIG_RECVD */,
        dlSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        dlSpsNotValidEvent   /* VOICE_PACKET */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_SUCCESS */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_FAILURE */,
        dlSpsDeactivated     /* FIRST_ACK_NACK */,
        dlSpsDeactPdcchFail  /* PDCCH_CCE_ALLOC_FAILURE */,
        dlSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        dlSpsNotValidEvent   /* SILENCE_DETECTED */,
        dlSpsNotValidEvent   /* MCS_CHANGED */,
        dlSpsNotValidEvent   /* BLER_THRESHOLD */,
        dlSpsNotValidEvent   /* PDU_SEG_IND */,
        dlSpsMaxDciAttemptReachedForDeact /* MAX_DCI_ATTEMPT */,
        dlSpsNotValidEvent   /* MAX_EMPTY_TX */,
        dlSpsMaxHarqRetx     /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        dlSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
        /* + SPS_TDD_Changes */
        dlSpsNotValidEvent /*MEAS_GAP_RECONFIG */,
        dlSpsNotValidEvent /*DRX_RECONFIG */,
       /* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        dlSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_DEACTIVATED */
        dlSpsUeSyncLossDetectedinDra /* UE_SYNC_LOSS_ON  */,
        dlSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        dlSpsImplicitRelease /* SPS_CONFIG_RECVD */,
        dlSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        dlSpsResRsvReq       /* VOICE_PACKET */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_SUCCESS */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_FAILURE */,
        dlSpsNotValidEvent   /* FIRST_ACK_NACK */,
        dlSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        dlSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        dlSpsNotValidEvent   /* SILENCE_DETECTED */,
        dlSpsNotValidEvent   /* MCS_CHANGED */,
        dlSpsNotValidEvent   /* BLER_THRESHOLD */,
        dlSpsNotValidEvent   /* PDU_SEG_IND */,
        dlSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        dlSpsNotValidEvent   /* MAX_EMPTY_TX */,
        dlSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        dlSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
        /* + SPS_TDD_Changes */
        dlSpsNotValidEvent /*MEAS_GAP_RECONFIG */,
        dlSpsNotValidEvent /*DRX_RECONFIG */,
       /* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        dlSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* SPS_REACTIVATION_INITIATED */
        dlSpsUeSyncLossDetectedinSdl /* UE_SYNC_LOSS_ON */,
        dlSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        dlSpsUeImplicitReleaseinSdl /* SPS_CONFIG_RECVD */,
        dlSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        dlSpsNotValidEvent   /* VOICE_PACKET */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_SUCCESS */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_FAILURE */,
        dlSpsNotValidEvent   /* FIRST_ACK_NACK */,
        dlSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        dlSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        dlSpsNotValidEvent   /* SILENCE_DETECTED */,
        dlSpsNotValidEvent   /* MCS_CHANGED */,
        dlSpsNotValidEvent   /* BLER_THRESHOLD */,
        dlSpsNotValidEvent   /* PDU_SEG_IND */,
        dlSpsMaxDciAttemptReached /* MAX_DCI_ATTEMPT */,
        dlSpsNotValidEvent   /* MAX_EMPTY_TX */,
        dlSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        dlSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
        /* + SPS_TDD_Changes */
        dlSpsNotValidEvent /*MEAS_GAP_RECONFIG */,
        dlSpsNotValidEvent /*DRX_RECONFIG */,
       /* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        dlSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    { /* UE_NOT_READY */
        dlSpsNotValidEvent   /* UE_SYNC_LOSS_ON  */,
        dlSpsUeSyncLossOff   /* UE_SYNC_LOSS_OFF */,
        dlSpsNotValidEvent   /* SPS_CONFIG_RECVD */,
        dlSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        dlSpsNotValidEvent   /* VOICE_PACKET */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_SUCCESS */,
        dlSpsNotValidEvent   /* SPS_PDSCH_PUSCH_FAILURE */,
        dlSpsNotValidEvent   /* FIRST_ACK_NACK */,
        dlSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        dlSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        dlSpsNotValidEvent   /* SILENCE_DETECTED */,
        dlSpsNotValidEvent   /* MCS_CHANGED */,
        dlSpsNotValidEvent   /* BLER_THRESHOLD */,
        dlSpsNotValidEvent   /* PDU_SEG_IND */,
        dlSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        dlSpsNotValidEvent   /* MAX_EMPTY_TX */,
        dlSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        dlSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
        /* + SPS_TDD_Changes */
        dlSpsNotValidEvent /*MEAS_GAP_RECONFIG */,
        dlSpsNotValidEvent /*DRX_RECONFIG */,
       /* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        dlSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    /* SPR 8244 fix start */
    { /* INVALID_STATE  */
        dlSpsNotValidEvent   /* UE_SYNC_LOSS_ON  */,
        dlSpsNotValidEvent   /* UE_SYNC_LOSS_OFF */,
        dlSpsNotValidEvent   /* SPS_CONFIG_RECVD */,
        dlSpsNotValidEvent   /* FIRST_NON_ZERO_QUEUE_LOAD */,
        dlSpsNotValidEvent   /* VOICE_PACKET */,
        dlSpsNotValidEvent   /* SPS_PUSCH_SUCCESS */,
        dlSpsNotValidEvent   /* SPS_PUSCH_FAILURE */,
        dlSpsNotValidEvent   /* FIRST_ACK_NACK */,
        dlSpsNotValidEvent   /* PDCCH_CCE_ALLOC_FAILURE */,
        dlSpsNotValidEvent   /* HARQ_TIMER_EXPIRED */,
        dlSpsNotValidEvent   /* SILENCE_DETECTED */,
        dlSpsNotValidEvent   /* MCS_CHANGED */,
        dlSpsNotValidEvent   /* BLER_THRESHOLD */,
        dlSpsNotValidEvent   /* PDU_SEG_IND */,
        dlSpsNotValidEvent   /* MAX_DCI_ATTEMPT */,
        dlSpsNotValidEvent   /* MAX_EMPTY_TX */,
        dlSpsNotValidEvent   /* MAX_SPS_HARQ_RETX */,
#ifdef FDD_CONFIG
        dlSpsNotValidEvent  /* TTI_BUNDLING_CONFIG_CHANGED */,
#endif
        /* + SPS_TDD_Changes */
        dlSpsNotValidEvent /*MEAS_GAP_RECONFIG */,
        dlSpsNotValidEvent /*DRX_RECONFIG */,
       /* - SPS_TDD_Changes */
/* SPR 19038 Fix Start */
        dlSpsNotValidEvent  /* MEAS_GAP_COL_WITH_SPS_OCC */,
/* SPR 19038 Fix End */
    },
    /* SPR 8244 fix end */
};

extern UInt8	absPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN]; /* EICIC +-*/
extern UInt8  ueTypeToScheduleDL_g[MAX_NUM_CELL];
extern UInt8 absPatternIndex_g[MAX_NUM_CELL];

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Definitions
 ****************************************************************************/

/*****************************************************************************
 * Function Name  : macDlSpsProcessActQ
 * Input          : count - number of nodes in queue,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function processes the activation queue nodes.
 *****************************************************************************/
UInt16 macDlSpsProcessActQ( UInt32 count, UInt32 delayToPhy ,
        InternalCellIndex internalCellIndex)
{
    LP_MacDlSpsActReq macDlSpsActReq_p  = PNULL;
    LP_SpsDlInfo      spsDlInfo_p       = PNULL;
    DLUEContext       *dlUeCtx_p        = PNULL;
    UInt8             pendingDeleteFlag = 0;

    /*EICIC +*/
    UInt32 sysFrameNum 			= 0;
    UInt32 subFrameNum 			= 0;
    UInt8  absSubFrameNum 		= 0;
    UInt8  isCurrentABSSubFrame = LTE_FALSE;
    UInt8 numBitsAbsPattern = 0;
 
    /* +- SPR 17777 */
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
 
    absSubFrameNum = ((((sysFrameNum )*MAX_SUB_FRAME) +(subFrameNum)) %numBitsAbsPattern) ;
  
    if (LTE_TRUE == absPattern_g[internalCellIndex][absPatternIndex_g[internalCellIndex]][absSubFrameNum]) 
    {
 	    isCurrentABSSubFrame = LTE_TRUE;
    }
   
	/* EICIC -*/

    /*!
     * \code 
     * Dequeue number of nodes in Activation queue, dlSpsActQ_g.
     * Call dlStateMachine for appropriate handling.
     * \endcode
     * */
    while ( count-- )
    {
        /* Dequeue the node from Activation queue */
        DEQUEUE_MAC_DL_SPS_Q( dlSpsActQ_gp[internalCellIndex], macDlSpsActReq_p );

        if( PNULL == macDlSpsActReq_p )
        {
            LOG_MAC_MSG( MAC_SPS_NULL_NODE, LOGWARNING, MAC_DL_SPS_STRATEGY, 
                    getCurrentTick(), __LINE__, count, 0,0,0, 0,0, 
                    __func__, "dlSpsActQ_g");
            continue;
        }
        pendingDeleteFlag =
            dlUECtxInfoArr_g[macDlSpsActReq_p->ueIndex].pendingDeleteFlag;
        /* SPR 6884 changes start */
        dlUeCtx_p = 
            dlUECtxInfoArr_g[macDlSpsActReq_p->ueIndex].dlUEContext_p;
        if( PNULL == dlUeCtx_p )
        {
            LOG_MAC_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, 
                    MAC_DL_Strategy, getCurrentTick(), __LINE__, 
                    macDlSpsActReq_p->ueIndex, 0,0,0, 0,0, __func__, 
                    "dlSpsActQ_g");

            /* Free the memory for Activation Queue node */
            freeMemPool( macDlSpsActReq_p );
            macDlSpsActReq_p = PNULL;
            continue;
        }
		/* EICIC +*/
        if (LTE_TRUE == isCurrentABSSubFrame)
        {
            if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsActQ_gp[internalCellIndex], macDlSpsActReq_p ))
            {
                freeMemPool( macDlSpsActReq_p );
                macDlSpsActReq_p = PNULL;
            }
            continue;
        }/* EICIC -*/
	        
        /* SPR 6884 changes end */
        if ( !pendingDeleteFlag )
        {
            spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

            /*! \code
             * If UL_SYNC_LOSS_ON for the UE
             *   call dlSpsUeSyncLossDetectedinDra.
             *   continue with next node in Act queue.
             *  \endcode
             * */
            if( UL_SYNC_LOSS_ON == checkUlSyncLossForUE( dlUeCtx_p,
                                          getCurrentTick() + delayToPhy 
                                          ,internalCellIndex) )
            {
                /* +- SPR 17777 */
                /* Call state machine for removing UE from SDL,send
                 * Resource Release Request to PDSCH, and change state */
                if ( MAC_SUCCESS == dlStateMachine[spsDlInfo_p->spsState]
                                                  [UE_SYNC_LOSS_ON](
                    macDlSpsActReq_p->ueIndex, spsDlInfo_p->spsState, 
                    UE_SYNC_LOSS_ON, dlUeCtx_p, spsDlInfo_p->sdlOccNode_p,
                    delayToPhy ,internalCellIndex) )
                {
                    /* +- SPR 17777 */
                    LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                              getCurrentTick(), __LINE__, macDlSpsActReq_p->ueIndex,spsDlInfo_p->spsState, UE_SYNC_LOSS_ON,
                              spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                    /* Free the memory for Activation Queue node */
                    freeMemPool( macDlSpsActReq_p );
                    macDlSpsActReq_p = PNULL;
                }
                else
                {
                    LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                              getCurrentTick(), __LINE__, macDlSpsActReq_p->ueIndex,spsDlInfo_p->spsState, UE_SYNC_LOSS_ON,
                              spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                    /* +- SPR 17777 */
                }    
                continue;
            }

            /* Update last scheduled tick */
            dlUeCtx_p->lastScheduledTick = getCurrentTick();
            /* +- SPR 17777 */
            /* Call state machine for appropriate handling */
            if ( MAC_SUCCESS == dlStateMachine[spsDlInfo_p->spsState]
                                              [macDlSpsActReq_p->cause](
                    macDlSpsActReq_p->ueIndex, spsDlInfo_p->spsState, 
                    macDlSpsActReq_p->cause, dlUeCtx_p, macDlSpsActReq_p, 
                    delayToPhy ,internalCellIndex) )
            { 
                /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                          getCurrentTick(), __LINE__, macDlSpsActReq_p->ueIndex,spsDlInfo_p->spsState, macDlSpsActReq_p->cause,
                          spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                /* Free the memory for Activation Queue node */
                freeMemPool( macDlSpsActReq_p );
                macDlSpsActReq_p = PNULL;
            }
            else
            {
                LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                          getCurrentTick(), __LINE__, macDlSpsActReq_p->ueIndex,spsDlInfo_p->spsState, macDlSpsActReq_p->cause,
                          spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                /* +- SPR 17777 */
            }
        }
        else
        {
            LOG_MAC_MSG( MAC_QUEUE_NODE_NOT_PROCESSED, LOGDEBUG, MAC_DL_Strategy,
                    getCurrentTick(), pendingDeleteFlag, 0, 0, 0, 0, 0, 0,
                    __func__, "dlSpsActQ_g");
                
            /* UE has moved to Pending delete so clean up the SPS context */
            dlSpsPendingDeleteHandling( dlUeCtx_p,internalCellIndex );
            
            /* Free the memory for Activation Queue node */
            freeMemPool( macDlSpsActReq_p );
            macDlSpsActReq_p = PNULL;
        }
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : macDlSpsProcessDeactQ
 * Input          : count - number of nodes in queue,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function processes the deactivation queue nodes.
 *****************************************************************************/
UInt16 macDlSpsProcessDeactQ( UInt32 count, UInt32 delayToPhy,
        InternalCellIndex internalCellIndex)
{
    UInt8                pendingDeleteFlag = 0;
    LP_MacDlSpsDeactReq  macDlSpsDectReq_p = PNULL;
    LP_SpsDlInfo         spsDlInfo_p       = PNULL;
    LP_MacSpsOccList     occNode_p         = PNULL;
    DLUEContext          *dlUeCtx_p        = PNULL;
    /* SPR 15909 fix start */
    tickType_t           currTick         =  getCurrentTick();
    /* SPR 15909 fix end */
    /* +- SPR 17777 */
    /*!
     * \code 
     * Dequeue number of nodes in Deactivation queue, dlSpsDeactQ_g.
     * Call dlStateMachine for appropriate handling.
     * \endcode
     * */
    while ( count-- )
    {
        /* Dequeue the node from Deactivation queue */
        DEQUEUE_MAC_DL_SPS_Q( dlSpsDeactQ_gp[internalCellIndex], macDlSpsDectReq_p );
        if( PNULL == macDlSpsDectReq_p )
        {
            LOG_MAC_MSG( MAC_SPS_NULL_NODE, LOGWARNING, MAC_DL_SPS_STRATEGY, 
                    currTick, __LINE__, count, 0,0,0, 0,0, 
                    __func__, "dlSpsDeactQ_g");
            continue;
        }
        pendingDeleteFlag =
            dlUECtxInfoArr_g[macDlSpsDectReq_p->ueIndex].pendingDeleteFlag;
        /* SPR 6884 changes start */
        dlUeCtx_p = dlUECtxInfoArr_g[macDlSpsDectReq_p->ueIndex].dlUEContext_p;
        if( PNULL == dlUeCtx_p )
        {
            LOG_MAC_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, 
                    MAC_DL_Strategy, currTick, __LINE__, 
                    macDlSpsDectReq_p->ueIndex, 0,0,0, 0,0, __func__, 
                    "dlSpsDeactQ_g");

            /* Free the memory for Deactivation Queue node */
            freeMemPool( macDlSpsDectReq_p );
            macDlSpsDectReq_p = PNULL;
            continue;
        }
        /* SPR 6884 changes end */
        if ( !pendingDeleteFlag )
        {
            spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

            /*! \code
             * If UL_SYNC_LOSS_ON for the UE
             *   call dlSpsUeSyncLossDetectedinOccList.
             *   continue with next node in Deact queue.
             *  \endcode
             * */
            if( UL_SYNC_LOSS_ON == checkUlSyncLossForUE( dlUeCtx_p, 
                                                  currTick + delayToPhy 
                                                  ,internalCellIndex) )
            {
                /* +- SPR 17777 */
                /* Call state machine for removing UE from Occ List, send
                 * Resource Release Request to PDSCH, and change state */
                if ( MAC_SUCCESS == dlStateMachine[spsDlInfo_p->spsState]
                                                  [UE_SYNC_LOSS_ON](
                    macDlSpsDectReq_p->ueIndex, spsDlInfo_p->spsState, 
                    UE_SYNC_LOSS_ON, dlUeCtx_p, spsDlInfo_p->sdlOccNode_p, 
                    delayToPhy ,internalCellIndex) )
                {
                    /* +- SPR 17777 */
                    LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                          getCurrentTick(), __LINE__, macDlSpsDectReq_p->ueIndex,spsDlInfo_p->spsState, UE_SYNC_LOSS_ON,
                          spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                    /* Free the memory for Activation Queue node */
                    freeMemPool( macDlSpsDectReq_p );
                    macDlSpsDectReq_p = PNULL;
                }
                else
                {
                    LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                          getCurrentTick(), __LINE__, macDlSpsDectReq_p->ueIndex,spsDlInfo_p->spsState, UE_SYNC_LOSS_ON,
                          spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                    /* +- SPR 17777 */
                }
                continue;
            }

            /* Update last scheduled tick */
            dlUeCtx_p->lastScheduledTick = currTick;
            
            occNode_p = (LP_MacSpsOccList)spsDlInfo_p->sdlOccNode_p;            
            if( PNULL != occNode_p )
            {
                /* +- SPR 17777 */
                /* Call state machine for appropriate handling */
                if( MAC_FAILURE == dlStateMachine[spsDlInfo_p->spsState]
                                                 [macDlSpsDectReq_p->cause](
                        macDlSpsDectReq_p->ueIndex, spsDlInfo_p->spsState,
                        macDlSpsDectReq_p->cause, dlUeCtx_p, occNode_p, 
                        delayToPhy ,internalCellIndex ) )
                {
                    /* +- SPR 17777 */
                    LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                          getCurrentTick(), __LINE__, macDlSpsDectReq_p->ueIndex,spsDlInfo_p->spsState, macDlSpsDectReq_p->cause,
                          spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                    /* +- SPR 17777 */

                    /* Enqueue the node back to deactivation queue to be
                     * processed in next tick */
                    /*Cov_fix_30April_63540_start*/
                    if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsDeactQ_gp[internalCellIndex], macDlSpsDectReq_p ))
                    {
                        freeMemPool( macDlSpsDectReq_p );
                        macDlSpsDectReq_p = PNULL;
                    }
                    /*Cov_fix_30April_63540_stop*/
                    /* Update Q Stats */
                    /* CA Stats Changes Start */
                    LTE_MAC_QUEUE_USED( DL_SPS_DEACT_Q,  
                            QCOUNT_MAC_DL_SPS_Q(dlSpsDeactQ_gp[internalCellIndex]),
                            internalCellIndex);
                    /* CA Stats Changes End */
                }
                else
                {            
                    /* +- SPR 17777 */
                    LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                          getCurrentTick(), __LINE__, macDlSpsDectReq_p->ueIndex,spsDlInfo_p->spsState, macDlSpsDectReq_p->cause,
                          spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                    /* +- SPR 17777 */
                    
                    /* Free the memory for Deactivation Queue node */
                    freeMemPool( macDlSpsDectReq_p );
                    macDlSpsDectReq_p = PNULL;
                }
            }
            else
            {
                LOG_MAC_MSG( MAC_NO_OCC_NODE, LOGWARNING, MAC_DL_SPS_STRATEGY,
                        currTick, __LINE__, macDlSpsDectReq_p->ueIndex,
                        spsDlInfo_p->spsState, macDlSpsDectReq_p->cause, 0, 
                        0,0, __func__, "dlSpsDeactQ_g" );
                    
                /* Free the memory for Deactivation Queue node */
                freeMemPool( macDlSpsDectReq_p );
                macDlSpsDectReq_p = PNULL;
            }
        }
        else
        {
            LOG_MAC_MSG( MAC_QUEUE_NODE_NOT_PROCESSED, LOGDEBUG, MAC_DL_Strategy,
                    currTick, pendingDeleteFlag, 0, 0, 0, 0, 0, 0,
                    __func__, "dlSpsDeactQ_g");
                
            /* UE has moved to Pending delete so clean up the SPS context */
            dlSpsPendingDeleteHandling( dlUeCtx_p ,internalCellIndex);

            /* Free the memory for Deactivation Queue node */
            freeMemPool( macDlSpsDectReq_p );
            macDlSpsDectReq_p = PNULL;
        }
    }
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : macDlSpsProcessStatusQ
 * Input          : count - number of nodes in queue,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function processes the DL status queue nodes.
 *****************************************************************************/
UInt16 macDlSpsProcessStatusQ( UInt32 count, UInt32 delayToPhy,
        InternalCellIndex internalCellIndex)
{
    UInt8             pendingDeleteFlag = 0;
    UInt16            retVal            = MAC_SUCCESS;
    LP_MacDlSpsStatus macDlSpsStatus_p  = PNULL;
    LP_SpsDlInfo      spsDlInfo_p       = PNULL; 
    DLUEContext       *dlUeCtx_p        = PNULL;
    /* +- SPR 17777 */

    /*!
     * \code 
     * Dequeue number of nodes in DL Status queue, dlSpsStatusQ_g.
     * Call dlStateMachine for appropriate handling.
     * \endcode
     * */
    while ( count-- )
    {
        /* Dequeue the node from Status queue */
        DEQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], macDlSpsStatus_p );
                        
        if( PNULL == macDlSpsStatus_p )
        {
            LOG_MAC_MSG( MAC_SPS_NULL_NODE, LOGWARNING, MAC_DL_SPS_STRATEGY, 
                    getCurrentTick(), __LINE__, count, 0,0,0, 0,0, 
                    __func__, "dlSpsStatusQ_g");
            continue;
        }
        
        pendingDeleteFlag =
            dlUECtxInfoArr_g[macDlSpsStatus_p->ueIndex].pendingDeleteFlag;
        /* SPR 6884 changes start */
        dlUeCtx_p = dlUECtxInfoArr_g[macDlSpsStatus_p->ueIndex].dlUEContext_p;
        if( PNULL == dlUeCtx_p )
        {
            LOG_MAC_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, 
                    MAC_DL_Strategy, getCurrentTick(), __LINE__, 
                    macDlSpsStatus_p->ueIndex, 0,0,0, 0,0, __func__, 
                    "dlSpsStatusQ_g");

            /* Free the memory for Status Queue node */
            freeMemPool( macDlSpsStatus_p );
            macDlSpsStatus_p = PNULL;
            continue;
        }
        /* SPR 6884 changes end */
        if ( !pendingDeleteFlag )
        {
            spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
            
             /*! \code
             * If UL_SYNC_LOSS_ON for the UE
             *   Change state to UE_NOT_READY and free resources.
             *   continue with next node in status queue.
             *  \endcode
             * */
            if( UL_SYNC_LOSS_ON == checkUlSyncLossForUE( dlUeCtx_p,
                                        getCurrentTick() + delayToPhy ,internalCellIndex) )
            {
                /* +- SPR 17777 */
                /* Call state machine for removing UE from SDL,send
                 * Resource Release Request to PDSCH, and change state */
                if ( MAC_SUCCESS == dlStateMachine[spsDlInfo_p->spsState]
                                                  [UE_SYNC_LOSS_ON](
                    macDlSpsStatus_p->ueIndex, spsDlInfo_p->spsState, 
                    UE_SYNC_LOSS_ON, dlUeCtx_p, spsDlInfo_p->sdlOccNode_p, 
                    delayToPhy ,internalCellIndex) )
                {
                    /* +- SPR 17777 */
                    LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                          getCurrentTick(), __LINE__, macDlSpsStatus_p->ueIndex,spsDlInfo_p->spsState, UE_SYNC_LOSS_ON,
                          spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                    /* +- SPR 17777 */
                    /* Free the memory for Activation Queue node */
                    freeMemPool( macDlSpsStatus_p );
                    macDlSpsStatus_p = PNULL;
                }
                else
                {
                    /* +- SPR 17777 */
                    LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                          getCurrentTick(), __LINE__, macDlSpsStatus_p->ueIndex, spsDlInfo_p->spsState, UE_SYNC_LOSS_ON,
                          spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                    /* +- SPR 17777 */
                }
                continue;
            }
            
            /* Update last scheduled tick */
            dlUeCtx_p->lastScheduledTick = getCurrentTick();
            /* +- SPR 17777 */
            /* Call state machine for appropriate handling */
            if( MAC_SUCCESS == dlStateMachine[spsDlInfo_p->spsState]
                                             [macDlSpsStatus_p->response](
                    macDlSpsStatus_p->ueIndex, spsDlInfo_p->spsState, 
                    macDlSpsStatus_p->response, dlUeCtx_p, macDlSpsStatus_p, 
                    delayToPhy ,internalCellIndex) )
            {
                    /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, macDlSpsStatus_p->ueIndex,spsDlInfo_p->spsState, macDlSpsStatus_p->response,
                     spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                    /* +- SPR 17777 */
                
                /* Free the memory */
                freeMemPool( macDlSpsStatus_p );
                macDlSpsStatus_p = PNULL;
            }
        }
        else
        {
            LOG_MAC_MSG( MAC_QUEUE_NODE_NOT_PROCESSED, LOGDEBUG, MAC_DL_Strategy,
                    getCurrentTick(), pendingDeleteFlag, 0, 0, 0, 0, 
                    0, 0, __func__, "dlSpsStatusQ_g");
                
            /* UE has moved to Pending delete so clean up the SPS context */
            dlSpsPendingDeleteHandling( dlUeCtx_p ,internalCellIndex);
            
            /* Free the memory for Status Queue node */
            freeMemPool( macDlSpsStatus_p );
            macDlSpsStatus_p = PNULL;
        }
    }    
    return retVal;
}

/*****************************************************************************
 * Function Name  : dlSpsConfigured
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function called when SPS Config is received by RRC.
 *****************************************************************************/
MacRetType dlSpsConfigured( UInt16 ueIndex, SpsStates state, SpsCause cause, 
        DLUEContext *dlUeCtx_p, void * spsCtx_p, UInt32 delayToPhy,
        InternalCellIndex internalCellIndex)
{
    UInt16                  dlSpsInterval          = 0;
    LP_SpsDlInfo            spsDlInfo_p            = PNULL;
    LP_SpsDLIntervalProfile dlSpsIntervalProfile_p = PNULL;
    
    dlSpsInterval = dlUeCtx_p->spsDlInfo.spsDlSetupInfo_p->
                                   semiPersistentSchedIntervalDL;
    dlSpsIntervalProfile_p =
             &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
             dlSchdConfig.spsDlIntervalProfile[dlSpsInterval]) ;

    /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
    /* +- SPR 17777 */

    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    /* Set the state accordingly */
    spsDlInfo_p->spsState = SPS_CONFIGURED;

    /* Set SDL or Occasion node pointer in UE context as NULL */
    spsDlInfo_p->sdlOccNode_p = PNULL;
    /* Initialize TTI index with invalid value in UE context */
    spsDlInfo_p->ttiIndex = MAX_SPS_INTERVAL;
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;

    /* Initialize vars in UE context in UE context */
    spsDlInfo_p->numOfSpsEmptyTx  = 0;
    spsDlInfo_p->headPktSize = 0;
    spsDlInfo_p->avgRlcSduSize = 0;
    spsDlInfo_p->maxPktsAfterDeactivatedForPdschFailure = 0;
    spsDlInfo_p->numDciAttempts = 0;
 
    if( dlUeCtx_p->dlCQIInfo.mcsIndexCodeWordOne >= 
            dlSpsIntervalProfile_p->dlMinMcsSps )
    {

        spsDlInfo_p->isUeDlWbMcsBelowThresh = LTE_FALSE;
    }
    else
    {
        spsDlInfo_p->isUeDlWbMcsBelowThresh = LTE_TRUE;
    }

    /* SPR 19038 Fix Start */
    spsDlInfo_p->isUeMeasGapCollisionWithSpsOcc = LTE_FALSE;
    /* SPR 19038 Fix End */

    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(), 
            cause ,0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : dlSpsResRsvReq
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Configured
 *                  state oe Deactivated state and First Non-Zero Q load/voice
 *                  packet is received.
 *****************************************************************************/
MacRetType dlSpsResRsvReq( UInt16 ueIndex, SpsStates state, SpsCause cause, 
        DLUEContext *dlUeCtx_p, void * spsCtx_p, UInt32 delayToPhy,
        InternalCellIndex internalCellIndex)
{
    LP_SpsDlInfo      spsDlInfo_p      = PNULL;
    LP_MacDlSpsResReq spsDlResReq_p    = PNULL;
    
    LOG_UT( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause, 0, 0,0, 
            __func__, "");
    
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    /*!
     * \code 
     * Change the SPS state to SPS_RES_RSV_REQ_INITIATED.
     * Enqueue node to PDSCH for resource reservation in dlSpsResRsvQ_g.
     * \endcode
     * */
    spsDlResReq_p = (LP_MacDlSpsResReq)getMemFromPool( 
                              sizeof(MacDlSpsResReq), PNULL );
    if( PNULL == spsDlResReq_p )
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacDlSpsResReq),delayToPhy,sizeof(spsCtx_p),cause,
                state,0, __FILE__, __func__);
        /* +- SPR 17777 */
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
	/*KLOCKWORK_CHANGES_START_36*/
	return MAC_FAILURE;
	/*KLOCKWORK_CHANGES_STOP_36*/
    }
    
    /* Fill the structure for PDSCH and enqueue in dlSpsResRsvQ_g */
    spsDlResReq_p->ueIndex = ueIndex;
    spsDlResReq_p->requestType = MAC_SPS_RES_RSV_REQ_ACT;
    spsDlResReq_p->avgRlcSduSize = spsDlInfo_p->avgRlcSduSize;
    
    /* Set the state accordingly */
    spsDlInfo_p->spsState = SPS_RES_RSV_REQ_INITIATED;
    /* Enqueue into dlSpsResRsvQ_g */
    /* + Coverity Id - 32602 */
    if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsResRsvQ_gp[internalCellIndex], spsDlResReq_p  ))
    {
        freeMemPool(spsDlResReq_p);

    }
    /* - Coverity Id - 32602 */
                    
    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( DL_SPS_RES_RSV_REL_Q,  
            QCOUNT_MAC_DL_SPS_Q( dlSpsResRsvQ_gp[internalCellIndex] ), 
            internalCellIndex);

    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(), 
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : dlSpsActivationPdschSuccess
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function called when PDSCH SUCCESS received in
 *                  SPS_RES_RSV_REQ_INITIATED.
 *****************************************************************************/
MacRetType dlSpsActivationPdschSuccess( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,
        InternalCellIndex internalCellIndex)
{
    MacRetType           retVal         = MAC_SUCCESS;
    LP_SpsDlInfo         spsDlInfo_p    = PNULL;
    LP_MacDlSpsStatus    spsDlStatus_p  = (LP_MacDlSpsStatus)spsCtx_p;
    LP_SpsStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsStrIntervalTTIContext  ttiContextPeriodicity20_p  = PNULL;
    AddDelTrigLCNode delTrigLCNode[MAX_LOGICAL_CHANNEL] = {{0}};
    LOG_UT( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause, 0, 0,0, 
            __func__, "");
    
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
    
    /* Get the TTI index in which SPS activation initiated */
    ttiContext_p = &(spsStrTTIMap_g[internalCellIndex][spsDlStatus_p->index]);
    ttiContextPeriodicity20_p = &(spsStrTTIMap_g[internalCellIndex][ (spsDlStatus_p->index + \
                                        SPS_INTERVAL_20) % MAX_SPS_INTERVAL]);

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
        if( ttiContext_p->numDlSpsDciPerTti == 
                (macReconfigSchedulerParamsDL_gp[internalCellIndex] +schParamsIndexDL_g[internalCellIndex])->\
                spsDlSchInfoPerTti.maxDlSpsDciPerTti )
        {
            /* Enqueue back to status queue as this node cannot be processed
             * in this TTI. */
            /* +- SPR 17777 */
            LOG_MAC_MSG( MAC_DL_SDL_TTI_FULL, LOGINFO, MAC_DL_SPS_STRATEGY, 
                    getCurrentTick(), __LINE__, spsDlStatus_p->index, 
                    ttiContext_p->numDlSpsDciPerTti, 
                    (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                    spsDlSchInfoPerTti.maxDlSpsDciPerTti,
                    delayToPhy,cause,state, __func__, "");
            /* +- SPR 17777 */
            /*Cov_fix_5May_63521_start*/
            /* SPR 23483 Changes Start */
            if(CIRC_SUCCESS == ENQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], spsDlStatus_p))
            /* SPR 23483 Changes End */
            {
            /*Cov_fix_5May_63521_stop*/
            /* Update Q Stats */
            /* CA Stats Changes Start */
            LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q,  
                    QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex] ),
                    internalCellIndex);

            /* Update Stats */
            LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
                    ttiContextPeriodicity20_p, getCurrentTick(), 
                    spsDlInfo_p->spsDlSetupInfo_p->\
                    semiPersistentSchedIntervalDL, internalCellIndex);
            /* CA Stats Changes End */
            /*Cov_fix_5May_63521_start*/
            }
            /*Cov_fix_5May_63521_stop*/
            retVal = MAC_FAILURE;
            break;
        }

        /* Check if interval is of 20 ms, then next SPS occasion entry in SDL
         * has reached max value */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[spsDlInfo_p->spsDlSetupInfo_p->\
                semiPersistentSchedIntervalDL] )
        {
            if( ttiContextPeriodicity20_p->numDlSpsDciPerTti ==
                    (macReconfigSchedulerParamsDL_gp[internalCellIndex] +schParamsIndexDL_g[internalCellIndex])->\
                    spsDlSchInfoPerTti.maxDlSpsDciPerTti )
            {
                /* Enqueue back to status queue as this node cannot be 
                 * processed in this TTI. */
                LOG_MAC_MSG( MAC_DL_SDL_TTI_FULL, LOGINFO, MAC_DL_SPS_STRATEGY, 
                        getCurrentTick(), __LINE__, spsDlStatus_p->index, 
                        ttiContext_p->numDlSpsDciPerTti, 
                        (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                        spsDlSchInfoPerTti.maxDlSpsDciPerTti,
                        0, 0, 0, __func__, "");
            /*Cov_fix_5May_63521_start*/
            /* SPR 23483 Changes Start */
             if(CIRC_SUCCESS == ENQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], spsDlStatus_p))
            /* SPR 23483 Changes End */
             {    
            /*Cov_fix_5May_63521_stop*/
                /* Update Q Stats */
                /* CA Stats Changes Start */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q,  
                        QCOUNT_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex]),
                        internalCellIndex);
                /* Update Stats */
                LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
                      ttiContextPeriodicity20_p, getCurrentTick(), 
                      spsDlInfo_p->spsDlSetupInfo_p->\
                              semiPersistentSchedIntervalDL, internalCellIndex);
                /* CA Stats Changes End */
            /*Cov_fix_5May_63521_start*/
             }
            /*Cov_fix_5May_63521_stop*/
                retVal = MAC_FAILURE;
                break;
            }
        }

        spsDlInfo_p->numDciAttempts = 0;

        /* Save the tti index and allocated RBs returned for SDL */
        spsDlInfo_p->ttiIndex = spsDlStatus_p->index;
        spsDlInfo_p->resAllocInfo.allocatedRB = spsDlStatus_p->numRbs;
        spsDlInfo_p->spsOpp = spsDlStatus_p->spsOpp;

        /* Set the state accordingly */
        spsDlInfo_p->spsState = SPS_ACTIVATION_INITIATED;
        /* CA Stats Changes Start */
        /* Put entry in SDL and update the TTI Context Map */
        dlAddUeInSdl( ueIndex, ttiContext_p, ttiContextPeriodicity20_p,
                      spsDlInfo_p, internalCellIndex);
        /* CA Stats Changes End */
        
        /* Generate Delete trigger to QOS strategy */
        if(DL_QOS_BASED_SCHEDULING == 
                cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                dlSchdConfig.dlSchedulerStrategy)
        {
            delTrigLCNode[0].lcId = spsDlInfo_p->lcId;
            delTrigLCNode[0].addDeleteTrigger = DELETE_TRIGGER;
            dlQosPutEntryInZeroNonZeroQueueLoadTriggerQueue(dlUeCtx_p,
                    delTrigLCNode, 1,internalCellIndex);
        }
        /* Subtract SPS LC Q load from UE Q load */
        if( dlUeCtx_p->ueQueueLoad >= 
                dlUeCtx_p->logicalChannel[spsDlInfo_p->lcId].queueLoad )
        {
            dlUeCtx_p->ueQueueLoad -= 
                dlUeCtx_p->logicalChannel[spsDlInfo_p->lcId].queueLoad;
        }
        else
        {
            dlUeCtx_p->ueQueueLoad = 0;
        }
        /* CA Stats Changes Start */
        /* Update Stats */
        LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
                                       cause, spsDlInfo_p->numDciAttempts, 
                                       internalCellIndex);
        /* CA Stats Changes End */
    } while(0);

    
    return retVal;
}

/*****************************************************************************
 * Function Name  : dlSpsActivationPdschFail
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function called when PDSCH FAILURE received in
 *                  SPS_RES_RSV_REQ_INITIATED.
 *****************************************************************************/
MacRetType dlSpsActivationPdschFail( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy, InternalCellIndex internalCellIndex )
{
    LP_SpsDlInfo       spsDlInfo_p    = PNULL;

    /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
    /* +- SPR 17777 */
    /*!
     * \code 
     * Change the SPS state to SPS_DEACTIVATED.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
        
    /* Set the state accordingly */
    spsDlInfo_p->spsState = SPS_DEACTIVATED;
    
    /* Initialize maxPktsAfterDeactivatedForPdschFailure with 
     * MAX_PKTS_RECEIVED_IN_DEACT_DL so that activation queue entry not made
     * again and again */
    spsDlInfo_p->maxPktsAfterDeactivatedForPdschFailure = 
                                    MAX_PKTS_RECEIVED_IN_DEACT_DL ;

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
                                     cause, 0, internalCellIndex);
    /* CA Stats Changes End */
   
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : dlSpsActivated
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Activation
 *                  Initiated state and First ACK/NACK received.
 *****************************************************************************/
MacRetType dlSpsActivated( UInt16 ueIndex, SpsStates state, SpsCause cause, 
        DLUEContext *dlUeCtx_p, void * spsCtx_p, UInt32 delayToPhy,
        InternalCellIndex internalCellIndex)
{
    UInt16                       ttiIndex      = 0;
    MacRetType                   retVal        = MAC_SUCCESS;
    LP_SpsDlInfo                 spsDlInfo_p   = PNULL;
    LP_MacDlSpsStatus            spsDlStatus_p = (LP_MacDlSpsStatus)spsCtx_p;
    LP_SpsStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsStrIntervalTTIContext  ttiContextPeriodicity20_p = PNULL;

    LOG_UT( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause, 0, 0,0, 
            __func__, "");
    /*!
     * \code 
     * Remove entry from SPS DCI List, SDL.
     * Put entry in PDSCH occasion list.
     * Change the SPS state to SPS_ACTIVATED.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    ttiIndex = spsDlInfo_p->ttiIndex;
    
    /* Get the TTI index in which SPS activation initiated */
    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][ttiIndex];
    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][(ttiIndex + SPS_INTERVAL_20)\
                                                     % MAX_SPS_INTERVAL];

    do
    {
        /* Check if max number of DCIs that can be scheduled has reached */
        if( ttiContext_p->numDlSpsOccasionPerTti == 
                (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                spsDlSchInfoPerTti.maxDlSpsOccasionPerTti )
        {
            /* Enqueue back to status queue as this node cannot be processed
             * in this TTI. */
            /* +- SPR 17777 */
            LOG_MAC_MSG( MAC_DL_SDL_OCC_LIST_FULL, LOGINFO, MAC_DL_SPS_STRATEGY, 
                    getCurrentTick(), __LINE__, spsDlStatus_p->index, 
                    ttiContext_p->numDlSpsOccasionPerTti, 
                    (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                    spsDlSchInfoPerTti.maxDlSpsOccasionPerTti,delayToPhy,
                    cause,state, __func__, "");
            /* +- SPR 17777 */
            /*Cov_fix_5May_63520_start*/ 
            /* SPR 23483 Changes Start */
            if(CIRC_SUCCESS == ENQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], spsDlStatus_p))
            /* SPR 23483 Changes End */
            {
            /*Cov_fix_5May_63520_stop*/
            /* Update Q Stats */
            /* CA Stats Changes Start */
            LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q,  
                    QCOUNT_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex]), 
                    internalCellIndex);
            /* Update Stats */
            LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
                    ttiContextPeriodicity20_p, getCurrentTick(), 
                    spsDlInfo_p->spsDlSetupInfo_p->\
                    semiPersistentSchedIntervalDL, 
                    internalCellIndex); 
            /* CA Stats Changes End */
            /*Cov_fix_5May_63520_start*/ 
            }
            /*Cov_fix_5May_63520_stop*/
            retVal = MAC_FAILURE;
            break;
        }

        /* Check if interval is of 20 ms, then next SPS occasion entry in SDL
         * has reached max value */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[spsDlInfo_p->spsDlSetupInfo_p->\
                                               semiPersistentSchedIntervalDL] )
        {
            if( ttiContextPeriodicity20_p->numDlSpsOccasionPerTti == 
                    (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                    spsDlSchInfoPerTti.maxDlSpsOccasionPerTti )
            {
                /* Enqueue back to status queue as this node cannot be 
                 * processed in this TTI. */
                LOG_MAC_MSG( MAC_DL_SDL_TTI_FULL, LOGINFO, MAC_DL_SPS_STRATEGY, 
                        getCurrentTick(), __LINE__, spsDlStatus_p->index, 
                        ttiContext_p->numDlSpsOccasionPerTti, 
                        (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                        spsDlSchInfoPerTti.maxDlSpsOccasionPerTti,
                        0, 0, 0, __func__, "");
            /*Cov_fix_5May_63520_start*/
            /* SPR 23483 Changes Start */
             if(CIRC_SUCCESS == ENQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], spsDlStatus_p))
            /* SPR 23483 Changes End */
             {
            /*Cov_fix_5May_63520_stop*/
                /* Update Q Stats */
                /* CA Stats Changes Start */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q,  
                        QCOUNT_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex]), 
                        internalCellIndex);
                /* Update Stats */
                LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
                        ttiContextPeriodicity20_p, getCurrentTick(), 
                        spsDlInfo_p->spsDlSetupInfo_p->\
                        semiPersistentSchedIntervalDL, internalCellIndex); 
                /* CA Stats Changes End */
            /*Cov_fix_5May_63520_start*/
             }
            /*Cov_fix_5May_63520_stop*/
                retVal = MAC_FAILURE;
                break;
            }
        }
    
        /* CA Stats Changes Start */
        /* Delete SDL node from SDL and update TTI Context*/
        /* +- SPR 17777 */
        dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p,
                spsDlInfo_p
#ifdef SSI_DEBUG_STATS
                ,internalCellIndex
#endif
                );
        /* +- SPR 17777 */
        /* CA Stats Changes End */
        spsDlInfo_p->ttiIndex = ttiIndex;
        

        /* CA Stats Changes Start */
        /* Put entry in occasion list and update TTI context */
        dlAddUeInOccasionList( ueIndex, ttiContext_p, 
                ttiContextPeriodicity20_p, spsDlInfo_p, internalCellIndex);
        /* CA Stats Changes End */
        
        /* Set the state accordingly */
        spsDlInfo_p->spsState = SPS_ACTIVATED;

    } while( 0 );

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(), 
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */
    return retVal;
}

/********************************************************************************
 * Function Name  : dlSpsMaxDciAttemptReached
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Activation
 *                  Initiated state and DCI Attempts have reached Maximum value.
 ********************************************************************************/
MacRetType dlSpsMaxDciAttemptReached( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy,InternalCellIndex internalCellIndex )
{
    UInt8                hpId           = 0;
    LP_SpsDlInfo         spsDlInfo_p    = PNULL;
    LP_MacDlSpsResReq    spsResRelReq_p = PNULL;
    DLHARQProcess        *harqProcess_p = PNULL;
    LP_SpsStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsStrIntervalTTIContext  ttiContextPeriodicity20_p = PNULL;

    LOG_UT( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause, 0, 0,0, 
            __func__, "");

    /*KLOCKWORK_CHANGES_START_47 */
    /* Allocate memory for Resource Release Request */
    spsResRelReq_p = (LP_MacDlSpsResReq)getMemFromPool( 
            sizeof(MacDlSpsResReq), PNULL );
    if( PNULL == spsResRelReq_p )
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacDlSpsResReq),delayToPhy,sizeof(spsCtx_p),cause,
                state,0, __FILE__, __func__);
        /* +- SPR 17777 */
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
        return MAC_FAILURE;
    }
    /*KLOCKWORK_CHANGES_START_47 */
    /*!
     * \code 
     * Delete entry from SDL and decrement numDlSpsDciPerTti.
     * Enqueue node to PDSCH for resource release in dlSpsResRelQ_g.
     * Change the SPS state to SPS_DEACTIVATED.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
    
    /* Get the TTI context for both intervals */    
    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->ttiIndex];
    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][ (spsDlInfo_p->ttiIndex +
                                        SPS_INTERVAL_20) % MAX_SPS_INTERVAL];

    /*! \code
     * Free HARQ process ID = 0 reserved for SPS Activation/Deactivation
     * as per 36.213 (Table 9.2-1) as SPS cannot be activated and
     * MAX_DCI_ATTEMPT has reached.
     * \endcode
     * */
    hpId = spsDlInfo_p->hpId;
    harqProcess_p = &dlUeCtx_p->dlHarqContext_p->harqProcess[hpId];

    if( HARQ_PROCESS_FREE != harqProcess_p->harqStatus)
    {
        /* free the HARQ process for SPS NEW_TX */
        freeDLHarqProcessTBSpecific( dlUeCtx_p, harqProcess_p, TB_ONE, internalCellIndex);
    }

    /* CA Stats Changes Start */
    /* Delete SDL node from SDL and update TTI Context*/
    /* +- SPR 17777 */
    dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p,
                spsDlInfo_p
#ifdef SSI_DEBUG_STATS
                ,internalCellIndex
#endif
                );
    /* +- SPR 17777 */
    /* CA Stats Changes End */

    /* Fill the structure for PDSCH for release request */
    spsResRelReq_p->ueIndex = ueIndex;
    spsResRelReq_p->requestType = MAC_SPS_RES_REL_REQ;

    /* Enqueue into dlSpsResRelQ_g */
    /* + Coverity Id - 32601 */
    if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex], spsResRelReq_p ))
    {
        freeMemPool(spsResRelReq_p);
    }    
    /* - Coverity Id - 32601 */

    /* Update Q Stats */
    LTE_MAC_QUEUE_USED( DL_SPS_RES_RSV_REL_Q,
            QCOUNT_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex] ),
            internalCellIndex);
    /* Set the state accordingly */
    spsDlInfo_p->spsState = SPS_DEACTIVATED;


    return MAC_SUCCESS;
}

/********************************************************************************
 * Function Name  : dlSpsMaxDciAttemptReachedForDeact
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Deactivation
 *                  Initiated state and DCI Attempts have reached Maximum value.
 ********************************************************************************/
MacRetType dlSpsMaxDciAttemptReachedForDeact( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p,
        UInt32 delayToPhy,InternalCellIndex internalCellIndex )
{
    UInt8                hpId           = 0;
    LP_SpsDlInfo         spsDlInfo_p    = PNULL;
    DLHARQProcess        *harqProcess_p = PNULL;
    LP_SpsStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsStrIntervalTTIContext  ttiContextPeriodicity20_p = PNULL;

    /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
    /* +- SPR 17777 */
    /*!
     * \code 
     * Delete entry from SDL and decrement numDlSpsDciPerTti.
     * Change the SPS state to SPS_DEACTIVATED.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
        
    /* Get the TTI context for both intervals */    
    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->deactTtiIndex];
    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][ (spsDlInfo_p->deactTtiIndex +
                                        SPS_INTERVAL_20) % MAX_SPS_INTERVAL];

    /*! \code
     * Free HARQ process ID = 0 reserved for SPS Activation/Deactivation
     * as per 36.213 (Table 9.2-1) as SPS cannot be activated and
     * MAX_DCI_ATTEMPT has reached.
     * \endcode
     * */
    hpId = spsDlInfo_p->hpId;
    harqProcess_p = &dlUeCtx_p->dlHarqContext_p->harqProcess[hpId];

    if( HARQ_PROCESS_FREE != harqProcess_p->harqStatus)
    {
        /* free the HARQ process for SPS NEW_TX */
        freeDLHarqProcessTBSpecific( dlUeCtx_p, harqProcess_p, TB_ONE, internalCellIndex);
    }

    /* CA Stats Changes Start */
    /* Delete SDL node from SDL and update TTI Context*/
    /* +- SPR 17777 */
    dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p,
                spsDlInfo_p
#ifdef SSI_DEBUG_STATS
                ,internalCellIndex
#endif
                );
    /* +- SPR 17777 */
    /* CA Stats Changes End */
   
    /* Store Deactivation tti Index for SDL deletion */
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;

    /* Set the state accordingly */
    spsDlInfo_p->spsState = SPS_DEACTIVATED;

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(), 
                                     cause, 0, internalCellIndex);
    /* CA Stats Changes End */
    
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : dlSpsDeactInit
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function  takes care for use cases when in Activation
 *                  state and number of Empty Transmissions reached max value.
 *****************************************************************************/
MacRetType dlSpsDeactInit( UInt16 ueIndex, SpsStates state, SpsCause cause, 
   DLUEContext *dlUeCtx_p, void * spsCtx_p, UInt32 delayToPhy,
   InternalCellIndex internalCellIndex )
{
    UInt16              ttiIndex    = 0;
    MacRetType          retVal      = MAC_SUCCESS;
    LP_SpsDlInfo        dlSpsInfo_p = PNULL;
    LP_MacDlSpsResReq   spsResRelReq_p = PNULL;
    LP_SpsStrIntervalTTIContext  ttiContext_p  = PNULL;
    LP_SpsStrIntervalTTIContext  ttiContextPeriodicity20_p  = PNULL;    
    AddDelTrigLCNode addTrigLCNode[MAX_LOGICAL_CHANNEL] = {{0}};
    
    LOG_UT( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause, 0, 0,0, 
            __func__, "");
    /*! \code
     * Remove Entry from the Occasion List.
     * Decrement numDlSpsOccasionPerTti value.
     * Send Resource Release Request to PDSCH in dlSpsResRelQ_g.
     * Put the entry in SDL.
     * Increment numDlSpsDciPerTti.
     * Change the State to SPS_DEACTIVATION_INITIATED.
     * \endcode
     * */       
    dlSpsInfo_p = &dlUeCtx_p->spsDlInfo;

    ttiIndex = (getCurrentTick() + delayToPhy) % MAX_SPS_INTERVAL;

    /* Get the TTI index in which SPS activation initiated */
    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][ttiIndex];
    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][(ttiIndex + SPS_INTERVAL_20) \
                                           % MAX_SPS_INTERVAL];

    do
    {
        /* Check if UE is not in DRX */
        /* + SPS_TDD_Changes */
        if ( ( DRX_STATE_ON == dlSpsInfo_p->ueDrxTTI[ttiIndex] ) ||
                (MEAS_GAP_ON == dlSpsInfo_p->ueMeasGapTTI[ttiIndex]))
        /* - SPS_TDD_Changes */
        {
            /* +- SPR 17777 */
            LOG_MAC_MSG( MAC_DL_UE_IN_DRX_MEAS_GAP, LOGWARNING, MAC_DL_SPS_STRATEGY,
                    /* + Coverity 88595 */
                    getCurrentTick(), __LINE__, ueIndex, dlSpsInfo_p->ueDrxTTI[ttiIndex],
                    dlSpsInfo_p->ueMeasGapTTI[ttiIndex],sizeof(spsCtx_p),cause,state, 
                    /* + Coverity 88595 */
                    __func__, "DEACTIVATION" );
            /* +- SPR 17777 */
            retVal = MAC_FAILURE;
            break;
        }
        /* Check if max number of DCIs that can be scheduled has reached */
        if( ttiContext_p->numDlSpsDciPerTti == 
                (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                spsDlSchInfoPerTti.maxDlSpsDciPerTti )
        {
            /* Enqueue back to status queue as this node cannot be processed
             * in this TTI. */
            LOG_MAC_MSG( MAC_DL_SDL_TTI_FULL, LOGINFO, MAC_DL_SPS_STRATEGY, 
                    getCurrentTick(), __LINE__, ttiIndex, 
                    ttiContext_p->numDlSpsDciPerTti, 
                    (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                    spsDlSchInfoPerTti.maxDlSpsDciPerTti, 0, 
                    0, 0, __func__, "");
            /* CA Stats Changes Start */
            /* Update Stats */
            LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
                    ttiContextPeriodicity20_p, getCurrentTick(), 
                    dlSpsInfo_p->spsDlSetupInfo_p->\
                    semiPersistentSchedIntervalDL, internalCellIndex); 
            /* CA Stats Changes End */
            retVal = MAC_FAILURE;
            break;
        }

        /* Check if interval is of 20 ms, then next SPS occasion entry in SDL
         * has reached max value */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[dlSpsInfo_p->spsDlSetupInfo_p->\
                                                semiPersistentSchedIntervalDL] )
        {
            if( ttiContextPeriodicity20_p->numDlSpsDciPerTti == 
                    (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                    spsDlSchInfoPerTti.maxDlSpsDciPerTti )
            {
                /* Enqueue back to status queue as this node cannot be 
                 * processed in this TTI. */
                LOG_MAC_MSG( MAC_DL_SDL_TTI_FULL, LOGINFO, MAC_DL_SPS_STRATEGY, 
                        getCurrentTick(), __LINE__, ttiIndex, 
                        ttiContext_p->numDlSpsDciPerTti, 
                        (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                        spsDlSchInfoPerTti.maxDlSpsDciPerTti, 0, 
                        0, 0, __func__, "");            
            
                /* CA Stats Changes Start */
                /* Update Stats */
                LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
                        ttiContextPeriodicity20_p, getCurrentTick(), 
                        dlSpsInfo_p->spsDlSetupInfo_p->\
                        semiPersistentSchedIntervalDL, internalCellIndex); 
                /* CA Stats Changes End */
                retVal = MAC_FAILURE;
                break;
            }
        }
        /*KLOCKWORK_CHANGES_START_46*/
        /* Allocate memory for Resource Release Request */
        spsResRelReq_p = (LP_MacDlSpsResReq)getMemFromPool(              
                                      sizeof(MacDlSpsResReq), PNULL );
        if( PNULL == spsResRelReq_p )
        {
            LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                    getCurrentTick(), __LINE__, sizeof(MacDlSpsResReq), 0,0,0,
                    0,0, __FILE__, __func__);
            ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            ltePanic("getMemFromPool fails %s", __func__);
            retVal = MAC_FAILURE;
            break;
        }
        /*KLOCKWORK_CHANGES_STOP_46*/

        dlSpsInfo_p->numDciAttempts = 0;
        
        /* Get the TTI index in which SPS activation initiated */
        ttiContext_p = &spsStrTTIMap_g[internalCellIndex][dlSpsInfo_p->ttiIndex];
        ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
                 (dlSpsInfo_p->ttiIndex + SPS_INTERVAL_20) % 
                 MAX_SPS_INTERVAL];

        /* Delete the node from occasion list */
        /* +- SPR 17777 */
        dlDeleteUeFromOccasionList( ttiContext_p, ttiContextPeriodicity20_p, 
                dlSpsInfo_p
#ifdef SSI_DEBUG_STATS
                ,internalCellIndex
#endif
                );
        /* +- SPR 17777 */
    
        /* Fill the structure for PDSCH for release request */
        spsResRelReq_p->ueIndex = ueIndex;
        spsResRelReq_p->requestType = MAC_SPS_RES_REL_REQ;
    
        /* Enqueue into dlSpsRsvRelQ_g */
        /* + Coverity Id - 32600 */
        if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex], spsResRelReq_p ))
        {
            freeMemPool(spsResRelReq_p);
        
        }
        /* - Coverity Id - 32600 */
    
        /* Update Q Stats */
        /* CA Stats Changes Start */
        LTE_MAC_QUEUE_USED( DL_SPS_RES_RSV_REL_Q,  
                QCOUNT_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex] ),
                internalCellIndex);
        /* CA Stats Changes End */

        /* Store Deactivation tti Index */
        dlSpsInfo_p->deactTtiIndex = ttiIndex;
        
        /* Get the TTI index in which SPS activation initiated */
        ttiContext_p = &spsStrTTIMap_g[internalCellIndex][ttiIndex];
        ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
                  (ttiIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL];

        /* CA Stats Changes Start */
        /* Put entry in SDL and update the TTI Context Map */
        dlAddUeInSdl( ueIndex, ttiContext_p, ttiContextPeriodicity20_p,
                dlSpsInfo_p, internalCellIndex);
        /* CA Stats Changes End */
        
        /* Reset isUeDlWbMcsBelowThresh */
        dlSpsInfo_p->isUeDlWbMcsBelowThresh = LTE_FALSE;
 
        /* SPR 19038 Fix Start */
        dlSpsInfo_p->isUeMeasGapCollisionWithSpsOcc = LTE_FALSE;
        /* SPR 19038 Fix End */

        /* Change state to SPS_DEACTIVATION_INITIATED */
        dlSpsInfo_p->spsState = SPS_DEACTIVATION_INITIATED;

        /* Generate Add trigger to QOS strategy */
        if(DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->initParams_p->dlSchdConfig.\
            dlSchedulerStrategy)
        {
            if (0 != dlUeCtx_p->logicalChannel[dlSpsInfo_p->lcId].queueLoad)
            {
                addTrigLCNode[0].lcId = dlSpsInfo_p->lcId;
                addTrigLCNode[0].addDeleteTrigger = ADD_TRIGGER;
                dlQosPutEntryInZeroNonZeroQueueLoadTriggerQueue(dlUeCtx_p,
                                                          addTrigLCNode, 1, internalCellIndex);
            }
        }
        /* Add SPS LC Q load to UE Q load */
        /* + SPS_TDD_Changes */
        /*dlUeCtx_p->ueQueueLoad += 
                dlUeCtx_p->logicalChannel[dlSpsInfo_p->lcId].queueLoad;*/
        /* - SPS_TDD_Changes */
        /* CA Stats Changes Start */
        /* Update Stats */
        LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
                cause, dlSpsInfo_p->numDciAttempts, internalCellIndex);
        /* CA Stats Changes End */
    } while (0);

    return retVal;
}

/*******************************************************************************
 * Function Name  : dlSpsMaxHarqRetx
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Deactivation
 *                  initiated state and max_sps_harq_reached.
 *******************************************************************************/
MacRetType dlSpsMaxHarqRetx( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex )
{
    UInt8               maxDlSpsDciPerTti = 0;
    UInt8               numDciAttempts    = 0;
    /*coverity-530 CID-32335 */
    MacRetType          retVal            = MAC_SUCCESS;
    /*coverity-530 CID-32335 */
    UInt16              ttiIndex          = 0;
    LP_SpsDlInfo        dlSpsInfo_p       = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;    
    LP_MacDlSpsStatus   spsDlStatus_p = (LP_MacDlSpsStatus)spsCtx_p;
    
    LOG_UT( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause, 0, 0,0, 
            __func__, "");
    /*! \code
     * if MaxHarqRetx for all unsure cases, except ACK/NACK.
     *    if( numDciAttempts >= MAX_DCI_ATTEMPTS )
     *       then Delete the node from SDL 
     *            change state to SPS_DEACTIVATED. 
     *    else
     *       Retry SPS Deactivation at next available tick. 
     *       State is unchanged.
     * else
     * In case of deactivation, HARQ should not send any other cause.
     * \endcode
     * */
    dlSpsInfo_p = &dlUeCtx_p->spsDlInfo;
   
    do
    {
        numDciAttempts = dlSpsInfo_p->numDciAttempts;        
        if( numDciAttempts >= cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->initParams_p->spsSysWideParam.maxSpsDciAttempt )
        {
            /* Get the TTI index in which SPS activation initiated */
            ttiContext_p = &spsStrTTIMap_g[internalCellIndex][dlSpsInfo_p->deactTtiIndex];
            ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
                     (dlSpsInfo_p->deactTtiIndex + SPS_INTERVAL_20) % 
                     MAX_SPS_INTERVAL];
            /* CA Stats Changes Start */
            /* Delete the node from SDL and update the TTI Context */
            /* +- SPR 17777 */
            dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p, 
                dlSpsInfo_p
#ifdef SSI_DEBUG_STATS
                ,internalCellIndex
#endif
                );
            /* +- SPR 17777 */
            /* CA Stats Changes End */

            /* Set the state accordingly */
            dlSpsInfo_p->spsState = SPS_DEACTIVATED;

            /* Reset Deact tti Index */
            dlSpsInfo_p->deactTtiIndex = MAX_SPS_INTERVAL; 

            /* CA Stats Changes Start */
            /* Update Stats */
            LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                    getCurrentTick(), cause, 0, internalCellIndex);
            break;
        }
        ttiIndex = (getCurrentTick() + delayToPhy) % MAX_SPS_INTERVAL;

        /* Since current ttiIndex is same as ttiIndex stored in UE conext,
         * so break the loop for it to process in SDL processing. */
        if( ttiIndex == dlSpsInfo_p->deactTtiIndex )
        {
            /* Update Stats */                
            LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                    getCurrentTick(), cause, dlSpsInfo_p->numDciAttempts, 
                    internalCellIndex);
            break;
        }

        /* Check if interval is of 20 ms, at next periodic interval as well */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[dlSpsInfo_p->
                spsDlSetupInfo_p->semiPersistentSchedIntervalDL] )
        {
            if( ttiIndex == ( dlSpsInfo_p->deactTtiIndex + SPS_INTERVAL_20) %
                    MAX_SPS_INTERVAL )
            {
                /* Update Stats */                
                LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                        getCurrentTick(), cause, dlSpsInfo_p->numDciAttempts,
                        internalCellIndex);
                /* CA Stats Changes End */
                break;
            }
        }
        /* Get the TTI index in which SPS Deactivation to be initiated */
        ttiContext_p = &spsStrTTIMap_g[internalCellIndex][ttiIndex];
        ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
                 (ttiIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL];

        maxDlSpsDciPerTti = (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                            spsDlSchInfoPerTti.maxDlSpsDciPerTti;
        /* Check if max number of DCIs that can be scheduled has reached */
        if( ttiContext_p->numDlSpsDciPerTti == maxDlSpsDciPerTti )
        {
            /* Enqueue back to status queue as this node cannot be 
             * processed in this TTI. */
            /* +- SPR 17777 */
            LOG_MAC_MSG( MAC_DL_SDL_TTI_FULL, LOGINFO, MAC_DL_SPS_STRATEGY, 
                    getCurrentTick(), __LINE__, ttiIndex, 
                    ttiContext_p->numDlSpsDciPerTti, maxDlSpsDciPerTti,cause,
                    state, 0, __func__, "");
            /* +- SPR 17777 */
           /*Cov_fix_5May_63523_start*/
            /* SPR 23483 Changes Start */
           if(CIRC_SUCCESS == ENQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], spsDlStatus_p))
            /* SPR 23483 Changes End */
           {
           /*Cov_fix_5May_63523_stop*/
            /* Update Q Stats */
            /* CA Stats Changes Start */
            LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                    QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex] ), 
                    internalCellIndex);
            /* Update Stats */                
            LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                    getCurrentTick(), cause, dlSpsInfo_p->numDciAttempts, 
                    internalCellIndex);
            LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
                    ttiContextPeriodicity20_p, getCurrentTick(), 
                    dlSpsInfo_p->spsDlSetupInfo_p->\
                    semiPersistentSchedIntervalDL, internalCellIndex); 
            /* CA Stats Changes End */
           /*Cov_fix_5May_63523_start*/
           }
           /*Cov_fix_5May_63523_stop*/
            retVal = MAC_FAILURE;
            break;
        }

        /* Check if interval is of 20 ms, then next SPS occasion entry in SDL
         * has reached max value */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[dlSpsInfo_p->
                spsDlSetupInfo_p->semiPersistentSchedIntervalDL] )
        {
            if( ttiContextPeriodicity20_p->numDlSpsDciPerTti == 
                                                maxDlSpsDciPerTti )
            {
                /* Enqueue back to status queue as this node cannot be 
                 * processed in this TTI. */
                LOG_MAC_MSG( MAC_DL_SDL_TTI_FULL, LOGINFO, MAC_DL_SPS_STRATEGY, 
                        getCurrentTick(), __LINE__, 
                        (ttiIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL, 
                        ttiContextPeriodicity20_p->numDlSpsDciPerTti, 
                        maxDlSpsDciPerTti, 0, 0, 0, __func__, "");            
              /*Cov_fix_5May_63523_start*/
            /* SPR 23483 Changes Start */
               if(CIRC_SUCCESS == ENQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], spsDlStatus_p))
            /* SPR 23483 Changes End */
               {
              /*Cov_fix_5May_63523_stop*/
                /* Update Q Stats */
                /* CA Stats Changes Start */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q,  
                        QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex] ), 
                        internalCellIndex);
                /* Update Stats */        
                LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                        getCurrentTick(), cause, dlSpsInfo_p->numDciAttempts, 
                        internalCellIndex);
                LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
                        ttiContextPeriodicity20_p, getCurrentTick(), 
                        dlSpsInfo_p->spsDlSetupInfo_p->\
                        semiPersistentSchedIntervalDL, internalCellIndex); 
                /* CA Stats Changes End */
              /*Cov_fix_5May_63523_start*/
               }
              /*Cov_fix_5May_63523_stop*/
                retVal = MAC_FAILURE;
                break;
            }
        }

        /* Get the TTI index in which SPS activation initiated */
        ttiContext_p = &spsStrTTIMap_g[internalCellIndex][dlSpsInfo_p->deactTtiIndex];
        ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
                     (dlSpsInfo_p->deactTtiIndex + SPS_INTERVAL_20) % 
                     MAX_SPS_INTERVAL];

        /* CA Stats Changes Start */
        /* Delete the node from SDL and update the TTI Context */
            /* +- SPR 17777 */
        dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p,
                dlSpsInfo_p
#ifdef SSI_DEBUG_STATS
                ,internalCellIndex
#endif
                );
            /* +- SPR 17777 */
        /* CA Stats Changes End */
        
        /* Store Deactivation tti Index for SDL deletion */
        dlSpsInfo_p->deactTtiIndex = ttiIndex;

        /* Get the TTI index in which SPS Deactivation to be initiated */
        ttiContext_p = &spsStrTTIMap_g[internalCellIndex][ttiIndex];
        ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
                 (ttiIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL];

        dlSpsInfo_p->numDciAttempts = numDciAttempts;

        /* CA Stats Changes Start */
        /* Put entry in SDL and update the TTI Context Map */
        dlAddUeInSdl( ueIndex, ttiContext_p, ttiContextPeriodicity20_p,
                dlSpsInfo_p, internalCellIndex);

        /* Update Stats */
        LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                getCurrentTick(), cause, dlSpsInfo_p->numDciAttempts, 
                internalCellIndex);
        /* CA Stats Changes End */
    } while( 0 );

    return retVal;
}

/*********************************************************************************
 * Function Name  : dlSpsDeactPdcchFail
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Deactivation
 *                  initiated state and CCE Alloc failure is returned from PDCCH.
 *********************************************************************************/
MacRetType dlSpsDeactPdcchFail( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy,InternalCellIndex internalCellIndex )
{
    UInt8               maxDlSpsDciPerTti = 0;
    UInt8               numDciAttempts    = 0;
    /*coverity-530 CID-32335 */
    MacRetType          retVal            = MAC_SUCCESS;
    /*coverity-530 CID-32335 */
    UInt16              ttiIndex          = 0;
    LP_SpsDlInfo        dlSpsInfo_p       = PNULL;
    LP_MacSpsDciListTTI sdlNode_p         = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;    
    LP_MacDlSpsStatus   spsDlStatus_p = (LP_MacDlSpsStatus)spsCtx_p;
    
    LOG_UT( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause, 0, 0,0, 
            __func__, "");
    /*! \code
     * if CCE Allocation fail is returned from PDCCH.
     *    - Retry at next available TTI.
     * \endcode
     * */
    dlSpsInfo_p = &dlUeCtx_p->spsDlInfo;
   
    sdlNode_p = (LP_MacSpsDciListTTI)dlSpsInfo_p->sdlOccNode_p;

    do
    {
        if( PNULL == sdlNode_p )
        {
            break;
        }

        ttiIndex = (getCurrentTick() + delayToPhy) % MAX_SPS_INTERVAL;
        numDciAttempts = dlSpsInfo_p->numDciAttempts;        

        /* Since current ttiIndex is same as ttiIndex stored in UE conext,
         * so break the loop for it to process in SDL processing. */
        if( ttiIndex == dlSpsInfo_p->ttiIndex )
        {
            /* CA Stats Changes Start */
            /* Update Stats */                
            LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                    getCurrentTick(), cause, dlSpsInfo_p->numDciAttempts, 
                    internalCellIndex);
            break;
        }

        /* Check if interval is of 20 ms, at next periodic interval as well */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[dlSpsInfo_p->
                spsDlSetupInfo_p->semiPersistentSchedIntervalDL] )
        {
            if( ttiIndex == ( dlSpsInfo_p->ttiIndex + SPS_INTERVAL_20) % 
                    MAX_SPS_INTERVAL )
            {
                /* Update Stats */                
                LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                        getCurrentTick(), cause, dlSpsInfo_p->numDciAttempts, 
                        internalCellIndex);
                /* CA Stats Changes End */
                break;
            }
        }
        /* Get the TTI index in which SPS Deactivation to be initiated */
        ttiContext_p = &spsStrTTIMap_g[internalCellIndex][ttiIndex];
        ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
                 (ttiIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL];

        maxDlSpsDciPerTti = (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                            spsDlSchInfoPerTti.maxDlSpsDciPerTti;
        /* Check if max number of DCIs that can be scheduled has reached */
        if( ttiContext_p->numDlSpsDciPerTti == maxDlSpsDciPerTti )
        {
            /* Enqueue back to status queue as this node cannot be 
             * processed in this TTI. */
            /* +- SPR 17777 */
            LOG_MAC_MSG( MAC_DL_SDL_TTI_FULL, LOGINFO, MAC_DL_SPS_STRATEGY, 
                    getCurrentTick(), __LINE__, ttiIndex, 
                    ttiContext_p->numDlSpsDciPerTti, maxDlSpsDciPerTti,cause,
                    state, 0, __func__, "");
            /* +- SPR 17777 */
           /*Cov_fix_5May_63522_start*/
            /* SPR 23483 Changes Start */
           if(CIRC_SUCCESS == ENQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], spsDlStatus_p))
            /* SPR 23483 Changes End */
           {
           /*Cov_fix_5May_63522_stop*/
            /* Update Q Stats */
            /* CA Stats Changes Start */
            LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                    QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex] ), 
                    internalCellIndex);
            /* Update Stats */                
            LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                    getCurrentTick(), cause, dlSpsInfo_p->numDciAttempts, 
                    internalCellIndex);
            LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
                    ttiContextPeriodicity20_p, getCurrentTick(), 
                    dlSpsInfo_p->spsDlSetupInfo_p->\
                    semiPersistentSchedIntervalDL, internalCellIndex); 
            /* CA Stats Changes End */
           /*Cov_fix_5May_63522_start*/
           }
           /*Cov_fix_5May_63522_stop*/
            retVal = MAC_FAILURE;
            break;
        }

        /* Check if interval is of 20 ms, then next SPS occasion entry in SDL
         * has reached max value */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[dlSpsInfo_p->
                spsDlSetupInfo_p->semiPersistentSchedIntervalDL] )
        {
            if( ttiContextPeriodicity20_p->numDlSpsDciPerTti == 
                                                maxDlSpsDciPerTti )
            {
                /* Enqueue back to status queue as this node cannot be 
                 * processed in this TTI. */
                LOG_MAC_MSG( MAC_DL_SDL_TTI_FULL, LOGINFO, MAC_DL_SPS_STRATEGY, 
                        getCurrentTick(), __LINE__, 
                        (ttiIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL, 
                        ttiContextPeriodicity20_p->numDlSpsDciPerTti, 
                        maxDlSpsDciPerTti, 0, 0, 0, __func__, "");            
           /*Cov_fix_5May_63522_start*/
            /* SPR 23483 Changes Start */
              if(CIRC_SUCCESS ==  ENQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], spsDlStatus_p))
            /* SPR 23483 Changes End */
              {
           /*Cov_fix_5May_63522_stop*/
                /* Update Q Stats */
                /* CA Stats Changes Start */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q,  
                        QCOUNT_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex]),
                        internalCellIndex);            
                /* Update Stats */        
                LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                        getCurrentTick(), cause, dlSpsInfo_p->numDciAttempts, 
                        internalCellIndex);
                LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
                        ttiContextPeriodicity20_p, getCurrentTick(), 
                        dlSpsInfo_p->spsDlSetupInfo_p->\
                        semiPersistentSchedIntervalDL, internalCellIndex); 
                /* CA Stats Changes End */
           /*Cov_fix_5May_63522_start*/
              }
           /*Cov_fix_5May_63522_stop*/
                retVal = MAC_FAILURE;
                break;
            }
        }

        /* Get the TTI index in which SPS Deactivation to be initiated */
        ttiContext_p = &spsStrTTIMap_g[internalCellIndex][dlSpsInfo_p->deactTtiIndex];
        ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
             (dlSpsInfo_p->deactTtiIndex + SPS_INTERVAL_20) % 
             MAX_SPS_INTERVAL];

        /* CA Stats Changes Start */
        /* Delete the node from SDL and update the TTI Context */
            /* +- SPR 17777 */
        dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p, 
                dlSpsInfo_p
#ifdef SSI_DEBUG_STATS
                ,internalCellIndex
#endif
                );
            /* +- SPR 17777 */
        /* CA Stats Changes End */

        /* Store Deactivation tti Index for SDL deletion */
        dlSpsInfo_p->deactTtiIndex = ttiIndex;

        /* Get the TTI index in which SPS Deactivation to be initiated */
        ttiContext_p = &spsStrTTIMap_g[internalCellIndex][dlSpsInfo_p->deactTtiIndex];
        ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
             (dlSpsInfo_p->deactTtiIndex + SPS_INTERVAL_20) % 
             MAX_SPS_INTERVAL];

        dlSpsInfo_p->numDciAttempts = numDciAttempts;

        /* CA Stats Changes Start */
        /* Put entry in SDL and update the TTI Context Map */
        dlAddUeInSdl( ueIndex, ttiContext_p, ttiContextPeriodicity20_p,
                dlSpsInfo_p, internalCellIndex);

        /* Update Stats */
        LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, 
                getCurrentTick(), cause, dlSpsInfo_p->numDciAttempts, 
                internalCellIndex);
        /* CA Stats Changes End */
    } while( 0 );

    return retVal;
}

/*******************************************************************************
 * Function Name  : dlSpsDeactivated
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when in Deactivation
 *                  initiated state and any ACK/NACK received.
 *******************************************************************************/
MacRetType dlSpsDeactivated( UInt16 ueIndex, SpsStates state, SpsCause cause, 
        DLUEContext *dlUeCtx_p, void * spsCtx_p, UInt32 delayToPhy
        ,InternalCellIndex internalCellIndex )
{
    LP_SpsDlInfo          spsDlInfo_p   = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;    

            /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
            /* +- SPR 17777 */
    /*!
     * \code 
     * Remove entry from SPS DCI List, SDL.
     * Change the SPS state to SPS_DEACTIVATED.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
                   
    /* Get the TTI index in which SPS Deactivation to be initiated */
    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->deactTtiIndex];
    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
             (spsDlInfo_p->deactTtiIndex + SPS_INTERVAL_20) % 
             MAX_SPS_INTERVAL];
                   
    /* CA Stats Changes Start */
    /* Delete the node from SDL and update the TTI Context */
            /* +- SPR 17777 */
    dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p, 
            spsDlInfo_p
#ifdef SSI_DEBUG_STATS
            ,internalCellIndex
#endif
            );
            /* +- SPR 17777 */
    /* CA Stats Changes End */

    /* Store Deactivation tti Index for SDL deletion */
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;
    /* BUG 13541 */    

    /* Set the state accordingly */
    spsDlInfo_p->spsState = SPS_DEACTIVATED;

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(), 
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */
        
    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsUeSyncLossDetectedinDra
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when sync loss is
 *                  detected and activation request is received.
 *******************************************************************************/
MacRetType dlSpsUeSyncLossDetectedinDra( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy 
        ,InternalCellIndex internalCellIndex)
{
    LP_SpsDlInfo        spsDlInfo_p = PNULL;

            /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
            /* +- SPR 17777 */
    
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
    
    /* Change state of the UE to UE_NOT_READY */
    spsDlInfo_p->spsState = UE_NOT_READY;
    
    
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsUeSyncLossDetectedPdschResp
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when sync loss is
 *                  detected and SPS PDSCH sends SUCCESS/FAILURE Response to
 *                  strategy after reserving resources.
 *******************************************************************************/
MacRetType dlSpsUeSyncLossDetectedPdschResp( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy 
        ,InternalCellIndex internalCellIndex)
{
    LP_SpsDlInfo      spsDlInfo_p    = PNULL;
    LP_MacDlSpsResReq spsResRelReq_p = PNULL;

    LOG_UT( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause, 0, 0,0, 
            __func__, "");

    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    /* Allocate memory for Resource Release Request */
    spsResRelReq_p = (LP_MacDlSpsResReq)getMemFromPool(   
                               sizeof(MacDlSpsResReq), PNULL );
    if( PNULL == spsResRelReq_p )
    {
            /* +- SPR 17777 */
	LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
            getCurrentTick(), __LINE__, sizeof(MacDlSpsResReq),delayToPhy,sizeof(spsCtx_p),cause,
            state,0, __FILE__, __func__);
            /* +- SPR 17777 */
	ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
	ltePanic("getMemFromPool fails %s", __func__);
	/*KLOCKWORK_CHANGES_START_37*/
	return MAC_FAILURE;
	/*KLOCKWORK_CHANGES_STOP_37*/
    }
    
    /* Fill the structure for PDSCH for release request */
    spsResRelReq_p->ueIndex = ueIndex;
    spsResRelReq_p->requestType = MAC_SPS_RES_REL_REQ;
    
    /* Enqueue into dlSpsResRelQ_g */
    /* + Coverity Id - 32603 */
    if(ENQUEUE_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex], spsResRelReq_p ))
    {
        freeMemPool(spsResRelReq_p);

    }
    /* - Coverity Id - 32603 */
    
    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( DL_SPS_RES_RSV_REL_Q,  
            QCOUNT_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex] ), 
            internalCellIndex);
    /* CA Stats Changes End */

    /* Change state of the UE to UE_NOT_READY */
    spsDlInfo_p->spsState = UE_NOT_READY;
    
    
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsUeSyncLossDetectedinSdl
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when sync loss is
 *                  detected and node is residing in SDL i.e state is either
 *                  SPS_ACTIVATION_INITIATED or SPS_REACTIVATION_INITIATED.
 *******************************************************************************/
MacRetType dlSpsUeSyncLossDetectedinSdl( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy
        ,InternalCellIndex internalCellIndex)
{
    LP_SpsDlInfo        spsDlInfo_p    = PNULL;
    LP_MacDlSpsResReq   resRelReq_p    = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;    

    LOG_UT( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause, 0, 0,0, 
            __func__, "");

    /*KLOCKWORK_CHANGES_START_45*/
    /* Allocate memory for release request to PDSCH */
    resRelReq_p = (LP_MacDlSpsResReq)getMemFromPool( 
                              sizeof(MacDlSpsResReq), PNULL );
    if( PNULL == resRelReq_p )
    {
            /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacDlSpsResReq),delayToPhy,sizeof(spsCtx_p),cause,
                state,0, __FILE__, __func__);
            /* +- SPR 17777 */
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
        return MAC_FAILURE;
    }
    /*KLOCKWORK_CHANGES_STOP_45*/
    /*! \code 
     * Call dlDeleteUeFromSdl.
     * Send Resource release request to PDSCH in dlSpsResRelQ_g.
     * Change the SPS state to UE_NOT_READY.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    /* Get the TTI index in which SPS Deactivation to be initiated */
    ttiContext_p = &(spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->ttiIndex]);
    ttiContextPeriodicity20_p = &(spsStrTTIMap_g[internalCellIndex][
             (spsDlInfo_p->ttiIndex + SPS_INTERVAL_20) % 
             MAX_SPS_INTERVAL]);

    /* CA Stats Changes Start */
    /* Delete Node from SDL */
            /* +- SPR 17777 */
    dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p, 
            spsDlInfo_p
#ifdef SSI_DEBUG_STATS
            ,internalCellIndex
#endif
            );
            /* +- SPR 17777 */
    /* CA Stats Changes End */


    /* Fill information for release of resources */
    resRelReq_p->ueIndex = ueIndex;
    resRelReq_p->requestType = MAC_SPS_RES_REL_REQ;

    /* Enqueue in PDSCH resource release request queue, dlSpsResRelQ_g */
    /* + Coverity Id - 32605 */
    if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex], resRelReq_p ))
    {
        freeMemPool(resRelReq_p);

    }    
    /* - Coverity Id - 32605 */
    
    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( DL_SPS_RES_RSV_REL_Q,  
            QCOUNT_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex] ), 
            internalCellIndex);
    /* CA Stats Changes End */
    
    /* Change state of the UE to UE_NOT_READY */
    spsDlInfo_p->spsState = UE_NOT_READY;
    
    /* Reset schType to DRA_SCHEDULING */
    dlUeCtx_p->schType = DRA_SCHEDULING; 

    spsDlInfo_p->numOfSpsEmptyTx = 0;
    spsDlInfo_p->hpId = 0;
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;
    spsDlInfo_p->headPktSize = 0;
    spsDlInfo_p->spsOpp = 0;
    spsDlInfo_p->avgRlcSduSize = 0;
    spsDlInfo_p->pktArrivalTs = 0; 

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsUeSyncLossDetectedinSdlInDeactInit
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when sync loss is
 *                  detected and node is residing in SDL i.e state is either
 *                  SPS_DEACTIVATION_INITIATED.
 *******************************************************************************/
MacRetType dlSpsUeSyncLossDetectedinSdlInDeactInit( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex )
{
    LP_SpsDlInfo        spsDlInfo_p    = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;    

            /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
            /* +- SPR 17777 */
    /*! \code 
     * Call dlDeleteUeFromSdl.
     * Change the SPS state to UE_NOT_READY.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    /* Get the TTI index in which SPS Deactivation to be initiated */
    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->deactTtiIndex];
    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
             (spsDlInfo_p->deactTtiIndex + SPS_INTERVAL_20) % 
             MAX_SPS_INTERVAL];

    /* CA Stats Changes Start */
    /* Delete Node from SDL */
            /* +- SPR 17777 */
    dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p,
            spsDlInfo_p
#ifdef SSI_DEBUG_STATS
            ,internalCellIndex
#endif
            );
            /* +- SPR 17777 */
    /* CA Stats Changes End */

    /* Store Deactivation tti Index for SDL deletion */
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;
    
    /* Change state of the UE to UE_NOT_READY */
    spsDlInfo_p->spsState = UE_NOT_READY;
    
    /* Reset schType to DRA_SCHEDULING */
    dlUeCtx_p->schType = DRA_SCHEDULING; 

    spsDlInfo_p->numOfSpsEmptyTx = 0;
    spsDlInfo_p->hpId = 0;
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;
    spsDlInfo_p->headPktSize = 0;
    spsDlInfo_p->spsOpp = 0;
    spsDlInfo_p->avgRlcSduSize = 0;
    spsDlInfo_p->pktArrivalTs = 0; 
  
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsUeSyncLossDetectedinOccList
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when sync loss is
 *                  detected when in SPS_ACTIVATED state.
 *******************************************************************************/
MacRetType dlSpsUeSyncLossDetectedinOccList( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex )
{
    LP_SpsDlInfo      spsDlInfo_p   = PNULL;
    LP_MacDlSpsResReq resRelReq_p   = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p  = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;
    
    LOG_UT( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause, 0, 0,0, 
            __func__, "");
    
    /*! \code 
     * Call dlDeleteUeFromOccasionList.
     * Send Resource release request to PDSCH in dlSpsResRelQ_g.
     * Change the SPS state to UE_NOT_READY.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
    
    /* Get the TTI index in which SPS activation initiated */
    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->ttiIndex];
    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
                 (spsDlInfo_p->ttiIndex + SPS_INTERVAL_20) % 
                 MAX_SPS_INTERVAL];

    /* Allocate memory for release request to PDSCH */
    resRelReq_p = (LP_MacDlSpsResReq)getMemFromPool( 
                              sizeof(MacDlSpsResReq), PNULL );
    if( PNULL == resRelReq_p )
    {
            /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacDlSpsResReq),delayToPhy,sizeof(spsCtx_p),cause,
                state,0, __FILE__, __func__);
            /* +- SPR 17777 */
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
        /*KLOCKWORK_CHANGES_START_44*/
        return MAC_FAILURE;
        /*KLOCKWORK_CHANGES_STOP_44*/
    }
    /* Fill information for release of resources */
    resRelReq_p->ueIndex = ueIndex;
    resRelReq_p->requestType = MAC_SPS_RES_REL_REQ;
            
    /* CA Stats Changes Start */
    /* Delete the node from occasion list */
            /* +- SPR 17777 */
    dlDeleteUeFromOccasionList( ttiContext_p, ttiContextPeriodicity20_p, 
            spsDlInfo_p
#ifdef SSI_DEBUG_STATS
            ,internalCellIndex
#endif
            );
            /* +- SPR 17777 */
    /* CA Stats Changes End */

    /* Enqueue in PDSCH resource release request queue, dlSpsResRelQ_g */
    /* + Coverity Id - 32604 */
    if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex], resRelReq_p ))
    {
        freeMemPool(resRelReq_p);

    }
    /* - Coverity Id - 32604 */

    /* Update Q Stats */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( DL_SPS_RES_RSV_REL_Q,  
            QCOUNT_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex] ), 
            internalCellIndex);
    /* CA Stats Changes End */
    
    /* Change state of the UE to UE_NOT_READY */
    spsDlInfo_p->spsState = UE_NOT_READY;
   
    /* Reset schType to DRA_SCHEDULING */
    dlUeCtx_p->schType = DRA_SCHEDULING; 

    spsDlInfo_p->numOfSpsEmptyTx = 0;
    spsDlInfo_p->hpId = 0;
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;
    spsDlInfo_p->headPktSize = 0;
    spsDlInfo_p->spsOpp = 0;
    spsDlInfo_p->avgRlcSduSize = 0;
    spsDlInfo_p->pktArrivalTs = 0; 

    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */
    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsImplicitRelease
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function called when implicit release takes place and UE
 *                  is in DRA.
 *******************************************************************************/
MacRetType dlSpsImplicitRelease( UInt16 ueIndex, SpsStates state, 
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy 
        ,InternalCellIndex internalCellIndex)
{
    LP_SpsDlInfo  spsDlInfo_p = PNULL;
    
            /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
            /* +- SPR 17777 */
    
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
    
    /* Change state of the UE to SPS_IDLE */
    spsDlInfo_p->spsState = SPS_IDLE;
    
    
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(), 
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */
    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsUeImplicitRelPdschResp
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when SPS_RELEASE
 *                  triggered by L3.
 *******************************************************************************/
MacRetType dlSpsUeImplicitRelPdschResp( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy 
        ,InternalCellIndex internalCellIndex)
{
    LP_SpsDlInfo      spsDlInfo_p    = PNULL;

            /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
            /* +- SPR 17777 */

    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    /*! \code
     * Clears resources reserved by SPS PDSCH if allocatedRB is not 0 
     * \endcode
     * */
    if( 0 != spsDlInfo_p->resAllocInfo.allocatedRB )
    {   
        processDLResourceReleaseRequest( dlUeCtx_p ,internalCellIndex);
    }

    /* Change state of the UE to SPS_IDLE */
    spsDlInfo_p->spsState = SPS_IDLE;
    /* Reset SPS DL Info */
    spsDlInfo_p->numOfSpsEmptyTx = 0;
    spsDlInfo_p->hpId = 0;
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;
    spsDlInfo_p->headPktSize = 0;
    spsDlInfo_p->spsOpp = 0;
    spsDlInfo_p->avgRlcSduSize = 0;
    spsDlInfo_p->pktArrivalTs = 0; 
    
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsUeImplicitReleaseinSdl
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when implicit release
 *                  received from L3 and node is residing in SDL i.e state is
 *                  either SPS_ACTIVATION_INITIATED or SPS_DEACTIVATION_INITIATED
 *                  or SPS_REACTIVATION_INITIATED.
 *******************************************************************************/
MacRetType dlSpsUeImplicitReleaseinSdl( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy 
        ,InternalCellIndex internalCellIndex)
{
    LP_SpsDlInfo        spsDlInfo_p = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p  = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;

    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
    /*! \code 
     * Call dlDeleteUeFromSdl.
     * Change the SPS state to SPS_IDLE.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    /* Get tti Context*/
    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->ttiIndex];
    ttiContextPeriodicity20_p = &(spsStrTTIMap_g[internalCellIndex][ (spsDlInfo_p->ttiIndex +
                                        SPS_INTERVAL_20) % MAX_SPS_INTERVAL]);

    /* Delete Node from SDL */
    dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p,
            spsDlInfo_p
#ifdef SSI_DEBUG_STATS
            ,internalCellIndex
#endif
            );

    /*! \code
     * Clears resources reserved by SPS PDSCH if allocatedRB is not 0 
     * \endcode
     * */
    if( 0 != spsDlInfo_p->resAllocInfo.allocatedRB )
    {   
        processDLResourceReleaseRequest( dlUeCtx_p ,internalCellIndex);
    }
        
    /* Change state of the UE to SPS_IDLE */
    spsDlInfo_p->spsState = SPS_IDLE;

    /* Reset SPS DL Info */
    spsDlInfo_p->numOfSpsEmptyTx = 0;
    spsDlInfo_p->hpId = 0;
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;
    spsDlInfo_p->headPktSize = 0;
    spsDlInfo_p->spsOpp = 0;
    spsDlInfo_p->avgRlcSduSize = 0;
    spsDlInfo_p->pktArrivalTs = 0;     

    /* + SPR 20756 */
    /* Free the SPS HARQ Processes if in use */
    checkAndFreeSpsHarqProcess( dlUeCtx_p, internalCellIndex );
    /* - SPR 20756 */

    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
            cause, 0, internalCellIndex);

    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsUeImplicitReleaseinSdlInDeactInit
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when implicit release
 *                  received from L3 and node is residing in SDL i.e state is
 *                  either SPS_ACTIVATION_INITIATED or SPS_DEACTIVATION_INITIATED
 *                  or SPS_REACTIVATION_INITIATED.
 *******************************************************************************/
MacRetType dlSpsUeImplicitReleaseinSdlInDeactInit( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex )
{
    LP_SpsDlInfo        spsDlInfo_p = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p  = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;

    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
    /*! \code 
     * Call dlDeleteUeFromSdl.
     * Send Resource release request to PDSCH in dlSpsResRelQ_g.
     * Change the SPS state to SPS_IDLE.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    /* Get tti Context*/
    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->deactTtiIndex];
    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][ (spsDlInfo_p->deactTtiIndex +
                                        SPS_INTERVAL_20) % MAX_SPS_INTERVAL];
    
    /* Delete Node from SDL */
    dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p,
            spsDlInfo_p
#ifdef SSI_DEBUG_STATS
            ,internalCellIndex
#endif
            );

    /* Store Deactivation tti Index for SDL deletion */
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;
   
    /* Change state of the UE to SPS_IDLE */
    spsDlInfo_p->spsState = SPS_IDLE;
    
    /* + SPR 20756 */
    /* Reset SPS DL Info */
    spsDlInfo_p->numOfSpsEmptyTx = 0;
    spsDlInfo_p->hpId = 0;
    spsDlInfo_p->headPktSize = 0;
    spsDlInfo_p->spsOpp = 0;
    spsDlInfo_p->avgRlcSduSize = 0;
    spsDlInfo_p->pktArrivalTs = 0;     
    
    /* Free the SPS HARQ Processes if in use */
    checkAndFreeSpsHarqProcess( dlUeCtx_p, internalCellIndex );
    /* - SPR 20756 */
    
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
            cause, 0, internalCellIndex);

    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsImplicitReleaseinOccList
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function takes care for use cases when implicit release
 *                  received from L3 when in SPS_ACTIVATED state.
 *******************************************************************************/
MacRetType dlSpsImplicitReleaseinOccList( UInt16 ueIndex, 
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy,InternalCellIndex internalCellIndex )
{
    LP_SpsDlInfo      spsDlInfo_p   = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p  = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;
    
    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
    
    /*! \code 
     * Call dlDeleteUeFromOccasionList.
     * Change the SPS state to SPS_IDLE.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
    
    /* Get the TTI index in which SPS activation initiated */
    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->ttiIndex];
    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
                 (spsDlInfo_p->ttiIndex + SPS_INTERVAL_20) % 
                 MAX_SPS_INTERVAL];
    
    /* Delete the node from occasion list */
    dlDeleteUeFromOccasionList( ttiContext_p, ttiContextPeriodicity20_p, 
            spsDlInfo_p
#ifdef SSI_DEBUG_STATS
            ,internalCellIndex
#endif
            );

    /*! \code
     * Clears resources reserved by SPS PDSCH if allocatedRB is not 0 
     * \endcode
     * */
    if( 0 != spsDlInfo_p->resAllocInfo.allocatedRB )
    {   
        processDLResourceReleaseRequest( dlUeCtx_p ,internalCellIndex);
    }
    
    /* Change state of the UE to SPS_IDLE */
    spsDlInfo_p->spsState = SPS_IDLE;
    /* Reset SPS DL Info */
    spsDlInfo_p->numOfSpsEmptyTx = 0;
    spsDlInfo_p->hpId = 0;
    spsDlInfo_p->deactTtiIndex = MAX_SPS_INTERVAL;
    spsDlInfo_p->headPktSize = 0;
    spsDlInfo_p->spsOpp = 0;
    spsDlInfo_p->avgRlcSduSize = 0;
    spsDlInfo_p->pktArrivalTs = 0; 

    /* + SPR 20756 */
    /* Free the SPS HARQ Processes if in use */
    checkAndFreeSpsHarqProcess( dlUeCtx_p, internalCellIndex );
    /* - SPR 20756 */
    
    /* Update Stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(),
            cause, 0, internalCellIndex);
    
    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsUeSyncLossOff
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function called when sync loss off is detected.
 *******************************************************************************/
MacRetType dlSpsUeSyncLossOff( UInt16 ueIndex, SpsStates state,
        SpsCause cause, DLUEContext *dlUeCtx_p, void * spsCtx_p, 
        UInt32 delayToPhy ,
        InternalCellIndex internalCellIndex)
{
    LP_SpsDlInfo      spsDlInfo_p   = PNULL;
    
    /* +- SPR 17777 */
    LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY, 
            getCurrentTick(), __LINE__, ueIndex, state, cause,internalCellIndex,delayToPhy,sizeof(spsCtx_p), 
            __func__, "");
    /* +- SPR 17777 */
    
    /*! \code 
     * Change the SPS state to SPS_IDLE or SPS_CONFIGURED.
     * \endcode
     * */
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    if( SPS_RELEASE == spsDlInfo_p->requestType)
    {
        spsDlInfo_p->spsState = SPS_IDLE;
    }
    else
    {
        spsDlInfo_p->spsState = SPS_CONFIGURED;
    }
   
    /* CA Stats Changes Start */
    /* Update stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(), 
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */
    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsNotValidEvent
 * Input          : ueIndex - UE ID,
 *                  state - SPS state at which request comes,
 *                  cause - why activation required,
 *                  dlUeCtx_p - Pointer to DLUEContext,
 *                  spsCtx_p - pointer to input parameter context,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function called when wrong cause is called in different
 *                  state and logs and ignore the call.
 *******************************************************************************/
MacRetType dlSpsNotValidEvent( UInt16 ueIndex, SpsStates state, SpsCause cause, 
        DLUEContext *dlUeCtx_p, void * spsCtx_p, UInt32 delayToPhy 
        ,InternalCellIndex internalCellIndex)
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(dlUeCtx_p)
    LOG_MAC_MSG( MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,
            getCurrentTick(), __LINE__, ueIndex, state, cause,sizeof(spsCtx_p),delayToPhy,
            internalCellIndex, __func__, "");
    /* +- SPR 17777 */
    
    /* CA Stats Changes Start */
    /* Update stats */
    LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCtx_p, getCurrentTick(), 
            cause, 0, internalCellIndex);
    /* CA Stats Changes End */

    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : processDLSpsOccasion
 * Input          : currentGlobalTTITickCount - current Global Tick,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks for SPS occasion and calls to process
 *                  SDL and occasion list.
 *******************************************************************************/
/* SPR 15909 fix start */
UInt32 processDLSpsOccasion( tickType_t currentGlobalTTITickCount, 
/* SPR 15909 fix end */
        UInt32 delayToPhy ,InternalCellIndex internalCellIndex)
{
    UInt8                        spsOccasionUeCount  = 0;
    UInt8                        sdlCount            = 0;
    UInt16                       index               = 0;
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    UInt16                       prevIndex           = 0;
    LP_SpsStrIntervalTTIContext  prevTtiCtx_p = PNULL;
#endif
    /* - SPS_TDD_Changes */
    LP_SpsStrIntervalTTIContext  spsStrIntervalTTI_p = PNULL;

    /*! 
     * \code
     * - Find the Interval TTI Index. 
     * - Find the TTI Context
     * \endcode 
     * */
    index = (currentGlobalTTITickCount + delayToPhy) % MAX_SPS_INTERVAL;
    spsStrIntervalTTI_p = &(spsStrTTIMap_g[internalCellIndex][index]);
   
    /* Assign RBs reserved for this TTI for DRA startegy */
    numDlSpsRbsInTti_g[internalCellIndex] = spsStrIntervalTTI_p->numDlSpsRbsPerTti;

    /* Initialize SPS UEs to 0 for this tick */
    numDlSpsUeInTti_g[internalCellIndex] = 0;

    /*! \code
     * Reset schType of each UE present in SDL and occasion list in previous 
     * ttiIndex 
     * \endcode 
     * */
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    /* - SPS_TDD_Changes */
    if( 0 == index  )
    {
        prevIndex = MAX_SPS_INTERVAL - 1; 
    }
    else
    {
        prevIndex = index - 1;
    }
    prevTtiCtx_p =  &spsStrTTIMap_g[internalCellIndex][prevIndex];
    /* Reset schType for all UEs in occ list in prev tick */
    spsOccasionUeCount = prevTtiCtx_p->numDlSpsOccasionPerTti;
    if (0 != spsOccasionUeCount)
    {
        dlSpsResetOccSchType( prevTtiCtx_p, spsOccasionUeCount );
    }
    
    /* Reset schType for all UEs in SDL in prev tick */
    sdlCount = prevTtiCtx_p->numDlSpsDciPerTti;
    if( 0 != sdlCount )
    {
        dlSpsResetSdlSchType( prevTtiCtx_p, sdlCount );
    }
    /* + SPS_TDD_Changes */
#endif
    /* - SPS_TDD_Changes */

    /*! \code
     * Get the Number of SPS UE's for which Occasion has to be processed in the
     * TTI index. If spsOccasionUeCount > 0, then Process All The Ue in the 
     * list till maxDlSpsOccasionPerTti.
     * \endcode 
     * */
    spsOccasionUeCount = spsStrIntervalTTI_p->numDlSpsOccasionPerTti;
    if( 0 != spsOccasionUeCount )
    {
        /* Process occasion list for the TTI context */
        processDLSpsOccList( spsStrIntervalTTI_p, spsOccasionUeCount, index, 
                             delayToPhy ,internalCellIndex);
    }

    /*! \code
     * Get the Number of SPS UE's for which SDL has to be processed in this TTI.
     * if sdlCount > 0, then Process All The UE in the SDL.
     * \endcode 
     * */
    sdlCount = spsStrIntervalTTI_p->numDlSpsDciPerTti;
    if( 0 != sdlCount )
    {
        /* Process SDL for the TTI context */
        processDLSpsSdl( spsStrIntervalTTI_p, sdlCount, delayToPhy ,internalCellIndex);
    }

    return MAC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dlSpsResetOccSchType
 * Input          : ttiContext_p - TTI context for tick,
 *                  count - Number of UEs eligible for SPS occasion in SDL
 * Outputs        : None
 * Returns        : None
 * Description    : This function resets the schType for the UEs in occasion
 *                  list in previous tick.
 *******************************************************************************/
void dlSpsResetOccSchType( LP_SpsStrIntervalTTIContext ttiContext_p,
    UInt8 count )
{
    LP_MacSpsOccList  occNode_p      = PNULL;
    DLUEContext       *ueDLContext_p = PNULL;

    /*! \code
     * Reset schType for each node in TTI context.
     * \endcode 
     * */
    GET_FIRST_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsOccasionList, 
                                       occNode_p );

    /* + coverity 25231 */
    while( PNULL != occNode_p && count-- )
    /* - coverity 25231 */
    {        
        ueDLContext_p = dlUECtxInfoArr_g[occNode_p->ueIndex].dlUEContext_p;
        if( PNULL == ueDLContext_p )
        {
            LOG_MAC_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, 
                    MAC_DL_Strategy, getCurrentTick(), __LINE__, 
                    occNode_p->ueIndex, 0,0,0, 0,0, __func__, "SPS OCC List");
            continue;            
        }
        /* Update last scheduled tick */
        ueDLContext_p->schType = DRA_SCHEDULING;

        /* Get next node of queue */
        GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsOccasionList,
                                          occNode_p );
    }
}

/*******************************************************************************
 * Function Name  : dlSpsResetSdlSchType
 * Input          : ttiContext_p - TTI context for tick,
 *                  count - Number of UEs eligible for SPS occasion in SDL
 * Outputs        : None
 * Returns        : None
 * Description    : This function resets  the schType for the UEs in SDL list
 *                  in previous tick.
 *******************************************************************************/
void dlSpsResetSdlSchType( LP_SpsStrIntervalTTIContext ttiContext_p,
    UInt8 count )
{
    LP_MacSpsDciListTTI  sdlNode_p        = PNULL;
    DLUEContext          *ueDLContext_p   = PNULL;

    /*! \code
     * Reset schType for each node in TTI context.
     * \endcode 
     * */
    GET_FIRST_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsDciList, sdlNode_p);

    /* + coverity 25232 */
    while( PNULL != sdlNode_p && count-- )
    /* - coverity 25232 */
    {        
        ueDLContext_p = dlUECtxInfoArr_g[sdlNode_p->ueIndex].dlUEContext_p;
        if( PNULL == ueDLContext_p )
        {
            LOG_MAC_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, 
                    MAC_DL_Strategy, getCurrentTick(), __LINE__, 
                    sdlNode_p->ueIndex, 0,0,0, 0,0, __func__, "SPS DCI List");
            continue;            
        }
        /* Update last scheduled tick */
        ueDLContext_p->schType = DRA_SCHEDULING;

        /* Get next node of queue */
        GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsDciList,
                                          sdlNode_p );
    }
}
 
/*******************************************************************************
 * Function Name  : processDLSpsSdl
 * Input          : spsTTIContext_p - TTI context for current tick,
 *                  sdlCount - Number of UEs eligible for SPS occasion in SDL,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function Processes the SPS DCI list on the SPS occasion
 *                  and sends DCI information on that occasion for activation.
 *******************************************************************************/
MacRetType processDLSpsSdl( LP_SpsStrIntervalTTIContext spsTTIContext_p, 
        UInt8 sdlCount, UInt32 delayToPhy ,
        InternalCellIndex internalCellIndex)
{
    /*coverity-530 CID-32335 */
    MacRetType           retVal           = MAC_SUCCESS;
    /*coverity-530 CID-32335 */
    MsgType              msgType          = SPS_DEACTIVATION;
    SchType              schType          = DRA_SCHEDULING;
    LP_MacSpsDciListTTI  sdlNode_p        = PNULL;
    LP_MacSpsDciListTTI  tempSdlNode_p    = PNULL;
    LP_MacDlSpsStatus    statusNode_p     = PNULL;
    DLStrategyTxNode     *dlStrNode_p     = PNULL;
    DLHARQProcess        *harqProcess_p   = PNULL;
    DLUEContext          *ueDLContext_p   = PNULL;
    LP_SpsDlInfo         spsDlInfo_p      = PNULL;
    DLUeScheduleInfo     *dlUeSchedInfo_p = PNULL;

        /*SPR 20899 Fix Start*/
    UInt8 strategyContainerIndex = MODULO_TWO(getCurrentTick()); 
        /*SPR 20899 Fix Stop*/
    /* +- SPR 17777 */

    /*! 
     * \code
     * Process for each node in TTI context.
     * Get the first SDL node in the TTI context.
     * \endcode 
     * */
    GET_FIRST_MAC_SPS_UE_LIST_PER_TTI(spsTTIContext_p->spsDciList, sdlNode_p);

    /* + coverity 25234 */
    while( PNULL != sdlNode_p && sdlCount-- )
    /* - coverity 25234 */
    {        
        /* SPR 16737 Fix Start */
        msgType = SPS_DEACTIVATION;
        schType = DRA_SCHEDULING;
        /* SPR 16737 Fix End */
        ueDLContext_p = dlUECtxInfoArr_g[sdlNode_p->ueIndex].dlUEContext_p;
        if( PNULL == ueDLContext_p )
        {
            LOG_MAC_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, 
                    MAC_DL_Strategy, getCurrentTick(), __LINE__, 
                    sdlNode_p->ueIndex, 0,0,0, 0,0, __func__, "SPS DCI List");
            retVal = MAC_FAILURE;
            continue;            
        }
        spsDlInfo_p = &(ueDLContext_p->spsDlInfo);

        /*! \code
         * If UE has moved to Pending delete, Clean the SPS context 
         * \endcode
         * */
        if( 0 != dlUECtxInfoArr_g[sdlNode_p->ueIndex].pendingDeleteFlag )
        {
            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsTTIContext_p->spsDciList,
                    sdlNode_p);
       
            /* UE has moved to Pending delete so clean up the SPS context */
            dlSpsPendingDeleteHandling( ueDLContext_p ,internalCellIndex);
            continue;
        }
        /*! \code
         * - If UL_SYNC_LOSS_ON for the UE
         *       - call dlSpsUeSyncLossDetectedinSdl if in ACTIVATION_INITATED
         *         else call dlSpsUeSyncLossDetectedinSdlInDeactInit.
         *       - Process the next Ue Node of SDL.
         *  \endcode
         * */
        if( UL_SYNC_LOSS_ON == checkUlSyncLossForUE(ueDLContext_p, 
                                              getCurrentTick() + delayToPhy,internalCellIndex) )
        {
            /* Store the node that needs to be deleted from queue */
            tempSdlNode_p = sdlNode_p;        

            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI(
                    spsTTIContext_p->spsDciList, sdlNode_p);
            /* +- SPR 17777 */

            /* Call state machine for removing UE from SDL,send
             * Resource Release Request to PDSCH, and change state */
            /* + coverity 25234 */
            dlStateMachine[spsDlInfo_p->spsState][UE_SYNC_LOSS_ON](
                    tempSdlNode_p->ueIndex, spsDlInfo_p->spsState, 
                    UE_SYNC_LOSS_ON, ueDLContext_p, tempSdlNode_p, 
                    delayToPhy ,internalCellIndex);
            /* - coverity 25234 */
    /* +- SPR 17777 */
            LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, tempSdlNode_p->ueIndex,spsDlInfo_p->spsState, UE_SYNC_LOSS_ON,
                     spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
    /* +- SPR 17777 */

            continue;
        }
            
        /* Update last scheduled tick */
        ueDLContext_p->lastScheduledTick = getCurrentTick();

        /*! \code
         * If Maximum Number of DCI Attempts reached 
         * \endcode
         * */
        if( spsDlInfo_p->numDciAttempts == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                spsSysWideParam.maxSpsDciAttempt )
        {
            /*! \code
             *  call dlSpsMaxDciAttemptReached and continue 
             * \endcode
             * */
            /* Store the node that needs to be deleted from queue */
            tempSdlNode_p = sdlNode_p;        
            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsTTIContext_p->spsDciList,
                    sdlNode_p);
            /* +- SPR 17777 */
            /* Call state machine for deletion of temporary node from queue and
             * do the desired handling */
            dlStateMachine[spsDlInfo_p->spsState][MAX_DCI_ATTEMPT](
                      tempSdlNode_p->ueIndex, spsDlInfo_p->spsState,
                      MAX_DCI_ATTEMPT, ueDLContext_p, tempSdlNode_p, 
                      delayToPhy ,internalCellIndex);
    /* +- SPR 17777 */
            LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, tempSdlNode_p->ueIndex,spsDlInfo_p->spsState, MAX_DCI_ATTEMPT,
                     spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
    /* +- SPR 17777 */
            continue;
        }

        /*! \code
         * Fill HARQ Process to be pushed in PDCCH Queue.
         * \endcode
         * */

        /* Overwrite msgType with SPS_DEACTIVATION if state is 
         * SPS_DEACTIVATION_INITIATED */
        if( SPS_ACTIVATION_INITIATED == spsDlInfo_p->spsState )
        {
            msgType = SPS_ACTIVATION; 
            schType = SPS_SCHEDULING;
        }
        
        /*! \code
         * Get the Harq Process and check if it is available
         * \endcode
         * */           
        if (PNULL == (harqProcess_p = checkForFreeHarqProcess( ueDLContext_p,
                        ueDLContext_p->dlHarqContext_p, schType, internalCellIndex) ) )
        {
            LOG_MAC_MSG( MAC_ALL_HARQ_ARE_BUSY, LOGFATAL, MAC_DL_Strategy,
                    getCurrentTick(), ueDLContext_p->ueIndex, 0, 0, 0, 0,
                    0, 0, __func__, "");
            /* As currently HARQ is not free, hence retry at next tick available
             * and so again enqueueing to STATUS queue, cause is
             * MAX_SPS_HARQ_RETX as functionality is same */
            if( SPS_DEACTIVATION_INITIATED == spsDlInfo_p->spsState )
            {
                statusNode_p = spsDLAllocateStatusDataReq( sdlNode_p->ueIndex,
                        MAX_SPS_HARQ_RETX, 0, 0, 0);
                /*Cov_fix_30April_63542_start*/
                if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], statusNode_p ))
                {
                    freeMemPool(statusNode_p);
                    statusNode_p = PNULL;
                }
                /*Cov_fix_30April_63542_stop*/
                /* Update Queue Stats */
                /* CA Stats Changes Start */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q,
                        QCOUNT_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex]),
                        internalCellIndex);
                /* CA Stats Changes End */
            }
            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsTTIContext_p->spsDciList,
                    sdlNode_p);
            continue;
        }
        /*! \code
         * Store HARQ Id in SPS UE context.
         * \endcode
         * */
        spsDlInfo_p->hpId = harqProcess_p->harqProcessId;

        harqProcess_p->dlHARQTBOneInfo.tbSize = spsDlInfo_p->spsOpp;
        harqProcess_p->dlHARQTBTwoInfo.tbSize = 0;
        /* + 16761 changes start */
        /* + SPS_TDD_Changes */
        harqProcess_p->dciFormat = DCI_FORMAT_1A;
        /* - SPS_TDD_Changes */
        /* - 16761 changes end */
       
        /* set SchType to SPS_SCHEDULING as SPS needs to be scheduled in this
         * tick */ 
        ueDLContext_p->schType = SPS_SCHEDULING;
        /* Increment UE for which DCI needs to be sent */
        numDlSpsUeInTti_g[internalCellIndex]++; 

        /* CA Stats Changes Start */
        /* Update HARQ process Id Stats */
        LTE_MAC_UPDATE_HARQ_PROCESS_ID_USED( ueDLContext_p->ueIndex,
                spsDlInfo_p->hpId, internalCellIndex);
        /* CA Stats Changes End */
        dlStrNode_p = fillDlSpsDciInfo( ueDLContext_p, harqProcess_p, 
                                        msgType ,internalCellIndex);
        /*cov warning 60997 fix start*/
        if(PNULL == dlStrNode_p)
        {
            /*if(PNULL!=sdlNode_p)
            {
               freeMemPool(sdlNode_p);
            }*/
            freeDLHarqProcess(ueDLContext_p,harqProcess_p, internalCellIndex);
            continue;
        }
        /*cov warning 60997 fix end*/
        if( DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                dlSchdConfig.dlSchedulerStrategy)
        {
             /*putEntry of DLStrategyTXNode in DL Strategy Schedule Info */            
            dlUeSchedInfo_p = &(dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].dlUeScheduleInfo
                                [dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount]);

            dlUeSchedInfo_p->ueDLContext_p = ueDLContext_p;
            dlUeSchedInfo_p->dLStrategyTxNode_p = dlStrNode_p;
            dlUeSchedInfo_p->harqProcess_p = harqProcess_p;
            dlUeSchedInfo_p->isTaDrxScheduled = msgType;

            /* Increament the scheduled UE count */
            dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount++;
        }
        else
        {
            /* Push DLStrategyTXNode in PDCCH Queue */
            if(!(putEntryInDLSchedulerNewTxQueue( dlStrNode_p ,internalCellIndex)))
            {
                freeDLHarqProcess(ueDLContext_p,harqProcess_p, internalCellIndex);
                /* SPR 21036 start */
                /* freeMemPool(dlStrNode_p); */
                /* dlStrNode_p = PNULL; */
                /* SPR 21036 end   */  
                ueDLContext_p->dlStrategyTxNode_p = PNULL;
            }
        }

        /* CA Stats Changes Start */
        /* Update SPS Strategy Stats */
        LTE_MAC_UPDATE_SPS_DL_STR_STATS( sdlNode_p->ueIndex, ueDLContext_p, 
                getCurrentTick(), SDL_PROCESSED, 
                spsDlInfo_p->numDciAttempts, internalCellIndex);
        /* CA Stats Changes End */
    
        GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsTTIContext_p->spsDciList,
                                          sdlNode_p );
    }
    return retVal;
}

/*******************************************************************************
 * Function Name  : processDLSpsOccList
 * Input          : spsStrIntervalTTI_p - TTI context for current tick,
 *                  spsOccasionUeCount - Number of UEs eligible for SPS occasion
 *                                       in occasion list,
 *                  ttiIndex - Index from TTI Map in which SPS occasion is
 *                             processed,
 *                  delayToPhy - Delay to PHY
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Scheduler Strategy will allocate the RBs to TA transmission,
 *                   DRX transmission and to UE depending upon the number of
 *                   sub-frames in advance for which strategy is run to schedule
 *                   allocations and depending upon the available queue load
 *                   of the UEs in the downlink.
 *                   The strategy uses either the proportional fair or round
 *                   robin algorithm to distribute the RBs.
 *******************************************************************************/
MacRetType processDLSpsOccList( LP_SpsStrIntervalTTIContext  spsStrIntervalTTI_p,
        UInt8 spsOccasionUeCount, UInt16 ttiIndex, UInt32 delayToPhy 
        ,InternalCellIndex internalCellIndex)
{
    /*coverity-530 CID-32335 */
    MacRetType                retVal            = MAC_SUCCESS;
    /*coverity-530 CID-32335 */
    UInt16                    dlSchedInterval   = 0;
    LP_SpsDlInfo              spsDlInfo_p       = PNULL;
    DLUEContext*              ueDLContext_p     = PNULL;
    LP_MacSpsOccList          occNode_p         = PNULL;
    LP_MacSpsOccList          tempOccNode_p     = PNULL;
    LP_SpsCommIntervalProfile commIntProfile_p  = PNULL;
    LP_MacDlSpsExclude        dlSpsExcludeUe_p  = PNULL;; 
    LP_MacDlSpsDeactReq       macDlSpsDectReq_p = PNULL;
    /* +- SPR 17777 */

    /*! 
     * \code
     * - Get the SPS Occasion list for the TTI index. 
     * \endcode 
     * */
    GET_FIRST_MAC_SPS_UE_LIST_PER_TTI(spsStrIntervalTTI_p->spsOccasionList,
            occNode_p);

    /* + coverity 25233 */
    while( PNULL != occNode_p && spsOccasionUeCount-- )
    /* - coverity 25233 */
    {
        /*! \code
         * Get the Ue Context of the Ue.
         * \endcode
         **/
        ueDLContext_p   = dlUECtxInfoArr_g[occNode_p->ueIndex].dlUEContext_p;
        if( PNULL == ueDLContext_p )
        {
            LOG_MAC_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, 
                    MAC_DL_Strategy, getCurrentTick(), __LINE__, 
                    occNode_p->ueIndex, 0,0,0, 0,0, __func__, "SPS Occ List");
            retVal = MAC_FAILURE;
            continue;            
        }
        spsDlInfo_p = &(ueDLContext_p->spsDlInfo);

        /*! \code
         * If UE has moved to Pending delete, Clean the SPS context 
         * \endcode
         * */
        if( 0 != dlUECtxInfoArr_g[occNode_p->ueIndex].pendingDeleteFlag )
        {
            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( 
                 spsStrIntervalTTI_p->spsOccasionList, occNode_p );
       
            /* UE has moved to Pending delete so clean up the SPS context */
            dlSpsPendingDeleteHandling( ueDLContext_p ,internalCellIndex);
            continue;
        }
        /*!
         * \code
         * - If UL_SYNC_LOSS_ON for the UE
         *       - call dlSpsUeSyncLossDetectedinOccList.
         *       - Process the next Ue Node of occasion List.
         * - Else If MCS drop flag is set
         *       - Trigger deactivation.
         * - Else 
         *       - Process the Current Ue Node.
         *  \endcode
         * */
        if( UL_SYNC_LOSS_ON == checkUlSyncLossForUE(ueDLContext_p, 
                                        getCurrentTick() + delayToPhy 
                                        ,internalCellIndex) )
        {
            tempOccNode_p = occNode_p;

            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI(
                    spsStrIntervalTTI_p->spsOccasionList,
                    occNode_p);
    /* +- SPR 17777 */
            
            /* Call state machine for removing UE from occasion list,send
             * Resource Release Request to PDSCH, and change state */
            dlStateMachine[spsDlInfo_p->spsState][UE_SYNC_LOSS_ON](
                    tempOccNode_p->ueIndex, spsDlInfo_p->spsState, 
                    UE_SYNC_LOSS_ON, ueDLContext_p, tempOccNode_p, 
                    delayToPhy ,internalCellIndex);

    /* +- SPR 17777 */
            LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                     getCurrentTick(), __LINE__, tempOccNode_p->ueIndex,spsDlInfo_p->spsState, UE_SYNC_LOSS_ON,
                     spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
    /* +- SPR 17777 */

            continue;
        }
        else if( LTE_TRUE == spsDlInfo_p->isUeDlWbMcsBelowThresh )
        {
            tempOccNode_p = occNode_p;

            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI(
                    spsStrIntervalTTI_p->spsOccasionList,
                    occNode_p);
    /* +- SPR 17777 */
             
            /* Call state machine for removing UE from occasion list,send
             * Resource Release Request to PDSCH, and change state */
            if( MAC_FAILURE == dlStateMachine[spsDlInfo_p->spsState]
                    [MCS_CHANGED]( tempOccNode_p->ueIndex, 
                        spsDlInfo_p->spsState,
                        MCS_CHANGED, ueDLContext_p, tempOccNode_p, 
                        delayToPhy ,internalCellIndex) )
            {
                /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                       getCurrentTick(), __LINE__, tempOccNode_p->ueIndex,spsDlInfo_p->spsState, MCS_CHANGED,
                       spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                /* Allocate the memory for deactivation queue node */
                macDlSpsDectReq_p = (LP_MacDlSpsDeactReq)getMemFromPool(
                        sizeof(MacDlSpsDeactReq), PNULL);
                if( PNULL == macDlSpsDectReq_p )
                {
                    LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, 
                            L2_SYS_FAIL, getCurrentTick(), __LINE__, 
                            sizeof(MacDlSpsDeactReq), 0, 0, 0, 0,0, 
                            __FILE__, __func__);
                    ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                            CRITICAL_ALARM);
                    ltePanic("getMemFromPool fails %s", __func__);
                    /** Klockwork Changes Start **/
                    continue;
                    /** Klockwork Changes End **/
                }
                /* Fill deactivation queue node */
                macDlSpsDectReq_p->ueIndex =  tempOccNode_p->ueIndex;
                macDlSpsDectReq_p->cause = MCS_CHANGED;
                /* Enqueue the node to deactivation queue so that it 
                 * can be processed in next tick */
                /* + Coverity Id - 32606 */
                if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsDeactQ_gp[internalCellIndex], 
                        macDlSpsDectReq_p ))
                {
                    freeMemPool(macDlSpsDectReq_p);
                    macDlSpsDectReq_p = PNULL;
                }
                /* - Coverity Id - 32606 */

                /* Update Q Stats */
                /* CA Stats Changes Start */
                LTE_MAC_QUEUE_USED( DL_SPS_DEACT_Q,  
                        QCOUNT_MAC_DL_SPS_Q( dlSpsDeactQ_gp[internalCellIndex] ), 
                        internalCellIndex);
                /* CA Stats Changes End */
            }
            else
            {
                /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                       getCurrentTick(), __LINE__, tempOccNode_p->ueIndex,spsDlInfo_p->spsState, MCS_CHANGED,
                       spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
            }
            continue;
        }
/* SPR 19038 Fix Start */
        /* If Meas Gap is conigured after SPS Activation 
         * Below code will initialte the deactivation process 
         * If Ue's Meas Gap collides with SPS occasion 
         */
        else if( LTE_TRUE == spsDlInfo_p->isUeMeasGapCollisionWithSpsOcc )
        {
            tempOccNode_p = occNode_p;

            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI(
                    spsStrIntervalTTI_p->spsOccasionList,
                    occNode_p);
            
            /* Call state machine for removing UE from occasion list,send
             * Resource Release Request to PDSCH, and change state */
            if( MAC_FAILURE == dlStateMachine[spsDlInfo_p->spsState]
                    [MEAS_GAP_COL_WITH_SPS_OCC]( tempOccNode_p->ueIndex, 
                        spsDlInfo_p->spsState,
                        MEAS_GAP_COL_WITH_SPS_OCC, ueDLContext_p, tempOccNode_p, 
                        delayToPhy,internalCellIndex ) )
            {
                /* Allocate the memory for deactivation queue node */
                macDlSpsDectReq_p = (LP_MacDlSpsDeactReq)getMemFromPool(
                        sizeof(MacDlSpsDeactReq), PNULL);
                if( PNULL == macDlSpsDectReq_p )
                {
                    LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, 
                            L2_SYS_FAIL, getCurrentTick(), __LINE__, 
                            sizeof(MacDlSpsDeactReq), 0, 0, 0, 0,0, 
                            __FILE__, __func__);
                    ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                            CRITICAL_ALARM);
                    ltePanic("getMemFromPool fails %s", __func__);
                }
                /* Fill deactivation queue node */
                macDlSpsDectReq_p->ueIndex =  tempOccNode_p->ueIndex;
                macDlSpsDectReq_p->cause = MEAS_GAP_COL_WITH_SPS_OCC;
                /* Enqueue the node to deactivation queue so that it 
                 * can be processed in next tick */
                /*CID 63541 Fix Start*/
		if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsDeactQ_gp[internalCellIndex],
                                              macDlSpsDectReq_p ))
                {
                	freeMemPool(macDlSpsDectReq_p);
                        macDlSpsDectReq_p = PNULL;
                }
                /* Update Q Stats */
                LTE_MAC_QUEUE_USED( DL_SPS_DEACT_Q,  
                QCOUNT_MAC_DL_SPS_Q( dlSpsDeactQ_gp[internalCellIndex]),internalCellIndex);
            }
            continue;
        }
/* SPR 19038 Fix End */
        else
        {
            /* Update last scheduled tick */
            ueDLContext_p->lastScheduledTick = getCurrentTick();

            /*!
             * \code
             * - If headPktSize = 0 
             *     Increment The Empty SPS Tx Count 
             *     If overrideSpsOccasionForOtherUe is enabled
             *        Put Entry in dlSpsUeExclude queue
             *        If Number of Empty Tx at SPS Occasion is >= 
             *                               explicitReleaseAfter
             *              Call dlSpsDeactInit()
             * - Else
             *     assignDLRBsAndSendToPDCCHFromSpsOccasion 
             *     Reset numOfSpsEmptyTx
             * \endcode
             * */
            if ( 0 == spsDlInfo_p->headPktSize)
            {
                spsDlInfo_p->numOfSpsEmptyTx++;

                dlSchedInterval = spsDlInfo_p->spsDlSetupInfo_p->\
                                           semiPersistentSchedIntervalDL;
                commIntProfile_p = &cellSpecificParams_g.\
                         cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                         spsCommIntervalProfile[dlSchedInterval];

                if ( commIntProfile_p->overrideSpsOccassionForOtherUe )
                {
                    /*! \code
                     * Allocate node for Exclude queue 
                     * \endcode 
                     * */
                    dlSpsExcludeUe_p = (LP_MacDlSpsExclude)getMemFromPool(
                                             sizeof(MacDlSpsExclude), PNULL );
                    if( PNULL == dlSpsExcludeUe_p )
                    {
                        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, 
                                L2_SYS_FAIL, getCurrentTick(), __LINE__, 
                                sizeof(MacDlSpsExclude), 0, 0, 0, 0,0, 
                                __FILE__, __func__);
                        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                                CRITICAL_ALARM);
                        ltePanic("getMemFromPool fails %s", __func__);
                        /** Klockwork Changes Start **/
                        continue;
                        /** Klockwork Changes End **/
                    }
                    dlSpsExcludeUe_p->ueIndex  = occNode_p->ueIndex;
                    dlSpsExcludeUe_p->ttiIndex =  ttiIndex;
                    /* + Coverity Id - 32607 */
                    if(!ENQUEUE_MAC_DL_SPS_Q(dlSpsUeExclude_gp[internalCellIndex], dlSpsExcludeUe_p))
                    {
                        freeMemPool(dlSpsExcludeUe_p);
                        dlSpsExcludeUe_p = PNULL;
                    }
                    /* - Coverity Id - 32607 */
    
                    /* Update Q Stats */
                    /* CA Stats Changes Start */
                    LTE_MAC_QUEUE_USED( DL_SPS_UE_EXCLUDE,  
                            QCOUNT_MAC_DL_SPS_Q( dlSpsUeExclude_gp[internalCellIndex] ), 
                            internalCellIndex);
                    /* CA Stats Changes End */

                    /* As zero Qload received and overrideSpsOccassionForOtherUe
                     * is enabled, decrement numOfRBs for this UE from 
                     * numDlSpsRbsInTti_g as the resources can be given to other
                     * UE */
                    numDlSpsRbsInTti_g[internalCellIndex] -= 
                        spsDlInfo_p->resAllocInfo.allocatedRB;
                }
                /* Call State Machine if number of empty transmissions have
                 * reached max value for SPS occasion */
                if( spsDlInfo_p->numOfSpsEmptyTx >= 
                    spsDlInfo_p->spsDlSetupInfo_p->explicitReleaseAfter )
                {
                    tempOccNode_p = occNode_p;
                    GET_NEXT_MAC_SPS_UE_LIST_PER_TTI(
                            spsStrIntervalTTI_p->spsOccasionList, occNode_p);
                    /* +- SPR 17777 */

                    if( MAC_FAILURE == dlStateMachine[spsDlInfo_p->spsState]
                                                     [MAX_EMPTY_TX](
                            tempOccNode_p->ueIndex, spsDlInfo_p->spsState, 
                            MAX_EMPTY_TX, ueDLContext_p, tempOccNode_p, 
                            delayToPhy,internalCellIndex ) )
                    {
                        /* +- SPR 17777 */
                        LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                             getCurrentTick(), __LINE__, tempOccNode_p->ueIndex,spsDlInfo_p->spsState, MAX_EMPTY_TX,
                             spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                        /* Allocate the memory for deactivation queue node */
                        macDlSpsDectReq_p = (LP_MacDlSpsDeactReq)getMemFromPool(
                                              sizeof(MacDlSpsDeactReq), PNULL);
                        if( PNULL == macDlSpsDectReq_p )
                        {
                            LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, 
                                    L2_SYS_FAIL, getCurrentTick(), __LINE__, 
                                    sizeof(MacDlSpsDeactReq), 0, 0, 0, 0,0, 
                                    __FILE__, __func__);
                            ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                                    CRITICAL_ALARM);
                            ltePanic("getMemFromPool fails %s", __func__);
                            /** Klockwork Changes Start **/
                            continue;
                            /** Klockwork Changes End **/
                        }
                        /* Fill deactivation queue node */
                        macDlSpsDectReq_p->ueIndex =  tempOccNode_p->ueIndex;
                        macDlSpsDectReq_p->cause = MAX_EMPTY_TX;
                        /* Enqueue the node to deactivation queue so that it 
                         * can be processed in next tick */
                        /*Cov_fix_30April_63541_start*/
                        if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsDeactQ_gp[internalCellIndex], 
                                              macDlSpsDectReq_p ))
                        {
                            freeMemPool(macDlSpsDectReq_p);
                            macDlSpsDectReq_p = PNULL;
                        }
                        /*Cov_fix_30April_63541_stop*/
                    
                        /* Update Q Stats */
                        /* CA Stats Changes Start */
                        LTE_MAC_QUEUE_USED( DL_SPS_DEACT_Q,  
                                QCOUNT_MAC_DL_SPS_Q( dlSpsDeactQ_gp[internalCellIndex] ), 
                                internalCellIndex);
                        /* CA Stats Changes End */
                    }
                    else
                    {
                        /* +- SPR 17777 */
                        LOG_MAC_MSG( MAC_DL_SPS_CURR_STATE, LOGDEBUG, MAC_DL_SPS_STRATEGY,
                             getCurrentTick(), __LINE__, tempOccNode_p->ueIndex,spsDlInfo_p->spsState, MAX_EMPTY_TX,
                             spsDlInfo_p->spsState, 0,0, __func__, "DlStateMachine");
                    }
                    continue;
                }
            }
            else
            {
                spsDlInfo_p->numOfSpsEmptyTx = 0;
            }
           
            /* In case of occasion, PDSCH is sent with no PDCCH. Incase of 0 
             * Q load, send Padding. */
            assignDLRBsAndSendToPDCCHFromSpsOccasion(ueDLContext_p,internalCellIndex);

            /* set SchType to SPS_SCHEDULING as SPS needs to be scheduled in 
             * this tick */ 
            ueDLContext_p->schType = SPS_SCHEDULING;

            /* CA Stats Changes Start */
            /* Update Stats */
            LTE_MAC_UPDATE_SPS_DL_STR_STATS( occNode_p->ueIndex, ueDLContext_p,
                    getCurrentTick(), OCC_PROCESSED, 0, internalCellIndex);
            /* CA Stats Changes End */
        }
        GET_NEXT_MAC_SPS_UE_LIST_PER_TTI(spsStrIntervalTTI_p->spsOccasionList,
                occNode_p);
    }
    return retVal;
}

/*******************************************************************************
 * Function Name  : assignDLRBsAndSendToPDCCHFromSpsOccasion
 * Input          : ueDLContext_p - Pointer toDLUEContext,
 * Outputs        : DLStrategyTxNode - is inserted into the PDCCH New Tx
 *                                     queues which will be used by the
 *                                     PDCCH  for CCE distribution
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Assigns Downlink RB's for SPS Occasion and prepares
 *                  The Strategy Node.
 *******************************************************************************/
MacRetType assignDLRBsAndSendToPDCCHFromSpsOccasion(DLUEContext *ueDLContext_p,
        InternalCellIndex internalCellIndex)
{
    /*coverity-530 CID-32335 */
    MacRetType        retVal           = MAC_SUCCESS;
    /*coverity-530 CID-32335 */
    LP_SpsDlInfo      spsDlInfo_p      = PNULL;
    DLHARQProcess     *harqProcess_p   = PNULL;
    DLStrategyTxNode  *dlStrNode_p     = PNULL;
    DLUeScheduleInfo  *dlUeSchedInfo_p = PNULL;

        /*SPR 20899 Fix Start*/
    UInt8 strategyContainerIndex = MODULO_TWO(getCurrentTick()); 
        /*SPR 20899 Fix Stop*/

    spsDlInfo_p = &ueDLContext_p->spsDlInfo;

    /*! \code
     * - Get the Harq Process and check if it is available
     * \endcode
     * */
    do
    {
        if (PNULL == (harqProcess_p = checkForFreeHarqProcess( ueDLContext_p,
                        ueDLContext_p->dlHarqContext_p, SPS_SCHEDULING, internalCellIndex) ) )
        {
            LOG_MAC_MSG( MAC_ALL_HARQ_ARE_BUSY, LOGFATAL, MAC_DL_Strategy,
                    getCurrentTick(), ueDLContext_p->ueIndex, 0, 0, 0, 0,
                    0, 0, __func__, "");
            retVal =  MAC_FAILURE;
            break;
        }
        /* Save in SPS UE Context */
        spsDlInfo_p->hpId = harqProcess_p->harqProcessId;

        /*! \code
         * - Fill the values in the dlStrategyTxNode 
         * - put Entry In DLSchedulerNewTxQueue
         * \endcode
         * */
        ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;

        /*Setting only TBOne as valid and other INVALID*/
        harqProcess_p->isTBOneValid = TRUE;
        harqProcess_p->isTBTwoValid = FALSE;
        harqProcess_p->dlHARQTBOneInfo.tbSize = spsDlInfo_p->spsOpp;
        /* + 16761 changes start */
        /* + SPS_TDD_Changes */
        harqProcess_p->dciFormat = DCI_FORMAT_1A;
        /* - SPS_TDD_Changes */
        /* - 16761 changes end */

        /*Preparing the DL Strategy Output Node*/

        dlStrNode_p = fillDlSpsDciInfo( ueDLContext_p, harqProcess_p, 
                SPS_OCCASION, internalCellIndex );  
        /*cov warning 60996 fixed start*/
        if(PNULL == dlStrNode_p)
        {
            freeDLHarqProcess(ueDLContext_p,harqProcess_p,internalCellIndex);
            freeMemPool(harqProcess_p);

            return MAC_FAILURE;
        }
        /*cov warning 60996 fixed end*/
        dlStrNode_p->minTBSize            = spsDlInfo_p->spsOpp;
        dlStrNode_p->maxTBSize            = spsDlInfo_p->spsOpp;
        dlStrNode_p->strictAllocatedFlag  = LTE_FALSE;

        if( DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                dlSchdConfig.dlSchedulerStrategy)
        {
             /*putEntry of DLStrategyTXNode in DL Strategy Schedule Info */            
            dlUeSchedInfo_p = &(dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].dlUeScheduleInfo
                                [dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount]);

            dlUeSchedInfo_p->ueDLContext_p = ueDLContext_p;
            dlUeSchedInfo_p->dLStrategyTxNode_p = dlStrNode_p;
            dlUeSchedInfo_p->harqProcess_p = harqProcess_p;
            dlUeSchedInfo_p->isTaDrxScheduled = SPS_OCCASION;

            /* Increament the scheduled UE count */
            dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount++;
        }
        else
        {
            /* Push DLStrategyTXNode in PDCCH Queue */
            if(!(putEntryInDLSchedulerNewTxQueue( dlStrNode_p ,internalCellIndex)))
            {
                freeDLHarqProcess(ueDLContext_p,harqProcess_p,internalCellIndex);
                /* SPR 21036 start */ 
                /* freeMemPool(dlStrNode_p);*/
                /* SPR 21036 end   */ 
                ueDLContext_p->dlStrategyTxNode_p = PNULL;
            }
        }
        /* Increment UE for which DCI needs to be sent */
        numDlSpsUeInTti_g[internalCellIndex]++; 
    }while( 0 );
    
    return retVal;
}

/*******************************************************************************
 * Function Name  : fillDlSpsDciInfo
 * Input          : ueDLContext_p - Pointer to DLUEContext,
 *                  harqProcess_p - Pointer to DLHARQProcess,
 *                  msgType - MsgType used
 * Outputs        : None
 * Returns        : Pointer to DCI node for PDCCH
 * Description    : This function allocates and fills information for DCI.
 *******************************************************************************/
DLStrategyTxNode * fillDlSpsDciInfo( 
        DLUEContext   *ueDLContext_p, 
        DLHARQProcess *harqProcess_p,
        MsgType       msgType, 
        InternalCellIndex internalCellIndex)
{
    DLStrategyTxNode       *dlStrNode_p           = PNULL;
    ResourceAllocatorInput *resAllocInput_p       = PNULL;
    DLQosStrategyLCNode    *dlQosStrategyLCNode_p = PNULL;
    DLLogicalChannelInfo   *lcInfo_p              = PNULL;
    LP_SpsDlInfo           spsDlInfo_p            = PNULL;

    spsDlInfo_p = &ueDLContext_p->spsDlInfo;
    
    /*! \code
     * Allocate node for CCE allocations for PDCCH
     * \endcode 
     * */
    dlStrNode_p = (DLStrategyTxNode *)getMemFromPool(
                                          sizeof(DLStrategyTxNode), PNULL );
    if( PNULL == dlStrNode_p )
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(DLStrategyTxNode), 
                internalCellIndex, 0, 0, 0,0, __FILE__, __func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
	 /*KLOCKWORK_CHANGES_START_38*/
        return PNULL;
     /*KLOCKWORK_CHANGES_STOP_38*/
    }

    /*! \code
     * Fill node for CCE allocations for PDCCH 
     * \endcode
     * */
    dlStrNode_p->ueIndex = ueDLContext_p->ueIndex;

    /*initialising the resource allocator input parameters*/
    resAllocInput_p = &(dlStrNode_p->resourceAllocInput);
    resAllocInput_p->subBandReportInfo_p = PNULL;
    /* + SPS_TDD_Changes */
    resAllocInput_p->requiredRB = ueDLContext_p->spsDlInfo.resAllocInfo.allocatedRB;
    /* - SPS_TDD_Changes */
        /*--coverity--530--54178*/
    resAllocInput_p->resourceRegionAllocated = CC_CE_REGION;
        /*--coverity--530--54178*/

    if (ueDLContext_p->dlCQIInfo.isSubBandReportAvailable)    
    {
        resAllocInput_p->subBandReportInfo_p =
            ueDLContext_p->dlCQIInfo.subBandReportInfo_p;
    }
    resAllocInput_p->strictAllocSubBand = FALSE;
    
    dlStrNode_p->msgType = msgType;
    
    /*DCI Format Calculation*/
    dlStrNode_p->primaryDCIFormat = DCI_FORMAT_1A;
    dlStrNode_p->secondaryDCIFormat = DCI_FORMAT_1A;
    
    dlStrNode_p->transmissionType = NEW_TX;
    dlStrNode_p->sumLCPriority = FALSE;
    dlStrNode_p->weightedQueueLoad = FALSE;
    dlStrNode_p->harqProcessId = harqProcess_p->harqProcessId ;
    dlStrNode_p->isCQIRequestedULGrantMsg = FALSE;
    dlStrNode_p->numPushedInAdvanceQueue = ZERO;
    dlStrNode_p->allocPushCount       = 0;
    dlStrNode_p->harqProcess_p = harqProcess_p;
    dlStrNode_p->strictAllocatedFlag = LTE_FALSE;
    dlStrNode_p->minTBSize = 0;
    dlStrNode_p->maxTBSize = 0;

    /*Setting only TBOne as valid and other INVALID*/
    harqProcess_p->isTBOneValid = TRUE;
    harqProcess_p->isTBTwoValid = FALSE;

    harqProcess_p->dlHARQTBOneInfo.txIndicator = NEW_TX;
    harqProcess_p->dlHARQTBTwoInfo.txIndicator = INVALID_TX;

    harqProcess_p->ratPolicyUsed = RA_POLICY_2_LOCALIZED;
    harqProcess_p->dlHARQTBOneInfo.mcsIndex  =  
            spsDlInfo_p->resAllocInfo.allocatedMCS;
    harqProcess_p->assignedResourcesNB = 
            spsDlInfo_p->resAllocInfo.allocatedRB; 
    ueDLContext_p->maxTBSize = ueDLContext_p->maxSISOTBSize;

    if( DL_QOS_BASED_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
            dlSchdConfig.dlSchedulerStrategy &&
        SPS_DEACTIVATION != msgType )
    {
        lcInfo_p = &ueDLContext_p->logicalChannel[spsDlInfo_p->lcId];
        /** klockwork Changes Start **/

        dlQosStrategyLCNode_p = 
            lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p;
        if( PNULL != dlQosStrategyLCNode_p )
        {
            dlQosStrategyLCNode_p->schedulableBytes = 
                spsDlInfo_p->spsOpp;
            dlQosStrategyLCNode_p->ueIndex = 
                ueDLContext_p->ueIndex;
            dlQosStrategyLCNode_p->lcId =
                lcInfo_p->dlLcQosInfo.dlQosStrategyLCNode_p->lcId;

            /* Insert the LC Node in the scheduled LC list */
            listInsertNode(&(ueDLContext_p->dlScheduledLcList),
                    &(dlQosStrategyLCNode_p->scheduledLcAnchor));

            (ueDLContext_p->scheduledLcCount)++;
       
            /* State change for QOS to DL_QOS_ALREADY_TRANSMITED */
            ueDLContext_p->scheduleFlag = DL_QOS_ALREADY_TRANSMITED;
        }
        /** klockwork Changes End **/
    }
    return dlStrNode_p;
}

/*******************************************************************************
 * Function Name  : processDLSpsSdl
 * Input          : ttiContext_p,
 *                  ttiContextPeriodicity20_p,
 *                  spsDlInfo_p - SPS DL Info of an UE
 * Outputs        : None
 * Returns        : None
 * Description    : This function deletes UE from occasion list and 
 *                  updates the TTI Context Map.
 *******************************************************************************/
/* CA Stats Changes Start */
void dlDeleteUeFromOccasionList( 
        LP_SpsStrIntervalTTIContext ttiContext_p,
        LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p,
        /* +- SPR 17777 */
        LP_SpsDlInfo       spsDlInfo_p
#ifdef SSI_DEBUG_STATS
        ,InternalCellIndex   internalCellIndex
#endif
        )
    /* +- SPR 17777 */
{
    /* CA Stats Changes End */
    LP_MacSpsOccList occNode_p = PNULL; 
    UInt8            semiPersSchedIntervalDL   = 0;
    
    semiPersSchedIntervalDL = 
        spsDlInfo_p->spsDlSetupInfo_p->semiPersistentSchedIntervalDL;
   
    occNode_p = (LP_MacSpsOccList)spsDlInfo_p->sdlOccNode_p; 

    /* Remove the entry from the SPS occasion List */
    DELETE_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsOccasionList,
                                   occNode_p );
    /* Decrement numDlSpsOccasionPerTti value */
    (ttiContext_p->numDlSpsOccasionPerTti)--;
    
    /* Free the memory for occasion node */
    freeMemPool( occNode_p );
    occNode_p = PNULL;

    /* Remove entry from next occasion also from TTI Map if 20 ms periodicity 
     * is configured */
    if( SPS_INTERVAL_20 == spsIntervalMap_g[semiPersSchedIntervalDL])
    {
        occNode_p = (LP_MacSpsOccList)spsDlInfo_p->sdlOccNode20_p; 
        DELETE_MAC_SPS_UE_LIST_PER_TTI(
               ttiContextPeriodicity20_p->spsOccasionList, occNode_p);
        /* Decrement numDlSpsOccasionPerTti value */
        (ttiContextPeriodicity20_p->numDlSpsOccasionPerTti)--;

        /* Free the memory for occasion node */
        freeMemPool( occNode_p );
    }

    /* Set Occasion node pointer in UE context as NULL */
    spsDlInfo_p->sdlOccNode_p = PNULL;
    spsDlInfo_p->sdlOccNode20_p = PNULL;

    /* Reset numOfSpsEmptyTx in UE context */
    spsDlInfo_p->numOfSpsEmptyTx  = 0;
   
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
            ttiContextPeriodicity20_p, getCurrentTick(), 
            semiPersSchedIntervalDL, internalCellIndex);
    /* CA Stats Changes End */

}

/*******************************************************************************
 * Function Name  : dlAddUeInOccasionList
 * Input          : ueIndex - Index of UE,
 *                  ttiContext_p,
 *                  ttiContextPeriodicity20_p,
 *                  spsDlInfo_p - SPS DL Info of an UE
 * Outputs        : None
 * Returns        : None
 * Description    : This function adds UE in occasion list and updates 
 *                  the TTI Context Map.
 *******************************************************************************/
/* CA Stats Changes Start */
void dlAddUeInOccasionList(  UInt16 ueIndex,
        LP_SpsStrIntervalTTIContext ttiContext_p,
        LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p,
        LP_SpsDlInfo spsDlInfo_p,
        InternalCellIndex   internalCellIndex)
{
    /* CA Stats Changes End */
    LP_MacSpsOccList occNode_p = PNULL;
    LP_MacSpsOccList tempoccNode_p = PNULL;
    UInt16 semiPersSchedIntervalDL   = 0;
        
    /* Allocate node for PDSCH Occasion List entry */
    occNode_p = (LP_MacSpsOccList)getMemFromPool( 
                sizeof(MacSpsOccList), PNULL );
    if( PNULL == occNode_p )
    {
        /* +- SPR 17777 */
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacSpsOccList),internalCellIndex,0,
                0, 0,0, __FILE__, __func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                 CRITICAL_ALARM );
        ltePanic("getMemFromPool fails %s", __func__);
	    /*KLOCKWORK_CHANGES_START_40*/
	    return;
	    /*KLOCKWORK_CHANGES_STOP_40*/
    }
    /* Fill the occasion list */
    occNode_p->ueIndex = ueIndex;
    
    semiPersSchedIntervalDL = 
        spsDlInfo_p->spsDlSetupInfo_p->semiPersistentSchedIntervalDL;

    if( SPS_INTERVAL_20 == spsIntervalMap_g[semiPersSchedIntervalDL] )
    {
        /* Allocate node for PDSCH Occasion List entry */
        tempoccNode_p = (LP_MacSpsOccList)getMemFromPool( 
                sizeof(MacSpsOccList), PNULL );
        if( PNULL == tempoccNode_p )
        {
            LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                    getCurrentTick(), __LINE__, sizeof(MacSpsOccList), 0,0,
                    0, 0,0, __FILE__, __func__);
            ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM );
            freeMemPool(occNode_p);
            ltePanic("getMemFromPool fails %s", __func__);
            return;
        }
    }
    
    ENQUEUE_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsOccasionList, occNode_p);
    
    /* Increment number of SPS occasions in the TTI */
    (ttiContext_p->numDlSpsOccasionPerTti)++; 

    /* Set Occasion node pointer in UE context as the node inserted */
    spsDlInfo_p->sdlOccNode_p = occNode_p;

    /* Put entry in occasion list for next occasion for 20 ms periodicity */
    /*klockworks warning fix*/
    if(tempoccNode_p != PNULL && SPS_INTERVAL_20 == spsIntervalMap_g[semiPersSchedIntervalDL] )
    {
        /* Fill the occasion list */
        tempoccNode_p->ueIndex = ueIndex;

        ENQUEUE_MAC_SPS_UE_LIST_PER_TTI(
                ttiContextPeriodicity20_p->spsOccasionList, 
                tempoccNode_p ); 
        /* Increment number of SPS occasions in the TTI */
        (ttiContextPeriodicity20_p->numDlSpsOccasionPerTti)++;
 
        /* Set Occasion node pointer in UE context as the node inserted */
        spsDlInfo_p->sdlOccNode20_p = tempoccNode_p;
    }
    
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
            ttiContextPeriodicity20_p, getCurrentTick(), 
            semiPersSchedIntervalDL, internalCellIndex);
    /* CA Stats Changes End */
}
	    /*KLOCKWORK_CHANGES_START_40*/

/************************************************************************************
 * Function Name  : dlDeleteUeFromSdl
 * Input          : ttiContext_p - tti Context for ttiIndex for Act/Deact/React,
 *                  ttiContextPeriodicity20_p - tti Context for next periodicity
 *                                              on case of 20 ms interval,
 *                  spsDlInfo_p - SPS DL Info of an UE
 * Outputs        : None
 * Returns        : None
 * Description    : This function deletes UE from SDL and update the TTI Context Map.
 ************************************************************************************/
/* CA Stats Changes Start */
void dlDeleteUeFromSdl( LP_SpsStrIntervalTTIContext ttiContext_p,
        LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p,
        /* +- SPR 17777 */
        LP_SpsDlInfo spsDlInfo_p
#ifdef SSI_DEBUG_STATS
        ,InternalCellIndex   internalCellIndex
#endif
        )
{
    /* CA Stats Changes End */
    LP_MacSpsDciListTTI sdlNode_p = PNULL;
    
    sdlNode_p = (LP_MacSpsDciListTTI)spsDlInfo_p->sdlOccNode_p;

    /* Delete the node from SPS DCI List */
    DELETE_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsDciList, sdlNode_p);
    /* Decrement number of SPS DCI in the TTI */
    (ttiContext_p->numDlSpsDciPerTti)--;
    
    /* Free the memory for SDL node */
    freeMemPool( sdlNode_p );
    sdlNode_p = PNULL;

    /* Check for next TTI index in if 20 ms periodicity in TTI Map */
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsDlInfo_p->spsDlSetupInfo_p->\
                                              semiPersistentSchedIntervalDL] )
    {
        sdlNode_p = (LP_MacSpsDciListTTI)spsDlInfo_p->sdlOccNode20_p;
        DELETE_MAC_SPS_UE_LIST_PER_TTI(
                    ttiContextPeriodicity20_p->spsDciList, sdlNode_p );        
        /* Decrement number of SPS DCI in the TTI */
        (ttiContextPeriodicity20_p->numDlSpsDciPerTti)--;

        /* Free the memory for SDL node */
        freeMemPool( sdlNode_p );
        
    }
    
    /* Set SDL pointer in UE context as NULL */
    spsDlInfo_p->sdlOccNode_p = PNULL;
    spsDlInfo_p->sdlOccNode20_p = PNULL;
    
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
            ttiContextPeriodicity20_p, getCurrentTick(), 
            spsDlInfo_p->spsDlSetupInfo_p->semiPersistentSchedIntervalDL,
            internalCellIndex);
    /* CA Stats Changes End */
}

/*******************************************************************************
 * Function Name  : dlAddUeInSdl
 * Input          : 
 * Outputs        : None
 * Returns        : None
 * Description    : This function adds UE in SDL and update the TTI Context Map.
 *******************************************************************************/

/* CA Stats Changes Start */
void dlAddUeInSdl( UInt16 ueIndex,
        LP_SpsStrIntervalTTIContext ttiContext_p,
        LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p,
        LP_SpsDlInfo        spsDlInfo_p,
        InternalCellIndex   internalCellIndex)
{
    /* CA Stats Changes End */
    LP_MacSpsDciListTTI sdlNode_p = PNULL;
    LP_MacSpsDciListTTI tempsdlNode_p = PNULL;
 
    /* Allocate node for SDL List entry */
    sdlNode_p = (LP_MacSpsDciListTTI)getMemFromPool( 
                         sizeof(MacSpsDciListTTI), PNULL );
    if( PNULL == sdlNode_p )
    {
        /* +- SPR 17777 */
       LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
               getCurrentTick(), __LINE__, sizeof(MacSpsDciListTTI),internalCellIndex,0,
               0, 0,0, __FILE__, __func__);
       ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s", __func__);
    /*KLOCKWORK_CHANGES_START_41*/
        return;
    /*KLOCKWORK_CHANGES_STOP_41*/
    }
    /*KLOCKWORK_CHANGES_START_41*/
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsDlInfo_p->spsDlSetupInfo_p->\
                semiPersistentSchedIntervalDL] )
    {
        /* Allocate node for SDL List entry */
        tempsdlNode_p = (LP_MacSpsDciListTTI)getMemFromPool( 
                sizeof(MacSpsDciListTTI), PNULL );
        if( PNULL == tempsdlNode_p )
        {
            LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                    getCurrentTick(), __LINE__, sizeof(MacSpsDciListTTI), 0,0,
                    0, 0,0, __FILE__, __func__);
            ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            freeMemPool(sdlNode_p);
            ltePanic("getMemFromPool fails %s", __func__);
            return;
        }
    }
    /*KLOCKWORK_CHANGES_STOP_41*/
    sdlNode_p->ueIndex = ueIndex;
    /* Enqueue in SDL */
    ENQUEUE_MAC_SPS_UE_LIST_PER_TTI( ttiContext_p->spsDciList, sdlNode_p);

    /* Increment number of SPS DCI in the TTI */
    (ttiContext_p->numDlSpsDciPerTti)++;

    /* Set SDL node pointer in UE context as the node inserted */
    spsDlInfo_p->sdlOccNode_p = sdlNode_p;

    /* Put entry in SDL for next occasion for 20 ms periodicity */
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsDlInfo_p->spsDlSetupInfo_p->\
                semiPersistentSchedIntervalDL] )
    {
        tempsdlNode_p->ueIndex = ueIndex;
        /* Enqueue in SDL */
        ENQUEUE_MAC_SPS_UE_LIST_PER_TTI(
                    ttiContextPeriodicity20_p->spsDciList, tempsdlNode_p );
        /* Increment number of SPS DCI in the TTI */
        (ttiContextPeriodicity20_p->numDlSpsDciPerTti)++ ;

        /* Set SDL node pointer in UE context as the node inserted */
        spsDlInfo_p->sdlOccNode20_p = tempsdlNode_p;
    }
    /* CA Stats Changes Start */
    /* Update Stats */
    LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiContext_p, 
            ttiContextPeriodicity20_p, getCurrentTick(), 
            spsDlInfo_p->spsDlSetupInfo_p->semiPersistentSchedIntervalDL, 
            internalCellIndex);
    /* CA Stats Changes End */
}

/*******************************************************************************
 * Function Name  : dlSpsPendingDeleteHandling
 * Input          : dlUeCtx_p - Pointer to DLUEContext
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles pending delete UE request and clears 
 *                  the TTI context stored by SPS scheduler. This API clears 
 *                  resources reserved by SPS PDSCH, SPS occasion list, SDL.
 *                  Release SPS-RNTI and also releases DL Setup Info pointer.
 *******************************************************************************/
void dlSpsPendingDeleteHandling( DLUEContext *dlUeCtx_p,
        InternalCellIndex internalCellIndex)
{
    LP_SpsDlInfo                spsDlInfo_p  = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p  = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;
    UInt32 qCount = 0;

    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;

    
    /*! \code
     * if state == SPS_ACTIVATED
     *    Delete UE node from SPS Occasion list
     * else if state == SPS_ACTIVATION_INITIATED or SPS_DEACTIVATION_INITIATED
     *    Delete UE node from SPS DCI List (SDL)
     * \endcode
     * */
    if( SPS_ACTIVATED == spsDlInfo_p->spsState )
    {
        ttiContext_p = &(spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->ttiIndex]);
        ttiContextPeriodicity20_p = &(spsStrTTIMap_g[internalCellIndex][ (spsDlInfo_p->ttiIndex +
                                        SPS_INTERVAL_20) % MAX_SPS_INTERVAL]);
        qCount =  QCOUNT_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsOccasionList);
        if ( qCount != 0)
        {
            /* CA Stats Changes Start */
            /* +- SPR 17777 */
            dlDeleteUeFromOccasionList( ttiContext_p, ttiContextPeriodicity20_p,
            spsDlInfo_p
#ifdef SSI_DEBUG_STATS
            ,internalCellIndex
#endif
            );
            /* CA Stats Changes End */
        }
    }
    else if( SPS_ACTIVATION_INITIATED == spsDlInfo_p->spsState ) 
    {
        ttiContext_p = &(spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->ttiIndex]);
        ttiContextPeriodicity20_p = &(spsStrTTIMap_g[internalCellIndex][ (spsDlInfo_p->ttiIndex +
                                        SPS_INTERVAL_20) % MAX_SPS_INTERVAL]);
        qCount =  QCOUNT_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsDciList);
        if ( qCount != 0)
        {
            /* CA Stats Changes Start */
            /* +- SPR 17777 */
            dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p, 
            spsDlInfo_p
#ifdef SSI_DEBUG_STATS
            ,internalCellIndex
#endif
            );
            /* CA Stats Changes End */
        }
    }
    else if( SPS_DEACTIVATION_INITIATED == spsDlInfo_p->spsState )
    {
        ttiContext_p = &(spsStrTTIMap_g[internalCellIndex][spsDlInfo_p->deactTtiIndex]);
        ttiContextPeriodicity20_p = &(spsStrTTIMap_g[internalCellIndex][ 
                                        (spsDlInfo_p->deactTtiIndex +
                                        SPS_INTERVAL_20) % MAX_SPS_INTERVAL]);
        qCount =  QCOUNT_MAC_SPS_UE_LIST_PER_TTI(ttiContext_p->spsDciList);
        if ( qCount != 0)
        {
            /* CA Stats Changes Start */
            /* +- SPR 17777 */
            dlDeleteUeFromSdl( ttiContext_p, ttiContextPeriodicity20_p, 
            spsDlInfo_p
#ifdef SSI_DEBUG_STATS
            ,internalCellIndex
#endif
            );
            /* CA Stats Changes End */
        }
    }

    /*! \code
     * Clears resources reserved by SPS PDSCH if allocatedRB is not 0 
     * \endcode
     * */
    if( 0 != spsDlInfo_p->resAllocInfo.allocatedRB )
    {   
        processDLResourceReleaseRequest( dlUeCtx_p ,internalCellIndex);
    }

    /*! \code
     * Release SPS CRNTI 
     * \endcode
     * */
    if (INVALID_SPS_CRNTI != spsDlInfo_p->SpsCrnti)
    {
        deAllocateSpsCrnti(spsDlInfo_p->SpsCrnti,internalCellIndex);
    }
    
    /* Set SPS state to INVALID_STATE  */
    spsDlInfo_p->spsState = INVALID_STATE;

    /* Set Sps Crnti as INVALID_SPS_CRNTI */
    spsDlInfo_p->SpsCrnti = INVALID_SPS_CRNTI;
   
    /*! \code
     * Releases DL Setup Info pointer
     * \endcode
     * */
    if( PNULL != spsDlInfo_p->spsDlSetupInfo_p )
    {
        freeMemPool( spsDlInfo_p->spsDlSetupInfo_p );
        spsDlInfo_p->spsDlSetupInfo_p = PNULL;
    }
}  

/*******************************************************************************
 * Function Name  : spsSort
 * Input          : sortedArray - Array to be sorted,
 *                  startingIndex - starting index in array,
 *                  lastIndex - last index in array
 * Outputs        : None
 * Returns        : None
 * Description    : This function sorts the array in ascending order using
 *                  insertion sort.
 *******************************************************************************/
void spsSort( SpsStrIntervalTTISorted sortedArray[],
        UInt8 startingIndex, UInt8 lastIndex,
        InternalCellIndex internalCellIndex)
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
            spsStrTTIMap_g[internalCellIndex][sortedArray[jter+1].index].\
                indexInSortedArray = jter+1;
            jter--;
        }
        sortedArray[jter+1].numSpsUeinRm = currentElement;
        sortedArray[jter+1].index = idx;
        /* Update position in actual array */
        spsStrTTIMap_g[internalCellIndex][sortedArray[jter+1].index].\
            indexInSortedArray = jter+1;
    }
}
/* + SPS_TDD_Changes */
/*******************************************************************************
 * Function Name  : processSPSDLScheduling 
 * Inputs         : currentGlobalTTITickCount
 * 		          : delayToPhy,
 * Outputs        : None
 * Returns        : None
 * Description    : This Function Process the SPS DL scheduling.
 *******************************************************************************/
void processSPSDLScheduling(UInt32 currentGlobalTTITickCount,
			UInt32 delayToPhy,
			InternalCellIndex internalCellIndex)	 
{

    UInt32 qCount = 0;
        /*SPR 20899 Fix Start*/
    UInt8 strategyContainerIndex = MODULO_TWO(getCurrentTick()); 
        /*SPR 20899 Fix Stop*/

    /*! \code 
     * Process the SPS startegy queues in following order:
     * 1. DL Status Queue, dlSpsStatusQ_g.
     * 2. Deactivation Queue, dlSpsDeactQ_g
     * 3. Activation Queue, dlSpsActQ_g
     * 4. Re-activation Queue, dlSpsReactQ_g
     * 5. SPS Occassion List and SDL
     * \endcode
     * */

    /* Initialize the scheduled UE count in the DL QOS Strategy Schedule Info
     */
    dlStrategyScheduleInfo_g[strategyContainerIndex][internalCellIndex].schdeuledUeCount = 0;
    if (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps)
    {
	    qCount = QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex] );
	    if ( 0 != qCount )
	    {
		    macDlSpsProcessStatusQ( qCount, delayToPhy,internalCellIndex) ;
	    }
	    qCount = QCOUNT_MAC_DL_SPS_Q( dlSpsDeactQ_gp[internalCellIndex] );
	    if ( 0 != qCount )
	    {
		    macDlSpsProcessDeactQ( qCount, delayToPhy,internalCellIndex );
	    }
	    qCount = QCOUNT_MAC_DL_SPS_Q(dlSpsActQ_gp[internalCellIndex]  );
	    if ( 0 != qCount )
	    {
		    macDlSpsProcessActQ( qCount,delayToPhy,internalCellIndex  );
	    }
	    /* Process Occasion List and SDL */
	    processDLSpsOccasion( currentGlobalTTITickCount, 
			    delayToPhy,
			    internalCellIndex );
    }

}
/*******************************************************************************
 * Function Name  : processDummyDlSPSSchedulerStrategy 
 * Inputs         : currentGlobalTTITickCount
 * 		          : delayToPhy,
 * Outputs        : None
 * Returns        : None
 * Description    : This Function Process the dummy SPS scheduler for Special subframe.
 *******************************************************************************/
void processDummyDlSPSSchedulerStrategy(UInt32 currentGlobalTTITickCount,
                        UInt32 delayToPhy, InternalCellIndex internalCellIndex)
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(currentGlobalTTITickCount)
    LTE_GCC_UNUSED_PARAM(delayToPhy)
    LTE_GCC_UNUSED_PARAM(internalCellIndex)
    /*In case of SF. No scheduling of SPS will be done.*/
}

/* + SPR 20756 */
/*******************************************************************************
 * Function Name  : checkAndFreeSpsHarqProcess 
 * Inputs         : dlUeCtx_p - DL UE Context
 *                  intCellIdx - Internal Cell ID
 * Outputs        : None
 * Returns        : None
 * Description    : Checks for all the SPS HARQ configured and free the same 
 *                  if in use
 *******************************************************************************/
void checkAndFreeSpsHarqProcess( DLUEContext *dlUeCtx_p, 
        InternalCellIndex intCellIdx )
{
    LP_SpsDlInfo spsDlInfo_p = PNULL;
    LP_SpsDlSetupInfo spsDlSetupInfo_p = PNULL;
    DLHARQEntity *dlHarqEntity_p = PNULL;
    DLHARQProcess* dlHarqProcess_p = PNULL;
    UInt8 hpId = 0;
    
    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
    spsDlSetupInfo_p = spsDlInfo_p->spsDlSetupInfo_p;

    if( PNULL != spsDlSetupInfo_p )
    {
        /* Check for each of the configured SPS HARQ Processes. More than one
         * harq process can be in use at a time because of re-transmissions 
         * ongoing for previous occasion and new transmission for next 
         * occasion */
        for( hpId = 0; hpId < spsDlSetupInfo_p->numberOfConfSpsProcesses; 
                hpId++ )
        {
            dlHarqEntity_p = dlUeCtx_p->dlHarqContext_p;
            if( PNULL == dlHarqEntity_p )
            {
                continue;
            }
            dlHarqProcess_p = &dlHarqEntity_p->harqProcess[ hpId ];
            if( ( PNULL != dlHarqProcess_p ) && 
                    ( HARQ_PROCESS_FREE != dlHarqProcess_p->harqStatus ) )
            {
                /* free the HARQ process if in use */
                freeDLHarqProcessTBSpecific( dlUeCtx_p, dlHarqProcess_p,
                        TB_ONE, intCellIdx );
            }
        }
    }
}
/* - SPR 20756 */
