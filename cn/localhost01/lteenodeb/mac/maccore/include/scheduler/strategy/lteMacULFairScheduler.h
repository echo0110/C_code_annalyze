/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULFairScheduler.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacULFairScheduler.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.11  2009/07/13 11:05:05  gur20052
 * Removed code in #if 0
 *
 * Revision 1.10  2009/06/08 09:58:43  gur20052
 * Removed the linking errors
 *
 * Revision 1.9  2009/06/05 16:20:48  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.8  2009/06/04 07:42:10  gur20052
 * Review comments are incorporated
 *
 * Revision 1.7  2009/06/02 07:19:29  gur20052
 * Updated during comments incorporation and dispostion
 *
 * Revision 1.6  2009/05/28 11:33:52  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.5  2009/05/25 16:20:36  gur20052
 * Updated the version to support the UL Fair and Round Robin Scheduler
 *
 * Revision 1.4  2009/05/21 17:22:24  gur20052
 * Updated the Basic Framework of UL Strategy
 *
 * Revision 1.3  2009/05/19 14:29:57  gur20052
 * Updated with DL Fair Scheduler module specific changes
 *
 * Revision 1.2  2009/05/11 06:20:24  gur20052
 * Updated the function prototype
 *
 * Revision 1.1  2009/04/30 17:36:15  gur20052
 * Initial version added
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_UL_FAIR_SCHEDULER_H
#define LTE_MAC_UL_FAIR_SCHEDULER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteMacTypes.h"
#include "lteMacULUEContext.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacUtilityAndSharedStructs.h"
#include "lteMacULResourceAllocator.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define NETBSRLOAD(UEContext_p)\
    ( UEContext_p->bsrNetTotal - ( \
                                   UEContext_p->ulresInfo[0].deltabsrLoad +\
                                   UEContext_p->ulresInfo[1].deltabsrLoad +\
                                   UEContext_p->ulresInfo[2].deltabsrLoad +\
                                   UEContext_p->ulresInfo[3].deltabsrLoad +\
                                   UEContext_p->ulresInfo[4].deltabsrLoad +\
                                   UEContext_p->ulresInfo[5].deltabsrLoad +\
                                   UEContext_p->ulresInfo[6].deltabsrLoad +\
                                   UEContext_p->ulresInfo[7].deltabsrLoad \
                                 ) )
/*
** Below MACRO is hard coded with value 8 and will be made configurable once
** OAM i/f changes w.r.t PFS Optimization are done.
*/
#define NUM_UES_COMPUTED_ONE_TICK_UL 4

#define  NUM_ITER_TO_CALC_MEAN_Q_LOAD_UL        4
#define NUM_BUFFER_UES_UL 2

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* This stores the information required to schedule node in 
 * cell center and cell edge list in UL */
typedef struct ULSchedulableUENodePFST
{
    /* Index mapped to DL Scheduler Data Element */
    UInt8 indexToDlSchedulerDataElement;

    /* Harq processid */
    UInt8 harqProcessId;
}ULSchedulableUENodePFS;

/* This stucture stores information about Cell Center and Cell Edge user which
 * are selected by DL PFS strategy.*/
typedef struct ULUserListPFST
{
    /* Number of user in the list */
    UInt32 count;

    /* Index mapped to DL Scheduler Data Element */
    ULSchedulableUENodePFS ulSchedulableUENode[MAX_UL_UE_SCHEDULED];
}ULUserListPFS;

/*CA Changes start */
typedef struct ULPfsGlobalsT
{
    UInt8 calcBsrloadMeanItr;
    UInt8 bsrLoadComputed;
    SortNodeMemAllocT ulSortNodeMemAllocElem;
    LTE_LIST lastTickScheduledList;
}ULPfsGlobals;
/*CA Changes end */


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* SPR 2446 Fix Begins*/
UInt32 ulProportionalFairScheduler
        (
        /*SPR 21068 start */
        tickType_t ulReferenceFlag,
        /*SPR 21068 end   */
        UInt32 * numberOfAvailableRBs_p,
        UInt8 maxUEsToBeScheduledUL,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG        
        ,UInt8 ulDelay
#endif
        /*CA Changes start */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );
/* SPR 2446 Fix Ends*/


void iterateNonZeroListsAndCalculateULWeightedPriority
        (
        ULSchedulerDataElement* ulScheduleData_p,
        /* SPR 21068 start */
        tickType_t referenceFlag,
        /* SPR 21068 end   */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
        );

UInt32 calculateULWeightedPriority
        (
        ULUEContext* ueULContext_p,
        UInt32 bsrLoadWeightedMean,
        ULSchedulerDataNodeInfo *ulSchedulerDataNodeArr_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex
        );

void iterateNonZeroListsAndCalculateWeightedBSRLoad
        (
        UInt32 *bsrLoadWeightedMean_p,
        UInt32 *totalUECount,
        /* SPR 21068 start */
        tickType_t referenceFlag
        /* SPR 21068 end   */
        /*CA Changes start */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        );

/*SPR 2446 Fix Begins*/        
UInt32 allocULRBsToSortedUEs
        (
        UInt32 *numberOfAvailableRBs_p,
        ULSchedulerDataElement *ulScheduleData_p,
        UInt8 maxUEsToBeScheduledUL,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
       ,UInt8 ulDelay
#endif
       /*CA Changes start  */
       ,InternalCellIndex internalCellIndex
       /*CA Changes end  */
       /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
       ,UInt8 ulSubFrameNum
#endif
       /* TDD Config 0 Changes End */
        );

/*SPR 2446 Fix Ends*/

MacRetType  assignULRBsAndSendToPDCCH
        (
         UInt32 ueIndex,
         UInt32 allocatedRB,
         UInt32 *numberOfAvailableRBs_p,
         /* SPR 15909 fix start */
         tickType_t currentGlobalTTITickCount,
         /* SPR 15909 fix end */
         /* ULA_CHG */
         UInt32 strictAllocRBFlag,
#ifdef TDD_CONFIG
         UInt8 ulHarqProcessId,
        /* SPR 11978 Fix Start */
        /* TDD Config 0 Changes Start */
         UInt8 ulSubFrameNum,
        /* TDD Config 0 Changes End */
        /* SPR 11978 Fix End */
#endif
         UInt32 minDataSize,
         UInt32 maxDatasize,
         UInt8 avgMcs
         /* ULA_CHG */
#ifdef FDD_CONFIG
        ,UInt8 ttiBundling
#endif
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */

        );

/* For TTIB_Code */
extern MacRetType processMeasGapValidation(ULUEContext *ulUEContext_p,
                                /* SPR 15909 fix start */
                                tickType_t currentGlobalTTITickCount,
                                /* SPR 15909 fix end */
                                  UInt32 ulDelay);


#endif  /* LTE_MAC_UL_FAIR_SCHEDULER_H */
