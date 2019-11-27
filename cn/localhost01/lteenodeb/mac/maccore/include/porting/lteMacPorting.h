
/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: lteMacPorting.h 
 *
 *************************************************************************
 *
 * File Description : This file contains the declaration of functions that
 *                    are used for Mac porting
 *************************************************************************/

#ifndef LTE_MAC_RANDOM_ACCESS_DEMUX_H
#define LTE_MAC_RANDOM_ACCESS_DEMUX_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacComPorting.h"
/****************************************************************************
 * Exported Types
 ****************************************************************************/

/*CA Changes start  */
/* +- SPR 17777 */
void initDLAckNackQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex);
void initUlStrategyQueue( 
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex);
#ifndef DL_UL_SPLIT_TDD
void initPUSCHSignalQueue(
#if defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL)
        SInt32 * qId, 
#endif    
        InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
#else
void initPDCCHSignalQueue();
void putEntryInUlPdcchSignalQueue(UInt32 availableCCE);
extern SEM_T waitForPDCCHSignalInd_g;
extern LTE_CIRCQUEUE pdcchSignalCircQueue_g;
#endif

void initULContainerNackQueue ( SInt32 *qId, InternalCellIndex cellIndex, UInt8 containerIndex);
void initULContainerNackQueueForTempUE( SInt32 *qId, InternalCellIndex cellIndex, UInt8 containerIndex);
/*CA Changes end  */

extern void cleanupUlStrategyQueue(InternalCellIndex cellIndex);
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#endif
