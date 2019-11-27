/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLResourceInfoMgr.h $
 *
 ****************************************************************************
 *
 *  File Description : This file contains definition of structure which store
 *                     DL Resource Information
 *
 ****************************************************************************
****************************************************************************/

#ifndef LTE_MAC_DL_RESOURCE_INFO_MGR_H
#define LTE_MAC_DL_RESOURCE_INFO_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacRRMInterface.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacRRMIntfInfo.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
#define MAX_RA_INFO_INDEX 2
#define MIB_OVERLAP_RB_COUNT 6
typedef struct DLResourceAllocationInfoT
{
    /* Indicates total RB available in cell edge region */
    UInt8 totalCellEdgeRB;

    /* Indicates total RB available in cell center and cell edge region 
     * excluding RB in blocked region 
     * */
    UInt8 totalAllowedRB;
    
    /* Stores RAT0, RAT1, RAT2 Localized/Distributed */
    ResourceAllocationPolicyInfo raPolicyInfo;
    
    /* Stores RAT0, RAT1, RAT2 Localized/Distributed reserving central 6 RBs */
    ResourceAllocationPolicyInfo raPolicyInfoMIBReserved;

    /* + PRS_CHANGES */ 
   /* Stores RAT0, RAT1, RAT2 Localized/Distributed reserving PRS RBs */
    ResourceAllocationPolicyInfo raPolicyInfoPRSReserved;
    /* - PRS_CHANGES */ 
    /* Indicates if RB is available in cell center, cell edge region or blocked
     * Value 0 -> Blocked
     * Value 1 -> Cell Center RB
     * Value 2 -> Cell Edge RB
     * */
    UInt8 rbToResourceTypeMapDL[MAX_RES_BLOCKS];
}DLResourceAllocationInfo;


/* This structure contains information about the Cell Center/Cell Edge
 * Frequency Partitions for DL */
typedef struct DLResourcePartitionConfigInfoT{
    /* indicates number of Cell Center Frequency Partitions */
    UInt32 cellCenterRegionCount;

    /* indicates number of Cell Edge Frequency Partitions */
    UInt32 cellEdgeRegionCount;
    
    /* indicates total non-blocked cell edge RBs */
    UInt32  totalCellEdgeRBDL; 

    /* indicates total non-blocked available RBs */
    UInt32  totalAvailableRBDL;   // Non Blocked

    /* This structure contains information about cell center and cell 
     * edge resource info for DL*/
    ResourceInfo ccResourceInfo[MAX_REGIONS];

    ResourceInfo ceResourceInfo;

}DLResourcePartitionConfigInfo;


/****************************************************************************
 * Exported Constants 
 ****************************************************************************/
#define MIN_RB_REQD_FOR_COMM_CH 2
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern DLResourceAllocationInfo* dlRAInfo_gp[MAX_NUM_CELL];
extern UInt8 dlRAInfoIndex_g[MAX_NUM_CELL];
extern SInt8 excessPoolRb_g[MAX_NUM_CELL][MAX_USER_LOCATION];

/* for storing partition info */
extern DLResourcePartitionConfigInfo dlResourcePartitionConfig_g[MAX_NUM_CELL];

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
MacRetType configureDLResourceAllocationInfo(
        DLResourcePartitionConfigInfo *resourcePartitionInfo_p,
        InternalCellIndex internalCellIndex);
void initializeRAPolicyMibScheduled(UInt8 dlRAInfoIndex, InternalCellIndex internalCellIndex);
void macInitDLResourceInfoMgr(InternalCellIndex numOfCells);
void initDlResInfo (InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
void deInitDlResInfo (InternalCellIndex internalCellIndex);

#define UPDATE_EXCESS_POOL_RB(resourceRegionAllocated,rbCount,internalCellIndex)                 \
    excessPoolRb_g[internalCellIndex][resourceRegionAllocated] += rbCount;                       

#define UPDATE_CELL_CENTRE_EXCESS_POOL(previousCellEdgeRBCont,internalCellIndex)                 \
    excessPoolRb_g[internalCellIndex][CC_CE_REGION] += (excessPoolRb_g[internalCellIndex][CE_REGION] -               \
            previousCellEdgeRBCont)
 
#endif
