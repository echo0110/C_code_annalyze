/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id:lteMacDLScellStateManager.c, 2014/02/24 16:05:42 gur29743 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This File is used to apply the DL Scell State Machine 
                       for all the UEs which needs to be schedule.
 *                     All event handlers and trigger function have defined in this file.  
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacDLScellStateManager.c,v $
 * Revision 1.0  2014/02/24 16:05:42 gur29743
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteTypes.h"
#include "lteMacCellConfMgr.h"
#include "lteMacDLScellStateManager.h"
#include "lteMacUtilityAndSharedStructs.h"
#include "lteMacDLQosTokenAlgo.h"
#include "lteCommonStatsManager.h"
#include "lteMacDRXMgr.h" /* DRX_CHG */
#include "lteMacMeasGapHandler.h" /* MEAS_GAP_CHG */
#include "lteCommonStatsManager.h"
#include "lteMacDLResourceInfoMgr.h"
#include "lteMacDLSpsScheduler.h"
#include "lteMacDLHarqMgr.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#include "lteMacTddMgr.h"
#endif
#include "ltePerfStats.h"
/*SPR 21948 Changes Start */
/* Changes under spr 21762 have been reverted */
/*SPR 21948 Changes End */


/******************************************************************************
 Private Definitions
 *****************************************************************************/

/******************************************************************************
 Private Types
 *****************************************************************************/
 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/
/*ca event queue to process Scell state machine.*/
DL_CA_EVENT_QUEUE_TYPE * dlCAEventQueue_gp[MAX_NUM_CELL];
/*ca MAC CE queue */
DL_CA_MAC_CE_QUEUE_TYPE * dlCAMacCEQueue_gp[MAX_NUM_CELL];
/*pending ca Mac ce queue*/
DL_CA_MAC_CE_QUEUE_TYPE * pendingDlCAMacCEQueue_gp[MAX_NUM_CELL];
/*pending failure ca Mac ce queue*/
DL_CA_MAC_CE_QUEUE_TYPE* pendingFailureDlCAMacCEQueue_gp[MAX_NUM_CELL];
/*ca phase-3 changes start*/
/*free scell harq buffer Q to procees the deactivation of scell*/
DL_CA_MAC_FREE_SCELL_HARQ_QUEUE_TYPE* dlCAMacFreeScellHarqBufferQueue_gp[MAX_NUM_CELL];
/*ca phase-3 changes end*/
/*state machine function ptr */
DlCAScellStateFunctionPtr dlCAScellStateEventArr_g[MAX_NUM_DL_CA_SCELL_STATE][MAX_NUM_DL_CA_EVENT];
/*global array of structure for Ue Qload info to decide activation/deactivation*/
DLCAScellUeQloadCheckList dlCAScellUeQloadCheckList_g[MAX_NUM_CELL];
/*ca chngs start*/
/* Stage-III SCELL Deactivation Timer Related Changes Start */
scellDeactExpiryTimerList* scellDeactExpiryTimerList_gp[MAX_NUM_CELL] = {PNULL};
/* Stage-III SCELL Deactivation Timer Related Changes End */


STATIC  void updateDlAggregateGbrOnScellStateChange( DLUEContext* ueDLContext_p,
                                                           UInt8 actDeactTrigger);
extern UInt32 dlAggregateGbrPerTTI_g[MAX_NUM_SCELL +1][MAX_VALID_UE_TYPE];/* EICIC +-*/
/*CYCLOMATIC_CHANGES_6.1_START*/
STATIC  UInt8 calculateFrameNumberForCATimerExpiry(UInt8 frameNumber);

STATIC  MacRetType enqueueInDLCAEventQueue(DLCATimerNode* dlCATimerNode_p,
                                           DLUEContext* dlUeContext_p
                                           );
/*CYCLOMATIC_CHANGES_6.1_END*/

/* EICIC +*/
extern UInt8 	ueTypeToScheduleDL_g[MAX_NUM_CELL] ;
extern UInt8 	usableAbsCountNew_g[MAX_NUM_CELL] ;
extern UInt8	absCountNew_g[MAX_NUM_CELL] ;
/* EICIC -*/
/*****************************************************************************
 * Function Name  : processScellRelToStateInit 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_SCELL_RELEASE 
 *                  for event handling of Scell states DL_CA_SCELL_DEACTIVATED,
 *                  DL_CA_SCELL_CONFIGURED, DL_CA_SCELL_ACTIVATION_ON_GOING &
 *                  DL_CA_SCELL_ACTIVE
 *****************************************************************************/
void processScellRelToStateInit(DLCAEventQueueNode * eventQNode_p)
{
    /* Stage-III SCELL Deactivation Timer Related Changes Start */ 
    ScellDeactTimerNode   *scellTimerNode_p = PNULL; 
    /* Stage-III SCELL Deactivation Timer Related Changes End */ 
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_INIT;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */
    /* Stage-III SCELL Deactivation Timer Related Changes Start */ 
    /*Will free memory in SCELL_RELEASE event only */ 
    scellTimerNode_p = \
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellStateVariables.scellTimerNode_p;
    
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellStateVariables.\
    scellCycleTimerExpiryIndex = INVALID_SCELL_DEACT_EXPIRY_INDEX; 
    
    if(scellTimerNode_p)
    {
        scellTimerNode_p->expiryTTI        = INVALID_SCELL_DEACT_EXPIRY_TTI;
        scellTimerNode_p->sCellArrayIndex  = INVALID_SERV_CELL_INDEX;
        FREE_MEM_SCELL_DEACT_TIMER_NODE(scellTimerNode_p);
        ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellStateVariables.scellTimerNode_p = PNULL;
    }
    /* Stage-III SCELL Deactivation Timer Related Changes End */ 

}
/*****************************************************************************
 * Function Name  : processScellActivInDeactivation 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_RRM_SCELL_ACTIVATION
 *                  for event handling of Scell state DL_CA_SCELL_DEACTIVATED.
 *****************************************************************************/
void processScellActivInDeactivation(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_CONFIGURED;

}
/*****************************************************************************
 * Function Name  : processUeCreateInInit 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_RRC_UE_CREATE  
 *                  for event handling of Scell state DL_CA_SCELL_INIT.
 *****************************************************************************/
void processUeCreateInInit(DLCAEventQueueNode * eventQNode_p)
{
    /* Stage-III SCELL Deactivation Timer Related Changes Start */ 
    ScellDeactTimerNode   *scellTimerNode_p = PNULL; 
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    /*Will free memory in SCELL_RELEASE event only */ 
    scellTimerNode_p = \
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellStateVariables.scellTimerNode_p;
    if(PNULL == scellTimerNode_p)
    {
        /*If no timernode memory allocated, allocate memory for the timer node*/
        /*Memory shall be allocated for the timer node when the SCELL in INIT state receives 
         * event DL_CA_EVENT_RRC_UE_CREATE*/
        scellStateMngrGetmemScellTimerNode(ueDLContext_p, eventQNode_p->scellId/*sCellArrIndex*/);
    }
    /* Stage-III SCELL Deactivation Timer Related Changes End */ 
}
/*****************************************************************************
 * Function Name  : processRrmDeactivInInit
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_SCELL_CONFIGURED_RRM_DEACTIVATION
 *                  for event handling of Scell state DL_CA_SCELL_INIT.
 *****************************************************************************/
void processRrmDeactivInInit(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_DEACTIVATED;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */

}

/*****************************************************************************
 * Function Name  : processRrmActivInInit 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_SCELL_CONFIGURED_RRM_ACTIVATION  
 *                  for event handling of Scell state DL_CA_SCELL_INIT.
 *****************************************************************************/
void processRrmActivInInit(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_CONFIGURED;    
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */
}
/*****************************************************************************
 * Function Name  : processRrmDeactivInConfigured 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_RRM_SCELL_DEACTIVATION  
 *                  for event handling of Scell state DL_CA_SCELL_CONFIGURED.
 *****************************************************************************/
void processRrmDeactivInConfigured(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_DEACTIVATED;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */

}
/*****************************************************************************
 * Function Name  : processRrmActivInConfigured 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_RRM_SCELL_ACTIVATION  
 *                  for event handling of Scell state DL_CA_SCELL_CONFIGURED.
 *****************************************************************************/
void processRrmActivInConfigured(DLCAEventQueueNode * eventQNode_p)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(eventQNode_p)
    /* +- SPR 17777 */
}
/*****************************************************************************
 * Function Name  : processMacScellActivInConfigured 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_MAC_SCELL_ACT_CE_SENT  
 *                  for event handling of Scell state DL_CA_SCELL_CONFIGURED.
 *****************************************************************************/
void processMacScellActivInConfigured(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_ACTIVATION_ON_GOING;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */
}
/*SPR 17561 FIX*/
#ifdef FDD_CONFIG
/*SPR 16026 chnages start*/
/*****************************************************************************
 * Function Name  : processActHarqFailureInActivationOnGoing 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_ACT_MAC_CE_HARQ_FAILURE  
 *                  for event handling of Scell state DL_CA_SCELL_ACTIVATION_ON_GOING.
 *****************************************************************************/
void processActHarqFailureInActivationOnGoing(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_CONFIGURED;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */
}
/*SPR 16026 chnages end*/
#endif
/*SPR 17561 FIX*/
/*****************************************************************************
 * Function Name  : processReconfigApiIndicationInConfigured 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_MAC_SCELL_ACT_CE_TRIGGER  
 *                  for event handling of Scell state DL_CA_SCELL_CONFIGURED.
 *****************************************************************************/
void processReconfigApiIndicationInConfigured(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    DLCAMacCEQueueNode *dlCAMacCEQueueNode_p = PNULL;
    /*prepare and send mac CE*/
    GET_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueueNode_p,DLCAMacCEQueueNode);
    if (dlCAMacCEQueueNode_p != PNULL)
    {
        dlCAMacCEQueueNode_p->ueId = eventQNode_p->ueId;
        dlCAMacCEQueueNode_p->scellId = eventQNode_p->scellId;
        dlCAMacCEQueueNode_p->dlCAQosEvent = DL_CA_EVENT_MAC_SCELL_ACT_CE_TRIGGER;
        /* + SPR 22352 fix */
        SET_SCELL_ACT_DEACT_BIT(dlCAMacCEQueueNode_p->ueId,dlCAMacCEQueueNode_p->scellId,dlCAMacCEQueueNode_p->dlCAQosEvent);
        /* - SPR 22352 fix */
 
        if(!ENQUEUE_DL_CA_MAC_CE_Q(dlCAMacCEQueue_gp[ueDLContext_p->internalCellIndex],(void *)dlCAMacCEQueueNode_p))
        {
            FREE_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueueNode_p);
        }
    }

}
/*****************************************************************************
 * Function Name  : processRrmActivInActivationOnGoing 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_RRM_SCELL_ACTIVATION  
 *                  for event handling of Scell state DL_CA_SCELL_ACTIVATION_ON_GOING.
 *****************************************************************************/
void processRrmActivInActivationOnGoing(DLCAEventQueueNode * eventQNode_p)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(eventQNode_p)
    /* +- SPR 17777 */
}
/*****************************************************************************
 * Function Name  : processRrmActivInActivation 
 * Inputs         : eventQNode_p - DLCAEventQueueNode  
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_RRM_SCELL_ACTIVATION  
 *                  for event handling of Scell state DL_CA_SCELL_ACTIVE.
 *****************************************************************************/
void processRrmActivInActivation(DLCAEventQueueNode * eventQNode_p)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(eventQNode_p)
    /* +- SPR 17777 */
}
/*****************************************************************************
 * Function Name  : processMacActTimerExpInActivationOnGoing 
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the Mac activation timer expiry
 *                  for event handling of Scell state DL_CA_SCELL_ACTIVATION_ON_GOING.
 *                  It shall also start the deactivation expiry timer.
 *****************************************************************************/
void processMacActTimerExpInActivationOnGoing(DLCAEventQueueNode * eventQNode_p)
{
    /*SPR 15909 fix start*/
    tickType_t      expiryTTI      = INVALID_SCELL_DEACT_EXPIRY_TTI;
    /*SPR 15909 fix end*/
    DLUEContext *ueDLContext_p = PNULL;
    DLCAEventQueueNode * tempEventQNode_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    DLUESCellContext *dlUeScellContext_p = 
        ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId];
    dlUeScellContext_p->scellState = DL_CA_SCELL_ACTIVE;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */
    /*Insert aperiodic CQI request node for scell in Q*/
    /* SPR 11634 Fix start */
    if ( (APERIODIC_CQI_REPORTING_MODE ==  dlUeScellContext_p->ScelldlCQIInfo.cqiTypeIndicator) ||
          (PERIODIC_APERIODIC_MODE == dlUeScellContext_p->ScelldlCQIInfo.cqiTypeIndicator))
    {
        putEntryInULAperiodicCQIQueue(eventQNode_p->ueId, ueDLContext_p->internalCellIndex
                ,servingCellBitMask_g[eventQNode_p->scellId]
                /* SPR 11634 Fix end */
                );
        dlUeScellContext_p->ScelldlCQIInfo.aperiodicCqiRequestTTI = getCurrentTick();
    }
    /*If currentTTI > isCQIReportValidTTI, then all cqiMap of SCell will be flushed (expect 
      lastReportedPeriodicRI, initial MCS) else cqiMap will be maintained and scheduling 
      will be done based on cqiMaps.*/
    /*SPR 16221 Fix Start*/
    /* SPR 16422 START */
    /* SPR 21998 Fix + */
    if((INVALID_CQI_REPORTING_MODE != dlUeScellContext_p->ScelldlCQIInfo.cqiTypeIndicator) &&
            (dlUeScellContext_p->cqiValidityTimer) &&
    /* SPR 21998 Fix - */
            (getCurrentTick() >= dlUeScellContext_p->ScelldlCQIInfo.isCQIReportValidTTI))
        /* SPR 16422 END */
    {
        resetScellCSIValuesOnCqiReportValidTimerExpiry(ueDLContext_p, eventQNode_p->scellId);
    }
    /*SPR 16221 Fix Start*/
    /*phase-3 code changes*/
    if(dlUeScellContext_p->ActDeactBitMask == SCELL_DEACT_BIT_SET &&\
         ueDLContext_p->isMACCESendToUE == LTE_TRUE)
    {        
        /*prepare and trigger an Event*/
        GET_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p,DLCAEventQueueNode);
        if(tempEventQNode_p != PNULL)
        { 
            tempEventQNode_p->ueId          = eventQNode_p->ueId;
            tempEventQNode_p->scellId       = eventQNode_p->scellId;
            tempEventQNode_p->dlCAQosEvent  = DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER;

            if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueDLContext_p->internalCellIndex],(void *)tempEventQNode_p))
            {
                FREE_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p);
            }
            /*reset the bitmask*/ 
            dlUeScellContext_p->ActDeactBitMask = SCELL_ACT_DEACT_BIT_RESET;
       }
    }
    /*phase-3 code changes*/
    /* Stage-III SCELL Deactivation Timer Related Changes Start */ 
    /*First time SCELL deactivation timer shall be started from here once we change the 
    scell state to ACTIVE
    rest all other cases shall be updated from processEncoderQueue, processRlcEncoderQueuePerUE
    and processMACEncoderQueue function
    Cases:- PDCCH on the activated SCell indicates an uplink grant or downlink assignment*/
     /*scellDeactivationTimer in ueDLContext_p is in radio frame.
     * For subframe level it has to be multiplied by 10 MAX_SUBFRAME*/
    if(INVALID_SCELL_DEACT_TIMER != ueDLContext_p->scellDeactivationTimer)
    {
        expiryTTI  = getCurrentTick() + (ueDLContext_p->scellDeactivationTimer * MAX_SUBFRAME) + SCELL_DEACT_NEXT_TICK + PHY_DL_DELAY;
        /* Moved to INIT State scellStateMngrGetmemScellTimerNode(ueDLContext_p, sCellArrIndex);*/
        scellStateMngrStartSCellDeactTimer(ueDLContext_p, expiryTTI, eventQNode_p->scellId, dlUeScellContext_p->internalCellIndex);
    }
    /* Stage-III SCELL Deactivation Timer Related Changes End */ 
}
/*SPR 17561 FIX*/
#ifdef FDD_CONFIG
/*SPR 16026 changes start*/ 
/*****************************************************************************
 * Function Name  : processHarqFailureInDeactivationOnGoingViaMac 
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the Mac Deactivation CE Harq Failure  
 *                  and revert back the scell state.
 *                  It shall also start the deactivation expiry timer.
 *****************************************************************************/
void processHarqFailureInDeactivationOnGoingViaMac(DLCAEventQueueNode * eventQNode_p)
{
    /*SPR 15909 fix start*/
    tickType_t      expiryTTI      = INVALID_SCELL_DEACT_EXPIRY_TTI;
    /*SPR 15909 fix end*/
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    DLUESCellContext *dlUeScellContext_p = 
        ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId];
    dlUeScellContext_p->scellState = DL_CA_SCELL_ACTIVE;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */
    /* Stage-III SCELL Deactivation Timer Related Changes Start */ 
    /*First time SCELL deactivation timer shall be started from here once we change the 
    scell state to ACTIVE
    rest all other cases shall be updated from processEncoderQueue, processRlcEncoderQueuePerUE
    and processMACEncoderQueue function
    Cases:- PDCCH on the activated SCell indicates an uplink grant or downlink assignment*/
     /*scellDeactivationTimer in ueDLContext_p is in radio frame.
     * For subframe level it has to be multiplied by 10 MAX_SUBFRAME*/
    if(INVALID_SCELL_DEACT_TIMER != ueDLContext_p->scellDeactivationTimer)
    {
        expiryTTI  = getCurrentTick() + (ueDLContext_p->scellDeactivationTimer * MAX_SUBFRAME) + SCELL_DEACT_NEXT_TICK + PHY_DL_DELAY;
        /* Moved to INIT State scellStateMngrGetmemScellTimerNode(ueDLContext_p, sCellArrIndex);*/
        scellStateMngrStartSCellDeactTimer(ueDLContext_p, expiryTTI, eventQNode_p->scellId, dlUeScellContext_p->internalCellIndex);
    }
    /* Stage-III SCELL Deactivation Timer Related Changes End */ 
} 
/*SPR 16026 changes end*/ 
#endif
/*SPR 17561 FIX*/
/*****************************************************************************
 * Function Name  : processScellDeactivInActivation 
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the Events
 *                  DL_CA_EVENT_RRM_SCELL_DEACTIVATION  And
 *                  DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER
 *                  in Scell state DL_CA_SCELL_ACTIVE.
 *****************************************************************************/
void processScellDeactivInActivation(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    DLCAMacCEQueueNode *dlCAMacCEQueueNode_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    
    if(ueDLContext_p->isMACCESendToUE == LTE_TRUE)
    {    
        /*prepare and send mac CE*/
        GET_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueueNode_p,DLCAMacCEQueueNode);
        if(dlCAMacCEQueueNode_p != PNULL)
        {
            dlCAMacCEQueueNode_p->ueId = eventQNode_p->ueId;
            dlCAMacCEQueueNode_p->scellId = eventQNode_p->scellId;
            dlCAMacCEQueueNode_p->dlCAQosEvent = DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER;

            /* + SPR 22352 fix */
            SET_SCELL_ACT_DEACT_BIT(dlCAMacCEQueueNode_p->ueId,dlCAMacCEQueueNode_p->scellId,dlCAMacCEQueueNode_p->dlCAQosEvent);
            /* - SPR 22352 fix */
            if(!ENQUEUE_DL_CA_MAC_CE_Q(dlCAMacCEQueue_gp[ueDLContext_p->internalCellIndex],(void *)dlCAMacCEQueueNode_p))
            {
                FREE_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueueNode_p);
            }
        }
    }
}
/*****************************************************************************
 * Function Name  : processRrmScellDeactivMacCESentInActivation 
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_RRM_SCELL_DEACTIVATION  
 *                  for event handling of Scell state DL_CA_SCELL_ACTIVE.
 *****************************************************************************/
void processRrmScellDeactivMacCESentInActivation(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    DLCAMacFreeScellHarqBufferQueueNode *dlCAMacFreeScellHarqBufferQueueNode_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_RRM;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */
    /*prepare and insert node to reset the harq buffer of scell deactivated*/
    GET_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p,DLCAMacFreeScellHarqBufferQueueNode);
    if(dlCAMacFreeScellHarqBufferQueueNode_p != PNULL)
    {
        dlCAMacFreeScellHarqBufferQueueNode_p->ueId = eventQNode_p->ueId;
        dlCAMacFreeScellHarqBufferQueueNode_p->scellId = eventQNode_p->scellId;

        if(!ENQUEUE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp[ueDLContext_p->internalCellIndex],\
                    (void *)dlCAMacFreeScellHarqBufferQueueNode_p))
        {
            FREE_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p);
        }
    }
}
/*****************************************************************************
 * Function Name  : processMacScellDeactivMacCESentInActivation 
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_MAC_DEACTIVATION_MAC_CE_SENT  
 *                  for event handling of Scell state DL_CA_SCELL_ACTIVE.
 *****************************************************************************/
void processMacScellDeactivMacCESentInActivation(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    DLCAMacFreeScellHarqBufferQueueNode *dlCAMacFreeScellHarqBufferQueueNode_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */
    /*prepare and insert node to reset the harq buffer of scell deactivated*/
    GET_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p,DLCAMacFreeScellHarqBufferQueueNode);
    if(dlCAMacFreeScellHarqBufferQueueNode_p != PNULL)
    {
        dlCAMacFreeScellHarqBufferQueueNode_p->ueId = eventQNode_p->ueId;
        dlCAMacFreeScellHarqBufferQueueNode_p->scellId = eventQNode_p->scellId;

        if(!ENQUEUE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp[ueDLContext_p->internalCellIndex],\
                    (void *)dlCAMacFreeScellHarqBufferQueueNode_p))
        {
            FREE_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p);
        }
    }
    
}
/*ca-tdd changes start*/
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : getSCellStateChangeTddTimerOffset 
 * Inputs         : cellIndex
 * Outputs        : None
 * Returns        : SCellStateChangeTddTimerOffset
 * Description    : This function Process tdd harqrtt max value for corresponding sub frame config.  
 *****************************************************************************/
UInt32 getSCellStateChangeTddTimerOffset(UInt8 cellIndex)
{
    /*cov-id 69711 fix*/
    TddSubFrameConfig subFrameAssign = (TddSubFrameConfig)cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->cellParams_p->subFrameAssign;
    /*check tdd harqrtt max value for corresponding sub frame config */
    if(subFrameAssign == SUB_FRAME_CONFIG_0)
    {
        return DL_HARQ_RTT_TIMER_6_PLUS_4;
    }
    else if(subFrameAssign == SUB_FRAME_CONFIG_1 || subFrameAssign == SUB_FRAME_CONFIG_6)
    {
        return DL_HARQ_RTT_TIMER_7_PLUS_4;
    }
    else if(subFrameAssign == SUB_FRAME_CONFIG_2)
    {
        return DL_HARQ_RTT_TIMER_8_PLUS_4;
    }
    else if(subFrameAssign == SUB_FRAME_CONFIG_3)
    {
        return DL_HARQ_RTT_TIMER_9_PLUS_4;
    }
    else if(subFrameAssign == SUB_FRAME_CONFIG_4)
    {
        return DL_HARQ_RTT_TIMER_12_PLUS_4;
    }
    else
    {
        return FALSE;
    }
}
#endif
/*ca-tdd changes end*/
/*****************************************************************************
 * Function Name  : processMacDeactTimerExpInActivation 
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_DEACTIVATION_TIMER_EXPIRE  
 *                  for event handling of Scell state DL_CA_SCELL_ACTIVE.
 *****************************************************************************/
void processMacDeactTimerExpInActivation(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    UInt8 scellDeactBitmask = 0;
    DLCAMacFreeScellHarqBufferQueueNode *dlCAMacFreeScellHarqBufferQueueNode_p = PNULL;    
 
    /*ca phase-3 changes start*/
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
#ifdef FDD_CONFIG
    /*SPR 15909 fix start*/
    tickType_t scheduledExpiryTick = getCurrentTick() + PHY_DL_DELAY + SCELL_STATE_CHANGE_TIMER_OFFSET/*8*/; 
    /*SPR 15909 fix end*/
    UInt8  harqSubFrameNum = (scheduledExpiryTick + FDD_HARQ_OFFSET )%MAX_SUB_FRAME;
#else
    UInt32 sCellStateChangeTddTimerOffset = 0; 
    /*calculate max rtt timer on the basis of tdd subframe config*/ 
    sCellStateChangeTddTimerOffset = getSCellStateChangeTddTimerOffset(ueDLContext_p->internalCellIndex);
    /*SPR 15909 fix start*/
    tickType_t scheduledExpiryTick = getCurrentTick() + PHY_DL_DELAY + sCellStateChangeTddTimerOffset; 
    /*SPR 15909 fix end*/
    UInt8  harqSubFrameNum = (scheduledExpiryTick)%MAX_SUB_FRAME;
#endif
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */

    /*prepare and insert node to reset the harq buffer of scell deactivated*/
    GET_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p,DLCAMacFreeScellHarqBufferQueueNode);
    if(dlCAMacFreeScellHarqBufferQueueNode_p != PNULL)
    {
        dlCAMacFreeScellHarqBufferQueueNode_p->ueId = eventQNode_p->ueId;
        dlCAMacFreeScellHarqBufferQueueNode_p->scellId = eventQNode_p->scellId;

        if(!ENQUEUE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp[ueDLContext_p->internalCellIndex],\
                    (void *)dlCAMacFreeScellHarqBufferQueueNode_p))
        {
            FREE_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p);
        }
    }

    SET_BYTE_FIELD( scellDeactBitmask, 
            1, 
            eventQNode_p->scellId,
            1);

    dlCATimerStart(ueDLContext_p->ueIndex,harqSubFrameNum,\
            scheduledExpiryTick,0 ,scellDeactBitmask
             /*Harq Id sent zero for CA deactivaion */
                     );

    /*ca phase-3 changes end*/

}
/*****************************************************************************
 * Function Name  : processMacRRMDeactTimerExpInDeactivationOnGoingViaMac 
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_RRM_DEACTIVATION_TIMER_EXPIRE  
 *                  for event handling of Scell state DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC.
 *****************************************************************************/
void processMacRRMDeactTimerExpInDeactivationOnGoingViaMac(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    UInt8 scellDeactBitmask = 0;
    /*ca-tdd changes start*/
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    /*ca phase-3 changes start*/
#ifdef FDD_CONFIG
    /*SPR 15909 fix start*/
    tickType_t scheduledExpiryTick = getCurrentTick() + PHY_DL_DELAY + SCELL_STATE_CHANGE_TIMER_OFFSET/*8*/; 
    /*SPR 15909 fix end*/
    UInt8  harqSubFrameNum = (scheduledExpiryTick + FDD_HARQ_OFFSET )%MAX_SUB_FRAME;
#else
    UInt32 sCellStateChangeTddTimerOffset = 0;
    sCellStateChangeTddTimerOffset = getSCellStateChangeTddTimerOffset(ueDLContext_p->internalCellIndex);
    /*SPR 15909 fix start*/
    tickType_t scheduledExpiryTick = getCurrentTick() + PHY_DL_DELAY + sCellStateChangeTddTimerOffset;
    /*SPR 15909 fix end*/
    UInt8  harqSubFrameNum = (scheduledExpiryTick)%MAX_SUB_FRAME;
#endif
    /*ca-tdd changes end*/
    SET_BYTE_FIELD( scellDeactBitmask, 
            1, 
            eventQNode_p->scellId,
            1);
    dlCATimerStart(ueDLContext_p->ueIndex,harqSubFrameNum,\
                   scheduledExpiryTick,0 ,scellDeactBitmask
             /*Harq Id sent zero for CA deactivaion */
                  );

    /*ca phase-3 changes end*/

}
/*****************************************************************************
 * Function Name  : processRrmDeactivInActivationOnGoing
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_RRM_SCELL_DEACTIVATION  
 *                  for event handling of Scell state DL_CA_SCELL_ACTIVATION_ON_GOING.
 *****************************************************************************/
void processRrmDeactivInActivationOnGoing(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    /*set deactivation bitmask at scell context*/
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->ActDeactBitMask = SCELL_DEACT_BIT_SET;
}
/*****************************************************************************
 * Function Name  : processRrmDeactivInDeactivationOnGoingViaMac 
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_RRM_SCELL_DEACTIVATION  
 *                  for event handling of Scell state DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC.
 *****************************************************************************/
void processRrmDeactivInDeactivationOnGoingViaMac(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    DLCAMacCEQueueNode *dlCAMacCEQueueNode_p = PNULL;
    DLCAMacFreeScellHarqBufferQueueNode *dlCAMacFreeScellHarqBufferQueueNode_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_RRM;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */
    
    if(ueDLContext_p->isMACCESendToUE == LTE_TRUE)
    {    
        /*prepare and send mac CE*/
        GET_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueueNode_p,DLCAMacCEQueueNode);
        if(dlCAMacCEQueueNode_p != PNULL)
        {
            dlCAMacCEQueueNode_p->ueId = eventQNode_p->ueId;
            dlCAMacCEQueueNode_p->scellId = eventQNode_p->scellId;
            dlCAMacCEQueueNode_p->dlCAQosEvent = DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER;

            /* + SPR 22352 fix */
            SET_SCELL_ACT_DEACT_BIT(dlCAMacCEQueueNode_p->ueId,dlCAMacCEQueueNode_p->scellId,dlCAMacCEQueueNode_p->dlCAQosEvent);
            /* - SPR 22352 fix */
            if(!ENQUEUE_DL_CA_MAC_CE_Q(dlCAMacCEQueue_gp[ueDLContext_p->internalCellIndex],(void *)dlCAMacCEQueueNode_p))
            {
                FREE_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueueNode_p);
            }
        }
    }
    /*prepare and insert node to reset the harq buffer of scell deactivated*/
    GET_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p,DLCAMacFreeScellHarqBufferQueueNode);
    if(dlCAMacFreeScellHarqBufferQueueNode_p != PNULL)
    {
        dlCAMacFreeScellHarqBufferQueueNode_p->ueId = eventQNode_p->ueId;
        dlCAMacFreeScellHarqBufferQueueNode_p->scellId = eventQNode_p->scellId;

        if(!ENQUEUE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp[ueDLContext_p->internalCellIndex],\
                    (void *)dlCAMacFreeScellHarqBufferQueueNode_p))
        {
            FREE_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p);
        }
    }
}
/*****************************************************************************
 * Function Name  : processMacDeactTimerExpInDeactivationOnGoingViaMac 
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_DEACTIVATION_TIMER_EXPIRE
 *                  for event handling of Scell state DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC.
 *****************************************************************************/
void processMacDeactTimerExpInDeactivationOnGoingViaMac(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    DLCAMacCEQueueNode *dlCAMacCEQueueNode_p = PNULL; 
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_CONFIGURED;
   /*SPR 21948 Changes Start */
    /* Changes under spr 21762 have been reverted */
     /*SPR 21948 Changes End */
    /*check whether there is any buffer request for activation */
    if(ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->ActDeactBitMask == SCELL_ACT_BIT_SET)
    {        
        /*prepare and send mac CE*/
        GET_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueueNode_p,DLCAMacCEQueueNode);
        if(dlCAMacCEQueueNode_p != PNULL)
        {
            dlCAMacCEQueueNode_p->ueId = eventQNode_p->ueId;
            dlCAMacCEQueueNode_p->scellId = eventQNode_p->scellId;
            dlCAMacCEQueueNode_p->dlCAQosEvent = DL_CA_EVENT_MAC_SCELL_ACT_CE_TRIGGER;

            /* + SPR 22352 fix */
            SET_SCELL_ACT_DEACT_BIT(dlCAMacCEQueueNode_p->ueId,dlCAMacCEQueueNode_p->scellId,dlCAMacCEQueueNode_p->dlCAQosEvent);
            /* - SPR 22352 fix */
            if(!ENQUEUE_DL_CA_MAC_CE_Q(dlCAMacCEQueue_gp[ueDLContext_p->internalCellIndex],(void *)dlCAMacCEQueueNode_p))
            {
                FREE_MEM_NODE_DL_CA_MAC_CE_Q(dlCAMacCEQueueNode_p);
            }
            /*reset the bitmask*/
            ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->ActDeactBitMask = SCELL_ACT_DEACT_BIT_RESET;
        }
    }
    /*SPR 16221 Fix Start*/
    /*cqiReportValidTimer is configured per UE from RRM and once MAC De-activation CE sent, 
      MAC will also update isCQIReportValidTTI as (currentTTI + cqiReportValidTimer)*/
    /* SPR 16422 START */
    if(ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->cqiValidityTimer)
    {
        ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->ScelldlCQIInfo.isCQIReportValidTTI = 
                         getCurrentTick() +
                         ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->cqiValidityTimer;
    }
    /* SPR 16422 END */
    /*SPR 16221 Fix end*/

}
/*****************************************************************************
 * Function Name  : processMacDeactTimerExpInDeactivationOnGoingViaRrm
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the DL_CA_EVENT_DEACTIVATION_TIMER_EXPIRE  
 *                  for event handling of Scell state DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_RRM.
 *****************************************************************************/
void processMacDeactTimerExpInDeactivationOnGoingViaRrm(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    DLCAEventQueueNode * tempEventQNode_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->scellState = DL_CA_SCELL_DEACTIVATED;
    /*SPR 21948 Changes Start */
     /* Changes under spr 21762 have been reverted */
      /*SPR 21948 Changes End */

    if(ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->ActDeactBitMask == SCELL_ACT_BIT_SET)
    {        
        /*prepare and send mac CE*/
        GET_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p,DLCAEventQueueNode);
        if(tempEventQNode_p != PNULL)
        { 
            tempEventQNode_p->ueId = eventQNode_p->ueId;
            tempEventQNode_p->scellId = eventQNode_p->scellId;
            tempEventQNode_p->dlCAQosEvent = DL_CA_EVENT_RRM_SCELL_ACTIVATION;

            if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueDLContext_p->internalCellIndex],(void *)tempEventQNode_p))
            {
                FREE_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p);
            }
            /*reset the bitmask*/ 
            ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->ActDeactBitMask = SCELL_ACT_DEACT_BIT_RESET;
       }
    }
    /*cqiReportValidTimer is configured per UE from RRM and once MAC De-activation CE sent, 
      MAC will also update isCQIReportValidTTI as (currentTTI + cqiReportValidTimer)*/
    /*SPR 16221 Fix start*/
    /* SPR 16422 START */
    if(ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->cqiValidityTimer)
    {
        ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->ScelldlCQIInfo.isCQIReportValidTTI = 
                          getCurrentTick() +
                          ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->cqiValidityTimer;
    }
    /* SPR 16422 END */
    /*SPR 16221 Fix end*/ 
}
/*****************************************************************************
* Function Name  : processRrmActivInDeactivationOnGoingViaRrm
* Inputs         : eventQNode_p - DLCAEventQueueNode
* Outputs        : None
* Returns        : void
* Description    : This function Process the DL_CA_EVENT_ACTIVE
*                  for event handling of Scell state DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_RRM.
*****************************************************************************/
void processRrmActivInDeactivationOnGoingViaRrm(DLCAEventQueueNode * eventQNode_p)
{
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = dlUECtxInfoArr_g[eventQNode_p->ueId].dlUEContext_p;
    /*set bitmask in scell context for activation*/
    ueDLContext_p->dlUeScellContext_p[eventQNode_p->scellId]->ActDeactBitMask = SCELL_ACT_BIT_SET;
    
}
 
/*****************************************************************************
 * Function Name  : processInvalidEvent 
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the invalid event handling.
 *****************************************************************************/
void processInvalidEvent(DLCAEventQueueNode * eventQNode_p)
{          
    LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_DL_Strategy,
            getCurrentTick(),
            eventQNode_p->ueId,LINE_NUMBER,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"");
}
/*****************************************************************************
 * Function Name  : processInvalidEventStateTrigger
 * Inputs         : eventQNode_p - DLCAEventQueueNode 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the default event handling for a 
 *                  particular Scell state.
 *****************************************************************************/
void processInvalidEventStateTrigger(DLCAEventQueueNode * eventQNode_p)
{
    LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_DL_Strategy,
            getCurrentTick(),
            eventQNode_p->ueId,LINE_NUMBER,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"");
}
/*****************************************************************************
 * Function Name  : initDlCAScellStateEventArr 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None.
 * Description    : This function intialise the function ptr array  
 *                  dlCAScellStateEventArr_g for Scell state event handling
 *****************************************************************************/
/* + SPR 17439 */
void initDlCAScellStateEventArr(void)
/* - SPR 17439 */    
{
     UInt8 eventIdx     = 0; 
     UInt8 cellStateIdx = 0; 
     for(eventIdx = 0;eventIdx < MAX_NUM_DL_CA_EVENT;eventIdx++)
     {
         for(cellStateIdx = 0;cellStateIdx < MAX_NUM_DL_CA_SCELL_STATE;cellStateIdx++)
         {
             dlCAScellStateEventArr_g[cellStateIdx][eventIdx] = processInvalidEventStateTrigger;
         }
     }
     /*Events handling for Scell Deactivation state*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_DEACTIVATED][DL_CA_EVENT_SCELL_RELEASE]                  = processScellRelToStateInit;
     dlCAScellStateEventArr_g[DL_CA_SCELL_DEACTIVATED][DL_CA_EVENT_RRM_SCELL_ACTIVATION]           = processScellActivInDeactivation;  
     /*Events handling for Scell INIT state*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_INIT][DL_CA_EVENT_RRM_SCELL_DEACTIVATION]                = processRrmDeactivInInit;
     dlCAScellStateEventArr_g[DL_CA_SCELL_INIT][DL_CA_EVENT_RRM_SCELL_ACTIVATION]                  = processRrmActivInInit;
     dlCAScellStateEventArr_g[DL_CA_SCELL_INIT][DL_CA_EVENT_RRC_UE_CREATE]                         = processUeCreateInInit;
     /*Events handling for Scell Configured state*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_CONFIGURED][DL_CA_EVENT_RRM_SCELL_DEACTIVATION]          = processRrmDeactivInConfigured;
     dlCAScellStateEventArr_g[DL_CA_SCELL_CONFIGURED][DL_CA_EVENT_RRM_SCELL_ACTIVATION]            = processRrmActivInConfigured;
     dlCAScellStateEventArr_g[DL_CA_SCELL_CONFIGURED][DL_CA_EVENT_SCELL_RELEASE]                   = processScellRelToStateInit;
     dlCAScellStateEventArr_g[DL_CA_SCELL_CONFIGURED][DL_CA_EVENT_MAC_SCELL_ACT_CE_SENT]           = processMacScellActivInConfigured;
     dlCAScellStateEventArr_g[DL_CA_SCELL_CONFIGURED][DL_CA_EVENT_MAC_SCELL_ACT_CE_TRIGGER]        = processReconfigApiIndicationInConfigured;
     /*Events handling for Scell Activation-on-going state*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVATION_ON_GOING][DL_CA_EVENT_RRM_SCELL_ACTIVATION]   = processRrmActivInActivationOnGoing;
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVATION_ON_GOING][DL_CA_EVENT_SCELL_RELEASE]          = processScellRelToStateInit;
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVATION_ON_GOING][DL_CA_EVENT_RRM_SCELL_DEACTIVATION] = processRrmDeactivInActivationOnGoing;
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVATION_ON_GOING][DL_CA_EVENT_ACTIVATION_TIMER_EXPIRE] = \
                                                                                     processMacActTimerExpInActivationOnGoing;             
     /*SPR 17561 FIX*/
#ifdef FDD_CONFIG
     /*SPR 16026 changes start*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVATION_ON_GOING][DL_CA_EVENT_ACT_MAC_CE_HARQ_FAILURE] = processActHarqFailureInActivationOnGoing;
     /*DL_CA_EVENT_ACT_MAC_CE_HARQ_FAILURE : Max DL re-transmission reached  go to configured state*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVATION_ON_GOING][DL_CA_EVENT_ACT_MAC_CE_HARQ_NACK]    = processMacScellActivInConfigured;
     /*SPR 16026 changes end*/
#endif
     /*SPR 17561 FIX*/
     /*Events handling for Scell Active state*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVE][DL_CA_EVENT_RRM_SCELL_ACTIVATION]                = processRrmActivInActivation;
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVE][DL_CA_EVENT_SCELL_RELEASE]                       = processScellRelToStateInit;
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVE][DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER]          = processScellDeactivInActivation;
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVE][DL_CA_EVENT_RRM_SCELL_DEACTIVATION]              = processScellDeactivInActivation;
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVE][DL_CA_EVENT_RRM_DEACTIVATION_MAC_CE_SENT]        = processRrmScellDeactivMacCESentInActivation;
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVE][DL_CA_EVENT_MAC_DEACTIVATION_MAC_CE_SENT]        = processMacScellDeactivMacCESentInActivation;
     /*spr 12471 fix start*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_ACTIVE][DL_CA_EVENT_RRM_DEACTIVATION_TIMER_EXPIRE]       = processMacDeactTimerExpInActivation;
     /*spr 12471 fix end*/
     /*Events handling for Scell Deactivation on going via MAC state*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC][DL_CA_EVENT_RRM_SCELL_DEACTIVATION] = \
                                                                                          processRrmDeactivInDeactivationOnGoingViaMac;
     dlCAScellStateEventArr_g[DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC][DL_CA_EVENT_DEACTIVATION_TIMER_EXPIRE] = \
                                                                                     processMacDeactTimerExpInDeactivationOnGoingViaMac;
     dlCAScellStateEventArr_g[DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC][DL_CA_EVENT_RRM_DEACTIVATION_TIMER_EXPIRE] =  \
                                                                                     processMacRRMDeactTimerExpInDeactivationOnGoingViaMac; 
     /*SPR 17561 FIX*/
#ifdef FDD_CONFIG
     /*SPR 16026 changes start*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC][DL_CA_EVENT_DEACT_MAC_CE_HARQ_FAILURE] = processHarqFailureInDeactivationOnGoingViaMac;
     /*SPR 16026 changes end*/
#endif
     /*SPR 17561 FIX*/
     /*Events handling for Scell Deactivation on going via RRM state*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_RRM][DL_CA_EVENT_DEACTIVATION_TIMER_EXPIRE] = \
                                                                                      processMacDeactTimerExpInDeactivationOnGoingViaRrm;
     dlCAScellStateEventArr_g[DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_RRM][DL_CA_EVENT_RRM_SCELL_ACTIVATION] = \
                                                                                      processRrmActivInDeactivationOnGoingViaRrm;
     /*SPR 17561 FIX*/
#ifdef FDD_CONFIG
     /*SPR 16026 changes start*/
     dlCAScellStateEventArr_g[DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_RRM][DL_CA_EVENT_DEACT_MAC_CE_HARQ_FAILURE] = \
                                                                                      processHarqFailureInDeactivationOnGoingViaMac;
     /*SPR 16026 changes end*/
#endif
     /*SPR 17561 FIX*/

}
/*****************************************************************************
 * Function Name  : processdlCAEventQueue 
 * Inputs         : internalCellIndex, dlCAEventQueueCount 
 * Outputs        : None
 * Returns        : void
 * Description    : This function Process the dlCAEventQueue_gp  
 *                  for event handling of Scell state.
 *****************************************************************************/
void processdlCAEventQueue(InternalCellIndex internalCellIndex,UInt16 dlCAEventQueueCount)
{
    UInt16 indx = 0; 
    DLCAEventQueueNode * eventQNode_p = PNULL;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext *ueDLContext_p = PNULL;
    DLCAScellStateFlag scellStatus;
    
    for (indx = 0; indx < dlCAEventQueueCount; indx++)
    {
        DEQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[internalCellIndex],(void **)&eventQNode_p);
        
        if (eventQNode_p == PNULL)
        {
            continue;
        }
        if(MAX_NUM_DL_CA_EVENT <= eventQNode_p->dlCAQosEvent)
        {
            processInvalidEvent(eventQNode_p);
        }
        else
        {
            if(INVALID_UE_ID <= eventQNode_p->ueId)
            {
                FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
                continue;
            }
            dlUEContextInfo_p = &dlUECtxInfoArr_g[eventQNode_p->ueId];
            /*SPR 12911 fix , added conditions to handle intra Cell HO scenario as well*/ 
            if((dlUEContextInfo_p) && ( !(dlUEContextInfo_p->pendingDeleteFlag) || 
                (dlUEContextInfo_p->dlUEContext_p && dlUEContextInfo_p->dlUEContext_p->tcrntiFlag)))
            {
                ueDLContext_p = dlUEContextInfo_p->dlUEContext_p;

	            /* EICIC +*/
	            /* Pushing UE back in trigger queue.*/
	            if ((VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (NON_VICTIM_UE == ueDLContext_p->userType))
	            {              
                    if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[internalCellIndex],(void *)eventQNode_p))
                    {
                        FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
                    }
	          	    continue;
	            }
	            else if ((NON_VICTIM_UE == ueTypeToScheduleDL_g[internalCellIndex]) && (VICTIM_UE == ueDLContext_p->userType))
	            {           
                    if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[internalCellIndex],(void *)eventQNode_p))
                    {
                        FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
                    }
	          	    continue;
	            }
	            /* EICIC - */
                if(ueDLContext_p != PNULL)
                {  
                    if (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX] != PNULL)
                    {
                        scellStatus = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->scellState;
                        /*calling event handling function for particular Scell state*/
                        if(scellStatus < MAX_NUM_DL_CA_SCELL_STATE)
                        {
                            dlCAScellStateEventArr_g[scellStatus][eventQNode_p->dlCAQosEvent](eventQNode_p);
                        }
                    }
                    else
                    {
                        FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
                        continue;
                    }
                }
            } 
        }

        FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);

    }
}
 
/*****************************************************************************
 * Function Name  : dlCAScellStateManagerIsScellActive 
 * Inputs         : ueIndex ,servCellIdx
 * Outputs        : None
 * Returns        : LTE_TRUE/LTE_FALSE
 * Description    : This function Check whether a SCell is active or not 
 *                  currently.
 *****************************************************************************/
UInt8 dlCAScellStateManagerIsScellActive(UInt16 ueIndex,UInt8 servCellIdx)
{
   DLUEContext *ueDLContext_p = PNULL;
   ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
   
    /* CA-Ph2 Integration fix start */
   /*coverity id 62974*/
   if ((!IS_CA_ENABLED()) || (servCellIdx > MAX_NUM_SCELL) || (ueDLContext_p->dlUeScellContext_p[servCellIdx] == PNULL))
    /* CA-Ph2 Integration fix end */
   {
       return LTE_FALSE;
   }   
   if(ueDLContext_p->dlUeScellContext_p[servCellIdx]->scellState == DL_CA_SCELL_ACTIVE)
   {
       return LTE_TRUE;
   }
   else
   {
       return LTE_FALSE;
   }
}

/*****************************************************************************
 * Function Name  : dlCAScellStateManagerIsScellActiveAtTti
 * Inputs         : ueIndex ,servCellIdx
 * Outputs        : None
 * Returns        : LTE_TRUE/LTE_FALSE
 * Description    : This function Check whether a SCell is active or not 
 *                  at a particular TTI(tick)
 *****************************************************************************/
UInt8 dlCAScellStateManagerIsScellActiveAtTti(UInt16 ueIndex,UInt8 servCellIdx)
{   
   DLUEContext *ueDLContext_p = PNULL;
   DLCATimerNode* dlCATimerNode_p = PNULL;
   UInt8 retVal = LTE_FALSE;
   ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    /* CA-Ph2 Integration fix start */
   if (servCellIdx <= MAX_NUM_SCELL && PNULL != ueDLContext_p->dlUeScellContext_p[servCellIdx])
   {
    /* CA-Ph2 Integration fix end */
       switch (ueDLContext_p->dlUeScellContext_p[servCellIdx]->scellState)
       {
           case DL_CA_SCELL_DEACTIVATED:
           case DL_CA_SCELL_INIT:
           case DL_CA_SCELL_CONFIGURED:
               retVal = LTE_FALSE;
               break;

           case DL_CA_SCELL_ACTIVE:
               retVal = LTE_TRUE;
               break;

           case DL_CA_SCELL_ACTIVATION_ON_GOING:
               dlCATimerNode_p = ueDLContext_p->dlUeScellContext_p[servCellIdx]->dlCATimerNode_p;
               if (!(dlCATimerNode_p) || (dlCATimerNode_p->dlCAExpiryTick < getCurrentTick()))
               {
                   retVal = LTE_FALSE;
               }  
               else
               {
                   retVal = LTE_TRUE;
               }
               break;
           case DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_RRM:
           case DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC:
               dlCATimerNode_p = ueDLContext_p->dlUeScellContext_p[servCellIdx]->dlCATimerNode_p;
               if (!(dlCATimerNode_p) || (dlCATimerNode_p->dlCAExpiryTick <= getCurrentTick()))
               {
                   retVal = LTE_FALSE;              
               }  
               else
               {
                   retVal = LTE_TRUE;
               }
               break;
           default: 
               break; 
       }
   /* CA-Ph2 Integration fix start */
   }
   /* CA-Ph2 Integration fix end */
    return retVal;
}
/*****************************************************************************
 * Function Name  :   dlCATimerStart
 * Inputs             ueIndex - The UE index of the UE for which a CA Timer  
 *                              has to be started,
 *                    frameNumber - The sub frame number for which the UE data
 *                                  will be on the PHYSICAL layer,
 *                    dlCAExpiryTick - The RTT Timer expiry tick,
 *                    scellActBitmask - 
 *                    scellDeActBitmask -
 * Outputs        :   None
 * Returns        :   MAC_FAILURE/MAC_SUCCESS
 * Description    :   This function will start the HARQ Timer functionality.
 *****************************************************************************/
 MacRetType dlCATimerStart( 
        UInt16  ueIndex,
        UInt32  frameNumber,
        /*SPR 15909 fix start*/
        tickType_t  dlCAExpiryTick,
        /*SPR 15909 fix end*/
        UInt8   scellActBitmask,
        UInt8   scellDeActBitmask
        )
{
    UInt8 dlCAEntityTimerArrIndex = 0;
    DLCATimerEntityList* dlCAEntityTimerList_p = PNULL;
    DLCATimerNode* dlCATimerNode_p = PNULL;
    DLUEContext*  ueContext_p = PNULL;       
    /*Need to make it generic for each scell in phase 3*/
    UInt8 actScellId = scellActBitmask >> SCELL_SHIFT_GET_SCELL_ID;
    UInt8 deActScellId = scellDeActBitmask >> SCELL_SHIFT_GET_SCELL_ID;
    ueContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
 /*SPR 14049 change start*/
#ifdef TDD_CONFIG
    frameNumber = dlCAExpiryTick % MAX_SUB_FRAME_TDD_OFFSET; 
#endif
 /*SPR 14049 change end*/

    /*Check if both activation/deactivation scell id are same*/
    /* SPR 16422 START */
    if(actScellId == deActScellId || (!ueContext_p->scellCount))
    /* SPR 16422 END */
    { 
         return MAC_FAILURE;
    }
    if(actScellId > 0)
    {
        dlCAEntityTimerList_p = &(dlCAGlobals_g[ueContext_p->internalCellIndex].dlCATimerEntityList[frameNumber]);
        /*
        NOTE:- The value of dlCAEntityTimerList_p->count was initialised to 0
        (invalid value) and hence we have to increase it after it is used as 
        array index, on the first occasion on the other occasion it just gives
        the proper array index.
        */ 

        dlCAEntityTimerArrIndex = dlCAEntityTimerList_p->count;

        dlCATimerNode_p = 
            &(dlCAEntityTimerList_p->dlCATimerNodeArr[dlCAEntityTimerArrIndex]);

        dlCATimerNode_p->ueIndex = ueIndex;
        dlCATimerNode_p->isValid = CA_TIMER_NODE_VALID;
        dlCATimerNode_p->dlCAExpiryTick= dlCAExpiryTick;
        dlCATimerNode_p->scellId = actScellId; 
        dlCATimerNode_p->dlCAScellState = DL_CA_SCELL_ACTIVE; 
        dlCAEntityTimerList_p->count++;
        ueContext_p->dlUeScellContext_p[actScellId]->dlCATimerNode_p = dlCATimerNode_p;
        updateDlAggregateGbrOnScellStateChange(ueContext_p, SCELL_ACT_TRIGGER);
        lteMacDLQoSDistributeTokenAmongCells(ueContext_p);

        return MAC_SUCCESS;
    }
    if(deActScellId > 0)
    {
        dlCAEntityTimerList_p = &(dlCAGlobals_g[ueContext_p->internalCellIndex].dlCATimerEntityList[frameNumber]);
        /*
        NOTE:- The value of dlCAEntityTimerList_p->count was initialised to 0
        (invalid value) and hence we have to increase it after it is used as 
        array index, on the first occasion on the other occasion it just gives
        the proper array index.
        */ 

        dlCAEntityTimerArrIndex = dlCAEntityTimerList_p->count;

        dlCATimerNode_p = \
                            &(dlCAEntityTimerList_p->dlCATimerNodeArr[dlCAEntityTimerArrIndex]);

        dlCATimerNode_p->ueIndex = ueIndex;
        dlCATimerNode_p->scellId = deActScellId;
        dlCATimerNode_p->isValid = CA_TIMER_NODE_VALID;
        dlCATimerNode_p->dlCAExpiryTick= dlCAExpiryTick;
        dlCATimerNode_p->dlCAScellState = DL_CA_SCELL_DEACTIVATED;
        dlCAEntityTimerList_p->count++;

        ueContext_p->dlUeScellContext_p[deActScellId]->dlCATimerNode_p = dlCATimerNode_p; 
        updateDlAggregateGbrOnScellStateChange(ueContext_p, SCELL_DEACT_TRIGGER);
        return MAC_SUCCESS;
    }

    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  :  expireAllNodeFromCATimerList 
 * Inputs         :  loopOfNumOfSubFrames - Represent the number of previous
 *                                          subframes including the current tick 
 *                                          one for which the entry corresponding
 *                                          to HARQ TIMER ARRAY has to be deleted,
 *                   currentSubFrame - the present Sub Frame,   
 *                   delaySubFrame - Delay Sub Frame
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the CA Timer 
 *                   corresponding to particular sub frame, this function puts 
 *****************************************************************************/
MacRetType expireAllNodeFromCATimerList (
        UInt8 loopOfNumOfSubFrames,
        /* +- SPR 17777 */
#ifdef FDD_CONFIG
        UInt32 currentSubFrame,
        UInt8 delaySubFrame,
#endif
        InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
{
    UInt16 dlCAEntityTimerArrCount = 0;        
    DLCATimerEntityList* dlCAEntityTimerList_p = PNULL;
    DLCATimerEntityList* dlCAEntityTimerListAccess_p = PNULL;
    DLCATimerNode* dlCATimerNode_p = PNULL;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContextInfo* dlUeContextInfoArrayAccess_p = PNULL;
    DLUEContext*  dlUeContext_p = PNULL;
    UInt16 ueIndex = INVALID_UE_ID;
    UInt8 frameNumber = 0;
    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    dlUeContextInfoArrayAccess_p = &dlUECtxInfoArr_g[0];
    dlCAEntityTimerListAccess_p = &(dlCAGlobals_g[internalCellIndex].dlCATimerEntityList[0]);

 /*SPR 14049 change start*/
#ifdef FDD_CONFIG
 /*SPR 14049 change end*/
    if (0 == currentSubFrame)
    {
        frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - delaySubFrame;
    }
    else if (currentSubFrame >= delaySubFrame)
    {
        frameNumber = currentSubFrame - delaySubFrame;
    }
    else 
    {
        delaySubFrame -= currentSubFrame;
        frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - delaySubFrame;
    }

    if (frameNumber >= MAX_SUB_FRAME)
    {
        frameNumber = (MAX_SUB_FRAME - 1);
    }
#elif TDD_CONFIG
 /*SPR 14049 change start*/
    frameNumber =((getCurrentTick())% MAX_SUB_FRAME_TDD_OFFSET);
#endif
 /*SPR 14049 change end*/

#ifdef OVERSHOOT_CORRECTION
    if(loopOfNumOfSubFrames > MAX_NUM_OF_TICK_MISSED)
    {
        return MAC_SUCCESS ;
    }
#endif 
    while (loopOfNumOfSubFrames--)
    {
        dlCAEntityTimerList_p = &dlCAEntityTimerListAccess_p[frameNumber];   
        if (dlCAEntityTimerList_p->count >= MAX_NUM_UE) 
        {
            return MAC_FAILURE;
        } 
        dlCAEntityTimerArrCount = dlCAEntityTimerList_p->count;

        if (!dlCAEntityTimerArrCount)
        {
            /*
               As there is no node in this Index of Array hence nothing is to be deleted
               remember the valid value of an array starts form zero and the above
               variable dlCAEntityTimerList_p->count represents the array index.
             */
            /*CYCLOMATIC_CHANGES_6.1_START*/
            frameNumber = calculateFrameNumberForCATimerExpiry(frameNumber);
            /*CYCLOMATIC_CHANGES_6.1_END*/
            continue;
        }

        while (dlCAEntityTimerArrCount--)
        {
            dlCATimerNode_p = \
                                &(dlCAEntityTimerList_p->dlCATimerNodeArr[dlCAEntityTimerArrCount]);    
            if (dlCATimerNode_p->isValid)
            {
                ueIndex = dlCATimerNode_p->ueIndex;
                dlUeContextInfo_p = &dlUeContextInfoArrayAccess_p[ueIndex];
                /*
                   We ascertain whether th eUE under consideration is already on the
                   process of deletion from the RRC
                 */  
                if ((!dlUeContextInfo_p->pendingDeleteFlag) && (PNULL != dlUeContextInfo_p->dlUEContext_p))
                {
                    dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;
                    /*check for scell status and insert in the queue*/
                    /*CYCLOMATIC_CHANGES_6.1_START*/
                    if (MAC_FAILURE == enqueueInDLCAEventQueue(dlCATimerNode_p,dlUeContext_p

                                ))
                    {
                        return MAC_FAILURE;
                    }
                    /*CYCLOMATIC_CHANGES_6.1_END*/
                    dlCATimerNode_p->isValid = CA_TIMER_NODE_INVALID;
                    dlCATimerNode_p->ueIndex = INVALID_UE_ID;
                    /* + SPR 14911 Changes */
                    if(dlUeContext_p->dlUeScellContext_p[dlCATimerNode_p->scellId])
                    {
                        dlUeContext_p->dlUeScellContext_p[dlCATimerNode_p->scellId]->dlCATimerNode_p = NULL;
                    }
                    /* - SPR 14911 Changes */
                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                            "\nThe Pending delete Flag has been set by RRC for the UE Index = %d"
                            " \n in side the function expireAllNodeFromHarqTimerList", ueIndex);
                    continue;
                }
            }
            else
            {
            }
        }

        dlCAEntityTimerList_p->count = 0;

        LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                "The value of count in DLCAEntityTimerList is ZERO  for the sub"
                "frame no. %d", frameNumber);
        /*CYCLOMATIC_CHANGES_6.1_START*/
        frameNumber = calculateFrameNumberForCATimerExpiry(frameNumber);
        /*CYCLOMATIC_CHANGES_6.1_END*/
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/************************************************************************
 * Function Name  :  initDLCATimerArray
 * Inputs         :  numOfCells - Number of cells configured
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function inits the DL HARQ Timer Array by initializing the values.
 ************************************************************************/

void initDLCATimerArray(UInt8 numOfCells)
{
    UInt8 i = 0;
    UInt8 j = 0;
    UInt8 cellIndex = 0;
    DLCATimerEntityList* dlCAEntityTimerList_p = PNULL;
    DLCATimerNode* dlCATimerNode_p = PNULL;

    for(cellIndex = 0;cellIndex < numOfCells;cellIndex++)
    {
        for (i = MAX_SUB_FRAME; i; i-- )
        {
            dlCAEntityTimerList_p = &(dlCAGlobals_g[cellIndex].dlCATimerEntityList[i-1]);
            for (j=MAX_DL_UE_SCHEDULED; j; j--)
            {
                dlCATimerNode_p = &dlCAEntityTimerList_p->dlCATimerNodeArr[j-1];
                dlCATimerNode_p->isValid = CA_TIMER_NODE_INVALID;
                dlCATimerNode_p->ueIndex = INVALID_UE_ID;
                dlCATimerNode_p->scellId = 0;
                dlCATimerNode_p->dlCAScellState = DL_CA_SCELL_INIT;
                dlCATimerNode_p->dlCAExpiryTick = 0;
            }
            dlCAEntityTimerList_p->count = 0;
        }
    }
}
/************************************************************************
 * Function Name  :  checkScellActDeactBitmask 
 * Inputs         :  dlHarqProcess_p - ptr to harq 
 *                   ueContext_p - ptr to dl uecontext
 *                   harqSubFrameNum - subframe for timer node 
 *                   scheduledExpiryTick - expected expiry tick 
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function checks the act/deact bitmask in harq and 
 *                   trigger the corresponding event. 
 ************************************************************************/
/*SPR 17561 FIX*/
#ifdef FDD_CONFIG
 void checkScellActDeactBitmask(
                    DLHARQProcess* dlHarqProcess_p,
                    DLUEContext *ueContext_p
				) 
                    /*SPR 16026 +-*/ 
#else

 void checkScellActDeactBitmask(
                    DLHARQProcess* dlHarqProcess_p,
                    DLUEContext *ueContext_p,
                    UInt8 harqSubFrameNum,
                    UInt32 scheduledExpiryTick)
#endif
/*SPR 17561 FIX*/
{
    DLCAEventQueueNode *eventQNode_p = PNULL;
    ServingCellIndex scellId = 0;

    if(dlHarqProcess_p->scellDeactBitmask)
    {
#ifdef TDD_CONFIG
        dlCATimerStart(ueContext_p->ueIndex,harqSubFrameNum,
                scheduledExpiryTick,dlHarqProcess_p->scellActBitmask ,dlHarqProcess_p->scellDeactBitmask);
#endif
        /* + coverity 63431 */
        scellId = dlHarqProcess_p->scellDeactBitmask >> 1;
        /*SPR 20192 Changes Start*/
        if(ueContext_p->dlUeScellContext_p[scellId])
        {
            /*prepare and insert mac evnt Q node*/
            GET_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p,DLCAEventQueueNode);
            if(eventQNode_p != PNULL)
            {    
                eventQNode_p->ueId = ueContext_p->ueIndex;
                eventQNode_p->scellId = scellId;
                /*SPR 20192 Changes End*/
                if(ueContext_p->dlUeScellContext_p[scellId]->isScellActivateStatus == LTE_FALSE)
                {
                    eventQNode_p->dlCAQosEvent = DL_CA_EVENT_RRM_DEACTIVATION_MAC_CE_SENT;
                }
                else
                { 
                    eventQNode_p->dlCAQosEvent = DL_CA_EVENT_MAC_DEACTIVATION_MAC_CE_SENT;
                }
                if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueContext_p->internalCellIndex],(void *)eventQNode_p))
                {
                    FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
                }
            }
        }
        /* - coverity 63431 */
    }
    else if(dlHarqProcess_p->scellActBitmask)
    {  
        /*SPR 17561 FIX*/
#ifdef TDD_CONFIG
        /*SPR 16026 chnages start*/
        dlCATimerStart(ueContext_p->ueIndex,harqSubFrameNum,\
              scheduledExpiryTick,dlHarqProcess_p->scellActBitmask,dlHarqProcess_p->scellDeactBitmask); 
        /*SPR 16026 chnages end*/
        /*SPR 17561 FIX*/
#endif
        /*prepare and insert mac evnt Q node*/
        GET_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p,DLCAEventQueueNode);
        if(eventQNode_p != PNULL)
        {               
            eventQNode_p->ueId = ueContext_p->ueIndex;
            eventQNode_p->scellId = dlHarqProcess_p->scellActBitmask >> 1;
            eventQNode_p->dlCAQosEvent = DL_CA_EVENT_MAC_SCELL_ACT_CE_SENT;
            if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueContext_p->internalCellIndex],(void *)eventQNode_p))
            {
                FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
            }
	}
    }
    else
    {
    } 
}
/*****************************************************************************
 * Function Name  : dlCAScellStateManagerRegenerateMacCEEvent 
 * Inputs         : internalCellIndex
 *                  node_p - Mac ce Q node ptr
 * Outputs        : None
 * Returns        : None
 * Description    : It will regenerate the event for scheduling failure Mac CE 
 *****************************************************************************/
void dlCAScellStateManagerRegenerateMacCEEvent(InternalCellIndex internalCellIndex,DLCAMacCEQueueNode* node_p)
{
    DLCAEventQueueNode * tempEventQNode_p = PNULL;
    /*getmempool for new event Q Node*/
    GET_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p,DLCAEventQueueNode);
    if(tempEventQNode_p != PNULL)
    {    
        tempEventQNode_p->ueId = node_p->ueId;
        tempEventQNode_p->scellId = node_p->scellId;
        tempEventQNode_p->dlCAQosEvent = node_p->dlCAQosEvent; 
        if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[internalCellIndex],(void *)tempEventQNode_p))
        {
            FREE_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p);
        }
    }
    else
    {
    }     
} 
/*ca chngs end*/ 

/*****************************************************************************
 * Function Name  : updateDlAggregateGbrOnScellStateChange
 * Inputs         : ueDLContext_p, actDeactTrigger
 * Outputs        :
 * Returns        : None
 * Description    : This function updates dlAggregateGbrPerTTI in respective
 *                  serving cells based on Scell Activation/DeActivation triggers 
 *                  and the bandwidthDistribution given from RRM.
 *****************************************************************************/
 void updateDlAggregateGbrOnScellStateChange( DLUEContext* ueDLContext_p,
                                                    UInt8 actDeactTrigger)
{
    InternalCellIndex pCellIndex = ueDLContext_p->internalCellIndex;
    InternalCellIndex sCellIndex = ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex;;
    UInt8 sCellBandWidthDist = 0; 
    UInt8 *gbrLcInfo_p = ueDLContext_p->gbrLcInfo;
    SInt8 gbrLcCount = ueDLContext_p->gbrLcCount;
    DlLcQosInfo *dlLcQosInfo_p = PNULL;
    UInt32 dlAggGbrOnScell = 0;
    UDouble32 absFactor = 0;
    UInt8 absCount = 0;

      
    /* EICIC +*/
    if (usableAbsCountNew_g[pCellIndex] > 0) 
    {   
        absCount = usableAbsCountNew_g[pCellIndex];
    }    
    else if (absCountNew_g[pCellIndex] > 0)
    {
        absCount = absCountNew_g[pCellIndex];
    }    
    else
    {
        absCount = 0;
    }    

    if (absCount)
    {
  
        if (NON_VICTIM_UE == ueDLContext_p->userType)
        {
            absFactor = (UDouble32) (NUM_BITS_ABS_PATTERN/ (UDouble32) (NUM_BITS_ABS_PATTERN - absCount));
        }    
        else
        {   
            absFactor =  (UDouble32) (NUM_BITS_ABS_PATTERN/ (UDouble32) (absCount));
        }    
    }
    else
    {
	    absFactor = ONE;
    }


    if(SCELL_DEACT_TRIGGER == actDeactTrigger)
    {
        while(gbrLcCount--)
        {
            if(ueDLContext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].logicalChannelId != INVALID_LCID)
            {
                dlLcQosInfo_p = &(ueDLContext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].dlLcQosInfo);
                sCellBandWidthDist = dlLcQosInfo_p->bandWidthDistribution[START_SCELL_INDEX];

                dlAggGbrOnScell = (((dlLcQosInfo_p->GBR / NUM_TICKS_PER_SECOND) *
                                            sCellBandWidthDist)/100 )*(absFactor);
                if (VICTIM_UE == ueDLContext_p->userType)
                {
                    dlAggregateGbrPerTTI_g[pCellIndex][VICTIM_UE] += dlAggGbrOnScell;
                    if(dlAggregateGbrPerTTI_g[sCellIndex][VICTIM_UE] < dlAggGbrOnScell)
                    {
                        dlAggregateGbrPerTTI_g[sCellIndex][VICTIM_UE] = 0;
                    }
                    else
                    {
                        dlAggregateGbrPerTTI_g[sCellIndex][VICTIM_UE] -= dlAggGbrOnScell;
                    }
                }
                else  if (NON_VICTIM_UE == ueDLContext_p->userType)
                {
                    dlAggregateGbrPerTTI_g[pCellIndex][NON_VICTIM_UE] += dlAggGbrOnScell;
                    if(dlAggregateGbrPerTTI_g[sCellIndex][NON_VICTIM_UE] < dlAggGbrOnScell)
                    {
                        dlAggregateGbrPerTTI_g[sCellIndex][NON_VICTIM_UE] = 0;
                    }
                    else
                    {
                        dlAggregateGbrPerTTI_g[sCellIndex][NON_VICTIM_UE] -= dlAggGbrOnScell;
                    }
                }
        }
    }
    }
    if(SCELL_ACT_TRIGGER == actDeactTrigger)
    {
        while(gbrLcCount--)
        {
            if(ueDLContext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].logicalChannelId != INVALID_LCID)
            {
                dlLcQosInfo_p = &(ueDLContext_p->logicalChannel[gbrLcInfo_p[gbrLcCount]].dlLcQosInfo);
                sCellBandWidthDist = dlLcQosInfo_p->bandWidthDistribution[START_SCELL_INDEX];

                dlAggGbrOnScell = (((dlLcQosInfo_p->GBR / NUM_TICKS_PER_SECOND) *
                            sCellBandWidthDist)/100 )*(absFactor);

                if (VICTIM_UE == ueDLContext_p->userType)
                {
                    if(dlAggregateGbrPerTTI_g[pCellIndex][VICTIM_UE] < dlAggGbrOnScell)
                    {
                        dlAggregateGbrPerTTI_g[pCellIndex][VICTIM_UE] = 0;
                    }
                    else
                    {
                        dlAggregateGbrPerTTI_g[pCellIndex][VICTIM_UE] -= dlAggGbrOnScell;
                    }
                    dlAggregateGbrPerTTI_g[sCellIndex][VICTIM_UE] += dlAggGbrOnScell;
                }
                else  if (NON_VICTIM_UE == ueDLContext_p->userType)
                {
                    if(dlAggregateGbrPerTTI_g[pCellIndex][NON_VICTIM_UE] < dlAggGbrOnScell)
                    {
                        dlAggregateGbrPerTTI_g[pCellIndex][NON_VICTIM_UE] = 0;
                    }
                    else
                    {
                        dlAggregateGbrPerTTI_g[pCellIndex][NON_VICTIM_UE] -= dlAggGbrOnScell;
                    }
                    dlAggregateGbrPerTTI_g[sCellIndex][NON_VICTIM_UE] += dlAggGbrOnScell;
                }
        }
      }
    }
}

/*****************************************************************************
 * Function Name  : scellStateMngrCheckQloadForActivation 
 * Inputs         : ueDLContext_p,sCellIndex
 * Outputs        :
 * Returns        : None
 * Description    : This function checks the ueQueueLoad with dlQLoadUpThresh 
 *                  for N consecutive instances maintanined by scellStateVariables.scellDeactThresCount 
 *                  also if ueQueueLoad is less than dlQLoadUpThresh then it will reset 
 *                  the scellStateVariables.scellActDeactThresCount.
 *****************************************************************************/
void scellStateMngrCheckQloadForActivation(DLUEContext* ueDLContext_p,ServingCellIndex sCellIndex)
{
    DLCAEventQueueNode * eventQNode_p = PNULL; 
    InternalCellIndex   pCellIndex      = 0;  
    UInt16 index                        = 0; 
    UInt8 isAlreadyConsidered = LTE_FALSE; 

    pCellIndex    = ueDLContext_p->internalCellIndex;
    /*spr 12463 fix start*/
    MacReconfigSchedulerParamsDL *schedulerParamsDL_p =
        &(macReconfigSchedulerParamsDL_gp[pCellIndex][schParamsIndexDL_g[pCellIndex]]);
    /*spr 12463 fix end*/
    /*check if the scellState is the configured and ueQueueLoad is less then dlQLoadUpThresh 
      increase the scellStateVariables.scellActDeactThresCount otherwise reset it ,To check activation qload
      threshold for N instances */
    if(ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellActThresCount < \
            schedulerParamsDL_p->caParams.actDeactTriggerCountThresh)
    {
        if((schedulerParamsDL_p->caParams.dlQLoadUpThresh < ueDLContext_p->ueQueueLoad) && \
            /*Code Review Comments Changes Start*/
            (dlCAScellUeQloadCheckList_g[sCellIndex].ueCount < MAX_DL_UE_SCHEDULED))
            /*Code Review Comments Changes End*/
        {
            for(index = 0; index < dlCAScellUeQloadCheckList_g[sCellIndex].ueCount;index ++)
            {  
                if((dlCAScellUeQloadCheckList_g[sCellIndex].dlCAUeQloadCountList[index].ueIndex == ueDLContext_p->ueIndex)\
                        && dlCAScellUeQloadCheckList_g[sCellIndex].dlCAUeQloadCountList[index].isAlreadyConsidered != LTE_FALSE)
                {
                    isAlreadyConsidered = LTE_TRUE;
                }
            }  
            if(isAlreadyConsidered == LTE_FALSE) 
            {
                /*spr 13019 change start*/
                ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellActThresCount++;
                /*spr 13019 change end*/
                dlCAScellUeQloadCheckList_g[sCellIndex].dlCAUeQloadCountList[index].ueIndex = ueDLContext_p->ueIndex;
                dlCAScellUeQloadCheckList_g[sCellIndex].dlCAUeQloadCountList[index].isAlreadyConsidered = LTE_TRUE;
                dlCAScellUeQloadCheckList_g[sCellIndex].ueCount++;  
            }
        }
        else
        {
         /*spr 13019 change start*/  
            ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellActThresCount = RESET_SCELL_QLOAD_THRESH_COUNT;
         /*spr 13019 change end*/  
        }
    }
    /* else if (ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellActDeactThresCount >= 
            schedulerParamsDL_p->caParams.actDeactTriggerCountThresh)*/
    else
    {
        /*Schedule an activation Mac CE if scellStateVariables.scellActThresCount has reached the 
          actDeactTriggerCountThresh as configured from RRM and reset the same*/   
         /*spr 13019 change start*/  
        ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellActThresCount = RESET_SCELL_QLOAD_THRESH_COUNT;
         /*spr 13019 change end*/  
        /*prepare and insert mac evnt Q node*/
        GET_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p,DLCAEventQueueNode);
        if(eventQNode_p != PNULL)
        {               
            eventQNode_p->ueId = ueDLContext_p->ueIndex;
            eventQNode_p->scellId = sCellIndex;
            eventQNode_p->dlCAQosEvent = DL_CA_EVENT_MAC_SCELL_ACT_CE_TRIGGER;
            if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueDLContext_p->internalCellIndex],(void *)eventQNode_p))
            {
                FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
            }
      }
   }
}
 
/*****************************************************************************
 * Function Name  : scellStateMngrCheckQloadForDeactivation 
 * Inputs         : ueDLContext_p,sCellIndex
 * Outputs        :
 * Returns        : None
 * Description    : This function checks the ueQueueLoad with dlQLoadDownThresh 
 *                  for N consecutive instances maintanined by scellStateVariables.scellDeactThresCount
 *                  also if ueQueueLoad is less than dlQLoadDownThresh then it will reset 
 *                  the scellStateVariables.scellDeactThresCount.
 *****************************************************************************/
void scellStateMngrCheckQloadForDeactivation(DLUEContext* ueDLContext_p,ServingCellIndex sCellIndex)
{
    DLCAEventQueueNode * eventQNode_p = PNULL;    
    InternalCellIndex   pCellIndex      = 0;  
    UInt16 index = 0;  
    UInt8 isAlreadyConsidered = LTE_FALSE; 
    DLCAMacFreeScellHarqBufferQueueNode *dlCAMacFreeScellHarqBufferQueueNode_p = PNULL;    
    ScellDeactTimerNode  *scellTimerNode_p = PNULL; 

    pCellIndex    = ueDLContext_p->internalCellIndex;
    /*spr 12463 fix start*/
    MacReconfigSchedulerParamsDL *schedulerParamsDL_p =
        &(macReconfigSchedulerParamsDL_gp[pCellIndex][schParamsIndexDL_g[pCellIndex]]);
    /*spr 12463 fix end*/
    /*check if the scellState is the Active and ueQueueLoad is greater then dlQLoadLowThresh 
      increase the scellStateVariables.scellDeactThresCount otherwise reset the it ,To check Deactivation qload
      threshold for N instances */
    if((ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellDeactThresCount\
                < schedulerParamsDL_p->caParams.actDeactTriggerCountThresh))
    {
        /* + SPR 14776 Changes */
        if((schedulerParamsDL_p->caParams.dlQLoadLowThresh >= ueDLContext_p->ueQueueLoad) && \
        /* - SPR 14776 Changes */
            /*Code Review Comments Changes Start*/
            (dlCAScellUeQloadCheckList_g[sCellIndex].ueCount < MAX_DL_UE_SCHEDULED))
            /*Code Review Comments Changes End*/
        {
            for(index = 0; index < dlCAScellUeQloadCheckList_g[sCellIndex].ueCount;index ++)
            {  
                if((dlCAScellUeQloadCheckList_g[sCellIndex].dlCAUeQloadCountList[index].ueIndex == ueDLContext_p->ueIndex)\
                        && dlCAScellUeQloadCheckList_g[sCellIndex].dlCAUeQloadCountList[index].isAlreadyConsidered != LTE_FALSE)
                {
                    isAlreadyConsidered = LTE_TRUE;
                }
            }  
            if(isAlreadyConsidered == LTE_FALSE) 
            {
                /*spr 13019 change start*/  
                ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellDeactThresCount++;
                /*spr 13019 change end*/
                dlCAScellUeQloadCheckList_g[sCellIndex].dlCAUeQloadCountList[index].ueIndex = ueDLContext_p->ueIndex;
                dlCAScellUeQloadCheckList_g[sCellIndex].dlCAUeQloadCountList[index].isAlreadyConsidered = LTE_TRUE;
                dlCAScellUeQloadCheckList_g[sCellIndex].ueCount++;  
            }
        }
        else
        {
            /*spr 13019 change start*/ 
            ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellDeactThresCount = RESET_SCELL_QLOAD_THRESH_COUNT;
            /*spr 13019 change end*/
        }
    }
    /* else if (ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellDeactThresCount >= 
            schedulerParamsDL_p->caParams.actDeactTriggerCountThresh)*/
    else
    {
        scellTimerNode_p  = ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellTimerNode_p;
        /*There will be one delta time before deactivation timer expiry when deactivation command sending is not beneficial 
          the Scell will not be deactivated if the remaining time to Deactivation timer expiry is below 
          the threshold value (dlDeactCommandThresh) configured by RRM.*/
       /*or Second case will be when Deactivation Timer is not started due to RInf but ueQueueLoad has reached actDeactTriggerCountThresh*/
       /*spr 12450 fix start*/
        if (((scellTimerNode_p) && (scellTimerNode_p->expiryTTI != INVALID_SCELL_DEACT_EXPIRY_TTI) && \
            ((scellTimerNode_p->expiryTTI - (getCurrentTick()+ PHY_DL_DELAY)) > \
            schedulerParamsDL_p->caParams.dlDeactCommandThresh)) || \
            (INVALID_SCELL_DEACT_TIMER == ueDLContext_p->scellDeactivationTimer))
        {/*spr 12450 fix end*/ 
            /*Schedule an activation Mac CE if scellStateVariables.scellDeactThresCount has reached the 
              actDeactTriggerCountThresh as configured from RRM and reset the same*/    
            /*prepare and insert mac evnt Q node*/
            GET_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p,DLCAEventQueueNode);
            if(eventQNode_p != PNULL)
            {               
                eventQNode_p->ueId = ueDLContext_p->ueIndex;
                eventQNode_p->scellId = sCellIndex;
                eventQNode_p->dlCAQosEvent = DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER;

                if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueDLContext_p->internalCellIndex],(void *)eventQNode_p))
                {
                    FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
                }
            }
            /*reset the local count variabes*/ 
            /*Here scellStateMngrStopScellDeactTimer will be called */
            scellStateMngrStopScellDeactTimer(ueDLContext_p->dlUeScellContext_p[sCellIndex], \
            ueDLContext_p->dlUeScellContext_p[sCellIndex]->internalCellIndex);
            /*scellTimerNode_p->expiryTTI = INVALID_SCELL_DEACT_EXPIRY_TTI;*/
        }
        else
        {
            ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellState = DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC;
            /*SPR 21948 Changes Start */
             /* Changes under spr 21762 have been reverted */
              /*SPR 21948 Changes End */
            /*prepare and insert node to reset the harq buffer of scell deactivated*/
            GET_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p,DLCAMacFreeScellHarqBufferQueueNode);
            if(dlCAMacFreeScellHarqBufferQueueNode_p != PNULL)
            {
                dlCAMacFreeScellHarqBufferQueueNode_p->ueId = ueDLContext_p->ueIndex;
                dlCAMacFreeScellHarqBufferQueueNode_p->scellId = sCellIndex;

                if(!ENQUEUE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp\
                    [ueDLContext_p->internalCellIndex],\
                    (void *)dlCAMacFreeScellHarqBufferQueueNode_p))
                {
                    FREE_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p);
                }
            }
        }
        /*spr 13019 change start*/
        ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellDeactThresCount = RESET_SCELL_QLOAD_THRESH_COUNT;
        /*spr 13019 change end*/
    }		
}
/*****************************************************************************
 * Function Name  : checkSecondTbValidFromTMMode 
 * Inputs         : ueDLContext_p,ServingCellIndex
 * Outputs        : 
 * Returns        : None
 * Description    : This function checks whether the tb 2 is valid or not for a particular  
 *                  maode of scell.
 *****************************************************************************/
UInt8 checkSecondTbValidFromTMMode(DLUEContext* ueDLContext_p,ServingCellIndex scellIndex)
{
    TransmissonMode tempTransmissionMode = TX_MODE_ATTACH_NOT_COMPLETE; 
    DLMIMOInfo* dlMIMOInfo_p            = PNULL;
    InternalCellIndex internalCellIndex = ueDLContext_p->dlUeScellContext_p[scellIndex]->internalCellIndex;

    dlMIMOInfo_p     = &(ueDLContext_p->dlUeScellContext_p[scellIndex]->ScelldlMIMOInfo);
    tempTransmissionMode = dlMIMOInfo_p->transmissionMode;
    if ((TX_MODE_3 == tempTransmissionMode)||
            (TX_MODE_4 == tempTransmissionMode)||                                       
            (((TX_MODE_7 == tempTransmissionMode) ||
              (TX_MODE_8 == tempTransmissionMode)) &&
             (1 < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
              ->cellParams_p->numOfTxAnteenas)))
    {
        if (dlMIMOInfo_p->dlRi == RANK_INDICATOR_1)
        {
            return LTE_FALSE; 
        }
        else
        {
            return LTE_TRUE;
        }
    }
    return LTE_FALSE;
}
/*****************************************************************************
 * Function Name  : scellStateMngrScheduleMacCEForDeactivationByMCSThres 
 * Inputs         : ueDLContext_p,internalCellIndex
 * Outputs        : 
 * Returns        : None
 * Description    : This function schedule the Mac CE for Deactivation after 
 *                  checking the dlDeactMCSLowThresh is greater than current mcs of scell.
 *****************************************************************************/
void scellStateMngrScheduleMacCEForDeactivationByMCSThres(DLUEContext* ueDLContext_p,InternalCellIndex internalCellIndex)
{
    DLCAEventQueueNode * eventQNode_p = PNULL;    
    InternalCellIndex   pCellIndex      = PCELL_SERV_CELL_IX;
    InternalCellIndex   sCellIndex      = INVALID_SERV_CELL_INDEX;
    UInt8 isTBTwoValid = LTE_FALSE;
    ScellDeactTimerNode  *scellTimerNode_p = PNULL; 
    DLCAMacFreeScellHarqBufferQueueNode *dlCAMacFreeScellHarqBufferQueueNode_p = PNULL;
    pCellIndex = ueDLContext_p->internalCellIndex; 

    sCellIndex = getServeCellIndexFromInternalCellId(ueDLContext_p,internalCellIndex);
    MacReconfigSchedulerParamsDL *schedulerParamsDL_p =
         &(macReconfigSchedulerParamsDL_gp[pCellIndex][schParamsIndexDL_g[pCellIndex]]);
         /* klockworks warning fix*/
    if ((schedulerParamsDL_p->caParams.dlDeactCommandThresh != INVALID_DL_DEACT_CMD_THLD) && 
                     (sCellIndex <= MAX_NUM_SCELL))
    {
        SCELLDLCQIInfo* scelldlCQIInfo_p = &ueDLContext_p->dlUeScellContext_p[sCellIndex]->ScelldlCQIInfo;
       
        isTBTwoValid = checkSecondTbValidFromTMMode(ueDLContext_p,sCellIndex);
        
        if((ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellState == DL_CA_SCELL_ACTIVE) && \
                ((schedulerParamsDL_p->caParams.dlDeactMCSLowThresh > scelldlCQIInfo_p->mcsIndexCodeWordOne)\
                 || (isTBTwoValid && (schedulerParamsDL_p->caParams.dlDeactMCSLowThresh > scelldlCQIInfo_p->mcsIndexCodeWordTwo)) ))
        {
            scellTimerNode_p  = ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellTimerNode_p;
            /*spr 12450 fix start*/
            if (((scellTimerNode_p) && (scellTimerNode_p->expiryTTI != INVALID_SCELL_DEACT_EXPIRY_TTI) && \
                ((scellTimerNode_p->expiryTTI - (getCurrentTick()+ PHY_DL_DELAY)) > schedulerParamsDL_p->caParams.dlDeactCommandThresh))||\
             /*or Second case will be when Deactivation Timer is not started due to RInf but ueQueueLoad has reached actDeactTriggerCountThresh*/
            (INVALID_SCELL_DEACT_TIMER == ueDLContext_p->scellDeactivationTimer))
            {/*spr 12450 fix end*/
                /*prepare and insert mac event Q node*/
                GET_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p,DLCAEventQueueNode);
                if(eventQNode_p != PNULL)
                {               
                    eventQNode_p->ueId = ueDLContext_p->ueIndex;
                    eventQNode_p->scellId = sCellIndex;
                    eventQNode_p->dlCAQosEvent = DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER;

                    if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueDLContext_p->internalCellIndex],\
                        (void *)eventQNode_p))
                    {
                        FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
                    }
                }
                /*reset the local count variabes*/ 
                /*Here scellStateMngrStopScellDeactTimer will be called */
                scellStateMngrStopScellDeactTimer(ueDLContext_p->dlUeScellContext_p[sCellIndex], \
                        ueDLContext_p->dlUeScellContext_p[sCellIndex]->internalCellIndex);
                /*scellTimerNode_p->expiryTTI = INVALID_SCELL_DEACT_EXPIRY_TTI;*/
            }
            else
            {
                ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellState = DL_CA_SCELL_DEACTIVATION_ON_GOING_VIA_MAC;
                /*SPR 21948 Changes Start */
                 /* Changes under spr 21762 have been reverted */
                  /*SPR 21948 Changes End */
                /*prepare and insert node to reset the harq buffer of scell deactivated*/
                GET_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p,DLCAMacFreeScellHarqBufferQueueNode);
                if(dlCAMacFreeScellHarqBufferQueueNode_p != PNULL)
                {
                    dlCAMacFreeScellHarqBufferQueueNode_p->ueId = ueDLContext_p->ueIndex;
                    dlCAMacFreeScellHarqBufferQueueNode_p->scellId = sCellIndex;

                    if(!ENQUEUE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp[\
                        ueDLContext_p->internalCellIndex],\
                        (void *)dlCAMacFreeScellHarqBufferQueueNode_p))
                    {
                        FREE_MEM_NODE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueueNode_p);
                    }
                }
            } 
            /*spr 13019 change start*/ 
            ueDLContext_p->dlUeScellContext_p[sCellIndex]->scellStateVariables.scellDeactThresCount = RESET_SCELL_QLOAD_THRESH_COUNT;
            /*spr 13019 change end*/
        }  
    }
}
/*****************************************************************************
 * Function Name  : scellStateMngrCheckQloadForActivationDeactivation 
 * Inputs         : ueDLContext_p,internalCellIndex
 * Outputs        : 
 * Returns        : None
 * Description    : This function checks ueQload with rrm configured qload threshold for 
 *                  scell activation/deactivation.
 *****************************************************************************/
void scellStateMngrCheckQloadForActivationDeactivation(DLUEContext* ueDLContext_p,InternalCellIndex internalCellIndex)
{
    ServingCellIndex scellIndex = START_SCELL_INDEX;
    InternalCellIndex   pCellIndex      = 0;
    pCellIndex    = ueDLContext_p->internalCellIndex;
    MacReconfigSchedulerParamsDL *schedulerParamsDL_p = &(macReconfigSchedulerParamsDL_gp[pCellIndex][schParamsIndexDL_g[pCellIndex]]);
    if (pCellIndex == internalCellIndex)
    {
        for(scellIndex = START_SCELL_INDEX; scellIndex < (MAX_NUM_SCELL + MAX_NUM_PCELL); scellIndex++)
        {
            if(ueDLContext_p->dlUeScellContext_p[scellIndex])
            {
                if(ueDLContext_p->dlUeScellContext_p[scellIndex]->scellState == DL_CA_SCELL_CONFIGURED)
                {
                    scellStateMngrCheckQloadForActivation(ueDLContext_p,scellIndex);
                }
                /*SPR 20699 and 20709 Fix Start */
                else if(
                        schedulerParamsDL_p->caParams.dlDeactCommandThresh != INVALID_DL_DEACT_CMD_THLD && 
                        ueDLContext_p->dlUeScellContext_p[scellIndex]->scellState == DL_CA_SCELL_ACTIVE )
                {
                    scellStateMngrCheckQloadForDeactivation(ueDLContext_p,scellIndex);
                }

                /*SPR 20699 and 20709 Fix End */
                /*check dlDeactCommandThresh and if it is invalid then reset the scellact/deact threshold count in scell context*/
                if(schedulerParamsDL_p->caParams.dlDeactCommandThresh == INVALID_DL_DEACT_CMD_THLD)
                {
                    ueDLContext_p->dlUeScellContext_p[scellIndex]->scellStateVariables.scellDeactThresCount = RESET_SCELL_QLOAD_THRESH_COUNT;
                }    
            }
        }
    }
}

/* Stage-III SCELL Deactivation Timer Related Changes Start */
/*****************************************************************************
 * Function Name  : scellStateMngrinitScellDeactTimer
 * Inputs         : numofCells - num of configured sCells
 * Outputs        :
 * Returns        : None
 * Description    : This function allocate memory for scellDeactExpiryTimerList_gp
 *                : which shall contain list of scells of UEs with their 
 *                : respective TTIs
 *****************************************************************************/
void scellStateMngrInitScellDeactTimer(UInt8 numOfCells)
{
    UInt8 cellCount = 0;
    UInt8 listIndex = 0;
    UInt16 macMaxNumOfUeSupported = 0;
    createMemPool(sizeof(scellDeactExpiryTimerList) * MAX_SCELL_DEACT_TIMER, numOfCells);
    for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {
        GET_MEM_FROM_POOL(scellDeactExpiryTimerList, scellDeactExpiryTimerList_gp[cellCount],
                sizeof(scellDeactExpiryTimerList) * MAX_SCELL_DEACT_TIMER, PNULL);

        for (listIndex = 0; listIndex < MAX_SCELL_DEACT_TIMER; listIndex++)
        {
            listInit(&(scellDeactExpiryTimerList_gp[cellCount] + listIndex)->expiryTimerList);
        }
    }

    /* Initialize mempool for timerNode. There shall be timer running for
     * each SCELL in UE. The mempool size shall be for maxNumOfUeSupported_g * MAX_NUM_SCELL */
    for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {   
        macMaxNumOfUeSupported = cellSpecificParams_g.\
                                 cellConfigAndInitParams_p[cellCount]->initParams_p->macMaxNumOfUeSupported;
        createMemPool(sizeof(ScellDeactTimerNode), (macMaxNumOfUeSupported * MAX_NUM_SCELL));
    }

    return;
}
/*****************************************************************************
 * Function Name  : scellStateMngrCleanUpScellDeactTimer
 * Inputs         : internalCellIndex
 * Outputs        :
 * Returns        : None
 * Description    : This function frees memory for scellDeactExpiryTimerList_gp
 *                  for the corresponding internalCellIndex passed
 *****************************************************************************/
void scellStateMngrCleanUpScellDeactTimer(InternalCellIndex internalCellIndex)
{
    if(scellDeactExpiryTimerList_gp[internalCellIndex])
    {
        freeMemPool(scellDeactExpiryTimerList_gp[internalCellIndex]);
    }
    return;
}
/*****************************************************************************
 * Function Name  : scellStateMngrGetmemScellTimerNode
 * Inputs         : dlUEContext_p,sCellArrayIndex
 * Outputs        :
 * Returns        : None
 * Description    : Function shall allocate memory for timer node
 *****************************************************************************/
void scellStateMngrGetmemScellTimerNode(DLUEContext *dlUEContext_p, ServingCellIndex sCellArrayIndex)
{
    ScellDeactTimerNode *scellTimerNode_p  = PNULL;
    DLUESCellContext    *scellContext_p    = PNULL;

    scellContext_p       = dlUEContext_p->dlUeScellContext_p[sCellArrayIndex];
    GET_MEM_FROM_POOL(ScellDeactTimerNode, scellTimerNode_p, sizeof(ScellDeactTimerNode), PNULL);
    if(scellTimerNode_p)
    {
        scellTimerNode_p->expiryTTI                     = INVALID_SCELL_DEACT_EXPIRY_TTI;
        scellTimerNode_p->sCellArrayIndex               = sCellArrayIndex;
        scellTimerNode_p->ueIndex                       = dlUEContext_p->ueIndex;
    }
    scellContext_p->scellStateVariables.scellTimerNode_p           = scellTimerNode_p; 
    scellContext_p->scellStateVariables.scellCycleTimerExpiryIndex = INVALID_SCELL_DEACT_EXPIRY_INDEX;
    return;
}
/*****************************************************************************
 * Function Name  : scellStateMngrStartSCellDeactTimer
 * Inputs         : dlUEContext_p, expiryTTI, sCellArrIndex, servCellIndex 
 * Outputs        :
 * Returns        : None
 * Description    : Function shall update the timernode and insert at new index
 *                  in scellDeactExpiryTimerList_gp based on expiry TTI 
 *****************************************************************************/
/*SPR 15909 fix start*/
void scellStateMngrStartSCellDeactTimer(DLUEContext *dlUEContext_p, tickType_t expiryTTI,
        /*SPR 15909 fix end*/
        ServingCellIndex sCellArrIndex,InternalCellIndex servCellIndex)
{
    ScellDeactTimerNode *scellTimerNode_p = PNULL;
    UInt16        expiryIndex             = ((expiryTTI) & (MAX_SCELL_DEACT_TIMER_MOD_VALUE)); 
    DLUESCellContext *scellContext_p      = dlUEContext_p->dlUeScellContext_p[sCellArrIndex];

    scellTimerNode_p = scellContext_p->scellStateVariables.scellTimerNode_p; 
    if(scellTimerNode_p)
    {
        scellContext_p->scellStateVariables.scellCycleTimerExpiryIndex = expiryIndex;
        scellTimerNode_p->expiryTTI                                    = expiryTTI;
        scellTimerNode_p->sCellArrayIndex                              = sCellArrIndex;
        scellTimerNode_p->ueIndex                                      = dlUEContext_p->ueIndex;
        listInsertNode(&(scellDeactExpiryTimerList_gp[servCellIndex] + expiryIndex)->expiryTimerList,
                &scellTimerNode_p->anchorNode);
    }        
}

/*****************************************************************************
 * Function Name  : scellStateMngrStopScellDeactTimer 
 * Inputs         : scellContext_p, servCellIndex 
 * Outputs        :
 * Returns        : None
 * Description    : Function shall delete the timernode
 *                  in scellDeactExpiryTimerList_gp based on TimerExpiryIndex
 *****************************************************************************/
void scellStateMngrStopScellDeactTimer(DLUESCellContext *scellContext_p, InternalCellIndex servCellIndex)
{
    UInt16                 expiryIndex           = 0;
    ScellDeactTimerNode   *scellTimerNode_p      = PNULL;

    scellTimerNode_p = scellContext_p->scellStateVariables.scellTimerNode_p;
    expiryIndex      = scellContext_p->scellStateVariables.scellCycleTimerExpiryIndex;
    /** SPR 15231 Changes Start **/
    if((scellTimerNode_p) && (scellTimerNode_p->expiryTTI != INVALID_SCELL_DEACT_EXPIRY_TTI)) 
        /** SPR 15231 Changes End **/
    {
        listDeleteNode(&(scellDeactExpiryTimerList_gp[servCellIndex] + expiryIndex)->expiryTimerList,
                (LTE_LIST_NODE *)scellTimerNode_p);
        scellTimerNode_p->expiryTTI                                    = INVALID_SCELL_DEACT_EXPIRY_TTI;
        scellTimerNode_p->sCellArrayIndex                              = INVALID_SERV_CELL_INDEX;
        scellContext_p->scellStateVariables.scellCycleTimerExpiryIndex = INVALID_SCELL_DEACT_EXPIRY_INDEX;
    }
    return;
}

/*****************************************************************************
 * Function Name  : scellStateMngrExpireAllScellDeactTimerOfCurrentTick 
 * Inputs         : globalTick, internalCellIndex 
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        :
 * Returns        : None
 * Description    : Function shall process all the timer nodes that are present
 *                  in the scellDeactExpiryTimerList_gp at the index which is
 *                  calculated on the basis of global tick received.
 *                  It shall trigger an event for all the expired scell timer
 *                  nodes.
 *                 This function shall be called in the last EL in every tick
 *****************************************************************************/
/*SPR 15909 fix start*/
 void scellStateMngrExpireAllScellDeactTimerOfCurrentTick(tickType_t globalTick,
        InternalCellIndex internalCellIndex)
    /*SPR 15909 fix end*/
{
    ScellDeactTimerNode *scellTimerNode_p  = PNULL;
    UInt16       expiryIndex               = 0; 
    UInt16       count                     = 0; 
    DLCAEventQueueNode * tempEventQNode_p  = PNULL;
    UInt8              sCellArrayIndex     = INVALID_SERV_CELL_INDEX;
    DLUEContext             *ueDLContext_p = PNULL;

    expiryIndex  = ((globalTick) & (MAX_SCELL_DEACT_TIMER_MOD_VALUE));
    if(PNULL == scellDeactExpiryTimerList_gp[internalCellIndex])  
    {
        return;
    }
    count        = listCount(&(scellDeactExpiryTimerList_gp[internalCellIndex] + expiryIndex)->expiryTimerList);    
    if (count)
    {    
        scellTimerNode_p = (ScellDeactTimerNode *)getListNode(
                &(scellDeactExpiryTimerList_gp[internalCellIndex] + expiryIndex)->expiryTimerList, 0);

        while(scellTimerNode_p &&
                (scellTimerNode_p->expiryTTI <= globalTick))
        {
            sCellArrayIndex  = scellTimerNode_p->sCellArrayIndex;
            ueDLContext_p    = dlUECtxInfoArr_g[scellTimerNode_p->ueIndex].dlUEContext_p;

            if((ueDLContext_p) && (ueDLContext_p->dlUeScellContext_p[sCellArrayIndex]))
            {
                ueDLContext_p->dlUeScellContext_p[sCellArrayIndex]\
                    ->scellStateVariables.scellCycleTimerExpiryIndex = INVALID_SCELL_DEACT_EXPIRY_INDEX;
                /*Now Trigger an event */
                GET_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p,DLCAEventQueueNode);
                if(tempEventQNode_p != PNULL)
                {
                    tempEventQNode_p->ueId    = ueDLContext_p->ueIndex;
                    tempEventQNode_p->scellId = sCellArrayIndex; 
                    tempEventQNode_p->dlCAQosEvent = DL_CA_EVENT_RRM_DEACTIVATION_TIMER_EXPIRE;

                    if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueDLContext_p->internalCellIndex],(void *)tempEventQNode_p))
                    {
                        FREE_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p);
                    }
                }
            }
            listDeleteNode(&(scellDeactExpiryTimerList_gp[internalCellIndex] + expiryIndex)->expiryTimerList,
                    (LTE_LIST_NODE *)scellTimerNode_p);
            scellTimerNode_p->expiryTTI        = INVALID_SCELL_DEACT_EXPIRY_TTI;
            scellTimerNode_p->sCellArrayIndex  = INVALID_SERV_CELL_INDEX;
            /*Will free memory in DL_CA_EVENT_SCELL_RELEASE event only in DL_CA_SCELL_INIT state.
             *FREE_MEM_SCELL_DEACT_TIMER_NODE(scellTimerNode_p);
             *Memory for timer node will be allocated only for event DL_CA_EVENT_RRC_UE_CREATE in DL_CA_SCELL_INIT state.
             */
            /*Move to next node in the same tick*/
            scellTimerNode_p = (ScellDeactTimerNode *)getListNode(
                    &(scellDeactExpiryTimerList_gp[internalCellIndex] + expiryIndex)->expiryTimerList, 0);
        }
    }
}

/*****************************************************************************
 * Function Name  : scellStateMngrProcessFreeHarqBufferQueue 
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : 
 * Returns        : None
 * Description    : This function free the harq for the deactivated scell.
 *****************************************************************************/
void scellStateMngrProcessFreeHarqBufferQueue(InternalCellIndex internalCellIndex)
{
    UInt16 qCount = 0;
    DLUEContext* dlUEContext_p = PNULL;
#ifdef FDD_CONFIG
    UInt8 subFrameNumber = 0;
    UInt16 dlHarqTimerArrayIndex = 0;
    DLHarqTimerNode* dlharqTimerNode_p = PNULL;
    DLHarqTimerEntityList* dlHarqEntityTimerList_p = PNULL; 
#endif
    SCELLDLHARQEntity* dlHarqEntity_p = PNULL;
    DLCAMacFreeScellHarqBufferQueueNode *scellFreeHarqQNode_p = PNULL;

    qCount = COUNT_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp[internalCellIndex]);

    while (qCount)
    {
        DEQUEUE_FREE_SCELL_HARQ_Q(dlCAMacFreeScellHarqBufferQueue_gp[internalCellIndex],(void **)&scellFreeHarqQNode_p); 
        if(PNULL ==scellFreeHarqQNode_p)
        {
            qCount--;
            continue;
        }
        if(INVALID_UE_ID <= scellFreeHarqQNode_p->ueId)
        {
            FREE_MEM_NODE_FREE_SCELL_HARQ_Q(scellFreeHarqQNode_p);
            qCount--;
            continue;
        }
        dlUEContext_p = dlUECtxInfoArr_g[scellFreeHarqQNode_p->ueId].dlUEContext_p;
        if(dlUEContext_p->dlUeScellContext_p[scellFreeHarqQNode_p->scellId] != PNULL)
        {
            dlHarqEntity_p = dlUEContext_p->dlUeScellContext_p[scellFreeHarqQNode_p->scellId]->ScelldlHarqContext_p;
            /*reset the harq processes mark them free*/
            /* SPR 12630 fix start */
            resetMACDLScellHarqEntity(&dlHarqEntity_p,
                    dlUEContext_p,dlUEContext_p->
                    dlUeScellContext_p[scellFreeHarqQNode_p->scellId]->internalCellIndex);
            /* SPR 12630 fix end */
            /*stop timer for each subframe*/
            /*ca-tdd changes start*/
#ifdef FDD_CONFIG
            for(subFrameNumber = 0; subFrameNumber < MAX_SUBFRAME; subFrameNumber++)
            {
                dlHarqTimerArrayIndex = dlHarqEntity_p->dlHarqTimerNodeArrrayIndex[subFrameNumber];
                /* SPR 12630 fix start */
                if(dlHarqTimerArrayIndex != INVALID_COUNT)
                    /* SPR 12630 fix end */
                {
                    dlHarqEntityTimerList_p =  &(dlHarqGlobals_g[internalCellIndex].dlHarqTimerEntityList[subFrameNumber]);
                    dlharqTimerNode_p = &(dlHarqEntityTimerList_p->dlHarqTimerNodeArr[dlHarqTimerArrayIndex]);
                    /* SPR 12630 fix start */
                    if(HARQ_TIMER_NODE_VALID == dlharqTimerNode_p->isValid)
                    {
                        /*stop timer, reset node params*/
                        dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;
                        dlharqTimerNode_p->harqProcessId = INVALID_HARQ_ID;
                        dlharqTimerNode_p->ueIndex = INVALID_UE_ID;  
                        dlHarqEntity_p->dlHarqTimerNodeArrrayIndex[subFrameNumber] = INVALID_COUNT;
                        /* SPR 12630 fix end */
                    }
                }
            }
#endif
        }
        /*ca-tdd changes end*/

        FREE_MEM_NODE_FREE_SCELL_HARQ_Q(scellFreeHarqQNode_p);
        qCount--;
    } 
}

/*****************************************************************************
 * Function Name  : scellStateMngrFreeAllUeScellTimerNode 
 * Inputs         : ueIndex - UE which is being deleted 
 *                  internalCellIndex - SCell-Index for ueIndex
 * Outputs        : 
 * Returns        : void 
 * Description    : This function will delete the Scell Timer Nodes while UE's Scell
 *                  Contex is being deleted.
 *****************************************************************************/
void scellStateMngrFreeAllUeScellTimerNode(UInt32 ueIndex, ServingCellIndex sCellArrIndex)
{
    ScellDeactTimerNode  *scellTimerNode_p   = PNULL;
    DLUEContext          *ueDLContext_p      = PNULL;
    DLUESCellContext     *dlUeScellContext_p = PNULL;

    ueDLContext_p  = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    if(ueDLContext_p->scellCount >= sCellArrIndex)
    {
        dlUeScellContext_p = ueDLContext_p->dlUeScellContext_p[sCellArrIndex];
        scellTimerNode_p   = dlUeScellContext_p->scellStateVariables.scellTimerNode_p;
        if(scellTimerNode_p)
        {   
            /* Remove the timer node from the tick which it was about to expire*/
            scellStateMngrStopScellDeactTimer(dlUeScellContext_p, ueDLContext_p->dlUeScellContext_p[sCellArrIndex]->internalCellIndex);
            FREE_MEM_SCELL_DEACT_TIMER_NODE(scellTimerNode_p);
            ueDLContext_p->dlUeScellContext_p[sCellArrIndex]->scellStateVariables.
                scellTimerNode_p = PNULL;
        }
    }
    return;
}
/*SPR 17561 FIX*/
#ifdef FDD_CONFIG
/*SPR 16026 Changes start*/ 
/*****************************************************************************
 * Function Name  : ScellStartDlCATimer 
 * Inputs         : dlUeContext_p - ueDlcontext 
 *                  dlHarqProcess_p - DLHARQProcess
 * Outputs        : 
 * Returns        : void 
 * Description    : This function will check the MAC CE Ack for scell activation 
 *                  /deactivation then spawn the Dlcatimer for the same.
 *****************************************************************************/
void ScellStartDlCATimer(DLUEContext *dlUeContext_p,DLHARQProcess* dlHarqProcess_p)
{
    /*SPR 15909 fix start*/
    tickType_t scheduledExpiryTick = getCurrentTick() + PHY_DL_DELAY; 
    /*SPR 15909 fix end*/
    UInt8  harqSubFrameNum = MAX_SUB_FRAME;
    /*SPR 17982 Start*/
    harqSubFrameNum  = (dlHarqProcess_p->dlharqRttExpiryTick - PHY_DL_DELAY - DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY) % 10;
    scheduledExpiryTick = dlHarqProcess_p->dlharqRttExpiryTick;
    /*SPR 17982 End*/

    dlCATimerStart(dlUeContext_p->ueIndex,harqSubFrameNum,\
            scheduledExpiryTick,dlHarqProcess_p->scellActBitmask,dlHarqProcess_p->scellDeactBitmask);
    /*as we have started the dlcatimer now reset the scell bitmask*/
    dlHarqProcess_p->scellActBitmask = 0;
    dlHarqProcess_p->scellDeactBitmask = 0;
}
/************************************************************************
 * Function Name  :  scellHarqFailureForMaxRetx 
 * Inputs         :  dlHarqProcess_p - ptr to harq 
 *                   ueContext_p - ptr to dl uecontext
 *                   maxReTxReached - Flag to indicate if HARQ Max ReTx
 *                                    have reached.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function checks the act/deact bitmask in harq 
 *                   failure and trigger the corresponding event. 
 ************************************************************************/
void scellHarqFailureForMaxRetx(
        DLHARQProcess* dlHarqProcess_p,
        DLUEContext *ueContext_p,
        UInt8 maxReTxReached )
{
    DLCAEventQueueNode *eventQNode_p = PNULL;

    if(dlHarqProcess_p->scellDeactBitmask)
    {
        /*prepare and insert mac evnt Q node*/
        GET_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p,DLCAEventQueueNode);
        if(eventQNode_p != PNULL)
        {    
            eventQNode_p->ueId = ueContext_p->ueIndex;
            eventQNode_p->scellId = dlHarqProcess_p->scellDeactBitmask >> 1;
            eventQNode_p->dlCAQosEvent = DL_CA_EVENT_DEACT_MAC_CE_HARQ_FAILURE;

            if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueContext_p->internalCellIndex],(void *)eventQNode_p))
            {
                FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
            }
        }
    }
    else if(dlHarqProcess_p->scellActBitmask)
    {  
        /*prepare and insert mac evnt Q node*/
        GET_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p,DLCAEventQueueNode);
        if(eventQNode_p != PNULL)
        {               
            eventQNode_p->ueId = ueContext_p->ueIndex;
            eventQNode_p->scellId = dlHarqProcess_p->scellActBitmask >> 1;
            if(maxReTxReached)
            {
                eventQNode_p->dlCAQosEvent = DL_CA_EVENT_ACT_MAC_CE_HARQ_FAILURE;
            }
            else
            {
                eventQNode_p->dlCAQosEvent = DL_CA_EVENT_ACT_MAC_CE_HARQ_NACK;
            }

            if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[ueContext_p->internalCellIndex],(void *)eventQNode_p))
            {
                FREE_MEM_NODE_DL_CA_EVENT_Q(eventQNode_p);
            }
        }
    }
} 
/*SPR 16026 changes end*/ 
#endif
/*SPR 17561 FIX*/
/*****************************************************************************
 * Function Name  : calculateFrameNumberForCATimerExpiry 
 * Inputs         : frameNumber - This indicates the sub frame number
 * Outputs        : None 
 * Returns        : This will return the sub frame number after processing 
 * Description    : This function will calculate the subframe number with 
 *                  and without overshoot for CA Timer expiry
 *****************************************************************************/
STATIC  UInt8 calculateFrameNumberForCATimerExpiry(UInt8 frameNumber)
{
#ifdef OVERSHOOT_CORRECTION
    if(!frameNumber)
    {
        frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME -1;
    }
    else
    {
        frameNumber--;
    }
#else
    frameNumber++;
    if (frameNumber == NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME)
    {
        frameNumber = 0;
    }
#endif
    return frameNumber;
}

/*****************************************************************************
 * Function Name  : enqueueInDLCAEventQueue 
 * Inputs         : dlCATimerNode_p - This indicates the CA Timer node to be
 *                  expired,
 *                  dlUeContext_p - This indicates the downlink UE Context
 * Outputs        : EventQ node is inserted in DL CA event queue
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will check for scell status and insert in the
 * queue 
 *****************************************************************************/
STATIC  MacRetType enqueueInDLCAEventQueue(DLCATimerNode* dlCATimerNode_p,
        DLUEContext* dlUeContext_p
        )
{
    DLCAEventQueueNode * tempEventQNode_p = PNULL;

    if (dlCATimerNode_p->dlCAScellState == DL_CA_SCELL_ACTIVE)
    {
        /*prepare and insert node in mac event q*/
        GET_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p,DLCAEventQueueNode);
        if(tempEventQNode_p == NULL)
        {
            return MAC_FAILURE;
        }
        tempEventQNode_p->ueId = dlCATimerNode_p->ueIndex;
        tempEventQNode_p->scellId = dlCATimerNode_p->scellId;
        tempEventQNode_p->dlCAQosEvent = DL_CA_EVENT_ACTIVATION_TIMER_EXPIRE;

        if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[dlUeContext_p->internalCellIndex],(void *)tempEventQNode_p))
        {
            FREE_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p);
        }
    }
    else if (dlCATimerNode_p->dlCAScellState == DL_CA_SCELL_DEACTIVATED)
    {
        /*prepare and insert node in mac event q*/
        GET_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p,DLCAEventQueueNode);
        if(tempEventQNode_p == NULL)
        {
            return MAC_FAILURE;
        }
        tempEventQNode_p->ueId = dlCATimerNode_p->ueIndex;
        tempEventQNode_p->scellId = dlCATimerNode_p->scellId;
        tempEventQNode_p->dlCAQosEvent = DL_CA_EVENT_DEACTIVATION_TIMER_EXPIRE;

        if(!ENQUEUE_DL_CA_EVENT_Q(dlCAEventQueue_gp[dlUeContext_p->internalCellIndex],(void *)tempEventQNode_p))
        {
            FREE_MEM_NODE_DL_CA_EVENT_Q(tempEventQNode_p);
        }
    }
    return MAC_SUCCESS;
}    
