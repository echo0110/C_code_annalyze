/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacExecutionScheduler.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacExecutionScheduler.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.3  2009/07/15 05:27:47  gur18569
 * changed exec thread name size to 20 from 100
 *
 * Revision 1.2  2009/06/03 10:42:34  gur18569
 * incorporated comments
 *
 * Revision 1.1  2009/05/29 14:27:09  gur18569
 * Initial version
 *
 *
 *
 ****************************************************************************/

#ifndef EXECUTION_SCHEDULER_H
#define EXECUTION_SCHEDULER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define EXECUTION_THREAD_NAME_SIZE 100


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct ExecutionStatsInfoT
{
    UInt32 peakTimeInMicroSec;
    UInt32 avgTimeInMicroSec;
}ExecutionStatsInfo;


/*structure to store internal cell index and execution flow count ie. DL thread
   UL thread or UL RLC, this is required as threads are created in loop hence
   each thread requires its specific arguments*/
typedef struct ExecutionFlowAndCellIndexT
{
    InternalCellIndex cellIndex;
    UInt16 executionFlowCount;
}ExecutionFlowAndCellIndex;

#ifdef DL_UL_SPLIT
extern ExecutionFlowAndCellIndex execSchedulerParams[MAX_NUM_CELL][NUM_OF_EXEC_FLOWS];
#else
extern ExecutionFlowAndCellIndex execSchedulerParams[MAX_NUM_CELL];
#endif
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
#ifdef DL_UL_SPLIT_TDD
extern void* executionTDDFlowThread(void* executionFlowThreadArgs_p);
extern void* executionFlowThreadRlc(void * executionFlowThreadArgs_p);
#endif

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt32 diffInMicroSec(TimeSpec *  start, TimeSpec *  end);
MacRetType initExecutionScheduler(UInt8 numOfCells);
/* + SPR 17439 */
void printTimingStats(void);
/* - SPR 17439 */
/* Memory cleanup changes start*/
void cleanupLayer2StartupQueues(InternalCellIndex internalCellIndex);
/* Memory cleanup changes end*/

#endif  /* EXECUTION_SCHEDULER_H */
