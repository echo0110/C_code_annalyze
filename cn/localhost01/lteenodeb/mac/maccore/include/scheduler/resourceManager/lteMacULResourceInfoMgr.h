/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULResourceInfoMgr.h $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
****************************************************************************/

#ifndef LTE_MAC_UL_RESOURCE_INFO_MGR_H
#define LTE_MAC_UL_RESOURCE_INFO_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacRRMInterface.h"
#include "lteMacULResourceAllocator.h"
/* +DYNAMIC_ICIC */
#include "lteMacParseUtil.h"
/* -DYNAMIC_ICIC */
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_RES_BLOCKS 100
#define MAX_RA_INFO_INDEX 2
/* -DYNAMIC_ICIC */
#define INVALID_UL_POWER_MASK (MAX_PC_MAX + 1)
MacRetType configULPowerMaskInfo(ULPowerMask *ulPowerMask_p, 
                                 InternalCellIndex internalCellIndex);
/* +DYNAMIC_ICIC */
MacRetType configureULResourceAllocationInfo(
        ULResourcePartitionConfigInfo *resourcePartitionInfo_p,
        InternalCellIndex internalCellIndex);

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* Keeps information about RB region type */ 
typedef struct RbMapInfoT
{
    /*Indication whether the rb belongs to 
     * 0 - Blocked region/
     * 1 - Cell Center region
     * 2 - Cell  Edge region */
    UInt8    regType;
    /* This count is applicable for blocked region only.
     * this specifies the blocked region count including itself*/
    UInt8    blockedCount;
}RbMapInfo;

typedef struct ULResourceAllocationInfoT
{
    /* Stores RB Map Node */
    RbMapNode rbMapInitInfo;

    /* Stores information about each PRB */
    RbMapInfo rbMapRegion[MAX_NUM_RBS +1];

    /* Indicates total RB available in cell edge region */
    UInt8 totalCellEdgeRB;

    /* Indicates total RB available in cell center and cell edge region 
     * excluding RB in blocked region */
    UInt8 totalAvailableRB;

}ULResourceAllocationInfo;

typedef struct ULPowerMaskInfoT
{
    /* +DYNAMIC_ICIC */
    /* keeps the Power Mask of the region */
    SInt32 powerMask[MAX_USER_LOCATION];
    /* This array contains delta power mask for all QCIs */
    SInt32 qciDeltaPowerMask[MAX_QCI];
    /* -DYNAMIC_ICIC */
}ULPowerMaskInfo;
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*CA Changes start  */
extern ULResourceAllocationInfo* ulRAInfo_gp[MAX_NUM_CELL];
extern UInt8 ulRAInfoIndex_g[MAX_NUM_CELL];

/* for storing partition info */
extern ULResourcePartitionConfigInfo* ulResourcePartitionConfig_gp[MAX_NUM_CELL];
extern ULPowerMaskInfo* ulPowerMaskInfo_gp[MAX_NUM_CELL];
extern UInt8 ulPowerMaskIndex_g[MAX_NUM_CELL];

void initULResourceInfo(UInt8 numOfCells);
void deInitULResourceInfo (InternalCellIndex internalCellIndex);

void initUlResInfo (InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
void deInitUlResInfo (InternalCellIndex internalCellIndex);
/*CA Changes end  */


#endif
