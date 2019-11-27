/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULHarqMgr.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the data structure and the function 
 *                     decleration related to ul harq.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_UL_HARQ_H
#define LTE_MAC_UL_HARQ_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacULUEContext.h"
#include "lteMacDLUEContext.h"
#include "lteMacULResourceAllocator.h"
/*SPR_3305_FIX_START*/
/* msg3_retx_tdd_support_start */
/*#ifdef ULHARQ_TIMER_PROC*/
#include "lteMacRandomAccess.h"
/*#endif*/
/* msg3_retx_tdd_support_end */
/*SPR_3305_FIX_END*/
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define LOG_UL_HARQ "UL_HARQ"

/****************************************************************************
 * Exported Types
 ****************************************************************************/
#ifdef FDD_CONFIG
#ifdef ULHARQ_TIMER_PROC
typedef struct ULHarqTimerNodeT
{
    UInt8   isValid;
    UInt8   harqProcessId; 
    /* + SPS_TDD_Changes */
    UInt8   schUsed;
    /* - SPS_TDD_Changes */
    UInt8   ulHarqExpiredSubframe;
    UInt16  ulHarqExpiredSysframe;
    UInt16  ueIndex;
    /* SPR 15909 fix start */
    tickType_t  ulHarqExpiredTTI;
    /* SPR 15909 fix end */
}ULHarqTimerNode;

typedef struct ULHarqTimerEntityListT
{
    /* The count will contain the number of valid field in the array*/
    UInt16 count;
    /* This array will store the harqTimerNode */       
    /* SPR 9381 changes start */
    ULHarqTimerNode ulHarqTimerNodeArr[MAX_UL_UE_SCHEDULED];
    /* SPR 9381 changes end */
}ULHarqTimerEntityList;

#endif
#endif

/* msg3_retx_tdd_support_start */
/*SPR_3305_FIX_START*/
typedef struct ULHarqTimerForMsg3NodeT
{
    UInt8   isValid;
    UInt8   harqProcessId;
    UInt8   ulHarqExpiredSubframe;
    UInt16  ulHarqExpiredSysframe;
    UInt16  ueIndex;
    /* SPR 15909 fix start */
    tickType_t  ulHarqExpiredTTI;
    /* SPR 15909 fix end */
}ULHarqTimerForMsg3Node;
typedef struct ULHarqTimerEntityForMsg3ListT
{
    /* The count will contain the number of valid field in the array*/
    UInt16 count;
    /* This array will store the harqTimerNode */
    ULHarqTimerForMsg3Node ulHarqForMsg3TimerNodeArr[5 * MAX_UL_UE_SCHEDULED];
}ULHarqTimerEntityForMsg3List;
/*SPR_3305_FIX_END*/
/* msg3_retx_tdd_support_end */
/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#ifdef FDD_CONFIG
#define START_ULHARQ_TIMER_OFFSET 0x4

/* TTIB_Code Start */
#define TTIB_ULHARQ_TIMER_TTI_TX_OFFSET 0x7

/* calculated as TTI_PHICH_TO_DCI_OFFSET (5) + FDD_HARQ_OFFSET(4)+ TTI_BUNDLE ((3)*/
#define TTIB_ULHARQ_TIMER_TTI_RETX_OFFSET 12
/* TTIB_Code End */

#define RESTART_HARQ_TIMER_WINDOW 8


#endif
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#ifdef FDD_CONFIG
#ifdef ULHARQ_TIMER_PROC
/* this array will store the harqs of UE on per subframe basis.*/
/*CA Changes start  */
extern ULHarqTimerEntityList ulHarqTimerEntityList_g[MAX_NUM_CELL][MAX_UL_HARQ_TIMER][MAX_SUB_FRAME];
/*SPR_3305_FIX_START*/
extern ULHarqTimerEntityForMsg3List ulHarqTimerEntityForMsg3List_g[MAX_NUM_CELL][MAX_UL_HARQ_TIMER][MAX_SUB_FRAME];
/*SPR_3305_FIX_END*/
#endif
#endif
/* ICIC changes start */
extern UInt16 totalRetxRb_g[MAX_NUM_CELL][MAX_USER_LOCATION][NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME]; 
/* ICIC changes end */
/*CA Changes end  */
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* SPR 7435 Changes Starts */
/* SPR 5599 changes start (ZIP ID 129059) */
/* SPR 16916 FIX START */
#ifdef KPI_STATS
void freeULHarqProcess(ULHarqInfo*   tempULHarqInfo, UInt16 ueIndex,InternalCellIndex internalCellIndex); 
#else
void freeULHarqProcess(ULHarqInfo*   tempULHarqInfo); 
#endif
/* SPR 16916 FIX END */
/* SPR 5599 changes end (ZIP ID 129059) */
/* SPR 7435 Changes Ends */

/* TTIB_Code Start */
#ifdef FDD_CONFIG 
 void updateTTIHarqMap(UInt8 harqID,
                             ULHarqInfo* tempULHarqInfo,
                             UInt8 *ttiHarqMap);
#endif
/* TTIB_Code End */

/* SPR 5599 changes start (ZIP ID 129059) */
void updateULHarqProcess(ULHarqInfo*   tempULHarqInfo,
/* SPR 5599 changes end (ZIP ID 129059) */
                         AllocatedResInfo* resourceReserved,
                         UInt8 updateFlag);


/* SPR 5599 changes start (ZIP ID 129059) */
void allocateULHarqProcess(ULHarqInfo*   tempULHarqInfo,
        /* +- SPR 17777 */
        /* SPR 5599 changes end (ZIP ID 129059) */
        AllocatedResInfo* resourceReserved
        /* +- SPR 17777 */
			       /* SPR 11331 Fix Start */
#ifdef TDD_CONFIG
                                /* SPR 15909 fix start */
                              ,tickType_t ulSfTick
                                /* SPR 15909 fix end */
#endif
			      /* SPR 11331 Fix End */
                               );
#ifdef FDD_CONFIG
#ifdef ULHARQ_TIMER_PROC
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType ulHarqTimerStart(UInt16  ueIndex,
                UInt8 harqProcessId,
                UInt8 frameNumber,
                /* + SPS_TDD_Changes */
                UInt8 schUsed,
                /* - SPS_TDD_Changes */
                /* SPR 15909 fix start */
                tickType_t  timerExpiredTTI,
                /* SPR 15909 fix end */
                UInt32 expirySysFrameNumber,
                /*CA Changes start  */
                InternalCellIndex  internalCellIndex);
                /*CA Changes end  */
/* SPR 7435 Changes Starts */
 MacRetType ulHarqTimerStop(ULUEContext* ueContext_p,
        /* +- SPR 17777 */
		UInt8 frameNumber,
		UInt32 currentSysFrameNumber,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex);
        /*CA Changes end  */
/* SPR 7435 Changes Ends */
/* SPR 5599 changes end (ZIP ID 129059) */
MacRetType expireAllNodeFromULHarqTimerList(UInt32 loopOfNumOfSubFrames,
            UInt8 currentSubFrame,
            UInt8 delaySubFrame,
            /* SPR 15909 fix start */
            tickType_t currentGlobalTick,
            /* SPR 15909 fix end */
            UInt32 sysFrameNumber,
            /*CA Changes start  */
            InternalCellIndex  internalCellIndex);
            /*CA Changes end  */
/*SPR_3305_FIX_START*/
MacRetType expireAllNodeFromULHarqTimerForMsg3List(UInt32 loopOfNumOfSubFrames,
        UInt8 currentSubFrame,
        UInt8 delaySubFrame,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt32 sysFrameNumber,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex);
        /*CA Changes end  */
/*SPR_3305_FIX_END*/
/* SPR 5599 changes start (ZIP ID 129059) */
/*CA Changes start */
void initULHarq(UInt8 numOfCells);
void initULHarqTimerArray(UInt8 numOfCells);
/*CA Changes end */
/* SPR 5599 changes end (ZIP ID 129059) */
/*SPR_3305_FIX_START*/

/* SPR 5599 changes start (ZIP ID 129059) */
void initULHarqTimerArrayForMsg3(UInt8 numOfCells);
/* SPR 5599 changes end (ZIP ID 129059) */
/*SPR_3305_FIX_END*/

/* SPR# 3860 Changes Starts  */
void expireULHarqTimerNodeForTickMiss(UInt8 loopOfNumOfSubFrames,
                                         /*CA Changes start  */
                                          InternalCellIndex internalCellIndex);
                                         /*CA Changes end  */
/* SPR# 3860 Changes Ends */
#endif
#endif 
/* msg3_retx_tdd_support_start */
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType ulHarqTimerStartForMsg3(UInt16 tcRnti, UInt8 harqProcessId,
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 frameNumber,
        /* SPR 15909 fix start */
        tickType_t timerExpiredTTI,
        /* SPR 15909 fix end */
        UInt32 expirySysFrameNumber,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex);
        /*CA Changes end  */

/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType ulHarqTimerForMsg3Stop( TempCRNTICtx *tempCRNTICtx_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                          UInt32 frameNumber,
                                          UInt32  currentSysFrameNumber,
                                          /*CA Changes start  */
                                          InternalCellIndex internalCellIndex);
                                          /*CA Changes end  */

/* msg3_retx_tdd_support_end */
/*CA Changes start */
void initULHarqForMsg3(UInt8 numOfCells); 
/*CA Changes end */
/* SPR 5599 changes end (ZIP ID 129059) */
#endif  /* LTE_MAC_UL_HARQ_H */
