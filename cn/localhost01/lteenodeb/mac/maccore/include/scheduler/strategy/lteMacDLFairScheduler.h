/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLFairScheduler.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacDLFairScheduler.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.13  2009/07/24 12:39:09  gur20052
 * Splint Warnings are removed
 *
 * Revision 1.12  2009/06/23 16:55:58  gur20052
 * Review comments are incorporated
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
 * Revision 1.5  2009/05/26 10:16:51  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.4  2009/05/25 16:20:36  gur20052
 * Updated the version to support the UL Fair and Round Robin Scheduler
 *
 * Revision 1.3  2009/05/19 14:29:57  gur20052
 * Updated with DL Fair Scheduler module specific changes
 *
 * Revision 1.2  2009/05/11 06:20:24  gur20052
 * Updated the function prototype
 *
 * Revision 1.1  2009/04/30 16:56:32  gur20052
 * Added initial version of DL Fair Scheduler
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_DL_FAIR_SCHEDULER_H
#define LTE_MAC_DL_FAIR_SCHEDULER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacUtilityAndSharedStructs.h"
#include "lteMacDLHarqMgr.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/
#define NACK_MULTPLICATION_FACTOR 10
/* 
** Below MACRO is hard coded with value 8 and will be made configurable once
** OAM i/f changes w.r.t PFS Optimization are done.
*/
#define NUM_UES_COMPUTED_ONE_TICK_DL 4 
#define MIN_RB_REQUIRED_FOR_PFS 1
#define  NUM_ITER_TO_CALC_MEAN_Q_LOAD_DL 4
#define NUM_BUFFER_UES_DL 2 

#define INIT_PFS_GLOBALS(globalsName_p) \
        { \
            globalsName_p->unUsedRBDL = 0; \
            globalsName_p->calcQloadMeanItr = ZERO; \
            globalsName_p->qLoadComputed = FALSE; \
        }

/* This stucture stores information about Cell Center and Cell Edge user which
 * are selected by DL PFS strategy.*/

typedef struct DLUserListPFST
{
    /* Number of user in the list */
    UInt32 count;

    /* Index mapped to DL Scheduler Data Element */
    UInt8 indexToDlSchedulerDataElement[MAX_DL_UE_SCHEDULED];
}DLUserListPFS;

/*CA_CHG_START*/
typedef struct PFSGlobalsT
{
    UInt32 unUsedRBDL;
    SortNodeMemAllocT dlSortNodeMemAllocElem;
    UInt8 calcQloadMeanItr;
    LTE_LIST lastTickScheduledList;
    UInt8 qLoadComputed;
}PFSGlobals;
/*CA_CHG_END*/
extern PFSGlobals *pfsGlobals_gp[MAX_NUM_CELL];

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

UInt32 dlProportionalFairScheduler
        (
        /* SPR 21068 start */
        tickType_t referenceFlag,
        /* SPR 21068 end   */
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement *dlScheduleData_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* SPR 17777 */
        UInt8  maxUEsToBeScheduledDL, /* RETX_CHG */
        InternalCellIndex internalCellIndex
        );


UInt32 calculateDLWeightedPriority
        (
        DLUEContext* ueDLContext_p ,
        UInt32 qLoadWeightedMean_g,
        DLSchedulerDataNodeInfo *dlSchedulerDataNodeArr_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
        ,InternalCellIndex internalCellIndex
        );

void iterateNonZeroListsAndCalculateWeightedQueueLoad
        (
        DLSchedulerDataElement * dlScheduleData_p,
        UInt32 *qLoadWeightedMean_p,
        UInt32 *totalUECount,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /*SPR 21068 start */
        tickType_t referenceFlag,
        /*SPR 21068 end   */
        InternalCellIndex internalCellIndex

        );
        
UInt32 getDLModulationScheme
        (
         DLUEContext*  ueDLContext_p
        );

void calculateDLWeightedQueueLoad
        (
         DLUEContext *ueContext_p,
         UInt32 *weightedQueueLoad 
         );

UInt32 allocDLRBsToSortedUEs
        (
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement *dlScheduleData_p,
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
#endif
        DLUserListPFS *dlUserListPFS,
        UInt32* totalUEInDLList
        ,InternalCellIndex internalCellIndex
        );

/*SPR 2446 Fix Begins*/
MacRetType assignDLRBsAndSendToPDCCH
        ( 
         DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p,
         UInt32 allocatedRB,
         UInt32 * availableRB_p
#ifdef TDD_CONFIG
         /* SPR 15909 fix start */
       ,tickType_t currentGlobalTTITickCount
       /* SPR 15909 fix end */
#endif
       ,InternalCellIndex internalCellIndex
       );
/*SPR 2446 Fix Ends*/
        
/* SPR 5599 changes start (ZIP ID 129096) */
MacRetType processDLFairSchedulerNewDataGeneral(
/* SPR 5599 changes start (ZIP ID 129096) */
        UInt32 allocatedRBFromRoundRobin,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
       /* SPR 15909 fix start */
        ,tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#endif
        ,InternalCellIndex internalCellIndex
        );

MacRetType processDLNewDataWithTwoTB
        (
        UInt32 allocatedRB,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLSchedulerDataNodeInfo  *tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        ,tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#endif
        /*4x4 DL MIMO CHG START*/
        ,DLHARQProcess * harqProcess_p
        /*4x4 DL MIMO CHG END*/
        ,InternalCellIndex internalCellIndex
        );
        
MacRetType processDLNewDataWithOneTBOnly
        (
        UInt32 allocatedRB,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLSchedulerDataNodeInfo  *  tempDLSchedulerDataNodeArr_p
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        ,tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#endif
        ,InternalCellIndex internalCellIndex
        );


#endif  /* LTE_MAC_DL_FAIR_SCHEDULER_H */
