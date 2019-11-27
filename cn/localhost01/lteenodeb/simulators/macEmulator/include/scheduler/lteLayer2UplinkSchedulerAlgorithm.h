/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2UplinkSchedulerAlgorithm.h,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
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
 * $Log: lteLayer2UplinkSchedulerAlgorithm.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2  2009/07/14 14:45:05  gur20439
 * rrc interface code.
 *
 * Revision 1.1  2009/03/30 10:20:31  gur19836
 * Initial Version
 *
 * Revision 1.5  2009/01/15 05:16:01  gur20052
 * Review comments of vandana are incorporated
 *
 * Revision 1.4  2009/01/08 13:15:31  gur20052
 * UT Defects are incorporated
 *
 * Revision 1.3  2009/01/05 07:58:37  gur20052
 * changes of UT Defets are incorporated
 *
 * Revision 1.2  2008/12/19 07:12:49  gur20052
 * Review Comments for Phase 2 Incorporated
 *
 * Revision 1.1  2008/12/08 12:54:29  gur12905
 * Initial Version phase -2
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_FILENAME_H
#define INCLUDED_FILENAME_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#if 0
#include<stdio.h>
#include "lteLayer2Types.h"
#include "lteLayer2UplinkUEContext.h"
#include "lteLayer2UplinkScheduler.h"
#include "time.h"
#include "lteLayer2Tick.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

typedef struct uplkSchInfoNodeT {
    UInt16     ueIdx;
    UInt8      numOfRBs;
    UDouble32  weightedPriority;          //value calculated by applying scheduler algorithm for this ue
    UInt32     weightedBsrLoad;
    UInt32     numLCGWithNonZeroBsrLoad;
} UplkSchInfoNode;

typedef struct uplkSchDataElementT {
    UInt16  totalUE;
    UInt32  tti;
    //UDouble32 maxWeightedPriority;
    UDouble32 sumWeightedPriority;
    UplkSchInfoNode schInfoArr[MAX_NUM_UE];
} uplkSchDataElement;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
 
UDouble32 calculateUplinkWeightedPriority(UInt16 tempUeIdx,
              UDouble32 bsrLoadWeightedMean_g, UplkSchInfoNode * schedulerInfoNode_p);
MacRetType allocUplinkRBsUsingMergesort
                 (UInt8 totalRBs, uplkSchDataElement * scheduleData, UInt32 tempTti );
UInt32 validateUplinkRB(UInt16 tempUeId ,UDouble32 allocatedRB,UInt32 tempTtiCounter,UInt8 tempAvailableRB );
void mergesortUplinkSchedulerData(UplkSchInfoNode* arrToBeSorted,
                                        UInt32 low,UInt32 high);
/*static void mergeSubArray(UplkSchInfoNode arrToBeSorted[], UInt32 low, 
                           UInt32 high, UInt32 mid);*/
UInt32 prepareAndSendULAllocation(UInt16 ueIdx ,UInt32 allocatedRB,UInt32 tempTti);

UInt32 determineBSRBufferSize(UInt32 BSRIndex);

//UDouble32 calculateUplinkWeightedPriority(UInt16,
//    UDouble32 bsrLoadWeightedMean_g, uplkSchInfoNode * schedulerInfoNode_p);

#endif
#endif  /* INCLUDED_FILENAME_H */

