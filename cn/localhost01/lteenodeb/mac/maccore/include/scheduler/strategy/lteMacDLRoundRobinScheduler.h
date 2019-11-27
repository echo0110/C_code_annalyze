/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLRoundRobinScheduler.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacDLRoundRobinScheduler.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.15  2009/07/24 12:39:09  gur20052
 * Splint Warnings are removed
 *
 * Revision 1.14  2009/07/13 11:05:05  gur20052
 * Removed code in #if 0
 *
 * Revision 1.13  2009/06/29 15:09:34  gur20052
 * UT Defects are removed
 *
 * Revision 1.12  2009/06/23 16:55:58  gur20052
 * Review comments are incorporated
 *
 * Revision 1.10  2009/06/10 14:27:58  gur20052
 * Changed the signature of function
 *
 * Revision 1.9  2009/06/08 08:22:20  gur20052
 *  review comments incorporated
 *
 * Revision 1.8  2009/06/05 16:20:48  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.7  2009/06/05 06:33:06  gur20052
 * incorporated the Review comments
 *
 * Revision 1.6  2009/06/02 07:19:29  gur20052
 * Updated during comments incorporation and dispostion
 *
 * Revision 1.5  2009/05/28 11:33:52  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.4  2009/05/26 10:16:52  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.3  2009/05/21 06:25:59  gur20052
 * Added code for Round Robin Scheduler
 *
 * Revision 1.2  2009/05/20 16:55:14  gur20052
 * Updated the initial version
 *
 * Revision 1.1  2009/04/30 16:30:20  gur20052
 * Added initial version of  DL Round Robin Header File
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_DL_ROUND_ROBIN_SCHEDULER_H
#define LTE_MAC_DL_ROUND_ROBIN_SCHEDULER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteMacDLSchedulerStrategy.h"
#include "lteMacDLFairScheduler.h"
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
/*ICIC changes start */
#define MIN_RB_REQUIRED_FOR_RR 1

/* This stores the information required to schedule node in 
 * cell center and cell edge list.*/
typedef struct DLSchedulableUENodeT
{
    /*DL UE Context*/
    DLUEContext* ueDLContext;

     /*This Will contain the different type of MsgType*/
    MsgType    msgType;

    /* Harq process allocated*/
    DLHARQProcess *dlHARQProcess;
}DLSchedulableUENode;

/* This stores the cell center and cell edge list that is going to be
 * scheduled */
typedef struct DLUserListRRT
{
    /* Number of UE */
    UInt8 count;

    /* Array will store info for Cell Center and Cell Edge user which are going
     * to be scheduled */
    DLSchedulableUENode dlSchedulableUENode[
         MAX_DL_UE_SCHEDULED];
}DLUserListRR;


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt32 dlRoundRobinScheduler
        (
        /* SPR 21068 start */ 
        tickType_t referenceFlag,
        /* SPR 21068 end   */
        UInt32 *numberOfAvailableRBs_p,
        DLSchedulerDataElement *dlScheduleData_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        UInt8  maxUEsToBeScheduledDL, /* RETX_CHG */
        InternalCellIndex internalCellIndex
        );
/* SPR 2446 Fix Begins*/
MacRetType  assignDLRBsAndSendToPDCCHFromRoundRobin
        ( 
        DLUEContext* ueDLContext_p,
        UInt32 * availableRB_p,
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
#endif
        UInt32 allocatedRBFromRoundRobin );
/* SPR 2446 Fix Ends*/

/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType processDLRoundRobinNewDataGeneral(
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt32 allocatedRBFromRoundRobin,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        MsgType msgType ,
        DLHARQProcess *dlHarqProcess
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
       ,tickType_t currentGlobalTTITickCount
       /* SPR 15909 fix end */
#endif      
       ,InternalCellIndex internalCellIndex
        );

MacRetType processDLRoundRobinNewDataWithOneTBOnly
        (
        UInt32 allocatedRBFromRoundRobin,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        UInt32 numLCWithNonZeroQueueLoad,
        MsgType msgType
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
       ,tickType_t currentGlobalTTITickCount
       /* SPR 15909 fix end */
#endif      
       ,InternalCellIndex internalCellIndex
        );
MacRetType processDLRoundRobinNewDataWithTwoTB
        (
        UInt32 allocatedRBFromRoundRobin,
        UInt32 * availableRB_p,
        DLUEContext* ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        UInt32 numLCWithNonZeroQueueLoad,
        MsgType msgType
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
       ,tickType_t currentGlobalTTITickCount
       /* SPR 15909 fix end */
#endif      
       ,InternalCellIndex internalCellIndex
        );
    

#endif  /* LTE_MAC_DL_ROUND_ROBIN_SCHEDULER_H */
