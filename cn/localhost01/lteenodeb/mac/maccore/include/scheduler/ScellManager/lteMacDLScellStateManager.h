/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLScellStateManager.h, 2014/02/24 17:15:42 gur29743 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacDLSchedulerStrategy.h,v $
 * Revision 1.0 2014/02/24 17:15:42 gur29743
 *
 ****************************************************************************/

#ifndef LTE_MAC_DL_SCELL_STATE_MANAGER_H
#define LTE_MAC_DL_SCELL_STATE_MANAGER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacDLUEContext.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacDLQueueLoadMgr.h"
#include "lteMacTBSize.h"
#include "lteMacTimingAdvanceMgr.h"
#include "lteMacUEConfMgr.h"
#include "lteCircQueue.h"
#include "lteMacDLSpsScheduler.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
 /*SPR 14049 change start*/
#define MAX_SUB_FRAME_TDD_OFFSET 20
 /*SPR 14049 change end*/
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*ca event queue to process Scell state machine.*/
extern DL_CA_EVENT_QUEUE_TYPE * dlCAEventQueue_gp[MAX_NUM_CELL];
/*ca MAC CE queue */
extern DL_CA_MAC_CE_QUEUE_TYPE * dlCAMacCEQueue_gp[MAX_NUM_CELL];
/*pending ca Mac ce queue*/
extern DL_CA_MAC_CE_QUEUE_TYPE * pendingDlCAMacCEQueue_gp[MAX_NUM_CELL];
/*spr 11102 fix*/
/*pending Failure ca Mac ce queue*/
extern DL_CA_MAC_CE_QUEUE_TYPE * pendingFailureDlCAMacCEQueue_gp[MAX_NUM_CELL];
/*phase-3 chnages*/
extern DL_CA_MAC_FREE_SCELL_HARQ_QUEUE_TYPE* dlCAMacFreeScellHarqBufferQueue_gp[MAX_NUM_CELL];
/* Mem-leak Changes Start */
extern scellDeactExpiryTimerList* scellDeactExpiryTimerList_gp[MAX_NUM_CELL];
/* Mem-leak Changes End */

typedef struct DLCAEventQueueNodeT
{
    UInt16            ueId;
    ServingCellIndex  scellId;
    DLCAQosEvent      dlCAQosEvent;
}DLCAEventQueueNode;

typedef struct DLCAMacCEQueueNodeT
{
    UInt16            ueId;
    InternalCellIndex scellId;
    DLCAQosEvent      dlCAQosEvent;
}DLCAMacCEQueueNode;
/*phase-3 changes start*/
typedef struct DLCAMacFreeScellHarqBufferQueueNodeT
{
    UInt16            ueId;
    InternalCellIndex scellId;
}DLCAMacFreeScellHarqBufferQueueNode;
/*phase-3 changes end*/
typedef struct DLCATimerEntityListT
{
    /* The count will contain the number of valid field in the array*/
    UInt16 count;
    /* This array will store the harqTimerNode */  
    DLCATimerNode dlCATimerNodeArr[MAX_UE_SUPPORTED];
}DLCATimerEntityList;

typedef enum ScellActDeactTriggerT                
{                                                  
    SCELL_ACT_TRIGGER,                        
    SCELL_DEACT_TRIGGER,                     
    INVALID_TRIGGER                                
}ScellActDeactTrigger;                            
/*ca-tdd changes start*/
typedef enum TddSubFrameConfigT
{
    SUB_FRAME_CONFIG_0,
    SUB_FRAME_CONFIG_1,
    SUB_FRAME_CONFIG_2,
    SUB_FRAME_CONFIG_3,   
    SUB_FRAME_CONFIG_4,   
    SUB_FRAME_CONFIG_5,
    SUB_FRAME_CONFIG_6,    
    SUB_FRAME_CONFIG_MAX    
}TddSubFrameConfig;
/*ca-tdd changes end*/
typedef struct DLCAGlobalsT
{
        /* this array will store the harqs of UE on per subframe basis.*/
/*ca-tdd changes start*/
#ifdef FDD_CONFIG
        DLCATimerEntityList dlCATimerEntityList[MAX_SUB_FRAME];
#else
        DLCATimerEntityList dlCATimerEntityList[MAX_SUB_FRAME_TDD_OFFSET];
#endif
/*ca-tdd changes end*/
}DLCAGlobals;
extern DLCAGlobals dlCAGlobals_g[MAX_NUM_CELL];
typedef void (* DlCAScellStateFunctionPtr)(DLCAEventQueueNode *);

extern DlCAScellStateFunctionPtr dlCAScellStateEventArr_g[MAX_NUM_DL_CA_SCELL_STATE][MAX_NUM_DL_CA_EVENT];
/*global param to store Ue qload on scheduled scell*/
typedef struct DLCAUeQloadCountListT 
{
    UInt16 ueIndex;
    UInt8  isAlreadyConsidered;
}DLCAUeQloadCountList;
typedef struct DLCAScellUeQloadCheckListT
{
    UInt16 ueCount;
    DLCAUeQloadCountList dlCAUeQloadCountList[MAX_DL_UE_SCHEDULED];
}DLCAScellUeQloadCheckList;
/* + SPR 17439 */
void initDlCAScellStateEventArr(void);
/* - SPR 17439 */
void processdlCAEventQueue(InternalCellIndex internalCellIndex,
                           UInt16 dlCAEventQueueCount);
UInt8 dlCAScellStateManagerIsScellActive(UInt16 ueIndex,UInt8 servCellIdx);
UInt8 dlCAScellStateManagerIsScellActiveAtTti(UInt16 ueIndex,UInt8 servCellIdx);

#define IS_SCELL_ACTIVE(ueIndex,servCellIdx) dlCAScellStateManagerIsScellActive(ueIndex,servCellIdx)
#define IS_SCELL_ACTIVE_AT_TTI(ueIndex,servCellIdx) dlCAScellStateManagerIsScellActiveAtTti(ueIndex,servCellIdx)
/* Stage-III SCELL Deactivation Timer Related Changes Start */
#define UPDATE_SCELL_DEACT_TIMER(dlUeScellContext_p, scellDeactivationTimer, currentGlobalTTITickCount) \
dlUeScellContext_p->scellSchedulingExpiryTTI = currentGlobalTTITickCount + scellDeactivationTimer
#define INVALID_SCELL_DEACT_TIMER        0
#define INVALID_SCELL_DEACT_EXPIRY_TTI   INVALID_SCELL_DEACT_TIMER
#define RESET_SCELL_QLOAD_THRESH_COUNT   0
/*SCELL_DEACT_NEXT_TICK is the increment for next tick from present current tick*/
#define SCELL_DEACT_NEXT_TICK   1
/* Stage-III SCELL Deactivation Timer Related Changes End */

/* + SPR 22352 fix */
/* set the SCELL_ACT_BIT/SCELL_DEACT_BIT in the bitmap of UE context 
 * based on the event triggered */
#define SET_SCELL_ACT_DEACT_BIT(ueIndex,scellId,event) \
{\
dlUECtxInfoArr_g[ueIndex].dlUEContext_p->TaDrxScellBitmap |= scellId << 4 ;\
if(DL_CA_EVENT_MAC_SCELL_ACT_CE_TRIGGER == event)\
dlUECtxInfoArr_g[ueIndex].dlUEContext_p->TaDrxScellBitmap |= SCELL_ACT_BIT;\
else if(DL_CA_EVENT_MAC_SCELL_DEACT_CE_TRIGGER == event)\
dlUECtxInfoArr_g[ueIndex].dlUEContext_p->TaDrxScellBitmap |= SCELL_DEACT_BIT;\
}
/* - SPR 22352 fix */


MacRetType dlCATimerStart(
                           UInt16  ueIndex,
                           UInt32  frameNumber,
                           /* SPR 15909 fix start */
                           tickType_t  dlCAExpiryTick,
                            /* SPR 15909 fix end */
                           UInt8   scellActBitmask,
                           UInt8   scellDeActBitmask
                           );

MacRetType expireAllNodeFromCATimerList (
                           UInt8 loopOfNumOfSubFrames,
                           /* +- SPR 17777 */
#ifdef FDD_CONFIG
                           UInt32 currentSubFrame,
                           UInt8 delaySubFrame,
#endif
                           InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
void dlCAScellStateManagerRegenerateMacCEEvent(InternalCellIndex internalCellIndex,DLCAMacCEQueueNode* node_p);

/*SPR 17561 FIX*/
#ifdef FDD_CONFIG
/*SPR 16026 changes start*/
 void checkScellActDeactBitmask(
                     DLHARQProcess* dlHarqProcess_p,
                     DLUEContext *ueContext_p
                      );
/*SPR 16026 changes end*/
#else
 void checkScellActDeactBitmask(
                     DLHARQProcess* dlHarqProcess_p,
                     DLUEContext *ueContext_p,
                     UInt8 harqSubFrameNum,
                     UInt32 scheduledExpiryTick);
#endif
/*SPR 17561 FIX*/
void initDLCATimerArray(UInt8 numOfCells);

/* Stage-III SCELL Deactivation Timer Related Changes Start */
/* SPR 15909 fix start */
extern  void scellStateMngrExpireAllScellDeactTimerOfCurrentTick(tickType_t globalTick, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */
extern void scellStateMngrStopScellDeactTimer(DLUESCellContext *scellContext_p, InternalCellIndex servCellIndex);
/* SPR 15909 fix start */
extern void scellStateMngrStartSCellDeactTimer(DLUEContext *dlUEContext_p,tickType_t expiryTTI,ServingCellIndex scellArrIndex,InternalCellIndex servCellIndex);
/* SPR 15909 fix end */
extern void scellStateMngrInitScellDeactTimer(UInt8 numOfCells);
extern void scellStateMngrCleanUpScellDeactTimer(InternalCellIndex internalCellIndex);

void scellStateMngrGetmemScellTimerNode(DLUEContext *dlUEContext_p, ServingCellIndex sCellArrIndex);
extern DLCAScellUeQloadCheckList dlCAScellUeQloadCheckList_g[MAX_NUM_CELL];
/* Stage-III SCELL Deactivation Timer Related Changes End */

void processScellActivInDeactivation(DLCAEventQueueNode * eventQNode_p);
void processUeCreateInInit(DLCAEventQueueNode * eventQNode_p);
void processRrmDeactivInInit(DLCAEventQueueNode * eventQNode_p);
void processRrmActivInInit(DLCAEventQueueNode * eventQNode_p);
void processRrmDeactivInConfigured(DLCAEventQueueNode * eventQNode_p);
void processRrmActivInConfigured(DLCAEventQueueNode * eventQNode_p);
void processMacScellActivInConfigured(DLCAEventQueueNode * eventQNode_p);
void processRrmActivInActivationOnGoing(DLCAEventQueueNode * eventQNode_p);
void processRrmActivInActivation(DLCAEventQueueNode * eventQNode_p);
void processScellDeactivInActivation(DLCAEventQueueNode * eventQNode_p);
void processMacScellDeactivMacCESentInActivation(DLCAEventQueueNode * eventQNode_p);
void processMacDeactTimerExpInActivation(DLCAEventQueueNode * eventQNode_p);
void processRrmScellDeactivMacCESentInActivation(DLCAEventQueueNode * eventQNode_p);
void processInvalidEvent(DLCAEventQueueNode * eventQNode_p);
void processInvalidEventStateTrigger(DLCAEventQueueNode * eventQNode_p);
void processRrmDeactivInActivationOnGoing(DLCAEventQueueNode * eventQNode_p);
void processRrmDeactivInDeactivationOnGoingViaMac(DLCAEventQueueNode * eventQNode_p);
void processMacDeactTimerExpInDeactivationOnGoingViaMac(DLCAEventQueueNode * eventQNode_p);
void processMacDeactTimerExpInDeactivationOnGoingViaRrm(DLCAEventQueueNode * eventQNode_p);
void processRrmActivInDeactivationOnGoingViaRrm(DLCAEventQueueNode * eventQNode_p);
void processReconfigApiIndicationInConfigured(DLCAEventQueueNode * eventQNode_p);
void processScellRelToStateInit(DLCAEventQueueNode * eventQNode_p);
/* Stage-III SCELL Deactivation Timer Related Changes Start */
/* Mem-leak Changes Start */
void scellStateMngrFreeAllUeScellTimerNode(UInt32 ueindex, InternalCellIndex internalCellIndex);
/* Mem-leak Changes End */
void scellStateMngrCheckQloadForActivation(DLUEContext* ueDLContext_p,ServingCellIndex internalCellIndex);
void scellStateMngrCheckQloadForDeactivation(DLUEContext* ueDLContext_p,ServingCellIndex sCellIndex);
void scellStateMngrScheduleMacCEForDeactivationByMCSThres(DLUEContext* ueDLContext_p,InternalCellIndex internalCellIndex);
void scellStateMngrCheckQloadForActivationDeactivation(DLUEContext* ueDLContext_p,InternalCellIndex internalCellIndex);
void processMacRRMDeactTimerExpInDeactivationOnGoingViaMac(DLCAEventQueueNode * eventQNode_p);
void scellStateMngrProcessFreeHarqBufferQueue(InternalCellIndex internalCellIndex);
/*SPR 17561 FIX*/
#ifdef FDD_CONFIG
/*SPR 16026 changes start*/
void scellHarqFailureForMaxRetx(DLHARQProcess* dlHarqProcess_p,DLUEContext *ueContext_p,UInt8 maxreTxReached);
void ScellStartDlCATimer(DLUEContext *dlUeContext_p,DLHARQProcess* dlHarqProcess_p);
/*SPR 16026 changes end*/
#endif
/*SPR 17561 FIX*/
#ifdef TDD_CONFIG
UInt32 getSCellStateChangeTddTimerOffset(UInt8 cellIndex);
#endif
/* Stage-III SCELL Deactivation Timer Related Changes End */
#endif  /* LTE_MAC_DL_SCELL_STATE_MANAGER_H */
