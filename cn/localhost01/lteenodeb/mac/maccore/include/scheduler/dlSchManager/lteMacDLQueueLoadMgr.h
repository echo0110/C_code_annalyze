/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLQueueLoadMgr.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : It contains macro declarations , function declarations , 
  *         structure declarations required by lteMacDLQueueLoad.c
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacDLQueueLoadMgr.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.6  2009/06/05 16:21:26  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.5  2009/05/28 11:40:22  gur20052
 * Updated to incorporated UE Context Changes
 * CV: ----------------------------------------------------------------------
 *
 * Revision 1.4  2009/05/28 11:37:54  gur20052
 * removed addDeleteTrigger
 *
 * Revision 1.3  2009/05/11 09:27:30  gur20052
 * Review Comments are incorporated
 *
 * Revision 1.2  2009/05/09 10:30:34  gur20052
 * Added New Logs
 *
 * Revision 1.1  2009/05/04 10:51:32  gur20052
 * Added initial version
 *
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_DL_QUEUE_LOAD_MANAGER_H
#define LTE_MAC_DL_QUEUE_LOAD_MANAGER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacDLUEContext.h"

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
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* SPR 5599 changes start (ZIP ID 129059) */
 void updateUEQueueLoad
/* SPR 5599 changes end (ZIP ID 129059) */
(
        DLUEContext * ueContext_p,
        UInt32 qLoad,
        /* SPR 1273 Start*/ 
        BOType boFlag
        /* SPR 1273 End*/ 
); 

extern  void putEntryInZeroNonZeroQueueLoadTriggerQueue
(
        DLUEContext * ueContext_p, 
        AddDeleteTrigger addDeleteTrigger , InternalCellIndex internalCellIndex);

void dlQosPutEntryInZeroNonZeroQueueLoadTriggerQueue(DLUEContext * ueContext_p, 
        AddDelTrigLCNode *addDelTrigLcNode_p, UInt8 lcTrigCount, InternalCellIndex internalCellIndex);
extern void putEntryInDLPriorityQueue( UInt32 ueIndex,
                                       InternalCellIndex internalCellIndex);
#endif  /* FILENAME_H */

