/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDRXMgr.h,v $
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the data type information that are
 *  used by the DRX manager
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacDRXMgr.h,v $
****************************************************************************/
#ifndef LTE_MAC_DRX_MGR_H
#define LTE_MAC_DRX_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#ifdef DL_UL_SPLIT
#include "lteCircQueue.h"
#endif
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define MAX_DRX_ONDURATION_TIMER_LIST_SIZE                      16
/*REL_10_CR_CHANGES_START*/
#define MAX_DRX_INACTIVITY_TIMER_LIST_SIZE                      23 
/*REL_10_CR_CHANGES_END*/
#define MAX_DRX_SHORT_CYCLE_TIMER_LIST_SIZE                     16
#define MAX_DRX_LONG_CYCLE_TIMER_LIST_SIZE                      16
#define MAX_DRX_RETRANSMISSION_TIMER_LIST_SIZE                  8
#define MAX_DRX_ONDURATION_TIMER_VALUE                          15
/*REL_10_CR_CHANGES_START*/
#define MAX_DRX_INACTIVITY_TIMER_VALUE                          22
/*REL_10_CR_CHANGES_END*/
#define MAX_DRX_SHORT_CYCLE_TIMER_VALUE                         15
#define MAX_DRX_LONG_CYCLE_TIMER_VALUE                          15
#define MAX_DRX_START_OFFSET                                    2560
#define MAX_DRX_RETRANSMISSION_TIMER_VALUE                      7 
#define MAX_NODES_SPLIT                                         (MAX_LOGICAL_CHANNEL * MAX_UE_SUPPORTED) 


/* SPR 99999 CQI with 255 UE Fix start */
/* + Coverity 40825 */
#define MAX_CQI_NODES_SPLIT                                      MAX_UE_SUPPORTED
/* - Coverity 40825 */
/* SPR 99999 CQI with 255 UE Fix End */

/*START : DRX_MAC_CE*/
#define MAX_DRX_TIMER                                           4096
#define MAX_DRX_TIMER_MOD_VALUE                                 4095
/*END   : DRX_MAC_CE*/
#define CHECK_UE_DRX_STATE(drxContext_p)                        \
        drxContext_p->drxStateVariables.drxCurrentState

#define SET_UE_DRX_STATE(drxContext_p, ulUEContext_p, state)    \
        drxContext_p->drxStateVariables.drxCurrentState = state;\
        ulUEContext_p->drxCurrentState = state 

#define SET_UE_DRX_SUB_STATE(drxContext_p, subState)        \
        drxContext_p->drxStateVariables.drxSubState = subState

#define SET_UE_DRX_NEXT_CYCLE(drxContext_p, nextCycle)      \
        drxContext_p->drxStateVariables.drxNextCycle = nextCycle

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*CA Changes start  */
extern DL_DRX_ON_OFF_TRIGGER_TYPE* dlDRXOnOffTriggerQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */
extern UInt16 drxInactivityTimerConfig_g[MAX_DRX_INACTIVITY_TIMER_LIST_SIZE];
extern UInt16 drxOnDurationTimerConfig_g[MAX_DRX_ONDURATION_TIMER_LIST_SIZE];
extern UInt16 drxShortCycleTimerConfig_g[MAX_DRX_SHORT_CYCLE_TIMER_LIST_SIZE];
extern UInt16 drxLongCycleTimerConfig_g[MAX_DRX_LONG_CYCLE_TIMER_LIST_SIZE];
extern UInt8 drxRetransmissionTimerConfig_g
                            [MAX_DRX_RETRANSMISSION_TIMER_LIST_SIZE];
extern void initDRXMgr(UInt8 numCells);

/* ++coverity--530-54624*/
extern void cleanupDRXQueue(InternalCellIndex internalCellIndex);
extern void deInitDRXQueue(InternalCellIndex internalCellIndex);
extern void cleanAndDeInitDRXQueue(InternalCellIndex internalCellIndex);
/* ++coverity--530-54624*/
/* SPR 5599 changes start (ZIP ID 129059) */
extern UInt8 getTddConfig(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/* This enum describes the DRX Cycle of the UE which could be Short DRX cycle or
 * Long DRX cycle */
typedef enum DRXCycleT
{
    DRX_CYCLE_INVALID = 0,
    DRX_SHORT_CYCLE,
    DRX_LONG_CYCLE
}DRXCycle; 

/* This enum describes the DRX Sub state of the UE. 
   If the UE DRX state is DRX_STATE_OFF, either DRX subState of the UE is 
   DRX_ON_DURATION_TIMER_RUNNING or DRX_INACTIVITY_TIMER_RUNNING 
   If the UE DRX state is DRX_STATE_ON, either DRX subState os the UE is 
   DRX_SHORT_CYCLE_TIMER_RUNNING or DRX_LONG_CYCLE_TIMER_RUNNING */
typedef enum DRXSubStateT
{
    DRX_SUB_STATE_INVALID = 0,
    DRX_ONDURATION_TIMER_RUNNING,
    DRX_SHORT_CYCLE_TIMER_RUNNING,
    DRX_LONG_CYCLE_TIMER_RUNNING,
    DRX_INACTIVITY_TIMER_RUNNING
}DRXSubState;

/*START : DRX_MAC_CE*/
typedef enum DRXTimerNodeTypeT
{
    DRX_CYCLE_TIMER = 0,
    DRX_MAC_CE_TIMER
}DRXTimerNodeType;
/*END   : DRX_MAC_CE*/

/* This enum describes the triggers that will be sent by the DRX manager for 
 * the DL and UL scheduler */
typedef enum DRXOnOffTriggerT
{
    DRX_TRIGGER_INVALID = 0,
    DRX_ON_TO_DRX_OFF,
    DRX_OFF_TO_DRX_ON,
    DRX_OFF_TO_DRX_OFF
}DRXOnOffTrigger; 

/* This structure contains parameters that will be send to DL/ UL scheduler to 
 * send DRX ON/ OFF triggers */
typedef struct DRXOnOffTriggerNodeT
{
    LTE_LIST_NODE      anchorNode;
    UInt16             ueIndex;
    /*SPR 15909 fix start*/
    tickType_t             nextOnDurationTTI;
    /*SPr 15909 fix end*/
    DRXOnOffTrigger    onOffTrigger;
}DRXOnOffTriggerNode;

/* This structure contains parameters that will be send to DRX manager by the 
 * DL and UL schedulers when new data is scheduled for a UE */
typedef struct DRXEventTriggerNodeT
{
    LTE_LIST_NODE    anchorNode;
    UInt16           ueIndex;
    /* SPR 15909 fix start */
    tickType_t           globalTTI;
    /* SPR 15909 fix end */
}DRXEventTriggerNode;

#ifdef TDD_CONFIG
typedef struct TDDDRXSFToTimerMapT
{ 
    UInt8    timerValue[10];  /* num of PDCCH SFs from a particular SF (<10) */
}TDDDRXSFToTimerMap;

typedef struct TDDDRXTimerConvMapT
{
    UInt8                  numPDCCHSF;  /* Num PDCCH SFs in a System Frame of 
                                           a particular TDD config */ 
    TDDDRXSFToTimerMap     tddDRXSFToTimerMap[10];    
}TDDDRXTimerConvMap; 
#endif
/*START : DRX_MAC_CE*/
typedef struct drxMacCeExpiryTimerListT
{
    LTE_LIST expiryTimerList;
}drxExpiryTimerList;
/*END   : DRX_MAC_CE*/

/* DRX Logging related data types */
/* This enum contains the possible DRX events. These enums are used for logging
 * purpose and dont directly map to the DRX events in the DRX state machine */
typedef enum DRXLogEventsT
{
    DRX_EVENT_INVALID = 0,
    DRX_EVENT_DL_DATA_SCHEDULED,
    DRX_EVENT_UL_DATA_SCHEDULED,
    DRX_EVENT_INACTIVITY_TIMER_STARTED,
    DRX_EVENT_INACTIVITY_TIMER_STOPPED,
    DRX_EVENT_INACTIVITY_TIMER_EXPIRED,
    DRX_EVENT_ONDURATION_TIMER_STARTED,
    DRX_EVENT_ONDURATION_TIMER_STOPPED,
    DRX_EVENT_ONDURATION_TIMER_EXPIRED,
    DRX_EVENT_SHORT_CYCLE_TIMER_STARTED,
    DRX_EVENT_SHORT_CYCLE_TIMER_STOPPED,
    DRX_EVENT_SHORT_CYCLE_TIMER_EXPIRED,
    DRX_EVENT_LONG_CYCLE_TIMER_STARTED,
    DRX_EVENT_LONG_CYCLE_TIMER_STOPPED,
    DRX_EVENT_LONG_CYCLE_TIMER_EXPIRED,
/*START : DRX_MAC_CE*/
    DRX_EVENT_MAC_CE_TIMER_STARTED,
    DRX_EVENT_MAC_CE_TIMER_STOPPED,/*5352 Removed RESTARTED enum*/
    DRX_EVENT_MAC_CE_TIMER_EXPIRED
/*END   : DRX_MAC_CE*/
}DRXLogEvents;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Functions declaration
 ****************************************************************************/
/* SPR 15909 fix start */
tickType_t tddConvertTimerVal(UInt16  timerVal, tickType_t globalTTI, InternalCellIndex internalCellIndex);
 /* SPR 15909 fix start */
/* + SPR 22243 Fix*/
extern DRX_Q_TYPE*  pdcchPdschFailureDRXQueue_gp[MAX_NUM_CELL];
/* - SPR 22243 Fix*/
#endif /* LTE_MAC_DRX_MGR_H */
