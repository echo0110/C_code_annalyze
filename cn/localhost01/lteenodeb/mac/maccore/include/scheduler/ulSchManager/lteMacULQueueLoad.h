/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULQueueLoad.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Thid file contains the data structures and function
 *                     declaraions of the UL Queueload. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacULQueueLoad.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.14  2009/07/13 14:21:40  gur15697
 * UL_QUEUELOAD_MGR  added
 *
 * Revision 1.13  2009/06/25 06:27:19  gur15697
 * initULQueueLoadMgr() cleanupULQueueLoadMgr() removed
 *
 * Revision 1.12  2009/06/05 08:04:40  gur15697
 * init and cleanup functions added
 *
 * Revision 1.11  2009/06/03 12:03:51  gur15697
 * initULQueueLoadMgr() added
 *
 * Revision 1.10  2009/06/03 10:07:39  gur15697
 * bug fixes in UT cycle-1
 *
 * Revision 1.9  2009/06/01 06:38:56  gur18569
 * removed compilation errors
 *
 * Revision 1.8  2009/05/29 10:18:56  gur18569
 * removed compilation error
 *
 * Revision 1.7  2009/05/28 10:12:13  gur15697
 * macros NON_ZERO_TO_ZERO_BSR and  ZERO_TO_NON_ZERO_BSR removed
 *
 * Revision 1.6  2009/05/22 06:13:43  gur15697
 * as per the modified UL and DL context code modified
 *
 * Revision 1.5  2009/05/18 05:57:24  gur15697
 * Incorporated the comments
 *
 * Revision 1.4  2009/05/11 10:35:53  gur15697
 * short BSR handling added
 *
 * Revision 1.3  2009/05/08 14:16:47  gur15697
 * compilation errors removed
 *
 * Revision 1.2  2009/05/05 11:18:36  gur15697
 * modified to add logic in the functions
 *
 * Revision 1.1  2009/04/30 06:55:46  gur15697
 * Initial vertion
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_UL_QUEUE_LOAD_H
#define LTE_MAC_UL_QUEUE_LOAD_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacSFNMgr.h"
#include "lteMacDLUEContext.h"
#include "lteMacULUEContext.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define UL_QUEUELOAD_MGR "ul_queueload_mgr"
#define FETCH_BUFFER_SIZE_FROM_BSR_INDEX(index) bsrIndexArray_g[index]

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct bsrQueueNodeT
{
    LTE_SNODE   bsrNodeAnchor;
    AddDeleteTrigger  addDeleteTrigger;
    UInt16            ueIdx;
    UInt8             lcgId;
}bsrQueueNode;
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt32 bsrIndexArray_g[];
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

extern UInt8 *fillLongBSR(UInt32 *bsrReport_p, UInt8 *dataPtr_p);

extern void updateDerivedBSR(UInt32 *tempPreviousBsr_p,
                             ULUEContext *ulCntxt_p,
                             UInt32 *bsrReport_p,
                             UInt32 shortBSRFlag,
                             UInt32 shortBSRLcgId,
                             UInt32 thdIndex);

extern UInt8* updateLongBSR(ULUEContext *ulCntxt_p,
                     UInt8  *dataPtr_p,
                     UInt32 thdIndex,
                     /*CA Changes start  */
                     InternalCellIndex cellIndex
                     /*CA Changes end  */
                     /* SPR 16916 FIX START */
#ifdef KPI_STATS
                     ,UInt8 *lcgIndex_p,
                     UInt8 *addTrigLCGNodeForKpi
#endif
                     /* SPR 16916 FIX END */
                     );

extern UInt32 updateShortOrTruncatedBSR(ULUEContext *ulCntxt_p,
                               UInt8  *dataPtr_p,
                               UInt32 thdIndex,
                               /*CA Changes start  */
                               InternalCellIndex cellIndex
                               /*CA Changes end  */
                               /* SPR 16916 FIX START */
#ifdef KPI_STATS
                               ,UInt8 *lcgIndex_p,
                               UInt8 *addTrigLCGNodeForKpi
#endif
                               /* SPR 16916 FIX END */
                               );
/*+COVERITY 5.3.0 - 53495*/
extern void putEntryInBSRQueue(UInt16 ueIndex,
                               UInt32 lcgId,
                               UInt32 thdIndex,
                               AddDeleteTrigger  flag,
                               /*CA Changes start  */
                               InternalCellIndex cellIndex);
                               /*CA Changes end  */
/*-COVERITY 5.3.0 - 53495*/

/* SRP 3608 changes start */
/*CA Changes start  */
extern void putBSREntryInPriorityGrantQueue(UInt16 ueIndex, InternalCellIndex cellIndex);
/*CA Changes end  */
/* SPR 3608 changes end */
/* SPR 5599 changes start (ZIP ID 129059) */
void updateBsrNet( ULUEContext* ulUEContext_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                          UInt32 tempAllocTbSize);

/* SPR 5599 changes start (ZIP ID 129059) */
void updateBsrNetOfLCG( ULUEContext* ulUEContext_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                  UInt32 tempAllocTbSize
                                  /* SPR 10027 FIX START */
                                  /* +- SPR 17777 */
                                  ,ULHarqInfo* ulHarqInfo_p
                                  /* SPR 10027 FIX END */
                                   /* SPR 11864 Fix */
#ifdef KPI_STATS
                                  ,InternalCellIndex  internalCellIndex
#endif
                                  /* +- SPR 17777 */
                                    /* SPR 11864 Fix */
                                  );

void ulSpsHandleActivationInRR(ULUEContext *ueULContext_p,
                               UInt32 lcgIndex,
                               /*CA Changes start  */
                               InternalCellIndex internalCellId                               
                               /*CA Changes end  */
                               );
#ifdef KPI_STATS
/* 32.425 Events code changes start*/
/* + PERF_CA GBL */
/* + SPR 17404 */
void lteMacUpdateKpiStatsIPThpUlOnEnqueue (UInt32 lcgIndex ,ULLCGInfo *ulLCGInfo_p,UInt16 ueIndex,
				InternalCellIndex internalCellIndex);
/* - SPR 17404 */
/* - PERF_CA GBL */
#endif
/* 32.425 Events code changes end*/
#endif  
