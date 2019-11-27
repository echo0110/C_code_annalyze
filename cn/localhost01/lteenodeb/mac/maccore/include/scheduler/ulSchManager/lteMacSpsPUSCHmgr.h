/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULSpsScheduler.h,v  $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the definitions for SPS scheduler.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/
#ifndef _LIB_MAC_UL_SPS_PUSCH_DEF_
#define _LIB_MAC_UL_SPS_PUSCH_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacULSpsScheduler.h"
#include "lteMacULResourceInfoMgr.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_UL_RES_RSV_REQ_PROCESS 1

#define NORMAL_LOCATION_UPDATE     1
#define RAR_LOCATION_UPDATE        2


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
typedef struct SpsULStrIntervalTTISortedT
{
    /* Index number of SPS TTIContext */
    UInt8 index;
    /* Counter maintained by RM for traversing indexes as per least loaded TTI
       It shall be taken as a key for quick sort of RM */
    UInt8 numSpsUeinRm;
} *LP_SpsULStrIntervalTTISorted, SpsULStrIntervalTTISorted;

/******************************************************************************
 * Function Name  : initDLSpsTTIContextMap
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function will invoke SPS PDSCH.
 ******************************************************************************/
/*CA Changes start  */
void initULSpsTTIContextMap(UInt32 rmSize, InternalCellIndex internalCellIndex);
/*CA Changes end  */

/******************************************************************************
 * Function Name  : ulSpsResourceReserveHandler
 * Inputs         : qCount - Number of nodes in a queue.
 * Outputs        : None
 * Returns        : None
 * Description    : This function will invoke SPS PUSCH for resource reserve 
 *                  request.
 ******************************************************************************/
/*CA Changes start  */
void ulSpsResourceReserveHandler( UInt32 qCount, UInt8 ulDelay,
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        UInt32 subframe,
#endif
        /* - SPS_TDD_Changes */
        InternalCellIndex internalCellIndex 
        );
/*CA Changes end  */

/******************************************************************************
 * Function Name  : ulSpsResourceReleaseHandler
 * Inputs         : qCount - Number of nodes in a queue.
 * Outputs        : None
 * Returns        : None
 * Description    : This function will invoke SPS PUSCH for resource release 
 *                  request.
 ******************************************************************************/
void ulSpsResourceReleaseHandler( UInt32 qCount, InternalCellIndex internalCellIndex);

void processULResourceReleaseRequest(ULUEContext *ulUeCtx_p
#ifdef FDD_CONFIG
                                    ,UInt8 reqType
#endif
                                    /*CA Changes start  */
                                    ,InternalCellIndex internalCellIndex
                                    /*CA Changes end  */
                                    );

/****************************************************************************
 * Function Name  : spsUlSort
 * Inputs         : sortedArray - Array to be sorted
 *                  startingIndex
 *                  lastIndex
 * Outputs        : None.
 * Returns        : None
 * Description    : Sorts the array in ascending order using insertion sort.
 ****************************************************************************/
void spsUlSort( SpsULStrIntervalTTISorted sortedArray[],
        UInt8 startingIndex, UInt8 lastIndex,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
        /*CA Changes end  */


/****************************************************************************
 * Function Name  : markULSpsRBsInRBMap
 * Inputs         : rbMapNum - resource map number 
 *                : spsArrayTti - SPS array TTI  
 * Returns        : None
 * Description    : marks the SPS RBs in PUSCH RM before RM is processed in 
 *                  PUSCH.
 ****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
void markULSpsRBsInRBMap(UInt8 rbMapNum, UInt8 spsArrayTti, UInt8 locationFlag,
 /* +- SPR 17777 */
#ifdef FDD_CONFIG
                         tickType_t currentGlobalTick, UInt8 ulDelay,
#endif
                         /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                         UInt8 subFrame,
#endif
                         /* - SPS_TDD_Changes */
                         InternalCellIndex internalCellIndex);
/*CA Changes end  */
#ifdef FDD_CONFIG
UInt8 numOfSpsTTIBUeForCurrentTick(tickType_t currentGlobalTTI,
                                    UInt8 ulDelay);
/* SPR 15909 fix end */
MacRetType getAvailableRbsFromMaps(UInt8 ttiIndex,
                                   /* +- SPR 17777 */
                                   ULUEContext *ueContext_p,
                                   UInt8  numOfTicksToCheck,
                                   UInt8  requiredRBCount,
                                   RbMapNode* rbMapNodeUnion,
                                   UInt8  spsPeriodicity,
                                   /*CA Changes start  */
                                   InternalCellIndex internalCellIndex);
                                   /*CA Changes end  */

#endif
#endif

