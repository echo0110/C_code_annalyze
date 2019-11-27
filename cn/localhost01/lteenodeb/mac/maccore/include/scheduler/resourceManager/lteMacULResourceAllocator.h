/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULResourceAllocator.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the function and structure used
 *                     by UL Resource Allocator.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacULResourceAllocator.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.9  2009/08/20 06:56:58  gur19413
 * added new parameter for UL Stat.
 *
 * Revision 1.8  2009/07/28 00:09:17  gur19413
 * added new Macro
 *
 * Revision 1.7  2009/07/13 04:44:14  gur19413
 * added new macro for log
 *
 * Revision 1.6  2009/06/22 09:22:57  gur19413
 * change function prototype
 *
 * Revision 1.5  2009/06/08 10:34:41  gur19413
 * dynamic array to static in RbMapNode
 *
 * Revision 1.4  2009/06/08 06:24:50  gur15697
 * getPUSCHResources() changed to getFreePUSCHResources()
 *
 * Revision 1.3  2009/05/26 14:16:53  gur19413
 * added new macro
 *
 * Revision 1.2  2009/05/06 07:42:17  gur19413
 * added allocatedResInfo structure
 *
 * Revision 1.1  2009/04/30 12:16:00  gur19413
 * initial version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_UL_RESOURCE_ALLOCATOR_H
#define LTE_MAC_UL_RESOURCE_ALLOCATOR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacRRMInterface.h"
#include "lteMacRRMIntfInfo.h"
/* COVERITY 10215 10585 10587 69273-69282 START */
#include "lteMacULUEContext.h"
/* COVERITY 10215 10585 10587 69273-69282 END */
/* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
#ifdef FDD_CONFIG
#include "lteMacRandomAccess.h"
#endif
/* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define RB_DEFINETELY_REQD          0x01
#define RB_REQUIRED                 0x00
/* SPR 1868 changes start */
#ifdef FDD_CONFIG
/* changed to 16 from 20  TTI BUNDLING starts*/
#define NUM_OF_RB_MAP               16
                        /* TTI BUNDLING ends*/ 
#elif TDD_CONFIG
#define NUM_OF_RB_MAP               25
#endif
/* SPR 1868 changes end */
#define BUSY_RESOURCE               1
#define FREE_RESOURCE               0
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
#define BARRED_RESOURCE             255
#endif
/* SPR 21958 PUSCH RAT1 Support End */
/* START SPR 1350 CHANGES */
#define RESERVED_RBS_DL_ACK_NACK    1
/* END SPR 1350 CHANGES */
#define MAX_NUM_RBS                 100
#define UL_RESOURCE_ALLOCATOR       "UL Resource Allocator"
#define RESERVED_RBS_PUCCH_REPORTS  1
/*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
#define BUDDY_UE_SET 1
#define BUDDY_UE_NOT_SET 0
#endif
/*SPR 8451 Changes End*/
/****************************************************************************
 * Exported Types
 ****************************************************************************/


/* ICIC changes start */
/* Available Resources Information */
typedef struct AvailableResInfoT{
     /* It represents the total number
      * of free resource blocks.*/
    UInt8 freeRBCounter;
    /* keeps starting index for free locations*/
    UInt8 freeRBIndex;
    /* Keeps the end index of the region(Cell Center or Cell Edge) */
    UInt8 endIndex;
    /* TDD Config 0 Changes Start 
     * number of resource block which are configured 
     * in particular partiton
     */
    UInt8 initAvailRes;
    /* TDD Config 0 Changes End */
}AvailableResInfo; 
/* ICIC changes end */



/* RB Map Info*/
typedef struct RbMapNodeT
{
   /*It represent the flag for PUCCH Resources 
     are allocated or not allocated.*/
   UInt8 pucchFlag;
   /*It represents the the starting Minimum Index 
     of the resource block Map.*/
   UInt8 minIndex;
   /* This flag signifies whether the resource partition changes from RRM in
    * map is already taken */
   UInt8 advanceAllocFlag;
   /* Keeps the start index of the region(Cell Center or Cell Edge) */
   UInt8 startIndex;
   /*It represent the flag for PRACH Resources 
     are allocated or not allocated.*/
   /*coverity-530 CID-32309*/
   BoolRetType prachFlag;
   /*coverity-530 CID-32309*/
   /* Index 0 contains the information about Cell Center user and
    * Index 1 contains the information about Cell Edge user */
   AvailableResInfo availableResourceInfo[MAX_USER_LOCATION]; 
   /* It represents the rb Map where resources are reserved*/
   UInt8 rbMap[MAX_NUM_RBS +1];
   /* SPR 9729 fix start */
   /* Flag to indicate if RACH info needs to be filled in UL Config */
   UInt8 uciRachFlag;
   /* SPR 9729 fix end */
#ifdef TDD_CONFIG
   /* Stores the number of RBs allocated for the PRACH */
   UInt8 allocatedPrachNumRbs;
   /* TDD config 0 changes start */
   /* Nack count in a particular UL Subframe for PUSCH Retransmission */
/*SPR 21769 Changes Start*/
   UInt16 nackQueueCount;
/*SPR 21769 Changes End*/
   /* Total Retx RB in particular UL Subframe for PUSCH Retransmission  */
   UInt16 totalRetxRb[MAX_USER_LOCATION];
   /*  aggregateMaxRBsRequired for UL MU MIMO */
   UInt16 aggregateMaxRBsRequired;
   /*Total available UL RB in a particular UL subframe  */
   BufferPool ulAvailRB; 
   /* TDD config 0 changes end */
#endif
   /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
   UInt8  rbgMapForRAT1[MAX_RBG_INDEX];
   UInt32 rat1bitmap;
#endif
   /* SPR 21958 PUSCH RAT1 Support End */
}RbMapNode;

/* This structure contains information about the Cell Center/Cell Edge
 * Frequency Partitions for UL */
typedef struct ULResourcePartitionConfigInfoT{
    /* indicates number of Cell Center Frequency Partitions */
    UInt32 cellCenterRegionCount; 
    /* indicates number of Cell Edge Frequency Partitions */
    UInt32 cellEdgeRegionCount;
    /* This structure contains information about cell center and cell
     * * edge resource info for UL*/
    
    /* indicates total non-blocked cell edge RBs */
    UInt32 totalCellEdgeRBUL;   
    /* indicates total non-blocked available RBs */
    UInt32 totalAvailableRBUL;  
    
    ResourceInfo ccResourceInfo[MAX_REGIONS];
    ResourceInfo ceResourceInfo;
}ULResourcePartitionConfigInfo;

/* Start of change for VARIBLE Declaration */
/*CA Changes start  */
extern ULResourcePartitionConfigInfo* ulResourcePartitionConfig_gp[MAX_NUM_CELL];
/*CA Changes end  */
/* End of change for VARIBLE Declaration */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*CA Changes start  */
extern RbMapNode* rbMapNodeArr_gp[MAX_NUM_CELL];
/* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
extern UInt8 rbsForPUCCH_g[MAX_NUM_CELL]; 
#else
extern UInt8 rbsForPUCCH_g[MAX_NUM_CELL][MAX_SUBFRAME];
#endif
/*TDD Config 0 Changes End */
/*CA Changes end  */
/****************************************************************************
 * Exported Functions
****************************************************************************/

/*This function initialize all resource block map with default values*/
/*CA Changes start  */
void initializeResourceMap(InternalCellIndex internalCellIndex);
/*CA Changes end  */

/*This function allocate resoruces for PUCCH Reports */
MacRetType allocateResourcesForPUCCHReports( UInt32 prbIndexSlot1,
                                             UInt32 prbIndexSlot2, 
                                             UInt32 rbMapNum,
                                             InternalCellIndex internalCellIndex
                                           );


/* This function check that PUCCH Resources is allocated or not*/
MacRetType isPUCCHResourcesAllocated(UInt32 rbMapNum,
#ifdef TDD_CONFIG
        UInt32 numPrachFreqs,
#endif
        InternalCellIndex internalCellIndex);

/* This function will set the Flag if PRACH resource is allocated. */
void setPRACHResourcesAllocated(UInt32 rbMapNum, 
#ifdef TDD_CONFIG
        UInt32 numPhysicalResources, 
#endif
        InternalCellIndex internalCellIndex);

    /*coverity-530 CID-32309*/
BoolRetType isPRACHResourcesAllocated(UInt32 rbMapNum, InternalCellIndex internalCellIndex);
    /*coverity-530 CID-32309*/

/*This function set the free Rbs in a particular resource Map */
/*SPR 2446 Fix Begins*/
void setFreeRbsAndPUCCHFlag( UInt32 rbMapNum,
        InternalCellIndex internalCellIndex
                           );
/*SPR 2446 Fix Ends*/

/*This function sets the free resource before pucch resource
 * allocation.This function works on advance subframe */
void setFreeRbsAdvanceAndPUCCHFlag( UInt32 rbMapNum, 
                                    InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
                                    ,UInt8 ulSubframeNum
#endif
                                  );

/*This function allocate the resources using PRB Index. */
MacRetType allocateResourcesUsingRBIndex( UInt32 startIndex,
                                          UInt32 length,
                                          UInt32 rbMapNum,
                                          /*CA Changes start  */
                                          InternalCellIndex internalCellIndex
                                          /*CA Changes end  */
                                        );
/*This function allocate the advance resource for NACK */
MacRetType allocateAdvanceResourcesForNACK( UInt32 prbIndex,
                                            UInt32 length,
                                            UInt32 rbMapNum
                                          );

/*This function return the remaining free Resources for PUSCH*/
UInt32 getFreePUSCHResources(UInt32 rbMapNum, InternalCellIndex internalCellIndex);
MacRetType getAvailableRbsForLongestSet(
                            MCSToPrbMapInfo *mcsToPrbInfo_p,
                            UInt8 maxReqRBs,
                            UInt8 *availableRBs_p,
                            RbMapNode *rbMapNode_p,
#ifdef FDD_CONFIG 
                            UInt8 ulDelay,
                            /* +- SPR 17777 */
                           /* SPR 15909 fix start */
                            tickType_t tickToReserve,
                            /* SPR 15909 fix end */
#endif                            
                            /* +- SPR 17777 */
                            ULUEContext* ulUEContext_p,
                            UInt8 *startIdx_p,
                            InternalCellIndex internalCellIndex
                            /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                            ,UInt8 *nDmrs_p
                            ,UInt8 phichSubFrameNum
                            ,UInt8 iPhich
#endif
                            /*TDD Config 0 Changes End*/
                            );
#ifdef FDD_CONFIG
MacRetType allocateUplinkResourcesForTTIBundle( UInt32 numOfResourcesReq,
                                    UInt32 minNumOfResources,
                                    AllocatedResInfo *allocatedResInfo_p,
                                    UInt32 strictAllocatedFlag,
                                    AvailableResInfo *availResInfo_p,
                                    UInt8 *rbMap_p,
                                    UInt8 ulDelay,
                                    /* SPR 15909 fix start */
                				    tickType_t tickToReserve,
                                    /* SPR 15909 fix end */
                                    ULUEContext* ulUEContext_p,
                                    InternalCellIndex internalCellIndex
                                   );
UInt32 reserveAlreadyAvailableResourcesForBundleRetx (
    UInt32 startIndex,
    UInt32 length ,
    AvailableResInfo  *availRes_p,
    /* +- SPR 17777 */
    UInt8  ulDelay,
    /* SPR 15909 fix start */
    tickType_t currentTick,
    /* SPR 15909 fix end */
    InternalCellIndex internalCellIndex
    );
UInt8 checkAvailableRBsToReturnLongestSetForTTIB(
    MCSToPrbMapInfo *mcsToPrbInfo_p, 
    UInt8 maxReqRBs, 
    UInt8 *availableRBs, 
    AvailableResInfo *rbMapNode_p,
    UInt8 *rbMap,
    UInt8 ulDelay,
    /* SPR 15909 fix start */
    tickType_t tickToReserve,
    /* SPR 15909 fix end */
    /* +- SPR 17777 */
    InternalCellIndex internalCellIndex
    );
MacRetType checkNumOfBundleResFromRBStartIndex (
    UInt8 startIndex,
    UInt8 length ,
    /* SPR 15909 fix start */
    tickType_t tickToReserve,
    /* SPR 15909 fix end */
    UInt8 ulDelay,
    InternalCellIndex internalCellIndex
    );
                                /* SPR 15909 fix start */
void enqueueNodesInTTIBScheduleQ(tickType_t enqueueTick,
                                 /* SPR 15909 fix end */
                                 /* +- SPR 18268 */
                                 UInt16 UeIndex ,
                                 /* +- SPR 18268 */
                                 UInt8 harqId,
                                 UInt8 bundleStartTick,
                                 UInt8 isSpsEnabled,
                                 InternalCellIndex internalCellIndex);
MacRetType reserveAlreadyAvailableResForThreeTicks (
    UInt32 startIndex,
    UInt32 length ,                        
    AllocatedResInfo  *res_p,
   /* SPR 15909 fix start */
    tickType_t tickToReserve,
    /* SPR 15909 fix end */
    UInt8 ulDelay,
    ULUEContext* ulUEContext_p,
    UInt8 userLocType,
    InternalCellIndex internalCellIndex
    );
#endif 

/*This function allocate the resource for uplink grant*/
MacRetType allocateUplinkResources( UInt32 numOfResourcesReq,
                                    UInt32 minNumOfResources,
                                    AllocatedResInfo *allocatedResInfo_p,
                                    UInt32 strictAllocatedFlag,
                                    AvailableResInfo *availResInfo_p,
                                    /* SPR 21958 PUSCH RAT1 Support Start */
                                    RbMapNode *rbMapNode_p,
                                    /* SPR 21958 PUSCH RAT1 Support End */
                                    InternalCellIndex internalCellIndex
                                    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                    ,UInt8 *nDmrs_p
                                    ,UInt8 phichSubFrameNum
                                    ,UInt8 iPhich
#endif
                                    /*TDD Config 0 Changes End*/
                                    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
                                    ,UInt8 isRAT1Supported
#endif
                                    /* SPR 21958 PUSCH RAT1 Support End */
                                   );
/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
MacRetType allocateUplinkResourcesForSecondPuschExecutionTddConfig0 (
	        UInt8 startIndex,
       		UInt8 rbLength,
	        AllocatedResInfo *allocatedResInfo_p,
        	AvailableResInfo *availRes_p,
	        UInt8 *rbMap, 
            InternalCellIndex internalCellIndex
		);
#endif
/*TDD Config 0 Changes End*/

/*This function free the resources (in case Negative RAR) */ 
void freeResourceUsingRBIndex ( UInt32 length,
                                UInt32 prbIndex,
                                UInt32 rbMapNum,
                                InternalCellIndex internalCellIndex
                               );

/*This function reset the particualar with default values */
void resetRBsAtLocation(UInt32 rbMapNum,
        InternalCellIndex internalCellIndex);

/* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
#ifdef FDD_CONFIG
void freeAllTTIBRARResources(DLRARsp   *rarToDl,
                             UInt32 rarStartIndex, 
                             UInt32 rarEndIndex,
                             InternalCellIndex internalCellIndex);
#endif
/* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
UInt8 getUplinkMinRBsInPowerOfTwoThreeFive (UInt8 rbNum);

UInt8 getUplinkMaxRBsInPowerOfTwoThreeFive (UInt8 rbNum);

/*TDD Config 0 Changes Start*/
#ifdef FDD_CONFIG
UInt8 checkAvailableRBsToReturnLongestSet(
        MCSToPrbMapInfo *mcsToPrbInfo_p,
        UInt8 maxReqRBs,
        UInt8 *availableRBs,
        AvailableResInfo *availResInfo_p,
        UInt8 *rbMap_p
        );
#elif TDD_CONFIG
MacRetType checkAvailableRBsToReturnLongestSet(
        MCSToPrbMapInfo *mcsToPrbInfo_p,
        UInt8 maxReqRBs,
        UInt8 *availableRBs,
        /* ICIC changes start */
        AvailableResInfo *rbMapNode_p,
        UInt8 *rbMap,
        UInt8 *nDmrs_p,
        UInt8 phichSubFrameNum,
        UInt8 iPhich,
        UInt8 *maxAvailableRBsStartIdx,
        InternalCellIndex internalCellIndex
        );
#endif
/*TDD Config 0 Changes End*/

MacRetType checkNumResourcesFromRBStartIndex (
        /* SPR 5530 changes start */
        UInt8 startIndex,
        UInt8 length ,
        /* SPR 5530 changes end */
        UInt32 rbMapNum,
        InternalCellIndex internalCellIndex
        );

/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
MacRetType checkNumRBGFromRBGSet1StartIndex (
		UInt8 s0,
		UInt8 s1,
		UInt8 s2,
		UInt8 s3,
		UInt8 rbMapNum,
		InternalCellIndex  internalCellIndex
		);
#endif
/* SPR 21958 PUSCH RAT1 Support End */
MacRetType reserveAlreadyAvailableResources (
        UInt32 startIndex,
        UInt32 length ,
        AvailableResInfo  *availRes_p,
        UInt8  *rbMap_p 
        );

/* SPR 7435 Changes Starts */
/* SPR 5599 changes start (ZIP ID 129059) */
 UInt32 getRIV(UInt32 prbIndex , UInt32 length, InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */
/* SPR 7435 Changes Ends */
/* This function reserves length(number) of resources which were 
 * already checked for availability. 
 */
UInt32 reserveAlreadyAvailableResourcesForRetx (
        /* SPR 5530 changes start */
        UInt8 startIndex,
        UInt8 length ,                        
        /* SPR 5530 changes end */
        AvailableResInfo  *availRes_p,
        UInt8  *rbMap_p 
        );

/*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
void checkULCodeRateUECategory(
        ULUEContext* ulUEContext_p,
        UInt8 *tempAllocatedMCS,
        UInt8 *rbAllocated,
        UInt8 buddyUeFlag,
        InternalCellIndex internalCellIndex);
#endif
/*SPR 8451 Changes End*/


#endif  /* LTE_MAC_UL_RESOURCE_ALLOCATOR_H */
