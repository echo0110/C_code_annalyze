/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacUtilityAndSharedStructs.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file defines the Utility functionality and shared 
                        struct
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacUtilityAndSharedStructs.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.4  2009/08/03 09:40:12  gur20052
 * Fix for UT_LOGS
 *
 * Revision 1.3  2009/07/20 08:41:52  gur20052
 * changed the signature of logint
 *
 * Revision 1.2  2009/07/17 11:16:11  gur11912
 * Added MACRO to reset CCE based on aggregation level
 *
 * Revision 1.1  2009/06/02 08:10:52  gur20052
 * Add initial version
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_UTILITY_AND_SHARED_STRUCTS_H  
#define LTE_MAC_UTILITY_AND_SHARED_STRUCTS_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacExecutionLegs.h"
#include "lteMacDLUEContext.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*  This Macro defines the Max Memory Allocation for UL - DL Nodes for PFS Scheduler */
#define NUM_UES_COMPUTED_ONE_TICK  (MAX_DL_UE_SCHEDULED + MAX_UL_UE_SCHEDULED + 1)
#define SHARED_STRUCTS "shared_struct"
/*pdcch allocation For scell changes start*/
#define IS_CA_ENABLED() (macParams_g.isCASupported)

#define CHECK_CA_STATE_AND_OTHER_CELL_STATE(internalCellIndex) \
    ((IS_CA_ENABLED()) &&((cellSpecificParams_g.\
      cellConfigAndInitParams_p[!(internalCellIndex)]) &&\
       ((CELL_PHY_RUNNING == cellSpecificParams_g.\
            cellConfigAndInitParams_p[!(internalCellIndex)]->cellState) ||\
        (CELL_PHY_RUNNING_CONFIG_RECVD == cellSpecificParams_g.\
                 cellConfigAndInitParams_p[!(internalCellIndex)]->cellState))))\

#define CHECK_CELL_STATE(internalCellIndex) \
    ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex])\
       && ((CELL_PHY_RUNNING == cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->cellState) ||\
        (CELL_PHY_RUNNING_CONFIG_RECVD == cellSpecificParams_g.\
                 cellConfigAndInitParams_p[internalCellIndex]->cellState)))

/* + Critical section changes */
/******** By Default EVENT FDs are used for sync threads. Mutex & semaphores are 
  implemented but not fully tested. **********/
/* if Semaphores are used to sync ELs */
#ifdef SEM_SYNC
#define CHECK_AND_WAIT_FOR_OTHER_CELLS_MUX(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
        /* SPR 18122 Changes Start*/ \
        semWaitDefault(&ev_fd_mux_g[!(internalCellIndex)][MODULO(getCurrentTick(),2)]);\
        /* SPR 18122 Changes End*/ \
    }\
}

#define CHECK_AND_WAIT_FOR_OTHER_CELLS_STRATEGY(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
        /* SPR 18122 Changes Start*/ \
        semWaitDefault(&ev_fd_strategy_g[!(internalCellIndex)][MODULO(getCurrentTick(),2)]);\
        /* SPR 18122 Changes End*/ \
    }\
}

#define CHECK_AND_WAIT_FOR_OTHER_CELLS_PDSCH(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
        /* SPR 18122 Changes Start*/ \
        semWaitDefault(&ev_fd_pdsch_g[!(internalCellIndex)][MODULO(getCurrentTick(),2)]);\
        /* SPR 18122 Changes End*/ \
    }\
}

#define CHECK_AND_WAIT_FOR_OTHER_CELLS_ENCODER(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
        /* SPR 18122 Changes Start*/ \
        semWaitDefault(&ev_fd_encoder_g[!(internalCellIndex)][MODULO(getCurrentTick(),2)]);\
        /* SPR 18122 Changes End*/ \
    }\
}

#define CELL_STRATEGY_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
{\
    /* SPR 18122 Changes Start*/ \
    semPostDefault (&ev_fd_strategy_g[internalCellIndex][MODULO(getCurrentTick(),2)]);\
    /* SPR 18122 Changes End*/ \
}

#define CELL_PDCCH_PDSCH_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
{\
        UInt8 isOtherCellStateRunning = 0;\
    \
    /* SPR 18122 Changes Start*/ \
    semPostDefault (&ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick(),2)]);\
    /* SPR 18122 Changes End*/ \
        semWait(&cellSyncSem_g); \
        isOtherCellStateRunning =  CHECK_CELL_STATE(!internalCellIndex);  \
        semPost(&cellSyncSem_g); \
 \
        if (isOtherCellStateRunning)\
        {\
        /* SPR 18122 Changes Start*/ \
        semPostDefault (&ev_fd_pdsch_g[!(internalCellIndex)][MODULO(getCurrentTick(),2)]);\
        /* SPR 18122 Changes End*/ \
        }\
}

#define CELL_MUX_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
{\
    /* SPR 18122 Changes Start*/ \
    semPostDefault (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick(),2)]);\
    /* SPR 18122 Changes End*/\
}

#define CELL_ENCODER_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
{\
    /* SPR 18122 Changes Start*/ \
    semPostDefault (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick(),2)]);\
    /* SPR 18122 Changes End*/ \
}
/******** By Default EVENT FDs are used for sync threads. Mutex & semaphores are 
  implemented but not fully tested. **********/
/* if Mutexes are used to sync ELs */
#elif MUTEX_SYNC

#define CHECK_AND_WAIT_FOR_OTHER_CELLS_MUX(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
        /* SPR 18122 Changes Start*/ \
        semWait (&ev_fd_mux_g[!(internalCellIndex)][MODULO(getCurrentTick(),2)]);\
        /* SPR 18122 Changes End*/ \
    }\
}

#define CHECK_AND_WAIT_FOR_OTHER_CELLS_STRATEGY(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
        /* SPR 18122 Changes Start*/ \
        semWait (&ev_fd_strategy_g[!(internalCellIndex)][MODULO(getCurrentTick(),2)]);\
        /* SPR 18122 Changes End*/ \
    }\
}

#define CHECK_AND_WAIT_FOR_OTHER_CELLS_PDSCH(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
        /* SPR 18122 Changes Start*/ \
        semWait (&ev_fd_pdsch_g[!(internalCellIndex)][MODULO(getCurrentTick(),2)]);\
        /* SPR 18122 Changes End*/ \
    }\
}

#define CHECK_AND_WAIT_FOR_OTHER_CELLS_ENCODER(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
        /* SPR 18122 Changes Start*/ \
        semWait (&ev_fd_encoder_g[!(internalCellIndex)][MODULO(getCurrentTick(),2)]);\
        /* SPR 18122 Changes End*/ \
    }\
}

#define CELL_STRATEGY_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
{\
    /* SPR 18122 Changes Start*/ \
    semPost (&ev_fd_strategy_g[internalCellIndex][MODULO(getCurrentTick(),2)]);\
    /* SPR 18122 Changes End*/ \
}

#define CELL_PDCCH_PDSCH_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
{\
        UInt8 isOtherCellStateRunning = 0;\
    \
    /* SPR 18122 Changes Start*/ \
    semPost (&ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick(),2)]);\
    /* SPR 18122 Changes End*/ \
        semWait(&cellSyncSem_g); \
        isOtherCellStateRunning =  CHECK_CELL_STATE(!internalCellIndex);  \
        semPost(&cellSyncSem_g); \
 \
        if (isOtherCellStateRunning)\
        {\
        /* SPR 18122 Changes Start*/ \
        semPost (&ev_fd_pdsch_g[!(internalCellIndex)][MODULO(getCurrentTick(),2)]);\
        /* SPR 18122 Changes End*/ \
        }\
}

#define CELL_MUX_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
{\
    /* SPR 18122 Changes Start*/ \
    semPost (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick(),2)]);\
    /* SPR 18122 Changes End*/ \
}

#define CELL_ENCODER_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
{\
    /* SPR 18122 Changes Start*/ \
    semPost (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick(),2)]);\
    /* SPR 18122 Changes End*/ \
}

#else
/******** By Default EVENT FDs are used for sync threads. Mutex & semaphores are 
  implemented but not fully tested. **********/

/* if Event FDs are used to sync ELs */
/* mac thread changes + */
/* mac thread changes - */
#define CHECK_AND_WAIT_FOR_OTHER_CELLS_MUX(internalCellIndex)\
        {\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
	eventFD_t muxfd = {0};\
        /* SPR 18122 Changes Start*/ \
        eventfd_read_wrapper(ev_fd_mux_g[!(internalCellIndex)][MODULO(getCurrentTickForCell(internalCellIndex),2)],&muxfd);\
        /* SPR 18122 Changes End*/ \
    }\
}

#define CHECK_AND_WAIT_FOR_OTHER_CELLS_STRATEGY(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
	eventFD_t strategyfd = {0};\
        /* SPR 18122 Changes Start*/ \
        eventfd_read_wrapper(ev_fd_strategy_g[!(internalCellIndex)][MODULO(getCurrentTickForCell(internalCellIndex),2)],&strategyfd);\
        /* SPR 18122 Changes End*/ \
    }\
}

#define CHECK_AND_WAIT_FOR_OTHER_CELLS_PDSCH(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
	eventFD_t pdschfd = {0};\
            /* SPR 18122 Changes Start*/ \
            eventfd_read_wrapper(ev_fd_pdsch_g[!(internalCellIndex)][MODULO(getCurrentTickForCell(internalCellIndex),2)],&pdschfd);\
            /* SPR 18122 Changes End*/ \
    }\
}

#define CHECK_AND_WAIT_FOR_OTHER_CELLS_ENCODER(internalCellIndex)\
{\
    UInt8 isFdTobeRead = 0; \
    semWait(&cellSyncSem_g); \
    isFdTobeRead = CHECK_CELL_STATE(!internalCellIndex); \
    semPost(&cellSyncSem_g); \
    \
    if (isFdTobeRead)\
    {\
	eventFD_t encoderfd = {0};\
            /* SPR 18122 Changes Start*/ \
            eventfd_read_wrapper(ev_fd_encoder_g[!(internalCellIndex)][MODULO(getCurrentTickForCell(internalCellIndex),2)],&encoderfd);\
            /* SPR 18122 Changes End*/ \
    }\
}

/* - Critical section changes */
/** EL Sync Changes Start **/

#define CELL_STRATEGY_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
    {\
        /* SPR 18122 Changes Start*/ \
        eventfd_write_wrapper(ev_fd_strategy_g[internalCellIndex][MODULO(getCurrentTickForCell(internalCellIndex),2)],1);\
        /* SPR 18122 Changes End*/ \
    }

#define CELL_PDCCH_PDSCH_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
{\
        UInt8 isOtherCellStateRunning = 0;\
            \
            /* SPR 18122 Changes Start*/ \
            eventfd_write_wrapper(ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTickForCell(internalCellIndex),2)],1);\
            /* SPR 18122 Changes End*/ \
        semWait(&cellSyncSem_g); \
        /*cellElSyncTracker[internalCellIndex] = MUX_SYNC; */\
        isOtherCellStateRunning =  CHECK_CELL_STATE(!internalCellIndex);  \
        semPost(&cellSyncSem_g); \
 \
        if (isOtherCellStateRunning)\
        {\
                /* SPR 18122 Changes Start*/ \
                eventfd_write_wrapper(ev_fd_pdsch_g[!(internalCellIndex)][MODULO(getCurrentTickForCell(internalCellIndex),2)],1);\
                /* SPR 18122 Changes End*/ \
        }\
}

#define CELL_MUX_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
        {\
            /* SPR 18122 Changes Start*/ \
            eventfd_write_wrapper(ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTickForCell(internalCellIndex),2)],1);\
            /* SPR 18122 Changes End*/ \
        }

#define CELL_ENCODER_COMP_IND_FOR_OTHER_CELLS(internalCellIndex)\
        {\
            /* SPR 18122 Changes Start*/ \
            eventfd_write_wrapper(ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTickForCell(internalCellIndex),2)],1);\
            /* SPR 18122 Changes End*/ \
        }

/* mac thread changes + */
/* mac thread changes - */
#endif
/** EL Sync Changes End **/


#define ALLOCATE_EXTRA_CCE(dlUEContext_p,internalCellIndex)\
(((TX_MODE_3 == dlUEContext_p->dlMIMOInfo.transmissionMode) ||\
                (TX_MODE_4 == dlUEContext_p->dlMIMOInfo.transmissionMode) ||\
                (TX_MODE_8 == dlUEContext_p->dlMIMOInfo.transmissionMode)) && \
                (dlUEContext_p->scellCount) && (dlUEContext_p->internalCellIndex == internalCellIndex))
DLCQIInfo *getdlCQIInfo( DLUEContext *dlUEContext_p,
		InternalCellIndex internalCellIndex);
void mac_reset_extra_cce( DciCCEContainerInfo *dciCCEContainerInfo_p  , DciCCEInfo *dciCCEInfo_p 
          ,DLUEContext *dlUEContext_p,InternalCellIndex internalCellIndex);

/*pdcch allocation For scell changes end*/

/* cyclomatic changes */
/* compilation 10-JUN-2014 fix start */
#ifdef FDD_CONFIG
void mac_reset_cce(
     SInt8 aggrLvl, 
     DciCCEInfo *dciInfo_p,
     InternalCellIndex internalCellIndex);
#else
void mac_reset_cce(
     SInt8 aggrLvl, 
     DciCCEInfo *dciInfo_p,
     InternalCellIndex internalCellIndex);
#endif
/* compilation 10-JUN-2014 fix end */

extern tickType_t currentTick_g[MAX_NUM_CELL];
extern UInt32 systemFrameNum_g[MAX_NUM_CELL];
extern UInt32 subFrameNum_g[MAX_NUM_CELL];

extern UInt8 currentCFIValue_g[MAX_NUM_CELL];
#ifdef FDD_CONFIG
extern UInt8 maxCCEForCFI_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE];
#else
/** CA-TDD Changes Start **/
extern UInt8  miValueForSubframe_g[MAX_NUM_CELL];
/** CA-TDD Changes Stop **/
extern UInt8 maxCCEForCFI_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE][MAX_VALUE_M_I];
#endif
/* + Critical section changes */
void lteMacInterfacePostAllSyncFds(InternalCellIndex internalCellIndex,
                                     UInt8 isotherCellStateRunning);
/* - Critical section changes */
/* cyclomatic changes */
#define CONVERT_INT32_TO_BINARY(binary, size, tpcBitmap) \
{                                                        \
    UInt32 num = tpcBitmap;                              \
    while (num >0)                                       \
    {                                                    \
        size--;                                          \
                /* SPR 18122 Changes Start*/ \
                binary[size] = (MODULO(num,2)==0)?'0':'1';         \
                num = DIVIDE_BY_TWO(num);                                     \
                /* SPR 18122 Changes End*/ \
    }                                                    \
}                                                           

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*  This is a link list implemented via array. It maintains top n UEs in a tick
 *   * This list contains nodes of type sortNodeT */

typedef struct sortNode
{
    LTE_LIST_NODE node;
    UInt32 weightedPriority;
    UInt16 nxtIdx;
    UInt16 Idx;
    UInt16 ueIndex;
    /* ICIC changes start */
    UInt8 userLocationType;
    /* ICIC changes end */
}SortNodeT;

typedef struct sortNodeMemAlloc
{
    UInt16    freeHeadIdx;
    SortNodeT sortListNode[NUM_UES_COMPUTED_ONE_TICK];
}SortNodeMemAllocT;

/* + Critical section changes */
typedef enum  CellELSyncTrackerStateT
{
    STRATEGY_SYNC=0,
    PDSCH_SYNC,
    MUX_SYNC,
    ENCODER_SYNC,
    FALSE_SYNC,
    INVALID_SYNC
}CellELSyncTrackerState;

extern CellELSyncTrackerState cellElSyncTracker[MAX_NUM_CELL];
/* - Critical section changes */
/****************************************************************************
 * Exported Constants
 ****************************************************************************/
/****************************************************************************
 * Exported Variables
 ****************************************************************************/

#ifdef SEM_SYNC
SEM_T ev_fd_strategy_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SEM_T ev_fd_pdsch_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SEM_T ev_fd_mux_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SEM_T ev_fd_encoder_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
#elif MUTEX_SYNC
LTE_SEM ev_fd_strategy_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
LTE_SEM ev_fd_pdsch_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
LTE_SEM ev_fd_mux_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
LTE_SEM ev_fd_encoder_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
#else
SInt32 ev_fd_strategy_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SInt32 ev_fd_pdsch_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SInt32 ev_fd_mux_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SInt32 ev_fd_encoder_g[MAX_NUM_CELL][MAX_NUM_FD_CONTAINER];
SInt32 ev_fd_sfi_g[MAX_NUM_CELL];
#endif
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* SPR 5599 changes start (ZIP ID 129059) */
UInt32 logint(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 val
        );


#ifdef OVERSHOOT_CORRECTION 
#ifdef TDD_CONFIG
#define MAX_NUMBER_OF_SUBFRAME_IN_SYSTEMFRAME		10

typedef enum OptionalExecutionLegsTddT
{
OPTIONAL_EL_PROCESS_UL_RA,
OPTIONAL_EL_PROCESS_DEMUX_QUEUE,
OPTIONAL_EL_PROCESS_CQI_REPORTS,
OPTIONAL_EL_PROCESS_UL_CSI_SINR_REPORTS,
/*+ E_CID_PRS_TO_5.3*/
OPTIONAL_EL_PROCESS_E_CID_REPORTS,
/*- E_CID_PRS_TO_5.3*/
/* + SPR 13854 Changes */
OPTIONAL_EL_UPDATE_DL_TOP_SCH_LIST_SCELL_STATE,
/* - SPR 13854 Changes */
/*SPR 16855 +-*/
OPTIONAL_EL_RLC_TIMER_CHK_FOR_TX_EXPIRY,
OPTIONAL_EL_PROCESS_PENDING_DELETE_UE,
OPTIONAL_SEND_AUDIT_REPORT_FOR_INACTIVE_UE,
OPTIONAL_EL_PROCESS_MISC_UPDATES,
/*SPR 16855 Fix Start*/
OPTIONAL_EL_PROCESS_DL_OAM_AND_RRC_MSG,
OPTIONAL_EL_PROCESS_UL_OAM_AND_RRC_MSG,
OPTIONAL_EL_PROCESS_UL_PENDING_DELETE_UE,
/*SPR 16855 Fix End*/
OPTIONAL_EL_RLC_HANDLE_COMMON_CHANNEL_AND_PENDING_QUEUE,
/* + SPR 13854 Changes */
OPTIONAL_EL_RLC_HANDLE_PENDING_DELETE_UE,
/* - SPR 13854 Changes */
MAX_OPTIONAL_LEGS

}OptionalExecutionLegsTdd; 


/* SPR 13768 Changes Start */
extern ExecutionFlowCreationParam *executionFlowCreationParamArr_g[MAX_NUM_CELL];
/* SPR 13768 Changes End */
/* It will contain sum of processing time for All ELs after MUX and before
 * UL_RA*/
/** CA-TDD Changes Start **/
extern UInt32
reqdProcessingTimeAtMuxTDD_g[MAX_NUM_CELL][MAX_NUMBER_OF_SUBFRAME_IN_SYSTEMFRAME][MAX_UES_SCHEDULED_INDX];
extern UInt64
reqdProcessingCyclesAtMuxTDD_g[MAX_NUM_CELL][MAX_NUMBER_OF_SUBFRAME_IN_SYSTEMFRAME][MAX_UES_SCHEDULED_INDX];
/** CA-TDD Changes Stop **/

extern UInt32
reqdProcessingTimeELTdd_g[MAX_EXECUTION_LEGS][MAX_UES_SCHEDULED_INDX];

extern UInt64 
reqOptionalElProcessingCyclesTdd_g[MAX_OPTIONAL_LEGS][MAX_UES_SCHEDULED_INDX];

/* SPR 13768 Changes Start */
/* +- SPR 17777 */
void updateElCycleCountTDD(InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
/* SPR 13768 Changes End */
#endif
#endif
/* TDD Tick overshoot correction changes end*/

#endif  /* LTE_MAC_UTILITY_AND_SHARED_STRUCTS_H*/

