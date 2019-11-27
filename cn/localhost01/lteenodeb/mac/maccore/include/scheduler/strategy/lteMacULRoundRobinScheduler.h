/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULRoundRobinScheduler.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacULRoundRobinScheduler.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.10  2009/07/13 14:07:07  gur20052
 * UT Bugs fixed
 *
 * Revision 1.9  2009/06/05 16:20:48  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.8  2009/06/05 06:33:06  gur20052
 * incorporated the Review comments
 *
 * Revision 1.7  2009/06/04 07:42:10  gur20052
 * Review comments are incorporated
 *
 * Revision 1.6  2009/06/02 07:19:29  gur20052
 * Updated during comments incorporation and dispostion
 *
 * Revision 1.5  2009/05/28 11:33:52  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.4  2009/05/25 16:20:36  gur20052
 * Updated the version to support the UL Fair and Round Robin Scheduler
 *
 * Revision 1.3  2009/05/21 17:22:24  gur20052
 * Updated the Basic Framework of UL Strategy
 *
 * Revision 1.2  2009/05/11 06:20:24  gur20052
 * Updated the function prototype
 *
 * Revision 1.1  2009/04/30 17:12:51  gur20052
 * Added inital version of Uplink Round Robin Scheduler header file
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_UL_ROUND_ROBIN_SCHEDULER_H
#define LTE_MAC_UL_ROUND_ROBIN_SCHEDULER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacULResourceAllocator.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define RB_FOR_ROUND_ROBIN 5

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*CA Changes start */
extern LTE_LIST_NODE * nonZeroBsrListStartRoundRobin_p[MAX_NUM_CELL];
/*CA Changes end */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/* ICIC Change Start */
/****************************************************************************
 *  Structure Declaration
 *  ************************************************************************/

/* This stores the information required to schedule node in 
 * cell center and cell edge list in UL */
typedef struct ULSchedulableUENodeT
{
    /*UL UE Id*/
    UInt16 ueIndex;

    /* Harq processid */
    UInt8 harqProcessId;
}ULSchedulableUENode;

/* This stores the cell center and cell edge list that is going to be
 * scheduled in UL */
typedef struct ULUserListRRT
{
    /* Number of UE */
    UInt8 count;

    /* Array will store info for Cell Center and Cell Edge user which are going
     * to be scheduled in UL */
    ULSchedulableUENode ulSchedulableUENode[
         MAX_UL_UE_SCHEDULED];
}ULUserListRR;
/* ICIC Change End */
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
#ifdef TDD_CONFIG
MacRetType  assignULRBsAndSendToPDCCHRoundRobin(
        UInt32 ueIndex,
        UInt32 allocatedRB,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 strictAllocRBFlag,
        UInt32 ulHarqProcessId,
        /* ULA_CHG */
        UInt32 minDataSize,
        UInt32 maxDataSize,
        /* ULA_CHG */
        /* ATB Changes Start */
        UInt8 mcsVal
        /* ATB Changes End */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        /* TDD Config 0 Changes Start */
        ,UInt8 ulSubFrameNum
        /* TDD Config 0 Changes End */
        );
#elif FDD_CONFIG        
MacRetType  assignULRBsAndSendToPDCCHRoundRobin(
        UInt32 ueIndex,
        UInt32 allocatedRB,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 strictAllocRBFlag,
        /* ULA_CHG */
        UInt32 minDataSize,
        UInt32 maxDataSize,
        /* ULA_CHG */
        /* ATB Changes Start */
        UInt8 mcsVal
        /* ATB Changes End */
        ,UInt8 ttiBundling
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );
#endif

extern MacRetType processMeasGapValidation(ULUEContext *ulUEContext_p,
                                  /* SPR 15909 fix start */       
                                  tickType_t currentGlobalTTITickCount,
                                  /* SPR 15909 fix end */
                                  UInt32 ulDelay);

#endif  /* LTE_MAC_UL_ROUND_ROBIN_SCHEDULER_H */
