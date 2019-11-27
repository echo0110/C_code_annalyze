/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPRSMgr.h,v $
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the data type information that 
 *                     is used by PRS  manager
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacPRSMgr.h,v $
****************************************************************************/
#ifndef _LTE_MAC_PRS_MANAGER_H_
#define _LTE_MAC_PRS_MANAGER_H_
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacCellConfMgr.h"
#include "lteMacDLResourceAllocator.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define TOTAL_NUM_ROW_IN_PRS_CONFIG 4
#define MAX_PRS_PERIODICITY 1280
#define  MAX_RES_REQUIRED_TO_SCHEDULE_SI_PAGING 6
#define MAX_NUM_PRS_MAPS 2
#define CHECK_REMAINING_RES_WITH_ICIC_PRS_ON(dlAvailableRBs,requiredRBs,\
                                 rbToResourceTypeMapDL_p,prsReserveRBMap_p)\
{\
    UInt8 idx = 0;\
    for (idx = 0; idx < dlAvailableRBs; idx++)\
    { \
	if (rbToResourceTypeMapDL_p[idx] && prsReserveRBMap_p[idx])\
	{\
	    requiredRBs++;\
	    if (requiredRBs == MAX_RES_REQUIRED_TO_SCHEDULE_SI_PAGING) \
	    {\
		break;\
	    }\
	}\
    }\
}

typedef enum PRSChangedParamtersT
{
  PRS_CONFIGURATION_INDEX_CHANGE = 1,
  PRS_BANDWIDTH_CHANGE = 2,
  PRS_TRANSMISSION_POWER_CHANGE = 3,
  PRS_SUBFRAME_CHANGE = 4,
  PRS_MUTING_CONFIGURATION_CHANGE = 5

}PRSChangedParamters;

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef struct PRSConfigT
{
    UInt16 prsConfigStartIndex;
    UInt16 prsConfigEndIndex;
    UInt16  prsPeriodicity;
    SInt16 prsSubframeOffset;
}PRSConfig;

typedef struct PRSPeriodicNodeT
{
  LTE_LIST_NODE nodeAnchor;
}PRSPeriodicNode;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Functions declaration
 ****************************************************************************/

/*CA Changes start  */
void initPRSMaps(PRSParseConfigInfo prsParseConfigInfo,
                       DlBandwidth  dlBandwidth,
                       InternalCellIndex internalCellIndex);
void cleanupPRSMgr(UInt8 prsIndex, InternalCellIndex internalCellIndex);
/* SPR 15909 fix start */
void processPrsTransmission(tickType_t currnetGlobalTick, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */
void  reconfigureParsePRSConfig(PRSParseConfigInfo *prsParseConfigInfo_p, InternalCellIndex internalCellIndex);
void reconfigurePRSConfig(PRSParseConfigInfo *prsParseConfigInfo_p, InternalCellIndex internalCellIndex);
/*CA Changes end  */
 
void initPRSRATPoliciesBitmaps(ResourceAllocationPolicyInfo *raPolicyInfo_p, 
                               UInt8 prsIndex,
                               InternalCellIndex internalCellIndex);
#endif



