/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacTimingAdvanceMgr.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacTimingAdvanceMgr.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.4  2009/06/24 13:58:39  gur19413
 * node structure changed.
 *
 * Revision 1.3  2009/06/10 13:25:56  gur19413
 * comments incorporated
 *
 * Revision 1.2  2009/06/04 10:11:32  gur19413
 * remove compilation error
 *
 * Revision 1.1  2009/05/10 09:08:12  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_TIMING_ADVANCE_MGR_H 
#define LTE_MAC_TIMING_ADVANCE_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteSQueue.h"
#include "lteCircQueue.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* TA_CHG */
/* SPR 2628 Changes Start */
#define TA_BETA_FACTOR 3180
#define TA_GAMMA_FACTOR 3030
#define MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31            31
#define MAC_TIMING_ADVANCE_VAL_MAX                      63

#ifdef FLEXRAN
#define MAC_TIMING_ADVANCE_DEFAULT_VAL					1282
#endif

/* SPR 2628 Changes End */
/* TA_CHG */
#define TA_COOL_OFF_PERIOD 9

/*SPR 6333 TI_TA_FIX_START*/
#define MAC_ALPHA_COEFFICIENT_IIR_TA 820
/*SPR 6333 TI_TA_FIX_START*/

/*Node structure of the TA Queues */
typedef struct TimeAdvanceQueueNodeT
{
    
    LTE_QUEUE_NODE   timeAdvanceQueueNodeAnchor;
    /*UE ID of the UE*/
    UInt16      ueIdx;
    /*Timing Advance of the UE*/
    UInt16      timeAdvanceValue;

}*LP_TimeAdvanceQueueNode, TimeAdvanceQueueNode;
                 

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* Compilation Fix Start */
/*CA Changes start  */
extern TA_Q_TYPE* dlTimeAdvanceQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */
/*Queue of type LTE_SQUEUE which contains indexes of UEs having TA Request
 * because of failure in PDCH and PDSCH
 */
extern TA_Q_TYPE *pdcchPdschFailureTAQueue_gp[MAX_NUM_CELL];
/* Compilation Fix End */

#ifdef LOG_PRINT_ENABLED

/* SPR 15909 fix start */
/* + SPR 17439 */
extern tickType_t getCurrentTick(void);
/* - SPR 17439 */
/* SPR 15909 fix end */

#endif

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* This function Initialize the Timing Advance global queue*/
/*CA Changes start  */
void initTimingAdvanceMgr(UInt8 numCellsConfigured);
void cleanupTimingAdvanceMgr(InternalCellIndex internalCellIndex);
void deInitTimingAdvanceMgr(InternalCellIndex internalCellIndex);
/*CA Changes end  */

/* This function is used to entry in to the Timing Advance Global Queue */
void putEntryInTimingAdvanceQueue( UInt32 ueIndex, 
                                   UInt16 timeAdvanceValue,
                                   InternalCellIndex internalCellIndex
                                 );
void putEntryInPdcchPdschFailureTAQueue( UInt32 ueIndex, 
                                         UInt16 timeAdvanceValue,
                                         InternalCellIndex internalCellIndex);

#endif  /* LTE_MAC_TIMING_ADVANCE_MGR_H */

