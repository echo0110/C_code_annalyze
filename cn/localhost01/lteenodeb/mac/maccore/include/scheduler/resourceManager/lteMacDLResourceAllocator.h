/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLResourceAllocator.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacDLResourceAllocator.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.28  2009/08/06 09:07:13  gur12140
 * Incorporated Comments
 *
 * Revision 1.27  2009/08/04 06:29:16  gur12140
 * Incorporated Comments
 *
 * Revision 1.26  2009/07/20 05:19:09  gur12140
 * Defined DL_RESOURCE_MGR
 *
 * Revision 1.25  2009/07/08 09:07:45  gur12140
 * Incorporated Commentsd
 *
 * Revision 1.24  2009/07/03 09:07:51  gur12140
 * Incorporated Comments
 *
 * Revision 1.23  2009/07/03 06:37:56  gur12140
 * Incorporated Comments
 *
 * Revision 1.22  2009/06/29 13:45:00  gur12140
 * Incorporated Comments and solved Bug
 *
 * Revision 1.21  2009/06/20 14:54:38  gur12140
 * Incorporated Comments
 *
 * Revision 1.20  2009/06/16 14:32:14  gur12140
 * Incorporated Comments
 *
 * Revision 1.19  2009/06/16 12:58:27  gur12140
 * Incorporated Comments
 *
 * Revision 1.18  2009/06/12 05:05:01  gur12140
 * Incorporated Comments
 *
 * Revision 1.17  2009/06/10 09:07:48  gur12140
 * Incorporated Comments
 *
 * Revision 1.16  2009/06/08 12:10:13  gur12140
 * Removed Linking Errors
 *
 * Revision 1.14  2009/06/05 05:50:45  gur12140
 * Added Funtion Declaration for allocateRBsForCommonChannel
 *
 * Revision 1.13  2009/06/05 05:04:41  gur12140
 * Added variable for Mcs Indec for Common Channel in Resource
 * Allocator Structure
 *
 * Revision 1.12  2009/06/01 13:00:37  gur12140
 * Removed Compilation Error
 *
 * Revision 1.10  2009/05/28 07:25:41  gur12140
 * Data Structure Modified
 *
 * Revision 1.9  2009/05/25 12:47:40  gur12140
 * Removed Some Warnings
 *
 * Revision 1.8  2009/05/25 10:49:37  gur12140
 * Restructuring and Enhancements were made
 *
 * Revision 1.7  2009/05/09 16:32:12  gur12140
 * Added funtions for allocating TBS for Spatial and Non Spatial Multiplexing
 *
 * Revision 1.6  2009/05/08 15:38:50  gur12140
 * Removed Warnings
 *
 * Revision 1.5  2009/05/08 14:08:51  gur12140
 * Removed Warnings
 *
 ****************************************************************************/

#ifndef LTE_MAC_DL_RESOURCE_ALLOCATOR_H
#define LTE_MAC_DL_RESOURCE_ALLOCATOR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacDLUEContext.h"
#include "lteMacULUEContext.h"
/* +DYNAMIC_ICIC */
#include "lteMacRRMPeriodicMgr.h"
/* -DYNAMIC_ICIC */
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/*
 * LOG2 given on basis of log(2)
 */
#define LOG2 0.693147

/* SPR 7435 Changes Starts */
/* SPR 5599 changes start (ZIP ID 129059) */
extern  UInt8 getMCSFromCQI(UInt8 cqiIndex
              );
/* SPR 5599 changes end (ZIP ID 129059) */
/* SPR 7435 Changes Ends */

/* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
extern  UInt8 getHDReducedMCSFromCQI(UInt8 cqiIndex);
#endif
/* HD FDD Changes End */

/* + CQI_4.1 */
/* SPR 5599 changes start (ZIP ID 129059) */
UInt32 FFS(UInt32 bitMap);
/* SPR 5599 changes end (ZIP ID 129059) */
/* - CQI_4.1 */
#define SORT_SUBSET_INDEX_BASED_ON_FREE_PRB(outerLoopSubsetCount, \
        subsetInfo_p, rat1Info_p, subetIndexArray) \
{  \
    UInt32 numFreePRB = 0; \
    UInt32 subsetIndex = 0; \
    UInt32 innerLoopSubsetCount = 0; \
    UInt32 count = 0; \
    while( outerLoopSubsetCount) \
    { \
        numFreePRB = (subsetInfo_p + outerLoopSubsetCount - 1)->numPRBFree; \
        subsetIndex = outerLoopSubsetCount - 1; \
        innerLoopSubsetCount = rat1Info_p->subsetCount; \
        while( innerLoopSubsetCount) \
        { \
            if ( subetIndexArray[count - 1] == ( innerLoopSubsetCount -1 )) \
            { \
                continue; \
            } \
            if ( numFreePRB < (subsetInfo_p + innerLoopSubsetCount - 1)-> \
                numPRBFree ) \
            { \
                subsetIndex = innerLoopSubsetCount - 1; \
                numFreePRB = (subsetInfo_p + innerLoopSubsetCount - 1)-> \
                numPRBFree; \
            } \
            innerLoopSubsetCount--; \
        } \
        subetIndexArray[count++] = subsetIndex; \
        outerLoopSubsetCount--; \
    } \
} 
/*Dynamic PDCCH + 5117 Changes Start*/
#define CAL_RAT0_MIB_RESRC_ASSGN_BIT_MAP(mibResrcAssignementBitMap, rat0Info_p)\
{\
    UInt32 temp = 0xFFFFFFFF;\
    temp <<= rat0Info_p->rbgCount;\
    temp = ~temp;\
    mibResrcAssignementBitMap = (temp)^(rat0Info_p->rat0BitMap[CC_USER]);\
    rat0Info_p->temp = temp;\
}
/*Dynamic PDCCH + 5117 Changes End*/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef struct ResourceAllocatorInputT
{
    /* This is array which holds the RAT policy to be used for allocating RBs*/
    RATPolicy     preferredRATPolicy; 
    /* This is pointer to structure which stores the information of SubBand 
     * Report*/
    SubBandReportInfo *subBandReportInfo_p;
    /* This stores the number of RBs to be allocated*/
    UInt8 requiredRB;
    /*This indicates if RBs need to be allocated strictly according to Subband
     *report or not*/
    UInt8 strictAllocSubBand; 
    /* This stores the subFrame Number for  which RBs need to be allocated*/
    UInt8 subFrameNum;
    /* This indicated if RBs need to be allocated according to RAT 2 
     * Distributed */ 
    UInt8 strictlyDistributedd;
    /* This indicates if the RB's can be reserved in advance .If  set to one and
     *  RB's cannot be allocated  in current SubFrame then we will try resrving 
     *  RBs in  the next SubFrame */
    UInt8 advanceResvFlag; 
    /*This Holds MCS Index to be used for Common Channel*/
    UInt8 mcsIndexForCommonChannel;

    /* ICIC changes start */
    /* + coverity 54168 to 54176 */
    UInt8 resourceRegionAllocated;
    /* - coverity 54168 to 54176 */
    /* ICIC changes end */
    /* + CQI_5.0 */
    /*This is the indication of presence/absence of subbandPMI to resource allocator*/
    UInt8 subbandPMI;
    SubbandPMIInfo *subBandPMIInfo_p;
    UInt8 aperiodicMode12;
    /* - CQI_5.0 */
} ResourceAllocatorInput;

typedef struct ResourceAllocatorOutputT
{
    /* This stores the RAT Policy which has been  used for allocating RBs */
    RATPolicy ratPolicy;
    /* This stores  the resource assignment information of allocated RBs */
    UInt32 resourceAssignmentBitMap;    
    /* Used in virtual resource block distribution
    Valid for DCI formats: 1B,1C,1D
    1= NGAP1
    2=  NGAP2   */
    UInt8 nGAP;
    /* This stores the number of allocateRB */
    UInt8 allocatedRB;
    UInt8 startingVRBIndex;    
    /* + CQI_5.0 */
    UInt8 pmiValue;
    UInt8 numberOfSubbands;
    /*SPR 5508 Start*/
    UInt8 subbandtoPMIInfo[MAX_SUBBANDS_PMI_INFO];
    /*SPR 5508 End*/
    /* - CQI_5.0 */
} ResourceAllocatorOutput;

typedef struct BitPositionToSubsetAndRBGInfoT
{
    UInt8 prbNumber;
    UInt8 rbgIndex;
    UInt32 rbgIndexBitMap;
}BitPositionToSubsetAndRBGInfo;

typedef struct SubsetInfoT
{
    BitPositionToSubsetAndRBGInfo bitPositionToSubsetAndRBGInfo[MAX_PRB_IN_SUBSET];  
    /* This holds the bitMap which stores the maximum number of addressable PRB
     *  in Subset taking span difference zero .Each set bit represents the free 
     *  RB */
    UInt32 bitMapWithSpanZero;
    /* This holds the bitMap which stores the maximum number of addressable PRB
     *  in Subset taking span difference as spanWidth .Each set bit represents 
     *  the free RB */
    UInt32 bitMapWithSpanNonZero;
    /* store the information about the free PRB in the Subset. Each bit set 
     * represent the position of free PRB in the indexed subset
     * subsetBitMap[0] represent the position of
     * free PRB in the indexed subset. subsetBitMap[1] represent
     * the position of free PRB belonging to cell edge parition in the
     * inedexed subset.
     */ 
    /* ICIC changes start */
    UInt32 subsetBitMap[MAX_USER_LOCATION];
    /* ICIC changes end */

    /* This stores the number of Free RB in the subset */
    UInt8 numPRBFree;
    /* This stores the Span witdh for the subset */
    UInt8 spanWidth;
    UInt8 maxAddressableRBCount;
}SubsetInfo;

typedef struct RAT1InfoT
{
     /*This is array of SubsetInfo Structure which stores information of free 
      * RBs for a Subset*/
     SubsetInfo subsetInfo[MAX_SUBSETS];     
     /*This stores the subset Size*/
     UInt8 subsetCount;
     UInt8 bitsCountForSubsetCount;
     UInt8 maxAddressableRBCount;
} RAT1Info;

typedef struct RAT2DistributedVRBInfoT
{
    /*Below stores the Index  of the next free VRB Index */
    UInt8 nextfreeVRBIndex;
    /*Below stores the Index  of the previous free VRB Index */
    UInt8 previousVRBfreeIndex;
    /*Below will store  the PRB number which is mapped to Even slot of the VRB*/
    UInt8 prbMappedToEvenSlotOfVRB ;
    /*Below will store  the PRB number which is mapped to Odd slot of the VRB*/
    UInt8 prbMappedToOddSlotOfVRB;   
    UInt8 numOfRBNeedToSubtracted;
    UInt32 rbgIndexBitMapForEvenVRBSlot;
    UInt32 rbgIndexBitMapForOddVRBSlot;
    UInt32 subsetBitMapForEvenVRBSlot;
    UInt32 subsetBitMapForOddVRBSlot;
    UInt32 subsetIndexForEvenVRBSlot;
    UInt32 subsetIndexForOddVRBSlot;
} RAT2DistributedVRBInfo;

typedef struct RAT2LocalizedVRBInfoT
{
    UInt8  nextfreeVRBIndex;
    /* Below will store the  Index  of the next   free VRB Index */
    UInt8  previousVRBfreeIndex;
    /*This stores the VRB number which is mapped to Even slot of PRB*/
    UInt8 vrbMappedToEvenSlotOfPRB;
    /*This stores the VRB number which is mapped to Odd slot of PRB*/
    UInt8 vrbMappedToOddSlotOfPRB;
    UInt32 rbgIndexBitMap;
    UInt32 subsetBitMap;
    UInt32 subsetIndex;
}RAT2LocalizedVRBInfo;

typedef struct RAT2LocalizedInfoT
{
    /*This ia array of RAT2LocalizedVRBInfo structure which stores information
     * of Free Localized VRB*/
    RAT2LocalizedVRBInfo rat2LocalizedVRBInfo[MAX_LOCALIZED_VRB + 3];
    /*This stores the maximum number of Free Localized VRB*/
    UInt8 maxLocalizedVRB;
    UInt8 numOfVRBAvailable;
    /* ICIC changes start */
    UInt8 numOfCellEdgeVRBAvailable;
    UInt8 firstCellEdgeFreeIndex;
    UInt8 endCellEdgeFreeIndex;
    /* ICIC changes end */
}RAT2LocalizedInfo;

typedef struct RAT2DistributedInfoT
{
    /*This ia array of RAT2DistributedVRBInfo structure which stores information
     * of Free Localized VRB*/
     RAT2DistributedVRBInfo rat2DistributedVRBInfo[MAX_DISTRIBUTED_VRB + 3];
    /*This stores the maximum number of Distributed Free VRB*/
    UInt8 maxDistributedVRB;  
    UInt8 numOfVRBAvailable;
    /* ICIC changes start */
    UInt8 numOfCellEdgeVRBAvailable;
    UInt8 firstCellEdgeFreeIndex;
    UInt8 endCellEdgeFreeIndex;
    /* ICIC changes end */
}RAT2DistributedInfo;

typedef struct RBGIndexInfoT
{
    UInt8 subsetIndex;
    UInt8 numOfRBs;
    UInt32 rbgSizeBitMapForSubset;
    UInt8 subsetIndex_2;
}RBGIndexInfo;

typedef struct RAT0InfoT
{
    /*This stores the RAT 0 BitMap in which each bit represent a RBG*/
    /* ICIC changes start */
    UInt32 rat0BitMap[MAX_USER_LOCATION];
    /* ICIC changes end */
    /*Dynamic PDCCH + 5117 Changes Start*/
    UInt32 mibResrcAssignementBitMap;
    UInt32 temp;
    /*Dynamic PDCCH + 5117 Changes End*/
    /*This holds the Resource Block Group Size*/
    UInt32 rbgSize;    
    UInt32 rbgCount;
    RBGIndexInfo rbgIndexInfo[MAX_RBG];
}RAT0Info;


typedef struct ResourceAllocationPolicyInfoT
{
    /* This  stores the information of  free RBG in case of Resource Allocation
     *  Policy 0.Each set bit  position represent the free RBG position */
    RAT0Info rat0Info;
    /* This is  structure  which stores the  the information of 
    RBs ofSubset of RAT 1 policy  and each index mapped to subset Index */
    RAT1Info rat1Info;
    /*This  is a pointer which points to structure that holds the information
     *  about the free Localized VRB in case ofRAT 2 Localized */
    RAT2LocalizedInfo rat2LocalizedInfo;
    /* This represents a structure   which stores information of Distributed VRB
     *  in case of RAT 2 Distributed */
    RAT2DistributedInfo rat2DistributedInfo;    

    
}ResourceAllocationPolicyInfo;

typedef struct PrbToVrbInfoT
{
    /* This stores the VRB which is mapped to even slot the PRB*/ 
    UInt8 vrbMappedToEvenSlotOfPRB;
    /* This stores the VRB which is mapped to odd slot the PRB*/ 
    UInt8 vrbMappedToOddSlotOfPRB;
}PrbToVrbInfo;

typedef struct BestVRBSpanInfoT
{
    /* This stores the starting VRB Index of continuous VRBs window found */
    UInt8 startingVRBIndex;
     /* This stores the ending VRB Index of continuous VRBs window found */
    UInt8 endingVRBIndex ;
}BestVRBSpanInfo;

typedef struct SubSetSpanInfoForSortingT
{
    /*This stores the subset Index*/
    UInt8 subsetIndex;
    /*This stores if Non Zerp Span has considered for Allocating RBs or not*/
    UInt8 isNonZeroSpanConsideredForAlloc;
    /*This stores number of Free RB in Subset*/
    UInt8 numFreeRB;
    /*This stores Available RB BitMap*/
    UInt32 availableRBBitMap;
}SubSetSpanInfoForSorting;



/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 /*Dynamic PDCCH + 5117 changes starts */
/* 4x4 DL MIMO CHG START */
extern McsRBInfoForNonSpatialMultiplex (*mcsRBTBSIzeInfoForNonSpatialMultiplex4x4_p)[TB_ON_LAYER_3_OR_4][MCS_27_OR_28][MAX_NUM_OF_RB];
extern McsRBInfoForSpatialMultiplex (*mcsRBTBSIzeInfoForSpatialMultiplex4x4_p)[TB_ON_LAYER_3_OR_4][MCS_27_OR_28][MAX_MCS_INDEX][MAX_NUM_OF_RB];
/* 4x4 DL MIMO CHG END */
extern McsRBInfoForNonSpatialMultiplex (*mcsRBTBSIzeInfoForNonSpatialMultiplex_p)[MAX_UE_CATEGORY][MAX_NUM_OF_RB];
extern McsRBInfoForSpatialMultiplex (*mcsRBTBSIzeInfoForSpatialMultiplex_p)[MAX_UE_CATEGORY][MAX_MCS_INDEX][MAX_NUM_OF_RB];
extern UInt32 (*maxNumberOfCodedBitsMap_p)[MAX_MOD_SCHEME][MAX_NUM_OF_RB][7];

extern UInt8 (*mcsNewTxReTxUnderUECatCodeRate_p)[MAX_UE_CATEGORY][MAX_MCS_INDEX][MAX_NUM_OF_RB];
extern UInt8 TBSArr[MAX_MCS_INDEX];
/* SPR 23259 fix start*/
extern UInt8 TBSUlArr[MAX_MCS_INDEX]; 
/* SPR 23259 fix end*/
extern UInt16 TBSizeArr[MAX_IBS_INDEX_DCI_1A + 1][MAX_RESOURCE_BLOCK];
/*4x4 DL MIMO CHG START*/
extern UInt16 TBSizeArrL2[MAX_IBS_INDEX_DCI_1A + 1][MAX_RESOURCE_BLOCK];
/*4x4 DL MIMO CHG END*/
/*Dynamic PDCCH + 5117 changes ends */
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* T2200 specific changes start*/
/* T2200 specific changes end*/

extern UInt32 uplinkRBs_g;
/* QOS PDSCH Change */
extern SInt8 excessPoolRb_g[MAX_NUM_CELL][MAX_USER_LOCATION];
/* QOS PDSCH Change */

/* Funtions Declaration */
/* SPR 5599 changes start (ZIP ID 129059) */
extern void initAllResourceAllocationPolicyInfo(InternalCellIndex internalCellIndex);
extern  MacRetType allocateRBsForSaptialMul(
/* SPR 5599 changes end (ZIP ID 129059) */
    ResourceAllocatorInput *resourceAllocInput_p,
    ResourceAllocatorOutput *resourceAllocOutput_p,
    DLUEContext *ueDLContext_p,
    DLHARQProcess *harqProcess_p,
    ResourceAllocationPolicyInfo *raPolicyInfo_p
#ifdef TDD_CONFIG
    , UInt8 subFrame
#endif
	);
#ifdef FDD_CONFIG
extern MacRetType allocateRBsForNonSaptialMul(
    ResourceAllocatorInput *resourceAllocInput,
    ResourceAllocatorOutput *resourceAllocOutput,
    DLUEContext *ueDLContext_p,
    DLHARQProcess *harqProcess_p,
    ResourceAllocationPolicyInfo *raPolicyInfo_p
	);
#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType allocateRBsForNonSaptialMul(
/* SPR 5599 changes end (ZIP ID 129059) */
        ResourceAllocatorInput *resourceAllocInput_p,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        DLUEContext *ueDLContext_p,
        DLHARQProcess *harqProcess_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt8 subFrame);
#endif

/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType allocateRBsForCommonChannel(
/* SPR 5599 changes end (ZIP ID 129059) */
        ResourceAllocatorInput *resourceAllocInput_p,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p) ;
extern void allocateResourcesForMIB(UInt32 subFrame);

/* SPR 5599 changes start (ZIP ID 129059) */
extern  MacRetType  allocRBCommonChannelRAT2Distributed(
/* SPR 5599 changes end (ZIP ID 129059) */
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DCIFormat format,
        InternalCellIndex internalCellIndex);
/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType  allocRBCommonChannelRAT2Localized(
/* SPR 5599 changes end (ZIP ID 129059) */
        ResourceAllocatorInput *resourceAllocInput_p ,
        ResourceAllocatorOutput *resourceAllocOutput_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        DCIFormat format,
        InternalCellIndex internalCellIndex);
/* SPS_CHG */
void populateResourceAssignmentInfo(
        UInt32 *resourceAssignment,
        UInt32 lengthOfVRB,
        UInt32 startingVRBIndex,
        UInt32 isDistributed,
        DCIFormat format,
        InternalCellIndex internalCellIndex);
void updateRATInfoFromRAT2LocalizedInfo(
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p,
        UInt32 allocatedRBCount,
        UInt32 updateRAT2DistributedFlag,
        /* +DYNAMIC_ICIC */
        UInt8 calculatedRNTP,
        InternalCellIndex internalCellIndex);
        /* -DYNAMIC_ICIC */

MacRetType  getBestContLocalizedVRBNewTx(UInt32 requiredRBCount,
        BestVRBSpanInfo *bestVRBSpanInfo_p,
        RAT2LocalizedInfo *rat2LocalizedInfo_p,
        UInt8 resourceRegionAllocated,
        UInt32 maxRBs,
        InternalCellIndex internalCellIndex);

 MacRetType getReportedLocalizedVRBNewTx(                                                                                                
        BestVRBSpanInfo *bestVRBSpanInfo_p, 
        RAT2LocalizedInfo *rat2LocalizedInfo_p,
        UInt32 minRBs,
        UInt32 maxRBs,
        UInt32 reportedRBGBitmap,
        UInt8 resourceRegionAllocated);

/* + CQI_5.0 */
/* SPR 5599 changes start (ZIP ID 129059) */
extern UInt32 getCount(UInt32 bitMap);
/* SPR 5599 changes end (ZIP ID 129059) */
/* - CQI_5.0 */
/*Dynamic PDCCH + 5117 Changes Start*/
/*SPR 5412 Fix Start*/
 UInt8 calculateNumOfOverLapRbs(UInt32 resrcAssignementBitMap,UInt32 rbsAllocated,
             InternalCellIndex internalCellIndex);
/*SPR 5412 Fix Start*/
/*Dynamic PDCCH + 5117 Changes End*/
/* +DYNAMIC_ICIC */
 UInt8  calculateRNTP(UInt8 pA,
        DLHARQProcess *harqProcess_p,
        InternalCellIndex internalCellIndex);
/* -DYNAMIC_ICIC */
/* + PRS_CHANGES */
 void updateRAT2DistributedInfoFromRAT2LocalizedInfo(
        RAT2LocalizedVRBInfo *localizedVRBInfo_p,
        ResourceAllocationPolicyInfo *raPolicyInfo_p);
 
 void updateRAT1InfoFromRAT2LocalizedInfo(
        SubsetInfo *subsetInfo_p,
        UInt32 subsetSize,
        UInt32 prbNumber);
/* - PRS_CHANGES */


 
#endif

