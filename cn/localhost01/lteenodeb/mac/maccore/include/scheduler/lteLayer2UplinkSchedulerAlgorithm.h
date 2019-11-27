/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2UplinkSchedulerAlgorithm.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.6  2009/04/10 17:26:26  gur18569
 * merged with optmization changes
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
#if 0   //ysl remove unused code 2019.10.29
#ifndef INCLUDED_FILENAME_H
#define INCLUDED_FILENAME_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2Types.h"
#include "lteLayer2UplinkUEContext.h"
#include "lteLayer2UplinkScheduler.h"
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
    UInt32  weightedPriority;          //value calculated by applying scheduler algorithm for this ue
    UInt32     weightedBsrLoad;
    UInt32     numLCGWithNonZeroBsrLoad;
} UplkSchInfoNode;

typedef struct uplkSchDataElementT {
    UInt16  totalUE;
   /* SPR 15909 fix start */
    tickType_t  tti;
   /* SPR 15909 fix end */
    UInt32 sumWeightedPriority;
    UplkSchInfoNode schInfoArr[MAX_NUM_UE];
} uplkSchDataElement;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
 
UInt32 calculateUplinkWeightedPriority(UInt16 tempUeIdx,
              UInt32 bsrLoadWeightedMean_g, UplkSchInfoNode * schedulerInfoNode_p);
/* SPR 15909 fix start */
MacRetType allocUplinkRBsUsingMergesort
                 (UInt8 totalRBs, uplkSchDataElement * scheduleData, tickType_t tempTti );
                 /* SPR 15909 fix end */
UInt32 validateUplinkRB(UInt16 tempUeId ,UInt32 allocatedRB,UInt32 tempTtiCounter,UInt8 tempAvailableRB );
void mergesortUplinkSchedulerData(UplkSchInfoNode* arrToBeSorted,
                                        UInt32 low,UInt32 high);
/* SPR 15909 fix start */
UInt32 prepareAndSendULAllocation(UInt16 ueIdx ,UInt32 allocatedRB,tickType_t tempTti);
/* SPR 15909 fix end */

UInt32 determineBSRBufferSize(UInt32 BSRIndex);

#endif  /* INCLUDED_FILENAME_H */
#endif
