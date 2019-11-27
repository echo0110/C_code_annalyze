/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacMeasGapMgr.h,v $
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the data type information that are
 *  used by the Measurement Gap manager
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacMeasGapMgr.h,v $
****************************************************************************/
#ifndef LTE_MAC_MEAS_GAP_MGR_H
#define LTE_MAC_MEAS_GAP_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#ifdef DL_UL_SPLIT
#include "lteCircQueue.h"
#endif
#include "lteMacULUEContext.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
extern DL_MEAS_GAP_ON_OFF_TRIGGER_TYPE* dlMeasGapOnOffTriggerQueue_gp[MAX_NUM_CELL];

extern UInt16 maxNumOfUeSupported_g;
extern void initMeasGapMgr(UInt8 numOfCells);
/* SPR 15909 fix start */
extern UInt8 isTTIInMeasGapPeriod(ULUEContext *ulUEContext_p, tickType_t tti);
/* SPR 15909 fix end */
/* SPR 5599 changes start (ZIP ID 129059) */
extern UInt8 getTddConfig(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define MEASUREMENT_GAP_PERIODICITY_40                    40
#define MEASUREMENT_GAP_PERIODICITY_80                    80
#ifdef ENDC_ENABLED
 /*Meas_Gap_Changes_Start*/
#define MEASUREMENT_GAP_PERIODICITY_20                    20
#define MEASUREMENT_GAP_PERIODICITY_160                   160
 /*Meas_Gap_Changes_End*/
#endif
#define CHECK_UE_MEAS_GAP_PERIOD(measgapContext_p)        \
                                        (measgapContext_p->isMeasGapPeriodOn)
#define MEAS_GAP_DURATION 6

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
typedef enum MeasGapPatternIdT
{
    GP0 = 0,
    GP1
#ifdef ENDC_ENABLED
	/*Meas_Gap_Changes_Start*/
	,GP4_R15 = 4,
	GP5_R15,
	GP6_R15,
	GP7_R15,
	GP8_R15,
	GP9_R15,
	GP10_R15,
	GP11_R15
	/*Meas_Gap_Changes_End*/
#endif
}MeasGapPatternId;
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Functions declaration
 ****************************************************************************/
#endif /* LTE_MAC_MEAS_GAP_MGR_H */
